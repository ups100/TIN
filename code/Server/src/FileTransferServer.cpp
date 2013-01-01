///////////////////////////////////////////////////////////
//  FileTransferServer.cpp
//  Implementation of the Class FileTransferServer
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "FileTransferServer.h"

namespace TIN_project {
namespace Server {

FileTransferServer::FileTransferServer()
{

}

FileTransferServer::~FileTransferServer()
{

}

void FileTransferServer::disconnectFromAliasSynch()
{

}

const QHostAddress& FileTransferServer::getAddress()
{

    QHostAddress add;
    return add;
}

quint16 FileTransferServer::getPort()
{

    return 0;
}

} //namespace server
} //namespace TIN_project
