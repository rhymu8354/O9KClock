/**
 * @file O9KClockTests.cpp
 *
 * This module contains the unit tests of the
 * O9K::Clock class.
 *
 * © 2018 by Richard Walters
 */

#include <gtest/gtest.h>
#include <O9KClock/Clock.hpp>
#include <SystemAbstractions/Time.hpp>
#include <time.h>

TEST(O9KClockTests, GetTimeLowPrecision) {
    O9K::Clock o9kClock;
    const auto nowO9KClock = o9kClock.GetTime();
    const auto nowWallClockTime = (double)time(NULL);
    EXPECT_NEAR(nowO9KClock, nowWallClockTime, 1.0);
}

TEST(O9KClockTests, GetTimeHighPrecisionShortInterval) {
    O9K::Clock o9kClock;
    SystemAbstractions::Time hiResClock;
    const auto startO9KTime = o9kClock.GetTime();
    const auto startHiResTime = hiResClock.GetTime();
    const auto startWallClockTime = time(NULL);
    double endO9KTime, endHiResTime;
    do {
        endO9KTime = o9kClock.GetTime();
        endHiResTime = hiResClock.GetTime();
    } while (time(NULL) - startWallClockTime < 2);
    const auto diffO9KTime = endO9KTime - startO9KTime;
    const auto diffHiResTime = endHiResTime - startHiResTime;
    EXPECT_NEAR(diffO9KTime, diffHiResTime, 0.00001);
}

TEST(O9KClockTests, GetTimeHighPrecisionLongInterval) {
    O9K::Clock o9kClock;
    SystemAbstractions::Time hiResClock;
    const auto startO9KTime = o9kClock.GetTime();
    const auto startHiResTime = hiResClock.GetTime();
    const auto startWallClockTime = time(NULL);
    double endO9KTime, endHiResTime;
    do {
        endO9KTime = o9kClock.GetTime();
        endHiResTime = hiResClock.GetTime();
    } while (time(NULL) - startWallClockTime < 10);
    const auto diffO9KTime = endO9KTime - startO9KTime;
    const auto diffHiResTime = endHiResTime - startHiResTime;
    EXPECT_NEAR(diffO9KTime, diffHiResTime, 0.00001);
}

TEST(O9KClockTests, HandleSuddenTimeJumps) {
    O9K::Clock o9kClock;
    double nowSystem = 10.0;
    double nowSteady = 0.0;
    o9kClock.SetSystemClock(
        [&nowSystem]{ return nowSystem; }
    );
    o9kClock.SetSteadyClock(
        [&nowSteady]{ return nowSteady; }
    );
    const auto measure1 = o9kClock.GetTime();
    nowSystem -= 1.0; // 9.0
    nowSteady += 0.1; // difference now: -1.1
    const auto measure2 = o9kClock.GetTime();
    nowSystem += 1.3; // 10.3
    nowSteady += 0.1; // difference now: -1.1 + 1.2 = 0.1
    const auto measure3 = o9kClock.GetTime();
    nowSystem += 100.0; // 110.3
    nowSteady += 100.0; // difference still: 0.1
    const auto measure4 = o9kClock.GetTime();
    EXPECT_NEAR(10.0, measure1, 0.000001);
    EXPECT_NEAR(10.099, measure2, 0.000001);
    EXPECT_NEAR(10.201, measure3, 0.000001);
    EXPECT_NEAR(110.3, measure4, 0.000001);
}
