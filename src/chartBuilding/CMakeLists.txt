find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(thirdParty)
add_subdirectory(thirdParty)

add_library(ChartBuilder chartBuilder.cpp)
target_link_libraries(ChartBuilder qcustomplot Qt5::Widgets)
add_executable(chartBuilding chart_builder_main.cpp)
add_executable(xyChartBuilding xy_chart_building.cpp)
target_link_libraries(chartBuilding resultsScanner ChartBuilder)
target_link_libraries(xyChartBuilding ChartBuilder ${Boost_LIBRARIES})

