///////////////////////////////////////////////////////////
//  ServerConnection.cpp
//  Implementation of the Class ServerConnection
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ServerConnection.h"
#include "Password.h"
#include "FileLocation.h"

namespace TIN_project {
namespace Client {

ServerConnection::ServerConnection()
{

}

ServerConnection::~ServerConnection()
{

}

void ServerConnection::connectToAlias(const QString& aliasName,
        const Utilities::Password& password)
{

}

void ServerConnection::connectToServer(const QHostAddress& address,
        quint16 port)
{

}

void ServerConnection::createAlias(const QString& name,
        const Utilities::Password& password)
{

}

void ServerConnection::findFileInAlias(const QString& fileName)
{

}

void ServerConnection::listAlias()
{

}

void ServerConnection::pullFileFrom(const Utilities::FileLocation& file)
{

}

/**
 * not sure about semantic of this method
 */
void ServerConnection::pushFileToAlias(const QString& path)
{

}

/**
 * not sure about semantic of this method 
 */
void ServerConnection::removeFileFromAlias(const QString& fileName)
{

}

} //namespace Client
} //namespace TIN_project
