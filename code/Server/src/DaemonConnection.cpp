/**
 * @file DaemonConnection.cpp
 *
 * @date 29-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::DaemonConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */
#include "DaemonConnection.h"
#include "DaemonConnectionListener.h"
#include "CommunicationProtocol.h"

#include <QEventLoop>
#include <QTimer>

namespace TIN_project {

using Utilities::CommunicationProtocol;

namespace Server {

DaemonConnection::DaemonConnection(QTcpSocket *socket, QThread *targetThread,
        DaemonConnectionListener *listener)
        : m_connectionListener(listener), m_socket(socket), m_isConnected(true),
                m_currentMessageId(CHAR_MAX), m_sizeOk(false), m_messageSize(-1)
{
    moveToThread(targetThread);
    m_socket->moveToThread(targetThread);

    connect(m_socket, SIGNAL(disconnected()), this,
            SLOT(socketDisconnectedSlot()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(socketErrorSlot(
                            QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketReadyReadSlot()));
}

DaemonConnection::~DaemonConnection()
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

void DaemonConnection::disconnectFromAlias()
{
    if (!m_isConnected) {
        return;
    }
    m_socket->disconnectFromHost();
}

void DaemonConnection::disconnectFromAliasSynch()
{
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

void DaemonConnection::sendConnectedToAlias()
{
    QTimer::singleShot(0, this, SLOT(sendConnectedToAliasSlot()));
}

void DaemonConnection::sendFindFile(const QString& fileName)
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::FIND_YOUR_FILE> message(
                fileName);
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void DaemonConnection::sendReciveFile(const QString& fileName,
        const QHostAddress& address, quint16 port)
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::RECIVE_FILE> message(
                fileName, address, port);
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void DaemonConnection::sendRemoveFile(const QString& fileName)
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::DELETE_YOUR_FILE> message(fileName);
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void DaemonConnection::sendSendFile(const QString& fileName,
        const QHostAddress& address, quint16 port)
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::SEND_FILE> message(
                fileName, address, port);
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void DaemonConnection::sendConnectedToAliasSlot()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::CONNECTED_TO_ALIAS> message;
        this->sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void DaemonConnection::disconnectSlot()
{
    m_socket->disconnectFromHost();
}

void DaemonConnection::socketDisconnectedSlot()
{
    m_isConnected = false;

    if (m_connectionListener != NULL) {
        m_connectionListener->onConnectionClosed(this);
    }
}

void DaemonConnection::socketErrorSlot(QAbstractSocket::SocketError socketError)
{
    //maybe in future will be implemented in other way but now just
    if (socketError != QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Socket error " << socketError;
    }
}

void DaemonConnection::sendAllFunction(const QByteArray& array)
{
    if (m_isConnected && m_socket != 0L) {
        quint64 size = array.size();
        quint64 send = 0;

        do {
            QByteArray arrayTmp = array.right(size - send);
            send += m_socket->write(arrayTmp);
        } while (send < size);
    } else {
        qDebug() << "Writing to not opened connection";
    }
}

void DaemonConnection::socketReadyReadSlot()
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
            case CommunicationProtocol::FILE_FOUND_HERE: {
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
                        CommunicationProtocol::FILE_FOUND_HERE> message(data);

                if (m_connectionListener != 0L) {
                    m_connectionListener->onFileFound(this,
                            message.getLocation());
                }
            }
                break;

            case CommunicationProtocol::NO_SUCH_FILE_HERE:
                m_currentMessageId = CHAR_MAX;
                if (m_connectionListener != 0L) {
                    m_connectionListener->onNoSuchFile(this);
                }
                break;

            case CommunicationProtocol::FILES_LIST: {
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
                        CommunicationProtocol::FILES_LIST> message(data);

                if (m_connectionListener != 0L) {
                    m_connectionListener->onFileList(this, message.getList());
                }
            }
                break;
            default:
                qDebug() << "Unknown code received " << m_currentMessageId;
                break;
        }
    } while (m_socket->bytesAvailable() != 0);
}

} //namespace server
} //namespace TIN_project
