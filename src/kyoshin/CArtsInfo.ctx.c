// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/CArtsInfo.cpp" line 3 "kyoshin/harness_catalog.hpp" */
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
/* "src/kyoshin/CArtsInfo.cpp" line 4 "kyoshin/CArtsInfo.hpp" */
#pragma once

/* "src/kyoshin/CArtsInfo.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/CArtsInfo.hpp" line 3 "monolib/lib/UnkClass_8045F564.hpp" */
#pragma once

/* "libs/monolib/include/monolib/lib/UnkClass_8045F564.hpp" line 2 "types.h" */
/* end "types.h" */

class UnkClass_8045F564{
public:
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;

    UnkClass_8045F564();
    ~UnkClass_8045F564();

    
    void createRegion(int, int, const char*, int);
    void func_8045F778();
    void func_8045F7E8();
    void func_8045F810();
};

class Class_8045F858{
public:
    void* unk0;
    u32 unk4;

    Class_8045F858(UnkClass_8045F564* unkClass);
    ~Class_8045F858();
};
/* end "monolib/lib/UnkClass_8045F564.hpp" */

namespace nw4r {
namespace lyt {
    class Layout;
    class AnimTransform;
    class ArcResourceAccessor;
}
}

class CArtsInfo {
public:
    CArtsInfo();
    virtual ~CArtsInfo();
    void OnFileEvent();
    u8 getField48();
    u8 getField49();
    void setField54(u8 val);
    void setField55(u8 val);
    void setField56(u8 val);
    void setField58(u16 val);
    u32 getField5A();
    int isField44GE6();
    void initialize();

    // vtable pointer at 0x00 (implicit)
    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ int field_0x14;  // file handle 1
    /* 0x18 */ int field_0x18;  // file handle 2
    /* 0x1C */ int field_0x1C;  // arc resource accessor
    /* 0x20 */ nw4r::lyt::Layout* mpLayout1;
    /* 0x24 */ nw4r::lyt::AnimTransform* mpAnimTrans1;
    /* 0x28 */ nw4r::lyt::AnimTransform* mpAnimTrans2;
    /* 0x2C */ nw4r::lyt::AnimTransform* mpAnimTrans3;
    /* 0x30 */ nw4r::lyt::AnimTransform* mpAnimTrans4;
    /* 0x34 */ nw4r::lyt::Layout* mpLayout2;
    /* 0x38 */ nw4r::lyt::AnimTransform* mpAnimTrans5;
    /* 0x3C */ nw4r::lyt::AnimTransform* mpAnimTrans6;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 _41[3];      // padding
    /* 0x44 */ int field_0x44;  // state machine
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    /* 0x4A */ u8 _4A[2];      // padding
    /* 0x4C */ int field_0x4C;
    /* 0x50 */ int field_0x50;
    /* 0x54 */ u8 field_0x54;
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ u8 field_0x56;
    /* 0x57 */ u8 _57;          // padding
    /* 0x58 */ u16 field_0x58;
    /* 0x5A */ s8 field_0x5A;
    /* 0x5B */ u8 _5B;          // padding
    /* 0x5C */ u8 mCursor[0x18]; // CCur18 embedded cursor
};
/* end "kyoshin/CArtsInfo.hpp" */

// Extern declarations for referenced symbols

// String table base (rodata, accessed via sda21 relocation)
extern char lbl_eu_8050B00C[];

// Vtable symbol for CArtsInfo
extern void* lbl_eu_80536A88;

// Jump table for state machine dispatch (func_8023587C)
extern void* jumptable_eu_805369A0[];

// Float constants in small data area
extern float lbl_eu_80668680; // 0.0f
extern float lbl_eu_80668684; // 1.0f

// Small data string pairs for func_802369C0
extern char lbl_eu_80664748[];
extern char lbl_eu_80664750[];

// External function declarations
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void func_8003AA8C__5CBdatFUl(u32);
extern "C" void func_801390E0__FPP11CFileHandle(void*);
extern "C" void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void func_8045F778__17UnkClass_8045F564Fv(void*);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__CCur18(void*, void*);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dl__FPv(void*);
extern "C" void func_80137038(void*, void*, int, int);
extern "C" u32 func_80137444(nw4r::lyt::AnimTransform*, float);
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_80136B4C(void*, const char*, void*, u32);
extern "C" void func_80136A1C(void*, const char*, void*, u32);
extern "C" void* func_80136190(const char*, const char*, int);
extern "C" void func_80138078__FUl(u32);
extern "C" u32 func_8013639C(u32, const char*, u32);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" u32 func_80139A18(void*, const char*, void*, void*);
extern "C" u32 func_80137924(void*, void*, void*, void*);
extern "C" u32 func_8009EC9C(u32);
extern "C" void func_801D202C(void*);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D216C(void*, int);

// sprintf is provided by the C runtime
extern "C" int sprintf(char*, const char*, ...);

// Forward declarations for state machine functions (defined later in this TU)
void func_80236508(CArtsInfo*);
void func_802369C0(CArtsInfo*);
void func_80236CF4(CArtsInfo*);

// Forward declarations for animation state handlers
void func_80235F6C(CArtsInfo*);
void func_80236020(CArtsInfo*);
void func_8023606C(CArtsInfo*);
void func_80236120(CArtsInfo*);
void func_8023616C(CArtsInfo*);
void func_80236220(CArtsInfo*);
void func_802362D4(CArtsInfo*);
void func_80236334(CArtsInfo*);
void func_80236408(CArtsInfo*);
void func_80236454(CArtsInfo*);

// Virtual method call helpers (offset 0x38 = Animate-like, offset 0x2C = BindAnim-like)
static inline void callVirt_38_0(nw4r::lyt::Layout* layout) {
    typedef void (*VirtFn)(void*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[14]; // 0x38 / 4 = 14
    fn(layout, 0);
}

static inline void callVirt_2C_50(nw4r::lyt::Layout* layout, void* arg1, u32 arg2) {
    typedef void (*VirtFn)(void*, void*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[11]; // 0x2C / 4 = 11
    fn(layout, arg1, arg2);
}

// Vtable virtual method at offset 0x08 with flags=1 (destructor)
static inline void callVirtDelete_08(void* obj) {
    typedef void (*VirtFn)(void*, int);
    VirtFn fn = ((VirtFn*)*(u32*)obj)[2]; // 0x08 / 4 = 2
    fn(obj, 1);
}

// Vtable virtual method at offset 0x0C
static inline void callVirt_0C(void* obj) {
    typedef void (*VirtFn)(void*);
    VirtFn fn = ((VirtFn*)*(u32*)obj)[3]; // 0x0C / 4 = 3
    fn(obj);
}

// Vtable virtual method at offset 0x10 with pointer arg
static inline void callVirt_10(void* obj, void* arg) {
    typedef void (*VirtFn)(void*, void*);
    VirtFn fn = ((VirtFn*)*(u32*)obj)[4]; // 0x10 / 4 = 4
    fn(obj, arg);
}

// Vtable virtual method at offset 0x3C with two args
static inline u32 callVirt_3C(nw4r::lyt::Layout* layout, const char* arg1, u32 arg2) {
    typedef u32 (*VirtFn)(void*, const char*, u32);
    VirtFn fn = ((VirtFn*)*(u32*)layout)[15]; // 0x3C / 4 = 15
    return fn(layout, arg1, arg2);
}

u8 CArtsInfo::getField48() { return field_0x48; }

u8 CArtsInfo::getField49() { return field_0x49; }

// __ct__CArtsInfo — constructor
// .text:0x0, size 0xAC
CArtsInfo::CArtsInfo() {
    // Set vtable pointer
    *(void**)this = &lbl_eu_80536A88;

    // Construct embedded UnkClass_8045F564 member
    __ct__17UnkClass_8045F564Fv(&mMemRegion);

    // Initialize all fields to zero/default
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x1C = 0;
    mpLayout1 = nullptr;
    mpAnimTrans1 = nullptr;
    mpAnimTrans2 = nullptr;
    mpAnimTrans3 = nullptr;
    mpAnimTrans4 = nullptr;
    mpLayout2 = nullptr;
    mpAnimTrans5 = nullptr;
    mpAnimTrans6 = nullptr;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x48 = 0;
    field_0x49 = 1;
    field_0x4C = 0;
    field_0x50 = 0;
    field_0x54 = 0;
    field_0x55 = 0;
    field_0x56 = 0;
    field_0x58 = 0;
    field_0x5A = 0;

    // Construct embedded CCur18 cursor
    __ct__CCur18(mCursor, nullptr);
}

// __dt__9CArtsInfoFv — destructor
// .text:0xAC, size 0x60
CArtsInfo::~CArtsInfo() {
    // Destroy CCur18 cursor
    __dt__6CCur18Fv(mCursor, -1);

    // Destroy UnkClass_8045F564 member
    __dt__17UnkClass_8045F564Fv(&mMemRegion, -1);
}

// func_80235814 — file loading
// .text:0x10C, size 0x68
void func_80235814(CArtsInfo* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (int)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)lbl_eu_8050B00C, (const char*)self, (void*)handle, 0, 0);

    func_800A9D90();
    self->field_0x18 = (int)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)(lbl_eu_8050B00C + 0x15), (const char*)self, (void*)handle, 0, 0);
}

// func_8023587C — state machine dispatch
// .text:0x174, size 0xDC
void func_8023587C(CArtsInfo* self) {
    if (self->field_0x40 == 0) return;

    int state = self->field_0x44;
    if (state >= 0 && state <= 0xC) {
        // Dispatch based on state via jump table
        switch (state) {
        case 0: func_80235F6C(self); break;
        case 1: func_80236020(self); break;
        case 2: func_8023606C(self); break;
        case 3: func_80236120(self); break;
        case 4: func_8023616C(self); break;
        case 5: func_80236220(self); break;
        case 6: func_802362D4(self); break;
        case 7: func_80236334(self); break;
        case 8: func_80236408(self); break;
        case 9: func_80236454(self); break;
        // cases 0xA-0xC fall through to default
        default: break;
        }
    }

    // Post-dispatch: animate both layouts and update cursor
    callVirt_38_0(self->mpLayout1);
    callVirt_38_0(self->mpLayout2);
    func_801D202C(self->mCursor);
}

// func_80235958 — draw
// .text:0x250, size 0x74
void func_80235958(CArtsInfo* self, void* drawInfo) {
    if (self->field_0x40 == 0) return;

    func_80137038(self->mpLayout1, drawInfo, 0, 1);
    func_80137038(self->mpLayout2, drawInfo, 0, 1);

    s8 cursorActive = self->field_0x5A;
    if (cursorActive >= 0) {
        func_801D20B0(self->mCursor, drawInfo);
    }
}

// func_802359CC — cleanup
// .text:0x2C4, size 0xC4
void func_802359CC(CArtsInfo* self) {
    func_8003AA8C__5CBdatFUl(2);

    func_801390E0__FPP11CFileHandle(&self->field_0x14);
    func_801390E0__FPP11CFileHandle(&self->field_0x18);

    self->field_0x40 = 0;

    // Delete layout 1 if non-null
    if (self->mpLayout1 != nullptr) {
        callVirtDelete_08(self->mpLayout1);
        self->mpLayout1 = nullptr;
    }

    // Delete layout 2 if non-null
    if (self->mpLayout2 != nullptr) {
        callVirtDelete_08(self->mpLayout2);
        self->mpLayout2 = nullptr;
    }

    // Cleanup arc resource accessor
    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(&self->field_0x1C);

    // Call virtual cleanup on cursor
    callVirt_0C(self->mCursor);

    // Cleanup memory region
    func_8045F778__17UnkClass_8045F564Fv(&self->mMemRegion);
}

// func_80235A90 — get field_0x48
// .text:0x388, size 0x8
u8 func_80235A90(CArtsInfo* self) { return self->field_0x48; }

// func_80235A98 — get field_0x49
// .text:0x390, size 0x8
u8 func_80235A98(CArtsInfo* self) { return self->field_0x49; }

// func_80235AA0 — state check (state==0 -> 1)
// .text:0x398, size 0x20
void func_80235AA0(CArtsInfo* self) {
    if (self->field_0x44 != 0) return;
    self->field_0x44 = 1;
    self->field_0x49 = 0;
}

// func_80235AC0 — state check (state==3 -> 4)
// .text:0x3B8, size 0x20
void func_80235AC0(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;
    self->field_0x44 = 4;
    self->field_0x49 = 0;
}

// func_80235AE0 — large state machine (state==3 -> 6, layout animation setup)
// .text:0x3D8, size 0x244
void func_80235AE0(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;

    self->field_0x44 = 6;
    self->field_0x49 = 0;

    // Bind animations to layout 1
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans4 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout1);

    // Bind animations to layout 1 again
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans3 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout1);

    // Bind animations to layout 2
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans6, 0);
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans5, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout2);

    // Reset cursor state
    self->field_0x5A = 0;

    // Update layout elements with arts data
    u32 artsData = func_8009EC9C(self->field_0x54);
    void* str1 = func_80136190(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3D, 0x18);

    char buf[64];
    sprintf(buf, lbl_eu_8050B00C + 0x42, *(u32*)((u8*)artsData + 0x17C), str1);

    func_80136A1C(self->mpLayout1, lbl_eu_8050B00C + 0x47, buf, 0);

    func_80138078__FUl(0x6D);
}

// func_80235D24 — state machine (state==9 -> 0xA)
// .text:0x61C, size 0xB4
void func_80235D24(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xA;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Bind animations to layout 2
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans6, 0);
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans5, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout2);

    func_80138078__FUl(6);
}

// func_80235DD8 — state machine (state==9 -> 0xC)
// .text:0x6D0, size 0xAC
void func_80235DD8(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xC;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Bind animations to layout 2
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans5, 0);
    callVirt_2C_50(self->mpLayout2, self->mpAnimTrans6, 1);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans6 + 0x10) = lbl_eu_80668680;

    callVirt_38_0(self->mpLayout2);
}

// func_80235E84 — set field_0x54
// .text:0x77C, size 0x8
void func_80235E84(CArtsInfo* self, u8 val) { self->field_0x54 = val; }

// func_80235E8C — set field_0x55
// .text:0x784, size 0x8
void func_80235E8C(CArtsInfo* self, u8 val) { self->field_0x55 = val; }

// func_80235E94 — set field_0x56
// .text:0x78C, size 0x8
void func_80235E94(CArtsInfo* self, u8 val) { self->field_0x56 = val; }

// func_80235E9C — set field_0x58
// .text:0x794, size 0x8
void func_80235E9C(CArtsInfo* self, u16 val) { self->field_0x58 = val; }

// func_80235EA4 — call func_80236508, check fields, call func_802369C0
// .text:0x79C, size 0x4C
void func_80235EA4(CArtsInfo* self) {
    func_80236508(self);

    if (self->field_0x54 != 0 && self->field_0x55 != 0) {
        func_802369C0(self);
    }
}

// func_80235EF0 — decrement field_0x5A, tail-call func_80236CF4
// .text:0x7E8, size 0x24
void func_80235EF0(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    val--;
    if (val < 0) val = 1;
    self->field_0x5A = val;
    func_80236CF4(self);
}

// func_80235F14 — increment field_0x5A, tail-call func_80236CF4
// .text:0x80C, size 0x28
void func_80235F14(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    val++;
    if (val > 1) val = 0;
    self->field_0x5A = val;
    func_80236CF4(self);
}

// func_80235F3C — check if field_0x5A is zero
// .text:0x834, size 0x14
u32 func_80235F3C(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    return (val == 0) ? 1 : 0;
}

// func_80235F50 — check if field_0x44 < 6
// .text:0x848, size 0x1C
u32 func_80235F50(CArtsInfo* self) {
    int state = self->field_0x44;
    return (state < 6) ? 1 : 0;
}

// func_80235F6C — animation state 0
// .text:0x864, size 0xB4
void func_80235F6C(CArtsInfo* self) {
    if (func_80137444(self->mpAnimTrans1, lbl_eu_80668684) == 0) return;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 1);

    self->field_0x44 = 2;
}

// func_80236020 — animation state 1
// .text:0x918, size 0x4C
void func_80236020(CArtsInfo* self) {
    if (func_80137444(self->mpAnimTrans2, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 3;
    self->field_0x49 = 1;
}

// func_8023606C — animation state 2
// .text:0x964, size 0xB4
void func_8023606C(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans2, lbl_eu_80668684) == 0) return;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 1);

    self->field_0x44 = 5;
}

// func_80236120 — animation state 3
// .text:0xA18, size 0x4C
void func_80236120(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans1, lbl_eu_80668684) == 0) return;

    self->field_0x49 = 1;
    self->field_0x44 = 0;
}

// func_8023616C — animation state 4
// .text:0xA64, size 0xB4
void func_8023616C(CArtsInfo* self) {
    if (func_80137444(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 7;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 1);
}

// func_80236220 — animation state 5
// .text:0xB18, size 0xB4
void func_80236220(CArtsInfo* self) {
    if (func_80137444(self->mpAnimTrans4, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 8;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 1);
}

// func_802362D4 — animation state 6
// .text:0xBCC, size 0x60
void func_802362D4(CArtsInfo* self) {
    if (func_80137444(self->mpAnimTrans5, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 9;
    self->field_0x49 = 1;
    func_801D216C(self->mCursor, 1);
    func_80236CF4(self);
}

// func_80236334 — animation state 7
// .text:0xC2C, size 0xD4
void func_80236334(CArtsInfo* self) {
    u32 done1 = func_80137510(self->mpAnimTrans4, lbl_eu_80668684);
    u32 done2 = func_80137510(self->mpAnimTrans5, lbl_eu_80668684);

    if (done1 == 0 || done2 == 0) return;

    self->field_0x44 = 0xB;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 1);
}

// func_80236408 — animation state 8
// .text:0xD00, size 0x4C
void func_80236408(CArtsInfo* self) {
    if (func_80137510(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 3;
    self->field_0x49 = 1;
}

// func_80236454 — animation state 9
// .text:0xD4C, size 0xB4
void func_80236454(CArtsInfo* self) {
    if (func_80137444(self->mpAnimTrans6, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 0xB;

    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans2, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans1, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans4, 0);
    callVirt_2C_50(self->mpLayout1, self->mpAnimTrans3, 1);
}

// func_80236508 — large layout setup (bind all pane animations)
// .text:0xE00, size 0x4B8
void func_80236508(CArtsInfo* self) {
    // Bind animations for all named panes on layout 1
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x50, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x59, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x64, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x72, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x80, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x8B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x96, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xA1, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xAC, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xB7, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xC2, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xCD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xD8, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xE3, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xEE, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0xFD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x10C, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x11B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x12A, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x139, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x148, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x157, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x166, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout1, lbl_eu_8050B00C + 0x175, lbl_eu_8050B00C + 0x58, 0);

    // Bind animations for all named panes on layout 2
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x50, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x59, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x64, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x72, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x80, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x8B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x96, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xA1, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xAC, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xB7, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xC2, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xCD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xD8, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xE3, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xEE, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0xFD, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x10C, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x11B, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x12A, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x139, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x148, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x157, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x166, lbl_eu_8050B00C + 0x58, 0);
    func_80136B4C(self->mpLayout2, lbl_eu_8050B00C + 0x175, lbl_eu_8050B00C + 0x58, 0);

    // Set text rendering parameters for specific panes on layout 2
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0xEE, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0xFD, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x10C, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x11B, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x12A, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x139, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x148, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x157, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x166, lbl_eu_80664748, lbl_eu_80664750);
    func_80139A18(self->mpLayout2, lbl_eu_8050B00C + 0x175, lbl_eu_80664748, lbl_eu_80664750);
}

void func_802369C0(){}

void func_80236CF4(){}

void func_80236DB8(){}

void func_80236DF0(){}

void func_80236E28(){}

void func_80236E6C(){}

void func_80237050(){}

void func_802370A8(){}

void func_80237100(){}

void func_8023719C(){}

void func_80237238(){}

void func_80237394(){}

void func_802374F0(){}

void func_8023754C(){}

void func_802375A8(){}

void func_80237A0C(){}

void func_80237B88(){}

void func_80237D58(){}

void func_80237E24(){}

void func_80238038(){}

void func_80238298(){}

void func_802384F4(){}

void func_80238904(){}

void func_80239030(){}

void func_8023916C(){}

void func_8023939C(){}

void func_8023959C(){}

void func_802397F4(){}

void func_80239964(){}

void func_80239AA0(){}

void func_80239BDC(){}

void func_80239D20(){}

void func_80239EFC(){}

void func_80239FC4(){}

void func_8023A148(){}

void func_8023A210(){}

void func_8023A2D8(){}

void func_8023A398(){}

void func_8023A460(){}

void func_8023A55C(){}

void func_8023A60C(){}

void func_8023A6BC(){}

void func_8023A76C(){}

void func_8023A81C(){}

void func_8023A8CC(){}

void func_8023A97C(){}

void func_8023AA2C(){}

void func_8023AADC(){}

void func_8023AB8C(){}

void func_8023AD5C(){}

void func_8023AE24(){}

void func_8023AF60(){}

void func_8023B074(){}

void func_8023B12C(){}

void func_8023B280(){}

void func_8023B368(){}

void func_eu_8023D490(){}

void CArtsInfo::setField54(u8 val) { field_0x54 = val; }

void CArtsInfo::setField55(u8 val) { field_0x55 = val; }

void CArtsInfo::setField56(u8 val) { field_0x56 = val; }

void CArtsInfo::setField58(u16 val) { field_0x58 = val; }

u32 CArtsInfo::getField5A() {
    s8 val = field_0x5A;
    u32 result = __cntlzw(val);
    return result >> 5;
}

int CArtsInfo::isField44GE6() {
    int field = field_0x44;
    return field >= 6 ? 1 : 0;
}

void CArtsInfo::initialize() {
    if (mpLayout1 == nullptr) return;
    if (field_0x4C == 0) return;
    if (field_0x50 != 0) {
        field_0x48 = 1;
        field_0x40 = 1;
    }
}

void CArtsInfo::OnFileEvent() {}



// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8023BC8C(){}
