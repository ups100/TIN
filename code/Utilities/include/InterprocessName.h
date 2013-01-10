/**
 * @file InterprocessName.h
 *
 * @date 04-01-2013
 *
 * @author Mikolaj Markiewicz <kajo100@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::InterprocessName
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(INTERPROCESS_NAME__INCLUDED_)
#define INTERPROCESS_NAME__INCLUDED_

namespace TIN_project {
namespace Utilities {

/**
 * Contains interprocess socket name for Client - Daemon communication
 */
class InterprocessName
{
public:
    /** Name for Unix socket to client-daemon communication */
    static const char *INTERPROCESS_NAME;
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(INTERPROCESS_NAME__INCLUDED_)
