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
