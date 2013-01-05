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

namespace TIN_project {
namespace Client {

ClientApplication::ClientApplication(int argc, char **argv)
        : m_state(ClientApplication::NOT_CONNECTED),m_application((QString) "TIN_project_Client", argc, argv),
                m_commandParser(), m_serverConnection(this, this),
                m_DaemonCommunication(), m_view(new ClientView(*this))
{

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

}

void ClientApplication::onAliasConnectionErrorSlot()
{
    m_view->showMessage("Not connected to alias, error appears");
}

void ClientApplication::onAliasCreatedSlot()
{
    m_view->showMessage("Alias created");
}

void ClientApplication::onAliasCreationErrorSlot()
{
    m_view->showMessage("Alias not created, error appears");
}

void ClientApplication::onAliasDeletedSlot()
{
    m_view->showMessage("Alias deleted successfully");
}

void ClientApplication::onAliasDeletionErrorSlot()
{
    m_view->showMessage("Alias not deleted successfully");
}

void ClientApplication::onAliasListedSlot(const Utilities::AliasFileList& list)
{
    //TODO
}

void ClientApplication::onConnectedSlot()
{
    m_view->showMessage("Connected");
}

void ClientApplication::onDisconnectedSlot()
{
    m_view->showMessage("Disconnected");
}

void ClientApplication::onFileFoundSlot(const Utilities::FileLocation& location)
{

}

void ClientApplication::onFileNotFoundSlot()
{
    m_view->showMessage("File not found");
}

void ClientApplication::onFileRemovedSlot()
{
    m_view->showMessage("File removed");
}

void ClientApplication::onFileRemovingErrorSlot()
{
    m_view->showMessage("Error while removing file");
}

void ClientApplication::onFileTransferErrorSlot()
{
    m_view->showMessage("File transfer error");
}

void ClientApplication::onFileTransferFinishedSlot()
{
    m_view->showMessage("File transfer finished");
}

void ClientApplication::onFileTransferStartedSlot()
{
    m_view->showMessage("File transfer started");
}

void ClientApplication::setView(boost::shared_ptr<ClientView> view)
{

}

void ClientApplication::getString(QString s)
{


  boost::shared_ptr<Commands> cmd =  m_commandParser.parseCommand(s);

  qDebug()<<"Koniec petli";
  //QTimer::singleShot(10000,&(*m_view),SLOT(showMessage("HELLO")));
  m_view->showMessage("TR");
  (*this).setState(ClientApplication::CONNECTED);

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

bool ClientApplication::checkIfPossible(boost::shared_ptr<Commands> cmd)
{
    return true;
}

void ClientApplication::setState(ClientApplication::States s)
{
    m_state = s;
}

ClientApplication::States ClientApplication::getState() const
{
    return m_state;
}
} //namespace Client
} //namespace TIN_project
