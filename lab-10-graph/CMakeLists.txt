cmake_minimum_required(VERSION 3.5)

add_executable(lab-10-graph Graph.h RoadNetwork.cpp)

target_link_libraries(lab-10-graph utils_static)