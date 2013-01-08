#include "DaemonApplication.h"

int main(int argc, char **argv)
{
    TIN_project::Daemon::DaemonApplication::initDaemon(argc, argv);

    return TIN_project::Daemon::DaemonApplication::getInstance().start();
}
