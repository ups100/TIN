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
        : m_name(name), m_password(password)
{
    // TODO and disable copy constructor

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

    static int ile= 0;
    ile++;
    if(ile == 2) {
        qDebug() << "Przesylanie .... ";
        onPullFileFrom(NULL, *(new Utilities::FileLocation(QString(),1, Utilities::Identifier(QString(),QString()))));     // TODO usun to
    }
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

    //stop flie transfers
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
    qDebug() << "Connection closed with client: "; // TODO wypisac jakies info o kliencie

}

void Alias::onConnectionClosed(DaemonConnection* daemon)
{
    qDebug() << "Daemon exit.";
}

void Alias::onFileFound(DaemonConnection* daemon,
        const Utilities::AliasFileList& location)
{
    qDebug() << "Client search for file and Daemon found file ";
    // TODO jak wykombinować kogo powiadomić ?
    // niby mogę chwilow przechowywać klienta który czeka na odpowiedź
    // i teraz ją wysłać
}

void Alias::onFileList(DaemonConnection* daemon,
        const Utilities::AliasFileList& list)
{
    qDebug() << "onFileList not implemented ";
    m_tmpAliasFileList->merge(list);

    if (!--m_waitForDaemons) {
        m_clients.first()->sendFileList(*m_tmpAliasFileList);
        m_tmpAliasFileList.reset();
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
    // TODO onFindFile
    qDebug() << "Klient chce znalezc plik " << name;

    foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
        dc->sendFindFile(name);
    }

}

void Alias::onListAlias(ClientConnection* client)
{
    qDebug() << "onListAlias not implemented";
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
}

void Alias::onPullFileFrom(ClientConnection* client,
        const Utilities::FileLocation& location)
{
    boost::shared_ptr<FileTransferServer> fts(new FileTransferServer(this,2,QFile("/home/major/aaa/abc").size()));
    m_transfers.append(fts);

    if(fts->startFileServer(QHostAddress::LocalHost)==false) qDebug() << "Zle serwer ftp nie wystartowal";

    qDebug() << "rozmiar pliku do wysylki" << QFile("/home/major/aaa/abc").size();
    m_daemons[1]->sendSendFile("abc",QHostAddress::LocalHost,fts->getPort());
    m_daemons[0]->sendReciveFile("abc",QHostAddress::LocalHost,fts->getPort(),QFile("/home/major/aaa/abc").size());
}

void Alias::onPushFileToAlias(ClientConnection* client, const QString& path,
        quint64 size)
{
    qDebug() << "onPushFileAlias not implemented";
}

void Alias::onRemoveFromAlias(ClientConnection* client, const QString& fileName)
{
    qDebug() << "onRemoveFromAlias not implemented";
}

} //namespace server
} //namespace TIN_project
