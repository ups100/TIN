/**
 * @file Identify.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::Identify
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(IDENTIFY__INCLUDED_)
#define IDENTIFY__INCLUDED_

#include <QString>

namespace TIN_project {
namespace Utilities {

/**
 * Class responsible for getting identification data of actual machine
 *
 * @author Kajo0
 */
class Identify
{
    /**
     * @brief Private C-tor
     */
    Identify();

    /**
     * @brief Private Copying C-tor
     */
    Identify(const Identify &other);

    /**
     * @brief Private Assignment operator
     */
    Identify& operator=(const Identify &other);

public:

    /*
     * @brief D-tor
     */
    virtual ~Identify();

    /**
     * @brief Get MAC address
     * @return Only the first non-loopback MAC Address
     */
    static QString getMachineIdentificator();

};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(IDENTIFY__INCLUDED_)
