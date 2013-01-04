///////////////////////////////////////////////////////////
//  AliasFileList.cpp
//  Implementation of the Class AliasFileList
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "AliasFileList.h"
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

AliasFileList::AliasFileList()
        : m_size(0)
{

}

AliasFileList::AliasFileList(const QByteArray &data)
        : m_size(0)
{
    QDataStream in(data);

    in >> *this;
}

AliasFileList::~AliasFileList()
{

}

void AliasFileList::addFile(const QString &filePath, const QString &date,
        const quint32 &size)
{
    if (m_fileTree.addFile(filePath, date, size)->getLocations().size() == 1)
        ++m_size; // No duplicates count
}

void AliasFileList::merge(const AliasFileList &other)
{
    mergeIt(other.m_fileTree.m_dirContent);
}

void AliasFileList::mergeIt(const QList<boost::shared_ptr<AliasTree> > &atree)
{
    foreach (boost::shared_ptr<AliasTree> at, atree) {
        if (at->isDir())
            mergeIt(at->m_dirContent);
        else if (at->isFile()) {
            foreach (AliasTree::Location loc, at->m_fileLocations) {
                m_fileTree.addFile(at->m_path, loc.m_date, loc.m_size, loc.m_id);
            }
        }
    }
}

quint32 AliasFileList::getSize()
{
    return m_size;
}

//TODO remove
void AliasFileList::str()
{
    m_fileTree.str(0);
}

QByteArray AliasFileList::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << *this;

    return bytes;
}

QDataStream& operator<<(QDataStream &out, const AliasFileList &fileList)
{
    out << fileList.m_size;
    out << fileList.m_fileTree;

    return out;
}

QDataStream& operator>>(QDataStream &in, AliasFileList &fileList)
{
    in >> fileList.m_size;
    in >> fileList.m_fileTree;

    return in;
}

} //namespace Utilities
} //namespace TIN_project
