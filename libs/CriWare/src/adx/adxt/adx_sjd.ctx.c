// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_sjd
// Replace stubs with high-level C/C++ during decomp.

/* "libs/CriWare/src/adx/adxt/adx_sjd.c" line 3 "harness_catalog.h" */
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

/* External function declarations */
void ADXB_Init(void);
void* ADXB_Create(void* ctx, void* buf, u32 size, u32 totalSize);
void ADXB_Destroy(void* self);
void ADXB_Stop(void* self);
void ADXB_Start(void* self);
void ADXB_Reset(void* self);
void ADXB_ExecHndl(void* self);
s32 ADXB_GetStat(void* self);
s16 ADXB_GetFormat(void* self);
u32 ADXB_GetSfreq(void* self);
s32 ADXB_GetNumChan(void* self);
u32 ADXB_GetBlkSmpl(void* self);
u32 ADXB_GetTotalNumSmpl(void* self);
s32 ADXB_GetFmtBps(void* self);
u32 ADXB_GetDecDtLen(void* self);
u32 ADXB_GetDecNumSmpl(void* self);
u32 ADXB_GetOutBps(void* self);
s16 ADXB_GetNumLoop(void* self);
u32 ADXB_GetLpStartPos(void* self);
u32 ADXB_GetLpStartOfst(u32 val);
u32 ADXB_GetLpEndPos(void* self);
u32 ADXB_GetLpEndOfst(void* self);
s32 ADXB_GetAinfLen(void* self);
s32 ADXB_GetDefOutVol(void* self);
s32 ADXB_GetDefPan(void* self, s32 pan);
void ADXB_GetPcmBuf(void* self);
void ADXB_EntryData(void* self, void* data, u32 len);
void ADXB_EntryGetWrFunc(void* self, void* func, void* ctx);
void ADXB_SetLnkSw(void* self, u32 val);
void ADXB_SetDefFmt(void* self);
void ADXB_SetDefPrm(void* self);
void ADXB_SetAhxInSj(void* self);
void ADXB_SetAhxDecSmpl(void* self, u32 val);
void ADXB_AhxTermSupply(void* self);
s32 ADXB_DecodeHeader(void* self, void* data);
s32 ADXB_GetDecErrMode(void* self);
void ADXB_TakeSnapshot(void* self);
void ADXB_RestoreSnapshot(void* self);

void* SJRBF_GetBufPtr(void* self);
u32 SJRBF_GetBufSize(void* self);
u32 SJRBF_GetXtrSize(void* self);
void SJ_SplitChunk(void* src, s32 size, void* dst1, void* dst2);
s32 ADX_DecodeFooter(u8* data, s16* outLen);

void ADXCRS_Lock(void);
void ADXCRS_Unlock(void);
void ADXERR_CallErrFunc2_(void* a, void* b);
void criware_eu_8038A864(void* self);

extern volatile u32 lbl_eu_805E3340;
extern u8 lbl_eu_805E3358[0xB40];
extern u32 lbl_eu_805E3354;
extern u8 lbl_eu_805160B8[];

/* Forward declarations */
void adxsjd_decode_prep(void* self);
void adxsjd_get_wr(void* self, u32 reqLen, void* outA, void* outB);
void adxsjd_decexec_start(void* self);
void adxsjd_decexec_end(void* self);
void adxsjd_insert_proc(void* self);
void adxsjd_discard_proc(void* self);
void ADXSJD_ExecServer(void);

/* Helper: call virtual function from C-style vtable at offset 0x0 of object */
/* The SJ/ADXB objects use a "class" pointer at +0x0 with function pointers at word offsets */

void ADXSJD_Init(void) {
    if (lbl_eu_805E3340 == 0) {
        ADXB_Init();
        memset(lbl_eu_805E3358, 0, 0xB40);
    }
    lbl_eu_805E3340++;
}

void ADXSJD_Finish(void) {
    lbl_eu_805E3340--;
    if (lbl_eu_805E3340 == 0) {
        memset(lbl_eu_805E3358, 0, 0xB40);
    }
}

void* ADXSJD_Create(void* stream, s32 numChan, void* sjArray) {
    void* sjd = NULL;
    u8* base = lbl_eu_805E3358;
    int i;
    void* bufPtr;
    u32 bufSize, xtrSize, halfBuf, halfXtr;
    void* adxb;
    u32* src;
    u32* dst;

    /* Find free slot (unrolled 8-at-a-time scan) */
    for (i = 0; i < 16; i++) {
        if (base[i * 0xB4] == 0) {
            sjd = &base[i * 0xB4];
            break;
        }
    }
    if (sjd == NULL) return NULL;

    bufPtr = SJRBF_GetBufPtr(stream);
    bufSize = SJRBF_GetBufSize(stream);
    halfBuf = bufSize / 2;
    xtrSize = SJRBF_GetXtrSize(stream);
    halfXtr = xtrSize / 2;

    adxb = ADXB_Create((u8*)sjd + 0x0C, bufPtr, halfBuf, halfBuf + halfXtr);
    if (adxb == NULL) return NULL;

    *(void**)((u8*)sjd + 0x04) = adxb;
    ADXB_EntryGetWrFunc(adxb, adxsjd_get_wr, sjd);
    *(void**)((u8*)sjd + 0x08) = stream;
    *(s8*)((u8*)sjd + 0x02) = (s8)numChan;

    /* Copy channel SJ pointers */
    src = (u32*)sjArray;
    dst = (u32*)((u8*)sjd + 0x0C);
    for (i = 0; i < numChan; i++) {
        dst[i] = src[i];
    }

    /* Initialize state fields */
    *(u8*)((u8*)sjd + 0x01) = 0;          /* status */
    *(u32*)((u8*)sjd + 0xA0) = 0;
    *(u32*)((u8*)sjd + 0x2C) = 0;
    *(u32*)((u8*)sjd + 0x30) = 0;
    *(u32*)((u8*)sjd + 0x34) = 0;
    *(u32*)((u8*)sjd + 0x38) = 0x7FFFFFFF;
    *(u32*)((u8*)sjd + 0x3C) = (u32)-1;
    *(u32*)((u8*)sjd + 0x40) = 0;
    *(u32*)((u8*)sjd + 0x44) = 0;
    *(u8*)((u8*)sjd + 0x03) = 0;
    *(u32*)((u8*)sjd + 0xA8) = 0;
    *(u32*)((u8*)sjd + 0xAC) = 0;
    *(u32*)((u8*)sjd + 0x48) = 0;
    *(u32*)((u8*)sjd + 0x4C) = 0;
    *(u32*)((u8*)sjd + 0x58) = 0;
    *(u32*)((u8*)sjd + 0x5C) = 0;
    *(u8*)((u8*)sjd + 0x00) = 1;          /* inUse */

    return sjd;
}

void ADXSJD_Destroy(void* self) {
    void* adxb;
    if (self == NULL) return;

    adxb = *(void**)((u8*)self + 0x04);
    if (adxb != NULL) {
        *(void**)((u8*)self + 0x04) = NULL;
        ADXB_Destroy(adxb);
    }

    ADXCRS_Lock();
    memset(self, 0, 0xB4);
    ADXCRS_Unlock();
}

int ADXSJD_GetStat(void* self) {
    return (s32)*(s8*)((u8*)self + 0x01);
}

void ADXSJD_SetInSj(void* self, void* sj) {
    *(void**)((u8*)self + 0x08) = sj;
    ADXB_SetAhxInSj(*(void**)((u8*)self + 0x04));
}

void ADXSJD_SetMaxDecSmpl(void* self, u32 val) {
    *(u32*)((u8*)self + 0x38) = val;
    ADXB_SetAhxDecSmpl(*(void**)((u8*)self + 0x04), val);
}

void ADXSJD_TermSupply(void* self) {
    ADXB_AhxTermSupply(*(void**)((u8*)self + 0x04));
}

void ADXSJD_Start(void* self) {
    *(u32*)((u8*)self + 0xA0) = 0;
    *(u32*)((u8*)self + 0x2C) = 0;
    *(u32*)((u8*)self + 0x30) = 0;
    *(u32*)((u8*)self + 0x34) = 0;
    *(u32*)((u8*)self + 0x38) = 0x7FFFFFFF;
    *(u32*)((u8*)self + 0x3C) = (u32)-1;
    *(u32*)((u8*)self + 0x40) = 0;
    *(u32*)((u8*)self + 0x44) = 0;
    *(u8*)((u8*)self + 0x03) = 0;
    *(u32*)((u8*)self + 0xA8) = 0;
    *(u32*)((u8*)self + 0xAC) = 0;
    *(u8*)((u8*)self + 0x01) = 1;
}

void ADXSJD_Stop(void* self) {
    ADXB_Stop(*(void**)((u8*)self + 0x04));
    *(u8*)((u8*)self + 0x01) = 0;
}

void adxsjd_decode_prep(void* self) {
    void* inSj = *(void**)((u8*)self + 0x08);
    void* adxb = *(void**)((u8*)self + 0x04);
    u32 sjBuf[4]; /* stack buffer for SJ split/commit */
    s32 hdrLen;
    s32 fmt;

    /* Read from input SJ via virtual call (vtable[6] = offset 0x18) */
    {
        void** vtbl = *(void***)inSj;
        void (*readFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[6];
        readFn(inSj, 1, sjBuf);
    }

    /* Check chunk length */
    if ((s32)sjBuf[1] <= 0) {
        /* No data, cancel read via vtable[7] = offset 0x1C */
        void** vtbl = *(void***)inSj;
        void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
        unreadFn(inSj, 1, sjBuf);
        return;
    }

    /* Check ADXB status fields */
    {
        s16 f9A = *(s16*)((u8*)adxb + 0x9A);
        s16 f98 = *(s16*)((u8*)adxb + 0x98);

        if (f9A == 0 && f98 != 0x0D) {
            /* Scan for leading zero bytes in chunk */
            u8* p = (u8*)sjBuf[0];
            s32 idx = 0;
            while (idx < (s32)sjBuf[1]) {
                if (*p != 0) break;
                p++;
                idx++;
            }

            /* Split off leading bytes and commit */
            SJ_SplitChunk(sjBuf, idx, sjBuf, sjBuf + 2);
            {
                void** vtbl = *(void***)inSj;
                void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
                commitFn(inSj, 0, sjBuf);
            }

            if ((s32)sjBuf[1] < 0x10) {
                void** vtbl = *(void***)inSj;
                void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
                unreadFn(inSj, 1, sjBuf);
                return;
            }
        }
    }

    /* Decode header */
    {
        s16 f9A = *(s16*)((u8*)adxb + 0x9A);
        s16 f98 = *(s16*)((u8*)adxb + 0x98);

        if (f9A == 0 && f98 != 0x0D) {
            hdrLen = ADXB_DecodeHeader(adxb, (u8*)sjBuf[0]);
            if (hdrLen == 0 || hdrLen > (s32)sjBuf[1]) {
                void** vtbl = *(void***)inSj;
                void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
                unreadFn(inSj, 1, sjBuf);
                return;
            }
            if (hdrLen < (s32)sjBuf[1]) {
                void** vtbl = *(void***)inSj;
                void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
                unreadFn(inSj, 1, sjBuf);
                if (ADXB_GetDecErrMode(adxb) == 0) {
                    ADXERR_CallErrFunc2_(lbl_eu_805160B8, lbl_eu_805160B8 + 0x1E);
                }
                *(u8*)((u8*)self + 0x01) = 4;
                return;
            }
        } else {
            if (f98 == 0x0D) {
                ADXB_SetDefPrm(adxb);
            } else {
                criware_eu_8038A864(adxb);
            }
            hdrLen = 0;
        }
    }

    *(u32*)((u8*)self + 0xA0) = hdrLen;

    /* Call filter callback if registered */
    if (*(u32*)((u8*)self + 0x50) != 0) {
        void (*fltFunc)(void*, s32, s32, s32, u32) =
            *(void (**)(void*, s32, s32, s32, u32))((u8*)self + 0x50);
        void* fltCtx = *(void**)((u8*)self + 0x54);
        fltFunc(fltCtx,
                ADXB_GetFormat(adxb),
                ADXB_GetNumChan(adxb),
                ADXB_GetSfreq(adxb),
                ADXB_GetTotalNumSmpl(adxb));
    }

    /* Set AHX flag for format 4 */
    if (ADXB_GetFormat(adxb) == 4) {
        *(u8*)((u8*)self + 0x03) = 1;
    }

    /* Copy SPSD info for format 2 */
    if (ADXB_GetFormat(adxb) == 2) {
        u32 copyLen = sjBuf[1];
        if (copyLen > 0x40) copyLen = 0x40;
        memcpy((u8*)self + 0x60, (void*)sjBuf[0], copyLen);
    }

    /* Handle different format types */
    fmt = ADXB_GetFormat(adxb);
    if ((fmt >= 0x0A && fmt <= 0x0D) || fmt == 0x14 || fmt == 0x0F) {
        void** vtbl = *(void***)inSj;
        void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
        unreadFn(inSj, 1, sjBuf);
    } else {
        SJ_SplitChunk(sjBuf, hdrLen, sjBuf, sjBuf + 2);
        {
            void** vtbl = *(void***)inSj;
            void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
            commitFn(inSj, 0, sjBuf);
        }
        {
            void** vtbl = *(void***)inSj;
            void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
            unreadFn(inSj, 1, sjBuf);
        }
    }

    /* Call link-switch callback if present */
    if (*(u32*)((u8*)adxb + 0xE0) != 0) {
        void (*cb)(void*, u32) = *(void (**)(void*, u32))&lbl_eu_805E3354;
        if (cb != NULL) {
            cb(adxb, *(u32*)((u8*)adxb + 0x14));
        }
    }

    *(u8*)((u8*)self + 0x01) = 2;
}

void adxsjd_get_wr(void* self, u32 reqLen, void* outA, void* outB) {
    void* adxb = *(void**)((u8*)self + 0x04);
    int i, nch;
    u8* wrBase = (u8*)self + 0x1C;
    u8* sjBase = (u8*)self + 0x0C;
    u32 diff, maxLen;
    u32 wrBuf[2]; /* ptr, len */

    nch = ADXB_GetNumChan(adxb);
    for (i = 0; i < nch; i++) {
        void* chan = *(void**)(sjBase + i * 4);
        void** vtbl = *(void***)chan;
        void (*getBufFn)(void*, s32, s32, void*) = (void (*)(void*, s32, s32, void*))vtbl[6];
        getBufFn(chan, 0, 0x4000, wrBase + i * 8);
    }

    /* outA = wr[0].ptr distance from buf start, halved */
    {
        void* bufPtr = SJRBF_GetBufPtr(self);
        diff = (u32)*(void**)(wrBase) - (u32)bufPtr;
    }
    *(u32*)outA = (diff + (diff >> 31)) / 2;

    /* outB = min(wr[0].len/2, maxDecSmpl) */
    {
        u32 w0len = *(u32*)(wrBase + 4);
        maxLen = (w0len + (w0len >> 31)) / 2;
    }
    if (maxLen < *(u32*)((u8*)self + 0x38)) {
        *(u32*)outB = maxLen;
    } else {
        *(u32*)outB = *(u32*)((u8*)self + 0x38);
    }

    /* Set trap remaining */
    {
        s32 trapNum = *(s32*)((u8*)self + 0x3C);
        if (trapNum >= 0) {
            *(u32*)((u8*)outB + 4) = (u32)(trapNum - *(s32*)((u8*)self + 0x40));
        } else {
            *(u32*)((u8*)outB + 4) = 0x1FFFFFFF;
        }
    }

    ADXB_GetPcmBuf(adxb);
}

void adxsjd_decexec_start(void* self) {
    void* adxb = *(void**)((u8*)self + 0x04);
    void* inSj = *(void**)((u8*)self + 0x08);
    u32 sjBuf[4];
    s32 needEnd = 0;
    u32 totalSmpl;

    /* Check trap callback */
    {
        s32 trapNum = *(s32*)((u8*)self + 0x3C);
        if (trapNum >= 0 && *(u32*)((u8*)self + 0x40) >= (u32)trapNum) {
            void (*trapFn)(void*, u32, u32) = *(void (**)(void*, u32, u32))((u8*)self + 0x48);
            if (trapFn != NULL) {
                trapFn(*(void**)((u8*)self + 0x4C),
                       *(u32*)((u8*)self + 0x40),
                       *(u32*)((u8*)self + 0x44));
            }
        }
    }

    /* Check AHX termination */
    if (*(u8*)((u8*)self + 0x03) == 1) {
        void** vtbl = *(void***)inSj;
        s32 (*getStatFn)(void*, s32) = (s32 (*)(void*, s32))vtbl[9];
        if (getStatFn(inSj, 1) == 0) {
            *(u8*)((u8*)self + 0x01) = 3;
            return;
        }
    }

    /* Read from input SJ */
    {
        void** vtbl = *(void***)inSj;
        void (*readFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[6];
        readFn(inSj, 1, sjBuf);
    }

    /* Check format: 0 = undetermined */
    if (ADXB_GetFormat(adxb) == 0) {
        u32 cLen = *(u32*)((u8*)self + 0x18);
        if (cLen >= 4) {
            u16 magic = *(u16*)*(u32*)((u8*)self + 0x14);
            if (magic == 0x8001) {
                s16 footerLen;
                *(u8*)((u8*)self + 0x01) = 3;
                if (ADX_DecodeFooter(*(u8**)((u8*)self + 0x14), &footerLen)) {
                    if ((u32)footerLen > cLen) {
                        void** vtbl = *(void***)inSj;
                        void (*readFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[6];
                        readFn(inSj, 1, sjBuf);
                        return;
                    }
                    SJ_SplitChunk((u8*)self + 0x14, (u32)footerLen, (u8*)self + 0x14, sjBuf);
                    {
                        void** vtbl = *(void***)inSj;
                        void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
                        commitFn(inSj, 0, (u8*)self + 0x14);
                    }
                    {
                        void** vtbl = *(void***)inSj;
                        void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
                        unreadFn(inSj, 1, sjBuf);
                    }
                }

                /* Link switch loop */
                if (*(u32*)((u8*)self + 0xA4) != 0) {
                    while (1) {
                        u32 dataLen;
                        s32 skip;
                        void** vtbl;
                        void (*readFn)(void*, s32, void*);
                        void (*commitFn)(void*, s32, void*);
                        void (*unreadFn)(void*, s32, void*);

                        vtbl = *(void***)inSj;
                        readFn = (void (*)(void*, s32, void*))vtbl[6];
                        readFn(inSj, 1, sjBuf);
                        dataLen = *(u32*)((u8*)self + 0x18);
                        if (dataLen == 0) break;
                        skip = 0;
                        {
                            u8* p = *(u8**)((u8*)self + 0x14);
                            while (skip < (s32)dataLen) {
                                if (p[skip] != 0) break;
                                skip++;
                            }
                        }
                        SJ_SplitChunk((u8*)self + 0x14, (u32)skip, (u8*)self + 0x14, sjBuf);
                        vtbl = *(void***)inSj;
                        commitFn = (void (*)(void*, s32, void*))vtbl[8];
                        commitFn(inSj, 0, (u8*)self + 0x14);
                        vtbl = *(void***)inSj;
                        unreadFn = (void (*)(void*, s32, void*))vtbl[7];
                        unreadFn(inSj, 1, sjBuf);
                        if (skip >= (s32)dataLen) break;
                    }
                }
                return;
            }
        }
    }

    /* Check if decoding is complete */
    totalSmpl = ADXB_GetTotalNumSmpl(adxb);
    if (*(u32*)((u8*)self + 0x34) >= totalSmpl) {
        if (ADXB_GetFormat(adxb) == 1 && *(u32*)((u8*)self + 0xB0) == 1) {
            /* Continue */
        } else {
            needEnd = 1;
        }
    } else if (ADXB_GetFormat(adxb) == 0x0A) {
        u32 decPos = *(u32*)((u8*)self + 0x34);
        if (decPos > 0 && decPos + 0x240 >= totalSmpl) {
            needEnd = 1;
        }
        if ((s32)totalSmpl <= 0) {
            needEnd = 1;
        }
    }

    if (needEnd) {
        *(u8*)((u8*)self + 0x01) = 3;
        {
            void** vtbl = *(void***)inSj;
            void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
            unreadFn(inSj, 1, sjBuf);
        }
        return;
    }

    /* Check block sample availability */
    {
        s32 blkSmpl = ADXB_GetBlkSmpl(adxb);
        void* sj0 = *(void**)((u8*)self + 0x0C);
        void** vtbl = *(void***)sj0;
        s32 (*getFreeFn)(void*, s32) = (s32 (*)(void*, s32))vtbl[9];
        s32 freeSize = getFreeFn(sj0, 0);
        u32 avail = ((u32)freeSize + ((u32)freeSize >> 31)) / 2;
        if (avail < (u32)blkSmpl) {
            void** vtbl2 = *(void***)inSj;
            void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl2[7];
            unreadFn(inSj, 1, sjBuf);
            return;
        }
    }

    /* Handle 16-bit PCM truncation */
    if (*(u32*)((u8*)self + 0xB0) != 1 && ADXB_GetFormat(adxb) == 1) {
        if (ADXB_GetFmtBps(adxb) == 0x10) {
            s32 nch = ADXB_GetNumChan(adxb);
            u32 cLen = *(u32*)((u8*)self + 0x18);
            u32 bps = (cLen / nch + ((cLen / nch) >> 31)) / 2;
            u32 endPos = *(u32*)((u8*)self + 0x34) + bps;
            if (endPos > totalSmpl) {
                u32 remain = totalSmpl - *(u32*)((u8*)self + 0x34);
                u32 byteLen = nch * remain * 2;
                SJ_SplitChunk((u8*)self + 0x14, byteLen, (u8*)self + 0x14, sjBuf);
                {
                    void** vtbl = *(void***)inSj;
                    void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
                    unreadFn(inSj, 1, sjBuf);
                }
            }
        } else {
            ADXERR_CallErrFunc2_(lbl_eu_805160B8 + 0x3F, lbl_eu_805160B8 + 0x60);
        }
    }

    /* AHX format: discard chunk */
    if (ADXB_GetFormat(adxb) == 0x0A) {
        void** vtbl = *(void***)inSj;
        void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
        unreadFn(inSj, 1, sjBuf);
    }

    ADXB_EntryData(adxb, *(u8**)((u8*)self + 0x14), *(u32*)((u8*)self + 0x18));
    ADXB_Start(adxb);
}

void adxsjd_decexec_end(void* self) {
    void* adxb = *(void**)((u8*)self + 0x04);
    void* inSj = *(void**)((u8*)self + 0x08);
    u32 totalSmpl = ADXB_GetTotalNumSmpl(adxb);
    u32 decDtLen = ADXB_GetDecDtLen(adxb);
    u32 decNumSmpl = ADXB_GetDecNumSmpl(adxb);
    u32 sjBuf[4];
    u32 outBuf[2];
    int i, nch;

    /* Limit decoded samples */
    if (!(ADXB_GetFormat(adxb) == 1 && *(u32*)((u8*)self + 0xB0) == 1)) {
        u32 remaining = totalSmpl - *(u32*)((u8*)self + 0x34);
        if (decNumSmpl < remaining) {
            /* keep */
        } else {
            decNumSmpl = remaining;
        }
    }

    SJ_SplitChunk((u8*)self + 0x14, decDtLen, sjBuf, outBuf);

    {
        void** vtbl = *(void***)inSj;
        void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
        commitFn(inSj, 0, sjBuf);
    }
    {
        void** vtbl = *(void***)inSj;
        void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
        unreadFn(inSj, 1, sjBuf);
    }

    /* Process each channel */
    {
        u32 smplBytes = decNumSmpl * 2;
        u8* wrBase = (u8*)self + 0x1C;
        u8* sjBase = (u8*)self + 0x0C;
        nch = ADXB_GetNumChan(adxb);
        for (i = 0; i < nch; i++) {
            SJ_SplitChunk(wrBase + i * 8, smplBytes, sjBuf, outBuf);

            /* Channel callback */
            if (*(u32*)((u8*)self + 0x58) != 0) {
                void (*cbFn)(void*, s32, void*, u32) =
                    *(void (**)(void*, s32, void*, u32))((u8*)self + 0x58);
                cbFn(*(void**)((u8*)self + 0x5C), i, (void*)sjBuf[0], sjBuf[1]);
            }

            {
                void* chan = *(void**)(sjBase + i * 4);
                void** vtbl = *(void***)chan;
                void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
                commitFn(chan, 1, sjBuf);
            }
            {
                void* chan = *(void**)(sjBase + i * 4);
                void** vtbl = *(void***)chan;
                void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
                unreadFn(chan, 0, sjBuf);
            }
        }
    }

    *(u32*)((u8*)self + 0x2C) += decNumSmpl;
    *(u32*)((u8*)self + 0x30) += decDtLen;
    *(u32*)((u8*)self + 0x34) += decNumSmpl;
    *(u32*)((u8*)self + 0x40) += decNumSmpl;
    *(u32*)((u8*)self + 0x44) += decDtLen;
    ADXB_Reset(adxb);
}

void ADXSJD_ExecHndl(void* self) {
    void* adxb;
    s16 fmt;

    /* Insert proc */
    if (*(s32*)((u8*)self + 0xA8) > 0) {
        ADXCRS_Lock();
        adxsjd_insert_proc(self);
        ADXCRS_Unlock();
    }

    /* State machine */
    if (*(s8*)((u8*)self + 0x01) == 2) {
        adxb = *(void**)((u8*)self + 0x04);
        if (ADXB_GetStat(adxb) == 0) {
            adxsjd_decexec_start(self);
        }
        ADXB_ExecHndl(adxb);
        if (ADXB_GetStat(adxb) == 3) {
            adxsjd_decexec_end(self);
        }
        fmt = ADXB_GetFormat(adxb);
        if ((fmt >= 0x0A && fmt <= 0x0C) || fmt == 0x14 || fmt == 0x0F) {
            u32 dtLen = ADXB_GetDecDtLen(adxb);
            u32 numSmpl = ADXB_GetDecNumSmpl(adxb);
            *(u32*)((u8*)self + 0x2C) += numSmpl;
            *(u32*)((u8*)self + 0x30) += dtLen;
            *(u32*)((u8*)self + 0x34) += numSmpl;
        }
    } else if (*(s8*)((u8*)self + 0x01) == 1) {
        adxsjd_decode_prep(self);
    }

    /* Discard proc */
    if (*(s32*)((u8*)self + 0xAC) > 0) {
        ADXCRS_Lock();
        adxsjd_discard_proc(self);
        ADXCRS_Unlock();
    }
}

void adxsjd_insert_proc(void* self) {
    int i, nch;
    u32 totalInsert, maxInsert, aligned, half;
    u32 wrBuf[2];

    nch = (s32)*(s8*)((u8*)self + 0x02);
    totalInsert = *(u32*)((u8*)self + 0xA8) * 2;
    maxInsert = totalInsert;

    /* Determine max insertable bytes */
    for (i = 0; i < nch; i++) {
        void* chan = *(void**)((u8*)self + 0x0C + i * 4);
        void** vtbl = *(void***)chan;
        void (*getBufFn)(void*, s32, s32, void*) = (void (*)(void*, s32, s32, void*))vtbl[6];
        getBufFn(chan, 0, 0x7FFFFFFF, wrBuf);
        if (maxInsert > wrBuf[1]) {
            maxInsert = wrBuf[1];
        }
    }

    aligned = (maxInsert + 1) & ~1u;
    half = aligned / 2;
    if ((s32)aligned <= 0) return;

    /* Insert silence into each channel */
    for (i = 0; i < nch; i++) {
        void* chan = *(void**)((u8*)self + 0x0C + i * 4);
        void** vtbl = *(void***)chan;
        void (*getBufFn)(void*, s32, s32, void*) = (void (*)(void*, s32, s32, void*))vtbl[6];
        void (*commitFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[8];
        getBufFn(chan, 0, aligned, wrBuf);
        memset((void*)wrBuf[0], 0, aligned);
        commitFn(chan, 1, wrBuf);
    }

    *(s32*)((u8*)self + 0xA8) -= (s32)half;
}

void adxsjd_discard_proc(void* self) {
    int i, nch;
    u32 totalDiscard, maxDiscard, aligned, half;
    u32 wrBuf[2];

    nch = (s32)*(s8*)((u8*)self + 0x02);
    totalDiscard = *(u32*)((u8*)self + 0xAC) * 2;
    maxDiscard = totalDiscard;

    /* Determine max discardable bytes */
    for (i = 0; i < nch; i++) {
        void* chan = *(void**)((u8*)self + 0x0C + i * 4);
        void** vtbl = *(void***)chan;
        void (*getBufFn)(void*, s32, s32, void*) = (void (*)(void*, s32, s32, void*))vtbl[6];
        getBufFn(chan, 1, 0x7FFFFFFF, wrBuf);
        if (maxDiscard > wrBuf[1]) {
            maxDiscard = wrBuf[1];
        }
    }

    aligned = (maxDiscard + 1) & ~1u;
    half = aligned / 2;
    if ((s32)aligned <= 0) return;

    /* Discard from each channel */
    for (i = 0; i < nch; i++) {
        void* chan = *(void**)((u8*)self + 0x0C + i * 4);
        void** vtbl = *(void***)chan;
        void (*getBufFn)(void*, s32, s32, void*) = (void (*)(void*, s32, s32, void*))vtbl[6];
        void (*unreadFn)(void*, s32, void*) = (void (*)(void*, s32, void*))vtbl[7];
        getBufFn(chan, 1, aligned, wrBuf);
        unreadFn(chan, 0, wrBuf);
    }

    *(s32*)((u8*)self + 0xAC) -= (s32)half;
}

void ADXSJD_ExecServer(void) {
    int i;
    u8* base = (u8*)&lbl_eu_805E3340;

    /* Pre-server callback */
    {
        u32 preCb = *(u32*)(base + 0x04);
        if (preCb != 0) {
            void (*cb)(void*) = (void (*)(void*))preCb;
            cb(*(void**)(base + 0x08));
        }
    }

    /* Process each handle */
    {
        u8* sjd = base + 0x18;
        for (i = 0; i < 16; i++) {
            if (*sjd == 1) {
                /* Insert */
                if (*(s32*)(sjd + 0xA8) > 0) {
                    ADXCRS_Lock();
                    adxsjd_insert_proc(sjd);
                    ADXCRS_Unlock();
                }

                if (*(s8*)(sjd + 0x01) == 2) {
                    void* adxb = *(void**)(sjd + 0x04);
                    s16 fmt;
                    if (ADXB_GetStat(adxb) == 0) {
                        adxsjd_decexec_start(sjd);
                    }
                    ADXB_ExecHndl(adxb);
                    if (ADXB_GetStat(adxb) == 3) {
                        adxsjd_decexec_end(sjd);
                    }
                    fmt = ADXB_GetFormat(adxb);
                    if ((fmt >= 0x0A && fmt <= 0x0C) || fmt == 0x14 || fmt == 0x0F) {
                        *(u32*)(sjd + 0x2C) += ADXB_GetDecNumSmpl(adxb);
                        *(u32*)(sjd + 0x30) += ADXB_GetDecDtLen(adxb);
                        *(u32*)(sjd + 0x34) += ADXB_GetDecNumSmpl(adxb);
                    }
                } else if (*(s8*)(sjd + 0x01) == 1) {
                    adxsjd_decode_prep(sjd);
                }

                /* Discard */
                if (*(s32*)(sjd + 0xAC) > 0) {
                    ADXCRS_Lock();
                    adxsjd_discard_proc(sjd);
                    ADXCRS_Unlock();
                }
            }
            sjd += 0xB4;
        }
    }

    /* Post-server callback */
    {
        u32 postCb = *(u32*)(base + 0x0C);
        if (postCb != 0) {
            void (*cb)(void*) = (void (*)(void*))postCb;
            cb(*(void**)(base + 0x10));
        }
    }
}

u32 ADXSJD_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x30); }

u32 ADXSJD_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x2C); }

void ADXSJD_SetDecPos(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void ADXSJD_SetLnkSw(void* self, u32 val) {
    *(u32*)((u8*)self + 0xA4) = val;
    ADXB_SetLnkSw(*(void**)((u8*)self + 0x04), val);
}

void ADXSJD_SetDefFmt(void* self) {
    ADXB_SetDefFmt(*(void**)((u8*)self + 0x04));
}

void ADXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x50) = func;
    *(void**)((u8*)self + 0x54) = ctx;
}

void ADXSJD_EntryTrapFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x48) = func;
    *(void**)((u8*)self + 0x4C) = ctx;
}

void ADXSJD_SetTrapNumSmpl(void* self, u32 val) { *(u32*)((u8*)self + 0x3C) = val; }

void ADXSJD_SetTrapCnt(void* self, u32 val) { *(u32*)((u8*)self + 0x40) = val; }

void ADXSJD_SetTrapDtLen(void* self, u32 val) { *(u32*)((u8*)self + 0x44) = val; }

s16 ADXSJD_GetFormat(void* self) {
    return ADXB_GetFormat(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetSfreq(void* self) {
    return ADXB_GetSfreq(*(void**)((u8*)self + 0x04));
}

void ADXSJD_GetNumChan(void* self) {
    ADXB_GetNumChan(*(void**)((u8*)self + 0x04));
}

void ADXSJD_GetOutBps(void* self) {
    ADXB_GetOutBps(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetBlkSmpl(void* self) {
    return ADXB_GetBlkSmpl(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetTotalNumSmpl(void* self) {
    return ADXB_GetTotalNumSmpl(*(void**)((u8*)self + 0x04));
}

s16 ADXSJD_GetNumLoop(void* self) {
    return ADXB_GetNumLoop(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpStartPos(void* self) {
    return ADXB_GetLpStartPos(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpStartOfst(void* self) {
    if (self == NULL) return 0;
    return ADXB_GetLpStartOfst(*(u32*)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpEndPos(void* self) {
    return ADXB_GetLpEndPos(*(void**)((u8*)self + 0x04));
}

u32 ADXSJD_GetLpEndOfst(void* self) {
    return ADXB_GetLpEndOfst(*(void**)((u8*)self + 0x04));
}

s32 ADXSJD_GetDefOutVol(void* self) {
    if (ADXB_GetAinfLen(*(void**)((u8*)self + 0x04)) > 0) {
        u8 st = *(u8*)((u8*)self + 0x01);
        if ((u8)(st - 2) <= 1) {
            return ADXB_GetDefOutVol(*(void**)((u8*)self + 0x04));
        }
    }
    return 0;
}

s32 ADXSJD_GetDefPan(void* self, s32 pan) {
    if (ADXB_GetAinfLen(*(void**)((u8*)self + 0x04)) > 0) {
        u8 st = *(u8*)((u8*)self + 0x01);
        if ((u8)(st - 2) <= 1) {
            return ADXB_GetDefPan(*(void**)((u8*)self + 0x04), pan);
        }
    }
    return -128;
}

void* ADXSJD_GetSpsdInfo(void* self) { return (void*)((u8*)self + 0x60); }

void ADXSJD_TakeSnapshot(void* self) {
    ADXB_TakeSnapshot(*(void**)((u8*)self + 0x04));
}

void ADXSJD_RestoreSnapshot(void* self) {
    ADXB_RestoreSnapshot(*(void**)((u8*)self + 0x04));
}
