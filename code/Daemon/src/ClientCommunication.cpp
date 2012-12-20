///////////////////////////////////////////////////////////
//  ClientCommunication.cpp
//  Implementation of the Class ClientCommunication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientCommunication.h"
#include "Message.h"
#include "InterprocessName.h"
#include "DaemonApplication.h"

namespace TIN_project {
namespace Daemon {

/**
 * C-tor
 */
ClientCommunication::ClientCommunication(
        const DaemonApplication *daemonApplication)
        : m_socket(-1), m_daemonApplication(daemonApplication)
{

}

/**
 * D-tor
 * @brief Close socket and unlink socket name
 */
ClientCommunication::~ClientCommunication()
{
    if (m_socket >= 0)
        close(m_socket);

    unlink(Utilities::InterprocessName::INTERPROCESS_NAME);
}

/**
 * @brief Open socket, bind it and start listening on it
 */
void ClientCommunication::run()
{
    // Unlink old socket if hasn't been closed properly
    unlink(Utilities::InterprocessName::INTERPROCESS_NAME);

    // Open socket
    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (m_socket < 0)
        throw "Daemon opening stream socket error.";

    // Set socket data
    m_server.sun_family = AF_UNIX;
    strcpy(m_server.sun_path, Utilities::InterprocessName::INTERPROCESS_NAME);

    if (bind(m_socket, (struct sockaddr *) &m_server,
            sizeof(struct sockaddr_un)))
        throw "Daemon binding stream socket error.";

    waitForMessage();
}

/**
 * @brief Standard BSD socket listening for a message
 * When receive message, convert it into proper object and invoke appropriate action
 * sending signal to the DaemonApplication
 * @throw exception when some socket error came in
 */
void ClientCommunication::waitForMessage()
{
    int msgsock;
    int rval;

    listen(m_socket, 5);

    while (1) {
        msgsock = accept(m_socket, 0, 0);

        if (msgsock == -1) {
            throw "Daemon accept error.";
        } else {
            // For a message
            QByteArray array;
            char buf[1024];

            do {
                bzero(buf, sizeof(buf));

                if ((rval = read(msgsock, buf, 1024)) < 0) {
                    throw "Daemon reading stream message error.";
                } else if (rval == 0) {} // ending connection
                else {
                    array.append(buf);
                }
            } while (rval > 0);

            // Decode message, which has been sent in HEX format to avoid Qt-BSD problems
            Utilities::Message message(QByteArray::fromHex(array.data()));

            m_daemonApplication->dispatchMessage(message);
        }

        close(msgsock);
    }
}

} //namespace Daemon
} //namespace TIN_project
