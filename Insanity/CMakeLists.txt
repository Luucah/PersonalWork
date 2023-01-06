cmake_minimum_required(VERSION 3.10)

# Set our project name and version number
project(Insanity VERSION 0.1)

IF (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, defaulting to Debug. \
Add `-DCMAKE_BUILD_TYPE=Release` at cmake time to generate a release build")
    set(CMAKE_BUILD_TYPE "Debug")
ELSE()
    message(STATUS "Building ${CMAKE_BUILD_TYPE}")
ENDIF()

# Specify our C++ standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# We would like to output to the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add some compiler options
IF(NOT WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -ggdb3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra")
ENDIF()

# Configure the header file for Insanity
configure_file(gen/insanity_config.h.in insanity_config.h)

# Add our subdirectory with all the cpp files
add_subdirectory(src)
