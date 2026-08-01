// High-level C reconstruction of RVL_SDK/src/revolution/bte/stack/gap/gap_utils

/* "libs/RVL_SDK/src/revolution/bte/stack/gap/gap_utils.c" line 2 "revolution/BTE/stack/include/btm_api.h" */
/******************************************************************************
 *
 *  NOTICE OF CHANGES
 *  2025/03/13:
 *      - Restore old function signatures
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

/******************************************************************************
 *
 *  This file contains the Bluetooth Manager (BTM) API function external
 *  definitions.
 *
 ******************************************************************************/
#ifndef BTM_API_H
#define BTM_API_H

/* "libs/RVL_SDK/include/revolution/BTE/stack/include/btm_api.h" line 41 "revolution/BTE/include/bt_target.h" */
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
/* "libs/RVL_SDK/include/revolution/BTE/stack/include/btm_api.h" line 42 "revolution/BTE/stack/include/sdp_api.h" */
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
#ifndef SDP_API_H
#define SDP_API_H

/* "libs/RVL_SDK/include/revolution/BTE/stack/include/sdp_api.h" line 22 "revolution/BTE/include/bt_target.h" */
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
/* "libs/RVL_SDK/include/revolution/BTE/stack/include/sdp_api.h" line 23 "revolution/BTE/stack/include/sdpdefs.h" */
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

/******************************************************************************
 *
 *  This file contains the definitions for the SDP API
 *
 ******************************************************************************/

#ifndef SDP_DEFS_H
#define SDP_DEFS_H

/* Define the service attribute IDs.
*/
#define  ATTR_ID_SERVICE_RECORD_HDL             0x0000
#define  ATTR_ID_SERVICE_CLASS_ID_LIST          0x0001
#define  ATTR_ID_SERVICE_RECORD_STATE           0x0002
#define  ATTR_ID_SERVICE_ID                     0x0003
#define  ATTR_ID_PROTOCOL_DESC_LIST             0x0004
#define  ATTR_ID_BROWSE_GROUP_LIST              0x0005
#define  ATTR_ID_LANGUAGE_BASE_ATTR_ID_LIST     0x0006
#define  ATTR_ID_SERVICE_INFO_TIME_TO_LIVE      0x0007
#define  ATTR_ID_SERVICE_AVAILABILITY           0x0008
#define  ATTR_ID_BT_PROFILE_DESC_LIST           0x0009
#define  ATTR_ID_DOCUMENTATION_URL              0x000A
#define  ATTR_ID_CLIENT_EXE_URL                 0x000B
#define  ATTR_ID_ICON_URL                       0x000C
#define  ATTR_ID_ADDITION_PROTO_DESC_LISTS      0x000D

#define  LANGUAGE_BASE_ID                       0x0100
#define  ATTR_ID_SERVICE_NAME                   LANGUAGE_BASE_ID + 0x0000
#define  ATTR_ID_SERVICE_DESCRIPTION            LANGUAGE_BASE_ID + 0x0001
#define  ATTR_ID_PROVIDER_NAME                  LANGUAGE_BASE_ID + 0x0002

/* Device Identification (DI)
*/
#define ATTR_ID_SPECIFICATION_ID                0x0200
#define ATTR_ID_VENDOR_ID                       0x0201
#define ATTR_ID_PRODUCT_ID                      0x0202
#define ATTR_ID_PRODUCT_VERSION                 0x0203
#define ATTR_ID_PRIMARY_RECORD                  0x0204
#define ATTR_ID_VENDOR_ID_SOURCE                0x0205

#define BLUETOOTH_DI_SPECIFICATION              0x0103  /* 1.3 */
#define DI_VENDOR_ID_DEFAULT                    0xFFFF
#define DI_VENDOR_ID_SOURCE_BTSIG               0x0001
#define DI_VENDOR_ID_SOURCE_USBIF               0x0002


#define  ATTR_ID_IP_SUBNET                      0x0200 /* PAN Profile (***) */
#define  ATTR_ID_VERSION_NUMBER_LIST            0x0200
#define  ATTR_ID_GROUP_ID                       0x0200
#define  ATTR_ID_SERVICE_DATABASE_STATE         0x0201
#define  ATTR_ID_SERVICE_VERSION                0x0300
#define  ATTR_ID_HCRP_1284ID                    0x0300

#define  ATTR_ID_SUPPORTED_DATA_STORES          0x0301
#define  ATTR_ID_NETWORK				        0x0301
#define  ATTR_ID_EXTERNAL_NETWORK               0x0301
#define  ATTR_ID_FAX_CLASS_1_SUPPORT            0x0302
#define  ATTR_ID_REMOTE_AUDIO_VOLUME_CONTROL    0x0302
#define  ATTR_ID_DEVICE_NAME                    0x0302
#define  ATTR_ID_SUPPORTED_FORMATS_LIST         0x0303
#define  ATTR_ID_FAX_CLASS_2_0_SUPPORT          0x0303
#define  ATTR_ID_FAX_CLASS_2_SUPPORT            0x0304
#define  ATTR_ID_FRIENDLY_NAME                  0x0304
#define  ATTR_ID_AUDIO_FEEDBACK_SUPPORT         0x0305
#define  ATTR_ID_NETWORK_ADDRESS                0x0306
#define  ATTR_ID_DEVICE_LOCATION                0x0306
#define  ATTR_ID_WAP_GATEWAY                    0x0307
#define  ATTR_ID_HOME_PAGE_URL                  0x0308
#define  ATTR_ID_WAP_STACK_TYPE                 0x0309
#define  ATTR_ID_IMG_SUPPORTED_CAPABILITIES     0x0310 /* Imaging Profile */
#define  ATTR_ID_SUPPORTED_FEATURES             0x0311 /* HFP, BIP */
#define  ATTR_ID_IMG_SUPPORTED_FUNCTIONS        0x0312 /* Imaging Profile */
#define  ATTR_ID_IMG_TOT_DATA_CAPABILITY        0x0313 /* Imaging Profile */
#define  ATTR_ID_SUPPORTED_REPOSITORIES         0x0314 /* Phone book access Profile */
#define  ATTR_ID_MAS_INSTANCE_ID                0x0315 /* MAP profile */
#define  ATTR_ID_SUPPORTED_MSG_TYPE             0x0316 /* MAP profile */

/* These values are for the BPP profile */
#define ATTR_ID_DOCUMENT_FORMATS_SUPPORTED      0x0350
#define ATTR_ID_CHARACTER_REPERTOIRES_SUPPORTED 0x0352
#define ATTR_ID_XHTML_IMAGE_FORMATS_SUPPORTED   0x0354
#define ATTR_ID_COLOR_SUPPORTED                 0x0356
#define ATTR_ID_1284ID                          0x0358
#define ATTR_ID_PRINTER_NAME                    0x035A
#define ATTR_ID_PRINTER_LOCATION                0x035C
#define ATTR_ID_DUPLEX_SUPPORTED                0x035E
#define ATTR_ID_MEDIA_TYPES_SUPPORTED           0x0360
#define ATTR_ID_MAX_MEDIA_WIDTH                 0x0362
#define ATTR_ID_MAX_MEDIA_LENGTH                0x0364
#define ATTR_ID_ENHANCED_LAYOUT_SUPPORTED       0x0366
#define ATTR_ID_RUI_FORMATS_SUPPORTED           0x0368
#define ATTR_ID_RUI_REF_PRINTING_SUPPORTED      0x0370  /* Boolean */
#define ATTR_ID_RUI_DIRECT_PRINTING_SUPPORTED   0x0372  /* Boolean */
#define ATTR_ID_REF_PRINTING_TOP_URL            0x0374
#define ATTR_ID_DIRECT_PRINTING_TOP_URL         0x0376
#define ATTR_ID_PRINTER_ADMIN_RUI_TOP_URL       0x0378
#define ATTR_ID_BPP_DEVICE_NAME                 0x037A

/* These values are for the PAN profile */
#define  ATTR_ID_SECURITY_DESCRIPTION           0x030A
#define  ATTR_ID_NET_ACCESS_TYPE                0x030B
#define  ATTR_ID_MAX_NET_ACCESS_RATE            0x030C
#define  ATTR_ID_IPV4_SUBNET                    0x030D
#define  ATTR_ID_IPV6_SUBNET                    0x030E
#define  ATTR_ID_PAN_SECURITY                   0x0400

/* These values are for HID profile */
#define  ATTR_ID_HID_DEVICE_RELNUM          0x0200
#define  ATTR_ID_HID_PARSER_VERSION         0x0201
#define  ATTR_ID_HID_DEVICE_SUBCLASS        0x0202
#define  ATTR_ID_HID_COUNTRY_CODE           0x0203
#define  ATTR_ID_HID_VIRTUAL_CABLE          0x0204
#define  ATTR_ID_HID_RECONNECT_INITIATE     0x0205
#define  ATTR_ID_HID_DESCRIPTOR_LIST        0x0206
#define  ATTR_ID_HID_LANGUAGE_ID_BASE       0x0207
#define  ATTR_ID_HID_SDP_DISABLE            0x0208
#define  ATTR_ID_HID_BATTERY_POWER          0x0209
#define  ATTR_ID_HID_REMOTE_WAKE            0x020A
#define  ATTR_ID_HID_PROFILE_VERSION        0x020B
#define  ATTR_ID_HID_LINK_SUPERVISION_TO    0x020C
#define  ATTR_ID_HID_NORMALLY_CONNECTABLE   0x020D
#define  ATTR_ID_HID_BOOT_DEVICE            0x020E
#define  ATTR_ID_HID_SSR_HOST_MAX_LAT       0x020F
#define  ATTR_ID_HID_SSR_HOST_MIN_TOUT      0x0210

/* These values are for the HDP profile */
#define  ATTR_ID_HDP_SUP_FEAT_LIST          0x0200  /* Supported features list */
#define  ATTR_ID_HDP_DATA_EXCH_SPEC         0x0301  /* Data exchange specification */
#define  ATTR_ID_HDP_MCAP_SUP_PROC          0x0302  /* MCAP supported procedures */

/* a temporary value for IOP */
#ifndef ATTR_ID_OBX_OVR_L2CAP_PSM
#define  ATTR_ID_OBX_OVR_L2CAP_PSM          0x0200
#endif


/* Define common 16-bit protocol UUIDs
*/
#define UUID_PROTOCOL_SDP                    0x0001
#define UUID_PROTOCOL_UDP                    0x0002
#define UUID_PROTOCOL_RFCOMM                 0x0003
#define UUID_PROTOCOL_TCP                    0x0004
#define UUID_PROTOCOL_TCS_BIN                0x0005
#define UUID_PROTOCOL_TCS_AT                 0x0006
#define UUID_PROTOCOL_OBEX                   0x0008
#define UUID_PROTOCOL_IP                     0x0009
#define UUID_PROTOCOL_FTP                    0x000A
#define UUID_PROTOCOL_HTTP                   0x000C
#define UUID_PROTOCOL_WSP                    0x000E
#define UUID_PROTOCOL_BNEP                   0x000F
#define UUID_PROTOCOL_UPNP                   0x0010
#define UUID_PROTOCOL_HIDP                   0x0011
#define UUID_PROTOCOL_HCRP_CTRL              0x0012
#define UUID_PROTOCOL_HCRP_DATA              0x0014
#define UUID_PROTOCOL_HCRP_NOTIF             0x0016
#define UUID_PROTOCOL_AVCTP                  0x0017
#define UUID_PROTOCOL_AVDTP                  0x0019
#define UUID_PROTOCOL_CMTP                   0x001B
#define UUID_PROTOCOL_UDI                    0x001D
#define UUID_PROTOCOL_MCAP_CTRL              0x001E
#define UUID_PROTOCOL_MCAP_DATA              0x001F
#define UUID_PROTOCOL_L2CAP                  0x0100
#define UUID_PROTOCOL_ATT                    0x0007

/* Define common 16-bit service class UUIDs
*/
#define UUID_SERVCLASS_SERVICE_DISCOVERY_SERVER 0X1000
#define UUID_SERVCLASS_BROWSE_GROUP_DESCRIPTOR  0X1001
#define UUID_SERVCLASS_PUBLIC_BROWSE_GROUP      0X1002
#define UUID_SERVCLASS_SERIAL_PORT              0X1101
#define UUID_SERVCLASS_LAN_ACCESS_USING_PPP     0X1102
#define UUID_SERVCLASS_DIALUP_NETWORKING        0X1103
#define UUID_SERVCLASS_IRMC_SYNC                0X1104
#define UUID_SERVCLASS_OBEX_OBJECT_PUSH         0X1105
#define UUID_SERVCLASS_OBEX_FILE_TRANSFER       0X1106
#define UUID_SERVCLASS_IRMC_SYNC_COMMAND        0X1107
#define UUID_SERVCLASS_HEADSET                  0X1108
#define UUID_SERVCLASS_CORDLESS_TELEPHONY       0X1109
#define UUID_SERVCLASS_AUDIO_SOURCE             0X110A
#define UUID_SERVCLASS_AUDIO_SINK               0X110B
#define UUID_SERVCLASS_AV_REM_CTRL_TARGET       0X110C  /* Audio/Video Control profile */
#define UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION   0X110D  /* Advanced Audio Distribution profile */
#define UUID_SERVCLASS_AV_REMOTE_CONTROL        0X110E  /* Audio/Video Control profile */
#define UUID_SERVCLASS_AV_REM_CTRL_CONTROL      0X110F  /* Audio/Video Control profile */
#define UUID_SERVCLASS_INTERCOM                 0X1110
#define UUID_SERVCLASS_FAX                      0X1111
#define UUID_SERVCLASS_HEADSET_AUDIO_GATEWAY    0X1112
#define UUID_SERVCLASS_WAP                      0X1113
#define UUID_SERVCLASS_WAP_CLIENT               0X1114
#define UUID_SERVCLASS_PANU                     0X1115  /* PAN profile */
#define UUID_SERVCLASS_NAP                      0X1116  /* PAN profile */
#define UUID_SERVCLASS_GN                       0X1117  /* PAN profile */
#define UUID_SERVCLASS_DIRECT_PRINTING          0X1118  /* BPP profile */
#define UUID_SERVCLASS_REFERENCE_PRINTING       0X1119  /* BPP profile */
#define UUID_SERVCLASS_IMAGING                  0X111A  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_RESPONDER        0X111B  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_AUTO_ARCHIVE     0X111C  /* Imaging profile */
#define UUID_SERVCLASS_IMAGING_REF_OBJECTS      0X111D  /* Imaging profile */
#define UUID_SERVCLASS_HF_HANDSFREE             0X111E  /* Handsfree profile */
#define UUID_SERVCLASS_AG_HANDSFREE             0X111F  /* Handsfree profile */
#define UUID_SERVCLASS_DIR_PRT_REF_OBJ_SERVICE  0X1120  /* BPP profile */
#define UUID_SERVCLASS_REFLECTED_UI             0X1121  /* BPP profile */
#define UUID_SERVCLASS_BASIC_PRINTING           0X1122  /* BPP profile */
#define UUID_SERVCLASS_PRINTING_STATUS          0X1123  /* BPP profile */
#define UUID_SERVCLASS_HUMAN_INTERFACE          0X1124  /* HID profile */
#define UUID_SERVCLASS_CABLE_REPLACEMENT        0X1125  /* HCRP profile */
#define UUID_SERVCLASS_HCRP_PRINT               0X1126  /* HCRP profile */
#define UUID_SERVCLASS_HCRP_SCAN                0X1127  /* HCRP profile */
#define UUID_SERVCLASS_COMMON_ISDN_ACCESS       0X1128  /* CAPI Message Transport Protocol*/
#define UUID_SERVCLASS_VIDEO_CONFERENCING_GW    0X1129  /* Video Conferencing profile */
#define UUID_SERVCLASS_UDI_MT                   0X112A  /* Unrestricted Digital Information profile */
#define UUID_SERVCLASS_UDI_TA                   0X112B  /* Unrestricted Digital Information profile */
#define UUID_SERVCLASS_VCP                      0X112C  /* Video Conferencing profile */
#define UUID_SERVCLASS_SAP                      0X112D  /* SIM Access profile */
#define UUID_SERVCLASS_PBAP_PCE                 0X112E  /* Phonebook Access - PCE */
#define UUID_SERVCLASS_PBAP_PSE                 0X112F  /* Phonebook Access - PSE */
#define UUID_SERVCLASS_PHONE_ACCESS             0x1130
#define UUID_SERVCLASS_HEADSET_HS               0x1131  /* Headset - HS, from HSP v1.2 */
#define UUID_SERVCLASS_PNP_INFORMATION          0X1200  /* Device Identification */
#define UUID_SERVCLASS_GENERIC_NETWORKING       0X1201
#define UUID_SERVCLASS_GENERIC_FILETRANSFER     0X1202
#define UUID_SERVCLASS_GENERIC_AUDIO            0X1203
#define UUID_SERVCLASS_GENERIC_TELEPHONY        0X1204
#define UUID_SERVCLASS_UPNP_SERVICE             0X1205  /* UPNP_Service [ESDP] */
#define UUID_SERVCLASS_UPNP_IP_SERVICE          0X1206  /* UPNP_IP_Service [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_PAN         0X1300  /* UPNP_IP_PAN [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_LAP         0X1301  /* UPNP_IP_LAP [ESDP] */
#define UUID_SERVCLASS_ESDP_UPNP_IP_L2CAP       0X1302  /* UPNP_L2CAP [ESDP] */
#define UUID_SERVCLASS_VIDEO_SOURCE             0X1303  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_VIDEO_SINK               0X1304  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_VIDEO_DISTRIBUTION       0X1305  /* Video Distribution Profile (VDP) */
#define UUID_SERVCLASS_HDP_PROFILE              0X1400  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_HDP_SOURCE               0X1401  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_HDP_SINK                 0X1402  /* Health Device profile (HDP) */
#define UUID_SERVCLASS_MAP_PROFILE              0X1134  /* MAP profile UUID */
#define UUID_SERVCLASS_MESSAGE_ACCESS           0X1132  /* Message Access Service UUID */
#define UUID_SERVCLASS_MESSAGE_NOTIFICATION     0X1133  /* Message Notification Service UUID */

#define UUID_SERVCLASS_GAP_SERVER               0x1800
#define UUID_SERVCLASS_GATT_SERVER              0x1801
#define UUID_SERVCLASS_IMMEDIATE_ALERT          0x1802      /* immediate alert */
#define UUID_SERVCLASS_LINKLOSS                 0x1803      /* Link Loss Alert */
#define UUID_SERVCLASS_TX_POWER                 0x1804      /* TX power */
#define UUID_SERVCLASS_CURRENT_TIME             0x1805      /* Link Loss Alert */
#define UUID_SERVCLASS_DST_CHG                  0x1806      /* DST Time change */
#define UUID_SERVCLASS_REF_TIME_UPD             0x1807      /* reference time update */
#define UUID_SERVCLASS_DEVICE_INFO              0x180A      /* device info service */
#define UUID_SERVCLASS_NWA                      0x180B      /* Network availability */
#define UUID_SERVCLASS_PHALERT                  0x180C      /* phone alert service */
#define UUID_SERVCLASS_GLUCOSE                  0xC000      /* Glucose Meter Service */
#define UUID_SERVCLASS_TEST_SERVER              0x9000      /* Test Group UUID */

#if (BTM_WBS_INCLUDED == TRUE )
#define UUID_CODEC_CVSD                         0x0001   /* CVSD */
#define UUID_CODEC_MSBC                         0x0002   /* mSBC */
#endif

/* Define all the 'Descriptor Type' values.
*/
#define  NULL_DESC_TYPE                     0
#define  UINT_DESC_TYPE                     1
#define  TWO_COMP_INT_DESC_TYPE             2
#define  UUID_DESC_TYPE                     3
#define  TEXT_STR_DESC_TYPE                 4
#define  BOOLEAN_DESC_TYPE                  5
#define  DATA_ELE_SEQ_DESC_TYPE             6
#define  DATA_ELE_ALT_DESC_TYPE             7
#define  URL_DESC_TYPE                      8

/* Define all the "Descriptor Size" values.
*/
#define  SIZE_ONE_BYTE                0
#define  SIZE_TWO_BYTES               1
#define  SIZE_FOUR_BYTES              2
#define  SIZE_EIGHT_BYTES             3
#define  SIZE_SIXTEEN_BYTES           4
#define  SIZE_IN_NEXT_BYTE            5
#define  SIZE_IN_NEXT_WORD            6
#define  SIZE_IN_NEXT_LONG            7

/* Language Encoding Constants */
#define LANG_ID_CODE_ENGLISH            ((UINT16) 0x656e)   /* "en" */
#define LANG_ID_CHAR_ENCODE_UTF8        ((UINT16) 0x006a)   /* UTF-8 */

/* Constants used for display purposes only.  These define ovelapping attribute values */
#define  ATTR_ID_VERS_OR_GRP_OR_DRELNUM_OR_IPSUB_OR_SPECID  0x0200
#define  ATTR_ID_VEND_ID_OR_SERVICE_DB_STATE_OR_PARSE_VER   0x0201
#define  ATTR_ID_PROD_ID_OR_HID_DEV_SUBCLASS                0x0202
#define  ATTR_ID_PROD_VER_OR_HID_COUNTRY_CODE               0x0203
#define  ATTR_ID_PRIMARY_REC_OR_HID_VIRTUAL_CABLE           0x0204
#define  ATTR_ID_DI_VENDOR_ID_SOURCE_OR_HID_INIT_RECONNECT  0x0205
#define  ATTR_ID_SERV_VERS_OR_1284ID                        0x0300
#define  ATTR_ID_DATA_STORES_OR_NETWORK                     0x0301
#define  ATTR_ID_FAX_1_OR_AUD_VOL_OR_DEV_NAME               0x0302
#define  ATTR_ID_FORMATS_OR_FAX_2_0                         0x0303
#define  ATTR_ID_FAX_CLASS_2_OR_FRIENDLY_NAME               0x0304
#define  ATTR_ID_NETADDRESS_OR_DEVLOCATION                  0x0306

#endif
/* end "revolution/BTE/stack/include/sdpdefs.h" */

/*****************************************************************************
**  Constants
*****************************************************************************/

/* Success code and error codes */
#define  SDP_SUCCESS                        0x0000
#define  SDP_INVALID_VERSION                0x0001
#define  SDP_INVALID_SERV_REC_HDL           0x0002
#define  SDP_INVALID_REQ_SYNTAX             0x0003
#define  SDP_INVALID_PDU_SIZE               0x0004
#define  SDP_INVALID_CONT_STATE             0x0005
#define  SDP_NO_RESOURCES                   0x0006
#define  SDP_DI_REG_FAILED                  0x0007
#define  SDP_DI_DISC_FAILED                 0x0008
#define  SDP_NO_DI_RECORD_FOUND             0x0009
#define  SDP_ERR_ATTR_NOT_PRESENT           0x000A
#define  SDP_ILLEGAL_PARAMETER              0x000B

#define  SDP_NO_RECS_MATCH                  0xFFF0
#define  SDP_CONN_FAILED                    0xFFF1
#define  SDP_CFG_FAILED                     0xFFF2
#define  SDP_GENERIC_ERROR                  0xFFF3
#define  SDP_DB_FULL                        0xFFF4
#define  SDP_INVALID_PDU                    0xFFF5
#define  SDP_SECURITY_ERR                   0xFFF6
#define  SDP_CONN_REJECTED                  0xFFF7
#define  SDP_CANCEL                         0xFFF8

/* these result codes are used only when SDP_FOR_JV_INCLUDED==TRUE */
#define  SDP_EVT_OPEN                       0x00F0 /* connected */
#define  SDP_EVT_DATA_IND                   0x00F1 /* data ind */
#define  SDP_EVT_CLOSE                      0x00F2 /* disconnected */

/* Define the PSM that SDP uses */
#define SDP_PSM     0x0001

/* Legacy #define to avoid code changes - SDP UUID is same as BT UUID */
#define tSDP_UUID   tBT_UUID

/* Masks for attr_value field of tSDP_DISC_ATTR */
#define SDP_DISC_ATTR_LEN_MASK          0x0FFF
#define SDP_DISC_ATTR_TYPE(len_type)    (len_type >> 12)
#define SDP_DISC_ATTR_LEN(len_type)     (len_type & SDP_DISC_ATTR_LEN_MASK)

/* Maximum number of protocol list items (list_elem in tSDP_PROTOCOL_ELEM) */
#define SDP_MAX_LIST_ELEMS      3


/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/* Define a callback function for when discovery is complete. */
typedef void (tSDP_DISC_CMPL_CB) (UINT16 result);
typedef void (tSDP_DISC_CMPL_CB2) (UINT16 result, void* user_data);

typedef struct
{
    BD_ADDR         peer_addr;
    UINT16          peer_mtu;
} tSDP_DR_OPEN;

typedef struct
{
    UINT8           *p_data;
    UINT16          data_len;
} tSDP_DR_DATA;

typedef union
{
    tSDP_DR_OPEN    open;
    tSDP_DR_DATA    data;
} tSDP_DATA;

/* Define a callback function for when discovery result is received. */
typedef void (tSDP_DISC_RES_CB) (UINT16 event, tSDP_DATA *p_data);

/* Define a structure to hold the discovered service information. */
typedef struct
{
    union
    {
        UINT8       u8;                         /* 8-bit integer            */
        UINT16      u16;                        /* 16-bit integer           */
        UINT32      u32;                        /* 32-bit integer           */
        UINT8       array[4];                   /* Variable length field    */
        struct t_sdp_disc_attr *p_sub_attr;     /* Addr of first sub-attr (list)*/
    } v;

} tSDP_DISC_ATVAL;

typedef struct t_sdp_disc_attr
{
    struct t_sdp_disc_attr *p_next_attr;        /* Addr of next linked attr     */
    UINT16                  attr_id;            /* Attribute ID                 */
    UINT16                  attr_len_type;      /* Length and type fields       */
    tSDP_DISC_ATVAL         attr_value;         /* Variable length entry data   */
} tSDP_DISC_ATTR;

typedef struct t_sdp_disc_rec
{
    tSDP_DISC_ATTR          *p_first_attr;      /* First attribute of record    */
    struct t_sdp_disc_rec   *p_next_rec;        /* Addr of next linked record   */
    UINT32                  time_read;          /* The time the record was read */
    BD_ADDR                 remote_bd_addr;     /* Remote BD address            */
} tSDP_DISC_REC;

typedef struct
{
    UINT32          mem_size;                   /* Memory size of the DB        */
    UINT32          mem_free;                   /* Memory still available       */
    tSDP_DISC_REC   *p_first_rec;               /* Addr of first record in DB   */
    UINT16          num_uuid_filters;           /* Number of UUIds to filter    */
    tSDP_UUID       uuid_filters[SDP_MAX_UUID_FILTERS]; /* UUIDs to filter      */
    UINT16          num_attr_filters;           /* Number of attribute filters  */
    UINT16          attr_filters[SDP_MAX_ATTR_FILTERS]; /* Attributes to filter */
    UINT8           *p_free_mem;                /* Pointer to free memory       */
#if (SDP_RAW_DATA_INCLUDED == TRUE)
    UINT8           *raw_data;                  /* Received record from server. allocated/released by client  */
    UINT32          raw_size;                   /* size of raw_data */
    UINT32          raw_used;                   /* length of raw_data used */
#endif
}tSDP_DISCOVERY_DB;

/* This structure is used to add protocol lists and find protocol elements */
typedef struct
{
    UINT16      protocol_uuid;
    UINT16      num_params;
    UINT16      params[SDP_MAX_PROTOCOL_PARAMS];
} tSDP_PROTOCOL_ELEM;

typedef struct
{
    UINT16              num_elems;
    tSDP_PROTOCOL_ELEM  list_elem[SDP_MAX_LIST_ELEMS];
} tSDP_PROTO_LIST_ELEM;

/* Device Identification (DI) data structure
*/
/* Used to set the DI record */
typedef struct t_sdp_di_record
{
    UINT16       vendor;
    UINT16       vendor_id_source;
    UINT16       product;
    UINT16       version;
    BOOLEAN      primary_record;
    char         client_executable_url[SDP_MAX_ATTR_LEN];   /* optional */
    char         service_description[SDP_MAX_ATTR_LEN];     /* optional */
    char         documentation_url[SDP_MAX_ATTR_LEN];       /* optional */
}tSDP_DI_RECORD;

/* Used to get the DI record */
typedef struct t_sdp_di_get_record
{
    UINT16          spec_id;
    tSDP_DI_RECORD  rec;
}tSDP_DI_GET_RECORD;


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/* API into the SDP layer for service discovery. */

/*******************************************************************************
**
** Function         SDP_InitDiscoveryDb
**
** Description      This function is called to initialize a discovery database.
**
** Returns          TRUE if successful, FALSE if one or more parameters are bad
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_InitDiscoveryDb (tSDP_DISCOVERY_DB *p_db, UINT32 len,
                                            UINT16 num_uuid,
                                            tSDP_UUID *p_uuid_list,
                                            UINT16 num_attr,
                                            UINT16 *p_attr_list);

/*******************************************************************************
**
** Function         SDP_CancelServiceSearch
**
** Description      This function cancels an active query to an SDP server.
**
** Returns          TRUE if discovery cancelled, FALSE if a matching activity is not found.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_CancelServiceSearch (tSDP_DISCOVERY_DB *p_db);

/*******************************************************************************
**
** Function         SDP_ServiceSearchRequest
**
** Description      This function queries an SDP server for information.
**
** Returns          TRUE if discovery started, FALSE if failed.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_ServiceSearchRequest (UINT8 *p_bd_addr,
                                                 tSDP_DISCOVERY_DB *p_db,
                                                 tSDP_DISC_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         SDP_ServiceSearchAttributeRequest
**
** Description      This function queries an SDP server for information.
**
**                  The difference between this API function and the function
**                  SDP_ServiceSearchRequest is that this one does a
**                  combined ServiceSearchAttributeRequest SDP function.
**
** Returns          TRUE if discovery started, FALSE if failed.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_ServiceSearchAttributeRequest (UINT8 *p_bd_addr,
                                                          tSDP_DISCOVERY_DB *p_db,
                                                          tSDP_DISC_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         SDP_ServiceSearchAttributeRequest2
**
** Description      This function queries an SDP server for information.
**
**                  The difference between this API function and the function
**                  SDP_ServiceSearchRequest is that this one does a
**                  combined ServiceSearchAttributeRequest SDP function with the
**                  user data piggyback
**
** Returns          TRUE if discovery started, FALSE if failed.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_ServiceSearchAttributeRequest2 (UINT8 *p_bd_addr,
                                                          tSDP_DISCOVERY_DB *p_db,
                                                          tSDP_DISC_CMPL_CB2 *p_cb, void * user_data);

/* API of utilities to find data in the local discovery database */

/*******************************************************************************
**
** Function         SDP_FindAttributeInDb
**
** Description      This function queries an SDP database for a specific attribute.
**                  If the p_start_rec pointer is NULL, it looks from the beginning
**                  of the database, else it continues from the next record after
**                  p_start_rec.
**
** Returns          Pointer to matching record, or NULL
**
*******************************************************************************/
SDP_API extern tSDP_DISC_REC *SDP_FindAttributeInDb (tSDP_DISCOVERY_DB *p_db,
                                                     UINT16 attr_id,
                                                     tSDP_DISC_REC *p_start_rec);


/*******************************************************************************
**
** Function         SDP_FindAttributeInRec
**
** Description      This function searches an SDP discovery record for a
**                  specific attribute.
**
** Returns          Pointer to matching attribute entry, or NULL
**
*******************************************************************************/
SDP_API extern tSDP_DISC_ATTR *SDP_FindAttributeInRec (tSDP_DISC_REC *p_rec,
                                                       UINT16 attr_id);


/*******************************************************************************
**
** Function         SDP_FindServiceInDb
**
** Description      This function queries an SDP database for a specific service.
**                  If the p_start_rec pointer is NULL, it looks from the beginning
**                  of the database, else it continues from the next record after
**                  p_start_rec.
**
** Returns          Pointer to record containing service class, or NULL
**
*******************************************************************************/
SDP_API extern tSDP_DISC_REC *SDP_FindServiceInDb (tSDP_DISCOVERY_DB *p_db,
                                                   UINT16 service_uuid,
                                                   tSDP_DISC_REC *p_start_rec);


/*******************************************************************************
**
** Function         SDP_FindServiceUUIDInDb
**
** Description      This function queries an SDP database for a specific service.
**                  If the p_start_rec pointer is NULL, it looks from the beginning
**                  of the database, else it continues from the next record after
**                  p_start_rec.
**
** NOTE             the only difference between this function and the previous
**                  function "SDP_FindServiceInDb()" is that this function takes
**                  a tBT_UUID input.
**
** Returns          Pointer to record containing service class, or NULL
**
*******************************************************************************/
SDP_API extern tSDP_DISC_REC *SDP_FindServiceUUIDInDb (tSDP_DISCOVERY_DB *p_db,
                                                       tBT_UUID *p_uuid,
                                                       tSDP_DISC_REC *p_start_rec);

/*******************************************************************************
**
** Function         SDP_FindServiceUUIDInRec_128bit
**
** Description      This function is called to read the 128-bit service UUID within a record
**                  if there is any.
**
** Parameters:      p_rec      - pointer to a SDP record.
**                  p_uuid     - output parameter to save the UUID found.
**
** Returns          TRUE if found, otherwise FALSE.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_FindServiceUUIDInRec_128bit(tSDP_DISC_REC *p_rec, tBT_UUID * p_uuid);

/*******************************************************************************
**
** Function         SDP_FindServiceInDb_128bit
**
** Description      This function queries an SDP database for a specific service.
**                  If the p_start_rec pointer is NULL, it looks from the beginning
**                  of the database, else it continues from the next record after
**                  p_start_rec.
**
** Returns          Pointer to record containing service class, or NULL
**
*******************************************************************************/
SDP_API extern tSDP_DISC_REC *SDP_FindServiceInDb_128bit(tSDP_DISCOVERY_DB *p_db,
                                                       tSDP_DISC_REC *p_start_rec);

/*******************************************************************************
**
** Function         SDP_FindProtocolListElemInRec
**
** Description      This function looks at a specific discovery record for a
**                  protocol list element.
**
** Returns          TRUE if found, FALSE if not
**                  If found, the passed protocol list element is filled in.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_FindProtocolListElemInRec (tSDP_DISC_REC *p_rec,
                                                      UINT16 layer_uuid,
                                                      tSDP_PROTOCOL_ELEM *p_elem);


/*******************************************************************************
**
** Function         SDP_FindAddProtoListsElemInRec
**
** Description      This function looks at a specific discovery record for a
**                  protocol list element.
**
** Returns          TRUE if found, FALSE if not
**                  If found, the passed protocol list element is filled in.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_FindAddProtoListsElemInRec (tSDP_DISC_REC *p_rec,
                                                       UINT16 layer_uuid,
                                                       tSDP_PROTOCOL_ELEM *p_elem);


/*******************************************************************************
**
** Function         SDP_FindProfileVersionInRec
**
** Description      This function looks at a specific discovery record for the
**                  Profile list descriptor, and pulls out the version number.
**                  The version number consists of an 8-bit major version and
**                  an 8-bit minor version.
**
** Returns          TRUE if found, FALSE if not
**                  If found, the major and minor version numbers that were passed
**                  in are filled in.
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_FindProfileVersionInRec (tSDP_DISC_REC *p_rec,
                                                    UINT16 profile_uuid,
                                                    UINT16 *p_version);


/* API into SDP for local service database updates */

/*******************************************************************************
**
** Function         SDP_CreateRecord
**
** Description      This function is called to create a record in the database.
**                  This would be through the SDP database maintenance API. The
**                  record is created empty, teh application should then call
**                  "add_attribute" to add the record's attributes.
**
** Returns          Record handle if OK, else 0.
**
*******************************************************************************/
SDP_API extern UINT32 SDP_CreateRecord (void);


/*******************************************************************************
**
** Function         SDP_DeleteRecord
**
** Description      This function is called to add a record (or all records)
**                  from the database. This would be through the SDP database
**                  maintenance API.
**
**                  If a record handle of 0 is passed, all records are deleted.
**
** Returns          TRUE if succeeded, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_DeleteRecord (UINT32 handle);


/*******************************************************************************
**
** Function         SDP_ReadRecord
**
** Description      This function is called to get the raw data of the record
**                  with the given handle from the database.
**
** Returns          -1, if the record is not found.
**                  Otherwise, the offset (0 or 1) to start of data in p_data.
**
**                  The size of data copied into p_data is in *p_data_len.
**
*******************************************************************************/
SDP_API extern INT32 SDP_ReadRecord(UINT32 handle, UINT8 *p_data, INT32 *p_data_len);

/*******************************************************************************
**
** Function         SDP_AddAttribute
**
** Description      This function is called to add an attribute to a record.
**                  This would be through the SDP database maintenance API.
**                  If the attribute already exists in the record, it is replaced
**                  with the new value.
**
** NOTE             Attribute values must be passed as a Big Endian stream.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddAttribute (UINT32 handle, UINT16 attr_id,
                                         UINT8 attr_type, UINT32 attr_len,
                                         UINT8 *p_val);


/*******************************************************************************
**
** Function         SDP_AddSequence
**
** Description      This function is called to add a sequence to a record.
**                  This would be through the SDP database maintenance API.
**                  If the sequence already exists in the record, it is replaced
**                  with the new sequence.
**
** NOTE             Element values must be passed as a Big Endian stream.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddSequence (UINT32 handle,  UINT16 attr_id,
                                        UINT16 num_elem, UINT8 type[],
                                        UINT8 len[], UINT8 *p_val[]);


/*******************************************************************************
**
** Function         SDP_AddUuidSequence
**
** Description      This function is called to add a UUID sequence to a record.
**                  This would be through the SDP database maintenance API.
**                  If the sequence already exists in the record, it is replaced
**                  with the new sequence.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddUuidSequence (UINT32 handle,  UINT16 attr_id,
                                            UINT16 num_uuids, UINT16 *p_uuids);


/*******************************************************************************
**
** Function         SDP_AddProtocolList
**
** Description      This function is called to add a protocol descriptor list to
**                  a record. This would be through the SDP database maintenance API.
**                  If the protocol list already exists in the record, it is replaced
**                  with the new list.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddProtocolList (UINT32 handle, UINT16 num_elem,
                                            tSDP_PROTOCOL_ELEM *p_elem_list);


/*******************************************************************************
**
** Function         SDP_AddAdditionProtoLists
**
** Description      This function is called to add a protocol descriptor list to
**                  a record. This would be through the SDP database maintenance API.
**                  If the protocol list already exists in the record, it is replaced
**                  with the new list.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddAdditionProtoLists (UINT32 handle, UINT16 num_elem,
                                                  tSDP_PROTO_LIST_ELEM *p_proto_list);


/*******************************************************************************
**
** Function         SDP_AddProfileDescriptorList
**
** Description      This function is called to add a profile descriptor list to
**                  a record. This would be through the SDP database maintenance API.
**                  If the version already exists in the record, it is replaced
**                  with the new one.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddProfileDescriptorList (UINT32 handle,
                                                     UINT16 profile_uuid,
                                                     UINT16 version);


/*******************************************************************************
**
** Function         SDP_AddLanguageBaseAttrIDList
**
** Description      This function is called to add a language base attr list to
**                  a record. This would be through the SDP database maintenance API.
**                  If the version already exists in the record, it is replaced
**                  with the new one.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddLanguageBaseAttrIDList (UINT32 handle,
                                                      UINT16 lang, UINT16 char_enc,
                                                      UINT16 base_id);


/*******************************************************************************
**
** Function         SDP_AddServiceClassIdList
**
** Description      This function is called to add a service list to a record.
**                  This would be through the SDP database maintenance API.
**                  If the service list already exists in the record, it is replaced
**                  with the new list.
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_AddServiceClassIdList (UINT32 handle,
                                                  UINT16 num_services,
                                                  UINT16 *p_service_uuids);


/*******************************************************************************
**
** Function         SDP_DeleteAttribute
**
** Description      This function is called to delete an attribute from a record.
**                  This would be through the SDP database maintenance API.
**
** Returns          TRUE if deleted OK, else FALSE if not found
**
*******************************************************************************/
SDP_API extern BOOLEAN SDP_DeleteAttribute (UINT32 handle, UINT16 attr_id);


/* Device Identification APIs */

/*******************************************************************************
**
** Function         SDP_SetLocalDiRecord
**
** Description      This function adds a DI record to the local SDP database.
**
** Returns          Returns SDP_SUCCESS if record added successfully, else error
**
*******************************************************************************/
SDP_API extern UINT16 SDP_SetLocalDiRecord (tSDP_DI_RECORD *device_info,
                                            UINT32 *p_handle);

/*******************************************************************************
**
** Function         SDP_GetLocalDiRecord
**
** Description      This function adds a DI record to the local SDP database.
**
**                  Fills in the device information of the record
**                  p_handle - if p_handle == NULL, the primary record is returned
**
** Returns          Returns SDP_SUCCESS if record exists, else error
**
*******************************************************************************/
SDP_API extern UINT16 SDP_GetLocalDiRecord(tSDP_DI_GET_RECORD *p_device_info,
                                           UINT32 *p_handle );

/*******************************************************************************
**
** Function         SDP_DiDiscover
**
** Description      This function queries a remote device for DI information.
**
** Returns          SDP_SUCCESS if query started successfully, else error
**
*******************************************************************************/
SDP_API extern UINT16 SDP_DiDiscover (BD_ADDR remote_device,
                                      tSDP_DISCOVERY_DB *p_db, UINT32 len,
                                      tSDP_DISC_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         SDP_GetNumDiRecords
**
** Description      Searches specified database for DI records
**
** Returns          number of DI records found
**
*******************************************************************************/
SDP_API extern UINT8  SDP_GetNumDiRecords (tSDP_DISCOVERY_DB *p_db);


/*******************************************************************************
**
** Function         SDP_GetDiRecord
**
** Description      This function retrieves a remote device's DI record from
**                  the specified database.
**
** Returns          SDP_SUCCESS if record retrieved, else error
**
*******************************************************************************/
SDP_API extern UINT16 SDP_GetDiRecord (UINT8 getRecordIndex,
                                       tSDP_DI_GET_RECORD *device_info,
                                       tSDP_DISCOVERY_DB *p_db);


/*******************************************************************************
**
** Function         SDP_SetTraceLevel
**
** Description      This function sets the trace level for SDP. If called with
**                  a value of 0xFF, it simply reads the current trace level.
**
** Returns          the new (current) trace level
**
*******************************************************************************/
SDP_API extern UINT8 SDP_SetTraceLevel (UINT8 new_level);

/*******************************************************************************
**
** Function         SDP_ConnOpen
**
** Description      This function creates a connection to the SDP server on the
**                  given device.
**
** Returns          0, if failed to initiate connection. Otherwise, the handle.
**
*******************************************************************************/
SDP_API UINT32 SDP_ConnOpen (UINT8 *p_bd_addr, tSDP_DISC_RES_CB *p_rcb,
                                  tSDP_DISC_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         SDP_WriteData
**
** Description      This function sends data to the connected SDP server.
**
** Returns          TRUE if data is sent, FALSE if failed.
**
*******************************************************************************/
SDP_API BOOLEAN SDP_WriteData (UINT32 handle, BT_HDR  *p_msg);

/*******************************************************************************
**
** Function         SDP_ConnClose
**
** Description      This function is called to close a SDP connection.
**
** Parameters:      handle      - Handle of the connection returned by SDP_ConnOpen
**
** Returns          TRUE if connection is closed, FALSE if failed to find the handle.
**
*******************************************************************************/
SDP_API BOOLEAN SDP_ConnClose (UINT32 handle);

/*******************************************************************************
**
** Function         SDP_FindServiceUUIDInRec
**
** Description      This function is called to read the service UUID within a record
**                  if there is any.
**
** Parameters:      p_rec      - pointer to a SDP record.
**
** Returns          TRUE if found, otherwise FALSE.
**
*******************************************************************************/
SDP_API BOOLEAN SDP_FindServiceUUIDInRec(tSDP_DISC_REC *p_rec, tBT_UUID *p_uuid);

#ifdef __cplusplus
}
#endif

#endif  /* SDP_API_H */
/* end "revolution/BTE/stack/include/sdp_api.h" */
/* "libs/RVL_SDK/include/revolution/BTE/stack/include/btm_api.h" line 43 "revolution/BTE/stack/include/hcidefs.h" */
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

#ifndef HCIDEFS_H
#define HCIDEFS_H

#ifdef BRCM_VS
/* "libs/RVL_SDK/include/revolution/BTE/stack/include/hcidefs.h" line 24 "brcm_vs_include.h" */
/* end "brcm_vs_include.h" */
#endif

#define HCI_PROTO_VERSION     0x01      /* Version for BT spec 1.1          */
#define HCI_PROTO_VERSION_1_2 0x02      /* Version for BT spec 1.2          */
#define HCI_PROTO_VERSION_2_0 0x03      /* Version for BT spec 2.0          */
#define HCI_PROTO_VERSION_2_1 0x04      /* Version for BT spec 2.1 [Lisbon] */
#define HCI_PROTO_VERSION_3_0 0x05      /* Version for BT spec 3.0          */
#define HCI_PROTO_REVISION    0x000C    /* Current implementation version   */
/*
**  Definitions for HCI groups
*/
#define HCI_GRP_LINK_CONTROL_CMDS       (0x01 << 10)            /* 0x0400 */
#define HCI_GRP_LINK_POLICY_CMDS        (0x02 << 10)            /* 0x0800 */
#define HCI_GRP_HOST_CONT_BASEBAND_CMDS (0x03 << 10)            /* 0x0C00 */
#define HCI_GRP_INFORMATIONAL_PARAMS    (0x04 << 10)            /* 0x1000 */
#define HCI_GRP_STATUS_PARAMS           (0x05 << 10)            /* 0x1400 */
#define HCI_GRP_TESTING_CMDS            (0x06 << 10)            /* 0x1800 */

#define HCI_GRP_VENDOR_SPECIFIC         (0x3F << 10)            /* 0xFC00 */

/* Group occupies high 6 bits of the HCI command rest is opcode itself */
#define HCI_OGF(p)  (UINT8)((0xFC00 & (p)) >> 10)
#define HCI_OCF(p)  ( 0x3FF & (p))

/*
**  Defentions for Link Control Commands
*/
/* Following opcode is used only in command complete event for flow control */
#define HCI_COMMAND_NONE                0x0000

/* Commands of HCI_GRP_LINK_CONTROL_CMDS group */
#define HCI_INQUIRY                     (0x0001 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_INQUIRY_CANCEL              (0x0002 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_PERIODIC_INQUIRY_MODE       (0x0003 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_EXIT_PERIODIC_INQUIRY_MODE  (0x0004 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_CREATE_CONNECTION           (0x0005 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_DISCONNECT                  (0x0006 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_ADD_SCO_CONNECTION          (0x0007 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_CREATE_CONNECTION_CANCEL    (0x0008 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_ACCEPT_CONNECTION_REQUEST   (0x0009 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_REJECT_CONNECTION_REQUEST   (0x000A | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_LINK_KEY_REQUEST_REPLY      (0x000B | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_LINK_KEY_REQUEST_NEG_REPLY  (0x000C | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_PIN_CODE_REQUEST_REPLY      (0x000D | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_PIN_CODE_REQUEST_NEG_REPLY  (0x000E | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_CHANGE_CONN_PACKET_TYPE     (0x000F | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_AUTHENTICATION_REQUESTED    (0x0011 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_SET_CONN_ENCRYPTION         (0x0013 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_CHANGE_CONN_LINK_KEY        (0x0015 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_MASTER_LINK_KEY             (0x0017 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_RMT_NAME_REQUEST            (0x0019 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_RMT_NAME_REQUEST_CANCEL     (0x001A | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_READ_RMT_FEATURES           (0x001B | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_READ_RMT_EXT_FEATURES       (0x001C | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_READ_RMT_VERSION_INFO       (0x001D | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_READ_RMT_CLOCK_OFFSET       (0x001F | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_READ_LMP_HANDLE             (0x0020 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_SETUP_ESCO_CONNECTION       (0x0028 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_ACCEPT_ESCO_CONNECTION      (0x0029 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_REJECT_ESCO_CONNECTION      (0x002A | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_IO_CAPABILITY_RESPONSE      (0x002B | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_USER_CONF_REQUEST_REPLY     (0x002C | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_USER_CONF_VALUE_NEG_REPLY   (0x002D | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_USER_PASSKEY_REQ_REPLY      (0x002E | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_USER_PASSKEY_REQ_NEG_REPLY  (0x002F | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_REM_OOB_DATA_REQ_REPLY      (0x0030 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_REM_OOB_DATA_REQ_NEG_REPLY  (0x0033 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_IO_CAP_REQ_NEG_REPLY        (0x0034 | HCI_GRP_LINK_CONTROL_CMDS)

/* AMP HCI */
#define HCI_CREATE_PHYSICAL_LINK        (0x0035 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_ACCEPT_PHYSICAL_LINK        (0x0036 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_DISCONNECT_PHYSICAL_LINK    (0x0037 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_CREATE_LOGICAL_LINK         (0x0038 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_ACCEPT_LOGICAL_LINK         (0x0039 | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_DISCONNECT_LOGICAL_LINK     (0x003A | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_LOGICAL_LINK_CANCEL         (0x003B | HCI_GRP_LINK_CONTROL_CMDS)
#define HCI_FLOW_SPEC_MODIFY            (0x003C | HCI_GRP_LINK_CONTROL_CMDS)

#define HCI_LINK_CTRL_CMDS_FIRST        HCI_INQUIRY
#define HCI_LINK_CTRL_CMDS_LAST         HCI_FLOW_SPEC_MODIFY

/* Commands of HCI_GRP_LINK_POLICY_CMDS */
#define HCI_HOLD_MODE                   (0x0001 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_SNIFF_MODE                  (0x0003 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_EXIT_SNIFF_MODE             (0x0004 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_PARK_MODE                   (0x0005 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_EXIT_PARK_MODE              (0x0006 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_QOS_SETUP                   (0x0007 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_ROLE_DISCOVERY              (0x0009 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_SWITCH_ROLE                 (0x000B | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_READ_POLICY_SETTINGS        (0x000C | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_WRITE_POLICY_SETTINGS       (0x000D | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_READ_DEF_POLICY_SETTINGS    (0x000E | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_WRITE_DEF_POLICY_SETTINGS   (0x000F | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_FLOW_SPECIFICATION          (0x0010 | HCI_GRP_LINK_POLICY_CMDS)
#define HCI_SNIFF_SUB_RATE              (0x0011 | HCI_GRP_LINK_POLICY_CMDS)

#define HCI_LINK_POLICY_CMDS_FIRST      HCI_HOLD_MODE
#define HCI_LINK_POLICY_CMDS_LAST       HCI_SNIFF_SUB_RATE


/* Commands of HCI_GRP_HOST_CONT_BASEBAND_CMDS */
#define HCI_SET_EVENT_MASK              (0x0001 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_RESET                       (0x0003 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_SET_EVENT_FILTER            (0x0005 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_FLUSH                       (0x0008 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_PIN_TYPE               (0x0009 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_PIN_TYPE              (0x000A | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_CREATE_NEW_UNIT_KEY         (0x000B | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_STORED_LINK_KEY        (0x000D | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_STORED_LINK_KEY       (0x0011 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_DELETE_STORED_LINK_KEY      (0x0012 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_CHANGE_LOCAL_NAME           (0x0013 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_LOCAL_NAME             (0x0014 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_CONN_ACCEPT_TOUT       (0x0015 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_CONN_ACCEPT_TOUT      (0x0016 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_PAGE_TOUT              (0x0017 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_PAGE_TOUT             (0x0018 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_SCAN_ENABLE            (0x0019 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_SCAN_ENABLE           (0x001A | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_PAGESCAN_CFG           (0x001B | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_PAGESCAN_CFG          (0x001C | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_INQUIRYSCAN_CFG        (0x001D | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_INQUIRYSCAN_CFG       (0x001E | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_AUTHENTICATION_ENABLE  (0x001F | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_AUTHENTICATION_ENABLE (0x0020 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_ENCRYPTION_MODE        (0x0021 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_ENCRYPTION_MODE       (0x0022 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_CLASS_OF_DEVICE        (0x0023 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_CLASS_OF_DEVICE       (0x0024 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_VOICE_SETTINGS         (0x0025 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_VOICE_SETTINGS        (0x0026 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_AUTO_FLUSH_TOUT        (0x0027 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_AUTO_FLUSH_TOUT       (0x0028 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_NUM_BCAST_REXMITS      (0x0029 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_NUM_BCAST_REXMITS     (0x002A | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_HOLD_MODE_ACTIVITY     (0x002B | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_HOLD_MODE_ACTIVITY    (0x002C | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_TRANSMIT_POWER_LEVEL   (0x002D | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_SCO_FLOW_CTRL_ENABLE   (0x002E | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_SCO_FLOW_CTRL_ENABLE  (0x002F | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_SET_HC_TO_HOST_FLOW_CTRL    (0x0031 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_HOST_BUFFER_SIZE            (0x0033 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_HOST_NUM_PACKETS_DONE       (0x0035 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_LINK_SUPER_TOUT        (0x0036 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_LINK_SUPER_TOUT       (0x0037 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_NUM_SUPPORTED_IAC      (0x0038 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_CURRENT_IAC_LAP        (0x0039 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_CURRENT_IAC_LAP       (0x003A | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_PAGESCAN_PERIOD_MODE   (0x003B | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_PAGESCAN_PERIOD_MODE  (0x003C | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_PAGESCAN_MODE          (0x003D | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_PAGESCAN_MODE         (0x003E | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_SET_AFH_CHANNELS            (0x003F | HCI_GRP_HOST_CONT_BASEBAND_CMDS)

#define HCI_READ_INQSCAN_TYPE           (0x0042 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_INQSCAN_TYPE          (0x0043 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_INQUIRY_MODE           (0x0044 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_INQUIRY_MODE          (0x0045 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_PAGESCAN_TYPE          (0x0046 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_PAGESCAN_TYPE         (0x0047 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_AFH_ASSESSMENT_MODE    (0x0048 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_AFH_ASSESSMENT_MODE   (0x0049 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_EXT_INQ_RESPONSE       (0x0051 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_EXT_INQ_RESPONSE      (0x0052 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_REFRESH_ENCRYPTION_KEY      (0x0053 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_SIMPLE_PAIRING_MODE    (0x0055 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_SIMPLE_PAIRING_MODE   (0x0056 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_LOCAL_OOB_DATA         (0x0057 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_INQ_TX_POWER_LEVEL     (0x0058 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_INQ_TX_POWER_LEVEL    (0x0059 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_ERRONEOUS_DATA_RPT     (0x005A | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_ERRONEOUS_DATA_RPT    (0x005B | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_ENHANCED_FLUSH              (0x005F | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_SEND_KEYPRESS_NOTIF         (0x0060 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)


/* AMP HCI */
#define HCI_READ_LOGICAL_LINK_ACCEPT_TIMEOUT  (0x0061 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT (0x0062 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_SET_EVENT_MASK_PAGE_2             (0x0063 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_LOCATION_DATA                (0x0064 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_LOCATION_DATA               (0x0065 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_FLOW_CONTROL_MODE            (0x0066 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_FLOW_CONTROL_MODE           (0x0067 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_BE_FLUSH_TOUT                (0x0069 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_BE_FLUSH_TOUT               (0x006A | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_SHORT_RANGE_MODE                  (0x006B | HCI_GRP_HOST_CONT_BASEBAND_CMDS) /* 802.11 only */

#define HCI_CONT_BASEBAND_CMDS_FIRST    HCI_SET_EVENT_MASK
#define HCI_CONT_BASEBAND_CMDS_LAST     HCI_SHORT_RANGE_MODE


/* Commands of HCI_GRP_INFORMATIONAL_PARAMS group */
#define HCI_READ_LOCAL_VERSION_INFO     (0x0001 | HCI_GRP_INFORMATIONAL_PARAMS)
#define HCI_READ_LOCAL_SUPPORTED_CMDS   (0x0002 | HCI_GRP_INFORMATIONAL_PARAMS)
#define HCI_READ_LOCAL_FEATURES         (0x0003 | HCI_GRP_INFORMATIONAL_PARAMS)
#define HCI_READ_LOCAL_EXT_FEATURES     (0x0004 | HCI_GRP_INFORMATIONAL_PARAMS)
#define HCI_READ_BUFFER_SIZE            (0x0005 | HCI_GRP_INFORMATIONAL_PARAMS)
#define HCI_READ_COUNTRY_CODE           (0x0007 | HCI_GRP_INFORMATIONAL_PARAMS)
#define HCI_READ_BD_ADDR                (0x0009 | HCI_GRP_INFORMATIONAL_PARAMS)
#define HCI_READ_DATA_BLOCK_SIZE        (0x000A | HCI_GRP_INFORMATIONAL_PARAMS)

#define HCI_INFORMATIONAL_CMDS_FIRST    HCI_READ_LOCAL_VERSION_INFO
#define HCI_INFORMATIONAL_CMDS_LAST     HCI_READ_BD_ADDR


/* Commands of HCI_GRP_STATUS_PARAMS group */
#define HCI_READ_FAILED_CONTACT_COUNT   (0x0001 | HCI_GRP_STATUS_PARAMS)
#define HCI_RESET_FAILED_CONTACT_COUNT  (0x0002 | HCI_GRP_STATUS_PARAMS)
#define HCI_GET_LINK_QUALITY            (0x0003 | HCI_GRP_STATUS_PARAMS)
#define HCI_READ_RSSI                   (0x0005 | HCI_GRP_STATUS_PARAMS)
#define HCI_READ_AFH_CH_MAP             (0x0006 | HCI_GRP_STATUS_PARAMS)
#define HCI_READ_CLOCK                  (0x0007 | HCI_GRP_STATUS_PARAMS)
#define HCI_READ_ENCR_KEY_SIZE          (0x0008 | HCI_GRP_STATUS_PARAMS)

/* AMP HCI */
#define HCI_READ_LOCAL_AMP_INFO         (0x0009 | HCI_GRP_STATUS_PARAMS)
#define HCI_READ_LOCAL_AMP_ASSOC        (0x000A | HCI_GRP_STATUS_PARAMS)
#define HCI_WRITE_REMOTE_AMP_ASSOC      (0x000B | HCI_GRP_STATUS_PARAMS)

#define HCI_STATUS_PARAMS_CMDS_FIRST    HCI_READ_FAILED_CONTACT_COUNT
#define HCI_STATUS_PARAMS_CMDS_LAST     HCI_WRITE_REMOTE_AMP_ASSOC

/* Commands of HCI_GRP_TESTING_CMDS group */
#define HCI_READ_LOOPBACK_MODE          (0x0001 | HCI_GRP_TESTING_CMDS)
#define HCI_WRITE_LOOPBACK_MODE         (0x0002 | HCI_GRP_TESTING_CMDS)
#define HCI_ENABLE_DEV_UNDER_TEST_MODE  (0x0003 | HCI_GRP_TESTING_CMDS)
#define HCI_WRITE_SIMP_PAIR_DEBUG_MODE  (0x0004 | HCI_GRP_TESTING_CMDS)

/* AMP HCI */
#define HCI_ENABLE_AMP_RCVR_REPORTS     (0x0007 | HCI_GRP_TESTING_CMDS)
#define HCI_AMP_TEST_END                (0x0008 | HCI_GRP_TESTING_CMDS)
#define HCI_AMP_TEST                    (0x0009 | HCI_GRP_TESTING_CMDS)

#define HCI_TESTING_CMDS_FIRST          HCI_READ_LOOPBACK_MODE
#define HCI_TESTING_CMDS_LAST           HCI_AMP_TEST

#define HCI_VENDOR_CMDS_FIRST           0x0001
#define HCI_VENDOR_CMDS_LAST            0xFFFF
#define HCI_VSC_MULTI_AV_HANDLE         0x0AAA
#define HCI_VSC_BURST_MODE_HANDLE       0x0BBB

/* BLE HCI */
#define HCI_GRP_BLE_CMDS                (0x08 << 10)
/* Commands of BLE Controller setup and configuration */
#define HCI_BLE_SET_EVENT_MASK          (0x0001 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_READ_BUFFER_SIZE        (0x0002 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_READ_LOCAL_SPT_FEAT     (0x0003 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_LOCAL_SPT_FEAT    (0x0004 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_RANDOM_ADDR       (0x0005 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_ADV_PARAMS        (0x0006 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_READ_ADV_CHNL_TX_POWER  (0x0007 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_ADV_DATA          (0x0008 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_SCAN_RSP_DATA     (0x0009 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_ADV_ENABLE        (0x000A | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_SCAN_PARAMS       (0x000B | HCI_GRP_BLE_CMDS)
#define HCI_BLE_WRITE_SCAN_ENABLE       (0x000C | HCI_GRP_BLE_CMDS)
#define HCI_BLE_CREATE_LL_CONN          (0x000D | HCI_GRP_BLE_CMDS)
#define HCI_BLE_CREATE_CONN_CANCEL      (0x000E | HCI_GRP_BLE_CMDS)
#define HCI_BLE_READ_WHITE_LIST_SIZE    (0x000F | HCI_GRP_BLE_CMDS)
#define HCI_BLE_CLEAR_WHITE_LIST        (0x0010 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_ADD_WHITE_LIST          (0x0011 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_REMOVE_WHITE_LIST       (0x0012 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_UPD_LL_CONN_PARAMS      (0x0013 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_SET_HOST_CHNL_CLASS     (0x0014 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_READ_CHNL_MAP           (0x0015 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_READ_REMOTE_FEAT        (0x0016 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_ENCRYPT                 (0x0017 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_RAND                    (0x0018 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_START_ENC               (0x0019 | HCI_GRP_BLE_CMDS)
#define HCI_BLE_LTK_REQ_REPLY           (0x001A | HCI_GRP_BLE_CMDS)
#define HCI_BLE_LTK_REQ_NEG_REPLY       (0x001B | HCI_GRP_BLE_CMDS)
#define HCI_BLE_READ_SUPPORTED_STATES   (0x001C | HCI_GRP_BLE_CMDS)

#define HCI_BLE_RESET                   (0x0020 | HCI_GRP_BLE_CMDS)

/* LE supported states definition */
#define HCI_LE_ADV_STATE          0x00000001
#define HCI_LE_SCAN_STATE         0x00000002
#define HCI_LE_INIT_STATE         0x00000004
#define HCI_LE_CONN_SL_STATE      0x00000008
#define HCI_LE_ADV_SCAN_STATE     0x00000010
#define HCI_LE_ADV_INIT_STATE     0x00000020
#define HCI_LE_ADV_MA_STATE       0x00000040
#define HCI_LE_ADV_SL_STATE       0x00000080
#define HCI_LE_SCAN_INIT_STATE    0x00000100
#define HCI_LE_SCAN_MA_STATE      0x00000200
#define HCI_LE_SCAN_SL_STATE      0x00000400
#define HCI_LE_INIT_MA_STATE      0x00000800

/*
**  Definitions for HCI Events
*/
#define HCI_INQUIRY_COMP_EVT                0x01
#define HCI_INQUIRY_RESULT_EVT              0x02
#define HCI_CONNECTION_COMP_EVT             0x03
#define HCI_CONNECTION_REQUEST_EVT          0x04
#define HCI_DISCONNECTION_COMP_EVT          0x05
#define HCI_AUTHENTICATION_COMP_EVT         0x06
#define HCI_RMT_NAME_REQUEST_COMP_EVT       0x07
#define HCI_ENCRYPTION_CHANGE_EVT           0x08
#define HCI_CHANGE_CONN_LINK_KEY_EVT        0x09
#define HCI_MASTER_LINK_KEY_COMP_EVT        0x0A
#define HCI_READ_RMT_FEATURES_COMP_EVT      0x0B
#define HCI_READ_RMT_VERSION_COMP_EVT       0x0C
#define HCI_QOS_SETUP_COMP_EVT              0x0D
#define HCI_COMMAND_COMPLETE_EVT            0x0E
#define HCI_COMMAND_STATUS_EVT              0x0F
#define HCI_HARDWARE_ERROR_EVT              0x10
#define HCI_FLUSH_OCCURED_EVT               0x11
#define HCI_ROLE_CHANGE_EVT                 0x12
#define HCI_NUM_COMPL_DATA_PKTS_EVT         0x13
#define HCI_MODE_CHANGE_EVT                 0x14
#define HCI_RETURN_LINK_KEYS_EVT            0x15
#define HCI_PIN_CODE_REQUEST_EVT            0x16
#define HCI_LINK_KEY_REQUEST_EVT            0x17
#define HCI_LINK_KEY_NOTIFICATION_EVT       0x18
#define HCI_LOOPBACK_COMMAND_EVT            0x19
#define HCI_DATA_BUF_OVERFLOW_EVT           0x1A
#define HCI_MAX_SLOTS_CHANGED_EVT           0x1B
#define HCI_READ_CLOCK_OFF_COMP_EVT         0x1C
#define HCI_CONN_PKT_TYPE_CHANGE_EVT        0x1D
#define HCI_QOS_VIOLATION_EVT               0x1E
#define HCI_PAGE_SCAN_MODE_CHANGE_EVT       0x1F
#define HCI_PAGE_SCAN_REP_MODE_CHNG_EVT     0x20
#define HCI_FLOW_SPECIFICATION_COMP_EVT     0x21
#define HCI_INQUIRY_RSSI_RESULT_EVT         0x22
#define HCI_READ_RMT_EXT_FEATURES_COMP_EVT  0x23
#define HCI_ESCO_CONNECTION_COMP_EVT        0x2C
#define HCI_ESCO_CONNECTION_CHANGED_EVT     0x2D
#define HCI_SNIFF_SUB_RATE_EVT              0x2E
#define HCI_EXTENDED_INQUIRY_RESULT_EVT     0x2F
#define HCI_ENCRYPTION_KEY_REFRESH_COMP_EVT 0x30
#define HCI_IO_CAPABILITY_REQUEST_EVT       0x31
#define HCI_IO_CAPABILITY_RESPONSE_EVT      0x32
#define HCI_USER_CONFIRMATION_REQUEST_EVT   0x33
#define HCI_USER_PASSKEY_REQUEST_EVT        0x34
#define HCI_REMOTE_OOB_DATA_REQUEST_EVT     0x35
#define HCI_SIMPLE_PAIRING_COMPLETE_EVT     0x36
#define HCI_LINK_SUPER_TOUT_CHANGED_EVT     0x38
#define HCI_ENHANCED_FLUSH_COMPLETE_EVT     0x39
#define HCI_USER_PASSKEY_NOTIFY_EVT         0x3B
#define HCI_KEYPRESS_NOTIFY_EVT             0x3C
#define HCI_RMT_HOST_SUP_FEAT_NOTIFY_EVT    0x3D

/*#define HCI_GENERIC_AMP_LINK_KEY_NOTIF_EVT  0x3E Removed from spec */
#define HCI_PHYSICAL_LINK_COMP_EVT          0x40
#define HCI_CHANNEL_SELECTED_EVT            0x41
#define HCI_DISC_PHYSICAL_LINK_COMP_EVT     0x42
#define HCI_PHY_LINK_LOSS_EARLY_WARNING_EVT 0x43
#define HCI_PHY_LINK_RECOVERY_EVT           0x44
#define HCI_LOGICAL_LINK_COMP_EVT           0x45
#define HCI_DISC_LOGICAL_LINK_COMP_EVT      0x46
#define HCI_FLOW_SPEC_MODIFY_COMP_EVT       0x47
#define HCI_NUM_COMPL_DATA_BLOCKS_EVT       0x48
#define HCI_SHORT_RANGE_MODE_COMPLETE_EVT   0x4C
#define HCI_AMP_STATUS_CHANGE_EVT           0x4D

/* ULP HCI Event */
#define HCI_BLE_EVENT                   0x03E
/* ULP Event sub code */
#define HCI_BLE_CONN_COMPLETE_EVT           0x01
#define HCI_BLE_ADV_PKT_RPT_EVT             0x02
#define HCI_BLE_LL_CONN_PARAM_UPD_EVT       0x03
#define HCI_BLE_READ_REMOTE_FEAT_CMPL_EVT   0x04
#define HCI_BLE_LTK_REQ_EVT                 0x05

#define HCI_EVENT_RSP_FIRST             HCI_INQUIRY_COMP_EVT
#define HCI_EVENT_RSP_LAST              HCI_AMP_STATUS_CHANGE_EVT

#define HCI_VENDOR_SPECIFIC_EVT         0xFF  /* Vendor specific events */
#define HCI_NAP_TRACE_EVT               0xFF  /* was define 0xFE, 0xFD, change to 0xFF
                                                 because conflict w/ TCI_EVT and per
                                                 specification compliant */



/*
**  Defentions for HCI Error Codes that are past in the events
*/
#define HCI_SUCCESS                                     0x00
#define HCI_PENDING                                     0x00
#define HCI_ERR_ILLEGAL_COMMAND                         0x01
#define HCI_ERR_NO_CONNECTION                           0x02
#define HCI_ERR_HW_FAILURE                              0x03
#define HCI_ERR_PAGE_TIMEOUT                            0x04
#define HCI_ERR_AUTH_FAILURE                            0x05
#define HCI_ERR_KEY_MISSING                             0x06
#define HCI_ERR_MEMORY_FULL                             0x07
#define HCI_ERR_CONNECTION_TOUT                         0x08
#define HCI_ERR_MAX_NUM_OF_CONNECTIONS                  0x09
#define HCI_ERR_MAX_NUM_OF_SCOS                         0x0A
#define HCI_ERR_CONNECTION_EXISTS                       0x0B
#define HCI_ERR_COMMAND_DISALLOWED                      0x0C
#define HCI_ERR_HOST_REJECT_RESOURCES                   0x0D
#define HCI_ERR_HOST_REJECT_SECURITY                    0x0E
#define HCI_ERR_HOST_REJECT_DEVICE                      0x0F
#define HCI_ERR_HOST_TIMEOUT                            0x10
#define HCI_ERR_UNSUPPORTED_VALUE                       0x11
#define HCI_ERR_ILLEGAL_PARAMETER_FMT                   0x12
#define HCI_ERR_PEER_USER                               0x13
#define HCI_ERR_PEER_LOW_RESOURCES                      0x14
#define HCI_ERR_PEER_POWER_OFF                          0x15
#define HCI_ERR_CONN_CAUSE_LOCAL_HOST                   0x16
#define HCI_ERR_REPEATED_ATTEMPTS                       0x17
#define HCI_ERR_PAIRING_NOT_ALLOWED                     0x18
#define HCI_ERR_UNKNOWN_LMP_PDU                         0x19
#define HCI_ERR_UNSUPPORTED_REM_FEATURE                 0x1A
#define HCI_ERR_SCO_OFFSET_REJECTED                     0x1B
#define HCI_ERR_SCO_INTERVAL_REJECTED                   0x1C
#define HCI_ERR_SCO_AIR_MODE                            0x1D
#define HCI_ERR_INVALID_LMP_PARAM                       0x1E
#define HCI_ERR_UNSPECIFIED                             0x1F
#define HCI_ERR_UNSUPPORTED_LMP_FEATURE                 0x20
#define HCI_ERR_ROLE_CHANGE_NOT_ALLOWED                 0x21
#define HCI_ERR_LMP_RESPONSE_TIMEOUT                    0x22
#define HCI_ERR_LMP_ERR_TRANS_COLLISION                 0x23
#define HCI_ERR_LMP_PDU_NOT_ALLOWED                     0x24
#define HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE               0x25
#define HCI_ERR_UNIT_KEY_USED                           0x26
#define HCI_ERR_QOS_NOT_SUPPORTED                       0x27
#define HCI_ERR_INSTANT_PASSED                          0x28
#define HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED     0x29
#define HCI_ERR_DIFF_TRANSACTION_COLLISION              0x2A
#define HCI_ERR_UNDEFINED_0x2B                          0x2B
#define HCI_ERR_QOS_UNACCEPTABLE_PARAM                  0x2C
#define HCI_ERR_QOS_REJECTED                            0x2D
#define HCI_ERR_CHAN_CLASSIF_NOT_SUPPORTED              0x2E
#define HCI_ERR_INSUFFCIENT_SECURITY                    0x2F
#define HCI_ERR_PARAM_OUT_OF_RANGE                      0x30
#define HCI_ERR_UNDEFINED_0x31                          0x31
#define HCI_ERR_ROLE_SWITCH_PENDING                     0x32
#define HCI_ERR_UNDEFINED_0x33                          0x33
#define HCI_ERR_RESERVED_SLOT_VIOLATION                 0x34
#define HCI_ERR_ROLE_SWITCH_FAILED                      0x35
#define HCI_ERR_INQ_RSP_DATA_TOO_LARGE                  0x36
#define HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED            0x37
#define HCI_ERR_HOST_BUSY_PAIRING                       0x38
#define HCI_ERR_REJ_NO_SUITABLE_CHANNEL                 0x39
#define HCI_ERR_CONTROLLER_BUSY                         0x3A
#define HCI_ERR_UNACCEPT_CONN_INTERVAL                  0x3B
#define HCI_ERR_DIRECTED_ADVERTISING_TIMEOUT            0x3C
#define HCI_ERR_CONN_TOUT_DUE_TO_MIC_FAILURE            0x3D
#define HCI_ERR_CONN_FAILED_ESTABLISHMENT               0x3E
#define HCI_ERR_MAC_CONNECTION_FAILED                   0x3F

#define HCI_ERR_MAX_ERR                                 0x40

#define HCI_HINT_TO_RECREATE_AMP_PHYS_LINK              0xFF

/*
** Definitions for HCI enable event
*/
#define HCI_INQUIRY_COMPLETE_EV(p)          (*((UINT32 *)(p)) & 0x00000001)
#define HCI_INQUIRY_RESULT_EV(p)            (*((UINT32 *)(p)) & 0x00000002)
#define HCI_CONNECTION_COMPLETE_EV(p)       (*((UINT32 *)(p)) & 0x00000004)
#define HCI_CONNECTION_REQUEST_EV(p)        (*((UINT32 *)(p)) & 0x00000008)
#define HCI_DISCONNECTION_COMPLETE_EV(p)    (*((UINT32 *)(p)) & 0x00000010)
#define HCI_AUTHENTICATION_COMPLETE_EV(p)   (*((UINT32 *)(p)) & 0x00000020)
#define HCI_RMT_NAME_REQUEST_COMPL_EV(p)    (*((UINT32 *)(p)) & 0x00000040)
#define HCI_CHANGE_CONN_ENCRPT_ENABLE_EV(p) (*((UINT32 *)(p)) & 0x00000080)
#define HCI_CHANGE_CONN_LINK_KEY_EV(p)      (*((UINT32 *)(p)) & 0x00000100)
#define HCI_MASTER_LINK_KEY_COMPLETE_EV(p)  (*((UINT32 *)(p)) & 0x00000200)
#define HCI_READ_RMT_FEATURES_COMPL_EV(p)   (*((UINT32 *)(p)) & 0x00000400)
#define HCI_READ_RMT_VERSION_COMPL_EV(p)    (*((UINT32 *)(p)) & 0x00000800)
#define HCI_QOS_SETUP_COMPLETE_EV(p)        (*((UINT32 *)(p)) & 0x00001000)
#define HCI_COMMAND_COMPLETE_EV(p)          (*((UINT32 *)(p)) & 0x00002000)
#define HCI_COMMAND_STATUS_EV(p)            (*((UINT32 *)(p)) & 0x00004000)
#define HCI_HARDWARE_ERROR_EV(p)            (*((UINT32 *)(p)) & 0x00008000)
#define HCI_FLASH_OCCURED_EV(p)             (*((UINT32 *)(p)) & 0x00010000)
#define HCI_ROLE_CHANGE_EV(p)               (*((UINT32 *)(p)) & 0x00020000)
#define HCI_NUM_COMPLETED_PKTS_EV(p)        (*((UINT32 *)(p)) & 0x00040000)
#define HCI_MODE_CHANGE_EV(p)               (*((UINT32 *)(p)) & 0x00080000)
#define HCI_RETURN_LINK_KEYS_EV(p)          (*((UINT32 *)(p)) & 0x00100000)
#define HCI_PIN_CODE_REQUEST_EV(p)          (*((UINT32 *)(p)) & 0x00200000)
#define HCI_LINK_KEY_REQUEST_EV(p)          (*((UINT32 *)(p)) & 0x00400000)
#define HCI_LINK_KEY_NOTIFICATION_EV(p)     (*((UINT32 *)(p)) & 0x00800000)
#define HCI_LOOPBACK_COMMAND_EV(p)          (*((UINT32 *)(p)) & 0x01000000)
#define HCI_DATA_BUF_OVERFLOW_EV(p)         (*((UINT32 *)(p)) & 0x02000000)
#define HCI_MAX_SLOTS_CHANGE_EV(p)          (*((UINT32 *)(p)) & 0x04000000)
#define HCI_READ_CLOCK_OFFSET_COMP_EV(p)    (*((UINT32 *)(p)) & 0x08000000)
#define HCI_CONN_PKT_TYPE_CHANGED_EV(p)     (*((UINT32 *)(p)) & 0x10000000)
#define HCI_QOS_VIOLATION_EV(p)             (*((UINT32 *)(p)) & 0x20000000)
#define HCI_PAGE_SCAN_MODE_CHANGED_EV(p)    (*((UINT32 *)(p)) & 0x40000000)
#define HCI_PAGE_SCAN_REP_MODE_CHNG_EV(p)   (*((UINT32 *)(p)) & 0x80000000)

/* the default event mask for 2.1+EDR (Lisbon) does not include Lisbon events */
#define HCI_DEFAULT_EVENT_MASK_0            0xFFFFFFFF
#define HCI_DEFAULT_EVENT_MASK_1            0x00001FFF

/* the event mask for 2.0 + EDR and later (includes Lisbon events) */
#define HCI_LISBON_EVENT_MASK_0             0xFFFFFFFF
#define HCI_LISBON_EVENT_MASK_1             0x1DBFFFFF
#define HCI_LISBON_EVENT_MASK               "\x0D\xBF\xFF\xFF\xFF\xFF\xFF\xFF"
#define HCI_LISBON_EVENT_MASK_EXT           "\x1D\xBF\xFF\xFF\xFF\xFF\xFF\xFF"
#define HCI_DUMO_EVENT_MASK_EXT             "\x3D\xBF\xFF\xFF\xFF\xFF\xFF\xFF"
/*  0x00001FFF FFFFFFFF Default - no Lisbon events
    0x00000800 00000000 Synchronous Connection Complete Event
    0x00001000 00000000 Synchronous Connection Changed Event
    0x00002000 00000000 Sniff Subrate Event
    0x00004000 00000000 Extended Inquiry Result Event
    0x00008000 00000000 Encryption Key Refresh Complete Event
    0x00010000 00000000 IO Capability Request Event
    0x00020000 00000000 IO Capability Response Event
    0x00040000 00000000 User Confirmation Request Event
    0x00080000 00000000 User Passkey Request Event
    0x00100000 00000000 Remote OOB Data Request Event
    0x00200000 00000000 Simple Pairing Complete Event
    0x00400000 00000000 Generic AMP Link Key Notification Event
    0x00800000 00000000 Link Supervision Timeout Changed Event
    0x01000000 00000000 Enhanced Flush Complete Event
    0x04000000 00000000 User Passkey Notification Event
    0x08000000 00000000 Keypress Notification Event
    0x10000000 00000000 Remote Host Supported Features Notification Event
    0x20000000 00000000 LE Meta Event
 */


/* the event mask for AMP controllers */
#define HCI_AMP_EVENT_MASK_3_0               "\x00\x00\x00\x00\x00\x00\x3F\xFF"

/*  0x0000000000000000 No events specified (default)
    0x0000000000000001 Physical Link Complete Event
    0x0000000000000002 Channel Selected Event
    0x0000000000000004 Disconnection Physical Link Event
    0x0000000000000008 Physical Link Loss Early Warning Event
    0x0000000000000010 Physical Link Recovery Event
    0x0000000000000020 Logical Link Complete Event
    0x0000000000000040 Disconnection Logical Link Complete Event
    0x0000000000000080 Flow Spec Modify Complete Event
    0x0000000000000100 Number of Completed Data Blocks Event
    0x0000000000000200 AMP Start Test Event
    0x0000000000000400 AMP Test End Event
    0x0000000000000800 AMP Receiver Report Event
    0x0000000000001000 Short Range Mode Change Complete Event
    0x0000000000002000 AMP Status Change Event
*/


/*
** Definitions for packet type masks (BT1.2 and BT2.0 definitions)
*/
#define HCI_PKT_TYPES_MASK_NO_2_DH1         0x0002
#define HCI_PKT_TYPES_MASK_NO_3_DH1         0x0004
#define HCI_PKT_TYPES_MASK_DM1              0x0008
#define HCI_PKT_TYPES_MASK_DH1              0x0010
#define HCI_PKT_TYPES_MASK_HV1              0x0020
#define HCI_PKT_TYPES_MASK_HV2              0x0040
#define HCI_PKT_TYPES_MASK_HV3              0x0080
#define HCI_PKT_TYPES_MASK_NO_2_DH3         0x0100
#define HCI_PKT_TYPES_MASK_NO_3_DH3         0x0200
#define HCI_PKT_TYPES_MASK_DM3              0x0400
#define HCI_PKT_TYPES_MASK_DH3              0x0800
#define HCI_PKT_TYPES_MASK_NO_2_DH5         0x1000
#define HCI_PKT_TYPES_MASK_NO_3_DH5         0x2000
#define HCI_PKT_TYPES_MASK_DM5              0x4000
#define HCI_PKT_TYPES_MASK_DH5              0x8000

/* Packet type should be one of valid but at least one should be specified */
#define HCI_VALID_SCO_PKT_TYPE(t) (((((t) & ~(HCI_PKT_TYPES_MASK_HV1       \
                                           |  HCI_PKT_TYPES_MASK_HV2       \
                                           |  HCI_PKT_TYPES_MASK_HV3)) == 0)) \
                                    && ((t) != 0))





/* Packet type should not be invalid and at least one should be specified */
#define HCI_VALID_ACL_PKT_TYPE(t) (((((t) & ~(HCI_PKT_TYPES_MASK_DM1        \
                                           |  HCI_PKT_TYPES_MASK_DH1        \
                                           |  HCI_PKT_TYPES_MASK_DM3        \
                                           |  HCI_PKT_TYPES_MASK_DH3        \
                                           |  HCI_PKT_TYPES_MASK_DM5        \
                                           |  HCI_PKT_TYPES_MASK_DH5        \
                                           |  HCI_PKT_TYPES_MASK_NO_2_DH1   \
                                           |  HCI_PKT_TYPES_MASK_NO_3_DH1   \
                                           |  HCI_PKT_TYPES_MASK_NO_2_DH3   \
                                           |  HCI_PKT_TYPES_MASK_NO_3_DH3   \
                                           |  HCI_PKT_TYPES_MASK_NO_2_DH5   \
                                           |  HCI_PKT_TYPES_MASK_NO_3_DH5  )) == 0)) \
                                    && (((t) &  (HCI_PKT_TYPES_MASK_DM1        \
                                              |  HCI_PKT_TYPES_MASK_DH1        \
                                              |  HCI_PKT_TYPES_MASK_DM3        \
                                              |  HCI_PKT_TYPES_MASK_DH3        \
                                              |  HCI_PKT_TYPES_MASK_DM5        \
                                              |  HCI_PKT_TYPES_MASK_DH5)) != 0))

/*
** Definitions for eSCO packet type masks (BT1.2 and BT2.0 definitions)
*/
#define HCI_ESCO_PKT_TYPES_MASK_HV1         0x0001
#define HCI_ESCO_PKT_TYPES_MASK_HV2         0x0002
#define HCI_ESCO_PKT_TYPES_MASK_HV3         0x0004
#define HCI_ESCO_PKT_TYPES_MASK_EV3         0x0008
#define HCI_ESCO_PKT_TYPES_MASK_EV4         0x0010
#define HCI_ESCO_PKT_TYPES_MASK_EV5         0x0020
#define HCI_ESCO_PKT_TYPES_MASK_NO_2_EV3    0x0040
#define HCI_ESCO_PKT_TYPES_MASK_NO_3_EV3    0x0080
#define HCI_ESCO_PKT_TYPES_MASK_NO_2_EV5    0x0100
#define HCI_ESCO_PKT_TYPES_MASK_NO_3_EV5    0x0200

/* Packet type should be one of valid but at least one should be specified for 1.2 */
#define HCI_VALID_ESCO_PKT_TYPE(t) (((((t) & ~(HCI_ESCO_PKT_TYPES_MASK_EV3       \
                                           |   HCI_ESCO_PKT_TYPES_MASK_EV4       \
                                           |   HCI_ESCO_PKT_TYPES_MASK_EV5)) == 0)) \
                                    && ((t) != 0))/* Packet type should be one of valid but at least one should be specified */

#define HCI_VALID_ESCO_SCOPKT_TYPE(t) (((((t) & ~(HCI_ESCO_PKT_TYPES_MASK_HV1       \
                                           |      HCI_ESCO_PKT_TYPES_MASK_HV2       \
                                           |      HCI_ESCO_PKT_TYPES_MASK_HV3)) == 0)) \
                                    && ((t) != 0))

#define HCI_VALID_SCO_ALL_PKT_TYPE(t) (((((t) & ~(HCI_ESCO_PKT_TYPES_MASK_HV1       \
                                           |      HCI_ESCO_PKT_TYPES_MASK_HV2       \
                                           |      HCI_ESCO_PKT_TYPES_MASK_HV3       \
                                           |      HCI_ESCO_PKT_TYPES_MASK_EV3       \
                                           |      HCI_ESCO_PKT_TYPES_MASK_EV4       \
                                           |      HCI_ESCO_PKT_TYPES_MASK_EV5)) == 0)) \
                                    && ((t) != 0))

/*
** Define parameters to allow role switch during create connection
*/
#define HCI_CR_CONN_NOT_ALLOW_SWITCH    0x00
#define HCI_CR_CONN_ALLOW_SWITCH        0x01

/*
** Hold Mode command destination
*/
#define HOLD_MODE_DEST_LOCAL_DEVICE     0x00
#define HOLD_MODE_DEST_RMT_DEVICE       0x01

/*
**  Definitions for different HCI parameters
*/
#define HCI_PER_INQ_MIN_MAX_PERIOD      0x0003
#define HCI_PER_INQ_MAX_MAX_PERIOD      0xFFFF
#define HCI_PER_INQ_MIN_MIN_PERIOD      0x0002
#define HCI_PER_INQ_MAX_MIN_PERIOD      0xFFFE

#define HCI_MAX_INQUIRY_LENGTH          0x30

#define HCI_MIN_INQ_LAP                 0x9E8B00
#define HCI_MAX_INQ_LAP                 0x9E8B3F

/* HCI role defenitions */
#define HCI_ROLE_MASTER                 0x00
#define HCI_ROLE_SLAVE                  0x01
#define HCI_ROLE_UNKNOWN                0xff

/* HCI mode defenitions */
#define HCI_MODE_ACTIVE                 0x00
#define HCI_MODE_HOLD                   0x01
#define HCI_MODE_SNIFF                  0x02
#define HCI_MODE_PARK                   0x03

/* HCI Flow Control Mode defenitions */
#define HCI_PACKET_BASED_FC_MODE        0x00
#define HCI_BLOCK_BASED_FC_MODE         0x01

/* Define Packet types as requested by the Host */
#define HCI_ACL_PKT_TYPE_NONE           0x0000
#define HCI_ACL_PKT_TYPE_DM1            0x0008
#define HCI_ACL_PKT_TYPE_DH1            0x0010
#define HCI_ACL_PKT_TYPE_AUX1           0x0200
#define HCI_ACL_PKT_TYPE_DM3            0x0400
#define HCI_ACL_PKT_TYPE_DH3            0x0800
#define HCI_ACL_PKT_TYPE_DM5            0x4000
#define HCI_ACL_PKT_TYPE_DH5            0x8000

/* Define key type in the Master Link Key command */
#define HCI_USE_SEMI_PERMANENT_KEY      0x00
#define HCI_USE_TEMPORARY_KEY           0x01

/* Page scan period modes */
#define HCI_PAGE_SCAN_REP_MODE_R0       0x00
#define HCI_PAGE_SCAN_REP_MODE_R1       0x01
#define HCI_PAGE_SCAN_REP_MODE_R2       0x02

/* Define limits for page scan repetition modes */
#define HCI_PAGE_SCAN_R1_LIMIT          0x0800
#define HCI_PAGE_SCAN_R2_LIMIT          0x1000

/* Page scan period modes */
#define HCI_PAGE_SCAN_PER_MODE_P0       0x00
#define HCI_PAGE_SCAN_PER_MODE_P1       0x01
#define HCI_PAGE_SCAN_PER_MODE_P2       0x02

/* Page scan modes */
#define HCI_MANDATARY_PAGE_SCAN_MODE    0x00
#define HCI_OPTIONAL_PAGE_SCAN_MODE1    0x01
#define HCI_OPTIONAL_PAGE_SCAN_MODE2    0x02
#define HCI_OPTIONAL_PAGE_SCAN_MODE3    0x03

/* Page and inquiry scan types */
#define HCI_SCAN_TYPE_STANDARD          0x00
#define HCI_SCAN_TYPE_INTERLACED        0x01       /* 1.2 devices or later */
#define HCI_DEF_SCAN_TYPE               HCI_SCAN_TYPE_STANDARD

/* Definitions for quality of service service types */
#define HCI_SERVICE_NO_TRAFFIC          0x00
#define HCI_SERVICE_BEST_EFFORT         0x01
#define HCI_SERVICE_GUARANTEED          0x02

#define HCI_QOS_LATENCY_DO_NOT_CARE     0xFFFFFFFF
#define HCI_QOS_DELAY_DO_NOT_CARE       0xFFFFFFFF

/* Definitions for Flow Specification */
#define HCI_FLOW_SPEC_LATENCY_DO_NOT_CARE 0xFFFFFFFF

/* Definitions for AFH Channel Map */
#define HCI_AFH_CHANNEL_MAP_LEN         10

/* Definitions for Extended Inquiry Response */
#define HCI_EXT_INQ_RESPONSE_LEN        240
#define HCI_EIR_FLAGS_TYPE                   BT_EIR_FLAGS_TYPE
#define HCI_EIR_MORE_16BITS_UUID_TYPE        BT_EIR_MORE_16BITS_UUID_TYPE
#define HCI_EIR_COMPLETE_16BITS_UUID_TYPE    BT_EIR_COMPLETE_16BITS_UUID_TYPE
#define HCI_EIR_MORE_32BITS_UUID_TYPE        BT_EIR_MORE_32BITS_UUID_TYPE
#define HCI_EIR_COMPLETE_32BITS_UUID_TYPE    BT_EIR_COMPLETE_32BITS_UUID_TYPE
#define HCI_EIR_MORE_128BITS_UUID_TYPE       BT_EIR_MORE_128BITS_UUID_TYPE
#define HCI_EIR_COMPLETE_128BITS_UUID_TYPE   BT_EIR_COMPLETE_128BITS_UUID_TYPE
#define HCI_EIR_SHORTENED_LOCAL_NAME_TYPE    BT_EIR_SHORTENED_LOCAL_NAME_TYPE
#define HCI_EIR_COMPLETE_LOCAL_NAME_TYPE     BT_EIR_COMPLETE_LOCAL_NAME_TYPE
#define HCI_EIR_TX_POWER_LEVEL_TYPE          BT_EIR_TX_POWER_LEVEL_TYPE
#define HCI_EIR_MANUFACTURER_SPECIFIC_TYPE   BT_EIR_MANUFACTURER_SPECIFIC_TYPE
#define HCI_EIR_OOB_BD_ADDR_TYPE             BT_EIR_OOB_BD_ADDR_TYPE
#define HCI_EIR_OOB_COD_TYPE                 BT_EIR_OOB_COD_TYPE
#define HCI_EIR_OOB_SSP_HASH_C_TYPE          BT_EIR_OOB_SSP_HASH_C_TYPE
#define HCI_EIR_OOB_SSP_RAND_R_TYPE          BT_EIR_OOB_SSP_RAND_R_TYPE

/* Definitions for Write Simple Pairing Mode */
#define HCI_SP_MODE_UNDEFINED           0x00
#define HCI_SP_MODE_ENABLED             0x01

/* Definitions for Write Simple Pairing Debug Mode */
#define HCI_SPD_MODE_DISABLED           0x00
#define HCI_SPD_MODE_ENABLED            0x01

/* Definitions for IO Capability Response/Command */
#define HCI_IO_CAP_DISPLAY_ONLY         0x00
#define HCI_IO_CAP_DISPLAY_YESNO        0x01
#define HCI_IO_CAP_KEYBOARD_ONLY        0x02
#define HCI_IO_CAP_NO_IO                0x03

#define HCI_OOB_AUTH_DATA_NOT_PRESENT   0x00
#define HCI_OOB_REM_AUTH_DATA_PRESENT   0x01

#define HCI_MITM_PROTECT_NOT_REQUIRED  0x00
#define HCI_MITM_PROTECT_REQUIRED      0x01


/* Policy settings status */
#define HCI_DISABLE_ALL_LM_MODES        0x0000
#define HCI_ENABLE_MASTER_SLAVE_SWITCH  0x0001
#define HCI_ENABLE_HOLD_MODE            0x0002
#define HCI_ENABLE_SNIFF_MODE           0x0004
#define HCI_ENABLE_PARK_MODE            0x0008

/* By default allow switch, because host can not allow that */
/* that until he created the connection */
#define HCI_DEFAULT_POLICY_SETTINGS     HCI_DISABLE_ALL_LM_MODES

/* Filters that are sent in set filter command */
#define HCI_FILTER_TYPE_CLEAR_ALL       0x00
#define HCI_FILTER_INQUIRY_RESULT       0x01
#define HCI_FILTER_CONNECTION_SETUP     0x02

#define HCI_FILTER_COND_NEW_DEVICE      0x00
#define HCI_FILTER_COND_DEVICE_CLASS    0x01
#define HCI_FILTER_COND_BD_ADDR         0x02

#define HCI_DO_NOT_AUTO_ACCEPT_CONNECT  1
#define HCI_DO_AUTO_ACCEPT_CONNECT      2   /* role switch disabled */
#define HCI_DO_AUTO_ACCEPT_CONNECT_RS   3   /* role switch enabled (1.1 errata 1115) */

/* Auto accept flags */
#define HCI_AUTO_ACCEPT_OFF             0x00
#define HCI_AUTO_ACCEPT_ACL_CONNECTIONS 0x01
#define HCI_AUTO_ACCEPT_SCO_CONNECTIONS 0x02

/* PIN type */
#define HCI_PIN_TYPE_VARIABLE           0
#define HCI_PIN_TYPE_FIXED              1

/* Loopback Modes */
#define HCI_LOOPBACK_MODE_DISABLED      0
#define HCI_LOOPBACK_MODE_LOCAL         1
#define HCI_LOOPBACK_MODE_REMOTE        2

#define SLOTS_PER_10MS                  16      /* 0.625 ms slots in a 10 ms tick */

/* Maximum connection accept timeout in 0.625msec */
#define HCI_MAX_CONN_ACCEPT_TOUT        0xB540  /* 29 sec */
#define HCI_DEF_CONN_ACCEPT_TOUT        0x1F40  /* 5 sec */

/* Page timeout is used in LC only and LC is counting down slots not using OS */
#define HCI_DEFAULT_PAGE_TOUT           0x2000  /* 5.12 sec (in slots) */

/* Scan enable flags */
#define HCI_NO_SCAN_ENABLED             0x00
#define HCI_INQUIRY_SCAN_ENABLED        0x01
#define HCI_PAGE_SCAN_ENABLED           0x02

/* Pagescan timer definitions in 0.625 ms */
#define HCI_MIN_PAGESCAN_INTERVAL       0x12    /* 11.25 ms */
#define HCI_MAX_PAGESCAN_INTERVAL       0x1000  /* 2.56 sec */
#define HCI_DEF_PAGESCAN_INTERVAL       0x0800  /* 1.28 sec */

/* Parameter for pagescan window is passed to LC and is kept in slots */
#define HCI_MIN_PAGESCAN_WINDOW         0x11    /* 10.625 ms */
#define HCI_MAX_PAGESCAN_WINDOW         0x1000  /* 2.56  sec */
#define HCI_DEF_PAGESCAN_WINDOW         0x12    /* 11.25 ms  */

/* Inquiryscan timer definitions in 0.625 ms */
#define HCI_MIN_INQUIRYSCAN_INTERVAL    0x12    /* 11.25 ms */
#define HCI_MAX_INQUIRYSCAN_INTERVAL    0x1000  /* 2.56 sec */
#define HCI_DEF_INQUIRYSCAN_INTERVAL    0x1000  /* 2.56 sec */

/* Parameter for inquiryscan window is passed to LC and is kept in slots */
#define HCI_MIN_INQUIRYSCAN_WINDOW      0x11    /* 10.625 ms */
#define HCI_MAX_INQUIRYSCAN_WINDOW      0x1000  /* 2.56 sec */
#define HCI_DEF_INQUIRYSCAN_WINDOW      0x12    /* 11.25 ms */

/* Encryption modes */
#define HCI_ENCRYPT_MODE_DISABLED       0x00
#define HCI_ENCRYPT_MODE_POINT_TO_POINT 0x01
#define HCI_ENCRYPT_MODE_ALL            0x02

/* Voice settings */
#define HCI_INP_CODING_LINEAR           0x0000 /* 0000000000 */
#define HCI_INP_CODING_U_LAW            0x0100 /* 0100000000 */
#define HCI_INP_CODING_A_LAW            0x0200 /* 1000000000 */
#define HCI_INP_CODING_MASK             0x0300 /* 1100000000 */

#define HCI_INP_DATA_FMT_1S_COMPLEMENT  0x0000 /* 0000000000 */
#define HCI_INP_DATA_FMT_2S_COMPLEMENT  0x0040 /* 0001000000 */
#define HCI_INP_DATA_FMT_SIGN_MAGNITUDE 0x0080 /* 0010000000 */
#define HCI_INP_DATA_FMT_UNSIGNED       0x00c0 /* 0011000000 */
#define HCI_INP_DATA_FMT_MASK           0x00c0 /* 0011000000 */

#define HCI_INP_SAMPLE_SIZE_8BIT        0x0000 /* 0000000000 */
#define HCI_INP_SAMPLE_SIZE_16BIT       0x0020 /* 0000100000 */
#define HCI_INP_SAMPLE_SIZE_MASK        0x0020 /* 0000100000 */

#define HCI_INP_LINEAR_PCM_BIT_POS_MASK 0x001c /* 0000011100 */
#define HCI_INP_LINEAR_PCM_BIT_POS_OFFS 2

#define HCI_AIR_CODING_FORMAT_CVSD      0x0000 /* 0000000000 */
#define HCI_AIR_CODING_FORMAT_U_LAW     0x0001 /* 0000000001 */
#define HCI_AIR_CODING_FORMAT_A_LAW     0x0002 /* 0000000010 */
#define HCI_AIR_CODING_FORMAT_TRANSPNT  0x0003 /* 0000000011 */
#define HCI_AIR_CODING_FORMAT_MASK      0x0003 /* 0000000011 */

/* default                                        0001100000 */
#define HCI_DEFAULT_VOICE_SETTINGS    (HCI_INP_CODING_LINEAR \
                                     | HCI_INP_DATA_FMT_2S_COMPLEMENT \
                                     | HCI_INP_SAMPLE_SIZE_16BIT \
                                     | HCI_AIR_CODING_FORMAT_CVSD)

#define HCI_CVSD_SUPPORTED(x)       (((x) & HCI_AIR_CODING_FORMAT_MASK) == HCI_AIR_CODING_FORMAT_CVSD)
#define HCI_U_LAW_SUPPORTED(x)      (((x) & HCI_AIR_CODING_FORMAT_MASK) == HCI_AIR_CODING_FORMAT_U_LAW)
#define HCI_A_LAW_SUPPORTED(x)      (((x) & HCI_AIR_CODING_FORMAT_MASK) == HCI_AIR_CODING_FORMAT_A_LAW)
#define HCI_TRANSPNT_SUPPORTED(x)   (((x) & HCI_AIR_CODING_FORMAT_MASK) == HCI_AIR_CODING_FORMAT_TRANSPNT)

/* Retransmit timer definitions in 0.625 */
#define HCI_MAX_AUTO_FLUSH_TOUT         0x07FF
#define HCI_DEFAULT_AUTO_FLUSH_TOUT     0       /* No auto flush */

/* Broadcast retransmitions */
#define HCI_DEFAULT_NUM_BCAST_RETRAN    1

/* Define broadcast data types as passed in the hci data packet */
#define HCI_DATA_POINT_TO_POINT         0x00
#define HCI_DATA_ACTIVE_BCAST           0x01
#define HCI_DATA_PICONET_BCAST          0x02

/* Hold mode activity */
#define HCI_MAINTAIN_CUR_POWER_STATE    0x00
#define HCI_SUSPEND_PAGE_SCAN           0x01
#define HCI_SUSPEND_INQUIRY_SCAN        0x02
#define HCI_SUSPEND_PERIODIC_INQUIRIES  0x04

/* Default Link Supervision timeoout */
#define HCI_DEFAULT_INACT_TOUT          0x7D00  /* BR/EDR (20 seconds) */
#define HCI_DEFAULT_AMP_INACT_TOUT      0x3E80  /* AMP    (10 seconds) */

/* Read transmit power level parameter */
#define HCI_READ_CURRENT                0x00
#define HCI_READ_MAXIMUM                0x01

/* Link types for connection complete event */
#define HCI_LINK_TYPE_SCO               0x00
#define HCI_LINK_TYPE_ACL               0x01
#define HCI_LINK_TYPE_ESCO              0x02

/* Link Key Notification Event (Key Type) definitions */
#define HCI_LKEY_TYPE_COMBINATION       0x00
#define HCI_LKEY_TYPE_LOCAL_UNIT        0x01
#define HCI_LKEY_TYPE_REMOTE_UNIT       0x02
#define HCI_LKEY_TYPE_DEBUG_COMB        0x03
#define HCI_LKEY_TYPE_UNAUTH_COMB       0x04
#define HCI_LKEY_TYPE_AUTH_COMB         0x05
#define HCI_LKEY_TYPE_CHANGED_COMB      0x06

/* Internal definitions - not used over HCI */
#define HCI_LKEY_TYPE_AMP_WIFI          0x80
#define HCI_LKEY_TYPE_AMP_UWB           0x81
#define HCI_LKEY_TYPE_UNKNOWN           0xff

/* Read Local Version HCI Version return values (Command Complete Event) */
#define HCI_VERSION_1_0B                0x00
#define HCI_VERSION_1_1                 0x01

/* Define an invalid value for a handle */
#define HCI_INVALID_HANDLE              0xFFFF

/* Define max ammount of data in the HCI command */
#define HCI_COMMAND_SIZE        255

/* Define the preamble length for all HCI Commands.
** This is 2-bytes for opcode and 1 byte for length
*/
#define HCIC_PREAMBLE_SIZE      3

/* Define the preamble length for all HCI Events
** This is 1-byte for opcode and 1 byte for length
*/
#define HCIE_PREAMBLE_SIZE      2
#define HCI_SCO_PREAMBLE_SIZE   3
#define HCI_DATA_PREAMBLE_SIZE  4

/* local Bluetooth controller id for AMP HCI */
#define LOCAL_BR_EDR_CONTROLLER_ID      0

/* controller id types for AMP HCI */
#define HCI_CONTROLLER_TYPE_BR_EDR      0
#define HCI_CONTROLLER_TYPE_802_11      1
#define HCI_CONTROLLER_TYPE_ECMA        2
#define HCI_MAX_CONTROLLER_TYPES        3




/* AMP Controller Status codes
*/
#define HCI_AMP_CTRLR_PHYSICALLY_DOWN   0
#define HCI_AMP_CTRLR_USABLE_BY_BT      1
#define HCI_AMP_CTRLR_UNUSABLE_FOR_BT   2
#define HCI_AMP_CTRLR_LOW_CAP_FOR_BT    3
#define HCI_AMP_CTRLR_MED_CAP_FOR_BT    4
#define HCI_AMP_CTRLR_HIGH_CAP_FOR_BT   5
#define HCI_AMP_CTRLR_FULL_CAP_FOR_BT   6

#define HCI_MAX_AMP_STATUS_TYPES        7


/* Define the extended flow specification fields used by AMP */
typedef struct
{
    UINT8       id;
    UINT8       stype;
    UINT16      max_sdu_size;
    UINT32      sdu_inter_time;
    UINT32      access_latency;
    UINT32      flush_timeout;
} tHCI_EXT_FLOW_SPEC;


/* HCI message type definitions (for H4 messages) */
#define HCIT_TYPE_COMMAND   1
#define HCIT_TYPE_ACL_DATA  2
#define HCIT_TYPE_SCO_DATA  3
#define HCIT_TYPE_EVENT     4
#define HCIT_TYPE_LM_DIAG   7
#define HCIT_TYPE_NFC       16

#define HCIT_LM_DIAG_LENGTH 63

/* Parameter information for HCI_BRCM_SET_ACL_PRIORITY */
#define HCI_BRCM_ACL_PRIORITY_PARAM_SIZE    3
#define HCI_BRCM_ACL_PRIORITY_LOW           0x00
#define HCI_BRCM_ACL_PRIORITY_HIGH          0xFF
#define HCI_BRCM_SET_ACL_PRIORITY           (0x0057 | HCI_GRP_VENDOR_SPECIFIC)

/* Define values for LMP Test Control parameters
** Test Scenario, Hopping Mode, Power Control Mode
*/
#define LMP_TESTCTL_TESTSC_PAUSE        0
#define LMP_TESTCTL_TESTSC_TXTEST_0     1
#define LMP_TESTCTL_TESTSC_TXTEST_1     2
#define LMP_TESTCTL_TESTSC_TXTEST_1010  3
#define LMP_TESTCTL_TESTSC_PSRND_BITSEQ 4
#define LMP_TESTCTL_TESTSC_CLOSEDLB_ACL 5
#define LMP_TESTCTL_TESTSC_CLOSEDLB_SCO 6
#define LMP_TESTCTL_TESTSC_ACL_NOWHIT   7
#define LMP_TESTCTL_TESTSC_SCO_NOWHIT   8
#define LMP_TESTCTL_TESTSC_TXTEST_11110000  9
#define LMP_TESTCTL_TESTSC_EXITTESTMODE 255

#define LMP_TESTCTL_HOPMOD_RXTX1FREQ    0
#define LMP_TESTCTL_HOPMOD_HOP_EURUSA   1
#define LMP_TESTCTL_HOPMOD_HOP_JAPAN    2
#define LMP_TESTCTL_HOPMOD_HOP_FRANCE   3
#define LMP_TESTCTL_HOPMOD_HOP_SPAIN    4
#define LMP_TESTCTL_HOPMOD_REDUCED_HOP  5

#define LMP_TESTCTL_POWCTL_FIXEDTX_OP   0
#define LMP_TESTCTL_POWCTL_ADAPTIVE     1


/*
** Define company IDs (from Bluetooth Assigned Numbers v1.1, section 2.2)
*/
#define LMP_COMPID_ERICSSON             0
#define LMP_COMPID_NOKIA                1
#define LMP_COMPID_INTEL                2
#define LMP_COMPID_IBM                  3
#define LMP_COMPID_TOSHIBA              4
#define LMP_COMPID_3COM                 5
#define LMP_COMPID_MICROSOFT            6
#define LMP_COMPID_LUCENT               7
#define LMP_COMPID_MOTOROLA             8
#define LMP_COMPID_INFINEON             9
#define LMP_COMPID_CSR                  10
#define LMP_COMPID_SILICON_WAVE         11
#define LMP_COMPID_DIGIANSWER           12
#define LMP_COMPID_TEXAS_INSTRUMENTS    13
#define LMP_COMPID_PARTHUS              14
#define LMP_COMPID_BROADCOM             15
#define LMP_COMPID_MITEL_SEMI           16
#define LMP_COMPID_WIDCOMM              17
#define LMP_COMPID_ZEEVO                18
#define LMP_COMPID_ATMEL                19
#define LMP_COMPID_MITSUBISHI           20
#define LMP_COMPID_RTX_TELECOM          21
#define LMP_COMPID_KC_TECH              22
#define LMP_COMPID_NEWLOGIC             23
#define LMP_COMPID_TRANSILICA           24
#define LMP_COMPID_ROHDE_SCHWARZ        25
#define LMP_COMPID_TTPCOM               26
#define LMP_COMPID_SIGNIA               27
#define LMP_COMPID_CONEXANT             28
#define LMP_COMPID_QUALCOMM             29
#define LMP_COMPID_INVENTEL             30
#define LMP_COMPID_AVM                  31
#define LMP_COMPID_BANDSPEED            32
#define LMP_COMPID_MANSELLA             33
#define LMP_COMPID_NEC_CORP             34
#define LMP_COMPID_WAVEPLUS             35
#define LMP_COMPID_ALCATEL              36
#define LMP_COMPID_PHILIPS              37
#define LMP_COMPID_C_TECHNOLOGIES       38
#define LMP_COMPID_OPEN_INTERFACE       39
#define LMP_COMPID_RF_MICRO             40
#define LMP_COMPID_HITACHI              41
#define LMP_COMPID_SYMBOL_TECH          42
#define LMP_COMPID_TENOVIS              43
#define LMP_COMPID_MACRONIX             44
#define LMP_COMPID_GCT_SEMI             45
#define LMP_COMPID_NORWOOD_SYSTEMS      46
#define LMP_COMPID_MEWTEL_TECH          47
#define LMP_COMPID_STM                  48
#define LMP_COMPID_SYNOPSYS             49
#define LMP_COMPID_RED_M_LTD            50
#define LMP_COMPID_COMMIL_LTD           51
#define LMP_COMPID_CATC                 52
#define LMP_COMPID_ECLIPSE              53
#define LMP_COMPID_RENESAS_TECH         54
#define LMP_COMPID_MOBILIAN_CORP        55
#define LMP_COMPID_TERAX                56
#define LMP_COMPID_ISSC                 57
#define LMP_COMPID_MATSUSHITA           58
#define LMP_COMPID_GENNUM_CORP          59
#define LMP_COMPID_RESEARCH_IN_MOTION   60
#define LMP_COMPID_IPEXTREME            61
#define LMP_COMPID_SYSTEMS_AND_CHIPS    62
#define LMP_COMPID_BLUETOOTH_SIG        63
#define LMP_COMPID_SEIKO_EPSON_CORP     64
#define LMP_COMPID_ISS_TAIWAN           65
#define LMP_COMPID_CONWISE_TECHNOLOGIES 66
#define LMP_COMPID_PARROT_SA            67
#define LMP_COMPID_SOCKET_COMM          68
#define LMP_COMPID_ALTHEROS             69
#define LMP_COMPID_MEDIATEK             70
#define LMP_COMPID_BLUEGIGA             71
#define LMP_COMPID_MARVELL              72
#define LMP_COMPID_3DSP_CORP            73
#define LMP_COMPID_ACCEL_SEMICONDUCTOR  74
#define LMP_COMPID_CONTINENTAL_AUTO     75
#define LMP_COMPID_APPLE                76
#define LMP_COMPID_STACCATO             77
#define LMP_COMPID_AVAGO_TECHNOLOGIES   78
#define LMP_COMPID_APT_LTD              79
#define LMP_COMPID_SIRF_TECHNOLOGY      80
#define LMP_COMPID_TZERO_TECHNOLOGY     81
#define LMP_COMPID_J_AND_M_CORP         82
#define LMP_COMPID_FREE_2_MOVE          83
#define LMP_COMPID_3DIJOY_CORP          84
#define LMP_COMPID_PLANTRONICS          85
#define LMP_COMPID_SONY_ERICSSON_MOBILE 86
#define LMP_COMPID_HARMON_INTL_IND      87
#define LMP_COMPID_VIZIO                88
#define LMP_COMPID_NORDIC SEMI          89
#define LMP_COMPID_EM MICRO             90
#define LMP_COMPID_RALINK TECH          91
#define LMP_COMPID_BELKIN INC           92
#define LMP_COMPID_REALTEK SEMI         93
#define LMP_COMPID_STONESTREET ONE      94
#define LMP_COMPID_WICENTRIC            95
#define LMP_COMPID_RIVIERAWAVES         96
#define LMP_COMPID_RDA MICRO            97
#define LMP_COMPID_GIBSON GUITARS       98
#define LMP_COMPID_MICOMMAND INC        99
#define LMP_COMPID_BAND XI              100
#define LMP_COMPID_HP COMPANY           101
#define LMP_COMPID_9SOLUTIONS OY        102
#define LMP_COMPID_GN NETCOM            103
#define LMP_COMPID_GENERAL MOTORS       104
#define LMP_COMPID_AD ENGINEERING       105
#define LMP_COMPID_MINDTREE LTD         106
#define LMP_COMPID_POLAR ELECTRO        107
#define LMP_COMPID_BEAUTIFUL ENTERPRISE 108
#define LMP_COMPID_BRIARTEK             109
#define LMP_COMPID_SUMMIT DATA COMM     110
#define LMP_COMPID_SOUND ID             111
#define LMP_COMPID_MONSTER LLC          112
#define LMP_COMPID_CONNECTBLU           113
#define LMP_COMPID_MAX_ID               114 /* this is a place holder */
#define LMP_COMPID_INTERNAL             65535

#define MAX_LMP_COMPID                  (LMP_COMPID_MAX_ID)
/*
** Define the packet types in the packet header, and a couple extra
*/
#define PKT_TYPE_NULL   0x00
#define PKT_TYPE_POLL   0x01
#define PKT_TYPE_FHS    0x02
#define PKT_TYPE_DM1    0x03

#define PKT_TYPE_DH1    0x04
#define PKT_TYPE_HV1    0x05
#define PKT_TYPE_HV2    0x06
#define PKT_TYPE_HV3    0x07
#define PKT_TYPE_DV     0x08
#define PKT_TYPE_AUX1   0x09

#define PKT_TYPE_DM3    0x0a
#define PKT_TYPE_DH3    0x0b

#define PKT_TYPE_DM5    0x0e
#define PKT_TYPE_DH5    0x0f


#define PKT_TYPE_ID     0x10        /* Internally used packet types */
#define PKT_TYPE_BAD    0x11
#define PKT_TYPE_NONE   0x12

/*
** Define packet size
*/
#define HCI_DM1_PACKET_SIZE         17
#define HCI_DH1_PACKET_SIZE         27
#define HCI_DM3_PACKET_SIZE         121
#define HCI_DH3_PACKET_SIZE         183
#define HCI_DM5_PACKET_SIZE         224
#define HCI_DH5_PACKET_SIZE         339
#define HCI_AUX1_PACKET_SIZE        29
#define HCI_HV1_PACKET_SIZE         10
#define HCI_HV2_PACKET_SIZE         20
#define HCI_HV3_PACKET_SIZE         30
#define HCI_DV_PACKET_SIZE          9
#define HCI_EDR2_DH1_PACKET_SIZE    54
#define HCI_EDR2_DH3_PACKET_SIZE    367
#define HCI_EDR2_DH5_PACKET_SIZE    679
#define HCI_EDR3_DH1_PACKET_SIZE    83
#define HCI_EDR3_DH3_PACKET_SIZE    552
#define HCI_EDR3_DH5_PACKET_SIZE    1021

/*
**   Features encoding - page 0
*/
#define HCI_NUM_FEATURE_BYTES           8
#define HCI_FEATURES_KNOWN(x) ((x[0] | x[1] | x[2] | x[3] | x[4] | x[5] | x[6] | x[7]) != 0)

#define HCI_FEATURE_3_SLOT_PACKETS_MASK 0x01
#define HCI_FEATURE_3_SLOT_PACKETS_OFF  0
#define HCI_3_SLOT_PACKETS_SUPPORTED(x) ((x)[HCI_FEATURE_3_SLOT_PACKETS_OFF] & HCI_FEATURE_3_SLOT_PACKETS_MASK)

#define HCI_FEATURE_5_SLOT_PACKETS_MASK 0x02
#define HCI_FEATURE_5_SLOT_PACKETS_OFF  0
#define HCI_5_SLOT_PACKETS_SUPPORTED(x) ((x)[HCI_FEATURE_5_SLOT_PACKETS_OFF] & HCI_FEATURE_5_SLOT_PACKETS_MASK)

#define HCI_FEATURE_ENCRYPTION_MASK     0x04
#define HCI_FEATURE_ENCRYPTION_OFF      0
#define HCI_ENCRYPTION_SUPPORTED(x)     ((x)[HCI_FEATURE_ENCRYPTION_OFF] & HCI_FEATURE_ENCRYPTION_MASK)

#define HCI_FEATURE_SLOT_OFFSET_MASK    0x08
#define HCI_FEATURE_SLOT_OFFSET_OFF     0
#define HCI_SLOT_OFFSET_SUPPORTED(x)    ((x)[HCI_FEATURE_SLOT_OFFSET_OFF] & HCI_FEATURE_SLOT_OFFSET_MASK)

#define HCI_FEATURE_TIMING_ACC_MASK     0x10
#define HCI_FEATURE_TIMING_ACC_OFF      0
#define HCI_TIMING_ACC_SUPPORTED(x)     ((x)[HCI_FEATURE_TIMING_ACC_OFF] & HCI_FEATURE_TIMING_ACC_MASK)

#define HCI_FEATURE_SWITCH_MASK         0x20
#define HCI_FEATURE_SWITCH_OFF          0
#define HCI_SWITCH_SUPPORTED(x)         ((x)[HCI_FEATURE_SWITCH_OFF] & HCI_FEATURE_SWITCH_MASK)

#define HCI_FEATURE_HOLD_MODE_MASK      0x40
#define HCI_FEATURE_HOLD_MODE_OFF       0
#define HCI_HOLD_MODE_SUPPORTED(x)      ((x)[HCI_FEATURE_HOLD_MODE_OFF] & HCI_FEATURE_HOLD_MODE_MASK)

#define HCI_FEATURE_SNIFF_MODE_MASK     0x80
#define HCI_FEATURE_SNIFF_MODE_OFF      0
#define HCI_SNIFF_MODE_SUPPORTED(x)      ((x)[HCI_FEATURE_SNIFF_MODE_OFF] & HCI_FEATURE_SNIFF_MODE_MASK)

#define HCI_FEATURE_PARK_MODE_MASK      0x01
#define HCI_FEATURE_PARK_MODE_OFF       1
#define HCI_PARK_MODE_SUPPORTED(x)      ((x)[HCI_FEATURE_PARK_MODE_OFF] & HCI_FEATURE_PARK_MODE_MASK)

#define HCI_FEATURE_RSSI_MASK           0x02
#define HCI_FEATURE_RSSI_OFF            1
#define HCI_RSSI_SUPPORTED(x)           ((x)[HCI_FEATURE_RSSI_OFF] & HCI_FEATURE_RSSI_MASK)

#define HCI_FEATURE_CQM_DATA_RATE_MASK  0x04
#define HCI_FEATURE_CQM_DATA_RATE_OFF   1
#define HCI_CQM_DATA_RATE_SUPPORTED(x)  ((x)[HCI_FEATURE_CQM_DATA_RATE_OFF] & HCI_FEATURE_CQM_DATA_RATE_MASK)

#define HCI_FEATURE_SCO_LINK_MASK       0x08
#define HCI_FEATURE_SCO_LINK_OFF        1
#define HCI_SCO_LINK_SUPPORTED(x)       ((x)[HCI_FEATURE_SCO_LINK_OFF] & HCI_FEATURE_SCO_LINK_MASK)

#define HCI_FEATURE_HV2_PACKETS_MASK    0x10
#define HCI_FEATURE_HV2_PACKETS_OFF     1
#define HCI_HV2_PACKETS_SUPPORTED(x)    ((x)[HCI_FEATURE_HV2_PACKETS_OFF] & HCI_FEATURE_HV2_PACKETS_MASK)

#define HCI_FEATURE_HV3_PACKETS_MASK    0x20
#define HCI_FEATURE_HV3_PACKETS_OFF     1
#define HCI_HV3_PACKETS_SUPPORTED(x)    ((x)[HCI_FEATURE_HV3_PACKETS_OFF] & HCI_FEATURE_HV3_PACKETS_MASK)

#define HCI_FEATURE_U_LAW_MASK          0x40
#define HCI_FEATURE_U_LAW_OFF           1
#define HCI_LMP_U_LAW_SUPPORTED(x)      ((x)[HCI_FEATURE_U_LAW_OFF] & HCI_FEATURE_U_LAW_MASK)

#define HCI_FEATURE_A_LAW_MASK          0x80
#define HCI_FEATURE_A_LAW_OFF           1
#define HCI_LMP_A_LAW_SUPPORTED(x)      ((x)[HCI_FEATURE_A_LAW_OFF] & HCI_FEATURE_A_LAW_MASK)

#define HCI_FEATURE_CVSD_MASK           0x01
#define HCI_FEATURE_CVSD_OFF            2
#define HCI_LMP_CVSD_SUPPORTED(x)       ((x)[HCI_FEATURE_CVSD_OFF] & HCI_FEATURE_CVSD_MASK)

#define HCI_FEATURE_PAGING_SCHEME_MASK  0x02
#define HCI_FEATURE_PAGING_SCHEME_OFF   2
#define HCI_PAGING_SCHEME_SUPPORTED(x) ((x)[HCI_FEATURE_PAGING_SCHEME_OFF] & HCI_FEATURE_PAGING_SCHEME_MASK)

#define HCI_FEATURE_POWER_CTRL_MASK     0x04
#define HCI_FEATURE_POWER_CTRL_OFF      2
#define HCI_POWER_CTRL_SUPPORTED(x)     ((x)[HCI_FEATURE_POWER_CTRL_OFF] & HCI_FEATURE_POWER_CTRL_MASK)

#define HCI_FEATURE_TRANSPNT_MASK       0x08
#define HCI_FEATURE_TRANSPNT_OFF        2
#define HCI_LMP_TRANSPNT_SUPPORTED(x)   ((x)[HCI_FEATURE_TRANSPNT_OFF] & HCI_FEATURE_TRANSPNT_MASK)

#define HCI_FEATURE_FLOW_CTRL_LAG_MASK  0x70
#define HCI_FEATURE_FLOW_CTRL_LAG_OFF   2
#define HCI_FLOW_CTRL_LAG_VALUE(x)      (((x)[HCI_FEATURE_FLOW_CTRL_LAG_OFF] & HCI_FEATURE_FLOW_CTRL_LAG_MASK) >> 4)

#define HCI_FEATURE_BROADCAST_ENC_MASK  0x80
#define HCI_FEATURE_BROADCAST_ENC_OFF   2
#define HCI_LMP_BCAST_ENC_SUPPORTED(x)  ((x)[HCI_FEATURE_BROADCAST_ENC_OFF] & HCI_FEATURE_BROADCAST_ENC_MASK)

#define HCI_FEATURE_SCATTER_MODE_MASK   0x01
#define HCI_FEATURE_SCATTER_MODE_OFF    3
#define HCI_LMP_SCATTER_MODE_SUPPORTED(x) ((x)[HCI_FEATURE_SCATTER_MODE_OFF] & HCI_FEATURE_SCATTER_MODE_MASK)

#define HCI_FEATURE_EDR_ACL_2MPS_MASK   0x02
#define HCI_FEATURE_EDR_ACL_2MPS_OFF    3
#define HCI_EDR_ACL_2MPS_SUPPORTED(x)   ((x)[HCI_FEATURE_EDR_ACL_2MPS_OFF] & HCI_FEATURE_EDR_ACL_2MPS_MASK)

#define HCI_FEATURE_EDR_ACL_3MPS_MASK   0x04
#define HCI_FEATURE_EDR_ACL_3MPS_OFF    3
#define HCI_EDR_ACL_3MPS_SUPPORTED(x)   ((x)[HCI_FEATURE_EDR_ACL_3MPS_OFF] & HCI_FEATURE_EDR_ACL_3MPS_MASK)

#define HCI_FEATURE_ENHANCED_INQ_MASK   0x08
#define HCI_FEATURE_ENHANCED_INQ_OFF    3
#define HCI_ENHANCED_INQ_SUPPORTED(x)   ((x)[HCI_FEATURE_ENHANCED_INQ_OFF] & HCI_FEATURE_ENHANCED_INQ_MASK)

#define HCI_FEATURE_INTERLACED_INQ_SCAN_MASK   0x10
#define HCI_FEATURE_INTERLACED_INQ_SCAN_OFF    3
#define HCI_LMP_INTERLACED_INQ_SCAN_SUPPORTED(x) ((x)[HCI_FEATURE_INTERLACED_INQ_SCAN_OFF] & HCI_FEATURE_INTERLACED_INQ_SCAN_MASK)

#define HCI_FEATURE_INTERLACED_PAGE_SCAN_MASK  0x20
#define HCI_FEATURE_INTERLACED_PAGE_SCAN_OFF   3
#define HCI_LMP_INTERLACED_PAGE_SCAN_SUPPORTED(x) ((x)[HCI_FEATURE_INTERLACED_PAGE_SCAN_OFF] & HCI_FEATURE_INTERLACED_PAGE_SCAN_MASK)

#define HCI_FEATURE_INQ_RSSI_MASK       0x40
#define HCI_FEATURE_INQ_RSSI_OFF        3
#define HCI_LMP_INQ_RSSI_SUPPORTED(x)   ((x)[HCI_FEATURE_INQ_RSSI_OFF] & HCI_FEATURE_INQ_RSSI_MASK)

#define HCI_FEATURE_ESCO_EV3_MASK       0x80
#define HCI_FEATURE_ESCO_EV3_OFF        3
#define HCI_ESCO_EV3_SUPPORTED(x)       ((x)[HCI_FEATURE_ESCO_EV3_OFF] & HCI_FEATURE_ESCO_EV3_MASK)

#define HCI_FEATURE_ESCO_EV4_MASK       0x01
#define HCI_FEATURE_ESCO_EV4_OFF        4
#define HCI_ESCO_EV4_SUPPORTED(x)       ((x)[HCI_FEATURE_ESCO_EV4_OFF] & HCI_FEATURE_ESCO_EV4_MASK)

#define HCI_FEATURE_ESCO_EV5_MASK       0x02
#define HCI_FEATURE_ESCO_EV5_OFF        4
#define HCI_ESCO_EV5_SUPPORTED(x)       ((x)[HCI_FEATURE_ESCO_EV5_OFF] & HCI_FEATURE_ESCO_EV5_MASK)

#define HCI_FEATURE_ABSENCE_MASKS_MASK  0x04
#define HCI_FEATURE_ABSENCE_MASKS_OFF   4
#define HCI_LMP_ABSENCE_MASKS_SUPPORTED(x) ((x)[HCI_FEATURE_ABSENCE_MASKS_OFF] & HCI_FEATURE_ABSENCE_MASKS_MASK)

#define HCI_FEATURE_AFH_CAP_SLAVE_MASK  0x08
#define HCI_FEATURE_AFH_CAP_SLAVE_OFF   4
#define HCI_LMP_AFH_CAP_SLAVE_SUPPORTED(x) ((x)[HCI_FEATURE_AFH_CAP_SLAVE_OFF] & HCI_FEATURE_AFH_CAP_SLAVE_MASK)

#define HCI_FEATURE_AFH_CLASS_SLAVE_MASK 0x10
#define HCI_FEATURE_AFH_CLASS_SLAVE_OFF  4
#define HCI_LMP_AFH_CLASS_SLAVE_SUPPORTED(x) ((x)[HCI_FEATURE_AFH_CLASS_SLAVE_OFF] & HCI_FEATURE_AFH_CLASS_SLAVE_MASK)

#define HCI_FEATURE_ALIAS_AUTH_MASK     0x20
#define HCI_FEATURE_ALIAS_AUTH_OFF      4
#define HCI_LMP_ALIAS_AUTH_SUPPORTED(x) ((x)[HCI_FEATURE_ALIAS_AUTH_OFF] & HCI_FEATURE_ALIAS_AUTH_MASK)

#define HCI_FEATURE_ANON_MODE_MASK      0x40
#define HCI_FEATURE_ANON_MODE_OFF       4
#define HCI_LMP_ANON_MODE_SUPPORTED(x)  ((x)[HCI_FEATURE_ANON_MODE_OFF] & HCI_FEATURE_ANON_MODE_MASK)

#define HCI_FEATURE_3_SLOT_EDR_ACL_MASK 0x80
#define HCI_FEATURE_3_SLOT_EDR_ACL_OFF  4
#define HCI_3_SLOT_EDR_ACL_SUPPORTED(x) ((x)[HCI_FEATURE_3_SLOT_EDR_ACL_OFF] & HCI_FEATURE_3_SLOT_EDR_ACL_MASK)

#define HCI_FEATURE_5_SLOT_EDR_ACL_MASK 0x01
#define HCI_FEATURE_5_SLOT_EDR_ACL_OFF  5
#define HCI_5_SLOT_EDR_ACL_SUPPORTED(x) ((x)[HCI_FEATURE_5_SLOT_EDR_ACL_OFF] & HCI_FEATURE_5_SLOT_EDR_ACL_MASK)

#define HCI_FEATURE_SNIFF_SUB_RATE_MASK 0x02
#define HCI_FEATURE_SNIFF_SUB_RATE_OFF  5
#define HCI_SNIFF_SUB_RATE_SUPPORTED(x) ((x)[HCI_FEATURE_SNIFF_SUB_RATE_OFF] & HCI_FEATURE_SNIFF_SUB_RATE_MASK)

#define HCI_FEATURE_ATOMIC_ENCRYPT_MASK 0x04
#define HCI_FEATURE_ATOMIC_ENCRYPT_OFF  5
#define HCI_ATOMIC_ENCRYPT_SUPPORTED(x) ((x)[HCI_FEATURE_ATOMIC_ENCRYPT_OFF] & HCI_FEATURE_ATOMIC_ENCRYPT_MASK)

#define HCI_FEATURE_AFH_CAP_MASTR_MASK  0x08
#define HCI_FEATURE_AFH_CAP_MASTR_OFF   5
#define HCI_LMP_AFH_CAP_MASTR_SUPPORTED(x) ((x)[HCI_FEATURE_AFH_CAP_MASTR_OFF] & HCI_FEATURE_AFH_CAP_MASTR_MASK)

#define HCI_FEATURE_AFH_CLASS_MASTR_MASK 0x10
#define HCI_FEATURE_AFH_CLASS_MASTR_OFF  5
#define HCI_LMP_AFH_CLASS_MASTR_SUPPORTED(x) ((x)[HCI_FEATURE_AFH_CLASS_MASTR_OFF] & HCI_FEATURE_AFH_CLASS_MASTR_MASK)

#define HCI_FEATURE_EDR_ESCO_2MPS_MASK  0x20
#define HCI_FEATURE_EDR_ESCO_2MPS_OFF   5
#define HCI_EDR_ESCO_2MPS_SUPPORTED(x)  ((x)[HCI_FEATURE_EDR_ESCO_2MPS_OFF] & HCI_FEATURE_EDR_ESCO_2MPS_MASK)

#define HCI_FEATURE_EDR_ESCO_3MPS_MASK  0x40
#define HCI_FEATURE_EDR_ESCO_3MPS_OFF   5
#define HCI_EDR_ESCO_3MPS_SUPPORTED(x)  ((x)[HCI_FEATURE_EDR_ESCO_3MPS_OFF] & HCI_FEATURE_EDR_ESCO_3MPS_MASK)

#define HCI_FEATURE_3_SLOT_EDR_ESCO_MASK 0x80
#define HCI_FEATURE_3_SLOT_EDR_ESCO_OFF  5
#define HCI_3_SLOT_EDR_ESCO_SUPPORTED(x) ((x)[HCI_FEATURE_3_SLOT_EDR_ESCO_OFF] & HCI_FEATURE_3_SLOT_EDR_ESCO_MASK)

#define HCI_FEATURE_EXT_INQ_RSP_MASK    0x01
#define HCI_FEATURE_EXT_INQ_RSP_OFF     6
#define HCI_EXT_INQ_RSP_SUPPORTED(x)    ((x)[HCI_FEATURE_EXT_INQ_RSP_OFF] & HCI_FEATURE_EXT_INQ_RSP_MASK)

#define HCI_FEATURE_ANUM_PIN_AWARE_MASK 0x02
#define HCI_FEATURE_ANUM_PIN_AWARE_OFF  6
#define HCI_ANUM_PIN_AWARE_SUPPORTED(x) ((x)[HCI_FEATURE_ANUM_PIN_AWARE_OFF] & HCI_FEATURE_ANUM_PIN_AWARE_MASK)

#define HCI_FEATURE_ANUM_PIN_CAP_MASK   0x04
#define HCI_FEATURE_ANUM_PIN_CAP_OFF    6
#define HCI_ANUM_PIN_CAP_SUPPORTED(x)   ((x)[HCI_FEATURE_ANUM_PIN_CAP_OFF] & HCI_FEATURE_ANUM_PIN_CAP_MASK)

#define HCI_FEATURE_SIMPLE_PAIRING_MASK 0x08
#define HCI_FEATURE_SIMPLE_PAIRING_OFF  6
#define HCI_SIMPLE_PAIRING_SUPPORTED(x) ((x)[HCI_FEATURE_SIMPLE_PAIRING_OFF] & HCI_FEATURE_SIMPLE_PAIRING_MASK)

#define HCI_FEATURE_ENCAP_PDU_MASK      0x10
#define HCI_FEATURE_ENCAP_PDU_OFF       6
#define HCI_ENCAP_PDU_SUPPORTED(x)      ((x)[HCI_FEATURE_ENCAP_PDU_OFF] & HCI_FEATURE_ENCAP_PDU_MASK)

#define HCI_FEATURE_ERROR_DATA_MASK     0x20
#define HCI_FEATURE_ERROR_DATA_OFF      6
#define HCI_ERROR_DATA_SUPPORTED(x)     ((x)[HCI_FEATURE_ERROR_DATA_OFF] & HCI_FEATURE_ERROR_DATA_MASK)

#define HCI_FEATURE_NON_FLUSHABLE_PB_MASK      0x40
#define HCI_FEATURE_NON_FLUSHABLE_PB_OFF       6

#define HCI_NON_FLUSHABLE_PB_SUPPORTED(x)      ((x)[HCI_FEATURE_NON_FLUSHABLE_PB_OFF] & HCI_FEATURE_NON_FLUSHABLE_PB_MASK)

#define HCI_FEATURE_LINK_SUP_TO_EVT_MASK 0x01
#define HCI_FEATURE_LINK_SUP_TO_EVT_OFF  7
#define HCI_LINK_SUP_TO_EVT_SUPPORTED(x) ((x)[HCI_FEATURE_LINK_SUP_TO_EVT_OFF] & HCI_FEATURE_LINK_SUP_TO_EVT_MASK)

#define HCI_FEATURE_INQ_RESP_TX_MASK     0x02
#define HCI_FEATURE_INQ_RESP_TX_OFF      7
#define HCI_INQ_RESP_TX_SUPPORTED(x)     ((x)[HCI_FEATURE_INQ_RESP_TX_OFF] & HCI_FEATURE_INQ_RESP_TX_MASK)

#define HCI_FEATURE_EXTENDED_MASK       0x80
#define HCI_FEATURE_EXTENDED_OFF        7
#define HCI_LMP_EXTENDED_SUPPORTED(x)   ((x)[HCI_FEATURE_EXTENDED_OFF] & HCI_FEATURE_EXTENDED_MASK)

/*
**   Features encoding - page 1
*/
#define HCI_EXT_FEATURE_SSP_HOST_MASK 0x01
#define HCI_EXT_FEATURE_SSP_HOST_OFF  0
#define HCI_SSP_HOST_SUPPORTED(x) ((x)[HCI_EXT_FEATURE_SSP_HOST_OFF] & HCI_EXT_FEATURE_SSP_HOST_MASK)

/*
**   Local Supported Commands encoding
*/
#define HCI_NUM_SUPP_COMMANDS_BYTES           64

#define HCI_SUPP_COMMANDS_INQUIRY_MASK 0x01
#define HCI_SUPP_COMMANDS_INQUIRY_OFF  0
#define HCI_INQUIRY_SUPPORTED(x) ((x)[HCI_SUPP_COMMANDS_INQUIRY_OFF] & HCI_SUPP_COMMANDS_INQUIRY_MASK)

#define HCI_SUPP_COMMANDS_INQUIRY_CANCEL_MASK 0x02
#define HCI_SUPP_COMMANDS_INQUIRY_CANCEL_OFF  0
#define HCI_INQUIRY_CANCEL_SUPPORTED(x) ((x)[HCI_SUPP_COMMANDS_INQUIRY_CANCEL_OFF] & HCI_SUPP_COMMANDS_INQUIRY_CANCEL_MASK)

#define HCI_SUPP_COMMANDS_PERIODIC_INQUIRY_MASK     0x04
#define HCI_SUPP_COMMANDS_PERIODIC_INQUIRY_OFF      0
#define HCI_PERIODIC_INQUIRY_SUPPORTED(x)     ((x)[HCI_SUPP_COMMANDS_PERIODIC_INQUIRY_OFF] & HCI_SUPP_COMMANDS_PERIODIC_INQUIRY_MASK)

#define HCI_SUPP_COMMANDS_EXIT_PERIODIC_INQUIRY_MASK    0x08
#define HCI_SUPP_COMMANDS_EXIT_PERIODIC_INQUIRY_OFF     0
#define HCI_EXIT_PERIODIC_INQUIRY_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_EXIT_PERIODIC_INQUIRY_OFF] & HCI_SUPP_COMMANDS_EXIT_PERIODIC_INQUIRY_MASK)

#define HCI_SUPP_COMMANDS_CREATE_CONN_MASK     0x10
#define HCI_SUPP_COMMANDS_CREATE_CONN_OFF      0
#define HCI_CREATE_CONN_SUPPORTED(x)     ((x)[HCI_SUPP_COMMANDS_CREATE_CONN_OFF] & HCI_SUPP_COMMANDS_CREATE_CONN_MASK)

#define HCI_SUPP_COMMANDS_DISCONNECT_MASK         0x20
#define HCI_SUPP_COMMANDS_DISCONNECT_OFF          0
#define HCI_DISCONNECT_SUPPORTED(x)         ((x)[HCI_SUPP_COMMANDS_DISCONNECT_OFF] & HCI_SUPP_COMMANDS_DISCONNECT_MASK)

#define HCI_SUPP_COMMANDS_ADD_SCO_CONN_MASK      0x40
#define HCI_SUPP_COMMANDS_ADD_SCO_CONN_OFF       0
#define HCI_ADD_SCO_CONN_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_ADD_SCO_CONN_OFF] & HCI_SUPP_COMMANDS_ADD_SCO_CONN_MASK)

#define HCI_SUPP_COMMANDS_CANCEL_CREATE_CONN_MASK     0x80
#define HCI_SUPP_COMMANDS_CANCEL_CREATE_CONN_OFF      0
#define HCI_CANCEL_CREATE_CONN_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_CANCEL_CREATE_CONN_OFF] & HCI_SUPP_COMMANDS_CANCEL_CREATE_CONN_MASK)

#define HCI_SUPP_COMMANDS_ACCEPT_CONN_REQUEST_MASK      0x01
#define HCI_SUPP_COMMANDS_ACCEPT_CONN_REQUEST_OFF       1
#define HCI_ACCEPT_CONN_REQUEST_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_ACCEPT_CONN_REQUEST_OFF] & HCI_SUPP_COMMANDS_ACCEPT_CONN_REQUEST_MASK)

#define HCI_SUPP_COMMANDS_REJECT_CONN_REQUEST_MASK           0x02
#define HCI_SUPP_COMMANDS_REJECT_CONN_REQUEST_OFF            1
#define HCI_REJECT_CONN_REQUEST_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_REJECT_CONN_REQUEST_OFF] & HCI_SUPP_COMMANDS_REJECT_CONN_REQUEST_MASK)

#define HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_REPLY_MASK  0x04
#define HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_REPLY_OFF   1
#define HCI_LINK_KEY_REQUEST_REPLY_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_REPLY_OFF] & HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_REPLY_MASK)

#define HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_NEG_REPLY_MASK       0x08
#define HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_NEG_REPLY_OFF        1
#define HCI_LINK_KEY_REQUEST_NEG_REPLY_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_NEG_REPLY_OFF] & HCI_SUPP_COMMANDS_LINK_KEY_REQUEST_NEG_REPLY_MASK)

#define HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_REPLY_MASK    0x10
#define HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_REPLY_OFF     1
#define HCI_PIN_CODE_REQUEST_REPLY_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_REPLY_OFF] & HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_REPLY_MASK)

#define HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_NEG_REPLY_MASK    0x20
#define HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_NEG_REPLY_OFF     1
#define HCI_PIN_CODE_REQUEST_NEG_REPLY_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_NEG_REPLY_OFF] & HCI_SUPP_COMMANDS_PIN_CODE_REQUEST_NEG_REPLY_MASK)

#define HCI_SUPP_COMMANDS_CHANGE_CONN_PKT_TYPE_MASK          0x40
#define HCI_SUPP_COMMANDS_CHANGE_CONN_PKT_TYPE_OFF           1
#define HCI_CHANGE_CONN_PKT_TYPE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_CHANGE_CONN_PKT_TYPE_OFF] & HCI_SUPP_COMMANDS_CHANGE_CONN_PKT_TYPE_MASK)

#define HCI_SUPP_COMMANDS_AUTH_REQUEST_MASK          0x80
#define HCI_SUPP_COMMANDS_AUTH_REQUEST_OFF           1
#define HCI_AUTH_REQUEST_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_AUTH_REQUEST_OFF] & HCI_SUPP_COMMANDS_AUTH_REQUEST_MASK)

#define HCI_SUPP_COMMANDS_SET_CONN_ENCRYPTION_MASK      0x01
#define HCI_SUPP_COMMANDS_SET_CONN_ENCRYPTION_OFF       2
#define HCI_SET_CONN_ENCRYPTION_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_SET_CONN_ENCRYPTION_OFF] & HCI_SUPP_COMMANDS_SET_CONN_ENCRYPTION_MASK)

#define HCI_SUPP_COMMANDS_CHANGE_CONN_LINK_KEY_MASK           0x02
#define HCI_SUPP_COMMANDS_CHANGE_CONN_LINK_KEY_OFF            2
#define HCI_CHANGE_CONN_LINK_KEY_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_CHANGE_CONN_LINK_KEY_OFF] & HCI_SUPP_COMMANDS_CHANGE_CONN_LINK_KEY_MASK)

#define HCI_SUPP_COMMANDS_MASTER_LINK_KEY_MASK  0x04
#define HCI_SUPP_COMMANDS_MASTER_LINK_KEY_OFF   2
#define HCI_MASTER_LINK_KEY_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_MASTER_LINK_KEY_OFF] & HCI_SUPP_COMMANDS_MASTER_LINK_KEY_MASK)

#define HCI_SUPP_COMMANDS_REMOTE_NAME_REQUEST_MASK       0x08
#define HCI_SUPP_COMMANDS_REMOTE_NAME_REQUEST_OFF        2
#define HCI_REMOTE_NAME_REQUEST_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_REMOTE_NAME_REQUEST_OFF] & HCI_SUPP_COMMANDS_REMOTE_NAME_REQUEST_MASK)

#define HCI_SUPP_COMMANDS_CANCEL_REMOTE_NAME_REQUEST_MASK    0x10
#define HCI_SUPP_COMMANDS_CANCEL_REMOTE_NAME_REQUEST_OFF     2
#define HCI_CANCEL_REMOTE_NAME_REQUEST_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_CANCEL_REMOTE_NAME_REQUEST_OFF] & HCI_SUPP_COMMANDS_CANCEL_REMOTE_NAME_REQUEST_MASK)

#define HCI_SUPP_COMMANDS_READ_REMOTE_SUPP_FEATURES_MASK    0x20
#define HCI_SUPP_COMMANDS_READ_REMOTE_SUPP_FEATURES_OFF     2
#define HCI_READ_REMOTE_SUPP_FEATURES_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_REMOTE_SUPP_FEATURES_OFF] & HCI_SUPP_COMMANDS_READ_REMOTE_SUPP_FEATURES_MASK)

#define HCI_SUPP_COMMANDS_READ_REMOTE_EXT_FEATURES_MASK          0x40
#define HCI_SUPP_COMMANDS_READ_REMOTE_EXT_FEATURES_OFF           2
#define HCI_READ_REMOTE_EXT_FEATURES_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_REMOTE_EXT_FEATURES_OFF] & HCI_SUPP_COMMANDS_READ_REMOTE_EXT_FEATURES_MASK)

#define HCI_SUPP_COMMANDS_READ_REMOTE_VER_INFO_MASK          0x80
#define HCI_SUPP_COMMANDS_READ_REMOTE_VER_INFO_OFF           2
#define HCI_READ_REMOTE_VER_INFO_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_REMOTE_VER_INFO_OFF] & HCI_SUPP_COMMANDS_READ_REMOTE_VER_INFO_MASK)

#define HCI_SUPP_COMMANDS_READ_CLOCK_OFFSET_MASK           0x01
#define HCI_SUPP_COMMANDS_READ_CLOCK_OFFSET_OFF            3
#define HCI_READ_CLOCK_OFFSET_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_READ_CLOCK_OFFSET_OFF] & HCI_SUPP_COMMANDS_READ_CLOCK_OFFSET_MASK)

#define HCI_SUPP_COMMANDS_READ_LMP_HANDLE_MASK  0x02
#define HCI_SUPP_COMMANDS_READ_LMP_HANDLE_OFF   3
#define HCI_READ_LMP_HANDLE_SUPPORTED(x) ((x)[HCI_SUPP_COMMANDS_READ_LMP_HANDLE_OFF] & HCI_SUPP_COMMANDS_READ_LMP_HANDLE_MASK)

#define HCI_SUPP_COMMANDS_HOLD_MODE_CMD_MASK           0x02
#define HCI_SUPP_COMMANDS_HOLD_MODE_CMD_OFF            4
#define HCI_HOLD_MODE_CMD_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_HOLD_MODE_CMD_OFF] & HCI_SUPP_COMMANDS_HOLD_MODE_CMD_MASK)

#define HCI_SUPP_COMMANDS_SNIFF_MODE_CMD_MASK  0x04
#define HCI_SUPP_COMMANDS_SNIFF_MODE_CMD_OFF   4
#define HCI_SNIFF_MODE_CMD_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_SNIFF_MODE_CMD_OFF] & HCI_SUPP_COMMANDS_SNIFF_MODE_CMD_MASK)

#define HCI_SUPP_COMMANDS_EXIT_SNIFF_MODE_MASK       0x08
#define HCI_SUPP_COMMANDS_EXIT_SNIFF_MODE_OFF        4
#define HCI_EXIT_SNIFF_MODE_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_EXIT_SNIFF_MODE_OFF] & HCI_SUPP_COMMANDS_EXIT_SNIFF_MODE_MASK)

#define HCI_SUPP_COMMANDS_PARK_STATE_MASK    0x10
#define HCI_SUPP_COMMANDS_PARK_STATE_OFF     4
#define HCI_PARK_STATE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_PARK_STATE_OFF] & HCI_SUPP_COMMANDS_PARK_STATE_MASK)

#define HCI_SUPP_COMMANDS_EXIT_PARK_STATE_MASK    0x20
#define HCI_SUPP_COMMANDS_EXIT_PARK_STATE_OFF     4
#define HCI_EXIT_PARK_STATE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_EXIT_PARK_STATE_OFF] & HCI_SUPP_COMMANDS_EXIT_PARK_STATE_MASK)

#define HCI_SUPP_COMMANDS_QOS_SETUP_MASK          0x40
#define HCI_SUPP_COMMANDS_QOS_SETUP_OFF           4
#define HCI_QOS_SETUP_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_QOS_SETUP_OFF] & HCI_SUPP_COMMANDS_QOS_SETUP_MASK)

#define HCI_SUPP_COMMANDS_ROLE_DISCOVERY_MASK          0x80
#define HCI_SUPP_COMMANDS_ROLE_DISCOVERY_OFF           4
#define HCI_ROLE_DISCOVERY_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_ROLE_DISCOVERY_OFF] & HCI_SUPP_COMMANDS_ROLE_DISCOVERY_MASK)

#define HCI_SUPP_COMMANDS_SWITCH_ROLE_MASK      0x01
#define HCI_SUPP_COMMANDS_SWITCH_ROLE_OFF       5
#define HCI_SWITCH_ROLE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_SWITCH_ROLE_OFF] & HCI_SUPP_COMMANDS_SWITCH_ROLE_MASK)

#define HCI_SUPP_COMMANDS_READ_LINK_POLICY_SET_MASK           0x02
#define HCI_SUPP_COMMANDS_READ_LINK_POLICY_SET_OFF            5
#define HCI_READ_LINK_POLICY_SET_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_READ_LINK_POLICY_SET_OFF] & HCI_SUPP_COMMANDS_READ_LINK_POLICY_SET_MASK)

#define HCI_SUPP_COMMANDS_WRITE_LINK_POLICY_SET_MASK  0x04
#define HCI_SUPP_COMMANDS_WRITE_LINK_POLICY_SET_OFF   5
#define HCI_WRITE_LINK_POLICY_SET_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_WRITE_LINK_POLICY_SET_OFF] & HCI_SUPP_COMMANDS_WRITE_LINK_POLICY_SET_MASK)

#define HCI_SUPP_COMMANDS_READ_DEF_LINK_POLICY_SET_MASK       0x08
#define HCI_SUPP_COMMANDS_READ_DEF_LINK_POLICY_SET_OFF        5
#define HCI_READ_DEF_LINK_POLICY_SET_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_READ_DEF_LINK_POLICY_SET_OFF] & HCI_SUPP_COMMANDS_READ_DEF_LINK_POLICY_SET_MASK)

#define HCI_SUPP_COMMANDS_WRITE_DEF_LINK_POLICY_SET_MASK    0x10
#define HCI_SUPP_COMMANDS_WRITE_DEF_LINK_POLICY_SET_OFF     5
#define HCI_WRITE_DEF_LINK_POLICY_SET_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_WRITE_DEF_LINK_POLICY_SET_OFF] & HCI_SUPP_COMMANDS_WRITE_DEF_LINK_POLICY_SET_MASK)

#define HCI_SUPP_COMMANDS_FLOW_SPECIFICATION_MASK    0x20
#define HCI_SUPP_COMMANDS_FLOW_SPECIFICATION_OFF     5
#define HCI_FLOW_SPECIFICATION_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_FLOW_SPECIFICATION_OFF] & HCI_SUPP_COMMANDS_FLOW_SPECIFICATION_MASK)

#define HCI_SUPP_COMMANDS_SET_EVENT_MASK_MASK          0x40
#define HCI_SUPP_COMMANDS_SET_EVENT_MASK_OFF           5
#define HCI_SET_EVENT_MASK_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_SET_EVENT_MASK_OFF] & HCI_SUPP_COMMANDS_SET_EVENT_MASK_MASK)

#define HCI_SUPP_COMMANDS_RESET_MASK          0x80
#define HCI_SUPP_COMMANDS_RESET_OFF           5
#define HCI_RESET_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_RESET_OFF] & HCI_SUPP_COMMANDS_RESET_MASK)

#define HCI_SUPP_COMMANDS_SET_EVENT_FILTER_MASK      0x01
#define HCI_SUPP_COMMANDS_SET_EVENT_FILTER_OFF       6
#define HCI_SET_EVENT_FILTER_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_SET_EVENT_FILTER_OFF] & HCI_SUPP_COMMANDS_SET_EVENT_FILTER_MASK)

#define HCI_SUPP_COMMANDS_FLUSH_MASK           0x02
#define HCI_SUPP_COMMANDS_FLUSH_OFF            6
#define HCI_FLUSH_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_FLUSH_OFF] & HCI_SUPP_COMMANDS_FLUSH_MASK)

#define HCI_SUPP_COMMANDS_READ_PIN_TYPE_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_PIN_TYPE_OFF   6
#define HCI_READ_PIN_TYPE_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_PIN_TYPE_OFF] & HCI_SUPP_COMMANDS_READ_PIN_TYPE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_PIN_TYPE_MASK       0x08
#define HCI_SUPP_COMMANDS_WRITE_PIN_TYPE_OFF        6
#define HCI_WRITE_PIN_TYPE_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_WRITE_PIN_TYPE_OFF] & HCI_SUPP_COMMANDS_WRITE_PIN_TYPE_MASK)

#define HCI_SUPP_COMMANDS_CREATE_NEW_UNIT_KEY_MASK    0x10
#define HCI_SUPP_COMMANDS_CREATE_NEW_UNIT_KEY_OFF     6
#define HCI_CREATE_NEW_UNIT_KEY_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_CREATE_NEW_UNIT_KEY_OFF] & HCI_SUPP_COMMANDS_CREATE_NEW_UNIT_KEY_MASK)

#define HCI_SUPP_COMMANDS_READ_STORED_LINK_KEY_MASK    0x20
#define HCI_SUPP_COMMANDS_READ_STORED_LINK_KEY_OFF     6
#define HCI_READ_STORED_LINK_KEY_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_STORED_LINK_KEY_OFF] & HCI_SUPP_COMMANDS_READ_STORED_LINK_KEY_MASK)

#define HCI_SUPP_COMMANDS_WRITE_STORED_LINK_KEY_MASK          0x40
#define HCI_SUPP_COMMANDS_WRITE_STORED_LINK_KEY_OFF           6
#define HCI_WRITE_STORED_LINK_KEY_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_STORED_LINK_KEY_OFF] & HCI_SUPP_COMMANDS_WRITE_STORED_LINK_KEY_MASK)

#define HCI_SUPP_COMMANDS_DELETE_STORED_LINK_KEY_MASK          0x80
#define HCI_SUPP_COMMANDS_DELETE_STORED_LINK_KEY_OFF           6
#define HCI_DELETE_STORED_LINK_KEY_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_DELETE_STORED_LINK_KEY_OFF] & HCI_SUPP_COMMANDS_DELETE_STORED_LINK_KEY_MASK)

#define HCI_SUPP_COMMANDS_WRITE_LOCAL_NAME_MASK      0x01
#define HCI_SUPP_COMMANDS_WRITE_LOCAL_NAME_OFF       7
#define HCI_WRITE_LOCAL_NAME_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_LOCAL_NAME_OFF] & HCI_SUPP_COMMANDS_WRITE_LOCAL_NAME_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_NAME_MASK           0x02
#define HCI_SUPP_COMMANDS_READ_LOCAL_NAME_OFF            7
#define HCI_READ_LOCAL_NAME_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_NAME_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_NAME_MASK)

#define HCI_SUPP_COMMANDS_READ_CONN_ACCEPT_TOUT_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_CONN_ACCEPT_TOUT_OFF   7
#define HCI_READ_CONN_ACCEPT_TOUT_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_CONN_ACCEPT_TOUT_OFF] & HCI_SUPP_COMMANDS_READ_CONN_ACCEPT_TOUT_MASK)

#define HCI_SUPP_COMMANDS_WRITE_CONN_ACCEPT_TOUT_MASK       0x08
#define HCI_SUPP_COMMANDS_WRITE_CONN_ACCEPT_TOUT_OFF        7
#define HCI_WRITE_CONN_ACCEPT_TOUT_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_WRITE_CONN_ACCEPT_TOUT_OFF] & HCI_SUPP_COMMANDS_WRITE_CONN_ACCEPT_TOUT_MASK)

#define HCI_SUPP_COMMANDS_READ_PAGE_TOUT_MASK    0x10
#define HCI_SUPP_COMMANDS_READ_PAGE_TOUT_OFF     7
#define HCI_READ_PAGE_TOUT_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_PAGE_TOUT_OFF] & HCI_SUPP_COMMANDS_READ_PAGE_TOUT_MASK)

#define HCI_SUPP_COMMANDS_WRITE_PAGE_TOUT_MASK    0x20
#define HCI_SUPP_COMMANDS_WRITE_PAGE_TOUT_OFF     7
#define HCI_WRITE_PAGE_TOUT_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_WRITE_PAGE_TOUT_OFF] & HCI_SUPP_COMMANDS_WRITE_PAGE_TOUT_MASK)

#define HCI_SUPP_COMMANDS_READ_SCAN_ENABLE_MASK          0x40
#define HCI_SUPP_COMMANDS_READ_SCAN_ENABLE_OFF           7
#define HCI_READ_SCAN_ENABLE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_SCAN_ENABLE_OFF] & HCI_SUPP_COMMANDS_READ_SCAN_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_SCAN_ENABLE_MASK          0x80
#define HCI_SUPP_COMMANDS_WRITE_SCAN_ENABLE_OFF           7
#define HCI_WRITE_SCAN_ENABLE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_SCAN_ENABLE_OFF] & HCI_SUPP_COMMANDS_WRITE_SCAN_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_ACTIVITY_MASK      0x01
#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_ACTIVITY_OFF       8
#define HCI_READ_PAGE_SCAN_ACTIVITY_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_PAGE_SCAN_ACTIVITY_OFF] & HCI_SUPP_COMMANDS_READ_PAGE_SCAN_ACTIVITY_MASK)

#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_ACTIVITY_MASK           0x02
#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_ACTIVITY_OFF            8
#define HCI_WRITE_PAGE_SCAN_ACTIVITY_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_ACTIVITY_OFF] & HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_ACTIVITY_MASK)

#define HCI_SUPP_COMMANDS_READ_INQURIY_SCAN_ACTIVITY_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_INQURIY_SCAN_ACTIVITY_OFF   8
#define HCI_READ_INQURIY_SCAN_ACTIVITY_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_INQURIY_SCAN_ACTIVITY_OFF] & HCI_SUPP_COMMANDS_READ_INQURIY_SCAN_ACTIVITY_MASK)

#define HCI_SUPP_COMMANDS_WRITE_INQURIY_SCAN_ACTIVITY_MASK       0x08
#define HCI_SUPP_COMMANDS_WRITE_INQURIY_SCAN_ACTIVITY_OFF        8
#define HCI_WRITE_INQURIY_SCAN_ACTIVITY_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_WRITE_INQURIY_SCAN_ACTIVITY_OFF] & HCI_SUPP_COMMANDS_WRITE_INQURIY_SCAN_ACTIVITY_MASK)

#define HCI_SUPP_COMMANDS_READ_AUTH_ENABLE_MASK    0x10
#define HCI_SUPP_COMMANDS_READ_AUTH_ENABLE_OFF     8
#define HCI_READ_AUTH_ENABLE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_AUTH_ENABLE_OFF] & HCI_SUPP_COMMANDS_READ_AUTH_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_AUTH_ENABLE_MASK    0x20
#define HCI_SUPP_COMMANDS_WRITE_AUTH_ENABLE_OFF     8
#define HCI_WRITE_AUTH_ENABLE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_WRITE_AUTH_ENABLE_OFF] & HCI_SUPP_COMMANDS_WRITE_AUTH_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_READ_ENCRYPT_ENABLE_MASK          0x40
#define HCI_SUPP_COMMANDS_READ_ENCRYPT_ENABLE_OFF           8
#define HCI_READ_ENCRYPT_ENABLE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_ENCRYPT_ENABLE_OFF] & HCI_SUPP_COMMANDS_READ_ENCRYPT_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_ENCRYPT_ENABLE_MASK          0x80
#define HCI_SUPP_COMMANDS_WRITE_ENCRYPT_ENABLE_OFF           8
#define HCI_WRITE_ENCRYPT_ENABLE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_ENCRYPT_ENABLE_OFF] & HCI_SUPP_COMMANDS_WRITE_ENCRYPT_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_READ_CLASS_DEVICE_MASK      0x01
#define HCI_SUPP_COMMANDS_READ_CLASS_DEVICE_OFF       9
#define HCI_READ_CLASS_DEVICE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_CLASS_DEVICE_OFF] & HCI_SUPP_COMMANDS_READ_CLASS_DEVICE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_CLASS_DEVICE_MASK           0x02
#define HCI_SUPP_COMMANDS_WRITE_CLASS_DEVICE_OFF            9
#define HCI_WRITE_CLASS_DEVICE_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_WRITE_CLASS_DEVICE_OFF] & HCI_SUPP_COMMANDS_WRITE_CLASS_DEVICE_MASK)

#define HCI_SUPP_COMMANDS_READ_VOICE_SETTING_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_VOICE_SETTING_OFF   9
#define HCI_READ_VOICE_SETTING_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_VOICE_SETTING_OFF] & HCI_SUPP_COMMANDS_READ_VOICE_SETTING_MASK)

#define HCI_SUPP_COMMANDS_WRITE_VOICE_SETTING_MASK       0x08
#define HCI_SUPP_COMMANDS_WRITE_VOICE_SETTING_OFF        9
#define HCI_WRITE_VOICE_SETTING_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_WRITE_VOICE_SETTING_OFF] & HCI_SUPP_COMMANDS_WRITE_VOICE_SETTING_MASK)

#define HCI_SUPP_COMMANDS_READ_AUTO_FLUSH_TOUT_MASK    0x10
#define HCI_SUPP_COMMANDS_READ_AUTO_FLUSH_TOUT_OFF     9
#define HCI_READ_AUTO_FLUSH_TOUT_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_AUTO_FLUSH_TOUT_OFF] & HCI_SUPP_COMMANDS_READ_AUTO_FLUSH_TOUT_MASK)

#define HCI_SUPP_COMMANDS_WRITE_AUTO_FLUSH_TOUT_MASK    0x20
#define HCI_SUPP_COMMANDS_WRITE_AUTO_FLUSH_TOUT_OFF     9
#define HCI_WRITE_AUTO_FLUSH_TOUT_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_WRITE_AUTO_FLUSH_TOUT_OFF] & HCI_SUPP_COMMANDS_WRITE_AUTO_FLUSH_TOUT_MASK)

#define HCI_SUPP_COMMANDS_READ_NUM_BROAD_RETRANS_MASK          0x40
#define HCI_SUPP_COMMANDS_READ_NUM_BROAD_RETRANS_OFF           9
#define HCI_READ_NUM_BROAD_RETRANS_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_NUM_BROAD_RETRANS_OFF] & HCI_SUPP_COMMANDS_READ_NUM_BROAD_RETRANS_MASK)

#define HCI_SUPP_COMMANDS_WRITE_NUM_BROAD_RETRANS_MASK          0x80
#define HCI_SUPP_COMMANDS_WRITE_NUM_BROAD_RETRANS_OFF           9
#define HCI_WRITE_NUM_BROAD_RETRANS_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_NUM_BROAD_RETRANS_OFF] & HCI_SUPP_COMMANDS_WRITE_NUM_BROAD_RETRANS_MASK)

#define HCI_SUPP_COMMANDS_READ_HOLD_MODE_ACTIVITY_MASK      0x01
#define HCI_SUPP_COMMANDS_READ_HOLD_MODE_ACTIVITY_OFF       10
#define HCI_READ_HOLD_MODE_ACTIVITY_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_HOLD_MODE_ACTIVITY_OFF] & HCI_SUPP_COMMANDS_READ_HOLD_MODE_ACTIVITY_MASK)

#define HCI_SUPP_COMMANDS_WRITE_HOLD_MODE_ACTIVITY_MASK           0x02
#define HCI_SUPP_COMMANDS_WRITE_HOLD_MODE_ACTIVITY_OFF            10
#define HCI_WRITE_HOLD_MODE_ACTIVITY_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_WRITE_HOLD_MODE_ACTIVITY_OFF] & HCI_SUPP_COMMANDS_WRITE_HOLD_MODE_ACTIVITY_MASK)

#define HCI_SUPP_COMMANDS_READ_TRANS_PWR_LEVEL_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_TRANS_PWR_LEVEL_OFF   10
#define HCI_READ_TRANS_PWR_LEVEL_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_TRANS_PWR_LEVEL_OFF] & HCI_SUPP_COMMANDS_READ_TRANS_PWR_LEVEL_MASK)

#define HCI_SUPP_COMMANDS_READ_SYNCH_FLOW_CTRL_ENABLE_MASK       0x08
#define HCI_SUPP_COMMANDS_READ_SYNCH_FLOW_CTRL_ENABLE_OFF        10
#define HCI_READ_SYNCH_FLOW_CTRL_ENABLE_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_READ_SYNCH_FLOW_CTRL_ENABLE_OFF] & HCI_SUPP_COMMANDS_READ_SYNCH_FLOW_CTRL_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_SYNCH_FLOW_CTRL_ENABLE_MASK    0x10
#define HCI_SUPP_COMMANDS_WRITE_SYNCH_FLOW_CTRL_ENABLE_OFF     10
#define HCI_WRITE_SYNCH_FLOW_CTRL_ENABLE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_WRITE_SYNCH_FLOW_CTRL_ENABLE_OFF] & HCI_SUPP_COMMANDS_WRITE_SYNCH_FLOW_CTRL_ENABLE_MASK)

#define HCI_SUPP_COMMANDS_SET_HOST_CTRLR_TO_HOST_FC_MASK    0x20
#define HCI_SUPP_COMMANDS_SET_HOST_CTRLR_TO_HOST_FC_OFF     10
#define HCI_SET_HOST_CTRLR_TO_HOST_FC_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_SET_HOST_CTRLR_TO_HOST_FC_OFF] & HCI_SUPP_COMMANDS_SET_HOST_CTRLR_TO_HOST_FC_MASK)

#define HCI_SUPP_COMMANDS_HOST_BUFFER_SIZE_MASK          0x40
#define HCI_SUPP_COMMANDS_HOST_BUFFER_SIZE_OFF           10
#define HCI_HOST_BUFFER_SIZE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_HOST_BUFFER_SIZE_OFF] & HCI_SUPP_COMMANDS_HOST_BUFFER_SIZE_MASK)

#define HCI_SUPP_COMMANDS_HOST_NUM_COMPLETED_PKTS_MASK          0x80
#define HCI_SUPP_COMMANDS_HOST_NUM_COMPLETED_PKTS_OFF           10
#define HCI_HOST_NUM_COMPLETED_PKTS_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_HOST_NUM_COMPLETED_PKTS_OFF] & HCI_SUPP_COMMANDS_HOST_NUM_COMPLETED_PKTS_MASK)

#define HCI_SUPP_COMMANDS_READ_LINK_SUP_TOUT_MASK      0x01
#define HCI_SUPP_COMMANDS_READ_LINK_SUP_TOUT_OFF       11
#define HCI_READ_LINK_SUP_TOUT_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_LINK_SUP_TOUT_OFF] & HCI_SUPP_COMMANDS_READ_LINK_SUP_TOUT_MASK)

#define HCI_SUPP_COMMANDS_WRITE_LINK_SUP_TOUT_MASK           0x02
#define HCI_SUPP_COMMANDS_WRITE_LINK_SUP_TOUT_OFF            11
#define HCI_WRITE_LINK_SUP_TOUT_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_WRITE_LINK_SUP_TOUT_OFF] & HCI_SUPP_COMMANDS_WRITE_LINK_SUP_TOUT_MASK)

#define HCI_SUPP_COMMANDS_READ_NUM_SUPP_IAC_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_NUM_SUPP_IAC_OFF   11
#define HCI_READ_NUM_SUPP_IAC_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_NUM_SUPP_IAC_OFF] & HCI_SUPP_COMMANDS_READ_NUM_SUPP_IAC_MASK)

#define HCI_SUPP_COMMANDS_READ_CURRENT_IAC_LAP_MASK       0x08
#define HCI_SUPP_COMMANDS_READ_CURRENT_IAC_LAP_OFF        11
#define HCI_READ_CURRENT_IAC_LAP_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_READ_CURRENT_IAC_LAP_OFF] & HCI_SUPP_COMMANDS_READ_CURRENT_IAC_LAP_MASK)

#define HCI_SUPP_COMMANDS_WRITE_CURRENT_IAC_LAP_MASK    0x10
#define HCI_SUPP_COMMANDS_WRITE_CURRENT_IAC_LAP_OFF     11
#define HCI_WRITE_CURRENT_IAC_LAP_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_WRITE_CURRENT_IAC_LAP_OFF] & HCI_SUPP_COMMANDS_WRITE_CURRENT_IAC_LAP_MASK)

#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_PER_MODE_MASK    0x20
#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_PER_MODE_OFF     11
#define HCI_READ_PAGE_SCAN_PER_MODE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_PAGE_SCAN_PER_MODE_OFF] & HCI_SUPP_COMMANDS_READ_PAGE_SCAN_PER_MODE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_PER_MODE_MASK          0x40
#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_PER_MODE_OFF           11
#define HCI_WRITE_PAGE_SCAN_PER_MODE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_PER_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_PER_MODE_MASK)

#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_MODE_MASK          0x80
#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_MODE_OFF           11
#define HCI_READ_PAGE_SCAN_MODE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_PAGE_SCAN_MODE_OFF] & HCI_SUPP_COMMANDS_READ_PAGE_SCAN_MODE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_MODE_MASK      0x01
#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_MODE_OFF       12
#define HCI_WRITE_PAGE_SCAN_MODE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_MODE_MASK)

#define HCI_SUPP_COMMANDS_SET_AFH_CHNL_CLASS_MASK           0x02
#define HCI_SUPP_COMMANDS_SET_AFH_CHNL_CLASS_OFF            12
#define HCI_SET_AFH_CHNL_CLASS_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_SET_AFH_CHNL_CLASS_OFF] & HCI_SUPP_COMMANDS_SET_AFH_CHNL_CLASS_MASK)

#define HCI_SUPP_COMMANDS_READ_INQUIRY_SCAN_TYPE_MASK    0x10
#define HCI_SUPP_COMMANDS_READ_INQUIRY_SCAN_TYPE_OFF     12
#define HCI_READ_INQUIRY_SCAN_TYPE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_INQUIRY_SCAN_TYPE_OFF] & HCI_SUPP_COMMANDS_READ_INQUIRY_SCAN_TYPE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_INQUIRY_SCAN_TYPE_MASK    0x20
#define HCI_SUPP_COMMANDS_WRITE_INQUIRY_SCAN_TYPE_OFF     12
#define HCI_WRITE_INQUIRY_SCAN_TYPE_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_WRITE_INQUIRY_SCAN_TYPE_OFF] & HCI_SUPP_COMMANDS_WRITE_INQUIRY_SCAN_TYPE_MASK)

#define HCI_SUPP_COMMANDS_READ_INQUIRY_MODE_MASK          0x40
#define HCI_SUPP_COMMANDS_READ_INQUIRY_MODE_OFF           12
#define HCI_READ_INQUIRY_MODE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_INQUIRY_MODE_OFF] & HCI_SUPP_COMMANDS_READ_INQUIRY_MODE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_INQUIRY_MODE_MASK          0x80
#define HCI_SUPP_COMMANDS_WRITE_INQUIRY_MODE_OFF           12
#define HCI_WRITE_INQUIRY_MODE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_WRITE_INQUIRY_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_INQUIRY_MODE_MASK)

#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_TYPE_MASK      0x01
#define HCI_SUPP_COMMANDS_READ_PAGE_SCAN_TYPE_OFF       13
#define HCI_READ_PAGE_SCAN_TYPE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_PAGE_SCAN_TYPE_OFF] & HCI_SUPP_COMMANDS_READ_PAGE_SCAN_TYPE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_TYPE_MASK           0x02
#define HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_TYPE_OFF            13
#define HCI_WRITE_PAGE_SCAN_TYPE_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_TYPE_OFF] & HCI_SUPP_COMMANDS_WRITE_PAGE_SCAN_TYPE_MASK)

#define HCI_SUPP_COMMANDS_READ_AFH_CHNL_ASSESS_MODE_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_AFH_CHNL_ASSESS_MODE_OFF   13
#define HCI_READ_AFH_CHNL_ASSESS_MODE_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_AFH_CHNL_ASSESS_MODE_OFF] & HCI_SUPP_COMMANDS_READ_AFH_CHNL_ASSESS_MODE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_AFH_CHNL_ASSESS_MODE_MASK       0x08
#define HCI_SUPP_COMMANDS_WRITE_AFH_CHNL_ASSESS_MODE_OFF        13
#define HCI_WRITE_AFH_CHNL_ASSESS_MODE_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_WRITE_AFH_CHNL_ASSESS_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_AFH_CHNL_ASSESS_MODE_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_VER_INFO_MASK       0x08
#define HCI_SUPP_COMMANDS_READ_LOCAL_VER_INFO_OFF        14
#define HCI_READ_LOCAL_VER_INFO_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_VER_INFO_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_VER_INFO_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_SUP_CMDS_MASK       0x10
#define HCI_SUPP_COMMANDS_READ_LOCAL_SUP_CMDS_OFF        14
#define HCI_READ_LOCAL_SUP_CMDS_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_SUP_CMDS_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_SUP_CMDS_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_SUPP_FEATURES_MASK    0x20
#define HCI_SUPP_COMMANDS_READ_LOCAL_SUPP_FEATURES_OFF     14
#define HCI_READ_LOCAL_SUPP_FEATURES_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_SUPP_FEATURES_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_SUPP_FEATURES_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_EXT_FEATURES_MASK          0x40
#define HCI_SUPP_COMMANDS_READ_LOCAL_EXT_FEATURES_OFF           14
#define HCI_READ_LOCAL_EXT_FEATURES_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_EXT_FEATURES_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_EXT_FEATURES_MASK)

#define HCI_SUPP_COMMANDS_READ_BUFFER_SIZE_MASK          0x80
#define HCI_SUPP_COMMANDS_READ_BUFFER_SIZE_OFF           14
#define HCI_READ_BUFFER_SIZE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_BUFFER_SIZE_OFF] & HCI_SUPP_COMMANDS_READ_BUFFER_SIZE_MASK)

#define HCI_SUPP_COMMANDS_READ_COUNTRY_CODE_MASK      0x01
#define HCI_SUPP_COMMANDS_READ_COUNTRY_CODE_OFF       15
#define HCI_READ_COUNTRY_CODE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_COUNTRY_CODE_OFF] & HCI_SUPP_COMMANDS_READ_COUNTRY_CODE_MASK)

#define HCI_SUPP_COMMANDS_READ_BD_ADDR_MASK           0x02
#define HCI_SUPP_COMMANDS_READ_BD_ADDR_OFF            15
#define HCI_READ_BD_ADDR_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_READ_BD_ADDR_OFF] & HCI_SUPP_COMMANDS_READ_BD_ADDR_MASK)

#define HCI_SUPP_COMMANDS_READ_FAIL_CONTACT_CNTR_MASK  0x04
#define HCI_SUPP_COMMANDS_READ_FAIL_CONTACT_CNTR_OFF   15
#define HCI_READ_FAIL_CONTACT_CNTR_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_READ_FAIL_CONTACT_CNTR_OFF] & HCI_SUPP_COMMANDS_READ_FAIL_CONTACT_CNTR_MASK)

#define HCI_SUPP_COMMANDS_RESET_FAIL_CONTACT_CNTR_MASK       0x08
#define HCI_SUPP_COMMANDS_RESET_FAIL_CONTACT_CNTR_OFF        15
#define HCI_RESET_FAIL_CONTACT_CNTR_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_RESET_FAIL_CONTACT_CNTR_OFF] & HCI_SUPP_COMMANDS_RESET_FAIL_CONTACT_CNTR_MASK)

#define HCI_SUPP_COMMANDS_GET_LINK_QUALITY_MASK    0x10
#define HCI_SUPP_COMMANDS_GET_LINK_QUALITY_OFF     15
#define HCI_GET_LINK_QUALITY_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_GET_LINK_QUALITY_OFF] & HCI_SUPP_COMMANDS_GET_LINK_QUALITY_MASK)

#define HCI_SUPP_COMMANDS_READ_RSSI_MASK    0x20
#define HCI_SUPP_COMMANDS_READ_RSSI_OFF     15
#define HCI_READ_RSSI_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_READ_RSSI_OFF] & HCI_SUPP_COMMANDS_READ_RSSI_MASK)

#define HCI_SUPP_COMMANDS_READ_AFH_CH_MAP_MASK          0x40
#define HCI_SUPP_COMMANDS_READ_AFH_CH_MAP_OFF           15
#define HCI_READ_AFH_CH_MAP_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_AFH_CH_MAP_OFF] & HCI_SUPP_COMMANDS_READ_AFH_CH_MAP_MASK)

#define HCI_SUPP_COMMANDS_READ_BD_CLOCK_MASK          0x80
#define HCI_SUPP_COMMANDS_READ_BD_CLOCK_OFF           15
#define HCI_READ_BD_CLOCK_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_BD_CLOCK_OFF] & HCI_SUPP_COMMANDS_READ_BD_CLOCK_MASK)

#define HCI_SUPP_COMMANDS_READ_LOOPBACK_MODE_MASK      0x01
#define HCI_SUPP_COMMANDS_READ_LOOPBACK_MODE_OFF       16
#define HCI_READ_LOOPBACK_MODE_SUPPORTED(x)      ((x)[HCI_SUPP_COMMANDS_READ_LOOPBACK_MODE_OFF] & HCI_SUPP_COMMANDS_READ_LOOPBACK_MODE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_LOOPBACK_MODE_MASK           0x02
#define HCI_SUPP_COMMANDS_WRITE_LOOPBACK_MODE_OFF            16
#define HCI_WRITE_LOOPBACK_MODE_SUPPORTED(x)           ((x)[HCI_SUPP_COMMANDS_WRITE_LOOPBACK_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_LOOPBACK_MODE_MASK)

#define HCI_SUPP_COMMANDS_ENABLE_DEV_UNDER_TEST_MASK  0x04
#define HCI_SUPP_COMMANDS_ENABLE_DEV_UNDER_TEST_OFF   16
#define HCI_ENABLE_DEV_UNDER_TEST_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_ENABLE_DEV_UNDER_TEST_OFF] & HCI_SUPP_COMMANDS_ENABLE_DEV_UNDER_TEST_MASK)

#define HCI_SUPP_COMMANDS_SETUP_SYNCH_CONN_MASK       0x08
#define HCI_SUPP_COMMANDS_SETUP_SYNCH_CONN_OFF        16
#define HCI_SETUP_SYNCH_CONN_SUPPORTED(x)       ((x)[HCI_SUPP_COMMANDS_SETUP_SYNCH_CONN_OFF] & HCI_SUPP_COMMANDS_SETUP_SYNCH_CONN_MASK)

#define HCI_SUPP_COMMANDS_ACCEPT_SYNCH_CONN_MASK    0x10
#define HCI_SUPP_COMMANDS_ACCEPT_SYNCH_CONN_OFF     16
#define HCI_ACCEPT_SYNCH_CONN_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_ACCEPT_SYNCH_CONN_OFF] & HCI_SUPP_COMMANDS_ACCEPT_SYNCH_CONN_MASK)

#define HCI_SUPP_COMMANDS_REJECT_SYNCH_CONN_MASK    0x20
#define HCI_SUPP_COMMANDS_REJECT_SYNCH_CONN_OFF     16
#define HCI_REJECT_SYNCH_CONN_SUPPORTED(x)    ((x)[HCI_SUPP_COMMANDS_REJECT_SYNCH_CONN_OFF] & HCI_SUPP_COMMANDS_REJECT_SYNCH_CONN_MASK)

#define HCI_SUPP_COMMANDS_READ_EXT_INQUIRY_RESP_MASK   0x01
#define HCI_SUPP_COMMANDS_READ_EXT_INQUIRY_RESP_OFF    17
#define HCI_READ_EXT_INQUIRY_RESP_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_EXT_INQUIRY_RESP_OFF] & HCI_SUPP_COMMANDS_READ_EXT_INQUIRY_RESP_MASK)

#define HCI_SUPP_COMMANDS_WRITE_EXT_INQUIRY_RESP_MASK  0x02
#define HCI_SUPP_COMMANDS_WRITE_EXT_INQUIRY_RESP_OFF   17
#define HCI_WRITE_EXT_INQUIRY_RESP_SUPPORTED(x)  ((x)[HCI_SUPP_COMMANDS_WRITE_EXT_INQUIRY_RESP_OFF] & HCI_SUPP_COMMANDS_WRITE_EXT_INQUIRY_RESP_MASK)

#define HCI_SUPP_COMMANDS_REFRESH_ENCRYPTION_KEY_MASK   0x04
#define HCI_SUPP_COMMANDS_REFRESH_ENCRYPTION_KEY_OFF    17
#define HCI_REFRESH_ENCRYPTION_KEY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_REFRESH_ENCRYPTION_KEY_OFF] & HCI_SUPP_COMMANDS_REFRESH_ENCRYPTION_KEY_MASK)

/* Octet 17, bit 3 is reserved */

#define HCI_SUPP_COMMANDS_SNIFF_SUB_RATE_MASK       0x10
#define HCI_SUPP_COMMANDS_SNIFF_SUB_RATE_OFF        17
#define HCI_SNIFF_SUB_RATE_CMD_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_SNIFF_SUB_RATE_OFF] & HCI_SUPP_COMMANDS_SNIFF_SUB_RATE_MASK)

#define HCI_SUPP_COMMANDS_READ_SIMPLE_PAIRING_MODE_MASK   0x20
#define HCI_SUPP_COMMANDS_READ_SIMPLE_PAIRING_MODE_OFF    17
#define HCI_READ_SIMPLE_PAIRING_MODE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_SIMPLE_PAIRING_MODE_OFF] & HCI_SUPP_COMMANDS_READ_SIMPLE_PAIRING_MODE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_MODE_MASK   0x40
#define HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_MODE_OFF    17
#define HCI_WRITE_SIMPLE_PAIRING_MODE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_MODE_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_OOB_DATA_MASK   0x80
#define HCI_SUPP_COMMANDS_READ_LOCAL_OOB_DATA_OFF    17
#define HCI_READ_LOCAL_OOB_DATA_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_OOB_DATA_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_OOB_DATA_MASK)

#define HCI_SUPP_COMMANDS_READ_INQUIRY_RESPONSE_TX_POWER_MASK   0x01
#define HCI_SUPP_COMMANDS_READ_INQUIRY_RESPONSE_TX_POWER_OFF    18
#define HCI_READ_INQUIRY_RESPONSE_TX_POWER_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_INQUIRY_RESPONSE_TX_POWER_OFF] & HCI_SUPP_COMMANDS_READ_INQUIRY_RESPONSE_TX_POWER_MASK)

#define HCI_SUPP_COMMANDS_WRITE_INQUIRY_RESPONSE_TX_POWER_MASK   0x02
#define HCI_SUPP_COMMANDS_WRITE_INQUIRY_RESPONSE_TX_POWER_OFF    18
#define HCI_WRITE_INQUIRY_RESPONSE_TX_POWER_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_INQUIRY_RESPONSE_TX_POWER_OFF] & HCI_SUPP_COMMANDS_WRITE_INQUIRY_RESPONSE_TX_POWER_MASK)

#define HCI_SUPP_COMMANDS_READ_DEFAULT_ERRONEOUS_DATA_REPORTING_MASK   0x04
#define HCI_SUPP_COMMANDS_READ_DEFAULT_ERRONEOUS_DATA_REPORTING_OFF    18
#define HCI_READ_DEFAULT_ERRONEOUS_DATA_REPORTING_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_DEFAULT_ERRONEOUS_DATA_REPORTING_OFF] & HCI_SUPP_COMMANDS_READ_DEFAULT_ERRONEOUS_DATA_REPORTING_MASK)

#define HCI_SUPP_COMMANDS_WRITE_DEFAULT_ERRONEOUS_DATA_REPORTING_MASK   0x08
#define HCI_SUPP_COMMANDS_WRITE_DEFAULT_ERRONEOUS_DATA_REPORTING_OFF    18
#define HCI_WRITE_DEFAULT_ERRONEOUS_DATA_REPORTING_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_DEFAULT_ERRONEOUS_DATA_REPORTING_OFF] & HCI_SUPP_COMMANDS_WRITE_DEFAULT_ERRONEOUS_DATA_REPORTING_MASK)

#define HCI_SUPP_COMMANDS_IO_CAPABILITY_RESPONSE_MASK   0x80
#define HCI_SUPP_COMMANDS_IO_CAPABILITY_RESPONSE_OFF    18
#define HCI_IO_CAPABILITY_RESPONSE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_IO_CAPABILITY_RESPONSE_OFF] & HCI_SUPP_COMMANDS_IO_CAPABILITY_RESPONSE_MASK)

#define HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_REPLY_MASK   0x01
#define HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_REPLY_OFF    19
#define HCI_USER_CONFIRMATION_REQUEST_REPLY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_REPLY_OFF] & HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_REPLY_MASK)

#define HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_NEG_REPLY_MASK   0x02
#define HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_NEG_REPLY_OFF    19
#define HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_NEG_REPLY_OFF] & HCI_SUPP_COMMANDS_USER_CONFIRMATION_REQUEST_NEG_REPLY_MASK)

#define HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_REPLY_MASK   0x04
#define HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_REPLY_OFF    19
#define HCI_USER_PASSKEY_REQUEST_REPLY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_REPLY_OFF] & HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_REPLY_MASK)

#define HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_NEG_REPLY_MASK   0x08
#define HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_NEG_REPLY_OFF    19
#define HCI_USER_PASSKEY_REQUEST_NEG_REPLY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_NEG_REPLY_OFF] & HCI_SUPP_COMMANDS_USER_PASSKEY_REQUEST_NEG_REPLY_MASK)

#define HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_REPLY_MASK   0x10
#define HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_REPLY_OFF    19
#define HCI_REMOTE_OOB_DATA_REQUEST_REPLY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_REPLY_OFF] & HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_REPLY_MASK)

#define HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_DBG_MODE_MASK       0x20
#define HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_DBG_MODE_OFF        19
#define HCI_WRITE_SIMPLE_PAIRING_DBG_MODE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_DBG_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_SIMPLE_PAIRING_DBG_MODE_MASK)

#define HCI_SUPP_COMMANDS_ENHANCED_FLUSH_MASK   0x40
#define HCI_SUPP_COMMANDS_ENHANCED_FLUSH_OFF    19
#define HCI_ENHANCED_FLUSH_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_ENHANCED_FLUSH_OFF] & HCI_SUPP_COMMANDS_ENHANCED_FLUSH_MASK)

#define HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_MASK       0x80
#define HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_OFF        19
#define HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_OFF] & HCI_SUPP_COMMANDS_REMOTE_OOB_DATA_REQUEST_NEG_REPLY_MASK)

/* Supported Commands (Byte 20) */
#define HCI_SUPP_COMMANDS_SEND_KEYPRESS_NOTIF_MASK       0x04
#define HCI_SUPP_COMMANDS_SEND_KEYPRESS_NOTIF_OFF        20
#define HCI_SEND_NOTIF_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_SEND_KEYPRESS_NOTIF_OFF] & HCI_SUPP_COMMANDS_SEND_KEYPRESS_NOTIF_MASK)

#define HCI_SUPP_COMMANDS_IO_CAP_REQ_NEG_REPLY_MASK      0x08
#define HCI_SUPP_COMMANDS_IO_CAP_REQ_NEG_REPLY_OFF       20
#define HCI_IO_CAP_REQ_NEG_REPLY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_IO_CAP_REQ_NEG_REPLY_OFF] & HCI_SUPP_COMMANDS_IO_CAP_REQ_NEG_REPLY_MASK)

#define HCI_SUPP_COMMANDS_READ_ENCR_KEY_SIZE_MASK      0x10
#define HCI_SUPP_COMMANDS_READ_ENCR_KEY_SIZE_OFF       20
#define HCI_READ_ENCR_KEY_SIZE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_ENCR_KEY_SIZE_OFF] & HCI_SUPP_COMMANDS_READ_ENCR_KEY_SIZE_MASK)

/* Supported Commands (Byte 21) */
#define HCI_SUPP_COMMANDS_CREATE_PHYSICAL_LINK_MASK   0x01
#define HCI_SUPP_COMMANDS_CREATE_PHYSICAL_LINK_OFF    21
#define HCI_CREATE_PHYSICAL_LINK_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_CREATE_PHYSICAL_LINK_OFF] & HCI_SUPP_COMMANDS_CREATE_PHYSICAL_LINK_MASK)

#define HCI_SUPP_COMMANDS_ACCEPT_PHYSICAL_LINK_MASK   0x02
#define HCI_SUPP_COMMANDS_ACCEPT_PHYSICAL_LINK_OFF    21
#define HCI_ACCEPT_PHYSICAL_LINK_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_ACCEPT_PHYSICAL_LINK_OFF] & HCI_SUPP_COMMANDS_ACCEPT_PHYSICAL_LINK_MASK)

#define HCI_SUPP_COMMANDS_DISCONNECT_PHYSICAL_LINK_MASK   0x04
#define HCI_SUPP_COMMANDS_DISCONNECT_PHYSICAL_LINK_OFF    21
#define HCI_DISCONNECT_PHYSICAL_LINK_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_DISCONNECT_PHYSICAL_LINK_OFF] & HCI_SUPP_COMMANDS_DISCONNECT_PHYSICAL_LINK_MASK)

#define HCI_SUPP_COMMANDS_CREATE_LOGICAL_LINK_MASK   0x08
#define HCI_SUPP_COMMANDS_CREATE_LOGICAL_LINK_OFF    21
#define HCI_CREATE_LOGICAL_LINK_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_CREATE_LOGICAL_LINK_OFF] & HCI_SUPP_COMMANDS_CREATE_LOGICAL_LINK_MASK)

#define HCI_SUPP_COMMANDS_ACCEPT_LOGICAL_LINK_MASK   0x10
#define HCI_SUPP_COMMANDS_ACCEPT_LOGICAL_LINK_OFF    21
#define HCI_ACCEPT_LOGICAL_LINK_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_ACCEPT_LOGICAL_LINK_OFF] & HCI_SUPP_COMMANDS_ACCEPT_LOGICAL_LINK_MASK)

#define HCI_SUPP_COMMANDS_DISCONNECT_LOGICAL_LINK_MASK   0x20
#define HCI_SUPP_COMMANDS_DISCONNECT_LOGICAL_LINK_OFF    21
#define HCI_DISCONNECT_LOGICAL_LINK_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_DISCONNECT_LOGICAL_LINK_OFF] & HCI_SUPP_COMMANDS_DISCONNECT_LOGICAL_LINK_MASK)

#define HCI_SUPP_COMMANDS_LOGICAL_LINK_CANCEL_MASK   0x40
#define HCI_SUPP_COMMANDS_LOGICAL_LINK_CANCEL_OFF    21
#define HCI_LOGICAL_LINK_CANCEL_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_LOGICAL_LINK_CANCEL_OFF] & HCI_SUPP_COMMANDS_LOGICAL_LINK_CANCEL_MASK)

#define HCI_SUPP_COMMANDS_FLOW_SPEC_MODIFY_MASK       0x80
#define HCI_SUPP_COMMANDS_FLOW_SPEC_MODIFY_OFF        21
#define HCI_FLOW_SPEC_MODIFY_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_FLOW_SPEC_MODIFY_OFF] & HCI_SUPP_COMMANDS_FLOW_SPEC_MODIFY_MASK)

/* Supported Commands (Byte 22) */
#define HCI_SUPP_COMMANDS_READ_LOGICAL_LINK_ACCEPT_TIMEOUT_MASK   0x01
#define HCI_SUPP_COMMANDS_READ_LOGICAL_LINK_ACCEPT_TIMEOUT_OFF    22
#define HCI_READ_LOGICAL_LINK_ACCEPT_TIMEOUT_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_LOGICAL_LINK_ACCEPT_TIMEOUT_OFF] & HCI_SUPP_COMMANDS_READ_LOGICAL_LINK_ACCEPT_TIMEOUT_MASK)

#define HCI_SUPP_COMMANDS_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT_MASK   0x02
#define HCI_SUPP_COMMANDS_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT_OFF    22
#define HCI_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT_OFF] & HCI_SUPP_COMMANDS_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT_MASK)

#define HCI_SUPP_COMMANDS_SET_EVENT_MASK_PAGE_2_MASK   0x04
#define HCI_SUPP_COMMANDS_SET_EVENT_MASK_PAGE_2_OFF    22
#define HCI_SET_EVENT_MASK_PAGE_2_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_SET_EVENT_MASK_PAGE_2_OFF] & HCI_SUPP_COMMANDS_SET_EVENT_MASK_PAGE_2_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCATION_DATA_MASK   0x08
#define HCI_SUPP_COMMANDS_READ_LOCATION_DATA_OFF    22
#define HCI_READ_LOCATION_DATA_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_LOCATION_DATA_OFF] & HCI_SUPP_COMMANDS_READ_LOCATION_DATA_MASK)

#define HCI_SUPP_COMMANDS_WRITE_LOCATION_DATA_MASK   0x10
#define HCI_SUPP_COMMANDS_WRITE_LOCATION_DATA_OFF    22
#define HCI_WRITE_LOCATION_DATA_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_LOCATION_DATA_OFF] & HCI_SUPP_COMMANDS_WRITE_LOCATION_DATA_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_AMP_INFO_MASK   0x20
#define HCI_SUPP_COMMANDS_READ_LOCAL_AMP_INFO_OFF    22
#define HCI_READ_LOCAL_AMP_INFO_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_AMP_INFO_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_AMP_INFO_MASK)

#define HCI_SUPP_COMMANDS_READ_LOCAL_AMP_ASSOC_MASK   0x40
#define HCI_SUPP_COMMANDS_READ_LOCAL_AMP_ASSOC_OFF    22
#define HCI_READ_LOCAL_AMP_ASSOC_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_LOCAL_AMP_ASSOC_OFF] & HCI_SUPP_COMMANDS_READ_LOCAL_AMP_ASSOC_MASK)

#define HCI_SUPP_COMMANDS_WRITE_REMOTE_AMP_ASSOC_MASK   0x80
#define HCI_SUPP_COMMANDS_WRITE_REMOTE_AMP_ASSOC_OFF    22
#define HCI_WRITE_REMOTE_AMP_ASSOC_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_REMOTE_AMP_ASSOC_OFF] & HCI_SUPP_COMMANDS_WRITE_REMOTE_AMP_ASSOC_MASK)

/* Supported Commands (Byte 23) */
#define HCI_SUPP_COMMANDS_READ_FLOW_CONTROL_MODE_MASK   0x01
#define HCI_SUPP_COMMANDS_READ_FLOW_CONTROL_MODE_OFF    23
#define HCI_READ_FLOW_CONTROL_MODE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_FLOW_CONTROL_MODE_OFF] & HCI_SUPP_COMMANDS_READ_FLOW_CONTROL_MODE_MASK)

#define HCI_SUPP_COMMANDS_WRITE_FLOW_CONTROL_MODE_MASK   0x02
#define HCI_SUPP_COMMANDS_WRITE_FLOW_CONTROL_MODE_OFF    23
#define HCI_WRITE_FLOW_CONTROL_MODE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_FLOW_CONTROL_MODE_OFF] & HCI_SUPP_COMMANDS_WRITE_FLOW_CONTROL_MODE_MASK)

#define HCI_SUPP_COMMANDS_READ_DATA_BLOCK_SIZE_MASK   0x04
#define HCI_SUPP_COMMANDS_READ_DATA_BLOCK_SIZE_OFF    23
#define HCI_READ_DATA_BLOCK_SIZE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_DATA_BLOCK_SIZE_OFF] & HCI_SUPP_COMMANDS_READ_DATA_BLOCK_SIZE_MASK)

#define HCI_SUPP_COMMANDS_ENABLE_AMP_RCVR_REPORTS_MASK   0x20
#define HCI_SUPP_COMMANDS_ENABLE_AMP_RCVR_REPORTS_OFF    23
#define HCI_ENABLE_AMP_RCVR_REPORTS_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_ENABLE_AMP_RCVR_REPORTS_OFF] & HCI_SUPP_COMMANDS_ENABLE_AMP_RCVR_REPORTS_MASK)

#define HCI_SUPP_COMMANDS_AMP_TEST_END_MASK   0x40
#define HCI_SUPP_COMMANDS_AMP_TEST_END_OFF    23
#define HCI_AMP_TEST_END_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_AMP_TEST_END_OFF] & HCI_SUPP_COMMANDS_AMP_TEST_END_MASK)

#define HCI_SUPP_COMMANDS_AMP_TEST_MASK   0x80
#define HCI_SUPP_COMMANDS_AMP_TEST_OFF    23
#define HCI_AMP_TEST_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_AMP_TEST_OFF] & HCI_SUPP_COMMANDS_AMP_TEST_MASK)

/* Supported Commands (Byte 24) */
#define HCI_SUPP_COMMANDS_READ_TRANSMIT_POWER_LEVEL_MASK   0x01
#define HCI_SUPP_COMMANDS_READ_TRANSMIT_POWER_LEVEL_OFF    24
#define HCI_READ_TRANSMIT_POWER_LEVEL_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_TRANSMIT_POWER_LEVEL_OFF] & HCI_SUPP_COMMANDS_READ_TRANSMIT_POWER_LEVEL_MASK)

#define HCI_SUPP_COMMANDS_READ_BE_FLUSH_TOUT_MASK   0x04
#define HCI_SUPP_COMMANDS_READ_BE_FLUSH_TOUT_OFF    24
#define HCI_READ_BE_FLUSH_TOUT_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_READ_BE_FLUSH_TOUT_OFF] & HCI_SUPP_COMMANDS_READ_BE_FLUSH_TOUT_MASK)

#define HCI_SUPP_COMMANDS_WRITE_BE_FLUSH_TOUT_MASK   0x08
#define HCI_SUPP_COMMANDS_WRITE_BE_FLUSH_TOUT_OFF    24
#define HCI_WRITE_BE_FLUSH_TOUT_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_WRITE_BE_FLUSH_TOUT_OFF] & HCI_SUPP_COMMANDS_WRITE_BE_FLUSH_TOUT_MASK)

#define HCI_SUPP_COMMANDS_SHORT_RANGE_MODE_MASK   0x10
#define HCI_SUPP_COMMANDS_SHORT_RANGE_MODE_OFF    24
#define HCI_SHORT_RANGE_MODE_SUPPORTED(x)   ((x)[HCI_SUPP_COMMANDS_SHORT_RANGE_MODE_OFF] & HCI_SUPP_COMMANDS_SHORT_RANGE_MODE_MASK)

/* LE commands TBD
** Supported Commands (Byte 24 continued)
** Supported Commands (Byte 25)
** Supported Commands (Byte 26)
** Supported Commands (Byte 27)
** Supported Commands (Byte 28)
*/

/*
Commands of HCI_GRP_VENDOR_SPECIFIC group for WIDCOMM SW LM Simulator
*/
#ifdef _WIDCOMM

#define HCI_SET_HCI_TRACE               (0x0001 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_SET_LM_TRACE                (0x0002 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_WRITE_COUNTRY_CODE          (0x0004 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_READ_LM_HISTORY             (0x0005 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_WRITE_BD_ADDR               (0x0006 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_DISABLE_ENCRYPTION          (0x0007 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_DISABLE_AUTHENTICATION      (0x0008 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_GENERIC_LC_CMD              (0x000A | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_INCR_POWER                  (0x000B | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_DECR_POWER                  (0x000C | HCI_GRP_VENDOR_SPECIFIC)

/* Definitions for the local transactions */
#define LM_DISCONNECT                  (0x00D0 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_AUTHENTICATION_REQUESTED    (0x00D1 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_SET_CONN_ENCRYPTION         (0x00D2 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_START_ENCRYPT_KEY_SIZE      (0x00D3 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_START_ENCRYPTION            (0x00D4 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_STOP_ENCRYPTION             (0x00D5 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_CHANGE_CONN_PACKET_TYPE     (0x00D6 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_RMT_NAME_REQUEST            (0x00D7 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_READ_RMT_FEATURES           (0x00D8 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_READ_RMT_VERSION_INFO       (0x00D9 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_READ_RMT_TIMING_INFO        (0x00DA | HCI_GRP_VENDOR_SPECIFIC)
#define LM_READ_RMT_CLOCK_OFFSET       (0x00DB | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HOLD_MODE                   (0x00DC | HCI_GRP_VENDOR_SPECIFIC)
#define LM_EXIT_PARK_MODE              (0x00DD | HCI_GRP_VENDOR_SPECIFIC)

#define LM_SCO_LINK_REQUEST            (0x00E0 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_SCO_CHANGE                  (0x00E4 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_SCO_REMOVE                  (0x00E8 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_MAX_SLOTS                   (0x00F1 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_MAX_SLOTS_REQUEST           (0x00F2 | HCI_GRP_VENDOR_SPECIFIC)

#ifdef INCLUDE_OPTIONAL_PAGING_SCHEME
#define LM_OPTIONAL_PAGE_REQUEST       (0x00F3 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_OPTIONAL_PAGESCAN_REQUEST   (0x00F4 | HCI_GRP_VENDOR_SPECIFIC)
#endif

#define LM_SETUP_COMPLETE              (0x00FF | HCI_GRP_VENDOR_SPECIFIC)

#define LM_HIST_SEND_LMP_FRAME         (0x0100 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_RECV_LMP_FRAME         (0x0101 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_HCIT_ERROR             (0x0102 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_PER_INQ_TOUT           (0x0103 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_INQ_SCAN_TOUT          (0x0104 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_PAGE_SCAN_TOUT         (0x0105 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_RESET_TOUT             (0x0106 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_MANDAT_PSCAN_TOUT      (0x0107 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_ACL_START_TRANS        (0x0108 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_ACL_HOST_REPLY         (0x0109 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_ACL_TIMEOUT            (0x010A | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_ACL_TX_COMP            (0x010B | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_ACL_HCID_SUSPENDED     (0x010C | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_ACL_FAILED             (0x010D | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_HCI_COMMAND            (0x010E | HCI_GRP_VENDOR_SPECIFIC)

#define LM_HIST_HCI_EVENT              (0x010F | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_HCI_UPDATA             (0x0110 | HCI_GRP_VENDOR_SPECIFIC)
#define LM_HIST_HCI_DNDATA             (0x0111 | HCI_GRP_VENDOR_SPECIFIC)

#define HCI_ENTER_TEST_MODE            (0x0300 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_LMP_TEST_CNTRL             (0x0301 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_DEBUG_LC_CMD_MIN           (0x0300 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_DEBUG_LC_CMD_MAX           (0x03FF | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_DEBUG_LC_COMMAND           HCI_DEBUG_LC_CMD_MAX

#endif


/* AMP VSE events
*/
#define AMP_VSE_CHANSPEC_CHAN_MASK      0x00ff

#define AMP_VSE_CHANSPEC_CTL_SB_MASK    0x0300
#define AMP_VSE_CHANSPEC_CTL_SB_LOWER   0x0100
#define AMP_VSE_CHANSPEC_CTL_SB_UPPER   0x0200
#define AMP_VSE_CHANSPEC_CTL_SB_NONE    0x0300

#define AMP_VSE_CHANSPEC_BW_MASK        0x0C00
#define AMP_VSE_CHANSPEC_BW_10          0x0400
#define AMP_VSE_CHANSPEC_BW_20          0x0800
#define AMP_VSE_CHANSPEC_BW_40          0x0C00

#define AMP_VSE_CHANSPEC_BAND_MASK      0xf000
#define AMP_VSE_CHANSPEC_BAND_5G        0x1000
#define AMP_VSE_CHANSPEC_BAND_2G        0x2000


#endif
/* end "revolution/BTE/stack/include/hcidefs.h" */

/*****************************************************************************
**  DEVICE CONTROL and COMMON
*****************************************************************************/
/*****************************
**  Device Control Constants
******************************/
/* Maximum number of bytes allowed for vendor specific command parameters */
#define BTM_MAX_VENDOR_SPECIFIC_LEN  HCI_COMMAND_SIZE

/* BTM application return status codes */
enum
{
    BTM_SUCCESS = 0,                    /* 0  Command succeeded                 */
    BTM_CMD_STARTED,                    /* 1  Command started OK.               */
    BTM_BUSY,                           /* 2  Device busy with another command  */
    BTM_NO_RESOURCES,                   /* 3  No resources to issue command     */
    BTM_MODE_UNSUPPORTED,               /* 4  Request for 1 or more unsupported modes */
    BTM_ILLEGAL_VALUE,                  /* 5  Illegal parameter value           */
    BTM_WRONG_MODE,                     /* 6  Device in wrong mode for request  */
    BTM_UNKNOWN_ADDR,                   /* 7  Unknown remote BD address         */
    BTM_DEVICE_TIMEOUT,                 /* 8  Device timeout                    */
    BTM_BAD_VALUE_RET,                  /* 9  A bad value was received from HCI */
    BTM_ERR_PROCESSING,                 /* 10 Generic error                     */
    BTM_NOT_AUTHORIZED,                 /* 11 Authorization failed              */
    BTM_DEV_RESET,                      /* 12 Device has been reset             */
    BTM_CMD_STORED,                     /* 13 request is stored in control block */
    BTM_ILLEGAL_ACTION,                 /* 14 state machine gets illegal command */
    BTM_DELAY_CHECK,                    /* 15 delay the check on encryption */
    BTM_SCO_BAD_LENGTH,                 /* 16 Bad SCO over HCI data length */
    BTM_SUCCESS_NO_SECURITY,            /* 17 security passed, no security set  */
    BTM_FAILED_ON_SECURITY ,             /* 18 security failed                   */
    BTM_REPEATED_ATTEMPTS               /* 19 repeated attempts for LE security requests */
};
typedef UINT8 tBTM_STATUS;

/*************************
**  Device Control Types
**************************/
#define BTM_DEVICE_ROLE_BR      0x01
#define BTM_DEVICE_ROLE_DUAL    0x02
#define BTM_MAX_DEVICE_ROLE     BTM_DEVICE_ROLE_DUAL
typedef UINT8 tBTM_DEVICE_ROLE;

/* Device name of peer (may be truncated to save space in BTM database) */
typedef UINT8 tBTM_BD_NAME[BTM_MAX_REM_BD_NAME_LEN + 1];

/* Structure returned with local version information */
typedef struct
{
    UINT8       hci_version;
    UINT16      hci_revision;
    UINT8       lmp_version;
    UINT16      manufacturer;
    UINT16      lmp_subversion;
} tBTM_VERSION_INFO;

/* Structure returned with Vendor Specific Command complete callback */
typedef struct
{
    UINT16  opcode;
    UINT16  param_len;
    UINT8   *p_param_buf;
} tBTM_VSC_CMPL;

#define  BTM_VSC_CMPL_DATA_SIZE  (BTM_MAX_VENDOR_SPECIFIC_LEN + sizeof(tBTM_VSC_CMPL))
/**************************************************
**  Device Control and General Callback Functions
***************************************************/
/* Callback function for when device status changes. Appl must poll for
** what the new state is (BTM_IsDeviceUp). The event occurs whenever the stack
** has detected that the controller status has changed. This asynchronous event
** is enabled/disabled by calling BTM_RegisterForDeviceStatusNotif().
*/
enum
{
    BTM_DEV_STATUS_UP,
    BTM_DEV_STATUS_DOWN,
    BTM_DEV_STATUS_CMD_TOUT
};

typedef UINT8 tBTM_DEV_STATUS;


typedef void (tBTM_DEV_STATUS_CB) (tBTM_DEV_STATUS status);


/* Callback function for when a vendor specific event occurs. The length and
** array of returned parameter bytes are included. This asynchronous event
** is enabled/disabled by calling BTM_RegisterForVSEvents().
*/
typedef void (tBTM_VS_EVT_CB) (UINT8 len, UINT8 *p);


/* General callback function for notifying an application that a synchronous
** BTM function is complete. The pointer contains the address of any returned data.
*/
typedef void (tBTM_CMPL_CB) (void *p1);

/* VSC callback function for notifying an application that a synchronous
** BTM function is complete. The pointer contains the address of any returned data.
*/
typedef void (tBTM_VSC_CMPL_CB) (tBTM_VSC_CMPL *p1);

/* Callback for apps to check connection and inquiry filters.
** Parameters are the BD Address of remote and the Dev Class of remote.
** If the app returns none zero, the connection or inquiry result will be dropped.
*/
typedef UINT8 (tBTM_FILTER_CB) (BD_ADDR bd_addr, DEV_CLASS dc);

/*****************************************************************************
**  DEVICE DISCOVERY - Inquiry, Remote Name, Discovery, Class of Device
*****************************************************************************/
/*******************************
**  Device Discovery Constants
********************************/
/* Discoverable modes */
#define BTM_NON_DISCOVERABLE        0
#define BTM_LIMITED_DISCOVERABLE    1
#define BTM_GENERAL_DISCOVERABLE    2
#define BTM_DISCOVERABLE_MASK       (BTM_LIMITED_DISCOVERABLE|BTM_GENERAL_DISCOVERABLE)
#define BTM_MAX_DISCOVERABLE        BTM_GENERAL_DISCOVERABLE
/* high byte for BLE Discoverable modes */
#define BTM_BLE_NON_DISCOVERABLE        0x0000
#define BTM_BLE_LIMITED_DISCOVERABLE    0x0100
#define BTM_BLE_GENERAL_DISCOVERABLE    0x0200
#define BTM_BLE_MAX_DISCOVERABLE        BTM_BLE_GENERAL_DISCOVERABLE
#define BTM_BLE_DISCOVERABLE_MASK       (BTM_BLE_NON_DISCOVERABLE|BTM_BLE_LIMITED_DISCOVERABLE|BTM_BLE_GENERAL_DISCOVERABLE)

/* Connectable modes */
#define BTM_NON_CONNECTABLE         0
#define BTM_CONNECTABLE             1
#define BTM_CONNECTABLE_MASK        (BTM_NON_CONNECTABLE | BTM_CONNECTABLE)
/* high byte for BLE Connectable modes */
#define BTM_BLE_NON_CONNECTABLE      0x0000
#define BTM_BLE_CONNECTABLE          0x0100
#define BTM_BLE_MAX_CONNECTABLE      BTM_BLE_CONNECTABLE
#define BTM_BLE_CONNECTABLE_MASK    (BTM_BLE_NON_CONNECTABLE | BTM_BLE_CONNECTABLE)

/* Inquiry modes
 * Note: These modes are associated with the inquiry active values (BTM_*ACTIVE) */
#define BTM_GENERAL_INQUIRY         0
#define BTM_LIMITED_INQUIRY         1
#define BTM_BR_INQUIRY_MASK         0x0f
/* high byte of inquiry mode for BLE inquiry mode */
#define BTM_BLE_INQUIRY_NONE         0x00
#define BTM_BLE_GENERAL_INQUIRY      0x10
#define BTM_BLE_LIMITED_INQUIRY      0x20
#define BTM_BLE_INQUIRY_MASK         (BTM_BLE_GENERAL_INQUIRY|BTM_BLE_LIMITED_INQUIRY)

/* BTM_IsInquiryActive return values (Bit Mask)
 * Note: These bit masks are associated with the inquiry modes (BTM_*_INQUIRY) */
#define BTM_INQUIRY_INACTIVE        0x0     /* no inquiry in progress */
#define BTM_GENERAL_INQUIRY_ACTIVE  0x1     /* a general inquiry is in progress */
#define BTM_LIMITED_INQUIRY_ACTIVE  0x2     /* a limited inquiry is in progress */
#define BTM_PERIODIC_INQUIRY_ACTIVE 0x8     /* a periodic inquiry is active */
#define BTM_SSP_INQUIRY_ACTIVE      0x4     /* SSP is active, so inquiry is disallowed (work around for FW bug) */

/* Define scan types */
#define BTM_SCAN_TYPE_STANDARD      0
#define BTM_SCAN_TYPE_INTERLACED    1       /* 1.2 devices only */

/* Define inquiry results mode */
#define BTM_INQ_RESULT_STANDARD     0
#define BTM_INQ_RESULT_WITH_RSSI    1
#define BTM_INQ_RESULT_EXTENDED     2

#define BTM_INQ_RES_IGNORE_RSSI     0x7f    /* RSSI value not supplied (ignore it) */

/* Inquiry Filter Condition types (see tBTM_INQ_PARMS) */
#define BTM_CLR_INQUIRY_FILTER          0                   /* Inquiry Filtering is turned off */
#define BTM_FILTER_COND_DEVICE_CLASS    HCI_FILTER_COND_DEVICE_CLASS /* Filter on device class */
#define BTM_FILTER_COND_BD_ADDR         HCI_FILTER_COND_BD_ADDR /* Filter on device addr */

/* State of the remote name retrieval during inquiry operations.
** Used in the tBTM_INQ_INFO structure, and returned in the
** BTM_InqDbRead, BTM_InqDbFirst, and BTM_InqDbNext functions.
** The name field is valid when the state returned is
** BTM_INQ_RMT_NAME_DONE */
#define BTM_INQ_RMT_NAME_EMPTY      0
#define BTM_INQ_RMT_NAME_PENDING    1
#define BTM_INQ_RMT_NAME_DONE       2
#define BTM_INQ_RMT_NAME_FAILED     3

/*********************************
 *** Class of Device constants ***
 *********************************/
#define BTM_FORMAT_TYPE_1   0x00

/****************************
** minor device class field
*****************************/

/* 0x00 is used as unclassified for all minor device classes */
#define BTM_COD_MINOR_UNCLASSIFIED          0x00

/* minor device class field for Computer Major Class */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_DESKTOP_WORKSTATION   0x04
#define BTM_COD_MINOR_SERVER_COMPUTER       0x08
#define BTM_COD_MINOR_LAPTOP                0x0C
#define BTM_COD_MINOR_HANDHELD_PC_PDA       0x10    /* clam shell */
#define BTM_COD_MINOR_PALM_SIZE_PC_PDA      0x14
#define BTM_COD_MINOR_WEARABLE_COMPUTER     0x18    /* watch sized */

/* minor device class field for Phone Major Class */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_CELLULAR              0x04
#define BTM_COD_MINOR_CORDLESS              0x08
#define BTM_COD_MINOR_SMART_PHONE           0x0C
#define BTM_COD_MINOR_WIRED_MDM_V_GTWY      0x10 /* wired modem or voice gatway */
#define BTM_COD_MINOR_ISDN_ACCESS           0x14

/* minor device class field for LAN Access Point Major Class */
/* Load Factor Field bit 5-7 */
#define BTM_COD_MINOR_FULLY_AVAILABLE       0x00
#define BTM_COD_MINOR_1_17_UTILIZED         0x20
#define BTM_COD_MINOR_17_33_UTILIZED        0x40
#define BTM_COD_MINOR_33_50_UTILIZED        0x60
#define BTM_COD_MINOR_50_67_UTILIZED        0x80
#define BTM_COD_MINOR_67_83_UTILIZED        0xA0
#define BTM_COD_MINOR_83_99_UTILIZED        0xC0
#define BTM_COD_MINOR_NO_SERVICE_AVAILABLE  0xE0
/* sub-Field bit 2-4 */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */

/* minor device class field for Audio/Video Major Class */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_CONFM_HEADSET         0x04
#define BTM_COD_MINOR_CONFM_HANDSFREE       0x08
#define BTM_COD_MINOR_MICROPHONE            0x10
#define BTM_COD_MINOR_LOUDSPEAKER           0x14
#define BTM_COD_MINOR_HEADPHONES            0x18
#define BTM_COD_MINOR_PORTABLE_AUDIO        0x1C
#define BTM_COD_MINOR_CAR_AUDIO             0x20
#define BTM_COD_MINOR_SET_TOP_BOX           0x24
#define BTM_COD_MINOR_HIFI_AUDIO            0x28
#define BTM_COD_MINOR_VCR                   0x2C
#define BTM_COD_MINOR_VIDEO_CAMERA          0x30
#define BTM_COD_MINOR_CAMCORDER             0x34
#define BTM_COD_MINOR_VIDEO_MONITOR         0x38
#define BTM_COD_MINOR_VIDDISP_LDSPKR        0x3C
#define BTM_COD_MINOR_VIDEO_CONFERENCING    0x40
#define BTM_COD_MINOR_GAMING_TOY            0x48

/* minor device class field for Peripheral Major Class */
/* Bits 6-7 independently specify mouse, keyboard, or combo mouse/keyboard */
#define BTM_COD_MINOR_KEYBOARD              0x40
#define BTM_COD_MINOR_POINTING              0x80
#define BTM_COD_MINOR_COMBO                 0xC0
/* Bits 2-5 OR'd with selection from bits 6-7 */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */
#define BTM_COD_MINOR_JOYSTICK              0x04
#define BTM_COD_MINOR_GAMEPAD               0x08
#define BTM_COD_MINOR_REMOTE_CONTROL        0x0C
#define BTM_COD_MINOR_SENSING_DEVICE        0x10
#define BTM_COD_MINOR_DIGITIZING_TABLET     0x14
#define BTM_COD_MINOR_CARD_READER           0x18    /* e.g. SIM card reader */
#define BTM_COD_MINOR_DIGITAL_PAN           0x1C
#define BTM_COD_MINOR_HAND_SCANNER          0x20
#define BTM_COD_MINOR_HAND_GESTURAL_INPUT   0x24

/* minor device class field for Imaging Major Class */
/* Bits 5-7 independently specify display, camera, scanner, or printer */
#define BTM_COD_MINOR_DISPLAY               0x10
#define BTM_COD_MINOR_CAMERA                0x20
#define BTM_COD_MINOR_SCANNER               0x40
#define BTM_COD_MINOR_PRINTER               0x80
/* Bits 2-3 Reserved */
/* #define BTM_COD_MINOR_UNCLASSIFIED       0x00    */

/* minor device class field for Wearable Major Class */
/* Bits 2-7 meaningful    */
#define BTM_COD_MINOR_WRIST_WATCH           0x04
#define BTM_COD_MINOR_PAGER                 0x08
#define BTM_COD_MINOR_JACKET                0x0C
#define BTM_COD_MINOR_HELMET                0x10
#define BTM_COD_MINOR_GLASSES               0x14

/* minor device class field for Toy Major Class */
/* Bits 2-7 meaningful    */
#define BTM_COD_MINOR_ROBOT                 0x04
#define BTM_COD_MINOR_VEHICLE               0x08
#define BTM_COD_MINOR_DOLL_ACTION_FIGURE    0x0C
#define BTM_COD_MINOR_CONTROLLER            0x10
#define BTM_COD_MINOR_GAME                  0x14

/* minor device class field for Health Major Class */
/* Bits 2-7 meaningful    */
#define BTM_COD_MINOR_BLOOD_MONITOR         0x04
#define BTM_COD_MINOR_THERMOMETER           0x08
#define BTM_COD_MINOR_WEIGHING_SCALE        0x0C
#define BTM_COD_MINOR_GLUCOSE_METER         0x10
#define BTM_COD_MINOR_PULSE_OXIMETER        0x14
#define BTM_COD_MINOR_HEART_PULSE_MONITOR   0x18
#define BTM_COD_MINOR_HEALTH_DATA_DISPLAY   0x1C
#define BTM_COD_MINOR_STEP_COUNTER          0x20
#define BTM_COD_MINOR_BODY_COM_ANALYZER     0x24
#define BTM_COD_MINOR_PEAK_FLOW_MONITOR     0x28
#define BTM_COD_MINOR_MEDICATION_MONITOR    0x2C
#define BTM_COD_MINOR_KNEE_PROSTHESIS       0x30
#define BTM_COD_MINOR_ANKLE_PROSTHESIS      0x34


/***************************
** major device class field
****************************/
#define BTM_COD_MAJOR_MISCELLANEOUS         0x00
#define BTM_COD_MAJOR_COMPUTER              0x01
#define BTM_COD_MAJOR_PHONE                 0x02
#define BTM_COD_MAJOR_LAN_ACCESS_PT         0x03
#define BTM_COD_MAJOR_AUDIO                 0x04
#define BTM_COD_MAJOR_PERIPHERAL            0x05
#define BTM_COD_MAJOR_IMAGING               0x06
#define BTM_COD_MAJOR_WEARABLE              0x07
#define BTM_COD_MAJOR_TOY                   0x08
#define BTM_COD_MAJOR_HEALTH                0x09
#define BTM_COD_MAJOR_UNCLASSIFIED          0x1F

/***************************
** service class fields
****************************/
#define BTM_COD_SERVICE_LMTD_DISCOVER       0x0020
#define BTM_COD_SERVICE_POSITIONING         0x0100
#define BTM_COD_SERVICE_NETWORKING          0x0200
#define BTM_COD_SERVICE_RENDERING           0x0400
#define BTM_COD_SERVICE_CAPTURING           0x0800
#define BTM_COD_SERVICE_OBJ_TRANSFER        0x1000
#define BTM_COD_SERVICE_AUDIO               0x2000
#define BTM_COD_SERVICE_TELEPHONY           0x4000
#define BTM_COD_SERVICE_INFORMATION         0x8000

/* class of device field macros */
#define BTM_COD_FORMAT_TYPE(u8, pd)         {u8  = pd[2]&0x03;}
#define BTM_COD_MINOR_CLASS(u8, pd)         {u8  = pd[2]&0xFC;}
#define BTM_COD_MAJOR_CLASS(u8, pd)         {u8  = pd[1]&0x1F;}
#define BTM_COD_SERVICE_CLASS(u16, pd)      {u16 = pd[0]; u16<<=8; u16 += pd[1]&0xE0;}

/* to set the fields (assumes that format type is always 0) */
#define FIELDS_TO_COD(pd, mn, mj, sv) {pd[2] = mn; pd[1] =              \
                                       mj+ ((sv)&BTM_COD_SERVICE_CLASS_LO_B); \
                                       pd[0] = (sv) >> 8;}

/* the COD masks */
#define BTM_COD_FORMAT_TYPE_MASK      0x03
#define BTM_COD_MINOR_CLASS_MASK      0xFC
#define BTM_COD_MAJOR_CLASS_MASK      0x1F
#define BTM_COD_SERVICE_CLASS_LO_B    0x00E0
#define BTM_COD_SERVICE_CLASS_MASK    0xFFE0


/* BTM service definitions
** Used for storing EIR data to bit mask
*/
#ifndef BTM_EIR_UUID_LKUP_TBL
enum
{
    BTM_EIR_UUID_SERVCLASS_SERVICE_DISCOVERY_SERVER,
/*    BTM_EIR_UUID_SERVCLASS_BROWSE_GROUP_DESCRIPTOR,   */
/*    BTM_EIR_UUID_SERVCLASS_PUBLIC_BROWSE_GROUP,       */
    BTM_EIR_UUID_SERVCLASS_SERIAL_PORT,
    BTM_EIR_UUID_SERVCLASS_LAN_ACCESS_USING_PPP,
    BTM_EIR_UUID_SERVCLASS_DIALUP_NETWORKING,
    BTM_EIR_UUID_SERVCLASS_IRMC_SYNC,
    BTM_EIR_UUID_SERVCLASS_OBEX_OBJECT_PUSH,
    BTM_EIR_UUID_SERVCLASS_OBEX_FILE_TRANSFER,
    BTM_EIR_UUID_SERVCLASS_IRMC_SYNC_COMMAND,
    BTM_EIR_UUID_SERVCLASS_HEADSET,
    BTM_EIR_UUID_SERVCLASS_CORDLESS_TELEPHONY,
    BTM_EIR_UUID_SERVCLASS_AUDIO_SOURCE,
    BTM_EIR_UUID_SERVCLASS_AUDIO_SINK,
    BTM_EIR_UUID_SERVCLASS_AV_REM_CTRL_TARGET,
/*    BTM_EIR_UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION,    */
    BTM_EIR_UUID_SERVCLASS_AV_REMOTE_CONTROL,
/*    BTM_EIR_UUID_SERVCLASS_VIDEO_CONFERENCING,        */
    BTM_EIR_UUID_SERVCLASS_INTERCOM,
    BTM_EIR_UUID_SERVCLASS_FAX,
    BTM_EIR_UUID_SERVCLASS_HEADSET_AUDIO_GATEWAY,
/*    BTM_EIR_UUID_SERVCLASS_WAP,                       */
/*    BTM_EIR_UUID_SERVCLASS_WAP_CLIENT,                */
    BTM_EIR_UUID_SERVCLASS_PANU,
    BTM_EIR_UUID_SERVCLASS_NAP,
    BTM_EIR_UUID_SERVCLASS_GN,
    BTM_EIR_UUID_SERVCLASS_DIRECT_PRINTING,
/*    BTM_EIR_UUID_SERVCLASS_REFERENCE_PRINTING,        */
    BTM_EIR_UUID_SERVCLASS_IMAGING,
    BTM_EIR_UUID_SERVCLASS_IMAGING_RESPONDER,
    BTM_EIR_UUID_SERVCLASS_IMAGING_AUTO_ARCHIVE,
    BTM_EIR_UUID_SERVCLASS_IMAGING_REF_OBJECTS,
    BTM_EIR_UUID_SERVCLASS_HF_HANDSFREE,
    BTM_EIR_UUID_SERVCLASS_AG_HANDSFREE,
    BTM_EIR_UUID_SERVCLASS_DIR_PRT_REF_OBJ_SERVICE,
/*    BTM_EIR_UUID_SERVCLASS_REFLECTED_UI,              */
    BTM_EIR_UUID_SERVCLASS_BASIC_PRINTING,
    BTM_EIR_UUID_SERVCLASS_PRINTING_STATUS,
    BTM_EIR_UUID_SERVCLASS_HUMAN_INTERFACE,
    BTM_EIR_UUID_SERVCLASS_CABLE_REPLACEMENT,
    BTM_EIR_UUID_SERVCLASS_HCRP_PRINT,
    BTM_EIR_UUID_SERVCLASS_HCRP_SCAN,
/*    BTM_EIR_UUID_SERVCLASS_COMMON_ISDN_ACCESS,        */
/*    BTM_EIR_UUID_SERVCLASS_VIDEO_CONFERENCING_GW,     */
/*    BTM_EIR_UUID_SERVCLASS_UDI_MT,                    */
/*    BTM_EIR_UUID_SERVCLASS_UDI_TA,                    */
/*    BTM_EIR_UUID_SERVCLASS_VCP,                       */
    BTM_EIR_UUID_SERVCLASS_SAP,
    BTM_EIR_UUID_SERVCLASS_PBAP_PCE,
    BTM_EIR_UUID_SERVCLASS_PBAP_PSE,
/*    BTM_EIR_UUID_SERVCLASS_TE_PHONE_ACCESS,           */
/*    BTM_EIR_UUID_SERVCLASS_ME_PHONE_ACCESS,           */
    BTM_EIR_UUID_SERVCLASS_PHONE_ACCESS,
    BTM_EIR_UUID_SERVCLASS_HEADSET_HS,
    BTM_EIR_UUID_SERVCLASS_PNP_INFORMATION,
/*    BTM_EIR_UUID_SERVCLASS_GENERIC_NETWORKING,        */
/*    BTM_EIR_UUID_SERVCLASS_GENERIC_FILETRANSFER,      */
/*    BTM_EIR_UUID_SERVCLASS_GENERIC_AUDIO,             */
/*    BTM_EIR_UUID_SERVCLASS_GENERIC_TELEPHONY,         */
/*    BTM_EIR_UUID_SERVCLASS_UPNP_SERVICE,              */
/*    BTM_EIR_UUID_SERVCLASS_UPNP_IP_SERVICE,           */
/*    BTM_EIR_UUID_SERVCLASS_ESDP_UPNP_IP_PAN,          */
/*    BTM_EIR_UUID_SERVCLASS_ESDP_UPNP_IP_LAP,          */
/*    BTM_EIR_UUID_SERVCLASS_ESDP_UPNP_IP_L2CAP,        */
    BTM_EIR_UUID_SERVCLASS_VIDEO_SOURCE,
    BTM_EIR_UUID_SERVCLASS_VIDEO_SINK,
/*    BTM_EIR_UUID_SERVCLASS_VIDEO_DISTRIBUTION         */
/*    BTM_EIR_UUID_SERVCLASS_HDP_PROFILE                */
    BTM_EIR_UUID_SERVCLASS_MESSAGE_ACCESS,
    BTM_EIR_UUID_SERVCLASS_MESSAGE_NOTIFICATION,
    BTM_EIR_UUID_SERVCLASS_HDP_SOURCE,
    BTM_EIR_UUID_SERVCLASS_HDP_SINK,
    BTM_EIR_MAX_SERVICES
};
#endif /* BTM_EIR_UUID_LKUP_TBL */

/* search result in EIR of inquiry database */
#define BTM_EIR_FOUND           0
#define BTM_EIR_NOT_FOUND       1
#define BTM_EIR_UNKNOWN         2

typedef UINT8 tBTM_EIR_SEARCH_RESULT;

#define BTM_EIR_FLAGS_TYPE                  HCI_EIR_FLAGS_TYPE                  /* 0x01 */
#define BTM_EIR_MORE_16BITS_UUID_TYPE       HCI_EIR_MORE_16BITS_UUID_TYPE       /* 0x02 */
#define BTM_EIR_COMPLETE_16BITS_UUID_TYPE   HCI_EIR_COMPLETE_16BITS_UUID_TYPE   /* 0x03 */
#define BTM_EIR_MORE_32BITS_UUID_TYPE       HCI_EIR_MORE_32BITS_UUID_TYPE       /* 0x04 */
#define BTM_EIR_COMPLETE_32BITS_UUID_TYPE   HCI_EIR_COMPLETE_32BITS_UUID_TYPE   /* 0x05 */
#define BTM_EIR_MORE_128BITS_UUID_TYPE      HCI_EIR_MORE_128BITS_UUID_TYPE      /* 0x06 */
#define BTM_EIR_COMPLETE_128BITS_UUID_TYPE  HCI_EIR_COMPLETE_128BITS_UUID_TYPE  /* 0x07 */
#define BTM_EIR_SHORTENED_LOCAL_NAME_TYPE   HCI_EIR_SHORTENED_LOCAL_NAME_TYPE   /* 0x08 */
#define BTM_EIR_COMPLETE_LOCAL_NAME_TYPE    HCI_EIR_COMPLETE_LOCAL_NAME_TYPE    /* 0x09 */
#define BTM_EIR_TX_POWER_LEVEL_TYPE         HCI_EIR_TX_POWER_LEVEL_TYPE         /* 0x0A */
#define BTM_EIR_MANUFACTURER_SPECIFIC_TYPE  HCI_EIR_MANUFACTURER_SPECIFIC_TYPE  /* 0xFF */

/* the following EIR tags are defined to OOB, not regular EIR data */
#define BTM_EIR_OOB_BD_ADDR_TYPE            HCI_EIR_OOB_BD_ADDR_TYPE    /* 6 bytes */
#define BTM_EIR_OOB_COD_TYPE                HCI_EIR_OOB_COD_TYPE        /* 3 bytes */
#define BTM_EIR_OOB_SSP_HASH_C_TYPE         HCI_EIR_OOB_SSP_HASH_C_TYPE /* 16 bytes */
#define BTM_EIR_OOB_SSP_RAND_R_TYPE         HCI_EIR_OOB_SSP_RAND_R_TYPE /* 16 bytes */

#define BTM_OOB_MANDATORY_SIZE      8   /* include 2 bytes length & 6 bytes bd_addr */
#define BTM_OOB_DATA_LEN_SIZE       2
#define BTM_OOB_BD_ADDR_SIZE        6
#define BTM_OOB_COD_SIZE            BT_OOB_COD_SIZE
#define BTM_OOB_HASH_C_SIZE         BT_OOB_HASH_C_SIZE
#define BTM_OOB_RAND_R_SIZE         BT_OOB_RAND_R_SIZE

/************************************************************************************************
** BTM Services MACROS handle array of UINT32 bits for more than 32 services
*************************************************************************************************/
/* Determine the number of UINT32's necessary for services */
#define BTM_EIR_ARRAY_BITS          32          /* Number of bits in each array element */
#define BTM_EIR_SERVICE_ARRAY_SIZE  (((UINT32)BTM_EIR_MAX_SERVICES / BTM_EIR_ARRAY_BITS) + \
                                    (((UINT32)BTM_EIR_MAX_SERVICES % BTM_EIR_ARRAY_BITS) ? 1 : 0))

/* MACRO to set the service bit mask in a bit stream */
#define BTM_EIR_SET_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_EIR_ARRAY_BITS)] |=  \
                                    ((UINT32)1 << (((UINT32)(service)) % BTM_EIR_ARRAY_BITS)))


/* MACRO to clear the service bit mask in a bit stream */
#define BTM_EIR_CLR_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_EIR_ARRAY_BITS)] &=  \
                                    ~((UINT32)1 << (((UINT32)(service)) % BTM_EIR_ARRAY_BITS)))

/* MACRO to check the service bit mask in a bit stream */
#define BTM_EIR_HAS_SERVICE(p, service)  ((((UINT32 *)(p))[(((UINT32)(service)) / BTM_EIR_ARRAY_BITS)] &  \
                                    ((UINT32)1 << (((UINT32)(service)) % BTM_EIR_ARRAY_BITS))) >> (((UINT32)(service)) % BTM_EIR_ARRAY_BITS))

/* start of EIR in HCI buffer, 4 bytes = HCI Command(2) + Length(1) + FEC_Req(1) */
#define BTM_HCI_EIR_OFFSET          (BT_HDR_SIZE + 4)

/***************************
**  Device Discovery Types
****************************/
/* Definitions of the parameters passed to BTM_StartInquiry and
** BTM_SetPeriodicInquiryMode.
*/
typedef struct              /* contains the two device class condition fields */
{
    DEV_CLASS       dev_class;
    DEV_CLASS       dev_class_mask;
} tBTM_COD_COND;


typedef union               /* contains the inquiry filter condition */
{
    BD_ADDR         bdaddr_cond;
    tBTM_COD_COND   cod_cond;
} tBTM_INQ_FILT_COND;


typedef struct              /* contains the parameters passed to the inquiry functions */
{
    UINT8   mode;                       /* general or limited */
    UINT8   duration;                   /* duration of the inquiry (1.28 sec increments) */
    UINT8   max_resps;                  /* maximum number of responses to return */
    BOOLEAN report_dup;                 /* report duplicated inquiry response with higher RSSI value */
    UINT8   filter_cond_type;           /* new devices, BD ADDR, COD, or No filtering */
    tBTM_INQ_FILT_COND  filter_cond;    /* filter value based on filter cond type */
} tBTM_INQ_PARMS;

#define BTM_INQ_RESULT_BR       0x01
#define BTM_INQ_RESULT_BLE      0x02

/* These are the fields returned in each device's response to the inquiry.  It
** is returned in the results callback if registered.
*/
typedef struct
{
    UINT16      clock_offset;
    BD_ADDR     remote_bd_addr;
    DEV_CLASS   dev_class;
    UINT8       page_scan_rep_mode;
    UINT8       page_scan_per_mode;
    UINT8       page_scan_mode;
    INT8        rssi;       /* Set to BTM_INQ_RES_IGNORE_RSSI if  not valid */
#if (BTM_EIR_CLIENT_INCLUDED == TRUE)
    UINT32      eir_uuid[BTM_EIR_SERVICE_ARRAY_SIZE];
    BOOLEAN     eir_complete_list;
#endif
} tBTM_INQ_RESULTS;


/* This is the inquiry response information held in its database by BTM, and available
** to applications via BTM_InqDbRead, BTM_InqDbFirst, and BTM_InqDbNext.
*/
typedef struct
{
    tBTM_INQ_RESULTS    results;

    BOOLEAN             appl_knows_rem_name;    /* set by application if it knows the remote name of the peer device.
                                                   This is later used by application to determine if remote name request is
                                                   required to be done. Having the flag here avoid duplicate store of inquiry results */



} tBTM_INQ_INFO;


/* Structure returned with inquiry complete callback */
typedef struct
{
    tBTM_STATUS status;
    UINT8       num_resp;       /* Number of results from the current inquiry */
} tBTM_INQUIRY_CMPL;


/* Structure returned with remote name  request */
typedef struct
{
    UINT16      status;
    UINT16      length;
    BD_NAME     remote_bd_name;
} tBTM_REMOTE_DEV_NAME;

typedef struct
{
    UINT8   pcm_intf_rate;  /* PCM interface rate: 0: 128kbps, 1: 256 kbps;
                                2:512 bps; 3: 1024kbps; 4: 2048kbps */
    UINT8   frame_type;     /* frame type: 0: short; 1: long */
    UINT8   sync_mode;      /* sync mode: 0: slave; 1: master */
    UINT8   clock_mode;     /* clock mode: 0: slave; 1: master */

}tBTM_SCO_PCM_PARAM;

/****************************************
**  Device Discovery Callback Functions
*****************************************/
/* Callback function for asynchronous notifications when the BTM inquiry DB
** changes. First param is inquiry database, second is if added to or removed
** from the inquiry database.
*/
typedef void (tBTM_INQ_DB_CHANGE_CB) (void *p1, BOOLEAN is_new);

/* Callback function for notifications when the BTM gets inquiry response.
** First param is inquiry results database, second is pointer of EIR.
*/
typedef void (tBTM_INQ_RESULTS_CB) (tBTM_INQ_RESULTS *p_inq_results, UINT8 *p_eir);

/*****************************************************************************
**  ACL CHANNEL MANAGEMENT
*****************************************************************************/
/******************
**  ACL Constants
*******************/

/* ACL modes */
#define BTM_ACL_MODE_NORMAL     HCI_MODE_ACTIVE
#define BTM_ACL_MODE_HOLD       HCI_MODE_HOLD
#define BTM_ACL_MODE_SNIFF      HCI_MODE_SNIFF
#define BTM_ACL_MODE_PARK       HCI_MODE_PARK

/* Returned with structure in role switch callback (tBTM_ROLE_SWITCH_CMPL) */
#define BTM_ROLE_MASTER         HCI_ROLE_MASTER
#define BTM_ROLE_SLAVE          HCI_ROLE_SLAVE
#define BTM_ROLE_UNDEFINED      0xff     /* undefined value (error status) */

/* ACL Packet Types */
#define BTM_ACL_PKT_TYPES_MASK_DM1      HCI_PKT_TYPES_MASK_DM1
#define BTM_ACL_PKT_TYPES_MASK_DH1      HCI_PKT_TYPES_MASK_DH1
#define BTM_ACL_PKT_TYPES_MASK_DM3      HCI_PKT_TYPES_MASK_DM3
#define BTM_ACL_PKT_TYPES_MASK_DH3      HCI_PKT_TYPES_MASK_DH3
#define BTM_ACL_PKT_TYPES_MASK_DM5      HCI_PKT_TYPES_MASK_DM5
#define BTM_ACL_PKT_TYPES_MASK_DH5      HCI_PKT_TYPES_MASK_DH5
#define BTM_ACL_PKT_TYPES_MASK_NO_2_DH1 HCI_PKT_TYPES_MASK_NO_2_DH1
#define BTM_ACL_PKT_TYPES_MASK_NO_3_DH1 HCI_PKT_TYPES_MASK_NO_3_DH1
#define BTM_ACL_PKT_TYPES_MASK_NO_2_DH3 HCI_PKT_TYPES_MASK_NO_2_DH3
#define BTM_ACL_PKT_TYPES_MASK_NO_3_DH3 HCI_PKT_TYPES_MASK_NO_3_DH3
#define BTM_ACL_PKT_TYPES_MASK_NO_2_DH5 HCI_PKT_TYPES_MASK_NO_2_DH5
#define BTM_ACL_PKT_TYPES_MASK_NO_3_DH5 HCI_PKT_TYPES_MASK_NO_3_DH5

/***************
**  ACL Types
****************/

/* Structure returned with link policy information (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadLinkPolicy call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    BD_ADDR     rem_bda;
    UINT16      settings;
} tBTM_LNK_POLICY_RESULTS;

/* Structure returned with Role Switch information (in tBTM_CMPL_CB callback function)
** in response to BTM_SwitchRole call.
*/
typedef struct
{
    UINT8   hci_status;     /* HCI status returned with the event */
    UINT8   role;           /* BTM_ROLE_MASTER or BTM_ROLE_SLAVE */
    BD_ADDR remote_bd_addr; /* Remote BD addr involved with the switch */
} tBTM_ROLE_SWITCH_CMPL;


/* Structure returned with Change Link Key information (in tBTM_CMPL_CB callback function)
** in response to BTM_ChangeLinkKey call.
*/
typedef struct
{
    UINT8   hci_status;     /* HCI status returned with the event */
    BD_ADDR remote_bd_addr; /* Remote BD addr involved with the change link key */
} tBTM_CHANGE_KEY_CMPL;


/* Structure returned with QoS information (in tBTM_CMPL_CB callback function)
** in response to BTM_SetQoS call.
*/
typedef struct
{
    FLOW_SPEC flow;
    UINT16 handle;
    UINT8 status;
} tBTM_QOS_SETUP_CMPL;


/* Structure returned with read RSSI event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadRSSI call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    INT8        rssi;
    BD_ADDR     rem_bda;
} tBTM_RSSI_RESULTS;

/* Structure returned with read current TX power event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadTxPower call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    INT8        tx_power;
    BD_ADDR     rem_bda;
} tBTM_TX_POWER_RESULTS;

/* Structure returned with read link quality event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadLinkQuality call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    UINT8       link_quality;
    BD_ADDR     rem_bda;
} tBTM_LINK_QUALITY_RESULTS;

/* Structure returned with read inq tx power quality event (in tBTM_CMPL_CB callback function)
** in response to BTM_ReadInquiryRspTxPower call.
*/
typedef struct
{
    tBTM_STATUS status;
    UINT8       hci_status;
    INT8        tx_power;
} tBTM_INQ_TXPWR_RESULTS;

enum
{
    BTM_BL_CONN_EVT,
    BTM_BL_DISCN_EVT,
    BTM_BL_UPDATE_EVT,
    BTM_BL_ROLE_CHG_EVT,
    BTM_BL_COLLISION_EVT
};
typedef UINT8 tBTM_BL_EVENT;
typedef UINT16 tBTM_BL_EVENT_MASK;

#define BTM_BL_CONN_MASK        0x0001
#define BTM_BL_DISCN_MASK       0x0002
#define BTM_BL_UPDATE_MASK      0x0004
#define BTM_BL_ROLE_CHG_MASK    0x0008

/* the data type associated with BTM_BL_CONN_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;      /* The event reported. */
    BD_ADDR_PTR     p_bda;      /* The address of the newly connected device */
    DEV_CLASS_PTR   p_dc;       /* The device class */
    BD_NAME_PTR     p_bdn;      /* The device name */
    UINT8          *p_features; /* The remote device's supported features */
} tBTM_BL_CONN_DATA;

/* the data type associated with BTM_BL_DISCN_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;  /* The event reported. */
    BD_ADDR_PTR     p_bda;  /* The address of the disconnected device */
} tBTM_BL_DISCN_DATA;

/* Busy-Level shall have the inquiry_paging mask set when
 * inquiry/paging is in progress, Else the number of ACL links */
#define BTM_BL_INQUIRY_PAGING_MASK 0x10
#define BTM_BL_INQUIRY_STARTED   (BTM_BL_INQUIRY_PAGING_MASK | 0x1)
#define BTM_BL_INQUIRY_CANCELLED (BTM_BL_INQUIRY_PAGING_MASK | 0x2)
#define BTM_BL_INQUIRY_COMPLETE  (BTM_BL_INQUIRY_PAGING_MASK | 0x3)
#define BTM_BL_PAGING_STARTED    (BTM_BL_INQUIRY_PAGING_MASK | 0x4)
#define BTM_BL_PAGING_COMPLETE   (BTM_BL_INQUIRY_PAGING_MASK | 0x5)
/* the data type associated with BTM_BL_UPDATE_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;  /* The event reported. */
    UINT8           busy_level;/* when paging or inquiring, level is as above.
                                * Otherwise, the number of ACL links. */
} tBTM_BL_UPDATE_DATA;

/* the data type associated with BTM_BL_ROLE_CHG_EVT */
typedef struct
{
    tBTM_BL_EVENT   event;      /* The event reported. */
    BD_ADDR_PTR     p_bda;      /* The address of the peer connected device */
    UINT8           new_role;
    UINT8           hci_status; /* HCI status returned with the event */
} tBTM_BL_ROLE_CHG_DATA;

typedef union
{
    tBTM_BL_EVENT           event;  /* The event reported. */
    tBTM_BL_CONN_DATA       conn;   /* The data associated with BTM_BL_CONN_EVT */
    tBTM_BL_DISCN_DATA      discn;  /* The data associated with BTM_BL_DISCN_EVT */
    tBTM_BL_UPDATE_DATA     update; /* The data associated with BTM_BL_UPDATE_EVT */
    tBTM_BL_ROLE_CHG_DATA   role_chg;/*The data associated with BTM_BL_ROLE_CHG_EVT */
} tBTM_BL_EVENT_DATA;

/* Callback function for notifications when the BTM busy level
** changes.
*/
typedef void (tBTM_BL_CHANGE_CB) (tBTM_BL_EVENT_DATA *p_data);

/***************************
**  ACL Callback Functions
****************************/
/* Callback function for notifications when the BTM ACL connection DB
** changes. First param is BD address, second is if added or removed.
** Registered through BTM_AclRegisterForChanges call.
*/
typedef void (tBTM_ACL_DB_CHANGE_CB) (BD_ADDR p_bda, DEV_CLASS p_dc,
                                      BD_NAME p_bdn, BD_FEATURES features,
                                      BOOLEAN is_new);

/*****************************************************************************
**  SCO CHANNEL MANAGEMENT
*****************************************************************************/
/******************
**  SCO Constants
*******************/

/* Define an invalid SCO index and an invalid HCI handle */
#define BTM_INVALID_SCO_INDEX       0xFFFF
#define BTM_INVALID_HCI_HANDLE      0xFFFF

/* Define an invalid SCO disconnect reason */
#define BTM_INVALID_SCO_DISC_REASON 0xFFFF

/* Define SCO packet types used in APIs */
#define BTM_SCO_PKT_TYPES_MASK_HV1  HCI_ESCO_PKT_TYPES_MASK_HV1
#define BTM_SCO_PKT_TYPES_MASK_HV2  HCI_ESCO_PKT_TYPES_MASK_HV2
#define BTM_SCO_PKT_TYPES_MASK_HV3  HCI_ESCO_PKT_TYPES_MASK_HV3
#define BTM_SCO_PKT_TYPES_MASK_EV3  HCI_ESCO_PKT_TYPES_MASK_EV3
#define BTM_SCO_PKT_TYPES_MASK_EV4  HCI_ESCO_PKT_TYPES_MASK_EV4
#define BTM_SCO_PKT_TYPES_MASK_EV5  HCI_ESCO_PKT_TYPES_MASK_EV5
#define BTM_SCO_PKT_TYPES_MASK_NO_2_EV3  HCI_ESCO_PKT_TYPES_MASK_NO_2_EV3
#define BTM_SCO_PKT_TYPES_MASK_NO_3_EV3  HCI_ESCO_PKT_TYPES_MASK_NO_3_EV3
#define BTM_SCO_PKT_TYPES_MASK_NO_2_EV5  HCI_ESCO_PKT_TYPES_MASK_NO_2_EV5
#define BTM_SCO_PKT_TYPES_MASK_NO_3_EV5  HCI_ESCO_PKT_TYPES_MASK_NO_3_EV5

#define BTM_SCO_LINK_ONLY_MASK  (BTM_SCO_PKT_TYPES_MASK_HV1 | \
                                 BTM_SCO_PKT_TYPES_MASK_HV2 | \
                                 BTM_SCO_PKT_TYPES_MASK_HV3)

#define BTM_ESCO_LINK_ONLY_MASK (BTM_SCO_PKT_TYPES_MASK_EV3 | \
                                 BTM_SCO_PKT_TYPES_MASK_EV4 | \
                                 BTM_SCO_PKT_TYPES_MASK_EV5)

#define BTM_SCO_LINK_ALL_PKT_MASK   (BTM_SCO_LINK_ONLY_MASK     | \
                                     BTM_ESCO_LINK_ONLY_MASK)

#define BTM_VALID_SCO_ALL_PKT_TYPE HCI_VALID_SCO_ALL_PKT_TYPE

/* Passed in BTM_CreateSco if the packet type parameter should be ignored */
#define BTM_IGNORE_SCO_PKT_TYPE     0

/***************
**  SCO Types
****************/
#define BTM_LINK_TYPE_SCO           HCI_LINK_TYPE_SCO
#define BTM_LINK_TYPE_ESCO          HCI_LINK_TYPE_ESCO
typedef UINT8 tBTM_SCO_TYPE;


/*******************
** SCO Routing Path
********************/
#define BTM_SCO_ROUTE_PCM           HCI_BRCM_SCO_ROUTE_PCM
#define BTM_SCO_ROUTE_HCI           HCI_BRCM_SCO_ROUTE_HCI
typedef UINT8 tBTM_SCO_ROUTE_TYPE;


/*******************
** SCO Codec Types
********************/
#define BTM_SCO_CODEC_NONE          0x0000
#define BTM_SCO_CODEC_CVSD          0x0001
#define BTM_SCO_CODEC_MSBC          0x0002
typedef UINT16 tBTM_SCO_CODEC_TYPE;

/*******************
** SCO Voice Settings
********************/
#define BTM_VOICE_SETTING_CVSD  ((UINT16)  (HCI_INP_CODING_LINEAR          |   \
                                            HCI_INP_DATA_FMT_2S_COMPLEMENT |   \
                                            HCI_INP_SAMPLE_SIZE_16BIT      |   \
                                            HCI_AIR_CODING_FORMAT_CVSD))

#define BTM_VOICE_SETTING_TRANS ((UINT16)  (HCI_INP_CODING_LINEAR          |   \
                                            HCI_INP_DATA_FMT_2S_COMPLEMENT |   \
                                            HCI_INP_SAMPLE_SIZE_16BIT      |   \
                                            HCI_AIR_CODING_FORMAT_TRANSPNT))

/*******************
** SCO Data Status
********************/
enum
{
    BTM_SCO_DATA_CORRECT,
    BTM_SCO_DATA_PAR_ERR,
    BTM_SCO_DATA_NONE,
    BTM_SCO_DATA_PAR_LOST
};
typedef UINT8 tBTM_SCO_DATA_FLAG;

/***************************
**  SCO Callback Functions
****************************/
typedef void (tBTM_SCO_CB) (UINT16 sco_inx);
typedef void (tBTM_SCO_DATA_CB) (UINT16 sco_inx, BT_HDR *p_data, tBTM_SCO_DATA_FLAG status);

/******************
**  eSCO Constants
*******************/
#define BTM_64KBITS_RATE            0x00001f40  /* 64 kbits/sec data rate */

/* Retransmission effort */
#define BTM_ESCO_RETRANS_OFF        0
#define BTM_ESCO_RETRANS_POWER      1
#define BTM_ESCO_RETRANS_QUALITY    2
#define BTM_ESCO_RETRANS_DONTCARE   0xff

/* Max Latency Don't Care */
#define BTM_ESCO_MAX_LAT_DONTCARE   0xffff

/***************
**  eSCO Types
****************/
/* tBTM_ESCO_CBACK event types */
#define BTM_ESCO_CHG_EVT        1
#define BTM_ESCO_CONN_REQ_EVT   2
typedef UINT8 tBTM_ESCO_EVT;

/* Passed into BTM_SetEScoMode() */
typedef struct
{
    UINT32 tx_bw;
    UINT32 rx_bw;
    UINT16 max_latency;
    UINT16 voice_contfmt;  /* Voice Settings or Content Format */
    UINT16 packet_types;
    UINT8  retrans_effort;
} tBTM_ESCO_PARAMS;

typedef struct
{
    UINT16 max_latency;
    UINT16 packet_types;
    UINT8  retrans_effort;
} tBTM_CHG_ESCO_PARAMS;

/* Returned by BTM_ReadEScoLinkParms() */
typedef struct
{
    UINT16  rx_pkt_len;
    UINT16  tx_pkt_len;
    BD_ADDR bd_addr;
    UINT8   link_type;  /* BTM_LINK_TYPE_SCO or BTM_LINK_TYPE_ESCO */
    UINT8   tx_interval;
    UINT8   retrans_window;
    UINT8   air_mode;
} tBTM_ESCO_DATA;

typedef struct
{
    UINT16  sco_inx;
    UINT16  rx_pkt_len;
    UINT16  tx_pkt_len;
    BD_ADDR bd_addr;
    UINT8   hci_status;
    UINT8   tx_interval;
    UINT8   retrans_window;
} tBTM_CHG_ESCO_EVT_DATA;

typedef struct
{
    UINT16        sco_inx;
    BD_ADDR       bd_addr;
    DEV_CLASS     dev_class;
    tBTM_SCO_TYPE link_type;
} tBTM_ESCO_CONN_REQ_EVT_DATA;

typedef union
{
    tBTM_CHG_ESCO_EVT_DATA      chg_evt;
    tBTM_ESCO_CONN_REQ_EVT_DATA conn_evt;
} tBTM_ESCO_EVT_DATA;

/***************************
**  eSCO Callback Functions
****************************/
typedef void (tBTM_ESCO_CBACK) (tBTM_ESCO_EVT event, tBTM_ESCO_EVT_DATA *p_data);


/*****************************************************************************
**  SECURITY MANAGEMENT
*****************************************************************************/
/*******************************
**  Security Manager Constants
********************************/

/* Security Mode (BTM_SetSecurityMode) */
#define BTM_SEC_MODE_UNDEFINED      0
#define BTM_SEC_MODE_NONE           1
#define BTM_SEC_MODE_SERVICE        2
#define BTM_SEC_MODE_LINK           3
#define BTM_SEC_MODE_SP             4
#define BTM_SEC_MODE_SP_DEBUG       5

/* Security Service Levels [bit mask] (BTM_SetSecurityLevel)
** Encryption should not be used without authentication
*/
#define BTM_SEC_NONE               0x0000 /* Nothing required */
#define BTM_SEC_IN_AUTHORIZE       0x0001 /* Inbound call requires authorization */
#define BTM_SEC_IN_AUTHENTICATE    0x0002 /* Inbound call requires authentication */
#define BTM_SEC_IN_ENCRYPT         0x0004 /* Inbound call requires encryption */
#define BTM_SEC_OUT_AUTHORIZE      0x0008 /* Outbound call requires authorization */
#define BTM_SEC_OUT_AUTHENTICATE   0x0010 /* Outbound call requires authentication */
#define BTM_SEC_OUT_ENCRYPT        0x0020 /* Outbound call requires encryption */
#define BTM_SEC_BOND               0x0040 /* Bonding */
#define BTM_SEC_BOND_CONN          0x0080 /* bond_created_connection */
#define BTM_SEC_FORCE_MASTER       0x0100 /* Need to switch connection to be master */
#define BTM_SEC_ATTEMPT_MASTER     0x0200 /* Try to switch connection to be master */
#define BTM_SEC_FORCE_SLAVE        0x0400 /* Need to switch connection to be master */
#define BTM_SEC_ATTEMPT_SLAVE      0x0800 /* Try to switch connection to be slave */
#define BTM_SEC_IN_MITM            0x1000 /* inbound Do man in the middle protection */
#define BTM_SEC_OUT_MITM           0x2000 /* outbound Do man in the middle protection */

/* Security Flags [bit mask] (BTM_GetSecurityFlags)
*/
#define BTM_SEC_FLAG_AUTHORIZED     0x01
#define BTM_SEC_FLAG_AUTHENTICATED  0x02
#define BTM_SEC_FLAG_ENCRYPTED      0x04
#define BTM_SEC_FLAG_LKEY_KNOWN     0x10
#define BTM_SEC_FLAG_LKEY_AUTHED    0x20

/* PIN types */
#define BTM_PIN_TYPE_VARIABLE       HCI_PIN_TYPE_VARIABLE
#define BTM_PIN_TYPE_FIXED          HCI_PIN_TYPE_FIXED

/* Link Key types used to generate the new link key.
** returned in link key notification callback function
*/
#define BTM_LKEY_TYPE_COMBINATION   HCI_LKEY_TYPE_COMBINATION
#define BTM_LKEY_TYPE_LOCAL_UNIT    HCI_LKEY_TYPE_LOCAL_UNIT
#define BTM_LKEY_TYPE_REMOTE_UNIT   HCI_LKEY_TYPE_REMOTE_UNIT
#define BTM_LKEY_TYPE_DEBUG_COMB    HCI_LKEY_TYPE_DEBUG_COMB
#define BTM_LKEY_TYPE_UNAUTH_COMB   HCI_LKEY_TYPE_UNAUTH_COMB
#define BTM_LKEY_TYPE_AUTH_COMB     HCI_LKEY_TYPE_AUTH_COMB
#define BTM_LKEY_TYPE_CHANGED_COMB  HCI_LKEY_TYPE_CHANGED_COMB
#define BTM_LKEY_TYPE_IGNORE        0xff    /* used when event is response from
                                               hci return link keys request */

/* Protocol level security (BTM_SetSecurityLevel) */
#define BTM_SEC_PROTO_L2CAP         0
#define BTM_SEC_PROTO_SDP           1
#define BTM_SEC_PROTO_TCS           2
#define BTM_SEC_PROTO_RFCOMM        3
#define BTM_SEC_PROTO_OBEX          4
#define BTM_SEC_PROTO_BNEP          5
#define BTM_SEC_PROTO_HID           6   /* HID      */
#define BTM_SEC_PROTO_AVDT          7
#define BTM_SEC_PROTO_MCA           8

/* Determine the number of UINT32's necessary for security services */
#define BTM_SEC_ARRAY_BITS          32          /* Number of bits in each array element */
#define BTM_SEC_SERVICE_ARRAY_SIZE  (((UINT32)BTM_SEC_MAX_SERVICES / BTM_SEC_ARRAY_BITS) + \
                                    (((UINT32)BTM_SEC_MAX_SERVICES % BTM_SEC_ARRAY_BITS) ? 1 : 0))

/* Security service definitions (BTM_SetSecurityLevel)
** Used for Authorization APIs
*/
#define BTM_SEC_SERVICE_SDP_SERVER      0
#define BTM_SEC_SERVICE_SERIAL_PORT     1
#define BTM_SEC_SERVICE_LAN_ACCESS      2
#define BTM_SEC_SERVICE_DUN             3
#define BTM_SEC_SERVICE_IRMC_SYNC       4
#define BTM_SEC_SERVICE_IRMC_SYNC_CMD   5
#define BTM_SEC_SERVICE_OBEX            6
#define BTM_SEC_SERVICE_OBEX_FTP        7
#define BTM_SEC_SERVICE_HEADSET         8
#define BTM_SEC_SERVICE_CORDLESS        9
#define BTM_SEC_SERVICE_INTERCOM        10
#define BTM_SEC_SERVICE_FAX             11
#define BTM_SEC_SERVICE_HEADSET_AG      12
#define BTM_SEC_SERVICE_PNP_INFO        13
#define BTM_SEC_SERVICE_GEN_NET         14
#define BTM_SEC_SERVICE_GEN_FILE        15
#define BTM_SEC_SERVICE_GEN_AUDIO       16
#define BTM_SEC_SERVICE_GEN_TEL         17
#define BTM_SEC_SERVICE_CTP_DATA        18
#define BTM_SEC_SERVICE_HCRP_CTRL       19
#define BTM_SEC_SERVICE_HCRP_DATA       20
#define BTM_SEC_SERVICE_HCRP_NOTIF      21
#define BTM_SEC_SERVICE_BPP_JOB         22
#define BTM_SEC_SERVICE_BPP_STATUS      23
#define BTM_SEC_SERVICE_BPP_REF         24
#define BTM_SEC_SERVICE_BNEP_PANU       25
#define BTM_SEC_SERVICE_BNEP_GN         26
#define BTM_SEC_SERVICE_BNEP_NAP        27
#define BTM_SEC_SERVICE_HF_HANDSFREE    28
#define BTM_SEC_SERVICE_AG_HANDSFREE    29
#define BTM_SEC_SERVICE_TE_PHONE_ACCESS 30
#define BTM_SEC_SERVICE_ME_PHONE_ACCESS 31

#define BTM_SEC_SERVICE_HID_SEC_CTRL    32
#define BTM_SEC_SERVICE_HID_NOSEC_CTRL  33
#define BTM_SEC_SERVICE_HID_INTR        34
#define BTM_SEC_SERVICE_BIP             35
#define BTM_SEC_SERVICE_BIP_REF         36
#define BTM_SEC_SERVICE_AVDTP           37
#define BTM_SEC_SERVICE_AVDTP_NOSEC     38
#define BTM_SEC_SERVICE_AVCTP           39
#define BTM_SEC_SERVICE_SAP             40
#define BTM_SEC_SERVICE_PBAP            41
#define BTM_SEC_SERVICE_RFC_MUX         42
#define BTM_SEC_SERVICE_AVCTP_BROWSE    43
#define BTM_SEC_SERVICE_MAP             44
#define BTM_SEC_SERVICE_MAP_NOTIF       45
#define BTM_SEC_SERVICE_MCAP_CTRL       46
#define BTM_SEC_SERVICE_MCAP_DATA       47
#define BTM_SEC_SERVICE_HDP_SNK         48
#define BTM_SEC_SERVICE_HDP_SRC         49
#define BTM_SEC_SERVICE_ATT             50
#define BTM_SEC_SERVICE_HIDD_SEC_CTRL   51
#define BTM_SEC_SERVICE_HIDD_NOSEC_CTRL 52
#define BTM_SEC_SERVICE_HIDD_INTR       53

/* Update these as services are added */
#define BTM_SEC_SERVICE_FIRST_EMPTY     54

#ifndef BTM_SEC_MAX_SERVICES
/* accomadate client profiles also */
#define BTM_SEC_MAX_SERVICES            70
#endif

/************************************************************************************************
** Security Services MACROS handle array of UINT32 bits for more than 32 trusted services
*************************************************************************************************/
/* MACRO to set the security service bit mask in a bit stream */
#define BTM_SEC_SET_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_SEC_ARRAY_BITS)] |=  \
                                    ((UINT32)1 << (((UINT32)(service)) % BTM_SEC_ARRAY_BITS)))


/* MACRO to clear the security service bit mask in a bit stream */
#define BTM_SEC_CLR_SERVICE(p, service)  (((UINT32 *)(p))[(((UINT32)(service)) / BTM_SEC_ARRAY_BITS)] &=  \
                                    ~((UINT32)1 << (((UINT32)(service)) % BTM_SEC_ARRAY_BITS)))

/* MACRO to check the security service bit mask in a bit stream (Returns TRUE or FALSE) */
#define BTM_SEC_IS_SERVICE_TRUSTED(p, service)    (((((UINT32 *)(p))[(((UINT32)(service)) / BTM_SEC_ARRAY_BITS)]) &   \
                                        (UINT32)(((UINT32)1 << (((UINT32)(service)) % BTM_SEC_ARRAY_BITS)))) ? TRUE : FALSE)

/* MACRO to copy two trusted device bitmask */
#define BTM_SEC_COPY_TRUSTED_DEVICE(p_src, p_dst)   {int trst; for (trst = 0; trst < BTM_SEC_SERVICE_ARRAY_SIZE; trst++) \
                                                        ((UINT32 *)(p_dst))[trst] = ((UINT32 *)(p_src))[trst];}

/* MACRO to clear two trusted device bitmask */
#define BTM_SEC_CLR_TRUSTED_DEVICE(p_dst)   {int trst; for (trst = 0; trst < BTM_SEC_SERVICE_ARRAY_SIZE; trst++) \
                                                        ((UINT32 *)(p_dst))[trst] = 0;}

/* Following bits can be provided by host in the trusted_mask array */
/* 0..31 bits of mask[0] (Least Significant Word) */
#define BTM_SEC_TRUST_SDP_SERVER        (1 << BTM_SEC_SERVICE_SDP_SERVER)
#define BTM_SEC_TRUST_SERIAL_PORT       (1 << BTM_SEC_SERVICE_SERIAL_PORT)
#define BTM_SEC_TRUST_LAN_ACCESS        (1 << BTM_SEC_SERVICE_LAN_ACCESS)
#define BTM_SEC_TRUST_DUN               (1 << BTM_SEC_SERVICE_DUN)
#define BTM_SEC_TRUST_IRMC_SYNC         (1 << BTM_SEC_SERVICE_IRMC_SYNC)
#define BTM_SEC_TRUST_IRMC_SYNC_CMD     (1 << BTM_SEC_SERVICE_IRMC_SYNC_CMD)
#define BTM_SEC_TRUST_OBEX              (1 << BTM_SEC_SERVICE_OBEX)
#define BTM_SEC_TRUST_OBEX_FTP          (1 << BTM_SEC_SERVICE_OBEX_FTP)
#define BTM_SEC_TRUST_HEADSET           (1 << BTM_SEC_SERVICE_HEADSET)
#define BTM_SEC_TRUST_CORDLESS          (1 << BTM_SEC_SERVICE_CORDLESS)
#define BTM_SEC_TRUST_INTERCOM          (1 << BTM_SEC_SERVICE_INTERCOM)
#define BTM_SEC_TRUST_FAX               (1 << BTM_SEC_SERVICE_FAX)
#define BTM_SEC_TRUST_HEADSET_AG        (1 << BTM_SEC_SERVICE_HEADSET_AG)
#define BTM_SEC_TRUST_PNP_INFO          (1 << BTM_SEC_SERVICE_PNP_INFO)
#define BTM_SEC_TRUST_GEN_NET           (1 << BTM_SEC_SERVICE_GEN_NET)
#define BTM_SEC_TRUST_GEN_FILE          (1 << BTM_SEC_SERVICE_GEN_FILE)
#define BTM_SEC_TRUST_GEN_AUDIO         (1 << BTM_SEC_SERVICE_GEN_AUDIO)
#define BTM_SEC_TRUST_GEN_TEL           (1 << BTM_SEC_SERVICE_GEN_TEL)
#define BTM_SEC_TRUST_CTP_DATA          (1 << BTM_SEC_SERVICE_CTP_DATA)
#define BTM_SEC_TRUST_HCRP_CTRL         (1 << BTM_SEC_SERVICE_HCRP_CTRL)
#define BTM_SEC_TRUST_HCRP_DATA         (1 << BTM_SEC_SERVICE_HCRP_DATA)
#define BTM_SEC_TRUST_HCRP_NOTIF        (1 << BTM_SEC_SERVICE_HCRP_NOTIF)
#define BTM_SEC_TRUST_BPP_JOB           (1 << BTM_SEC_SERVICE_JOB)
#define BTM_SEC_TRUST_BPP_STATUS        (1 << BTM_SEC_SERVICE_STATUS)
#define BTM_SEC_TRUST_BPP_REF           (1 << BTM_SEC_SERVICE_REF)
#define BTM_SEC_TRUST_BNEP_PANU         (1 << BTM_SEC_SERVICE_BNEP_PANU)
#define BTM_SEC_TRUST_BNEP_GN           (1 << BTM_SEC_SERVICE_BNEP_GN)
#define BTM_SEC_TRUST_BNEP_NAP          (1 << BTM_SEC_SERVICE_BNEP_NAP)
#define BTM_SEC_TRUST_HFP_HF            (1 << BTM_SEC_SERVICE_HF_HANDSFREE)
#define BTM_SEC_TRUST_HFP_AG            (1 << BTM_SEC_SERVICE_AG_HANDSFREE)
#define BTM_SEC_TRUST_TE_PHONE_ACCESS   (1 << BTM_SEC_SERVICE_TE_PHONE_ACCESS)
#define BTM_SEC_TRUST_ME_PHONE_ACCESS   (1 << BTM_SEC_SERVICE_ME_PHONE_ACCESS)

/* 0..31 bits of mask[1] (Most Significant Word) */
#define BTM_SEC_TRUST_HID_CTRL          (1 << (BTM_SEC_SERVICE_HID_SEC_CTRL - 32))
#define BTM_SEC_TRUST_HID_NOSEC_CTRL    (1 << (BTM_SEC_SERVICE_HID_NOSEC_CTRL - 32))
#define BTM_SEC_TRUST_HID_INTR          (1 << (BTM_SEC_SERVICE_HID_INTR - 32))
#define BTM_SEC_TRUST_BIP               (1 << (BTM_SEC_SERVICE_BIP - 32))
#define BTM_SEC_TRUST_BIP_REF           (1 << (BTM_SEC_SERVICE_BIP_REF - 32))
#define BTM_SEC_TRUST_AVDTP             (1 << (BTM_SEC_SERVICE_AVDTP - 32))
#define BTM_SEC_TRUST_AVDTP_NOSEC       (1 << (BTM_SEC_SERVICE_AVDTP_NOSEC - 32))
#define BTM_SEC_TRUST_AVCTP             (1 << (BTM_SEC_SERVICE_AVCTP - 32))
#define BTM_SEC_TRUST_SAP               (1 << (BTM_SEC_SERVICE_SAP - 32))
#define BTM_SEC_TRUST_PBAP              (1 << (BTM_SEC_SERVICE_PBAP - 32))
#define BTM_SEC_TRUST_RFC_MUX           (1 << (BTM_SEC_SERVICE_RFC_MUX - 32))
#define BTM_SEC_TRUST_AVCTP_BROWSE      (1 << (BTM_SEC_SERVICE_AVCTP_BROWSE - 32))
#define BTM_SEC_TRUST_MAP               (1 << (BTM_SEC_SERVICE_MAP - 32))
#define BTM_SEC_TRUST_MAP_NOTIF         (1 << (BTM_SEC_SERVICE_MAP_NOTIF - 32))
#define BTM_SEC_TRUST_MCAP_CTRL         (1 << (BTM_SEC_SERVICE_MCAP_CTRL - 32))
#define BTM_SEC_TRUST_MCAP_DATA         (1 << (BTM_SEC_SERVICE_MCAP_DATA - 32))
#define BTM_SEC_TRUST_HDP_SNK           (1 << (BTM_SEC_SERVICE_HDP_SNK - 32))
#define BTM_SEC_TRUST_HDP_SRC           (1 << (BTM_SEC_SERVICE_HDP_SRC - 32))

#define BTM_SEC_TRUST_ALL               0xFFFFFFFF  /* for each array element */

/****************************************
**  Security Manager Callback Functions
*****************************************/
/* Authorize device for service.  Parameters are
**              BD Address of remote
**              Device Class of remote
**              BD Name of remote
**              Service name
**              Service Id (NULL - unknown service or unused
**                                 [BTM_SEC_SERVICE_NAME_LEN set to 0])
**              Is originator of the connection
**              Result of the operation
*/
typedef UINT8 (tBTM_AUTHORIZE_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dev_class,
                                         tBTM_BD_NAME bd_name, UINT8 *service_name,
                                         UINT8 service_id, BOOLEAN is_originator);

/* Get PIN for the connection.  Parameters are
**              BD Address of remote
**              Device Class of remote
**              BD Name of remote
*/
typedef UINT8 (tBTM_PIN_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dev_class,
                                   tBTM_BD_NAME bd_name);


/* Get Link Key for the connection.  Parameters are
**              BD Address of remote
**              Link Key
*/
typedef UINT8 (tBTM_LINK_KEY_REQ_CALLBACK) (BD_ADDR bd_addr, LINK_KEY key);

/* New Link Key for the connection.  Parameters are
**              BD Address of remote
**              Link Key
**              Key Type: Combination, Local Unit, or Remote Unit
*/
typedef UINT8 (tBTM_LINK_KEY_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dev_class,
                                        tBTM_BD_NAME bd_name, UINT8 *key,
                                        UINT8 key_type);


/* Remote Name Resolved.  Parameters are
**              BD Address of remote
**              BD Name of remote
*/
typedef void (tBTM_RMT_NAME_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dc,
                                       tBTM_BD_NAME bd_name);


/* Authentication complete for the connection.  Parameters are
**              BD Address of remote
**              Device Class of remote
**              BD Name of remote
**
*/
typedef UINT8 (tBTM_AUTH_COMPLETE_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dev_class,
                                             tBTM_BD_NAME bd_name, int result);

/* Operation abort.  Called by the stack when link goes down during.  Pin code
** request or authorization.  Parameters are
**              BD Address of remote
**
*/
typedef UINT8 (tBTM_ABORT_CALLBACK) (BD_ADDR bd_addr, DEV_CLASS dev_class,
                                     tBTM_BD_NAME bd_name);

enum
{
    BTM_SP_IO_REQ_EVT,      /* received IO_CAPABILITY_REQUEST event */
    BTM_SP_IO_RSP_EVT,      /* received IO_CAPABILITY_RESPONSE event */
    BTM_SP_CFM_REQ_EVT,     /* received USER_CONFIRMATION_REQUEST event */
    BTM_SP_KEY_NOTIF_EVT,   /* received USER_PASSKEY_NOTIFY event */
    BTM_SP_KEY_REQ_EVT,     /* received USER_PASSKEY_REQUEST event */
    BTM_SP_KEYPRESS_EVT,    /* received KEYPRESS_NOTIFY event */
    BTM_SP_LOC_OOB_EVT,     /* received result for READ_LOCAL_OOB_DATA command */
    BTM_SP_RMT_OOB_EVT,     /* received REMOTE_OOB_DATA_REQUEST event */
    BTM_SP_COMPLT_EVT,      /* received SIMPLE_PAIRING_COMPLETE event */
    BTM_SP_UPGRADE_EVT      /* check if the application wants to upgrade the link key */
};
typedef UINT8 tBTM_SP_EVT;

#define BTM_IO_CAP_OUT      0   /* DisplayOnly */
#define BTM_IO_CAP_IO       1   /* DisplayYesNo */
#define BTM_IO_CAP_IN       2   /* KeyboardOnly */
#define BTM_IO_CAP_NONE     3   /* NoInputNoOutput */
#define BTM_IO_CAP_MAX      4

typedef UINT8 tBTM_IO_CAP;

#define BTM_MAX_PASSKEY_VAL (999999)
#define BTM_MIN_PASSKEY_VAL (0)

#define BTM_AUTH_SP_NO      0   /* MITM Protection Not Required - Single Profile/non-bonding
                                Numeric comparison with automatic accept allowed */
#define BTM_AUTH_SP_YES     1   /* MITM Protection Required - Single Profile/non-bonding
                                Use IO Capabilities to determine authentication procedure */
#define BTM_AUTH_AP_NO      2   /* MITM Protection Not Required - All Profiles/dedicated bonding
                                Numeric comparison with automatic accept allowed */
#define BTM_AUTH_AP_YES     3   /* MITM Protection Required - All Profiles/dedicated bonding
                                Use IO Capabilities to determine authentication procedure */
#define BTM_AUTH_SPGB_NO    4   /* MITM Protection Not Required - Single Profiles/general bonding
                                Numeric comparison with automatic accept allowed */
#define BTM_AUTH_SPGB_YES   5   /* MITM Protection Required - Single Profiles/general bonding
                                Use IO Capabilities to determine authentication procedure */
#define BTM_AUTH_DD_BOND    2   /* this bit is ORed to the BTM_AUTH_SP_* when IO exchange for dedicated bonding */
#define BTM_AUTH_GB_BIT     4   /* the genernal bonding bit */
#define BTM_AUTH_BONDS      6   /* the general/dedicated bonding bits  */
#define BTM_AUTH_YN_BIT     1   /* this is the Yes or No bit  */

typedef UINT8 tBTM_AUTH_REQ;

enum
{
    BTM_OOB_NONE,
    BTM_OOB_PRESENT
#if BTM_OOB_INCLUDED == TRUE
    ,BTM_OOB_UNKNOWN
#endif
};
typedef UINT8 tBTM_OOB_DATA;

/* data type for BTM_SP_IO_REQ_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    tBTM_IO_CAP     io_cap;         /* local IO capabilities */
    tBTM_OOB_DATA   oob_data;       /* OOB data present (locally) for the peer device */
    tBTM_AUTH_REQ   auth_req;       /* Authentication required (for local device) */
    BOOLEAN         is_orig;        /* TRUE, if local device initiated the SP process */
} tBTM_SP_IO_REQ;

/* data type for BTM_SP_IO_RSP_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    tBTM_IO_CAP     io_cap;         /* peer IO capabilities */
    tBTM_OOB_DATA   oob_data;       /* OOB data present at peer device for the local device */
    tBTM_AUTH_REQ   auth_req;       /* Authentication required for peer device */
} tBTM_SP_IO_RSP;

/* data type for BTM_SP_CFM_REQ_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    DEV_CLASS       dev_class;      /* peer CoD */
    tBTM_BD_NAME    bd_name;        /* peer device name */
    UINT32          num_val;        /* the numeric value for comparison. If just_works, do not show this number to UI */
    BOOLEAN         just_works;     /* TRUE, if "Just Works" association model */
    tBTM_AUTH_REQ   loc_auth_req;   /* Authentication required for local device */
    tBTM_AUTH_REQ   rmt_auth_req;   /* Authentication required for peer device */
    tBTM_IO_CAP     loc_io_caps;    /* IO Capabilities of the local device */
    tBTM_IO_CAP     rmt_io_caps;    /* IO Capabilities of the remot device */
} tBTM_SP_CFM_REQ;

/* data type for BTM_SP_KEY_REQ_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    DEV_CLASS       dev_class;      /* peer CoD */
    tBTM_BD_NAME    bd_name;        /* peer device name */
} tBTM_SP_KEY_REQ;

/* data type for BTM_SP_KEY_NOTIF_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    DEV_CLASS       dev_class;      /* peer CoD */
    tBTM_BD_NAME    bd_name;        /* peer device name */
    UINT32          passkey;        /* passkey */
} tBTM_SP_KEY_NOTIF;

enum
{
    BTM_SP_KEY_STARTED,         /* passkey entry started */
    BTM_SP_KEY_ENTERED,         /* passkey digit entered */
    BTM_SP_KEY_ERASED,          /* passkey digit erased */
    BTM_SP_KEY_CLEARED,         /* passkey cleared */
    BTM_SP_KEY_COMPLT           /* passkey entry completed */
};
typedef UINT8   tBTM_SP_KEY_TYPE;

/* data type for BTM_SP_KEYPRESS_EVT */
typedef struct
{
    BD_ADDR             bd_addr;        /* peer address */
    tBTM_SP_KEY_TYPE   notif_type;
} tBTM_SP_KEYPRESS;

/* data type for BTM_SP_LOC_OOB_EVT */
typedef struct
{
    tBTM_STATUS     status;         /* */
    BT_OCTET16      c;              /* Simple Pairing Hash C */
    BT_OCTET16      r;              /* Simple Pairing Randomnizer R */
} tBTM_SP_LOC_OOB;

/* data type for BTM_SP_RMT_OOB_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    DEV_CLASS       dev_class;      /* peer CoD */
    tBTM_BD_NAME    bd_name;        /* peer device name */
} tBTM_SP_RMT_OOB;


/* data type for BTM_SP_COMPLT_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    DEV_CLASS       dev_class;      /* peer CoD */
    tBTM_BD_NAME    bd_name;        /* peer device name */
    tBTM_STATUS     status;         /* status of the simple pairing process */
} tBTM_SP_COMPLT;

/* data type for BTM_SP_UPGRADE_EVT */
typedef struct
{
    BD_ADDR         bd_addr;        /* peer address */
    BOOLEAN         upgrade;        /* TRUE, to upgrade the link key */
} tBTM_SP_UPGRADE;

typedef union
{
    tBTM_SP_IO_REQ      io_req;     /* BTM_SP_IO_REQ_EVT      */
    tBTM_SP_IO_RSP      io_rsp;     /* BTM_SP_IO_RSP_EVT      */
    tBTM_SP_CFM_REQ     cfm_req;    /* BTM_SP_CFM_REQ_EVT     */
    tBTM_SP_KEY_NOTIF   key_notif;  /* BTM_SP_KEY_NOTIF_EVT   */
    tBTM_SP_KEY_REQ     key_req;    /* BTM_SP_KEY_REQ_EVT     */
    tBTM_SP_KEYPRESS    key_press;  /* BTM_SP_KEYPRESS_EVT    */
    tBTM_SP_LOC_OOB     loc_oob;    /* BTM_SP_LOC_OOB_EVT     */
    tBTM_SP_RMT_OOB     rmt_oob;    /* BTM_SP_RMT_OOB_EVT     */
    tBTM_SP_COMPLT      complt;     /* BTM_SP_COMPLT_EVT      */
    tBTM_SP_UPGRADE     upgrade;    /* BTM_SP_UPGRADE_EVT      */
} tBTM_SP_EVT_DATA;

/* Simple Pairing Events.  Called by the stack when Simple Pairing related
** events occur.
*/
typedef UINT8 (tBTM_SP_CALLBACK) (tBTM_SP_EVT event, tBTM_SP_EVT_DATA *p_data);


typedef void (tBTM_MKEY_CALLBACK) (BD_ADDR bd_addr, UINT8 status, UINT8 key_flag) ;

/* Encryption enabled/disabled complete: Optionally passed with BTM_SetEncryption.
** Parameters are
**              BD Address of remote
**              optional data passed in by BTM_SetEncryption
**              tBTM_STATUS - result of the operation
*/
typedef void (tBTM_SEC_CBACK) (BD_ADDR bd_addr, void *p_ref_data, tBTM_STATUS result);

/* Bond Cancel complete. Parameters are
**              Result of the cancel operation
**
*/
typedef void (tBTM_BOND_CANCEL_CMPL_CALLBACK) (tBTM_STATUS result);

/* LE related event and data structure
*/
enum
{
    BTM_LE_IO_REQ_EVT = 1,      /* received IO_CAPABILITY_REQUEST event */
    BTM_LE_SEC_REQUEST_EVT, /* security request event */
    BTM_LE_KEY_NOTIF_EVT,   /* received USER_PASSKEY_NOTIFY event */
    BTM_LE_KEY_REQ_EVT,     /* received USER_PASSKEY_REQUEST event */
    BTM_LE_OOB_REQ_EVT,     /* OOB data request event */
    BTM_LE_COMPLT_EVT,      /* received SIMPLE_PAIRING_COMPLETE event */
    BTM_LE_KEY_EVT         /* KEY update event */
};
typedef UINT8 tBTM_LE_EVT;

#define BTM_LE_KEY_PENC      SMP_SEC_KEY_TYPE_ENC        /* encryption information of peer device */
#define BTM_LE_KEY_PID       SMP_SEC_KEY_TYPE_ID         /* identity key of the peer device */
#define BTM_LE_KEY_PCSRK      SMP_SEC_KEY_TYPE_CSRK        /* peer SRK */
#define BTM_LE_KEY_LENC      (SMP_SEC_KEY_TYPE_ENC << 3)       /* master role security information:div */
#define BTM_LE_KEY_LID       (SMP_SEC_KEY_TYPE_ID << 3)        /* master device ID key */
#define BTM_LE_KEY_LCSRK     (SMP_SEC_KEY_TYPE_CSRK << 3)       /* local CSRK has been deliver to peer */
typedef UINT8 tBTM_LE_KEY_TYPE;

#define BTM_LE_AUTH_REQ_NO_BOND SMP_AUTH_NO_BOND   /* 0 */
#define BTM_LE_AUTH_REQ_BOND    SMP_AUTH_GEN_BOND  /* 1 << 0 */
#define BTM_LE_AUTH_REQ_MITM    SMP_AUTH_YN_BIT    /* 1 << 2 */
typedef UINT8 tBTM_LE_AUTH_REQ;

#define BTM_LE_AUTH_REQ_MASK SMP_AUTH_MASK  /* 0x03*/

/* LE security level */
#define BTM_LE_SEC_NONE             SMP_SEC_NONE
#define BTM_LE_SEC_UNAUTHENTICATE   SMP_SEC_UNAUTHENTICATE
#define BTM_LE_SEC_AUTHENTICATED    SMP_SEC_AUTHENTICATED
typedef UINT8 tBTM_LE_SEC;


typedef struct
{
    tBTM_IO_CAP         io_cap;         /* local IO capabilities */
    UINT8               oob_data;       /* OOB data present (locally) for the peer device */
    tBTM_LE_AUTH_REQ    auth_req;       /* Authentication request (for local device) contain bonding and MITM info */
    UINT8               max_key_size;   /* max encryption key size */
    tBTM_LE_KEY_TYPE    init_keys;      /* keys to be distributed, bit mask */
    tBTM_LE_KEY_TYPE    resp_keys;      /* keys to be distributed, bit mask */
} tBTM_LE_IO_REQ;

/* BLE encryption keys */
typedef struct
{
    BT_OCTET16  ltk;
    BT_OCTET8   rand;
    UINT16      ediv;
    UINT8       sec_level;
    UINT8       key_size;
}tBTM_LE_PENC_KEYS;

/* BLE CSRK keys */
typedef struct
{
    UINT32          counter;
    BT_OCTET16      csrk;
    UINT8           sec_level;
}tBTM_LE_PCSRK_KEYS;

/* BLE Encryption reproduction keys */
typedef struct
{
    UINT16      div;
    UINT8       key_size;
    UINT8       sec_level;
}tBTM_LE_LENC_KEYS;

/* BLE SRK keys */
typedef struct
{
    UINT32          counter;
    UINT16          div;
    UINT8           sec_level;

}tBTM_LE_LCSRK_KEYS;


typedef union
{
    tBTM_LE_PENC_KEYS   penc_key;       /* received peer encryption key */
    tBTM_LE_PCSRK_KEYS   pcsrk_key;       /* received peer device SRK */
    BT_OCTET16          pid_key;        /* peer device ID key */
    tBTM_LE_LENC_KEYS   lenc_key;       /* local encryption reproduction keys LTK = = d1(ER,DIV,0)*/
    tBTM_LE_LCSRK_KEYS   lcsrk_key;      /* local device CSRK = d1(ER,DIV,1)*/
}tBTM_LE_KEY_VALUE;

typedef struct
{
    tBTM_LE_KEY_TYPE        key_type;
    tBTM_LE_KEY_VALUE       *p_key_value;
}tBTM_LE_KEY;

typedef union
{
    tBTM_LE_IO_REQ      io_req;     /* BTM_LE_IO_REQ_EVT      */
    UINT32              key_notif;  /* BTM_LE_KEY_NOTIF_EVT   */
                                    /* no callback dta for BTM_LE_KEY_REQ_EVT & BTM_LE_OOB_REQ_EVT  */
    tBTM_LE_KEY         key;
} tBTM_LE_EVT_DATA;

/* Simple Pairing Events.  Called by the stack when Simple Pairing related
** events occur.
*/
typedef UINT8 (tBTM_LE_CALLBACK) (tBTM_LE_EVT event, BD_ADDR bda, tBTM_LE_EVT_DATA *p_data);

#define BTM_BLE_KEY_TYPE_ID         1
#define BTM_BLE_KEY_TYPE_ER         2
#define BTM_BLE_KEY_TYPE_COUNTER    3  //tobe obsolete

typedef struct
{
    BT_OCTET16       ir;
    BT_OCTET16       irk;
    BT_OCTET16       dhk;

}tBTM_BLE_LOCAL_ID_KEYS;

typedef union
{
    tBTM_BLE_LOCAL_ID_KEYS  id_keys;
    BT_OCTET16              er;
}tBTM_BLE_LOCAL_KEYS;


/* New LE identity key for local device.
*/
typedef void (tBTM_LE_KEY_CALLBACK) (UINT8 key_type, tBTM_BLE_LOCAL_KEYS *p_key);


/***************************
**  Security Manager Types
****************************/
/* Structure that applications use to register with BTM_SecRegister */
typedef struct
{
    tBTM_AUTHORIZE_CALLBACK     *p_authorize_callback;
    tBTM_PIN_CALLBACK           *p_pin_callback;
    tBTM_LINK_KEY_CALLBACK      *p_link_key_callback;
    tBTM_LINK_KEY_REQ_CALLBACK  *p_link_key_req_callback;
    tBTM_AUTH_COMPLETE_CALLBACK *p_auth_complete_callback;
    tBTM_ABORT_CALLBACK         *p_abort_callback;
    tBTM_BOND_CANCEL_CMPL_CALLBACK *p_bond_cancel_cmpl_callback;
    tBTM_SP_CALLBACK            *p_sp_callback;
} tBTM_APPL_INFO;

/* Callback function for when a link supervision timeout event occurs.
** This asynchronous event is enabled/disabled by calling BTM_RegForLstoEvt().
*/
typedef void (tBTM_LSTO_CBACK) (BD_ADDR remote_bda, UINT16 timeout);

/*****************************************************************************
**  POWER MANAGEMENT
*****************************************************************************/
/****************************
**  Power Manager Constants
*****************************/
/* BTM Power manager status codes */
enum
{
    BTM_PM_STS_ACTIVE = HCI_MODE_ACTIVE,
    BTM_PM_STS_HOLD   = HCI_MODE_HOLD,
    BTM_PM_STS_SNIFF  = HCI_MODE_SNIFF,
    BTM_PM_STS_PARK   = HCI_MODE_PARK,
    BTM_PM_STS_SSR,     /* report the SSR parameters in HCI_SNIFF_SUB_RATE_EVT */
    BTM_PM_STS_PENDING,   /* when waiting for status from controller */
    BTM_PM_STS_ERROR   /* when HCI command status returns error */
};
typedef UINT8 tBTM_PM_STATUS;

/* BTM Power manager modes */
enum
{
    BTM_PM_MD_ACTIVE = BTM_PM_STS_ACTIVE,
    BTM_PM_MD_HOLD   = BTM_PM_STS_HOLD,
    BTM_PM_MD_SNIFF  = BTM_PM_STS_SNIFF,
    BTM_PM_MD_PARK   = BTM_PM_STS_PARK,
    BTM_PM_MD_FORCE  = 0x10 /* OR this to force ACL link to a certain mode */
};
typedef UINT8 tBTM_PM_MODE;

#define BTM_PM_SET_ONLY_ID  0x80

/* Operation codes */
#define BTM_PM_REG_SET      1 /* The module wants to set the desired power mode */
#define BTM_PM_REG_NOTIF    2 /* The module wants to receive mode change event */
#define BTM_PM_DEREG        4 /* The module does not want to involve with PM anymore */

/************************
**  Power Manager Types
*************************/
typedef struct
{
    UINT16          max;
    UINT16          min;
    UINT16          attempt;
    UINT16          timeout;
    tBTM_PM_MODE    mode;
} tBTM_PM_PWR_MD;

/*************************************
**  Power Manager Callback Functions
**************************************/
typedef void (tBTM_PM_STATUS_CBACK) (BD_ADDR p_bda, tBTM_PM_STATUS status,
                                     UINT16 value, UINT8 hci_status);


/************************
**  Stored Linkkey Types
*************************/
#define BTM_CB_EVT_RETURN_LINK_KEYS         1
#define BTM_CB_EVT_READ_STORED_LINK_KEYS    2
#define BTM_CB_EVT_WRITE_STORED_LINK_KEYS   3
#define BTM_CB_EVT_DELETE_STORED_LINK_KEYS  4

typedef struct
{
    UINT8          event;

} tBTM_STORED_LINK_KEYS_EVT;


typedef struct
{
    UINT8          event;
    UINT8          num_keys;

} tBTM_RETURN_LINK_KEYS_EVT;


typedef struct
{
    BD_ADDR         bd_addr;
    LINK_KEY        link_key;

} tBTM_BD_ADDR_LINK_KEY_PAIR;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT16         max_keys;
    UINT16         read_keys;

} tBTM_READ_STORED_LINK_KEY_COMPLETE;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT8          num_keys;

} tBTM_WRITE_STORED_LINK_KEY_COMPLETE;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT16         num_keys;

} tBTM_DELETE_STORED_LINK_KEY_COMPLETE;


/* These macros are defined to check the Broadcom features supported in controller
 * (the return value for BTM_ReadBrcmFeatures() */
/* multi-av */
#define BTM_FEATURE_MULTI_AV_MASK 0x01
#define BTM_FEATURE_MULTI_AV_OFF  0
#define BTM_VSC_MULTI_AV_SUPPORTED(x) ((x)[BTM_FEATURE_MULTI_AV_OFF] & BTM_FEATURE_MULTI_AV_MASK)

/* WBS SBC codec */
#define BTM_FEATURE_WBS_SBC_MASK 0x02
#define BTM_FEATURE_WBS_SBC_OFF  0
#define BTM_VSC_WBS_SBC_SUPPORTED(x) ((x)[BTM_FEATURE_WBS_SBC_OFF] & BTM_FEATURE_WBS_SBC_MASK)

/* Advanced Audio LC-PLC */
#define BTM_FEATURE_AUDIO_LC_PLC_MASK 0x04
#define BTM_FEATURE_AUDIO_LC_PLC_OFF  0
#define BTM_VSC_AUDIO_LC_PLC_SUPPORTED(x) ((x)[BTM_FEATURE_AUDIO_LC_PLC_OFF] & BTM_FEATURE_AUDIO_LC_PLC_MASK)

/* Light stack for audio routing in Controller */
#define BTM_FEATURE_LIGHT_STACK_MASK 0x08
#define BTM_FEATURE_LIGHT_STACK_OFF  0
#define BTM_VSC_LIGHT_STACK_SUPPORTED(x) ((x)[BTM_FEATURE_LIGHT_STACK_OFF] & BTM_FEATURE_LIGHT_STACK_MASK)

/* NFC support */
#define BTM_FEATURE_NFC_MASK        (HCI_BRCM_FEATURE_NFC_MASK) /* 0x10 */
#define BTM_FEATURE_NFC_OFF         (HCI_BRCM_FEATURE_NFC_OFF)  /* 0    */
#define BTM_VSC_NFC_SUPPORTED(x) ((x)[BTM_FEATURE_NFC_OFF] & BTM_FEATURE_NFC_MASK)


/************************
**  Dual-Stack support
*************************/
/* BTM_SYNC_FAIL_EVT reason codes */
#define BTM_SYNC_SUCCESS                    0
#define BTM_SYNC_FAIL_BTE_SWITCH_REJECTED   1
#define BTM_SYNC_FAIL_TRANS_PAUSE           2
#define BTM_SYNC_FAIL_CORE_SYNC             3
#define BTM_SYNC_FAIL_BTA_SYNC              4
#define BTM_SYNC_FAIL_TRANS_RESUME          5
#define BTM_SYNC_FAIL_RESYNC                6
#define BTM_SYNC_FAIL_ERROR                 7
#define BTM_SYNC_FAIL_UIPC_OPEN             8
typedef UINT8 tBTM_SYNC_STATUS;

/* Direction of sync (used by BTM_SyncStack) */
#define BTM_SW_BB_TO_MM     0
#define BTM_SW_TO_BB        1   /* Switch back to baseband stack (from either MM or BTC host) */
#define BTM_SW_RESYNC       2
#define BTM_SW_BB_TO_BTC    3   /* Switch from baseband stack to Bluetooth Controller Host stack */
#define BTM_SW_MM_TO_BB     4
#define BTM_SW_BTC_TO_BB    5
typedef UINT8 tBTM_SW_DIR;

/* Stack synchronization events (returned by tBTM_SYNC_STACK_CBACK callback) */
#define BTM_SYNC_CPLT_EVT   0
#define BTM_SYNC_BTA_EVT    1
#define BTM_RESYNC_CPLT_EVT 2
#define BTM_UIPC_OPENED_EVT 3
#define BTM_UIPC_CLOSED_EVT 4
typedef UINT8 tBTM_SYNC_STACK_EVT;

/* Synchronization info from BTA/application that will be sent when calling BTE sync request functions */
typedef struct
{
    tBTM_SW_DIR dir;
    UINT16      lcid[BTM_SYNC_INFO_NUM_STR];
    UINT8       avdt_handle[BTM_SYNC_INFO_NUM_STR];
} tBTM_SYNC_INFO;

/* Stack synchonization callback function
** Parameters are
**              event:  stack synchronization event
**              status: BTM_SUCCESS if event was successful
*/
typedef void (*tBTM_SYNC_STACK_CBACK)(tBTM_SYNC_STACK_EVT event, tBTM_SYNC_STATUS status);


/* Sync complete callback function. Called by bte layers after synchronization is complete
** so that BTM_SYNC can procede with the next step for switching stack to MM
**
** Parameters are
**              status: BTM_SUCCESS if synchronization was successful
*/
typedef void (*tBTM_SYNC_CPLT_CBACK)(tBTM_STATUS status);



/* IPC event callback function. Called by BTM when an IPC event is received.
** These events are currently sent to DM through the callback function.
**
** Parameters are
**              status: BTM_SUCCESS if synchronization was successful
**              p_data: Actual message in the IPC
*/
typedef void (tBTM_IPC_EVT_CBACK)(tBTM_STATUS status, BT_HDR *p_data);

/* MIP evnets, callbacks    */
enum
{
    BTM_MIP_MODE_CHG_EVT,
    BTM_MIP_DISCONNECT_EVT,
    BTM_MIP_PKTS_COMPL_EVT,
    BTM_MIP_RXDATA_EVT
};
typedef UINT8 tBTM_MIP_EVT;

typedef struct
{
    tBTM_MIP_EVT    event;
    BD_ADDR         bd_addr;
    UINT16          mip_id;
} tBTM_MIP_MODE_CHANGE;

typedef struct
{
    tBTM_MIP_EVT    event;
    UINT16          mip_id;
    UINT8           disc_reason;
} tBTM_MIP_CONN_TIMEOUT;

#define BTM_MIP_MAX_RX_LEN  17

typedef struct
{
    tBTM_MIP_EVT    event;
    UINT16          mip_id;
    UINT8           rx_len;
    UINT8           rx_data[BTM_MIP_MAX_RX_LEN];
} tBTM_MIP_RXDATA;

typedef struct
{
    tBTM_MIP_EVT    event;
    BD_ADDR         bd_addr;
    UINT8           data[11];       /* data[0] shows Vender-specific device type */
} tBTM_MIP_EIR_HANDSHAKE;

typedef struct
{
    tBTM_MIP_EVT    event;
    UINT16          num_sent;       /* Number of packets completed at the controller */
} tBTM_MIP_PKTS_COMPL;

typedef union
{
    tBTM_MIP_EVT            event;
    tBTM_MIP_MODE_CHANGE    mod_chg;
    tBTM_MIP_CONN_TIMEOUT   conn_tmo;
    tBTM_MIP_EIR_HANDSHAKE  eir;
    tBTM_MIP_PKTS_COMPL     completed;
    tBTM_MIP_RXDATA         rxdata;
} tBTM_MIP_EVENT_DATA;

/* MIP event callback function  */
typedef void (tBTM_MIP_EVENTS_CB) (tBTM_MIP_EVT event, tBTM_MIP_EVENT_DATA data);

/* MIP Device query callback function  */
typedef BOOLEAN (tBTM_MIP_QUERY_CB) (BD_ADDR dev_addr, UINT8 *p_mode, LINK_KEY link_key);

/*****************************************************************************
**  EXTERNAL FUNCTION DECLARATIONS
*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**  DEVICE CONTROL and COMMON FUNCTIONS
*****************************************************************************/

/*******************************************************************************
**
** Function         BTM_SetAfhChannels
**
** Description      This function is called to disable channels
**
** Returns          status
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetAfhChannels (UINT8 first, UINT8 last);

/*******************************************************************************
**
** Function         BTM_SetAfhChannelAssessment
**
** Description      This function is called to set the channel assessment mode on or off
**
** Returns          status
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetAfhChannelAssessment (BOOLEAN enable_or_disable);

/*******************************************************************************
**
** Function         BTM_DeviceReset
**
** Description      This function is called to reset the controller.The Callback function
**                  if provided is called when startup of the device has
**                  completed.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_DeviceReset (tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_IsDeviceUp
**
** Description      This function is called to check if the device is up.
**
** Returns          TRUE if device is up, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_IsDeviceUp (void);


/*******************************************************************************
**
** Function         BTM_SetLocalDeviceName
**
** Description      This function is called to set the local device name.
**
** Returns          BTM_CMD_STARTED if successful, otherwise an error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetLocalDeviceName (char *p_name);

/*******************************************************************************
**
** Function         BTM_SetDeviceClass
**
** Description      This function is called to set the local device class
**
** Returns          BTM_SUCCESS if successful, otherwise an error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_SetDeviceClass (DEV_CLASS dev_class);


/*******************************************************************************
**
** Function         BTM_ReadLocalDeviceName
**
** Description      This function is called to read the local device name.
**
** Returns          status of the operation
**                  If success, BTM_SUCCESS is returned and p_name points stored
**                              local device name
**                  If BTM doesn't store local device name, BTM_NO_RESOURCES is
**                              is returned and p_name is set to NULL
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadLocalDeviceName (char **p_name);

/*******************************************************************************
**
** Function         BTM_ReadLocalDeviceNameFromController
**
** Description      Get local device name from controller. Do not use cached
**                  name (used to get chip-id prior to btm reset complete).
**
** Returns          BTM_CMD_STARTED if successful, otherwise an error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadLocalDeviceNameFromController (tBTM_CMPL_CB *p_rln_cmpl_cback);

/*******************************************************************************
**
** Function         BTM_ReadLocalVersion
**
** Description      This function is called to read the local device version
**
** Returns          BTM_SUCCESS if successful, otherwise an error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadLocalVersion (tBTM_VERSION_INFO *p_vers);


/*******************************************************************************
**
** Function         BTM_ReadLocalDeviceAddr
**
** Description      This function is called to read the local device address
**
** Returns          BTM_SUCCESS
**                  Callback returns the local device address
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadLocalDeviceAddr (tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_GetLocalDeviceAddr
**
** Description      This function is called to read the local device address
**
** Returns          void
**                  the local device address is copied into bd_addr
**
*******************************************************************************/
    BTM_API extern void BTM_GetLocalDeviceAddr (BD_ADDR bd_addr);


/*******************************************************************************
**
** Function         BTM_ReadDeviceClass
**
** Description      This function is called to read the local device class
**
** Returns          pointer to the device class
**
*******************************************************************************/
    BTM_API extern UINT8 *BTM_ReadDeviceClass (void);


/*******************************************************************************
**
** Function         BTM_ReadLocalFeatures
**
** Description      This function is called to read the local features
**
** Returns          pointer to the local features string
**
*******************************************************************************/
    BTM_API extern UINT8 *BTM_ReadLocalFeatures (void);

/*******************************************************************************
**
** Function         BTM_ReadBrcmFeatures
**
** Description      This function is called to read the Broadcom specific features
**
** Returns          pointer to the Broadcom features string
**
*******************************************************************************/
    BTM_API extern UINT8 *BTM_ReadBrcmFeatures (void);

/*******************************************************************************
**
** Function         BTM_RegisterForDeviceStatusNotif
**
** Description      This function is called to register for device status
**                  change notifications.
**
** Returns          pointer to previous caller's callback function or NULL if first
**                  registration.
**
*******************************************************************************/
    BTM_API extern tBTM_DEV_STATUS_CB *BTM_RegisterForDeviceStatusNotif (tBTM_DEV_STATUS_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_RegisterForVSEvents
**
** Description      This function is called to register/deregister for vendor
**                  specific HCI events.
**
**                  If is_register=TRUE, then the function will be registered;
**                  if is_register=FALSE, then the function will be deregistered.
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_BUSY if maximum number of callbacks have already been
**                           registered.
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_RegisterForVSEvents (tBTM_VS_EVT_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ContinueReset
**
** Description      Instructs stack to continue its stack initialization after
**                  an application has completed any vender specific commands
**                  sent to the controller.
**
**                  Note: This function is only called if an application
**                      initialization function has been inserted in the reset
**                      sequence.  (BTM_APP_DEV_INIT is defined with a function).
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_ContinueReset (void);


/*******************************************************************************
**
** Function         BTM_VendorSpecificCommand
**
** Description      Send a vendor specific HCI command to the controller.
**
** Returns
**      BTM_SUCCESS         Command sent. Does not expect command complete
**                              event. (command cmpl callback param is NULL)
**      BTM_CMD_STARTED     Command sent. Waiting for command cmpl event.
**      BTM_BUSY            Command not sent. Waiting for cmd cmpl event for
**                              prior command.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_VendorSpecificCommand(UINT16 opcode,
                                                         UINT8 param_len,
                                                         UINT8 *p_param_buf,
                                                         tBTM_VSC_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_AllocateSCN
**
** Description      Look through the Server Channel Numbers for a free one to be
**                  used with an RFCOMM connection.
**
** Returns          Allocated SCN number or 0 if none.
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_AllocateSCN(void);

// btla-specific ++
/*******************************************************************************
**
** Function         BTM_TryAllocateSCN
**
** Description      Try to allocate a fixed server channel
**
** Returns          Returns TRUE if server channel was available
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_TryAllocateSCN(UINT8 scn);
// btla-specific --


/*******************************************************************************
**
** Function         BTM_FreeSCN
**
** Description      Free the specified SCN.
**
** Returns          TRUE if successful, FALSE if SCN is not in use or invalid
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_FreeSCN(UINT8 scn);


/*******************************************************************************
**
** Function         BTM_SetTraceLevel
**
** Description      This function sets the trace level for BTM.  If called with
**                  a value of 0xFF, it simply returns the current trace level.
**
** Returns          The new or current trace level
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_SetTraceLevel (UINT8 new_level);


/*******************************************************************************
**
** Function         BTM_WritePageTimeout
**
** Description      Send HCI Wite Page Timeout.
**
** Returns
**      BTM_SUCCESS         Command sent.
**      BTM_NO_RESOURCES    If out of resources to send the command.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_WritePageTimeout(UINT16 timeout);

/*******************************************************************************
**
** Function         BTM_WriteVoiceSettings
**
** Description      Send HCI Write Voice Settings command.
**                  See hcidefs.h for settings bitmask values.
**
** Returns
**      BTM_SUCCESS         Command sent.
**      BTM_NO_RESOURCES     If out of resources to send the command.
**
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_WriteVoiceSettings(UINT16 settings);

/*******************************************************************************
**
** Function         BTM_EnableTestMode
**
** Description      Send HCI the enable device under test command.
**
**                  Note: Controller can only be taken out of this mode by
**                      resetting the controller.
**
** Returns
**      BTM_SUCCESS         Command sent.
**      BTM_NO_RESOURCES    If out of resources to send the command.
**
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_EnableTestMode(void);


/*****************************************************************************
**  DEVICE DISCOVERY FUNCTIONS - Inquiry, Remote Name, Discovery, Class of Device
*****************************************************************************/

/*******************************************************************************
**
** Function         BTM_SetDiscoverability
**
** Description      This function is called to set the device into or out of
**                  discoverable mode. Discoverable mode means inquiry
**                  scans are enabled.  If a value of '0' is entered for window or
**                  interval, the default values are used.
**
** Returns          BTM_SUCCESS if successful
**                  BTM_BUSY if a setting of the filter is already in progress
**                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
**                  BTM_ILLEGAL_VALUE if a bad parameter was detected
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_SetDiscoverability (UINT16 inq_mode, UINT16 window,
                                                        UINT16 interval);


/*******************************************************************************
**
** Function         BTM_ReadDiscoverability
**
** Description      This function is called to read the current discoverability
**                  mode of the device.
**
** Output Params:   p_window - current inquiry scan duration
**                  p_interval - current inquiry scan interval
**
** Returns          BTM_NON_DISCOVERABLE, BTM_LIMITED_DISCOVERABLE, or
**                  BTM_GENERAL_DISCOVERABLE
**
*******************************************************************************/
    BTM_API extern UINT16       BTM_ReadDiscoverability (UINT16 *p_window,
                                                         UINT16 *p_interval);


/*******************************************************************************
**
** Function         BTM_SetPeriodicInquiryMode
**
** Description      This function is called to set the device periodic inquiry mode.
**                  If the duration is zero, the periodic inquiry mode is cancelled.
**
** Parameters:      p_inqparms - pointer to the inquiry information
**                      mode - GENERAL or LIMITED inquiry
**                      duration - length in 1.28 sec intervals (If '0', the inquiry is CANCELLED)
**                      max_resps - maximum amount of devices to search for before ending the inquiry
**                      filter_cond_type - BTM_CLR_INQUIRY_FILTER, BTM_FILTER_COND_DEVICE_CLASS, or
**                                         BTM_FILTER_COND_BD_ADDR
**                      filter_cond - value for the filter (based on filter_cond_type)
**
**                  max_delay - maximum amount of time between successive inquiries
**                  min_delay - minimum amount of time between successive inquiries
**                  p_results_cb - callback returning pointer to results (tBTM_INQ_RESULTS)
**
** Returns          BTM_CMD_STARTED if successfully started
**                  BTM_ILLEGAL_VALUE if a bad parameter is detected
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_SUCCESS - if cancelling the periodic inquiry
**                  BTM_BUSY - if an inquiry is already active
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_SetPeriodicInquiryMode (tBTM_INQ_PARMS *p_inqparms,
                                                            UINT16 max_delay, UINT16 min_delay,
                                                            tBTM_INQ_RESULTS_CB *p_results_cb);


/*******************************************************************************
**
** Function         BTM_StartInquiry
**
** Description      This function is called to start an inquiry.
**
** Parameters:      p_inqparms - pointer to the inquiry information
**                      mode - GENERAL or LIMITED inquiry
**                      duration - length in 1.28 sec intervals (If '0', the inquiry is CANCELLED)
**                      max_resps - maximum amount of devices to search for before ending the inquiry
**                      filter_cond_type - BTM_CLR_INQUIRY_FILTER, BTM_FILTER_COND_DEVICE_CLASS, or
**                                         BTM_FILTER_COND_BD_ADDR
**                      filter_cond - value for the filter (based on filter_cond_type)
**
**                  p_results_cb   - Pointer to the callback routine which gets called
**                                upon receipt of an inquiry result. If this field is
**                                NULL, the application is not notified.
**
**                  p_cmpl_cb   - Pointer to the callback routine which gets called
**                                upon completion.  If this field is NULL, the
**                                application is not notified when completed.
** Returns          tBTM_STATUS
**                  BTM_CMD_STARTED if successfully initiated
**                  BTM_BUSY if already in progress
**                  BTM_ILLEGAL_VALUE if parameter(s) are out of range
**                  BTM_NO_RESOURCES if could not allocate resources to start the command
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_StartInquiry (tBTM_INQ_PARMS *p_inqparms,
                                                  tBTM_INQ_RESULTS_CB *p_results_cb,
                                                  tBTM_CMPL_CB *p_cmpl_cb);


/*******************************************************************************
**
** Function         BTM_IsInquiryActive
**
** Description      This function returns a bit mask of the current inquiry state
**
** Returns          BTM_INQUIRY_INACTIVE if inactive (0)
**                  BTM_LIMITED_INQUIRY_ACTIVE if a limted inquiry is active
**                  BTM_GENERAL_INQUIRY_ACTIVE if a general inquiry is active
**                  BTM_PERIODIC_INQUIRY_ACTIVE if a periodic inquiry is active
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_IsInquiryActive (void);


/*******************************************************************************
**
** Function         BTM_CancelInquiry
**
** Description      This function cancels an inquiry if active
**
** Returns          BTM_SUCCESS if successful
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_CancelInquiry(void);


/*******************************************************************************
**
** Function         BTM_CancelPeriodicInquiry
**
** Description      This function cancels a periodic inquiry
**
** Returns
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_SUCCESS - if cancelling the periodic inquiry
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_CancelPeriodicInquiry(void);


/*******************************************************************************
**
** Function         BTM_SetInquiryFilterCallback
**
** Description      Host can register to be asked whenever an inquiry result
**                  is received.  If host does not like the device no name
**                  request is issued for the device
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetInquiryFilterCallback(tBTM_FILTER_CB *p_callback);


/*******************************************************************************
**
** Function         BTM_SetConnectability
**
** Description      This function is called to set the device into or out of
**                  connectable mode. Discoverable mode means page scans enabled.
**
** Returns          BTM_SUCCESS if successful
**                  BTM_ILLEGAL_VALUE if a bad parameter is detected
**                  BTM_NO_RESOURCES if could not allocate a message buffer
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetConnectability (UINT16 page_mode, UINT16 window,
                                                      UINT16 interval);


/*******************************************************************************
**
** Function         BTM_ReadConnectability
**
** Description      This function is called to read the current discoverability
**                  mode of the device.
** Output Params    p_window - current page scan duration
**                  p_interval - current time between page scans
**
** Returns          BTM_NON_CONNECTABLE or BTM_CONNECTABLE
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_ReadConnectability (UINT16 *p_window, UINT16 *p_interval);


/*******************************************************************************
**
** Function         BTM_SetInquiryMode
**
** Description      This function is called to set standard, with RSSI
**                  mode or extended of the inquiry for local device.
**
** Input Params:    BTM_INQ_RESULT_STANDARD, BTM_INQ_RESULT_WITH_RSSI or
**                  BTM_INQ_RESULT_EXTENDED
**
** Returns          BTM_SUCCESS if successful
**                  BTM_NO_RESOURCES if couldn't get a memory pool buffer
**                  BTM_ILLEGAL_VALUE if a bad parameter was detected
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BT_API extern tBTM_STATUS  BTM_SetInquiryMode (UINT8 mode);

/*******************************************************************************
**
** Function         BTM_SetInquiryScanType
**
** Description      This function is called to set the iquiry scan-type to
**                  standard or interlaced.
**
** Input Params:    BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED
**
** Returns          BTM_SUCCESS if successful
**                  BTM_MODE_UNSUPPORTED if not a 1.2 device
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BT_API extern tBTM_STATUS BTM_SetInquiryScanType (UINT16 scan_type);

/*******************************************************************************
**
** Function         BTM_SetPageScanType
**
** Description      This function is called to set the page scan-type to
**                  standard or interlaced.
**
** Input Params:    BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED
**
** Returns          BTM_SUCCESS if successful
**                  BTM_MODE_UNSUPPORTED if not a 1.2 device
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/

    BT_API extern tBTM_STATUS BTM_SetPageScanType (UINT16 scan_type);

/*******************************************************************************
**
** Function         BTM_ReadRemoteDeviceName
**
** Description      This function initiates a remote device HCI command to the
**                  controller and calls the callback when the process has completed.
**
** Input Params:    remote_bda      - device address of name to retrieve
**                  p_cb            - callback function called when BTM_CMD_STARTED
**                                    is returned.
**                                    A pointer to tBTM_REMOTE_DEV_NAME is passed to the
**                                    callback.
**
** Returns
**                  BTM_CMD_STARTED is returned if the request was successfully sent
**                                  to HCI.
**                  BTM_BUSY if already in progress
**                  BTM_UNKNOWN_ADDR if device address is bad
**                  BTM_NO_RESOURCES if could not allocate resources to start the command
**                  BTM_WRONG_MODE if the device is not up.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_ReadRemoteDeviceName (BD_ADDR remote_bda,
                                                          tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_CancelRemoteDeviceName
**
** Description      This function initiates the cancel request for the specified
**                  remote device.
**
** Input Params:    None
**
** Returns
**                  BTM_CMD_STARTED is returned if the request was successfully sent
**                                  to HCI.
**                  BTM_NO_RESOURCES if could not allocate resources to start the command
**                  BTM_WRONG_MODE if there is not an active remote name request.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_CancelRemoteDeviceName (void);

/*******************************************************************************
**
** Function         BTM_ReadRemoteVersion
**
** Description      This function is called to read a remote device's version
**
** Returns          BTM_SUCCESS if successful, otherwise an error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadRemoteVersion (BD_ADDR addr,
                                                      UINT8 *lmp_version,
                                                      UINT16 *manufacturer,
                                                      UINT16 *lmp_sub_version);

/*******************************************************************************
**
** Function         BTM_ReadRemoteFeatures
**
** Description      This function is called to read a remote device's features
**
** Returns          pointer to the features string
**
*******************************************************************************/
    BTM_API extern UINT8 *BTM_ReadRemoteFeatures (BD_ADDR addr);

/*******************************************************************************
**
** Function         BTM_InqFirstResult
**
** Description      This function looks through the inquiry database for the first
**                  used entrysince the LAST inquiry. This is used in conjunction
**                  with BTM_InqNext by applications as a way to walk through the
**                  inquiry results database.
**
** Returns          pointer to first in-use entry, or NULL if DB is empty
**
*******************************************************************************/
    BTM_API extern tBTM_INQ_INFO *BTM_InqFirstResult (void);


/*******************************************************************************
**
** Function         BTM_InqNextResult
**
** Description      This function looks through the inquiry database for the next
**                  used entrysince the LAST inquiry. If the input parameter is NULL,
**                  the first entry is returned.
**
** Returns          pointer to next in-use entry, or NULL if no more found.
**
*******************************************************************************/
    BTM_API extern tBTM_INQ_INFO *BTM_InqNextResult (tBTM_INQ_INFO *p_cur);


/*******************************************************************************
**
** Function         BTM_InqDbRead
**
** Description      This function looks through the inquiry database for a match
**                  based on Bluetooth Device Address. This is the application's
**                  interface to get the inquiry details of a specific BD address.
**
** Returns          pointer to entry, or NULL if not found
**
*******************************************************************************/
    BTM_API extern tBTM_INQ_INFO *BTM_InqDbRead (BD_ADDR p_bda);


/*******************************************************************************
**
** Function         BTM_InqDbFirst
**
** Description      This function looks through the inquiry database for the first
**                  used entry, and returns that. This is used in conjunction with
**                  BTM_InqDbNext by applications as a way to walk through the
**                  inquiry database.
**
** Returns          pointer to first in-use entry, or NULL if DB is empty
**
*******************************************************************************/
    BTM_API extern tBTM_INQ_INFO *BTM_InqDbFirst (void);


/*******************************************************************************
**
** Function         BTM_InqDbNext
**
** Description      This function looks through the inquiry database for the next
**                  used entry, and returns that.  If the input parameter is NULL,
**                  the first entry is returned.
**
** Returns          pointer to next in-use entry, or NULL if no more found.
**
*******************************************************************************/
    BTM_API extern tBTM_INQ_INFO *BTM_InqDbNext (tBTM_INQ_INFO *p_cur);


/*******************************************************************************
**
** Function         BTM_ClearInqDb
**
** Description      This function is called to clear out a device or all devices
**                  from the inquiry database.
**
** Parameter        p_bda - (input) BD_ADDR ->  Address of device to clear
**                                              (NULL clears all entries)
**
** Returns          BTM_BUSY if an inquiry, get remote name, or event filter
**                          is active, otherwise BTM_SUCCESS
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_ClearInqDb (BD_ADDR p_bda);


/*******************************************************************************
**
** Function         BTM_ReadNumInqDbEntries
**
** Returns          This function returns the number of entries in the inquiry database.
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_ReadNumInqDbEntries (void);


/*******************************************************************************
**
** Function         BTM_InquiryRegisterForChanges
**
** Description      This function is called to register a callback for when the
**                  inquiry database changes, i.e. new entry or entry deleted.
**
** Returns          BTM_SUCCESS if successful, otherwise error code
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_InquiryRegisterForChanges (tBTM_INQ_DB_CHANGE_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ReadInquiryRspTxPower
**
** Description      This command will read the inquiry Transmit Power level used
**                  to transmit the FHS and EIR data packets.
**                  This can be used directly in the Tx Power Level EIR data type.
**
** Returns          BTM_SUCCESS if successful
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadInquiryRspTxPower (tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_WriteInquiryTxPower
**
** Description      This command is used to write the inquiry transmit power level
**                  used to transmit the inquiry (ID) data packets. The Controller
**                  should use the supported TX power level closest to the Tx_Power
**                  parameter.
**
** Returns          BTM_SUCCESS if successful
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS  BTM_WriteInquiryTxPower (INT8 tx_power);

/*******************************************************************************
**
** Function         BTM_StartDiscovery
**
** Description      This function is called by an application (or profile)
**                  when it wants to trigger an service discovery using the
**                  BTM's discovery database.
**
** Returns          tBTM_STATUS
**                      BTM_CMD_STARTED if the discovery was initiated
**                      BTM_BUSY if one is already in progress
**                      BTM_UNKNOWN_ADDR if no addresses are in the INQ DB
**                      BTM_ERR_PROCESSING if err initiating the command
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_StartDiscovery (tBTM_CMPL_CB *p_cmpl_cb,
                                                   BD_ADDR_PTR p_rem_addr);


/*******************************************************************************
**
** Function         BTM_FindAttribute
**
** Description      This function is called by an application (or profile)
**                  when it wants to see if an attribute exists in the BTM
**                  discovery database.
**
** Returns          Pointer to matching record, or NULL
**
*******************************************************************************/
    BTM_API extern tSDP_DISC_REC *BTM_FindAttribute (UINT16 attr_id,
                                                     tSDP_DISC_REC *p_start_rec);


/*******************************************************************************
**
** Function         BTM_FindService
**
** Description      This function is called by an application (or profile)
**                  when it wants to see if a service exists in the BTM
**                  discovery database.
**
** Returns          Pointer to matching record, or NULL
**
*******************************************************************************/
    BTM_API extern tSDP_DISC_REC *BTM_FindService (UINT16 service_uuid,
                                                   tSDP_DISC_REC *p_start_rec);


/*******************************************************************************
**
** Function         BTM_SetDiscoveryParams
**
** Description      This function is called to set the BTM default discovery parameters.
**                  These UUID and attribute filters are used during the call to
**                  BTM_StartDiscovery.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetDiscoveryParams (UINT16 num_uuid, tSDP_UUID *p_uuid_list,
                                                UINT16 num_attr, UINT16 *p_attr_list);


/*****************************************************************************
**  ACL CHANNEL MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_SetLinkPolicy
**
** Description      Create and send HCI "Write Policy Set" command
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetLinkPolicy (BD_ADDR remote_bda,
                                                  UINT16 *settings);


/*******************************************************************************
**
** Function         BTM_ReadLinkPolicy
**
** Description      This function is called to read the link policy settings.
**                  The address of link policy results are returned in the callback.
**                  (tBTM_LNK_POLICY_RESULTS)
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadLinkPolicy (BD_ADDR remote_bda,
                                                   tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_SetDefaultLinkPolicy
**
** Description      Set the default value for HCI "Write Policy Set" command
**                  to use when an ACL link is created.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetDefaultLinkPolicy (UINT16 settings);


/*******************************************************************************
**
** Function         BTM_SetDefaultLinkSuperTout
**
** Description      Set the default value for HCI "Write Link Supervision Timeout"
**                  command to use when an ACL link is created.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetDefaultLinkSuperTout (UINT16 timeout);


/*******************************************************************************
**
** Function         BTM_SetLinkSuperTout
**
** Description      Create and send HCI "Write Link Supervision Timeout" command
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetLinkSuperTout (BD_ADDR remote_bda,
                                                     UINT16 timeout);

/*******************************************************************************
**
** Function         BTM_RegForLstoEvt
**
** Description      register for the HCI "Link Supervision Timeout Change" event
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_RegForLstoEvt (tBTM_LSTO_CBACK *p_cback);


/*******************************************************************************
**
** Function         BTM_SetPacketTypes
**
** Description      This function is set the packet types used for a specific
**                  ACL connection,
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetPacketTypes (BD_ADDR remote_bda, UINT16 pkt_types);


/*******************************************************************************
**
** Function         BTM_ReadPacketTypes
**
** Description      This function is set the packet types used for the specified
**                  ACL connection,
**
** Returns          packet types supported for the connection, or 0 if no BD address
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_ReadPacketTypes (BD_ADDR remote_bda);


/*******************************************************************************
**
** Function         BTM_IsAclConnectionUp
**
** Description      This function is called to check if an ACL connection exists
**                  to a specific remote BD Address.
**
** Returns          TRUE if connection is up, else FALSE.
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_IsAclConnectionUp (BD_ADDR remote_bda);


/*******************************************************************************
**
** Function         BTM_GetRole
**
** Description      This function is called to get the role of the local device
**                  for the ACL connection with the specified remote device
**
** Returns          BTM_SUCCESS if connection exists.
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_GetRole (BD_ADDR remote_bd_addr, UINT8 *p_role);



/*******************************************************************************
**
** Function         BTM_SwitchRole
**
** Description      This function is called to switch role between master and
**                  slave.  If role is already set it will do nothing.  If the
**                  command was initiated, the callback function is called upon
**                  completion.
**
** Returns          BTM_SUCCESS if already in specified role.
**                  BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_MODE_UNSUPPORTED if local device does not support role switching
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SwitchRole (BD_ADDR remote_bd_addr,
                                               UINT8 new_role,
                                               tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ChangeLinkKey
**
** Description      This function is called to change the link key of the
**                  connection.
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if the previous command is not completed
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ChangeLinkKey (BD_ADDR remote_bd_addr,
                                                  tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ReadRSSI
**
** Description      This function is called to read the link policy settings.
**                  The address of link policy results are returned in the callback.
**                  (tBTM_RSSI_RESULTS)
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if command is already in progress
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadRSSI (BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_ReadTxPower
**
** Description      This function is called to read the current connection
**                  TX power of the connection. The TX power level results
**                  are returned in the callback.
**                  (tBTM_RSSI_RESULTS)
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if command is already in progress
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadTxPower (BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_ReadLinkQuality
**
** Description      This function is called to read the link quality.
**                  The value of the link quality is returned in the callback.
**                  (tBTM_LINK_QUALITY_RESULTS)
**
** Returns          BTM_CMD_STARTED if command issued to controller.
**                  BTM_NO_RESOURCES if couldn't allocate memory to issue command
**                  BTM_UNKNOWN_ADDR if no active link with bd addr specified
**                  BTM_BUSY if command is already in progress
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadLinkQuality (BD_ADDR remote_bda, tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_RegBusyLevelNotif
**
** Description      This function is called to register a callback to receive
**                  busy level change events.
**
** Returns          BTM_SUCCESS if successfully registered, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_RegBusyLevelNotif (tBTM_BL_CHANGE_CB *p_cb, UINT8 *p_level,
                                                      tBTM_BL_EVENT_MASK evt_mask);

/*******************************************************************************
**
** Function         BTM_AclRegisterForChanges
**
** Description      This function is called to register a callback to receive
**                  ACL database change events, i.e. new connection or removed.
**
** Returns          BTM_SUCCESS if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_AclRegisterForChanges (tBTM_ACL_DB_CHANGE_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_GetNumAclLinks
**
** Description      This function is called to count the number of
**                  ACL links that are active.
**
** Returns          UINT16  Number of active ACL links
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_GetNumAclLinks (void);


/*******************************************************************************
**
** Function         BTM_ReadClockOffset
**
** Description      This returns the clock offset for a specific
**                  ACL connection.
**
** Returns          clock-offset or 0 if unknown
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_ReadClockOffset (BD_ADDR remote_bda);


/*******************************************************************************
**
** Function         BTM_SetQoS
**
** Description      This function is called to setup QoS
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetQoS(BD_ADDR bd, FLOW_SPEC *p_flow,
                                          tBTM_CMPL_CB *p_cb);


/*****************************************************************************
**  (e)SCO CHANNEL MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_CreateSco
**
** Description      This function is called to create an SCO connection. If the
**                  "is_orig" flag is TRUE, the connection will be originated,
**                  otherwise BTM will wait for the other side to connect.
**
** Returns          BTM_UNKNOWN_ADDR if the ACL connection is not up
**                  BTM_BUSY         if another SCO being set up to
**                                   the same BD address
**                  BTM_NO_RESOURCES if the max SCO limit has been reached
**                  BTM_CMD_STARTED  if the connection establishment is started.
**                                   In this case, "*p_sco_inx" is filled in
**                                   with the sco index used for the connection.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_CreateSco (BD_ADDR remote_bda, BOOLEAN is_orig,
                                              UINT16 pkt_types, UINT16 *p_sco_inx,
                                              tBTM_SCO_CB *p_conn_cb,
                                              tBTM_SCO_CB *p_disc_cb);


/*******************************************************************************
**
** Function         BTM_RemoveSco
**
** Description      This function is called to remove a specific SCO connection.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_RemoveSco (UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_SetScoPacketTypes
**
** Description      This function is called to set the packet types used for
**                  a specific SCO connection,
**
** Parameters       pkt_types - One or more of the following
**                  BTM_SCO_PKT_TYPES_MASK_HV1
**                  BTM_SCO_PKT_TYPES_MASK_HV2
**                  BTM_SCO_PKT_TYPES_MASK_HV3
**                  BTM_SCO_PKT_TYPES_MASK_EV3
**                  BTM_SCO_PKT_TYPES_MASK_EV4
**                  BTM_SCO_PKT_TYPES_MASK_EV5
**
**                  BTM_SCO_LINK_ALL_MASK   - enables all supported types
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetScoPacketTypes (UINT16 sco_inx, UINT16 pkt_types);


/*******************************************************************************
**
** Function         BTM_ReadScoPacketTypes
**
** Description      This function is read the packet types used for a specific
**                  SCO connection.
**
** Returns       One or more of the following (bitmask)
**                  BTM_SCO_PKT_TYPES_MASK_HV1
**                  BTM_SCO_PKT_TYPES_MASK_HV2
**                  BTM_SCO_PKT_TYPES_MASK_HV3
**                  BTM_SCO_PKT_TYPES_MASK_EV3
**                  BTM_SCO_PKT_TYPES_MASK_EV4
**                  BTM_SCO_PKT_TYPES_MASK_EV5
**
** Returns          packet types supported for the connection
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_ReadScoPacketTypes (UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_ReadDeviceScoPacketTypes
**
** Description      This function is read the SCO packet types that
**                  the device supports.
**
** Returns          packet types supported by the device.
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_ReadDeviceScoPacketTypes (void);


/*******************************************************************************
**
** Function         BTM_ReadScoHandle
**
** Description      This function is used to read the HCI handle used for a specific
**                  SCO connection,
**
** Returns          handle for the connection, or 0xFFFF if invalid SCO index.
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_ReadScoHandle (UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_ReadScoBdAddr
**
** Description      This function is read the remote BD Address for a specific
**                  SCO connection,
**
** Returns          pointer to BD address or NULL if not known
**
*******************************************************************************/
    BTM_API extern UINT8 *BTM_ReadScoBdAddr (UINT16 sco_inx);


/*******************************************************************************
**
** Function         BTM_ReadScoDiscReason
**
** Description      This function is returns the reason why an (e)SCO connection
**                  has been removed. It contains the value until read, or until
**                  another (e)SCO connection has disconnected.
**
** Returns          HCI reason or BTM_INVALID_SCO_DISC_REASON if not set.
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_ReadScoDiscReason (void);


/*******************************************************************************
**
** Function         BTM_SetEScoMode
**
** Description      This function sets up the negotiated parameters for SCO or
**                  eSCO, and sets as the default mode used for calls to
**                  BTM_CreateSco.  It can be called only when there are no
**                  active (e)SCO links.
**
** Returns          BTM_SUCCESS if the successful.
**                  BTM_BUSY if there are one or more active (e)SCO links.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetEScoMode (tBTM_SCO_TYPE sco_mode,
                                                tBTM_ESCO_PARAMS *p_parms);

/*******************************************************************************
**
** Function         BTM_SetWBSCodec
**
** Description      This function sends command to the controller to setup
**                  WBS codec for the upcoming eSCO connection.
**
** Returns          BTM_SUCCESS.
**
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetWBSCodec (tBTM_SCO_CODEC_TYPE codec_type);

/*******************************************************************************
**
** Function         BTM_RegForEScoEvts
**
** Description      This function registers a SCO event callback with the
**                  specified instance.  It should be used to received
**                  connection indication events and change of link parameter
**                  events.
**
** Returns          BTM_SUCCESS if the successful.
**                  BTM_ILLEGAL_VALUE if there is an illegal sco_inx
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_RegForEScoEvts (UINT16 sco_inx,
                                                   tBTM_ESCO_CBACK *p_esco_cback);

/*******************************************************************************
**
** Function         BTM_ReadEScoLinkParms
**
** Description      This function returns the current eSCO link parameters for
**                  the specified handle.  This can be called anytime a connection
**                  is active, but is typically called after receiving the SCO
**                  opened callback.
**
**
** Returns          BTM_SUCCESS if returned data is valid connection.
**                  BTM_ILLEGAL_VALUE if no connection for specified sco_inx.
**                  BTM_MODE_UNSUPPORTED if local controller does not support
**                      1.2 specification.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadEScoLinkParms (UINT16 sco_inx,
                                                      tBTM_ESCO_DATA *p_parms);

/*******************************************************************************
**
** Function         BTM_ChangeEScoLinkParms
**
** Description      This function requests renegotiation of the parameters on
**                  the current eSCO Link.  If any of the changes are accepted
**                  by the controllers, the BTM_ESCO_CHG_EVT event is sent in
**                  the tBTM_ESCO_CBACK function with the current settings of
**                  the link. The callback is registered through the call to
**                  BTM_SetEScoMode.
**
**
** Returns          BTM_CMD_STARTED if command is successfully initiated.
**                  BTM_ILLEGAL_VALUE if no connection for specified sco_inx.
**                  BTM_NO_RESOURCES - not enough resources to initiate command.
**                  BTM_MODE_UNSUPPORTED if local controller does not support
**                      1.2 specification.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ChangeEScoLinkParms (UINT16 sco_inx,
                                                        tBTM_CHG_ESCO_PARAMS *p_parms);

/*******************************************************************************
**
** Function         BTM_EScoConnRsp
**
** Description      This function is called upon receipt of an (e)SCO connection
**                  request event (BTM_ESCO_CONN_REQ_EVT) to accept or reject
**                  the request. Parameters used to negotiate eSCO links.
**                  If p_parms is NULL, then values set through BTM_SetEScoMode
**                  are used.
**                  If the link type of the incoming request is SCO, then only
**                  the tx_bw, max_latency, content format, and packet_types are
**                  valid.  The hci_status parameter should be
**                  ([0x0] to accept, [0x0d..0x0f] to reject)
**
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_EScoConnRsp (UINT16 sco_inx, UINT8 hci_status,
                                         tBTM_ESCO_PARAMS *p_parms);

/*******************************************************************************
**
** Function         BTM_GetNumScoLinks
**
** Description      This function returns the number of active SCO links.
**
** Returns          UINT8
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_GetNumScoLinks (void);

/*****************************************************************************
**  SECURITY MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_SecRegister
**
** Description      Application manager calls this function to register for
**                  security services.  There can be one and only one application
**                  saving link keys.  BTM allows only first registration.
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SecRegister (tBTM_APPL_INFO *p_cb_info);


/*******************************************************************************
**
** Function         BTM_SecRegisterLinkKeyNotificationCallback
**
** Description      Profiles can register to be notified when a new Link Key
**                  is generated per connection.
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SecRegisterLinkKeyNotificationCallback (tBTM_LINK_KEY_CALLBACK *p_callback);


/*******************************************************************************
**
** Function         BTM_SecAddRmtNameNotifyCallback
**
** Description      Profiles can register to be notified when name of the
**                  remote device is resolved (up to BTM_SEC_MAX_RMT_NAME_CALLBACKS).
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SecAddRmtNameNotifyCallback (tBTM_RMT_NAME_CALLBACK *p_callback);


/*******************************************************************************
**
** Function         BTM_SecDeleteRmtNameNotifyCallback
**
** Description      A profile can deregister notification when a new Link Key
**                  is generated per connection.
**
** Returns          TRUE if OK, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SecDeleteRmtNameNotifyCallback (tBTM_RMT_NAME_CALLBACK *p_callback);


/*******************************************************************************
**
** Function         BTM_SecSetConnectFilterCallback
**
** Description      Host can register to be asked whenever an HCI connection
**                  request is received.  In the registered function host
**                  suppose to check connectibility filters.  Yes/No result
**                  should be returned synchronously.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SecSetConnectFilterCallback (tBTM_FILTER_CB *p_callback);


/*******************************************************************************
**
** Function         BTM_GetSecurityMode
**
** Description      Get security mode for the device
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_GetSecurityMode (void);


/*******************************************************************************
**
** Function         BTM_GetSecurityFlags
**
** Description      Get security flags for the device
**
** Returns          BOOLEAN TRUE or FALSE is device found
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_GetSecurityFlags (BD_ADDR bd_addr, UINT8 * p_sec_flags);

/*******************************************************************************
**
** Function         BTM_ReadTrustedMask
**
** Description      Get trusted mask for the device
**
** Returns          NULL, if the device record is not found.
**                  otherwise, the trusted mask
**
*******************************************************************************/
    BTM_API extern UINT32 * BTM_ReadTrustedMask (BD_ADDR bd_addr);


/*******************************************************************************
**
** Function         BTM_SetSecurityMode
**
** Description      Set security mode for the device
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetSecurityMode (UINT8 sec_mode);


/*******************************************************************************
**
** Function         BTM_SetPinType
**
** Description      Set PIN type for the device.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetPinType (UINT8 pin_type, PIN_CODE pin_code, UINT8 pin_code_len);


/*******************************************************************************
**
** Function         BTM_SetPairableMode
**
** Description      Enable or disable pairing
**
** Parameters       allow_pairing - (TRUE or FALSE) whether or not the device
**                      allows pairing.
**                  connect_only_paired - (TRUE or FALSE) whether or not to
**                      only allow paired devices to connect.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetPairableMode (BOOLEAN allow_pairing, BOOLEAN connect_only_paired);

/*******************************************************************************
**
** Function         BTM_SetSecurityLevel
**
** Description      Register service security level with Security Manager.  Each
**                  service must register its requirements regardless of the
**                  security level that is used.  This API is called once for originators
**                  nad again for acceptors of connections.
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SetSecurityLevel (BOOLEAN is_originator, char *p_name,
                                                 UINT8 service_id, UINT16 sec_level,
                                                 UINT16 psm, UINT32 mx_proto_id,
                                                 UINT32 mx_chan_id);

/*******************************************************************************
**
** Function         BTM_SetUCDSecurityLevel
**
** Description      Register UCD service security level with Security Manager.  Each
**                  service must register its requirements regardless of the
**                  security level that is used.  This API is called once for originators
**                  and again for acceptors of connections.
**
** Returns          TRUE if registered OK, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SetUCDSecurityLevel (BOOLEAN is_originator, char *p_name, UINT8 service_id,
                                                    UINT16 sec_level, UINT16 psm, UINT32 mx_proto_id,
                                                    UINT32 mx_chan_id);

/*******************************************************************************
**
** Function         BTM_SetOutService
**
** Description      This function is called to set the service for
**                  outgoing connection.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetOutService(BD_ADDR bd_addr, UINT8 service_id, UINT32 mx_chan_id);

/*******************************************************************************
**
** Function         BTM_SecClrService
**
** Description      Removes specified service record(s) from the security database.
**                  All service records with the specified name are removed.
**                  Typically used only by devices with limited RAM so that it can
**                  reuse an old security service record.
**                          records (except SDP).
**
** Returns          Number of records that were freed.
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_SecClrService (UINT8 service_id);

/*******************************************************************************
**
** Function         BTM_SecClrUCDService
**
** Description
**
** Parameters       Service ID - Id of the service to remove. ('0' removes all service
**                          records.
**
** Returns          Number of records that were freed.
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_SecClrUCDService (UINT8 service_id);

/*******************************************************************************
**
** Function         BTM_SecAddDevice
**
** Description      Add/modify device.  This function will be normally called
**                  during host startup to restore all required information
**                  stored in the NVRAM.
**                  dev_class, bd_name, link_key, and features are NULL if unknown
**
** Returns          TRUE if added OK, else FALSE
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SecAddDevice (BD_ADDR bd_addr, DEV_CLASS dev_class,
                                             BD_NAME bd_name, BD_FEATURES features,
                                             UINT32 trusted_mask[], LINK_KEY link_key,
                                             UINT8 key_type, tBTM_IO_CAP io_cap);


/*******************************************************************************
**
** Function         BTM_SecDeleteDevice
**
** Description      Free resources associated with the device.
**
** Returns          TRUE if rmoved OK, FALSE if not found
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_SecDeleteDevice (BD_ADDR bd_addr);


/*******************************************************************************
**
** Function         BTM_SecUseMasterLinkKey
**
** Description      This function is called to tell master of the piconet to
**                  switch to master link key
**
** Returns          BTM_SUCCESS if command is successully initiated
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SecUseMasterLinkKey (BOOLEAN use_master_key);


/*******************************************************************************
**
** Function         BTM_SetMasterKeyCompCback
**
** Description      This function is called to register for the master key complete
**                  status event.
**
** Parameters:      mkey_cback - callback registered with the security manager
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SetMasterKeyCompCback(tBTM_MKEY_CALLBACK *mkey_cback );


/*******************************************************************************
**
** Function         BTM_SecGetDeviceLinkKey
**
** Description      This function is called to obtain link key for the device
**                  it returns BTM_SUCCESS if link key is available, or
**                  BTM_UNKNOWN_ADDR if Security Manager does not know about
**                  the device or device record does not contain link key info
**
** Returns          BTM_SUCCESS if successful, otherwise error code
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SecGetDeviceLinkKey (BD_ADDR bd_addr,
                                                        LINK_KEY link_key);


/*******************************************************************************
**
** Function         BTM_PINCodeReply
**
** Description      This function is called after Security Manager submitted
**                  PIN code request to the UI.
**
** Parameters:      bd_addr      - Address of the device for which PIN was requested
**                  res          - result of the operation BTM_SUCCESS if success
**                  pin_len      - length in bytes of the PIN Code
**                  p_pin        - pointer to array with the PIN Code
**                  trusted_mask - bitwise OR of trusted services (array of UINT32)
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_PINCodeReply (BD_ADDR bd_addr, UINT8 res, UINT8 pin_len,
                                          UINT8 *p_pin, UINT32 trusted_mask[]);


/*******************************************************************************
**
** Function         BTM_DeviceAuthorized
**
** Description      This function is called after Security Manager submitted
**                  authorization request to the UI.
**
** Parameters:      bd_addr     - Address of the device for which PIN was requested
**                  res         - result of the operation BTM_SUCCESS if success
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_DeviceAuthorized (BD_ADDR bd_addr, UINT8 res,
                                              UINT32 trusted_mask[]);


/*******************************************************************************
**
** Function         BTM_SecBond
**
** Description      This function is called to perform bonding with peer device.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SecBond (BD_ADDR bd_addr, UINT8 pin_len, UINT8 *p_pin, UINT32 trusted_mask[]);

/*******************************************************************************
**
** Function         BTM_SecBondCancel
**
** Description      This function is called to cancel ongoing bonding process
**                  with peer device.
**
** Returns          BTM_CMD_STARTED if successfully initiated, otherwise error
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SecBondCancel (BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_SetEncryption
**
** Description      This function is called to ensure that connection is
**                  encrypted.  Should be called only on an open connection.
**                  Typically only needed for connections that first want to
**                  bring up unencrypted links, then later encrypt them.
**
** Parameters:      bd_addr       - Address of the peer device
**                  p_callback    - Pointer to callback function called if
**                                  this function returns PENDING after required
**                                  procedures are completed.  Can be set to NULL
**                                  if status is not desired.
**                  p_ref_data    - pointer to any data the caller wishes to receive
**                                  in the callback function upon completion.
*                                   can be set to NULL if not used.
**
** Returns          BTM_SUCCESS   - already encrypted
**                  BTM_PENDING   - command will be returned in the callback
**                  BTM_WRONG_MODE- connection not up.
**                  BTM_BUSY      - security procedures are currently active
**                  BTM_MODE_UNSUPPORTED - if security manager not linked in.
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetEncryption (BD_ADDR bd_addr, tBTM_SEC_CBACK *p_callback,
                                                  void *p_ref_data);

/*******************************************************************************
**
** Function         BTM_ConfirmReqReply
**
** Description      This function is called to confirm the numeric value for
**                  Simple Pairing in response to BTM_SP_CFM_REQ_EVT
**
** Parameters:      res           - result of the operation BTM_SUCCESS if success
**                  bd_addr       - Address of the peer device
**
*******************************************************************************/
    BTM_API extern void BTM_ConfirmReqReply(tBTM_STATUS res, BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_PasskeyReqReply
**
** Description      This function is called to provide the passkey for
**                  Simple Pairing in response to BTM_SP_KEY_REQ_EVT
**
** Parameters:      res           - result of the operation BTM_SUCCESS if success
**                  bd_addr       - Address of the peer device
**                  passkey       - numeric value in the range of 0 - 999999(0xF423F).
**
*******************************************************************************/
    BTM_API extern void BTM_PasskeyReqReply(tBTM_STATUS res, BD_ADDR bd_addr, UINT32 passkey);

/*******************************************************************************
**
** Function         BTM_SendKeypressNotif
**
** Description      This function is used during the passkey entry model
**                  by a device with KeyboardOnly IO capabilities
**                  (very likely to be a HID Device).
**                  It is called by a HID Device to inform the remote device when
**                  a key has been entered or erased.
**
** Parameters:      bd_addr - Address of the peer device
**                  type - notification type
**
*******************************************************************************/
    BTM_API extern void BTM_SendKeypressNotif(BD_ADDR bd_addr, tBTM_SP_KEY_TYPE type);

/*******************************************************************************
**
** Function         BTM_IoCapRsp
**
** Description      This function is called in response to BTM_SP_IO_REQ_EVT
**                  When the event data io_req.oob_data is set to BTM_OOB_UNKNOWN
**                  by the tBTM_SP_CALLBACK implementation, this function is
**                  called to provide the actual response
**
** Parameters:      bd_addr - Address of the peer device
**                  io_cap  - The IO capability of local device.
**                  oob     - BTM_OOB_NONE or BTM_OOB_PRESENT.
**                  auth_req- MITM protection required or not.
**
*******************************************************************************/
    BTM_API extern void BTM_IoCapRsp(BD_ADDR bd_addr, tBTM_IO_CAP io_cap,
                                     tBTM_OOB_DATA oob, tBTM_AUTH_REQ auth_req);

/*******************************************************************************
**
** Function         BTM_ReadLocalOobData
**
** Description      This function is called to read the local OOB data from
**                  LM
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadLocalOobData(void);

/*******************************************************************************
**
** Function         BTM_RemoteOobDataReply
**
** Description      This function is called to provide the remote OOB data for
**                  Simple Pairing in response to BTM_SP_RMT_OOB_EVT
**
** Parameters:      bd_addr     - Address of the peer device
**                  c           - simple pairing Hash C.
**                  r           - simple pairing Randomizer  C.
**
*******************************************************************************/
    BTM_API extern void BTM_RemoteOobDataReply(tBTM_STATUS res, BD_ADDR bd_addr, BT_OCTET16 c, BT_OCTET16 r);

/*******************************************************************************
**
** Function         BTM_BuildOobData
**
** Description      This function is called to build the OOB data payload to
**                  be sent over OOB (non-Bluetooth) link
**
** Parameters:      p_data  - the location for OOB data
**                  max_len - p_data size.
**                  c       - simple pairing Hash C.
**                  r       - simple pairing Randomizer  C.
**                  name_len- 0, local device name would not be included.
**                            otherwise, the local device name is included for
**                            up to this specified length
**
** Returns          Number of bytes in p_data.
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_BuildOobData(UINT8 *p_data, UINT16 max_len, BT_OCTET16 c,
                                           BT_OCTET16 r, UINT8 name_len);

/*******************************************************************************
**
** Function         BTM_ReadOobData
**
** Description      This function is called to parse the OOB data payload
**                  received over OOB (non-Bluetooth) link
**
** Parameters:      p_data  - the location for OOB data
**                  eir_tag - The associated EIR tag to read the data.
**                  *p_len(output) - the length of the data with the given tag.
**
** Returns          the beginning of the data with the given tag.
**                  NULL, if the tag is not found.
**
*******************************************************************************/
    BTM_API extern UINT8 * BTM_ReadOobData(UINT8 *p_data, UINT8 eir_tag, UINT8 *p_len);

/*******************************************************************************
**
** Function         BTM_SecReadDevName
**
** Description      Looks for the device name in the security database for the
**                  specified BD address.
**
** Returns          Pointer to the name or NULL
**
*******************************************************************************/
    BTM_API extern char *BTM_SecReadDevName (BD_ADDR bd_addr);


/*****************************************************************************
**  POWER MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_PmRegister
**
** Description      register or deregister with power manager
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_NO_RESOURCES if no room to hold registration
**                  BTM_ILLEGAL_VALUE
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_PmRegister (UINT8 mask, UINT8 *p_pm_id,
                                               tBTM_PM_STATUS_CBACK *p_cb);


/*******************************************************************************
**
** Function         BTM_SetPowerMode
**
** Description      store the mode in control block or
**                  alter ACL connection behavior.
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_UNKNOWN_ADDR if bd addr is not active or bad
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetPowerMode (UINT8 pm_id, BD_ADDR remote_bda,
                                                 tBTM_PM_PWR_MD *p_mode);


/*******************************************************************************
**
** Function         BTM_ReadPowerMode
**
** Description      This returns the current mode for a specific
**                  ACL connection.
**
** Input Param      remote_bda - device address of desired ACL connection
**
** Output Param     p_mode - address where the current mode is copied into.
**                          BTM_ACL_MODE_NORMAL
**                          BTM_ACL_MODE_HOLD
**                          BTM_ACL_MODE_SNIFF
**                          BTM_ACL_MODE_PARK
**                          (valid only if return code is BTM_SUCCESS)
**
** Returns          BTM_SUCCESS if successful,
**                  BTM_UNKNOWN_ADDR if bd addr is not active or bad
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadPowerMode (BD_ADDR remote_bda,
                                                  tBTM_PM_MODE *p_mode);

/*******************************************************************************
**
** Function         BTM_SetSsrParams
**
** Description      This sends the given SSR parameters for the given ACL
**                  connection if it is in ACTIVE mode.
**
** Input Param      remote_bda - device address of desired ACL connection
**                  max_lat    - maximum latency (in 0.625ms)(0-0xFFFE)
**                  min_rmt_to - minimum remote timeout
**                  min_loc_to - minimum local timeout
**
**
** Returns          BTM_SUCCESS if the HCI command is issued successful,
**                  BTM_UNKNOWN_ADDR if bd addr is not active or bad
**                  BTM_CMD_STORED if the command is stored
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SetSsrParams (BD_ADDR remote_bda, UINT16 max_lat,
                                                 UINT16 min_rmt_to, UINT16 min_loc_to);

/*******************************************************************************
**
** Function         BTM_IsPowerManagerOn
**
** Description      This function is called to check if power manager is included.
**                  in the BTE version.
**
** Returns          TRUE if power manager is compiled in, otherwise FALSE.
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_IsPowerManagerOn (void);


/*******************************************************************************
**
** Function         BTM_GetHCIConnHandle
**
** Description      This function is called to get the handle for an ACL connection
**                  to a specific remote BD Address.
**
** Returns          the handle of the connection, or 0xFFFF if none.
**
*******************************************************************************/
    BTM_API extern UINT16 BTM_GetHCIConnHandle (BD_ADDR remote_bda);


/*******************************************************************************
**
** Function         BTM_ReadStoredLinkKey
**
** Description      This function is called to obtain link key for the specified
**                  device from the NVRAM storage attached to the Bluetooth
**                  controller.
**
** Parameters:      bd_addr      - Address of the device
**                  p_cb         - Call back function to be called to return
**                                 the results
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ReadStoredLinkKey (BD_ADDR bd_addr,  tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_WriteStoredLinkKey
**
** Description      This function is called to write link keys for the specified
**                  device addresses to the NVRAM storage attached to the Bluetooth
**                  controller.
**
** Parameters:      num_keys     - Number of link keys
**                  bd_addr      - Addresses of the devices
**                  link_key     - Link Keys to be stored
**                  p_cb         - Call back function to be called to return
**                                 the results
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_WriteStoredLinkKey (UINT8 num_keys,
                                                       BD_ADDR *bd_addr,
                                                       LINK_KEY *link_key,
                                                       tBTM_CMPL_CB *p_cb);


/*******************************************************************************
**
** Function         BTM_DeleteStoredLinkKey
**
** Description      This function is called to delete link key for the specified
**                  device addresses from the NVRAM storage attached to the Bluetooth
**                  controller.
**
** Parameters:      bd_addr      - Addresses of the devices
**                  p_cb         - Call back function to be called to return
**                                 the results
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_DeleteStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB *p_cb);

/*******************************************************************************
**
** Function         BTM_WriteEIR
**
** Description      This function is called to write EIR data to controller.
**
** Parameters       p_buff - allocated HCI command buffer including extended
**                           inquriry response
**
** Returns          BTM_SUCCESS  - if successful
**                  BTM_MODE_UNSUPPORTED - if local device cannot support it
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_WriteEIR( BT_HDR * p_buff );

/*******************************************************************************
**
** Function         BTM_CheckEirData
**
** Description      This function is called to get EIR data from significant part.
**
** Parameters       p_eir - pointer of EIR significant part
**                  type   - finding EIR data type
**                  p_length - return the length of EIR data
**
** Returns          pointer of EIR data
**
*******************************************************************************/
    BTM_API extern UINT8 *BTM_CheckEirData( UINT8 *p_eir, UINT8 type, UINT8 *p_length );

/*******************************************************************************
**
** Function         BTM_HasEirService
**
** Description      This function is called to know if UUID in bit map of UUID.
**
** Parameters       p_eir_uuid - bit map of UUID list
**                  uuid16 - UUID 16-bit
**
** Returns          TRUE - if found
**                  FALSE - if not found
**
*******************************************************************************/
    BTM_API extern BOOLEAN BTM_HasEirService( UINT32 *p_eir_uuid, UINT16 uuid16 );

/*******************************************************************************
**
** Function         BTM_HasInquiryEirService
**
** Description      This function is called to know if UUID in bit map of UUID list.
**
** Parameters       p_results - inquiry results
**                  uuid16 - UUID 16-bit
**
** Returns          BTM_EIR_FOUND - if found
**                  BTM_EIR_NOT_FOUND - if not found and it is complete list
**                  BTM_EIR_UNKNOWN - if not found and it is not complete list
**
*******************************************************************************/
    BTM_API extern tBTM_EIR_SEARCH_RESULT BTM_HasInquiryEirService( tBTM_INQ_RESULTS *p_results,
                                                                    UINT16 uuid16 );

/*******************************************************************************
**
** Function         BTM_AddEirService
**
** Description      This function is called to add a service in bit map of UUID list.
**
** Parameters       p_eir_uuid - bit mask of UUID list for EIR
**                  uuid16 - UUID 16-bit
**
** Returns          None
**
*******************************************************************************/
    BTM_API extern void BTM_AddEirService( UINT32 *p_eir_uuid, UINT16 uuid16 );

/*******************************************************************************
**
** Function         BTM_RemoveEirService
**
** Description      This function is called to remove a service in bit map of UUID list.
**
** Parameters       p_eir_uuid - bit mask of UUID list for EIR
**                  uuid16 - UUID 16-bit
**
** Returns          None
**
*******************************************************************************/
    BTM_API extern void BTM_RemoveEirService( UINT32 *p_eir_uuid, UINT16 uuid16 );

/*******************************************************************************
**
** Function         BTM_GetEirSupportedServices
**
** Description      This function is called to get UUID list from bit map of UUID list.
**
** Parameters       p_eir_uuid - bit mask of UUID list for EIR
**                  p - reference of current pointer of EIR
**                  max_num_uuid16 - max number of UUID can be written in EIR
**                  num_uuid16 - number of UUID have been written in EIR
**
** Returns          BTM_EIR_MORE_16BITS_UUID_TYPE, if it has more than max
**                  BTM_EIR_COMPLETE_16BITS_UUID_TYPE, otherwise
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_GetEirSupportedServices( UINT32 *p_eir_uuid,    UINT8 **p,
                                                      UINT8  max_num_uuid16, UINT8 *p_num_uuid16);

/*******************************************************************************
**
** Function         BTM_GetEirUuidList
**
** Description      This function parses EIR and returns UUID list.
**
** Parameters       p_eir - EIR
**                  uuid_size - LEN_UUID_16, LEN_UUID_32, LEN_UUID_128
**                  p_num_uuid - return number of UUID in found list
**                  p_uuid_list - return UUID 16-bit list
**                  max_num_uuid - maximum number of UUID to be returned
**
** Returns          0 - if not found
**                  BTM_EIR_COMPLETE_16BITS_UUID_TYPE
**                  BTM_EIR_MORE_16BITS_UUID_TYPE
**                  BTM_EIR_COMPLETE_32BITS_UUID_TYPE
**                  BTM_EIR_MORE_32BITS_UUID_TYPE
**                  BTM_EIR_COMPLETE_128BITS_UUID_TYPE
**                  BTM_EIR_MORE_128BITS_UUID_TYPE
**
*******************************************************************************/
    BTM_API extern UINT8 BTM_GetEirUuidList( UINT8 *p_eir, UINT8 uuid_size, UINT8 *p_num_uuid,
                                             UINT8 *p_uuid_list, UINT8 max_num_uuid);

/*******************************************************************************
**
** Function         BTM_SyncStack
**
** Description      For Dual-Stack support. Called to initiate switching to/from
**                  main stack (running on phone baseband) to mm stack (light
**                  stack running on multi-media chip)
**
** Parameters       sync_dir: BTM_SW_BB_TO_MM: switch from BB to MM stack
**                            BTM_SW_MM_TO_BB: switch from MM to BB stack
**                            BTM_SW_RESYNC: resync MM and BB stacks
**
**                  p_sync_cback: callback function for event notification
** Returns
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_SyncStack(tBTM_SW_DIR sync_dir, tBTM_SYNC_STACK_CBACK p_sync_cback);

/*******************************************************************************
**
** Function         BTM_SyncBtaRsp
**
** Description      For Dual-Stack support. Called to indicate that upper layers
**                  (e.g. BTA or application) have completed synchronizing bta/app
**                  specific layers for switching.
**
**                  Called in response to 'BTM_SYNC_BTA_EVT'
**
** Parameters       status: BTM_SUCESS: bta/app successfully synchronized
**                          otherwise:  sync was unsuccessfule. Abort switch.
**
**                  p_btm_sync_info: information from bta/app that will be needed
**                                  by BTE (avdt and l2cap) for switching.
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_SyncBtaRsp(tBTM_STATUS status, tBTM_SYNC_INFO *p_btm_sync_info);

/*******************************************************************************
**
** Function         BTM_OpenUIPC
**
** Description      For Dual-Stack support. Called to open UIPC between
**                  main stack (running on phone baseband) to embedded light stack
**                  (running on Multimedia or Bluetooth Controller chip)
**
** Parameters       sync_dir: BTM_SW_BB_TO_MM: switch from BB to MM stack
**                            BTM_SW_BB_TO_BTC:switch from BB to BTC stack
**
**                  p_sync_callback: callback function for event notification
** Returns
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_OpenUIPC(tBTM_SW_DIR sync_dir, tBTM_SYNC_STACK_CBACK p_sync_callback);

/*******************************************************************************
**
** Function         BTM_CloseUIPC
**
** Description      For Dual-Stack support. Called to close UIPC between
**                  main stack (running on phone baseband) to embedded light stack
**                  (running on Multimedia or Bluetooth Controller chip)
**
** Parameters
**                  p_sync_callback: callback function for event notification
** Returns
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_CloseUIPC(tBTM_SYNC_STACK_CBACK p_sync_callback);

/*******************************************************************************
**
** Function         BTM_IpcSend
**
** Description      For Dual-Stack support. Called to send ipc messages from
**                  full stack to lite stack and vice-versa. This API is
**                  typically called by bta layers e.g. bta_av.
**
**
** Parameters       len:    Length of the buffer in the ipc message
**
**                  buffer: Pointer to the buffer to be passed in the IPC message
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_IpcSend(UINT16 len, UINT8* buffer);

/*******************************************************************************
**
** Function         BTM_IpcSendBuf
**
** Description      For Dual-Stack support. Called to send ipc messages from
**                  full stack to lite stack and vice-versa. This API is
**                  typically called by bta layers e.g. bta_av_sync.
**
**
** Parameters       p_buf: Pointer to the buffer to be passed in the IPC message
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_IpcSendBuf(BT_HDR* p_buf);

/*******************************************************************************
**
** Function         BTM_RegIpcEvtHandler
**
** Description      registers the DM provided handler for IPC events
**
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_RegIpcEvtHandler(tBTM_IPC_EVT_CBACK *p_cback);

/*******************************************************************************
**
** Function         BTM_RegRTIpcEvtHandler
**
** Description      registers the RT(Audio Routing) provided handler for IPC events
**
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_RegRTIpcEvtHandler(tBTM_IPC_EVT_CBACK *p_cback);

/*****************************************************************************
**  N2BT
*****************************************************************************/

/* Data callback for N2BT */
    typedef void (tBTM_N2BT_DATA_CB) (BD_ADDR bd_addr, UINT16 handle, UINT8 *p_data, UINT16 datalen);

/*******************************************************************************
**
** Function         BTM_N2BtAcquire
**
** Description      Put controller into acquisition mode
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_N2BtAcquire(BD_ADDR bd_addr, UINT16 timeout,
                                        UINT8 freq, UINT8 src_addrlen, UINT8 sensor_flags,
                                        UINT8 sensor_type, UINT8 sensor_clk_accuracy,
                                        UINT16 add_rx_window, UINT16 init_crc,
                                        UINT32 ac_low, UINT32 ac_high, UINT16 pkt_hdr,
                                        UINT16 list_dur, UINT16 list_int,
                                        UINT8 oor_missed_pkts, tBTM_VSC_CMPL_CB *p_cb,
                                        tBTM_N2BT_DATA_CB *p_data_cback);

/*******************************************************************************
**
** Function         BTM_N2BtDisconnect
**
** Description      Disconnects all N2BT devices
**
** Returns          void
**
*******************************************************************************/
    BTM_API extern void BTM_N2BtDisconnect(void);


/*******************************************************************************
**
** Function         BTM_ConfigI2SPCM
**
** Description      This function sends VSC Write_I2SPCM_Interface_Param
**                  as to the specified codec_type.
**
**
** Parameter        codec_type: codec_type to be used for sco connection.
**                  role: master or slave role
**                  sample_rate: sampling rate
**                  clock_rate:clock rate 128K to 2048K
**
**
** Returns          BTM_SUCCESS if the successful.
**                  BTM_ILLEGAL_VALUE: wrong codec type
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ConfigI2SPCM (tBTM_SCO_CODEC_TYPE codec_type, UINT8 role, UINT8 sample_rate, UINT8 clock_rate);

/*****************************************************************************
**  SCO OVER HCI
*****************************************************************************/
/*******************************************************************************
**
** Function         BTM_ConfigScoPath
**
** Description      This function enable/disable SCO over HCI and registers SCO
**                  data callback if SCO over HCI is enabled.
**
** Parameter        path: SCO or HCI
**                  p_sco_data_cb: callback function or SCO data if path is set
**                                 to transport.
**                  p_pcm_param: pointer to the PCM interface parameter. If a NULL
**                               pointer is used, PCM parameter maintained in
**                               the control block will be used; otherwise update
**                               control block value.
**                  err_data_rpt: Lisbon feature to enable the erronous data report
**                                or not.
**
** Returns          BTM_SUCCESS if the successful.
**                  BTM_NO_RESOURCES: no rsource to start the command.
**                  BTM_ILLEGAL_VALUE: invalid callback function pointer.
**                  BTM_CMD_STARTED :Command sent. Waiting for command cmpl event.
**
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_ConfigScoPath (tBTM_SCO_ROUTE_TYPE path,
                                                  tBTM_SCO_DATA_CB *p_sco_data_cb,
                                                  tBTM_SCO_PCM_PARAM *p_pcm_param,
                                                  BOOLEAN err_data_rpt);

/*******************************************************************************
**
** Function         BTM_WriteScoData
**
** Description      This function write SCO data to a specified instance. The data
**                  to be written p_buf needs to carry an offset of
**                  HCI_SCO_PREAMBLE_SIZE bytes, and the data length can not
**                  exceed BTM_SCO_DATA_SIZE_MAX bytes, whose default value is set
**                  to 60 and is configurable. Data longer than the maximum bytes
**                  will be truncated.
**
** Returns          BTM_SUCCESS: data write is successful
**                  BTM_ILLEGAL_VALUE: SCO data contains illegal offset value.
**                  BTM_SCO_BAD_LENGTH: SCO data length exceeds the max SCO packet
**                                      size.
**                  BTM_NO_RESOURCES: no resources.
**                  BTM_UNKNOWN_ADDR: unknown SCO connection handle, or SCO is not
**                                    routed via HCI.
**
**
*******************************************************************************/
    BTM_API extern tBTM_STATUS BTM_WriteScoData (UINT16 sco_inx, BT_HDR *p_buf);

/*******************************************************************************
**
** Function         BTM_SetARCMode
**
** Description      Send Audio Routing Control command.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_SetARCMode (UINT8 iface, UINT8 arc_mode, tBTM_VSC_CMPL_CB *p_arc_cb);


/*******************************************************************************
**
** Function         BTM_PCM2Setup_Write
**
** Description      Send PCM2_Setup write command.
**
** Returns          void
**
*******************************************************************************/
BTM_API extern void BTM_PCM2Setup_Write (BOOLEAN clk_master, tBTM_VSC_CMPL_CB *p_arc_cb);

#ifdef __cplusplus
}
#endif

#endif /* BTM_API_H */
/* end "revolution/BTE/stack/include/btm_api.h" */

/* GAP return codes (not present in the RVL SDK headers; values recovered
 * from the retail gap_convert_btm_status jump table / data section). */
#define GAP_SUCCESS          0x0000
#define GAP_CMD_INITIATED    0x010B
#define GAP_ERR_BUSY         0x0103
#define GAP_ERR_ILLEGAL_PARAM 0x0109
#define GAP_ERR_NOT_READY    0x010C
#define GAP_ERR_BAD_BD_ADDR  0x010D
#define GAP_ERR_PROCESSING   0x0114
#define GAP_DEVICE_TIMEOUT   0x0115

/* BTM event dispatcher: index selects the GAP control block (0 or 1),
 * p_msg is the BTM event message. gap_btm_cback0/1 tail-call into it. */
#pragma push
#pragma auto_inline off
void btm_cback(UINT16 index, void *p_msg) {}
#pragma pop

void gap_btm_cback0(void *p_msg)
{
    btm_cback(0, p_msg);
}

void gap_btm_cback1(void *p_msg)
{
    btm_cback(1, p_msg);
}

void gap_find_addr_name_cb() {}

void gap_find_addr_inq_cb() {}

UINT16 gap_convert_btm_status(tBTM_STATUS btm_status)
{
    switch (btm_status) {
    case BTM_SUCCESS:
        return GAP_SUCCESS;
    case BTM_CMD_STARTED:
        return GAP_CMD_INITIATED;
    case BTM_BUSY:
        return GAP_ERR_BUSY;
    case BTM_MODE_UNSUPPORTED:
    case BTM_ILLEGAL_VALUE:
        return GAP_ERR_ILLEGAL_PARAM;
    case BTM_WRONG_MODE:
        return GAP_ERR_NOT_READY;
    case BTM_UNKNOWN_ADDR:
        return GAP_ERR_BAD_BD_ADDR;
    case BTM_DEVICE_TIMEOUT:
        return GAP_DEVICE_TIMEOUT;
    default:
        return GAP_ERR_PROCESSING;
    }
}
