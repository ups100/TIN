///////////////////////////////////////////////////////////
//  Password.cpp
//  Implementation of the Class Password
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "Password.h"
#include <QCryptographicHash>
#include <QDebug>
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
    QByteArray myHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1);
    (*this).m_hashed_password = myHash;
}

Password::Password(const QByteArray& password)
{
    QByteArray myHash = QCryptographicHash::hash(password, QCryptographicHash::Sha1);
    (*this).m_hashed_password = myHash;
}
/**
 * Check if password is correct
 */
bool Password::check(const QString& password) const
{
    QByteArray myHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1);
    return ((*this).m_hashed_password == myHash);
}

bool Password::check(const Password& password) const
{
    QByteArray myHash = password.toQByteArray();
    return (myHash == (*this).m_hashed_password);
}

bool Password::check(const QByteArray& password) const
{
    return ((*this).m_hashed_password == password);
}
Password& Password::operator=(const Password& other)
{
    (*this).m_hashed_password = other.toQByteArray();
    return *this;
}

QByteArray Password::toQByteArray() const
{
    return (*this).m_hashed_password;
}

QByteArray Password::getHash() const
{
    return (*this).m_hashed_password;
}


} //namespace Utilities
} //namespace TIN_project
