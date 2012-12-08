///////////////////////////////////////////////////////////
//  MainServer.h
//  Implementation of the Class MainServer
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_)
#define EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_

#include <QHostAddress>
#include <QString>

#include "ClientServerConnectionListener.h"
#include "Alias.h"

namespace TIN_project {
namespace Utilities {
class Password;
}
namespace Server {

class MainServer : public ClientServerConnectionListener
{

public:
    MainServer();
    virtual ~MainServer();

    virtual void onConnectToAlias(boost::shared_ptr<QString> name,
            boost::shared_ptr<Utilities::Password> password);
    virtual void onCreateAlias(boost::shared_ptr<QString> name,
            boost::shared_ptr<Utilities::Password> password);
    virtual void onRemoveAlias(boost::shared_ptr<QString> name,
            boost::shared_ptr<Utilities::Password> password);
    void start(const QHostAddress& address, quint16 port);

private:
    QList<Alias> m_aliases;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_6F9FF3A4_91E1_4b50_9D38_C64AB3756BB5__INCLUDED_)
