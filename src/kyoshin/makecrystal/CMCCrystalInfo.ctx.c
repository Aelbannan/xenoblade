// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalInfo
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/makecrystal/CMCCrystalInfo.cpp" line 4 "kyoshin/makecrystal/CMCCrystalInfo.hpp" */
#pragma once

/* "src/kyoshin/makecrystal/CMCCrystalInfo.hpp" line 2 "types.h" */
#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
/* "include/types.h" line 4 "cstdarg" */
#ifndef MSL_CPP_CSTDARG_H
#define MSL_CPP_CSTDARG_H
/* "libs/PowerPC_EABI_Support/include/stl/cstdarg" line 2 "stdarg.h" */
#ifndef MSL_STDARG_H
#define MSL_STDARG_H
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _va_arg_type {
    arg_ARGPOINTER,
    arg_WORD,
    arg_DOUBLEWORD,
    arg_ARGREAL
} _va_arg_type;

typedef struct __va_list_struct {
    char gpr;
    char fpr;
    char reserved[2];
    char* input_arg_area;
    char* reg_save_area;
} va_list[1];

void* __va_arg(va_list argp, int type);

#define va_start(VA_LIST, ARG) ((void)ARG, __builtin_va_info(&VA_LIST))
#define va_end(VA_LIST) ((void)VA_LIST)
#define va_arg(VA_LIST, ARG_TYPE)                                              \
    (*(ARG_TYPE*)__va_arg(VA_LIST, _var_arg_typeof(ARG_TYPE)))

#ifdef __cplusplus
}
#endif
#endif
/* end "stdarg.h" */
#ifdef __cplusplus

namespace std {
using ::va_list;
} // namespace std

#endif
#endif
/* end "cstdarg" */
/* "include/types.h" line 5 "cstddef" */
#ifndef MSL_CPP_CSTDDEF_H
#define MSL_CPP_CSTDDEF_H
/* "libs/PowerPC_EABI_Support/include/stl/cstddef" line 2 "stddef.h" */
#ifndef MSL_STDDEF_H
#define MSL_STDDEF_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif // NULL

#ifndef nullptr
#define nullptr 0
#endif // nullptr

#define offsetof(ST, M) ((size_t) & (((ST*)0)->M))

typedef signed long ptrdiff_t;
typedef unsigned long size_t;

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

typedef wchar_t wint_t;

typedef void (*funcptr_t)(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "stddef.h" */
#ifdef __cplusplus

namespace std {
using ::ptrdiff_t;
using ::size_t;
} // namespace std

#endif
#endif
/* end "cstddef" */
/* "include/types.h" line 6 "cstdint" */
#ifndef MSL_CPP_CSTDINT_H
#define MSL_CPP_CSTDINT_H
/* "libs/PowerPC_EABI_Support/include/stl/cstdint" line 2 "stdint.h" */
#ifndef MSL_STDINT_H
#define MSL_STDINT_H

/* "libs/PowerPC_EABI_Support/include/stl/stdint.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

typedef signed char int8_t;
typedef short       int16_t;
typedef long        int32_t;

typedef unsigned long long uint64_t;
typedef long long          int64_t;

typedef uint32_t uintptr_t;
typedef int32_t  intptr_t;

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "stdint.h" */
#ifdef __cplusplus

namespace std {
using ::uint8_t;
using ::uint16_t;
using ::uint32_t;
using ::int8_t;
using ::int16_t;
using ::int32_t;
using ::uint64_t;
using ::int64_t;
using ::uintptr_t;
using ::intptr_t;
} // namespace std

#endif
#endif
/* end "cstdint" */
/* "include/types.h" line 7 "new" */
#ifndef MSL_CPP_NEW_H
#define MSL_CPP_NEW_H
/* "libs/PowerPC_EABI_Support/include/stl/new" line 2 "stddef.h" */
/* end "stddef.h" */
#ifdef __cplusplus
/* "libs/PowerPC_EABI_Support/include/stl/new" line 4 "exception" */
#ifndef MSL_CPP_EXCEPTION_H
#define MSL_CPP_EXCEPTION_H

namespace std{
    class exception{
    public:
        exception(){}
        virtual ~exception(){}
        virtual const char* what() const {
            return "exception";
        }
    };

    class bad_exception : public exception {
    public:
        bad_exception(){}
        virtual ~bad_exception(){}
        virtual const char* what() const {
            return "bad_exception";
        }
    };

    typedef void (*unexpected_handler)();
    unexpected_handler set_unexpected(unexpected_handler handler);
    void unexpected();

    typedef void (*terminate_handler)();
    terminate_handler set_terminate(terminate_handler handler);
    void terminate();

}

#endif
/* end "exception" */

namespace std{
    class bad_alloc : public exception {
    public:
        virtual ~bad_alloc(){}
        virtual const char* what() const{
            return "bad_alloc";
        }
    };

    struct nothrow_t {};
    extern const nothrow_t nothrow;
}

inline void* operator new(size_t size, void* ptr) {
#pragma unused(size)
    return ptr;
}

inline void* operator new[](size_t size, void* ptr) {
#pragma unused(size)
    return ptr;
}

#endif
#endif
/* end "new" */
#else
/* "include/types.h" line 9 "stdarg.h" */
/* end "stdarg.h" */
/* "include/types.h" line 10 "stddef.h" */
/* end "stddef.h" */
/* "include/types.h" line 11 "stdint.h" */
/* end "stdint.h" */
#endif

/* "include/types.h" line 14 "compat.h" */
/**
 * Compatability macros for language parsers such as Clangd and Intellisense
 */

#ifndef COMPAT_H
#define COMPAT_H

/******************************************************************************
 *
 * Detect language parsers
 *
 ******************************************************************************/

// Detect Visual Studio Intellisense
#if defined(__INTELLISENSE__)
#define COMPAT_INTELLISENSE
#endif

// Detect Clangd
#if defined(__clang__)
#define COMPAT_CLANGD
#endif

// Define macro to test for ANY language parser
#if defined(COMPAT_INTELLISENSE) || defined(COMPAT_CLANGD)
#define COMPAT_ANY
#endif

/******************************************************************************
 *
 * Common parser fixes
 *
 ******************************************************************************/

#if defined(COMPAT_ANY)

// Hide assembly keywords
#define register
#define asm

// Hide inline assembly
#define ASM(...)
#define ASM_VOLATILE(...)

// Hide attributes
#define __attribute__(...)
#define __declspec(...)

// Fix decltype
#define DECLTYPE(x) __decltype(x)

// Hide memory-mapped symbols
#define DECL_ADDRESS(...)

// Hide flexible arrays
#define FLEXIBLE_ARRAY(NAME) NAME[0]

// Fix __vec2x32float__
typedef int __vec2x32float__;

#else

// Keep inline assembly
#define ASM asm
#define ASM_VOLATILE asm volatile

// decltype
#define DECLTYPE(x) __decltype__(x)

// Place a symbol at a specific address
#define DECL_ADDRESS(addr) : addr

// Allow flexible arrays
#define FLEXIBLE_ARRAY(NAME) NAME[]

#endif

/******************************************************************************
 *
 * Intellisense fixes
 *
 ******************************************************************************/

#if defined(COMPAT_INTELLISENSE)

// Redirect compiler intrinsics
#define __alloca(...)
#define __memclr(...)

#endif

/******************************************************************************
 *
 * Clangd fixes
 *
 ******************************************************************************/

#if defined(COMPAT_CLANGD)

// Redirect compiler intrinsics
#define __alloca __builtin_alloca
#define __memclr(var, size) __builtin_memset((var), 0, size)
#define offsetof __builtin_offsetof

// Fix variadic argument macros
// TODO(kiwi) gotta be a less intrusive way
#undef va_start
#define va_start(...)
#undef va_arg
#define va_arg(v, t) t()

#endif

/******************************************************************************
 *
 * PowerPC compiler intrinsics
 *
 ******************************************************************************/

#if defined(COMPAT_ANY)

// Hide PowerPC intrinsics
#define __abs(...) 0
#define __cntlzw(...) 0
#define __dcbf(...)
#define __dcbst(...)
#define __dcbt(...)
#define __dcbtst(...)
#define __dcbz(...)
#define __divw(...) 0
#define __divwu(...) 0
#define __eieio(...)
#define __fabs(...) 0
#define __fabsf(...) 0
#define __fmadd(...) 0
#define __fmadds(...) 0
#define __fmsub(...) 0
#define __fmsubs(...) 0
#define __fnabs(...) 0
#define __fnabsf(...) 0
#define __fnmadd(...) 0
#define __fnmadds(...) 0
#define __fnmsub(...) 0
#define __fnmsubs(...) 0
#define __fres(...) 0
#define __frsqrte(...) 0
#define __fsel(...) 0
#define __isync(...)
#define __labs(...) 0
#define __lhbrx(...) 0
#define __lwbrx(...) 0
#define __mffs(...) 0
#define __mulhw(...) 0
#define __mulhwu(...) 0
#define __rlwimi(...) 0
#define __rlwinm(...) 0
#define __rlwnm(...) 0
#define __setflm(...)
#define __sthbrx(...)
#define __strcpy(...)
#define __stwbrx(...)
#define __sync(...)

#endif

#endif
/* end "compat.h" */
/* "include/types.h" line 15 "decomp.h" */
/**
 * Codewarrior tricks for matching decomp
 * (Macros generate prototypes to satisfy -requireprotos)
 */

#ifndef DECOMP_H
#define DECOMP_H

/* "include/decomp.h" line 8 "macros.h" */
/**
 * Common macros
 */

#ifndef MACROS_H
#define MACROS_H

/******************************************************************************
 *
 * Strings
 *
 ******************************************************************************/

// Stringify expression
#define __STR(x) #x
#define STR(x) __STR(x)

// Concatenate strings
#define __CONCAT(x, y) x##y
#define CONCAT(x, y) __CONCAT(x, y)

// Multi-character character constants
// clang-format off
#define TWOCC(c0, c1)                                                          \
    (u32)((c0 & 0xFF) << 8  | (c1 & 0xFF))
#define THREECC(c0, c1, c2)                                                    \
    (u32)((c0 & 0xFF) << 16 | (c1 & 0xFF) << 8  | (c2 & 0xFF))
#define FOURCC(c0, c1, c2, c3)                                                 \
    (u32)((c0 & 0xFF) << 24 | (c1 & 0xFF) << 16 | (c2 & 0xFF) << 8 | (c3 & 0xFF))
// clang-format on

/******************************************************************************
 *
 * Arithmetic
 *
 ******************************************************************************/

// Min/max expression
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

// Clamp to a range
#define CLAMP(low, high, x)                                                    \
    ((x) > (high) ? (high) : ((x) < (low) ? (low) : (x)))

// Round up value
#define ROUND_UP(x, align) (((x) + (align) - 1) & (-(align)))
#define ROUND_UP_PTR(x, align)                                                 \
    ((void*)((((u32)(x)) + (align) - 1) & (~((align) - 1))))

// Round down value
#define ROUND_DOWN(x, align) ((x) & (-(align)))
#define ROUND_DOWN_PTR(x, align) ((void*)(((u32)(x)) & (~((align) - 1))))

// Distance between pointers
#define PTR_DISTANCE(start, end) ((u8*)(end) - (u8*)(start))

/******************************************************************************
 *
 * Arrays
 *
 ******************************************************************************/

// Size of compile-time arrays
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#define LENGTHOF(x) ARRAY_SIZE(x)

// Declare an array of hardware registers
#define DECL_HW_REGS(NAME) FLEXIBLE_ARRAY(NAME##_HW_REGS)

/******************************************************************************
 *
 * Intrinsics
 *
 ******************************************************************************/

// Memory clear intrinsic
#define MEMCLR(x) __memclr((x), sizeof(*(x)))

/******************************************************************************
 *
 * Attributes
 *
 ******************************************************************************/

// Alignment attribute
#define ALIGN(x) __attribute__((aligned(x)))

// Place a symbol in a specific ELF section
#define DECL_SECTION(x) __declspec(section x)

// Give a symbol weak linkage
#define DECL_WEAK __declspec(weak)

#endif
/* end "macros.h" */

// Compile without matching hacks.
#if defined(NONMATCHING) || defined(COMPAT_ANY)
#define DECOMP_FORCEACTIVE(module, ...)
#define DECOMP_FORCELITERAL(module, ...)
#define DECOMP_FORCEACTIVE_DTOR(module, cls)
#define DECOMP_INLINE
#define DECOMP_DONT_INLINE
#define DECOMP_PPC_RLWINM(value, rot, mb, me) ((value) << (rot))
#define DECOMP_PPC_SHL1_U32(value) ((value) << 1)
#define DECOMP_ASM_INSN_BEGIN
#define DECOMP_ASM_INSN_END
// Compile with matching hacks.
// (This version of CW does not support pragmas inside macros.)
#else
// Force reference specific data
#define DECOMP_FORCEACTIVE(module, ...)                                        \
    void fake_function(...);                                                   \
    void CONCAT(FORCEACTIVE##module, __LINE__)(void);                          \
    void CONCAT(FORCEACTIVE##module, __LINE__)(void) {                         \
        fake_function(__VA_ARGS__);                                            \
    }

// Force literal ordering, such as floats in sdata2
#define DECOMP_FORCELITERAL(module, ...)                                       \
    void CONCAT(FORCELITERAL##module, __LINE__)(void);                         \
    void CONCAT(FORCELITERAL##module, __LINE__)(void) {                        \
        (__VA_ARGS__);                                                         \
    }

// Force reference destructor
#define DECOMP_FORCEACTIVE_DTOR(module, cls)                                   \
    void CONCAT(FORCEDTOR##module##cls, __LINE__)(void);                       \
    void CONCAT(FORCEDTOR##module##cls, __LINE__)(void) {                      \
        cls dummy;                                                             \
        dummy.~cls();                                                          \
    }

#define DECOMP_INLINE inline
#define DECOMP_DONT_INLINE __attribute__((never_inline))

/**
 * MWCC PPC rotate-mask intrinsics (PLAN.md section 17.6).
 * Same builtin family as SDK __rlwimi / __rlwinm; counts as high-level C, not asm.
 */
#define DECOMP_PPC_RLWINM(value, rot, mb, me) __rlwinm((value), (rot), (mb), (me))
/** slwi expansion: rlwinm rD,rA,1,0,30 */
#define DECOMP_PPC_SHL1_U32(value) DECOMP_PPC_RLWINM((value), 1, 0, 30)

/**
 * Markers for single-instruction asm carve-out (PLAN.md section 17.6).
 * Place MWCC asm { } between BEGIN and END; log policy_exception in attempts.jsonl.
 */
#define DECOMP_ASM_INSN_BEGIN
#define DECOMP_ASM_INSN_END

#endif

#endif
/* end "decomp.h" */
/* "include/types.h" line 16 "lang.h" */
/**
 * Compatability macros for deprecated/future C++ features
 */

#ifndef LANG_H
#define LANG_H

#if __cplusplus < 201103L
#define noexcept throw()
#define override
#endif

#endif
/* end "lang.h" */
/* "include/types.h" line 17 "macros.h" */
/**
 * Common macros
 */

#ifndef MACROS_H
#define MACROS_H

/******************************************************************************
 *
 * Strings
 *
 ******************************************************************************/

// Stringify expression
#define __STR(x) #x
#define STR(x) __STR(x)

// Concatenate strings
#define __CONCAT(x, y) x##y
#define CONCAT(x, y) __CONCAT(x, y)

// Multi-character character constants
// clang-format off
#define TWOCC(c0, c1)                                                          \
    (u32)((c0 & 0xFF) << 8  | (c1 & 0xFF))
#define THREECC(c0, c1, c2)                                                    \
    (u32)((c0 & 0xFF) << 16 | (c1 & 0xFF) << 8  | (c2 & 0xFF))
#define FOURCC(c0, c1, c2, c3)                                                 \
    (u32)((c0 & 0xFF) << 24 | (c1 & 0xFF) << 16 | (c2 & 0xFF) << 8 | (c3 & 0xFF))
// clang-format on

/******************************************************************************
 *
 * Arithmetic
 *
 ******************************************************************************/

// Min/max expression
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

// Clamp to a range
#define CLAMP(low, high, x)                                                    \
    ((x) > (high) ? (high) : ((x) < (low) ? (low) : (x)))

// Round up value
#define ROUND_UP(x, align) (((x) + (align) - 1) & (-(align)))
#define ROUND_UP_PTR(x, align)                                                 \
    ((void*)((((u32)(x)) + (align) - 1) & (~((align) - 1))))

// Round down value
#define ROUND_DOWN(x, align) ((x) & (-(align)))
#define ROUND_DOWN_PTR(x, align) ((void*)(((u32)(x)) & (~((align) - 1))))

// Distance between pointers
#define PTR_DISTANCE(start, end) ((u8*)(end) - (u8*)(start))

/******************************************************************************
 *
 * Arrays
 *
 ******************************************************************************/

// Size of compile-time arrays
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#define LENGTHOF(x) ARRAY_SIZE(x)

// Declare an array of hardware registers
#define DECL_HW_REGS(NAME) FLEXIBLE_ARRAY(NAME##_HW_REGS)

/******************************************************************************
 *
 * Intrinsics
 *
 ******************************************************************************/

// Memory clear intrinsic
#define MEMCLR(x) __memclr((x), sizeof(*(x)))

/******************************************************************************
 *
 * Attributes
 *
 ******************************************************************************/

// Alignment attribute
#define ALIGN(x) __attribute__((aligned(x)))

// Place a symbol in a specific ELF section
#define DECL_SECTION(x) __declspec(section x)

// Give a symbol weak linkage
#define DECL_WEAK __declspec(weak)

#endif
/* end "macros.h" */
/* "include/types.h" line 18 "build_settings.h" */
#ifndef BUILD_SETTINGS_H
#define BUILD_SETTINGS_H

//Enable bugfixes if the debug flag is enabled
#if defined(DEBUG) && !defined(BUGFIX)
    #define BUGFIX
#endif

#endif
/* end "build_settings.h" */

typedef unsigned long long u64;
typedef signed long long s64;

typedef unsigned long u32;
typedef signed long s32;

typedef unsigned short u16;
typedef signed short s16;

typedef unsigned char u8;
typedef signed char s8;

typedef float f32;
typedef double f64;

typedef unsigned int uint;

typedef int UNKWORD;
typedef void UNKTYPE;

enum { FALSE, TRUE };
typedef int BOOL;

#endif
/* end "types.h" */
/* "src/kyoshin/makecrystal/CMCCrystalInfo.hpp" line 3 "monolib/lib/UnkClass_8045F564.hpp" */
#pragma once

/* "libs/monolib/include/monolib/lib/UnkClass_8045F564.hpp" line 2 "types.h" */
/* end "types.h" */

class UnkClass_8045F564{
public:
    int unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;

    UnkClass_8045F564();
    ~UnkClass_8045F564();

    
    void createRegion(int, int, const char*, int);
    void func_8045F778();
    void func_8045F810();
};

class Class_8045F858{
public:
    void* unk0;
    u32 unk4;

    Class_8045F858(UnkClass_8045F564* unkClass);
    ~Class_8045F858();
};
/* end "monolib/lib/UnkClass_8045F564.hpp" */
/* "src/kyoshin/makecrystal/CMCCrystalInfo.hpp" line 4 "monolib/work/IWorkEvent.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/IWorkEvent.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/IWorkEvent.hpp" line 3 "monolib/monolib_types.hpp" */
#pragma once

//List of forward declarations for commonly used classes.

//Core
class CView;
class CException;

//Device
class CFileHandle;
class CDeviceFileJob;

//Math
namespace ml {
    struct CPnt16;
    struct CRect16;
    struct CVec3;
    struct CVec4;
    struct CCol3;
    struct CCol4;
    struct CMat34;
    struct CFrustum;
} //namespace ml

//Scene
class CScn;
class CScnNw4r;
class IScnRender;
class ICulling;

//Util
class CChildListNode;

//Work
class CEventFile;
class CProcess;
class CProc;
class CWorkThread;
/* end "monolib/monolib_types.hpp" */

/* Interface for work events, which provides a set of 32 event handler functions that
get triggered when a certain event happens (such as when loading a file for OnFileEvent).
Deriving classes can override any of these functions to run their own code when the
corresponding event happens.

Of the 32 available event slots, however, only events 1-5 are ever overriden, with the rest
being empty slots that were left in for some dumb reason (tysm monolithsoft <3). Additionally,
out of the 5 overriden events, only OnFileEvent and OnPauseTrigger seem to be used,
with no apparent calls to the other 3 (possibly debug only).

In XC3D, all instances of the unused event functions (including events 1, 3, and 4) are absent,
with the entries for each instead just being 0 in the vtable. This points to the extra 3 overridden
events being unused as well.

Default virtual bodies (WorkEvent1..31, OnFileEvent, OnPauseTrigger) live in
kyoshin/CGame.cpp to match retail weak placement. Only ~IWorkEvent stays in
IWorkEvent.cpp. Do not make these inline in the header -- that pulls weak stubs
into every overriding TU and blows split budgets (see MWCC_REFERENCE
CBattery/CBgTex note). */
class IWorkEvent {
public:
    virtual ~IWorkEvent();
    virtual bool WorkEvent1(UNKTYPE* r4, const char* r5);
    virtual bool OnFileEvent(CEventFile* pEventFile);
    virtual bool WorkEvent3(UNKTYPE* r4);
    virtual bool WorkEvent4();
    virtual void OnPauseTrigger(bool paused);
    // Completely unused, but still left in...
    virtual bool WorkEvent6();
    virtual bool WorkEvent7();
    virtual bool WorkEvent8();
    virtual bool WorkEvent9();
    virtual bool WorkEvent10();
    virtual bool WorkEvent11();
    virtual bool WorkEvent12();
    virtual bool WorkEvent13();
    virtual bool WorkEvent14();
    virtual bool WorkEvent15();
    virtual bool WorkEvent16();
    virtual bool WorkEvent17();
    virtual bool WorkEvent18();
    virtual bool WorkEvent19();
    virtual bool WorkEvent20();
    virtual bool WorkEvent21();
    virtual bool WorkEvent22();
    virtual bool WorkEvent23();
    virtual bool WorkEvent24();
    virtual bool WorkEvent25();
    virtual bool WorkEvent26();
    virtual bool WorkEvent27();
    virtual bool WorkEvent28();
    virtual bool WorkEvent29();
    virtual bool WorkEvent30();
    virtual void WorkEvent31();
};
/* end "monolib/work/IWorkEvent.hpp" */

// vtable at lbl_eu_80535CF8
extern "C" void* lbl_eu_80535CF8[];

/* Crystal info for the makecrystal UI. Manages crystal information display
   state. Inherits from IWorkEvent for file-load callbacks. */
class CMCCrystalInfo : public IWorkEvent {
public:
    CMCCrystalInfo();
    virtual ~CMCCrystalInfo();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    /* 0x00 */ // IWorkEvent base (vptr)
    /* 0x04 */ UnkClass_8045F564 mMemRegion1;
    /* 0x14 */ UnkClass_8045F564 mMemRegion2;
    /* 0x24 */ u32 mFileHandle1;
    /* 0x28 */ u32 mFileHandle2;
    /* 0x2C */ void* mArcResAccessor;     // nw4r::lyt::ArcResourceAccessor*
    /* 0x30 */ u32 mField30;
    /* 0x34 */ void* mLayout;              // nw4r::lyt::Layout*
    /* 0x38 */ void* mAnimTransform1;      // nw4r::lyt::AnimTransform*
    /* 0x3C */ void* mAnimTransform2;      // nw4r::lyt::AnimTransform*
    /* 0x40 */ void* mAnimTransform3;      // nw4r::lyt::AnimTransform*
    /* 0x44 */ void* mAnimTransform4;      // nw4r::lyt::AnimTransform*
    /* 0x48 */ u8 mEnabled;                // Set when layout is ready; gates proc/draw
    /* 0x4C */ s32 mState;                 // State (0=inactive, 1/2/4-8=active steps)
    /* 0x50 */ u8 mIsActive;               // Active flag; set when layout initialised
    /* 0x51 */ u8 mField51;                // Cleared when state transitions from 0→1
};
/* end "kyoshin/makecrystal/CMCCrystalInfo.hpp" */

// LLM-HARNESS-BEGIN: us-8021c48c
extern "C" void __ct__CMCCrystalInfo() {}
// LLM-HARNESS-END: us-8021c48c

// LLM-HARNESS-BEGIN: us-8021c510
extern "C" void __dt__14CMCCrystalInfoFv() {}
// LLM-HARNESS-END: us-8021c510

// LLM-HARNESS-BEGIN: us-8021c570
extern "C" void func_8021A718() {}
// LLM-HARNESS-END: us-8021c570

// LLM-HARNESS-BEGIN: us-8021c5d8
extern "C" void func_8021A780() {}
// LLM-HARNESS-END: us-8021c5d8

// LLM-HARNESS-BEGIN: us-8021c698
extern "C" void func_8021A840() {}
// LLM-HARNESS-END: us-8021c698

// LLM-HARNESS-BEGIN: us-8021c6b8
extern "C" void func_8021A860() {}
// LLM-HARNESS-END: us-8021c6b8

// LLM-HARNESS-BEGIN: us-8021c744
extern "C" u8 func_8021A8EC(CMCCrystalInfo* self) {
    return self->mIsActive;
}
// LLM-HARNESS-END: us-8021c744

// LLM-HARNESS-BEGIN: us-8021c74c
extern "C" void func_8021A8F4() {}
// LLM-HARNESS-END: us-8021c74c

// LLM-HARNESS-BEGIN: us-8021c770
extern "C" void func_8021A918() {}
// LLM-HARNESS-END: us-8021c770

// LLM-HARNESS-BEGIN: us-8021c794
extern "C" void func_8021A93C() {}
// LLM-HARNESS-END: us-8021c794

// LLM-HARNESS-BEGIN: us-8021c7b8
extern "C" void func_8021A960() {}
// LLM-HARNESS-END: us-8021c7b8

// LLM-HARNESS-BEGIN: us-8021c7dc
extern "C" void func_8021A984() {}
// LLM-HARNESS-END: us-8021c7dc

// LLM-HARNESS-BEGIN: us-8021c800
extern "C" void func_8021A9A8() {}
// LLM-HARNESS-END: us-8021c800

// LLM-HARNESS-BEGIN: us-8021c8f4
extern "C" void func_8021AA9C() {}
// LLM-HARNESS-END: us-8021c8f4

// LLM-HARNESS-BEGIN: us-8021cc1c
extern "C" void func_8021ADC4() {}
// LLM-HARNESS-END: us-8021cc1c

// LLM-HARNESS-BEGIN: us-8021cd28
extern "C" void func_8021AED0() {}
// LLM-HARNESS-END: us-8021cd28

// LLM-HARNESS-BEGIN: us-8021cdcc
extern "C" void func_8021AF74() {}
// LLM-HARNESS-END: us-8021cdcc

// LLM-HARNESS-BEGIN: us-8021ce18
extern "C" void func_8021AFC0() {}
// LLM-HARNESS-END: us-8021ce18

// LLM-HARNESS-BEGIN: us-8021ce64
extern "C" void func_8021B00C() {}
// LLM-HARNESS-END: us-8021ce64

// LLM-HARNESS-BEGIN: us-8021ceb0
extern "C" void func_8021B058() {}
// LLM-HARNESS-END: us-8021ceb0

// LLM-HARNESS-BEGIN: us-8021cefc
extern "C" void func_8021B0A4() {}
// LLM-HARNESS-END: us-8021cefc

// LLM-HARNESS-BEGIN: us-8021cf48
extern "C" void func_8021B0F0() {}
// LLM-HARNESS-END: us-8021cf48

// LLM-HARNESS-BEGIN: us-8021cf94
extern "C" void func_8021B13C() {}
// LLM-HARNESS-END: us-8021cf94

// LLM-HARNESS-BEGIN: us-8021cfe0
extern "C" void func_8021B188() {}
// LLM-HARNESS-END: us-8021cfe0

// LLM-HARNESS-BEGIN: us-8021d138
extern "C" void func_8021B2E0() {}
// LLM-HARNESS-END: us-8021d138

// LLM-HARNESS-BEGIN: us-8021d284
extern "C" void func_8021B42C() {}
// LLM-HARNESS-END: us-8021d284

// LLM-HARNESS-BEGIN: us-8021d358
extern "C" void func_8021B500(void* this_) {
    unsigned int* p34 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x34);
    unsigned int* p30 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x30);
    if (*p34 != 0) {
        if (*p30 != 0) {
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x48) = 1;
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x50) = 1;
        }
    }
}
// LLM-HARNESS-END: us-8021d358

// LLM-HARNESS-BEGIN: us-8021d384
extern "C" void func_8021B52C() {}
// LLM-HARNESS-END: us-8021d384

// LLM-HARNESS-BEGIN: us-8021d40c
extern "C" void func_8021B5B4() {}
// LLM-HARNESS-END: us-8021d40c

// LLM-HARNESS-BEGIN: us-8021d494
extern "C" void func_8021B63C() {}
// LLM-HARNESS-END: us-8021d494

// LLM-HARNESS-BEGIN: us-8021d51c
extern "C" void func_8021B6C4() {}
// LLM-HARNESS-END: us-8021d51c

// LLM-HARNESS-BEGIN: us-8021d5a4
extern "C" void OnFileEvent__14CMCCrystalInfoFP10CEventFile() {}
// LLM-HARNESS-END: us-8021d5a4

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// LLM-HARNESS-BEGIN: us-8021da1c
extern "C" void sinit_8021BBC4() {}
// LLM-HARNESS-END: us-8021da1c
