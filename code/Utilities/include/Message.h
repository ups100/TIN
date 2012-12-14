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
#include <QDataStream>

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
    void message(const QString message);
    QByteArray toQByteArray();

    friend QDataStream &operator<<(QDataStream &out, const Message &message);
    friend QDataStream &operator>>(QDataStream &in, Message &message);

};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_)
