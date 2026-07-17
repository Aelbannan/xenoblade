/* "libs/RVL_SDK/src/revolution/axfx/AXFXSrcCoef.c" line 0 "revolution/AXFX.h" */
#ifndef RVL_SDK_PUBLIC_AXFX_H
#define RVL_SDK_PUBLIC_AXFX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/AXFX.h" line 6 "revolution/AXFX/AXFXChorus.h" */
#ifndef RVL_SDK_AXFX_CHORUS_H
#define RVL_SDK_AXFX_CHORUS_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXChorus.h" line 2 "types.h" */
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

/* "libs/RVL_SDK/include/revolution/AXFX/AXFXChorus.h" line 4 "revolution/AXFX/AXFXChorusExp.h" */
#ifndef RVL_SDK_AXFX_CHORUS_EXP_H
#define RVL_SDK_AXFX_CHORUS_EXP_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXChorusExp.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUS;
typedef struct AXFX_BUFFERUPDATE;

typedef struct AXFX_CHORUS_EXP_DELAY {
    f32* line[3]; // at 0x0
    u32 inPos;    // at 0xC
    u32 outPos;   // at 0x10
    u32 lastPos;  // at 0x14
    u32 sizeFP;   // at 0x18
    u32 size;     // at 0x1C
} AXFX_CHORUS_EXP_DELAY;

typedef struct AXFX_CHORUS_EXP_LFO {
    s32* table;     // at 0x0
    s32 phaseAdd;   // at 0x4
    s32 stepSamp;   // at 0x8
    s32 depthSamp;  // at 0xC
    u32 phase;      // at 0x10
    u32 sign;       // at 0x14
    u32 lastNum;    // at 0x18
    s32 lastValue;  // at 0x1C
    s32 grad;       // at 0x20
    s32 gradFactor; // at 0x24
} AXFX_CHORUS_EXP_LFO;

typedef struct AXFX_CHORUS_EXP {
    AXFX_CHORUS_EXP_DELAY delay; // at 0x0
    AXFX_CHORUS_EXP_LFO lfo;     // at 0x20
    f32 history[3][4];           // at 0x48
    u32 histIndex;               // at 0x78
    u32 active;                  // at 0x7C
    f32 delayTime;               // at 0x80
    f32 depth;                   // at 0x84
    f32 rate;                    // at 0x88
    f32 feedback;                // at 0x8C
    struct AXFX_BUS* busIn;             // at 0x90
    struct AXFX_BUS* busOut;            // at 0x94
    f32 outGain;                 // at 0x98
    f32 sendGain;                // at 0x9C
} AXFX_CHORUS_EXP;

u32 AXFXChorusExpGetMemSize(const AXFX_CHORUS_EXP* fx);
BOOL AXFXChorusExpInit(AXFX_CHORUS_EXP* fx);
void AXFXChorusExpShutdown(AXFX_CHORUS_EXP* fx);
BOOL AXFXChorusExpSettings(AXFX_CHORUS_EXP* fx);
void AXFXChorusExpCallback(struct AXFX_BUFFERUPDATE* update, AXFX_CHORUS_EXP* fx);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXChorusExp.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_CHORUS {
    AXFX_CHORUS_EXP exp; // at 0x0
    u32 baseDelay;       // at 0xA0
    u32 variation;       // at 0xA4
    u32 period;          // at 0xA8
} AXFX_CHORUS;

u32 AXFXChorusGetMemSize(const AXFX_CHORUS* fx);
BOOL AXFXChorusInit(AXFX_CHORUS* fx);
BOOL AXFXChorusShutdown(AXFX_CHORUS* fx);
BOOL AXFXChorusSettings(AXFX_CHORUS* fx);
void AXFXChorusCallback(void* chans, void* context);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXChorus.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 7 "revolution/AXFX/AXFXChorusExp.h" */
/* end "revolution/AXFX/AXFXChorusExp.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 8 "revolution/AXFX/AXFXCommon.h" */
#ifndef RVL_SDK_AXFX_COMMON_H
#define RVL_SDK_AXFX_COMMON_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXCommon.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_BUS {
    s32* left;     // at 0x0
    s32* right;    // at 0x4
    s32* surround; // at 0x8
} AXFX_BUS;

typedef struct AXFX_BUFFERUPDATE {
    s32* left;     // at 0x0
    s32* right;    // at 0x4
    s32* surround; // at 0x8
} AXFX_BUFFERUPDATE;

typedef struct AXFX_BUFFERUPDATE_DPL2 {
    s32* left;           // at 0x0
    s32* right;          // at 0x4
    s32* left_surround;  // at 0x8
    s32* right_surround; // at 0xC
} AXFX_BUFFERUPDATE_DPL2;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXCommon.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 9 "revolution/AXFX/AXFXDelay.h" */
#ifndef RVL_SDK_AXFX_DELAY_H
#define RVL_SDK_AXFX_DELAY_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXDelay.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_DELAY {
    s32* line[3];        // at 0x0
    u32 curPos[3];       // at 0xC
    u32 length[3];       // at 0x18
    s32 feedbackGain[3]; // at 0x24
    s32 outGain[3];      // at 0x30
    u32 active;          // at 0x3C
    u32 delay[3];        // at 0x40
    u32 feedback[3];     // at 0x4C
    u32 output[3];       // at 0x58
} AXFX_DELAY;

u32 AXFXDelayGetMemSize(const AXFX_DELAY* fx);
BOOL AXFXDelayInit(AXFX_DELAY* fx);
BOOL AXFXDelaySettings(AXFX_DELAY* fx);
void AXFXDelayShutdown(AXFX_DELAY* fx);
void AXFXDelayCallback(void* chans, void* context);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXDelay.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 10 "revolution/AXFX/AXFXHooks.h" */
#ifndef RVL_SDK_AXFX_HOOKS_H
#define RVL_SDK_AXFX_HOOKS_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXHooks.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*AXFXAllocHook)(size_t size);
typedef void (*AXFXFreeHook)(void* block);

extern AXFXAllocHook __AXFXAlloc;
extern AXFXFreeHook __AXFXFree;

void AXFXSetHooks(AXFXAllocHook alloc, AXFXFreeHook free);
void AXFXGetHooks(AXFXAllocHook* alloc, AXFXFreeHook* free);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXHooks.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 11 "revolution/AXFX/AXFXLfoTable.h" */
#ifndef RVL_SDK_AXFX_LFO_TABLE_H
#define RVL_SDK_AXFX_LFO_TABLE_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXLfoTable.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

s32* __AXFXGetLfoSinTable(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXLfoTable.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 12 "revolution/AXFX/AXFXReverbHi.h" */
#ifndef RVL_SDK_AXFX_REVERB_HI_H
#define RVL_SDK_AXFX_REVERB_HI_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXReverbHi.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/AXFX/AXFXReverbHi.h" line 4 "revolution/AXFX/AXFXReverbHiExp.h" */
#ifndef RVL_SDK_AXFX_REVERB_HI_EXP_H
#define RVL_SDK_AXFX_REVERB_HI_EXP_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXReverbHiExp.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUS;
typedef struct AXFX_BUFFERUPDATE;

typedef struct AXFX_REVERBHI_EXP {
    f32* earlyLine[3];  // at 0x0
    u32 earlyPos[3];    // at 0xC
    u32 earlyLength;    // at 0x18
    u32 earlyMaxLength; // at 0x1C
    f32 earlyCoef[3];   // at 0x20

    f32* preDelayLine[3];  // at 0x2C
    u32 preDelayPos;       // at 0x38
    u32 preDelayLength;    // at 0x3C
    u32 preDelayMaxLength; // at 0x40

    f32* combLine[3][3];  // at 0x44
    u32 combPos[3];       // at 0x68
    u32 combLength[3];    // at 0x74
    u32 combMaxLength[3]; // at 0x80
    f32 combCoef[3];      // at 0x8C

    f32* allpassLine[3][2];  // at 0x98
    u32 allpassPos[2];       // at 0xB0
    u32 allpassLength[2];    // at 0xB8
    u32 allpassMaxLength[2]; // at 0xC0

    f32* lastAllpassLine[3];     // at 0xC8
    u32 lastAllpassPos[3];       // at 0xD4
    u32 lastAllpassLength[3];    // at 0xE0
    u32 lastAllpassMaxLength[3]; // at 0xEC

    f32 allpassCoef;     // at 0xF8
    f32 lastLpfOut[3];   // at 0xFC
    f32 lpfCoef;         // at 0x108
    u32 active;          // at 0x10C
    u32 earlyMode;       // at 0x110
    f32 preDelayTimeMax; // at 0x114
    f32 preDelayTime;    // at 0x118
    u32 fusedMode;       // at 0x11C
    f32 fusedTime;       // at 0x120
    f32 coloration;      // at 0x124
    f32 damping;         // at 0x128
    f32 crosstalk;       // at 0x12C
    f32 earlyGain;       // at 0x130
    f32 fusedGain;       // at 0x134
    struct AXFX_BUS* busIn;     // at 0x138
    struct AXFX_BUS* busOut;    // at 0x13C
    f32 outGain;         // at 0x140
    f32 sendGain;        // at 0x144
} AXFX_REVERBHI_EXP;

u32 AXFXReverbHiExpGetMemSize(const AXFX_REVERBHI_EXP* fx);
BOOL AXFXReverbHiExpInit(AXFX_REVERBHI_EXP* fx);
void AXFXReverbHiExpShutdown(AXFX_REVERBHI_EXP* fx);
BOOL AXFXReverbHiExpSettings(AXFX_REVERBHI_EXP* fx);
void AXFXReverbHiExpCallback(AXFX_BUFFERUPDATE* update, AXFX_REVERBHI_EXP* fx);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXReverbHiExp.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_REVERBHI {
    AXFX_REVERBHI_EXP exp; // at 0x0
    f32 coloration;        // at 0x148
    f32 mix;               // at 0x14C
    f32 time;              // at 0x150
    f32 damping;           // at 0x154
    f32 preDelay;          // at 0x158
    f32 crosstalk;         // at 0x15C
} AXFX_REVERBHI;

u32 AXFXReverbHiGetMemSize(AXFX_REVERBHI* fx);
BOOL AXFXReverbHiInit(AXFX_REVERBHI* fx);
BOOL AXFXReverbHiShutdown(AXFX_REVERBHI* fx);
BOOL AXFXReverbHiSettings(AXFX_REVERBHI* fx);
void AXFXReverbHiCallback(void* chans, void* context);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXReverbHi.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 13 "revolution/AXFX/AXFXReverbHiDpl2.h" */
#ifndef RVL_SDK_AXFX_REVERB_HI_DPL2_H
#define RVL_SDK_AXFX_REVERB_HI_DPL2_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXReverbHiDpl2.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/AXFX/AXFXReverbHiDpl2.h" line 4 "revolution/AXFX/AXFXReverbHiExpDpl2.h" */
#ifndef RVL_SDK_AXFX_REVERB_HI_EXP_DPL2_H
#define RVL_SDK_AXFX_REVERB_HI_EXP_DPL2_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXReverbHiExpDpl2.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUS;
typedef struct AXFX_BUFFERUPDATE;

typedef struct AXFX_REVERBHI_EXP_DPL2 {
    f32* earlyLine[4];  // at 0x0
    u32 earlyPos[3];    // at 0x10
    u32 earlyLength;    // at 0x1C
    u32 earlyMaxLength; // at 0x20
    f32 earlyCoef[3];   // at 0x24

    f32* preDelayLine[4];  // at 0x30
    u32 preDelayPos;       // at 0x40
    u32 preDelayLength;    // at 0x44
    u32 preDelayMaxLength; // at 0x48

    f32* combLine[4][3];  // at 0x4C
    u32 combPos[3];       // at 0x7C
    u32 combLength[3];    // at 0x88
    u32 combMaxLength[3]; // at 0x94
    f32 combCoef[3];      // at 0xA0

    f32* allpassLine[4][2];  // at 0xAC
    u32 allpassPos[2];       // at 0xCC
    u32 allpassLength[2];    // at 0xD4
    u32 allpassMaxLength[2]; // at 0xDC

    f32* lastAllpassLine[4];     // at 0xE4
    u32 lastAllpassPos[4];       // at 0xF4
    u32 lastAllpassLength[4];    // at 0x104
    u32 lastAllpassMaxLength[4]; // at 0x114

    f32 allpassCoef;     // at 0x124
    f32 lastLpfOut[4];   // at 0x128
    f32 lpfCoef;         // at 0x138
    u32 active;          // at 0x13C
    u32 earlyMode;       // at 0x140
    f32 preDelayTimeMax; // at 0x144
    f32 preDelayTime;    // at 0x148
    u32 fusedMode;       // at 0x14C
    f32 fusedTime;       // at 0x150
    f32 coloration;      // at 0x154
    f32 damping;         // at 0x158
    f32 crosstalk;       // at 0x15C
    f32 earlyGain;       // at 0x160
    f32 fusedGain;       // at 0x164
    struct AXFX_BUS* busIn;     // at 0x168
    struct AXFX_BUS* busOut;    // at 0x16C
    f32 outGain;         // at 0x170
    f32 sendGain;        // at 0x174
} AXFX_REVERBHI_EXP_DPL2;

u32 AXFXReverbHiExpGetMemSizeDpl2(const AXFX_REVERBHI_EXP_DPL2* fx);
BOOL AXFXReverbHiExpInitDpl2(AXFX_REVERBHI_EXP_DPL2* fx);
void AXFXReverbHiExpShutdownDpl2(AXFX_REVERBHI_EXP_DPL2* fx);
BOOL AXFXReverbHiExpSettingsDpl2(AXFX_REVERBHI_EXP_DPL2* fx);
void AXFXReverbHiExpCallbackDpl2(AXFX_BUFFERUPDATE_DPL2* update,
                                 AXFX_REVERBHI_EXP_DPL2* fx);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXReverbHiExpDpl2.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AXFX_REVERBHI_DPL2 {
    AXFX_REVERBHI_EXP_DPL2 exp; // at 0x0
    f32 coloration;             // at 0x148
    f32 mix;                    // at 0x14C
    f32 time;                   // at 0x150
    f32 damping;                // at 0x154
    f32 preDelay;               // at 0x158
    f32 crosstalk;              // at 0x15C
} AXFX_REVERBHI_DPL2;

u32 AXFXReverbHiGetMemSizeDpl2(AXFX_REVERBHI_DPL2* fx);
BOOL AXFXReverbHiInitDpl2(AXFX_REVERBHI_DPL2* fx);
BOOL AXFXReverbHiShutdownDpl2(AXFX_REVERBHI_DPL2* fx);
BOOL AXFXReverbHiSettingsDpl2(AXFX_REVERBHI_DPL2* fx);
void AXFXReverbHiCallbackDpl2(void* chans, void* context);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXReverbHiDpl2.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 14 "revolution/AXFX/AXFXReverbHiExp.h" */
/* end "revolution/AXFX/AXFXReverbHiExp.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 15 "revolution/AXFX/AXFXReverbHiExpDpl2.h" */
/* end "revolution/AXFX/AXFXReverbHiExpDpl2.h" */
/* "libs/RVL_SDK/include/revolution/AXFX.h" line 16 "revolution/AXFX/AXFXSrcCoef.h" */
#ifndef RVL_SDK_AXFX_SRC_COEF_H
#define RVL_SDK_AXFX_SRC_COEF_H
/* "libs/RVL_SDK/include/revolution/AXFX/AXFXSrcCoef.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SrcCoef{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} SrcCoef;

SrcCoef* __AXFXGetSrcCoef(u32 i);


#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX/AXFXSrcCoef.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/AXFX.h" */

static SrcCoef __SrcTab12khz[128] = {
    {0.09750366211f, 0.80221557617f, 0.10159301758f, -0.00097656250f},
    {0.09350585938f, 0.80203247070f, 0.10580444336f, -0.00103759766f},
    {0.08959960938f, 0.80169677734f, 0.11010742188f, -0.00115966797f},
    {0.08578491211f, 0.80117797852f, 0.11447143555f, -0.00128173828f},
    {0.08203125000f, 0.80047607422f, 0.11892700195f, -0.00140380859f},
    {0.07836914062f, 0.79962158203f, 0.12347412109f, -0.00152587891f},
    {0.07479858398f, 0.79861450195f, 0.12814331055f, -0.00164794922f},
    {0.07135009766f, 0.79742431641f, 0.13287353516f, -0.00177001953f},
    {0.06796264648f, 0.79605102539f, 0.13769531250f, -0.00192260742f},
    {0.06469726562f, 0.79452514648f, 0.14260864258f, -0.00204467773f},
    {0.06149291992f, 0.79284667969f, 0.14761352539f, -0.00219726562f},
    {0.05834960938f, 0.79098510742f, 0.15270996094f, -0.00231933594f},
    {0.05532836914f, 0.78894042969f, 0.15789794922f, -0.00247192383f},
    {0.05236816406f, 0.78674316406f, 0.16317749023f, -0.00265502930f},
    {0.04949951172f, 0.78442382812f, 0.16851806641f, -0.00280761719f},
    {0.04672241211f, 0.78189086914f, 0.17398071289f, -0.00299072266f},
    {0.04400634766f, 0.77920532227f, 0.17950439453f, -0.00314331055f},
    {0.04141235352f, 0.77636718750f, 0.18511962891f, -0.00332641602f},
    {0.03887939453f, 0.77337646484f, 0.19082641602f, -0.00350952148f},
    {0.03640747070f, 0.77023315430f, 0.19659423828f, -0.00369262695f},
    {0.03402709961f, 0.76693725586f, 0.20248413086f, -0.00387573242f},
    {0.03173828125f, 0.76348876953f, 0.20843505859f, -0.00405883789f},
    {0.02951049805f, 0.75985717773f, 0.21444702148f, -0.00427246094f},
    {0.02737426758f, 0.75610351562f, 0.22055053711f, -0.00445556641f},
    {0.02529907227f, 0.75219726562f, 0.22674560547f, -0.00466918945f},
    {0.02331542969f, 0.74816894531f, 0.23300170898f, -0.00485229492f},
    {0.02139282227f, 0.74398803711f, 0.23931884766f, -0.00506591797f},
    {0.01956176758f, 0.73965454102f, 0.24572753906f, -0.00531005859f},
    {0.01779174805f, 0.73519897461f, 0.25219726562f, -0.00552368164f},
    {0.01605224609f, 0.73059082031f, 0.25872802734f, -0.00570678711f},
    {0.01440429688f, 0.72586059570f, 0.26535034180f, -0.00592041016f},
    {0.01281738281f, 0.72100830078f, 0.27203369141f, -0.00616455078f},
    {0.01132202148f, 0.71600341797f, 0.27877807617f, -0.00637817383f},
    {0.00988769531f, 0.71090698242f, 0.28558349609f, -0.00656127930f},
    {0.00851440430f, 0.70565795898f, 0.29244995117f, -0.00677490234f},
    {0.00720214844f, 0.70031738281f, 0.29934692383f, -0.00701904297f},
    {0.00592041016f, 0.69485473633f, 0.30633544922f, -0.00723266602f},
    {0.00469970703f, 0.68927001953f, 0.31338500977f, -0.00741577148f},
    {0.00357055664f, 0.68356323242f, 0.32046508789f, -0.00762939453f},
    {0.00247192383f, 0.67773437500f, 0.32760620117f, -0.00787353516f},
    {0.00143432617f, 0.67184448242f, 0.33477783203f, -0.00808715820f},
    {0.00045776367f, 0.66583251953f, 0.34197998047f, -0.00827026367f},
    {-0.00048828125f, 0.65972900391f, 0.34924316406f, -0.00845336914f},
    {-0.00134277344f, 0.65353393555f, 0.35656738281f, -0.00863647461f},
    {-0.00216674805f, 0.64721679688f, 0.36389160156f, -0.00885009766f},
    {-0.00296020508f, 0.64083862305f, 0.37127685547f, -0.00903320312f},
    {-0.00369262695f, 0.63433837891f, 0.37869262695f, -0.00921630859f},
    {-0.00436401367f, 0.62777709961f, 0.38613891602f, -0.00933837891f},
    {-0.00497436523f, 0.62115478516f, 0.39361572266f, -0.00949096680f},
    {-0.00558471680f, 0.61444091797f, 0.40109252930f, -0.00964355469f},
    {-0.00613403320f, 0.60763549805f, 0.40859985352f, -0.00979614258f},
    {-0.00665283203f, 0.60076904297f, 0.41610717773f, -0.00991821289f},
    {-0.00714111328f, 0.59384155273f, 0.42364501953f, -0.01000976562f},
    {-0.00756835938f, 0.58685302734f, 0.43121337891f, -0.01013183594f},
    {-0.00796508789f, 0.57977294922f, 0.43875122070f, -0.01022338867f},
    {-0.00833129883f, 0.57266235352f, 0.44631958008f, -0.01028442383f},
    {-0.00866699219f, 0.56552124023f, 0.45388793945f, -0.01034545898f},
    {-0.00897216797f, 0.55831909180f, 0.46145629883f, -0.01040649414f},
    {-0.00921630859f, 0.55105590820f, 0.46902465820f, -0.01040649414f},
    {-0.00946044922f, 0.54373168945f, 0.47659301758f, -0.01040649414f},
    {-0.00967407227f, 0.53640747070f, 0.48413085938f, -0.01037597656f},
    {-0.00985717773f, 0.52902221680f, 0.49166870117f, -0.01037597656f},
    {-0.01000976562f, 0.52160644531f, 0.49917602539f, -0.01031494141f},
    {-0.01013183594f, 0.51416015625f, 0.50668334961f, -0.01025390625f},
    {-0.01025390625f, 0.50668334961f, 0.51416015625f, -0.01013183594f},
    {-0.01031494141f, 0.49917602539f, 0.52160644531f, -0.01000976562f},
    {-0.01037597656f, 0.49166870117f, 0.52902221680f, -0.00985717773f},
    {-0.01037597656f, 0.48413085938f, 0.53640747070f, -0.00967407227f},
    {-0.01040649414f, 0.47659301758f, 0.54373168945f, -0.00946044922f},
    {-0.01040649414f, 0.46902465820f, 0.55105590820f, -0.00921630859f},
    {-0.01040649414f, 0.46145629883f, 0.55831909180f, -0.00897216797f},
    {-0.01034545898f, 0.45388793945f, 0.56552124023f, -0.00866699219f},
    {-0.01028442383f, 0.44631958008f, 0.57266235352f, -0.00833129883f},
    {-0.01022338867f, 0.43875122070f, 0.57977294922f, -0.00796508789f},
    {-0.01013183594f, 0.43121337891f, 0.58685302734f, -0.00756835938f},
    {-0.01000976562f, 0.42364501953f, 0.59384155273f, -0.00714111328f},
    {-0.00991821289f, 0.41610717773f, 0.60076904297f, -0.00665283203f},
    {-0.00979614258f, 0.40859985352f, 0.60763549805f, -0.00613403320f},
    {-0.00964355469f, 0.40109252930f, 0.61444091797f, -0.00558471680f},
    {-0.00949096680f, 0.39361572266f, 0.62115478516f, -0.00497436523f},
    {-0.00933837891f, 0.38613891602f, 0.62777709961f, -0.00436401367f},
    {-0.00921630859f, 0.37869262695f, 0.63433837891f, -0.00369262695f},
    {-0.00903320312f, 0.37127685547f, 0.64083862305f, -0.00296020508f},
    {-0.00885009766f, 0.36389160156f, 0.64721679688f, -0.00216674805f},
    {-0.00863647461f, 0.35656738281f, 0.65353393555f, -0.00134277344f},
    {-0.00845336914f, 0.34924316406f, 0.65972900391f, -0.00048828125f},
    {-0.00827026367f, 0.34197998047f, 0.66583251953f, 0.00045776367f},
    {-0.00808715820f, 0.33477783203f, 0.67184448242f, 0.00143432617f},
    {-0.00787353516f, 0.32760620117f, 0.67773437500f, 0.00247192383f},
    {-0.00762939453f, 0.32046508789f, 0.68356323242f, 0.00357055664f},
    {-0.00741577148f, 0.31338500977f, 0.68927001953f, 0.00469970703f},
    {-0.00723266602f, 0.30633544922f, 0.69485473633f, 0.00592041016f},
    {-0.00701904297f, 0.29934692383f, 0.70031738281f, 0.00720214844f},
    {-0.00677490234f, 0.29244995117f, 0.70565795898f, 0.00851440430f},
    {-0.00656127930f, 0.28558349609f, 0.71090698242f, 0.00988769531f},
    {-0.00637817383f, 0.27877807617f, 0.71600341797f, 0.01132202148f},
    {-0.00616455078f, 0.27203369141f, 0.72100830078f, 0.01281738281f},
    {-0.00592041016f, 0.26535034180f, 0.72586059570f, 0.01440429688f},
    {-0.00570678711f, 0.25872802734f, 0.73059082031f, 0.01605224609f},
    {-0.00552368164f, 0.25219726562f, 0.73519897461f, 0.01779174805f},
    {-0.00531005859f, 0.24572753906f, 0.73965454102f, 0.01956176758f},
    {-0.00506591797f, 0.23931884766f, 0.74398803711f, 0.02139282227f},
    {-0.00485229492f, 0.23300170898f, 0.74816894531f, 0.02331542969f},
    {-0.00466918945f, 0.22674560547f, 0.75219726562f, 0.02529907227f},
    {-0.00445556641f, 0.22055053711f, 0.75610351562f, 0.02737426758f},
    {-0.00427246094f, 0.21444702148f, 0.75985717773f, 0.02951049805f},
    {-0.00405883789f, 0.20843505859f, 0.76348876953f, 0.03173828125f},
    {-0.00387573242f, 0.20248413086f, 0.76693725586f, 0.03402709961f},
    {-0.00369262695f, 0.19659423828f, 0.77023315430f, 0.03640747070f},
    {-0.00350952148f, 0.19082641602f, 0.77337646484f, 0.03887939453f},
    {-0.00332641602f, 0.18511962891f, 0.77636718750f, 0.04141235352f},
    {-0.00314331055f, 0.17950439453f, 0.77920532227f, 0.04400634766f},
    {-0.00299072266f, 0.17398071289f, 0.78189086914f, 0.04672241211f},
    {-0.00280761719f, 0.16851806641f, 0.78442382812f, 0.04949951172f},
    {-0.00265502930f, 0.16317749023f, 0.78674316406f, 0.05236816406f},
    {-0.00247192383f, 0.15789794922f, 0.78894042969f, 0.05532836914f},
    {-0.00231933594f, 0.15270996094f, 0.79098510742f, 0.05834960938f},
    {-0.00219726562f, 0.14761352539f, 0.79284667969f, 0.06149291992f},
    {-0.00204467773f, 0.14260864258f, 0.79452514648f, 0.06469726562f},
    {-0.00192260742f, 0.13769531250f, 0.79605102539f, 0.06796264648f},
    {-0.00177001953f, 0.13287353516f, 0.79742431641f, 0.07135009766f},
    {-0.00164794922f, 0.12814331055f, 0.79861450195f, 0.07479858398f},
    {-0.00152587891f, 0.12347412109f, 0.79962158203f, 0.07836914062f},
    {-0.00140380859f, 0.11892700195f, 0.80047607422f, 0.08203125000f},
    {-0.00128173828f, 0.11447143555f, 0.80117797852f, 0.08578491211f},
    {-0.00115966797f, 0.11010742188f, 0.80169677734f, 0.08959960938f},
    {-0.00103759766f, 0.10580444336f, 0.80203247070f, 0.09350585938f},
    {-0.00097656250f, 0.10159301758f, 0.80221557617f, 0.09750366211f}};

SrcCoef* __AXFXGetSrcCoef(u32 i) {
    return &__SrcTab12khz[i & 0x0FFFFFFF];
}
