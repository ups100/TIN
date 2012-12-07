///////////////////////////////////////////////////////////
//  Alias.h
//  Implementation of the Class Alias
//  Created on:      07-gru-2012 00:33:32
//  Original author: kopasiak
///////////////////////////////////////////////////////////

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

#include <boost/shared_ptr.hpp>
namespace TIN_project {
namespace Server {

/**
 * Remember copy constructor 
 */
class Alias : public FileTransferListener, public ClientConnectionListener, public DaemonConnectionListener
{

public:
	Alias();
	virtual ~Alias();
	FileTransferServer *m_FileTransferServer;
	ClientConnection *m_ClientConnection;
	DaemonConnection *m_DaemonConnection;

	Alias(QString name, Utilities::Password password);
	void addClient(boost::shared_ptr<ClientConnection> client);
	void addDaemon(boost::shared_ptr<DaemonConnection> daemon);
	bool checkPassword(Utilities::Password password);
	virtual void onConnectionClose(ClientConnection* client);
	virtual void onConnectionClose(DaemonConnection* daemon);
	virtual void onFileFound(DaemonConnection* daemon, Utilities::FileLocation location);
	virtual void onFileList(DaemonConnection* daemon, Utilities::AliasFileList list);
	void onFileTransferCompleted(FileTransferServer * transfer);
	void onFileTransferError(FileTransferServer* transfer);
	virtual void onFindFile(ClientConnection* client, QString name);
	virtual void onListAlias(ClientConnection* client);
	virtual void onNoSuchFile(DaemonConnection* daemon);
	virtual void onPullFileFrom(ClientConnection* client, Utilities::FileLocation location);
	virtual void onPushFileToAlias(ClientConnection* client, QString path, quint64 size);
	virtual void onRemoveFromAlias(ClientConnection* client, QString fileName);

private:
	/**
	 * List of connected clients
	 */
	QList<boost::shared_ptr<ClientConnection> >  m_clients;
	QList<boost::shared_ptr<DaemonConnection> > m_daemons;
	/**
	 * Name of alias
	 */
	QString m_name;
	/**
	 * Password for this alias
	 */
	Utilities::Password m_password;

};

}//namespace server
}//namespace TIN_project
#endif // !defined(EA_F04FAC3F_04DD_40c0_89EC_6EFF6338002F__INCLUDED_)
