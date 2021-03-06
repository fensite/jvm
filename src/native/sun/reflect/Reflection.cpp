/*
 * Author: Yo Ka
 */

#include "../../jni_inner.h"
#include "../../../objects/object.h"
#include "../../../jvmstd.h"
#include "../../../runtime/frame.h"
#include "../../../runtime/thread_info.h"
#include "../../../objects/class.h"


// public static native Class<?> getCallerClass(int level)
static jclsref getCallerClass0(jint level)
{
    jvm_abort("getCallerClass0");
}

// public static native Class<?> getCallerClass()
static jref getCallerClass()
{
    // top0, current frame is executing getCallerClass()
    // top1, who called getCallerClass, the one who wants to know his caller.
    // top2, the caller of top1, the result.
    Frame *frame = (Frame *) getCurrentThread()->getTopFrame();
    Frame *top1 = frame->prev;
    assert(top1 != nullptr);

    Frame *top2 = top1->prev;
    assert(top2 != nullptr);

    jref o = top2->method->clazz;
    return o;
}

// public static native int getClassAccessFlags(Class<?> type)
static jint getClassAccessFlags(jclsref type)
{
    return type->clazz->modifiers; // todo
}

static JNINativeMethod methods[] = {
        JNINativeMethod_registerNatives,
        { "getCallerClass", "()Ljava/lang/Class;", (void *) getCallerClass },
        { "getCallerClass", "(I)Ljava/lang/Class;", (void *) getCallerClass0 },
        { "getClassAccessFlags", "(Ljava/lang/Class;)I", (void *) getClassAccessFlags },
};

void sun_reflect_Reflection_registerNatives()
{
    registerNatives("sun/reflect/Reflection", methods, ARRAY_LENGTH(methods));
}