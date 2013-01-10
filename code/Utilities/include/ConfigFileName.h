/*
 * ConfigFileName.h
 *
 *  Created on: 05-01-2013
 *      Author: marcin
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
