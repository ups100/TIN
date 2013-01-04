/**
 * @file Message.cpp
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::Message
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

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
