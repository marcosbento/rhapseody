/*
 * This file is part of the raphsEOdy project.
 *
 * Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 **/

#ifndef RHAPSEODY_CHRONO_INSTANT_HPP
#define RHAPSEODY_CHRONO_INSTANT_HPP

#include <fmt/ostream.h>

#include <chrono>
#include <vector>

#include <rhapseody/chrono/Duration.hpp>
#include <rhapseody/chrono/Ratio.hpp>

namespace rhapseody::chrono
{

/// Time Instant

template <typename CLOCK>
class Instant final {
public:
  using ticks_t = Duration<typename CLOCK::counter_t, Nano>;

  Instant() : ticks_since_reference_{0} {}

  template <typename DURATION>
  explicit Instant(DURATION ticks_since_reference)
      : ticks_since_reference_{impl___::duration_cast<ticks_t>(ticks_since_reference)}
  {}

  [[nodiscard]] inline ticks_t since_reference() const { return ticks_since_reference_; }

private:
  ticks_t ticks_since_reference_;  // considering reference to be 2000-01-01T00:00:00.000000000
};

template <typename CLOCK>
bool operator==(const Instant<CLOCK>& lhs, const Instant<CLOCK>& rhs)
{
  return lhs.since_reference() == rhs.since_reference();
}

template <typename CLOCK>
bool operator!=(const Instant<CLOCK>& lhs, const Instant<CLOCK>& rhs)
{
  return !(lhs == rhs);
}

template <typename CLOCK>
bool operator<(const Instant<CLOCK>& lhs, const Instant<CLOCK>& rhs)
{
  return lhs.since_reference() < rhs.since_reference();
}

template <typename CLOCK>
bool operator<=(const Instant<CLOCK>& lhs, const Instant<CLOCK>& rhs)
{
  return !(rhs < lhs);
}

template <typename CLOCK>
bool operator>(const Instant<CLOCK>& lhs, const Instant<CLOCK>& rhs)
{
  return rhs < lhs;
}

template <typename CLOCK>
bool operator>=(const Instant<CLOCK>& lhs, const Instant<CLOCK>& rhs)
{
  return !(lhs < rhs);
}

template <typename CLOCK, typename DURATION>
Instant<CLOCK> operator+(const Instant<CLOCK>& lhs, const DURATION& rhs)
{
  return Instant<CLOCK>(lhs.since_reference() + impl___::duration_cast<typename Instant<CLOCK>::ticks_t>(rhs));
}

template <typename CLOCK, typename DURATION>
Instant<CLOCK> operator-(const Instant<CLOCK>& lhs, const DURATION& rhs)
{
  return Instant<CLOCK>(lhs.since_reference() - impl___::duration_cast<typename Instant<CLOCK>::ticks_t>(rhs));
}

/// Clock(s), time measuring 'devices' that apply a known Time Standard

struct ClockTAI final
{
  using counter_t = default_duration_counter_t;

  inline static const char* description = "TAI";
};

struct ClockGPS final
{
  using counter_t = default_duration_counter_t;

  inline static const char* description = "GPS";
};

struct ClockUTC final
{
  using counter_t = default_duration_counter_t;

  inline static const char* description = "UTC";
};

struct ClockUT1 final
{
  using counter_t = default_duration_counter_t;

  inline static const char* description = "UT1";
};

/// Some useful types of Instant

using InstantTAI = Instant<ClockTAI>;
using InstantUTC = Instant<ClockUTC>;
using InstantUT1 = Instant<ClockUT1>;
using InstantGPS = Instant<ClockGPS>;

/// Time definition of the correlation between the multiple Clocks

class Time {
public:
  using ticks_t = Nanosecond;

private:
  struct Record
  {
  public:
    Record(ticks_t tai, ticks_t utc, ticks_t ut1, ticks_t gps)
        : tai_{std::move(tai)}, utc_{std::move(utc)}, ut1_{std::move(ut1)}, gps_{std::move(gps)}
    {}

    [[nodiscard]] ticks_t delta_tai_utc() const { return tai_ - utc_; };
    [[nodiscard]] ticks_t delta_tai_ut1() const { return tai_ - ut1_; };
    [[nodiscard]] ticks_t delta_tai_gps() const { return tai_ - gps_; };

  private:
    ticks_t tai_;
    ticks_t utc_;
    ticks_t ut1_;
    ticks_t gps_;
  };

public:
  Time(Nanosecond tai, Nanosecond utc, Nanosecond ut1, Nanosecond gps)
      : correlation_{Record{std::move(tai), std::move(utc), std::move(ut1), std::move(gps)}} {};

  [[nodiscard]] ticks_t delta_tai_utc() const { return correlation_[0].delta_tai_utc(); };
  [[nodiscard]] ticks_t delta_tai_ut1() const { return correlation_[0].delta_tai_ut1(); };
  [[nodiscard]] ticks_t delta_tai_gps() const { return correlation_[0].delta_tai_gps(); };

private:
  std::vector<Record> correlation_;
};

namespace impl___
{

template <typename CLOCK_TO, typename CLOCK_FROM>
struct TimeScaleConverter
{
  static Instant<CLOCK_TO> convert(const Time& time, const Instant<CLOCK_FROM>& instant)
  {
    return TimeScaleConverter<CLOCK_TO, ClockTAI>::convert(
        time, TimeScaleConverter<ClockTAI, CLOCK_FROM>::convert(time, instant));
  }
};

template <>
struct TimeScaleConverter<ClockTAI, ClockTAI>
{
  static Instant<ClockTAI> convert(const Time& time [[maybe_unused]], const Instant<ClockTAI>& instant)
  {
    return instant;
  }
};

template <>
struct TimeScaleConverter<ClockUTC, ClockTAI>
{
  static Instant<ClockUTC> convert(const Time& time, const Instant<ClockTAI>& instant)
  {
    return Instant<ClockUTC>{instant.since_reference() - time.delta_tai_utc()};
  }
};

template <>
struct TimeScaleConverter<ClockUT1, ClockTAI>
{
  static Instant<ClockUT1> convert(const Time& time, const Instant<ClockTAI>& instant)
  {
    return Instant<ClockUT1>{instant.since_reference() - time.delta_tai_ut1()};
  }
};

template <>
struct TimeScaleConverter<ClockGPS, ClockTAI>
{
  static Instant<ClockGPS> convert(const Time& time, const Instant<ClockTAI>& instant)
  {
    return Instant<ClockGPS>{instant.since_reference() - time.delta_tai_gps()};
  }
};

template <>
struct TimeScaleConverter<ClockTAI, ClockUTC>
{
  static Instant<ClockTAI> convert(const Time& time, const Instant<ClockUTC>& instant)
  {
    return Instant<ClockTAI>{instant.since_reference() + time.delta_tai_utc()};
  }
};

template <>
struct TimeScaleConverter<ClockTAI, ClockUT1>
{
  static Instant<ClockTAI> convert(const Time& time, const Instant<ClockUT1>& instant)
  {
    return Instant<ClockTAI>{instant.since_reference() + time.delta_tai_ut1()};
  }
};

template <>
struct TimeScaleConverter<ClockTAI, ClockGPS>
{
  static Instant<ClockTAI> convert(const Time& time, const Instant<ClockGPS>& instant)
  {
    return Instant<ClockTAI>{instant.since_reference() + time.delta_tai_gps()};
  }
};

}  // namespace impl___

template <typename CLOCK_TO, typename CLOCK_FROM>
Instant<CLOCK_TO> instant_cast(const Time& time, const Instant<CLOCK_FROM>& from)
{
  return impl___::TimeScaleConverter<CLOCK_TO, CLOCK_FROM>::convert(time, from);
}

}  // namespace rhapseody::chrono

#endif  // RHAPSEODY_CHRONO_INSTANT_HPP
