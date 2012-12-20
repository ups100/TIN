///////////////////////////////////////////////////////////
//  FileLocation.cpp
//  Implementation of the Class FileLocation
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "FileLocation.h"
#include <QDataStream>
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

FileLocation::FileLocation()
{

}

FileLocation::FileLocation(const QByteArray &bytes)
{
    QDataStream in(bytes);

    // Flush size
    quint16 size;
    in >> size;

    // TODO implement that
//    in >> ?!;
}

FileLocation::~FileLocation()
{

}

/**
 * @brief Convert (serialize) Message object into QByteArray
 * @return Serialized object as QByteArray with size at the beginning
 */
QByteArray FileLocation::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    // Size
    out << (quint16) 0;

    // TODO implement that
//    out << ?!;

    // Set size
    out.device()->seek(0);
    out << (quint16) (bytes.size() - sizeof(quint16));

    return bytes;
}

} //namespace Utilities
} //namespace TIN_project
