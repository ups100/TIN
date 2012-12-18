/**
 * @file AliasCommunicationListener.h
 *
 * @date 08-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Client::AliasCommunicationListener
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_B3F6A841_8C44_4944_A884_DC8E2DEBD789__INCLUDED_)
#define EA_B3F6A841_8C44_4944_A884_DC8E2DEBD789__INCLUDED_

#include <boost/shared_ptr.hpp>

namespace TIN_project {

namespace Utilities {
class AliasFileList;
class FileLocation;
}

namespace Client {

/**
 * @brief Class used as interface for communication by ServerConnection class
 *
 * @details This class is used for notifying about communicates
 * incoming directly from alias.
 *
 * @warning Remember that all those methods are called in context of ServerConnection's thread.
 */
class AliasCommunicationListener
{

public:

    /**
     * @brief Constructor
     */
    AliasCommunicationListener();

    /**
     * @brief Destructor
     */
    virtual ~AliasCommunicationListener();

    /**
     * @brief Informs that alias contention list has arrived.
     *
     * @param list contention of alias
     */
    virtual void onAliasListed(
            boost::shared_ptr<Utilities::AliasFileList> list) = 0;

    /**
     * @brief Informs that searched file has been found.
     *
     * @param location location of found file
     */
    virtual void onFileFound(
            boost::shared_ptr<Utilities::FileLocation> location) = 0;

    /**
     * @brief Informs that searched file has not been found.
     */
    virtual void onFileNotFound() = 0;

    /**
     * @brief Informs that all occurrences of file has been removed
     */
    virtual void onFileRemoved() = 0;

    /**
     * @brief Informs that error occurred while removing file
     * (especially file does not exist)
     */
    virtual void onFileRemovingError() = 0;

    /**
     * @brief Informs that an error occurred while trying
     * to transfer some file
     */
    virtual void onFileTransferError() = 0;

    /**
     * @brief Informs that file transfer has been finished.
     */
    virtual void onFileTransferFinished() = 0;

    /**
     * @brief Informs that file transfer has been started.
     */
    virtual void onFileTransferStarted() =0;

};

} //namespace Client
} //namespace TIN_project

#endif // !defined(EA_B3F6A841_8C44_4944_A884_DC8E2DEBD789__INCLUDED_)
