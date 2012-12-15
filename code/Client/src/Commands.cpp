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
}

Commands::Commands(QString command, Argument::Types f)
{
    m_correct = true;
    if(f != Argument::NONE) {
        qDebug()<<"Something went wrong: Commands(QString, Argument::Types)"<<endl;
        qDebug()<<"Commands object was created, but is invalid"<<endl;
        m_correct = false;
    }
    m_flague = f;
    m_command = command;
}

Commands::Commands(QString command, QString value, Argument::Types f)
{
    m_flague = f;
    m_command = command;
    m_correct = true;
    switch(f) {
        case Argument::NONE:
            m_parameter = value;
            break;
        case Argument::FILELOCATION:
            m_argument = Argument(value, f);
            break;
        case Argument::FILENAME:
            m_argument = Argument(value, f);
            break;
        default:
            qDebug()<<"Something went wrong: Commands(QString, QString, Argument::Types"<<endl;
            qDebug()<<"Commands object was created, but is invalid"<<endl;
            m_correct = false;
            break;
    }
}
Commands::Commands(QString command, QString arg1, QString arg2, Argument::Types f)
{
    m_flague = f;
    m_command = command;
    m_correct = true;
    switch (f) {
        case Argument::ALIAS:
            m_argument = Argument(arg1,f);
            m_password = Password(arg2);
            break;
        case Argument::FILELOCATION:
            m_parameter = arg1;
            m_argument = Argument(arg2,f);
            break;
        case Argument::FILENAME:
            m_parameter = arg1;
            m_argument = Argument(arg2,f);
            break;
        default:
            qDebug()<<"Something went wrong: Commands(QString, QString, QString, Argument::Types"<<endl;
            qDebug()<<"Commands object was created, but is invalid"<<endl;
            m_correct = false;
            break;
    }
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
 * @return one of the types that are stored in Argument class
 */
QVariant Commands::getArg(Argument::Types f) const
{
    QVariant l_var;
    l_var.setValue(NULL);
    if(!m_correct) {
        qDebug()<<"Command is not correct, do not use it"<<endl;
    }
    if (f != m_flague) {
        qDebug()<<"Something went wrong: Commands::getArg(Argument::Types)"<<endl;
        qDebug()<<"The Argument::Type is not proper"<<endl;
        return l_var;
    }
    switch (f) {
        case Argument::NONE:
            l_var.setValue(NULL);
            break;
        case Argument::ALIAS:
            l_var.setValue(getAlias(f));
            break;
        case Argument::FILELOCATION:
            l_var.setValue(getFileLocation(f));
            break;
        case Argument::FILENAME:
            l_var.setValue(getFileName(f));
            break;
        default:
            qDebug()<<"Something went wrong: Commands::getArg";
            break;
    }
    return l_var;
}

/**
 * @brief getter for the name of an alias
 * @return alias name
 */
QString Commands::getAlias(Argument::Types f) const
{
    if (f != Argument::ALIAS) {
        qDebug()<<"Something went wrong: Commands::getAlias"<<endl;

    }
    return m_argument.getAlias();
}

/**
 *  @brief getter for the file name
 *  @return file name
 */
QString Commands::getFileName(Argument::Types f) const
{
    if (f != Argument::FILENAME) {
        qDebug()<<"Don't do that"<<endl;
        return NULL;
    }
    return m_argument.getFileName();
}

/**
 *  @brief getter for the file location
 *  @return file location
 */
FileLocation Commands::getFileLocation(Argument::Types f) const
{
    if (f != Argument::FILELOCATION) {
        qDebug()<<"Don't do that"<<endl;
        return (FileLocation)NULL;
    }
    return m_argument.getFileLocation();
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

Commands::~Commands()
{

}


} //namespace Client
} //namespace TIN_project
