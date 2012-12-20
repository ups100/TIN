///////////////////////////////////////////////////////////
//  AliasFileList.cpp
//  Implementation of the Class AliasFileList
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "AliasFileList.h"
#include <QDataStream>
#include <QIODevice>

namespace TIN_project {
namespace Utilities {

AliasFileList::AliasFileList()
{

}

AliasFileList::AliasFileList(const QByteArray &bytes)
{
    QDataStream in(bytes);

    // Flush size
    quint16 size;
    in >> size;

    // TODO implement that
//    in >> ?!;
}

AliasFileList::~AliasFileList()
{

}

/**
 * @brief Convert (serialize) Message object into QByteArray
 * @return Serialized object as QByteArray with size at the beginning
 */
QByteArray AliasFileList::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    // Size
    out << (quint16) 0;

    // TODO implement that
//    out << ?!;

    // Set size
    out.device()->seek(0);
    out << (quint16) (bytes.size() - sizeof(quint16));

    return bytes;
}

} //namespace Utilities
} //namespace TIN_project
