// Auto-scaffolded catalog TU for kyoshin/CItemBoxGrid
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/CItemBoxGrid.cpp" line 3 "kyoshin/harness_catalog.hpp" */
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
/* "src/kyoshin/CItemBoxGrid.cpp" line 4 "kyoshin/CItemBoxGrid.hpp" */
#pragma once

/* "src/kyoshin/CItemBoxGrid.hpp" line 2 "types.h" */
/* end "types.h" */

class CQuestItem {
public:
    virtual ~CQuestItem();

    // TODO: add fields
};

class CVisionItem {
public:
    virtual ~CVisionItem();

    // TODO: add fields
};

class CArtsBookItem {
public:
    CArtsBookItem();
    virtual ~CArtsBookItem();

    // TODO: add fields
};

// Full object layout for CItemBoxGrid (used by C-linkage accessors)
struct CItemBoxGridFull {
    u8 _00[0x61];
    u8 field_61;
    u8 _62[0x52D - 0x62];
    u8 field_52D;
    u8 _52E[0x549 - 0x52E];
    u8 field_549;
    u8 _54A[0x2800 - 0x54A];
    u16 field_2800;
    u8 field_2802;
    u8 field_2803;
    u8 field_2804;
};

class CItemBoxGrid {
public:
    CItemBoxGrid();
    virtual ~CItemBoxGrid();
    void OnFileEvent();

    u8 GetField61();
    u8 GetField549();
    u8 GetField52D();
    void PushToList(unsigned char val);

    // TODO: add fields
};

/* end "kyoshin/CItemBoxGrid.hpp" */
/* "src/kyoshin/CItemBoxGrid.cpp" line 5 "stdio.h" */
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

// Forward declarations for external functions
extern "C" int func_8022D09C(void*);
extern "C" int func_8022DB74(void*);
extern "C" void __dl__FPv(void*);
extern "C" void* func_801394D4(u32);
extern "C" void func_801D4AE0(void*, int, void*);
extern "C" void func_801CFF28(void*);
extern "C" int CSysWin_isActive(void*);
extern "C" int CSysWin_getUnk34(void*);
extern "C" void func_801D216C(void*, int);
extern "C" void func_801D0950(void*);
extern "C" u32 func_80137510(u32, float);
extern "C" void* func_80157C4C(u8, s16);
extern "C" u32 func_801392E4(u32);
extern "C" u32 func_801361E8(u32, const char*, u32);
extern "C" u32 func_80139358(u32);
extern "C" void* CItem_initItemImplInstances(void*);
extern "C" u32 func_801D3320(void*);
extern "C" void func_80158118(void*, u32);
extern "C" u32 func_80208360(void*);
extern "C" void func_80208760(void*, void*);
extern "C" void func_8022E498(void*, void*);
extern "C" void func_8022D0F8(void*, void*, u8);
extern "C" u32 func_801D32DC(void*);
extern "C" u32 getItemBoxState__FP12CItemBoxInfo(void*);
extern "C" u32 func_801EB018(void*);
extern "C" u32 func_8022D08C(void*);
extern "C" u32 CSysWin_isReady(void*);
void* __dt__13CArtsBookItemFv(void* self, int mode);
void* __dt__11CVisionItemFv(void* self, int mode);
void* __dt__10CQuestItemFv(void* self, int mode);
extern "C" u32 func_801EB020(void*);
extern "C" u32 func_80208358(void*);
extern "C" u32 func_8022D094(void*);
extern "C" u32 func_8022DB6C(void*);
extern "C" void advanceItemBoxState__FP12CItemBoxInfo(void*);
extern "C" void func_801EB410(void*, int);
extern "C" void func_801EB0D4(void*);
extern "C" void func_801D0328(void*);
extern "C" void func_801CFFEC(void*);
extern "C" void func_801D05D4(void*, int);
extern "C" void func_80138078__FUl(u32);
extern "C" u32 func_8015780C(void*);
extern "C" u32 func_801D3328(void*);
extern "C" void func_801D3454(void*);
extern "C" void func_801D3620(void*);
extern "C" void func_801D3698(void*);
extern "C" void func_801D3724(void*);
extern "C" void func_801D377C(void*);
extern "C" void func_801D3408(void*);
extern "C" u32 func_801EB028(void*);
extern "C" u32 func_801EB064(void*);
extern "C" u32 func_801EB04C(void*);
extern "C" u32 func_801EB218(void*);
extern "C" u32 func_801EB314(void*);
extern "C" void func_801EB178(void*);
extern "C" void func_8020844C(void*);
extern "C" void func_802083A4(void*);
extern "C" void func_80208838(void*);
extern "C" void func_8022E3A8(void*);
extern "C" void func_8022E490(void*);
extern "C" void func_8022E2F8(void*);
extern "C" void func_8022E3AC(void*);
extern "C" void func_8022E488(void*);
extern "C" void func_8022DD68(void*);
extern "C" void func_8022D0D0(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void func_801D47D4(void*, u32, u32, u32);
extern "C" u32 func_801D421C(void*);
extern "C" void func_801D4C3C(void*, u32);
extern "C" void func_80124270(void*, u32);
extern "C" u32 func_80137E7C(void*, u32);
extern "C" u32 func_80137924(void*, u32);
extern "C" u32 func_8013600C(void*, u32);
extern "C" u32 func_800A32BC(u32);
extern "C" u32 func_8003B1EC(u32);
extern "C" u32 func_8009CF8C(u32);
extern "C" u32 func_80139A18(void*);
extern "C" void func_80136190(void*, u32, u32);
extern "C" void func_801C5158(void*, u32);
extern "C" u32 func_801392B4(void*);
extern "C" u32 func_801393CC(void*);
extern "C" u32 func_8026178C(void*);
extern "C" u32 func_8025FB10(void*);
extern "C" u32 func_802083CC(void*);
extern "C" u32 func_8009EC9C(u32);
extern "C" u32 func_800A082C(void);
extern "C" u32 code80135FDC_getByte_64077(void*);
extern "C" void func_801C4B60(void*, u8, u8, u8, u8);
extern "C" void func_801D1F9C(void*, u32);
extern "C" u32 func_801C62AC(void*, u32);
extern "C" u32 func_801C631C(void*, u32);
extern "C" u16 ArrayGet12(u8);
extern "C" int func_801C6E90(void*);
extern "C" u32 func_801D4260(void*, u8);
extern "C" void func_801D0BD8(void*);
extern "C" void func_801CFD2C(void*);
extern const float lbl_eu_80667F40;
extern const float lbl_eu_80667F48;
extern const float lbl_eu_80667F80;
extern float lbl_eu_80667F78;
extern const float lbl_eu_80667F30;
extern const float lbl_eu_80667F34;
extern const float lbl_eu_80667F38;
extern const float lbl_eu_80667F50;
extern const float lbl_eu_80667F54;
extern const float lbl_eu_80667F58;
extern const float lbl_eu_80667F74;
extern const float lbl_eu_80667F7C;
extern const float lbl_eu_80667F88;
extern const float lbl_eu_80667F8C;
extern const float lbl_eu_80667F90;
extern const float lbl_eu_80667F94;
extern const float lbl_eu_80667F98;

extern u32 lbl_eu_80664098;
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640EC;

extern u32 lbl_eu_806640F8;
extern u32 lbl_eu_80664104;
extern u32 lbl_eu_80664110;
extern u32 lbl_eu_80664488;
extern u32 lbl_eu_80664490;
extern u32 lbl_eu_80664498;
extern u32 lbl_eu_806644A0;
extern u32 lbl_eu_806644A8;
extern u32 lbl_eu_806644B0;
extern u32 lbl_eu_806644B8;
extern u32 lbl_eu_806644C0;
extern u32 lbl_eu_806644C8;
extern u32 lbl_eu_806644D0;
extern u32 lbl_eu_806644D8;
extern u32 lbl_eu_806644E0;
extern u32 lbl_eu_806644E8;
extern u32 lbl_eu_806644F0;
extern u32 lbl_eu_806644F8;
extern u32 lbl_eu_80664500;
extern u32 lbl_eu_80664508;
extern u32 lbl_eu_8066450C;
extern u32 lbl_eu_80664510;

extern const char lbl_eu_80505590[];
extern const char lbl_eu_805055F0[];
extern const char lbl_eu_8050560C[];
extern const char lbl_eu_80505628[];
extern const char lbl_eu_80534740[];
extern const char lbl_eu_80534818[];
extern const char lbl_eu_80573D18[];
extern const char lbl_eu_8050566C[];
extern u32 lbl_eu_80664514;
extern char lbl_eu_806640F4[];
extern "C" u32 func_80157C20(u8);
extern "C" void func_801CE390(void*);
extern "C" float func_801C9F88(void*, void*);
extern "C" u32 func_80137444__FPQ34nw4r3lyt13AnimTransformf(void*, float);
extern "C" void CopyEntry9Bytes(char*, const char*);
extern "C" void __ct__CVisionItem(void*);
extern "C" void __ct__CArtsBookItem(void*);
extern "C" void SetEntry9Bytes(unsigned char*, unsigned short, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);
extern "C" void func_801C562C(void*, char*);

extern "C" u8 func_801C67F8(CItemBoxGridFull* self);
extern "C" u8 func_801C6840(CItemBoxGridFull* self);

u8 CItemBoxGrid::GetField61() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_61; }

// Constructor for sub-item grid.
void* __ct__801C5514(void* self) {
    u8* p = (u8*)self;
    u32 i;
    // Initialize entry array
    u8* end = p + 0x2800;
    u8* cur = p;
    while (cur < end) {
        SetEntry9Bytes(cur, (unsigned short)0xFFFF,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0);
        cur += 10;
    }
    *(u16*)(p + 0x2800) = 0;
    p[0x2802] = 0;
    p[0x2803] = 0;
    p[0x2804] = 0;
    *(u32*)(p + 0x2CA8) = (u32)&lbl_eu_80534818;
    *(u16*)(p + 0x34AC) = 0;
    __ct__CVisionItem(p + 0x34B0);
    __ct__CArtsBookItem(p + 0x3CB8);
    lbl_eu_80664514 = (u32)self;
    for (i = 0; i < 0x400; i++) {
        char tmp[27];
        SetEntry9Bytes((unsigned char*)tmp, (unsigned short)0xFFFF,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0, (unsigned char)0, (unsigned char)0,
                       (unsigned char)0);
        // func_801C562C copies from tmp to p+i*10
        func_801C562C(p + i * 10, tmp);
    }
    return self;
}

void SetEntry9Bytes(unsigned char* p, unsigned short a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h) {
    unsigned char* buf = (unsigned char*)p;
    *((unsigned short*)(buf + 0)) = a;
    buf[2] = b;
    buf[3] = c;
    buf[4] = d;
    buf[5] = e;
    buf[6] = f;
    buf[7] = g;
    buf[8] = h;
}

// Copy a 9-byte entry from src to dst.
void func_801C562C(void* dst, void* src) {
    u8* d = (u8*)dst;
    u8* s = (u8*)src;
    short v0 = *(short*)(s + 0);
    u8 v1 = s[2];
    u8 v2 = s[3];
    u8 v3 = s[4];
    u8 v4 = s[5];
    u8 v5 = s[6];
    u8 v6 = s[7];
    u8 v7 = s[8];
    *(short*)(d + 0) = v0;
    d[2] = v1;
    d[3] = v2;
    d[4] = v3;
    d[5] = v4;
    d[6] = v5;
    d[7] = v6;
    d[8] = v7;
}

// Destructor for the main grid container.
void* __dt__801C5670(void* self, int mode) {
    u8* p = (u8*)self;
    if (!self) return self;
    lbl_eu_80664514 = 0;
    __dt__13CArtsBookItemFv(p + 0x3cb8, 0xFFFFFFFF);
    __dt__11CVisionItemFv(p + 0x34b0, 0xFFFFFFFF);
    if (mode > 0) __dl__FPv(self);
    return self;
}

void func_801C56D8() { }

// Search for a matching short id in an array, return 1 if found.
int func_801C51BC(void* obj, u32 id) {
    u16 count = *(u16*)((u8*)obj + 0x804);
    u16 i;
    for (i = 0; i < count; i++) {
        if (*(u16*)((u8*)obj + 4 + i * 2) == (u16)id) {
            return 1;
        }
    }
    return 0;
}

// Convert category to timing value - complex float math
// Compute grid row count.
u32 func_801C5E5C(void* self) {
    u8* p = (u8*)self;
    u32 val = func_80157C20(p[0x2802]);
    float f = (float)(s32)val;
    float f50 = lbl_eu_80667F50;
    if (f >= f50) return 10;
    float div = f / lbl_eu_80667F30;
    s32 result = (s32)div;
    float frac = div - (float)result;
    if (lbl_eu_80667F34 != frac) result++;
    return (u8)result;
}

// Increment a sub-index counter; wrap to 0 when reaching the limit.
void func_801C5EF4(CItemBoxGridFull* self) {
    u8 idx = self->field_2804 + 1;
    self->field_2804 = idx;
    if ((s8)idx >= self->field_2803) {
        self->field_2804 = 0;
    }
}

// Decrement a sub-index counter; borrow from the limit field when underflowing.
void func_801C5F20(CItemBoxGridFull* self) {
    u8 idx = self->field_2804 - 1;
    self->field_2804 = idx;
    if ((s8)idx < 0) {
        self->field_2804 = self->field_2803 - 1;
    }
}

// Lookup entry in 10-byte stride table; return stored short or -1.
s16 func_801C5F48(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return -1;
    return *(s16*)((u8*)self + offset * 0xa);
}

void func_801C5FC0() { }

// Round a double to nearest integer with .5 tie-breaking biased away from zero.
long func_801C6158(double f) {
    if (f > 0.0) {
        return (long)(f + 0.5);
    } else {
        return (long)(f - 0.5);
    }
}

void func_801C618C() { }

// Lookup entry, check category; return word >> 20 or 0.
u32 func_801C62AC(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    s16 val = *(s16*)((u8*)self + offset * 0xa);
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return *(u32*)obj >> 20;
}

// Lookup entry, check category; return obj ptr or 0.
void* func_801C631C(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    s16 val = *(s16*)((u8*)self + offset * 0xa);
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return obj;
}

// Check item entry validity based on category.
s32 func_801C6388(CItemBoxGridFull* self, u16 idx) {
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= *(u16*)(p + 0x2800)) return 0;
    s16 val = *(s16*)(p + offset * 0xa);
    void* obj = func_80157C4C(p[0x2802], val);
    if (!obj || !*(u32*)obj) return 0;
    u8 cat = p[0x2802];
    if ((u32)(cat - 4) <= 4) {
        // category 4-8
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        u8 result = (u8)((u32(*)(void*, void*))vtbl[12])(inst, obj);
        if (!result) {
            u8 v2 = ((u8*)p + offset * 0xa)[2];
            return (s8)((v2 - 1) | 1);
        }
        u32 j;
        for (j = 0; j < result; j++) {
            void* inst2 = CItem_initItemImplInstances(obj);
            void** vtbl2 = *(void***)inst2;
            s16 r = (s16)((s32(*)(void*, void*, u32))vtbl2[16])(inst2, obj, j);
            if (r == -1) {
                void* inst3 = CItem_initItemImplInstances(obj);
                void** vtbl3 = *(void***)inst3;
                void* res3 = ((void*(*)(void*, void*, u32))vtbl3[11])(inst3, obj, j);
                if (res3 && (*(u16*)((u8*)res3 + 4) & 1)) return -3;
            }
        }
        return -2;
    } else if ((u32)(cat - 10) <= 3) {
        // category 10-13
        u8 v6 = ((u8*)obj)[6];
        return (s8)v6;
    } else if (cat == 2) {
        // same as cat 4-8
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        u8 result = (u8)((u32(*)(void*, void*))vtbl[12])(inst, obj);
        if (!result) {
            u8 v2 = ((u8*)p + offset * 0xa)[2];
            return (s8)((v2 - 1) | 1);
        }
        u32 j;
        for (j = 0; j < result; j++) {
            void* inst2 = CItem_initItemImplInstances(obj);
            void** vtbl2 = *(void***)inst2;
            s16 r = (s16)((s32(*)(void*, void*, u32))vtbl2[16])(inst2, obj, j);
            if (r == -1) {
                void* inst3 = CItem_initItemImplInstances(obj);
                void** vtbl3 = *(void***)inst3;
                void* res3 = ((void*(*)(void*, void*, u32))vtbl3[11])(inst3, obj, j);
                if (res3 && (*(u16*)((u8*)res3 + 4) & 1)) return -3;
            }
        }
        return -2;
    } else if (cat == 3) {
        u8 v2 = ((u8*)p + offset * 0xa)[2];
        return (s8)((v2 - 1) | 1);
    } else if (cat == 9) {
        return 0;
    }
    return 0;
}

// Lookup entry, check category; return byte at offset 2 or 0.
u8 func_801C6528(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[2];
}

// Lookup entry, check category; return byte at offset 4 or 0.
u8 func_801C65A0(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[4];
}

// Lookup entry, check category; return byte at offset 5 or 0.
u8 func_801C6618(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[5];
}

// Lookup entry, check category; return byte at offset 3 or 0.
u8 func_801C6690(CItemBoxGridFull* self, u16 idx) {
    s8 base = (s8)self->field_2804;
    u16 offset = (u16)(base * 0x1e + idx);
    if (offset >= self->field_2800) return 0;
    void* entry = (u8*)self + offset * 0xa;
    s16 val = *(s16*)entry;
    void* obj = func_80157C4C(self->field_2802, val);
    if (!obj || !*(u32*)obj) return 0;
    return ((u8*)entry)[3];
}

// Lookup a byte from a 10-byte-entry table indexed by (field_2804 * 0x1e + idx).
// Returns byte at offset 7 within the entry, or 0 if out of bounds.
u8 func_801C6708(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return ((u8*)self)[offset * 0xa + 7];
    }
    return 0;
}

u8 func_801C673C(CItemBoxGridFull* self, u16 idx) {
    s32 tmp = (s8)self->field_2804 * 0x1e + idx;
    u32 offset = (u32)(u16)tmp;
    if (offset < 0x400) {
        return ((u8*)self)[offset * 0xa + 8];
    }
    return 0;
}


// Toggle an entry's flag based on category cap.
void func_801C6770(CItemBoxGridFull* self, u16 idx) {
    u16 offset = (u16)((s8)self->field_2804 * 0x1e + idx);
    if (offset >= 0x400) return;
    u8* entry = (u8*)self + offset * 0xa;
    if (entry[8] == 0) {
        u8 cap = func_801C6840(self);
        if (func_801C67F8(self) >= cap) return;
    }
    entry[8] = entry[8] ? 0 : 1;
}

// Count entries with non-zero byte at offset 8 in a 10-byte stride array.
u8 func_801C67F8(CItemBoxGridFull* self) {
    u16 count = self->field_2800;
    u16 i;
    u16 result = 0;
    for (i = 0; i < count; i++) {
        u8* entry = (u8*)self + i * 0xa;
        if (entry[8] != 0) {
            result++;
        }
    }
    return (u8)result;
}

// Return a duration/stride value based on the category byte at offset 0x2802.
u8 func_801C6840(CItemBoxGridFull* self) {
    u8 cat = self->field_2802;
    u32 d = cat - 4;
    if (d <= 4) goto ret30;
    if (cat == 2) goto ret30;
    if (cat == 0xb) {
        return 0x3c;
    }
    return 0;
ret30:
    return 0x1e;
}

int LookupIndexedByte(char* obj) {
    char off = *(signed char*)((char*)obj + 0x2804);
    if (off >= 0x400) return 0;
    return *(unsigned char*)((char*)obj + off + 0x28a5);
}

// Iterate entries and init item instances.
void func_801C68A0(CItemBoxGridFull* self) {
    u32 i;
    for (i = 0; i < self->field_2800; i++) {
        u8* entry = (u8*)self + i * 10;
        if (entry[8] != 0) continue;
        s16 val = *(s16*)entry;
        void* obj = func_80157C4C(self->field_2802, val);
        if (!obj || !*(u32*)obj) continue;
        void* inst = CItem_initItemImplInstances(obj);
        void** vtbl = *(void***)inst;
        ((void(*)(void*, void*))vtbl[4])(inst, obj);
    }
}

u32 func_801C6938(void* self, u32 idx) { return 0; }

void func_801C6A44() { }

// Check if an object has type 9 (extracted from vtable bits) and subtype 2.
int func_801C6E90(void* obj) {
    u32 w = *(u32*)obj;
    u32 type = (w >> 16) & 0xF;
    int result = 0;
    if (type == 9) {
        u8 sub = *(u8*)((u8*)obj + 7) & 3;
        if (sub == 2) {
            result = 1;
        }
    }
    return result;
}

// Check entry state.
u32 func_801C6EC0(CItemBoxGridFull* self, u16 idx) {
    u8* p = (u8*)self;
    s8 base = (s8)p[0x2804];
    u16 offset = (u16)(base * 0x1e + idx);
    u16 count = *(u16*)(p + 0x2800);
    if (offset >= count) return 0;
    void* obj = func_80157C4C(p[0x2802], *(s16*)(p + offset * 10));
    if (!obj) return 0;
    return func_8015780C(obj);
}

// Clear inactive entries and sort remaining.
void func_801C7730(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    u16 count = self->field_2800;
    u16 i;
    for (i = 0; i < count; i++) {
        u8* e = p + i * 10;
        if (e[8]) {
            SetEntry9Bytes((unsigned char*)e, (unsigned short)0xFFFF, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0, (unsigned char)0);
            self->field_2800--;
        }
    }
    // Bubble sort remaining
    u16 pass;
    for (pass = 0; pass < 0x3FF; pass++) {
        u16 stay = 0x3FF - pass;
        u16 j;
        u32 swapped = 0;
        for (j = 0; j < stay; j++) {
            u8* e1 = p + j * 10;
            u8* e2 = p + (j + 1) * 10;
            s16 v1 = *(s16*)e1;
            s16 v2 = *(s16*)e2;
            u32 a = v1 + 1;
            u32 b = v2 + 1;
            if (a <= b) continue;
            char tmp[27];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

void CopyEntry9Bytes(char* dst, const char* src) {
    *(short*)((char*)dst + 0) = *(short*)((char*)src + 0);
    *(char*)((char*)dst + 2) = *(char*)((char*)src + 2);
    *(char*)((char*)dst + 3) = *(char*)((char*)src + 3);
    *(char*)((char*)dst + 4) = *(char*)((char*)src + 4);
    *(char*)((char*)dst + 5) = *(char*)((char*)src + 5);
    *(char*)((char*)dst + 6) = *(char*)((char*)src + 6);
    *(char*)((char*)dst + 7) = *(char*)((char*)src + 7);
    *(char*)((char*)dst + 8) = *(char*)((char*)src + 8);
}

void func_801C7958() { }

void func_801C7C7C() { }

// Sort entries with item instance comparison.
void func_801C7EF0(CItemBoxGridFull* self, u32 mode) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            s16 v1 = *(s16*)e1;
            s16 v2 = *(s16*)e2;
            void* obj1 = func_80157C4C(self->field_2802, v1);
            void* obj2 = func_80157C4C(self->field_2802, v2);
            if (!obj1 || !obj2) continue;
            // Compare based on mode
            u32 cmp = 0;
            if (mode == 3) {
                cmp = CItem_initItemImplInstances(obj2) > CItem_initItemImplInstances(obj1);
            } else {
                u16 w1 = *(u16*)((u8*)obj1 + 4);
                u16 w2 = *(u16*)((u8*)obj2 + 4);
                cmp = w1 > w2;
            }
            if (!cmp) continue;
            // Swap using CopyEntry9Bytes / func_801C562C
            char tmp[9];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes((char*)e1, (const char*)e2);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C81D0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C82D0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Sort entries by kind.
void func_801C83E4(CItemBoxGridFull* self) {
    u8* p = (u8*)self;
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = p + j * 10;
            u8* e2 = p + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            u32 w1 = *(u32*)obj1;
            u32 k1 = func_80139358(w1 >> 20);
            u32 w2 = *(u32*)obj2;
            u32 k2 = func_80139358(w2 >> 20);
            u32 c1 = func_801361E8(lbl_eu_80664104, &lbl_eu_8050566C[0x212], k1 & 0xFFFF);
            u32 c2 = func_801361E8(lbl_eu_80664104, &lbl_eu_8050566C[0x212], k2 & 0xFFFF);
            if ((c1 & 0xFF) <= (c2 & 0xFF)) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C8534(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C8634(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte at offset 6.
void func_801C87CC(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        u8 tmp[16];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            u8 v1 = e1[6];
            u8 v2 = e2[6];
            if (v1 <= v2) continue;
            CopyEntry9Bytes((char*)tmp, (const char*)e1);
            CopyEntry9Bytes((char*)tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes((char*)tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte at offset 6.
void func_801C88B0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            u8 v1 = e1[6];
            u8 v2 = e2[6];
            if (v1 <= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble-sort entries using item comparison.
void func_801C8994(CItemBoxGridFull* self) {
    u32 i;
    for (i = 0; i < self->field_2800 - 1; i++) {
        u32 j;
        int swapped = 0;
        u32 limit = self->field_2800 - 1 - i;
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            s16 val1 = *(s16*)e1;
            s16 val2 = *(s16*)e2;
            void* obj1 = func_80157C4C(self->field_2802, val1);
            void* obj2 = func_80157C4C(self->field_2802, val2);
            u16 w1 = *(u16*)((u8*)obj1 + 4);
            u16 w2 = *(u16*)((u8*)obj2 + 4);
            if (w1 <= w2) continue;
            // Swap entries
            u8 tmp[9];
            // CopyEntry9Bytes
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C8ACC(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C8C58(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C8DE4(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            char tmp[27];
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C8F04(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9040(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9158(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9270(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9390(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C94E0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9630(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9780(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C98D0(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9A3C(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by byte comparison.
void func_801C9B8C(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            if (e1[6] <= e2[6]) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9CCC(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

// Bubble sort entries by float comparison.
void func_801C9E1C(CItemBoxGridFull* self) {
    u32 n = self->field_2800;
    u32 i;
    for (i = 0; i < n - 1; i++) {
        u32 limit = n - 1 - i;
        u32 j;
        int swapped = 0;
        char tmp[27];
        for (j = 0; j < limit; j++) {
            u8* e1 = (u8*)self + j * 10;
            u8* e2 = (u8*)self + (j + 1) * 10;
            void* obj1 = func_80157C4C(self->field_2802, *(s16*)e1);
            void* obj2 = func_80157C4C(self->field_2802, *(s16*)e2);
            float v1 = func_801C9F88(self, obj1);
            float v2 = func_801C9F88(self, obj2);
            if (v1 >= v2) continue;
            CopyEntry9Bytes(tmp, (const char*)e1);
            CopyEntry9Bytes(tmp + 9, (const char*)e2);
            func_801C562C(e1, tmp + 9);
            CopyEntry9Bytes(tmp + 18, (const char*)tmp);
            func_801C562C(e2, tmp);
            swapped = 1;
        }
        if (!swapped) break;
    }
}

float func_801C9F88(void* self, void* entry) { return 0.0f; }

// Check item slots for first valid entry.
u32 func_801CA070(void* self, void* item) {
    void* inst = CItem_initItemImplInstances(item);
    void** vtbl = *(void***)inst;
    u16 count = (u16)((u32(*)(void*, void*))vtbl[12])(inst, item);
    u32 i;
    for (i = 0; i < count; i++) {
        void* inst2 = CItem_initItemImplInstances(item);
        void** vtbl2 = *(void***)inst2;
        void* obj = ((void*(*)(void*, void*, u32))vtbl2[11])(inst2, item, i);
        if (!obj) continue;
        if (*(u16*)((u8*)obj + 4) & 1) return 1;
    }
    return 0;
}

// Check if entry kind exists in pool.
u32 func_801CA110(void* self, void* entry) {
    u32 val = *(u32*)entry;
    u32 obj = *(u32*)(lbl_eu_806640F4 + val);
    u32 kind = func_80139358(val >> 20);
    u32 i;
    for (i = 1; i <= 10; i++) {
        char buf[64];
        sprintf(buf, &lbl_eu_8050566C[0x132], (u8)i);
        if (func_801361E8(obj, buf, kind & 0xFFFF)) return i;
    }
    return 0;
}

void __ct__CItemBoxGrid() { }

void __dt__12CItemBoxGridFv() { }

void func_801CAA6C() { }

void func_801CABC8() { }

void func_801CAD8C() { }

void func_801CAE9C() { }

// Check if item grid is fully ready.
u32 func_801CB038(void* self) {
    u8* p = (u8*)self;
    if (!func_801D32DC(p + 0xe8)) return 0;
    if (!getItemBoxState__FP12CItemBoxInfo(p + 0x1d8)) return 0;
    if (!func_801EB018(p + 0x3e4)) return 0;
    if (!func_8022D08C(p + 0x440)) return 0;
    if (!CSysWin_isReady(p + 0x4ac)) return 0;
    if (CSysWin_isReady(p + 0x4e8)) return p[0x60];
    return 0;
}


// Check if item grid is active.
u32 func_801CB0FC(void* self) {
    u8* p = (u8*)self;
    if (p[0x528]) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    if (CSysWin_getUnk34(p + 0x4e8)) return 1;
    if (p[0x544]) return 1;
    return func_801D3320(p + 0xe8);
}

// Get field depending on window state.
u8 func_801CB184(void* self) {
    if (CSysWin_getUnk34((u8*)self + 0x4e8)) return 0;
    s32 state = *(s32*)((u8*)self + 0x58);
    if (state != 3) return 0;
    return ((u8*)self)[0x542];
}

u8 CItemBoxGrid::GetField549() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_549; }

// Check if any sub-system is active.
u32 func_801CB1E4(void* self) {
    u8* p = (u8*)self;
    if (func_801EB020(p + 0x3e4)) return 1;
    if (func_80208358(p + 0x418)) return 1;
    if (func_8022D094(p + 0x440)) return 1;
    if (func_8022DB6C(p + 0x468)) return 1;
    if (CSysWin_getUnk34(p + 0x4ac)) return 1;
    return CSysWin_getUnk34(p + 0x4e8);
}

// Initialize item display state.
void func_801CB28C(void* self) {
    u8* p = (u8*)self;
    u32 state = *(u32*)(p + 0x58);
    if (state) return;
    *(u32*)(p + 0x58) = 1;
    p[0x61] = 0;
    func_801CFD2C(self);
    func_801D0BD8(self);
    func_801D421C(p + 0x1D8);
    s8 off = (s8)p[0x6F];
    u8 val = p[off + 0x62];
    func_801D4260(p + 0x1D8, val);
    u8 idx0 = p[0x525];
    u8 idx1 = p[0x524];
    u32 entry_idx = (idx1 + idx0 * 10) & 0xFF;
    u8* sub = p + 0x54C;
    u32 r1 = func_801C631C(sub, entry_idx);
    u32 r2 = func_801C62AC(sub, entry_idx);
    func_801D47D4(p + 0x1D8, r2 & 0xFFFF, r1, 1);
    u32 v = func_801C6938(sub, entry_idx);
    func_801D4AE0((void*)(p + 0x1D8), 1, (void*)v);
    func_801D216C(p + 0xB8, 0);
    p[0x549] = 0;
    if (p[0x527] == 4) func_801D4C3C(p + 0x1D8, 1);
}

// Advance item box state.
void func_801CB38C(void* self) {
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58) != 3) return;
    if (func_801D3320(p + 0xe8)) return;
    *(u32*)(p + 0x58) = 4;
    void* obj = (void*)*(u32*)(p + 0x44);
    void** vtbl = *(void***)obj;
    ((void(*)(void*, void*, int))vtbl[11])(obj, (void*)*(u32*)(p + 0x50), 0);
    ((void(*)(void*, void*, int))vtbl[11])(obj, (void*)*(u32*)(p + 0x48), 0);
    ((void(*)(void*, void*, int))vtbl[11])(obj, (void*)*(u32*)(p + 0x4c), 1);
    p[0x61] = 0;
    func_801D216C(p + 0x70, 0);
    func_801D216C(p + 0x88, 0);
    func_801D216C(p + 0xd0, 0);
    func_801D216C(p + 0xb8, 0);
    advanceItemBoxState__FP12CItemBoxInfo(p + 0x1d8);
    if (!p[0x52c]) func_80138078__FUl(6);
}

// Clear a 14-byte region (list/array init).
void func_801CB480(void* self) {
    int i;
    u8* p = (u8*)self;
    for (i = 0; i < 12; i++) p[0x62 + i] = 0;
    p[0x6e] = 0;
    p[0x6f] = 0;
}

void CItemBoxGrid::PushToList(unsigned char val) {
    unsigned char count = reinterpret_cast<unsigned char*>(this)[0x6e];
    if (count >= 0xc) {
        return;
    }
    reinterpret_cast<unsigned char*>(this)[0x62 + count] = val;
    reinterpret_cast<unsigned char*>(this)[0x6e] = count + 1;
}

// Increment list counter with wrap.
void func_801CB4E4(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] + 1;
    p[0x6f] = idx;
    if ((s8)idx >= (s8)p[0x6e]) {
        p[0x6f] = 0;
    }
    func_801D05D4(self, 0);
    func_801CFF28(self);
    func_80138078__FUl(0x70);
}

// Decrement a list counter with wrap.
void func_801CB56C(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    u8 idx = p[0x6f] - 1;
    p[0x6f] = idx;
    if ((s8)idx < 0) {
        p[0x6f] = p[0x6e] - 1;
    }
    func_801D05D4(self, 1);
    func_801CFF28(self);
    func_80138078__FUl(0x70);
}

void func_801CB5F0() { }

unsigned short ArrayGet12(const unsigned short* p, unsigned char i) {
    if (i < 12) {
        return p[i];
    }
    return 0;
}

// Copy 3 words (12 bytes) from a 12-byte-entry array at index idx.
void func_801CB9D8(u32* dst, u32* src, int idx) {
    if (idx >= 12) return;
    u32* entry = src + idx * 3; // each entry is 12 bytes = 3 words
    dst[0] = entry[6];          // offset 0x18
    dst[1] = entry[7];          // offset 0x1C
    dst[2] = entry[8];          // offset 0x20
}

void func_801CBA04() { }

void func_801CBDE8() { }

void func_801CC0EC() { }

// Grid state update with conditions.
// Grid state update.
void func_801CC3F4(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    // Inline func_801C5EF4 for sub-struct at offset 0x54c
    u8* sub = p + 0x54c;
    u8 idx = sub[0x2804] + 1;
    sub[0x2804] = idx;
    if ((s8)idx >= (s8)sub[0x2803]) sub[0x2804] = 0;
    // LookupIndexedByte for sub
    u8 val;
    s8 off = (s8)sub[0x2804];
    if (off < 0x400) val = sub[off + 0x28a5]; else val = 0;
    if ((s8)p[0x525] < (s8)val) p[0x525] = val - 1;
    func_801CFFEC(self);
    func_801D0328(self);
    u8 f = sub[0x2803] ? sub[0x2803] : 1;
    if (f != 1) func_80138078__FUl(0xa);
}

// Grid state update (decrement variant).
void func_801CC4E8(void* self) {
    u8* p = (u8*)self;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x528]) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    u8* sub = p + 0x54c;
    u8 idx = sub[0x2804] - 1;
    sub[0x2804] = idx;
    if ((s8)idx < 0) sub[0x2804] = sub[0x2803] - 1;
    u8 val;
    s8 off = (s8)sub[0x2804];
    if (off < 0x400) val = sub[off + 0x28a5]; else val = 0;
    if ((s8)p[0x525] < (s8)val) p[0x525] = val - 1;
    func_801CFFEC(self);
    func_801D0328(self);
    u8 f = sub[0x2803] ? sub[0x2803] : 1;
    if (f != 1) func_80138078__FUl(0xa);
}

void func_801CC5DC() { }

void func_801CC7B0() { }

void func_801CCAF0() { }

// Store value and call helpers.
void func_801CDB94(void* self, u32 val) {
    *(u16*)((u8*)self + 0x52e) = (u16)val;
    void* res = func_801394D4(val & 0xFFFF);
    func_801D4AE0((void*)((u8*)self + 0x1d8), 0, res);
    func_801CFF28(self);
}

// Get field depending on obj state.
u8 func_801CDBE0(void* self) {
    if (!func_8022D09C((u8*)self + 0x440)) return 0;
    s32 state = *(s32*)((u8*)self + 0x58);
    if (state != 3) return 0;
    return ((u8*)self)[0x52c];
}

u8 CItemBoxGrid::GetField52D() { return reinterpret_cast<CItemBoxGridFull*>(this)->field_52D; }

void func_801CDC40() { }

// Check conditions and update state.
void func_801CDEE8(void* self) {
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58) != 3) return;
    if (p[0x528]) return;
    if (p[0x525] == 0xFF) return;
    if (CSysWin_getUnk34(p + 0x4ac)) return;
    if (CSysWin_getUnk34(p + 0x4e8)) return;
    if (func_80208358(p + 0x418)) return;
    if (func_8022DB6C(p + 0x468)) return;
    if (func_801D3320(p + 0xe8)) return;
    if (p[0x544]) return;
    p[0x525] = 0xFF;
    func_801D0950(self);
    func_801D0328(self);
    func_80138078__FUl(2);
}

// Check prerequisites and return result.
u32 func_801CDFB4(void* self) {
    u8* p = (u8*)self;
    if (*(u32*)(p + 0x58) != 3) return 0;
    if (func_801EB020(p + 0x3e4)) return 0;
    if (func_80208358(p + 0x418)) return 0;
    if (func_8022D094(p + 0x440)) return 0;
    if (func_8022DB6C(p + 0x468)) return 0;
    if (CSysWin_getUnk34(p + 0x4ac)) return 0;
    if (CSysWin_getUnk34(p + 0x4e8)) return 0;
    return 1;
}

// Animate grid panels (alt variant).
void func_801CE108(void* self) {
    u8* p = (u8*)self;
    u32 obj = *(u32*)(p + 0x48);
    float f = lbl_eu_80667F78;
    if (!func_80137444__FPQ34nw4r3lyt13AnimTransformf((void*)obj, f)) return;
    void** vtbl;
    u32 ptr44 = *(u32*)(p + 0x44);
    u32 ptr50 = *(u32*)(p + 0x50);
    vtbl = *(void***)ptr44;
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr50, 0);
    u32 ptr48 = *(u32*)(p + 0x48);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr48, 0);
    u32 ptr4C = *(u32*)(p + 0x4C);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr4C, 1);
    *(u32*)(p + 0x58) = 2;
}

void func_801CE1A0() { }


namespace nw4r { namespace lyt { class AnimTransform; } }
void func_80137444(nw4r::lyt::AnimTransform*, float);

void CheckState4_Animate(char* self) {
    if (*(unsigned char*)((char*)self + 0x527) != 4) {
        return;
    }
    func_80137444(*(nw4r::lyt::AnimTransform**)((char*)self + 0x50), lbl_eu_80667F78);
}


// Animate grid panels.
void func_801CE2F8(void* self) {
    u8* p = (u8*)self;
    u32 obj = *(u32*)(p + 0x4C);
    float f = lbl_eu_80667F78;
    if (!func_80137510(obj, f)) return;
    void** vtbl;
    u32 ptr44 = *(u32*)(p + 0x44);
    u32 ptr4C = *(u32*)(p + 0x4C);
    vtbl = *(void***)ptr44;
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr4C, 0);
    u32 ptr50 = *(u32*)(p + 0x50);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr50, 0);
    u32 ptr48 = *(u32*)(p + 0x48);
    ((void(*)(void*, u32, int))vtbl[0x2C / 4])((void*)ptr44, ptr48, 1);
    *(u32*)(p + 0x58) = 5;
}

void func_801CE390(void* self) { }

void func_801CE3E8() { }

// Handle sub-object activation.
void func_801CE4B4(void* self) {
    u8* p = (u8*)self;
    if (!func_80208360(p + 0x418)) return;
    *(u32*)(p + 0x58) = 7;
    u8 temp[16];
    func_80208760(temp, p + 0x418);
    void** vtbl = *(void***)(p + 0xa0);
    ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
    func_801D216C(p + 0xa0, 1);
}

void func_801CE524() { }

// Handle sub-object activation with copy.
void func_801CE974(void* self) {
    u8* p = (u8*)self;
    if (!func_8022D09C(p + 0x440)) return;
    *(u32*)(p + 0x58) = 0xd;
    func_801D216C(p + 0xa0, 1);
    u8 temp[16];
    func_8022D0F8(temp, p + 0x440, p[0x529]);
    void** vtbl = *(void***)(p + 0xa0);
    ((void(*)(void*, void*))vtbl[4])(p + 0xa0, temp);
}

// Check if sub-obj is active; set state to 3 and clear flag.
void func_801CE9E8(void* self) {
    u8* p = (u8*)self;
    if (func_8022D09C(p + 0x440)) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Handle sub-object activation (variant).
void func_801CEA30(void* self) {
    u8* p = (u8*)self;
    if (!func_8022DB74(p + 0x468)) return;
    *(u32*)(p + 0x58) = 0x10;
    u8 temp[16];
    func_8022E498(temp, p + 0x468);
    void** vtbl = *(void***)(p + 0xd0);
    ((void(*)(void*, void*))vtbl[4])(p + 0xd0, temp);
    func_801D216C(p + 0xd0, 1);
}

// Check if sub-obj is active; set state to 3 and clear flag.
void func_801CEAA0(void* self) {
    u8* p = (u8*)self;
    if (func_8022DB74(p + 0x468)) {
        *(u32*)(p + 0x58) = 3;
        p[0x528] = 0;
    }
}

// Check system window; if active, set state and call helpers.
void func_801CEAE8(void* self) {
    u8* p = (u8*)self;
    if (CSysWin_isActive(p + 0x4ac)) {
        *(u32*)(p + 0x58) = 0x16;
        func_801D216C(p + 0xa0, 1);
        func_801D0950(self);
    }
}

// Handle system window state change.
void func_801CEB3C(void* self) {
    u8* p = (u8*)self;
    if (!CSysWin_isActive(p + 0x4ac)) return;
    if ((s8)p[0x540]) {
        *(u32*)(p + 0x58) = 3;
        func_801D216C(p + 0x70, 1);
        func_801D0950(self);
    } else {
        u8 idx = p[0x6f];
        u8 cat = *(u8*)((u8*)self + (s8)idx + 0x62);
        u32 diff = cat - 2;
        if (diff <= 7) {
            func_801EB410(p + 0x3e4, 0);
        } else {
            func_801EB410(p + 0x3e4, 1);
        }
        func_801EB0D4(p + 0x3e4);
        *(u32*)(p + 0x58) = 0x18;
        p[0x528] = 1;
    }
}

// Handle system window activation.
void func_801CEBF0(void* self) {
    u8* p = (u8*)self;
    if (!CSysWin_isActive(p + 0x4e8)) return;
    *(u32*)(p + 0x58) = 3;
    if (!p[0x542] && !p[0x544]) {
        func_801D216C(p + 0x70, 1);
    }
    if (!p[0x544]) {
        func_801D0950(self);
    }
    if (p[0x544]) {
        func_801D216C(p + 0xa0, 1);
    }
}

// Check conditions; set flags.
void func_801CEC80(void* self) {
    if (!*(u32*)((u8*)self + 0x44)) return;
    if (!*(u32*)((u8*)self + 0x40)) return;
    ((u8*)self)[0x60] = 1;
    ((u8*)self)[0x54] = 1;
}

void func_801CECD0() { }

void func_801CF240() { }

void func_801CF71C() { }

void func_801CF900() { }

void func_801CFA58() { }

// Format text and set on layout pane.
void func_801CFCBC(void* self, u32 val) {
    u8* p = (u8*)self;
    char buf[64];
    sprintf(buf, &lbl_eu_8050566C[0x513], val + 1);
    u32 obj = *(u32*)(p + 0x44);
    u32 sub = *(u32*)(obj + 0x10);
    void** vtbl = *(void***)sub;
    void* ret = ((void*(*)(void*, char*, int))vtbl[0x3C / 4])((void*)sub, buf, 1);
    func_80124270(ret, val);
}

void func_801CFD2C() { }

void func_801CFF28() { }

void func_801CFFEC(void* self){}

void func_801D0328(void* self){}

void func_801D05D4() { }

void func_801D0950() { }

void func_801D0BD8() { }

void func_801D0E88() { }

// Handle item event dispatch.
void func_801D11B8(void* self, void* item, int eventType) {
    if (!item) return;
    if (eventType >= 1) {
        u32 w = *(u32*)item;
        func_80158118(item, w >> 20);
    } else {
        void* inst = CItem_initItemImplInstances(item);
        void** vtbl = *(void***)inst;
        ((void(*)(void*, void*))vtbl[4])(inst, item);
    }
}

// Dispatch based on entry category.
u32 func_801D1220(void* self) {
    u8* p = (u8*)self;
    s8 idx = (s8)p[0x6f];
    u8 cat = p[idx + 0x62];
    u32 result;
    switch (cat) {
        case 0:  result = 0x33; break;
        case 1:  result = 0x3C; break;
        case 2:  result = 0x34; break;
        case 3:  result = 0x35; break;
        case 4:  result = 0x36; break;
        case 5:  result = 0x37; break;
        case 6:  result = 0x38; break;
        case 7:  result = 0x3D; break;
        case 8:  result = 0x3E; break;
        case 9:  result = 0x39; break;
        case 10: result = 0x3B; break;
        case 11: result = 0x3A; break;
        default: result = 0; break;
    }
    if (result) {
        // Would call func_80136190 with string constants
        // Skipped due to string constant inaccessibility
    }
    return result;
}

// Check item availability in item storage.
u32 func_801D12D4(void* self, void* entry) {
    return 1;
}


void CopyVec4s(short* dst, const short* src) { dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3]; }

// Split a u32 into its four bytes, stored as shorts (big-endian order).
void func_801D1F9C(short* dst, unsigned long val) {
    dst[0] = (val >> 24) & 0xFF;
    dst[1] = (val >> 16) & 0xFF;
    dst[2] = (val >> 8) & 0xFF;
    dst[3] = val & 0xFF;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CArtsBookItem constructor
void __ct__CArtsBookItem(void* self) {
    u8* vtable;
    // vtable setup
    *(u16*)((u8*)self + 0x804) = 0;
}
// Standard MWCC virtual destructor
void* __dt__10CQuestItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}
void* __dt__11CVisionItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}
void* __dt__13CArtsBookItemFv(void* self, int mode) {
    if (self && mode > 0) __dl__FPv(self);
    return self;
}

void func_801C4BB4() { }
// Add id to list if not already present and capacity check passes.
void func_801C5158(void* self, u32 id) {
    if (func_801C51BC(self, id)) return;
    if ((func_801392E4(id) & 0xFFFF) == 12) return;
    u16 count = *(u16*)((u8*)self + 0x804);
    *(u16*)((u8*)self + 4 + count * 2) = (u16)id;
    *(u16*)((u8*)self + 0x804) = count + 1;
}
void func_801C5254() { }
void func_801C53D8() { }
void OnFileEvent__12CItemBoxGridFP10CEventFile() { }

// Static initialization of color table entries.
void sinit_801D1E30(void) {
    func_801D1F9C((void*)&lbl_eu_80664488, 0);
    func_801D1F9C((void*)&lbl_eu_80664490, 0);
    func_801C4B60((void*)&lbl_eu_80664498, 0x79, 0x49, 0x07, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A0, 0xED, 0xCD, 0x83, 0x00);
    func_801C4B60((void*)&lbl_eu_806644A8, 0x1A, 0x43, 0x53, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B0, 0xC4, 0xE8, 0xEB, 0x00);
    func_801C4B60((void*)&lbl_eu_806644B8, 0x74, 0x54, 0x1D, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C0, 0xD5, 0xB9, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644C8, 0x3D, 0x68, 0x78, 0x00);
    func_801C4B60((void*)&lbl_eu_806644D0, 0xC4, 0xE8, 0xEB, 0x00);
    func_801D1F9C((void*)&lbl_eu_806644D8, 0);
    func_801D1F9C((void*)&lbl_eu_806644E0, 0);
    func_801C4B60((void*)&lbl_eu_806644E8, 0x12, 0xA3, 0xE7, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F0, 0xFF, 0xFF, 0xFF, 0x00);
    func_801C4B60((void*)&lbl_eu_806644F8, 0xB3, 0x09, 0xC0, 0x00);
    func_801C4B60((void*)&lbl_eu_80664500, 0xFF, 0xFF, 0xFF, 0x00);
}

extern u8 lbl_eu_805347F8[];

