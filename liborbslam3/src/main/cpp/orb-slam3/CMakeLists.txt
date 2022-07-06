cmake_minimum_required(VERSION 3.18.1)
project(orbslam3)

add_subdirectory(Thirdparty/DBoW2)
add_subdirectory(Thirdparty/g2o)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions(-DCOMPILEDWITHC11)
    message(STATUS "Using flag -std=c++11.")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
include_directories(include include/CameraModels)
include_directories(.)
include_directories(Thirdparty)
include_directories(Thirdparty/eigen3)
include_directories(Thirdparty/eigen3/Eigen)
include_directories(Thirdparty/eigen3/Eigen/src)
include_directories(Thirdparty/boost/include)
include_directories(Thirdparty/openssl/include)
file(GLOB SRC_LIST "src/*.cc" "src/*.cpp" "src/CameraModels/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

add_library(crypto SHARED IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/openssl/lib/${CMAKE_ANDROID_ARCH_ABI}/libcrypto.so)
add_library(ssl SHARED IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/openssl/lib/${CMAKE_ANDROID_ARCH_ABI}/libssl.so)
add_library(boost_serialization STATIC IMPORTED)
set_target_properties(boost_serialization PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/boost/lib/${CMAKE_ANDROID_ARCH_ABI}/libboost_serialization.a)

target_link_libraries(
        ${PROJECT_NAME}
        ${OpenCV_LIBS}
        DBoW2
        g2o
        crypto
        ssl
        boost_serialization
        log
)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})