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

Identifier::Identifier(const QString &id, const QString &path)
        : m_id(id), m_path(path)
{

}

Identifier::Identifier(const QByteArray& data)
{
    QDataStream in(data);

    in >> m_id;
    in >> m_path;
}

Identifier::~Identifier()
{

}

const QString& Identifier::getId() const
{
    return m_id;
}

const QString& Identifier::getPath() const
{
    return m_path;
}

QByteArray Identifier::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << m_id;
    out << m_path;

    return bytes;
}

QDataStream& operator<<(QDataStream &out, const Identifier &identifier)
{
    out << identifier.m_id;
    out << identifier.m_path;

    return out;
}

QDataStream& operator>>(QDataStream &in, Identifier &identifier)
{
    in >> identifier.m_id;
    in >> identifier.m_path;

    return in;
}

} //namespace Utilities
} //namespace TIN_project
