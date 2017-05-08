package com.eve.basejni;

import android.util.Log;

/**
 * Created by eve
 * yiviyi@github.com
 * on 2017/5/8.
 */

public class ClassMethod {
    private void CallInstanceMethod(int i, String str) {
        Log.e("ClassMethod::CallInstanceMethod", "str=" + str + "\ti=" + i);
    }

    private static void CallStaticMethod(String str, int i) {
        Log.e("ClassMethod::CallInstanceMethod", "str=" + str + "\ti=" + i);
    }
}
