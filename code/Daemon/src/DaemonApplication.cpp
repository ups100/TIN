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

// static members of DaemonApplication class definition
QMutex DaemonApplication::m_mutex;
DaemonApplication* DaemonApplication::instance = NULL;
int DaemonApplication::argc = 1;
char **DaemonApplication::argv = NULL;

DaemonApplication& DaemonApplication::getInstance()
{
    if(!instance) {
        QMutexLocker locker(&m_mutex);
        if (!instance)
            instance = makeInstance(); //it will by call only once
    }

    return *instance;
}
DaemonApplication* DaemonApplication::makeInstance()
{
    static DaemonApplication s;
    return &s;
}

DaemonApplication::DaemonApplication()
        : m_clientCommunication(*this),
          m_isClean(true),
          m_singleApplication(argc,argv) //argc,argv are static fields set by initDaemon() method
{
    connect(this, SIGNAL(onThreadClosedSingal(DaemonThread *)), this, SLOT(onThreadClosedSlot(DaemonThread *)));
}

void DaemonApplication::stopApplication()
{
    m_clientCommunication.terminate(); // TODO kopasiak check that: exit / terminate / quit / leave it alone
    m_clientCommunication.wait();

    foreach (DaemonThread *dt, m_daemonThreads){
        dt->stopThread();
        delete dt;
    }

    // Above we clean all things so object is clean:
    m_isClean = true;
}

DaemonApplication::~DaemonApplication()
{
    // if nobody call stopApplication() method this object will be usually dirty
    if (!m_isClean)
        stopApplication();
}

int DaemonApplication::start(int argc, char **argv)
{
    // Check if it is first instance of application
    if (m_singleApplication.isRunning()) {
        qDebug() << "Another instance of daemon is now running";
        return -1;
    }

    // Run listener for local client
    m_clientCommunication.start();

    if (true) {     // TODO delete this block
        qDebug() << "Pierwszy testowy watek DaemonThread.";
        boost::shared_ptr<DaemonConfiguration::Config> cnf(new DaemonConfiguration::Config());
        QHostAddress addr(QHostAddress::LocalHost);
        cnf->m_ip = addr.toString();
        cnf->m_port = 8080;
        cnf->m_aliasId = "a";
        cnf->m_password = "abc";
        DaemonThread *dt = new DaemonThread(cnf);
            //dt->start();  // TODO delete this line (look below)
            m_daemonThreads.append(dt);
    }

    foreach (boost::shared_ptr<DaemonConfiguration::Config> cnf, m_config.getConfigs()){
        qDebug() << "Tworze watek DaemonThread";    // TODO delete this line
    DaemonThread *dt = new DaemonThread(cnf);
    //dt->start();  //unnecessary because constructor above do everything
    // TODO ewentualnie funkcję start można wykorzystać do tego żeby zwracała status DeamonThread
    // i np jesli połączenie się nie powiodło to tutaj moglibyśmy coś zrobić
    m_daemonThreads.append(dt);
    }

    // Above we create some things so we tell that invocation of stop method is needed before ~DaemonApplication
    m_isClean = false;
    qDebug()<<"start petli zdarzen";
    return m_singleApplication.exec();
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
        //dt->start();  // TODO delete this line
    }
}

void DaemonApplication::removeCatalogueFromAlias(const QString &path,
        const QString &aliasId)
{
    if (m_config.removeConfig(aliasId, path)) {
        foreach (DaemonThread* thread, m_daemonThreads){
        if (thread->getConfig()->m_aliasId == aliasId && thread->getConfig()->m_cataloguePath == path) {
            thread->stopThread();
            // TODO usunąć ten daemonThread z listy
            break;
        }
    }
}
}

void DaemonApplication::onStarted(DaemonThread *dt)
{
    // comment
    qDebug() << "DaemonThread started successful for alias: " << dt->getConfig()->m_aliasId;
    qDebug() << " with catalog" << dt->getConfig()->m_cataloguePath;
}

void DaemonApplication::onStartingError(DaemonThread *dt)
{
    qDebug() << "Error while DaemonThread tries connecting for alias: " << dt->getConfig()->m_aliasId;
    qDebug() << " with catalog" << dt->getConfig()->m_cataloguePath;
    // TODO usunięcie tego demona
}

void DaemonApplication::onClosed(DaemonThread *dt)  // TODO maybe I can use this pointer, but how singleShot to SLOT with value ? I can't
{
    //QMutexLocker lock(&m_mutex); //it is not unlock when signal is emiting
    qDebug() << "Server closed connection with DaemonThread " << dt->getConfig()->m_aliasId
             << " with catalog: "<< dt->getConfig()->m_cataloguePath;

    emit onThreadClosedSingal(dt);
    //QTimer::singleShot(10, this, SLOT(onDaemonThreadClosedSlot()));
}


// Depreciated. Don't use anywhere
void DaemonApplication::onDaemonThreadClosedSlot()
{
    qDebug() << "Depreciated method. Don't use it";
    QMutexLocker lock(&m_mutex);

    QList<DaemonThread*>::iterator it;

    for (it = m_daemonThreads.begin(); it != m_daemonThreads.end(); ) {
        if ( (*it)->isReadyToDestroy() ) {
            m_config.removeConfig((*it)->getConfig()->m_aliasId, (*it)->getConfig()->m_cataloguePath);
            it = m_daemonThreads.erase(it);
        }
    }

    // TODO Consider this concept - closing DaemonApplication when last DaemonThread close
    if (m_daemonThreads.isEmpty()) {
        stopApplication();
        QTimer::singleShot(100, &m_singleApplication, SLOT(quit()));
    }

}

void DaemonApplication::onThreadClosedSlot(DaemonThread *dt)
{
    QMutexLocker lock(&m_mutex);

    // TODO it can done be better:
    removeCatalogueFromAlias(dt->getConfig()->m_cataloguePath ,dt->getConfig()->m_aliasId);
    m_daemonThreads.removeAll(dt);

    // TODO Consider this concept - closing DaemonApplication when last DaemonThread close
     if (m_daemonThreads.isEmpty()) {
         stopApplication();
         QTimer::singleShot(100, &m_singleApplication, SLOT(quit()));
     }
}

QtSingleCoreApplication* DaemonApplication::getSingleApplicationPointer()
{
    return &m_singleApplication;
}

void signal_handler(int sig)
{
    qDebug()<<" Signal_handler";

    DaemonApplication::getInstance().stopApplication();

    // stop DaemonApplication's event loop
    QTimer::singleShot(0, (DaemonApplication::getInstance().getSingleApplicationPointer()), SLOT(quit()));
}

void DaemonApplication::initDaemon(int argc, char **argv)
{
    signal(SIGINT, Daemon::signal_handler);
    DaemonApplication::argc = argc;
    DaemonApplication::argv = argv;
}

} //namespace Daemon
} //namespace TIN_project
