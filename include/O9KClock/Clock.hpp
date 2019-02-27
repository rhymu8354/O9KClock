#pragma once

/**
 * @file Clock.hpp
 *
 * This module declares the O9K::Clock class.
 *
 * © 2018 by Richard Walters
 */

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
