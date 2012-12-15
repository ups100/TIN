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

Password::Password(QString& password)
{
    m_password = password;
}


/* I guess something has to be done with this */
/**
 * @brief getter for the password
 * @return password
 */
QString Password::getPassword()
{
    return m_password;
}

Password::~Password()
{

}

/**
 * @brief check if password is correct
 */
bool Password::check(const QString& password) const
{
    return (this->m_password == password);
}

/**
 * @brief check if password is correct
 */
bool Password::check(const Password& password) const
{
    return (this->m_password == password.m_password);

}
} //namespace Utilities
} //namespace TIN_project
