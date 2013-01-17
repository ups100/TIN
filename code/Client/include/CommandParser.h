/**
 * @file CommandParser.h
 *
 * @date 04-01-2013
 *
 * @author Marcin Kubik <markubik@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Client::CommandParser
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_7D25CB8A_5FE7_4422_AE99_A2C2A1D6102D__INCLUDED_)
#define EA_7D25CB8A_5FE7_4422_AE99_A2C2A1D6102D__INCLUDED_

#include <QString>
#include <QStringList>
#include <boost/shared_ptr.hpp>
#include "Commands.h"
namespace TIN_project {
namespace Client {


class CommandParser
{

public:

    /**
     * @brief Constructor
     */
    CommandParser();

    /**
     * @brief Destructor
     */
    virtual ~CommandParser();


    /**
     *  @brief translate string with command into object of Command class
     *  @details split string into 3 segments: command, parameter, arguments
     *  @param cmd string received from client
     *  @return pointer to object of class Commands
     *  @note returns "Wrong" command if the command is not correct
     *  @note see "syntax.txt" for available commands
     */
    boost::shared_ptr<Commands> parseCommand(QString const& cmd);

private:
    enum commandListEnum
            {
                CONNECT_TO_ALIAS,
                CREATE,
                ADD,
                REMOVE,
                FIND,
                READ,
                PUSH,
                PULL,
                LS,
                SYNCH,
                CHOOSE,
                DISCONNECT,
                EXIT,
                CHANGE
            };
    /**
     * @brief list of all possible commands
     */
    QStringList commandList;

};

} //namespace Client
} //namespace TIN_project

#endif // !defined(EA_7D25CB8A_5FE7_4422_AE99_A2C2A1D6102D__INCLUDED_)
