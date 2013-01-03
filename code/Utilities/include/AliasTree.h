#if !defined(ALIAS_TREE__INCLUDED_)
#define ALIAS_TREE__INCLUDED_

#include <QByteArray>
#include <QString>
#include <QList>
#include <boost/shared_ptr.hpp>

namespace TIN_project {
namespace Utilities {

/**
 * Structure to gather simple file-system list of directories and files inside
 *
 * @author Kajo0
 */
class AliasTree
{
public:

    /**
     * Structure to store file attributes data
     *
     * @author Kajo0
     */
    struct Location
    {
        /** ID of machine */
        QString m_id;
        /** Last modification date as timestamp */
        QString m_date;
        /** File size */
        unsigned int m_size;

        /**
         * @brief C-tor
         *
         * @param id
         * @param date
         * @param size
         */
        Location(const QString id, const QString date, const unsigned int size)
                : m_id(id), m_date(date), m_size(size)
        {

        }

    };

    typedef struct Location Location;

    static int i;
    int id;
private:

    /** List of file locations refers to one or more machines, empty if dir */
//    QList<boost::shared_ptr<Location> > m_fileLocations;
    QList<Location> m_fileLocations;

    /** List of files or dirs inside of current directory, empty if file */
    QList<boost::shared_ptr<AliasTree> > m_dirContent;

    /** File path */
    QString m_path;
    int a;

public:

    /**
     * @brief C-tor, root node with / path
     */
    AliasTree();

    /**
     * @brief C-tor
     *
     * @param path Node path
     */
    AliasTree(const QString &path);

    /**
     * @brief C-tor
     *
     * @param data Data to create object
     */
    AliasTree(const QByteArray& data);

    /**
     * @brief D-tor
     */
    virtual ~AliasTree();

    /**
     * @brief Add location to node
     *
     * @param id Machine ID from file origin
     * @param date Last modification date
     * @param size Size of file
     */
    void addLocation(const QString &id, const QString &date, const unsigned int &size);

    /**
     * @brief Split path into parts, if non last part -> create tree node,
     *              if last -> create node and add location inside
     *
     * @param path Path to file
     * @param date Last modification date
     * @param size Size of file
     */
    boost::shared_ptr<AliasTree> addFile(const QString &path, const QString &date, const unsigned int &size);

    /**
     * @brief Get filename, or name of directory if dir
     *
     * @return Name of file/dir
     */
    QString getFilename();

    /**
     * @brief Check whether instance represent directory
     *
     * @return true if it's directory, false otherwise
     */
    bool isDir();

    /**
     * @brief Check whether instance represent file
     *
     * @return true if it's file, false otherwise
     */
    bool isFile();

    /**
     * @brief Convert object to QByteArray
     *
     * @return Object as QByteArray
     */
    QByteArray toQByteArray();

    //TODO remove debug
    void str(int indent);
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(ALIAS_TREE__INCLUDED_)
