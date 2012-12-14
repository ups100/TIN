///////////////////////////////////////////////////////////
//  Message.cpp
//  Implementation of the Class Message
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "Message.h"
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

Message::Message()
{
}

Message::Message(const char *message)
        : m_message(message)
{

}

Message::Message(const QByteArray &bytes)
{
    QDataStream in(bytes);
    in >> *this;
}

Message::~Message()
{

}

QString Message::message()
{
    return m_message;
}

void Message::message(const QString message)
{
    m_message = message;
}

QByteArray Message::toQByteArray()
{
    QByteArray bytes;

    QDataStream out(&bytes, QIODevice::WriteOnly);
    out << *this;

    return bytes;
}

QDataStream &operator<<(QDataStream &out, const Message &message)
{
    out << message.m_message;

    return out;
}

QDataStream &operator>>(QDataStream &in, Message &message)
{
    in >> message.m_message;

    return in;
}

} //namespace Utilities
} //namespace TIN_project
