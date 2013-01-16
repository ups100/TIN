/**
 * @file MainServer.h
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::MainServer
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_)
#define EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_

#include <QHostAddress>
#include <QString>
#include <QTcpServer>
#include <QFile>

#include "qtsinglecoreapplication.h"

#include <boost/shared_ptr.hpp>

#include "UnknownConnectionListener.h"
#include "Alias.h"
#include "TcpServer.h"

namespace TIN_project {
namespace Utilities {
class Password;
}

namespace Server {

class UnknownConnection;

/**
 * @brief Main server class.
 *
 * @details This class implements whole server logic.
 * To use this class remember to execute MainServer::start() function to enter event loop.
 *
 * @note Multiple instances of this object are allowed but only one of them can be "in run".
 */
class MainServer : public QObject, public UnknownConnectionListener
{
Q_OBJECT
    ;

public:
    /**
     * @brief Constructor
     *
     * @param[in] argc number of arguments
     *
     * @param[in] argv command line arguments
     */
    MainServer(int argc, char **argv);

    /**
     *@brief Destructor
     */
    virtual ~MainServer();

    /**
     * @brief Starts listening for new connections and enters the event loop
     *
     * @param[in] address to listen
     *
     * @param[in] port to listen
     *
     * @param[in] outerAddress is outer Server Address
     *
     * @return value returned by event loop
     */
    int start(const QHostAddress& address, quint16 port, const QHostAddress& outerAddress);

    /**
     * @brief Add connection to set of handled connections
     *
     * @param connection new connection
     *
     * @note This function takes the ownership of passed object
     */
    void addNewConnection(UnknownConnection *connection);

    virtual void onConnectionClosed(UnknownConnection *connection);

    virtual void onAddDirecotry(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password, const Utilities::Identifier& id);

    virtual void onConnectToAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password, const Utilities::Identifier& id);

    virtual void onCreateAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password);

    virtual void onRemoveAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password);

private slots:

    /**
     * @brief Stops the server and exits from event loop with given code.
     *
     * @param[in] exitCode of the event loop
     */
    void stopServer(int exitCode = 0);

    /**
     * @brief Helper slot to implements onConnectToAlias() function.
     *
     * @param[in] connection identifier
     *
     * @param[in] aliasName name of alias
     *
     * @param[in] password to alias
     *
     * @param[in] id identifier
     */
    void onConnectToAliasSlot(UnknownConnection* connection, QString aliasName,
            TIN_project::Utilities::Password password, TIN_project::Utilities::Identifier id);

    /**
     * @brief Helper slot to implements onAddDirectory() function.
     *
     * @param[in] connection identifier
     *
     * @param[in] aliasName name of alias
     *
     * @param[in] password to alias
     *
     * @param[in] id identifier
     */
    void onAddDirecotrySlot(UnknownConnection* connection, QString aliasName,
            TIN_project::Utilities::Password password, TIN_project::Utilities::Identifier id);

private:

    /**
     * @brief Object for handling signals and check for other instances
     */
    QtSingleCoreApplication m_application;

    /**
     * @brief Server to listening for incoming connections
     */
    TcpServer m_server;

    /**
     * @brief Aliases on this server
     */
    QList<boost::shared_ptr<Alias> > m_aliases;

    /**
     * @brief Connections handed by main server
     */
    QList<boost::shared_ptr<UnknownConnection> > m_connections;

    /**
     * @brief Outer address of server
     */
    QHostAddress m_outerAddress;
};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_)
