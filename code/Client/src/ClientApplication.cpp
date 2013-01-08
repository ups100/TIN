///////////////////////////////////////////////////////////
//  ClientApplication.cpp
//  Implementation of the Class ClientApplication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientApplication.h"
#include "AliasFileList.h"
#include "FileLocation.h"
#include "CommunicationProtocol.h"

const static QString NAME_OF_FILE = "/lua_tutorial.txt";
namespace TIN_project {
namespace Client {

ClientApplication::ClientApplication(int argc, char **argv)
        : m_state(ClientApplication::NOT_CONNECTED),
                m_application(QString("TIN_project_Client"), argc, argv),
                m_commandParser(), m_serverConnection(this, this),
                m_DaemonCommunication(), m_view(new ClientView(this)),
                m_address(QHostAddress("0.0.0.0")), m_port(quint16(0))

{

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
    QTimer::singleShot(0, this, SLOT(onAliasConnectedSlot()));
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

void ClientApplication::onAliasListed(const Utilities::AliasFileList& list)
{
    QMetaObject::invokeMethod(this, "onAliasListedSlot", Qt::AutoConnection,
            Q_ARG(const AliasFileList&, list));
        }

void ClientApplication::onConnected()
{
    QTimer::singleShot(0, this, SLOT(onConnectedSlot()));
}

void ClientApplication::onDisconnected()
{
    QTimer::singleShot(0, this, SLOT(onDisconnectedSlot()));
}

void ClientApplication::onFileFound(const Utilities::AliasFileList& location)
{
    //TODO
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
    QTimer::singleShot(0, this, SLOT(onFileTransferErrorSlot()));
}

void ClientApplication::onFileTransferFinished()
{
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

void ClientApplication::onAliasListedSlot(const Utilities::AliasFileList& list)
{
    (*this).setState(ClientApplication::FILELIST);
    (*this).m_list = list;
    if ((*this).m_command->getCommand() == "ls")
        //m_view->showList(const_cast<Utilities::AliasFileList&>(list));
        (*this).showList(list, true);
    else if (((*this).m_command->getCommand() == "synch")
            && ((*this).m_command->getParameter() == "o")) {
        (*this).synchWithOverWriting(list);
    } else if (((*this).m_command->getCommand() == "synch")
            && ((*this).m_command->getParameter() == "d")) {
        (*this).showList(list, false);
        //(*this).synchWithDecisions(list);
    }
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onConnectedSlot()
{
    m_view->showMessage("Connected");
    (*this).setState(ClientApplication::CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onDisconnectedSlot()
{
    m_view->showMessage("Disconnected");
    (*this).setState(ClientApplication::NOT_CONNECTED);
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileFoundSlot(const Utilities::FileLocation& location)
{
    //TODO
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
    /**
     * This singleShot shouldn't be here, but otherwise it wouldn't work at all
     * reconnectNotifier should be invoked by slots
     */

    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
    if ((!(*this).checkIntegrity(cmd)) || (!(*this).checkStateCondition(cmd)))
        return;

    else {
        qDebug() << "WYKONUJEMY";
        (*this).invokeCommand(cmd);
    }

}

bool ClientApplication::invokeCommand(boost::shared_ptr<Commands> cmd)
{
    m_command = cmd;
    if (cmd->getCommand() == "exit") {
        //(*this).setState(ClientApplication::WAITING_FOR_DISCONNECT);
        m_serverConnection.disconnectFromServer();
    } else if (cmd->getCommand() == "disconnect") {
        //(*this).setState(ClientApplication::WAITING_FOR_DISCONNECT);
        m_serverConnection.disconnectFromServer();
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.connectToAlias((*this).m_alias, (*this).m_password);
    } else if (cmd->getCommand() == "log") {
        (*this).m_alias = cmd->getArg();
        (*this).m_password = cmd->getPassword();
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.connectToServer((*this).m_address, (*this).m_port);
    } else if (cmd->getCommand() == "create") {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.createAlias(cmd->getArg(), cmd->getPassword());
    } else if (cmd->getCommand() == "add") {
        //(*this).setState(ClientApplication::WAITING);
        Utilities::CommunicationProtocol::Communicate<
                Utilities::CommunicationProtocol::ADD_DIRECTORY_AND_CONNECT> message(
                Utilities::Message(m_alias, m_password, cmd->getArg(),
                        m_address, m_port));
        m_DaemonCommunication.talkToDaemon(message.toQByteArray());
    } else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "d")) {
        //(*this).setState(ClientApplication::WAITING);
        Utilities::CommunicationProtocol::Communicate<
                Utilities::CommunicationProtocol::REMOVE_DIRECTORY_AND_DISCONNECT> message(
                Utilities::Message(m_alias, cmd->getArg()));
        m_DaemonCommunication.talkToDaemon(message.toQByteArray());
    } else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "a")) {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.removeAlias(cmd->getArg(), cmd->getPassword());
    } else if ((cmd->getCommand() == "rm")) {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.removeFileFromAlias(cmd->getArg());
    } else if ((cmd->getCommand() == "find")) {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.findFileInAlias(cmd->getArg());
    } else if (cmd->getCommand() == "read") {
        //(*this).setState(ClientApplication::WAITING);
        //TODO SWITCH QSTRING TO NUMBER AND INVOKE MY FUNCTION

    } else if (cmd->getCommand() == "push") {
        //(*this).setState(ClientApplication::WAITING);
        //TODO SWITCH QSTRING TO NUMBER AND INVOKE MY FUNCTION
    } else if (cmd->getCommand() == "ls") {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.listAlias();
    } else if ((cmd->getCommand() == "synch") && (cmd->getParameter() == "o")) {
        //(*this).setState(ClientApplication::WAITING);
    } else if ((cmd->getCommand() == "synch") && (cmd->getParameter() == "d")) {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.listAlias();
    } else if ((cmd->getCommand() == "choose")) {
        //(*this).setState(ClientApplication::WAITING);
        //TODO SWITCH QSTRING TO NUMBER AND INVOKE MY COMMAND
    } else if ((cmd->getCommand() == "pull")) {
        //(*this).setState(ClientApplication::WAITING);
        //INVOKE MY COMMAND
        //TODO SWITCH QSTRING TO NUMBER
    }

    return true;
}

int ClientApplication::start(const QHostAddress& address, quint16 port)
{
    if (m_application.isRunning()) {
        qDebug() << "Another client application is running" << endl;
        return -1;
    }
    qDebug() << "Client application started" << endl;

    /** Server should be working to set state to waiting */
//(*this).setState(ClientApplication::WAITING);
    m_address = address;
    m_port = port;
    m_serverConnection.connectToServer(address, port);

    return m_application.exec();

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
        return (*this).checkIfConfigFileExists();
    }

    if ((cmd->getCommand() == "add")
            || ((cmd->getCommand() == "rm") && (cmd->getParameter() == "d"))) {
        return (*this).checkAbsolutePath(cmd->getArg());
    }
    if (((cmd->getCommand() == "rm") && (cmd->getParameter() == ""))
            || (cmd->getCommand() == "push")) {
        return (*this).checkRelativePath(cmd->getArg());
    }
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
        else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "a"))
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
        else if ((cmd->getCommand() == "choose"))
            return false;
        else
            return true;
    }
    if (state == ClientApplication::FILELIST) {
        if (cmd->getCommand() == "log")
            return false;
        else if (cmd->getCommand() == "create")
            return false;
        else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "a"))
            return false;
        else if (((*this).m_command->getCommand() == "ls")
                && (cmd->getCommand() == "choose"))
            return false;
        else
            return true;
    }
    return false;
}

bool ClientApplication::checkRelativePath(QString s) const
{
    QString tmpDir = QDir::currentPath();
    tmpDir.append(QDir::separator());
    tmpDir.append(s);
    qDebug() << tmpDir;
    return true;
}

bool ClientApplication::checkAbsolutePath(QString s) const
{
    QFile file(s);
    return file.exists();
}

bool ClientApplication::checkIfConfigFileExists() const
{
    //This should be finally
    //Checks if the file exists

    /*QString path = QDir::currentPath();
     path.append(QDir::separator());
     path.append(ConfigFileName::CONFIG_FILE_NAME);
     QFile file(path);
     return (file.exists());*/
    return true;
}

bool ClientApplication::synchWithOverWriting(
        const Utilities::AliasFileList & list)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;

    (*this).moveOnTreeAutoSynch(tree, 0, counter);
    (*this).setState(ClientApplication::LOGGED);
    return true;
}

bool ClientApplication::invokeCommandByIndex(
        const Utilities::AliasFileList & list, int index, QString command)
{

    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;

    (*this).moveOnTreeIndex(tree, 0, counter, index, command);
    return true;
}

void ClientApplication::moveOnTreeAutoSynch(boost::shared_ptr<AliasTree> tree,
        int indent, int & counter)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();
    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            int index = 0;
            long int max = m_tree->getFileLocations().first().m_date.toLong();
            for (int j = 0; j < m_tree->getFileLocations().size(); ++j) {
                if (m_tree->getFileLocations()[j].m_date.toLong() > max) {
                    max = m_tree->getFileLocations()[j].m_date.toLong();
                    index = j;
                }
            }
            if (m_tree->getFileLocations()[index].m_id.toStdString()
                    == Identify::getMachineIdentificator().toStdString()) {

                m_serverConnection.pushFileToAlias(QString(m_tree->getPath()),
                        m_tree->getFileLocations()[index].m_size);
            } else {
                m_serverConnection.pullFileFrom(
                        FileLocation(QString(m_tree->getPath()),
                                m_tree->getFileLocations()[index].m_id));
            }

            /** TODO Seems to be useless here */
            qDebug()<<counter;
            counter++;

            /*for (int i = 0; i < m_tree->getFileLocations().size(); ++i) {
             std::cout.width(indent * 4);
             std::cout << " ";
             std::cout << m_tree->getFilename().toStdString() << "["
             << (counter++) << "]" << "\t"
             << m_tree->getFileLocations()[i].m_date.toStdString()
             << "\t" << m_tree->getFileLocations()[i].m_size << "\t"
             << m_tree->getFileLocations()[i].m_id.toStdString()
             << "\n";
             }*/
        } else {
            /*std::cout.width(indent * 4);
             std::cout << " ";
             std::cout << m_tree->getFilename().toStdString() << "\t" << "\n";*/
            moveOnTreeAutoSynch(m_tree, indent + 1, counter);

        }
    }
}

void ClientApplication::showList(const Utilities::AliasFileList & list, bool v)
{
    boost::shared_ptr<AliasTree> tree(new AliasTree(list.getTree()));
    int counter = 1;
    (*this).moveOnTreeShowList(tree, 0, counter, v);
    return;
}

void ClientApplication::moveOnTreeShowList(boost::shared_ptr<AliasTree> tree,
        int indent, int & counter, bool v)
{
    QList<boost::shared_ptr<AliasTree> > list = tree->getMDirContent();
    for (int i = 0; i < list.size(); ++i) {
        boost::shared_ptr<AliasTree> m_tree = list[i];
        if (m_tree->isFile()) {
            if ((m_tree->getFileLocations().size() == 1) && (v == false))
                continue;
            for (int i = 0; i < m_tree->getFileLocations().size(); ++i) {

                std::cout.width(indent * 4);
                std::cout << " ";
                std::cout << m_tree->getFilename().toStdString() << "["
                        << (counter++) << "]" << "\t"
                        << m_tree->getFileLocations()[i].m_date.toStdString()
                        << "\t" << m_tree->getFileLocations()[i].m_size << "\t"
                        << m_tree->getFileLocations()[i].m_id.toStdString()
                        << "\n";
            }
        } else {
            std::cout.width(indent * 4);
            std::cout << " ";
            //std::cout<<atree->m_path.toStdString()<<"\n";
            std::cout << m_tree->getFilename().toStdString() << "\t" << "\n";
            (*this).moveOnTreeShowList(m_tree, indent + 1, counter, v);
        }
    }
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
                if (index == counter) {
                    qDebug() << "ZNALEZIONO " << m_tree->getPath();
                    if ((command == "push")
                            && (m_tree->getFileLocations()[j].m_id
                                    == Identify::getMachineIdentificator()))

                            {
                        m_serverConnection.pushFileToAlias(m_tree->getPath(),
                                m_tree->getFileLocations()[j].m_size);
                    } else if (command == "pull") {
                        m_serverConnection.pullFileFrom(
                                FileLocation(m_tree->getPath(),
                                        Identify::getMachineIdentificator()));
                    } else if (command == "choose") {
                        if (m_tree->getFileLocations()[j].m_id
                                == Identify::getMachineIdentificator()) {
                            m_serverConnection.pushFileToAlias(
                                    m_tree->getPath(),
                                    m_tree->getFileLocations()[j].m_size);
                        } else {
                            m_serverConnection.pullFileFrom(
                                    FileLocation(m_tree->getPath(),
                                            Identify::getMachineIdentificator()));
                        }
                    }
                }

                std::cout.width(indent * 4);
                std::cout << " ";
                std::cout << m_tree->getFilename().toStdString() << "["
                        << (counter++) << "]" << "\t"
                        << m_tree->getFileLocations()[i].m_date.toStdString()
                        << "\t" << m_tree->getFileLocations()[i].m_size << "\t"
                        << m_tree->getFileLocations()[i].m_id.toStdString()
                        << "\n";
            }
        } else {
            std::cout.width(indent * 4);
            std::cout << " ";
            //std::cout<<atree->m_path.toStdString()<<"\n";
            std::cout << m_tree->getFilename().toStdString() << "\t" << "\n";
            (*this).moveOnTreeIndex(m_tree, indent + 1, counter, index,
                    command);
        }
    }
}

ClientApplication::~ClientApplication()
{

}
} //namespace Client
} //namespace TIN_project
