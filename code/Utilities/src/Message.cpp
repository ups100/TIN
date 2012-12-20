///////////////////////////////////////////////////////////
//  Message.cpp
//  Implementation of the Class Message
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "Message.h"
#include <QDataStream>
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

/**
 * C-tor
 */
Message::Message()
{
}

/**
 * C-tor
 * @brief Construct message
 * @param message Char sequence to save in message
 */
Message::Message(const char *message)
        : m_message(message)
{

}

/**
 * C-tor
 * @brief Creates message instance from given QByteArray
 * @param bytes QByteArray object made of serialized message
 */
Message::Message(const QByteArray &bytes)
{
    QDataStream in(bytes);

    // Flush size
    quint16 size;
    in >> size;

    in >> m_message;
}

/**
 * D-tor
 */
Message::~Message()
{

}

/**
 * @brief Get value of message
 * @return message
 */
QString Message::message()
{
    return m_message;
}

/**
 * @see Message::message()
 * @return message
 */
QString Message::message() const
{
    return m_message;
}

/**
 * @brief Set new message
 * @param message New message
 */
void Message::message(const QString message)
{
    m_message = message;
}

/**
 * @brief Convert (serialize) Message object into QByteArray
 * @return Serialized object as QByteArray with size at the beginning
 */
QByteArray Message::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    // Size
    out << (quint16) 0;

    out << m_message;

    // Set size
    out.device()->seek(0);
    out << (quint16) (bytes.size() - sizeof(quint16));

    return bytes;
}

} //namespace Utilities
} //namespace TIN_project
