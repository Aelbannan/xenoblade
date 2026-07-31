// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/code_800B06A4.cpp" line 3 "kyoshin/harness_catalog.hpp" */
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
/* "src/kyoshin/code_800B06A4.cpp" line 4 "kyoshin/code_800B06A4.hpp" */
#ifndef KYOSHIN_CODE_800B06A4_HPP
#define KYOSHIN_CODE_800B06A4_HPP

/* "include/kyoshin/code_800B06A4.hpp" line 3 "types.h" */
/* end "types.h" */

namespace cf {
struct CfObject {
    char _pad[4];
};
struct TboxInfo {
    char _pad[4];
};
struct IFactoryEvent {
    char _pad[4];
};
}

namespace ml {
template <int N>
struct FixStr {
    char mString[N];
    int mLength;
    FixStr();
};
}

// Simple intrusive singly-linked list node (next pointer at offset 0)
struct SListNode {
    SListNode* next;
};

// Container holding an embedded sentinel pointer at offset 4
struct SList {
    u8 field_0x00[4];
    SListNode* sentinel;
};

template <typename T>
class _reslist_base {
public:
    ~_reslist_base();
    char _pad[0x1F];
};

template <typename T>
class reslist : public _reslist_base<T> {
public:
    ~reslist();
    char _pad2[0x20 - sizeof(_reslist_base<T>)];
};

class UnkClass_800B0AD8 {
public:
    UnkClass_800B0AD8();
    void clearCounters();
    u32 getCount();
    u32 getSize();
    void* getRingElem(u32 index);
    u8 unk0[0xAF8];                // 0x00
    UnkClass_800B0AD8* unkAF8;     // 0xAF8
    u32 unkAFC;                     // 0xAFC
    u32 unkB00;                     // 0xB00
    u32 unkB04;                     // 0xB04
    // total size: 0xB08
};

class UnkClass_805764CC : public reslist<cf::CfObject> {
public:
    UnkClass_805764CC();
    ~UnkClass_805764CC() {}
    static UnkClass_805764CC* func_800B07E8();

    u32 get_u32_18();
    u32 get_u32_04();
    u32 get_u32_74();
    u32 get_u32_70();
    u32 get_u32_380();
    u32 get_u32_98();
    u32 get_u32_620();
    u16 get_u16_45C0();
    void appendToBuffer(void* other);
    void clear_380();
    void clear_700();
    void fwdTo_2DB0(int param);
    void fwdTo_99EC(int param);
    void maskField_6C(u32 mask, int enable);
    void set_float_730(float val);
    void set_u16_738(u16 val);
    void set_u32_00(u32 val);
    void set_u32_720(u32 val);
    void set_u32_734(u32 val);
    void set_u8_73A(u8 a, u8 b);
    void* getNull();
    void* getPtr_1A8();
    void* getPtr_720();
    
    // 0x20 - 0xB27
    UnkClass_800B0AD8 field_0x20;
    // 0xB28 - 0xB47
    reslist<cf::CfObject> field_0xB28;
    // 0xB48 - 0xB67
    reslist<cf::CfObject> field_0xB48;
    // 0xB68 - 0xB87
    reslist<cf::CfObject> field_0xB68;
    // 0xB88 - 0xBA7
    reslist<cf::CfObject> field_0xB88;
    // 0xBA8 - 0xBC7
    reslist<cf::CfObject> field_0xBA8;
    // 0xBC8 - 0xBE7
    reslist<cf::CfObject> field_0xBC8;
    // 0xBE8 - 0xC07
    reslist<cf::CfObject> field_0xBE8;
    // 0xC08 - 0xC27
    reslist<cf::CfObject> field_0xC08;
    // 0xC28 - 0xC47
    reslist<cf::CfObject> field_0xC28;
    // 0xC48 - 0xC7F (0x38 bytes for reslist<cf::TboxInfo>)
    u8 field_0xC48[0x38];
    // 0xC80 - 0xC9F
    reslist<cf::IFactoryEvent*> field_0xC80;
    // 0xCA0 - 0xCAF
    u32 field_0xCA0;
    u32 field_0xCA4;
    u32 field_0xCA8;
    u32 field_0xCAC;
    // 0xCB0 - 0xCFB
    ml::FixStr<64> field_0xCB0;
    // 0xCFC - 0xD07
    u32 field_0xCFC;
    u32 field_0xD00;
    u32 field_0xD04;
    // 0xD08 - 0xD11
    u16 field_0xD0E;
    u16 field_0xD10;
    // 0xD12 - 0x15EF
    u8 _pad_D12_to_15EF[0x15F0 - 0xD12];
    // 0x15F0
    u32 field_0x15F0;
};

// Free functions defined in this translation unit
//void func_800B93AC();
//void func_800B1A5C(SList* listPtr);
//void func_800B998C(void* a, void* b, void* c, u32 d, void* e, void* f);
//void func_800B99BC(void* a, void* b, void* c, u32 d, void* e, void* f);
//void sinit_800B9A40();

#endif/* end "kyoshin/code_800B06A4.hpp" */

void UnkClass_800B0AD8::clearCounters() {
    unkB00 = 0;
    unkAFC = 0;
}
// Forward declarations for functions called from wrappers
// func_800B07E8 is declared as UnkClass_805764CC::func_800B07E8() in the header.
// Provide an alias for unqualified calls (sub_mainReset, sub_dispatchInit_*).
UnkClass_805764CC* func_800B07E8();
u32 func_800AA2BC(u32 a, u32 b);

// Forward declarations for functions called from this TU (defined in other units)
extern void func_80193810(u32);
extern void func_801F3CCC(u32);
extern void func_801F45B4(u32);
extern void func_802074F0(u32);

void gfloat_initStore(float a){
    extern float lbl_eu_80661CCC;
    extern float lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8;
    extern float lbl_eu_80663ECC;
    extern float lbl_eu_80663ED0;
    extern float lbl_eu_80663ED4;

    double ad = a;
    float sq = (float)ad;
    float c = lbl_eu_80661CD0;
    lbl_eu_80661CCC = a;
    sq = sq * sq;
    lbl_eu_80663EC8 = a;
    lbl_eu_80663ECC = sq;
    lbl_eu_80663ED0 = c;
    lbl_eu_80663ED4 = sq;
}

void init_0A90(void){}

void __ct__reslist_cf_TboxInfo(){}
void __ct__reslist_cf_IFactoryEvent(){}
void init_0FA0(){}

u32 UnkClass_805764CC::get_u32_18() { return *(u32*)((u8*)this + 0x18); }

void init_0FF4(){}
void init_dispatchTarget_1(){}
void __ct__cf_CfValueItemManager(){}

void gflag_setBits(unsigned long flags) {
    extern unsigned long lbl_eu_80663EE0;
    lbl_eu_80663EE0 |= flags;
}

void FactoryEvent3__Q22cf13IFactoryEventFv() {}

void init_137C(){}

void node_copyNextPtr(void* dst, void* src) {
    void* ptr = *(void**)((char*)src + 4);
    *(unsigned long*)dst = *(unsigned long*)ptr;
}

void* node_getDataPtr(void* self) {
    return (char*)(*(void**)self) + 8;
}

void init_14E0() {}

void node_copyNextU32(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

void init_14FC() {}

void __dt__800B151C();

extern "C" void func_800B1518() { void (*dtor)() = __dt__800B151C; dtor(); }

void __dt__800B151C(){}

void init_dispatchTarget_2(){}

extern u32 lbl_eu_80663F24;
void gvar_clearF24(){ lbl_eu_80663F24 = 0; }

void FactoryEvent4__Q22cf13IFactoryEventFv() {}

void UnkClass_805764CC::set_u32_00(u32 val) { *(u32*)((u8*)this + 0x0) = val; }

void copy_int_ptr(int* dst, int* src) {
    *dst = *src;
}

void init_182C() {}
void __dt__800B183C(){}
void __dt__800B18CC(){}
void init_dispatchTarget_5(){}
void init_1A5C(){}
void init_1A8C() {}

void* node_getDataPtr_alt(void* self) {
    return (char*)(*(void**)self) + 8;
}

void init_1AA8() {}

void copy_int_ptr_alt(int* dst, int* src) {
    *dst = *src;
}

void node_copyNextU32_alt(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

void init_1AD8() {}
void init_1AF4(){}
void init_dispatchTarget_6(){}
void init_1BBC(){}

unsigned long gflag_getBit25(){
    extern unsigned long lbl_eu_80663EE0;
    return (lbl_eu_80663EE0 >> 25) & 1;
}

void init_1C0C() {}
void init_1C24() {}
void init_1C40(){}
void init_1C78(){}
void init_1CDC(){}
void init_1E18(){}
void init_1E2C(){}
void init_1EB8() {}
void init_1EC8(){}
void init_1F2C(){}

// Load field_0xCA0; if non-zero, tail-call func_80193810
void func_800B1F40(UnkClass_805764CC* self) {
    if (self->field_0xCA0) {
        func_80193810(self->field_0xCA0);
    }
}

void init_1F54(){}
void init_1F6C(){}
void init_1FD8(){}

// Load field_0xCFC; if non-zero, tail-call func_801F3CCC
void func_800B2034(UnkClass_805764CC* self) {
    if (self->field_0xCFC) {
        func_801F3CCC(self->field_0xCFC);
    }
}

// Load field_0xCFC; if non-zero, tail-call func_801F45B4
void func_800B2048(UnkClass_805764CC* self) {
    if (self->field_0xCFC) {
        func_801F45B4(self->field_0xCFC);
    }
}

void init_205C(){}

// Load field_0xCAC; if non-zero, tail-call func_802074F0
void func_800B20A0(UnkClass_805764CC* self) {
    if (self->field_0xCAC) {
        func_802074F0(self->field_0xCAC);
    }
}

void init_20B4(){}

u32 UnkClass_805764CC::get_u32_04() { return *(u32*)((u8*)this + 0x4); }

void __dt__800B2D30(){}
void FactoryEvent1__Q22cf13IFactoryEventFv() {}

void UnkClass_805764CC::fwdTo_2DB0(int param) {
    extern void fwd_2DB0_body(void*, void*);
    fwd_2DB0_body(this, &param);
}

void fwd_2DB0_body(){}
void init_2E38(){}
void init_2ED0(){}

int obj_testBit_64_v0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
int obj_testBit_64_v1(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
int obj_testBit_64_v2(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
int obj_testBit_64_v3(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
int obj_testBit_64_v4(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
int obj_testBit_64_v5(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
int obj_testBit_64_v6(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

u32 shift_u32_hi8(u32 val) { return (val >> 16) & 0xFF; }

void init_3210(){}

u32 UnkClass_805764CC::get_u32_74() { return *(u32*)((u8*)this + 0x74); }

void init_39C8(){}

void copy_int_ptr_alt2(int* dst, int* src) {
    *dst = *src;
}

void UnkClass_805764CC::fwdTo_99EC(int param) {
    extern void fwd_99EC_body(void*, void*);
    fwd_99EC_body(this, &param);
}

void init_3A88(){}

u32 shift_u32_byte3(u32 val) { return val >> 24; }
u32 shift_u32_hi8_alt1(u32 val) { return (val >> 16) & 0xFF; }

void init_3D4C() {}
void init_3D64(){}
void init_4120(){}
void init_4278(){}
void init_42E8(){}
void init_4368(){}
void init_4400(){}
void init_44A0(){}
void init_4554(){}
void init_4588(u32 val) {}

u32 shift_u32_hi8_alt2(u32 val) { return (val >> 16) & 0xFF; }

void init_45A0(){}
void init_47A8(){}
void init_4A24(){}

int CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void* self) { return 0; }

void init_4B0C(){}

// Compare field_0x15F0 with val; returns 1 if equal, 0 otherwise
int func_800B4B74(UnkClass_805764CC* self, u32 val) {
    return (self->field_0x15F0 == val) ? 1 : 0;
}

void init_4B88(){}
void init_4CA0(){}
void init_4D84(){}

u32 shift_u32_hi8_alt3(u32 val) { return (val >> 16) & 0xFF; }

extern u16 lbl_eu_8066408C;
unsigned short gvar_get408C(){
    return lbl_eu_8066408C;
}

extern "C" u16 func_800B4F64() { extern u16 lbl_eu_80664314; return lbl_eu_80664314; }

void init_4F6C() {}
void init_4F80() {}
void init_4F90() {}

u32 UnkClass_805764CC::get_u32_70() { return *(u32*)((u8*)this + 0x70); }

void init_4FAC(){}
void init_535C(){}
void init_570C(){}
void init_587C(){}
void init_5944() {}
void init_5948() {}

void UnkClass_805764CC::clear_380() { *(u32*)((u8*)this + 896) = 0; }
void* UnkClass_805764CC::getNull() { return 0; }
u32 UnkClass_805764CC::get_u32_380() { return *(u32*)((u8*)this + 0x380); }

void init_5978(){}
void init_5994(){}
void init_6484() {}
void init_6494() {}

u32 shift_u32_hi8_alt4(u32 val) { return (val >> 16) & 0xFF; }

void init_64B8() {}

u32 shift_u32_and(u32 a, u32 b) { return a & b; }

u32 UnkClass_805764CC::get_u32_98() { return *(u32*)((u8*)this + 0x98); }
u16 UnkClass_805764CC::get_u16_45C0() { return *(u16*)((u8*)this + 0x45C0); }

u32 shift_u32_hi8_alt5(u32 val) { return (val >> 16) & 0xFF; }

void init_64F8() {}
void init_6508() {}

void UnkClass_805764CC::appendToBuffer(void* other) {
    unsigned int count = *(unsigned int*)((unsigned char*)this + 0x700);
    unsigned int val = *(unsigned int*)other;
    unsigned int newCount = count + 1;
    *(unsigned int*)((unsigned char*)this + 0x700) = newCount;
    float fval = *(float*)((unsigned char*)other + 4);
    void* dest = (unsigned char*)this + count * 8;
    *(unsigned int*)dest = val;
    *(float*)((unsigned char*)dest + 4) = fval;
}

void init_6544() {}
void init_655C(){}
void init_66AC() {}
void init_66BC(){}
void init_67CC(){}

void* UnkClass_805764CC::getPtr_1A8() { return (void*)((u8*)this + 0x1a8); }
void UnkClass_805764CC::clear_700() { *(u32*)((u8*)this + 1792) = 0; }

void init_6800(){}
void init_68A8(){}
void init_6AF4(){}

extern "C" void func_800B6BA0() {}

void* sub_getReslist_B28() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB28;
}
void* sub_getReslist_B48() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB48;
}
void* sub_getReslist_B68() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB68;
}
void* sub_getReslist_B88() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xB88;
}
void* sub_getReslist_BE8() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xBE8;
}
void* sub_getReslist_BC8() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xBC8;
}
void* sub_getReslist_C08() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xC08;
}
void* sub_getReslist_C48() {
    return &UnkClass_805764CC::func_800B07E8()->field_0xC48;
}

void init_6CC4(){}
void init_6CF8(){}

void* func_800B6D3C(void*);
void fwd_6DD0_body(){}

void init_6EC0(){}

u32 UnkClass_800B0AD8::getCount() { return *(u32*)((u8*)this + 0xB00); }
u32 UnkClass_800B0AD8::getSize() { return *(u32*)((u8*)this + 0xB04); }

void* UnkClass_800B0AD8::getRingElem(u32 index) {
    u32 start = *(u32*)((u8*)this + 0xAFC);
    u32 count = *(u32*)((u8*)this + 0xB04);
    u32* base = *(u32**)((u8*)this + 0xAF8);
    return &base[(start + index) % count];
}

void sub_mainReset() {
    func_800B6D3C(func_800B07E8());
}

void init_7058(){}
void init_708C(){}
void init_70FC(){}

u32 gvar_get40F4(void) {
    extern u32 lbl_eu_806640F4;
    return lbl_eu_806640F4;
}

void init_71CC(){}
void* UnkClass_805764CC::getPtr_720() { return (void*)((u8*)this + 0x720); }
void init_7214(){}
void init_72DC(){}
void init_7320(){}

void list_removeNode(void* out_prev, void* unused, void* list_ptr) {
    (void)unused;
    void* entry = *(void**)list_ptr;
    void* prev = *(void**)entry;
    void* next = *(void**)((char*)entry + 4);
    *(void**)next = prev;
    *(void**)((char*)prev + 4) = next;
    entry = *(void**)list_ptr;
    *(void**)entry = NULL;
    *(void**)out_prev = prev;
}

void init_7410(){}

unsigned short gvar_get408C_alt1(){
    return lbl_eu_8066408C;
}
unsigned short gvar_get408C_alt2(){
    return lbl_eu_8066408C;
}

void UnkClass_805764CC::maskField_6C(u32 mask, int enable) {
    u32* field = (u32*)((u8*)this + 0x6C);
    if (enable) {
        *field |= mask;
    } else {
        *field &= ~mask;
    }
}

extern "C" u16 func_800B75B4() { extern u16 lbl_eu_80663E42; return lbl_eu_80663E42; }
extern "C" u16 func_800B75BC() { extern u16 lbl_eu_80663E44; return lbl_eu_80663E44; }

void init_75EC(){}

int list_countNodes(void* self){
    void* head = *(void**)((char*)self + 4);
    void* cur = *(void**)head;
    int count = 0;
    while (cur != head) {
        cur = *(void**)cur;
        count++;
    }
    return count;
}

void sub_resetReslist_B28(void* self) {
    func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB28);
}
void sub_resetReslist_B48(void* self) {
    func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB48);
}
void sub_resetReslist_B68(void* self) {
    func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB68);
}
void* sub_resetReslist_B88(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB88);
}
void* sub_resetReslist_BE8(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBE8);
}
void* sub_resetReslist_BC8(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBC8);
}
void* sub_resetReslist_C08(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xC08);
}
void* sub_resetReslist_BA8(void* self) {
    return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBA8);
}

void init_77E4(){}
void init_781C(){}
void init_7854(){}

void sub_pushToReslist_B88(cf::CfObject* param_1) { extern void fwd_6DD0_body(void*, void*); fwd_6DD0_body(&UnkClass_805764CC::func_800B07E8()->field_0xB88, param_1); }

void init_78C4(){}
void init_78FC(){}
void init_7934(){}
void init_796C(){}
void init_79A4(){}
void init_7A18(){}
void init_7AF0(){}
void init_83AC(){}
void init_8524(){}
void init_dispatchTarget_3() {}
void init_dispatchTarget_4() {}
void init_8804(){}
void init_88E0(){}
void init_8920(){}
void init_89CC(){}
void init_8A64(){}
void init_8AFC(){}
void init_8B94(){}
void init_8C78(){}
void init_8D5C(){}
void init_8FC4(){}

void sub_dispatchInit_1() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_1)();
}
void sub_dispatchInit_2() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_2)();
}
void init_92FC(){}
void sub_dispatchInit_3() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_3)();
}
void sub_dispatchInit_4() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_4)();
}
void sub_dispatchInit_5() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_5)();
}
void sub_dispatchInit_6() {
    func_800B07E8();
    ((void(*)())init_dispatchTarget_6)();
}

void init_93D0(){}
void init_9404(){}
void init_9438(){}
void init_946C(){}
void init_94A0(){}
void init_dispatchTarget_7(){}

void sub_dispatchWithArgs(u32 a, u32 b) {
    func_800AA2BC(a, b);
    ((void(*)())init_dispatchTarget_7)();
}

void init_9548(){}
void init_957C(){}

void UnkClass_805764CC::set_u32_734(u32 val) { *(u32*)((u8*)this + 0x734) = val; }
void UnkClass_805764CC::set_u8_73A(u8 a, u8 b) { this->field_0x20.unk0[0x71A] = a; this->field_0x20.unk0[0x71B] = b; }
void UnkClass_805764CC::set_u32_720(u32 val) { *(u32*)((u8*)this + 0x720) = val; }
void UnkClass_805764CC::set_u16_738(u16 val) { *(u16*)((u8*)this + 0x738) = val; }

void init_96E8() {}
void UnkClass_805764CC::set_float_730(float val) { *(float*)((u8*)this + 0x730) = val; }
void __ct__800B970C(){}
void init_97A0(){}
void init_985C(){}
void init_98C8(){}

u32 UnkClass_805764CC::get_u32_620() { return *(u32*)((u8*)this + 0x620); }

void init_998C(){}
void init_99BC(){}
void fwd_99EC_body(){}
void init_9A30() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Definitions provided by include/kyoshin/code_800B06A4.hpp
template <> _reslist_base<cf::TboxInfo>::~_reslist_base() {}
template <> reslist<cf::TboxInfo>::~reslist() {}
template <> _reslist_base<cf::IFactoryEvent*>::~_reslist_base() {}
template <> reslist<cf::IFactoryEvent*>::~reslist() {}
void sinit_800B9A40(){}