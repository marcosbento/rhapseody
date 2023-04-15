# 
# This file is part of the raphsEOdy project.
#
# Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
#
# Distributed under the Boost Software License, Version 1.0.
# (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
#
##

add_rhapseody_library(
  TARGET rhapseody
  HEADERS
    include/rhapseody.hpp
  SOURCES
    src/rhapseody.cpp
  INCLUDE_DIRS
    include
)
