///////////////////////////////////////////////////////////
//  DaemonCommunication.h
//  Implementation of the Class DaemonCommunication
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_01D47BB0_E0AE_4403_9C29_EFF62B888A7D__INCLUDED_)
#define EA_01D47BB0_E0AE_4403_9C29_EFF62B888A7D__INCLUDED_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/unistd.h>

namespace TIN_project {
namespace Utilities {
class Message;
}
namespace Client {

class DaemonCommunication
{
public:
    DaemonCommunication();
    virtual ~DaemonCommunication();

    void talkToDaemon(Utilities::Message message);
};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_01D47BB0_E0AE_4403_9C29_EFF62B888A7D__INCLUDED_)
