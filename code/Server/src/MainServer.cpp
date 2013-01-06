/**
 * @file MainServer.cpp
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

#include "MainServer.h"
#include "Password.h"
#include <QTimer>

namespace TIN_project {
namespace Server {

MainServer::MainServer(int argc, char **argv)
        : m_application(QString("TIN_project_server"), argc, argv),
                m_server(this)
{
    qRegisterMetaType<Utilities::Password>();
    qRegisterMetaType<UnknownConnection*>("UnknownConnection*");
}

MainServer::~MainServer()
{
    if (m_server.isListening()) {
        qDebug() << "Destroying server but it is still listening. Closing...";
        m_server.close();
    }

    if (m_connections.size() > 0) {
        qDebug()
                << "Destroying server with active connections. Disconnecting...";
        for (int i = 0; i < m_connections.size(); ++i) {
            m_connections[i]->disconnectClientSynch();
        }
        m_connections.clear();
    }

    if (m_aliases.size() > 0) {
        qDebug() << "Destroying server with running aliases. Stopping...";
        for (int i = 0; i < m_aliases.size(); ++i) {
            m_aliases[i]->stop();
        }
        m_aliases.clear();
    }
}

void MainServer::onConnectionClosed(UnknownConnection *connection)
{
    qDebug() << "closed";
    for (int i = 0; i < m_connections.size(); ++i) {
        if (m_connections[i].get() == connection) {
            m_connections.removeAt(i);
            return;
        }
    }
}

void MainServer::onAddDirecotry(UnknownConnection *connection,
        const QString &aliasName, const Utilities::Password &password)
{
    QMetaObject::invokeMethod(this, "onAddDirecotrySlot",
            Qt::QueuedConnection, Q_ARG(UnknownConnection*, connection),
            Q_ARG(QString, aliasName),
            Q_ARG(TIN_project::Utilities::Password, password));
}

void MainServer::onConnectToAlias(UnknownConnection *connection,
        const QString &aliasName, const Utilities::Password &password)
{
    QMetaObject::invokeMethod(this, "onConnectToAliasSlot",
            Qt::QueuedConnection, Q_ARG(UnknownConnection*, connection),
            Q_ARG(QString, aliasName),
            Q_ARG(TIN_project::Utilities::Password, password));
}

void MainServer::onCreateAlias(UnknownConnection *connection,
        const QString &aliasName, const Utilities::Password &password)
{
    for (int i = 0; i < m_aliases.size(); ++i) {
        if (m_aliases[i]->getName() == aliasName) {
            connection->sendAliasNotCreated();
            return;
        }
    }

    Alias *alias = new Alias(aliasName, password);
    alias->start();
    boost::shared_ptr<Alias> ptr(alias);
    m_aliases.append(ptr);

    connection->sendAliasCreated();
}

void MainServer::onRemoveAlias(UnknownConnection *connection,
        const QString &aliasName, const Utilities::Password &password)
{
    for (int i = 0; i < m_aliases.size(); ++i) {
        if (m_aliases[i]->getName() == aliasName) {
            if (m_aliases[i]->checkPassword(password)) {
                m_aliases[i]->stop();
                m_aliases.removeAt(i);
                connection->sendAliasRemoved();
            } else {
                connection->sendAliasNotRemoved();
            }

            return;
        }
    }

    connection->sendAliasNotRemoved();
}

void MainServer::addNewConnection(UnknownConnection *connection)
{
    boost::shared_ptr<UnknownConnection> shared(connection);
    m_connections.append(shared);
}

int MainServer::start(const QHostAddress& address, quint16 port)
{
    if (m_application.isRunning()) {
        qDebug() << "Another instance of server is now running";
        return -1;
    }

    if (!m_server.listen(address, port)) {
        qDebug() << "Unable to listen on this address and port";
        return -2;
    }

    qDebug() << "Server started at " << m_server.serverAddress().toString()
            << ":" << m_server.serverPort();

    qDebug() << "Creating new alias process start...";
    Utilities::Password pass(QString("abc"));
    Alias *alias = new Alias("TEST", pass );
       alias->start();
       boost::shared_ptr<Alias> ptr(alias);
       m_aliases.append(ptr);
    qDebug() << "Creating new alias process end - alias should be created.";

    return m_application.exec();
}

void MainServer::stopServer(int exitCode)
{
    if (m_server.isListening()) {
        qDebug() << "Closing server...";
        m_server.close();
    }

    if (m_connections.size() > 0) {
        qDebug() << "Disconnecting clients...";
        for (int i = 0; i < m_connections.size(); ++i) {
            m_connections[i]->disconnectClientSynch();
        }
        m_connections.clear();
    }

    if (m_aliases.size() > 0) {
        qDebug() << "Stopping aliases...";
        for (int i = 0; i < m_aliases.size(); ++i) {
            m_aliases[i]->stop();
        }
        m_aliases.clear();
    }

    qDebug() << "Exiting event loop...";
    m_application.exit(exitCode);
}

void MainServer::onConnectToAliasSlot(UnknownConnection *connection,
        QString aliasName, TIN_project::Utilities::Password password)
{
    for (int i = 0; i < m_aliases.size(); ++i) {
        if (m_aliases[i]->getName() == aliasName) {
            if (m_aliases[i]->checkPassword(password)) {
                boost::shared_ptr<UnknownConnection> con;

                for (int j = 0; j < m_connections.size(); ++j) {
                    if (m_connections[j].get() == connection) {
                        con = m_connections[j];
                        m_connections.removeAt(j);
                        break;
                    }
                }

                m_aliases[i]->addClient(con);
            } else {
                connection->sendNotConnectedToAlias();
            }
            return;
        }
    }

    connection->sendNotConnectedToAlias();
}

void MainServer::onAddDirecotrySlot(UnknownConnection* connection,
        QString aliasName, TIN_project::Utilities::Password password)
{
    for (int i = 0; i < m_aliases.size(); ++i) {
        if (m_aliases[i]->getName() == aliasName) {
            if (m_aliases[i]->checkPassword(password)) {
                boost::shared_ptr<UnknownConnection> con;

                for (int j = 0; j < m_connections.size(); ++j) {
                    if (m_connections[j].get() == connection) {
                        con = m_connections[j];
                        m_connections.removeAt(j);
                        break;
                    }
                }

                m_aliases[i]->addDaemon(con);
            } else {
                connection->sendNotConnectedToAlias();
            }
            return;
        }
    }

    connection->sendNotConnectedToAlias();
}

} //namespace server
} //namespace TIN_project
