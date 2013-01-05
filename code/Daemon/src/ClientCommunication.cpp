/**
 * @file ClientCommunication.cpp
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

#include "ClientCommunication.h"
#include "Message.h"
#include "InterprocessName.h"
#include "DaemonApplication.h"
#include "CommunicationProtocol.h"
#include <stdexcept>

namespace TIN_project {
namespace Daemon {

ClientCommunication::ClientCommunication(
        DaemonApplication &daemonApplication)
        : m_socket(-1), m_daemonApplication(daemonApplication)
{

}

ClientCommunication::~ClientCommunication()
{
    if (m_socket >= 0)
        close(m_socket);

    unlink(Utilities::InterprocessName::INTERPROCESS_NAME);
}

void ClientCommunication::run()
{
    // Unlink old socket if hasn't been closed properly
    unlink(Utilities::InterprocessName::INTERPROCESS_NAME);

    // Open socket
    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (m_socket < 0)
        throw std::runtime_error("Daemon opening stream socket error.");

    // Set socket data
    m_server.sun_family = AF_UNIX;
    strcpy(m_server.sun_path, Utilities::InterprocessName::INTERPROCESS_NAME);

    if (bind(m_socket, (struct sockaddr *) &m_server,
            sizeof(struct sockaddr_un)))
        throw std::runtime_error("Daemon binding stream socket error.");

    waitForMessage();
}

void ClientCommunication::waitForMessage()
{
    int msgsock;
    int rval;

    listen(m_socket, 5);

    while (1) {
        msgsock = accept(m_socket, 0, 0);

        if (msgsock == -1) {
            throw std::runtime_error("Daemon accept error.");
        } else {
            // For a message
            QByteArray array;
            char buf[1024];

            do {
                bzero(buf, sizeof(buf));

                if ((rval = read(msgsock, buf, 1024)) < 0) {
                    throw std::runtime_error("Daemon reading stream message error.");
                } else if (rval == 0) {} // ending connection
                else {
                    array.append(buf);
                }
            } while (rval > 0);

            // Decode message, which has been sent in HEX format to avoid Qt-BSD problems
            QByteArray bytes = QByteArray::fromHex(array.data());

            m_daemonApplication.dispatchMessage(bytes);
        }

        close(msgsock);
    }
}

} //namespace Daemon
} //namespace TIN_project
