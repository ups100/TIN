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
#include <QMutex>

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
class FileTransferServer : public QObject
{
Q_OBJECT
    ;
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
     * @brief Starts the server at passed address and port
     *
     * @param[in] address ip (Any by default)
     *
     * @param[in] port (0 by default - means let system choose)
     *
     * @return
     * - true is server has been correctly started
     * - false otherwise
     */
    bool startFileServer(const QHostAddress& address = QHostAddress::Any,
            quint16 port = 0);

    /**
     * @brief Disconnects and closes server
     *
     * @warning This function should not be called in context of #m_additionalThread
     */
    void disconnectFromAliasSynch();

    /**
     * @brief Gets address of server
     *
     * @return
     * - server address
     * - default object if server is not listening
     */
    inline QHostAddress getAddress()
    {
        return (m_server != 0L ? m_server->isListening() : false) ?
                m_address : QHostAddress();
    }

    /**
     * @brief Gets server's port.
     *
     * @return
     * - port number if set
     * - 0 if server is not listening
     */
    quint16 getPort()
    {
        return (m_server != 0L ? m_server->isListening() : false) ? m_port : 0;
    }

    /**
     * @brief Informs if server is working now.
     *
     * @return
     * - true if yes
     * - false if no
     */
    inline bool isRunning()
    {
        return !(m_state == OFFLINE);
    }
signals:

    /**
     * @brief Some signal for internal class usage
     *
     * @warning Do not use this signal, startFileServer() method is synchronous.
     */
    void serverStartedSignal();

private slots:

    /**
     * @brief Creates server object and starts it
     */
    void startServerSlot();

    /**
     * @brief Slot executed when there is new incoming connection to server
     */
    void addNewConnectionSlot();

    /**
     * @brief Slot executed when new data is available
     */
    void readDataSlot();

    /**
     * @brief Slot executed when some socket has been disconnected
     */
    void socketDisconnectedSlot();

    /**
     * @brief Slot executed when an error occurred in some socket
     *
     * @param[in] error occurred in socket
     */
    void socketErrorSlot(QAbstractSocket::SocketError error);

    /**
     * @brief Stops all connections and the server.
     */
    void stopAllSlot();

    /**
     * @brief Slot executed when additional thread has finished execution
     */
    void threadFinishedSlot();

    /**
     * @brief Slot used for sending initialization peak.
     */
    void sendPeak();

private:
    /**
     * @brief Object notified about transfer status
     */
    FileTransferListener *m_FileTransferListener;

    /**
     * @brief Number of connections we would like to have to start transfer
     */
    int m_numberOfConenctions;

    /**
     * @brief Server address
     */
    QHostAddress m_address;

    /**
     * Port number
     */
    quint16 m_port;

    /**
     * @brief Size of file to be transfered.
     */
    quint64 m_fileSize;

    /**
     * @brief Current size of transfered file
     */
    quint64 m_currentSize;

    /**
     * @brief Server for waiting for connection
     */
    QTcpServer *m_server;

    /**
     * @brief List of connected clients
     */
    QList<QTcpSocket*> m_clients;

    /**
     * @brief Thread which created this object
     */
    QThread *m_creatorThread;

    /**
     * @brief Additional thread for handling file transfer
     */
    QThread m_additionalThread;

    /**
     * @brief States of this class
     */
    enum STATE
    {
        /**
         * @brief Default state. Nothing running here.
         */
        OFFLINE,

        /**
         * @brief Server is running and waiting for clients
         */
        WAITING_FOR_CLIENTS,

        /**
         * @brief All clients are connected, peak sending.
         */
        ALL_CLIENTS_CONNECTED,

        /**
         * @brief File transfer started
         */
        TRANSFER_IN_PROGRESS,

        /**
         * @brief File transfer completed
         */
        TRANSFER_COMPLETED,

        /**
         * @brief File transfer has been completed and all clients
         * has been disconnected.
         */
        ALL_DISCONNECTED,

        /**
         * @brief Error occurred.
         */
        ERROR,

        /**
         * @brief Error occurred and all clients has been disconnected
         */
        ERROR_DISCONNECTING,

        /**
         * @brief Forcing synchronous close.
         */
        CLOSING
    };

    STATE m_state;

    /**
     * @brief Mutex for synchronization
     */
    QMutex m_mutex;
};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_8F738BBF_D47D_472b_9E55_623B17AAECE7__INCLUDED_)
