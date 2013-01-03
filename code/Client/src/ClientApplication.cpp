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
        : m_application((QString) "TIN_project_Client", argc, argv),
                m_commandParser(), m_serverConnection(this, this),
                m_DaemonCommunication()
{

}

ClientApplication::~ClientApplication()
{

}

void ClientApplication::onAliasConnected()
{

}

void ClientApplication::onAliasConnectionError()
{

}

void ClientApplication::onAliasCreated()
{

}

void ClientApplication::onAliasCreationError()
{

}

void ClientApplication::onAliasDeleted()
{

}

void ClientApplication::onAliasDeletionError()
{

}

void ClientApplication::onAliasListed(const Utilities::AliasFileList& list)
{

}

void ClientApplication::onConnected()
{

}

void ClientApplication::onDisconnected()
{

}

void ClientApplication::onFileFound(const Utilities::FileLocation& location)
{

}

void ClientApplication::onFileNotFound()
{

}

void ClientApplication::onFileRemoved()
{

}

void ClientApplication::onFileRemovingError()
{

}

void ClientApplication::onFileTransferError()
{

}

void ClientApplication::onFileTransferFinished()
{

}

void ClientApplication::onFileTransferStarted()
{

}

void ClientApplication::setView(boost::shared_ptr<ClientView> view)
{

}

int ClientApplication::start()
{
    if (m_application.isRunning()) {
         qDebug()<<"Another client application is running"<<endl;
          return -1;
    }

    return m_application.exec();
  //  return 0;
}

} //namespace Client
} //namespace TIN_project
