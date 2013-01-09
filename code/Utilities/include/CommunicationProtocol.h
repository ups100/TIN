/**
 * @file CommunicationProtocol.h
 *
 * @date 19-12-2012
 *
 * @author Opasiak Krzsztof <ups100@tlen.pl>
 *
 * @brief Implementation of the Class TIN_project::Utilities::CommunicationProtocol
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#ifndef __COMMUNICATION_PROTOCOL_H__
#define __COMMUNICATION_PROTOCOL_H__

#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QString>
#include <QtEndian>
#include <QtGlobal>
#include <QHostAddress>

#include "Password.h"
#include "AliasFileList.h"
#include "FileLocation.h"
#include "Message.h"

namespace TIN_project {

namespace Utilities {

/**
 * @brief Implementation of project's communication protocol
 *
 * @details This class holds informations about messages passed via network
 *
 * @par Usage
 * If you would like to send some communicate just select it from
 * CommunicationProtocol::CommunicateType and create it's object using
 * CommunicationProtocol::Communicate templates.
 *
 * @warning If you would like to change some communication code, remember to change template
 * Specialization
 */
class CommunicationProtocol
{
public:

    /**
     * @brief Types of communicates.
     *
     * @warning Never use values! Use only enum!
     */
    enum CommunicateType
    {
        CONNECT_TO_ALIAS = 0,
        CONNECTED_TO_ALIAS = 1,
        NOT_CONNECTED_TO_ALIAS = 2,
        CREATE_ALIAS = 3,
        ALIAS_CREATED = 4,
        ALIAS_NOT_CREATED = 5,
        REMOVE_ALIAS = 6,
        ALIAS_REMOVED = 7,
        ALIAS_NOT_REMOVED = 8,

        LIST_ALIAS = 9,
        ALIAS_LISTED = 10,
        FIND_FILE = 11,
        FILE_LOCATION = 12,
        FILE_NOT_FOUND = 13,
        PULL_FILE = 14,
        NO_SUCH_FILE = 15,
        TRANSFER_IN_PROGRESS = 16,
        TRANSFER_FINISHED = 17,
        TRANSFER_ERROR = 18,
        DELETE_FROM_ALIAS = 19,
        DELETED_FROM_ALIAS = 20,
        NOT_DELETED_FROM_ALIAS = 21,
        PUSH_FILE = 22,

        RECIVE_FILE = 23,
        SEND_FILE = 24,
        LIST_YOUR_FILES = 25,
        FILES_LIST = 26,
        DELETE_YOUR_FILE = 27,
        FIND_YOUR_FILE = 28,
        NO_SUCH_FILE_HERE = 29,
        FILE_FOUND_HERE = 30,

        ADD_DIRECTORY = 31,

        ADD_DIRECTORY_AND_CONNECT = 32,
        REMOVE_DIRECTORY_AND_DISCONNECT = 33,
        END_OF_CODES = 34
    };

    /**
     * @brief Gets the code value from enumeration
     *
     * @param[in] type enumeration which determines type of message
     *
     * @return code of message
     */
    static const char& getCode(CommunicateType type);

    /**
     * @brief Gets the enumeration from message code
     *
     * @param code of the message
     *
     * @return enumeration type of message
     */
    static CommunicateType getType(char code);

    /*
     template<class T> void invokeFunction(T function)
     {
     switch (CommunicationProtocol::getType(m_code)) {
     case CONNECT_TO_ALIAS:
     case CREATE_ALIAS:
     case REMOVE_ALIAS: //nazwa aliasu i haslo
     case ADD_DIRECTORY:
     groupe = QSTRING_AND_PASSWORD;
     break;

     case ALIAS_LISTED: // alias file list
     case FILES_LIST:
     groupe = ALIAS_FILE_LIST;
     break;

     case FIND_FILE: // nazwa
     case DELETE_FROM_ALIAS:
     case RECIVE_FILE:
     case SEND_FILE: // nazwa
     case DELETE_YOUR_FILE: //nazwa
     case FIND_YOUR_FILE:
     groupe = QSTRING;
     break;

     case FILE_LOCATION: //fileLocation
     case FILE_FOUND_HERE:
     case PULL_FILE:
     groupe = FILE_LOCATION;
     break;

     case PUSH_FILE: //nazwa, dlugosc pliku
     groupe = QSTRING_AND_ULONGLONG;
     break;

     default:
     groupe = EMPTY;
     break;
     }
     return groupe;
     }
     */

    /**
     * @brief Converts 4 bytes integer to array of bytes in Big Endian.
     *
     * @param[in] toConvert Integer to be converted
     *
     * @return Array of bytes with converted integer
     */
    static QByteArray getQByteArrayFromInt(qint32 toConvert);

    /**
     * @brief Converts from array of bytes to 4 bytes integer in machine endianess.
     *
     * @param[in] toConvert array with raw data to be converted to 4 bytes integer
     *
     * @return Integer after conversion
     *
     * @warning There is no error checking, you should ensure that array
     * is capable to be converted
     */
    static qint32 getIntFromQByteArray(const QByteArray& toConvert);

    /**
     * @brief Base class for all communicates.
     */
    class CommunicateBase
    {
    public:
        /**
         * @brief Destructor
         */
        virtual ~CommunicateBase()
        {

        }

        /**
         * @brief Forms message in ready to send form
         */
        virtual QByteArray toQByteArray() = 0;
    };

    /**
     * @brief Base class for QString converting.
     */
    class CommunicateName
    {
    public:
        /**
         * @brief Gets the name of something.
         *
         * @return name of something
         */
        inline const QString& getName() const
        {
            return m_name;
        }

    protected:

        /**
         * @brief Constructor
         *
         * @param[in] name QString to be converted
         */
        CommunicateName(const QString& name);

        /**
         * @brief Constructor
         *
         * @param[in] data to form an object
         */
        CommunicateName(const QByteArray& data);

        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        QByteArray getQByteArray() const;

    private:
        /**
         * @brief Name to be converted
         */
        QString m_name;
    };

    /**
     * @brief Base class for address and port converting
     */
    class CommunicateNameAddressAndPort
    {
    public:
        /**
         * @brief Gets the name of something.
         *
         * @return name of something
         */
        inline const QString& getName() const
        {
            return m_name;
        }

        /**
         * @brief Gets the address.
         *
         * @return IP address
         */
        inline const QHostAddress& getAddress() const
        {
            return m_address;
        }

        /**
         * @brief Gets the port
         *
         * @return port
         */
        inline quint16 getPort() const
        {
            return m_port;
        }

    protected:

        /**
         * @brief Constructor
         *
         * @param[in] name of something
         *
         * @param[in] address address to be converted
         *
         * @param[in] port
         */
        CommunicateNameAddressAndPort(const QString& name, const QHostAddress& address,
                quint16 port);

        /**
         * @brief Constructor
         *
         * @param[in] data to form an object
         */
        CommunicateNameAddressAndPort(const QByteArray& data);

        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        QByteArray getQByteArray() const;

    private:

        /**
         * @brief Name to be converted
         */
        QString m_name;

        /**
         * @brief Address to be converted
         */
        QHostAddress m_address;

        /**
         * @brief Port to be converted
         */
        quint16 m_port;
    };

    /**
     * @brief Base class for address, port and size converting
     */
    class CommunicateNameAddressPortAndSize
    {
    public:
        /**
         * @brief Gets the name of something.
         *
         * @return name of something
         */
        inline const QString& getName() const
        {
            return m_name;
        }

        /**
         * @brief Gets the address.
         *
         * @return IP address
         */
        inline const QHostAddress& getAddress() const
        {
            return m_address;
        }

        /**
         * @brief Gets the port
         *
         * @return port
         */
        inline quint16 getPort() const
        {
            return m_port;
        }

        /**
         * @brief Gets the port
         *
         * @return port
         */
        inline qint64 getSize() const
        {
            return m_size;
        }

    protected:

        /**
         * @brief Constructor
         *
         * @param[in] name of something
         *
         * @param[in] address address to be converted
         *
         * @param[in] port
         *
         * @param[in] size
         */
        CommunicateNameAddressPortAndSize(const QString& name, const QHostAddress& address,
                quint16 port, qint64 size);

        /**
         * @brief Constructor
         *
         * @param[in] data to form an object
         */
        CommunicateNameAddressPortAndSize(const QByteArray& data);

        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        QByteArray getQByteArray() const;

    private:

        /**
         * @brief Name to be converted
         */
        QString m_name;

        /**
         * @brief Address to be converted
         */
        QHostAddress m_address;

        /**
         * @brief Port to be converted
         */
        quint16 m_port;

        /**
         * @brief Size to be converted
         */
        qint64 m_size;
    };

    /**
     * @brief Base class for QString and Password converting.
     */
    class CommunicateNameAndPassword
    {
    public:

        /**
         * @brief Gets the name of something.
         *
         * @return name of something
         */
        inline const QString& getName() const
        {
            return m_name;
        }

        /**
         * @brief Gets the password.
         *
         * @return password
         */
        inline const Password& getPassword() const
        {
            return m_password;
        }

    protected:
        /**
         * @brief Constructor
         *
         * @param[in] name QString to be converted
         *
         * @param[in] password Password to be converted
         */
        CommunicateNameAndPassword(const QString& name,
                const Password& password);

        /**
         * @brief Constructor
         *
         * @param[in] data to form an object
         */
        CommunicateNameAndPassword(const QByteArray &data);

        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        QByteArray getQByteArray();

    private:
        /**
         * @brief Name to be converted
         */
        QString m_name;

        /**
         * @brief Password to be converted
         */
        Password m_password;
    };

    /**
     * @brief Base class for AliasFileList list converting.
     */
    class CommunicateAliasFileList
    {
    public:
        /**
         * @brief Gets the list
         *
         * @return list of files
         */
        inline const AliasFileList& getList()
        {
            return m_list;
        }
    protected:

        /**
         * @brief Constructor
         *
         * @param[in] list File list to be converted
         */
        inline CommunicateAliasFileList(const AliasFileList& list)
                : m_list(list)
        {

        }

        inline CommunicateAliasFileList(const QByteArray &data)
                : m_list(data)
        {

        }
        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        inline QByteArray getQByteArray()
        {
            return m_list.toQByteArray();
        }

    private:
        /**
         * @brief List of files to be converted.
         */
        AliasFileList m_list;
    };

    /**
     * @brief Base class for FileLocation converting
     */
    class CommunicateAliasFileLocation
    {
    public:
        /**
         * @brief Gets the file location
         */
        inline const FileLocation& getLocation()
        {
            return m_location;
        }
    protected:

        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        inline QByteArray getQByteArray()
        {
            return m_location.toQByteArray();
        }

        /**
         * @brief Constructor
         *
         * @param[in] location of file to be converted
         */
        inline CommunicateAliasFileLocation(const FileLocation& location)
                : m_location(location)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to construct the object
         */
        inline CommunicateAliasFileLocation(const QByteArray &data)
                : m_location(data)
        {

        }

    private:
        /**
         * @brief File location to be converted
         */
        FileLocation m_location;
    };

    /**
     * @brief Base class for QString and qint64 conversion.
     */
    class CommunicateNameAndLong
    {
    public:
        /**
         * @brief Gets the name of something.
         *
         * @return name of something
         */
        inline const QString& getName() const
        {
            return m_name;
        }

        /**
         * @brief Gets the length of something
         *
         * @return length of something
         */
        inline qint64 getLength()
        {
            return m_length;
        }
    protected:
        /**
         * @brief Constructor
         *
         * @param[in] name QString to be converted
         *
         * @param[in] length qint64 to be converted
         */
        CommunicateNameAndLong(const QString& name, qint64 length);

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to construct the object
         */
        CommunicateNameAndLong(const QByteArray& data);

        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        QByteArray getQByteArray() const;

    private:
        /**
         * @brief QString to be converted
         */
        QString m_name;

        /**
         * @brief qint64 to be converted.
         */
        qint64 m_length;
    };

    /**
     * @brief Base class for Message converting
     */
    class CommunicateMessage
    {
    public:
        /**
         * @brief Gets message
         */
        inline const Message& getMessage()
        {
            return m_message;
        }
    protected:

        /**
         * @brief Converts an object to QByteArray
         *
         * @return Raw object data
         */
        inline QByteArray getQByteArray()
        {
            return m_message.toQByteArray();
        }

        /**
         * @brief Constructor
         *
         * @param[in] message to be converted
         */
        inline CommunicateMessage(const Message& message)
                : m_message(message)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to construct the object
         */
        inline CommunicateMessage(const QByteArray &data)
                : m_message(data)
        {

        }

    private:
        /**
         * @brief Message to be converted
         */
        Message m_message;
    };

    /**
     * @brief Template class for handling preparing communicates.
     *
     * @details Use this class to create or parse well-formed communicate
     *
     * @tparam code of the message to be formed
     */
    template<char code, typename T = void> class Communicate : public CommunicateBase
    {
    public:
        virtual QByteArray toQByteArray()
        {
            return QByteArray() + code;
        }
    };

    /**
     * @brief Specialization of template class for #CONNECT_TO_ALIAS message
     */
    template<typename T> class Communicate<0, T> : public CommunicateBase,
            public CommunicateNameAndPassword
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name alias name
         *
         * @param[in] password alias password
         */
        Communicate(const QString& name, const Password& password)
                : CommunicateNameAndPassword(name, password)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateNameAndPassword(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateNameAndPassword::getQByteArray();
            return QByteArray()
                    + CommunicationProtocol::getCode(CONNECT_TO_ALIAS)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #CREATE_ALIAS message
     */
    template<typename T> class Communicate<3, T> : public CommunicateBase,
            public CommunicateNameAndPassword
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name alias name
         *
         * @param[in] password alias password
         */
        Communicate(const QString& name, const Password& password)
                : CommunicateNameAndPassword(name, password)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateNameAndPassword(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateNameAndPassword::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(CREATE_ALIAS)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #REMOVE_ALIAS message
     */
    template<typename T> class Communicate<6, T> : public CommunicateBase,
            public CommunicateNameAndPassword
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name alias name
         *
         * @param[in] password alias password
         */
        Communicate(const QString& name, const Password& password)
                : CommunicateNameAndPassword(name, password)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateNameAndPassword(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateNameAndPassword::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(REMOVE_ALIAS)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #ADD_DIRECTORY message
     */
    template<typename T> class Communicate<31, T> : public CommunicateBase,
            public CommunicateNameAndPassword
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name alias name
         *
         * @param[in] password alias password
         */
        Communicate(const QString& name, const Password& password)
                : CommunicateNameAndPassword(name, password)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateNameAndPassword(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateNameAndPassword::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(ADD_DIRECTORY)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #ALIAS_LISTED message
     */
    template<typename T>
    class Communicate<10, T> : public CommunicateBase,
            public CommunicateAliasFileList
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] list of files
         */
        Communicate(const AliasFileList &list)
                : CommunicateAliasFileList(list)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateAliasFileList(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateAliasFileList::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(ALIAS_LISTED)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #FILES_LIST message
     */
    template<typename T>
    class Communicate<26, T> : public CommunicateBase,
            public CommunicateAliasFileList
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] list of files
         */
        Communicate(const AliasFileList &list)
                : CommunicateAliasFileList(list)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateAliasFileList(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateAliasFileList::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(FILES_LIST)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #FIND_FILE message
     */
    template<typename T>
    class Communicate<11, T> : public CommunicateBase, public CommunicateName
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name of file
         */
        Communicate(const QString &name)
                : CommunicateName(name)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateName(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateName::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(FIND_FILE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #DELETE_FROM_ALIAS message
     */
    template<typename T>
    class Communicate<19, T> : public CommunicateBase, public CommunicateName
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name of file (relative path)
         */
        Communicate(const QString &name)
                : CommunicateName(name)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateName(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateName::getQByteArray();
            return QByteArray()
                    + CommunicationProtocol::getCode(DELETE_FROM_ALIAS)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #RECIVE_FILE message
     */
    template<typename T>
    class Communicate<23, T> : public CommunicateBase,
            public CommunicateNameAddressPortAndSize
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name of file (relative path)
         *
         * @param[in] address of server to receive file from
         *
         * @param[in] port of server to receive file from
         *
         * @param[in] size of file to be received
         */
        Communicate(const QString &name, const QHostAddress& address,
                quint16 port, qint64 size)
            :CommunicateNameAddressPortAndSize(name, address, port, size)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateNameAddressPortAndSize(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateNameAddressPortAndSize::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(RECIVE_FILE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #SEND_FILE message
     */
    template<typename T>
    class Communicate<24, T> : public CommunicateBase, public CommunicateNameAddressAndPort
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name of file (relative path)
         *
         * @param[in] address of server to upload file
         *
         * @param[in] port of server to upload file
         */
        Communicate(const QString &name, const QHostAddress& address,
                quint16 port)
                : CommunicateNameAddressAndPort(name, address, port)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateNameAddressAndPort(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateNameAddressAndPort::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(SEND_FILE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #DELETE_YOUR_FILE message
     */
    template<typename T>
    class Communicate<27, T> : public CommunicateBase, public CommunicateName
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name of file (relative path)
         */
        Communicate(const QString &name)
                : CommunicateName(name)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateName(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateName::getQByteArray();
            return QByteArray()
                    + CommunicationProtocol::getCode(DELETE_YOUR_FILE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #FIND_YOUR_FILE message
     */
    template<typename T>
    class Communicate<28, T> : public CommunicateBase, public CommunicateName
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name of file
         */
        Communicate(const QString &name)
                : CommunicateName(name)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateName(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateName::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(FIND_YOUR_FILE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #FILE_LOCATION message
     */
    template<typename T>
    class Communicate<12, T> : public CommunicateBase,
            public CommunicateAliasFileList
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] location of file
         */
        Communicate(const AliasFileList &location)
                : CommunicateAliasFileList(location)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateAliasFileList(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateAliasFileList::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(FILE_LOCATION)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #FILE_FOUND_HERE message
     */
    template<typename T>
    class Communicate<30, T> : public CommunicateBase,
            public CommunicateAliasFileList
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] location of file
         */
        Communicate(const AliasFileList &location)
                : CommunicateAliasFileList(location)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateAliasFileList(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateAliasFileList::getQByteArray();
            return QByteArray()
                    + CommunicationProtocol::getCode(FILE_FOUND_HERE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #PULL_FILE message
     */
    template<typename T>
    class Communicate<14, T> : public CommunicateBase,
            public CommunicateAliasFileLocation
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] location of file
         */
        Communicate(const FileLocation &location)
                : CommunicateAliasFileLocation(location)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateAliasFileLocation(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateAliasFileLocation::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(PULL_FILE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #PUSH_FILE message
     */
    template<typename T>
    class Communicate<22, T> : public CommunicateBase,
            public CommunicateNameAndLong
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] name of file
         *
         * @param[in] size of file
         */
        Communicate(const QString &name, qint64 size)
                : CommunicateNameAndLong(name, size)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateNameAndLong(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateNameAndLong::getQByteArray();
            return QByteArray() + CommunicationProtocol::getCode(PUSH_FILE)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #ADD_DIRECTORY_AND_CONNECT message
     */
    template<typename T>
    class Communicate<32, T> : public CommunicateBase,
            public CommunicateMessage
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] message
         */
        Communicate(const Message &message)
                : CommunicateMessage(message)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateMessage(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateMessage::getQByteArray();
            return QByteArray()
                    + CommunicationProtocol::getCode(ADD_DIRECTORY_AND_CONNECT)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };

    /**
     * @brief Specialization of template class for #REMOVE_DIRECTORY_AND_DISCONNECT message
     */
    template<typename T>
    class Communicate<33, T> : public CommunicateBase,
            public CommunicateMessage
    {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] message
         */
        Communicate(const Message &message)
                : CommunicateMessage(message)
        {

        }

        /**
         * @brief Constructor
         *
         * @param[in] data raw data to parse message.
         */
        Communicate(const QByteArray &data)
                : CommunicateMessage(data)
        {

        }

        virtual QByteArray toQByteArray()
        {
            QByteArray buff = CommunicateMessage::getQByteArray();
            return QByteArray()
                    + CommunicationProtocol::getCode(REMOVE_DIRECTORY_AND_DISCONNECT)
                    + CommunicationProtocol::getQByteArrayFromInt(buff.size())
                    + buff;
        }

    };
};

} //namespace Utilities
} //namespace TIN_project
#endif
