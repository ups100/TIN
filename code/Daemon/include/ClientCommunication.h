///////////////////////////////////////////////////////////
//  ClientCommunication.h
//  Implementation of the Class ClientCommunication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

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

class ClientCommunication : public QThread
{
    /** Socket descriptor */
    int m_socket;

    /** Server addr things */
    struct sockaddr_un m_server;

    /** DaemonApplication handler */
    const DaemonApplication *m_daemonApplication;

public:
    ClientCommunication(const DaemonApplication *daemonApplication);
    virtual ~ClientCommunication();

    void run();
    void waitForMessage();
};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_)
