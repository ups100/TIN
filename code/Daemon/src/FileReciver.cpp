/**
 * @file FileReciver.cpp
 *
 * @date 07-01-2013
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Daemon::FileReciver
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */
#include "FileReciver.h"
#include "FileTransferListener.h"

#include <QMutexLocker>
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QThread>
#include <QTcpSocket>
#include <QMutexLocker>



namespace TIN_project {
namespace Daemon {

FileReciver::~FileReciver()
{
    if (m_state == UNCONNECTED) {
        qDebug() << "Destroying transfer object but transfer still in progress";
        disconnectFromServer();
    }

    delete m_file;
}

FileReciver::FileReciver(FileTransferListener *listener, QFile *file,
        qint64 fileSize)
        : m_FileTransferListener(listener), m_socket(0L), m_file(file),
                m_port(0), m_state(UNCONNECTED), m_fileSize(fileSize),
                m_currentlyReceived(0)
{
    m_creatorThread = QThread::currentThread();

    connect(&m_additionalThread, SIGNAL(started()), this,
            SLOT(establishConnectionSlot()));

    if (!(m_file->open(QIODevice::ReadWrite))) {
        qDebug() << "Unable to open " << m_file->fileName()
                << " for reading and writing";
        m_state = FILE_ERROR;
        return;
    }
}

bool FileReciver::connectToServer(const QHostAddress& address, quint16 port)
{
    QMutexLocker locker(&(this->m_mutex));

    if (m_state != UNCONNECTED) {
        return false;
    }

    this->m_address = address;
    this->m_port = port;

    moveToThread(&m_additionalThread);
    m_additionalThread.moveToThread(&m_additionalThread);

    QEventLoop loop;

    connect(this, SIGNAL(connectingFinished()), &loop, SLOT(quit()));
    connect(&m_additionalThread, SIGNAL(finished()), &loop, SLOT(quit()));

    m_additionalThread.start();

    loop.exec();

    if (m_state == UNCONNECTED) {
        delete m_socket;
        m_socket = 0L;
    }

    return !(m_state == UNCONNECTED);
}

void FileReciver::disconnectFromServer()
{
    QMutexLocker locker(&(this->m_mutex));

    if ((m_state == UNCONNECTED) || (m_state == ERROR)
            || (m_state == FILE_ERROR)) {
        return;
    }

    QEventLoop loop;

    disconnect(&m_additionalThread, SIGNAL(finished()), this,
            SLOT(threadFinishedSlot()));
    connect(&m_additionalThread, SIGNAL(finished()), &loop, SLOT(quit()));

    QTimer::singleShot(0, this, SLOT(synchCloseImplementationSlot()));

    loop.exec();

    m_state = UNCONNECTED;
    delete m_socket;
    m_socket = 0L;
}

void FileReciver::establishConnectionSlot()
{
    m_socket = new QTcpSocket();

    connect(m_socket, SIGNAL(connected()), this, SLOT(socketConnectedSlot()));

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(receiveDataSlot()));

    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(socketErrorSlot(QAbstractSocket::SocketError)));

    connect(m_socket, SIGNAL(disconnected()), this,
            SLOT(socketDisconnectedSlot()));

    m_socket->connectToHost(m_address, m_port);
}

void FileReciver::socketConnectedSlot()
{
    m_state = CONNECTED;
    connect(&m_additionalThread, SIGNAL(finished()), this,
            SLOT(threadFinishedSlot()));
    emit connectingFinished();
}

void FileReciver::socketErrorSlot(QAbstractSocket::SocketError error)
{
    switch (error) {
        case QAbstractSocket::ConnectionRefusedError:
        case QAbstractSocket::HostNotFoundError:
            m_state = UNCONNECTED;
            break;
        case QAbstractSocket::RemoteHostClosedError:
            m_state = ERROR;
            break;
        default:
            qDebug() << "Socket error " << error
                    << " occurred during file transfer";
            break;
    }

    moveToThread(m_creatorThread);
    m_additionalThread.moveToThread(m_creatorThread);

    m_additionalThread.quit();
}

void FileReciver::socketDisconnectedSlot()
{
    if ((m_state != TRANSFER_FINISHED) && (m_state != CLOSING)) {
        m_state = ERROR;
    }

    moveToThread(m_creatorThread);
    m_additionalThread.moveToThread(m_creatorThread);

    m_additionalThread.quit();
}

void FileReciver::receiveDataSlot()
{
    if (m_state == CONNECTED) {
        m_state = TRANSFER_IN_PROGRESS;
        m_socket->read(1);
    }

    QByteArray buffer = m_socket->readAll();

    m_currentlyReceived += buffer.size();

    m_file->write(buffer);
    qDebug()<<"leci transfer";
    if (m_currentlyReceived == m_fileSize) {
        m_file->close();
        qDebug()<<"status finished";
        m_state = TRANSFER_FINISHED;

        m_socket->disconnectFromHost();

    } else if (m_currentlyReceived > m_fileSize) {
        qDebug() << "Received too large file";
        m_state = ERROR;

        m_socket->disconnectFromHost();
    }

}

void FileReciver::threadFinishedSlot()
{
    if (m_state == TRANSFER_FINISHED) { qDebug()<<"finished fun";
        if (m_FileTransferListener != 0L) {
            m_FileTransferListener->onTransferEnd(this);
        }
    } else if (m_state == CLOSING) {
        qDebug() << "Forcing close file transfer";
    } else {
        if (m_FileTransferListener != 0L) {
            m_FileTransferListener->onTransferError(this);
        }
    }

    m_state = UNCONNECTED;
    delete m_socket;
    m_socket = 0L;
}

void FileReciver::synchCloseImplementationSlot()
{
    m_state = CLOSING;
    m_socket->disconnectFromHost();
}

} //namespace Daemon
} //namespace TIN_project
