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
#include <../include/Argument.h>
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
    QString getCommand();
    QString getParameter();
    QVariant getArg(Argument::Types) const;
    Password getPassword() const;
    Argument::Types getFlague() const;
    bool isCorrect() const;
    virtual ~Commands();
    bool invoke(); /* To do */
private:
    QString getAlias(Argument::Types) const;
    QString getFileName(Argument::Types) const;
    FileLocation getFileLocation(Argument::Types) const;

    QString m_command;
    QString m_parameter;
    Argument m_argument;
    Password m_password;
    Argument::Types m_flague;
    bool m_correct;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_)
