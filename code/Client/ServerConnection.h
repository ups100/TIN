///////////////////////////////////////////////////////////
//  ServerConnection.h
//  Implementation of the Class ServerConnection
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_)
#define EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_

class ServerConnection
{

public:
	ServerConnection();
	virtual ~ServerConnection();

	void connectToAlias(QString aliasName, Password password);
	void connectToServer(QHostAddress address, quint16 port);
	void createAlias(QString name, Password password);
	void findFileInAlias(QString fileName);
	void listAlias();
	void pullFileFrom(FileLocation file);
	void pushFileToAlias(QString path);
	void removeFileFromAlias(QString fileName);

};
#endif // !defined(EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_)
