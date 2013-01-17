/**
 * @file Password.cpp
 *
 * @date 06-01-2013
 *
 * @author Marcin Kubik <markubik@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::Password
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

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
    QByteArray myHash = QCryptographicHash::hash(password.toUtf8(),
            QCryptographicHash::Sha1);
    (*this).m_hashed_password = myHash;
}

Password::Password(const QByteArray& password)
{
    //QByteArray myHash = QCryptographicHash::hash(password, QCryptographicHash::Sha1);
    (*this).m_hashed_password = password; // TODO ...
}
/**
 * Check if password is correct
 */
bool Password::check(const QString& password) const
{
    QByteArray myHash = QCryptographicHash::hash(password.toUtf8(),
            QCryptographicHash::Sha1);
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
