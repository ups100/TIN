///////////////////////////////////////////////////////////
//  DaemonThread.h
//  Implementation of the Class DaemonThread
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_)
#define EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_

#include "FileTransferListener.h"
#include "ServerConnectionListener.h"
#include "ServerConnection.h"

class DaemonThread : public FileTransferListener, public ServerConnectionListener
{

public:
	DaemonThread();
	virtual ~DaemonThread();
	ServerConnection *m_ServerConnection;

	void DaemonThread(QHostAddress ip, quint16 port, QString path);
	virtual void onAliasConnected();
	virtual void onAliasConnectionError();
	virtual void onConnected();
	virtual void onDisconnected();
	virtual void onFindFile(QString fileName);
	virtual void onListFiles();
	virtual void onReciveFile(File file);
	virtual void onRemoveFile(QString fileName);
	virtual void onSendFile(File file);
	void onTransferEnd(FileSender * sender);
	void onTransferEnd(FileReciver * reciver);
	void stopThread();

private:
	QString m_path;

};
#endif // !defined(EA_864A61DB_9A5A_400d_A0D2_DCA5A718FEDA__INCLUDED_)
