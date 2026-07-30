// Auto-scaffolded catalog TU for kyoshin/action/CActParamAnim
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/action/CActParamAnim.cpp" line 3 "kyoshin/harness_catalog.hpp" */
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

/* "src/kyoshin/action/CActParamAnim.cpp" line 5 "kyoshin/action/CActParamAnim.hpp" */
#pragma once

/* "src/kyoshin/action/CActParamAnim.hpp" line 2 "types.h" */
/* end "types.h" */

// Full object layout for C-linkage accessors
struct CActParamAnimFull {
    u8 _00[0x10];
    u8 mChildData[0x4AE]; // from +0x10 to +0x4BE
    u8 mEffByte0;          // +0x4BE
    u8 mEffByte1;          // +0x4BF
    u8 _4C0[4];            // +0x4C0: eff obj pointer
};

class CActParamAnim {
public:
    CActParamAnim();
    virtual ~CActParamAnim();
    void func_8004B114();
    void func_8004D074();
    void func_8004D194();
    void func_8004D2F8();
    void func_8004D4AC();
    void func_8004D650();
    void func_8004D7EC();
    void func_8004D950();
    void func_8004DAE0();
    void func_8004DDD0();
    void func_8004DF9C();
    void func_8004E168();
    void func_8004E334();
    void func_8004E500();
    void func_8004E694();
    void func_8004E828();
    void func_8004E9EC();
    void func_8004ECF4() const;
    void func_8004F1E4();
    void func_8004F334();
    void func_8004F484();
    void func_8004F5FC();
    void func_8004F884();
    void func_8004FAB4();
    void func_8004FCE0();
    void func_8004FE58();
    void func_8004FFBC();
    void func_80050744();
    void func_80050890();
    void func_80050C50();
    void func_80050DB0();
    void func_80050F5C();
    void func_800510FC();
    void func_800512A8();
    void func_80051448();
    void func_80051584();
    void func_800516C0();
    void func_800517FC();
    void func_8005194C();
    void func_80052934();
    void func_80053164();

    // Converted free functions → members
    void* getChild();
    float getBlendWeight();
    int checkFlag(int mask);
    void* getOwner();
    float getScale();
    void updateSubAnim();
    void setOwner(int val);
    float getAnimSpeed() const;
    void copyVec3To3C0(const float* src);
    void setVec3Y(float val);
    int checkAnimFlag();
    float func_8004B854();
    float func_8004B86C();
    void callObjUpdate();
    float func_8004B898();
    int callObjFunc(void* param);
    void* getNextChainObj();
    void setBlendFlag(int param);
    void* getModelObj();
    int checkRenderFlag();
    void setAnimCounter(int value);
    void* getEffObj();
    void clearEffObj();
    void func_8004C5E8();
    void resetVec3Y();
    void setActiveFlag(s32 param);
    void startAnimA();
    void startAnimA2();
    void startAnimB();
    void startAnimC();
    void stopAnim();
    u32 checkFlag13();
    float getParamFloat();
    float getField10();
    u32 checkFlag16();
    u32 testAndClearFlag18();
    void mulVec3Y(float param_2);
    int getChainInt();
    bool func_80053990();
    bool func_80053998();
    bool func_800539A0();
    bool func_800539A8();
    bool func_800539B0();
    bool func_800539B8();
    bool func_800539C0();
    bool func_800539C8();
    bool func_800539D0();
    bool func_800539D8();
};

/* end "kyoshin/action/CActParamAnim.hpp" */
CActParamAnim::CActParamAnim() {}

void __dt__8004B070(){}

void CActParamAnim_initSub1() {}

void CActParamAnim_initSub2() {}

CActParamAnim::~CActParamAnim() {}

void CActParamAnim::func_8004B114() {}

void* CActParamAnim::getChild() { return &reinterpret_cast<CActParamAnimFull*>(this)->mChildData; }

float CActParamAnim::getBlendWeight() {
    return *(float*)((char*)this + 0x43c);
}

void func_8004B354(){}

int CActParamAnim::checkFlag(int mask) {
    return (*(int*)((char*)this) & mask) != 0 ? 1 : 0;
}

void CActParamAnim_copyTranslation(void* dst, const void* src) {
    *(int*)((char*)dst + 0) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 4) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 8) = *(int*)((char*)src + 8);
}

void func_8004B40C(){}

void func_8004B4A4(){}

void* CActParamAnim::getOwner() {
    return *(void**)((char*)this + 8);
}

bool CActParamAnim_isEnabled() { return true; }

void func_8004B52C(){}

void CActParamAnim_copyRotation(void* dst, const void* src) {
    *(int*)((char*)dst + 0xC) = *(int*)((char*)src + 0);
    *(int*)((char*)dst + 0x10) = *(int*)((char*)src + 4);
    *(int*)((char*)dst + 0x14) = *(int*)((char*)src + 8);
}

void vec3_set(float *data, float a, float b, float c) { data[0] = a; data[1] = b; data[2] = c; }

float CActParamAnim::getScale() {
    return *(float*)((char*)this + 0x444);
}

void func_8004B624(){}

void bits_clear(uint32_t* ptr, uint32_t mask) {
    *ptr &= ~mask;
}

void CActParamAnim::updateSubAnim() {
    extern void func_80055AC4(void*);
    if (*(int*)((char*)this + 0x3A4) != 0) func_80055AC4((char*)this + 16);
}

void func_8004B6BC(){}

void CActParamAnim::setOwner(int val) { *(int*)((char*)this + 8) = val; }

// STALLED: uses paired-single (PS) SIMD instructions (psq_l/ps_sum1/psq_st)
// that MWCC Wii 1.1 cannot generate from standard C. Requires DECOMP_ASM_INSN
// carve-outs per PLAN.md 17.6, but the entire function body is PS ops, making
// it effectively a whole-function asm body which is not permitted.
void func_8004B738(){}

void func_8004B75C(){}

void vec3_copy(float *dst, const float *src) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

float CActParamAnim::getAnimSpeed() const {
    return *(const float*)((const char*)this + 0x390);
}

void CActParamAnim::copyVec3To3C0(const float* src) {
    *(int*)((char*)this + 0x3C0) = *(int*)((char*)src + 0);
    *(int*)((char*)this + 0x3C4) = *(int*)((char*)src + 4);
    *(int*)((char*)this + 0x3C8) = *(int*)((char*)src + 8);
}

void func_8004B7DC(){}

void CActParamAnim::setVec3Y(float val) {
    *(float*)((char*)this + 0x3c4) = val;
}

int CActParamAnim::checkAnimFlag() {
    return (*(unsigned int *)((char *)this + 12) >> 1) & 1;
}

extern "C" u32 func_8004B848(const CActParamAnim* self) {
    return (*(const u32*)((const u8*)self + 0xC) >> 1) & 1;
}

float CActParamAnim::func_8004B854() {
    extern float func_80484F18(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj) return func_80484F18(obj);
    return lbl_eu_80665EA0;
}

float CActParamAnim::func_8004B86C() {
    extern float func_804850A4(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj != 0) return func_804850A4(obj);
    return lbl_eu_80665EA0;
}

void CActParamAnim::callObjUpdate() {
    extern void func_80484F80(void*);
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj != 0) func_80484F80(obj);
}

float CActParamAnim::func_8004B898() {
    extern float func_80484EB0(void*);
    extern float lbl_eu_80665EA0;
    void* obj = *(void**)((char*)this + 0x3A0);
    if (obj != 0) return func_80484EB0(obj);
    return lbl_eu_80665EA0;
}

void func_8004B8B0(){}

void func_8004B8F8(){}

extern int func_80485464(void*, void*);
extern int func_80485244(void*);
int CActParamAnim::callObjFunc(void* param) {
    void* val = *(void**)((char*)this + 0x3A0);
    if (!val) return 0;
    if (!param) {
        return func_80485244(val);
    }
    return func_80485464(val, param);
}

void* CActParamAnim::getNextChainObj() {
    void* v = *(void**)((char*)this + 0x8);
    if (v) {
        return *(void**)((char*)v + 0x18);
    }
    return *(void**)((char*)this + 0x4b4);
}

void func_8004B9D4(){}

void func_8004BC28(){}

void CActParamAnim::setBlendFlag(int param) {
    if (param != 0) {
        int tmp = *(int*)((char*)this + 0xc);
        *(unsigned char*)((char*)this + 0x4da) = 30;
        *(int*)((char*)this + 0xc) = tmp | 0x4000000;
    } else {
        *(int*)((char*)this + 0xc) &= ~0x4000000;
    }
}

void func_8004BC94(){}

void func_8004BDCC(){}

void CActParamAnim::func_8004C5E8() {}

void* CActParamAnim::getModelObj() {
    void* result = *(void**)((char*)this + 0x2fc);
    if (result) return result;
    return *(void**)((char*)this + 0x27c);
}

void func_8004C608(){}

void func_8004CB80(){}

void func_8004CBC8(){}

float Atan2FIdx__Q24nw4r4mathFff(float, float);
float CActParamAnim_atan2Scaled(float y, float x) {
    extern float lbl_eu_80665ED0;
    return lbl_eu_80665ED0 * Atan2FIdx__Q24nw4r4mathFff(y, x);
}

void func_8004CC68(){}

void func_8004CC74(){}

int CActParamAnim::checkRenderFlag() { return ((*(unsigned int*)((unsigned char*)this + 0x260) >> 5) & 1); }

void func_8004CC8C(){}

void CActParamAnim::setAnimCounter(int value) {
    *(int*)((char*)this + 0x374) = value;
}

void func_8004CF00(){}

void CActParamAnim::func_8004D074() {}

void CActParamAnim::func_8004D194() {}

void CActParamAnim::func_8004D2F8() {}

void CActParamAnim::func_8004D4AC() {}

void CActParamAnim::func_8004D650() {}

void CActParamAnim::func_8004D7EC() {}

void CActParamAnim::func_8004D950() {}

void* CActParamAnim::getEffObj() {
    return *(void**)((char*)this + 0x4c0);
}

void CActParamAnim::clearEffObj() {
    CActParamAnimFull* full = reinterpret_cast<CActParamAnimFull*>(this);
    full->mEffByte0 = 0;
    full->mEffByte1 = 0;
    *(int *)((char *)this + 0x4c0) = 0;
}

void CActParamAnim::func_8004DAE0() {}

void CActParamAnim::func_8004DDD0() {}

void CActParamAnim::func_8004DF9C() {}

void CActParamAnim::func_8004E168() {}

void CActParamAnim::func_8004E334() {}

void CActParamAnim::func_8004E500() {}

void CActParamAnim::func_8004E694() {}

void CActParamAnim::func_8004E828() {}

void CActParamAnim::func_8004E9EC() {}

void func_8004EC78(){}

void CActParamAnim::func_8004ECF4() const {}

void CActParamAnim::func_8004F1E4() {}

void CActParamAnim::func_8004F334() {}

void CActParamAnim::func_8004F484() {}

void CActParamAnim::func_8004F5FC() {}

void CActParamAnim::func_8004F884() {}

void CActParamAnim::func_8004FAB4() {}

void CActParamAnim::func_8004FCE0() {}

void CActParamAnim::func_8004FE58() {}

void CActParamAnim::func_8004FFBC() {}

f32 func_800504BC(const f32* a, const f32* b){
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void func_800504DC(){}

void CActParamAnim::func_80050744() {}

void CActParamAnim::func_80050890() {}

void CActParamAnim::func_80050C50() {}

void CActParamAnim::func_80050DB0() {}

void CActParamAnim::func_80050F5C() {}

void CActParamAnim::func_800510FC() {}

void CActParamAnim::func_800512A8() {}

void CActParamAnim::func_80051448() {}

void CActParamAnim::func_80051584() {}

void CActParamAnim::func_800516C0() {}

void CActParamAnim::func_800517FC() {}

void CActParamAnim::func_8005194C() {}

extern float lbl_eu_80665EA0;
void CActParamAnim::resetVec3Y() { *(float*)((u8*)this + 964) = lbl_eu_80665EA0; }

void CActParamAnim::setActiveFlag(s32 param) {
    if (param != 0) {
        *(u32*)((u8*)this + 0xC) |= 2;
    } else {
        *(u32*)((u8*)this + 0xC) &= ~2;
    }
}

void func_80051AD0(){}

void func_80051B38(){}

void CActParamAnim::startAnimA() {
    *(unsigned int *)((char *)this + 0x4a8) = 0x44a05;
    *(unsigned int *)((char *)this + 0xc) |= 0x800000;
}

void CActParamAnim::startAnimA2() {
    extern float lbl_eu_80665F18;
    u32 v = *(u32*)((u8*)this + 12);
    float f = lbl_eu_80665F18;
    *(volatile u32*)((u8*)this + 0x4A8) = 0x44A05;
    *(volatile float*)((u8*)this + 0x484) = f;
    *(u32*)((u8*)this + 12) = v | 0x00800000;
}

void CActParamAnim::startAnimB() {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)this + 0x4A8) = 0x44A09;
    *(volatile float*)((char*)this + 0x484) = tmp;
}

void CActParamAnim::startAnimC() {
    extern float lbl_eu_80665F18;
    float tmp = lbl_eu_80665F18;
    *(volatile unsigned int*)((char*)this + 0x4A8) = 0x44A11;
    *(volatile float*)((char*)this + 0x484) = tmp;
}

void func_80051BF4(){}

void func_80051C40(){}

void CActParamAnim::stopAnim() {
    u32 val = *(u32*)((u8*)this + 12);
    u32 tmp = val & ~0x10u;
    *(u32*)((u8*)this + 12) = tmp;
    if ((val & 2u) == 0) return;
    tmp &= ~0x80u;
    *(u8*)((u8*)this + 1176) = 0;
    *(u32*)((u8*)this + 12) = tmp;
}

void func_80051CD4(){}

u32 CActParamAnim::checkFlag13() { return (*(u32*)((u8*)this + 608) >> 13) & 0x1u; }

float CActParamAnim::getParamFloat() { return *(float*)((char*)this + 0x4A0); }

float CActParamAnim::getField10() { return *(float *)((char *)this + 0x10); }

u32 CActParamAnim::checkFlag16() {
    return (*(u32*)((u8*)this + 0x260) >> 0x10) & 1;
}

u32 CActParamAnim::testAndClearFlag18() {
    u32 result = (*(u32*)((u8*)this + 0x260) >> 18) & 1u;
    *(volatile u32*)((u8*)this + 0x260) &= ~(1u << 18);
    return result;
}

void func_80052584(){}

void func_800526C0(){}

void func_8005274C(){}

void func_80052780(void* self, void* src){
    *(int*)((char*)self + 168) = *(int*)((char*)src + 0);
    *(int*)((char*)self + 172) = *(int*)((char*)src + 4);
    *(int*)((char*)self + 176) = *(int*)((char*)src + 8);
    *(int*)((char*)self + 180) = *(int*)((char*)src + 12);
    *(int*)((char*)self + 224) |= 4;
}

void func_800527B0(){}

void func_800527E8(){}

void CActParamAnim::mulVec3Y(float param_2) {
    *(float*)((char*)this + 0x3c4) *= param_2;
}

void CActParamAnim::func_80052934() {}

void CActParamAnim::func_80053164() {}

void func_80053198(){}

void func_80053490(){}

extern "C" {
extern float lbl_eu_80665ECC;
}
extern "C" float CActParamAnim_getAnimConstant() { return lbl_eu_80665ECC; }

int CActParamAnim::getChainInt() {
    int val = *(int*)((char*)this + 1220);
    if (val) return val;
    void* ptr = *(void**)((char*)this + 8);
    if (ptr) return *(int*)((char*)ptr + 24);
    return *(int*)((char*)this + 1204);
}

extern u8 lbl_eu_80663D4C;
extern "C" void func_eu_80053FC8() { lbl_eu_80663D4C = 0; }

extern "C" u8 lbl_eu_80663D4C;
extern "C" u8 func_eu_80053FD4() { return lbl_eu_80663D4C; }

extern u8 lbl_eu_80663D4C;
extern "C" void func_eu_80053FDC() { lbl_eu_80663D4C = 1; }

bool CActParamAnim::func_80053990() { return false; }

bool CActParamAnim::func_80053998() { return false; }

bool CActParamAnim::func_800539A0() { return false; }

bool CActParamAnim::func_800539A8() { return false; }

bool CActParamAnim::func_800539B0() { return false; }

bool CActParamAnim::func_800539B8() { return false; }

bool CActParamAnim::func_800539C0() { return false; }

bool CActParamAnim::func_800539C8() { return false; }

bool CActParamAnim::func_800539D0() { return false; }

bool CActParamAnim::func_800539D8() { return false; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void sinit_800539E0() {
    extern float lbl_eu_8066AF20;
    extern float lbl_eu_80663D48;
    lbl_eu_80663D48 = lbl_eu_8066AF20;
}
