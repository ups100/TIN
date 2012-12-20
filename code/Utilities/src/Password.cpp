///////////////////////////////////////////////////////////
//  Password.cpp
//  Implementation of the Class Password
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "Password.h"
#include <QDataStream>
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

Password::Password()
{

}

Password::Password(const QByteArray &bytes)
{
    QDataStream in(bytes);

    // Flush size
    quint16 size;
    in >> size;

    // TODO implement that
//    in >> ?!;
}

Password::~Password()
{

}

/**
 * Creates an object and set it's password
 */
Password::Password(const QString& password)
{

}

/**
 * Check if password is correct
 */
bool Password::check(const QString& password) const
{

    return false;
}

bool Password::check(const Password& password) const
{

    return false;
}

/**
 * @brief Convert (serialize) Message object into QByteArray
 * @return Serialized object as QByteArray with size at the beginning
 */
QByteArray Password::toQByteArray()
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
