///////////////////////////////////////////////////////////
//  DaemonApplication.h
//  Implementation of the Class DaemonApplication
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_)
#define EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_

#include "ClientCommunication.h"
#include "DaemonThread.h"

class DaemonApplication
{

public:
	DaemonApplication();
	virtual ~DaemonApplication();
	ClientCommunication *m_ClientCommunication;
	DaemonThread *m_DaemonThread;

	void start();

private:
	QList<DaemonThread> m_daemonThreads;

};
#endif // !defined(EA_E9B53BF6_B582_49b8_8FDE_DA96921106D3__INCLUDED_)
