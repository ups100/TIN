///////////////////////////////////////////////////////////
//  FileLocation.cpp
//  Implementation of the Class FileLocation
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "FileLocation.h"

namespace TIN_project {
namespace Utilities {

FileLocation::FileLocation(const QString &path, const QString &ownerId)
        : m_path(path), m_ownerId(ownerId)
{

}

FileLocation::FileLocation(const QByteArray& data)
{

}

FileLocation::~FileLocation()
{

}

QString FileLocation::getPath() {
    return m_path;
}

QString FileLocation::getOwnerId() {
    return m_ownerId;
}

QByteArray FileLocation::toQByteArray()
{
    return QByteArray();
}

} //namespace Utilities
} //namespace TIN_project
