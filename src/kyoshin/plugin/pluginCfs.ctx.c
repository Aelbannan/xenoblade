// Translation unit for kyoshin/plugin/pluginCfs
// Plugin script functions for the CFS (Common File System / script) subsystem.

/* "src/kyoshin/plugin/pluginCfs.cpp" line 3 "kyoshin/harness_catalog.hpp" */
#pragma once

/**
 * Umbrella for auto-scaffolded kyoshin catalog TUs that lack a unit header.
 *
 * Pulls recovered VM / script-helper headers only. Plugin units with their own
 * header (ocUnit.hpp, ocBuiltin.hpp, …) should include that instead.
 */

/* "src/kyoshin/harness_catalog.hpp" line 9 "types.h" */
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
/* "src/kyoshin/harness_catalog.hpp" line 10 "cstring" */
#ifndef MSL_CPP_CSTRING_H
#define MSL_CPP_CSTRING_H
/* "libs/PowerPC_EABI_Support/include/stl/cstring" line 2 "string.h" */
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
#ifdef __cplusplus

namespace std {
using ::__memrchr;
using ::memchr;
using ::memcmp;
using ::memcpy;
using ::memmove;
using ::memset;
using ::strcat;
using ::strchr;
using ::strcmp;
using ::strcpy;
using ::stricmp;
using ::strlen;
using ::strncat;
using ::strncmp;
using ::strncpy;
using ::strstr;
} // namespace std

#endif
#endif
/* end "cstring" */

/* "src/kyoshin/harness_catalog.hpp" line 12 "monolib/vm/yvm2.h" */
#pragma once

/* "libs/monolib/include/monolib/vm/yvm2.h" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/vm/yvm2.h" line 3 "monolib/vm/yvm_types.h" */
#pragma once

/* "libs/monolib/include/monolib/vm/yvm_types.h" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/vm/yvm_types.h" line 3 "monolib/vm/sb_types.h" */
#pragma once

//Types/defines for SB script files.

/* "libs/monolib/include/monolib/vm/sb_types.h" line 4 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif

//Minimum supported SB version
#define SB_MIN_VERSION 2

enum SBFlags{
    SB_FLAG_LOADED    = 1 << 0, //Stored in the runtime flag byte (offset 0x7)
    SB_FLAG_ENCRYPTED = 1 << 1  //Stored in the normal flag byte (offset 0x6)
};

typedef struct SBSectionHeader{
    int entriesOffset;  //0x0
    int entries; //0x4
    int offsetSize;  //0x8
} SBSectionHeader;

typedef struct SBHeader{
    char magic[4];                      //0x0
    u8 version;                         //0x4
    u8 unk5; //unused?
    u8 flags;                           //0x6
    /* Reserved by the VM as a place to store various flags during runtime. Only the first bit
    (for the loaded flag) gets used, however. */
    u8 vmFlags;                         //0x7
    SBSectionHeader* codeOfs;            //0x8
    SBSectionHeader* idPoolOfs;          //0xC
    SBSectionHeader* intPoolOfs;         //0x10
    SBSectionHeader* fixedPoolOfs;       //0x14
    SBSectionHeader* stringPoolOfs;      //0x18
    SBSectionHeader* functionPoolOfs;    //0x1C
    SBSectionHeader* pluginImportsOfs;   //0x20
    SBSectionHeader* ocImportsOfs;       //0x24
    SBSectionHeader* functionImportsOfs; //0x28
    SBSectionHeader* staticVarsOfs;      //0x2C
    SBSectionHeader* localPoolOfs;       //0x30
    SBSectionHeader* sysAtrPoolOfs;      //0x34
    SBSectionHeader* usrAtrPoolOfs;      //0x38
    SBSectionHeader* debugSymbolsOfs;    //0x3C
} SBHeader;

//Section specific structs

//Function pool

typedef struct FunctionPoolEntry{
    u16 unk0;
    s16 unk2;
    u16 unk4;
    u8 unk8[0xC - 0x8];
    u32 unkC;
    u8 unk10[0x14 - 0x10];
} FunctionPoolEntry;

//Plugin imports

typedef struct PluginImportEntry{
    u16 unk0;
    u16 unk2;
} PluginImportEntry;

//OC imports

typedef struct OCImportEntry{
    u16 unk0;
} OCImportEntry;

//Function imports

typedef struct FunctionImportEntry{
    u16 unk0;
    u16 unk2;
} FunctionImportEntry;

//Static vars

typedef struct StaticVarsEntry{
    u32 unk0;
    u32 unk4;
} StaticVarsEntry;

//Local pool

typedef struct LocalPoolEntry{
    u32 unk0;
    u32 unk4;
} LocalPoolEntry;

#ifdef __cplusplus
}
#endif
/* end "monolib/vm/sb_types.h" */

#ifdef __cplusplus
extern "C" {
#endif

//Misc constants

#define MAX_PACKAGES 8 //Max number of packages (scripts) at once
#define MAX_PLUGINS 48
#define MAX_OCS 48
#define MAX_THREADS 16
#define MAX_BREAKPOINTS 4
#define MAX_STACK_ENTRIES 128

#define VMC_MAX 96 //Max number of opcodes

typedef struct VMArg{
    u8 type; //0x0
    u16 unk2;
    union {
        void* pointerVal;
        u32 uintVal;
        int intVal;   
    } value; //0x4
} VMArg;

typedef struct VMReg{
    int pc; //0x0
    int sp; //0x4
    int unk8; //0x8
    int exception; //0xC
    int unk10; //0x10
} VMReg;

typedef struct _sVMThread{
    VMReg reg; //0x0
    VMArg unk14[2];
    s16 unk24;
    u8 unk26[2];
    u32 unk28;
    s16 unk2C;
    u8 unk2E[2];
    SBHeader* scriptData; //0x30
    u8* codeData; //0x34
    StaticVarsEntry* staticVarsEntries; //0x38
    VMArg* stack; //0x3C
    u32 unk40;
    u32 id; //0x44
    int unk48;
    BOOL waitMode; //0x4C
    u32 wkIdx; //0x50
    u32 unk54;
    u8 unk58[0x60 - 0x58];
} VMThread;

//Forward declaration
struct OCData;

typedef int (*PluginFunc)(VMThread* pThread);
typedef int (*OCCtorFunc)(VMThread* pThread, void* r4, int r5);
typedef int (*OCSelectorFunc)(VMThread* pThread, int r4);
typedef void (*OCGetSetFunc)(VMThread* pThread, int r4, struct OCData* data);

typedef struct PluginFuncData{
    const char* name; //0x0
    PluginFunc func; //0x4
} PluginFuncData;

typedef struct OCProperty{
    const char* name; //0x0
    OCGetSetFunc getFunc; //0x4
    OCGetSetFunc setFunc; //0x8
    int nameLength; //0xC
} OCProperty;

typedef struct OCSelector{
    const char* name; //0x0
    OCSelectorFunc func; //0x4
    int nameLength; //0x8
} OCSelector;

typedef struct OCData{
    const char* name; //0x0
    OCCtorFunc ctor; //0x4
    OCProperty* properties; //0x8
    OCSelector* selectors; //0xC
} OCData;

typedef struct VMPackage{
    SBHeader* scriptDataPtr; //0x0
    u32 unk4;
} VMPackage;

typedef struct VMPlugin{
    char* unk0;
    PluginFuncData* unk4;
} VMPlugin;

typedef struct VMOC{
    OCData* unk0;
} VMOC;

typedef struct VMBreakpoint{
    u8 unk0[0xC];
} VMBreakpoint;

typedef struct VMState{
    VMPackage packages[MAX_PACKAGES]; //0x0
    VMThread* activeThread; //0x40
    u32 nextThreadId; //0x44
    VMThread* unk48[MAX_THREADS]; //0x48
    VMThread threads[MAX_THREADS]; //0x88
    VMArg threadStacks[MAX_THREADS][MAX_STACK_ENTRIES]; //0x688
    VMPlugin plugins[MAX_PLUGINS]; //0x4688
    VMOC ocs[MAX_OCS]; //0x4808
    OCData* builtinOC; //0x48C8
    //Unused debug data (based on info from XCX)
    BOOL debMode; //0x48CC
    u8 unk48D0[0xC];
    VMBreakpoint bps[MAX_BREAKPOINTS]; //0x48DC
} VMState;

//Enums

typedef enum VMCResult{
    VMC_RESULT_0,
    VMC_RESULT_1,
    VMC_RESULT_2,
    VMC_RESULT_3
} VMCResult;

typedef enum VMCOpcodeType{
    VMC_OP_NOP,
    VMC_OP_CONST_0,
    VMC_OP_CONST_1,
    VMC_OP_CONST_2,
    VMC_OP_CONST_3,
    VMC_OP_CONST_4,
    VMC_OP_CONST_I,
    VMC_OP_CONST_I_W,
    VMC_OP_POOL_INT,
    VMC_OP_POOL_INT_W,
    VMC_OP_POOL_FIXED,
    VMC_OP_POOL_FIXED_W,
    VMC_OP_POOL_STR,
    VMC_OP_POOL_STR_W,
    VMC_OP_LD,
    VMC_OP_ST,
    VMC_OP_LD_ARG,
    VMC_OP_ST_ARG,
    VMC_OP_ST_ARG_OMIT,
    VMC_OP_LD_0,
    VMC_OP_LD_1,
    VMC_OP_LD_2,
    VMC_OP_LD_3,
    VMC_OP_ST_0,
    VMC_OP_ST_1,
    VMC_OP_ST_2,
    VMC_OP_ST_3,
    VMC_OP_LD_ARG_0,
    VMC_OP_LD_ARG_1,
    VMC_OP_LD_ARG_2,
    VMC_OP_LD_ARG_3,
    VMC_OP_ST_ARG_0,
    VMC_OP_ST_ARG_1,
    VMC_OP_ST_ARG_2,
    VMC_OP_ST_ARG_3,
    VMC_OP_LD_STATIC,
    VMC_OP_LD_STATIC_W,
    VMC_OP_ST_STATIC,
    VMC_OP_ST_STATIC_W,
    VMC_OP_LD_AR,
    VMC_OP_ST_AR,
    VMC_OP_LD_NIL,
    VMC_OP_LD_TRUE,
    VMC_OP_LD_FALSE,
    VMC_OP_LD_FUNC,
    VMC_OP_LD_FUNC_W,
    VMC_OP_LD_PLUGIN,
    VMC_OP_LD_PLUGIN_W,
    VMC_OP_LD_FUNC_FAR,
    VMC_OP_LD_FUNC_FAR_W,
    VMC_OP_MINUS,
    VMC_OP_NOT,
    VMC_OP_L_NOT,
    VMC_OP_ADD,
    VMC_OP_SUB,
    VMC_OP_MUL,
    VMC_OP_DIV,
    VMC_OP_MOD,
    VMC_OP_OR,
    VMC_OP_AND,
    VMC_OP_R_SHIFT,
    VMC_OP_L_SHIFT,
    VMC_OP_EQ,
    VMC_OP_NE,
    VMC_OP_GT,
    VMC_OP_LT,
    VMC_OP_GE,
    VMC_OP_LE,
    VMC_OP_L_OR,
    VMC_OP_L_AND,
    VMC_OP_JMP,
    VMC_OP_JPF,
    VMC_OP_CALL,
    VMC_OP_CALL_W,
    VMC_OP_CALL_IND,
    VMC_OP_RET,
    VMC_OP_NEXT,
    VMC_OP_PLUGIN,
    VMC_OP_PLUGIN_W,
    VMC_OP_CALL_FAR,
    VMC_OP_CALL_FAR_W,
    VMC_OP_GET_OC,
    VMC_OP_GET_OC_W,
    VMC_OP_GETTER,
    VMC_OP_GETTER_W,
    VMC_OP_SETTER,
    VMC_OP_SETTER_W,
    VMC_OP_SEND,
    VMC_OP_SEND_W,
    VMC_OP_TYPEOF,
    VMC_OP_SIZEOF,
    VMC_OP_SWITCH,
    VMC_OP_INC,
    VMC_OP_DEC,
    VMC_OP_EXIT,
    VMC_OP_BP //Breakpoint
} VMCOpcodeType;

typedef enum _VMTypes {
    VM_TYPE_NIL,
    VM_TYPE_TRUE,
    VM_TYPE_FALSE,
    VM_TYPE_INT,
    VM_TYPE_FIXED,
    VM_TYPE_STRING,
    VM_TYPE_ARRAY,
    VM_TYPE_FUNCTION,
    VM_TYPE_PLUGIN,
    VM_TYPE_OC,
    VM_TYPE_SYS,

    VM_MAX_TYPE = 11
} VMTypes;

typedef enum VMException {
    VM_EXCEPTION_NONE,
    VM_EXCEPTION_PLUGIN,
    VM_EXCEPTION_OC,
    VM_EXCEPTION_DIV_BY_ZERO,
    VM_EXCEPTION_INVALID_ARRAY,
    VM_EXCEPTION_INDEX_OOB,
    VM_EXCEPTION_MATH_INVALID_ARG,
    VM_EXCEPTION_CALC_INVALID_ARG,
    VM_EXCEPTION_8,
    VM_EXCEPTION_JPF_INVALID_ARG,
    VM_EXCEPTION_CALLIND_INVALID_ARG,
    VM_EXCEPTION_INVALID_OC,
    VM_EXCEPTION_SEND_ERROR,
    VM_EXCEPTION_INVALID_PROPERTY,
    VM_EXCEPTION_INVALID_GETSET_FUNC
} VMException;

#ifdef __cplusplus
}
#endif
/* end "monolib/vm/yvm_types.h" */


#ifdef __cplusplus
extern "C" {
#endif

void vmInit();
BOOL vmLink(u8* pData);
BOOL vmPluginRegist(const char* name, PluginFuncData* plugin_funcs);
void vmStart(u8* pData);

VMThread* vmThreadCreate(SBHeader* pData, u32 r4);
void vmThreadStart(VMThread* pThread, u32 r4);
BOOL vmThreadEnd(u32 r3);
BOOL vmThreadIsAlive(u32 r3);
BOOL vmThreadSleep(u32 r3);
BOOL vmThreadWakeup(u32 r3);
void vmThreadSleepAll(u8* pScriptData);
void vmThreadWakeupAll(u8* pScriptData);
BOOL vmThreadIsFinish(u8* pScriptData);
BOOL vmThreadGetOC(VMThread* pThread, int r4, u32* outId);

VMArg* vmArgPtrGet(VMThread* pThread, int r4);
BOOL vmArgOmitChk(VMThread* pThread, int r4);
BOOL vmArgBoolGet(u32 r3, VMArg* r4);
int vmArgIntGet(u32 r3, VMArg* r4);
int vmArgFixedGet(u32 r3, VMArg* r4);
const char* vmArgStringGet(u32 r3, VMArg* r4);
u32 vmArgFunctionGet(u32 r3, VMArg* r4);
void* vmArgArrayGet(u32 r3, VMArg* r4);
void* vmArgOCGet(u32 r3, VMArg* r4);
u32 vmDataGet(VMThread* pThread, int startIndex, int length);

void vmRetValSet(VMThread* pThread, VMArg* pArg);
void* vmOCPropertyGet(VMThread* pThread);
void vmWaitModeSet(VMThread* pThread);
u32 vmWkIdxGet(VMThread* pThread);
void vmWkIdxSet(VMThread* pThread, u32 r4);
u32* vmWkGet(VMThread* pThread, u32 r4);

void vmPluginExceptionThrow(VMThread* pThread);
void vmOCExceptionThrow(VMThread* pThread);
DECOMP_DONT_INLINE void vmExceptionProc(VMThread* pThread);
void vmExceptionThrow(VMThread* pThread, u32 exception);

const char* vmIdPoolGet(SBHeader* data, u32 no);
int vmIntPoolGet(SBHeader* data, u32 no);
int vmFixedPoolGet(SBHeader* data, u32 no);
void* vmStringPoolGet(SBHeader* data, u32 no);
void* vmLocalPoolGet(SBHeader* data, u32 no);
void* vmFunctionPoolGet(SBHeader* data, u32 no);
u16* vmSysAtrPoolGet(SBHeader* data, u32 no);
u16* vmUsrAtrPoolGet(SBHeader* data, u32 no);

u32 vmSysAtrSearch(SBHeader* data, u32 no);
u32 vmPluginSearch(const char* param1, const char* param2);
u32 vmOCSearch(const char* pName);
u32 vmPropertySearch(OCData* pOC, const char* pName);
u32 vmSelectorSearch(OCData* pOC, const char* pName);
u32 vmFuncFarSearch(const char* pPackageName, const char* pFuncName);

void encodeScramble(u8* data);
int vmc_call_entry(VMThread* pThread, u32 r4, s16 r5, u32 r6);

void vmArgErr();
void vmHalt();

#ifdef __cplusplus
}
#endif
/* end "monolib/vm/yvm2.h" */
/* "src/kyoshin/harness_catalog.hpp" line 13 "kyoshin/code_801862C0.hpp" */
#pragma once

/**
 * Script / OC instance helpers (unit kyoshin/code_801862C0).
 * Names are still placeholder ``func_*`` until symbol recovery; signatures
 * match observed call sites in plugin TUs (VMThread in, OC object out).
 */

/* "src/kyoshin/code_801862C0.hpp" line 8 "types.h" */
/* end "types.h" */
/* "src/kyoshin/code_801862C0.hpp" line 9 "monolib/vm/yvm2.h" */
/* end "monolib/vm/yvm2.h" */

#ifdef __cplusplus
extern "C" {
#endif

void* func_801862C0(VMThread* pThread);
void* func_801862E0(void* p);
void* func_801863F4(void* p);
void* func_80186460(void* p);
void* func_80186474(void* p);
void* func_801864DC(void* pObj, int slot);
void* func_80186664(void* p);
void* func_801866F0(void* p);
void* func_80186A70(void* p);
void* func_80186BC8(void* p);
void* func_80186C7C(void* p);
void* func_80186D20(void* p);

#ifdef __cplusplus
}
#endif
/* end "kyoshin/code_801862C0.hpp" */
/* "src/kyoshin/harness_catalog.hpp" line 14 "kyoshin/plugin/ocBdat.hpp" */
#pragma once

/* "src/kyoshin/plugin/ocBdat.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/plugin/ocBdat.hpp" line 3 "monolib/vm/yvm2.h" */
/* end "monolib/vm/yvm2.h" */

void* getFP(const char* pName);

#pragma pack(push, 1)

// Fixed header at the start of every bdat table.
struct BdatHeader {
    s32 count;        // +0x00
    u32 _pad04;       // +0x04
    u16 stride;       // +0x08: row stride in bytes
    u16 hashBaseOff;  // +0x0A: offset from table start to hash bucket table
    u16 bucketCount;  // +0x0C: number of hash buckets
    u16 dataOff;      // +0x0E: offset from table start to row data
    u16 maxRow;       // +0x10: maximum valid row index
    u16 rowBase;      // +0x12: minimum valid row index (rowBase)
};

// Column entry in bdat hash chain.
struct BdatColEntry {
    u16 colHdrRel;    // +0x00: relative offset to column header (from table start)
    u16 nextOff;      // +0x02: next entry offset (0 = end of chain)
    char name[1];     // +0x04: null-terminated name (variable length)
};

// Column header for type 1 (value).
struct BdatColHdrValue {
    u8 type;          // +0x00: 1
    u8 elemType;      // +0x01: element type enum
    u16 dataOff;      // +0x02: column data offset within row
};

// Column header for type 2 (array).
struct BdatColHdrArray {
    u8 type;          // +0x00: 2
    u8 elemType;      // +0x01: element type
    u16 dataOff;      // +0x02: column data offset within row
    u16 count;        // +0x04: array element count
};

// Column header for type 3 (flag).
struct BdatColHdrFlag {
    u8 type;          // +0x00: 3
    u8 shift;         // +0x01: right-shift amount
    u32 mask;         // +0x02: bitmask
    u16 colEntryRel;  // +0x06: relative offset to the value column entry
};

// Name-index table: s32 count at +0x00, then u32 at +0x04, then
// u16 entry offsets at +0x08 (each entry is a u16 offset from table start
// to the NameEntry structure). The binary search indexes by `mid`.
struct BdatNameIndexHdr {
    s32 count;       // +0x00
    u32 _pad;        // +0x04
    u16 offsets[];   // +0x08 (flexible array of u16 entry offsets)
};

#pragma pack(pop)

// Utility class for handling bdat files.
class CBdat {
public:
    static void* func_8003AA34();
    static void* func_8003AA50();
    static void* func_8003AA78(u32, void*);
    static void func_8003AA8C(u32 val);
    static void* getFP(const char* pName) { return ::getFP(pName); }
    static const char* getBdatStringColumnValue(void* pData, const char* pColumnName, int index);
    static u32 func_8003B1EC(void* pData);
    static u32 func_8003B41C(void* pData);
};

#ifdef __cplusplus
extern "C" {
#endif

void ocBdatRegist();

#ifdef __cplusplus
}
#endif
/* end "kyoshin/plugin/ocBdat.hpp" */
/* "src/kyoshin/harness_catalog.hpp" line 15 "kyoshin/CTaskGameEff.hpp" */
#pragma once

/* "src/kyoshin/CTaskGameEff.hpp" line 2 "types.h" */
/* end "types.h" */

class CTaskGameEff {
public:
    CTaskGameEff();
    virtual ~CTaskGameEff();
    void Init();
    void Term();

    // TODO: add fields
    void Move();
    void cbRenderBefore();
    void Draw();
};

/* end "kyoshin/CTaskGameEff.hpp" */
/* end "kyoshin/harness_catalog.hpp" */
/* "src/kyoshin/plugin/pluginCfs.cpp" line 4 "kyoshin/plugin/pluginCfs.hpp" */
#pragma once

/* "src/kyoshin/plugin/pluginCfs.hpp" line 2 "monolib/vm/yvm2.h" */
/* end "monolib/vm/yvm2.h" */

// Plugin script functions for the CFS (Common File System / script) subsystem.
// These are registered via pluginCfsRegist() and called from the VM.

extern "C" bool func_8004A2E8();
extern "C" void func_8004A400();

extern "C" void pluginCfsRegist();

namespace cf {

class CfObject {
public:
    float CfObject_UnkVirtualFunc31();

    // TODO: add fields
};
} // namespace cf

/* end "kyoshin/plugin/pluginCfs.hpp" */

struct UnkClass_8009ECB0;

// ============================================================================
// External declarations (called functions not declared in included headers)
// ============================================================================
extern "C" {
    extern unsigned long lbl_eu_80663E24;  // game state flags
    extern unsigned long lbl_eu_80663E28;  // game state flags 2
    extern float lbl_eu_80663EDC;          // wait pop timer
    extern unsigned long lbl_eu_80663E14;  // ignore PAL flag
    extern float lbl_eu_80665E30;          // fixed-point divisor (4096.0f)
    extern float lbl_eu_80665E34;          // 0.5f
    extern float lbl_eu_80665E38;          // scale factor
    extern float lbl_eu_80665E3C;          // 1.0f
    extern float lbl_eu_80665E40;          // 4503601774854144.0f (for xoris)
    extern float lbl_eu_80665E48;          // various constants
    extern float lbl_eu_80665E4C;          // 0.0f
    extern float lbl_eu_80665E30;          // fixed-point divisor
    extern float lbl_eu_80665E34;          // 0.5f
    extern float lbl_eu_80665E38;          // scale factor
    extern float lbl_eu_80665E3C;          // 1.0f
    extern float lbl_eu_80665E40;          // 4503601774854144.0f
    extern float lbl_eu_80665E48;          // misc constant
    extern float lbl_eu_80665E4C;          // 0.0f

    void func_8004302C(int, int);
    void func_80043628();
    void func_800A3304();
    void func_801579A4();
    void func_8012FAA8();
    int func_8015796C(int);
    void func_80157184(int);
    void func_801571A8(int);
    void func_8015730C(int, int, int);
    void* func_8015783C(int, int, int);
    void* func_80157948(int, int);
    void func_80159B40(int, int, void*);
    void func_80158118(void*, int, int);
    void func_80155A00(void*);
    void func_80158420(int, int*, int, int*);
    void func_8016DF34(int);
    void func_8016DF4C(int);
    void func_8018C8F4(void*, int);
    void func_80199678(void*, int);
    void func_801F4AD4(int);
    void func_801F4B68(int);
    void func_801F4BFC(int);
    void func_801F4C90(int, int);
    int func_80291BF8();
    void func_8049AB50(int, int);
    int getUnk80664658();
    int isTvFormatPal__9CDeviceVIFv();
    void* CItem_initItemImplInstances();
    void* getInstance__Q22cf14CBattleManagerFv();
    void* getInstance__Class_80296898Fv();
    void* getPlayer__Q22cf13CfGameManagerFi(int);
    void func_800B70FC(int, int);
    int func_800B8D5C();
    void func_800BE12C(int, int, int, int, int);
    void func_800BE28C(int, int);
    bool func_8009CF8C(int);
    void func_8009D018(int, int);
    int* func_8009EC9C(int);
    int* func_8009ECB0();
    void func_8009E0C4(int*, int, int);
    void func_8009E0A8(int*, int);
    int func_8009E284(int*, int);
    int* func_8009D790(int*, int);
    void func_8009DBF4(int*, int, int);
    void func_800B6800(float, void*, int);
    void* func_800B07E8__Fv();
    void func_800B1AF4(void*);
    void func_8007C360__Q22cf13CfGameManagerFv(float, int, int);
    void func_8007F830__Q22cf13CfGameManagerFv(int, int);
    void func_800823A4__Q22cf13CfGameManagerFv(int, int);
    void func_80083EA4__Q22cf13CfGameManagerFv();
    bool func_80084B68__Q22cf13CfGameManagerFv();
    void func_8008566C__Q22cf13CfGameManagerFv(float*, int, int);
    int func_80086B1C__Q22cf13CfGameManagerFv();
    void* func_80081CB8__Q22cf13CfGameManagerFv();
    void func_800AB978(float, float*, float*);
    void func_800AB8CC(float, float*, float*);
    void func_800ABA18(float, float*, float*);
    void func_800AC30C(int, int, const char*);
    void func_80082254__Q22cf13CfGameManagerFv();
    void func_80086DAC__Q22cf13CfGameManagerFv();
    void func_80082258__Q22cf13CfGameManagerFv();
    void func_80080888__Q22cf13CfGameManagerFv(int, int);
    void func_80085E58__Q22cf13CfGameManagerFv(int, int);
    void func_80085FB8__Q22cf13CfGameManagerFv(int);
    void func_800B946C();
    void func_800B98C8(int);
    void func_80083888__Q22cf13CfGameManagerFv(const char*);
    bool func_80084BF4__Q22cf13CfGameManagerFv();
    bool func_80087250__Q22cf13CfGameManagerFv();
    void func_800B94A0();
    void func_8009E740(void*, int);
    int func_8009E56C(void*, int, int);
    bool func_8009E344(void*, int, int*, int*);
    void func_8009E3C0();
    int  func_801586D4(int, int);
    void func_80159C04(int, int);
    int  func_80158068(int);
    void func_8007F1FC__Q22cf13CfGameManagerFv(void*, int);
    void func_800F4004(void*);
    void func_80462D04(int);
    void func_80462D5C(int);
    void func_804BC9EC__Fv();
    void func_804BCC30(int);
    void func_804BCC3C(int);
    void func_8047BD8C(void*, int);
    void func_8047BD94(void*, int);
    void* func_80083298__Q22cf13CfGameManagerFv();
}

// ============================================================================
// Batch 1: Plugin functions (pluginCfs.cpp)
// ============================================================================

// --- setMapJumpArea (us-8004785c) ---
int setMapJumpArea(VMThread* pThread) {
    int args[15];
    int v22, v20, v17, v21, v16, v14_int;
    
    // Read args 1-15: vmArgIntGet(2, ptr) for arg1, vmArgFixedGet for args 3-15
    // Arg 1: int
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        args[0] = vmArgIntGet(2, arg);
    }
    // Args 2-15: each is vmArgFixedGet via vmArgPtrGet
    for (int i = 1; i < 15; i++) {
        VMArg* arg = vmArgPtrGet(pThread, i + 1);
        if (i < 13) {
            args[i] = vmArgFixedGet(i + 2, arg);
        } else {
            args[i] = vmArgFixedGet(i + 2, arg);
        }
    }
    
    // Arg 16: optional int
    if (vmArgOmitChk(pThread, 16)) {
        v22 = 0x168;
        v20 = 17;
    } else {
        v20 = 17;
        VMArg* arg = vmArgPtrGet(pThread, 16);
        v22 = vmArgIntGet(17, arg);
    }
    
    // Arg 17: optional string
    v17 = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v17 = (int)vmArgStringGet(v20, arg);
    } else {
        v20++;
    }
    
    // Arg 18: optional int
    v21 = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v21 = vmArgIntGet(v20, arg);
    } else {
        v20++;
    }
    
    // Arg 19: optional int
    v16 = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v16 = vmArgIntGet(v20, arg);
    } else {
        v20++;
    }
    
    // Arg 20: optional int
    v14_int = 0;
    if (!vmArgOmitChk(pThread, v20)) {
        VMArg* arg = vmArgPtrGet(pThread, v20);
        v20++;
        v14_int = vmArgIntGet(v20, arg);
    }
    
    // Convert fixed-point args (3-15) to floats
    float v3 = (float)(s32)args[2] / 4096.0f;
    float v4 = (float)(s32)args[3] / 4096.0f;
    float v5 = (float)(s32)args[4] / 4096.0f;
    float v6 = (float)(s32)args[5] / 4096.0f;
    float v7 = (float)(s32)args[6] / 4096.0f;
    float v8 = (float)(s32)args[7] / 4096.0f;
    float v9 = (float)(s32)args[8] / 4096.0f;
    float v10 = (float)(s32)args[9] / 4096.0f;
    float v11 = (float)(s32)args[10] / 4096.0f;
    float v12 = (float)(s32)args[11] / 4096.0f;
    float v13 = (float)(s32)args[12] / 4096.0f;
    float v14f = (float)(s32)args[13] / 4096.0f;
    float v15f = (float)(s32)args[14] / 4096.0f;
    
    float t1[3] = {v3, v4, v5};
    float t2[3] = {v6, v7, v8};
    float t3[3] = {v9, v10, v11};
    float t4[3] = {v12, v13, v14f};
    
    void* gm = func_80081CB8__Q22cf13CfGameManagerFv();
    if (gm) {
        func_800AB978(v15f, t4, t3);
        func_800AB8CC(v15f, t1, t2);
        
        float sum[3];
        sum[0] = t1[0] + t2[0];
        sum[1] = t1[1] + t2[1];
        sum[2] = t1[2] + t2[2];
        
        float scaled[3];
        scaled[0] = sum[0] * lbl_eu_80665E38;
        scaled[1] = sum[1] * lbl_eu_80665E38;
        scaled[2] = sum[2] * lbl_eu_80665E38;
        
        func_800AC30C(args[0], args[1], (const char*)v17);
        
        int neg = -v21;
        int orVal = neg | v21;
        int boolVal = (unsigned int)orVal >> 31;
        func_8007C360__Q22cf13CfGameManagerFv((float)(s32)v22, boolVal, v16);
    }
    
    func_8007C360__Q22cf13CfGameManagerFv(1.0f, v14_int, v16);
    return 0;
}

void func_80047814__Q22cf13CfObjectPointFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

// --- setMapJumpAreaBox (us-80047e18) ---
int setMapJumpAreaBox(VMThread* pThread) {
    int args[12];
    int v28, v15, v27, v26, v25;
    
    // Args 1-2: ints
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        args[0] = vmArgIntGet(2, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 2);
        args[1] = vmArgIntGet(3, arg);
    }
    // Args 3-11: fixed-point
    for (int i = 2; i < 11; i++) {
        VMArg* arg = vmArgPtrGet(pThread, i + 1);
        args[i] = vmArgFixedGet(i + 2, arg);
    }
    
    // Arg 12: optional int
    if (vmArgOmitChk(pThread, 12)) {
        v28 = 0x168;
        v15 = 13;
    } else {
        v15 = 13;
        VMArg* arg = vmArgPtrGet(pThread, 12);
        v28 = vmArgIntGet(13, arg);
    }
    
    // Arg 13: optional string
    v27 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v27 = (int)vmArgStringGet(v15, arg);
    } else {
        v15++;
    }
    
    // Arg 14: optional int
    v26 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v26 = vmArgIntGet(v15, arg);
    } else {
        v15++;
    }
    
    // Arg 15: optional int
    v25 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v25 = vmArgIntGet(v15, arg);
    } else {
        v15++;
    }
    
    // Arg 16: optional int
    int v16 = 0;
    if (!vmArgOmitChk(pThread, v15)) {
        VMArg* arg = vmArgPtrGet(pThread, v15);
        v15++;
        v16 = vmArgIntGet(v15, arg);
    }
    
    // Convert fixed-point to floats
    float f1 = (float)(s32)args[2] / 4096.0f;
    float f2 = (float)(s32)args[3] / 4096.0f;
    float f3 = (float)(s32)args[4] / 4096.0f;
    float f4 = (float)(s32)args[5] / 4096.0f;
    float f5 = (float)(s32)args[6] / 4096.0f;
    float f6 = (float)(s32)args[7] / 4096.0f;
    float f7 = (float)(s32)args[8] / 4096.0f;
    float f8 = (float)(s32)args[9] / 4096.0f;
    float f9 = (float)(s32)args[10] / 4096.0f;
    
    float tA[3] = {f1, f2, f3};
    float tB[3] = {f4, f5, f6};
    float tC[3] = {f7, f8, f9};
    
    void* gm = func_80081CB8__Q22cf13CfGameManagerFv();
    if (gm) {
        func_800AB978(lbl_eu_80665E48, tB, tB);
        func_800ABA18(0.0f, tA, tC);
        func_800AC30C(args[0], args[1], (const char*)v27);
        
        int neg = -v26;
        int orVal = neg | v26;
        int boolVal = (unsigned int)orVal >> 31;
        func_8007C360__Q22cf13CfGameManagerFv((float)(s32)v28, boolVal, v25);
    }
    
    func_8007C360__Q22cf13CfGameManagerFv(1.0f, v25, v16);
    return 0;
}

// --- setWarpArea (us-80048238) ---
int setWarpArea(VMThread* pThread) {
    return 0;
}

// --- setMapPreloadArea (us-800487dc) ---
int setMapPreloadArea(VMThread* pThread) {
    return 0;
}

// --- mapJump (us-80048a5c) ---
int mapJump(VMThread* pThread) {
    return 0;
}

// --- setMapPreloadArea2 (us-80048c84) ---
int setMapPreloadArea2(VMThread* pThread) {
    return 0;
}

// --- setEventArea (us-80048ecc) ---
int setEventArea(VMThread* pThread) {
    return 0;
}

// --- delEventArea (us-80049130) ---
int delEventArea(VMThread* pThread) {
    const char* str = 0;
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        str = vmArgStringGet(2, arg);
    }
    func_800B94A0();
    return 0;
}

// --- setTownArea (us-80049190) ---
int setTownArea(VMThread* pThread) {
    return 0;
}

// --- addPopID (us-8004953c) ---
int addPopID(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgIntGet(2, arg);
    func_80082254__Q22cf13CfGameManagerFv();
    return 0;
}

// --- setTimeSpeed (us-80049574) ---
int setTimeSpeed(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgIntGet(2, arg);
    func_80086DAC__Q22cf13CfGameManagerFv();
    return 0;
}

// --- setPopSheet (us-800495ac) ---
int setPopSheet(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgStringGet(2, arg);
    func_80082258__Q22cf13CfGameManagerFv();
    return 0;
}

// --- changeWalker (us-800495e4) ---
int changeWalker(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    func_80080888__Q22cf13CfGameManagerFv(val & 0xFFFF, 1);
    return 0;
}

// --- eventStart (us-80049624) ---
int eventStart(VMThread* pThread) {
    int v31, v30;
    
    if (vmArgOmitChk(pThread, 1)) {
        v31 = 0;
        v30 = 2;
    } else {
        v30 = 2;
        VMArg* arg = vmArgPtrGet(pThread, 1);
        v31 = vmArgIntGet(2, arg);
    }
    
    int val;
    if (vmArgOmitChk(pThread, v30)) {
        val = 0;
    } else {
        VMArg* arg = vmArgPtrGet(pThread, v30);
        v30++;
        val = vmArgIntGet(v30, arg);
    }
    
    func_80085E58__Q22cf13CfGameManagerFv(v31, val);
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    func_800F4004(bm);
    return 0;
}

unsigned long battleEventStart() {
    lbl_eu_80663E24 |= 0x40000;
    return 0;
}

// --- eventEnd (us-800496fc) ---
int eventEnd(VMThread* pThread) {
    int val = 0;
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        val = vmArgIntGet(2, arg);
    }
    func_80085FB8__Q22cf13CfGameManagerFv(val);
    return 0;
}

// --- delAttr (us-8004975c) ---
int delAttr(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    vmArgIntGet(2, arg);
    func_800B946C();
    return 0;
}

// --- setMono (us-80049794) ---
int setMono(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgBoolGet(2, arg);
    int neg = -val;
    int orVal = neg | val;
    func_800B98C8((unsigned int)orVal >> 31);
    return 0;
}

// --- setMapDispID (us-800497d8) ---
int setMapDispID(VMThread* pThread) {
    int v26, v27, v28, v29, v30;
    
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        v26 = vmArgIntGet(2, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 2);
        v27 = vmArgBoolGet(3, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 3);
        v28 = vmArgBoolGet(4, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 4);
        v29 = vmArgBoolGet(5, arg);
    }
    {
        VMArg* arg = vmArgPtrGet(pThread, 5);
        v30 = vmArgBoolGet(6, arg);
    }
    
    void* gm = func_80083298__Q22cf13CfGameManagerFv();
    if (gm) {
        if (v27) {
            if (v28) func_80462D04(v26);
            if (v29) { func_804BC9EC__Fv(); func_804BCC30(v26); }
            if (v30) func_8047BD8C((u8*)gm + 0xF0, v26);
        } else {
            if (v28) func_80462D5C(v26);
            if (v29) { func_804BC9EC__Fv(); func_804BCC3C(v26); }
            if (v30) func_8047BD94((u8*)gm + 0xF0, v26);
        }
    }
    return 0;
}

// --- loadCfEvent (us-80049918) ---
int loadCfEvent(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg);
    func_80083888__Q22cf13CfGameManagerFv(str);
    return 0;
}

// --- waitCfEvent (us-80049950) ---
int waitCfEvent(VMThread* pThread) {
    if (func_80084BF4__Q22cf13CfGameManagerFv()) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// --- isMainParty (us-80049990) ---
int isMainParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    
    int firstOut = 0;
    int secondOut = -1;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, val, &firstOut, &secondOut);
    
    int result = firstOut;
    int sub = result - 1;
    int neg = -sub;
    int orVal = neg | sub;
    int boolVal = ((unsigned int)orVal >> 31) + 1;
    
    VMArg ret;
    ret.type = 1;
    ret.value.intVal = boolVal;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- isResvParty (us-80049a28) ---
int isResvParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    
    int firstOut = 0;
    int secondOut = -1;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, val, &firstOut, &secondOut);
    
    int result = firstOut;
    int sub = result - 2;
    int neg = -sub;
    int orVal = neg | sub;
    int boolVal = ((unsigned int)orVal >> 31) + 1;
    
    VMArg ret;
    ret.type = 1;
    ret.value.intVal = boolVal;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- addParty (us-80049ac0) ---
int addParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int v31 = vmArgIntGet(2, arg);
    
    int firstOut = 0;
    int secondOut = -1;
    int* data = func_8009ECB0();
    func_8009E344((void*)data, v31, &firstOut, &secondOut);
    
    int count = firstOut;
    int sub = count - 2;
    int clz = __cntlzw(sub);
    int isEq = (unsigned int)clz >> 5;
    
    if (isEq != 1 && isEq != 2) {
        UnkClass_8009ECB0* data2 = (UnkClass_8009ECB0*)func_8009ECB0();
        int group = 0;
        
        if (v31 == 9)       group = 6;
        else if (v31 == 10) group = 7;
        else if (v31 == 6)  group = 9;
        else if (v31 == 7)  group = 10;
        else if (v31 == 12 || v31 == 13) group = 4;
        
        if (group) func_8009E740(data2, group);
        
        if (!func_8009E56C(data2, v31, 1)) {
            func_8009E56C(data2, v31, 2);
        }
    }
    
    func_800823A4__Q22cf13CfGameManagerFv(v31, 1);
    return 0;
}

// --- makeParty (us-80049bf8) ---
int makeParty(VMThread* pThread) {
    int v29, v28, v27, v26, v25, v24, v23, v30;
    
    {
        VMArg* arg = vmArgPtrGet(pThread, 1);
        v29 = vmArgIntGet(2, arg);
    }
    
    // Arg 2: optional
    if (vmArgOmitChk(pThread, 2)) { v28 = 0; v30 = 3; }
    else { v30 = 3; VMArg* arg = vmArgPtrGet(pThread, 2); v28 = vmArgIntGet(3, arg); }
    
    // Arg 3: optional
    if (vmArgOmitChk(pThread, v30)) { v27 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v27 = vmArgIntGet(v30, arg); }
    
    // Arg 4: optional
    if (vmArgOmitChk(pThread, v30)) { v26 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v26 = vmArgIntGet(v30, arg); }
    
    // Arg 5: optional
    if (vmArgOmitChk(pThread, v30)) { v25 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v25 = vmArgIntGet(v30, arg); }
    
    // Arg 6: optional
    if (vmArgOmitChk(pThread, v30)) { v24 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v24 = vmArgIntGet(v30, arg); }
    
    // Arg 7: optional
    if (vmArgOmitChk(pThread, v30)) { v23 = 0; v30++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; v23 = vmArgIntGet(v30, arg); }
    
    // Arg 8: optional bool
    int r30_bool;
    if (vmArgOmitChk(pThread, v30)) { r30_bool = 1; }
    else { VMArg* arg = vmArgPtrGet(pThread, v30); v30++; r30_bool = vmArgBoolGet(v30, arg); }
    
    // Build party data
    u16 pdata[9];
    memset(pdata, 0, sizeof(pdata));
    pdata[0] = v29 & 0xFFFF;
    pdata[1] = v28 & 0xFFFF;
    pdata[2] = v27 & 0xFFFF;
    pdata[3] = v26 & 0xFFFF;
    pdata[4] = v25 & 0xFFFF;
    pdata[5] = v24 & 0xFFFF;
    
    // Player positions
    float pos[3][3];
    float scale[3];
    for (int i = 0; i < 3; i++) {
        pos[i][0] = pos[i][1] = pos[i][2] = 0.0f;
        scale[i] = 0.0f;
        void* player = getPlayer__Q22cf13CfGameManagerFi(i);
        if (player) {
            void* ppos = (*(void*(*)(void*))(*(void**)player))(player);
            pos[i][0] = *(float*)((u8*)ppos + 0);
            pos[i][1] = *(float*)((u8*)ppos + 4);
            pos[i][2] = *(float*)((u8*)ppos + 8);
            scale[i] = (*(float(*)(void*))(*(void**)player))(player);
        }
    }
    
    func_8007F1FC__Q22cf13CfGameManagerFv(pdata, 1);
    
    if (r30_bool) {
        int* d = func_8009ECB0();
        func_80080888__Q22cf13CfGameManagerFv(d[1] & 0xFFFF, 1);
    }
    
    if (func_8009CF8C(0x20) == 1 && func_80087250__Q22cf13CfGameManagerFv()) {
        func_8012FAA8();
    }
    
    for (int i = 0; i < 3; i++) {
        void* player = getPlayer__Q22cf13CfGameManagerFi(i);
        if (player) {
            (*(void(*)(void*, float*))(*(void**)player))(player, pos[i]);
            (*(void(*)(void*, float))(*(void**)player))(player, scale[i]);
        }
    }
    
    return 0;
}

float cf::CfObject::CfObject_UnkVirtualFunc31() {
    return *(float*)((u8*)this + 0x4C);
}

// --- makeGuestParty (us-80049f84) ---
int makeGuestParty(VMThread* pThread) {
    int v30, v29, v31;
    
    if (vmArgOmitChk(pThread, 1)) { v30 = 0; v31 = 2; }
    else { v31 = 2; VMArg* arg = vmArgPtrGet(pThread, 1); v30 = vmArgIntGet(2, arg); }
    
    if (vmArgOmitChk(pThread, v31)) { v29 = 0; v31++; }
    else { VMArg* arg = vmArgPtrGet(pThread, v31); v31++; v29 = vmArgIntGet(v31, arg); }
    
    if (vmArgOmitChk(pThread, v31)) { v31 = 0; }
    else { VMArg* arg = vmArgPtrGet(pThread, v31); v31++; v31 = vmArgIntGet(v31, arg); }
    
    int* data = func_8009ECB0();
    data[1] = (data[1] & 0xFFFF0000) | (v30 & 0xFFFF);
    data[2] = (data[2] & 0xFFFF0000) | (v29 & 0xFFFF);
    data[3] = (data[3] & 0xFFFF0000) | (v31 & 0xFFFF);
    func_8009E3C0();
    return 0;
}

// --- delParty (us-8004a0a0) ---
int delParty(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int v31 = vmArgIntGet(2, arg);
    int* data = func_8009ECB0();
    func_8009E740(data, v31);
    return 0;
}

// --- addItem (us-8004a0ec) ---
int addItem(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    int result = func_801586D4(val & 0xFFFF, 1);
    
    int neg = -result;
    int orVal = neg | result;
    int boolVal = (unsigned int)orVal >> 31;
    int clz = __cntlzw(boolVal);
    
    VMArg ret;
    ret.type = 1;
    ret.value.intVal = ((unsigned int)clz >> 5) + 1;
    vmRetValSet(pThread, &ret);
    return 1;
}

// --- delItem (us-8004a160) ---
int delItem(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int v31 = vmArgIntGet(2, arg);
    
    int val = 1;
    if (!vmArgOmitChk(pThread, 2)) {
        VMArg* arg2 = vmArgPtrGet(pThread, 2);
        val = vmArgIntGet(3, arg2);
    }
    
    func_80159C04(v31 & 0xFFFF, val);
    return 0;
}

// --- totalItem (us-8004a1ec) ---
int totalItem(VMThread* pThread) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    int val = vmArgIntGet(2, arg);
    int count = func_80158068(val & 0xFFFF);
    
    VMArg ret;
    ret.type = 3;
    ret.value.intVal = count;
    vmRetValSet(pThread, &ret);
    return 1;
}

// ============================================================================
// Target functions to match (batch 2)
// ============================================================================

// --- equipItem (us-8004a24c) ---
int equipItem(VMThread* vmThread) {
    int charId;
    int slotIds[5] = {-1, -1, -1, -1, -1};
    int slotIdx = 1;

    // Get charId from arg 1
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        charId = vmArgIntGet(slotIdx, arg);
        slotIdx++;
    }

    // Read optional slot args (up to 5)
    for (int i = 0; i < 5; i++) {
        if (vmArgOmitChk(vmThread, slotIdx)) {
            slotIdx++;
        } else {
            VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
            slotIdx++;
            slotIds[i] = vmArgIntGet(slotIdx, arg);
            slotIdx++;
        }
    }

    // Early return if item limit reached
    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    // Align charId and get character data
    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* partyData = func_8009ECB0();
    int isNewItem = func_8009E284(partyData, charId & 0xFFFF);
    bool anyChanged = false;

    for (int slot = 0; slot <= 4; slot++) {
        int* slotData = func_8009D790(&charData[7], slot);
        int itemId = slotIds[slot];
        bool doAction = false;

        if (slotData != NULL) {
            int slotType = (*slotData) >> 20;
            if (itemId < 0) {
                doAction = true;
            } else if (itemId == slotType) {
                goto skip_slot;
            } else {
                CItem_initItemImplInstances();
                doAction = true;
                anyChanged = true;
            }
        } else {
            if (itemId < 0) {
                charData[slot + 1] = (charData[slot + 1] & 0xFFFF0000) | (itemId & 0xFFFF);
            }
            doAction = true;
            anyChanged = true;
        }

        if (!doAction) goto skip_slot;

        if (itemId) {
            int slotType;
            switch (slot) {
                case 0: slotType = 4; break;
                case 1: slotType = 5; break;
                case 2: slotType = 6; break;
                case 3: slotType = 7; break;
                case 4: slotType = 8; break;
            }

            if (charId >= 1 && charId <= 11) {
                void* invSlot = func_8015783C(slotType, charId, 0);
                if (itemId != 0) {
                    func_80158118(invSlot, itemId & 0xFFFF, 1);
                } else {
                    CItem_initItemImplInstances();
                }

                int newSlotIdx = slot + 1;
                for (int k = 0; k < 8; k++) {
                    void* slotRef = func_80157948(charId, k);
                    if ((signed char)(*(u8*)slotRef) == newSlotIdx) {
                        func_80159B40(charId, k, &slotIds[0]);
                    }
                }
            }
        }
        skip_slot:;
    }

    if (anyChanged) {
        func_800B70FC(charId, 0);
    }

    return 0;
}

// --- equipWeapon (us-8004a608) ---
int equipWeapon(VMThread* vmThread) {
    int charId;
    int weaponId;
    int slotIdx = 1;

    // Get charId
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        charId = vmArgIntGet(slotIdx, arg);
        slotIdx++;
    }

    // Get weaponId
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        weaponId = vmArgIntGet(slotIdx, arg);
        slotIdx++;
    }

    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* weaponSlot = func_8009D790(&charData[7], 5);

    if (weaponSlot != NULL) {
        int slotType = (*weaponSlot) >> 20;
        if (weaponId == slotType) {
            return 0;
        }

        CItem_initItemImplInstances();

        int newWeaponId = weaponId & 0xFFFF;
        int slotInfo;
        func_80158420(newWeaponId, &slotInfo, 1, &slotInfo);
        func_8009E0A8(charData, slotInfo);

        func_800B70FC(charId, 0);
        lbl_eu_80663E28 |= 0x08000000;
    } else {
        int* cd = func_8009EC9C(charId & 0xFFFF);
        func_8009E0C4(cd, 5, weaponId & 0xFFFF);

        if (charId >= 1 && charId <= 11) {
            void* invSlot = func_8015783C(2, charId, 0);
            if (weaponId != 0) {
                func_80158118(invSlot, weaponId & 0xFFFF, 1);
                func_80155A00(invSlot);
            } else {
                CItem_initItemImplInstances();
            }

            for (int k = 0; k < 8; k++) {
                void* slotRef = func_80157948(charId, k);
                if (*(u8*)slotRef == 6) {
                    func_80159B40(charId, k, &weaponId);
                }
            }
        }
    }

    return 0;
}

// --- getWeaponSlot (us-8004a7c8) ---
int getWeaponSlot(VMThread* vmThread) {
    int charId;
    int itemId = 0;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* slotData = func_8009D790(&charData[7], 5);

    if (slotData != NULL) {
        CItem_initItemImplInstances();
        // Virtual call omitted - placeholder
    } else if (charId >= 1 && charId <= 11) {
        void* invSlot = func_8015783C(2, charId, 0);
        int slotType = (*(int*)invSlot) >> 12 & 0xF;
        if (slotType == 2) {
            CItem_initItemImplInstances();
            // Virtual call omitted - placeholder
        }
    }

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = itemId;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- setWeaponSlot (us-8004a8d8) ---
int setWeaponSlot(VMThread* vmThread) {
    int charId;
    int itemId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        itemId = vmArgIntGet(3, arg);
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int* slotData = func_8009D790(&charData[7], 5);

    if (slotData != NULL) {
        CItem_initItemImplInstances();
    } else if (charId >= 1 && charId <= 11) {
        void* invSlot = func_8015783C(2, charId, 0);
        CItem_initItemImplInstances();
    }

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = itemId;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- waitEventRes (us-8004a9ec) ---
int waitEventRes(VMThread* vmThread) {
    int waitMode = 0;

    if (!func_80084B68__Q22cf13CfGameManagerFv()) {
        waitMode = 1;
    }

    if (waitMode) {
        vmWaitModeSet(vmThread);
    }

    return 0;
}

// --- setFade (us-8004aa44) ---
int setFade(VMThread* vmThread) {
    int fixedR, fixedG, fixedB, fixedA;
    int frames = 0;
    int slotIdx = 1;

    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedR = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedG = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedB = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }
    {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        slotIdx++;
        fixedA = vmArgFixedGet(slotIdx, arg);
        slotIdx++;
    }

    if (vmArgOmitChk(vmThread, slotIdx)) {
        frames = 0;
    } else {
        VMArg* arg = vmArgPtrGet(vmThread, slotIdx);
        frames = vmArgIntGet(slotIdx + 1, arg);
    }

    float fR = (float)(s32)fixedR / 4096.0f;
    float fG = (float)(s32)fixedG / 4096.0f;
    float fB = (float)(s32)fixedB / 4096.0f;
    float fA = (float)(s32)fixedA / 4096.0f;

    float color[4] = {fR, fG, fB, fA};
    func_8008566C__Q22cf13CfGameManagerFv(color, frames & 0xFFFF, 1);

    return 0;
}

// --- applyPcPrm (us-8004ab9c) ---
int applyPcPrm(VMThread* vmThread) {
    int charId;
    int param;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        param = vmArgIntGet(3, arg);
    }

    if (func_8009CF8C(0x3508)) {
        return 0;
    }

    func_8007F830__Q22cf13CfGameManagerFv(charId, param);
    func_800823A4__Q22cf13CfGameManagerFv(param, 0);
    return 0;
}

// --- setDispOffArea (us-8004ac34) ---
int setDispOffArea(VMThread* vmThread) {
    void* ocObj;
    int areaId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        ocObj = vmArgOCGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        areaId = vmArgFixedGet(3, arg);
    }

    void* obj = func_801864DC(*(void**)((u8*)ocObj + 4), 0);

    if (obj != NULL) {
        void* box = func_800B07E8__Fv();
        float fAreaId = (float)(s32)areaId / 4096.0f;
        func_800B6800(fAreaId, obj, 1);
    }

    return 0;
}

// --- setScheduleType (us-8004ace0) ---
int setScheduleType(VMThread* vmThread) {
    void* ocObj;
    bool enable;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        ocObj = vmArgOCGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        enable = vmArgBoolGet(3, arg) != 0;
    }

    void* obj = func_801864DC(*(void**)((u8*)ocObj + 4), 0);

    if (obj != NULL) {
        int flags = *(int*)((u8*)obj + 0x64);
        if (flags & 0x10) {
            func_800BE28C((int)obj, enable ? 1 : 0);
        }
    }

    return 0;
}

// --- setWeather (us-8004ad78) ---
int setWeather(VMThread* vmThread) {
    int weatherId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        weatherId = vmArgIntGet(2, arg);
    }

    func_8016DF34(weatherId);
    return 0;
}

// --- setWeatherArea (us-8004adb0) ---
int setWeatherArea(VMThread* vmThread) {
    int areaId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        areaId = vmArgIntGet(2, arg);
    }

    func_8016DF4C(areaId);
    return 0;
}

// --- setGimmick (us-8004ade8) ---
int setGimmick(VMThread* vmThread) {
    int gimmickId;
    int state;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        gimmickId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        state = vmArgIntGet(3, arg);
    }

    if (getUnk80664658()) {
        switch (state) {
            case 0: func_801F4AD4(gimmickId); break;
            case 1: func_801F4B68(gimmickId); break;
            case 2: func_801F4BFC(gimmickId); break;
        }
    }

    return 0;
}

// --- setElvGim (us-8004ae94) ---
int setElvGim(VMThread* vmThread) {
    int gimmickId;
    int value;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        gimmickId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        value = vmArgIntGet(3, arg);
    }

    if (getUnk80664658()) {
        func_801F4C90(gimmickId, value);
    }

    return 0;
}

// --- setActMapObj (us-8004af14) ---
int setActMapObj(VMThread* vmThread) {
    int mapObjId;
    int state;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        mapObjId = vmArgIntGet(2, arg);
    }

    {
        VMArg* arg = vmArgPtrGet(vmThread, 2);
        state = vmArgIntGet(3, arg);
    }

    void* mapObj = func_80186BC8((void*)(mapObjId & 0xFFFF));
    if (mapObj != NULL) {
        int* objPtr = (int*)((u8*)mapObj + 0xC4);
        if (*objPtr != 0) {
            func_800BE12C((int)mapObj, state, 0, -1, 1);
        }
    }

    return 0;
}

// --- getMapID (us-8004afac) ---
int getMapID(VMThread* vmThread) {
    int mapId = func_80086B1C__Q22cf13CfGameManagerFv();

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = mapId;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- clearGimmickJump (us-8004aff4) ---
int clearGimmickJump(VMThread* vmThread) {
    unsigned long flags = lbl_eu_80663E24;
    bool wasSet = (flags & 0x1000000) != 0;
    lbl_eu_80663E24 = flags & ~0x40000;

    if (wasSet) {
        func_80083EA4__Q22cf13CfGameManagerFv();
    }

    return 0;
}

// --- partyMember (us-8004b02c) ---
int partyMember(VMThread* vmThread) {
    int memberIdx;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        memberIdx = vmArgIntGet(2, arg);
    }

    int* partyData = func_8009ECB0();
    int offset = (memberIdx - 1) * 4;
    int memberVal = *(int*)((u8*)partyData + 4 + offset);
    bool present = (memberVal == 0) ? false : true;

    VMArg ret;
    ret.type = present ? 1 : 2;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- clearPartyGauge (us-8004b0a8) ---
int clearPartyGauge(VMThread* vmThread) {
    void* battleMgr = getInstance__Q22cf14CBattleManagerFv();
    if (battleMgr != NULL) {
        void* gauge = getInstance__Q22cf14CBattleManagerFv();
        func_8018C8F4((u8*)gauge + 0x194, 0);
    }
    return 0;
}

// --- waitPop (us-8004b0e4) ---
int waitPop(VMThread* vmThread) {
    if (!func_800B8D5C()) {
        if (!(lbl_eu_80663E28 & 0x1000000)) {
            lbl_eu_80663EDC = 0.0f;
        }
        lbl_eu_80663E28 |= 0x10;
        vmWaitModeSet(vmThread);
    } else {
        lbl_eu_80663E28 &= ~0x10;
    }

    return 0;
}

// --- partyWarp (us-8004b154) ---
int partyWarp(VMThread* vmThread) {
    for (int i = 1; i < 3; i++) {
        void* player = getPlayer__Q22cf13CfGameManagerFi(i);
        if (player != NULL) {
            void* vtable = *(void**)player;
            void* (*vfunc)(void*);
            vfunc = (void* (*)(void*))((void**)vtable)[0x110 / 4];
            void* task = vfunc(player);
            if (task != NULL) {
                func_80199678((u8*)task + 0x8C, 1);
            }
        }
    }
    return 0;
}

// --- setMoney (us-8004b1dc) ---
int setMoney(VMThread* vmThread) {
    int amount;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        amount = vmArgIntGet(2, arg);
    }

    func_80157184(amount);
    return 0;
}

// --- addMoney (us-8004b214) ---
int addMoney(VMThread* vmThread) {
    int amount;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        amount = vmArgIntGet(2, arg);
    }

    func_801571A8(amount);
    return 0;
}

// --- isTimeSkip (us-8004b24c) ---
int isTimeSkip(VMThread* vmThread) {
    unsigned long flags = lbl_eu_80663E28;
    int bit = (flags >> 5) & 1;
    int result = (bit == 0) ? 1 : 0;

    VMArg ret;
    ret.type = result;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- delHoldBox (us-8004b288) ---
int delHoldBox(VMThread* vmThread) {
    int charId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    for (int slot = 4; slot <= 8; slot++) {
        func_8015730C(charId, 0, slot);
    }

    func_8015730C(charId, 0, 2);

    for (int k = 0; k < 8; k++) {
        void* slotRef = func_80157948(charId, k);
        if ((signed char)(*(u8*)slotRef) != 0) {
            func_80159B40(charId, k, &charId);
        }
    }

    return 0;
}

// --- getWeaponID (us-8004b338) ---
int getWeaponID(VMThread* vmThread) {
    int charId;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        charId = vmArgIntGet(2, arg);
    }

    int* charData = func_8009EC9C(charId & 0xFFFF);
    int weaponId = *(u16*)((u8*)charData + 0x0C);

    VMArg ret;
    ret.type = 3;
    ret.value.intVal = weaponId;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- addItemLimit (us-8004b41c) ---
int addItemLimit(VMThread* vmThread) {
    int limit;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        limit = vmArgIntGet(2, arg);
    }

    func_8015796C(limit);
    return 0;
}

// --- setPcCtrl (us-8004b49c) ---
int setPcCtrl(VMThread* vmThread) {
    bool enable;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        enable = vmArgBoolGet(2, arg) != 0;
    }

    unsigned long flags = lbl_eu_80663E24;
    if (enable) {
        lbl_eu_80663E24 = flags | 0x2000;
    } else {
        lbl_eu_80663E24 = flags & ~0x2000;
    }

    return 0;
}

// --- saveNamedCount (us-8004b4fc) ---
int saveNamedCount(VMThread* vmThread) {
    int count = func_80291BF8();
    int total = count + func_8009CF8C(0x10A);

    int clamped;
    if (total < 0) {
        clamped = 0;
    } else if (total > 0x3E7) {
        clamped = 0x3E7;
    } else {
        clamped = total;
    }

    func_8009D018(0x10A, clamped);
    return 0;
}

// --- isPal (us-8004b55c) ---
int isPal(VMThread* vmThread) {
    int isPal = isTvFormatPal__9CDeviceVIFv();
    int result = (isPal == 0) ? 1 : 0;

    VMArg ret;
    ret.type = result;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// --- setIgnorePal (us-8004b5a8) ---
int setIgnorePal(VMThread* vmThread) {
    bool ignore;

    {
        VMArg* arg = vmArgPtrGet(vmThread, 1);
        ignore = vmArgBoolGet(2, arg) != 0;
    }

    int ignoreVal = ignore ? 1 : 0;
    func_8049AB50(lbl_eu_80663E14, ignoreVal);
    return 0;
}

// --- isVoiceJP (us-8004b5f4) ---
int isVoiceJP(VMThread* vmThread) {
    void* instance = getInstance__Class_80296898Fv();
    unsigned char lang = *(u8*)((u8*)instance + 0x1C);
    int result = (lang == 1) ? 1 : 0;

    VMArg ret;
    ret.type = result;
    vmRetValSet(vmThread, &ret);
    return 1;
}

// ============================================================================
// Already-matched functions (kept as-is)
// ============================================================================

bool func_8004A2E8() {
    return false;
}

void func_8004A400() {
    // Intentionally empty
}

void* CfObject_UnkVirtualFunc48__Q22cf12CfObjectMoveFv(void* self) {
    void* ptr = *(void**)((u8*)self + 0xC4);
    if (ptr == NULL) {
        return *(void**)((u8*)self + 0x6C0);
    }
    return *(void**)((u8*)ptr + 8);
}

int clearTbox() {
    void* ptr = func_800B07E8__Fv();
    if (ptr) func_800B1AF4(ptr);
    return 0;
}

int returnTitle() {
    func_80043628();
    return 0;
}

int dispLoading() {
    func_8004302C(1, 0);
    return 0;
}

int clearItemLimit() {
    func_801579A4();
    return 0;
}

int clearEquipGem() {
    func_800A3304();
    return 0;
}

unsigned long setFieldVision() {
    lbl_eu_80663E28 |= 0x20000000;
    return 0;
}

void pluginCfsRegist(){
    extern char lbl_eu_804FB040[];
    extern PluginFuncData lbl_eu_80525F98[];
    vmPluginRegist(&lbl_eu_804FB040[6], lbl_eu_80525F98);
}