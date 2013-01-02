#include "MainServer.h"
#include <QTimer>
int main(int argc, char **argv)
{
    TIN_project::Server::MainServer server(argc, argv);

    //temporary just closing server test, remove it later
    QTimer::singleShot(10000, &server, SLOT(stopServer()));
    return server.start(QHostAddress::LocalHost, 8080);
}
