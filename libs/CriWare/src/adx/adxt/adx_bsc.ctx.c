// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsc
// Replace stubs with high-level C/C++ during decomp.

/* "libs/CriWare/src/adx/adxt/adx_bsc.c" line 3 "harness_catalog.h" */
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

extern void ADXPD_Init(void);
extern void* ADXPD_Create(void);
extern void ADXPD_Destroy(void* pd);
extern void ADXPD_Stop(void* pd);
extern void ADXPD_Reset(void* pd);
extern void ADXPD_EntryMono(void* pd, void* data, void* dest, void* arg7);
extern void ADXPD_EntrySte(void* pd, void* data, s32 num, void* dest, void* arg7);
extern void ADXPD_EntryPl2(void* pd, void* data, s32 num, void* dest, void* arg7);
extern void ADXPD_Start(void* pd);
extern int ADXPD_GetStat(void* pd);
extern int ADXPD_GetNumBlk(void* pd);
extern void ADXPD_ExecHndl(void* pd);
extern void ADXPD_GetDly(void* pd, void* out1, void* out2);
extern void ADXPD_SetDly(void* pd, void* in1, void* in2);
extern void ADXPD_GetExtPrm(void* pd, void* out1, void* out2, void* out3);
extern void ADXPD_SetExtPrm(void* pd, s16 v1, s16 v2, s16 v3);

extern s32 ADX_DecodeInfo(void* data, s32 size, void* out_audioOffset,
    void* out_encoding, void* out_version, void* out_blockSize,
    void* out_channelCount, void* out_sampleRate,
    void* out_totalSamples, void* out_samplesPerBlock);
extern s32 ADX_DecodeInfoExVer(void* data, s32 size, void* out_version, void* out_flags);
extern s32 ADX_DecodeInfoExADPCM2(void* data, s32 size, void* out);
extern s32 ADX_DecodeInfoExIdly(void* data, s32 size, void* out1, void* out2);
extern s32 ADX_DecodeInfoExLoop(void* data, s32 size,
    void* out1, void* out2, void* out3, void* out4,
    void* out5, void* out6, void* out7);
extern s32 ADX_DecodeInfoAinf(void* data, s32 size,
    void* out1, void* out2, void* out3, void* out4);
extern void ADXPD_SetCoef(void* pd, u32 sampleRate, s16 coef);
extern int ADXB_CheckSpsd(void* data);
extern int ADXB_CheckWav(void* data);
extern int ADXB_CheckAiff(void* data);
extern int ADXB_CheckAu(void* data);
extern s32 ADXB_DecodeHeaderSpsd(void* adxb, void* data, void* out_encoding);
extern s32 ADXB_DecodeHeaderWav(void* adxb, void* data, void* out_encoding);
extern s32 ADXB_DecodeHeaderAiff(void* adxb, void* data, void* out_encoding);
extern s32 ADXB_DecodeHeaderAu(void* adxb, void* data, void* out_encoding);
extern void ADXB_ExecOneAhx(void* adxb);
extern void ADXB_ExecOneSpsd(void* adxb);
extern void ADXB_ExecOneAiff(void* adxb);
extern void ADXB_ExecOneAu(void* adxb);
extern void ADXB_ExecOneWav(void* adxb);
extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);

extern void CRICRW_Sprintf(char* buf, int size, const char* fmt, ...);

extern u32 lbl_eu_805E5370;
extern u32 lbl_eu_805E5358;
extern u8 lbl_eu_805E5378[0x1000];
extern u32 lbl_eu_80560048;
extern s16 lbl_eu_80516B30[];

extern char lbl_eu_80517330[];
extern void ADXERR_CallErrFunc1_(const char* msg);
extern void ADXERR_CallErrFunc2_(const char* msg, const char* detail);

extern void (*lbl_eu_805E5364)(void*);
extern void (*lbl_eu_805E535C)(void*, void*);
extern void (*lbl_eu_805E5360)(void*, s16, void*, void*);

/* Forward declarations */
int adxb_get_key(void* adxb, void* data, void* out_key1, void* out_key2, void* out_key3);
s32 ADXB_DecodeHeaderAdx(void* adxb, void* data, void* out_encoding);

s16 SKG_GenerateKey(u8* data, int len, s16* key1, s16* key2, s16* key3) {
    int i;
    s16 x;

    if (lbl_eu_805E5358 == 0) {
        lbl_eu_805E5358 = lbl_eu_805E5358 + 1;
    }

    *key1 = 0;
    *key2 = 0;
    *key3 = 0;

    if (data == NULL && len <= 0) {
        return 0;
    }

    x = 0x4A1D;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x * (s32)lbl_eu_80516B30[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = (m >> 31) & 1;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x = *(s16*)((u8*)lbl_eu_80516B30 + (t << 1));
    }
    *key1 = x;

    x = 0x53FF;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x * (s32)lbl_eu_80516B30[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = (m >> 31) & 1;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x = *(s16*)((u8*)lbl_eu_80516B30 + (t << 1));
    }
    *key2 = x;

    x = 0x5DC1;
    for (i = 0; i < len; i++) {
        u32 m = (u32)((s32)x * (s32)lbl_eu_80516B30[0x80 + (s8)data[i]]);
        u32 b = m << 22;
        u32 s = (m >> 31) & 1;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x = *(s16*)((u8*)lbl_eu_80516B30 + (t << 1));
    }
    *key3 = x;

    return 0;
}

u32 ADXB_GetDecErrMode(void) { return lbl_eu_805E5370; }

void ADXB_Init(void) {
    u8* p = (u8*)&lbl_eu_805E5358;
    ADXPD_Init();
    (*(volatile u32*)p)++;
    memset(p + 0x20, 0, 0x1000);
    *(u32*)(p + 0x18) = 0;
}

u32 adxb_DefGetWr(void* self, u32* out1, u32* out2, u32* out3) {
    *out1 = *(u32*)((u8*)self + 0x8C);
    *out2 = *(u32*)((u8*)self + 0x40) - *(u32*)((u8*)self + 0x8C);
    *out3 = *(u32*)((u8*)self + 0x18) - *(u32*)((u8*)self + 0x88);
    return *(u32*)((u8*)self + 0x3C);
}

void adxb_DefAddWr(void* self, u32 unused, u32 addend) {
    *(u32*)((u8*)self + 0x8c) += addend;
    *(u32*)((u8*)self + 0x88) += addend;
}

void* ADXB_Create(void* arg0, void* pcmBuf, void* workBuf, void* workSize) {
    int i;
    u8* slot;
    u8* p;

    p = (u8*)lbl_eu_805E5378;
    for (i = 0; i < 16; i++) {
        if (*(s16*)p == 0) break;
        p += 0x100;
    }
    if (i == 16) return NULL;

    slot = (u8*)lbl_eu_805E5378 + i * 0x100;
    memset(slot, 0, 0x100);
    *(u16*)(slot + 0x00) = 1;

    {
        void* pd = ADXPD_Create();
        *(void**)(slot + 0x08) = pd;
        if (pd == NULL) {
            if (slot != NULL) {
                *(void**)(slot + 0x08) = NULL;
                ADXPD_Destroy(pd);
                memset(slot, 0, 0x100);
                *(u16*)(slot + 0x00) = 0;
            }
            return NULL;
        }
    }

    *(void**)(slot + 0x38) = arg0;
    *(void**)(slot + 0x3C) = pcmBuf;
    *(void**)(slot + 0x40) = workBuf;
    *(void**)(slot + 0x44) = workSize;
    *(void**)(slot + 0x78) = (void*)adxb_DefGetWr;
    *(void**)(slot + 0x7C) = slot;
    *(void**)(slot + 0x80) = (void*)adxb_DefAddWr;
    *(void**)(slot + 0x84) = slot;
    *(u32*)(slot + 0xC4) = 0;
    *(s16*)(slot + 0xD8) = 0;
    *(s16*)(slot + 0xDA) = -0x80;
    *(s16*)(slot + 0xDC) = -0x80;
    memset(slot + 0xC8, 0, 0x10);
    return slot;
}

void ADXB_Destroy(void* self) {
    void* pd;
    if (self == NULL) return;
    pd = *(void**)((u8*)self + 0x08);
    *(void**)((u8*)self + 0x08) = NULL;
    ADXPD_Destroy(pd);
    memset(self, 0, 0x100);
    *(u16*)((u8*)self + 0x00) = 0;
}

int adxb_get_key(void* adxb, void* data, void* out_key1, void* out_key2, void* out_key3) {
    u8* p = (u8*)adxb;
    s16* k3 = (s16*)out_key3;
    s16* k2 = (s16*)out_key2;
    s16* k1 = (s16*)out_key1;
    s16* defKeys = (s16*)((u8*)&lbl_eu_805E5358 + 0x10);

    if (lbl_eu_80560048 == 0) {
        if (p[0xEC] < 4) {
            *k1 = 0;
            *k2 = 0;
            *k3 = 0;
        } else if (p[0xED] >= 0x10) {
            char buf[16];
            CRICRW_Sprintf(buf, 16, lbl_eu_80517330 + 0xA7, data);
            SKG_GenerateKey((u8*)buf, 8, k1, k2, k3);
        } else if (p[0xED] >= 8) {
            if (*(s16*)(p + 0xA0) == 0 && *(s16*)(p + 0xA2) == 0 && *(s16*)(p + 0xA4) == 0) {
                *(s16*)(p + 0xA0) = defKeys[0];
                *(s16*)(p + 0xA2) = defKeys[1];
                *(s16*)(p + 0xA4) = defKeys[2];
            }
            *k1 = *(s16*)(p + 0xA0);
            *k2 = *(s16*)(p + 0xA2);
            *k3 = *(s16*)(p + 0xA4);
        } else {
            *k1 = 0;
            *k2 = 0;
            *k3 = 0;
        }
    } else {
        if (p[0xEC] < 4) {
            if (*(s16*)(p + 0xA0) == 0 && *(s16*)(p + 0xA2) == 0 && *(s16*)(p + 0xA4) == 0 &&
                defKeys[0] == 0 && defKeys[1] == 0 && defKeys[2] == 0) {
                *k1 = 0;
                *k2 = 0;
                *k3 = 0;
                return 0;
            }
            *k1 = -0x1000;
            *k2 = -0x7F24;
            *k3 = 0x7FFF;
            ADXERR_CallErrFunc1_(lbl_eu_80517330 + 0xAC);
            return -1;
        } else {
            if (*(s16*)(p + 0xA0) == 0 && *(s16*)(p + 0xA2) == 0 && *(s16*)(p + 0xA4) == 0) {
                *(s16*)(p + 0xA0) = defKeys[0];
                *(s16*)(p + 0xA2) = defKeys[1];
                *(s16*)(p + 0xA4) = defKeys[2];
            }
            *k1 = *(s16*)(p + 0xA0);
            *k2 = *(s16*)(p + 0xA2);
            *k3 = *(s16*)(p + 0xA4);
        }
    }
    return 0;
}

s32 ADXB_DecodeHeaderAdx(void* adxb, void* data, void* out_encoding) {
    u8* p = (u8*)adxb;
    s16 l3, l2, l1;
    s16 highPassVal;
    s16 dly2, dly1;
    s16 sp32;
    s16 k1, k2, k3;

    if (((u32)data & 1) != 0 && lbl_eu_805E5370 == 0) {
        ADXERR_CallErrFunc2_(lbl_eu_80517330, lbl_eu_80517330 + 0x1F);
        return -1;
    }

    *(u16*)(p + 0x02) = 1;

    if (ADX_DecodeInfo(data, (s32)out_encoding, &highPassVal,
            p + 0x0C, p + 0x0D, p + 0x0F, p + 0x0E,
            p + 0x14, p + 0x18, p + 0x10) < 0) {
        return 0;
    }

    if ((s8)p[0x0C] > 4) {
        if (*(u32*)(p + 0xB8) == 0) {
            ADXERR_CallErrFunc2_(lbl_eu_80517330 + 0x3E, lbl_eu_80517330 + 0x5E);
            return -1;
        }

        p[0x0D] = 8;
        p[0x0F] = p[0x0E] * 192;
        *(u32*)(p + 0x10) = 96;
        *(s16*)(p + 0x98) = 10;
        *(s16*)(p + 0x1C) = 0;
        *(s16*)(p + 0x24) = 0;
        *(s16*)(p + 0x26) = 0;
        *(u32*)(p + 0x20) = 0;
        *(u32*)(p + 0x28) = 0;
        *(u32*)(p + 0x2C) = 0;
        *(u32*)(p + 0x30) = 0;
        *(u32*)(p + 0x34) = 0;
        *(u32*)(p + 0x88) = 0;

        if (ADX_DecodeInfoExVer(data, (s32)out_encoding, p + 0xEC, p + 0xED) < 0) {
            return 0;
        }

        sp32 = 0;

        if (adxb_get_key(adxb, (void*)*(u32*)(p + 0x18), &k1, &k2, &k3) < 0) {
            return -1;
        }

        if (lbl_eu_805E535C != NULL) {
            lbl_eu_805E535C(*(void**)(p + 0xB8), &sp32);
        }
    } else {
        if (ADX_DecodeInfoExVer(data, (s32)out_encoding, p + 0xEC, p + 0xED) < 0) {
            return 0;
        }

        if (adxb_get_key(adxb, (void*)*(u32*)(p + 0x18), &l1, &l2, &l3) < 0) {
            return -1;
        }

        ADXPD_SetExtPrm(*(void**)(p + 0x08), l1, l2, l3);

        if (ADX_DecodeInfoExADPCM2(data, (s32)out_encoding, p + 0x1C) < 0) {
            return 0;
        }

        if (ADX_DecodeInfoExIdly(data, (s32)out_encoding, &dly1, &dly2) < 0) {
            return 0;
        }

        ADXPD_SetCoef(*(void**)(p + 0x08), *(u32*)(p + 0x14), *(s16*)(p + 0x1C));
        ADXPD_SetDly(*(void**)(p + 0x08), &dly1, &dly2);
        ADX_DecodeInfoExLoop(data, (s32)out_encoding,
            p + 0x20, p + 0x24, p + 0x26, p + 0x28,
            p + 0x2C, p + 0x30, p + 0x34);
        ADX_DecodeInfoAinf(data, (s32)out_encoding,
            p + 0xC4, p + 0xC8, p + 0xD8, p + 0xDA);
    }

    *(s32*)(p + 0x50) = (s8)p[0x0E];
    *(s32*)(p + 0x54) = (s8)p[0x0F];
    *(s32*)(p + 0x58) = *(u32*)(p + 0x10);
    *(s32*)(p + 0x5C) = *(u32*)(p + 0x3C);
    *(s32*)(p + 0x60) = *(u32*)(p + 0x40);
    *(s32*)(p + 0x64) = *(u32*)(p + 0x44);
    *(u32*)(p + 0x8C) = 0;

    return highPassVal;
}

void ADXB_SetDefFmt(void* self) {
    *(s16*)((u8*)self + 0x98) = 0;
}

void ADXB_SetDefPrm(void* self);
void criware_eu_8038A864(void* self) {
    *(s16*)((u8*)self + 0x98) = *(s16*)((u8*)self + 0x9a);
    ADXB_SetDefPrm(self);
}

void ADXB_SetDefPrm(void* self) {
    u8* p = (u8*)self;
    u32 v3c = *(u32*)(p + 0x3c);
    u32 v40 = *(u32*)(p + 0x40);
    u32 v44 = *(u32*)(p + 0x44);

    *(u16*)(p + 0x02) = 1;
    *(u32*)(p + 0x14) = 48000;
    *(u8*)(p + 0x0e) = 2;
    *(u8*)(p + 0x0d) = 16;
    *(u32*)(p + 0x18) = 0x7fffffff;
    *(u8*)(p + 0x0f) = 0x7f;
    *(u32*)(p + 0x10) = 0x400;
    *(u32*)(p + 0x50) = 2;
    *(u32*)(p + 0x54) = 0x7f;
    *(u32*)(p + 0x58) = 0x400;
    *(u32*)(p + 0x5c) = v3c;
    *(u32*)(p + 0x60) = v40;
    *(u32*)(p + 0x64) = v44;
    *(u32*)(p + 0x8c) = 0;
    *(u16*)(p + 0x1c) = 0;
    *(u16*)(p + 0x24) = 0;
    *(u16*)(p + 0x26) = 0;
    *(u32*)(p + 0x20) = 0;
    *(u32*)(p + 0x28) = 0;
    *(u32*)(p + 0x2c) = 0;
    *(u32*)(p + 0x30) = 0;
    *(u32*)(p + 0x34) = 0;
    *(u32*)(p + 0x88) = 0;
    *(u8*)(p + 0xec) = 0;
    *(u8*)(p + 0xed) = 0;
}

s32 ADXB_DecodeHeader(void* adxb, void* data, void* out_encoding) {
    u8* p = (u8*)adxb;
    u8* d = (u8*)data;
    u16 magic;

    *(u32*)(p + 0xC4) = 0;
    *(s16*)(p + 0xD8) = 0;
    *(s16*)(p + 0xDA) = -128;
    *(s16*)(p + 0xDC) = -128;
    memset(p + 0xC8, 0, 16);

    magic = (u16)((d[0] << 8) | d[1]);

    if (magic == 0x8000) {
        return ADXB_DecodeHeaderAdx(adxb, data, out_encoding);
    }
    if (ADXB_CheckSpsd(data)) {
        return ADXB_DecodeHeaderSpsd(adxb, data, out_encoding);
    }
    if (ADXB_CheckWav(data)) {
        return ADXB_DecodeHeaderWav(adxb, data, out_encoding);
    }
    if (ADXB_CheckAiff(data)) {
        return ADXB_DecodeHeaderAiff(adxb, data, out_encoding);
    }
    if (ADXB_CheckAu(data)) {
        return ADXB_DecodeHeaderAu(adxb, data, out_encoding);
    }
    return -1;
}

void ADXB_EntryGetWrFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x78) = func;
    *(void**)((u8*)self + 0x7c) = ctx;
}

u32 ADXB_GetPcmBuf(void* self) { return *(u32*)((u8*)self + 0x3c); }

s16 ADXB_GetFormat(void* self) { return *(s16*)((u8*)self + 0x98); }

u32 ADXB_GetSfreq(void* self) { return *(u32*)((u8*)self + 0x14); }

s32 ADXB_GetNumChan(void* self) {
    int ch = *(u8*)((u8*)self + 0x0E);
    if (ch == 1 && *(u32*)((u8*)self + 0xE0) != 0)
        return 2;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_80517330 + 0x81);
        return -1;
    }
    return (s8)ch;
}

s16 ADXB_GetFmtBps(void* self) {
    return (s8)*(u8*)((u8*)self + 0x0D);
}

s32 ADXB_GetOutBps(void* self) {
    s16 val = *(s16*)((u8*)self + 0x98);
    if (val == 0) return 0x10;
    if (val == 2) {
        s16 sub = *(s16*)((u8*)self + 0x9c);
        if (sub == 2) return 4;
        if (sub == 1) return 8;
        return 0x10;
    }
    if (val == 1) {
        s16 sub = *(s16*)((u8*)self + 0x9c);
        if (sub == 2) return 4;
        return 0x10;
    }
    return 0x10;
}

u32 ADXB_GetBlkSmpl(void* self) { return *(u32*)((u8*)self + 0x10); }

u32 ADXB_GetTotalNumSmpl(void* self) { return *(u32*)((u8*)self + 0x18); }

s16 ADXB_GetNumLoop(void* self) { return *(s16*)((u8*)self + 0x24); }

u32 ADXB_GetLpStartPos(void* self) { return *(u32*)((u8*)self + 0x28); }

u32 ADXB_GetLpStartOfst(void* self) {
    if (self == NULL) return 0;
    return *(u32*)((u8*)self + 0x2c);
}

u32 ADXB_GetLpEndPos(void* self) { return *(u32*)((u8*)self + 0x30); }

u32 ADXB_GetLpEndOfst(void* self) { return *(u32*)((u8*)self + 0x34); }

u32 ADXB_GetAinfLen(void* self) { return *(u32*)((u8*)self + 0xc4); }

s16 ADXB_GetDefOutVol(void* self) { return *(s16*)((u8*)self + 0xd8); }

s16 ADXB_GetDefPan(void* self, u32 idx) {
    return *(s16*)((u8*)self + 0xda + idx * 2);
}

void ADXB_TakeSnapshot(void* self) {
    ADXPD_GetDly(*(void**)((u8*)self + 0x08), (u8*)self + 0xb0, (u8*)self + 0xb4);
    ADXPD_GetExtPrm(*(void**)((u8*)self + 0x08), (u8*)self + 0xa6, (u8*)self + 0xa8, (u8*)self + 0xaa);
}

void ADXB_RestoreSnapshot(void* self) {
    ADXPD_SetDly(*(void**)((u8*)self + 0x08), (u32*)((u8*)self + 0xB0), (u32*)((u8*)self + 0xB4));
    ADXPD_SetExtPrm(*(void**)((u8*)self + 0x08),
        *(s16*)((u8*)self + 0xA6),
        *(s16*)((u8*)self + 0xA8),
        *(s16*)((u8*)self + 0xAA));
}

void ADXB_SetLnkSw(void* self, int val) {
    if (val == 0) {
        *(u32*)((u8*)self + 0xAC) = 0;
    } else {
        *(u32*)((u8*)self + 0xAC) = 1;
    }
}

u32 ADXB_GetStat(void* self) { return *(u32*)((u8*)self + 0x4); }

void ADXB_EntryData(void* self, void* data, int size) {
    if (*(s16*)((u8*)self + 0x98) == 0) {
        *(void**)((u8*)self + 0x48) = data;
        int ch = (s8)*(u8*)((u8*)self + 0x0F);
        *(u32*)((u8*)self + 0x74) = 0;
        *(u32*)((u8*)self + 0x4C) = size / ch;
    } else {
        *(void**)((u8*)self + 0x48) = data;
        int t = (s8)*(u8*)((u8*)self + 0x0D) / 8;
        *(u32*)((u8*)self + 0x74) = 0;
        *(u32*)((u8*)self + 0x4C) = size / (t * (s8)*(u8*)((u8*)self + 0x0E));
    }
    *(u32*)((u8*)self + 0x90) = 0;
    *(u32*)((u8*)self + 0x94) = 0;
    *(u32*)((u8*)self + 0xF4) = 0;
    *(u32*)((u8*)self + 0xF0) = 0;
}

void ADXB_Start(void* self) {
    if (*(u32*)((u8*)self + 0x04) != 0) return;
    *(u32*)((u8*)self + 0x04) = 1;
}

void ADXB_Stop(void* self) {
    if (*(u32*)((u8*)self + 0xE0) != 0) {
        lbl_eu_805E5364(self);
    }
    ADXPD_Stop(*(void**)((u8*)self + 0x08));
    *(u32*)((u8*)self + 0x04) = 0;
}

void ADXB_Reset(void* self) {
    if (*(int*)((u8*)self + 0x04) == 3) {
        ADXPD_Reset(*(void**)((u8*)self + 0x08));
        *(u32*)((u8*)self + 0x8C) = 0;
        *(u32*)((u8*)self + 0x04) = 0;
    }
}

u32 ADXB_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x94); }

u32 ADXB_GetDecNumSmpl(void* self) { return *(u32*)((u8*)self + 0x90); }

void ADXB_EvokeDecode(void* adxb) {
    s32 blkSize = *(s32*)((u8*)adxb + 0x58);
    s32 wrPos = *(s32*)((u8*)adxb + 0x70);
    s32 pcmOfst = *(s32*)((u8*)adxb + 0x68);
    s32 pcmBufSize = *(s32*)((u8*)adxb + 0x60);
    s32 blkSmpl = *(s32*)((u8*)adxb + 0x50);
    s32 maxBlks = *(s32*)((u8*)adxb + 0x4C);
    s32 availWrPos = *(s32*)((u8*)adxb + 0x6C);
    s32 tmp;
    s32 numBlks;
    s32 endAdj;
    s32 decodeEnd;
    s32 deSize;
    s32 availBlks;

    tmp = (wrPos + blkSize) - 1;
    numBlks = tmp / blkSize;
    endAdj = (blkSize - 1) - (tmp % blkSize);
    decodeEnd = (pcmBufSize - pcmOfst + blkSize - 1) / blkSize;
    deSize = decodeEnd * blkSize;
    maxBlks = maxBlks / blkSmpl;

    if (numBlks < decodeEnd) {
        if (pcmOfst + deSize - endAdj < pcmBufSize) {
            decodeEnd++;
        }
    }

    if (wrPos < availWrPos) {
        availBlks = (availWrPos + endAdj) / blkSize;
        if (maxBlks > availBlks) maxBlks = availBlks;
    }
    if (maxBlks > numBlks) maxBlks = numBlks;
    if (maxBlks > decodeEnd) maxBlks = decodeEnd;

    if (blkSmpl == 2) {
        void* pd = *(void**)((u8*)adxb + 0x08);
        ADXPD_EntrySte(pd, *(void**)((u8*)adxb + 0x48), maxBlks << 1,
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1)),
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1) + (*(u32*)((u8*)adxb + 0x64) << 1)));
        ADXPD_Start(pd);
    } else if (*(s32*)((u8*)adxb + 0xE0) != 0) {
        void* pd = *(void**)((u8*)adxb + 0x08);
        ADXPD_EntryPl2(pd, *(void**)((u8*)adxb + 0x48), maxBlks,
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1)),
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1) + (*(u32*)((u8*)adxb + 0x64) << 1)));
        ADXPD_Start(pd);
    } else {
        void* pd = *(void**)((u8*)adxb + 0x08);
        ADXPD_EntryMono(pd, *(void**)((u8*)adxb + 0x48),
            (void*)(*(u32*)((u8*)adxb + 0x5C) + (*(u32*)((u8*)adxb + 0x68) << 1)),
            NULL);
        ADXPD_Start(pd);
    }
}

void ADXB_EndDecode(void* adxb) {
    u8* p = (u8*)adxb;
    s32 blkSize = *(s32*)(p + 0x58);
    s32 wrPos = *(s32*)(p + 0x70);
    s32 numBlkSmpl = *(s32*)(p + 0x54);
    u32 pcmOfst = *(u32*)(p + 0x68);
    u32 pcmBase = *(u32*)(p + 0x5C);
    s32 pcmBufSize = *(s32*)(p + 0x40);
    u32 workBufSize = *(u32*)(p + 0x44);
    s32 tmp;
    s32 endAdj2;
    s32 numBlks;
    s32 numBlkPD;
    s32 decSmpl;
    s32 decDtLen;
    s32 pcmEnd;
    s32 samplesLeft;
    int i;

    tmp = wrPos + blkSize - 1;
    endAdj2 = (blkSize - 1) - (tmp % blkSize);
    numBlks = tmp / blkSize;

    numBlkPD = ADXPD_GetNumBlk(*(void**)(p + 0x08));
    decSmpl = (numBlkPD * blkSize) / numBlkSmpl;
    if (numBlks * (s32)(*(u32*)(p + 0x50)) <= numBlkPD) {
        decSmpl -= endAdj2;
    }
    decDtLen = numBlkPD * numBlkSmpl;

    *(u32*)(p + 0x90) = (u32)decSmpl;
    *(u32*)(p + 0x94) = (u32)decDtLen;

    pcmEnd = pcmOfst + decSmpl;
    if (pcmEnd < pcmBufSize) return;

    samplesLeft = pcmEnd - pcmBufSize;

    if (*(s32*)(p + 0x50) == 2 || *(s32*)(p + 0xE0) != 0) {
        {
            s16* d = (s16*)pcmBase;
            const s16* s = (const s16*)(pcmBase + (pcmBufSize << 1));
            s32 n = samplesLeft;
            while (n > 0) {
                *d++ = *s++;
                n--;
            }
        }
        {
            s16* d = (s16*)(pcmBase + (workBufSize << 1));
            const s16* s = (const s16*)(pcmBase + ((workBufSize + pcmBufSize) << 1));
            s32 n = samplesLeft;
            while (n > 0) {
                *d++ = *s++;
                n--;
            }
        }
    } else {
        s16* d = (s16*)pcmBase;
        const s16* s = (const s16*)(pcmBase + (pcmBufSize << 1));
        s32 n = samplesLeft;
        while (n > 0) {
            *d++ = *s++;
            n--;
        }
    }
}

void ADXB_ExecOneAdx(void* adxb) {
    u8* p = (u8*)adxb;

    if (*(int*)(p + 0x04) == 1) {
        if (ADXPD_GetStat(*(void**)(p + 0x08)) == 0) {
            void (*getWr)(void*, u32*, u32*, u32*);
            void* ctx;

            getWr = *(void (**)(void*, u32*, u32*, u32*))(p + 0x78);
            ctx = *(void**)(p + 0x7C);
            getWr(ctx, (u32*)(p + 0x68), (u32*)(p + 0x6C), (u32*)(p + 0x70));
            ADXB_EvokeDecode(adxb);
            *(int*)(p + 0x04) = 2;
        }
    }

    if (*(int*)(p + 0x04) == 2) {
        ADXPD_ExecHndl(*(void**)(p + 0x08));
        if (ADXPD_GetStat(*(void**)(p + 0x08)) == 3) {
            if (*(int*)(p + 0xE0) != 0) {
                void* pd2 = *(void**)(p + 0x08);
                int i;
                ADXCRS_Lock();
                for (i = 0; i < (int)(*(u32*)((u8*)pd2 + 0x10) << 5); i++) {
                    void* buf1 = *(void**)((u8*)pd2 + 0x20);
                    void* buf2 = *(void**)((u8*)pd2 + 0x24);
                    lbl_eu_805E5360(adxb,
                        *(s16*)((u8*)buf1 + (i << 1)),
                        (u8*)buf1 + (i << 1), (u8*)buf2 + (i << 1));
                }
                ADXCRS_Unlock();
            }
            ADXB_EndDecode(adxb);
            ADXPD_Reset(*(void**)(p + 0x08));
            {
                void (*eosCb)(void*, u32, u32);
                void* eosCtx;
                eosCb = *(void (**)(void*, u32, u32))(p + 0x80);
                eosCtx = *(void**)(p + 0x84);
                eosCb(eosCtx, *(u32*)(p + 0x94), *(u32*)(p + 0x90));
            }
            *(int*)(p + 0x04) = 3;
        }
    }
}

void ADXB_ExecHndl(void* adxb) {
    u8* p = (u8*)adxb;
    s16 fmt = *(s16*)(p + 0x98);

    if (fmt == 0) {
        ADXB_ExecOneAdx(adxb);
    } else if (fmt == 0xA) {
        ADXB_ExecOneAhx(adxb);
    } else if (fmt == 2) {
        ADXB_ExecOneSpsd(adxb);
    } else if (fmt == 3) {
        ADXB_ExecOneAiff(adxb);
    } else if (fmt == 4) {
        ADXB_ExecOneAu(adxb);
    } else if (fmt == 1) {
        ADXB_ExecOneWav(adxb);
    }

    {
        void (*wrCb)(void*, s32, s32);
        wrCb = *(void (**)(void*, s32, s32))(p + 0xF8);
        if (wrCb != NULL) {
            s32 prevLen = *(s32*)(p + 0xF0);
            s32 dataLen = *(s32*)(p + 0x94);
            s32 decodedSmpl = *(s32*)(p + 0x90);
            s32 diff = dataLen - prevLen;
            if (diff < 0) {
                diff = (0x7FFFFFFF - prevLen) + dataLen;
            }
            s8 chan = (s8)*(u8*)(p + 0x0E);
            wrCb(*(void**)(p + 0xFC), diff, chan * (decodedSmpl << 1));
            *(u32*)(p + 0xF0) = *(u32*)(p + 0x94);
        }
    }
}
