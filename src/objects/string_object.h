/*
 * Author: Yo Ka
 */

#ifndef KAYO_STRING_OBJECT_H
#define KAYO_STRING_OBJECT_H

#include "object.h"

jstrref newString(const utf8_t *str);
jstrref newString(const unicode_t *str, jsize len);

utf8_t *strObjToUtf8(jstrref so);

jsize strObjGetLength(jstrref so);
jsize strObjGetUTFLength(jstrref so);

struct StrObjEquals {
    bool operator()(Object *x, Object *y) const;
};

struct StrObjHash {
    size_t operator()(Object *x) const;
};

#endif //KAYO_STRING_OBJECT_H
