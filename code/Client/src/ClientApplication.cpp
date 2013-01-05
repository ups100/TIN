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
        : m_state(ClientApplication::NOT_CONNECTED),m_application((QString) "TIN_project_Client", argc, argv),
                m_commandParser(), m_serverConnection(this, this),
                m_DaemonCommunication(), m_view(new ClientView(*this))
{
    //(*this).setState(ClientApplication::NOT_CONNECTED);

}

ClientApplication::~ClientApplication()
{

}

void ClientApplication::onAliasConnected()
{
    QTimer::singleShot(0,this,SLOT(onAliasConnectedSlot()));
}

void ClientApplication::onAliasConnectionError()
{
    QTimer::singleShot(0,this,SLOT(onAliasConnectionErrorSlot()));
}

void ClientApplication::onAliasCreated()
{
    QTimer::singleShot(0,this,SLOT(onAliasConnectedSlot()));
}

void ClientApplication::onAliasCreationError()
{
    QTimer::singleShot(0,this,SLOT(onAliasCreationErrorSlot()));
}

void ClientApplication::onAliasDeleted()
{
    QTimer::singleShot(0,this,SLOT(onAliasDeletedSlot()));
}

void ClientApplication::onAliasDeletionError()
{
    QTimer::singleShot(0,this,SLOT(onAliasDeletionErrorSlot()));
}

void ClientApplication::onAliasListed(const Utilities::AliasFileList& list)
{
    //TODO
}

void ClientApplication::onConnected()
{
    QTimer::singleShot(0,this,SLOT(onConnectedSlot()));
}

void ClientApplication::onDisconnected()
{
    QTimer::singleShot(0,this,SLOT(onDisconnectedSlot()));
}

void ClientApplication::onFileFound(const Utilities::FileLocation& location)
{
    //TODO
}

void ClientApplication::onFileNotFound()
{
    QTimer::singleShot(0,this,SLOT(onFileNotFoundSlot()));
}

void ClientApplication::onFileRemoved()
{
    QTimer::singleShot(0,this,SLOT(onFileRemovedSlot()));
}

void ClientApplication::onFileRemovingError()
{
    QTimer::singleShot(0,this,SLOT(onFileRemovingErrorSlot()));
}

void ClientApplication::onFileTransferError()
{
    QTimer::singleShot(0,this,SLOT(onFileTransferErrorSlot()));
}

void ClientApplication::onFileTransferFinished()
{
    QTimer::singleShot(0,this,SLOT(onFileTransferFinishedSlot()));
}

void ClientApplication::onFileTransferStarted()
{
    QTimer::singleShot(0,this,SLOT(onFileTransferStartedSlot()));
}




void ClientApplication::onAliasConnectedSlot()
{
    m_view->showMessage("Connected to alias");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasConnectionErrorSlot()
{
    m_view->showMessage("Not connected to alias, error appears");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasCreatedSlot()
{
    m_view->showMessage("Alias created");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasCreationErrorSlot()
{
    m_view->showMessage("Alias not created, error appears");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasDeletedSlot()
{
    m_view->showMessage("Alias deleted successfully");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onAliasDeletionErrorSlot()
{
    m_view->showMessage("Alias not deleted successfully");
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
    //QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileFoundSlot(const Utilities::FileLocation& location)
{

    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileNotFoundSlot()
{
    m_view->showMessage("File not found");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileRemovedSlot()
{
    m_view->showMessage("File removed");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileRemovingErrorSlot()
{
    m_view->showMessage("Error while removing file");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileTransferErrorSlot()
{
    m_view->showMessage("File transfer error");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileTransferFinishedSlot()
{
    m_view->showMessage("File transfer finished");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::onFileTransferStartedSlot()
{
    m_view->showMessage("File transfer started");
    QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
}

void ClientApplication::setView(boost::shared_ptr<ClientView> view)
{

}

void ClientApplication::getCommand(QString s)
{
  (*this).setState(ClientApplication::WAITING);
  boost::shared_ptr<Commands> cmd =  m_commandParser.parseCommand(s);
  QTimer::singleShot(0, &(*m_view), SLOT(reconnectNotifier()));
  if(!(*this).checkIntegrity(cmd)) return;

}

int ClientApplication::start(const QHostAddress& address, quint16 port)
{
    if (m_application.isRunning()) {
         qDebug()<<"Another client application is running"<<endl;
          return -1;
    }
    qDebug()<<"Client application started"<<endl;

    /** Server should be working */
    m_serverConnection.connectToServer(address,port);


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
   if(!cmd->isCorrect()) return false;
   if(cmd->getCommand() == "log")
   {
       return (*this).checkIfConfigFileExists();
   }

   if((cmd->getCommand() == "add") || ((cmd->getCommand() == "rm") && (cmd->getParameter() == "d")))
   {
       return (*this).checkAbsolutePath(cmd->getArg());
   }
   if(((cmd->getCommand() == "rm") && (cmd->getParameter() == "")) || (cmd->getCommand() == "push"))
   {
       return (*this).checkRelativePath(cmd->getArg());
   }
   if((cmd->getCommand() == "choose"))
   {
       bool number;
       cmd->getArg().toInt(&number,10);
       return number;
   }
    return true;
}

bool ClientApplication::checkStateCondition(boost::shared_ptr<Commands> cmd) const
{
    ClientApplication::States state= (*this).getState();
    if(state == ClientApplication::NOT_CONNECTED)
        return false;
    if((state == ClientApplication::CONNECTED) && (cmd->getCommand() != "log"))
        return false;
    return true;
}

bool ClientApplication::checkRelativePath(QString s) const
{
    QString tmpDir = QDir::currentPath();
    tmpDir.append(QDir::separator());
    tmpDir.append(s);
    qDebug()<<tmpDir;
    return true;
}

bool ClientApplication::checkAbsolutePath(QString s) const
{
    QFile file(s);
    return file.exists();
}

bool ClientApplication::checkIfConfigFileExists() const
{
    QString path = QDir::currentPath();
    path.append(QDir::separator());
    path.append(ConfigFileName::CONFIG_FILE_NAME);
    QFile file(path);
    return (file.exists());
}
} //namespace Client
} //namespace TIN_project
