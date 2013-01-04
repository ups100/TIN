#include "DaemonApplication.h"
#include <signal.h>

#include "qtsinglecoreapplication.h"

TIN_project::Daemon::DaemonApplication *app;

// TODO for a while while non QApp
void signal_handler(int sig)
{
    qDebug()<<"Signla_handler";
    app->~DaemonApplication();
    exit(0);
}


int main(int argc, char **argv)
{
    QtSingleCoreApplication a(argc, argv);

    // Check if it is first instance of application
    if (a.isRunning()) {
        qDebug() << "Another instance of daemon is now running";
        return -1;
    }

    signal(SIGINT, signal_handler);

    TIN_project::Daemon::DaemonApplication daemon(&a);
    app = &daemon;
    return daemon.start();

    //return a.exec();
}
