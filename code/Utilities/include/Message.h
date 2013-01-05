/**
 * @file Message.h
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

#if !defined(EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_)
#define EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_

#include "Password.h"
#include <QString>
#include <QByteArray>
#include <QHostAddress>

namespace TIN_project {
namespace Utilities {

/**
 * Communication between client and daemons
 */
class Message
{
    /** Alias id */
    QString m_aliasId;

    /** Password to alias */
    Password m_aliasPassword;

    /** Path to catalogue */
    QString m_cataloguePath;

    /** Server ip address */
    QHostAddress m_serverIpAddress;

    /** Server port number */
    quint16 m_serverPort;

public:
    Message();

    /**
     * @brief C-tor, construct message
     * @param aliasId Alias id
     * @param password Password to alias
     * @param path Path to catalogue
     * @param ip Server IP address
     * @param port Server port number
     */
    Message(const QString aliasId, const Password password, const QString path,
            const QHostAddress ip, const quint16 port);

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
     * @brief Get value of aliasId
     * @return Alias ID
     */
    const QString& getAliasId();

    /**
     * @brief Get value of password
     * @return Password to alias
     */
    const Password& getAliasPassword();

    /**
     * @brief Get value of path
     * @return Path to catalogue
     */
    const QString& getCataloguePath();

    /**
     * @brief Get value of server ip address
     * @return Server IP address
     */
    const QHostAddress& getServerIpAddress();

    /**
     * @brief Get value of serer port
     * @return Server port
     */
    const quint16& getServerPort();

    /**
     * @brief Convert (serialize) Message object into QByteArray
     * @return Serialized object as QByteArray with size at the beginning
     */
    QByteArray toQByteArray();
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_0E30CFF6_DCAD_423d_B631_22AE074FA14F__INCLUDED_)
