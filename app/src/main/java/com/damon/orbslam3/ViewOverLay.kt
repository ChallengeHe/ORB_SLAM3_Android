package com.damon.orbslam3

import android.content.Context
import android.graphics.*
import android.text.StaticLayout
import android.text.TextPaint
import android.util.AttributeSet
import android.util.Log
import android.view.View
import androidx.core.content.ContextCompat
import com.google.android.material.snackbar.Snackbar

class ViewOverLay constructor(context: Context?, attributeSet: AttributeSet?) :
    View(context, attributeSet) {

    private var mText: String? = null
    private var mBmp: Bitmap? = null
    private var mRects: List<Rect>? = null
    private var mPoints: List<PointF>? = null

    //图片的与实际尺寸比
    private var mScaleWidth = 1.0f
    private var mScaleHeight = 1.0f

    private val textPaint = TextPaint().apply {
        style = Paint.Style.FILL
        color = ContextCompat.getColor(context!!, android.R.color.holo_purple)
        strokeWidth = 10f
        textSize = 60f
        isFakeBoldText = true
    }


    private val paint = Paint().apply {
        style = Paint.Style.STROKE
        color = ContextCompat.getColor(context!!, android.R.color.holo_purple)
        strokeWidth = 5f
    }


    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)

        try {
            mBmp?.let {
                canvas?.drawBitmap(it, x, y, Paint())
            }
            mRects?.let {
                it.forEach { p ->
                    Log.e("", "人脸框尺寸：${p.width()}x${p.height()}")
                    Log.e("", "人脸框 left：${p.left}")
                    Log.e("", "人脸框 top：${p.top}")
                    Log.e("", "人脸框 right：${p.right}")
                    Log.e("", "人脸框 bottom：${p.bottom}")
                    p.left = (p.left / mScaleWidth).toInt()
                    p.top = (p.top / mScaleHeight).toInt()
                    p.right = (p.right / mScaleWidth).toInt()
                    p.bottom = (p.bottom / mScaleHeight).toInt()
                    canvas?.drawRect(p, paint)
                }
            }
            mText?.let {
                if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
                    val builder = StaticLayout.Builder.obtain(it, 0, it.length, textPaint, width)
                    val myStaticLayout = builder.build()
                    canvas?.let { t ->
                        t.translate(x, y)
                        myStaticLayout.draw(t)
                    }
                } else {
                    canvas?.drawText(it, x, y, textPaint)
                }
            }
            mPoints?.let {
                it.forEach { point ->
                    canvas?.drawCircle(point.x / mScaleWidth, point.y / mScaleHeight, 10f, paint)
                }
            }

        } catch (e: Exception) {
            e.message?.let {
                Snackbar.make(this, it, Snackbar.LENGTH_SHORT).show()
            }
        }
    }

    fun drawInit() {
        mBmp = null
        mText = ""
        mRects = null
        mPoints = null
        invalidate()
    }

    fun drawBitmap(bmp: Bitmap?, isScale: Boolean = true) {
        bmp?.let {
            mBmp = if (isScale)
                Bitmap.createScaledBitmap(bmp, width, height, true)
            else
                bmp
        }
        invalidate()
    }

    fun drawText(str: String?) {
        mText = str
        invalidate()
    }

    fun drawRect(rects: List<Rect>?, w: Int = width, h: Int = height) {
        mRects = rects
        mScaleWidth = w.toFloat() / width
        mScaleHeight = h.toFloat() / height
        invalidate()
    }

    fun drawPoints(points: List<PointF>?, w: Int = width, h: Int = height) {
        mPoints = points
        mScaleWidth = w.toFloat() / width
        mScaleHeight = h.toFloat() / height
        invalidate()
    }

}