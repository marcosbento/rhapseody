/*
 * This file is part of the raphsEOdy project.
 *
 * Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 **/

#ifndef RHAPSEODY_CHRONO_DURATION_HPP
#define RHAPSEODY_CHRONO_DURATION_HPP

#include <rhapseody/chrono/Ratio.hpp>

#include <boost/multiprecision/cpp_int.hpp>

namespace rhapseody::chrono
{

// Forward declarations

template <typename COUNTER, typename RATIO>
class Duration;

// --- Duration Casting

namespace impl___
{

template <typename TO>
struct DurationCast;

template <typename COUNTER, typename TO_TICKS>
struct DurationCast<Duration<COUNTER, TO_TICKS>>
{
  template <typename FROM_TICKS>
  static Duration<COUNTER, TO_TICKS> convert(const Duration<COUNTER, FROM_TICKS>& from)
  {
    auto factor = (TO_TICKS::denominator * FROM_TICKS::numerator) / (TO_TICKS::numerator * FROM_TICKS::denominator);
    return Duration<COUNTER, TO_TICKS>(from.ticks() * factor);
  }
};

template <typename TO, typename FROM>
TO duration_cast(const FROM& from)
{
  return DurationCast<TO>::convert(from);
}

}  // namespace impl___

/// Time Duration

template <typename COUNTER, typename TICK>
class Duration {
public:
  using counter_t = COUNTER;

public:
  Duration() = default;
  explicit Duration(counter_t ticks) : ticks_{std::move(ticks)} {}

  template <typename TICK2>
  explicit Duration(const Duration<COUNTER, TICK2>& rhs)
      : ticks_{impl___::DurationCast<Duration<COUNTER, TICK>>::convert(rhs).ticks()}
  {}

  [[nodiscard]] COUNTER ticks() const { return ticks_; }

  template <typename DURATION_OUT>
  DURATION_OUT as() const
  {
    return impl___::DurationCast<DURATION_OUT>::convert(*this);
  }

private:
  counter_t ticks_;
};

template <typename COUNTER, typename TICK1, typename TICK2>
bool operator==(const Duration<COUNTER, TICK1>& lhs, const Duration<COUNTER, TICK2>& rhs)
{
  return lhs.ticks() == impl___::DurationCast<Duration<COUNTER, TICK1>>::convert(rhs).ticks();
}

template <typename COUNTER, typename TICK1, typename TICK2>
Duration<COUNTER, TICK1> operator+(const Duration<COUNTER, TICK1>& lhs, const Duration<COUNTER, TICK2>& rhs)
{
  return Duration<COUNTER, TICK1>{lhs.ticks() + impl___::DurationCast<Duration<COUNTER, TICK1>>::convert(rhs).ticks()};
}

template <typename COUNTER, typename TICK1, typename TICK2>
Duration<COUNTER, TICK1> operator-(const Duration<COUNTER, TICK1>& lhs, const Duration<COUNTER, TICK2>& rhs)
{
  return Duration<COUNTER, TICK1>{lhs.ticks() - impl___::DurationCast<Duration<COUNTER, TICK1>>::convert(rhs).ticks()};
}

/// Some useful types of Duration

using default_duration_counter_t = boost::multiprecision::int128_t;

using Attosecond   = Duration<default_duration_counter_t, Atto>;
using Femtossecond = Duration<default_duration_counter_t, Femto>;
using Picossecond  = Duration<default_duration_counter_t, Pico>;
using Nanosecond   = Duration<default_duration_counter_t, Nano>;
using Microsecond  = Duration<default_duration_counter_t, Micro>;
using Millisecond  = Duration<default_duration_counter_t, Milli>;
using Second       = Duration<default_duration_counter_t, Ratio<1LL>>;

}  // namespace rhapseody::chrono

#endif  // RHAPSEODY_CHRONO_DURATION_HPP
