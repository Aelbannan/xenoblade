// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/code_800B06A4.cpp" line 4 "kyoshin/harness_catalog.hpp" */
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

// Forward declarations for functions called from extern "C" wrappers
void* func_800B07E8();
extern "C" u32 func_800AA2BC(u32 a, u32 b);

extern "C" void func_800B06A4(float a) {
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






extern "C" void func_800B0A90(void) {}




extern "C" void func_800B0B40(void* self) {
    *(unsigned long*)((char*)self + 0xb00) = 0;
    *(unsigned long*)((char*)self + 0xafc) = 0;
}

extern "C" void __ct__reslist_cf_TboxInfo() {}


extern "C" void __ct__reslist_cf_IFactoryEvent() {}

extern "C" void __dt__17UnkClass_805764CCFv() {}

extern "C" void func_800B0FA0() {}

extern "C" u32 func_800B0FEC(void* self) { return *(u32*)((u8*)self + 0x18); }

extern "C" void func_800B0FF4() {}

extern "C" void func_800B1120() {}

extern "C" void __ct__cf_CfValueItemManager() {}

extern "C" void func_800B1368(unsigned long flags) {
    extern unsigned long lbl_eu_80663EE0;
    lbl_eu_80663EE0 |= flags;
}

extern "C" void FactoryEvent3__Q22cf13IFactoryEventFv() {}

extern "C" void func_800B137C() {}

extern "C" void func_800B14C4(void* dst, void* src) {
    void* ptr = *(void**)((char*)src + 4);
    *(unsigned long*)dst = *(unsigned long*)ptr;
}

extern "C" void* func_800B14D4(void* self) {
    return (char*)(*(void**)self) + 8;
}

extern "C" void func_800B14E0() {}

extern "C" void func_800B14F0(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

extern "C" void func_800B14FC() {}

extern "C" void func_800B1518(void) {}

extern "C" void __dt__800B151C() {}

extern "C" void func_800B15A4() {}

extern u32 lbl_eu_80663F24;
extern "C" void func_800B1808() { lbl_eu_80663F24 = 0; }

extern "C" void FactoryEvent4__Q22cf13IFactoryEventFv() {}

extern "C" void func_800B1818(void* self, u32 val) { *(u32*)((u8*)self + 0x0) = val; }

extern "C" void func_800B1820(int* dst, int* src) {
    *dst = *src;
}

extern "C" void func_800B182C() {}

extern "C" void __dt__800B183C() {}

extern "C" void __dt__800B18CC() {}

extern "C" void func_800B1954() {}

extern "C" void func_800B1A5C() {}

extern "C" void func_800B1A8C() {}

extern "C" void* func_800B1A9C(void* self) {
    return (char*)(*(void**)self) + 8;
}

extern "C" void func_800B1AA8() {}

extern "C" void func_800B1AC0(int* dst, int* src) {
    *dst = *src;
}

extern "C" void func_800B1ACC(void* dst, void* src) {
    *(unsigned long*)dst = *(unsigned long*)((char*)src + 4);
}

extern "C" void func_800B1AD8() {}

extern "C" void func_800B1AF4() {}

extern "C" void func_800B1B2C() {}

extern "C" void func_800B1BBC() {}

extern "C" unsigned long func_800B1C00() {
    extern unsigned long lbl_eu_80663EE0;
    return (lbl_eu_80663EE0 >> 25) & 1;
}

extern "C" void func_800B1C0C() {}

extern "C" void func_800B1C24() {}

extern "C" void func_800B1C40() {}

extern "C" void func_800B1C78() {}

extern "C" void func_800B1CDC() {}

extern "C" void func_800B1E18() {}

extern "C" void func_800B1E2C() {}

extern "C" void func_800B1EB8() {}

extern "C" void func_800B1EC8() {}

extern "C" void func_800B1F2C() {}

extern "C" void func_800B1F40() {}

extern "C" void func_800B1F54() {}

extern "C" void func_800B1F6C() {}

extern "C" void func_800B1FD8() {}

extern "C" void func_800B2034() {}

extern "C" void func_800B2048() {}

extern "C" void func_800B205C() {}

extern "C" void func_800B20A0() {}

extern "C" void func_800B20B4() {}

extern "C" u32 func_800B2D28(void* self) { return *(u32*)((u8*)self + 0x4); }

extern "C" void __dt__800B2D30() {}

extern "C" void FactoryEvent1__Q22cf13IFactoryEventFv() {}

extern "C" void func_800B2D88(void* self, int param) {
    extern void func_800B2DB0(void*, void*);
    func_800B2DB0(self, &param);
}

extern "C" void func_800B2DB0() {}

extern "C" void func_800B2E38() {}

extern "C" void func_800B2ED0() {}

extern "C" int func_800B31B0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

extern "C" int func_800B31BC(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

extern "C" int func_800B31C8(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

extern "C" int func_800B31D4(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

extern "C" int func_800B31E0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

extern "C" int func_800B31EC(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

extern "C" int func_800B31F8(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}

extern "C" u32 func_800B3204(u32 val) { return (val >> 16) & 0xFF; }

extern "C" void func_800B3210() {}

extern "C" u32 func_800B39C0(void* self) { return *(u32*)((u8*)self + 0x74); }

extern "C" void func_800B39C8() {}

extern "C" void func_800B3A54(int* dst, int* src) {
    *dst = *src;
}

extern "C" void func_800B3A60(void* self, int param) {
    extern void func_800B99EC(void*, void*);
    func_800B99EC(self, &param);
}

extern "C" void func_800B3A88() {}

extern "C" u32 func_800B3D34(u32 val) { return val >> 24; }

extern "C" u32 func_800B3D40(u32 val) { return (val >> 16) & 0xFF; }

extern "C" void func_800B3D4C() {}

extern "C" void func_800B3D64() {}

extern "C" void func_800B4120() {}

extern "C" void func_800B4278() {}

extern "C" void func_800B42E8() {}

extern "C" void func_800B4368() {}

extern "C" void func_800B4400() {}

extern "C" void func_800B44A0() {}

extern "C" void func_800B4554() {}

extern "C" void func_800B4588(u32 val) {}

extern "C" u32 func_800B4594(u32 val) { return (val >> 16) & 0xFF; }

extern "C" void func_800B45A0() {}

extern "C" void func_800B47A8() {}

extern "C" void func_800B4A24() {}

extern "C" int CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void* self) { return 0; }

extern "C" void func_800B4B0C() {}

extern "C" void func_800B4B74() {}

extern "C" void func_800B4B88() {}

extern "C" void func_800B4CA0() {}

extern "C" void func_800B4D84() {}

extern "C" u32 func_800B4F58(u32 val) { return (val >> 16) & 0xFF; }

extern u16 lbl_eu_8066408C;
extern "C" unsigned short func_800B4F64()
{
    return lbl_eu_8066408C;
}

extern "C" void func_800B4F6C() {}

extern "C" void func_800B4F80() {}

extern "C" void func_800B4F90() {}

extern "C" u32 func_800B4FA4(void* self) { return *(u32*)((u8*)self + 0x70); }

extern "C" void func_800B4FAC() {}

extern "C" void func_800B535C() {}

extern "C" void func_800B570C() {}

extern "C" void func_800B587C() {}

extern "C" void func_800B5944() {}

extern "C" void func_800B5948() {}

extern "C" void func_800B5958(void* self) { *(u32*)((u8*)self + 896) = 0; }

extern "C" void* func_800B5964(void* self) { return 0; }

extern "C" u32 func_800B5970(void* self) { return *(u32*)((u8*)self + 0x380); }

extern "C" void func_800B5978() {}

extern "C" void func_800B5994() {}

extern "C" void func_800B6484() {}

extern "C" void func_800B6494() {}

extern "C" u32 func_800B64AC(u32 val) { return (val >> 16) & 0xFF; }

extern "C" void func_800B64B8() {}

extern "C" u32 func_800B64D0(u32 a, u32 b) { return a & b; }

extern "C" u32 func_800B64DC(void* self) { return *(u32*)((u8*)self + 0x98); }

extern "C" u16 func_800B64E4(void* self) { return *(u16*)((u8*)self + 0x45C0); }

extern "C" u32 func_800B64EC(u32 val) { return (val >> 16) & 0xFF; }

extern "C" void func_800B64F8() {}

extern "C" void func_800B6508() {}

extern "C" void func_800B6520(void* self, void* other) {
    unsigned int count = *(unsigned int*)((unsigned char*)self + 0x700);
    unsigned int val = *(unsigned int*)other;
    unsigned int newCount = count + 1;
    *(unsigned int*)((unsigned char*)self + 0x700) = newCount;
    float fval = *(float*)((unsigned char*)other + 4);
    void* dest = (unsigned char*)self + count * 8;
    *(unsigned int*)dest = val;
    *(float*)((unsigned char*)dest + 4) = fval;
}

extern "C" void func_800B6544() {}

extern "C" void func_800B655C() {}

extern "C" void func_800B66AC() {}

extern "C" void func_800B66BC() {}

extern "C" void func_800B67CC() {}

extern "C" void* func_800B67EC(void* self) { return (void*)((u8*)self + 0x1a8); }

extern "C" void func_800B67F4(void* self) { *(u32*)((u8*)self + 1792) = 0; }

extern "C" void func_800B6800() {}

extern "C" void func_800B68A8() {}

extern "C" void func_800B6AF4() {}

extern "C" void func_800B6BA0(void) {}

extern "C" void* func_800B6BA4__Fv() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB28;
}

extern "C" void* func_800B6BC8() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB48;
}

extern "C" void* func_800B6BEC() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB68;
}

extern "C" void* func_800B6C10() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xB88;
}

extern "C" void* func_800B6C34() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xBE8;
}

extern "C" void* func_800B6C58() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xBC8;
}

extern "C" void* func_800B6C7C() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xC08;
}

extern "C" void* func_800B6CA0() {
    extern void* func_800B07E8__Fv();
    return (unsigned char*)func_800B07E8__Fv() + 0xC48;
}

extern "C" void func_800B6CC4() {}

extern "C" void func_800B6CF8() {}

extern "C" void* func_800B6D3C(void*);

extern "C" void func_800B6DD0() {}

extern "C" void func_800B6EC0() {}

extern "C" u32 func_800B6FFC(void* self) { return *(u32*)((u8*)self + 0xB00); }

extern "C" u32 func_800B7004(void* self) { return *(u32*)((u8*)self + 0xB04); }

extern "C" void* func_800B700C(void* self, u32 index) {
    u32 start = *(u32*)((u8*)self + 0xAFC);
    u32 count = *(u32*)((u8*)self + 0xB04);
    u32* base = *(u32**)((u8*)self + 0xAF8);
    return &base[(start + index) % count];
}

extern "C" void func_800B7034() {
    func_800B6D3C(func_800B07E8());
}

extern "C" void func_800B7058() {}

extern "C" void func_800B708C__Fi() {}

extern "C" void func_800B70FC() {}

extern "C" u32 func_800B71C4(void) {
    extern u32 lbl_eu_806640F4;
    return lbl_eu_806640F4;
}

extern "C" void func_800B71CC() {}

extern "C" void* func_800B720C(void* self) { return (void*)((u8*)self + 0x720); }

extern "C" void func_800B7214() {}

extern "C" void func_800B72DC() {}

extern "C" void func_800B7320() {}

extern "C" void func_800B73E8(void* out_prev, void* unused, void* list_ptr) {
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

extern "C" void func_800B7410() {}

extern "C" unsigned short func_800B75B4()
{
    return lbl_eu_8066408C;
}

extern "C" unsigned short func_800B75BC()
{
    return lbl_eu_8066408C;
}

extern "C" void func_800B75C4(void* self, u32 mask, int enable) {
    u32* field = (u32*)((u8*)self + 0x6C);
    if (enable) {
        *field |= mask;
    } else {
        *field &= ~mask;
    }
}

extern "C" void func_800B75EC() {}

extern "C" int func_800B7680(void* self) {
    void* head = *(void**)((char*)self + 4);
    void* cur = *(void**)head;
    int count = 0;
    while (cur != head) {
        cur = *(void**)cur;
        count++;
    }
    return count;
}

extern "C" void func_800B76A4(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB28));
}

extern "C" void func_800B76CC(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB48));
}

extern "C" void func_800B76F4(void* self) {
    void* ptr = func_800B07E8();
    func_800B6D3C((void*)((u8*)ptr + 0xB68));
}

extern "C" void* func_800B771C(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xB88);
}

extern "C" void* func_800B7744(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBE8);
}

extern "C" void* func_800B776C(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBC8);
}

extern "C" void* func_800B7794(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xC08);
}

extern "C" void* func_800B77BC(void* self) {
    extern void* func_800B07E8__Fv();
    return func_800B6D3C((char*)func_800B07E8__Fv() + 0xBA8);
}

extern "C" void func_800B77E4() {}

extern "C" void func_800B781C() {}

extern "C" void func_800B7854() {}

extern "C" void func_800B788C(void* param_1) { extern void* func_800B07E8(); extern void func_800B6DD0(void*, void*); void* ptr = func_800B07E8(); func_800B6DD0((char*)ptr + 0xb88, param_1); }

extern "C" void func_800B78C4() {}

extern "C" void func_800B78FC() {}

extern "C" void func_800B7934() {}

extern "C" void func_800B796C() {}

extern "C" void func_800B79A4() {}

extern "C" void func_800B7A18() {}

extern "C" void func_800B7AF0() {}

extern "C" void func_800B83AC() {}

extern "C" void func_800B8524() {}

extern "C" void func_800B87FC() {}

extern "C" void func_800B8800() {}

extern "C" void func_800B8804__FPvPQ22cf13IFactoryEvent() {}

extern "C" void func_800B88E0() {}

extern "C" void func_800B8920() {}

extern "C" void func_800B89CC() {}

extern "C" void func_800B8A64() {}

extern "C" void func_800B8AFC() {}

extern "C" void func_800B8B94() {}

extern "C" void func_800B8C78() {}

extern "C" void func_800B8D5C() {}

extern "C" void func_800B8FC4() {}

extern "C" void func_800B92B4() {
    func_800B07E8();
    ((void(*)())func_800B1120)();
}

extern "C" void func_800B92D8() {
    func_800B07E8();
    ((void(*)())func_800B15A4)();
}

extern "C" void func_800B92FC() {}

extern "C" void func_800B9340() {
    func_800B07E8();
    ((void(*)())func_800B87FC)();
}

extern "C" void func_800B9364() {
    func_800B07E8();
    ((void(*)())func_800B8800)();
}

extern "C" void func_800B9388() {
    func_800B07E8();
    ((void(*)())func_800B1954)();
}

extern "C" void func_800B93AC() {
    func_800B07E8();
    ((void(*)())func_800B1B2C)();
}

extern "C" void func_800B93D0() {}

extern "C" void func_800B9404() {}

extern "C" void func_800B9438() {}

extern "C" void func_800B946C() {}

extern "C" void func_800B94A0() {}

extern "C" void func_800B94D4() {}

extern "C" void func_800B9524(u32 a, u32 b) {
    func_800AA2BC(a, b);
    ((void(*)())func_800B94D4)();
}

extern "C" void func_800B9548() {}

extern "C" void func_800B957C() {}

extern "C" void func_800B96C4(void* self, u32 val) { *(u32*)((u8*)self + 0x734) = val; }

extern "C" void func_800B96CC(void* self, u8 a, u8 b) { ((u8*)self)[1850] = a; ((u8*)self)[1851] = b; }

extern "C" void func_800B96D8(void* self, u32 val) { *(u32*)((u8*)self + 0x720) = val; }

extern "C" void func_800B96E0(void* self, u16 val) { *(u16*)((u8*)self + 0x738) = val; }

extern "C" void func_800B96E8() {}

extern "C" void func_800B9704(void* self, float val) { *(float*)((u8*)self + 0x730) = val; }

extern "C" void __ct__800B970C() {}

extern "C" void func_800B97A0() {}

extern "C" void func_800B985C() {}

extern "C" void func_800B98C8() {}

extern "C" u32 func_800B9984(void* self) { return *(u32*)((u8*)self + 0x620); }

extern "C" void func_800B998C() {}

extern "C" void func_800B99BC() {}

extern "C" void func_800B99EC() {}

extern "C" void func_800B9A30() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace cf { struct TboxInfo { char _pad; }; }
namespace cf { struct IFactoryEvent { char _pad; }; }
template <typename T> class _reslist_base { public: ~_reslist_base(); };
template <typename T> class reslist { public: ~reslist(); };
template <> _reslist_base<cf::TboxInfo>::~_reslist_base() {}
template <> reslist<cf::TboxInfo>::~reslist() {}
template <> _reslist_base<cf::IFactoryEvent*>::~_reslist_base() {}
template <> reslist<cf::IFactoryEvent*>::~reslist() {}
extern "C" void sinit_800B9A40() {}
