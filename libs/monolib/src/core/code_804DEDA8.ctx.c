// Auto-scaffolded catalog TU for monolib/src/core/code_804DEDA8
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "libs/monolib/src/core/code_804DEDA8.cpp" line 4 "harness_catalog.h" */
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

// LLM-HARNESS-BEGIN: us-804e3244
extern "C" void func_804DEDA8() {}
// LLM-HARNESS-END: us-804e3244

// LLM-HARNESS-BEGIN: us-804e3268
extern "C" void __dt__804DEDCC() {}
// LLM-HARNESS-END: us-804e3268

// LLM-HARNESS-BEGIN: us-804e3504
extern "C" void __dt__804DF068() {}
// LLM-HARNESS-END: us-804e3504

// LLM-HARNESS-BEGIN: us-804e35b4
extern "C" void func_804DF118() {}
// LLM-HARNESS-END: us-804e35b4

// LLM-HARNESS-BEGIN: us-804e35ec
extern "C" void func_804DF150() {}
// LLM-HARNESS-END: us-804e35ec

// LLM-HARNESS-BEGIN: us-804e3600
extern "C" void func_804DF164() {}
// LLM-HARNESS-END: us-804e3600

// LLM-HARNESS-BEGIN: us-804e3744
extern "C" void func_804DF2A8() {}
// LLM-HARNESS-END: us-804e3744

// LLM-HARNESS-BEGIN: us-804e3760
extern "C" void func_804DF2C4() {}
// LLM-HARNESS-END: us-804e3760

// LLM-HARNESS-BEGIN: us-804e378c
extern "C" void func_804DF2F0() {}
// LLM-HARNESS-END: us-804e378c

// LLM-HARNESS-BEGIN: us-804e37e0
extern "C" void func_804DF344() {}
// LLM-HARNESS-END: us-804e37e0

// LLM-HARNESS-BEGIN: us-804e386c
extern "C" void func_804DF3D0() {}
// LLM-HARNESS-END: us-804e386c

// LLM-HARNESS-BEGIN: us-804e3958
extern "C" void func_804DF4BC() {}
// LLM-HARNESS-END: us-804e3958

// LLM-HARNESS-BEGIN: us-804e3a94
extern "C" void func_804DF5F8() {}
// LLM-HARNESS-END: us-804e3a94

// LLM-HARNESS-BEGIN: us-804e3b2c
extern "C" void func_804DF690() {}
// LLM-HARNESS-END: us-804e3b2c

// LLM-HARNESS-BEGIN: us-804e3be0
extern "C" void __dt__804DF744() {}
// LLM-HARNESS-END: us-804e3be0

// LLM-HARNESS-BEGIN: us-804e3c40
extern "C" void func_804DF7A4() {}
// LLM-HARNESS-END: us-804e3c40

// LLM-HARNESS-BEGIN: us-804e3c98
extern "C" void func_804DF7FC(void) {}
// LLM-HARNESS-END: us-804e3c98

// LLM-HARNESS-BEGIN: us-804e3ca4
extern "C" void func_804DF808() {}
// LLM-HARNESS-END: us-804e3ca4

// LLM-HARNESS-BEGIN: us-804e3ea4
extern "C" void func_804DFA08() {}
// LLM-HARNESS-END: us-804e3ea4

// LLM-HARNESS-BEGIN: us-804e3f20
extern "C" void func_804DFA84() {}
// LLM-HARNESS-END: us-804e3f20

// LLM-HARNESS-BEGIN: us-804e4024
extern "C" void func_804DFB88() {}
// LLM-HARNESS-END: us-804e4024

// LLM-HARNESS-BEGIN: us-804e4090
extern "C" void func_804DFBF4() {}
// LLM-HARNESS-END: us-804e4090

// LLM-HARNESS-BEGIN: us-804e40e4
extern "C" void func_804DFC48() {}
// LLM-HARNESS-END: us-804e40e4

// LLM-HARNESS-BEGIN: us-804e4160
extern "C" void func_804DFCC4() {}
// LLM-HARNESS-END: us-804e4160

// LLM-HARNESS-BEGIN: us-804e42bc
extern "C" void func_804DFE20() {}
// LLM-HARNESS-END: us-804e42bc

// LLM-HARNESS-BEGIN: us-804e4328
extern "C" void func_804DFE8C() {}
// LLM-HARNESS-END: us-804e4328

// LLM-HARNESS-BEGIN: us-804e4338
extern "C" void func_804DFE9C() {}
// LLM-HARNESS-END: us-804e4338

// LLM-HARNESS-BEGIN: us-804e4348
extern "C" void func_804DFEAC() {}
// LLM-HARNESS-END: us-804e4348

// LLM-HARNESS-BEGIN: us-804e439c
extern "C" void func_804DFF00() {}
// LLM-HARNESS-END: us-804e439c

// LLM-HARNESS-BEGIN: us-804e4444
extern "C" void func_804DFFA8() {}
// LLM-HARNESS-END: us-804e4444

// LLM-HARNESS-BEGIN: us-804e4534
extern "C" void func_804E0098() {}
// LLM-HARNESS-END: us-804e4534

// LLM-HARNESS-BEGIN: us-804e45a0
extern "C" void func_804E0104() {}
// LLM-HARNESS-END: us-804e45a0

// LLM-HARNESS-BEGIN: us-804e45b0
extern "C" void func_804E0114() {}
// LLM-HARNESS-END: us-804e45b0

// LLM-HARNESS-BEGIN: us-804e4604
extern "C" void func_804E0168() {}
// LLM-HARNESS-END: us-804e4604

// LLM-HARNESS-BEGIN: us-804e4624
extern "C" void func_804E0188() {}
// LLM-HARNESS-END: us-804e4624

// LLM-HARNESS-BEGIN: us-804e46e4
extern "C" void func_804E0248() {}
// LLM-HARNESS-END: us-804e46e4

// LLM-HARNESS-BEGIN: us-804e4970
extern "C" void func_804E04D4() {}
// LLM-HARNESS-END: us-804e4970

// LLM-HARNESS-BEGIN: us-804e4a1c
extern "C" void func_804E0580() {}
// LLM-HARNESS-END: us-804e4a1c

// LLM-HARNESS-BEGIN: us-804e4b50
extern "C" void func_804E06B4() {}
// LLM-HARNESS-END: us-804e4b50

// LLM-HARNESS-BEGIN: us-804e4c24
extern "C" void func_804E0788() {}
// LLM-HARNESS-END: us-804e4c24

// LLM-HARNESS-BEGIN: us-804e4d58
extern "C" void func_804E08BC() {}
// LLM-HARNESS-END: us-804e4d58

// LLM-HARNESS-BEGIN: us-804e4e2c
extern "C" void func_804E0990() {}
// LLM-HARNESS-END: us-804e4e2c

// LLM-HARNESS-BEGIN: us-804e5030
extern "C" void func_804E0B94() {}
// LLM-HARNESS-END: us-804e5030

// LLM-HARNESS-BEGIN: us-804e518c
extern "C" void func_804E0CF0() {}
// LLM-HARNESS-END: us-804e518c

// LLM-HARNESS-BEGIN: us-804e52e4
extern "C" void func_804E0E48() {}
// LLM-HARNESS-END: us-804e52e4

// LLM-HARNESS-BEGIN: us-804e54e0
extern "C" void func_804E1044() {}
// LLM-HARNESS-END: us-804e54e0

// LLM-HARNESS-BEGIN: us-804e5730
extern "C" void func_804E1294() {}
// LLM-HARNESS-END: us-804e5730

// LLM-HARNESS-BEGIN: us-804e5c40
extern "C" void func_804E17A4() {}
// LLM-HARNESS-END: us-804e5c40

// LLM-HARNESS-BEGIN: us-804e5d68
extern "C" void func_804E18CC() {}
// LLM-HARNESS-END: us-804e5d68

// LLM-HARNESS-BEGIN: us-804e5e08
extern "C" void func_804E196C() {}
// LLM-HARNESS-END: us-804e5e08

// LLM-HARNESS-BEGIN: us-804e5ee0
extern "C" void func_804E1A44() {}
// LLM-HARNESS-END: us-804e5ee0

// LLM-HARNESS-BEGIN: us-804e5f44
extern "C" void func_804E1AA8() {}
// LLM-HARNESS-END: us-804e5f44

// LLM-HARNESS-BEGIN: us-804e60b8
extern "C" void func_804E1C1C() {}
// LLM-HARNESS-END: us-804e60b8

// LLM-HARNESS-BEGIN: us-804e61ec
extern "C" void func_804E1D50() {}
// LLM-HARNESS-END: us-804e61ec

// LLM-HARNESS-BEGIN: us-804e6524
extern "C" void func_804E2088() {}
// LLM-HARNESS-END: us-804e6524

// LLM-HARNESS-BEGIN: us-804e65e8
extern "C" void func_804E214C() {}
// LLM-HARNESS-END: us-804e65e8

// LLM-HARNESS-BEGIN: us-804e6944
extern "C" void func_804E24A8() {}
// LLM-HARNESS-END: us-804e6944

// LLM-HARNESS-BEGIN: us-804e6b74
extern "C" void func_804E26D8() {}
// LLM-HARNESS-END: us-804e6b74

// LLM-HARNESS-BEGIN: us-804e6ef8
extern "C" void func_804E2A5C() {}
// LLM-HARNESS-END: us-804e6ef8

// LLM-HARNESS-BEGIN: us-804e6ff0
extern "C" void func_804E2B54() {}
// LLM-HARNESS-END: us-804e6ff0

// LLM-HARNESS-BEGIN: us-804e7228
extern "C" void func_804E2D8C() {}
// LLM-HARNESS-END: us-804e7228

// LLM-HARNESS-BEGIN: us-804e7348
extern "C" void func_804E2EAC() {}
// LLM-HARNESS-END: us-804e7348

// LLM-HARNESS-BEGIN: us-804e7418
extern "C" void func_804E2F7C() {}
// LLM-HARNESS-END: us-804e7418

// LLM-HARNESS-BEGIN: us-804e758c
extern "C" void func_804E30F0() {}
// LLM-HARNESS-END: us-804e758c
