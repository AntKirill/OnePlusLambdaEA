find_package(Boost 1.66.0 COMPONENTS system REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
include_directories(utils onePlusLambdaSolvers ${Boost_INCLUDE_DIRS})

add_subdirectory(utils)
add_subdirectory(chartBuilding)
add_subdirectory(onePlusLambdaSolvers)
add_subdirectory(paramsSearch)
