/**
 * @file DaemonApplication.cpp
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Daemon::DaemonApplication
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "DaemonApplication.h"
#include "CommunicationProtocol.h"
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

void DaemonApplication::dispatchMessage(const QByteArray &communicate)
{
    uchar code = communicate.left(1)[0];
    Utilities::Message msg;

    // I know.. ;]
    if (code == 32) {
        Utilities::CommunicationProtocol::Communicate<32> message(
                communicate.mid(5, communicate.length()));
        msg = message.getMessage();

        qDebug() << "addCatalogueToAlias()"; //TODO invoke it
    } else if (code == 33) {
        Utilities::CommunicationProtocol::Communicate<33> message(
                communicate.mid(5, communicate.length()));
        msg = message.getMessage();

        qDebug() << "removeCatalogueFromAlias()"; //TODO invoke it
    } else {
        return;
    }

    qDebug() << msg.getAliasId();
    qDebug() << "Waiting 4 a message";

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
                    password.getHash(), path));

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
        if (thread->getConfig()->m_aliasId == aliasId && thread->getConfig()->m_cataloguePath == path) {
            thread->stopThread();
            break;
        }
    }
}
}

} //namespace Daemon
} //namespace TIN_project
