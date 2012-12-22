#if !defined(DAEMON_CONFIGURATION__INCLUDED_)
#define DAEMON_CONFIGURATION__INCLUDED_

#include <boost/shared_ptr.hpp>
#include <QList>
#include <QString>

namespace TIN_project {
namespace Daemon {

class DaemonConfiguration
{
    /** Structure for storing daemon threads information */
    typedef struct
    {
        QString m_ip;
        QString m_port;
        QString m_aliasId;
        QString m_password;
        QString m_cataloguePath;
    } Config;

    /**  Vector of daemon threads configuration */
    QList<boost::shared_ptr<DaemonConfiguration::Config> > m_config;

    /** MAC Address of current machine */
    QString m_macAddress;

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
     * @brief C-tor
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
     * @brief Get MAC address
     * @return Only the first non-loopback MAC Address
     */
    QString getMacAddress();

};

} //namespace Daemon
} //namespace TIN_project
#endif // !defined(DAEMON_CONFIGURATION__INCLUDED_)
