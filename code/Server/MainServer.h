///////////////////////////////////////////////////////////
//  MainServer.h
//  Implementation of the Class MainServer
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_)
#define EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_

#include "ClientServerConnectionListener.h"

class MainServer : public ClientServerConnectionListener
{

public:
	MainServer();
	virtual ~MainServer();

	virtual void onConnectToAlias(QString name, Password password);
	virtual void onCreateAlias(QString name, Password password);
	virtual void onRemoveAlias(QString name, Password password);
	void start(QHostAddress address, quint16 port);

private:
	QList<Alias> m_aliases;

};
#endif // !defined(EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_)
