# 
# This file is part of the raphsEOdy project.
#
# Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
#
# Distributed under the Boost Software License, Version 1.0.
# (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
#
##

# Catch2
find_package(Catch2 3.3.0 REQUIRED)

# Fmt
find_package(fmt 9.1.0 REQUIRED)

# Boost
set(Boost_NO_BOOST_CMAKE)
find_package(Boost 1.81.0 COMPONENTS system filesystem program_options json REQUIRED)
