/**
 * @file ClientCommunication.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Daemon::ClientCommunication
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_)
#define EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/unistd.h>
#include <QThread>

namespace TIN_project {
namespace Utilities {
class Message;
}

namespace Daemon {
class DaemonApplication;

/**
 * Class responsible for communication between client and daemon app
 *
 * @author Kajo0
 */
class ClientCommunication : public QThread
{
    /** Socket descriptor */
    int m_socket;

    /** Server addr things */
    struct sockaddr_un m_server;

    /** DaemonApplication handler */
    DaemonApplication &m_daemonApplication;

public:

    /**
     * @brief C-tor
     */
    ClientCommunication(DaemonApplication &daemonApplication);

    /**
     * @brief D-tor, Close socket and unlink socket name
     */
    virtual ~ClientCommunication();

    /**
     * @brief Open socket, bind it and start listening on it
     */
    void run();

    /**
     * @brief Standard BSD socket listening for a message
     * When receive message, convert it into proper object and invoke appropriate action
     * sending signal to the DaemonApplication
     * @throw exception when some socket error came in
     */
    void waitForMessage();
};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_)
