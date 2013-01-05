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

}

DaemonThread::~DaemonThread()
{
    delete m_ServerConnection;
}

DaemonThread::DaemonThread(
        boost::shared_ptr<DaemonConfiguration::Config> config,
        DaemonApplication *daemonApplication)
        : m_config(config), m_ServerConnection(new ServerConnection(this)),
                m_daemonApplication(daemonApplication)
{

}

void DaemonThread::onAliasConnected()
{
    qDebug() << "ALIAS CONNECTED";
}

void DaemonThread::onAliasConnectionError()
{
    qDebug() << "ALIAS CONNECTION ERROR";
    socketErrorHandler();
}

void DaemonThread::onConnected()
{
    qDebug() << "CONNECTING";
    m_ServerConnection->connectToAlias(m_config->m_aliasId,
            Utilities::Password(m_config->m_password));
}

void DaemonThread::onDisconnected()
{
    qDebug() << "DISCONNECTED";
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

void DaemonThread::onTransferEnd(FileSender * sender)
{

}

void DaemonThread::onTransferEnd(FileReciver * reciver)
{

}

void DaemonThread::socketErrorHandler()
{
    qDebug() << "Socket connection Error occurred, stop me now.";
    m_daemonApplication->stopDaemonThread(this);
}

void DaemonThread::stopThread()
{

}

void DaemonThread::start()
{
    m_ServerConnection->connectToServer(QHostAddress(m_config->m_ip),
            m_config->m_port);
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
