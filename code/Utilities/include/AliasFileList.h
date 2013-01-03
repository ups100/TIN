///////////////////////////////////////////////////////////
//  AliasFileList.h
//  Implementation of the Class AliasFileList
//  Created on:      07-gru-2012 00:33:33
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_)
#define EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_

#include "AliasTree.h"
#include <QByteArray>
#include <QString>

namespace TIN_project {
namespace Utilities {

/**
 * Container to store file lists
 *
 * @author Kajo0
 */
class AliasFileList
{public:
    /** Amount of files? on list */
    unsigned int m_size;

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
     * @param filename Path to file
     * @param date Last file modification date timestamp
     * @param size Size of file
     */
    void addFile(const QString &filePath, const QString &date,
            const unsigned int &size);

    /**
     * @brief Merge two instances of AliasFileList into one
     *
     * @param other Another object
     */
    void merge(const AliasFileList &other);

    /**
     * @brief Convert object to QByteArray
     *
     * @return Object as QByteArray
     */
    QByteArray toQByteArray();
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_A26F37CC_44D6_4f5b_915E_18BA44DC4E9A__INCLUDED_)
