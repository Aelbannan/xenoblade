// Auto-scaffolded catalog TU for kyoshin/code_8027513C
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/code_8027513C.cpp" line 3 "kyoshin/harness_catalog.hpp" */
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

/* "src/kyoshin/code_8027513C.cpp" line 5 "kyoshin/code_8027513C.hpp" */
#pragma once

/* "src/kyoshin/code_8027513C.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {

class CfObjectImplTbox {
public:
    virtual ~CfObjectImplTbox();

    // TODO: add fields
};

class CfObjectMove;
class CfObjectEff;

} // namespace cf

// Target data struct pointed to by CfObjectMove::mTargetC4.
// Real name unknown; used by func_80275454.
struct UnkTargetData {
    u8 _00[0xC];
    u32 field_0xC;           // bit flags
    u8 _10[0x3C4 - 0x10];
    float field_0x3C4;
    u8 _3C8[0x4EC - 0x3C8];
    u32 field_0x4EC;          // bit flags
    u8 _4F0[0x4F8 - 0x4F0];
    float field_0x4F8;
    u8 _4FC[0x504 - 0x4FC];
    float field_0x504;
    u8 _508[0x50C - 0x508];
    float field_0x50C;
};

// Position container pointed to by CfObjectModel::field_0x90[8].
// Real name unknown; contains world-space coordinates for effects.
struct UnkPosContainer {
    u8 _000[0x760];
    float posX;    // 0x760
    float posY;    // 0x764
    float posZ;    // 0x768
};

// Camera/scene settings object returned by func_8049603C.
// Real name unknown; float at 0xC read by func_80275454.
struct UnkCamObj {
    u8 _00[0xC];
    float field_0xC;
};

// Layout wrapper for CfObjectEff to access field at offset 0xB0
// without pointer arithmetic. Real field name unknown.
struct CfObjectEffB0 {
    u8 _00[0xB0];
    u8* field_0xB0;
};

// TODO: identify real class name; contains embedded cf::CfObjectImplTbox subobjects
class UnkCode8027513C {
public:
    u8 _00[0x14];
    cf::CfObjectMove* field_0x14;   // pointer to CfObjectMove-derived instance
    u8 _18[0x1C - 0x18];
    int field_0x1C;
    int field_0x20;
    u8 _24[0x6C - 0x24];
    u32 field_0x6C;                 // counter for branching logic
    u8 _70[0x74 - 0x70];
    cf::CfObjectEff* field_0x74;
    s16 field_0x78;
};

/* end "kyoshin/code_8027513C.hpp" */
/* "src/kyoshin/code_8027513C.cpp" line 6 "kyoshin/cf/CfGameManager.hpp" */
#pragma once

/* "include/kyoshin/cf/CfGameManager.hpp" line 2 "types.h" */
/* end "types.h" */

class CPad;
class CScnNw4r;
class CView;
class UnkClass_80186D20;

struct CfGameManagerData1C {
    u8 field_0x0[0xC];
};

/* TODO: it's possible this file contains multiple separate classes, either just all being put in here,
or due to being in separate files, but compiled together in one file (unity compilation). For now,
to make things simpler, everything exists in a single class. */
namespace cf{
    class CfPadData;
    class CfObject;
    struct CfObjectSub54;
    class CfObjectMove;
    //unofficial name
    class CfGameManager{
    public:
        CfGameManager();

        static CfGameManager* getInstance();
        static CfGameManager* init(CScnNw4r* spSene, CView* pView, bool arg3);

        static void func_80086B5C(int arg1, int arg2, int arg3);
        static void enablePadFlags(u32 enableFlags, bool enable);
        static bool func_8007E1B4();
        static void func_8007E218();
        static void func_8007E514(int, int, char const*, int, int);
        static void func_8007F930(bool arg1);
        static UNKWORD func_800822F4();
        static bool func_800829B8();
        static u32 getCurrentPadChannel();
        static UNKTYPE* func_80083298();
        static CfObjectMove* getPlayer(int playerIndex);
        static u32 getEnabledInputFlags();
        static bool func_80086F9C();
        static bool func_80086F9C(s16) { return func_80086F9C(); }
        static void setCurrentPadPtr(const CPad* pad, u32 channel);
        static CPad* getPad(int channel);
        static void setPad(int r3, CPad* pPad, u32 r5);
        static CfPadData* getCfPadData();
        static CPad* getCurrentPad();

        static bool checkUnkFlag(int bit){
            return sUnkFlags & (1 << bit);
        }

        static void setUnkFlag(int bit, bool state){
            if(state == true) sUnkFlags |= (1 << bit);
            else sUnkFlags &= ~(1 << bit);
        }

        u32 unk0;
        CfObject* field_0x4;
        u32 mObjectFlags;
        u8 field_0xC[0xC];
        u16 field_0x18;
        u8 field_0x1A[2];
        CfGameManagerData1C field_0x1C;
        u8 unk28;
        u8 unk29[0x68 - 0x29];
        u32 unk68;
        u8 unk6C;
        u8 unk6D[3];          // 0x6D-0x6F
        u32 unk70;             // 0x70-0x73
        u8 unk74[8];           // 0x74-0x7B
        u32 unk7C;
        u8 unk80[0x8C - 0x80];
        u32 unk8C;
        u32 unk90;
        //between CObjectParam - CfObjectMove
        //likely player character object array, seems to always store pointers
        //to CfObjectPc objects except pointing at the 4th vtable
        CfObjectMove* unk94[3];
        u32 unkA0;
        UnkClass_80186D20* field_0xA4;
        u32 unkA8;
        u32 unkAC;
        u32 unkB0;
        u32 unkB4;

        static u32 sUnkFlags;
        static CScnNw4r* spScene;
public:
    void func_8007C0F8();
    void func_8007C140();
    void func_8007C188(unsigned long flags);
    void func_8007C198();
    void func_8007C2F4();
    void func_8007C344();
    void func_8007C360();
    void func_8007C374();
    void func_8007C4B4();
    virtual ~CfGameManager() {}
    void func_8007C5B8();
    cf::CfObjectMove** func_8007C6B4(cf::CfObjectMove** slots, int index);
    void func_8007C6C0();
    void func_8007C8C8();
    bool func_8007CBC8();
    void func_8007CBD4();
    void func_8007CBEC();
    void func_8007CDA8();
    void func_8007CE94();
    void func_8007CF64() const;
    void func_8007D190(unsigned long flags);
    void func_8007D1A0();
    void func_8007D794();
    void func_8007D7A4();
    void func_8007D834();
    void func_8007D84C();
    void func_8007DA00();
    void func_8007DA0C();
    void func_8007DCA8();
    void func_8007DCB8();
    void func_8007DE94();
    void func_8007DECC();
    u16 func_8007E030();
    void func_8007E038();
    CfObject** func_8007E0C8();
    void func_8007E0D0();
    void func_8007E4CC();
    void func_8007E4DC();
    void func_8007E864();
    void func_8007E908();
    void func_8007E960();
    void func_8007E9CC();
    void func_8007EEE0();
    void func_8007EEF0();
    void func_8007EEF8();
    void func_8007EF04();
    void func_8007EF44();
    void func_8007EF48();
    void func_8007EF4C();
    void func_8007F044();
    void func_8007F054();
    void func_8007F0A4();
    void func_8007F0AC();
    void func_8007F0C4();
    void func_8007F114();
    void func_8007F11C();
    void func_8007F1FC();
    void func_8007F830();
    void func_8007F8B8();
    void func_8007F8C0();
    void func_8007F8D0();
    void func_8007F8DC();
    void func_8007F8F4();
    void func_8007F900();
    bool func_8007F91C();
    void func_8007F990();
    void func_8007F9AC();
    void func_8007F9B4();
    void func_8007F9BC();
    void func_8007F9C4();
    void func_8007FBFC();
    void func_8007FC2C();
    void func_8007FC5C();
    void func_8007FD00();
    void func_8007FD8C();
    void func_8007FE18();
    void func_8007FE1C();
    void func_8007FE20();
    void func_8007FE24();
    void func_8007FE2C();
    void func_8007FECC();
    void func_8007FF6C();
    void func_8008064C();
    void func_800807BC();
    void func_80080888();
    void func_80080E20();
    void func_80080E28();
    void func_80080E30();
    void func_80080E44();
    void func_80080EE4();
    void func_80080F40();
    void func_80080F44();
    void func_80080F48();
    void func_80081258();
    void func_80081264();
    void func_8008126C();
    void func_80081274();
    void func_8008127C();
    void func_80081284();
    void func_8008128C();
    void func_80081294();
    void func_8008129C();
    void func_800812A4();
    void func_800812AC();
    void func_800812B4();
    void func_800812BC();
    void func_800812C4();
    void func_800812CC();
    void func_800812D4();
    void func_800812DC();
    void func_800812E4();
    void func_800812EC();
    void func_800812F4();
    void func_80081318();
    void func_80081330();
    void func_80081338();
    void func_80081340();
    void func_80081348();
    void func_80081350();
    void func_80081358();
    void func_80081694();
    void func_800817A8();
    void func_800817B0();
    void func_800817BC();
    void func_80081874();
    void func_8008187C();
    void func_80081900();
    void func_80081988();
    void func_80081990();
    void func_80081A24();
    void func_80081A40();
    void func_80081CA0();
    void func_80081CB0();
    void func_80081CB8();
    void func_80081CBC();
    void func_80081D2C();
    void func_80081D88();
    void func_80081D8C();
    void func_80081DD8();
    void func_80081E90();
    void func_80081F28();
    void func_80081F90();
    void func_80082008();
    void func_80082060();
    void func_80082088();
    void func_80082104();
    void func_8008212C();
    void func_800821F8();
    void func_8008221C();
    void func_80082254();
    void func_80082258();
    void func_8008228C();
    void func_800822FC();
    void func_80082354();
    void func_8008235C();
    void func_800823A4();
    void func_80082418();
    void func_800824FC();
    void func_80082544();
    void func_80082568();
    void func_80082614();
    void func_8008261C();
    bool func_80082680();
    void func_80082694();
    void func_8008269C();
    void func_800826F0();
    void func_80082768();
    void func_80082770();
    void func_800827A8();
    void func_800827E4();
    void func_80082834();
    void func_800828DC();
    void func_80082900();
    void func_80082940();
    void func_8008294C();
    void func_80082A0C();
    void func_80082A7C();
    void func_80082B38();
    void func_80082C48();
    void func_80082D90();
    void func_80082E50();
    void func_80082EC0();
    void func_80082EC4();
    void func_80082F2C();
    void func_80082FCC();
    void func_80082FE4();
    void func_80083100();
    void func_8008310C();
    void func_80083118();
    void func_80083284();
    void func_80083290();
    void func_800832BC();
    void func_80083304();
    void func_80083328();
    void func_80083458();
    void func_80083460();
    void func_80083468();
    void func_80083470();
    bool func_80083538();
    bool func_80083544();
    void func_80083550();
    void func_80083560();
    void func_800835FC();
    void func_8008360C();
    void func_80083718();
    void func_8008372C();
    void func_80083878();
    void func_80083888();
    void func_800838F4();
    void func_80083C70();
    void func_80083C78();
    void func_80083CC8();
    void func_80083CD8();
    void func_80083D50();
    void func_80083D70();
    void func_80083DEC();
    void func_80083EA4();
    void func_80083F28();
    void func_8008402C();
    void func_8008413C();
    void func_80084654();
    void func_80084A00();
    void func_80084AD4();
    void func_80084B68();
    void func_80084BAC();
    bool func_80084BF4();
    void func_80084C10();
    void func_80084CA4();
    void func_80084F50();
    void func_80085220();
    void func_80085248();
    void func_80085334();
    void func_800853C8();
    void func_8008566C();
    void func_80085838();
    bool func_80085840();
    bool func_8008585C();
    void func_80085878();
    void func_800858B8();
    void func_80085978();
    void func_80085E58();
    void func_80085FB8();
    void func_800862D0();
    void func_800863F4();
    void func_80086490();
    void func_800865E8();
    void func_800866A0();
    void func_8008670C();
    void func_80086778();
    void func_80086B04();
    void func_80086B08();
    void func_80086B0C();
    void func_80086B10();
    void func_80086B14();
    void func_80086B18();
    void func_80086B1C();
    void func_80086B24();
    void func_80086B2C();
    void func_80086B34();
    void func_80086B3C();
    void func_80086B44();
    void func_80086B48();
    void func_80086D90();
    void func_80086D94();
    void func_80086D98();
    void func_80086D9C();
    void func_80086DA0();
    void func_80086DA4();
    void func_80086DA8();
    void func_80086DAC();
    void func_80086DB0();
    void func_80086DB4();
    void func_80086DBC();
    void func_80086E6C();
    bool func_80087244();
    bool func_80087250();
    void func_80087280();
    void func_80087330();
    void func_80087334();
    void func_80087348();
    void func_80087364();
    void func_80087378();
    void func_80087390();
    void func_800873AC();
    CfObjectSub54* func_800873C8();
    void func_800873D4();
    void func_800873E8();
    void func_800873FC();
    void func_80087410();
    u32 func_80087424();
    void func_8008742C();
    void func_8008743C();
    void func_80087448();
    }; //size = 0xB8
} // namespace cf
/* end "kyoshin/cf/CfGameManager.hpp" */
/* "src/kyoshin/code_8027513C.cpp" line 7 "kyoshin/cf/object/CfObjectMove.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObjectMove.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObjectMove.hpp" line 3 "kyoshin/cf/object/CfObjectModel.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObjectModel.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObjectModel.hpp" line 3 "kyoshin/cf/object/CfObject.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObject.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObject.hpp" line 3 "kyoshin/cf/object/CObjectParam.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CObjectParam.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CObjectParam.hpp" line 3 "kyoshin/cf/object/CObjectState.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CObjectState.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {
    //min size: 0x10
    class CObjectState {
    public:
        virtual void CObjectState_UnkVirtualFunc1();  //0x8
        virtual void CObjectState_UnkVirtualFunc2();  //0xC
        virtual void CObjectState_UnkVirtualFunc3();  //0x10
        virtual void CObjectState_UnkVirtualFunc4();  //0x14
        virtual void CObjectState_UnkVirtualFunc5();  //0x18
        virtual void CObjectState_UnkVirtualFunc6();  //0x1C
        virtual void CObjectState_UnkVirtualFunc7();  //0x20
        virtual void CObjectState_UnkVirtualFunc8();  //0x24
        virtual void CObjectState_UnkVirtualFunc9();  //0x28
        virtual void CObjectState_UnkVirtualFunc10(); //0x2C
        virtual void CObjectState_UnkVirtualFunc11(); //0x30
        virtual void* CObjectState_UnkVirtualFunc12(); //0x34
        virtual void CObjectState_UnkVirtualFunc13(); //0x38

        //0x0: vtable
        u32 unk4;          // 0x04
        u32 unk8;          // 0x08
        u32 unkC;          // 0x0C
    };
}
/* end "kyoshin/cf/object/CObjectState.hpp" */

namespace cf {
    //min size: 0x38
    class CObjectParam : public CObjectState {
    public:
        virtual void CObjectParam_UnkVirtualFunc1(); //0x3C
        virtual void CObjectParam_UnkVirtualFunc2(); //0x40
        virtual int CObjectParam_UnkVirtualFunc3(); //0x44
        virtual void CObjectParam_UnkVirtualFunc4(); //0x48
        virtual BOOL CObjectParam_UnkVirtualFunc5(); //0x4C
        virtual void CObjectParam_UnkVirtualFunc6(); //0x50

        //0x0: vtable
        //0x0-10: CObjectState
        void* mPtr10;          // 0x10-0x13 (pointer stored at offset 0x10)
        u8 unk14[0x20 - 0x14]; // 0x14-0x2F
        u32 field_30;          // 0x30  — checked for non-zero by UnkVirtualFunc3
        u8  unk34[4];          // 0x34..0x37  (remainder of old unk10_3[0x28])
    };
}
/* end "kyoshin/cf/object/CObjectParam.hpp" */

namespace cf {
    struct CfObjectSub54 {
        u8 field_0x0[0xC];
    };

    //min size: 0x70
    class CfObject : public CObjectParam {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObject();                      //0x54
        virtual void CfObject_UnkVirtualFunc2() = 0;  //0x58
        virtual void CfObject_UnkVirtualFunc3();      //0x5C
        virtual void CfObject_UnkVirtualFunc4() = 0;  //0x60
        virtual void CfObject_UnkVirtualFunc5();      //0x64
        virtual void CfObject_UnkVirtualFunc6();      //0x68
        virtual void CfObject_UnkVirtualFunc7() = 0;  //0x6C
        virtual void CfObject_UnkVirtualFunc8() = 0;  //0x70
        virtual void CfObject_UnkVirtualFunc9();      //0x74
        virtual void CfObject_UnkVirtualFunc10();     //0x78
        virtual void CfObject_UnkVirtualFunc11();     //0x7C
        virtual void CfObject_UnkVirtualFunc12();     //0x80
        virtual void CfObject_UnkVirtualFunc13();     //0x84
        virtual void CfObject_UnkVirtualFunc14();     //0x88
        virtual void CfObject_UnkVirtualFunc15();     //0x8C
        virtual void CfObject_UnkVirtualFunc16();     //0x90
        virtual void CfObject_UnkVirtualFunc17();     //0x94
        virtual void CfObject_UnkVirtualFunc18();     //0x98
        virtual void CfObject_UnkVirtualFunc19();     //0x9C
        virtual void CfObject_UnkVirtualFunc20(float a, float b);     //0xA0
        virtual void CfObject_UnkVirtualFunc21();     //0xA4
        virtual void CfObject_UnkVirtualFunc22();     //0xA8
        virtual void CfObject_UnkVirtualFunc23();     //0xAC
        virtual void CfObject_UnkVirtualFunc24();     //0xB0
        virtual void CfObject_UnkVirtualFunc25();     //0xB4
        virtual void CfObject_UnkVirtualFunc26();     //0xB8
        virtual void CfObject_UnkVirtualFunc27();     //0xBC
        virtual void CfObject_UnkVirtualFunc28();     //0xC0
        virtual void CfObject_UnkVirtualFunc29();     //0xC4
        virtual void CfObject_UnkVirtualFunc30();     //0xC8
        virtual void CfObject_UnkVirtualFunc31();     //0xCC
        virtual void CfObject_UnkVirtualFunc32();     //0xD0
        virtual void CfObject_UnkVirtualFunc33();     //0xD4
        virtual void CfObject_UnkVirtualFunc34();     //0xD8
        virtual void CfObject_UnkVirtualFunc35();     //0xDC
        virtual void CfObject_UnkVirtualFunc36();     //0xE0
        virtual void CfObject_UnkVirtualFunc37();     //0xE4
        virtual void CfObject_UnkVirtualFunc38();     //0xE8
        virtual void CfObject_UnkVirtualFunc39();     //0xEC
        virtual void CfObject_UnkVirtualFunc40();     //0xF0
        virtual void CfObject_UnkVirtualFunc41();     //0xF4
        virtual void CfObject_UnkVirtualFunc42();     //0xF8
        virtual void CfObject_UnkVirtualFunc43();     //0xFC
        virtual void CfObject_UnkVirtualFunc44();     //0x100
        virtual void CfObject_UnkVirtualFunc45();     //0x104
        virtual void CfObject_UnkVirtualFunc46();     //0x108
        virtual void CfObject_UnkVirtualFunc47();     //0x10C
        virtual void CfObject_UnkVirtualFunc48();     //0x110
        virtual void CfObject_UnkVirtualFunc49();     //0x114
        virtual void CfObject_UnkVirtualFunc50();     //0x118
        virtual void CfObject_UnkVirtualFunc51();     //0x11C
        virtual void CfObject_UnkVirtualFunc52();     //0x120
        virtual CfObject* CfObject_UnkVirtualFunc53(); //0x124
        virtual void CfObject_UnkVirtualFunc54();     //0x128
        virtual void CfObject_UnkVirtualFunc55();     //0x12C
        virtual float CfObject_UnkVirtualFunc56();     //0x130
        virtual void CfObject_UnkVirtualFunc57();     //0x134
        virtual u32* CfObject_UnkVirtualFunc58();     //0x138
        virtual void CfObject_UnkVirtualFunc59();     //0x13C
        virtual void CfObject_UnkVirtualFunc60();     //0x140
        virtual void CfObject_UnkVirtualFunc61();     //0x144
        virtual void CfObject_UnkVirtualFunc62();     //0x148
        virtual void CfObject_UnkVirtualFunc63();     //0x14C
        virtual void CfObject_UnkVirtualFunc64();     //0x150
        virtual void CfObject_UnkVirtualFunc65();     //0x154
        virtual void CfObject_UnkVirtualFunc66() = 0; //0x158
        virtual void CfObject_UnkVirtualFunc67();     //0x15C
        virtual void CfObject_UnkVirtualFunc68() = 0; //0x160
        virtual void CfObject_UnkVirtualFunc69();     //0x164
        virtual void CfObject_UnkVirtualFunc70();     //0x168
        virtual void CfObject_UnkVirtualFunc71();     //0x16C
        virtual void CfObject_UnkVirtualFunc72();     //0x170
        virtual void CfObject_UnkVirtualFunc73();     //0x174

        //not sure if belongs here? (can be in any class from CObjectState to CfObjectMove)
        void func_800BE898(int, u32, float, float);


        //0x0: vtable
        // CObjectParam currently ends at 0x28.
        u8 field_0x28[0x10];
        void* mSubObj38;          // 0x38-0x3B
        u8 _pad3C[0x4C - 0x3C];   // 0x3C-0x4B
        u32 mField4C;             // 0x4C-0x4F (also read as float)
        u8 _pad50[0x54 - 0x50];   // 0x50-0x53
        CfObjectSub54 mSubObj54;     // 0x54-0x5F
        float mFloat60;            // 0x60-0x63
        u32 unk64;                  // 0x64-0x67
        u32 mFlags68;               // 0x68-0x6B
        u8 _pad6C[0x70 - 0x6C];    // 0x6C-0x6F
    };
}
/* end "kyoshin/cf/object/CfObject.hpp" */

namespace cf {
    //min size: 0xbe
    class CfObjectModel : public CfObject {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObjectModel();
        //vtable 1 (CfObjectModel)
        virtual void CfObjectModel_UnkVirtualFunc1();  //0x178
        virtual void CfObjectModel_UnkVirtualFunc2();  //0x17C
        virtual void CfObjectModel_UnkVirtualFunc3();  //0x180
        virtual void CfObjectModel_UnkVirtualFunc4();  //0x184
        virtual void CfObjectModel_UnkVirtualFunc5();  //0x188
        virtual void CfObjectModel_UnkVirtualFunc6();  //0x18C
        virtual void CfObjectModel_UnkVirtualFunc7();  //0x190
        virtual void CfObjectModel_UnkVirtualFunc8();  //0x194
        virtual void CfObjectModel_UnkVirtualFunc9();  //0x198
        virtual void CfObjectModel_UnkVirtualFunc10(); //0x19C
        virtual void CfObjectModel_UnkVirtualFunc11(); //0x1A0
        virtual void CfObjectModel_UnkVirtualFunc12(); //0x1A4
        virtual void CfObjectModel_UnkVirtualFunc13(); //0x1A8
        virtual void CfObjectModel_UnkVirtualFunc14(); //0x1AC
        virtual void CfObjectModel_UnkVirtualFunc15(); //0x1B0
        virtual void CfObjectModel_UnkVirtualFunc16(); //0x1B4
        virtual void CfObjectModel_UnkVirtualFunc17(); //0x1B8
        virtual void CfObjectModel_UnkVirtualFunc18(); //0x1BC
        virtual void CfObjectModel_UnkVirtualFunc19(); //0x1C0
        virtual void CfObjectModel_UnkVirtualFunc20(); //0x1C4

        //0x0: vtable
        // CfObject ends at 0x70.
        u8 field_0x70[0x1C];
        u16 unk8C_3;
        u16 field_0x8E;
        u8 field_0x90[0x20]; // 0x90-0xAF
        void* mSubObjB0;      // 0xB0-0xB3
        u8 unkB4[0xBC - 0xB4]; // 0xB4-0xBB
        u8 field_BC;          // 0xBC
        u8 field_BD;          // 0xBD
    CfObjectModel();
    void CfObject_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc8();
    void CfObject_UnkVirtualFunc63();
    void CfObject_UnkVirtualFunc19();
    void CfObject_UnkVirtualFunc22();
    void CfObject_UnkVirtualFunc20();
    void CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27();
    void CfObject_UnkVirtualFunc29();
    void CfObject_UnkVirtualFunc32();
    void CfObject_UnkVirtualFunc34();
    void CfObject_UnkVirtualFunc33();
    void CfObject_UnkVirtualFunc30();
    float CfObject_UnkVirtualFunc56();
    void CfObject_UnkVirtualFunc52();
    CfObject* CfObject_UnkVirtualFunc53();
    void CfObject_UnkVirtualFunc54();
    void CfObject_UnkVirtualFunc55();
    void CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc66();
    void CfObject_UnkVirtualFunc67();
    void CfObject_UnkVirtualFunc70();
    void CfObject_UnkVirtualFunc69();
    void CfObject_UnkVirtualFunc68();
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc28();
    void CfObject_UnkVirtualFunc31();
    void CfObject_UnkVirtualFunc35();
    void CfObject_UnkVirtualFunc36();
    void CfObject_UnkVirtualFunc72();
    };
}

/* end "kyoshin/cf/object/CfObjectModel.hpp" */

namespace cf {
    //min size: 0x715
    class CfObjectMove : public CfObjectModel {
    public:
        CfObjectMove();
        //vtable 1 (CfObject)
        virtual ~CfObjectMove();
        //vtable 1 (CfObjectMove)
        virtual void CfObjectMove_UnkVirtualFunc1();  //0x1C8
        virtual void CfObjectMove_UnkVirtualFunc2();  //0x1CC
        virtual void CfObjectMove_UnkVirtualFunc3();  //0x1D0
        virtual void CfObjectMove_UnkVirtualFunc4();  //0x1D4
        virtual void CfObjectMove_UnkVirtualFunc5();  //0x1D8
        virtual void CfObjectMove_UnkVirtualFunc6();  //0x1DC
        virtual void CfObjectMove_UnkVirtualFunc7();  //0x1E0
        virtual void CfObjectMove_UnkVirtualFunc8();  //0x1E4
        virtual void CfObjectMove_UnkVirtualFunc9();  //0x1E8
        virtual void CfObjectMove_UnkVirtualFunc10(); //0x1EC
        virtual void CfObjectMove_UnkVirtualFunc11(); //0x1F0
        virtual void CfObjectMove_UnkVirtualFunc12(); //0x1F4
        virtual void CfObjectMove_UnkVirtualFunc13(); //0x1F8
        virtual void CfObjectMove_UnkVirtualFunc14(); //0x1FC
        virtual void CfObjectMove_UnkVirtualFunc15(); //0x200
        virtual void CfObjectMove_UnkVirtualFunc16(); //0x204
        virtual void CfObjectMove_UnkVirtualFunc17(); //0x208
        virtual void CfObjectMove_UnkVirtualFunc18(); //0x20C
        virtual void CfObjectMove_UnkVirtualFunc19(); //0x210
        virtual void CfObjectMove_UnkVirtualFunc20(); //0x214
        virtual void CfObjectMove_UnkVirtualFunc21(); //0x218
        virtual void CfObjectMove_UnkVirtualFunc22(); //0x21C
        virtual void CfObjectMove_UnkVirtualFunc23(); //0x220

        //0x0: vtable
        //0x0-BE: CfObjectModel
        // Field layout starting at offset 0xBE:
        u8 _BE[6];              // 0xBE-0xC3
        void* mTargetC4;         // 0xC4-0xC7
        u8 _C8[0x544];           // 0xC8-0x60B
        u8 _60C_region[0xB4];   // 0x60C-0x6BF
        void* mTarget6C0;         // 0x6C0-0x6C3
        u8 _6C4[5];              // 0x6C4-0x6C8
        u8 mFlags6C9;             // 0x6C9
        u8 _6CA[0x26];           // 0x6CA-0x6EF
        float mMoveSpeed;         // 0x6F0-0x6F3
        u8 _6F4[0x21];           // 0x6F4-0x714
        u8 unk715[3];            // 0x715-0x717
    void CfObject_UnkVirtualFunc4();
    void CfObject_UnkVirtualFunc7();
    void CfObject_UnkVirtualFunc6();
    void CfObjectModel_UnkVirtualFunc1();
    void CfObjectModel_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc5();
    void CfObject_UnkVirtualFunc46();
    void CfObject_UnkVirtualFunc47();
    void CfObject_UnkVirtualFunc49();
    void CfObject_UnkVirtualFunc64();
    void CfObject_UnkVirtualFunc65();
    void CfObject_UnkVirtualFunc19();
    void CfObject_UnkVirtualFunc22();
    void CfObject_UnkVirtualFunc25();
    void CfObject_UnkVirtualFunc26();
    void CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27();
    void CfObject_UnkVirtualFunc30();
    void CfObject_UnkVirtualFunc32();
    void CfObject_UnkVirtualFunc33();
    void CfObject_UnkVirtualFunc13();
    void CfObject_UnkVirtualFunc57();
    void CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc14();
    void CfObject_UnkVirtualFunc15();
    void CfObject_UnkVirtualFunc16();
    void CfObject_UnkVirtualFunc17();
    void CfObjectModel_UnkVirtualFunc18();
    void CfObject_UnkVirtualFunc9();
    void CfObject_UnkVirtualFunc10();
    void CfObject_UnkVirtualFunc61();
    void CfObject_UnkVirtualFunc62();
    void CfObject_UnkVirtualFunc12();
    void CfObject_UnkVirtualFunc66();
    void CfObjectModel_UnkVirtualFunc19();
    void CfObjectModel_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc37();
    void CfObject_UnkVirtualFunc38();
    void CfObject_UnkVirtualFunc39();
    void CfObject_UnkVirtualFunc40();
    void CfObject_UnkVirtualFunc42();
    void CfObject_UnkVirtualFunc43();
    void CfObject_UnkVirtualFunc45();
    void CfObject_UnkVirtualFunc70();
    void CfObject_UnkVirtualFunc50();
    void CfObject_UnkVirtualFunc51();
    void CfObject_UnkVirtualFunc60();
    void CfObject_UnkVirtualFunc29(float value);
    void setMoveSpeed(float value);
    void resetMoveSpeed();
    void updatePos();
    void* getUnk54();
    int getSubState();
    void freeSub();
    void setSubFieldC(unsigned short val);
    int getSubFieldA();
    void setSubFieldA(unsigned short val);
    int getSubFieldE();
    void setSubFieldE(unsigned short val);
    void virtCall10();
    int nullsub_25();
    int nullsub_26();
    int nullsub_27();
    int nullsub_28();
    int isActive();
    void setBit6c9(unsigned long bit);
    cf::CfObjectMove* testFlag8();
    };
}

/* end "kyoshin/cf/object/CfObjectMove.hpp" */
/* "src/kyoshin/code_8027513C.cpp" line 8 "kyoshin/cf/object/CfObjectEff.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObjectEff.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObjectEff.hpp" line 3 "kyoshin/cf/object/CfObject.hpp" */
/* end "kyoshin/cf/object/CfObject.hpp" */

namespace cf {

struct CfObjectEffChild {
    u8 _pad00[0x2C];
    float unk2C;
    u8 _pad30[0x34 - 0x30];
    u32 unk34[3];
    u32 unk40[4];
    u8 _pad50[0x59 - 0x50];
    u8 unk59;
    u8 _pad5A[0x5C - 0x5A];
    int unk5C;
};

class CfObjectEff : public CfObject {
public:
    u8 _pad70[0x94 - 0x70];
    CfObjectEffChild* mChildEff;
    u8 _pad98[0xA4 - 0x98];
    u16 mFlagsA4;
    u16 mCountA6;
    u8 _padA8[0xC0 - 0xA8];

    CfObjectEff();
    void func_800AC7CC();
    void func_800AC7FC();
    void func_800AC810();
    void func_800AC86C();
    void func_800AC990();
    void func_800ACA58();
    void func_800ACAE8();
    void func_800ACB08();
    void func_800ACBA4();
    void func_800ACBCC();
    void func_800ACC3C();
    void func_800ACC94();
    void func_800ACCD4();
    void func_800ACCE4();
    void func_800ACD4C();
    void func_800ACD5C();
    void func_800ACDA0();
    void func_800ACDE0();
    void func_800ACDFC();
    void func_800ACE60();
    void func_800ACE78();
    void func_800ACEAC();
    void func_800ACF50() const;
    void func_800AD3A4();
    void func_800AD4A4();
    void func_800AD68C();
    virtual ~CfObjectEff();
    void func_800AD818();
    void func_800AD830();
    void func_800AD840();
    void func_800ACE44();
    void func_800ACF34() const;
    void func_800AD850();
    void func_800AD858();
};

} // namespace cf
/* end "kyoshin/cf/object/CfObjectEff.hpp" */

class CScn;

// Forward declarations with retail-accurate signatures.
// func_8008187C is declared void() in CfGameManager.hpp but retail
// takes an int type code in r3 and returns CfObjectEff*.
cf::CfObjectEff* func_8008187C(int type);
void func_800ACC14(void* self, s8 val);
void func_800ACC64(cf::CfObjectEff* obj, const float* src);
void func_800ACF78(cf::CfObjectEff* eff, cf::CfObjectMove* obj, int arg);
void func_801BFDE8(int arg0, int arg1, float* pos, float arg3, float arg4);
UnkCamObj* func_8049603C(CScn* arg);

// Float constants from sdata2 (retail .sda21 loads); extern refs match retail relocs
extern "C" float lbl_eu_806689D8; // -4.0
extern "C" float lbl_eu_806689DC; // 0.9
extern "C" float lbl_eu_806689E8; // 1.0
extern "C" float lbl_eu_806689EC; // 30.0
extern "C" float lbl_eu_806689F0; // 0.001
extern "C" float lbl_eu_806689F4; // 0.1
extern "C" float lbl_eu_806689F8; // 0.4

extern "C" CScn* lbl_eu_80663E14;

int func_802759A8(void* self) { return 0; }

// Vtable helper structs for calling methods whose header signature
// differs from the retail ABI (return type or extra parameters).
struct CfObjectMove_VTable43 {
    u32 _pad[43];
    float* (*fn)(cf::CfObjectMove*);
};

struct CfObjectEff_VTable39 {
    u32 _pad[39];
    void (*fn)(cf::CfObjectEff*, float*);
};

void func_80275454(UnkCode8027513C* self) {
    cf::CfObjectMove* obj;
    UnkTargetData* target;
    
    obj = self->field_0x14;
    target = (UnkTargetData*)obj->mTargetC4;
    
    if (target == nullptr) {
        return;
    }
    
    // Check if field_0x4EC has bit 1 (0x2) set
    if (target->field_0x4EC & 2) {
        self->field_0x6C++;
        
        // If field_0x3C4 is below threshold, spawn/manage an effect
        if (target->field_0x3C4 < lbl_eu_806689F0) {
            // CfObject_UnkVirtualFunc23 returns float* in retail (header says void)
            float* pos = (*(CfObjectMove_VTable43**)obj)->fn(obj);
            
            // Reload after vtable call (may have been modified)
            obj = self->field_0x14;
            if (obj != nullptr) {
                UnkTargetData* reloadedTarget = (UnkTargetData*)obj->mTargetC4;
                int type = 46;  // 0x2e
                int mode = 217; // 0xd9
                
                if (reloadedTarget->field_0x3C4 > lbl_eu_806689D8 ||
                    reloadedTarget->field_0x4F8 < lbl_eu_806689DC) {
                    type = 45;  // 0x2d
                    mode = 216; // 0xd8
                }
                
                cf::CfObjectEff* eff = func_8008187C(type);
                if (eff != nullptr) {
                    // Vector addition: pos + (0.0, 0.01, 0.0)
                    // Retail uses paired singles for the bulk add; decomp produces scalar ops.
                    float tmp[3];
                    tmp[0] = pos[0] + 0.0f;
                    tmp[1] = pos[1] + 0.01f;
                    tmp[2] = pos[2] + 0.0f;
                    
                    float result[3];
                    result[0] = tmp[0];
                    result[1] = tmp[1];
                    result[2] = tmp[2];
                    
                    // CfObject_UnkVirtualFunc19 takes float* in retail (header says void())
                    (*(CfObjectEff_VTable39**)eff)->fn(eff, result);
                    
                    // Load position from obj->field_0x90[8] (offset 0x98)
                    UnkPosContainer* posContainer =
                        *(UnkPosContainer**)(&obj->field_0x90[8]);
                    float posVec[4];
                    posVec[0] = posContainer->posX;
                    posVec[1] = posContainer->posY;
                    posVec[2] = posContainer->posZ;
                    posVec[3] = lbl_eu_806689E8;
                    func_800ACC64(eff, posVec);
                }
                
                // Play sound effect at position
                UnkCamObj* camObj = func_8049603C(lbl_eu_80663E14);
                float camVal = lbl_eu_806689E8 - camObj->field_0xC;
                func_801BFDE8(0, mode, pos, camVal, lbl_eu_806689EC);
            }
        }
        
        // Set target float fields if field_0x90[0] is non-null
        obj = self->field_0x14;
        if (*(u32*)(&obj->field_0x90[0]) != 0) {
            target->field_0x504 = lbl_eu_806689F4;
            target->field_0x50C = lbl_eu_806689F8;
        }
    } else {
        // Second branch: field_0x4EC bit 1 not set, check field_0xC
        if (target->field_0xC & 2) {
            // func_8007F91C ignores its 'this' pointer, so casting self works
            if (!((cf::CfGameManager*)self)->func_8007F91C()) {
                if (self->field_0x74 == nullptr) {
                    cf::CfObjectEff* eff = func_8008187C(190); // 0xbe
                    if (eff != nullptr) {
                        // Store &self->field_0x10 into eff at offset 0xB0.
                        // self+0x10 is the address of a CfObjectImplTbox subobject.
                        u8* ptr = (u8*)self + 0x10;
                        if (self == nullptr) {
                            ptr = nullptr;
                        }
                        ((CfObjectEffB0*)eff)->field_0xB0 = ptr;
                        
                        self->field_0x74 = eff;
                        func_800ACF78(eff, self->field_0x14, 0);
                        
                        obj = self->field_0x14;
                        obj->CfObject_UnkVirtualFunc36();
                        eff->CfObject_UnkVirtualFunc35();
                        
                        if (self->field_0x74 != nullptr && self->field_0x78 != 2) {
                            func_800ACC14(self->field_0x74, 2);
                            self->field_0x78 = 2;
                        }
                        
                        // Load position from obj->field_0x90[8] (offset 0x98)
                        UnkPosContainer* posContainer =
                            *(UnkPosContainer**)(&obj->field_0x90[8]);
                        float posVec[4];
                        posVec[0] = posContainer->posX;
                        posVec[1] = posContainer->posY;
                        posVec[2] = posContainer->posZ;
                        posVec[3] = lbl_eu_806689E8;
                        func_800ACC64(eff, posVec);
                    }
                }
            }
        }
    }
}

void func_802756F0(){}

void func_80275808(UnkCode8027513C* ptr) {
    ptr->field_0x20 = -1;
    ptr->field_0x1C = 0;
    ptr->field_0x74 = nullptr;
    ptr->field_0x78 = 0;
}

void func_80275824(){}

void func_80275850(){}

void func_8027594C(void* self){}

// If field_0x74 is set and field_0x78 differs from arg, notify the child
// effect via func_800ACC14 and cache the new value
void func_802753F8(UnkCode8027513C* self, s16 arg) {
    if (self->field_0x74 != nullptr) {
        if (self->field_0x78 != arg) {
            func_800ACC14(self->field_0x74, (s8)arg);
            self->field_0x78 = arg;
        }
    }
}

cf::CfObjectImplTbox::~CfObjectImplTbox() {}


void func_802759B0(void* self) { ((cf::CfObjectImplTbox*)((u8*)self - 0xc))->~CfObjectImplTbox(); }

void func_802759B8(void* self) { func_8027594C((u8*)self - 0x10); }

void func_802759C0(void* self) { ((cf::CfObjectImplTbox*)((u8*)self - 0x10))->~CfObjectImplTbox(); }
