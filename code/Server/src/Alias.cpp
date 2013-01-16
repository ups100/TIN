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
#include <QRegExp>
#include <QHostInfo>

namespace TIN_project {
namespace Server {

Alias::~Alias()
{
    // TODO
}

Alias::Alias(const QString& name, Utilities::Password password, QHostAddress address)
        : m_name(name), m_password(password), m_senderDaemon(NULL),
          m_removeFind(false), m_address(address), m_onTransmission(false)
{
    m_currentAction = NONE;
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
    foreach(FileTransferServer *connection, m_transfers){
    connection->disconnectFromAliasSynch();
}
    m_transfers.clear();

    QEventLoop loop;

    QObject::connect(&m_thread, SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(0, &m_thread, SLOT(quit()));

    loop.exec();
}

void Alias::onConnectionClosed(ClientConnection* client) // TODO w ogole
{
    qDebug() << "Connection closed with client: ";

    if (m_currentAction != NONE) {
        qDebug() << "Client exit while waiting for some operations "
                 << " it will cause wrong state in the Alias ";
    }

    if (m_onTransmission == true)
        qDebug() << "Transfer initialize by user is still in progress.";
    //in this case all answers should be ignore
    m_currentAction = NONE;

    // removes closed Client
    QMetaObject::invokeMethod(this, "removeClientSlot",
                Qt::QueuedConnection, Q_ARG(ClientConnection*, client));
}

void Alias::onConnectionClosed(DaemonConnection* daemon)
{
    qDebug() << "Daemon exit.";

    //common actions in all states:
    // count - usually should be 1, or 0, never greater
    int count = m_actionDaemon.count(daemon);
    //in case when daemon disconnected after send all required data:

    if (count == 1)
        m_actionDaemon.removeOne(daemon);
    // for safety checking:
    if (count > 1)
        qDebug() << "Alias::onClosed Daemons cloning. Call help.";

    // different actions for states
    switch (m_currentAction) {
        case NONE:
            break;
        case PULL_TRANSFER:
            // if it is last daemon I waiting
            if (m_actionDaemon.size() == 0)
                QTimer::singleShot(0, this, SLOT(performPullActionSlot()));
            break;
        case FIND_FILE:
            if (m_actionDaemon.size() == 0)
                QTimer::singleShot(0, this, SLOT(performFindFileSlot()));
            break;
        case REMOVE_FILE:
            if (m_actionDaemon.size() == 0)
                QTimer::singleShot(0, this, SLOT(performRemoveFileSlot()));
            break;
        case LIST_ALIAS:
            if (m_actionDaemon.size() == 0) {
                m_clients.first()->sendFileList(*m_tmpAliasFileList);
                m_tmpAliasFileList.reset();
                m_actionDaemon.clear();
                m_currentAction = NONE;
            }
            break;
        case PUSH_TRANSFER:
            if (m_actionDaemon.size() == 0)
                QTimer::singleShot(0, this, SLOT(performPushActionSlot()));
            break;
    }

    // remove closed Deamon
    QMetaObject::invokeMethod(this, "removeDaemonSlot",
                    Qt::QueuedConnection, Q_ARG(DaemonConnection*, daemon));
}

void Alias::onFileFound(DaemonConnection* daemon,
        const Utilities::AliasFileList& location)   //TODO dodajPush
{
    //qDebug() << "Client search for file and Daemon found file ";
    // if some goes wrong, for example client was unexpectedly disconnected
    if (m_daemons.size() == 0 || m_actionDaemon.size() == 0) {
        qDebug() << "Ups. There is no daemon in alias.";
        return;
    }

    int count=0; // count daemon occurs below

    switch (m_currentAction) {
        case NONE:
            return;
        case LIST_ALIAS:
            qDebug() << "onFileFound: state List_Alias - wrong state";
            return;
        case PULL_TRANSFER:
            qDebug() << "inAlias::onFileFound /transfer/ daemon have"
            << m_tmpAliasFileList->getSize() << " files.";

            // Count number of object in the list
            count = m_actionDaemon.count(daemon);

            // for safety checking the correctness of counts
            if (count > 1 || count <= 0) {
                qDebug() << "Alias::noSuchFIle ERROR daemon multiplication or zeros. Call help.";
                return;
            }

            // if above is correct - Check if this is that daemon who provide file which should be pull
            if (daemon->getIdentifier().getId() == m_location.first()->getOwnerIdentifier().getId()) {
                // in this line m_senderDaemon should be NULL
                if (m_senderDaemon != NULL)
                    qDebug() << "Alias::onFIleFound Error m_sender NOT NULL Call help.";

                // remember daemonConnection pointer to create connection in future
                m_senderDaemon = daemon;
            }

            // line below says that this daemon just send me his answer
            // so I don't wait for him any more
            m_actionDaemon.removeOne(daemon);

            // if it was last daemon which should give me answer
            if (m_actionDaemon.size() == 0)
                performPullAction();

            return; // return from case PULL_TRANSFER
        case FIND_FILE:
            m_tmpAliasFileList->merge(location);
            qDebug() << "inAlias::onFileFound getSize" << m_tmpAliasFileList->getSize();
            //daemon send his answer so it can be removed from waiting queue
            m_actionDaemon.removeOne(daemon);

            if (m_actionDaemon.size() == 0)
                performFindFile();

            break;
        case REMOVE_FILE:
            m_actionDaemon.removeOne(daemon);
            // it cause that client should see the information FileRemoved
            m_removeFind = true;
            if (m_actionDaemon.size() == 0)
                performRemoveFile();
            break;
        case PUSH_TRANSFER:
            // daemon just gave me his answer so:
            m_actionDaemon.removeOne(daemon);
            // but it means that I should send him this file
            m_receiverDaemon.append(daemon);
            if (m_actionDaemon.size() == 0)
                performPushAction();
            break;

    }//switch end


}

void Alias::onFileList(DaemonConnection* daemon,
        const Utilities::AliasFileList& list)
{
    if (m_currentAction != LIST_ALIAS) {
        qDebug() << "Wrong state. Should be List_alias";
    }
    qDebug() << "Daemon give his file list which size is: "<< list.getSize();
    // if some goes wrong, for example client was unexpectedly disconnected
    //if (!m_waitForDaemons) return;

    m_tmpAliasFileList->merge(list);

    // mark that this daemon gives his list
    m_actionDaemon.removeOne(daemon);

    if (m_actionDaemon.size() == 0) {// this block is copied into onConnectionClosed
        qDebug() << "All alias listing is send to Client. Number of element "
                 << m_tmpAliasFileList->getSize();
        m_clients.first()->sendFileList(*m_tmpAliasFileList);
        m_tmpAliasFileList.reset();
        m_actionDaemon.clear();
        m_currentAction = NONE;
    }
}

void Alias::onFileTransferStarted(FileTransferServer *transfer)
{
    // Client should know about it:
    m_clients.first()->sendFileTransferStarted();

    switch (m_currentAction) {
        case PULL_TRANSFER:
            qDebug() << "Start pulling... ";
            break;
        case PUSH_TRANSFER:
            qDebug() << "Start pushing... ";
            break;
        default:
            if (m_onTransmission == false)
                qDebug() << "Wrong state when TransferStarted";
            break;
    }
}

/**
 * Notify if transfer completed
 */
void Alias::onFileTransferCompleted(FileTransferServer *transfer)
{
    // Client should know about it:
    m_clients.first()->sendFileTransferFinished();

    switch (m_currentAction) {
        case PULL_TRANSFER:
            qDebug() << "Pull FileTransferCompleted";
            afterPullAction();
            break;
        case PUSH_TRANSFER:
            qDebug() << "Push FileTransferCompleted";
            afterPushAction();
            break;
        default:
            // if there is no transmission and state is different Pull or Push
            if (m_onTransmission == false)
                qDebug() << "Wrong state when TransferCompleted";
            break;
    }
}

/**
 * Notify if an error occurred
 */
void Alias::onFileTransferError(FileTransferServer *transfer)
{
    // Client should know about it:
    m_notifyClient.first()->sendFileTransferError();

    switch (m_currentAction) {
        case PULL_TRANSFER:
            qDebug() << "FileTransferError while Pull";
            afterPullAction();
            break;
        case PUSH_TRANSFER:
            qDebug() << "FileTransferError while Push";
            afterPushAction();
            break;
        default:
            if (m_onTransmission == false)
                qDebug() << "Wrong state when TransferError";
            break;
    }
}

void Alias::onFindFile(ClientConnection* client, const QString& name)
{
    if (m_currentAction != NONE) {
        qDebug() << "Wrong state. Should be None" << m_currentAction;
        return;
    }
    qDebug() << "Client searching file: " << name;

    if (m_daemons.isEmpty()) {
        qDebug() << "Send File NOT found to Client "
                 << "because in alias:" << m_name << "is no daemon";
        client->sendFileNotFound();
        return;
    }

    m_currentAction = FIND_FILE;

    m_tmpAliasFileList = boost::shared_ptr<Utilities::AliasFileList>(
            new Utilities::AliasFileList());

    //m_waitForDaemons = m_daemons.size();

    foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
        m_actionDaemon.append(dc.get());
        dc->sendFindFile(name);
    }

}

void Alias::onListAlias(ClientConnection* client, bool remoteOnly)
{
    if (m_currentAction != NONE) {
        qDebug() << "IN onListAlias. Wrong state. Should be none" << m_currentAction;
    }
    qDebug() << "onListAlias - begin " << m_name;

    if (m_daemons.isEmpty()) {
        qDebug() << "Send empty file list to Client"
                 << "because in this Alias" << m_name << "is no daemon";
        client->sendFileList(Utilities::AliasFileList());
        return;
    }

    m_currentAction = LIST_ALIAS;

    m_tmpAliasFileList = boost::shared_ptr<Utilities::AliasFileList>(
            new Utilities::AliasFileList());
    //m_waitForDaemons = m_daemons.size();

    foreach (boost::shared_ptr<DaemonConnection> dcon, m_daemons) {
        if ( *client == *dcon )
            continue;
        m_actionDaemon.append(dcon.get());
        dcon->sendListYourFiles();
    }

}

void Alias::onNoSuchFile(DaemonConnection* daemon)
{
    qDebug() << "onNoSuchFile: state:" << m_currentAction;

    if (m_currentAction == NONE) {
        qDebug() << "onNoSuchFile: state NONE is wrong state for this method";
        return;
    }

    // Here is some common features
    // which should by executed always in this function
    int count = m_actionDaemon.count(daemon);
    if (count == 1)
        m_actionDaemon.removeOne(daemon);
        // albo gdyby jednak powyższa linijka wywoływała destruktor klasy to:
        // m_actionDaemon.takeAt(m_actionDaemon.indexOf(daemon));
    // for safety checking:
    if (count > 1 || count < 0)
        qDebug() << "Alias::noSuchFIle ERROR errror. Call help.";

    // different behavior
    switch(m_currentAction) {
        case NONE:
            return;
        case LIST_ALIAS:
            qDebug()<<"onNoSuchFile: state ListAlias - wrong state";
            return;
        case PULL_TRANSFER:
            // if it was last daemon which should give me answer
            if (m_actionDaemon.size() == 0)
                performPullAction();
            return;
        case FIND_FILE:
            if (m_actionDaemon.size() == 0)
                performFindFile();
            break;
        case REMOVE_FILE:
            if (m_actionDaemon.size() == 0)
                performRemoveFile();
            break;
        case PUSH_TRANSFER:
            if (m_actionDaemon.size() == 0)
                performPushAction();
            break;
    }//switch end

}

void Alias::onPullFileFrom(ClientConnection* client,
        const Utilities::FileLocation& location)
{
    if (m_currentAction != NONE) {
        qDebug() << "Wrong state. Should by none." << m_currentAction;
        return;
    }
    qDebug() << "From alias" << m_name << "Pull file:" << location.getPath()
             << "fileSize: " <<location.getSize()
             << "owner: " << location.getOwnerIdentifier().getId();  // TODO delete this and line above. first line zostaw

    if (m_daemons.isEmpty() || m_daemons.size() == 1) {
        qDebug() << "Send File stop, because in alias:"
                << m_name << "is no daemon or one";
        client->sendFileTransferError();
        return;
    }

    if (location.getSize() <= 0) {
        qDebug() << "Transmission empty file. Abort.";
        client->sendFileTransferError();
        return;
    }

    const QString fileName(location.getPath());

    // remember here a location object for future use
    m_location.append(new Utilities::FileLocation(location));
    qDebug() << "after appending: size:" << m_location.first()->getSize() //TODO del this 3 line
             << "fileName: " << m_location.first()->getPath()
             << "owner: " << m_location.first()->getOwnerIdentifier().getId();

    // remember who should be notify about Pull action
    m_notifyClient.append(client);

    // PREPARING TO PULL FILE

    // for safety checking:
    if (m_senderDaemon != NULL) {
        qDebug() << "Alias::onPullFile: m_senderDaemon is not NULL";
    }

    // so everything above goes right :
    m_currentAction = PULL_TRANSFER;

    // remember sender Identifier - this is need here in onFileFound method
    //m_senderDaemonIdentifier = location.getOwnerIdentifier();

    foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
        // remember who is reciver
        if ( *client == *dc) {
            m_receiverDaemon.push_front(dc.get());
            continue;   // don't append this daemon to m_actionDaemons List
        }
        // NEW CONCEPTION - adding this if:
        // finding sender
        if ( (dc.get()->getIdentifier().getId() == location.getOwnerIdentifier().getId() ) ) // TODO TODO TODO TODO TODO - komentarz napraw nizej:
                //&& (dc.get()->getIdentifier().getPath() == location.getOwnerIdentifier().getPath() ) )
            m_senderDaemon = dc.get();  // TODO sprawdź czy to nie rozmnaża shared_ptr - czy program konczy sie poprawnie?

        /* PREVIOUS PULL CONCEPTION
        //all daemon without that corresponding to client are searched
        // send a request to all Daemon to check if they have pulling file
        m_actionDaemon.append(dc.get()); // I point that I wait for this daemon action completed
        dc->sendFindFile(fileName);
        */
     }

    qDebug() << "rozmiar pliku do wysylki - pull" << location.getSize();   // TODO remove this line

    //NEW CONCEPTION:
    performPullAction(); // executing next commands


    //previous conception comment below
//NOT// Waiting for all Daemon acknowledge. Next instruction are in onFindFile or onNoSuchFile
}

void Alias::onPushFileToAlias(ClientConnection* client, const QString& path,
        quint64 size)
{
    if (m_currentAction != NONE) {
        qDebug() << "Wrong state. Should by none." << m_currentAction;
        return;
    }
    qDebug() << "Pushing a file" << path << "to alias:" << m_name;

    if (m_daemons.isEmpty() || m_daemons.size() == 1) {
        qDebug() << "Stop push. because in alias:"
                << m_name << "is no daemon or one";
        client->sendFileTransferError();
        return;
    }

    if (size <= 0) {
        qDebug() << "Transmission empty file. Abort.";
        client->sendFileTransferError();
        return;
    }
    // state is set below

    //integrity (and safety) checking:
    if (m_senderDaemon != NULL) {
        qDebug() << "Something goes wrong. I can't push,"
                 << "because m_senderDaemon isn't null at push start";
        client->sendFileTransferError();
        //m_clients.first()->sendFileTransferError();
        return;
    }

    // search for daemon which corresponds the client
    // and check other running daemons
    foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
        if ( *client == *dc ) {
            //remember who is sender
            m_senderDaemon = dc.get(); //and don't append this daemon to m_actionDaemons List
            continue;
        }
        m_actionDaemon.append(dc.get()); // I point that I wait for this daemon action completed
        // sendFindFile signal to m_actionDaemons is execution a few line below in know reason
    }

    // if there is no thread which corresponds to the client
    if (m_senderDaemon == NULL) {
        qDebug() << "inAlias: Can not find sender Daemon before Push " << path;
        client->sendFileTransferError();
        m_actionDaemon.clear();
        return;
    }

    // if everything above goes right I set:
    m_currentAction = PUSH_TRANSFER;

    // save file properties - which will be need when transfer start in performPushAction
    // note I never use this FileLocation identifier so it is empty
    qDebug() << "Server knew size of file: "<< size;    //TODO delet this line
    m_location.append( new Utilities::FileLocation(path, size, Utilities::Identifier(QString())) ); //TODO remember to free this

    // and send a request to all Daemon to check if they have pushed file
    foreach(DaemonConnection *dc, m_actionDaemon) {
        //all daemon without that corresponding to client are searched
        dc->sendFindFile(path);
    }

}

void Alias::onRemoveFromAlias(ClientConnection* client, const QString& fileName)
{
    if (m_currentAction != NONE) {
        qDebug() << "Wrong state. Should by None" << m_currentAction;
        return;
    }
    qDebug() << "onRemoveFromAlias" << fileName;

    if (m_daemons.isEmpty()) {
        qDebug() << "Send File NOT removed to Client "
                 << "because in alias:" << m_name << "is no daemon";
        client->sendFileNotRemoved();
        return;
    }
    m_currentAction = REMOVE_FILE;

    // line below tells initially that there is no file to delete
    m_removeFind = false;

    // save the fileName which should be removed
    m_removeName = fileName;

    QString searchName(fileName);
    qDebug() << "szukam: " << searchName.replace(QRegExp(QString() + fileName + "?/"), "./");


    // we wait for their answers
    foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
        m_actionDaemon.append(dc.get());
        dc->sendFindFile(searchName);
    }
    // deleting is executed in performRemoveFile()

    m_currentAction = REMOVE_FILE;
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

void Alias::performPullActionSlot()
{
    performPullAction();
}

void Alias::performFindFileSlot()
{
    performFindFile();
}

void Alias::performRemoveFileSlot()
{
    performRemoveFile();
}

void Alias::performPushActionSlot()
{
    performPushAction();
}

void Alias::performPullAction()
{
    qDebug() << "Alias: performPullAction ";

    if (m_currentAction != PULL_TRANSFER) {
        qDebug() << "We shouldn't be in this state / Pull_transfer need";
    }

    if (m_senderDaemon == NULL || m_receiverDaemon.size() == 0) {
        qDebug() << "Alias: Nobody wants sending or receiving a files";
        m_notifyClient.first()->sendFileTransferError();
        return; // TODO wyczysc dane w tym przypadku
    }

    if (m_senderDaemon == m_receiverDaemon.first()) {
        qDebug() << "The same daemons for sender and receiver. Error";
        m_clients.first()->sendFileTransferError();
        return;
    }

    // preparing data
    qDebug() << "preparing data";
    QString fileName(m_location.first()->getPath());
    int fileSize = m_location.first()->getSize();
    qDebug() << "filePath:" << fileName << "size:" << fileSize
             << "ownerID" << m_location.first()->getOwnerIdentifier().getId(); //TODO del this

    // PULL FILE EXECUTION STARTED // between ONLY 2 daemons:
    FileTransferServer *fts = new FileTransferServer(this, 2, fileSize);
    m_transfers.append(fts);

    if(fts->startFileServer()==false) {
        qDebug() << "in Alias: FileTransferServer don't start properly while Pull";
        m_notifyClient.first()->sendFileTransferError();
        fts->deleteLater();
        m_currentAction = NONE;
        return;
    }
    qDebug() << "FTS for pull address: "<< /*fts->getAddress()*/ this->m_address
             << "FTS port: " << fts->getPort();
    m_onTransmission = true;

    // proper send action started
    qDebug() << "Notify sender... ";
    m_senderDaemon->sendSendFile(fileName, this->m_address, fts->getPort());
    qDebug() << "Notify receiver";
    m_receiverDaemon.first()->sendReciveFile(fileName, this->m_address, fts->getPort(), fileSize);

    // don't change state here - it will be done in afterPullAction() method
}

void Alias::afterPullAction()
{
    if (m_currentAction != PULL_TRANSFER) {
        qDebug() << "Wrong state. Should be Pull_transfer";
        return;
    }

    m_currentAction = NONE;
    m_onTransmission = false;
    // if pull is ended - nothing to notifing
    m_notifyClient.clear();

    if (m_location.size() != 1)
        qDebug() << "Alias:afterPullAction: m_location multiply. Error";

    if (m_location.size() > 0 )
        delete m_location.first();
    m_location.clear();

    // some cleaning
    m_actionDaemon.clear();
    m_receiverDaemon.clear();
    m_senderDaemon = NULL;

    // TODO choice
    //m_transfers.first()->deleteLater();
    QTimer::singleShot(0, m_transfers.first(), SLOT(deleteLater()));
    m_transfers.clear();
}

void Alias::performPushAction()
{
    if (m_currentAction != PUSH_TRANSFER || !m_daemons.size()) {
        qDebug() << "Wrong state. Should be Push_transfer";
        m_clients.first()->sendFileTransferError();
        m_currentAction = NONE;
        return;
    }

    if (m_receiverDaemon.size() == 0) {
        qDebug() << "Nobody wants to receive pushed file.";
        qDebug() << "but we have " << m_daemons.size() <<"daemons";
        int x = qrand() % m_daemons.size();
        int i = 0; // counter
        foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
            if (x == i && dc.get() == m_senderDaemon ) {
                //qDebug() << "Alias:push, wylosowany ten sam daemon co klient " << x;  //TODO usun to
                x = (x+1) % m_daemons.size();
                break;
            }
            if (x == i)
                break;

            ++i; // must be here
        }
        i = 0;

        foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
            if (x == i && dc.get() == m_senderDaemon ) {
                qDebug() << "It can't be such situation."
                        << "Double daemons corresponding to client.";
            }
            if (x == i) {
                m_receiverDaemon.append(dc.get());
                break;
            }
            ++i; // must be here
        }
        if (m_receiverDaemon.size() == 0)
            qDebug() << "Amount of receiver is still 0 fatal error.";
    }

    //preparing data:
    QString filePath(m_location.first()->getPath());
    quint32 fileSize = m_location.first()->getSize();
    //qDebug() << "Ten sam rozmiar pliku co alias knew to: " << fileSize; //TODO remove
    // I don't need FileLocation object any more so:
    //delete m_location.first();
    //m_location.clear();

    // remember to add 1 to number of receivers
    qDebug() << "FileTransferServer starting. Number of receivers to:" << m_receiverDaemon.size();
    FileTransferServer *fts = new FileTransferServer(this,m_receiverDaemon.size() + 1, fileSize);
    m_transfers.append(fts);

    if (fts->startFileServer()==false) {
        qDebug() << "in Alias: FileTransferServer don't start properly while Push.";
        fts->deleteLater();
        m_clients.first()->sendFileTransferError();
        m_currentAction = NONE;
        return;
    }
    qDebug() << "FTS for pushing: address: " << /*fts->getAddress()*/ this->m_address
             << "port" << fts->getPort();
    m_onTransmission = true;

    // proper send action started
    qDebug() << "sender is notify to push a file";
    m_senderDaemon->sendSendFile(filePath, m_address, fts->getPort());
    foreach(DaemonConnection *dc, m_receiverDaemon) {
        qDebug() <<"receiver is notify to receive a file";  // TODO usun
        dc->sendReciveFile(filePath, this->m_address, fts->getPort(), fileSize);
    }

    // client is notify about start sending when all daemon will be connected

    // here is no state change - it will be done in afterPerformPushAction() method
}

void Alias::afterPushAction()
{
    if (m_currentAction != PUSH_TRANSFER) {
        qDebug() << "Wrong state. Should by Push_transfer" << m_currentAction;
        return;
    }
    qDebug() << "czyszczenie po pushowaniu";

    // transfer ended so changes state
    m_currentAction = NONE;
    m_onTransmission = false;

    if (m_location.size() > 0 )
        delete m_location.first();
    m_location.clear();

    // some cleaning
    m_actionDaemon.clear();
    m_receiverDaemon.clear();
    m_senderDaemon = NULL;

    // TODO choice
    //m_transfers.first()->deleteLater();
    QTimer::singleShot(0, m_transfers.first(), SLOT(deleteLater()));
    m_transfers.clear();
}

QHostAddress Alias::getServerIp()
{
    /*
    QHostInfo info;
    // the return value of server ip
    QHostAddress address = QHostAddress::Any;
    qDebug() << "Number of server addreses: " << info.addresses().size();

    if (!info.addresses().isEmpty()) {
        address = info.addresses().first();
        qDebug() << "First find ip address: " << address;

        if (address == QHostAddress::LocalHost || address == QHostAddress::LocalHostIPv6) {
            //address = info.addresses().takeAt(1);
            // maybe:
            address = info.addresses().at(1);
        }
    }*/
    //qDebug() << "inAlias::checkServerIP: " << m_address;

    return m_address;
}

void Alias::performFindFile()
{
    if (m_currentAction != FIND_FILE) {
        qDebug() << "Wrong state, should be Find_File";
    }

    if (m_tmpAliasFileList->getSize() > 0) {
        qDebug() << "Client is notify that file is found";
        m_clients.first()->sendFileFound(*m_tmpAliasFileList);
    }
    else {
        qDebug() << "Client is notify that file is NOT found";
        m_clients.first()->sendFileNotFound();
    }

    m_tmpAliasFileList.reset();
    m_actionDaemon.clear();

    m_currentAction = NONE;
}

void Alias::performRemoveFile()
{
    if (m_currentAction != REMOVE_FILE) {
        qDebug() << "Wrong state, should be Remove_file";
    }

    if (m_removeFind == true) {
        qDebug() << "inAlias: sending remove file" << m_removeName;
        foreach(boost::shared_ptr<DaemonConnection> dc, m_daemons) {
            dc->sendRemoveFile(m_removeName);
        }
    }//if end

    //client notify
    if (m_removeFind == true) {
        qDebug() << "Client is notify that file is removed.";
        m_clients.first()->sendFileRemoved();
    }
    else {
        qDebug() << "Client is notify that file is NOT removed";
        m_clients.first()->sendFileNotRemoved();
    }

    // some cleaning features
    m_removeFind = false;
    m_actionDaemon.clear();

    m_currentAction = NONE;
}

} //namespace server
} //namespace TIN_project
