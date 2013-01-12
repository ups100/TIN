/**
 * @file DaemonCommunication.cpp
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Client::DaemonCommunication
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "DaemonCommunication.h"
#include "Message.h"
#include "InterprocessName.h"
#include <stdexcept>

namespace TIN_project {
namespace Client {

DaemonCommunication::DaemonCommunication()
{

}

DaemonCommunication::~DaemonCommunication()
{

}

void DaemonCommunication::talkToDaemon(const QByteArray &message)
{
    int m_socket;
    struct sockaddr_un m_server;

    // Need to convert into another representation than .data() due to BSD socket, HEX chosen
    QByteArray array = message.toHex();

    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    // Set socket data
    m_server.sun_family = AF_UNIX;
    strcpy(m_server.sun_path, Utilities::InterprocessName::INTERPROCESS_NAME);

    // Give client 10 chances to run daemon application
    for (int tries = 0; tries < 10; ++tries) {

        if (connect(m_socket, (struct sockaddr *) &m_server,
                sizeof(struct sockaddr_un)) < 0) {
            system(
                    "echo \"I'm starting daemon application..Wait for a while...\"");
            system("gnome-terminal -x bash -c \"./daemon; bash\""); // TODO run in the background
            //system("./daemon&"); // TODO make alias for daemon while installation
            sleep(1); // Wait to start daemon app
        } else {
            break;
        }

        if (tries == 9)
            throw std::runtime_error(
                    "Client unable to connect. Couldn't connect(run) to DaemonApplication.");
    }

    if (write(m_socket, array.data(), array.size()) < 0)
        throw std::runtime_error("Client writing on stream socket error.");

    close(m_socket);
}

} //namespace Client
} //namespace TIN_project
