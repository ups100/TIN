///////////////////////////////////////////////////////////
//  FileSender.h
//  Implementation of the Class FileSender
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_)
#define EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_

#include <QHostAddress>
#include <QtGlobal>

#include "File.h"

namespace TIN_project {
namespace Daemon {

class FileTransferListener;

/**
 * Used for sending some file from this machine
 */
class FileSender
{

public:
	FileSender();
	virtual ~FileSender();
	FileTransferListener *m_FileTransferListener;

	FileSender(QHostAddress address, quint16 port, File file);

};

}//namespace Daemon
}//namespace TIN_project
#endif // !defined(EA_8360E247_8764_4b3d_95B5_552986452643__INCLUDED_)
