#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_eve_basejni_BaseUtils_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++ by eve";
    return env->NewStringUTF(hello.c_str());
}