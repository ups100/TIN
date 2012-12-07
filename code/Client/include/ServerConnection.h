///////////////////////////////////////////////////////////
//  ServerConnection.h
//  Implementation of the Class ServerConnection
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_)
#define EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_

#include <QtGlobal>
#include <QString>
#include <QHostAddress>

namespace TIN_project {

namespace Utilities {
	class Password;
	class FileLocation;
}

namespace Client {

class ServerConnection
{

public:
	ServerConnection();
	virtual ~ServerConnection();

	void connectToAlias(QString aliasName, Utilities::Password password);
	void connectToServer(QHostAddress address, quint16 port);
	void createAlias(QString name, Utilities::Password password);
	void findFileInAlias(QString fileName);
	void listAlias();
	void pullFileFrom(Utilities::FileLocation file);
	void pushFileToAlias(QString path);
	void removeFileFromAlias(QString fileName);

};

}//namespace Client
}//namespace TIN_project
#endif // !defined(EA_57AB8497_7A1A_44cf_B8D0_14DE9A413A5A__INCLUDED_)
