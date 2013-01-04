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
#include <QObject>
#include <QFileInfo>
#include <QMetaType>
#include <QDebug>

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

    /** ID of machine where the file is */
    QString m_ownerId;

public:

    /**
     * @brief Constructor
     *
     * @details Creates invalid object
     */
    FileLocation();

    /**
     * @brief C-tor
     *
     * @param path Path to file relative to alias
     * @param ownerId ID of the files owner machine
     */
    FileLocation(const QString &path, const QString &ownerId);

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
     * @brief Get file owner ID
     *
     * @return ID of the owner
     */
    QString getOwnerId();

    /**
     * @brief Convert object to QByteArray
     *
     * @return Object as QByteArray
     */
    QByteArray toQByteArray();

};

} //namespace Utilities
} //namespace TIN_project
Q_DECLARE_METATYPE(TIN_project::Utilities::FileLocation);
#endif // !defined(EA_F8C56EF9_6597_4618_8783_B8F31AB3B1AC__INCLUDED_
