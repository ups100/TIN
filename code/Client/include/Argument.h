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
        NUMBER = 2,
        NAME = 3,
        ABS_PATH = 4,
        REL_PATH = 5,
        SYNCH = 6
    };
    Q_DECLARE_FLAGS(Types, Type)
    Argument();
    Argument(QString, Argument::Types);

    /**
     * @brief getter for the flague
     * @return Argument::Types flague
     */
    Argument::Types getFlague() const;

    /**
     * @brief getter for data stored in argument
     * @return data
     */
    QString getData() const;
    virtual ~Argument();

private:

    /**
     * @brief handles flague of command that argument is connected to
     */
    Argument::Types m_flague;

    /**
     * @brief data stored in argument
     */
    QString m_data;

};

} /* namespace Client */
} /* namespace TIN_project */
Q_DECLARE_OPERATORS_FOR_FLAGS(TIN_project::Client::Argument::Types)
#endif /* ARGUMENT_H_ */
