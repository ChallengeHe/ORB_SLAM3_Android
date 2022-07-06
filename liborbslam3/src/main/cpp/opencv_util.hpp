//
// Created by Damon on 2022/7/4.
//
#include <android/bitmap.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace cv;

int mat2Bitmap(JNIEnv *env, jobject bitmap, Mat mat) {
    AndroidBitmapInfo info;
    void *pixels = 0;

    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0)
        return -1;

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 &&
        info.format != ANDROID_BITMAP_FORMAT_RGB_565)
        return -1; // Error: Unsupported Format

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0)
        return -1; // Error: Lock Data Failed

    if (!pixels) {
        AndroidBitmap_unlockPixels(env, bitmap);
        return -1; // Error: Get Date Failed
    }

    if (info.height == 0 || info.width == 0) {
        AndroidBitmap_unlockPixels(env, bitmap);
        return -1; // Error: Size
    }

    if (info.height != mat.rows || info.width != mat.cols) {
        resize(mat, mat, Size(info.width, info.height), 0, 0);
    }

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {

        Mat pointer(info.height, info.width, CV_8UC4, pixels);
        AndroidBitmap_unlockPixels(env, bitmap);
        cvtColor(mat, mat, CV_RGB2BGR);
        cvtColor(mat, pointer, CV_BGR2RGBA);
        return 0;

    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {

        Mat pointer(info.height, info.width, CV_8UC2, pixels);
        cvtColor(mat, mat, CV_RGB2BGR);
        cvtColor(mat, pointer, CV_BGR2BGR565);
        AndroidBitmap_unlockPixels(env, bitmap);
        return 0;
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    return 0;

}

Mat bitmap2Mat(JNIEnv *env, jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels = 0;

    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0)
        return Mat();

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 &&
        info.format != ANDROID_BITMAP_FORMAT_RGB_565)
        return Mat(); // Error: Unsupported Format

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0)
        return Mat(); // Error: Lock Data Failed

    if (!pixels) {
        AndroidBitmap_unlockPixels(env, bitmap);
        return Mat(); // Error: Get Date Failed
    }


    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {

        Mat dst(info.height, info.width, CV_8UC4, pixels);
        cvtColor(dst, dst, CV_RGBA2RGB);
        AndroidBitmap_unlockPixels(env, bitmap);
        return dst;

    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {

        Mat dst(info.height, info.width, CV_8UC2, pixels);
        cvtColor(dst, dst, COLOR_BGR5652BGR);
        AndroidBitmap_unlockPixels(env, bitmap);
        return dst;
    }

    // Error: Never Go Here
    AndroidBitmap_unlockPixels(env, bitmap);
    return Mat();

}

cv::Mat byteArrayToMat(JNIEnv *env, jbyteArray bytes, jint width, jint height) {
    try {
        cv::Mat mBgr;
        //读取Yuv的图片数据
        jbyte *_yuv = env->GetByteArrayElements(bytes, 0);
        //加载为Mat
        cv::Mat mYuv(height + height / 2, width, CV_8UC1, (uchar *) _yuv);

        //将Yuv420转为BGR的Mat
        cvtColor(mYuv, mBgr, cv::COLOR_YUV2BGRA_I420);

        env->ReleaseByteArrayElements(bytes, _yuv, 0);
        mYuv.release();
        return mBgr;
    } catch (cv::Exception e) {
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
    } catch (...) {
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
    }
}
