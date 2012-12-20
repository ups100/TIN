///////////////////////////////////////////////////////////
//  DaemonCommunication.cpp
//  Implementation of the Class DaemonCommunication
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonCommunication.h"
#include "Message.h"
#include "InterprocessName.h"

namespace TIN_project {
namespace Client {

/**
 * C-tor
 */
DaemonCommunication::DaemonCommunication()
{

}

/**
 * D-tor
 */
DaemonCommunication::~DaemonCommunication()
{

}

/**
 * @brief Open socket and connect to daemon.
 * Send some message to daemon
 * @note If connection fails, start DaemonApplication
 * @param message Message to send
 * @throw exception if no daemon listening for a message
 */
void DaemonCommunication::talkToDaemon(Utilities::Message message)
{
    int m_socket;
    struct sockaddr_un m_server;

    // Need to convert into another representation than .data() due to BSD socket, HEX chosen
    QByteArray array = message.toQByteArray().toHex();

    m_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    // Set socket data
    m_server.sun_family = AF_UNIX;
    strcpy(m_server.sun_path, Utilities::InterprocessName::INTERPROCESS_NAME);

    // Give client 10 chances to run daemon application
    for (int tries = 0; tries < 10; ++tries) {

        if (connect(m_socket, (struct sockaddr *) &m_server,
                sizeof(struct sockaddr_un)) < 0) {
            system("echo \"I'm starting daemon application..Wait for a while...\"");
            system("gnome-terminal -x bash -c \"./daemon; bash\""); // TODO run in the background
            // system("./daemon&"); // TODO make alias for daemon while installation
            sleep(1); // Wait to start daemon app
        } else {
            break;
        }

        if (tries == 9)
            throw "Client unable to connect. Couldn't connect(run) to DaemonApplication.";
    }

    if (write(m_socket, array.data(), array.size()) < 0)
        throw "Client writing on stream socket error.";

    close(m_socket);
}

} //namespace Client
} //namespace TIN_project
