///////////////////////////////////////////////////////////
//  Password.cpp
//  Implementation of the Class Password
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "Password.h"

namespace TIN_project {
namespace Utilities {

Password::Password()
{
    //Nothing to do here
}

Password::~Password()
{

}

Password::Password(const Password& password)
{
    (*this) = password;
}
/**
 * Creates an object and set it's password
 */
Password::Password(const QString& password)
{

}

Password::Password(const QByteArray& password)
{

}
/**
 * Check if password is correct
 */
bool Password::check(const QString& password) const
{

    return true;
}

bool Password::check(const Password& password) const
{

    return true;
}

Password& Password::operator=(const Password& other)
{
    (*this).m_hashed_password = other.toQByteArray();
    return *this;
}

QByteArray Password::toQByteArray() const
{
    return QByteArray();
}

} //namespace Utilities
} //namespace TIN_project
