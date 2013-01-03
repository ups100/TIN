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
#include <../include/CommandParser.h>
#include <../include/Commands.h>
#include <../include/Argument.h>
#include <../include/ClientApplication.h>
using namespace TIN_project::Client;
int main(int argc, char **argv)
{

    //QCoreApplication app(argc, argv);
    //CommandParser tmpCom;

    //tmpCom.parseCommand(QString("add code/Client"));

    //boost::shared_ptr<Commands> tmp = tmpCom.parseCommand(QString("add code/Client"));
    //tmp->getArg(Argument::FILELOCATION);

    ClientApplication app(argc,argv);
    QNetworkInterface *inter=new QNetworkInterface();
    QList<QHostAddress> list = inter->allAddresses();
    return app.start(list.first(), 8080);
}
