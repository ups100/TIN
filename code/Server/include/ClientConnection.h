/**
 * @file ClientConnection.h
 *
 * @date 23-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::ClientConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_8025FA43_E1FD_4c29_94EA_DE5FDF7489E8__INCLUDED_)
#define EA_8025FA43_E1FD_4c29_94EA_DE5FDF7489E8__INCLUDED_

#include "ClientConnectionListener.h"
#include <QObject>
#include <QTcpSocket>

namespace TIN_project {

namespace Utilities {
class FileLocation;
class AliasFileList;
}
namespace Server {

/**
 * @brief Class which represents single connection witch client
 *
 * @warning This class is not thread safe and should be used only in context of alias thread.
 */
class ClientConnection : public QObject
{
Q_OBJECT
    ;
public:
    /**
     * @brief Constructor
     *
     * @param socket for communication. This object takes the ownership of it.
     *
     * @param targetThread of alias to which client is connected
     *
     * @param listener to be notified about incoming messages
     */
    ClientConnection(QTcpSocket *socket, QThread *targetThread,
            ClientConnectionListener *listener);

    /**
     * @brief Destructor
     */
    virtual ~ClientConnection();

    /**
     * @brief Disconnects from client.
     *
     * @details Asynchronous method results are provided
     * to ClientConnectionListener::onConnectionClose()
     */
    void disconnectFromAlias();

    /**
     * @brief Disconnects from client.
     */
    void disconnectFromAliasSynch();

    /**
     * @brief Sends information that file has been found.
     *
     * @param[in] location locations of file
     */
    void sendFileFound(const Utilities::FileLocation& location);

    /**
     * @brief Sends listeted content of alias
     *
     * @param[in] list of files
     */
    void sendFileList(const Utilities::AliasFileList& list);

    /**
     * @brief Sends information that searched file has not been found.
     */
    void sendFileNotFound();

    /**
     * @brief Sends information that file has not been removed.
     */
    void sendFileNotRemoved();

    /**
     * @brief Sends information that file has been removed.
     */
    void sendFileRemoved();

    /**
     * @brief Sends information that error occurred while file transfer.
     */
    void sendFileTransferError();

    /**
     * @brief Sends information that file transfer has been finished.
     */
    void sendFileTransferFinished();

    /**
     * @brief Sends information that file transfer has been started
     */
    void sendFileTransferStarted();

    /**
     * @brief Sends information that there is no such file in alias
     */
    void sendNoSuchFile();

private slots:
    /**
     * @brief Helper slot used for disconnecting
     */
    void disconnectSlot();

    /**
     * @brief Helper slot used while socket disconnecting
     */
    void socketDisconnectedSlot();

    /**
     * @brief Helper slot used for handling socket errors
     */
    void socketErrorSlot(QAbstractSocket::SocketError socketError);

    /**
     * @brief Helper slot used for data reading
     */
    void socketReadyReadSlot();

private:

    /**
     * @brief Helper function for data sending.
     *
     * @param[in] array data to be send
     */
    void sendAllFunction(const QByteArray& array);

    /**
     * @brief Object notified about incoming messages
     */
    ClientConnectionListener *m_connectionListener;

    /**
     * @brief Socket used for this connection
     */
    QTcpSocket *m_socket;

    /**
     * @brief Informs if socket is connected.
     */
    bool m_isConnected;

    /**
     * @brief ID of lastly received message
     */
    char m_currentMessageId;

    /**
     * @brief Informs if there was enough data to determine message's size
     */
    bool m_sizeOk;

    /**
     * @brief Size of lastly received message
     */
    qint32 m_messageSize;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_8025FA43_E1FD_4c29_94EA_DE5FDF7489E8__INCLUDED_)
