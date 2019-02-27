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
    EXPECT_NEAR(nowO9KClock, nowWallClockTime, 0.01);
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
    EXPECT_NEAR(diffO9KTime, diffHiResTime, 0.0000000001);
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
    EXPECT_NEAR(diffO9KTime, diffHiResTime, 0.0000000001);
}
