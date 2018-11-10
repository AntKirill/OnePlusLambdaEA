add_library(measurer measurer.cpp)
target_link_libraries(measurer algorithmsReporter solversFabric resultsPrinter configParser ${Boost_LIBRARIES})
add_executable(runTimeMeasurement runTimeMeasurment.cpp)
target_link_libraries(runTimeMeasurement ${LIBS} ${Boost_LIBRARIES})
