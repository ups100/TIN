///////////////////////////////////////////////////////////
//  Alias.cpp
//  Implementation of the Class Alias
//  Created on:      07-gru-2012 00:33:32
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "Alias.h"
#include "Password.h"

namespace TIN_project {
namespace Server {

Alias::Alias()
{

}

Alias::~Alias()
{

}

Alias::Alias(const QString& name, Utilities::Password password)
{

}

/**
 * We assume that password has been checked
 */
void Alias::addClient(boost::shared_ptr<ClientConnection> client)
{

}

/**
 * We assume that password has been checked
 */
void Alias::addDaemon(boost::shared_ptr<DaemonConnection> daemon)
{

}

/**
 * Check if password is correct
 */
bool Alias::checkPassword(const Utilities::Password& password)
{

    return false;
}

void Alias::onConnectionClose(ClientConnection* client)
{

}

void Alias::onConnectionClose(DaemonConnection* daemon)
{

}

void Alias::onFileFound(DaemonConnection* daemon,
        boost::shared_ptr<Utilities::FileLocation> location)
{

}

void Alias::onFileList(DaemonConnection* daemon,
        boost::shared_ptr<Utilities::AliasFileList> list)
{

}

/**
 * Notify if transfer completed
 */
void Alias::onFileTransferCompleted(FileTransferServer * transfer)
{

}

/**
 * Notify if an error occured
 */
void Alias::onFileTransferError(FileTransferServer* transfer)
{

}

void Alias::onFindFile(ClientConnection* client,
        boost::shared_ptr<QString> name)
{

}

void Alias::onListAlias(ClientConnection* client)
{

}

void Alias::onNoSuchFile(DaemonConnection* daemon)
{

}

void Alias::onPullFileFrom(ClientConnection* client,
        boost::shared_ptr<Utilities::FileLocation> location)
{

}

void Alias::onPushFileToAlias(ClientConnection* client,
        boost::shared_ptr<QString> path, quint64 size)
{

}

void Alias::onRemoveFromAlias(ClientConnection* client,
        boost::shared_ptr<QString> fileName)
{

}

} //namespace server
} //namespace TIN_project
