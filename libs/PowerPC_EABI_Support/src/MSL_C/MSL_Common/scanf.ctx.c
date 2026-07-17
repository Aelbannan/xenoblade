/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/scanf.c" line 0 "math.h" */
#ifndef MSL_MATH_H
#define MSL_MATH_H

/* "libs/PowerPC_EABI_Support/include/stl/math.h" line 3 "types.h" */
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
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/scanf.c" line 1 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/scanf.c" line 2 "ctype.h" */
#ifndef MSL_CTYPE_H
#define MSL_CTYPE_H

/* "libs/PowerPC_EABI_Support/include/stl/ctype.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/stl/ctype.h" line 4 "locale.h" */
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
/* "libs/PowerPC_EABI_Support/include/stl/ctype.h" line 5 "PowerPC_EABI_Support/MSL_C/MSL_Common/ctype_api.h" */
#ifndef _MSL_CTYPE_API_H
#define _MSL_CTYPE_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/ctype_api.h" line 3 "types.h" */
/* end "types.h" */


#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

#define ctype_alpha 0x0001
#define ctype_blank 0x0002
#define ctype_cntrl 0x0004
#define ctype_digit 0x0008
#define ctype_graph 0x0010
#define ctype_lower 0x0020
#define ctype_print 0x0040
#define ctype_punct 0x0080
#define ctype_space 0x0100
#define ctype_upper 0x0200
#define ctype_xdigit 0x0400

#define ctype_alnum (ctype_alpha | ctype_digit)

extern const unsigned short __ctype_mapC[256];
extern const unsigned char __lower_mapC[256];
extern const unsigned char __upper_mapC[256];

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/ctype_api.h" */

#ifdef __cplusplus
extern "C" {
#endif

static inline int isalnum(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_alnum);
}

static inline int isalpha(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_alpha);
}

static inline int isblank(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_blank);
}

static inline int iscntrl(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_cntrl);
}

static inline int isdigit(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_digit);
}

static inline int isgraph(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_graph);
}

static inline int islower(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_lower);
}

static inline int isprint(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_print);
}

static inline int ispunct(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_punct);
}

static inline int isspace(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_space);
}

static inline int isupper(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_upper);
}

static inline int isxdigit(int c) {
    return (c < 0 || c >= 256) ? 0 : (int)(_current_locale.ctype_cmpt_ptr->ctype_map_ptr[c] & ctype_xdigit);
}

static inline int tolower(int c) {
    return (c < 0 || c >= 256) ? c : (int)(&_current_locale)->ctype_cmpt_ptr->lower_map_ptr[c];
}

static inline int toupper(int c) {
    return (c < 0 || c >= 256) ? c : (int)(&_current_locale)->ctype_cmpt_ptr->upper_map_ptr[c];
}

#ifdef __cplusplus
}
#endif
#endif
/* end "ctype.h" */
/* "libs/PowerPC_EABI_Support/src/MSL_C/MSL_Common/scanf.c" line 3 "stdio.h" */
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

typedef long long intmax_t;

enum argument_options {
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    intmax_argument,
    size_t_argument,
    ptrdiff_argument,
    long_long_argument,
    double_argument,
    long_double_argument,
    wchar_argument
};

typedef unsigned char char_map[32];

typedef struct {
    unsigned char suppress_assignment;
    unsigned char field_width_specified;
    unsigned char argument_options;
    unsigned char conversion_char;
    int           field_width;
    char_map char_set;
} scan_format;

#define set_char_map(map, ch) map[(unsigned char)ch >> 3] |= (1 << (ch & 7))
#define tst_char_map(map, ch) (map[(unsigned char)ch >> 3] &  (1 << (ch & 7)))

static const char* parse_format(const char* format_string, scan_format *format) {
    const char* s = format_string;
    int c;
    int flag_found, invert;
    scan_format f = { 0, 0, normal_argument, 0, 2147483647, { 0 } };

    if (((c = *++s) == '%')) {
        f.conversion_char = c;
        *format = f;
        return ((const char*)s + 1);
    }

    if (c == '*') {
        f.suppress_assignment = 1;
        c = *++s;
    }

    if (isdigit(c)) {
        f.field_width = 0;

        do {
            f.field_width = (f.field_width * 10) + (c - '0');
            c = *++s;
        } while (isdigit(c));

        if (f.field_width == 0) {
            f.conversion_char = 0xFF;
            *format = f;
            return ((const char*)s + 1);
        }

        f.field_width_specified = 1;
    }

    flag_found = 1;

    switch (c) {
        case 'h':
            f.argument_options = short_argument;

            if (s[1] == 'h') {
                f.argument_options = char_argument;
                c = *++s;
            }

            break;
        case 'l':
            f.argument_options = long_argument;

            if (s[1] == 'l') {
                f.argument_options = long_long_argument;
                c = *++s;
            }

            break;
        case 'L':
            f.argument_options = long_double_argument;
            break;

        case 'j':
            f.argument_options = intmax_argument;
            break;
        case 'z':
            f.argument_options = size_t_argument;
            break;
        case 't':
            f.argument_options = ptrdiff_argument;
            break;
        default:
            flag_found = 0;
    }

    if (flag_found) {
        c = *++s;
    }

    f.conversion_char = c;
    
    switch (c) {
        case 'd':
        case 'i':
        case 'u':
        case 'o':
        case 'x':
        case 'X':
            if (f.argument_options == long_double_argument) {
                f.conversion_char = 0xFF;
                break;
            }

            break;

        case 'a':
        case 'A':
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (f.argument_options == char_argument || f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument) {
                f.conversion_char = 0xFF;
                break;
            }

            if (f.argument_options == long_argument) {
                f.argument_options = double_argument;
            }

            break;

        case 'p':
            f.argument_options = long_argument;
            f.conversion_char = 'x';
            break;

        case 'c':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            break;

        case 's':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            {
                int i;
                unsigned char* p;

                for (i = sizeof(f.char_set), p = f.char_set; i; --i) {
                    *p++ = 0xFF;
                }

                f.char_set[1] = 0xC1;
                f.char_set[4] = 0xFE;
            }

            break;

        case 'n':
            break;

        case '[':
            if (f.argument_options == long_argument) {
                f.argument_options = wchar_argument;
            }
            else {
                if (f.argument_options != normal_argument) {
                    f.conversion_char = 0xFF;
                }
            }

            c = *++s;
            invert = 0;

            if (c == '^') {
                invert = 1;
                c = *++s;
            }

            if (c == ']') {
                set_char_map(f.char_set, ']');
                c = *++s;
            }

            while (c && c != ']') {
                int d;
                set_char_map(f.char_set, c);

                if (*(s + 1) == '-' && (d = *(s + 2)) != 0 && d != ']') {
                    while (++c <= d) {
                        set_char_map(f.char_set, c);
                    }

                    c = *(s += 3);
                }
                else {
                    c = *++s;
                }
            }

            if (!c) {
                f.conversion_char = 0xFF;
                break;
            }

            if (invert) {
                int i;
                unsigned char* p;

                for (i = sizeof(f.char_set), p = f.char_set; i; --i, ++p) {
                    *p = ~*p;
                }

                break;
            }

            break;
        default:
            f.conversion_char = 0xFF;
            break;
    }

    *format = f;
    return ((const char*)s + 1);
}


static int __sformatter(int (*ReadProc)(void *, int, int), void * ReadProcArg, const char * format_str, va_list arg, int is_secure)
{
    int num_chars, chars_read, items_assigned, conversions;
    int base, negative, overflow;
    int rval;
    const char* format_ptr;
    char format_char;
    char c;
    scan_format format;
    long long_num;
    unsigned long u_long_num;
    long long long_long_num = 0;
    unsigned long long u_long_long_num;
    long double long_double_num;
    char * arg_ptr;
    int elem_valid;
    size_t elem_maxsize;
    int match_failure = 0;
    int terminate = 0;

    format_ptr = format_str;
    chars_read = 0;
    items_assigned = 0;
    conversions = 0;
    
    while (!terminate && (format_char = *format_ptr) != 0)
    {
        if (isspace(format_char))
        {
            do{
                format_char = *++format_ptr;
            } while (isspace(format_char));
            
            if (!match_failure)
            {
                while (isspace(c = (*ReadProc)(ReadProcArg, 0, __GetAChar)))
                    ++chars_read;
                
                (*ReadProc)(ReadProcArg, c, __UngetAChar);
            }
            continue;
        }
        
        if ((format_char != '%') && (!match_failure))
        {
            if ((c = (*ReadProc)(ReadProcArg, 0, __GetAChar)) != (unsigned char)format_char)
            {
                (*ReadProc)(ReadProcArg, c, __UngetAChar);
                if (!is_secure)
                    goto exit;
                else
                {
                    match_failure = 1;
                    ++format_ptr;
                    continue;
                }
            }
            
            chars_read++;
            format_ptr++;
            
            continue;
        }

        format_ptr = parse_format(format_ptr, &format);

        if (!format.suppress_assignment && format.conversion_char != '%')
        {
            arg_ptr = va_arg(arg, char *);
        }
        else
        {
            arg_ptr = 0;
        }
            
        if ((format.conversion_char != 'n') && (!match_failure) && (*ReadProc)(ReadProcArg, 0, __TestForError))
        {
            if (!is_secure)
                goto exit;
            else
                match_failure = 1;
        }
        
        switch (format.conversion_char)
        {
            case 'd':
                base = 10;
                goto signed_int;
            case 'i':
                base = 0;
            signed_int:
                
                if (match_failure)
                {
                    long_num = 0;
                    long_long_num = 0;
                }
                else
                {
                    if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument))
                        u_long_long_num = __strtoull(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    else
                        u_long_num = __strtoul(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    
                    if (!num_chars)
                    {
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            long_num = 0;
                            long_long_num = 0;
                            goto signed_int_assign;
                        }
                    }
                    
                    chars_read += num_chars;
                    
                    if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument))
                        long_long_num = (negative ? -u_long_long_num : u_long_long_num);
                    else
                        long_num = (negative ? -u_long_num : u_long_num);
                }
                
            signed_int_assign:

                if (arg_ptr)
                {
                    switch (format.argument_options)
                    {
                        case normal_argument:  * (int *)         arg_ptr = long_num; break;
                        case char_argument:    * (signed char *) arg_ptr = long_num; break;
                        case short_argument:   * (short *)       arg_ptr = long_num; break;
                        case long_argument:    * (long *)        arg_ptr = long_num; break;
                        case intmax_argument:  * (intmax_t *)    arg_ptr = long_long_num; break;
                        case size_t_argument:  * (size_t *)      arg_ptr = long_num; break;
                        case ptrdiff_argument: * (ptrdiff_t *)   arg_ptr = long_num; break;
                        case long_long_argument: * (long long *) arg_ptr = long_long_num; break;
                    }
                    if (!match_failure) items_assigned++;
                }

                conversions++;
                break;
            case 'o':
                base = 8;
                goto unsigned_int;
            case 'u':
                base = 10;
                goto unsigned_int;
            case 'x':
            case 'X':
                base = 16;
            unsigned_int:
                
                if (match_failure)
                {
                    u_long_num = 0;
                    u_long_long_num = 0;
                }
                else
                {
                    if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument))
                       u_long_long_num = __strtoull(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    else
                        u_long_num = __strtoul(base, format.field_width, ReadProc, ReadProcArg, &num_chars, &negative, &overflow);
                    
                    if (!num_chars)
                    {
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            u_long_num = 0;
                            u_long_long_num = 0;
                            goto unsigned_int_assign;
                        }
                    }
                    
                    chars_read += num_chars;
                    
                    if (negative)
                    if (format.argument_options == long_long_argument)
                        u_long_long_num = -u_long_long_num;
                    else
                        u_long_num = -u_long_num;
                }
                
            unsigned_int_assign:

                if (arg_ptr)
                {
                    switch (format.argument_options)
                    {
                        case normal_argument:  * (unsigned int *)arg_ptr = u_long_num; break;
                        case char_argument:    * (unsigned char *)arg_ptr = u_long_num; break;
                        case short_argument:   * (unsigned short *)arg_ptr = u_long_num; break;
                        case long_argument:    * (unsigned long *)arg_ptr = u_long_num; break;
                        case intmax_argument:  * (intmax_t *)    arg_ptr = u_long_long_num; break;
                        case size_t_argument:  * (size_t *)      arg_ptr = u_long_num; break;
                        case ptrdiff_argument: * (ptrdiff_t *)   arg_ptr = u_long_num; break;
                        case long_long_argument: * (unsigned long long *) arg_ptr = u_long_long_num; break;


                    }
                    
                    if (!match_failure) items_assigned++;
                }
                
                conversions++;
                break;
            case 'a':
            case 'A':
            case 'f':
            case 'F':
            case 'e':
            case 'E':
            case 'g':
            case 'G':
            flt:
                if (match_failure) long_double_num = NAN;
                else
                {
                    long_double_num = __strtold(format.field_width, ReadProc, ReadProcArg, &num_chars, &overflow);
                    
                    if (!num_chars)
                    {
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            long_double_num = NAN;
                            goto assign_float;
                        }
                    }
                    
                    chars_read += num_chars;
                }
                
            assign_float:

                if (arg_ptr)
                {
                    switch (format.argument_options)
                    {
                        case normal_argument:      * (float *)       arg_ptr = long_double_num; break;
                        case double_argument:      * (double *)      arg_ptr = long_double_num; break;
                        case long_double_argument: * (long double *) arg_ptr = long_double_num; break;
                    }
                    
                    if (!match_failure) items_assigned++;
                }

                conversions++;
                break;

            case 'c':
                
                if (!format.field_width_specified) format.field_width = 1;

                if (arg_ptr)
                {
                    if (is_secure)
                    {
                        elem_valid = 1;
                        elem_maxsize = va_arg(arg, size_t);
                    }
                    
                    num_chars = 0;
                    
                    if (match_failure)
                    {
                        if (elem_maxsize > 0)
                            *arg_ptr = 0;
                        continue;
                    }
                    else
                    {
                        char * arg_start = arg_ptr;
                        
                        while (format.field_width-- && (!is_secure || ((elem_valid = (elem_maxsize > num_chars)) != 0)) && ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                        {
                            c = rval;
                            
                            if (format.argument_options == wchar_argument)
                            {
                                mbtowc(((wchar_t*)arg_ptr), (char*)(&c), 1);
                                arg_ptr += sizeof(wchar_t);
                            }
                            else
                                *arg_ptr++ = c;
                            num_chars++;
                        }
                        
                        c = rval;
                        if (!num_chars || (is_secure && !elem_valid))
                        {
                            if (!is_secure)
                                goto exit;
                            else
                            {
                                match_failure = 1;
                                if (elem_maxsize > 0)
                                    *arg_start = 0;
                                continue;
                            }
                        }
                        
                        chars_read += num_chars;
                        
                        items_assigned++;
                    }
                }
                else
                {
                    num_chars = 0;
                    
                    while (format.field_width-- && ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                    {
                        c = rval;
                        num_chars++;
                    }
                    c = rval;
                    if (!num_chars) goto exit;
                }
                
                conversions++;
                break;
            
            case '%':
                if (match_failure)
                    continue;
                else
                {
                    while (isspace(c = (*ReadProc)(ReadProcArg, 0, __GetAChar)))
                        chars_read++;
                    
                    if (c != '%')
                    {
                        (*ReadProc)(ReadProcArg, c, __UngetAChar);
                        if (!is_secure)
                            goto exit;
                        else
                        {
                            match_failure = 1;
                            continue;
                        }
                    }
                    
                    chars_read++;
                }
                break;
            
            case 's':
                if (!match_failure)
                {
                    c = (*ReadProc)(ReadProcArg, 0, __GetAChar);
                    while(isspace(c))
                    {
                        chars_read++;
                        c = (*ReadProc)(ReadProcArg, 0, __GetAChar);
                    }
                    
                    (*ReadProc)(ReadProcArg, c, __UngetAChar);
                }
            case '[':
                if (arg_ptr)
                {
                    if (is_secure)
                    {
                        elem_valid = 1;
                        elem_maxsize = va_arg(arg, size_t) - 1;
                    }
                    
                    num_chars = 0;
                    
                    if (match_failure)
                    {
                        if (elem_maxsize > 0)
                            *arg_ptr = 0;
                        continue;
                    }
                    else
                    {
                        char * arg_start = arg_ptr;
                        
                        while (format.field_width-- &&
                            (!is_secure || ((elem_valid = (elem_maxsize >= num_chars)) != 0)) &&
                                    ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                        {
                            c = rval;
                            
                            if (!tst_char_map(format.char_set, c))
                                break;

                            if (format.argument_options == wchar_argument)
                            {
                                mbtowc(((wchar_t*)arg_ptr), (char*)&c, 1);
                                arg_ptr = (char*)((wchar_t*)arg_ptr + 1);
                            }
                            else
                                *arg_ptr++ = c;
                            num_chars++;
                        }
                        c = rval;
                        
                        if (!num_chars || (is_secure && !elem_valid))
                        {
                            (*ReadProc)(ReadProcArg, c, __UngetAChar);
                            if (!is_secure)
                                goto exit;
                            else
                            {
                                match_failure = 1;
                                if (elem_maxsize > 0)
                                    *arg_start = 0;
                                continue;
                            }
                        }
                        
                        chars_read += num_chars;
                        
                        if (format.argument_options == wchar_argument)
                            *(wchar_t*)arg_ptr = L'\0';
                        else
                            *arg_ptr = 0;
                        
                        items_assigned++;
                    }
                }
                else
                {
                    num_chars = 0;
                    
                    while (format.field_width-- &&
                        ((rval = ((*ReadProc)(ReadProcArg, 0, __GetAChar))) != -1))
                    {
                        c = rval;
                        if (!tst_char_map(format.char_set, c))
                            break;
                        
                        num_chars++;
                    }
                    c = rval;
                    
                    if (!num_chars)
                    {
                        (*ReadProc)(ReadProcArg, c, __UngetAChar);
                        break;
                    }
                    chars_read += num_chars;
                }
                
                if (format.field_width >= 0)
                    (*ReadProc)(ReadProcArg, c, __UngetAChar);
                
                conversions++;
                break;
            case 'n':
                if (arg_ptr)
                    switch (format.argument_options)
                    {
                        case normal_argument: * (int *)   arg_ptr = chars_read; break;
                        case short_argument:  * (short *) arg_ptr = chars_read; break;
                        case long_argument:   * (long *)  arg_ptr = chars_read; break;
                        case char_argument:   * (char *)  arg_ptr = chars_read; break;
                        case long_long_argument: * (long long *) arg_ptr = chars_read; break;
                    }
                continue;
            case 0xFF:
            default:
                goto exit;
        }
    }
    
exit:
    
    if ((*ReadProc)(ReadProcArg, 0, __TestForError) && conversions == 0)
        return -1;
    
    return items_assigned;
}

//unused
void __FileRead(){
}


int __StringRead(void *pPtr, int ch, int act) {
    char ret;
    __InStrCtrl* Iscp = (__InStrCtrl*)pPtr;

    switch (act) {
        case __GetAChar:
            ret = *(Iscp->NextChar);
            
            if (ret == '\0') {
                Iscp->NullCharDetected = 1;
                return -1;
            }
            else {
                Iscp->NextChar++;
                return (unsigned char)ret;
            }

        case __UngetAChar:
            if (Iscp->NullCharDetected == 0) {
                Iscp->NextChar--;
            }
            else {
                Iscp->NullCharDetected = 0;
            }

            return ch;

        case __TestForError:
            return Iscp->NullCharDetected;
    }

    return 0;
}

//unsued
void fscanf(){
}

//unused
void fscanf_s(){
}

//unused
void vscanf(){
}

//unused
void scanf(){
}

//unused
void scanf_s(){
}

//unused
void vfscanf(){
}

//unused
void vfscanf_s(){
}

int isspace_string(const char *s) {
    int i = 0;

    while(s[i] != '\0'){
       if(!isspace(s[i++])) return 0;
    }

    return 1;
}

int vsscanf(const char *s, const char *format, va_list arg) {
    __InStrCtrl isc;
    isc.NextChar = (char*)s;

    if ((s == 0) || (*isc.NextChar == '\0') || isspace_string(s)) {
        return -1;
    }

    isc.NullCharDetected = 0;
    return __sformatter(&__StringRead, (void*)&isc, format, arg, 0);
}

//unused
void vsscanf_s(){
}

int sscanf(const char *s, const char *pFormat, ...) {
    int ret;
    va_list args;
    va_start(args, pFormat);
    return vsscanf(s, pFormat, args);
}

//unused
void sscanf_s(){
}
