#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QString>
#include <QHostAddress>

//for tests
#include <QTcpServer>
#include <QTimer>
#include "ServerConnection.h"

using namespace std;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    TIN_project::Client::ServerConnection con(0L,0L);
    qint16 port = 8080;
    con.connectToServer(QHostAddress::LocalHost, port);
    return app.exec();
}

