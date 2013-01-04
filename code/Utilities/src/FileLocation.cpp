///////////////////////////////////////////////////////////
// FileLocation.cpp
// Implementation of the Class FileLocation
// Created on: 07-gru-2012 00:33:36
// Original author: kopasiak
///////////////////////////////////////////////////////////

#include "FileLocation.h"
#include <QDataStream>
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

FileLocation::FileLocation()
{

}

FileLocation::FileLocation(const QString &path, const QString &ownerId)
        : m_path(path), m_ownerId(ownerId)
{

}

FileLocation::FileLocation(const QByteArray& data)
{
    QDataStream in(data);

    in >> m_path;
    in >> m_ownerId;
}

FileLocation::~FileLocation()
{

}

QString FileLocation::getPath()
{
    return m_path;
}

QString FileLocation::getOwnerId()
{
    return m_ownerId;
}

QByteArray FileLocation::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << m_path;
    out << m_ownerId;

    return bytes;
}

} //namespace Utilities
} //namespace TIN_project
