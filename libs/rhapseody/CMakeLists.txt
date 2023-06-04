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
    include/rhapseody/chrono/Duration.hpp
    include/rhapseody/chrono/Instant.hpp
    include/rhapseody/chrono/Ratio.hpp
    include/rhapseody/exception/Exceptions.hpp
  SOURCES
    src/rhapseody.cpp
    src/rhapseody/exception/Exceptions.cpp
    src/rhapseody/chrono/Instant.cpp
  INCLUDE_DIRS
    include
  LIBRARIES
    fmt::fmt
    Boost::boost
)

add_rhapseody_utest(
  TARGET rhapseody_ut
  SOURCES
    test/rhapseody_ut.cpp # provides main()
    test/rhapseody_ut_chrono_instant.cpp
    test/rhapseody_ut_exceptions.cpp
  LIBRARIES
    rhapseody
    Boost::unit_test_framework
)
