/* "libs/RVL_SDK/src/revolution/mtx/mtx.c" line 0 "revolution/MTX.h" */
#ifndef RVL_SDK_PUBLIC_MTX_H
#define RVL_SDK_PUBLIC_MTX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/MTX.h" line 6 "revolution/MTX/mtx.h" */
#ifndef RVL_SDK_MTX_MTX_H
#define RVL_SDK_MTX_MTX_H
/* "libs/RVL_SDK/include/revolution/MTX/mtx.h" line 2 "types.h" */
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

/* "libs/RVL_SDK/src/revolution/mtx/mtx.c" line 2 "math.h" */
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

#define MTXDegToRad(a) ((a)*0.01745329252f)

static f32 Unit01[] = { 0.0f, 1.0f };

DECOMP_FORCELITERAL(mtx_c, 1.0f, 0.0f);

//unused
void C_MTXIdentity(){
}

void PSMTXIdentity(register Mtx m) {
    register f32 c_zero = 0.0f;
    register f32 c_one = 1.0f;
    register f32 c_01;
    register f32 c_10;

    ASM (
        psq_st     c_zero, 8(m),  0, 0   // m[0][2] = 0, m[0][3] = 0;
        ps_merge01 c_01, c_zero,  c_one  // c_01 = (0,0)
        psq_st     c_zero, 24(m), 0, 0   // m[]
        ps_merge10 c_10, c_one,   c_zero // c_10 = (0,0)
        psq_st     c_zero, 32(m), 0, 0
        psq_st     c_01, 16(m),   0, 0
        psq_st     c_10, 0(m),    0, 0
        psq_st     c_10, 40(m),   0, 0
    )
}

//unused
void C_MTXCopy(){
}

void PSMTXCopy(const Mtx src, Mtx dst) {
    dst[0][0] = src[0][0];
    dst[0][1] = src[0][1];
    dst[0][2] = src[0][2];
    dst[0][3] = src[0][3];
    dst[1][0] = src[1][0];
    dst[1][1] = src[1][1];
    dst[1][2] = src[1][2];
    dst[1][3] = src[1][3];
    dst[2][0] = src[2][0];
    dst[2][1] = src[2][1];
    dst[2][2] = src[2][2];
    dst[2][3] = src[2][3];
}

//unused
void C_MTXConcat(){
}

asm void PSMTXConcat(const register Mtx mA, const register Mtx mB,
                     register Mtx mAB) {
    // clang-format off
    nofralloc
    stwu r1, -64(r1)
    psq_l fp0, 0(mA), 0, 0
    stfd fp14, 8(r1)
    psq_l fp6, 0(mB), 0, 0
    addis r6, 0, Unit01 @ha
    psq_l fp7, 8(mB), 0, 0
    stfd fp15, 16(r1)
    addi r6, r6, Unit01 @l
    stfd fp31, 40(r1)
    psq_l fp8, 16(mB), 0, 0
    ps_muls0 fp12, fp6, fp0
    psq_l fp2, 16(mA), 0, 0
    ps_muls0 fp13, fp7, fp0
    psq_l fp31, 0(r6), 0, 0
    ps_muls0 fp14, fp6, fp2
    psq_l fp9, 24(mB), 0, 0
    ps_muls0 fp15, fp7, fp2
    psq_l fp1, 8(mA), 0, 0
    ps_madds1 fp12, fp8, fp0, fp12
    psq_l fp3, 24(mA), 0, 0
    ps_madds1 fp14, fp8, fp2, fp14
    psq_l fp10, 32(mB), 0, 0
    ps_madds1 fp13, fp9, fp0, fp13
    psq_l fp11, 40(mB), 0, 0
    ps_madds1 fp15, fp9, fp2, fp15
    psq_l fp4, 32(mA), 0, 0
    psq_l fp5, 40(mA), 0, 0
    ps_madds0 fp12, fp10, fp1, fp12
    ps_madds0 fp13, fp11, fp1, fp13
    ps_madds0 fp14, fp10, fp3, fp14
    ps_madds0 fp15, fp11, fp3, fp15
    psq_st fp12, 0(mAB), 0, 0
    ps_muls0 fp2, fp6, fp4
    ps_madds1 fp13, fp31, fp1, fp13
    ps_muls0 fp0, fp7, fp4
    psq_st fp14, 16(mAB), 0, 0
    ps_madds1 fp15, fp31, fp3, fp15
    psq_st fp13, 8(mAB), 0, 0
    ps_madds1 fp2, fp8, fp4, fp2
    ps_madds1 fp0, fp9, fp4, fp0
    ps_madds0 fp2, fp10, fp5, fp2
    lfd fp14, 8(r1)
    psq_st fp15, 24(mAB), 0, 0
    ps_madds0 fp0, fp11, fp5, fp0
    psq_st fp2, 32(mAB), 0, 0
    ps_madds1 fp0, fp31, fp5, fp0
    lfd fp15, 16(r1)
    psq_st fp0, 40(mAB), 0, 0
    lfd fp31, 40(r1)
    addi r1, r1, 64
    blr
    // clang-format on
}

//unused
void C_MTXConcatArray(){
}

void PSMTXConcatArray(const register Mtx mtx1, const register Mtx mtx2,
                      register Mtx mtx3, register u32 vv4) {
    register f32 va0, va1, va2, va3, va4, va5;
    register f32 vb0, vb1, vb2, vb3, vb4, vb5;
    register f32 vd0, vd1, vd2, vd3, vd4, vd5;
    register f32 u01;
    register f32* u01Ptr = Unit01;

    ASM (
        psq_l va0, 0(mtx1), 0, 0
        psq_l va1, 8(mtx1), 0, 0
        psq_l va2, 16(mtx1), 0, 0
        psq_l va3, 24(mtx1), 0, 0
        subi vv4, vv4, 1
        psq_l va4, 32(mtx1), 0, 0
        psq_l va5, 40(mtx1), 0, 0
        mtctr vv4
        psq_l u01, 0(u01Ptr), 0, 0
        psq_l vb0, 0(mtx2), 0, 0
        psq_l vb2, 16(mtx2), 0, 0
        ps_muls0 vd0, vb0, va0
        ps_muls0 vd2, vb0, va2
        ps_muls0 vd4, vb0, va4
        psq_l vb4, 32(mtx2), 0, 0
        ps_madds1 vd0, vb2, va0, vd0
        ps_madds1 vd2, vb2, va2, vd2
        ps_madds1 vd4, vb2, va4, vd4
        psq_l vb1, 8(mtx2), 0, 0
        ps_madds0 vd0, vb4, va1, vd0
        ps_madds0 vd2, vb4, va3, vd2
        ps_madds0 vd4, vb4, va5, vd4
        psq_l vb3, 24(mtx2), 0, 0
        psq_st vd0, 0(mtx3), 0, 0
        ps_muls0 vd1, vb1, va0
        ps_muls0 vd3, vb1, va2
        ps_muls0 vd5, vb1, va4
        psq_l vb5, 40(mtx2), 0, 0
        psq_st vd2, 16(mtx3), 0, 0
        ps_madds1 vd1, vb3, va0, vd1
        ps_madds1 vd3, vb3, va2, vd3
        ps_madds1 vd5, vb3, va4, vd5
_loop:
        addi mtx2, mtx2, sizeof(Mtx)
        ps_madds0 vd1, vb5, va1, vd1
        ps_madds0 vd3, vb5, va3, vd3
        ps_madds0 vd5, vb5, va5, vd5
        psq_l vb0, 0(mtx2), 0, 0
        psq_st vd4, 32(mtx3), 0, 0
        ps_madd vd1, u01, va1, vd1
        ps_madd vd3, u01, va3, vd3
        ps_madd vd5, u01, va5, vd5
        psq_l vb2, 16(mtx2), 0, 0
        psq_st vd1, 8(mtx3), 0, 0
        ps_muls0 vd0, vb0, va0
        ps_muls0 vd2, vb0, va2
        ps_muls0 vd4, vb0, va4
        psq_l vb4, 32(mtx2), 0, 0
        psq_st vd3, 24(mtx3), 0, 0
        ps_madds1 vd0, vb2, va0, vd0
        ps_madds1 vd2, vb2, va2, vd2
        ps_madds1 vd4, vb2, va4, vd4
        psq_l vb1, 8(mtx2), 0, 0
        psq_st vd5, 40(mtx3), 0, 0
        addi mtx3, mtx3, sizeof(Mtx)
        ps_madds0 vd0, vb4, va1, vd0
        ps_madds0 vd2, vb4, va3, vd2
        ps_madds0 vd4, vb4, va5, vd4
        psq_l vb3, 24(mtx2), 0, 0
        psq_st vd0, 0(mtx3), 0, 0
        ps_muls0 vd1, vb1, va0
        ps_muls0 vd3, vb1, va2
        ps_muls0 vd5, vb1, va4
        psq_l vb5, 40(mtx2), 0, 0
        psq_st vd2, 16(mtx3), 0, 0
        ps_madds1 vd1, vb3, va0, vd1
        ps_madds1 vd3, vb3, va2, vd3
        ps_madds1 vd5, vb3, va4, vd5
        bdnz _loop
        psq_st vd4, 32(mtx3), 0, 0
        ps_madds0 vd1, vb5, va1, vd1
        ps_madds0 vd3, vb5, va3, vd3
        ps_madds0 vd5, vb5, va5, vd5
        ps_madd vd1, u01, va1, vd1
        ps_madd vd3, u01, va3, vd3
        ps_madd vd5, u01, va5, vd5
        psq_st vd1, 8(mtx3), 0, 0
        psq_st vd3, 24(mtx3), 0, 0
        psq_st vd5, 40(mtx3), 0, 0
    )
}

//unused
void C_MTXTranspose(){
}

void PSMTXTranspose(const register Mtx src, register Mtx xPose) {
    register f32 c_zero = 0.0f;
    register f32 row0a, row1a, row0b, row1b;
    register f32 trns0, trns1, trns2;

    ASM (
        psq_l       row0a, 0(src),  0, 0
        stfs        c_zero, 44(xPose)
        psq_l       row1a, 16(src), 0, 0
        ps_merge00  trns0, row0a, row1a
        psq_l       row0b, 8(src),  1, 0
        ps_merge11  trns1, row0a, row1a
        psq_l       row1b, 24(src), 1, 0
        psq_st      trns0, 0(xPose),  0, 0
        psq_l       row0a, 32(src), 0, 0
        ps_merge00  trns2, row0b, row1b
        psq_st      trns1, 16(xPose), 0, 0
        ps_merge00  trns0, row0a, c_zero
        psq_st      trns2, 32(xPose), 0, 0
        ps_merge10  trns1, row0a, c_zero
        psq_st      trns0, 8(xPose),  0, 0
        lfs         row0b, 40(src)
        psq_st      trns1, 24(xPose), 0, 0
        stfs        row0b, 40(xPose)
    )
}

//unused
void C_MTXInverse(){
}

asm u32 PSMTXInverse(const register Mtx src, register Mtx inv) {
    // clang-format off
    nofralloc
    psq_l fp0, 0(src), 1, 0
    psq_l fp1, 4(src), 0, 0
    psq_l fp2, 16(src), 1, 0
    ps_merge10 fp6, fp1, fp0
    psq_l fp3, 20(src), 0, 0
    psq_l fp4, 32(src), 1, 0
    ps_merge10 fp7, fp3, fp2
    psq_l fp5, 36(src), 0, 0
    ps_mul fp11, fp3, fp6
    ps_merge10 fp8, fp5, fp4
    ps_mul fp13, fp5, fp7
    ps_msub fp11, fp1, fp7, fp11
    ps_mul fp12, fp1, fp8
    ps_msub fp13, fp3, fp8, fp13
    ps_mul fp10, fp3, fp4
    ps_msub fp12, fp5, fp6, fp12
    ps_mul fp7, fp0, fp13
    ps_mul fp9, fp0, fp5
    ps_mul fp8, fp1, fp2
    ps_madd fp7, fp2, fp12, fp7
    ps_sub fp6, fp6, fp6
    ps_msub fp10, fp2, fp5, fp10
    ps_madd fp7, fp4, fp11, fp7
    ps_msub fp9, fp1, fp4, fp9
    ps_msub fp8, fp0, fp3, fp8
    ps_cmpo0 cr0, fp7, fp6
    bne loc0
    addi r3, 0, 0
    blr
loc0:
    fres fp0, fp7
    ps_add fp6, fp0, fp0
    ps_mul fp5, fp7, fp0
    ps_nmsub fp0, fp0, fp5, fp6
    lfs fp1, 12(src)
    ps_muls0 fp13, fp13, fp0
    lfs fp2, 28(src)
    ps_muls0 fp12, fp12, fp0
    lfs fp3, 44(src)
    ps_muls0 fp11, fp11, fp0
    ps_merge00 fp5, fp13, fp12
    ps_merge11 fp4, fp13, fp12
    ps_mul fp6, fp13, fp1
    psq_st fp5, 0(inv), 0, 0
    psq_st fp4, 16(inv), 0, 0
    ps_muls0 fp10, fp10, fp0
    ps_muls0 fp9, fp9, fp0
    ps_madd fp6, fp12, fp2, fp6
    psq_st fp10, 32(inv), 1, 0
    ps_muls0 fp8, fp8, fp0
    ps_nmadd fp6, fp11, fp3, fp6
    psq_st fp9, 36(inv), 1, 0
    ps_mul fp7, fp10, fp1
    ps_merge00 fp5, fp11, fp6
    psq_st fp8, 40(inv), 1, 0
    ps_madd fp7, fp9, fp2, fp7
    ps_merge11 fp4, fp11, fp6
    psq_st fp5, 8(inv), 0, 0
    ps_nmadd fp7, fp8, fp3, fp7
    psq_st fp4, 24(inv), 0, 0
    psq_st fp7, 44(inv), 1, 0
    addi r3, 0, 1
    blr
    // clang-format on
}

//unused
void C_MTXRotRad(){
}

void PSMTXRotRad(Mtx m, char axis, f32 rad) {
    f32 sinA, cosA;
    sinA = sinf(rad);
    cosA = cosf(rad);
    PSMTXRotTrig(m, sinA, cosA, axis);
}

//unused
void C_MTXRotTrig(){
}

void PSMTXRotTrig(register Mtx m, register f32 sinA, register f32 cosA,
                  register char arg2) {
    register f32 fc0, fc1, nsinA;
    register f32 fw0, fw1, fw2, fw3;

    ASM (
        frsp sinA, sinA
        frsp cosA, cosA
    )

    fc0 = 0.0f;
    fc1 = 1.0f;

    ASM (
        ori arg2, arg2, 0x20
        ps_neg nsinA, sinA
        cmplwi arg2, 'x'
        beq loc0
        cmplwi arg2, 'y'
        beq loc1
        cmplwi arg2, 'z'
        beq loc2
        b loc3
loc0:
        psq_st fc1, 0(m), 1, 0
        psq_st fc0, 4(m), 0, 0
        ps_merge00 fw0, sinA, cosA
        psq_st fc0, 12(m), 0, 0
        ps_merge00 fw1, cosA, nsinA
        psq_st fc0, 28(m), 0, 0
        psq_st fc0, 44(m), 1, 0
        psq_st fw0, 36(m), 0, 0
        psq_st fw1, 20(m), 0, 0
        b loc3
loc1:
        ps_merge00 fw0, cosA, fc0
        ps_merge00 fw1, fc0, fc1
        psq_st fc0, 24(m), 0, 0
        psq_st fw0, 0(m), 0, 0
        ps_merge00 fw2, nsinA, fc0
        ps_merge00 fw3, sinA, fc0
        psq_st fw0, 40(m), 0, 0
        psq_st fw1, 16(m), 0, 0
        psq_st fw3, 8(m), 0, 0
        psq_st fw2, 32(m), 0, 0
        b loc3
loc2:
        psq_st fc0, 8(m), 0, 0
        ps_merge00 fw0, sinA, cosA
        ps_merge00 fw2, cosA, nsinA
        psq_st fc0, 24(m), 0, 0
        psq_st fc0, 32(m), 0, 0
        ps_merge00 fw1, fc1, fc0
        psq_st fw0, 16(m), 0, 0
        psq_st fw2, 0(m), 0, 0
        psq_st fw1, 40(m), 0, 0
loc3:
    )
}

//unused
void C_MTXRotAxisRad(){
}

void __PSMTXRotAxisRadInternal(register Mtx m, const register Vec* axis,
                               register f32 sT, register f32 cT) {
    register f32 tT, fc0;
    register f32 tmp0, tmp1, tmp2, tmp3, tmp4;
    register f32 tmp5, tmp6, tmp7, tmp8, tmp9;

    tmp9 = 0.5f;
    tmp8 = 3.0f;

    ASM (
        frsp cT, cT
        psq_l tmp0, 0(axis), 0, 0
        frsp sT, sT
        lfs tmp1, 8(axis)
        ps_mul tmp2, tmp0, tmp0
        fadds tmp7, tmp9, tmp9
        ps_madd tmp3, tmp1, tmp1, tmp2
        fsubs fc0, tmp9, tmp9
        ps_sum0 tmp4, tmp3, tmp1, tmp2
        fsubs tT, tmp7, cT
        frsqrte tmp5, tmp4
        fmuls tmp2, tmp5, tmp5
        fmuls tmp3, tmp5, tmp9
        fnmsubs tmp2, tmp2, tmp4, tmp8
        fmuls tmp5, tmp2, tmp3
        ps_merge00  cT, cT, cT
        ps_muls0 tmp0, tmp0, tmp5
        ps_muls0 tmp1, tmp1, tmp5
        ps_muls0 tmp4, tmp0, tT
        ps_muls0 tmp9, tmp0, sT
        ps_muls0 tmp5, tmp1, tT
        ps_muls1 tmp3, tmp4, tmp0
        ps_muls0 tmp2, tmp4, tmp0
        ps_muls0 tmp4, tmp4, tmp1
        fnmsubs tmp6, tmp1, sT, tmp3
        fmadds tmp7, tmp1, sT, tmp3
        ps_neg tmp0, tmp9
        ps_sum0 tmp8, tmp4, fc0, tmp9
        ps_sum0 tmp2, tmp2, tmp6, cT
        ps_sum1 tmp3, cT, tmp7, tmp3
        ps_sum0 tmp6, tmp0, fc0, tmp4
        psq_st tmp8, 8(m), 0, 0
        ps_sum0 tmp0, tmp4, tmp4, tmp0
        psq_st tmp2, 0(m), 0, 0
        ps_muls0 tmp5, tmp5, tmp1
        psq_st tmp3, 16(m), 0, 0
        ps_sum1 tmp4, tmp9, tmp0, tmp4
        psq_st tmp6, 24(m), 0, 0
        ps_sum0 tmp5, tmp5, fc0, cT
        psq_st tmp4, 32(m), 0, 0
        psq_st tmp5, 40(m), 0, 0
    )
}

void PSMTXRotAxisRad(Mtx m, const Vec* axis, f32 rad) {
    f32 sinT = sinf(rad);
    f32 cosT = cosf(rad);
    __PSMTXRotAxisRadInternal(m, axis, sinT, cosT);
}

//unused
void C_MTXTrans(){
}

void PSMTXTrans(register Mtx m, register f32 xT, register f32 yT,
                register f32 zT) {
    register f32 c0 = 0.0f;
    register f32 c1 = 1.0f;

    ASM (
        stfs xT, 12(m)
        stfs yT, 28(m)
        psq_st c0, 4(m), 0, 0
        psq_st c0, 32(m), 0, 0
        stfs c0, 16(m)
        stfs c1, 20(m)
        stfs c0, 24(m)
        stfs c1, 40(m)
        stfs zT, 44(m)
        stfs c1, 0(m)
    )
}

//unused
void C_MTXTransApply(){
}

asm void PSMTXTransApply(const register Mtx src, register Mtx dst,
                         register f32 xT, register f32 yT, register f32 zT) {
    // clang-format off
    nofralloc
    psq_l fp4, 0(src), 0, 0
    frsp xT, xT
    psq_l fp5, 8(src), 0, 0
    frsp yT, yT
    psq_l fp7, 24(src), 0, 0
    frsp zT, zT

    psq_l fp8, 40(src), 0, 0
    psq_st fp4, 0(dst), 0, 0
    ps_sum1 fp5, xT, fp5, fp5

    psq_l fp6, 16(src), 0, 0
    psq_st fp5, 8(dst), 0, 0
    ps_sum1 fp7, yT, fp7, fp7

    psq_l fp9, 32(src), 0, 0
    psq_st fp6, 16(dst), 0, 0
    ps_sum1 fp8, zT, fp8, fp8

    psq_st fp7, 24(dst), 0, 0
    psq_st fp9, 32(dst), 0, 0
    psq_st fp8, 40(dst), 0, 0
    blr
    // clang-format on
}

//unused
void C_MTXScale(){
}

void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS) {
    f32 zero = 0.0f;
    m[0][0] = zero;
    m[0][1] = m[0][2] = zero;
    m[0][3] = m[1][0] = zero;
    m[1][1] = yS;
    m[1][2] = m[1][3] = zero;
    m[2][0] = m[2][1] = zero;
    m[2][2] = zS;
    m[2][3] = zero;
}

//unused
void C_MTXScaleApply(){
}

void PSMTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS) {
    dst[0][0] = src[0][0] * xS;
    dst[0][1] = src[0][1] * xS;
    dst[0][2] = src[0][2] * xS;
    dst[0][3] = src[0][3] * xS;
    dst[1][0] = src[1][0] * yS;
    dst[1][1] = src[1][1] * yS;
    dst[1][2] = src[1][2] * yS;
    dst[1][3] = src[1][3] * yS;
    dst[2][0] = src[2][0] * zS;
    dst[2][1] = src[2][1] * zS;
    dst[2][2] = src[2][2] * zS;
    dst[2][3] = src[2][3] * zS;
}

//unused
void C_MTXQuat(){
}

void PSMTXQuat(Mtx m, const Quaternion *q) {
    float x = q->x, y = q->y, z = q->z, w = q->w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float xw = w * x2;
    float yy = y * y2, yz = y * z2, yw = w * y2;
    float zz = z * z2, zw = w * z2;

    m[0][0] = 1.0f - yy - zz;
    m[0][1] = xy - zw;
    m[0][2] = xz + yw;
    m[0][3] = 0.0f;
    m[1][0] = xy + zw;
    m[1][1] = 1.0f - xx - zz;
    m[1][2] = yz - xw;
    m[1][3] = 0.0f;
    m[2][0] = xz - yw;
    m[2][1] = yz + xw;
    m[2][2] = 1.0f - xx - yy;
    m[2][3] = 0.0f;
}

//unused
void C_MTXReflect(){
}

//unused
void PSMTXReflect(){
}

void C_MTXLookAt(Mtx m, const Vec* camPos, const Vec* camUp, const Vec* target) {
    Vec vLook, vRight, vUp;

    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;

    PSVECNormalize(&vLook, &vLook);
    PSVECCrossProduct(camUp, &vLook, &vRight);

    PSVECNormalize(&vRight, &vRight);

    PSVECCrossProduct(&vLook, &vRight, &vUp);
    
    m[0][0] = vRight.x;
    m[0][1] = vRight.y;
    m[0][2] = vRight.z;
    m[0][3] = -(camPos->x * vRight.x + camPos->y * vRight.y + camPos->z * vRight.z);

    m[1][0] = vUp.x;
    m[1][1] = vUp.y;
    m[1][2] = vUp.z;
    m[1][3] = -(camPos->x * vUp.x + camPos->y * vUp.y + camPos->z * vUp.z);

    m[2][0] = vLook.x;
    m[2][1] = vLook.y;
    m[2][2] = vLook.z;
    m[2][3] = -(camPos->x * vLook.x + camPos->y * vLook.y + camPos->z * vLook.z);
}

void C_MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n,
                       f32 scaleS, f32 scaleT, f32 transS, f32 transT) {
    f32 tmp;
    
    tmp = 1.0f / (r - l);
    m[0][0] = ((2 * n) * tmp) * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = (((r + l) * tmp) * scaleS) - transS;
    m[0][3] = 0.0f;

    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = ((2 * n) * tmp) * scaleT;
    m[1][2] = (((t + b) * tmp) * scaleT) - transT;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -1.0f;
    m[2][3] = 0.0f;
}

void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT,
                           f32 transS, f32 transT) {
    f32 angle = MTXDegToRad(fovY * 0.5f);
    f32 cot = 1.0f / tanf(angle);

    m[0][0] = (cot / aspect) * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = -transS;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = cot * scaleT;
    m[1][2] = -transT;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -1.0f;
    m[2][3] = 0.0f;
}

void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS,
                     f32 scaleT, f32 transS, f32 transT) {
    f32 tmp;
    
    tmp = 1.0f / (r - l);
    m[0][0] = (2.0f * tmp * scaleS);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = ((-(r + l) * tmp) * scaleS) + transS;

    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = (2.0f * tmp) * scaleT;
    m[1][2] = 0.0f;
    m[1][3] = ((-(t + b) * tmp) * scaleT) + transT;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 0.0f;
    m[2][3] = 1.0f;
}
