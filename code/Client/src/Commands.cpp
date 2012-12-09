///////////////////////////////////////////////////////////
//  Commands.cpp
//  Implementation of the Class Commands
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "Commands.h"

namespace TIN_project {
namespace Client {

Commands::Commands()
{

}

Commands::Commands(QStringList list)
{

}

Commands::Commands(QString cmd, QString param, QStringList args)
{
    m_command = cmd;
    m_parameter = param;
    m_arguments = args;
}

/**
 * @brief getter for the command
 * @return command
 */
QString Commands::getCommand()
{
    return m_command;
}

/**
 * @brief getter for the parameter
 * @return parameter
 */
QString Commands::getParameter()
{
   return m_parameter;
}

/**
 * @brief getter for one argument
 * @param i index of argument (starts with 0)
 * @return one particular argument
 */
QString Commands::getArg(int i)
{
    if ((i >= m_arguments.size()) || (i < 0)) {
        return NULL;
    }
    return m_arguments[i];
}

/**
 * @brief getter for list of arguments
 * @return list of all arguments
 */
QStringList Commands::getArg()
{
    return m_arguments;
}

Commands::~Commands()
{

}


} //namespace Client
} //namespace TIN_project
