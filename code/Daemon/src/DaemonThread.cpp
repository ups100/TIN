///////////////////////////////////////////////////////////
//  DaemonThread.cpp
//  Implementation of the Class DaemonThread
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonThread.h"

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

void DaemonThread::onFindFile(const QString& fileName)
{

}

void DaemonThread::onListFiles()
{

}

void DaemonThread::onReciveFile(const QString& fileName, const QHostAddress& address, quint16 port)
{

}

void DaemonThread::onRemoveFile(const QString& fileName)
{

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

}

} //namespace Daemon
} //namespace TIN_project
