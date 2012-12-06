///////////////////////////////////////////////////////////
//  ClientApplication.h
//  Implementation of the Class ClientApplication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_)
#define EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_

#include "ServerConnection.h"
#include "ClientView.h"
#include "CommandParser.h"
#include "DaemonCommunication.h"
#include "ServerConnectionListener.h"
#include "AliasCommunicationListener.h"

class ClientApplication : public ServerConnectionListener, public AliasCommunicationListener
{

public:
	ClientApplication();
	virtual ~ClientApplication();
	ClientView *m_ClientView;
	ServerConnection *m_ServerConnection;
	CommandParser *m_CommandParser;
	DaemonCommunication *m_DaemonCommunication;

	virtual void onAliasConnected();
	virtual void onAliasConnectionError();
	virtual void onAliasCreated();
	virtual void onAliasCreationError();
	virtual void onAliasDeleted();
	virtual void onAliasDeletionError();
	virtual void onAliasListed(AliasFileList list);
	virtual void onConnected();
	virtual void onDisconnected();
	virtual void onFileFound(FileLocation location);
	virtual void onFileNotFound();
	virtual void onFileRemoved();
	virtual void onFileRemovingError();
	virtual void onFileTransferError();
	virtual void onFileTransferFinished();
	virtual void onFileTransferStarted();
	void setView(boost::shared_ptr<ClientView> view);
	void start();

private:
	CommandParser m_commandParser;
	ServerConnection m_serverConnection;
	boost::shared_ptr<ClientView> m_view;

};
#endif // !defined(EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_)