/**
 * @file ClientConnectionListener.h
 *
 * @date 31-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::ClientConnectionListener
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_AE18438A_08E8_4911_BEEF_91902EC822A3__INCLUDED_)
#define EA_AE18438A_08E8_4911_BEEF_91902EC822A3__INCLUDED_

#include <boost/shared_ptr.hpp>

namespace TIN_project {

namespace Utilities {
class FileLocation;
class AliasFileList;
}
namespace Server {

class DaemonConnection;

/**
 * @brief Class used as interface for communication by DaemonConnection class
 *
 * @details This class is used for notifying about communicates
 * incoming directly from daemon.
 *
 * @warning Remember that all those methods are called in context of alias's thread.
 */
class DaemonConnectionListener
{

public:

    /**
     * @brief Constructor
     */
    DaemonConnectionListener();

    /**
     * @brief Destructor
     */
    virtual ~DaemonConnectionListener();

    /**
     * @brief Informs that connection with client has been closed
     *
     * @param daemon identity
     */
    virtual void onConnectionClosed(DaemonConnection* daemon) = 0;

    /**
     * @brief Informs that searched file has been found
     *
     * @param daemon identity
     *
     * @param[in] location of file
     */
    virtual void onFileFound(DaemonConnection* daemon,
            const Utilities::FileLocation& location) = 0;

    /**
     * @brief Informs about content of folder handled by daemon
     *
     * @param daemon identity
     *
     * @param[in] list of files
     */
    virtual void onFileList(DaemonConnection* daemon,
            const Utilities::AliasFileList& list) = 0;

    /**
     * @brief Informs that searched file has not been found
     *
     * @param daemon identity
     */
    virtual void onNoSuchFile(DaemonConnection* daemon) = 0;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_AE18438A_08E8_4911_BEEF_91902EC822A3__INCLUDED_)
