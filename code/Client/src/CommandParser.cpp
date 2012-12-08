///////////////////////////////////////////////////////////
//  CommandParser.cpp
//  Implementation of the Class CommandParser
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "CommandParser.h"
#include "Commands.h"

namespace TIN_project {
namespace Client {

CommandParser::CommandParser()
{

}

CommandParser::~CommandParser()
{

}

boost::shared_ptr<Commands> parseCommand(const QString& command)
{

    return boost::shared_ptr<Commands>((Commands*) 0L);
}

} //namespace Client
} //namespace TIN_project
