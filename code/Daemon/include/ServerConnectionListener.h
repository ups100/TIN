///////////////////////////////////////////////////////////
//  ServerConnectionListener.h
//  Implementation of the Class ServerConnectionListener
//  Created on:      07-gru-2012 00:33:38
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_)
#define EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_

#include <QString>
#include <boost/shared_ptr.hpp>

#include "File.h"

namespace TIN_project {
namespace Daemon {

class ServerConnectionListener
{

public:
    ServerConnectionListener();
    virtual ~ServerConnectionListener();

    virtual void onAliasConnected() =0;
    virtual void onAliasConnectionError() =0;
    virtual void onConnected() =0;
    virtual void onDisconnected() =0;
    virtual void onFileNotRemoved() =0;
    virtual void onFindFile(boost::shared_ptr<QString> fileName) =0;
    virtual void onListFiles() =0;
    virtual void onReciveFile(boost::shared_ptr<File> file) =0;
    virtual void onRemoveFile(boost::shared_ptr<QString> fileName) =0;
    virtual void onSendFile(boost::shared_ptr<File> file) =0;

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_0CA2CADB_F467_42f7_B3EB_F7C3C8870391__INCLUDED_)
