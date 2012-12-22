#include "DaemonConfiguration.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QString>
#include <QFile>
#include <QDebug>
#include <stdexcept>

namespace TIN_project {
namespace Daemon {

const char* DaemonConfiguration::DAEMON_CONFIG_FILEPATH = "config_daemon.xml"; // TODO from ${variable} set in installation ?
const char* DaemonConfiguration::DAEMON_CONFIG_XML_THREADS = "threads";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_THREAD = "thread";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_ADDRESS = "address";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_ADDRESS_ATTR_IP = "ip";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_ADDRESS_ATTR_PORT = "port";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_ALIAS = "alias";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_ALIAS_ATTR_ID = "id";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_PASSWORD = "password";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_PASSWORD_ATTR_PASSWORD =
        "password";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_CATALOGUE = "catalogue";
const char* DaemonConfiguration::DAEMON_CONFIG_XML_CATALOGUE_ATTR_PATH = "path";

DaemonConfiguration::DaemonConfiguration()
{
    loadConfiguration();
}

DaemonConfiguration::~DaemonConfiguration()
{
    saveConfiguration();
}

void DaemonConfiguration::loadConfiguration()
{
    if (!QFile(DAEMON_CONFIG_FILEPATH).exists()) {
        // Save empty file for the future
        saveConfiguration();
    } else {
        QFile file(DAEMON_CONFIG_FILEPATH);
        if (!file.open(QFile::ReadOnly | QFile::Text))
            throw std::runtime_error(
                    "Cannot read configuration file while exists.");

        QXmlStreamReader reader(&file);
        reader.readNext();

        boost::shared_ptr<Config> tmp(new Config());

        while (!reader.atEnd()) {
            if (reader.isStartElement()) {
                if (reader.name() == DAEMON_CONFIG_XML_ADDRESS) {
                    QXmlStreamAttributes attribs = reader.attributes();
                    tmp->m_ip =
                            attribs.value(
                                    QString(DAEMON_CONFIG_XML_ADDRESS_ATTR_IP))
                                    .toString();
                    tmp->m_port = attribs.value(
                            QString(DAEMON_CONFIG_XML_ADDRESS_ATTR_PORT))
                            .toString();
                } else if (reader.name() == DAEMON_CONFIG_XML_ALIAS) {
                    tmp->m_aliasId =
                            reader.attributes().value(
                                    QString(DAEMON_CONFIG_XML_ALIAS_ATTR_ID))
                                    .toString();
                } else if (reader.name() == DAEMON_CONFIG_XML_PASSWORD) {
                    tmp->m_password = reader.attributes().value(
                            QString(DAEMON_CONFIG_XML_PASSWORD_ATTR_PASSWORD))
                            .toString();
                } else if (reader.name() == DAEMON_CONFIG_XML_CATALOGUE) {
                    tmp->m_cataloguePath = reader.attributes().value(
                            QString(DAEMON_CONFIG_XML_CATALOGUE_ATTR_PATH))
                            .toString();
                }
            } else if (reader.isEndElement()
                    && reader.name() == DAEMON_CONFIG_XML_THREAD) {
                m_config.append(tmp);
                tmp.reset(new Config());
            }

            reader.readNext();
        }

        file.close();
    }
}

void DaemonConfiguration::saveConfiguration()
{
    QFile file(DAEMON_CONFIG_FILEPATH);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement(DAEMON_CONFIG_XML_THREADS);

    foreach(boost::shared_ptr<Config> it, m_config){
    xmlWriter.writeStartElement(DAEMON_CONFIG_XML_THREAD);

    xmlWriter.writeStartElement(DAEMON_CONFIG_XML_ADDRESS);
    xmlWriter.writeAttribute(DAEMON_CONFIG_XML_ADDRESS_ATTR_IP,
            (*it).m_ip);
    xmlWriter.writeAttribute(DAEMON_CONFIG_XML_ADDRESS_ATTR_PORT,
            (*it).m_port);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement(DAEMON_CONFIG_XML_ALIAS);
    xmlWriter.writeAttribute(DAEMON_CONFIG_XML_ALIAS_ATTR_ID,
            (*it).m_aliasId);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement(DAEMON_CONFIG_XML_PASSWORD);
    xmlWriter.writeAttribute(DAEMON_CONFIG_XML_PASSWORD_ATTR_PASSWORD,
            (*it).m_password);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement(DAEMON_CONFIG_XML_CATALOGUE);
    xmlWriter.writeAttribute(DAEMON_CONFIG_XML_CATALOGUE_ATTR_PATH,
            (*it).m_cataloguePath);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();
}

    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    file.close();
}

const QList<boost::shared_ptr<DaemonConfiguration::Config> >& DaemonConfiguration::getConfig()
{
    return m_config;
}

bool DaemonConfiguration::addConfig(
        boost::shared_ptr<DaemonConfiguration::Config> config)
{
    foreach (boost::shared_ptr<DaemonConfiguration::Config> cnf, m_config){
    if (*cnf == *config)
    return false;
}

m_config.append(config);
return true;
}

bool DaemonConfiguration::removeConfig(const QString &aliasId,
        const QString &path)
{
    boost::shared_ptr<Config> config(new Config(aliasId, path));

    foreach (boost::shared_ptr<DaemonConfiguration::Config> cnf, m_config){
    if (*cnf == *config) {
        m_config.removeAll(cnf);
        return true;
    }
}

    return false;
}

} //namespace Daemon
} //namespace TIN_project
