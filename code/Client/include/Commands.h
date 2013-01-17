///////////////////////////////////////////////////////////
//  Commands.h
//  Implementation of the Class Commands
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_)
#define EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_
#include <QList>
#include <QDebug>
#include <QStringList>
#include <QVariant>
#include "Argument.h"

namespace TIN_project {
namespace Client {
using namespace TIN_project::Utilities;

class Commands
{
public:

    /**
     * @brief Constructor
     */
    Commands();
    /**
     * @brief Constructor
     * @param[in] cmd Command
     * @param[in] flague Flague
     */
    Commands(QString cmd, Argument::Types flague);

    /**
     * @brief Constructor
     * @param[in] cmd Command
     * @param[in] arg Argument
     * @param[in] flague Flague
     */
    Commands(QString cmd, QString arg, Argument::Types flague);

    /**
     * @brief Constructor
     * @param[in] cmd Command
     * @param[in] arg Argument
     * @param[in] arg2 Argument
     * @param[in] flague Flague
     */
    Commands(QString cmd, QString arg, QString arg2, Argument::Types flague);

    /**
     * @brief Constructor
     * @param[in] cmd Command
     * @param[in] arg Argument
     * @param[in] arg2 Argument
     * @param[in] flague Flague
     */
    Commands(QString cmd, QString arg, QString arg2, QString arg3,
            Argument::Types flague);

    /**
     * @brief Getter for the command
     */
    QString getCommand();

    /**
     * @brief Getter for the parameter
     */
    QString getParameter();

    /**
     * @brief Getter for the argument
     */
    QString getArg() const;

    /**
     * @brief Getter for the second argument (used in add and remove)
     */
    QString getArg2() const;

    /**
     * @brief Getter for the password
     */
    Password getPassword() const;

    /**
     * @brief Getter for the flague (but to be honest I hardly use it)
     */
    Argument::Types getFlague() const;

    /**
     * @brief checks if command is correct
     * @return true if is correct, false otherwise
     */
    bool isCorrect() const;

    /**
     * @brief Just for debug
     */
    void shout() const;
    virtual ~Commands();
private:

    /**
     * @brief useless, to be removed
     */
    QString getData(Argument::Types) const;

    /**
     * @brief Command invoked by user
     */
    QString m_command;
    /**
     * @brief Parameter of command
     */
    QString m_parameter;

    /**
     * @brief Argument of command
     */
    Argument m_argument;

    /**
     * @brief Used only on add and remove directory
     */
    Argument m_argument2;

    /**
     * @brief Password (not used always)
     */
    Password m_password;

    /**
     * @brief flague of command (hardly used)
     */
    Argument::Types m_flague;

    /**
     * @brief handles information if the command is correct
     */
    bool m_correct;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_)
