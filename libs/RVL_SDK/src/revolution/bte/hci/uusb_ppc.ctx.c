// UUSB (USB-HCI) bridge for the Broadway BTE stack.
// Reconstruction of the retail RVL_SDK uusb_ppc translation unit.

/* "libs/RVL_SDK/src/revolution/bte/hci/uusb_ppc.c" line 3 "harness_catalog.h" */
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

/* "libs/RVL_SDK/src/revolution/bte/hci/uusb_ppc.c" line 5 "string.h" */
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

/* "libs/RVL_SDK/src/revolution/bte/hci/uusb_ppc.c" line 7 "revolution/bte/rvl/uusb_ppc.h" */
#ifndef RVL_SDK_BTE_RVL_UUSB_PPC_H
#define RVL_SDK_BTE_RVL_UUSB_PPC_H
/* "libs/RVL_SDK/include/revolution/bte/rvl/uusb_ppc.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/bte/rvl/uusb_ppc.h" line 4 "revolution/bte/gki/common/gki.h" */
//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#ifndef GKI_H
#define GKI_H


/* Include platform-specific over-rides */
#if (defined(NFC_STANDALONE) && (NFC_STANDALONE == TRUE))
/* "libs/RVL_SDK/include/revolution/bte/gki/common/gki.h" line 25 "revolution/BTE/include/gki_target.h" */
/******************************************************************************
 *
 *  NOTICE OF CHANGES
 *  2024/03/25:
 *      - Add #defines for RVL target
 * 
 *  Compile with REVOLUTION defined to include these changes.
 * 
 ******************************************************************************/



//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#ifndef GKI_TARGET_H
#define GKI_TARGET_H

/**
 * Modifications for decomp
 */
#define GKI_MAX_TASKS               8
#define GKI_NUM_TIMERS              2
#define GKI_DELAY_STOP_SYS_TICK     0
#define GKI_BUF1_SIZE               128
#define GKI_BUF3_SIZE               1800
#define GKI_BUF3_MAX                30
#define GKI_BUF4_SIZE               8192
#define GKI_BUF4_MAX                9
#define GKI_NUM_FIXED_BUF_POOLS     5
#define GKI_DEF_BUFPOOL_PERM_MASK   0xfff0
#define GKI_NUM_TOTAL_BUF_POOLS     9

// RVL version didn't have A2DP?
#define GKI_PPC_TASK 2


/* Operating System Selection */
#ifndef BTE_SIM_APP
#define _GKI_ARM
#define _GKI_STANDALONE
#else
#define _BT_WIN32
#endif

/* define prefix for exporting APIs from libraries */
#define EXPORT_API

#ifndef BTE_BSE_WRAPPER
#ifdef  BTE_SIM_APP
#undef  EXPORT_API
#define EXPORT_API  __declspec(dllexport)
#endif
#endif

#define GKI_API EXPORT_API
#define UDRV_API EXPORT_API

#ifndef GKI_DEBUG
#define GKI_DEBUG FALSE
#endif


#if defined (GKI_DEBUG) && (GKI_DEBUG == TRUE)
#define GKI_TRACE(fmt, ...)     ALOGI ("%s: " fmt, __FUNCTION__, ## __VA_ARGS__)
#else
#define GKI_TRACE(fmt, ...)
#endif

/******************************************************************************
**
** Task configuration
**
******************************************************************************/

/* Definitions of task IDs for inter-task messaging */
#ifndef BTU_TASK
#define BTU_TASK                2
#endif

#ifndef BTIF_TASK
#define BTIF_TASK               1
#endif

#ifndef A2DP_MEDIA_TASK
#define A2DP_MEDIA_TASK         2
#endif

/* The number of GKI tasks in the software system. */
#ifndef GKI_MAX_TASKS
#define GKI_MAX_TASKS               3
#endif

/******************************************************************************
**
** Timer configuration
**
******************************************************************************/

/* The number of GKI timers in the software system. */
#ifndef GKI_NUM_TIMERS
#define GKI_NUM_TIMERS              3
#endif

/* A conversion value for translating ticks to calculate GKI timer.  */
#ifndef TICKS_PER_SEC
#define TICKS_PER_SEC               100
#endif

/************************************************************************
**  Utility macros converting ticks to time with user define OS ticks per sec
**/
#ifndef GKI_MS_TO_TICKS
#define GKI_MS_TO_TICKS(x)   ((x) / (1000 / TICKS_PER_SEC))
#endif

#ifndef GKI_SECS_TO_TICKS
#define GKI_SECS_TO_TICKS(x)   ((x) * (TICKS_PER_SEC))
#endif

#ifndef GKI_TICKS_TO_MS
#define GKI_TICKS_TO_MS(x)   ((x) * 1000 / TICKS_PER_SEC)
#endif

#ifndef GKI_TICKS_TO_SECS
#define GKI_TICKS_TO_SECS(x)   ((x) / TICKS_PER_SEC)
#endif



/* TICK per second from OS (OS dependent change this macro accordingly to various OS) */
#ifndef OS_TICKS_PER_SEC
#define OS_TICKS_PER_SEC               1000
#endif

/************************************************************************
**  Utility macros converting ticks to time with user define OS ticks per sec
**/

#ifndef GKI_OS_TICKS_TO_MS
#define GKI_OS_TICKS_TO_MS(x)   ((x) * 1000 / OS_TICKS_PER_SEC)
#endif


#ifndef GKI_OS_TICKS_TO_SECS
#define GKI_OS_TICKS_TO_SECS(x)   ((x) / OS_TICKS_PER_SEC))
#endif


/* delay in ticks before stopping system tick. */
#ifndef GKI_DELAY_STOP_SYS_TICK
#define GKI_DELAY_STOP_SYS_TICK     10
#endif

/* Option to guarantee no preemption during timer expiration (most system don't need this) */
#ifndef GKI_TIMER_LIST_NOPREEMPT
#define GKI_TIMER_LIST_NOPREEMPT    FALSE
#endif

/******************************************************************************
**
** Buffer configuration
**
******************************************************************************/

/* TRUE if GKI uses dynamic buffers. */
#ifndef GKI_USE_DYNAMIC_BUFFERS
#define GKI_USE_DYNAMIC_BUFFERS     FALSE
#endif

/* The size of the buffers in pool 0. */
#ifndef GKI_BUF0_SIZE
#define GKI_BUF0_SIZE               64
#endif

/* The number of buffers in buffer pool 0. */
#ifndef GKI_BUF0_MAX
#define GKI_BUF0_MAX                48
#endif

/* The ID of buffer pool 0. */
#ifndef GKI_POOL_ID_0
#define GKI_POOL_ID_0               0
#endif

/* The size of the buffers in pool 1. */
#ifndef GKI_BUF1_SIZE
#define GKI_BUF1_SIZE               288
#endif

/* The number of buffers in buffer pool 1. */
#ifndef GKI_BUF1_MAX
#define GKI_BUF1_MAX                26
#endif

/* The ID of buffer pool 1. */
#ifndef GKI_POOL_ID_1
#define GKI_POOL_ID_1               1
#endif

/* The size of the buffers in pool 2. */
#ifndef GKI_BUF2_SIZE
#define GKI_BUF2_SIZE               660
#endif

/* The number of buffers in buffer pool 2. */
#ifndef GKI_BUF2_MAX
#define GKI_BUF2_MAX                45
#endif

/* The ID of buffer pool 2. */
#ifndef GKI_POOL_ID_2
#define GKI_POOL_ID_2               2
#endif

/* The size of the buffers in pool 3. */
#ifndef GKI_BUF3_SIZE
#define GKI_BUF3_SIZE               (4096+16)
#endif

/* The number of buffers in buffer pool 3. */
#ifndef GKI_BUF3_MAX
#define GKI_BUF3_MAX                200
#endif

/* The ID of buffer pool 3. */
#ifndef GKI_POOL_ID_3
#define GKI_POOL_ID_3               3
#endif

/* The size of the largest PUBLIC fixed buffer in system. */
#ifndef GKI_MAX_BUF_SIZE
#define GKI_MAX_BUF_SIZE            GKI_BUF3_SIZE
#endif

/* The pool ID of the largest PUBLIC fixed buffer in system. */
#ifndef GKI_MAX_BUF_SIZE_POOL_ID
#define GKI_MAX_BUF_SIZE_POOL_ID    GKI_POOL_ID_3
#endif

/* RESERVED buffer pool for OBX */
/* Ideally there should be 1 buffer for each instance for RX data, and some number
of TX buffers based on active instances. OBX will only use these if packet size
requires it. In most cases the large packets are used in only one direction so
the other direction will use smaller buffers.
Devices with small amount of RAM should limit the number of active obex objects.
*/
/* The size of the buffers in pool 4. */
#ifndef GKI_BUF4_SIZE
#define GKI_BUF4_SIZE               (8080+26)
#endif

/* The number of buffers in buffer pool 4. */
#ifndef GKI_BUF4_MAX
#define GKI_BUF4_MAX                (OBX_NUM_SERVERS + OBX_NUM_CLIENTS)
#endif

/* The ID of buffer pool 4. */
#ifndef GKI_POOL_ID_4
#define GKI_POOL_ID_4               4
#endif

/* The number of fixed GKI buffer pools.
eL2CAP requires Pool ID 5
If BTM_SCO_HCI_INCLUDED is FALSE, Pool ID 6 is unnecessary, otherwise set to 7
If BTA_HL_INCLUDED is FALSE then Pool ID 7 is uncessary and set the following to 7, otherwise set to 8
If BLE_INCLUDED is FALSE then Pool ID 8 is uncessary and set the following to 8, otherwise set to 9
POOL_ID 9 is a public pool meant for large buffer needs such as SDP_DB
*/
// btla-specific ++
#ifndef GKI_NUM_FIXED_BUF_POOLS
#define GKI_NUM_FIXED_BUF_POOLS     10
#endif

/* The buffer pool usage mask. */
#ifndef GKI_DEF_BUFPOOL_PERM_MASK
/* Setting POOL_ID 9 as a public pool meant for large buffers such as SDP_DB */
#define GKI_DEF_BUFPOOL_PERM_MASK   0xfdf0
#endif
// btla-specific --

/* The number of fixed and dynamic buffer pools */
#ifndef GKI_NUM_TOTAL_BUF_POOLS
#define GKI_NUM_TOTAL_BUF_POOLS     10
#endif

/* The following is intended to be a reserved pool for L2CAP
Flow control and retransmissions and intentionally kept out
of order */

/* The number of buffers in buffer pool 5. */
#ifndef GKI_BUF5_MAX
#define GKI_BUF5_MAX                64
#endif

/* The ID of buffer pool 5. */
#ifndef GKI_POOL_ID_5
#define GKI_POOL_ID_5               5
#endif

/* The size of the buffers in pool 5
** Special pool used by l2cap retransmissions only.  This size based on segment
** that will fit into both DH5 and 2-DH3 packet types after accounting for GKI
** header.  13 bytes of max headers allows us a 339 payload max. (in btui_app.txt)
** Note: 748 used for insight scriptwrapper with CAT-2 scripts.
*/
#ifndef GKI_BUF5_SIZE
#define GKI_BUF5_SIZE               748
#endif

/* The buffer corruption check flag. */
#ifndef GKI_ENABLE_BUF_CORRUPTION_CHECK
#define GKI_ENABLE_BUF_CORRUPTION_CHECK TRUE
#endif

/* The GKI severe error macro. */
#ifndef GKI_SEVERE
#define GKI_SEVERE(code)
#endif

/* TRUE if GKI includes debug functionality. */
#ifndef GKI_DEBUG
#define GKI_DEBUG                   FALSE
#endif

/* Maximum number of exceptions logged. */
#ifndef GKI_MAX_EXCEPTION
#define GKI_MAX_EXCEPTION           8
#endif

/* Maximum number of chars stored for each exception message. */
#ifndef GKI_MAX_EXCEPTION_MSGLEN
#define GKI_MAX_EXCEPTION_MSGLEN    64
#endif

#ifndef GKI_SEND_MSG_FROM_ISR
#define GKI_SEND_MSG_FROM_ISR    FALSE
#endif


/* The following is intended to be a reserved pool for SCO
over HCI data and intentionally kept out of order */

/* The ID of buffer pool 6. */
#ifndef GKI_POOL_ID_6
#define GKI_POOL_ID_6               6
#endif

/* The size of the buffers in pool 6,
  BUF_SIZE = max SCO data 255 + sizeof(BT_HDR) = 8 + SCO packet header 3 + padding 2 = 268 */
#ifndef GKI_BUF6_SIZE
#define GKI_BUF6_SIZE               268
#endif

/* The number of buffers in buffer pool 6. */
#ifndef GKI_BUF6_MAX
#define GKI_BUF6_MAX                60
#endif


/* The following pool is a dedicated pool for HDP
   If a shared pool is more desirable then
   1. set BTA_HL_LRG_DATA_POOL_ID to the desired Gki Pool ID
   2. make sure that the shared pool size is larger than 9472
   3. adjust GKI_NUM_FIXED_BUF_POOLS accordingly since
      POOL ID 7 is not needed
*/

/* The ID of buffer pool 7. */
#ifndef GKI_POOL_ID_7
#define GKI_POOL_ID_7               7
#endif

/* The size of the buffers in pool 7 */
#ifndef GKI_BUF7_SIZE
#define GKI_BUF7_SIZE               9472
#endif

/* The number of buffers in buffer pool 7. */
#ifndef GKI_BUF7_MAX
#define GKI_BUF7_MAX                2
#endif

/* The following pool is a dedicated pool for GATT
   If a shared pool is more desirable then
   1. set GATT_DB_POOL_ID to the desired Gki Pool ID
   2. make sure that the shared pool size fit a common GATT database needs
   3. adjust GKI_NUM_FIXED_BUF_POOLS accordingly since
      POOL ID 8 is not needed
*/

/* The ID of buffer pool 8. */
#ifndef GKI_POOL_ID_8
#define GKI_POOL_ID_8               8
#endif

/* The size of the buffers in pool 8 */
#ifndef GKI_BUF8_SIZE
#define GKI_BUF8_SIZE               128
#endif

/* The number of buffers in buffer pool 8. */
#ifndef GKI_BUF8_MAX
#define GKI_BUF8_MAX                30
#endif

// btla-specific ++
/* The following pool is  meant for large allocations such as SDP_DB */
#ifndef GKI_POOL_ID_9
#define GKI_POOL_ID_9              9
#endif

#ifndef GKI_BUF9_SIZE
#define GKI_BUF9_SIZE            8192
#endif

#ifndef GKI_BUF9_MAX
#define GKI_BUF9_MAX           5
#endif
// btla-specific --

/* GKI Trace Macros */
#define GKI_TRACE_0(m)                          LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m)
#define GKI_TRACE_1(m,p1)                       LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1)
#define GKI_TRACE_2(m,p1,p2)                    LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2)
#define GKI_TRACE_3(m,p1,p2,p3)                 LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3)
#define GKI_TRACE_4(m,p1,p2,p3,p4)              LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3,p4)
#define GKI_TRACE_5(m,p1,p2,p3,p4,p5)           LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3,p4,p5)
#define GKI_TRACE_6(m,p1,p2,p3,p4,p5,p6)        LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3,p4,p5,p6)

#define GKI_TRACE_ERROR_0(m)                    LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m)
#define GKI_TRACE_ERROR_1(m,p1)                 LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1)
#define GKI_TRACE_ERROR_2(m,p1,p2)              LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2)
#define GKI_TRACE_ERROR_3(m,p1,p2,p3)           LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3)
#define GKI_TRACE_ERROR_4(m,p1,p2,p3,p4)        LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3,p4)
#define GKI_TRACE_ERROR_5(m,p1,p2,p3,p4,p5)     LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3,p4,p5)
#define GKI_TRACE_ERROR_6(m,p1,p2,p3,p4,p5,p6)  LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3,p4,p5,p6)

#ifdef __cplusplus
extern "C"
{
#endif

EXPORT_API extern void LogMsg (UINT32 trace_set_mask, const char *fmt_str, ...);

#ifdef __cplusplus
}
#endif

#endif  /* GKI_TARGET_H */
/* end "revolution/BTE/include/gki_target.h" */
#else
    /* For non-nfc_standalone, include Bluetooth definitions */
/* "libs/RVL_SDK/include/revolution/bte/gki/common/gki.h" line 28 "revolution/BTE/include/bt_target.h" */
/******************************************************************************
 *
 *  NOTICE OF CHANGES
 *  2024/03/25:
 *      - Add #defines for RVL target
 * 
 *  Compile with REVOLUTION defined to include these changes.
 * 
 ******************************************************************************/



//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef BT_TARGET_H
#define BT_TARGET_H

/**
 * Modifications for decomp
 */
#ifdef REVOLUTION

#define HAS_NO_BDROID_BUILDCFG

#define RFCOMM_INCLUDED         TRUE
#define GAP_INCLUDED            TRUE
#define HID_DEV_INCLUDED        TRUE
#define HID_HOST_INCLUDED       TRUE
#define BNEP_INCLUDED           FALSE
#define A2D_INCLUDED            FALSE
#define AVRC_INCLUDED           FALSE
#define PAN_INCLUDED            FALSE

#define BTA_AG_INCLUDED         FALSE
#define BTA_PAN_INCLUDED        FALSE
#define BTA_FS_INCLUDED         FALSE
#define BTA_AR_INCLUDED         FALSE
#define BTA_AV_INCLUDED         FALSE

#define BT_USE_TRACES           TRUE

#define BTM_SSR_INCLUDED        FALSE
#define BTM_EIR_SERVER_INCLUDED FALSE

#define ANDROID_APP_INCLUDED    FALSE
#define ANDROID_USE_LOGCAT      FALSE
#define LINUX_GKI_INCLUDED      FALSE

#define BTM_BUSY_LEVEL_CHANGE_INCLUDED FALSE

#define BTA_DM_COD              {0x40, 0x02, 0x04}
#define BTA_SYS_TIMER_PERIOD    1000

#endif

#ifndef BUILDCFG
#define BUILDCFG
#endif
/* "libs/RVL_SDK/include/revolution/BTE/include/bt_target.h" line 76 "revolution/BTE/gki/platform/data_types.h" */
/******************************************************************************
 *
 *  NOTICE OF CHANGES
 *  2024/03/25:
 *      - Move from ulinux/ to platform/
 *      - Add #include for RVL types (include/types.h)
 * 
 *  Compile with REVOLUTION defined to include these changes.
 * 
 ******************************************************************************/



//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

/* "libs/RVL_SDK/include/revolution/BTE/gki/platform/data_types.h" line 36 "types.h" */
/* end "types.h" */

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef signed   long   INT32;
typedef signed   char   INT8;
typedef signed   short  INT16;
typedef unsigned char   BOOLEAN;


typedef UINT32          TIME_STAMP;

#ifndef TRUE
#define TRUE   (!FALSE)
#endif

typedef unsigned char   UBYTE;

#ifdef __arm
#define PACKED  __packed
#define INLINE  __inline
#else
#define PACKED
#define INLINE
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN FALSE
#endif

#define UINT16_LOW_BYTE(x)      ((x) & 0xff)
#define UINT16_HI_BYTE(x)       ((x) >> 8)


#define BCM_STRCAT_S(x1,x2,x3)      strcat((x1),(x3))
#define BCM_STRNCAT_S(x1,x2,x3,x4)  strncat((x1),(x3),(x4))
#define BCM_STRCPY_S(x1,x2,x3)      strcpy((x1),(x3))
#define BCM_STRNCPY_S(x1,x2,x3,x4)  strncpy((x1),(x3),(x4))



#endif
/* end "revolution/BTE/gki/platform/data_types.h" */


#ifndef BTIF_HSAG_SERVICE_NAME
#define BTIF_HSAG_SERVICE_NAME  ("Headset Gateway")
#endif

#ifndef BTIF_HFAG_SERVICE_NAME
#define BTIF_HFAG_SERVICE_NAME  ("Handsfree Gateway")
#endif

/* Include common GKI definitions used by this platform */
/* "libs/RVL_SDK/include/revolution/BTE/include/bt_target.h" line 88 "revolution/BTE/include/gki_target.h" */
/******************************************************************************
 *
 *  NOTICE OF CHANGES
 *  2024/03/25:
 *      - Add #defines for RVL target
 * 
 *  Compile with REVOLUTION defined to include these changes.
 * 
 ******************************************************************************/



//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#ifndef GKI_TARGET_H
#define GKI_TARGET_H

/**
 * Modifications for decomp
 */
#define GKI_MAX_TASKS               8
#define GKI_NUM_TIMERS              2
#define GKI_DELAY_STOP_SYS_TICK     0
#define GKI_BUF1_SIZE               128
#define GKI_BUF3_SIZE               1800
#define GKI_BUF3_MAX                30
#define GKI_BUF4_SIZE               8192
#define GKI_BUF4_MAX                9
#define GKI_NUM_FIXED_BUF_POOLS     5
#define GKI_DEF_BUFPOOL_PERM_MASK   0xfff0
#define GKI_NUM_TOTAL_BUF_POOLS     9

// RVL version didn't have A2DP?
#define GKI_PPC_TASK 2


/* Operating System Selection */
#ifndef BTE_SIM_APP
#define _GKI_ARM
#define _GKI_STANDALONE
#else
#define _BT_WIN32
#endif

/* define prefix for exporting APIs from libraries */
#define EXPORT_API

#ifndef BTE_BSE_WRAPPER
#ifdef  BTE_SIM_APP
#undef  EXPORT_API
#define EXPORT_API  __declspec(dllexport)
#endif
#endif

#define GKI_API EXPORT_API
#define UDRV_API EXPORT_API

#ifndef GKI_DEBUG
#define GKI_DEBUG FALSE
#endif


#if defined (GKI_DEBUG) && (GKI_DEBUG == TRUE)
#define GKI_TRACE(fmt, ...)     ALOGI ("%s: " fmt, __FUNCTION__, ## __VA_ARGS__)
#else
#define GKI_TRACE(fmt, ...)
#endif

/******************************************************************************
**
** Task configuration
**
******************************************************************************/

/* Definitions of task IDs for inter-task messaging */
#ifndef BTU_TASK
#define BTU_TASK                2
#endif

#ifndef BTIF_TASK
#define BTIF_TASK               1
#endif

#ifndef A2DP_MEDIA_TASK
#define A2DP_MEDIA_TASK         2
#endif

/* The number of GKI tasks in the software system. */
#ifndef GKI_MAX_TASKS
#define GKI_MAX_TASKS               3
#endif

/******************************************************************************
**
** Timer configuration
**
******************************************************************************/

/* The number of GKI timers in the software system. */
#ifndef GKI_NUM_TIMERS
#define GKI_NUM_TIMERS              3
#endif

/* A conversion value for translating ticks to calculate GKI timer.  */
#ifndef TICKS_PER_SEC
#define TICKS_PER_SEC               100
#endif

/************************************************************************
**  Utility macros converting ticks to time with user define OS ticks per sec
**/
#ifndef GKI_MS_TO_TICKS
#define GKI_MS_TO_TICKS(x)   ((x) / (1000 / TICKS_PER_SEC))
#endif

#ifndef GKI_SECS_TO_TICKS
#define GKI_SECS_TO_TICKS(x)   ((x) * (TICKS_PER_SEC))
#endif

#ifndef GKI_TICKS_TO_MS
#define GKI_TICKS_TO_MS(x)   ((x) * 1000 / TICKS_PER_SEC)
#endif

#ifndef GKI_TICKS_TO_SECS
#define GKI_TICKS_TO_SECS(x)   ((x) / TICKS_PER_SEC)
#endif



/* TICK per second from OS (OS dependent change this macro accordingly to various OS) */
#ifndef OS_TICKS_PER_SEC
#define OS_TICKS_PER_SEC               1000
#endif

/************************************************************************
**  Utility macros converting ticks to time with user define OS ticks per sec
**/

#ifndef GKI_OS_TICKS_TO_MS
#define GKI_OS_TICKS_TO_MS(x)   ((x) * 1000 / OS_TICKS_PER_SEC)
#endif


#ifndef GKI_OS_TICKS_TO_SECS
#define GKI_OS_TICKS_TO_SECS(x)   ((x) / OS_TICKS_PER_SEC))
#endif


/* delay in ticks before stopping system tick. */
#ifndef GKI_DELAY_STOP_SYS_TICK
#define GKI_DELAY_STOP_SYS_TICK     10
#endif

/* Option to guarantee no preemption during timer expiration (most system don't need this) */
#ifndef GKI_TIMER_LIST_NOPREEMPT
#define GKI_TIMER_LIST_NOPREEMPT    FALSE
#endif

/******************************************************************************
**
** Buffer configuration
**
******************************************************************************/

/* TRUE if GKI uses dynamic buffers. */
#ifndef GKI_USE_DYNAMIC_BUFFERS
#define GKI_USE_DYNAMIC_BUFFERS     FALSE
#endif

/* The size of the buffers in pool 0. */
#ifndef GKI_BUF0_SIZE
#define GKI_BUF0_SIZE               64
#endif

/* The number of buffers in buffer pool 0. */
#ifndef GKI_BUF0_MAX
#define GKI_BUF0_MAX                48
#endif

/* The ID of buffer pool 0. */
#ifndef GKI_POOL_ID_0
#define GKI_POOL_ID_0               0
#endif

/* The size of the buffers in pool 1. */
#ifndef GKI_BUF1_SIZE
#define GKI_BUF1_SIZE               288
#endif

/* The number of buffers in buffer pool 1. */
#ifndef GKI_BUF1_MAX
#define GKI_BUF1_MAX                26
#endif

/* The ID of buffer pool 1. */
#ifndef GKI_POOL_ID_1
#define GKI_POOL_ID_1               1
#endif

/* The size of the buffers in pool 2. */
#ifndef GKI_BUF2_SIZE
#define GKI_BUF2_SIZE               660
#endif

/* The number of buffers in buffer pool 2. */
#ifndef GKI_BUF2_MAX
#define GKI_BUF2_MAX                45
#endif

/* The ID of buffer pool 2. */
#ifndef GKI_POOL_ID_2
#define GKI_POOL_ID_2               2
#endif

/* The size of the buffers in pool 3. */
#ifndef GKI_BUF3_SIZE
#define GKI_BUF3_SIZE               (4096+16)
#endif

/* The number of buffers in buffer pool 3. */
#ifndef GKI_BUF3_MAX
#define GKI_BUF3_MAX                200
#endif

/* The ID of buffer pool 3. */
#ifndef GKI_POOL_ID_3
#define GKI_POOL_ID_3               3
#endif

/* The size of the largest PUBLIC fixed buffer in system. */
#ifndef GKI_MAX_BUF_SIZE
#define GKI_MAX_BUF_SIZE            GKI_BUF3_SIZE
#endif

/* The pool ID of the largest PUBLIC fixed buffer in system. */
#ifndef GKI_MAX_BUF_SIZE_POOL_ID
#define GKI_MAX_BUF_SIZE_POOL_ID    GKI_POOL_ID_3
#endif

/* RESERVED buffer pool for OBX */
/* Ideally there should be 1 buffer for each instance for RX data, and some number
of TX buffers based on active instances. OBX will only use these if packet size
requires it. In most cases the large packets are used in only one direction so
the other direction will use smaller buffers.
Devices with small amount of RAM should limit the number of active obex objects.
*/
/* The size of the buffers in pool 4. */
#ifndef GKI_BUF4_SIZE
#define GKI_BUF4_SIZE               (8080+26)
#endif

/* The number of buffers in buffer pool 4. */
#ifndef GKI_BUF4_MAX
#define GKI_BUF4_MAX                (OBX_NUM_SERVERS + OBX_NUM_CLIENTS)
#endif

/* The ID of buffer pool 4. */
#ifndef GKI_POOL_ID_4
#define GKI_POOL_ID_4               4
#endif

/* The number of fixed GKI buffer pools.
eL2CAP requires Pool ID 5
If BTM_SCO_HCI_INCLUDED is FALSE, Pool ID 6 is unnecessary, otherwise set to 7
If BTA_HL_INCLUDED is FALSE then Pool ID 7 is uncessary and set the following to 7, otherwise set to 8
If BLE_INCLUDED is FALSE then Pool ID 8 is uncessary and set the following to 8, otherwise set to 9
POOL_ID 9 is a public pool meant for large buffer needs such as SDP_DB
*/
// btla-specific ++
#ifndef GKI_NUM_FIXED_BUF_POOLS
#define GKI_NUM_FIXED_BUF_POOLS     10
#endif

/* The buffer pool usage mask. */
#ifndef GKI_DEF_BUFPOOL_PERM_MASK
/* Setting POOL_ID 9 as a public pool meant for large buffers such as SDP_DB */
#define GKI_DEF_BUFPOOL_PERM_MASK   0xfdf0
#endif
// btla-specific --

/* The number of fixed and dynamic buffer pools */
#ifndef GKI_NUM_TOTAL_BUF_POOLS
#define GKI_NUM_TOTAL_BUF_POOLS     10
#endif

/* The following is intended to be a reserved pool for L2CAP
Flow control and retransmissions and intentionally kept out
of order */

/* The number of buffers in buffer pool 5. */
#ifndef GKI_BUF5_MAX
#define GKI_BUF5_MAX                64
#endif

/* The ID of buffer pool 5. */
#ifndef GKI_POOL_ID_5
#define GKI_POOL_ID_5               5
#endif

/* The size of the buffers in pool 5
** Special pool used by l2cap retransmissions only.  This size based on segment
** that will fit into both DH5 and 2-DH3 packet types after accounting for GKI
** header.  13 bytes of max headers allows us a 339 payload max. (in btui_app.txt)
** Note: 748 used for insight scriptwrapper with CAT-2 scripts.
*/
#ifndef GKI_BUF5_SIZE
#define GKI_BUF5_SIZE               748
#endif

/* The buffer corruption check flag. */
#ifndef GKI_ENABLE_BUF_CORRUPTION_CHECK
#define GKI_ENABLE_BUF_CORRUPTION_CHECK TRUE
#endif

/* The GKI severe error macro. */
#ifndef GKI_SEVERE
#define GKI_SEVERE(code)
#endif

/* TRUE if GKI includes debug functionality. */
#ifndef GKI_DEBUG
#define GKI_DEBUG                   FALSE
#endif

/* Maximum number of exceptions logged. */
#ifndef GKI_MAX_EXCEPTION
#define GKI_MAX_EXCEPTION           8
#endif

/* Maximum number of chars stored for each exception message. */
#ifndef GKI_MAX_EXCEPTION_MSGLEN
#define GKI_MAX_EXCEPTION_MSGLEN    64
#endif

#ifndef GKI_SEND_MSG_FROM_ISR
#define GKI_SEND_MSG_FROM_ISR    FALSE
#endif


/* The following is intended to be a reserved pool for SCO
over HCI data and intentionally kept out of order */

/* The ID of buffer pool 6. */
#ifndef GKI_POOL_ID_6
#define GKI_POOL_ID_6               6
#endif

/* The size of the buffers in pool 6,
  BUF_SIZE = max SCO data 255 + sizeof(BT_HDR) = 8 + SCO packet header 3 + padding 2 = 268 */
#ifndef GKI_BUF6_SIZE
#define GKI_BUF6_SIZE               268
#endif

/* The number of buffers in buffer pool 6. */
#ifndef GKI_BUF6_MAX
#define GKI_BUF6_MAX                60
#endif


/* The following pool is a dedicated pool for HDP
   If a shared pool is more desirable then
   1. set BTA_HL_LRG_DATA_POOL_ID to the desired Gki Pool ID
   2. make sure that the shared pool size is larger than 9472
   3. adjust GKI_NUM_FIXED_BUF_POOLS accordingly since
      POOL ID 7 is not needed
*/

/* The ID of buffer pool 7. */
#ifndef GKI_POOL_ID_7
#define GKI_POOL_ID_7               7
#endif

/* The size of the buffers in pool 7 */
#ifndef GKI_BUF7_SIZE
#define GKI_BUF7_SIZE               9472
#endif

/* The number of buffers in buffer pool 7. */
#ifndef GKI_BUF7_MAX
#define GKI_BUF7_MAX                2
#endif

/* The following pool is a dedicated pool for GATT
   If a shared pool is more desirable then
   1. set GATT_DB_POOL_ID to the desired Gki Pool ID
   2. make sure that the shared pool size fit a common GATT database needs
   3. adjust GKI_NUM_FIXED_BUF_POOLS accordingly since
      POOL ID 8 is not needed
*/

/* The ID of buffer pool 8. */
#ifndef GKI_POOL_ID_8
#define GKI_POOL_ID_8               8
#endif

/* The size of the buffers in pool 8 */
#ifndef GKI_BUF8_SIZE
#define GKI_BUF8_SIZE               128
#endif

/* The number of buffers in buffer pool 8. */
#ifndef GKI_BUF8_MAX
#define GKI_BUF8_MAX                30
#endif

// btla-specific ++
/* The following pool is  meant for large allocations such as SDP_DB */
#ifndef GKI_POOL_ID_9
#define GKI_POOL_ID_9              9
#endif

#ifndef GKI_BUF9_SIZE
#define GKI_BUF9_SIZE            8192
#endif

#ifndef GKI_BUF9_MAX
#define GKI_BUF9_MAX           5
#endif
// btla-specific --

/* GKI Trace Macros */
#define GKI_TRACE_0(m)                          LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m)
#define GKI_TRACE_1(m,p1)                       LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1)
#define GKI_TRACE_2(m,p1,p2)                    LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2)
#define GKI_TRACE_3(m,p1,p2,p3)                 LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3)
#define GKI_TRACE_4(m,p1,p2,p3,p4)              LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3,p4)
#define GKI_TRACE_5(m,p1,p2,p3,p4,p5)           LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3,p4,p5)
#define GKI_TRACE_6(m,p1,p2,p3,p4,p5,p6)        LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_GENERIC,m,p1,p2,p3,p4,p5,p6)

#define GKI_TRACE_ERROR_0(m)                    LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m)
#define GKI_TRACE_ERROR_1(m,p1)                 LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1)
#define GKI_TRACE_ERROR_2(m,p1,p2)              LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2)
#define GKI_TRACE_ERROR_3(m,p1,p2,p3)           LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3)
#define GKI_TRACE_ERROR_4(m,p1,p2,p3,p4)        LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3,p4)
#define GKI_TRACE_ERROR_5(m,p1,p2,p3,p4,p5)     LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3,p4,p5)
#define GKI_TRACE_ERROR_6(m,p1,p2,p3,p4,p5,p6)  LogMsg(TRACE_CTRL_GENERAL | TRACE_LAYER_GKI | TRACE_ORG_GKI | TRACE_TYPE_ERROR,m,p1,p2,p3,p4,p5,p6)

#ifdef __cplusplus
extern "C"
{
#endif

EXPORT_API extern void LogMsg (UINT32 trace_set_mask, const char *fmt_str, ...);

#ifdef __cplusplus
}
#endif

#endif  /* GKI_TARGET_H */
/* end "revolution/BTE/include/gki_target.h" */

/* "libs/RVL_SDK/include/revolution/BTE/include/bt_target.h" line 90 "revolution/BTE/stack/include/bt_types.h" */
//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef BT_TYPES_H
#define BT_TYPES_H

/* "libs/RVL_SDK/include/revolution/BTE/stack/include/bt_types.h" line 23 "revolution/BTE/gki/platform/data_types.h" */
/******************************************************************************
 *
 *  NOTICE OF CHANGES
 *  2024/03/25:
 *      - Move from ulinux/ to platform/
 *      - Add #include for RVL types (include/types.h)
 * 
 *  Compile with REVOLUTION defined to include these changes.
 * 
 ******************************************************************************/



//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

/* "libs/RVL_SDK/include/revolution/BTE/gki/platform/data_types.h" line 36 "types.h" */
/* end "types.h" */

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef signed   long   INT32;
typedef signed   char   INT8;
typedef signed   short  INT16;
typedef unsigned char   BOOLEAN;


typedef UINT32          TIME_STAMP;

#ifndef TRUE
#define TRUE   (!FALSE)
#endif

typedef unsigned char   UBYTE;

#ifdef __arm
#define PACKED  __packed
#define INLINE  __inline
#else
#define PACKED
#define INLINE
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN FALSE
#endif

#define UINT16_LOW_BYTE(x)      ((x) & 0xff)
#define UINT16_HI_BYTE(x)       ((x) >> 8)


#define BCM_STRCAT_S(x1,x2,x3)      strcat((x1),(x3))
#define BCM_STRNCAT_S(x1,x2,x3,x4)  strncat((x1),(x3),(x4))
#define BCM_STRCPY_S(x1,x2,x3)      strcpy((x1),(x3))
#define BCM_STRNCPY_S(x1,x2,x3,x4)  strncpy((x1),(x3),(x4))



#endif
/* end "revolution/BTE/gki/platform/data_types.h" */

#ifdef _WIN32
#ifdef BLUESTACK_TESTER
/* "libs/RVL_SDK/include/revolution/BTE/stack/include/bt_types.h" line 27 "bte_stack_entry.h" */
/* end "bte_stack_entry.h" */
#endif
#endif

/* READ WELL !!
**
** This section defines global events. These are events that cross layers.
** Any event that passes between layers MUST be one of these events. Tasks
** can use their own events internally, but a FUNDAMENTAL design issue is
** that global events MUST be one of these events defined below.
**
** The convention used is the the event name contains the layer that the
** event is going to.
*/
#define BT_EVT_MASK                 0xFF00
#define BT_SUB_EVT_MASK             0x00FF
                                                /* To Bluetooth Upper Layers        */
                                                /************************************/
#define BT_EVT_TO_BTU_L2C_EVT       0x0900      /* L2CAP event */
#define BT_EVT_TO_BTU_HCI_EVT       0x1000      /* HCI Event                        */
#define BT_EVT_TO_BTU_HCI_BR_EDR_EVT (0x0000 | BT_EVT_TO_BTU_HCI_EVT)      /* event from BR/EDR controller */
#define BT_EVT_TO_BTU_HCI_AMP1_EVT   (0x0001 | BT_EVT_TO_BTU_HCI_EVT)      /* event from local AMP 1 controller */
#define BT_EVT_TO_BTU_HCI_AMP2_EVT   (0x0002 | BT_EVT_TO_BTU_HCI_EVT)      /* event from local AMP 2 controller */
#define BT_EVT_TO_BTU_HCI_AMP3_EVT   (0x0003 | BT_EVT_TO_BTU_HCI_EVT)      /* event from local AMP 3 controller */

#define BT_EVT_TO_BTU_HCI_ACL       0x1100      /* ACL Data from HCI                */
#define BT_EVT_TO_BTU_HCI_SCO       0x1200      /* SCO Data from HCI                */
#define BT_EVT_TO_BTU_HCIT_ERR      0x1300      /* HCI Transport Error              */

#define BT_EVT_TO_BTU_SP_EVT        0x1400      /* Serial Port Event                */
#define BT_EVT_TO_BTU_SP_DATA       0x1500      /* Serial Port Data                 */

#define BT_EVT_TO_BTU_HCI_CMD       0x1600      /* HCI command from upper layer     */


#define BT_EVT_TO_BTU_L2C_SEG_XMIT  0x1900      /* L2CAP segment(s) transmitted     */

#define BT_EVT_PROXY_INCOMING_MSG   0x1A00      /* BlueStackTester event: incoming message from target */

#define BT_EVT_BTSIM                0x1B00      /* Insight BTSIM event */
#define BT_EVT_BTISE                0x1C00      /* Insight Script Engine event */

                                                /* To LM                            */
                                                /************************************/
#define BT_EVT_TO_LM_HCI_CMD        0x2000      /* HCI Command                      */
#define BT_EVT_TO_LM_HCI_ACL        0x2100      /* HCI ACL Data                     */
#define BT_EVT_TO_LM_HCI_SCO        0x2200      /* HCI SCO Data                     */
#define BT_EVT_TO_LM_HCIT_ERR       0x2300      /* HCI Transport Error              */
#define BT_EVT_TO_LM_LC_EVT         0x2400      /* LC event                         */
#define BT_EVT_TO_LM_LC_LMP         0x2500      /* LC Received LMP command frame    */
#define BT_EVT_TO_LM_LC_ACL         0x2600      /* LC Received ACL data             */
#define BT_EVT_TO_LM_LC_SCO         0x2700      /* LC Received SCO data  (not used) */
#define BT_EVT_TO_LM_LC_ACL_TX      0x2800      /* LMP data transmit complete       */
#define BT_EVT_TO_LM_LC_LMPC_TX     0x2900      /* LMP Command transmit complete    */
#define BT_EVT_TO_LM_LOCAL_ACL_LB   0x2a00      /* Data to be locally loopbacked    */
#define BT_EVT_TO_LM_HCI_ACL_ACK    0x2b00      /* HCI ACL Data ack      (not used) */
#define BT_EVT_TO_LM_DIAG           0x2c00      /* LM Diagnostics commands          */


#define BT_EVT_TO_BTM_CMDS          0x2f00
#define BT_EVT_TO_BTM_PM_MDCHG_EVT (0x0001 | BT_EVT_TO_BTM_CMDS)

#define BT_EVT_TO_TCS_CMDS          0x3000

#define BT_EVT_TO_OBX_CL_MSG        0x3100
#define BT_EVT_TO_OBX_SR_MSG        0x3200

#define BT_EVT_TO_CTP_CMDS          0x3300

/* Obex Over L2CAP */
#define BT_EVT_TO_OBX_CL_L2C_MSG    0x3400
#define BT_EVT_TO_OBX_SR_L2C_MSG    0x3500

/* ftp events */
#define BT_EVT_TO_FTP_SRVR_CMDS     0x3800
#define BT_EVT_TO_FTP_CLNT_CMDS     0x3900

#define BT_EVT_TO_BTU_SAP           0x3a00       /* SIM Access Profile events */

/* opp events */
#define BT_EVT_TO_OPP_SRVR_CMDS     0x3b00
#define BT_EVT_TO_OPP_CLNT_CMDS     0x3c00

/* gap events */
#define BT_EVT_TO_GAP_MSG           0x3d00

/* start timer */
#define BT_EVT_TO_START_TIMER       0x3e00

/* start quick timer */
#define BT_EVT_TO_START_QUICK_TIMER 0x3f00


/* for NFC                          */
                                                /************************************/
#define BT_EVT_TO_NFC_NCI           0x4000      /* NCI Command, Notification or Data*/
#define BT_EVT_TO_NFC_INIT          0x4100      /* Initialization message */
#define BT_EVT_TO_LLCP_ECHO         0x4200      /* LLCP Echo Service */
#define BT_EVT_TO_LLCP_SOCKET       0x4300      /* LLCP over TCP/IP */
#define BT_EVT_TO_NCI_LP            0x4400      /* Low power */
#define BT_EVT_TO_NFC_ERR           0x4500      /* Error notification to NFC Task */

#define BT_EVT_TO_NFCCSIM_NCI       0x4a00      /* events to NFCC simulation (NCI packets) */

/* HCISU Events */

#define BT_EVT_HCISU                0x5000

// btla-specific ++
#define BT_EVT_TO_HCISU_RECONFIG_EVT            (0x0001 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_UPDATE_BAUDRATE_EVT     (0x0002 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_ENABLE_EVT           (0x0003 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_DISABLE_EVT          (0x0004 | BT_EVT_HCISU)
// btla-specific --
#define BT_EVT_TO_HCISU_LP_APP_SLEEPING_EVT     (0x0005 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_ALLOW_BT_SLEEP_EVT   (0x0006 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_WAKEUP_HOST_EVT      (0x0007 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_RCV_H4IBSS_EVT       (0x0008 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_H5_RESET_EVT            (0x0009 | BT_EVT_HCISU)
#define BT_EVT_HCISU_START_QUICK_TIMER          (0x000a | BT_EVT_HCISU)

#define BT_EVT_DATA_TO_AMP_1        0x5100
#define BT_EVT_DATA_TO_AMP_15       0x5f00

/* HSP Events */

#define BT_EVT_BTU_HSP2             0x6000

#define BT_EVT_TO_BTU_HSP2_EVT     (0x0001 | BT_EVT_BTU_HSP2)

/* BPP Events */
#define BT_EVT_TO_BPP_PR_CMDS       0x6100      /* Printer Events */
#define BT_EVT_TO_BPP_SND_CMDS      0x6200      /* BPP Sender Events */

/* BIP Events */
#define BT_EVT_TO_BIP_CMDS          0x6300

/* HCRP Events */

#define BT_EVT_BTU_HCRP             0x7000

#define BT_EVT_TO_BTU_HCRP_EVT     (0x0001 | BT_EVT_BTU_HCRP)
#define BT_EVT_TO_BTU_HCRPM_EVT    (0x0002 | BT_EVT_BTU_HCRP)


#define BT_EVT_BTU_HFP              0x8000
#define BT_EVT_TO_BTU_HFP_EVT      (0x0001 | BT_EVT_BTU_HFP)

#define BT_EVT_BTU_IPC_EVT          0x9000
#define BT_EVT_BTU_IPC_LOGMSG_EVT  (0x0000 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_ACL_EVT     (0x0001 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BTU_EVT     (0x0002 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_L2C_EVT     (0x0003 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_L2C_MSG_EVT (0x0004 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BTM_EVT     (0x0005 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_AVDT_EVT    (0x0006 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_SLIP_EVT    (0x0007 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_MGMT_EVT    (0x0008 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BTTRC_EVT   (0x0009 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BURST_EVT   (0x000A | BT_EVT_BTU_IPC_EVT)


/* BTIF Events */
#define BT_EVT_BTIF                 0xA000
#define BT_EVT_CONTEXT_SWITCH_EVT  (0x0001 | BT_EVT_BTIF)

#define BT_EVT_TRIGGER_STACK_INIT   EVENT_MASK(APPL_EVT_0)


/* Define the header of each buffer used in the Bluetooth stack.
*/
typedef struct
{
    UINT16          event;
    UINT16          len;
    UINT16          offset;
    UINT16          layer_specific;
} BT_HDR;

#define BT_HDR_SIZE (sizeof (BT_HDR))

#define BT_PSM_SDP                      0x0001
#define BT_PSM_RFCOMM                   0x0003
#define BT_PSM_TCS                      0x0005
#define BT_PSM_CTP                      0x0007
#define BT_PSM_BNEP                     0x000F
#define BT_PSM_HIDC                     0x0011
#define BT_PSM_HIDI                     0x0013
#define BT_PSM_UPNP                     0x0015
#define BT_PSM_AVCTP                    0x0017
#define BT_PSM_AVDTP                    0x0019
#define BT_PSM_AVCTP_13                 0x001B /* Advanced Control - Browsing */
#define BT_PSM_UDI_CP                   0x001D /* Unrestricted Digital Information Profile C-Plane  */
#define BT_PSM_ATT                      0x001F /* Attribute Protocol  */


/* These macros extract the HCI opcodes from a buffer
*/
#define HCI_GET_CMD_HDR_OPCODE(p)    (UINT16)((*((UINT8 *)((p) + 1) + p->offset) + \
                                              (*((UINT8 *)((p) + 1) + p->offset + 1) << 8)))
#define HCI_GET_CMD_HDR_PARAM_LEN(p) (UINT8)  (*((UINT8 *)((p) + 1) + p->offset + 2))

#define HCI_GET_EVT_HDR_OPCODE(p)    (UINT8)(*((UINT8 *)((p) + 1) + p->offset))
#define HCI_GET_EVT_HDR_PARAM_LEN(p) (UINT8)  (*((UINT8 *)((p) + 1) + p->offset + 1))


/********************************************************************************
** Macros to get and put bytes to and from a stream (Little Endian format).
*/
#define UINT32_TO_STREAM(p, u32) {*(p)++ = (UINT8)(u32); *(p)++ = (UINT8)((u32) >> 8); *(p)++ = (UINT8)((u32) >> 16); *(p)++ = (UINT8)((u32) >> 24);}
#define UINT24_TO_STREAM(p, u24) {*(p)++ = (UINT8)(u24); *(p)++ = (UINT8)((u24) >> 8); *(p)++ = (UINT8)((u24) >> 16);}
#define UINT16_TO_STREAM(p, u16) {*(p)++ = (UINT8)(u16); *(p)++ = (UINT8)((u16) >> 8);}
#define UINT8_TO_STREAM(p, u8)   {*(p)++ = (UINT8)(u8);}
#define INT8_TO_STREAM(p, u8)    {*(p)++ = (INT8)(u8);}
#define ARRAY32_TO_STREAM(p, a)  {register int ijk; for (ijk = 0; ijk < 32;           ijk++) *(p)++ = (UINT8) a[31 - ijk];}
#define ARRAY16_TO_STREAM(p, a)  {register int ijk; for (ijk = 0; ijk < 16;           ijk++) *(p)++ = (UINT8) a[15 - ijk];}
#define ARRAY8_TO_STREAM(p, a)   {register int ijk; for (ijk = 0; ijk < 8;            ijk++) *(p)++ = (UINT8) a[7 - ijk];}
#define BDADDR_TO_STREAM(p, a)   {register int ijk; for (ijk = 0; ijk < BD_ADDR_LEN;  ijk++) *(p)++ = (UINT8) a[BD_ADDR_LEN - 1 - ijk];}
#define LAP_TO_STREAM(p, a)      {register int ijk; for (ijk = 0; ijk < LAP_LEN;      ijk++) *(p)++ = (UINT8) a[LAP_LEN - 1 - ijk];}
#define DEVCLASS_TO_STREAM(p, a) {register int ijk; for (ijk = 0; ijk < DEV_CLASS_LEN;ijk++) *(p)++ = (UINT8) a[DEV_CLASS_LEN - 1 - ijk];}
#define ARRAY_TO_STREAM(p, a, len) {register int ijk; for (ijk = 0; ijk < len;        ijk++) *(p)++ = (UINT8) a[ijk];}
#define REVERSE_ARRAY_TO_STREAM(p, a, len)  {register int ijk; for (ijk = 0; ijk < len; ijk++) *(p)++ = (UINT8) a[len - 1 - ijk];}

#define STREAM_TO_UINT8(u8, p)   {u8 = (UINT8)(*(p)); (p) += 1;}
#define STREAM_TO_UINT16(u16, p) {u16 = ((UINT16)(*(p)) + (((UINT16)(*((p) + 1))) << 8)); (p) += 2;}
#define STREAM_TO_UINT24(u32, p) {u32 = (((UINT32)(*(p))) + ((((UINT32)(*((p) + 1)))) << 8) + ((((UINT32)(*((p) + 2)))) << 16) ); (p) += 3;}
#define STREAM_TO_UINT32(u32, p) {u32 = (((UINT32)(*(p))) + ((((UINT32)(*((p) + 1)))) << 8) + ((((UINT32)(*((p) + 2)))) << 16) + ((((UINT32)(*((p) + 3)))) << 24)); (p) += 4;}
#define STREAM_TO_BDADDR(a, p)   {register int ijk; register UINT8 *pbda = (UINT8 *)a + BD_ADDR_LEN - 1; for (ijk = 0; ijk < BD_ADDR_LEN; ijk++) *pbda-- = *p++;}
#define STREAM_TO_ARRAY32(a, p)  {register int ijk; register UINT8 *_pa = (UINT8 *)a + 31; for (ijk = 0; ijk < 32; ijk++) *_pa-- = *p++;}
#define STREAM_TO_ARRAY16(a, p)  {register int ijk; register UINT8 *_pa = (UINT8 *)a + 15; for (ijk = 0; ijk < 16; ijk++) *_pa-- = *p++;}
#define STREAM_TO_ARRAY8(a, p)   {register int ijk; register UINT8 *_pa = (UINT8 *)a + 7; for (ijk = 0; ijk < 8; ijk++) *_pa-- = *p++;}
#define STREAM_TO_DEVCLASS(a, p) {register int ijk; register UINT8 *_pa = (UINT8 *)a + DEV_CLASS_LEN - 1; for (ijk = 0; ijk < DEV_CLASS_LEN; ijk++) *_pa-- = *p++;}
#define STREAM_TO_LAP(a, p)      {register int ijk; register UINT8 *plap = (UINT8 *)a + LAP_LEN - 1; for (ijk = 0; ijk < LAP_LEN; ijk++) *plap-- = *p++;}
#define STREAM_TO_ARRAY(a, p, len) {register int ijk; for (ijk = 0; ijk < len; ijk++) ((UINT8 *) a)[ijk] = *p++;}
#define REVERSE_STREAM_TO_ARRAY(a, p, len) {register int ijk; register UINT8 *_pa = (UINT8 *)a + len - 1; for (ijk = 0; ijk < len; ijk++) *_pa-- = *p++;}

/********************************************************************************
** Macros to get and put bytes to and from a field (Little Endian format).
** These are the same as to stream, except the pointer is not incremented.
*/
#define UINT32_TO_FIELD(p, u32) {*(UINT8 *)(p) = (UINT8)(u32); *((UINT8 *)(p)+1) = (UINT8)((u32) >> 8); *((UINT8 *)(p)+2) = (UINT8)((u32) >> 16); *((UINT8 *)(p)+3) = (UINT8)((u32) >> 24);}
#define UINT24_TO_FIELD(p, u24) {*(UINT8 *)(p) = (UINT8)(u24); *((UINT8 *)(p)+1) = (UINT8)((u24) >> 8); *((UINT8 *)(p)+2) = (UINT8)((u24) >> 16);}
#define UINT16_TO_FIELD(p, u16) {*(UINT8 *)(p) = (UINT8)(u16); *((UINT8 *)(p)+1) = (UINT8)((u16) >> 8);}
#define UINT8_TO_FIELD(p, u8)   {*(UINT8 *)(p) = (UINT8)(u8);}


/********************************************************************************
** Macros to get and put bytes to and from a stream (Big Endian format)
*/
#define UINT32_TO_BE_STREAM(p, u32) {*(p)++ = (UINT8)((u32) >> 24);  *(p)++ = (UINT8)((u32) >> 16); *(p)++ = (UINT8)((u32) >> 8); *(p)++ = (UINT8)(u32); }
#define UINT24_TO_BE_STREAM(p, u24) {*(p)++ = (UINT8)((u24) >> 16); *(p)++ = (UINT8)((u24) >> 8); *(p)++ = (UINT8)(u24);}
#define UINT16_TO_BE_STREAM(p, u16) {*(p)++ = (UINT8)((u16) >> 8); *(p)++ = (UINT8)(u16);}
#define UINT8_TO_BE_STREAM(p, u8)   {*(p)++ = (UINT8)(u8);}
#define ARRAY_TO_BE_STREAM(p, a, len) {register int ijk; for (ijk = 0; ijk < len; ijk++) *(p)++ = (UINT8) a[ijk];}

#define BE_STREAM_TO_UINT8(u8, p)   {u8 = (UINT8)(*(p)); (p) += 1;}
#define BE_STREAM_TO_UINT16(u16, p) {u16 = (UINT16)(((UINT16)(*(p)) << 8) + (UINT16)(*((p) + 1))); (p) += 2;}
#define BE_STREAM_TO_UINT24(u32, p) {u32 = (((UINT32)(*((p) + 2))) + ((UINT32)(*((p) + 1)) << 8) + ((UINT32)(*(p)) << 16)); (p) += 3;}
#define BE_STREAM_TO_UINT32(u32, p) {u32 = ((UINT32)(*((p) + 3)) + ((UINT32)(*((p) + 2)) << 8) + ((UINT32)(*((p) + 1)) << 16) + ((UINT32)(*(p)) << 24)); (p) += 4;}
#define BE_STREAM_TO_ARRAY(p, a, len) {register int ijk; for (ijk = 0; ijk < len; ijk++) ((UINT8 *) a)[ijk] = *p++;}


/********************************************************************************
** Macros to get and put bytes to and from a field (Big Endian format).
** These are the same as to stream, except the pointer is not incremented.
*/
#define UINT32_TO_BE_FIELD(p, u32) {*(UINT8 *)(p) = (UINT8)((u32) >> 24);  *((UINT8 *)(p)+1) = (UINT8)((u32) >> 16); *((UINT8 *)(p)+2) = (UINT8)((u32) >> 8); *((UINT8 *)(p)+3) = (UINT8)(u32); }
#define UINT24_TO_BE_FIELD(p, u24) {*(UINT8 *)(p) = (UINT8)((u24) >> 16); *((UINT8 *)(p)+1) = (UINT8)((u24) >> 8); *((UINT8 *)(p)+2) = (UINT8)(u24);}
#define UINT16_TO_BE_FIELD(p, u16) {*(UINT8 *)(p) = (UINT8)((u16) >> 8); *((UINT8 *)(p)+1) = (UINT8)(u16);}
#define UINT8_TO_BE_FIELD(p, u8)   {*(UINT8 *)(p) = (UINT8)(u8);}


/* Common Bluetooth field definitions */
#define BD_ADDR_LEN     6                   /* Device address length */
typedef UINT8 BD_ADDR[BD_ADDR_LEN];         /* Device address */
typedef UINT8 *BD_ADDR_PTR;                 /* Pointer to Device Address */

#define AMP_KEY_TYPE_GAMP       0
#define AMP_KEY_TYPE_WIFI       1
#define AMP_KEY_TYPE_UWB        2
typedef UINT8 tAMP_KEY_TYPE;

#define BT_OCTET8_LEN    8
typedef UINT8 BT_OCTET8[BT_OCTET8_LEN];   /* octet array: size 16 */

#define LINK_KEY_LEN    16
typedef UINT8 LINK_KEY[LINK_KEY_LEN];       /* Link Key */

#define AMP_LINK_KEY_LEN        32
typedef UINT8 AMP_LINK_KEY[AMP_LINK_KEY_LEN];   /* Dedicated AMP and GAMP Link Keys */

#define BT_OCTET16_LEN    16
typedef UINT8 BT_OCTET16[BT_OCTET16_LEN];   /* octet array: size 16 */

#define PIN_CODE_LEN    16
typedef UINT8 PIN_CODE[PIN_CODE_LEN];       /* Pin Code (upto 128 bits) MSB is 0 */
typedef UINT8 *PIN_CODE_PTR;                /* Pointer to Pin Code */

#define DEV_CLASS_LEN   3
typedef UINT8 DEV_CLASS[DEV_CLASS_LEN];     /* Device class */
typedef UINT8 *DEV_CLASS_PTR;               /* Pointer to Device class */

#define EXT_INQ_RESP_LEN   3
typedef UINT8 EXT_INQ_RESP[EXT_INQ_RESP_LEN];/* Extended Inquiry Response */
typedef UINT8 *EXT_INQ_RESP_PTR;             /* Pointer to Extended Inquiry Response */

#define BD_NAME_LEN     248
typedef UINT8 BD_NAME[BD_NAME_LEN];         /* Device name */
typedef UINT8 *BD_NAME_PTR;                 /* Pointer to Device name */

#define BD_FEATURES_LEN 8
typedef UINT8 BD_FEATURES[BD_FEATURES_LEN]; /* LMP features supported by device */

#define BT_EVENT_MASK_LEN  8
typedef UINT8 BT_EVENT_MASK[BT_EVENT_MASK_LEN];   /* Event Mask */

#define LAP_LEN         3
typedef UINT8 LAP[LAP_LEN];                 /* IAC as passed to Inquiry (LAP) */
typedef UINT8 INQ_LAP[LAP_LEN];             /* IAC as passed to Inquiry (LAP) */

#define RAND_NUM_LEN    16
typedef UINT8 RAND_NUM[RAND_NUM_LEN];

#define ACO_LEN         12
typedef UINT8 ACO[ACO_LEN];                 /* Authenticated ciphering offset */

#define COF_LEN         12
typedef UINT8 COF[COF_LEN];                 /* ciphering offset number */

typedef struct {
    UINT8               qos_flags;          /* TBD */
    UINT8               service_type;       /* see below */
    UINT32              token_rate;         /* bytes/second */
    UINT32              token_bucket_size;  /* bytes */
    UINT32              peak_bandwidth;     /* bytes/second */
    UINT32              latency;            /* microseconds */
    UINT32              delay_variation;    /* microseconds */
} FLOW_SPEC;

/* Values for service_type */
#define NO_TRAFFIC      0
#define BEST_EFFORT     1
#define GUARANTEED      2

/* Service class of the CoD */
#define SERV_CLASS_NETWORKING               (1 << 1)
#define SERV_CLASS_RENDERING                (1 << 2)
#define SERV_CLASS_CAPTURING                (1 << 3)
#define SERV_CLASS_OBJECT_TRANSFER          (1 << 4)
#define SERV_CLASS_OBJECT_AUDIO             (1 << 5)
#define SERV_CLASS_OBJECT_TELEPHONY         (1 << 6)
#define SERV_CLASS_OBJECT_INFORMATION       (1 << 7)

/* Second byte */
#define SERV_CLASS_LIMITED_DISC_MODE        (0x20)

/* Field size definitions. Note that byte lengths are rounded up. */
#define ACCESS_CODE_BIT_LEN             72
#define ACCESS_CODE_BYTE_LEN            9
#define SHORTENED_ACCESS_CODE_BIT_LEN   68

typedef UINT8 ACCESS_CODE[ACCESS_CODE_BYTE_LEN];

#define SYNTH_TX                1           /* want synth code to TRANSMIT at this freq */
#define SYNTH_RX                2           /* want synth code to RECEIVE at this freq */

#define SYNC_REPS 1             /* repeats of sync word transmitted to start of burst */

/* Bluetooth CLK27 */
#define BT_CLK27            (2 << 26)

/* Bluetooth CLK12 is 1.28 sec */
#define BT_CLK12_TO_MS(x)    ((x) * 1280)
#define BT_MS_TO_CLK12(x)    ((x) / 1280)
#define BT_CLK12_TO_SLOTS(x) ((x) << 11)

/* Bluetooth CLK is 0.625 msec */
#define BT_CLK_TO_MS(x)      (((x) * 5 + 3) / 8)
#define BT_MS_TO_CLK(x)      (((x) * 8 + 2) / 5)

#define BT_CLK_TO_MICROSECS(x)  (((x) * 5000 + 3) / 8)
#define BT_MICROSECS_TO_CLK(x)  (((x) * 8 + 2499) / 5000)

/* Maximum UUID size - 16 bytes, and structure to hold any type of UUID. */
#define MAX_UUID_SIZE              16
typedef struct
{
#define LEN_UUID_16     2
#define LEN_UUID_32     4
#define LEN_UUID_128    16

    UINT16          len;

    union
    {
        UINT16      uuid16;
        UINT32      uuid32;
        UINT8       uuid128[MAX_UUID_SIZE];
    } uu;

} tBT_UUID;

#define BT_EIR_FLAGS_TYPE                   0x01
#define BT_EIR_MORE_16BITS_UUID_TYPE        0x02
#define BT_EIR_COMPLETE_16BITS_UUID_TYPE    0x03
#define BT_EIR_MORE_32BITS_UUID_TYPE        0x04
#define BT_EIR_COMPLETE_32BITS_UUID_TYPE    0x05
#define BT_EIR_MORE_128BITS_UUID_TYPE       0x06
#define BT_EIR_COMPLETE_128BITS_UUID_TYPE   0x07
#define BT_EIR_SHORTENED_LOCAL_NAME_TYPE    0x08
#define BT_EIR_COMPLETE_LOCAL_NAME_TYPE     0x09
#define BT_EIR_TX_POWER_LEVEL_TYPE          0x0A
#define BT_EIR_OOB_BD_ADDR_TYPE             0x0C
#define BT_EIR_OOB_COD_TYPE                 0x0D
#define BT_EIR_OOB_SSP_HASH_C_TYPE          0x0E
#define BT_EIR_OOB_SSP_RAND_R_TYPE          0x0F
#define BT_EIR_MANUFACTURER_SPECIFIC_TYPE   0xFF

#define BT_OOB_COD_SIZE            3
#define BT_OOB_HASH_C_SIZE         16
#define BT_OOB_RAND_R_SIZE         16

/* Broadcom proprietary UUIDs and reserved PSMs
**
** The lowest 4 bytes byte of the UUID or GUID depends on the feature. Typically,
** the value of those bytes will be the PSM or SCN, but it is up to the features.
*/
#define BRCM_PROPRIETARY_UUID_BASE  0xDA, 0x23, 0x41, 0x02, 0xA3, 0xBB, 0xC1, 0x71, 0xBA, 0x09, 0x6f, 0x21
#define BRCM_PROPRIETARY_GUID_BASE  0xda23, 0x4102, 0xa3, 0xbb, 0xc1, 0x71, 0xba, 0x09, 0x6f, 0x21

/* We will not allocate a PSM in the reserved range to 3rd party apps
*/
#define BRCM_RESERVED_PSM_START	    0x5AE1
#define BRCM_RESERVED_PSM_END	    0x5AFF

#define BRCM_UTILITY_SERVICE_PSM    0x5AE1
#define BRCM_MATCHER_PSM            0x5AE3

/* Connection statistics
*/

/* Structure to hold connection stats */
#ifndef BT_CONN_STATS_DEFINED
#define BT_CONN_STATS_DEFINED

/* These bits are used in the bIsConnected field */
#define BT_CONNECTED_USING_BREDR   1
#define BT_CONNECTED_USING_AMP     2

typedef struct
{
    UINT32   is_connected;
    INT32    rssi;
    UINT32   bytes_sent;
    UINT32   bytes_rcvd;
    UINT32   duration;
} tBT_CONN_STATS;

#endif


/*****************************************************************************
**                          Low Energy definitions
**
** Address types
*/
#define BLE_ADDR_PUBLIC         0x00
#define BLE_ADDR_RANDOM         0x01
#define BLE_ADDR_TYPE_MASK      (BLE_ADDR_RANDOM | BLE_ADDR_PUBLIC)
typedef UINT8 tBLE_ADDR_TYPE;

#define BLE_ADDR_IS_STATIC(x)   ((x[0] & 0xC0) == 0xC0)

typedef struct
{
    tBLE_ADDR_TYPE      type;
    BD_ADDR             bda;
} tBLE_BD_ADDR;

/* Device Types
*/
#define BT_DEVICE_TYPE_BREDR   0x01
#define BT_DEVICE_TYPE_BLE     0x02
#define BT_DEVICE_TYPE_DUMO    0x03
typedef UINT8 tBT_DEVICE_TYPE;
/*****************************************************************************/


/* Define trace levels */
#define BT_TRACE_LEVEL_NONE    0          /* No trace messages to be generated    */
#define BT_TRACE_LEVEL_ERROR   1          /* Error condition trace messages       */
#define BT_TRACE_LEVEL_WARNING 2          /* Warning condition trace messages     */
#define BT_TRACE_LEVEL_API     3          /* API traces                           */
#define BT_TRACE_LEVEL_EVENT   4          /* Debug messages for events            */
#define BT_TRACE_LEVEL_DEBUG   5          /* Full debug messages                  */
#define BT_TRACE_LEVEL_VERBOSE 6          /* Verbose debug messages               */

#define MAX_TRACE_LEVEL        6


/* Define New Trace Type Definition */
/* TRACE_CTRL_TYPE                  0x^^000000*/
#define TRACE_CTRL_MASK             0xff000000
#define TRACE_GET_CTRL(x)           ((((UINT32)(x)) & TRACE_CTRL_MASK) >> 24)

#define TRACE_CTRL_GENERAL          0x00000000
#define TRACE_CTRL_STR_RESOURCE     0x01000000
#define TRACE_CTRL_SEQ_FLOW         0x02000000
#define TRACE_CTRL_MAX_NUM          3

/* LAYER SPECIFIC                   0x00^^0000*/
#define TRACE_LAYER_MASK            0x00ff0000
#define TRACE_GET_LAYER(x)          ((((UINT32)(x)) & TRACE_LAYER_MASK) >> 16)

#define TRACE_LAYER_NONE            0x00000000
#define TRACE_LAYER_USB             0x00010000
#define TRACE_LAYER_SERIAL          0x00020000
#define TRACE_LAYER_SOCKET          0x00030000
#define TRACE_LAYER_RS232           0x00040000
#define TRACE_LAYER_TRANS_MAX_NUM   5
#define TRACE_LAYER_TRANS_ALL       0x007f0000
#define TRACE_LAYER_LC              0x00050000
#define TRACE_LAYER_LM              0x00060000
#define TRACE_LAYER_HCI             0x00070000
#define TRACE_LAYER_L2CAP           0x00080000
#define TRACE_LAYER_RFCOMM          0x00090000
#define TRACE_LAYER_SDP             0x000a0000
#define TRACE_LAYER_TCS             0x000b0000
#define TRACE_LAYER_OBEX            0x000c0000
#define TRACE_LAYER_BTM             0x000d0000
#define TRACE_LAYER_GAP             0x000e0000
#define TRACE_LAYER_DUN             0x000f0000
#define TRACE_LAYER_GOEP            0x00100000
#define TRACE_LAYER_ICP             0x00110000
#define TRACE_LAYER_HSP2            0x00120000
#define TRACE_LAYER_SPP             0x00130000
#define TRACE_LAYER_CTP             0x00140000
#define TRACE_LAYER_BPP             0x00150000
#define TRACE_LAYER_HCRP            0x00160000
#define TRACE_LAYER_FTP             0x00170000
#define TRACE_LAYER_OPP             0x00180000
#define TRACE_LAYER_BTU             0x00190000
#define TRACE_LAYER_GKI             0x001a0000
#define TRACE_LAYER_BNEP            0x001b0000
#define TRACE_LAYER_PAN             0x001c0000
#define TRACE_LAYER_HFP             0x001d0000
#define TRACE_LAYER_HID             0x001e0000
#define TRACE_LAYER_BIP             0x001f0000
#define TRACE_LAYER_AVP             0x00200000
#define TRACE_LAYER_A2D             0x00210000
#define TRACE_LAYER_SAP             0x00220000
#define TRACE_LAYER_AMP             0x00230000
#define TRACE_LAYER_MCA             0x00240000
#define TRACE_LAYER_ATT             0x00250000
#define TRACE_LAYER_SMP             0x00260000
#define TRACE_LAYER_NFC             0x00270000
#define TRACE_LAYER_NCI             0x00280000
#define TRACE_LAYER_IDEP            0x00290000
#define TRACE_LAYER_NDEP            0x002a0000
#define TRACE_LAYER_LLCP            0x002b0000
#define TRACE_LAYER_RW              0x002c0000
#define TRACE_LAYER_CE              0x002d0000
#define TRACE_LAYER_SNEP            0x002e0000
#define TRACE_LAYER_NDEF            0x002f0000
#define TRACE_LAYER_NFA             0x00300000

#define TRACE_LAYER_MAX_NUM         0x0031


/* TRACE_ORIGINATOR                 0x0000^^00*/
#define TRACE_ORG_MASK              0x0000ff00
#define TRACE_GET_ORG(x)            ((((UINT32)(x)) & TRACE_ORG_MASK) >> 8)

#define TRACE_ORG_STACK             0x00000000
#define TRACE_ORG_HCI_TRANS         0x00000100
#define TRACE_ORG_PROTO_DISP        0x00000200
#define TRACE_ORG_RPC               0x00000300
#define TRACE_ORG_GKI               0x00000400
#define TRACE_ORG_APPL              0x00000500
#define TRACE_ORG_SCR_WRAPPER       0x00000600
#define TRACE_ORG_SCR_ENGINE        0x00000700
#define TRACE_ORG_USER_SCR          0x00000800
#define TRACE_ORG_TESTER            0x00000900
#define TRACE_ORG_MAX_NUM           10          /* 32-bit mask; must be < 32 */
#define TRACE_LITE_ORG_MAX_NUM		6
#define TRACE_ORG_ALL               0x03ff
#define TRACE_ORG_RPC_TRANS         0x04

#define TRACE_ORG_REG               0x00000909
#define TRACE_ORG_REG_SUCCESS       0x0000090a

/* TRACE_TYPE                       0x000000^^*/
#define TRACE_TYPE_MASK             0x000000ff
#define TRACE_GET_TYPE(x)           (((UINT32)(x)) & TRACE_TYPE_MASK)

#define TRACE_TYPE_ERROR            0x00000000
#define TRACE_TYPE_WARNING          0x00000001
#define TRACE_TYPE_API              0x00000002
#define TRACE_TYPE_EVENT            0x00000003
#define TRACE_TYPE_DEBUG            0x00000004
#define TRACE_TYPE_STACK_ONLY_MAX   TRACE_TYPE_DEBUG
#define TRACE_TYPE_TX               0x00000005
#define TRACE_TYPE_RX               0x00000006
#define TRACE_TYPE_DEBUG_ASSERT     0x00000007
#define TRACE_TYPE_GENERIC          0x00000008
#define TRACE_TYPE_REG              0x00000009
#define TRACE_TYPE_REG_SUCCESS      0x0000000a
#define TRACE_TYPE_CMD_TX           0x0000000b
#define TRACE_TYPE_EVT_TX           0x0000000c
#define TRACE_TYPE_ACL_TX           0x0000000d
#define TRACE_TYPE_CMD_RX           0x0000000e
#define TRACE_TYPE_EVT_RX           0x0000000f
#define TRACE_TYPE_ACL_RX           0x00000010
#define TRACE_TYPE_TARGET_TRACE     0x00000011
#define TRACE_TYPE_SCO_TX           0x00000012
#define TRACE_TYPE_SCO_RX           0x00000013


#define TRACE_TYPE_MAX_NUM          20
#define TRACE_TYPE_ALL              0xffff

/* Define color for script type */
#define SCR_COLOR_DEFAULT       0
#define SCR_COLOR_TYPE_COMMENT  1
#define SCR_COLOR_TYPE_COMMAND  2
#define SCR_COLOR_TYPE_EVENT    3
#define SCR_COLOR_TYPE_SELECT   4

/* Define protocol trace flag values */
#define SCR_PROTO_TRACE_HCI_SUMMARY 0x00000001
#define SCR_PROTO_TRACE_HCI_DATA    0x00000002
#define SCR_PROTO_TRACE_L2CAP       0x00000004
#define SCR_PROTO_TRACE_RFCOMM      0x00000008
#define SCR_PROTO_TRACE_SDP         0x00000010
#define SCR_PROTO_TRACE_TCS         0x00000020
#define SCR_PROTO_TRACE_OBEX        0x00000040
#define SCR_PROTO_TRACE_OAPP        0x00000080 /* OBEX Application Profile */
#define SCR_PROTO_TRACE_AMP         0x00000100
#define SCR_PROTO_TRACE_BNEP        0x00000200
#define SCR_PROTO_TRACE_AVP         0x00000400
#define SCR_PROTO_TRACE_MCA         0x00000800
#define SCR_PROTO_TRACE_ATT         0x00001000
#define SCR_PROTO_TRACE_SMP         0x00002000
#define SCR_PROTO_TRACE_NCI         0x00004000
#define SCR_PROTO_TRACE_DEP         0x00008000
#define SCR_PROTO_TRACE_LLCP        0x00010000
#define SCR_PROTO_TRACE_NDEF        0x00020000
#define SCR_PROTO_TRACE_TAGS        0x00040000
#define SCR_PROTO_TRACE_ALL         0x0007ffff
#define SCR_PROTO_TRACE_HCI_LOGGING_VSE 0x0800 /* Brcm vs event for logmsg and protocol traces */

#define MAX_SCRIPT_TYPE             5

#define TCS_PSM_INTERCOM        5
#define TCS_PSM_CORDLESS        7
#define BT_PSM_BNEP             0x000F
/* Define PSMs HID uses */
#define HID_PSM_CONTROL         0x0011
#define HID_PSM_INTERRUPT       0x0013

/* Define a function for logging */
typedef void (BT_LOG_FUNC) (int trace_type, const char *fmt_str, ...);

#endif
/* end "revolution/BTE/stack/include/bt_types.h" */


//------------------Added from Bluedroid buildcfg.h---------------------
#ifndef UNV_INCLUDED
#define UNV_INCLUDED FALSE
#endif

#ifndef GATT_PTS
#define GATT_PTS FALSE
#endif

#ifndef L2CAP_INCLUDED
#define L2CAP_INCLUDED TRUE
#endif

#ifndef L2CAP_EXTFEA_SUPPORTED_MASK
#define L2CAP_EXTFEA_SUPPORTED_MASK (L2CAP_EXTFEA_ENH_RETRANS | L2CAP_EXTFEA_STREAM_MODE | L2CAP_EXTFEA_NO_CRC | L2CAP_EXTFEA_FIXED_CHNLS)
#endif

#ifndef BTUI_OPS_FORMATS
#define BTUI_OPS_FORMATS (BTA_OP_VCARD21_MASK | BTA_OP_ANY_MASK)
#endif

#ifndef BTA_RFC_MTU_SIZE
#define BTA_RFC_MTU_SIZE (L2CAP_MTU_SIZE-L2CAP_MIN_OFFSET-RFCOMM_DATA_OVERHEAD)
#endif

#ifndef BTA_DUN_MTU
#define BTA_DUN_MTU BTA_RFC_MTU_SIZE
#endif

#ifndef BTA_SPP_MTU
#define BTA_SPP_MTU BTA_RFC_MTU_SIZE
#endif

#ifndef BTA_FAX_MTU
#define BTA_FAX_MTU BTA_RFC_MTU_SIZE
#endif

#ifndef SDP_RAW_PDU_INCLUDED
#define SDP_RAW_PDU_INCLUDED  TRUE
#endif

#ifndef GATTS_APPU_USE_GATT_TRACE
#define GATTS_APPU_USE_GATT_TRACE FALSE
#endif

#ifndef SMP_HOST_ENCRYPT_INCLUDED
#define SMP_HOST_ENCRYPT_INCLUDED FALSE
#endif

#ifndef SAP_INCLUDED
#define SAP_INCLUDED FALSE
#endif

#ifndef SBC_NO_PCM_CPY_OPTION
#define SBC_NO_PCM_CPY_OPTION FALSE
#endif

#ifndef SBC_IPAQ_OPT
#define SBC_IPAQ_OPT FALSE
#endif

#ifndef SBC_IS_64_MULT_IN_QUANTIZER
#define SBC_IS_64_MULT_IN_QUANTIZER FALSE
#endif

#ifndef BTA_INCLUDED
#define BTA_INCLUDED TRUE
#endif

#ifndef BTA_AG_INCLUDED
#define BTA_AG_INCLUDED  TRUE
#endif

#ifndef BTA_CT_INCLUDED
#define BTA_CT_INCLUDED  FALSE
#endif

#ifndef BTA_CG_INCLUDED
#define BTA_CG_INCLUDED  FALSE
#endif

#ifndef BTA_DG_INCLUDED
#define BTA_DG_INCLUDED  FALSE
#endif

#ifndef BTA_FT_INCLUDED
#define BTA_FT_INCLUDED FALSE
#endif

#ifndef BTA_OP_INCLUDED
#define BTA_OP_INCLUDED FALSE
#endif

#ifndef BTA_PR_INCLUDED
#define BTA_PR_INCLUDED FALSE
#endif

#ifndef BTA_SS_INCLUDED
#define BTA_SS_INCLUDED FALSE
#endif

#ifndef BTA_DM_INCLUDED
#define BTA_DM_INCLUDED TRUE
#endif


#ifndef BTA_DI_INCLUDED
#define BTA_DI_INCLUDED FALSE
#endif

#ifndef BTA_BI_INCLUDED
#define BTA_BI_INCLUDED FALSE
#endif

#ifndef BTA_SC_INCLUDED
#define BTA_SC_INCLUDED FALSE
#endif

#ifndef BTA_PAN_INCLUDED
#define BTA_PAN_INCLUDED TRUE
#endif

#ifndef BTA_FS_INCLUDED
#define BTA_FS_INCLUDED TRUE
#endif

#ifndef BTA_AC_INCLUDED
#define BTA_AC_INCLUDED FALSE
#endif

#ifndef BTA_HD_INCLUDED
#define BTA_HD_INCLUDED FALSE
#endif

#ifndef BTA_HH_INCLUDED
#define BTA_HH_INCLUDED TRUE
#endif

#ifndef BTA_HH_ROLE
#define BTA_HH_ROLE BTA_MASTER_ROLE_PREF
#endif

#ifndef BTA_AR_INCLUDED
#define BTA_AR_INCLUDED TRUE
#endif

#ifndef BTA_AV_INCLUDED
#define BTA_AV_INCLUDED TRUE
#endif

#ifndef BTA_AV_VDP_INCLUDED
#define BTA_AV_VDP_INCLUDED FALSE
#endif

#ifndef BTA_AVK_INCLUDED
#define BTA_AVK_INCLUDED FALSE
#endif

#ifndef BTA_PBS_INCLUDED
#define BTA_PBS_INCLUDED FALSE
#endif

#ifndef BTA_PBC_INCLUDED
#define BTA_PBC_INCLUDED FALSE
#endif

#ifndef BTA_FM_INCLUDED
#define BTA_FM_INCLUDED FALSE
#endif

#ifndef BTA_FM_DEBUG
#define BTA_FM_DEBUG FALSE
#endif

#ifndef BTA_FMTX_INCLUDED
#define BTA_FMTX_INCLUDED FALSE
#endif

#ifndef BTA_FMTX_DEBUG
#define BTA_FMTX_DEBUG FALSE
#endif

#ifndef BTA_FMTX_FMRX_SWITCH_WORKAROUND
#define BTA_FMTX_FMRX_SWITCH_WORKAROUND FALSE
#endif

#ifndef BTA_FMTX_US_FCC_RULES
#define BTA_FMTX_US_FCC_RULES FALSE
#endif

#ifndef BTA_HS_INCLUDED
#define BTA_HS_INCLUDED FALSE
#endif

#ifndef BTA_MSE_INCLUDED
#define BTA_MSE_INCLUDED FALSE
#endif

#ifndef BTA_MCE_INCLUDED
#define BTA_MCE_INCLUDED FALSE
#endif

#ifndef BTA_PLAYBACK_INCLUDED
#define BTA_PLAYBACK_INCLUDED FALSE
#endif

#ifndef BTA_SSR_INCLUDED
#define BTA_SSR_INCLUDED FALSE
#endif

#ifndef BTA_JV_INCLUDED
#define BTA_JV_INCLUDED FALSE
#endif

#ifndef BTA_GATT_INCLUDED
#define BTA_GATT_INCLUDED FALSE
#endif

#ifndef BTA_DISABLE_DELAY
#define BTA_DISABLE_DELAY 200 /* in milliseconds */
#endif

#ifndef RPC_TRACE_ONLY
#define RPC_TRACE_ONLY  FALSE
#endif

#ifndef ANDROID_APP_INCLUDED
#define ANDROID_APP_INCLUDED  TRUE
#endif

#ifndef ANDROID_USE_LOGCAT
#define ANDROID_USE_LOGCAT  TRUE
#endif

#ifndef LINUX_GKI_INCLUDED
#define LINUX_GKI_INCLUDED  TRUE
#endif

#ifndef BTA_SYS_TIMER_PERIOD
#define BTA_SYS_TIMER_PERIOD  100
#endif

#ifndef GKI_SHUTDOWN_EVT
#define GKI_SHUTDOWN_EVT  APPL_EVT_7
#endif

#ifndef GKI_PTHREAD_JOINABLE
#define GKI_PTHREAD_JOINABLE  TRUE
#endif

#ifndef LINUX_DRV_INCLUDED
#define LINUX_DRV_INCLUDED  TRUE
#endif

#ifndef LINUX_OS
#define LINUX_OS  TRUE
#endif

#ifndef BTM_APP_DEV_INIT
#define BTM_APP_DEV_INIT  bte_main_post_reset_init
#endif

#ifndef SBC_FOR_EMBEDDED_LINUX
#define SBC_FOR_EMBEDDED_LINUX TRUE
#endif

#ifndef BTA_DM_REMOTE_DEVICE_NAME_LENGTH
#define BTA_DM_REMOTE_DEVICE_NAME_LENGTH 248
#endif

#ifndef AVDT_VERSION
#define AVDT_VERSION  0x0102
#endif

#ifndef BTA_AG_AT_MAX_LEN
#define BTA_AG_AT_MAX_LEN  512
#endif

#ifndef BTA_AVRCP_FF_RW_SUPPORT
#define BTA_AVRCP_FF_RW_SUPPORT TRUE
#endif

#ifndef BTA_AG_SCO_PKT_TYPES
#define BTA_AG_SCO_PKT_TYPES  (BTM_SCO_LINK_ONLY_MASK | BTM_SCO_PKT_TYPES_MASK_EV3 |  BTM_SCO_PKT_TYPES_MASK_NO_3_EV3 | BTM_SCO_PKT_TYPES_MASK_NO_2_EV5 | BTM_SCO_PKT_TYPES_MASK_NO_3_EV5)
#endif

#ifndef BTA_AV_MAX_A2DP_MTU
#define BTA_AV_MAX_A2DP_MTU  668
#endif

#ifndef BTA_AV_RET_TOUT
#define BTA_AV_RET_TOUT 15
#endif

#ifndef PORCHE_PAIRING_CONFLICT
#define PORCHE_PAIRING_CONFLICT  TRUE
#endif

#ifndef BTA_AV_CO_CP_SCMS_T
#define BTA_AV_CO_CP_SCMS_T  FALSE
#endif

#ifndef AVDT_CONNECT_CP_ONLY
#define AVDT_CONNECT_CP_ONLY  FALSE
#endif

#ifndef BT_TRACE_PROTOCOL
#define BT_TRACE_PROTOCOL  TRUE
#endif

#ifndef BT_USE_TRACES
#define BT_USE_TRACES  TRUE
#endif

#ifndef BT_TRACE_BTIF
#define BT_TRACE_BTIF  TRUE
#endif

#ifndef BTTRC_INCLUDED
#define BTTRC_INCLUDED  FALSE
#endif

#ifndef BT_TRACE_VERBOSE
#define BT_TRACE_VERBOSE  FALSE
#endif

#ifndef BTTRC_PARSER_INCLUDED
#define BTTRC_PARSER_INCLUDED  FALSE
#endif

#ifndef MAX_TRACE_RAM_SIZE
#define MAX_TRACE_RAM_SIZE  10000
#endif

#ifndef OBX_INITIAL_TRACE_LEVEL
#define OBX_INITIAL_TRACE_LEVEL  BT_TRACE_LEVEL_ERROR
#endif

#ifndef PBAP_ZERO_VCARD_IN_DB
#define PBAP_ZERO_VCARD_IN_DB  FALSE
#endif

#ifndef BTA_DM_SDP_DB_SIZE
#define BTA_DM_SDP_DB_SIZE  8000
#endif

#ifndef FTS_REJECT_INVALID_OBEX_SET_PATH_REQ
#define FTS_REJECT_INVALID_OBEX_SET_PATH_REQ FALSE
#endif

#ifndef HL_INCLUDED
#define HL_INCLUDED  TRUE
#endif

#ifndef NO_GKI_RUN_RETURN
#define NO_GKI_RUN_RETURN  TRUE
#endif

#ifndef AG_VOICE_SETTINGS
#define AG_VOICE_SETTINGS  HCI_DEFAULT_VOICE_SETTINGS
#endif

#ifndef BTIF_DM_OOB_TEST
#define BTIF_DM_OOB_TEST  TRUE
#endif
//------------------End added from Bluedroid buildcfg.h---------------------



/* #define BYPASS_AVDATATRACE */

/******************************************************************************
**
** Platform-Specific
**
******************************************************************************/

/* API macros for simulator */

#define BTAPI

#ifndef BTE_BSE_WRAPPER
#ifdef  BTE_SIM_APP
#undef  BTAPI
#define BTAPI         __declspec(dllexport)
#endif
#endif

#define BT_API          BTAPI
#define BTU_API         BTAPI
#define A2D_API         BTAPI
#define VDP_API         BTAPI
#define AVDT_API        BTAPI
#define AVCT_API        BTAPI
#define AVRC_API        BTAPI
#define BIP_API         BTAPI
#define BNEP_API        BTAPI
#define BPP_API         BTAPI
#define BTM_API         BTAPI
#define CTP_API         BTAPI
#define DUN_API         BTAPI
#define FTP_API         BTAPI
#define GAP_API         BTAPI
#define GOEP_API        BTAPI
#define HCI_API         BTAPI
#define HCRP_API        BTAPI
#define HID_API         BTAPI
#define HFP_API         BTAPI
#define HSP2_API        BTAPI
#define ICP_API         BTAPI
#define L2C_API         BTAPI
#define OBX_API         BTAPI
#define OPP_API         BTAPI
#define PAN_API         BTAPI
#define RFC_API         BTAPI
#define RPC_API         BTAPI
#define SDP_API         BTAPI
#define SPP_API         BTAPI
#define TCS_API         BTAPI
#define XML_API         BTAPI
#define BTA_API         BTAPI
#define SBC_API         BTAPI
#define MCE_API         BTAPI
#define MCA_API         BTAPI
#define GATT_API        BTAPI
#define SMP_API         BTAPI


/******************************************************************************
**
** GKI Buffer Pools
**
******************************************************************************/

/* Receives HCI events from the lower-layer. */
#ifndef HCI_CMD_POOL_ID
#define HCI_CMD_POOL_ID             GKI_POOL_ID_2
#endif

#ifndef HCI_CMD_POOL_BUF_SIZE
#define HCI_CMD_POOL_BUF_SIZE       GKI_BUF2_SIZE
#endif

/* Receives ACL data packets from thelower-layer. */
#ifndef HCI_ACL_POOL_ID
#define HCI_ACL_POOL_ID             GKI_POOL_ID_3
#endif

#ifndef HCI_ACL_POOL_BUF_SIZE
#define HCI_ACL_POOL_BUF_SIZE       GKI_BUF3_SIZE
#endif

/* Maximum number of buffers available for ACL receive data. */
#ifndef HCI_ACL_BUF_MAX
#define HCI_ACL_BUF_MAX             GKI_BUF3_MAX
#endif

/* Receives SCO data packets from the lower-layer. */
#ifndef HCI_SCO_POOL_ID
#define HCI_SCO_POOL_ID             GKI_POOL_ID_6
#endif

/* Not used. */
#ifndef HCI_DATA_DESCR_POOL_ID
#define HCI_DATA_DESCR_POOL_ID      GKI_POOL_ID_0
#endif

/* Sends SDP data packets. */
#ifndef SDP_POOL_ID
#define SDP_POOL_ID                 3
#endif

/* Sends RFCOMM command packets. */
#ifndef RFCOMM_CMD_POOL_ID
#define RFCOMM_CMD_POOL_ID          GKI_POOL_ID_2
#endif

#ifndef RFCOMM_CMD_POOL_BUF_SIZE
#define RFCOMM_CMD_POOL_BUF_SIZE    GKI_BUF2_SIZE
#endif

/* Sends RFCOMM data packets. */
#ifndef RFCOMM_DATA_POOL_ID
#define RFCOMM_DATA_POOL_ID         GKI_POOL_ID_3
#endif

#ifndef RFCOMM_DATA_POOL_BUF_SIZE
#define RFCOMM_DATA_POOL_BUF_SIZE   GKI_BUF3_SIZE
#endif

/* Sends L2CAP packets to the peer and HCI messages to the controller. */
#ifndef L2CAP_CMD_POOL_ID
#define L2CAP_CMD_POOL_ID           GKI_POOL_ID_2
#endif

/* Sends L2CAP segmented packets in ERTM mode */
#ifndef L2CAP_FCR_TX_POOL_ID
#define L2CAP_FCR_TX_POOL_ID        HCI_ACL_POOL_ID
#endif

/* Receives L2CAP segmented packets in ERTM mode */
#ifndef L2CAP_FCR_RX_POOL_ID
#define L2CAP_FCR_RX_POOL_ID        HCI_ACL_POOL_ID
#endif

/* Used by BTM when it sends HCI commands to the controller. */
#ifndef BTM_CMD_POOL_ID
#define BTM_CMD_POOL_ID             GKI_POOL_ID_2
#endif

/* Sends TCS messages. */
#ifndef TCS_MSG_POOL_ID
#define TCS_MSG_POOL_ID             GKI_POOL_ID_2
#endif

#ifndef OBX_CMD_POOL_SIZE
#define OBX_CMD_POOL_SIZE           GKI_BUF2_SIZE
#endif

#ifndef OBX_LRG_DATA_POOL_SIZE
#define OBX_LRG_DATA_POOL_SIZE      GKI_BUF4_SIZE
#endif

#ifndef OBX_LRG_DATA_POOL_ID
#define OBX_LRG_DATA_POOL_ID        GKI_POOL_ID_4
#endif

/* Used for CTP discovery database. */
#ifndef CTP_SDP_DB_POOL_ID
#define CTP_SDP_DB_POOL_ID          GKI_POOL_ID_3
#endif

/* Used for CTP data exchange feature. */
#ifndef CTP_DATA_EXCHG_POOL_ID
#define CTP_DATA_EXCHG_POOL_ID      GKI_POOL_ID_2
#endif

/* Used to send data to L2CAP. */
#ifndef GAP_DATA_POOL_ID
#define GAP_DATA_POOL_ID            GKI_POOL_ID_3
#endif

/* Used for SPP inquiry and discovery databases. */
#ifndef SPP_DB_POOL_ID
#define SPP_DB_POOL_ID              GKI_POOL_ID_3
#endif

#ifndef SPP_DB_SIZE
#define SPP_DB_SIZE                 GKI_BUF3_SIZE
#endif

/* HCRP protocol and internal commands. */
#ifndef HCRP_CMD_POOL_ID
#define HCRP_CMD_POOL_ID            GKI_POOL_ID_2
#endif

#ifndef HCRP_CMD_POOL_SIZE
#define HCRP_CMD_POOL_SIZE          GKI_BUF2_SIZE
#endif

#ifndef BIP_EVT_POOL_SIZE
#define BIP_EVT_POOL_SIZE           GKI_BUF3_SIZE
#endif

#ifndef BIP_DB_SIZE
#define BIP_DB_SIZE                 GKI_BUF3_SIZE
#endif


/* BNEP data and protocol messages. */
#ifndef BNEP_POOL_ID
#define BNEP_POOL_ID                GKI_POOL_ID_3
#endif

/* RPC pool for temporary trace message buffers. */
#ifndef RPC_SCRATCH_POOL_ID
#define RPC_SCRATCH_POOL_ID         GKI_POOL_ID_2
#endif

/* RPC scratch buffer size (not related to RPC_SCRATCH_POOL_ID) */
#ifndef RPC_SCRATCH_BUF_SIZE
#define RPC_SCRATCH_BUF_SIZE        GKI_BUF3_SIZE
#endif

/* RPC pool for protocol messages */
#ifndef RPC_MSG_POOL_ID
#define RPC_MSG_POOL_ID             GKI_POOL_ID_3
#endif

#ifndef RPC_MSG_POOL_SIZE
#define RPC_MSG_POOL_SIZE           GKI_BUF3_SIZE
#endif

/* AVDTP pool for protocol messages */
#ifndef AVDT_CMD_POOL_ID
#define AVDT_CMD_POOL_ID            GKI_POOL_ID_2
#endif

/* AVDTP pool size for media packets in case of fragmentation */
#ifndef AVDT_DATA_POOL_SIZE
#define AVDT_DATA_POOL_SIZE         GKI_BUF3_SIZE
#endif

#ifndef PAN_POOL_ID
#define PAN_POOL_ID                 GKI_POOL_ID_3
#endif

/* UNV pool for read/write serialization */
#ifndef UNV_MSG_POOL_ID
#define UNV_MSG_POOL_ID             GKI_POOL_ID_2
#endif

#ifndef UNV_MSG_POOL_SIZE
#define UNV_MSG_POOL_SIZE           GKI_BUF2_SIZE
#endif

/* AVCTP pool for protocol messages */
#ifndef AVCT_CMD_POOL_ID
#define AVCT_CMD_POOL_ID            GKI_POOL_ID_1
#endif

#ifndef AVCT_META_CMD_POOL_ID
#define AVCT_META_CMD_POOL_ID       GKI_POOL_ID_2
#endif

/* AVRCP pool for protocol messages */
#ifndef AVRC_CMD_POOL_ID
#define AVRC_CMD_POOL_ID            GKI_POOL_ID_1
#endif

/* AVRCP pool size for protocol messages */
#ifndef AVRC_CMD_POOL_SIZE
#define AVRC_CMD_POOL_SIZE          GKI_BUF1_SIZE
#endif

/* AVRCP Metadata pool for protocol messages */
#ifndef AVRC_META_CMD_POOL_ID
#define AVRC_META_CMD_POOL_ID       GKI_POOL_ID_2
#endif

/* AVRCP Metadata pool size for protocol messages */
#ifndef AVRC_META_CMD_POOL_SIZE
#define AVRC_META_CMD_POOL_SIZE     GKI_BUF2_SIZE
#endif


/* AVRCP buffer size for browsing channel messages */
#ifndef AVRC_BROWSE_POOL_SIZE
#define AVRC_BROWSE_POOL_SIZE     GKI_MAX_BUF_SIZE
#endif

/*  HDP buffer size for the Pulse Oximeter  */
#ifndef BTA_HL_LRG_DATA_POOL_SIZE
#define BTA_HL_LRG_DATA_POOL_SIZE      GKI_BUF7_SIZE
#endif

#ifndef BTA_HL_LRG_DATA_POOL_ID
#define BTA_HL_LRG_DATA_POOL_ID        GKI_POOL_ID_7
#endif

/* GATT Server Database pool ID */
#ifndef GATT_DB_POOL_ID
#define GATT_DB_POOL_ID                 GKI_POOL_ID_8
#endif

/******************************************************************************
**
** Lower Layer Interface
**
******************************************************************************/

/* Sends ACL data received over HCI to the upper stack. */
#ifndef HCI_ACL_DATA_TO_UPPER
#define HCI_ACL_DATA_TO_UPPER(p)    {((BT_HDR *)p)->event = BT_EVT_TO_BTU_HCI_ACL; GKI_send_msg (BTU_TASK, BTU_HCI_RCV_MBOX, p);}
#endif

/* Sends SCO data received over HCI to the upper stack. */
#ifndef HCI_SCO_DATA_TO_UPPER
#define HCI_SCO_DATA_TO_UPPER(p)    {((BT_HDR *)p)->event = BT_EVT_TO_BTU_HCI_SCO; GKI_send_msg (BTU_TASK, BTU_HCI_RCV_MBOX, p);}
#endif

/* Sends an HCI event received over HCI to theupper stack. */
#ifndef HCI_EVT_TO_UPPER
#define HCI_EVT_TO_UPPER(p)         {((BT_HDR *)p)->event = BT_EVT_TO_BTU_HCI_EVT; GKI_send_msg (BTU_TASK, BTU_HCI_RCV_MBOX, p);}
#endif

/* Macro for allocating buffer for HCI commands */
#ifndef HCI_GET_CMD_BUF
#if (!defined(HCI_USE_VARIABLE_SIZE_CMD_BUF) || (HCI_USE_VARIABLE_SIZE_CMD_BUF == FALSE))
/* Allocate fixed-size buffer from HCI_CMD_POOL (default case) */
#define HCI_GET_CMD_BUF(paramlen)    ((BT_HDR *)GKI_getpoolbuf (HCI_CMD_POOL_ID))
#else
/* Allocate smallest possible buffer (for platforms with limited RAM) */
#define HCI_GET_CMD_BUF(paramlen)    ((BT_HDR *)GKI_getbuf ((UINT16)(BT_HDR_SIZE + HCIC_PREAMBLE_SIZE + (paramlen))))
#endif
#endif  /* HCI_GET_CMD_BUF */

/******************************************************************************
**
** HCI Services (H4)
**
******************************************************************************/
#ifndef HCISU_H4_INCLUDED
#define HCISU_H4_INCLUDED               TRUE
#endif

#ifdef __cplusplus
extern "C" {
#endif

BT_API extern void bte_main_hci_send (BT_HDR *p_msg, UINT16 event);
#if (HCISU_H4_INCLUDED == TRUE)
BT_API extern void bte_main_lpm_allow_bt_device_sleep(void);
#endif

#ifdef __cplusplus
}
#endif

/* Sends ACL data received from the upper stack to the BD/EDR HCI transport. */
#ifndef HCI_ACL_DATA_TO_LOWER
#define HCI_ACL_DATA_TO_LOWER(p)    bte_main_hci_send((BT_HDR *)(p), BT_EVT_TO_LM_HCI_ACL);
#endif

#ifndef HCI_BLE_ACL_DATA_TO_LOWER
#define HCI_BLE_ACL_DATA_TO_LOWER(p)    bte_main_hci_send((BT_HDR *)(p), (UINT16)(BT_EVT_TO_LM_HCI_ACL|LOCAL_BLE_CONTROLLER_ID));
#endif

/* Sends SCO data received from the upper stack to the HCI transport. */
#ifndef HCI_SCO_DATA_TO_LOWER
#define HCI_SCO_DATA_TO_LOWER(p)    bte_main_hci_send((BT_HDR *)(p), BT_EVT_TO_LM_HCI_SCO);
#endif

/* Sends an HCI command received from the upper stack to the BD/EDR HCI transport. */
#ifndef HCI_CMD_TO_LOWER
#define HCI_CMD_TO_LOWER(p)         bte_main_hci_send((BT_HDR *)(p), BT_EVT_TO_LM_HCI_CMD);
#endif

/* Sends an LM Diagnosic command received from the upper stack to the HCI transport. */
#ifndef HCI_LM_DIAG_TO_LOWER
#define HCI_LM_DIAG_TO_LOWER(p)     bte_main_hci_send((BT_HDR *)(p), BT_EVT_TO_LM_DIAG);
#endif

/* Send HCISU a message to allow BT sleep */
#ifndef HCI_LP_ALLOW_BT_DEVICE_SLEEP
#define HCI_LP_ALLOW_BT_DEVICE_SLEEP()       bte_main_lpm_allow_bt_device_sleep()
#endif

/* If nonzero, the upper-layer sends at most this number of HCI commands to the lower-layer. */
#ifndef HCI_MAX_SIMUL_CMDS
#define HCI_MAX_SIMUL_CMDS          0
#endif

/* Timeout for receiving response to HCI command */
#ifndef BTU_CMD_CMPL_TIMEOUT
#define BTU_CMD_CMPL_TIMEOUT        8
#endif

/* If TRUE, BTU task will check HCISU again when HCI command timer expires */
#ifndef BTU_CMD_CMPL_TOUT_DOUBLE_CHECK
#define BTU_CMD_CMPL_TOUT_DOUBLE_CHECK      FALSE
#endif

/* Use 2 second for low-resolution systems, override to 1 for high-resolution systems */
#ifndef BT_1SEC_TIMEOUT
#define BT_1SEC_TIMEOUT             (2)
#endif

/* Quick Timer */
/* if L2CAP_FCR_INCLUDED is TRUE then it should have 100 millisecond resolution */
/* if none of them is included then QUICK_TIMER_TICKS_PER_SEC is set to 0 to exclude quick timer */
#ifndef QUICK_TIMER_TICKS_PER_SEC
#define QUICK_TIMER_TICKS_PER_SEC   10       /* 10ms timer */
#endif

/******************************************************************************
**
** BTM
**
******************************************************************************/
/* if set to TRUE, stack will automatically send an HCI reset at start-up. To be
set to FALSE for advanced start-up / shut-down procedures using USER_HW_ENABLE_API
and USER_HW_DISABLE_API macros */
#ifndef BTM_AUTOMATIC_HCI_RESET
#define BTM_AUTOMATIC_HCI_RESET      FALSE
#endif

/* Include BTM Discovery database and code. */
#ifndef BTM_DISCOVERY_INCLUDED
#define BTM_DISCOVERY_INCLUDED      TRUE
#endif

/* Include inquiry code. */
#ifndef BTM_INQUIRY_INCLUDED
#define BTM_INQUIRY_INCLUDED        TRUE
#endif

/* Cancel Inquiry on incoming SSP */
#ifndef BTM_NO_SSP_ON_INQUIRY
#define BTM_NO_SSP_ON_INQUIRY       FALSE
#endif

/* Include periodic inquiry code (used when BTM_INQUIRY_INCLUDED is TRUE). */
#ifndef BTM_PERIODIC_INQ_INCLUDED
#define BTM_PERIODIC_INQ_INCLUDED   TRUE
#endif

/* Include security authorization code */
#ifndef BTM_AUTHORIZATION_INCLUDED
#define BTM_AUTHORIZATION_INCLUDED  TRUE
#endif

/* Includes SCO if TRUE */
#ifndef BTM_SCO_INCLUDED
#define BTM_SCO_INCLUDED            TRUE       /* TRUE includes SCO code */
#endif

/* Includes SCO if TRUE */
#ifndef BTM_SCO_HCI_INCLUDED
#define BTM_SCO_HCI_INCLUDED            FALSE       /* TRUE includes SCO over HCI code */
#endif

/* Includes WBS if TRUE */
#ifndef BTM_WBS_INCLUDED
#define BTM_WBS_INCLUDED            FALSE       /* TRUE includes WBS code */
#endif

/* Includes PCM2 support if TRUE */
#ifndef BTM_PCM2_INCLUDED
#define BTM_PCM2_INCLUDED           FALSE
#endif

/*  This is used to work around a controller bug that doesn't like Disconnect
**  issued while there is a role switch in progress
*/
#ifndef BTM_DISC_DURING_RS
#define BTM_DISC_DURING_RS TRUE
#endif

/**************************
** Initial SCO TX credit
*************************/
/* max TX SCO data packet size */
#ifndef BTM_SCO_DATA_SIZE_MAX
#define BTM_SCO_DATA_SIZE_MAX       240
#endif

/* maximum BTM buffering capacity */
#ifndef BTM_SCO_MAX_BUF_CAP
#define BTM_SCO_MAX_BUF_CAP     (BTM_SCO_INIT_XMIT_CREDIT * 4)
#endif

/* The size in bytes of the BTM inquiry database. */
#ifndef BTM_INQ_DB_SIZE
#define BTM_INQ_DB_SIZE             40
#endif

/* This is set to enable automatic periodic inquiry at startup. */
#ifndef BTM_ENABLE_AUTO_INQUIRY
#define BTM_ENABLE_AUTO_INQUIRY     FALSE
#endif

/* This is set to always try to acquire the remote device name. */
#ifndef BTM_INQ_GET_REMOTE_NAME
#define BTM_INQ_GET_REMOTE_NAME     FALSE
#endif

/* The inquiry duration in 1.28 second units when auto inquiry is enabled. */
#ifndef BTM_DEFAULT_INQ_DUR
#define BTM_DEFAULT_INQ_DUR         5
#endif

/* The inquiry mode when auto inquiry is enabled. */
#ifndef BTM_DEFAULT_INQ_MODE
#define BTM_DEFAULT_INQ_MODE        BTM_GENERAL_INQUIRY
#endif

/* The default periodic inquiry maximum delay when auto inquiry is enabled, in 1.28 second units. */
#ifndef BTM_DEFAULT_INQ_MAX_DELAY
#define BTM_DEFAULT_INQ_MAX_DELAY   30
#endif

/* The default periodic inquiry minimum delay when auto inquiry is enabled, in 1.28 second units. */
#ifndef BTM_DEFAULT_INQ_MIN_DELAY
#define BTM_DEFAULT_INQ_MIN_DELAY   20
#endif

/* The maximum age of entries in inquiry database in seconds ('0' disables feature). */
#ifndef BTM_INQ_MAX_AGE
#define BTM_INQ_MAX_AGE             0
#endif

/* The maximum age of entries in inquiry database based on inquiry response failure ('0' disables feature). */
#ifndef BTM_INQ_AGE_BY_COUNT
#define BTM_INQ_AGE_BY_COUNT        0
#endif

/* TRUE if controller does not support inquiry event filtering. */
#ifndef BTM_BYPASS_EVENT_FILTERING
#define BTM_BYPASS_EVENT_FILTERING  FALSE
#endif

/* TRUE if inquiry filtering is desired from BTM. */
#ifndef BTM_USE_INQ_RESULTS_FILTER
#define BTM_USE_INQ_RESULTS_FILTER  TRUE
#endif

/* The default scan mode */
#ifndef BTM_DEFAULT_SCAN_TYPE
#define BTM_DEFAULT_SCAN_TYPE       BTM_SCAN_TYPE_INTERLACED
#endif

/* Should connections to unknown devices be allowed when not discoverable? */
#ifndef BTM_ALLOW_CONN_IF_NONDISCOVER
#define BTM_ALLOW_CONN_IF_NONDISCOVER   TRUE
#endif

/* When connectable mode is set to TRUE, the device will respond to paging. */
#ifndef BTM_IS_CONNECTABLE
#define BTM_IS_CONNECTABLE          FALSE
#endif

/* Sets the Page_Scan_Window:  the length of time that the device is performing a page scan. */
#ifndef BTM_DEFAULT_CONN_WINDOW
#define BTM_DEFAULT_CONN_WINDOW     0x0012
#endif

/* Sets the Page_Scan_Activity:  the interval between the start of two consecutive page scans. */
#ifndef BTM_DEFAULT_CONN_INTERVAL
#define BTM_DEFAULT_CONN_INTERVAL   0x0800
#endif

/* This is set to automatically perform inquiry scan on startup. */
#ifndef BTM_IS_DISCOVERABLE
#define BTM_IS_DISCOVERABLE         FALSE
#endif

/* When automatic inquiry scan is enabled, this sets the discovery mode. */
#ifndef BTM_DEFAULT_DISC_MODE
#define BTM_DEFAULT_DISC_MODE       BTM_GENERAL_DISCOVERABLE
#endif

/* When automatic inquiry scan is enabled, this sets the inquiry scan window. */
#ifndef BTM_DEFAULT_DISC_WINDOW
#define BTM_DEFAULT_DISC_WINDOW     0x0012
#endif

/* When automatic inquiry scan is enabled, this sets the inquiry scan interval. */
#ifndef BTM_DEFAULT_DISC_INTERVAL
#define BTM_DEFAULT_DISC_INTERVAL   0x0800
#endif

/* Sets the period, in seconds, to automatically perform service discovery. */
#ifndef BTM_AUTO_DISCOVERY_PERIOD
#define BTM_AUTO_DISCOVERY_PERIOD   0
#endif

/* The size in bytes of the BTM discovery database (if discovery is included). */
#ifndef BTM_DISCOVERY_DB_SIZE
#define BTM_DISCOVERY_DB_SIZE       4000
#endif

/* Number of milliseconds to delay BTU task startup upon device initialization. */
#ifndef BTU_STARTUP_DELAY
#define BTU_STARTUP_DELAY           0
#endif

/* Whether BTA is included in BTU task. */
#ifndef BTU_BTA_INCLUDED
#define BTU_BTA_INCLUDED            TRUE
#endif

/* Number of seconds to wait to send an HCI Reset command upon device initialization. */
#ifndef BTM_FIRST_RESET_DELAY
#define BTM_FIRST_RESET_DELAY       0
#endif

/* The number of seconds to wait for controller module to reset after issuing an HCI Reset command. */
#ifndef BTM_AFTER_RESET_TIMEOUT
#define BTM_AFTER_RESET_TIMEOUT     0
#endif

/* Default class of device
* {SERVICE_CLASS, MAJOR_CLASS, MINOR_CLASS}
*
* SERVICE_CLASS:0x5A (Bit17 -Networking,Bit19 - Capturing,Bit20 -Object Transfer,Bit22 -Telephony)
* MAJOR_CLASS:0x02 - PHONE
* MINOR_CLASS:0x0C - SMART_PHONE
*
*/
#ifndef BTA_DM_COD
#define BTA_DM_COD {0x5A, 0x02, 0x0C}
#endif

/* The number of SCO links. */
#ifndef BTM_MAX_SCO_LINKS
#define BTM_MAX_SCO_LINKS           2
#endif

/* The preferred type of SCO links (2-eSCO, 0-SCO). */
#ifndef BTM_DEFAULT_SCO_MODE
#define BTM_DEFAULT_SCO_MODE        2
#endif

/* The number of security records for peer devices. */
#ifndef BTM_SEC_MAX_DEVICE_RECORDS
#define BTM_SEC_MAX_DEVICE_RECORDS  100
#endif

/* The number of security records for services. */
#ifndef BTM_SEC_MAX_SERVICE_RECORDS
#define BTM_SEC_MAX_SERVICE_RECORDS 32
#endif

/* If True, force a retrieval of remote device name for each bond in case it's changed */
#ifndef BTM_SEC_FORCE_RNR_FOR_DBOND
#define BTM_SEC_FORCE_RNR_FOR_DBOND  FALSE
#endif

/* Maximum device name length used in btm database. */
#ifndef BTM_MAX_REM_BD_NAME_LEN
#define BTM_MAX_REM_BD_NAME_LEN     248
#endif

/* Maximum local device name length stored btm database.
  '0' disables storage of the local name in BTM */
#ifndef BTM_MAX_LOC_BD_NAME_LEN
#define BTM_MAX_LOC_BD_NAME_LEN     248
#endif

/* TRUE if default string is used, FALSE if device name is set in the application */
#ifndef BTM_USE_DEF_LOCAL_NAME
#define BTM_USE_DEF_LOCAL_NAME      TRUE
#endif

/* Fixed Default String (Ignored if BTM_USE_DEF_LOCAL_NAME is FALSE) */
#ifndef BTM_DEF_LOCAL_NAME
#define BTM_DEF_LOCAL_NAME      ""
#endif

/* Maximum service name stored with security authorization (0 if not needed) */
#ifndef BTM_SEC_SERVICE_NAME_LEN
#define BTM_SEC_SERVICE_NAME_LEN    BT_MAX_SERVICE_NAME_LEN
#endif

/* Maximum number of pending security callback */
#ifndef BTM_SEC_MAX_CALLBACKS
#define BTM_SEC_MAX_CALLBACKS       7
#endif

/* Maximum length of the service name. */
#ifndef BT_MAX_SERVICE_NAME_LEN
#define BT_MAX_SERVICE_NAME_LEN     21
#endif

/* ACL buffer size in HCI Host Buffer Size command. */
#ifndef BTM_ACL_BUF_SIZE
#define BTM_ACL_BUF_SIZE            0
#endif

/* This is set to use the BTM power manager. */
#ifndef BTM_PWR_MGR_INCLUDED
#define BTM_PWR_MGR_INCLUDED        TRUE
#endif

/* The maximum number of clients that can register with the power manager. */
#ifndef BTM_MAX_PM_RECORDS
#define BTM_MAX_PM_RECORDS          2
#endif

/* This is set to show debug trace messages for the power manager. */
#ifndef BTM_PM_DEBUG
#define BTM_PM_DEBUG                FALSE
#endif

/* This is set to TRUE if link is to be unparked due to BTM_CreateSCO API. */
#ifndef BTM_SCO_WAKE_PARKED_LINK
#define BTM_SCO_WAKE_PARKED_LINK    TRUE
#endif

/* May be set to the the name of a function used for vendor specific chip initialization */
#ifndef BTM_APP_DEV_INIT
/* #define BTM_APP_DEV_INIT         myInitFunction() */
#endif

/* This is set to TRUE if the busy level change event is desired. (replace ACL change event) */
#ifndef BTM_BUSY_LEVEL_CHANGE_INCLUDED
#define BTM_BUSY_LEVEL_CHANGE_INCLUDED  TRUE
#endif

/* If the user does not respond to security process requests within this many seconds,
 * a negative response would be sent automatically.
 * It's recommended to use a value between 30 and OBX_TIMEOUT_VALUE
 * 30 is LMP response timeout value */
#ifndef BTM_SEC_TIMEOUT_VALUE
#define BTM_SEC_TIMEOUT_VALUE           35
#endif

/* Maximum number of callbacks that can be registered using BTM_RegisterForVSEvents */
#ifndef BTM_MAX_VSE_CALLBACKS
#define BTM_MAX_VSE_CALLBACKS           3
#endif

/* Number of streams for dual stack */
#ifndef BTM_SYNC_INFO_NUM_STR
#define BTM_SYNC_INFO_NUM_STR           2
#endif

/* Number of streams for dual stack in BT Controller */
#ifndef BTM_SYNC_INFO_NUM_STR_BTC
#define BTM_SYNC_INFO_NUM_STR_BTC       2
#endif

/******************************************
**    Lisbon Features
*******************************************/
/* This is set to TRUE if the server Extended Inquiry Response feature is desired. */
/* server sends EIR to client */
#ifndef BTM_EIR_SERVER_INCLUDED
#define BTM_EIR_SERVER_INCLUDED         TRUE
#endif

/* This is set to TRUE if the client Extended Inquiry Response feature is desired. */
/* client inquiry to server */
#ifndef BTM_EIR_CLIENT_INCLUDED
#define BTM_EIR_CLIENT_INCLUDED         TRUE
#endif

/* This is set to TRUE if the FEC is required for EIR packet. */
#ifndef BTM_EIR_DEFAULT_FEC_REQUIRED
#define BTM_EIR_DEFAULT_FEC_REQUIRED    TRUE
#endif

/* User defined UUID look up table */
#ifndef BTM_EIR_UUID_LKUP_TBL
#endif

/* The IO capability of the local device (for Simple Pairing) */
#ifndef BTM_LOCAL_IO_CAPS
#define BTM_LOCAL_IO_CAPS               BTM_IO_CAP_IO
#endif

/* The default MITM Protection Requirement (for Simple Pairing)
 * Possible values are BTM_AUTH_SP_YES or BTM_AUTH_SP_NO */
#ifndef BTM_DEFAULT_AUTH_REQ
#define BTM_DEFAULT_AUTH_REQ            BTM_AUTH_SP_NO
#endif

/* The default MITM Protection Requirement for dedicated bonding using Simple Pairing
 * Possible values are BTM_AUTH_AP_YES or BTM_AUTH_AP_NO */
#ifndef BTM_DEFAULT_DD_AUTH_REQ
#define BTM_DEFAULT_DD_AUTH_REQ            BTM_AUTH_AP_YES
#endif

/* Include Out-of-Band implementation for Simple Pairing */
#ifndef BTM_OOB_INCLUDED
#define BTM_OOB_INCLUDED                TRUE
#endif

/* TRUE to include Sniff Subrating */
#ifndef BTM_SSR_INCLUDED
#define BTM_SSR_INCLUDED                TRUE
#endif

/*************************
** End of Lisbon Features
**************************/

/* Used for conformance testing ONLY */
#ifndef BTM_BLE_CONFORMANCE_TESTING
#define BTM_BLE_CONFORMANCE_TESTING           FALSE
#endif


/******************************************************************************
**
** L2CAP
**
******************************************************************************/

/* Flow control and retransmission mode */

#ifndef L2CAP_FCR_INCLUDED
#define L2CAP_FCR_INCLUDED TRUE
#endif

/* The maximum number of simultaneous links that L2CAP can support. */
#ifndef MAX_ACL_CONNECTIONS
#define MAX_L2CAP_LINKS             7
#else
#define MAX_L2CAP_LINKS             MAX_ACL_CONNECTIONS
#endif

/* The maximum number of simultaneous channels that L2CAP can support. */
#ifndef MAX_L2CAP_CHANNELS
#define MAX_L2CAP_CHANNELS          10
#endif

/* The maximum number of simultaneous applications that can register with L2CAP. */
#ifndef MAX_L2CAP_CLIENTS
#define MAX_L2CAP_CLIENTS           15
#endif

/* The number of seconds of link inactivity before a link is disconnected. */
#ifndef L2CAP_LINK_INACTIVITY_TOUT
#define L2CAP_LINK_INACTIVITY_TOUT  4
#endif

/* The number of seconds of link inactivity after bonding before a link is disconnected. */
#ifndef L2CAP_BONDING_TIMEOUT
#define L2CAP_BONDING_TIMEOUT       3
#endif

/* The time from the HCI connection complete to disconnect if no channel is established. */
#ifndef L2CAP_LINK_STARTUP_TOUT
#define L2CAP_LINK_STARTUP_TOUT     60
#endif

/* The L2CAP MTU; must be in accord with the HCI ACL pool size. */
#ifndef L2CAP_MTU_SIZE
#define L2CAP_MTU_SIZE              1691
#endif

/* The L2CAP MPS over Bluetooth; must be in accord with the FCR tx pool size and ACL down buffer size. */
#ifndef L2CAP_MPS_OVER_BR_EDR
#define L2CAP_MPS_OVER_BR_EDR       1010
#endif

/* This is set to enable host flow control. */
#ifndef L2CAP_HOST_FLOW_CTRL
#define L2CAP_HOST_FLOW_CTRL        FALSE
#endif

/* If host flow control enabled, this is the number of buffers the controller can have unacknowledged. */
#ifndef L2CAP_HOST_FC_ACL_BUFS
#define L2CAP_HOST_FC_ACL_BUFS      20
#endif

/* The percentage of the queue size allowed before a congestion event is sent to the L2CAP client (typically 120%). */
#ifndef L2CAP_FWD_CONG_THRESH
#define L2CAP_FWD_CONG_THRESH       120
#endif

/* This is set to enable L2CAP to  take the ACL link out of park mode when ACL data is to be sent. */
#ifndef L2CAP_WAKE_PARKED_LINK
#define L2CAP_WAKE_PARKED_LINK      TRUE
#endif

/* Whether link wants to be the master or the slave. */
#ifndef L2CAP_DESIRED_LINK_ROLE
#define L2CAP_DESIRED_LINK_ROLE     HCI_ROLE_SLAVE
#endif

/* Include Non-Flushable Packet Boundary Flag feature of Lisbon */
#ifndef L2CAP_NON_FLUSHABLE_PB_INCLUDED
#define L2CAP_NON_FLUSHABLE_PB_INCLUDED     TRUE
#endif

/* Minimum number of ACL credit for high priority link */
#ifndef L2CAP_HIGH_PRI_MIN_XMIT_QUOTA
#define L2CAP_HIGH_PRI_MIN_XMIT_QUOTA       4
#endif

/* used for monitoring HCI ACL credit management */
#ifndef L2CAP_HCI_FLOW_CONTROL_DEBUG
#define L2CAP_HCI_FLOW_CONTROL_DEBUG        TRUE
#endif

/* Used for calculating transmit buffers off of */
#ifndef L2CAP_NUM_XMIT_BUFFS
#define L2CAP_NUM_XMIT_BUFFS                HCI_ACL_BUF_MAX
#endif

/* Unicast Connectionless Data */
#ifndef L2CAP_UCD_INCLUDED
#define L2CAP_UCD_INCLUDED                  FALSE
#endif

/* Unicast Connectionless Data MTU */
#ifndef L2CAP_UCD_MTU
#define L2CAP_UCD_MTU                       L2CAP_MTU_SIZE
#endif

/* Unicast Connectionless Data Idle Timeout */
#ifndef L2CAP_UCD_IDLE_TIMEOUT
#define L2CAP_UCD_IDLE_TIMEOUT              2
#endif

/* Unicast Connectionless Data Idle Timeout */
#ifndef L2CAP_UCD_CH_PRIORITY
#define L2CAP_UCD_CH_PRIORITY               L2CAP_CHNL_PRIORITY_MEDIUM
#endif

/* Max clients on Unicast Connectionless Data */
#ifndef L2CAP_MAX_UCD_CLIENTS
#define L2CAP_MAX_UCD_CLIENTS               5
#endif

/* Used for features using fixed channels; set to zero if no fixed channels supported (BLE, etc.) */
/* Excluding L2CAP signaling channel and UCD */
#ifndef L2CAP_NUM_FIXED_CHNLS
#define L2CAP_NUM_FIXED_CHNLS               4
#endif

/* First fixed channel supported */
#ifndef L2CAP_FIRST_FIXED_CHNL
#define L2CAP_FIRST_FIXED_CHNL              3
#endif

#ifndef L2CAP_LAST_FIXED_CHNL
#define L2CAP_LAST_FIXED_CHNL           (L2CAP_FIRST_FIXED_CHNL + L2CAP_NUM_FIXED_CHNLS - 1)
#endif

/* Round Robin service channels in link */
#ifndef L2CAP_ROUND_ROBIN_CHANNEL_SERVICE
#define L2CAP_ROUND_ROBIN_CHANNEL_SERVICE   TRUE
#endif

/* Used for calculating transmit buffers off of */
#ifndef L2CAP_NUM_XMIT_BUFFS
#define L2CAP_NUM_XMIT_BUFFS                HCI_ACL_BUF_MAX
#endif

/* Used for features using fixed channels; set to zero if no fixed channels supported (BLE, etc.) */
#ifndef L2CAP_NUM_FIXED_CHNLS
#define L2CAP_NUM_FIXED_CHNLS               1
#endif

/* First fixed channel supported */
#ifndef L2CAP_FIRST_FIXED_CHNL
#define L2CAP_FIRST_FIXED_CHNL              3
#endif

#ifndef L2CAP_LAST_FIXED_CHNL
#define L2CAP_LAST_FIXED_CHNL           (L2CAP_FIRST_FIXED_CHNL + L2CAP_NUM_FIXED_CHNLS - 1)
#endif

/* used for monitoring eL2CAP data flow */
#ifndef L2CAP_ERTM_STATS
#define L2CAP_ERTM_STATS                    FALSE
#endif

/* USED FOR FCR TEST ONLY:  When TRUE generates bad tx and rx packets */
#ifndef L2CAP_CORRUPT_ERTM_PKTS
#define L2CAP_CORRUPT_ERTM_PKTS             FALSE
#endif

/* Used for conformance testing ONLY:  When TRUE lets scriptwrapper overwrite info response */
#ifndef L2CAP_CONFORMANCE_TESTING
#define L2CAP_CONFORMANCE_TESTING           FALSE
#endif


#ifndef TIMER_PARAM_TYPE
#ifdef  WIN2000
#define TIMER_PARAM_TYPE    void *
#else
#define TIMER_PARAM_TYPE    UINT32
#endif
#endif

/******************************************************************************
**
** BLE
**
******************************************************************************/

#ifndef BLE_INCLUDED
#define BLE_INCLUDED            FALSE
#endif

#ifndef LOCAL_BLE_CONTROLLER_ID
#define LOCAL_BLE_CONTROLLER_ID         (1)
#endif

/******************************************************************************
**
** ATT/GATT Protocol/Profile Settings
**
******************************************************************************/
#ifndef ATT_INCLUDED
#define ATT_INCLUDED         FALSE
#endif

#ifndef ATT_DEBUG
#define ATT_DEBUG           FALSE
#endif

#ifndef GATT_SERVER_ENABLED
#define GATT_SERVER_ENABLED          FALSE
#endif

#ifndef GATT_CLIENT_ENABLED
#define GATT_CLIENT_ENABLED          FALSE
#endif

#ifndef GATT_MAX_SR_PROFILES
#define GATT_MAX_SR_PROFILES        32 /* max is 32 */
#endif

#ifndef GATT_MAX_APPS
#define GATT_MAX_APPS            10 /* note: 2 apps used internally GATT and GAP */
#endif

#ifndef GATT_MAX_CL_PROFILES
#define GATT_MAX_CL_PROFILES        4
#endif

#ifndef GATT_MAX_PHY_CHANNEL
#define GATT_MAX_PHY_CHANNEL        4
#endif

/* Used for conformance testing ONLY */
#ifndef GATT_CONFORMANCE_TESTING
#define GATT_CONFORMANCE_TESTING           FALSE
#endif

/* number of background connection device allowence, ideally to be the same as WL size
*/
#ifndef GATT_MAX_BG_CONN_DEV
#define GATT_MAX_BG_CONN_DEV        32
#endif

/******************************************************************************
**
** SMP
**
******************************************************************************/
#ifndef SMP_INCLUDED
#define SMP_INCLUDED         FALSE
#endif

#ifndef SMP_DEBUG
#define SMP_DEBUG            FALSE
#endif

#ifndef SMP_DEFAULT_AUTH_REQ
#define SMP_DEFAULT_AUTH_REQ    SMP_AUTH_NB_ENC_ONLY
#endif

#ifndef SMP_MAX_ENC_KEY_SIZE
#define SMP_MAX_ENC_KEY_SIZE    16
#endif

#ifndef SMP_MIN_ENC_KEY_SIZE
#define SMP_MIN_ENC_KEY_SIZE    7
#endif

/* Used for conformance testing ONLY */
#ifndef SMP_CONFORMANCE_TESTING
#define SMP_CONFORMANCE_TESTING           FALSE
#endif

/******************************************************************************
**
** SDP
**
******************************************************************************/

/* This is set to enable SDP server functionality. */
#ifndef SDP_SERVER_ENABLED
#define SDP_SERVER_ENABLED          TRUE
#endif

/* The maximum number of SDP records the server can support. */
#ifndef SDP_MAX_RECORDS
#define SDP_MAX_RECORDS             20
#endif

/* The maximum number of attributes in each record. */
#ifndef SDP_MAX_REC_ATTR
//#if defined(HID_DEV_INCLUDED) && (HID_DEV_INCLUDED==TRUE)
#define SDP_MAX_REC_ATTR            25
//#else
//#define SDP_MAX_REC_ATTR            13
//#endif
#endif

#ifndef SDP_MAX_PAD_LEN
#define SDP_MAX_PAD_LEN             600
#endif

/* The maximum length, in bytes, of an attribute. */
#ifndef SDP_MAX_ATTR_LEN
//#if defined(HID_DEV_INCLUDED) && (HID_DEV_INCLUDED==TRUE)
//#define SDP_MAX_ATTR_LEN            80
//#else
//#define SDP_MAX_ATTR_LEN            100
//#endif
#define SDP_MAX_ATTR_LEN            400
#endif

/* The maximum number of attribute filters supported by SDP databases. */
#ifndef SDP_MAX_ATTR_FILTERS
#define SDP_MAX_ATTR_FILTERS        15
#endif

/* The maximum number of UUID filters supported by SDP databases. */
#ifndef SDP_MAX_UUID_FILTERS
#define SDP_MAX_UUID_FILTERS        3
#endif

/* This is set to enable SDP client functionality. */
#ifndef SDP_CLIENT_ENABLED
#define SDP_CLIENT_ENABLED          TRUE
#endif

/* The maximum number of record handles retrieved in a search. */
#ifndef SDP_MAX_DISC_SERVER_RECS
#define SDP_MAX_DISC_SERVER_RECS    21
#endif

/* The size of a scratchpad buffer, in bytes, for storing the response to an attribute request. */
#ifndef SDP_MAX_LIST_BYTE_COUNT
#define SDP_MAX_LIST_BYTE_COUNT     4096
#endif

/* The maximum number of parameters in an SDP protocol element. */
#ifndef SDP_MAX_PROTOCOL_PARAMS
#define SDP_MAX_PROTOCOL_PARAMS     2
#endif

/* The maximum number of simultaneous client and server connections. */
#ifndef SDP_MAX_CONNECTIONS
#define SDP_MAX_CONNECTIONS         4
#endif

/* The MTU size for the L2CAP configuration. */
#ifndef SDP_MTU_SIZE
#define SDP_MTU_SIZE                256
#endif

/* The flush timeout for the L2CAP configuration. */
#ifndef SDP_FLUSH_TO
#define SDP_FLUSH_TO                0xFFFF
#endif

/* The name for security authorization. */
#ifndef SDP_SERVICE_NAME
#define SDP_SERVICE_NAME            "Service Discovery"
#endif

/* The security level for BTM. */
#ifndef SDP_SECURITY_LEVEL
#define SDP_SECURITY_LEVEL          BTM_SEC_NONE
#endif

/* Device identification feature. */
#ifndef SDP_DI_INCLUDED
#define SDP_DI_INCLUDED             TRUE
#endif

/******************************************************************************
**
** RFCOMM
**
******************************************************************************/

#ifndef RFCOMM_INCLUDED
#define RFCOMM_INCLUDED             TRUE
#endif

/* The maximum number of ports supported. */
#ifndef MAX_RFC_PORTS
#define MAX_RFC_PORTS               30
#endif

/* The maximum simultaneous links to different devices. */
#ifndef MAX_ACL_CONNECTIONS
#define MAX_BD_CONNECTIONS          7
#else
#define MAX_BD_CONNECTIONS          MAX_ACL_CONNECTIONS
#endif

/* The port receive queue low watermark level, in bytes. */
#ifndef PORT_RX_LOW_WM
#define PORT_RX_LOW_WM              (BTA_RFC_MTU_SIZE * PORT_RX_BUF_LOW_WM)
#endif

/* The port receive queue high watermark level, in bytes. */
#ifndef PORT_RX_HIGH_WM
#define PORT_RX_HIGH_WM             (BTA_RFC_MTU_SIZE * PORT_RX_BUF_HIGH_WM)
#endif

/* The port receive queue critical watermark level, in bytes. */
#ifndef PORT_RX_CRITICAL_WM
#define PORT_RX_CRITICAL_WM         (BTA_RFC_MTU_SIZE * PORT_RX_BUF_CRITICAL_WM)
#endif

/* The port receive queue low watermark level, in number of buffers. */
#ifndef PORT_RX_BUF_LOW_WM
#define PORT_RX_BUF_LOW_WM          4
#endif

/* The port receive queue high watermark level, in number of buffers. */
#ifndef PORT_RX_BUF_HIGH_WM
#define PORT_RX_BUF_HIGH_WM         10
#endif

/* The port receive queue critical watermark level, in number of buffers. */
#ifndef PORT_RX_BUF_CRITICAL_WM
#define PORT_RX_BUF_CRITICAL_WM     15
#endif

/* The port transmit queue high watermark level, in bytes. */
#ifndef PORT_TX_HIGH_WM
#define PORT_TX_HIGH_WM             (BTA_RFC_MTU_SIZE * PORT_TX_BUF_HIGH_WM)
#endif

/* The port transmit queue critical watermark level, in bytes. */
#ifndef PORT_TX_CRITICAL_WM
#define PORT_TX_CRITICAL_WM         (BTA_RFC_MTU_SIZE * PORT_TX_BUF_CRITICAL_WM)
#endif

/* The port transmit queue high watermark level, in number of buffers. */
#ifndef PORT_TX_BUF_HIGH_WM
#define PORT_TX_BUF_HIGH_WM         10
#endif

/* The port transmit queue high watermark level, in number of buffers. */
#ifndef PORT_TX_BUF_CRITICAL_WM
#define PORT_TX_BUF_CRITICAL_WM     15
#endif

/* The RFCOMM multiplexer preferred flow control mechanism. */
#ifndef PORT_FC_DEFAULT
#define PORT_FC_DEFAULT             PORT_FC_CREDIT
#endif

/* The maximum number of credits receiver sends to peer when using credit-based flow control. */
#ifndef PORT_CREDIT_RX_MAX
#define PORT_CREDIT_RX_MAX          16
#endif

/* The credit low watermark level. */
#ifndef PORT_CREDIT_RX_LOW
#define PORT_CREDIT_RX_LOW          8
#endif

/* Test code allowing l2cap FEC on RFCOMM.*/
#ifndef PORT_ENABLE_L2CAP_FCR_TEST
#define PORT_ENABLE_L2CAP_FCR_TEST  FALSE
#endif

/* if application like BTA, Java or script test engine is running on other than BTU thread, */
/* PORT_SCHEDULE_LOCK shall be defined as GKI_sched_lock() or GKI_disable() */
#ifndef PORT_SCHEDULE_LOCK
#define PORT_SCHEDULE_LOCK          GKI_disable()
#endif

/* if application like BTA, Java or script test engine is running on other than BTU thread, */
/* PORT_SCHEDULE_LOCK shall be defined as GKI_sched_unlock() or GKI_enable() */
#ifndef PORT_SCHEDULE_UNLOCK
#define PORT_SCHEDULE_UNLOCK        GKI_enable()
#endif

/******************************************************************************
**
** TCS
**
******************************************************************************/

#ifndef TCS_INCLUDED
#define TCS_INCLUDED                FALSE
#endif

/* If set to TRUE, gives lean TCS state machine configuration. */
#ifndef TCS_LEAN
#define TCS_LEAN                    FALSE
#endif

/* To include/exclude point-to-multipoint broadcast SETUP configuration. */
#ifndef TCS_BCST_SETUP_INCLUDED
#define TCS_BCST_SETUP_INCLUDED     TRUE
#endif

/* To include/exclude supplementary services. */
#ifndef TCS_SUPP_SVCS_INCLUDED
#define TCS_SUPP_SVCS_INCLUDED      TRUE
#endif

/* To include/exclude WUG master role. */
#ifndef TCS_WUG_MASTER_INCLUDED
#define TCS_WUG_MASTER_INCLUDED     TRUE
#endif

/* To include/exclude WUG member role. */
#ifndef TCS_WUG_MEMBER_INCLUDED
#define TCS_WUG_MEMBER_INCLUDED     TRUE
#endif

/* Maximum number of WUG members. */
#ifndef TCS_MAX_WUG_MEMBERS
#define TCS_MAX_WUG_MEMBERS         7
#endif

/* Broadcom specific acknowledgement message to ensure fast and robust operation of WUG FIMA procedure. */
#ifndef TCS_WUG_LISTEN_ACPT_ACK_INCLUDED
#define TCS_WUG_LISTEN_ACPT_ACK_INCLUDED TRUE
#endif

/* The number of simultaneous calls supported. */
#ifndef TCS_MAX_NUM_SIMUL_CALLS
#define TCS_MAX_NUM_SIMUL_CALLS     3
#endif

/* The number of devices the device can connect to. */
#ifndef TCS_MAX_NUM_ACL_CONNS
#define TCS_MAX_NUM_ACL_CONNS       7
#endif

/* The maximum length, in bytes, of the company specific information element. */
#ifndef TCS_MAX_CO_SPEC_LEN
#define TCS_MAX_CO_SPEC_LEN         40
#endif

/* The maximum length, in bytes, of the audio control information element . */
#ifndef TCS_MAX_AUDIO_CTL_LEN
#define TCS_MAX_AUDIO_CTL_LEN       40
#endif

/* (Dis)allow EDR ESCO */
#ifndef TCS_AUDIO_USE_ESCO_EDR
#define TCS_AUDIO_USE_ESCO_EDR      FALSE
#endif

/******************************************************************************
**
** OBX
**
******************************************************************************/
#ifndef OBX_INCLUDED
#define OBX_INCLUDED               FALSE
#endif

#ifndef OBX_CLIENT_INCLUDED
#define OBX_CLIENT_INCLUDED        TRUE
#endif

#ifndef OBX_SERVER_INCLUDED
#define OBX_SERVER_INCLUDED        TRUE
#endif

/* TRUE to include OBEX authentication/MD5 code */
#ifndef OBX_MD5_INCLUDED
#define OBX_MD5_INCLUDED           TRUE
#endif

/* TRUE to include OBEX authentication/MD5 test code */
#ifndef OBX_MD5_TEST_INCLUDED
#define OBX_MD5_TEST_INCLUDED       FALSE
#endif

/* TRUE to include OBEX 1.4 enhancement (including Obex Over L2CAP) */
#ifndef OBX_14_INCLUDED
#define OBX_14_INCLUDED             FALSE
#endif
/* MD5 code is required to use OBEX 1.4 features (Reliable session) */
#if (OBX_MD5_INCLUDED == FALSE)
#undef OBX_14_INCLUDED
#define OBX_14_INCLUDED             FALSE
#endif

/* L2CAP FCR/eRTM mode is required to use OBEX Over L2CAP */
#if (L2CAP_FCR_INCLUDED == FALSE)
#undef OBX_14_INCLUDED
#define OBX_14_INCLUDED             FALSE
#endif

/* The timeout value (in seconds) for reliable sessions to remain in suspend. 0xFFFFFFFF for no timeout event. */
#ifndef OBX_SESS_TIMEOUT_VALUE
#define OBX_SESS_TIMEOUT_VALUE      600
#endif

/* The idle timeout value. 0 for no timeout event. */
#ifndef OBX_TIMEOUT_VALUE
#define OBX_TIMEOUT_VALUE           60
#endif

/* Timeout value used for disconnect */
#ifndef OBX_DISC_TOUT_VALUE
#define OBX_DISC_TOUT_VALUE         5
#endif

/* The maximum number of registered servers. */
#ifndef OBX_NUM_SERVERS
#define OBX_NUM_SERVERS             12
#endif

/* The maximum number of sessions per registered server. */
#ifndef OBX_MAX_SR_SESSION
#define OBX_MAX_SR_SESSION          4
#endif

/* The maximum number of sessions for all registered servers.
 * (must be equal or bigger than OBX_NUM_SERVERS) */
#ifndef OBX_NUM_SR_SESSIONS
#define OBX_NUM_SR_SESSIONS         26
#endif

/* The maximum number of sessions per registered server.
 * must be less than MAX_BD_CONNECTIONS */
#ifndef OBX_MAX_SR_SESSION
#define OBX_MAX_SR_SESSION          4
#endif

/* The maximum number of suspended sessions per registered servers. */
#ifndef OBX_MAX_SUSPEND_SESSIONS
#define OBX_MAX_SUSPEND_SESSIONS    4
#endif

/* The maximum number of active clients. */
#ifndef OBX_NUM_CLIENTS
#define OBX_NUM_CLIENTS             8
#endif

/* The maximum length of OBEX target header.*/
#ifndef OBX_MAX_TARGET_LEN
#define OBX_MAX_TARGET_LEN          16
#endif

/* The maximum length of authentication challenge realm.*/
#ifndef OBX_MAX_REALM_LEN
#define OBX_MAX_REALM_LEN           30
#endif

/* The maximum of GKI buffer queued at OBX before flow control L2CAP */
#ifndef OBX_MAX_RX_QUEUE_COUNT
#define OBX_MAX_RX_QUEUE_COUNT      3
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
   Pool ID where to reassemble the SDU.
   This Pool will allow buffers to be used that are larger than
   the L2CAP_MAX_MTU. */
#ifndef OBX_USER_RX_POOL_ID
#define OBX_USER_RX_POOL_ID     OBX_LRG_DATA_POOL_ID
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
   Pool ID where to hold the SDU.
   This Pool will allow buffers to be used that are larger than
   the L2CAP_MAX_MTU. */
#ifndef OBX_USER_TX_POOL_ID
#define OBX_USER_TX_POOL_ID     OBX_LRG_DATA_POOL_ID
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
GKI Buffer Pool ID used to hold MPS segments during SDU reassembly
*/
#ifndef OBX_FCR_RX_POOL_ID
#define OBX_FCR_RX_POOL_ID      HCI_ACL_POOL_ID
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
GKI Buffer Pool ID used to hold MPS segments used in (re)transmissions.
L2CAP_DEFAULT_ERM_POOL_ID is specified to use the HCI ACL data pool.
Note:  This pool needs to have enough buffers to hold two times the window size negotiated
 in the L2CA_SetFCROptions (2 * tx_win_size)  to allow for retransmissions.
 The size of each buffer must be able to hold the maximum MPS segment size passed in
 L2CA_SetFCROptions plus BT_HDR (8) + HCI preamble (4) + L2CAP_MIN_OFFSET (11 - as of BT 2.1 + EDR Spec).
*/
#ifndef OBX_FCR_TX_POOL_ID
#define OBX_FCR_TX_POOL_ID      HCI_ACL_POOL_ID
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
Size of the transmission window when using enhanced retransmission mode. Not used
in basic and streaming modes. Range: 1 - 63
*/
#ifndef OBX_FCR_OPT_TX_WINDOW_SIZE_BR_EDR
#define OBX_FCR_OPT_TX_WINDOW_SIZE_BR_EDR       20
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
Number of transmission attempts for a single I-Frame before taking
Down the connection. Used In ERTM mode only. Value is Ignored in basic and
Streaming modes.
Range: 0, 1-0xFF
0 - infinite retransmissions
1 - single transmission
*/
#ifndef OBX_FCR_OPT_MAX_TX_B4_DISCNT
#define OBX_FCR_OPT_MAX_TX_B4_DISCNT    20
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
Retransmission Timeout
Range: Minimum 2000 (2 secs) on BR/EDR when supporting PBF.
 */
#ifndef OBX_FCR_OPT_RETX_TOUT
#define OBX_FCR_OPT_RETX_TOUT           2000
#endif

/* This option is application when OBX_14_INCLUDED=TRUE
Monitor Timeout
Range: Minimum 12000 (12 secs) on BR/EDR when supporting PBF.
*/
#ifndef OBX_FCR_OPT_MONITOR_TOUT
#define OBX_FCR_OPT_MONITOR_TOUT        12000
#endif

/******************************************************************************
**
** BNEP
**
******************************************************************************/

#ifndef BNEP_INCLUDED
#define BNEP_INCLUDED               TRUE
#endif

/* Protocol filtering is an optional feature. Bydefault it will be turned on */
#ifndef BNEP_SUPPORTS_PROT_FILTERS
#define BNEP_SUPPORTS_PROT_FILTERS          TRUE
#endif

/* Multicast filtering is an optional feature. Bydefault it will be turned on */
#ifndef BNEP_SUPPORTS_MULTI_FILTERS
#define BNEP_SUPPORTS_MULTI_FILTERS         TRUE
#endif

/* BNEP status API call is used mainly to get the L2CAP handle */
#ifndef BNEP_SUPPORTS_STATUS_API
#define BNEP_SUPPORTS_STATUS_API            TRUE
#endif

/* This is just a debug function */
#ifndef BNEP_SUPPORTS_DEBUG_DUMP
#define BNEP_SUPPORTS_DEBUG_DUMP            TRUE
#endif

#ifndef BNEP_SUPPORTS_ALL_UUID_LENGTHS
#define BNEP_SUPPORTS_ALL_UUID_LENGTHS      TRUE    /* Otherwise it will support only 16bit UUIDs */
#endif

/*
** When BNEP connection changes roles after the connection is established
** we will do an authentication check again on the new role
*/
#ifndef BNEP_DO_AUTH_FOR_ROLE_SWITCH
#define BNEP_DO_AUTH_FOR_ROLE_SWITCH        TRUE
#endif


/* Maximum number of protocol filters supported. */
#ifndef BNEP_MAX_PROT_FILTERS
#define BNEP_MAX_PROT_FILTERS       5
#endif

/* Maximum number of multicast filters supported. */
#ifndef BNEP_MAX_MULTI_FILTERS
#define BNEP_MAX_MULTI_FILTERS      5
#endif

/* Minimum MTU size. */
#ifndef BNEP_MIN_MTU_SIZE
#define BNEP_MIN_MTU_SIZE           L2CAP_MTU_SIZE
#endif

/* Preferred MTU size. */
#ifndef BNEP_MTU_SIZE
#define BNEP_MTU_SIZE               BNEP_MIN_MTU_SIZE
#endif

/* Maximum size of user data, in bytes.  */
#ifndef BNEP_MAX_USER_DATA_SIZE
#define BNEP_MAX_USER_DATA_SIZE     1500
#endif

/* Maximum number of buffers allowed in transmit data queue. */
#ifndef BNEP_MAX_XMITQ_DEPTH
#define BNEP_MAX_XMITQ_DEPTH        20
#endif

/* Maximum number BNEP of connections supported. */
#ifndef BNEP_MAX_CONNECTIONS
#define BNEP_MAX_CONNECTIONS        7
#endif


/******************************************************************************
**
** AVDTP
**
******************************************************************************/

#ifndef AVDT_INCLUDED
#define AVDT_INCLUDED               TRUE
#endif

/* Include reporting capability in AVDTP */
#ifndef AVDT_REPORTING
#define AVDT_REPORTING              TRUE
#endif

/* Include multiplexing capability in AVDTP */
#ifndef AVDT_MULTIPLEXING
#define AVDT_MULTIPLEXING           TRUE
#endif

/* Number of simultaneous links to different peer devices. */
#ifndef AVDT_NUM_LINKS
#define AVDT_NUM_LINKS              2
#endif

/* Number of simultaneous stream endpoints. */
#ifndef AVDT_NUM_SEPS
#define AVDT_NUM_SEPS               3
#endif

/* Number of transport channels setup per media stream(audio or video) */
#ifndef AVDT_NUM_CHANNELS

#if AVDT_REPORTING == TRUE
/* signaling, media and reporting channels */
#define AVDT_NUM_CHANNELS   3
#else
/* signaling and media channels */
#define AVDT_NUM_CHANNELS   2
#endif

#endif

/* Number of transport channels setup by AVDT for all media streams
 * AVDT_NUM_CHANNELS * Number of simultaneous streams.
 */
#ifndef AVDT_NUM_TC_TBL
#define AVDT_NUM_TC_TBL             6
#endif


/* Maximum size in bytes of the codec capabilities information element. */
#ifndef AVDT_CODEC_SIZE
#define AVDT_CODEC_SIZE             10
#endif

/* Maximum size in bytes of the content protection information element. */
#ifndef AVDT_PROTECT_SIZE
#define AVDT_PROTECT_SIZE           90
#endif

/* Maximum number of GKI buffers in the fragment queue (for video frames).
 * Must be less than the number of buffers in the buffer pool of size AVDT_DATA_POOL_SIZE */
#ifndef AVDT_MAX_FRAG_COUNT
#define AVDT_MAX_FRAG_COUNT         15
#endif

/******************************************************************************
**
** PAN
**
******************************************************************************/

#ifndef PAN_INCLUDED
#define PAN_INCLUDED                     TRUE
#endif

/* This will enable the PANU role */
#ifndef PAN_SUPPORTS_ROLE_PANU
#define PAN_SUPPORTS_ROLE_PANU              TRUE
#endif

/* This will enable the GN role */
#ifndef PAN_SUPPORTS_ROLE_GN
#define PAN_SUPPORTS_ROLE_GN                TRUE
#endif

/* This will enable the NAP role */
#ifndef PAN_SUPPORTS_ROLE_NAP
#define PAN_SUPPORTS_ROLE_NAP               TRUE
#endif

/* This is just for debugging purposes */
#ifndef PAN_SUPPORTS_DEBUG_DUMP
#define PAN_SUPPORTS_DEBUG_DUMP             TRUE
#endif


/* Maximum number of PAN connections allowed */
#ifndef MAX_PAN_CONNS
#define MAX_PAN_CONNS                    7
#endif

/* Default service name for NAP role */
#ifndef PAN_NAP_DEFAULT_SERVICE_NAME
#define PAN_NAP_DEFAULT_SERVICE_NAME    "Network Access Point Service"
#endif

/* Default service name for GN role */
#ifndef PAN_GN_DEFAULT_SERVICE_NAME
#define PAN_GN_DEFAULT_SERVICE_NAME     "Group Network Service"
#endif

/* Default service name for PANU role */
#ifndef PAN_PANU_DEFAULT_SERVICE_NAME
#define PAN_PANU_DEFAULT_SERVICE_NAME   "PAN User Service"
#endif

/* Default description for NAP role service */
#ifndef PAN_NAP_DEFAULT_DESCRIPTION
#define PAN_NAP_DEFAULT_DESCRIPTION     "NAP"
#endif

/* Default description for GN role service */
#ifndef PAN_GN_DEFAULT_DESCRIPTION
#define PAN_GN_DEFAULT_DESCRIPTION      "GN"
#endif

/* Default description for PANU role service */
#ifndef PAN_PANU_DEFAULT_DESCRIPTION
#define PAN_PANU_DEFAULT_DESCRIPTION    "PANU"
#endif

/* Default Security level for PANU role. */
#ifndef PAN_PANU_SECURITY_LEVEL
#define PAN_PANU_SECURITY_LEVEL          0
#endif

/* Default Security level for GN role. */
#ifndef PAN_GN_SECURITY_LEVEL
#define PAN_GN_SECURITY_LEVEL            0
#endif

/* Default Security level for NAP role. */
#ifndef PAN_NAP_SECURITY_LEVEL
#define PAN_NAP_SECURITY_LEVEL           0
#endif




/******************************************************************************
**
** GAP
**
******************************************************************************/

#ifndef GAP_INCLUDED
#define GAP_INCLUDED                FALSE
#endif

/* This is set to enable use of GAP L2CAP connections. */
#ifndef GAP_CONN_INCLUDED
#define GAP_CONN_INCLUDED           TRUE
#endif

/* This is set to enable posting event for data write */
#ifndef GAP_CONN_POST_EVT_INCLUDED
#define GAP_CONN_POST_EVT_INCLUDED  FALSE
#endif

/* The maximum number of simultaneous GAP L2CAP connections. */
#ifndef GAP_MAX_CONNECTIONS
#define GAP_MAX_CONNECTIONS         8
#endif

/******************************************************************************
**
** CTP
**
******************************************************************************/

#ifndef CTP_INCLUDED
#define CTP_INCLUDED                FALSE
#endif

/* To include CTP gateway functionality or not. */
#ifndef CTP_GW_INCLUDED
#define CTP_GW_INCLUDED             TRUE
#endif

/* The number of terminals supported. */
#ifndef CTP_MAX_NUM_TLS
#define CTP_MAX_NUM_TLS             7
#endif

/* If the controller can not support sniff mode when the SCO is up, set this to FALSE. */
#ifndef CTP_USE_SNIFF_ON_SCO
#define CTP_USE_SNIFF_ON_SCO        FALSE
#endif

/* When ACL link between TL and GW is idle for more than this amount of seconds, the ACL may be put to low power mode. */
#ifndef CTP_TL_IDLE_TIMEOUT
#define CTP_TL_IDLE_TIMEOUT         90
#endif

/* To include CTP terminal functionality or not. */
#ifndef CTP_TL_INCLUDED
#define CTP_TL_INCLUDED             TRUE
#endif

/* To include CTP device discovery functionality or not. */
#ifndef CTP_DISCOVERY_INCLUDED
#define CTP_DISCOVERY_INCLUDED      TRUE
#endif

/* set to TRUE for controllers that do not support multi-point */
#ifndef CTP_TL_WAIT_DISC
#define CTP_TL_WAIT_DISC            TRUE
#endif

/* The CTP inquiry database size. */
#ifndef CTP_INQ_DB_SIZE
#define CTP_INQ_DB_SIZE             CTP_DISC_REC_SIZE
#endif

/* The CTP discovery record size. */
#ifndef CTP_DISC_REC_SIZE
#define CTP_DISC_REC_SIZE           60
#endif

/* CTP TL would try to re-establish L2CAP channel after channel is disconnected for this amount of seconds. */
#ifndef CTP_GUARD_LINK_LOST
#define CTP_GUARD_LINK_LOST         1
#endif

/* The link policy bitmap. */
#ifndef CTP_DEFAULT_LINK_POLICY
#define CTP_DEFAULT_LINK_POLICY     0x000F
#endif

/* The minimum period interval used for the sniff and park modes. */
#ifndef CTP_DEF_LOWPWR_MIN_PERIOD
#define CTP_DEF_LOWPWR_MIN_PERIOD   0x100
#endif

/* The maximum period interval used for the sniff and park modes. */
#ifndef CTP_DEF_LOWPWR_MAX_PERIOD
#define CTP_DEF_LOWPWR_MAX_PERIOD   0x1E0
#endif

/* The number of baseband receive slot sniff attempts. */
#ifndef CTP_DEF_LOWPWR_ATTEMPT
#define CTP_DEF_LOWPWR_ATTEMPT      0x200
#endif

/* The number of baseband receive slots for sniff timeout. */
#ifndef CTP_DEF_LOWPWR_TIMEOUT
#define CTP_DEF_LOWPWR_TIMEOUT      0x200
#endif

/* This is set if CTP is to use park mode. */
#ifndef CTP_PARK_INCLUDED
#define CTP_PARK_INCLUDED           TRUE
#endif

/* This is set if CTP is to use sniff mode. */
#ifndef CTP_SNIFF_INCLUDED
#define CTP_SNIFF_INCLUDED          TRUE
#endif

/* To include CTP data exchange functionality or not. */
#ifndef CTP_DATA_EXCHG_FEATURE
#define CTP_DATA_EXCHG_FEATURE      FALSE
#endif

/* To include CTP GW intercom functionality or not. */
#ifndef CTP_GW_INTERCOM_FEATURE
#define CTP_GW_INTERCOM_FEATURE     FALSE
#endif

/* The MTU size for L2CAP channel. */
#ifndef CTP_MTU_SIZE
#define CTP_MTU_SIZE                200
#endif

/* The L2CAP PSM for the data exchange feature. */
#ifndef CTP_DATA_EXCHG_PSM
#define CTP_DATA_EXCHG_PSM          13
#endif

/* The flush timeout for L2CAP channels. */
#ifndef CTP_FLUSH_TO
#define CTP_FLUSH_TO                0xFFFF
#endif

/* The default service name for CTP. */
#ifndef CTP_DEFAULT_SERVICE_NAME
#define CTP_DEFAULT_SERVICE_NAME    "Cordless Telephony"
#endif

/* The CTP security level. */
#ifndef CTP_SECURITY_LEVEL
#define CTP_SECURITY_LEVEL          (BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT)
#endif

/* The number of lines to the external network. */
#ifndef CTP_MAX_LINES
#define CTP_MAX_LINES               1
#endif

/* Test if the number of resources in TCS is consistent with CTP setting. */
#ifndef CTP_TEST_FULL_TCS
#define CTP_TEST_FULL_TCS           TRUE
#endif

/* The default inquiry mode. */
#ifndef CTP_DEFAULT_INQUIRY_MODE
#define CTP_DEFAULT_INQUIRY_MODE    BTM_GENERAL_INQUIRY
#endif

/* The default inquiry duration. */
#ifndef CTP_DEFAULT_INQ_DURATION
#define CTP_DEFAULT_INQ_DURATION    4
#endif

/* The maximum number of inquiry responses. */
#ifndef CTP_DEFAULT_INQ_MAX_RESP
#define CTP_DEFAULT_INQ_MAX_RESP    3
#endif

/* When TL does not create another L2CAP channel within this period of time GW declares that it's "Connected Limited". */
#ifndef CTP_TL_CONN_TIMEOUT
#define CTP_TL_CONN_TIMEOUT         5
#endif

/* The delay for ACL to completely disconnect (for intercom) before sending the connect request to GW. */
#ifndef CTP_RECONNECT_DELAY
#define CTP_RECONNECT_DELAY         5
#endif

/* How many times to retry connection when it has failed. */
#ifndef CTP_RETRY_ON_CONN_ERR
#define CTP_RETRY_ON_CONN_ERR       5
#endif

/******************************************************************************
**
** ICP
**
******************************************************************************/

#ifndef ICP_INCLUDED
#define ICP_INCLUDED                FALSE
#endif

/* The ICP default MTU. */
#ifndef ICP_MTU_SIZE
#define ICP_MTU_SIZE                100
#endif

/* The ICP security level. */
#ifndef ICP_SECURITY_LEVEL
#define ICP_SECURITY_LEVEL          BTM_SEC_NONE
#endif

/* The default service name for ICP. */
#ifndef ICP_DEFAULT_SERVICE_NAME
#define ICP_DEFAULT_SERVICE_NAME    "Intercom"
#endif

/* The flush timeout for L2CAP channels. */
#ifndef ICP_FLUSH_TO
#define ICP_FLUSH_TO                0xFFFF
#endif

/******************************************************************************
**
** SPP
**
******************************************************************************/

#ifndef SPP_INCLUDED
#define SPP_INCLUDED                FALSE
#endif

/* The SPP default MTU. */
#ifndef SPP_DEFAULT_MTU
#define SPP_DEFAULT_MTU             127
#endif

/* The interval, in seconds, that a client tries to reconnect to a service. */
#ifndef SPP_RETRY_CONN_INTERVAL
#define SPP_RETRY_CONN_INTERVAL     1
#endif

/* The SPP discoverable mode: limited or general. */
#ifndef SPP_DISCOVERABLE_MODE
#define SPP_DISCOVERABLE_MODE       BTM_GENERAL_DISCOVERABLE
#endif

/* The maximum number of inquiry results returned in by inquiry procedure. */
#ifndef SPP_DEF_INQ_MAX_RESP
#define SPP_DEF_INQ_MAX_RESP        10
#endif

/* The SPP discovery record size. */
#ifndef SPP_DISC_REC_SIZE
#define SPP_DISC_REC_SIZE           60
#endif

#ifndef SPP_MAX_RECS_PER_DEVICE
#define SPP_MAX_RECS_PER_DEVICE     (SPP_DB_SIZE / SPP_DISC_REC_SIZE)
#endif

/* Inquiry duration in 1.28 second units. */
#ifndef SPP_DEF_INQ_DURATION
#define SPP_DEF_INQ_DURATION        9
#endif

/* keep the raw data received from SDP server in database. */
#ifndef SDP_RAW_DATA_INCLUDED
#define SDP_RAW_DATA_INCLUDED       TRUE
#endif

/* TRUE, to allow JV to create L2CAP connection on SDP PSM. */
#ifndef SDP_FOR_JV_INCLUDED
#define SDP_FOR_JV_INCLUDED         FALSE
#endif

/* Inquiry duration in 1.28 second units. */
#ifndef SDP_DEBUG
#define SDP_DEBUG                   TRUE
#endif

/******************************************************************************
**
** HSP2, HFP
**
******************************************************************************/

#ifndef HSP2_INCLUDED
#define HSP2_INCLUDED               FALSE
#endif

/* Include the ability to perform inquiry for peer devices. */
#ifndef HSP2_INQUIRY_INCLUDED
#define HSP2_INQUIRY_INCLUDED       TRUE
#endif

/* Include Audio Gateway specific code. */
#ifndef HSP2_AG_INCLUDED
#define HSP2_AG_INCLUDED            TRUE
#endif

/* Include Headset Specific Code. */
#ifndef HSP2_HS_INCLUDED
#define HSP2_HS_INCLUDED            TRUE
#endif

/* Include the ability to open an SCO connection for In-Band Ringing. */
#ifndef HSP2_IB_RING_INCLUDED
#define HSP2_IB_RING_INCLUDED       TRUE
#endif

/* Include the ability to repeat a ring. */
#ifndef HSP2_AG_REPEAT_RING
#define HSP2_AG_REPEAT_RING         TRUE
#endif

#ifndef HSP2_APP_CLOSES_ON_CKPD
#define HSP2_APP_CLOSES_ON_CKPD     FALSE
#endif


/* Include the ability to park a connection. */
#ifndef HSP2_PARK_INCLUDED
#define HSP2_PARK_INCLUDED          TRUE
#endif

/* Include HSP State Machine debug trace messages. */
#ifndef HSP2_FSM_DEBUG
#define HSP2_FSM_DEBUG              TRUE
#endif

/* The Module's Inquiry Scan Window. */
#ifndef HSP2_INQ_SCAN_WINDOW
#define HSP2_INQ_SCAN_WINDOW        0
#endif

/* The Module's Inquiry Scan Interval. */
#ifndef HSP2_INQ_SCAN_INTERVAL
#define HSP2_INQ_SCAN_INTERVAL      0
#endif

/* The Module's Page Scan Interval. */
#ifndef HSP2_PAGE_SCAN_INTERVAL
#define HSP2_PAGE_SCAN_INTERVAL     0
#endif

/* The Module's Page Scan Window. */
#ifndef HSP2_PAGE_SCAN_WINDOW
#define HSP2_PAGE_SCAN_WINDOW       0
#endif

/* The Park Mode's Minimum Beacon Period. */
#ifndef HSP2_BEACON_MIN_PERIOD
#define HSP2_BEACON_MIN_PERIOD      450
#endif

/* The Park Mode's Maximum Beacon Period. */
#ifndef HSP2_BEACON_MAX_PERIOD
#define HSP2_BEACON_MAX_PERIOD      500
#endif

/* The duration of the inquiry in seconds. */
#ifndef HSP2_INQ_DURATION
#define HSP2_INQ_DURATION           4
#endif

/* Maximum number of peer responses during an inquiry. */
#ifndef HSP2_INQ_MAX_NUM_RESPS
#define HSP2_INQ_MAX_NUM_RESPS      3
#endif

/* Maximum number of times to retry an inquiry prior to failure. */
#ifndef HSP2_MAX_INQ_RETRY
#define HSP2_MAX_INQ_RETRY          6
#endif

/* Maximum number of times to retry an RFCOMM connection prior to failure. */
#ifndef HSP2_MAX_CONN_RETRY
#define HSP2_MAX_CONN_RETRY         3
#endif

/* If the connect request failed for authentication reasons, do not retry */
#ifndef HSP2_NO_RETRY_ON_AUTH_FAIL
#define HSP2_NO_RETRY_ON_AUTH_FAIL  TRUE
#endif

/* Maximum number of characters in an HSP2 device name. */
#ifndef HSP2_MAX_NAME_LEN
#define HSP2_MAX_NAME_LEN           32
#endif

/* The minimum speaker and/or microphone gain setting. */
#ifndef HSP2_MIN_GAIN
#define HSP2_MIN_GAIN               0
#endif

/* The maximum speaker and/or microphone setting. */
#ifndef HSP2_MAX_GAIN
#define HSP2_MAX_GAIN               15
#endif

/* The default value to send on an AT+CKPD. */
#ifndef HSP2_KEYPRESS_DEFAULT
#define HSP2_KEYPRESS_DEFAULT       200
#endif

/* Maximum amount a data that can be received per RFCOMM frame. */
#ifndef HSP2_MAX_RFC_READ_LEN
#define HSP2_MAX_RFC_READ_LEN       128
#endif

/* The time in seconds to wait for completion of a partial AT command or response from the peer. */
#ifndef HSP2_AT_TO_INTERVAL
#define HSP2_AT_TO_INTERVAL         30
#endif

/* The time to wait before repeating a ring to a peer Headset. */
#ifndef HSP2_REPEAT_RING_TO
#define HSP2_REPEAT_RING_TO         4
#endif

/* Time to wait for a response for an AT command */
#ifndef HSP2_AT_RSP_TO
#define HSP2_AT_RSP_TO              20
#endif

/* SCO packet type(s) to use (bitmask: see spec), 0 - device default (recommended) */
#ifndef HSP2_SCO_PKT_TYPES
#define HSP2_SCO_PKT_TYPES          ((UINT16)0x0000)
#endif

/* The default settings of the SCO voice link. */
#ifndef HSP2_DEFAULT_VOICE_SETTINGS
#define HSP2_DEFAULT_VOICE_SETTINGS (HCI_INP_CODING_LINEAR | HCI_INP_DATA_FMT_2S_COMPLEMENT | HCI_INP_SAMPLE_SIZE_16BIT | HCI_AIR_CODING_FORMAT_CVSD)
#endif

#ifndef HSP2_MAX_AT_CMD_LENGTH
#define HSP2_MAX_AT_CMD_LENGTH       16
#endif

#ifndef HSP2_MAX_AT_VAL_LENGTH
#if (defined(HFP_INCLUDED) && HFP_INCLUDED == TRUE)
#define HSP2_MAX_AT_VAL_LENGTH       310
#else
#define HSP2_MAX_AT_VAL_LENGTH       5
#endif
#endif


#ifndef HSP2_SDP_DB_SIZE
#define HSP2_SDP_DB_SIZE             300
#endif


/******************************************************************************
**
** HFP
**
******************************************************************************/

#ifndef HFP_INCLUDED
#define HFP_INCLUDED                FALSE
#endif

/* Include Audio Gateway specific code. */
#ifndef HFP_AG_INCLUDED
#define HFP_AG_INCLUDED             TRUE
#endif

/* Include Hand Free Specific Code. */
#ifndef HFP_HF_INCLUDED
#define HFP_HF_INCLUDED             TRUE
#endif

/* Use AT interface instead of full blown API */
#ifndef AT_INTERFACE
#define AT_INTERFACE            FALSE
#endif

/* HFP Manages SCO establishement for various procedures */
#ifndef HFP_SCO_MGMT_INCLUDED
#define HFP_SCO_MGMT_INCLUDED             TRUE
#endif

/* CCAP compliant features and behavior desired */
#ifndef CCAP_COMPLIANCE
#define CCAP_COMPLIANCE             TRUE
#endif

/* Caller ID string, part of +CLIP result code */
#ifndef HFP_MAX_CLIP_INFO
#define HFP_MAX_CLIP_INFO             45
#endif

#ifndef HFP_RPT_PEER_INFO_INCLUDED
#define HFP_RPT_PEER_INFO_INCLUDED  TRUE  /* Reporting of peer features enabled */
#endif

/******************************************************************************
**
** HID
**
******************************************************************************/

/* HID Device Role Included */
#ifndef HID_DEV_INCLUDED
#define HID_DEV_INCLUDED             FALSE
#endif

#ifndef HID_DEV_PM_INCLUDED
#define HID_DEV_PM_INCLUDED         TRUE
#endif

/* The HID Device is a virtual cable */
#ifndef HID_DEV_VIRTUAL_CABLE
#define HID_DEV_VIRTUAL_CABLE       TRUE
#endif

/* The HID device initiates the reconnections */
#ifndef HID_DEV_RECONN_INITIATE
#define HID_DEV_RECONN_INITIATE     TRUE
#endif

/* THe HID device is normally connectable */
#ifndef HID_DEV_NORMALLY_CONN
#define HID_DEV_NORMALLY_CONN       FALSE
#endif

/* The device is battery powered */
#ifndef HID_DEV_BATTERY_POW
#define HID_DEV_BATTERY_POW         TRUE
#endif

/* Device is capable of waking up the host */
#ifndef HID_DEV_REMOTE_WAKE
#define HID_DEV_REMOTE_WAKE         TRUE
#endif

/* Device needs host to close SDP channel after SDP is over */
#ifndef HID_DEV_SDP_DISABLE
#define HID_DEV_SDP_DISABLE         TRUE
#endif

#ifndef HID_DEV_MTU_SIZE
#define HID_DEV_MTU_SIZE                 64
#endif

#ifndef HID_DEV_FLUSH_TO
#define HID_DEV_FLUSH_TO                 0xffff
#endif

#ifndef HID_DEV_PAGE_SCAN_WIN
#define HID_DEV_PAGE_SCAN_WIN       (0)
#endif

#ifndef HID_DEV_PAGE_SCAN_INT
#define HID_DEV_PAGE_SCAN_INT       (0)
#endif

#ifndef HID_DEV_MAX_CONN_RETRY
#define HID_DEV_MAX_CONN_RETRY      (15)
#endif

#ifndef HID_DEV_REPAGE_WIN
#define HID_DEV_REPAGE_WIN          (1)
#endif

#ifndef HID_DEV_SVC_NAME
#define HID_DEV_SVC_NAME            "HID"
#endif

#ifndef HID_DEV_SVC_DESCR
#define HID_DEV_SVC_DESCR           "3-button mouse and keyboard"
#endif

#ifndef HID_DEV_PROVIDER_NAME
#define HID_DEV_PROVIDER_NAME       "Widcomm"
#endif

#ifndef HID_DEV_REL_NUM
#define HID_DEV_REL_NUM             0x0100
#endif

#ifndef HID_DEV_PARSER_VER
#define HID_DEV_PARSER_VER          0x0111
#endif

#ifndef HID_DEV_SUBCLASS
#define HID_DEV_SUBCLASS            COD_MINOR_POINTING
#endif

#ifndef HID_DEV_COUNTRY_CODE
#define HID_DEV_COUNTRY_CODE        0x33
#endif

#ifndef HID_DEV_SUP_TOUT
#define HID_DEV_SUP_TOUT            0x8000
#endif

#ifndef HID_DEV_NUM_LANGS
#define HID_DEV_NUM_LANGS           1
#endif

#ifndef HID_DEV_INACT_TIMEOUT
#define HID_DEV_INACT_TIMEOUT       60
#endif

#ifndef HID_DEV_BUSY_MODE_PARAMS
#define HID_DEV_BUSY_MODE_PARAMS    { 320, 160, 10, 20, HCI_MODE_ACTIVE }
#endif

#ifndef HID_DEV_IDLE_MODE_PARAMS
#define HID_DEV_IDLE_MODE_PARAMS    { 320, 160, 10, 20, HCI_MODE_SNIFF }
#endif

#ifndef HID_DEV_SUSP_MODE_PARAMS
#define HID_DEV_SUSP_MODE_PARAMS    { 640, 320,  0,    0, HCI_MODE_PARK }
#endif

#ifndef HID_DEV_MAX_DESCRIPTOR_SIZE
#define HID_DEV_MAX_DESCRIPTOR_SIZE      128     /* Max descriptor size          */
#endif

#ifndef HID_DEV_LANGUAGELIST
#define HID_DEV_LANGUAGELIST             {0x35, 0x06, 0x09, 0x04, 0x09, 0x09, 0x01, 0x00}
#endif

#ifndef HID_DEV_LINK_SUPERVISION_TO
#define HID_DEV_LINK_SUPERVISION_TO      0x8000
#endif

#ifndef HID_CONTROL_POOL_ID
#define HID_CONTROL_POOL_ID             2
#endif

#ifndef HID_INTERRUPT_POOL_ID
#define HID_INTERRUPT_POOL_ID           2
#endif

/*************************************************************************
** Definitions for Both HID-Host & Device
*/
#ifndef HID_MAX_SVC_NAME_LEN
#define HID_MAX_SVC_NAME_LEN  32
#endif

#ifndef HID_MAX_SVC_DESCR_LEN
#define HID_MAX_SVC_DESCR_LEN 32
#endif

#ifndef HID_MAX_PROV_NAME_LEN
#define HID_MAX_PROV_NAME_LEN 32
#endif

/*************************************************************************
** Definitions for HID-Host
*/
#ifndef  HID_HOST_INCLUDED
#define HID_HOST_INCLUDED           TRUE
#endif

#ifndef HID_HOST_MAX_DEVICES
#define HID_HOST_MAX_DEVICES        7
#endif

#ifndef HID_HOST_MTU
#define HID_HOST_MTU                640
#endif

#ifndef HID_HOST_FLUSH_TO
#define HID_HOST_FLUSH_TO                 0xffff
#endif

#ifndef HID_HOST_MAX_CONN_RETRY
#define HID_HOST_MAX_CONN_RETRY     (3)
#endif

#ifndef HID_HOST_REPAGE_WIN
#define HID_HOST_REPAGE_WIN          (2)
#endif


/******************************************************************************
**
** DUN and FAX
**
******************************************************************************/

#ifndef DUN_INCLUDED
#define DUN_INCLUDED                FALSE
#endif


/******************************************************************************
**
** GOEP
**
******************************************************************************/

#ifndef GOEP_INCLUDED
#define GOEP_INCLUDED               FALSE
#endif

/* This is set to enable GOEP non-blocking file system access functions. */
#ifndef GOEP_FS_INCLUDED
#define GOEP_FS_INCLUDED        FALSE
#endif

/* GOEP authentication key size. */
#ifndef GOEP_MAX_AUTH_KEY_SIZE
#define GOEP_MAX_AUTH_KEY_SIZE      16
#endif

/* Maximum size of the realm authentication string. */
#ifndef GOEP_MAX_AUTH_REALM_SIZE
#define GOEP_MAX_AUTH_REALM_SIZE    16
#endif

/* Realm Character Set */
#ifndef GOEP_REALM_CHARSET
#define GOEP_REALM_CHARSET          0       /* ASCII */
#endif

/* This is set to the maximum length of path name allowed in the system (_MAX_PATH). */
#ifndef GOEP_MAX_PATH_SIZE
#define GOEP_MAX_PATH_SIZE          255
#endif

/* Specifies whether or not client's user id is required during obex authentication */
#ifndef GOEP_SERVER_USERID_REQUIRED
#define GOEP_SERVER_USERID_REQUIRED FALSE
#endif

/* This is set to the maximum length of file name allowed in the system (_MAX_FNAME). */
#ifndef GOEP_MAX_FILE_SIZE
#define GOEP_MAX_FILE_SIZE          128
#endif

/* Character used as path separator */
#ifndef GOEP_PATH_SEPARATOR
#define GOEP_PATH_SEPARATOR         ((char) 0x5c)   /* 0x2f ('/'), or 0x5c ('\') */
#endif

/******************************************************************************
**
** OPP
**
******************************************************************************/

#ifndef OPP_INCLUDED
#define OPP_INCLUDED                FALSE
#endif

/* This is set to enable OPP client capabilities. */
#ifndef OPP_CLIENT_INCLUDED
#define OPP_CLIENT_INCLUDED         FALSE
#endif

/* This is set to enable OPP server capabilities. */
#ifndef OPP_SERVER_INCLUDED
#define OPP_SERVER_INCLUDED         FALSE
#endif

/* if the optional formating functions are to be included or not */
#ifndef OPP_FORMAT_INCLUDED
#define OPP_FORMAT_INCLUDED         FALSE
#endif

/* Maximum number of client sessions allowed by server */
#ifndef OPP_MAX_SRVR_SESS
#define OPP_MAX_SRVR_SESS           3
#endif

/******************************************************************************
**
** FTP
**
******************************************************************************/

#ifndef FTP_INCLUDED
#define FTP_INCLUDED                FALSE
#endif

/* This is set to enable FTP client capabilities. */
#ifndef FTP_CLIENT_INCLUDED
#define FTP_CLIENT_INCLUDED         TRUE
#endif

/* This is set to enable FTP server capabilities. */
#ifndef FTP_SERVER_INCLUDED
#define FTP_SERVER_INCLUDED         TRUE
#endif

/******************************************************************************
**
** XML Parser
**
******************************************************************************/

#ifndef XML_STACK_SIZE
#define XML_STACK_SIZE             7
#endif

/******************************************************************************
**
** BPP Printer
**
******************************************************************************/
#ifndef BPP_DEBUG
#define BPP_DEBUG            FALSE
#endif

#ifndef BPP_INCLUDED
#define BPP_INCLUDED                FALSE
#endif

#ifndef BPP_SND_INCLUDED
#define BPP_SND_INCLUDED            FALSE
#endif

/* Maximum number of senders allowed to connect simultaneously
** The maximum is 6 or (OBX_NUM_SERVERS / 2), whichever is smaller
*/
#ifndef BPP_PR_MAX_CON
#define BPP_PR_MAX_CON         3
#endif

/* Service Name. maximum length: 248
#ifndef BPP_SERVICE_NAME
#define BPP_SERVICE_NAME            "Basic Printing"
#endif
 */
/* Document Format Supported. ASCII comma-delimited list of MIME type:version string
#ifndef BPP_DOC_FORMAT_SUPPORTED
#define BPP_DOC_FORMAT_SUPPORTED    "application/vnd.pwg-xhtml-print:1.0,application/vnd.hp-PCL:5E,application/PDF"
#endif

#ifndef BPP_DOC_FORMAT_SUPPORTED_LEN
#define BPP_DOC_FORMAT_SUPPORTED_LEN    77
#endif
 */
/* Character repertoires.
#ifndef BPP_CHARACTER_REPERTOIRES
#define BPP_CHARACTER_REPERTOIRES {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}
#endif
 */
/* XHTML formats.
#ifndef BPP_XHTML_PRINT_FORMATS
#define BPP_XHTML_PRINT_FORMATS     "image/gif:89A,image/jpeg"
#endif

#ifndef BPP_XHTML_PRINT_FORMATS_LEN
#define BPP_XHTML_PRINT_FORMATS_LEN 24
#endif
 */
/* Color supported.
#ifndef BPP_COLOR_SUPORTED
#define BPP_COLOR_SUPORTED          FALSE
#endif
 */
/* 1284 ID string. First 2 bytes are the length.
#ifndef BPP_1284ID
#define BPP_1284ID                  "\x00\x48MANUFACTURER:ACME Manufacturing;COMMAND SET:PCL,MPL;MODEL:LaserBeam \?;"
#endif

#ifndef BPP_1284ID_LEN
#define BPP_1284ID_LEN              72
#endif
 */
/* Printer name.
#ifndef BPP_PRINTER_NAME
#define BPP_PRINTER_NAME            "My Printer"
#endif

#ifndef BPP_PRINTER_NAME_LEN
#define BPP_PRINTER_NAME_LEN        10
#endif
 */

/* Printer location.
#ifndef BPP_PRINTER_LOCATION
#define BPP_PRINTER_LOCATION        "Hotel Lobby"
#endif

#ifndef BPP_PRINTER_LOCATION_LEN
#define BPP_PRINTER_LOCATION_LEN    11
#endif
 */
/* Duplex printing supported.
#ifndef BPP_DUPLEX_SUPPORTED
#define BPP_DUPLEX_SUPPORTED        TRUE
#endif
 */

/* Media types supported.
#ifndef BPP_MEDIA_TYPES_SUPPORTED
#define BPP_MEDIA_TYPES_SUPPORTED   "stationary,continuous-long,photographic-high-gloss,cardstock"
#endif

#ifndef BPP_MEDIA_TYPES_SUPPORTED_LEN
#define BPP_MEDIA_TYPES_SUPPORTED_LEN   60
#endif
 */
/* Maximum media with supported.
#ifndef BPP_MAX_MEDIA_WIDTH
#define BPP_MAX_MEDIA_WIDTH         205
#endif
 */
/* Maximum media length supported.
#ifndef BPP_MAX_MEDIA_LENGTH
#define BPP_MAX_MEDIA_LENGTH        285
#endif
 */
/* the maximum string len for the media size of medium loaded */
#ifndef BPP_MEDIA_SIZE_LEN
#define BPP_MEDIA_SIZE_LEN          33
#endif

/* Debug Trace the SOAP object, if TRUE */
#ifndef BPP_TRACE_XML
#define BPP_TRACE_XML               TRUE
#endif

/* in case that the SOAP object does not all come in one OBEX packet,
 * this size of data may be kept in the BPP control block for continuing parsing.
 * The maximum is the size of the biggest GKI buffer (GKI_MAX_BUF_SIZE) */
#ifndef BPP_SOAP_KEEP_SIZE
#define BPP_SOAP_KEEP_SIZE          200
#endif


/******************************************************************************
**
** BIP
**
******************************************************************************/
#ifndef BIP_INCLUDED
#define BIP_INCLUDED                FALSE
#endif

/* TRUE to include imaging initiator */
#ifndef BIP_INITR_INCLUDED
#define BIP_INITR_INCLUDED          FALSE
#endif

/* TRUE to include imaging responder */
#ifndef BIP_RSPDR_INCLUDED
#define BIP_RSPDR_INCLUDED          FALSE
#endif

/* TRUE to include image push feature */
#ifndef BIP_PUSH_INCLUDED
#define BIP_PUSH_INCLUDED           TRUE
#endif

/* TRUE to include image pull feature */
#ifndef BIP_PULL_INCLUDED
#define BIP_PULL_INCLUDED           TRUE
#endif

/* TRUE to include advanced image printing feature */
#ifndef BIP_PRINTING_INCLUDED
#define BIP_PRINTING_INCLUDED       TRUE
#endif

/* TRUE to include automatic archive feature */
#ifndef BIP_ARCHIVE_INCLUDED
#define BIP_ARCHIVE_INCLUDED        TRUE
#endif

/* TRUE to include remote camera feature */
#ifndef BIP_CAMERA_INCLUDED
#define BIP_CAMERA_INCLUDED         TRUE
#endif

/* TRUE to include remote display feature */
#ifndef BIP_DISPLAY_INCLUDED
#define BIP_DISPLAY_INCLUDED        TRUE
#endif

/* TRUE to include sanity check code for API functions */
#ifndef BIP_SANITY_CHECKS
#define BIP_SANITY_CHECKS           TRUE
#endif

/* TRUE to show the received XML object in trace for conformance tests */
#ifndef BIP_TRACE_XML
#define BIP_TRACE_XML               TRUE
#endif

/* in case that the received XML object is not complete, the XML parser state machine needs
 * to keep a copy of the data from the last '<'
 * This macro specifies the maximun amount of data for this purpose */
#ifndef BIP_XML_CARRY_OVER_LEN
#define BIP_XML_CARRY_OVER_LEN      100
#endif

/* minimum 4, maximum is 255. The value should be set to the maximum size of encoding string + 1. JPEG2000.
 * If vendor specific format is supported, it might be bigger than 9 */
#ifndef BIP_IMG_ENCODE_SIZE
#define BIP_IMG_ENCODE_SIZE         9
#endif

/* MIME type: text/plain */
#ifndef BIP_TYPE_SIZE
#define BIP_TYPE_SIZE               20
#endif

/* example: iso-8895-1 */
#ifndef BIP_CHARSET_SIZE
#define BIP_CHARSET_SIZE            10
#endif

/* friendly name */
#ifndef BIP_FNAME_SIZE
#define BIP_FNAME_SIZE              20
#endif

/* service name */
#ifndef BIP_SNAME_SIZE
#define BIP_SNAME_SIZE              60
#endif

/* temporary storage file name(for file system access, may include path) */
#ifndef BIP_TEMP_NAME_SIZE
#define BIP_TEMP_NAME_SIZE          200
#endif

/* image file name */
#ifndef BIP_IMG_NAME_SIZE
#define BIP_IMG_NAME_SIZE           200
#endif

/* attachment file name */
#ifndef BIP_ATT_NAME_SIZE
#define BIP_ATT_NAME_SIZE           200
#endif

/* object (image, attachment, thumbnail) file name (may be used for file system) */
#ifndef BIP_OBJ_NAME_SIZE
#define BIP_OBJ_NAME_SIZE           200
#endif



/******************************************************************************
**
** HCRP
**
******************************************************************************/

#ifndef HCRP_INCLUDED
#define HCRP_INCLUDED               FALSE
#endif

/* This is set to enable server. */
#ifndef HCRP_SERVER_INCLUDED
#define HCRP_SERVER_INCLUDED       FALSE
#endif

/* This is set to enable client. */
#ifndef HCRP_CLIENT_INCLUDED
#define HCRP_CLIENT_INCLUDED        FALSE
#endif

/* TRUE enables the notification option of the profile. */
#ifndef HCRP_NOTIFICATION_INCLUDED
#define HCRP_NOTIFICATION_INCLUDED  TRUE
#endif

/* TRUE enables the vendor specific option of the profile. */
#ifndef HCRP_VENDOR_SPEC_INCLUDED
#define HCRP_VENDOR_SPEC_INCLUDED   TRUE
#endif

/* TRUE enables state machine traces. */
#ifndef HCRP_FSM_DEBUG
#define HCRP_FSM_DEBUG              FALSE
#endif

/* TRUE enables protocol message traces. */
#ifndef HCRP_PROTO_DEBUG
#define HCRP_PROTO_DEBUG            FALSE
#endif

/* Maximum length used to store the service name (Minimum 1). */
#ifndef HCRP_MAX_SERVICE_NAME_LEN
#define HCRP_MAX_SERVICE_NAME_LEN   32
#endif

/* Maximum length used to store the device name (Minimum 1). */
#ifndef HCRP_MAX_DEVICE_NAME_LEN
#define HCRP_MAX_DEVICE_NAME_LEN    32
#endif

/* Maximum length of device location (Minimum 1) */
#ifndef HCRP_MAX_DEVICE_LOC_LEN
#define HCRP_MAX_DEVICE_LOC_LEN     32
#endif

/* Maximum length used to store the friendly name (Minimum 1). */
#ifndef HCRP_MAX_FRIENDLY_NAME_LEN
#define HCRP_MAX_FRIENDLY_NAME_LEN  32
#endif

/* Maximum length used to store the 1284 id string (Minimum 2 byte length field). */
#ifndef HCRP_MAX_SDP_1284_ID_LEN
#define HCRP_MAX_SDP_1284_ID_LEN    128
#endif

/* Maximum length for parameters to be processed for vendor specific commands. */
#ifndef HCRP_MAX_VEND_SPEC_LEN
#define HCRP_MAX_VEND_SPEC_LEN      4
#endif

/* Number of seconds to wait for 2nd GAP to open. */
#ifndef HCRP_OPEN_CHAN_TOUT
#define HCRP_OPEN_CHAN_TOUT         5
#endif

/* Number of seconds to wait for 2nd GAP to close. */
#ifndef HCRP_CLOSE_CHAN_TOUT
#define HCRP_CLOSE_CHAN_TOUT        3
#endif

/* Number of seconds to wait for the application to respond to a protocol request. */
#ifndef HCRP_APPL_RSP_TOUT
#define HCRP_APPL_RSP_TOUT          5
#endif

/* Number of seconds to wait for the peer device to respond to a protocol request. */
#ifndef HCRP_CMD_RSP_TOUT
#define HCRP_CMD_RSP_TOUT           7
#endif

/* Number of seconds between subsequent credit requests to the server when the send watermark has been exceeded. */
#ifndef HCRP_CREDIT_REQ_UPDATES
#define HCRP_CREDIT_REQ_UPDATES     1
#endif

/* Maximum number of results to return in a HCRP_FindServices search. */
#ifndef HCRP_MAX_SEARCH_RESULTS
#define HCRP_MAX_SEARCH_RESULTS     1
#endif

/* Maximum number of bytes to be reserved for searching for the client's notification record. */
#ifndef HCRP_MAX_NOTIF_DISC_BUF
#define HCRP_MAX_NOTIF_DISC_BUF     300
#endif

/* Maximum number of clients the server will allow to be registered for notifications. */
#ifndef HCRP_MAX_NOTIF_CLIENTS
#define HCRP_MAX_NOTIF_CLIENTS      3
#endif

/* Spec says minimum of two notification retries. */
#ifndef HCRP_NOTIF_NUM_RETRIES
#define HCRP_NOTIF_NUM_RETRIES      4
#endif

/*************************************************************************
** Definitions for Multi-Client Server HCRP
** Note: Many of the above HCRP definitions are also used
** Maximum number of clients allowed to connect simultaneously
** Must be less than ((GAP_MAX_CONNECTIONS - 1) / 2)
*/
#ifndef HCRPM_MAX_CLIENTS
#define HCRPM_MAX_CLIENTS           3
#endif


/******************************************************************************
**
** PAN
**
******************************************************************************/

#ifndef PAN_INCLUDED
#define PAN_INCLUDED                FALSE
#endif


/******************************************************************************
**
** SAP
**
******************************************************************************/

#ifndef SAP_SERVER_INCLUDED
#define SAP_SERVER_INCLUDED         FALSE
#endif


/*************************************************************************
 * A2DP Definitions
 */
#ifndef A2D_INCLUDED
#define A2D_INCLUDED            TRUE
#endif

/* TRUE to include SBC utility functions */
#ifndef A2D_SBC_INCLUDED
#define A2D_SBC_INCLUDED        A2D_INCLUDED
#endif

/* TRUE to include MPEG-1,2 (mp3) utility functions */
#ifndef A2D_M12_INCLUDED
#define A2D_M12_INCLUDED        A2D_INCLUDED
#endif

/* TRUE to include MPEG-2,4 (aac) utility functions */
#ifndef A2D_M24_INCLUDED
#define A2D_M24_INCLUDED        A2D_INCLUDED
#endif

/******************************************************************************
**
** AVCTP
**
******************************************************************************/

#ifndef AVCT_INCLUDED
#define AVCT_INCLUDED               TRUE
#endif

/* Number of simultaneous ACL links to different peer devices. */
#ifndef AVCT_NUM_LINKS
#define AVCT_NUM_LINKS              2
#endif

/* Number of simultaneous AVCTP connections. */
#ifndef AVCT_NUM_CONN
#define AVCT_NUM_CONN               3
#endif

/* Pool ID where to reassemble the SDU.
   This Pool allows buffers to be used that are larger than
   the L2CAP_MAX_MTU. */
#ifndef AVCT_BR_USER_RX_POOL_ID
#define AVCT_BR_USER_RX_POOL_ID     HCI_ACL_POOL_ID
#endif

/* Pool ID where to hold the SDU.
   This Pool allows buffers to be used that are larger than
   the L2CAP_MAX_MTU. */
#ifndef AVCT_BR_USER_TX_POOL_ID
#define AVCT_BR_USER_TX_POOL_ID     HCI_ACL_POOL_ID
#endif

/*
GKI Buffer Pool ID used to hold MPS segments during SDU reassembly
*/
#ifndef AVCT_BR_FCR_RX_POOL_ID
#define AVCT_BR_FCR_RX_POOL_ID      HCI_ACL_POOL_ID
#endif

/*
GKI Buffer Pool ID used to hold MPS segments used in (re)transmissions.
L2CAP_DEFAULT_ERM_POOL_ID is specified to use the HCI ACL data pool.
Note:  This pool needs to have enough buffers to hold two times the window size negotiated
 in the tL2CAP_FCR_OPTIONS (2 * tx_win_size)  to allow for retransmissions.
 The size of each buffer must be able to hold the maximum MPS segment size passed in
 tL2CAP_FCR_OPTIONS plus BT_HDR (8) + HCI preamble (4) + L2CAP_MIN_OFFSET (11 - as of BT 2.1 + EDR Spec).
*/
#ifndef AVCT_BR_FCR_TX_POOL_ID
#define AVCT_BR_FCR_TX_POOL_ID      HCI_ACL_POOL_ID
#endif

/* AVCTP Browsing channel FCR Option:
Size of the transmission window when using enhanced retransmission mode. Not used
in basic and streaming modes. Range: 1 - 63
*/
#ifndef AVCT_BR_FCR_OPT_TX_WINDOW_SIZE
#define AVCT_BR_FCR_OPT_TX_WINDOW_SIZE      10
#endif

/* AVCTP Browsing channel FCR Option:
Number of transmission attempts for a single I-Frame before taking
Down the connection. Used In ERTM mode only. Value is Ignored in basic and
Streaming modes.
Range: 0, 1-0xFF
0 - infinite retransmissions
1 - single transmission
*/
#ifndef AVCT_BR_FCR_OPT_MAX_TX_B4_DISCNT
#define AVCT_BR_FCR_OPT_MAX_TX_B4_DISCNT    20
#endif

/* AVCTP Browsing channel FCR Option: Retransmission Timeout
The AVRCP specification set a value in the range of 300 - 2000 ms
Timeout (in msecs) to detect Lost I-Frames. Only used in Enhanced retransmission mode.
Range: Minimum 2000 (2 secs) when supporting PBF.
 */
#ifndef AVCT_BR_FCR_OPT_RETX_TOUT
#define AVCT_BR_FCR_OPT_RETX_TOUT           2000
#endif

/* AVCTP Browsing channel FCR Option: Monitor Timeout
The AVRCP specification set a value in the range of 300 - 2000 ms
Timeout (in msecs) to detect Lost S-Frames. Only used in Enhanced retransmission mode.
Range: Minimum 12000 (12 secs) when supporting PBF.
*/
#ifndef AVCT_BR_FCR_OPT_MONITOR_TOUT
#define AVCT_BR_FCR_OPT_MONITOR_TOUT        12000
#endif

/******************************************************************************
**
** AVRCP
**
******************************************************************************/

#ifndef AVRC_INCLUDED
#define AVRC_INCLUDED               TRUE
#endif

/******************************************************************************
**
** MCAP
**
******************************************************************************/
#ifndef MCA_INCLUDED
#define MCA_INCLUDED                FALSE
#endif

/* TRUE to support Clock Synchronization OpCodes */
#ifndef MCA_SYNC_INCLUDED
#define MCA_SYNC_INCLUDED           FALSE
#endif

/* The MTU size for the L2CAP configuration on control channel. 48 is the minimal */
#ifndef MCA_CTRL_MTU
#define MCA_CTRL_MTU    60
#endif

/* The maximum number of registered MCAP instances. */
#ifndef MCA_NUM_REGS
#define MCA_NUM_REGS    3
#endif

/* The maximum number of control channels (to difference devices) per registered MCAP instances. */
#ifndef MCA_NUM_LINKS
#define MCA_NUM_LINKS   3
#endif

/* The maximum number of MDEP (including HDP echo) per registered MCAP instances. */
#ifndef MCA_NUM_DEPS
#define MCA_NUM_DEPS    3
#endif

/* The maximum number of MDL link per control channel. */
#ifndef MCA_NUM_MDLS
#define MCA_NUM_MDLS    4
#endif

/* Pool ID where to reassemble the SDU. */
#ifndef MCA_USER_RX_POOL_ID
#define MCA_USER_RX_POOL_ID     HCI_ACL_POOL_ID
#endif

/* Pool ID where to hold the SDU. */
#ifndef MCA_USER_TX_POOL_ID
#define MCA_USER_TX_POOL_ID     HCI_ACL_POOL_ID
#endif

/*
GKI Buffer Pool ID used to hold MPS segments during SDU reassembly
*/
#ifndef MCA_FCR_RX_POOL_ID
#define MCA_FCR_RX_POOL_ID      HCI_ACL_POOL_ID
#endif

/*
GKI Buffer Pool ID used to hold MPS segments used in (re)transmissions.
L2CAP_DEFAULT_ERM_POOL_ID is specified to use the HCI ACL data pool.
Note:  This pool needs to have enough buffers to hold two times the window size negotiated
 in the tL2CAP_FCR_OPTIONS (2 * tx_win_size)  to allow for retransmissions.
 The size of each buffer must be able to hold the maximum MPS segment size passed in
 tL2CAP_FCR_OPTIONS plus BT_HDR (8) + HCI preamble (4) + L2CAP_MIN_OFFSET (11 - as of BT 2.1 + EDR Spec).
*/
#ifndef MCA_FCR_TX_POOL_ID
#define MCA_FCR_TX_POOL_ID      HCI_ACL_POOL_ID
#endif

/* MCAP control channel FCR Option:
Size of the transmission window when using enhanced retransmission mode.
1 is defined by HDP specification for control channel.
*/
#ifndef MCA_FCR_OPT_TX_WINDOW_SIZE
#define MCA_FCR_OPT_TX_WINDOW_SIZE      1
#endif

/* MCAP control channel FCR Option:
Number of transmission attempts for a single I-Frame before taking
Down the connection. Used In ERTM mode only. Value is Ignored in basic and
Streaming modes.
Range: 0, 1-0xFF
0 - infinite retransmissions
1 - single transmission
*/
#ifndef MCA_FCR_OPT_MAX_TX_B4_DISCNT
#define MCA_FCR_OPT_MAX_TX_B4_DISCNT    20
#endif

/* MCAP control channel FCR Option: Retransmission Timeout
The AVRCP specification set a value in the range of 300 - 2000 ms
Timeout (in msecs) to detect Lost I-Frames. Only used in Enhanced retransmission mode.
Range: Minimum 2000 (2 secs) when supporting PBF.
 */
#ifndef MCA_FCR_OPT_RETX_TOUT
#define MCA_FCR_OPT_RETX_TOUT           2000
#endif

/* MCAP control channel FCR Option: Monitor Timeout
The AVRCP specification set a value in the range of 300 - 2000 ms
Timeout (in msecs) to detect Lost S-Frames. Only used in Enhanced retransmission mode.
Range: Minimum 12000 (12 secs) when supporting PBF.
*/
#ifndef MCA_FCR_OPT_MONITOR_TOUT
#define MCA_FCR_OPT_MONITOR_TOUT        12000
#endif

/* MCAP control channel FCR Option: Maximum PDU payload size.
The maximum number of payload octets that the local device can receive in a single PDU.
*/
#ifndef MCA_FCR_OPT_MPS_SIZE
#define MCA_FCR_OPT_MPS_SIZE            1000
#endif

/* Shared transport */
#ifndef NFC_SHARED_TRANSPORT_ENABLED
#define NFC_SHARED_TRANSPORT_ENABLED    FALSE
#endif

/******************************************************************************
**
** SER
**
******************************************************************************/

#ifndef SER_INCLUDED
#define SER_INCLUDED                FALSE
#endif

/* Task which runs the serial application. */
#ifndef SER_TASK
#define SER_TASK                    BTE_APPL_TASK
#endif

/* Mailbox used by serial application. */
#ifndef SER_MBOX
#define SER_MBOX                    TASK_MBOX_1
#endif

/* Mailbox mask. */
#ifndef SER_MBOX_MASK
#define SER_MBOX_MASK               TASK_MBOX_1_EVT_MASK
#endif

/* TX path application event. */
#ifndef SER_TX_PATH_APPL_EVT
#define SER_TX_PATH_APPL_EVT        EVENT_MASK(APPL_EVT_3)
#endif

/* RX path application event. */
#ifndef SER_RX_PATH_APPL_EVT
#define SER_RX_PATH_APPL_EVT        EVENT_MASK(APPL_EVT_4)
#endif

/******************************************************************************
**
** Sleep Mode (Low Power Mode)
**
******************************************************************************/

#ifndef HCILP_INCLUDED
#define HCILP_INCLUDED                  TRUE
#endif

/******************************************************************************
**
** RPC
**
******************************************************************************/

#ifndef RPC_INCLUDED
#define RPC_INCLUDED                FALSE
#endif

/* RPCT task mailbox ID for messages coming from rpcgen code. */
#ifndef RPCT_MBOX
#define RPCT_MBOX                   TASK_MBOX_0
#endif

/* RPCT task event for mailbox. */
#ifndef RPCT_RPC_MBOX_EVT
#define RPCT_RPC_MBOX_EVT           TASK_MBOX_0_EVT_MASK
#endif

/* RPCT task event from driver indicating RX data is ready. */
#ifndef RPCT_RX_READY_EVT
#define RPCT_RX_READY_EVT           APPL_EVT_0
#endif

/* RPCT task event from driver indicating data TX is done. */
#ifndef RPCT_TX_DONE_EVT
#define RPCT_TX_DONE_EVT            APPL_EVT_1
#endif

/* RPCT task event indicating data is in the circular buffer. */
#ifndef RPCT_UCBUF_EVT
#define RPCT_UCBUF_EVT              APPL_EVT_2
#endif

/* Task ID of RPCGEN task. */
#ifndef RPCGEN_TASK
#define RPCGEN_TASK                 BTU_TASK
#endif

/* RPCGEN task event for messages coming from RPCT. */
#ifndef RPCGEN_MSG_EVT
#define RPCGEN_MSG_EVT              TASK_MBOX_1_EVT_MASK
#endif

#ifndef RPCGEN_MSG_MBOX
#define RPCGEN_MSG_MBOX             TASK_MBOX_1
#endif

/* Size of circular buffer used to store diagnostic messages. */
#ifndef RPCT_UCBUF_SIZE
#define RPCT_UCBUF_SIZE             2000
#endif

/******************************************************************************
**
** SAP - Sample applications
**
******************************************************************************/

#ifndef MMI_INCLUDED
#define MMI_INCLUDED                FALSE
#endif

/******************************************************************************
**
** APPL - Application Task
**
******************************************************************************/
/* When TRUE indicates that an application task is to be run */
#ifndef APPL_INCLUDED
#define APPL_INCLUDED                TRUE
#endif

/* When TRUE remote terminal code included (RPC MUST be included) */
#ifndef RSI_INCLUDED
#define RSI_INCLUDED                TRUE
#endif



#define L2CAP_FEATURE_REQ_ID      73
#define L2CAP_FEATURE_RSP_ID     173


#define L2CAP_ENHANCED_FEATURES   0


/******************************************************************************
**
** BTA
**
******************************************************************************/
/* BTA EIR canned UUID list (default is dynamic) */
#ifndef BTA_EIR_CANNED_UUID_LIST
#define BTA_EIR_CANNED_UUID_LIST FALSE
#endif

/* Number of supported customer UUID in EIR */
#ifndef BTA_EIR_SERVER_NUM_CUSTOM_UUID
#define BTA_EIR_SERVER_NUM_CUSTOM_UUID     8
#endif

/* CHLD override for bluedroid */
#ifndef BTA_AG_CHLD_VAL_ECC
#define BTA_AG_CHLD_VAL_ECC  "(0,1,1x,2,2x,3)"
#endif

#ifndef BTA_AG_CHLD_VAL
#define BTA_AG_CHLD_VAL  "(0,1,2,3)"
#endif

/* Set the CIND to match HFP 1.5 */
#ifndef BTA_AG_CIND_INFO
#define BTA_AG_CIND_INFO "(\"call\",(0,1)),(\"callsetup\",(0-3)),(\"service\",(0-1)),(\"signal\",(0-5)),(\"roam\",(0,1)),(\"battchg\",(0-5)),(\"callheld\",(0-2))"
#endif


/******************************************************************************
**
** BTE
**
******************************************************************************/
#ifndef BTE_PLATFORM_IDLE
#define BTE_PLATFORM_IDLE
#endif

#ifndef BTE_IDLE_TASK_INCLUDED
#define BTE_IDLE_TASK_INCLUDED FALSE
#endif

#ifndef BTE_PLATFORM_INITHW
#define BTE_PLATFORM_INITHW
#endif

#ifndef BTE_BTA_CODE_INCLUDED
#define BTE_BTA_CODE_INCLUDED FALSE
#endif

/******************************************************************************
**
** BTTRC
**
******************************************************************************/
/* Whether to parse and display traces-> Platform specific implementation */
#ifndef BTTRC_DISP
#define BTTRC_DISP        BTTRC_DispOnInsight
#endif

#endif /* BT_TARGET_H */
/* end "revolution/BTE/include/bt_target.h" */
#endif



/* "libs/RVL_SDK/include/revolution/bte/gki/common/gki.h" line 33 "revolution/BTE/stack/include/bt_types.h" */
//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef BT_TYPES_H
#define BT_TYPES_H

/* "libs/RVL_SDK/include/revolution/BTE/stack/include/bt_types.h" line 23 "revolution/BTE/gki/platform/data_types.h" */
/******************************************************************************
 *
 *  NOTICE OF CHANGES
 *  2024/03/25:
 *      - Move from ulinux/ to platform/
 *      - Add #include for RVL types (include/types.h)
 * 
 *  Compile with REVOLUTION defined to include these changes.
 * 
 ******************************************************************************/



//Modified by celestialamber

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

/* "libs/RVL_SDK/include/revolution/BTE/gki/platform/data_types.h" line 36 "types.h" */
/* end "types.h" */

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef signed   long   INT32;
typedef signed   char   INT8;
typedef signed   short  INT16;
typedef unsigned char   BOOLEAN;


typedef UINT32          TIME_STAMP;

#ifndef TRUE
#define TRUE   (!FALSE)
#endif

typedef unsigned char   UBYTE;

#ifdef __arm
#define PACKED  __packed
#define INLINE  __inline
#else
#define PACKED
#define INLINE
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN FALSE
#endif

#define UINT16_LOW_BYTE(x)      ((x) & 0xff)
#define UINT16_HI_BYTE(x)       ((x) >> 8)


#define BCM_STRCAT_S(x1,x2,x3)      strcat((x1),(x3))
#define BCM_STRNCAT_S(x1,x2,x3,x4)  strncat((x1),(x3),(x4))
#define BCM_STRCPY_S(x1,x2,x3)      strcpy((x1),(x3))
#define BCM_STRNCPY_S(x1,x2,x3,x4)  strncpy((x1),(x3),(x4))



#endif
/* end "revolution/BTE/gki/platform/data_types.h" */

#ifdef _WIN32
#ifdef BLUESTACK_TESTER
/* "libs/RVL_SDK/include/revolution/BTE/stack/include/bt_types.h" line 27 "bte_stack_entry.h" */
/* end "bte_stack_entry.h" */
#endif
#endif

/* READ WELL !!
**
** This section defines global events. These are events that cross layers.
** Any event that passes between layers MUST be one of these events. Tasks
** can use their own events internally, but a FUNDAMENTAL design issue is
** that global events MUST be one of these events defined below.
**
** The convention used is the the event name contains the layer that the
** event is going to.
*/
#define BT_EVT_MASK                 0xFF00
#define BT_SUB_EVT_MASK             0x00FF
                                                /* To Bluetooth Upper Layers        */
                                                /************************************/
#define BT_EVT_TO_BTU_L2C_EVT       0x0900      /* L2CAP event */
#define BT_EVT_TO_BTU_HCI_EVT       0x1000      /* HCI Event                        */
#define BT_EVT_TO_BTU_HCI_BR_EDR_EVT (0x0000 | BT_EVT_TO_BTU_HCI_EVT)      /* event from BR/EDR controller */
#define BT_EVT_TO_BTU_HCI_AMP1_EVT   (0x0001 | BT_EVT_TO_BTU_HCI_EVT)      /* event from local AMP 1 controller */
#define BT_EVT_TO_BTU_HCI_AMP2_EVT   (0x0002 | BT_EVT_TO_BTU_HCI_EVT)      /* event from local AMP 2 controller */
#define BT_EVT_TO_BTU_HCI_AMP3_EVT   (0x0003 | BT_EVT_TO_BTU_HCI_EVT)      /* event from local AMP 3 controller */

#define BT_EVT_TO_BTU_HCI_ACL       0x1100      /* ACL Data from HCI                */
#define BT_EVT_TO_BTU_HCI_SCO       0x1200      /* SCO Data from HCI                */
#define BT_EVT_TO_BTU_HCIT_ERR      0x1300      /* HCI Transport Error              */

#define BT_EVT_TO_BTU_SP_EVT        0x1400      /* Serial Port Event                */
#define BT_EVT_TO_BTU_SP_DATA       0x1500      /* Serial Port Data                 */

#define BT_EVT_TO_BTU_HCI_CMD       0x1600      /* HCI command from upper layer     */


#define BT_EVT_TO_BTU_L2C_SEG_XMIT  0x1900      /* L2CAP segment(s) transmitted     */

#define BT_EVT_PROXY_INCOMING_MSG   0x1A00      /* BlueStackTester event: incoming message from target */

#define BT_EVT_BTSIM                0x1B00      /* Insight BTSIM event */
#define BT_EVT_BTISE                0x1C00      /* Insight Script Engine event */

                                                /* To LM                            */
                                                /************************************/
#define BT_EVT_TO_LM_HCI_CMD        0x2000      /* HCI Command                      */
#define BT_EVT_TO_LM_HCI_ACL        0x2100      /* HCI ACL Data                     */
#define BT_EVT_TO_LM_HCI_SCO        0x2200      /* HCI SCO Data                     */
#define BT_EVT_TO_LM_HCIT_ERR       0x2300      /* HCI Transport Error              */
#define BT_EVT_TO_LM_LC_EVT         0x2400      /* LC event                         */
#define BT_EVT_TO_LM_LC_LMP         0x2500      /* LC Received LMP command frame    */
#define BT_EVT_TO_LM_LC_ACL         0x2600      /* LC Received ACL data             */
#define BT_EVT_TO_LM_LC_SCO         0x2700      /* LC Received SCO data  (not used) */
#define BT_EVT_TO_LM_LC_ACL_TX      0x2800      /* LMP data transmit complete       */
#define BT_EVT_TO_LM_LC_LMPC_TX     0x2900      /* LMP Command transmit complete    */
#define BT_EVT_TO_LM_LOCAL_ACL_LB   0x2a00      /* Data to be locally loopbacked    */
#define BT_EVT_TO_LM_HCI_ACL_ACK    0x2b00      /* HCI ACL Data ack      (not used) */
#define BT_EVT_TO_LM_DIAG           0x2c00      /* LM Diagnostics commands          */


#define BT_EVT_TO_BTM_CMDS          0x2f00
#define BT_EVT_TO_BTM_PM_MDCHG_EVT (0x0001 | BT_EVT_TO_BTM_CMDS)

#define BT_EVT_TO_TCS_CMDS          0x3000

#define BT_EVT_TO_OBX_CL_MSG        0x3100
#define BT_EVT_TO_OBX_SR_MSG        0x3200

#define BT_EVT_TO_CTP_CMDS          0x3300

/* Obex Over L2CAP */
#define BT_EVT_TO_OBX_CL_L2C_MSG    0x3400
#define BT_EVT_TO_OBX_SR_L2C_MSG    0x3500

/* ftp events */
#define BT_EVT_TO_FTP_SRVR_CMDS     0x3800
#define BT_EVT_TO_FTP_CLNT_CMDS     0x3900

#define BT_EVT_TO_BTU_SAP           0x3a00       /* SIM Access Profile events */

/* opp events */
#define BT_EVT_TO_OPP_SRVR_CMDS     0x3b00
#define BT_EVT_TO_OPP_CLNT_CMDS     0x3c00

/* gap events */
#define BT_EVT_TO_GAP_MSG           0x3d00

/* start timer */
#define BT_EVT_TO_START_TIMER       0x3e00

/* start quick timer */
#define BT_EVT_TO_START_QUICK_TIMER 0x3f00


/* for NFC                          */
                                                /************************************/
#define BT_EVT_TO_NFC_NCI           0x4000      /* NCI Command, Notification or Data*/
#define BT_EVT_TO_NFC_INIT          0x4100      /* Initialization message */
#define BT_EVT_TO_LLCP_ECHO         0x4200      /* LLCP Echo Service */
#define BT_EVT_TO_LLCP_SOCKET       0x4300      /* LLCP over TCP/IP */
#define BT_EVT_TO_NCI_LP            0x4400      /* Low power */
#define BT_EVT_TO_NFC_ERR           0x4500      /* Error notification to NFC Task */

#define BT_EVT_TO_NFCCSIM_NCI       0x4a00      /* events to NFCC simulation (NCI packets) */

/* HCISU Events */

#define BT_EVT_HCISU                0x5000

// btla-specific ++
#define BT_EVT_TO_HCISU_RECONFIG_EVT            (0x0001 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_UPDATE_BAUDRATE_EVT     (0x0002 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_ENABLE_EVT           (0x0003 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_DISABLE_EVT          (0x0004 | BT_EVT_HCISU)
// btla-specific --
#define BT_EVT_TO_HCISU_LP_APP_SLEEPING_EVT     (0x0005 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_ALLOW_BT_SLEEP_EVT   (0x0006 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_WAKEUP_HOST_EVT      (0x0007 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_LP_RCV_H4IBSS_EVT       (0x0008 | BT_EVT_HCISU)
#define BT_EVT_TO_HCISU_H5_RESET_EVT            (0x0009 | BT_EVT_HCISU)
#define BT_EVT_HCISU_START_QUICK_TIMER          (0x000a | BT_EVT_HCISU)

#define BT_EVT_DATA_TO_AMP_1        0x5100
#define BT_EVT_DATA_TO_AMP_15       0x5f00

/* HSP Events */

#define BT_EVT_BTU_HSP2             0x6000

#define BT_EVT_TO_BTU_HSP2_EVT     (0x0001 | BT_EVT_BTU_HSP2)

/* BPP Events */
#define BT_EVT_TO_BPP_PR_CMDS       0x6100      /* Printer Events */
#define BT_EVT_TO_BPP_SND_CMDS      0x6200      /* BPP Sender Events */

/* BIP Events */
#define BT_EVT_TO_BIP_CMDS          0x6300

/* HCRP Events */

#define BT_EVT_BTU_HCRP             0x7000

#define BT_EVT_TO_BTU_HCRP_EVT     (0x0001 | BT_EVT_BTU_HCRP)
#define BT_EVT_TO_BTU_HCRPM_EVT    (0x0002 | BT_EVT_BTU_HCRP)


#define BT_EVT_BTU_HFP              0x8000
#define BT_EVT_TO_BTU_HFP_EVT      (0x0001 | BT_EVT_BTU_HFP)

#define BT_EVT_BTU_IPC_EVT          0x9000
#define BT_EVT_BTU_IPC_LOGMSG_EVT  (0x0000 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_ACL_EVT     (0x0001 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BTU_EVT     (0x0002 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_L2C_EVT     (0x0003 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_L2C_MSG_EVT (0x0004 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BTM_EVT     (0x0005 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_AVDT_EVT    (0x0006 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_SLIP_EVT    (0x0007 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_MGMT_EVT    (0x0008 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BTTRC_EVT   (0x0009 | BT_EVT_BTU_IPC_EVT)
#define BT_EVT_BTU_IPC_BURST_EVT   (0x000A | BT_EVT_BTU_IPC_EVT)


/* BTIF Events */
#define BT_EVT_BTIF                 0xA000
#define BT_EVT_CONTEXT_SWITCH_EVT  (0x0001 | BT_EVT_BTIF)

#define BT_EVT_TRIGGER_STACK_INIT   EVENT_MASK(APPL_EVT_0)


/* Define the header of each buffer used in the Bluetooth stack.
*/
typedef struct
{
    UINT16          event;
    UINT16          len;
    UINT16          offset;
    UINT16          layer_specific;
} BT_HDR;

#define BT_HDR_SIZE (sizeof (BT_HDR))

#define BT_PSM_SDP                      0x0001
#define BT_PSM_RFCOMM                   0x0003
#define BT_PSM_TCS                      0x0005
#define BT_PSM_CTP                      0x0007
#define BT_PSM_BNEP                     0x000F
#define BT_PSM_HIDC                     0x0011
#define BT_PSM_HIDI                     0x0013
#define BT_PSM_UPNP                     0x0015
#define BT_PSM_AVCTP                    0x0017
#define BT_PSM_AVDTP                    0x0019
#define BT_PSM_AVCTP_13                 0x001B /* Advanced Control - Browsing */
#define BT_PSM_UDI_CP                   0x001D /* Unrestricted Digital Information Profile C-Plane  */
#define BT_PSM_ATT                      0x001F /* Attribute Protocol  */


/* These macros extract the HCI opcodes from a buffer
*/
#define HCI_GET_CMD_HDR_OPCODE(p)    (UINT16)((*((UINT8 *)((p) + 1) + p->offset) + \
                                              (*((UINT8 *)((p) + 1) + p->offset + 1) << 8)))
#define HCI_GET_CMD_HDR_PARAM_LEN(p) (UINT8)  (*((UINT8 *)((p) + 1) + p->offset + 2))

#define HCI_GET_EVT_HDR_OPCODE(p)    (UINT8)(*((UINT8 *)((p) + 1) + p->offset))
#define HCI_GET_EVT_HDR_PARAM_LEN(p) (UINT8)  (*((UINT8 *)((p) + 1) + p->offset + 1))


/********************************************************************************
** Macros to get and put bytes to and from a stream (Little Endian format).
*/
#define UINT32_TO_STREAM(p, u32) {*(p)++ = (UINT8)(u32); *(p)++ = (UINT8)((u32) >> 8); *(p)++ = (UINT8)((u32) >> 16); *(p)++ = (UINT8)((u32) >> 24);}
#define UINT24_TO_STREAM(p, u24) {*(p)++ = (UINT8)(u24); *(p)++ = (UINT8)((u24) >> 8); *(p)++ = (UINT8)((u24) >> 16);}
#define UINT16_TO_STREAM(p, u16) {*(p)++ = (UINT8)(u16); *(p)++ = (UINT8)((u16) >> 8);}
#define UINT8_TO_STREAM(p, u8)   {*(p)++ = (UINT8)(u8);}
#define INT8_TO_STREAM(p, u8)    {*(p)++ = (INT8)(u8);}
#define ARRAY32_TO_STREAM(p, a)  {register int ijk; for (ijk = 0; ijk < 32;           ijk++) *(p)++ = (UINT8) a[31 - ijk];}
#define ARRAY16_TO_STREAM(p, a)  {register int ijk; for (ijk = 0; ijk < 16;           ijk++) *(p)++ = (UINT8) a[15 - ijk];}
#define ARRAY8_TO_STREAM(p, a)   {register int ijk; for (ijk = 0; ijk < 8;            ijk++) *(p)++ = (UINT8) a[7 - ijk];}
#define BDADDR_TO_STREAM(p, a)   {register int ijk; for (ijk = 0; ijk < BD_ADDR_LEN;  ijk++) *(p)++ = (UINT8) a[BD_ADDR_LEN - 1 - ijk];}
#define LAP_TO_STREAM(p, a)      {register int ijk; for (ijk = 0; ijk < LAP_LEN;      ijk++) *(p)++ = (UINT8) a[LAP_LEN - 1 - ijk];}
#define DEVCLASS_TO_STREAM(p, a) {register int ijk; for (ijk = 0; ijk < DEV_CLASS_LEN;ijk++) *(p)++ = (UINT8) a[DEV_CLASS_LEN - 1 - ijk];}
#define ARRAY_TO_STREAM(p, a, len) {register int ijk; for (ijk = 0; ijk < len;        ijk++) *(p)++ = (UINT8) a[ijk];}
#define REVERSE_ARRAY_TO_STREAM(p, a, len)  {register int ijk; for (ijk = 0; ijk < len; ijk++) *(p)++ = (UINT8) a[len - 1 - ijk];}

#define STREAM_TO_UINT8(u8, p)   {u8 = (UINT8)(*(p)); (p) += 1;}
#define STREAM_TO_UINT16(u16, p) {u16 = ((UINT16)(*(p)) + (((UINT16)(*((p) + 1))) << 8)); (p) += 2;}
#define STREAM_TO_UINT24(u32, p) {u32 = (((UINT32)(*(p))) + ((((UINT32)(*((p) + 1)))) << 8) + ((((UINT32)(*((p) + 2)))) << 16) ); (p) += 3;}
#define STREAM_TO_UINT32(u32, p) {u32 = (((UINT32)(*(p))) + ((((UINT32)(*((p) + 1)))) << 8) + ((((UINT32)(*((p) + 2)))) << 16) + ((((UINT32)(*((p) + 3)))) << 24)); (p) += 4;}
#define STREAM_TO_BDADDR(a, p)   {register int ijk; register UINT8 *pbda = (UINT8 *)a + BD_ADDR_LEN - 1; for (ijk = 0; ijk < BD_ADDR_LEN; ijk++) *pbda-- = *p++;}
#define STREAM_TO_ARRAY32(a, p)  {register int ijk; register UINT8 *_pa = (UINT8 *)a + 31; for (ijk = 0; ijk < 32; ijk++) *_pa-- = *p++;}
#define STREAM_TO_ARRAY16(a, p)  {register int ijk; register UINT8 *_pa = (UINT8 *)a + 15; for (ijk = 0; ijk < 16; ijk++) *_pa-- = *p++;}
#define STREAM_TO_ARRAY8(a, p)   {register int ijk; register UINT8 *_pa = (UINT8 *)a + 7; for (ijk = 0; ijk < 8; ijk++) *_pa-- = *p++;}
#define STREAM_TO_DEVCLASS(a, p) {register int ijk; register UINT8 *_pa = (UINT8 *)a + DEV_CLASS_LEN - 1; for (ijk = 0; ijk < DEV_CLASS_LEN; ijk++) *_pa-- = *p++;}
#define STREAM_TO_LAP(a, p)      {register int ijk; register UINT8 *plap = (UINT8 *)a + LAP_LEN - 1; for (ijk = 0; ijk < LAP_LEN; ijk++) *plap-- = *p++;}
#define STREAM_TO_ARRAY(a, p, len) {register int ijk; for (ijk = 0; ijk < len; ijk++) ((UINT8 *) a)[ijk] = *p++;}
#define REVERSE_STREAM_TO_ARRAY(a, p, len) {register int ijk; register UINT8 *_pa = (UINT8 *)a + len - 1; for (ijk = 0; ijk < len; ijk++) *_pa-- = *p++;}

/********************************************************************************
** Macros to get and put bytes to and from a field (Little Endian format).
** These are the same as to stream, except the pointer is not incremented.
*/
#define UINT32_TO_FIELD(p, u32) {*(UINT8 *)(p) = (UINT8)(u32); *((UINT8 *)(p)+1) = (UINT8)((u32) >> 8); *((UINT8 *)(p)+2) = (UINT8)((u32) >> 16); *((UINT8 *)(p)+3) = (UINT8)((u32) >> 24);}
#define UINT24_TO_FIELD(p, u24) {*(UINT8 *)(p) = (UINT8)(u24); *((UINT8 *)(p)+1) = (UINT8)((u24) >> 8); *((UINT8 *)(p)+2) = (UINT8)((u24) >> 16);}
#define UINT16_TO_FIELD(p, u16) {*(UINT8 *)(p) = (UINT8)(u16); *((UINT8 *)(p)+1) = (UINT8)((u16) >> 8);}
#define UINT8_TO_FIELD(p, u8)   {*(UINT8 *)(p) = (UINT8)(u8);}


/********************************************************************************
** Macros to get and put bytes to and from a stream (Big Endian format)
*/
#define UINT32_TO_BE_STREAM(p, u32) {*(p)++ = (UINT8)((u32) >> 24);  *(p)++ = (UINT8)((u32) >> 16); *(p)++ = (UINT8)((u32) >> 8); *(p)++ = (UINT8)(u32); }
#define UINT24_TO_BE_STREAM(p, u24) {*(p)++ = (UINT8)((u24) >> 16); *(p)++ = (UINT8)((u24) >> 8); *(p)++ = (UINT8)(u24);}
#define UINT16_TO_BE_STREAM(p, u16) {*(p)++ = (UINT8)((u16) >> 8); *(p)++ = (UINT8)(u16);}
#define UINT8_TO_BE_STREAM(p, u8)   {*(p)++ = (UINT8)(u8);}
#define ARRAY_TO_BE_STREAM(p, a, len) {register int ijk; for (ijk = 0; ijk < len; ijk++) *(p)++ = (UINT8) a[ijk];}

#define BE_STREAM_TO_UINT8(u8, p)   {u8 = (UINT8)(*(p)); (p) += 1;}
#define BE_STREAM_TO_UINT16(u16, p) {u16 = (UINT16)(((UINT16)(*(p)) << 8) + (UINT16)(*((p) + 1))); (p) += 2;}
#define BE_STREAM_TO_UINT24(u32, p) {u32 = (((UINT32)(*((p) + 2))) + ((UINT32)(*((p) + 1)) << 8) + ((UINT32)(*(p)) << 16)); (p) += 3;}
#define BE_STREAM_TO_UINT32(u32, p) {u32 = ((UINT32)(*((p) + 3)) + ((UINT32)(*((p) + 2)) << 8) + ((UINT32)(*((p) + 1)) << 16) + ((UINT32)(*(p)) << 24)); (p) += 4;}
#define BE_STREAM_TO_ARRAY(p, a, len) {register int ijk; for (ijk = 0; ijk < len; ijk++) ((UINT8 *) a)[ijk] = *p++;}


/********************************************************************************
** Macros to get and put bytes to and from a field (Big Endian format).
** These are the same as to stream, except the pointer is not incremented.
*/
#define UINT32_TO_BE_FIELD(p, u32) {*(UINT8 *)(p) = (UINT8)((u32) >> 24);  *((UINT8 *)(p)+1) = (UINT8)((u32) >> 16); *((UINT8 *)(p)+2) = (UINT8)((u32) >> 8); *((UINT8 *)(p)+3) = (UINT8)(u32); }
#define UINT24_TO_BE_FIELD(p, u24) {*(UINT8 *)(p) = (UINT8)((u24) >> 16); *((UINT8 *)(p)+1) = (UINT8)((u24) >> 8); *((UINT8 *)(p)+2) = (UINT8)(u24);}
#define UINT16_TO_BE_FIELD(p, u16) {*(UINT8 *)(p) = (UINT8)((u16) >> 8); *((UINT8 *)(p)+1) = (UINT8)(u16);}
#define UINT8_TO_BE_FIELD(p, u8)   {*(UINT8 *)(p) = (UINT8)(u8);}


/* Common Bluetooth field definitions */
#define BD_ADDR_LEN     6                   /* Device address length */
typedef UINT8 BD_ADDR[BD_ADDR_LEN];         /* Device address */
typedef UINT8 *BD_ADDR_PTR;                 /* Pointer to Device Address */

#define AMP_KEY_TYPE_GAMP       0
#define AMP_KEY_TYPE_WIFI       1
#define AMP_KEY_TYPE_UWB        2
typedef UINT8 tAMP_KEY_TYPE;

#define BT_OCTET8_LEN    8
typedef UINT8 BT_OCTET8[BT_OCTET8_LEN];   /* octet array: size 16 */

#define LINK_KEY_LEN    16
typedef UINT8 LINK_KEY[LINK_KEY_LEN];       /* Link Key */

#define AMP_LINK_KEY_LEN        32
typedef UINT8 AMP_LINK_KEY[AMP_LINK_KEY_LEN];   /* Dedicated AMP and GAMP Link Keys */

#define BT_OCTET16_LEN    16
typedef UINT8 BT_OCTET16[BT_OCTET16_LEN];   /* octet array: size 16 */

#define PIN_CODE_LEN    16
typedef UINT8 PIN_CODE[PIN_CODE_LEN];       /* Pin Code (upto 128 bits) MSB is 0 */
typedef UINT8 *PIN_CODE_PTR;                /* Pointer to Pin Code */

#define DEV_CLASS_LEN   3
typedef UINT8 DEV_CLASS[DEV_CLASS_LEN];     /* Device class */
typedef UINT8 *DEV_CLASS_PTR;               /* Pointer to Device class */

#define EXT_INQ_RESP_LEN   3
typedef UINT8 EXT_INQ_RESP[EXT_INQ_RESP_LEN];/* Extended Inquiry Response */
typedef UINT8 *EXT_INQ_RESP_PTR;             /* Pointer to Extended Inquiry Response */

#define BD_NAME_LEN     248
typedef UINT8 BD_NAME[BD_NAME_LEN];         /* Device name */
typedef UINT8 *BD_NAME_PTR;                 /* Pointer to Device name */

#define BD_FEATURES_LEN 8
typedef UINT8 BD_FEATURES[BD_FEATURES_LEN]; /* LMP features supported by device */

#define BT_EVENT_MASK_LEN  8
typedef UINT8 BT_EVENT_MASK[BT_EVENT_MASK_LEN];   /* Event Mask */

#define LAP_LEN         3
typedef UINT8 LAP[LAP_LEN];                 /* IAC as passed to Inquiry (LAP) */
typedef UINT8 INQ_LAP[LAP_LEN];             /* IAC as passed to Inquiry (LAP) */

#define RAND_NUM_LEN    16
typedef UINT8 RAND_NUM[RAND_NUM_LEN];

#define ACO_LEN         12
typedef UINT8 ACO[ACO_LEN];                 /* Authenticated ciphering offset */

#define COF_LEN         12
typedef UINT8 COF[COF_LEN];                 /* ciphering offset number */

typedef struct {
    UINT8               qos_flags;          /* TBD */
    UINT8               service_type;       /* see below */
    UINT32              token_rate;         /* bytes/second */
    UINT32              token_bucket_size;  /* bytes */
    UINT32              peak_bandwidth;     /* bytes/second */
    UINT32              latency;            /* microseconds */
    UINT32              delay_variation;    /* microseconds */
} FLOW_SPEC;

/* Values for service_type */
#define NO_TRAFFIC      0
#define BEST_EFFORT     1
#define GUARANTEED      2

/* Service class of the CoD */
#define SERV_CLASS_NETWORKING               (1 << 1)
#define SERV_CLASS_RENDERING                (1 << 2)
#define SERV_CLASS_CAPTURING                (1 << 3)
#define SERV_CLASS_OBJECT_TRANSFER          (1 << 4)
#define SERV_CLASS_OBJECT_AUDIO             (1 << 5)
#define SERV_CLASS_OBJECT_TELEPHONY         (1 << 6)
#define SERV_CLASS_OBJECT_INFORMATION       (1 << 7)

/* Second byte */
#define SERV_CLASS_LIMITED_DISC_MODE        (0x20)

/* Field size definitions. Note that byte lengths are rounded up. */
#define ACCESS_CODE_BIT_LEN             72
#define ACCESS_CODE_BYTE_LEN            9
#define SHORTENED_ACCESS_CODE_BIT_LEN   68

typedef UINT8 ACCESS_CODE[ACCESS_CODE_BYTE_LEN];

#define SYNTH_TX                1           /* want synth code to TRANSMIT at this freq */
#define SYNTH_RX                2           /* want synth code to RECEIVE at this freq */

#define SYNC_REPS 1             /* repeats of sync word transmitted to start of burst */

/* Bluetooth CLK27 */
#define BT_CLK27            (2 << 26)

/* Bluetooth CLK12 is 1.28 sec */
#define BT_CLK12_TO_MS(x)    ((x) * 1280)
#define BT_MS_TO_CLK12(x)    ((x) / 1280)
#define BT_CLK12_TO_SLOTS(x) ((x) << 11)

/* Bluetooth CLK is 0.625 msec */
#define BT_CLK_TO_MS(x)      (((x) * 5 + 3) / 8)
#define BT_MS_TO_CLK(x)      (((x) * 8 + 2) / 5)

#define BT_CLK_TO_MICROSECS(x)  (((x) * 5000 + 3) / 8)
#define BT_MICROSECS_TO_CLK(x)  (((x) * 8 + 2499) / 5000)

/* Maximum UUID size - 16 bytes, and structure to hold any type of UUID. */
#define MAX_UUID_SIZE              16
typedef struct
{
#define LEN_UUID_16     2
#define LEN_UUID_32     4
#define LEN_UUID_128    16

    UINT16          len;

    union
    {
        UINT16      uuid16;
        UINT32      uuid32;
        UINT8       uuid128[MAX_UUID_SIZE];
    } uu;

} tBT_UUID;

#define BT_EIR_FLAGS_TYPE                   0x01
#define BT_EIR_MORE_16BITS_UUID_TYPE        0x02
#define BT_EIR_COMPLETE_16BITS_UUID_TYPE    0x03
#define BT_EIR_MORE_32BITS_UUID_TYPE        0x04
#define BT_EIR_COMPLETE_32BITS_UUID_TYPE    0x05
#define BT_EIR_MORE_128BITS_UUID_TYPE       0x06
#define BT_EIR_COMPLETE_128BITS_UUID_TYPE   0x07
#define BT_EIR_SHORTENED_LOCAL_NAME_TYPE    0x08
#define BT_EIR_COMPLETE_LOCAL_NAME_TYPE     0x09
#define BT_EIR_TX_POWER_LEVEL_TYPE          0x0A
#define BT_EIR_OOB_BD_ADDR_TYPE             0x0C
#define BT_EIR_OOB_COD_TYPE                 0x0D
#define BT_EIR_OOB_SSP_HASH_C_TYPE          0x0E
#define BT_EIR_OOB_SSP_RAND_R_TYPE          0x0F
#define BT_EIR_MANUFACTURER_SPECIFIC_TYPE   0xFF

#define BT_OOB_COD_SIZE            3
#define BT_OOB_HASH_C_SIZE         16
#define BT_OOB_RAND_R_SIZE         16

/* Broadcom proprietary UUIDs and reserved PSMs
**
** The lowest 4 bytes byte of the UUID or GUID depends on the feature. Typically,
** the value of those bytes will be the PSM or SCN, but it is up to the features.
*/
#define BRCM_PROPRIETARY_UUID_BASE  0xDA, 0x23, 0x41, 0x02, 0xA3, 0xBB, 0xC1, 0x71, 0xBA, 0x09, 0x6f, 0x21
#define BRCM_PROPRIETARY_GUID_BASE  0xda23, 0x4102, 0xa3, 0xbb, 0xc1, 0x71, 0xba, 0x09, 0x6f, 0x21

/* We will not allocate a PSM in the reserved range to 3rd party apps
*/
#define BRCM_RESERVED_PSM_START	    0x5AE1
#define BRCM_RESERVED_PSM_END	    0x5AFF

#define BRCM_UTILITY_SERVICE_PSM    0x5AE1
#define BRCM_MATCHER_PSM            0x5AE3

/* Connection statistics
*/

/* Structure to hold connection stats */
#ifndef BT_CONN_STATS_DEFINED
#define BT_CONN_STATS_DEFINED

/* These bits are used in the bIsConnected field */
#define BT_CONNECTED_USING_BREDR   1
#define BT_CONNECTED_USING_AMP     2

typedef struct
{
    UINT32   is_connected;
    INT32    rssi;
    UINT32   bytes_sent;
    UINT32   bytes_rcvd;
    UINT32   duration;
} tBT_CONN_STATS;

#endif


/*****************************************************************************
**                          Low Energy definitions
**
** Address types
*/
#define BLE_ADDR_PUBLIC         0x00
#define BLE_ADDR_RANDOM         0x01
#define BLE_ADDR_TYPE_MASK      (BLE_ADDR_RANDOM | BLE_ADDR_PUBLIC)
typedef UINT8 tBLE_ADDR_TYPE;

#define BLE_ADDR_IS_STATIC(x)   ((x[0] & 0xC0) == 0xC0)

typedef struct
{
    tBLE_ADDR_TYPE      type;
    BD_ADDR             bda;
} tBLE_BD_ADDR;

/* Device Types
*/
#define BT_DEVICE_TYPE_BREDR   0x01
#define BT_DEVICE_TYPE_BLE     0x02
#define BT_DEVICE_TYPE_DUMO    0x03
typedef UINT8 tBT_DEVICE_TYPE;
/*****************************************************************************/


/* Define trace levels */
#define BT_TRACE_LEVEL_NONE    0          /* No trace messages to be generated    */
#define BT_TRACE_LEVEL_ERROR   1          /* Error condition trace messages       */
#define BT_TRACE_LEVEL_WARNING 2          /* Warning condition trace messages     */
#define BT_TRACE_LEVEL_API     3          /* API traces                           */
#define BT_TRACE_LEVEL_EVENT   4          /* Debug messages for events            */
#define BT_TRACE_LEVEL_DEBUG   5          /* Full debug messages                  */
#define BT_TRACE_LEVEL_VERBOSE 6          /* Verbose debug messages               */

#define MAX_TRACE_LEVEL        6


/* Define New Trace Type Definition */
/* TRACE_CTRL_TYPE                  0x^^000000*/
#define TRACE_CTRL_MASK             0xff000000
#define TRACE_GET_CTRL(x)           ((((UINT32)(x)) & TRACE_CTRL_MASK) >> 24)

#define TRACE_CTRL_GENERAL          0x00000000
#define TRACE_CTRL_STR_RESOURCE     0x01000000
#define TRACE_CTRL_SEQ_FLOW         0x02000000
#define TRACE_CTRL_MAX_NUM          3

/* LAYER SPECIFIC                   0x00^^0000*/
#define TRACE_LAYER_MASK            0x00ff0000
#define TRACE_GET_LAYER(x)          ((((UINT32)(x)) & TRACE_LAYER_MASK) >> 16)

#define TRACE_LAYER_NONE            0x00000000
#define TRACE_LAYER_USB             0x00010000
#define TRACE_LAYER_SERIAL          0x00020000
#define TRACE_LAYER_SOCKET          0x00030000
#define TRACE_LAYER_RS232           0x00040000
#define TRACE_LAYER_TRANS_MAX_NUM   5
#define TRACE_LAYER_TRANS_ALL       0x007f0000
#define TRACE_LAYER_LC              0x00050000
#define TRACE_LAYER_LM              0x00060000
#define TRACE_LAYER_HCI             0x00070000
#define TRACE_LAYER_L2CAP           0x00080000
#define TRACE_LAYER_RFCOMM          0x00090000
#define TRACE_LAYER_SDP             0x000a0000
#define TRACE_LAYER_TCS             0x000b0000
#define TRACE_LAYER_OBEX            0x000c0000
#define TRACE_LAYER_BTM             0x000d0000
#define TRACE_LAYER_GAP             0x000e0000
#define TRACE_LAYER_DUN             0x000f0000
#define TRACE_LAYER_GOEP            0x00100000
#define TRACE_LAYER_ICP             0x00110000
#define TRACE_LAYER_HSP2            0x00120000
#define TRACE_LAYER_SPP             0x00130000
#define TRACE_LAYER_CTP             0x00140000
#define TRACE_LAYER_BPP             0x00150000
#define TRACE_LAYER_HCRP            0x00160000
#define TRACE_LAYER_FTP             0x00170000
#define TRACE_LAYER_OPP             0x00180000
#define TRACE_LAYER_BTU             0x00190000
#define TRACE_LAYER_GKI             0x001a0000
#define TRACE_LAYER_BNEP            0x001b0000
#define TRACE_LAYER_PAN             0x001c0000
#define TRACE_LAYER_HFP             0x001d0000
#define TRACE_LAYER_HID             0x001e0000
#define TRACE_LAYER_BIP             0x001f0000
#define TRACE_LAYER_AVP             0x00200000
#define TRACE_LAYER_A2D             0x00210000
#define TRACE_LAYER_SAP             0x00220000
#define TRACE_LAYER_AMP             0x00230000
#define TRACE_LAYER_MCA             0x00240000
#define TRACE_LAYER_ATT             0x00250000
#define TRACE_LAYER_SMP             0x00260000
#define TRACE_LAYER_NFC             0x00270000
#define TRACE_LAYER_NCI             0x00280000
#define TRACE_LAYER_IDEP            0x00290000
#define TRACE_LAYER_NDEP            0x002a0000
#define TRACE_LAYER_LLCP            0x002b0000
#define TRACE_LAYER_RW              0x002c0000
#define TRACE_LAYER_CE              0x002d0000
#define TRACE_LAYER_SNEP            0x002e0000
#define TRACE_LAYER_NDEF            0x002f0000
#define TRACE_LAYER_NFA             0x00300000

#define TRACE_LAYER_MAX_NUM         0x0031


/* TRACE_ORIGINATOR                 0x0000^^00*/
#define TRACE_ORG_MASK              0x0000ff00
#define TRACE_GET_ORG(x)            ((((UINT32)(x)) & TRACE_ORG_MASK) >> 8)

#define TRACE_ORG_STACK             0x00000000
#define TRACE_ORG_HCI_TRANS         0x00000100
#define TRACE_ORG_PROTO_DISP        0x00000200
#define TRACE_ORG_RPC               0x00000300
#define TRACE_ORG_GKI               0x00000400
#define TRACE_ORG_APPL              0x00000500
#define TRACE_ORG_SCR_WRAPPER       0x00000600
#define TRACE_ORG_SCR_ENGINE        0x00000700
#define TRACE_ORG_USER_SCR          0x00000800
#define TRACE_ORG_TESTER            0x00000900
#define TRACE_ORG_MAX_NUM           10          /* 32-bit mask; must be < 32 */
#define TRACE_LITE_ORG_MAX_NUM		6
#define TRACE_ORG_ALL               0x03ff
#define TRACE_ORG_RPC_TRANS         0x04

#define TRACE_ORG_REG               0x00000909
#define TRACE_ORG_REG_SUCCESS       0x0000090a

/* TRACE_TYPE                       0x000000^^*/
#define TRACE_TYPE_MASK             0x000000ff
#define TRACE_GET_TYPE(x)           (((UINT32)(x)) & TRACE_TYPE_MASK)

#define TRACE_TYPE_ERROR            0x00000000
#define TRACE_TYPE_WARNING          0x00000001
#define TRACE_TYPE_API              0x00000002
#define TRACE_TYPE_EVENT            0x00000003
#define TRACE_TYPE_DEBUG            0x00000004
#define TRACE_TYPE_STACK_ONLY_MAX   TRACE_TYPE_DEBUG
#define TRACE_TYPE_TX               0x00000005
#define TRACE_TYPE_RX               0x00000006
#define TRACE_TYPE_DEBUG_ASSERT     0x00000007
#define TRACE_TYPE_GENERIC          0x00000008
#define TRACE_TYPE_REG              0x00000009
#define TRACE_TYPE_REG_SUCCESS      0x0000000a
#define TRACE_TYPE_CMD_TX           0x0000000b
#define TRACE_TYPE_EVT_TX           0x0000000c
#define TRACE_TYPE_ACL_TX           0x0000000d
#define TRACE_TYPE_CMD_RX           0x0000000e
#define TRACE_TYPE_EVT_RX           0x0000000f
#define TRACE_TYPE_ACL_RX           0x00000010
#define TRACE_TYPE_TARGET_TRACE     0x00000011
#define TRACE_TYPE_SCO_TX           0x00000012
#define TRACE_TYPE_SCO_RX           0x00000013


#define TRACE_TYPE_MAX_NUM          20
#define TRACE_TYPE_ALL              0xffff

/* Define color for script type */
#define SCR_COLOR_DEFAULT       0
#define SCR_COLOR_TYPE_COMMENT  1
#define SCR_COLOR_TYPE_COMMAND  2
#define SCR_COLOR_TYPE_EVENT    3
#define SCR_COLOR_TYPE_SELECT   4

/* Define protocol trace flag values */
#define SCR_PROTO_TRACE_HCI_SUMMARY 0x00000001
#define SCR_PROTO_TRACE_HCI_DATA    0x00000002
#define SCR_PROTO_TRACE_L2CAP       0x00000004
#define SCR_PROTO_TRACE_RFCOMM      0x00000008
#define SCR_PROTO_TRACE_SDP         0x00000010
#define SCR_PROTO_TRACE_TCS         0x00000020
#define SCR_PROTO_TRACE_OBEX        0x00000040
#define SCR_PROTO_TRACE_OAPP        0x00000080 /* OBEX Application Profile */
#define SCR_PROTO_TRACE_AMP         0x00000100
#define SCR_PROTO_TRACE_BNEP        0x00000200
#define SCR_PROTO_TRACE_AVP         0x00000400
#define SCR_PROTO_TRACE_MCA         0x00000800
#define SCR_PROTO_TRACE_ATT         0x00001000
#define SCR_PROTO_TRACE_SMP         0x00002000
#define SCR_PROTO_TRACE_NCI         0x00004000
#define SCR_PROTO_TRACE_DEP         0x00008000
#define SCR_PROTO_TRACE_LLCP        0x00010000
#define SCR_PROTO_TRACE_NDEF        0x00020000
#define SCR_PROTO_TRACE_TAGS        0x00040000
#define SCR_PROTO_TRACE_ALL         0x0007ffff
#define SCR_PROTO_TRACE_HCI_LOGGING_VSE 0x0800 /* Brcm vs event for logmsg and protocol traces */

#define MAX_SCRIPT_TYPE             5

#define TCS_PSM_INTERCOM        5
#define TCS_PSM_CORDLESS        7
#define BT_PSM_BNEP             0x000F
/* Define PSMs HID uses */
#define HID_PSM_CONTROL         0x0011
#define HID_PSM_INTERRUPT       0x0013

/* Define a function for logging */
typedef void (BT_LOG_FUNC) (int trace_type, const char *fmt_str, ...);

#endif
/* end "revolution/BTE/stack/include/bt_types.h" */

/* Error codes */
#define GKI_SUCCESS         0x00
#define GKI_FAILURE         0x01
#define GKI_INVALID_TASK    0xF0
#define GKI_INVALID_POOL    0xFF


/************************************************************************
** Mailbox definitions. Each task has 4 mailboxes that are used to
** send buffers to the task.
*/
#define TASK_MBOX_0    0
#define TASK_MBOX_1    1
#define TASK_MBOX_2    2
#define TASK_MBOX_3    3

#define NUM_TASK_MBOX  4

/************************************************************************
** Event definitions.
**
** There are 4 reserved events used to signal messages rcvd in task mailboxes.
** There are 4 reserved events used to signal timeout events.
** There are 8 general purpose events available for applications.
*/
#define MAX_EVENTS              16

#define TASK_MBOX_0_EVT_MASK   0x0001
#define TASK_MBOX_1_EVT_MASK   0x0002
#define TASK_MBOX_2_EVT_MASK   0x0004
#define TASK_MBOX_3_EVT_MASK   0x0008


#define TIMER_0             0
#define TIMER_1             1
#define TIMER_2             2
#define TIMER_3             3

#define TIMER_0_EVT_MASK    0x0010
#define TIMER_1_EVT_MASK    0x0020
#define TIMER_2_EVT_MASK    0x0040
#define TIMER_3_EVT_MASK    0x0080

#define APPL_EVT_0          8
#define APPL_EVT_1          9
#define APPL_EVT_2          10
#define APPL_EVT_3          11
#define APPL_EVT_4          12
#define APPL_EVT_5          13
#define APPL_EVT_6          14
#define APPL_EVT_7          15

#define EVENT_MASK(evt)       ((UINT16)(0x0001 << (evt)))

/************************************************************************
**  Max Time Queue
**/
#ifndef GKI_MAX_TIMER_QUEUES
#define GKI_MAX_TIMER_QUEUES    3
#endif

/************************************************************************
**  Macro to determine the pool buffer size based on the GKI POOL ID at compile time.
**  Pool IDs index from 0 to GKI_NUM_FIXED_BUF_POOLS - 1
*/

#if (GKI_NUM_FIXED_BUF_POOLS < 1)

#ifndef GKI_POOL_ID_0
#define GKI_POOL_ID_0                0
#endif /* ifndef GKI_POOL_ID_0 */

#ifndef GKI_BUF0_SIZE
#define GKI_BUF0_SIZE                0
#endif /* ifndef GKI_BUF0_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 1 */


#if (GKI_NUM_FIXED_BUF_POOLS < 2)

#ifndef GKI_POOL_ID_1
#define GKI_POOL_ID_1                0
#endif /* ifndef GKI_POOL_ID_1 */

#ifndef GKI_BUF1_SIZE
#define GKI_BUF1_SIZE                0
#endif /* ifndef GKI_BUF1_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 2 */


#if (GKI_NUM_FIXED_BUF_POOLS < 3)

#ifndef GKI_POOL_ID_2
#define GKI_POOL_ID_2                0
#endif /* ifndef GKI_POOL_ID_2 */

#ifndef GKI_BUF2_SIZE
#define GKI_BUF2_SIZE                0
#endif /* ifndef GKI_BUF2_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 3 */


#if (GKI_NUM_FIXED_BUF_POOLS < 4)

#ifndef GKI_POOL_ID_3
#define GKI_POOL_ID_3                0
#endif /* ifndef GKI_POOL_ID_4 */

#ifndef GKI_BUF3_SIZE
#define GKI_BUF3_SIZE                0
#endif /* ifndef GKI_BUF3_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 4 */


#if (GKI_NUM_FIXED_BUF_POOLS < 5)

#ifndef GKI_POOL_ID_4
#define GKI_POOL_ID_4                0
#endif /* ifndef GKI_POOL_ID_4 */

#ifndef GKI_BUF4_SIZE
#define GKI_BUF4_SIZE                0
#endif /* ifndef GKI_BUF4_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 5 */


#if (GKI_NUM_FIXED_BUF_POOLS < 6)

#ifndef GKI_POOL_ID_5
#define GKI_POOL_ID_5                0
#endif /* ifndef GKI_POOL_ID_5 */

#ifndef GKI_BUF5_SIZE
#define GKI_BUF5_SIZE                0
#endif /* ifndef GKI_BUF5_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 6 */


#if (GKI_NUM_FIXED_BUF_POOLS < 7)

#ifndef GKI_POOL_ID_6
#define GKI_POOL_ID_6                0
#endif /* ifndef GKI_POOL_ID_6 */

#ifndef GKI_BUF6_SIZE
#define GKI_BUF6_SIZE                0
#endif /* ifndef GKI_BUF6_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 7 */


#if (GKI_NUM_FIXED_BUF_POOLS < 8)

#ifndef GKI_POOL_ID_7
#define GKI_POOL_ID_7                0
#endif /* ifndef GKI_POOL_ID_7 */

#ifndef GKI_BUF7_SIZE
#define GKI_BUF7_SIZE                0
#endif /* ifndef GKI_BUF7_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 8 */


#if (GKI_NUM_FIXED_BUF_POOLS < 9)

#ifndef GKI_POOL_ID_8
#define GKI_POOL_ID_8                0
#endif /* ifndef GKI_POOL_ID_8 */

#ifndef GKI_BUF8_SIZE
#define GKI_BUF8_SIZE                0
#endif /* ifndef GKI_BUF8_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 9 */


#if (GKI_NUM_FIXED_BUF_POOLS < 10)

#ifndef GKI_POOL_ID_9
#define GKI_POOL_ID_9                0
#endif /* ifndef GKI_POOL_ID_9 */

#ifndef GKI_BUF9_SIZE
#define GKI_BUF9_SIZE                0
#endif /* ifndef GKI_BUF9_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 10 */


#if (GKI_NUM_FIXED_BUF_POOLS < 11)

#ifndef GKI_POOL_ID_10
#define GKI_POOL_ID_10                0
#endif /* ifndef GKI_POOL_ID_10 */

#ifndef GKI_BUF10_SIZE
#define GKI_BUF10_SIZE                0
#endif /* ifndef GKI_BUF10_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 11 */


#if (GKI_NUM_FIXED_BUF_POOLS < 12)

#ifndef GKI_POOL_ID_11
#define GKI_POOL_ID_11                0
#endif /* ifndef GKI_POOL_ID_11 */

#ifndef GKI_BUF11_SIZE
#define GKI_BUF11_SIZE                0
#endif /* ifndef GKI_BUF11_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 12 */


#if (GKI_NUM_FIXED_BUF_POOLS < 13)

#ifndef GKI_POOL_ID_12
#define GKI_POOL_ID_12                0
#endif /* ifndef GKI_POOL_ID_12 */

#ifndef GKI_BUF12_SIZE
#define GKI_BUF12_SIZE                0
#endif /* ifndef GKI_BUF12_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 13 */


#if (GKI_NUM_FIXED_BUF_POOLS < 14)

#ifndef GKI_POOL_ID_13
#define GKI_POOL_ID_13                0
#endif /* ifndef GKI_POOL_ID_13 */

#ifndef GKI_BUF13_SIZE
#define GKI_BUF13_SIZE                0
#endif /* ifndef GKI_BUF13_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 14 */


#if (GKI_NUM_FIXED_BUF_POOLS < 15)

#ifndef GKI_POOL_ID_14
#define GKI_POOL_ID_14                0
#endif /* ifndef GKI_POOL_ID_14 */

#ifndef GKI_BUF14_SIZE
#define GKI_BUF14_SIZE                0
#endif /* ifndef GKI_BUF14_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 15 */


#if (GKI_NUM_FIXED_BUF_POOLS < 16)

#ifndef GKI_POOL_ID_15
#define GKI_POOL_ID_15                0
#endif /* ifndef GKI_POOL_ID_15 */

#ifndef GKI_BUF15_SIZE
#define GKI_BUF15_SIZE                0
#endif /* ifndef GKI_BUF15_SIZE */

#endif /* GKI_NUM_FIXED_BUF_POOLS < 16 */


/* Timer list entry callback type
*/
typedef void (TIMER_CBACK)(void *p_tle);
#ifndef TIMER_PARAM_TYPE
#ifdef  WIN2000
#define TIMER_PARAM_TYPE    void *
#else
#define TIMER_PARAM_TYPE    UINT32
#endif
#endif
/* Define a timer list entry
*/
typedef struct _tle
{
    struct _tle  *p_next;
    struct _tle  *p_prev;
    TIMER_CBACK  *p_cback;
    INT32         ticks;
    TIMER_PARAM_TYPE   param;
    UINT16        event;
    UINT8         in_use;
} TIMER_LIST_ENT;

/* Define a timer list queue
*/
typedef struct
{
    TIMER_LIST_ENT   *p_first;
    TIMER_LIST_ENT   *p_last;
    INT32             last_ticks;
} TIMER_LIST_Q;


/***********************************************************************
** This queue is a general purpose buffer queue, for application use.
*/
typedef struct
{
    void    *p_first;
    void    *p_last;
    UINT16   count;
} BUFFER_Q;

#define GKI_IS_QUEUE_EMPTY(p_q) ((p_q)->count == 0)

/* Task constants
*/
#ifndef TASKPTR
typedef void (*TASKPTR)(UINT32);
#endif


#define GKI_PUBLIC_POOL         0       /* General pool accessible to GKI_getbuf() */
#define GKI_RESTRICTED_POOL     1       /* Inaccessible pool to GKI_getbuf() */

/***********************************************************************
** Function prototypes
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Task management
*/
GKI_API extern UINT8   GKI_create_task (TASKPTR, UINT8, INT8 *, UINT16 *, UINT16);
GKI_API extern void    GKI_destroy_task(UINT8 task_id);
GKI_API extern void    GKI_task_self_cleanup(UINT8 task_id);
GKI_API extern void    GKI_exit_task(UINT8);
GKI_API extern UINT8   GKI_get_taskid(void);
GKI_API extern void    GKI_init(void);
GKI_API extern void    GKI_shutdown(void);
GKI_API extern INT8   *GKI_map_taskname(UINT8);
GKI_API extern UINT8   GKI_resume_task(UINT8);
GKI_API extern void    GKI_run(void *);
GKI_API extern void    GKI_freeze(void);
GKI_API extern void    GKI_stop(void);
GKI_API extern UINT8   GKI_suspend_task(UINT8);
GKI_API extern UINT8   GKI_is_task_running(UINT8);

/* memory management
*/
GKI_API extern void GKI_shiftdown (UINT8 *p_mem, UINT32 len, UINT32 shift_amount);
GKI_API extern void GKI_shiftup (UINT8 *p_dest, UINT8 *p_src, UINT32 len);

/* To send buffers and events between tasks
*/
GKI_API extern UINT8   GKI_isend_event (UINT8, UINT16);
GKI_API extern void   *GKI_read_mbox  (UINT8);
GKI_API extern void    GKI_send_msg   (UINT8, UINT8, void *);
GKI_API extern UINT8   GKI_send_event (UINT8, UINT16);


/* To get and release buffers, change owner and get size
*/
GKI_API extern void    GKI_change_buf_owner (void *, UINT8);
GKI_API extern UINT8   GKI_create_pool (UINT16, UINT16, UINT8, void *);
GKI_API extern void    GKI_delete_pool (UINT8);
GKI_API extern void   *GKI_find_buf_start (void *);
GKI_API extern void    GKI_freebuf (void *);
GKI_API extern void   *GKI_getbuf (UINT16);
GKI_API extern UINT16  GKI_get_buf_size (void *);
GKI_API extern void   *GKI_getpoolbuf (UINT8);
GKI_API extern UINT16  GKI_poolfreecount (UINT8);
GKI_API extern UINT16  GKI_poolutilization (UINT8);
GKI_API extern void    GKI_register_mempool (void *p_mem);
GKI_API extern UINT8   GKI_set_pool_permission(UINT8, UINT8);


/* User buffer queue management
*/
GKI_API extern void   *GKI_dequeue  (BUFFER_Q *);
GKI_API extern void    GKI_enqueue (BUFFER_Q *, void *);
GKI_API extern void    GKI_enqueue_head (BUFFER_Q *, void *);
GKI_API extern void   *GKI_getfirst (BUFFER_Q *);
GKI_API extern void   *GKI_getnext (void *);
GKI_API extern void    GKI_init_q (BUFFER_Q *);
GKI_API extern BOOLEAN GKI_queue_is_empty(BUFFER_Q *);
GKI_API extern void   *GKI_remove_from_queue (BUFFER_Q *, void *);
GKI_API extern UINT16  GKI_get_pool_bufsize (UINT8);

/* Timer management
*/
GKI_API extern void    GKI_add_to_timer_list (TIMER_LIST_Q *, TIMER_LIST_ENT  *);
GKI_API extern void    GKI_delay(UINT32);
GKI_API extern UINT32  GKI_get_tick_count(void);
GKI_API extern INT8   *GKI_get_time_stamp(INT8 *);
GKI_API extern void    GKI_init_timer_list (TIMER_LIST_Q *);
GKI_API extern void    GKI_init_timer_list_entry (TIMER_LIST_ENT  *);
GKI_API extern INT32   GKI_ready_to_sleep (void);
GKI_API extern void    GKI_remove_from_timer_list (TIMER_LIST_Q *, TIMER_LIST_ENT  *);
GKI_API extern void    GKI_start_timer(UINT8, INT32, BOOLEAN);
GKI_API extern void    GKI_stop_timer (UINT8);
GKI_API extern void    GKI_timer_update(INT32);
GKI_API extern UINT16  GKI_update_timer_list (TIMER_LIST_Q *, INT32);
GKI_API extern UINT16  GKI_wait(UINT16, UINT32);

/* Start and Stop system time tick callback
 * true for start system tick if time queue is not empty
 * false to stop system tick if time queue is empty
*/
typedef void (SYSTEM_TICK_CBACK)(BOOLEAN);

/* Time queue management for system ticks
*/
GKI_API extern BOOLEAN GKI_timer_queue_empty (void);
GKI_API extern void    GKI_timer_queue_register_callback(SYSTEM_TICK_CBACK *);

/* Disable Interrupts, Enable Interrupts
*/
GKI_API extern void    GKI_enable(void);
GKI_API extern void    GKI_disable(void);
GKI_API extern void    GKI_sched_lock(void);
GKI_API extern void    GKI_sched_unlock(void);

/* Allocate (Free) memory from an OS
*/
GKI_API extern void     *GKI_os_malloc (UINT32);
GKI_API extern void      GKI_os_free (void *);

/* os timer operation */
GKI_API extern UINT32 GKI_get_os_tick_count(void);

/* Exception handling
*/
GKI_API extern void    GKI_exception (UINT16, char *);

#if GKI_DEBUG == TRUE
GKI_API extern void    GKI_PrintBufferUsage(UINT8 *p_num_pools, UINT16 *p_cur_used);
GKI_API extern void    GKI_PrintBuffer(void);
GKI_API extern void    GKI_print_task(void);
#else
#undef GKI_PrintBufferUsage
#define GKI_PrintBuffer() NULL
#endif

#ifdef __cplusplus
}
#endif


#endif
/* end "revolution/bte/gki/common/gki.h" */
/* "libs/RVL_SDK/include/revolution/bte/rvl/uusb_ppc.h" line 5 "revolution/usb/usb.h" */
#ifndef RVL_SDK_USB_H
#define RVL_SDK_USB_H
/* "libs/RVL_SDK/include/revolution/usb/usb.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/usb/usb.h" line 4 "revolution/IPC.h" */
/**
 * References: WiiBrew, Dolphin Emulator, fail0verflow
 */

#ifndef RVL_SDK_PUBLIC_IPC_H
#define RVL_SDK_PUBLIC_IPC_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/IPC.h" line 10 "revolution/IPC/ipcMain.h" */
#ifndef RVL_SDK_IPC_MAIN_H
#define RVL_SDK_IPC_MAIN_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcMain.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/IPC/ipcMain.h" line 4 "revolution/IPC/ipcHardware.h" */
#ifndef RVL_SDK_IPC_HARDWARE_H
#define RVL_SDK_IPC_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcHardware.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * IPC hardware registers
 */
volatile u32 DECL_HW_REGS(IPC_PPC) DECL_ADDRESS(0xCD000000);
volatile u32 DECL_HW_REGS(IPC) DECL_ADDRESS(0xCD800000);

/**
 * Hardware register indexes
 */
// clang-format off
#define LIST_OF_REGS                                                           \
    X(IPC_PPCMSG,      0xCD000000)                                             \
    X(IPC_PPCCTRL,     0xCD000004)                                             \
    X(IPC_ARMMSG,      0xCD000008)                                             \
    X(IPC_ARMCTRL,     0xCD00000C)                                             \
    X(TIMER,           0xCD000010)                                             \
    X(ALARM,           0xCD000014)                                             \
    X(PPCIRQFLAG,      0xCD000030)                                             \
    X(PPCIRQMASK,      0xCD000034)                                             \
    X(ARMIRQFLAG,      0xCD000038)                                             \
    X(ARMIRQMASK,      0xCD00003C)                                             \
    X(MEMMIRR,         0xCD000060)                                             \
    X(AHBPROT,         0xCD000064)                                             \
    X(EXICTRL,         0xCD000070)                                             \
    X(GPIO1BOUT,       0xCD0000C0)                                             \
    X(GPIO1BDIR,       0xCD0000C4)                                             \
    X(GPIO1BIN,        0xCD0000C8)                                             \
    X(GPIO1BINTLVL,    0xCD0000CC)                                             \
    X(GPIO1BINTFLAG,   0xCD0000D0)                                             \
    X(GPIO1BINTENABLE, 0xCD0000D4)                                             \
    X(GPIO1BINMIR,     0xCD0000D8)                                             \
    X(GPIO1ENABLE,     0xCD0000DC)                                             \
    X(GPIO1OUT,        0xCD0000E0)                                             \
    X(GPIO1DIR,        0xCD0000E4)                                             \
    X(GPIO1IN,         0xCD0000E8)                                             \
    X(GPIO1INTLVL,     0xCD0000EC)                                             \
    X(GPIO1INTFLAG,    0xCD0000F0)                                             \
    X(GPIO1INTENABLE,  0xCD0000F4)                                             \
    X(GPIO1INMIR,      0xCD0000F8)                                             \
    X(GPIO1OWNER,      0xCD0000FC)                                             \
    X(DIFLAGS,         0xCD000180)                                             \
    X(RESETS,          0xCD000194)                                             \
    X(CLOCKS,          0xCD0001B4)                                             \
    X(GPIO2OUT,        0xCD0001C8)                                             \
    X(GPIO2DIR,        0xCD0001CC)                                             \
    X(GPIO2IN,         0xCD0001D0)                                             \
    X(OTPCMD,          0xCD0001EC)                                             \
    X(OTPDATA,         0xCD0001F0)                                             \
    X(VERSION,         0xCD000214)
// clang-format on

/**
 * Hardware register indexes (IPC)
 */
#define X(NAME, ADDR) IPC_##NAME = (ADDR - 0xCD000000) / 4,
typedef enum { LIST_OF_REGS } IPCHwReg;
#undef X

/**
 * Hardware register indexes (ACR)
 */
#define X(NAME, ADDR) ACR_##NAME = (ADDR - 0xCD000000),
typedef enum { LIST_OF_REGS } ACRHwReg;
#undef X

/**
 * GPIO register flags
 */
typedef enum {
    GPIO_POWER = (1 << 0),
    GPIO_SHUTDOWN = (1 << 1),
    GPIO_FAN = (1 << 2),
    GPIO_DCDC = (1 << 3),
    GPIO_DISPIN = (1 << 4),
    GPIO_SLOTLED = (1 << 5),
    GPIO_EJECTBTN = (1 << 6),
    GPIO_SLOTIN = (1 << 7),
    GPIO_SENSORBAR = (1 << 8),
    GPIO_DOEJECT = (1 << 9),
    GPIO_EEP_CS = (1 << 10),
    GPIO_EEP_CLK = (1 << 11),
    GPIO_EEP_MOSI = (1 << 12),
    GPIO_EEP_MISO = (1 << 13),
    GPIO_AVE_SCL = (1 << 14),
    GPIO_AVE_SDA = (1 << 15),
    GPIO_DEBUG0 = (1 << 16),
    GPIO_DEBUG1 = (1 << 17),
    GPIO_DEBUG2 = (1 << 18),
    GPIO_DEBUG3 = (1 << 19),
    GPIO_DEBUG4 = (1 << 20),
    GPIO_DEBUG5 = (1 << 21),
    GPIO_DEBUG6 = (1 << 22),
    GPIO_DEBUG7 = (1 << 23),
} GPIOFlag;

#undef LIST_OF_REGS

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcHardware.h" */
#ifdef __cplusplus
extern "C" {
#endif

inline u32 ACRReadReg(u32 reg) {
    return IPC_PPC_HW_REGS[reg / 4];
}

inline void ACRWriteReg(u32 reg, u32 val) {
    IPC_PPC_HW_REGS[reg / 4] = val;
}

void IPCInit(void);
void IPCReInit(void);
u32 IPCReadReg(s32 index);
void IPCWriteReg(s32 index, u32 value);
void* IPCGetBufferHi(void);
void* IPCGetBufferLo(void);
void IPCSetBufferLo(void* lo);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcMain.h" */
/* "libs/RVL_SDK/include/revolution/IPC.h" line 11 "revolution/IPC/ipcProfile.h" */
#ifndef RVL_SDK_IPC_PROFILE_H
#define RVL_SDK_IPC_PROFILE_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcProfile.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/IPC/ipcProfile.h" line 4 "revolution/IPC/ipcclt.h" */
#ifndef RVL_SDK_IPC_CLT_H
#define RVL_SDK_IPC_CLT_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcclt.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/IPC/ipcclt.h" line 4 "revolution/OS.h" */
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
    char padding[4];        // tail padding for 8-byte array alignment
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

typedef enum _GXPerf0 {
    GX_PERF0_VERTICES,
    GX_PERF0_CLIP_VTX,
    GX_PERF0_CLIP_CLKS,
    GX_PERF0_XF_WAIT_IN,
    GX_PERF0_XF_WAIT_OUT,
    GX_PERF0_XF_XFRM_CLKS,
    GX_PERF0_XF_LIT_CLKS,
    GX_PERF0_XF_BOT_CLKS,
    GX_PERF0_XF_REGLD_CLKS,
    GX_PERF0_XF_REGRD_CLKS,
    GX_PERF0_CLIP_RATIO,
    GX_PERF0_TRIANGLES,
    GX_PERF0_TRIANGLES_CULLED,
    GX_PERF0_TRIANGLES_PASSED,
    GX_PERF0_TRIANGLES_SCISSORED,
    GX_PERF0_TRIANGLES_0TEX,
    GX_PERF0_TRIANGLES_1TEX,
    GX_PERF0_TRIANGLES_2TEX,
    GX_PERF0_TRIANGLES_3TEX,
    GX_PERF0_TRIANGLES_4TEX,
    GX_PERF0_TRIANGLES_5TEX,
    GX_PERF0_TRIANGLES_6TEX,
    GX_PERF0_TRIANGLES_7TEX,
    GX_PERF0_TRIANGLES_8TEX,
    GX_PERF0_TRIANGLES_0CLR,
    GX_PERF0_TRIANGLES_1CLR,
    GX_PERF0_TRIANGLES_2CLR,
    GX_PERF0_QUAD_0CVG,
    GX_PERF0_QUAD_NON0CVG,
    GX_PERF0_QUAD_1CVG,
    GX_PERF0_QUAD_2CVG,
    GX_PERF0_QUAD_3CVG,
    GX_PERF0_QUAD_4CVG,
    GX_PERF0_AVG_QUAD_CNT,
    GX_PERF0_CLOCKS,
    GX_PERF0_NONE
} GXPerf0;

typedef enum _GXPerf1 {
    GX_PERF1_TEXELS,
    GX_PERF1_TX_IDLE,
    GX_PERF1_TX_REGS,
    GX_PERF1_TX_MEMSTALL,
    GX_PERF1_TC_CHECK1_2,
    GX_PERF1_TC_CHECK3_4,
    GX_PERF1_TC_CHECK5_6,
    GX_PERF1_TC_CHECK7_8,
    GX_PERF1_TC_MISS,
    GX_PERF1_VC_ELEMQ_FULL,
    GX_PERF1_VC_MISSQ_FULL,
    GX_PERF1_VC_MEMREQ_FULL,
    GX_PERF1_VC_STATUS7,
    GX_PERF1_VC_MISSREP_FULL,
    GX_PERF1_VC_STREAMBUF_LOW,
    GX_PERF1_VC_ALL_STALLS,
    GX_PERF1_VERTICES,
    GX_PERF1_FIFO_REQ,
    GX_PERF1_CALL_REQ,
    GX_PERF1_VC_MISS_REQ,
    GX_PERF1_CP_ALL_REQ,
    GX_PERF1_CLOCKS,
    GX_PERF1_NONE
} GXPerf1;

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
void OSShutdownSystem(void);
void OSRestart(u32 resetCode);
void __OSReturnToMenu(u8 menuMode);
void OSReturnToMenu(void);
void __OSReturnToMenuForError(void);
void __OSHotResetForError(void);
u32 OSGetResetCode(void);
void OSResetSystem(BOOL reset, u32 resetCode, BOOL forceMenu);
extern volatile BOOL __OSIsReturnToIdle;

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
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IPC_RESULT_FATAL_ERROR = -119,
    IPC_RESULT_BUSY,
    IPC_RESULT_NOTEMPTY = -115,
    IPC_RESULT_ECC_CRIT,
    IPC_RESULT_OPENFD = -111,
    IPC_RESULT_MAXFD = -109,
    IPC_RESULT_MAXBLOCKS,
    IPC_RESULT_MAXFILES,
    IPC_RESULT_NOEXISTS,
    IPC_RESULT_EXISTS,
    IPC_RESULT_CORRUPT = -103,
    IPC_RESULT_ACCESS,
    IPC_RESULT_INVALID,

    IPC_RESULT_ALLOC_FAILED = -22,
    IPC_RESULT_ECC_CRIT_INTERNAL = -12,
    IPC_RESULT_BUSY_INTERNAL = -8,
    IPC_RESULT_NOEXISTS_INTERNAL = -6,
    IPC_RESULT_CONN_MAX_INTERNAL = -5,
    IPC_RESULT_INVALID_INTERNAL = -4,
    IPC_RESULT_EXISTS_INTERNAL = -2,
    IPC_RESULT_ACCESS_INTERNAL = -1,

    IPC_RESULT_OK = 0
} IPCResult;

typedef enum {
    IPC_REQ_NONE,
    IPC_REQ_OPEN,
    IPC_REQ_CLOSE,
    IPC_REQ_READ,
    IPC_REQ_WRITE,
    IPC_REQ_SEEK,
    IPC_REQ_IOCTL,
    IPC_REQ_IOCTLV
} IPCRequestType;

typedef enum {
    // Ioctl
    IPC_IOCTL_GET_NAND_STATS = 2,
    IPC_IOCTL_CREATE_DIR = 3,
    IPC_IOCTL_READ_DIR = 4,
    IPC_IOCTL_GET_ATTR = 6,
    IPC_IOCTL_DELETE_PATH = 7,
    IPC_IOCTL_RENAME_PATH = 8,
    IPC_IOCTL_CREATE_FILE = 9,
    IPC_IOCTL_GET_FILE_STATS = 11,
    IPC_IOCTL_SHUTDOWN_FS = 13,
    IPC_IOCTL_REG_STM_EVENT = 0x1000,
    IPC_IOCTL_HOT_RESET = 0x2001,
    IPC_IOCTL_SHUTDOWN_TO_SBY = 0x2003,
    IPC_IOCTL_UNREG_STM_EVENT = 0x3002,
    IPC_IOCTL_SET_VI_DIM = 0x5001,
    IPC_IOCTL_SET_IDLE_LED_MODE = 0x6002,

    // Ioctlv
    IPC_IOCTLV_LAUNCH_TITLE = 8,
    IPC_IOCTLV_GET_USAGE = 12,
    IPC_IOCTLV_GET_NUM_TICKET_VIEWS = 18,
    IPC_IOCTLV_GET_TICKET_VIEWS = 19,
    IPC_IOCTLV_GET_DATA_DIR = 29,
    IPC_IOCTLV_GET_TITLE_ID = 32,
} IPCIoctlType;

typedef enum {
    IPC_OPEN_NONE = 0,
    IPC_OPEN_READ = (1 << 0),
    IPC_OPEN_WRITE = (1 << 1),
    IPC_OPEN_RW = IPC_OPEN_READ | IPC_OPEN_WRITE
} IPCOpenMode;

typedef enum {
    IPC_SEEK_BEG,
    IPC_SEEK_CUR,
    IPC_SEEK_END,
} IPCSeekMode;

typedef s32 (*IPCAsyncCallback)(s32 result, void* arg);

typedef struct IPCIOVector {
    void* base; // at 0x0
    u32 length; // at 0x4
} IPCIOVector;

typedef struct IPCOpenArgs {
    const char* path; // at 0x0
    IPCOpenMode mode; // at 0x4
} IPCOpenArgs;

typedef struct IPCReadWriteArgs {
    void* data; // at 0x0
    u32 length; // at 0x4
} IPCReadWriteArgs;

typedef struct IPCSeekArgs {
    s32 offset;       // at 0x0
    IPCSeekMode mode; // at 0x4
} IPCSeekArgs;

typedef struct IPCIoctlArgs {
    s32 type;    // at 0x0
    void* in;    // at 0x4
    s32 inSize;  // at 0x8
    void* out;   // at 0xC
    s32 outSize; // at 0x10
} IPCIoctlArgs;

typedef struct IPCIoctlvArgs {
    s32 type;             // at 0x0
    u32 inCount;          // at 0x4
    u32 outCount;         // at 0x8
    IPCIOVector* vectors; // at 0xC
} IPCIoctlvArgs;

typedef struct IPCRequest {
    IPCRequestType type; // at 0x0
    s32 ret;             // at 0x4
    s32 fd;              // at 0x8
    union {
        IPCOpenArgs open;
        IPCReadWriteArgs rw;
        IPCSeekArgs seek;
        IPCIoctlArgs ioctl;
        IPCIoctlvArgs ioctlv;
    }; // at 0xC
} IPCRequest;

typedef struct IPCRequestEx {
    IPCRequest base;           // at 0x0
    IPCAsyncCallback callback; // at 0x20
    void* callbackArg;         // at 0x24
    BOOL reboot;               // at 0x28
    OSThreadQueue queue;       // at 0x2C
    char padding[64 - 0x34];
} IPCRequestEx;

s32 IPCCltInit(void);
s32 IPCCltReInit(void);
s32 IOS_OpenAsync(const char* path, IPCOpenMode mode, IPCAsyncCallback callback,
                  void* callbackArg);
s32 IOS_Open(const char* path, IPCOpenMode mode);
s32 IOS_CloseAsync(s32 fd, IPCAsyncCallback callback, void* callbackArg);
s32 IOS_Close(s32 fd);
s32 IOS_ReadAsync(s32 fd, void* buf, s32 len, IPCAsyncCallback callback,
                  void* callbackArg);
s32 IOS_Read(s32 fd, void* buf, s32 len);
s32 IOS_WriteAsync(s32 fd, const void* buf, s32 len, IPCAsyncCallback callback,
                   void* callbackArg);
s32 IOS_Write(s32 fd, const void* buf, s32 len);
s32 IOS_SeekAsync(s32 fd, s32 offset, IPCSeekMode mode,
                  IPCAsyncCallback callback, void* callbackArg);
s32 IOS_Seek(s32 fd, s32 offset, IPCSeekMode mode);
s32 IOS_IoctlAsync(s32 fd, s32 type, void* in, s32 inSize, void* out,
                   s32 outSize, IPCAsyncCallback callback, void* callbackArg);
s32 IOS_Ioctl(s32 fd, s32 type, void* in, s32 inSize, void* out, s32 outSize);
s32 IOS_IoctlvAsync(s32 fd, s32 type, s32 inCount, s32 outCount,
                    IPCIOVector* vectors, IPCAsyncCallback callback,
                    void* callbackArg);
s32 IOS_Ioctlv(s32 fd, s32 type, s32 inCount, s32 outCount,
               IPCIOVector* vectors);
s32 IOS_IoctlvReboot(s32 fd, s32 type, s32 inCount, s32 outCount,
                     IPCIOVector* vectors);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcclt.h" */
#ifdef __cplusplus
extern "C" {
#endif

void IPCiProfInit(void);
void IPCiProfQueueReq(IPCRequestEx* req, s32 fd);
void IPCiProfAck(void);
void IPCiProfReply(IPCRequestEx* req, s32 fd);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcProfile.h" */
/* "libs/RVL_SDK/include/revolution/IPC.h" line 12 "revolution/IPC/ipcclt.h" */
/* end "revolution/IPC/ipcclt.h" */
/* "libs/RVL_SDK/include/revolution/IPC.h" line 13 "revolution/IPC/memory.h" */
#ifndef RVL_SDK_IPC_MEMORY_H
#define RVL_SDK_IPC_MEMORY_H
/* "libs/RVL_SDK/include/revolution/IPC/memory.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

s32 iosCreateHeap(void* base, u32 size);
void* iosAllocAligned(s32 handle, u32 size, u32 align);
s32 iosFree(s32 handle, void* block);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/memory.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*USBCallback)(IPCResult result, void* arg);
typedef void (*USBISOCallback)(IPCResult result, void* arg1, void* arg2);

IPCResult IUSB_OpenLib(void);
IPCResult IUSB_CloseLib(void);
IPCResult IUSB_OpenDeviceIds(const char* interface, u16 vid, u16 pid,
                             IPCResult* resultOut);
IPCResult IUSB_CloseDeviceAsync(s32 fd, USBCallback callback,
                                void* callbackArg);
IPCResult IUSB_ReadIntrMsgAsync(s32 fd, u32 endpoint, u32 length, void* buffer,
                                USBCallback callback, void* callbackArg);
IPCResult IUSB_ReadBlkMsgAsync(s32 fd, u32 endpoint, u32 length, void* buffer,
                               USBCallback callback, void* callbackArg);
IPCResult IUSB_WriteBlkMsgAsync(s32 fd, u32 endpoint, u32 length,
                                const void* buffer, USBCallback callback,
                                void* callbackArg);
IPCResult IUSB_WriteCtrlMsgAsync(s32 fd, u8 requestType, u8 request, u16 value,
                                 u16 index, u16 length, void* buffer,
                                 USBCallback callback, void* callbackArg);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/usb/usb.h" */

#ifdef __cplusplus
extern "C" {
#endif

u32 __ntd_get_allocated_mem_size(void);

/* UUSB private control block (retail global `usb`, size 0x4C).
 * Field names are recovered where semantics are understood; unknown
 * members keep `field_0xNN` offsets. */
typedef struct tUUSB_CB {
    s32 fd;                       /* 0x00 device file descriptor */
    u8 field_0x04[0xC];          /* 0x04 unused in matched functions */
    u8 field_0x10;                /* 0x10 endpoint (intr/bulk ctrl) */
    u8 field_0x11;                /* 0x11 endpoint */
    u8 field_0x12;                /* 0x12 endpoint */
    u8 field_0x13;                /* 0x13 endpoint */
    u32 vid;                      /* 0x14 USB vendor id */
    u32 pid;                      /* 0x18 USB product id */
    u8 pool_id_intr;             /* 0x1C intr read pool id */
    u8 pool_id_bulk;             /* 0x1D bulk read pool id */
    u8 field_0x1E[2];            /* 0x1E */
    void (*close_cb)(int reason, s8 result); /* 0x20 upper-layer close notify */
    u32 close_cb_arg;            /* 0x24 close callback argument */
    u8 trace_state;             /* 0x28 trace init state */
    u8 state;                    /* 0x29 UUSB state machine */
    u8 field_0x2A;              /* 0x2A */
    u8 field_0x2B;              /* 0x2B read-restart flag */
    BUFFER_Q bulk_write_q;       /* 0x2C bulk write buffer queue */
    u8 bulk_pending;            /* 0x38 outstanding bulk writes */
    u8 field_0x39[3];           /* 0x39 */
    BUFFER_Q ctrl_write_q;       /* 0x3C ctrl write buffer queue */
    u8 ctrl_pending;            /* 0x48 outstanding ctrl writes */
    u8 field_0x49[3];           /* 0x49 */
} tUUSB_CB;

/* UUSB state values (usb.state). */
enum {
    UUSB_STATE_IDLE = 0,
    UUSB_STATE_OPEN = 2,
    UUSB_STATE_READY = 4,
    UUSB_STATE_CLOSED = 5,
};

/* Close notification reason passed to tUUSB_CB.close_cb. */
#define UUSB_CLOSE_REASON_CLOSED 4

/* Global control block and trace flags (retail linker names). */
extern tUUSB_CB usb;
extern u8 uusb_g_usb_devid_found;
extern u8 uusb_g_trace_state_initialized;
extern u32 wait4hci;

/* Public UUSB API. */
void UUSB_Register(void* cb_arg);
void UUSB_Open(s32 fd, void (*close_cb)(int reason, s8 result));
u16 UUSB_Read(u8 channel, void* p_buf, u16 len);
s32 UUSB_Write(s32 type, void* data, u16 length);
void UUSB_Close(void);
void UUSB_Unregister(void);

/* USB close completion callback (IUSB_CloseDeviceAsync). */
void uusb_CloseDeviceCB(IPCResult result, void* arg);
void uusb_ReadIntrDataCB(IPCResult result, void* arg);
void uusb_ReadBulkDataCB(IPCResult result, void* arg);
void uusb_WriteCtrlDataCB(IPCResult result, void* arg);
void uusb_WriteBulkDataCB(IPCResult result, void* arg);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/bte/rvl/uusb_ppc.h" */
/* "libs/RVL_SDK/src/revolution/bte/hci/uusb_ppc.c" line 8 "revolution/os/OSContext.h" */
/* end "revolution/os/OSContext.h" */

/* BTE stack helpers (defined in other BTE TUs, not declared in headers). */
extern void bta_ci_hci_msg_handler(void* p_data);
extern BT_HDR* l2cap_link_chk_pkt_start(BT_HDR* p_buf);
extern BOOLEAN l2cap_link_chk_pkt_end(void);

/* Global control block and trace flags (retail linker symbols). */
tUUSB_CB usb;
u8 uusb_g_usb_devid_found;
u8 uusb_g_trace_state_initialized;

/* Fiber stacks for the BTE HCI message dispatcher (retail linker symbols). */
u8 __uusb_ppc_stack1[0x1000];
u8 __uusb_ppc_stack2[0x1000];

/* Upper-layer wait-for-HCI flag (retail .sdata symbol). */
u32 wait4hci = 1;

/* uusb_CloseDeviceCB - completion callback for IUSB_CloseDeviceAsync.
 * Tears down the control block, closes the IUSB library, resets the UUSB
 * state machine to CLOSED, then notifies the upper layer (if it registered
 * a close callback) with the IPC result. */
void uusb_CloseDeviceCB(IPCResult result, void* arg) {
    (void)arg;

    /* Forget the device and all endpoint/pool bookkeeping. */
    usb.fd = 0;
    usb.field_0x10 = 0;
    usb.field_0x11 = 0;
    usb.field_0x12 = 0;
    usb.field_0x13 = 0;
    uusb_g_usb_devid_found = 0;

    IUSB_CloseLib();

    /* Reset the UUSB state machine under the GKI lock. */
    GKI_disable();
    usb.state = UUSB_STATE_CLOSED;
    usb.trace_state = 0;
    uusb_g_trace_state_initialized = 0;
    GKI_enable();

    /* Notify the upper layer that the device has closed. */
    if (usb.close_cb != NULL) {
        usb.close_cb(UUSB_CLOSE_REASON_CLOSED, (s8)result);
    }
}

void uusb_ReadIntrDataCB(IPCResult result, void* arg) {
    (void)result;
    (void)arg;
}

/* uusb_ReadBulkDataCB - IUSB_ReadBlkMsgAsync completion callback.
 * If the UUSB state machine left the OPEN state the pool is torn down and
 * the buffer freed. Otherwise the received payload is repacked into a
 * fresh pool-3 buffer, checked through the L2CAP reassembly filter and,
 * when the packet is complete, handed to the BTE HCI message handler on a
 * fiber stack. A new bulk-in read is then always re-armed. */
void uusb_ReadBulkDataCB(IPCResult result, void* arg) {
    BT_HDR* buf;
    BT_HDR* pkt;
    u8* data;

    if (usb.state != UUSB_STATE_OPEN) {
        GKI_freebuf(arg);
        GKI_delete_pool(usb.pool_id_bulk);
        usb.pool_id_bulk = 0xff;
        return;
    }

    if (result <= 0) {
        GKI_freebuf(arg);
    } else {
        ((BT_HDR*)arg)->len = (u16)result;
        buf = (BT_HDR*)GKI_getpoolbuf(3);
        if (buf == NULL) {
            GKI_freebuf(arg);
        } else {
            memcpy(buf, arg,
                   (((u32)(((BT_HDR*)arg)->len + ((BT_HDR*)arg)->offset +
                           BT_HDR_SIZE)) &
                    ~3u) +
                       4);
            pkt = l2cap_link_chk_pkt_start(buf);
            if (pkt != NULL && l2cap_link_chk_pkt_end()) {
                OSSwitchFiberEx((u32)pkt, 0, 0, 0, bta_ci_hci_msg_handler,
                                __uusb_ppc_stack2 + 0x1000);
            }
            GKI_freebuf(arg);
        }
    }

    /* Re-arm the bulk-in read on a fresh pool buffer. */
    do {
        buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_bulk);
    } while (buf == NULL);
    buf->event = 0x1100;
    buf->len = 0;
    data = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
    buf->offset = (u16)(data - ((u8*)buf + BT_HDR_SIZE));
    if (IUSB_ReadBlkMsgAsync(usb.fd, usb.field_0x11,
                             (u16)GKI_get_buf_size(buf) - 0x28 - buf->offset,
                             data, uusb_ReadBulkDataCB, buf) != 0) {
        GKI_freebuf(buf);
    }
}

/* uusb_WriteCtrlDataCB - IUSB_WriteCtrlMsgAsync completion callback.
 * On entry `arg` is the BT_HDR buffer that was submitted. When the write
 * completes (arg != NULL) the buffer is returned to the GKI pool and the
 * outstanding-write counter is decremented. Then, while the in-flight window
 * (5) is not saturated and the queue is non-empty, the next queued buffer is
 * dispatched; a failed submit pushes the buffer back to the head. */
void uusb_WriteCtrlDataCB(IPCResult result, void* arg) {
    BT_HDR* buf;
    IPCResult rc;

    (void)result;

    if (arg != NULL) {
        GKI_freebuf(arg);
        GKI_disable();
        usb.ctrl_pending -= 1;
        GKI_enable();
    }

    /* While the in-flight window (5) is not saturated and the queue is
     * non-empty, dispatch the next queued control-write buffer. */
    if (usb.ctrl_pending < 5 && usb.ctrl_write_q.count != 0) {
        buf = (BT_HDR*)GKI_dequeue(&usb.ctrl_write_q);
        rc = IUSB_WriteCtrlMsgAsync(usb.fd, 0x20, 0, 0, 0, buf->len,
                                    (u8*)buf + buf->offset + BT_HDR_SIZE,
                                    uusb_WriteCtrlDataCB, buf);
        if (rc < IPC_RESULT_OK) {
            /* Submission rejected: restore the buffer to the queue head. */
            GKI_enqueue_head(&usb.ctrl_write_q, buf);
        } else {
            GKI_disable();
            usb.ctrl_pending += 1;
            GKI_enable();
        }
    }
}

/* uusb_WriteBulkDataCB - IUSB_WriteBlkMsgAsync completion callback.
 * On entry `arg` is the BT_HDR buffer that was submitted. When the write
 * completes (arg != NULL) the buffer is returned to the GKI pool and the
 * outstanding-write counter is decremented. Then, while the in-flight window
 * (5) is not saturated and the queue is non-empty, the next queued buffer is
 * dispatched; a failed submit pushes the buffer back to the head. */
void uusb_WriteBulkDataCB(IPCResult result, void* arg) {
    BT_HDR* buf;
    IPCResult rc;

    (void)result;

    if (arg != NULL) {
        GKI_freebuf(arg);
        GKI_disable();
        usb.bulk_pending -= 1;
        GKI_enable();
    }

    /* While the in-flight window (5) is not saturated and the queue is
     * non-empty, dispatch the next queued bulk-write buffer. */
    if (usb.bulk_pending < 5 && usb.bulk_write_q.count != 0) {
        buf = (BT_HDR*)GKI_dequeue(&usb.bulk_write_q);
        rc = IUSB_WriteBlkMsgAsync(usb.fd, usb.field_0x10, buf->len,
                                   (u8*)buf + buf->offset + BT_HDR_SIZE,
                                   uusb_WriteBulkDataCB, buf);
        if (rc < IPC_RESULT_OK) {
            /* Submission rejected: restore the buffer to the queue head. */
            GKI_enqueue_head(&usb.bulk_write_q, buf);
        } else {
            GKI_disable();
            usb.bulk_pending += 1;
            GKI_enable();
        }
    }
}

void UUSB_Register(void* cb_arg) {
    (void)cb_arg;
}

/* UUSB_Open - bring the USB-HCI link up once UUSB_Register has built the
 * read pools (state READY). Installs the upper-layer close callback, then
 * arms the interrupt-in and bulk-in reads with fresh GKI buffers and clears
 * the wait-for-HCI flag so the stack init can proceed. */
void UUSB_Open(s32 fd, void (*close_cb)(int reason, s8 result)) {
    BT_HDR* buf;
    u8* data;

    (void)fd;

    if (usb.state != UUSB_STATE_READY) goto done;
    if (usb.pool_id_intr == 0xff) goto done;
    if (usb.pool_id_bulk == 0xff) goto done;
    goto body;
done:
    return;
body:
        GKI_disable();
        usb.close_cb = close_cb;
        usb.state = UUSB_STATE_OPEN;
        GKI_enable();

        /* Arm the interrupt-in read. */
        do {
            buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_intr);
        } while (buf == NULL);
        buf->event = 0x1000;
        buf->len = 0;
        data = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
        buf->offset = (u16)(data - ((u8*)buf + BT_HDR_SIZE));
        if (IUSB_ReadIntrMsgAsync(usb.fd, usb.field_0x12,
                                  (u16)GKI_get_buf_size(buf) - 0x28 - buf->offset,
                                  data, uusb_ReadIntrDataCB, buf) != 0) {
            GKI_freebuf(buf);
        }
        usb.field_0x2B = 1;

        /* Arm the bulk-in read. */
        do {
            buf = (BT_HDR*)GKI_getpoolbuf(usb.pool_id_bulk);
        } while (buf == NULL);
        buf->event = 0x1100;
        buf->len = 0;
        data = (u8*)(((u32)buf + 0x27) & ~0x1Fu);
        buf->offset = (u16)(data - ((u8*)buf + BT_HDR_SIZE));
        if (IUSB_ReadBlkMsgAsync(usb.fd, usb.field_0x11,
                                 (u16)GKI_get_buf_size(buf) - 0x28 - buf->offset,
                                 data, uusb_ReadBulkDataCB, buf) != 0) {
            GKI_freebuf(buf);
        }
        wait4hci = 0;
    return;
}

u16 UUSB_Read(u8 channel, void* p_buf, u16 len) {
    (void)channel;
    (void)p_buf;
    (void)len;
    return 0;
}

s32 UUSB_Write(s32 type, void* data, u16 length) {
    (void)type;
    (void)data;
    (void)length;
    return 0;
}

/* UUSB_Close - mark the control block idle, drain any queued bulk/ctrl
 * write buffers back to the GKI pool, then kick off an async device close.
 * The actual teardown (state -> CLOSED, close notify) happens in the
 * uusb_CloseDeviceCB completion callback. */
void UUSB_Close(void) {
    GKI_disable();
    usb.state = UUSB_STATE_IDLE;
    GKI_enable();

    /* Free any bulk-write buffers still pending in the queue. */
    if (usb.bulk_write_q.count != 0) {
        while (usb.bulk_write_q.count != 0) {
            GKI_freebuf(GKI_dequeue(&usb.bulk_write_q));
        }
    }

    /* Free any control-write buffers still pending in the queue. */
    if (usb.ctrl_write_q.count != 0) {
        while (usb.ctrl_write_q.count != 0) {
            GKI_freebuf(GKI_dequeue(&usb.ctrl_write_q));
        }
    }

    IUSB_CloseDeviceAsync(usb.fd, uusb_CloseDeviceCB, NULL);
}

/* UUSB_Unregister - release the IUSB library and return the UUSB state
 * machine to CLOSED so a subsequent UUSB_Register can reinitialise it. */
void UUSB_Unregister(void) {
    IUSB_CloseLib();

    GKI_disable();
    usb.state = UUSB_STATE_CLOSED;
    usb.trace_state = 0;
    uusb_g_trace_state_initialized = 0;
    GKI_enable();
}
