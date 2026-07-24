// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_anmchr
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "libs/nw4r/src/g3d/g3d_anmchr.cpp" line 4 "harness_catalog.h" */
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

// LLM-HARNESS-BEGIN: us-803e6178
extern "C" void Release__Q34nw4r3g3d9AnmObjChrFv() {}
// LLM-HARNESS-END: us-803e6178

// LLM-HARNESS-BEGIN: us-803e61b4
extern "C" int Attach__Q34nw4r3g3d9AnmObjChrFiPQ34nw4r3g3d12AnmObjChrRes(void) { return 0x0; }
// LLM-HARNESS-END: us-803e61b4

// LLM-HARNESS-BEGIN: us-803e61bc
extern "C" int Detach__Q34nw4r3g3d9AnmObjChrFi(void) { return 0x0; }
// LLM-HARNESS-END: us-803e61bc

// LLM-HARNESS-BEGIN: us-803e61c4
extern "C" void SetWeight__Q34nw4r3g3d9AnmObjChrFif(void) {}
// LLM-HARNESS-END: us-803e61c4

// LLM-HARNESS-BEGIN: us-803e61c8
extern "C" bool GetWeight__Q34nw4r3g3d9AnmObjChrCFi() { return false; }
// LLM-HARNESS-END: us-803e61c8

// LLM-HARNESS-BEGIN: us-803e61d0
extern "C" void DetachAll__Q34nw4r3g3d9AnmObjChrFv(void) {}
// LLM-HARNESS-END: us-803e61d0

// LLM-HARNESS-BEGIN: us-803e61d4
extern "C" void __dt__Q34nw4r3g3d9AnmObjChrFv() {}
// LLM-HARNESS-END: us-803e61d4

// LLM-HARNESS-BEGIN: us-803e6214
extern "C" void __dt__Q34nw4r3g3d13AnmObjChrNodeFv() {}
// LLM-HARNESS-END: us-803e6214

// LLM-HARNESS-BEGIN: us-803e6278
extern "C" void Attach__Q34nw4r3g3d13AnmObjChrNodeFiPQ34nw4r3g3d12AnmObjChrRes() {}
// LLM-HARNESS-END: us-803e6278

// LLM-HARNESS-BEGIN: us-803e6364
extern "C" void Detach__Q34nw4r3g3d13AnmObjChrNodeFi() {}
// LLM-HARNESS-END: us-803e6364

// LLM-HARNESS-BEGIN: us-803e647c
extern "C" void DetachAll__Q34nw4r3g3d13AnmObjChrNodeFv() {}
// LLM-HARNESS-END: us-803e647c

// LLM-HARNESS-BEGIN: us-803e64dc
extern "C" void UpdateFrame__Q34nw4r3g3d13AnmObjChrNodeFv() {}
// LLM-HARNESS-END: us-803e64dc

// LLM-HARNESS-BEGIN: us-803e6554
extern "C" void SetFrame__Q34nw4r3g3d13AnmObjChrNodeFf() {}
// LLM-HARNESS-END: us-803e6554

// LLM-HARNESS-BEGIN: us-803e65dc
extern "C" void GetFrame__Q34nw4r3g3d13AnmObjChrNodeCFv() {}
// LLM-HARNESS-END: us-803e65dc

// LLM-HARNESS-BEGIN: us-803e6630
extern "C" void SetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeFf() {}
// LLM-HARNESS-END: us-803e6630

// LLM-HARNESS-BEGIN: us-803e66b8
extern "C" void GetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeCFv() {}
// LLM-HARNESS-END: us-803e66b8

// LLM-HARNESS-BEGIN: us-803e670c
extern "C" void Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdl() {}
// LLM-HARNESS-END: us-803e670c

// LLM-HARNESS-BEGIN: us-803e67fc
extern "C" void Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption() {}
// LLM-HARNESS-END: us-803e67fc

// LLM-HARNESS-BEGIN: us-803e68fc
extern "C" void Release__Q34nw4r3g3d13AnmObjChrNodeFv() {}
// LLM-HARNESS-END: us-803e68fc

// LLM-HARNESS-BEGIN: us-803e69b4
extern "C" void Release__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption() {}
// LLM-HARNESS-END: us-803e69b4

// LLM-HARNESS-BEGIN: us-803e6af0
extern "C" void G3dProc__Q34nw4r3g3d13AnmObjChrNodeFUlUlPv() {}
// LLM-HARNESS-END: us-803e6af0

// LLM-HARNESS-BEGIN: us-803e6b80
extern "C" void Construct__Q34nw4r3g3d14AnmObjChrBlendFP12MEMAllocatorPUlQ34nw4r3g3d6ResMdli() {}
// LLM-HARNESS-END: us-803e6b80

// LLM-HARNESS-BEGIN: us-803e6cf4
extern "C" void GetResult__Q34nw4r3g3d14AnmObjChrBlendFPQ34nw4r3g3d12ChrAnmResultUl() {}
// LLM-HARNESS-END: us-803e6cf4

// LLM-HARNESS-BEGIN: us-803e7334
extern "C" void SetWeight__Q34nw4r3g3d14AnmObjChrBlendFif() {}
// LLM-HARNESS-END: us-803e7334

// LLM-HARNESS-BEGIN: us-803e7344
extern "C" void GetWeight__Q34nw4r3g3d14AnmObjChrBlendCFi() {}
// LLM-HARNESS-END: us-803e7344

// LLM-HARNESS-BEGIN: us-803e7354
extern "C" void Construct__Q34nw4r3g3d12AnmObjChrResFP12MEMAllocatorPUlQ34nw4r3g3d9ResAnmChrQ34nw4r3g3d6ResMdlb() {}
// LLM-HARNESS-END: us-803e7354

// LLM-HARNESS-BEGIN: us-803e7544
extern "C" void SetFrame__Q34nw4r3g3d12AnmObjChrResFf() {}
// LLM-HARNESS-END: us-803e7544

// LLM-HARNESS-BEGIN: us-803e7604
extern "C" float GetFrame__Q34nw4r3g3d12AnmObjChrResCFv(void *self) { return *(float *)((char *)self + 0x10); }
// LLM-HARNESS-END: us-803e7604

// LLM-HARNESS-BEGIN: us-803e760c
extern "C" void SetUpdateRate__Q34nw4r3g3d12AnmObjChrResFf() {}
// LLM-HARNESS-END: us-803e760c

// LLM-HARNESS-BEGIN: us-803e76c0
extern "C" float GetUpdateRate__Q34nw4r3g3d12AnmObjChrResCFv(void *self) { return *(float *)((char *)self + 0x10); }
// LLM-HARNESS-END: us-803e76c0

// LLM-HARNESS-BEGIN: us-803e76c8
extern "C" void UpdateFrame__Q34nw4r3g3d12AnmObjChrResFv() {}
// LLM-HARNESS-END: us-803e76c8

// LLM-HARNESS-BEGIN: us-803e77a8
extern "C" void Bind__Q34nw4r3g3d12AnmObjChrResFQ34nw4r3g3d6ResMdl() {}
// LLM-HARNESS-END: us-803e77a8

// LLM-HARNESS-BEGIN: us-803e78ac
extern "C" void Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption() {}
// LLM-HARNESS-END: us-803e78ac

// LLM-HARNESS-BEGIN: us-803e7aa4
extern "C" void Release__Q34nw4r3g3d12AnmObjChrResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption() {}
// LLM-HARNESS-END: us-803e7aa4

// LLM-HARNESS-BEGIN: us-803e7c70
extern "C" void GetResult__Q34nw4r3g3d12AnmObjChrResFPQ34nw4r3g3d12ChrAnmResultUl() {}
// LLM-HARNESS-END: us-803e7c70

// LLM-HARNESS-BEGIN: us-803e7ce4
extern "C" void G3dProc__Q34nw4r3g3d12AnmObjChrResFUlUlPv() {}
// LLM-HARNESS-END: us-803e7ce4

// LLM-HARNESS-BEGIN: us-803e7d38
extern "C" void IsDerivedFrom__Q34nw4r3g3d12AnmObjChrResCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803e7d38

// LLM-HARNESS-BEGIN: us-803e7da0
extern "C" void IsDerivedFrom__Q34nw4r3g3d9AnmObjChrCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803e7da0

// LLM-HARNESS-BEGIN: us-803e7df0
extern "C" void GetTypeName__Q34nw4r3g3d12AnmObjChrResCFv() {}
// LLM-HARNESS-END: us-803e7df0

// LLM-HARNESS-BEGIN: us-803e7e20
extern "C" void* GetTypeObj__Q34nw4r3g3d12AnmObjChrResCFv(void) { return 0; }
// LLM-HARNESS-END: us-803e7e20

// LLM-HARNESS-BEGIN: us-803e7e2c
extern "C" void __dt__Q34nw4r3g3d12AnmObjChrResFv() {}
// LLM-HARNESS-END: us-803e7e2c

// LLM-HARNESS-BEGIN: us-803e7e70
extern "C" void IsDerivedFrom__Q34nw4r3g3d14AnmObjChrBlendCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803e7e70

// LLM-HARNESS-BEGIN: us-803e7eec
extern "C" void IsDerivedFrom__Q34nw4r3g3d13AnmObjChrNodeCFQ44nw4r3g3d6G3dObj7TypeObj() {}
// LLM-HARNESS-END: us-803e7eec

// LLM-HARNESS-BEGIN: us-803e7f54
extern "C" void GetTypeName__Q34nw4r3g3d14AnmObjChrBlendCFv() {}
// LLM-HARNESS-END: us-803e7f54

// LLM-HARNESS-BEGIN: us-803e7f84
extern "C" void* GetTypeObj__Q34nw4r3g3d14AnmObjChrBlendCFv(void) { return 0; }
// LLM-HARNESS-END: us-803e7f84

// LLM-HARNESS-BEGIN: us-803e7f90
extern "C" void __dt__Q34nw4r3g3d14AnmObjChrBlendFv() {}
// LLM-HARNESS-END: us-803e7f90

// LLM-HARNESS-BEGIN: us-803e7ff8
extern "C" void GetTypeName__Q34nw4r3g3d13AnmObjChrNodeCFv() {}
// LLM-HARNESS-END: us-803e7ff8

// LLM-HARNESS-BEGIN: us-803e8028
extern "C" void* GetTypeObj__Q34nw4r3g3d13AnmObjChrNodeCFv(void) { return 0; }
// LLM-HARNESS-END: us-803e8028

// LLM-HARNESS-BEGIN: us-803e8034
extern "C" void GetTypeName__Q34nw4r3g3d9AnmObjChrCFv() {}
// LLM-HARNESS-END: us-803e8034

// LLM-HARNESS-BEGIN: us-803e8064
extern "C" void* GetTypeObj__Q34nw4r3g3d9AnmObjChrCFv(void) { return 0; }
// LLM-HARNESS-END: us-803e8064
