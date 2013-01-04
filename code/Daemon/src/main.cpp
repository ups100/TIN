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
    signal(SIGINT, signal_handler);

    return TIN_project::Daemon::DaemonApplication::getInstance().start(argc, argv);
}
