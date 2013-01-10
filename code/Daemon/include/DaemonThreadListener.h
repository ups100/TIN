/**
 * @file Daemon/include/DaemonThreadListener.h
 *
 * @date 06-01-2013
 *
 * @author Sosnowski Jacek
 *
 * @brief Implementation of the Class TIN_project::Daemon::DaemonThreadListener
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#ifndef DAEMONTHREADLISTENER_H_
#define DAEMONTHREADLISTENER_H_

#include "DaemonThread.h"

namespace TIN_project {
namespace Daemon {

/**
 * @brief This class provide interface which allows DaemonThread send some communicates to DaemonApplication.
 * @details This interface is implemented by DaemonApplication class
 *
 * @warning All this method is calling by DaemonThread objects but I'm not sure in which thread.
 * I suppose this method are called in context of ServerConnection's thread.
 */
class DaemonThreadListener
{
public:
    DaemonThreadListener();
    virtual ~DaemonThreadListener();

    /**
     * @brief This method is calling by DaemonThread when his connection to server and alias was successful
     */
    virtual void onStarted(DaemonThread *dt) = 0;

    /**
     * @brief When DaemoThread encounter difficulties while connecting with server - will call this method.
     */
    virtual void onStartingError(DaemonThread *dt) = 0;

    /**
     * @brief When connection DaemonThread - Server was closed by Server this will be executed by DaemonApplication.
     */
    virtual void onClosed(DaemonThread *dt) = 0;
};

} /* namespace Daemon */
} /* namespace TIN_project */
#endif /* DAEMONTHREADLISTENER_H_ */
