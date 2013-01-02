/**
 * @file Client/include/ServerConnectionListener.h
 *
 * @date 08-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Client::ServerConnectionListener
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_)
#define EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_

#include "ServerConnection.h"

namespace TIN_project {
namespace Client {

/**
 * @brief Class used as interface for communication by ServerConnection class
 *
 * @details This class is used for notifying about communicates
 * incoming directly from server.
 *
 * @warning Remember that all those methods are called in context of ServerConnection's thread.
 */
class ServerConnectionListener
{

public:

    /**
     * @brief Constructor
     */
    ServerConnectionListener();

    /**
     * @brief Destructor
     */
    virtual ~ServerConnectionListener();

    /**
     * @brief Informs that connection with alias
     * has been established
     */
    virtual void onAliasConnected() = 0;

    /**
     * @brief Informs that some error occurred while trying
     * to connect to alias
     */
    virtual void onAliasConnectionError() = 0;

    /**
     * @brief Informs that alias has been created.
     */
    virtual void onAliasCreated() = 0;

    /**
     * @brief Informs that some error occurred while trying
     * to create new alias
     */
    virtual void onAliasCreationError() = 0;

    /**
     * @brief Informs that alias has been removed from server.
     */
    virtual void onAliasDeleted() = 0;

    /**
     * @brief Informs that some error occurred while trying to remove alias
     * from server (especially no such alias or wrong password)
     */
    virtual void onAliasDeletionError() = 0;

    /**
     * @brief Informs that connection with server has been established.
     */
    virtual void onConnected() = 0;

    /**
     * @brief Informs that connection with server has been closed or
     * some connection error occurred.
     */
    virtual void onDisconnected() = 0;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_736FDE0B_EC7C_485e_BF18_247829D94E66__INCLUDED_)
