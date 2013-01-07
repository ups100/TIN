/**
 * @file FileLocation.cpp
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::FileLocation
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "FileLocation.h"
#include <QDataStream>
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

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
