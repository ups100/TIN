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
#include "Identifier.h"

#include <boost/shared_ptr.hpp>
#include <QThread>

namespace TIN_project {
namespace Server {

/**
 * Remember to disable copy constructor
 */
class Alias : public QObject,
        public FileTransferListener,
        public ClientConnectionListener,
        public DaemonConnectionListener
{
    Q_OBJECT;
public:
    /**
     * @brief Constructor
     *
     * @param[in] name of alias
     *
     * @param[in] password to this alias
     *
     * @param[in] address of running Server
     *
     * @note Remember to start alias after creation with start() function.
     */
    Alias(const QString& name, Utilities::Password password, QHostAddress address);

    /**
     * @brief Destructor
     */
    virtual ~Alias();

    /**
     * @brief Adds client to this alias
     *
     * @param[in] client to be added to alias
     *
     * @param[in] id identity of client
     *
     * @note Remember to check password before using this method
     *
     * @warning Due to some thread issues, his function should not be executed
     * in context of alias thread.
     */
    void addClient(boost::shared_ptr<UnknownConnection> client, const Utilities::Identifier& id);

    /**
     * @brief Adds daemon to this alias
     *
     * @param[in] daemon to be added to alias
     *
     * @param[in] id identity of daemon
     *
     * @note Remember to check password before using this method
     *
     * @warning Due to some thread issues, his function should not be executed
     * in context of alias thread.
     */
    void addDaemon(boost::shared_ptr<UnknownConnection> daemon, const Utilities::Identifier& id);

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
            const Utilities::AliasFileList& location);

    virtual void onFileList(DaemonConnection* daemon,
            const Utilities::AliasFileList& list);

    virtual void onFileTransferStarted(FileTransferServer *transfer);

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

    /**
     * @brief Takes care of ending action when some unexpected
     * situation occurs.
     */
    //void performLastAliasAction();

    /**
     * @brief This method is used when some daemon is call to check
     * what make with him in case this is TRANSFER state
     */
    void waitForReceivers(DaemonConnection *dc);

private slots:
    void removeDaemonSlot(DaemonConnection *dc);
    void removeClientSlot(ClientConnection *cc);
    void performPullActionSlot();
    void performFindFileSlot();
    void performRemoveFileSlot();
    void performPushActionSlot();

private:
    /**
     * @brief Disable copy constructor
     */
    Alias(const Alias &);

    /**
     * @brief Check server IP address
     */
    QHostAddress getServerIp();

    /**
     * @brief Executed in state PULL_TRANSFER
     * when all daemons answer or disconnect this is perform.
     */
    void performPullAction();

    /**
     * @brief This method clear data after perform Pull action.
     */
    void afterPullAction();

    /**
     * @brief This method is call when all daemon answers.
     * It is executed in state Push_transfer
     */
    void performPushAction();

    /**
     * @brief This method clear data after perform Push action.
     * Keeps state to be right etc.
     */
    void afterPushAction();

    /**
     * @brief Executed in state FIND_FILE when all daemons answered.
     */
    void performFindFile();

    /**
     * @brief Executed in state REMOVE_FILE when all daemons answered.
     */
    void performRemoveFile();

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
    QList<FileTransferServer* > m_transfers;

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

    QList<Utilities::FileLocation *> m_location;

    //Utilities::Identifier *m_senderDaemonIdentifier;

    /**
     * @brief Pointer to daemon which will be send a file.
     * @brief Out of transferring state this should be Null.
     * @note There is always only one Sender
     */
    DaemonConnection *m_senderDaemon;

    /**
     * @brief List of receivers daemon.
     * @note This is used in Pull, Push, etc.
     */
    QList<DaemonConnection *> m_receiverDaemon;

    /**
     * @brief This list provide all daemons which are in some action.
     *  For example somebody wants FileList from them
     *  or in transfer state they hold all receivers.
     */
    QList<DaemonConnection*> m_actionDaemon;

    /**
     * @brief This is the list of client which should be notify
     * about some action performed.
     */
    QList<ClientConnection*> m_notifyClient;

    /**
     * @brief How many daemons left to wait for them
     */
    //quint32 m_waitForDaemons;

    /**
     * @brief Inform only this object if there is file to remove
     */
    bool m_removeFind;
    QString m_removeName;

    /**
     * @brief Temporary AliasFileList to merge from each daemon and send it all to client
     */
    boost::shared_ptr<Utilities::AliasFileList> m_tmpAliasFileList;

    enum AliasAction {
        LIST_ALIAS,
        FIND_FILE,
        REMOVE_FILE,
        PULL_TRANSFER,
        PUSH_TRANSFER,
        NONE
    };
    /**
     * Depreciated Try to do not use it. It will be deleted
     */
    //enum AliasAction m_lastAliasAction;
    /**
     * @brief Current execution action.
     */
    enum AliasAction m_currentAction;

    QHostAddress m_address;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_F04FAC3F_04DD_40c0_89EC_6EFF6338002F__INCLUDED_)
