///////////////////////////////////////////////////////////
//  FileTransferListener.h
//  Implementation of the Class FileTransferListener
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_CDCA04F0_86FE_4b54_BBD9_F094F10EFB6E__INCLUDED_)
#define EA_CDCA04F0_86FE_4b54_BBD9_F094F10EFB6E__INCLUDED_

namespace TIN_project {
namespace Daemon {

class FileSender;
class FileReciver;

class FileTransferListener
{

public:
	FileTransferListener();
	virtual ~FileTransferListener();

	virtual void onTransferEnd(FileSender * sender) =0;
	virtual void onTransferEnd(FileReciver * reciver) =0;

};

}//namespace Daemon
}//namespace TIN_project
#endif // !defined(EA_CDCA04F0_86FE_4b54_BBD9_F094F10EFB6E__INCLUDED_)
