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

    // Flush size
    quint16 size;
    in >> size;

    in >> m_message;
}

Message::~Message()
{

}

QString Message::message()
{
    return m_message;
}

QString Message::message() const
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
