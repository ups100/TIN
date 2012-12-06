///////////////////////////////////////////////////////////
//  FileSender.h
//  Implementation of the Class FileSender
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_)
#define EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_

#include "FileTransferListener.h"

/**
 * Used for sending some file from this machine
 */
class FileSender
{

public:
	FileSender();
	virtual ~FileSender();
	FileTransferListener *m_FileTransferListener;

	FileSender(QHostAdderss address, quint16 port, File file);

};
#endif // !defined(EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_)
