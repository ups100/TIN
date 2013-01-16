///////////////////////////////////////////////////////////
//  ClientApplication.cpp
//  Implementation of the Class ClientApplication
//  Created on:      07-gru-2012 00:33:33
//  Original author: Marcin Kubik
///////////////////////////////////////////////////////////

#include "ClientApplication.h"
#include "AliasFileList.h"
#include "FileLocation.h"
#include "CommunicationProtocol.h"
#include "FileTree.h"
#include <QDir>
#include <QBuffer>
#include <QIODevice>
#include <QIODevice>
#include <QXmlFormatter>
#include <QRegExp>
#include <QXmlStreamReader>
#include <stdexcept>

namespace TIN_project {
namespace Client {

const int ClientApplication::FILE_TIMESTAMP_INDENT = 26;
const int ClientApplication::FILE_SIZE_WIDTH = 20;

ClientApplication::ClientApplication(int argc, char **argv)
        : m_state(ClientApplication::NOT_CONNECTED),
                m_application(QString("TIN_project_Client"), argc, argv),
                m_commandParser(), m_serverConnection(this, this),
                m_DaemonCommunication(), m_view(new ClientView(this)),
                m_address(QHostAddress("0.0.0.0")), m_port(quint16(0))
{
    qRegisterMetaType<Utilities::AliasFileList>();

}

void ClientApplication::onAliasConnected()
{
    QTimer::singleShot(0, this, SLOT(onAliasConnectedSlot()));
}

void ClientApplication::onAliasConnectionError()
{
    QTimer::singleShot(0, this, SLOT(onAliasConnectionErrorSlot()));
}

void ClientApplication::onAliasCreated()
{
    QTimer::singleShot(0, this, SLOT(onAliasCreatedSlot()));
}

void ClientApplication::onAliasCreationError()
{
    QTimer::singleShot(0, this, SLOT(onAliasCreationErrorSlot()));
}

void ClientApplication::onAliasDeleted()
{
    QTimer::singleShot(0, this, SLOT(onAliasDeletedSlot()));
}

void ClientApplication::onAliasDeletionError()
{
    QTimer::singleShot(0, this, SLOT(onAliasDeletionErrorSlot()));
}

void ClientApplication::onAliasListed(
        TIN_project::Utilities::AliasFileList list)
{
    QMetaObject::invokeMethod(this, "onAliasListedSlot", Qt::AutoConnection,
            Q_ARG(TIN_project::Utilities::AliasFileList, list));
}

void ClientApplication::onConnected()
{
    QTimer::singleShot(0, this, SLOT(onConnectedSlot()));
}

void ClientApplication::onDisconnected()
{

    emit onDisconnectedSignal();
    QTimer::singleShot(0, this, SLOT(onDisconnectedSlot()));
}

void ClientApplication::onFileFound(
        TIN_project::Utilities::AliasFileList location)
{
    qDebug() << "KLIENT POINFORMOWANY";
    QMetaObject::invokeMethod(this, "onFileFoundSlot", Qt::AutoConnection,
            Q_ARG(TIN_project::Utilities::AliasFileList, location));
}

void ClientApplication::onFileNotFound()
{
    QTimer::singleShot(0, this, SLOT(onFileNotFoundSlot()));
}

void ClientApplication::onFileRemoved()
{
    QTimer::singleShot(0, this, SLOT(onFileRemovedSlot()));
}

void ClientApplication::onFileRemovingError()
{
    QTimer::singleShot(0, this, SLOT(onFileRemovingErrorSlot()));
}

void ClientApplication::onFileTransferError()
{
    emit onFileTransferSignal();
    QTimer::singleShot(0, this, SLOT(onFileTransferErrorSlot()));
}

void ClientApplication::onFileTransferFinished()
{
    emit onFileTransferSignal();
    QTimer::singleShot(0, this, SLOT(onFileTransferFinishedSlot()));
}

void ClientApplication::onFileTransferStarted()
{
    QTimer::singleShot(0, this, SLOT(onFileTransferStartedSlot()));
}

void ClientApplication::onAliasConnectedSlot()
{
    m_view->showMessage("Connected to alias");
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasConnectionErrorSlot()
{
    m_view->showMessage("Not connected to alias, error appears");
    (*this).setState(ClientApplication::CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasCreatedSlot()
{
    m_view->showMessage("Alias created");
    (*this).setState(ClientApplication::CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasCreationErrorSlot()
{
    m_view->showMessage("Alias not created, error appears");
    (*this).setState(ClientApplication::CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasDeletedSlot()
{
    m_view->showMessage("Alias deleted successfully");
    (*this).setState(ClientApplication::CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasDeletionErrorSlot()
{
    m_view->showMessage("Alias not deleted successfully");
    (*this).setState(ClientApplication::CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasListedSlot(
        TIN_project::Utilities::AliasFileList list)
{
    (*this).m_list = list;
    (*this).setState(ClientApplication::LOGGED);
    if (((*this).m_command->getCommand() == "ls")
            && ((*this).m_command->getParameter() == "l"))
        (*this).showListOfLocal(list);
    else if (((*this).m_command->getCommand() == "ls")
            && ((*this).m_command->getParameter() == "r"))
        (*this).showListOfRemote(list);
    else if (((*this).m_command->getCommand() == "ls"))
        (*this).showList(list);
    else if (((*this).m_command->getCommand() == "synch")
            && ((*this).m_command->getParameter() == "o")) {
        (*this).synchWithOverWriting(list);
    } else if (((*this).m_command->getCommand() == "synch")
            && ((*this).m_command->getParameter() == "d")) {
        (*this).showListOfConflicts(list);
    }
    QTimer::singleShot(500, &(*m_view), SLOT(reconnectNotifier()));

}

void ClientApplication::onConnectedSlot()
{
    m_view->showMessage("Connected");
    (*this).setState(ClientApplication::CONNECTED);
    qDebug() << "O TU O";
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onDisconnectedSlot()
{
    m_view->showMessage("Disconnected");
    (*this).setState(ClientApplication::NOT_CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    if ((*this).m_command->getCommand() != "disconnect")
        QTimer::singleShot(0, &m_application, SLOT(quit()));
}

void ClientApplication::onFileFoundSlot(
        TIN_project::Utilities::AliasFileList location)
{
    (*this).m_list = location;
    m_view->showMessage("File found");
    (*this).showListOfFoundFiles(location);
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileNotFoundSlot()
{
    m_view->showMessage("File not found");
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileRemovedSlot()
{
    m_view->showMessage("File removed");
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileRemovingErrorSlot()
{
    m_view->showMessage("Error while removing file");
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileTransferErrorSlot()
{
    m_view->showMessage("File transfer error");
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileTransferFinishedSlot()
{
    m_view->showMessage("File transfer finished");
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileTransferStartedSlot()
{
    m_view->showMessage("File transfer started");
    (*this).setState(ClientApplication::WAITING);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::setView(boost::shared_ptr<ClientView> view)
{

}

void ClientApplication::getCommand(QString s)
{
    boost::shared_ptr<Commands> cmd = m_commandParser.parseCommand(s);

    if ((!(*this).checkIntegrity(cmd)) || (!(*this).checkStateCondition(cmd))) {
        qDebug() << "SPRAWDZENIE POPRAWNOSCI NEGATYWNE";
        QTimer::singleShot(1000, &(*m_view), SLOT(reconnectNotifier()));
        return;
    } else {
        qDebug() << "WYKONUJEMY";
        (*this).invokeCommand(cmd);
    }

}

bool ClientApplication::invokeCommand(boost::shared_ptr<Commands> cmd)
{

    /**
     * Here i'm trying to prevent user from random deletion, when
     * he type push or pull with number and does not know, which file
     * it is. So i'm trying to check whether the previous command
     * allows user to get the info about indexes
     */
    if ((cmd->getCommand() == "choose")
            && (!((m_command->getCommand() == "choose")
                    || ((m_command->getCommand() == "synch")
                            && (m_command->getParameter() == "o"))))) {
        qDebug() << "You cannot invoke this method in this state. Type 'help'";
        QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
        return false;
    }
    if ((cmd->getCommand() == "push")
            && (!((m_command->getCommand() == "push")
                    || ((m_command->getCommand() == "ls")
                            && (m_command->getParameter() == "l"))))) {
        qDebug() << "You cannot invoke this method in this state. Type 'help'";
        QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
        return false;
    }
    if ((cmd->getCommand() == "pull")
            && (!((m_command->getCommand() == "pull")
                    || ((m_command->getCommand() == "ls")
                            && (m_command->getParameter() == "r"))))) {
        qDebug() << "You cannot invoke this method in this state. Type 'help'";
        QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
        return false;
    }
    if ((cmd->getCommand() == "read")
            && (!((m_command->getCommand() == "read")
                    || ((m_command->getCommand() == "ls")
                            && (m_command->getParameter() == "r"))))) {
        qDebug() << "You cannot invoke this method in this state. Type 'help'";
        QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
        return false;
    }

    /**
     * End of checking the previous command, maybe it will be deleted later
     */

    m_command = cmd;

    if (cmd->getCommand() == "exit") {
        (*this).setState(ClientApplication::WAITING_FOR_DISCONNECT);
        m_serverConnection.disconnectFromServer();

    } else if (cmd->getCommand() == "disconnect") {
        (*this).setState(ClientApplication::WAITING_FOR_DISCONNECT);
        m_serverConnection.disconnectFromServer();
        QEventLoop loop;
        QObject::connect(this, SIGNAL(onDisconnectedSignal()), &loop,
                SLOT(quit()));
        loop.exec();
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.connectToServer((*this).m_address, (*this).m_port);

    } else if (cmd->getCommand() == "log") {
        (*this).m_alias = cmd->getArg();
        (*this).m_password = cmd->getPassword();
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.connectToAlias(cmd->getArg(), cmd->getPassword(),
                Utilities::Identifier(
                        Utilities::Identify::getMachineIdentificator()));

    } else if (cmd->getCommand() == "create") {

        (*this).m_alias = cmd->getArg();
        (*this).m_password = cmd->getPassword();
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.createAlias(cmd->getArg(), cmd->getPassword());

    } else if (cmd->getCommand() == "add") {
        m_path = cmd->getArg2();
        Utilities::CommunicationProtocol::Communicate<
                Utilities::CommunicationProtocol::ADD_DIRECTORY_AND_CONNECT> message(
                Utilities::Message(cmd->getArg(), cmd->getPassword(),
                        cmd->getArg2(), m_address, m_port));
        QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
        m_DaemonCommunication.talkToDaemon(message.toQByteArray());

    } else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "")) {
        Utilities::CommunicationProtocol::Communicate<
                Utilities::CommunicationProtocol::REMOVE_DIRECTORY_AND_DISCONNECT> message(
                Utilities::Message(cmd->getArg(), cmd->getArg2()));
        QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
        m_DaemonCommunication.talkToDaemon(message.toQByteArray());

    } else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "a")) {
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.removeAlias(cmd->getArg(), cmd->getPassword());

    } else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "f")) {
        (*this).setState(ClientApplication::WAITING);
        (*this).m_serverConnection.removeFileFromAlias(cmd->getArg());

    } else if ((cmd->getCommand() == "find")) {
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.findFileInAlias(cmd->getArg());

    } else if (cmd->getCommand() == "read") {
        (*this).setState(ClientApplication::WAITING);
        (*this).invokeCommandByIndex((*this).m_list, cmd->getArg(),
                cmd->getCommand());

    } else if (cmd->getCommand() == "push") {
        (*this).setState(ClientApplication::WAITING);
        (*this).invokeCommandByIndex((*this).m_list, cmd->getArg(),
                cmd->getCommand());

    } else if ((cmd->getCommand() == "ls") && (cmd->getParameter() == "r")) {
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.listAlias(true);
    }
    else if ((cmd->getCommand() == "ls") && (cmd->getParameter() == "l")) {

        (*this).listLocalPath();
    }
    else if (cmd->getCommand() == "ls") {
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.listAlias();
    } else if ((cmd->getCommand() == "synch") && (cmd->getParameter() == "o")) {
        m_serverConnection.listAlias();
        (*this).setState(ClientApplication::WAITING);

    } else if ((cmd->getCommand() == "synch") && (cmd->getParameter() == "d")) {
        (*this).setState(ClientApplication::WAITING);
        m_serverConnection.listAlias();

    } else if ((cmd->getCommand() == "choose")) {
        (*this).setState(ClientApplication::WAITING);
        (*this).invokeCommandByIndex((*this).m_list, cmd->getArg(),
                cmd->getCommand());

    } else if ((cmd->getCommand() == "pull")) {
        (*this).setState(ClientApplication::WAITING);
        (*this).invokeCommandByIndex((*this).m_list, cmd->getArg(),
                cmd->getCommand());

    } else if (cmd->getCommand() == "change") {
        (*this).changeRootPath(cmd->getArg());
        QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));

    }
    return true;
}

int ClientApplication::start(const QHostAddress& address, quint16 port,
        QString path)
{
    if (m_application.isRunning()) {
        qDebug() << "Another client application is running" << endl;
        return -1;
    }

    // Pass empty message to start Daemon if not running
    m_DaemonCommunication.talkToDaemon(QByteArray("00000"));

    qDebug() << "Client application started" << endl;
    m_address = address;
    m_port = port;
    (*this).setState(ClientApplication::WAITING);
    m_serverConnection.connectToServer(address, port);
    m_application.exec();

    return 0;
}

void ClientApplication::setState(ClientApplication::States s)
{
    m_state = s;
}

ClientApplication::States ClientApplication::getState() const
{
    return m_state;
}

bool ClientApplication::checkIntegrity(boost::shared_ptr<Commands> cmd) const
{
    if (!cmd->isCorrect())
        return false;
    if (cmd->getCommand() == "log") {
        //NOTHING??
    }
    if (cmd->getCommand() == "change") {
        return (*this).checkAbsolutePath(cmd->getArg());
    } else if (cmd->getCommand() == "add") {
        return ((*this).checkAbsolutePath(cmd->getArg2())
                && (!(*this).checkIfConfigFileExists(cmd->getArg2())));
    }
    if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "")) {
        qDebug() << "TU WCHODZIMY";
        qDebug()
                << (*this).checkIntegrityOfConfigFile(cmd->getArg2(),
                        cmd->getArg(), cmd->getPassword());
        return ((*this).checkAbsolutePath(cmd->getArg2())
                && ((*this).checkIntegrityOfConfigFile(cmd->getArg2(),
                        cmd->getArg(), cmd->getPassword())));
    } else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "f"))
        return (*this).checkRelativePath(cmd->getArg());
    if ((cmd->getCommand() == "choose")
            || ((cmd->getCommand() == "push") && (cmd->getCommand() == "pull"))
            || (cmd->getCommand() == "read")) {
        bool number;
        cmd->getArg().toInt(&number, 10);
        return number;
    }
    return true;
}

bool ClientApplication::checkStateCondition(
        boost::shared_ptr<Commands> cmd) const
{

    ClientApplication::States state = (*this).getState();
    if (state == ClientApplication::NOT_CONNECTED)
        return false;
    if (state == ClientApplication::WAITING)
        qDebug() << "We shouldn't be here at all";
    if (state == ClientApplication::WAITING_FOR_DISCONNECT)
        qDebug() << "We shouldn't be here at all";

    if (state == ClientApplication::CONNECTED) {
        if (cmd->getCommand() == "log")
            return true;
        else if (cmd->getCommand() == "create")
            return true;
        else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "a")) {
            qDebug() << "CHECK STATE CONDITION ZWRACA TRUE";
            return true;
        }
        //       return true;
        else if ((cmd->getCommand() == "add"))
            return true;
        else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == ""))
            return true;
        else if (cmd->getCommand() == "choose")
            return true;
        else if (cmd->getCommand() == "change")
            return true;
        else
            return false;
    }
    if (state == ClientApplication::LOGGED) {
        if (cmd->getCommand() == "log")
            return false;
        else if (cmd->getCommand() == "create")
            return false;
        else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "a"))
            return false;
        else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == ""))
            return false;
        else if ((cmd->getCommand() == "add") && (cmd->getParameter() == ""))
            return false;
        else
            return true;
    }
    return false;
}

bool ClientApplication::checkRelativePath(QString s) const
{
    //NOTHING TO DO HERE
    QString tmpDir = m_path;
    tmpDir.append(QDir::separator());
    tmpDir.append(s);
    qDebug() << tmpDir;
    QFile file(tmpDir);
    //return file.exists();
    return true;
}

bool ClientApplication::checkAbsolutePath(QString s) const
{
    QDir dir(s);
    return dir.exists();
    //return true;
}

bool ClientApplication::checkIfConfigFileExists(QString cnfPath) const
{
    QString path = cnfPath;
    path.append(QDir::separator());
    path.append(ConfigFileName::CONFIG_FILE_NAME);
    QFile file(path);
    qDebug() << path;
    return (file.exists());
}

bool ClientApplication::checkIntegrityOfConfigFile(QString path, QString alias,
        Password pass) const
{
    QString dir = path;
    dir.append(QDir::separator());
    dir.append(ConfigFileName::CONFIG_FILE_NAME);
    QFile file(dir);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    qDebug() << "SCIEZKA TO " << dir;
    QString tmpAlias = file.readLine().simplified();
    QString tmpPass = QByteArray::fromHex(file.readLine());
    qDebug() << "TMP ALIAS TO " << tmpAlias;
    qDebug() << "ALIAS TO " << alias;
    qDebug() << "PIERWSZE INFO TO" << (tmpAlias == alias);
    qDebug() << "DRUGIE INFO TO " << (pass.getHash() == tmpPass.simplified());
    qDebug() << "TRZECIE INFO TO " << (pass.getHash() == tmpPass);
    qDebug() << "PASS TO " << pass.getHash();
    qDebug() << "TMP PASS TO " << tmpPass.simplified();
    file.close();
    return ((tmpAlias == alias) && (pass.getHash() == tmpPass));
    //return true;
}
void ClientApplication::synchWithOverWriting(
        const Utilities::AliasFileList & list)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;

    (*this).moveOnTreeAutoSynch(tree, 0, counter);
    (*this).setState(ClientApplication::LOGGED);
    m_view->showMessage("End of synchronization");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));

}

void ClientApplication::moveOnTreeAutoSynch(boost::shared_ptr<AliasTree> tree,
        int indent, int & counter)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();
    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            int index = 0;
            long int maxDate =
                    m_tree->getFileLocations().first().m_date.toLong();
            int maxSize = m_tree->getFileLocations().first().m_size;
            if (m_tree->getFileLocations().size() == 1)
                continue;

            for (int j = 0; j < m_tree->getFileLocations().size(); ++j) {
                if ((m_tree->getFileLocations()[j].m_date.toLong() > maxDate)
                        && (m_tree->getFileLocations()[j].m_size != maxSize)) {
                    maxDate = m_tree->getFileLocations()[j].m_date.toLong();
                    maxSize = m_tree->getFileLocations()[j].m_size;
                    index = j;
                }
            }

            QString tmpPath = m_path + "/" + m_tree->getPath();
            QFile file(tmpPath);
            qDebug() << "SCIEZKA TO " << tmpPath;
            if ((file.exists()) && (file.size() != maxSize)) {
                m_serverConnection.pullFileFrom(
                        FileLocation(QString(m_tree->getPath()),
                                m_tree->getFileLocations()[index].m_size,
                                Identifier(
                                        m_tree->getFileLocations()[index].m_id,
                                        m_tree->getPath())));
                QEventLoop loop;
                QObject::connect(this, SIGNAL(onFileTransferSignal()), &loop,
                        SLOT(quit()));
                loop.exec();

            }

            if (m_tree->getFileLocations()[index].m_id
                    == Identify::getMachineIdentificator()) {
                qDebug() << m_tree->getPath() << "PUSH"
                        << m_tree->getFileLocations()[index].m_date;
                m_serverConnection.pushFileToAlias(QString(m_tree->getPath()),
                        m_tree->getFileLocations()[index].m_size);
                QEventLoop loop;
                QObject::connect(this, SIGNAL(onFileTransferSignal()), &loop,
                        SLOT(quit()));
                loop.exec();
            } else {
                qDebug() << m_tree->getPath() << "PULL"
                        << m_tree->getFileLocations()[index].m_date;
                m_serverConnection.pullFileFrom(
                        FileLocation(QString(m_tree->getPath()),
                                m_tree->getFileLocations()[index].m_size,
                                Identifier(
                                        m_tree->getFileLocations()[index].m_id,
                                        m_tree->getPath())));
                QEventLoop loop;
                QObject::connect(this, SIGNAL(onFileTransferSignal()), &loop,
                        SLOT(quit()));
                loop.exec();
            }

            /** TODO Seems to be useless here */
            qDebug() << counter;
            counter++;

        } else {
            moveOnTreeAutoSynch(m_tree, indent + 1, counter);
        }
    }
}

void ClientApplication::showList(const Utilities::AliasFileList & list)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;
    (*this).moveOnTreeShowList(tree, 0, counter);
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    return;
}

void ClientApplication::moveOnTreeShowList(boost::shared_ptr<AliasTree> tree,
        int indent, int & counter)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();
    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            for (int i = 0; i < m_tree->getFileLocations().size(); ++i) {
                qint64 date =
                        (m_tree->getFileLocations()[i].m_date).toLongLong();

                std::cout
                        << (QDateTime::fromMSecsSinceEpoch(date).toString(
                                "hh:mm:ss dd/MM/yyyy")).toStdString()
                        << " Size: ";

                QString t = QString::number(
                        m_tree->getFileLocations()[i].m_size) + " bytes";
                std::cout.width(FILE_SIZE_WIDTH);
                std::cout << std::left << t.toStdString().c_str();

                std::cout.width(indent * 4);
                std::cout << " ";
                std::cout << m_tree->getFilename().toStdString() << "\n";
            }
        } else {
            std::cout.width(FILE_TIMESTAMP_INDENT + FILE_SIZE_WIDTH);
            std::cout << " ";

            std::cout.width(indent * 4);
            std::cout << " ";

            std::cout << "/" << m_tree->getFilename().toStdString() << "\t"
                    << "\n";
            (*this).moveOnTreeShowList(m_tree, indent + 1, counter);
        }
    }
}

void ClientApplication::showListOfConflicts(const AliasFileList & list)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;
    (*this).moveOnTreeShowListOfConflicts(tree, 0, counter);
    (*this).setState(ClientApplication::LOGGED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    return;
}

void ClientApplication::moveOnTreeShowListOfConflicts(
        boost::shared_ptr<AliasTree> tree, int indent, int & counter)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();

    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            if (m_tree->getFileLocations().size() == 1)
                continue;

            for (int j = 0; j < m_tree->getFileLocations().size(); ++j) {
                qint64 date =
                        (m_tree->getFileLocations()[i].m_date).toLongLong();

                std::cout
                        << (QDateTime::fromMSecsSinceEpoch(date).toString(
                                "hh:mm:ss dd/MM/yyyy")).toStdString()
                        << " Size: ";

                QString t = QString::number(
                        m_tree->getFileLocations()[i].m_size) + " bytes";
                std::cout.width(FILE_SIZE_WIDTH);
                std::cout << std::left << t.toStdString().c_str();

                std::cout.width(indent * 4);
                std::cout << " ";
                std::cout << "[" << counter++ << "] ";
                std::cout << m_tree->getFilename().toStdString() << "\n";
            }
        } else {
            std::cout.width(FILE_TIMESTAMP_INDENT + FILE_SIZE_WIDTH);
            std::cout << " ";

            std::cout.width(indent * 4);
            std::cout << " ";

            std::cout << "/" << m_tree->getFilename().toStdString() << "\t"
                    << "\n";
            (*this).moveOnTreeShowListOfConflicts(m_tree, indent + 1, counter);
        }
    }
}

void ClientApplication::showListOfRemote(const AliasFileList& list)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;
    (*this).moveOnTreeShowListOfRemote(tree, 0, counter);
    (*this).setState(ClientApplication::LOGGED);
    //QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    return;
}

void ClientApplication::moveOnTreeShowListOfRemote(
        boost::shared_ptr<AliasTree> tree, int indent, int & counter)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();
    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            for (int i = 0; i < m_tree->getFileLocations().size(); ++i) {
                if (m_tree->getFileLocations()[i].m_id
                        != Identify::getMachineIdentificator()) {
                    qint64 date = (m_tree->getFileLocations()[i].m_date)
                            .toLongLong();

                    std::cout
                            << (QDateTime::fromMSecsSinceEpoch(date).toString(
                                    "hh:mm:ss dd/MM/yyyy")).toStdString()
                            << " Size: ";

                    QString t = QString::number(
                            m_tree->getFileLocations()[i].m_size) + " bytes";
                    std::cout.width(FILE_SIZE_WIDTH);
                    std::cout << std::left << t.toStdString().c_str();

                    std::cout.width(indent * 4);
                    std::cout << " ";
                    std::cout << "[" << counter++ << "] ";
                    std::cout << m_tree->getFilename().toStdString() << "\n";
                }
            }
        } else {
            std::cout.width(FILE_TIMESTAMP_INDENT + FILE_SIZE_WIDTH);
            std::cout << " ";

            std::cout.width(indent * 4);
            std::cout << " ";

            std::cout << "/" << m_tree->getFilename().toStdString() << "\t"
                    << "\n";
            (*this).moveOnTreeShowListOfRemote(m_tree, indent + 1, counter);
        }
    }
}

void ClientApplication::showListOfLocal(const AliasFileList& list)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;
    (*this).moveOnTreeShowListOfLocal(tree, 0, counter);
    (*this).setState(ClientApplication::LOGGED);
    //QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    return;
}

void ClientApplication::moveOnTreeShowListOfLocal(
        boost::shared_ptr<AliasTree> tree, int indent, int & counter)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();
    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            for (int i = 0; i < m_tree->getFileLocations().size(); ++i) {
                if (m_tree->getFileLocations()[i].m_id
                        == Identify::getMachineIdentificator()) {
                    qint64 date = (m_tree->getFileLocations()[i].m_date)
                            .toLongLong();

                    std::cout
                            << (QDateTime::fromMSecsSinceEpoch(date).toString(
                                    "hh:mm:ss dd/MM/yyyy")).toStdString()
                            << " Size: ";

                    QString t = QString::number(
                            m_tree->getFileLocations()[i].m_size) + " bytes";
                    std::cout.width(FILE_SIZE_WIDTH);
                    std::cout << std::left << t.toStdString().c_str();

                    std::cout.width(indent * 4);
                    std::cout << " ";
                    std::cout << "[" << counter++ << "] ";
                    std::cout << m_tree->getFilename().toStdString() << "\n";
                }
            }
        } else {
            std::cout.width(FILE_TIMESTAMP_INDENT + FILE_SIZE_WIDTH);
            std::cout << " ";

            std::cout.width(indent * 4);
            std::cout << " ";

            std::cout << "/" << m_tree->getFilename().toStdString() << "\t"
                    << "\n";
            (*this).moveOnTreeShowListOfLocal(m_tree, indent + 1, counter);
        }
    }
}

void ClientApplication::invokeCommandByIndex(Utilities::AliasFileList & list,
        QString ind, QString command)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;
    bool ok;
    int index = ind.toInt(&ok, 10);
    qDebug() << index << command;
    if (ok)
        (*this).moveOnTreeIndex(tree, 0, counter, index, command);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    return;
}

void ClientApplication::moveOnTreeIndex(boost::shared_ptr<AliasTree> tree,
        int indent, int & counter, int index, QString command)
{

    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();

    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {

            if ((command == "choose")
                    && (m_tree->getFileLocations().size() == 1))
                continue;
            for (int j = 0; j < m_tree->getFileLocations().size(); ++j) {

                if ((command == "push")
                        && (m_tree->getFileLocations()[j].m_id
                                == Identify::getMachineIdentificator())) {

                    if (index == counter) {
                        qDebug() << "ZNALEZIONO " << m_tree->getPath();

                        m_serverConnection.pushFileToAlias(m_tree->getPath(),
                                m_tree->getFileLocations()[j].m_size);
                        (*this).setState(ClientApplication::WAITING);
                    }
                    counter++;

                } else if ((command == "pull" || command == "read")
                        && (m_tree->getFileLocations()[j].m_id
                                != Identify::getMachineIdentificator())) {
                    if (index == counter) {
                        qDebug() << "ZNALEZIONO " << m_tree->getPath();
                        m_serverConnection.pullFileFrom(
                                FileLocation(m_tree->getPath(),
                                        m_tree->getFileLocations()[j].m_size,
                                        Identifier(
                                                m_tree->getFileLocations()[j]
                                                        .m_id,
                                                m_tree->getPath())));
                        (*this).setState(ClientApplication::WAITING);
                    }
                    counter++;
                } else if (command == "choose") {
                    if (m_tree->getFileLocations()[j].m_id
                            == Identify::getMachineIdentificator()) {
                        if (index == counter) {
                            qDebug() << "ZNALEZIONO " << m_tree->getPath();
                            m_serverConnection.pushFileToAlias(
                                    m_tree->getPath(),
                                    m_tree->getFileLocations()[j].m_size);
                            (*this).setState(ClientApplication::WAITING);
                        }
                        counter++;
                    } else {

                        if (index == counter) {
                            qDebug() << "ZNALEZIONO " << m_tree->getPath();
                            m_serverConnection.pullFileFrom(
                                    FileLocation(m_tree->getPath(),
                                            m_tree->getFileLocations()[j].m_size,
                                            Identifier(
                                                    m_tree->getFileLocations()[j]
                                                            .m_id,
                                                    m_tree->getPath())));
                            (*this).setState(ClientApplication::WAITING);
                        }
                        counter++;

                    }
                }

                //counter++;
            }
        } else {
            (*this).moveOnTreeIndex(m_tree, indent + 1, counter, index,
                    command);
        }
    }
}

void ClientApplication::showListOfFoundFiles(const AliasFileList& list)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;
    (*this).moveOnTreeShowFoundFiles(tree, 0, counter);
    (*this).setState(ClientApplication::LOGGED);
    //QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    return;
}

void ClientApplication::moveOnTreeShowFoundFiles(
        boost::shared_ptr<AliasTree> tree, int indent, int & counter)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();
    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            for (int i = 0; i < m_tree->getFileLocations().size(); ++i) {
                qint64 date =
                        (m_tree->getFileLocations()[i].m_date).toLongLong();
                std::cout << m_tree->getPath().toStdString() << "\t"
                        << (QDateTime::fromMSecsSinceEpoch(date).toString(
                                "hh:mm:ss dd/MM/yyyy")).toStdString() << "\t"
                        << "Size: " << m_tree->getFileLocations()[i].m_size
                        << " bytes" << "\t"
                        << ((m_tree->getFileLocations()[i].m_id
                                == Identify::getMachineIdentificator()) ?
                                "Local computer" : "Remote Computer") << "\n";
            }
        } else {
            (*this).moveOnTreeShowFoundFiles(m_tree, indent, counter);
        }
    }
}

void ClientApplication::changeRootPath(QString s)
{
    QString dir = s;
    dir.append(QDir::separator());
    dir.append(ConfigFileName::CONFIG_FILE_NAME);

    QFile file(dir);
    if (file.exists())
        m_path = s;
    else
        m_view->showMessage(
                "Cannot change to given path, no config file there");
    (*this).setState(ClientApplication::CONNECTED);
    QTimer::singleShot(1000, &(*m_view), SLOT(reconnectNotifier()));
}
ClientApplication::~ClientApplication()
{

}

Utilities::AliasFileList ClientApplication::listLocalPath()
{
    QDir dir(m_path);
    if (!dir.exists())
        throw std::runtime_error("Path doesn't exit.");

    // Recursive read alias catalogue
    QXmlNamePool namePool;
    Utilities::FileTree fileTree(namePool);
    QXmlNodeModelIndex fileNode = fileTree.nodeFor(m_path);

    QXmlQuery query(namePool);
    query.bindVariable("fileTree", fileNode);
    query.setQuery("$fileTree");

    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);

    QXmlFormatter formatter(query, &buffer);
    query.evaluateTo(&formatter);

    // Remove absolute path, make it relative
    QString str(output.data());
    str = str.replace(QRegExp(QString() + m_path + "/?"), "/");

    // Create AliasFileList object to send it after
    Utilities::AliasFileList atree;

    QXmlStreamReader reader(str);
    reader.readNext();

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "directory") {
            } else if (reader.name() == "file") {
                QXmlStreamAttributes attribs = reader.attributes();

                // Ommit daemon temporary file
                if (attribs.value("fileName").toString()
                        != Utilities::ConfigFileName::CONFIG_FILE_NAME) {

                    atree.addFile(attribs.value("filePath").toString(),
                            attribs.value("lastModified").toString(),
                            attribs.value("size").toString().toUInt());
                }
            }
        }

        reader.readNext();
    }

    return atree;
}

} //namespace Client
} //namespace TIN_project
