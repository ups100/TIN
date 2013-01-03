///////////////////////////////////////////////////////////
//  Message.cpp
//  Implementation of the Class Message
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

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
