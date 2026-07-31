// AHX SJD (Streaming Jingle Decoder)
// libs/CriWare/src/adx/ahx/ahx_sjd.c

/* "libs/CriWare/src/adx/ahx/ahx_sjd.c" line 3 "harness_catalog.h" */
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
/* "libs/CriWare/src/adx/ahx/ahx_sjd.c" line 4 "string.h" */
#ifndef MSL_STRING_H
#define MSL_STRING_H

/* "libs/PowerPC_EABI_Support/include/stl/string.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif

/* "libs/PowerPC_EABI_Support/include/stl/string.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/string_api.h" */
#ifndef _MSL_STRING_API_H
#define _MSL_STRING_API_H

#ifdef __cplusplus
extern "C" {
#endif

void* __memrchr(const void* src, int val, size_t n);

#ifdef __cplusplus
}
#endif

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/string_api.h" */
/* "libs/PowerPC_EABI_Support/include/stl/string.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/extras.h" */
#ifndef _EXTRAS_H
#define _EXTRAS_H
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/extras.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

int stricmp(const char*, const char*);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/extras.h" */

char* strcpy(char*, const char*);
char* strncpy(char*, const char*, size_t);

char* strcat(char*, const char*);
char* strncat(char*, const char*, size_t);

int strcmp(const char*, const char*);
int strncmp(const char*, const char*, size_t);

char* strchr(const char*, int);
char* strstr(const char*, const char*);

size_t strlen(const char*);

void* memmove(void*, const void*, size_t);
int memcmp(const void*, const void*, size_t);
void* memchr(const void*, int, size_t);

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, int val, size_t count);

#ifdef __cplusplus
}
#endif
#endif
/* end "string.h" */

typedef struct AHXSJD {
    /*0x00*/ void* dcd;
    /*0x04*/ void* bsr;
    /*0x08*/ u8 initialized;
    /*0x09*/ s8 status;
    /*0x0A*/ u8 numChannels;
    /*0x0B*/ u8 pad;
    /*0x0C*/ u32 termSupply;
    /*0x10*/ void* streamObj;
    /*0x14*/ u32 chanInfo[8];
    /*0x34*/ u32 decSmpl;
    /*0x38*/ u32 lnkSw;
    /*0x3C*/ u32 decSmplLim;
    /*0x40*/ u32 outSmplOfst;
    /*0x44*/ u32 outSmplTotal;
    /*0x48*/ u32 pad2;
    /*0x4C*/ u32 decCallback;
    /*0x50*/ u32 decCallbackPrm;
    /*0x54*/ void* fltFunc;
    /*0x58*/ void* fltCtx;
} AHXSJD;

extern volatile s32 lbl_eu_805E64C0;

extern void AHXTBL_GetAtblInfo(u32*, u32*);
extern void AHXTBL_GetMtblInfo(u32*, u32*);
extern void AHXTBL_GetFtblInfo(u32*, u32*);
extern void AHXTBL_GetWtblInfo(u32*, u32*);
extern void AHXSJD_SetupAtbl(u32, u32);
extern void AHXSJD_SetupMtbl(u32, u32);
extern void AHXSJD_SetupFtbl(u32, u32);
extern void AHXSJD_SetupWtbl(u32, u32);
extern void AHXDCD_Init(void);
extern void AHXDCD_Finish(void);
extern void* AHXBSR_Create(void*, void*, s32);
extern void AHXBSR_Destroy(void*);
extern void AHXBSR_Restruct(void*, void*);
extern void* AHXDCD_Create(void*, s32);
extern void AHXDCD_Destroy(void*);
extern void AHXDCD_Reset(void*);
extern void AHXDCD_SetBsr(void*, void*);
extern void AHXDCD_DecodeHeader(void*);
extern s32 AHXDCD_DecodeFrmHdr(void*);
extern s32 AHXDCD_IsEof(void*);
extern s32 AHXDCD_GetOutSmpl(void*);
extern s32 AHXDCD_GetOutBps(void*);
extern s32 AHXDCD_DecodeData(void*, void*, s32, s32);
extern s32 AHXDCD_GetTotalNumSmpl(void*);
extern void AHXDCD_SetExtPrm(void*);
extern s32 AHXBSR_Tell(void*);

void AHXSJD_Init(void) {
    u32 info[2];

    if (lbl_eu_805E64C0 == 0) {
        AHXTBL_GetAtblInfo(&info[0], &info[1]);
        AHXSJD_SetupAtbl(info[0], info[1]);
        AHXTBL_GetMtblInfo(&info[0], &info[1]);
        AHXSJD_SetupMtbl(info[0], info[1]);
        AHXTBL_GetFtblInfo(&info[0], &info[1]);
        AHXSJD_SetupFtbl(info[0], info[1]);
        AHXTBL_GetWtblInfo(&info[0], &info[1]);
        AHXSJD_SetupWtbl(info[0], info[1]);
        AHXDCD_Init();
    }
    ++lbl_eu_805E64C0;
}

void AHXSJD_Finish(void) {
    if (lbl_eu_805E64C0 == 1) {
        AHXDCD_Finish();
    }
    --lbl_eu_805E64C0;
}

void* AHXSJD_Create(void* allocator, s32 numChannels, void* chanInfo, s32 bufSize, s32 extraSize) {
    AHXSJD* sjd;
    void* bsr;
    void* dcd;
    s32 sjdSize, bsrEnd, dcdEnd;
    s32 i;

    sjdSize = ROUND_UP(bufSize, 8);
    sjd = (AHXSJD*)sjdSize;
    memset(sjd, 0, 0x5C);

    bsrEnd = ROUND_UP(sjdSize + 0x63, 8) + 0x34;
    if (bsrEnd > bufSize + extraSize) {
        return NULL;
    }

    bsr = AHXBSR_Create(allocator, (void*)sjdSize, 0x34);
    if (bsr == NULL) {
        return NULL;
    }
    sjd->bsr = bsr;

    dcdEnd = ROUND_UP(bsrEnd + 7, 8) + 0x1BF0;
    if (dcdEnd > bufSize + extraSize) {
        AHXBSR_Destroy(bsr);
        return NULL;
    }

    dcd = AHXDCD_Create((void*)ROUND_UP(bsrEnd + 7, 8), 0x1BF0);
    if (dcd == NULL) {
        AHXBSR_Destroy(bsr);
        return NULL;
    }
    sjd->dcd = dcd;

    sjd->numChannels = numChannels;
    sjd->streamObj = allocator;

    // Copy channel info
    if (numChannels > 0) {
        u32* src = (u32*)chanInfo;
        u32* dst = (u32*)&sjd->chanInfo[0];

        // Unrolled copy for 8+ channels
        if (numChannels > 8) {
            s32 count = numChannels - 8;
            s32 idx = 0;
            for (i = 0; i < count; i += 8) {
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
                dst[4] = src[4];
                dst[5] = src[5];
                dst[6] = src[6];
                dst[7] = src[7];
                src += 8;
                dst += 8;
                idx += 8;
            }
            // Copy remaining
            for (i = idx; i < numChannels; i++) {
                *dst++ = *src++;
            }
        } else {
            for (i = 0; i < numChannels; i++) {
                *dst++ = *src++;
            }
        }
    }

    sjd->status = 0;
    sjd->termSupply = 0;
    sjd->outSmplOfst = 0;
    sjd->outSmplTotal = 0;
    sjd->decSmpl = 0x7FFFFFFF;
    sjd->decSmplLim = -1;
    sjd->decCallback = 0;
    sjd->decCallbackPrm = 0;
    sjd->fltFunc = NULL;
    sjd->fltCtx = NULL;
    sjd->initialized = 1;

    return sjd;
}

void AHXSJD_Destroy(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    void* bsr;
    void* dcd;

    if (sjd == NULL) return;

    bsr = sjd->bsr;
    if (bsr != NULL) {
        sjd->bsr = NULL;
        AHXBSR_Destroy(bsr);
    }

    dcd = sjd->dcd;
    if (dcd != NULL) {
        sjd->dcd = NULL;
        AHXDCD_Destroy(dcd);
    }

    memset(sjd, 0, 0x5C);
}

s32 AHXSJD_GetStat(void* self) {
    return ((AHXSJD*)self)->status;
}

void AHXSJD_SetInSj(void* self, void* val) {
    ((AHXSJD*)self)->streamObj = val;
}

void AHXSJD_Start(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    void* stream;

    sjd->termSupply = 0;
    sjd->outSmplOfst = 0;
    sjd->outSmplTotal = 0;
    sjd->decSmpl = 0x7FFFFFFF;
    sjd->decSmplLim = -1;
    sjd->decCallback = 0;
    sjd->decCallbackPrm = 0;

    stream = sjd->streamObj;
    AHXBSR_Restruct(sjd->bsr, stream);
    AHXDCD_SetBsr(sjd->dcd, sjd->bsr);
    sjd->status = 1;
}

void AHXSJD_Stop(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    AHXDCD_Reset(sjd->dcd);
    sjd->status = 0;
}

void criware_8038CB9C(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    void* dcd = sjd->dcd;
    s32 outSmpl = AHXDCD_GetOutSmpl(dcd);
    s32 outBps = AHXDCD_GetOutBps(dcd);
    s32 bytesPerSmpl = outBps >> 3;
    void* stream = sjd->streamObj;

    u32** vt = *(u32***)stream;
    s32 avail = ((s32 (*)(void*, s32))vt[9])(stream, 0);
    s32 needed = avail / bytesPerSmpl;

    if (needed < outSmpl) {
        return;
    }

    if (AHXDCD_IsEof(dcd)) {
        s32 result = AHXDCD_DecodeFrmHdr(dcd);
        if (result == 1) {
            if (sjd->lnkSw == 0) {
                sjd->status = 3;
                return;
            }
            sjd->outSmplTotal += sjd->outSmplOfst;
            sjd->outSmplOfst = 0;
            return;
        }
        if (result == -1) {
            return;
        }
    }

    u32 outBuf[4];
    vt = *(u32***)sjd->streamObj;
    ((void (*)(void*, s32, u32*, s32))vt[6])(sjd->streamObj, 0, outBuf, outSmpl * bytesPerSmpl);

    s32 actualSmpl = outBuf[1] / bytesPerSmpl;
    if (outSmpl != actualSmpl) {
        vt = *(u32***)sjd->streamObj;
        ((void (*)(void*, s32, u32*))vt[7])(sjd->streamObj, 0, outBuf);
        return;
    }

    s32 decoded = AHXDCD_DecodeData(dcd, (void*)outBuf[0], 0, outSmpl);

    s32 maxDec = 0x1E0;
    s32 decCount = (sjd->outSmplOfst >= maxDec) ? decoded : (decoded & ~(decoded >> 31));

    s32 totalSmpl = AHXDCD_GetTotalNumSmpl(dcd);
    if (sjd->outSmplOfst + decCount > totalSmpl) {
        decCount = totalSmpl - sjd->outSmplOfst;
    }

    s32 outBytes = decCount * bytesPerSmpl;
    if (outBuf[1] > outBytes) {
        outBuf[1] = outBytes;
    }

    u32 tmpBuf[2];
    tmpBuf[0] = outBuf[0];
    tmpBuf[1] = outBuf[1];

    if (sjd->fltFunc != NULL && decCount > 0) {
        ((void (*)(void*, s32, u32))sjd->fltFunc)(sjd->fltCtx, 0, tmpBuf[0]);
    }

    vt = *(u32***)sjd->streamObj;
    ((void (*)(void*, s32, u32*))vt[8])(sjd->streamObj, 1, outBuf);

    vt = *(u32***)sjd->streamObj;
    ((void (*)(void*, s32, u32*))vt[7])(sjd->streamObj, 0, tmpBuf);

    sjd->outSmplOfst += decoded;

    s32 bsrPos = AHXBSR_Tell(sjd->bsr);
    s32 bytes = (bsrPos + 7) >> 3;
    u32 decSmplLim = sjd->decSmplLim;

    sjd->decSmpl = bytes;
    sjd->outSmplOfst += decCount;
    sjd->outSmplTotal += decoded;
    sjd->decCallback += decoded;

    if (decSmplLim > 0 && sjd->decCallback >= decSmplLim) {
        if (sjd->decCallbackPrm != 0) {
            ((void (*)(u32))sjd->decCallbackPrm)(sjd->decCallback);
        }
    }

    if (sjd->lnkSw == 0 && sjd->outSmplOfst >= totalSmpl) {
        sjd->status = 3;
    }
}

void AHXSJD_ExecHndl(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    s32 status = sjd->status;

    if (status == 2) {
        criware_8038CB9C(sjd);
    } else if (status == 1) {
        void* stream = sjd->streamObj;
        u32** vt = *(u32***)stream;
        s32 avail = ((s32 (*)(void*, s32))vt[9])(stream, 1);
        if (avail >= 0x24) {
            AHXDCD_DecodeHeader(sjd->dcd);
            AHXDCD_DecodeFrmHdr(sjd->dcd);
            sjd->status = 2;
        }
    }
}

u32 AHXSJD_GetDecDtLen(void* self) {
    return ((AHXSJD*)self)->decSmpl;
}

u32 AHXSJD_GetDecNumSmpl(void* self) {
    AHXSJD* sjd = (AHXSJD*)self;
    return sjd->outSmplOfst + sjd->outSmplTotal;
}

void AHXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    ((AHXSJD*)self)->fltFunc = func;
    ((AHXSJD*)self)->fltCtx = ctx;
}

void AHXSJD_SetExtPrm(void* self) {
    AHXDCD_SetExtPrm(((AHXSJD*)self)->dcd);
}

void AHXSJD_SetDecSmpl(void* self, u32 val) {
    ((AHXSJD*)self)->decSmpl = val;
}

void AHXSJD_TermSupply(void* self) {
    ((AHXSJD*)self)->termSupply = 1;
}

void AHXSJD_SetLnkSw(void* self, s32 val) {
    if (val == 0) {
        ((AHXSJD*)self)->lnkSw = 0;
    } else {
        ((AHXSJD*)self)->lnkSw = 1;
    }
}
