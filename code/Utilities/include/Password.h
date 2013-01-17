/**
 * @file Password.h
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

#if !defined(EA_91C787C3_94C4_4da7_BA4C_9508DA439345__INCLUDED_)
#define EA_91C787C3_94C4_4da7_BA4C_9508DA439345__INCLUDED_

#include <QString>
#include <QMetaType>

namespace TIN_project {
namespace Utilities {

/**
 * Contains password
 */
class Password
{

public:
    Password();
    virtual ~Password();

    Password(const Password& password);
    Password(const QString& password);
    Password(const QByteArray& password);
    bool check(const QString& password) const;
    bool check(const QByteArray& password) const;
    bool check(const Password& password) const;
    QByteArray toQByteArray() const;
    QByteArray getHash() const;
    Password& operator=(const Password& other);
private:
    QByteArray m_hashed_password;
};

} //namespace Utilities
} //namespace TIN_project

Q_DECLARE_METATYPE(TIN_project::Utilities::Password);

#endif // !defined(EA_91C787C3_94C4_4da7_BA4C_9508DA439345__INCLUDED_)
