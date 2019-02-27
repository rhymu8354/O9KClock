/**
 * @file Clock.cpp
 *
 * This module contains the implementation of the O9K::Clock class.
 *
 * © 2018 by Richard Walters
 */

#include <chrono>
#include <O9KClock/Clock.hpp>

namespace O9K {

    /**
     * This contains the private properties of a Clock instance.
     */
    struct Clock::Impl {
    };

    Clock::~Clock() noexcept = default;
    Clock::Clock(Clock&& other) noexcept = default;
    Clock& Clock::operator=(Clock&& other) noexcept = default;

    Clock::Clock()
        : impl_(new Impl)
    {
    }

    double Clock::GetTime() {
        const auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast< std::chrono::microseconds >(
            now.time_since_epoch()
        ).count() / 1000000.0;
    }

}
