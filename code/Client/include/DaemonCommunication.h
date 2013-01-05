/**
 * @file DaemonCommunication.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Client::DaemonCommunication
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_01D47BB0_E0AE_4403_9C29_EFF62B888A7D__INCLUDED_)
#define EA_01D47BB0_E0AE_4403_9C29_EFF62B888A7D__INCLUDED_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/unistd.h>
#include <QByteArray>

namespace TIN_project {
namespace Utilities {
class Message;
}
namespace Client {

class DaemonCommunication
{
public:
    /**
     * @brief C-tor
     */
    DaemonCommunication();

    /**
     * @brief D-tor
     */
    virtual ~DaemonCommunication();

    /**
     * @brief Open socket and connect to daemon.
     * Send some message to daemon
     * @note If connection fails, start DaemonApplication
     * @param message Message to send
     * @throw exception if no daemon listening for a message or writing error
     */
    void talkToDaemon(const QByteArray &message);
};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_01D47BB0_E0AE_4403_9C29_EFF62B888A7D__INCLUDED_)
