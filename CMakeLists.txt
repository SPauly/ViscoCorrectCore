# ViscoCorrectCore - Correction factors for centrifugal pumps
# Copyright (C) 2024  Simon Pauly
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# Contact via <https://github.com/SPauly/ViscoCorrectCore>
cmake_minimum_required(VERSION 3.14) # 3.14 is required by GoogleTest

project(ViscoCorrectCore)

set(CMAKE_CXX_STANDARD 20) # to use the c++20 async implementations
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration options for the build
option(VCC_BUILD_TESTS "Build tests for ViscoCorrecCore" OFF)
option(VCC_BUILD_EXAMPLES "Build examples for ViscoCorrectCore" OFF)

# Set the output directory for the build
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Build the library
add_library(ViscoCorrectCore
    src/correction_context.cpp
)

target_include_directories(ViscoCorrectCore PUBLIC 
${CMAKE_SOURCE_DIR}/include
${CMAKE_SOURCE_DIR}/
)

# fast-cpp-csv-parser needs to be linked against lpthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ViscoCorrectCore PRIVATE Threads::Threads)


# Build the tests
if(VCC_BUILD_TESTS)
    add_subdirectory(test)
endif()
