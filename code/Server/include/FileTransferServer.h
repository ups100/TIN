/**
 * @file FileTransferServer.h
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::FileTransferServer
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_8F738BBF_D47D_472b_9E55_623B17AAECE7__INCLUDED_)
#define EA_8F738BBF_D47D_472b_9E55_623B17AAECE7__INCLUDED_

#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

#include <boost/shared_ptr.hpp>

#include "FileTransferListener.h"

namespace TIN_project {
namespace Server {

class FileTransferListener;

/**
 * @brief Class responsible for file transferring
 *
 * @details This class runs in separate thread.
 * Results are provided to FileTransferListener
 */
class FileTransferServer
{

public:
    /**
     * @brief Constructor
     *
     * @param listener object to be notified about transfer state
     *
     * @param[in] numberOfConnections
     *
     * @param[in] fileSize size of file
     */
    FileTransferServer(FileTransferListener *listener, int numberOfConnections,
            quint64 fileSize);

    /**
     * @brief Destructor
     */
    virtual ~FileTransferServer();

    /**
     * @brief Disconnects and closes server
     */
    void disconnectFromAliasSynch();

    /**
     * @brief Gets address of server
     */
    const QHostAddress& getAddress();

    /**
     * @brief Gets server's port.
     */
    quint16 getPort();

private:
    /**
     * @brief Object notified about transfer status
     */
    FileTransferListener *m_FileTransferListener;

    /**
     * @brief Server address
     */
    QHostAddress m_address;

    /**
     * Port number
     */
    quint16 m_port;

    /**
     * @brief Server for waiting for connection
     */
    QTcpServer *m_server;

    /**
     * @brief List of connected clients
     */
    QList<boost::shared_ptr<QTcpSocket> > m_clients;

    /**
     * @brief Additional thread for handling file transfer
     */
    QThread m_additionalThread;
};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_8F738BBF_D47D_472b_9E55_623B17AAECE7__INCLUDED_)
