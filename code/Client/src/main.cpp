#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QVariant>
#include <QNetworkInterface>
#include <QList>
#include <QHostAddress>
#include "CommandParser.h"
#include "Commands.h"
#include "Argument.h"
#include "ClientApplication.h"
#include "AliasTree.h"
#include "AliasFileList.h"
#include <iostream>
using namespace TIN_project::Client;
int main(int argc, char **argv)
{

    AliasFileList afl;
    QString tmp = "/home/marcin";
    QString tmp2 = "/home/marta";
    QString tmp3 = "/home/marta/cos";
    QString tmp4 = "/home/marta/cos/cos2";
    QString tmp5 = "/home/marcin/nowy";
    QString tmp6 = "/users/home/marcin/nowy2";

    afl.addFile(tmp,"2",3);
    afl.addFile(tmp2,"3", 4);
    afl.addFile(tmp3,"2",3);
    afl.addFile(tmp4,"3",5);
    afl.addFile(tmp5,"4",6);
    afl.addFile(tmp5,"5",7);
    //afl.addFile(tmp6,"5",6);

    afl.str();
    ClientApplication app(argc,argv);
    QNetworkInterface *inter=new QNetworkInterface();
    QList<QHostAddress> list = inter->allAddresses();
    return app.start(list.first(), 8080);
    return 0;
}
