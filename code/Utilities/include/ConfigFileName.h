/**
 * @file ConfigFileName.h
 *
 * @date 05-01-2013
 *
 * @author Marcin Kubik <markubik@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Utilities::ConfigFileName
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

#if !defined(CONFIG_FILE_NAME__INCLUDED_)
#define CONFIG_FILE_NAME__INCLUDED_

namespace TIN_project {
namespace Utilities {

/**
 * Contains name of file that determines, whether client is in good catalog
 */
class ConfigFileName
{
public:
    /** Name of config file, similar syntax as ".git"*/
    static const char *CONFIG_FILE_NAME;
};

} //namespace Utilities
} //namespace TIN_project
#endif // !defined(CONFIG_FILE_NAME__INCLUDED_)
