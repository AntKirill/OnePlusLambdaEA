find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(thirdParty)
add_subdirectory(thirdParty)

add_executable(chartBuilding chart_builder_main.cpp mainwindow.cpp)
target_link_libraries(chartBuilding qcustomplot resultsScanner Qt5::Widgets)

