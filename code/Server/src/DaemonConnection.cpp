///////////////////////////////////////////////////////////
//  DaemonConnection.cpp
//  Implementation of the Class DaemonConnection
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonConnection.h"

namespace TIN_project {
namespace Server {

DaemonConnection::DaemonConnection()
{

}

DaemonConnection::~DaemonConnection()
{

}

void DaemonConnection::disconnectFromServer()
{

}

void DaemonConnection::sendFindFile(const QString& fileName)
{

}

void DaemonConnection::sendReciveFile(const QString& filename,
        const QHostAddress& address, quint16 port)
{

}

void DaemonConnection::sendRemoveFile(const QString& fileName)
{

}

void DaemonConnection::sendSendFile(const QString& fileName,
        const QHostAddress& address, quint16 port)
{

}

} //namespace server
} //namespace TIN_project
