// Auto-scaffolded catalog TU for kyoshin/cf/CfRes
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/cf/CfRes.cpp" line 4 "kyoshin/harness_catalog.hpp" */
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
/* end "kyoshin/harness_catalog.hpp" */

// LLM-HARNESS-BEGIN: us-80061f70
extern "C" void func_80061870() {}
// LLM-HARNESS-END: us-80061f70

// LLM-HARNESS-BEGIN: us-80062180
extern "C" void func_80061A80() {}
// LLM-HARNESS-END: us-80062180

// LLM-HARNESS-BEGIN: us-8006235c
extern "C" void func_80061C5C() {}
// LLM-HARNESS-END: us-8006235c

// LLM-HARNESS-BEGIN: us-8006242c
extern "C" void func_80061D2C() {}
// LLM-HARNESS-END: us-8006242c

// LLM-HARNESS-BEGIN: us-8006258c
extern "C" void func_80061E8C() {}
// LLM-HARNESS-END: us-8006258c

// LLM-HARNESS-BEGIN: us-800626d8
extern u32 lbl_eu_80663D80;
extern "C" int func_80061FD8() { return lbl_eu_80663D80; }
// LLM-HARNESS-END: us-800626d8

// LLM-HARNESS-BEGIN: us-800626e0
extern "C" bool func_80061FE0() { return false; }
// LLM-HARNESS-END: us-800626e0

// LLM-HARNESS-BEGIN: us-800626e8
extern "C" void func_80061FE8() {}
// LLM-HARNESS-END: us-800626e8

// LLM-HARNESS-BEGIN: us-800626fc
extern "C" void func_80061FFC() {}
// LLM-HARNESS-END: us-800626fc

// LLM-HARNESS-BEGIN: us-80062710
extern "C" void func_80062010() {}
// LLM-HARNESS-END: us-80062710

// LLM-HARNESS-BEGIN: us-80062728
extern u32 lbl_eu_80663D7C;
extern "C" int func_80062028() { return lbl_eu_80663D7C; }
// LLM-HARNESS-END: us-80062728

// LLM-HARNESS-BEGIN: us-80062730
extern "C" void func_80062030() {}
// LLM-HARNESS-END: us-80062730

// LLM-HARNESS-BEGIN: us-80062758
extern "C" void func_80062058() {}
// LLM-HARNESS-END: us-80062758

// LLM-HARNESS-BEGIN: us-80062780
extern "C" void func_80062080() {}
// LLM-HARNESS-END: us-80062780

// LLM-HARNESS-BEGIN: us-800627a4
extern "C" void func_800620A4() {}
// LLM-HARNESS-END: us-800627a4

// LLM-HARNESS-BEGIN: us-800627cc
extern "C" void func_800620CC() {}
// LLM-HARNESS-END: us-800627cc

// LLM-HARNESS-BEGIN: us-800627f0
extern "C" void func_800620F0() {}
// LLM-HARNESS-END: us-800627f0

// LLM-HARNESS-BEGIN: us-80062814
extern "C" void func_80062114() {}
// LLM-HARNESS-END: us-80062814

// LLM-HARNESS-BEGIN: us-800628a0
extern "C" void func_800621A0() {}
// LLM-HARNESS-END: us-800628a0

// LLM-HARNESS-BEGIN: us-800628f4
extern "C" void func_800621F4() {}
// LLM-HARNESS-END: us-800628f4

// LLM-HARNESS-BEGIN: us-80062ad0
extern "C" void func_800623D0() {}
// LLM-HARNESS-END: us-80062ad0

// LLM-HARNESS-BEGIN: us-80062adc
extern "C" void func_800623DC() {}
// LLM-HARNESS-END: us-80062adc

// LLM-HARNESS-BEGIN: us-80062b30
extern "C" void func_80062430() {}
// LLM-HARNESS-END: us-80062b30

// LLM-HARNESS-BEGIN: us-80062ba8
extern "C" void func_800624A8() {}
// LLM-HARNESS-END: us-80062ba8

// LLM-HARNESS-BEGIN: us-80062c1c
extern "C" u32 func_8006251C(void* self) { return ((u32)(uintptr_t)self >> 16) & 0x1F; }
// LLM-HARNESS-END: us-80062c1c

// LLM-HARNESS-BEGIN: us-80062c24
extern "C" u32 func_80062524(void* self) { return ((u32)(uintptr_t)self >> 16) & 0x1F; }
// LLM-HARNESS-END: us-80062c24

// LLM-HARNESS-BEGIN: us-80062c2c
extern "C" void func_8006252C() {}
// LLM-HARNESS-END: us-80062c2c

// LLM-HARNESS-BEGIN: us-80062ca0
extern "C" void func_800625A0() {}
// LLM-HARNESS-END: us-80062ca0

// LLM-HARNESS-BEGIN: us-80062d00
extern "C" void func_80062600() {}
// LLM-HARNESS-END: us-80062d00

// LLM-HARNESS-BEGIN: us-80062d48
extern "C" void func_80062648() {}
// LLM-HARNESS-END: us-80062d48

// LLM-HARNESS-BEGIN: us-80062d64
extern "C" void func_80062664() {}
// LLM-HARNESS-END: us-80062d64

// LLM-HARNESS-BEGIN: us-80062d80
extern "C" void func_80062680() {}
// LLM-HARNESS-END: us-80062d80

// LLM-HARNESS-BEGIN: us-80062df4
extern "C" void func_800626F4() {}
// LLM-HARNESS-END: us-80062df4

// LLM-HARNESS-BEGIN: us-80062e58
extern "C" void func_eu_80062E58() {}
// LLM-HARNESS-END: us-80062e58

// LLM-HARNESS-BEGIN: us-80062ecc
extern "C" void func_80062758() {}
// LLM-HARNESS-END: us-80062ecc

// LLM-HARNESS-BEGIN: us-80062f30
extern "C" void func_800627BC() {}
// LLM-HARNESS-END: us-80062f30

// LLM-HARNESS-BEGIN: us-80062f70
extern "C" void func_800627FC() {}
// LLM-HARNESS-END: us-80062f70

// LLM-HARNESS-BEGIN: us-80062fd4
extern "C" void func_80062860() {}
// LLM-HARNESS-END: us-80062fd4

// LLM-HARNESS-BEGIN: us-80063038
extern "C" void func_800628C4() {}
// LLM-HARNESS-END: us-80063038

// LLM-HARNESS-BEGIN: us-8006309c
extern "C" void func_80062928() {}
// LLM-HARNESS-END: us-8006309c

// LLM-HARNESS-BEGIN: us-80063104
extern "C" u32 func_80062990(void* self) { return *(u32*)((u8*)self + 0x18); }
// LLM-HARNESS-END: us-80063104

// LLM-HARNESS-BEGIN: us-8006310c
extern "C" void func_80062998() {}
// LLM-HARNESS-END: us-8006310c

// LLM-HARNESS-BEGIN: us-80063174
extern "C" void func_eu_80063174() {}
// LLM-HARNESS-END: us-80063174

// LLM-HARNESS-BEGIN: us-800631c4
extern "C" void func_80062A00() {}
// LLM-HARNESS-END: us-800631c4

// LLM-HARNESS-BEGIN: us-80063248
extern "C" void func_80062A84() {}
// LLM-HARNESS-END: us-80063248

// LLM-HARNESS-BEGIN: us-8006329c
extern "C" void func_80062AD8() {}
// LLM-HARNESS-END: us-8006329c

// LLM-HARNESS-BEGIN: us-80063300
extern "C" void func_80062B3C() {}
// LLM-HARNESS-END: us-80063300

// LLM-HARNESS-BEGIN: us-80063364
extern "C" void func_80062BA0() {}
// LLM-HARNESS-END: us-80063364

// LLM-HARNESS-BEGIN: us-80063370
extern "C" void func_80062BAC() {}
// LLM-HARNESS-END: us-80063370

// LLM-HARNESS-BEGIN: us-800633d4
extern "C" void func_80062C10() {}
// LLM-HARNESS-END: us-800633d4

// LLM-HARNESS-BEGIN: us-800633e0
extern "C" void func_80062C1C() {}
// LLM-HARNESS-END: us-800633e0

// LLM-HARNESS-BEGIN: us-800633ec
extern "C" void func_80062C28() {}
// LLM-HARNESS-END: us-800633ec

// LLM-HARNESS-BEGIN: us-80063444
extern "C" bool func_80062C80() { return false; }
// LLM-HARNESS-END: us-80063444

// LLM-HARNESS-BEGIN: us-8006344c
extern "C" void func_80062C88() {}
// LLM-HARNESS-END: us-8006344c

// LLM-HARNESS-BEGIN: us-80063494
extern "C" void func_80062CD0() {}
// LLM-HARNESS-END: us-80063494

// LLM-HARNESS-BEGIN: us-800634ac
extern "C" void func_80062CE4() {}
// LLM-HARNESS-END: us-800634ac

// LLM-HARNESS-BEGIN: us-800634f4
extern "C" void func_80062D2C() {}
// LLM-HARNESS-END: us-800634f4

// LLM-HARNESS-BEGIN: us-8006350c
extern "C" void func_80062D44() {}
// LLM-HARNESS-END: us-8006350c

// LLM-HARNESS-BEGIN: us-80063554
extern "C" void func_80062D8C() {}
// LLM-HARNESS-END: us-80063554

// LLM-HARNESS-BEGIN: us-8006356c
extern "C" void func_80062DA4() {}
// LLM-HARNESS-END: us-8006356c

// LLM-HARNESS-BEGIN: us-800635b4
extern "C" void func_80062DEC() {}
// LLM-HARNESS-END: us-800635b4

// LLM-HARNESS-BEGIN: us-800635cc
extern "C" void func_80062E04() {}
// LLM-HARNESS-END: us-800635cc

// LLM-HARNESS-BEGIN: us-80063614
extern "C" void func_80062E4C() {}
// LLM-HARNESS-END: us-80063614

// LLM-HARNESS-BEGIN: us-8006362c
extern "C" void func_80062E64() {}
// LLM-HARNESS-END: us-8006362c

// LLM-HARNESS-BEGIN: us-80063674
extern "C" void func_80062EAC() {}
// LLM-HARNESS-END: us-80063674

// LLM-HARNESS-BEGIN: us-8006368c
extern "C" void func_80062EC4() {}
// LLM-HARNESS-END: us-8006368c

// LLM-HARNESS-BEGIN: us-800636d8
extern "C" bool func_80062F10() { return false; }
// LLM-HARNESS-END: us-800636d8

// LLM-HARNESS-BEGIN: us-800636e0
extern "C" void func_80062F18() {}
// LLM-HARNESS-END: us-800636e0

// LLM-HARNESS-BEGIN: us-80063718
extern "C" bool func_80062F50() { return false; }
// LLM-HARNESS-END: us-80063718

// LLM-HARNESS-BEGIN: us-80063720
extern "C" bool func_80062F58() { return false; }
// LLM-HARNESS-END: us-80063720

// LLM-HARNESS-BEGIN: us-80063728
extern "C" void func_80062F60() {}
// LLM-HARNESS-END: us-80063728

// LLM-HARNESS-BEGIN: us-80063760
extern "C" bool func_80062F98() { return false; }
// LLM-HARNESS-END: us-80063760

// LLM-HARNESS-BEGIN: us-80063768
extern "C" bool func_80062FA0() { return false; }
// LLM-HARNESS-END: us-80063768

// LLM-HARNESS-BEGIN: us-80063770
extern "C" void func_80062FA8() {}
// LLM-HARNESS-END: us-80063770

// LLM-HARNESS-BEGIN: us-800637a8
extern "C" bool func_80062FE0() { return false; }
// LLM-HARNESS-END: us-800637a8

// LLM-HARNESS-BEGIN: us-800637b0
extern "C" bool func_80062FE8() { return false; }
// LLM-HARNESS-END: us-800637b0

// LLM-HARNESS-BEGIN: us-800637b8
extern "C" void func_80062FF0() {}
// LLM-HARNESS-END: us-800637b8

// LLM-HARNESS-BEGIN: us-800637f0
extern "C" bool func_80063028() { return false; }
// LLM-HARNESS-END: us-800637f0

// LLM-HARNESS-BEGIN: us-800637f8
extern "C" bool func_80063030() { return false; }
// LLM-HARNESS-END: us-800637f8

// LLM-HARNESS-BEGIN: us-80063800
extern "C" void func_80063038() {}
// LLM-HARNESS-END: us-80063800

// LLM-HARNESS-BEGIN: us-80063838
extern "C" bool func_80063070() { return false; }
// LLM-HARNESS-END: us-80063838

// LLM-HARNESS-BEGIN: us-80063840
extern "C" bool func_80063078() { return false; }
// LLM-HARNESS-END: us-80063840

// LLM-HARNESS-BEGIN: us-80063848
extern "C" void func_80063080() {}
// LLM-HARNESS-END: us-80063848

// LLM-HARNESS-BEGIN: us-80063880
extern "C" bool func_800630B8() { return false; }
// LLM-HARNESS-END: us-80063880

// LLM-HARNESS-BEGIN: us-80063888
extern "C" bool func_800630C0() { return false; }
// LLM-HARNESS-END: us-80063888

// LLM-HARNESS-BEGIN: us-80063890
extern "C" void func_800630C8() {}
// LLM-HARNESS-END: us-80063890

// LLM-HARNESS-BEGIN: us-800638c8
extern "C" bool func_80063100() { return false; }
// LLM-HARNESS-END: us-800638c8

// LLM-HARNESS-BEGIN: us-800638d0
extern "C" bool func_80063108() { return false; }
// LLM-HARNESS-END: us-800638d0

// LLM-HARNESS-BEGIN: us-800638d8
extern "C" bool func_80063110() { return false; }
// LLM-HARNESS-END: us-800638d8

// LLM-HARNESS-BEGIN: us-800638e0
extern "C" bool func_80063118() { return false; }
// LLM-HARNESS-END: us-800638e0

// LLM-HARNESS-BEGIN: us-800638e8
extern "C" void func_80063120() {}
// LLM-HARNESS-END: us-800638e8

// LLM-HARNESS-BEGIN: us-80063920
extern "C" bool func_80063158() { return false; }
// LLM-HARNESS-END: us-80063920

// LLM-HARNESS-BEGIN: us-80063928
extern "C" void func_80063160() {}
// LLM-HARNESS-END: us-80063928

// LLM-HARNESS-BEGIN: us-800639a4
extern "C" void func_800631DC() {}
// LLM-HARNESS-END: us-800639a4

// LLM-HARNESS-BEGIN: us-800639b8
extern "C" void func_800631F0() {}
// LLM-HARNESS-END: us-800639b8

// LLM-HARNESS-BEGIN: us-800639c4
extern "C" void func_800631FC() {}
// LLM-HARNESS-END: us-800639c4

// LLM-HARNESS-BEGIN: us-80063a40
extern "C" void func_80063278() {}
// LLM-HARNESS-END: us-80063a40

// LLM-HARNESS-BEGIN: us-80063a4c
extern "C" u32 func_80063284(void* self) { return *(u32*)((u8*)self + 0x0); }
// LLM-HARNESS-END: us-80063a4c

// LLM-HARNESS-BEGIN: us-80063a54
extern "C" void func_8006328C() {}
// LLM-HARNESS-END: us-80063a54

// LLM-HARNESS-BEGIN: us-80063ad8
extern "C" void func_80063310() {}
// LLM-HARNESS-END: us-80063ad8

// LLM-HARNESS-BEGIN: us-80063b5c
extern "C" void func_80063394() {}
// LLM-HARNESS-END: us-80063b5c

// LLM-HARNESS-BEGIN: us-80063c54
extern "C" u32 func_8006348C(void* self) { return *(u32*)((u8*)self + 0x4); }
// LLM-HARNESS-END: us-80063c54

// LLM-HARNESS-BEGIN: us-80063c5c
extern "C" void func_80063494(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }
// LLM-HARNESS-END: us-80063c5c

// LLM-HARNESS-BEGIN: us-80063c64
extern "C" void func_8006349C() {}
// LLM-HARNESS-END: us-80063c64

// LLM-HARNESS-BEGIN: us-80063d0c
extern "C" u32 func_80063544(void* self) { return *(u32*)((u8*)self + 0x8); }
// LLM-HARNESS-END: us-80063d0c

// LLM-HARNESS-BEGIN: us-80063d14
extern "C" void func_8006354C(void* self, u32 val) { *(u32*)((u8*)self + 0x4) = val; }
// LLM-HARNESS-END: us-80063d14

// LLM-HARNESS-BEGIN: us-80063d1c
extern "C" void func_80063554() {}
// LLM-HARNESS-END: us-80063d1c

// LLM-HARNESS-BEGIN: us-80063d28
extern "C" void func_80063560() {}
// LLM-HARNESS-END: us-80063d28

// LLM-HARNESS-BEGIN: us-80064070
extern "C" void func_800638A8() {}
// LLM-HARNESS-END: us-80064070

// LLM-HARNESS-BEGIN: us-8006407c
extern "C" void func_800638B4() {}
// LLM-HARNESS-END: us-8006407c

// LLM-HARNESS-BEGIN: us-800640c8
extern "C" void func_80063900() {}
// LLM-HARNESS-END: us-800640c8

// LLM-HARNESS-BEGIN: us-80064154
extern "C" void func_8006398C() {}
// LLM-HARNESS-END: us-80064154

// LLM-HARNESS-BEGIN: us-80064158
extern "C" void func_80063990() {}
// LLM-HARNESS-END: us-80064158

// LLM-HARNESS-BEGIN: us-8006415c
extern "C" void func_80063994() {}
// LLM-HARNESS-END: us-8006415c

// LLM-HARNESS-BEGIN: us-800641fc
extern "C" void func_80063A34() {}
// LLM-HARNESS-END: us-800641fc

// LLM-HARNESS-BEGIN: us-8006422c
extern "C" void func_80063A60() {}
// LLM-HARNESS-END: us-8006422c

// LLM-HARNESS-BEGIN: us-80064298
extern "C" void func_80063ACC() {}
// LLM-HARNESS-END: us-80064298

// LLM-HARNESS-BEGIN: us-8006429c
extern "C" void func_80063AD0() {}
// LLM-HARNESS-END: us-8006429c

// LLM-HARNESS-BEGIN: us-800643a8
extern "C" void func_80063BDC() {}
// LLM-HARNESS-END: us-800643a8

// LLM-HARNESS-BEGIN: us-800643bc
extern "C" void func_80063BF0() {}
// LLM-HARNESS-END: us-800643bc

// LLM-HARNESS-BEGIN: us-800643d0
extern "C" void func_80063C04() {}
// LLM-HARNESS-END: us-800643d0

// LLM-HARNESS-BEGIN: us-800643f8
extern "C" void func_80063C2C() {}
// LLM-HARNESS-END: us-800643f8

// LLM-HARNESS-BEGIN: us-800643fc
extern "C" void func_80063C30() {}
// LLM-HARNESS-END: us-800643fc

// LLM-HARNESS-BEGIN: us-8006441c
extern "C" void func_80063C50() {}
// LLM-HARNESS-END: us-8006441c

// LLM-HARNESS-BEGIN: us-80064444
extern "C" void func_80063C78() {}
// LLM-HARNESS-END: us-80064444

// LLM-HARNESS-BEGIN: us-80064448
extern "C" void func_80063C7C() {}
// LLM-HARNESS-END: us-80064448

// LLM-HARNESS-BEGIN: us-8006457c
extern "C" void func_80063DB0() {}
// LLM-HARNESS-END: us-8006457c

// LLM-HARNESS-BEGIN: us-800645a4
extern "C" void func_80063DD8() {}
// LLM-HARNESS-END: us-800645a4

// LLM-HARNESS-BEGIN: us-800645b0
extern "C" void func_80063DE4() {}
// LLM-HARNESS-END: us-800645b0

// LLM-HARNESS-BEGIN: us-800645bc
extern "C" void func_80063DF0() {}
// LLM-HARNESS-END: us-800645bc

// LLM-HARNESS-BEGIN: us-800645d4
extern "C" void func_80063E08() {}
// LLM-HARNESS-END: us-800645d4

// LLM-HARNESS-BEGIN: us-800645f4
extern "C" u32 func_80063E28(void* self) { return ((u32)(uintptr_t)self) >> 24; }
// LLM-HARNESS-END: us-800645f4

// LLM-HARNESS-BEGIN: us-800645fc
extern "C" void func_80063E30() {}
// LLM-HARNESS-END: us-800645fc

// LLM-HARNESS-BEGIN: us-800646e8
extern "C" void func_80063F1C() {}
// LLM-HARNESS-END: us-800646e8

// LLM-HARNESS-BEGIN: us-80064774
extern "C" void func_80063FA8() {}
// LLM-HARNESS-END: us-80064774

// LLM-HARNESS-BEGIN: us-800647e0
extern "C" void func_80064014() {}
// LLM-HARNESS-END: us-800647e0

// LLM-HARNESS-BEGIN: us-800648c0
extern "C" void func_800640F4() {}
// LLM-HARNESS-END: us-800648c0

// LLM-HARNESS-BEGIN: us-80064918
extern "C" void func_8006414C() {}
// LLM-HARNESS-END: us-80064918

// LLM-HARNESS-BEGIN: us-80064990
extern "C" u32 func_800641C4(void* self) { return (u32)(uintptr_t)self & 0xFFFFFF; }
// LLM-HARNESS-END: us-80064990

// LLM-HARNESS-BEGIN: us-80064998
extern "C" void func_800641CC() {}
// LLM-HARNESS-END: us-80064998

// LLM-HARNESS-BEGIN: us-80064b2c
extern "C" void func_80064360() {}
// LLM-HARNESS-END: us-80064b2c

// LLM-HARNESS-BEGIN: us-80064b3c
extern "C" void func_80064370() {}
// LLM-HARNESS-END: us-80064b3c

// LLM-HARNESS-BEGIN: us-80064b54
extern "C" bool func_80064388() { return false; }
// LLM-HARNESS-END: us-80064b54

// LLM-HARNESS-BEGIN: us-80064b5c
extern "C" void func_80064390() {}
// LLM-HARNESS-END: us-80064b5c

// LLM-HARNESS-BEGIN: us-80064b74
extern "C" bool func_800643A8() { return false; }
// LLM-HARNESS-END: us-80064b74

// LLM-HARNESS-BEGIN: us-80064b7c
extern "C" void func_800643B0() {}
// LLM-HARNESS-END: us-80064b7c

// LLM-HARNESS-BEGIN: us-80064b94
extern "C" bool func_800643C8() { return false; }
// LLM-HARNESS-END: us-80064b94

// LLM-HARNESS-BEGIN: us-80064b9c
extern "C" void func_800643D0() {}
// LLM-HARNESS-END: us-80064b9c

// LLM-HARNESS-BEGIN: us-80064bb4
extern "C" bool func_800643E8() { return false; }
// LLM-HARNESS-END: us-80064bb4

// LLM-HARNESS-BEGIN: us-80064bbc
extern "C" void func_800643F0() {}
// LLM-HARNESS-END: us-80064bbc

// LLM-HARNESS-BEGIN: us-80065134
extern "C" void func_80064968() {}
// LLM-HARNESS-END: us-80065134

// LLM-HARNESS-BEGIN: us-80065144
extern "C" void func_80064978() {}
// LLM-HARNESS-END: us-80065144

// LLM-HARNESS-BEGIN: us-80065150
extern "C" void func_80064984() {}
// LLM-HARNESS-END: us-80065150

// LLM-HARNESS-BEGIN: us-80065160
extern "C" void func_80064994() {}
// LLM-HARNESS-END: us-80065160

// LLM-HARNESS-BEGIN: us-80065180
extern "C" void func_800649B4() {}
// LLM-HARNESS-END: us-80065180

// LLM-HARNESS-BEGIN: us-80065190
extern u32 lbl_eu_80663E14;
extern "C" int func_800649C4() { return lbl_eu_80663E14; }
// LLM-HARNESS-END: us-80065190

// LLM-HARNESS-BEGIN: us-80065198
extern "C" void func_800649CC() {}
// LLM-HARNESS-END: us-80065198

// LLM-HARNESS-BEGIN: us-800651a8
extern u32 lbl_eu_80663E30;
extern "C" int func_800649DC() { return lbl_eu_80663E30; }
// LLM-HARNESS-END: us-800651a8

// LLM-HARNESS-BEGIN: us-800651b0
extern "C" void func_800649E4() {}
// LLM-HARNESS-END: us-800651b0

// LLM-HARNESS-BEGIN: us-800651c0
extern "C" void func_800649F4() {}
// LLM-HARNESS-END: us-800651c0

// LLM-HARNESS-BEGIN: us-800651d4
extern "C" void func_80064A08() {}
// LLM-HARNESS-END: us-800651d4

// LLM-HARNESS-BEGIN: us-800651ec
extern "C" void OnFileEvent__5CfResFP10CEventFile() {}
// LLM-HARNESS-END: us-800651ec

// LLM-HARNESS-BEGIN: us-80065238
extern "C" u32 func_80064A6C(void* self) { return *(u32*)((u8*)self + 0x4); }
// LLM-HARNESS-END: us-80065238

// LLM-HARNESS-BEGIN: us-80065240
extern "C" void func_80064A74() {}
// LLM-HARNESS-END: us-80065240

// LLM-HARNESS-BEGIN: us-8006533c
extern "C" u32 func_80064B70(void* self) { return (u32)(uintptr_t)self & 0xFFFFFF; }
// LLM-HARNESS-END: us-8006533c

// LLM-HARNESS-BEGIN: us-80065344
extern "C" void func_80064B78() {}
// LLM-HARNESS-END: us-80065344

// LLM-HARNESS-BEGIN: us-80065484
extern "C" void func_80064CB8() {}
// LLM-HARNESS-END: us-80065484

// LLM-HARNESS-BEGIN: us-800654a4
extern "C" void func_80064CD8() {}
// LLM-HARNESS-END: us-800654a4

// LLM-HARNESS-BEGIN: us-80065590
extern "C" void func_eu_80065590() {}
// LLM-HARNESS-END: us-80065590

// LLM-HARNESS-BEGIN: us-80065640
extern "C" void func_eu_80065640() {}
// LLM-HARNESS-END: us-80065640

// LLM-HARNESS-BEGIN: us-8006565c
extern "C" void func_80064DC4() {}
// LLM-HARNESS-END: us-8006565c

// LLM-HARNESS-BEGIN: us-80065748
extern "C" void func_80064EB0() {}
// LLM-HARNESS-END: us-80065748

// LLM-HARNESS-BEGIN: us-800657f0
extern "C" u32 func_80064F58(void* self) { return *(u32*)((u8*)self + 0x18); }
// LLM-HARNESS-END: us-800657f0

// LLM-HARNESS-BEGIN: us-800657f8
extern "C" void func_80064F60() {}
// LLM-HARNESS-END: us-800657f8

// LLM-HARNESS-BEGIN: us-80065810
extern "C" void func_80064F78() {}
// LLM-HARNESS-END: us-80065810

// LLM-HARNESS-BEGIN: us-800658e8
extern "C" void func_80065050() {}
// LLM-HARNESS-END: us-800658e8

// LLM-HARNESS-BEGIN: us-800659f0
extern "C" void func_80065158() {}
// LLM-HARNESS-END: us-800659f0

// LLM-HARNESS-BEGIN: us-80065aec
extern "C" void func_80065254() {}
// LLM-HARNESS-END: us-80065aec

// LLM-HARNESS-BEGIN: us-80065b94
extern "C" void func_800652FC() {}
// LLM-HARNESS-END: us-80065b94

// LLM-HARNESS-BEGIN: us-80065bac
extern "C" void func_80065314() {}
// LLM-HARNESS-END: us-80065bac

// LLM-HARNESS-BEGIN: us-80065c7c
extern "C" void func_eu_80065C7C() {}
// LLM-HARNESS-END: us-80065c7c

// LLM-HARNESS-BEGIN: us-80065d60
extern "C" bool func_eu_80065D60() { return false; }
// LLM-HARNESS-END: us-80065d60

// LLM-HARNESS-BEGIN: us-80065d68
extern "C" void func_800653E4() {}
// LLM-HARNESS-END: us-80065d68

// LLM-HARNESS-BEGIN: us-80065e38
extern "C" void func_800654B4() {}
// LLM-HARNESS-END: us-80065e38

// LLM-HARNESS-BEGIN: us-80065f48
extern "C" void func_800655C4() {}
// LLM-HARNESS-END: us-80065f48

// LLM-HARNESS-BEGIN: us-80066018
extern "C" void func_80065694() {}
// LLM-HARNESS-END: us-80066018

// LLM-HARNESS-BEGIN: us-80066188
extern "C" void func_80065804() {}
// LLM-HARNESS-END: us-80066188

// LLM-HARNESS-BEGIN: us-8006619c
extern "C" void func_80065818() {}
// LLM-HARNESS-END: us-8006619c

// LLM-HARNESS-BEGIN: us-800661b8
extern "C" void func_80065834() {}
// LLM-HARNESS-END: us-800661b8

// LLM-HARNESS-BEGIN: us-80066224
extern "C" void __dt__Q22cf5CfResFv() {}
// LLM-HARNESS-END: us-80066224

// LLM-HARNESS-BEGIN: us-80066280
extern "C" void __dt__Q22cf9CfResTaskFv() {}
// LLM-HARNESS-END: us-80066280

// LLM-HARNESS-BEGIN: us-800662f8
extern "C" void Init__Q22cf9CfResTaskFv() {}
// LLM-HARNESS-END: us-800662f8

// LLM-HARNESS-BEGIN: us-800662fc
extern "C" void Term__Q22cf9CfResTaskFv() {}
// LLM-HARNESS-END: us-800662fc

// LLM-HARNESS-BEGIN: us-80066344
extern "C" void Move__Q22cf9CfResTaskFv() {}
// LLM-HARNESS-END: us-80066344

// LLM-HARNESS-BEGIN: us-80066348
extern "C" void __ct__Q22cf5CfResFv() {}
// LLM-HARNESS-END: us-80066348

// LLM-HARNESS-BEGIN: us-800664d4
extern "C" void Draw__Q22cf9CfResTaskFv() {}
// LLM-HARNESS-END: us-800664d4

// LLM-HARNESS-BEGIN: us-80066568
extern "C" void func_80065BE4() {}
// LLM-HARNESS-END: us-80066568

// LLM-HARNESS-BEGIN: us-800665cc
extern "C" void func_80065C48() {}
// LLM-HARNESS-END: us-800665cc

// LLM-HARNESS-BEGIN: us-800665e0
extern "C" void func_80065C5C() {}
// LLM-HARNESS-END: us-800665e0

// LLM-HARNESS-BEGIN: us-80066604
extern "C" void func_80065C80() {}
// LLM-HARNESS-END: us-80066604

// LLM-HARNESS-BEGIN: us-80066628
extern "C" void func_80065CA4() {}
// LLM-HARNESS-END: us-80066628

// LLM-HARNESS-BEGIN: us-80066684
extern "C" void func_80065D00() {}
// LLM-HARNESS-END: us-80066684

// LLM-HARNESS-BEGIN: us-80066688
extern "C" void func_80065D04() {}
// LLM-HARNESS-END: us-80066688

// LLM-HARNESS-BEGIN: us-8006668c
extern "C" void func_80065D08() {}
// LLM-HARNESS-END: us-8006668c

// LLM-HARNESS-BEGIN: us-80066690
extern "C" void func_80065D0C() {}
// LLM-HARNESS-END: us-80066690

// LLM-HARNESS-BEGIN: us-800666e8
extern "C" void func_80065D64() {}
// LLM-HARNESS-END: us-800666e8

// LLM-HARNESS-BEGIN: us-800666f8
extern "C" void func_80065D74() {}
// LLM-HARNESS-END: us-800666f8

// LLM-HARNESS-BEGIN: us-800666fc
extern "C" void func_80065D78() {}
// LLM-HARNESS-END: us-800666fc

// LLM-HARNESS-BEGIN: us-80066700
extern "C" void func_80065D7C() {}
// LLM-HARNESS-END: us-80066700

// LLM-HARNESS-BEGIN: us-80066704
extern "C" void func_80065D80() {}
// LLM-HARNESS-END: us-80066704

// LLM-HARNESS-BEGIN: us-80066708
extern "C" void func_80065D84() {}
// LLM-HARNESS-END: us-80066708

// LLM-HARNESS-BEGIN: us-8006670c
extern "C" void func_80065D88() {}
// LLM-HARNESS-END: us-8006670c

// LLM-HARNESS-BEGIN: us-80066710
extern "C" void func_80065D8C() {}
// LLM-HARNESS-END: us-80066710

// LLM-HARNESS-BEGIN: us-80066714
extern "C" void func_80065D90() {}
// LLM-HARNESS-END: us-80066714

// LLM-HARNESS-BEGIN: us-80066784
extern "C" bool func_80065E00() { return false; }
// LLM-HARNESS-END: us-80066784

// LLM-HARNESS-BEGIN: us-8006678c
extern "C" void func_80065E08() {}
// LLM-HARNESS-END: us-8006678c

// LLM-HARNESS-BEGIN: us-800667b0
extern "C" void func_80065E2C() {}
// LLM-HARNESS-END: us-800667b0

// LLM-HARNESS-BEGIN: us-800667d8
extern "C" void func_80065E54() {}
// LLM-HARNESS-END: us-800667d8

// LLM-HARNESS-BEGIN: us-8006689c
extern "C" void func_80065F18() {}
// LLM-HARNESS-END: us-8006689c

// LLM-HARNESS-BEGIN: us-800668a0
extern "C" void func_80065F1C() {}
// LLM-HARNESS-END: us-800668a0

// LLM-HARNESS-BEGIN: us-800668a4
extern "C" void func_80065F20() {}
// LLM-HARNESS-END: us-800668a4

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Local CTTask (out-of-line Move/Draw/dtor) for harness stubs.
// Do not include monolib/work/CTTask.hpp here — its inline methods collide.
template <typename T>
class CTTask {
public:
    CTTask();
    virtual ~CTTask();
    virtual void Move();
    virtual void Draw();
};

namespace cf { class CfResTask; }
// LLM-HARNESS-BEGIN: us-800661cc
template<> CTTask<cf::CfResTask>::~CTTask() {}
// LLM-HARNESS-END: us-800661cc
// LLM-HARNESS-BEGIN: us-800664d8
template<> void CTTask<cf::CfResTask>::Move() {}
// LLM-HARNESS-END: us-800664d8
// LLM-HARNESS-BEGIN: us-80066520
template<> void CTTask<cf::CfResTask>::Draw() {}
// LLM-HARNESS-END: us-80066520
