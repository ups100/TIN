/**
 * @file AliasFileList.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::AliasFileList
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_)
#define EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_

#include "AliasTree.h"
#include <QDataStream>
#include <QByteArray>
#include <QString>
#include <qmetatype.h>

namespace TIN_project {
namespace Utilities {

/**
 * Container to store file lists
 *
 * @author Kajo0
 */
class AliasFileList
{
    /** Amount of files on list, different locations of same file count once */
    quint32 m_size;

    /** Tree of files in container */
    AliasTree m_fileTree;

public:

    /**
     * @brief C-tor
     */
    AliasFileList();

    /**
     * @brief C-tor
     *
     * @param data Data to create object
     */
    AliasFileList(const QByteArray &data);

    /**
     * @brief D-tor
     */
    virtual ~AliasFileList();

    /**
     * @brief Add file into list, if first file in directory tree'll be created,
     *          otherwise will be added onto location list
     *
     * @param filePath Path to file
     * @param date Last file modification date timestamp
     * @param size Size of file
     */
    void addFile(const QString &filePath, const QString &date,
            const quint32 &size);

    /**
     * Just for tests
     */
    void addFileWithId(const QString &filePath, const QString &date,
            const quint32 &size, const QString s);

    /**
     * @brief Merge two instances of AliasFileList into one
     *
     * @param other Another object
     */
    void merge(const AliasFileList &other);

    /**
     * @brief Get files amount on the list
     *
     * @return Amount of files inside
     */
    quint32 getSize();
    quint32 getSize() const;

    /**
     * @brief getter for the tree, used in ClientApplication
     * @see ClientApplication::synchWithOverwriting();
     */
    const AliasTree& getTree() const;

    void str(int&);

    /**
     * @brief Convert object to QByteArray
     *
     * @return Object as QByteArray
     */
    QByteArray toQByteArray();

    friend QDataStream& operator<<(QDataStream &out,
            const AliasFileList &fileList);
    friend QDataStream& operator>>(QDataStream &in, AliasFileList &fileList);

private:

    /**
     * @brief 'Recursive' Merges given list of AliasTree with own tree
     *
     * @param atree List of AliasTree to merge with
     */
    void mergeIt(const QList<boost::shared_ptr<AliasTree> > &atree);

};

} //namespace Utilities
} //namespace TIN_project
Q_DECLARE_METATYPE(TIN_project::Utilities::AliasFileList);
#endif // !defined(EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_)
