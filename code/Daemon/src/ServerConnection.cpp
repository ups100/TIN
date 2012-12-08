///////////////////////////////////////////////////////////
//  ServerConnection.cpp
//  Implementation of the Class ServerConnection
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ServerConnection.h"
#include "ServerConnectionListener.h"
#include "FileLocation.h"
#include "AliasFileList.h"

namespace TIN_project {
namespace Daemon {

ServerConnection::ServerConnection()
{

}

ServerConnection::~ServerConnection()
{

}

void ServerConnection::connectToServer(const QHostAddress& address,
        quint16 port)
{

}

void ServerConnection::sendFileFound(const Utilities::FileLocation& location)
{

}

void ServerConnection::sendFileList(const Utilities::AliasFileList& list)
{

}

} //namespace Daemon
} //namespace TIN_project
