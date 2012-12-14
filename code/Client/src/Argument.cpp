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
        case Argument::PASSWORD:
            m_password = Password(s);
            break;
        case Argument::FILES:
            if(FileLocation::checkLocation(s)) {
                m_paths.push_back(FileLocation(s));
            }
            else qDebug()<<"There is no such file"<<endl;
            break;
        case Argument::STRING:
            m_file = s;
            break;
        default:
            return;
    }
}

Argument::Types Argument::getFlague()
{
    return flague;
}

Argument::~Argument()
{
    // TODO Auto-generated destructor stub
}

} /* namespace Client */
} /* namespace TIN_project */
