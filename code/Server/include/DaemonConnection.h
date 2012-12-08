///////////////////////////////////////////////////////////
//  DaemonConnection.h
//  Implementation of the Class DaemonConnection
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_)
#define EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_

#include <QHostAddress>
#include <QString>

namespace TIN_project {
namespace Server {

class DaemonConnectionListener;

/**
 * Represents single connection with daemon
 */
class DaemonConnection
{

public:
    DaemonConnection();
    virtual ~DaemonConnection();

    void disconnectFromServer();
    void sendFindFile(const QString& fileName);
    void sendReciveFile(const QString& filename, const QHostAddress& address,
            quint16 port);
    void sendRemoveFile(const QString& fileName);
    void sendSendFile(const QString& fileName, const QHostAddress& address,
            quint16 port);

private:
    DaemonConnectionListener* m_connectionListener;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_6C6268DD_2EDB_44a9_98C4_B9F5505AFD3B__INCLUDED_)
