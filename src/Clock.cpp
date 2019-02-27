/**
 * @file Clock.cpp
 *
 * This module contains the implementation of the O9K::Clock class.
 *
 * © 2018 by Richard Walters
 */

#include <O9KClock/Clock.hpp>
#include <SystemAbstractions/Time.hpp>
#include <time.h>

namespace O9K {

    /**
     * This contains the private properties of a Clock instance.
     */
    struct Clock::Impl {
        /**
         * This is used to interface with the operating system's notion of time.
         */
        SystemAbstractions::Time time;
    };

    Clock::~Clock() noexcept = default;
    Clock::Clock(Clock&& other) noexcept = default;
    Clock& Clock::operator=(Clock&& other) noexcept = default;

    Clock::Clock()
        : impl_(new Impl)
    {
    }

    double Clock::GetTime() {
        static const auto startTimeHighRes = impl_->time.GetTime();
        static const auto startTimeReal = (double)time(NULL);
        return startTimeReal + (impl_->time.GetTime() - startTimeHighRes);
    }

}
