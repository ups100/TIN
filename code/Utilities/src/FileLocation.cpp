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

}

bool FileLocation::checkLocation(QString f)
{
    /** Question: how to get the root of an alias in here? */
    /*  Here should be the root of an alias */
    QString root = "/home/marcin/repo/TIN/";


    f.prepend(root);
    qDebug()<<f<<endl;
    QDir dir(f);
    if(dir.exists()) {
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
