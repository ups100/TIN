/**
 * @file TcpServer.cpp
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::TcpServer
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "TcpServer.h"
#include "UnknownConnection.h"
#include "MainServer.h"

namespace TIN_project {
namespace Server {

TcpServer::TcpServer(MainServer *mainServer)
: m_mainServer(mainServer)
{

}

void TcpServer::incomingConnection(int socketDescriptor)
{
    UnknownConnection *connection = new UnknownConnection(socketDescriptor, m_mainServer);
    m_mainServer->addNewConnection(connection);
}

} //namespace Server
} //namespace TIN_project
