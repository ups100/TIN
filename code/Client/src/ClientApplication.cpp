///////////////////////////////////////////////////////////
//  ClientApplication.cpp
//  Implementation of the Class ClientApplication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientApplication.h"
#include "AliasFileList.h"
#include "FileLocation.h"

namespace TIN_project {
namespace Client {

ClientApplication::ClientApplication(int argc, char **argv)
        : m_state(ClientApplication::NOT_CONNECTED),m_application((QString) "TIN_project_Client", argc, argv),
                m_commandParser(), m_serverConnection(this, this),
                m_DaemonCommunication(), m_view(new ClientView(*this))
{
    m_view->start();
}

ClientApplication::~ClientApplication()
{

}

void ClientApplication::onAliasConnected()
{
    m_view->showMessage("Connected to alias");
}

void ClientApplication::onAliasConnectionError()
{
    m_view->showMessage("Not connected to alias, error appears");
}

void ClientApplication::onAliasCreated()
{
    m_view->showMessage("Alias created");
}

void ClientApplication::onAliasCreationError()
{
    m_view->showMessage("Alias not created, error appears");
}

void ClientApplication::onAliasDeleted()
{
    m_view->showMessage("Alias deleted successfully");
}

void ClientApplication::onAliasDeletionError()
{
    m_view->showMessage("Alias not deleted successfully");
}

void ClientApplication::onAliasListed(const Utilities::AliasFileList& list)
{
    //TODO
}

void ClientApplication::onConnected()
{
    m_view->showMessage("Connected");
}

void ClientApplication::onDisconnected()
{
    m_view->showMessage("Disconnected");
}

void ClientApplication::onFileFound(const Utilities::FileLocation& location)
{

}

void ClientApplication::onFileNotFound()
{
    m_view->showMessage("File not found");
}

void ClientApplication::onFileRemoved()
{
    m_view->showMessage("File removed");
}

void ClientApplication::onFileRemovingError()
{
    m_view->showMessage("Error while removing file");
}

void ClientApplication::onFileTransferError()
{
    m_view->showMessage("File transfer error");
}

void ClientApplication::onFileTransferFinished()
{
    m_view->showMessage("File transfer finished");
}

void ClientApplication::onFileTransferStarted()
{
    m_view->showMessage("File transfer started");
}

void ClientApplication::setView(boost::shared_ptr<ClientView> view)
{

}

void ClientApplication::getString(QString s)
{
  boost::shared_ptr<Commands> cmd =  m_commandParser.parseCommand(s);
}

int ClientApplication::start(const QHostAddress& address, quint16 port)
{
    if (m_application.isRunning()) {
         qDebug()<<"Another client application is running"<<endl;
          return -1;
    }
    qDebug()<<"Client application started"<<endl;
    return m_application.exec();
}

} //namespace Client
} //namespace TIN_project
