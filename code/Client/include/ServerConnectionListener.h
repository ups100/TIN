///////////////////////////////////////////////////////////
//  ServerConnectionListener.h
//  Implementation of the Class ServerConnectionListener
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_)
#define EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_

#include "ServerConnection.h"

namespace TIN_project {
namespace Client {

class ServerConnectionListener
{

public:
    ServerConnectionListener();
    virtual ~ServerConnectionListener();

    virtual void onAliasConnected() =0;
    virtual void onAliasConnectionError() =0;
    virtual void onAliasCreated() =0;
    virtual void onAliasCreationError() =0;
    virtual void onAliasDeleted() =0;
    virtual void onAliasDeletionError() =0;
    virtual void onConnected() =0;
    virtual void onDisconnected() =0;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_)
