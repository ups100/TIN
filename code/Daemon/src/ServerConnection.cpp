/**
 * @file Daemon/src/ServerConnection.cpp
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Daemon::ServerConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "ServerConnection.h"
#include "ServerConnectionListener.h"
#include "FileLocation.h"
#include "AliasFileList.h"
#include "CommunicationProtocol.h"
#include "File.h"

namespace TIN_project {

using Utilities::CommunicationProtocol;

namespace Daemon {

ServerConnection::ServerConnection(
        ServerConnectionListener *serverConnectionListener)
        : m_socket(0L), m_creatorThread(0L),
                m_serverListener(serverConnectionListener),
                m_isReadyState(false), m_isConnecting(false),
                m_isClosing(false), m_currentMessageId(CHAR_MAX),
                m_sizeOk(false), m_messageSize(-1)
{
    connect(&m_additionalThread, SIGNAL(started()), this,
            SLOT(threadStartedSlot()));
    connect(&m_additionalThread, SIGNAL(finished()), this,
            SLOT(threadFinishedSlot()));
    connect(this, SIGNAL(sendData(QByteArray*)), this,
            SLOT(sendSlot(QByteArray*)));
}

ServerConnection::~ServerConnection()
{
    m_mutex.lock();
    if (m_isReadyState) {
        m_mutex.unlock();
        qDebug() << "Destroying ServerConnection object with open connection";

        QTimer::singleShot(0, this, SLOT(disconnectSlot()));
    }

    m_mutex.unlock();

}

void ServerConnection::connectToServer(const QHostAddress& address,
        quint16 port)
{
    QMutexLocker locker(&m_mutex);
    if (m_isReadyState || m_isConnecting || m_isClosing) {
        qDebug() << "Already connected or closing";
        return;
    }

    m_isConnecting = true;
    m_serverAddress = address;
    m_serverPort = port;

    this->moveToThread(&m_additionalThread);
    m_additionalThread.moveToThread(&m_additionalThread);
    m_additionalThread.start();
}

void ServerConnection::disconnectFromServer()
{
    QMutexLocker locker(&m_mutex);

    if (m_isConnecting || m_isClosing) {
        return;
    }

    m_isReadyState = false;
    m_isClosing = true;
    QTimer::singleShot(0, this, SLOT(disconnectSlot()));
}

void ServerConnection::connectToAlias(const QString& aliasName,
        const Utilities::Password& password)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::ADD_DIRECTORY> message(
                aliasName, password);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ServerConnection::sendNoSuchFile()
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::NO_SUCH_FILE_HERE> message;
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ServerConnection::sendFileFound(const Utilities::FileLocation& location)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::FILE_FOUND_HERE> message(location);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ServerConnection::sendFileList(const Utilities::AliasFileList& list)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::FILES_LIST> message(
                list);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ServerConnection::disconnectSlot()
{
    m_socket->disconnectFromHost();
}

void ServerConnection::sendSlot(QByteArray *array)
{
    quint64 size = array->size();
    quint64 send = 0;

    do {
        QByteArray arrayTmp = array->right(size - send);
        send += m_socket->write(arrayTmp);
    } while (send < size);

    delete array;
}

void ServerConnection::threadStartedSlot()
{
    //executed in context of additional thread
    m_socket = new QTcpSocket();

    connect(m_socket, SIGNAL(connected()), this, SLOT(socketConnectedSlot()));
    connect(m_socket, SIGNAL(disconnected()), this,
            SLOT(socketDisconnectedSlot()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(socketErrorSlot(QAbstractSocket::SocketError)));

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketReadyReadSlot()));

    m_socket->connectToHost(m_serverAddress, m_serverPort);
}

void ServerConnection::threadFinishedSlot()
{
    m_mutex.lock();
    m_isReadyState = false;
    m_isConnecting = false;
    m_isClosing = false;
    m_mutex.unlock();

    delete m_socket;
    m_socket = 0L;

    if (m_serverListener != 0L) {
        m_serverListener->onDisconnected();
    }
}

void ServerConnection::socketConnectedSlot()
{
    m_mutex.lock();
    m_isReadyState = true;
    m_isConnecting = false;
    m_mutex.unlock();

    if (m_serverListener != 0L) {
        m_serverListener->onConnected();
    }
}

void ServerConnection::socketErrorSlot(QAbstractSocket::SocketError socketError)
{
    if (socketError != QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Socket error " << socketError;
    }

    m_serverListener->socketErrorHandler();
}

void ServerConnection::socketDisconnectedSlot()
{
    m_mutex.lock();
    m_isReadyState = false;
    m_isClosing = true;
    m_mutex.unlock();

    this->moveToThread(m_creatorThread);
    m_additionalThread.moveToThread(m_creatorThread);

    m_additionalThread.quit();
}

void ServerConnection::socketReadyReadSlot()
{
    do {
        if (m_currentMessageId == CHAR_MAX) {
            m_socket->read(&m_currentMessageId, 1);
        }

        QByteArray size;
        QByteArray data;
        qint64 currentLeftSize = 0;

        switch (CommunicationProtocol::getType(m_currentMessageId)) {
            case CommunicationProtocol::CONNECTED_TO_ALIAS:
                m_currentMessageId = CHAR_MAX;
                if (m_serverListener != 0L) {
                    m_serverListener->onAliasConnected();
                }
                break;

            case CommunicationProtocol::NOT_CONNECTED_TO_ALIAS:
                m_currentMessageId = CHAR_MAX;
                if (m_serverListener != 0L) {
                    m_serverListener->onAliasConnectionError();
                }
                break;

            case CommunicationProtocol::RECIVE_FILE: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                }
                m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                        size);

                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::RECIVE_FILE> message(data);

                if (m_serverListener != 0L) {
                    m_serverListener->onReciveFile(message.getName(),
                            message.getAddress(), message.getPort());
                }
            }
                break;

            case CommunicationProtocol::SEND_FILE: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                }
                m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                        size);

                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::SEND_FILE> message(data);

                if (m_serverListener != 0L) {
                    m_serverListener->onSendFile(message.getName(),
                            message.getAddress(), message.getPort());
                }
            }
                break;

            case CommunicationProtocol::LIST_YOUR_FILES:
                m_currentMessageId = CHAR_MAX;
                if (m_serverListener != 0L) {
                    m_serverListener->onListFiles();
                }
                break;

            case CommunicationProtocol::DELETE_YOUR_FILE: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                }
                m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                        size);

                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::DELETE_YOUR_FILE> message(data);

                if (m_serverListener != 0L) {
                    m_serverListener->onRemoveFile(message.getName());
                }
            }
                break;

            case CommunicationProtocol::FIND_YOUR_FILE: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                }
                m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                        size);

                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::FIND_YOUR_FILE> message(data);

                if (m_serverListener != 0L) {
                    m_serverListener->onFindFile(message.getName());
                }
            }
                break;

            default:
                qDebug() << "Unknown code received " << m_currentMessageId;
                break;
        }
    } while (m_socket->bytesAvailable() != 0);
}

} //namespace Daemon
} //namespace TIN_project
