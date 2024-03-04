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

project(ViscoCorrectCoreTests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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