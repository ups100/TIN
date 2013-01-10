/**
 * @file UnknownConnectionListener.h
 *
 * @date 22-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Server::UnknownConnectionListener
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_DB7A2236_3D5B_4f08_81C5_037266859012__INCLUDED_)
#define EA_DB7A2236_3D5B_4f08_81C5_037266859012__INCLUDED_

#include <QString>

namespace TIN_project {
namespace Utilities {
class Identifier;
class Password;
}

namespace Server {

class UnknownConnection;

/**
 * @brief Class used as interface for communication by ServerConnection class
 *
 * @details This class is used for notifying about communicates
 * incoming directly from client or daemon.
 *
 * @warning Remember that all those methods are called in context of main server thread.
 */
class UnknownConnectionListener
{

public:
    /**
     * @brief Constructor
     */
    UnknownConnectionListener();

    /**
     * @brief Destructor
     */
    virtual ~UnknownConnectionListener();

    /**
     * @brief Informs that connection has been closed
     */
    virtual void onConnectionClosed(UnknownConnection *connection) = 0;

    /**
     * @brief Informs that message to connect new daemon to some alias has been received
     *
     * @param[in] connection identifier
     *
     * @param[in] aliasName name of alias
     *
     * @param[in] password to alias
     *
     * @param[in] id identity of daemon
     */
    virtual void onAddDirecotry(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password, const Utilities::Identifier& id) = 0;

    /**
     * @brief Informs that message to connect new client to some alias has been received
     *
     * @param[in] connection identifier
     *
     * @param[in] aliasName name of alias
     *
     * @param[in] password to alias
     *
     * @param[in] id identity of daemon
     */
    virtual void onConnectToAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password, const Utilities::Identifier& id) = 0;

    /**
     * @brief Informs that message to create new alias has been received
     *
     * @param[in] connection identifier
     *
     * @param[in] aliasName name of alias
     *
     * @param[in] password to alias
     */
    virtual void onCreateAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password) = 0;

    /**
     * @brief Informs that message to remove alias has been received
     *
     * @param[in] connection identifier
     *
     * @param[in] aliasName name of alias
     *
     * @param[in] password to alias
     */
    virtual void onRemoveAlias(UnknownConnection *connection,
            const QString &aliasName, const Utilities::Password &password) = 0;

};

} //namespace Server
} //namespace TIN_project

#endif // !defined(EA_DB7A2236_3D5B_4f08_81C5_037266859012__INCLUDED_)
