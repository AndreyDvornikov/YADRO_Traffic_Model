cmake_minimum_required(VERSION 3.10)
project(TrafficModel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(traffic_model
    src/main.cpp
    src/traffic.cpp
)

target_include_directories(traffic_model PRIVATE ${CMAKE_SOURCE_DIR})