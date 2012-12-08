///////////////////////////////////////////////////////////
//  ClientConnectionListener.h
//  Implementation of the Class ClientConnectionListener
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_7F35A60D_33DF_46e4_92ED_F054E1BB612F__INCLUDED_)
#define EA_7F35A60D_33DF_46e4_92ED_F054E1BB612F__INCLUDED_

#include <QString>
#include <boost/shared_ptr.hpp>

namespace TIN_project {

namespace Utilities {
class FileLocation;
}
namespace Server {

class ClientConnection;
/**
 * Class for handling connection wit client
 */
class ClientConnectionListener
{

public:
    ClientConnectionListener();
    virtual ~ClientConnectionListener();

    virtual void onConnectionClose(ClientConnection* client) =0;
    virtual void onFindFile(ClientConnection* client,
            boost::shared_ptr<QString> name) =0;
    virtual void onListAlias(ClientConnection* client) =0;
    virtual void onPullFileFrom(ClientConnection* client,
            boost::shared_ptr<Utilities::FileLocation> location) =0;
    virtual void onPushFileToAlias(ClientConnection* client,
            boost::shared_ptr<QString> path, quint64 size) =0;
    virtual void onRemoveFromAlias(ClientConnection* client,
            boost::shared_ptr<QString> fileName) =0;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_7F35A60D_33DF_46e4_92ED_F054E1BB612F__INCLUDED_)
