#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QString>

//for tests
#include <QTcpServer>
#include <QTimer>
#include "ServerConnection.h"

using namespace std;

int main(int argc, char **argv)
{
    /*QTcpServer server;
    server.listen(QHostAddress::LocalHost, 7777);
    TIN_project::Client::ServerConnection con(0L,0L);

    con.connectToServer(QHostAddress::LocalHost, 7777);

    QCoreApplication app(argc, argv);
    QTimer::singleShot(10000, &app, SLOT(quit()));
    return app.exec();*/
    QString str("abrakadabra");

    qDebug()<<str;
    QByteArray arr;
    QDataStream stream;

    stream<< str;


    QString str2;
    stream >> str2;

    qDebug() << arr.size();
    qDebug() << arr.data();
    qDebug()<< str2;
}
