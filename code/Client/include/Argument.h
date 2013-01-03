/*
 * Argument.h
 *
 *  Created on: 14-12-2012
 *      Author: marcin
 */

#ifndef ARGUMENT_H_
#define ARGUMENT_H_
#include <../../Utilities/include/FileLocation.h>
#include <../../Utilities/include/Password.h>
#include <QList>
#include <QDir>
#include <iostream>
#include <QDebug>
using namespace TIN_project::Utilities;
namespace TIN_project {
namespace Client {

class Argument
{
public:
    enum Type
    {
        NONE = 0,
        ALIAS = 1,
        FILELOCATION = 2,
        FILENAME = 3,
    };
    Q_DECLARE_FLAGS(Types, Type)
    Argument();
    Argument(QString, Argument::Types);
    Argument::Types getFlague() const;
    QString getAlias() const;
    QString getFileName() const;
    virtual ~Argument();

private:
    Argument::Types flague;
    QString m_filename;
    QString m_alias;

};

} /* namespace Client */
} /* namespace TIN_project */
Q_DECLARE_OPERATORS_FOR_FLAGS(TIN_project::Client::Argument::Types)
#endif /* ARGUMENT_H_ */
