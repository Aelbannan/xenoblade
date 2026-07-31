// sfd_buf.c - CriWare Sofdec buffer management unit
// Manages ring buffers, video frame buffers, and audio ring buffers
// for the Sofdec video playback system.

/* "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_buf.c" line 4 "harness_catalog.h" */
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
extern void *SJRBF_Create(void *pool_mem, u32 buf_size, u32 xtr_size);
extern void *SJMEM_Create(void *pool_mem, u32 flags);
extern int SJRBF_GetFlowCnt(void *self, int mode, int reset);
extern int SJMEM_GetBufSize(void *self);
extern u32 SFTRN_IsSetup(void *self, u32 idx);
extern void SFPTS_InitPtsQue(void *self);
extern void SFPTS_ResetPtsQue(void *self);
extern s32 SFLIB_SetErr(void *h, u32 err_code);
extern void SFLIB_LockCs(void *cs);
extern void SFLIB_UnlockCs(void *cs);
extern s32 SFTRN_CallTrtTrif(void *self, int idx, int funcIdx, int *arg4, int arg5);

/* Global buffer object pointers (set by SFBUF_Init) */
extern void *lbl_eu_80606E10; /* ring buffer object */
extern void *lbl_eu_80606E14; /* memory buffer object */

/* Forward declarations for internal functions */
static void sfbuf_InitRingSj(void *self, u32 *cumulative, u32 *sizes,
                              int idx, u32 extraSize);
void sfbuf_InitVfrmBuf(void *self, void *handle, u32 *cumulative,
                        u32 *sizes, int idx);
void sfbuf_InitAringBuf(void *self, u32 *cumulative, u32 *sizes, int idx);
void sfbuf_RingGetSub(void *self, int idx, u32 *out, int mode);
void sfbuf_RingAddSub(void *self, int idx, u32 size, int mode);

/* Buffer constants */
#define SFBUF_MAX_IDX   8
#define SFBUF_BUF_STRIDE 0x74
#define SFBUF_STATE_ACTIVE 5
#define SFBUF_STATE_READY  4
#define SFBUF_ERR_BASE     0xFF000400

/* Per-buffer field offsets (relative to buffer base + idx*0x74) */
#define OFF_STATE       0x00
#define OFF_ACTIVE      0x04
#define OFF_BUFOBJ      0x1C
#define OFF_BUFSIZ      0x18
#define OFF_WTOT        0x20
#define OFF_RTOT        0x24
#define OFF_DLM_PTR     0x28
#define OFF_DLM_SIZE    0x2C

/* Helper: get per-buffer base pointer */
static inline u8 *sfbuf_base(void *self, int idx) {
    return (u8 *)self + idx * SFBUF_BUF_STRIDE;
}

/*
 * SFBUF_Init - Initialize global buffer allocators
 *
 * Creates temporary SJRBF and SJMEM objects to obtain their
 * internal buffer handles, stores them in globals, then destroys
 * the temporary objects.
 */
void SFBUF_Init(void) {
    void *pool[2];
    void *obj;
    void *vtable;

    /* Create ring buffer and get its handle */
    obj = SJRBF_Create(pool, 8, 0);
    vtable = *(void **)obj;
    lbl_eu_80606E10 = ((void *(*)(void *))((void **)vtable)[4])(obj);
    vtable = *(void **)obj;
    ((void (*)(void *))((void **)vtable)[3])(obj);

    /* Create memory buffer and get its handle */
    obj = SJMEM_Create(pool, 8);
    vtable = *(void **)obj;
    lbl_eu_80606E14 = ((void *(*)(void *))((void **)vtable)[4])(obj);
    vtable = *(void **)obj;
    ((void (*)(void *))((void **)vtable)[3])(obj);
}

/*
 * sfbuf_InitRingSj - Initialize a single ring buffer slot
 *
 * Sets up the ring buffer at the given index with the specified
 * buffer size, extra size, and PTS queue initialization.
 */
static void sfbuf_InitRingSj(void *self, u32 *cumulative, u32 *sizes,
                              int idx, u32 extraSize) {
    u8 *p = sfbuf_base(self, idx);
    u32 bufSize = cumulative[idx];
    u32 allocSize;
    void *ringObj;
    int active;
    int state;

    if (bufSize == 0) {
        /* Buffer not allocated - mark inactive */
        active = 0;
        state = 4; /* not active */
    } else {
        u32 actualSize = bufSize - extraSize;
        void *memObj = (void *)cumulative[idx]; /* buffer memory pointer */

        active = 1;
        state = 5; /* active */

        /* Allocate ring buffer */
        if (actualSize <= 0) {
            SFLIB_SetErr(NULL, SFBUF_ERR_BASE + 0x0C);
            allocSize = 0;
        } else {
            ringObj = SJRBF_Create(memObj, actualSize, extraSize);
            if (ringObj == NULL) {
                SFLIB_SetErr(NULL, SFBUF_ERR_BASE + 0x0A);
                allocSize = 0;
            } else {
                allocSize = (u32)ringObj;
            }
        }

        if (allocSize != 0) {
            /* Lock and initialize buffer fields */
            u32 cs;
            SFLIB_LockCs(&cs);

            *(u32 *)(p + OFF_BUFOBJ) = allocSize;
            *(u32 *)(p + OFF_BUFSIZ) = actualSize;
            *(u32 *)(p + OFF_WTOT) = 0;
            *(u32 *)(p + OFF_RTOT) = 0;
            *(u32 *)(p + OFF_DLM_PTR) = 0;
            *(u32 *)(p + OFF_DLM_SIZE) = 0;

            SFLIB_UnlockCs(&cs);

            /* Initialize PTS queue */
            SFPTS_InitPtsQue(p + 0x38);
        }
    }

    /* Set buffer metadata */
    *(u32 *)(p + OFF_STATE) = state;
    *(u32 *)(p + OFF_ACTIVE) = active;
    *(u32 *)(p + 0x08) = 0;
    *(u32 *)(p + 0x0C) = 0;
    *(u32 *)(p + 0x4C) = 9;
    *(u32 *)(p + 0x50) = 9;
}

/*
 * SFBUF_InitHn - Initialize buffer handle with all ring buffers
 *
 * Sets up the complete buffer system: initializes ring buffers,
 * configures video frame buffers and audio ring buffers, and
 * sets initial state.
 *
 * @param self   Buffer handle pointer
 * @param idx    Buffer group index
 * @param config Configuration array with sizes and metadata
 */
void SFBUF_InitHn(void *self, int idx, u32 *config) {
    u32 cumulative[8];
    u32 bufSize = config[0];
    u32 divisor = config[11]; /* config[0x2C/4] */
    u32 extraSize;
    int i;

    /* Build cumulative offset array from config sizes */
    cumulative[0] = bufSize;
    for (i = 1; i < 7; i++) {
        cumulative[i] = cumulative[i - 1] + config[i];
    }

    /* Compute extra size as remainder of bufSize / divisor */
    extraSize = bufSize - (bufSize / divisor) * divisor;

    /* Initialize ring buffer (index 0) */
    sfbuf_InitRingSj(self, cumulative, config, 0, extraSize);
    if (*(u32 *)sfbuf_base(self, 0) != 0) return;

    /* Initialize video frame buffer 0 (index 1) */
    sfbuf_InitRingSj(self, cumulative, config, 1, 0x800);
    if (*(u32 *)sfbuf_base(self, 1) != 0) return;

    /* Initialize audio ring buffer 0 (index 2) */
    sfbuf_InitRingSj(self, cumulative, config, 2, 0);
    if (*(u32 *)sfbuf_base(self, 2) != 0) return;

    /* Initialize video frame buffer 1 (index 3) */
    sfbuf_InitVfrmBuf(self, self, cumulative, config, 3);

    /* Initialize audio ring buffer 1 (index 4) */
    sfbuf_InitAringBuf(self, cumulative, config, 4);

    /* Initialize video frame buffer 2 (index 5) */
    sfbuf_InitVfrmBuf(self, self, cumulative, config, 5);

    /* Initialize audio ring buffer 2 (index 6) */
    sfbuf_InitAringBuf(self, cumulative, config, 6);

    /* Set initial handle state */
    *(u32 *)((u8 *)self + 0x32C) = 3;
    *(u32 *)((u8 *)self + 0x330) = 1;
    *(u32 *)((u8 *)self + 0x334) = 0;
    *(u32 *)((u8 *)self + 0x338) = 0;
    *(u32 *)((u8 *)self + 0x378) = 9;
    *(u32 *)((u8 *)self + 0x37C) = 9;
    *(u32 *)((u8 *)self + 0x33C) = 0;
    *(u32 *)((u8 *)self + 0x340) = 0;
    *(u32 *)((u8 *)self + 0x344) = 0;
    *(u32 *)((u8 *)self + 0x348) = 0;
    *(u32 *)((u8 *)self + 0x34C) = 0;
    *(u32 *)((u8 *)self + 0x350) = 0;
    *(u32 *)((u8 *)self + 0x354) = 0;
    *(u32 *)((u8 *)self + 0x358) = 0;
    *(u32 *)((u8 *)self + 0x35C) = 0;
    *(u32 *)((u8 *)self + 0x360) = 0;
    *(u32 *)((u8 *)self + 0x364) = 0;
    *(u32 *)((u8 *)self + 0x368) = 0;
}

/*
 * SFBUF_DestroySj - Destroy all active SJ buffer objects
 *
 * Iterates through the three primary buffer types (ring, vfrm, aring)
 * and destroys their associated SJ objects if they are in the active state.
 */
void SFBUF_DestroySj(void *self) {
    u8 *base = (u8 *)self;
    u8 *supply = base + 0x13C8;
    void *obj;

    /* Buffer 0: ring buffer */
    if (*(u32 *)(base + 0x13B8) == SFBUF_STATE_ACTIVE) {
        obj = *(void **)(supply + 0x04);
        if (obj != NULL) {
            void *vtable = *(void **)obj;
            ((void (*)(void *))((void **)vtable)[3])(obj);
            *(u32 *)(supply + 0x04) = 0;
        }
    }

    /* Buffer 1: vfrm buffer */
    supply = base + 0x143C;
    if (*(u32 *)(base + 0x142C) == SFBUF_STATE_ACTIVE) {
        obj = *(void **)(supply + 0x04);
        if (obj != NULL) {
            void *vtable = *(void **)obj;
            ((void (*)(void *))((void **)vtable)[3])(obj);
            *(u32 *)(supply + 0x04) = 0;
        }
    }

    /* Buffer 2: aring buffer */
    supply = base + 0x14B0;
    if (*(u32 *)(base + 0x14A0) == SFBUF_STATE_ACTIVE) {
        obj = *(void **)(supply + 0x04);
        if (obj != NULL) {
            void *vtable = *(void **)obj;
            ((void (*)(void *))((void **)vtable)[3])(obj);
            *(u32 *)(supply + 0x04) = 0;
        }
    }
}

/*
 * sfbuf_InitVfrmBuf - Initialize video frame buffer
 *
 * Sets up a video frame buffer at the given index with the specified
 * sizes and pointers. Initializes the VFRM frame queue structure.
 */
void sfbuf_InitVfrmBuf(void *self, void *handle, u32 *cumulative,
                        u32 *sizes, int idx) {
    u8 *p = (u8 *)self + idx * SFBUF_BUF_STRIDE;
    u32 ptrVal = sizes[idx];
    u32 valid = (u32)(-(s32)ptrVal | ptrVal) >> 31; /* non-zero check */
    u8 *vfrm = (u8 *)handle + 0x1758;
    int i;

    *(u32 *)(p + 0x00) = 1; /* VFRM type */
    *(u32 *)(p + 0x04) = valid;
    *(u32 *)(p + 0x08) = 0;
    *(u32 *)(p + 0x0C) = 0;
    *(u32 *)(p + 0x4C) = 9;
    *(u32 *)(p + 0x50) = 9;
    *(u32 *)(p + 0x10) = cumulative[idx];
    *(u32 *)(p + 0x14) = sizes[idx];
    *(u32 *)(p + 0x18) = 0;
    *(u32 *)(p + 0x1C) = 0;

    /* Set up VFRM frame queue at offset 0x20 */
    *(u32 *)(p + 0x20) = (u32)vfrm;
    *(u32 *)vfrm = 0;

    /* Initialize 16 frame slots in the VFRM queue (stride 0x88) */
    for (i = 0; i < 16; i++) {
        *(u32 *)(*(u32 *)(p + 0x20) + i * 0x88) = 0;
    }
}

/*
 * sfbuf_InitAringBuf - Initialize audio ring buffer
 *
 * Sets up an audio ring buffer at the given index with the specified
 * sizes and pointers. Clears all ring control fields.
 */
void sfbuf_InitAringBuf(void *self, u32 *cumulative, u32 *sizes, int idx) {
    u8 *p = (u8 *)self + idx * SFBUF_BUF_STRIDE;
    u32 ptrVal = sizes[idx];
    u32 valid = (u32)(-(s32)ptrVal | ptrVal) >> 31; /* non-zero check */

    *(u32 *)(p + 0x00) = 2; /* ARING type */
    *(u32 *)(p + 0x04) = valid;
    *(u32 *)(p + 0x08) = 0;
    *(u32 *)(p + 0x0C) = 0;
    *(u32 *)(p + 0x4C) = 9;
    *(u32 *)(p + 0x50) = 9;
    *(u32 *)(p + 0x10) = cumulative[idx];
    *(u32 *)(p + 0x14) = sizes[idx];
    *(u32 *)(p + 0x18) = 0;
    *(u32 *)(p + 0x1C) = 0;
    *(u32 *)(p + 0x20) = 0;
    *(u32 *)(p + 0x24) = 0;
    *(u32 *)(p + 0x28) = 0;
    *(u32 *)(p + 0x2C) = 0;
    *(u32 *)(p + 0x30) = 0;
    *(u32 *)(p + 0x34) = 0;
    *(u32 *)(p + 0x38) = 0;
    *(u32 *)(p + 0x3C) = 0;
}

/*
 * SFBUF_SetSupplySj - Set supply buffer configuration
 *
 * Validates the supply configuration and copies it into the
 * appropriate ring buffer's supply section. Also resets the PTS queue.
 *
 * @param self    Buffer handle
 * @param idx     Transport index (1=video, 2=audio, 3=subtitle)
 * @param supply  Supply configuration (ptr, size, readLen, writeLen, ...)
 * @return 0 on success, -1 on error
 */
s32 SFBUF_SetSupplySj(void *self, int idx, u32 *supply) {
    u32 bufBase;
    int bufIdx;
    u32 cs;

    /* Validate supply configuration */
    if (supply[1] == 0) {
        return -1;
    }
    if (supply[0] != 0) {
        /* Check additional fields */
        if (supply[2] == 0) {
            return -1;
        }
        if ((s32)supply[3] <= 0) {
            return -1;
        }
        if ((s32)supply[5] > 0) {
            return -1;
        }
    }

    /* Determine buffer index from transport type */
    if (SFTRN_IsSetup(self, 1)) {
        bufIdx = 0;
    } else if (SFTRN_IsSetup(self, 2)) {
        bufIdx = 1;
    } else if (SFTRN_IsSetup(self, 3)) {
        bufIdx = 2;
    } else {
        bufIdx = 0;
    }

    bufBase = bufIdx * SFBUF_BUF_STRIDE;

    /* Check that buffer is in ready state */
    if (*(u32 *)((u8 *)self + bufBase + 0x13B8) != SFBUF_STATE_READY) {
        SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x09);
        return -1;
    }

    /* Compute non-zero flag for supply pointer */
    {
        u32 ptr = supply[1];
        u32 flag = (u32)(-(s32)ptr | ptr) >> 31;
        u8 *dst = (u8 *)self + bufBase + 0x13C8;

        /* Lock critical section */
        SFLIB_LockCs(&cs);

        /* Store active flag */
        *(u32 *)((u8 *)self + bufBase + 0x13BC) = flag;

        /* Copy supply fields (ptr, size, readLen, writeLen) */
        *(u32 *)(dst + 0x00) = supply[0];
        *(u32 *)(dst + 0x04) = supply[1];
        *(u32 *)(dst + 0x08) = supply[2];
        *(u32 *)(dst + 0x0C) = supply[3];
        *(u32 *)(dst + 0x10) = supply[4];
        *(u32 *)(dst + 0x14) = supply[5];

        /* Clear counters and state */
        *(u32 *)(dst + 0x18) = 0;
        *(u32 *)(dst + 0x1C) = 0;
        *(u32 *)(dst + 0x20) = 0;
        *(u32 *)(dst + 0x24) = 0;

        SFLIB_UnlockCs(&cs);

        /* Reset PTS queue */
        SFPTS_ResetPtsQue(dst + 0x28);
    }

    return 0;
}

/*
 * fn_803C1CAC - Check transport setup status and supply availability
 *
 * Checks if any transport (video/audio/subtitle) is set up and has
 * a valid supply buffer configured.
 *
 * @param self  Buffer handle
 * @return 1 if supply available, 0 otherwise
 */
int fn_803C1CAC(void *self) {
    int bufIdx;
    u32 *fields;

    /* Determine buffer index from transport type */
    if (SFTRN_IsSetup(self, 1)) {
        bufIdx = 0;
    } else if (SFTRN_IsSetup(self, 2)) {
        bufIdx = 1;
    } else if (SFTRN_IsSetup(self, 3)) {
        bufIdx = 2;
    } else {
        bufIdx = 0;
    }

    /* Check if buffer has supply configured */
    fields = (u32 *)((u8 *)self + bufIdx * 0x74 + 0x13B8);

    if (fields[1] == 0 || fields[5] == 0) { /* active flag or ring obj */
        return 0;
    }
    return 1;
}

/*
 * SFBUF_SetUoch - Set UOCH (user operation channel) data
 *
 * Copies a 4-field UOCH structure into the buffer's UOCH slot.
 *
 * @param self    Buffer handle
 * @param idx     Buffer index
 * @param sub_idx Sub-index (UOCH slot)
 * @param src     Source UOCH data (4 x u32)
 */
void SFBUF_SetUoch(void *self, int idx, int sub_idx, u32 *src) {
    u32 s0 = src[0];
    u32 s1 = src[1];
    u8 *dst = (u8 *)self + idx * SFBUF_BUF_STRIDE + (sub_idx << 4) + 0x13C8;
    *(u32 *)(dst + 0) = s0;
    *(u32 *)(dst + 4) = s1;
    s0 = src[2];
    s1 = src[3];
    *(u32 *)(dst + 8) = s0;
    *(u32 *)(dst + 12) = s1;
}

/*
 * SFBUF_GetUoch - Get UOCH (user operation channel) data
 *
 * Reads a 4-field UOCH structure from the buffer's UOCH slot.
 *
 * @param self    Buffer handle
 * @param idx     Buffer index
 * @param sub_idx Sub-index (UOCH slot)
 * @param dst     Destination buffer (4 x u32)
 */
void SFBUF_GetUoch(void *self, int idx, int sub_idx, u32 *dst) {
    u32 *src = (u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE +
                        (sub_idx << 4) + 0x13C8);
    u32 s0 = src[0];
    u32 s1 = src[1];
    dst[1] = s1;
    dst[0] = s0;
    s0 = src[2];
    s1 = src[3];
    dst[3] = s1;
    dst[2] = s0;
}

/*
 * sfbuf_RingGetSub - Internal ring buffer get operation
 *
 * Retrieves data from the ring buffer at the specified index.
 * Mode 0 = write access, Mode 1 = read access.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (passed as pointer value)
 * @param out   Output data structure
 * @param mode  Access mode (0=write, 1=read)
 */
void sfbuf_RingGetSub(void *self, int idx, u32 *out, int mode) {
    u8 *p = sfbuf_base(self, idx);
    u32 active = *(u32 *)(p + 0x13BC);
    void *ringObj = *(void **)(p + 0x13CC);

    /* Clear output structure */
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = 0;
    out[6] = 0;

    /* Check if buffer is active */
    if (active == 0 || ringObj == NULL) {
        return;
    }

    /* Call virtual methods on the ring buffer object */
    {
        void *vtable = *(void **)ringObj;
        u32 (*getFunc)(void *, int) = (u32 (*)(void *, int))((void **)vtable)[9];
        u32 (*infoFunc)(void *, int, u32, u32 *) =
            (u32 (*)(void *, int, u32, u32 *))((void **)vtable)[6];
        u32 (*readFunc)(void *, int, u32 *) =
            (u32 (*)(void *, int, u32 *))((void **)vtable)[7];

        u32 dataAvail = getFunc(ringObj, mode);
        u32 info[2];
        u32 chunk[2];

        infoFunc(ringObj, mode, 0x7FFFFFFF, info);

        if (info[1] < dataAvail) {
            /* Need second chunk */
            infoFunc(ringObj, mode, 0x7FFFFFFF, chunk);
            readFunc(ringObj, mode, chunk);
        } else {
            chunk[0] = 0;
            chunk[1] = 0;
        }

        /* Get final data */
        infoFunc(ringObj, mode, 0x7FFFFFFF, info);
        readFunc(ringObj, mode, info);

        /* Copy results to output */
        out[0] = info[0];
        out[1] = info[1];
        out[2] = chunk[0];
        out[3] = chunk[1];
    }
}

/*
 * SFBUF_RingGetWrite - Get write access to ring buffer
 */
void SFBUF_RingGetWrite(void *self, int idx, u32 *out) {
    sfbuf_RingGetSub(self, idx, out, 0);
}

/*
 * SFBUF_RingGetRead - Get read access to ring buffer
 */
void SFBUF_RingGetRead(void *self, int idx, u32 *out) {
    sfbuf_RingGetSub(self, idx, out, 1);
}

/*
 * sfbuf_RingAddSub - Internal ring buffer add operation
 *
 * Adds data to the ring buffer at the specified index.
 * Handles write pointer advancement and PTS tracking.
 *
 * @param self   Buffer handle
 * @param idx    Buffer index
 * @param size   Data size to add
 * @param mode   Access mode (0=write, 1=read)
 * @return 0 on success, error code on failure
 */
void sfbuf_RingAddSub(void *self, int idx, u32 size, int mode) {
    u8 *p = sfbuf_base(self, idx);
    u8 *supply = p + 0x13C8;
    u32 active = *(u32 *)(p + 0x13BC);
    void *ringObj = *(void **)(p + 0x13CC);
    u32 result = 0;

    /* Check inputs */
    if (size == 0) return;
    if (active == 0 || ringObj == NULL) return;

    /* Call virtual methods on the ring buffer object */
    {
        void *vtable = *(void **)ringObj;
        u32 (*infoFunc)(void *, int, u32, u32 *) =
            (u32 (*)(void *, int, u32, u32 *))((void **)vtable)[6];
        u32 (*addFunc)(void *, int, u32 *) =
            (u32 (*)(void *, int, u32 *))((void **)vtable)[8];
        u32 (*getFunc)(void *, int) = (u32 (*)(void *, int))((void **)vtable)[9];

        u32 isRead = (mode == 1) ? 1 : 0;
        u32 info[2];

        /* Get available space */
        infoFunc(ringObj, isRead, 0x7FFFFFFF, info);
        addFunc(ringObj, isRead, info);

        if (info[1] < size) {
            /* Not enough space - try second chunk */
            u32 remaining = size - info[1];
            u32 chunk[2];

            infoFunc(ringObj, isRead, remaining, chunk);
            addFunc(ringObj, isRead, chunk);

            if (chunk[1] < remaining) {
                SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x0B);
                result = 1;
            }
        }

        /* Update PTS tracking for read mode */
        if (mode == 1 && idx == 1) {
            u32 *dlmPtr = (u32 *)(p + 0x13CC);
            u32 *ptsPtr = (u32 *)(p + 0x13E0);
            u32 dlm[2];
            u32 pts[2];
            u32 (*dlmFunc)(void *, int, u32 *) =
                (u32 (*)(void *, int, u32 *))((void **)vtable)[6];
            u32 (*ptsFunc)(void *, int, u32 *) =
                (u32 (*)(void *, int, u32 *))((void **)vtable)[7];

            dlmFunc(ringObj, 1, dlm);
            ptsFunc(ringObj, 1, pts);

            /* Check PTS validity */
            if (dlm[0] < pts[0] || dlm[0] >= pts[0] + pts[1]) {
                /* PTS out of range - reset */
                ptsFunc(ringObj, 1, pts);
                if (dlm[0] >= pts[0] && dlm[0] < pts[0] + pts[1]) {
                    /* Still valid */
                } else {
                    *(u32 *)(p + 0x13E0) = 0;
                    *(u32 *)(p + 0x13E4) = 0;
                }
            }
        }

        /* Update counters */
        if (mode == 1) {
            u32 *cntPtr = (u32 *)(p + 0x13E4);
            if ((s32)*cntPtr >= 0) {
                *cntPtr += size;
            }
        } else {
            u32 *cntPtr = (u32 *)(p + 0x13E0);
            if ((s32)*cntPtr >= 0) {
                *cntPtr += size;
            }
        }
    }

    /* Mark buffer as updated */
    *(u32 *)((u8 *)self + 0x50) = 1;
}

/*
 * SFBUF_RingAddWrite - Add data to ring buffer (write mode)
 */
void SFBUF_RingAddWrite(void *self, int idx, u32 size) {
    sfbuf_RingAddSub(self, idx, size, 0);
}

/*
 * SFBUF_RingAddRead - Add data to ring buffer (read mode)
 */
void SFBUF_RingAddRead(void *self, int idx, u32 size) {
    sfbuf_RingAddSub(self, idx, size, 1);
}

/*
 * SFBUF_RingGetDlm - Get DLM (decode latency marker) values
 *
 * Thread-safe read of the DLM pointer and size from the ring buffer.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param dlm   Output: DLM pointer
 * @param size  Output: DLM size
 */
void SFBUF_RingGetDlm(void *self, int idx, u32 *dlm, u32 *size) {
    u8 *p = (u8 *)self + idx * 0x74;
    u32 cs;

    SFLIB_LockCs(&cs);
    *dlm = *(u32 *)(p + 0x13E0);
    *size = *(u32 *)(p + 0x13E4);
    SFLIB_UnlockCs(&cs);
}

/*
 * SFBUF_RingSetDlm - Set DLM (decode latency marker) values
 *
 * Thread-safe write of the DLM pointer and size to the ring buffer.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param dlm   DLM pointer value
 * @param size  DLM size value
 */
void SFBUF_RingSetDlm(void *self, int idx, u32 dlm, u32 size) {
    u8 *p = (u8 *)self + idx * 0x74;
    u32 cs;

    SFLIB_LockCs(&cs);
    *(u32 *)(p + 0x13E0) = dlm;
    *(u32 *)(p + 0x13E4) = size;
    SFLIB_UnlockCs(&cs);
}

/*
 * SFBUF_GetRingBufSiz - Get ring buffer size
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Buffer size in bytes
 */
u32 SFBUF_GetRingBufSiz(void *self, u32 idx) {
    return *(u32 *)((u8 *)self + 0x13D4 + idx * SFBUF_BUF_STRIDE);
}

/*
 * SFBUF_GetRTot - Get read total counter
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Read total
 */
u32 SFBUF_GetRTot(void *self, u32 idx) {
    return *(u32 *)((u8 *)self + 0x13EC + idx * SFBUF_BUF_STRIDE);
}

/*
 * SFBUF_GetWTot - Get write total counter (thread-safe)
 *
 * Computes the write total by combining the stored write total
 * with the current ring buffer write position when needed.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Write total (clamped to 0x7FFFFFFF)
 */
u32 SFBUF_GetWTot(void *self, int idx) {
    u8 *p = (u8 *)self + idx * 0x74;
    u32 cs;
    u32 wtot;
    u32 rtot;

    SFLIB_LockCs(&cs);

    wtot = *(u32 *)(p + 0x13E8);
    rtot = *(u32 *)(p + 0x13EC);

    if (wtot == 0 && rtot != 0) {
        void *ringObj = *(void **)(p + 0x13CC);
        void *vtable = *(void **)ringObj;
        u32 (*getFunc)(void *, int) = (u32 (*)(void *, int))((void **)vtable)[9];
        wtot = rtot + getFunc(ringObj, 1);
    }

    if ((s32)wtot < 0) {
        wtot = 0x7FFFFFFF;
    }

    SFLIB_UnlockCs(&cs);

    return wtot;
}

/*
 * SFBUF_RingGetSj - Get ring buffer SJ (supply) object
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param out   Output: pointer to store SJ object
 * @return 0 on success, error code on failure
 */
s32 SFBUF_RingGetSj(void *self, s32 idx, void **out) {
    u8 *p = sfbuf_base(self, idx);

    *out = NULL;

    /* Check if buffer is active */
    if (*(u32 *)(p + 0x13BC) == 0) {
        return SFLIB_SetErr(self, SFBUF_ERR_BASE + 0x01);
    }

    /* Return the SJ object */
    *out = *(void **)(p + 0x13CC);
    return 0;
}

/*
 * SFBUF_AddRtotSj - Add to read total counter
 *
 * Increments the read total for the specified buffer.
 * Skips the update if the counter is negative (disabled).
 *
 * @param self    Buffer handle
 * @param idx     Buffer index
 * @param addend  Value to add to read total
 */
void SFBUF_AddRtotSj(void *self, int idx, int addend) {
    u32 *ptr = (u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13EC);
    if ((s32)*ptr < 0) return;
    *ptr += addend;
}

/*
 * SFBUF_VfrmGetRead - Get video frame read data
 *
 * If the buffer is not active, delegates to the transport
 * layer to handle the video frame read.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param out   Output data pointer
 * @param size  Requested size
 * @return Result from transport layer, or 0 if active
 */
int SFBUF_VfrmGetRead(void *self, int idx, int *out, int size) {
    u8 *p = sfbuf_base(self, idx);

    if (*(u32 *)(p + 0x13BC) == 0) {
        return SFTRN_CallTrtTrif(self, *(int *)(p + 0x1404), 0x0B, out, size);
    }
    return 0;
}

/*
 * SFBUF_VfrmAddRead - Add to video frame read counter
 *
 * Updates the video frame read position and marks buffer as updated.
 * Delegates to transport layer if buffer is not active.
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @param size  Data size to add
 * @param flags Additional flags
 */
void SFBUF_VfrmAddRead(void *self, int idx, u32 size, u32 flags) {
    u8 *p = (u8 *)self + idx * 0x74;
    s32 result;
    u32 savedFlags;

    result = 0;
    savedFlags = flags;

    if (*(u32 *)(p + 0x13BC) == 0) {
        result = SFTRN_CallTrtTrif(self, *(int *)(p + 0x1404), 0x0C,
                                   (int *)(u32)size, (int)savedFlags);
    }

    *(u32 *)((u8 *)self + 0x50) = 1;
}

/*
 * SFBUF_SetPrepFlg - Set preparation flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = skip)
 * @param val   Flag value
 */
void SFBUF_SetPrepFlg(void *self, u32 idx, u32 val) {
    if (idx == SFBUF_MAX_IDX) return;
    *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C0) = val;
}

/*
 * SFBUF_GetPrepFlg - Get preparation flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = return 0)
 * @return Flag value
 */
int SFBUF_GetPrepFlg(void *self, int idx) {
    if (idx == SFBUF_MAX_IDX) return 0;
    return *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C0);
}

/*
 * SFBUF_SetTermFlg - Set termination flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = skip)
 * @param val   Flag value
 */
void SFBUF_SetTermFlg(void *self, s32 idx, u32 val) {
    if (idx == SFBUF_MAX_IDX) return;
    *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C4) = val;
}

/*
 * SFBUF_GetTermFlg - Get termination flag
 *
 * @param self  Buffer handle
 * @param idx   Buffer index (8 = return 1)
 * @return Flag value
 */
int SFBUF_GetTermFlg(void *self, int idx) {
    if (idx == SFBUF_MAX_IDX) return 1;
    return *(u32 *)((u8 *)self + idx * SFBUF_BUF_STRIDE + 0x13C4);
}

/*
 * SFBUF_RingGetDataSiz - Get total data size in ring buffer
 *
 * Retrieves the current read buffer information and returns
 * the sum of both data segments (for circular buffer wrap).
 *
 * @param self  Buffer handle
 * @param idx   Buffer index
 * @return Total data size available
 */
s32 SFBUF_RingGetDataSiz(void *self, int idx) {
    u32 result[7];
    sfbuf_RingGetSub(self, idx, result, 1);
    return (s32)(result[1] + result[3]);
}

/*
 * SFBUF_GetFlowCnt - Get flow control counters
 *
 * Queries the flow control state from the appropriate buffer
 * object (ring buffer or memory buffer) and stores the results.
 *
 * @param self      Buffer handle
 * @param readCnt   Output: read flow count
 * @param writeCnt  Output: write flow count
 */
void SFBUF_GetFlowCnt(void *self, u32 *readCnt, u32 *writeCnt) {
    void *vtable = *(void **)self;
    u32 (*initFunc)(void *) = (u32 (*)(void *))((void **)vtable)[4];
    u32 initResult = initFunc(self);
    u32 *wCnt = writeCnt;
    u32 *rCnt = readCnt;

    if (initResult == (u32)lbl_eu_80606E10) {
        *rCnt = SJRBF_GetFlowCnt(self, 1, 1);
        *wCnt = SJRBF_GetFlowCnt(self, 0, 1);
    } else if (initResult == (u32)lbl_eu_80606E14) {
        u32 bufSize = SJMEM_GetBufSize(self);
        *rCnt = bufSize;

        vtable = *(void **)self;
        initFunc = (u32 (*)(void *))((void **)vtable)[9];
        *wCnt = bufSize - initFunc(self);
    } else {
        *rCnt = 0;
        *wCnt = 0;
    }
}

/*
 * SFBUF_UpdateFlowCnt - Update flow control counter
 *
 * Computes a new flow count based on bit changes between
 * old and new values. Uses leading zero count to determine
 * which bit changed.
 *
 * @param count   Current flow count
 * @param newVal  New value
 * @param oldVal  Old value
 * @return Updated flow count
 */
int SFBUF_UpdateFlowCnt(int count, int newVal, int oldVal) {
    u32 diff = newVal ^ oldVal;
    u32 leading = __cntlzw(diff);
    u32 shifted = newVal << leading;
    u32 bit = shifted >> 31;
    return count + (int)bit;
}
