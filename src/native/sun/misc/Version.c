#include "../../registry.h"
#include "../../../jvm.h"

/*
 * Author: Jia Yang
 */

// public static native String getJdkSpecialVersion()
static void getJdkSpecialVersion(struct frame *frame)
{
    jvm_abort(""); // todo
}

// public static native String getJvmSpecialVersion()
static void getJvmSpecialVersion(struct frame *frame)
{
    jvm_abort(""); // todo
}

static void getJdkVersionInfo(struct frame *frame)
{
    jvm_abort(""); // todo
}

static void getJvmVersionInfo(struct frame *frame)
{
    jvm_abort(""); // todo
}

void sun_misc_Version_registerNatives()
{
    register_native_method("sun/misc/Version~registerNatives~()V", registerNatives);
    register_native_method("sun/misc/Version~getJdkSpecialVersion~()Ljava/lang/String;", getJdkSpecialVersion);
    register_native_method("sun/misc/Version~getJvmSpecialVersion~()Ljava/lang/String;", getJvmSpecialVersion);
    register_native_method("sun/misc/Version~getJdkVersionInfo~()V", getJdkVersionInfo);
    register_native_method("sun/misc/Version~getJvmVersionInfo~()Z", getJvmVersionInfo);
}