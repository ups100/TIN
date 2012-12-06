///////////////////////////////////////////////////////////
//  FileReciver.h
//  Implementation of the Class FileReciver
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_)
#define EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_

#include "FileTransferListener.h"

/**
 * Used for receiving files
 */
class FileReciver
{

public:
	FileReciver();
	virtual ~FileReciver();
	FileTransferListener *m_FileTransferListener;

	void FileReciver(QHostAddress address, quint port, File file);

};
#endif // !defined(EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_)
