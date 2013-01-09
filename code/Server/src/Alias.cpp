/**
 * @file Alias.cpp
 *
 * @date 28-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::Alias
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "Alias.h"
#include "Password.h"

#include <QTimer>
#include <QEventLoop>
#include <QtGlobal>

namespace TIN_project {
namespace Server {

Alias::~Alias()
{

}

Alias::Alias(const QString& name, Utilities::Password password)
        : m_name(name), m_password(password)
{

}

void Alias::addClient(boost::shared_ptr<UnknownConnection> client)
{
    if (!m_thread.isRunning()) {
        qDebug() << "Alias is not running";
        return;
    }

    ClientConnection *connection = new ClientConnection(
            client->convertToOtherConnection(), &m_thread, this);

    boost::shared_ptr<ClientConnection> shared(connection);
    m_clients.append(shared);

    shared->sendConnectedToAlias();
}

void Alias::addDaemon(boost::shared_ptr<UnknownConnection> daemon)
{
    if (!m_thread.isRunning()) {
        qDebug() << "Alias is not running";
        return;
    }

    DaemonConnection *connection = new DaemonConnection(
            daemon->convertToOtherConnection(), &m_thread, this);

    boost::shared_ptr<DaemonConnection> shared(connection);
    m_daemons.append(shared);

    shared->sendConnectedToAlias();
}

bool Alias::checkPassword(const Utilities::Password& password)
{

    return m_password.check(password);
}

const QString& Alias::getName()
{
    return m_name;
}

void Alias::start()
{
    if (m_thread.isRunning()) {
        qDebug() << "Alias is already running";
        return;
    }
    QEventLoop loop;

    QObject::connect(&m_thread, SIGNAL(started()), &loop, SLOT(quit()));
    QTimer::singleShot(0, &m_thread, SLOT(start()));

    loop.exec();
}

void Alias::stop()
{
    if (!m_thread.isRunning()) {
        qDebug() << "Alias is not running";
        return;
    }

    //disconnect clients
    foreach(boost::shared_ptr<ClientConnection> connection, m_clients) {
        connection->disconnectFromAliasSynch();
    }
    m_clients.clear();

    //disconnects daemons
    foreach(boost::shared_ptr<DaemonConnection> connection, m_daemons) {
        connection->disconnectFromAliasSynch();
    }
    m_daemons.clear();

    //stop flie transfers
    foreach(boost::shared_ptr<FileTransferServer> connection, m_transfers) {
        connection->disconnectFromAliasSynch();
    }
    m_transfers.clear();

    QEventLoop loop;

    QObject::connect(&m_thread, SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(0, &m_thread, SLOT(quit()));

    loop.exec();
}

void Alias::onConnectionClosed(ClientConnection* client)
{
    qDebug() << "Connection closed with client: ";
}

void Alias::onConnectionClosed(DaemonConnection* daemon)
{
    qDebug() << "Daemon exit.";
}

void Alias::onFileFound(DaemonConnection* daemon,
        const Utilities::AliasFileList& location)
{

}

void Alias::onFileList(DaemonConnection* daemon,
        const Utilities::AliasFileList& list)
{

}

void Alias::onFileTransferStarted(FileTransferServer *transfer)
{

}

/**
 * Notify if transfer completed
 */
void Alias::onFileTransferCompleted(FileTransferServer *transfer)
{

}

/**
 * Notify if an error occurred
 */
void Alias::onFileTransferError(FileTransferServer *transfer)
{

}

void Alias::onFindFile(ClientConnection* client, const QString& name)
{

}

void Alias::onListAlias(ClientConnection* client)
{

}

void Alias::onNoSuchFile(DaemonConnection* daemon)
{
    qDebug() << "Somebody call No such File";
}

void Alias::onPullFileFrom(ClientConnection* client,
        const Utilities::FileLocation& location)
{
    FileTransferServer fts(this, 2, 4);
    fts.startFileServer(QHostAddress::LocalHost);
    m_daemons[0]->sendSendFile("abc",QHostAddress::LocalHost,fts.getPort());
    m_daemons[1]->sendReciveFile("abc",QHostAddress::LocalHost,fts.getPort());
}

void Alias::onPushFileToAlias(ClientConnection* client, const QString& path,
        quint64 size)
{

}

void Alias::onRemoveFromAlias(ClientConnection* client, const QString& fileName)
{

}

} //namespace server
} //namespace TIN_project
