///////////////////////////////////////////////////////////
//  CommandParser.h
//  Implementation of the Class CommandParser
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

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

    CommandParser();
    virtual ~CommandParser();

    /**
     * @brief translate string command to Commands object
     * @param[in] cmd command that should be create
     * @return Commands object
     * @warning if the Commands m_correct variable is false, Command should not be invoked
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
            };
    /**
     * @brief list of all possible commands
     */
    QStringList commandList;

};

} //namespace Client
} //namespace TIN_project

#endif // !defined(EA_7D25CB8A_5FE7_4422_AE99_A2C2A1D6102D__INCLUDED_)
