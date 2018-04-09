find_package(Qt5PrintSupport REQUIRED)

add_library(qcustomplot STATIC qcustomplot.cpp)
target_link_libraries(qcustomplot Qt5::Widgets Qt5::PrintSupport)
