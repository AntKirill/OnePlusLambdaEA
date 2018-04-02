add_library(probability STATIC probability_t.cpp)
add_library(configParser STATIC configParser.cpp)
add_executable(dumpParams dump_params.cpp)
target_link_libraries(dumpParams configParser)
