#include "DaemonApplication.h"

int main(int argc, char **argv)
{
    TIN_project::Daemon::DaemonApplication daemon;
    daemon.start();

    return 0;
}
