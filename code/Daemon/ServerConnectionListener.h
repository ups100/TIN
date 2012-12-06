///////////////////////////////////////////////////////////
//  ServerConnectionListener.h
//  Implementation of the Class ServerConnectionListener
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_)
#define EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_

#include "ServerConnection.h"

class ServerConnectionListener
{

public:
	ServerConnectionListener();
	virtual ~ServerConnectionListener();
	ServerConnection *m_ServerConnection;

	virtual void onAliasConnected() =0;
	virtual void onAliasConnectionError() =0;
	virtual void onConnected() =0;
	virtual void onDisconnected() =0;
	void onFileNotRemoved();
	virtual void onFindFile(QString fileName) =0;
	virtual void onListFiles() =0;
	virtual void onReciveFile(File file) =0;
	virtual void onRemoveFile(QString fileName) =0;
	virtual void onSendFile(File file) =0;

};
#endif // !defined(EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_)
