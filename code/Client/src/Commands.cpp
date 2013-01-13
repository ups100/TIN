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
    qDebug() << "NIE UDALO SIE" << endl;
}

Commands::Commands(QString command, Argument::Types f)
{
    //LS, DISCONNECT, EXIT
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
    switch (f) {
        //ADD, RM , PUSH
        case Argument::REL_PATH:
            m_argument = Argument(arg1, f);
            break;

            //FIND
        case Argument::NAME:
            m_argument = Argument(arg1, f);
            break;

            //READ, CHOOSE
        case Argument::NUMBER:
            m_argument = Argument(arg1, f);
            break;

            //SYNCH
        case Argument::SYNCH:
            m_parameter = arg1;
            break;

            //LS -L
        case Argument::NONE:
            m_parameter = arg1;
            break;

            //CHANGE
        case Argument::ABS_PATH:
            m_argument = Argument(arg1, f);
            break;
        default:
            qDebug()
                    << "Something went wrong: Commands(QString, QString, Argument::Types"
                    << endl;
            qDebug() << "Commands object was created, but is invalid" << endl;
            m_correct = false;
            break;
    }
    shout();
}

Commands::Commands(QString command, QString arg1, QString arg2,
        Argument::Types f)
{
    m_flague = f;
    m_command = command;
    m_correct = true;
    switch (f) {
        //LOG and CREATE
        case Argument::ALIAS:
            m_argument = Argument(arg1, f);
            m_password = Password(arg2);
            break;
            //RM
        case Argument::REL_PATH:
            m_parameter = arg1;
            m_argument = Argument(arg2, f);
            break;
        default:
            qDebug()
                    << "Something went wrong: Commands(QString, QString, QString, Argument::Types"
                    << endl;
            qDebug() << "Commands object was created, but is invalid" << endl;
            m_correct = false;
            break;
    }
    shout();
}

Commands::Commands(QString command, QString arg1, QString arg2, QString arg3,
        Argument::Types f)
{
    m_flague = f;
    m_command = command;
    m_correct = true;

    if ((f == Argument::ALIAS) && (!(command == "add"))) {
        m_parameter = arg1;
        m_argument = Argument(arg2, f);
        m_password = Password(arg3);
    } else if ((command == "add") || (command == "rm")) {
        m_argument = Argument(arg1, f);
        m_password = Password(arg2);
        m_argument2 = Argument(arg3, f);
    } else
        qDebug() << "Something went wrong" << endl;
    shout();
}

/**
 * @brief Getter for the command
 * @return command
 */
QString Commands::getCommand()
{
    if (m_correct) {
        return m_command;
    } else
        return NULL;
}

/**
 * @brief Getter for the parameter
 * @return parameter
 */
QString Commands::getParameter()
{
    return m_parameter;
}

/**
 * @brief Getter for the argument
 * @return QString
 */
QString Commands::getArg() const
{
    return m_argument.getData();
}

QString Commands::getArg2() const
{
    return m_argument2.getData();
}
/**
 * @brief Getter for the name of an alias
 * @return Alias name
 */
QString Commands::getData(Argument::Types f) const
{
    if (f != Argument::ALIAS) {
        qDebug() << "Something went wrong: Commands::getAlias" << endl;

    }
    return m_argument.getData();
}

/**
 * @brief Getter for the flague
 * @return flague
 */
Argument::Types Commands::getFlague() const
{
    return m_flague;
}

/**
 * @brief Getter for the password
 * @return password
 */
Password Commands::getPassword() const
{
    return m_password;
}

/**
 * @brief To check if the whole command is correct
 * @return is the command correct or not
 */
bool Commands::isCorrect() const
{
    return m_correct;
}

void Commands::shout() const
{
    qDebug() << "UDALO SIE" << endl;
}

Commands::~Commands()
{

}

} //namespace Client
} //namespace TIN_project
