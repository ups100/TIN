///////////////////////////////////////////////////////////
//  DaemonApplication.cpp
//  Implementation of the Class DaemonApplication
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonApplication.h"
#include <QDebug>
#include <QRegExp>

namespace TIN_project {
namespace Daemon {

DaemonApplication::DaemonApplication()
        : m_clientCommunication(this)
{

}

DaemonApplication::~DaemonApplication()
{
    m_clientCommunication.terminate(); // TODO kopasiak check that: exit / terminate / quit / leave it alone
    m_clientCommunication.wait();
}

int DaemonApplication::start()
{
    // Run listener for local client
    m_clientCommunication.start();

    //TODO test remove
    addCatalogueToAlias(QString("/home/kajo/workspace/tin"),
            QString("Alias_Kajo"), Utilities::Password(QString("passwd")),
            QHostAddress("127.0.0.0"), 80);

    removeCatalogueFromAlias("", "");

    // TODO remove demo loop
    qDebug() << "Waiting 4 a message";
    while (1) {
        qDebug() << ".";
        sleep(1);
    }

    return 0;
}

// TODO dispatch message to do what is needed
void DaemonApplication::dispatchMessage(const Utilities::Message &message) const
{
    qDebug() << message.message();
    qDebug() << "Waiting 4 a message";
}

void DaemonApplication::addCatalogueToAlias(const QString &path,
        const QString &aliasId, const Utilities::Password &password,
        const QHostAddress& ip, quint16 port)
{
    boost::shared_ptr<DaemonConfiguration::Config> config(
            new DaemonConfiguration::Config(ip.toString(),
                    QString::number(port), aliasId, password.password(), path));

//    /*
//     * Check overlap duplicate
//     */
//    {
//    // TODO if given path is higher in filesystem tree replace oldone to that ?, what if more than one is inside ? For now just do nothing
//        QRegExp regex(QString("^(") + path + ")");
//    }

    m_config.addConfig(config);
}

void DaemonApplication::removeCatalogueFromAlias(const QString &path,
        const QString &aliasId)
{
    m_config.removeConfig(aliasId, path);
}

} //namespace Daemon
} //namespace TIN_project
