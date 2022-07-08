cmake_minimum_required(VERSION 3.18.1)
project(native-lib)

set(OpenCV_DIR D:/opencv/opencv-4.6.0-android-sdk/OpenCV-android-sdk/sdk/native/jni)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_subdirectory(orb-slam3)
include_directories(
        .
        orb-slam3
        orb-slam3/include
        orb-slam3/include/CameraModels
        orb-slam3/Thirdparty
        orb-slam3/Thirdparty/boost/include
        orb-slam3/Thirdparty/eigen3/
        orb-slam3/Thirdparty/eigen3/Eigen
        orb-slam3/Thirdparty/eigen3/Eigen/src
        orb-slam3/Thirdparty/openssl/include
        orb-slam3/Thirdparty/Sophus
)
add_library(
        ${PROJECT_NAME}
        SHARED
        native-lib.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        orbslam3
        android
        jnigraphics
        log)