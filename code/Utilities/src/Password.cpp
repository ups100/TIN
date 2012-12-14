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

}

Password::Password(const QString& password)
{
    m_password = password;
}

QString Password::getPassword()
{
    return m_password;
}
Password::~Password()
{

}

/**
 * Check if password is correct
 */
bool Password::check(const QString& password) const
{
    return (this->m_password == password);
}

bool Password::check(const Password& password) const
{
    return (this->m_password == password.m_password);

}
} //namespace Utilities
} //namespace TIN_project
