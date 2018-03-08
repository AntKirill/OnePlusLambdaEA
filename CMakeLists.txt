cmake_minimum_required(VERSION 3.1.0)

project(Practice)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14 -O4 -Wall")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Boost 1.66.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_library(fast_ea STATIC fastEAlgs.cpp probability_t.cpp)

add_executable(run main.cpp)
add_executable(chartBuilder chart_builder_main.cpp qcustomplot.cpp mainwindow.cpp)

target_link_libraries(run fast_ea ${Boost_LIBRARIES})
target_link_libraries(chartBuilder Qt5::Widgets Qt5::PrintSupport)
