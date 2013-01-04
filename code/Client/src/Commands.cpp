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
    m_correct = false;
    qDebug()<<"NIE UDALO SIE"<<endl;
}

Commands::Commands(QString command, Argument::Types f)
{
    //LS or DISCONNECT
    m_correct = true;
    m_flague = f;
    m_command = command;
    shout();
}

Commands::Commands(QString command, QString arg1, Argument::Types f)
{
    m_flague = f;
    m_command = command;
    m_correct = true;
    switch(f) {
        //ADD, RM , PUSH
        case Argument::REL_PATH:
            m_argument = Argument(arg1,f);
            break;

        //FIND
        case Argument::NAME:
            m_argument = Argument(arg1,f);
            break;

        //READ, CHOOSE
        case Argument::NUMBER:
            m_argument = Argument(arg1,f);
            break;

        //SYNCH
        case Argument::SYNCH:
            m_parameter = arg1;
            break;


        default:
            qDebug()<<"Something went wrong: Commands(QString, QString, Argument::Types"<<endl;
            qDebug()<<"Commands object was created, but is invalid"<<endl;
            m_correct = false;
            break;
    }
    shout();
}

Commands::Commands(QString command, QString arg1, QString arg2, Argument::Types f)
{
    m_flague = f;
    m_command = command;
    m_correct = true;
    switch (f) {
        //LOG and CREATE
        case Argument::ALIAS:
            m_argument = Argument(arg1,f);
            m_password = Password(arg2);
            break;
        //RM
        case Argument::REL_PATH:
            m_parameter = arg1;
            m_argument = Argument(arg2,f);
            break;
        default:
            qDebug()<<"Something went wrong: Commands(QString, QString, QString, Argument::Types"<<endl;
            qDebug()<<"Commands object was created, but is invalid"<<endl;
            m_correct = false;
            break;
    }
    shout();
}


Commands::Commands(QString command, QString arg1, QString arg2, QString arg3, Argument::Types f)
{
    m_flague = f;
    m_command = command;
    m_correct = true;
    if(f == Argument::ALIAS)
    {
        m_parameter = arg1;
        m_argument = Argument(arg2,f);
        m_password = Password(arg3);
    }
    else qDebug()<<"Something went wrong"<<endl;
    shout();
}

bool Commands::invoke()
{
    if (!m_correct) {
        qDebug()<<"You can't invoke this command, because is incorrect"<<endl;
        return false;
    }
    //TODO don't know what to do here exactly
    return true;
}

/**
 * @brief getter for the command
 * @return command
 */
QString Commands::getCommand()
{
    if (m_correct) {
        return m_command;
    }
    else return NULL;
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
 * @brief getter for the argument
 * @return QString
 */
QString Commands::getArg(Argument::Types f) const
{
    return m_argument.getData();
}

/**
 * @brief getter for the name of an alias
 * @return alias name
 */
QString Commands::getData(Argument::Types f) const
{
    if (f != Argument::ALIAS) {
        qDebug()<<"Something went wrong: Commands::getAlias"<<endl;

    }
    return m_argument.getData();
}




/**
 * @brief getter for the flague
 * @return flague
 */
Argument::Types Commands::getFlague() const
{
    return m_flague;
}


/**
 * @brief getter for the password
 * @return password
 */
Password Commands::getPassword() const
{
    if (m_flague != Argument::ALIAS) {
        qDebug()<<"Something went wrong: Commands::getPassword()"<<endl;
    }
    return m_password;
}

/**
 * @brief to check if the whole command is correct
 * @return is the command correct or not
 */
bool Commands::isCorrect() const
{
    return m_correct;
}

void Commands::shout() const
{
    qDebug()<<"UDALO SIE"<<endl;
}
Commands::~Commands()
{

}


} //namespace Client
} //namespace TIN_project
