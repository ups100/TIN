///////////////////////////////////////////////////////////
//  ClientServerConnectionListener.h
//  Implementation of the Class ClientServerConnectionListener
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_95DC1A3C_38E6_4071_B16C_8876A967096B__INCLUDED_)
#define EA_95DC1A3C_38E6_4071_B16C_8876A967096B__INCLUDED_

#include <QString>
#include <boost/shared_ptr.hpp>

namespace TIN_project {

namespace Utilities {
class Password;
}
namespace Server {

class ClientServerConnectionListener
{

public:
    ClientServerConnectionListener();
    virtual ~ClientServerConnectionListener();

    virtual void onConnectToAlias(boost::shared_ptr<QString> name,
            boost::shared_ptr<Utilities::Password> password) =0;
    virtual void onCreateAlias(boost::shared_ptr<QString> name,
            boost::shared_ptr<Utilities::Password> password) =0;
    virtual void onRemoveAlias(boost::shared_ptr<QString> name,
            boost::shared_ptr<Utilities::Password> password) =0;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_95DC1A3C_38E6_4071_B16C_8876A967096B__INCLUDED_)
