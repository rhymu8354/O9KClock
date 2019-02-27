# CMakeLists.txt for O9KClockTests
#
# © 2018 by Richard Walters

cmake_minimum_required(VERSION 3.8)
set(This O9KClockTests)

set(Sources
    src/ClockTests.cpp
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests
)

target_include_directories(${This} PRIVATE ..)

target_link_libraries(${This} PUBLIC
    gtest_main
    O9KClock
    SystemAbstractions
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
