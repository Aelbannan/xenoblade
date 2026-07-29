// Auto-scaffolded catalog TU for kyoshin/CFloorMap
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/CFloorMap.cpp" line 3 "kyoshin/harness_catalog.hpp" */
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
/* "src/kyoshin/CFloorMap.cpp" line 4 "kyoshin/CFloorMap.hpp" */
#pragma once

/* "src/kyoshin/CFloorMap.hpp" line 2 "types.h" */
/* end "types.h" */

// Full object layout for CFloorMap (used by C-linkage accessors)
struct CFloorMapFull {
    u8 _00[0x40];
    u8 field_40;
    u8 field_41;
    u8 _42[0x58 - 0x42];
    u8 field_58;
    u8 _59[0x208 - 0x59];
    u8 field_208;
};

class CFloorMap {
public:
    CFloorMap();
    virtual ~CFloorMap();
    void OnFileEvent() const;

    // TODO: add fields
};

/* end "kyoshin/CFloorMap.hpp" */
/* "src/kyoshin/CFloorMap.cpp" line 5 "cstdio" */
#ifndef MSL_CPP_CSTDIO_H
#define MSL_CPP_CSTDIO_H
/* "libs/PowerPC_EABI_Support/include/stl/cstdio" line 2 "stdio.h" */
#ifndef MSL_STDIO_H
#define MSL_STDIO_H

/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 8 "PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" */
#ifndef STDIO_API_H
#define STDIO_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 4 "PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" */
#ifndef _MSL_COMMON_FILE_STRUC_H
#define _MSL_COMMON_FILE_STRUC_H
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" line 2 "types.h" */
/* end "types.h" */

typedef unsigned long __file_handle;
typedef unsigned long fpos_t;
typedef struct _FILE _FILE, *P_FILE;

#define __ungetc_buffer_size 2

enum __file_kinds {
    __closed_file,
    __disk_file,
    __console_file,
    __unavailable_file
};

enum __open_modes {
    __must_exist,
    __create_if_necessary,
    __create_or_truncate
};

enum __file_orientation {
    __unoriented,
    __char_oriented,
    __wide_oriented
};

enum __io_modes {
    __read = 1,
    __write = 2,
    __read_write = 3,
    __append = 4
};

typedef struct __file_modes {
    u32 open_mode : 2;
    u32 io_mode : 3;
    u32 buffer_mode : 2;
    u32 file_kind : 3;

#ifdef _MSL_WIDE_CHAR
    u32 file_orientation : 2;
#endif /* _MSL_WIDE_CHAR */

    u32 binary_io : 1;
} __file_modes;

enum __io_states {
    __neutral,
    __writing,
    __reading,
    __rereading
};

typedef struct __file_state {
    u32 io_state : 3;
    u32 free_buffer : 1;
    u8 eof;
    u8 error;
} __file_state;

typedef void* __ref_con;
typedef void (*__idle_proc)(void);
typedef int (*__pos_proc)(__file_handle file, fpos_t* position, int mode, __ref_con ref_con);
typedef int (*__io_proc)(__file_handle file, u8* buff, size_t* count, __ref_con ref_con);
typedef int (*__close_proc)(__file_handle file);

struct _FILE {
    __file_handle handle;                           // _00
    __file_modes mode;                              // _04
    __file_state state;                              // _08
    u8 is_dynamically_allowed;                      // _0C
    u8 char_buffer;                                 // _0D
    u8 char_buffer_overflow;                        // _0E
    u8 ungetc_buffer[__ungetc_buffer_size];         // _0F
    wchar_t ungetwc_buffer[__ungetc_buffer_size];   // _12
    u32 position;                                   // _18
    u8* buffer;                                   // _1C
    u32 buffer_size;                                // _20
    u8* buffer_ptr;                               // _24
    u32 buffer_len;                                 // _28
    u32 buffer_alignment;                           // _2C
    u32 saved_buffer_len;                           // _30
    u32 buffer_pos;                                 // _34
    __pos_proc position_proc;                       // _38
    __io_proc read_proc;                            // _3C
    __io_proc write_proc;                           // _40
    __close_proc close_proc;                        // _44
    __ref_con ref_con;                              // _48
    _FILE* next_file_struct;                        // _4C
};

typedef struct _FILE FILE;


#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

// define standard C file pointer location names
#define SEEK_SET (0)
#define SEEK_CUR (1)
#define SEEK_END (2)

#define stdin &(__files[0])
#define stdout &(__files[1])
#define stderr &(__files[2])

#define _STATIC_FILES 4

extern FILE __files[];

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 5 "wchar.h" */
#ifndef MSL_WCHAR_H
#define MSL_WCHAR_H

/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif

/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" */
#ifndef _WCHAR_IO_H
#define _WCHAR_IO_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" line 4 "stdio.h" */
/* end "stdio.h" */

int fwide(FILE* stream, int mode);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" */
#ifndef MSL_WCSTOUL_H
#define MSL_WCSTOUL_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" line 3 "types.h" */
/* end "types.h" */


unsigned long __wcstoul(int, int, wint_t (*wReadProc)(void*, wint_t, int), void*, int*, int*, int*);
//__wcstoull
//wcstoul
//wcstoull
long wcstol(const wchar_t*, wchar_t**, int);
//wcstoll
//watoi
//watol

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 11 "PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" */
#ifndef MSL_WMEM_H
#define MSL_WMEM_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" line 3 "types.h" */
/* end "types.h" */

wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n);
wchar_t* wmemchr(wchar_t* s, wchar_t c, int n);
void* memmove(void*, const void*, size_t);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 12 "PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" */
#ifndef MSL_WPRINTF_H
#define MSL_WPRINTF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 4 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 5 "stdio.h" */
/* end "stdio.h" */

//wprintf
//wprintf_s
//fwprintf
//fwprintf_s
//vwprintf
//vwprintf_s
//vfwprintf
//vfwprintf_s
int swprintf(wchar_t*, size_t, const wchar_t*, ...);
//swprintf_s
//snwprintf_s
int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);
//vswprintf_s
//vsnwprintf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 13 "PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" */
#ifndef MSL_WSTRING_H
#define MSL_WSTRING_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" line 3 "types.h" */
/* end "types.h" */

size_t wcslen(const wchar_t*);
wchar_t* wcscpy(wchar_t*, const wchar_t*);
wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t);
wchar_t* wcscat(wchar_t*, const wchar_t*);
int wcscmp(const wchar_t*, const wchar_t*);
wchar_t* wcschr(const wchar_t*, wchar_t);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" */

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "wchar.h" */

enum __ReadProcActions {
    __GetAChar,
    __UngetAChar,
    __TestForError
};

enum __WReadProcActions
{
    __GetAwChar,
    __UngetAwChar,
    __TestForwcsError
};

typedef struct {
    char* CharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __OutStrCtrl;

typedef struct{
    char* NextChar;
    int NullCharDetected;
} __InStrCtrl;

typedef struct {
    wchar_t * wCharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __wOutStrCtrl;

typedef struct {
    wchar_t * wNextChar;
    int    wNullCharDetected;
} __wInStrCtrl;

//__fread
size_t __fwrite(const void *pPtr, size_t memb_size, size_t num_memb, FILE *file);
int __StringRead(void *, int, int);
wint_t __wStringRead(void*, wint_t, int);

#endif // STDIO_API_H
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" */
#ifndef MSL_FILE_POS_H
#define MSL_FILE_POS_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" line 4 "stdio.h" */
/* end "stdio.h" */

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

int fseek(FILE* stream, u32 offset, int whence);
int _fseek(FILE* stream, u32 offset, int whence);
int ftell(FILE* stream);
int _ftell(FILE* stream);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" */
#ifndef MSL_FILE_IO_H
#define MSL_FILE_IO_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" line 4 "stdio.h" */
/* end "stdio.h" */

int fclose(FILE* file);
int fflush(FILE* file);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 11 "PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" */
#ifndef MSL_PRINTF_H
#define MSL_PRINTF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" line 3 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" line 4 "stdio.h" */
/* end "stdio.h" */


//printf
//printf_s
int fprintf(FILE*, const char* format, ...);
//fprintf_s
int vprintf(const char*, va_list);
//vprintf_s
//vfprintf
//vfprintf_s
int vsnprintf(char*, size_t, const char*, va_list);
//vsnprintf_s
int vsprintf(char*, const char*, va_list);
//vsprintf_s
int snprintf(char*, size_t, const char*, ...);
//snprintf_s
int sprintf(char*, const char*, ...);
//sprintf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 12 "PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" */
#ifndef MSL_SCANF_H
#define MSL_SCANF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" line 3 "stdarg.h" */
/* end "stdarg.h" */

//fscanf
//fscanf_s
//vscanf
//scanf
//scanf_s
//vfscanf
//vfscanf_s
int vsscanf(const char*, const char*, va_list);
//vsscanf_s
int sscanf(const char*, const char*, ...);
//sscanf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" */

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "stdio.h" */
#ifdef __cplusplus

namespace std {
using ::fclose;
using ::fflush;
using ::FILE;
using ::ftell;
using ::fwide;
using ::snprintf;
using ::sprintf;
using ::sscanf;
using ::vprintf;
using ::vsnprintf;
using ::vsprintf;
} // namespace std

#endif
#endif
/* end "cstdio" */

extern "C" char lbl_eu_8050BEA8[];
extern u32 lbl_eu_8066479C;
extern u32 func_8003B1EC(u32);
extern u32 lbl_eu_8050BDF8[];
extern u8 lbl_eu_80664798;

extern int CSysWin_getUnk34(void*);
extern void func_80246200(void*);
extern u32 func_80248558(u8*);
extern void func_8024577C(void*, u16);
extern void func_801F3850(void*, u16);
extern void func_801375A0(float*, void*);
extern void func_80137C1C(void*, void*);
extern void* func_80136190(const char*, const char*, const char*);
extern void* func_801355F4();
extern void* createPicture__10CLibLayoutFv();
extern void SetName__Q34nw4r3lyt4PaneFPCc(void*, const char*);
extern void* func_80137E7C(void*, const char*, const char*);
extern u32 func_8009CF8C(u32);
extern void func_80138078(u32);

extern float lbl_eu_80668764;
extern float lbl_eu_80668794;
extern float lbl_eu_80668798;
extern float lbl_eu_806687A4;
extern float lbl_eu_806687A8;
extern float lbl_eu_806687AC;
extern float lbl_eu_806687B0;
extern float lbl_eu_806687B4;
extern float lbl_eu_806687B8;

typedef void* (*VFuncPtr)(void*, const char*, u32);

u8 func_8024CE60(void* self) { return static_cast<CFloorMapFull*>(self)->field_40; }










void func_80245450(void* self) {
    extern void func_80246200(void*);
    extern void func_80138078(u32);
    u8* p = (u8*)self;
    s8 idx0 = (s8)p[0x09];
    u32 base = idx0 * 0x30C;
    u8 count = *(u8*)(p + base + 0x318);
    s8 idxA = (s8)p[0x0A];
    s8 idxB = (s8)p[0x0B];
    if (count >= 5) {
        idxA++;
        if (idxA >= 5) {
            idxA = 4;
            idxB++;
            if (idxB > (s8)(count - 5)) {
                idxA = 0;
                idxB = 0;
            }
        }
    } else {
        idxA++;
        if (idxA >= (s8)count) {
            idxA = 0;
            idxB = 0;
        }
    }
    p[0x0A] = idxA;
    p[0x0B] = idxB;
    func_80246200(self);
}

void func_802455F0(void* self) {
    extern void func_80246200(void*);
    u8* p = (u8*)self;
    s8 idx0 = (s8)p[0x09];
    u32 base = idx0 * 0x30C;
    u8 count = *(u8*)(p + base + 0x318);
    if (count >= 5) {
        p[0x0B] += 5;
        if ((s8)p[0x0B] > (s8)count) {
            p[0x0A] = p[0x0B] - count;
            p[0x0B] = count;
            if ((s8)p[0x0A] >= 5) {
                p[0x0A] = 0;
            }
        }
    } else {
        p[0x0A] = count - 1;
        p[0x0B] = 0;
        if ((s8)p[0x0A] < 0) p[0x0A] = 0;
    }
    func_80246200(self);
}

void func_8024577C(void* self, u16 val) {
    u8* p = (u8*)self;
    if (!val) {
        p[0x0A] = -1;
        p[0x0B] = 0;
        return;
    }
    s8 idx0 = (s8)p[0x09];
    s8 idxA = (s8)p[0x0A];
    s8 idxB = (s8)p[0x0B];
    u32 base = idx0 * 0x30C;
    u8 count = *(u8*)(p + base + 0x318);
    for (u8 i = 0; i < count; i++) {
        if (*(u16*)(p + base + i * 0x18 + 0x18) == val) {
            if (i >= 5) {
                p[0x0A] = 4;
                p[0x0B] = i - 4;
            } else {
                p[0x0A] = i;
                p[0x0B] = 0;
            }
            func_80246200(self);
            if (idxA != (s8)p[0x0A] || idxB != (s8)p[0x0B]) {
                func_80138078(1);
            }
            return;
        }
    }
}

u8* __dt__80244724(u8* self, int mode) {
    extern u8* __dl__FPv(u8*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

u8* __dt__8024503C(u8* self, int mode) {
    extern u8* __dl__FPv(u8*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_80245950(){}

void func_80245DF8(){}

void func_80246200(void* self){}

void* __dt__802462F0(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_80246330(){}

void* __dt__802468C8(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_80246908(){}

void func_80247490(){}

void func_8024808C(void* self, void* arg2) {
    extern void func_8003AA34();
    extern void* getFP__FPCc(const char*);
    extern u32 func_801361E8(void*, char*, u32);
    extern u32 func_8009CF8C(u32);
    extern void func_80141DC4(f32*);
    extern s16 func_80136330(u32, const char*, u32);
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8003B1EC(u32);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_80668778;
    extern f64 lbl_eu_80668788;
    u8* p = (u8*)self;
    func_8003AA34();
    void* fp = getFP__FPCc(&lbl_eu_8050BEA8[0x17F]);
    u8 map = func_801361E8(fp, &lbl_eu_8050BEA8[0x18C], *(u32*)((u8*)arg2 + 0x10));
    if (map != lbl_eu_80664798) return;
    if (!func_8009CF8C(0x20C8)) return;
    f32 buf[3];
    func_80141DC4(buf);
    u8 count = func_8003B1EC((u32)fp);
    u8 r26 = 0;
    for (u8 i = 1; i <= count; i++) {
        s16 val = func_80136330((u32)fp, &lbl_eu_8050BEA8[0x15A], i);
        if ((f32)(s16)val > buf[1]) {
            if (i == p[0x0C]) { r26 = 1; break; }
        }
    }
    if (r26) {
        p[0x0A]++;
    }
}

void func_8024830C(void* self, void* arg2) {
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern s16 func_80136330(u32, const char*, u32);
    extern u32 func_8009CF8C(u32);
    extern void func_80141DC4(f32*);
    extern u32 func_801361E8(const char*, const char*, u32);
    extern u32 lbl_eu_80664184;
    extern u32 lbl_eu_806640A8;
    extern u8 lbl_eu_8050B798;
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_80668778;
    extern f32 lbl_eu_8066877C;
    extern f64 lbl_eu_80668788;
    u8* p = (u8*)self;
    f32* result = (f32*)p;
    result[0] = result[1] = result[2] = lbl_eu_80668764;
    void* slot = *(void**)((u8*)arg2 + 0x00);
    if (!slot) return;
    if ((lbl_eu_80664184 & 0xFF) == lbl_eu_80664798) {
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        if (!player) return;
        void** vt = *(void***)player;
        f32* pos = (f32*)((void*(*)(void*))vt[0xAC])(player);
        result[0] = pos[0]; result[1] = pos[1]; result[2] = pos[2];
    } else {
        u16 idx = *(u16*)lbl_eu_8050B798;
        if (!idx) return;
        f32 buf[3];
        func_80141DC4(buf);
        result[0] = buf[0]; result[1] = buf[1]; result[2] = buf[2];
    }
    s16 val1 = func_80136330(*(u32*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1E2], lbl_eu_80664798);
    s16 val2 = func_80136330(*(u32*)lbl_eu_806640A8, &lbl_eu_8050BEA8[0x1F0], lbl_eu_80664798);
    u8 region = func_801361E8(&lbl_eu_8050BEA8[0x1FE], &lbl_eu_8050BEA8[0x1F], lbl_eu_80664798);
    result[0] += (f32)(s16)val1 / ((f32)(s32)region * lbl_eu_80668778);
    result[1] -= (f32)(s16)val2 / ((f32)(s32)region * lbl_eu_80668778);
    result[2] = lbl_eu_80668764;
    if (*(void**)(p + 0x3108)) {
        void* obj = *(void**)((u8*)*(void**)(p + 0x3108) + 0x10);
        *(f32*)((u8*)obj + 0x2C) = result[0];
        *(f32*)((u8*)obj + 0x30) = result[1];
        *(f32*)((u8*)obj + 0x34) = result[2];
    }
    func_801F3850(*(void**)(p + 0x3134), (u16)(s16)p[0x0B]);
}

u32 func_80248558(u8* self) {
    extern void Panic__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
    u8* p = self;
    void* data = *(void**)(p + 0x08);
    if (!data) return 0;
    void* obj = *(void**)((u8*)data + 0x10);
    if (!obj) Panic__Q24nw4r2dbFPCciPCce(&lbl_eu_8050BEA8[0x26C], 0x23D, "", "");
    void** vtable = *(void***)obj;
    void* result = ((void*(*)(void*, const char*, u32))vtable[15])(obj, &lbl_eu_8050BEA8[0x26C], 1);
    if (!result) return 0;
    void* target = *(void**)((u8*)result + 0x14);
    void* current = result;
    while (current) {
        void* cur_target = *(void**)((u8*)current + 0x10);
        if (cur_target == target) break;
        void* next = *(void**)((u8*)current + 0x0C);
        if (!next) break;
        current = next;
    }
    return current ? *(u32*)((u8*)current + 0x14) : 0;
}

void* func_80248920(void* self, const char* name, float x, float y, void* arg5, const char* paneName) {
    if (!name) return NULL;
    if (!paneName) return NULL;

    void* result = func_80136190(&lbl_eu_8050BEA8[0x2f6], &lbl_eu_8050BEA8[0x303], name);

    char buf[48];
    sprintf(buf, &lbl_eu_8050BEA8[0x30e], result);

    void* accessor = func_801355F4();
    typedef void* (*VFuncPtr4)(void*, u32, void*, u32);
    VFuncPtr4* vt = *(VFuncPtr4**)accessor;
    void* picture = vt[3](accessor, 0x74696d67, buf, 0);

    if (!picture) return NULL;

    void* pic = createPicture__10CLibLayoutFv();
    SetName__Q34nw4r3lyt4PaneFPCc(pic, paneName);

    *(float*)((u8*)pic + 0x2C) = x;
    *(float*)((u8*)pic + 0x30) = y;
    *(float*)((u8*)pic + 0x34) = lbl_eu_80668764;

    func_80137C1C(pic, arg5);

    u8* byte = (u8*)pic + 0xBB;
    *byte = (*byte & 0x7F) | 0x01;

    *(float*)((u8*)pic + 0x44) = lbl_eu_80668794;
    *(float*)((u8*)pic + 0x48) = lbl_eu_80668794;

    return pic;
}

void func_80248A6C(){}

void func_80248ED8(){}

void func_80249344(){}

void func_802497B0(){}

void func_80249C1C(){}

void func_8024A448(){}

void func_8024A748(){}

void func_8024AEEC(){}

void func_8024B234(){}

void func_8024B4CC(void* result, void* data, void* node) {
    extern f32 lbl_eu_80668764;
    f32* r = (f32*)result;
    r[0] = r[1] = r[2] = lbl_eu_80668764;
    void* target = *(void**)((u8*)data + 0x10);
    if (target == node || !node) return;
    r[0] = *(f32*)((u8*)node + 0x2C);
    r[1] = *(f32*)((u8*)node + 0x30);
    r[2] = *(f32*)((u8*)node + 0x34);
    void* next = *(void**)((u8*)node + 0x0C);
    f32 temp[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
    if (target != next && next) {
        temp[0] = *(f32*)((u8*)next + 0x2C);
        temp[1] = *(f32*)((u8*)next + 0x30);
        temp[2] = *(f32*)((u8*)next + 0x34);
    }
    void* next2 = next ? *(void**)((u8*)next + 0x0C) : 0;
    if (target != next2 && next2) {
        f32 local[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
        local[0] = *(f32*)((u8*)next2 + 0x2C);
        local[1] = *(f32*)((u8*)next2 + 0x30);
        local[2] = *(f32*)((u8*)next2 + 0x34);
        void* next3 = *(void**)((u8*)next2 + 0x0C);
        if (target != next3 && next3) {
            f32 rec[3];
            func_8024B4CC(rec, data, next3);
            local[0] += rec[0]; local[1] += rec[1]; local[2] += rec[2];
        }
        temp[0] += local[0]; temp[1] += local[1]; temp[2] += local[2];
    }
    r[0] += temp[0]; r[1] += temp[1]; r[2] += temp[2];
}

void* __dt__8024B6B8(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void func_8024B6F8(void* self, void* arg2, u32 arg3, u32 arg4) {
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8009CF8C(u32);
    extern s16 func_80136330(u32, const char*, u32);
    extern s16 func_80137E7C(void*, void*, const char*, ...);
    u8* p = (u8*)self;
    if (!arg2 || !*(void**)p) return;
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    if (!player) return;
    void* data = *(void**)p;
    void* obj = *(void**)((u8*)data + 0x10);
    void** vtable = *(void***)obj;
    void* result = ((void*(*)(void*, void*, const char*, ...))vtable[1])(data, arg2, &lbl_eu_8050BEA8[0x47F]);
    if (!result) return;
    for (u32 i = 1; i <= arg3; i++) {
        s16 val = func_80136330(*(u32*)lbl_eu_8066479C, &lbl_eu_8050BEA8[0x487], i);
        if (val) {
            u8* pBB = (u8*)result + 0xBB;
            u8 bit = (i == arg4) ? 1 : 0;
            *pBB = (*pBB & 0x7F) | bit;
        }
    }
    if (arg4 == 0xC) {
        if (result) {
            u32 val = func_8009CF8C(0x20);
            u8* pBB = (u8*)result + 0xBB;
            *pBB = (*pBB & 0x7F) | ((__cntlzw(val ^ 0x166) >> 5) & 1);
        }
    } else if (arg4 == 5) {
        if (result) {
            u32 val = func_8009CF8C(0x20);
            u8* pBB = (u8*)result + 0xBB;
            u8 bit = ((val - 0x171) | (val ^ 0x171)) >> 31;
            *pBB = (*pBB & 0x7F) | (bit & 1);
        }
    }
}

void* __dt__8024B894(void* self, int mode) {
    extern void* __dl__FPv(void*);
    if (self && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}

void __ct__CFloorMap(){}

CFloorMap::~CFloorMap() {}

void func_8024BE1C(){}


void func_8024C1FC(){}

void func_8024C8F8(){}

void func_8024CB94(){}

u8 func_8024CE1C(void* self) {
    extern int CScrollBar_isVisible(void*);
    if (CScrollBar_isVisible((u8*)self + 0x60)) {
        return *(u8*)((u8*)self + 0x42);
    }
    return 0;
}


void func_8024CE68(){}

void func_8024D23C(){}

void func_8024D614(){}

void func_8024DA0C(){}

void func_8024DE08(){}

void func_8024E2BC(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern void func_802455F0(void*);
    extern int sprintf(char*, const char*, ...);
    extern void func_8024B4CC(void*, void*, void*);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687BC;
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x2C)) return;
    if (!p[0x208]) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    if (p[0x58]) return;
    s8 idx0 = (s8)p[0x205];
    u32 base = idx0 * 0x30C;
    if (!*(u8*)(p + base + 0x514)) return;
    func_802455F0(p + 0x1FC);
    s8 idx1 = (s8)p[0x206];
    u16 val = 0;
    if (idx1 >= 0) {
        s8 idx2 = (s8)p[0x207];
        val = *(u16*)(p + base + (idx2 + idx1) * 0x18 + 0x214);
    }
    if (!val) return;
    f32 pos[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);
    void* data = *(void**)(p + 0x140);
    void* obj = *(void**)((u8*)data + 0x10);
    VFuncPtr* vtable = *(VFuncPtr**)obj;
    void* result = vtable[15](obj, buf, 1);
    if (result) {
        void* target = *(void**)((u8*)data + 0x10);
        void* node = result;
        if (node && *(void**)((u8*)node + 0x10) != target) {
            void* next = *(void**)((u8*)node + 0x0C);
            if (next && *(void**)((u8*)next + 0x10) != target) {
                void* next2 = *(void**)((u8*)next + 0x0C);
                if (next2 && *(void**)((u8*)next2 + 0x10) != target) {
                    f32 p1[3];
                    func_8024B4CC(p1, data, next2);
                    pos[0] += p1[0]; pos[1] += p1[1]; pos[2] += p1[2];
                }
                pos[0] += *(f32*)((u8*)next2 + 0x2C);
                pos[1] += *(f32*)((u8*)next2 + 0x30);
                pos[2] += *(f32*)((u8*)next2 + 0x34);
            }
            pos[0] += *(f32*)((u8*)next + 0x2C);
            pos[1] += *(f32*)((u8*)next + 0x30);
            pos[2] += *(f32*)((u8*)next + 0x34);
        }
        void* result2 = vtable[15](obj, &lbl_eu_8050BEA8[0x136], 1);
        if (result2) {
            f32 scale = *(f32*)((u8*)result2 + 0x44);
            pos[0] *= scale;
            pos[0] += *(f32*)((u8*)data + 0x2C);
            pos[1] += *(f32*)((u8*)data + 0x30);
            pos[2] += *(f32*)((u8*)data + 0x34);
        }
    }
    *(f32*)(p + 0x54) = lbl_eu_806687BC;
    p[0x41] = 2;
    *(f32*)(p + 0x4C) = pos[0] / lbl_eu_806687BC;
    *(f32*)(p + 0x50) = pos[1] / lbl_eu_806687BC;
}

void func_8024E650(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(u8*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687A4;
    extern f32 lbl_eu_806687A8;
    u8* p = (u8*)self;
    if (p[0x58]) goto done;
    if (CSysWin_getUnk34(p + 0xB8)) goto done;
    if (CSysWin_getUnk34(p + 0xF4)) goto done;
    f32* pos = (f32*)(p + 0x44);
    pos[1] -= lbl_eu_806687A8;
    if (pos[1] < lbl_eu_806687A4) pos[1] = lbl_eu_806687A4;
    for (int i = 0; i < 3; i++) {
        void* slot = *(void**)(p + 0x130 + i * 8);
        if (slot) {
            void* obj = *(void**)((u8*)slot + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    void* slot = *(void**)(p + 0x150);
    if (slot) {
        void* obj = *(void**)((u8*)slot + 0x10);
        *(f32*)((u8*)obj + 0x2C) = pos[0];
        *(f32*)((u8*)obj + 0x30) = pos[1];
        *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s = *(void**)(p + 0x150 + i * 8);
        if (s) {
            void* obj = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
done:;
}

void func_8024E828(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(u8*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687A8;
    extern f32 lbl_eu_806687AC;
    u8* p = (u8*)self;
    if (p[0x58]) goto done;
    if (CSysWin_getUnk34(p + 0xB8)) goto done;
    if (CSysWin_getUnk34(p + 0xF4)) goto done;
    f32* pos = (f32*)(p + 0x44);
    pos[1] += lbl_eu_806687A8;
    if (pos[1] > lbl_eu_806687AC) pos[1] = lbl_eu_806687AC;
    for (int i = 0; i < 3; i++) {
        void* slot = *(void**)(p + 0x130 + i * 8);
        if (slot) {
            void* obj = *(void**)((u8*)slot + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    void* slot = *(void**)(p + 0x150);
    if (slot) {
        void* obj = *(void**)((u8*)slot + 0x10);
        *(f32*)((u8*)obj + 0x2C) = pos[0];
        *(f32*)((u8*)obj + 0x30) = pos[1];
        *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s = *(void**)(p + 0x150 + i * 8);
        if (s) {
            void* obj = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)obj + 0x2C) = pos[0];
            *(f32*)((u8*)obj + 0x30) = pos[1];
            *(f32*)((u8*)obj + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
done:;
}

void func_8024EA00(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(u8*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668798;
    extern f32 lbl_eu_806687A8;
    extern f32 lbl_eu_806687B0;
    extern f32 lbl_eu_806687B4;
    extern f32 lbl_eu_80668764;
    u8* p = (u8*)self;
    if (p[0x58] || CSysWin_getUnk34(p + 0xB8) || CSysWin_getUnk34(p + 0xF4)) return;
    void* slot = *(void**)(p + 0x130);
    if (!slot) slot = 0;
    void* obj = *(void**)((u8*)slot + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, &lbl_eu_8050BEA8[0x136], 1);
    f32 f3 = lbl_eu_806687B0 * *(f32*)((u8*)result + 0x44) - lbl_eu_806687B4;
    f32* pos = (f32*)(p + 0x44);
    *pos += lbl_eu_806687A8;
    f32 limit = lbl_eu_80668798 * f3;
    if (*pos > limit) *pos = limit;
    for (int i = 0; i < 3; i++) {
        void* s = *(void**)(p + 0x130 + i * 8);
        if (s) {
            void* o = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    void* s = *(void**)(p + 0x150);
    if (s) {
        void* o = *(void**)((u8*)s + 0x10);
        *(f32*)((u8*)o + 0x2C) = pos[0];
        *(f32*)((u8*)o + 0x30) = pos[1];
        *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s2 = *(void**)(p + 0x150 + i * 8);
        if (s2) {
            void* o = *(void**)((u8*)s2 + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
}

void func_8024EC24(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern u32 func_80248558(u8*);
    extern void func_8024577C(void*, u16);
    extern f32 lbl_eu_80668798;
    extern f32 lbl_eu_806687A8;
    extern f32 lbl_eu_806687B0;
    extern f32 lbl_eu_806687B4;
    extern f32 lbl_eu_806687B8;
    extern f32 lbl_eu_80668764;
    u8* p = (u8*)self;
    if (p[0x58] || CSysWin_getUnk34(p + 0xB8) || CSysWin_getUnk34(p + 0xF4)) return;
    void* slot = *(void**)(p + 0x130);
    if (!slot) slot = 0;
    void* obj = *(void**)((u8*)slot + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, &lbl_eu_8050BEA8[0x136], 1);
    f32 f4 = lbl_eu_806687B0 * *(f32*)((u8*)result + 0x44) - lbl_eu_806687B4;
    f32 f3 = lbl_eu_80668798 * f4;
    f32* pos = (f32*)(p + 0x44);
    *pos -= lbl_eu_806687A8;
    f32 limit = lbl_eu_806687B8 * f3;
    if (*pos < limit) *pos = limit;
    for (int i = 0; i < 3; i++) {
        void* s = *(void**)(p + 0x130 + i * 8);
        if (s) {
            void* o = *(void**)((u8*)s + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    void* s = *(void**)(p + 0x150);
    if (s) {
        void* o = *(void**)((u8*)s + 0x10);
        *(f32*)((u8*)o + 0x2C) = pos[0];
        *(f32*)((u8*)o + 0x30) = pos[1];
        *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
    }
    for (u8 i = 0; i < p[0x1F0]; i++) {
        void* s2 = *(void**)(p + 0x150 + i * 8);
        if (s2) {
            void* o = *(void**)((u8*)s2 + 0x10);
            *(f32*)((u8*)o + 0x2C) = pos[0];
            *(f32*)((u8*)o + 0x30) = pos[1];
            *(f32*)((u8*)o + 0x34) = lbl_eu_80668764;
        }
    }
    func_80248558(p + 0x140);
    func_8024577C(p + 0x1FC, *(u16*)(p + 0x5A));
    p[0x5D] = 1;
}

void func_8024EE50(){}

void func_8024F1FC(void* self, u32 arg2) {
    extern void func_8003AA34();
    extern void* getFP__FPCc(const char*);
    extern u8 lbl_eu_80664798;
    lbl_eu_80664798 = (u8)arg2;
    if (arg2 > 0x1C) return;
    func_8003AA34();
    u32 strs[] = {
        0x524, 0x534, 0x544, 0x554, 0x564, 0x574, 0x584, 0x594,
        0x5A4, 0x5B4, 0x5C4, 0x5D4, 0x5E4, 0x5F4, 0x604, 0x614,
        0x624, 0x634, 0x634, 0x644, 0x654, 0x664, 0x674, 0x684,
        0x634, 0x644, 0x654, 0x664
    };
    lbl_eu_8066479C = (u32)getFP__FPCc(&lbl_eu_8050BEA8[strs[arg2]]);
}

u32 func_8024F538(void* self) {
    u8 val = *(u8*)((u8*)self + 0x41);
    u32 result = __cntlzw(val - 2);
    return result >> 5;
}

u16 func_8024F54C(void* self) { return *(u16*)((u8*)self + 0x5A); }

u8 func_8024F554(void* self) { return static_cast<CFloorMapFull*>(self)->field_58; }

void func_8024F55C(void* self) {
    extern int CSysWin_isActive(void*);
    extern void func_801D216C(void*, int);
    extern void func_8022B8E4(void*);
    extern void func_80138078(unsigned long);
    u8* p = (u8*)self;
    if (p[0x58] && CSysWin_isActive(p + 0xB8)) {
        func_801D216C(p + 0xA0, 0);
        func_8022B8E4(p + 0xB8);
        p[0x58] = 0;
        func_80138078(6);
    }
}

typedef void* (*VFuncPtr)(void*, const char*, u32);
u32 getHandleMEM2__Q23mtl10MemManagerFv();
void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, char const*, void*, int, int);
int func_800A9D90();
void func_801F34F4(void*);

void func_8024C104(void* self) {
    u8* p = (u8*)self;
    u32 handle = getHandleMEM2__Q23mtl10MemManagerFv();
    *(void**)(p + 0x24) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, &lbl_eu_8050BEA8[0x4e7], self, 0, 0);
    u32 handle2 = func_800A9D90();
    *(void**)(p + 0x30) = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(handle2, &lbl_eu_8050BEA8[0x4fc], self, 0, 0);
    u32 buffer[29];
    u32* dst = &buffer[1];
    u32* src = lbl_eu_8050BDF8;
    buffer[0] = 0;
    int count = 14;
    do {
        *dst++ = *src++;
        *dst++ = *src++;
    } while (--count);
    handle = getHandleMEM2__Q23mtl10MemManagerFv();
    u8 idx = lbl_eu_80664798;
    *(void**)(p + 0x28) = readFile__11CDeviceFileFUlPCcP10IWorkEventii(handle, (char*)buffer[idx], self, 0, 0);
    func_801F34F4(p + 0x60);
    typedef void (*VoidVFuncPtr)(void*);
    VoidVFuncPtr* vt = *(VoidVFuncPtr**)(p + 0xB8);
    vt[0x20](p + 0xB8);
    vt = *(VoidVFuncPtr**)(p + 0xF4);
    vt[0x20](p + 0xF4);
}


void func_8024F5C4(void* self, u32 arg2) {
    void* ptr = *(void**)((u8*)self + 0x32D4);
    if (!ptr) return;
    void* obj = *(void**)((u8*)ptr + 0x10);
    VFuncPtr* vt = *(VFuncPtr**)obj;
    void* result = vt[15](obj, (char*)&lbl_eu_8050BEA8 + 0xEE, 1);
    *(u8*)((u8*)result + 0xBB) = (*(u8*)((u8*)result + 0xBB) & 0x7F) | (u8)arg2;
}

unsigned char func_8024F630(void) {
    return (unsigned char)func_8003B1EC(lbl_eu_8066479C);
}

void func_8024F658(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (p[0x58]) return;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    u8 val = p[0x208];
    u32 result = __cntlzw(val);
    p[0x208] = result >> 5;
}

u8 func_8024F6BC(void* self) {
    CFloorMapFull* full = static_cast<CFloorMapFull*>(self);
    if (full->field_58) return 0;
    return full->field_208;
}

u8 func_8024F6D8(void* self) { return static_cast<CFloorMapFull*>(self)->field_208; }

u32 func_8024F6E0(void* self) {
    s8 idx1 = *(s8*)((u8*)self + 0x206);
    u16 val;
    if (idx1 < 0) {
        val = 0;
    } else {
        s8 idx0 = *(s8*)((u8*)self + 0x205);
        s8 idx2 = *(s8*)((u8*)self + 0x207);
        u32 offset = idx0 * 0x30C + (idx2 + idx1) * 0x18;
        val = *(u16*)((u8*)self + offset + 0x214);
    }
    return val != 0 ? 1 : 0;
}

void func_8024F72C(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    p[0x333C] = (u32)__cntlzw(p[0x333C]) >> 5;
}

u32 func_8024F784(void* self) {
    extern int CSysWin_getUnk34(void*);
    u8* p = (u8*)self;
    if (CSysWin_getUnk34(p + 0xB8)) return 1;
    return CSysWin_getUnk34(p + 0xF4);
}

void func_8024F7CC(void* self) {
    extern int CSysWin_getUnk34(void*);
    extern int sprintf(char*, const char*, ...);
    extern void func_8024B4CC(void*, void*, void*);
    extern f32 lbl_eu_80668764;
    extern f32 lbl_eu_806687BC;
    extern u32 func_80248558(u8*);
    extern void func_8024577C(void*, u16);
    u8* p = (u8*)self;
    if (!p[0x5D]) return;
    p[0x5D] = 0;
    if (*(u32*)(p + 0x2C)) return;
    if (p[0x41] != 1) return;
    if (!p[0x208]) return;
    if (CSysWin_getUnk34(p + 0xB8)) return;
    if (CSysWin_getUnk34(p + 0xF4)) return;
    if (p[0x58]) return;
    s8 idx0 = (s8)p[0x205];
    u32 base = idx0 * 0x30C;
    if (!*(u8*)(p + base + 0x514)) return;
    s8 idx1 = (s8)p[0x206];
    u16 val = 0;
    if (idx1 >= 0) {
        s8 idx2 = (s8)p[0x207];
        val = *(u16*)(p + base + (idx2 + idx1) * 0x18 + 0x214);
    }
    if (!val) return;
    f32 pos[3] = {lbl_eu_80668764, lbl_eu_80668764, lbl_eu_80668764};
    char buf[0x20];
    sprintf(buf, &lbl_eu_8050BEA8[0x2EB]);
    void* data = *(void**)(p + 0x140);
    void* obj = *(void**)((u8*)data + 0x10);
    VFuncPtr* vtable = *(VFuncPtr**)obj;
    void* result = vtable[15](obj, buf, 1);
    if (result) {
        void* target = *(void**)((u8*)data + 0x10);
        void* node = result;
        if (node && *(void**)((u8*)node + 0x10) != target) {
            void* next = *(void**)((u8*)node + 0x0C);
            if (next && *(void**)((u8*)next + 0x10) != target) {
                void* next2 = *(void**)((u8*)next + 0x0C);
                if (next2 && *(void**)((u8*)next2 + 0x10) != target) {
                    f32 p1[3];
                    func_8024B4CC(p1, data, next2);
                    pos[0] += p1[0]; pos[1] += p1[1]; pos[2] += p1[2];
                }
                pos[0] += *(f32*)((u8*)next2 + 0x2C);
                pos[1] += *(f32*)((u8*)next2 + 0x30);
                pos[2] += *(f32*)((u8*)next2 + 0x34);
            }
            pos[0] += *(f32*)((u8*)next + 0x2C);
            pos[1] += *(f32*)((u8*)next + 0x30);
            pos[2] += *(f32*)((u8*)next + 0x34);
        }
        void* result2 = vtable[15](obj, &lbl_eu_8050BEA8[0x136], 1);
        if (result2) {
            f32 scale = *(f32*)((u8*)result2 + 0x44);
            pos[0] *= scale;
            pos[0] += *(f32*)((u8*)data + 0x2C);
            pos[1] += *(f32*)((u8*)data + 0x30);
            pos[2] += *(f32*)((u8*)data + 0x34);
        }
    }
    *(f32*)(p + 0x54) = lbl_eu_806687BC;
    p[0x41] = 2;
    *(f32*)(p + 0x4C) = pos[0] / lbl_eu_806687BC;
    *(f32*)(p + 0x50) = pos[1] / lbl_eu_806687BC;
}

u32 func_8024FB78() {
    extern u32 lbl_eu_80664184;
    extern u8 lbl_eu_80664798;
    extern u32 lbl_eu_8066479C;
    extern void* getPlayer__Q22cf13CfGameManagerFi(int);
    extern u32 func_8003B1EC(u32);
    extern s16 func_80136330(u32, const char*, u32);
    extern void func_80141DC4(f32*);
    extern f64 lbl_eu_80668770;
    u32 result = 0;
    if ((lbl_eu_80664184 & 0xFF) == lbl_eu_80664798) {
        if (!lbl_eu_8066479C) return 0;
        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
        if (!player) return 0;
        void** vt = *(void***)player;
        f32* pos = (f32*)((void*(*)(void*))vt[0xAC])(player);
        f32 y = pos[1];
        u32 count = func_8003B1EC(lbl_eu_8066479C);
        for (u32 i = 1; i <= count; i++) {
            s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
            if ((f32)(s16)val > y) return i;
        }
    } else {
        if (!lbl_eu_8066479C) return 0;
        f32 buf[3];
        func_80141DC4(buf);
        u32 count = func_8003B1EC(lbl_eu_8066479C);
        for (u32 i = 1; i <= count; i++) {
            s16 val = func_80136330(lbl_eu_8066479C, &lbl_eu_8050BEA8[0x15A], i);
            if ((f32)(s16)val > buf[1]) return i;
        }
    }
    return 0;
}

void CFloorMap::OnFileEvent() const {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_80250CB4() {
    extern u16 lbl_eu_806647A0[];
    extern u16 lbl_eu_806647A8[];
    extern u16 lbl_eu_806647B0[];
    extern u16 lbl_eu_806647B8[];
    for (int i = 0; i < 4; i++) {
        lbl_eu_806647A0[i] = 0xFF;
        lbl_eu_806647A8[i] = 0xFF;
    }
    lbl_eu_806647B0[0] = 0xA0;
    lbl_eu_806647B0[1] = 0x8C;
    lbl_eu_806647B0[2] = 0x23;
    lbl_eu_806647B0[3] = 0xFF;
    lbl_eu_806647B8[0] = 0xD9;
    lbl_eu_806647B8[1] = 0xC0;
    lbl_eu_806647B8[2] = 0x43;
    lbl_eu_806647B8[3] = 0xFF;
}
