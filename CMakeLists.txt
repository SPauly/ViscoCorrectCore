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
option(VCC_BUILD_TESTS "Build tests for ViscoCorrecCore" ON)
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
    # Add GoogleTest
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # Required for Windows to prevent overriding compiler/linker flags
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE) # Disable building GoogleMock
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE) # Disable installing GoogleTest

    add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest)

    enable_testing()

    option(VCC_BUILD_ALL_TESTS "Build all tests" ON)
    option(VCC_BUILD_CORE_TESTS "Build core tests" OFF)
    option(VCC_BUILD_TOOL_TESTS "Build tool tests" OFF)

    # Add test files
    set(VCC_TEST_CORCTX ${CMAKE_SOURCE_DIR}/test/correction_context_test_.cpp)

    if(VCC_BUILD_ALL_TESTS OR VCC_BUILD_CORE_TESTS)
        add_executable(ViscoCorrectCoreTests ${VCC_TEST_CORCTX})
        target_link_libraries(ViscoCorrectCoreTests GTest::gtest_main ViscoCorrectCore)
        target_include_directories(ViscoCorrectCoreTests PRIVATE ${CMAKE_SOURCE_DIR}/include)
        add_dependencies(ViscoCorrectCoreTests ViscoCorrectCore)
    endif()

    include(GoogleTest)
    gtest_discover_tests(ViscoCorrectCoreTests) # Automatically discover and run all tests
endif()
