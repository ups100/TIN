///////////////////////////////////////////////////////////
//  FileTransferServer.h
//  Implementation of the Class FileTransferServer
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_8F738BBF_D47D_472b_9E55_623B17AAECE7__INCLUDED_)
#define EA_8F738BBF_D47D_472b_9E55_623B17AAECE7__INCLUDED_

#include "FileTransferListener.h"

/**
 * W konstruktorze podawana liczba po³aczeñ
 */
class FileTransferServer
{

public:
	FileTransferServer();
	virtual ~FileTransferServer();
	FileTransferListener *m_FileTransferListener;

	QHostAddress getAddress();
	quint getPort();

private:
	QHostAddress m_address;
	/**
	 * Port number
	 */
	quint16 m_port;

};
#endif // !defined(EA_8F738BBF_D47D_472b_9E55_623B17AAECE7__INCLUDED_)
