/*
 * This file is part of the raphsEOdy project.
 *
 * Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 **/

#ifndef RHAPSEODY_EXCEPTION_EXCEPTIONS_HPP
#define RHAPSEODY_EXCEPTION_EXCEPTIONS_HPP

#include <fmt/core.h>
#include <stdexcept>

namespace rhapseody::exception
{

/// Useful Exceptions

struct InvalidArgument : public std::runtime_error
{
  template <typename FMT, typename... ARGS>
  explicit InvalidArgument(FMT&& fmt, ARGS... args)
      : std::runtime_error(fmt::format(std::forward<FMT>(fmt), std::forward<ARGS>(args)...))
  {}

  ~InvalidArgument() override;
};

}  // namespace rhapseody::exception

#endif  // RHAPSEODY_EXCEPTION_EXCEPTIONS_HPP
