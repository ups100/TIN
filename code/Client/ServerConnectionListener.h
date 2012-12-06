///////////////////////////////////////////////////////////
//  ServerConnectionListener.h
//  Implementation of the Class ServerConnectionListener
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_)
#define EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_

#include "ServerConnection.h"

class ServerConnectionListener
{

public:
	ServerConnectionListener();
	virtual ~ServerConnectionListener();
	ServerConnection *m_ServerConnection;

	virtual void onAliasConnected() =0;
	virtual void onAliasConnectionError() =0;
	virtual void onAliasCreated() =0;
	virtual void onAliasCreationError() =0;
	virtual void onAliasDeleted() =0;
	virtual void onAliasDeletionError() =0;
	virtual void onConnected();
	virtual void onDisconnected() =0;

};
#endif // !defined(EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_)
