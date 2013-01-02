/**
 * @file Daemon/include/ServerConnectionListener.h
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Daemon::ServerConnectionListener
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_)
#define EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_

#include <QString>
#include <QHostAddress>

#include "File.h"

namespace TIN_project {
namespace Daemon {

/**
 * @brief Class used as interface for communication by ServerConnection class
 *
 * @details This class is used for notifying about communicates
 * incoming from server.
 *
 * @warning Remember that all those methods are called in context of ServerConnection's thread.
 */
class ServerConnectionListener
{

public:
    /**
     * @brief Constructor
     */
    ServerConnectionListener();

    /**
     * @brief Destructor
     */
    virtual ~ServerConnectionListener();

    /**
     * @brief Informs that catalog has been correctly attached to alias
     */
    virtual void onAliasConnected() = 0;

    /**
     * @brief Informs that catalog has not been correctly attached to alias
     */
    virtual void onAliasConnectionError() = 0;

    /**
     * @brief Informs that connection with server has been established.
     */
    virtual void onConnected() = 0;

    /**
     * @brief Informs that connection with server has been closed or
     * some connection error occurred.
     */
    virtual void onDisconnected() = 0;

    /**
     * @brief Informs that server asked to look for some file
     *
     * @param[in] fileName name of file to look for
     */
    virtual void onFindFile(const QString& fileName) = 0;

    /**
     * @brief Informs that server asked to list content of catalog
     */
    virtual void onListFiles() = 0;

    /**
     * @brief Informs that server asked to receive some file
     *
     * @param[in] fileName name of file (relative path)
     *
     * @param[in] address of server to receive file
     *
     * @param[in] port of server to receive file
     */
    virtual void onReciveFile(const QString& fileName, const QHostAddress& address, quint16 port) = 0;

    /**
     * @brief Informs that server asked to remove some file from catalog
     *
     * @param[in] fileName path to file to be removed
     */
    virtual void onRemoveFile(const QString& fileName) = 0;

    /**
     * @todo not sure about semantic
     */
    virtual void onSendFile(const QString& fileName, const QHostAddress& address, quint16 port) = 0;

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_)
