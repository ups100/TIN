/**
 * @file FileTransferListener.h
 *
 * @date 05-01-2013
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::FileTransferServer
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_CDCA04F0_86FE_4b54_BBD9_F094F10EFB6E__INCLUDED_)
#define EA_CDCA04F0_86FE_4b54_BBD9_F094F10EFB6E__INCLUDED_

namespace TIN_project {
namespace Daemon {

class FileSender;
class FileReciver;

/**
 * @brief Interface for informing about file transfer status
 *
 * @note All those functions are called in context of thread which created FileSender or FileReciver
 */
class FileTransferListener
{
public:

    /**
     * @brief Constructor
     */
    FileTransferListener();

    /**
     * @brief Constructor
     */
    virtual ~FileTransferListener();

    /**
     * @brief Informs that file upload has been finished
     *
     * @param sender transfer identification
     */
    virtual void onTransferEnd(FileSender *sender) = 0;

    /**
     * @brief Informs that error occurred during file uploading
     *
     * @param sender transfer identification
     */
    virtual void onTransferError(FileSender *sender) = 0;

    /**
     * @brief Informs that file receiving has been finished
     *
     * @param receiver transfer identification
     */
    virtual void onTransferEnd(FileReciver *receiver) = 0;

    /**
     * @brief Informs that error occurred during file receiving
     *
     * @param receiver transfer identification
     */
    virtual void onTransferError(FileReciver *receiver) = 0;
};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(EA_CDCA04F0_86FE_4b54_BBD9_F094F10EFB6E__INCLUDED_)
