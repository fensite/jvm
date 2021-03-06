/*
 * Author: Yo Ka
 */

#include "../../jni_inner.h"

// public native String intern();
static jstrref intern(jstrref _this)
{
    return _this->clazz->intern(_this);
}

static JNINativeMethod methods[] = {
        JNINativeMethod_registerNatives,
        { "intern", "()Ljava/lang/String;", (void *) intern },
};

void java_lang_String_registerNatives()
{
    registerNatives("java/lang/String", methods, ARRAY_LENGTH(methods));
}
