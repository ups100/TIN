///////////////////////////////////////////////////////////
//  DaemonApplication.cpp
//  Implementation of the Class DaemonApplication
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonApplication.h"

namespace TIN_project {
namespace Daemon {

DaemonApplication::DaemonApplication()
{

}

DaemonApplication::~DaemonApplication()
{

}

void DaemonApplication::start()
{
    //TODO run in own thread
    m_clientCommunication.waitForMessage();
}

} //namespace Daemon
} //namespace TIN_project
