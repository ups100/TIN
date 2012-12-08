///////////////////////////////////////////////////////////
//  MainServer.cpp
//  Implementation of the Class MainServer
//  Created on:      07-gru-2012 00:33:37
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "MainServer.h"
#include "Password.h"

namespace TIN_project {
namespace Server {

MainServer::MainServer()
{

}

MainServer::~MainServer()
{

}

void MainServer::onConnectToAlias(boost::shared_ptr<QString> name,
        boost::shared_ptr<Utilities::Password> password)
{

}

void MainServer::onCreateAlias(boost::shared_ptr<QString> name,
        boost::shared_ptr<Utilities::Password> password)
{

}

void MainServer::onRemoveAlias(boost::shared_ptr<QString> name,
        boost::shared_ptr<Utilities::Password> password)
{

}

void MainServer::start(const QHostAddress& address, quint16 port)
{

}

} //namespace server
} //namespace TIN_project
