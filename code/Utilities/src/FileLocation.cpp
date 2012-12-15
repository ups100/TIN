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


/**
 * @brief static function to check if the location of file exists
 * @return true if the location exists, false otherwise
 */
bool FileLocation::checkLocation(QString f)
{
    /**
     * Here should be the root of a certain alias
     * By now, it is just "/home/marcin/repo/TIN/
     */
    QString root = "/home/marcin/repo/TIN/";
    f.prepend(root);
    qDebug()<<f<<endl;
    /**
     * Delete 3 lines from the above if
     * the path will not be resolved and will be checked
     * by the root of an alias
     */

    QDir dir(f);
    if (dir.exists()) {
        qDebug()<<"File exists"<<endl;
        return true;
    }
    else {
        qDebug()<<"File does not exists"<<endl;
        return true;
    }
}


FileLocation::~FileLocation()
{

}

} //namespace Utilities
} //namespace TIN_project
