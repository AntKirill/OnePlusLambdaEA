cmake_minimum_required(VERSION 3.1.0)
project(OnePlusLambdaEA LANGUAGES CXX)
set(PROJECT_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(TEST_BINARY_DIR ${CMAKE_BINARY_DIR}/test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

message("${CMAKE_CXX_COMPILER_VERSION}")
option(ENABLE_INTERNAL_INFO "Logging internal info of algorithms" OFF)

if (ENABLE_INTERNAL_INFO)
    message("Internal info enabled")
    add_definitions(-DENABLE_INTERNAL_INFO=1)
endif()

add_subdirectory(src)
