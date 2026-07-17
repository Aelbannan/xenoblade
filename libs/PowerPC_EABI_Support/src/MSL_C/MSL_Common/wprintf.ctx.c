/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 0 "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_fp.h" */
#ifndef _MSL_C_ANSI_FP_H
#define _MSL_C_ANSI_FP_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_fp.h" line 3 "types.h" */
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
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_fp.h" line 4 "math.h" */
#ifndef MSL_MATH_H
#define MSL_MATH_H

/* "libs/PowerPC_EABI_Support/include/stl/math.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

/* "libs/PowerPC_EABI_Support/include/stl/math.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/math_double.h" */
#ifndef MSL_MATH_DOUBLE_H
#define MSL_MATH_DOUBLE_H
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/math_double.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif


double atan(double x);
double ceil(double x);
double copysign(double x, double y);
double cos(double x);
double floor(double x);
double frexp(double x, int* eptr);
double ldexp(double value, int exp);
double modf(double x, double* iptr);
double sin(double x);
double tan(double x);
double acos(double x);
double asin(double x);
double atan2(double y, double x);
double fmod(double x, double y);
double pow(double x, double y);
double sqrt(double x);
double log(double x);

inline float acosf(float x) {
    return acos(x);
}
inline float ceilf(float x) {
    return ceil(x);
}
inline float cosf(float x) {
    return cos(x);
}
inline float sinf(float x) {
    return sin(x);
}

float sqrtf(float x);

inline float tanf(float x) {
    return tan(x);
}
inline float floorf(float x) {
    return floor(x);
}
inline float fmodf(float x, float y) {
    return fmod(x, y);
}

inline float modff(float x, float* iptr) {
    float frac;
    double intg;

    x = (double)x;
    frac = modf(x, &intg);
    *iptr = intg;

    return frac;
}

float fabsf(float);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/math_double.h" */
/* "libs/PowerPC_EABI_Support/include/stl/math.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h" */
#ifndef _MSL_MATH_API_H
#define _MSL_MATH_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h" line 3 "types.h" */
/* end "types.h" */


#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus


#if defined(i386) || defined(i486) || defined(intel) || defined(x86) || defined(i86pc) || defined(__alpha) || defined(__osf__)
#define __LITTLE_ENDIAN
#endif

#ifdef __LITTLE_ENDIAN
#define __HI(x)  *(1 + (int*)&x)
#define __LO(x)  *(int*)&x
#define __HIp(x) *(1 + (int*)x)
#define __LOp(x) *(int*)x
#else
#define __HI(x)  *(int*)&x
#define __LO(x)  *(1 + (int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1 + (int*)x)
#endif


#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h" */

extern int __float_nan[];
extern int __float_huge[];
extern int __double_huge[];

#define INFINITY (*(float*) __float_huge)
#define NAN (*(float*) __float_nan)
#define HUGE_VAL (*(double*) __double_huge)

#define FP_NAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL

#define M_PI    3.1415927f
#define M_SQRT3 1.73205f

int __signbitf(float);
int __fpclassifyf(float);
int __signbitd(double);
int __fpclassifyd(double);

#define fpclassify(x) ((sizeof(x) == sizeof(float)) ? __fpclassifyf((float)(x)) : __fpclassifyd((double)(x)))
#define signbit(x) ((sizeof(x) == sizeof(float)) ? __signbitf((float)(x)) : __signbitd((double)(x)))

#define isfinite(x) ((fpclassify(x) > FP_INFINITE))
#define isnan(x) (fpclassify(x) == FP_NAN)
#define isnormal(x) (fpclassify(x) == FP_NORMAL)

inline long double fabsl(long double x) {
    return __fabs((double)x);
}

double __ieee754_acos(double);
double __ieee754_fmod(double, double);
double __ieee754_log(double);
double __ieee754_log10(double);
double __ieee754_pow(double, double);
double __ieee754_sqrt(double);
double __ieee754_atan2(double, double);
double __ieee754_asin(double);

double copysign(double, double);
double scalbn(double, int);

double __kernel_sin(double, double, int);
double __kernel_cos(double, double);
double __kernel_tan(double, double, int);

int __ieee754_rem_pio2(double, double *);

double nan(const char* arg);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "math.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_fp.h" line 5 "float.h" */
#ifndef MSL_FLOAT_H
#define MSL_FLOAT_H

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

extern int __double_max[];
extern int __extended_min[];
extern int __extended_max[];
extern int __float_max[];
extern int __float_epsilon[];

#define FLT_MANT_DIG 24
#define FLT_DIG 6
#define FLT_MIN_EXP (-125)
#define FLT_MIN_10_EXP (-37)
#define FLT_MAX_EXP 128
#define FLT_MAX_10_EXP 38

#define FLT_MAX 0x1.fffffeP127F
#define FLT_EPSILON 0x1.000000P-23F
#define FLT_MIN 0x1.000000P-126F


#define DBL_MANT_DIG 53
#define DBL_DIG 15
#define DBL_MIN_EXP (-1021)
#define DBL_MIN_10_EXP (-308)
#define DBL_MAX_EXP 1024
#define DBL_MAX_10_EXP 308

#define DBL_MAX 0x1.fffffffffffffP1023
#define DBL_EPSILON 0x1.0000000000000P-52
#define DBL_MIN 0x1.0000000000000P-1022


#define LDBL_MANT_DIG 53
#define LDBL_DIG 15
#define LDBL_MIN_EXP (-1021)
#define LDBL_MIN_10_EXP (-308)
#define LDBL_MAX_EXP 1024
#define LDBL_MAX_10_EXP 308

#define LDBL_MAX 0x1.fffffffffffffP1023L
#define LDBL_EPSILON 0x1.0000000000000P-52L
#define LDBL_MIN 0x1.0000000000000P-1022L

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "float.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_fp.h" line 6 "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/fdlibm.h" */
#ifndef _FDLIBM_H
#define _FDLIBM_H

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus


//#ifdef __STDC__
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/fdlibm.h" line 9 "math.h" */
/* end "math.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/fdlibm.h" line 10 "errno.h" */
#ifndef MSL_ERRNO_H
#define MSL_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

//Error constants
#define E2BIG 7
#define EACCES 13
#define EAGAIN 11
#define EBADF 9
#define EBUSY 16
#define ECHILD 10
#define EDEADLK 35
#define EDEADLOCK EDEADLK
#define EDOM 33
#define EEXIST 17
#define EFAULT 14
#define EFBIG 27
#define EFPOS 40
#define EILSEQ 88
#define EINTR 4
#define EINVAL 22
#define EIO 5
#define EISDIR 21
#define EMFILE 24
#define EMLINK 31
#define ENFILE 23
#define ENAMETOOLONG 36
#define ENODEV 19
#define ENOENT 2
#define ENOERR 0
#define ENOEXEC 8
#define ENOLCK 77
#define ENOMEM 12
#define ENOSPC 28
#define ENOSYS 38
#define ENOTDIR 20
#define ENOTEMPTY 39
#define ENOTTY 25
#define ENXIO 6
#define EPERM 1
#define EPIPE 32
#define ERANGE 34
#define EROFS 30
#define ESIGPARM 26
#define ESPIPE 29
#define ESRCH 3
#define EUNKNOWN 99
#define EXDEV 18

extern int errno;

#ifdef __cplusplus
}
#endif

#endif
/* end "errno.h" */
//#endif

/**
 * For including FDLIBM declarations without also including its many macros.
 */

double atan(double x);
double ceil(double x);
double copysign(double x, double y);
double cos(double x);
double floor(double x);
double frexp(double x, int* eptr);
double ldexp(double value, int exp);
double modf(double x, double* iptr);
double sin(double x);
double tan(double x);
double acos(double x);
double asin(double x);
double atan2(double y, double x);
double fmod(double x, double y);
double pow(double x, double y);
double sqrt(double x);
double log(double x);
double sqrt(double x);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common_Embedded/Math/fdlibm.h" */

#define SIGDIGLEN 36

typedef struct decimal {
    char sign;
    char unk1;
    s16 exp;
    struct {
        u8 length;
        u8 text[36];
        u8 unk41;
    } sig;
} decimal;

typedef struct decform
{
    char style;
    char unk1;
    short digits;
} decform;

void __ull2dec(decimal*, u64);
void __timesdec(decimal*, const decimal*, const decimal*);
void __str2dec(decimal*, const char*, short);
void __two_exp(decimal*, long);
BOOL __equals_dec(const decimal*, const decimal*);
BOOL __less_dec(const decimal*, const decimal*);
void __minus_dec(decimal*, const decimal*, const decimal*);
void __num2dec_internal(decimal*, double);
void __num2dec(const decform*, double, decimal*);
double __dec2num(const decimal*);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_fp.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 1 "math.h" */
/* end "math.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 2 "locale.h" */
#ifndef MSL_LOCALE_H
#define MSL_LOCALE_H

/* "libs/PowerPC_EABI_Support/include/stl/locale.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/stl/locale.h" line 4 "stdlib.h" */
#ifndef MSL_STDLIB_H
#define MSL_STDLIB_H

/* "libs/PowerPC_EABI_Support/include/stl/stdlib.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

/* "libs/PowerPC_EABI_Support/include/stl/stdlib.h" line 8 "PowerPC_EABI_Support/MSL_C/MSL_Common/alloc.h" */
#ifndef _MSL_ALLOC_H
#define _MSL_ALLOC_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/alloc.h" line 3 "types.h" */
/* end "types.h" */

void free(void*);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/alloc.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdlib.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/arith.h" */
#ifndef _MSL_COMMON_ARITH_H
#define _MSL_COMMON_ARITH_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/arith.h" line 3 "types.h" */
/* end "types.h" */

typedef struct {
    int quot; /* quotient */
    int rem;  /* remainder */
} div_t;


int abs(int __x);
long labs(long __x);
div_t div(s32 __numer, s32 __denom);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/arith.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdlib.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/mbstring.h" */
#ifndef MSL_MBSTRING_H
#define MSL_MBSTRING_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/mbstring.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

int __mbtowc_noconv(wchar_t*, const char*, size_t);
int __wctomb_noconv(char*, wchar_t);
int mbtowc(wchar_t*, const char*, size_t);
size_t mbstowcs(wchar_t*, const char*, size_t);
size_t wcstombs(char*, const wchar_t*, size_t);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/mbstring.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdlib.h" line 11 "PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h" */
#ifndef _MSL_RAND_H
#define _MSL_RAND_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

int rand();
void srand(u32 seed);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdlib.h" line 12 "PowerPC_EABI_Support/MSL_C/MSL_Common/strtold.h" */
#ifndef MSL_STRTOLD_H
#define MSL_STRTOLD_H

long double __strtold(int max_width, int (*ReadProc)(void *, int, int), void* ReadProcArg, int* chars_scanned, int* overflow);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/strtold.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdlib.h" line 13 "PowerPC_EABI_Support/MSL_C/MSL_Common/strtoul.h" */
#ifndef MSL_STRTOUL_H
#define MSL_STRTOUL_H

unsigned long __strtoul(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, int* chars_scanned, int* negative, int* overflow);
unsigned long long __strtoull(int base, int max_width, int (*ReadProc)(void *, int, int), void *ReadProcArg, int* chars_scanned, int* negative, int* overflow);
int atoi(const char *str);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/strtoul.h" */

// For functions that return 0 on a success and -1 on failure
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1
#endif // ifndef EXIT_SUCCESS

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "stdlib.h" */
/* "libs/PowerPC_EABI_Support/include/stl/locale.h" line 5 "PowerPC_EABI_Support/MSL_C/MSL_Common/locale_api.h" */
#ifndef _MSL_LOCALE_API_H
#define _MSL_LOCALE_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/locale_api.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef int (*__decode_mbyte)(wchar_t*, const char*, size_t);
typedef int (*__encode_mbyte)(char*, wchar_t);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/locale_api.h" */

#ifdef __cplusplus
extern "C" {
#endif

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char* currency_symbol;
    char frac_digits;
    char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char* int_curr_symbol;
    char int_frac_digits;
    char int_p_cs_precedes;
    char int_n_cs_precedes;
    char int_p_sep_by_space;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};

struct _loc_mon_cmpt {
    char CmptName[8];
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char* currency_symbol;
    char frac_digits;
    char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char* int_curr_symbol;
    char int_frac_digits;
    char int_p_cs_precedes;
    char int_n_cs_precedes;
    char int_p_sep_by_space;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};


struct _loc_num_cmpt{
    char CmptName[8];
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
};

struct _loc_time_cmpt {
    char CmptName[8];
    char* am_pm;
    char* DateTime_Format;
    char* Twelve_hr_format;
    char* Date_Format;
    char* Time_Format;
    char* Day_Names;
    char* MonthNames;
    char* TimeZone;
};

struct _loc_coll_cmpt {
    char CmptName[8];
    int char_start_value;
    int char_coll_tab_size;
    short char_spec_accents;
    unsigned short* char_coll_table_ptr;
    unsigned short* wchar_coll_seq_ptr;
};

struct _loc_ctype_cmpt{
    char CmptName[8];
    const unsigned short* ctype_map_ptr;
    const unsigned char* upper_map_ptr;
    const unsigned char* lower_map_ptr;
    const unsigned short* wctype_map_ptr;
    const wchar_t* wupper_map_ptr;
    const wchar_t* wlower_map_ptr;
    __decode_mbyte decode_mb;
    __encode_mbyte encode_wc;
};

struct __locale {
    struct __locale* next_locale;
    char locale_name[48];
    struct _loc_coll_cmpt* coll_cmpt_ptr;
    struct _loc_ctype_cmpt* ctype_cmpt_ptr;
    struct _loc_mon_cmpt* mon_cmpt_ptr;
    struct _loc_num_cmpt* num_cmpt_ptr;
    struct _loc_time_cmpt* time_cmpt_ptr;
};

extern struct __locale _current_locale;
extern struct lconv __lconv;

#ifdef __cplusplus
}
#endif
#endif
/* end "locale.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 3 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 4 "stdlib.h" */
/* end "stdlib.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 5 "wctype.h" */
#ifndef MSL_WCTYPE_H
#define MSL_WCTYPE_H

/* "libs/PowerPC_EABI_Support/include/stl/wctype.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wctype.h" line 4 "locale.h" */
/* end "locale.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wctype.h" line 5 "PowerPC_EABI_Support/MSL_C/MSL_Common/wctype_api.h" */
#ifndef _MSL_WCTYPE_API_H
#define _MSL_WCTYPE_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wctype_api.h" line 3 "types.h" */
/* end "types.h" */


#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

#define wctype_alpha 0x0001
#define wctype_blank 0x0002
#define wctype_cntrl 0x0004
#define wctype_digit 0x0008
#define wctype_graph 0x0010
#define wctype_lower 0x0020
#define wctype_print 0x0040
#define wctype_punct 0x0080
#define wctype_space 0x0100
#define wctype_upper 0x0200
#define wctype_xdigit 0x0400

#define wctype_alnum (wctype_alpha | wctype_digit)

extern const unsigned short __wctype_mapC[256];
extern const wchar_t __wlower_mapC[256];
extern const wchar_t __wupper_mapC[256];

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wctype_api.h" */

#ifdef __cplusplus
extern "C" {
#endif


static inline int iswalnum(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_alnum);
}

static inline int iswalpha(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_alpha);
}

static inline int iswblank(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_blank);
}

static inline int iswcntrl(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_cntrl);
}

static inline int iswdigit(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_digit);
}

static inline int iswgraph(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_graph);
}

static inline int iswlower(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_lower);
}

static inline int iswprint(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_print);
}

static inline int iswpunct(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_punct);
}

static inline int iswspace(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_space);
}

static inline int iswupper(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_upper);
}

static inline int iswxdigit(wint_t c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->wctype_map_ptr[c] & wctype_xdigit);
}

static inline wint_t towlower(wint_t c) {
    return (c < 0 || c >= 256) ? c : (int)(&_current_locale)->ctype_cmpt_ptr->wlower_map_ptr[c];
}

static inline wint_t towupper(wint_t c) {
    return (c < 0 || c >= 256) ? c : (int)(&_current_locale)->ctype_cmpt_ptr->wupper_map_ptr[c];
}

#ifdef __cplusplus
}
#endif
#endif
/* end "wctype.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 6 "wchar.h" */
#ifndef MSL_WCHAR_H
#define MSL_WCHAR_H

/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif

/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" */
#ifndef _WCHAR_IO_H
#define _WCHAR_IO_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" line 4 "stdio.h" */
#ifndef MSL_STDIO_H
#define MSL_STDIO_H

/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 8 "PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" */
#ifndef STDIO_API_H
#define STDIO_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 4 "PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" */
#ifndef _MSL_COMMON_FILE_STRUC_H
#define _MSL_COMMON_FILE_STRUC_H
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" line 2 "types.h" */
/* end "types.h" */

typedef unsigned long __file_handle;
typedef unsigned long fpos_t;
typedef struct _FILE _FILE, *P_FILE;

#define __ungetc_buffer_size 2

enum __file_kinds {
    __closed_file,
    __disk_file,
    __console_file,
    __unavailable_file
};

enum __open_modes {
    __must_exist,
    __create_if_necessary,
    __create_or_truncate
};

enum __file_orientation {
    __unoriented,
    __char_oriented,
    __wide_oriented
};

enum __io_modes {
    __read = 1,
    __write = 2,
    __read_write = 3,
    __append = 4
};

typedef struct __file_modes {
    u32 open_mode : 2;
    u32 io_mode : 3;
    u32 buffer_mode : 2;
    u32 file_kind : 3;

#ifdef _MSL_WIDE_CHAR
    u32 file_orientation : 2;
#endif /* _MSL_WIDE_CHAR */

    u32 binary_io : 1;
} __file_modes;

enum __io_states {
    __neutral,
    __writing,
    __reading,
    __rereading
};

typedef struct __file_state {
    u32 io_state : 3;
    u32 free_buffer : 1;
    u8 eof;
    u8 error;
} __file_state;

typedef void* __ref_con;
typedef void (*__idle_proc)(void);
typedef int (*__pos_proc)(__file_handle file, fpos_t* position, int mode, __ref_con ref_con);
typedef int (*__io_proc)(__file_handle file, u8* buff, size_t* count, __ref_con ref_con);
typedef int (*__close_proc)(__file_handle file);

struct _FILE {
    __file_handle handle;                           // _00
    __file_modes mode;                              // _04
    __file_state state;                              // _08
    u8 is_dynamically_allowed;                      // _0C
    u8 char_buffer;                                 // _0D
    u8 char_buffer_overflow;                        // _0E
    u8 ungetc_buffer[__ungetc_buffer_size];         // _0F
    wchar_t ungetwc_buffer[__ungetc_buffer_size];   // _12
    u32 position;                                   // _18
    u8* buffer;                                   // _1C
    u32 buffer_size;                                // _20
    u8* buffer_ptr;                               // _24
    u32 buffer_len;                                 // _28
    u32 buffer_alignment;                           // _2C
    u32 saved_buffer_len;                           // _30
    u32 buffer_pos;                                 // _34
    __pos_proc position_proc;                       // _38
    __io_proc read_proc;                            // _3C
    __io_proc write_proc;                           // _40
    __close_proc close_proc;                        // _44
    __ref_con ref_con;                              // _48
    _FILE* next_file_struct;                        // _4C
};

typedef struct _FILE FILE;


#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

// define standard C file pointer location names
#define SEEK_SET (0)
#define SEEK_CUR (1)
#define SEEK_END (2)

#define stdin &(__files[0])
#define stdout &(__files[1])
#define stderr &(__files[2])

#define _STATIC_FILES 4

extern FILE __files[];

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 5 "wchar.h" */
/* end "wchar.h" */

enum __ReadProcActions {
    __GetAChar,
    __UngetAChar,
    __TestForError
};

enum __WReadProcActions
{
    __GetAwChar,
    __UngetAwChar,
    __TestForwcsError
};

typedef struct {
    char* CharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __OutStrCtrl;

typedef struct{
    char* NextChar;
    int NullCharDetected;
} __InStrCtrl;

typedef struct {
    wchar_t * wCharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __wOutStrCtrl;

typedef struct {
    wchar_t * wNextChar;
    int    wNullCharDetected;
} __wInStrCtrl;

//__fread
size_t __fwrite(const void *pPtr, size_t memb_size, size_t num_memb, FILE *file);
int __StringRead(void *, int, int);
wint_t __wStringRead(void*, wint_t, int);

#endif // STDIO_API_H
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" */
#ifndef MSL_FILE_POS_H
#define MSL_FILE_POS_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" line 4 "stdio.h" */
/* end "stdio.h" */

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

int fseek(FILE* stream, u32 offset, int whence);
int _fseek(FILE* stream, u32 offset, int whence);
int ftell(FILE* stream);
int _ftell(FILE* stream);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" */
#ifndef MSL_FILE_IO_H
#define MSL_FILE_IO_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" line 4 "stdio.h" */
/* end "stdio.h" */

int fclose(FILE* file);
int fflush(FILE* file);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 11 "PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" */
#ifndef MSL_PRINTF_H
#define MSL_PRINTF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" line 3 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" line 4 "stdio.h" */
/* end "stdio.h" */


//printf
//printf_s
int fprintf(FILE*, const char* format, ...);
//fprintf_s
int vprintf(const char*, va_list);
//vprintf_s
//vfprintf
//vfprintf_s
int vsnprintf(char*, size_t, const char*, va_list);
//vsnprintf_s
int vsprintf(char*, const char*, va_list);
//vsprintf_s
int snprintf(char*, size_t, const char*, ...);
//snprintf_s
int sprintf(char*, const char*, ...);
//sprintf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 12 "PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" */
#ifndef MSL_SCANF_H
#define MSL_SCANF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" line 3 "stdarg.h" */
/* end "stdarg.h" */

//fscanf
//fscanf_s
//vscanf
//scanf
//scanf_s
//vfscanf
//vfscanf_s
int vsscanf(const char*, const char*, va_list);
//vsscanf_s
int sscanf(const char*, const char*, ...);
//sscanf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" */

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "stdio.h" */

int fwide(FILE* stream, int mode);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" */
#ifndef MSL_WCSTOUL_H
#define MSL_WCSTOUL_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" line 3 "types.h" */
/* end "types.h" */


unsigned long __wcstoul(int, int, wint_t (*wReadProc)(void*, wint_t, int), void*, int*, int*, int*);
//__wcstoull
//wcstoul
//wcstoull
long wcstol(const wchar_t*, wchar_t**, int);
//wcstoll
//watoi
//watol

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 11 "PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" */
#ifndef MSL_WMEM_H
#define MSL_WMEM_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" line 3 "types.h" */
/* end "types.h" */

wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n);
wchar_t* wmemchr(wchar_t* s, wchar_t c, int n);
void* memmove(void*, const void*, size_t);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 12 "PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" */
#ifndef MSL_WPRINTF_H
#define MSL_WPRINTF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 4 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 5 "stdio.h" */
/* end "stdio.h" */

//wprintf
//wprintf_s
//fwprintf
//fwprintf_s
//vwprintf
//vwprintf_s
//vfwprintf
//vfwprintf_s
int swprintf(wchar_t*, size_t, const wchar_t*, ...);
//swprintf_s
//snwprintf_s
int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);
//vswprintf_s
//vsnwprintf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 13 "PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" */
#ifndef MSL_WSTRING_H
#define MSL_WSTRING_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" line 3 "types.h" */
/* end "types.h" */

size_t wcslen(const wchar_t*);
wchar_t* wcscpy(wchar_t*, const wchar_t*);
wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t);
wchar_t* wcscat(wchar_t*, const wchar_t*);
int wcscmp(const wchar_t*, const wchar_t*);
wchar_t* wcschr(const wchar_t*, wchar_t);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" */

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "wchar.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/wprintf.c" line 7 "stdio.h" */
/* end "stdio.h" */

#define TARGET_FLOAT_BITS 64
#define TARGET_FLOAT_BYTES (TARGET_FLOAT_BITS/8)
#define TARGET_FLOAT_MAX_EXP  LDBL_MAX_EXP
#define TARGET_FLOAT_MANT_DIG LDBL_MANT_DIG
#define TARGET_FLOAT_IMPLICIT_J_BIT 1
#define TARGET_FLOAT_MANT_BITS (TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS  (TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

typedef long long intmax_t;

enum justification_options {
    left_justification,
    right_justification,
    zero_fill
};

enum sign_options {
    only_minus,
    sign_always,
    space_holder
};

enum argument_options {
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    long_long_argument,
    wchar_argument,
    intmax_argument,
    size_t_argument,
    ptrdiff_argument,
    long_double_argument
};

typedef struct {
    unsigned char justification_options; // _0
    unsigned char sign_options;          // _1
    unsigned char precision_specified;   // _2
    unsigned char alternate_form;        // _3
    unsigned char argument_options;      // _4
    wchar_t conversion_char;             // _5
    int field_width;                     // _8
    int precision;                       // _C
} print_format;

static const wchar_t* parse_format(const wchar_t *format_string, va_list *arg, print_format *format) {
    print_format f;
    const wchar_t* s = format_string;
    wchar_t c;
    int flag_found;
    f.justification_options = right_justification;
    f.sign_options = only_minus;
    f.precision_specified = 0;
    f.alternate_form = 0;
    f.argument_options = normal_argument;
    f.field_width = 0;
    f.precision = 0;
    
    if ((c = *++s) == L'%') {
        f.conversion_char = c;
        *format = f;
        return ((const wchar_t*)s + 1);
    }

    for (;;) {
        flag_found = 1;

        switch (c) {
            case L'-':
                f.justification_options = left_justification;
                break;
            case L'+':
                f.sign_options = sign_always;
                break;
            case L' ':
                if (f.sign_options != sign_always) {
                    f.sign_options = space_holder;
                }
                break;
            case L'#':
                f.alternate_form = 1;
                break;
            case L'0':
                if (f.justification_options != left_justification) {
                    f.justification_options = zero_fill;
                }
                break;
            default:
                flag_found = 0;
                break;
        }

        if (flag_found) {
            c = *++s;
        }
        else {
            break;
        }
    }

    if (c == L'*') {
        if ((f.field_width = va_arg(*arg, int)) < 0) {
            f.justification_options = left_justification;
            f.field_width = -f.field_width;
        }

        c = *++s;
    }
    else {
        while (iswdigit(c)) {
            f.field_width = (f.field_width * 10) + (c - L'0');
            c = *++s;
        }
    }

    if (f.field_width > 509) {
        f.conversion_char = 0xFFFF;
        *format = f;
        return ((const wchar_t*)s + 1);
    }

    if (c == L'.') {
        f.precision_specified = 1;

        if ((c = *++s) == L'*') {
            if ((f.precision = va_arg(*arg, int)) < 0) {
                f.precision_specified = 0;
            }

            c = *++s;
        }
        else {
            while (iswdigit(c)) {
                f.precision = (f.precision * 10) + (c - L'0');
                c = *++s;
            }
        }
    }

    flag_found = 1;

    switch (c) {
        case L'h':
            f.argument_options = short_argument;

            if (s[1] == L'h') {
                f.argument_options = char_argument;
                c = *++s;
            }

            break;

        case L'l':
            f.argument_options = long_argument;

            if (s[1] == L'l') {
                f.argument_options = long_long_argument;
                c = *++s;
            }

            break;

        case L'L':
            f.argument_options = long_double_argument;
            break;
        case L'j':
            f.argument_options = intmax_argument;
            break;
        case L't':
            f.argument_options = ptrdiff_argument;
            break;
        case L'z':
            f.argument_options = size_t_argument;
            break;
        default:
            flag_found = 0;
            break;
    }

    if (flag_found) {
        c = *++s;
    }

    f.conversion_char = c;

    switch (c) {
        case L'd':
        case L'i':
        case L'u':
        case L'o':
        case L'x':
        case L'X':
            if (f.argument_options == long_double_argument) {
                f.argument_options = long_long_argument;
            }

            if (!f.precision_specified) {
                f.precision = 1;
            }
            else if (f.justification_options == zero_fill) {
                f.justification_options = right_justification;
            }
            break;

        case L'f':
        case L'F':
            if (f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument) {
                f.conversion_char = 0xFFFF;
                break;
            }

            if (!f.precision_specified) {
                f.precision = 6;
            }
            break;

        case L'a':
        case L'A':
            if (!f.precision_specified) {
                f.precision = 0xD;
            }

            if (f.argument_options == short_argument || f.argument_options == intmax_argument ||
                f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument ||
                f.argument_options == long_long_argument || f.argument_options == char_argument) {
                f.conversion_char = 0xFFFF;
            }

            break;

        case L'g':
        case L'G':
            if (!f.precision) {
                f.precision = 1;
            }

        case L'e':
        case L'E':
            if (f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument || f.argument_options == char_argument) {
                f.conversion_char = 0xFFFF;
                break;
            }

            if (!f.precision_specified) {
                f.precision = 6;
            }
            break;

        case L'p':
            f.argument_options = long_argument;
            f.alternate_form = 1;
            f.conversion_char = L'x';
            f.precision = 8;
            break;

        case L'c':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.precision_specified || f.argument_options != normal_argument) {
                    f.conversion_char = 0xFFFF;
                }
            }

            break;

        case L's':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFFFF;
                }
            }

            break;

        case L'n':
            if (f.argument_options == long_double_argument) {
                f.argument_options = long_long_argument;
            }
            
            break;

        default:
            f.conversion_char = 0xFFFF;
            break;
    }

    *format = f;
    return ((const wchar_t*)s + 1);
}

static wchar_t * long2str(long num, wchar_t * buff, print_format format) {
    unsigned long unsigned_num, base;
    wchar_t* p;
    int n, digits;
    int minus = 0;
    unsigned_num = num;
    
    p = buff;
    *--p = 0;
    digits = 0;
    
    if (!num && !format.precision && !(format.alternate_form && format.conversion_char == L'o')) {
        return p;
    }
    
    switch (format.conversion_char) {
        case L'd':
        case L'i':
            base = 10;
            if (num < 0) {
                if (num != 0x80000000) {
                    unsigned_num = -num;
                }
                minus = 1;
            }
            break;
        case L'o':
            base = 8;
            format.sign_options = only_minus;
            break;
        case L'u':
            base = 10;
            format.sign_options = only_minus;
            break;
        case L'x':
        case L'X':
            base = 16;
            format.sign_options = only_minus;
            break;
    }
    
    do {
        n = unsigned_num % base;
        unsigned_num /= base;
        
        if (n < 10) {
            n += L'0';
        } else {
            n -= 10;
            if (format.conversion_char == L'x') {
                n += L'a';
            } else {
                n += L'A';
            }
        }
        
        *--p = n;
        ++digits;
    } while (unsigned_num != 0);
    
    if (base == 8 && format.alternate_form && *p != L'0') {
        *--p = L'0';
        ++digits;
    }
    
    if (format.justification_options == zero_fill) {
        format.precision = format.field_width;
        if (minus || format.sign_options != only_minus)
            --format.precision;
        if (base == 16 && format.alternate_form)
            format.precision -= 2;
    }
    
    if (buff - p + format.precision > 509)
        return 0;
    
    while (digits < format.precision) {
        *--p = L'0';
        ++digits;
    }
    
    if (base == 16 && format.alternate_form) {
        *--p = format.conversion_char;
        *--p = L'0';
    }
    
    if (minus) {
        *--p = L'-';
    } else if (format.sign_options == sign_always) {
        *--p = L'+';
    } else if (format.sign_options == space_holder) {
        *--p = L' ';
    }
    
    return p;
}

static wchar_t* longlong2str(long long num, wchar_t *pBuf, print_format fmt) {
    unsigned long long unsigned_num, base;
    wchar_t* p;
    int n, digits;
    int minus = 0;
    unsigned_num = num;
    minus = 0;
    p = pBuf;
    *--p = 0;
    digits = 0;

    if (!num && !fmt.precision && !(fmt.alternate_form && fmt.conversion_char == L'o')) {
        return p;
    }

    switch (fmt.conversion_char) {
        case L'd':
        case L'i':
            base = 10;

            if (num < 0) {
                if (num != 0x8000000000000000) {
                    unsigned_num = -num;
                }

                minus = 1;
            }
            break;
        case L'o':
            base = 8;
            fmt.sign_options = only_minus;
            break;
        case L'u':
            base = 10;
            fmt.sign_options = only_minus;
            break;
        case L'x':
        case L'X':
            base = 16;
            fmt.sign_options = only_minus;
            break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10) {
            n += L'0';
        }
        else {
            n -= 10;
            if (fmt.conversion_char == L'x') {
                n += L'a';
            }
            else {
                n += L'A';
            }
        }

        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    if (base == 8 && fmt.alternate_form && *p != L'0') {
        *--p = L'0';
        ++digits;
    }

    if (fmt.justification_options == zero_fill) {
        fmt.precision = fmt.field_width;

        if (minus || fmt.sign_options != only_minus) {
            --fmt.precision;
        }

        if (base == 16 && fmt.alternate_form) {
            fmt.precision -= 2;
        }
    }

    if (pBuf - p + fmt.precision > 509) {
        return 0;
    }

    while (digits < fmt.precision) {
        *--p = L'0';
        ++digits;
    }

    if (base == 16 && fmt.alternate_form) {
        *--p = fmt.conversion_char;
        *--p = L'0';
    }

    if (minus) {
        *--p = L'-';
    }
    else if (fmt.sign_options == sign_always) {
        *--p = L'+';
    }
    else if (fmt.sign_options == space_holder) {
        *--p = L' ';
    }

    return p;
}

static wchar_t * double2hex(long double num, wchar_t * buff, print_format format)  {
    wchar_t *p;
    unsigned char *q;
    unsigned char working_byte;
    long double ld;
    int expbits, expmask;
    unsigned snum;
    long exp;
    print_format exp_format;
    int hex_precision;
    int mantissa_bit;
    decform form;
    decimal dec;
    int radix_marker;
    
    radix_marker = *(unsigned char *)(__lconv).decimal_point;
    p = buff;
    ld = num;
    
    if (format.precision > 509) {
        return 0;
    }

    form.style = (char)0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);


    switch(*dec.sig.text){
        case '0':
            dec.exp = 0;
            if (dec.sign) {
                p = buff - 5;
                if (format.conversion_char == L'A') wcscpy(p, L"-0X0");
                else wcscpy(p, L"-0x0");
            } else {
                p = buff - 4;
                if (format.conversion_char == L'A') wcscpy(p, L"0X0");
                else wcscpy(p, L"0x0");
            }
    
            return p;
        case 'I':
            if (dec.sign) {
                p = buff - 5;
                if (format.conversion_char == L'A') wcscpy(p, L"-INF");
                else wcscpy(p, L"-inf");
            } else {
                p = buff - 4;
                if (format.conversion_char == L'A') wcscpy(p, L"INF");
                else wcscpy(p, L"inf");
            }
    
            return p;
        case 'N':
            if (dec.sign) {
                p = buff - 5;
                if (format.conversion_char == L'A') wcscpy(p, L"-NAN");
                else wcscpy(p, L"-nan");
            } else {
                p = buff - 4;
                if (format.conversion_char == L'A') wcscpy(p, L"NAN");
                else wcscpy(p, L"nan");
            }
            
            return p;
    }

    exp_format.justification_options = 1;
    exp_format.sign_options = 1;
    exp_format.precision_specified = 0;
    exp_format.alternate_form = 0;
    exp_format.argument_options = 0;
    exp_format.field_width = 0;
    exp_format.precision = 1;
    exp_format.conversion_char = 'd';

    expbits = 11;
    expmask = 0x7FF;

    snum = ((unsigned char *)&num)[0] << 25;
    if (TARGET_FLOAT_EXP_BITS > 7)
        snum |= ((unsigned char *)&num)[1] << 17;
    if (TARGET_FLOAT_EXP_BITS > 15)
        snum |= ((unsigned char *)&num)[2] << 9;
    if (TARGET_FLOAT_EXP_BITS > 23)
        snum |= ((unsigned char *)&num)[3] << 1;

    snum = (snum >> (32 - expbits)) & expmask;
    if(snum != 0) exp = snum - 0x3FF;
    else exp = 0;

    p = long2str(exp, buff, exp_format);
    if (format.conversion_char == L'a')
        *--p = L'p';
    else
        *--p = L'P';
    q = (unsigned char *)&num;

    if (TARGET_FLOAT_IMPLICIT_J_BIT) {
        mantissa_bit = (1 + expbits + format.precision * 4) - 1;
    }
    else {
        mantissa_bit = (1 + expbits + format.precision * 4) - 4;
    }
    
    for (hex_precision = format.precision; hex_precision >= 1; hex_precision--) {
        if (mantissa_bit < 64) {
            int mantissa_byte;
            
            mantissa_byte = mantissa_bit >> 3;
            working_byte = (*(q + mantissa_byte)) >> (7 - (mantissa_bit & 7));

            if ((mantissa_bit & ~7) != ((mantissa_bit - 4) & ~7)) {
                working_byte |= ((*(q + (mantissa_byte - 1))) << 8) >> (7 - ((mantissa_bit) & 7));
            }

            if (!TARGET_FLOAT_IMPLICIT_J_BIT) {
                if (mantissa_bit == 1 + expbits) {
                    *--p = radix_marker;
                    working_byte &= 0x1;
                }
            }
            
            if ((working_byte &= 0xF) < 10) {
                working_byte += L'0';
            }
            else
                if (format.conversion_char == L'a') {
                    working_byte += L'a' - 10;
                }
                else {
                    working_byte += L'A' - 10;
                }
        }
        else {
            working_byte = L'0';
        }

        *--p = working_byte;
        mantissa_bit -= 4;
    }
    
    if (TARGET_FLOAT_IMPLICIT_J_BIT){
        if (format.precision || format.alternate_form) {
            *--p = radix_marker;
        }

        if (fabsl(ld) != 0.0)
            *--p = '1';
        else
            *--p = '0';
    }

    if (format.conversion_char == L'a') {
        *--p = L'x';
    }
    else {
        *--p = L'X';
    }

    *--p = L'0';

    if (dec.sign) {
        *--p = L'-';
    }
    else if (format.sign_options == sign_always) {
        *--p = L'+';
    }
    else if (format.sign_options == space_holder) {
        *--p = L' ';
    }

    return p;
}

static void round_decimal(decimal *dec, int new_length) {
    char c;
    char* p;
    int carry;

    if (new_length < 0) {
return_zero:
        dec->exp = 0;
        dec->sig.length = 1;
        *dec->sig.text = '0';
        return;
    }

    if (new_length >= dec->sig.length) {
        return;
    }

    p = (char*)dec->sig.text + new_length + 1;
    c = *--p - '0';

    if (c == 5) {
        char* q = &((char*)dec->sig.text)[dec->sig.length];

        while (--q > p && *q == '0')
            ;
        carry = (q == p) ? p[-1] & 1 : 1;
    } else {
        carry = (c > 5);
    }

    while (new_length != 0) {
        c = *--p - '0' + carry;

        if ((carry = (c > 9)) != 0 || c == 0) {
            --new_length;
        } else {
            *p = c + '0';
            break;
        }
    }

    if (carry != 0) {
        dec->exp += 1;
        dec->sig.length = 1;
        *dec->sig.text = '1';
        return;
    } else if (new_length == 0) {
        goto return_zero;
    }

    dec->sig.length = new_length;
}

static wchar_t* float2str(long double num, wchar_t *wbuff, print_format format) {
    decimal dec;
    decform form;
    wchar_t* pw;
    char* p;
    char buff[512];
    char* q;
    int n, digits, sign;
    int int_digits, frac_digits;
    int radix_marker;

    radix_marker = *(unsigned char*)__lconv.decimal_point;

    if (format.precision > 509) {
        return 0;
    }

    form.style = 0;
    form.digits = 0x20;
    __num2dec(&form, num, &dec);
    p = (char*)dec.sig.text + dec.sig.length;

    while (dec.sig.length > 1 && *--p == '0') {
        --dec.sig.length;
        ++dec.exp;
    }

    switch (*dec.sig.text) {
        case '0':
            dec.exp = 0;
            break;
        case 'I':
            if (num < 0) {
                pw = wbuff - 5;

                if (iswupper(format.conversion_char)) {
                    wcscpy(pw, L"-INF");
                }
                else {
                    wcscpy(pw, L"-inf");
                }
            }
            else {
                pw = wbuff - 4;
                if (iswupper(format.conversion_char)) {
                    wcscpy(pw, L"INF");
                }
                else {
                    wcscpy(pw, L"inf");
                }
            }

            return pw;

        case 'N':
            if (dec.sign) {
                pw = wbuff - 5;

                if (iswupper(format.conversion_char)) {
                    wcscpy(pw, L"-NAN");
                }
                else {
                    wcscpy(pw, L"-nan");
                }
            }
            else {
                pw = wbuff - 4;
                if (iswupper(format.conversion_char)) {
                    wcscpy(pw, L"NAN");
                }
                else {
                    wcscpy(pw, L"nan");
                }
            }

            return pw;
    }

    dec.exp += dec.sig.length - 1;
    p = buff + 512;
    *--p = 0;

    switch (format.conversion_char)
    {
        case L'g':
        case L'G':
            
            if (dec.sig.length > format.precision) {
                round_decimal(&dec, format.precision);
            }
            
            if (dec.exp < -4 || dec.exp >= format.precision)
            {
                if (format.alternate_form) {
                    --format.precision;
                }
                else {
                    format.precision = dec.sig.length - 1;
                }
                
                if (format.conversion_char == L'g') {
                    format.conversion_char = L'e';
                }
                else {
                    format.conversion_char = L'E';
                }
                
                goto e_format;
            }
            
            if (format.alternate_form) {
                format.precision -= dec.exp + 1;
            }
            else {
                if ((format.precision = dec.sig.length - (dec.exp + 1)) < 0) {
                    format.precision = 0;
                }
            }
            
            goto f_format;
        
        case L'e':
        case L'E':
        e_format:
            
            if (dec.sig.length > format.precision + 1) {
                round_decimal(&dec, format.precision + 1);
            }
            
            n = dec.exp;
            sign = '+';
            
            if (n < 0) {
                n = -n;
                sign = '-';
            }
            
            for (digits = 0; n || digits < 2; ++digits) {
                *--p = n % 10 + '0';
                n /= 10;
            }
            
            *--p = sign;
            *--p = format.conversion_char;
            
            if (buff - p + format.precision > 509) {
                return 0;
            }
            
            if (dec.sig.length < format.precision + 1) {
                for (n = format.precision + 1 - dec.sig.length + 1; --n;) {
                    *--p = '0';
                }
            }
            
            for (n = dec.sig.length, q = (char*)dec.sig.text + dec.sig.length; --n;) {
                *--p = *--q;
            }
            
            if (format.precision || format.alternate_form) {
                *--p = radix_marker;
            }
            
            *--p = *dec.sig.text;
            
            if (dec.sign)
                *--p = '-';
            else if (format.sign_options == sign_always)
                *--p = '+';
            else if (format.sign_options == space_holder)
                *--p = ' ';
            
            break;

        case L'f':
        case L'F':
        f_format:
            
            if ((frac_digits = -dec.exp + dec.sig.length - 1) < 0)
                frac_digits = 0;
            
            if (frac_digits > format.precision) {
                round_decimal(&dec, dec.sig.length - (frac_digits - format.precision));
                
                if ((frac_digits = -dec.exp + dec.sig.length - 1) < 0)
                    frac_digits = 0;
            }
            
            if ((int_digits = dec.exp + 1) < 0)
                int_digits = 0;
            
            if (int_digits + frac_digits > 509)
                return 0;
            
            q = (char *) dec.sig.text + dec.sig.length;
            
            for (digits = 0; digits < (format.precision - frac_digits); ++digits)
                *--p = '0';
            
            for (digits = 0; digits < frac_digits && digits < dec.sig.length; ++digits)
                *--p = *--q;
            
            for (; digits < frac_digits; ++digits)
                *--p = '0';
            
            if (format.precision || format.alternate_form)
                *--p = radix_marker;
            
            if (int_digits) {
                for (digits = 0; digits < int_digits - dec.sig.length; ++digits) {
                    *--p = '0';
                }

                for (; digits < int_digits; ++digits) {
                    *--p = *--q;
                }
            }
            else {
                *--p = '0';
            }
            
            if (dec.sign) {
                *--p = '-';
            }
            else if (format.sign_options == sign_always) {
                *--p = '+';
            }
            else if (format.sign_options == space_holder) {
                *--p = ' ';
            }
            
            break;
    }

    pw = wbuff - strlen(p) - 1;
    mbstowcs(pw, p, strlen(p));
    return pw;
}

static int __wpformatter(void *(*WriteProc)(void *, const wchar_t *, size_t), void *WriteProcArg, const wchar_t * format_str, va_list arg, int is_secure) {
    int num_chars, chars_written, field_width;
    const wchar_t* format_ptr;
    const wchar_t* curr_format;
    print_format format;
    long long_num;
    long long long_long_num;
    long double long_double_num;
    wchar_t buff[512];
    wchar_t* buff_ptr;
    wchar_t* string_end;
    char* cstring_end;
    wchar_t fill_char = L' ';
    char* strptr;

    format_ptr = format_str;
    chars_written = 0;

    while (*format_ptr) {
        if (!(curr_format = wcschr(format_ptr, L'%'))) {
            num_chars = wcslen(format_ptr);
            chars_written += num_chars;

            if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
                return -1;
            }

            break;
        }

        num_chars = curr_format - format_ptr;
        chars_written += num_chars;

        if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
            return -1;
        }

        format_ptr = curr_format;
        format_ptr = parse_format(format_ptr, (va_list*)arg, &format);

        switch (format.conversion_char) {
            case L'd':
            case L'i':
                if (format.argument_options == long_argument) {
                    long_num = va_arg(arg, long);
                }
                else if (format.argument_options == long_long_argument) {
                    long_long_num = va_arg(arg, long long);
                }
                else if (format.argument_options == intmax_argument) {
                    long_long_num = va_arg(arg, intmax_t);
                }
                else if (format.argument_options == size_t_argument) {
                    long_num = va_arg(arg, size_t);
                }
                else if (format.argument_options == ptrdiff_argument) {
                    long_num = va_arg(arg, ptrdiff_t);
                }
                else {
                    long_num = va_arg(arg, int);
                }

                if (format.argument_options == short_argument) {
                    long_num = (short)long_num;
                }

                if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument)) {
                    if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format))) {
                        goto conversion_error;
                    }
                }
                else {
                    if (!(buff_ptr = long2str(long_num, buff + 512, format))) {
                        goto conversion_error;
                    }
                }

                num_chars = buff + 512 - 1 - buff_ptr;
                break;

            case L'o':
            case L'u':
            case L'x':
            case L'X':
                if (format.argument_options == long_argument) {
                    long_num = va_arg(arg, unsigned long);
                }
                else if (format.argument_options == long_long_argument) {
                    long_long_num = va_arg(arg, long long);
                }
                else if (format.argument_options == intmax_argument) {
                    long_long_num = va_arg(arg, intmax_t);
                }
                else if (format.argument_options == size_t_argument) {
                    long_num = va_arg(arg, size_t);
                }
                else if (format.argument_options == ptrdiff_argument) {
                    long_num = va_arg(arg, ptrdiff_t);
                }
                else {
                    long_num = va_arg(arg, unsigned int);
                }

                if (format.argument_options == short_argument) {
                    long_num = (unsigned short)long_num;
                }

                if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument)) {
                    if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format))) {
                        goto conversion_error;
                    }
                }
                else {
                    if (!(buff_ptr = long2str(long_num, buff + 512, format))) {
                        goto conversion_error;
                    }
                }

                num_chars = buff + 512 - 1 - buff_ptr;
                break;

            case L'f':
            case L'F':
            case L'e':
            case L'E':
            case L'g':
            case L'G':
                if (format.argument_options == long_double_argument) {
                    long_double_num = va_arg(arg, long double);
                }
                else {
                    long_double_num = va_arg(arg, double);
                }

                if (!(buff_ptr = float2str(long_double_num, buff + 512, format))) {
                    goto conversion_error;
                }

                num_chars = buff + 512 - 1 - buff_ptr;
                break;

            case 'a':
            case 'A':
                if (format.argument_options == long_double_argument) {
                    long_double_num = va_arg(arg, long double);
                }
                else {
                    long_double_num = va_arg(arg, double);
                }

                if (!(buff_ptr = double2hex(long_double_num, buff + 512, format))) {
                    goto conversion_error;
                }

                num_chars = buff + 512 - 1 - buff_ptr;
                break;

            case L's':
                if (format.argument_options == wchar_argument) {
                    buff_ptr = va_arg(arg, wchar_t *);

                    if(is_secure && buff_ptr == NULL){
                        __msl_runtime_constraint_violation_s(NULL, NULL, -1);
                        return -1;
                    }

                    if (buff_ptr == NULL) {
                        buff_ptr = L"";
                    }

                    if (format.alternate_form)
                    {
                        num_chars = (unsigned char) *buff_ptr++;
                        
                        if (format.precision_specified && num_chars > format.precision)
                            num_chars = format.precision;
                    }
                    else if (format.precision_specified)
                    {
                        num_chars = format.precision;
                        
                        if ((string_end = (wchar_t *)wmemchr(buff_ptr, 0, num_chars)) != 0) {
                            num_chars = string_end - buff_ptr;
                        }
                    }
                    else
                        num_chars = wcslen(buff_ptr);
                }
                else {
                    strptr = va_arg(arg, char *);

                    if(is_secure && strptr == NULL){
                        __msl_runtime_constraint_violation_s(0,0,-1);
                        return -1;
                    }

                    if (strptr == NULL) {
                          strptr = "";
                    }

                    if (format.alternate_form) {
                        num_chars = (unsigned char) *buff_ptr++;
                        
                        if (format.precision_specified && num_chars > format.precision) {
                            num_chars = format.precision;
                        }
                    }
                    else if (format.precision_specified) {
                        num_chars = format.precision;
                        
                        if ((cstring_end = (char *) memchr(strptr, 0, num_chars)) != 0) {
                            num_chars = cstring_end - strptr;
                        }
                    }
                    else
                        num_chars = strlen(strptr);
                      if ((num_chars = mbstowcs(buff, strptr, num_chars)) < 0)
                        goto conversion_error;
                    
                    buff_ptr = &buff[0];
                }

                break;

            case 'n':
                if(is_secure){
                    __msl_runtime_constraint_violation_s(0,0,-1);
                    return -1;
                }

                buff_ptr = va_arg(arg, wchar_t *);

                switch (format.argument_options) {
                    case normal_argument:
                        *(int*)buff_ptr = chars_written;
                        break;
                    case short_argument:
                        *(short*)buff_ptr = chars_written;
                        break;
                    case long_argument:
                        *(long*)buff_ptr = chars_written;
                        break;
                    case intmax_argument:
                        *(intmax_t*)buff_ptr = chars_written;
                        break;
                    case size_t_argument:
                        *(size_t*)buff_ptr = chars_written;
                        break;
                    case ptrdiff_argument:
                        *(ptrdiff_t*)buff_ptr = chars_written;
                        break;
                    case long_long_argument:
                        *(long long*)buff_ptr = chars_written;
                        break;
                }

                continue;

            case L'c':
                buff_ptr = buff;
                if (format.argument_options == wchar_argument) {
                    *buff_ptr = va_arg(arg, int);
                    num_chars = 1;
                }
                else {
                    char chint = va_arg(arg, int);
                    num_chars = mbtowc(buff, &chint, 1);
                }
                
                break;

            case L'%':
                buff_ptr = buff;
                *buff_ptr = '%';
                num_chars = 1;
                break;
            
            case 0xFFFF:
            default:
                conversion_error:
                    num_chars = wcslen(curr_format);
                    chars_written += num_chars;

                    if (num_chars && !(*WriteProc)(WriteProcArg, curr_format, num_chars)) {
                        return -1;
                    }

                    return chars_written;
                    break;
        }

        field_width = num_chars;

        if (format.justification_options != left_justification) {
            fill_char = (format.justification_options == zero_fill) ? L'0' : L' ';

            if (((*buff_ptr == L'+') || (*buff_ptr == L'-') || (*buff_ptr == L' ')) && (fill_char == L'0')) {
                if ((*WriteProc)(WriteProcArg, buff_ptr, 1) == 0) {
                    return -1;
                }

                ++buff_ptr;
                num_chars--;
            }

            if ((format.justification_options == zero_fill) && ((format.conversion_char == L'a') || (format.conversion_char == L'A'))){
                if (num_chars < 2)
                    return -1;
                if ((*WriteProc)(WriteProcArg, buff_ptr, 2) == 0)
                    return -1;
                num_chars -= 2;
                buff_ptr += 2;
            }

            while (field_width < format.field_width) {
                if ((*WriteProc)(WriteProcArg, &fill_char, 1) == 0) {
                    return -1;
                }

                ++field_width;
            }
        }

        if (num_chars && !(*WriteProc)(WriteProcArg, buff_ptr, num_chars)) {
            return -1;
        }

        if (format.justification_options == left_justification) {
            while (field_width < format.field_width) {
                wchar_t blank = L' ';

                if ((*WriteProc)(WriteProcArg, &blank, 1) == 0) {
                    return -1;
                }

                ++field_width;
            }
        }

        chars_written += field_width;
    }

    return chars_written;
}

//unused
void __wFileWrite(){
}

void* __wStringWrite(void *wosc, const wchar_t * buffer, size_t numchars) {
    size_t tobewritten;
    void* res;
    __wOutStrCtrl * wOscp = (__wOutStrCtrl*)wosc;
    tobewritten = ((wOscp->CharsWritten + numchars) <= wOscp->MaxCharCount) ? numchars : wOscp->MaxCharCount - wOscp->CharsWritten;
    res = (void*)wmemcpy(wOscp->wCharStr + wOscp->CharsWritten, buffer, tobewritten);
    wOscp->CharsWritten += tobewritten;
    return res;
}

//unused
void wprintf(){
}

//unused
void wprintf_s(){
}

//unused
void fwprintf(){
}

//unused
void fwprintf_s(){
}

//unused
void vwprintf(){
}

//unused
void vwprintf_s(){
}

//unused
void vfwprintf(){
}

//unused
void vfwprintf_s(){
}

//unused
int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...) {
    va_list args;
    va_start(args, format);
    return vswprintf(s, n, format, args);
}

//unused
void swprintf_s(){
}

//unused
void snwprintf_s(){
}

int vswprintf(wchar_t *s, size_t n, const wchar_t *format, va_list arg) {
    int count;
    __wOutStrCtrl wosc;
    wosc.wCharStr = s;
    wosc.MaxCharCount = n;
    wosc.CharsWritten = 0;

    count = __wpformatter(&__wStringWrite, &wosc, format, arg, FALSE);

    if (count >= 0) {
        if (count < n) {
            s[count] = 0;
        }
        else {
            s[n - 1] = 0;
            count = -1;
        }
    }

    return count;
}

//unused
void vswprintf_s(){
}

//unused
void vsnwprintf_s(){
}
