# CMakeLists.txt for O9KClock
#
# © 2019 by Richard Walters

cmake_minimum_required(VERSION 3.8)
set(This O9KClock)

set(Headers
    include/O9KClock/Clock.hpp
)

set(Sources
    src/Clock.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${This} PUBLIC include)

target_link_libraries(${This} PUBLIC
    SystemAbstractions
)

add_subdirectory(test)

add_executable(Bob src/bob.cpp)
target_link_libraries(Bob PUBLIC
    ${This}
    SystemAbstractions
)
