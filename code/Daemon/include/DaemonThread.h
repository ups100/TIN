/**
 * @file DaemonThread.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Daemon::DaemonThread
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_)
#define EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_

#include <QHostAddress>
#include <QtGlobal>
#include <QString>
#include <boost/shared_ptr.hpp>
#include <QThread>
#include <QEventLoop>
#include "FileTransferListener.h"
#include "ServerConnectionListener.h"
#include "ServerConnection.h"
#include "DaemonConfiguration.h"
#include "FileReciver.h"
#include "FileSender.h"

namespace TIN_project {
namespace Daemon {

class DaemonThread : public FileTransferListener,
        public ServerConnectionListener
{

public:
    DaemonThread();
    virtual ~DaemonThread();

    DaemonThread(boost::shared_ptr<DaemonConfiguration::Config> config);
    virtual void onAliasConnected();
    virtual void onAliasConnectionError();
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onFindFile(const QString& fileName);
    virtual void onFileNotRemoved();
    virtual void onListFiles();
    virtual void onReciveFile(const QString& fileName, const QHostAddress& address, quint16 port, qint64 size);
    virtual void onRemoveFile(const QString& fileName);
    virtual void onSendFile(const QString& fileName, const QHostAddress& address, quint16 port);
    virtual void onTransferEnd(FileSender * sender);
    virtual void onTransferError(FileSender *sender);
    virtual void onTransferEnd(FileReciver * receiver);
    virtual void onTransferError(FileReciver *receiver);
    /** @brief Stop DeamonThread communication */
    void stopThread();
    //void start(); // it could return status of starting DeamonThread only

    boost::shared_ptr<DaemonConfiguration::Config> getConfig();
    /** @brief Tells if this object is ready to destroy **/
    bool isReadyToDestroy();

private:
    DaemonThread(const DaemonThread&);
    DaemonThread(DaemonThread&);
    boost::shared_ptr<DaemonConfiguration::Config> m_config;
    ServerConnection *m_ServerConnection;
    FileReciver *m_receiver;
    FileSender *m_sender;
    QEventLoop *m_loop;

    /**
     * @brief Cut absolute file path to relative to supported catalogue
     *
     * @return String with cutted path
     */
    QString& cutAbsolutePath(QString &str);

    /**
     * @brief Show connection status.
     * @details If connection is established value true.
     */
    bool m_connectionOk;

    /**
     * @brief True if alias connected successful.
     */
    bool m_aliasConnected;

    /**
     * @brief Tells if this object is prepare to destroy by DaemonApplication;
     *  This is set by onDisconect method before calling DaemonApplication::onClosed
     */
    bool m_readyToDestroy;

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_)
