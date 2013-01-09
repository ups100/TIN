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

FileLocation::FileLocation(const QString &path, const quint32 &size,
        const Identifier &ownerId)
        : m_path(path), m_size(size), m_ownerIdentifier(ownerId)
{

}

FileLocation::FileLocation(const QByteArray& data)
        : m_size(0)
{
    QDataStream in(data);

    in >> m_path;
    in >> m_size;
    in >> m_ownerIdentifier;
}

FileLocation::~FileLocation()
{

}

QString FileLocation::getPath()
{
    return m_path;
}

quint32 FileLocation::getSize()
{
    return m_size;
}

Identifier FileLocation::getOwnerIdentifier()
{
    return m_ownerIdentifier;
}

QByteArray FileLocation::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << m_path;
    out << m_size;
    out << m_ownerIdentifier;

    return bytes;
}

} //namespace Utilities
} //namespace TIN_project
