/**
 * @file Server/src/main.cpp
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
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
    TIN_project::Server::MainServer server(argc, argv);

    return server.start(QHostAddress::LocalHost, 8080);
}
