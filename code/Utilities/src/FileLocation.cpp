///////////////////////////////////////////////////////////
//  FileLocation.cpp
//  Implementation of the Class FileLocation
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "FileLocation.h"

namespace TIN_project {
namespace Utilities {

FileLocation::FileLocation()
{

}

FileLocation::FileLocation(QString f)
{
    m_location = QDir(f);
}

bool FileLocation::checkLocation(QString f)
{
    /**
     * Here should be the root of a certain alias
     * By now, it is just "/home/marcin/repo/TIN/
     */
    QString root = "/home/marcin/repo/TIN/";
    f.prepend(root);
    qDebug()<<f<<endl;
    QDir dir(f);
    if (dir.exists()) {
        qDebug()<<"PLIK ISTNIEJE"<<endl;
        return true;
    }
    else {
        qDebug()<<"PLIK NIE ISTNIEJE"<<endl;
        return true;
    }
}


FileLocation::~FileLocation()
{

}

} //namespace Utilities
} //namespace TIN_project
