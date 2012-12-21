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

namespace TIN_project {

namespace Utilities {
class Password;
class FileLocation;
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
     */
    void connectToAlias(const QString& aliasName,
            const Utilities::Password& password);

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
    void listAlias();

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

    void sendData(QByteArray data);

private slots:

    void disconnectSlot();

    void sendSlot(QByteArray* array);

    void threadStartedSlot();

    void threadFinishedSlot();

    void socketConnectedSlot();

    void socketErrorSlot(QAbstractSocket::SocketError socketError);

    void socketDisconnectedSlot();

    void socketReadyReadSlot();

private:

    QThread m_additionalThread;

    QTcpSocket *m_socket;

    QThread *m_creatorThread;

    ServerConnectionListener *m_serverListener;

    AliasCommunicationListener *m_aliasListener;

    QMutex m_mutex;

    bool m_isReadyState;

    bool m_isConnecting;

    bool m_isClosing;

    QHostAddress m_serverAddress;

    quint16 m_serverPort;
};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_)
