#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <../include/CommandParser.h>
#include <../include/Commands.h>
using namespace TIN_project::Client;
int main(int argc, char **argv)
{

    QCoreApplication app(argc, argv);
    CommandParser tmpCom;

    //FOR TESTS
    tmpCom.parseCommand(QString("synch -o"));
    Commands tmp(QString("nowy"),QString("hello"),QStringList("moto"));

    return app.exec();
}
