// Auto-scaffolded catalog TU for kyoshin/cf/CtrlObjectParam
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/cf/CtrlObjectParam.cpp" line 3 "kyoshin/harness_catalog.hpp" */
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
/* "src/kyoshin/cf/CtrlObjectParam.cpp" line 4 "kyoshin/cf/CtrlObjectParam.hpp" */
#pragma once

/* "src/kyoshin/cf/CtrlObjectParam.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {

    // ── Opaque struct for func_8009D764 ─────────────────────────────────────
    //   6 u16s (set to 0xFFFF) + 192-byte zeroed area.
    struct CtrlObjectParamInit {
        u16 field_00;
        u16 field_02;
        u16 field_04;
        u16 field_06;
        u16 field_08;
        u16 field_0A;
        u8  blob[192];
    };

    // ── 32-byte sub-struct entry (getSubStruct / getShortAt1C / getByteE4) ──
    struct CtrlObjectParamSubEntry {
        u8  pad_00[0x10];        // 0x00..0x0F
        s16 shortArr[8];         // 0x10..0x1F  — accessed at work+0x1C = entry[0].shortArr
    };

    // ── Opaque data block with sub-entries (getSubStruct, getShortAt1C, getByteE4) ──
    struct CtrlObjectParamData {
        u8                       pad_0C[0x0C];          // 0x00..0x0B
        CtrlObjectParamSubEntry  entries[6];            // 0x0C..0xCB  (6×32=192)
        u8                       pad_CC[0x18];           // 0xCC..0xE3
        u8                       field_E4;               // 0xE4

        CtrlObjectParamSubEntry* getSubStruct(unsigned long index);
        void setArgType2(void* arg);
        void setArgType3(void* arg);
        void setArgType5(void* arg);
        long getShortAt1C(unsigned long index);
    };

    // ── Swap view (swapIntFields): int arrays at +4 and +16 ─────────────────
    struct CtrlObjectParamSwap {
        u8  pad_00[4];       // 0x00..0x03
        int intArr1[3];      // 0x04..0x0F  (type 1, indices 0,1,2)
        int intArr2[3];      // 0x10..0x1B  (type 2, indices 0,1,2)
    };

    // ── Clear view (clearStruct): byte + 24 ints ───────────────────────────
    struct CtrlObjectParamClear {
        u8  firstByte;        // 0x00
        u32 words[24];        // 0x04..0x63

        int clearStruct();
    };

    // ── Clear 16 bytes view (clear16Bytes) ─────────────────────────────────
    struct CtrlObjectParamClear16 {
        u32 words[4];         // 0x00..0x0F

        void clear16Bytes();
    };

    // ── Byte-at-E4 view (getByteE4) ────────────────────────────────────────
    struct CtrlObjectParamByteE4 {
        u8  pad_00[0xE4];
        u8  field_E4;

        u8 getByteE4();
    };

} // namespace cf
/* end "kyoshin/cf/CtrlObjectParam.hpp" */
/* "src/kyoshin/cf/CtrlObjectParam.cpp" line 5 "kyoshin/cf/object/CActorParam.hpp" */
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
        virtual void CBattleState_UnkVirtualFunc5();  //0x18
        virtual void CBattleState_UnkVirtualFunc6();  //0x1C
        virtual void CBattleState_UnkVirtualFunc7();  //0x20
        virtual void CBattleState_UnkVirtualFunc8();  //0x24
        virtual void CBattleState_UnkVirtualFunc9();  //0x28
        virtual void CBattleState_UnkVirtualFunc10(); //0x2C
        virtual void CBattleState_UnkVirtualFunc11(); //0x30
        virtual void CBattleState_UnkVirtualFunc12(); //0x34
        virtual void CBattleState_UnkVirtualFunc13(); //0x38
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
        virtual void CBattleState_UnkVirtualFunc26(); //0x6C
        virtual void CBattleState_UnkVirtualFunc27(); //0x70
        virtual void CBattleState_UnkVirtualFunc28(); //0x74
        virtual void CBattleState_UnkVirtualFunc29(); //0x78
        virtual void CBattleState_UnkVirtualFunc30(); //0x7C
        virtual void CBattleState_UnkVirtualFunc31(); //0x80
        virtual void CBattleState_UnkVirtualFunc32(); //0x84
        virtual void CBattleState_UnkVirtualFunc33(); //0x88

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

namespace cf {
    struct CBattleStateSrcEntry;
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
        virtual void CActorParam_UnkVirtualFunc37();  //0x128
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
/* "src/kyoshin/cf/CtrlObjectParam.cpp" line 6 "kyoshin/cf/object/CObjectParam.hpp" */
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

void __ct__8009D604(){}

void __dt__8009D72C(){}

extern "C" void* func_8009D764(cf::CtrlObjectParamInit* p) {
    p->field_00 = 0xFFFF;
    p->field_02 = 0xFFFF;
    p->field_04 = 0xFFFF;
    p->field_06 = 0xFFFF;
    p->field_08 = 0xFFFF;
    p->field_0A = 0xFFFF;
    extern void* memset(void*, int, unsigned long);
    return static_cast<char*>(memset(p->blob, 0, sizeof(p->blob)));
}

void func_8009D790(){}

cf::CtrlObjectParamSubEntry* cf::CtrlObjectParamData::getSubStruct(unsigned long index) {
    return &entries[index];
}

void func_8009D7F4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc94() {
    return &unk1650;
}

void func_8009DB1C(){}

void func_8009DB28(){}

void func_8009DBF4(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc33(float val) {
    reinterpret_cast<float&>(unk17E4.unk0[4]) = val;
}

void func_8009DFC8(){}

void func_8009E024(){}

void func_8009E030(){}

void cf::CtrlObjectParamData::setArgType2(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 2, arg);
}

void cf::CtrlObjectParamData::setArgType3(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 3, arg);
}

void func_8009E054(){}

void cf::CtrlObjectParamData::setArgType5(void* arg) {
    extern void func_8009DBF4(cf::CtrlObjectParamData*, unsigned long, void*);
    func_8009DBF4(this, 5, arg);
}

long cf::CtrlObjectParamData::getShortAt1C(unsigned long index) {
    return entries[0].shortArr[index];
}

void func_8009E0C4(){}

void func_8009E120(){}

void func_8009E168(){}

extern "C" int func_8009E20C(cf::CtrlObjectParamSwap* self, int firstType, int firstIndex,
                             int secondType, int secondIndex) {
    int* first = 0;
    int* second = 0;
    if (firstType == 1)
        first = &self->intArr1[firstIndex];
    else if (firstType == 2)
        first = &self->intArr2[firstIndex];
    if (secondType == 1)
        second = &self->intArr1[secondIndex];
    else if (secondType == 2)
        second = &self->intArr2[secondIndex];
    int value = *first;
    *first = *second;
    *second = value;
    return 1;
}

void func_8009E284(){}

extern "C" int func_8009E344(const unsigned int* param_1, unsigned int param_2,
                              int* param_3, int* param_4) {
    for (int i = 0; i < 3; ++i) {
        if (param_1[i + 1] == param_2) {
            *param_3 = 1;
            *param_4 = i;
            return 1;
        }
    }
    for (int i = 0; i < 6; ++i) {
        if (param_1[i + 4] == param_2) {
            *param_3 = 2;
            *param_4 = i;
            return 1;
        }
    }
    return 0;
}

void func_8009E3C0(){}

void func_8009E474(){}

void func_8009E574(){}

void func_8009E56C(void* self){ func_8009E574(); }

void func_8009E740(){}

int cf::CtrlObjectParamClear::clearStruct() {
    firstByte = 0;
    for (int i = 0; i < 24; ++i) words[i] = 0;
    return 1;
}

void func_8009E838(){}

void func_8009E974(){}

void func_8009EABC(){}

void func_8009EB2C(){}

void func_8009EB94(){}

extern "C" int func_8009EBE8(unsigned int idx) {
    extern u32 lbl_eu_80663E88;
    if (idx >= 50000) return 0;
    const u8* bitmap = reinterpret_cast<const u8*>(lbl_eu_80663E88 + 8188);
    return (bitmap[idx >> 3] >> (idx & 7)) & 1;
}

void func_8009EC18(){}

void func_8009EC6C(){}

extern "C" void* func_8009EC9C(int idx) {
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + idx * 15828 + 16880);
}

extern "C" void* func_8009ECB0() {
    extern unsigned long lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + 0x1f98);
}

extern "C" void* func_8009ECBC(int idx) {
    extern u32 lbl_eu_80663E88;
    return reinterpret_cast<void*>(lbl_eu_80663E88 + (idx * 8) + 88);
}

extern "C" void func_8009ECD0(unsigned long val) {
    extern unsigned long lbl_eu_80663E88;
    unsigned long addr = lbl_eu_80663E88;
    *reinterpret_cast<unsigned long*>(addr + 0x3A388) = val;
}

extern "C" u32 func_8009ECE0() {
    extern u32 lbl_eu_80663E88;
    return *reinterpret_cast<u32*>(lbl_eu_80663E88 + 0x3A388);
}

extern "C" unsigned long func_8009ECF0() {
    extern unsigned long lbl_eu_80663E88;
    return *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50);
}

extern "C" void func_8009ECFC(unsigned short value) {
    extern unsigned long lbl_eu_80663E88;
    *reinterpret_cast<unsigned short*>(lbl_eu_80663E88 + 0x50) = value;
}

void __ct__8009ED08(){}

void func_8009EF9C(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc16(float val) {
    unk15F8 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc55(u16 val) {
    unk160E = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc61(u16 val) {
    unk1616 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc65(float val) {
    unk1610 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc68(float val) {
    unk1618 = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc74(float val) {
    unk17E4.unk4C = val;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc14(u8 val) {
    unk15F4[0] = val;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc165() {
    return &unk164C;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc116(float val) {
    unk1620 = val;
}

void func_8009F6D4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc152() {
    return &unk3358;
}

void __ct__8009F8B8(){}

void func_800A03F4(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc125() {
    return unk2740;
}

void* cf::CActorParam::CActorParam_UnkVirtualFunc76() {
    return &unk17E4.unk4C;
}

void func_800A082C(){}

void func_800A0860(){}

void* cf::CActorParam::CActorParam_UnkVirtualFunc100() {
    return &unk17E4;
}

void func_800A0E64(){}

void func_800A11A4(){}

void func_800A1370(){}

void CActorParam_UnkVirtualFunc126__Q22cf11CActorParamFv() {}

void func_800A13C4(){}

void func_800A145C(){}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc26() {
    return reinterpret_cast<u32&>(unk1650);
}

void CActorParam_UnkVirtualFunc166__Q22cf11CActorParamFv() {}

void func_800A18A4(){}

void func_800A1B08(){}

void func_800A1CA0(){}

void func_800A1E3C(){}

void func_800A21F8(){}

void cf::CActorParam::CActorParam_UnkVirtualFunc82(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1604 + addend;
    unk1604 = val;
    if (val > cap) {
        unk1604 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc90(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1608 + addend;
    unk1608 = val;
    if (val > cap) {
        unk1608 = cap;
    }
}

void func_800A26A4(){}

void func_800A282C(){}

void func_800A2974(){}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc85() {
    return unk1604;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc83(u32 addend) {
    u32 cap = 0x05F60000 - 7937;
    u32 val = unk1600 + addend;
    unk1600 = val;
    if (val > cap) {
        unk1600 = cap;
    }
}

void cf::CActorParam::CActorParam_UnkVirtualFunc81(u32 val) {
    unk1604 = val;
}

void func_800A2AF0(){}

void func_800A2DE8(){}

void func_800A30E4(){}

u8 cf::CtrlObjectParamByteE4::getByteE4() {
    return field_E4;
}

void func_800A32C4(){}

void func_800A3304(){}

void func_800A33C8(){}

void cf::CObjectState::CObjectState_UnkVirtualFunc13() {
    unkC = unk8;
}

void* cf::CObjectState::CObjectState_UnkVirtualFunc12() {
    return &unkC;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc6() {
    unk8 = 0;
}

void cf::CObjectState::CObjectState_UnkVirtualFunc4() {
    unk4 = 0;
}

int cf::CObjectParam::CObjectParam_UnkVirtualFunc3() {
    return field_30 != 0 ? 1 : 0;
}

void cf::CtrlObjectParamClear16::clear16Bytes() {
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
}

void __dt__800A34E0(){}

void func_800A3520(){}

void func_800A3594(){}

void func_800A36A4(){}

void func_800A37CC(){}

void func_800A3940(){}

void func_800A3998(){}

void func_800A39E8(){}

void func_800A3A6C(){}
