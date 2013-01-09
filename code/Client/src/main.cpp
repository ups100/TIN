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
    QString tmp1 = "/home/marcin";
    QString tmp2 = "/home/marta";
    QString tmp3 = "/home/marta/cos";
    QString tmp4 = "/home/marta/cos";
    QString tmp5 = "/home/marcin/nowy";
    QString tmp6 = "/home/marta/cos/cos2";
    QString tmp7 = "/home/marta/costm/cos3";

    //KATALOGI
    afl.addFileWithId(tmp1, "2", 3,Identify::getMachineIdentificator());
    afl.addFileWithId(tmp2, "3", 4, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp3, "2", 3, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp4, "3", 5, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp5, "4", 6, Identify::getMachineIdentificator());

    //PLIKI
    afl.addFileWithId(tmp6, "5", 4, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp7, "5", 6, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp7, "7", 8,"EAAC::44545::F4G4");
    afl.addFileWithId(tmp7, "6", 8,Identify::getMachineIdentificator());

    int i = 1;

    ClientApplication app(argc, argv);

    //app.showList(afl);
    //app.showListOfRemote(afl);
    //app.synchWithOverWriting(afl);
    app.showListOfConflicts(afl);
    //app.invokeCommandByIndex(afl, "1", "pull");
    //app.start(QHostAddress("127.0.0.1"), 8080);

    return 0;
}
