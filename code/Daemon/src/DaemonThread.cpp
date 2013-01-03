///////////////////////////////////////////////////////////
//  DaemonThread.cpp
//  Implementation of the Class DaemonThread
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonThread.h"
#include "FileTree.h"
#include "DaemonApplication.h"
#include "AliasFileList.h"
#include <QDir>
#include <QFile>
#include <QBuffer>
#include <QXmlFormatter>
#include <QRegExp>
#include <QDirIterator>
#include <stdexcept>

namespace TIN_project {
namespace Daemon {

DaemonThread::DaemonThread()
{

}

DaemonThread::~DaemonThread()
{

}

DaemonThread::DaemonThread(
        boost::shared_ptr<DaemonConfiguration::Config> config)
        : m_config(config)
{

}

void DaemonThread::onAliasConnected()
{

}

void DaemonThread::onAliasConnectionError()
{

}

void DaemonThread::onConnected()
{

}

void DaemonThread::onDisconnected()
{

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

        if (info.isFile()) {
            if (regex.indexIn(info.fileName()) != -1) {
//                qDebug()<<"\t\t\tFOUND!!: "<<info.filePath();
                foundPaths.append(info.filePath());
            }
        }

        it.next();
    }

    //TODO remove
    foreach (QString str, foundPaths) {
        qDebug()<<"\t"<<str;
    }
    // TODO create fileLocations and send them
//    if (foundPaths.size())
//        onFoundFile(); // ?
//    else
//        onFileNotFound(); // ?
}

void DaemonThread::onListFiles()
{
    QDir dir(m_config->m_cataloguePath);
    if (!dir.exists()) // TODO what then ?!
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
    str.replace(QRegExp(QString() + m_config->m_cataloguePath + "/?"), "/");

//    qDebug() << str;

    qDebug()<<"ZACZYNAMY";

    Utilities::AliasFileList a;
    a.addFile("/kajo/ot/tak.g", "12.12.12r", 123);
    a.addFile("/kajo/ot/ty.g", "12.12.12r", 123);
    a.addFile("/kajo/ot/ty.g", "12.12.13r", 312);
    a.addFile("/kajo/ot/jak.g", "12.12.12r", 123);
    a.addFile("/kajo/main.cpp", "12.12.12r", 123);
    a.addFile("/level.1", "12.12.12r", 123);
    a.addFile("/singleton.h", "12.12.12r", 123);
    a.m_fileTree.str(0);

    qDebug()<<"KONCZYMY";

    // TODO send QString with 'xmlified' data
}

void DaemonThread::onReciveFile(const QString& fileName, const QHostAddress& address, quint16 port)
{

}

void DaemonThread::onRemoveFile(const QString& fileName)
{
//    QFile file(QString(m_config->m_cataloguePath) + QString(fileLocation->path));
//    if (file.exists())
//        if (file.remove()) {
//            onFileRemoved();
//            return;
//        }
//
//    onFileNotRemoved();
}

void DaemonThread::onSendFile(const QString& fileName, const QHostAddress& address, quint16 port)
{

}

void DaemonThread::onTransferEnd(FileSender * sender)
{

}

void DaemonThread::onTransferEnd(FileReciver * reciver)
{

}

void DaemonThread::stopThread()
{
    terminate();
}

void DaemonThread::run()
{
    // TODO connect to server and start listening
    while (1) {
//        qDebug() << m_config->m_cataloguePath << " " << m_config->m_port << " "
//                << m_config->m_aliasId;
        sleep(5);
    }
}

boost::shared_ptr<DaemonConfiguration::Config> DaemonThread::config()
{
    return m_config;
}

} //namespace Daemon
} //namespace TIN_project
