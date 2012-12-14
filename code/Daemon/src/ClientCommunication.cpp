///////////////////////////////////////////////////////////
//  ClientCommunication.cpp
//  Implementation of the Class ClientCommunication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientCommunication.h"
#include "Message.h"
#include "InterprocessName.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>

namespace TIN_project {
namespace Daemon {

ClientCommunication::ClientCommunication()
{
    // Unlink old socket if hasn't been closed properly
    unlink(Utilities::InterprocessName::INTERPROCESS_NAME);
}

ClientCommunication::~ClientCommunication()
{
    unlink(Utilities::InterprocessName::INTERPROCESS_NAME);
}

/**
 * @brief Standard BSD socket listening for a message
 * When receive message, convert it into proper object and invoke appropriate action
 * sending signal to the DaemonApplication
 * @throw exception when some socket error came in
 */
void ClientCommunication::waitForMessage()
{
    int sock;
    int msgsock;
    int rval;
    struct sockaddr_un server;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock < 0)
        throw "Daemon opening stream socket error.";

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, Utilities::InterprocessName::INTERPROCESS_NAME);

    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)))
        throw "Daemon binding stream socket error.";

    listen(sock, 5);

    while (1) {
        msgsock = accept(sock, 0, 0);

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
            Utilities::Message m(QByteArray::fromHex(array.data()));
            // TODO send signal to daemon application
            std::cout<<m.message().toStdString().c_str()<<"\n";// TODO remove
        }

        close(msgsock);
    }

    // Close socket and release name
    close(sock);
}

boost::shared_ptr<Utilities::Message> ClientCommunication::getMessage()
{
    return boost::shared_ptr<Utilities::Message>((Utilities::Message*) 0L);
}

} //namespace Daemon
} //namespace TIN_project
