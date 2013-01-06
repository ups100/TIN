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
}

DaemonThread::~DaemonThread()
{
    if (m_connectionOk)
        stopThread();
    // TODO choose:
    m_ServerConnection->deleteLater();
    // or if in m_ServerConnection is no event loop choose this:
    // delete m_ServerConnection;
}

DaemonThread::DaemonThread(
        boost::shared_ptr<DaemonConfiguration::Config> config)
        : m_config(config)
{
    m_ServerConnection = new ServerConnection(this);
    m_connectionOk = false;
    m_aliasConnected = false;
    m_ServerConnection->connectToServer(QHostAddress(m_config->m_ip),
            m_config->m_port);

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
}

void DaemonThread::onAliasConnectionError()
{
    qDebug() << "Unsuccessful connection to the Alias: ";
    qDebug() << m_config->m_aliasId;

    m_aliasConnected = false;

    // TODO uzgodnic scenariusz wypadku niepołączenia się z Aliasem
    // proponuję zakończyć wtedy komunikację z serwerem, żeby potem się nie plątała niepotrzebnie
    // i można powiadomić DaemonApplication o tym zdarzeniu, żeby usunęła wątek
    // stopThread()
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
        m_ServerConnection->connectToAlias(m_config->m_aliasId,
                Utilities::Password(m_config->m_password));
    }

}

void DaemonThread::onDisconnected()
{
    qDebug() << "Disconnect from server.";
    m_connectionOk = false;
    m_aliasConnected = false;

    // TODO uzgodnic scenariusz braku polaczenia
}

void DaemonThread::onFileNotRemoved()
{

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
        const QHostAddress& address, quint16 port)
{

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

}

// this method comes from FileTransferListener class
void DaemonThread::onTransferEnd(FileSender * sender)
{

}
// this method comes from FileTransferListener class
void DaemonThread::onTransferEnd(FileReciver * reciver)
{

}

void DaemonThread::stopThread()
{
    if (m_connectionOk) {
        // nevertheless disconnectFromServer() succeed or not, I select connection false
        m_connectionOk = false;
        m_aliasConnected = false;
        m_ServerConnection->disconnectFromServer();
    }
}

//void DaemonThread::start()
//{
//    // TODO connect to server and start listening <-- // it has been done by constructor (comment by js)
////    while (1) {
//////        qDebug() << m_config->m_cataloguePath << " " << m_config->m_port << " "
//////                << m_config->m_aliasId;
////        sleep(5);
////    }
//
//    // start event loop
//    // exec();
//}

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
