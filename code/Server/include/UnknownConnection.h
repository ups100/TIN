/**
 * @file UnknownConnection.h
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::UnknownConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_3F862948_4CE1_4732_89C3_D6D45C90673C__INCLUDED_)
#define EA_3F862948_4CE1_4732_89C3_D6D45C90673C__INCLUDED_

#include "UnknownConnectionListener.h"
#include <QTcpSocket>
#include <QMutex>

namespace TIN_project {
namespace Server {

/**
 * @brief Class which represents new connection to server
 */
class UnknownConnection : public QObject
{
Q_OBJECT
    ;
public:
    /**
     * @brief Constructor
     *
     * @param connectionDescriptor descriptor of new connection
     *
     * @param listener object notified about messages
     */
    UnknownConnection(int connectionDescriptor,
            UnknownConnectionListener *listener);

    /**
     * @brief Destructor
     *
     * @warning You should disconnect socket or use convertToOtherConnection() function
     * before object destruction
     */
    virtual ~UnknownConnection();

    /**
     * @brief Sends confirmation that new alias has been created.
     */
    void sendAliasCreated();

    /**
     * @brief Sends message that alias has not been created due to some error.
     */
    void sendAliasNotCreated();

    /**
     * @brief Sends message that alias has not been removed to some error (esp. wrong password).
     */
    void sendAliasNotRemoved();

    /**
     * @brief Sends information that alias has been removed.
     */
    void sendAliasRemoved();

    /**
     * @brief Sends information that access to alias has not been granted.
     */
    void sendNotConnectedToAlias();

    /**
     * @brief Closes connection
     *
     * @details Asynchronus method results are provided to
     */
    void disconnectClinet();

    /**
     * @brief Closes connection
     */
    void disconnectClientSynch();

    /**
     * @brief Ends handling connection in this class
     *
     * @details This method is used to convert to ClientConnection or DaemonConnection class.
     *  After returning from this functions there is no more active connection in this object
     *
     *  @return Object for communication in this connection
     *
     *  @warning Caller takes the ownership of returned object.
     */
    QTcpSocket* convertToOtherConnection();

signals:

    /**
     * @brief Signal emitted to send some data.
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
     * @brief Helper slot used while socket disconnecting
     */
    void socketDisconnectedSlot();

    /**
     * @brief Helper slot for data sending.
     *
     * @param[in] array data to be send
     *
     * @warning This function takes the ownership of it's argument!
     */
    void sendSlot(QByteArray *array);

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
     * @brief Object notified about incoming messages.
     */
    UnknownConnectionListener *m_unknownConnectionListener;

    /**
     * @brief Socket used for this connection
     */
    QTcpSocket *m_socket;

    /**
     * @brief Informs if socket is connected.
     */
    bool m_isConnected;

    /**
     * @brief Mutex for synchronization
     */
    QMutex m_mutex;

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

} //namespace Server
} //namespace TIN_project

#endif // !defined(EA_3F862948_4CE1_4732_89C3_D6D45C90673C__INCLUDED_)
