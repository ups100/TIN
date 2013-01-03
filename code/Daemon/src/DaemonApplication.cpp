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
        : m_clientCommunication(*this)
{

}

DaemonApplication::~DaemonApplication()
{
    m_clientCommunication.terminate(); // TODO kopasiak check that: exit / terminate / quit / leave it alone
    m_clientCommunication.wait();

    foreach (DaemonThread *dt, m_daemonThreads){
    dt->stopThread();
    dt->wait();
    delete dt;
}
}

int DaemonApplication::start()
{
    // Run listener for local client
    m_clientCommunication.start();

    foreach (boost::shared_ptr<DaemonConfiguration::Config> cnf, m_config.getConfigs()){
    DaemonThread *dt = new DaemonThread(cnf);
    dt->start();
    m_daemonThreads.append(dt);
}

// TODO remove demo loop
    qDebug() << "Waiting 4 a message";
    while (1) {
        qDebug() << ".";
        sleep(1);
    }

    return 0;
}

// TODO dispatch message to do what is needed
void DaemonApplication::dispatchMessage(const Utilities::Message &message)
{
    qDebug() << message.getMessage();
    qDebug() << "Waiting 4 a message";

    m_daemonThreads.at(qrand() % m_daemonThreads.size())->onListFiles();

//    if (m_daemonThreads.size())
//    switch (qrand() % 4) {
//        case 0:
//            m_daemonThreads.at(qrand() % m_daemonThreads.size())->onListFiles();
//            break;
//        case 1:
////            addCatalogueToAlias(
////                    QString("/random/") + QString::number(qrand() % 20),
////                    QString("Testowy"), Utilities::Password(QString("pass")),
////                    QHostAddress("192.168.1.1"), 23);
//            break;
//        case 2:
////            removeCatalogueFromAlias(
////                    QString("/random/") + QString::number(qrand() % 20),
////                    QString("Testowy"));
//            break;
//        case 3:
//            m_daemonThreads.at(qrand() % m_daemonThreads.size())->onFindFile(
//                    message.getMessage());
//            break;
//    }
}

void DaemonApplication::addCatalogueToAlias(const QString &path,
        const QString &aliasId, const Utilities::Password &password,
        const QHostAddress& ip, quint16 port)
{
    boost::shared_ptr<DaemonConfiguration::Config> config(
            new DaemonConfiguration::Config(ip.toString(), port, aliasId,
                    password.getPassword(), path));

//    /*
//     * Check overlap duplicate
//     */
//    {
//    // TODO if given path is higher in filesystem tree replace oldone to that ?, what if more than one is inside ? For now just do nothing
//        QRegExp regex(QString("^(") + path + ")");
//    }

    if (m_config.addConfig(config)) {
        DaemonThread *dt = new DaemonThread(config);
        m_daemonThreads.append(dt);
        dt->start();
    }
}

void DaemonApplication::removeCatalogueFromAlias(const QString &path,
        const QString &aliasId)
{
    if (m_config.removeConfig(aliasId, path)) {
        foreach (DaemonThread* thread, m_daemonThreads){
        if (thread->config()->m_aliasId == aliasId && thread->config()->m_cataloguePath == path) {
            thread->stopThread();
            break;
        }
    }
}
}

} //namespace Daemon
} //namespace TIN_project
