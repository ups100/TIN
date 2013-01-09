/**
 * @file Identifier.cpp
 *
 * @date 08-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::Identifier
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "Identifier.h"
#include <QDataStream>
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

Identifier::Identifier()
{

}

Identifier::Identifier(const QString &path, const QString &id)
        : m_path(path), m_id(id)
{

}

Identifier::Identifier(const QByteArray& data)
{
    QDataStream in(data);

    in >> m_path;
    in >> m_id;
}

Identifier::~Identifier()
{

}

QString Identifier::getPath()
{
    return m_path;
}

QString Identifier::getId()
{
    return m_id;
}

QByteArray Identifier::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << m_path;
    out << m_id;

    return bytes;
}

QDataStream& operator<<(QDataStream &out, const Identifier &identifier)
{
    out << identifier.m_path;
    out << identifier.m_id;

    return out;
}

QDataStream& operator>>(QDataStream &in, Identifier &identifier)
{
    in >> identifier.m_path;
    in >> identifier.m_id;

    return in;
}

} //namespace Utilities
} //namespace TIN_project
