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

Commands::Commands(QString command, QString parameter, QString argument, Argument::Types flag)
{
    qDebug()<<"TU JESTESMY"<<endl;
    flague = flag;
    m_command = command;
    m_parameter = parameter;
    m_argument = Argument(argument,flag);
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

Argument Commands::getArg()
{
    return m_argument;
}



Argument::Types Commands::getFlague()
{
    return flague;
}



Commands::~Commands()
{

}


} //namespace Client
} //namespace TIN_project
