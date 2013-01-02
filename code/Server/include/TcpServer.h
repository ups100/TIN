/**
 * @file TcpServer.h
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

#include <QTcpServer>

#ifndef __TIN_PTOJECT_SERVER_TCPSERVER_H__
#define __TIN_PTOJECT_SERVER_TCPSERVER_H__

namespace TIN_project {
namespace Server {

class MainServer;

/**
 * @brief Class for accepting incoming connections
 *
 * @warning This class has been written for special needs so it breaks
 * the mechanism of pending connections.
 */
class TcpServer : private QTcpServer
{
public:
    /**
     * @brief Constructor
     *
     * @param mainServer object which will handle new connections
     *
     * @note This class does not take the ownership of it's parameter
     */
    TcpServer(MainServer *mainServer);

    using QTcpServer::listen;

    using QTcpServer::isListening;

    using QTcpServer::close;

    using QTcpServer::serverAddress;

    using QTcpServer::serverPort;

protected:
    virtual void incomingConnection(int socketDescriptor);

    /**
     * @brief Object which will handle new connections
     */
    MainServer *m_mainServer;
};

} //namespace Server
} //namespace TIN_project
#endif
