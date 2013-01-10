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
#include <QDebug>

namespace TIN_project {
namespace Server {

Alias::~Alias()
{

}

Alias::Alias(const QString& name, Utilities::Password password)
        : m_name(name), m_password(password)
{

}

void Alias::addClient(boost::shared_ptr<UnknownConnection> client, const Utilities::Identifier& id)
{
    if (!m_thread.isRunning()) {
        qDebug() << "Alias is not running";
        return;
    }

    ClientConnection *connection = new ClientConnection(
            client->convertToOtherConnection(), &m_thread, this,id);

    boost::shared_ptr<ClientConnection> shared(connection);
    m_clients.append(shared);

    shared->sendConnectedToAlias();
}

void Alias::addDaemon(boost::shared_ptr<UnknownConnection> daemon, const Utilities::Identifier& id)
{
    if (!m_thread.isRunning()) {
        qDebug() << "Alias is not running";
        return;
    }

    DaemonConnection *connection = new DaemonConnection(
            daemon->convertToOtherConnection(), &m_thread, this, id);

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
    foreach(boost::shared_ptr<ClientConnection> connection, m_clients){
    connection->disconnectFromAliasSynch();
}
    m_clients.clear();

    //disconnects daemons
    foreach(boost::shared_ptr<DaemonConnection> connection, m_daemons){
    connection->disconnectFromAliasSynch();
}
    m_daemons.clear();

    //stop file transfers
    foreach(boost::shared_ptr<FileTransferServer> connection, m_transfers){
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
    m_tmpAliasFileList->merge(list);

    if (!--m_waitForDaemons) {
        m_clients.first()->sendFileList(*m_tmpAliasFileList);
        m_tmpAliasFileList.reset();
    }
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
    m_tmpAliasFileList = boost::shared_ptr<Utilities::AliasFileList>(
            new Utilities::AliasFileList());
    m_waitForDaemons = m_daemons.size();

    foreach (boost::shared_ptr<DaemonConnection> dcon, m_daemons){
    dcon->sendListYourFiles();
}
}

void Alias::onNoSuchFile(DaemonConnection* daemon)
{
    qDebug() << "Somebody call No such File";
}

void Alias::onPullFileFrom(ClientConnection* client,
        const Utilities::FileLocation& location)
{

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
