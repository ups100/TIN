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
                m_path = FileLocation(s);
            }
            else qDebug()<<"There is no such file"<<endl;
            break;
        case Argument::STRING:
            m_filename = s;
            break;
        default:
            return;
    }
}

Argument::Types Argument::getFlague()
{
    return flague;
}

Password Argument::getPassword()
{
    if (flague != Argument::PASSWORD) {
        qDebug()<<"Don't do that"<<endl;
        return (Password)NULL;
    }
    return m_password;
}

FileLocation Argument::getFileLocation()
{
  if (flague != Argument::FILES) {
      qDebug()<<"Don't do that"<<endl;
      return (FileLocation)NULL;
  }
  return m_path;
}

QString Argument::getFileName()
{
    if(flague != Argument::STRING) {
        qDebug()<<"Don't do that"<<endl;
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
