///////////////////////////////////////////////////////////
//  DaemonConnectionListener.h
//  Implementation of the Class DaemonConnectionListener
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_AE18438A_08E8_4911_BEEF_91902EC822A3__INCLUDED_)
#define EA_AE18438A_08E8_4911_BEEF_91902EC822A3__INCLUDED_

#include "DaemonConnection.h"

/**
 * Interface for handling connection with daemon
 */
class DaemonConnectionListener
{

public:
	DaemonConnectionListener();
	virtual ~DaemonConnectionListener();
	DaemonConnection *m_DaemonConnection;

	virtual void onConnectionClose(DaemonConnection* daemon) =0;
	virtual void onFileFound(DaemonConnection* daemon, FileLocation location) =0;
	virtual void onFileList(DaemonConnection* daemon, AliasFileList list) =0;
	virtual void onNoSuchFile(DaemonConnection* daemon) =0;

};
#endif // !defined(EA_AE18438A_08E8_4911_BEEF_91902EC822A3__INCLUDED_)
