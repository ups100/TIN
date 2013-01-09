/**
 * @file Identifier.h
 *
 * @date 08-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::Identifier
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(IDENTIFIER__INCLUDED_)
#define IDENTIFIER__INCLUDED_

#include <QByteArray>
#include <QString>

namespace TIN_project {
namespace Utilities {

/**
 * Identity of client or daemon connected to Alias
 *
 * @author Kajo0
 */
class Identifier
{
    /** Absolute path to catalogue */
    QString m_path;

    /** ID of machine where is the catalogue */
    QString m_id;

public:

    /**
     * @brief Default C-tor
     */
    Identifier();

    /**
     * @brief C-tor
     *
     * @param path Absolute path to catalogue
     * @param id ID of the machine where the catalogue is
     */
    Identifier(const QString &path, const QString &id);

    /**
     * @brief C-tor
     *
     * @param data Data to create object
     */
    Identifier(const QByteArray &data);

    /**
     * @brief D-tor
     */
    virtual ~Identifier();

    /**
     * @brief Get path to catalogue
     *
     * @return Path to catalogue
     */
    QString getPath();

    /**
     * @brief Get ID
     *
     * @return ID
     */
    QString getId();

    /**
     * @brief Convert object to QByteArray
     *
     * @return Object as QByteArray
     */
    QByteArray toQByteArray();

    friend QDataStream& operator<<(QDataStream &out,
            const Identifier &identifier);
    friend QDataStream& operator>>(QDataStream &in, Identifier &identifier);

};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(IDENTIFIER__INCLUDED_)
