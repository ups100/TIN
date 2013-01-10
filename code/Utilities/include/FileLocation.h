/**
 * @file FileLocation.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::FileLocation
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(EA_F8C56EF9_6597_4618_8783_B8F31AB3B1AC__INCLUDED_)
#define EA_F8C56EF9_6597_4618_8783_B8F31AB3B1AC__INCLUDED_

#include "Identifier.h"
#include <QByteArray>
#include <QString>

namespace TIN_project {
namespace Utilities {

/**
 * Location of the file in Alias
 *
 * @author Kajo0
 */
class FileLocation
{
    /** Relative to alias path to file */
    QString m_path;

    /** File size */
    quint32 m_size;

    /** Identifier of the machine where the file is */
    Identifier m_ownerIdentifier;

public:

    /**
     * @brief C-tor
     *
     * @param path Path to file relative to alias
     * @param size Size of file
     * @param ownerIdentifier Identifier of the files owner machine
     */
    FileLocation(const QString &path, const quint32 &size,
            const Identifier &ownerIdentifier);

    /**
     * @brief C-tor
     *
     * @param data Data to create object
     */
    FileLocation(const QByteArray &data);

    /**
     * @brief D-tor
     */
    virtual ~FileLocation();

    /**
     * @brief Get path to file
     *
     * @return Path to file
     */
    QString getPath();

    /**
     * @brief Get file size
     *
     * @return Size of file
     */
    quint32 getSize();

    /**
     * @brief Get file owner ID
     *
     * @return ID of the owner
     */
    Identifier getOwnerIdentifier();

    /**
     * @brief Convert object to QByteArray
     *
     * @return Object as QByteArray
     */
    QByteArray toQByteArray();

};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(EA_F8C56EF9_6597_4618_8783_B8F31AB3B1AC__INCLUDED_)
