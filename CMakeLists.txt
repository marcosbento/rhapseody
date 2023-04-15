# 
# This file is part of the raphsEOdy project.
#
# Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
#
# Distributed under the Boost Software License, Version 1.0.
# (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
#
##

cmake_minimum_required(VERSION 3.24.2)

project(rhapseody
  VERSION 0.0.1
  DESCRIPTION "rhapsEOdy"
  HOMEPAGE_URL https://github.com/adlibx/rhapseody
  LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# *** Define Build Type (if not already defined)
include(BuildType)

# *** Specify executables/libraries deployment location
include(DeploymentLocation)

# *** Generic Compiler Setup
include(CompilerFlags)

# *** Setup Testing
option(RHAPSEODY_BUILD_TESTS "Enable building and setting up execution of tests" ON)
if (RHAPSEODY_BUILD_TESTS)
  enable_testing()
endif()

# *** Setup Dependencies
include(Dependencies)

# *** Build libs and tools
include(BuildRecipes)
add_subdirectory(libs)
