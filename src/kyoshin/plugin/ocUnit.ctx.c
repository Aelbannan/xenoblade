// Auto-scaffolded catalog TU for kyoshin/plugin/ocUnit
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/plugin/ocUnit.cpp" line 3 "kyoshin/plugin/ocUnit.hpp" */
#pragma once

/**
 * Object-container / unit plugin + CfObject virtuals that live in this TU.
 * Class layouts and virtuals come from the recovered cf object headers.
 */

/* "src/kyoshin/plugin/ocUnit.hpp" line 7 "cstring" */
#ifndef MSL_CPP_CSTRING_H
#define MSL_CPP_CSTRING_H
/* "libs/PowerPC_EABI_Support/include/stl/cstring" line 2 "string.h" */
#ifndef MSL_STRING_H
#define MSL_STRING_H

/* "libs/PowerPC_EABI_Support/include/stl/string.h" line 3 "types.h" */
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

/* "src/kyoshin/plugin/ocUnit.hpp" line 9 "monolib/vm/yvm2.h" */
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
/* "src/kyoshin/plugin/ocUnit.hpp" line 10 "kyoshin/code_801862C0.hpp" */
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
/* "src/kyoshin/plugin/ocUnit.hpp" line 11 "kyoshin/cf/object/CActorParam.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CActorParam.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CActorParam.hpp" line 3 "kyoshin/cf/object/CDebugState.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CDebugState.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {
    class CDebugState {

    };
}
/* end "kyoshin/cf/object/CDebugState.hpp" */
/* "src/kyoshin/cf/object/CActorParam.hpp" line 4 "kyoshin/cf/object/CBattleState.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CBattleState.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {
    class UnkClass_CActorParam15E0;
    struct CBattleStateSrcEntry;

    // 0x34-byte slot layout used by CBattleState_UnkVirtualFunc6's incoming
    // arg (r4) and by the 8-entry array at CBattleState+0x1388. Same struct
    // shape reused for both (see MWCC_REFERENCE §CBattleState_UnkVirtualFunc6).
    struct CBattleStateEntry {
        u32 unk00; // 0x00
        u32 unk04; // 0x04
        u32 unk08; // 0x08
        u16 unk0C; // 0x0C - id; also bit index into CBattleState::unk15AC
        s32 unk10; // 0x10 - clamped value
        s16 unk14; // 0x14
        s16 unk16; // 0x16
        s16 unk18; // 0x18 - lower clamp bound (0 == no lower clamp)
        s16 unk1A; // 0x1A
        f32 unk1C; // 0x1C
        f32 unk20; // 0x20
        f32 unk24; // 0x24
        f32 unk28; // 0x28
        u16 unk2C; // 0x2C
        u16 unk2E; // 0x2E
        u32 unk30; // 0x30
    };

    // size: 0x15DC
    class CBattleState {
    public:
        virtual void CBattleState_UnkVirtualFunc1();  //0x8
        virtual void CBattleState_UnkVirtualFunc2();  //0xC
        virtual int CBattleState_UnkVirtualFunc3();  //0x10
        virtual void CBattleState_UnkVirtualFunc4();  //0x14
        virtual void CBattleState_UnkVirtualFunc5(CBattleStateEntry* entry);  //0x18
        virtual void CBattleState_UnkVirtualFunc6(CBattleStateEntry* entry);  //0x1C
        virtual void CBattleState_UnkVirtualFunc7();  //0x20
        virtual void CBattleState_UnkVirtualFunc8(CBattleStateEntry* entry);  //0x24
        virtual void CBattleState_UnkVirtualFunc9();  //0x28
        virtual void CBattleState_UnkVirtualFunc10(CBattleStateEntry* arg); //0x2C
        virtual void CBattleState_UnkVirtualFunc11(u32 mask); //0x30
        virtual void CBattleState_UnkVirtualFunc12(); //0x34
        virtual void* CBattleState_UnkVirtualFunc13(int index); //0x38
        virtual void CBattleState_UnkVirtualFunc14(); //0x3C
        virtual void CBattleState_UnkVirtualFunc15(); //0x40
        virtual void CBattleState_UnkVirtualFunc16(); //0x44
        virtual void CBattleState_UnkVirtualFunc17(); //0x48
        virtual void CBattleState_UnkVirtualFunc18(); //0x4C
        virtual void CBattleState_UnkVirtualFunc19(); //0x50
        virtual void CBattleState_UnkVirtualFunc20(); //0x54
        virtual void CBattleState_UnkVirtualFunc21(); //0x58
        virtual void CBattleState_UnkVirtualFunc22(); //0x5C
        virtual void CBattleState_UnkVirtualFunc23(); //0x60
        virtual void CBattleState_UnkVirtualFunc24(); //0x64
        virtual void CBattleState_UnkVirtualFunc25(); //0x68
        virtual void CBattleState_UnkVirtualFunc26(const CBattleStateSrcEntry* src); //0x6C
        virtual void CBattleState_UnkVirtualFunc27(); //0x70
        virtual void CBattleState_UnkVirtualFunc28(); //0x74
        virtual void CBattleState_UnkVirtualFunc29(); //0x78
        virtual void CBattleState_UnkVirtualFunc30(); //0x7C
        virtual int CBattleState_UnkVirtualFunc31(u32 id); //0x80
        virtual void CBattleState_UnkVirtualFunc32(); //0x84
        virtual int CBattleState_UnkVirtualFunc33(u32 id); //0x88

        CBattleState();

        u16 unk4;
        u16 unk6;
        u8 unk8[0x1520];
        u8 unk1528[4];
        u8 unk152C[0x80];
        u8 unk15AC[0x15D8 - 0x15AC];
        UnkClass_CActorParam15E0* field_0x15D8;
    };
}
/* end "kyoshin/cf/object/CBattleState.hpp" */
/* "src/kyoshin/cf/object/CActorParam.hpp" line 5 "kyoshin/cf/object/CActorState.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CActorState.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {
    class CActorState {
    public:
        virtual void CActorState_UnkVirtualFunc1(); //0x8

        CActorState(UNKTYPE* r4){
            unk4 = r4;
        }

        UNKTYPE* unk4;
    };
}
/* end "kyoshin/cf/object/CActorState.hpp" */
/* "src/kyoshin/cf/object/CActorParam.hpp" line 6 "kyoshin/cf/CArtsSet.hpp" */
#pragma once

/* "src/kyoshin/cf/CArtsSet.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/CArtsSet.hpp" line 3 "cstring" */
/* end "cstring" */

namespace cf {
    
    //size: 0x88
    class CAttackParam {
    public:
        u8 unk0;
        u8 unk4[0x20 - 0x4];
        u32 unk20;
        u32 unk24;
        u16 unk28;
        u8 unk2A;
        u8 unk2B;
        float unk2C;
        float unk30;
        u16 unk34;
        u16 unk36;
        u8 unk38[4];
        u16 unk3C;
        u8 unk3E;
        u8 unk3F;
        u16 unk40;
        u8 unk42;
        u8 unk43;
        u8 unk44;
        u16 unk46;
        u16 unk48;
        u16 unk4A;
        u16 unk4C;
        float unk50;
        float unk54;
        u16 unk58;
        u16 unk5A;
        u16 unk5C;
        u16 unk5E;
        float unk60;
        u16 unk64;
        u8 unk66;
        u8 unk67;
        u16 unk68;
        u16 unk6A;
        u8 unk6C[5];
        u8 unk71;
        u16 unk72;
        u16 unk74;
        u8 unk76;
        u8 unk77;
        u32 unk78;
        float unk7C;
        float unk80;

        CAttackParam();

        virtual void CAttackParam_UnkVirtualFunc1(){
            unk0 = 0;
            unk20 = 0;
            unk24 = 0;
            unk28 = 0;
            unk2A = 1;
            unk2B = 0;
            unk2C = 0;
            unk30 = 0;
            unk34 = 0;
            unk36 = 0;
            unk3C = 0;
            unk3E = 0;
            unk40 = 0;
            unk42 = 0;
            unk43 = 0;
            unk44 = 0;
            unk46 = 0;
            unk48 = 0;
            unk4A = 0;
            unk4C = 0;
            unk50 = 0;
            unk54 = 0;
            unk58 = 0;
            unk5A = 0;
            unk5C = 0;
            unk5E = 0;
            unk60 = 0;
            unk64 = 0;
            unk66 = 0;
            unk67 = 0;
            unk68 = 0;
            unk6A = 0;
            unk72 = 0;
            unk74 = 0;
            unk77 = 0;
            unk7C = 0;
            unk80 = 0;  

            std::memset(unk38, 0, sizeof(unk38));
            std::memset(unk6C, 0, sizeof(unk6C));
        }
        virtual u8 CAttackParam_UnkVirtualFunc2();
        virtual void CAttackParam_UnkVirtualFunc3(u8 r4);
        virtual void CAttackParam_UnkVirtualFunc4();
    };

    struct _sAttackSet {
    };

    //size: 0x334
    class CAttackSet : _sAttackSet {
    public:
        CAttackSet(){}
        virtual void func_80153E88();
        
        //0x0: vtable
        CAttackParam mAttackParams[6]; //0x4
    };

    //size: 0x8C
    class CArtsParam : public CAttackParam {
    public:
        UNKTYPE* unk88;

        CArtsParam();
        virtual void CArtsParam_UnkVirtualFunc1();
        virtual u8 CArtsParam_UnkVirtualFunc2();
        virtual void CArtsParam_UnkVirtualFunc3(u8 r4);
    };

    //size: 0x38
    struct _sArtsSet {
        union {
            struct {
                u16 unk0;
                u8 unk2[2];
                u8 unk4[0x30];
            };
            u16 mArtsSlotData[24];
        };

        _sArtsSet();
        virtual void _sArtsSet_UnkVirtualFunc1(){
            unk0 = 0;
            std::memset(unk4, 0, sizeof(unk4));
        }
    };

    //size: 0xD58
    class CArtsSet : _sArtsSet {
    public:
        CArtsSet(){}
        virtual void CArtsSet_UnkVirtualFunc1();

        void setArtsSlotRC(unsigned short value, unsigned int row, unsigned int index);
        unsigned short getArtsSlotRC(int index, int subindex);
        void setArtsSlotByIdx(unsigned short value, int index);
        void* getArtsParamRC(int index460, int index8c);
        void* getArtsParamRC2(int index1, int index2);
        void* getArtsParamByIdx(int index);

        //0x0: vtable
        //0x0-38: _sArtsSet
        CArtsParam mArtsParams[24]; //0x38
    };
};
/* end "kyoshin/cf/CArtsSet.hpp" */
/* "src/kyoshin/cf/object/CActorParam.hpp" line 7 "cstring" */
/* end "cstring" */

namespace cf {

    class UnkClass_CActorParam15E0;

    //size: 0x7C
    struct CActorParam_UnkStruct2 {
        u8 unk0[0x40];
        u16 unk40;
        u8 unk42[0x78 - 0x42];
        u32 unk78;
    };

    //might be fake?
    struct CActorParam_UnkStruct6 {
        CActorParam_UnkStruct6(){
            unk0 = 0;
        }
    
        u8 unk0;
    };

    struct CActorParam_Bitflags {
        CActorParam_Bitflags(){
            flags = 0;
        }

        u32 flags;
    };

    //size: 0xBC
    struct CActorParam_UnkStruct1 {
        CActorParam_UnkStruct1() {
            init();
        }

        void init(){
            unk0 = 0;
            unk4 = 0;
            unk48 = 0;
            unk4C = -1;
            unk50 = 0;
            unk54 = 0;
            unk58 = 0;
            unk5C = 0;
            unk60 = 0;
            unk64 = 0;
            unk7C = 0;
            unk80 = 0;
            unkB8 = 0;
            unk68 = 0;
            unk6C = 0;
            unk70 = 0;
            unk72 = 0;
            std::memset(unk8, 0, sizeof(unk8));
            std::memset(unk84, 0, sizeof(unk84));
            mFlagsArray[0].flags = 0;
            mFlagsArray[1].flags = 0;
        }

        u32 unk0;
        u32 unk4;
        u8 unk8[0x40];
        u32 unk48;
        int unk4C;
        CActorParam_UnkStruct2* unk50;
        float unk54;
        float unk58;
        float unk5C;
        float unk60;
        float unk64;
        float unk68;
        float unk6C;
        u16 unk70;
        u16 unk72;
        CActorParam_Bitflags mFlagsArray[2]; //0x74
        u32 unk7C;
        u16 unk80;
        u8 unk82[2];
        u8 unk84[0x34];
        u32 unkB8;

        enum Flags_74 {
            FLAG_BIT_0 = (1 << 0),
            FLAG_BIT_1 = (1 << 1),
            FLAG_BIT_2 = (1 << 2),
            FLAG_BIT_3 = (1 << 3),
            FLAG_BIT_4 = (1 << 4),
            FLAG_BIT_5 = (1 << 5),
            FLAG_BIT_6 = (1 << 6),
            FLAG_BIT_7 = (1 << 7),
            FLAG_BIT_8 = (1 << 8),
            FLAG_BIT_9 = (1 << 9),
            FLAG_BIT_10 = (1 << 10),
            FLAG_BIT_11 = (1 << 11),
            FLAG_BIT_12 = (1 << 12),
            FLAG_BIT_13 = (1 << 13),
            FLAG_BIT_14 = (1 << 14),
            FLAG_BIT_15 = (1 << 15),
            FLAG_BIT_16 = (1 << 16),
            FLAG_BIT_17 = (1 << 17),
            FLAG_BIT_18 = (1 << 18),
            FLAG_BIT_19 = (1 << 19),
            FLAG_BIT_20 = (1 << 20),
            FLAG_BIT_21 = (1 << 21),
            FLAG_BIT_22 = (1 << 22),
            FLAG_BIT_23 = (1 << 23),
            FLAG_BIT_24 = (1 << 24),
            FLAG_BIT_25 = (1 << 25),
            FLAG_BIT_26 = (1 << 26),
            FLAG_BIT_27 = (1 << 27),
            FLAG_BIT_28 = (1 << 28),
            FLAG_BIT_29 = (1 << 29),
            FLAG_BIT_30 = (1 << 30),
            FLAG_BIT_31 = (1 << 31),
        };
    };

    //size: 0x52
    struct CActorParam_UnkStruct4 {
        CActorParam_UnkStruct4() {
            std::memset(this, 0, sizeof(*this)); //wtf??
        }

        u8 unk0[0x4E];
        CActorParam_UnkStruct6 unk4E[4];
    };

    //TODO: related to above struct?
    //size: 0x78
    struct CActorParam_UnkStruct3 {
        CActorParam_UnkStruct3() {
            unk74 = 0;

            std::memset(this, 0, sizeof(*this)); //wtf??

            unk5C = 1.0f;
            unk38 = 5;
            unk3A = 5;
        }

        u8 unk0[0x38];
        u16 unk38;
        u16 unk3A;
        u8 unk3C[0x44 - 0x3C];
        float unk44;
        u8 unk48[4];
        float unk4C;
        u8 unk50[0x5C - 0x50];
        float unk5C;
        u8 unk60[0x70 - 0x60];
        CActorParam_UnkStruct6 unk70[4];
        u32 unk74;
    };

    //size: 0x18
    struct CActorParam_UnkStruct5 {
        CActorParam_UnkStruct5(){
            std::memset(this, 0, sizeof(*this)); //wtf??
        }

        void init(){
            unk14 = 0;
            unk4 = 0;
            unk0 = 0;
            unkC = 0;
            unk8 = 0;
            unk10 = 0;
        }

        float unk0;
        float unk4;
        float unk8;
        float unkC;
        float unk10;
        u32 unk14;
    };

    //size: 0x3384
    class CActorParam : public CActorState, public CBattleState, public CDebugState {
    public:
        CActorParam(UNKTYPE* r4, UNKTYPE* r5);
    #pragma region vtable
        virtual void CActorParam_UnkVirtualFunc1();   //0x98
        virtual void CActorParam_UnkVirtualFunc2();   //0x9C
        virtual void CActorParam_UnkVirtualFunc3();   //0xA0
        virtual void CActorParam_UnkVirtualFunc4();   //0xA4
        virtual void CActorParam_UnkVirtualFunc5();   //0xA8
        virtual void CActorParam_UnkVirtualFunc6();   //0xAC
        virtual void CActorParam_UnkVirtualFunc7();   //0xB0
        virtual void CActorParam_UnkVirtualFunc8();   //0xB4
        virtual void CActorParam_UnkVirtualFunc9();   //0xB8
        virtual void CActorParam_UnkVirtualFunc10();  //0xBC
        virtual void CActorParam_UnkVirtualFunc11();  //0xC0
        virtual void CActorParam_UnkVirtualFunc12();  //0xC4
        virtual void CActorParam_UnkVirtualFunc13();  //0xC8
        virtual void CActorParam_UnkVirtualFunc14(u8 val);  //0xCC
        virtual void CActorParam_UnkVirtualFunc15();  //0xD0
        virtual void CActorParam_UnkVirtualFunc16(float val);  //0xD4
        virtual void CActorParam_UnkVirtualFunc17();  //0xD8
        virtual void CActorParam_UnkVirtualFunc18();  //0xDC
        virtual u32 CActorParam_UnkVirtualFunc19();  //0xE0
        virtual void CActorParam_UnkVirtualFunc20();  //0xE4
        virtual void CActorParam_UnkVirtualFunc21();  //0xE8
        virtual void CActorParam_UnkVirtualFunc22();  //0xEC
        virtual void CActorParam_UnkVirtualFunc23();  //0xF0
        virtual void CActorParam_UnkVirtualFunc24();  //0xF4
        virtual void CActorParam_UnkVirtualFunc25();  //0xF8
        virtual u32 CActorParam_UnkVirtualFunc26();  //0xFC
        virtual void CActorParam_UnkVirtualFunc27();  //0x100
        virtual void CActorParam_UnkVirtualFunc28();  //0x104
        virtual u32 CActorParam_UnkVirtualFunc29();  //0x108
        virtual void CActorParam_UnkVirtualFunc30();  //0x10C
        virtual void CActorParam_UnkVirtualFunc31();  //0x110
        virtual void CActorParam_UnkVirtualFunc32();  //0x114
        virtual void CActorParam_UnkVirtualFunc33(float val);  //0x118
        virtual void CActorParam_UnkVirtualFunc34();  //0x11C
        virtual void CActorParam_UnkVirtualFunc35();  //0x120
        virtual void CActorParam_UnkVirtualFunc36();  //0x124
        virtual float CActorParam_UnkVirtualFunc37();  //0x128
        virtual void CActorParam_UnkVirtualFunc38();  //0x12C
        virtual void CActorParam_UnkVirtualFunc39();  //0x130
        virtual void CActorParam_UnkVirtualFunc40();  //0x134
        virtual void CActorParam_UnkVirtualFunc41();  //0x138
        virtual void CActorParam_UnkVirtualFunc42();  //0x13C
        virtual void CActorParam_UnkVirtualFunc43();  //0x140
        virtual void CActorParam_UnkVirtualFunc44();  //0x144
        virtual void CActorParam_UnkVirtualFunc45();  //0x148
        virtual void CActorParam_UnkVirtualFunc46();  //0x14C
        virtual void CActorParam_UnkVirtualFunc47();  //0x150
        virtual void CActorParam_UnkVirtualFunc48();  //0x154
        virtual void CActorParam_UnkVirtualFunc49();  //0x158
        virtual void CActorParam_UnkVirtualFunc50();  //0x15C
        virtual void CActorParam_UnkVirtualFunc51();  //0x160
        virtual void CActorParam_UnkVirtualFunc52();  //0x164
        virtual void CActorParam_UnkVirtualFunc53();  //0x168
        virtual void CActorParam_UnkVirtualFunc54();  //0x16C
        virtual void CActorParam_UnkVirtualFunc55(u16 val);  //0x170
        virtual void CActorParam_UnkVirtualFunc56();  //0x174
        virtual void CActorParam_UnkVirtualFunc57();  //0x178
        virtual void CActorParam_UnkVirtualFunc58();  //0x17C
        virtual void CActorParam_UnkVirtualFunc59();  //0x180
        virtual void CActorParam_UnkVirtualFunc60();  //0x184
        virtual void CActorParam_UnkVirtualFunc61(u16 val);  //0x188
        virtual void CActorParam_UnkVirtualFunc62();  //0x18C
        virtual void CActorParam_UnkVirtualFunc63();  //0x190
        virtual void CActorParam_UnkVirtualFunc64();  //0x194
        virtual void CActorParam_UnkVirtualFunc65(float val);  //0x198
        virtual void CActorParam_UnkVirtualFunc66();  //0x19C
        virtual void CActorParam_UnkVirtualFunc67();  //0x1A0
        virtual void CActorParam_UnkVirtualFunc68(float val);  //0x1A4
        virtual void CActorParam_UnkVirtualFunc69();  //0x1A8
        virtual void CActorParam_UnkVirtualFunc70();  //0x1AC
        virtual void CActorParam_UnkVirtualFunc71();  //0x1B0
        virtual void CActorParam_UnkVirtualFunc72();  //0x1B4
        virtual void CActorParam_UnkVirtualFunc73();  //0x1B8
        virtual void CActorParam_UnkVirtualFunc74(float val);  //0x1BC
        virtual void CActorParam_UnkVirtualFunc75();  //0x1C0
        virtual void* CActorParam_UnkVirtualFunc76();  //0x1C4
        virtual void CActorParam_UnkVirtualFunc77();  //0x1C8
        virtual void CActorParam_UnkVirtualFunc78();  //0x1CC
        virtual void CActorParam_UnkVirtualFunc79();  //0x1D0
        virtual void CActorParam_UnkVirtualFunc80();  //0x1D4
        virtual void CActorParam_UnkVirtualFunc81(u32 val);  //0x1D8
        virtual void CActorParam_UnkVirtualFunc82(u32 addend);  //0x1DC
        virtual void CActorParam_UnkVirtualFunc83(u32 addend);  //0x1E0
        virtual void CActorParam_UnkVirtualFunc84();  //0x1E4
        virtual u32 CActorParam_UnkVirtualFunc85();  //0x1E8
        virtual void CActorParam_UnkVirtualFunc86();  //0x1EC
        virtual void CActorParam_UnkVirtualFunc87();  //0x1F0
        virtual void CActorParam_UnkVirtualFunc88();  //0x1F4
        virtual void CActorParam_UnkVirtualFunc89();  //0x1F8
        virtual void CActorParam_UnkVirtualFunc90(u32 addend);  //0x1FC
        virtual void CActorParam_UnkVirtualFunc91();  //0x200
        virtual void CActorParam_UnkVirtualFunc92();  //0x204
        virtual void CActorParam_UnkVirtualFunc93();  //0x208
        virtual void* CActorParam_UnkVirtualFunc94();  //0x20C
        virtual void CActorParam_UnkVirtualFunc95();  //0x210
        virtual void CActorParam_UnkVirtualFunc96();  //0x214
        virtual void CActorParam_UnkVirtualFunc97();  //0x218
        virtual void CActorParam_UnkVirtualFunc98();  //0x21C
        virtual void CActorParam_UnkVirtualFunc99();  //0x220
        virtual void* CActorParam_UnkVirtualFunc100(); //0x224
        virtual void CActorParam_UnkVirtualFunc101(); //0x228
        virtual void CActorParam_UnkVirtualFunc102(); //0x22C
        virtual void CActorParam_UnkVirtualFunc103(); //0x230
        virtual void CActorParam_UnkVirtualFunc104(); //0x234
        virtual void CActorParam_UnkVirtualFunc105(); //0x238
        virtual void CActorParam_UnkVirtualFunc106(); //0x23C
        virtual void CActorParam_UnkVirtualFunc107(); //0x240
        virtual void CActorParam_UnkVirtualFunc108(); //0x244
        virtual void CActorParam_UnkVirtualFunc109(); //0x248
        virtual void CActorParam_UnkVirtualFunc110(); //0x24C
        virtual void CActorParam_UnkVirtualFunc111(); //0x250
        virtual void CActorParam_UnkVirtualFunc112(); //0x254
        virtual u32* CActorParam_UnkVirtualFunc113(); //0x258
        virtual void CActorParam_UnkVirtualFunc114(); //0x25C
        virtual bool CActorParam_UnkVirtualFunc115(); //0x260
        virtual void CActorParam_UnkVirtualFunc116(float val); //0x264
        virtual float* CActorParam_UnkVirtualFunc117(); //0x268
        virtual void CActorParam_UnkVirtualFunc118(); //0x26C
        virtual float* CActorParam_UnkVirtualFunc119(); //0x270
        virtual void CActorParam_UnkVirtualFunc120(); //0x274
        virtual void CActorParam_UnkVirtualFunc121(); //0x278
        virtual void* CActorParam_UnkVirtualFunc122(); //0x27C
        virtual void CActorParam_UnkVirtualFunc123(); //0x280
        virtual void CActorParam_UnkVirtualFunc124(); //0x284
        virtual void* CActorParam_UnkVirtualFunc125(); //0x288
        virtual void CActorParam_UnkVirtualFunc126(); //0x28C
        virtual UnkClass_CActorParam15E0* CActorParam_UnkVirtualFunc127(); //0x290
        virtual void CActorParam_UnkVirtualFunc128(); //0x294
        virtual CActorParam_UnkStruct1* CActorParam_UnkVirtualFunc129(); //0x298
        virtual void CActorParam_UnkVirtualFunc130(); //0x29C
        virtual void CActorParam_UnkVirtualFunc131(); //0x2A0
        virtual void* CActorParam_UnkVirtualFunc132(); //0x2A4
        virtual void CActorParam_UnkVirtualFunc133(); //0x2A8
        virtual void CActorParam_UnkVirtualFunc134(); //0x2AC
        virtual void CActorParam_UnkVirtualFunc135(); //0x2B0
        virtual void CActorParam_UnkVirtualFunc136(); //0x2B4
        virtual void CActorParam_UnkVirtualFunc137(); //0x2B8
        virtual bool CActorParam_UnkVirtualFunc138(); //0x2BC
        virtual void CActorParam_UnkVirtualFunc139(); //0x2C0
        virtual void CActorParam_UnkVirtualFunc140(); //0x2C4
        virtual void CActorParam_UnkVirtualFunc141(); //0x2C8
        virtual void CActorParam_UnkVirtualFunc142(); //0x2CC
        virtual void CActorParam_UnkVirtualFunc143(); //0x2D0
        virtual void CActorParam_UnkVirtualFunc144(); //0x2D4
        virtual void CActorParam_UnkVirtualFunc145(); //0x2D8
        virtual void CActorParam_UnkVirtualFunc146(); //0x2DC
        virtual void CActorParam_UnkVirtualFunc147(); //0x2E0
        virtual void CActorParam_UnkVirtualFunc148(); //0x2E4
        virtual void CActorParam_UnkVirtualFunc149(); //0x2E8
        virtual void CActorParam_UnkVirtualFunc150(); //0x2EC
        virtual void CActorParam_UnkVirtualFunc151(); //0x2F0
        virtual void* CActorParam_UnkVirtualFunc152(); //0x2F4
        virtual void CActorParam_UnkVirtualFunc153(); //0x2F8
        virtual void CActorParam_UnkVirtualFunc154(); //0x2FC
        virtual void CActorParam_UnkVirtualFunc155(); //0x300
        virtual void CActorParam_UnkVirtualFunc156(); //0x304
        virtual void CActorParam_UnkVirtualFunc157(); //0x308
        virtual void CActorParam_UnkVirtualFunc158(); //0x30C
        virtual void CActorParam_UnkVirtualFunc159(); //0x310
        virtual void CActorParam_UnkVirtualFunc160(); //0x314
        virtual void CActorParam_UnkVirtualFunc161(); //0x318
        virtual void CActorParam_UnkVirtualFunc162(); //0x31C
        virtual void CActorParam_UnkVirtualFunc163(); //0x320
        virtual void CActorParam_UnkVirtualFunc164(); //0x324
        virtual void* CActorParam_UnkVirtualFunc165(); //0x328
        virtual void CActorParam_UnkVirtualFunc166(); //0x32C
        virtual void CActorParam_UnkVirtualFunc167(); //0x330
        virtual void CActorParam_UnkVirtualFunc168(); //0x334
        virtual void CActorParam_UnkVirtualFunc169(); //0x338
        virtual void CActorParam_UnkVirtualFunc170(); //0x33C
        virtual void CActorParam_UnkVirtualFunc171(); //0x340
        virtual void CActorParam_UnkVirtualFunc172(); //0x344
        virtual void CActorParam_UnkVirtualFunc173(); //0x348
        virtual void CActorParam_UnkVirtualFunc174(); //0x34C
        virtual void CActorParam_UnkVirtualFunc175(); //0x350
        virtual void CActorParam_UnkVirtualFunc176(); //0x354
        virtual void CActorParam_UnkVirtualFunc177(); //0x358
        virtual void CActorParam_UnkVirtualFunc178(); //0x35C
        virtual void CActorParam_UnkVirtualFunc179(); //0x360
        virtual void CActorParam_UnkVirtualFunc180(); //0x364
        virtual void CActorParam_UnkVirtualFunc181(); //0x368
    #pragma endregion

        UNKTYPE* unk15DC;
        UnkClass_CActorParam15E0* unk15E0;
        u32 unk15E4;
        float unk15E8;
        u32 unk15EC;
        u32 unk15F0;
        u8 unk15F4[4];        // 0x15F4
        float unk15F8;         // 0x15F8
        float unk15FC;
        u32 unk1600;
        u32 unk1604;
        u32 unk1608;
        u16 unk160C;
        u16 unk160E;
        float unk1610;
        u16 unk1614;
        u16 unk1616;
        float unk1618;
        u32 unk161C;
        float unk1620;
        float unk1624;
        u8 unk1628;
        u8 unk1629;
        u8 unk162A;
        u8 unk162B;
        u8 unk162C;
        float unk1630;
        u32 unk1634;
        u32 unk1638;
        u32 unk163C;
        u32 unk1640;
        u32 unk1644;
        u16 unk1648;
        u16 unk164A;
        u16 unk164C;
        u8 unk164E[2];
        CActorParam_UnkStruct3 unk1650;
        CActorParam_UnkStruct3 unk16C8;
        CActorParam_UnkStruct4 unk1740;
        CActorParam_UnkStruct4 unk1792;
        CActorParam_UnkStruct3 unk17E4;
        CActorParam_UnkStruct3 unk185C;
        CActorParam_UnkStruct4 unk18D4;
        u8 unk1926[2]; //filler?
        CActorParam_UnkStruct5 unk1928[8];
        CArtsSet mArtsSet; //0x19E8
        u8 unk2740[0xC];
        CAttackSet mAttackSet; //0x274C
        u32 unk2A80; //probably not here
        CActorParam_UnkStruct1 unk2A84[10];
        CActorParam_UnkStruct1 unk31DC;
        CActorParam_UnkStruct1 unk3298;
        u8 unk3354;
        u8 unk3355[3]; //padding?
        u16 unk3358;
        u16 unk335A;
        u8 unk335C[5];
        u8 unk3361[3]; //padding?
        float unk3364;
        float unk3368;
        u32 unk336C;
        u32 unk3370;
        u32 unk3374;
        u8 unk3378[4];
        float unk337C;
    CActorParam();
    void CBattleState_UnkVirtualFunc18();
    void CBattleState_UnkVirtualFunc17();
    int CBattleState_UnkVirtualFunc3();
    void CBattleState_UnkVirtualFunc2();
    };

inline u32 cf::CActorParam::CActorParam_UnkVirtualFunc19() { return unk15EC; }
inline u32 cf::CActorParam::CActorParam_UnkVirtualFunc29() { return *(u32*)&unk17E4; }
inline u32* cf::CActorParam::CActorParam_UnkVirtualFunc113() { return &unk161C; }
inline bool cf::CActorParam::CActorParam_UnkVirtualFunc115() { return !!unk1628; }
inline float* cf::CActorParam::CActorParam_UnkVirtualFunc117() { return &unk1620; }
inline float* cf::CActorParam::CActorParam_UnkVirtualFunc119() { return &unk1624; }
inline void* cf::CActorParam::CActorParam_UnkVirtualFunc122() { return &mArtsSet; }
}
/* end "kyoshin/cf/object/CActorParam.hpp" */
/* "src/kyoshin/plugin/ocUnit.hpp" line 12 "kyoshin/cf/object/CObjectParam.hpp" */
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
        virtual void CObjectState_UnkVirtualFunc1(u32 bits);  //0x8
        virtual int CObjectState_UnkVirtualFunc2(int mask);  //0xC
        virtual void CObjectState_UnkVirtualFunc3(u32 mask);  //0x10
        virtual void CObjectState_UnkVirtualFunc4();  //0x14
        virtual void CObjectState_UnkVirtualFunc5(int arg);  //0x18
        virtual void CObjectState_UnkVirtualFunc6();  //0x1C
        virtual void CObjectState_UnkVirtualFunc7(int arg);  //0x20
        virtual int CObjectState_UnkVirtualFunc8(int arg);  //0x24
        virtual void CObjectState_UnkVirtualFunc9();  //0x28
        virtual int CObjectState_UnkVirtualFunc10(void* arg, int arg2); //0x2C
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
        virtual void CObjectParam_UnkVirtualFunc1(u32 a, u8 b); //0x3C
        virtual void* CObjectParam_UnkVirtualFunc2(); //0x40
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
/* "src/kyoshin/plugin/ocUnit.hpp" line 13 "kyoshin/cf/object/CfObject.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObject.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObject.hpp" line 3 "kyoshin/cf/object/CObjectParam.hpp" */
/* end "kyoshin/cf/object/CObjectParam.hpp" */

namespace cf {
    class UnkClass_80082D90;
    struct CfObjectSub54 {
        u8 field_0x0[0xC];
    };

    //min size: 0x70
    class CfObject : public CObjectParam {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObject();                      //0x54
        virtual void CfObject_UnkVirtualFunc2() = 0;  //0x58
        virtual void CfObject_UnkVirtualFunc3(UnkClass_80082D90* data); //0x5C
        virtual void CfObject_UnkVirtualFunc4() = 0;  //0x60
        virtual void CfObject_UnkVirtualFunc5();      //0x64
        virtual void CfObject_UnkVirtualFunc6();      //0x68
        virtual void CfObject_UnkVirtualFunc7() = 0;  //0x6C
        virtual void CfObject_UnkVirtualFunc8() = 0;  //0x70
        virtual bool CfObject_UnkVirtualFunc9();      //0x74
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
        virtual u32 CfObject_UnkVirtualFunc23();      //0xAC
        virtual void CfObject_UnkVirtualFunc24();     //0xB0
        virtual void CfObject_UnkVirtualFunc25();     //0xB4
        virtual void CfObject_UnkVirtualFunc26(u32 value, float amount); //0xB8
        virtual void CfObject_UnkVirtualFunc27();     //0xBC
        virtual void CfObject_UnkVirtualFunc28();     //0xC0
        virtual void CfObject_UnkVirtualFunc29(float value);     //0xC4
        virtual void CfObject_UnkVirtualFunc30();     //0xC8
        virtual float CfObject_UnkVirtualFunc31();     //0xCC
        virtual void CfObject_UnkVirtualFunc32();     //0xD0
        virtual void CfObject_UnkVirtualFunc33(float amount); //0xD4
        virtual float CfObject_UnkVirtualFunc34();     //0xD8
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
        virtual int CfObject_UnkVirtualFunc50();     //0x118
        virtual int CfObject_UnkVirtualFunc51();     //0x11C
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
        virtual void CfObject_UnkVirtualFunc66(int) = 0; //0x158
        virtual void CfObject_UnkVirtualFunc67();     //0x15C
        virtual void CfObject_UnkVirtualFunc68() = 0; //0x160
        virtual void CfObject_UnkVirtualFunc69();     //0x164
        virtual void CfObject_UnkVirtualFunc70(float value); //0x168
        virtual void CfObject_UnkVirtualFunc71();     //0x16C
        virtual void CfObject_UnkVirtualFunc72();     //0x170
        virtual void CfObject_UnkVirtualFunc73();     //0x174

        //not sure if belongs here? (can be in any class from CObjectState to CfObjectMove)
        void func_800BE898(int, u32, float, float);


        //0x0: vtable
        // CObjectParam currently ends at 0x28.
        u8 field_0x28[0x10];
        void* mSubObj38;          // 0x38-0x3B
        float mPos3C;           // 0x3C-0x3F
        float mPos40;           // 0x40-0x43
        float mPos44;           // 0x44-0x47
        u8 _pad48[0x4C - 0x48];   // 0x48-0x4B
        float mField4C;           // 0x4C-0x4F
        u8 _pad50[0x54 - 0x50];   // 0x50-0x53
        CfObjectSub54 mSubObj54;     // 0x54-0x5F
        float mFloat60;            // 0x60-0x63
        u32 unk64;                  // 0x64-0x67
        u32 mFlags68;               // 0x68-0x6B
        u8 _pad6C[0x70 - 0x6C];    // 0x6C-0x6F
    };
}
/* end "kyoshin/cf/object/CfObject.hpp" */
/* "src/kyoshin/plugin/ocUnit.hpp" line 14 "kyoshin/cf/object/CfObjectMove.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObjectMove.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObjectMove.hpp" line 3 "kyoshin/cf/object/CfObjectModel.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObjectModel.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObjectModel.hpp" line 3 "kyoshin/cf/object/CfObject.hpp" */
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
    u32 CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27();
    void CfObject_UnkVirtualFunc29();
    void CfObject_UnkVirtualFunc32();
    float CfObject_UnkVirtualFunc34();
    void CfObject_UnkVirtualFunc33(float amount);
    void CfObject_UnkVirtualFunc30();
    float CfObject_UnkVirtualFunc56();
    void CfObject_UnkVirtualFunc52();
    CfObject* CfObject_UnkVirtualFunc53();
    void CfObject_UnkVirtualFunc54();
    void CfObject_UnkVirtualFunc55();
    void* CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc66(int);
    void CfObject_UnkVirtualFunc67();
    void CfObject_UnkVirtualFunc70(float value);
    void CfObject_UnkVirtualFunc69();
    void CfObject_UnkVirtualFunc68();
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc28();
    float CfObject_UnkVirtualFunc31();
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
        u8 _6CA[4];             // 0x6CA-0x6CD
        u8 field_6CE;            // 0x6CE
        u8 field_6CF;            // 0x6CF
        u8 _6D0[0x20];           // 0x6D0-0x6EF
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
    void CfObject_UnkVirtualFunc26(u32 value, float amount);
    u32 CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27();
    void CfObject_UnkVirtualFunc30();
    void CfObject_UnkVirtualFunc32();
    void CfObject_UnkVirtualFunc33(float amount);
    void CfObject_UnkVirtualFunc13();
    void CfObject_UnkVirtualFunc57();
    void* CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc14();
    void CfObject_UnkVirtualFunc15();
    void CfObject_UnkVirtualFunc16();
    void CfObject_UnkVirtualFunc17();
    void CfObjectModel_UnkVirtualFunc18();
    bool CfObject_UnkVirtualFunc9();
    void CfObject_UnkVirtualFunc10();
    void CfObject_UnkVirtualFunc61();
    void CfObject_UnkVirtualFunc62();
    void CfObject_UnkVirtualFunc12();
    void CfObject_UnkVirtualFunc66(int);
    void CfObjectModel_UnkVirtualFunc19();
    void CfObjectModel_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc37();
    void CfObject_UnkVirtualFunc38();
    void CfObject_UnkVirtualFunc39();
    void CfObject_UnkVirtualFunc40();
    void CfObject_UnkVirtualFunc42();
    void CfObject_UnkVirtualFunc43();
    void CfObject_UnkVirtualFunc45();
    void CfObject_UnkVirtualFunc70(float value);
    int CfObject_UnkVirtualFunc50();
    int CfObject_UnkVirtualFunc51();
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
/* "src/kyoshin/plugin/ocUnit.hpp" line 15 "kyoshin/cf/CfGameManager.hpp" */
#pragma once

/* "include/kyoshin/cf/CfGameManager.hpp" line 2 "types.h" */
/* end "types.h" */

class CPad;
class CScnNw4r;
class CView;
class UnkClass_80186D20;
class CfCamEventManager;
class UnkClass_800821F8;
class UnkClass_80085334;
class UnkClass_8007E864;
class CSysWinBuff;

struct CfGameManagerData1C {
    u8 field_0x0[0xC];
};

struct UnkClass_80083298SubF0 {
    u8 field_0x0;
};

class UnkClass_80083298 {
public:
    virtual void vfunc_0x08();
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10();
    virtual void vfunc_0x14();
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20();
    virtual void vfunc_0x24();
    virtual void vfunc_0x28();
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40();
    virtual void vfunc_0x44();
    virtual void vfunc_0x48();
    virtual void vfunc_0x4C();
    virtual void vfunc_0x50();
    virtual void vfunc_0x54();
    virtual void vfunc_0x58();
    virtual void vfunc_0x5C();
    virtual void vfunc_0x60();
    virtual void vfunc_0x64();
    virtual void vfunc_0x68();
    virtual void vfunc_0x6C();
    virtual void vfunc_0x70();
    virtual u32 vfunc_0x74();

    u8 field_0x4[0xEC];
    UnkClass_80083298SubF0 field_0xF0;
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
        static UnkClass_80083298* func_80083298();
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
        u8 unk29[0x40 - 0x29];
        u16* field_0x40;
        s32 field_0x44;
        s32 field_0x48;
        s32 field_0x4C;
        u8 field_0x50[0x18];
        u32 unk68;
        u8 unk6C;
        u8 unk6D[3];          // 0x6D-0x6F
        u32 unk70;             // 0x70-0x73
        u8 unk74[8];           // 0x74-0x7B
        u32 unk7C;
        u8 unk80[0x86 - 0x80];
        u16 field_0x86;
        u8 field_0x88[0x8C - 0x88];
        s32 unk8C;
        UnkClass_80083298* unk90;
        //between CObjectParam - CfObjectMove
        //likely player character object array, seems to always store pointers
        //to CfObjectPc objects except pointing at the 4th vtable
        CfObjectMove* unk94[3];
        u32 unkA0;
        UnkClass_80186D20* field_0xA4;
        UnkClass_8007E864* unkA8;
        UnkClass_80085334* unkAC;
        UnkClass_800821F8* unkB0;
        CfCamEventManager* unkB4;
        u8 field_0xB8[0x4EC - 0xB8];
        u32 field_0x4EC;

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
    ~CfGameManager();
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
    void* func_8007DA00();
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
    u32 func_8007EEF8();
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
    u16 func_8007F8B8();
    void func_8007F8C0();
    void** func_8007F8D0();
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
    u8 func_80080E20();
    u8 func_80080E28();
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
    bool func_80082104();
    void func_8008212C();
    UnkClass_800821F8* func_800821F8();
    CfObject* func_8008221C();
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
    u16 func_80082770();
    void func_800827A8();
    void func_800827E4();
    void func_80082834();
    u32 func_800828DC();
    u32 func_80082900();
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
    u32 func_80083100();
    u32 func_8008310C();
    void func_80083118();
    void func_80083284();
    void func_80083290();
    void func_800832BC();
    void func_80083304();
    void func_80083328();
    u32 func_80083458();
    u32 func_80083460();
    void func_80083468(u32 value);
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
    bool func_80084B68();
    bool func_80084BAC();
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
    static u32 func_80086DA0();
    void func_80086DA4();
    void func_80086DA8();
    void func_80086DAC();
    void func_80086DB0();
    void func_80086DB4();
    static u32 func_80086DBC();
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

#ifdef __cplusplus
extern "C" {
#endif

void ocUnitRegist();
void func_8013D07C(void* obj, const char* str, int mode);
void func_8013D448(void* obj, const char* str);
unsigned int func_8013EC58();
int func_80174C98(void* player, int* val, int mode);
u8 code80135FDC_getByte_64058();
unsigned int func_8013EC58();

#ifdef __cplusplus
}
#endif



/* end "kyoshin/plugin/ocUnit.hpp" */

// C-linkage retail symbols referenced by the plugin functions below.
extern "C" {
    extern u32 lbl_eu_80663E24;
    extern char lbl_eu_804FA74C[];
    extern void* lbl_eu_806618D8;
    extern void* lbl_eu_806618F0;
    extern void* lbl_eu_806618E8;

    // VM/script helpers
    void func_800BE12C(void* obj, int a, int b, int c, int d);
    void func_800BE824(void* obj, int flag);
    void func_800BE0F8(void* obj, int target);
    void func_800BE33C(void* obj, int flag);
    void* func_800BBC0C();
    void func_800BF314(void* obj, int flag);
    void func_800F38E0(void* battleMgr, void* actor, int flag);
    void func_800F3958(void* battleMgr, void* actor, int index);
    void func_800EC8FC(void* battleMgr, void* actor, void* data, int flag);
    u32 func_800FE68C();
    void func_800ACC14(void* obj, s8 val);
    void func_800ACF78(void* obj, void* target, void* child);
    void func_800ACFD8(void* obj, void* target);
    void func_800AC4A8(void* obj, u16 param);
    void func_800ABF24(void* obj, void* pos, void* offset, float f);
    void func_8013D07C(void* subObj, const char* str, int flag);
    void func_8013D448(void* subObj, const char* str);
    unsigned int func_8013EC58();
    int func_80174C98(void* player, int* val, int flag);
    unsigned char code80135FDC_getByte_64058();
    void* __dynamic_cast(void* obj, int offset, void* rtti, void* targetRtti, int flag);
    int strcmp(const char* s1, const char* s2);
    void* memset(void* ptr, int val, u32 size);
    void* getInstance__Q22cf14CBattleManagerFv();
    void* getInstance__Q22cf13CfGameManagerFv();
    void* getPlayer__Q22cf13CfGameManagerFi(int i);
    void* func_80081694__Q22cf13CfGameManagerFv(unsigned short a, unsigned short b);
    void* func_80081CBC__Q22cf13CfGameManagerFv(const char* name, unsigned short param);
    void* func_80081CB8__Q22cf13CfGameManagerFv();
    void* func_800817BC__Q22cf13CfGameManagerFv(int index, int mode);
    void* func_8008187C__Q22cf13CfGameManagerFv(int index);
}

void func_8003BC10(){}

int cf::CfObjectMove::CfObject_UnkVirtualFunc50() { return static_cast<cf::CfObjectMove*>(this)->field_6CE; }

int cf::CfObjectMove::CfObject_UnkVirtualFunc51() { return static_cast<cf::CfObjectMove*>(this)->field_6CF; }

void func_8003BD7C(){}

void func_8003C044(){}

u32 cf::CfObject::CfObject_UnkVirtualFunc23() { return (u32)&mPos3C; }

void func_8003C0D0(){}

void func_8003C154(){}

void func_8003C1D8(){}

float cf::CfObject::CfObject_UnkVirtualFunc34() { return mField4C * 0.019592438f; }

void func_8003C260(){}

void* cf::CObjectParam::CObjectParam_UnkVirtualFunc2() { return mPtr10; }

void func_8003C2F4(){}

void func_8003C354(){}

void func_8003C3D0(){}

float cf::CActorParam::CActorParam_UnkVirtualFunc37() { return *(float*)((u8*)this + 0x17E8); }

void func_8003C480(){}

extern "C" void CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

void func_8003C560(){}

void func_8003C624(){}

void func_8003C6E8(){}

void cf::CfObject::CfObject_UnkVirtualFunc30() {
    void** vtable = *(void***)this;
    void (*func)(void*) = (void (*)(void*))vtable[0xC4 / 4];
    func(this);
}

void cf::CfObject::CfObject_UnkVirtualFunc29(float value) { mField4C = value; }

void func_8003C78C(){}

void cf::CObjectParam::CObjectParam_UnkVirtualFunc1(u32, u8) {}

bool isValid() { return false; }

void func_8003C84C(){}

void cf::CfObject::CfObject_UnkVirtualFunc26(u32 value, float amount) {}

void CfObject_UnkVirtualFunc19__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

void dispOn(){}

void dispOff(){}

int cf::CObjectState::CObjectState_UnkVirtualFunc8(int) { return 0; }

int CObjectState_UnkVirtualFunc2__Q22cf12CObjectStateFv(void* self, int mask) {
    return (*(int*)((char*)self + 4) & mask) != 0 ? 1 : 0;
}

void func_8003CB70(){}

void func_8003CC0C(){}

void func_8003CC9C(){}

void CfObject_UnkVirtualFunc49__Q22cf8CfObjectFv() {}

void func_8003CD6C(){}

int cf::CfObject::CfObject_UnkVirtualFunc50() { return -1; }

void func_8003CDE0(){}

void walkR(){}

void func_8003CED0(){}

void func_8003CF48(){}

void func_8003D060(){}

void func_8003D2B8(){}

void func_8003D32C(){}

void moveTo(){}

void func_8003D570(){}

void func_8003D5DC(){}

void isTalk(){}

void cf::CObjectState::CObjectState_UnkVirtualFunc11() { }

void onEvent(){}

int cf::CObjectState::CObjectState_UnkVirtualFunc10(void* arg, int arg2) { return 0; }

void cf::CObjectState::CObjectState_UnkVirtualFunc9() {}

// --- Batch 2 targets start here ---

// us-8003df40: func_8003D9C4
// Checks if a CfObject supports a talk/hybridheal flag, returns 1 or 2
extern "C" int func_8003D9C4(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (!obj) {
        u8 retVal = 2;
        vmRetValSet(pThread, (VMArg*)&retVal);
        return 0;
    }
    if (obj->unk64 & 0x10) {
        u8 retVal;
        if (((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc10((void*)0x2000, 0)) {
            retVal = 1;
        } else {
            retVal = 2;
        }
        vmRetValSet(pThread, (VMArg*)&retVal);
        return 1;
    } else {
        u8 retVal = 2;
        vmRetValSet(pThread, (VMArg*)&retVal);
        vmOCExceptionThrow(pThread);
        return 0;
    }
}

// us-8003dfec: winTalk
// Starts a talk window interaction
extern "C" int winTalk(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg1);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    cf::CfObjectMove* player = (cf::CfObjectMove*)cf::CfGameManager::getPlayer(0);
    if (player) {
        player = (cf::CfObjectMove*)((u8*)player - 0x3E9C);
    }
    func_8013D07C(obj->mSubObj38, str, 1);
    if (obj->unk64 & 0x10) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc10((void*)1, 0)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
        if (!code80135FDC_getByte_64058()) {
            u32* valPtr = (u32*)((cf::CObjectState*)player->mTargetC4)->CObjectState_UnkVirtualFunc8(0);
            u32 val = *valPtr;
            if (!func_80174C98((void*)player, (int*)&val, 1)) {
                cf::CObjectState* state = (cf::CObjectState*)((u8*)player + 0x3E9C);
                state->CObjectState_UnkVirtualFunc1(1);
                ((void(*)(void*, void*))(*(void***)((u8*)player + 0x3E9C))[0x50/4])((u8*)player + 0x3E9C, obj->mSubObj38);
            }
        }
    } else if ((obj->unk64 & 0x4000) || (obj->unk64 & 0x8000)) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc8(1)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
        u32* valPtr = (u32*)((cf::CObjectState*)player->mTargetC4)->CObjectState_UnkVirtualFunc8(0);
        u32 val = *valPtr;
        if (!func_80174C98((void*)player, (int*)&val, 1)) {
            cf::CObjectState* state = (cf::CObjectState*)((u8*)player + 0x3E9C);
            state->CObjectState_UnkVirtualFunc1(1);
            ((void(*)(void*, void*))(*(void***)((u8*)player + 0x3E9C))[0x50/4])((u8*)player + 0x3E9C, obj->mSubObj38);
        }
    }
    return 0;
}

// us-8003e1f8: func_8003DC7C
// Sets or clears a CfObject flag based on bool arg
extern "C" int func_8003DC7C(VMThread* pThread, int handle) {
    int boolVal;
    if (vmArgOmitChk(pThread, 1)) {
        boolVal = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        boolVal = vmArgBoolGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj && (obj->unk64 & 0x10)) {
        if (boolVal) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(0x10);
        } else {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc3(0x10);
        }
    }
    return 0;
}

// us-8003e2c0: func_8003DD44
// Gets a string arg and calls func_8013D07C on the object's sub-field
extern "C" int func_8003DD44(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg1);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_8013D07C(obj->mSubObj38, str, 1);
    return 0;
}

// us-8003e328: func_8003DDAC
// Returns the current object handle (OC search result)
extern "C" int func_8003DDAC(VMThread* pThread, int handle) {
    u8 type = 3;
    u32 ocHandle = func_8013EC58();
    VMArg retVal;
    retVal.type = type;
    retVal.value.uintVal = ocHandle;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003e370: func_8003DDF4
// Gets a string and calls a func, then handles notification flags
extern "C" int func_8003DDF4(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    const char* str = vmArgStringGet(2, arg1);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_8013D448(obj->mSubObj38, str);
    if (obj->unk64 & 0x10) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc10((void*)1, 0)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
    } else if ((obj->unk64 & 0x4000) || (obj->unk64 & 0x8000)) {
        if (!((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc8(1)) {
            ((cf::CObjectState*)obj)->CObjectState_UnkVirtualFunc1(1);
        }
    }
    return 0;
}

// us-8003e46c: setAct
// Sets an action on an object: action ID and optional fixed-point parameter
extern "C" int setAct(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    int actionId = vmArgIntGet(2, arg1);
    int fixedParam;
    if (vmArgOmitChk(pThread, 2)) {
        fixedParam = 1;
    } else {
        VMArg* arg2 = vmArgPtrGet(pThread, 2);
        fixedParam = vmArgFixedGet(3, arg2);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_800BE12C(obj, actionId, 0, -1, 1);
    if (fixedParam != 1) {
        float f = (float)(s32)fixedParam / 2048.0f;
        ((void(*)(void*, float))(*(void***)obj)[0x88/4])(obj, f);
    }
    return 0;
}

// us-8003e560: func_8003DFE4
// Calls a virtual function with a fixed-point parameter converted to float
extern "C" int func_8003DFE4(VMThread* pThread, int handle) {
    VMArg* arg1 = vmArgPtrGet(pThread, 1);
    int fixedVal = vmArgFixedGet(2, arg1);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float f = (float)(s32)fixedVal / 2048.0f;
    ((void(*)(void*, float))(*(void***)obj)[0x88/4])(obj, f);
    return 0;
}

// us-8003e5ec: lookAt
// Makes an object look at a target, with optional snap/rotate flags
extern "C" int lookAt(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    int snap;
    if (vmArgOmitChk(pThread, 2)) {
        snap = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 2);
        snap = vmArgBoolGet(3, ptr);
    }
    int rotate;
    if (vmArgOmitChk(pThread, 3)) {
        rotate = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 3);
        rotate = vmArgBoolGet(4, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* self = (cf::CfObject*)func_801864DC(ctx, handle);
    if (targetOC) {
        ctx = func_801862C0(pThread);
        cf::CfObject* target = (cf::CfObject*)func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
        void* actor = (void*)__dynamic_cast(target, 0, (void*)lbl_eu_806618D8, (void*)lbl_eu_806618F0, 0);
        if (actor) {
            if (((cf::CfObject*)actor)->unk64 & 0x40000000 || ((cf::CfObject*)actor)->unk64 & 0x10) {
                if (*(void**)((u8*)self + 0xC4)) {
                    if (((int(*)(void*, const char*))(*(void***)actor)[0x120/4])(actor, &lbl_eu_804FA74C[0x3C])) {
                        if (snap) {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x3C]);
                        } else {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x3C]);
                        }
                    } else if (((int(*)(void*, const char*))(*(void***)actor)[0x120/4])(actor, &lbl_eu_804FA74C[0x43])) {
                        if (snap) {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x43]);
                        } else {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x43]);
                        }
                    } else if (((int(*)(void*, const char*))(*(void***)actor)[0x120/4])(actor, &lbl_eu_804FA74C[0x4B])) {
                        if (snap) {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                        } else {
                            ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                        }
                    }
                }
            } else {
                if (snap) {
                    ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                } else {
                    ((void(*)(void*, void*, const char*))(*(void***)self)[0x1AC/4])(self, actor, &lbl_eu_804FA74C[0x4B]);
                }
            }
        } else {
            void* pos = ((void*(*)(void*))(*(void***)target)[0xBC/4])(target);
            ((void(*)(void*, void*))(*(void***)self)[0x1A8/4])(self, pos);
        }
        ((void(*)(void*, int))(*(void***)self)[0x1B4/4])(self, rotate ? 1 : 0);
    } else {
        ((void(*)(void*, void*, const char*))(*(void***)self)[0x1B0/4])(self, 0, &lbl_eu_804FA74C[0x3C]);
    }
    return 0;
}

// us-8003e94c: turn
// Turns an object by a fixed-point angle
extern "C" int turn(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int angle = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float f = (float)(s32)angle / 2048.0f * 0.0000958738f;
    ((void(*)(void*, float))(*(void***)obj)[0xCC/4])(obj, f);
    func_800BE12C(obj, 3, 0, -1, 1);
    return 0;
}

// us-8003e9f4: func_8003E478
// Sets a flag on the object if it's alive
extern "C" int func_8003E478(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj && !(obj->unk64 & 1)) {
        obj->mFlags68 |= 0x00080000;
    }
    return 0;
}

// us-8003ea4c: func_8003E4D0
// Sets a flag on the object if it's alive
extern "C" int func_8003E4D0(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj && !(obj->unk64 & 1)) {
        obj->mFlags68 |= 0x00100000;
    }
    return 0;
}

// us-8003eaa4: func_8003E528
// Gets the current OC handle for the object's party member
extern "C" int func_8003E528(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 ocHandle = obj->CObjectParam_UnkVirtualFunc5();
    if (!ocHandle) {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        if (obj == (cf::CfObject*)player) {
            ocHandle = func_800FE68C();
            ocHandle = *(u32*)((u8*)ocHandle + 0x10000 - 0x6F1C);
        }
    }
    if (ocHandle) {
        ctx = func_801862C0(pThread);
        cf::CfObject* target = (cf::CfObject*)func_801864DC(ctx, ocHandle);
        const char* typeName;
        if (target->unk64 & 0x01000000) {
            typeName = &lbl_eu_804FA74C[0x50];
        } else if (target->unk64 & 0x4000) {
            typeName = &lbl_eu_804FA74C[0x54];
        } else if (target->unk64 & 0x04000000) {
            typeName = &lbl_eu_804FA74C[0x5A];
        } else if (target->unk64 & 0x8000) {
            typeName = &lbl_eu_804FA74C[0x61];
        } else {
            typeName = &lbl_eu_804FA74C[0x66];
        }
        u8 type = 9;
        u16 ocId = vmOCSearch(typeName);
        VMArg retVal;
        retVal.type = type;
        retVal.unk2 = ocId;
        retVal.value.pointerVal = target->mSubObj38;
        vmRetValSet(pThread, &retVal);
        return 1;
    } else {
        vmOCExceptionThrow(pThread);
        return 0;
    }
}

// us-8003ebe8: func_8003E66C
// Gets the current battle target's OC handle
extern "C" int func_8003E66C(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u32 ocHandle = obj->CObjectParam_UnkVirtualFunc5();
    if (ocHandle) {
        ctx = func_801862C0(pThread);
        obj = (cf::CfObject*)func_801864DC(ctx, ocHandle);
        u8 type = 3;
        u16 hp = *(u16*)((u8*)obj + 0x8C);
        VMArg retVal;
        retVal.type = type;
        retVal.value.uintVal = hp;
        vmRetValSet(pThread, &retVal);
    } else {
        VMArg retVal;
        retVal.type = 0;
        vmRetValSet(pThread, &retVal);
    }
    return 1;
}

// us-8003ec88: isPC
// Checks if the object is a player character
extern "C" int isPC(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    int isPC = (obj->unk64 >> 30) & 1;
    u8 retVal = isPC ? 2 : 1;
    vmRetValSet(pThread, (VMArg*)&retVal);
    return 1;
}

// us-8003ecec: isNPC
// Checks if the object is an NPC
extern "C" int isNPC(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    int isNPC = (obj->unk64 >> 28) & 1;
    u8 retVal = isNPC ? 2 : 1;
    vmRetValSet(pThread, (VMArg*)&retVal);
    return 1;
}

// us-8003ed50: isENE
// Checks if the object is an enemy
extern "C" int isENE(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    int isENE = (obj->unk64 >> 29) & 1;
    u8 retVal = isENE ? 2 : 1;
    vmRetValSet(pThread, (VMArg*)&retVal);
    return 1;
}

// us-8003edb4: isPT
// Checks if the object is a party member
extern "C" int isPT(VMThread* pThread, int handle) {
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    u8 retVal;
    if ((obj->unk64 & 0x4000) || (obj->unk64 & 0x8000)) {
        retVal = 1;
    } else {
        retVal = 2;
    }
    vmRetValSet(pThread, (VMArg*)&retVal);
    return 1;
}

// us-8003ee2c: invin
// Toggles invincibility on a CfObjectActor
extern "C" int invin(VMThread* pThread, int handle) {
    int enable;
    if (vmArgOmitChk(pThread, 1)) {
        enable = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        enable = vmArgBoolGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)lbl_eu_806618D8, (void*)lbl_eu_806618F0, 0);
    if (actor) {
        void* battleMgr = getInstance__Q22cf14CBattleManagerFv();
        if (enable) {
            func_800F38E0(battleMgr, actor, 1);
        } else {
            func_800F3958(battleMgr, actor, 1);
        }
    }
    return 0;
}

// us-8003eef0: func_8003E974
// Complex battle AI function - sets up a battle event with ID, type, etc.
extern "C" int func_8003E974(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int eventId = vmArgIntGet(2, ptr);
    int arg2;
    if (vmArgOmitChk(pThread, 2)) {
        arg2 = 0;
    } else {
        VMArg* ptr2 = vmArgPtrGet(pThread, 2);
        arg2 = vmArgIntGet(3, ptr2);
    }
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    int arg4;
    if (vmArgOmitChk(pThread, 4)) {
        arg4 = 0;
    } else {
        VMArg* ptr4 = vmArgPtrGet(pThread, 4);
        arg4 = vmArgIntGet(5, ptr4);
    }
    int arg5;
    if (vmArgOmitChk(pThread, 5)) {
        arg5 = 0;
    } else {
        VMArg* ptr5 = vmArgPtrGet(pThread, 5);
        arg5 = vmArgIntGet(6, ptr5);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)lbl_eu_806618D8, (void*)lbl_eu_806618F0, 0);
    if (actor) {
        u8 data[0x34];
        memset(data, 0, sizeof(data));
        float f1 = (float)(s32)arg2 / 2048.0f;
        float f2 = (float)(s32)arg5 / 2048.0f;
        *(u32*)&data[0] = *(u32*)((u8*)actor + 0x3F10);
        *(u32*)&data[4] = 0;
        *(u16*)&data[0x14] = (u16)eventId;
        *(u32*)&data[0x18] = arg3;
        *(u16*)&data[0x1C] = (u16)arg4;
        *(float*)&data[0x28] = f1;
        *(float*)&data[0x2C] = f2;
        void* battleMgr = getInstance__Q22cf14CBattleManagerFv();
        func_800EC8FC(battleMgr, actor, data, 0);
    }
    return 0;
}

// us-8003f0e0: func_8003EB64
// Removes buff/debuff from an actor by index
extern "C" int func_8003EB64(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int index = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)lbl_eu_806618D8, (void*)lbl_eu_806618F0, 0);
    if (actor) {
        void* battleMgr = getInstance__Q22cf14CBattleManagerFv();
        func_800F3958(battleMgr, actor, index);
    }
    return 0;
}

// us-8003f168: setColi
// Enables/disables collision on an actor
extern "C" int setColi(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* actor = (void*)__dynamic_cast(obj, 0, (void*)lbl_eu_806618D8, (void*)lbl_eu_806618F0, 0);
    if (actor) {
        func_800BE824(actor, enable ? 1 : 0);
    }
    return 0;
}

// us-8003f1ec: setEye
// Sets the eye/attention target
extern "C" int setEye(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int target = vmArgIntGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        func_800BE0F8(obj, target);
    }
    return 0;
}

// us-8003f254: func_8003ECD8
// Enables/disables gravity on an object
extern "C" int func_8003ECD8(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        ((void(*)(void*, int))(*(void***)obj)[0x150/4])(obj, enable ? 0 : 1);
    }
    return 0;
}

// us-8003f2cc: func_8003ED50
// Enables/disables a visibility flag on an object and its model
extern "C" int func_8003ED50(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        if (enable) {
            obj->mFlags68 |= 0x8000;
        } else {
            obj->mFlags68 &= ~0x8000;
        }
        void* model = *(void**)((u8*)obj + 0x98);
        if (model) {
            if (enable) {
                *(u32*)((u8*)model + 0x7A8) |= 0x1000;
            } else {
                *(u32*)((u8*)model + 0x7A8) &= ~0x1000;
            }
        }
    }
    return 0;
}

// us-8003f380: func_8003EE04
// Enables/disables a flag on the object
extern "C" int func_8003EE04(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    if (obj) {
        func_800BE33C(obj, enable ? 1 : 0);
    }
    return 0;
}

// us-8003f3f0: func_8003EE74
// Calls a game-level function with a bool parameter
extern "C" int func_8003EE74(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    func_801864DC(ctx, handle);
    void* gameObj = func_800BBC0C();
    if (gameObj) {
        func_800BF314(gameObj, enable);
    }
    return 0;
}

// us-8003f45c: func_8003EEE0
// Battle party selection/generation
extern "C" int func_8003EEE0(VMThread* pThread, int handle, int r5) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int partyId = vmArgIntGet(2, ptr);
    int arg2;
    if (vmArgOmitChk(pThread, 2)) {
        arg2 = 0;
    } else {
        VMArg* ptr2 = vmArgPtrGet(pThread, 2);
        arg2 = vmArgIntGet(3, ptr2);
    }
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    u32 flag = lbl_eu_80663E24;
    int hadFlag = (flag >> 13) & 1;
    u32 newFlag = flag & ~0x00020000;
    if (arg3) {
        newFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = newFlag;
    void* result = func_80081694__Q22cf13CfGameManagerFv((u16)partyId, (u16)arg2);
    newFlag = lbl_eu_80663E24;
    u32 restoredFlag = newFlag & ~0x00020000;
    if (hadFlag) {
        restoredFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = restoredFlag;
    void* ctx;
    if (!result) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    ctx = func_801862C0(pThread);
    func_801863F4(ctx);
    *(u32*)((u8*)result + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003f5b0: setRot
// Sets the rotation of an object from 3 fixed-point angles
extern "C" int setRot(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int rotX = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int rotY = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int rotZ = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)rotX / 2048.0f * 0.0000958738f;
    float fy = (float)(s32)rotY / 2048.0f * 0.0000958738f;
    float fz = (float)(s32)rotZ / 2048.0f * 0.0000958738f;
    float rot[3] = { fx, fy, fz };
    ((void(*)(void*, void*))(*(void***)obj)[0xBC/4])(obj, rot);
    return 0;
}

// us-8003f6f4: gravity
// Toggles gravity on an object
extern "C" int gravity(VMThread* pThread, int handle) {
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    int enable = vmArgBoolGet(2, ptr);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x150/4])(obj, enable ? 0 : 1);
    return 0;
}

// us-8003f78c: func_8003F210
// Battle party lookup by name, returns OC handle
extern "C" int func_8003F210(VMThread* pThread, int handle, int r5) {
    void* ctx;
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int param = vmArgIntGet(3, ptr2);
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    u32 flag = lbl_eu_80663E24;
    int hadFlag = (flag >> 13) & 1;
    u32 newFlag = flag & ~0x00020000;
    if (arg3) {
        newFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = newFlag;
    void* result = 0;
    if (strcmp(name, &lbl_eu_804FA74C[0x6B]) == 0 || strcmp(name, &lbl_eu_804FA74C[0x54]) == 0) {
        result = func_80081CBC__Q22cf13CfGameManagerFv(&lbl_eu_804FA74C[0x6B], (u16)param);
    } else if (strcmp(name, &lbl_eu_804FA74C[0x79]) == 0) {
        result = func_80081CB8__Q22cf13CfGameManagerFv();
    }
    if (!result) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    newFlag = lbl_eu_80663E24;
    u32 restoredFlag = newFlag & ~0x00020000;
    if (hadFlag) {
        restoredFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = restoredFlag;
    ctx = func_801862C0(pThread);
    func_801863F4(ctx);
    *(u32*)((u8*)result + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003f914: func_8003F398
// Sets a virtual function on the object (enable)
extern "C" int func_8003F398(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x158/4])(obj, 1);
    return 0;
}

// us-8003f994: func_8003F418
// Sets a virtual function on the object (disable)
extern "C" int func_8003F418(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x158/4])(obj, 0);
    return 0;
}

// us-8003fa14: func_8003F498
// Battle party member lookup by name/type
extern "C" int func_8003F498(VMThread* pThread, int handle, int r5) {
    void* ctx;
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int index = vmArgIntGet(3, ptr2);
    int arg3;
    if (vmArgOmitChk(pThread, 3)) {
        arg3 = 0;
    } else {
        VMArg* ptr3 = vmArgPtrGet(pThread, 3);
        arg3 = vmArgIntGet(4, ptr3);
    }
    u32 flag = lbl_eu_80663E24;
    int hadFlag = (flag >> 13) & 1;
    u32 newFlag = flag & ~0x00020000;
    if (arg3) {
        newFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = newFlag;
    void* result = 0;
    if (strcmp(name, &lbl_eu_804FA74C[0x85]) == 0 || strcmp(name, &lbl_eu_804FA74C[0x89]) == 0) {
        result = func_800817BC__Q22cf13CfGameManagerFv(index, 0);
    } else if (strcmp(name, &lbl_eu_804FA74C[0x8C]) == 0 || strcmp(name, &lbl_eu_804FA74C[0x93]) == 0) {
        result = func_8008187C__Q22cf13CfGameManagerFv(index);
    }
    if (!result) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    cf::CfGameManager::getInstance();
    if (lbl_eu_80663E24 & 0x00040000) {
        *(u32*)((u8*)result + 0x64) |= 0x00010000;
    }
    newFlag = lbl_eu_80663E24;
    u32 restoredFlag = newFlag & ~0x00020000;
    if (hadFlag) {
        restoredFlag |= 0x00040000;
    }
    lbl_eu_80663E24 = restoredFlag;
    ctx = func_801862C0(pThread);
    func_801863F4(ctx);
    *(u32*)((u8*)result + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-8003fbc8: func_8003F64C
// Enables a virtual function on the object (same as func_8003F398)
extern "C" int func_8003F64C(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x158/4])(obj, 1);
    return 0;
}

// us-8003fc48: func_8003F6CC
// Disables a virtual function on the object (same as func_8003F418)
extern "C" int func_8003F6CC(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x158/4])(obj, 0);
    return 0;
}

// us-8003fcc8: func_8003F74C
// Sets a property on an object with a signed byte value
extern "C" int func_8003F74C(VMThread* pThread, int handle) {
    int value;
    if (vmArgOmitChk(pThread, 1)) {
        value = 1;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        value = vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    func_800ACC14(obj, (s8)value);
    return 0;
}

// us-8003fd48: func_8003F7CC
// Sets a follow/attention relationship between two objects
extern "C" int func_8003F7CC(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* target = 0;
    if (targetOC) {
        ctx = func_801862C0(pThread);
        target = func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
    }
    func_800ACF78(obj, target, 0);
    return 0;
}

// us-8003fdec: func_8003F870
// Sets a follow relationship with parent/child lookup
extern "C" int func_8003F870(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* parent = 0;
    void* child = 0;
    if (targetOC) {
        ctx = func_801862C0(pThread);
        parent = func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
        if (parent) {
            u32* fieldC8 = (u32*)((u8*)parent + 0xC8);
            void* followTarget = 0;
            if (*fieldC8) {
                followTarget = *(void**)((u8*)parent + 0x6F8);
            } else {
                u32* fieldCC = (u32*)((u8*)parent + 0xCC);
                if (*fieldCC) {
                    followTarget = *(void**)((u8*)parent + 0x6FC);
                }
            }
            if (followTarget) {
                    void** vt = *(void***)followTarget;
                    void (*vfunc)(void*) = (void (*)(void*))vt[0xAC / 4];
                    vfunc(followTarget);
                    child = followTarget;
            }
        }
    }
    func_800ACF78(obj, parent, child);
    return 0;
}

// us-8003fef8: func_8003F97C
// Unfollow/clear a relationship
extern "C" int func_8003F97C(VMThread* pThread, int handle) {
    int targetOC;
    if (vmArgOmitChk(pThread, 1)) {
        targetOC = 0;
    } else {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        targetOC = (int)vmArgOCGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    void* target = 0;
    if (targetOC) {
        ctx = func_801862C0(pThread);
        target = func_801864DC(ctx, *(int*)((u8*)targetOC + 4));
    }
    func_800ACFD8(obj, target);
    return 0;
}

// us-8003ff98: func_8003FA1C
// Sets up a battle encounter by name
extern "C" int func_8003FA1C(VMThread* pThread, int handle, int r5) {
    void* ctx;
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int param = vmArgIntGet(3, ptr2);
    void* battleMgr = func_80081CB8__Q22cf13CfGameManagerFv();
    if (battleMgr) {
        *(u32*)((u8*)battleMgr + 0x94) = 5;
        func_800AC4A8(battleMgr, (u16)param);
        ((void(*)(void*, int))(*(void***)battleMgr)[0x158/4])(battleMgr, 0);
    }
    if (!battleMgr) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    ctx = func_801862C0(pThread);
    func_801863F4(ctx);
    *(u32*)((u8*)battleMgr + 0x68) |= 0x40000000;
    u8 type = 9;
    VMArg retVal;
    retVal.type = type;
    retVal.unk2 = (u16)r5;
    retVal.value.pointerVal = ctx;
    vmRetValSet(pThread, &retVal);
    return 1;
}

// us-80040094: func_8003FB18
// Enables a virtual function on the object (same as func_8003F398)
extern "C" int func_8003FB18(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x158/4])(obj, 1);
    return 0;
}

// us-80040114: func_8003FB98
// Disables a virtual function on the object (same as func_8003F418)
extern "C" int func_8003FB98(VMThread* pThread, int handle) {
    if (!vmArgOmitChk(pThread, 1)) {
        VMArg* ptr = vmArgPtrGet(pThread, 1);
        vmArgIntGet(2, ptr);
    }
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    ((void(*)(void*, int))(*(void***)obj)[0x158/4])(obj, 0);
    return 0;
}

// us-80040194: func_8003FC18
// Moves an object by a fixed-point offset in its local X direction
extern "C" int func_8003FC18(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int distX = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int distY = vmArgFixedGet(3, ptr2);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fDistX = (float)(s32)distX / 2048.0f;
    float fDistY = (float)(s32)distY / 2048.0f;
    if (obj) {
        void* pos = ((void*(*)(void*))(*(void***)obj)[0xBC/4])(obj);
        float x = *(float*)((u8*)pos + 0);
        float y = *(float*)((u8*)pos + 4);
        float z = *(float*)((u8*)pos + 8);
        float offset[3] = { x, y + fDistY, z };
        func_800ABF24(obj, pos, offset, fDistX);
    }
    return 0;
}

// us-800402c4: func_8003FD48
// Moves an object to a position using 3 fixed-point coordinates
extern "C" int func_8003FD48(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int x = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int y = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int z = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)x / 2048.0f;
    float fy = (float)(s32)y / 2048.0f;
    float fz = (float)(s32)z / 2048.0f;
    float pos[3] = { fx, fy, fz };
    ((void(*)(void*, void*))(*(void***)obj)[0xBC/4])(obj, pos);
    return 0;
}

// us-80040458: func_8003FEDC
// Rotation function with 3 fixed-point angles
extern "C" int func_8003FEDC(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int rotX = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int rotY = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int rotZ = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)rotX / 2048.0f * 0.0000958738f;
    float fy = (float)(s32)rotY / 2048.0f * 0.0000958738f;
    float fz = (float)(s32)rotZ / 2048.0f * 0.0000958738f;
    float rot[3] = { fx, fy, fz };
    ((void(*)(void*, void*))(*(void***)obj)[0xBC/4])(obj, rot);
    return 0;
}

// us-80040570: func_8003FFF4
// Sets a position on an object using 3 fixed-point coords
extern "C" int func_8003FFF4(VMThread* pThread, int handle) {
    VMArg* ptr1 = vmArgPtrGet(pThread, 1);
    int x = vmArgFixedGet(2, ptr1);
    VMArg* ptr2 = vmArgPtrGet(pThread, 2);
    int y = vmArgFixedGet(3, ptr2);
    VMArg* ptr3 = vmArgPtrGet(pThread, 3);
    int z = vmArgFixedGet(4, ptr3);
    void* ctx = func_801862C0(pThread);
    cf::CfObject* obj = (cf::CfObject*)func_801864DC(ctx, handle);
    float fx = (float)(s32)x / 2048.0f;
    float fy = (float)(s32)y / 2048.0f;
    float fz = (float)(s32)z / 2048.0f;
    float pos[3] = { fx, fy, fz };
    ((void(*)(void*, void*))(*(void***)obj)[0xBC/4])(obj, pos);
    return 0;
}

// us-80040664: ocUnitRegist
// Registers all OC unit plugin functions
extern "C" void ocUnitRegist() {}

void CObjectState_UnkVirtualFunc1__Q22cf12CObjectStateFv(void* self, unsigned long bits) {
    *(unsigned long*)((char*)self + 4) |= bits;
}

void CObjectParam_UnkVirtualFunc6__Q22cf12CObjectParamFv(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void CObjectState_UnkVirtualFunc3__Q22cf12CObjectStateFv(void* self, unsigned long mask) {
    *(unsigned long*)((char*)self + 4) &= ~mask;
}

void CfObjectModel_UnkVirtualFunc16__Q22cf13CfObjectModelFv(void* self, u8 val) { static_cast<cf::CfObjectModel*>(self)->field_BD = val; }

u32 CObjectParam_UnkVirtualFunc5__Q22cf12CObjectParamFv(void* self) { return *(u32*)((u8*)self + 0x34); }

void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x48) = a;
    *(u32*)((u8*)self + 0x4C) = b;
    *(u32*)((u8*)self + 0x50) = c;
}

void CfObject_UnkVirtualFunc64__Q22cf8CfObjectFv(void* self, int flag) {
    u32* field = (u32*)((char*)self + 0x68);
    if (flag) {
        *field |= 0x01000000;
    } else {
        *field &= ~0x01000000;
    }
}