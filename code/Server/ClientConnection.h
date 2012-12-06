///////////////////////////////////////////////////////////
//  ClientConnection.h
//  Implementation of the Class ClientConnection
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_8025FA43_E1FD_4c29_94EA_DE5FDF7489E8__INCLUDED_)
#define EA_8025FA43_E1FD_4c29_94EA_DE5FDF7489E8__INCLUDED_

#include "ClientConnectionListener.h"
#include "ClientServerConnectionListener.h"

/**
 * Represents single client
 */
class ClientConnection
{

public:
	ClientConnection();
	virtual ~ClientConnection();
	ClientConnectionListener *m_ClientConnectionListener;
	ClientServerConnectionListener *m_ClientServerConnectionListener;

	void disconnectFromServer();
	void sendFileFound(FileLocation location);
	void sendFileList(AliasFileList list);
	void sendFileNotFound();
	void sendFileNotRemoved();
	void sendFileRemoved();
	void sendFileTransferError();
	void sendFileTransferFinished();
	void sendFileTransferStarted();
	void sendNoSuchFile();

private:
	ClientConnectionListener* m_connectionListener;

};
#endif // !defined(EA_8025FA43_E1FD_4c29_94EA_DE5FDF7489E8__INCLUDED_)
