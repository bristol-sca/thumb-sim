#[=[
    MIT License

    Copyright (c) 2018 Scott Egerton

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
#]=]

option(${PROJECT_NAME}_BUILD_EXECUTABLE
    "Build ${PROJECT_NAME} as a command line executable. (The library will be built regardless)"
    ON
)

# An option exposed to provide a user settable cache value
# with project name prefix.
set(${PROJECT_NAME}_DEFAULT_MEMORY_SIZE
    "0xA00000" CACHE STRING
    "The default amount of memory that will be used for the simulation \
    in hexidecimal bytes. This can be overriden via the simulator constructor.")

# A local version of the user settable value, without the project prefix.
# This is used to configure files so that the values can persist even if the
# project name changes or this code is included elsewhere.
set(DEFAULT_MEMORY_SIZE ${${PROJECT_NAME}_DEFAULT_MEMORY_SIZE})

# Configure a header file to pass some of the CMake settings to the source code
configure_file(
    "${PROJECT_SOURCE_DIR}/include/simulator/config.h.in"
    "${PROJECT_SOURCE_DIR}/include/simulator/config.h"
    #"${CMAKE_CURRENT_BINARY_DIR}/include/simulator/config.h"
    @ONLY
)

# Add library called that is built from the source files
add_library(lib${PROJECT_NAME}
    fetch.cpp
    memory.cpp
    processor.cpp
    regfile.cpp
    decode.cpp
    execute.cpp
    alu.cpp
    lsu.cpp
    branch.cpp
    misc.cpp
    stats.cpp
    simulator.cpp
)

# C++17 for string_view
target_compile_features(lib${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(lib${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/library>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_compile_options(lib${PROJECT_NAME}
    PUBLIC
        -fPIC
)

set_target_properties(lib${PROJECT_NAME}
    PROPERTIES
        # Removes automatic lib prefix, to stop it being called liblibSimulator
        PREFIX ""
)

if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    # Add executable that is a command line wrapper around the library
    add_executable(${PROJECT_NAME} main.cpp)

    target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include)
endif()
