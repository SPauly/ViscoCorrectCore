cmake_minimum_required(VERSION 3.0)

project(ViscoCorrectCore)

set(CMAKE_CXX_STANDARD 14) # required by GoogleTest
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the build
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

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
