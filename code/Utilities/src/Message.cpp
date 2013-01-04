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

    in >> m_message;
}

Message::~Message()
{

}

QString Message::getMessage()
{
    return m_message;
}

QString Message::getMessage() const
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

    out << m_message;

    return bytes;
}

} //namespace Utilities
} //namespace TIN_project
