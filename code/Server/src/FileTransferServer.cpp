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
                m_state(OFFLINE)
{
    m_creatorThread = QThread::currentThread();

    connect(&m_additionalThread, SIGNAL(started()), this,
            SLOT(startServerSlot()));
}

FileTransferServer::~FileTransferServer()
{
    if (m_state != OFFLINE) {
        qDebug() << "Destroying file transfer server with active connections";
        disconnectFromAliasSynch();
    }
}

bool FileTransferServer::startFileServer(const QHostAddress& address =
        QHostAddress::Any, quint16 port = 0)
{
    QMutexLocker locker(&(this->m_mutex));

    if (m_state != OFFLINE) {
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

    if (m_state == ERROR) {
        return false;
    } else {
        return true;
    }
}

void FileTransferServer::disconnectFromAliasSynch()
{
    QMutexLocker locker(&(this->m_mutex));

    if (!(m_state == OFFLINE)) {
        return;
    }

    QEventLoop loop;

    disconnect(&m_additionalThread, SIGNAL(finished()), this,
            SLOT(threadFinishedSlot()));
    connect(&m_additionalThread, SIGNAL(finished()), &loop, SLOT(quit()));

    QTimer::singleShot(0, this, SLOT(stopAllSlot()));
    loop.exec();
    delete m_server;
    m_server = 0L;
    m_state = OFFLINE;

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

        connect(&m_additionalThread, SIGNAL(finished()), this,
                SLOT(threadFinishedSlot()));

        m_state = WAITING_FOR_CLIENTS;

        emit serverStartedSignal();

    } else {
        m_state = ERROR;

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

        m_state = ALL_CLIENTS_CONNECTED;

        foreach(QTcpSocket *client, m_clients) {

            connect(client, SIGNAL(readyRead()), this, SLOT(readDataSlot()));

            connect(client, SIGNAL(disconnected()), this,
                    SLOT(socketDisconnectedSlot()));

            connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this,
                    SLOT(socketErrorSlot(QAbstractSocket::SocketError)));
        }

        QTimer::singleShot(0, this, SLOT(sendPeak()));
    }
}

void FileTransferServer::readDataSlot()
{
    if (m_state == ALL_CLIENTS_CONNECTED) {
        m_state = TRANSFER_IN_PROGRESS;
    } else if (m_state != TRANSFER_IN_PROGRESS) {
        qDebug() << "Too much date received";
        m_state = ERROR;
        QTimer::singleShot(0, this, SLOT(stopAllSlot()));
    }

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
                m_state = ERROR;
                QTimer::singleShot(0, this, SLOT(stopAllSlot()));
            }

            m_state = TRANSFER_COMPLETED;
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

    switch (m_state) {
        case TRANSFER_COMPLETED:
            if (m_clients.size() == 0) {
                m_state = ALL_DISCONNECTED;
            }
            //break should not be here
        case CLOSING:
        case ERROR_DISCONNECTING:
            if (m_clients.size() == 0) {
                moveToThread(m_creatorThread);
                m_additionalThread.moveToThread(m_creatorThread);

                m_additionalThread.quit();
            }
            break;
        case ERROR:
        case WAITING_FOR_CLIENTS:
        case ALL_CLIENTS_CONNECTED:
        case TRANSFER_IN_PROGRESS:
            if (m_clients.size() == 0) {
                moveToThread(m_creatorThread);
                m_additionalThread.moveToThread(m_creatorThread);

                m_additionalThread.quit();
                return;
            } else {
                foreach(QTcpSocket* rec, m_clients) {
                    rec->disconnectFromHost();
                }
            }
            break;
    }
}

void FileTransferServer::socketErrorSlot(QAbstractSocket::SocketError error)
{
    if (error != QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Socket error " << error
                << " occurred during file transfer";
    }
}

void FileTransferServer::stopAllSlot()
{
    if (m_state != ERROR) {
        m_state = CLOSING;
    }

    if (m_clients.size() != 0) {

        foreach(QTcpSocket* rec, m_clients) {
            rec->disconnectFromHost();
        }
    } else {
        moveToThread(m_creatorThread);
        m_additionalThread.moveToThread(m_creatorThread);

        m_additionalThread.quit();
    }

}
void FileTransferServer::threadFinishedSlot()
{
    switch (m_state) {
        case ALL_DISCONNECTED:
            if (m_FileTransferListener != 0L) {
                m_FileTransferListener->onFileTransferCompleted(this);
            }
            break;
        case ERROR_DISCONNECTING:
        case ERROR:
            if (m_FileTransferListener != 0L) {
                m_FileTransferListener->onFileTransferError(this);
            }
            break;
        case CLOSING:
            qDebug()<<"Forcing file transfer close";
            break;
        default:
            qDebug()<<"We should not be here in state "<<m_state;
            break;
    }
}

void FileTransferServer::sendPeak()
{
    QByteArray data;
    data.append('S');
    foreach(QTcpSocket* client, m_clients) {
        client->write(data);
    } //foreach
}

} //namespace server
} //namespace TIN_project
