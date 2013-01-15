/**
 * @file Server/src/main.cpp
 *
 * @date 22-12-2012
 *
 * @author Marcin Kubik <markubik@gmail.com>
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 * @author Jacek Sosnowski <jacek5656@tlen.pl>
 *
 * @brief TIN_project::Server module main function
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "MainServer.h"

int main(int argc, char **argv)
{
    QHostAddress addr = QHostAddress::LocalHost;
    quint16 port = 8080;

    if (argc > 1) {
        QHostAddress address;
        // below return true if addres is correct
        if ( address.setAddress(argv[1]))
            addr = address;
        else
            qDebug() << "Bad IP address. Type something like 127.0.0.1";
    }

    if (argc > 2) {
        quint16 givenPort = atoi(argv[2]);

        if (givenPort >= 1024 || givenPort <= 65535)
            port = givenPort;
        else
            qDebug() << "Bad port address. Type number (1024 - 65535)";

    }

    TIN_project::Server::MainServer server(argc, argv);
    return server.start(addr, port);
}
