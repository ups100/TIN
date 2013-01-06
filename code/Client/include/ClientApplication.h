/**
 * @file ClientApplication.h
 *
 * @date 06-01-2013
 *
 * @author Marcin Kubik <markubik@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Client::ClientApplication
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_)
#define EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_

#include <boost/shared_ptr.hpp>
#include <QString>
#include <QObject>
#include "ServerConnection.h"
#include "ClientView.h"
#include "CommandParser.h"
#include "DaemonCommunication.h"
#include "ServerConnectionListener.h"
#include "AliasCommunicationListener.h"
#include "CommunicationProtocol.h"
#include "Message.h"
#include "ConfigFileName.h"
#include "qtsinglecoreapplication.h"

namespace TIN_project {

namespace Utilities {
class AliasFileList;
class FileLocation;
class CommunicationProtocol;
}

namespace Client {

class ClientApplication : public QObject, public ServerConnectionListener,
        public AliasCommunicationListener
{
Q_OBJECT;

public:
/**
 * @brief Enum that represents status of the application
 */
enum State
   {
       IDLE = 0,
       NOT_CONNECTED = 0,
       CONNECTED = 1,
       WAITING = 2,
       WAITING_FOR_DISCONNECT = 3,
       FILELIST = 4,
       LOGGED = 5
   };
    Q_DECLARE_FLAGS(States, State)

    /**
    * @brief Constructor
    *
    * @param[in] argc number of arguments
    *
    * @param[in] argv command line arguments
    */
    ClientApplication(int, char**);

    /**
     * @brief Destructor
     */
    virtual ~ClientApplication();


    /**
     * @brief Invoked by server when client connected to alias
     */
    virtual void onAliasConnected();

    /**
     * @brief Invoked by server when error while connecting appears
     */
    virtual void onAliasConnectionError();

    /**
     * @brief Invoked by server when alias was created
     */
    virtual void onAliasCreated();

    /**
     * @brief Invoked by server when some error while creating alias appears
     */
    virtual void onAliasCreationError();

    /**
     * @brief Invoked by server when alias was deleted
     */
    virtual void onAliasDeleted();

    /**
     * @brief Invoked by server when error while deleting alias appears
     */
    virtual void onAliasDeletionError();

    /**
     * @brief Invoked by server when wants to send AliasFileList object
     * @param[in] list AliasFileList object with information about files in alias
     */
    virtual void onAliasListed(const Utilities::AliasFileList& list);
    virtual void onConnected();

    /**
     * @brief Invoked by server when client disconnected from server
     */
    virtual void onDisconnected();

    /**
     * @brief Invoked by server when File was found
     * @param[in] location Location of File (held in FileLocation object)
     */
    virtual void onFileFound(const Utilities::FileLocation& location);
    virtual void onFileNotFound();

    /**
     * @brief Invoked by server when file was removed from alias
     */
    virtual void onFileRemoved();

    /**
     * @brief Invoked by server when error during removing file appears
     */
    virtual void onFileRemovingError();

    /**
     * @brief Invoked by server when some error during file transfer appears
     */
    virtual void onFileTransferError();

    /**
     * @brief Invoked by server when file transfer is finished
     */
    virtual void onFileTransferFinished();

    /**
     * @brief Invoked by server when file transfer started
     */
    virtual void onFileTransferStarted();

    /**
     * Probably to remove
     */
    void setView(boost::shared_ptr<ClientView> view);

    /**
     * @brief Starter of the Client Application thread
     * @param[in] address IP address of client
     * @param[in] port Port on which client connects
     * @return exec
     */
    int start(const QHostAddress& address, quint16 port);

    /**
     * @brief Used to communicate with daemon
     * @param[in] message Message to daemon
     */
    void talkToDaemon(Utilities::Message message);

    /**
     * @brief Sets actual state of application
     * @param[in] state State to be set
     */
    void setState(ClientApplication::States state);

    /**
     * @brief Getter for the actual state of application
     * @return Actual state of application
     */
    ClientApplication::States getState() const;

    /**
     * @brief Invoked by Client View when client sets a command
     * @param[in] command Command typed by client
     */
    void getCommand(QString command);

private slots:

    /**
     * @brief Helper slot for onAliasConnected()
     * @see onAliasConnected()
     */
    void onAliasConnectedSlot();

    /**
     * @brief Helper slot for onAliasConnectionError()
     * @see onAliasConnectionError()
     */
    void onAliasConnectionErrorSlot();

    /**
     * @brief Helper slot for onAliasCreated()
     * @see onAliasCreated()
     */
    void onAliasCreatedSlot();

    /**
     * @brief Helper slot for onAliasCreationError()
     * @see onAliasCreationError()
     */
    void onAliasCreationErrorSlot();

    /**
     * @brief Helper slot for onAliasDeleted()
     * @see onAliasDeleted()
     */
    void onAliasDeletedSlot();

    /**
     * @brief Helper slot for onAliasDeletionError()
     * @see onAliasDeletionError()
     */
    void onAliasDeletionErrorSlot();

    /**
     * @brief Helper slot for onAliasListed()
     * @see onAliasListed()
     * @param[in] list AliasFileList object that held information about files in alias
     */
    void onAliasListedSlot(const Utilities::AliasFileList& list);

    /**
     * @brief Helper slot for onConnected()
     * @see onConnected()
     */
    void onConnectedSlot();

    /**
     * @brief Helper slot for onDisconnected()
     * @see onDisconnected()
     */
    void onDisconnectedSlot();

    /**
     * @brief Helper slot for onFileFound()
     * @see onFileFound()
     * @param[in] location Location of file in alias
     */
    void onFileFoundSlot(const Utilities::FileLocation& location);

    /**
     * @brief Helper slot for onFileNotFound()
     * @see onFileNotFound()
     */
    void onFileNotFoundSlot();

    /**
     * @brief Helper slot for onFileRemoved()
     * @see onFileRemoved()
     */
    void onFileRemovedSlot();

    /**
     * @brief Helper slot for onFileRemovingError()
     * @see onFileRemovingError()
     */
    void onFileRemovingErrorSlot();

    /**
     * @brief Helper slot for onFileTransferError()
     * @see onFileTransferError()
     */
    void onFileTransferErrorSlot();

    /**
     * @brief Helper slot for onFileTransferFinished()
     * @see onFileTransferFinished()
     */
    void onFileTransferFinishedSlot();

    /**
     * @brief Helper slot for onFileTransferStarted()
     * @see on FileTransferStarted()
     */
    void onFileTransferStartedSlot();

private:
    /**
     * @brief Checks if the arguments of commands are correct (file exists etc.)
     * @param[in] cmd Command to be checked
     * @return True if command may be invoked, false otherwise
     */
    bool checkIntegrity(boost::shared_ptr<Commands> cmd) const;

    /**
     * @brief Checks if app is in appropriate state to invoke command
     * @param[in] cmd Command to be checked
     * @return True if command may be invoked, false otherwise
     */
    bool checkStateCondition(boost::shared_ptr<Commands> cmd) const;

    /**
     * @brief Checks if file exists
     * @param[in] path Relative path to be checked
     * @return True if file exists, false otherwise
     */
    bool checkRelativePath(QString path) const;

    /**
     * @brief Checks if file exists
     * @param[in] path Absolute path to be checked
     * @return True if file exists, false otherwise
     */
    bool checkAbsolutePath(QString path) const;

    /**
     * @brief Checks if exists config file
     * @return True if exists, false otherwise
     */
    bool checkIfConfigFileExists() const;

    /**
     * @brief Invokes command if everything is correct
     * @param[in] cmd Command to be invoked
     * @return True if everything went wrong
     */
    bool invokeCommand(boost::shared_ptr<Commands> cmd);

    /**
     * @brief Actual state of app
     */
    ClientApplication::States m_state;

    /**
     * @brief Object for handling signals and check for other instances
     */
    QtSingleCoreApplication m_application;

    /**
     * @brief Parser used to change string commands to Commands objects
     */
    CommandParser m_commandParser;

    /**
     * @brief Handles connection to the server
     */
    ServerConnection m_serverConnection;

    /**
     * @brief Handles connection to the daemon
     */
    DaemonCommunication m_DaemonCommunication;

    /**
     * @brief View that is responsible for interaction with user
     */
    boost::shared_ptr<ClientView> m_view;

    /**
     * @brief Name of alias under which works the app
     */
    QString m_alias;

    /**
     * @brief Password that is used to log to alias
     */
    Password m_password;

    /**
     * @brief IP address of the client
     */
    QHostAddress m_address;

    /**
     * @brief Port on which client connects
     */
    quint16 m_port;

    /**
     * @brief Holds the current command
     * @info Useful when onAliastList invoked
     */
    boost::shared_ptr<Commands> m_command;

};

} //namespace Client
} //namespace TIN_project
Q_DECLARE_OPERATORS_FOR_FLAGS(TIN_project::Client::ClientApplication::States)
#endif // !defined(EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_)
