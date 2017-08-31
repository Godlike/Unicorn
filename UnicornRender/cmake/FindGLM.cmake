# Copyright (C) 2017 by Godlike
# This code is licensed under the MIT license (MIT)
# (http://opensource.org/licenses/MIT)
#
# FindGLM
# --------
#
# Find GLM include directory.
#
# This module sets the following result variables:
#   GLM_INCLUDE_DIR = directory that contains glm/glm.hpp file path
#   GLM_FOUND       = TRUE if GLM headers were found
#

include(FindPackageHandleStandardArgs)

find_path( GLM_INCLUDE_DIR
    NAMES glm/glm.hpp
    PATHS
        ${GLM_ROOT_DIR}
        /usr/include
        /usr/local/include
        /opt/local/include
        $ENV{PROGRAMFILES}/glm
    DOC "The directory that contains glm/glm.hpp file path")

find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)
