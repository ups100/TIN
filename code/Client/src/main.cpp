/**
 * @file Client/src/main.cpp
 *
 * @date 22-12-2012
 *
 * @author Marcin Kubik <markubik@gmail.com>
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 * @author Opasiak Krzysztof <ups100@tlen.pl>
 * @author Jacek Sosnowski <jacek5656@tlen.pl>
 *
 * @brief TIN_project::Client module main function
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */


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
#include <string>
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
    /*QFile file("/home/marcin/TIN_testy/.TIN_project");
    file.open(QIODevice::WriteOnly);
    file.write("login");
    file.write("\n");
    file.write(QByteArray("Marta").toHex());
    file.close();
    qDebug()<<"MOMENCIK";
    ClientApplication app(argc, argv);
    qDebug()<<app.checkIntegrityOfConfigFile("/home/marcin/TIN_testy","login", TIN_project::Utilities::Password(QString("Marta")));*/
    /*AliasFileList afl;
    QString tmp1 = "/home/marcin";
    QString tmp2 = "/home/marta";
    QString tmp3 = "/home/marta/cos";
    QString tmp4 = "/home/marta/cos";
    QString tmp5 = "/home/marcin/nowy";
    QString tmp6 = "/home/marta/cos/cos2";
    QString tmp7 = "/home/marta/costm/cos3";

    //KATALOGI
    afl.addFileWithId(tmp1, "2", 3, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp2, "3", 4, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp3, "2", 3, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp4, "3", 5, Identify::getMachineIdentificator());

    //PLIKI
    afl.addFileWithId(tmp5, "4", 6, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp6, "5", 4, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp7, "5", 6, Identify::getMachineIdentificator());
    afl.addFileWithId(tmp7, "4", 8, "EAAC::44545::F4G4");
    afl.addFileWithId(tmp6, "7", 8, "EAAC::44545::F4G4");
    afl.addFileWithId(tmp6, "6", 12, "EAAC::3423:2323");

    int i = 1;
    ClientApplication app(argc, argv);

    //app.showList(afl);
    //app.showListOfLocal(afl);
    //app.showListOfRemote(afl);
    //app.showListOfConflicts(afl);
   // app.synchWithOverWriting(afl);
    //app.showListOfConflicts(afl);
    //app.invokeCommandByIndex(afl, "1", "pull");*/
    ClientApplication app(argc,argv);
    app.start(QHostAddress(argv[1]), atoi(argv[2]),QString(argv[3]));

    //ClientApplication app(argc,argv);
    return 0;
}
