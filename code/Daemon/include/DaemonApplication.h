///////////////////////////////////////////////////////////
//  DaemonApplication.h
//  Implementation of the Class DaemonApplication
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_)
#define EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_
#include <QList>

#include "ClientCommunication.h"
#include "DaemonThread.h"

namespace TIN_project {
namespace Daemon {

class DaemonApplication
{

public:
    DaemonApplication();
    virtual ~DaemonApplication();

    void start();

private:
    QList<DaemonThread> m_daemonThreads;
    ClientCommunication m_clientCommunication;

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_)
