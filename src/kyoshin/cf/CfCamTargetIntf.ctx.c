/* "src/kyoshin/cf/CfCamTargetIntf.cpp" line 0 "kyoshin/cf/CfCamTargetIntf.hpp" */
#pragma once

/* "src/kyoshin/cf/CfCamTargetIntf.hpp" line 2 "kyoshin/cf/CfCamDirectionIntf.hpp" */
#pragma once

/* "src/kyoshin/cf/CfCamDirectionIntf.hpp" line 2 "types.h" */
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
/* "src/kyoshin/cf/CfCamDirectionIntf.hpp" line 3 "monolib/math.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math.hpp" line 2 "monolib/math/CAttrTransform.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 3 "monolib/math/CVec3.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CVec3.hpp" line 2 "monolib/math/Utility.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/Utility.hpp" line 2 "nw4r/math.h" */
#ifndef NW4R_PUBLIC_MATH_H
#define NW4R_PUBLIC_MATH_H

/* "libs/nw4r/include/nw4r/math.h" line 3 "nw4r/math/math_arithmetic.h" */
#ifndef NW4R_MATH_ARITHMETIC_H
#define NW4R_MATH_ARITHMETIC_H
/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 2 "nw4r/types_nw4r.h" */
#ifndef NW4R_TYPES_H
#define NW4R_TYPES_H
/* "libs/nw4r/include/nw4r/types_nw4r.h" line 2 "types.h" */
/* end "types.h" */

#define NW4R_BYTEORDER_BIG 0xFEFF
#define NW4R_BYTEORDER_LITTLE 0xFFFE

#if defined(NW4R_LITTLE_ENDIAN)
#define NW4R_BYTEORDER_NATIVE NW4R_BYTEORDER_LITTLE
#else
#define NW4R_BYTEORDER_NATIVE NW4R_BYTEORDER_BIG
#endif

#define NW4R_VERSION(major, minor) ((major & 0xFF) << 8 | minor & 0xFF)

#define NW4R_LIB_VERSION(NAME, ORIGINAL_DATE, ORIGINAL_TIME, ORIGINAL_CWCC)    \
    const char* NW4R_##NAME##_Version_ =                                       \
        "<< NW4R    - " #NAME " \tfinal   build: " ORIGINAL_DATE               \
        " " ORIGINAL_TIME " (" ORIGINAL_CWCC ") >>"

#endif
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 4 "nw4r/db/db_assert.h" */
#ifndef NW4R_DB_ASSERT_H
#define NW4R_DB_ASSERT_H

/* "libs/nw4r/include/nw4r/db/db_assert.h" line 3 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r
{
    namespace db
    {
        // Forward declarations
        namespace detail {
            class ConsoleHead;
        }

        #define NW4R_WARNING(...) nw4r::db::Warning(__FILE__, __LINE__, __VA_ARGS__)
        #define NW4R_PANIC(...) nw4r::db::Panic(__FILE__, __LINE__, __VA_ARGS__)
        #define NW4R_DB_ASSERT(exp, ...) \
            if(!(exp))                \
            nw4r::db::Panic(__FILE__, __LINE__, __VA_ARGS__)

        #define NW4R_DB_ASSERT_WARN(exp, ...) \
            if(!(exp))                \
            nw4r::db::Warning(__FILE__, __LINE__, __VA_ARGS__)

        /* DECL_WEAK */ void VPanic(const char* file, int line, const char* fmt, std::va_list vlist, bool halt);
        /* DECL_WEAK */ void Panic(const char* file, int line, const char* fmt, ...);
        /* DECL_WEAK */ void VWarning(const char* file, int line, const char* fmt, std::va_list vlist);
        /* DECL_WEAK */ void Warning(const char* file, int line, const char* msg, ...);
        
        namespace detail
        {
            void Log(const char* fmt, ...);
        }
        
        detail::ConsoleHead* Assertion_SetConsole(detail::ConsoleHead* console);
        detail::ConsoleHead* Assertion_GetConsole();
        void Assertion_ShowConsole(u32 time);
        void Assertion_HideConsole();
        void Assertion_SetWarningTime(u32 time);
        bool Assertion_SetAutoWarning(bool enable);
    }
}

#endif
/* end "nw4r/db/db_assert.h" */
/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 5 "nw4r/math/math_constant.h" */
#ifndef NW4R_MATH_CONSTANT_H
#define NW4R_MATH_CONSTANT_H
/* "libs/nw4r/include/nw4r/math/math_constant.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_constant.h" line 4 "cmath" */
#ifndef MSL_CPP_CMATH_H
#define MSL_CPP_CMATH_H
/* "libs/PowerPC_EABI_Support/include/stl/cmath" line 2 "math.h" */
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
#ifdef __cplusplus

namespace std {
using ::acos;
using ::acosf;
using ::asin;
using ::atan;
using ::atan2;
using ::ceil;
using ::ceilf;
using ::copysign;
using ::cos;
using ::cosf;
using ::fabsf;
using ::floor;
using ::floorf;
using ::fmod;
using ::fmodf;
using ::frexp;
using ::ldexp;
using ::modf;
using ::modff;
using ::nan;
using ::pow;
using ::scalbn;
using ::sin;
using ::sinf;
using ::sqrt;
using ::sqrtf;
using ::tan;
using ::tanf;
} // namespace std

#endif
#endif
/* end "cmath" */

/******************************************************************************
 *
 * Mathematical constants
 *
 ******************************************************************************/
// Pi mathematical constant
#define NW4R_MATH_PI M_PI

// ln(2)
#define NW4R_MATH_LN_2 0.69314718056f

// 1 / sqrt(3)
#define NW4R_MATH_INVSQRT3 0.577350258f

/******************************************************************************
 *
 * Not-a-number (NaN) types
 *
 ******************************************************************************/
// Quiet NaN (0x7FC00000)
#define NW4R_MATH_QNAN (-(0.0f / 0.0f))

/******************************************************************************
 *
 * Float limits
 *
 ******************************************************************************/
// Smallest finite floating-point value
#define NW4R_MATH_FLT_MIN 1.175494350e-38f

// Largest finite floating-point value
#define NW4R_MATH_FLT_MAX 3.402823466e+38f

// Difference between 1.0 and the next representable value
#define NW4R_MATH_FLT_EPSILON 1.192092895e-7f

#endif
/* end "nw4r/math/math_constant.h" */

/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 7 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
#ifndef RVL_SDK_OS_H
#define RVL_SDK_OS_H
/* "libs/RVL_SDK/include/revolution/OS/OS.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OS.h" line 4 "revolution/OS/OSContext.h" */
#ifndef RVL_SDK_OS_CONTEXT_H
#define RVL_SDK_OS_CONTEXT_H
/* "libs/RVL_SDK/include/revolution/OS/OSContext.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OS_CONTEXT_STATE_FP_SAVED = (1 << 0),
} OSContextState;

typedef struct OSContext {
    u32 gprs[32];  // at 0x0
    u32 cr;        // at 0x80
    u32 lr;        // at 0x84
    u32 ctr;       // at 0x88
    u32 xer;       // at 0x8C
    f64 fprs[32];  // at 0x90
    u32 fpscr_pad; // at 0x190
    u32 fpscr;     // at 0x194
    u32 srr0;      // at 0x198
    u32 srr1;      // at 0x19C
    u16 mode;      // at 0x1A0
    u16 state;     // at 0x1A2
    u32 gqrs[8];   // at 0x1A4
    u32 psf_pad;   // at 0x1C4
    f64 psfs[32];  // at 0x1C8
} OSContext;

void OSSaveFPUContext(OSContext* ctx);
void OSSetCurrentContext(OSContext* ctx);
OSContext* OSGetCurrentContext(void);
BOOL OSSaveContext(OSContext* ctx);
void OSLoadContext(OSContext* ctx);
void* OSGetStackPointer(void);
void OSSwitchFiber(void* func, void* stack);
void OSSwitchFiberEx(u32 r3, u32 r4, u32 r5, u32 r6, void* func, void* stack);
void OSClearContext(OSContext* ctx);
void OSInitContext(OSContext* ctx, void* _srr0, void* stack);
void OSDumpContext(const OSContext* ctx);
void __OSContextInit(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS/OS.h" line 5 "revolution/OS/OSExec.h" */
#ifndef RVL_SDK_OS_EXEC_H
#define RVL_SDK_OS_EXEC_H
/* "libs/RVL_SDK/include/revolution/OS/OSExec.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSExecParams {
    UNKWORD WORD_0x0;
    UNKWORD WORD_0x4;
    char UNK_0x8[0x4];
    void* regionStart; // at 0xC
    void* regionEnd;   // at 0x10
    char UNK_0x14[0x1C - 0x14];
} OSExecParams;

extern BOOL __OSInReboot;
extern void* __OSNextPartitionType;

void __OSGetExecParams(OSExecParams* out);
void __OSLaunchMenu(void);
void __OSBootDol(u32, u32, u32*);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSExec.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OS_CONSOLE_MASK = 0xF0000000,
    OS_CONSOLE_MASK_RVL = 0x00000000,
    OS_CONSOLE_MASK_EMU = 0x10000000,
    OS_CONSOLE_MASK_TDEV = 0x20000000,

    OS_CONSOLE_RVL_PP_1 = 0x00000011,
    OS_CONSOLE_RVL_PP_2_1 = 0x00000012,
    OS_CONSOLE_RVL_PP_2_2 = 0x00000020,
    OS_CONSOLE_RVA_1 = 0x00000100,

    OS_CONSOLE_RVL_EMU = 0x10000008,
    OS_CONSOLE_NDEV_1_0 = 0x10000010,
    OS_CONSOLE_NDEV_1_1 = 0x10000011,
    OS_CONSOLE_NDEV_1_2 = 0x10000012,
    OS_CONSOLE_NDEV_2_0 = 0x10000020,
    OS_CONSOLE_NDEV_2_1 = 0x10000021,
} OSConsoleType;

typedef enum {
    OS_APP_TYPE_IPL = 0x40,
    OS_APP_TYPE_DVD = 0x80,
    OS_APP_TYPE_CHANNEL = 0x81,
} OSAppType;

typedef enum {
    OS_EXC_SYSTEM_RESET,
    OS_EXC_MACHINE_CHECK,
    OS_EXC_DSI,
    OS_EXC_ISI,
    OS_EXC_EXT_INTERRUPT,
    OS_EXC_ALIGNMENT,
    OS_EXC_PROGRAM,
    OS_EXC_FP_UNAVAIL,
    OS_EXC_DECREMENTER,
    OS_EXC_SYSTEM_CALL,
    OS_EXC_TRACE,
    OS_EXC_PERF_MONITOR,
    OS_EXC_IABR,
    OS_EXC_SMI,
    OS_EXC_THERMAL_INT,

    OS_EXC_MAX
} OSExceptionType;

typedef struct OSIOSRev {
    u8 idHi;       // at 0x0
    u8 idLo;       // at 0x1
    u8 verMajor;   // at 0x2
    u8 verMinor;   // at 0x3
    u8 buildMon;   // at 0x4
    u8 buildDay;   // at 0x5
    u16 buildYear; // at 0x6
} OSIOSRev;

typedef void (*OSExceptionHandler)(u8 type, OSContext* ctx);

extern BOOL __OSInIPL;
extern BOOL __OSInNandBoot;
extern BOOL __OSIsGcam;
extern s64 __OSStartTime;
extern OSExecParams __OSRebootParams;

void __OSFPRInit(void);
u32 __OSGetHollywoodRev(void);
void __OSGetIOSRev(OSIOSRev* rev);
u32 OSGetConsoleType(void);
void OSInit(void);
OSExceptionHandler __OSSetExceptionHandler(u8 type, OSExceptionHandler handler);
OSExceptionHandler __OSGetExceptionHandler(u8 type);
void OSDefaultExceptionHandler(u8 type, OSContext* ctx);
void __OSPSInit(void);
u32 __OSGetDIConfig(void);
void OSRegisterVersion(const char* ver);
const char* OSGetAppGamename(void);
u8 OSGetAppType(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
#ifndef RVL_SDK_OS_ADDRESS_H
#define RVL_SDK_OS_ADDRESS_H
/* "libs/RVL_SDK/include/revolution/OS/OSAddress.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

static inline void* OSPhysicalToCached(u32 ofs) {
    return (void*)(ofs + 0x80000000);
}

static inline void* OSPhysicalToUncached(u32 ofs) {
    return (void*)(ofs + 0xC0000000);
}

static inline void* OSCachedToPhysical(const void* ofs) {
    return (u8*)ofs - 0x80000000;
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
#ifndef RVL_SDK_OS_ALARM_H
#define RVL_SDK_OS_ALARM_H
/* "libs/RVL_SDK/include/revolution/OS/OSAlarm.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSAlarm;
typedef struct OSContext;
typedef struct OSThread;

typedef void (*OSAlarmHandler)(struct OSAlarm* alarm, struct OSContext* ctx);

typedef struct OSAlarm {
    OSAlarmHandler handler; // at 0x0
    u32 tag;                // at 0x4
    s64 end;                // at 0x8
    struct OSAlarm* prev;   // at 0x10
    struct OSAlarm* next;   // at 0x14
    s64 period;             // at 0x18
    s64 start;              // at 0x20
    void* userData;         // at 0x28
} OSAlarm;

typedef struct OSAlarmQueue {
    OSAlarm* head; // at 0x0
    OSAlarm* tail; // at 0x4
} OSAlarmQueue;

void __OSInitAlarm(void);
void OSCreateAlarm(OSAlarm* alarm);
void OSSetAlarm(OSAlarm* alarm, s64 tick, OSAlarmHandler handler);
void OSSetPeriodicAlarm(OSAlarm* alarm, s64 tick, s64 period,
                        OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm* alarm);
void OSSetAlarmTag(OSAlarm* alarm, u32 tag);
void OSSetAlarmUserData(OSAlarm* alarm, void* userData);
void* OSGetAlarmUserData(const OSAlarm* alarm);
void __OSCancelInternalAlarms(struct OSThread* thread);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
#ifndef RVL_SDK_OS_ALLOC_H
#define RVL_SDK_OS_ALLOC_H
/* "libs/RVL_SDK/include/revolution/OS/OSAlloc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

extern volatile s32 __OSCurrHeap;

void* OSAllocFromHeap(s32 handle, s32 size);
void OSFreeToHeap(s32 handle, void* p);
s32 OSSetCurrentHeap(s32 handle);
void* OSInitAlloc(void* start, void* end, s32 numHeaps);
s32 OSCreateHeap(void* start, void* end);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
#ifndef RVL_SDK_OS_ARENA_H
#define RVL_SDK_OS_ARENA_H
/* "libs/RVL_SDK/include/revolution/OS/OSArena.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void* OSGetMEM1ArenaHi(void);
void* OSGetMEM2ArenaHi(void);
void* OSGetArenaHi(void);

void* OSGetMEM1ArenaLo(void);
void* OSGetMEM2ArenaLo(void);
void* OSGetArenaLo(void);

void OSSetMEM1ArenaHi(void* hi);
void OSSetMEM2ArenaHi(void* hi);
void OSSetArenaHi(void* hi);

void OSSetMEM1ArenaLo(void* lo);
void OSSetMEM2ArenaLo(void* lo);
void OSSetArenaLo(void* lo);

void* OSAllocFromMEM1ArenaLo(size_t size, u32 align);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
#ifndef RVL_SDK_OS_AUDIOSYSTEM_H
#define RVL_SDK_OS_AUDIOSYSTEM_H
/* "libs/RVL_SDK/include/revolution/OS/OSAudioSystem.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitAudioSystem(void);
void __OSStopAudioSystem(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
#ifndef RVL_SDK_OS_CACHE_H
#define RVL_SDK_OS_CACHE_H
/* "libs/RVL_SDK/include/revolution/OS/OSCache.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_CACHE_BASE 0xE0000000

// Forward declarations
typedef struct OSContext;

void DCEnable(void);
void DCInvalidateRange(const void* buf, u32 len);
void DCFlushRange(const void* buf, u32 len);
void DCStoreRange(const void* buf, u32 len);
void DCFlushRangeNoSync(const void* buf, u32 len);
void DCStoreRangeNoSync(const void* buf, u32 len);
void DCZeroRange(const void* buf, u32 len);

void ICInvalidateRange(const void* buf, u32 len);
void ICFlashInvalidate(void);
void ICEnable(void);

void LCEnable(void);
void LCDisable(void);
void LCLoadBlocks(void* dst, const void* src, u32 blocks);
void LCStoreBlocks(void* dst, const void* src, u32 blocks);
u32 LCStoreData(void* dst, const void* src, u32 len);
u32 LCQueueLength(void);
void LCQueueWait(u32 n);

void L2Enable(void);
void L2Disable(void);
void L2GlobalInvalidate(void);

void DMAErrorHandler(u8 error, OSContext* ctx, u32 dsisr, u32 dar, ...);

void __OSCacheInit(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
#ifndef RVL_SDK_OS_CRC_H
#define RVL_SDK_OS_CRC_H
/* "libs/RVL_SDK/include/revolution/OS/OSCrc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

u16 OSCalcCRC16(const u8* array, s32 length);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
#ifndef RVL_SDK_OS_ERROR_H
#define RVL_SDK_OS_ERROR_H
/* "libs/RVL_SDK/include/revolution/OS/OSError.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

#define OS_ERROR(...) OSPanic(__FILE__, __LINE__, __VA_ARGS__)

#define OS_ASSERT(exp, ...)                                                    \
    if (!(exp))                                                                \
    OSPanic(__FILE__, __LINE__, __VA_ARGS__)

typedef enum {
    OS_ERR_SYSTEM_RESET,
    OS_ERR_MACHINE_CHECK,
    OS_ERR_DSI,
    OS_ERR_ISI,
    OS_ERR_EXT_INTERRUPT,
    OS_ERR_ALIGNMENT,
    OS_ERR_PROGRAM,
    OS_ERR_FP_UNAVAIL,
    OS_ERR_DECREMENTER,
    OS_ERR_SYSTEM_CALL,
    OS_ERR_TRACE,
    OS_ERR_PERF_MONITOR,
    OS_ERR_IABR,
    OS_ERR_SMI,
    OS_ERR_THERMAL_INT,
    OS_ERR_PROTECTION,
    OS_ERR_FP_EXCEPTION,

    OS_ERR_MAX
} OSErrorType;

typedef void (*OSErrorHandler)(u8 error, struct OSContext* ctx, u32 dsisr, u32 dar,
                               ...);

extern OSErrorHandler __OSErrorTable[OS_ERR_MAX];
extern u32 __OSFpscrEnableBits;

/* DECL_WEAK */ void OSReport(const char* msg, ...);
/* DECL_WEAK */ void OSPanic(const char* file, int line, const char* msg, ...);
/* DECL_WEAK */ void OSVReport(const char* msg, va_list arg);

OSErrorHandler OSSetErrorHandler(u16 error, OSErrorHandler handler);
void __OSUnhandledException(u8 error, struct OSContext* ctx, u32 dsisr, u32 dar);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
#ifndef RVL_SDK_OS_FAST_CAST_H
#define RVL_SDK_OS_FAST_CAST_H
/* "libs/RVL_SDK/include/revolution/OS/OSFastCast.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_GQR_TYPE_U8 4
#define OS_GQR_TYPE_U16 5
#define OS_GQR_TYPE_S8 6
#define OS_GQR_TYPE_S16 7

static void OSInitFastCast(void) {
    ASM (
        li r3, 4
        oris r3, r3, 4
        mtgqr2 r3

        li r3, 5
        oris r3, r3, 5
        mtgqr3 r3

        li r3, 6
        oris r3, r3, 6
        mtgqr4 r3
        
        li r3, 7
        oris r3, r3, 7
        mtgqr5 r3
    )
}

static void OSSetGQR6(register u32 type, register u32 scale) {
    register u32 val = ((scale << 8 | type) << 16) | ((scale << 8) | type);

    ASM (
        mtspr 0x396, val
    )
}

static void OSSetGQR7(register u32 type, register u32 scale) {
    register u32 val = ((scale << 8 | type) << 16) | ((scale << 8) | type);

    ASM (
        mtspr 0x397, val
    )
}

/******************************************************************************
 *
 * Convert from U8
 *
 ******************************************************************************/
static f32 __OSu8tof32(register u8* in) {
    register f32 ret;

    ASM (
        psq_l ret, 0(in), 1, 2
    )

    return ret;
}

static void OSu8tof32(u8* in, volatile f32* out) {
    *out = __OSu8tof32(in);
}

/******************************************************************************
 *
 * Convert from U16
 *
 ******************************************************************************/
static f32 __OSu16tof32(register u16* arg) {
    register f32 ret;

    ASM (
        psq_l ret, 0(arg), 1, 3
    )

    return ret;
}

static void OSu16tof32(u16* in, volatile f32* out) {
    *out = __OSu16tof32(in);
}

/******************************************************************************
 *
 * Convert from S16
 *
 ******************************************************************************/
static f32 __OSs16tof32(register s16* arg) {
    register f32 ret;

    ASM (
        psq_l ret, 0(arg), 1, 5
    )

    return ret;
}

static void OSs16tof32(s16* in, volatile f32* out) {
    *out = __OSs16tof32(in);
}

/******************************************************************************
 *
 * Convert from F32
 *
 ******************************************************************************/
static u8 __OSf32tou8(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    u8 r;

    ASM (
        psq_st arg, 0(ptr), 1, 2
    )

    r = *(u8*)ptr;
    return r;
}

static void OSf32tou8(f32* in, volatile u8* out) {
    *out = __OSf32tou8(*in);
}

static u16 __OSf32tou16(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    u16 r;

    ASM (
        psq_st arg, 0(ptr), 1, 3
    )

    r = *(u16*)ptr;
    return r;
}

static void OSf32tou16(f32* in, volatile u16* out) {
    *out = __OSf32tou16(*in);
}

static s16 __OSf32tos16(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    s16 r;

    ASM (
        psq_st arg, 0(ptr), 1, 5
    )

    r = *(s16*)ptr;
    return r;
}

static void OSf32tos16(f32* in, volatile s16* out) {
    *out = __OSf32tos16(*in);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
#ifndef RVL_SDK_OS_FATAL_H
#define RVL_SDK_OS_FATAL_H
/* "libs/RVL_SDK/include/revolution/OS/OSFatal.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSFatal.h" line 4 "revolution/GX/GXTypes.h" */
#ifndef RVL_SDK_GX_TYPES_H
#define RVL_SDK_GX_TYPES_H
/* "libs/RVL_SDK/include/revolution/GX/GXTypes.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pack value into bitfield.
 * Value is shifted to the specified bit position.
 * (Bit indices are LSB)
 */
#define GX_BITSET(field, pos, size, value)                                     \
    __rlwimi((field), (value), 31 - (pos) - (size) + 1, (pos),                 \
             (pos) + (size) - 1)

/**
 * Compose value from bitfield.
 * Value is shifted after masking.
 * (Bit indices are LSB)
 */
#define GX_BITGET(field, pos, size)                                            \
    ((field) >> (31 - (pos) - (size) + 1) & ((1 << (size)) - 1))

/**
 * Pack value into bitfield.
 * Value is not shifted, only masked.
 * (Bit indices are LSB)
 */
#define GX_BITSET_TRUNC(field, pos, size, value)                               \
    __rlwimi((field), (value), 0, (pos), (pos) + (size) - 1)

/**
 * Compose value from bitfield.
 * Value is not shifted, only masked.
 * (Bit indices are LSB)
 */
#define GX_BITGET_TRUNC(field, pos, size)                                      \
    ((field) & (((1 << (size)) - 1) << (32 - (pos) - (size))))

/**
 * Common types used throughout many GX files.
 * To avoid circular depencies, common structures and all enums have been moved
 * here. Everything is organized in alphabetical order.
 */

// Differentiate between 8-byte and boolean values
// Real name! (from patent)
typedef unsigned char GXBool;

#define GX_TRUE ((GXBool)TRUE)
#define GX_FALSE ((GXBool)FALSE)

#define GX_ENABLE GX_TRUE
#define GX_DISABLE GX_FALSE

/**
 * Common structs
 */

typedef struct _GXColor {
    u8 r, g, b, a;
} GXColor;

typedef struct _GXColorS10 {
    s16 r, g, b, a;
} GXColorS10;

/**
 * Common enums
 */

typedef enum _GXAlphaOp {
    GX_AOP_AND,
    GX_AOP_OR,
    GX_AOP_XOR,
    GX_AOP_XNOR,

    GX_MAX_ALPHAOP
} GXAlphaOp;

typedef enum _GXAnisotropy {
    GX_ANISO_1,
    GX_ANISO_2,
    GX_ANISO_4,

    GX_MAX_ANISOTROPY
} GXAnisotropy;

typedef enum _GXAttnFn {
    GX_AF_SPEC,
    GX_AF_SPOT,
    GX_AF_NONE,
} GXAttnFn;

typedef enum _GXAttr {
    GX_VA_PNMTXIDX,   // Position/Normal matrix index
    GX_VA_TEX0MTXIDX, // GX_TEXCOORD0 matrix index
    GX_VA_TEX1MTXIDX, // GX_TEXCOORD1 matrix index
    GX_VA_TEX2MTXIDX, // GX_TEXCOORD2 matrix index
    GX_VA_TEX3MTXIDX, // GX_TEXCOORD3 matrix index
    GX_VA_TEX4MTXIDX, // GX_TEXCOORD4 matrix index
    GX_VA_TEX5MTXIDX, // GX_TEXCOORD5 matrix index
    GX_VA_TEX6MTXIDX, // GX_TEXCOORD6 matrix index
    GX_VA_TEX7MTXIDX, // GX_TEXCOORD7 matrix index

    GX_VA_POS, // Position
    GX_VA_NRM, // Normal

    GX_VA_CLR0, // Color 0
    GX_VA_CLR1, // Color 1

    GX_VA_TEX0, // Texture Coordinate 0
    GX_VA_TEX1, // Texture Coordinate 1
    GX_VA_TEX2, // Texture Coordinate 2
    GX_VA_TEX3, // Texture Coordinate 3
    GX_VA_TEX4, // Texture Coordinate 4
    GX_VA_TEX5, // Texture Coordinate 5
    GX_VA_TEX6, // Texture Coordinate 6
    GX_VA_TEX7, // Texture Coordinate 7

    GX_POS_MTX_ARRAY,
    GX_NRM_MTX_ARRAY,
    GX_TEX_MTX_ARRAY,
    GX_LIGHT_ARRAY,

    GX_VA_NBT, // All three normal elements (normal/binormal/tangent)

    GX_VA_MAX_ATTR,
    GX_VA_NULL = 255
} GXAttr;

typedef enum _GXAttrType {
    GX_NONE,   // No data sent
    GX_DIRECT, // Data sent directly to FIFO
    GX_INDEX8, // 8-bit index sent to FIFO
    GX_INDEX16 // 16-bit index sent to FIFO
} GXAttrType;

typedef enum _GXBlendFactor {
    GX_BL_ZERO,
    GX_BL_ONE,
    GX_BL_SRCCLR,
    GX_BL_INVSRCCLR,
    GX_BL_SRCALPHA,
    GX_BL_INVSRCALPHA,
    GX_BL_DSTALPHA,
    GX_BL_INVDSTALPHA,

    GX_BL_DSTCLR = GX_BL_SRCCLR,
    GX_BL_INVDSTCLR = GX_BL_INVSRCCLR
} GXBlendFactor;

typedef enum _GXBlendMode {
    GX_BM_NONE,
    GX_BM_BLEND,
    GX_BM_LOGIC,
    GX_BM_SUBTRACT,

    GX_MAX_BLENDMODE
} GXBlendMode;

typedef enum _GXChannelID {
    GX_COLOR0,
    GX_COLOR1,
    GX_ALPHA0,
    GX_ALPHA1,
    GX_COLOR0A0,
    GX_COLOR1A1,
    GX_COLOR_ZERO,
    GX_ALPHA_BUMP,
    GX_ALPHA_BUMPN,

    GX_COLOR_NULL = 255
} GXChannelID;

typedef enum _GXCITexFmt {
    GX_TF_C4 = 8,
    GX_TF_C8,
    GX_TF_C14X2,
} GXCITexFmt;

typedef enum _GXClearZ {
    GX_CLEAR_Z_MIN = 0,
    GX_CLEAR_Z_MAX = (1 << 24) - 1,
} GXClearZ;

typedef enum _GXClipMode {
    // "ClipDisable" in XF mem, so 0 = enable
    GX_CLIP_ENABLE,
    GX_CLIP_DISABLE,
} GXClipMode;

typedef enum _GXColorSrc { GX_SRC_REG, GX_SRC_VTX } GXColorSrc;

typedef enum _GXCompare {
    GX_NEVER,
    GX_LESS,
    GX_EQUAL,
    GX_LEQUAL,
    GX_GREATER,
    GX_NEQUAL,
    GX_GEQUAL,
    GX_ALWAYS
} GXCompare;

typedef enum _GXCompCnt {
    GX_POS_XY = 0,
    GX_POS_XYZ,

    GX_NRM_XYZ = 0,
    GX_NRM_NBT,
    GX_NRM_NBT3,

    GX_CLR_RGB = 0,
    GX_CLR_RGBA,

    GX_TEX_S = 0,
    GX_TEX_ST
} GXCompCnt;

typedef enum _GXCompType {
    GX_U8,
    GX_S8,
    GX_U16,
    GX_S16,
    GX_F32,

    GX_RGB565 = 0,
    GX_RGB8,
    GX_RGBX8,
    GX_RGBA4,
    GX_RGBA6,
    GX_RGBA8
} GXCompType;

typedef enum _GXCopyClamp {
    GX_CLAMP_NONE,
    GX_CLAMP_TOP,
    GX_CLAMP_BOTTOM,
    GX_CLAMP_ALL,
} GXCopyClamp;

typedef enum _GXCullMode {
    GX_CULL_NONE,
    GX_CULL_FRONT,
    GX_CULL_BACK,
    GX_CULL_ALL
} GXCullMode;

typedef enum _GXDiffuseFn { GX_DF_NONE, GX_DF_SIGN, GX_DF_CLAMP } GXDiffuseFn;

typedef enum _GXDirtyFlag {
    GX_DIRTY_SU_TEX = (1 << 0),
    GX_DIRTY_BP_MASK = (1 << 1),
    GX_DIRTY_GEN_MODE = (1 << 2),
    GX_DIRTY_VCD = (1 << 3),
    GX_DIRTY_VAT = (1 << 4),
    // . . .
    GX_DIRTY_AMB_COLOR0 = (1 << 8),
    GX_DIRTY_AMB_COLOR1 = (1 << 9),
    GX_DIRTY_MAT_COLOR0 = (1 << 10),
    GX_DIRTY_MAT_COLOR1 = (1 << 11),
    GX_DIRTY_CHAN_COLOR0 = (1 << 12),
    GX_DIRTY_CHAN_COLOR1 = (1 << 13),
    GX_DIRTY_CHAN_ALPHA0 = (1 << 14),
    GX_DIRTY_CHAN_ALPHA1 = (1 << 15),
    GX_DIRTY_TEX0 = (1 << 16),
    GX_DIRTY_TEX1 = (1 << 17),
    GX_DIRTY_TEX2 = (1 << 18),
    GX_DIRTY_TEX3 = (1 << 19),
    GX_DIRTY_TEX4 = (1 << 20),
    GX_DIRTY_TEX5 = (1 << 21),
    GX_DIRTY_TEX6 = (1 << 22),
    GX_DIRTY_TEX7 = (1 << 23),
    GX_DIRTY_NUM_COLORS = (1 << 24),
    GX_DIRTY_NUM_TEX = (1 << 25),
    GX_DIRTY_MTX_IDX = (1 << 26),
    GX_DIRTY_PROJECTION = (1 << 27),
    GX_DIRTY_VIEWPORT = (1 << 28),

    GX_AMB_MAT_MASK = GX_DIRTY_AMB_COLOR0 | GX_DIRTY_AMB_COLOR1 |
                      GX_DIRTY_MAT_COLOR0 | GX_DIRTY_MAT_COLOR1,

    GX_LIGHT_CHAN_MASK = GX_DIRTY_CHAN_COLOR0 | GX_DIRTY_CHAN_COLOR1 |
                         GX_DIRTY_CHAN_ALPHA0 | GX_DIRTY_CHAN_ALPHA1 |
                         GX_DIRTY_NUM_COLORS,

    GX_TEX_GEN_MASK = 0x2FF0000,
} GXDirtyFlag;

typedef enum _GXDistAttnFn {
    GX_DA_OFF,
    GX_DA_GENTLE,
    GX_DA_MEDIUM,
    GX_DA_STEEP
} GXDistAttnFn;

typedef enum _GXFogType {
    GX_FOG_NONE,

    GX_FOG_PERSP_LIN = 2,
    GX_FOG_PERSP_EXP = 4,
    GX_FOG_PERSP_EXP2 = 5,
    GX_FOG_PERSP_REVEXP = 6,
    GX_FOG_PERSP_REVEXP2 = 7,

    // Fourth bit is set to mark orthographic
    GX_FOG_ORTHO_LIN = 1 << 3 | GX_FOG_PERSP_LIN,
    GX_FOG_ORTHO_EXP = 1 << 3 | GX_FOG_PERSP_EXP,
    GX_FOG_ORTHO_EXP2 = 1 << 3 | GX_FOG_PERSP_EXP2,
    GX_FOG_ORTHO_REVEXP = 1 << 3 | GX_FOG_PERSP_REVEXP,
    GX_FOG_ORTHO_REVEXP2 = 1 << 3 | GX_FOG_PERSP_REVEXP2
} GXFogType;

// Access components of the fog type
#define GX_FOG_GET_PROJ(x) ((x) >> 3 & 1)
#define GX_FOG_GET_FSEL(x) ((x) & 7)

typedef enum _GXIndTexAlphaSel {
    GX_ITBA_OFF,
    GX_ITBA_S,
    GX_ITBA_T,
    GX_ITBA_U,

    GX_MAX_ITBALPHA
} GXIndTexAlphaSel;

typedef enum _GXIndTexBiasSel {
    GX_ITB_NONE,
    GX_ITB_S,
    GX_ITB_T,
    GX_ITB_ST,
    GX_ITB_U,
    GX_ITB_SU,
    GX_ITB_TU,
    GX_ITB_STU,

    GX_MAX_ITBIAS
} GXIndTexBiasSel;

typedef enum _GXIndTexFormat {
    GX_ITF_8,
    GX_ITF_5,
    GX_ITF_4,
    GX_ITF_3,

    GX_MAX_ITFORMAT
} GXIndTexFormat;

typedef enum _GXIndTexMtxID {
    GX_ITM_OFF,
    GX_ITM_0,
    GX_ITM_1,
    GX_ITM_2,

    GX_ITM_S0 = 5,
    GX_ITM_S1,
    GX_ITM_S2,

    GX_ITM_T0 = 9,
    GX_ITM_T1,
    GX_ITM_T2,
} GXIndTexMtxID;

typedef enum _GXIndTexScale {
    GX_ITS_1,
    GX_ITS_2,
    GX_ITS_4,
    GX_ITS_8,
    GX_ITS_16,
    GX_ITS_32,
    GX_ITS_64,
    GX_ITS_128,
    GX_ITS_256,

    GX_MAX_ITSCALE
} GXIndTexScale;

typedef enum _GXIndTexStageID {
    GX_INDTEXSTAGE0,
    GX_INDTEXSTAGE1,
    GX_INDTEXSTAGE2,
    GX_INDTEXSTAGE3,

    GX_MAX_INDTEXSTAGE
} GXIndTexStageID;

typedef enum _GXIndTexWrap {
    GX_ITW_OFF,
    GX_ITW_256,
    GX_ITW_128,
    GX_ITW_64,
    GX_ITW_32,
    GX_ITW_16,
    GX_ITW_0,

    GX_MAX_ITWRAP,
} GXIndTexWrap;

typedef enum _GXLightID {
    GX_LIGHT0 = (1 << 0),
    GX_LIGHT1 = (1 << 1),
    GX_LIGHT2 = (1 << 2),
    GX_LIGHT3 = (1 << 3),
    GX_LIGHT4 = (1 << 4),
    GX_LIGHT5 = (1 << 5),
    GX_LIGHT6 = (1 << 6),
    GX_LIGHT7 = (1 << 7),

    GX_MAX_LIGHT = (1 << 8),
    GX_LIGHT_NULL = 0
} GXLightID;

typedef enum _GXLogicOp {
    GX_LO_CLEAR,
    GX_LO_AND,
    GX_LO_REVAND,
    GX_LO_COPY,
    GX_LO_INVAND,
    GX_LO_NOOP,
    GX_LO_XOR,
    GX_LO_OR,
    GX_LO_NOR,
    GX_LO_EQUIV,
    GX_LO_INV,
    GX_LO_REVOR,
    GX_LO_INVCOPY,
    GX_LO_INVOR,
    GX_LO_NAND,
    GX_LO_SET
} GXLogicOp;

typedef enum _GXMtxType {
    GX_MTX_3x4,
    GX_MTX_2x4,
} GXMtxType;

typedef enum _GXPixelFmt {
    GX_PF_RGB8_Z24,
    GX_PF_RGBA6_Z24,
    GX_PF_RGBA565_Z16,
    GX_PF_Z24,
    GX_PF_Y8,
    GX_PF_U8,
    GX_PF_V8,
    GX_PF_YUV420,

    GX_MAX_PIXELFMT
} GXPixelFmt;

/**
 * Matrix column index into XF memory.
 * (Multiply by row dimension to get XF mem offset)
 */
typedef enum _GXPosNrmMtx {
    GX_PNMTX0 = 0,
    GX_PNMTX1 = 3,
    GX_PNMTX2 = 6,
    GX_PNMTX3 = 9,
    GX_PNMTX4 = 12,
    GX_PNMTX5 = 15,
    GX_PNMTX6 = 18,
    GX_PNMTX7 = 21,
    GX_PNMTX8 = 24,
    GX_PNMTX9 = 27
} GXPosNrmMtx;

typedef enum _GXPrimitive {
    GX_POINTS = 0xB8,
    GX_LINES = 0xA8,
    GX_LINESTRIP = 0xB0,
    GX_TRIANGLES = 0x90,
    GX_TRIANGLESTRIP = 0x98,
    GX_TRIANGLEFAN = 0xA0,
    GX_QUADS = 0x80,
} GXPrimitive;

typedef enum _GXProjectionType {
    GX_PERSPECTIVE,
    GX_ORTHOGRAPHIC
} GXProjectionType;

typedef enum _GXSpotFn {
    GX_SP_OFF,
    GX_SP_FLAT,
    GX_SP_COS,
    GX_SP_COS2,
    GX_SP_SHARP,
    GX_SP_RING1,
    GX_SP_RING2
} GXSpotFn;

typedef enum _GXTevAlphaArg {
    GX_CA_APREV,
    GX_CA_A0,
    GX_CA_A1,
    GX_CA_A2,
    GX_CA_TEXA,
    GX_CA_RASA,
    GX_CA_KONST,
    GX_CA_ZERO,
    GX_CA_ONE
} GXTevAlphaArg;

typedef enum _GXTevBias {
    GX_TB_ZERO,
    GX_TB_ADDHALF,
    GX_TB_SUBHALF,

    GX_MAX_TEVBIAS
} GXTevBias;

typedef enum _GXTevColorArg {
    GX_CC_CPREV,
    GX_CC_APREV,
    GX_CC_C0,
    GX_CC_A0,
    GX_CC_C1,
    GX_CC_A1,
    GX_CC_C2,
    GX_CC_A2,
    GX_CC_TEXC,
    GX_CC_TEXA,
    GX_CC_RASC,
    GX_CC_RASA,
    GX_CC_ONE,
    GX_CC_HALF,
    GX_CC_KONST,
    GX_CC_ZERO,
    GX_CC_TEXRRR,
    GX_CC_TEXGGG,
    GX_CC_TEXBBB,

    GX_CC_QUARTER = GX_CC_KONST
} GXTevColorArg;

typedef enum _GXTevColorChan {
    GX_CH_RED,
    GX_CH_GREEN,
    GX_CH_BLUE,
    GX_CH_ALPHA
} GXTevColorChan;

typedef enum _GXTevOp {
    GX_TEV_ADD,
    GX_TEV_SUB,

    GX_TEV_COMP_R8_GT = 8,
    GX_TEV_COMP_R8_EQ,
    GX_TEV_COMP_GR16_GT,
    GX_TEV_COMP_GR16_EQ,
    GX_TEV_COMP_BGR24_GT,
    GX_TEV_COMP_BGR24_EQ,
    GX_TEV_COMP_RGB8_GT,
    GX_TEV_COMP_RGB8_EQ,

    GX_TEV_COMP_A8_GT = GX_TEV_COMP_RGB8_GT,
    GX_TEV_COMP_A8_EQ = GX_TEV_COMP_RGB8_EQ
} GXTevOp;

typedef enum _GXTevRegID {
    GX_TEVPREV,
    GX_TEVREG0,
    GX_TEVREG1,
    GX_TEVREG2,

    GX_MAX_TEVREG
} GXTevRegID;

typedef enum _GXTevScale {
    GX_CS_SCALE_1,
    GX_CS_SCALE_2,
    GX_CS_SCALE_4,
    GX_CS_DIVIDE_2,

    GX_MAX_TEVSCALE
} GXTevScale;

typedef enum _GXTevStageID {
    GX_TEVSTAGE0,
    GX_TEVSTAGE1,
    GX_TEVSTAGE2,
    GX_TEVSTAGE3,
    GX_TEVSTAGE4,
    GX_TEVSTAGE5,
    GX_TEVSTAGE6,
    GX_TEVSTAGE7,
    GX_TEVSTAGE8,
    GX_TEVSTAGE9,
    GX_TEVSTAGE10,
    GX_TEVSTAGE11,
    GX_TEVSTAGE12,
    GX_TEVSTAGE13,
    GX_TEVSTAGE14,
    GX_TEVSTAGE15,

    GX_MAX_TEVSTAGE
} GXTevStageID;

typedef enum _GXTevSwapSel {
    GX_TEV_SWAP0,
    GX_TEV_SWAP1,
    GX_TEV_SWAP2,
    GX_TEV_SWAP3,

    GX_MAX_TEVSWAP
} GXTevSwapSel;

typedef enum _GXTevKAlphaSel {
    GX_TEV_KASEL_8_8,
    GX_TEV_KASEL_7_8,
    GX_TEV_KASEL_6_8,
    GX_TEV_KASEL_5_8,
    GX_TEV_KASEL_4_8,
    GX_TEV_KASEL_3_8,
    GX_TEV_KASEL_2_8,
    GX_TEV_KASEL_1_8,

    GX_TEV_KASEL_1 = 0,
    GX_TEV_KASEL_3_4 = 2,
    GX_TEV_KASEL_1_2 = 4,
    GX_TEV_KASEL_1_4 = 6,

    GX_TEV_KASEL_K0_R = 16,
    GX_TEV_KASEL_K1_R,
    GX_TEV_KASEL_K2_R,
    GX_TEV_KASEL_K3_R,
    GX_TEV_KASEL_K0_G,
    GX_TEV_KASEL_K1_G,
    GX_TEV_KASEL_K2_G,
    GX_TEV_KASEL_K3_G,
    GX_TEV_KASEL_K0_B,
    GX_TEV_KASEL_K1_B,
    GX_TEV_KASEL_K2_B,
    GX_TEV_KASEL_K3_B,
    GX_TEV_KASEL_K0_A,
    GX_TEV_KASEL_K1_A,
    GX_TEV_KASEL_K2_A,
    GX_TEV_KASEL_K3_A
} GXTevKAlphaSel;

typedef enum _GXTevKColorID {
    GX_KCOLOR0,
    GX_KCOLOR1,
    GX_KCOLOR2,
    GX_KCOLOR3,

    GX_MAX_KCOLOR
} GXTevKColorID;

typedef enum _GXTevKColorSel {
    GX_TEV_KCSEL_8_8,
    GX_TEV_KCSEL_7_8,
    GX_TEV_KCSEL_6_8,
    GX_TEV_KCSEL_5_8,
    GX_TEV_KCSEL_4_8,
    GX_TEV_KCSEL_3_8,
    GX_TEV_KCSEL_2_8,
    GX_TEV_KCSEL_1_8,

    GX_TEV_KCSEL_1 = 0,
    GX_TEV_KCSEL_3_4 = 2,
    GX_TEV_KCSEL_1_2 = 4,
    GX_TEV_KCSEL_1_4 = 6,

    GX_TEV_KCSEL_K0 = 12,
    GX_TEV_KCSEL_K1,
    GX_TEV_KCSEL_K2,
    GX_TEV_KCSEL_K3,
    GX_TEV_KCSEL_K0_R,
    GX_TEV_KCSEL_K1_R,
    GX_TEV_KCSEL_K2_R,
    GX_TEV_KCSEL_K3_R,
    GX_TEV_KCSEL_K0_G,
    GX_TEV_KCSEL_K1_G,
    GX_TEV_KCSEL_K2_G,
    GX_TEV_KCSEL_K3_G,
    GX_TEV_KCSEL_K0_B,
    GX_TEV_KCSEL_K1_B,
    GX_TEV_KCSEL_K2_B,
    GX_TEV_KCSEL_K3_B,
    GX_TEV_KCSEL_K0_A,
    GX_TEV_KCSEL_K1_A,
    GX_TEV_KCSEL_K2_A,
    GX_TEV_KCSEL_K3_A
} GXTevKColorSel;

typedef enum _GXTevMode {
    GX_MODULATE,
    GX_DECAL,
    GX_BLEND,
    GX_REPLACE,
    GX_PASSCLR
} GXTevMode;

typedef enum _GXTexCoordID {
    GX_TEXCOORD0,
    GX_TEXCOORD1,
    GX_TEXCOORD2,
    GX_TEXCOORD3,
    GX_TEXCOORD4,
    GX_TEXCOORD5,
    GX_TEXCOORD6,
    GX_TEXCOORD7,

    GX_MAX_TEXCOORD,
    GX_TEXCOORD_NULL = 255
} GXTexCoordID;

typedef enum _GXTexFilter {
    GX_NEAR,
    GX_LINEAR,
    GX_NEAR_MIP_NEAR,
    GX_LIN_MIP_NEAR,
    GX_NEAR_MIP_LIN,
    GX_LIN_MIP_LIN,
} GXTexFilter;

typedef enum _GXTexFmt {
    GX_TF_I4,
    GX_TF_I8,
    GX_TF_IA4,
    GX_TF_IA8,
    GX_TF_RGB565,
    GX_TF_RGB5A3,
    GX_TF_RGBA8,
    GX_TF_CMPR = 14,

    GX_CTF_R4 = 32,
    GX_CTF_RA4 = 34,
    GX_CTF_RA8 = 35,
    GX_CTF_YUVA8 = 38,
    GX_CTF_A8 = 39,
    GX_CTF_R8 = 40,
    GX_CTF_G8 = 41,
    GX_CTF_B8 = 42,
    GX_CTF_RG8 = 43,
    GX_CTF_GB8 = 44,

    GX_TF_Z8 = 17,
    GX_TF_Z16 = 19,
    GX_TF_Z24X8 = 22,

    GX_CTF_Z4 = 48,
    GX_CTF_Z8M = 57,
    GX_CTF_Z8L = 58,
    GX_CTF_Z16L = 60,

    GX_TF_A8 = GX_CTF_YUVA8
} GXTexFmt;

typedef enum _GXTexGenSrc {
    GX_TG_POS,
    GX_TG_NRM,
    GX_TG_BINRM,
    GX_TG_TANGENT,
    GX_TG_TEX0,
    GX_TG_TEX1,
    GX_TG_TEX2,
    GX_TG_TEX3,
    GX_TG_TEX4,
    GX_TG_TEX5,
    GX_TG_TEX6,
    GX_TG_TEX7,
    GX_TG_TEXCOORD0,
    GX_TG_TEXCOORD1,
    GX_TG_TEXCOORD2,
    GX_TG_TEXCOORD3,
    GX_TG_TEXCOORD4,
    GX_TG_TEXCOORD5,
    GX_TG_TEXCOORD6,
    GX_TG_COLOR0,
    GX_TG_COLOR1,
} GXTexGenSrc;

typedef enum _GXTexGenType {
    GX_TG_MTX3x4,
    GX_TG_MTX2x4,
    GX_TG_BUMP0,
    GX_TG_BUMP1,
    GX_TG_BUMP2,
    GX_TG_BUMP3,
    GX_TG_BUMP4,
    GX_TG_BUMP5,
    GX_TG_BUMP6,
    GX_TG_BUMP7,
    GX_TG_SRTG
} GXTexGenType;

typedef enum _GXTexMapID {
    GX_TEXMAP0,
    GX_TEXMAP1,
    GX_TEXMAP2,
    GX_TEXMAP3,
    GX_TEXMAP4,
    GX_TEXMAP5,
    GX_TEXMAP6,
    GX_TEXMAP7,
    GX_MAX_TEXMAP,

    GX_TEXMAP_NULL = 255,
    GX_TEX_DISABLE
} GXTexMapID;

typedef enum _GXTexMtx {
    // Any dimension (in standard XF matrix memory)
    // Enum represents base row of matrix
    GX_TEXMTX0 = 30,
    GX_TEXMTX1 = 33,
    GX_TEXMTX2 = 36,
    GX_TEXMTX3 = 39,
    GX_TEXMTX4 = 42,
    GX_TEXMTX5 = 45,
    GX_TEXMTX6 = 48,
    GX_TEXMTX7 = 51,
    GX_TEXMTX8 = 54,
    GX_TEXMTX9 = 57,
    GX_IDENTITY = 60,
} GXTexMtx;

typedef enum _GXPTTexMtx {
    // 3x4 matrices (in dual-tex / "post-matrix" XF matrix memory)
    // Enum represents base row of matrix
    GX_PTTEXMTX0 = 64,
    GX_PTTEXMTX1 = 67,
    GX_PTTEXMTX2 = 70,
    GX_PTTEXMTX3 = 73,
    GX_PTTEXMTX4 = 76,
    GX_PTTEXMTX5 = 79,
    GX_PTTEXMTX6 = 82,
    GX_PTTEXMTX7 = 85,
    GX_PTTEXMTX8 = 88,
    GX_PTTEXMTX9 = 91,
    GX_PTTEXMTX10 = 94,
    GX_PTTEXMTX11 = 97,
    GX_PTTEXMTX12 = 100,
    GX_PTTEXMTX13 = 103,
    GX_PTTEXMTX14 = 106,
    GX_PTTEXMTX15 = 109,
    GX_PTTEXMTX16 = 112,
    GX_PTTEXMTX17 = 115,
    GX_PTTEXMTX18 = 118,
    GX_PTTEXMTX19 = 121,
    GX_PTIDENTITY = 125
} GXPTTexMtx;

typedef enum _GXTexWrapMode {
    GX_CLAMP,
    GX_REPEAT,
    GX_MIRROR,

    GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

typedef enum _GXTlut {
    GX_TLUT0,
    GX_TLUT1,
    GX_TLUT2,
    GX_TLUT3,
    GX_TLUT4,
    GX_TLUT5,
    GX_TLUT6,
    GX_TLUT7,
    GX_TLUT8,
    GX_TLUT9,
    GX_TLUT10,
    GX_TLUT11,
    GX_TLUT12,
    GX_TLUT13,
    GX_TLUT14,
    GX_TLUT15,

    GX_BIGTLUT0,
    GX_BIGTLUT1,
    GX_BIGTLUT2,
    GX_BIGTLUT3,
} GXTlut;

typedef enum _GXTlutFmt {
    GX_TL_IA8,
    GX_TL_RGB565,
    GX_TL_RGB5A3,

    GX_MAX_TLUTFMT
} GXTlutFmt;

typedef enum _GXVtxFmt {
    GX_VTXFMT0,
    GX_VTXFMT1,
    GX_VTXFMT2,
    GX_VTXFMT3,
    GX_VTXFMT4,
    GX_VTXFMT5,
    GX_VTXFMT6,
    GX_VTXFMT7,

    GX_MAX_VTXFMT
} GXVtxFmt;

typedef enum _GXZFmt16 {
    GX_ZC_LINEAR,
    GX_ZC_NEAR,
    GX_ZC_MID,
    GX_ZC_FAR,
} GXZFmt16;

// From patent
typedef enum _GXZTexOp {
    GX_ZT_DISABLE,
    GX_ZT_ADD,
    GX_ZT_REPLACE,

    GX_MAX_ZTEXOP
} GXZTexOp;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void OSFatal(GXColor textColor, GXColor bgColor, const char* msg);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
#ifndef RVL_SDK_OS_FONT_H
#define RVL_SDK_OS_FONT_H
/* "libs/RVL_SDK/include/revolution/OS/OSFont.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OS_FONT_ENCODE_ANSI,
    OS_FONT_ENCODE_SJIS,
    OS_FONT_ENCODE_2,
    OS_FONT_ENCODE_UTF8,
    OS_FONT_ENCODE_UTF16,
    OS_FONT_ENCODE_UTF32,
    OS_FONT_ENCODE_MAX
} OSFontEncode;

typedef struct OSFontHeader {
    u16 type;          // at 0x0
    u16 firstChar;     // at 0x2
    u16 lastChar;      // at 0x4
    u16 invalidChar;   // at 0x6
    u16 ascent;        // at 0x8
    u16 descent;       // at 0xA
    u16 width;         // at 0xC
    u16 leading;       // at 0xE
    u16 cellWidth;     // at 0x10
    u16 cellHeight;    // at 0x12
    u32 sheetSize;     // at 0x14
    u16 sheetFormat;   // at 0x18
    u16 sheetNumCol;   // at 0x1A
    u16 sheetNumRow;   // at 0x1C
    u16 sheetWidth;    // at 0x1E
    u16 sheetHeight;   // at 0x20
    u16 widthTableOfs; // at 0x22
    u32 sheetImageOfs; // at 0x24
    u32 sheetFullSize; // at 0x28
    u8 c0;             // at 0x2C
    u8 c1;             // at 0x2D
    u8 c2;             // at 0x2E
    u8 c3;             // at 0x2F
} OSFontHeader;

u16 OSGetFontEncode(void);
u16 OSSetFontEncode(u16 encode);
u32 OSLoadFont(OSFontHeader* font, void* dst);
const char* OSGetFontTexel(const char* str, void* dst, s32 xOfs, s32 arg3,
                           u32* widthOut);
BOOL OSInitFont(OSFontHeader* font);
const char* OSGetFontTexture(const char* str, void** texOut, u32* xOut,
                             u32* yOut, u32* widthOut);
const char* OSGetFontWidth(const char* str, u32* widthOut);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
#ifndef RVL_SDK_DVD_H
#define RVL_SDK_DVD_H
/* "libs/RVL_SDK/include/revolution/DVD/dvd.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// OS sets MSB to signal that the device code was successfully read
#define DVD_DEVICE_CODE_READ (1 << 15)
#define MAKE_DVD_DEVICE_CODE(x) (DVD_DEVICE_CODE_READ | (x))

#define H3_HASH_TABLE_SIZE 0x18000

// Forward declarations
typedef struct DVDCommandBlock;
typedef struct DVDFileInfo;
typedef struct OSAlarm;

typedef enum {
    DVD_RESULT_COVER_CLOSED = -4,
    DVD_RESULT_CANCELED,
    DVD_RESULT_IGNORED,
    DVD_RESULT_FATAL_ERROR,
    DVD_RESULT_GOOD,
} DVDResult;

typedef enum {
    DVD_STATE_FATAL = -1,
    DVD_STATE_IDLE,
    DVD_STATE_BUSY,
    DVD_STATE_WAITING,
    DVD_STATE_COVER_CLOSED,
    DVD_STATE_NO_DISK,
    DVD_STATE_COVER_OPENED,
    DVD_STATE_WRONG_DISK_ID,
    DVD_STATE_7,
    DVD_STATE_PAUSED,
    DVD_STATE_9,
    DVD_STATE_CANCELED,
    DVD_STATE_DISK_ERROR,
    DVD_STATE_MOTOR_STOPPED,
} DVDAsyncState;

typedef enum {
    DVD_COVER_BUSY,
    DVD_COVER_OPENED,
    DVD_COVER_CLOSED,
} DVDCoverState;

typedef void (*DVDAsyncCallback)(s32 result, struct DVDFileInfo* info);
typedef void (*DVDCommandCallback)(s32 result, struct DVDCommandBlock* block);

typedef struct DVDDiskID {
    char game[4];    // at 0x0
    char company[2]; // at 0x4
    u8 disk;         // at 0x6
    u8 version;      // at 0x7
    u8 strmEnable;   // at 0x8
    u8 strmBufSize;  // at 0x9
    u8 padding[14];  // at 0xA
    u32 rvlMagic;    // at 0x18
    u32 gcMagic;     // at 0x1C
} DVDDiskID;

typedef struct DVDCommandBlock {
    struct DVDCommandBlock* next;       // at 0x0
    struct DVDCommandBlock* prev;       // at 0x4
    u32 command;                 // at 0x8
    volatile s32 state;          // at 0xC
    u32 offset;                  // at 0x10
    u32 length;                  // at 0x14
    void* addr;                  // at 0x18
    u32 transferSize;            // at 0x1C
    u32 transferTotal;           // at 0x20
    DVDDiskID* id;               // at 0x24
    DVDCommandCallback callback; // at 0x28
    void* userData;              // at 0x2C
} DVDCommandBlock;

typedef struct DVDDriveInfo {
    u16 revision;    // at 0x0
    u16 deviceCode;  // at 0x2
    u32 releaseDate; // at 0x4
    char padding[32 - 0x8];
} DVDDriveInfo;

typedef struct DVDFileInfo {
    DVDCommandBlock block;     // at 0x0
    u32 offset;                // at 0x30
    u32 size;                  // at 0x34
    DVDAsyncCallback callback; // at 0x38
} DVDFileInfo;

typedef struct DVDDir{
    u32 entryNum; // at 0x0
    u32 location; // at 0x4
    u32 next;     // at 0x8
} DVDDir;

typedef struct DVDDirEntry{
    u32 entryNum; // at 0x0
    BOOL isDir;   // at 0x4
    char* name;   // at 0x8
} DVDDirEntry;

extern volatile u32 __DVDLayoutFormat;

void DVDInit(void);
BOOL DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* dst, u32 size,
                         u32 offset, DVDCommandCallback callback, s32 prio);
BOOL DVDInquiryAsync(DVDCommandBlock* block, DVDDriveInfo* info,
                     DVDCommandCallback callback);
s32 DVDGetCommandBlockStatus(const DVDCommandBlock* block);
s32 DVDGetDriveStatus(void);
void DVDPause(void);
void DVDResume(void);
BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCommandCallback callback);
s32 DVDCancel(DVDCommandBlock* block);
BOOL DVDCancelAllAsync(DVDCommandCallback callback);
const DVDDiskID* DVDGetCurrentDiskID(void);
u32 __DVDGetCoverStatus(void);
void __DVDPrepareResetAsync(DVDCommandCallback callback);
void __DVDPrepareReset(void);
BOOL __DVDTestAlarm(const struct OSAlarm* alarm);
BOOL __DVDLowBreak(void);
BOOL __DVDStopMotorAsync(DVDCommandBlock* block, DVDCommandCallback callback);
void __DVDRestartMotor(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
#ifndef RVL_SDK_OS_THREAD_H
#define RVL_SDK_OS_THREAD_H
/* "libs/RVL_SDK/include/revolution/OS/OSThread.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSThread.h" line 4 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_PRIORITY_MIN 0
#define OS_PRIORITY_MAX 31

#define OS_THREAD_STACK_MAGIC 0xDEADBABE

typedef enum {
    OS_THREAD_STATE_EXITED = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_RUNNING = 2,
    OS_THREAD_STATE_SLEEPING = 4,
    OS_THREAD_STATE_MORIBUND = 8
} OSThreadState;

typedef enum { OS_THREAD_DETACHED = (1 << 0) } OSThreadFlag;

typedef struct OSThreadQueue {
    struct OSThread* head; // at 0x0
    struct OSThread* tail; // at 0x4
} OSThreadQueue;

typedef struct OSMutexQueue {
    struct OSMutex* head; // at 0x0
    struct OSMutex* tail; // at 0x4
} OSMutexQueue;

typedef struct OSThread {
    OSContext context;
    u16 state;                   // at 0x2C8
    u16 flags;                   // at 0x2CA
    s32 suspend;                 // at 0x2CC
    s32 priority;                // at 0x2D0
    s32 base;                    // at 0x2D4
    u32 val;                     // at 0x2D8
    OSThreadQueue* queue;        // at 0x2DC
    struct OSThread* next;       // at 0x2E0
    struct OSThread* prev;       // at 0x2E4
    OSThreadQueue joinQueue;     // at 0x2E8
    struct OSMutex* mutex;       // at 0x2F0
    OSMutexQueue mutexQueue;     // at 0x2F4
    struct OSThread* nextActive; // at 0x2FC
    struct OSThread* prevActive; // at 0x300
    u32* stackBegin;             // at 0x304
    u32* stackEnd;               // at 0x308
    s32 error;                   // at 0x30C
    void* specific[2];           // at 0x310
} OSThread;

typedef void (*OSSwitchThreadCallback)(OSThread* currThread,
                                       OSThread* newThread);
typedef void* (*OSThreadFunc)(void* arg);

OSSwitchThreadCallback
OSSetSwitchThreadCallback(OSSwitchThreadCallback callback);
void __OSThreadInit(void);
void OSSetCurrentThread(OSThread* thread);
void OSInitMutexQueue(OSMutexQueue* queue);
void OSInitThreadQueue(OSThreadQueue* queue);
OSThread* OSGetCurrentThread(void);
BOOL OSIsThreadTerminated(OSThread* thread);
s32 OSDisableScheduler(void);
s32 OSEnableScheduler(void);
s32 __OSGetEffectivePriority(OSThread* thread);
void __OSPromoteThread(OSThread* thread, s32 prio);
void __OSReschedule(void);
void OSYieldThread(void);
BOOL OSCreateThread(OSThread* thread, OSThreadFunc func, void* funcArg,
                    void* stackBegin, u32 stackSize, s32 prio, u16 flags);
void OSExitThread(OSThread* thread);
void OSCancelThread(OSThread* thread);
BOOL OSJoinThread(OSThread* thread, void* val);
void OSDetachThread(OSThread* thread);
s32 OSResumeThread(OSThread* thread);
s32 OSSuspendThread(OSThread* thread);
void OSSleepThread(OSThreadQueue* queue);
void OSWakeupThread(OSThreadQueue* queue);
BOOL OSSetThreadPriority(OSThread* thread, s32 prio);
s32 OSGetThreadPriority(OSThread* thread);
void OSClearStack(u8 val);
void OSSleepTicks(s64 ticks);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
#ifndef RVL_SDK_OS_INTERRUPT_H
#define RVL_SDK_OS_INTERRUPT_H
/* "libs/RVL_SDK/include/revolution/OS/OSInterrupt.h" line 2 "types.h" */
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
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
#ifndef RVL_SDK_OS_IPC_H
#define RVL_SDK_OS_IPC_H
/* "libs/RVL_SDK/include/revolution/OS/OSIpc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void* __OSGetIPCBufferHi(void);
void* __OSGetIPCBufferLo(void);
void __OSInitIPCBuffer(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
#ifndef RVL_SDK_OS_LINK_H
#define RVL_SDK_OS_LINK_H
/* "libs/RVL_SDK/include/revolution/OS/OSLink.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSModuleInit(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
#ifndef RVL_SDK_OS_MEMORY_H
#define RVL_SDK_OS_MEMORY_H
/* "libs/RVL_SDK/include/revolution/OS/OSMemory.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_MEM_KB_TO_B(mb) ((mb) * 1024)
#define OS_MEM_B_TO_KB(mb) ((mb) / 1024)

#define OS_MEM_MB_TO_B(mb) ((mb) * 1024 * 1024)
#define OS_MEM_B_TO_MB(mb) ((mb) / 1024 / 1024)

#define OSIsMEM1Region(addr) (((u32)(addr) & 0x30000000) == 0)
#define OSIsMEM2Region(addr) (((u32)(addr) & 0x30000000) == 0x10000000)

#define OS_IS_MEM1_REGION(addr) OSIsMEM1Region(addr)
#define OS_IS_MEM2_REGION(addr) OSIsMEM2Region(addr)

u32 OSGetPhysicalMem1Size(void);
u32 OSGetPhysicalMem2Size(void);
u32 OSGetConsoleSimulatedMem1Size(void);
u32 OSGetConsoleSimulatedMem2Size(void);
void __OSRestoreCodeExecOnMEM1(u32 size);
void __OSInitMemoryProtection(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
#ifndef RVL_SDK_OS_MESSAGE_H
#define RVL_SDK_OS_MESSAGE_H
/* "libs/RVL_SDK/include/revolution/OS/OSMessage.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSMessage.h" line 4 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// General-purpose typedef
typedef void* OSMessage;

typedef enum { OS_MSG_BLOCKING = (1 << 0) } OSMessageFlags;

typedef struct OSMessageQueue {
    OSThreadQueue sendQueue; // at 0x0
    OSThreadQueue recvQueue; // at 0x8
    OSMessage* buffer;       // at 0x10
    s32 capacity;            // at 0x14
    s32 front;               // at 0x18
    s32 size;                // at 0x1C
} OSMessageQueue;

void OSInitMessageQueue(OSMessageQueue* queue, OSMessage* buffer, s32 capacity);
BOOL OSSendMessage(OSMessageQueue* queue, OSMessage mesg, u32 flags);
BOOL OSReceiveMessage(OSMessageQueue* queue, OSMessage* mesg, u32 flags);
BOOL OSJamMessage(OSMessageQueue* queue, OSMessage mesg, u32 flags);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
#ifndef RVL_SDK_OS_MUTEX_H
#define RVL_SDK_OS_MUTEX_H
/* "libs/RVL_SDK/include/revolution/OS/OSMutex.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSMutex.h" line 4 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSMutex {
    OSThreadQueue queue;  // at 0x0
    OSThread* thread;     // at 0x8
    s32 lock;             // at 0xC
    struct OSMutex* next; // at 0x10
    struct OSMutex* prev; // at 0x14
} OSMutex;

void OSInitMutex(OSMutex* mutex);
void OSLockMutex(OSMutex* mutex);
void OSUnlockMutex(OSMutex* mutex);
void __OSUnlockAllMutex(OSThread* thread);
BOOL OSTryLockMutex(OSMutex* mutex);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
#ifndef RVL_SDK_OS_NET_H
#define RVL_SDK_OS_NET_H
/* "libs/RVL_SDK/include/revolution/OS/OSNet.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitNet(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
#ifndef RVL_SDK_OS_PLAY_RECORD_H
#define RVL_SDK_OS_PLAY_RECORD_H
/* "libs/RVL_SDK/include/revolution/OS/OSPlayRecord.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSStartPlayRecord(void);
void __OSStopPlayRecord(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
#ifndef RVL_SDK_OS_PLAY_TIME_H
#define RVL_SDK_OS_PLAY_TIME_H
/* "libs/RVL_SDK/include/revolution/OS/OSPlayTime.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitPlayTime(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
#ifndef RVL_SDK_OS_RESET_H
#define RVL_SDK_OS_RESET_H
/* "libs/RVL_SDK/include/revolution/OS/OSReset.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (*OSShutdownFunction)(BOOL final, u32 event);

typedef enum {
    OS_SD_EVENT_FATAL,
    OS_SD_EVENT_1,
    OS_SD_EVENT_SHUTDOWN,
    OS_SD_EVENT_3,
    OS_SD_EVENT_RESTART,
    OS_SD_EVENT_RETURN_TO_MENU,
    OS_SD_EVENT_LAUNCH_APP,
} OSShutdownEvent;

typedef struct OSShutdownFunctionInfo {
    OSShutdownFunction func;             // at 0x0
    u32 prio;                            // at 0x4
    struct OSShutdownFunctionInfo* next; // at 0x8
    struct OSShutdownFunctionInfo* prev; // at 0xC
} OSShutdownFunctionInfo;

typedef struct OSShutdownFunctionQueue {
    OSShutdownFunctionInfo* head; // at 0x0
    OSShutdownFunctionInfo* tail; // at 0x4
} OSShutdownFunctionQueue;

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info);
BOOL __OSCallShutdownFunctions(u32 pass, u32 event);
void __OSShutdownDevices(u32 event);
void __OSGetDiscState(u8* out);
void OSShutdownSystem(void);
void OSRestart(u32 resetCode);
void OSReturnToMenu(void);
u32 OSGetResetCode(void);
void OSResetSystem(BOOL reset, u32 resetCode, BOOL forceMenu);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
#ifndef RVL_SDK_OS_RTC_H
#define RVL_SDK_OS_RTC_H
/* "libs/RVL_SDK/include/revolution/OS/OSRtc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSSram {
    u16 checksum;    // at 0x0
    u16 invchecksum; // at 0x2
    u32 ead0;        // at 0x4
    u32 ead1;        // at 0x8
    u32 counterBias; // at 0xC
    u8 dispOfsH;     // at 0x10
    u8 ntd;          // at 0x11
    u8 lang;         // at 0x12
    u8 flags;        // at 0x13
} OSSram;

typedef struct OSSramEx {
    char UNK_0x0[0x1C];
    u16 wirelessPadId[4]; // at 0x1C
    char UNK_0x38[0x3C - 0x38];
    u16 gbs; // at 0x3C
    char UNK_0x3E[0x40 - 0x3E];
} OSSramEx;

void __OSInitSram(void);
OSSramEx* __OSLockSramEx(void);
BOOL __OSUnlockSramEx(BOOL save);
BOOL __OSSyncSram(void);
BOOL __OSReadROM(void* dst, s32 size, const void* src);
u16 OSGetWirelessID(s32 pad);
void OSSetWirelessID(s32 pad, u16 id);
u16 OSGetGbsMode(void);
void OSSetGbsMode(u16 gbs);
BOOL __OSGetRTCFlags(u32* out);
BOOL __OSClearRTCFlags(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
#ifndef RVL_SDK_OS_SERIAL
#define RVK_SDK_OS_SERIAL

/* "libs/RVL_SDK/include/revolution/OS/OSSerial.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif

#define SI_MAX_COMCSR_INLNGTH 128
#define SI_MAX_COMCSR_OUTLNGTH 128
#define SI_ERROR_UNDER_RUN 0x0001
#define SI_ERROR_OVER_RUN 0x0002
#define SI_ERROR_COLLISION 0x0004
#define SI_ERROR_NO_RESPONSE 0x0008
#define SI_ERROR_WRST 0x0010
#define SI_ERROR_RDST 0x0020
#define SI_ERROR_UNKNOWN 0x0040
#define SI_CHAN0 0
#define SI_CHAN1 1
#define SI_CHAN2 2
#define SI_CHAN3 3
#define SI_CHAN0_BIT 0x80000000
#define SI_CHAN1_BIT 0x40000000
#define SI_CHAN2_BIT 0x20000000
#define SI_CHAN3_BIT 0x10000000
#define SI_CHAN_BIT(chan) (SI_CHAN0_BIT >> (chan))
#define SI_TYPE_MASK 0x18000000u
#define SI_TYPE_N64 0x00000000u
#define SI_TYPE_DOLPHIN 0x08000000u
#define SI_TYPE_GC SI_TYPE_DOLPHIN
#define SI_GC_WIRELESS 0x80000000
#define SI_GC_NOMOTOR 0x20000000
#define SI_GC_STANDARD 0x01000000
#define SI_WIRELESS_RECEIVED 0x40000000
#define SI_WIRELESS_IR 0x04000000
#define SI_WIRELESS_STATE 0x02000000
#define SI_WIRELESS_ORIGIN 0x00200000
#define SI_WIRELESS_FIX_ID 0x00100000
#define SI_WIRELESS_TYPE 0x000f0000
#define SI_WIRELESS_LITE_MASK 0x000c0000
#define SI_WIRELESS_CONT_MASK 0x00080000
#define SI_WIRELESS_ID 0x00c0ff00
#define SI_WIRELESS_TYPE_ID (SI_WIRELESS_TYPE | SI_WIRELESS_ID)
#define SI_N64_CONTROLLER (SI_TYPE_N64 | 0x05000000)
#define SI_N64_MIC (SI_TYPE_N64 | 0x00010000)
#define SI_N64_KEYBOARD (SI_TYPE_N64 | 0x00020000)
#define SI_N64_MOUSE (SI_TYPE_N64 | 0x02000000)
#define SI_GBA (SI_TYPE_N64 | 0x00040000)
#define SI_GC_CONTROLLER (SI_TYPE_GC | SI_GC_STANDARD)
#define SI_GC_RECEIVER (SI_TYPE_GC | SI_GC_WIRELESS)
#define SI_GC_WAVEBIRD                                                                             \
  (SI_TYPE_GC | SI_GC_WIRELESS | SI_GC_STANDARD | SI_WIRELESS_STATE | SI_WIRELESS_FIX_ID)
#define SI_GC_KEYBOARD (SI_TYPE_GC | 0x00200000)
#define SI_GC_STEERING (SI_TYPE_GC | 0x00000000)

u32 SIProbe(s32 chan);
char* SIGetTypeString(u32 type);
void SIRefreshSamplingRate(void);
void SISetSamplingRate(u32 msec);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
#ifndef RVL_SDK_OS_STATE_FLAGS_H
#define RVL_SDK_OS_STATE_FLAGS_H
/* "libs/RVL_SDK/include/revolution/OS/OSStateFlags.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSStateFlags {
    u32 checksum; // at 0x0
    u8 BYTE_0x4;
    u8 BYTE_0x5;  // at 0x5
    u8 discState; // at 0x6
    u8 BYTE_0x7;
    u32 WORD_0x8;
    u32 WORD_0xC;
    u32 WORD_0x10;
    u32 WORD_0x14;
    u32 WORD_0x18;
    u32 WORD_0x1C;
} OSStateFlags;

BOOL __OSWriteStateFlags(const OSStateFlags* state);
BOOL __OSReadStateFlags(OSStateFlags* state);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
#ifndef RVL_SDK_OS_STATETM_H
#define RVL_SDK_OS_STATETM_H
/* "libs/RVL_SDK/include/revolution/OS/OSStateTM.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSStateCallback)(void);

OSStateCallback OSSetResetCallback(OSStateCallback callback);
OSStateCallback OSSetPowerCallback(OSStateCallback callback);
BOOL __OSInitSTM(void);
void __OSShutdownToSBY(void);
void __OSHotReset(void);
BOOL __OSGetResetButtonStateRaw(void);
s32 __OSSetVIForceDimming(u32 arg0, u32 arg1, u32 arg2);
s32 __OSSetIdleLEDMode(u32 mode);
s32 __OSUnRegisterStateEvent(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
#ifndef RVL_SDK_OS_SYNC_H
#define RVL_SDK_OS_SYNC_H
/* "libs/RVL_SDK/include/revolution/OS/OSSync.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitSystemCall(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
#ifndef RVL_SDK_OS_TIME_H
#define RVL_SDK_OS_TIME_H
/* "libs/RVL_SDK/include/revolution/OS/OSTime.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSTime.h" line 4 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Time base frequency = 1/4 bus clock
#define OS_TIME_SPEED (OS_BUS_CLOCK_SPEED / 4)

// OS time -> Real time
#define OS_TICKS_TO_SEC(x) ((x) / (OS_TIME_SPEED))
#define OS_TICKS_TO_MSEC(x) ((x) / (OS_TIME_SPEED / 1000))
#define OS_TICKS_TO_USEC(x) (((x) * 8) / (OS_TIME_SPEED / 125000))
#define OS_TICKS_TO_NSEC(x) (((x) * 8000) / (OS_TIME_SPEED / 125000))

// Real time -> OS time
#define OS_SEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED))
#define OS_MSEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 1000))
#define OS_USEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 125000) / 8)
#define OS_NSEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 125000) / 8000)

// Interpret as signed to find tick delta
#define OS_TICKS_DELTA(x, y) ((s32)x - (s32)y)

typedef struct OSCalendarTime {
    s32 sec;   // at 0x0
    s32 min;   // at 0x4
    s32 hour;  // at 0x8
    s32 mday;  // at 0xC
    s32 month; // at 0x10
    s32 year;  // at 0x14
    s32 wday;  // at 0x18
    s32 yday;  // at 0x1C
    s32 msec;  // at 0x20
    s32 usec;  // at 0x24
} OSCalendarTime;

s64 OSGetTime(void);
u32 OSGetTick(void);

s64 __OSGetSystemTime(void);
s64 __OSTimeToSystemTime(s64 time);

void OSTicksToCalendarTime(s64 time, OSCalendarTime* cal);
s64 OSCalendarTimeToTicks(const OSCalendarTime* cal);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
#ifndef RVL_SDK_OS_UTF_H
#define RVL_SDK_OS_UTF_H
/* "libs/RVL_SDK/include/revolution/OS/OSUtf.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

const u8* OSUTF8to32(const u8* utf8, u32* utf32);
const wchar_t* OSUTF16to32(const wchar_t* utf16, u32* utf32);
u8 OSUTF32toANSI(u32 utf32);
wchar_t OSUTF32toSJIS(u32 utf32);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
#ifndef RVL_SDK_OS_PPC_EABI_INIT_H
#define RVL_SDK_OS_PPC_EABI_INIT_H
/* "libs/RVL_SDK/include/revolution/OS/__ppc_eabi_init.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#pragma section ".init"
DECL_SECTION(".init") void __init_hardware(void);
DECL_SECTION(".init") void __flush_cache(void*, size_t);

void __init_user(void);
void __init_cpp(void);
void __fini_cpp(void);
DECL_WEAK void exit(void);
void _ExitProcess(void);

/**
 * Linker Generated Symbols
 */

// Declare linker symbols for a section in the ROM
#define DECL_ROM_SECTION(x)                                                    \
    extern u8 _f##x[];                                                         \
    extern u8 _f##x##_rom[];                                                   \
    extern u8 _e##x[];

// Declare linker symbols for a BSS section
#define DECL_BSS_SECTION(x)                                                    \
    extern u8 _f##x[];                                                         \
    extern u8 _e##x[];

// Debugger stack
extern u8 _db_stack_addr[];
extern u8 _db_stack_end[];

// Program arena
extern u8 __ArenaLo[];
extern u8 __ArenaHi[];

// Program stack
extern u8 _stack_addr[];
extern u8 _stack_end[];

// Small data bases
extern u8 _SDA_BASE_[];
extern u8 _SDA2_BASE_[];

// ROM sections
DECL_ROM_SECTION(_init);
DECL_ROM_SECTION(extab);
DECL_ROM_SECTION(extabindex);
DECL_ROM_SECTION(_text);
DECL_ROM_SECTION(_ctors);
DECL_ROM_SECTION(_dtors);
DECL_ROM_SECTION(_rodata);
DECL_ROM_SECTION(_data);
DECL_ROM_SECTION(_sdata);
DECL_ROM_SECTION(_sdata2);
DECL_ROM_SECTION(_stack);

// BSS sections
DECL_BSS_SECTION(_bss);
DECL_BSS_SECTION(_sbss);
DECL_BSS_SECTION(_sbss2);

typedef struct RomSection {
    void* phys;  // at 0x4
    void* virt;  // at 0x0
    size_t size; // at 0x8
} RomSection;

typedef struct BssSection {
    void* virt;  // at 0x0
    size_t size; // at 0x8
} BssSection;

typedef struct ExtabIndexInfo {
    void* section;                // at 0x0
    struct ExtabIndexInfo* extab; // at 0x4
    void* codeStart;              // at 0x8
    u32 codeSize;                 // at 0xC
} ExtabIndexInfo;

DECL_SECTION(".init") extern const RomSection _rom_copy_info[];
DECL_SECTION(".init") extern const BssSection _bss_init_info[];
DECL_SECTION(".init") extern const ExtabIndexInfo _eti_init_info[];

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */

/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 9 "cmath" */
/* end "cmath" */

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * Implementation details
 *
 ******************************************************************************/
namespace detail {

f32 FExp(f32 x);
f32 FLog(f32 x);

} // namespace detail

/******************************************************************************
 *
 * Arithmetic functions
 *
 ******************************************************************************/
f32 FrSqrt(f32 x);

inline f32 FAbs(register f32 x) {
    register f32 ax;

    ASM (
        fabs ax, x
    )

    return ax;
}

inline f32 FCeil(f32 x) {
    return std::ceilf(x);
}

inline f32 FExp(f32 x) {
    return detail::FExp(x);
}

inline f32 FFloor(f32 x) {
    return std::floorf(x);
}

inline f32 FInv(register f32 x) {
    register f32 work0, work1, work2, work3;

    ASM (
        fmr  work1, x     // x
        fres work0, work1 // 1/x

        // Refine estimate
        ps_add   work2, work0, work0        // 2/x
        ps_mul   work3, work0, work0        // 1/x^2
        ps_nmsub work0, work1, work3, work2 // -(x * 1/x^2 - 2/x)
    )

    return work0;
}

inline f32 FMod(f32 x, f32 y) {
    return std::fmodf(x, y);
}

inline f32 FModf(f32 x, f32* pY) {
    return std::modff(x, pY);
}

inline f32 FSqrt(f32 x) {
#line 627
    NW4R_DB_ASSERT_WARN(x >= 0.0f, "FSqrt: Input is out of the domain.");

    return x <= 0.0f ? 0.0f : x * FrSqrt(x);
}

inline f32 FLog(f32 x) {
    if (x > 0.0f) {
        return detail::FLog(x);
    }

    return NW4R_MATH_QNAN;
}

inline f32 FSelect(register f32 value, register f32 ge_zero,
                   register f32 lt_zero) {
    register f32 ret;

    ASM (
        fsel ret, value, ge_zero, lt_zero
    )

    return ret;
}

/******************************************************************************
 *
 * Fastcast functions
 *
 ******************************************************************************/
inline f32 U16ToF32(u16 arg) {
    f32 ret;
    OSu16tof32(&arg, &ret);
    return ret;
}
inline u16 F32ToU16(f32 arg) {
    u16 ret;
    OSf32tou16(&arg, &ret);
    return ret;
}

inline f32 S16ToF32(s16 arg) {
    f32 ret;
    OSs16tof32(&arg, &ret);
    return ret;
}
inline s16 F32ToS16(f32 arg) {
    s16 ret;
    OSf32tos16(&arg, &ret);
    return ret;
}

inline u32 F32AsU32(f32 arg) {
    return *reinterpret_cast<u32*>(&arg);
}
inline f32 U32AsF32(u32 arg) {
    return *reinterpret_cast<f32*>(&arg);
}

inline s32 FGetExpPart(f32 x) {
    s32 s = F32AsU32(x);
    return ((s >> 23) & 0xFF) - 127;
}
inline f32 FGetMantPart(f32 x) {
    u32 u = F32AsU32(x);
    return U32AsF32((u & 0x807FFFFF) | 0x3F800000);
}

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_arithmetic.h" */
/* "libs/nw4r/include/nw4r/math.h" line 4 "nw4r/math/math_geometry.h" */
#ifndef NW4R_MATH_GEOMETRY_H
#define NW4R_MATH_GEOMETRY_H
/* "libs/nw4r/include/nw4r/math/math_geometry.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_geometry.h" line 4 "nw4r/math/math_types.h" */
#ifndef NW4R_MATH_TYPES_H
#define NW4R_MATH_TYPES_H
/* "libs/nw4r/include/nw4r/math/math_types.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_types.h" line 4 "nw4r/math/math_arithmetic.h" */
/* end "nw4r/math/math_arithmetic.h" */
/* "libs/nw4r/include/nw4r/math/math_types.h" line 5 "nw4r/math/math_triangular.h" */
#ifndef NW4R_MATH_TRIANGULAR_H
#define NW4R_MATH_TRIANGULAR_H
/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 4 "nw4r/db/db_assert.h" */
/* end "nw4r/db/db_assert.h" */
/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 5 "nw4r/math/math_arithmetic.h" */
/* end "nw4r/math/math_arithmetic.h" */
/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 6 "nw4r/math/math_constant.h" */
/* end "nw4r/math/math_constant.h" */

/******************************************************************************
 *
 * Macros to convert between units
 *
 ******************************************************************************/
// [0, 65535] -> [0.0, 256.0f]
#define NW4R_MATH_IDX_TO_FIDX(x) ((x) * (1.0f / 256.0f))

// Convert degrees <-> lookup-table index
#define NW4R_MATH_DEG_TO_FIDX(x) ((x) * (256.0f / 360.0f))
#define NW4R_MATH_FIDX_TO_DEG(x) ((x) * (360.0f / 256.0f))

// Convert radians <-> lookup-table index
#define NW4R_MATH_RAD_TO_FIDX(x) ((x) * (128.0f / NW4R_MATH_PI))
#define NW4R_MATH_FIDX_TO_RAD(x) ((x) * (NW4R_MATH_PI / 128.0f))

// Convert degrees <-> radians
#define NW4R_MATH_DEG_TO_RAD(x) ((x) * (NW4R_MATH_PI / 180.0f))
#define NW4R_MATH_RAD_TO_DEG(x) ((x) * (180.0f / NW4R_MATH_PI))

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * Sin functions
 *
 ******************************************************************************/
f32 SinFIdx(f32 fidx);

inline f32 SinIdx(u16 idx) {
    return SinFIdx(NW4R_MATH_IDX_TO_FIDX(U16ToF32(idx)));
}
inline f32 SinDeg(f32 deg) {
    return SinFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
}
inline f32 SinRad(f32 rad) {
    return SinFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Cosine functions
 *
 ******************************************************************************/
f32 CosFIdx(f32 fidx);

inline f32 CosDeg(f32 deg) {
    return CosFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
}
inline f32 CosRad(f32 rad) {
    return CosFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Tangent functions
 *
 ******************************************************************************/
inline f32 TanFIdx(f32 fidx) {
    return std::tanf(NW4R_MATH_FIDX_TO_RAD(fidx));
}
inline f32 TanDeg(f32 deg) {
    return TanFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
}
inline f32 TanRad(f32 rad) {
    return TanFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Sin & cosine functions
 *
 ******************************************************************************/
void SinCosFIdx(f32* pSin, f32* pCos, f32 fidx);

inline void SinCosDeg(f32* pSin, f32* pCos, f32 deg) {
    return SinCosFIdx(pSin, pCos, NW4R_MATH_DEG_TO_FIDX(deg));
}
inline void SinCosRad(f32* pSin, f32* pCos, f32 rad) {
    return SinCosFIdx(pSin, pCos, NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Arc-sine functions
 *
 ******************************************************************************/

 inline f32 AsinRad(f32 x){
    bool inRange = x <= 1.0f && x >= -1.0f;

#line 228
    NW4R_DB_ASSERT_WARN(inRange, "AsinRad: Input is out of the domain.");

    return std::asin(x);
}

/******************************************************************************
 *
 * Arc-cosine functions
 *
 ******************************************************************************/

 inline f32 AcosRad(f32 x){
    bool inRange = x <= 1.0f && x >= -1.0f;

#line 239
    NW4R_DB_ASSERT_WARN(inRange, "AcosRad: Input is out of the domain.");

    return std::acos(x);
}

/******************************************************************************
 *
 * Arc-tangent functions
 *
 ******************************************************************************/
f32 AtanFIdx(f32 x);

inline f32 AtanDeg(f32 x) {
    return NW4R_MATH_FIDX_TO_DEG(AtanFIdx(x));
}
inline f32 AtanRad(f32 x) {
    return NW4R_MATH_FIDX_TO_RAD(AtanFIdx(x));
}

/******************************************************************************
 *
 * Arc-tangent (2-argument) functions
 *
 ******************************************************************************/
f32 Atan2FIdx(f32 y, f32 x);

inline f32 Atan2Deg(f32 y, f32 x) {
    return NW4R_MATH_FIDX_TO_DEG(Atan2FIdx(y, x));
}
inline f32 Atan2Rad(f32 y, f32 x) {
    return NW4R_MATH_FIDX_TO_RAD(Atan2FIdx(y, x));
}

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_triangular.h" */

/* "libs/nw4r/include/nw4r/math/math_types.h" line 7 "revolution/MTX.h" */
#ifndef RVL_SDK_PUBLIC_MTX_H
#define RVL_SDK_PUBLIC_MTX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/MTX.h" line 6 "revolution/MTX/mtx.h" */
#ifndef RVL_SDK_MTX_MTX_H
#define RVL_SDK_MTX_MTX_H
/* "libs/RVL_SDK/include/revolution/MTX/mtx.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtx.h" line 4 "revolution/MTX/mtxtypes.h" */
#ifndef RVL_SDK_MTX_TYPES_H
#define RVL_SDK_MTX_TYPES_H
/* "libs/RVL_SDK/include/revolution/MTX/mtxtypes.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef struct Vec {
    f32 x, y, z;
} Vec;

typedef struct Vec2 {
    f32 x, y;
} Vec2;

typedef struct Quaternion {
    f32 x, y, z, w;
} Quaternion;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

// TODO(kiwi) Create macros for PS/C_ functions
void PSMTXIdentity(Mtx);
void PSMTXCopy(const Mtx, Mtx);
void PSMTXConcat(const Mtx, const Mtx, Mtx);
void PSMTXConcatArray(const Mtx, const Mtx, Mtx, u32);
void PSMTXTranspose(const Mtx, Mtx);
u32 PSMTXInverse(const Mtx, Mtx);
u32 PSMTXInvXpose(const Mtx, Mtx);
void PSMTXRotRad(Mtx, char, f32);
void PSMTXRotTrig(Mtx, f32, f32, char);
void PSMTXRotAxisRad(Mtx, const Vec*, f32);
void PSMTXTrans(Mtx, f32, f32, f32);
void PSMTXTransApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXScale(Mtx, f32, f32, f32);
void PSMTXScaleApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXQuat(Mtx, const Quaternion*);

void C_MTXLookAt(Mtx, const Vec*, const Vec*, const Vec*);
void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);
void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtx.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 7 "revolution/MTX/mtx44.h" */
#ifndef RVL_SDK_MTX_MTX44_H
#define RVL_SDK_MTX_MTX44_H
/* "libs/RVL_SDK/include/revolution/MTX/mtx44.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtx44.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSMTX44Copy(Mtx44, Mtx44);
void C_MTXFrustum(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx44 mtx, f32 fovy, f32 aspect, f32 n, f32 f);
void C_MTXOrtho(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtx44.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 8 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 9 "revolution/MTX/mtxvec.h" */
#ifndef RVL_SDK_MTX_MTXVEC_H
#define RVL_SDK_MTX_MTXVEC_H
/* "libs/RVL_SDK/include/revolution/MTX/mtxvec.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtxvec.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSMTXMultVec(const Mtx mtx, const Vec* vec, Vec* out);
void PSMTXMultVecSR(const Mtx, const Vec*, Vec*);
void PSMTXMultVecArraySR();

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtxvec.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 10 "revolution/MTX/quat.h" */
#ifndef RVL_SDK_MTX_QUAT_H
#define RVL_SDK_MTX_QUAT_H
/* "libs/RVL_SDK/include/revolution/MTX/quat.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/quat.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif


void PSQUATAdd(const Quaternion* a, const Quaternion* b, Quaternion* sum);
void PSQUATMultiply(const Quaternion* a, const Quaternion* b, Quaternion* prod);
void PSQUATScale(const Quaternion* in, Quaternion* out, f32 scale);
f32 PSQUATDotProduct(const Quaternion* a, const Quaternion* b);
void PSQUATNormalize(const Quaternion* in, Quaternion* out);
void PSQUATInverse(const Quaternion* in, Quaternion* out);
void C_QUATMtx(Quaternion* quat, const Mtx mtx);
void C_QUATLerp(const Quaternion* a, const Quaternion* b, Quaternion* out,
                f32 t);
void C_QUATSlerp(const Quaternion* a, const Quaternion* b, Quaternion* out,
                 f32 t);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/quat.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 11 "revolution/MTX/vec.h" */
#ifndef RVL_SDK_MTX_VEC_H
#define RVL_SDK_MTX_VEC_H
/* "libs/RVL_SDK/include/revolution/MTX/vec.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/vec.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSVECAdd(const Vec* a, const Vec* b, Vec* sum);
void PSVECScale(const Vec* in, Vec* out, f32 scale);
void PSVECNormalize(const Vec* in, Vec* out);
f32 PSVECMag(const Vec* v);
f32 PSVECDotProduct(const Vec* a, const Vec* b);
void PSVECCrossProduct(const Vec* a, const Vec* b, Vec* prod);
f32 PSVECSquareDistance(const Vec* a, const Vec* b);
void C_VECHalfAngle(const Vec* a, const Vec* b, Vec* half);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/vec.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX.h" */

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * VEC2 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _VEC2 {
    f32 x, y;
};

struct VEC2 : _VEC2 {
    VEC2() {}
    VEC2(f32 fx, f32 fy) {
        x = fx;
        y = fy;
    }

    operator f32*() {
        return reinterpret_cast<f32*>(this);
    }
    operator const f32*() const {
        return reinterpret_cast<const f32*>(this);
    }

    VEC2 operator+(const VEC2& rRhs) const {
        return VEC2(x + rRhs.x, y + rRhs.y);
    }
    VEC2 operator-(const VEC2& rRhs) const {
        return VEC2(x - rRhs.x, y - rRhs.y);
    }

    VEC2& operator+=(const VEC2& rRhs) {
        x += rRhs.x;
        y += rRhs.y;
        return *this;
    }
    VEC2& operator-=(const VEC2& rRhs) {
        x -= rRhs.x;
        y -= rRhs.y;
        return *this;
    }

    bool operator==(const VEC2& rRhs) const {
        return x == rRhs.x && y == rRhs.y;
    }
    bool operator!=(const VEC2& rRhs) const {
        return x != rRhs.x || y != rRhs.y;
    }
};

/******************************************************************************
 *
 * VEC3 structure
 *
 ******************************************************************************/
// Forward declarations
struct VEC3;
VEC3* VEC3Add(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3Sub(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3Scale(VEC3* pOut, const VEC3* pIn, f32 scale);

// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _VEC3 {
    f32 x, y, z;
};

struct VEC3 : _VEC3 {
    VEC3() {}
    VEC3(f32 fx, f32 fy, f32 fz) {
        x = fx;
        y = fy;
        z = fz;
    }
    VEC3(const _VEC3& rVec) {
        x = rVec.x;
        y = rVec.y;
        z = rVec.z;
    }
    VEC3(const Vec& rVec) {
        x = rVec.x;
        y = rVec.y;
        z = rVec.z;
    }
    VEC3(const f32* pData) {
        x = pData[0];
        y = pData[1];
        z = pData[2];
    }

    operator Vec*() {
        return reinterpret_cast<Vec*>(this);
    }
    operator const Vec*() const {
        return reinterpret_cast<const Vec*>(this);
    }

    f32 LenSq() const {
        return x * x + y * y + z * z;
    }

    VEC3 operator-() const {
        return VEC3(-x, -y, -z);
    }

    VEC3 operator+(const VEC3& rRhs) const {
        VEC3 out;
        VEC3Add(&out, this, &rRhs);
        return out;
    }
    VEC3 operator-(const VEC3& rRhs) const {
        VEC3 out;
        VEC3Sub(&out, this, &rRhs);
        return out;
    }
    VEC3 operator*(f32 x) const {
        VEC3 out;
        VEC3Scale(&out, this, x);
        return out;
    }
    VEC3 operator/(f32 x) const {
        f32 r = 1 / x;
        return *this * r;
    }

    VEC3& operator+=(const VEC3& rRhs) {
        VEC3Add(this, this, &rRhs);
        return *this;
    }
    VEC3& operator-=(const VEC3& rRhs) {
        VEC3Sub(this, this, &rRhs);
        return *this;
    }
    VEC3& operator*=(f32 x) {
        VEC3Scale(this, this, x);
        return *this;
    }
    VEC3& operator/=(f32 x) {
        return *this *= (1 / x);
    }

    bool operator==(const VEC3& rRhs) const {
        return x == rRhs.x && y == rRhs.y && z == rRhs.z;
    }
    bool operator!=(const VEC3& rRhs) const {
        return x != rRhs.x || y != rRhs.y || z != rRhs.z;
    }
};

/******************************************************************************
 *
 * MTX33 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _MTX33 {
    union {
        struct {
            f32 _00, _01, _02;
            f32 _10, _11, _12;
            f32 _20, _21, _22;
        };

        f32 m[3][3];
        f32 a[3 * 3];
    };
};

struct MTX33 : _MTX33 {
    MTX33() {}
};

/******************************************************************************
 *
 * MTX34 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _MTX34 {
    union {
        struct {
            f32 _00, _01, _02, _03;
            f32 _10, _11, _12, _13;
            f32 _20, _21, _22, _23;
        };

        f32 m[3][4];
        f32 a[3 * 4];
        Mtx mtx;
    };
};

struct MTX34 : _MTX34 {
    typedef f32 (*MtxRef)[4];
    typedef const f32 (*MtxRefConst)[4];

    MTX34() {}

    // clang-format off
    MTX34(f32 f00, f32 f01, f32 f02, f32 f03,
          f32 f10, f32 f11, f32 f12, f32 f13,
          f32 f20, f32 f21, f32 f22, f32 f23) {
        _00 = f00; _01 = f01; _02 = f02; _03 = f03;
        _10 = f10; _11 = f11; _12 = f12; _13 = f13;
        _20 = f20; _21 = f21; _22 = f22; _23 = f23;
    }
    // clang-format on

    operator MtxRef() {
        return mtx;
    }
    operator MtxRefConst() const {
        return mtx;
    }
};

/******************************************************************************
 *
 * MTX44 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _MTX44 {
    union {
        struct {
            f32 _00, _01, _02, _03;
            f32 _10, _11, _12, _13;
            f32 _20, _21, _22, _23;
            f32 _30, _31, _32, _33;
        };

        f32 m[4][4];
        f32 a[4 * 4];
        Mtx44 mtx;
    };
};

struct MTX44 : _MTX44 {
    typedef f32 (*Mtx44Ref)[4];
    typedef const f32 (*Mtx44RefConst)[4];

    MTX44() {}

    operator Mtx44Ref() {
        return mtx;
    }
    operator Mtx44RefConst() const {
        return mtx;
    }
};

/******************************************************************************
 *
 * QUAT structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _QUAT {
    f32 x, y, z, w;
};

struct QUAT : _QUAT {
    QUAT() {}
    QUAT(f32 fx, f32 fy, f32 fz, f32 fw) {
        x = fx;
        y = fy;
        z = fz;
        w = fw;
    }

    // TODO(kiwi) These are not real AFAIK. Do they really manually cast the
    // QUAT?
    operator Quaternion*() {
        return reinterpret_cast<Quaternion*>(this);
    }
    operator const Quaternion*() const {
        return reinterpret_cast<const Quaternion*>(this);
    }
};

/******************************************************************************
 *
 * VEC2 functions
 *
 ******************************************************************************/
inline f32 VEC2Len(const VEC2* pVec) {
    return FSqrt(pVec->x * pVec->x + pVec->y * pVec->y);
}

/******************************************************************************
 *
 * VEC3 functions
 *
 ******************************************************************************/
VEC3* VEC3Maximize(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3Minimize(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3TransformNormal(VEC3* pOut, const MTX34* pMtx, const VEC3* pVec);

inline VEC3* VEC3Add(register VEC3* pOut, register const VEC3* pA,
                     register const VEC3* pB) {
    register f32 work0, work1, work2;

    ASM (
        // Add XY
        psq_l  work0, VEC3.x(pA),   0, 0
        psq_l  work1, VEC3.x(pB),   0, 0
        ps_add work2, work0, work1
        psq_st work2, VEC3.x(pOut), 0, 0
        
        // Add Z
        psq_l  work0, VEC3.z(pA),   1, 0
        psq_l  work1, VEC3.z(pB),   1, 0
        ps_add work2, work0, work1
        psq_st work2, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline f32 VEC3Dot(register const VEC3* pA, register const VEC3* pB) {
    register f32 dot;
    register f32 work0, work1, work2, work3;

    ASM (
        // YZ product
        psq_l  work0, VEC3.y(pA), 0, 0
        psq_l  work1, VEC3.y(pB), 0, 0
        ps_mul work0, work0, work1
        
        // X product + YZ product
        psq_l   work3, VEC3.x(pA), 1, 0
        psq_l   work2, VEC3.x(pB), 1, 0
        ps_madd work1, work3, work2, work0
        
        // Dot product
        ps_sum0 dot, work1, work0, work0
    )

    return dot;
}

inline f32 VEC3LenSq(register const VEC3* pVec) {
    register f32 work0, work1, work2;

    ASM (
        // Square XY
        psq_l  work0, VEC3.x(pVec), 0, 0
        ps_mul work0, work0, work0

        // Square Z
        lfs     work1, VEC3.z(pVec)
        ps_madd work2, work1, work1, work0

        // Sum products
        ps_sum0 work2, work2, work0, work0
    )

    return work2;
}

inline VEC3* VEC3Lerp(register VEC3* pOut, register const VEC3* pVec1,
                      register const VEC3* pVec2, register f32 t) {
    register f32 work0, work1, work2;

    ASM (
        // X/Y delta
        psq_l  work0, VEC3.x(pVec1), 0, 0
        psq_l  work1, VEC3.x(pVec2), 0, 0
        ps_sub work2, work1, work0
        // Scale with time and add to v0
        ps_madds0 work2, work2, t, work0
        psq_st    work2, VEC3.x(pOut), 0, 0
        
        // Z delta
        psq_l  work0, VEC3.z(pVec1), 1, 0
        psq_l  work1, VEC3.z(pVec2), 1, 0
        ps_sub work2, work1, work0
        // Scale with time and add to v0
        ps_madds0 work2, work2, t, work0
        psq_st    work2, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline VEC3* VEC3Scale(register VEC3* pOut, register const VEC3* pIn,
                       register f32 scale) {
    register f32 work0, work1;

    ASM (
        // Scale XY
        psq_l    work0, VEC3.x(pIn),  0, 0
        ps_muls0 work1, work0, scale
        psq_st   work1, VEC3.x(pOut), 0, 0

        // Scale Z
        psq_l    work0, VEC3.z(pIn),  1, 0
        ps_muls0 work1, work0, scale
        psq_st   work1, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline VEC3* VEC3Sub(register VEC3* pOut, register const VEC3* pA,
                     register const VEC3* pB) {
    register f32 work0, work1, work2;

    ASM (
        // Sub XY
        psq_l  work0, VEC3.x(pA),   0, 0
        psq_l  work1, VEC3.x(pB),   0, 0
        ps_sub work2, work0, work1
        psq_st work2, VEC3.x(pOut), 0, 0
        
        // Sub Z
        psq_l  work0, VEC3.z(pA),   1, 0
        psq_l  work1, VEC3.z(pB),   1, 0
        ps_sub work2, work0, work1
        psq_st work2, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline VEC3* VEC3Cross(VEC3* pOut, const VEC3* pA, const VEC3* pB) {
    PSVECCrossProduct(*pA, *pB, *pOut);
    return pOut;
}

inline f32 VEC3DistSq(const VEC3* pA, const VEC3* pB) {
    return PSVECSquareDistance(*pA, *pB);
}

inline f32 VEC3Len(const VEC3* pVec) {
    return PSVECMag(*pVec);
}

inline VEC3* VEC3Normalize(VEC3* pOut, const VEC3* pIn) {
    PSVECNormalize(*pIn, *pOut);
    return pOut;
}

inline VEC3* VEC3Transform(VEC3* pOut, const MTX34* pMtx, const VEC3* pVec) {
    PSMTXMultVec(*pMtx, *pVec, *pOut);
    return pOut;
}

inline VEC3* VEC3TransformCoord(VEC3* pOut, const MTX34* pMtx,
                                const VEC3* pVec) {
    PSMTXMultVec(*pMtx, *pVec, *pOut);
    return pOut;
}

/******************************************************************************
 *
 * MTX33 functions
 *
 ******************************************************************************/
MTX33* MTX33Identity(MTX33* pMtx);

/******************************************************************************
 *
 * MTX34 functions
 *
 ******************************************************************************/
MTX33* MTX34ToMTX33(MTX33* pOut, const MTX34* pIn);
u32 MTX34InvTranspose(MTX33* pOut, const MTX34* pIn);
MTX34* MTX34Zero(MTX34* pMtx);
MTX34* MTX34Scale(MTX34* pOut, const MTX34* pIn, const VEC3* pScale);
MTX34* MTX34Trans(MTX34* pOut, const MTX34* pIn, const VEC3* pTrans);
MTX34* MTX34RotAxisFIdx(MTX34* pMtx, const VEC3* pAxis, f32 fidx);
MTX34* MTX34RotXYZFIdx(MTX34* pMtx, f32 fx, f32 fy, f32 fz);

inline MTX34* MTX34Copy(MTX34* pOut, const MTX34* pIn) {
    PSMTXCopy(*pIn, *pOut);
    return pOut;
}

inline MTX34* MTX34Identity(MTX34* pMtx) {
    PSMTXIdentity(*pMtx);
    return pMtx;
}

inline u32 MTX34Inv(MTX34* pOut, const MTX34* pIn) {
    return PSMTXInverse(*pIn, *pOut);
}

inline u32 MTX34InvTranspose(MTX34* pOut, const MTX34* pIn) {
    return PSMTXInvXpose(*pIn, *pOut);
}

inline MTX34* MTX34LookAt(MTX34* pMtx, const VEC3* pPos, const VEC3* pUp,
                          const VEC3* pTarget) {
    C_MTXLookAt(*pMtx, *pPos, *pUp, *pTarget);
    return pMtx;
}

inline MTX34* MTX34Mult(MTX34* pOut, const MTX34* pA, const MTX34* pB) {
    PSMTXConcat(*pA, *pB, *pOut);
    return pOut;
}

inline MTX34* MTX34MultArray(MTX34* pOut, const MTX34* p1, const MTX34* pSrc,
                             u32 len) {
    PSMTXConcatArray(*p1, *pSrc, *pOut, len);
    return pOut;
}

inline MTX34* MTX34RotAxisRad(MTX34* pOut, const VEC3* pAxis, f32 frad) {
    return MTX34RotAxisFIdx(pOut, pAxis, NW4R_MATH_RAD_TO_FIDX(frad));
}

inline MTX34* MTX34RotXYZDeg(MTX34* pMtx, f32 dx, f32 dy, f32 dz) {
    return MTX34RotXYZFIdx(pMtx, NW4R_MATH_DEG_TO_FIDX(dx),
                           NW4R_MATH_DEG_TO_FIDX(dy),
                           NW4R_MATH_DEG_TO_FIDX(dz));
}

inline MTX34* MTX34RotXYZRad(MTX34* pMtx, f32 rx, f32 ry, f32 rz) {
    return MTX34RotXYZFIdx(pMtx, NW4R_MATH_RAD_TO_FIDX(rx),
                           NW4R_MATH_RAD_TO_FIDX(ry),
                           NW4R_MATH_RAD_TO_FIDX(rz));
}

inline MTX34* MTX34Scale(MTX34* pOut, const VEC3* pScale, const MTX34* pIn) {
    PSMTXScaleApply(*pIn, *pOut, pScale->x, pScale->y, pScale->z);
    return pOut;
}

inline QUAT* MTX34ToQUAT(QUAT* pQuat, const MTX34* pMtx) {
    C_QUATMtx(*pQuat, *pMtx);
    return pQuat;
}

inline MTX34* MTX34Trans(MTX34* pOut, const VEC3* pTrans, const MTX34* pIn) {
    PSMTXTransApply(*pIn, *pOut, pTrans->x, pTrans->y, pTrans->z);
    return pOut;
}

/******************************************************************************
 *
 * MTX44 functions
 *
 ******************************************************************************/
MTX44* MTX44Identity(MTX44* pMtx);
MTX44* MTX44Copy(MTX44* pDst, const MTX44* pSrc);

/******************************************************************************
 *
 * QUAT functions
 *
 ******************************************************************************/
inline MTX34* QUATToMTX34(MTX34* pMtx, const QUAT* pQuat) {
    PSMTXQuat(*pMtx, *pQuat);
    return pMtx;
}

// @bug QUATSlerp macro changes this function name!
inline QUAT* C_QUATSlerp(QUAT* pOut, const QUAT* p1, const QUAT* p2, f32 t) {
    ::C_QUATSlerp(*p1, *p2, *pOut, t);
    return pOut;
}

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_types.h" */

namespace nw4r {
namespace math {

// Forward declarations
struct AABB;

enum IntersectionResult {
    INTERSECTION_NONE,
    INTERSECTION_1,
    INTERSECTION_2,

    INTERSECTION_LINE3_ON_PLANE = INTERSECTION_2,
    INTERSECTION_RAY3_ON_PLANE = INTERSECTION_2,
    INTERSECTION_SEGMENT3_ON_PLANE = INTERSECTION_2,

    INTERSECTION_OUTSIDE = 0,
    INTERSECTION_INSIDE,
    INTERSECTION_INTERSECT
};

bool IntersectionAABB(const AABB* pA, const AABB* pB);

/******************************************************************************
 *
 * Plane
 *
 ******************************************************************************/
struct PLANE {
    PLANE() {}

    f32 Test(const VEC3& rPoint) const {
        return d + VEC3Dot(&n, &rPoint);
    }

    void Set(const VEC3* p0, const VEC3* p1, const VEC3* p2);

    VEC3 n; // at 0x0
    f32 d;  // at 0xC
};

/******************************************************************************
 *
 * Axis-aligned bounding box
 *
 ******************************************************************************/
struct AABB {
    AABB() {}

    void Set(const VEC3* pPoints, unsigned int num);
    void Set(const AABB* pBox, const MTX34* pMtx);

    VEC3 min; // at 0x0
    VEC3 max; // at 0xC
};

/******************************************************************************
 *
 * Frustum
 *
 ******************************************************************************/
class FRUSTUM {
private:
    enum Point {
        POINT_NEAR_TL,
        POINT_NEAR_TR,
        POINT_NEAR_BR,
        POINT_NEAR_BL,

        POINT_FAR_TL,
        POINT_FAR_TR,
        POINT_FAR_BR,
        POINT_FAR_BL,

        POINT_MAX
    };

    enum Plane {
        PLANE_L,
        PLANE_R,
        PLANE_N,
        PLANE_F,
        PLANE_T,
        PLANE_B,

        PLANE_MAX
    };

public:
    void Set(f32 fovy, f32 aspect, f32 n, f32 f, const MTX34& rCamMtx);
    void Set(f32 t, f32 b, f32 l, f32 r, f32 n, f32 f, const MTX34& rCamMtx);

    IntersectionResult IntersectAABB_Ex(const AABB* pBox) const;

private:
    MTX34 mCamMtx;            // at 0x0
    PLANE mPlaneL;            // at 0x30
    PLANE mPlaneR;            // at 0x40
    PLANE mPlaneT;            // at 0x50
    PLANE mPlaneB;            // at 0x60
    f32 mNearZ;               // at 0x70
    f32 mFarZ;                // at 0x74
    AABB mBox;                // at 0x78
    PLANE mPlanes[PLANE_MAX]; // at 0x90
};

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_geometry.h" */
/* "libs/nw4r/include/nw4r/math.h" line 5 "nw4r/math/math_triangular.h" */
/* end "nw4r/math/math_triangular.h" */
/* "libs/nw4r/include/nw4r/math.h" line 6 "nw4r/math/math_types.h" */
/* end "nw4r/math/math_types.h" */

#endif
/* end "nw4r/math.h" */

#define PI 3.14159265f
#define DEG2RAD(x) ((x)*(PI/180.0f))

//Utility math functions
namespace ml{
    namespace math{

    inline float abs(float x){
        return (double)nw4r::math::FAbs(x);
    }

    inline float sqrt(float x){
        return nw4r::math::FSqrt(x);
    }

    inline float clamp(float x, float min, float max) {
        if (x < min) x = min;
        else if(x > max) x = max;
        return x;
    }

    inline float sin(float x){
        return nw4r::math::SinRad(x);
    }

    inline float cos(float x){
        return nw4r::math::CosRad(x);
    }

    inline void sincos(float x, float& sinX, float& cosX){
        sinX = sin(x);
        cosX = cos(x);
    }

    inline float asin(float x){
        x = clamp(x, -1.0f, 1.0f);
        return nw4r::math::AsinRad(x);
    }

    inline float acos(float x){
        x = clamp(x, -1.0f, 1.0f);
        return nw4r::math::AcosRad(x);
    }

    inline float atan2(float y, float x){
        return nw4r::math::Atan2FIdx(y, x);
    }

    inline float atan2Deg(float y, float x){
        return nw4r::math::Atan2Deg(y, x);
    }

    inline float atan2Rad(float y, float x){
        return nw4r::math::Atan2Rad(y, x);
    }

    inline float dot(const float* vec){
        return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    }

    } //namespace math
} //namespace ml
/* end "monolib/math/Utility.hpp" */
/* "libs/monolib/include/monolib/math/CVec3.hpp" line 3 "monolib/math/MathConstants.hpp" */
#pragma once

namespace ml{
    //General math constants
    extern const float pi;
    extern const float tau;
    extern const float halfpi;
    extern const float quarterpi;
    extern const float epsilon;
    extern const float rad2deg;
    extern const float deg2rad;
    extern const float huge;
    extern const float hugeminus;
} //namespace ml
/* end "monolib/math/MathConstants.hpp" */
/* "libs/monolib/include/monolib/math/CVec3.hpp" line 4 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    //Possibly inherits from nw4r VEC3?
    struct CVec3 {
        static CVec3 zero;
        static CVec3 unitX;
        static CVec3 unitY;
        static CVec3 unitZ;
        static CVec3 unit;

        CVec3(){}

        CVec3(float x, float y, float z){
            set(x, y, z);
        }

        CVec3(const CVec3& vec){
            set(vec.x, vec.y, vec.z);
        }

        //Conversion functions for converting to the SDK/NW4R vector types.
        operator Vec*(){
            return reinterpret_cast<Vec*>(this);
        }

        operator const Vec*() const {
            return reinterpret_cast<const Vec*>(this);
        }

        operator nw4r::math::VEC3*(){
            return reinterpret_cast<nw4r::math::VEC3*>(this);
        }

        operator const nw4r::math::VEC3*() const {
            return reinterpret_cast<const nw4r::math::VEC3*>(this);
        }

        void set(float x, float y, float z){
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void set(const CVec3& vec){
            x = vec.x;
            y = vec.y;
            z = vec.z;
        }

        void setZero(){
            *this = zero;
        }
        
        CVec3 operator-() const {
            return CVec3(-x, -y, -z);
        }

        CVec3 operator+(const CVec3& rhs) const {
            CVec3 out;
            //add(out, *this, rhs);
            nw4r::math::VEC3Add(out, *this, rhs);
            return out;
        }
        CVec3 operator-(const CVec3& rhs) const {
            CVec3 out;
            sub(out, *this, rhs);
            //nw4r::math::VEC3Sub(out, *this, rhs);
            return out;
        }
        CVec3 operator*(float x) const {
            CVec3 out;
            scale(out, *this, x);
            return out;
        }
        CVec3 operator/(float x) const {
            float r = 1/x;
            return *this * r;
        }

        CVec3& operator+=(const CVec3& rhs) {
            add(*this, *this, rhs);
            return *this;
        }
        CVec3& operator-=(const CVec3& rhs) {
            sub(*this, *this, rhs);
            return *this;
        }
        CVec3& operator*=(float x) {
            scale(*this, *this, x);
            return *this;
        }
        CVec3& operator/=(float x) {
            return *this *= (1/x);
        }

        bool operator==(const CVec3& vec) const {
            return x == vec.x && y == vec.y && z == vec.z;
        }

        bool operator!=(const CVec3& vec) const {
            return x != vec.x || y != vec.y || z != vec.z;
        }

        void normalize(){
            if(!isZero()) {
                normalizeSub();
            }else{
                set(0,0,1);
            }
        }

        //Unofficial
        void normalizeSub(){
            if(x*x + y*y + z*z == 0.0f){
                setZero();
            }else {
                PSVECNormalize(*this,*this);
            }
        }
        
        //Unofficial
        float magnitude() const {
            return x*x + y*y + z*z;
        }

        float getLength() const {
            return math::sqrt(magnitude());
        }

        bool isZero() const {
            //TODO: this can't be it, right???
            bool result = false;
            bool temp = false;
            if(math::abs(x) <= epsilon && math::abs(y) <= epsilon){
                temp = true;
            }
            if(temp && math::abs(z) <= epsilon) result = true;
            return result;
        }

        bool isErr() const;

        //TODO: properly figure out these asm inlines

        static float dot(const CVec3& lhs, const CVec3& rhs) {
            return nw4r::math::VEC3Dot(lhs, rhs);
        }
        
        static void add(CVec3& outVec, const CVec3& lhs, const CVec3& rhs){
            CVec3 temp;
            nw4r::math::VEC3Add(temp, lhs, rhs);
            outVec.set(temp);
        }

        static void sub(CVec3& outVec, const CVec3& lhs, const CVec3& rhs){
            CVec3 temp;
            nw4r::math::VEC3Sub(temp, lhs, rhs);
            outVec.set(temp);
        }

        static void scale(CVec3& outVec, const CVec3& vec, float scale){
            CVec3 temp;
            nw4r::math::VEC3Scale(temp, vec, scale);
            outVec.set(temp);
        }

        static void cross(CVec3& outVec, const CVec3& lhs, const CVec3& rhs){
            CVec3 temp;
            nw4r::math::VEC3Cross(temp, lhs, rhs);
            outVec.set(temp);
        }
        
        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float x;
            float y;
            float z;
        };

    };

} //namespace ml
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 4 "monolib/math/CMat34.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CMat34.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CMat34.hpp" line 3 "monolib/math/CQuat.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CQuat.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CQuat.hpp" line 3 "monolib/math/MathConstants.hpp" */
/* end "monolib/math/MathConstants.hpp" */
/* "libs/monolib/include/monolib/math/CQuat.hpp" line 4 "monolib/math/Utility.hpp" */
/* end "monolib/math/Utility.hpp" */
/* "libs/monolib/include/monolib/math/CQuat.hpp" line 5 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CQuat{
        CQuat(){}

        CQuat(float x, float y, float z, float w){
            set(x,y,z,w);
        }

        operator Quaternion*(){
            return reinterpret_cast<Quaternion*>(this);
        }

        operator const Quaternion*() const{
            return reinterpret_cast<const Quaternion*>(this);
        }

        CQuat& operator*=(const CQuat& other){
            PSQUATMultiply(*this, other, *this);
            return *this;
        }

        inline void set(float x, float y, float z, float w){
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        inline void setIdentity(){
            set(0,0,0,1);
        }
  
        //Sets the quaternion from the given euler angle, following the 3-2-1 conversion.
        inline void setRotXYZ(const CVec3* angle){
            float x = angle->x * 0.5f;
            float y = angle->y * 0.5f;
            float z = angle->z * 0.5f;
            float sinX = math::sin(x);
            float cosX = math::cos(x);
            float sinY = math::sin(y);
            float cosY = math::cos(y);
            float sinZ = math::sin(z);
            float cosZ = math::cos(z);

            this->x = cosZ*(sinX*cosY) - sinZ*(cosX*sinY);
            this->y = cosZ*(cosX*sinY) + sinZ*(sinX*cosY);
            this->z = sinZ*(cosX*cosY) - cosZ*(sinX*sinY);
            this->w = cosZ*(cosX*cosY) + sinZ*(sinX*sinY);
        }

        //Converts this quaternion to euler angles, storing the result in the given vector.
        inline void getRotXYZ(CVec3* result) const {
            //So many variables :p
            float twoX = x + x;
            float twoY = y + y;
            float twoZ = z + z;

            float twoXZ = x * twoZ;
            float twoYW = w * twoY;

            //NOTE: Unnecessary clamp
            float input = -(twoXZ - twoYW);
            if(input >= 1) input = 1;
            else if(input <= -1) input = -1;
            //y = asin(-(2xz - 2yw))
            float angle = math::asin(input);
            result->y = angle;

            float twoX2 = x * twoX;
            float twoXY = x * twoY;
            float twoZ2 = z * twoZ;
            float twoZW = w * twoZ;
                    
            if (angle < halfpi) {
                if (angle > -halfpi) {
                    //-pi/2 < angle < pi/2
                    float twoY2 = y * twoY;
                    float twoYZ = y * twoZ;
                    float twoXW = w * twoX;
                    //x = atan2(2yz + 2xw, 1 - (2x^2 + 2y^2))
                    result->x = math::atan2Rad(twoYZ + twoXW, 1.0f - (twoX2 + twoY2));
                    //z = atan2(2xy + 2zw, 1 - (2y^2 + 2z^2))
                    result->z = math::atan2Rad(twoXY + twoZW, 1.0f - (twoY2 + twoZ2));
                }else{
                    //angle > pi/2
                    //x = -atan2(2xy - 2zw, 1 - (2x^2 + 2z^2))
                    result->x = -math::atan2Rad(twoXY - twoZW, 1.0f - (twoX2 + twoZ2));
                    result->z = 0;
                }
            }else{
                //angle < -pi/2
                //x = atan2(2xy - 2zw, 1 - (2x^2 + 2z^2))
                result->x = math::atan2Rad(twoXY - twoZW, 1.0f - (twoX2 + twoZ2));
                result->z = 0;
            }
        }

        static void slerp(CQuat& outQuat, const CQuat& a, const CQuat& b, float t);
        void setRotZXY(const CVec3* angle);
        void getRotZXY(CVec3* result) const;


        struct{
        float x;
        float y;
        float z;
        float w;
        };

        static CQuat zero;
        static CQuat identity;

    };

} //namespace ml
/* end "monolib/math/CQuat.hpp" */
/* "libs/monolib/include/monolib/math/CMat34.hpp" line 4 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CMat34{
        CMat34(){}

        CMat34(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23){
            set(
                m00,m01,m02,m03,
                m10,m11,m12,m13,
                m20,m21,m22,m23
            );
        }

        void set(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23){
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
        }

        operator nw4r::math::MTX34*(){
            return reinterpret_cast<nw4r::math::MTX34*>(this);
        }

        operator const nw4r::math::MTX34*() const {
            return reinterpret_cast<const nw4r::math::MTX34*>(this);
        }

        //TODO: can this be included without interfering with the above operators?
        /*
        operator Mtx*(){
            return &m;
        }

        operator const Mtx*() const {
            return &m;
        }
        */

        CMat34 operator*(CMat34& rhs) const {
            CMat34 mat;
            mul(mat, *this, rhs);
            return mat;
        }

        void mul(CVec3& outVec, const CVec3& vec) const {
            PSMTXMultVec(mtx, vec, outVec);
        }


        static void mul(CMat34& outMat, const CMat34& mat1, const CMat34& mat2){
            PSMTXConcat(mat2.mtx, mat1.mtx, outMat.mtx);
        }

        void setUnit(){
            PSMTXIdentity(mtx);
        }

        void setScale(float x, float y, float z){
            set(
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0
            );
        }

        void setScale(const CVec3& scale){
            setScale(scale.x, scale.y, scale.z);
        }

        void setRotX(float x){
            float sinX, cosX;
            math::sincos(x, sinX, cosX);

            set(
            1, 0,    0,     0,
            0, cosX, -sinX, 0,
            0, sinX, cosX,  0
            );
        }

        void setRotY(float y){
            float sinY, cosY;
            math::sincos(y, sinY, cosY);

            set(
            cosY,  0, sinY, 0,
            0,     1, 0,    0,
            -sinY, 0, cosY, 0
            );
        }

        void setRotZ(float z){
            float sinZ, cosZ;
            math::sincos(z, sinZ, cosZ);

            set(
            cosZ, -sinZ, 0, 0,
            sinZ, cosZ,  0, 0,
            0,    0,     1, 0
            );
        }

        void addRotX(float x){
            CMat34 mat;
            mat.setRotX(x);
            mul(*this, *this, mat);
        }

        void addRotY(float y){
            CMat34 mat;
            mat.setRotY(y);
            mul(*this, *this, mat);
        }

        void addRotZ(float z){
            CMat34 mat;
            mat.setRotZ(z);
            mul(*this, *this, mat);
        }

        void getRotQuat(CQuat& quat) const{
            CQuat temp;
            C_QUATMtx(temp, mtx);
            quat = temp;
        }

        void setRotQuat(const CQuat& quat){
            PSMTXQuat(mtx, quat);
        }

        void setRotXYZ(const CVec3& angle){
            nw4r::math::MTX34RotXYZRad(*this, angle.x, angle.y, angle.z);
        }

        void setRotZXY(const CVec3& angle){
            setRotZ(angle.z);
            addRotX(angle.x);
            addRotY(angle.y);
        }

        void invert(CMat34* outMat){
            PSMTXInverse(mtx, outMat->mtx);
        }

        CVec3 getTranslation(){
            return CVec3(m[0][3], m[1][3], m[2][3]);
        }

        void addTranslation(const CVec3& vec){
            m[0][3] += vec.x;
            m[1][3] += vec.y;
            m[2][3] += vec.z;
        }


        void replaceTranslation(const CVec3& vec){
            m[0][3] = vec.x;
            m[1][3] = vec.y;
            m[2][3] = vec.z;
        }

        void getRotAxis(CVec3& vec, float* outAngle) const;
        bool getRotXYZ(CVec3& vec) const;
        bool getRotZXY(CVec3& vec) const;
        bool getRotZYX(CVec3& vec) const;

        union {
            float m[3][4];
            Mtx mtx;
        };

        static CMat34 zero;
        static CMat34 identity;
    };
} //namespace ml
/* end "monolib/math/CMat34.hpp" */
/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 5 "monolib/math/CQuat.hpp" */
/* end "monolib/math/CQuat.hpp" */

namespace ml{
    struct CAttrTransform{
        enum Flags{
            FLAG_0 = 1 << 0,
            FLAG_1 = 1 << 1,
            FLAG_2 = 1 << 2,
            FLAG_USE_ZXY = 1 << 3
        };

        CAttrTransform(){
            clear();
        }

        void clear();
        void update();

        CVec3 mPos; //0x0
        CVec3 mRot; //0xC
        CMat34 mMtx1; //0x18
        CMat34 mLocalMat; //0x48
        CMat34 mLocalMatInv; //0x78
        CQuat unkA8;
        CVec3 mPrevPos; //0xB8
        CVec3 mPrevRot; //0xC4
        CQuat mLocalQuat; //0xD0
        u32 mFlags; //0xE0
    };
} //namespace ml
/* end "monolib/math/CAttrTransform.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 3 "monolib/math/CCamUtil.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CCamUtil.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */

namespace ml {
    struct CCamUtil {
        static void getXYZ2ZXY(CVec3& outVec, const CVec3& vec);
    };
} //namespace ml
/* end "monolib/math/CCamUtil.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 4 "monolib/math/CCol3.hpp" */
#pragma once

namespace ml {
    struct CCol3{
        CCol3(){}
        CCol3(float r, float g, float b){
            set(r, g, b);
        }

        void set(float r, float g, float b){
            this->r = r;
            this->g = g;
            this->b = b;
        }

        void clamp(float min, float max){
            if(r > max) r = max;
            else if(r < min) r = min;
            if(g > max) g = max;
            else if(g < min) g = min;
            if(b > max) b = max;
            else if(b < min) b = min;
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float r;
            float g;
            float b;
        };

        static CCol3 white;
        static CCol3 gray;
        static CCol3 black;
        static CCol3 red;
        static CCol3 green;
        static CCol3 blue;
        static CCol3 yellow;
        static CCol3 cyan;
        static CCol3 magenta;
        static CCol3 salmon;
        static CCol3 orange;
    };
} //namespace ml
/* end "monolib/math/CCol3.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 5 "monolib/math/CCol4.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CCol4.hpp" line 2 "types.h" */
/* end "types.h" */


#define COLOR_TO_U32(r, g, b, a) (((u8)(255 * r) << 24) | ((u8)(255 * g) << 16) | ((u8)(255 * b) << 8) | (u8)(255 * a))

namespace ml {
    
    struct CCol4{
        CCol4(){}
        CCol4(float r, float g, float b, float a){
            set(r,g,b,a);
        }

        void set(float r, float g, float b, float a){
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        void clamp(float min, float max){
            if(r > max) r = max;
            else if(r < min) r = min;
            if(g > max) g = max;
            else if(g < min) g = min;
            if(b > max) b = max;
            else if(b < min) b = min;
            if(a > max) a = max;
            else if(a < min) a = min;
        }

        u32 toU32() const {
            return COLOR_TO_U32(r, g, b, a);
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float r;
            float g;
            float b;
            float a;
        };

        static CCol4 white;
        static CCol4 gray;
        static CCol4 black;
        static CCol4 red;
        static CCol4 green;
        static CCol4 blue;
        static CCol4 yellow;
        static CCol4 cyan;
        static CCol4 magenta;
        static CCol4 salmon;
        static CCol4 orange;
        static CCol4 zero;

    };

} //namespace ml
/* end "monolib/math/CCol4.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 6 "monolib/math/CFrustum.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 3 "monolib/math/CMat34.hpp" */
/* end "monolib/math/CMat34.hpp" */
/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 4 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 5 "monolib/math/CPlane.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CPlane.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */

namespace ml {
    struct CPlane {
        CPlane(){
        }
        CPlane(const CVec3& pos, const CVec3& p1, const CVec3& p2){
            set(pos, p1, p2);
        }

        CPlane* set(const CVec3& pos, const CVec3& p1, const CVec3& p2);
        static void getCross(CVec3& outVec, const CPlane& plane, const CVec3& rayOrigin, const CVec3& rayDir);

        void normalize(){
            mNormal.normalizeSub();
        }

        CVec3 convertRayToNormal(const CVec3& rayStartPos, const CVec3& rayEndPos) const {
            CVec3 normal = rayEndPos - rayStartPos;
            normal.normalizeSub();
            return normal;
        }

        //Sets this plane from the given ray start/end position.
        void set(const CVec3& rayStartPos, const CVec3& rayEndPos){
            mNormal = convertRayToNormal(rayStartPos, rayEndPos);
            mDist = -CVec3::dot(rayStartPos, mNormal);
        }

        float getPointDistance(const CVec3& pos) const {
            return CVec3::dot(pos, mNormal) + mDist;
        }
        
        bool isWithinDistance(const CVec3& vec, float distance) const {
            if(getPointDistance(vec) < distance) return true;
            else return false;
        }

        bool isOnNegativeSide(const CVec3& vec) const {
            if(getPointDistance(vec) < 0) return true;
            else return false;
        }

        bool isOnPositiveSide(const CVec3& vec) const {
            if(getPointDistance(vec) >= 0) return true;
            else return false;
        }

        //Determines if the points are on different sides of the plane.
        bool isDifferentSide(const CVec3& vec1, const CVec3& vec2) const {
            float f0 = getPointDistance(vec2);
            float f1 = getPointDistance(vec1);
            
            bool r3 = f1 >= 0;
            bool r0 = f0 >= 0;
            //TODO: is this some weird inline for calculating if only one of two conditions are true?
            return r3 ^ r0;
        }

        CVec3 mNormal; //0x0
        float mDist; //0x4
    };

} //namespace ml
/* end "monolib/math/CPlane.hpp" */

//TODO: idk if this belongs here in monolib or in the scene code. There's no evidence of it in XCX
namespace ml{

    struct CFrustum{
        u8 unk0[0xCC];
        ml::CMat34 unkCC;
        u8 unkFC[0x10];
        ml::CVec3 unk10C;
        u8 unk118[0x258 - 0x118];
        ml::CPlane unk248[6];
    };

} //namespace ml
/* end "monolib/math/CFrustum.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 7 "monolib/math/CMat33.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CMat33.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CMat33.hpp" line 3 "monolib/math/Utility.hpp" */
/* end "monolib/math/Utility.hpp" */
/* "libs/monolib/include/monolib/math/CMat33.hpp" line 4 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CMat33{
        CMat33(){}

        CMat33(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22){
            set(m00, m01, m02, m10, m11, m12, m20, m21, m22);
        }

        void set(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22){
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
        }

        void setRotXYZ(const CVec3& angle){
            float sinX = math::sin(angle.x);
            float cosX = math::cos(angle.x);
            float sinY = math::sin(angle.y);
            float cosY = math::cos(angle.y);
            float sinZ = math::sin(angle.z);
            float cosZ = math::cos(angle.z);

            set(
            cosY*cosZ, sinX*sinY*cosZ - cosX*sinZ, cosX*sinY*cosZ + sinX*sinZ,
            cosY*sinZ, sinX*sinY*sinZ + cosX*cosZ, cosX*sinY*sinZ - sinX*cosZ,
            -sinY,     sinX*cosY,                  cosX*cosY
            );
        }

        void getRotZXY(CVec3& outVec){
            float angle = math::asin(-m[1][2]);
            outVec.x = angle;

            if(angle < halfpi){
                if(angle > -halfpi){
                    //-pi/2 < angle < pi/2
                    outVec.z = math::atan2Rad(m[1][0], m[1][1]);
                    outVec.y = math::atan2Rad(m[0][2], m[2][2]);
                }else{
                    //angle < -pi/2
                    outVec.z = -math::atan2Rad(m[2][0], m[0][0]);
                    outVec.y = 0;
                }
            }else{
                //angle > pi/2
                outVec.z = math::atan2Rad(m[2][0], m[0][0]);
                outVec.y = 0;
            }
        }

        float m[3][3];
    };
} //namespace ml
/* end "monolib/math/CMat33.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 8 "monolib/math/CMat34.hpp" */
/* end "monolib/math/CMat34.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 9 "monolib/math/CMat44.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CMat44.hpp" line 2 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CMat44{
        CMat44(){}

        CMat44(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33){
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }

        union {
            float m[4][4];
            Mtx44 mtx;
        };
    
        static CMat44 zero;
        static CMat44 identity;
    };
} //namespace ml
/* end "monolib/math/CMat44.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 10 "monolib/math/CPlane.hpp" */
/* end "monolib/math/CPlane.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 11 "monolib/math/CPnt16.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CPnt16.hpp" line 2 "types.h" */
/* end "types.h" */

namespace ml {
    struct CPnt16{
        CPnt16(){}

        CPnt16(s16 x, s16 y){
            set(x,y);
        }

        void set(s16 x, s16 y){
            this->x = x;
            this->y = y;
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lhz/sth. */
        struct{
            s16 x;
            s16 y;
        };
    };
} //namespace ml
/* end "monolib/math/CPnt16.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 12 "monolib/math/CQuat.hpp" */
/* end "monolib/math/CQuat.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 13 "monolib/math/CRect16.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CRect16.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/math/CRect16.hpp" line 3 "monolib/math/CPnt16.hpp" */
/* end "monolib/math/CPnt16.hpp" */

namespace ml {
    struct CRect16 {
        CRect16(){
        }
    
        CRect16(s16 x, s16 y, s16 width, s16 height){
            set(x, y, width, height);
        }

        CRect16(const CPnt16& pos, const CPnt16& size){
            set(pos, size);
        }

        void set(s16 x, s16 y, s16 width, s16 height){
            mPos.x = x;
            mPos.y = y;
            mSize.x = width;
            mSize.y = height;
        }

        void set(const CPnt16& pos, const CPnt16& size){
            mPos.x = pos.x;
            mPos.y = pos.y;
            mSize.x = size.x;
            mSize.y = size.y;
        }
    
        bool isInside(const CPnt16& point) const;

        CPnt16 mPos; //0x0
        CPnt16 mSize; //0x4
    };
} //namespace ml
/* end "monolib/math/CRect16.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 14 "monolib/math/CVec2.hpp" */
#pragma once

/* Not known to be used yet, but it's here anyways just in case they did end up using it (it exists
in XCX, so it probably at least existed). It also doesn't hurt to have anyway for modding and stuff. */
namespace ml {
    struct CVec2 {

        CVec2(){}

        CVec2(float x, float y){
            set(x, y);
        }

        CVec2(const CVec2& vec){
            set(vec.x, vec.y);
        }

        void set(float x, float y){
            this->x = x;
            this->y = y;
        }

        void set(const CVec2& vec){
            x = vec.x;
            y = vec.y;
        }

        bool operator==(const CVec2& vec) const {
            return x == vec.x && y == vec.y;
        }

        bool operator!=(const CVec2& vec) const {
            return x != vec.x || y != vec.y;
        }
        
        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float x;
            float y;
        };

    };

} //namespace ml
/* end "monolib/math/CVec2.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 15 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 16 "monolib/math/CVec4.hpp" */
#pragma once

namespace ml {
    struct CVec4{
        static CVec4 zero;
        static CVec4 unitX;
        static CVec4 unitY;
        static CVec4 unitZ;
        static CVec4 unit;

        CVec4() {}

        CVec4(float x, float y, float z, float w){
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float x;
            float y;
            float z;
            float w;
        };

    };
} //namespace ml
/* end "monolib/math/CVec4.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 17 "monolib/math/Main.hpp" */
#pragma once

namespace ml{
    namespace math{
        void initialize();
    } //namespace math
} //namespace ml
/* end "monolib/math/Main.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 18 "monolib/math/FloatUtils.hpp" */
#pragma once

#define FLOAT_LARGE 1e14f
#define FLOAT_SMALL -1e14f
#define FLOAT_MAX_HEX 0xD3D3D3D3
#define FLOAT_MAX_HEX_NEG 0xF3F3F3F3
#define FLOAT_NAN 0xFFFFFFFF
#define FLOAT_TO_HEX(f) (*(u32*)&f)

namespace ml{
    namespace math{
        bool isErrFloat(float f);
    } //namespace math
} //namespace ml
/* end "monolib/math/FloatUtils.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 19 "monolib/math/MathConstants.hpp" */
/* end "monolib/math/MathConstants.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 20 "monolib/math/MTRand.hpp" */
//Original copyright comments:

// Mersenne Twister random number generator -- a C++ class MTRand
// Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus
// Richard J. Wagner  v1.1  28 September 2009  wagnerr@umich.edu

// The Mersenne Twister is an algorithm for generating random numbers.  It
// was designed with consideration of the flaws in various other generators.
// The period, 2^19937-1, and the order of equidistribution, 623 dimensions,
// are far greater.  The generator is also fast; it avoids multiplication and
// division, and it benefits from caches and pipelines.  For more information
// see the inventors' web page at
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html

// Reference
// M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
// Equidistributed Uniform Pseudo-Random Number Generator", ACM Transactions on
// Modeling and Computer Simulation, Vol. 8, No. 1, January 1998, pp 3-30.

// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
// Copyright (C) 2000 - 2009, Richard J. Wagner
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//   3. The names of its contributors may not be used to endorse or promote 
//      products derived from this software without specific prior written 
//      permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// The original code included the following notice:
// 
//     When you use this, send an email to: m-mat@math.sci.hiroshima-u.ac.jp
//     with an appropriate reference to your work.
// 
// It would be nice to CC: wagnerr@umich.edu and Cokus@math.washington.edu
// when you write.


/* Monolithsoft made slight modifications from the most commonly found version,
most notably changing function names.
This might be the version this is based on: https://gist.github.com/yuikns/10017640 */

#pragma once

/* "libs/monolib/include/monolib/math/MTRand.hpp" line 65 "types.h" */
/* end "types.h" */

namespace ml{

//Probably uses a singleton template

class MTRand{
protected:
    static const int N = 624; //length
    static const int M = 397; //period
    u32 state[N]; //twister
    int left; //0x9C0
    BOOL initialized; //0x9C4
    u32* pNext; //0x9C8
    u32 unk9D0;

public:
    static MTRand* getInstance();
    void srand(u32);
    void nextMt();
    u32 rand32();
    u32 rand31();
    float randFloat();
    float randFloat1();

    inline u32 rand(){
        return rand31();
    }

    inline u32 rand(int max){
        if(max != 0){
            int randVal = rand();
            return randVal % max;
        }else{
            return 0;
        }
    }

    inline u32 rand(int min, int max){
        int range = max - min;
        int result = 0;

        if(range != 0){
            int randVal = rand();
            result = (randVal % range);
        }else{
            result = 0;
        }

        return min + result; //wtf? why not just add it before?
    }

protected:
    inline u32 hiBit(u32 u) { return u & 0x80000000; }
    inline u32 loBit(u32 u) { return u & 0x00000001; }
    inline u32 loBits(u32 u) { return u & 0x7fffffff; }
    inline u32 mixBits(u32 u, u32 v) { return hiBit(u) | loBits(v); }
    inline u32 magic(u32 u) { return loBit(u) ? 0x9908b0dfUL : 0x0UL; }
    inline u32 twist(u32 m, u32 s0, u32 s1) {
        const u32 y = mixBits(s0, s1);
        const u32 mask = magic(s1);
        return m ^ (mask ^ (y >> 1));
    }
};

} //namespace ml
/* end "monolib/math/MTRand.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 21 "monolib/math/Random.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/Random.hpp" line 2 "types.h" */
/* end "types.h" */

namespace ml{
    namespace math{
        void mtInit(u32 seed);
        int mtRand();
        int mtRand(int max);
        int mtRand(int min,int max);
    } //namespace math
} //namespace ml
/* end "monolib/math/Random.hpp" */
/* end "monolib/math.hpp" */

namespace cf {

class CfCamEvent {
public:
    u8 unk0[0x28];
    ml::CVec3 unk28;
    u8 unk34[0x25C - 0x34];
    ml::CVec3 unk25C;
    ml::CVec3 mLookat;
    ml::CVec3 unk274;
    u8 unk280[0x284 - 0x280];
    f32 unk284;
};

} // namespace cf
/* end "kyoshin/cf/CfCamDirectionIntf.hpp" */
/* end "kyoshin/cf/CfCamTargetIntf.hpp" */

extern "C" {
    void func_80074488(ml::CVec3* out, ml::CVec3* lookat, ml::CVec3 const* dir, f32 unk284);
    extern const f32 lbl_eu_80666268;
}

using namespace ml;

namespace cf {

extern "C" {

void CfCamTargetIntf_update(void*) {
}

void CfCamTargetIntf_setPos(void*, CfCamEvent* camEvent, CVec3 const* pos) {
    camEvent->unk28 = *pos;
    camEvent->unk274 = *pos;
    camEvent->unk25C.z = lbl_eu_80666268;
}

void CfCamTargetIntf_setDir(void*, CfCamEvent* camEvent, CVec3 const* dir) {
    CVec3 result;
    func_80074488(&result, &camEvent->mLookat, dir, camEvent->unk284);
    camEvent->unk274 = result;
    camEvent->unk25C = *dir;
}

void CfCamTargetIntf_setLookat(void*, CfCamEvent* camEvent, CVec3 const* lookat) {
    camEvent->mLookat = *lookat;
    camEvent->unk25C.z = lbl_eu_80666268;
}

} // extern "C"

} // namespace cf
