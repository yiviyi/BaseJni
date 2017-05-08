package com.eve.basejni;

import android.util.Log;

/**
 * Created by eve
 * yiviyi@github.com
 * on 2017/5/2.
 */
public class BaseUtils {
    static {
        System.loadLibrary("native-lib");
    }

    public void voidMethod() {
        Log.e("hehe", "hehe");
    }

    public void paramsMethod(String str) {
        Log.e("str", str);
    }

    public int getSum(int a, int b) {
        return a + b;
    }

    public native void CallJavaMethod();

    public native void CallJavaParamsMethod();

    public native int CallJavaSumMethod(int a, int b);

    public native String HelloEve();
    public native String LinkEve(String str1,String str2);

    public native int sumArray(int[] arr);
    public native int[][] initObjectArray(int size);
    public static native void CallJavaStaticMethod();
    public static native void CallJavaInstanceMethod();
    public static native void UseInstanceField(ClassField obj);
    public static native void UseStaticField();
}
