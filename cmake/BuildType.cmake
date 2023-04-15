# 
# This file is part of the raphsEOdy project.
#
# Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
#
# Distributed under the Boost Software License, Version 1.0.
# (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
#
##

# Define build type as 'Release'
set(DEFAULT_BUILD_TYPE "Release")
# ... unless you're inside a Git repository
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(DEFAULT_BUILD_TYPE "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
    STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
