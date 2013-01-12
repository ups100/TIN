/**
 * @file DaemonConfiguration.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Daemon::DaemonConfiguration
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(DAEMON_CONFIGURATION__INCLUDED_)
#define DAEMON_CONFIGURATION__INCLUDED_

#include <boost/shared_ptr.hpp>
#include <QList>
#include <QString>
#include <QMutexLocker>

namespace TIN_project {
namespace Daemon {

/**
 * Class containing and manage daemon threads configurations
 *
 * @author Kajo0
 */
class DaemonConfiguration
{
public:

    /**
     * Structure for storing daemon threads information
     *
     * @author Kajo0
     */
    struct Config
    {
        /** Server IP */
        QString m_ip;
        /** Server Port */
        quint16 m_port;
        /** Alias name */
        QString m_aliasId;
        /** Password to alias */
        QByteArray m_password;
        /** Absolute path to catalogue */
        QString m_cataloguePath;

        /**
         * @brief Empty C-tor
         */
        Config()
                : m_port(0)
        {

        }

        /**
         * @brief C-tor
         * @param ip
         * @param port
         * @param aliasId
         * @param password
         * @param cataloguePath
         */
        Config(const QString ip, const quint16 port, const QString aliasId,
                const QByteArray password, const QString cataloguePath)
                : m_ip(ip), m_port(port), m_aliasId(aliasId),
                        m_password(password), m_cataloguePath(cataloguePath)
        {

        }

        /**
         * @brief C-tor
         * @param aliasId
         * @param cataloguePath
         */
        Config(const QString aliasId, const QString cataloguePath)
                : m_aliasId(aliasId), m_cataloguePath(cataloguePath), m_port(0)
        {

        }

        /**
         * @brief Check equality of configs by aliasId and cataloguePath
         * @param other Other config to check with
         * @return true if aliases & paths are equal
         */
        bool operator==(const Config &other)
        {
            if (m_aliasId == other.m_aliasId
                    && m_cataloguePath == other.m_cataloguePath)
                return true;

            return false;
        }
    };

    typedef struct Config Config;

private:

    /**
     * @brief List of daemon threads configuration
     */
    QList<boost::shared_ptr<DaemonConfiguration::Config> > m_configs;

    /**
     * @brief IO mutex
     */
    static QMutex m_mutex;

public:

    /** Path to configuration file */
    static const char* DAEMON_CONFIG_FILEPATH;

    /**
     * XML strings
     */
    static const char* DAEMON_CONFIG_XML_THREADS;
    static const char* DAEMON_CONFIG_XML_THREAD;
    static const char* DAEMON_CONFIG_XML_ADDRESS;
    static const char* DAEMON_CONFIG_XML_ADDRESS_ATTR_IP;
    static const char* DAEMON_CONFIG_XML_ADDRESS_ATTR_PORT;
    static const char* DAEMON_CONFIG_XML_ALIAS;
    static const char* DAEMON_CONFIG_XML_ALIAS_ATTR_ID;
    static const char* DAEMON_CONFIG_XML_PASSWORD;
    static const char* DAEMON_CONFIG_XML_PASSWORD_ATTR_PASSWORD;
    static const char* DAEMON_CONFIG_XML_CATALOGUE;
    static const char* DAEMON_CONFIG_XML_CATALOGUE_ATTR_PATH;

    /**
     * @brief C-tor,
     *  Load Threads configuration,
     *  Get MAC address as field
     */
    DaemonConfiguration();

    /**
     * @brief D-tor
     *  Save actual configuration to file
     */
    virtual ~DaemonConfiguration();

    /**
     * @brief Load configuration into memory from file,
     *  If no config file, generate empty one
     */
    void loadConfiguration();

    /**
     * @brief Save to config file actual daemon configurations for threads
     */
    void saveConfiguration();

    /**
     * @brief Get list of threads configuration
     * @return List of configs
     */
    const QList<boost::shared_ptr<DaemonConfiguration::Config> >& getConfigs();

    /**
     * @brief Add new thread configuration and save configuration if added
     * @param config configuration with address, pass, alias etc
     * @return true if added, false otherwise (if already exists)
     */
    bool addConfig(boost::shared_ptr<DaemonConfiguration::Config> config);

    /**
     * @brief Remove thread config by given alias & path and save configuration if removed
     * @param aliasId Id of alias
     * @param path Path to catalogue
     * @return true if removed, false otherwise (if no given config)
     */
    bool removeConfig(const QString &aliasId, const QString &path);

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(DAEMON_CONFIGURATION__INCLUDED_)
