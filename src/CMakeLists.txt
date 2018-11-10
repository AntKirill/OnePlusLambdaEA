find_package(Boost 1.66.0 COMPONENTS system filesystem REQUIRED)
message(${Boost_INCLUDE_DIRS})
message(${Boost_LIBRARIES})

set(Boost_USE_MULTITHREADED ON)
include_directories(utils onePlusLambdaSolvers offsprings onePlusLambdaAlgorithms/algorithmsCreation  onePlusLambdaAlgorithms/algorithmsImpl)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_subdirectory(utils)
add_subdirectory(chartBuilding)
add_subdirectory(paramsSearch)
add_subdirectory(onePlusLambdaAlgorithms)
