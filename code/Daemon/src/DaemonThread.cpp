/**
 * @file DaemonThread.cpp
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Daemon::DaemonThread
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "DaemonThread.h"
#include "FileTree.h"
#include "DaemonApplication.h"
#include "AliasFileList.h"
#include "Password.h"
#include <QDir>
#include <QFile>
#include <QBuffer>
#include <QXmlFormatter>
#include <QRegExp>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QFileInfo>
#include <QDateTime>
#include <QHostAddress>
#include <stdexcept>

namespace TIN_project {
namespace Daemon {

DaemonThread::DaemonThread()
{
    //create an object to connect to the server
    m_ServerConnection = new ServerConnection(this);
    m_connectionOk = false; //connection not established
    m_aliasConnected = false; //alias not connected
    m_readyToDestroy = false;
    m_receiver = NULL;
    m_sender = NULL;
}

DaemonThread::~DaemonThread()
{
    if (m_connectionOk)
        stopThread();
    // delete ServerConnection object
    m_ServerConnection->deleteLater();
}

DaemonThread::DaemonThread(
        boost::shared_ptr<DaemonConfiguration::Config> config)
        : m_config(config), m_receiver(NULL), m_sender(NULL)
{
    m_ServerConnection = new ServerConnection(this);
    m_connectionOk = false;
    m_aliasConnected = false;
    m_ServerConnection->connectToServer(QHostAddress(m_config->m_ip), m_config->m_port);

    // connecting to the Alias is in the onConnected() method
    m_readyToDestroy = false;
}

void DaemonThread::onAliasConnected()
{
    qDebug() << "Alias connected successful";
    if (m_aliasConnected) {
        qDebug()
                << "Error. Double AliasConnected information from server to DeamonThread";
    } else
        m_aliasConnected = true;

    // tells DaemonApplication that connection with server is established
    DaemonApplication::getInstance().onStarted(this);
}

void DaemonThread::onAliasConnectionError()
{
    qDebug() << "Unsuccessful connection to the Alias: " << m_config->m_aliasId;

    m_aliasConnected = false;
    m_readyToDestroy = true;

    // it cause this->stopThread() and deleting this thread from DaemonApplication
    DaemonApplication::getInstance().onStartingError(this);

}

void DaemonThread::onConnected()
{
    if (m_connectionOk) {
        qDebug() << "Incoming connection from server after previous connection was established. Double onConnected().";
        m_connectionOk = false;
    } else
    {
        qDebug() << "Connection to server successful. Starting connection to alias... ";
        m_connectionOk = true;  // if everything OK
        // connecting to the Alias
        m_ServerConnection->connectToAlias(m_config->m_aliasId, Utilities::Password(m_config->m_password)); // TODO check if it is not a Hash
    }

}

void DaemonThread::onDisconnected()
{
    qDebug() << m_config->m_cataloguePath << "gets disconnect from server.";
    //m_connectionOk = false;   // this sets onClose(this) // TODO think jeszcze raz
    //m_aliasConnected = false;
    // this object may be delete by DaemonApplication so:
    m_readyToDestroy = true;

    // cause normal procedure for ending DaemonThread include stopThread() and delete this
    DaemonApplication::getInstance().onClosed(this);

}

void DaemonThread::onFileNotRemoved()
{
    // TODO onFileNotRemoved
}

void DaemonThread::onFindFile(const QString &fileName)
{
    QDirIterator it(m_config->m_cataloguePath, QDirIterator::Subdirectories);
    QList<QString> foundPaths;
    QRegExp regex(QRegExp::escape(fileName));

    while (it.hasNext()) {
        QFileInfo info = it.fileInfo();

        if (info.isFile())
            if (regex.indexIn(info.fileName()) != -1)
                foundPaths.append(info.filePath());

        it.next();
    }

    // Create list of alias files
    Utilities::AliasFileList files;

    // Add found files to list with their data
    foreach (QString str, foundPaths){
    QString date = QString::number(QFileInfo(str).lastModified().toMSecsSinceEpoch());
    quint32 size = QFileInfo(str).size();

    files.addFile(cutAbsolutePath(str), date, size);
}

// TODO kopasiak check / change communicates / method / param
//    if (files.getSize())
//        m_ServerConnection->sendFileFound(files);
//    else
//        m_ServerConnection->sendNoSuchFile();
}

void DaemonThread::onListFiles()
{
    QDir dir(m_config->m_cataloguePath);
    if (!dir.exists())
        throw std::runtime_error("Path doesn't exit.");

    // Recursive read alias catalogue
    QXmlNamePool namePool;
    Utilities::FileTree fileTree(namePool);
    QXmlNodeModelIndex fileNode = fileTree.nodeFor(m_config->m_cataloguePath);

    QXmlQuery query(namePool);
    query.bindVariable("fileTree", fileNode);
    query.setQuery("$fileTree");

    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);

    QXmlFormatter formatter(query, &buffer);
    query.evaluateTo(&formatter);

    // Remove absolute path, make it relative
    QString str(output.data());
    str = cutAbsolutePath(str);

    // Create AliasFileList object to send it after
    Utilities::AliasFileList atree;

    QXmlStreamReader reader(str);
    reader.readNext();

    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "directory") {
            } else if (reader.name() == "file") {
                QXmlStreamAttributes attribs = reader.attributes();
                atree.addFile(attribs.value("filePath").toString(),
                        attribs.value("lastModified").toString(),
                        attribs.value("size").toString().toUInt());
            }
        }

        reader.readNext();
    }

    m_ServerConnection->sendFileList(atree);
}

void DaemonThread::onReciveFile(const QString& fileName,
        const QHostAddress& address, quint16 port, qint64 size)
{
    //TODO onReceive
    QString filePath(m_config->m_cataloguePath);
    filePath += "/";
    filePath += fileName;
    qDebug() << "Somebody wants to Receive file in: " << filePath;

    QFile recFile(filePath);

    // if file exists then I change it name for having backup in case
    if (recFile.exists()) {
        QString orig(filePath);
        orig += ".orig";
        recFile.rename(orig);
    }

    qDebug() << "receiver ma taki plik " << size;
    m_receiver = new FileReciver(this, new QFile(filePath), size);
    qDebug() << m_receiver->connectToServer(address, port);

}

void DaemonThread::onRemoveFile(const QString& fileName)
{
    // TODO uncomment that when communication method 'll be exist
//    QFile file(QString(m_config->m_cataloguePath) + QString(fileLocation->path));
//    if (file.exists())
//        if (file.remove()) {
//            onFileRemoved();
//            return;
//        }
//
//    onFileNotRemoved();
}

void DaemonThread::onSendFile(const QString& fileName,
        const QHostAddress& address, quint16 port)
{
    // TODO onSendFile - permission zrobic i co jesli plik nie istnieje?
    QString filePath(m_config->m_cataloguePath);
    filePath += "/";
    filePath += fileName;
    qDebug() << "I send somebody file: " << filePath;

    QFile sendFile(filePath);

    //sendFile.open(QIODevice::ReadOnly); // don't do this
    if (sendFile.exists() )
           //&& (sendFile.permissions()==QFile::ReadOwner || sendFile.permissions()==QFile::ReadUser))
    {
       m_sender = new FileSender(this, new QFile(filePath));
       m_sender->connectToServer(address, port);
    }
    else
        qDebug() << "Request file has no read rights or it isn't exist. " << filePath;
}

// this method comes from FileTransferListener class
void DaemonThread::onTransferEnd(FileSender * sender)
{
    //TODO onTransferEnd - never disconnect
    qDebug() << "File sending completed: ";

    delete m_sender;

}
// this method comes from FileTransferListener class
void DaemonThread::onTransferError(FileSender *sender)
{
    // TODO onTransferError
    qDebug() << "File sending with error. ";
    delete m_sender;
}

void DaemonThread::onTransferEnd(FileReciver * reciver)
{
    // TODO onTransferEnd Reciver
    qDebug() << "File receiving completed. ";
    delete m_receiver;
}

void DaemonThread::onTransferError(FileReciver * receiver)
{
    // TODO onTransferError
    qDebug() << "File receiving with error. ";
    delete m_receiver;
}

void DaemonThread::stopThread()
{
    if (m_connectionOk) {
        // nevertheless disconnectFromServer() succeed or not, I select connection false
        m_connectionOk = false;
        m_aliasConnected = false;
        m_readyToDestroy = true;
        m_ServerConnection->disconnectFromServer();
    }
}

boost::shared_ptr<DaemonConfiguration::Config> DaemonThread::getConfig()
{
    return m_config;
}

QString& DaemonThread::cutAbsolutePath(QString &str)
{
    return str.replace(QRegExp(QString() + m_config->m_cataloguePath + "/?"),
            "/");
}

bool DaemonThread::isReadyToDestroy()
{
    // Rather don't use this method.
    return m_readyToDestroy;
}

} //namespace Daemon
} //namespace TIN_project
