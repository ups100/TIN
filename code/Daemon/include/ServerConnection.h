/**
 * @file Daemon/include/ServerConnection.h
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Daemon::ServerConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_)
#define EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_

#include <QtGlobal>
#include <QHostAddress>
#include <QByteArray>
#include <QString>
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>
#include <QTimer>
#include <climits>

namespace TIN_project {

namespace Utilities {
class FileLocation;
class AliasFileList;
class Password;
class Identifier;
}

namespace Daemon {

class ServerConnectionListener;

/**
 * @brief Class for handling connection between client and server
 *
 * @details This class runs in separate thread.
 *
 */
class ServerConnection : public QObject
{

Q_OBJECT
    ;

public:

    /**
     * @brief Constructor
     *
     * @param serverConnectionListener object notified while communication with server
     *
     * @note This object doesn't take the ownership of passed argument.
     */
    ServerConnection(ServerConnectionListener *serverConnectionListener);

    /**
     * @brief Destructor
     *
     * @warning Remember to disconnect from server before object destruction.
     */
    virtual ~ServerConnection();

    /**
     * @brief Establish connection to server.
     *
     * @details Asynchronous method. Results are provided to
     * ServerConnectionListener::onConnected() or to
     * ServerConnectionListener::onDisconnected() if unable to connect
     *
     * @param[in] address server IP address
     *
     * @param[in] port server port
     */
    void connectToServer(const QHostAddress& address, quint16 port);

    /**
     * @brief Disconnects from server
     *
     * @details Asynchronous method. Results are provided to
     * ServerConnectionListener::onDisconnected()
     */
    void disconnectFromServer();

    /**
     * @brief Connects to alias stored on server.
     *
     * @details Asynchronous method. Results are provided to
     * ServerConnectionListener::onAliasConnected or
     * ServerConnectionListener::onAliasConnectionError
     * if some error occurred (especially no such alias or wrong password)
     *
     * @param[in] aliasName name of alias
     *
     * @param[in] password password to alias
     *
     * @param[in] id identifier of daemon
     */
    void connectToAlias(const QString& aliasName,
            const Utilities::Password& password, const Utilities::Identifier& id);

    /**
     * @brief Sends location of file
     *
     * @details Usually this method is an answer to looking for some file
     *
     * @param[in] location object which contains location of file
     */
    void sendFileFound(const Utilities::AliasFileList& location);

    /**
     * @brief Sends information that there is no such file in this catalog
     *
     * @details Usually this method is an answer to looking for some file
     */
    void sendNoSuchFile();

    /**
     * @brief Sends a tree with content of catalog
     *
     * @brief Usually this method is an answer to list content
     *
     * @param[in] list content of catalog
     */
    void sendFileList(const Utilities::AliasFileList& list);

signals:

    /**
     * @brief Signal emited to send some data.
     *
     * @param[in] data to be send
     *
     * @warning Do not connect this signal to any of your slots due to
     * arguments deletion.
     */
    void sendData(QByteArray *data);

private slots:

    /**
     * @brief Helper slot used for disconnecting
     */
    void disconnectSlot();

    /**
     * @brief Helper slot for data sending.
     *
     * @param[in] array data to be send
     *
     * @warning This function takes the ownership of it's argument!
     */
    void sendSlot(QByteArray *array);

    /**
     * @brief Helper slot used while starting additional thread.
     */
    void threadStartedSlot();

    /**
     * @brief Helper slot used while finishing additional thread.
     */
    void threadFinishedSlot();

    /**
     * @brief Helper slot used while socket connecting.
     */
    void socketConnectedSlot();

    /**
     * @brief Helper slot used for handling socket errors
     */
    void socketErrorSlot(QAbstractSocket::SocketError socketError);

    /**
     * @brief Helper slot used while socket disconnecting
     */
    void socketDisconnectedSlot();

    /**
     * @brief Helper slot used for data reading
     */
    void socketReadyReadSlot();

private:

    /**
     * @brief Additional thread for handling connection
     */
    QThread m_additionalThread;

    /**
     * @brief Socket used for data reading and sending
     */
    QTcpSocket *m_socket;

    /**
     * @brief Thread which created this object
     */
    QThread *m_creatorThread;

    /**
     * @brief Listener notified about received messages.
     */
    ServerConnectionListener *m_serverListener;

    /**
     * @brief Mutex for cross-thread synchronization.
     */
    QMutex m_mutex;

    /**
     * @brief Informs if additional thread and socket are in ready to use state.
     */
    bool m_isReadyState;

    /**
     * @brief Informs if additional thread and socket are preparing.
     */
    bool m_isConnecting;

    /**
     * @brief Informs if additional thread and socket are closing.
     */
    bool m_isClosing;

    /**
     * @brief Address of server.
     */
    QHostAddress m_serverAddress;

    /**
     * @brief Server's port.
     */
    quint16 m_serverPort;

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

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_)
