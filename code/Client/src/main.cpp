#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QVariant>
#include <../include/CommandParser.h>
#include <../include/Commands.h>
#include <../include/Argument.h>
#include <../include/ClientView.h>
using namespace TIN_project::Client;
int main(int argc, char **argv)
{

    QCoreApplication app(argc, argv);
    //CommandParser tmpCom;

    //tmpCom.parseCommand(QString("add code/Client"));

    //boost::shared_ptr<Commands> tmp = tmpCom.parseCommand(QString("add code/Client"));
    //tmp->getArg(Argument::FILELOCATION);

    ClientView client;
    client.start();
    return app.exec();
}
