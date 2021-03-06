#include "../../../jni_inner.h"
#include "../../../../jvmstd.h"
#include "../../../../runtime/frame.h"
#include "../../../../objects/class.h"
#include "../../../../objects/field.h"
#include "../../../../objects/method.h"
#include "../../../../objects/invoke.h"
#include "../../../../objects/array_object.h"

/*
 * Author: Yo Ka
 */

using namespace utf8;
using namespace method_type;
using namespace member_name;

/*
 * Fetch MH-related JVM parameter.
 * which=0 retrieves MethodHandlePushLimit
 * which=1 retrieves stack slot push size (in address units)
 * 
 * static native int getConstant(int which);
 */
static jint getConstant(jint which)
{
    // todo 啥意思
    // if (which == 4)
    //     return 1;
    // else
        return 0;
}

/* Method flags */

#define MB_LAMBDA_HIDDEN        1
#define MB_LAMBDA_COMPILED      2
#define MB_CALLER_SENSITIVE     4
#define MB_DEFAULT_CONFLICT     8


#define IS_METHOD        0x010000
#define IS_CONSTRUCTOR   0x020000
#define IS_FIELD         0x040000
#define IS_TYPE          0x080000
#define CALLER_SENSITIVE 0x100000

#define SEARCH_SUPERCLASSES 0x100000
#define SEARCH_INTERFACES   0x200000

#define ALL_KINDS (IS_METHOD | IS_CONSTRUCTOR | IS_FIELD | IS_TYPE)
                
#define REFERENCE_KIND_SHIFT 24
#define REFERENCE_KIND_MASK  (0xf000000 >> REFERENCE_KIND_SHIFT)

static int __method_flags(Method *m) 
{
    assert(m != nullptr);

    int flags = m->modifiers;  //m->access_flags;

    if(m->modifiers & MB_CALLER_SENSITIVE)
        flags |= CALLER_SENSITIVE;

    return flags;
}

// static native void init(MemberName self, Object ref);
static void init(jref self, jref ref)
{
    /*
	 * in fact, `ref` will be one of these three:
	 * 1. java/lang/reflect/Field.
	 * 2. java/lang/reflect/Constructor.
	 * 3. java/lang/reflect/Method.
	 */

    // private Class<?> clazz;
    auto clazz = ref->getRefField<Class>(S(clazz), S(sig_java_lang_Class));

    // private int slot;
    auto slot = ref->getIntField("slot", "I");

    // private String name;
    auto name = ref->getRefField("name", S(sig_java_lang_String));

    // private transient String signature;
    auto signature = ref->getRefField("signature", S(sig_java_lang_String));

    //private Class<?> returnType;
    auto rtype = ref->getRefField<Class>("returnType", S(sig_java_lang_Class));
    //private Class<?>[] parameterTypes;
    auto ptypes = ref->getRefField<Array>("parameterTypes", S(array_java_lang_Class));

    // private Object   type;        // may be null if not yet materialized
    auto type = self->getRefField("type", S(sig_java_lang_Object));
    //private Object   resolution;  // if null, this guy is resolved
    auto resolution = self->getRefField("resolution", S(sig_java_lang_Object));
    // printvm("type: %p, resolution: %p\n", type, resolution); ///////////////////////////////////////////

    // private int modifiers;
    auto modifiers = ref->getIntField("modifiers", "I");

    if (equals(ref->clazz->className, "java/lang/reflect/Field")) {
        jvm_abort("not support!");
    } else if (equals(ref->clazz->className, "java/lang/reflect/Constructor")) {
        jvm_abort("not support!");
    } else if (equals(ref->clazz->className, "java/lang/reflect/Method")) {
        
#if 0
    if(target->class == method_reflect_class) {
        int slot = INST_DATA(target, int, mthd_slot_offset);
        Class *decl_class = INST_DATA(target, Class*, mthd_class_offset);

        ClassBlock *cb = CLASS_CB(decl_class);
        MethodBlock *mb = &(cb->methods[slot]);
        int flags = mbFlags(mb) | IS_METHOD;
        int ref_kind;
        
        if(mb->access_flags & ACC_STATIC)
            ref_kind = REF_invokeStatic;
        else if(IS_INTERFACE(cb))
            ref_kind = REF_invokeInterface;
        else
            ref_kind = REF_invokeVirtual;

        flags |= ref_kind << REFERENCE_KIND_SHIFT;

        INST_DATA(mname, Class*, mem_name_clazz_offset) = decl_class;
        INST_DATA(mname, int, mem_name_flags_offset) = flags;
        INST_DATA(mname, MethodBlock*, mem_name_vmtarget_offset) = mb;

   }
#endif
        auto descriptor = toMethodDescriptor(methodType(rtype, ptypes))->toUtf8();
        Method *m = clazz->lookupMethod(name->toUtf8(), descriptor);
        auto type = self->getRefField("type", S(sig_java_lang_Object));
        // printf("init: self(%p), slot: %d, type(%p), %s, %s\n", self, slot, type, name->toUtf8(), descriptor); /////////////////////////////////////////

        int flags = __method_flags(m) | IS_METHOD;

        int ref_kind;
        if (m->isStatic()) {
            ref_kind = JVM_REF_invokeStatic;
        } else if (clazz->isInterface()) {
            ref_kind = JVM_REF_invokeInterface;
        } else {
            ref_kind = JVM_REF_invokeVirtual;
        }

        flags |= ref_kind << REFERENCE_KIND_SHIFT;

        self->setRefField("clazz", "Ljava/lang/Class;", clazz);
        self->setIntField("flags", "I", flags);
        /*
        classObj := ref.GetFieldValue("clazz", "Ljava/lang/Class;").Ref
                class := classObj.GetGoClass()
                slot := ref.GetFieldValue("slot", "I").IntValue()
                method := class.Methods[slot]

                mn.SetFieldValue("clazz", "Ljava/lang/Class;", heap.NewRefSlot(classObj))
                mn.SetFieldValue("flags", "I", heap.NewIntSlot(getMNFlags(method)))
        */
    } else {
        jvm_abort("never get here.");
    }
}
/*
func getMNFlags(method *heap.Method) int32 {
	flags := int32(method.AccessFlags)
	if method.IsStatic() {
		flags |= MN_IS_METHOD | (references.RefInvokeStatic << MN_REFERENCE_KIND_SHIFT)
	} else if method.IsConstructor() {
		flags |= MN_IS_CONSTRUCTOR | (references.RefInvokeSpecial << MN_REFERENCE_KIND_SHIFT)
	} else {
		flags |= MN_IS_METHOD | (references.RefInvokeSpecial << MN_REFERENCE_KIND_SHIFT)
	}
	return flags
}
*/

// static native void expand(MemberName self);
static void expand(jref self)
{
    jvm_abort("expand");
}

/*
 * todo 说明这函数是干嘛的。。。。。。。。。
 * // There are 4 entities in play here:
//   * LC: lookupClass
//   * REFC: symbolic reference class (MN.clazz before resolution);
//   * DEFC: resolved method holder (MN.clazz after resolution);
//   * PTYPES: parameter types (MN.type)
//
// What we care about when resolving a MemberName is consistency between DEFC and PTYPES.
// We do type alias (TA) checks on DEFC to ensure that. DEFC is not known until the JVM
// finishes the resolution, so do TA checks right after MHN.resolve() is over.
//
// All parameters passed by a caller are checked against MH type (PTYPES) on every invocation,
// so it is safe to call a MH from any context.
//
// REFC view on PTYPES doesn't matter, since it is used only as a starting point for resolution and doesn't
// participate in method selection.
 *
 * static native MemberName resolve(MemberName self, Class<?> caller) throws LinkageError, ClassNotFoundException;
 */
 #if 0
 func resolve(frame *rtda.Frame) {
	mnSlot := frame.GetLocalVar(0)
	mnObj := mnSlot.Ref
	// caller := frame.GetRefVar(1)
	// panic("TODO: resolve!")
	frame.PushRef(mnObj)

	clsObj := mnObj.GetFieldValue("clazz", "Ljava/lang/Class;").Ref
	nameObj := mnObj.GetFieldValue("name", "Ljava/lang/String;").Ref
	flags := mnObj.GetFieldValue("flags", "I").IntValue()
	getSig := mnObj.Class.GetInstanceMethod("getSignature", "()Ljava/lang/String;")

	cls := clsObj.GetGoClass()
	nameStr := nameObj.JSToGoStr()

	frame.Thread.InvokeMethodWithShim(getSig, []heap.Slot{mnSlot})
	frame.Thread.CurrentFrame().AppendOnPopAction(func(shim *rtda.Frame) {
		sigObj := shim.TopRef(0)
		sigStr := sigObj.JSToGoStr()
		if sigStr[0] == '(' {
			if m := getMethod(cls, nameStr, sigStr); m != nil {
				flags |= int32(m.AccessFlags)
				mnObj.SetFieldValue("flags", "I", heap.NewIntSlot(flags))
			}
		} else {
			panic("TODO")
		}
	})
}
// TODO
func getMethod(cls *heap.Class, name, descriptor string) *heap.Method {
	if m := cls.GetStaticMethod(name, descriptor); m != nil {
		return m
	}
	if m := cls.GetInstanceMethod(name, descriptor); m != nil {
		return m
	}
	return nil
}
 #endif
static jref resolve(jref self, jclsref caller)
{
//    jvm_abort("resolve");

    // jref mn = frame->getLocalAsRef(0);
    // jref caller = frame->getLocalAsRef(1);

    // todo
    // private Class<?> clazz;       // class in which the method is defined
    // private String   name;        // may be null if not yet materialized
    // private Object   type;        // may be null if not yet materialized
    // private int      flags;       // modifier bits; see reflect.Modifier
    // private Object   resolution;  // if null, this guy is resolved
    auto clazz = self->getRefField<Class>("clazz", "Ljava/lang/Class;");
    auto name = self->getRefField("name", "Ljava/lang/String;");
    // type maybe a String or an Object[] or a MethodType
    // Object[]: (Class<?>) Object[0] is return type
    //           (Class<?>[]) Object[1] is parameter types
    auto type = self->getRefField("type", "Ljava/lang/Object;");
    jint flags = self->getIntField("flags", "I");
    auto resolution = self->getRefField("resolution", "Ljava/lang/Object;");

    Method *m = self->clazz->lookupInstMethod("getSignature", "()Ljava/lang/String;");
    jref sig = RSLOT(execJavaFunc(m, {self}));

    auto refKind = getRefKind(self);
    switch (flags & ALL_KINDS) {
        case IS_METHOD: {
            jstrref descriptor = nullptr;

            // TODO "java/lang/invoke/MethodHandle" 及其子类暂时特殊处理，因为取到的type一直是错的，我也不知道为什么？？？？
            if (equals(clazz->className, "java/lang/invoke/MethodHandle") &&
                    (equals(name->toUtf8(), "invoke")
                    || equals(name->toUtf8(), "invokeBasic")
                    || equals(name->toUtf8(), "invokeExact")
                    || equals(name->toUtf8(), "invokeWithArguments")
                    || equals(name->toUtf8(), "linkToSpecial")
                    || equals(name->toUtf8(), "linkToStatic")
                    || equals(name->toUtf8(), "linkToVirtual")
                    || equals(name->toUtf8(), "linkToInterface"))) {
                descriptor = newString("([Ljava/lang/Object;)Ljava/lang/Object;");
            } else if (equals(clazz->className, "java/lang/invoke/BoundMethodHandle$Species_L") && equals(name->toUtf8(), "make")) {
                descriptor = newString("(Ljava/lang/invoke/MethodType;Ljava/lang/invoke/LambdaForm;Ljava/lang/Object;)Ljava/lang/invoke/BoundMethodHandle;");
            } else if (equals(type->clazz->className, S(java_lang_String))) {
                descriptor = type;
            } else if (equals(type->clazz->className, "java/lang/invoke/MethodType")) {
                descriptor = toMethodDescriptor(type);
            } else if (type->isArrayObject()) {
                auto arr = (jarrref) (type);

                auto rtype = arr->get<jclsref>(0);
                auto ptypes = arr->get<jarrref>(1);
                descriptor = toMethodDescriptor(methodType(rtype, ptypes));
            } else {
                jvm_abort("never go here.");
            }
            assert(descriptor != nullptr);

            // printf("resolve: self(%p), %s, %s, %s\n", self, name->toUtf8(), descriptor->toUtf8(), sig->toUtf8()); /////////////////////////////////

            m = clazz->lookupMethod(name->toUtf8(), descriptor->toUtf8());
            flags |= __method_flags(m);
            self->setIntField("flags", "I", flags); 
            // if (refKind == JVM_REF_invokeStatic) {
            //     m = clazz->getDeclaredStaticMethod(name->toUtf8(), descriptor->toUtf8());
            //     flags |= __method_flags(m);
            //     self->setIntField("flags", "I", flags); 
            // } else {
            //     jvm_abort("not support!");
            // }

            return self;
        }
        case IS_CONSTRUCTOR: {
            jvm_abort("IS_CONSTRUCTOR.");
            break;
        }
        case IS_FIELD: {
            #if 0
                        FieldBlock *fb;

            fb = lookupField(clazz, name_sym, type_sym);
            if(fb == NULL)
                goto throw_excep;

            flags |= fb->access_flags;
            INST_DATA(mname, int, mem_name_flags_offset) = flags;
            INST_DATA(mname, FieldBlock*, mem_name_vmtarget_offset) = fb;
            break;
            #endif
            Field *f = clazz->lookupField(name->toUtf8(), sig->toUtf8());
            flags |= f->modifiers;
            self->setIntField("flags", "I", flags); 
            return self;
        }  
        default:
            jvm_abort("never go here.");
    }

    jvm_abort("not support!");
}

// static native int getMembers(Class<?> defc, String matchName, String matchSig,
//                              int matchFlags, Class<?> caller, int skip, MemberName[] results);
static jint getMembers(jclsref defc, jstrref match_name, jstrref match_sig, 
                        jint match_flags, jclsref caller, jint skip, jarrref results)
{
    int search_super = (match_flags & SEARCH_SUPERCLASSES) != 0;
    int search_intf = (match_flags & SEARCH_INTERFACES) != 0;
    int local = !(search_super || search_intf);
    char *name_sym = NULL, *sig_sym = NULL;

    if (match_name != nullptr) {
        auto x = match_name->toUtf8();
        jvm_abort("unimplemented");
    }

    if (match_sig != nullptr) {
        auto x = match_sig->toUtf8();
        jvm_abort("unimplemented");
    }

    if(match_flags & IS_FIELD)
        jvm_abort("unimplemented");

    if(!local)
        jvm_abort("unimplemented");

    if(match_flags & (IS_METHOD | IS_CONSTRUCTOR)) {
        int count = 0;

        for (Method *m : defc->methods) {
            if(m->name == SYMBOL(class_init))
                continue;
            if(m->name == SYMBOL(object_init))
                continue;
            if(skip-- > 0)
                continue;

            if(count < results->len) {
                Object *member_name = results->get<jref>(count);
                count++;
                int flags = __method_flags(m) | IS_METHOD;

                flags |= (m->isStatic() ? JVM_REF_invokeStatic : JVM_REF_invokeVirtual) << REFERENCE_KIND_SHIFT;

                member_name->setIntField("flags", "I", flags);
                member_name->setRefField("clazz", "Ljava/lang/Class;", m->clazz);
                member_name->setRefField("name", "Ljava/lang/String;", stringClass->intern(m->name));
                member_name->setRefField("type", "Ljava/lang/Object;", newString(m->type));
                // INST_DATA(mname, int, mem_name_flags_offset) = flags;
                // INST_DATA(mname, Class*, mem_name_clazz_offset) = mb->class;
                // INST_DATA(mname, Object*, mem_name_name_offset) =
                //                 findInternedString(createString(mb->name));
                // INST_DATA(mname, Object*, mem_name_type_offset) =
                //                 createString(mb->type);
                // INST_DATA(mname, MethodBlock*, mem_name_vmtarget_offset) = mb;
            }
        }

        return count;
    }


    jvm_abort("unimplemented");
}

// static native long objectFieldOffset(MemberName self);  // e.g., returns vmindex
static jlong objectFieldOffset(jref self)
{
    // private Class<?> clazz;       // class in which the method is defined
    // private String   name;        // may be null if not yet materialized
    // private Object   type;        // may be null if not yet materialized
    auto clazz = self->getRefField<Class>("clazz", "Ljava/lang/Class;");
    auto name = self->getRefField("name", "Ljava/lang/String;");
    // type maybe a String or an Object[] or a MethodType
    // Object[]: (Class<?>) Object[0] is return type
    //           (Class<?>[]) Object[1] is parameter types
    auto type = self->getRefField("type", "Ljava/lang/Object;");

    Method *m = self->clazz->lookupInstMethod("getSignature", "()Ljava/lang/String;");
    jref sig = RSLOT(execJavaFunc(m, {self}));

    Field *f = clazz->lookupField(name->toUtf8(), sig->toUtf8());
    return f->id;
}

// static native long staticFieldOffset(MemberName self);  // e.g., returns vmindex
static jlong staticFieldOffset(jref self)
{
    jvm_abort("staticFieldOffset");
}

// static native Object staticFieldBase(MemberName self);  // e.g., returns clazz
static jref staticFieldBase(jref self)
{
    jvm_abort("staticFieldBase");
}

// static native Object getMemberVMInfo(MemberName self);  // returns {vmindex,vmtarget}
static jref getMemberVMInfo(jref self)
{
    jvm_abort("getMemberVMInfo");
}

// static native void setCallSiteTargetNormal(CallSite site, MethodHandle target);
static void setCallSiteTargetNormal(jref site, jref target)
{
    jvm_abort("setCallSiteTargetNormal");
}

// static native void setCallSiteTargetVolatile(CallSite site, MethodHandle target);
static void setCallSiteTargetVolatile(jref site, jref target)
{
    jvm_abort("setCallSiteTargetVolatile");
}

static JNINativeMethod methods[] = {
    JNINativeMethod_registerNatives,

    // MemberName support

    {"init", "(Ljava/lang/invoke/MemberName;Ljava/lang/Object;)V", (void *)init},
    {"expand", "(Ljava/lang/invoke/MemberName;)V", (void *)expand},
    {"resolve", "(Ljava/lang/invoke/MemberName;" CLS ")Ljava/lang/invoke/MemberName;", (void *)resolve},
    {"getMembers", _CLS STR STR "I" CLS "I[Ljava/lang/invoke/MemberName;)I", (void *)getMembers},

    // Field layout queries parallel to sun.misc.Unsafe:

    {"objectFieldOffset", "(Ljava/lang/invoke/MemberName;)J", (void *)objectFieldOffset},
    {"staticFieldOffset", "(Ljava/lang/invoke/MemberName;)J", (void *)staticFieldOffset},
    {"staticFieldBase", "(Ljava/lang/invoke/MemberName;)" OBJ, (void *)staticFieldBase},
    {"getMemberVMInfo", "(Ljava/lang/invoke/MemberName;)" OBJ, (void *)getMemberVMInfo},

    // MethodHandle support
    {"getConstant", "(I)I", (void *)getConstant},

    // CallSite support
    /* Tell the JVM that we need to change the target of a CallSite. */
    {"setCallSiteTargetNormal", "(Ljava/lang/invoke/CallSite;Ljava/lang/invoke/MethodHandle)V", (void *)setCallSiteTargetNormal},
    {"setCallSiteTargetVolatile", "(Ljava/lang/invoke/CallSite;Ljava/lang/invoke/MethodHandle)V", (void *)setCallSiteTargetVolatile},
};

void java_lang_invoke_MethodHandleNatives_registerNatives()
{
    registerNatives("java/lang/invoke/MethodHandleNatives", methods, ARRAY_LENGTH(methods));
}
