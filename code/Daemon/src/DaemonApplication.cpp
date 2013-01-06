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
    //QtSingleCoreApplication application(argc, argv);

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

// TODO dispatch message to do what is needed
void DaemonApplication::dispatchMessage(const Utilities::Message &message)
{
    qDebug() << message.getMessage();
    qDebug() << "Waiting 4 a message";

    if (m_daemonThreads.size())
        m_daemonThreads.at(qrand() % m_daemonThreads.size())->onFindFile(
                message.getMessage());

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
    qDebug() << "DaemonThread started for alias: " << dt->getConfig()->m_aliasId;
}

void DaemonApplication::onStartingError(DaemonThread *dt)
{
    qDebug() << "DaemonThread starting error. ";
}

void DaemonApplication::onClosed(DaemonThread *dt)  // TODO maybe I can use this pointer, but how singleShot to SLOT with value ? I can't
{
    QMutexLocker lock(&m_mutex);
    qDebug() << "Server closed connection with DaemonThread " << dt->getConfig()->m_cataloguePath;

    QTimer::singleShot(0, this, SLOT(onDaemonThreadClosedSlot()));
}


void DaemonApplication::onDaemonThreadClosedSlot()
{
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
