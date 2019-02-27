/**
 * @file Clock.cpp
 *
 * This module contains the implementation of the O9K::Clock class.
 *
 * © 2018 by Richard Walters
 */

#include <algorithm>
#include <chrono>
#include <math.h>
#include <mutex>
#include <O9KClock/Clock.hpp>

namespace {

    /**
     * This is the ratio of how fast or slow the output clock can be
     * relative to the steady clock.
     */
    constexpr double TOLERANCE = 0.01;

}

namespace O9K {

    /**
     * This contains the private properties of a Clock instance.
     */
    struct Clock::Impl {
        /**
         * This is the function to call in order to sample the system
         * time.  It should return the number of seconds since the
         * UNIX epoch (Midnight UTC January 1, 1970).
         */
        std::function< double() > systemClock = []{
            const auto now = std::chrono::system_clock::now();
            return std::chrono::duration_cast< std::chrono::microseconds >(
                now.time_since_epoch()
            ).count() / 1000000.0;
        };

        /**
         * This is the function to call in order to sample time in a way
         * guaranteed to be monotonic but not necessarily related to
         * system time.  It should return the number of seconds elapsed
         * since an arbitrary point in time which only stays the same
         * as long as the function exists.
         */
        std::function< double() > steadyClock = []{
            const auto now = std::chrono::steady_clock::now();
            return std::chrono::duration_cast< std::chrono::microseconds >(
                now.time_since_epoch()
            ).count() / 1000000.0;
        };

        /**
         * This is the difference between the system clock and the times
         * that the O9K::Clock returns.
         */
        double outputOffset = 0.0;

        /**
         * This is the time reported last by the steady clock.
         */
        double lastSteadyClockSample = 0.0;

        /**
         * This is used to serialize access to the clock.
         */
        std::mutex mutex;

        /**
         * Measure the difference between the steady clock and the system
         * clock, and reset the output offset.
         */
        void Reset() {
            outputOffset = systemClock() - steadyClock();
            lastSteadyClockSample = steadyClock();
        }
    };

    Clock::~Clock() noexcept = default;
    Clock::Clock(Clock&& other) noexcept = default;
    Clock& Clock::operator=(Clock&& other) noexcept = default;

    Clock::Clock()
        : impl_(new Impl)
    {
        impl_->Reset();
    }

    void Clock::SetSystemClock(std::function< double() > systemClock) {
        impl_->systemClock = systemClock;
        impl_->Reset();
    }

    void Clock::SetSteadyClock(std::function< double() > steadyClock) {
        impl_->steadyClock = steadyClock;
        impl_->Reset();
    }

    double Clock::GetTime() {
        std::lock_guard< decltype(impl_->mutex) > lock(impl_->mutex);
        const auto systemTime = impl_->systemClock();
        const auto steadyTime = impl_->steadyClock();
        const auto intervalSinceLastSample = steadyTime - impl_->lastSteadyClockSample;
        const auto now = steadyTime;
        const auto targetOffset = systemTime - now;
        if (fabs(targetOffset - impl_->outputOffset) * intervalSinceLastSample > TOLERANCE) {
            if (targetOffset > impl_->outputOffset) {
                impl_->outputOffset = std::min(
                    targetOffset,
                    impl_->outputOffset + intervalSinceLastSample * TOLERANCE
                );
            } else {
                impl_->outputOffset = std::max(
                    targetOffset,
                    impl_->outputOffset - intervalSinceLastSample * TOLERANCE
                );
            }
        }
        return now + impl_->outputOffset;
    }

}
