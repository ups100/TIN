#include <QCoreApplication>
#include <QFile>

#include "FileReciver.h"
#include "FileSender.h"

using namespace TIN_project::Daemon;
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    FileSender sender(0L, new QFile("Nad.Zycie.2012.rar"));
    FileReciver rec(0L, new QFile("Nad.Zycie.2013.rar"), QFile("Nad.Zycie.2012.rar").size() );

    sender.connectToServer(QHostAddress("192.168.1.5"), 8080);
    rec.connectToServer(QHostAddress("192.168.1.5"), 8080);
    return app.exec();
}
