// Auto-scaffolded catalog TU for monolib/src/scn/CScnVirtualLight
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "libs/monolib/src/scn/CScnVirtualLight.cpp" line 4 "harness_catalog.h" */
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

// LLM-HARNESS-BEGIN: us-80495ee8
extern "C" void __ct__CScnVirtualLight() {}
// LLM-HARNESS-END: us-80495ee8

// LLM-HARNESS-BEGIN: us-804960a4
extern "C" void func_80492030(void) {}
// LLM-HARNESS-END: us-804960a4

// LLM-HARNESS-BEGIN: us-804960a8
extern "C" void __dt__6CLightFv() {}
// LLM-HARNESS-END: us-804960a8

// LLM-HARNESS-BEGIN: us-804960e8
extern "C" void __ct__80492074() {}
// LLM-HARNESS-END: us-804960e8

// LLM-HARNESS-BEGIN: us-80496124
extern "C" void __ct__804920B0() {}
// LLM-HARNESS-END: us-80496124

// LLM-HARNESS-BEGIN: us-80496154
extern "C" void func_804920E0() {}
// LLM-HARNESS-END: us-80496154

// LLM-HARNESS-BEGIN: us-804961cc
extern "C" void func_80492158(void) {}
// LLM-HARNESS-END: us-804961cc

// LLM-HARNESS-BEGIN: us-804961d0
extern "C" void func_8049215C() {}
// LLM-HARNESS-END: us-804961d0

// LLM-HARNESS-BEGIN: us-804961dc
extern "C" void func_80492168(void) {}
// LLM-HARNESS-END: us-804961dc

// LLM-HARNESS-BEGIN: us-804961e0
extern "C" void func_8049216C() {}
// LLM-HARNESS-END: us-804961e0

// LLM-HARNESS-BEGIN: us-8049630c
extern "C" void __dt__16CScnVirtualLightFv() {}
// LLM-HARNESS-END: us-8049630c

// LLM-HARNESS-BEGIN: us-80496410
extern "C" void func_8049239C(void) {}
// LLM-HARNESS-END: us-80496410

// LLM-HARNESS-BEGIN: us-80496414
extern "C" void __dt__804923A0() {}
// LLM-HARNESS-END: us-80496414

// LLM-HARNESS-BEGIN: us-8049646c
extern "C" void func_804923F8() {}
// LLM-HARNESS-END: us-8049646c

// LLM-HARNESS-BEGIN: us-804969f4
extern "C" void __dt__16CVirtualLightObjFv() {}
// LLM-HARNESS-END: us-804969f4

// LLM-HARNESS-BEGIN: us-80496a34
extern "C" void func_804929C0() {}
// LLM-HARNESS-END: us-80496a34

// LLM-HARNESS-BEGIN: us-80496ac4
extern "C" void func_80492A50() {}
// LLM-HARNESS-END: us-80496ac4

// LLM-HARNESS-BEGIN: us-80496ad0
extern "C" void func_80492A5C(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }
// LLM-HARNESS-END: us-80496ad0

// LLM-HARNESS-BEGIN: us-80496ad8
extern "C" void func_80492A64() {}
// LLM-HARNESS-END: us-80496ad8

// LLM-HARNESS-BEGIN: us-80496ae4
extern "C" void func_80492A70() {}
// LLM-HARNESS-END: us-80496ae4

// LLM-HARNESS-BEGIN: us-80496af4
extern "C" void func_80492A80(void* self) { func_80492A5C(*(void**)((char*)self + 0x4), 0); }
// LLM-HARNESS-END: us-80496af4

// LLM-HARNESS-BEGIN: us-80496afc
extern "C" void func_80492A88() {}
// LLM-HARNESS-END: us-80496afc

// LLM-HARNESS-BEGIN: us-80496b18
extern "C" void func_80492AA4(void) {}
// LLM-HARNESS-END: us-80496b18

// LLM-HARNESS-BEGIN: us-80496b1c
extern "C" void func_80492AA8() {}
// LLM-HARNESS-END: us-80496b1c

// LLM-HARNESS-BEGIN: us-80496b28
extern "C" void func_80492AB4() {}
// LLM-HARNESS-END: us-80496b28

// LLM-HARNESS-BEGIN: us-80496ba8
extern "C" void func_80492B34() {}
// LLM-HARNESS-END: us-80496ba8

// LLM-HARNESS-BEGIN: us-80496bb4
extern "C" void func_80492B40() {}
// LLM-HARNESS-END: us-80496bb4

// LLM-HARNESS-BEGIN: us-80496e2c
extern "C" void func_80492DB8() {}
// LLM-HARNESS-END: us-80496e2c

// LLM-HARNESS-BEGIN: us-80496e38
extern "C" void func_80492DC4() {}
// LLM-HARNESS-END: us-80496e38

// LLM-HARNESS-BEGIN: us-80496e48
extern "C" void func_80492DD4() {}
// LLM-HARNESS-END: us-80496e48

// LLM-HARNESS-BEGIN: us-80496e54
extern "C" void func_80492DE0() {}
// LLM-HARNESS-END: us-80496e54

// LLM-HARNESS-BEGIN: us-80496e60
extern "C" void func_80492DEC() {}
// LLM-HARNESS-END: us-80496e60

// LLM-HARNESS-BEGIN: us-80496e70
extern "C" void func_80492DFC() {}
// LLM-HARNESS-END: us-80496e70

// LLM-HARNESS-BEGIN: us-80496e7c
extern "C" void func_80492E08() {}
// LLM-HARNESS-END: us-80496e7c

// LLM-HARNESS-BEGIN: us-80497130
extern "C" void func_804930BC() {}
// LLM-HARNESS-END: us-80497130

// LLM-HARNESS-BEGIN: us-804971b4
extern "C" void func_80493140(void* self, float val) { *(float*)((u8*)self + 0x14) = val; }
// LLM-HARNESS-END: us-804971b4

// LLM-HARNESS-BEGIN: us-804971bc
extern "C" void func_80493148() {}
// LLM-HARNESS-END: us-804971bc

// LLM-HARNESS-BEGIN: us-80497210
extern "C" void func_8049319C() {}
// LLM-HARNESS-END: us-80497210

// LLM-HARNESS-BEGIN: us-80497284
extern "C" void func_80493210() {}
// LLM-HARNESS-END: us-80497284

// LLM-HARNESS-BEGIN: us-804972cc
extern "C" void func_80493258() {}
// LLM-HARNESS-END: us-804972cc

// LLM-HARNESS-BEGIN: us-80497328
extern "C" void* func_804932B4(void* self) { return self; }
// LLM-HARNESS-END: us-80497328

// LLM-HARNESS-BEGIN: us-80497330
extern "C" void func_804932BC() {}
// LLM-HARNESS-END: us-80497330

// LLM-HARNESS-BEGIN: us-80497374
extern "C" void func_80493300() {}
// LLM-HARNESS-END: us-80497374

// LLM-HARNESS-BEGIN: us-80497420
extern "C" void func_804933AC() {}
// LLM-HARNESS-END: us-80497420

// LLM-HARNESS-BEGIN: us-804974f0
extern "C" void func_8049347C() {}
// LLM-HARNESS-END: us-804974f0

// LLM-HARNESS-BEGIN: us-804975e8
extern "C" u32 func_80493574(void* self) { return *(u32*)((u8*)self + 0x2c); }
// LLM-HARNESS-END: us-804975e8

// LLM-HARNESS-BEGIN: us-804975f0
extern "C" void func_8049357C() {}
// LLM-HARNESS-END: us-804975f0

// LLM-HARNESS-BEGIN: us-804975fc
extern "C" void func_80493588() {}
// LLM-HARNESS-END: us-804975fc

// LLM-HARNESS-BEGIN: us-80497608
extern "C" void func_80493594() {}
// LLM-HARNESS-END: us-80497608

// LLM-HARNESS-BEGIN: us-80497618
extern "C" void func_804935A4() {}
// LLM-HARNESS-END: us-80497618

// LLM-HARNESS-BEGIN: us-80497634
extern "C" void func_804935C0() {}
// LLM-HARNESS-END: us-80497634

// LLM-HARNESS-BEGIN: us-80497720
extern "C" void func_804936AC() {}
// LLM-HARNESS-END: us-80497720

// LLM-HARNESS-BEGIN: us-80497bec
extern void func_8049B158();
extern "C" void func_80493B78(void* self) { func_8049B158(); }
// LLM-HARNESS-END: us-80497bec

// LLM-HARNESS-BEGIN: us-80497bf4
extern "C" u32 func_80493B80(void* self) { return *(u32*)((u8*)self + 0x68); }
// LLM-HARNESS-END: us-80497bf4

// LLM-HARNESS-BEGIN: us-80497bfc
extern "C" void func_80493B88() {}
// LLM-HARNESS-END: us-80497bfc

// LLM-HARNESS-BEGIN: us-80497c0c
extern "C" void func_80493B98() {}
// LLM-HARNESS-END: us-80497c0c

// LLM-HARNESS-BEGIN: us-80497c1c
extern "C" void func_80493BA8() {}
// LLM-HARNESS-END: us-80497c1c

// LLM-HARNESS-BEGIN: us-80497c2c
extern "C" void func_80493BB8() {}
// LLM-HARNESS-END: us-80497c2c

extern "C" void func_8004B3D8(void* self);
// LLM-HARNESS-BEGIN: us-80497c38
extern "C" void func_80493BC4(void* self) { ((void(*)(void*))func_8004B3D8)((char*)self + 0x30); }
// LLM-HARNESS-END: us-80497c38

// LLM-HARNESS-BEGIN: us-80497c40
extern "C" void func_80493BCC() {}
// LLM-HARNESS-END: us-80497c40

// LLM-HARNESS-BEGIN: us-80497c74
extern "C" void* func_80493C00(void* self) { return (void*)((u8*)self + 0x4); }
// LLM-HARNESS-END: us-80497c74

// LLM-HARNESS-BEGIN: us-80497c7c
extern "C" void func_80493C08() {}
// LLM-HARNESS-END: us-80497c7c

// LLM-HARNESS-BEGIN: us-80497c8c
extern "C" float func_80493C18(void* self) { return *(float*)((u8*)self + 0x3c); }
// LLM-HARNESS-END: us-80497c8c

// LLM-HARNESS-BEGIN: us-80497c94
extern "C" void func_80493C20() {}
// LLM-HARNESS-END: us-80497c94

// LLM-HARNESS-BEGIN: us-80497ca4
extern "C" void func_80493C30() {}
// LLM-HARNESS-END: us-80497ca4

// LLM-HARNESS-BEGIN: us-80497f34
extern "C" float func_80493EC0(void* self) { return *(float*)((u8*)self + 0xd4); }
// LLM-HARNESS-END: us-80497f34

// LLM-HARNESS-BEGIN: us-80497f3c
extern "C" u8 func_80493EC8(void* self) { return ((u8*)self)[0xd0]; }
// LLM-HARNESS-END: us-80497f3c

// LLM-HARNESS-BEGIN: us-80497f44
extern "C" void* func_80493ED0(void* self) { return (void*)((u8*)self + 0x14); }
// LLM-HARNESS-END: us-80497f44

// LLM-HARNESS-BEGIN: us-80497f4c
extern "C" u32 func_80493ED8(void* self) { return *(u32*)((u8*)self + 0x8); }
// LLM-HARNESS-END: us-80497f4c

// LLM-HARNESS-BEGIN: us-80497f54
extern "C" u32 func_80493EE0(void* self) { return *(u32*)((u8*)self + 0x64); }
// LLM-HARNESS-END: us-80497f54

// LLM-HARNESS-BEGIN: us-80497f5c
extern "C" void func_80493EE8() {}
// LLM-HARNESS-END: us-80497f5c

// LLM-HARNESS-BEGIN: us-80497f68
extern "C" u32 func_80493EF4(void* self) { return *(u32*)((u8*)self + 0x480); }
// LLM-HARNESS-END: us-80497f68

// LLM-HARNESS-BEGIN: us-80497f70
extern "C" void func_80493EFC() {}
// LLM-HARNESS-END: us-80497f70

// LLM-HARNESS-BEGIN: us-80497f7c
extern "C" void func_80493F08() {}
// LLM-HARNESS-END: us-80497f7c

// LLM-HARNESS-BEGIN: us-80498154
extern "C" float func_804940E0(void* self) { return *(float*)((u8*)self + 0x34); }
// LLM-HARNESS-END: us-80498154

// LLM-HARNESS-BEGIN: us-8049815c
extern "C" float func_804940E8(void* self) { return *(float*)((u8*)self + 0x38); }
// LLM-HARNESS-END: us-8049815c

// LLM-HARNESS-BEGIN: us-80498164
extern "C" void* func_804940F0(void* self) { return (void*)((u8*)self + 0x34); }
// LLM-HARNESS-END: us-80498164

// LLM-HARNESS-BEGIN: us-8049816c
extern "C" float func_804940F8(void* self) { return *(float*)((u8*)self + 0x40); }
// LLM-HARNESS-END: us-8049816c

// LLM-HARNESS-BEGIN: us-80498174
extern "C" float func_80494100(void* self) { return *(float*)((u8*)self + 0x40); }
// LLM-HARNESS-END: us-80498174

// LLM-HARNESS-BEGIN: us-8049817c
extern "C" float func_80494108(void* self) { return *(float*)((u8*)self + 0x44); }
// LLM-HARNESS-END: us-8049817c

// LLM-HARNESS-BEGIN: us-80498184
extern "C" void* func_80494110(void* self) { return (void*)((u8*)self + 0x34); }
// LLM-HARNESS-END: us-80498184

// LLM-HARNESS-BEGIN: us-8049818c
extern "C" float func_80494118(void* self) { return *(float*)((u8*)self + 0x4c); }
// LLM-HARNESS-END: us-8049818c

// LLM-HARNESS-BEGIN: us-80498194
extern "C" float func_80494120(void* self) { return *(float*)((u8*)self + 0x48); }
// LLM-HARNESS-END: us-80498194

// LLM-HARNESS-BEGIN: us-8049819c
extern "C" void func_80494128() {}
// LLM-HARNESS-END: us-8049819c

// LLM-HARNESS-BEGIN: us-804981fc
extern "C" void func_80494188() {}
// LLM-HARNESS-END: us-804981fc

// LLM-HARNESS-BEGIN: us-8049827c
extern "C" void func_80494208() {}
// LLM-HARNESS-END: us-8049827c

// LLM-HARNESS-BEGIN: us-80498330
extern "C" void func_804942BC() {}
// LLM-HARNESS-END: us-80498330

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// LLM-HARNESS-BEGIN: us-80498374
extern "C" void sinit_80494300() {}
// LLM-HARNESS-END: us-80498374
