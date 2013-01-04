#include "DaemonApplication.h"
#include <signal.h>

#include "qtsinglecoreapplication.h"

//TIN_project::Daemon::DaemonApplication *app;

// TODO for a while while non QApp
void signal_handler(int sig)
{
    qDebug()<<"Signla_handler";
    //app->~DaemonApplication(); // przy Singleton'ie nie działa - czy mogę wywalić Destruktor do private?
    TIN_project::Daemon::DaemonApplication::getInstance().stopApplication();
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

    TIN_project::Daemon::DaemonApplication &daemon = TIN_project::Daemon::DaemonApplication::getInstance();
    //app = &daemon; // już niepotrzebne bo jest Singleton
    daemon.start();

    return a.exec();
}
