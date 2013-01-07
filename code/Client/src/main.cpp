#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QVariant>
#include <QNetworkInterface>
#include <QList>
#include <QDate>
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
    QString tmp7 = "/home/marta/cos";
    QString tmp4 = "/home/marta/cos/cos2";
    QString tmp5 = "/home/marcin/nowy";
    QString tmp6 = "/home/marta/cos2/cos3";



    afl.addFile(tmp,"2",3);
    afl.addFile(tmp2,"3", 4);
    afl.addFile(tmp3,"2",3);
    afl.addFile(tmp4,"3",5);

    afl.addFile(tmp5,"4",6);
    afl.addFile(tmp5,"5",4);
    afl.addFile(tmp6, "5", 6);
    afl.addFile(tmp6,"5",5);

    int i = 1;
    //afl.str(i);

    ClientApplication app(argc,argv);

    //app.showList(afl,false);
    app.synchWithOverWriting(afl);
    //app.invokeCommandByIndex(afl,9,"choose");
    return app.start(QHostAddress::LocalHost, 8080);
    //return 0;
}
