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
    QString str("aaaa");

    qDebug()<<str;
    QByteArray arr;
    QByteArray arr2;
    QDataStream stream(&arr, QIODevice::WriteOnly);

    stream<< str;

    stream.unsetDevice();
    QDataStream stream2(&arr2, QIODevice::WriteOnly);
    stream2<<32.0;

    QByteArray arr3 = arr + arr2;
    QDataStream stream3(arr3);

    double d;
    QString name;
    stream3>>name;
    qDebug()<<name;
}

