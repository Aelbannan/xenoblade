// Auto-scaffolded catalog TU for monolib/src/scn/CScnFilterMan
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "libs/monolib/src/scn/CScnFilterMan.cpp" line 4 "harness_catalog.h" */
#pragma once

/**
 * Minimal includes for auto-scaffolded catalog TUs (libs + non-kyoshin).
 * Kyoshin scaffolds use kyoshin/harness_catalog.hpp instead.
 */
/* "include/harness_catalog.h" line 6 "types.h" */
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
/* end "harness_catalog.h" */

// LLM-HARNESS-BEGIN: us-804a0aa4
extern "C" void __ct__CScnFilterMan() {}
// LLM-HARNESS-END: us-804a0aa4

// LLM-HARNESS-BEGIN: us-804a0b54
extern "C" void func_8049C9F8() {}
// LLM-HARNESS-END: us-804a0b54

// LLM-HARNESS-BEGIN: us-804a0b60
extern "C" void __ct__reslist_CScnFilter() {}
// LLM-HARNESS-END: us-804a0b60

// LLM-HARNESS-BEGIN: us-804a0b9c
extern "C" void __ct___reslist_base_CScnFilter() {}
// LLM-HARNESS-END: us-804a0b9c

// LLM-HARNESS-BEGIN: us-804a0bcc
extern "C" void __dt___reslist_base_CScnFilter() {}
// LLM-HARNESS-END: us-804a0bcc

// LLM-HARNESS-BEGIN: us-804a0c50
extern "C" void func_8049CAF4() {}
// LLM-HARNESS-END: us-804a0c50

// LLM-HARNESS-BEGIN: us-804a0cc8
extern "C" void func_8049CB6C(void) {}
// LLM-HARNESS-END: us-804a0cc8

// LLM-HARNESS-BEGIN: us-804a0ccc
extern "C" void func_8049CB70() {}
// LLM-HARNESS-END: us-804a0ccc

// LLM-HARNESS-BEGIN: us-804a0cd8
extern "C" void __dt__reslist_CScnFilter() {}
// LLM-HARNESS-END: us-804a0cd8

// LLM-HARNESS-BEGIN: us-804a0d30
extern "C" void __ct__8049CBD4() {}
// LLM-HARNESS-END: us-804a0d30

// LLM-HARNESS-BEGIN: us-804a0d6c
extern "C" void __ct__8049CC10() {}
// LLM-HARNESS-END: us-804a0d6c

// LLM-HARNESS-BEGIN: us-804a0dcc
extern "C" void func_8049CC70() {}
// LLM-HARNESS-END: us-804a0dcc

// LLM-HARNESS-BEGIN: us-804a0e00
extern "C" void func_8049CCA4(void) {}
// LLM-HARNESS-END: us-804a0e00

// LLM-HARNESS-BEGIN: us-804a0e04
extern "C" void __dt___reslist_base__reslist_iterator_CScnFilter_CScnFilter_CScnFilter() {}
// LLM-HARNESS-END: us-804a0e04

// LLM-HARNESS-BEGIN: us-804a0e90
extern "C" void func_8049CD34() {}
// LLM-HARNESS-END: us-804a0e90

// LLM-HARNESS-BEGIN: us-804a0f08
extern "C" void func_8049CDAC(void) {}
// LLM-HARNESS-END: us-804a0f08

// LLM-HARNESS-BEGIN: us-804a0f0c
extern "C" void func_8049CDB0() {}
// LLM-HARNESS-END: us-804a0f0c

// LLM-HARNESS-BEGIN: us-804a0f18
extern "C" void __dt__reslist__reslist_iterator_CScnFilter_CScnFilter_CScnFilter() {}
// LLM-HARNESS-END: us-804a0f18

// LLM-HARNESS-BEGIN: us-804a0f70
extern "C" void func_8049CE14(void) {}
// LLM-HARNESS-END: us-804a0f70

// LLM-HARNESS-BEGIN: us-804a0f74
extern "C" void func_8049CE18() {}
// LLM-HARNESS-END: us-804a0f74

// LLM-HARNESS-BEGIN: us-804a10a0
extern "C" void func_8049CF44(void) {}
// LLM-HARNESS-END: us-804a10a0

// LLM-HARNESS-BEGIN: us-804a10a4
extern "C" void func_8049CF48() {}
// LLM-HARNESS-END: us-804a10a4

// LLM-HARNESS-BEGIN: us-804a11ec
extern "C" void __dt__13CScnFilterManFv() {}
// LLM-HARNESS-END: us-804a11ec

// LLM-HARNESS-BEGIN: us-804a1288
extern "C" void func_8049D12C(void) {}
// LLM-HARNESS-END: us-804a1288

// LLM-HARNESS-BEGIN: us-804a128c
extern "C" void __dt__8049D130() {}
// LLM-HARNESS-END: us-804a128c

// LLM-HARNESS-BEGIN: us-804a12e4
extern "C" void func_8049D188(void) {}
// LLM-HARNESS-END: us-804a12e4

// LLM-HARNESS-BEGIN: us-804a12e8
extern "C" void __dt__8049D18C() {}
// LLM-HARNESS-END: us-804a12e8

// LLM-HARNESS-BEGIN: us-804a1348
extern "C" void func_8049D1EC() {}
// LLM-HARNESS-END: us-804a1348

// LLM-HARNESS-BEGIN: us-804a1374
extern "C" void func_8049D218() {}
// LLM-HARNESS-END: us-804a1374

// LLM-HARNESS-BEGIN: us-804a13c8
extern "C" void func_8049D274(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }
// LLM-HARNESS-END: us-804a13d0

// LLM-HARNESS-BEGIN: us-804a13c8
extern "C" void func_8049D26C(void* self) { func_8049D274(*(void**)((char*)self + 0x4), 0); }
// LLM-HARNESS-END: us-804a13d0

// LLM-HARNESS-BEGIN: us-804a13d8
extern "C" void func_8049D27C() {}
// LLM-HARNESS-END: us-804a13d8

// LLM-HARNESS-BEGIN: us-804a144c
extern "C" void func_8049D2F0() {}
// LLM-HARNESS-END: us-804a144c

// LLM-HARNESS-BEGIN: us-804a1494
extern "C" void func_8049D338() {}
// LLM-HARNESS-END: us-804a1494

// LLM-HARNESS-BEGIN: us-804a14f0
extern "C" void func_8049D394() {}
// LLM-HARNESS-END: us-804a14f0

// LLM-HARNESS-BEGIN: us-804a1534
extern "C" void func_8049D3D8() {}
// LLM-HARNESS-END: us-804a1534

// LLM-HARNESS-BEGIN: us-804a15ec
extern "C" void func_8049D490() {}
// LLM-HARNESS-END: us-804a15ec

// LLM-HARNESS-BEGIN: us-804a167c
extern "C" void func_8049D520() {}
// LLM-HARNESS-END: us-804a167c

// LLM-HARNESS-BEGIN: us-804a168c
extern "C" void func_8049D530() {}
// LLM-HARNESS-END: us-804a168c

// LLM-HARNESS-BEGIN: us-804a1698
extern "C" void func_8049D53C() {}
// LLM-HARNESS-END: us-804a1698

// LLM-HARNESS-BEGIN: us-804a16a4
extern "C" void func_8049D548() {}
// LLM-HARNESS-END: us-804a16a4

// LLM-HARNESS-BEGIN: us-804a16c0
extern "C" void func_8049D564() {}
// LLM-HARNESS-END: us-804a16c0

// LLM-HARNESS-BEGIN: us-804a16cc
extern "C" void func_8049D570() {}
// LLM-HARNESS-END: us-804a16cc

// LLM-HARNESS-BEGIN: us-804a174c
extern "C" void func_8049D5F0() {}
// LLM-HARNESS-END: us-804a174c

// LLM-HARNESS-BEGIN: us-804a17a0
extern "C" void func_8049D64C(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }
// LLM-HARNESS-END: us-804a17a8

// LLM-HARNESS-BEGIN: us-804a17a0
extern "C" void func_8049D644(void* self) { func_8049D64C(*(void**)((char*)self + 0x4), 0); }
// LLM-HARNESS-END: us-804a17a8

// LLM-HARNESS-BEGIN: us-804a17b0
extern "C" void func_8049D654() {}
// LLM-HARNESS-END: us-804a17b0

// LLM-HARNESS-BEGIN: us-804a1824
extern "C" void func_8049D6C8() {}
// LLM-HARNESS-END: us-804a1824

// LLM-HARNESS-BEGIN: us-804a186c
extern "C" void func_8049D710() {}
// LLM-HARNESS-END: us-804a186c

// LLM-HARNESS-BEGIN: us-804a18c8
extern "C" void func_8049D76C() {}
// LLM-HARNESS-END: us-804a18c8

// LLM-HARNESS-BEGIN: us-804a190c
extern "C" void update__13CScnFilterManFv() {}
// LLM-HARNESS-END: us-804a190c

// LLM-HARNESS-BEGIN: us-804a1a30
extern "C" void func_8049D8D4() {}
// LLM-HARNESS-END: us-804a1a30

// LLM-HARNESS-BEGIN: us-804a1a40
extern "C" void func_8049D8E4() {}
// LLM-HARNESS-END: us-804a1a40

// LLM-HARNESS-BEGIN: us-804a1a70
extern "C" void func_8049D914() {}
// LLM-HARNESS-END: us-804a1a70

// LLM-HARNESS-BEGIN: us-804a1aa4
extern "C" void func_8049D948() {}
// LLM-HARNESS-END: us-804a1aa4

// LLM-HARNESS-BEGIN: us-804a1ab0
extern "C" void func_8049D954() {}
// LLM-HARNESS-END: us-804a1ab0

// LLM-HARNESS-BEGIN: us-804a1abc
extern "C" void func_8049D960() {}
// LLM-HARNESS-END: us-804a1abc

// LLM-HARNESS-BEGIN: us-804a1af0
extern "C" void func_8049D994() {}
// LLM-HARNESS-END: us-804a1af0

// LLM-HARNESS-BEGIN: us-804a1afc
extern "C" void func_8049D9A0() {}
// LLM-HARNESS-END: us-804a1afc

// LLM-HARNESS-BEGIN: us-804a1b0c
extern "C" void func_8049D9B0() {}
// LLM-HARNESS-END: us-804a1b0c

// LLM-HARNESS-BEGIN: us-804a1b28
extern "C" void func_8049D9CC(void) {}
// LLM-HARNESS-END: us-804a1b28

// LLM-HARNESS-BEGIN: us-804a1b2c
extern "C" void func_8049D9D0() {}
// LLM-HARNESS-END: us-804a1b2c

// LLM-HARNESS-BEGIN: us-804a1c50
extern "C" u8 func_8049DAF4(void* self) { return ((u8*)self)[0x4]; }
// LLM-HARNESS-END: us-804a1c50

// LLM-HARNESS-BEGIN: us-804a1c58
extern "C" void func_8049DAFC() {}
// LLM-HARNESS-END: us-804a1c58

// LLM-HARNESS-BEGIN: us-804a1c70
extern "C" void func_8049DB14() {}
// LLM-HARNESS-END: us-804a1c70

// LLM-HARNESS-BEGIN: us-804a1d4c
extern "C" bool func_8049DBF0() { return false; }
// LLM-HARNESS-END: us-804a1d4c

// LLM-HARNESS-BEGIN: us-804a1d54
extern "C" void func_8049DBF8() {}
// LLM-HARNESS-END: us-804a1d54

// LLM-HARNESS-BEGIN: us-804a1d60
extern "C" void func_8049DC04() {}
// LLM-HARNESS-END: us-804a1d60

// LLM-HARNESS-BEGIN: us-804a1db8
extern "C" void func_8049DC5C() {}
// LLM-HARNESS-END: us-804a1db8

// LLM-HARNESS-BEGIN: us-804a1e24
extern "C" void func_8049DCC8() {}
// LLM-HARNESS-END: us-804a1e24

// LLM-HARNESS-BEGIN: us-804a1e34
extern "C" void func_8049DCD8() {}
// LLM-HARNESS-END: us-804a1e34

// LLM-HARNESS-BEGIN: us-804a1e50
extern "C" void func_8049DCF4() {}
// LLM-HARNESS-END: us-804a1e50

// LLM-HARNESS-BEGIN: us-804a1e84
extern "C" void func_8049DD28(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }
// LLM-HARNESS-END: us-804a1e84

// LLM-HARNESS-BEGIN: us-804a1e8c
extern "C" void func_8049DD30() {}
// LLM-HARNESS-END: us-804a1e8c

// LLM-HARNESS-BEGIN: us-804a1ebc
extern "C" void cbRenderBefore__13CScnFilterManFv() {}
// LLM-HARNESS-END: us-804a1ebc
