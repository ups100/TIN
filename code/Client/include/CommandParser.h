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

namespace TIN_project {
namespace Client {

class Commands;
class CommandParser
{

public:
    CommandParser();
    virtual ~CommandParser();

    boost::shared_ptr<Commands> parseCommand(QString const&);

private:
    enum commandListEnum
    {
        LOG,
        CREATE_ALIAS,
        ADD_TO_ALIAS,
        REMOVE,
        FIND,
        READ,
        SYNCH,
        LS,
        PUSH,
        PULL,
        ADD
    };
    QStringList commandList;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_7D25CB8A_5FE7_4422_AE99_A2C2A1D6102D__INCLUDED_)
