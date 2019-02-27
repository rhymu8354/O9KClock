#pragma once

/**
 * @file Clock.hpp
 *
 * This module declares the O9K::Clock class.
 *
 * © 2018 by Richard Walters
 */

#include <functional>
#include <memory>

namespace O9K {

    /**
     * This class implements a high-precision real-time clock, by combining the
     * separate real-time and high-precision clocks provided by the operating
     * system.
     */
    class Clock {
        // Lifecycle management
    public:
        ~Clock() noexcept;
        Clock(const Clock&) = delete;
        Clock(Clock&&) noexcept;
        Clock& operator=(const Clock&) = delete;
        Clock& operator=(Clock&&) noexcept;

        // Public methods
    public:
        /**
         * This is the default constructor.
         */
        Clock();

        /**
         * Set the function used to sample the system time.
         *
         * @param[in] systemClock
         *     This is the function to call in order to sample the system
         *     time.  It should return the number of seconds since the
         *     UNIX epoch (Midnight UTC January 1, 1970).
         */
        void SetSystemClock(std::function< double() > systemClock);

        /**
         * Set the function used to sample time in a way guaranteed
         * to be monotonic but not necessarily related to system time.
         *
         * @param[in] steadyClock
         *     This is the function to call in order to sample time in a way
         *     guaranteed to be monotonic but not necessarily related to
         *     system time.  It should return the number of seconds elapsed
         *     since an arbitrary point in time which only stays the same
         *     as long as the function exists.
         */
        void SetSteadyClock(std::function< double() > steadyClock);

        /**
         * Sample and return the current time according to the clock.
         *
         * @return
         *     The current time according to the clock, in seconds,
         *     is returned.
         */
        double GetTime();

        // Private properties
    private:
        /**
         * This is the type of structure that contains the private
         * properties of the instance.  It is defined in the implementation
         * and declared here to ensure that it is scoped inside the class.
         */
        struct Impl;

        /**
         * This contains the private properties of the instance.
         */
        std::unique_ptr< Impl > impl_;
    };

}
