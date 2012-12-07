///////////////////////////////////////////////////////////
//  ServerConnection.h
//  Implementation of the Class ServerConnection
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_)
#define EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_

#include <QHostAddress>

namespace TIN_project {

namespace Utilities {
	class FileLocation;
	class AliasFileList;
}

namespace Daemon {

class ServerConnectionListener;

class ServerConnection
{

public:
	ServerConnection();
	virtual ~ServerConnection();

	void connectToServer(QHostAddress address, quint64 port);
	void sendFileFound(Utilities::FileLocation location);
	void sendFileList(Utilities::AliasFileList list);

private:
	ServerConnectionListener *m_serverConnectionListener;

};

}//namespace Daemon
}//namespace TIN_project
#endif // !defined(EA_8811645B_7EE0_4f8a_881A_13DB83EB0BD6__INCLUDED_)
