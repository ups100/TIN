///////////////////////////////////////////////////////////
//  AliasCommunicationListener.h
//  Implementation of the Class AliasCommunicationListener
//  Created on:      07-gru-2012 00:33:32
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_B3F6A841_8C44_4944_A884_DC8E2DEBD789__INCLUDED_)
#define EA_B3F6A841_8C44_4944_A884_DC8E2DEBD789__INCLUDED_

namespace TIN_project {

namespace Utilities {
	class AliasFileList;
	class FileLocation;
}

namespace Client {

class AliasCommunicationListener
{

public:
	AliasCommunicationListener();
	virtual ~AliasCommunicationListener();

	virtual void onAliasListed(Utilities::AliasFileList list) =0;
	virtual void onFileFound(Utilities::FileLocation location) =0;
	virtual void onFileNotFound() =0;
	virtual void onFileRemoved() =0;
	virtual void onFileRemovingError() =0;
	virtual void onFileTransferError() =0;
	virtual void onFileTransferFinished() =0;
	virtual void onFileTransferStarted() =0;

};

}//namespace Client
}//namespace TIN_project

#endif // !defined(EA_B3F6A841_8C44_4944_A884_DC8E2DEBD789__INCLUDED_)
