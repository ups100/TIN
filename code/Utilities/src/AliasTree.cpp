/**
 * @file AliasTree.cpp
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::AliasTree
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#include "AliasTree.h"
#include "Identify.h"
#include <QDir>
#include <QIODevice>
#include <QDebug>
#include <iostream> // TODO remove debug

namespace TIN_project {
namespace Utilities {

AliasTree::AliasTree()
        : m_path("")
{

}

AliasTree::AliasTree(const QString &path)
        : m_path(path)
{

}

AliasTree::AliasTree(const QByteArray &data)
{
    QDataStream in(data);

    in >> *this;
}

AliasTree::~AliasTree()
{

}

void AliasTree::addLocation(const QString &id, const QString &date,
        const unsigned int &size)
{
    m_fileLocations.append(Location(id, date, size));
}

boost::shared_ptr<AliasTree> AliasTree::addFile(const QString &path,
        const QString &date, const unsigned int &size, QString id)
{
    QStringList parts = path.split(QDir::separator(), QString::SkipEmptyParts);

    if (!parts.size()) {
        // Never should be there
        qDebug() << "No file given.";
        throw "No file given";
    }

    QString name = parts.first();
    parts.pop_front();

    // Whether we're searching for a file to place it in
    bool searchForFile = (parts.size() == 0);

    foreach (boost::shared_ptr<AliasTree> atree, m_dirContent){
    if (atree->getFilename() == name) {
        if (searchForFile) {
            atree->addLocation((id.length() ? id : Identify::getMachineIdentificator()), date, size);
            return atree;
        } else {
            return atree->addFile(parts.join(QDir::separator()), date, size, id);
        }
    }
}

// If desn't found any file or directory, create new one
    boost::shared_ptr<AliasTree> atree = boost::shared_ptr<AliasTree>(
            new AliasTree(m_path + QDir::separator() + name));
    m_dirContent.append(atree);

    if (searchForFile) {
        atree->addLocation(
                (id.length() ? id : Identify::getMachineIdentificator()), date,
                size);
        return atree;
    } else {
        return atree->addFile(parts.join(QDir::separator()), date, size, id);
    }
}

void AliasTree::str(int indent)
{
    foreach (boost::shared_ptr<AliasTree> atree, m_dirContent){
    if (atree->isFile()) {
        //std::cout.width(indent * 4); std::cout<<" ";
        //std::cout<<atree->getFilename().toStdString()<<"\t(x"<<atree->m_fileLocations.size()<<")"<<"\t"<<atree->m_fileLocations.first().m_date.toStdString()<<"\t"<<atree->m_fileLocations.first().m_size<<"\t"<<atree->m_fileLocations.first().m_id.toStdString()<<"\n";
        for(int i = 0; i<atree->m_fileLocations.size();++i)
        {
            std::cout.width(indent * 4); std::cout<<" ";
                  std::cout<<atree->getFilename().toStdString()<<"\t"<<atree->m_fileLocations[i].m_date.toStdString()<<"\t"<<atree->m_fileLocations[i].m_size<<"\t"<<atree->m_fileLocations[i].m_id.toStdString()<<"\n";
        }
    } else {
        std::cout.width(indent * 4); std::cout<<" ";
       // std::cout<<atree->m_path.toStdString()<<"\n";
        std::cout<<atree->getFilename().toStdString()<<"\n";
        atree->str(indent + 1);
    }
}
}

QString AliasTree::getFilename()
{
    QStringList parts = m_path.split(QDir::separator(),
            QString::SkipEmptyParts);

    return parts.last();
}

bool AliasTree::isDir()
{
    return !m_dirContent.empty();
}

bool AliasTree::isFile()
{
    return !isDir();
}

const QList<AliasTree::Location>& AliasTree::getLocations()
{
    return m_fileLocations;
}

QByteArray AliasTree::toQByteArray()
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);

    out << *this;

    return bytes;
}

QDataStream& operator<<(QDataStream &out, const AliasTree &atree)
{
    out << atree.m_path;
    out << atree.m_fileLocations;
    out << atree.m_dirContent;

    return out;
}

QDataStream& operator>>(QDataStream &in, AliasTree &atree)
{
    in >> atree.m_path;
    in >> atree.m_fileLocations;
    in >> atree.m_dirContent;

    return in;
}

QDataStream& operator<<(QDataStream &out,
        const boost::shared_ptr<AliasTree> &atree)
{
    out << *atree;

    return out;
}

QDataStream& operator>>(QDataStream &in, boost::shared_ptr<AliasTree> &atree)
{
    AliasTree *buff = new AliasTree();

    in >> *buff;
    atree.reset(buff);

    return in;
}

} //namespace Utilities
} //namespace TIN_project
