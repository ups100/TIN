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
    if (!instance) {
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
        : m_singleApplication(argc,argv), //argc,argv are static fields set by initDaemon() method
          m_isClean(true)
{
    connect(this, SIGNAL(onThreadClosedSingal(DaemonThread *)), this, SLOT(onThreadClosedSlot(DaemonThread *)));
    // to avoid communication with client when this is the second running proces of ./daemon application
    if (!m_singleApplication.isRunning())
        m_clientCommunication = new ClientCommunication(*this);
}

void DaemonApplication::stopApplication()
{
    qDebug() << "stopApplication() method";
    m_clientCommunication->terminate();
    m_clientCommunication->wait();

    foreach (DaemonThread *dt, m_daemonThreads){
        qDebug() << "Stopping... " << dt->getConfig()->m_cataloguePath;
        dt->stopThread();
        delete dt;
    }
    // remove all element from the list
    m_daemonThreads.clear();

// Above we clean all things so object is clean:
    m_isClean = true;

    // End event loop so DaemonApplication is ended at all
    QTimer::singleShot(0, &m_singleApplication, SLOT(quit()));
}

DaemonApplication::~DaemonApplication()
{
    // if nobody call stopApplication() method this object will be usually dirty
    if (!m_isClean)
        stopApplication();
}

int DaemonApplication::start()
{
    // Check if it is first instance of application
    if (m_singleApplication.isRunning()) {
        qDebug() << "Another instance of daemon is now running";
        return -1;
    }

    // Run listener for local client
    m_clientCommunication->start();

    if (true) {     // TODO delete this block
        qDebug() << "Pierwszy testowy watek DaemonThread.";
        /*boost::shared_ptr<DaemonConfiguration::Config> cnf(new DaemonConfiguration::Config());
        QHostAddress addr(QHostAddress::LocalHost);
        cnf->m_ip = addr.toString();
        cnf->m_port = 8080;
        cnf->m_aliasId = "a";
        cnf->m_password = "abc";
        m_config.addConfig(cnf);
        DaemonThread *dt = new DaemonThread(cnf);*/

        //addCatalogueToAlias(".","a", Utilities::Password(QString("abc")), QHostAddress::LocalHost, 8080);
            //dt->start();  // TODO delete this line (look below)

           //m_daemonThreads.append(dt);

           qDebug() << "m_deamonThread: ";
           qDebug() << m_daemonThreads.size();
    }

    foreach (boost::shared_ptr<DaemonConfiguration::Config> cnf, m_config.getConfigs()){
        qDebug() << "Tworze watek DaemonThread";    // TODO delete this line
    DaemonThread *dt = new DaemonThread(cnf);
    //dt->start();  //unnecessary because constructor above do everything
    // TODO ewentualnie funkcję start można wykorzystać do tego żeby zwracała status DeamonThread -- TODO nie, bo to robi DaemonThreadListener
    // i np jesli połączenie się nie powiodło to tutaj moglibyśmy coś zrobić
    m_daemonThreads.append(dt);
    }

    // Above we create some things so we tell that invocation of stop method is needed before ~DaemonApplication
    m_isClean = false;
    return m_singleApplication.exec();
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
    //QMutexLocker lock(&m_mutex); // synchronization // TODO

    if (m_config.removeConfig(aliasId, path)) {
        foreach (DaemonThread* thread, m_daemonThreads){
        if (thread->getConfig()->m_aliasId == aliasId && thread->getConfig()->m_cataloguePath == path) {
            thread->stopThread(); // TODO check if no thread etc
            delete thread;
            m_daemonThreads.removeOne(thread); // disconnect this from the list
            break;
        }
    }
}
}

void DaemonApplication::detachDaemonThread(DaemonThread *dt)
{
    QMutexLocker lock(&m_mutex); // synchronization

    QString aliasId(dt->getConfig()->m_aliasId);
    QString path(dt->getConfig()->m_cataloguePath);

    foreach (DaemonThread *thread, m_daemonThreads) {
        if (thread->getConfig()->m_aliasId == aliasId && thread->getConfig()->m_cataloguePath == path ) {
            thread->stopThread();
            delete thread;
            m_daemonThreads.removeOne(thread); // disconnect this from the list
            break;
        }
    }
}

void DaemonApplication::onStarted(DaemonThread *dt)
{
    qDebug() << "DaemonThread started successful for alias: " << dt->getConfig()->m_aliasId;
    qDebug() << " with catalog" << dt->getConfig()->m_cataloguePath;
}

void DaemonApplication::onStartingError(DaemonThread *dt)
{
    qDebug() << "Error while DaemonThread tries connecting for alias: " << dt->getConfig()->m_aliasId;
    qDebug() << " with catalog" << dt->getConfig()->m_cataloguePath;
    // TODO usunięcie tego demona
    emit onThreadClosedSingal(dt); // no nie wiem jeszcze co tam będzie się dziać - sprawdź najpierw tam wywoływaną metodę removeCatalogueFromAlias
}

void DaemonApplication::onClosed(DaemonThread *dt)
{
    qDebug() << "Server closed connection with DaemonThread " << dt->getConfig()->m_aliasId
             << " with catalog: "<< dt->getConfig()->m_cataloguePath;

    emit onThreadClosedSingal(dt);
    //QTimer::singleShot(10, this, SLOT(onDaemonThreadClosedSlot())); // can't passing arguments by it
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

    // Closing DaemonApplication when last DaemonThread closed
    if (m_daemonThreads.isEmpty()) {
        stopApplication();
    }

}

void DaemonApplication::onThreadClosedSlot(DaemonThread *dt)
{
    //removeCatalogueFromAlias(dt->getConfig()->m_cataloguePath ,dt->getConfig()->m_aliasId);
    detachDaemonThread(dt);

    // Closing DaemonApplication when last DaemonThread closed
     if (m_daemonThreads.isEmpty()) {
         stopApplication();
     }
}

QtSingleCoreApplication* DaemonApplication::getSingleApplicationPointer()
{
    return &m_singleApplication;
}

void signal_handler(int sig)
{
    qDebug() << " Signal_handler";

    DaemonApplication::getInstance().stopApplication();

    // stop DaemonApplication's event loop
    //QTimer::singleShot(0, (DaemonApplication::getInstance().getSingleApplicationPointer()), SLOT(quit()));
}

void DaemonApplication::initDaemon(int argc, char **argv)
{
    signal(SIGINT, Daemon::signal_handler);
    DaemonApplication::argc = argc;
    DaemonApplication::argv = argv;
}

} //namespace Daemon
} //namespace TIN_project
