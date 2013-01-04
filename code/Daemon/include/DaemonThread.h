///////////////////////////////////////////////////////////
//  DaemonThread.h
//  Implementation of the Class DaemonThread
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_)
#define EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_

#include <QHostAddress>
#include <QtGlobal>
#include <QString>
#include <boost/shared_ptr.hpp>
#include <QThread>
#include "FileTransferListener.h"
#include "ServerConnectionListener.h"
#include "ServerConnection.h"
#include "DaemonConfiguration.h"

namespace TIN_project {
namespace Daemon {

class DaemonThread : public FileTransferListener,
        public ServerConnectionListener, public QThread
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
    virtual void onReciveFile(const QString& fileName, const QHostAddress& address, quint16 port);
    virtual void onRemoveFile(const QString& fileName);
    virtual void onSendFile(const QString& fileName, const QHostAddress& address, quint16 port);
    virtual void onTransferEnd(FileSender * sender);
    virtual void onTransferEnd(FileReciver * reciver);
    void stopThread();
    void run();

    boost::shared_ptr<DaemonConfiguration::Config> getConfig();

private:
    boost::shared_ptr<DaemonConfiguration::Config> m_config;
    ServerConnection *m_ServerConnection;

    /**
     * @brief Cut absolute file path to relative to supported catalogue
     *
     * @return String with cutted path
     */
    QString& cutAbsolutePath(QString &str);

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_)
