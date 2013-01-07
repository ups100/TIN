/**
 * @file FileReciver.h
 *
 * @date 07-01-2013
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::FileSender
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_)
#define EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_

#include <QHostAddress>
#include <boost/shared_ptr.hpp>

#include "File.h"

namespace TIN_project {
namespace Daemon {

class FileTransferListener;

/**
 * @brief Class used for file receiving
 *
 * @details This class runs in separate thread.
 */
class FileReciver
{

public:
    /**
     * @brief Destructor
     */
    virtual ~FileReciver();
    FileTransferListener *m_FileTransferListener;

    FileReciver(const QHostAddress& address, quint16 port,
            boost::shared_ptr<File> file);

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_723305C7_1A82_4b81_8DF6_2EBC4C2063C7__INCLUDED_)
