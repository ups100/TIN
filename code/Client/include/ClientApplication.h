///////////////////////////////////////////////////////////
//  ClientApplication.h
//  Implementation of the Class ClientApplication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

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
#include "Message.h"
#include "ConfigFileName.h"
#include "qtsinglecoreapplication.h"

namespace TIN_project {

namespace Utilities {
class AliasFileList;
class FileLocation;
}

namespace Client {

class ClientApplication : public QObject, public ServerConnectionListener,
        public AliasCommunicationListener
{
Q_OBJECT;
public:
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
    ClientApplication(int, char**);
    virtual ~ClientApplication();

    virtual void onAliasConnected();
    virtual void onAliasConnectionError();
    virtual void onAliasCreated();
    virtual void onAliasCreationError();
    virtual void onAliasDeleted();
    virtual void onAliasDeletionError();
    virtual void onAliasListed(const Utilities::AliasFileList& list);
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onFileFound(const Utilities::FileLocation& location);
    virtual void onFileNotFound();
    virtual void onFileRemoved();
    virtual void onFileRemovingError();
    virtual void onFileTransferError();
    virtual void onFileTransferFinished();
    virtual void onFileTransferStarted();

    /**
     * @Probably to remove
     */
    void setView(boost::shared_ptr<ClientView> view);

    /**
     * @brief starter of the Client Application thread
     * @param[in] address IP address of client
     * @param[in] port port on which client connects
     * @return exec
     */
    int start(const QHostAddress& address, quint16 port);

    /**
     * @brief used to communicate with daemon
     * @param[in] message message to daemon
     */
    void talkToDaemon(Utilities::Message message);

    /**
     * @brief setts actual state of application
     * @param[in] state state to be set
     */
    void setState(ClientApplication::States state);

    /**
     * @brief getter for the actual state of application
     * @return actual state of application
     */
    ClientApplication::States getState() const;

    /**
     * @brief Invoked by Client View when client sets a command
     * @param[in] command command typed by client
     */
    void getCommand(QString command);

private slots:
    void onAliasConnectedSlot();
    void onAliasConnectionErrorSlot();
    void onAliasCreatedSlot();
    void onAliasCreationErrorSlot();
    void onAliasDeletedSlot();
    void onAliasDeletionErrorSlot();
    void onAliasListedSlot(const Utilities::AliasFileList& list);
    void onConnectedSlot();
    void onDisconnectedSlot();
    void onFileFoundSlot(const Utilities::FileLocation& location);
    void onFileNotFoundSlot();
    void onFileRemovedSlot();
    void onFileRemovingErrorSlot();
    void onFileTransferErrorSlot();
    void onFileTransferFinishedSlot();
    void onFileTransferStartedSlot();

private:
    /**
     * @brief checks if the arguments of commands are correct (file exists etc.)
     * @param[in] cmd command to be checked
     * @return true if command may be invoked, false otherwise
     */
    bool checkIntegrity(boost::shared_ptr<Commands> cmd) const;

    /**
     * @brief checks if app is in appropriate state to invoke command
     * @param[in] cmd command to be checked
     * @return true if command may be invoked, false otherwise
     */
    bool checkStateCondition(boost::shared_ptr<Commands> cmd) const;

    /**
     * @brief checks if file exists
     * @param[in] path relative path to be checked
     * @return true if file exists, false otherwise
     */
    bool checkRelativePath(QString path) const;

    /**
     * @brief checks if file exists
     * @param[in] path absolute path to be checked
     * @return true if file exists, false otherwise
     */
    bool checkAbsolutePath(QString path) const;

    /**
     * @brief checks if exists config file
     * @return true if exists, false otherwise
     */
    bool checkIfConfigFileExists() const;

    /**
     * @brief invokes command if everything is correct
     * @param[in] cmd command to be invoked
     * @return true if everything went wrong
     */
    bool invokeCommand(boost::shared_ptr<Commands> cmd);

    /**
     * @brief actual state of app
     */
    ClientApplication::States m_state;

    /**
     * @brief used to make client application work as thread
     */
    QtSingleCoreApplication m_application;

    /**
     * @brief parser used to change string commands to Commands objects
     */
    CommandParser m_commandParser;

    /**
     * @brief handles connection to the server
     */
    ServerConnection m_serverConnection;

    /**
     * @brief handles connection to the daemon
     */
    DaemonCommunication m_DaemonCommunication;

    /**
     * @brief view that is responsible for interaction with user
     */
    boost::shared_ptr<ClientView> m_view;

    /**
     * @brief name of alias under which works the app
     */
    QString m_alias;

    /**
     * @brief password that is used to log to alias
     */
    Password m_password;

    /**
     * @brief IP address of the client
     */
    QHostAddress m_address;

    /**
     * @brief port on which client connects
     */
    quint16 m_port;

};

} //namespace Client
} //namespace TIN_project
Q_DECLARE_OPERATORS_FOR_FLAGS(TIN_project::Client::ClientApplication::States)
#endif // !defined(EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_)
