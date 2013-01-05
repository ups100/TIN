///////////////////////////////////////////////////////////
//  ClientApplication.cpp
//  Implementation of the Class ClientApplication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientApplication.h"
#include "AliasFileList.h"
#include "FileLocation.h"

#include "AliasFileList.h"
const static QString NAME_OF_FILE = "/lua_tutorial.txt";
namespace TIN_project {
namespace Client {

ClientApplication::ClientApplication(int argc, char **argv)
        : m_state(ClientApplication::NOT_CONNECTED),
                m_application((QString) "TIN_project_Client", argc, argv),
                m_commandParser(), m_serverConnection(this, this),
                m_DaemonCommunication(), m_view(new ClientView(*this))
{

}

ClientApplication::~ClientApplication()
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
    //TODO
}

void ClientApplication::onConnected()
{
    QTimer::singleShot(0, this, SLOT(onConnectedSlot()));
}

void ClientApplication::onDisconnected()
{
    QTimer::singleShot(0, this, SLOT(onDisconnectedSlot()));
}

void ClientApplication::onFileFound(const Utilities::FileLocation& location)
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
        m_serverConnection.connectToAlias((*this).m_alias, (*this).m_password);
    } else if (cmd->getCommand() == "create") {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.createAlias(cmd->getArg(), cmd->getPassword());
    } else if (cmd->getCommand() == "add") {
        //(*this).setState(ClientApplication::WAITING);
        Utilities::CommunicationProtocol::Communicate<Utilities::CommunicationProtocol::ADD_DIRECTORY> message(Utilities::Message(m_alias,m_password),
                cmd->getArg(),m_address,m_port));
        //TODO TO DAEMON
    } else if ((cmd->getCommand() == "rm") && (cmd->getParameter() == "d")) {
        //(*this).setState(ClientApplication::WAITING);
        //TODO TO DAEMON
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
        //TODO FROM NUMBER TO FILE NAME
        //Or maybe it will be better to invoke it by relative path
    } else if (cmd->getCommand() == "push") {
        //(*this).setState(ClientApplication::WAITING);
        //TODO get Size of file
        m_serverConnection.pushFileToAlias(cmd->getArg(), 0);
    } else if (cmd->getCommand() == "ls") {
        //(*this).setState(ClientApplication::WAITING);
        m_serverConnection.listAlias();
    } else if ((cmd->getCommand() == "synch") && (cmd->getParameter() == "o")) {
        //(*this).setState(ClientApplication::WAITING);
        //TODO Don't know which command to invoke
    } else if ((cmd->getCommand() == "synch") && (cmd->getParameter() == "d")) {
        //(*this).setState(ClientApplication::WAITING);
        //TODO Not sure if this should be invoked
        m_serverConnection.listAlias();
    } else if ((cmd->getCommand() == "choose")) {
        //(*this).setState(ClientApplication::WAITING);
        //TODO change number to filename
        //and decide if pull or push
    } else if ((cmd->getCommand() == "pull")) {
        //(*this).setState(ClientApplication::WAITING);
        //TODO How to get the ID
        m_serverConnection.pullFileFrom(FileLocation(cmd->getArg(), "Damn"));
    }


    //Utilities::CommunicationProtocol::Communicate<
    // Utilities::CommunicationProtocol::ADD_DIRECTORY_AND_CONNECT> message(
    // Utilities::Message(QString(buf),
    // Utilities::Password(QString("passwd")),
    // "/home/kajo/workspace/tin/testDir1",
    // QHostAddress("127.0.0.1"), 8080));

    return true;
}
int ClientApplication::start(const QHostAddress& address, quint16 port)
{
    if (m_application.isRunning()) {
        qDebug() << "Another client application is running" << endl;
        return -1;
    }
    qDebug() << "Client application started" << endl;

    /** Server should be working */
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
    if ((cmd->getCommand() == "choose")) {
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
    /*QString path = QDir::currentPath();
     path.append(QDir::separator());
     path.append(ConfigFileName::CONFIG_FILE_NAME);
     QFile file(path);
     return (file.exists());*/
    return true;
}

} //namespace Client
} //namespace TIN_project
