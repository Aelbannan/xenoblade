// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm
// RFCOMM port state machine (Broadcom BTE RFCOMM stack).
// Field offsets recovered from retail assembly and cross-checked against the
// already-decompiled sibling units port_rfc.c / port_utils.c / rfc_ts_frames.c.

/* "libs/RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm.c" line 5 "harness_catalog.h" */
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
/* "libs/RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_port_fsm.c" line 6 "string.h" */
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

/* ------------------------------------------------------------------ */
/*  Types                                                              */
/* ------------------------------------------------------------------ */
typedef struct RfcMuxChannel tRFC_MCB;
typedef struct RfcPort tPORT;

/* GKI buffer header. */
typedef struct {
    u16 event;            /* 0x00 */
    u16 len;              /* 0x02 */
    u16 offset;           /* 0x04 */
    u16 layer_specific;   /* 0x06 */
} BT_HDR;

/* Received TS 07.10 MX frame (union payload at 0x08). */
typedef struct {
    u8  dlci;             /* 0x00 */
    u8  type;             /* 0x01 */
    u8  cr;               /* 0x02 */
    u8  ea;               /* 0x03 */
    u8  pf;               /* 0x04 */
    u8  credit;           /* 0x05 */
    u8  pad_06[2];
    union {
        struct {
            u8  dlci;        /* 0x08 */
            u8  frame_type;  /* 0x09 */
            u8  conv_layer;  /* 0x0A */
            u8  priority;    /* 0x0B */
            u8  t1;          /* 0x0C */
            u8  pad_0d;
            u16 mtu;         /* 0x0E */
            u8  n2;          /* 0x10 */
            u8  k;           /* 0x11 */
        } pn;
        struct {
            u8  dlci;          /* 0x08 */
            u8  signals;       /* 0x09 */
            u8  break_present; /* 0x0A */
            u8  break_duration;/* 0x0B */
        } msc;
        struct {
            u8  dlci;          /* 0x08 */
            u8  is_request;    /* 0x09 */
            u8  baud_rate;     /* 0x0A */
            u8  byte_size;     /* 0x0B */
            u8  stop_bits;     /* 0x0C */
            u8  parity;        /* 0x0D */
            u8  parity_type;   /* 0x0E */
            u8  fc_type;       /* 0x0F */
            u8  xon_char;      /* 0x10 */
            u8  xoff_char;     /* 0x11 */
            u16 param_mask;    /* 0x12 */
        } rpn;
        struct {
            u8  dlci;          /* 0x08 */
            u8  line_status;   /* 0x09 */
        } rls;
    } u;
} MX_FRAME;

/* Port negotiation / modem status control block (9 bytes, matches tPORT port_ctrl). */
typedef struct {
    u8 baud_rate;         /* 0x00 */
    u8 byte_size;         /* 0x01 */
    u8 stop_bits;         /* 0x02 */
    u8 parity;            /* 0x03 */
    u8 parity_type;       /* 0x04 */
    u8 fc_type;           /* 0x05 */
    u8 pad_06;            /* 0x06 */
    u8 xon_char;          /* 0x07 */
    u8 xoff_char;         /* 0x08 */
} tPORT_STATE;

/* Local MSC processing block (5 bytes). */
typedef struct {
    u8 fc;                /* 0x00 */
    u8 break_duration;    /* 0x01 */
    u8 disc;              /* 0x02 */
    u8 dtr;               /* 0x03 */
    u8 break_signal;      /* 0x04 */
} tMSC_CTRL;

/* RFCOMM control block (flow @ 0x64, trace level @ 0x414). */
typedef struct {
    u8 pad_00[0x64];
    u8 flow;              /* 0x64 - flow control disabled flag */
    u8 pad_65[0x3AF];
    u8 trace_level;       /* 0x414 */
} tRFC_CB;

/* tRFC_MCB - RFCOMM multiplexer channel. */
struct RfcMuxChannel {
    u8  pad_00[0x62];
    u8  bd_addr[6];         /* 0x62 */
    u16 lcid;               /* 0x68 */
    u16 peer_l2cap_mtu;     /* 0x6A */
    u8  state;              /* 0x6C */
    u8  pad_6d[5];
    u8  flow;               /* 0x72 */
    u8  l2cap_congested;    /* 0x73 */
    u8  is_disc_initiator;  /* 0x74 */
};

/* tPORT - port object (only fields accessed by this TU). */
struct RfcPort {
    u8  pad_00[0x0D];
    u8  dlci;               /* 0x0D */
    u8  pad_0e[0x06];
    u16 peer_mtu;           /* 0x14 */
    u8  pad_16[0x0E];
    u8  field_24;           /* 0x24 - previous break/flow state */
    u8  pad_25[0x18];
    u8  field_3d;           /* 0x3D */
    u8  pad_3e[0x13];
    tPORT_STATE port_ctrl;  /* 0x51 (9 bytes) */
    u8  pad_5a[0x09];
    u8  field_63;           /* 0x63 */
    u8  pad_64[4];
    u8  rfc_state;          /* 0x68 - port state-machine state */
    u8  rfc_flags;          /* 0x69 - expected-response bits */
    u8  pad_6a[2];
    tRFC_MCB *p_mcb;        /* 0x6C */
    u8  pad_70[0x28];
    u16 credit_tx;          /* 0x98 */
    u16 credit_rx;          /* 0x9A */
    u16 credit_rx_max;      /* 0x9C */
};

/* ------------------------------------------------------------------ */
/*  Constants                                                          */
/* ------------------------------------------------------------------ */
/* Port states (p_port->rfc_state). */
#define PORT_STATE_CLOSED               0
#define PORT_STATE_OPENING              1
#define PORT_STATE_ORIG_WAIT_SEC_CHECK  2
#define PORT_STATE_TERM_WAIT_SEC_CHECK  3
#define PORT_STATE_OPENED               4
#define PORT_STATE_DISC_WAIT_UA         5

/* Port state-machine events. */
#define PORT_EV_SABME           0   /* SABME frame received */
#define PORT_EV_UA              1   /* UA frame received */
#define PORT_EV_DM              2   /* DM frame received */
#define PORT_EV_DISC            3   /* DISC frame received */
#define PORT_EV_DATA            4   /* UIH data frame received */
#define PORT_EV_TIMEOUT         5   /* T1 timeout */
#define PORT_EV_DLC_EST_REQ     9   /* local DlcEstablish request */
#define PORT_EV_DLC_EST_RSP     11  /* local DlcEstablish response */
#define PORT_EV_DLC_REL_REQ     12  /* local DlcRelease request */
#define PORT_EV_LCID_DISCONNECTED 13 /* L2CAP channel disconnected */
#define PORT_EV_DATA_REQ        14  /* local data request */
#define PORT_EV_SEC_COMPLETE    15  /* security check complete */

/* Expected-response bits in p_port->rfc_flags (0x69). */
#define RFC_FLAG_EXPECT_PN      0x01
#define RFC_FLAG_EXPECT_RPN     0x02
#define RFC_FLAG_EXPECT_RPN2    0x04
#define RFC_FLAG_EXPECT_MSC     0x08
#define RFC_FLAG_EXPECT_RLS     0x10

/* RFCOMM multiplexer states (p_mcb->state). */
#define RFC_MX_STATE_DISC_WAIT_UA 6

/* Trace helpers. */
#define RFCOMM_TRACE_ERROR0(m) \
    { if (rfc_cb.trace_level >= 1) LogMsg_0(0x90000, (m)); }
#define RFCOMM_TRACE_ERROR2(m, p1, p2) \
    { if (rfc_cb.trace_level >= 1) LogMsg_2(0x90000, (m), (u32)(p1), (u32)(p2)); }
#define RFCOMM_TRACE_WARNING1(m, p1) \
    { if (rfc_cb.trace_level >= 2) LogMsg_1(0x90001, (m), (u32)(p1)); }

/* ------------------------------------------------------------------ */
/*  Externs                                                            */
/* ------------------------------------------------------------------ */
extern tRFC_CB rfc_cb;

extern void LogMsg_0(u32 level, const char *msg);
extern void LogMsg_1(u32 level, const char *msg, u32 p1);
extern void LogMsg_2(u32 level, const char *msg, u32 p1, u32 p2);

extern void GKI_freebuf(void *p_buf);

extern void rfc_send_sabme(tRFC_MCB *p_mcb, u8 dlci);
extern void rfc_send_ua(tRFC_MCB *p_mcb, u8 dlci);
extern void rfc_send_dm(tRFC_MCB *p_mcb, u8 dlci, u8 pf);
extern void rfc_send_disc(tRFC_MCB *p_mcb, u8 dlci);
extern void rfc_send_buf_uih(tRFC_MCB *p_mcb, u8 dlci, BT_HDR *p_buf);
extern void rfc_send_pn(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, u16 mtu, u8 cl, u8 k);
extern void rfc_send_fcon(tRFC_MCB *p_mcb, u8 is_command);
extern void rfc_send_fcoff(tRFC_MCB *p_mcb, u8 is_command);
extern void rfc_send_msc(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, void *p_pars);
extern void rfc_send_rls(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, u8 status);
extern void rfc_send_rpn(tRFC_MCB *p_mcb, u8 dlci, u8 is_command, void *p_pars, u16 mask);

extern void rfc_port_timer_start(tPORT *p_port, u16 tout);
extern void rfc_port_timer_stop(tPORT *p_port);
extern void rfc_port_closed(tPORT *p_port);
extern void rfc_check_send_cmd(tRFC_MCB *p_mcb, void *p_buf);
extern void rfc_dec_credit(tPORT *p_port);
extern void rfc_sec_check_complete(u8 *bd_addr, void *p_ref_data, u8 res);

extern tPORT *port_find_mcb_dlci_port(tRFC_MCB *p_mcb, u8 dlci);

extern void port_rfc_closed(tPORT *p_port, u8 result);

extern void PORT_DlcEstablishInd(tRFC_MCB *p_mcb, u8 dlci, u16 mtu);
extern void PORT_DlcEstablishCnf(tRFC_MCB *p_mcb, u8 dlci, u16 mtu, u16 result);
extern void PORT_ParNegInd(tRFC_MCB *p_mcb, u8 dlci, u16 mtu, u8 cl, u8 k);
extern void PORT_ParNegCnf(tRFC_MCB *p_mcb, u8 dlci, u16 mtu, u8 cl, u8 k);
extern void PORT_PortNegInd(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl, u16 request);
extern void PORT_PortNegCnf(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl, u16 result);
extern void PORT_ControlInd(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl);
extern void PORT_ControlCnf(tRFC_MCB *p_mcb, u8 dlci, void *p_ctrl);
extern void PORT_LineStatusInd(tRFC_MCB *p_mcb, u8 dlci, u8 line_status);
extern void PORT_DlcReleaseInd(tRFC_MCB *p_mcb, u8 dlci);
extern void PORT_DataInd(tRFC_MCB *p_mcb, u8 dlci, BT_HDR *p_buf);
extern void PORT_FlowInd(tRFC_MCB *p_mcb, u8 dlci, u8 fc);
extern void Port_TimeOutCloseMux(tRFC_MCB *p_mcb);

extern void btm_sec_mx_access_request(u8 *bd_addr, u16 psm, u8 is_originator,
                                      u32 mx_proto_id, u32 mx_chan_id,
                                      void (*p_callback)(u8 *, void *, u8),
                                      void *p_ref_data);
extern void btm_sec_abort_access_req(u8 *bd_addr);

/* ------------------------------------------------------------------ */
/*  Forward declarations                                               */
/* ------------------------------------------------------------------ */
void rfc_port_sm_state_closed(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_sabme_wait_ua(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_term_wait_sec_check(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_orig_wait_sec_check(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_opened(tPORT *p_port, u16 event, void *p_data);
void rfc_port_sm_disc_wait_ua(tPORT *p_port, u16 event, void *p_data);
void rfc_set_port_state(u8 *p_port_state, MX_FRAME *p_frame);

/* ================================================================== */
/*  rfc_port_sm_execute                                               */
/* ================================================================== */
void rfc_port_sm_execute(tPORT *p_port, u16 event, void *p_data)
{
    if (p_port == NULL) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_1(0x90001, "NULL port event %d", event);
        }
        return;
    }

    switch (p_port->rfc_state) {
    case PORT_STATE_CLOSED:
        rfc_port_sm_state_closed(p_port, event, p_data);
        break;
    case PORT_STATE_OPENING:
        rfc_port_sm_sabme_wait_ua(p_port, event, p_data);
        break;
    case PORT_STATE_ORIG_WAIT_SEC_CHECK:
        rfc_port_sm_orig_wait_sec_check(p_port, event, p_data);
        break;
    case PORT_STATE_TERM_WAIT_SEC_CHECK:
        rfc_port_sm_term_wait_sec_check(p_port, event, p_data);
        break;
    case PORT_STATE_OPENED:
        rfc_port_sm_opened(p_port, event, p_data);
        break;
    case PORT_STATE_DISC_WAIT_UA:
        rfc_port_sm_disc_wait_ua(p_port, event, p_data);
        break;
    }
}

/* ================================================================== */
/*  rfc_port_sm_state_closed                                          */
/* ================================================================== */
void rfc_port_sm_state_closed(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 - local connect request */
        p_port->rfc_state = PORT_STATE_ORIG_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->p_mcb->bd_addr, 3, 1, 3,
                                  (u32)p_port->dlci >> 1, rfc_sec_check_complete, p_port);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        GKI_freebuf(p_data);
        break;
    case PORT_EV_SABME:                     /* 0 - remote connect request */
        p_port->rfc_state = PORT_STATE_TERM_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->p_mcb->bd_addr, 3, 0, 3,
                                  (u32)p_port->dlci >> 1, rfc_sec_check_complete, p_port);
        return;
    case PORT_EV_UA:                        /* 1 */
        return;
    case PORT_EV_DM:                        /* 2 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 0);
        return;
    case PORT_EV_DISC:                      /* 3 */
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 0);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        Port_TimeOutCloseMux(p_port->p_mcb);
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state closed Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_sabme_wait_ua                                         */
/* ================================================================== */
void rfc_port_sm_sabme_wait_ua(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 */
    case PORT_EV_DLC_EST_RSP:               /* 11 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_DLC_REL_REQ:               /* 12 - local abort */
        rfc_port_timer_start(p_port, 3);
        rfc_send_disc(p_port->p_mcb, p_port->dlci);
        p_port->rfc_flags = 0;
        p_port->rfc_state = PORT_STATE_DISC_WAIT_UA;
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        GKI_freebuf(p_data);
        break;
    case PORT_EV_UA:                        /* 1 - connection established */
        rfc_port_timer_stop(p_port);
        p_port->rfc_state = PORT_STATE_OPENED;
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 0);
        return;
    case PORT_EV_DM:                        /* 2 - refused */
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 1);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DISC:                      /* 3 */
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 1);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_SABME:                     /* 0 - collision, respond UA */
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        p_port->rfc_state = PORT_STATE_CLOSED;
        PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci,
                             p_port->p_mcb->peer_l2cap_mtu, 1);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state sabme_wait_ua Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_term_wait_sec_check                                   */
/* ================================================================== */
void rfc_port_sm_term_wait_sec_check(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_SEC_COMPLETE:              /* 15 */
        if (*(u8 *)p_data != 0) {
            if (p_port->p_mcb != NULL) {
                rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
                port_rfc_closed(p_port, 0x0F);
            }
        } else {
            PORT_DlcEstablishInd(p_port->p_mcb, p_port->dlci,
                                 p_port->p_mcb->peer_l2cap_mtu);
        }
        return;
    case PORT_EV_DLC_EST_REQ:               /* 9 */
    case PORT_EV_DLC_REL_REQ:               /* 12 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        btm_sec_abort_access_req(p_port->p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        RFCOMM_TRACE_ERROR0("Port error state Term Wait Sec event Data");
        GKI_freebuf(p_data);
        return;
    case PORT_EV_SABME:                     /* 0 */
        return;
    case PORT_EV_DISC:                      /* 3 - remote gave up */
        btm_sec_abort_access_req(p_port->p_mcb->bd_addr);
        p_port->rfc_state = PORT_STATE_CLOSED;
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        PORT_DlcReleaseInd(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    case PORT_EV_DLC_EST_RSP:               /* 11 - local app response */
        if (*(u8 *)p_data != 0) {
            if (p_port->p_mcb != NULL) {
                rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
            }
        } else {
            rfc_send_ua(p_port->p_mcb, p_port->dlci);
            p_port->rfc_state = PORT_STATE_OPENED;
        }
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state term_wait_sec_check Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_orig_wait_sec_check                                   */
/* ================================================================== */
void rfc_port_sm_orig_wait_sec_check(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_SEC_COMPLETE:              /* 15 */
        if (*(u8 *)p_data != 0) {
            PORT_DlcEstablishCnf(p_port->p_mcb, p_port->dlci, 0, 0x70);
            rfc_port_closed(p_port);
        } else {
            rfc_send_sabme(p_port->p_mcb, p_port->dlci);
            rfc_port_timer_start(p_port, 0x3C);
            p_port->rfc_state = PORT_STATE_OPENING;
        }
        return;
    case PORT_EV_SABME:                     /* 0 */
    case PORT_EV_DLC_EST_REQ:               /* 9 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        btm_sec_abort_access_req(p_port->p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        RFCOMM_TRACE_ERROR0("Port error state Orig Wait Sec event Data");
        GKI_freebuf(p_data);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state orig_wait_sec_check Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_opened                                                */
/* ================================================================== */
void rfc_port_sm_opened(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_DLC_REL_REQ:               /* 12 - local close */
        rfc_port_timer_start(p_port, 3);
        rfc_send_disc(p_port->p_mcb, p_port->dlci);
        p_port->rfc_flags = 0;
        p_port->rfc_state = PORT_STATE_DISC_WAIT_UA;
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 - data with credit */
        if (p_port->p_mcb->flow == 2
            && ((BT_HDR *)p_data)->len < p_port->peer_mtu
            && p_port->field_3d == 0
            && p_port->credit_rx_max > p_port->credit_rx) {
            ((BT_HDR *)p_data)->layer_specific =
                (u16)(u8)(p_port->credit_rx_max - p_port->credit_rx);
            p_port->credit_rx = p_port->credit_rx_max;
        } else {
            ((BT_HDR *)p_data)->layer_specific = 0;
        }
        rfc_send_buf_uih(p_port->p_mcb, p_port->dlci, (BT_HDR *)p_data);
        rfc_dec_credit(p_port);
        return;
    case PORT_EV_UA:                        /* 1 */
        return;
    case PORT_EV_SABME:                     /* 0 */
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DM:                        /* 2 - remote disconnected */
        PORT_DlcReleaseInd(p_port->p_mcb, p_port->dlci);
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DISC:                      /* 3 - remote disconnected */
        p_port->rfc_state = PORT_STATE_CLOSED;
        rfc_send_ua(p_port->p_mcb, p_port->dlci);
        PORT_DlcReleaseInd(p_port->p_mcb, p_port->dlci);
        return;
    case PORT_EV_DATA:                      /* 4 */
        PORT_DataInd(p_port->p_mcb, p_port->dlci, (BT_HDR *)p_data);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        Port_TimeOutCloseMux(p_port->p_mcb);
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state opened Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_port_sm_disc_wait_ua                                          */
/* ================================================================== */
void rfc_port_sm_disc_wait_ua(tPORT *p_port, u16 event, void *p_data)
{
    switch (event) {
    case PORT_EV_DLC_EST_REQ:               /* 9 */
    case PORT_EV_DLC_EST_RSP:               /* 11 */
        RFCOMM_TRACE_ERROR2("Port error state %d event %d", p_port->rfc_state, event);
        return;
    case PORT_EV_LCID_DISCONNECTED:         /* 13 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_DATA_REQ:                  /* 14 */
        GKI_freebuf(p_data);
        return;
    case PORT_EV_UA:                        /* 1 */
        p_port->p_mcb->is_disc_initiator = 1;
        /* fall through */
    case PORT_EV_DM:                        /* 2 */
        rfc_port_closed(p_port);
        return;
    case PORT_EV_SABME:                     /* 0 */
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
        return;
    case PORT_EV_DISC:                      /* 3 */
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 1);
        return;
    case PORT_EV_DATA:                      /* 4 */
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->p_mcb, p_port->dlci, 0);
        return;
    case PORT_EV_TIMEOUT:                   /* 5 */
        rfc_port_closed(p_port);
        return;
    }
    RFCOMM_TRACE_WARNING1("Port state disc_wait_ua Event ignored %d", event);
}

/* ================================================================== */
/*  rfc_process_pn                                                    */
/* ================================================================== */
void rfc_process_pn(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame)
{
    tPORT *p_port;
    u8 dlci = p_frame->dlci;

    if (is_command) {
        /* Request */
        if (p_mcb->state != RFC_MX_STATE_DISC_WAIT_UA) {
            PORT_ParNegInd(p_mcb, dlci, p_frame->u.pn.mtu,
                           p_frame->u.pn.conv_layer, p_frame->u.pn.k);
        } else {
            /* We are disconnecting, send DM */
            rfc_send_dm(p_mcb, dlci, 0);
            if (rfc_cb.trace_level >= 2) {
                LogMsg_0(0x90001, "***** MX PN while disconnecting *****");
            }
        }
    } else {
        /* Response */
        p_port = port_find_mcb_dlci_port(p_mcb, dlci);
        if ((p_port == NULL) || !(p_port->rfc_flags & RFC_FLAG_EXPECT_PN)) {
            return;
        }
        p_port->rfc_flags &= 0xFE;
        rfc_port_timer_stop(p_port);
        PORT_ParNegCnf(p_mcb, dlci, p_frame->u.pn.mtu,
                       p_frame->u.pn.conv_layer, p_frame->u.pn.k);
    }
}

/* ================================================================== */
/*  rfc_process_rpn                                                   */
/* ================================================================== */
void rfc_process_rpn(tRFC_MCB *p_mcb, u8 is_command, u8 is_request, MX_FRAME *p_frame)
{
    tPORT *p_port;
    tPORT_STATE buf;

    p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
    if (p_port == NULL) {
        if (is_command) {
            memset(&buf, 0, sizeof(buf));
            rfc_set_port_state((u8 *)&buf, p_frame);
            PORT_PortNegInd(p_mcb, p_frame->dlci, &buf, p_frame->u.rpn.param_mask);
        }
        return;
    }

    if (is_command && is_request) {
        /* Request for new parameters: reply with the current port state */
        buf = p_port->port_ctrl;
        rfc_send_rpn(p_mcb, p_frame->dlci, 0, &p_port->port_ctrl, 0);
    } else {
        /* Copy current state, then apply the received parameters */
        buf = p_port->port_ctrl;
        rfc_set_port_state((u8 *)&buf, p_frame);

        if (is_command) {
            PORT_PortNegInd(p_mcb, p_frame->dlci, &buf, p_frame->u.rpn.param_mask);
        } else {
            /* Response */
            p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
            if ((p_port == NULL) || !(p_port->rfc_flags & (RFC_FLAG_EXPECT_RPN | RFC_FLAG_EXPECT_RPN2))) {
                return;
            }
            rfc_port_timer_stop(p_port);
            if (p_port->rfc_flags & RFC_FLAG_EXPECT_RPN) {
                p_port->rfc_flags &= ~RFC_FLAG_EXPECT_RPN;
                p_port->port_ctrl = buf;
                if (buf.fc_type == 0x0C || buf.fc_type == 0x30) {
                    p_frame->u.rpn.param_mask = 0x3F7F;
                } else {
                    p_port->port_ctrl.fc_type = 0x0C;
                    p_port->rfc_flags |= RFC_FLAG_EXPECT_RPN2;
                    rfc_send_rpn(p_mcb, p_frame->dlci, 1, &p_port->port_ctrl, 0x0C00);
                    rfc_port_timer_start(p_port, 0x3C);
                    return;
                }
            } else {
                p_port->rfc_flags &= ~RFC_FLAG_EXPECT_RPN2;
            }

            if ((p_frame->u.rpn.param_mask & 0x0C00) == 0x0C00
                || (p_frame->u.rpn.param_mask & 0x3000) == 0x3000) {
                PORT_PortNegCnf(p_mcb, p_port->dlci, &buf, 0);
            } else if (p_port->port_ctrl.fc_type == 0x0C) {
                p_port->port_ctrl.fc_type = 0x30;
                p_port->rfc_flags |= RFC_FLAG_EXPECT_RPN2;
                rfc_send_rpn(p_mcb, p_frame->dlci, 1, &p_port->port_ctrl, 0x3000);
                rfc_port_timer_start(p_port, 0x3C);
            } else if (p_port->port_ctrl.fc_type == 0x30) {
                p_port->port_ctrl.fc_type = 0;
                PORT_PortNegCnf(p_mcb, p_port->dlci, &buf, 0);
            }
        }
    }
}

/* ================================================================== */
/*  rfc_process_msc                                                   */
/* ================================================================== */
void rfc_process_msc(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame)
{
    tPORT *p_port;
    u8 signals = p_frame->u.msc.signals;
    tMSC_CTRL port_ctrl;

    p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
    if (p_port == NULL) {
        return;
    }

    /* Extract modem signals into the local control block */
    port_ctrl.fc = 0;
    if (signals & 0x04) {
        port_ctrl.fc |= 0x01;
    }
    if (signals & 0x08) {
        port_ctrl.fc |= 0x02;
    }
    if (signals & 0x40) {
        port_ctrl.fc |= 0x04;
    }
    if (signals & 0x80) {
        port_ctrl.fc |= 0x08;
    }
    port_ctrl.break_signal = (signals >> 1) & 1;
    port_ctrl.break_duration = p_frame->u.msc.break_present
                                   ? p_frame->u.msc.break_duration : 0;
    port_ctrl.disc = 0;
    port_ctrl.dtr = 1;

    if (is_command) {
        /* Send the response and report the new state */
        rfc_send_msc(p_mcb, p_frame->dlci, 0, &port_ctrl);
        if (p_port->p_mcb->flow != 2) {
            u8 break_signal = port_ctrl.break_signal;
            p_port->field_63 = break_signal;
            if (break_signal != p_port->field_24) {
                PORT_FlowInd(p_mcb, p_frame->dlci,
                             (u8)((u32)__cntlzw(break_signal) >> 5));
            }
        }
        PORT_ControlInd(p_mcb, p_frame->dlci, &port_ctrl);
    } else {
        /* Response */
        if (p_port->rfc_flags & RFC_FLAG_EXPECT_MSC) {
            p_port->rfc_flags &= ~RFC_FLAG_EXPECT_MSC;
            rfc_port_timer_stop(p_port);
            PORT_ControlCnf(p_port->p_mcb, p_port->dlci, &port_ctrl);
        }
    }
}

/* ================================================================== */
/*  rfc_process_rls                                                   */
/* ================================================================== */
void rfc_process_rls(tRFC_MCB *p_mcb, u8 is_command, MX_FRAME *p_frame)
{
    tPORT *p_port;

    if (is_command) {
        /* Request: report line status and send the response */
        PORT_LineStatusInd(p_mcb, p_frame->dlci, p_frame->u.rls.line_status);
        rfc_send_rls(p_mcb, p_frame->dlci, 0, p_frame->u.rls.line_status);
    } else {
        /* Response */
        p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
        if ((p_port == NULL) || !(p_port->rfc_flags & RFC_FLAG_EXPECT_RLS)) {
            return;
        }
        p_port->rfc_flags &= ~RFC_FLAG_EXPECT_RLS;
        rfc_port_timer_stop(p_port);
    }
}

/* ================================================================== */
/*  rfc_process_nsc / rfc_process_test_rsp                            */
/* ================================================================== */
void rfc_process_nsc(tRFC_MCB *p_mcb, MX_FRAME *p_frame)
{
}

void rfc_process_test_rsp(tRFC_MCB *p_mcb, BT_HDR *p_buf)
{
    GKI_freebuf(p_buf);
}

/* ================================================================== */
/*  rfc_process_fcon / rfc_process_fcoff                              */
/* ================================================================== */
void rfc_process_fcon(tRFC_MCB *p_mcb, u8 is_command)
{
    if (is_command) {
        rfc_cb.flow = 0;
        rfc_send_fcon(p_mcb, 0);
        if (p_mcb->l2cap_congested == 0) {
            PORT_FlowInd(p_mcb, 0, 1);
        }
    }
}

void rfc_process_fcoff(tRFC_MCB *p_mcb, u8 is_command)
{
    if (is_command) {
        rfc_cb.flow = 1;
        if (p_mcb->l2cap_congested == 0) {
            PORT_FlowInd(p_mcb, 0, 0);
        }
        rfc_send_fcoff(p_mcb, 0);
    }
}

/* ================================================================== */
/*  rfc_process_l2cap_congestion                                      */
/* ================================================================== */
void rfc_process_l2cap_congestion(tRFC_MCB *p_mcb, u8 is_congested)
{
    p_mcb->l2cap_congested = is_congested;
    if (!is_congested) {
        rfc_check_send_cmd(p_mcb, NULL);
    }

    if (rfc_cb.flow == 0) {
        if (!is_congested) {
            PORT_FlowInd(p_mcb, 0, 1);
        } else {
            PORT_FlowInd(p_mcb, 0, 0);
        }
    }
}

/* ================================================================== */
/*  rfc_set_port_state                                                */
/* ================================================================== */
void rfc_set_port_state(u8 *p_port_state, MX_FRAME *p_frame)
{
    if (p_frame->u.rpn.param_mask & 0x0001) {
        p_port_state[0] = p_frame->u.rpn.baud_rate;
    }
    if (p_frame->u.rpn.param_mask & 0x0002) {
        p_port_state[1] = p_frame->u.rpn.byte_size;
    }
    if (p_frame->u.rpn.param_mask & 0x0004) {
        p_port_state[2] = p_frame->u.rpn.stop_bits;
    }
    if (p_frame->u.rpn.param_mask & 0x0008) {
        p_port_state[3] = p_frame->u.rpn.parity;
    }
    if (p_frame->u.rpn.param_mask & 0x0010) {
        p_port_state[4] = p_frame->u.rpn.parity_type;
    }
    if (p_frame->u.rpn.param_mask & 0x3F00) {
        p_port_state[5] = p_frame->u.rpn.fc_type;
    }
    if (p_frame->u.rpn.param_mask & 0x0020) {
        p_port_state[7] = p_frame->u.rpn.xon_char;
    }
    if (p_frame->u.rpn.param_mask & 0x0040) {
        p_port_state[8] = p_frame->u.rpn.xoff_char;
    }
}
