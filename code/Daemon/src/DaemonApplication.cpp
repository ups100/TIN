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
    delete dt;
}
}

int DaemonApplication::start()
{
    // Run listener for local client
    m_clientCommunication.start();

    foreach (boost::shared_ptr<DaemonConfiguration::Config> cnf, m_config.getConfigs()){
    DaemonThread *dt = new DaemonThread(cnf);
    dt->start(); // TODO if no thread js check
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
    using namespace Utilities;

    CommunicationProtocol::CommunicateType code =
            CommunicationProtocol::getType(communicate.left(1)[0]);
    Utilities::Message msg;

    if (code == CommunicationProtocol::ADD_DIRECTORY_AND_CONNECT) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::ADD_DIRECTORY_AND_CONNECT> message(
                communicate.mid(5, communicate.length()));
        msg = message.getMessage();

        addCatalogueToAlias(msg.getCataloguePath(), msg.getAliasId(),
                msg.getAliasPassword(), msg.getServerIpAddress(),
                msg.getServerPort());
        qDebug() << "addCatalogueToAlias()"; //TODO remove that

    } else if (code == CommunicationProtocol::REMOVE_DIRECTORY_AND_DISCONNECT) {
        CommunicationProtocol::Communicate<
                CommunicationProtocol::REMOVE_DIRECTORY_AND_DISCONNECT> message(
                communicate.mid(5, communicate.length()));
        msg = message.getMessage();

        removeCatalogueFromAlias(msg.getCataloguePath(), msg.getAliasId());
        qDebug() << "removeCatalogueFromAlias()"; //TODO remove that

    } else {
        return;
    }

    if (m_daemonThreads.size())
        m_daemonThreads.at(qrand() % m_daemonThreads.size())->onListFiles();

    qDebug() << "Waiting 4 a next message"; // TODO remove
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
        dt->start(); // TODO check if no thread etc
    }
}

void DaemonApplication::removeCatalogueFromAlias(const QString &path,
        const QString &aliasId)
{
    if (m_config.removeConfig(aliasId, path)) {
        foreach (DaemonThread* thread, m_daemonThreads){
        if (thread->getConfig()->m_aliasId == aliasId && thread->getConfig()->m_cataloguePath == path) {
            thread->stopThread(); // TODO check if no thread etc
            break;
        }
    }
}
}

} //namespace Daemon
} //namespace TIN_project
