// Decompiled btm_inq.c - high-level C reconstruction (Wii RVL SDK).
//
// Retail symbol btm_cb is a .bss object at 0x805BC2F8 (size 0x27C4).
// btm_int.h's tBTM_CB / tBTM_INQUIRY_VAR_ST layouts do not match the Wii
// binary (e.g. retail BTM_INQ_DB_SIZE is 12, not 40; the inq vars struct
// has extra padding), so we declare a local surrogate covering only the
// fields we touch (verified against
// build/us/asm/RVL_SDK/src/revolution/bte/stack/btm/btm_inq.s offsets).

/* "libs/RVL_SDK/src/revolution/bte/stack/btm/btm_inq.c" line 9 "string.h" */
#ifndef MSL_STRING_H
#define MSL_STRING_H

/* "libs/PowerPC_EABI_Support/include/stl/string.h" line 3 "types.h" */
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

/* "libs/RVL_SDK/src/revolution/bte/stack/btm/btm_inq.c" line 11 "revolution/BTE/stack/include/bt_types.h" */
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

/* ------------------------------------------------------------------ */
/*  Return codes / constants (from btm_api.h)                          */
/* ------------------------------------------------------------------ */
typedef UINT8 tBTM_STATUS;

#define BTM_SUCCESS          0
#define BTM_CMD_STARTED      1
#define BTM_BUSY             2
#define BTM_NO_RESOURCES     3
#define BTM_MODE_UNSUPPORTED 4
#define BTM_ILLEGAL_VALUE    5
#define BTM_WRONG_MODE       6
#define BTM_BAD_VALUE_RET    9

#define BTM_INQ_DB_SIZE      12
#define BTM_RMT_NAME_INACTIVE 0
#define BTM_RMT_NAME_EXT     0x01

#define BTM_INQ_INACTIVE_STATE  0
#define BTM_INQ_CLR_FILT_STATE  1
#define BTM_INQ_SET_FILT_STATE  2
#define BTM_INQ_REMNAME_STATE   4

#define BT_TRACE_LEVEL_API   3
#define BT_TRACE_LEVEL_DEBUG 5

#define HCI_SUCCESS          0x00
#define HCI_ERR_UNSPECIFIED  0x1F

#define BTM_ERR_PROCESSING   10
#define BTM_DEV_RESET        12

/* Local controller LMP feature bits (byte 3 of the feature pages) */
#define HCI_FEATURE_INTERLACED_INQ_SCAN_MASK   0x10
#define HCI_FEATURE_INTERLACED_PAGE_SCAN_MASK  0x20
#define HCI_FEATURE_INQ_RSSI_MASK              0x40

#define HCI_CMD_POOL_ID       2

#define BTM_INQ_RESULT_STANDARD     0
#define BTM_INQ_RESULT_WITH_RSSI    1

#define BTM_SCAN_TYPE_STANDARD      0
#define BTM_SCAN_TYPE_INTERLACED    1

#define BTM_NON_CONNECTABLE         0
#define BTM_CONNECTABLE             1

#define BTM_NON_DISCOVERABLE        0

#define BTM_DEFAULT_CONN_WINDOW     0x0012
#define BTM_DEFAULT_CONN_INTERVAL   0x0800
#define BTM_MIN_CONN_WINDOW         0x0012
#define BTM_MAX_CONN_WINDOW         0x1000
#define BTM_MIN_CONN_INTERVAL       0x0012
#define BTM_MAX_CONN_INTERVAL       0x1000

#define BTM_FILTER_TYPE_CLEAR       0
#define BTM_FILTER_TYPE_DEV_CLASS   1
#define BTM_FILTER_TYPE_BD_ADDR     2
#define HCI_FILTER_INQUIRY_RESULT   0x01

#define BTM_EXT_RMT_NAME_TIMEOUT    40
#define BTU_TTYPE_BTM_RMT_NAME      10
#define BTM_CLOCK_OFFSET_VALID      0x8000

#define HCI_PAGE_SCAN_REP_MODE_R1   0x01
#define HCI_MANDATARY_PAGE_SCAN_MODE 0x00

#define HCI_INQUIRY_SCAN_ENABLED    0x01
#define HCI_PAGE_SCAN_ENABLED       0x02

/* ------------------------------------------------------------------ */
/*  Callback / log types                                              */
/* ------------------------------------------------------------------ */
typedef void (tBTM_CMPL_CB)(void *p1);
typedef void (tBTM_INQ_DB_CHANGE_CB)(void *p_inq_info, BOOLEAN is_new);

extern void LogMsg_0 (UINT32 trace_set_mask, const char *p_str);
extern void LogMsg_1 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1);
extern void LogMsg_2 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2);
extern void LogMsg_3 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3);
extern void LogMsg_6 (UINT32 trace_set_mask, const char *fmt_str, UINT32 p1, UINT32 p2,
                      UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);
extern void btu_stop_timer (void *p_tle);
extern void btu_start_timer (void *p_tle, UINT16 type, UINT32 timeout);
extern void *GKI_getpoolbuf (UINT8 pool_id);
extern void GKI_freebuf (void *p_buf);

extern void btsnd_hcic_write_inqscan_type (void *p_buf, UINT8 type);
extern BOOLEAN BTM_IsDeviceUp (void);
extern void btsnd_hcic_write_pagescan_type (void *p_buf, UINT8 type);
extern void btsnd_hcic_write_inquiry_mode (void *p_buf, UINT8 mode);
extern void btsnd_hcic_write_pagescan_cfg (void *p_buf, UINT16 interval, UINT16 window);
extern void btsnd_hcic_write_scan_enable (void *p_buf, UINT8 mode);
extern BOOLEAN btsnd_hcic_inq_cancel (void);
extern void btsnd_hcic_set_event_filter (void *p_buf, UINT8 filt_type, UINT8 filt_cond_type,
                                         UINT8 *p_filt_cond, UINT8 filt_cond_len);
extern BOOLEAN btsnd_hcic_rmt_name_req (BD_ADDR remote_bda, UINT8 page_scan_rep_mode,
                                         UINT8 page_scan_mode, UINT16 clock_offset);
extern BOOLEAN btsnd_hcic_rmt_name_req_cancel (BD_ADDR remote_bda);
extern void btm_sec_rmt_name_request_complete (UINT8 *bd_addr, UINT8 *bd_name,
                                               UINT8 status);

/* ------------------------------------------------------------------ */
/*  Remote name result structure (from btm_api.h)                     */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT16  status;
    UINT16  length;
    BD_NAME remote_bd_name;
} tBTM_REMOTE_DEV_NAME;

/* Inquiry completion info (status and number of responses) */
typedef struct
{
    UINT8   status;
    UINT8   num_resp;
} tBTM_INQUIRY_CMPL;

/* Forward declaration (defined below; used by BTM_ReadRemoteDeviceName) */

/* ------------------------------------------------------------------ */
/*  Inquiry database structures (retail layout)                       */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT16  clock_offset;            /* 0x00 */
    BD_ADDR remote_bd_addr;          /* 0x02 */
    DEV_CLASS dev_class;             /* 0x08 */
    UINT8   page_scan_rep_mode;      /* 0x0B */
    UINT8   page_scan_per_mode;      /* 0x0C */
    UINT8   page_scan_mode;          /* 0x0D */
    INT8    rssi;                    /* 0x0E */
    UINT8   appl_knows_rem_name;     /* 0x0F */
    UINT8   pad[0x12 - 0x10];        /* 0x10-0x11 */
} tBTM_INQ_INFO;                     /* 0x12 */

/* Forward declaration (defined below; used by BTM_ReadRemoteDeviceName) */
extern tBTM_STATUS btm_initiate_rem_name (BD_ADDR remote_bda, tBTM_INQ_INFO *p_cur,
                                           UINT8 origin, UINT32 timeout,
                                           tBTM_CMPL_CB *p_cb);

typedef struct
{
    UINT32      time_of_resp;        /* 0x00 */
    UINT32      inq_count;           /* 0x04 */
    tBTM_INQ_INFO inq_info;          /* 0x08 */
    BOOLEAN     in_use;              /* 0x1A */
} tINQ_DB_ENT;                       /* 0x1C */

typedef struct
{
    UINT32  inq_count;               /* 0x00 */
    BD_ADDR bd_addr;                 /* 0x04 */
} tINQ_BDADDR;                       /* 0x0C */

/* ------------------------------------------------------------------ */
/*  Local btm_cb surrogate - only the fields we touch.                */
/* ------------------------------------------------------------------ */
typedef struct
{
    UINT8              _pad0[0x640];                     /* 0x0000-0x063F */
    UINT8              btm_features[8];                  /* 0x0640 - local LMP features */
    DEV_CLASS          dev_class;                        /* 0x0648 */
    UINT8              _pad0a[0x64E - 0x064B];           /* 0x064B-0x064D */
    UINT8              dev_state;                        /* 0x064E - device state */
    UINT8              _pad0b[0x167C - 0x064F];          /* 0x064F-0x167B */
    tBTM_CMPL_CB      *p_remname_cmpl_cb;                /* 0x167C */
    UINT8              rmt_name_timer_ent[0xC];          /* 0x1680 */
    UINT8              _pad1[0x1698 - 0x168C];           /* 0x168C-0x1697 */
    UINT16             discoverable_mode;                /* 0x1698 */
    UINT16             connectable_mode;                 /* 0x169A */
    UINT16             page_scan_window;                 /* 0x169C */
    UINT16             page_scan_period;                 /* 0x169E */
    UINT16             inq_scan_window;                  /* 0x16A0 */
    UINT16             inq_scan_period;                  /* 0x16A2 */
    UINT16             inq_scan_type;                    /* 0x16A4 */
    UINT16             page_scan_type;                   /* 0x16A6 */
    BD_ADDR            remname_bda;                      /* 0x16A8 */
    UINT8              remname_active;                   /* 0x16AE */
    UINT8              _pad2;                            /* 0x16AF */
    tBTM_CMPL_CB      *p_inq_cmpl_cb;                    /* 0x16B0 */
    void              *p_inq_results_cb;                 /* 0x16B4 */
    tBTM_CMPL_CB      *p_inqfilter_cmpl_cb;              /* 0x16B8 */
    tBTM_INQ_DB_CHANGE_CB *p_inq_change_cb;              /* 0x16BC */
    UINT32             inq_counter;                      /* 0x16C0 */
    UINT8              inq_timer_ent[0xC];               /* 0x16C4 */
    UINT8              _pad3[0x16DC - 0x16D0];           /* 0x16D0-0x16DB */
    tINQ_BDADDR       *p_bd_db;                          /* 0x16DC */
    UINT16             num_bd_entries;                   /* 0x16E0 */
    UINT16             max_bd_entries;                   /* 0x16E2 */
    tINQ_DB_ENT        inq_db[BTM_INQ_DB_SIZE];          /* 0x16E4 */
    UINT8              _pad4[0x183E - 0x16E4 -
                             sizeof(tINQ_DB_ENT) * BTM_INQ_DB_SIZE]; /* 0x1834-0x183D */
    tBTM_INQUIRY_CMPL  inq_cmpl_info;                    /* 0x183E */
    UINT8              _pad4a[0x1844 - 0x1840];          /* 0x1840-0x1843 */
    UINT8              inqfilt_active;                   /* 0x1844 */
    UINT8              inqfilt_type;                     /* 0x1845 */
    UINT8              _pad5;                            /* 0x1846 */
    UINT8              pending_filt_complete_event;      /* 0x1847 */
    UINT8              state;                            /* 0x1848 */
    UINT8              _pad6[0x27C0 - 0x1849];           /* 0x1849-0x27BF */
    UINT8              trace_level;                      /* 0x27C0 */
} tBTM_INQ_CB;

/* The real global */
extern tBTM_INQ_CB btm_cb;

void BTM_SetDiscoverability() {}

/* ------------------------------------------------------------------ */
/*  BTM_SetInquiryScanType - set the inquiry scan type (standard or    */
/*  interlaced).                                                       */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetInquiryScanType (UINT16 scan_type)
{
    tBTM_STATUS status = BTM_SUCCESS;
    void       *p_buf;

    if ((scan_type != BTM_SCAN_TYPE_STANDARD) && (scan_type != BTM_SCAN_TYPE_INTERLACED))
        return (BTM_ILLEGAL_VALUE);

    /* Whatever app wants if device is not 1.2 scan type should be STANDARD */
    if (!(btm_cb.btm_features[3] & HCI_FEATURE_INTERLACED_INQ_SCAN_MASK))
        return (BTM_MODE_UNSUPPORTED);

    /* Check for scan type if configuration has been changed */
    if (scan_type != btm_cb.inq_scan_type)
    {
        if (BTM_IsDeviceUp())
        {
            if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
            {
                btsnd_hcic_write_inqscan_type (p_buf, (UINT8)scan_type);
                btm_cb.inq_scan_type = scan_type;
            }
            else
                return (BTM_NO_RESOURCES);
        }
        else
            return (BTM_WRONG_MODE);
    }

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetPageScanType - set the page scan type (standard or          */
/*  interlaced).                                                       */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetPageScanType (UINT16 scan_type)
{
    tBTM_STATUS status = BTM_SUCCESS;
    void       *p_buf;

    if ((scan_type != BTM_SCAN_TYPE_STANDARD) && (scan_type != BTM_SCAN_TYPE_INTERLACED))
        return (BTM_ILLEGAL_VALUE);

    /* Whatever app wants if device is not 1.2 scan type should be STANDARD */
    if (!(btm_cb.btm_features[3] & HCI_FEATURE_INTERLACED_PAGE_SCAN_MASK))
        return (BTM_MODE_UNSUPPORTED);

    /* Check for scan type if configuration has been changed */
    if (scan_type != btm_cb.page_scan_type)
    {
        if (BTM_IsDeviceUp())
        {
            if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
            {
                btsnd_hcic_write_pagescan_type (p_buf, (UINT8)scan_type);
                btm_cb.page_scan_type = scan_type;
            }
            else
                return (BTM_NO_RESOURCES);
        }
        else
            return (BTM_WRONG_MODE);
    }

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetInquiryMode - set the inquiry result mode (standard or      */
/*  with RSSI).                                                        */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetInquiryMode (UINT8 mode)
{
    tBTM_STATUS status = BTM_SUCCESS;
    void       *p_buf;

    if ((mode != BTM_INQ_RESULT_STANDARD) && (mode != BTM_INQ_RESULT_WITH_RSSI))
        return (BTM_ILLEGAL_VALUE);

    /* If mode is not supported by controller, return */
    if (!(btm_cb.btm_features[3] & HCI_FEATURE_INQ_RSSI_MASK))
        return (BTM_MODE_UNSUPPORTED);

    /* If the device is not up, return */
    if (!BTM_IsDeviceUp())
        return (BTM_WRONG_MODE);

    /* Send the HCI command */
    if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
    {
        btsnd_hcic_write_inquiry_mode (p_buf, mode);
    }
    else
        return (BTM_NO_RESOURCES);

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  BTM_SetConnectability - set the device into or out of connectable  */
/*  mode (page scans enabled).                                         */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_SetConnectability (UINT16 page_mode, UINT16 window, UINT16 interval)
{
    UINT8        scan_mode = 0;
    void        *p_buf;
    tBTM_INQ_CB *p_inq = &btm_cb;

    /* Check mode parameter */
    if ((page_mode != BTM_NON_CONNECTABLE) && (page_mode != BTM_CONNECTABLE))
        return (BTM_ILLEGAL_VALUE);

    /* Make sure the controller is active */
    if (btm_cb.dev_state < 3)
        return (BTM_DEV_RESET);

    /* If the window and/or interval is '0', set to default values */
    if (!window)
        window = BTM_DEFAULT_CONN_WINDOW;

    if (!interval)
        interval = BTM_DEFAULT_CONN_INTERVAL;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_3 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_SetConnectability: mode %d [NonConn-0, Conn-1], window 0x%04x, interval 0x%04x",
                  page_mode, window, interval);

    /* Check for valid window and interval parameters */
    /* Only check window and duration if mode is connectable */
    if (page_mode == BTM_CONNECTABLE)
    {
        /* window must be less than or equal to interval */
        if ((window < BTM_MIN_CONN_WINDOW) || (window > BTM_MAX_CONN_WINDOW) ||
            (interval < BTM_MIN_CONN_INTERVAL) || (interval > BTM_MAX_CONN_INTERVAL) ||
            (window > interval))
        {
            return (BTM_ILLEGAL_VALUE);
        }

        scan_mode |= HCI_PAGE_SCAN_ENABLED;
    }

    /* Send down the page scan window and period if changed */
    if ((window != p_inq->page_scan_window) || (interval != p_inq->page_scan_period))
    {
        if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
        {
            p_inq->page_scan_window = window;
            p_inq->page_scan_period = interval;
            btsnd_hcic_write_pagescan_cfg (p_buf, interval, window);
        }
        else
            return (BTM_NO_RESOURCES);
    }

    /* Keep the inquiry scan as previously set */
    if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
    {
        if (p_inq->discoverable_mode)
            scan_mode |= HCI_INQUIRY_SCAN_ENABLED;

        p_inq->connectable_mode = page_mode;
        btsnd_hcic_write_scan_enable (p_buf, scan_mode);
    }
    else
        return (BTM_NO_RESOURCES);

    return (BTM_SUCCESS);
}
/* ------------------------------------------------------------------ */
/*  BTM_IsInquiryActive - return the current inquiry state byte.      */
/* ------------------------------------------------------------------ */
UINT16 BTM_IsInquiryActive(void)
{
    return btm_cb.state;
}

/* ------------------------------------------------------------------ */
/*  BTM_CancelInquiry - cancel an inquiry if active.                   */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_CancelInquiry (void)
{
    tBTM_INQ_CB *p_inq = &btm_cb;
    tBTM_STATUS  status = BTM_SUCCESS;

    if (p_inq->trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_0 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_CancelInquiry called");

    /* Make sure the device is up */
    if (!BTM_IsDeviceUp())
        return (BTM_WRONG_MODE);

    /* Only cancel if not in periodic mode, otherwise the caller should call */
    /* BTM_CancelPeriodicMode */
    if ((p_inq->state != BTM_INQ_INACTIVE_STATE) && (!(p_inq->state & BTM_INQ_REMNAME_STATE)))
    {
        p_inq->state = BTM_INQ_INACTIVE_STATE;
        p_inq->pending_filt_complete_event = 0;
        p_inq->p_inq_results_cb = NULL;    /* Do not notify caller any more */
        p_inq->p_inq_cmpl_cb = NULL;       /* Do not notify caller any more */

        /* If the event filter is in progress, mark it so that the processing of the return */
        /* event will be ignored */
        if (p_inq->inqfilt_active)
        {
            p_inq->inqfilt_active = FALSE;
            p_inq->inqfilt_type++;
        }
        /* Initiate the cancel inquiry */
        else
        {
            if (!btsnd_hcic_inq_cancel())
                status = BTM_NO_RESOURCES;
        }

        p_inq->inq_counter++;

        /* Clear the BD-addr results filter */
        if (btm_cb.p_bd_db)
        {
            GKI_freebuf (btm_cb.p_bd_db);
            btm_cb.p_bd_db = NULL;
        }
        p_inq->num_bd_entries = 0;
        p_inq->max_bd_entries = 0;
    }

    return (status);
}

void BTM_StartInquiry() {}

/* ------------------------------------------------------------------ */
/*  BTM_ReadRemoteDeviceName - initiate a remote device name request.  */
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_ReadRemoteDeviceName (BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb)
{
    tBTM_INQ_INFO *p_cur = NULL;
    tINQ_DB_ENT   *p_ent;
    UINT16         xx;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_6 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                  "BTM_ReadRemoteDeviceName: bd addr [%02x%02x%02x%02x%02x%02x]",
                  remote_bda[0], remote_bda[1], remote_bda[2],
                  remote_bda[3], remote_bda[4], remote_bda[5]);

    /* Use the remote device's clock offset if it is in the local inquiry database */
    p_ent = btm_cb.inq_db;
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use && memcmp (p_ent->inq_info.remote_bd_addr, remote_bda, BD_ADDR_LEN) == 0)
            break;
    }

    if (xx < BTM_INQ_DB_SIZE)
        p_cur = &p_ent->inq_info;
    else
        p_ent = NULL;

    return (btm_initiate_rem_name (remote_bda, p_cur, BTM_RMT_NAME_EXT,
                                   BTM_EXT_RMT_NAME_TIMEOUT, p_cb));
}
/* ------------------------------------------------------------------ */
/*  BTM_CancelRemoteDeviceName - cancel a pending remote name request.*/
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_CancelRemoteDeviceName(void)
{
    tBTM_INQ_CB *p = &btm_cb;

    if (p->trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_0(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                 "BTM_CancelRemoteDeviceName()");

    /* Make sure the device is not busy */
    if (p->remname_active & BTM_RMT_NAME_EXT)
    {
        if (btsnd_hcic_rmt_name_req_cancel(p->remname_bda))
            return (BTM_CMD_STARTED);
        else
            return (BTM_NO_RESOURCES);
    }
    else
    {
        return (BTM_WRONG_MODE);
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_InqDbRead - look up an inquiry DB entry by BD address.        */
/* ------------------------------------------------------------------ */
tBTM_INQ_INFO *BTM_InqDbRead(BD_ADDR p_bda)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent = btm_cb.inq_db;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_API)
        LogMsg_6(TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_API,
                 "BTM_InqDbRead: bd addr [%02x%02x%02x%02x%02x%02x]",
                 p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use && memcmp(p_ent->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0)
            return (&p_ent->inq_info);
    }

    return (NULL);
}

/* ------------------------------------------------------------------ */
/*  BTM_InqDbFirst - return the first in-use inquiry DB entry.        */
/* ------------------------------------------------------------------ */
tBTM_INQ_INFO *BTM_InqDbFirst(void)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent = btm_cb.inq_db;

    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use)
            return (&p_ent->inq_info);
    }

    /* If here, no database entries */
    return (NULL);
}

/* ------------------------------------------------------------------ */
/*  BTM_InqDbNext - return the next in-use entry after p_cur.         */
/* ------------------------------------------------------------------ */
tBTM_INQ_INFO *BTM_InqDbNext(tBTM_INQ_INFO *p_cur)
{
    tINQ_DB_ENT *p_ent;
    UINT16      xx;

    if (p_cur)
    {
        /* If the current entry is valid, start from the next entry */
        xx = (UINT16)(((tINQ_DB_ENT *)((UINT8 *)p_cur - offsetof(tINQ_DB_ENT, inq_info)) -
                       btm_cb.inq_db) + 1);
        p_ent = &btm_cb.inq_db[xx];

        for (; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
        {
            if (p_ent->in_use)
                return (&p_ent->inq_info);
        }

        /* If here, no more database entries */
        return (NULL);
    }
    else
    {
        return (BTM_InqDbFirst());
    }
}

/* ------------------------------------------------------------------ */
/*  BTM_ClearInqDb - clear the inquiry database (optionally one addr).*/
/* ------------------------------------------------------------------ */
tBTM_STATUS BTM_ClearInqDb(BD_ADDR p_bda)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent;

    /* If the device is in the middle of an inquiry, return busy */
    if (btm_cb.state != BTM_INQ_INACTIVE_STATE ||
        btm_cb.remname_active ||
        btm_cb.inqfilt_active)
    {
        return (BTM_BUSY);
    }

    p_ent = btm_cb.inq_db;

    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use)
        {
            if (!p_bda || memcmp(p_ent->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0)
            {
                p_ent->in_use = FALSE;

                if (btm_cb.p_inq_change_cb)
                    btm_cb.p_inq_change_cb(&p_ent->inq_info, FALSE);
            }
        }
    }

    return (BTM_SUCCESS);
}

/* ------------------------------------------------------------------ */
/*  btm_inq_db_reset - clear the inquiry database and pending          */
/*  callbacks at reset.                                                */
/* ------------------------------------------------------------------ */
void btm_inq_db_reset (void)
{
    tBTM_REMOTE_DEV_NAME rem_name;
    UINT8               num_responses;
    UINT8               temp_state;
    tBTM_STATUS         status;
    tBTM_INQ_CB        *p_inq = &btm_cb;
    tINQ_DB_ENT        *p_ent;
    UINT16              xx;

    /* If an inquiry is active, cancel it */
    btu_stop_timer (p_inq->inq_timer_ent);

    /* If an inquiry or periodic inquiry is active, reset the mode to inactive */
    if (p_inq->state != BTM_INQ_INACTIVE_STATE)
    {
        temp_state = p_inq->state;    /* Save so state can change BEFORE callback is called */
        p_inq->state = BTM_INQ_INACTIVE_STATE;

        /* If not a periodic inquiry, the complete callback must be called to notify caller */
        if ((temp_state == BTM_INQ_CLR_FILT_STATE) || (temp_state == BTM_INQ_SET_FILT_STATE))
        {
            if (p_inq->p_inq_cmpl_cb)
            {
                num_responses = 0;
                (*p_inq->p_inq_cmpl_cb)(&num_responses);
            }
        }
    }

    /* Cancel a remote name request if active, and notify the caller (if waiting) */
    if (p_inq->remname_active & BTM_RMT_NAME_EXT)
    {
        btu_stop_timer (p_inq->rmt_name_timer_ent);

        p_inq->remname_active = BTM_RMT_NAME_INACTIVE;
        memset (p_inq->remname_bda, 0, BD_ADDR_LEN);

        if (p_inq->p_remname_cmpl_cb)
        {
            rem_name.status = BTM_DEV_RESET;
            (*p_inq->p_remname_cmpl_cb)(&rem_name);
            p_inq->p_remname_cmpl_cb = NULL;
        }
    }

    /* Cancel an inquiry filter request if active, and notify the caller (if waiting) */
    if (p_inq->inqfilt_active)
    {
        p_inq->inqfilt_active = FALSE;

        if (p_inq->p_inqfilter_cmpl_cb)
        {
            status = BTM_DEV_RESET;
            (*p_inq->p_inqfilter_cmpl_cb)(&status);
        }
    }

    p_inq->pending_filt_complete_event = 0;
    p_inq->inqfilt_type = 0;
    p_inq->p_inq_results_cb = NULL;

    /* Clear out all the entries in the database */
    p_ent = btm_cb.inq_db;
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (p_ent->in_use)
        {
            p_ent->in_use = FALSE;

            if (btm_cb.p_inq_change_cb)
                (*btm_cb.p_inq_change_cb)(&p_ent->inq_info, FALSE);
        }
    }

    /* Clear the BD-addr results filter */
    if (btm_cb.p_bd_db)
    {
        GKI_freebuf (btm_cb.p_bd_db);
        btm_cb.p_bd_db = NULL;
    }
    p_inq->num_bd_entries = 0;
    p_inq->max_bd_entries = 0;
    p_inq->discoverable_mode = BTM_NON_DISCOVERABLE;
    p_inq->connectable_mode = BTM_NON_CONNECTABLE;
    p_inq->page_scan_type = BTM_SCAN_TYPE_STANDARD;
    p_inq->inq_scan_type = BTM_SCAN_TYPE_STANDARD;
}

void btm_inq_db_init(void) {}

/* ------------------------------------------------------------------ */
/*  btm_inq_find_bdaddr - search the inquiry BD-addr filter table.    */
/* ------------------------------------------------------------------ */
BOOLEAN btm_inq_find_bdaddr(BD_ADDR p_bda)
{
    tINQ_BDADDR *p_ent = btm_cb.p_bd_db;
    UINT16      xx;
    tBTM_INQ_CB *p = &btm_cb;

    if ((p->state & BTM_INQ_REMNAME_STATE) || !p_ent)
    {
        /* If the database hasn't been allocated, we can not have an entry */
        return (FALSE);
    }

    for (xx = 0; xx < p->num_bd_entries; xx++, p_ent++)
    {
        if (memcmp(p_ent->bd_addr, p_bda, BD_ADDR_LEN) == 0 &&
            p_ent->inq_count == p->inq_counter)
        {
            return (TRUE);
        }
    }

    /* If here, not found.  Add the address to the database. */
    if (xx < p->max_bd_entries)
    {
        p_ent->inq_count = p->inq_counter;
        memcpy(p_ent->bd_addr, p_bda, BD_ADDR_LEN);
        p->num_bd_entries++;
    }

    return (FALSE);
}

/* ------------------------------------------------------------------ */
/*  btm_inq_db_new - create a new record in the inquiry database.     */
/* ------------------------------------------------------------------ */
tINQ_DB_ENT *btm_inq_db_new(BD_ADDR p_bda)
{
    UINT16      xx;
    tINQ_DB_ENT *p_ent = btm_cb.inq_db;
    tINQ_DB_ENT *p_oldest = btm_cb.inq_db;
    UINT32      time_of_resp = 0xFFFFFFFF;

    /* Find an unused entry, or the oldest in-use entry */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++)
    {
        if (!p_ent->in_use)
        {
            memset(p_ent, 0, sizeof(tINQ_DB_ENT));
            memcpy(p_ent->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN);
            p_ent->in_use = TRUE;
            return (p_ent);
        }

        if (p_ent->time_of_resp < time_of_resp)
        {
            p_oldest = p_ent;
            time_of_resp = p_ent->time_of_resp;
        }
    }

    /* If the oldest entry is in use, discard it */
    if (btm_cb.p_inq_change_cb)
        btm_cb.p_inq_change_cb(&p_oldest->inq_info, FALSE);

    memset(p_oldest, 0, sizeof(tINQ_DB_ENT));
    memcpy(p_oldest->inq_info.remote_bd_addr, p_bda, BD_ADDR_LEN);
    p_oldest->in_use = TRUE;

    return (p_oldest);
}

/* ------------------------------------------------------------------ */
/*  btm_set_inq_event_filter - set the inquiry event filter.           */
/* ------------------------------------------------------------------ */
UINT8 btm_set_inq_event_filter (UINT8 filter_type, BD_ADDR bd_addr)
{
    UINT8  condition_buf[DEV_CLASS_LEN * 2];
    UINT8 *p_cond = condition_buf;
    UINT8  condition_length = DEV_CLASS_LEN * 2;
    void  *p_buf;

    if ((p_buf = GKI_getpoolbuf (HCI_CMD_POOL_ID)) != NULL)
    {
        switch (filter_type)
        {
        case BTM_FILTER_TYPE_DEV_CLASS:
            /* Copy the device class and device class fields into contiguous memory to send to HCI */
            memcpy (condition_buf, bd_addr, DEV_CLASS_LEN);
            memcpy (&condition_buf[DEV_CLASS_LEN], bd_addr + DEV_CLASS_LEN, DEV_CLASS_LEN);

            /* condition length should already be set as the default */
            break;

        case BTM_FILTER_TYPE_BD_ADDR:
            p_cond = bd_addr;

            /* condition length should already be set as the default */
            break;

        case BTM_FILTER_TYPE_CLEAR:
            condition_length = 0;
            break;

        default:
            return (BTM_ILLEGAL_VALUE);    /* Bad parameter was passed in */
        }

        btm_cb.inqfilt_active = TRUE;

        /* Filter the inquiry results for the specified condition type and value */
        btsnd_hcic_set_event_filter (p_buf, HCI_FILTER_INQUIRY_RESULT, filter_type,
                                     p_cond, condition_length);

        return (BTM_CMD_STARTED);
    }

    return (BTM_NO_RESOURCES);
}

void btm_event_filter_complete() {}

void btm_process_inq_results() {}

/* ------------------------------------------------------------------ */
/*  btm_process_inq_complete - handle the inquiry complete event.      */
/* ------------------------------------------------------------------ */
void btm_process_inq_complete (UINT8 status)
{
    tBTM_CMPL_CB *p_cb = btm_cb.p_inq_cmpl_cb;
    tBTM_INQ_CB  *p_inq = &btm_cb;
    tBTM_STATUS   btm_status;

    /* Ignore any stray or late complete messages if the inquiry is not active */
    if (!p_inq->state)
        return;

    /* Increment the inquiry counter, so the results can be distinguished */
    p_inq->inq_counter++;

    /* status is BTM_SUCCESS if HCI_STATUS is zero */
    btm_status = BTM_SUCCESS;
    if (status != HCI_SUCCESS)
        btm_status = BTM_ERR_PROCESSING;

    p_inq->inq_cmpl_info.status = btm_status;

    /* Do not notify the caller if a remote name request is active */
    if (p_inq->state & BTM_INQ_REMNAME_STATE)
        return;

    p_inq->p_inq_results_cb = NULL;
    p_inq->state = BTM_INQ_INACTIVE_STATE;
    p_inq->pending_filt_complete_event = 0;
    p_inq->p_inq_cmpl_cb = NULL;

    if (btm_cb.p_bd_db)
    {
        GKI_freebuf (btm_cb.p_bd_db);
        btm_cb.p_bd_db = NULL;
    }
    p_inq->num_bd_entries = 0;
    p_inq->max_bd_entries = 0;

    if (btm_cb.trace_level >= BT_TRACE_LEVEL_DEBUG)
        LogMsg_2 (TRACE_CTRL_GENERAL | TRACE_LAYER_BTM | TRACE_ORG_STACK | TRACE_TYPE_DEBUG,
                  "BTM Inq Compl Callback: status 0x%02x, num results %d",
                  p_inq->inq_cmpl_info.status, p_inq->inq_cmpl_info.num_resp);

    /* If we have a callback registered for inquiry complete, call it */
    if (p_cb)
        (*p_cb)(&p_inq->inq_cmpl_info);
}

/* ------------------------------------------------------------------ */
/*  btm_initiate_rem_name - initiate a remote name request.            */
/* ------------------------------------------------------------------ */
tBTM_STATUS btm_initiate_rem_name (BD_ADDR remote_bda, tBTM_INQ_INFO *p_cur, UINT8 origin,
                                   UINT32 timeout, tBTM_CMPL_CB *p_cb)
{
    tBTM_INQ_CB *p_inq = &btm_cb;
    BOOLEAN      start = TRUE;
    BOOLEAN      cmd_ok = TRUE;
    tBTM_STATUS  status = BTM_CMD_STARTED;

    /* Make sure the device is ready */
    if (!BTM_IsDeviceUp())
        return (BTM_WRONG_MODE);

    /* Make sure there are no two remote name requests from external API in progress */
    if (!(p_inq->remname_active & origin))
    {
        if (origin == BTM_RMT_NAME_EXT)
        {
            if (p_inq->remname_active)
            {
                /* If it is the same BD address, we are just re-registering the callback */
                if (memcmp (remote_bda, p_inq->remname_bda, BD_ADDR_LEN) == 0)
                    start = FALSE;
                else
                    return (BTM_BUSY);
            }
            p_inq->p_remname_cmpl_cb = p_cb;
        }
        else if (p_inq->remname_active)
        {
            return (BTM_BUSY);
        }

        if (start)
        {
            memcpy (p_inq->remname_bda, remote_bda, BD_ADDR_LEN);
            btu_start_timer (p_inq->rmt_name_timer_ent, BTU_TTYPE_BTM_RMT_NAME, timeout);

            /* If the database entry exists for the device, use its clock offset */
            if (p_cur)
                cmd_ok = btsnd_hcic_rmt_name_req (remote_bda, p_cur->page_scan_rep_mode,
                                                  p_cur->page_scan_mode,
                                                  (UINT16)(p_cur->clock_offset | BTM_CLOCK_OFFSET_VALID));
            else    /* Otherwise use defaults and mark the clock offset as invalid */
                cmd_ok = btsnd_hcic_rmt_name_req (remote_bda, HCI_PAGE_SCAN_REP_MODE_R1,
                                                  HCI_MANDATARY_PAGE_SCAN_MODE, 0);
        }

        if (cmd_ok)
            p_inq->remname_active |= origin;
        else
            status = BTM_NO_RESOURCES;
    }
    else
    {
        status = BTM_BUSY;
    }

    return (status);
}
/* ------------------------------------------------------------------ */
/*  btm_process_remote_name - handle a remote name response.          */
/* ------------------------------------------------------------------ */
void btm_process_remote_name(BD_ADDR bda, BD_NAME name, UINT16 evt_len, UINT8 hci_status)
{
    tBTM_INQ_CB *p = &btm_cb;
    tBTM_CMPL_CB *p_cb = p->p_remname_cmpl_cb;
    UINT8         remname_active = p->remname_active;
    tBTM_REMOTE_DEV_NAME rem_name;

    btu_stop_timer(p->rmt_name_timer_ent);
    p->remname_active = BTM_RMT_NAME_INACTIVE;

    /* If the HCI command failed, send a fake response */
    if (!hci_status)
    {
        UINT8 *d = rem_name.remote_bd_name;
        UINT8 *s = name;
        UINT16 len = (evt_len < BD_NAME_LEN) ? evt_len : BD_NAME_LEN;

        rem_name.length = len;
        rem_name.status = HCI_SUCCESS;

        while (len > 0)
        {
            *d = *s;
            d++;
            s++;
            len--;
        }
    }
    else
    {
        rem_name.length = 0;
        rem_name.status = BTM_BAD_VALUE_RET;
        rem_name.remote_bd_name[0] = 0;
    }

    memset(p->remname_bda, 0, BD_ADDR_LEN);

    /* If a callback is registered, call it */
    if (remname_active & BTM_RMT_NAME_EXT)
    {
        p->p_remname_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(&rem_name);
    }
}

/* ------------------------------------------------------------------ */
/*  btm_inq_rmt_name_failed - remote name request failed.             */
/* ------------------------------------------------------------------ */
void btm_inq_rmt_name_failed(void)
{
    btm_process_remote_name(NULL, NULL, 0, HCI_ERR_UNSPECIFIED);
    btm_sec_rmt_name_request_complete(NULL, NULL, HCI_ERR_UNSPECIFIED);
}
