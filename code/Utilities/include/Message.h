///////////////////////////////////////////////////////////
//  Message.h
//  Implementation of the Class Message
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_)
#define EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_

#include <QString>
#include <QByteArray>

namespace TIN_project {
namespace Utilities {

/**
 * Communication between client and daemons
 */
class Message
{
    QString m_message;

public:
    Message();
    Message(const char *message);
    Message(const QByteArray &bytes);
    virtual ~Message();

    QString message();
    QString message() const;
    void message(const QString message);
    QByteArray toQByteArray();
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_)
