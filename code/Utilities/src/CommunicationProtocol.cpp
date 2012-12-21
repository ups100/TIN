/**
 * @file CommunicationProtocol.cpp
 *
 * @date 21-12-2012
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

#include "CommunicationProtocol.h"

namespace TIN_project {

namespace Utilities {

const char& CommunicationProtocol::getCode(CommunicateType type)
{
    static int i = 0;
    static char codes[END_OF_CODES];
    if (i == 0) {
        for (int j = CONNECT_TO_ALIAS; j < END_OF_CODES; ++j) {
            codes[j] = (char) j;
        }
        ++i;
    }

    return codes[type];
}

CommunicationProtocol::CommunicateType CommunicationProtocol::getType(char code)
{
    if(code < END_OF_CODES)
    {
        return (CommunicateType)code;
    }
    throw "Unknown code";
}

QByteArray CommunicationProtocol::getQByteArrayFromInt(qint32 toConvert)
{
    uchar data[4];
    qToBigEndian(toConvert, data);

    QByteArray buff;
    buff.append(reinterpret_cast<char*>(data), 4);
    return buff;
}

qint32 CommunicationProtocol::getIntFromQByteArray(const QByteArray& toConvert)
{
    uchar data[4];

    for (int i = 0; i < 4; ++i) {
        data[i] = toConvert[i];
    }

    return qFromBigEndian<qint32>(data);
}

CommunicationProtocol::CommunicateName::CommunicateName(const QString& name)
        : m_name(name)
{

}

CommunicationProtocol::CommunicateName::CommunicateName(const QByteArray& data)
: m_name(data)
{

}

QByteArray CommunicationProtocol::CommunicateName::getQByteArray() const
{
    return m_name.toAscii();
}

CommunicationProtocol::CommunicateNameAndPassword::CommunicateNameAndPassword(
        const QString& name, const Password& password)
        : m_name(name), m_password(password)
{

}

CommunicationProtocol::CommunicateNameAndPassword::CommunicateNameAndPassword(
        const QByteArray &data)
{
    if (data.size() < 5)
        throw "wrong size of data";
    qint32 size = CommunicationProtocol::getIntFromQByteArray(data);

    if (data.size() < size + 4)
        throw "wrong size of data";

    QByteArray name = data.mid(5, size);
    m_name.append(name);

    QByteArray password = data.right(data.size() - 4 - size);
    m_password = Password(password);
}

QByteArray CommunicationProtocol::CommunicateNameAndPassword::getQByteArray()
{
    QByteArray name = m_name.toAscii();
    return CommunicationProtocol::getQByteArrayFromInt(name.size()) + name
            + m_password.toQByteArray();
}

CommunicationProtocol::CommunicateNameAndLong::CommunicateNameAndLong(
        const QString& name, qint64 length)
        : m_name(name), m_length(length)
{

}

CommunicationProtocol::CommunicateNameAndLong::CommunicateNameAndLong(
        const QByteArray& data)
{
    if (data.size() < 5)
        throw "wrong size of data";

    QByteArray sizeRaw = data.left(4);
    qint32 size = CommunicationProtocol::getIntFromQByteArray(sizeRaw);

    if (data.size() < size + 4)
        throw "wrong size of data";
    QByteArray name = data.mid(5, size);
    m_name.append(name);

    QByteArray length = data.mid(4 + size);
    uchar rawData[8];

    for (int i = 0; i < 4; ++i) {
        rawData[i] = length[i];
    }

    m_length = qFromBigEndian<qint64>(rawData);
}

QByteArray CommunicationProtocol::CommunicateNameAndLong::getQByteArray() const
{
    QByteArray name = m_name.toAscii();

    uchar data[8];
    qToBigEndian(m_length, data);

    QByteArray buff;
    buff.append(reinterpret_cast<char*>(data), 4);

    return CommunicationProtocol::getQByteArrayFromInt(name.size()) + name
            + buff;
}

} //namespace Utilities
} //namespace TIN_project
