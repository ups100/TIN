///////////////////////////////////////////////////////////
//  DaemonApplication.h
//  Implementation of the Class DaemonApplication
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_)
#define EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_

#include "ClientCommunication.h"
#include "DaemonThread.h"
#include "Message.h"
#include "Password.h"
#include "DaemonConfiguration.h"
#include <QList>
#include <QHostAddress>
#include <QString>

namespace TIN_project {
namespace Daemon {

class DaemonApplication
{
public:

    DaemonApplication();
    virtual ~DaemonApplication();

    int start();
    void dispatchMessage(const Utilities::Message &message);

    /**
     * @brief Add catalogue to alias, start DaemonThread, and add daemon data to config
     * @param path Path of catalogue wanted to add
     * @param aliasId Id of alias to which want to add
     * @param password Password to alias
     * @param ip Ip to server managing alias
     * @param port Port to server managing alias
     */
    void addCatalogueToAlias(const QString &path, const QString &aliasId,
            const Utilities::Password &password, const QHostAddress& ip,
            quint16 port);

    /**
     * @brief Removes catalogue from alias, removes it from config and
     *  stop Daemon Thread responsible for that path
     * @param path Path to catalogue
     * @param aliasId Alias containging catalogue
     */
    void removeCatalogueFromAlias(const QString &path, const QString &aliasId);

private:

    QList<DaemonThread*> m_daemonThreads;

    /** Client communication thread filed */
    ClientCommunication m_clientCommunication;

    /** Daemon threads configuration */
    DaemonConfiguration m_config;
};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_)
