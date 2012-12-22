#include "DaemonApplication.h"
#include <signal.h>

TIN_project::Daemon::DaemonApplication *app;

// TODO for a while while non QApp
void signal_handler(int sig)
{
    app->~DaemonApplication();
    exit(0);
}


int main(int argc, char **argv)
{
    signal(SIGINT, signal_handler);

    TIN_project::Daemon::DaemonApplication daemon;
    app = &daemon;
    daemon.start();

    return 0;
}
