///////////////////////////////////////////////////////////
//  FileTransferListener.h
//  Implementation of the Class FileTransferListener
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_01367691_430E_42ec_8D5F_87C8F2B45C82__INCLUDED_)
#define EA_01367691_430E_42ec_8D5F_87C8F2B45C82__INCLUDED_

namespace TIN_project {
namespace Server {

class FileTransferServer;

class FileTransferListener
{

public:
	FileTransferListener();
	virtual ~FileTransferListener();

	virtual void onFileTransferCompleted(FileTransferServer * transfer) =0;
	virtual void onFileTransferError(FileTransferServer* transfer) =0;

};

}//namespace server
}//namespace TIN_project
#endif // !defined(EA_01367691_430E_42ec_8D5F_87C8F2B45C82__INCLUDED_)
