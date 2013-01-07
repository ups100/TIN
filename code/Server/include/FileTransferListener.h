/**
 * @file Server/include/FileTransferListener.h
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

class FileTransferServer;

/**
 * @brief Interface for informing about file transfer state
 *
 * @note Those functions are called in context of thread in which FileTransferServer
 * has been created
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

    /**
     * @brief Informs that file transfer has been completed.
     *
     * @param transfer FileTransferServer identity
     *
     * @warning Do not delete FileTransferServer in this method!
     *  Use slot instead and do it there!
     */
    virtual void onFileTransferCompleted(FileTransferServer *transfer) = 0;

    /**
     * @brief Informs that some error occurred during file transfering
     *
     * @param transfer FileTransferServer identity
     *
     * @warning Do not delete FileTransferServer in this method!
     *  Use slot instead and do it there!
     */
    virtual void onFileTransferError(FileTransferServer *transfer) = 0;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_01367691_430E_42ec_8D5F_87C8F2B45C82__INCLUDED_)
