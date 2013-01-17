/**
 * @file Daemon/src/main.cpp
 *
 * @date 22-12-2012
 *
 * @author Marcin Kubik <markubik@gmail.com>
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 * @author Jacek Sosnowski <jacek5656@tlen.pl>
 *
 * @brief TIN_project::Daemon module main function
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "DaemonApplication.h"

int main(int argc, char **argv)
{
    TIN_project::Daemon::DaemonApplication::initDaemon(argc, argv);

    return TIN_project::Daemon::DaemonApplication::getInstance().start();
}
