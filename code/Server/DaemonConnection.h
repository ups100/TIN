///////////////////////////////////////////////////////////
//  DaemonConnection.h
//  Implementation of the Class DaemonConnection
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_)
#define EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_

/**
 * Represents single connection with daemon
 */
class DaemonConnection
{

public:
	DaemonConnection();
	virtual ~DaemonConnection();

	void disconnectFromServer();
	void sendFindFile(QString fileName);
	void sendReciveFile(QString filename, QHostAddress address, quint16 port);
	void sendRemoveFile(QString fileName);
	void sendSendFile(QString fileName, QHostAddress address, quint16 port);

private:
	DaemonConnectionListener* m_connectionListener;

};
#endif // !defined(EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_)
