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
#include <QSocketNotifier>

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

class MainServer : public QObject, public UnknownConnectionListener
{
Q_OBJECT
    ;

public:
    /**
     * @brief Constructor
     *
     * @param argc number of arguments
     *
     * @param argv command line arguments
     */
    MainServer(int argc, char **argv);

    /**
     *@brief Destructor
     */
    virtual ~MainServer();

    virtual void onConnectionClosed(UnknownConnection *connection);

    virtual void onAddDirecotry(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password);

    virtual void onConnectToAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password);

    virtual void onCreateAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password);

    virtual void onRemoveAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password);

    int start(const QHostAddress& address, quint16 port);

    void addNewConnection(UnknownConnection *connection);

public slots:

    /**
     * @brief Stops the server and exits from event loop with given code.
     *
     * @param[in] exitCode of the event loop
     */
    void stopServer(int exitCode = 0);

private slots:

    void onConnectToAliasSlot(UnknownConnection* connection, QString aliasName,
            TIN_project::Utilities::Password password);

private:

    QtSingleCoreApplication m_application;

    TcpServer m_server;

    QList<boost::shared_ptr<Alias> > m_aliases;

    QList<boost::shared_ptr<UnknownConnection> > m_connections;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_)
