///////////////////////////////////////////////////////////
//  ServerConnection.h
//  Implementation of the Class ServerConnection
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_)
#define EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_

#include "ServerConnectionListener.h"

class ServerConnection
{

public:
	ServerConnection();
	virtual ~ServerConnection();

	void connectToServer(QHostAddress address, quint64 port);
	void sendFileFound(FileLocation location);
	void sendFileList(AliasFileList lis);

private:
	ServerConnectionListener m_serverConnectionListener;

};
#endif // !defined(EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_)
