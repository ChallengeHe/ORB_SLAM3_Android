package com.damon.orbslam3

import android.annotation.SuppressLint
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.util.Log
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import java.nio.ByteBuffer

private const val TAG = "ORBSLAM3Tracking"
class ORBSLAM3Tracking(val mView: ViewOverLay) : ImageAnalysis.Analyzer {
    @SuppressLint("UnsafeOptInUsageError")
    override fun analyze(imgProxy: ImageProxy) {
        val image = imgProxy.image
        if (image == null) {
            imgProxy.close()
            return
        }
        try {
            val bytes = imageProxyToByteArray(imgProxy)
            var width = image.width
            var height = image.height
            Log.e(TAG, "image: $width x $height")
            val trackingState = NativeLib.nativeGetTrackingState()
            Log.e(TAG, "trackingState: $trackingState" )
            val timestampSec = 1.0 * System.currentTimeMillis() / 1000
            val result = NativeLib.nativeTrackingMono(bytes, width, height, timestampSec)
            result?.let { points ->
                mView.post {
                    mView.drawPoints(points, width, height)
                }
            }
        } catch (e: Exception) {
            Log.e(TAG, e.message.toString())
        } finally {
            imgProxy.close()
        }
    }

    private fun imageProxyToByteArray(image: ImageProxy): ByteArray {
        val yuvBytes = ByteArray(image.width * (image.height + image.height / 2))
        val yPlane = image.planes[0].buffer
        val uPlane = image.planes[1].buffer
        val vPlane = image.planes[2].buffer

        yPlane.get(yuvBytes, 0, image.width * image.height)

        val chromaRowStride = image.planes[1].rowStride
        val chromaRowPadding = chromaRowStride - image.width / 2

        var offset = image.width * image.height
        if (chromaRowPadding == 0) {

            uPlane.get(yuvBytes, offset, image.width * image.height / 4)
            offset += image.width * image.height / 4
            vPlane.get(yuvBytes, offset, image.width * image.height / 4)
        } else {
            for (i in 0 until image.height / 2) {
                uPlane.get(yuvBytes, offset, image.width / 2)
                offset += image.width / 2
                if (i < image.height / 2 - 2) {
                    uPlane.position(uPlane.position() + chromaRowPadding)
                }
            }
            for (i in 0 until image.height / 2) {
                vPlane.get(yuvBytes, offset, image.width / 2)
                offset += image.width / 2
                if (i < image.height / 2 - 1) {
                    vPlane.position(vPlane.position() + chromaRowPadding)
                }
            }
        }

        return yuvBytes
    }

    private fun imageProxyToBitmap(image: ImageProxy): Bitmap {
        val buffer: ByteBuffer = image.planes[0].buffer
        val bytes = ByteArray(buffer.remaining())
        buffer.get(bytes)
        return BitmapFactory.decodeByteArray(bytes, 0, bytes.size)
    }
}
