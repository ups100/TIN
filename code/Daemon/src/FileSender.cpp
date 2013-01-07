/**
 * @file FileSender.cpp
 *
 * @date 06-01-2013
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Daemon::FileSender
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include <QMutexLocker>
#include <QEventLoop>
#include <QTimer>

#include "FileSender.h"
#include "FileTransferListener.h"

namespace TIN_project {
namespace Daemon {

FileSender::FileSender(FileTransferListener *listener, QFile *file,
        qint64 bufferSize = 1024)
        : m_FileTransferListener(listener), m_socket(0L), m_file(file),
                m_port(0), m_state(UNCONNECTED), m_bufferSize(bufferSize)
{
    m_creatorThread = QThread::currentThread();

    connect(&m_additionalThread, SIGNAL(started()), this,
            SLOT(establishConnectionSlot()));

    if (!(m_file->exists())) {
        qDebug() << "File " << m_file->fileName() << " does not exist";
        m_state = FILE_ERROR;
        return;
    }

    if (!(m_file->open(QIODevice::ReadOnly))) {
        qDebug() << "Unable to open " << m_file->fileName() << " for reading";
        m_state = FILE_ERROR;
        return;
    }
}

FileSender::~FileSender()
{
    if (m_state == UNCONNECTED) {
        qDebug() << "Destroying transfer object but transfer still in progress";
        disconnectFromServer();
    }

    delete m_file;
}

bool FileSender::connectToServer(const QHostAddress& address, quint16 port)
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

void FileSender::disconnectFromServer()
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

void FileSender::establishConnectionSlot()
{
    m_socket = new QTcpSocket();

    connect(m_socket, SIGNAL(connected()), this, SLOT(socketConnectedSlot()));

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(startDataSendingSlot()));

    connect(m_socket, SIGNAL(bytesWritten(qint64)), this,
                    SLOT(bytesWrittenSlot(qint64)));

    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(socketErrorSlot(QAbstractSocket::SocketError)));

    connect(m_socket, SIGNAL(disconnected()), this,
            SLOT(socketDisconnectedSlot()));

    m_socket->connectToHost(m_address, m_port);
}

void FileSender::socketConnectedSlot()
{
    m_state = CONNECTED;
    connect(&m_additionalThread, SIGNAL(finished()), this,
            SLOT(threadFinishedSlot()));
    emit connectingFinished();
}

void FileSender::socketErrorSlot(QAbstractSocket::SocketError error)
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

void FileSender::socketDisconnectedSlot()
{
    if ((m_state != TRANSFER_FINISHED) && (m_state != CLOSING)) {
        m_state = ERROR;
    }

    moveToThread(m_creatorThread);
    m_additionalThread.moveToThread(m_creatorThread);

    m_additionalThread.quit();
}

void FileSender::startDataSendingSlot()
{
    if (m_state != CONNECTED) {
        m_state = ERROR;
        qDebug() << "Received more than one data peak";

        m_socket->disconnectFromHost();
        return;
    }

    m_socket->readAll();

    m_state = TRANSFER_IN_PROGRESS;

    m_buffer = m_file->read(m_bufferSize);

    qint64 written = m_socket->write(m_buffer);

    m_buffer = m_buffer.right(m_buffer.size() - written);
}

void FileSender::bytesWrittenSlot(qint64 bytes)
{
    if (m_state == ALL_DATA_WRITTEN) {
        if (m_socket->bytesToWrite() != 0) {
            return;
        }

        m_state = TRANSFER_FINISHED;
        disconnect(m_socket, SIGNAL(bytesWritten(qint64)), this,
                SLOT(bytesWrittenSlot(qint64)));

        m_socket->disconnectFromHost();
    } else {
        if ((!(m_state == ALL_DATA_READ)) && (m_buffer.size() < m_bufferSize)) {
            m_buffer.append(m_file->read(m_bufferSize - m_buffer.size()));

            if (m_file->atEnd()) {
                m_state = ALL_DATA_READ;
                m_file->close();
            }
        }

        qint64 written = m_socket->write(m_buffer);

        m_buffer = m_buffer.right(m_buffer.size() - written);

        if ((m_state == ALL_DATA_READ) && (m_buffer.size() == 0)) {
            m_state = ALL_DATA_WRITTEN;
        }
    }
}

void FileSender::threadFinishedSlot()
{
    if (m_state == TRANSFER_FINISHED) {
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

void FileSender::synchCloseImplementationSlot()
{
    m_state = CLOSING;
    m_socket->disconnectFromHost();
}

} //namespace Daemon
} //namespace TIN_project
