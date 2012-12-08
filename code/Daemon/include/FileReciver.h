///////////////////////////////////////////////////////////
//  FileReciver.h
//  Implementation of the Class FileReciver
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_)
#define EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_

#include <QHostAddress>
#include <boost/shared_ptr.hpp>

#include "File.h"

namespace TIN_project {
namespace Daemon {

class FileTransferListener;
/**
 * Used for receiving files
 */
class FileReciver
{

public:
    FileReciver();
    virtual ~FileReciver();
    FileTransferListener *m_FileTransferListener;

    FileReciver(const QHostAddress& address, quint16 port,
            boost::shared_ptr<File> file);

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_)
