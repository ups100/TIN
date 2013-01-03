/*
 * Argument.cpp
 *
 *  Created on: 14-12-2012
 *      Author: marcin
 */

#include "Argument.h"

namespace TIN_project {
namespace Client {

Argument::Argument()
{

}


Argument::Argument(QString s, Argument::Types f)
{
    flague = f;
    switch(f) {
        case Argument::NONE:
            break;
        case Argument::ALIAS:
            m_alias = s;
            break;
        case Argument::FILELOCATION:

            break;
        case Argument::FILENAME:
            m_filename = s;
            break;
        default:
            return;
    }
}

/**
 * @brief getter for the flague
 * @return flague
 */
Argument::Types Argument::getFlague() const
{
    return flague;
}

/**
 * @brief getter for the alias
 * @return alias
 */
QString Argument::getAlias() const
{
    if (flague != Argument::ALIAS) {
        qDebug()<<"Something went wrong: Argument::getAlias()"<<endl;
    }
    return m_alias;
}



/**
 * @brief getter for the file name
 * @return file name
 */
QString Argument::getFileName() const
{
    if(flague != Argument::FILENAME) {
        qDebug()<<"Something went wrong: Argument::getFileName()"<<endl;
        return NULL;
    }
    return m_filename;
}
Argument::~Argument()
{
    // TODO Auto-generated destructor stub
}

} /* namespace Client */
} /* namespace TIN_project */
