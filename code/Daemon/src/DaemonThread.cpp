///////////////////////////////////////////////////////////
//  DaemonThread.cpp
//  Implementation of the Class DaemonThread
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonThread.h"
#include "FileTree.h"
#include "DaemonApplication.h"
#include <QFile>
#include <QBuffer>
#include <QXmlFormatter>
#include <QRegExp>

namespace TIN_project {
namespace Daemon {

DaemonThread::DaemonThread()
{

}

DaemonThread::~DaemonThread()
{

}

DaemonThread::DaemonThread(const QHostAddress& ip, quint16 port,
        const QString& path)
        : m_path(path)
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

void DaemonThread::onFindFile(boost::shared_ptr<QString> fileName)
{

}

void DaemonThread::onListFiles()
{
    // Recursive read alias catalogue
    QXmlNamePool m_namePool;
    Utilities::FileTree m_fileTree(m_namePool);
    QXmlNodeModelIndex m_fileNode = m_fileTree.nodeFor(m_path);

    QXmlQuery query(m_namePool);
    query.bindVariable("fileTree", m_fileNode);
    query.setQuery("$fileTree");

    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);

    QXmlFormatter formatter(query, &buffer);
    query.evaluateTo(&formatter);

    // Remove absolute path, make it relative
    QString str(output.data());
    str.replace(QRegExp(QString() + m_path + "/?"), "/");

    qDebug()<<str;
    // TODO send QString with 'xmlified' data
}

void DaemonThread::onReciveFile(boost::shared_ptr<File> file)
{

}

void DaemonThread::onRemoveFile(boost::shared_ptr<QString> fileName)
{

}

void DaemonThread::onSendFile(boost::shared_ptr<File> file)
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
    // TODO ?!
    while (1) {
        sleep(1);
    }
}

} //namespace Daemon
} //namespace TIN_project
