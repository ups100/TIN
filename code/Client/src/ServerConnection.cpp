/**
 * @file Client/include/ServerConnection.h
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

#include <QEventLoop>
#include <QTimer>

namespace TIN_project {
namespace Client {

ServerConnection::ServerConnection(ServerConnectionListener *serverListener,
        AliasCommunicationListener *aliasListener)
        : m_socket(0L), m_creatorThread(0L), m_serverListener(serverListener),
                m_aliasListener(aliasListener), m_isReadyState(false),
                m_isConnecting(false), m_isClosing(false)
{
    m_creatorThread = QThread::currentThread();

    connect(&m_additionalThread, SIGNAL(started()), this,
            SLOT(threadStartedSlot()));
    connect(&m_additionalThread, SIGNAL(finished()), this,
            SLOT(threadFinishedSlot()));
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

}

void ServerConnection::findFileInAlias(const QString& fileName)
{

}

void ServerConnection::listAlias()
{

}

void ServerConnection::pullFileFrom(const Utilities::FileLocation& file)
{

}

/**
 * not sure about semantic of this method
 */
void ServerConnection::pushFileToAlias(const QString& path)
{

}

/**
 * not sure about semantic of this method 
 */
void ServerConnection::removeFileFromAlias(const QString& fileName)
{

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
    socketDisconnectedSlot();
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
        QByteArray arrayTmp = array->right( size - send);
        send += m_socket->write(arrayTmp);
    } while(send < size);

    delete array;
}

void ServerConnection::socketReadyReadSlot()
{

}

} //namespace Client
} //namespace TIN_project
