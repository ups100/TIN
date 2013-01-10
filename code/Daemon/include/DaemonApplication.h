/**
 * @file DaemonApplication.h
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
#include <QMutexLocker>
#include <QTimer>

#include "DaemonThreadListener.h"
#include "qtsinglecoreapplication.h"
#include <signal.h>

namespace TIN_project {
namespace Daemon {

class DaemonApplication: public QObject, public DaemonThreadListener
{
    Q_OBJECT;
public:

    virtual ~DaemonApplication();

    int start();
    /**
     * @brief Call this method instead of ~DaemonApplication
     * @details This close application at all.
     */
    void stopApplication();
    /**
     * @brief Dispatch received message
     * @param communicate Received communicate as QByteArray
     */
    void dispatchMessage(const QByteArray &communicate);

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

    /**
     * @brief Similar to removeCatalogueFromAlias but this method don't use DaemonThread info from config.
     * @brief This method take pointer and disconnect, remove and unlink from m_daemonThreads lists.
     * @details It is call when application is ended (then all DeamonThreads are detached one by another.
     */
    void detachDaemonThread(DaemonThread *dt);

    /**
     * @brief Provide the instance of DaemonApplication object
     * @details There could by only one such object - this method keep an eye on it.
     * @return DaemonApplication class current instance.
     */
    static DaemonApplication& getInstance();


    /**
     * @brief Simple geter which is used in SIGKILL handler when SingleShot is sending
     */
    QtSingleCoreApplication* getSingleApplicationPointer();

    /**
     * @brief Static method which should by invoke before DaemonApplication::start()
     * @details It provide a arguments to the QtSimpleCoreAppplication before it is create
     * and this method install UNIX signal SIGKILL handler
     */
    static void initDaemon(int argc, char **argv);

    virtual void onStarted(DaemonThread *dt);
    virtual void onStartingError(DaemonThread *dt);
    virtual void onClosed(DaemonThread *dt);

private:
    /**
     * @brief Private class constructors - because of Singleton
     * @details Use DaemonApplication::getInstance() method
     */
    DaemonApplication();
    DaemonApplication(const DaemonApplication &);
    /**
     * @brief Makes first instance of object this class.
     * @details It should be call only from getInstance
     */
    static DaemonApplication* makeInstance();

private slots:
    /** Do not use this slot. Use another one below. */
    void onDaemonThreadClosedSlot();
    void onThreadClosedSlot(DaemonThread *dt);

private:

    QList<DaemonThread*> m_daemonThreads;

    /** Client communication thread filed */
    ClientCommunication *m_clientCommunication;

    /** Daemon threads configuration */
    DaemonConfiguration m_config;

    /**
     * @brief Singleton implementation.
     */
    static DaemonApplication *instance;

    /**
     * @brief Mutex which lock singletron's function
     */
    static QMutex m_mutex;

    /**
     * @brief This variable is used in start and stop method to show destructor to clean behind this object
     */
    bool m_isClean;

    /**
      * @brief Public pointer to object which provide event loop
      */
     QtSingleCoreApplication m_singleApplication;

    static int argc;
    static char **argv;
};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_)
