/**
 * @file ConfigFileName.cpp
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

#include "ConfigFileName.h"

namespace TIN_project {
namespace Utilities {

const char *ConfigFileName::CONFIG_FILE_NAME = ".TIN_project";

} //namespace Utilities
} //namespace TIN_project
