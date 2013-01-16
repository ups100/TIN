/**
 * @file UnknownConnection.cpp
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::UnknownConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "UnknownConnection.h"
#include "CommunicationProtocol.h"

#include <QMutexLocker>
#include <QEventLoop>
#include <QByteArray>
#include <QTimer>

namespace TIN_project {

using Utilities::CommunicationProtocol;

namespace Server {

UnknownConnection::UnknownConnection(int connectionDescriptor,
        UnknownConnectionListener *listener)
        : m_unknownConnectionListener(listener), m_isConnected(true),
                m_currentMessageId(CHAR_MAX), m_sizeOk(false), m_messageSize(-1)
{
    QMutexLocker locker(&this->m_mutex);
    m_socket = new QTcpSocket();

    if (m_socket->setSocketDescriptor(connectionDescriptor) == false) {
        m_isConnected = false;
        throw "Unable to use this socket descriptor";
    }

    connect(m_socket, SIGNAL(disconnected()), this,
            SLOT(socketDisconnectedSlot()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(socketErrorSlot(
                            QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketReadyReadSlot()));
    connect(this, SIGNAL(sendData(QByteArray*)), this,
            SLOT(sendSlot(QByteArray*)));
}

UnknownConnection::~UnknownConnection()
{
    if (m_isConnected) {
        qDebug() << "Destroying object with opened connection";

        disconnect(m_socket, SIGNAL(disconnected()), this,
                SLOT(socketDisconnectedSlot()));

        QEventLoop loop;

        connect(m_socket, SIGNAL(disconnected()), &loop, SLOT(quit()));

        QTimer::singleShot(0, this, SLOT(disconnectSlot()));
        loop.exec();
    }

    if (m_socket != 0L) {
        QTimer::singleShot(0, m_socket, SLOT(deleteLater()));
    }
}

void UnknownConnection::sendAliasCreated()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::ALIAS_CREATED> message;
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void UnknownConnection::sendAliasNotCreated()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::ALIAS_NOT_CREATED> message;
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void UnknownConnection::sendAliasNotRemoved()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::ALIAS_NOT_REMOVED> message;
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void UnknownConnection::sendAliasRemoved()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::ALIAS_REMOVED> message;
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void UnknownConnection::sendNotConnectedToAlias()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::NOT_CONNECTED_TO_ALIAS> message;
        emit sendData(new QByteArray(message.toQByteArray()));
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void UnknownConnection::disconnectClinet()
{
    if (!m_isConnected) {
        return;
    }
    m_socket->disconnectFromHost();
}

void UnknownConnection::disconnectClientSynch()
{
    QMutexLocker locker(&this->m_mutex);

    if (!m_isConnected) {
        return;
    }
    m_isConnected = false;
    disconnect(m_socket, SIGNAL(disconnected()), this,
            SLOT(socketDisconnectedSlot()));

    QEventLoop loop;

    connect(m_socket, SIGNAL(disconnected()), &loop, SLOT(quit()));

    QTimer::singleShot(0, this, SLOT(disconnectSlot()));
    loop.exec();
}

QTcpSocket* UnknownConnection::convertToOtherConnection()
{
    QMutexLocker locker(&this->m_mutex);
    if (!m_isConnected) {
        return 0L;
    }
    m_isConnected = false;
    QTcpSocket *tmp = m_socket;

    disconnect(m_socket, SIGNAL(disconnected()), this,
            SLOT(socketDisconnectedSlot()));
    disconnect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(socketErrorSlot(
                            QAbstractSocket::SocketError)));
    disconnect(m_socket, SIGNAL(readyRead()), this,
            SLOT(socketReadyReadSlot()));
    disconnect(this, SIGNAL(sendData(QByteArray*)), this,
            SLOT(sendSlot(QByteArray*)));
    m_socket = 0L;

    return tmp;
}

void UnknownConnection::socketDisconnectedSlot()
{
    m_mutex.lock();
    m_isConnected = false;
    m_mutex.unlock();

    if (m_unknownConnectionListener != NULL) {
        m_unknownConnectionListener->onConnectionClosed(this);
    }
}

void UnknownConnection::sendSlot(QByteArray *array)
{
    QMutexLocker locker(&this->m_mutex);

    if (m_isConnected && m_socket != 0L) {
        quint64 size = array->size();
        quint64 send = 0;

        do {
            QByteArray arrayTmp = array->right(size - send);
            send += m_socket->write(arrayTmp);
        } while (send < size);
    } else {
        qDebug() << "Writing to not opened connection";
    }

    delete array;
}

void UnknownConnection::disconnectSlot()
{
    m_socket->disconnectFromHost();
}

void UnknownConnection::socketErrorSlot(
        QAbstractSocket::SocketError socketError)
{
    //maybe in future will be implemented in other way but now just
    if (socketError != QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Socket error " << socketError;
    }
}

void UnknownConnection::socketReadyReadSlot()
{
    do {
        if (m_currentMessageId == CHAR_MAX) {
            m_socket->read(&m_currentMessageId, 1);
            qDebug() << (int) m_currentMessageId;
        }

        QByteArray size;
        QByteArray data;
        qint64 currentLeftSize = 0;

        switch (CommunicationProtocol::getType(m_currentMessageId)) {
            case CommunicationProtocol::CONNECT_TO_ALIAS: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                    m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                                            size);
                }

                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::CONNECT_TO_ALIAS> message(data);

                if (m_unknownConnectionListener != 0L) {
                    m_unknownConnectionListener->onConnectToAlias(this,
                            message.getName(), message.getPassword(), message.getId());
                }
            }
                break;

            case CommunicationProtocol::CREATE_ALIAS: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                    m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                                            size);
                }

                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::CREATE_ALIAS> message(data);

                if (m_unknownConnectionListener != 0L) {
                    m_unknownConnectionListener->onCreateAlias(this,
                            message.getName(), message.getPassword());
                }
            }
                break;

            case CommunicationProtocol::REMOVE_ALIAS: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                    m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                                            size);
                }


                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::REMOVE_ALIAS> message(data);

                if (m_unknownConnectionListener != 0L) {
                    m_unknownConnectionListener->onRemoveAlias(this,
                            message.getName(), message.getPassword());
                }
            }
                break;

            case CommunicationProtocol::ADD_DIRECTORY: {
                if (!m_sizeOk) {
                    if (m_socket->bytesAvailable() < 4) {
                        return;
                    }
                    size = m_socket->read(4);
                    m_sizeOk = true;
                    m_messageSize = CommunicationProtocol::getIntFromQByteArray(
                                            size);
                }

                if (m_socket->bytesAvailable() < m_messageSize) {
                    return;
                }

                data = m_socket->read(m_messageSize);
                m_currentMessageId = CHAR_MAX;
                m_sizeOk = false;
                m_messageSize = -1;

                CommunicationProtocol::Communicate<
                        CommunicationProtocol::ADD_DIRECTORY> message(data);

                if (m_unknownConnectionListener != 0L) {
                    m_unknownConnectionListener->onAddDirecotry(this,
                            message.getName(), message.getPassword(), message.getId());
                }
            }
                break;

            default:
                qDebug() << "Unknown code received " << m_currentMessageId;
                break;
        }
    } while ((m_socket != 0L) && (m_socket->bytesAvailable() != 0));
}

} //namespace server
} //namespace TIN_project
