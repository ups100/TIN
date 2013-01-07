/**
 * @file FileSender.h
 *
 * @date 05-01-2013
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

#if !defined(EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_)
#define EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_

#include <QHostAddress>
#include <QtGlobal>
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QMutex>
#include <QHostAddress>

namespace TIN_project {
namespace Daemon {

class FileTransferListener;

/**
 * @brief Class used for file uploading
 *
 * @details This class runs in separate thread.
 */
class FileSender : public QObject
{
Q_OBJECT

public:
    /**
     * @brief Destructor
     */
    virtual ~FileSender();

    /**
     * @brief Constructor
     *
     * @param listener to be notified about transfer state
     *
     * @param[in] file to be uploaded
     *
     * @param[in] bufferSize size of internal buffer for file reading
     * (1024 bytes as default)
     *
     * @note File passed to constructor should be present on disk and have read rights
     * This class takes the ownership of file object passed as argument.
     */
    FileSender(FileTransferListener *listener, QFile *file, qint64 bufferSize  = 1024);

    /**
     * @brief Starts new thread and connects to server
     *
     * @details Synchronous method
     *
     * @param[in] address of server
     *
     * @param[in] port of server
     *
     * @return
     * - true if connection established
     * - false otherwise
     */
    bool connectToServer(const QHostAddress& address, quint16 port);

    /**
     * @brief Stops the file transfer and disconnects from server
     *
     * @details Synchronous method. If not connected this function does nothing
     */
    void disconnectFromServer();

    /**
     * @brief Check if file is property opened
     *
     * @return
     * - true if file has been successfully opened
     * - false otherwise
     */
    inline bool isFileOk()
    {
        return !(m_state == FILE_ERROR);
    }

signals:

    /**
     * @brief Helper signal for socket connecting
     *
     * @warning Do not connect this signal to any of your slots.
     * Only for class internal usage!
     */
    void connectingFinished();

private slots:

    /**
     * @brief Helper slot used for connection establishing
     */
    void establishConnectionSlot();

    /**
     * @brief Slot executed when socket has been connected
     */
    void socketConnectedSlot();

    /**
     * @brief Slot executed when socket has been disconnected
     */
    void socketDisconnectedSlot();

    /**
     * @brief Slot executed when some error occurred in socket
     *
     * @param[in] error type
     */
    void socketErrorSlot(QAbstractSocket::SocketError error);

    /**
     * @brief Helper slot used to start file upload
     */
    void startDataSendingSlot();

    /**
     * @brief Slot executed when next part of data has been written to native socket
     */
    void bytesWrittenSlot(qint64 bytes);

    /**
     * @brief Slot executed when additional thread has finished execution
     */
    void threadFinishedSlot();

    /**
     * @brief Helper slot used for synchronous disconnecting
     */
    void synchCloseImplementationSlot();

private:
    /**
     * @brief Object notified about transfer states
     */
    FileTransferListener *m_FileTransferListener;

    /**
     * @brief Additional thread for file transfer
     */
    QThread m_additionalThread;

    /**
     * @brief Thread which created this object
     */
    QThread *m_creatorThread;

    /**
     * @brief Socket used for file transfer
     */
    QTcpSocket *m_socket;

    /**
     * @brief File to be uploaded
     */
    QFile *m_file;

    /**
     * @brief Address of server for connection
     */
    QHostAddress m_address;

    /**
     * @brief Server's port
     */
    quint16 m_port;

    /**
     * @brief Enum which represents states of this class.
     */
    enum STATE {
        /**
         * @brief Default state. Nothing running.
         */
        UNCONNECTED,

        /**
         * @brief Socket has been created and connection established
         */
        CONNECTED,

        /**
         * @brief Some error occurred in socket
         */
        ERROR,

        /**
         * @brief Transfer has been started
         */
        TRANSFER_IN_PROGRESS,

        /**
         * @brief Transfer has been finished
         */
        TRANSFER_FINISHED,

        /**
         * @brief All data has been read from file
         */
        ALL_DATA_READ,

        /**
         * @brief All data has been written to socket
         */
        ALL_DATA_WRITTEN,

        /**
         * @brief Synchronous close was called
         */
        CLOSING,

        /**
         * @brief State used when file not exist or unable to open file
         */
        FILE_ERROR
    };

    /**
     * @brief State of this object
     */
    STATE m_state;

    /**
     * @brief Size of data to be written at once
     */
    qint64 m_bufferSize;

    /**
     * @brief Buffer for data reading
     */
    QByteArray m_buffer;

    /**
     * @brief Mutex for synchronization
     */
    QMutex m_mutex;

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_)
