///////////////////////////////////////////////////////////
//  ServerConnection.cpp
//  Implementation of the Class ServerConnection
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ServerConnection.h"


ServerConnection::ServerConnection(){

}



ServerConnection::~ServerConnection(){

}





void ServerConnection::connectToAlias(QString aliasName, Password password){

}


void ServerConnection::connectToServer(QHostAddress address, quint16 port){

}


void ServerConnection::createAlias(QString name, Password password){

}


void ServerConnection::findFileInAlias(QString fileName){

}


void ServerConnection::listAlias(){

}


void ServerConnection::pullFileFrom(FileLocation file){

}


/**
 * not sure about semantic of this method
 */
void ServerConnection::pushFileToAlias(QString path){

}


/**
 * not sure about semantic of this method 
 */
void ServerConnection::removeFileFromAlias(QString fileName){

}