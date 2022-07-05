package com.damon.orbslam3

class NativeLib {

    /**
     * A native method that is implemented by the 'liborbslam3' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'liborbslam3' library on application startup.
        init {
            System.loadLibrary("liborbslam3")
        }
    }
}