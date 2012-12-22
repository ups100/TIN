///////////////////////////////////////////////////////////
//  DaemonApplication.cpp
//  Implementation of the Class DaemonApplication
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonApplication.h"
#include <QDebug>

namespace TIN_project {
namespace Daemon {

DaemonApplication::DaemonApplication()
        : m_clientCommunication(this)
{

}

DaemonApplication::~DaemonApplication()
{
    m_clientCommunication.terminate(); // TODO kopasiak check that: exit / terminate / quit / leave it alone
    m_clientCommunication.wait();
}

int DaemonApplication::start()
{
    // Run listener for local client
    m_clientCommunication.start();

    // TODO remove demo loop
    qDebug() << "Waiting 4 a message";
    while (1)
    {
        qDebug() << ".";
        sleep(1);
    }

    return 0;
}

// TODO dispatch message to do what is needed
void DaemonApplication::dispatchMessage(const Utilities::Message &message) const
{
    qDebug() << message.message();
    qDebug() << "Waiting 4 a message";
}

} //namespace Daemon
} //namespace TIN_project
