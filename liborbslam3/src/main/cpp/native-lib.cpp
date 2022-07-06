#include <jni.h>
#include <string>
#include <System.h>
#include "log.h"
#include "opencv_util.hpp"

static ORB_SLAM3::System *pSystem = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_damon_orbslam3_NativeLib_nativeInitTrackingSystem(JNIEnv *env, jobject thiz,
                                                           jstring voc_file_path,
                                                           jstring settings_file_path) {
    const char *p_voc_file_path = env->GetStringUTFChars(voc_file_path, 0);
    if (!p_voc_file_path) {
        env->ReleaseStringUTFChars(voc_file_path, p_voc_file_path);
        return;
    }
    const char *p_settings_file_path = env->GetStringUTFChars(settings_file_path, 0);
    if (!p_settings_file_path) {
        env->ReleaseStringUTFChars(settings_file_path, p_settings_file_path);
        return;
    }
    if (!pSystem) {
        pSystem = new ORB_SLAM3::System(p_voc_file_path, p_settings_file_path,
                                        ORB_SLAM3::System::MONOCULAR, false);
    }
    env->ReleaseStringUTFChars(voc_file_path, p_voc_file_path);
    env->ReleaseStringUTFChars(settings_file_path, p_settings_file_path);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_damon_orbslam3_NativeLib_nativeReleaseTrackingSystem(JNIEnv *env, jobject thiz) {
    if (pSystem) delete pSystem;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_damon_orbslam3_NativeLib_nativeGetTrackingState(JNIEnv *env, jobject thiz) {
    if (pSystem) {
        return pSystem->GetTrackingState();
    }
    return ORB_SLAM3::Tracking::eTrackingState::SYSTEM_NOT_READY;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_damon_orbslam3_NativeLib_nativeTrackingMono(JNIEnv *env, jobject thiz, jbyteArray bytes,
                                                     jint width, jint height,
                                                     jdouble timestamp_sec) {
    if (pSystem) {
        jclass list_jcls = env->FindClass("java/util/ArrayList");
        jmethodID list_init = env->GetMethodID(list_jcls, "<init>", "()V");
        jobject list_obj = env->NewObject(list_jcls, list_init);
        jmethodID list_add = env->GetMethodID(list_jcls, "add", "(Ljava/lang/Object;)Z");

        cv::Mat input = byteArrayToMat(env, bytes, width, height);
        cv::Mat inputSmall;
        int scale = 2;
        cv::resize(input, inputSmall, cv::Size(input.cols / scale, input.rows / scale));
        LOGE("input:%d x %d", input.cols, input.rows);
        LOGE("inputSmall:%d x %d", inputSmall.cols, inputSmall.rows);
        pSystem->TrackMonocular(inputSmall, timestamp_sec);
        vector<KeyPoint> points = pSystem->GetTrackedKeyPointsUn();
        if (points.size() > 0) {
            jclass point_jcls = env->FindClass("android/graphics/PointF");
            jmethodID point_init = env->GetMethodID(point_jcls, "<init>", "()V");
            jmethodID point_set = env->GetMethodID(point_jcls, "set", "(FF)V");
            jobject tmpPoint = env->NewObject(point_jcls, point_init);
            for (int i = 0; i < points.size(); ++i) {
                env->CallVoidMethod(tmpPoint, point_set, points[i].pt.x * scale,
                                    points[i].pt.y * scale);
                env->CallBooleanMethod(list_obj, list_add, tmpPoint);
            }
        }
        return list_obj;
    }
    return NULL;
}