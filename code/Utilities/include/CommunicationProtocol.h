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
        CONNECT_TO_ALIAS,
        CONNECTED_TO_ALIAS,
        NOT_CONNECTED_TO_ALIAS,
        CREATE_ALIAS,
        ALIAS_CREATED,
        ALIAS_NOT_CREATED,
        REMOVE_ALIAS,
        ALIAS_REMOVED,
        ALIAS_NOT_REMOVED,

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

        template <class T> void invokeFunction(T function, char code = m_code)
        {
            switch(CommunicationProtocol::getType(m_code)) {
                case CONNECT_TO_ALIAS:
                case CREATE_ALIAS:
                case REMOVE_ALIAS: //nazwa aliasu i haslo
                case ADD_DIRECTORY:
                    break;

                case ALIAS_LISTED: // alias file list
                case FILES_LIST:
                    break;

                case FIND_FILE: // nazwa
                case DELETE_FROM_ALIAS:
                case RECIVE_FILE:
                case SEND_FILE: // nazwa
                case DELETE_YOUR_FILE: //nazwa
                case FIND_YOUR_FILE:
                    function(m_args.at(0).toString());
                    break;

                case FILE_LOCATION: //fileLocation
                case FILE_FOUND_HERE:
                case PULL_FILE:
                    break;

                case PUSH_FILE: //nazwa, dlugosc pliku
                    function(m_args.at(0).toString(), m_args.at(0).toULongLong());
                    break;

                default:
                    function();
                    break;
            }
        }



    private:
        enum ParameterGroupe {

        };
        char m_code;
        QList<QVariant> m_args;
    };
};

} //namespace Utilities
} //namespace TIN_project
#endif
