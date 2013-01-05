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
    Commands();
    Commands(QString, Argument::Types);
    Commands(QString, QString, Argument::Types);
    Commands(QString, QString, QString, Argument::Types);
    Commands(QString, QString, QString, QString, Argument::Types);

    /**
     * @brief getter for the command
     */
    QString getCommand();

    /**
     * @brief getter for the parameter
     */
    QString getParameter();

    /**
     * @brief getter for the argument (luckily maximum one is possible)
     */
    QString getArg() const;

    /**
     * @brief getter for the password
     */
    Password getPassword() const;

    /**
     * @brief getter for the flague (but to be hones I hardly use it)
     */
    Argument::Types getFlague() const;

    /**
     * @brief checks if command is correct
     * @return true if is correct, false otherwise
     */
    bool isCorrect() const;

    /**
     * @brief just for debug
     */
    void shout() const;
    virtual ~Commands();
private:

    /**
     * @brief useless, to be removed
     */
    QString getData(Argument::Types) const;

    /**
     * @brief command invoked by user
     */
    QString m_command;

    /**
     * @brief parameter of command
     */
    QString m_parameter;

    /**
     * @brief argument of command
     */
    Argument m_argument;

    /**
     * @brief password (not used always)
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
