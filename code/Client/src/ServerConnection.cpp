/**
 * @file Client/src/ServerConnection.cpp
 *
 * @date 08-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Client::ServerConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "ServerConnection.h"
#include "Password.h"
#include "FileLocation.h"
#include "ServerConnectionListener.h"
#include "AliasCommunicationListener.h"
#include "CommunicationProtocol.h"

#include <QEventLoop>
#include <QTimer>

namespace TIN_project {

using Utilities::CommunicationProtocol;

namespace Client {

ServerConnection::ServerConnection(ServerConnectionListener *serverListener,
        AliasCommunicationListener *aliasListener)
        : m_socket(0L), m_creatorThread(0L), m_serverListener(serverListener),
                m_aliasListener(aliasListener), m_isReadyState(false),
                m_isConnecting(false), m_isClosing(false),
                m_currentMessageId(CHAR_MAX), m_sizeOk(false), m_messageSize(-1)
{
    m_creatorThread = QThread::currentThread();

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

void ServerConnection::connectToAlias(const QString& aliasName,
        const Utilities::Password& password)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::CONNECT_TO_ALIAS> message(aliasName,
                password);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
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

void ServerConnection::createAlias(const QString& name,
        const Utilities::Password& password)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::CREATE_ALIAS> message(
                name, password);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ServerConnection::findFileInAlias(const QString& fileName)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::FIND_FILE> message(
                fileName);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ServerConnection::listAlias()
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::LIST_ALIAS> message;
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ServerConnection::pullFileFrom(const Utilities::FileLocation& file)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::PULL_FILE> message(
                file);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

/**
 * not sure about semantic of this method
 */
void ServerConnection::pushFileToAlias(const QString& path, qint64 size)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::PUSH_FILE> message(
                path, size);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

/**
 * not sure about semantic of this method 
 */
void ServerConnection::removeFileFromAlias(const QString& fileName)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::DELETE_FROM_ALIAS> message(fileName);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
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

void ServerConnection::removeAlias(const QString& name,
        const Utilities::Password& password)
{
    QMutexLocker locker(&this->m_mutex);
    if (m_isReadyState) {
        CommunicationProtocol::Communicate<CommunicationProtocol::REMOVE_ALIAS> message(
                name, password);
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
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

void ServerConnection::socketErrorSlot(QAbstractSocket::SocketError socketError)
{
    //maybe in future will be implemented in other way but now just
    if (socketError != QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Socket error " << socketError;
    }
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

void ServerConnection::disconnectSlot()
{
    m_socket->disconnectFromHost();
}

void ServerConnection::sendSlot(QByteArray* array)
{
    quint64 size = array->size();
    quint64 send = 0;
    do {
        QByteArray arrayTmp = array->right(size - send);
        send += m_socket->write(arrayTmp);
    } while (send < size);

    delete array;
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

            case CommunicationProtocol::ALIAS_CREATED:
                m_currentMessageId = CHAR_MAX;
                if (m_serverListener != 0L) {
                    m_serverListener->onAliasCreated();
                }
                break;

            case CommunicationProtocol::ALIAS_NOT_CREATED:
                m_currentMessageId = CHAR_MAX;
                if (m_serverListener != 0L) {
                    m_serverListener->onAliasCreationError();
                }
                break;

            case CommunicationProtocol::ALIAS_REMOVED:
                m_currentMessageId = CHAR_MAX;
                if (m_serverListener != 0L) {
                    m_serverListener->onAliasDeleted();
                }
                break;

            case CommunicationProtocol::ALIAS_NOT_REMOVED:
                m_currentMessageId = CHAR_MAX;
                if (m_serverListener != 0L) {
                    m_serverListener->onAliasDeletionError();
                }
                break;

            case CommunicationProtocol::ALIAS_LISTED: {
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
                        CommunicationProtocol::ALIAS_LISTED> message(data);

                if (m_aliasListener != 0L) {
                    m_aliasListener->onAliasListed(message.getList());
                }
            }
                break;

            case CommunicationProtocol::FILE_LOCATION: {
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
                        CommunicationProtocol::FILE_LOCATION> message(data);

                if (m_aliasListener != 0L) {
                    m_aliasListener->onFileFound(message.getLocation());
                }
            }
                break;

            case CommunicationProtocol::FILE_NOT_FOUND:
                m_currentMessageId = CHAR_MAX;
                if (m_aliasListener != 0L) {
                    m_aliasListener->onFileNotFound();
                }
                break;

            case CommunicationProtocol::DELETED_FROM_ALIAS:
                m_currentMessageId = CHAR_MAX;
                if (m_aliasListener != 0L) {
                    m_aliasListener->onFileRemoved();
                }
                break;

            case CommunicationProtocol::NOT_DELETED_FROM_ALIAS:
                m_currentMessageId = CHAR_MAX;
                if (m_aliasListener != 0L) {
                    m_aliasListener->onFileRemovingError();
                }
                break;

            case CommunicationProtocol::TRANSFER_ERROR:
                m_currentMessageId = CHAR_MAX;
                if (m_aliasListener != 0L) {
                    m_aliasListener->onFileTransferError();
                }
                break;

            case CommunicationProtocol::TRANSFER_FINISHED:
                m_currentMessageId = CHAR_MAX;
                if (m_aliasListener != 0L) {
                    m_aliasListener->onFileTransferFinished();
                }
                break;

            case CommunicationProtocol::TRANSFER_IN_PROGRESS:
                m_currentMessageId = CHAR_MAX;
                if (m_aliasListener != 0L) {
                    m_aliasListener->onFileTransferStarted();
                }
                break;

            default:
                qDebug() << "Unknown code received " << m_currentMessageId;
                break;
        }
    } while (m_socket->bytesAvailable() != 0);

}

} //namespace Client
} //namespace TIN_project
