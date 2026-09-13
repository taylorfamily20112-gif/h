cmake_minimum_required(VERSION 3.21)
project(MoreCheckpoints VERSION 1.0.0)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Environment variable GEODE_SDK is not defined. Install the Geode CLI/SDK and set GEODE_SDK to its path.")
else()
    message(STATUS "Found Geode SDK at $ENV{GEODE_SDK}")
    set(GEODE_SDK_PATH $ENV{GEODE_SDK})
endif()

add_subdirectory(${GEODE_SDK_PATH} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
