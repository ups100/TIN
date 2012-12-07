///////////////////////////////////////////////////////////
//  DaemonThread.cpp
//  Implementation of the Class DaemonThread
//  Created on:      07-gru-2012 00:33:35
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "DaemonThread.h"

namespace TIN_project {
namespace Daemon {

DaemonThread::DaemonThread(){

}



DaemonThread::~DaemonThread(){

}





DaemonThread::DaemonThread(QHostAddress ip, quint16 port, QString path){

}


void DaemonThread::onAliasConnected(){

}


void DaemonThread::onAliasConnectionError(){

}


void DaemonThread::onConnected(){

}


void DaemonThread::onDisconnected(){

}


void DaemonThread::onFindFile(QString fileName){

}


void DaemonThread::onListFiles(){

}


void DaemonThread::onReciveFile(File file){

}


void DaemonThread::onRemoveFile(QString fileName){

}


void DaemonThread::onSendFile(File file){

}


void DaemonThread::onTransferEnd(FileSender * sender){

}


void DaemonThread::onTransferEnd(FileReciver * reciver){

}


void DaemonThread::stopThread(){

}

}//namespace Daemon
}//namespace TIN_project
