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

project(ViscoCorrectCore VERSION 0.0.1)

# Set the Version and SONAME 
set(vcc_VERSION ${PROJECT_VERSION})
set(vcc_SOVERSION 0)

# Set the installation directories
set(vcc_INSTALL_BINDIR "bin" CACHE STRING "Installation directory for executables") # Not used in Release mode
set(vcc_INSTALL_LIBDIR "lib" CACHE STRING "Installation directory for libraries")
set(vcc_INSTALL_INCLUDEDIR "include" CACHE STRING "Installation directory for headers")
set(vcc_INSTALL_CMAKEDIR "lib/cmake/" CACHE STRING "Installation directory for cmake config files")
set(vcc_INSTALL_SHAREDIR "share/ViscoCorrectCore" CACHE STRING "Installation directory for root certificates")

# Set the build options
option(vcc_BUILD_TESTS "Build tests for ViscoCorrectCore" ON)
option(vcc_BUILD_EXAMPLES "Build examples for ViscoCorrectCore" OFF)

# Set the installation options (default to ON if building as a standalone project)
set(vcc_INSTALL_default ON)
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  # Disable vcc_INSTALL by default if building as a submodule
  set(vcc_INSTALL_default OFF)
endif()
set(vcc_INSTALL ${vcc_INSTALL_default} CACHE BOOL
    "Generate installation target")

include(GoogleTest)
include(GNUInstallDirs)

# fast-cpp-csv-parser needs to be linked against lpthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Source files for ViscosityCorrectCore
set(vcc_SOURCES 
    src/correction_context.cpp
)

# Header files for ViscosityCorrectCore
set(vcc_HEADERS
    include/visco_correct_core/correction_context.hpp
)

# Build the library
add_library(ViscoCorrectCore
    ${vcc_SOURCES}
)

# Set library definitions
target_compile_features(ViscoCorrectCore PUBLIC cxx_std_20)

set_target_properties(ViscoCorrectCore PROPERTIES 
    SOVERSION ${vcc_SOVERSION} 
    VERSION ${vcc_VERSION}
)

# Additionaly handle pdb name for windows and MSVC
if(WIN32 AND MSVC)
  set_target_properties(ViscoCorrectCore PROPERTIES COMPILE_PDB_NAME "ViscoCorrectCore"
    COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  )
  if(vcc_INSTALL)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ViscoCorrectCore.pdb
      DESTINATION ${vcc_INSTALL_LIBDIR} OPTIONAL
    )
  endif()
endif()

target_include_directories(ViscoCorrectCore PUBLIC 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/> 
$<INSTALL_INTERFACE:${vcc_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ViscoCorrectCore PRIVATE 
    Threads::Threads
)

add_library(ViscoCorrectCore::ViscoCorrectCore ALIAS ViscoCorrectCore)

foreach(header ${vcc_HEADERS})
    string(REPLACE "include/" "" _path ${header})
    get_filename_component(_path ${_path} PATH)
    message(STATUS "Installing header ${header} to ${vcc_INSTALL_INCLUDEDIR}/${_path}")
    install(FILES ${header} 
        DESTINATION "${vcc_INSTALL_INCLUDEDIR}/${_path}"
    )        
endforeach()

# Install the library
if(vcc_INSTALL)
    install(TARGETS ViscoCorrectCore EXPORT ViscoCorrectCoreTargets
        LIBRARY DESTINATION ${vcc_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${vcc_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${vcc_INSTALL_BINDIR}
        INCLUDES DESTINATION ${vcc_INSTALL_INCLUDEDIR}
    )

    install(EXPORT ViscoCorrectCoreTargets
        FILE ViscoCorrectCoreTargets.cmake
        NAMESPACE ViscoCorrectCore::
        DESTINATION ${vcc_INSTALL_CMAKEDIR}
    )
endif()

# Build the tests
if(vcc_BUILD_TESTS)
    # Add GoogleTest
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # Required for Windows to prevent overriding compiler/linker flags
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE) # Disable building GoogleMock
    set(INSTALL_GTEST ON CACHE BOOL "" FORCE) # Disable installing GoogleTest

    add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest)

    enable_testing()

    option(vcc_BUILD_ALL_TESTS "Build all tests" ON)
    option(vcc_BUILD_CORE_TESTS "Build core tests" OFF)
    option(vcc_BUILD_TOOL_TESTS "Build tool tests" OFF)

    # Add test files
    set(vcc_TEST_CORCTX ${CMAKE_SOURCE_DIR}/test/correction_context_test_.cpp)

    if(vcc_BUILD_ALL_TESTS OR vcc_BUILD_CORE_TESTS)
        add_executable(ViscoCorrectCoreTests ${vcc_TEST_CORCTX})
        target_link_libraries(ViscoCorrectCoreTests GTest::gtest_main ViscoCorrectCore)
        target_include_directories(ViscoCorrectCoreTests PRIVATE ${CMAKE_SOURCE_DIR}/include)
        add_dependencies(ViscoCorrectCoreTests ViscoCorrectCore)
    endif()

    gtest_discover_tests(ViscoCorrectCoreTests) # Automatically discover and run all tests
endif()

# Create config files for the library
include(CMakePackageConfigHelpers)

configure_file(cmake/ViscoCorrectCoreConfig.cmake.in
    ViscoCorrectCoreConfig.cmake @ONLY)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ViscoCorrectCoreConfigVersion.cmake
  VERSION ${vcc_VERSION}
  COMPATIBILITY AnyNewerVersion)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ViscoCorrectCoreConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ViscoCorrectCoreConfigVersion.cmake
  DESTINATION ${vcc_INSTALL_CMAKEDIR}
)