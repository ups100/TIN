///////////////////////////////////////////////////////////
//  FileLocation.h
//  Implementation of the Class FileLocation
//  Created on:      07-gru-2012 00:33:36
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_F8C56EF9_6597_4618_8783_B8F31AB3B1AC__INCLUDED_)
#define EA_F8C56EF9_6597_4618_8783_B8F31AB3B1AC__INCLUDED_
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
namespace TIN_project {
namespace Utilities {

/**
 * Location of the file in Alias
 */
class FileLocation
{
public:
    FileLocation();
    FileLocation(QString);
    static bool checkLocation(QString);
    virtual ~FileLocation();

private:
    QDir m_location;
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_F8C56EF9_6597_4618_8783_B8F31AB3B1AC__INCLUDED_)
