//
// Created by Damon on 2022/7/4.
//

#ifndef ORB_SLAM3_ANDROID_LOG_H
#define ORB_SLAM3_ANDROID_LOG_H
#include <android/log.h>
#define  LOG_TAG    "ORB_SLAM3_JNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //ORB_SLAM3_ANDROID_LOG_H
