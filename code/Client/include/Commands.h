///////////////////////////////////////////////////////////
//  Commands.h
//  Implementation of the Class Commands
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_)
#define EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_
#include <QStringList>
#include <QDebug>
#include <QDataStream>
namespace TIN_project {
namespace Client {

class Commands
{

public:
    Commands();
    Commands(QStringList);
    Commands(QString, QString, QStringList);
    QString getCommand();
    QString getParameter();
    QString getArg(int);
    QStringList getArg();
    virtual ~Commands();
private:
    QString m_command;
    QString m_parameter;
    QStringList m_arguments;
};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_BF1C3997_E0EB_4819_BDC9_CF0F9B797239__INCLUDED_)
