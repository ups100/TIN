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
        : m_serverPort(0)
{

}

Message::Message(const QString aliasId, const Password password,
        const QString path, const QHostAddress ip, const quint16 port)
        : m_aliasId(aliasId), m_aliasPassword(password), m_cataloguePath(path),
                m_serverIpAddress(ip), m_serverPort(port)
{

}

Message::Message(const QString aliasId, const QString path)
        : m_aliasId(aliasId), m_cataloguePath(path), m_serverPort(0)
{

}

Message::Message(const QByteArray &bytes)
        : m_serverPort(0)
{
    QDataStream in(bytes);
    QByteArray tmp;

    in >> m_aliasId;
    in >> tmp;
    m_aliasPassword = tmp;

    in >> m_cataloguePath;
    in >> m_serverIpAddress;
    in >> m_serverPort;
}

Message::~Message()
{

}

const QString& Message::getAliasId()
{
    return m_aliasId;
}

const Password& Message::getAliasPassword()
{
    return m_aliasPassword;
}

const QString& Message::getCataloguePath()
{
    return m_cataloguePath;
}

const QHostAddress& Message::getServerIpAddress()
{
    return m_serverIpAddress;
}

const quint16& Message::getServerPort()
{
    return m_serverPort;
}

QByteArray Message::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << m_aliasId;
    out << m_aliasPassword.toQByteArray();
    out << m_cataloguePath;
    out << m_serverIpAddress;
    out << m_serverPort;

    return bytes;
}

} //namespace Utilities
} //namespace TIN_project
