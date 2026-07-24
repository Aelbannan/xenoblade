// Auto-scaffolded catalog TU for monolib/src/scn/CScnEnvLgtCtrl
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "libs/monolib/src/scn/CScnEnvLgtCtrl.cpp" line 4 "harness_catalog.h" */
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

// LLM-HARNESS-BEGIN: us-804c4d40
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c4d40

// LLM-HARNESS-BEGIN: us-804c4e38
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c4e38

// LLM-HARNESS-BEGIN: us-804c4eec
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c4eec

// LLM-HARNESS-BEGIN: us-804c4fa4
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c4fa4

// LLM-HARNESS-BEGIN: us-804c51b0
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c51b0

// LLM-HARNESS-BEGIN: us-804c51f0
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c51f0

// LLM-HARNESS-BEGIN: us-804c5398
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c5398

// LLM-HARNESS-BEGIN: us-804c53cc
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c53cc

// LLM-HARNESS-BEGIN: us-804c5400
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c5400

// LLM-HARNESS-BEGIN: us-804c5494
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804c5494

// LLM-HARNESS-BEGIN: us-804c565c
extern "C" void func_804C1500() {}
// LLM-HARNESS-END: us-804c565c

// LLM-HARNESS-BEGIN: us-804c575c
extern "C" void func_804C1600() {}
// LLM-HARNESS-END: us-804c575c

// LLM-HARNESS-BEGIN: us-804c57d0
extern "C" void func_804C1674() {}
// LLM-HARNESS-END: us-804c57d0

// LLM-HARNESS-BEGIN: us-804c587c
extern "C" void func_804C1720(void) {}
// LLM-HARNESS-END: us-804c587c

// LLM-HARNESS-BEGIN: us-804c5888
extern "C" void func_804C172C() {}
// LLM-HARNESS-END: us-804c5888

// LLM-HARNESS-BEGIN: us-804c5a68
extern "C" void func_804C190C() {}
// LLM-HARNESS-END: us-804c5a68

// LLM-HARNESS-BEGIN: us-804c5b14
extern "C" void func_804C19B8() {}
// LLM-HARNESS-END: us-804c5b14

// LLM-HARNESS-BEGIN: us-804c5bc0
extern "C" void func_804C1A64() {}
// LLM-HARNESS-END: us-804c5bc0

// LLM-HARNESS-BEGIN: us-804c5c58
extern "C" void func_804C1AFC() {}
// LLM-HARNESS-END: us-804c5c58

// LLM-HARNESS-BEGIN: us-804c5cfc
extern "C" void func_804C1BA0() {}
// LLM-HARNESS-END: us-804c5cfc

// LLM-HARNESS-BEGIN: us-804c5ed8
extern "C" void func_804C1D7C() {}
// LLM-HARNESS-END: us-804c5ed8

// LLM-HARNESS-BEGIN: us-804c606c
extern "C" void func_804C1F10() {}
// LLM-HARNESS-END: us-804c606c

// LLM-HARNESS-BEGIN: us-804c6170
extern "C" void func_804C2014() {}
// LLM-HARNESS-END: us-804c6170

// LLM-HARNESS-BEGIN: us-804c61f0
extern "C" void func_804C2094() {}
// LLM-HARNESS-END: us-804c61f0

// LLM-HARNESS-BEGIN: us-804c6280
extern "C" void func_804C2124() {}
// LLM-HARNESS-END: us-804c6280

// LLM-HARNESS-BEGIN: us-804c644c
extern "C" void func_804C22F0() {}
// LLM-HARNESS-END: us-804c644c

// LLM-HARNESS-BEGIN: us-804c67b0
extern "C" void func_804C2654() {}
// LLM-HARNESS-END: us-804c67b0

// LLM-HARNESS-BEGIN: us-804c684c
extern "C" void func_804C26F0() {}
// LLM-HARNESS-END: us-804c684c

// LLM-HARNESS-BEGIN: us-804c6afc
extern "C" void __ct__CScnEnvLgtCtrl() {}
// LLM-HARNESS-END: us-804c6afc

// LLM-HARNESS-BEGIN: us-804c71c8
extern "C" void __dt__14CScnEnvLgtCtrlFv() {}
// LLM-HARNESS-END: us-804c71c8

// LLM-HARNESS-BEGIN: us-804c7244
extern "C" void func_804C30E8() {}
// LLM-HARNESS-END: us-804c7244

// LLM-HARNESS-BEGIN: us-804c7324
extern "C" void func_804C31C8() {}
// LLM-HARNESS-END: us-804c7324

// LLM-HARNESS-BEGIN: us-804c754c
extern "C" void func_804C33F0() {}
// LLM-HARNESS-END: us-804c754c

// LLM-HARNESS-BEGIN: us-804c7560
extern "C" void func_804C3404() {}
// LLM-HARNESS-END: us-804c7560

// LLM-HARNESS-BEGIN: us-804c75fc
extern "C" void func_804C34A0() {}
// LLM-HARNESS-END: us-804c75fc

// LLM-HARNESS-BEGIN: us-804c78d4
extern "C" void func_804C3778() {}
// LLM-HARNESS-END: us-804c78d4

// LLM-HARNESS-BEGIN: us-804c7a88
extern "C" void func_804C392C() {}
// LLM-HARNESS-END: us-804c7a88

// LLM-HARNESS-BEGIN: us-804c7c24
extern "C" void func_804C3AC8() {}
// LLM-HARNESS-END: us-804c7c24

// LLM-HARNESS-BEGIN: us-804c7df8
extern "C" void func_804C3C9C() {}
// LLM-HARNESS-END: us-804c7df8

// LLM-HARNESS-BEGIN: us-804c80b4
extern "C" void func_804C3F58() {}
// LLM-HARNESS-END: us-804c80b4

// LLM-HARNESS-BEGIN: us-804c81c8
extern "C" void func_804C406C() {}
// LLM-HARNESS-END: us-804c81c8

// LLM-HARNESS-BEGIN: us-804c8404
extern "C" void func_804C42A8() {}
// LLM-HARNESS-END: us-804c8404

// LLM-HARNESS-BEGIN: us-804c8500
extern "C" void func_804C43A4() {}
// LLM-HARNESS-END: us-804c8500

// LLM-HARNESS-BEGIN: us-804c8ab0
extern "C" void func_804C4954() {}
// LLM-HARNESS-END: us-804c8ab0

// LLM-HARNESS-BEGIN: us-804c8e84
extern "C" void func_804C4D28() {}
// LLM-HARNESS-END: us-804c8e84

// LLM-HARNESS-BEGIN: us-804c8f60
extern "C" void func_804C4E04() {}
// LLM-HARNESS-END: us-804c8f60

// LLM-HARNESS-BEGIN: us-804c92f4
extern "C" void func_804C5198() {}
// LLM-HARNESS-END: us-804c92f4

// LLM-HARNESS-BEGIN: us-804c9330
extern "C" void func_804C51D4() {}
// LLM-HARNESS-END: us-804c9330

// LLM-HARNESS-BEGIN: us-804c936c
extern "C" void func_804C5210() {}
// LLM-HARNESS-END: us-804c936c

// LLM-HARNESS-BEGIN: us-804c93c8
extern "C" void func_804C526C() {}
// LLM-HARNESS-END: us-804c93c8

// LLM-HARNESS-BEGIN: us-804c94dc
extern "C" void func_804C5380() {}
// LLM-HARNESS-END: us-804c94dc

// LLM-HARNESS-BEGIN: us-804c9630
extern "C" void func_804C54D4() {}
// LLM-HARNESS-END: us-804c9630

// LLM-HARNESS-BEGIN: us-804c9784
extern "C" void func_804C5628() {}
// LLM-HARNESS-END: us-804c9784

// LLM-HARNESS-BEGIN: us-804c9a34
extern "C" void func_804C58D8() {}
// LLM-HARNESS-END: us-804c9a34

// LLM-HARNESS-BEGIN: us-804c9aec
extern "C" void func_804C5990() {}
// LLM-HARNESS-END: us-804c9aec

// LLM-HARNESS-BEGIN: us-804c9ba4
extern "C" void func_804C5A48() {}
// LLM-HARNESS-END: us-804c9ba4

// LLM-HARNESS-BEGIN: us-804c9c5c
extern "C" void func_804C5B00() {}
// LLM-HARNESS-END: us-804c9c5c

// LLM-HARNESS-BEGIN: us-804c9d64
extern "C" void func_804C5C08() {}
// LLM-HARNESS-END: us-804c9d64

// LLM-HARNESS-BEGIN: us-804c9dc8
extern "C" void func_804C5C6C() {}
// LLM-HARNESS-END: us-804c9dc8

// LLM-HARNESS-BEGIN: us-804c9ed8
extern "C" void func_804C5D7C() {}
// LLM-HARNESS-END: us-804c9ed8

// LLM-HARNESS-BEGIN: us-804c9efc
extern "C" void func_804C5DA0() {}
// LLM-HARNESS-END: us-804c9efc

// LLM-HARNESS-BEGIN: us-804c9f60
extern "C" void func_804C5E04() {}
// LLM-HARNESS-END: us-804c9f60

// LLM-HARNESS-BEGIN: us-804c9ff8
extern "C" void func_804C5E9C() {}
// LLM-HARNESS-END: us-804c9ff8

// LLM-HARNESS-BEGIN: us-804ca0c8
extern "C" void func_804C5F6C() {}
// LLM-HARNESS-END: us-804ca0c8

// LLM-HARNESS-BEGIN: us-804ca10c
extern "C" void func_804C5FB0() {}
// LLM-HARNESS-END: us-804ca10c

// LLM-HARNESS-BEGIN: us-804ca16c
extern "C" void func_804C6010() {}
// LLM-HARNESS-END: us-804ca16c

// LLM-HARNESS-BEGIN: us-804ca1b0
extern "C" void func_804C6054() {}
// LLM-HARNESS-END: us-804ca1b0

// LLM-HARNESS-BEGIN: us-804ca26c
extern "C" void func_804C6110() {}
// LLM-HARNESS-END: us-804ca26c

// LLM-HARNESS-BEGIN: us-804ca604
extern "C" void func_804C64A8() {}
// LLM-HARNESS-END: us-804ca604

// LLM-HARNESS-BEGIN: us-804ca8e8
extern "C" void func_804C678C() {}
// LLM-HARNESS-END: us-804ca8e8

// LLM-HARNESS-BEGIN: us-804cabcc
extern "C" void func_804C6A70() {}
// LLM-HARNESS-END: us-804cabcc

// LLM-HARNESS-BEGIN: us-804cac38
extern "C" u32 func_804C6ADC(u32 val) { return (val >> 16) & 0xFF; }
// LLM-HARNESS-END: us-804cac38

// LLM-HARNESS-BEGIN: us-804cac44
extern "C" int func_804C6AE8(unsigned int* arg0) {
    return (arg0[1] >> 9) & 1;
}
// LLM-HARNESS-END: us-804cac44

// LLM-HARNESS-BEGIN: us-804cac50
extern "C" void func_804C6AF4() {}
// LLM-HARNESS-END: us-804cac50

// LLM-HARNESS-BEGIN: us-804cac78
extern "C" void func_804C6B1C() {}
// LLM-HARNESS-END: us-804cac78

// LLM-HARNESS-BEGIN: us-804caca0
extern "C" void func_804C6B44() {}
// LLM-HARNESS-END: us-804caca0

// LLM-HARNESS-BEGIN: us-804cacc0
extern "C" void func_804C6B64(void* self, u32 val) { *(u32*)((u8*)self + 0xa8) = val; }
// LLM-HARNESS-END: us-804cacc0

// LLM-HARNESS-BEGIN: us-804cacc8
extern "C" void func_804C6B6C() {}
// LLM-HARNESS-END: us-804cacc8

// LLM-HARNESS-BEGIN: us-804cace4
extern "C" float func_804C6B88(void* self) { return *(float*)((u8*)self + 0xc8); }
// LLM-HARNESS-END: us-804cace4

// LLM-HARNESS-BEGIN: us-804cacec
extern "C" void func_804C6B90(void* self, float val) { *(float*)((u8*)self + 0xc8) = val; }
// LLM-HARNESS-END: us-804cacec

// LLM-HARNESS-BEGIN: us-804cacf4
extern "C" float func_804C6B98(void* self) { return *(float*)((u8*)self + 0xcc); }
// LLM-HARNESS-END: us-804cacf4

// LLM-HARNESS-BEGIN: us-804cacfc
extern "C" void func_804C6BA0(void* self, float val) { *(float*)((u8*)self + 0xcc) = val; }
// LLM-HARNESS-END: us-804cacfc

// LLM-HARNESS-BEGIN: us-804cad04
extern "C" void func_804C6BA8() {}
// LLM-HARNESS-END: us-804cad04

// LLM-HARNESS-BEGIN: us-804caec0
extern "C" void func_804C6D64() {}
// LLM-HARNESS-END: us-804caec0

// LLM-HARNESS-BEGIN: us-804cb0d4
extern "C" void func_804C6F78() {}
// LLM-HARNESS-END: us-804cb0d4

// LLM-HARNESS-BEGIN: us-804cb2ec
extern "C" void func_804C7190() {}
// LLM-HARNESS-END: us-804cb2ec

// LLM-HARNESS-BEGIN: us-804cb68c
extern "C" void func_804C7530() {}
// LLM-HARNESS-END: us-804cb68c

// LLM-HARNESS-BEGIN: us-804cb6c0
extern "C" void func_804C7564() {}
// LLM-HARNESS-END: us-804cb6c0

// LLM-HARNESS-BEGIN: us-804cb6d4
extern "C" void func_804C7578() {}
// LLM-HARNESS-END: us-804cb6d4

// LLM-HARNESS-BEGIN: us-804cb6e8
extern "C" void func_804C758C() {}
// LLM-HARNESS-END: us-804cb6e8

// LLM-HARNESS-BEGIN: us-804cb6fc
extern "C" void func_804C75A0() {}
// LLM-HARNESS-END: us-804cb6fc

// LLM-HARNESS-BEGIN: us-804cb710
extern "C" bool func_804C75B4() { return false; }
// LLM-HARNESS-END: us-804cb710

// LLM-HARNESS-BEGIN: us-804cb718
extern "C" bool func_804C75BC() { return false; }
// LLM-HARNESS-END: us-804cb718

// LLM-HARNESS-BEGIN: us-804cb720
extern "C" bool func_804C75C4() { return false; }
// LLM-HARNESS-END: us-804cb720

// LLM-HARNESS-BEGIN: us-804cb728
extern "C" void func_804C75CC() {}
// LLM-HARNESS-END: us-804cb728

// LLM-HARNESS-BEGIN: us-804cb73c
extern "C" void func_804C75E0() {}
// LLM-HARNESS-END: us-804cb73c

// LLM-HARNESS-BEGIN: us-804cb750
extern "C" void func_804C75F4() {}
// LLM-HARNESS-END: us-804cb750

// LLM-HARNESS-BEGIN: us-804cb764
extern "C" bool func_804C7608() { return false; }
// LLM-HARNESS-END: us-804cb764

// LLM-HARNESS-BEGIN: us-804cb76c
extern "C" bool func_804C7610() { return false; }
// LLM-HARNESS-END: us-804cb76c

// LLM-HARNESS-BEGIN: us-804cb774
extern "C" bool func_804C7618() { return false; }
// LLM-HARNESS-END: us-804cb774

// LLM-HARNESS-BEGIN: us-804cb77c
extern "C" void func_804C7620() {}
// LLM-HARNESS-END: us-804cb77c

// LLM-HARNESS-BEGIN: us-804cb798
extern "C" void func_804C763C() {}
// LLM-HARNESS-END: us-804cb798

// LLM-HARNESS-BEGIN: us-804cb7b4
extern "C" void func_804C7658() {}
// LLM-HARNESS-END: us-804cb7b4

// LLM-HARNESS-BEGIN: us-804cb7d0
extern "C" bool func_804C7674() { return false; }
// LLM-HARNESS-END: us-804cb7d0

// LLM-HARNESS-BEGIN: us-804cb7d8
extern "C" bool func_804C767C() { return false; }
// LLM-HARNESS-END: us-804cb7d8

// LLM-HARNESS-BEGIN: us-804cb7e0
extern "C" bool func_804C7684() { return false; }
// LLM-HARNESS-END: us-804cb7e0

// LLM-HARNESS-BEGIN: us-804cb7e8
extern "C" bool func_804C768C() { return false; }
// LLM-HARNESS-END: us-804cb7e8

// LLM-HARNESS-BEGIN: us-804cb7f0
extern "C" bool func_804C7694() { return false; }
// LLM-HARNESS-END: us-804cb7f0

// LLM-HARNESS-BEGIN: us-804cb7f8
extern "C" bool func_804C769C() { return false; }
// LLM-HARNESS-END: us-804cb7f8

// LLM-HARNESS-BEGIN: us-804cb800
extern "C" bool func_804C76A4() { return false; }
// LLM-HARNESS-END: us-804cb800

// LLM-HARNESS-BEGIN: us-804cb808
extern "C" void func_804C76AC() {}
// LLM-HARNESS-END: us-804cb808

// LLM-HARNESS-BEGIN: us-804cb824
extern "C" void func_804C76C8() {}
// LLM-HARNESS-END: us-804cb824

// LLM-HARNESS-BEGIN: us-804cb840
extern "C" void func_804C76E4() {}
// LLM-HARNESS-END: us-804cb840

// LLM-HARNESS-BEGIN: us-804cb85c
extern "C" void func_804C7700() {}
// LLM-HARNESS-END: us-804cb85c

// LLM-HARNESS-BEGIN: us-804cb878
extern "C" void func_804C771C() {}
// LLM-HARNESS-END: us-804cb878

// LLM-HARNESS-BEGIN: us-804cb894
extern "C" void func_804C7738() {}
// LLM-HARNESS-END: us-804cb894

// LLM-HARNESS-BEGIN: us-804cb8b0
extern "C" bool func_804C7754() { return false; }
// LLM-HARNESS-END: us-804cb8b0

// LLM-HARNESS-BEGIN: us-804cb8b8
extern "C" bool func_804C775C() { return false; }
// LLM-HARNESS-END: us-804cb8b8

// LLM-HARNESS-BEGIN: us-804cb8c0
extern "C" bool func_804C7764() { return false; }
// LLM-HARNESS-END: us-804cb8c0

// LLM-HARNESS-BEGIN: us-804cb8c8
extern "C" bool func_804C776C() { return false; }
// LLM-HARNESS-END: us-804cb8c8

// LLM-HARNESS-BEGIN: us-804cb8d0
extern "C" void func_804C7774() {}
// LLM-HARNESS-END: us-804cb8d0

// LLM-HARNESS-BEGIN: us-804cb8ec
extern "C" void func_804C7790() {}
// LLM-HARNESS-END: us-804cb8ec

// LLM-HARNESS-BEGIN: us-804cb9dc
extern "C" void func_804C7880() {}
// LLM-HARNESS-END: us-804cb9dc

// LLM-HARNESS-BEGIN: us-804cba6c
extern "C" void func_804C7910() {}
// LLM-HARNESS-END: us-804cba6c

// LLM-HARNESS-BEGIN: us-804cbcb0
extern "C" void func_804C7B54() {}
// LLM-HARNESS-END: us-804cbcb0

// LLM-HARNESS-BEGIN: us-804cc1b0
extern "C" void func_804C8054() {}
// LLM-HARNESS-END: us-804cc1b0

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// LLM-HARNESS-BEGIN: us-804cc2d0
extern "C" void sinit_804C8174() {}
// LLM-HARNESS-END: us-804cc2d0
