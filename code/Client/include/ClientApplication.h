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

#include "ServerConnection.h"
#include "ClientView.h"
#include "CommandParser.h"
#include "DaemonCommunication.h"
#include "ServerConnectionListener.h"
#include "AliasCommunicationListener.h"


namespace TIN_project {

namespace Utilities {
	class AliasFileList;
	class FileLocation;
}

namespace Client {

class ClientApplication : public ServerConnectionListener, public AliasCommunicationListener
{

public:
	ClientApplication();
	virtual ~ClientApplication();

	virtual void onAliasConnected();
	virtual void onAliasConnectionError();
	virtual void onAliasCreated();
	virtual void onAliasCreationError();
	virtual void onAliasDeleted();
	virtual void onAliasDeletionError();
	virtual void onAliasListed(Utilities::AliasFileList list);
	virtual void onConnected();
	virtual void onDisconnected();
	virtual void onFileFound(Utilities::FileLocation location);
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
	DaemonCommunication m_DaemonCommunication;
	boost::shared_ptr<ClientView> m_view;

};

}//namespace Client
}//namespace TIN_project

#endif // !defined(EA_0598AE82_7F01_435a_8E47_7FCAFCEE17A4__INCLUDED_)
