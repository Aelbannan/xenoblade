// High-level C reconstruction of RVL_SDK/src/revolution/hbm/mix
//
// Home Button Mini library audio mixer (a fork of the RVL SDK MIX mixer).
// Maintains 16 internal mixer channels; each is attached to an AX voice
// (AXVPB) and drives its parameter block (main/AuxA/AuxB/AuxC mixes, VE,
// remote mix) with per-frame deltas.

/* "libs/RVL_SDK/src/revolution/hbm/mix.c" line 7 "types.h" */
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
/* "libs/RVL_SDK/src/revolution/hbm/mix.c" line 8 "string.h" */
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
/* "libs/RVL_SDK/src/revolution/hbm/mix.c" line 9 "revolution/ax/AXVPB.h" */
#ifndef RVL_SDK_AX_VPB_H
#define RVL_SDK_AX_VPB_H
/* "libs/RVL_SDK/include/revolution/ax/AXVPB.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/ax/AXVPB.h" line 4 "revolution/AX/AXPB.h" */
#ifndef RVL_SDK_AX_PB_H
#define RVL_SDK_AX_PB_H
/* "libs/RVL_SDK/include/revolution/AX/AXPB.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * One frame contains eight bytes:
 * - One for the header
 * - Seven for the audio samples
 */
#define AX_ADPCM_FRAME_SIZE 8
#define AX_ADPCM_SAMPLE_BYTES_PER_FRAME (AX_ADPCM_FRAME_SIZE - 1)

// Two audio samples per byte (each nibble)
#define AX_ADPCM_SAMPLES_PER_BYTE 2

// Amount of audio samples in a frame
#define AX_ADPCM_SAMPLES_PER_FRAME                                             \
    (AX_ADPCM_SAMPLE_BYTES_PER_FRAME * AX_ADPCM_SAMPLES_PER_BYTE)

// Amount of nibbles in a frame
#define AX_ADPCM_NIBBLES_PER_FRAME (AX_ADPCM_FRAME_SIZE * 2)

typedef enum { AX_VOICE_NORMAL, AX_VOICE_STREAM } AXVOICETYPE;

typedef enum { AX_VOICE_STOP, AX_VOICE_RUN } AXVOICESTATE;

typedef enum {
    AX_SAMPLE_FORMAT_DSP_ADPCM = 0,
    AX_SAMPLE_FORMAT_PCM_S16 = 10,
    AX_SAMPLE_FORMAT_PCM_S8 = 25,
} AXSAMPLETYPE;

// For rmtIIR union I think? From NW4R asserts, but fits well in __AXSyncPBs
typedef enum {
    AX_PB_LPF_ON = 1,
    AX_PB_BIQUAD_ON,
};

typedef enum {
    AX_SRC_TYPE_NONE,
    AX_SRC_TYPE_LINEAR,
    AX_SRC_TYPE_4TAP_8K,
    AX_SRC_TYPE_4TAP_12K,
    AX_SRC_TYPE_4TAP_16K,
    AX_SRC_TYPE_4TAP_AUTO
} AXPBSRCTYPE;

typedef enum {
    AX_MIXER_CTRL_L = (1 << 0),
    AX_MIXER_CTRL_R = (1 << 1),
    AX_MIXER_CTRL_DELTA = (1 << 2),
    AX_MIXER_CTRL_S = (1 << 3),
    AX_MIXER_CTRL_DELTA_S = (1 << 4),

    AX_MIXER_CTRL_AL = (1 << 16),
    AX_MIXER_CTRL_AR = (1 << 17),
    AX_MIXER_CTRL_DELTA_A = (1 << 18),
    AX_MIXER_CTRL_AS = (1 << 19),
    AX_MIXER_CTRL_DELTA_AS = (1 << 20),

    AX_MIXER_CTRL_BL = (1 << 21),
    AX_MIXER_CTRL_BR = (1 << 22),
    AX_MIXER_CTRL_DELTA_B = (1 << 23),
    AX_MIXER_CTRL_BS = (1 << 24),
    AX_MIXER_CTRL_DELTA_BS = (1 << 25),

    AX_MIXER_CTRL_CL = (1 << 26),
    AX_MIXER_CTRL_CR = (1 << 27),
    AX_MIXER_CTRL_DELTA_C = (1 << 28),
    AX_MIXER_CTRL_CS = (1 << 29),
    AX_MIXER_CTRL_DELTA_CS = (1 << 30)
};

typedef enum {
    AX_MIXER_CTRL_RMT_M0 = (1 << 0),
    AX_MIXER_CTRL_RMT_DELTA_M0 = (1 << 1),
    AX_MIXER_CTRL_RMT_A0 = (1 << 2),
    AX_MIXER_CTRL_RMT_DELTA_A0 = (1 << 3),

    AX_MIXER_CTRL_RMT_M1 = (1 << 4),
    AX_MIXER_CTRL_RMT_DELTA_M1 = (1 << 5),
    AX_MIXER_CTRL_RMT_A1 = (1 << 6),
    AX_MIXER_CTRL_RMT_DELTA_A1 = (1 << 7),

    AX_MIXER_CTRL_RMT_M2 = (1 << 8),
    AX_MIXER_CTRL_RMT_DELTA_M2 = (1 << 9),
    AX_MIXER_CTRL_RMT_A2 = (1 << 10),
    AX_MIXER_CTRL_RMT_DELTA_A2 = (1 << 11),

    AX_MIXER_CTRL_RMT_M3 = (1 << 12),
    AX_MIXER_CTRL_RMT_DELTA_M3 = (1 << 13),
    AX_MIXER_CTRL_RMT_A3 = (1 << 14),
    AX_MIXER_CTRL_RMT_DELTA_A3 = (1 << 15)
};

typedef struct _AXPBMIX {
    u16 vL;          // at 0x0
    u16 vDeltaL;     // at 0x2
    u16 vR;          // at 0x4
    u16 vDeltaR;     // at 0x6
    u16 vAuxAL;      // at 0x8
    u16 vDeltaAuxAL; // at 0xA
    u16 vAuxAR;      // at 0xC
    u16 vDeltaAuxAR; // at 0xE
    u16 vAuxBL;      // at 0x10
    u16 vDeltaAuxBL; // at 0x12
    u16 vAuxBR;      // at 0x14
    u16 vDeltaAuxBR; // at 0x16
    u16 vAuxCL;      // at 0x18
    u16 vDeltaAuxCL; // at 0x1A
    u16 vAuxCR;      // at 0x1C
    u16 vDeltaAuxCR; // at 0x1E
    u16 vS;          // at 0x20
    u16 vDeltaS;     // at 0x22
    u16 vAuxAS;      // at 0x24
    u16 vDeltaAuxAS; // at 0x26
    u16 vAuxBS;      // at 0x28
    u16 vDeltaAuxBS; // at 0x2A
    u16 vAuxCS;      // at 0x2C
    u16 vDeltaAuxCS; // at 0x2E
} AXPBMIX;

typedef struct _AXPBITD {
    u16 flag;         // at 0x0
    u16 bufferHi;     // at 0x2
    u16 bufferLo;     // at 0x4
    u16 shiftL;       // at 0x6
    u16 shiftR;       // at 0x8
    u16 targetShiftL; // at 0xA
    u16 targetShiftR; // at 0xC
} AXPBITD;

typedef struct _AXPBDPOP {
    s16 aL;     // at 0x0
    s16 aAuxAL; // at 0x2
    s16 aAuxBL; // at 0x4
    s16 aAuxCL; // at 0x6
    s16 aR;     // at 0x8
    s16 aAuxAR; // at 0xA
    s16 aAuxBR; // at 0xC
    s16 aAuxCR; // at 0xE
    s16 aS;     // at 0x10
    s16 aAuxAS; // at 0x12
    s16 aAuxBS; // at 0x14
    s16 aAuxCS; // at 0x16
} AXPBDPOP;

typedef struct _AXPBVE {
    u16 currentVolume; // at 0x0
    s16 currentDelta;  // at 0x2
} AXPBVE;

typedef struct _AXPBADDR {
    u16 loopFlag;         // at 0x0
    u16 format;           // at 0x2
    u16 loopAddressHi;    // at 0x4
    u16 loopAddressLo;    // at 0x6
    u16 endAddressHi;     // at 0x8
    u16 endAddressLo;     // at 0xA
    u16 currentAddressHi; // at 0xC
    u16 currentAddressLo; // at 0xE
} AXPBADDR;

typedef struct _AXPBADPCM {
    u16 a[8][2];    // at 0x0
    u16 gain;       // at 0x20
    u16 pred_scale; // at 0x22
    u16 yn1;        // at 0x24
    u16 yn2;        // at 0x26
} AXPBADPCM;

typedef struct _AXPBSRC {
    u16 ratioHi;            // at 0x0
    u16 ratioLo;            // at 0x2
    u16 currentAddressFrac; // at 0x4
    u16 last_samples[4];    // at 0x6
} AXPBSRC;

typedef struct _AXPBADPCMLOOP {
    u16 loop_pred_scale; // at 0x0
    u16 loop_yn1;        // at 0x2
    u16 loop_yn2;        // at 0x4
} AXPBADPCMLOOP;

typedef struct _AXPBLPF {
    u16 on;  // at 0x0
    u16 yn1; // at 0x2
    u16 a0;  // at 0x4
    u16 b0;  // at 0x6
} AXPBLPF;

typedef struct _AXPBBIQUAD {
    u16 on;  // at 0x0
    u16 xn1; // at 0x2
    u16 xn2; // at 0x4
    u16 yn1; // at 0x6
    u16 yn2; // at 0x8
    u16 b0;  // at 0xA
    u16 b1;  // at 0xC
    u16 b2;  // at 0xE
    u16 a1;  // at 0x10
    u16 a2;  // at 0x12
} AXPBBIQUAD;

typedef struct _AXPBRMTMIX {
    u16 vMain0;      // at 0x0
    u16 vDeltaMain0; // at 0x2
    u16 vAux0;       // at 0x4
    u16 vDeltaAux0;  // at 0x6
    u16 vMain1;      // at 0x8
    u16 vDeltaMain1; // at 0xA
    u16 vAux1;       // at 0xC
    u16 vDeltaAux1;  // at 0xE
    u16 vMain2;      // at 0x10
    u16 vDeltaMain2; // at 0x12
    u16 vAux2;       // at 0x14
    u16 vDeltaAux2;  // at 0x16
    u16 vMain3;      // at 0x18
    u16 vDeltaMain3; // at 0x1A
    u16 vAux3;       // at 0x1C
    u16 vDeltaAux3;  // at 0x1E
} AXPBRMTMIX;

typedef struct _AXPBRMTDPOP {
    s16 aMain0; // at 0x0
    s16 aMain1; // at 0x2
    s16 aMain2; // at 0x4
    s16 aMain3; // at 0x6
    s16 aAux0;  // at 0x8
    s16 aAux1;  // at 0xA
    s16 aAux2;  // at 0xC
    s16 aAux3;  // at 0xE
} AXPBRMTDPOP;

typedef struct _AXPBRMTSRC {
    u16 currentAddressFrac; // at 0x0
    u16 last_samples[4];    // at 0x2
} AXPBRMTSRC;

typedef union __AXPBRMTIIR {
    AXPBLPF lpf;
    AXPBBIQUAD biquad;
} AXPBRMTIIR;

typedef struct _AXPB {
    u16 nextHi;                // at 0x0
    u16 nextLo;                // at 0x2
    u16 currHi;                // at 0x4
    u16 currLo;                // at 0x6
    u16 srcSelect;             // at 0x8
    u16 coefSelect;            // at 0xA
    u32 mixerCtrl;             // at 0xC
    u16 state;                 // at 0x10
    u16 type;                  // at 0x12
    AXPBMIX mix;               // at 0x14
    AXPBITD itd;               // at 0x44
    AXPBDPOP dpop;             // at 0x52
    AXPBVE ve;                 // at 0x6A
    AXPBADDR addr;             // at 0x6E
    AXPBADPCM adpcm;           // at 0x7E
    AXPBSRC src;               // at 0xA6
    AXPBADPCMLOOP adpcmLoop;   // at 0xB4
    AXPBLPF lpf;               // at 0xBA
    AXPBBIQUAD biquad;         // at 0xC2
    u16 remote;                // at 0xD6
    u16 rmtMixerCtrl;          // at 0xD8
    AXPBRMTMIX rmtMix;         // at 0xDA
    AXPBRMTDPOP rmtDpop;       // at 0xFA
    AXPBRMTSRC rmtSrc;         // at 0x10A
    AXPBRMTIIR rmtIIR;         // at 0x114
    u8 padding[0x140 - 0x128]; // at 0x128
} AXPB;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AX/AXPB.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define AX_SAMPLE_RATE 32000
#define AX_VOICE_MAX 96

typedef enum {
    AX_PBSYNC_SELECT = (1 << 0),
    AX_PBSYNC_MIXER_CTRL = (1 << 1),
    AX_PBSYNC_STATE = (1 << 2),
    AX_PBSYNC_TYPE = (1 << 3),
    AX_PBSYNC_MIX = (1 << 4),
    AX_PBSYNC_ITD = (1 << 5),
    AX_PBSYNC_ITD_SHIFT = (1 << 6),
    AX_PBSYNC_DPOP = (1 << 7),
    AX_PBSYNC_VE = (1 << 8),
    AX_PBSYNC_VE_DELTA = (1 << 9),
    AX_PBSYNC_ADDR = (1 << 10),
    AX_PBSYNC_LOOP_FLAG = (1 << 11),
    AX_PBSYNC_LOOP_ADDR = (1 << 12),
    AX_PBSYNC_END_ADDR = (1 << 13),
    AX_PBSYNC_CURR_ADDR = (1 << 14),
    AX_PBSYNC_ADPCM = (1 << 15),
    AX_PBSYNC_SRC = (1 << 16),
    AX_PBSYNC_SRC_RATIO = (1 << 17),
    AX_PBSYNC_ADPCM_LOOP = (1 << 18),
    AX_PBSYNC_LPF = (1 << 19),
    AX_PBSYNC_LPF_COEFS = (1 << 20),
    AX_PBSYNC_BIQUAD = (1 << 21),
    AX_PBSYNC_BIQUAD_COEFS = (1 << 22),
    AX_PBSYNC_REMOTE = (1 << 23),
    AX_PBSYNC_RMT_MIXER_CTRL = (1 << 24),
    AX_PBSYNC_RMTMIX = (1 << 25),
    AX_PBSYNC_RMTDPOP = (1 << 26),
    AX_PBSYNC_RMTSRC = (1 << 27),
    AX_PBSYNC_RMTIIR = (1 << 28),
    AX_PBSYNC_RMTIIR_LPF_COEFS = (1 << 29),
    AX_PBSYNC_RMTIIR_BIQUAD_COEFS = (1 << 30),
    AX_PBSYNC_ALL = (1 << 31),
};

typedef void (*AXVoiceCallback)(void* vpb);

typedef struct _AXVPB {
    void* next;               // at 0x0
    void* prev;               // at 0x4
    void* next1;              // at 0x8
    u32 priority;             // at 0xC
    AXVoiceCallback callback; // at 0x10
    u32 userContext;          // at 0x14
    u32 index;                // at 0x18
    u32 sync;                 // at 0x1C
    u32 depop;                // at 0x20
    void* itdBuffer;          // at 0x24
    AXPB pb;                  // at 0x28
} AXVPB;

s32 __AXGetNumVoices(void);
void __AXServiceVPB(AXVPB* vpb);
void __AXDumpVPB(AXVPB* vpb);
void __AXSyncPBs(u32 baseCycles);
AXPB* __AXGetPBs(void);
void __AXSetPBDefault(AXVPB* vpb);
void __AXVPBInit(void);
void __AXVPBQuit(void);
void AXSetVoiceState(AXVPB* vpb, u16 state);
void AXSetVoiceAddr(AXVPB* vpb, AXPBADDR* addr);
void AXGetLpfCoefs(u16 freq, u16* a, u16* b);
void AXSetMaxDspCycles(u32 num);
s32 AXGetMaxVoices(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/ax/AXVPB.h" */
/* "libs/RVL_SDK/src/revolution/hbm/mix.c" line 10 "revolution/os/OSInterrupt.h" */
#ifndef RVL_SDK_OS_INTERRUPT_H
#define RVL_SDK_OS_INTERRUPT_H
/* "libs/RVL_SDK/include/revolution/os/OSInterrupt.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

// Create mask from interrupt ID
#define OS_INTR_MASK(intr) (1 << (31 - intr))

typedef enum {
    OS_INTR_MEM_0,
    OS_INTR_MEM_1,
    OS_INTR_MEM_2,
    OS_INTR_MEM_3,
    OS_INTR_MEM_ADDRESS,
    OS_INTR_DSP_AI,
    OS_INTR_DSP_ARAM,
    OS_INTR_DSP_DSP,
    OS_INTR_AI_AI,
    OS_INTR_EXI_0_EXI,
    OS_INTR_EXI_0_TC,
    OS_INTR_EXI_0_EXT,
    OS_INTR_EXI_1_EXI,
    OS_INTR_EXI_1_TC,
    OS_INTR_EXI_1_EXT,
    OS_INTR_EXI_2_EXI,
    OS_INTR_EXI_2_TC,
    OS_INTR_PI_CP,
    OS_INTR_PI_PE_TOKEN,
    OS_INTR_PI_PE_FINISH,
    OS_INTR_PI_SI,
    OS_INTR_PI_DI,
    OS_INTR_PI_RSW,
    OS_INTR_PI_ERROR,
    OS_INTR_PI_VI,
    OS_INTR_PI_DEBUG,
    OS_INTR_PI_HSP,
    OS_INTR_PI_ACR,
    OS_INTR_28,
    OS_INTR_29,
    OS_INTR_30,
    OS_INTR_31,

    OS_INTR_MAX
} OSInterruptType;

//TODO: is this s16 or s32???
typedef void (*OSInterruptHandler)(s32 intr, struct OSContext* ctx);

extern u32 __OSLastInterruptSrr0;
extern s16 __OSLastInterrupt;
extern s64 __OSLastInterruptTime;

BOOL OSDisableInterrupts(void);
BOOL OSEnableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL status);

OSInterruptHandler __OSSetInterruptHandler(OSInterruptType type,
                                           OSInterruptHandler handler);
OSInterruptHandler __OSGetInterruptHandler(OSInterruptType type);

void __OSInterruptInit(void);

u32 __OSMaskInterrupts(u32 userMask);
u32 __OSUnmaskInterrupts(u32 userMask);
void __OSDispatchInterrupt(u8 intr, struct OSContext* ctx);

void __RAS_OSDisableInterrupts_begin(void);
void __RAS_OSDisableInterrupts_end(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/os/OSInterrupt.h" */

BOOL AXIsInit(void);

typedef struct HBMMIXChannel {
    AXVPB* vpb;        // at 0x00 — attached AX voice (NULL = free slot)
    u32 flags;         // at 0x04 — pending update flags
    s32 input;         // at 0x08 — main input volume (dB x10)
    s32 auxA;          // at 0x0C — AuxA send volume (dB x10)
    s32 pan;           // at 0x10 — pan [0, 127]
    s32 fader;         // at 0x14 — fader volume (dB x10)
    s32 panL;          // at 0x18 — left pan attenuation (dB x10)
    s32 panR;          // at 0x1C — right pan attenuation (dB x10)
    s32 panFrontL;     // at 0x20 — front-left base attenuation
    s32 panFrontR;     // at 0x24 — front-right base attenuation
    s32 panRearL;      // at 0x28 — rear-left attenuation (DPL2)
    s32 panRearR;      // at 0x2C — rear-right attenuation (DPL2)
    u16 vMain;         // at 0x30
    u16 vMainTarget;   // at 0x32
    u16 vL;            // at 0x34
    u16 vLTarget;      // at 0x36
    u16 vR;            // at 0x38
    u16 vRTarget;      // at 0x3A
    u16 vS;            // at 0x3C
    u16 vSTarget;      // at 0x3E
    u16 vAuxAL;        // at 0x40
    u16 vAuxALTarget;  // at 0x42
    u16 vAuxAR;        // at 0x44
    u16 vAuxARTarget;  // at 0x46
    u16 vAuxAS;        // at 0x48
    u16 vAuxASTarget;  // at 0x4A
    u16 vAuxBL;        // at 0x4C
    u16 vAuxBLTarget;  // at 0x4E
    u16 vAuxBR;        // at 0x50
    u16 vAuxBRTarget;  // at 0x52
    u16 vAuxBS;        // at 0x54
    u16 vAuxBSTarget;  // at 0x56
    u16 vAuxCL;        // at 0x58
    u16 vAuxCLTarget;  // at 0x5A
    u16 vAuxCR;        // at 0x5C
    u16 vAuxCRTarget;  // at 0x5E
    u16 vAuxCS;        // at 0x60
    u16 vAuxCSTarget;  // at 0x62
} HBMMIXChannel; // size 0x64

// Channel update flags (HBMMIXChannel.flags)
#define HBMMIX_FLAG_INPUT 0x10000000u // input volume changed -> new vMainTarget
#define HBMMIX_FLAG_MAIN  0x20000000u // vMainTarget ready -> apply to vMain
#define HBMMIX_FLAG_MIX   0x40000000u // auxA/pan/fader changed -> new mix targets
#define HBMMIX_FLAG_APPLY 0x80000000u // mix targets ready -> apply to currents

// Sound modes (__HBMMIXSoundMode)
#define HBMMIX_MODE_MONO    0
#define HBMMIX_MODE_STEREO  1
#define HBMMIX_MODE_DPL2    2

#define HBMMIX_NUM_CHANNELS 16

// dB limits for the volume table (dB x 10)
#define HBMMIX_DB_MIN (-904)
#define HBMMIX_DB_MAX 60

// Per-frame volume delta divisor (full-scale fade ~256 frames)
#define HBMMIX_DELTA_UNIT 24576

// Retail volume/pan tables (data-exact copies from build/us/asm/.../mix.s)
u16 __HBMMIXVolumeTable[968] = {
    0x0000, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
    0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
    0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
    0x0002, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003,
    0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003,
    0x0003, 0x0003, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004,
    0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0005, 0x0005, 0x0005,
    0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0005,
    0x0005, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006,
    0x0006, 0x0006, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007,
    0x0007, 0x0007, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008,
    0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x000A, 0x000A, 0x000A,
    0x000A, 0x000A, 0x000A, 0x000A, 0x000A, 0x000A, 0x000B, 0x000B, 0x000B, 0x000B, 0x000B, 0x000B,
    0x000B, 0x000C, 0x000C, 0x000C, 0x000C, 0x000C, 0x000C, 0x000C, 0x000D, 0x000D, 0x000D, 0x000D,
    0x000D, 0x000D, 0x000D, 0x000E, 0x000E, 0x000E, 0x000E, 0x000E, 0x000E, 0x000F, 0x000F, 0x000F,
    0x000F, 0x000F, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011,
    0x0012, 0x0012, 0x0012, 0x0012, 0x0012, 0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0014, 0x0014,
    0x0014, 0x0014, 0x0015, 0x0015, 0x0015, 0x0015, 0x0016, 0x0016, 0x0016, 0x0016, 0x0017, 0x0017,
    0x0017, 0x0018, 0x0018, 0x0018, 0x0018, 0x0019, 0x0019, 0x0019, 0x001A, 0x001A, 0x001A, 0x001A,
    0x001B, 0x001B, 0x001B, 0x001C, 0x001C, 0x001C, 0x001D, 0x001D, 0x001D, 0x001E, 0x001E, 0x001E,
    0x001F, 0x001F, 0x0020, 0x0020, 0x0020, 0x0021, 0x0021, 0x0021, 0x0022, 0x0022, 0x0023, 0x0023,
    0x0023, 0x0024, 0x0024, 0x0025, 0x0025, 0x0026, 0x0026, 0x0026, 0x0027, 0x0027, 0x0028, 0x0028,
    0x0029, 0x0029, 0x002A, 0x002A, 0x002B, 0x002B, 0x002C, 0x002C, 0x002D, 0x002D, 0x002E, 0x002E,
    0x002F, 0x002F, 0x0030, 0x0031, 0x0031, 0x0032, 0x0032, 0x0033, 0x0033, 0x0034, 0x0035, 0x0035,
    0x0036, 0x0037, 0x0037, 0x0038, 0x0038, 0x0039, 0x003A, 0x003A, 0x003B, 0x003C, 0x003D, 0x003D,
    0x003E, 0x003F, 0x003F, 0x0040, 0x0041, 0x0042, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0046,
    0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051,
    0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D,
    0x005E, 0x005F, 0x0060, 0x0061, 0x0062, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x006A, 0x006B,
    0x006C, 0x006D, 0x006F, 0x0070, 0x0071, 0x0072, 0x0074, 0x0075, 0x0076, 0x0078, 0x0079, 0x007B,
    0x007C, 0x007E, 0x007F, 0x0080, 0x0082, 0x0083, 0x0085, 0x0087, 0x0088, 0x008A, 0x008B, 0x008D,
    0x008F, 0x0090, 0x0092, 0x0094, 0x0095, 0x0097, 0x0099, 0x009B, 0x009C, 0x009E, 0x00A0, 0x00A2,
    0x00A4, 0x00A6, 0x00A8, 0x00AA, 0x00AB, 0x00AD, 0x00AF, 0x00B2, 0x00B4, 0x00B6, 0x00B8, 0x00BA,
    0x00BC, 0x00BE, 0x00C0, 0x00C3, 0x00C5, 0x00C7, 0x00CA, 0x00CC, 0x00CE, 0x00D1, 0x00D3, 0x00D6,
    0x00D8, 0x00DB, 0x00DD, 0x00E0, 0x00E2, 0x00E5, 0x00E7, 0x00EA, 0x00ED, 0x00F0, 0x00F2, 0x00F5,
    0x00F8, 0x00FB, 0x00FE, 0x0101, 0x0104, 0x0107, 0x010A, 0x010D, 0x0110, 0x0113, 0x0116, 0x011A,
    0x011D, 0x0120, 0x0124, 0x0127, 0x012A, 0x012E, 0x0131, 0x0135, 0x0138, 0x013C, 0x0140, 0x0143,
    0x0147, 0x014B, 0x014F, 0x0153, 0x0157, 0x015B, 0x015F, 0x0163, 0x0167, 0x016B, 0x016F, 0x0173,
    0x0178, 0x017C, 0x0180, 0x0185, 0x0189, 0x018E, 0x0193, 0x0197, 0x019C, 0x01A1, 0x01A6, 0x01AB,
    0x01AF, 0x01B4, 0x01BA, 0x01BF, 0x01C4, 0x01C9, 0x01CE, 0x01D4, 0x01D9, 0x01DF, 0x01E4, 0x01EA,
    0x01EF, 0x01F5, 0x01FB, 0x0201, 0x0207, 0x020D, 0x0213, 0x0219, 0x021F, 0x0226, 0x022C, 0x0232,
    0x0239, 0x0240, 0x0246, 0x024D, 0x0254, 0x025B, 0x0262, 0x0269, 0x0270, 0x0277, 0x027E, 0x0286,
    0x028D, 0x0295, 0x029D, 0x02A4, 0x02AC, 0x02B4, 0x02BC, 0x02C4, 0x02CC, 0x02D5, 0x02DD, 0x02E6,
    0x02EE, 0x02F7, 0x0300, 0x0309, 0x0312, 0x031B, 0x0324, 0x032D, 0x0337, 0x0340, 0x034A, 0x0354,
    0x035D, 0x0367, 0x0371, 0x037C, 0x0386, 0x0390, 0x039B, 0x03A6, 0x03B1, 0x03BB, 0x03C7, 0x03D2,
    0x03DD, 0x03E9, 0x03F4, 0x0400, 0x040C, 0x0418, 0x0424, 0x0430, 0x043D, 0x0449, 0x0456, 0x0463,
    0x0470, 0x047D, 0x048A, 0x0498, 0x04A5, 0x04B3, 0x04C1, 0x04CF, 0x04DD, 0x04EC, 0x04FA, 0x0509,
    0x0518, 0x0527, 0x0536, 0x0546, 0x0555, 0x0565, 0x0575, 0x0586, 0x0596, 0x05A6, 0x05B7, 0x05C8,
    0x05D9, 0x05EB, 0x05FC, 0x060E, 0x0620, 0x0632, 0x0644, 0x0657, 0x066A, 0x067D, 0x0690, 0x06A4,
    0x06B7, 0x06CB, 0x06DF, 0x06F4, 0x0708, 0x071D, 0x0732, 0x0748, 0x075D, 0x0773, 0x0789, 0x079F,
    0x07B6, 0x07CD, 0x07E4, 0x07FB, 0x0813, 0x082B, 0x0843, 0x085C, 0x0874, 0x088E, 0x08A7, 0x08C1,
    0x08DA, 0x08F5, 0x090F, 0x092A, 0x0945, 0x0961, 0x097D, 0x0999, 0x09B5, 0x09D2, 0x09EF, 0x0A0D,
    0x0A2A, 0x0A48, 0x0A67, 0x0A86, 0x0AA5, 0x0AC5, 0x0AE5, 0x0B05, 0x0B25, 0x0B47, 0x0B68, 0x0B8A,
    0x0BAC, 0x0BCF, 0x0BF2, 0x0C15, 0x0C39, 0x0C5D, 0x0C82, 0x0CA7, 0x0CCC, 0x0CF2, 0x0D19, 0x0D3F,
    0x0D67, 0x0D8E, 0x0DB7, 0x0DDF, 0x0E08, 0x0E32, 0x0E5C, 0x0E87, 0x0EB2, 0x0EDD, 0x0F09, 0x0F36,
    0x0F63, 0x0F91, 0x0FBF, 0x0FEE, 0x101D, 0x104D, 0x107D, 0x10AE, 0x10DF, 0x1111, 0x1144, 0x1177,
    0x11AB, 0x11DF, 0x1214, 0x124A, 0x1280, 0x12B7, 0x12EE, 0x1326, 0x135F, 0x1399, 0x13D3, 0x140D,
    0x1449, 0x1485, 0x14C2, 0x14FF, 0x153E, 0x157D, 0x15BC, 0x15FD, 0x163E, 0x1680, 0x16C3, 0x1706,
    0x174A, 0x178F, 0x17D5, 0x181C, 0x1863, 0x18AC, 0x18F5, 0x193F, 0x198A, 0x19D5, 0x1A22, 0x1A6F,
    0x1ABE, 0x1B0D, 0x1B5D, 0x1BAE, 0x1C00, 0x1C53, 0x1CA7, 0x1CFC, 0x1D52, 0x1DA9, 0x1E01, 0x1E5A,
    0x1EB4, 0x1F0F, 0x1F6B, 0x1FC8, 0x2026, 0x2086, 0x20E6, 0x2148, 0x21AA, 0x220E, 0x2273, 0x22D9,
    0x2341, 0x23A9, 0x2413, 0x247E, 0x24EA, 0x2557, 0x25C6, 0x2636, 0x26A7, 0x271A, 0x278E, 0x2803,
    0x287A, 0x28F2, 0x296B, 0x29E6, 0x2A62, 0x2AE0, 0x2B5F, 0x2BDF, 0x2C61, 0x2CE5, 0x2D6A, 0x2DF1,
    0x2E79, 0x2F03, 0x2F8E, 0x301B, 0x30AA, 0x313A, 0x31CC, 0x325F, 0x32F5, 0x338C, 0x3425, 0x34BF,
    0x355B, 0x35FA, 0x369A, 0x373C, 0x37DF, 0x3885, 0x392C, 0x39D6, 0x3A81, 0x3B2F, 0x3BDE, 0x3C90,
    0x3D43, 0x3DF9, 0x3EB1, 0x3F6A, 0x4026, 0x40E5, 0x41A5, 0x4268, 0x432C, 0x43F4, 0x44BD, 0x4589,
    0x4657, 0x4727, 0x47FA, 0x48D0, 0x49A8, 0x4A82, 0x4B5F, 0x4C3E, 0x4D20, 0x4E05, 0x4EEC, 0x4FD6,
    0x50C3, 0x51B2, 0x52A4, 0x5399, 0x5491, 0x558C, 0x5689, 0x578A, 0x588D, 0x5994, 0x5A9D, 0x5BAA,
    0x5CBA, 0x5DCD, 0x5EE3, 0x5FFC, 0x6119, 0x6238, 0x635C, 0x6482, 0x65AC, 0x66D9, 0x680A, 0x693F,
    0x6A77, 0x6BB2, 0x6CF2, 0x6E35, 0x6F7B, 0x70C6, 0x7214, 0x7366, 0x74BC, 0x7616, 0x7774, 0x78D6,
    0x7A3D, 0x7BA7, 0x7D16, 0x7E88, 0x7FFF, 0x817B, 0x82FB, 0x847F, 0x8608, 0x8795, 0x8927, 0x8ABE,
    0x8C59, 0x8DF9, 0x8F9E, 0x9148, 0x92F6, 0x94AA, 0x9663, 0x9820, 0x99E3, 0x9BAB, 0x9D79, 0x9F4C,
    0xA124, 0xA302, 0xA4E5, 0xA6CE, 0xA8BC, 0xAAB0, 0xACAA, 0xAEAA, 0xB0B0, 0xB2BC, 0xB4CE, 0xB6E5,
    0xB904, 0xBB28, 0xBD53, 0xBF84, 0xC1BC, 0xC3FA, 0xC63F, 0xC88B, 0xCADD, 0xCD37, 0xCF97, 0xD1FE,
    0xD46D, 0xD6E3, 0xD960, 0xDBE4, 0xDE70, 0xE103, 0xE39E, 0xE641, 0xE8EB, 0xEB9E, 0xEE58, 0xF11B,
    0xF3E6, 0xF6B9, 0xF994, 0xFC78, 0xFF64, 0x0000, 0x0000, 0x0000,
};

s32 __HBMMIXPanTable[128] = {
    0, 0, -1, -1, -1, -2, -2, -2,
    -3, -3, -4, -4, -4, -5, -5, -5,
    -6, -6, -7, -7, -7, -8, -8, -9,
    -9, -10, -10, -10, -11, -11, -12, -12,
    -13, -13, -14, -14, -14, -15, -15, -16,
    -16, -17, -17, -18, -18, -19, -20, -20,
    -21, -21, -22, -22, -23, -23, -24, -25,
    -25, -26, -26, -27, -28, -28, -29, -30,
    -30, -31, -32, -33, -33, -34, -35, -36,
    -36, -37, -38, -39, -40, -40, -41, -42,
    -43, -44, -45, -46, -47, -48, -49, -50,
    -51, -52, -54, -55, -56, -57, -59, -60,
    -61, -63, -64, -66, -67, -69, -71, -72,
    -74, -76, -78, -80, -83, -85, -87, -90,
    -93, -96, -99, -102, -106, -110, -115, -120,
    -126, -133, -140, -150, -163, -180, -210, -904,
};

s16 __HBMMIX_DPL2_front[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3,
    -3, -4, -4, -4, -5, -5, -6, -6, -6, -7, -7, -8,
    -8, -9, -9, -10, -11, -11, -12, -12, -13, -14, -14, -15,
    -16, -17, -17, -18, -19, -20, -21, -21, -22, -23, -24, -25,
    -26, -27, -28, -29, -30, -31, -32, -34, -35, -36, -37, -38,
    -40, -41, -42, -44, -45, -47, -48, -50, -52, -53, -55, -57,
    -58, -60, -62, -64, -66, -68, -70, -73, -75, -77, -80, -82,
    -85, -88, -90, -93, -96, -100, -103, -106, -110, -114, -118, -122,
    -126, -131, -136, -141, -146, -152, -159, -166, -173, -181, -190, -201,
    -212, -225, -241, -261, -286, -321, -381, -960,
};

s16 __HBMMIX_DPL2_rear[128] = {
    -61, -61, -60, -59, -59, -58, -58, -57, -56, -56, -55, -55,
    -54, -53, -53, -52, -52, -51, -50, -50, -49, -49, -48, -48,
    -47, -47, -46, -46, -45, -45, -44, -44, -43, -43, -42, -42,
    -41, -41, -40, -40, -39, -39, -38, -38, -38, -37, -37, -36,
    -36, -35, -35, -35, -34, -34, -33, -33, -32, -32, -32, -31,
    -31, -31, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27,
    -27, -26, -26, -26, -25, -25, -25, -24, -24, -24, -23, -23,
    -23, -22, -22, -22, -21, -21, -21, -20, -20, -20, -20, -19,
    -19, -19, -18, -18, -18, -18, -17, -17, -17, -17, -16, -16,
    -16, -16, -15, -15, -15, -15, -14, -14, -14, -14, -13, -13,
    -13, -13, -13, -12, -12, -12, -12, -11,
};

s8 sIndex2IdTable[16];
s8 sId2IndexTable[96];
u32 __HBMMIXSoundMode;
HBMMIXChannel* __HBMMIXChannel;
HBMMIXChannel __s_HBMMIXChannel[HBMMIX_NUM_CHANNELS];
static u32 __init;

static u16 __HBMMIXGetVolume(s32 db) {
    if (db <= HBMMIX_DB_MIN) {
        return 0;
    }
    if (db >= HBMMIX_DB_MAX) {
        return 0xFF64;
    }
    return __HBMMIXVolumeTable[db - HBMMIX_DB_MIN];
}

void __HBMMIXSetPan(HBMMIXChannel* ch) {
    s32 pan = ch->pan;
    s32 inv = 127 - pan;

    if (__HBMMIXSoundMode == HBMMIX_MODE_DPL2) {
        ch->panL = __HBMMIX_DPL2_front[pan];
        ch->panR = __HBMMIX_DPL2_front[inv];
        ch->panFrontL = __HBMMIX_DPL2_front[0];
        ch->panFrontR = __HBMMIX_DPL2_front[127];
        ch->panRearL = __HBMMIX_DPL2_rear[inv];
        ch->panRearR = __HBMMIX_DPL2_rear[pan];
    } else {
        ch->panL = __HBMMIXPanTable[pan];
        ch->panR = __HBMMIXPanTable[inv];
        ch->panFrontL = __HBMMIXPanTable[0];
        ch->panFrontR = __HBMMIXPanTable[127];
        ch->panRearL = 0;
        ch->panRearR = 0;
    }
}

extern "C" {

void HBMMIXInit(void) {
    s32 i;

    if (AXIsInit() && !__init) {
        for (i = 0; i < 16; i++) {
            sIndex2IdTable[i] = -1;
        }
        for (i = 0; i < 96; i++) {
            sId2IndexTable[i] = -1;
        }

        __HBMMIXChannel = __s_HBMMIXChannel;

        for (i = 0; i < HBMMIX_NUM_CHANNELS; i++) {
            HBMMIXChannel* ch;

            __HBMMIXChannel[i].vpb = NULL;
            ch = &__HBMMIXChannel[i];
            ch->flags = 0x50000000;
            ch->input = 0;
            ch->auxA = -960;
            ch->pan = 64;
            ch->fader = 0;
            ch->vAuxCS = 0;
            ch->vAuxCR = 0;
            ch->vAuxCL = 0;
            ch->vAuxBS = 0;
            ch->vAuxBR = 0;
            ch->vAuxBL = 0;
            ch->vAuxAS = 0;
            ch->vAuxAR = 0;
            ch->vAuxAL = 0;
            ch->vS = 0;
            ch->vR = 0;
            ch->vL = 0;
            ch->vMain = 0;
            __HBMMIXSetPan(ch);
        }

        __HBMMIXSoundMode = HBMMIX_MODE_STEREO;
        __init = 1;
    }
}

void HBMMIXQuit(void) {
    __HBMMIXChannel = NULL;
    __init = 0;
}

void HBMMIXSetSoundMode(u32 mode) {
    __HBMMIXSoundMode = mode;
}

void HBMMIXInitChannel(AXVPB* vpb, s32 input, s32 auxA, s32 pan, s32 fader) {
    HBMMIXChannel* ch = &__HBMMIXChannel[sId2IndexTable[vpb->index]];
    u32 ctrl = 0;
    BOOL enabled;

    ch->vpb = vpb;
    ch->flags = 0;
    ch->input = input;
    ch->auxA = auxA;
    ch->pan = pan;
    ch->fader = fader;
    __HBMMIXSetPan(ch);

    ch->vMain = __HBMMIXGetVolume(input);

    switch (__HBMMIXSoundMode) {
    case HBMMIX_MODE_MONO:
        ch->vL = __HBMMIXGetVolume(fader + ch->panFrontL);
        ch->vR = __HBMMIXGetVolume(fader + ch->panFrontL);
        ch->vS = __HBMMIXGetVolume(fader + ch->panFrontR - 30);
        ch->vAuxAL = __HBMMIXGetVolume(auxA + fader + ch->panFrontL);
        ch->vAuxAR = __HBMMIXGetVolume(auxA + fader + ch->panFrontL);
        ch->vAuxAS = __HBMMIXGetVolume(fader + auxA + ch->panFrontR - 30);
        ch->vAuxBL = 0;
        ch->vAuxBR = 0;
        ch->vAuxBS = 0;
        ch->vAuxCL = 0;
        ch->vAuxCR = 0;
        ch->vAuxCS = 0;
        break;
    case HBMMIX_MODE_STEREO:
        ch->vL = __HBMMIXGetVolume(fader + ch->panFrontL + ch->panL);
        ch->vR = __HBMMIXGetVolume(fader + ch->panFrontL + ch->panR);
        ch->vS = __HBMMIXGetVolume(fader + ch->panFrontR - 30);
        ch->vAuxAL = __HBMMIXGetVolume(ch->panFrontL + ch->panL + fader + auxA);
        ch->vAuxAR = __HBMMIXGetVolume(ch->panFrontL + ch->panR + fader + auxA);
        ch->vAuxAS = __HBMMIXGetVolume(fader + auxA + ch->panFrontR - 30);
        ch->vAuxBL = 0;
        ch->vAuxBR = 0;
        ch->vAuxBS = 0;
        ch->vAuxCL = 0;
        ch->vAuxCR = 0;
        ch->vAuxCS = 0;
        break;
    case HBMMIX_MODE_DPL2:
        ch->vL = __HBMMIXGetVolume(fader + ch->panFrontL + ch->panL);
        ch->vR = __HBMMIXGetVolume(fader + ch->panFrontL + ch->panR);
        ch->vS = __HBMMIXGetVolume(fader + ch->panFrontR + ch->panRearL);
        ch->vAuxCL = __HBMMIXGetVolume(fader + ch->panFrontR + ch->panRearR);
        ch->vAuxAL = __HBMMIXGetVolume(ch->panFrontL + ch->panL + fader + auxA);
        ch->vAuxAR = __HBMMIXGetVolume(ch->panFrontL + ch->panR + fader + auxA);
        ch->vAuxAS = __HBMMIXGetVolume(ch->panFrontR + ch->panRearL + fader + auxA);
        ch->vAuxCR = __HBMMIXGetVolume(ch->panFrontR + ch->panRearR + fader + auxA);
        ch->vAuxBL = 0;
        ch->vAuxBR = 0;
        ch->vAuxBS = 0;
        ch->vAuxCS = 0;
        ctrl |= 0x80000000;
        break;
    }

    enabled = OSDisableInterrupts();

    vpb->pb.ve.currentVolume = ch->vMain;
    vpb->pb.ve.currentDelta = 0;

    vpb->pb.mix.vL = ch->vL;
    if (ch->vL != 0) {
        ctrl |= AX_MIXER_CTRL_L;
    }
    vpb->pb.mix.vDeltaL = 0;

    vpb->pb.mix.vR = ch->vR;
    if (ch->vR != 0) {
        ctrl |= AX_MIXER_CTRL_R;
    }
    vpb->pb.mix.vDeltaR = 0;

    vpb->pb.mix.vAuxAL = ch->vAuxAL;
    if (ch->vAuxAL != 0) {
        ctrl |= AX_MIXER_CTRL_AL;
    }
    vpb->pb.mix.vDeltaAuxAL = 0;

    vpb->pb.mix.vAuxAR = ch->vAuxAR;
    if (ch->vAuxAR != 0) {
        ctrl |= AX_MIXER_CTRL_AR;
    }
    vpb->pb.mix.vDeltaAuxAR = 0;

    vpb->pb.mix.vAuxBL = ch->vAuxBL;
    if (ch->vAuxBL != 0) {
        ctrl |= AX_MIXER_CTRL_BL;
    }
    vpb->pb.mix.vDeltaAuxBL = 0;

    vpb->pb.mix.vAuxBR = ch->vAuxBR;
    if (ch->vAuxBR != 0) {
        ctrl |= AX_MIXER_CTRL_BR;
    }
    vpb->pb.mix.vDeltaAuxBR = 0;

    vpb->pb.mix.vAuxCL = ch->vAuxCL;
    if (ch->vAuxCL != 0) {
        ctrl |= AX_MIXER_CTRL_CL;
    }
    vpb->pb.mix.vDeltaAuxCL = 0;

    vpb->pb.mix.vAuxCR = ch->vAuxCR;
    if (ch->vAuxCR != 0) {
        ctrl |= AX_MIXER_CTRL_CR;
    }
    vpb->pb.mix.vDeltaAuxCR = 0;

    vpb->pb.mix.vS = ch->vS;
    if (ch->vS != 0) {
        ctrl |= AX_MIXER_CTRL_S;
    }
    vpb->pb.mix.vDeltaS = 0;

    vpb->pb.mix.vAuxAS = ch->vAuxAS;
    if (ch->vAuxAS != 0) {
        ctrl |= AX_MIXER_CTRL_AS;
    }
    vpb->pb.mix.vDeltaAuxAS = 0;

    vpb->pb.mix.vAuxBS = ch->vAuxBS;
    if (ch->vAuxBS != 0) {
        ctrl |= AX_MIXER_CTRL_BS;
    }
    vpb->pb.mix.vDeltaAuxBS = 0;

    vpb->pb.mix.vAuxCS = ch->vAuxCS;
    if (ch->vAuxCS != 0) {
        ctrl |= AX_MIXER_CTRL_CS;
    }
    vpb->pb.mix.vDeltaAuxCS = 0;

    vpb->pb.mixerCtrl = ctrl;
    vpb->sync |= AX_PBSYNC_MIXER_CTRL | AX_PBSYNC_MIX | AX_PBSYNC_VE;

    memset(&vpb->pb.rmtMix, 0, sizeof(vpb->pb.rmtMix));
    vpb->pb.rmtMixerCtrl = 0;
    vpb->sync |= AX_PBSYNC_RMT_MIXER_CTRL | AX_PBSYNC_RMTMIX;

    OSRestoreInterrupts(enabled);
}

void HBMMIXReleaseChannel(AXVPB* vpb) {
    s32 index = sId2IndexTable[vpb->index];
    __HBMMIXChannel[index].vpb = NULL;
}

void HBMMIXSetInput(AXVPB* vpb, s32 input) {
    s32 index = sId2IndexTable[vpb->index];
    HBMMIXChannel* ch = &__HBMMIXChannel[index];

    ch->input = input;
    ch->flags |= HBMMIX_FLAG_INPUT;
}

void HBMMIXSetAuxA(AXVPB* vpb, s32 auxA) {
    s32 index = sId2IndexTable[vpb->index];
    HBMMIXChannel* ch = &__HBMMIXChannel[index];

    ch->auxA = auxA;
    ch->flags |= HBMMIX_FLAG_MIX;
}

void HBMMIXSetPan(AXVPB* vpb, s32 pan) {
    s32 index = sId2IndexTable[vpb->index];
    HBMMIXChannel* ch = &__HBMMIXChannel[index];

    ch->pan = pan < 0 ? 0 : (pan > 127 ? 127 : pan);
    __HBMMIXSetPan(ch);
    ch->flags |= HBMMIX_FLAG_MIX;
}

void HBMMIXSetFader(AXVPB* vpb, s32 fader) {
    s32 index = sId2IndexTable[vpb->index];
    HBMMIXChannel* ch = &__HBMMIXChannel[index];

    ch->fader = fader;
    ch->flags |= HBMMIX_FLAG_MIX;
}

void HBMMIXUpdateSettings(void) {
    s32 i;

    if (!__init) {
        return;
    }

    for (i = 0; i < HBMMIX_NUM_CHANNELS; i++) {
        HBMMIXChannel* ch = &__HBMMIXChannel[i];
        AXVPB* vpb = ch->vpb;
        u32 ctrl = 0;
        s32 veChanged = 0;
        s32 mixChanged = 0;

        if (vpb == NULL) {
            continue;
        }

        if (ch->flags & HBMMIX_FLAG_MAIN) {
            ch->vMain = ch->vMainTarget;
            veChanged = 1;
            ch->flags &= ~HBMMIX_FLAG_MAIN;
        }

        if (ch->flags & HBMMIX_FLAG_INPUT) {
            ch->vMainTarget = __HBMMIXGetVolume(ch->input);
            veChanged = 1;
            ch->flags &= ~HBMMIX_FLAG_INPUT;
            ch->flags |= HBMMIX_FLAG_MAIN;
        }

        if (ch->flags & HBMMIX_FLAG_APPLY) {
            ch->vL = ch->vLTarget;
            ch->vR = ch->vRTarget;
            ch->vS = ch->vSTarget;
            ch->vAuxAL = ch->vAuxALTarget;
            ch->vAuxAR = ch->vAuxARTarget;
            ch->vAuxAS = ch->vAuxASTarget;
            ch->vAuxBL = ch->vAuxBLTarget;
            ch->vAuxBR = ch->vAuxBRTarget;
            ch->vAuxBS = ch->vAuxBSTarget;
            ch->vAuxCL = ch->vAuxCLTarget;
            ch->vAuxCR = ch->vAuxCRTarget;
            ch->vAuxCS = ch->vAuxCSTarget;
            mixChanged = 1;
            ch->flags &= ~HBMMIX_FLAG_APPLY;
        }

        if (ch->flags & HBMMIX_FLAG_MIX) {
            switch (__HBMMIXSoundMode) {
            case HBMMIX_MODE_MONO:
                ch->vLTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontL);
                ch->vRTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontL);
                ch->vSTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontR - 30);
                ch->vAuxALTarget = __HBMMIXGetVolume(ch->auxA + ch->fader + ch->panFrontL);
                ch->vAuxARTarget = __HBMMIXGetVolume(ch->auxA + ch->fader + ch->panFrontL);
                ch->vAuxASTarget = __HBMMIXGetVolume(ch->fader + ch->auxA + ch->panFrontR - 30);
                ch->vAuxBLTarget = 0;
                ch->vAuxBRTarget = 0;
                ch->vAuxBSTarget = 0;
                ch->vAuxCLTarget = 0;
                ch->vAuxCRTarget = 0;
                ch->vAuxCSTarget = 0;
                break;
            case HBMMIX_MODE_STEREO:
                ch->vLTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontL + ch->panL);
                ch->vRTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontL + ch->panR);
                ch->vSTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontR - 30);
                ch->vAuxALTarget = __HBMMIXGetVolume(ch->panFrontL + ch->panL + ch->fader + ch->auxA);
                ch->vAuxARTarget = __HBMMIXGetVolume(ch->panFrontL + ch->panR + ch->fader + ch->auxA);
                ch->vAuxASTarget = __HBMMIXGetVolume(ch->fader + ch->auxA + ch->panFrontR - 30);
                ch->vAuxBLTarget = 0;
                ch->vAuxBRTarget = 0;
                ch->vAuxBSTarget = 0;
                ch->vAuxCLTarget = 0;
                ch->vAuxCRTarget = 0;
                ch->vAuxCSTarget = 0;
                break;
            case HBMMIX_MODE_DPL2:
                ch->vLTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontL + ch->panL);
                ch->vRTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontL + ch->panR);
                ch->vSTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontR + ch->panRearL);
                ch->vAuxCLTarget = __HBMMIXGetVolume(ch->fader + ch->panFrontR + ch->panRearR);
                ch->vAuxALTarget = __HBMMIXGetVolume(ch->panFrontL + ch->panL + ch->fader + ch->auxA);
                ch->vAuxARTarget = __HBMMIXGetVolume(ch->panFrontL + ch->panR + ch->fader + ch->auxA);
                ch->vAuxASTarget = __HBMMIXGetVolume(ch->panFrontR + ch->panRearL + ch->fader + ch->auxA);
                ch->vAuxCRTarget = __HBMMIXGetVolume(ch->panFrontR + ch->panRearR + ch->fader + ch->auxA);
                ctrl |= 0x80000000;
                ch->vAuxBLTarget = 0;
                ch->vAuxBRTarget = 0;
                ch->vAuxBSTarget = 0;
                ch->vAuxCSTarget = 0;
                break;
            }
            mixChanged = 1;
            ch->flags &= ~HBMMIX_FLAG_MIX;
            ch->flags |= HBMMIX_FLAG_APPLY;
        }

        if (veChanged) {
            vpb->pb.ve.currentVolume = ch->vMain;
            vpb->pb.ve.currentDelta =
                (s16)((ch->vMainTarget - ch->vMain) / HBMMIX_DELTA_UNIT);
            vpb->sync |= AX_PBSYNC_VE;
        }

        if (mixChanged) {
            vpb->pb.mix.vL = ch->vL;
            if (ch->vL != 0) {
                ctrl |= AX_MIXER_CTRL_L;
            }
            vpb->pb.mix.vDeltaL = (s16)((ch->vLTarget - ch->vL) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaL != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA;
            }

            vpb->pb.mix.vR = ch->vR;
            if (ch->vR != 0) {
                ctrl |= AX_MIXER_CTRL_R;
            }
            vpb->pb.mix.vDeltaR = (s16)((ch->vRTarget - ch->vR) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaR != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA;
            }

            vpb->pb.mix.vAuxAL = ch->vAuxAL;
            if (ch->vAuxAL != 0) {
                ctrl |= AX_MIXER_CTRL_AL;
            }
            vpb->pb.mix.vDeltaAuxAL = (s16)((ch->vAuxALTarget - ch->vAuxAL) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxAL != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_A;
            }

            vpb->pb.mix.vAuxAR = ch->vAuxAR;
            if (ch->vAuxAR != 0) {
                ctrl |= AX_MIXER_CTRL_AR;
            }
            vpb->pb.mix.vDeltaAuxAR = (s16)((ch->vAuxARTarget - ch->vAuxAR) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxAR != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_A;
            }

            vpb->pb.mix.vAuxBL = ch->vAuxBL;
            if (ch->vAuxBL != 0) {
                ctrl |= AX_MIXER_CTRL_BL;
            }
            vpb->pb.mix.vDeltaAuxBL = (s16)((ch->vAuxBLTarget - ch->vAuxBL) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxBL != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_B;
            }

            vpb->pb.mix.vAuxBR = ch->vAuxBR;
            if (ch->vAuxBR != 0) {
                ctrl |= AX_MIXER_CTRL_BR;
            }
            vpb->pb.mix.vDeltaAuxBR = (s16)((ch->vAuxBRTarget - ch->vAuxBR) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxBR != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_B;
            }

            vpb->pb.mix.vAuxCL = ch->vAuxCL;
            if (ch->vAuxCL != 0) {
                ctrl |= AX_MIXER_CTRL_CL;
            }
            vpb->pb.mix.vDeltaAuxCL = (s16)((ch->vAuxCLTarget - ch->vAuxCL) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxCL != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_C;
            }

            vpb->pb.mix.vAuxCR = ch->vAuxCR;
            if (ch->vAuxCR != 0) {
                ctrl |= AX_MIXER_CTRL_CR;
            }
            vpb->pb.mix.vDeltaAuxCR = (s16)((ch->vAuxCRTarget - ch->vAuxCR) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxCR != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_C;
            }

            vpb->pb.mix.vS = ch->vS;
            if (ch->vS != 0) {
                ctrl |= AX_MIXER_CTRL_S;
            }
            vpb->pb.mix.vDeltaS = (s16)((ch->vSTarget - ch->vS) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaS != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_S;
            }

            vpb->pb.mix.vAuxAS = ch->vAuxAS;
            if (ch->vAuxAS != 0) {
                ctrl |= AX_MIXER_CTRL_AS;
            }
            vpb->pb.mix.vDeltaAuxAS = (s16)((ch->vAuxASTarget - ch->vAuxAS) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxAS != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_AS;
            }

            vpb->pb.mix.vAuxBS = ch->vAuxBS;
            if (ch->vAuxBS != 0) {
                ctrl |= AX_MIXER_CTRL_BS;
            }
            vpb->pb.mix.vDeltaAuxBS = (s16)((ch->vAuxBSTarget - ch->vAuxBS) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxBS != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_BS;
            }

            vpb->pb.mix.vAuxCS = ch->vAuxCS;
            if (ch->vAuxCS != 0) {
                ctrl |= AX_MIXER_CTRL_CS;
            }
            vpb->pb.mix.vDeltaAuxCS = (s16)((ch->vAuxCSTarget - ch->vAuxCS) / HBMMIX_DELTA_UNIT);
            if (vpb->pb.mix.vDeltaAuxCS != 0) {
                ctrl |= AX_MIXER_CTRL_DELTA_CS;
            }

            vpb->pb.mixerCtrl = ctrl;
            vpb->sync |= AX_PBSYNC_MIXER_CTRL | AX_PBSYNC_MIX;
        }
    }
}

s32 HBMAllocIndex(u32 key) {
    s32 i;

    for (i = 0; i < HBMMIX_NUM_CHANNELS; i++) {
        if (sIndex2IdTable[i] < 0) {
            sIndex2IdTable[i] = (s8)key;
            sId2IndexTable[key] = (s8)i;
            return i;
        }
    }

    return -1;
}

s32 HBMGetIndex(s32 key) {
    return sId2IndexTable[key];
}

void HBMFreeIndex(s32 index) {
    s32 key = sIndex2IdTable[index];

    sIndex2IdTable[index] = -1;
    sId2IndexTable[key] = -1;
}

void HBMFreeIndexByKey(s32 key) {
    s32 index = sId2IndexTable[key];

    if (index < 0) {
        return;
    }

    sId2IndexTable[key] = -1;
    sIndex2IdTable[index] = -1;
}

} // extern "C"
