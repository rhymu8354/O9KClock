/**
 * @file bob.cpp
 *
 * This is a test program which tracks the difference between
 * time (C standard library), SystemAbstractions::Time, and O9KClock::Time
 * over a very long time.
 *
 * © 2019 by Richard Walters
 */

#include <chrono>
#include <math.h>
#include <O9KClock/Clock.hpp>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <SystemAbstractions/Time.hpp>
#include <thread>

/**
 * This flag indicates whether or not the server should shut down.
 */
bool SHUTDOWN = false;

/**
 * This function is set up to be called when the SIGINT signal is
 * received by the program.  It just sets the "SHUTDOWN" flag
 * and relies on the program to be polling the flag to detect
 * when it's been set.
 *
 * @param[in] sig
 *     This is the signal for which this function was called.
 */
void InterruptHandler(int) {
    SHUTDOWN = true;
}

/**
 * Sample the time according to the O9K::Clock, the SystemAbstractions::Time,
 * and the time function from the C standard library, and report their
 * differences and drift since the first sampling.
 */
void CompareClocks(
    O9K::Clock& o9kClock,
    SystemAbstractions::Time& hiResClock
) {
    const static auto startO9KTime = o9kClock.GetTime();
    const static auto startHiResTime = hiResClock.GetTime();
    const static auto startWallClockTime = std::chrono::system_clock::now();
    static bool firstTime = true;
    if (firstTime) {
        firstTime = false;
        return;
    }
    const auto o9KTime = o9kClock.GetTime();
    const auto hiResTime = hiResClock.GetTime();
    const auto wallClockTime = std::chrono::system_clock::now();
    const auto o9KDiff = o9KTime - startO9KTime;
    const auto hiResDiff = hiResTime - startHiResTime;
    const std::chrono::duration<double> wallClockDiff = wallClockTime - startWallClockTime;
    const auto hiResDrift = hiResDiff - o9KDiff;
    const auto wallClockDrift = wallClockDiff.count() - o9KDiff;
    printf(
        "%lf: hires: %lf  wall: %.7lf\n",
        o9KDiff,
        hiResDrift,
        wallClockDrift
    );
}

/**
 * This function is the entrypoint of the program.
 *
 * @param[in] argc
 *     This is the number of command-line arguments given to the program.
 *
 * @param[in] argv
 *     This is the array of command-line arguments given to the program.
 */
int main(int argc, char* argv[]) {
    // Set up to catch memory leaks.
#ifdef _WIN32
    //_crtBreakAlloc = 18;
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif /* _WIN32 */
    (void)setbuf(stdout, NULL);
    const auto previousInterruptHandler = signal(SIGINT, InterruptHandler);
    const auto wallClockTime1 = std::chrono::system_clock::now();
    std::chrono::time_point< std::chrono::system_clock > wallClockTime2;
    size_t count = 0;
    do {
        wallClockTime2 = std::chrono::system_clock::now();
        ++count;
    } while (std::chrono::duration< double >(wallClockTime2 - wallClockTime1).count() == 0.0);
    printf(
        "Smallest difference in wall clock: %.10lf (in %zu loops)\n",
        std::chrono::duration< double >(wallClockTime2 - wallClockTime1).count(),
        count
    );
    SystemAbstractions::Time hiResClock;
    O9K::Clock o9kClock;
    const static auto startO9KTime = o9kClock.GetTime();
    long long millisecondsToNextSecond = 1000;
    while (!SHUTDOWN) {
        std::this_thread::sleep_for(std::chrono::milliseconds(millisecondsToNextSecond));
        CompareClocks(o9kClock, hiResClock);
        const auto o9KTime = o9kClock.GetTime();
        const auto o9KDiff = o9KTime - startO9KTime;
        millisecondsToNextSecond = 1000 - (long long)(
            (o9KDiff - floor(o9KDiff)) * 1000
        );
    }
    printf("Bob says, \"Goodbye!\"\n");
    return EXIT_SUCCESS;
}
