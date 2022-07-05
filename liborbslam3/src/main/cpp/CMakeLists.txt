cmake_minimum_required(VERSION 3.18.1)
project(native-lib)

set(OpenCV_DIR D:/opencv/opencv-4.6.0-android-sdk/OpenCV-android-sdk/sdk/native/jni)
add_subdirectory(orb-slam3)

add_library(
        ${PROJECT_NAME}
        SHARED
        native-lib.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        orbslam3
        log)