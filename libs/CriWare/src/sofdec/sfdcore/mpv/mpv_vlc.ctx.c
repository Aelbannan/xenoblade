// CriWare Sofdec MPEG-2 VLC table initialization.
// All tables live in .bss and are filled at MPVVLC_Init time.
// Entries pack MPEG-2 VLC code info: (run << 8) | level-style codes for
// mb_type / mb_addr_inc / cbp / dc_size / run-level tables.

/* "libs/CriWare/src/sofdec/sfdcore/mpv/mpv_vlc.c" line 5 "harness_catalog.h" */
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

extern u16 lbl_eu_80602FF8[];
extern u8 lbl_eu_80603400[];
extern u8 lbl_eu_80603488[];
extern u8 lbl_eu_80603510[];
extern u16 lbl_eu_80603618[];
extern u16 lbl_eu_80603660[];
extern u16 lbl_eu_806036A8[];
extern u16 lbl_eu_80603728[];
extern u16 lbl_eu_80603928[];
extern u16 lbl_eu_806039A8[];
extern u16 lbl_eu_80603AA8[];
extern u16 lbl_eu_80603AE8[];
extern u16 lbl_eu_80603BE8[];
extern u8 lbl_eu_80603C28[];
extern u8 lbl_eu_80604028[];
extern u32 lbl_eu_80604428[];
extern u32 lbl_eu_8051C2B8[];

extern int UTY_MemcpyDword(u32 *dst, const u32 *src, u32 n);

int MPVVLC_IsVlcSizErr(void) { return 0x0; }

void MPVVLC_Init() {}

/* P-picture macroblock type VLC table (32 entries) */
void mpvvlc_InitMbTypePpic(void) {
    u16 *p = lbl_eu_80603660;
    int i;

    *p++ = 0x1106;
    *p++ = 0x1205;
    *p++ = 0x1a05;
    *p++ = 0x0105;
    for (i = 0; i < 4; i++)
        *p++ = 0x0803;
    for (i = 0; i < 8; i++)
        *p++ = 0x0202;
    for (i = 0; i < 16; i++)
        *p++ = 0x0a01;
}

/* B-picture macroblock type VLC table (48 entries) */
void mpvvlc_InitMbTypeBpic(void) {
    u16 *p = lbl_eu_806036A8;
    int i;

    *p++ = 0x1f00;
    *p++ = 0x1106;
    *p++ = 0x1606;
    *p++ = 0x1a06;
    *p++ = 0x1e05;
    *p++ = 0x1e05;
    *p++ = 0x0105;
    *p++ = 0x0105;
    for (i = 0; i < 4; i++)
        *p++ = 0x0804;
    for (i = 0; i < 4; i++)
        *p++ = 0x0a04;
    for (i = 0; i < 8; i++)
        *p++ = 0x0403;
    for (i = 0; i < 8; i++)
        *p++ = 0x0603;
    for (i = 0; i < 16; i++)
        *p++ = 0x0c02;
    for (i = 0; i < 16; i++)
        *p++ = 0x0e02;
}

/* Coded-block-pattern sub-table 1 (luma pattern code table) */
u16 *mpvvlc_InitCbpSub1(u16 *tbl) {
    s16 *p = (s16 *)tbl;

    p[0] = 0;
    p[1] = 0;
    p[2] = -0x18f7;
    p[3] = -0x24f7;
    p[4] = -0x04f7;
    p[5] = -0x08f7;
    p[6] = -0x10f7;
    p[7] = -0x20f7;
    p[8] = -0x45f8;
    p[9] = -0x45f8;
    p[10] = -0x49f8;
    p[11] = -0x49f8;
    p[12] = -0x51f8;
    p[13] = -0x51f8;
    p[14] = -0x61f8;
    p[15] = -0x61f8;
    p[16] = 0x7908;
    p[17] = 0x7908;
    p[18] = 0x7508;
    p[19] = 0x7508;
    p[20] = 0x6d08;
    p[21] = 0x6d08;
    p[22] = 0x5d08;
    p[23] = 0x5d08;
    p[24] = -0x59f8;
    p[25] = -0x59f8;
    p[26] = -0x65f8;
    p[27] = -0x65f8;
    p[28] = 0x6508;
    p[29] = 0x6508;
    p[30] = 0x5908;
    p[31] = 0x5908;
    p[32] = -0x14f8;
    p[33] = -0x14f8;
    p[34] = -0x28f8;
    p[35] = -0x28f8;
    p[36] = -0x0cf8;
    p[37] = -0x0cf8;
    p[38] = -0x30f8;
    p[39] = -0x30f8;
    p[40] = -0x55f8;
    p[41] = -0x55f8;
    p[42] = -0x69f8;
    p[43] = -0x69f8;
    p[44] = -0x4df8;
    p[45] = -0x4df8;
    p[46] = -0x71f8;
    p[47] = -0x71f8;
    p[48] = 0x6908;
    p[49] = 0x6908;
    p[50] = 0x5508;
    p[51] = 0x5508;
    p[52] = 0x7108;
    p[53] = 0x7108;
    p[54] = 0x4d08;
    p[55] = 0x4d08;
    p[56] = -0x1cf8;
    p[57] = -0x1cf8;
    p[58] = -0x2cf8;
    p[59] = -0x2cf8;
    p[60] = -0x34f8;
    p[61] = -0x34f8;
    p[62] = -0x38f8;
    p[63] = -0x38f8;
    return tbl + 0x40;
}


u16 *mpvvlc_InitCbpSub2(u16 *tbl) {
    s16 *p = (s16 *)tbl;
    s16 *q = (s16 *)(tbl + 0x50);
    int i;

    for (i = 0; i < 4; i++) *p++ = -0x5df9;
    for (i = 0; i < 4; i++) *p++ = -0x6df9;
    for (i = 0; i < 4; i++) *p++ = -0x75f9;
    for (i = 0; i < 4; i++) *p++ = -0x79f9;
    for (i = 0; i < 4; i++) *p++ = 0x6107;
    for (i = 0; i < 4; i++) *p++ = 0x5107;
    for (i = 0; i < 4; i++) *p++ = 0x4907;
    for (i = 0; i < 4; i++) *p++ = 0x4507;
    for (i = 0; i < 8; i++) *p++ = -0x00fa;
    for (i = 0; i < 8; i++) *p++ = -0x3cfa;
    for (i = 0; i < 8; i++) *p++ = 0x2406;
    for (i = 0; i < 8; i++) *p++ = 0x1806;
    for (i = 0; i < 16; i++) *p++ = -0x41fb;

    for (i = 0; i < 16; i++) *q++ = -0x7dfb;
    for (i = 0; i < 16; i++) *q++ = 0x7d05;
    for (i = 0; i < 16; i++) *q++ = 0x4105;
    for (i = 0; i < 16; i++) *q++ = 0x3805;
    for (i = 0; i < 16; i++) *q++ = 0x3405;
    for (i = 0; i < 16; i++) *q++ = 0x2c05;
    for (i = 0; i < 16; i++) *q++ = 0x1c05;
    for (i = 0; i < 16; i++) *q++ = 0x2805;
    for (i = 0; i < 16; i++) *q++ = 0x1405;
    for (i = 0; i < 16; i++) *q++ = 0x3005;
    for (i = 0; i < 16; i++) *q++ = 0x0c05;
    for (i = 0; i < 32; i++) *q++ = 0x2004;
    for (i = 0; i < 32; i++) *q++ = 0x1004;
    for (i = 0; i < 32; i++) *q++ = 0x0804;
    for (i = 0; i < 32; i++) *q++ = 0x0404;
    for (i = 0; i < 64; i++)
        *q++ = 0x3c03;

    return (u16 *)q;
}

/* Luma DC size table */
void mpvvlc_InitDcSizY(void) {
    u8 *p = lbl_eu_80603400;
    int i;

    for (i = 0; i < 32; i++) *p++ = 0x12;
    for (i = 0; i < 32; i++) *p++ = 0x22;
    for (i = 0; i < 16; i++) *p++ = 0x03;
    for (i = 0; i < 16; i++) *p++ = 0x33;
    for (i = 0; i < 16; i++) *p++ = 0x43;
    for (i = 0; i < 8; i++) *p++ = 0x54;
    for (i = 0; i < 4; i++) *p++ = 0x65;
    for (i = 0; i < 2; i++) *p++ = 0x76;
    for (i = 0; i < 2; i++) *p++ = 0x87;
}

/* Chroma DC size table */
void mpvvlc_InitDcSizC(void) {
    u8 *p = lbl_eu_80603488;
    int i;

    for (i = 0; i < 32; i++) *p++ = 0x02;
    for (i = 0; i < 32; i++) *p++ = 0x12;
    for (i = 0; i < 32; i++) *p++ = 0x22;
    for (i = 0; i < 16; i++) *p++ = 0x33;
    for (i = 0; i < 8; i++) *p++ = 0x44;
    for (i = 0; i < 4; i++) *p++ = 0x55;
    for (i = 0; i < 2; i++) *p++ = 0x66;
    *p++ = 0x77;
    *p++ = 0x88;
}

/* Luma DC size table (2nd form) */
void mpvvlc2_InitDcSizY(void) {
    s8 *p = (s8 *)lbl_eu_80603C28;
    int i;

    for (i = 0; i < 256; i++)
        *p++ = 0x12;
    for (i = 0; i < 256; i++)
        *p++ = 0x22;
    for (i = 0; i < 128; i++)
        *p++ = 0x03;
    for (i = 0; i < 128; i++)
        *p++ = 0x33;
    for (i = 0; i < 128; i++)
        *p++ = 0x43;
    for (i = 0; i < 64; i++)
        *p++ = 0x54;
    for (i = 0; i < 32; i++)
        *p++ = 0x65;
    for (i = 0; i < 16; i++)
        *p++ = 0x76;
    for (i = 0; i < 8; i++)
        *p++ = -0x79;
    for (i = 0; i < 4; i++)
        *p++ = -0x68;
    *p++ = -0x57;
    *p++ = -0x57;
    *p++ = -0x47;
    *p++ = -0x47;
}

/* Chroma DC size table (2nd form) */
void mpvvlc2_InitDcSizC(void) {
    s8 *p = (s8 *)lbl_eu_80604028;
    int i;

    for (i = 0; i < 256; i++)
        *p++ = 0x02;
    for (i = 0; i < 256; i++)
        *p++ = 0x12;
    for (i = 0; i < 256; i++)
        *p++ = 0x22;
    for (i = 0; i < 128; i++)
        *p++ = 0x33;
    for (i = 0; i < 64; i++)
        *p++ = 0x44;
    for (i = 0; i < 32; i++)
        *p++ = 0x55;
    for (i = 0; i < 16; i++)
        *p++ = 0x66;
    for (i = 0; i < 8; i++)
        *p++ = 0x77;
    for (i = 0; i < 4; i++)
        *p++ = -0x78;
    *p++ = -0x67;
    *p++ = -0x67;
    *p++ = -0x56;
    *p++ = -0x46;
}

/* Intra run/level VLC table (128 dwords) */
void mpvvlc_InitIntRunLevel(void) {
    u32 *p = lbl_eu_80604428;
    int i;

    for (i = 0; i < 4; i++) *p++ = 0x00000000;
    for (i = 0; i < 4; i++) *p++ = 0x00064040;
    for (i = 0; i < 2; i++) *p++ = 0x00080202;
    for (i = 0; i < 2; i++) *p++ = 0x00080109;
    for (i = 0; i < 2; i++) *p++ = 0x00080400;
    for (i = 0; i < 2; i++) *p++ = 0x00080108;
    for (i = 0; i < 4; i++) *p++ = 0x00070107;
    for (i = 0; i < 4; i++) *p++ = 0x00070106;
    for (i = 0; i < 4; i++) *p++ = 0x00070201;
    for (i = 0; i < 4; i++) *p++ = 0x00070105;
    *p++ = 0x0009010d;
    *p++ = 0x00090600;
    *p++ = 0x0009010c;
    *p++ = 0x0009010b;
    *p++ = 0x00090203;
    *p++ = 0x00090301;
    *p++ = 0x00090500;
    *p++ = 0x0009010a;
    for (i = 0; i < 8; i++) *p++ = 0x00060300;
    for (i = 0; i < 8; i++) *p++ = 0x00060104;
    for (i = 0; i < 8; i++) *p++ = 0x00060103;
    for (i = 0; i < 16; i++) *p++ = 0x00050200;
    for (i = 0; i < 16; i++) *p++ = 0x00050102;
    for (i = 0; i < 32; i++) *p++ = 0x00040101;
}

/* I-picture macroblock address increment VLC tables */
void mpvvlc_InitMbaiIpic(void) {
    s16 *p = (s16 *)lbl_eu_80603728;
    s16 *q = (s16 *)lbl_eu_80603928;
    int v, i, j;

    for (i = 0; i < 16; i++)
        *p++ = 0x0240;
    *p++ = 0x023b;
    *p++ = 0x023b;
    for (i = 0; i < 12; i++)
        *p++ = 0x0240;
    *p++ = 0x022b;
    *p++ = 0x022b;
    for (i = 0; i < 16; i++)
        *p++ = 0x0240;

    v = 0x210;
    i = 2;
    while (i--) {
        for (j = 0; j < 6; j++) {
            *p++ = v | 0x440d;
            *p++ = v | 0x040c;
            v -= 0x10;
        }
    }
    for (i = 0; i < 6; i++) {
        *p++ = v | 0x440c;
        *p++ = v | 0x440c;
        *p++ = v | 0x040b;
        *p++ = v | 0x040b;
        v -= 0x10;
    }
    i = 2;
    while (i--) {
        for (j = 0; j < 3; j++) {
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x440a;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            *p++ = v | 0x0409;
            v -= 0x10;
        }
    }
    i = 2;
    while (i--) {
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x4409;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        *p++ = v | 0x0408;
        v -= 0x10;
    }

    for (i = 0; i < 4; i++)
        *q++ = 0x0240;
    *q++ = 0x70 | 0x4407;
    *q++ = 0x70 | 0x0406;
    *q++ = 0x60 | 0x4407;
    *q++ = 0x60 | 0x0406;
    for (i = 0; i < 2; i++)
        *q++ = 0x50 | 0x4406;
    for (i = 0; i < 2; i++)
        *q++ = 0x50 | 0x0405;
    for (i = 0; i < 2; i++)
        *q++ = 0x40 | 0x4406;
    for (i = 0; i < 2; i++)
        *q++ = 0x40 | 0x0405;
    for (i = 0; i < 4; i++)
        *q++ = 0x30 | 0x4405;
    for (i = 0; i < 4; i++)
        *q++ = 0x30 | 0x0404;
    for (i = 0; i < 4; i++)
        *q++ = 0x20 | 0x4405;
    for (i = 0; i < 4; i++)
        *q++ = 0x20 | 0x0404;
    for (i = 0; i < 16; i++)
        *q++ = 0x10 | 0x4403;
    for (i = 0; i < 16; i++)
        *q++ = 0x10 | 0x0402;
}

/* P-picture macroblock address increment VLC tables */
void mpvvlc_InitMbaiPpic(void) {
    s16 *p = (s16 *)lbl_eu_806039A8;
    s16 *q = (s16 *)lbl_eu_80603AA8;
    int v, i, j;

    for (i = 0; i < 8; i++)
        *p++ = 0x0240;
    *p++ = 0x023b;
    for (i = 0; i < 6; i++)
        *p++ = 0x0240;
    *p++ = 0x022b;
    for (i = 0; i < 8; i++)
        *p++ = 0x0240;

    v = 0x0210;
    for (i = 0; i < 12; i++) {
        *p++ = v | 0x000b;
        v -= 0x10;
    }
    *p++ = 0x015a;
    *p++ = -0x56a5;
    *p++ = 0x014a;
    *p++ = -0x56b5;
    *p++ = 0x013a;
    *p++ = -0x56c5;
    *p++ = 0x012a;
    *p++ = -0x56d5;
    *p++ = 0x011a;
    *p++ = -0x56e5;
    *p++ = 0x010a;
    *p++ = -0x56f5;

    v = 0xf0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            *p++ = v | 0x0008;
            *p++ = v | 0xa00b;
            *p++ = v | 0x880a;
            *p++ = v | 0x880a;
            *p++ = v | 0xa809;
            *p++ = v | 0xa809;
            *p++ = v | 0xa809;
            *p++ = v | 0xa809;
            v -= 0x10;
        }
    }

    *p++ = 0x0097;
    *p++ = 0x0097;
    *p++ = -0x5f66;
    *p++ = -0x5f66;
    *p++ = -0x7767;
    *p++ = -0x7767;
    *p++ = -0x7767;
    *p++ = -0x7767;
    for (i = 0; i < 8; i++)
        *p++ = -0x5768;
    *p++ = 0x0087;
    *p++ = 0x0087;
    *p++ = -0x5f76;
    *p++ = -0x5f76;
    for (i = 0; i < 4; i++)
        *p++ = -0x7777;
    for (i = 0; i < 8; i++)
        *p++ = -0x5f78;

    *q++ = 0x0240;
    *q++ = 0x0240;
    *q++ = 0x0075;
    *q++ = 0x0065;
    *q++ = 0x0054;
    *q++ = -0x57ab;
    *q++ = 0x0044;
    *q++ = -0x57bb;
    *q++ = 0x0033;
    *q++ = -0x77cb;
    *q++ = -0x57cc;
    *q++ = -0x57cc;
    *q++ = 0x0023;
    *q++ = -0x77db;
    *q++ = -0x57dc;
    *q++ = -0x57dc;
    *q++ = 0x0011;
    *q++ = 0x0011;
    *q++ = -0x5fec;
    *q++ = -0x5fec;
    for (i = 0; i < 4; i++)
        *q++ = -0x77ed;
    for (i = 0; i < 8; i++)
        *q++ = -0x57ee;
}

/* B-picture macroblock address increment VLC tables */
void mpvvlc_InitMbaiBpic(void) {
    s16 *p = (s16 *)lbl_eu_80603AE8;
    s16 *q = (s16 *)lbl_eu_80603BE8;
    int v, i, j;

    for (i = 0; i < 8; i++)
        *p++ = 0x0240;
    *p++ = 0x023b;
    for (i = 0; i < 6; i++)
        *p++ = 0x0240;
    *p++ = 0x022b;
    for (i = 0; i < 8; i++)
        *p++ = 0x0240;

    v = 0x210;
    for (i = 0; i < 12; i++) {
        *p++ = v | 0x000b;
        v -= 0x10;
    }
    *p++ = 0x015a;
    *p++ = 0x015a;
    *p++ = 0x014a;
    *p++ = 0x014a;
    *p++ = 0x013a;
    *p++ = 0x013a;
    *p++ = 0x012a;
    *p++ = 0x012a;
    *p++ = 0x011a;
    *p++ = 0x011a;
    *p++ = 0x010a;
    *p++ = 0x010a;

    v = 0xf0;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++) {
            *p++ = v | 0x0008;
            *p++ = v | 0x0008;
            *p++ = v | 0x900b;
            *p++ = v | 0x980b;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb00a;
            *p++ = v | 0xb80a;
            *p++ = v | 0xb80a;
            v -= 0x10;
        }

    *p++ = 0x0097;
    *p++ = 0x0097;
    *p++ = -0x5f65;
    *p++ = -0x5765;
    *p++ = -0x6f66;
    *p++ = -0x6f66;
    *p++ = -0x6766;
    *p++ = -0x6766;
    for (i = 0; i < 4; i++)
        *p++ = -0x4f67;
    for (i = 0; i < 4; i++)
        *p++ = -0x4767;
    *p++ = 0x0087;
    *p++ = 0x0087;
    *p++ = -0x5f75;
    *p++ = -0x5775;
    *p++ = -0x6f76;
    *p++ = -0x6f76;
    *p++ = -0x6776;
    *p++ = -0x6776;
    for (i = 0; i < 4; i++)
        *p++ = -0x4f77;
    for (i = 0; i < 4; i++)
        *p++ = -0x4777;

    *q++ = 0x0240;
    *q++ = 0x0240;
    *q++ = 0x0075;
    *q++ = 0x0065;
    *q++ = 0x0054;
    *q++ = 0x0054;
    *q++ = 0x0044;
    *q++ = 0x0044;
    *q++ = 0x0033;
    *q++ = 0x0033;
    *q++ = -0x4fcb;
    *q++ = -0x47cb;
    *q++ = 0x0023;
    *q++ = 0x0023;
    *q++ = -0x4fdb;
    *q++ = -0x47db;
    *q++ = 0x0011;
    *q++ = 0x0011;
    *q++ = -0x5feb;
    *q++ = -0x57eb;
    *q++ = -0x6fec;
    *q++ = -0x6fec;
    *q++ = -0x67ec;
    *q++ = -0x67ec;
    for (i = 0; i < 4; i++)
        *q++ = -0x4ffd;
    for (i = 0; i < 4; i++)
        *q++ = -0x47fd;
}

/* Set a motion vector code pair: (n | base) and ((u8)(-n) | base) */
static void set_mvcode(s16 *p, int n, int base) {
    *p++ = n | base;
    *p++ = (u8)(-n) | base;
}

/* Motion vector code tables */
void mpvvlc_InitMotion(void) {
    s16 *p = (s16 *)lbl_eu_80603510;
    s16 *q = (s16 *)lbl_eu_80603618;
    int n, i;

    for (i = 0; i < 24; i++)
        *p++ = 0x7f;
    n = 0x10;
    while (n >= 0x0b) {
        set_mvcode(p, n, 0x0b00);
        p += 2;
        n--;
    }
    for (n = 0x0a; n >= 0x08; n--) {
        *p++ = n | 0x0a00;
        *p++ = n | 0x0a00;
        *p++ = (u8)(-n) | 0x0a00;
        *p++ = (u8)(-n) | 0x0a00;
    }
    for (n = 0x07; n >= 0x05; n--) {
        for (i = 0; i < 8; i++)
            *p++ = n | 0x0800;
        for (i = 0; i < 8; i++)
            *p++ = (u8)(-n) | 0x0800;
    }
    for (i = 0; i < 8; i++)
        *p++ = 0x04 | 0x0700;
    for (i = 0; i < 8; i++)
        *p++ = (u8)(-0x04) | 0x0700;
    for (i = 0; i < 8; i++)
        *p++ = 0x02 | 0x0400;
    for (i = 0; i < 8; i++)
        *p++ = (u8)(-0x02) | 0x0400;

    *q++ = 0x7f;
    *q++ = 0x7f;
    *q++ = 0x03 | 0x0500;
    *q++ = (u8)(-0x03) | 0x0500;
    *q++ = 0x02 | 0x0400;
    *q++ = 0x02 | 0x0400;
    *q++ = (u8)(-0x02) | 0x0400;
    *q++ = (u8)(-0x02) | 0x0400;
    for (i = 0; i < 4; i++)
        *q++ = 0x01 | 0x0300;
    for (i = 0; i < 4; i++)
        *q++ = (u8)(-0x01) | 0x0300;
    for (i = 0; i < 16; i++)
        *q++ = 0x00 | 0x0100;
}

/* Set VLC table default pointers in the MPV context */
void mpvvlc_SetDflPtr(void) {
    u32 *g = (u32 *)lbl_eu_80602FF8;
    u32 *d = lbl_eu_8051C2B8;

    g[0x58c] = (u32)(g + 0x1cc);
    g[0x58d] = (u32)(g + 0x24c);
    g[0x58e] = (u32)(g + 0x26c);
    g[0x58f] = (u32)(g + 0x2ac);
    g[0x590] = (u32)(g + 0x2bc);
    g[0x591] = (u32)(g + 0x2fc);
    g[0x198] = (u32)(g + 0x19a);
    g[0x1aa] = (u32)(g + 0x1ac);
    g[0x144] = (u32)(g + 0x146);
    g[0x186] = (u32)(g + 0x188);
    g[0x592] = (u32)(g + 0x000);
    g[0x100] = (u32)(g + 0x102);
    g[0x122] = (u32)(g + 0x124);
    g[0x593] = (u32)(g + 0x30c);
    g[0x594] = (u32)(g + 0x40c);
    g[0x595] = (u32)(d + 0x000);
    g[0x596] = (u32)(d + 0x008);
    g[0x597] = (u32)(d + 0x010);
    g[0x598] = (u32)(d + 0x018);
    g[0x599] = (u32)(d + 0x020);
    g[0x59a] = (u32)(d + 0x028);
    g[0x59b] = (u32)(g + 0x50c);
}

/* Install run/level VLC tables (copied from the default table image) */
u32 *mpvvlc_SetVlcRunLevel(u32 *tbl) {
    u32 *g = (u32 *)lbl_eu_80602FF8;
    u32 *d = lbl_eu_8051C2B8;

    g[0x59b] = (u32)(tbl - 0x80);
    UTY_MemcpyDword(tbl - 0x80, &g[0x50c], 0x80);
    g[0x59a] = (u32)(tbl - 0x84);
    UTY_MemcpyDword(tbl - 0x84, &d[0x28], 4);
    g[0x599] = (u32)(tbl - 0x8c);
    UTY_MemcpyDword(tbl - 0x8c, &d[0x20], 8);
    g[0x598] = (u32)(tbl - 0x94);
    UTY_MemcpyDword(tbl - 0x94, &d[0x18], 8);
    g[0x597] = (u32)(tbl - 0x9c);
    UTY_MemcpyDword(tbl - 0x9c, &d[0x10], 8);
    g[0x596] = (u32)(tbl - 0xa4);
    UTY_MemcpyDword(tbl - 0xa4, &d[0x08], 8);
    g[0x595] = (u32)(tbl - 0xac);
    UTY_MemcpyDword(tbl - 0xac, &d[0x00], 8);
    return tbl - 0xac;
}
