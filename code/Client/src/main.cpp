#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <../include/CommandParser.h>
#include <../include/Commands.h>
#include <../include/Argument.h>
using namespace TIN_project::Client;
int main(int argc, char **argv)
{

    QCoreApplication app(argc, argv);
    CommandParser tmpCom;

    tmpCom.parseCommand(QString("synch -o"));
    //Commands tmp(QString("nowy"),QString("hello"),QStringList("moto"),Argument::FILES);
    FileLocation::checkLocation("code/Client");
    return app.exec();
}
