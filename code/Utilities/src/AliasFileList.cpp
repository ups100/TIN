///////////////////////////////////////////////////////////
//  AliasFileList.cpp
//  Implementation of the Class AliasFileList
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "AliasFileList.h"

namespace TIN_project {
namespace Utilities {

AliasFileList::AliasFileList()
        : m_size(0)
{

}

AliasFileList::AliasFileList(const QByteArray &data)
{

}

AliasFileList::~AliasFileList()
{

}

void AliasFileList::addFile(const QString &filePath, const QString &date,
        const unsigned int &size)
{
    m_fileTree.addFile(filePath, date, size);
}

QByteArray AliasFileList::toQByteArray()
{
    return QByteArray();
}

} //namespace Utilities
} //namespace TIN_project
