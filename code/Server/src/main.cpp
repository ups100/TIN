#include "MainServer.h"

int main(int argc, char **argv)
{
    TIN_project::Server::MainServer server(argc, argv);

    return server.start(QHostAddress::LocalHost, 8080);
}
