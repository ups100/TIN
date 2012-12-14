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

ClientApplication::ClientApplication()
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

void ClientApplication::onAliasListed(
        boost::shared_ptr<Utilities::AliasFileList> list)
{

}

void ClientApplication::onConnected()
{

}

void ClientApplication::onDisconnected()
{

}

void ClientApplication::onFileFound(
        boost::shared_ptr<Utilities::FileLocation> location)
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

void ClientApplication::start()
{
    // TODO parser inject, send message test
    do {
        char buf[1024];
        std::cout << " $> ";
        std::cin >> buf;

        if (strcmp(buf, "quit") == 0)
            break;

        m_DaemonCommunication.talkToDaemon(Utilities::Message(buf));

    } while (1);
}

} //namespace Client
} //namespace TIN_project
