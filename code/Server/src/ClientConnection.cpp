///////////////////////////////////////////////////////////
//  ClientConnection.cpp
//  Implementation of the Class ClientConnection
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientConnection.h"
#include "FileLocation.h"
#include "AliasFileList.h"

namespace TIN_project {
namespace Server {

ClientConnection::ClientConnection(){

}



ClientConnection::~ClientConnection(){

}





void ClientConnection::disconnectFromServer(){

}


void ClientConnection::sendFileFound(Utilities::FileLocation location){

}


void ClientConnection::sendFileList(Utilities::AliasFileList list){

}


void ClientConnection::sendFileNotFound(){

}


void ClientConnection::sendFileNotRemoved(){

}


void ClientConnection::sendFileRemoved(){

}


void ClientConnection::sendFileTransferError(){

}


void ClientConnection::sendFileTransferFinished(){

}


void ClientConnection::sendFileTransferStarted(){

}


void ClientConnection::sendNoSuchFile(){

}

}//namespace server
}//namespace TIN_project
