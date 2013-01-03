#include "AliasTree.h"
#include "Identify.h"
#include <QDir>
#include <boost/foreach.hpp>
#include <iostream> // TODO remove debug
#include <QDebug>

namespace TIN_project {
namespace Utilities {

AliasTree::AliasTree()
        : m_path("")
{
    id = i++;
    a = 0;
}

AliasTree::AliasTree(const QString &path)
        : m_path(path)
{
    id = i++;
    a = 0;
}

AliasTree::AliasTree(const QByteArray &data)
{

}

AliasTree::~AliasTree()
{

}

int AliasTree::i = 0;

void AliasTree::addLocation(const QString &id, const QString &date,
        const unsigned int &size)
{
//    m_fileLocations.append(boost::shared_ptr<Location>(new Location(id, date, size)));
    qDebug()<<"\tDodaje do: "<<m_path<<" o id: "<<this->id;
    m_fileLocations.append(Location(id, date, size));
    qDebug()<<&m_fileLocations;
    a++;
}

boost::shared_ptr<AliasTree> AliasTree::addFile(const QString &path,
        const QString &date, const unsigned int &size)
{
    QStringList parts = path.split(QDir::separator(), QString::SkipEmptyParts);

    if (!parts.size()) {
        // Never should be there
        qDebug()<<"No file given.";
        throw "No file given";
    }

    QString name = parts.first();
    parts.pop_front();

    // Whether we're searching for a file to place it in
    bool searchForFile = (parts.size() == 0);

    foreach (boost::shared_ptr<AliasTree> atree, m_dirContent){
//     BOOST_FOREACH(boost::shared_ptr<AliasTree> &atree, m_dirContent) {
        if (atree->getFilename() == name) {
            if (searchForFile) {
                qDebug()<<"Nowa lokacja w znalezionym pathie";
                atree->addLocation(Identify::getMachineIdentificator(), date, size);
                return atree;
            } else {
                qDebug()<<"Nowy kat dla pahta";
                return atree->addFile(parts.join(QDir::separator()), date, size);
            }
        }
    }


    // If desn't found any file or directory, create new one
    AliasTree *at = new AliasTree(m_path + QDir::separator() + name);
    boost::shared_ptr<AliasTree> atree = boost::shared_ptr<AliasTree>();
    atree.reset(at);
    m_dirContent.append(atree);

    if (searchForFile) {
        qDebug()<<"Nowa lokacja w nowym kat";
        qDebug()<<"\t-:"<<m_path<<atree->m_fileLocations.size();

        at->addLocation(Identify::getMachineIdentificator(), date, size);
        return atree;
    } else {
        qDebug()<<"Nowy kat dla patha nowego";
        return atree->addFile(parts.join(QDir::separator()), date, size);
    }
}

void AliasTree::str(int indent) {
    foreach (boost::shared_ptr<AliasTree> atree, m_dirContent) {
        if (atree->isFile()) {
            std::cout.width(indent * 4); std::cout<<" ";
            qDebug()<<m_fileLocations.size()<<" id: "<<atree->id<<"a: "<<atree->a;
            qDebug()<<&m_fileLocations;
            std::cout<<atree->getFilename().toStdString()<<"\n";//"\t"<<m_fileLocations.first().m_date.toStdString()<<"\n";//<<"\t"<<m_fileLocations.first().m_size<<"\t"<<m_fileLocations.first().m_id.toStdString()<<"\n";
        } else {
            std::cout.width(indent * 4); std::cout<<" ";
            std::cout<<atree->m_path.toStdString()<<"\n";
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
    return m_fileLocations.empty();
}

bool AliasTree::isFile()
{
    return !isDir();
}

QByteArray AliasTree::toQByteArray()
{
    return QByteArray();
}

} //namespace Utilities
} //namespace TIN_project
