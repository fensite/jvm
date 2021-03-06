/*
 * Author: Yo Ka
 */

#include "string_object.h"
#include "class_loader.h"
#include "field.h"
#include "method.h"
#include "class.h"
#include "array_object.h"
#include "../interpreter/interpreter.h"

using namespace std;
using namespace utf8;

bool StrObjEquals::operator()(Object *x, Object *y) const
{
    assert(x != nullptr);
    assert(y != nullptr);
    assert(x->clazz == stringClass);
    assert(y->clazz == stringClass);

    // public boolean equals(Object anObject);
    Method *equals = stringClass->getDeclaredInstMethod("equals", "(Ljava/lang/Object;)Z");
    return ISLOT(execJavaFunc(equals, { x, y })) != 0;
}

size_t StrObjHash::operator()(Object *x) const
{
    assert(x != nullptr);
    assert(x->clazz == stringClass);

    // public int hashCode();
    Method *hashCode = stringClass->getDeclaredInstMethod("hashCode", "()I");
    return (size_t) ISLOT(execJavaFunc(hashCode, {x}));

}

static Object *newString_jdk_8_and_under(const utf8_t *str)
{
    assert(stringClass != nullptr && str != nullptr);

    initClass(stringClass);
    auto strobj =  newObject(stringClass);
    auto len = length(str);

    // set java/lang/String 的 value 变量赋值
    Array *value = newArray(loadBootClass(S(array_C) /* [C */ ), len);
    toUnicode(str, (unicode_t *) (value->data));
    strobj->setRefField(S(value), S(array_C), value);

    return strobj;
}

static Object *newString_jdk_9_and_upper(const utf8_t *str)
{
    assert(stringClass != nullptr && str != nullptr);

    initClass(stringClass);
    assert(is_jtrue(stringClass->lookupField("COMPACT_STRINGS", "Z")->staticValue.z));

    auto strobj =  newObject(stringClass);
    auto len = length(str);

    // set java/lang/String 的 value 变量赋值
    // private final byte[] value;
    Array *value = newArray(loadBootClass(S(array_B) /* [B */ ), len);
    memcpy(value->data, str, len);
    strobj->setRefField(S(value), S(array_B), value);

    // set java/lang/String 的 coder 变量赋值
    // private final byte coder;
    // 可取一下两值之一：
    // @Native static final byte LATIN1 = 0;
    // @Native static final byte UTF16  = 1;
    strobj->setByteField(S(coder), "B", 0);
    return strobj;
}

jstrref newString(const utf8_t *str)
{
    if (g_jdk_version_9_and_upper) {
        return newString_jdk_9_and_upper(str);
    } else {
        return newString_jdk_8_and_under(str);
    }
}

jstrref newString(const unicode_t *str, jsize len)
{
    // todo
    jvm_abort("not implement.");
}

utf8_t *strObjToUtf8(jstrref so)
{
    assert(so != nullptr);
    assert(stringClass != nullptr);
    assert(so->clazz == stringClass);

    if (g_jdk_version_9_and_upper) {
        // byte[] value;
        auto value = so->getRefField<Array>(S(value), S(array_B));
        static_assert(sizeof(utf8_t) == sizeof(jbyte), ""); // todo
        auto utf8 = new utf8_t[value->len + 1];
        utf8[value->len] = 0;
        memcpy(utf8, value->data, value->len * sizeof(jbyte));
        return utf8;
    } else {
        // char[] value;
        auto value = so->getRefField<Array>(S(value), S(array_C));
        return unicode::toUtf8((const unicode_t *) (value->data), value->len);
    }
}

jsize strObjGetLength(jstrref so)
{
    // todo
    jvm_abort("not implement.");
}

jsize strObjGetUTFLength(jstrref so)
{
    // todo
    jvm_abort("not implement.");
}
