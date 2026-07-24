// Auto-scaffolded catalog TU for kyoshin/action/CActParamData
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/action/CActParamData.cpp" line 4 "kyoshin/harness_catalog.hpp" */
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

// LLM-HARNESS-BEGIN: us-80054044
extern "C" void __ct__13CActParamDataFv() {}
// LLM-HARNESS-END: us-80054044

// LLM-HARNESS-BEGIN: us-800540a8
extern "C" void __dt__13CActParamDataFv() {}
// LLM-HARNESS-END: us-800540a8

// LLM-HARNESS-BEGIN: us-800540e8
extern "C" void func_80053A90() {}
// LLM-HARNESS-END: us-800540e8

// LLM-HARNESS-BEGIN: us-8005417c
extern "C" void func_80053B24() {}
// LLM-HARNESS-END: us-8005417c

// LLM-HARNESS-BEGIN: us-80054440
extern "C" void func_80053DE8() {}
// LLM-HARNESS-END: us-80054440

// LLM-HARNESS-BEGIN: us-80054598
extern "C" void func_80053F40() {}
// LLM-HARNESS-END: us-80054598

// LLM-HARNESS-BEGIN: us-800545d4
extern "C" void func_80053F7C() {}
// LLM-HARNESS-END: us-800545d4

// LLM-HARNESS-BEGIN: us-800547d8
extern "C" void func_80054170() {}
// LLM-HARNESS-END: us-800547d8

// LLM-HARNESS-BEGIN: us-80054aa0
extern "C" void func_80054438() {}
// LLM-HARNESS-END: us-80054aa0

// LLM-HARNESS-BEGIN: us-80054c7c
extern "C" void func_80054614() {}
// LLM-HARNESS-END: us-80054c7c

// LLM-HARNESS-BEGIN: us-80054e3c
extern "C" void func_800547D4() {}
// LLM-HARNESS-END: us-80054e3c

// LLM-HARNESS-BEGIN: us-80054fe8
extern "C" void func_80054980() {}
// LLM-HARNESS-END: us-80054fe8

// LLM-HARNESS-BEGIN: us-80055088
extern "C" void func_80054A20() {}
// LLM-HARNESS-END: us-80055088

// LLM-HARNESS-BEGIN: us-8005508c
extern "C" void func_80054A24() {}
// LLM-HARNESS-END: us-8005508c

// LLM-HARNESS-BEGIN: us-800550a4
extern "C" void func_80054A3C() {}
// LLM-HARNESS-END: us-800550a4

// LLM-HARNESS-BEGIN: us-800550fc
extern "C" void func_80054A94() {}
// LLM-HARNESS-END: us-800550fc

extern "C" void func_80054D3C(void* self);
// LLM-HARNESS-BEGIN: us-8005539c
extern "C" void func_80054D34(void* self) { ((void(*)(void*))func_80054D3C)((char*)self + 0x2e0); }
// LLM-HARNESS-END: us-8005539c


// LLM-HARNESS-BEGIN: us-80055740
extern "C" void func_800550D8() {}
// LLM-HARNESS-END: us-80055740

// LLM-HARNESS-BEGIN: us-80055744
extern "C" void func_800550DC() {}
// LLM-HARNESS-END: us-80055744

// LLM-HARNESS-BEGIN: us-80055748
extern "C" void func_800550E0() {}
// LLM-HARNESS-END: us-80055748

// LLM-HARNESS-BEGIN: us-8005574c
extern "C" void func_800550E4() {}
// LLM-HARNESS-END: us-8005574c

// LLM-HARNESS-BEGIN: us-80055750
extern "C" void func_800550E8() {}
// LLM-HARNESS-END: us-80055750

// LLM-HARNESS-BEGIN: us-80055b38
extern "C" void func_800554D0() {}
// LLM-HARNESS-END: us-80055b38

// LLM-HARNESS-BEGIN: us-80055b3c
extern "C" void func_800554D4() {}
// LLM-HARNESS-END: us-80055b3c

// LLM-HARNESS-BEGIN: us-80055b40
extern "C" void func_800554D8() {}
// LLM-HARNESS-END: us-80055b40

// LLM-HARNESS-BEGIN: us-80055b44
extern "C" void func_800554DC() {}
// LLM-HARNESS-END: us-80055b44

// LLM-HARNESS-BEGIN: us-80055c54
extern "C" void func_800555EC() {}
// LLM-HARNESS-END: us-80055c54

// LLM-HARNESS-BEGIN: us-80055d68
extern "C" void func_80055700() {}
// LLM-HARNESS-END: us-80055d68

// LLM-HARNESS-BEGIN: us-80055de4
extern "C" void func_8005577C() {}
// LLM-HARNESS-END: us-80055de4

// LLM-HARNESS-BEGIN: us-80055e50
extern "C" void func_800557E8() {}
// LLM-HARNESS-END: us-80055e50

// LLM-HARNESS-BEGIN: us-80055fc8
extern "C" void func_80055960() {}
// LLM-HARNESS-END: us-80055fc8

// LLM-HARNESS-BEGIN: us-8005612c
extern "C" void func_80055AC4() {}
// LLM-HARNESS-END: us-8005612c

// LLM-HARNESS-BEGIN: us-800561f0
extern "C" void func_80055B88() {}
// LLM-HARNESS-END: us-800561f0

// LLM-HARNESS-BEGIN: us-80056420
extern "C" void func_80055DB8() {}
// LLM-HARNESS-END: us-80056420

// LLM-HARNESS-BEGIN: us-8005643c
extern "C" void func_80055DD4() {}
// LLM-HARNESS-END: us-8005643c

// LLM-HARNESS-BEGIN: us-80056458
extern "C" void func_80055DF0() {}
// LLM-HARNESS-END: us-80056458

// LLM-HARNESS-BEGIN: us-80056508
extern "C" void* func_80055EA0(void* param) {
    void* val = *(void**)((char*)param + 0x2f4);
    if (!val) return 0;
    return (char*)val + 0x14;
}
// LLM-HARNESS-END: us-80056508

// LLM-HARNESS-BEGIN: us-80056524
extern "C" void func_80055EBC() {}
// LLM-HARNESS-END: us-80056524

// LLM-HARNESS-BEGIN: us-8005654c
extern "C" void func_80055EE4() {}
// LLM-HARNESS-END: us-8005654c

// LLM-HARNESS-BEGIN: us-80056570
extern "C" void func_80055F08() {}
// LLM-HARNESS-END: us-80056570

// LLM-HARNESS-BEGIN: us-8005658c
extern "C" int func_80055F24(void* self) {
    char* base = (char*)self + 0x260;
    if (*(int*)((char*)self + 0x2d4) == 0)
        base = (char*)self + 0x2e0;
    void* ptr = *(void**)(base + 0x74);
    if (ptr == NULL)
        return -1;
    return *(short*)((char*)ptr + 8);
}
// LLM-HARNESS-END: us-8005658c

// LLM-HARNESS-BEGIN: us-800565bc
extern "C" s16 func_80055F54(void* self) {
    u8* base = (u8*)self + 0x260;
    if (*(s32*)((u8*)self + 0x2d4) == 0)
        base = (u8*)self + 0x2e0;
    void* ptr = *(void**)(base + 0x74);
    if (!ptr) return -1;
    return *(s16*)((u8*)ptr + 0xa);
}
// LLM-HARNESS-END: us-800565bc

// LLM-HARNESS-BEGIN: us-800565ec
extern "C" void func_80055F84() {}
// LLM-HARNESS-END: us-800565ec

// LLM-HARNESS-BEGIN: us-800565fc
extern "C" void func_80055F94() {}
// LLM-HARNESS-END: us-800565fc

// LLM-HARNESS-BEGIN: us-800566c4
extern "C" void* func_8005605C(void* param) {
    if (*(int*)((int)param + 0x274) != 0) {
        void* ptr = *(void**)((int)param + 0x2d8);
        if (ptr != 0) return ptr;
    }
    if (*(int*)((int)param + 0x2f4) != 0) {
        void* result = *(void**)((int)param + 0x358);
        if (result != 0) return result;
    }
    return 0;
}
// LLM-HARNESS-END: us-800566c4

// LLM-HARNESS-BEGIN: us-80056704
extern "C" bool func_8005609C() { return false; }
// LLM-HARNESS-END: us-80056704

// LLM-HARNESS-BEGIN: us-8005670c
extern "C" void func_800560A4() {}
// LLM-HARNESS-END: us-8005670c

// LLM-HARNESS-BEGIN: us-8005671c
extern "C" void func_800560B4() {}
// LLM-HARNESS-END: us-8005671c

// LLM-HARNESS-BEGIN: us-8005672c
extern "C" void func_800560C4() {}
// LLM-HARNESS-END: us-8005672c

// LLM-HARNESS-BEGIN: us-8005673c
extern "C" void func_800560D4() {}
// LLM-HARNESS-END: us-8005673c

// LLM-HARNESS-BEGIN: us-8005674c
extern "C" void func_800560E4() {}
// LLM-HARNESS-END: us-8005674c

// LLM-HARNESS-BEGIN: us-8005675c
extern "C" void func_800560F4() {}
// LLM-HARNESS-END: us-8005675c

// LLM-HARNESS-BEGIN: us-8005676c
extern "C" void func_80056104() {}
// LLM-HARNESS-END: us-8005676c

// LLM-HARNESS-BEGIN: us-80056780
extern "C" void func_80056118() {}
// LLM-HARNESS-END: us-80056780

// LLM-HARNESS-BEGIN: us-80056790
extern "C" void func_80056128() {}
// LLM-HARNESS-END: us-80056790

// LLM-HARNESS-BEGIN: us-800567a0
extern "C" void func_80056138() {}
// LLM-HARNESS-END: us-800567a0

// LLM-HARNESS-BEGIN: us-800567b0
extern "C" void func_80056148() {}
// LLM-HARNESS-END: us-800567b0

// LLM-HARNESS-BEGIN: us-800567c0
extern "C" void func_80056158() {}
// LLM-HARNESS-END: us-800567c0

// LLM-HARNESS-BEGIN: us-800567d0
extern "C" void func_80056168() {}
// LLM-HARNESS-END: us-800567d0

// LLM-HARNESS-BEGIN: us-800567e0
extern "C" void func_80056178() {}
// LLM-HARNESS-END: us-800567e0

// LLM-HARNESS-BEGIN: us-800567f0
extern "C" void func_80056188() {}
// LLM-HARNESS-END: us-800567f0

// LLM-HARNESS-BEGIN: us-80056800
extern "C" void func_80056198() {}
// LLM-HARNESS-END: us-80056800

// LLM-HARNESS-BEGIN: us-80056810
extern "C" void func_800561A8() {}
// LLM-HARNESS-END: us-80056810

// LLM-HARNESS-BEGIN: us-80056820
extern "C" void func_800561B8() {}
// LLM-HARNESS-END: us-80056820

// LLM-HARNESS-BEGIN: us-80056830
extern "C" void func_800561C8() {}
// LLM-HARNESS-END: us-80056830

// LLM-HARNESS-BEGIN: us-80056840
extern "C" void func_800561D8() {}
// LLM-HARNESS-END: us-80056840

// LLM-HARNESS-BEGIN: us-80056850
extern "C" void func_800561E8() {}
// LLM-HARNESS-END: us-80056850

// LLM-HARNESS-BEGIN: us-80056860
extern "C" void func_800561F8() {}
// LLM-HARNESS-END: us-80056860

// LLM-HARNESS-BEGIN: us-80056870
extern "C" void func_80056208() {}
// LLM-HARNESS-END: us-80056870

// LLM-HARNESS-BEGIN: us-80056880
extern "C" void func_80056218() {}
// LLM-HARNESS-END: us-80056880

// LLM-HARNESS-BEGIN: us-80056890
extern "C" void func_80056228() {}
// LLM-HARNESS-END: us-80056890

// LLM-HARNESS-BEGIN: us-800568a0
extern "C" void func_80056238() {}
// LLM-HARNESS-END: us-800568a0

// LLM-HARNESS-BEGIN: us-800568b0
extern "C" void func_80056248() {}
// LLM-HARNESS-END: us-800568b0

// LLM-HARNESS-BEGIN: us-800568c0
extern "C" void func_80056258() {}
// LLM-HARNESS-END: us-800568c0

// LLM-HARNESS-BEGIN: us-800568d0
extern "C" void func_80056268() {}
// LLM-HARNESS-END: us-800568d0

// LLM-HARNESS-BEGIN: us-800568e4
extern "C" void func_8005627C() {}
// LLM-HARNESS-END: us-800568e4

// Fake SI iface for vt+0x7c tail-call (-RTTI on: omit _v000/_v004).
struct CActParamDataVTableIf {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual int vf7C(int a, int b);
};

// LLM-HARNESS-BEGIN: us-800568f8
extern "C" int func_80056290(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 3);
}
// LLM-HARNESS-END: us-800568f8

// LLM-HARNESS-BEGIN: us-8005690c
extern "C" int func_800562A4(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 4);
}
// LLM-HARNESS-END: us-8005690c

// LLM-HARNESS-BEGIN: us-80056920
extern "C" int func_800562B8(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 5);
}
// LLM-HARNESS-END: us-80056920

// LLM-HARNESS-BEGIN: us-80056934
extern "C" int func_800562CC(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 6);
}
// LLM-HARNESS-END: us-80056934

// LLM-HARNESS-BEGIN: us-80056948
extern "C" int func_800562E0(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 7);
}
// LLM-HARNESS-END: us-80056948

// LLM-HARNESS-BEGIN: us-8005695c
extern "C" int func_800562F4(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 8);
}
// LLM-HARNESS-END: us-8005695c

// LLM-HARNESS-BEGIN: us-80056970
extern "C" int func_80056308(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 9);
}
// LLM-HARNESS-END: us-80056970

// LLM-HARNESS-BEGIN: us-80056984
extern "C" int func_8005631C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 10);
}
// LLM-HARNESS-END: us-80056984

// LLM-HARNESS-BEGIN: us-80056998
extern "C" int func_80056330(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 11);
}
// LLM-HARNESS-END: us-80056998

// LLM-HARNESS-BEGIN: us-800569ac
extern "C" int func_80056344(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 12);
}
// LLM-HARNESS-END: us-800569ac

// LLM-HARNESS-BEGIN: us-800569c0
extern "C" int func_80056358(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 13);
}
// LLM-HARNESS-END: us-800569c0

// LLM-HARNESS-BEGIN: us-800569d4
extern "C" int func_8005636C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 14);
}
// LLM-HARNESS-END: us-800569d4

// LLM-HARNESS-BEGIN: us-800569e8
extern "C" int func_80056380(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 15);
}
// LLM-HARNESS-END: us-800569e8

// LLM-HARNESS-BEGIN: us-800569fc
extern "C" int func_80056394(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 16);
}
// LLM-HARNESS-END: us-800569fc

// LLM-HARNESS-BEGIN: us-80056a10
extern "C" int func_800563A8(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 17);
}
// LLM-HARNESS-END: us-80056a10

// LLM-HARNESS-BEGIN: us-80056a24
extern "C" int func_800563BC(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 18);
}
// LLM-HARNESS-END: us-80056a24

// LLM-HARNESS-BEGIN: us-80056a38
extern "C" int func_800563D0(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 19);
}
// LLM-HARNESS-END: us-80056a38

// LLM-HARNESS-BEGIN: us-80056a4c
extern "C" int func_800563E4(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 20);
}
// LLM-HARNESS-END: us-80056a4c

// LLM-HARNESS-BEGIN: us-80056a60
extern "C" int func_800563F8(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 21);
}
// LLM-HARNESS-END: us-80056a60

// LLM-HARNESS-BEGIN: us-80056a74
extern "C" int func_8005640C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 22);
}
// LLM-HARNESS-END: us-80056a74

// LLM-HARNESS-BEGIN: us-80056a88
extern "C" int func_80056420(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 23);
}
// LLM-HARNESS-END: us-80056a88

// LLM-HARNESS-BEGIN: us-80056a9c
extern "C" int func_80056434(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 24);
}
// LLM-HARNESS-END: us-80056a9c

// LLM-HARNESS-BEGIN: us-80056ab0
extern "C" int func_80056448(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 25);
}
// LLM-HARNESS-END: us-80056ab0

// LLM-HARNESS-BEGIN: us-80056ac4
extern "C" int func_8005645C(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 26);
}
// LLM-HARNESS-END: us-80056ac4

// LLM-HARNESS-BEGIN: us-80056ad8
extern "C" int func_80056470(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 27);
}
// LLM-HARNESS-END: us-80056ad8

// LLM-HARNESS-BEGIN: us-80056aec
extern "C" int func_80056484(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 28);
}
// LLM-HARNESS-END: us-80056aec

// LLM-HARNESS-BEGIN: us-80056b00
extern "C" int func_80056498(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 29);
}
// LLM-HARNESS-END: us-80056b00

// LLM-HARNESS-BEGIN: us-80056b14
extern "C" int func_800564AC(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 30);
}
// LLM-HARNESS-END: us-80056b14

// LLM-HARNESS-BEGIN: us-80056b28
extern "C" int func_800564C0(void* self, int a) {
    return reinterpret_cast<CActParamDataVTableIf*>(self)->vf7C(a, 31);
}
// LLM-HARNESS-END: us-80056b28

// LLM-HARNESS-BEGIN: us-80056b3c
extern "C" void func_800564D4() {}
// LLM-HARNESS-END: us-80056b3c

// LLM-HARNESS-BEGIN: us-80056b50
extern "C" void func_800564E8() {}
// LLM-HARNESS-END: us-80056b50

// LLM-HARNESS-BEGIN: us-80056b64
extern "C" void func_800564FC() {}
// LLM-HARNESS-END: us-80056b64

// LLM-HARNESS-BEGIN: us-80056b78
extern "C" void func_80056510() {}
// LLM-HARNESS-END: us-80056b78

// LLM-HARNESS-BEGIN: us-80056b8c
extern "C" void func_80056524() {}
// LLM-HARNESS-END: us-80056b8c

// LLM-HARNESS-BEGIN: us-80056ba0
extern "C" void func_80056538() {}
// LLM-HARNESS-END: us-80056ba0

// LLM-HARNESS-BEGIN: us-80056bb4
extern "C" void func_8005654C() {}
// LLM-HARNESS-END: us-80056bb4

// LLM-HARNESS-BEGIN: us-80056bc8
extern "C" void func_80056560() {}
// LLM-HARNESS-END: us-80056bc8

// LLM-HARNESS-BEGIN: us-80056bdc
extern "C" void func_80056574() {}
// LLM-HARNESS-END: us-80056bdc

// LLM-HARNESS-BEGIN: us-80056bf0
extern "C" void func_80056588() {}
// LLM-HARNESS-END: us-80056bf0

// LLM-HARNESS-BEGIN: us-80056c04
extern "C" void func_8005659C() {}
// LLM-HARNESS-END: us-80056c04

// LLM-HARNESS-BEGIN: us-80056c18
extern "C" void func_800565B0() {}
// LLM-HARNESS-END: us-80056c18

// LLM-HARNESS-BEGIN: us-80056c2c
extern "C" void func_800565C4() {}
// LLM-HARNESS-END: us-80056c2c

// LLM-HARNESS-BEGIN: us-80056c40
extern "C" void func_800565D8() {}
// LLM-HARNESS-END: us-80056c40

// LLM-HARNESS-BEGIN: us-80056c54
extern "C" void func_800565EC() {}
// LLM-HARNESS-END: us-80056c54

// LLM-HARNESS-BEGIN: us-80056c68
extern "C" void func_80056600() {}
// LLM-HARNESS-END: us-80056c68

// LLM-HARNESS-BEGIN: us-80056c7c
extern "C" void func_80056614() {}
// LLM-HARNESS-END: us-80056c7c

// LLM-HARNESS-BEGIN: us-80056c8c
extern "C" void func_80056624() {}
// LLM-HARNESS-END: us-80056c8c

// LLM-HARNESS-BEGIN: us-80056ca0
extern "C" void func_80056638() {}
// LLM-HARNESS-END: us-80056ca0

// LLM-HARNESS-BEGIN: us-80056cb4
extern "C" void func_8005664C() {}
// LLM-HARNESS-END: us-80056cb4

// LLM-HARNESS-BEGIN: us-80056cc8
extern "C" void func_80056660() {}
// LLM-HARNESS-END: us-80056cc8

// LLM-HARNESS-BEGIN: us-80056cd8
extern "C" void func_80056670() {}
// LLM-HARNESS-END: us-80056cd8

// LLM-HARNESS-BEGIN: us-80056ce8
extern "C" void func_80056680() {}
// LLM-HARNESS-END: us-80056ce8

// LLM-HARNESS-BEGIN: us-80056cfc
extern "C" void func_80056694() {}
// LLM-HARNESS-END: us-80056cfc

// LLM-HARNESS-BEGIN: us-80056d10
extern "C" void func_800566A8() {}
// LLM-HARNESS-END: us-80056d10

// LLM-HARNESS-BEGIN: us-80056d24
extern "C" void func_800566BC() {}
// LLM-HARNESS-END: us-80056d24

// LLM-HARNESS-BEGIN: us-80056d38
extern "C" void func_800566D0() {}
// LLM-HARNESS-END: us-80056d38

// LLM-HARNESS-BEGIN: us-80056d4c
extern "C" void func_800566E4() {}
// LLM-HARNESS-END: us-80056d4c

// LLM-HARNESS-BEGIN: us-80056d60
extern "C" void func_800566F8() {}
// LLM-HARNESS-END: us-80056d60

// LLM-HARNESS-BEGIN: us-80056d70
extern "C" void func_80056708() {}
// LLM-HARNESS-END: us-80056d70

// LLM-HARNESS-BEGIN: us-80056d84
extern "C" void func_8005671C() {}
// LLM-HARNESS-END: us-80056d84

// LLM-HARNESS-BEGIN: us-80056d98
extern "C" void func_80056730() {}
// LLM-HARNESS-END: us-80056d98

// LLM-HARNESS-BEGIN: us-80056dc8
extern "C" void func_80056760() {}
// LLM-HARNESS-END: us-80056dc8

// LLM-HARNESS-BEGIN: us-80056df8
extern "C" void func_80056790() {}
// LLM-HARNESS-END: us-80056df8

// LLM-HARNESS-BEGIN: us-80056e08
extern "C" void func_800567A0() {}
// LLM-HARNESS-END: us-80056e08

// LLM-HARNESS-BEGIN: us-80056e18
extern "C" void func_800567B0() {}
// LLM-HARNESS-END: us-80056e18

// LLM-HARNESS-BEGIN: us-80056e28
extern "C" void func_800567C0() {}
// LLM-HARNESS-END: us-80056e28

// LLM-HARNESS-BEGIN: us-80056e38
extern "C" void func_800567D0() {}
// LLM-HARNESS-END: us-80056e38

// LLM-HARNESS-BEGIN: us-80056e48
extern "C" void func_800567E0() {}
// LLM-HARNESS-END: us-80056e48

// LLM-HARNESS-BEGIN: us-80056e58
extern "C" bool func_800567F0() { return false; }
// LLM-HARNESS-END: us-80056e58

// LLM-HARNESS-BEGIN: us-80056e60
extern "C" bool func_800567F8() { return false; }
// LLM-HARNESS-END: us-80056e60

// LLM-HARNESS-BEGIN: us-80056e68
extern "C" bool func_80056800() { return false; }
// LLM-HARNESS-END: us-80056e68

// LLM-HARNESS-BEGIN: us-80056e70
extern "C" void func_80056808() {}
// LLM-HARNESS-END: us-80056e70

// LLM-HARNESS-BEGIN: us-80056e90
extern "C" void func_80056828() {}
// LLM-HARNESS-END: us-80056e90

// LLM-HARNESS-BEGIN: us-80056ef0
extern "C" void func_80056888() {}
// LLM-HARNESS-END: us-80056ef0

// LLM-HARNESS-BEGIN: us-80056f10
extern "C" void func_800568A8() {}
// LLM-HARNESS-END: us-80056f10

// LLM-HARNESS-BEGIN: us-80056f30
extern "C" void func_800568C8() {}
// LLM-HARNESS-END: us-80056f30

// LLM-HARNESS-BEGIN: us-80056f50
extern "C" void func_800568E8() {}
// LLM-HARNESS-END: us-80056f50

// LLM-HARNESS-BEGIN: us-80057100
extern "C" void func_80056A98() {}
// LLM-HARNESS-END: us-80057100

// LLM-HARNESS-BEGIN: us-8005729c
extern "C" void func_80056C34() {}
// LLM-HARNESS-END: us-8005729c

// LLM-HARNESS-BEGIN: us-800572bc
extern "C" void func_80056C54() {}
// LLM-HARNESS-END: us-800572bc

// LLM-HARNESS-BEGIN: us-800572e4
extern "C" void func_80056C7C() {}
// LLM-HARNESS-END: us-800572e4

// LLM-HARNESS-BEGIN: us-80057330
extern "C" void func_80056CC8() {}
// LLM-HARNESS-END: us-80057330

// LLM-HARNESS-BEGIN: us-8005734c
extern "C" void func_80056CE4() {}
// LLM-HARNESS-END: us-8005734c

// LLM-HARNESS-BEGIN: us-80057368
extern "C" void func_80056D00() {}
// LLM-HARNESS-END: us-80057368

// LLM-HARNESS-BEGIN: us-80057514
extern "C" void func_80056EAC() {}
// LLM-HARNESS-END: us-80057514

// LLM-HARNESS-BEGIN: us-80057530
extern "C" void func_80056EC8() {}
// LLM-HARNESS-END: us-80057530

// LLM-HARNESS-BEGIN: us-800576ec
extern "C" void func_80057084() {}
// LLM-HARNESS-END: us-800576ec

// LLM-HARNESS-BEGIN: us-800578ac
extern "C" void func_80057244() {}
// LLM-HARNESS-END: us-800578ac

// LLM-HARNESS-BEGIN: us-800578cc
extern "C" void func_80057264() {}
// LLM-HARNESS-END: us-800578cc

// LLM-HARNESS-BEGIN: us-800578e8
extern "C" void func_80057280() {}
// LLM-HARNESS-END: us-800578e8

// LLM-HARNESS-BEGIN: us-80057a94
extern "C" void func_8005742C() {}
// LLM-HARNESS-END: us-80057a94

// LLM-HARNESS-BEGIN: us-80057ad8
extern "C" void func_80057470() {}
// LLM-HARNESS-END: us-80057ad8

// LLM-HARNESS-BEGIN: us-80057af8
extern "C" void func_80057490() {}
// LLM-HARNESS-END: us-80057af8

// LLM-HARNESS-BEGIN: us-80057cbc
extern "C" void func_80057654() {}
// LLM-HARNESS-END: us-80057cbc

// LLM-HARNESS-BEGIN: us-80057cd8
extern "C" void func_80057670() {}
// LLM-HARNESS-END: us-80057cd8

// LLM-HARNESS-BEGIN: us-80057e90
extern "C" void func_80057828() {}
// LLM-HARNESS-END: us-80057e90

// LLM-HARNESS-BEGIN: us-80057f04
extern "C" void func_8005789C() {}
// LLM-HARNESS-END: us-80057f04

// LLM-HARNESS-BEGIN: us-800580cc
extern "C" void func_80057A64() {}
// LLM-HARNESS-END: us-800580cc

// LLM-HARNESS-BEGIN: us-80058208
extern "C" void func_80057BA0() {}
// LLM-HARNESS-END: us-80058208

// LLM-HARNESS-BEGIN: us-80058344
extern "C" void func_80057CDC__13CActParamDataFv() {}
// LLM-HARNESS-END: us-80058344

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// LLM-HARNESS-BEGIN: us-800589b4
namespace ml {
template <int N> class FixStr { public: void format(const char*, ...); };
template <> void FixStr<16>::format(const char*, ...) {}
}
// LLM-HARNESS-END: us-800589b4
