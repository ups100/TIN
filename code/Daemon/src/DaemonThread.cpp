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
#include "Identifier.h"
#include "ConfigFileName.h"

#include <QDir>
#include <QFile>
#include <QBuffer>
#include <QIODevice>
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
        : m_config(config), m_suffix("orig")
{
    m_ServerConnection = new ServerConnection(this);
    m_connectionOk = false;
    m_aliasConnected = false;
    m_ServerConnection->connectToServer(QHostAddress(m_config->m_ip), m_config->m_port);

    // connecting to the Alias is in the onConnected() method
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
    // Create temporary file
    QFile file(
            m_config->m_cataloguePath + QDir::separator()
                    + Utilities::ConfigFileName::CONFIG_FILE_NAME);
    file.open(QIODevice::WriteOnly);
    file.close();
}

void DaemonThread::onAliasConnectionError()
{
    qDebug() << "Unsuccessful connection to the Alias: " << m_config->m_aliasId;

    m_aliasConnected = false;

    // it cause this->stopThread() and deleting this thread from DaemonApplication
    DaemonApplication::getInstance().onStartingError(this);

    // Delete temporary file if exists
    QFile file(
            m_config->m_cataloguePath + QDir::separator()
                    + Utilities::ConfigFileName::CONFIG_FILE_NAME);
    if (file.exists())
        file.remove();

}

void DaemonThread::onConnected()
{
    if (m_connectionOk) {
        qDebug()
                << "Incoming connection from server after previous connection was established. Double onConnected().";
        m_connectionOk = false;
    } else {
        qDebug()
                << "Connection to server successful. Starting connection to alias... ";
        m_connectionOk = true;  // if everything OK
        // connecting to the Alias

        //todo ADD id to connection!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        qDebug() << "Dodac id do connectToAlias";
        m_ServerConnection->connectToAlias(m_config->m_aliasId,
                Utilities::Password(m_config->m_password),
                Utilities::Identifier());
    }

}

void DaemonThread::onDisconnected()
{
    qDebug() << m_config->m_cataloguePath << "gets disconnect from server.";
    //m_connectionOk = false;   // this sets onClose(this) // TODO think jeszcze raz
    //m_aliasConnected = false;

    // cause normal procedure for ending DaemonThread include stopThread() and delete this
    DaemonApplication::getInstance().onClosed(this);

    // Delete temporary file if exists
    QFile file(
            m_config->m_cataloguePath + QDir::separator()
                    + Utilities::ConfigFileName::CONFIG_FILE_NAME);
    if (file.exists())
        file.remove();

}

void DaemonThread::onFileNotRemoved()
{
    // TODO onFileNotRemoved
}

void DaemonThread::onFindFile(const QString &fileName)
{
    qDebug() << "in DaemonThread catalog "<< m_config->m_cataloguePath<<" onFindFIle: search " << fileName;

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
        qDebug() << "DaemonThread find " << str;
    QString date = QString::number(QFileInfo(str).lastModified().toMSecsSinceEpoch());
    quint32 size = QFileInfo(str).size();

    files.addFile(cutAbsolutePath(str), date, size);
}
    qDebug() <<"In" << m_config->m_cataloguePath <<"Finding complete. How many: " << files.getSize();

 //TODO kopasiak check / change communicates / method / param
    if (files.getSize()) {
        qDebug() << "DT "<< m_config->m_cataloguePath <<" sending client that FileFound";
        m_ServerConnection->sendFileFound(files);   // it sometimes do "naruszenie ochrony pamięci"
    }
    else {
        qDebug() << "DT"<< m_config->m_cataloguePath<<" sending client that File NOT Found";
        m_ServerConnection->sendNoSuchFile();
    }
}

void DaemonThread::onListFiles()
{
    qDebug() << "DaemonThread start onList File";
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
        orig += m_suffix; //adding constant suffix to form backup file name
        recFile.rename(orig);
    }

    qDebug() << "receiver ma taki plik " << size;
    m_receiver.append(new FileReciver(this, new QFile(filePath), size)) ;
    m_receiver.last()->connectToServer(address, port);

}

void DaemonThread::onRemoveFile(const QString& fileName)
{
    // TODO uncomment that when communication method 'll be exist
    QFile file(QString(m_config->m_cataloguePath) + QString(fileName));//fileLocation->path));

    if (file.exists())
        if (file.remove()) {
            //onFileRemoved();
            qDebug() << "DaemonThread delete file: " << fileName;
            return;
        } else
            qDebug() << "Attention. DaemonThread CAN NOT delete file: " << fileName
                     << " I suppose file rights are incorrect. ";

    //onFileNotRemoved();
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

    //TODO consider taki przypadek
    // blad wysylania (send) i poprawny odbior (receiver)
    if (sendFile.exists() && sendFile.size() == 0) {
        qDebug() << "Somebody wants to send empty file... " << filePath;
    }

    //sendFile.open(QIODevice::ReadOnly); // don't do this
    if (sendFile.exists() )
           //&& (sendFile.permissions()==QFile::ReadOwner || sendFile.permissions()==QFile::ReadUser))
    {
       m_sender.append(new FileSender(this, new QFile(filePath)));
       m_sender.last()->connectToServer(address, port);
    }
    else
        qDebug() << "Request file has no read rights or it isn't exist. " << filePath;
}

// this method comes from FileTransferListener class
void DaemonThread::onTransferEnd(FileSender * sender)
{
    //TODO onTransferEnd - never disconnect
    qDebug() << "File sending completed: ";

    int i=0;
    for (i=0; i<m_sender.size(); ++i) {
        if (m_sender[i] == sender) {
            break;
        }
    }
    if (i != m_sender.size() ) {
        QTimer::singleShot(0, m_sender[i], SLOT(deleteLater()));
        m_sender.removeAt(i);
    }
}
// this method comes from FileTransferListener class
void DaemonThread::onTransferError(FileSender *sender)
{
    // TODO onTransferError - jak powiadomić Klienta o tym? Serwer to zrobi.
    qDebug() << "File sending with error. ";

    int i=0;
    for (i=0; i<m_sender.size(); ++i) {
        if (m_sender[i] == sender) {
            break;
        }
    }
    if (i != m_sender.size() ) {
        QTimer::singleShot(0, m_sender[i], SLOT(deleteLater()));
        m_sender.removeAt(i);
    }
}

void DaemonThread::onTransferEnd(FileReciver * reciver)
{
    // TODO onTransferEnd Reciver - powiadamiać klienta?
    QString filePath(reciver->getFileName());
    qDebug() << "File receiving completed. " << reciver->getFileName();
    QString fileBackup(filePath);
    //adding suffics to get backupfile
    fileBackup += m_suffix;
    QFile file(fileBackup);
    if (file.exists())
        file.remove();  // on successful transmission backup is removing

    // deleting FileReciver object
    int i=0;
    for (i=0; i<m_receiver.size(); ++i) {
        if (m_receiver[i] == reciver) {
            break;
        }
    }
    if (i != m_receiver.size() ) {
        QTimer::singleShot(0, m_receiver[i], SLOT(deleteLater()));
        m_receiver.removeAt(i);
    }
}

void DaemonThread::onTransferError(FileReciver * receiver)
{
    QString filePath(receiver->getFileName());
    qDebug() << "File receiving with error. " << filePath;
    QString fileBackup(filePath);
    //adding suffics to get backupfile
    fileBackup += m_suffix;
    QFile file(fileBackup);
    if (file.exists())
        file.rename(filePath);  // on unsuccessful transmission backup is recovered

    // deleting FileReciver object
     int i=0;
     for (i=0; i<m_receiver.size(); ++i) {
         if (m_receiver[i] == receiver) {
             break;
         }
     }
     if (i != m_receiver.size() ) {
         QTimer::singleShot(0, m_receiver[i], SLOT(deleteLater()));
         m_receiver.removeAt(i);
     }
}

void DaemonThread::stopThread()
{
    if (m_connectionOk) {
        // nevertheless disconnectFromServer() succeed or not, I select connection false
        m_connectionOk = false;
        m_aliasConnected = false;
//        m_readyToDestroy = true;
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

} //namespace Daemon
} //namespace TIN_project
