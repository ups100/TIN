///////////////////////////////////////////////////////////
//  ClientCommunication.h
//  Implementation of the Class ClientCommunication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_)
#define EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_

#include <boost/shared_ptr.hpp>

namespace TIN_project {
namespace Utilities {
class Message;
}
namespace Daemon {

class ClientCommunication
{

public:
    ClientCommunication();
    virtual ~ClientCommunication();

    boost::shared_ptr<Utilities::Message> getMessage();

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_12038366_02C4_491b_8F39_E9A6E75F2374__INCLUDED_)
