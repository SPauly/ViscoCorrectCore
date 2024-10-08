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

project(ViscoCorrectCore VERSION 1.0.0)

#####################################################
### Set project configuration
#####################################################

# Set the Version and SONAME 
set(vcc_VERSION ${PROJECT_VERSION})
set(vcc_SOVERSION 0)

# Set the installation directories
set(vcc_INSTALL_BINDIR "bin" CACHE STRING "Installation directory for executables") # Not used in Release mode
set(vcc_INSTALL_LIBDIR "lib" CACHE STRING "Installation directory for libraries")
set(vcc_INSTALL_INCLUDEDIR "include" CACHE STRING "Installation directory for headers")
set(vcc_INSTALL_CMAKEDIR "lib/cmake/ViscoCorrectCore" CACHE STRING "Installation directory for cmake config files")
set(vcc_INSTALL_SHAREDIR "share/ViscoCorrectCore" CACHE STRING "Installation directory for root certificates")

# Install ViscosityCorrectCore into the build directory by default
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation directory" FORCE)

# Set the build options
option(vcc_BUILD_TESTS "Build tests for ViscoCorrectCore" ON)
option(vcc_BUILD_EXAMPLES "Build examples for ViscoCorrectCore" OFF)

# Set the installation options (default to ON if building as a standalone project)
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  # Disable vcc_INSTALL by default if building as a submodule
  set(vcc_BUILD_TESTS OFF)
endif()

set(vcc_INSTALL ON CACHE BOOL
    "Generate installation target")

#####################################################
### Main Library ViscoCorrectCore
#####################################################

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Build the library
add_library(ViscoCorrectCore STATIC
    src/calculator.cpp
)

# Set library definitions
target_compile_features(ViscoCorrectCore PUBLIC cxx_std_17)

set_target_properties(ViscoCorrectCore PROPERTIES 
    SOVERSION ${vcc_SOVERSION} 
    VERSION ${vcc_VERSION}
)

 # Set the output directory for build artifacts.
 set_target_properties(ViscoCorrectCore
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
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
$<INSTALL_INTERFACE:${vcc_INSTALL_INCLUDEDIR}>
)

target_include_directories(ViscoCorrectCore PRIVATE 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
$<INSTALL_INTERFACE:${vcc_INSTALL_INCLUDEDIR}>
)

add_library(ViscoCorrectCore::ViscoCorrectCore ALIAS ViscoCorrectCore)

#####################################################
### Install ViscoCorrectCore
#####################################################

# Install the library
if(vcc_INSTALL)

include(GNUInstallDirs)

# Install the headers into the installation directory
foreach(header 
    include/spauly/vccore/impl/conversion_functions.h
    include/spauly/vccore/impl/math.h
    include/spauly/vccore/calculator.h
    include/spauly/vccore/data.h
)
    string(REPLACE "include/" "" _path ${header})
    get_filename_component(_path ${_path} PATH)
    install(FILES ${header} 
        DESTINATION "${vcc_INSTALL_INCLUDEDIR}/${_path}"
    )        
endforeach()

    install(TARGETS ViscoCorrectCore EXPORT ViscoCorrectCoreTargets
        LIBRARY DESTINATION ${vcc_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${vcc_INSTALL_BINDIR}
        RUNTIME DESTINATION ${vcc_INSTALL_BINDIR}
        INCLUDES DESTINATION ${vcc_INSTALL_INCLUDEDIR}
        CONFIGURATIONS Release 
    )

    install(EXPORT ViscoCorrectCoreTargets
        FILE ViscoCorrectCoreTargets.cmake
        NAMESPACE ViscoCorrectCore::
        DESTINATION ${vcc_INSTALL_CMAKEDIR}
    )
endif()

#####################################################
### Include GoogleTests
#####################################################

if(vcc_BUILD_TESTS)

include(GoogleTest)
    
# Add GoogleTest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # Required for Windows to prevent overriding compiler/linker flags
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE) # Disable building GoogleMock
if(vcc_INSTALL)
    set(INSTALL_GTEST ON CACHE BOOL "" FORCE) # Enable installing GoogleTest
else()
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE) # Disable installing GoogleTest
endif()

# Only add GTest if it was not already added
if(NOT TARGET GTest::gtest_)
    add_subdirectory(third_party/googletest)
endif()

enable_testing()

endif() # vcc_BUILD_TESTS

#####################################################
### Build Tests for ViscoCorrectCore
#####################################################

# Build the tests
if(vcc_BUILD_TESTS)

    set(vcc_TEST_TARGETS
        conversion_functions_test
        math_test
        calculator_test
    )

    foreach(target ${vcc_TEST_TARGETS})
    add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/testing/${target}.cpp)
    target_compile_features(${target} PUBLIC cxx_std_17)
    target_include_directories(${target} PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    )
    target_link_libraries(${target} GTest::gtest_main ViscoCorrectCore)
    add_dependencies(${target} ViscoCorrectCore)
    gtest_discover_tests(${target})
endforeach()

endif()

#####################################################
### Add Config file for Library
#####################################################

if(vcc_INSTALL)
# Create config files for the library
include(CMakePackageConfigHelpers)

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

configure_file(cmake/ViscoCorrectCoreConfig.cmake.in
    ${generated_dir}/ViscoCorrectCoreConfig.cmake @ONLY)

write_basic_package_version_file(${generated_dir}/ViscoCorrectCoreConfigVersion.cmake
  VERSION ${vcc_VERSION}
  COMPATIBILITY AnyNewerVersion)

install(FILES
    ${generated_dir}/ViscoCorrectCoreConfig.cmake
    ${generated_dir}/ViscoCorrectCoreConfigVersion.cmake
  DESTINATION ${vcc_INSTALL_CMAKEDIR}
)

# Generate pkg-config file
set(configured_pc "${generated_dir}/ViscoCorrectCore.pc")
configure_file("${PROJECT_SOURCE_DIR}/cmake/ViscoCorrectCore.pc.in"
    "${configured_pc}" @ONLY)
install(FILES "${configured_pc}"
    COMPONENT "${PROJECT_NAME}"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

endif() # vcc_INSTALL


#####################################################
### Install utility files
#####################################################
if(vcc_INSTALL)
    install(FILES
        LICENSE
        README.md
        DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
endif()