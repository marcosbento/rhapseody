/*
 * This file is part of the raphsEOdy project.
 *
 * Copyright (c) 2023 Marcos Bento (mb at adlibx dot io)
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See file LICENSE.bsl.txt or find a copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 **/

#include <rhapseody/chrono/Instant.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(use_instant_in_tai)
{
  using rhapseody::chrono::InstantTAI;
  using rhapseody::chrono::Microsecond;
  using rhapseody::chrono::Millisecond;
  using rhapseody::chrono::Nanosecond;
  using rhapseody::chrono::Seconds;

  InstantTAI reference;
  BOOST_CHECK(reference.since_reference() == Nanosecond(0));
  InstantTAI instant;
  BOOST_CHECK(instant.since_reference() == Nanosecond(0));
  BOOST_CHECK(reference == instant);

  instant = reference + Nanosecond(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(1));
  instant = reference + Microsecond(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(1000));
  instant = reference + Millisecond(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(1000000));
  instant = reference + Seconds(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(1000000000));

  instant = reference - Nanosecond(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(-1));
  instant = reference - Microsecond(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(-1000));
  instant = reference - Millisecond(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(-1000000));
  instant = reference - Seconds(1);
  BOOST_CHECK(instant.since_reference() == Nanosecond(-1000000000));
}

BOOST_AUTO_TEST_CASE(use_instant_in_tai_to_convert_to_others)
{
  using rhapseody::chrono::ClockGPS;
  using rhapseody::chrono::ClockTAI;
  using rhapseody::chrono::ClockUT1;
  using rhapseody::chrono::ClockUTC;
  using rhapseody::chrono::InstantGPS;
  using rhapseody::chrono::InstantTAI;
  using rhapseody::chrono::InstantUT1;
  using rhapseody::chrono::InstantUTC;
  using rhapseody::chrono::Microsecond;
  using rhapseody::chrono::Millisecond;
  using rhapseody::chrono::Nanosecond;
  using rhapseody::chrono::Seconds;
  using rhapseody::chrono::Time;

  auto tai = Nanosecond(0);
  auto utc = tai - Seconds(32);
  auto ut1 = utc - Millisecond(4);  // Warning: This ignores Earth Rotation parameters
  auto gps = tai - Seconds(19);

  Time time(tai, utc, ut1, gps);

  InstantTAI reference_tai;

  InstantUTC reference_utc = instant_cast<ClockUTC>(time, reference_tai);
  InstantUT1 reference_ut1 = instant_cast<ClockUT1>(time, reference_tai);
  InstantGPS reference_gps = instant_cast<ClockGPS>(time, reference_tai);

  BOOST_CHECK(reference_tai.since_reference() == reference_utc.since_reference() + Microsecond(32000000));
  BOOST_CHECK(reference_tai.since_reference() == reference_ut1.since_reference() + Microsecond(32004000));
  BOOST_CHECK(reference_tai.since_reference() == reference_gps.since_reference() + Microsecond(19000000));
  BOOST_CHECK(reference_utc.since_reference() == reference_ut1.since_reference() + Microsecond(4000));
  BOOST_CHECK(reference_utc.since_reference() == reference_gps.since_reference() - Microsecond(13000000));
  BOOST_CHECK(reference_ut1.since_reference() == reference_gps.since_reference() - Microsecond(13004000));
}

BOOST_AUTO_TEST_CASE(use_instant_to_convert_between_all_supported_time_standards)
{
  using rhapseody::chrono::ClockGPS;
  using rhapseody::chrono::ClockTAI;
  using rhapseody::chrono::ClockUT1;
  using rhapseody::chrono::ClockUTC;
  using rhapseody::chrono::InstantGPS;
  using rhapseody::chrono::InstantTAI;
  using rhapseody::chrono::InstantUT1;
  using rhapseody::chrono::InstantUTC;
  using rhapseody::chrono::Microsecond;
  using rhapseody::chrono::Millisecond;
  using rhapseody::chrono::Nanosecond;
  using rhapseody::chrono::Seconds;
  using rhapseody::chrono::Time;

  auto tai = Nanosecond(0);
  auto utc = tai - Seconds(32);
  auto ut1 = utc - Millisecond(4);
  auto gps = tai - Seconds(19);
  Time time(tai, utc, ut1, gps);

  {
    InstantTAI reference;
    {
      [[maybe_unused]] auto t_tai = instant_cast<ClockTAI>(time, reference);
      [[maybe_unused]] auto t_utc = instant_cast<ClockUTC>(time, reference);
      [[maybe_unused]] auto t_ut1 = instant_cast<ClockUT1>(time, reference);
      [[maybe_unused]] auto t_gps = instant_cast<ClockGPS>(time, reference);
    }
  }

  {
    InstantUTC reference;
    {
      [[maybe_unused]] auto t_tai = instant_cast<ClockTAI>(time, reference);
      [[maybe_unused]] auto t_utc = instant_cast<ClockUTC>(time, reference);
      [[maybe_unused]] auto t_ut1 = instant_cast<ClockUT1>(time, reference);
      [[maybe_unused]] auto t_gps = instant_cast<ClockGPS>(time, reference);
    }
  }

  {
    InstantUT1 reference;
    {
      [[maybe_unused]] auto t_tai = instant_cast<ClockTAI>(time, reference);
      [[maybe_unused]] auto t_utc = instant_cast<ClockUTC>(time, reference);
      [[maybe_unused]] auto t_ut1 = instant_cast<ClockUT1>(time, reference);
      [[maybe_unused]] auto t_gps = instant_cast<ClockGPS>(time, reference);
    }
  }

  {
    InstantGPS reference;
    {
      [[maybe_unused]] auto t_tai = instant_cast<ClockTAI>(time, reference);
      [[maybe_unused]] auto t_utc = instant_cast<ClockUTC>(time, reference);
      [[maybe_unused]] auto t_ut1 = instant_cast<ClockUT1>(time, reference);
      [[maybe_unused]] auto t_gps = instant_cast<ClockGPS>(time, reference);
    }
  }
}
