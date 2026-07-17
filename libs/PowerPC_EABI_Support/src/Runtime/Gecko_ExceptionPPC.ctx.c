/* "libs/PowerPC_EABI_Support/src/Runtime/Gecko_ExceptionPPC.cp" line 0 "PowerPC_EABI_Support/Runtime/MWCPlusLib.h" */
#ifndef _RUNTIME_MWCPLUSLIB_H
#define _RUNTIME_MWCPLUSLIB_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Runtime/MWCPlusLib.h" line 3 "types.h" */
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
#endif

#define CTORARG_TYPE int
#define CTORARG_PARTIAL (0)
#define CTORARG_COMPLETE (1)

#define CTORCALL_COMPLETE(ctor, objptr) (((void (*)(void*, CTORARG_TYPE))ctor)(objptr, CTORARG_COMPLETE))

#define DTORARG_TYPE int

#define DTORCALL_COMPLETE(dtor, objptr) (((void (*)(void*, DTORARG_TYPE))dtor)(objptr, -1))
#define DTORCALL_PARTIAL(dtor,objptr) (((void (*)(void*, DTORARG_TYPE))dtor)(objptr, 0))


typedef void* ConstructorDestructor;


extern void __construct_array(void* ptr, ConstructorDestructor ctor, ConstructorDestructor dtor, size_t size, size_t n);
extern void __destroy_arr(void* block, ConstructorDestructor* dtor, size_t size, size_t n);
extern void* __construct_new_array(void* block, ConstructorDestructor ctor, ConstructorDestructor dtor_arg, size_t size, size_t n);
extern void __destroy_new_array(void* block, ConstructorDestructor dtor);
extern void __destroy_new_array2();
extern void __destroy_new_array3();


#ifdef __cplusplus
}
#endif

#endif
/* end "PowerPC_EABI_Support/Runtime/MWCPlusLib.h" */
/* "libs/PowerPC_EABI_Support/src/Runtime/Gecko_ExceptionPPC.cp" line 1 "exception.h" */
#ifndef MSL_EXCEPTION_H
#define MSL_EXCEPTION_H

/* "libs/PowerPC_EABI_Support/include/stl/exception.h" line 3 "exception" */
/* end "exception" */

using std::exception;
using std::bad_exception;
using std::unexpected_handler;
using std::set_unexpected;
using std::unexpected;
using std::terminate_handler;
using std::set_terminate;
using std::terminate;

#endif
/* end "exception.h" */
/* "libs/PowerPC_EABI_Support/src/Runtime/Gecko_ExceptionPPC.cp" line 2 "PowerPC_EABI_Support/Runtime/Gecko_ExceptionPPC.h" */
#ifndef _RUNTIME_GECKO_EXCEPTIONPPC_H
#define _RUNTIME_GECKO_EXCEPTIONPPC_H


typedef unsigned char exaction_type;

#define EXACTION_ENDBIT 0x80
#define EXACTION_MASK 0x7F


//EXAction structs

#define EXACTION_ENDOFLIST 0
#define EXACTION_BRANCH 1

typedef struct ex_branch {
    exaction_type action;
    unsigned char unused;
    unsigned short target;
} ex_branch;

#define EXACTION_DESTROYLOCAL 2

typedef struct ex_destroylocal {
    exaction_type action;
    unsigned char unused;
    short local;
    void* dtor;
} ex_destroylocal;

#define EXACTION_DESTROYLOCALCOND 3

typedef struct ex_destroylocalcond {
    exaction_type action;
    unsigned char dlc_field;
    short cond;
    short local;
    void* dtor;
} ex_destroylocalcond;

#define ex_destroylocalcond_MakeField(regcond) (((regcond) << 7))
#define ex_destroylocalcond_GetRegCond(field) ((field) >> 7)

#define EXACTION_DESTROYLOCALPOINTER 4

typedef struct ex_destroylocalpointer {
    exaction_type action;
    unsigned char dlp_field;
    short pointer;
    void* dtor;
} ex_destroylocalpointer;

#define ex_destroylocalpointer_MakeField(regpointer) (((regpointer) << 7))
#define ex_destroylocalpointer_GetRegPointer(field) ((field) >> 7)

#define EXACTION_DESTROYLOCALARRAY 5

typedef struct ex_destroylocalarray {
    exaction_type action;
    unsigned char unused;
    short localarray;
    unsigned short elements;
    unsigned short element_size;
    void* dtor;
} ex_destroylocalarray;

#define EXACTION_DESTROYBASE 6
#define EXACTION_DESTROYMEMBER 7

typedef struct ex_destroymember {
    exaction_type action;
    unsigned char dm_field;
    short objectptr;
    long offset;
    void* dtor;
} ex_destroymember;

#define ex_destroymember_MakeField(regpointer) (((regpointer) << 7))
#define ex_destroymember_GetRegPointer(field) ((field) >> 7)


#define EXACTION_DESTROYMEMBERCOND 8

typedef struct ex_destroymembercond {
    exaction_type action;
    unsigned char dmc_field;
    short cond;
    short objectptr;
    long offset;
    void* dtor;
} ex_destroymembercond;

#define ex_destroymembercond_MakeField(regcond,regpointer) \
            (((regcond) << 7) | (((regpointer) & 0x1) << 6))
#define ex_destroymembercond_GetRegCond(field) ((field) >> 7)
#define ex_destroymembercond_GetRegPointer(field) (((field) >> 6) & 0x1)

#define EXACTION_DESTROYMEMBERARRAY 9

typedef struct ex_destroymemberarray {
    exaction_type action;
    unsigned char dma_field;
    short objectptr;
    long offset;
    long elements;
    long element_size;
    void* dtor;
} ex_destroymemberarray;

#define ex_destroymemberarray_MakeField(regpointer) (((regpointer) << 7))
#define ex_destroymemberarray_GetRegPointer(field) ((field) >> 7)

#define EXACTION_DELETEPOINTER 10

typedef struct ex_deletepointer {
    exaction_type action;
    unsigned char dp_field;
    short objectptr;
    void* deletefunc;
} ex_deletepointer;

#define ex_deletepointer_MakeField(regpointer) (((regpointer) << 7))
#define ex_deletepointer_GetRegPointer(field) ((field) >> 7)

#define EXACTION_DELETEPOINTERCOND 11

typedef struct ex_deletepointercond {
    exaction_type action;
    unsigned char dpc_field;
    short cond;
    short objectptr;
    void* deletefunc;
} ex_deletepointercond;

#define ex_deletepointercond_MakeField(regcond,regpointer) \
            (((regcond) << 7)|(((regpointer) & 0x1)<<6))
#define ex_deletepointercond_GetRegCond(field) ((field) >> 7)
#define ex_deletepointercond_GetRegPointer(field) (((field)>>6) & 0x1)

#define EXACTION_CATCHBLOCK 12

typedef struct ex_catchblock {
    exaction_type action;
    unsigned char unused;
    char* catch_type;
    unsigned short catch_pcoffset;
    short cinfo_ref;
} ex_catchblock;

#define EXACTION_ACTIVECATCHBLOCK 13

typedef struct ex_activecatchblock {
    exaction_type action;
    unsigned char unused;
    short cinfo_ref;
} ex_activecatchblock;

#define EXACTION_TERMINATE 14

typedef struct ex_terminate {
    exaction_type action;
    unsigned char unused;
} ex_terminate;

#define EXACTION_SPECIFICATION 15

typedef struct ex_specification {
    exaction_type action;
    unsigned char unused;
    unsigned short specs;
    long pcoffset;
    long cinfo_ref;
    char* spec[];
} ex_specification;

#define EXACTION_CATCHBLOCK_32 16

typedef struct ex_catchblock_32 {
    exaction_type action;
    unsigned char unused;
    char* catch_type;
    long catch_pcoffset;
    long cinfo_ref;
} ex_catchblock_32;


//Other structs

typedef struct ExceptionRangeSmall {
    unsigned short start;
    unsigned short end;
    unsigned short action;
} ExceptionRangeSmall;

typedef struct ExceptionTableSmall {
    unsigned short et_field;
    ExceptionRangeSmall ranges[0];
} ExceptionTableSmall;

typedef struct ExceptionRangeLarge {
    unsigned long  start;
    unsigned short size;
    unsigned short action;
} ExceptionRangeLarge;

typedef struct ExceptionTableLarge {
    unsigned short et_field;
    unsigned short et_field2;
    ExceptionRangeLarge ranges[];
} ExceptionTableLarge;

#define ET_MakeField(savedGPRs,savedFPRs, savedCR, hasframeptr, isLarge) \
            (((savedGPRs) << 11) | ((savedFPRs & 0x1f) << 6) | ((savedCR & 0x1) << 5) | ((hasframeptr & 0x1) << 4) | ((isLarge & 1) << 3))

#define ET_GetSavedGPRs(field)   ((field) >> 11)
#define ET_GetSavedFPRs(field)   (((field) >> 6) & 0x1f)
#define ET_GetSavedCR(field)     (((field) >> 5) & 0x1)
#define ET_GetHasFramePtr(field) (((field) >> 4) & 0x1)
#define ET_IsLargeTable(field)   (((field) >> 3) & 0x1)
#define ET_ClearLargeBit(field)  ((field) & ~(1 << 3))
#define ET_SetLargeBit(field)    ((field) | (1 << 3))

#define ET_HasElfVector(field) (((field) >> 1) & 0x1)

typedef struct ExceptionTableIndex {
    unsigned long functionoffset;
    unsigned long eti_field;
    unsigned long exceptionoffset;
} ExceptionTableIndex;

#define ETI_MakeField(direct,fsize) ((((long)(direct)) << 31) | ((fsize) & 0x7fffffff))
#define ETI_GetDirectStore(field) ((field) >> 31)
#define ETI_GetFunctionSize(field) ((field) & 0x7fffffff)


#endif
/* end "PowerPC_EABI_Support/Runtime/Gecko_ExceptionPPC.h" */
/* "libs/PowerPC_EABI_Support/src/Runtime/Gecko_ExceptionPPC.cp" line 3 "PowerPC_EABI_Support/Runtime/NMWException.h" */
#ifndef _NMWEXCEPTION
#define _NMWEXCEPTION

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Runtime/NMWException.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Runtime/NMWException.h" line 4 "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h" */
#ifndef __PPC_EABI_LINKER
#define __PPC_EABI_LINKER

__declspec(section ".init") extern char _stack_addr[];
__declspec(section ".init") extern char _stack_end[];
__declspec(section ".init") extern char _heap_addr[];
__declspec(section ".init") extern char _heap_end[];

__declspec(section ".init") extern const char _fextabindex_rom[];
__declspec(section ".init") extern char _fextabindex[];
__declspec(section ".init") extern char _eextabindex[];

__declspec(section ".init") extern char _SDA_BASE_[];
__declspec(section ".init") extern char _SDA2_BASE_[];

typedef struct __rom_copy_info {
  char* rom;
  char* addr;
  unsigned int size;
} __rom_copy_info;

__declspec(section ".init") extern __rom_copy_info _rom_copy_info[];

typedef struct __bss_init_info {
  char* addr;
  unsigned int size;
} __bss_init_info;

__declspec(section ".init") extern __bss_init_info _bss_init_info[];

typedef struct __eti_init_info {
  void* eti_start;
  void* eti_end;
  void* code_start;
  unsigned long code_size;
} __eti_init_info;

__declspec(section ".init") extern __eti_init_info _eti_init_info[];


__declspec(section ".init") extern const char _f_init_rom[];
__declspec(section ".init") extern char _f_init[];
__declspec(section ".init") extern char _e_init[];

__declspec(section ".init") extern const char _f_text_rom[];
__declspec(section ".init") extern char _f_text[];
__declspec(section ".init") extern char _e_text[];

__declspec(section ".init") extern const char _f_rodata_rom[];
__declspec(section ".init") extern char _f_rodata[];
__declspec(section ".init") extern char _e_rodata[];

__declspec(section ".init") extern const char _fextab_rom[];
__declspec(section ".init") extern char _fextab[];
__declspec(section ".init") extern char _eextab[];

__declspec(section ".init") extern const char _f_data_rom[];
__declspec(section ".init") extern char _f_data[];
__declspec(section ".init") extern char _e_data[];

__declspec(section ".init") extern char _f_bss[];
__declspec(section ".init") extern char _e_bss[];

__declspec(section ".init") extern const char _f_sdata_rom[];
__declspec(section ".init") extern char _f_sdata[];
__declspec(section ".init") extern char _e_sdata[];

__declspec(section ".init") extern char _f_sbss[];
__declspec(section ".init") extern char _e_sbss[];

__declspec(section ".init") extern const char _f_sdata2_rom[];
__declspec(section ".init") extern char _f_sdata2[];
__declspec(section ".init") extern char _e_sdata2[];

__declspec(section ".init") extern char _f_sbss2[];
__declspec(section ".init") extern char _e_sbss2[];

__declspec(section ".init") extern const char _f_PPC_EMB_sdata0_rom[];
__declspec(section ".init") extern char _f_PPC_EMB_sdata0[];
__declspec(section ".init") extern char _e_PPC_EMB_sdata0[];

__declspec(section ".init") extern char _f_PPC_EMB_sbss0[];
__declspec(section ".init") extern char _e_PPC_EMB_sbss0[];


#endif // __PPC_EABI_LINKER
/* end "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h" */

#ifdef __cplusplus
extern "C" {
#endif

typedef short vbase_ctor_arg_type;
typedef char local_cond_type;

typedef struct CatchInfo {
    void* location;
    void* typeinfo;
    void* dtor;
    void* sublocation;
    long pointercopy;
    void* stacktop;
} CatchInfo;

typedef struct DestructorChain {
  struct DestructorChain* next;
  void* destructor;
  void* object;
} DestructorChain;

extern void* __register_global_object(void* object, void* destructor, void* registration);
extern void __destroy_global_chain(void);

extern void __end__catch(CatchInfo* catchinfo);
extern void __throw(char* throwtype, void* location, void* dtor);
extern char __throw_catch_compare(const char* throwtype, const char* catchtype, long* offset_result);
extern void __unexpected(CatchInfo* catchinfo);

extern int __register_fragment(struct __eti_init_info* info, char* TOC);
extern void __unregister_fragment(int fragmentID);

#ifdef __cplusplus
}
#endif

#endif // _NMWEXCEPTION
/* end "PowerPC_EABI_Support/Runtime/NMWException.h" */
/* "libs/PowerPC_EABI_Support/src/Runtime/Gecko_ExceptionPPC.cp" line 4 "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h" */
/* end "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h" */


#define RETURN_ADDRESS 4

union MWE_GeckoVector64 {
    double d;
    float f[2];
};

typedef union MWE_GeckoVector64 MWE_GeckoVector64;

struct GeckoFPRContext {
    double d;
    MWE_GeckoVector64 v;
};

typedef struct GeckoFPRContext GeckoFPRContext;

typedef struct ThrowContext {
    GeckoFPRContext FPR[32];
    long GPR[32];
    long CR;
    char* SP;
    char* FP;
    char* throwSP;
    char* returnaddr;
    char* throwtype;
    void* location;
    void* dtor;
    CatchInfo *catchinfo;
} ThrowContext;

typedef ThrowContext* ThrowContextPtr;

typedef struct MWExceptionInfo {
    ExceptionTableSmall* exception_record;
    char* current_function;
    char* action_pointer;
    char* code_section;
    char* data_section;
    char* TOC;
} MWExceptionInfo;

typedef struct FragmentInfo {
    ExceptionTableIndex* exception_start;
    ExceptionTableIndex* exception_end;
    char* code_start;
    char* code_end;
    char* data_start;
    char* data_end;
    char* TOC;
    int active;
} FragmentInfo;

typedef struct ProcessInfo {
    __eti_init_info* exception_info;
    char* TOC;
    int active;
} ProcessInfo;

typedef struct ActionIterator {
    MWExceptionInfo info;
    char* current_SP;
    char* current_FP;
    long current_R31;
} ActionIterator;

#define MAXFRAGMENTS 32
static ProcessInfo fragmentinfo[MAXFRAGMENTS];

typedef void (*DeleteFunc)(void *);



//Likely a fakematch
#pragma schedule once
int __register_fragment(struct __eti_init_info* info, char* TOC){
    
    ProcessInfo* f = fragmentinfo;
    int i;

    for(i = 0; i < MAXFRAGMENTS; i++, f++){
        if(f->active == 0){
            f->exception_info = info;
            f->TOC = TOC;
            f->active = 1;
            return i;
        }
    }

    return -1;
}
#pragma schedule twice

void __unregister_fragment(unsigned int fragmentID)
{
    if (fragmentID < 32) {
        fragmentinfo[fragmentID].exception_info = 0;
        fragmentinfo[0].TOC = 0;
        fragmentinfo[0].active = 0;
    }
}

int ExPPC_FindExceptionFragment(char* returnaddr, FragmentInfo* frag)
{
    ProcessInfo* f;
    int i;
    __eti_init_info* eti_info;
    
    for (i = 0, f = fragmentinfo; i < 32; ++i, ++f) {
        if (f->active) {
            eti_info = f->exception_info;
            while (1) {
                if (eti_info->code_size == 0)
                    break;
                if (returnaddr >= eti_info->code_start &&
                    returnaddr < (char*)eti_info->code_start + eti_info->code_size) {
                    frag->exception_start = (ExceptionTableIndex*)eti_info->eti_start;
                    frag->exception_end = (ExceptionTableIndex*)eti_info->eti_end;
                    frag->code_start = 0;
                    frag->code_end = 0;
                    frag->data_start = 0;
                    frag->data_end = 0;
                    frag->TOC = f->TOC;
                    frag->active = f->active;
                    return 1;
                }
                eti_info++;
            }
        }
    }
    
    return 0;
}


static void ExPPC_FindExceptionRecord(char* returnaddr, MWExceptionInfo* info){
    FragmentInfo* fragment;
    FragmentInfo frag;
    ExceptionTableIndex *exceptionindex,*p;
    unsigned long returnoffset;
    long i, m, n;

    info->exception_record = 0;
    info->action_pointer = 0;
    
    if ((ExPPC_FindExceptionFragment(returnaddr, &frag)) == 0) return;
    fragment = &frag;

    info->code_section = fragment->code_start;
    info->data_section = fragment->data_start;
    info->TOC = fragment->TOC;
    
    returnoffset = returnaddr-fragment->code_start;
    exceptionindex = fragment->exception_start;
    for(i = 0, n = fragment->exception_end-fragment->exception_start;;){
        if(i > n) return;
        p = &exceptionindex[m = (i+n)/2];

        if(returnoffset < p->functionoffset){
            n = m - 1;
        }else if(returnoffset > p->functionoffset + ETI_GetFunctionSize(p->eti_field)){
            i = m + 1;
        }else break;
    }
    info->current_function = fragment->code_start + p->functionoffset;
    info->exception_record = ETI_GetDirectStore(p->eti_field) ? (ExceptionTableSmall*)(&p->exceptionoffset) : (ExceptionTableSmall*)(fragment->data_start + p->exceptionoffset);
    
    returnoffset -= p->functionoffset;
    
    if (ET_IsLargeTable(info->exception_record->et_field)){
        ExceptionTableLarge* etl = (ExceptionTableLarge*)info->exception_record;
        ExceptionRangeLarge* erl;
        
        for(erl = etl->ranges; erl->start != 0; erl++){
            unsigned long range_end = erl->start + (erl->size * 4);
            
            if (erl->start <= returnoffset && range_end >= returnoffset){
                info->action_pointer = (char*)etl + erl->action;
                break;
            }
        }
    }else{
        ExceptionTableSmall* ets = (ExceptionTableSmall*)info->exception_record;
        ExceptionRangeSmall* ers;

        for(ers = ets->ranges; ers->start != 0; ers++){
            if(ers->start <= returnoffset && ers->end >= returnoffset){
                info->action_pointer = (char*)ets + ers->action;
                break;
            }
        }

    }
}

static long ExPPC_PopR31(char *SP,MWExceptionInfo *info){
    double* FPR_save_area;
    long* GPR_save_area;
    int saved_GPRs, saved_FPRs;
    
    saved_FPRs = ET_GetSavedFPRs(info->exception_record->et_field);
    FPR_save_area = (double*)(SP-saved_FPRs * 8);
    saved_GPRs = ET_GetSavedGPRs(info->exception_record->et_field);
    GPR_save_area = (long*)FPR_save_area;

    return GPR_save_area[-1];
}

static exaction_type ExPPC_CurrentAction(const ActionIterator* iter){
    if(iter->info.action_pointer == 0){
        return EXACTION_ENDOFLIST;
    }

    return ((ex_destroylocal*)iter->info.action_pointer)->action & EXACTION_MASK;
}

static exaction_type ExPPC_NextAction(ActionIterator* iter){
    exaction_type action;

    for(;;){
        if(iter->info.action_pointer == 0 || ((action = ((ex_destroylocal*)iter->info.action_pointer)->action) & EXACTION_ENDBIT) != 0){
            char* return_addr, *callers_SP;
            
            callers_SP = *(char**)iter->current_SP;

            if(ET_GetSavedGPRs(iter->info.exception_record->et_field)){
                iter->current_R31 = ExPPC_PopR31(callers_SP, &iter->info);
            }

            return_addr = *(char**)(callers_SP + RETURN_ADDRESS);

            ExPPC_FindExceptionRecord(return_addr, &iter->info);

            if(iter->info.exception_record == 0){
                terminate();
            }

            iter->current_SP = callers_SP;
            iter->current_FP = (ET_GetHasFramePtr(iter->info.exception_record->et_field)) ? (char*)iter->current_R31 : iter->current_SP;

            if(iter->info.action_pointer == 0) continue;
        }else{
            switch(action){
                case EXACTION_DESTROYLOCAL:
                    iter->info.action_pointer += sizeof(ex_destroylocal);
                    break;
                case EXACTION_DESTROYLOCALCOND:
                    iter->info.action_pointer += sizeof(ex_destroylocalcond);
                    break;
                case EXACTION_DESTROYLOCALPOINTER:
                    iter->info.action_pointer += sizeof(ex_destroylocalpointer);
                    break;
                case EXACTION_DESTROYLOCALARRAY:
                    iter->info.action_pointer += sizeof(ex_destroylocalarray);
                    break;
                case EXACTION_DESTROYBASE:
                case EXACTION_DESTROYMEMBER:
                    iter->info.action_pointer += sizeof(ex_destroymember);
                    break;
                case EXACTION_DESTROYMEMBERCOND:
                    iter->info.action_pointer += sizeof(ex_destroymembercond);
                    break;
                case EXACTION_DESTROYMEMBERARRAY:
                    iter->info.action_pointer += sizeof(ex_destroymemberarray);
                    break;
                case EXACTION_DELETEPOINTER:
                    iter->info.action_pointer += sizeof(ex_deletepointer);
                    break;
                case EXACTION_DELETEPOINTERCOND:
                    iter->info.action_pointer += sizeof(ex_deletepointercond);
                    break;
                case EXACTION_CATCHBLOCK:
                    iter->info.action_pointer += sizeof(ex_catchblock);
                    break;
                case EXACTION_CATCHBLOCK_32:
                    iter->info.action_pointer += sizeof(ex_catchblock_32);
                    break;
                case EXACTION_ACTIVECATCHBLOCK:
                    iter->info.action_pointer += sizeof(ex_activecatchblock);
                    break;
                case EXACTION_SPECIFICATION:
                    iter->info.action_pointer += sizeof(ex_specification) + ((ex_specification*)iter->info.action_pointer)->specs * sizeof(void*);
                    break;
                default:
                    terminate();
            }
        }

        action = ((ex_destroylocal*)iter->info.action_pointer)->action & EXACTION_MASK;

        if(action == EXACTION_BRANCH){
            iter->info.action_pointer = ((char*)iter->info.exception_record) + ((ex_branch*)iter->info.action_pointer)->target;
            action = ((ex_destroylocal*)iter->info.action_pointer)->action & EXACTION_MASK;
        }
        return action;
    }
}

static char* ExPPC_PopStackFrame(ThrowContext* context, MWExceptionInfo* info)
{
    char *SP, *callers_SP;
    double *FPR_save_area;
    long *GPR_save_area;
    int saved_GPRs, saved_FPRs;
    GeckoFPRContext *Vector_save_area;
    int i, j;
    
    SP = context->SP;
    callers_SP = *(char**)SP;
    saved_FPRs = ET_GetSavedFPRs(info->exception_record->et_field);
    
    if (ET_HasElfVector(info->exception_record->et_field)) {
        Vector_save_area = (GeckoFPRContext*)(callers_SP - saved_FPRs * 16);
        FPR_save_area = (double*)Vector_save_area;
    } else {
        FPR_save_area = (double*)(callers_SP - saved_FPRs * 8);
    }
    
    if (ET_HasElfVector(info->exception_record->et_field)) {
        for (i = 32 - saved_FPRs, j = 0; i < 32; ++i, ++j) {
            context->FPR[i].v.f[0] = Vector_save_area[j].v.f[0];
            context->FPR[i].v.f[1] = Vector_save_area[j].v.f[1];
            context->FPR[i].d = Vector_save_area[j].d;
        }
    } else {
        for (i = 32 - saved_FPRs, j = 0; i < 32; ++i, ++j) {
            context->FPR[i].d = FPR_save_area[j];
        }
    }
    
    saved_GPRs = ET_GetSavedGPRs(info->exception_record->et_field);
    GPR_save_area = (long*)FPR_save_area;
    GPR_save_area -= saved_GPRs;
    
    for (i = 32 - saved_GPRs, j = 0; i < 32; ++i, ++j) {
        context->GPR[i] = GPR_save_area[j];
    }
    
    context->SP = callers_SP;
    return *(char**)(callers_SP + 4);
}

static void ExPPC_DestroyLocal(ThrowContext* context, const ex_destroylocal* ex){
    DTORCALL_COMPLETE(ex->dtor, context->FP + ex->local);
}

static void ExPPC_DestroyLocalCond(ThrowContext* context, const ex_destroylocalcond* ex){
    int cond = ex_destroylocalcond_GetRegCond(ex->dlc_field) ? (local_cond_type)context->GPR[ex->cond] : *(local_cond_type*)(context->FP + ex->cond);

    if(cond){
        DTORCALL_COMPLETE(ex->dtor, context->FP + ex->local);
    }
}

static void ExPPC_DestroyLocalPointer(ThrowContext* context, const ex_destroylocalpointer* ex){
    void *pointer = ex_destroylocalpointer_GetRegPointer(ex->dlp_field) ? (void*)context->GPR[ex->pointer] : *(void**)(context->FP + ex->pointer);

    DTORCALL_COMPLETE(ex->dtor, pointer);
}

static void ExPPC_DestroyLocalArray(ThrowContext* context, const ex_destroylocalarray* ex){
    char* ptr = context->FP + ex->localarray;
    long n = ex->elements;
    long size = ex->element_size;

    for(ptr = ptr + size*n; n > 0; n--){
        ptr -= size;
        DTORCALL_COMPLETE(ex->dtor, ptr);
    }
}

static void ExPPC_DestroyMember(ThrowContext* context, const ex_destroymember* ex){
    char *objectptr = ex_destroymember_GetRegPointer(ex->dm_field) ? (char*)context->GPR[ex->objectptr] : *(char**)(context->FP + ex->objectptr);

    DTORCALL_COMPLETE(ex->dtor,objectptr + ex->offset);
}

static void ExPPC_DestroyBase(ThrowContext* context, const ex_destroymember* ex){
    char* objectptr = ex_destroymember_GetRegPointer(ex->dm_field) ? (char*)context->GPR[ex->objectptr] : *(char**)(context->FP + ex->objectptr);

    DTORCALL_PARTIAL(ex->dtor,objectptr + ex->offset);
}

static void ExPPC_DestroyMemberCond(ThrowContext* context, const ex_destroymembercond* ex){
    char* objectptr = ex_destroymembercond_GetRegPointer(ex->dmc_field) ? (char*)context->GPR[ex->objectptr] : *(char**)(context->FP + ex->objectptr);
    int cond = ex_destroymembercond_GetRegCond(ex->dmc_field) ? (vbase_ctor_arg_type)context->GPR[ex->cond] : *(vbase_ctor_arg_type*)(context->FP + ex->cond);

    if(cond){
        DTORCALL_PARTIAL(ex->dtor, objectptr + ex->offset);
    }
}

static void ExPPC_DestroyMemberArray(ThrowContext* context, const ex_destroymemberarray* ex){
    char* ptr = ex_destroymemberarray_GetRegPointer(ex->dma_field) ? (char*)context->GPR[ex->objectptr] : *(char**)(context->FP + ex->objectptr);
    long n = ex->elements;
    long size = ex->element_size;

    ptr += ex->offset;

    for(ptr = ptr + size*n; n > 0; n--){
        ptr -= size;
        DTORCALL_COMPLETE(ex->dtor, ptr);
    }
}

static void ExPPC_DeletePointer(ThrowContext* context, const ex_deletepointer* ex){
    char* objectptr = ex_deletepointer_GetRegPointer(ex->dp_field) ? (char*)context->GPR[ex->objectptr] : *(char**)(context->FP + ex->objectptr);

    ((DeleteFunc)ex->deletefunc)(objectptr);
}

static void ExPPC_DeletePointerCond(ThrowContext* context, const ex_deletepointercond* ex){
    char* objectptr = ex_deletepointercond_GetRegPointer(ex->dpc_field) ? (char*)context->GPR[ex->objectptr] : *(char**)(context->FP + ex->objectptr);
    int cond = ex_deletepointercond_GetRegCond(ex->dpc_field) ? (local_cond_type)context->GPR[ex->cond] : *(local_cond_type*)(context->FP+ex->cond);

    if(cond){
        ((DeleteFunc)ex->deletefunc)(objectptr);
    }
}

static void ExPPC_UnwindStack(ThrowContext* context, MWExceptionInfo* info, void* catcher){
    exaction_type action;

    #pragma exception_terminate

    for(;;){
        if(info->action_pointer == 0){
            char* return_addr;
    
            return_addr = ExPPC_PopStackFrame(context, info);
            ExPPC_FindExceptionRecord(return_addr, info);

            if(info->exception_record == 0){
                terminate();
            }

            context->FP = (ET_GetHasFramePtr(info->exception_record->et_field)) ? (char*)context->GPR[31] : context->SP;
            continue;
        }

        action = ((ex_destroylocal*)info->action_pointer)->action;

        switch(action & EXACTION_MASK){
            case EXACTION_BRANCH:
                info->action_pointer = ((char*)info->exception_record) + ((ex_branch*)info->action_pointer)->target;
                break;
            case EXACTION_DESTROYLOCAL:
                ExPPC_DestroyLocal(context, (ex_destroylocal*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroylocal);
                break;
            case EXACTION_DESTROYLOCALCOND:
                ExPPC_DestroyLocalCond(context, (ex_destroylocalcond*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroylocalcond);
                break;
            case EXACTION_DESTROYLOCALPOINTER:
                ExPPC_DestroyLocalPointer(context, (ex_destroylocalpointer*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroylocalpointer);
                break;
            case EXACTION_DESTROYLOCALARRAY:
                ExPPC_DestroyLocalArray(context, (ex_destroylocalarray*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroylocalarray);
                break;
            case EXACTION_DESTROYBASE:
                ExPPC_DestroyBase(context, (ex_destroymember*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroymember);
                break;
            case EXACTION_DESTROYMEMBER:
                ExPPC_DestroyMember(context, (ex_destroymember*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroymember);
                break;
            case EXACTION_DESTROYMEMBERCOND:
                ExPPC_DestroyMemberCond(context, (ex_destroymembercond*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroymembercond);
                break;
            case EXACTION_DESTROYMEMBERARRAY:
                ExPPC_DestroyMemberArray(context, (ex_destroymemberarray*)info->action_pointer);
                info->action_pointer += sizeof(ex_destroymemberarray);
                break;
            case EXACTION_DELETEPOINTER:
                ExPPC_DeletePointer(context, (ex_deletepointer*)info->action_pointer);
                info->action_pointer += sizeof(ex_deletepointer);
                break;
            case EXACTION_DELETEPOINTERCOND:
                ExPPC_DeletePointerCond(context, (ex_deletepointercond*)info->action_pointer);
                info->action_pointer += sizeof(ex_deletepointercond);
                break;
            case EXACTION_CATCHBLOCK:
                if(catcher == (void *)info->action_pointer) return;
                info->action_pointer += sizeof(ex_catchblock);
                break;
            case EXACTION_CATCHBLOCK_32:
                if(catcher == (void *)info->action_pointer) return;
                info->action_pointer += sizeof(ex_catchblock_32);
                break;
            case EXACTION_ACTIVECATCHBLOCK:
                {
                    CatchInfo* catchinfo;

                    catchinfo = (CatchInfo*)(context->FP + ((ex_activecatchblock*)info->action_pointer)->cinfo_ref);
                    
                    if (catchinfo->dtor){
                        if (context->location == catchinfo->location){
                            context->dtor = catchinfo->dtor;
                        }else{
                            DTORCALL_COMPLETE(catchinfo->dtor, catchinfo->location);
                        }
                       }
                    info->action_pointer += sizeof(ex_activecatchblock);
                }
                break;
            case EXACTION_SPECIFICATION:
                if(catcher == (void*)info->action_pointer) return;
                info->action_pointer += sizeof(ex_specification) + ((ex_specification*)info->action_pointer)->specs * sizeof(void*);
                break;
            default:
                terminate();
        }

        if(action & EXACTION_ENDBIT) info->action_pointer = 0;
    }
}

static int ExPPC_IsInSpecification(char* extype, ex_specification* spec){
    long i, offset;

    for(i = 0; i < spec->specs; i++){
        if(__throw_catch_compare(extype, spec->spec[i], &offset)) return 1;
    }

    return 0;
}

//unused
extern void __unexpected(CatchInfo* catchinfo){
    ex_specification* unexp = (ex_specification*)catchinfo->stacktop;

    #pragma exception_magic //allow access to __exception_magic in try/catch blocks

    try {
        unexpected();
    }
    catch(...){
        if(ExPPC_IsInSpecification((char*)((CatchInfo*)&__exception_magic)->typeinfo, unexp)){
            throw;
        }
        if(ExPPC_IsInSpecification("!bad_exception!!",unexp)){
            throw bad_exception();
        }
        if(ExPPC_IsInSpecification("!std::bad_exception!!",unexp)){
            throw bad_exception();
        }
    }
    terminate();
}

static asm void ExPPC_LongJump(register ThrowContext* context, register void* newRTOC, register void* newPC){
    nofralloc

    mr r8, newPC
    mr RTOC, newRTOC
    lwz r0, context->CR
    mtcrf 255, r0

    lmw r13, context->GPR[13]

    la r7, context->FPR[14].v
    psq_lx fp14, 0, r7, 0, 0
    lfd fp14, context->FPR[14].d

    la r7, context->FPR[15].v
    psq_lx fp15, 0, r7, 0, 0
    lfd fp15, context->FPR[15].d

    la r7, context->FPR[16].v
    psq_lx fp16, 0, r7, 0, 0
    lfd fp16, context->FPR[16].d

    la r7, context->FPR[17].v
    psq_lx fp17, 0, r7, 0, 0
    lfd fp17, context->FPR[17].d

    la r7, context->FPR[18].v
    psq_lx fp18, 0, r7, 0, 0
    lfd fp18, context->FPR[18].d

    la r7, context->FPR[19].v
    psq_lx fp19, 0, r7, 0, 0
    lfd fp19, context->FPR[19].d
    
    la r7, context->FPR[20].v
    psq_lx fp20, 0, r7, 0, 0
    lfd fp20, context->FPR[20].d

    la r7, context->FPR[21].v
    psq_lx fp21, 0, r7, 0, 0
    lfd fp21, context->FPR[21].d

    la r7, context->FPR[22].v
    psq_lx fp22, 0, r7, 0, 0
    lfd fp22, context->FPR[22].d

    la r7, context->FPR[23].v
    psq_lx fp23, 0, r7, 0, 0
    lfd fp23, context->FPR[23].d

    la r7, context->FPR[24].v
    psq_lx fp24, 0, r7, 0, 0
    lfd fp24, context->FPR[24].d

    la r7, context->FPR[25].v
    psq_lx fp25, 0, r7, 0, 0
    lfd fp25, context->FPR[25].d

    la r7, context->FPR[26].v
    psq_lx fp26, 0, r7, 0, 0
    lfd fp26, context->FPR[26].d

    la r7, context->FPR[27].v
    psq_lx fp27, 0, r7, 0, 0
    lfd fp27, context->FPR[27].d

    la r7, context->FPR[28].v
    psq_lx fp28, 0, r7, 0, 0
    lfd fp28, context->FPR[28].d

    la r7, context->FPR[29].v
    psq_lx fp29, 0, r7, 0, 0
    lfd fp29, context->FPR[29].d

    la r7, context->FPR[30].v
    psq_lx fp30, 0, r7, 0, 0
    lfd fp30, context->FPR[30].d

    la r7, context->FPR[31].v
    psq_lx fp31, 0, r7, 0, 0
    lfd fp31, context->FPR[31].d
    
    mtlr r8

    lwz SP, context->throwSP
    lwz r3, context->SP
    lwz r3, 0(r3)
    stw r3, 0(SP)
    blr
}

static void ExPPC_HandleUnexpected(ThrowContext* context, MWExceptionInfo* info, ex_specification* unexp){
    CatchInfo* catchinfo;

    #pragma exception_terminate

    ExPPC_UnwindStack(context, info, unexp);

    catchinfo = (CatchInfo*)(context->FP + unexp->cinfo_ref);
    catchinfo->location = context->location;
    catchinfo->typeinfo = context->throwtype;
    catchinfo->dtor = context->dtor;
    catchinfo->stacktop = unexp;

    ExPPC_LongJump(context, info->TOC, info->current_function + unexp->pcoffset);
}

static void ExPPC_ThrowHandler(ThrowContext* context){
    ActionIterator iter;
    MWExceptionInfo info;
    exaction_type action;
    CatchInfo* catchinfo;
    long offset;
    
    ExPPC_FindExceptionRecord(context->returnaddr, &info);

    if(info.exception_record == 0){
        terminate();
    }

    context->FP = (ET_GetHasFramePtr(info.exception_record->et_field)) ? (char*)context->GPR[31] : context->SP;

    if(context->throwtype == 0){
        iter.info = info;
        iter.current_SP = context->SP;
        iter.current_FP = context->FP;
        iter.current_R31 = context->GPR[31];

        for(action = ExPPC_CurrentAction(&iter);; action = ExPPC_NextAction(&iter)){
            switch(action){
                case EXACTION_ACTIVECATCHBLOCK:
                    break;
                case EXACTION_ENDOFLIST:
                case EXACTION_DESTROYLOCAL:
                case EXACTION_DESTROYLOCALCOND:
                case EXACTION_DESTROYLOCALPOINTER:
                case EXACTION_DESTROYLOCALARRAY:
                case EXACTION_DESTROYBASE:
                case EXACTION_DESTROYMEMBER:
                case EXACTION_DESTROYMEMBERCOND:
                case EXACTION_DESTROYMEMBERARRAY:
                case EXACTION_DELETEPOINTER:
                case EXACTION_DELETEPOINTERCOND:
                case EXACTION_CATCHBLOCK:
                case EXACTION_CATCHBLOCK_32:
                case EXACTION_SPECIFICATION:
                    continue;
                case EXACTION_TERMINATE:
                default:
                    terminate();
            }
            break;
        }

        catchinfo = (CatchInfo*)(iter.current_FP + ((ex_activecatchblock*)iter.info.action_pointer)->cinfo_ref);
        context->throwtype = (char*)catchinfo->typeinfo;
        context->location = catchinfo->location;
        context->dtor = 0;
        context->catchinfo = catchinfo;
    }else{
        context->catchinfo = 0L;
    }

    iter.info = info;
    iter.current_SP = context->SP;
    iter.current_FP = context->FP;
    iter.current_R31 = context->GPR[31];

    for(action = ExPPC_CurrentAction(&iter);; action = ExPPC_NextAction(&iter)){
        switch(action){
            case EXACTION_CATCHBLOCK_32:
                if(__throw_catch_compare(context->throwtype, ((ex_catchblock_32*)iter.info.action_pointer)->catch_type, &offset)){
                    break;
                }
                continue;
            case EXACTION_CATCHBLOCK:
                if(__throw_catch_compare(context->throwtype, ((ex_catchblock*)iter.info.action_pointer)->catch_type, &offset)){
                    break;
                }
                continue;
            case EXACTION_SPECIFICATION:
                if(!ExPPC_IsInSpecification(context->throwtype, (ex_specification*)iter.info.action_pointer)){
                    ExPPC_HandleUnexpected(context, &info, (ex_specification *)iter.info.action_pointer);
                }
                continue;
            case EXACTION_ENDOFLIST:
            case EXACTION_DESTROYLOCAL:
            case EXACTION_DESTROYLOCALCOND:
            case EXACTION_DESTROYLOCALPOINTER:
            case EXACTION_DESTROYLOCALARRAY:
            case EXACTION_DESTROYBASE:
            case EXACTION_DESTROYMEMBER:
            case EXACTION_DESTROYMEMBERCOND:
            case EXACTION_DESTROYMEMBERARRAY:
            case EXACTION_DELETEPOINTER:
            case EXACTION_DELETEPOINTERCOND:
            case EXACTION_ACTIVECATCHBLOCK:
                continue;
            case EXACTION_TERMINATE:
            default:
                terminate();
        }
        break;
    }

    if (action == EXACTION_CATCHBLOCK_32) {
        ex_catchblock_32* catchblock_32;
        catchblock_32 = (ex_catchblock_32*)iter.info.action_pointer;

        ExPPC_UnwindStack(context, &info, catchblock_32);

        catchinfo = (CatchInfo*)(context->FP + catchblock_32->cinfo_ref);
        catchinfo->location = context->location;
        catchinfo->typeinfo = context->throwtype;
        catchinfo->dtor = context->dtor;

        if(*context->throwtype == '*'){
            catchinfo->sublocation = &catchinfo->pointercopy;
            catchinfo->pointercopy = *(long*)context->location + offset;
        }else{
            catchinfo->sublocation = (char*)context->location + offset;
        }

        ExPPC_LongJump(context, info.TOC, info.current_function + catchblock_32->catch_pcoffset);
    }else{
        ex_catchblock* catchblock;
    
        catchblock = (ex_catchblock*)iter.info.action_pointer;
        ExPPC_UnwindStack(context, &info, catchblock);
    
        catchinfo = (CatchInfo*)(context->FP+catchblock->cinfo_ref);
        catchinfo->location = context->location;
        catchinfo->typeinfo = context->throwtype;
        catchinfo->dtor = context->dtor;

        if(*context->throwtype == '*'){
            catchinfo->sublocation = &catchinfo->pointercopy;
            catchinfo->pointercopy = *(long*)context->location+offset;
        }else{
            catchinfo->sublocation = (char*)context->location+offset;
        }

        ExPPC_LongJump(context, info.TOC, info.current_function + catchblock->catch_pcoffset);
    }
}

asm void __throw(char* throwtype, void* location, void* dtor){
    ThrowContext throwcontext;

    fralloc

    stmw r13, throwcontext.GPR[13]

    stfd fp14, throwcontext.FPR[14].d
    la r3, throwcontext.FPR[14].v
    psq_stx fp14, 0, r3,0,0

    stfd fp15, throwcontext.FPR[15].d
    la r3, throwcontext.FPR[15].v
    psq_stx fp15, 0, r3, 0, 0

    stfd fp16, throwcontext.FPR[16].d
    la r3, throwcontext.FPR[16].v
    psq_stx fp16, 0, r3, 0, 0

    stfd fp17, throwcontext.FPR[17].d
    la r3, throwcontext.FPR[17].v
    psq_stx fp17, 0, r3, 0, 0

    stfd fp18, throwcontext.FPR[18].d
    la r3, throwcontext.FPR[18].v
    psq_stx fp18, 0, r3, 0, 0

    stfd fp19, throwcontext.FPR[19].d
    la r3, throwcontext.FPR[19].v
    psq_stx fp19, 0, r3, 0, 0

    stfd fp20, throwcontext.FPR[20].d
    la r3, throwcontext.FPR[20].v
    psq_stx fp20, 0, r3, 0, 0

    stfd fp21, throwcontext.FPR[21].d
    la r3, throwcontext.FPR[21].v
    psq_stx fp21, 0, r3, 0, 0

    stfd fp22, throwcontext.FPR[22].d
    la r3, throwcontext.FPR[22].v
    psq_stx fp22, 0, r3, 0, 0

    stfd fp23, throwcontext.FPR[23].d
    la r3, throwcontext.FPR[23].v
    psq_stx fp23, 0, r3, 0, 0

    stfd fp24, throwcontext.FPR[24].d
    la r3, throwcontext.FPR[24].v
    psq_stx fp24, 0, r3, 0, 0

    stfd fp25, throwcontext.FPR[25].d
    la r3, throwcontext.FPR[25].v
    psq_stx fp25, 0, r3, 0, 0

    stfd fp26, throwcontext.FPR[26].d
    la r3, throwcontext.FPR[26].v
    psq_stx fp26, 0, r3, 0, 0

    stfd fp27, throwcontext.FPR[27].d
    la r3, throwcontext.FPR[27].v
    psq_stx fp27, 0, r3, 0, 0

    stfd fp28, throwcontext.FPR[28].d
    la r3, throwcontext.FPR[28].v
    psq_stx fp28, 0, r3, 0, 0

    stfd fp29, throwcontext.FPR[29].d
    la r3, throwcontext.FPR[29].v
    psq_stx fp29, 0, r3, 0, 0

    stfd fp30, throwcontext.FPR[30].d
    la r3, throwcontext.FPR[30].v
    psq_stx fp30, 0, r3, 0, 0

    stfd fp31, throwcontext.FPR[31].d
    la r3, throwcontext.FPR[31].v
    psq_stx fp31, 0, r3, 0, 0


    mfcr r3
    stw r3, throwcontext.CR;

    lwz r3, 0(sp)
    lwz r4, RETURN_ADDRESS(r3)
    stw r3, throwcontext.SP;
    stw r3, throwcontext.throwSP;
    stw r4, throwcontext.returnaddr;

    lwz r3,throwtype
    stw r3, throwcontext.throwtype
    lwz r3,location
    stw r3, throwcontext.location
    lwz r3,dtor
    stw r3, throwcontext.dtor
    la r3, throwcontext
    bl ExPPC_ThrowHandler
    nop
    frfree
    blr
}

//unused
void __end__catch(CatchInfo* catchinfo){
    if (catchinfo->location && catchinfo->dtor){
        DTORCALL_COMPLETE(catchinfo->dtor,catchinfo->location);
    }
}
