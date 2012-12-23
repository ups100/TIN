/**
 * @file ClientConnection.cpp
 *
 * @date 23-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::ClientConnection
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "ClientConnection.h"
#include "FileLocation.h"
#include "AliasFileList.h"
#include "CommunicationProtocol.h"

#include <climits>
#include <QTcpSocket>
#include <QEventLoop>
#include <QTimer>

namespace TIN_project {

using Utilities::CommunicationProtocol;

namespace Server {

ClientConnection::ClientConnection(QTcpSocket *socket, QThread *targetThread,
        ClientConnectionListener *listener)
        : m_connectionListener(listener), m_socket(socket), m_isConnected(true),
                m_currentMessageId(CHAR_MAX), m_sizeOk(false), m_messageSize(-1)
{
    moveToThread(targetThread);
    m_socket->moveToThread(targetThread);
}

ClientConnection::~ClientConnection()
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

void ClientConnection::disconnectFromAlias()
{
    if (!m_isConnected) {
        return;
    }
    m_socket->disconnectFromHost();
}

void ClientConnection::disconnectFromAliasSynch()
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

void ClientConnection::sendFileFound(const Utilities::FileLocation& location)
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::FILE_LOCATION> message(
                location);
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendFileList(const Utilities::AliasFileList& list)
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::ALIAS_LISTED> message(
                list);
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendFileNotFound()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::FILE_NOT_FOUND> message;
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendFileNotRemoved()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::NOT_DELETED_FROM_ALIAS> message;
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendFileRemoved()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate
                < CommunicationProtocol::DELETED_FROM_ALIAS> message;
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendFileTransferError()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::TRANSFER_ERROR> message;
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendFileTransferFinished()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::TRANSFER_FINISHED> message;
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendFileTransferStarted()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::TRANSFER_IN_PROGRESS> message;
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::sendNoSuchFile()
{
    if (m_isConnected) {
        CommunicationProtocol::Communicate<CommunicationProtocol::NO_SUCH_FILE> message;
        sendAllFunction(message.toQByteArray());
    } else {
        qDebug() << "Trying to send but connection is not opened";
    }
}

void ClientConnection::disconnectSlot()
{
    m_socket->disconnectFromHost();
}

void ClientConnection::socketDisconnectedSlot()
{
    m_isConnected = false;

    if (m_connectionListener != NULL) {
        m_connectionListener->onConnectionClosed(this);
    }
}

void ClientConnection::socketErrorSlot(QAbstractSocket::SocketError socketError)
{
    //maybe in future will be implemented in other way but now just
    if (socketError != QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Socket error " << socketError;
    }
}

void ClientConnection::socketReadyReadSlot()
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
            case CommunicationProtocol::CONNECT_TO_ALIAS:

            default:
                qDebug() << "Unknown code received " << m_currentMessageId;
                break;
        }
    } while (m_socket->bytesAvailable() != 0);
}

} //namespace server
} //namespace TIN_project
