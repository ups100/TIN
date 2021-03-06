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
        : m_singleApplication(argc, argv), //argc,argv are static fields set by initDaemon() method
        m_isClean(true)
{
    qRegisterMetaType<DaemonThread*>("DaemonThread*");
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

    foreach (boost::shared_ptr<DaemonConfiguration::Config> cnf, m_config.getConfigs()){
    qDebug() << "Tworze watek DaemonThread" << cnf->m_cataloguePath; // TODO delete this line
    DaemonThread *dt = new DaemonThread(cnf);
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
                    password.getHash(), path));

    /*
     * Check overlap duplicate
     */
    QRegExp regex(QString("^(") + QRegExp::escape(path) + ")");

    QList<DaemonThread*>::iterator it = m_daemonThreads.begin();
    while (it != m_daemonThreads.end()) {
        boost::shared_ptr<DaemonConfiguration::Config> cnf = (*it)->getConfig();

        if (cnf->m_aliasId == aliasId
                && cnf->m_cataloguePath.length() != path.length()) {
            QRegExp innerRegex(
                    QString("^(") + QRegExp::escape(cnf->m_cataloguePath)
                            + ")");

            // If outer overlapping (path in m_cataloguePath => path shorter than m_cataloguePath)
            if (regex.indexIn(cnf->m_cataloguePath) != -1) {
                // Stop daemon and change his path into bigger one
                (*it)->stopThread();
                QList<DaemonThread*>::iterator iter = m_daemonThreads.erase(it);
                delete *it;
                it = iter;

                m_config.removeConfig(cnf->m_aliasId, cnf->m_cataloguePath);

            } // Else if inner overlapping (m_cataloguePath in path => path longer than m_cataloguePath)
            else if (innerRegex.indexIn(path) != -1) {
                // Only one possibility
                // do nothing, catalogue already in
                return;
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    if (m_config.addConfig(config)) {
        DaemonThread *dt = new DaemonThread(config);
        m_daemonThreads.append(dt);
    }
}

void DaemonApplication::removeCatalogueFromAlias(const QString &path,
        const QString &aliasId)
{
    QMutexLocker lock(&m_mutex); // synchronization // TODO

    if (m_config.removeConfig(aliasId, path)) {
        foreach (DaemonThread* thread, m_daemonThreads){
        if (thread->getConfig()->m_aliasId == aliasId && thread->getConfig()->m_cataloguePath == path) {
            thread->stopThread(); // TODO check if no thread etc
            m_daemonThreads.removeOne(thread);// disconnect this from the list
            delete thread;// TODO WAIT FOR STOP THREAD!!!
            break;
        }
    }
}
}

void DaemonApplication::detachDaemonThread(DaemonThread *dt)
{
    QMutexLocker lock(&m_mutex); // synchronization

    if (m_daemonThreads.size() == 0)
        return;

    QString aliasId(dt->getConfig()->m_aliasId);
    QString path(dt->getConfig()->m_cataloguePath);

    foreach (DaemonThread *thread, m_daemonThreads){
    if (thread->getConfig()->m_aliasId == aliasId && thread->getConfig()->m_cataloguePath == path ) {
        thread->stopThread();
        m_daemonThreads.removeOne(thread); // disconnect this from the list
        delete thread;// TODO WAIT FOR STOP THREAD!!!
        break;
    }
}
}

void DaemonApplication::onStarted(DaemonThread *dt)
{
    qDebug() << "DaemonThread started successful for alias: "
            << dt->getConfig()->m_aliasId << " with catalog"
            << dt->getConfig()->m_cataloguePath;
}

void DaemonApplication::onStartingError(DaemonThread *dt)
{
    qDebug() << "Error while DaemonThread tries connecting for alias: "
            << dt->getConfig()->m_aliasId << " with catalog"
            << dt->getConfig()->m_cataloguePath;

    // even if error occurs I don't remove this DeamonThread from config file
    QMetaObject::invokeMethod(this, "onThreadClosedSlot", Qt::QueuedConnection,
            Q_ARG(DaemonThread*, dt));
        }

void DaemonApplication::onClosed(DaemonThread *dt)
{
    qDebug() << "Server closed connection with DaemonThread "
            << dt->getConfig()->m_aliasId << " with catalog: "
            << dt->getConfig()->m_cataloguePath;

    QMetaObject::invokeMethod(this, "onThreadClosedSlot", Qt::QueuedConnection,
            Q_ARG(DaemonThread*, dt));
        }

void DaemonApplication::removeAfterServerConnectionFails(DaemonThread *dt)
{
    qDebug()
            << "Server doesn't response for connection tries with DaemonThread "
            << dt->getConfig()->m_aliasId << " with catalog: "
            << dt->getConfig()->m_cataloguePath;

    detachDaemonThread(dt);
}

void DaemonApplication::onThreadClosedSlot(DaemonThread *dt)
{
    //removeCatalogueFromAlias(dt->getConfig()->m_cataloguePath ,dt->getConfig()->m_aliasId);
    detachDaemonThread(dt);

    // Closing DaemonApplication when last DaemonThread closed
    if (m_daemonThreads.isEmpty()) {
//        stopApplication(); // TODO remember that
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
