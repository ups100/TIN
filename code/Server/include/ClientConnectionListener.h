/**
 * @file ClientConnectionListener.h
 *
 * @date 23-12-2012
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

#if !defined(EA_7F35A60D_33DF_46e4_92ED_F054E1BB612F__INCLUDED_)
#define EA_7F35A60D_33DF_46e4_92ED_F054E1BB612F__INCLUDED_

#include <QString>

namespace TIN_project {

namespace Utilities {
class FileLocation;
}
namespace Server {

class ClientConnection;

/**
 * @brief Class used as interface for communication by ClientConnection class
 *
 * @details This class is used for notifying about communicates
 * incoming directly from client.
 *
 * @warning Remember that all those methods are called in context of alias's thread.
 */
class ClientConnectionListener
{

public:
    /**
     * @brief Constructor
     */
    ClientConnectionListener();

    /**
     * @brief Destructor
     */
    virtual ~ClientConnectionListener();

    /**
     * @brief Informs that connection with client has been closed
     *
     * @param client identity
     */
    virtual void onConnectionClosed(ClientConnection* client) = 0;

    /**
     * @brief Informs that client would like to look for some file
     *
     * @param client identity
     *
     * @param[in] name of file to look for
     */
    virtual void onFindFile(ClientConnection* client, const QString& name) = 0;

    /**
     * @brief Informs that client would like to list content of alias
     *
     * @param client identity
     *
     * @param remoteOnly is flag which tells
     * to list only remote file to this client
     *
     */
    virtual void onListAlias(ClientConnection* client, bool remoteOnly) = 0;

    /**
     * @brief Informs that client would like to pull some file
     *
     * @param client identity
     *
     * @param[in] location of file to be pulled
     */
    virtual void onPullFileFrom(ClientConnection* client,
            const Utilities::FileLocation& location) = 0;

    /**
     * @brief Informs that client would like to push some file to alias
     *
     * @param client identity
     *
     * @param[in] path relative path of file
     *
     * @param[in] size of file
     */
    virtual void onPushFileToAlias(ClientConnection* client,
            const QString& path, quint64 size) = 0;

    /**
     * @brief Informs that client would like to remove some file from alias.
     *
     * @param client identity
     *
     * @param[in] fileName name of file to be removed
     */
    virtual void onRemoveFromAlias(ClientConnection* client,
            const QString& fileName) = 0;

};

} //namespace server
} //namespace TIN_project
#endif // !defined(EA_7F35A60D_33DF_46e4_92ED_F054E1BB612F__INCLUDED_)
