///////////////////////////////////////////////////////////
//  Password.h
//  Implementation of the Class Password
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_91C787C3_94C4_4da7_BA4C_9508DA439345__INCLUDED_)
#define EA_91C787C3_94C4_4da7_BA4C_9508DA439345__INCLUDED_

#include <QString>
#include <QByteArray>

namespace TIN_project {
namespace Utilities {

/**
 * Contains password
 */
class Password
{
    QString m_password;

public:
    Password();
    Password(const QByteArray &bytes);
    virtual ~Password();

    Password(const QString& password);
    bool check(const QString& password) const;
    bool check(const Password& password) const;
    QString password() const;
    QByteArray toQByteArray();
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_91C787C3_94C4_4da7_BA4C_9508DA439345__INCLUDED_)
