/**
 * @file Argument.cpp
 *
 * @date 04-01-2013
 *
 * @author Marcin Kubik <markubik@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Client::Argument
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
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
