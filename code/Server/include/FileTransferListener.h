/**
 * @file FileTransferListener.h
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::FileTransferListener
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_01367691_430E_42ec_8D5F_87C8F2B45C82__INCLUDED_)
#define EA_01367691_430E_42ec_8D5F_87C8F2B45C82__INCLUDED_

namespace TIN_project {
namespace Server {

/**
 * @brief Interface for informing about file transfer state
 */
class FileTransferListener
{

public:
    /**
     * @brief Constructor
     */
    FileTransferListener();

    /**
     * @brief Destructor
     */
    virtual ~FileTransferListener();

    virtual void onFileTransferStarted(FileTransferServer *transfer) = 0;
    virtual void onFileTransferCompleted(FileTransferServer *transfer) = 0;
    virtual void onFileTransferError(FileTransferServer *transfer) = 0;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_01367691_430E_42ec_8D5F_87C8F2B45C82__INCLUDED_)
