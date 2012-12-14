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
#include <../include/Argument.h>
namespace TIN_project {
namespace Client {
using namespace TIN_project::Utilities;
class Commands
{

public:
    Commands();
    Commands(QString, QString, QStringList, Argument::Types);
    QString getCommand();
    QString getParameter();
    Argument getArg(int);
    QList<Argument> getArg();
    Argument::Types getFlague();
    virtual ~Commands();
private:
    QString m_command;
    QString m_parameter;
    QList<Argument> m_arguments;
    Argument::Types flague;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_)
