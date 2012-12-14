///////////////////////////////////////////////////////////
//  DaemonCommunication.cpp
//  Implementation of the Class DaemonCommunication
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonCommunication.h"
#include "Message.h"
#include "InterprocessName.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/unistd.h>
#include <stdio.h>

namespace TIN_project {
namespace Client {

DaemonCommunication::DaemonCommunication()
{

}

DaemonCommunication::~DaemonCommunication()
{

}

/**
 * @brief Send some message to daemon
 * @param message Message to send
 * @throw exception if no daemon listening for a message
 */
void DaemonCommunication::talkToDaemon(Utilities::Message message)
{
    int sock;
    struct sockaddr_un server;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock < 0)
        throw "Client socket opening failed.";

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, Utilities::InterprocessName::INTERPROCESS_NAME);

    if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))
            < 0) {
        close(sock);

        throw "Client unable to connect.";
    }

    // Need to convert into another representation than .data() due to BSD socket, HEX chosen
    QByteArray array = message.toQByteArray().toHex();

    if (write(sock, array.data(), array.size()) < 0)
        throw "Client writing on stream socket error.";

    close(sock);
}

} //namespace Client
} //namespace TIN_project
