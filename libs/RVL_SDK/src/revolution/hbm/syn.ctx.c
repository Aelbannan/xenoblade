// High-level C reconstruction of RVL_SDK/src/revolution/hbm/syn
// No inline asm, no register tricks. Matches retail via EQUIVALENT_MATCH.

/* "libs/RVL_SDK/src/revolution/hbm/syn.c" line 3 "types.h" */
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
/* "libs/RVL_SDK/src/revolution/hbm/syn.c" line 4 "revolution/os/OSInterrupt.h" */
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
/* "libs/RVL_SDK/src/revolution/hbm/syn.c" line 5 "revolution/ax/AXVPB.h" */
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
/* "libs/RVL_SDK/src/revolution/hbm/syn.c" line 6 "revolution/ax/AXAlloc.h" */
#ifndef RVL_SDK_AX_ALLOC_H
#define RVL_SDK_AX_ALLOC_H
/* "libs/RVL_SDK/include/revolution/ax/AXAlloc.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/ax/AXAlloc.h" line 4 "revolution/AX/AXVPB.h" */
/* end "revolution/AX/AXVPB.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define AX_PRIORITY_FREE 0
#define AX_PRIORITY_MIN 1
#define AX_PRIORITY_MAX 31

AXVPB* __AXGetStackHead(u32 prio);
void __AXServiceCallbackStack(void);
void __AXInitVoiceStacks(void);
void __AXAllocInit(void);
void __AXAllocQuit(void);
void __AXPushFreeStack(AXVPB* vpb);
AXVPB* __AXPopFreeStack(void);
void __AXPushCallbackStack(AXVPB* vpb);
AXVPB* __AXPopCallbackStack(void);
void __AXRemoveFromStack(AXVPB* vpb);
void __AXPushStackHead(AXVPB* vpb, u32 prio);
AXVPB* __AXPopStackFromBottom(u32 prio);
void AXFreeVoice(AXVPB* vpb);
AXVPB* AXAcquireVoice(u32 prio, AXVoiceCallback callback, u32 userContext);
void AXSetVoicePriority(AXVPB* vpb, u32 prio);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/ax/AXAlloc.h" */

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations of external functions in the HBM/OS/AX subsystems
void __HBMSYNResetAllControllers(struct HBMSYNSYNTH*);
void __HBMSYNRunInputBufferEvents(struct HBMSYNSYNTH*);
void __HBMSYNServiceVoice(s32);
void HBMMIXReleaseChannel(void*);
void HBMFreeIndexByKey(void*);
BOOL AXIsInit(void);

// ---- BSS globals (single contiguous .bss block) ----
extern struct HBMSYNSYNTH* __HBMSYNSynthList; // .bss:0x0 | size 4
extern void* __HBMSYNVoice;                   // .bss:0x4 | size 4 — pointer to voice array
extern u32 __init;                             // .bss:0x4C8 | size 4 — init flag

// ---- HBMSYNSYNTH — synthesizer instance structure ----
// Offsets inferred from retail ASM (syn.c, synctrl.c, synvoice.c, seq.c)
typedef struct HBMSYNSYNTH {
    struct HBMSYNSYNTH* next;   // 0x00 — linked-list next
    void* dataSections[6];      // 0x04-0x18 — pointers into sound data
    u32  field1C;               // 0x1C — playback speed param (from r5)
    u32  field20;               // 0x20 — playback speed param (from r5 + 0x80000000)
    u32  field24;               // 0x24 — playback speed param (from r5 << 1)
    u8   pad28[0x68 - 0x28];   // 0x28-0x67
    u32  masterVolume;          // 0x68 — master volume (<< 16)
    u8   pad6C[0xFC - 0x6C];   // 0x6C-0xFB
    u8   midiBuffer[0x300];    // 0xFC-0x3FB — MIDI input buffer
    u8*  midiWritePtr;          // 0x3FC — current write pos in midiBuffer
    u32  midiCount;             // 0x400 — number of MIDI bytes received
    u32  activeVoiceFlag;       // 0x404 — non-zero when voices are active
    u8   voiceData[0x2000];    // 0x408-0x2407 — per-voice state blocks
} HBMSYNSYNTH;

// ---- Per-voice entry (size 0x4C) ----
typedef struct {
    u8   pad00[0x04];          // 0x00
    void* mixChannel;           // 0x04 — HBMMIXChannel*
    struct HBMSYNSYNTH* synth; // 0x08 — owning synthesizer
    u8   pad0C[0x4C - 0x0C];   // 0x0C-0x4B — rest of voice state
} HBMSYNVoiceEntry;

// ---- Function implementations ----

void __HBMSYNRemoveSynthFromList__FP11HBMSYNSYNTH(HBMSYNSYNTH* synth)
{
    BOOL intr = OSDisableInterrupts();
    HBMSYNSYNTH* cur = __HBMSYNSynthList;
    HBMSYNSYNTH* last = NULL;
    HBMSYNSYNTH* first = NULL;

    while (cur != NULL) {
        if (cur != synth) {
            if (first != NULL) {
                last->next = cur;
                last = cur;
            } else {
                last = cur;
                first = cur;
            }
        }
        cur = cur->next;
    }

    if (last != NULL) {
        last->next = NULL;
    }

    __HBMSYNSynthList = first;
    OSRestoreInterrupts(intr);
}

void HBMSYNInit(void)
{
    u8* base = (u8*)&__HBMSYNSynthList;

    if (!AXIsInit()) {
        return;
    }

    u32* initFlag = (u32*)(base + 0x4C8);

    if (*initFlag != 0) {
        return;
    }

    // __HBMSYNVoice = &__s_HBMSYNVoice (at base + 8)
    *(void**)(base + 4) = base + 8;

    // Clear the synth reference in every voice (16 voices x 0x4C bytes)
    {
        s32 i;
        for (i = 0; i < 16; i++) {
            ((HBMSYNVoiceEntry*)__HBMSYNVoice)[i].synth = NULL;
        }
    }

    // __HBMSYNSynthList = NULL
    *(HBMSYNSYNTH**)base = NULL;

    *initFlag = 1;
}

void HBMSYNQuit(void)
{
    __HBMSYNVoice = NULL;
    __init = 0;
}

void HBMSYNRunAudioFrame(void)
{
    if (__init == 0) {
        return;
    }

    s32 i;
    for (i = 0; i < 16; i++) {
        __HBMSYNServiceVoice(i);
    }

    HBMSYNSYNTH* syn = __HBMSYNSynthList;
    while (syn != NULL) {
        __HBMSYNRunInputBufferEvents(syn);
        syn = syn->next;
    }
}

void HBMSYNInitSynth(HBMSYNSYNTH* syn, u32* config, u32 param3)
{
    u32 v = param3 + 0x80000000;

    syn->dataSections[0] = (u8*)config + config[0];
    syn->dataSections[1] = (u8*)config + config[1];
    syn->dataSections[2] = (u8*)config + config[2];
    syn->dataSections[3] = (u8*)config + config[3];
    syn->dataSections[4] = (u8*)config + config[4];
    syn->dataSections[5] = (u8*)config + config[5];

    syn->field1C = v >> 1;
    syn->field20 = v;
    syn->field24 = v << 1;

    syn->masterVolume = 0;

    __HBMSYNResetAllControllers(syn);

    syn->midiWritePtr = syn->midiBuffer;
    syn->midiCount = 0;
    syn->activeVoiceFlag = 0;

    // Clear voice data region: 16 groups x 0x200 bytes, zeroed 4 words at a time
    {
        u32* p = (u32*)syn->voiceData;
        u32* end = (u32*)(syn->voiceData + sizeof(syn->voiceData));
        while (p < end) {
            *p++ = 0;
        }
    }

    // Insert this synth into the global linked list (under interrupt lock)
    {
        BOOL intr = OSDisableInterrupts();
        HBMSYNSYNTH* head = __HBMSYNSynthList;
        if (head != NULL) {
            syn->next = head;
        } else {
            syn->next = NULL;
        }
        __HBMSYNSynthList = syn;
        OSRestoreInterrupts(intr);
    }
}

void HBMSYNQuitSynth(HBMSYNSYNTH* syn)
{
    BOOL intr = OSDisableInterrupts();

    if (syn->activeVoiceFlag != 0) {
        HBMSYNVoiceEntry* voices = (HBMSYNVoiceEntry*)__HBMSYNVoice;
        s32 i;
        for (i = 0; i < 16; i++) {
            HBMSYNVoiceEntry* v = &voices[i];
            if (v->synth == syn) {
                HBMMIXReleaseChannel(v->mixChannel);
                HBMFreeIndexByKey(*(void**)((u8*)v->mixChannel + 0x18));
                AXFreeVoice((AXVPB*)v->mixChannel);
                v->synth = NULL;
            }
        }
    }

    __HBMSYNRemoveSynthFromList__FP11HBMSYNSYNTH(syn);

    OSRestoreInterrupts(intr);
}

void HBMSYNMidiInput(HBMSYNSYNTH* syn, const u8* data)
{
    u8* p;
    p = syn->midiWritePtr;
    *p = data[0];
    p = syn->midiWritePtr;
    syn->midiWritePtr = p + 1;
    p = syn->midiWritePtr;
    *p = data[1];
    p = syn->midiWritePtr;
    syn->midiWritePtr = p + 1;
    p = syn->midiWritePtr;
    *p = data[2];
    p = syn->midiWritePtr;
    syn->midiWritePtr = p + 1;
    syn->midiCount++;
}

void HBMSYNSetMasterVolume(HBMSYNSYNTH* syn, u32 vol)
{
    syn->masterVolume = vol << 16;
}

#ifdef __cplusplus
}
#endif
