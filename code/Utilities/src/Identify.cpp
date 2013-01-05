/**
 * @file Identify
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::Identify
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "Identify.h"
#include <QNetworkInterface>

namespace TIN_project {
namespace Utilities {

Identify::Identify()
{

}

Identify::Identify(const Identify &other)
{

}

Identify& Identify::operator=(const Identify &other)
{
    return *this;
}

Identify::~Identify()
{

}

QString Identify::getMachineIdentificator()
{
    static QString id;

    if (id.size())
        return id;

    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces()){
    // Return only the first non-loopback MAC Address
    if (!(interface.flags() & QNetworkInterface::IsLoopBack))
    id = interface.hardwareAddress();
}

    return id;
}

} //namespace Utilities
} //namespace TIN_project
