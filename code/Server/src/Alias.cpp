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
#include "FileLocation.h"
#include <QDebug>
#include <QFile>

namespace TIN_project {
namespace Server {

Alias::~Alias()
{
    // TODO
}

Alias::Alias(const QString& name, Utilities::Password password)
        : m_name(name), m_password(password),
          m_waitForDaemons(0)
{
    m_lastAliasAction = none;
    qRegisterMetaType<ClientConnection*>("ClientConnection*");
    qRegisterMetaType<DaemonConnection*>("DaemonConnection*");

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

    //in this case all client transaction is closed
    m_waitForDaemons = 0;
    //m_tmpAliasFileList is reseting in invokeMethod above

    // removes closed Client
    QMetaObject::invokeMethod(this, "removeClientSlot",
                Qt::QueuedConnection, Q_ARG(ClientConnection*, client));
}

void Alias::onConnectionClosed(DaemonConnection* daemon)
{
    qDebug() << "Daemon exit.";

    // one daemon left so we don't wait for messages from it
    // for example when onListAlias
    if (m_waitForDaemons > 0)
        --m_waitForDaemons;

    // if this was a last daemon try to performAction
    if (m_waitForDaemons == 0 && m_lastAliasAction != none)
        performLastAliasAction();

    // remove closed Deamon
    QMetaObject::invokeMethod(this, "removeDaemonSlot",
                    Qt::QueuedConnection, Q_ARG(DaemonConnection*, daemon));
}

void Alias::onFileFound(DaemonConnection* daemon,
        const Utilities::AliasFileList& location)
{
    qDebug() << "Client search for file and Daemon found file ";
    // if some goes wrong, for example client was unexpectedly disconnected
    if (!m_waitForDaemons) return;

    m_tmpAliasFileList->merge(location);

    if (!--m_waitForDaemons) {
        if (m_tmpAliasFileList->getSize() > 0)
            m_clients.first()->sendFileFound(*m_tmpAliasFileList);
        else
            m_clients.first()->sendFileNotFound();

        m_tmpAliasFileList.reset();
        m_lastAliasAction = none;
     }
}

void Alias::onFileList(DaemonConnection* daemon,
        const Utilities::AliasFileList& list)
{
    // if some goes wrong, for example client was unexpectedly disconnected
    if (!m_waitForDaemons) return;
    qDebug() << "onFileList - daemon daje liste plikow ";
    m_tmpAliasFileList->merge(list);

    if (!--m_waitForDaemons) {
        m_clients.first()->sendFileList(*m_tmpAliasFileList);
        m_tmpAliasFileList.reset();
        m_lastAliasAction = none;
    }
}

void Alias::onFileTransferStarted(FileTransferServer *transfer)
{
    qDebug() << "FileTransferStarted not implemented";
}

/**
 * Notify if transfer completed
 */
void Alias::onFileTransferCompleted(FileTransferServer *transfer)
{
    qDebug() << "FileTransferCompleted not implemented";
}

/**
 * Notify if an error occurred
 */
void Alias::onFileTransferError(FileTransferServer *transfer)
{
    qDebug() << "FileTransferError ";
}

void Alias::onFindFile(ClientConnection* client, const QString& name)
{
    qDebug() << "Klient chce znalezc plik: " << name;

    if (m_daemons.isEmpty()) {
        client->sendFileNotFound();
        return;
    }

    m_lastAliasAction = onFindFileAction;

    m_tmpAliasFileList = boost::shared_ptr<Utilities::AliasFileList>(
            new Utilities::AliasFileList());
    m_waitForDaemons = m_daemons.size();

    foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
        dc->sendFindFile(name);
    }

}

void Alias::onListAlias(ClientConnection* client)
{
    qDebug() << "onListAlias - wylistuj alias " << m_name;

    if (m_daemons.isEmpty()) {
        client->sendFileList(Utilities::AliasFileList());
        return;
    }

    m_lastAliasAction = onListAliasAction;

    m_tmpAliasFileList = boost::shared_ptr<Utilities::AliasFileList>(
            new Utilities::AliasFileList());
    m_waitForDaemons = m_daemons.size();

    foreach (boost::shared_ptr<DaemonConnection> dcon, m_daemons){
    dcon->sendListYourFiles();
}
}

void Alias::onNoSuchFile(DaemonConnection* daemon)
{
    qDebug() << "Client call FindFile, but daemon doesn't have it";

    if (m_waitForDaemons > 0)
            --m_waitForDaemons;

    // what if it was last Daemon ?
    if (m_waitForDaemons == 0)
        performLastAliasAction();
}

void Alias::onPullFileFrom(ClientConnection* client,
        const Utilities::FileLocation& location)    // TODO
{
    boost::shared_ptr<FileTransferServer> fts(new FileTransferServer(this,2,QFile("/home/major/aaa/abc").size()));
    m_transfers.append(fts);

    if(fts->startFileServer(QHostAddress::LocalHost)==false)
        qDebug() << "in Alias: FileTransferServer don't start properly while Pull";

    qDebug() << "rozmiar pliku do wysylki" << QFile("/home/major/aaa/abc").size();
    m_daemons[1]->sendSendFile("abc",QHostAddress::LocalHost,fts->getPort());
    m_daemons[0]->sendReciveFile("abc",QHostAddress::LocalHost,fts->getPort(),QFile("/home/major/aaa/abc").size());
}

void Alias::onPushFileToAlias(ClientConnection* client, const QString& path,
        quint64 size)
{
    qDebug() << "onPushFileAlias not implemented";

    if (m_daemons.size() == 0) {
        qDebug() << "in Alias: Push but no daemons online";   // TODO rozkmina aby powiadomić clienta
        client->sendFileTransferError();
        return;
    }

    // TODO after implementation this function delete this  three calls (and return)
    qDebug() << "Alias::onPushFileToAlias error because of Unimplemented";
    client->sendFileTransferError();
    return;

    boost::shared_ptr<FileTransferServer> fts(new FileTransferServer(this,2,size));

    if (fts->startFileServer(QHostAddress::LocalHost)==false) {
        qDebug() << "in Alias: FileTransferServer don't start properly while Push.";
        return;
    }

    // szukam pliku
    // i puszuję do każdego demona który ten plik ma a sprawdzam który demon odpowiada klientowi i z niego robię sendera

    // Even I know that I can Push this file to anybody but:
    // How can I figure out from which daemon I should send this file?
    // I only know ClientConnection object which tells nothing about neither client nor daemon
}

void Alias::onRemoveFromAlias(ClientConnection* client, const QString& fileName)
{
    qDebug() << "onRemoveFromAlias" << fileName;

    // we don't wait for answer from them
    foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
        dc->sendRemoveFile(fileName);
    }
    client->sendFileRemoved();
}

void Alias::removeDaemonSlot(DaemonConnection *dc)
{
    int i=0;
    for (i=0; i<m_daemons.size(); ++i) {
        if (m_daemons[i].get() == dc) {
            break;
        }
    }
    m_daemons.removeAt(i);
}

void Alias::removeClientSlot(ClientConnection *cc)
{
    m_tmpAliasFileList.reset(); // deleting temporary

    int i=0;
    for (i=0; i<m_clients.size(); ++i) {
        if (m_clients[i].get() == cc) {
            break;
        }
    }
    m_clients.removeAt(i);
}

void Alias::performLastAliasAction()
{
    qDebug() << "Performing last action in Alias."
             << "Attention - Unusual situation."
             << m_lastAliasAction;

    switch(m_lastAliasAction) {
        case none:
            return;
            break;
        case onFindFileAction:
            if (m_tmpAliasFileList->getSize() > 0)
                m_clients.first()->sendFileFound(*m_tmpAliasFileList);
            else
                m_clients.first()->sendFileNotFound();

            break;
        case onListAliasAction:
            m_clients.first()->sendFileList(*m_tmpAliasFileList);
            break;
     }

    m_tmpAliasFileList.reset();
    m_lastAliasAction = none;
}

} //namespace server
} //namespace TIN_project
