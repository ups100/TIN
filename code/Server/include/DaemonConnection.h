/**
 * @file DaemonConnection.h
 *
 * @date 29-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::DaemonConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_)
#define EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QThread>
#include <QObject>

namespace TIN_project {
namespace Server {

class DaemonConnectionListener;

/**
 * @brief Class which represents single connection witch daemon
 *
 * @warning This class is not thread safe and should be used only in context of alias thread.
 */
class DaemonConnection : public QObject
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
    DaemonConnection(QTcpSocket *socket, QThread *targetThread,
            DaemonConnectionListener *listener);

    /**
     * @brief Destructor
     */
    virtual ~DaemonConnection();

    /**
     * @brief Disconnects from client.
     *
     * @details Asynchronous method results are provided
     * to DaemonConnectionListener::onConnectionClose()
     */
    void disconnectFromAlias();

    /**
     * @brief Disconnects from client.
     */
    void disconnectFromAliasSynch();

    /**
     * @brief Sends information that access to alias has been granted.
     */
    void sendConnectedToAlias();

    /**
     * @brief Sends information to daemon to look for some file
     *
     * @param fileName name of file to look for
     */
    void sendFindFile(const QString& fileName);

    /**
     * @brief Sends information to daemon to receive file from some server
     *
     * @param fileName name of file and reletive path
     *
     * @param address of server
     *
     * @param port of server
     */
    void sendReciveFile(const QString& fileName, const QHostAddress& address,
            quint16 port);

    /**
     * @brief Sends information to daemon to remove file
     *
     * @param fileName name of file and reletive path
     */
    void sendRemoveFile(const QString& fileName);

    /**
     * @brief Sends information to daemon to send file to some server
     *
     * @param fileName name of file and reletive path
     *
     * @param address of server
     *
     * @param port of server
     */
    void sendSendFile(const QString& fileName, const QHostAddress& address,
            quint16 port);

private slots:

    /**
     * @brief Helper slot used to implement sending messages from other thread.
     */
    void sendConnectedToAliasSlot();

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
    DaemonConnectionListener* m_connectionListener;

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
#endif // !defined(EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_)
