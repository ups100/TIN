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

Commands::Commands(QString command, QString parameter, QStringList arguments, Argument::Types flag)
{
    flague = flag;
    m_command = command;
    m_parameter = parameter;
    for(int i = 0; i < arguments.size(); ++i) {
        m_arguments.push_back(Argument(arguments[i], flag));
    }
    /** Test if all arguments are of the same type */
    for(int i = 0; i < m_arguments.size()-1; ++i)
    {
        if(m_arguments[i].getFlague() != m_arguments[i+1].getFlague()) {
            qDebug()<<"BAD FLAGUES"<<endl;
        }
    }
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
Argument Commands::getArg(int i)
{
    if ((i >= m_arguments.size()) || (i < 0)) {
        qDebug()<<"BAD PARAMETER"<<endl;
    }
    return m_arguments[i];
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
