package com.damon.orbslam3

import android.graphics.PointF

object NativeLib {
    external fun nativeInitTrackingSystem(vocFilePath: String, settingsFilePath: String)
    external fun nativeTrackingMono(
        bytes: ByteArray,
        width: Int,
        height: Int,
        timestampSec: Double
    ): List<PointF>

    external fun nativeGetTrackingState(): Int
    external fun nativeReleaseTrackingSystem()

    init {
        System.loadLibrary("native-lib")
    }
}