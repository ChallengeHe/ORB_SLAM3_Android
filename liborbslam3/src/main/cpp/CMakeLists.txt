cmake_minimum_required(VERSION 3.18.1)

project("liborbslam3")

add_library(
        liborbslam3
        SHARED
        liborbslam3.cpp)

target_link_libraries(
        liborbslam3
        log)