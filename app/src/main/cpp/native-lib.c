#include <jni.h>
#include <stdio.h>
#include<stdlib.h>

JNIEXPORT jstring JNICALL
Java_com_eve_basejni_BaseUtils_HelloEve(JNIEnv *env, jobject instance) {
    jstring text = (*env)->NewStringUTF(env, "Hello,eve. Let's to learn Jni");
    return text;
}

JNIEXPORT jstring JNICALL
Java_com_eve_basejni_BaseUtils_LinkEve0(JNIEnv *env, jobject instance,
                                        jstring j_str1,
                                        jstring j_str2) {
/*    jsize len1 = (*env)->GetStringLength(env,j_str1);  // 获取unicode字符串的长度
    jsize len2 = (*env)->GetStringLength(env,j_str2);  // 获取unicode字符串的长度
    printf("str_len:%d %d\n",len1,len2);
    char buff[128] = "";
    char* pBuff = buff;
    // 将JVM中的字符串以utf-8编码拷入C缓冲区,该函数内部不会分配内存空间
    (*env)->GetStringUTFRegion(env,j_str1,0,len1+len2,pBuff);
    (*env)->GetStringUTFRegion(env,j_str2,len1,len1+len2,pBuff);
    return (*env)->NewStringUTF(env,buff);*/
    jsize len = (*env)->GetStringLength(env, j_str1);  // 获取unicode字符串的长度
    printf("str_len:%d\n", len);
    char buff[128] = "hello ";
    char *pBuff = buff + 6;
    // 将JVM中的字符串以utf-8编码拷入C缓冲区,该函数内部不会分配内存空间
    (*env)->GetStringUTFRegion(env, j_str1, 0, len, pBuff);
    return (*env)->NewStringUTF(env, buff);
}

JNIEXPORT jstring JNICALL
Java_com_eve_basejni_BaseUtils_LinkEve(JNIEnv *env, jobject instance,
                                       jstring str1,
                                       jstring str2) {
    const char *c_str1 = NULL;
    const char *c_str2 = NULL;
    char buff[128] = {0};
    jboolean isCopy;    // 返回JNI_TRUE表示原字符串的拷贝，返回JNI_FALSE表示返回原字符串的指针
    c_str1 = (*env)->GetStringUTFChars(env, str1,
                                       &isCopy);  //当内存空间不够分配的时候，会导致调用失败，失败后GetStringUTFChars会返回NULL，并抛出一个OutOfMemoryError异常
    c_str2 = (*env)->GetStringUTFChars(env, str2, &isCopy);
    printf("isCopy:%d\n", isCopy);
    if (c_str1 == NULL || c_str2 == NULL) {
        return NULL;
    }
    printf("STR: %s %s\n", c_str1, c_str2);
    sprintf(buff, "%s%s", c_str1, c_str2);
    (*env)->ReleaseStringUTFChars(env, str1, c_str1); //通过调用ReleaseStringUTFChars函数通知JVM这块内存已经不使用了
    (*env)->ReleaseStringUTFChars(env, str1, c_str2); //通过调用ReleaseStringUTFChars函数通知JVM这块内存已经不使用了
    return (*env)->NewStringUTF(env, buff);
}


/**
 * C调用java中的带参数的方法
 */
JNIEXPORT jint JNICALL
Java_com_eve_basejni_BaseUtils_CallJavaSumMethod(JNIEnv *env, jobject instance, jint num1,
                                                 jint num2) {
    jclass clazz = (*env)->FindClass(env, "com/eve/basejni/BaseUtils");
    jmethodID methodId = (*env)->GetMethodID(env, clazz, "getSum", "(II)I");
    jobject javaSimple = (*env)->AllocObject(env, clazz);
    int result = (*env)->CallIntMethod(env, javaSimple, methodId, num1, num2);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_eve_basejni_BaseUtils_sumArray(JNIEnv *env, jobject obj, jintArray j_array) {
    jint i, sum = 0;
    jint *c_array;
    jint arr_len;
    //1. 获取数组长度
    arr_len = (*env)->GetArrayLength(env, j_array);
    //2. 根据数组长度和数组元素的数据类型申请存放java数组元素的缓冲区
    c_array = (jint *) malloc(sizeof(jint) * arr_len);
    //3. 初始化缓冲区
    memset(c_array, 0, sizeof(jint) * arr_len);
    //4. 拷贝Java数组中的所有元素到缓冲区中
    (*env)->GetIntArrayRegion(env, j_array, 0, arr_len, c_array);
    for (i = 0; i < arr_len; i++) {
        sum += c_array[i]; //5. 累加数组元素的和
    }
    //6. 释放存储数组元素的缓冲区
    free(c_array);
    return sum;
}

JNIEXPORT jint JNICALL
Java_com_eve_basejni_BaseUtils_sumArray2(JNIEnv *env, jobject obj, jintArray j_array) {
    jint i, sum = 0;
    jint *c_array;
    jint arr_len;
    // 可能数组中的元素在内存中是不连续的，JVM可能会复制所有原始数据到缓冲区，然后返回这个缓冲区的指针
    c_array = (*env)->GetIntArrayElements(env, j_array, NULL);
    if (c_array == NULL) {
        return 0;   // JVM复制原始数据到缓冲区失败
    }
    arr_len = (*env)->GetArrayLength(env, j_array);
    printf("arr_len = %d\n", arr_len);
    for (i = 0; i < arr_len; i++) {
        sum += c_array[i];
    }
    (*env)->ReleaseIntArrayElements(env, j_array, c_array, 0); // 释放可能复制的缓冲区
    return sum;
}

//访问对象数组
JNIEXPORT jobjectArray JNICALL
Java_com_eve_basejni_BaseUtils_initObjectArray(JNIEnv *env, jobject obj, jint size) {
    jobjectArray result;
    jclass clsIntArray;
    jint i, j;
    // 1.获得一个int型二维数组类的引用
    clsIntArray = (*env)->FindClass(env, "[I");
    if (clsIntArray == NULL) {
        return NULL;
    }
    // 2.创建一个数组对象（里面每个元素用clsIntArray表示）
    result = (*env)->NewObjectArray(env, size, clsIntArray, NULL);
    if (result == NULL) {
        return NULL;
    }
    // 3.为数组元素赋值
    for (i = 0; i < size; i++) {
        jint buff[256];
        jintArray intArr = (*env)->NewIntArray(env, size);
        if (intArr == NULL) {
            return NULL;
        }
        for (int j = 0; j < size; j++) {
            buff[j] = i + j;
        }
        (*env)->SetIntArrayRegion(env, intArr, 0, size, buff);
        (*env)->SetObjectArrayElement(env, result, i, intArr);
        (*env)->DeleteLocalRef(env, intArr);
    }
    return result;
}

/**
 * 在这个方法中，使用C代码调用JavaSimple中的sayHello方法，使用的是C的反射技术
 */
JNIEXPORT void JNICALL
Java_com_eve_basejni_BaseUtils_CallJavaMethod(JNIEnv *env, jobject instance) {
    /*1，获取该类的字节码文件
     * jclass      (*FindClass)(JNIEnv*, const char*);
     * char*参数 传入的是全类名(需要把点（.)改成斜线（/）
     *
     * */
    jclass clazz = (*env)->FindClass(env, "com/eve/basejni/BaseUtils");


    /*2，获取该类的某一个方法对象（jmethodID）
     * jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char* str1, const char* str2);
     * jclass 要调用的java类的字节码
     * str1 要调用的类的方法的方法名
     * str2 对应的这个方法的签名（可以在该类的.class文件所在的根目录下执行命令行：javap -s 全类名（不包括.java））得到
     * */
    jmethodID methodId = (*env)->GetMethodID(env, clazz, "voidMethod", "()V");

//    3，根据获取该类的实例对象
    jobject javaSimple = (*env)->AllocObject(env, clazz);
    //4，调用对应的方法
    (*env)->CallVoidMethod(env, javaSimple, methodId);
}

//C调用JAVA里面 带参数的方法
JNIEXPORT void JNICALL
Java_com_eve_basejni_BaseUtils_CallJavaParamsMethod(JNIEnv *env, jobject instance) {
    jclass clazz = (*env)->FindClass(env, "com/eve/basejni/BaseUtils");
    jmethodID methodId = (*env)->GetMethodID(env, clazz, "paramsMethod", "(Ljava/lang/String;)V");
    jobject obj = (*env)->AllocObject(env, clazz);
    jstring text = (*env)->NewStringUTF(env, "this method has params");
    (*env)->CallVoidMethod(env, obj, methodId, text);
}

JNIEXPORT void JNICALL
Java_com_eve_basejni_BaseUtils_CallJavaInstanceMethod(JNIEnv *env, jclass cls) {
    jclass clazz = NULL;
    jobject jobj = NULL;
    jmethodID mid_construct = NULL;
    jmethodID mid_instance = NULL;
    jstring str_arg = NULL;

    // 1、从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    clazz = (*env)->FindClass(env, "com/eve/basejni/ClassMethod");
    if (clazz == NULL) {
        printf("找不到com/eve/basejni/ClassMethod");
        return;
    }

    // 2、获取类的默认构造方法ID
    mid_construct = (*env)->GetMethodID(env, clazz, "<init>", "()V");
    if (mid_construct == NULL) {
        printf("找不到构造方法");
        return;
    }

    // 3、查找实例方法的ID
    mid_instance = (*env)->GetMethodID(env, clazz, "CallInstanceMethod", "(ILjava/lang/String;)V");
    if (mid_instance == NULL) {
        printf("找不到CallInstanceMethod方法ID");
        return;
    }

    // 4、创建该类的实例
    jobj = (*env)->NewObject(env, clazz, mid_construct);
    if (jobj == NULL) {
        printf("创建不了CallInstanceMethod方法实例");
        return;
    }

    // 5、调用对象的实例方法
    str_arg = (*env)->NewStringUTF(env, "Hi form Instance Method");
    (*env)->CallVoidMethod(env, jobj, mid_instance, 200, str_arg);
    // 删除局部引用
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, jobj);
    (*env)->DeleteLocalRef(env, str_arg);
}

JNIEXPORT void JNICALL
Java_com_eve_basejni_BaseUtils_CallJavaStaticMethod(JNIEnv *env, jclass cls) {
    jclass clazz = NULL;
    jstring str_arg = NULL;
    jmethodID mid_static_method;

    // 1、从classpath路径下搜索ClassMethod这个类，并返回该类的Class对象
    clazz = (*env)->FindClass(env, "com/eve/basejni/ClassMethod");
    if (clazz == NULL) {
        printf("找不到com/eve/basejni/ClassMethod");
        return;
    }

    // 2、从clazz类中查找callStaticMethod方法
    mid_static_method = (*env)->GetStaticMethodID(env, clazz, "CallStaticMethod",
                                                  "(Ljava/lang/String;I)V");
    if (mid_static_method == NULL) {
        printf("找不到CallStaticMethod的静态方法");
        return;
    }

    // 3、调用clazz类的callStaticMethod静态方法
    str_arg = (*env)->NewStringUTF(env, "This is CallStaticMethod form C");
    (*env)->CallStaticVoidMethod(env, clazz, mid_static_method, str_arg, 100);

    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, str_arg);
}

JNIEXPORT void JNICALL Java_com_eve_basejni_BaseUtils_UseInstanceField(
        JNIEnv *env, jclass cls, jobject obj) {
    jclass clazz;
    jfieldID fid;
    jstring j_str;
    jstring j_newStr;
    const char *c_str = NULL;

    // 1.获取AccessField类的Class引用
    clazz = (*env)->GetObjectClass(env, obj);
    if (clazz == NULL) {
        return;
    }

    // 2. 获取AccessField类实例变量str的属性ID
    fid = (*env)->GetFieldID(env, clazz, "str", "Ljava/lang/String;");
    if (fid == NULL) {
        return;
    }

    // 3. 获取实例变量str的值
    j_str = (jstring) (*env)->GetObjectField(env, obj, fid);

    // 4. 将unicode编码的java字符串转换成C风格字符串
    c_str = (*env)->GetStringUTFChars(env, j_str, NULL);
    if (c_str == NULL) {
        return;
    }
    printf("In C--->ClassField.str = %s\n", c_str);
    (*env)->ReleaseStringUTFChars(env, j_str, c_str);

    // 5. 修改实例变量str的值
    j_newStr = (*env)->NewStringUTF(env, "This is C String");
    if (j_newStr == NULL) {
        return;
    }

    (*env)->SetObjectField(env, obj, fid, j_newStr);

    // 6.删除局部引用
    (*env)->DeleteLocalRef(env, clazz);
    (*env)->DeleteLocalRef(env, j_str);
    (*env)->DeleteLocalRef(env, j_newStr);
}

JNIEXPORT void JNICALL Java_com_eve_basejni_BaseUtils_UseStaticField
        (JNIEnv *env, jclass cls)
{
    jclass clazz;
    jfieldID fid;
    jint num;

    //1.获取ClassField类的Class引用
    clazz = (*env)->FindClass(env,"com/eve/basejni/ClassField");
    if (clazz == NULL) {    // 错误处理
        return;
    }

    //2.获取ClassField类静态变量num的属性ID
    fid = (*env)->GetStaticFieldID(env, clazz, "num", "I");
    if (fid == NULL) {
        return;
    }

    // 3.获取静态变量num的值
    num = (*env)->GetStaticIntField(env,clazz,fid);
    printf("In C--->ClassField.num = %d\n", num);

    // 4.修改静态变量num的值
    (*env)->SetStaticIntField(env, clazz, fid, 80);

    // 删除属部引用
    (*env)->DeleteLocalRef(env,clazz);
}