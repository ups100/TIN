///////////////////////////////////////////////////////////
//  ClientCommunication.cpp
//  Implementation of the Class ClientCommunication
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientCommunication.h"
#include "Message.h"

namespace TIN_project {
namespace Daemon {

ClientCommunication::ClientCommunication()
{

}

ClientCommunication::~ClientCommunication()
{

}

boost::shared_ptr<Utilities::Message> ClientCommunication::getMessage()
{

    return boost::shared_ptr<Utilities::Message>((Utilities::Message*) 0L);
}

} //namespace Daemon
} //namespace TIN_project
