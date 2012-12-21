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

namespace TIN_project {

namespace Utilities {

class CommunicationProtocol
{
public:

    enum CommunicateType {
        CONNECT_TO_ALIAS = 1,
        CONNECTED_TO_ALIAS = 2,
        NOT_CONNECTED_TO_ALIAS = 3,
        CREATE_ALIAS = 4,
        ALIAS_CREATED = 5,
        ALIAS_NOT_CREATED = 6,
        REMOVE_ALIAS = 7,
        ALIAS_REMOVED = 8,
        ALIAS_NOT_REMOVED = 9,

        LIST_ALIAS,
        ALIAS_LISTED,
        FIND_FILE,
        FILE_LOCATION,
        FILE_NOT_FOUND,
        PULL_FILE,
        NO_SUCH_FILE,
        TRANSFER_IN_PROGRESS,
        TRANSFER_FINISHED,
        TRANSFER_ERROR,
        DELETE_FROM_ALIAS,
        DELETED_FROM_ALIAS,
        NOT_DELETED_FROM_ALIAS,
        PUSH_FILE,

        RECIVE_FILE,
        SEND_FILE,
        LIST_YOUR_FILES,
        FILES_LIST,
        DELETE_YOUR_FILE,
        FIND_YOUR_FILE,
        NO_SUCH_FILE_HERE,
        FILE_FOUND_HERE,

        ADD_DIRECTORY
    };

    static const char& getCode(CommunicateType type);

    static CommunicateType getType(char code);

    class Communicate
    {
    public:
        Communicate(char code, const QByteArray& data);

        Communicate(char code, const QList<QVariant> &args);

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



    private:
        struct QStringAndPasswordTrait {};
        struct AliasFileListTrait {};
        struct QStringTrait{};
        struct FileLocationTrait {};
        struct QStringAndULongLongTrait {};
        struct EmptyTrait {};



        struct QStringAndPasswordCategory
        {
            typedef QStringAndPasswordTrait FunctionCategory;
        };

        struct AliasFileListCategory
        {
            typedef AliasFileListTrait FunctionCategory;
        };

        struct QStringCategory
        {
            typedef QStringTrait FunctionCategory;
        };

        struct FileLocationCategory
        {
            typedef FileLocationTrait FunctionCategory;
        };

        struct QStringAndULongLongCategory
        {
            typedef QStringAndULongLongTrait FunctionCategory;
        };

        struct EmptyCategory
        {
            typedef EmptyTrait FunctionCategory;
        };

        template<char c> struct CheckCategory : public EmptyCategory {};
        template<

        template <char ch> struct getTrait
        {

        };
        template <class T, int i> void invokeFunctionImpl(T function)
        {
            function();
        }

        template <class T, QSTRING_AND_PASSWORD> void invokeFunctionImpl(T function)
        {
            //to implement
            ;;;;
        }

        template <class T, ALIAS_FILE_LIST> void invokeFunctionImpl(T function)
        {
            //to implement
        }

        template <class T, QSTRING> void invokeFunctionImpl(T function)
        {
            function(m_args.at(0).toString());
        }

        template <class T, FILE_LOCATION> void invokeFunctionImpl(T function)
        {
            //to implement
        }

        template <class T, QSTRING_AND_ULONGLONG> void invokeFunctionImpl(T function)
        {
            function(m_args.at(0).toString(), m_args.at(0).toULongLong());
        }

        char m_code;
        QList<QVariant> m_args;
    };
};

} //namespace Utilities
} //namespace TIN_project
#endif
