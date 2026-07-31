// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdcre
// Replace stubs with high-level C/C++ during decomp.

/* "libs/CriWare/src/sofdec/mwply/mwsfdcre.c" line 3 "harness_catalog.h" */
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

/* ---- Shared types for the Sofdec creator (MWSFCRE) ---- */

typedef struct TraceCb TraceCb;
typedef struct TraceCbVtable {
    u8 pad_0x00[0x24];
    void (*trace)(TraceCb* self, void* rec); /* 0x24 */
} TraceCbVtable;
struct TraceCb {
    const TraceCbVtable* vtable;
};

/* Per-function trace record (lbl_eu_80566*); entry sub-record at +0x04,
 * exit sub-record at +0x6c. */
typedef struct TraceRec {
    const char* name;   /* 0x00 */
    u32 field_0x04;     /* 0x04 */
    u32 field_0x08;     /* 0x08 */
    u32 self;           /* 0x0c */
    u32 field_0x10;     /* 0x10 */
    u32 field_0x14;     /* 0x14 */
    u32 arg;            /* 0x18 */
    u8 pad_0x1c[0x50];  /* 0x1c..0x6b */
    u32 field_0x6c;     /* 0x6c */
    u8 pad_0x70[0x4];   /* 0x70..0x73 */
    u32 field_0x74;     /* 0x74 */
} TraceRec;

/* Handle objects with a vtable exposing destroy at +0x0c (SJRBF/SJUNI). */
typedef struct HnObjVtable {
    u8 pad_0x00[0x0C];
    void (*destroy)(void* self);
} HnObjVtable;
typedef struct HnObj {
    HnObjVtable* vtable;
} HnObj;

/* MWSFDPLY movie-player handle. Partial layout covering the fields touched by
 * the creator helpers in this TU. */
typedef struct MWSFDPLY {
    u32 enable;             /* 0x00 */
    u32 status;             /* 0x04 */
    s32 width;              /* 0x08 */
    s32 height;             /* 0x0c */
    u32 frameCount;         /* 0x10 */
    u32 field_0x14;         /* 0x14 */
    s32 field_0x18;         /* 0x18 */
    u32 field_0x1c;         /* 0x1c */
    u32 field_0x20;         /* 0x20 */
    u32 field_0x24;         /* 0x24 */
    s32 poolCount;          /* 0x28 */
    s32 poolSize;           /* 0x2c */
    void** pool;            /* 0x30 */
    u8 pad_0x34[0x24];      /* 0x34..0x57 */
    void* sfd;              /* 0x58 */
    void* stm;              /* 0x5c */
    u8 pad_0x60[0x4];       /* 0x60..0x63 */
    void* lsc;              /* 0x64 */
    u8 pad_0x68[0x60];      /* 0x68..0xc7 */
    void* sfx;              /* 0xc8 */
    void* compoWork;        /* 0xcc */
    u32 compoWorkSize;      /* 0xd0 */
    u8 pad_0xd4[0x3E8];     /* 0xd4..0x4bb */
    void* picUsrTgt;        /* 0x4bc */
    u8 pad_0x4c0[0x4];      /* 0x4c0..0x4c3 */
    void* field_0x4c4;      /* 0x4c4 */
    u32 field_0x4c8;        /* 0x4c8 */
    u8 pad_0x4cc[0x34];     /* 0x4cc..0x4ff */
    u32 sj;                 /* 0x500 */
    HnObj* rbf;             /* 0x504 */
    u32 field_0x508;        /* 0x508 */
    u32 field_0x50c;        /* 0x50c */
    u32 field_0x510;        /* 0x510 */
    u32 field_0x514;        /* 0x514 */
    u32 field_0x518;        /* 0x518 */
    u32 field_0x51c;        /* 0x51c */
    u32 field_0x520;        /* 0x520 */
    HnObj* uni;             /* 0x524 */
    u8 pad_0x528[0xB0];     /* 0x528..0x5d7 */
    u8 field_0x5d8[0x5C];   /* 0x5d8..0x633 (SstHn sub-objects) */
    u32* field_0x634;       /* 0x634 */
} MWSFDPLY;

/* SFD creation parameter block shared by the frame-buffer allocators. */
typedef struct MWSFCRE_Para {
    u8 pad_0x00[0x08];
    s32 width;              /* 0x08 */
    s32 height;             /* 0x0c */
    s32 frameCount;         /* 0x10 */
    u8 pad_0x14[0x10];      /* 0x14..0x23 */
    s32 mode;               /* 0x24 */
    s32 outer;              /* 0x28 (outer frame pool mode) */
    s32 poolSize;           /* 0x2c */
    void* pool;             /* 0x30 */
} MWSFCRE_Para;

/* SFD_SetSupplySj argument block. */
typedef struct MWSFCRE_SjArg {
    u32 a;                  /* 0x00 */
    u32 b;                  /* 0x04 */
    u32 c;                  /* 0x08 */
    u32 d;                  /* 0x0c */
    u32 e;                  /* 0x10 */
    u32 f;                  /* 0x14 */
} MWSFCRE_SjArg;

/* ---- External symbols / helpers ---- */

extern TraceCb* lbl_eu_805FF3A0;
extern TraceRec lbl_eu_805661AC;
extern TraceRec lbl_eu_80566280;
extern const char lbl_eu_8051A3CC[];

extern void MWSFSVM_Error(const char* message, ...);
extern s32 MWSFLIB_SetErrCode(s32 code);
extern void* MWSFLIB_GetLibWorkPtr(void);
extern s32 MWSFD_GetUsePicUsr(void);
extern void MWSFD_SetProhibitServer(s32 val);
extern void MWSFLIB_SfdErrFunc(u32 a, u32 b);

extern s32 SFD_Stop(void* sfd);
extern s32 SFD_Destroy(void* sfd);
extern s32 SFD_SetSupplySj(void* sfd, MWSFCRE_SjArg* arg);
extern void SFD_SetPicUsrBuf(void* sfd, void* buf, s32 height, s32 count);
extern void SFD_SetCond(void* sfd, u32 cond, u32 val);

extern s32 criware_803C0D94(void* handle, void (*errFn)(u32, u32), u32 errArg);
extern void criware_803A3A48(void* pool);
extern void* criware_803A3AE4(void* pool, u32 size);
extern s32 fn_803A537C(void* self);
extern s32 fn_803A7320(void* self);
extern s32 mwSfdStopDec(void* self);

extern s32 MWSFSFX_CalcHnWorkSiz(u32 a, u32 b);
extern void MWSFSFX_Destroy(void* sfx);
extern s32 MWSFTAG_IsUseAinfSj(void* self);
extern void MWSFTAG_DestroyAinfSj(void* self);
extern void LSC_Destroy(void* lsc);
extern void MWSTM_Destroy(void* stm);
extern void MWSST_Destroy(void* sst);

extern int sprintf(char* buf, const char* fmt, ...);
extern void* memset(void* dst, int val, size_t n);

void* mwPlyCreateSofdec(MWSFDPLY* self);
void mwply_Destroy(MWSFDPLY* self);

/* ---- Target implementations ---- */

s32 mwsfcre_IsOuterFrmPoolUsed(MWSFDPLY* self) {
    s32 poolCount = self->poolCount;
    s32 poolSize = self->poolSize;
    void** pool = self->pool;
    s32 w = self->width;
    s32 h = self->height;
    s32 ok = 1;
    if (poolCount > 0 || poolSize > 0 || pool != NULL) {
        s32 w16 = (w + 15) / 16;
        s32 h16 = (h + 15) / 16;
        s32 W = w16 * 16;
        s32 H = h16 * 16;
        s32 luma = ((w16 >> 27) & 1) + W;
        s32 chroma = ((h16 >> 27) & 1) + H;
        s32 frameSize = ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 +
                        H * (((W + 0x1f) / 32) * 32) + 0x20;
        if (poolSize < frameSize) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0xB9);
            ok = 0;
        }
        if (pool == NULL) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0xDB);
            ok = 0;
        }
        for (s32 i = 0; i < poolCount; i++) {
            if (pool[i] == NULL) {
                MWSFSVM_Error(lbl_eu_8051A3CC + 0xFC);
                ok = 0;
            }
        }
    } else {
        ok = 0;
    }
    return ok;
}

void MWSFCRE_SetSupplySj(MWSFDPLY* self) {
    u32 sj = self->sj;
    void* sfd = self->sfd;
    if (sj != 0) {
        MWSFCRE_SjArg arg;
        if (sj == (u32)self->uni) {
            arg.a = 2;
            arg.b = sj;
            arg.c = 0;
            arg.d = 0;
            arg.e = 0;
            arg.f = 0;
        } else if (sj == (u32)self->rbf) {
            arg.a = 0;
            arg.b = sj;
            arg.c = self->field_0x508;
            arg.d = self->field_0x50c;
            arg.e = self->field_0x510;
            arg.f = 0;
        } else {
            arg.a = self->field_0x514;
            arg.b = sj;
            arg.c = self->field_0x518;
            arg.d = self->field_0x51c;
            arg.e = self->field_0x520;
            arg.f = 0;
        }
        if (self->sj == (u32)self->uni) {
            SFD_SetCond(self->sfd, 0x55, 1);
            SFD_SetCond(self->sfd, 0x56, 0x800);
        } else {
            SFD_SetCond(self->sfd, 0x55, 0);
            SFD_SetCond(self->sfd, 0x56, 4);
        }
        if (SFD_SetSupplySj(sfd, &arg)) {
            MWSFLIB_SetErrCode(-0x138);
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x11D);
        }
    }
}

s32 mwsfcre_MallocRfb(void* pool, MWSFCRE_Para* para, void** out) {
    s32 rc = 0;
    s32 w = para->width;
    s32 h = para->height;
    s32 outer = para->outer;
    s32 w16 = (w + 15) / 16;
    s32 h16 = (h + 15) / 16;
    s32 W = w16 * 16;
    s32 H = h16 * 16;
    s32 luma = ((w16 >> 27) & 1) + W;
    s32 chroma = ((h16 >> 27) & 1) + H;
    s32 frameSize = ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 +
                    H * (((W + 0x1f) / 32) * 32) + 0x20;
    if (outer != 0) {
        if (outer < 2 || para->poolSize < frameSize) {
            out[0] = NULL;
            out[1] = NULL;
            rc = -1;
        } else {
            out[0] = (void*)((u32*)para->pool)[0];
            out[1] = (void*)((u32*)para->pool)[1];
        }
    } else {
        out[0] = criware_803A3AE4(pool, frameSize);
        out[1] = criware_803A3AE4(pool, frameSize);
    }
    if (out[0] == NULL || out[1] == NULL) {
        rc = -1;
    }
    return rc;
}

s32 mwsfcre_MallocTab(void* pool, MWSFCRE_Para* para, void** out) {
    s32 rc = 0;
    s32 mode = para->mode;
    s32 frames = para->frameCount;
    s32 w = para->width;
    s32 h = para->height;
    if ((u32)mode > 3) {
        MWSFSVM_Error(lbl_eu_8051A3CC);
    }
    s32 w16 = (w + 15) / 16;
    s32 h16 = (h + 15) / 16;
    s32 W = w16 * 16;
    s32 H = h16 * 16;
    s32 luma = ((w16 >> 27) & 1) + W;
    s32 chroma = ((h16 >> 27) & 1) + H;
    s32 frameSize = ((chroma >> 1) * (((luma >> 1) + 0x1f) / 32 * 32)) * 2 +
                    H * (((W + 0x1f) / 32) * 32) + 0x20;
    if (para->outer != 0) {
        if (para->outer < frames + 2 || para->poolSize < frameSize) {
            rc = -1;
        } else {
            for (s32 i = 0; i < frames; i++) {
                out[i] = (void*)((u32*)para->pool)[i + 2];
                if (out[i] == NULL) {
                    rc = -1;
                }
            }
        }
    } else {
        for (s32 i = 0; i < frames; i++) {
            out[i] = criware_803A3AE4(pool, frameSize);
            if (out[i] == NULL) {
                rc = -1;
            }
        }
    }
    return rc;
}

#pragma push
#pragma auto_inline off
void mwsfcre_AttachPicUsrBuf(MWSFDPLY* self) {
    void* tgt = self->picUsrTgt;
    if (tgt == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x25F);
    } else {
        s32 h = self->field_0x18;
        s32 count = *(s32*)((u8*)tgt + 0x08);
        s32 size = *(s32*)((u8*)tgt + 0x04);
        void* buf = *(void**)((u8*)tgt + 0x00);
        if (size < (h + 3) * count) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x294);
        } else if (MWSFD_GetUsePicUsr() == 1) {
            SFD_SetPicUsrBuf(self->sfd, buf, h + 3, count);
        }
    }
}
#pragma pop

s32 MWSFCRE_ResetSfdHn(MWSFDPLY* self) {
    void* sfd = self->sfd;
    self->sfd = NULL;
    if (SFD_Stop(sfd)) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x31F);
        return -1;
    }
    self->sfd = sfd;
    if (criware_803C0D94(sfd, MWSFLIB_SfdErrFunc, (u32)self)) {
        MWSFLIB_SetErrCode(-0x12F);
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x34F);
        return -1;
    }
    mwsfcre_AttachPicUsrBuf(self);
    return 0;
}

void* criware_8039FF34(MWSFDPLY* self) {
    if (lbl_eu_805FF3A0 != NULL) {
        char buf[0x200];
        sprintf(buf, lbl_eu_8051A3CC + 0x383, self->enable, self->status,
                self->width, self->height, self->frameCount, self->field_0x14,
                self->field_0x18, self->field_0x1c, self->field_0x20,
                self->field_0x24, self->poolCount, self->poolSize, self->pool);
        TraceCb* cb = lbl_eu_805FF3A0;
        if (cb != NULL) {
            lbl_eu_805661AC.self = (u32)buf;
            cb->vtable->trace(cb, &lbl_eu_805661AC.field_0x04);
        }
    }
    {
        void* hn = mwPlyCreateSofdec(self);
        TraceCb* cb = lbl_eu_805FF3A0;
        if (cb != NULL) {
            lbl_eu_805661AC.field_0x74 = (u32)hn;
            cb->vtable->trace(cb, &lbl_eu_805661AC.field_0x6c);
        }
        return hn;
    }
}

s32 mwsfcre_MallocCompoWork(MWSFDPLY* self) {
    void* tag = (u8*)self + 8;
    s32 size = MWSFSFX_CalcHnWorkSiz(self->frameCount, *(u32*)((u8*)tag + 0x0C));
    void* work = criware_803A3AE4(self, size);
    if (work == NULL) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x572);
        criware_803A3A48(self);
        return -1;
    }
    self->compoWork = work;
    self->compoWorkSize = size;
    if (MWSFTAG_IsUseAinfSj(tag) == 1) {
        void* ainf = criware_803A3AE4(self, 0x20000);
        if (ainf == NULL) {
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x593);
            criware_803A3A48(self);
            return -1;
        }
        self->field_0x4c4 = ainf;
        self->field_0x4c8 = 0x20000;
    } else {
        self->field_0x4c4 = NULL;
        self->field_0x4c8 = 0;
    }
    return 0;
}

void criware_803A09B4(MWSFDPLY* self) {
    fn_803A537C(self);
    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_80566280.self = (u32)self;
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x04);
    }
    mwply_Destroy(self);
    if (lbl_eu_805FF3A0 != NULL) {
        lbl_eu_805FF3A0->vtable->trace(lbl_eu_805FF3A0, &lbl_eu_80566280.field_0x6c);
    }
    *(u32*)((u8*)MWSFLIB_GetLibWorkPtr() + 0x34F4) -= 1;
}

void mwply_Destroy(MWSFDPLY* self) {
    if (self == NULL) {
        return;
    }
    MWSFD_SetProhibitServer(1);
    mwSfdStopDec(self);
    self->enable = 0;
    MWSFD_SetProhibitServer(0);
    MWSFTAG_DestroyAinfSj(self);
    if (self->sfx != NULL) {
        MWSFSFX_Destroy(self->sfx);
    }
    if (self->lsc != NULL) {
        LSC_Destroy(self->lsc);
    }
    if (self->stm != NULL) {
        MWSTM_Destroy(self->stm);
    }
    if (self->rbf != NULL) {
        self->rbf->vtable->destroy(self->rbf);
    }
    if (self->uni != NULL) {
        self->uni->vtable->destroy(self->uni);
    }
    if (self->field_0x634 != NULL) {
        *self->field_0x634 = 0;
    }
    if (self->sfd != NULL) {
        if (SFD_Destroy(self->sfd)) {
            MWSFLIB_SetErrCode(-0x132);
            MWSFSVM_Error(lbl_eu_8051A3CC + 0x301);
        }
    }
    MWSST_Destroy((void*)((u8*)self + 0x5D8));
    MWSST_Destroy((void*)((u8*)self + 0x600));
    criware_803A3A48(self);
    if (fn_803A7320(self)) {
        MWSFSVM_Error(lbl_eu_8051A3CC + 0x5B8);
    }
    memset(self, 0, 0x690);
    self->enable = 0;
}

/* ---- Non-target stubs preserved from the scaffold ---- */

void mwsfcre_CalcWorkStmBuf() {}

void mwPlyCalcWorkCprmSfd() {}

void criware_eu_803A29E0() {}

void mwsfcre_CreateSfd() {}

void MWSFCRE_SetCondSfd() {}

#pragma push
#pragma auto_inline off
void* mwPlyCreateSofdec(MWSFDPLY* self) { return NULL; }
#pragma pop

uint32_t criware_803A0BA8(void* r3_this, uint32_t r4) {
    uint32_t a = *(uint32_t*)((char*)r3_this + 0x54c);
    uint32_t limit = *(uint32_t*)((char*)r3_this + 0x544);
    uint32_t sum = a + r4;
    if (sum > limit) {
        return 0;
    }
    uint32_t b = *(uint32_t*)((char*)r3_this + 0x548);
    *(uint32_t*)((char*)r3_this + 0x54c) = sum;
    *(uint32_t*)((char*)r3_this + 0x548) = b + r4;
    return b;
}

void criware_803A0BDC(void* self) {
    void* work = MWSFLIB_GetLibWorkPtr();
    void (*cb)(void*, void*) = (void(*)(void*, void*))*(void**)((u8*)work + 0x28);
    void* param = *(void**)((u8*)work + 0x30);
    cb(param, self);
}

void criware_803A0C1C() {}

void criware_803A0C20(void* self) {
    void* work = MWSFLIB_GetLibWorkPtr();
    void (*cb)(void*, void*) = (void(*)(void*, void*))*(void**)((u8*)work + 0x2C);
    void* param = *(void**)((u8*)work + 0x30);
    cb(param, self);
}
