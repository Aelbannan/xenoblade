// Auto-scaffolded catalog TU for kyoshin/plugin/ocBdat
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

/* "src/kyoshin/plugin/ocBdat.cpp" line 4 "kyoshin/plugin/ocBdat.hpp" */
#pragma once

/* "src/kyoshin/plugin/ocBdat.hpp" line 2 "types.h" */
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
/* "src/kyoshin/plugin/ocBdat.hpp" line 3 "monolib/vm/yvm2.h" */
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
/* "src/kyoshin/plugin/ocBdat.cpp" line 5 "kyoshin/code_801862C0.hpp" */
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
/* "src/kyoshin/plugin/ocBdat.cpp" line 6 "monolib/vm/yvm2.h" */
/* end "monolib/vm/yvm2.h" */
/* "src/kyoshin/plugin/ocBdat.cpp" line 7 "string.h" */
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

extern "C" {
extern s8 lbl_eu_80663D10;
extern u32 lbl_eu_80663D14;
extern void* lbl_eu_805705D0[8];

BOOL vmOCRegist(OCData* pOC);
extern OCData lbl_eu_80524E40;

void* func_8003B4B0(void* bdat, const char* col);
u32 func_8003B6A0(void* base, void* data, u32 type);
u32 func_8003B748(void* table, void* col, s32 row, s32 index);
u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);
u32 func_8003AD98(void* bdat, const char* col, s32 row, s32 index);
u32 func_8003AFC0(void* bdat, const char* col);
u32 func_8003B204(void* bdat, const char* col);
u32 func_eu_8003B488(void* bdat, const char* col1, s32 row, const char* col2);
void func_8003B800(VMArg* out, void* data, u32 type);
}

extern "C" void* func_eu_8003B720(void* p);

// LLM-HARNESS-BEGIN: us-8003ae14
extern "C" void* func_8003AA34() {
    if (!lbl_eu_80663D10) {
        lbl_eu_80663D10 = 1;
    }
    return &lbl_eu_80663D14;
}
// LLM-HARNESS-END: us-8003ae14

// LLM-HARNESS-BEGIN: us-8003ae30
void* CBdat::func_8003AA50() {
    void** table = lbl_eu_805705D0;
    table[0] = 0;
    table[1] = 0;
    table[2] = 0;
    table[3] = 0;
    table[4] = 0;
    table[5] = 0;
    table[6] = 0;
    return table;
}
// LLM-HARNESS-END: us-8003ae30

// LLM-HARNESS-BEGIN: us-8003ae5c
void* CBdat::func_8003AA78(u32 idx, void* p) {
    lbl_eu_805705D0[idx] = p;
    return func_eu_8003B720(p);
}
// LLM-HARNESS-END: us-8003ae5c

// LLM-HARNESS-BEGIN: us-8003ae74
void CBdat::func_8003AA8C(u32 idx) {
    lbl_eu_805705D0[idx] = 0;
}
// LLM-HARNESS-END: us-8003ae74

// LLM-HARNESS-BEGIN: us-8003ae8c
#pragma dont_inline on
void* getFP(const char* pName) {
    void** tableSlot;
    const char* name;
    s32 tableIdx;
    void* table;
    s32 hi;
    s32 lo;
    u32 sum;
    u32 adj;
    s32 mid;
    u32 entryOff;
    char* entry;
    u16 nameOff;
    const char* entryName;
    s32 cmp;

    tableSlot = lbl_eu_805705D0;
    name = pName;
    tableIdx = 0;
    while (tableIdx < 7) {
        table = *tableSlot;
        if (table != 0) {
            lo = 0;
            hi = *reinterpret_cast<s32*>(table);
            while (lo < hi) {
                sum = static_cast<u32>(lo) + static_cast<u32>(hi);
                adj = sum + (sum >> 31);
                mid = adj >> 1;
                entryOff = *reinterpret_cast<u32*>(reinterpret_cast<char*>(table) + (adj & ~1u) + 8u);
                entry = reinterpret_cast<char*>(table) + entryOff;
                nameOff = *reinterpret_cast<u16*>(entry + 6);
                entryName = entry + nameOff;
                cmp = strcmp(entryName, name);
                if (cmp < 0) {
                    lo = static_cast<s32>(mid + 1);
                } else if (cmp > 0) {
                    hi = static_cast<s32>(mid);
                } else {
                    return entry;
                }
            }
        }
        tableIdx++;
        tableSlot++;
    }
    return 0;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003ae8c

// LLM-HARNESS-BEGIN: us-8003af48
#pragma dont_inline on
extern "C" void* func_8003B4B0(void* bdat, const char* col) {
    void* bdatArg;
    const char* colArg;
    u16 bucketCount;
    char* hashBase;
    s32 hashLen;
    s32 pos;
    s32 hash;
    u32 rem;
    u16 bucketOff;
    char* entry;
    u16 nextOff;

    bdatArg = bdat;
    colArg = col;
    if (bdatArg == 0) {
        return 0;
    }
    bucketCount = *reinterpret_cast<u16*>(reinterpret_cast<char*>(bdatArg) + 0xC);
    hashBase = reinterpret_cast<char*>(bdatArg) + *reinterpret_cast<u16*>(reinterpret_cast<char*>(bdatArg) + 0xA);
    hashLen = static_cast<s32>(strlen(colArg));
    if (hashLen > 8) {
        hashLen = 8;
    }
    hash = 0;
    pos = 0;
    while (pos < hashLen) {
        hash = hash * 7 + static_cast<s8>(colArg[pos]);
        pos++;
    }
    rem = static_cast<u32>(hash);
    rem -= (rem / bucketCount) * bucketCount;
    bucketOff = *reinterpret_cast<u16*>(hashBase + rem * 2);
    if (bucketOff == 0) {
        return 0;
    }
    entry = reinterpret_cast<char*>(bdatArg) + bucketOff;
    while (1) {
        if (strcmp(colArg, entry + 4) == 0) {
            break;
        }
        nextOff = *reinterpret_cast<u16*>(entry + 2);
        if (nextOff == 0) {
            entry = 0;
            break;
        }
        entry = reinterpret_cast<char*>(bdatArg) + nextOff;
    }
    return entry;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003af48

// LLM-HARNESS-BEGIN: us-8003b148
#pragma dont_inline on
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index) {
    void* bdatArg;
    const char* colArg;
    s32 indexArg;
    void* colEntry;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    u16 colRel;
    char* colHdr;
    u16 stride;
    u16 dataOff;
    s32 rowBytes;
    u16 colDataOff;
    u32 elemType;
    char* dataPtr;

    bdatArg = bdat;
    colArg = col;
    indexArg = index;
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = indexArg - rowBase;
    if (rowIdx < 0 || maxRow < rowIdx) {
        return 0;
    }
    indexArg = rowIdx;
    colRel = *reinterpret_cast<u16*>(colEntry);
    colHdr = tbl + colRel;
    if (static_cast<u8>(colHdr[0]) != 1) {
        return 0;
    }
    stride = *reinterpret_cast<u16*>(tbl + 0x8);
    dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
    rowBytes = indexArg * stride;
    colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    elemType = static_cast<u8>(colHdr[1]);
    dataPtr = tbl + dataOff + rowBytes + colDataOff;
    return func_8003B6A0(bdatArg, dataPtr, elemType);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b148

// LLM-HARNESS-BEGIN: us-8003b220
#pragma dont_inline on
extern "C" u32 func_8003AD98(void* bdat, const char* col, s32 row, s32 index) {
    void* bdatArg;
    const char* colArg;
    s32 rowArg;
    s32 indexArg;
    void* colEntry;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;

    bdatArg = bdat;
    colArg = col;
    rowArg = row;
    indexArg = index;
    if (bdatArg == 0) {
        return 0;
    }
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx) {
        ok = 0;
    } else if (rowIdx < 0) {
        ok = 0;
    } else {
        rowArg = rowIdx;
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    return func_8003B748(bdatArg, colEntry, rowArg, indexArg);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b220

// LLM-HARNESS-BEGIN: us-8003b2e4
#pragma dont_inline on
extern "C" u32 func_8003AFC0(void* bdat, const char* col) {
    void* bdatArg;
    const char* colArg;
    void* colEntry;
    char* tbl;
    char* colHdr;

    bdatArg = bdat;
    colArg = col;
    if (bdatArg == 0) {
        return 0;
    }
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    colHdr = tbl + *reinterpret_cast<u16*>(colEntry);
    if (static_cast<u8>(colHdr[0]) != 2) {
        return 0;
    }
    return *reinterpret_cast<u16*>(colHdr + 0x4);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b2e4

// LLM-HARNESS-BEGIN: us-8003b360
extern "C" u32 func_8003B1EC(void* p) {
    if (p == 0) {
        return 0;
    }
    return *reinterpret_cast<u16*>(reinterpret_cast<char*>(p) + 0x10);
}
// LLM-HARNESS-END: us-8003b360

// LLM-HARNESS-BEGIN: us-8003b378
extern "C" u32 func_8003B6A0(void* base, void* data, u32 type);

#pragma dont_inline on
extern "C" u32 func_8003B434(void* table, void*, void* col, s32 row) {
    void* tableArg;
    void* colArg;
    s32 rowArg;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;
    u16 colRel;
    char* colHdr;
    u16 stride;
    u16 dataOff;
    s32 rowBytes;
    u16 colDataOff;
    char* dataBase;
    u8 elemType;
    char* dataPtr;

    tableArg = table;
    colArg = col;
    rowArg = row;
    if (tableArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(tableArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx) {
        goto bounds_fail;
    }
    if (rowIdx >= 0) {
        goto bounds_success;
    }
bounds_fail:
    ok = 0;
    goto bounds_check;
bounds_success:
    rowArg = rowIdx;
    ok = 1;
bounds_check:
    if (ok == 0) {
        return 0;
    }
    colRel = *reinterpret_cast<u16*>(colArg);
    colHdr = tbl + colRel;
    if (static_cast<u8>(colHdr[0]) != 1) {
        return 0;
    }
    stride = *reinterpret_cast<u16*>(tbl + 0x8);
    dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
    rowBytes = rowArg * stride;
    colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    dataBase = tbl + dataOff;
    elemType = static_cast<u8>(colHdr[1]);
    dataBase += rowBytes;
    dataPtr = dataBase + colDataOff;
    return func_8003B6A0(tableArg, dataPtr, elemType);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b378

// LLM-HARNESS-BEGIN: us-8003b408
#pragma dont_inline on
extern "C" u32 func_8003B204(void* bdat, const char* col) {
    void* bdatArg;
    const char* colArg;
    void* colEntry;
    char* tbl;
    char* colHdr;

    bdatArg = bdat;
    colArg = col;
    if (bdatArg == 0) {
        return 0;
    }
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    colHdr = tbl + *reinterpret_cast<u16*>(colEntry);
    return static_cast<u8>(colHdr[1]);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b408

// LLM-HARNESS-BEGIN: us-8003b470
extern "C" u32 func_8003B41C(void* p) {
    if (p == 0) {
        return 0;
    }
    return *reinterpret_cast<u16*>(reinterpret_cast<char*>(p) + 0x12);
}
// LLM-HARNESS-END: us-8003b470

// LLM-HARNESS-BEGIN: us-8003b488
#pragma dont_inline on
extern "C" u32 func_eu_8003B488(void* bdat, const char* col1, s32 row, const char* col2) {
    void* bdatArg;
    const char* col1Arg;
    s32 rowArg;
    const char* col2Arg;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;
    void* col1Entry;
    void* col2Entry;
    char* flagHdr;
    char* col1Hdr;
    u32 val;
    u32 mask;
    u8 shift;

    bdatArg = bdat;
    col1Arg = col1;
    rowArg = row;
    col2Arg = col2;
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx) {
        ok = 0;
    } else if (rowIdx < 0) {
        ok = 0;
    } else {
        rowArg = rowIdx;
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    col1Entry = func_8003B4B0(bdatArg, col1Arg);
    if (col1Entry == 0) {
        return 0;
    }
    col2Entry = func_8003B4B0(bdatArg, col2Arg);
    if (col2Entry == 0) {
        return 0;
    }
    flagHdr = tbl + *reinterpret_cast<u16*>(col2Entry);
    if (static_cast<u8>(flagHdr[0]) != 3) {
        return 0;
    }
    if (static_cast<u16>(reinterpret_cast<char*>(col1Entry) - tbl) != *reinterpret_cast<u16*>(flagHdr + 0x6)) {
        return 0;
    }
    val = 0;
    col1Hdr = tbl + *reinterpret_cast<u16*>(col1Entry);
    if (static_cast<u8>(col1Hdr[0]) == 1) {
        u16 stride = *reinterpret_cast<u16*>(tbl + 0x8);
        u16 dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
        s32 rowBytes = rowArg * stride;
        u16 colDataOff = *reinterpret_cast<u16*>(col1Hdr + 0x2);
        u32 elemType = static_cast<u8>(col1Hdr[1]);
        char* dataPtr = tbl + dataOff + rowBytes + colDataOff;
        val = func_8003B6A0(bdatArg, dataPtr, elemType);
    }
    mask = *reinterpret_cast<u32*>(flagHdr + 0x2);
    shift = static_cast<u8>(flagHdr[1]);
    return (val & mask) >> shift;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b488

// LLM-HARNESS-BEGIN: us-8003b5d8
#pragma dont_inline on
extern "C" u32 func_8003B6A0(void* base, void* data, u32 type) {
    volatile u32 local;
    local = 0;
    switch (type) {
    case 0:
        break;
    case 1:
        *reinterpret_cast<volatile u8*>(&local) = *reinterpret_cast<u8*>(data);
        break;
    case 2:
        *reinterpret_cast<volatile u16*>(&local) = *reinterpret_cast<u16*>(data);
        break;
    case 3:
        local = *reinterpret_cast<u32*>(data);
        break;
    case 4:
        *reinterpret_cast<volatile u8*>(&local) = *reinterpret_cast<u8*>(data);
        break;
    case 5:
        *reinterpret_cast<volatile s16*>(&local) = *reinterpret_cast<s16*>(data);
        break;
    case 6:
        local = *reinterpret_cast<u32*>(data);
        break;
    case 7:
        local = reinterpret_cast<u32>(base) + *reinterpret_cast<u32*>(data);
        break;
    case 8:
        local = *reinterpret_cast<u32*>(data);
        break;
    default:
        break;
    }
    return local;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b5d8

// LLM-HARNESS-BEGIN: us-8003b670
extern "C" u32 func_8003B748(void* table, void* col, s32 row, s32 index) {
    char* tbl = reinterpret_cast<char*>(table);
    u16 colOff = *reinterpret_cast<u16*>(col);
    char* colHdr = tbl + colOff;
    if (static_cast<u8>(colHdr[0]) != 2) {
        return 0;
    }
    s32 count = *reinterpret_cast<u16*>(colHdr + 0x4);
    if (count <= index) {
        return 0;
    }
    u16 stride = *reinterpret_cast<u16*>(tbl + 0x8);
    u16 dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
    s32 rowStride = row * stride;
    u32 elemType = static_cast<u8>(colHdr[1]);
    char* dataBase = tbl + dataOff;
    u16 colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    dataBase += rowStride;
    dataBase += colDataOff;
    // Retail: subi/cmplwi then ble to *4; fall-through is the cmpwi switch.
    if (elemType - 6 > 1) {
        switch (elemType) {
        case 1:
        case 4:
            dataBase += index;
            break;
        case 2:
        case 5:
            dataBase += index * 2;
            break;
        case 3:
            dataBase += index * 4;
            break;
        default:
            break;
        }
    } else {
        dataBase += index * 4;
    }
    return func_8003B6A0(table, dataBase, elemType);
}
// LLM-HARNESS-END: us-8003b670

// LLM-HARNESS-BEGIN: us-8003b720
#pragma dont_inline on
extern "C" void* func_eu_8003B720(void* p) {
    u32 mem1 = 0x80000000u;
    u32* entryList = reinterpret_cast<u32*>(reinterpret_cast<char*>(p) + 8);
    u32 idx = 0;
    // Separate load vs xor-result temps (retail r23 / r31) to improve stmw/regalloc.
    u32 xorTmp;

    // Reload count at loop_check so MWCC cannot steal CTR for the outer walk
    // (inners are addic./bne; retail uses CTR on inners and cmplw on outer).
    goto loop_check;
loop_body:
    {
        u8* ent = reinterpret_cast<u8*>(p) + *entryList;
        if ((ent[4] & 2) != 0) {
            s32 start = *reinterpret_cast<s32*>(ent + 0x18);
            s32 len = *reinterpret_cast<s32*>(ent + 0x1C);
            u32 keyWord = *reinterpret_cast<u16*>(ent + 0x16);
            s32 end = start + len;
            u32 byteHi = static_cast<u8>(keyWord >> 8);
            u32 byteLo = static_cast<u8>(keyWord);
            // nor interleaved with cmpw (cr1) before masking keys.
            u32 norHi = ~byteHi;
            s32 phase1Empty = (start >= end);
            u32 norLo = ~byteLo;
            u32 kHi = static_cast<u8>(norHi);
            u32 kLo = static_cast<u8>(norLo);
            if (phase1Empty)
                goto phase2;

            {
                s32 lim16 = end - 0x10;
                s32 half = (len + 1) / 2;
                if (half > 8) {
                    s32 f24 = 0;
                    s32 f25 = 0;
                    s32 f26 = 0;
                    s32 f27 = 0;
                    s32 f23 = 0;
                    s32 lim = static_cast<s32>(mem1) - 3;
                    if (start > end)
                        goto gate1b;
                    if (end > lim)
                        goto gate1b;
                    f23 = 1;
                gate1b:
                    if (f23 == 0)
                        goto gate1c;
                    lim = static_cast<s32>(mem1) - 3;
                    if (start > lim)
                        goto gate1c;
                    f27 = 1;
                gate1c:
                    if (f27 == 0)
                        goto gate1d;
                    if (static_cast<u32>(start) + 0x80000000u == 0)
                        goto gate1d;
                    f26 = 1;
                gate1d:
                    if (f26 == 0)
                        goto gate1e;
                    {
                        s32 tmp = 1;
                        s32 signEnd = end & static_cast<s32>(0x80000000u);
                        s32 signNegStart = (-start) & static_cast<s32>(0x80000000u);
                        if (signEnd == signNegStart) {
                            if (signEnd != (len & static_cast<s32>(0x80000000u))) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate1e;
                        f25 = 1;
                    }
                gate1e:
                    if (f25 == 0)
                        goto gate1f;
                    {
                        s32 tmp = 1;
                        if ((len & static_cast<s32>(0x80000000u)) == 0) {
                            len += 1;
                            if ((len & static_cast<s32>(0x80000000u)) != 0) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate1f;
                        f24 = 1;
                    }
                gate1f:
                    if (f24 == 0)
                        goto residual1;
                    {
                        // Peak size 0x540: do-while(--n) → addic./bne (retail mtctr soft-cap).
                        u32 n = static_cast<u32>(lim16 + 0xF - start) >> 4;
                        if (start < lim16) {
                            do {
                                xorTmp = ent[start];
                                u8* cur = ent + start;
                                ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                start += 16;
                                xorTmp = cur[1];
                                cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[2];
                                cur[2] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[3];
                                cur[3] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[4];
                                cur[4] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[5];
                                cur[5] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[6];
                                cur[6] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[7];
                                cur[7] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[8];
                                cur[8] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[9];
                                cur[9] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[10];
                                cur[10] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[11];
                                cur[11] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[12];
                                cur[12] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[13];
                                cur[13] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[14];
                                cur[14] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[15];
                                cur[15] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                            } while (--n != 0);
                        }
                    }
                }
            residual1:
                {
                    u32 n = static_cast<u32>(end + 1 - start) >> 1;
                    if (start < end) {
                        do {
                            xorTmp = ent[start];
                            u8* cur = ent + start;
                            ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                            start += 2;
                            xorTmp = cur[1];
                            cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                        } while (--n != 0);
                    }
                }
            }

        phase2:
            kHi = static_cast<u8>(norHi);
            start = *reinterpret_cast<u16*>(ent + 6);
            end = *reinterpret_cast<u16*>(ent + 0xA);
            kLo = static_cast<u8>(norLo);
            if (start < end) {
                s32 span = end - start;
                s32 lim16 = end - 0x10;
                s32 span1 = span + 1;
                s32 half = span1 / 2;
                if (half > 8) {
                    s32 f28 = 0;
                    s32 f31 = 0;
                    s32 f26 = 0;
                    s32 f25 = 0;
                    s32 f24 = 0;
                    s32 lim = static_cast<s32>(mem1) - 3;
                    if (start > end)
                        goto gate2b;
                    if (end > lim)
                        goto gate2b;
                    f24 = 1;
                gate2b:
                    if (f24 == 0)
                        goto gate2c;
                    lim = static_cast<s32>(mem1) - 3;
                    if (start > lim)
                        goto gate2c;
                    f25 = 1;
                gate2c:
                    if (f25 == 0)
                        goto gate2d;
                    if (static_cast<u32>(start) + 0x80000000u == 0)
                        goto gate2d;
                    f26 = 1;
                gate2d:
                    if (f26 == 0)
                        goto gate2e;
                    {
                        s32 tmp = 1;
                        s32 signEnd = end & static_cast<s32>(0x80000000u);
                        s32 signNegStart = (-start) & static_cast<s32>(0x80000000u);
                        if (signEnd == signNegStart) {
                            if (signEnd != (span & static_cast<s32>(0x80000000u))) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate2e;
                        f31 = 1;
                    }
                gate2e:
                    if (f31 == 0)
                        goto gate2f;
                    {
                        // Retail: test span sign, then span1 sign (no mutate).
                        s32 tmp = 1;
                        if ((span & static_cast<s32>(0x80000000u)) == 0) {
                            if ((span1 & static_cast<s32>(0x80000000u)) != 0) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate2f;
                        f28 = 1;
                    }
                gate2f:
                    if (f28 == 0)
                        goto residual2;
                    {
                        u32 n = static_cast<u32>(lim16 + 0xF - start) >> 4;
                        if (start < lim16) {
                            do {
                                xorTmp = ent[start];
                                u8* cur = ent + start;
                                ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                start += 16;
                                xorTmp = cur[1];
                                cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[2];
                                cur[2] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[3];
                                cur[3] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[4];
                                cur[4] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[5];
                                cur[5] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[6];
                                cur[6] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[7];
                                cur[7] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[8];
                                cur[8] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[9];
                                cur[9] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[10];
                                cur[10] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[11];
                                cur[11] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[12];
                                cur[12] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[13];
                                cur[13] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[14];
                                cur[14] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[15];
                                cur[15] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                            } while (--n != 0);
                        }
                    }
                }
            residual2:
                {
                    u32 n = static_cast<u32>(end + 1 - start) >> 1;
                    if (start < end) {
                        do {
                            xorTmp = ent[start];
                            u8* cur = ent + start;
                            ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                            start += 2;
                            xorTmp = cur[1];
                            cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                        } while (--n != 0);
                    }
                }
            }

            ent[4] = ent[4] & 0xFD;
        }
        entryList += 1;
        idx += 1;
    }
loop_check:
    if (idx < *reinterpret_cast<u32*>(p)) {
        goto loop_body;
    }
    return p;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b720

// LLM-HARNESS-BEGIN: us-8003bc60
#pragma dont_inline on
extern "C" void func_8003B800(VMArg* out, void* data, u32 type) {
    VMArg* outArg;
    void* dataArg;
    u32 typeArg;
    u32 local;

    outArg = out;
    dataArg = data;
    typeArg = type;
    local = 0;
    switch (typeArg) {
    case 0:
        break;
    case 1:
        local = *reinterpret_cast<u8*>(dataArg);
        break;
    case 2:
        local = *reinterpret_cast<u16*>(dataArg);
        break;
    case 3:
        local = *reinterpret_cast<u32*>(dataArg);
        break;
    case 4:
        local = static_cast<u32>(static_cast<s32>(static_cast<s8>(*reinterpret_cast<u8*>(dataArg))));
        break;
    case 5:
        local = static_cast<u32>(static_cast<s32>(*reinterpret_cast<s16*>(dataArg)));
        break;
    case 6:
        local = *reinterpret_cast<u32*>(dataArg);
        break;
    case 7:
        outArg->type = VM_TYPE_STRING;
        outArg->unk2 = static_cast<u16>(strlen(*reinterpret_cast<const char* const*>(dataArg)));
        outArg->value.pointerVal = *reinterpret_cast<void* const*>(dataArg);
        break;
    case 8:
        outArg->type = VM_TYPE_FIXED;
        outArg->value.uintVal = *reinterpret_cast<u32*>(dataArg);
        break;
    default:
        break;
    }
    if (typeArg != 7 && typeArg != 8) {
        outArg->type = VM_TYPE_INT;
        outArg->value.uintVal = local;
    }
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bc60

// LLM-HARNESS-BEGIN: us-8003bd4c
#pragma dont_inline on
extern "C" int bdat(VMThread* t, void* /*unused*/, u16 unk) {
    VMThread* thread;
    u16 unkArg;
    const char* name;
    VMArg result;

    thread = t;
    unkArg = unk;
    name = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    result.type = VM_TYPE_OC;
    result.unk2 = unkArg;
    result.value.pointerVal = getFP(name);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bd4c

// LLM-HARNESS-BEGIN: us-8003bdb8
#pragma dont_inline on
extern "C" int getVal(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    s32 idx;
    u32 dataSlot;
    u32 type;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    idx = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    dataSlot = getBdatStringColumnValue(bdatTbl, col, idx);
    type = func_8003B204(bdatTbl, col);
    func_8003B800(&result, &dataSlot, type);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bdb8

// LLM-HARNESS-BEGIN: us-8003be70
#pragma dont_inline on
extern "C" int getArrayVal(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    s32 row;
    s32 index;
    u32 dataSlot;
    u32 type;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    row = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    index = vmArgIntGet(4, vmArgPtrGet(thread, 3));
    dataSlot = func_8003AD98(bdatTbl, col, row, index);
    type = func_8003B204(bdatTbl, col);
    func_8003B800(&result, &dataSlot, type);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003be70

// LLM-HARNESS-BEGIN: us-8003bf48
#pragma dont_inline on
extern "C" int getArrayCount(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003AFC0(bdatTbl, col);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bf48

// LLM-HARNESS-BEGIN: us-8003bfb8
#pragma dont_inline on
extern "C" int getVarType(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B204(bdatTbl, col);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bfb8

// LLM-HARNESS-BEGIN: us-8003c028
#pragma dont_inline on
extern "C" int getIdCount(VMThread* t, void* bdat) {
    VMArg result;

    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B1EC(bdat);
    vmRetValSet(t, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003c028

// LLM-HARNESS-BEGIN: us-8003c074
#pragma dont_inline on
extern "C" int getIdTop(VMThread* t, void* bdat) {
    VMArg result;

    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B41C(bdat);
    vmRetValSet(t, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003c074

// LLM-HARNESS-BEGIN: us-8003c0c0
#pragma dont_inline on
extern "C" int getFlagVal(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col1;
    s32 row;
    const char* col2;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col1 = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    row = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    col2 = vmArgStringGet(4, vmArgPtrGet(thread, 3));
    result.type = VM_TYPE_INT;
    result.value.uintVal = func_eu_8003B488(bdatTbl, col1, row, col2);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003c0c0

// LLM-HARNESS-BEGIN: us-8003c180
extern "C" void ocBdatRegist() {
    vmOCRegist(&lbl_eu_80524E40);
}
// LLM-HARNESS-END: us-8003c180
