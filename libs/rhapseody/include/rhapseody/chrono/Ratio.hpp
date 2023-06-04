/*
 * This file is part of the raphsEOdy project.
 *
 * Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 **/

#ifndef RHAPSEODY_CHRONO_RATIO_HPP
#define RHAPSEODY_CHRONO_RATIO_HPP

#include <cstdint>

namespace rhapseody::chrono
{

namespace impl_
{

// --- compile-time absolute

template <intmax_t V>
struct compile_time_abs
{
  static constexpr intmax_t value = V < 0 ? -V : V;
};

// --- compile-time sign

template <intmax_t V>
struct compile_time_sign
{
  static constexpr intmax_t value = V == 0 ? 0 : (V < 0 ? -1 : +1);
};

// --- compile-time greatest common divider (using Euclides algorithm)

template <intmax_t Vn, intmax_t Vd>
struct compile_time_gcd
{
  static constexpr intmax_t value = compile_time_gcd<Vd, Vn % Vd>::value;
};

template <intmax_t Vn>
struct compile_time_gcd<Vn, 0>
{
  static constexpr intmax_t value = Vn;
};

template <>
struct compile_time_gcd<0, 0>
{
  static constexpr intmax_t value = 1;
};

}  // namespace impl_

// --- Ratio

template <intmax_t Vn, intmax_t Vd = 1LL>
class Ratio {
  static_assert(Vd != 0, "invalid denominator, causes division by 0");

  static constexpr intmax_t vn_abs = impl_::compile_time_abs<Vn>::value;
  static constexpr intmax_t vd_abs = impl_::compile_time_abs<Vd>::value;
  static constexpr intmax_t sign   = impl_::compile_time_sign<Vn>::value * impl_::compile_time_sign<Vd>::value;
  static constexpr intmax_t gcd    = impl_::compile_time_gcd<Vn, Vd>::value;

public:
  static const intmax_t numerator   = sign * vn_abs / gcd;
  static const intmax_t denominator = vd_abs / gcd;

  using type = Ratio<numerator, denominator>;
};

// --- Some Useful Ratios

using Atto  = Ratio<1LL, 1000000000000000000LL>;
using Femto = Ratio<1LL, 1000000000000000LL>;
using Pico  = Ratio<1LL, 1000000000000LL>;
using Nano  = Ratio<1LL, 1000000000LL>;
using Micro = Ratio<1LL, 1000000LL>;
using Milli = Ratio<1LL, 1000LL>;

}  // namespace rhapseody::chrono

#endif  // RHAPSEODY_CHRONO_RATIO_HPP
