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
       NOT_CONNECTED = 0,
       CONNECTED = 1,
       WAITING = 2,
       WAITING_FOR_DISCONNECT = 3,
       FILELIST = 4,
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
    void setView(boost::shared_ptr<ClientView> view);
    int start(const QHostAddress&, quint16);
    void talkToDaemon(Utilities::Message message);
    bool checkIfPossible(boost::shared_ptr<Commands>);
    void setState(ClientApplication::States);
    ClientApplication::States getState() const;
    void getString(QString);
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
    bool works;
    ClientApplication::States m_state;
    QtSingleCoreApplication m_application;
    CommandParser m_commandParser;
    ServerConnection m_serverConnection;
    DaemonCommunication m_DaemonCommunication;
    boost::shared_ptr<ClientView> m_view;

};

} //namespace Client
} //namespace TIN_project
Q_DECLARE_OPERATORS_FOR_FLAGS(TIN_project::Client::ClientApplication::States)
#endif // !defined(EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_)
