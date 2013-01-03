/**
 * @file Alias.h
 *
 * @date 28-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::Alias
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_F04FAC3F_04DD_40c0_89EC_6EFF6338002F__INCLUDED_)
#define EA_F04FAC3F_04DD_40c0_89EC_6EFF6338002F__INCLUDED_

#include "FileTransferServer.h"
#include "FileTransferListener.h"
#include "ClientConnectionListener.h"
#include "DaemonConnectionListener.h"
#include "ClientConnection.h"
#include "DaemonConnection.h"
#include "Password.h"
#include "FileLocation.h"
#include "AliasFileList.h"
#include "UnknownConnection.h"

#include <boost/shared_ptr.hpp>
#include <QThread>

namespace TIN_project {
namespace Server {

/**
 * Remember to disable copy constructor
 */
class Alias : public FileTransferListener,
        public ClientConnectionListener,
        public DaemonConnectionListener
{

public:
    /**
     * @brief Constructor
     *
     * @param[in] name of alias
     *
     * @param[in] password to this alias
     *
     * @note Remember to start alias after creation with start() function.
     */
    Alias(const QString& name, Utilities::Password password);

    /**
     * @brief Destructor
     */
    virtual ~Alias();

    /**
     * @brief Adds client to this alias
     *
     * @param[in] client to be added to alias
     *
     * @note Remember to check password before using this method
     *
     * @warning Due to some thread issues, his function should not be executed
     * in context of alias thread.
     */
    void addClient(boost::shared_ptr<UnknownConnection> client);

    /**
     * @brief Adds daemon to this alias
     *
     * @param[in] daemon to be added to alias
     *
     * @note Remember to check password before using this method
     *
     * @warning Due to some thread issues, his function should not be executed
     * in context of alias thread.
     */
    void addDaemon(boost::shared_ptr<UnknownConnection> daemon);

    /**
     * @brief Check if passed password is that same password as set in constructor.
     *
     * @param[in] password to be checked
     *
     * @return
     * - true if password is correct
     * - false otherwise
     */
    bool checkPassword(const Utilities::Password& password);

    /**
     * @brief Gets the name set in constructor
     *
     * @return Name of alias
     */
    const QString& getName();

    /**
     * @brief Starts the alias
     *
     * @details Synchronous method.
     */
    void start();

    /**
     * @brief Stops the alias.
     *
     * @details Synchronous method. If alias is not started, returns immediately.
     */
    void stop();

    virtual void onConnectionClosed(ClientConnection* client);

    virtual void onConnectionClosed(DaemonConnection* daemon);

    virtual void onFileFound(DaemonConnection* daemon,
            const Utilities::FileLocation& location);

    virtual void onFileList(DaemonConnection* daemon,
            const Utilities::AliasFileList& list);

    virtual void onFileTransferCompleted(FileTransferServer *transfer);

    virtual void onFileTransferError(FileTransferServer *transfer);

    virtual void onFindFile(ClientConnection *client, const QString& name);

    virtual void onListAlias(ClientConnection *client);

    virtual void onNoSuchFile(DaemonConnection *daemon);

    virtual void onPullFileFrom(ClientConnection *client,
            const Utilities::FileLocation& location);

    virtual void onPushFileToAlias(ClientConnection *client,
            const QString& path, quint64 size);

    virtual void onRemoveFromAlias(ClientConnection *client,
            const QString& fileName);

private:

    /**
     * @brief Clients connected to this alias
     */
    QList<boost::shared_ptr<ClientConnection> > m_clients;

    /**
     * @brief Daemons connected to this alias
     */
    QList<boost::shared_ptr<DaemonConnection> > m_daemons;

    /**
     * @brief File transfers in alias
     */
    QList<boost::shared_ptr<FileTransferServer> > m_transfers;

    /**
     * @brief Name of alias
     */
    QString m_name;

    /**
     * @brief Password for this alias
     */
    Utilities::Password m_password;

    /**
     * @brief Additional thread
     */
    QThread m_thread;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_F04FAC3F_04DD_40c0_89EC_6EFF6338002F__INCLUDED_)
