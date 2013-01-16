/**
 * @file Client/include/ServerConnection.h
 *
 * @date 08-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Client::ServerConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_)
#define EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_

#include <QtGlobal>
#include <QString>
#include <QHostAddress>
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>
#include <climits>

namespace TIN_project {

namespace Utilities {
class Password;
class FileLocation;
class Identifier;
}

namespace Client {

class ServerConnectionListener;
class AliasCommunicationListener;

/**
 * @brief Class for handling communication between client and server.
 *
 * @details This class runs in separate thread.
 * Methods of this class are separated into two groups.
 * First group is related to server and it consist of:
 * - ServerConnection::connectToAlias()
 * - ServerConnection::createAlias()
 * - ServerConnection::removeAlias()
 * - ServerConnection::disconnectFromServer()
 *
 * Above function may be used after connecting to server using
 * ServerConnection::connectToServer() but before connecting to
 * Alias using ServerConnection::connectToAlias() function.
 *
 * Second group is related to alias and it consist of:
 * - ServerConnection::findFileInAlias()
 * - ServerConnection::listAlias()
 * - ServerConnection::pullFileFrom()
 * - ServerConnection::pushFileToAlias()
 * - ServerConnection::removeFileFromAlias()
 *
 * Those method should be used only after connecting to alias using
 * ServerConnection::connectToAlias() method.
 *
 * @see ServerConnectionListener
 * @see AliasCommunicationListener
 *
 * @warning Remember to not use any method before connecting to server using
 * ServerConnection::connectToServer() function.
 */
class ServerConnection : public QObject
{

Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param serverListener object notified while communication with server
     *
     * @param aliasListener object notified while communication with alias
     *
     * @note This object doesn't take the ownership of passed arguments.
     */
    ServerConnection(ServerConnectionListener *serverListener,
            AliasCommunicationListener *aliasListener);

    /**
     * @brief Destructor.
     *
     * @warning Remember to disconnect from server before object destruction.
     */
    virtual ~ServerConnection();

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
     * @param[in] id identity of client machine
     */
    void connectToAlias(const QString& aliasName,
            const Utilities::Password& password, const Utilities::Identifier& id);

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
     * @brief Creates new alias on server.
     *
     * @details Asynchronous method. Results are provided to
     * ServerConnectionListener::onAliasCreated() or to
     * ServerConnectionListener::onAliasCreationError()
     * if alias with that name already exist on server.
     *
     * @param[in] name of new alias (should be unique on server)
     *
     * @param[in] password to alias
     */
    void createAlias(const QString& name, const Utilities::Password& password);

    /**
     * @brief Removes alias from server.
     *
     * @details Asynchronous method. Results are provided to
     * ServerConnectionListener::onAliasDeleted() or to
     * ServerConnectionListener::onAliasDeletionError()
     * if there was no such alias or the password is incorrect.
     *
     * @param[in] name of alias
     *
     * @param[in] password to ailas
     */
    void removeAlias(const QString& name, const Utilities::Password& password);

    /**
     * @brief Looks for file or directory with specific name.
     *
     * @details Asynchronous method. Results are provided to
     * AliasCommunicationListener::onFileFound() or to
     * AliasCommunicationListener::onFileNotFound() if there is
     * no such file or directory.
     *
     * @param[in] fileName name of file or directory to look for
     */
    void findFileInAlias(const QString& fileName);

    /**
     * @brief List the whole content of alias.
     *
     * @details Asynchronous method. Results are provided to
     * AliasCommunicationListener::onAliasListed()
     */
    void listAlias(bool remoteOnly = false);

    /**
     * @brief Pull the specific file from other computer to this.
     *
     * @details Asynchronous method. Results are provided to
     * AliasCommunicationListener::onFileTransferStarted()
     * when file transfer is started or to
     * AliasCommunicationListener::onFileTransferError()
     * when there is no such file or some connection problems occurred or to
     * AliasCommunicationListener::onFileTransferFinished(0 when transfer
     * has been successfully finished
     *
     * @param[in] file location of file
     */
    void pullFileFrom(const Utilities::FileLocation& file);

    /**
     * @todo not sure about semantic of this method
     */
    void pushFileToAlias(const QString& path, qint64 size);

    /**
     * @brief Removes all occurrences of file/directory from alias
     *
     * @details Asynchronous method. Results are provided to
     * AliasCommunicationListener::onFileRemoved() or
     * AliasCommunicationListener::onFileRemovingError()
     * when no such file or directory
     *
     * @param[in] fileName name of file or directory to be removed
     */
    void removeFileFromAlias(const QString& fileName);

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
     * @brief Listener notified about received messages.
     */
    AliasCommunicationListener *m_aliasListener;

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

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_)
