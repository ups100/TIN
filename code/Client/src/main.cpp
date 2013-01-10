#include "ClientApplication.h"
#include <QDebug>

using namespace TIN_project;
using namespace std;

int main(int argc, char **argv)
{
    TIN_project::Client::ClientApplication client;
    client.start();

    return 0;
}
