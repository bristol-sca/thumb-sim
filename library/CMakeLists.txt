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

set(CMAKE_CXX_STANDARD 17) # C++17 for string_view
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

target_include_directories(lib${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Removes lib prefix that is auto added, to stop it being called liblibSimulator
SET_TARGET_PROPERTIES(lib${PROJECT_NAME} PROPERTIES PREFIX "")

if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    # Add executable that is a command line wrapper around the library
    add_executable(${PROJECT_NAME} main.cpp)

    target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
endif(${PROJECT_NAME}_BUILD_EXECUTABLE)
