/*
 * This file is part of the raphsEOdy project.
 *
 * Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 **/

#include <rhapseody/exception/Exceptions.hpp>

#include <boost/test/unit_test.hpp>

template<typename E>
void throw_me(std::string_view message) {
  throw E(fmt::runtime(message));
}

BOOST_AUTO_TEST_CASE(use_invalid_argument_exceptions)
{
  using rhapseody::exception::InvalidArgument;
  BOOST_REQUIRE_THROW(throw_me<rhapseody::exception::InvalidArgument>("Invalid argument detected"), InvalidArgument);
}
