/*
 * Author: Yo Ka
 */

#include "../../jni_inner.h"

// static native void copyFromShortArray(Object src, long srcPos, long dstAddr, long length);
static void copyFromShortArray(jref _this, jref src, jlong srcPos, jlong dstAddr, jlong length)
{
    jvm_abort("copyFromShortArray"); // todo
}

// static native void copyToShortArray(long srcAddr, Object dst, long dstPos, long length);
static void copyToShortArray(jref _this, jlong srcAddr, jref dst, jlong dstPos, jlong length)
{
    jvm_abort("copyToShortArray"); // todo
}

// static native void copyFromIntArray(Object src, long srcPos, long dstAddr, long length);
static void copyFromIntArray(jref _this, jref src, jlong srcPos, jlong dstAddr, jlong length)
{
    jvm_abort("copyFromIntArray"); // todo
}


// static native void copyToIntArray(long srcAddr, Object dst, long dstPos, long length);
static void copyToIntArray(jref _this, jlong srcAddr, jref dst, jlong dstPos, jlong length)
{
    jvm_abort("copyToIntArray"); // todo
}

// static native void copyFromLongArray(Object src, long srcPos, long dstAddr, long length);
static void copyFromLongArray(jref _this, jref src, jlong srcPos, jlong dstAddr, jlong length)
{
    jvm_abort("copyFromLongArray"); // todo
}

// static native void copyToLongArray(long srcAddr, Object dst, long dstPos, long length);
static void copyToLongArray(jref _this, jlong srcAddr, jref dst, jlong dstPos, jlong length)
{
    jvm_abort("copyToLongArray"); // todo
}

static JNINativeMethod methods[] = {
        JNINativeMethod_registerNatives,
        { "copyFromShortArray", "(Ljava/lang/Object;JJJ)V", (void *) copyFromShortArray },
        { "copyToShortArray", "(JLjava/lang/Object;JJ)V", (void *) copyToShortArray },
        { "copyFromIntArray", "(Ljava/lang/Object;JJJ)V", (void *) copyFromIntArray },
        { "copyToIntArray", "(JLjava/lang/Object;JJ)V", (void *) copyToIntArray },
        { "copyFromLongArray", "(Ljava/lang/Object;JJJ)V", (void *) copyFromLongArray },
        { "copyToLongArray", "(JLjava/lang/Object;JJ)V", (void *) copyToLongArray },
};

void java_nio_Bits_registerNatives()
{
    registerNatives("java/nio/Bits", methods, ARRAY_LENGTH(methods));
}
