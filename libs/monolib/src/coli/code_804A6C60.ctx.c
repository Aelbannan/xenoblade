// Auto-scaffolded catalog TU for monolib/src/coli/code_804A6C60
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "libs/monolib/src/coli/code_804A6C60.cpp" line 4 "harness_catalog.h" */
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

// LLM-HARNESS-BEGIN: us-804aadbc
extern "C" void func_804A6C60() {}
// LLM-HARNESS-END: us-804aadbc

// LLM-HARNESS-BEGIN: us-804aaeec
extern "C" void func_804A6D90() {}
// LLM-HARNESS-END: us-804aaeec

// LLM-HARNESS-BEGIN: us-804aaf1c
extern "C" void func_804A6DC0() {}
// LLM-HARNESS-END: us-804aaf1c

// LLM-HARNESS-BEGIN: us-804aaf7c
extern "C" void func_804A6E20() {}
// LLM-HARNESS-END: us-804aaf7c

// LLM-HARNESS-BEGIN: us-804ab254
extern "C" void func_804A70F8() {}
// LLM-HARNESS-END: us-804ab254

// LLM-HARNESS-BEGIN: us-804ab488
extern "C" void func_804A732C() {}
// LLM-HARNESS-END: us-804ab488

// LLM-HARNESS-BEGIN: us-804ab4fc
extern "C" void func_804A73A0() {}
// LLM-HARNESS-END: us-804ab4fc

// LLM-HARNESS-BEGIN: us-804ab798
extern "C" void func_804A763C() {}
// LLM-HARNESS-END: us-804ab798

// LLM-HARNESS-BEGIN: us-804ab990
extern "C" void func_804A7834() {}
// LLM-HARNESS-END: us-804ab990

// LLM-HARNESS-BEGIN: us-804ab9d4
extern "C" void func_804A7878() {}
// LLM-HARNESS-END: us-804ab9d4

// LLM-HARNESS-BEGIN: us-804aba68
extern "C" void func_804A790C() {}
// LLM-HARNESS-END: us-804aba68

// LLM-HARNESS-BEGIN: us-804abb10
extern "C" void func_804A79B4() {}
// LLM-HARNESS-END: us-804abb10

// LLM-HARNESS-BEGIN: us-804abc28
extern "C" void func_804A7ACC() {}
// LLM-HARNESS-END: us-804abc28

// LLM-HARNESS-BEGIN: us-804abd38
extern "C" void func_804A7BDC() {}
// LLM-HARNESS-END: us-804abd38

// LLM-HARNESS-BEGIN: us-804abdc0
extern "C" void func_804A7C64() {}
// LLM-HARNESS-END: us-804abdc0

// LLM-HARNESS-BEGIN: us-804abe78
extern "C" void func_804A7D1C() {}
// LLM-HARNESS-END: us-804abe78

// LLM-HARNESS-BEGIN: us-804abf74
extern "C" void func_804A7E18() {}
// LLM-HARNESS-END: us-804abf74

// LLM-HARNESS-BEGIN: us-804abfd8
extern "C" void func_804A7E7C() {}
// LLM-HARNESS-END: us-804abfd8

// LLM-HARNESS-BEGIN: us-804ac024
extern "C" bool func_804A7EC8() { return false; }
// LLM-HARNESS-END: us-804ac024

// LLM-HARNESS-BEGIN: us-804ac02c
extern "C" void func_804A7ED0() {}
// LLM-HARNESS-END: us-804ac02c

// LLM-HARNESS-BEGIN: us-804ac068
extern "C" void func_804A7F0C() {}
// LLM-HARNESS-END: us-804ac068

// LLM-HARNESS-BEGIN: us-804ac0ac
extern "C" void func_804A7F50() {}
// LLM-HARNESS-END: us-804ac0ac

// LLM-HARNESS-BEGIN: us-804ac388
extern "C" void func_804A822C(void) {}
// LLM-HARNESS-END: us-804ac388

// LLM-HARNESS-BEGIN: us-804ac38c
extern "C" void func_804A8230() {}
// LLM-HARNESS-END: us-804ac38c

// LLM-HARNESS-BEGIN: us-804ac9ac
extern "C" void func_804A8850() {}
// LLM-HARNESS-END: us-804ac9ac

// LLM-HARNESS-BEGIN: us-804acd3c
extern "C" void func_804A8BE0() {}
// LLM-HARNESS-END: us-804acd3c

// LLM-HARNESS-BEGIN: us-804ace0c
extern "C" void func_804A8CB0() {}
// LLM-HARNESS-END: us-804ace0c

// LLM-HARNESS-BEGIN: us-804ada20
extern "C" void func_804A98C4() {}
// LLM-HARNESS-END: us-804ada20

// LLM-HARNESS-BEGIN: us-804ae650
extern "C" void func_804AA4F4() {}
// LLM-HARNESS-END: us-804ae650

// LLM-HARNESS-BEGIN: us-804ae660
extern "C" void func_804AA504() {}
// LLM-HARNESS-END: us-804ae660

// LLM-HARNESS-BEGIN: us-804ae9cc
extern "C" void func_804AA870() {}
// LLM-HARNESS-END: us-804ae9cc

// LLM-HARNESS-BEGIN: us-804aebf4
extern "C" void func_804AAA98() {}
// LLM-HARNESS-END: us-804aebf4

// LLM-HARNESS-BEGIN: us-804aeeec
extern "C" void func_804AAD90() {}
// LLM-HARNESS-END: us-804aeeec

// LLM-HARNESS-BEGIN: us-804af680
extern "C" void func_804AB524() {}
// LLM-HARNESS-END: us-804af680

// LLM-HARNESS-BEGIN: us-804af694
extern "C" void func_804AB538() {}
// LLM-HARNESS-END: us-804af694

// LLM-HARNESS-BEGIN: us-804afb64
extern "C" void func_804ABA08() {}
// LLM-HARNESS-END: us-804afb64

// LLM-HARNESS-BEGIN: us-804afbc4
extern "C" void func_804ABA68() {}
// LLM-HARNESS-END: us-804afbc4

// LLM-HARNESS-BEGIN: us-804afc4c
extern "C" void func_804ABAF0() {}
// LLM-HARNESS-END: us-804afc4c

// LLM-HARNESS-BEGIN: us-804afd50
extern "C" void func_804ABBF4() {}
// LLM-HARNESS-END: us-804afd50

// LLM-HARNESS-BEGIN: us-804afe00
extern "C" void func_804ABCA4() {}
// LLM-HARNESS-END: us-804afe00

// LLM-HARNESS-BEGIN: us-804afe68
extern "C" void func_804ABD0C() {}
// LLM-HARNESS-END: us-804afe68

// LLM-HARNESS-BEGIN: us-804aff30
extern "C" void func_804ABDD4() {}
// LLM-HARNESS-END: us-804aff30

// LLM-HARNESS-BEGIN: us-804affe0
extern "C" void func_804ABE84() {}
// LLM-HARNESS-END: us-804affe0

// LLM-HARNESS-BEGIN: us-804b0064
extern "C" void func_804ABF08() {}
// LLM-HARNESS-END: us-804b0064

// LLM-HARNESS-BEGIN: us-804b017c
extern "C" void func_804AC020() {}
// LLM-HARNESS-END: us-804b017c

// LLM-HARNESS-BEGIN: us-804b02f4
extern "C" void func_804AC198() {}
// LLM-HARNESS-END: us-804b02f4

// LLM-HARNESS-BEGIN: us-804b050c
extern "C" void func_804AC3B0() {}
// LLM-HARNESS-END: us-804b050c

// LLM-HARNESS-BEGIN: us-804b0640
extern "C" void func_804AC4E4() {}
// LLM-HARNESS-END: us-804b0640

// LLM-HARNESS-BEGIN: us-804b0650
extern "C" void func_804AC4F4() {}
// LLM-HARNESS-END: us-804b0650

// LLM-HARNESS-BEGIN: us-804b06d8
extern "C" void func_804AC57C() {}
// LLM-HARNESS-END: us-804b06d8

// LLM-HARNESS-BEGIN: us-804b0734
extern "C" void func_804AC5D8() {}
// LLM-HARNESS-END: us-804b0734

// LLM-HARNESS-BEGIN: us-804b0778
extern "C" int func_804AC61C(void* self) { return 0; }
// LLM-HARNESS-END: us-804b0778

// LLM-HARNESS-BEGIN: us-804b0780
extern "C" void func_804AC624() {}
// LLM-HARNESS-END: us-804b0780

// LLM-HARNESS-BEGIN: us-804b0b50
extern "C" void func_804AC9F4() {}
// LLM-HARNESS-END: us-804b0b50

// LLM-HARNESS-BEGIN: us-804b0ef8
extern "C" void func_804ACD9C() {}
// LLM-HARNESS-END: us-804b0ef8

// LLM-HARNESS-BEGIN: us-804b133c
extern "C" void func_804AD1E0() {}
// LLM-HARNESS-END: us-804b133c

// LLM-HARNESS-BEGIN: us-804b156c
extern "C" void func_804AD410() {}
// LLM-HARNESS-END: us-804b156c

// LLM-HARNESS-BEGIN: us-804b1a58
extern "C" void func_804AD8FC() {}
// LLM-HARNESS-END: us-804b1a58

// LLM-HARNESS-BEGIN: us-804b1e98
extern "C" void func_804ADD3C() {}
// LLM-HARNESS-END: us-804b1e98

// LLM-HARNESS-BEGIN: us-804b222c
extern "C" void func_804AE0D0() {}
// LLM-HARNESS-END: us-804b222c

// LLM-HARNESS-BEGIN: us-804b2278
extern "C" void func_804AE11C() {}
// LLM-HARNESS-END: us-804b2278

// LLM-HARNESS-BEGIN: us-804b24e4
extern "C" void func_804AE388() {}
// LLM-HARNESS-END: us-804b24e4

// LLM-HARNESS-BEGIN: us-804b2b00
extern "C" void func_804AE9A4() {}
// LLM-HARNESS-END: us-804b2b00

// LLM-HARNESS-BEGIN: us-804b2de8
extern "C" void func_804AEC8C() {}
// LLM-HARNESS-END: us-804b2de8

// LLM-HARNESS-BEGIN: us-804b31d8
extern "C" void func_804AF07C() {}
// LLM-HARNESS-END: us-804b31d8

// LLM-HARNESS-BEGIN: us-804b31f8
extern "C" void func_804AF09C() {}
// LLM-HARNESS-END: us-804b31f8

// LLM-HARNESS-BEGIN: us-804b344c
extern "C" void func_804AF2F0() {}
// LLM-HARNESS-END: us-804b344c

// LLM-HARNESS-BEGIN: us-804b346c
extern "C" void func_804AF310() {}
// LLM-HARNESS-END: us-804b346c

// LLM-HARNESS-BEGIN: us-804b3488
extern "C" void func_804AF32C() {}
// LLM-HARNESS-END: us-804b3488

// LLM-HARNESS-BEGIN: us-804b3964
extern "C" void func_804AF808() {}
// LLM-HARNESS-END: us-804b3964

// LLM-HARNESS-BEGIN: us-804b3ae8
extern "C" void func_804AF98C() {}
// LLM-HARNESS-END: us-804b3ae8

// LLM-HARNESS-BEGIN: us-804b3b64
extern "C" void func_804AFA08() {}
// LLM-HARNESS-END: us-804b3b64

// LLM-HARNESS-BEGIN: us-804b3c84
extern "C" void func_804AFB28() {}
// LLM-HARNESS-END: us-804b3c84

// LLM-HARNESS-BEGIN: us-804b43e8
extern "C" void func_804B028C() {}
// LLM-HARNESS-END: us-804b43e8

// LLM-HARNESS-BEGIN: us-804b4858
extern "C" void func_804B06FC() {}
// LLM-HARNESS-END: us-804b4858

// LLM-HARNESS-BEGIN: us-804b4898
extern "C" void func_804B073C() {}
// LLM-HARNESS-END: us-804b4898

// LLM-HARNESS-BEGIN: us-804b48d8
extern "C" void func_804B077C() {}
// LLM-HARNESS-END: us-804b48d8

// LLM-HARNESS-BEGIN: us-804b494c
extern "C" void func_804B07F0() {}
// LLM-HARNESS-END: us-804b494c

// LLM-HARNESS-BEGIN: us-804b4974
extern "C" void func_804B0818() {}
// LLM-HARNESS-END: us-804b4974

// LLM-HARNESS-BEGIN: us-804b49fc
extern "C" void func_804B08A0() {}
// LLM-HARNESS-END: us-804b49fc

// LLM-HARNESS-BEGIN: us-804b4a80
extern "C" void func_804B0924() {}
// LLM-HARNESS-END: us-804b4a80

// LLM-HARNESS-BEGIN: us-804b4ab8
extern "C" void __dt__804B095C() {}
// LLM-HARNESS-END: us-804b4ab8

// LLM-HARNESS-BEGIN: us-804b4b24
extern "C" void func_804B09C8() {}
// LLM-HARNESS-END: us-804b4b24

// LLM-HARNESS-BEGIN: us-804b4bc8
extern "C" bool func_804B0A6C() { return false; }
// LLM-HARNESS-END: us-804b4bc8

// LLM-HARNESS-BEGIN: us-804b4bd0
extern "C" bool func_804B0A74() { return false; }
// LLM-HARNESS-END: us-804b4bd0

// LLM-HARNESS-BEGIN: us-804b4bd8
extern "C" void func_804B0A7C() {}
// LLM-HARNESS-END: us-804b4bd8

// LLM-HARNESS-BEGIN: us-804b4c30
extern "C" void func_804B0AD4() {}
// LLM-HARNESS-END: us-804b4c30

// LLM-HARNESS-BEGIN: us-804b4c68
extern "C" void func_804B0B0C() {}
// LLM-HARNESS-END: us-804b4c68

// LLM-HARNESS-BEGIN: us-804b4cb0
extern "C" void func_804B0B54() {}
// LLM-HARNESS-END: us-804b4cb0

// LLM-HARNESS-BEGIN: us-804b4d68
extern "C" void func_804B0C0C() {}
// LLM-HARNESS-END: us-804b4d68

// LLM-HARNESS-BEGIN: us-804b4e44
extern "C" void func_804B0CE8() {}
// LLM-HARNESS-END: us-804b4e44

// LLM-HARNESS-BEGIN: us-804b4f50
extern "C" void func_804B0DF4() {}
// LLM-HARNESS-END: us-804b4f50

// LLM-HARNESS-BEGIN: us-804b4ffc
extern "C" void func_804B0EA0() {}
// LLM-HARNESS-END: us-804b4ffc

// LLM-HARNESS-BEGIN: us-804b5188
extern "C" void func_804B102C() {}
// LLM-HARNESS-END: us-804b5188

// LLM-HARNESS-BEGIN: us-804b528c
extern "C" void func_804B1130() {}
// LLM-HARNESS-END: us-804b528c

// LLM-HARNESS-BEGIN: us-804b52c0
extern "C" void func_804B1164() {}
// LLM-HARNESS-END: us-804b52c0

// LLM-HARNESS-BEGIN: us-804b5a88
extern "C" void func_804B192C() {}
// LLM-HARNESS-END: us-804b5a88

// LLM-HARNESS-BEGIN: us-804b5b28
extern "C" void func_804B19CC() {}
// LLM-HARNESS-END: us-804b5b28

// LLM-HARNESS-BEGIN: us-804b5c34
extern "C" void func_804B1AD8() {}
// LLM-HARNESS-END: us-804b5c34

// LLM-HARNESS-BEGIN: us-804b5d38
extern "C" void func_804B1BDC() {}
// LLM-HARNESS-END: us-804b5d38

// LLM-HARNESS-BEGIN: us-804b5df8
extern "C" void func_804B1C9C() {}
// LLM-HARNESS-END: us-804b5df8

// LLM-HARNESS-BEGIN: us-804b5f1c
extern "C" void func_804B1DC0() {}
// LLM-HARNESS-END: us-804b5f1c

// LLM-HARNESS-BEGIN: us-804b5f48
extern "C" void func_804B1DEC() {}
// LLM-HARNESS-END: us-804b5f48

// LLM-HARNESS-BEGIN: us-804b61a8
extern "C" void func_804B204C() {}
// LLM-HARNESS-END: us-804b61a8

// LLM-HARNESS-BEGIN: us-804b6304
extern "C" void func_804B21A8() {}
// LLM-HARNESS-END: us-804b6304

// LLM-HARNESS-BEGIN: us-804b64c8
extern "C" void func_804B236C() {}
// LLM-HARNESS-END: us-804b64c8

// LLM-HARNESS-BEGIN: us-804b6600
extern "C" void func_804B24A4() {}
// LLM-HARNESS-END: us-804b6600

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// LLM-HARNESS-BEGIN: us-804b6680
extern "C" void sinit_804B2524() {}
// LLM-HARNESS-END: us-804b6680
