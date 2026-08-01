// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf
// Replace stubs with high-level C/C++ during decomp.

/* "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_mpvf.c" line 3 "harness_catalog.h" */
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
    };

    class bad_exception : public exception {
    public:
        bad_exception(){}
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

extern u32 lbl_eu_80619B20[];
void* memset(void* dst, int val, size_t n);
void* memcpy(void* dst, const void* src, size_t n);

void SFMPVF_InitPool(void) {
    u8* p = (u8*)lbl_eu_80619B20;
    memset(&lbl_eu_80619B20[0], 0, 0x24);
    memset(p + 0x28, 0, 8);
    memset(p + 0x30, 0, 0x40);
}

void SFD_SetMpvParaTbl(u32* src, u32* strides, u32* tbl) {
    u32* g = lbl_eu_80619B20;
    u32* p = g;
    u32* q = g + 0xA;
    u32* r = g + 0xC;
    s32 i = 0;
    s32 j, k;

    *(u64*)&p[0] = *(u64*)&src[0];
    *(u64*)&p[2] = *(u64*)&src[2];
    *(u64*)&p[4] = *(u64*)&src[4];
    *(u64*)&p[6] = *(u64*)&src[6];
    p[4] = 0;
    p[8] = 0;
    q[0] = (strides[0] + 0x1F) & ~0x1F;
    q[1] = (strides[1] + 0x1F) & ~0x1F;

    for (j = 0; j < 4; j++) {
        for (k = 0; k < 4; k++) {
            if (i < src[7]) {
                r[j * 4 + k] = (tbl[k] + 0x1F) & ~0x1F;
            } else {
                r[j * 4 + k] = 0;
            }
            i++;
        }
        tbl += 4;
    }
}

s32 sfmpvf_CheckMpvPara(void* self) {
    u32* p;
    u32* g = (u32*)lbl_eu_80619B20;
    u32* q;
    u32* r;
    s32 width;
    s32 i;
    p = g;
    width = (s32)p[0x1C / 4];
    if ((u32)(width - 1) > 0xF) {
        return -1;
    }
    if (p[0x10 / 4] == 0 || p[0x20 / 4] == 0) {
        q = g + 0xA;
        if (q[0] == 0) {
            return -1;
        }
        if (q[1] == 0) {
            return -1;
        }
        r = g + 0xC;
        for (i = 0; i < width; i++, r++) {
            if (*r == 0) {
                return -1;
            }
        }
    }
    return 0;
}

s32 SFLIB_SetErr(void* h, u32 err_code);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern void* SFMPVF_HoldFrm(void* self);

s32 SFMPVF_ReadGlobalMpvPara(void* self) {
    u64* s = (u64*)lbl_eu_80619B20;
    u32* g = (u32*)lbl_eu_80619B20;

    if (sfmpvf_CheckMpvPara(self) != 0) {
        return SFLIB_SetErr(0, 0xff000f15);
    }
    *(u64*)((u8*)self + 0x2780) = s[0];
    *(u64*)((u8*)self + 0x2788) = s[1];
    *(u64*)((u8*)self + 0x2790) = s[2];
    *(u64*)((u8*)self + 0x2798) = s[3];
    *(u32*)((u8*)self + 0x27a0) = g[8];
    memcpy((u8*)self + 0x27a4, g + 0xa, 8);
    memcpy((u8*)self + 0x27ac, g + 0xc, 0x40);
    return 0;
}

s32 SFMPVF_WriteGlobalMpvPara(void* self) {
    u64* d = (u64*)lbl_eu_80619B20;
    u32* g = (u32*)lbl_eu_80619B20;

    d[0] = *(u64*)((u8*)self + 0x2780);
    d[1] = *(u64*)((u8*)self + 0x2788);
    d[2] = *(u64*)((u8*)self + 0x2790);
    d[3] = *(u64*)((u8*)self + 0x2798);
    g[8] = *(u32*)((u8*)self + 0x27a0);
    memcpy(g + 0xa, (u8*)self + 0x27a4, 8);
    memcpy(g + 0xc, (u8*)self + 0x27ac, 0x40);
    g[0x1c] = *(u32*)((u8*)self + 0x38f8);
    g[0x1d] = *(u32*)((u8*)self + 0x38fc);
    g[0x1e] = *(u32*)((u8*)self + 0x3900);
}

void SFD_CalcYccPlane(s32 base, s32 w, s32 h, void* out) {
    s32 wa = (w + 15) / 16;
    s32 h15 = h + 15;
    *(s32*)((u8*)out + 8) = base;
    s32 w16 = wa * 16;
    s32 w32 = (w16 + 31) / 32;
    s32 t = (((((wa >> 27) & 1) + w16) >> 1) + 31) / 32;
    *(s16*)((u8*)out + 14) = (s16)(w32 * 32);
    s32 ha = h15 / 16;
    *(s16*)((u8*)out + 12) = (s16)(t * 32);
    s32 h16 = ha * 16;
    s32 u = (((ha >> 27) & 1) + h16) >> 1;
    *(s32*)((u8*)out + 0) = base + h16 * (w32 * 32);
    *(s32*)((u8*)out + 4) = base + h16 * (w32 * 32) + u * (t * 32);
}

s32 sfmpvf_SetPicUsrBuf(void* self, u32 w, u32 h, u32 stride);
s32 SFLIB_CheckHn(void* h);

s32 SFD_SetPicUsrBuf(void* sfd, void* buf, s32 height, s32 count) {
    if (SFLIB_CheckHn(sfd) != 0) {
        return SFLIB_SetErr(0, 0xff000185);
    }
    return sfmpvf_SetPicUsrBuf(sfd, (u32)buf, height, count);
}

void SFMPVF_ResetPicUsrBuf(void* self) {
    sfmpvf_SetPicUsrBuf(self, lbl_eu_80619B20[0x1c], lbl_eu_80619B20[0x1d], lbl_eu_80619B20[0x1e]);
}

s32 sfmpvf_SetPicUsrBuf(void* self, u32 w, u32 h, u32 stride) {
    u32* p = (u32*)((u8*)self + 0x38f8);
    u32 n;
    u32 lim;
    u32 i;
    u32 cur;

    if (w == 0 || h == 0 || stride == 0) {
        for (i = 0; i < 0x94 / 4; i++) {
            p[i] = 0;
        }
        return 0;
    }
    if ((s32)h < *(s32*)((u8*)self + 0x30) + 3) {
        return SFLIB_SetErr(self, 0xff000f1d);
    }
    p[0] = w;
    p[1] = h;
    p[2] = stride;
    p[3] = w;
    p[4] = 0;
    cur = w + stride;
    n = h - 1;
    lim = (n >= 16) ? 16 : n;
    for (i = 0; i < lim; i++) {
        p[5 + 2 * i] = cur;
        p[6 + 2 * i] = 0;
        cur += stride;
    }
    return 0;
}

void SFMPVF_InitPicUsr(void* self) {
    u32* w = (u32*)self;
    u32 i;
    for (i = 0; i < 0x94 / 4; i++) {
        w[i] = 0;
    }
}

extern u32 SFSET_GetCond(void* self, u32 idx);
extern s32 SFTIM_IsGetFrmTime(void* self, void* frm);
extern s32 SFTIM_ExecCyclicFrameOutput(void* self);

s32 SFMPVF_GetRead(void* self, void** ppRead, u32* pFrmCnt, void (*callback)(void*, void*, void*)) {
    void* frm;
    void* entry;
    s32 i;
    u32 cnt = *(u32*)((u8*)self + 0x27ec);
    u8* base = (u8*)self + 0x27f8;

    if (SFSET_GetCond(self, 0xf) == 0 && *(s32*)((u8*)self + 0x5c) == 1) {
        *ppRead = NULL;
        return 0;
    }

    frm = SFMPVF_HoldFrm(self);
    if (frm == NULL) {
        *ppRead = NULL;
        return 0;
    }

    /* Search for the held frame's entry index */
    entry = NULL;
    for (i = 0; i < (s32)cnt; i++) {
        void* e = base + i * 0x110;
        if (e == frm) {
            entry = (u8*)self + 0x1758 + i * 0x88;
            break;
        }
    }

    *(u32*)entry = 1;
    *(void**)((u8*)self + 0x27f4) = frm;
    *ppRead = (u8*)entry + 8;

    /* Call the read callback */
    callback(self, frm, *ppRead);

    /* Copy frame info */
    *(u32*)((u8*)self + 0x1018) = *(u32*)((u8*)*ppRead + 0x14);
    *(u32*)((u8*)self + 0x101c) = *(u32*)((u8*)*ppRead + 0x18);

    if (SFTIM_IsGetFrmTime(self, *ppRead) == 0) {
        *ppRead = NULL;
        return 0;
    }

    if (SFTIM_ExecCyclicFrameOutput(self) == 0) {
        *ppRead = NULL;
        return 0;
    }

    if (*(s32*)((u8*)self + 0x68) == 2) {
        s32 frameCnt = *(s32*)((u8*)self + 0x6c);
        u32 newCnt = frameCnt + 1;
        *(u32*)((u8*)self + 0x6c) = newCnt;
        if ((s32)newCnt < 0) {
            *(u32*)((u8*)self + 0x6c) = 0;
        }
        *(u32*)((u8*)frm + 0x64) = frameCnt;
        *pFrmCnt = frameCnt;
    }
    return 0;
}

void SFMPVF_AddRead() {}

void SFMPVF_TermDec(void* self) { *(u32*)((u8*)self + 0x27F0) = 1; }

u32 SFMPVF_IsTermDec(void* self) { return *(u32*)((u8*)self + 0x27f0); }

void SFMPVF_SetGopStat(void* self) {
    s32* p = (s32*)((u8*)self + 0x27f8);
    s32 cnt = *(s32*)((u8*)self + 0x27ec);
    while (cnt-- > 0) {
        if (*p == 2 || *p == 4)
            *(p + 0x3F) = 1;
        p += 0x44;
    }
}

s32 SFMPVF_GetNumFrm(void* self) {
    void* cs;
    s32 n;
    s32* p;
    s32 cnt;
    SFLIB_LockCs(&cs);
    n = *(s32*)((u8*)self + 0x27ec);
    p = (s32*)((u8*)self + 0x27f8);
    cnt = 0;
    while (n-- > 0) {
        if ((*p == 2 || *p == 4) && *(p + 0x19) == -1) {
            cnt++;
        }
        p += 0x44;
    }
    if (*(s32*)((u8*)self + 0x27f0) == 1 && cnt == 0) {
        cnt = -1;
    }
    SFLIB_UnlockCs(&cs);
    return cnt;
}

void* SFMPVF_AllocFrm(void* self) {
    void* cs;
    s32* p;
    s32 i;
    s32 n;
    SFLIB_LockCs(&cs);
    n = *(s32*)((u8*)self + 0x27ec);
    p = (s32*)((u8*)self + 0x27f8);
    for (i = 0; i < n; i++, p += 0x44) {
        if (p[0] == 0 && p[1] == 0) {
            p[0] = 1;
            break;
        }
    }
    if (i == n) {
        p = NULL;
    }
    SFLIB_UnlockCs(&cs);
    return p;
}

void SFMPVF_FreeFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 0;
    }
}

void SFMPVF_StbyFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 2;
    }
}

void SFMPVF_RefStbyFrm(void* self) {
    if (self != NULL) {
        *(u32*)((u8*)self + 0x00) = 4;
    }
}

void SFMPVF_EndRefFrm(void* self) {
    if (self == NULL) return;
    if (*(u32*)self == 4) {
        *(u32*)self = 2;
        return;
    }
    *(u32*)self = 0;
}

s32 sfmpvf_IsChkFirst(void* a, void* b);

void* SFMPVF_HoldFrm(void* self) {
    void* cs;
    s32 i;
    s32 cnt;
    u8* p;
    void* best;
    s32 count;
    SFLIB_LockCs(&cs);
    count = *(s32*)((u8*)self + 0x27ec);
    p = (u8*)self + 0x27f8;
    best = NULL;
    cnt = 0;
    for (i = 0; i < count; i++, p += 0x110) {
        s32 st = *(s32*)p;
        if ((st == 2 || st == 4) && *(s32*)(p + 0x64) == -1) {
            cnt++;
            if (sfmpvf_IsChkFirst(best, p)) {
                best = p;
            }
        }
    }
    if (cnt == 1 && *(s32*)((u8*)self + 0x27f0) == 0 && *(s32*)((u8*)best + 0xfc) == 0) {
        best = NULL;
    }
    SFLIB_UnlockCs(&cs);
    return best;
}

void sfmpvf_SearchStbyFrm(void* h, void** outA, void** outB);
u32 SFSET_GetCond(void* self, u32 idx);
s32 SFTIM_IsGetFrmTimeTunit(void* self, s32 cmpA, s32 cmpB);

s32 SFD_IsNextFrmReady(void* h) {
    void* cs;
    void* frm;
    void* dummy;
    if (SFLIB_CheckHn(h) != 0) {
        SFLIB_SetErr(0, 0xff000183);
        return 0;
    }
    SFLIB_LockCs(&cs);
    if (*(s32*)((u8*)h + 0x54) != 4) {
        frm = NULL;
    } else {
        sfmpvf_SearchStbyFrm(h, &dummy, &frm);
        if (frm != NULL && SFSET_GetCond(h, 0xf) != 0 &&
            SFTIM_IsGetFrmTimeTunit(h, *(u32*)((u8*)frm + 0x44), *(u32*)((u8*)frm + 0x48)) == 0) {
            frm = NULL;
        }
    }
    SFLIB_UnlockCs(&cs);
    return frm != NULL;
}

void sfmpvf_SearchStbyFrm(void* h, void** outA, void** outB) {
    u8* p;
    s32 count;
    s32 cnt;
    s32 i;
    *outA = NULL;
    *outB = NULL;
    count = *(s32*)((u8*)h + 0x27ec);
    p = (u8*)h + 0x27f8;
    cnt = 0;
    for (i = 0; i < count; i++, p += 0x110) {
        s32 st = *(s32*)p;
        if ((st == 2 || st == 4) && *(s32*)(p + 0x64) == -1) {
            cnt++;
            if (sfmpvf_IsChkFirst(*outA, p)) {
                *outB = *outA;
                *outA = p;
            } else if (sfmpvf_IsChkFirst(*outB, p)) {
                *outB = p;
            }
        }
    }
    if (*(s32*)((u8*)h + 0x27f0) == 0) {
        cnt--;
    }
    if (cnt <= 0) {
        *outA = NULL;
        *outB = NULL;
    } else if (cnt == 1) {
        *outB = NULL;
    }
}

s32 sfmpvf_IsChkFirst(void* a, void* b) {
    if (a == NULL) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0x54) < *(s32*)((u8*)a + 0x54)) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0x54) > *(s32*)((u8*)a + 0x54)) {
        return 0;
    }
    if (*(s32*)((u8*)b + 0xF0) < *(s32*)((u8*)a + 0xF0)) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0xF0) > *(s32*)((u8*)a + 0xF0)) {
        return 0;
    }
    if (*(s32*)((u8*)b + 0xF4) < *(s32*)((u8*)a + 0xF4)) {
        return 1;
    }
    if (*(s32*)((u8*)b + 0xF4) > *(s32*)((u8*)a + 0xF4)) {
        return 0;
    }
    {
        s32 x = *(s32*)((u8*)a + 0xF8) ^ *(s32*)((u8*)b + 0xF8);
        s32 r = (x >> 1) - (x & *(s32*)((u8*)a + 0xF8));
        return (u32)r >> 31;
    }
}

s32 SFMPVF_ChkImageSize(void* self, u32 w, u32 h) {
    s32 cw = *(s32*)((u8*)self + 0x91c);
    if (cw > 0 && (cw != (s32)w || *(s32*)((u8*)self + 0x920) != (s32)h)) {
        return -1;
    }
    {
        s32 (*cb)(void*) = *(s32(**)(void*))((u8*)self + 0xd7c);
        void* arg = *(void**)((u8*)self + 0xd80);
        if (cb != NULL && cb(arg) != 0) {
            return -1;
        }
    }
    return 0;
}
