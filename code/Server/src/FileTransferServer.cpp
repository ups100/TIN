/**
 * @file FileTransferServer.cpp
 *
 * @date 05-01-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::FileTransferServer
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "FileTransferServer.h"
#include <QMutexLocker>
#include <QEventLoop>
#include <QTimer>

namespace TIN_project {
namespace Server {

FileTransferServer::FileTransferServer(FileTransferListener *listener,
        int numberOfConnections, quint64 fileSize)
        : m_FileTransferListener(listener),
                m_numberOfConenctions(numberOfConnections), m_port(0),
                m_fileSize(fileSize), m_currentSize(0), m_server(0L),
                m_serverStarted(false), m_transferInProgres(false),
                m_transferCompleted(false), m_errorOccurred(false),
                m_isClosing(false)
{
    m_creatorThread = QThread::currentThread();

    connect(&m_additionalThread, SIGNAL(started()), this,
            SLOT(startServerSlot()));
}

FileTransferServer::~FileTransferServer()
{
    if (m_serverStarted) {
        qDebug() << "Destroying file transfer server with active connections";
        disconnectFromAliasSynch();
    }
}

bool FileTransferServer::startFileServer(const QHostAddress& address =
        QHostAddress::Any, quint16 port = 0)
{
    QMutexLocker locker(&(this->m_mutex));

    if (m_serverStarted) {
        return false;
    }

    this->m_address = address;
    this->m_port = port;

    moveToThread(&m_additionalThread);
    m_additionalThread.moveToThread(&m_additionalThread);

    QEventLoop loop;

    connect(this, SIGNAL(serverStartedSignal()), &loop, SLOT(quit()));
    connect(&m_additionalThread, SIGNAL(finished()), &loop, SLOT(quit()));

    m_additionalThread.start();

    loop.exec();

    return m_serverStarted;
}

void FileTransferServer::disconnectFromAliasSynch()
{
    QMutexLocker locker(&(this->m_mutex));

    if (!m_serverStarted) {
        return;
    }

    m_isClosing = true;

    QEventLoop loop;

    connect(&m_additionalThread, SIGNAL(finished()), &loop, SLOT(quit()));

    QTimer::singleShot(0, this, SLOT("stopAllSlot()"));
    loop.exec();

    return;
}

void FileTransferServer::startServerSlot()
{
    if (m_server != 0L) {
        delete m_server;
    }

    m_server = new QTcpServer();

    bool isListening = m_server->listen(m_address, m_port);

    if (isListening) {
        m_address = m_server->serverAddress();
        m_port = m_server->serverPort();

        connect(m_server, SIGNAL(newConnection()), this,
                SLOT(addNewConnectionSlot()));

        m_serverStarted = true;

        emit serverStartedSignal();

    } else {
        m_serverStarted = false;

        moveToThread(m_creatorThread);
        m_additionalThread.moveToThread(m_creatorThread);

        m_additionalThread.quit();
    }
}

void FileTransferServer::addNewConnectionSlot()
{
    QTcpSocket *newSocket = m_server->nextPendingConnection();

    m_clients.append(newSocket);

    if (m_clients.size() == m_numberOfConenctions) {
        //we are ready to start data transfer
        m_server->close();
        disconnect(m_server, SIGNAL(newConnection()), this,
                SLOT(addNewConnectionSlot()));

        foreach(QTcpSocket *client, m_clients) {

            connect(client, SIGNAL(readyRead()), this, SLOT(readDataSlot()));

            connect(client, SIGNAL(disconnected()), this,
                    SLOT(socketDisconnectedSlot()));

            connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this,
                    SLOT(socketErrorSlot(QAbstractSocket::SocketError)));
        }

        if (this->m_FileTransferListener != 0L) {
            m_FileTransferListener->onFileTransferStarted(this);
        }
    }
}

void FileTransferServer::readDataSlot()
{
    QTcpSocket *sender = (QTcpSocket*) QObject::sender();

    //just one client is allowed to send data so we wont like to get data from others
    if (m_currentSize == 0) {
        foreach(QTcpSocket *client, m_clients) {
            if (client != sender) {
                disconnect(client, SIGNAL(readyRead()), this,
                        SLOT(readDataSlot()));
            }
        }
    }

    if ((!m_transferCompleted) && (!m_transferInProgres)) {
        m_transferInProgres = true;
    }

    while (sender->bytesAvailable() != 0) {
        QByteArray buffer = sender->readAll();

        //send to all who are not sender
        foreach(QTcpSocket* rec, m_clients) {
            if (rec != sender) {
                quint64 size = buffer.size();
                quint64 send = 0;

                do {
                    QByteArray arrayTmp = buffer.right(size - send);
                    send += rec->write(arrayTmp);
                } while (send < size);
            }
        } //foreach

        m_currentSize += buffer.size();

        if (m_currentSize >= m_fileSize) {

            if (sender->bytesAvailable() != 0) {
                qDebug() << "Too much data sent in file transfer";
            }

            m_transferCompleted = true;
            //we don't need any more data
            disconnect(sender, SIGNAL(readyRead()), this, SLOT(readDataSlot()));
            break;
        }
    } //while
}

void FileTransferServer::socketDisconnectedSlot()
{
    QTcpSocket *sender = (QTcpSocket*) QObject::sender();

    m_clients.removeOne(sender);
    sender->deleteLater();

    if (m_transferCompleted) {
        if (m_clients.size() == 0) {
            if (this->m_FileTransferListener != 0L) {
                m_FileTransferListener->onFileTransferCompleted(this);
            }
        }
    } else if (!m_isClosing) {
        if (!m_errorOccurred) {
            m_errorOccurred = true;

            foreach(QTcpSocket* rec, m_clients) {
                rec->disconnectFromHost();
            }
        } else {
            if (m_clients.size() == 0) {
                if (this->m_FileTransferListener != 0L) {
                    m_FileTransferListener->onFileTransferError(this);
                }
            }
        }
    } else {
        //if m_isClosing
        if (m_clients.size() == 0) {
            QTimer::singleShot(0, this, SLOT(stopAllSlot()));
        }
    }
}

void FileTransferServer::socketErrorSlot(QAbstractSocket::SocketError error)
{
    if (error != QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Socket error " << error;
    }
}

void FileTransferServer::stopAllSlot()
{
    if (m_clients.size() != 0) {

        foreach(QTcpSocket* rec, m_clients) {
            rec->disconnectFromHost();
        }
    } else {
        //stop server
        m_server->close();
        m_server->deleteLater();
        m_server = 0L;

        //change state
        m_serverStarted = false;
        m_transferInProgres = false;
        m_transferCompleted = false;
        m_errorOccurred = false;
        m_isClosing = false;

        //go back to thread
        moveToThread(m_creatorThread);
        m_additionalThread.moveToThread(m_creatorThread);

        QTimer::singleShot(0, &m_additionalThread, SLOT(quit()));
    }
}

} //namespace server
} //namespace TIN_project
