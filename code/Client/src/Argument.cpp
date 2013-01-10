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
    m_flague = f;
    m_data = s;
}

/**
 * @brief getter for the flag
 * @return flag
 */
Argument::Types Argument::getFlague() const
{
    return m_flague;
}

/**
 * @brief getter for the data
 * @return data
 */
QString Argument::getData() const
{
    return m_data;
}




Argument::~Argument()
{
    // TODO Auto-generated destructor stub
}

} /* namespace Client */
} /* namespace TIN_project */
