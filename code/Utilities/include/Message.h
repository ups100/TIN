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
    /** Message value */
    QString m_message;

public:
    Message();

    /**
     * @brief C-tor, construct message
     * @param message Char sequence to save in message
     */
    Message(const char *message);

    /**
     * @brief C-tor, creates message instance from given QByteArray
     * @param bytes QByteArray object made of serialized message
     */
    Message(const QByteArray &bytes);

    /**
     * @brief D-tor
     */
    virtual ~Message();

    /**
     * @brief Get value of message
     * @return message
     */
    QString getMessage();

    /**
     * @see Message::message()
     * @return message
     */
    QString getMessage() const;

    /**
     * @brief Set new message
     * @param message New message
     */
    void message(const QString message);

    /**
     * @brief Convert (serialize) Message object into QByteArray
     * @return Serialized object as QByteArray with size at the beginning
     */
    QByteArray toQByteArray();
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_)
