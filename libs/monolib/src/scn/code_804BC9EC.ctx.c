// Auto-scaffolded catalog TU for monolib/src/scn/code_804BC9EC
// Replace stubs with high-level C/C++ during decomp.

/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 3 "harness_catalog.h" */
#pragma once

/**
 * Minimal includes for auto-scaffolded catalog TUs (libs + non-kyoshin).
 * Kyoshin scaffolds use kyoshin/harness_catalog.hpp instead.
 */
/* "include/harness_catalog.h" line 6 "types.h" */
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
/* end "harness_catalog.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 4 "monolib/scn/code_804BC9EC.hpp" */
#pragma once

/* "libs/monolib/include/monolib/scn/code_804BC9EC.hpp" line 2 "harness_catalog.h" */
/* end "harness_catalog.h" */

// Minimal layout for the global data at lbl_eu_8065F32C.
// Field offsets 0x60 and 0x78 are used by func_804B80A4 / func_804B8078.
struct ScnResData {
    u8 field_0x00[0x60];
    void* field_0x60; // array of 0xE0-byte elements
    u8 field_0x64[0x78 - 0x64];
    int count; // 0x78
};

extern ScnResData lbl_eu_8065F32C;

extern "C" {
ScnResData* func_804B80A4(ScnResData* self, int index);
void func_804B8078(ScnResData* self, int index);
void func_804B80CC(ScnResData* self);
void func_804B7804(ScnResData* self);
void func_804BC9A0(ScnResData*);
void func_804B7D9C(int, int);
void func_804B7DD4(ScnResData*);
}
/* end "monolib/scn/code_804BC9EC.hpp" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 5 "PowerPC_EABI_Support/Runtime/NMWException.h" */
#ifndef _NMWEXCEPTION
#define _NMWEXCEPTION

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Runtime/NMWException.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/Runtime/NMWException.h" line 4 "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h" */
#ifndef __PPC_EABI_LINKER
#define __PPC_EABI_LINKER

__declspec(section ".init") extern char _stack_addr[];
__declspec(section ".init") extern char _stack_end[];
__declspec(section ".init") extern char _heap_addr[];
__declspec(section ".init") extern char _heap_end[];

__declspec(section ".init") extern const char _fextabindex_rom[];
__declspec(section ".init") extern char _fextabindex[];
__declspec(section ".init") extern char _eextabindex[];

__declspec(section ".init") extern char _SDA_BASE_[];
__declspec(section ".init") extern char _SDA2_BASE_[];

typedef struct __rom_copy_info {
  char* rom;
  char* addr;
  unsigned int size;
} __rom_copy_info;

__declspec(section ".init") extern __rom_copy_info _rom_copy_info[];

typedef struct __bss_init_info {
  char* addr;
  unsigned int size;
} __bss_init_info;

__declspec(section ".init") extern __bss_init_info _bss_init_info[];

typedef struct __eti_init_info {
  void* eti_start;
  void* eti_end;
  void* code_start;
  unsigned long code_size;
} __eti_init_info;

__declspec(section ".init") extern __eti_init_info _eti_init_info[];


__declspec(section ".init") extern const char _f_init_rom[];
__declspec(section ".init") extern char _f_init[];
__declspec(section ".init") extern char _e_init[];

__declspec(section ".init") extern const char _f_text_rom[];
__declspec(section ".init") extern char _f_text[];
__declspec(section ".init") extern char _e_text[];

__declspec(section ".init") extern const char _f_rodata_rom[];
__declspec(section ".init") extern char _f_rodata[];
__declspec(section ".init") extern char _e_rodata[];

__declspec(section ".init") extern const char _fextab_rom[];
__declspec(section ".init") extern char _fextab[];
__declspec(section ".init") extern char _eextab[];

__declspec(section ".init") extern const char _f_data_rom[];
__declspec(section ".init") extern char _f_data[];
__declspec(section ".init") extern char _e_data[];

__declspec(section ".init") extern char _f_bss[];
__declspec(section ".init") extern char _e_bss[];

__declspec(section ".init") extern const char _f_sdata_rom[];
__declspec(section ".init") extern char _f_sdata[];
__declspec(section ".init") extern char _e_sdata[];

__declspec(section ".init") extern char _f_sbss[];
__declspec(section ".init") extern char _e_sbss[];

__declspec(section ".init") extern const char _f_sdata2_rom[];
__declspec(section ".init") extern char _f_sdata2[];
__declspec(section ".init") extern char _e_sdata2[];

__declspec(section ".init") extern char _f_sbss2[];
__declspec(section ".init") extern char _e_sbss2[];

__declspec(section ".init") extern const char _f_PPC_EMB_sdata0_rom[];
__declspec(section ".init") extern char _f_PPC_EMB_sdata0[];
__declspec(section ".init") extern char _e_PPC_EMB_sdata0[];

__declspec(section ".init") extern char _f_PPC_EMB_sbss0[];
__declspec(section ".init") extern char _e_PPC_EMB_sbss0[];


#endif // __PPC_EABI_LINKER
/* end "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h" */

#ifdef __cplusplus
extern "C" {
#endif

typedef short vbase_ctor_arg_type;
typedef char local_cond_type;

typedef struct CatchInfo {
    void* location;
    void* typeinfo;
    void* dtor;
    void* sublocation;
    long pointercopy;
    void* stacktop;
} CatchInfo;

typedef struct DestructorChain {
  struct DestructorChain* next;
  void* destructor;
  void* object;
} DestructorChain;

extern void* __register_global_object(void* object, void* destructor, void* registration);
extern void __destroy_global_chain(void);

extern void __end__catch(CatchInfo* catchinfo);
extern void __throw(char* throwtype, void* location, void* dtor);
extern void __unexpected(CatchInfo* catchinfo);

extern int __register_fragment(struct __eti_init_info* info, char* TOC);

#ifdef __cplusplus
}
#endif

// Defined in Gecko_ExceptionPPC.cp with C++ linkage; callers and the
// definition must agree on the mangled name.
void __unregister_fragment(unsigned int fragmentID);

#endif // _NMWEXCEPTION
/* end "PowerPC_EABI_Support/Runtime/NMWException.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 6 "revolution/gx/GXGeometry.h" */
#ifndef RVL_SDK_GX_GEOMETRY_H
#define RVL_SDK_GX_GEOMETRY_H
/* "libs/RVL_SDK/include/revolution/gx/GXGeometry.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXGeometry.h" line 4 "revolution/GX/GXTypes.h" */
#ifndef RVL_SDK_GX_TYPES_H
#define RVL_SDK_GX_TYPES_H
/* "libs/RVL_SDK/include/revolution/GX/GXTypes.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pack value into bitfield.
 * Value is shifted to the specified bit position.
 * (Bit indices are LSB)
 */
#define GX_BITSET(field, pos, size, value)                                     \
    __rlwimi((field), (value), 31 - (pos) - (size) + 1, (pos),                 \
             (pos) + (size) - 1)

/**
 * Compose value from bitfield.
 * Value is shifted after masking.
 * (Bit indices are LSB)
 */
#define GX_BITGET(field, pos, size)                                            \
    ((field) >> (31 - (pos) - (size) + 1) & ((1 << (size)) - 1))

/**
 * Pack value into bitfield.
 * Value is not shifted, only masked.
 * (Bit indices are LSB)
 */
#define GX_BITSET_TRUNC(field, pos, size, value)                               \
    __rlwimi((field), (value), 0, (pos), (pos) + (size) - 1)

/**
 * Compose value from bitfield.
 * Value is not shifted, only masked.
 * (Bit indices are LSB)
 */
#define GX_BITGET_TRUNC(field, pos, size)                                      \
    ((field) & (((1 << (size)) - 1) << (32 - (pos) - (size))))

/**
 * Common types used throughout many GX files.
 * To avoid circular depencies, common structures and all enums have been moved
 * here. Everything is organized in alphabetical order.
 */

// Differentiate between 8-byte and boolean values
// Real name! (from patent)
typedef unsigned char GXBool;

#define GX_TRUE ((GXBool)TRUE)
#define GX_FALSE ((GXBool)FALSE)

#define GX_ENABLE GX_TRUE
#define GX_DISABLE GX_FALSE

/**
 * Common structs
 */

typedef struct _GXColor {
    u8 r, g, b, a;
} GXColor;

typedef struct _GXColorS10 {
    s16 r, g, b, a;
} GXColorS10;

/**
 * Common enums
 */

typedef enum _GXAlphaOp {
    GX_AOP_AND,
    GX_AOP_OR,
    GX_AOP_XOR,
    GX_AOP_XNOR,

    GX_MAX_ALPHAOP
} GXAlphaOp;

typedef enum _GXAnisotropy {
    GX_ANISO_1,
    GX_ANISO_2,
    GX_ANISO_4,

    GX_MAX_ANISOTROPY
} GXAnisotropy;

typedef enum _GXAttnFn {
    GX_AF_SPEC,
    GX_AF_SPOT,
    GX_AF_NONE,
} GXAttnFn;

typedef enum _GXAttr {
    GX_VA_PNMTXIDX,   // Position/Normal matrix index
    GX_VA_TEX0MTXIDX, // GX_TEXCOORD0 matrix index
    GX_VA_TEX1MTXIDX, // GX_TEXCOORD1 matrix index
    GX_VA_TEX2MTXIDX, // GX_TEXCOORD2 matrix index
    GX_VA_TEX3MTXIDX, // GX_TEXCOORD3 matrix index
    GX_VA_TEX4MTXIDX, // GX_TEXCOORD4 matrix index
    GX_VA_TEX5MTXIDX, // GX_TEXCOORD5 matrix index
    GX_VA_TEX6MTXIDX, // GX_TEXCOORD6 matrix index
    GX_VA_TEX7MTXIDX, // GX_TEXCOORD7 matrix index

    GX_VA_POS, // Position
    GX_VA_NRM, // Normal

    GX_VA_CLR0, // Color 0
    GX_VA_CLR1, // Color 1

    GX_VA_TEX0, // Texture Coordinate 0
    GX_VA_TEX1, // Texture Coordinate 1
    GX_VA_TEX2, // Texture Coordinate 2
    GX_VA_TEX3, // Texture Coordinate 3
    GX_VA_TEX4, // Texture Coordinate 4
    GX_VA_TEX5, // Texture Coordinate 5
    GX_VA_TEX6, // Texture Coordinate 6
    GX_VA_TEX7, // Texture Coordinate 7

    GX_POS_MTX_ARRAY,
    GX_NRM_MTX_ARRAY,
    GX_TEX_MTX_ARRAY,
    GX_LIGHT_ARRAY,

    GX_VA_NBT, // All three normal elements (normal/binormal/tangent)

    GX_VA_MAX_ATTR,
    GX_VA_NULL = 255
} GXAttr;

typedef enum _GXAttrType {
    GX_NONE,   // No data sent
    GX_DIRECT, // Data sent directly to FIFO
    GX_INDEX8, // 8-bit index sent to FIFO
    GX_INDEX16 // 16-bit index sent to FIFO
} GXAttrType;

typedef enum _GXBlendFactor {
    GX_BL_ZERO,
    GX_BL_ONE,
    GX_BL_SRCCLR,
    GX_BL_INVSRCCLR,
    GX_BL_SRCALPHA,
    GX_BL_INVSRCALPHA,
    GX_BL_DSTALPHA,
    GX_BL_INVDSTALPHA,

    GX_BL_DSTCLR = GX_BL_SRCCLR,
    GX_BL_INVDSTCLR = GX_BL_INVSRCCLR
} GXBlendFactor;

typedef enum _GXBlendMode {
    GX_BM_NONE,
    GX_BM_BLEND,
    GX_BM_LOGIC,
    GX_BM_SUBTRACT,

    GX_MAX_BLENDMODE
} GXBlendMode;

typedef enum _GXChannelID {
    GX_COLOR0,
    GX_COLOR1,
    GX_ALPHA0,
    GX_ALPHA1,
    GX_COLOR0A0,
    GX_COLOR1A1,
    GX_COLOR_ZERO,
    GX_ALPHA_BUMP,
    GX_ALPHA_BUMPN,

    GX_COLOR_NULL = 255
} GXChannelID;

typedef enum _GXCITexFmt {
    GX_TF_C4 = 8,
    GX_TF_C8,
    GX_TF_C14X2,
} GXCITexFmt;

typedef enum _GXClearZ {
    GX_CLEAR_Z_MIN = 0,
    GX_CLEAR_Z_MAX = (1 << 24) - 1,
} GXClearZ;

typedef enum _GXClipMode {
    // "ClipDisable" in XF mem, so 0 = enable
    GX_CLIP_ENABLE,
    GX_CLIP_DISABLE,
} GXClipMode;

typedef enum _GXColorSrc { GX_SRC_REG, GX_SRC_VTX } GXColorSrc;

typedef enum _GXCompare {
    GX_NEVER,
    GX_LESS,
    GX_EQUAL,
    GX_LEQUAL,
    GX_GREATER,
    GX_NEQUAL,
    GX_GEQUAL,
    GX_ALWAYS
} GXCompare;

typedef enum _GXCompCnt {
    GX_POS_XY = 0,
    GX_POS_XYZ,

    GX_NRM_XYZ = 0,
    GX_NRM_NBT,
    GX_NRM_NBT3,

    GX_CLR_RGB = 0,
    GX_CLR_RGBA,

    GX_TEX_S = 0,
    GX_TEX_ST
} GXCompCnt;

typedef enum _GXCompType {
    GX_U8,
    GX_S8,
    GX_U16,
    GX_S16,
    GX_F32,

    GX_RGB565 = 0,
    GX_RGB8,
    GX_RGBX8,
    GX_RGBA4,
    GX_RGBA6,
    GX_RGBA8
} GXCompType;

typedef enum _GXCopyClamp {
    GX_CLAMP_NONE,
    GX_CLAMP_TOP,
    GX_CLAMP_BOTTOM,
    GX_CLAMP_ALL,
} GXCopyClamp;

typedef enum _GXCullMode {
    GX_CULL_NONE,
    GX_CULL_FRONT,
    GX_CULL_BACK,
    GX_CULL_ALL
} GXCullMode;

typedef enum _GXDiffuseFn { GX_DF_NONE, GX_DF_SIGN, GX_DF_CLAMP } GXDiffuseFn;

typedef enum _GXDirtyFlag {
    GX_DIRTY_SU_TEX = (1 << 0),
    GX_DIRTY_BP_MASK = (1 << 1),
    GX_DIRTY_GEN_MODE = (1 << 2),
    GX_DIRTY_VCD = (1 << 3),
    GX_DIRTY_VAT = (1 << 4),
    // . . .
    GX_DIRTY_AMB_COLOR0 = (1 << 8),
    GX_DIRTY_AMB_COLOR1 = (1 << 9),
    GX_DIRTY_MAT_COLOR0 = (1 << 10),
    GX_DIRTY_MAT_COLOR1 = (1 << 11),
    GX_DIRTY_CHAN_COLOR0 = (1 << 12),
    GX_DIRTY_CHAN_COLOR1 = (1 << 13),
    GX_DIRTY_CHAN_ALPHA0 = (1 << 14),
    GX_DIRTY_CHAN_ALPHA1 = (1 << 15),
    GX_DIRTY_TEX0 = (1 << 16),
    GX_DIRTY_TEX1 = (1 << 17),
    GX_DIRTY_TEX2 = (1 << 18),
    GX_DIRTY_TEX3 = (1 << 19),
    GX_DIRTY_TEX4 = (1 << 20),
    GX_DIRTY_TEX5 = (1 << 21),
    GX_DIRTY_TEX6 = (1 << 22),
    GX_DIRTY_TEX7 = (1 << 23),
    GX_DIRTY_NUM_COLORS = (1 << 24),
    GX_DIRTY_NUM_TEX = (1 << 25),
    GX_DIRTY_MTX_IDX = (1 << 26),
    GX_DIRTY_PROJECTION = (1 << 27),
    GX_DIRTY_VIEWPORT = (1 << 28),

    GX_AMB_MAT_MASK = GX_DIRTY_AMB_COLOR0 | GX_DIRTY_AMB_COLOR1 |
                      GX_DIRTY_MAT_COLOR0 | GX_DIRTY_MAT_COLOR1,

    GX_LIGHT_CHAN_MASK = GX_DIRTY_CHAN_COLOR0 | GX_DIRTY_CHAN_COLOR1 |
                         GX_DIRTY_CHAN_ALPHA0 | GX_DIRTY_CHAN_ALPHA1 |
                         GX_DIRTY_NUM_COLORS,

    GX_TEX_GEN_MASK = 0x2FF0000,
} GXDirtyFlag;

typedef enum _GXDistAttnFn {
    GX_DA_OFF,
    GX_DA_GENTLE,
    GX_DA_MEDIUM,
    GX_DA_STEEP
} GXDistAttnFn;

typedef enum _GXFogType {
    GX_FOG_NONE,

    GX_FOG_PERSP_LIN = 2,
    GX_FOG_PERSP_EXP = 4,
    GX_FOG_PERSP_EXP2 = 5,
    GX_FOG_PERSP_REVEXP = 6,
    GX_FOG_PERSP_REVEXP2 = 7,

    // Fourth bit is set to mark orthographic
    GX_FOG_ORTHO_LIN = 1 << 3 | GX_FOG_PERSP_LIN,
    GX_FOG_ORTHO_EXP = 1 << 3 | GX_FOG_PERSP_EXP,
    GX_FOG_ORTHO_EXP2 = 1 << 3 | GX_FOG_PERSP_EXP2,
    GX_FOG_ORTHO_REVEXP = 1 << 3 | GX_FOG_PERSP_REVEXP,
    GX_FOG_ORTHO_REVEXP2 = 1 << 3 | GX_FOG_PERSP_REVEXP2
} GXFogType;

// Access components of the fog type
#define GX_FOG_GET_PROJ(x) ((x) >> 3 & 1)
#define GX_FOG_GET_FSEL(x) ((x) & 7)

typedef enum _GXIndTexAlphaSel {
    GX_ITBA_OFF,
    GX_ITBA_S,
    GX_ITBA_T,
    GX_ITBA_U,

    GX_MAX_ITBALPHA
} GXIndTexAlphaSel;

typedef enum _GXIndTexBiasSel {
    GX_ITB_NONE,
    GX_ITB_S,
    GX_ITB_T,
    GX_ITB_ST,
    GX_ITB_U,
    GX_ITB_SU,
    GX_ITB_TU,
    GX_ITB_STU,

    GX_MAX_ITBIAS
} GXIndTexBiasSel;

typedef enum _GXIndTexFormat {
    GX_ITF_8,
    GX_ITF_5,
    GX_ITF_4,
    GX_ITF_3,

    GX_MAX_ITFORMAT
} GXIndTexFormat;

typedef enum _GXIndTexMtxID {
    GX_ITM_OFF,
    GX_ITM_0,
    GX_ITM_1,
    GX_ITM_2,

    GX_ITM_S0 = 5,
    GX_ITM_S1,
    GX_ITM_S2,

    GX_ITM_T0 = 9,
    GX_ITM_T1,
    GX_ITM_T2,
} GXIndTexMtxID;

typedef enum _GXIndTexScale {
    GX_ITS_1,
    GX_ITS_2,
    GX_ITS_4,
    GX_ITS_8,
    GX_ITS_16,
    GX_ITS_32,
    GX_ITS_64,
    GX_ITS_128,
    GX_ITS_256,

    GX_MAX_ITSCALE
} GXIndTexScale;

typedef enum _GXIndTexStageID {
    GX_INDTEXSTAGE0,
    GX_INDTEXSTAGE1,
    GX_INDTEXSTAGE2,
    GX_INDTEXSTAGE3,

    GX_MAX_INDTEXSTAGE
} GXIndTexStageID;

typedef enum _GXIndTexWrap {
    GX_ITW_OFF,
    GX_ITW_256,
    GX_ITW_128,
    GX_ITW_64,
    GX_ITW_32,
    GX_ITW_16,
    GX_ITW_0,

    GX_MAX_ITWRAP,
} GXIndTexWrap;

typedef enum _GXLightID {
    GX_LIGHT0 = (1 << 0),
    GX_LIGHT1 = (1 << 1),
    GX_LIGHT2 = (1 << 2),
    GX_LIGHT3 = (1 << 3),
    GX_LIGHT4 = (1 << 4),
    GX_LIGHT5 = (1 << 5),
    GX_LIGHT6 = (1 << 6),
    GX_LIGHT7 = (1 << 7),

    GX_MAX_LIGHT = (1 << 8),
    GX_LIGHT_NULL = 0
} GXLightID;

typedef enum _GXLogicOp {
    GX_LO_CLEAR,
    GX_LO_AND,
    GX_LO_REVAND,
    GX_LO_COPY,
    GX_LO_INVAND,
    GX_LO_NOOP,
    GX_LO_XOR,
    GX_LO_OR,
    GX_LO_NOR,
    GX_LO_EQUIV,
    GX_LO_INV,
    GX_LO_REVOR,
    GX_LO_INVCOPY,
    GX_LO_INVOR,
    GX_LO_NAND,
    GX_LO_SET
} GXLogicOp;

typedef enum _GXMtxType {
    GX_MTX_3x4,
    GX_MTX_2x4,
} GXMtxType;

typedef enum _GXPixelFmt {
    GX_PF_RGB8_Z24,
    GX_PF_RGBA6_Z24,
    GX_PF_RGBA565_Z16,
    GX_PF_Z24,
    GX_PF_Y8,
    GX_PF_U8,
    GX_PF_V8,
    GX_PF_YUV420,

    GX_MAX_PIXELFMT
} GXPixelFmt;

/**
 * Matrix column index into XF memory.
 * (Multiply by row dimension to get XF mem offset)
 */
typedef enum _GXPosNrmMtx {
    GX_PNMTX0 = 0,
    GX_PNMTX1 = 3,
    GX_PNMTX2 = 6,
    GX_PNMTX3 = 9,
    GX_PNMTX4 = 12,
    GX_PNMTX5 = 15,
    GX_PNMTX6 = 18,
    GX_PNMTX7 = 21,
    GX_PNMTX8 = 24,
    GX_PNMTX9 = 27
} GXPosNrmMtx;

typedef enum _GXPrimitive {
    GX_POINTS = 0xB8,
    GX_LINES = 0xA8,
    GX_LINESTRIP = 0xB0,
    GX_TRIANGLES = 0x90,
    GX_TRIANGLESTRIP = 0x98,
    GX_TRIANGLEFAN = 0xA0,
    GX_QUADS = 0x80,
} GXPrimitive;

typedef enum _GXProjectionType {
    GX_PERSPECTIVE,
    GX_ORTHOGRAPHIC
} GXProjectionType;

typedef enum _GXPerf0 {
    GX_PERF0_VERTICES,
    GX_PERF0_CLIP_VTX,
    GX_PERF0_CLIP_CLKS,
    GX_PERF0_XF_WAIT_IN,
    GX_PERF0_XF_WAIT_OUT,
    GX_PERF0_XF_XFRM_CLKS,
    GX_PERF0_XF_LIT_CLKS,
    GX_PERF0_XF_BOT_CLKS,
    GX_PERF0_XF_REGLD_CLKS,
    GX_PERF0_XF_REGRD_CLKS,
    GX_PERF0_CLIP_RATIO,
    GX_PERF0_TRIANGLES,
    GX_PERF0_TRIANGLES_CULLED,
    GX_PERF0_TRIANGLES_PASSED,
    GX_PERF0_TRIANGLES_SCISSORED,
    GX_PERF0_TRIANGLES_0TEX,
    GX_PERF0_TRIANGLES_1TEX,
    GX_PERF0_TRIANGLES_2TEX,
    GX_PERF0_TRIANGLES_3TEX,
    GX_PERF0_TRIANGLES_4TEX,
    GX_PERF0_TRIANGLES_5TEX,
    GX_PERF0_TRIANGLES_6TEX,
    GX_PERF0_TRIANGLES_7TEX,
    GX_PERF0_TRIANGLES_8TEX,
    GX_PERF0_TRIANGLES_0CLR,
    GX_PERF0_TRIANGLES_1CLR,
    GX_PERF0_TRIANGLES_2CLR,
    GX_PERF0_QUAD_0CVG,
    GX_PERF0_QUAD_NON0CVG,
    GX_PERF0_QUAD_1CVG,
    GX_PERF0_QUAD_2CVG,
    GX_PERF0_QUAD_3CVG,
    GX_PERF0_QUAD_4CVG,
    GX_PERF0_AVG_QUAD_CNT,
    GX_PERF0_CLOCKS,
    GX_PERF0_NONE
} GXPerf0;

typedef enum _GXPerf1 {
    GX_PERF1_TEXELS,
    GX_PERF1_TX_IDLE,
    GX_PERF1_TX_REGS,
    GX_PERF1_TX_MEMSTALL,
    GX_PERF1_TC_CHECK1_2,
    GX_PERF1_TC_CHECK3_4,
    GX_PERF1_TC_CHECK5_6,
    GX_PERF1_TC_CHECK7_8,
    GX_PERF1_TC_MISS,
    GX_PERF1_VC_ELEMQ_FULL,
    GX_PERF1_VC_MISSQ_FULL,
    GX_PERF1_VC_MEMREQ_FULL,
    GX_PERF1_VC_STATUS7,
    GX_PERF1_VC_MISSREP_FULL,
    GX_PERF1_VC_STREAMBUF_LOW,
    GX_PERF1_VC_ALL_STALLS,
    GX_PERF1_VERTICES,
    GX_PERF1_FIFO_REQ,
    GX_PERF1_CALL_REQ,
    GX_PERF1_VC_MISS_REQ,
    GX_PERF1_CP_ALL_REQ,
    GX_PERF1_CLOCKS,
    GX_PERF1_NONE
} GXPerf1;

typedef enum _GXSpotFn {
    GX_SP_OFF,
    GX_SP_FLAT,
    GX_SP_COS,
    GX_SP_COS2,
    GX_SP_SHARP,
    GX_SP_RING1,
    GX_SP_RING2
} GXSpotFn;

typedef enum _GXTevAlphaArg {
    GX_CA_APREV,
    GX_CA_A0,
    GX_CA_A1,
    GX_CA_A2,
    GX_CA_TEXA,
    GX_CA_RASA,
    GX_CA_KONST,
    GX_CA_ZERO,
    GX_CA_ONE
} GXTevAlphaArg;

typedef enum _GXTevBias {
    GX_TB_ZERO,
    GX_TB_ADDHALF,
    GX_TB_SUBHALF,

    GX_MAX_TEVBIAS
} GXTevBias;

typedef enum _GXTevColorArg {
    GX_CC_CPREV,
    GX_CC_APREV,
    GX_CC_C0,
    GX_CC_A0,
    GX_CC_C1,
    GX_CC_A1,
    GX_CC_C2,
    GX_CC_A2,
    GX_CC_TEXC,
    GX_CC_TEXA,
    GX_CC_RASC,
    GX_CC_RASA,
    GX_CC_ONE,
    GX_CC_HALF,
    GX_CC_KONST,
    GX_CC_ZERO,
    GX_CC_TEXRRR,
    GX_CC_TEXGGG,
    GX_CC_TEXBBB,

    GX_CC_QUARTER = GX_CC_KONST
} GXTevColorArg;

typedef enum _GXTevColorChan {
    GX_CH_RED,
    GX_CH_GREEN,
    GX_CH_BLUE,
    GX_CH_ALPHA
} GXTevColorChan;

typedef enum _GXTevOp {
    GX_TEV_ADD,
    GX_TEV_SUB,

    GX_TEV_COMP_R8_GT = 8,
    GX_TEV_COMP_R8_EQ,
    GX_TEV_COMP_GR16_GT,
    GX_TEV_COMP_GR16_EQ,
    GX_TEV_COMP_BGR24_GT,
    GX_TEV_COMP_BGR24_EQ,
    GX_TEV_COMP_RGB8_GT,
    GX_TEV_COMP_RGB8_EQ,

    GX_TEV_COMP_A8_GT = GX_TEV_COMP_RGB8_GT,
    GX_TEV_COMP_A8_EQ = GX_TEV_COMP_RGB8_EQ
} GXTevOp;

typedef enum _GXTevRegID {
    GX_TEVPREV,
    GX_TEVREG0,
    GX_TEVREG1,
    GX_TEVREG2,

    GX_MAX_TEVREG
} GXTevRegID;

typedef enum _GXTevScale {
    GX_CS_SCALE_1,
    GX_CS_SCALE_2,
    GX_CS_SCALE_4,
    GX_CS_DIVIDE_2,

    GX_MAX_TEVSCALE
} GXTevScale;

typedef enum _GXTevStageID {
    GX_TEVSTAGE0,
    GX_TEVSTAGE1,
    GX_TEVSTAGE2,
    GX_TEVSTAGE3,
    GX_TEVSTAGE4,
    GX_TEVSTAGE5,
    GX_TEVSTAGE6,
    GX_TEVSTAGE7,
    GX_TEVSTAGE8,
    GX_TEVSTAGE9,
    GX_TEVSTAGE10,
    GX_TEVSTAGE11,
    GX_TEVSTAGE12,
    GX_TEVSTAGE13,
    GX_TEVSTAGE14,
    GX_TEVSTAGE15,

    GX_MAX_TEVSTAGE
} GXTevStageID;

typedef enum _GXTevSwapSel {
    GX_TEV_SWAP0,
    GX_TEV_SWAP1,
    GX_TEV_SWAP2,
    GX_TEV_SWAP3,

    GX_MAX_TEVSWAP
} GXTevSwapSel;

typedef enum _GXTevKAlphaSel {
    GX_TEV_KASEL_8_8,
    GX_TEV_KASEL_7_8,
    GX_TEV_KASEL_6_8,
    GX_TEV_KASEL_5_8,
    GX_TEV_KASEL_4_8,
    GX_TEV_KASEL_3_8,
    GX_TEV_KASEL_2_8,
    GX_TEV_KASEL_1_8,

    GX_TEV_KASEL_1 = 0,
    GX_TEV_KASEL_3_4 = 2,
    GX_TEV_KASEL_1_2 = 4,
    GX_TEV_KASEL_1_4 = 6,

    GX_TEV_KASEL_K0_R = 16,
    GX_TEV_KASEL_K1_R,
    GX_TEV_KASEL_K2_R,
    GX_TEV_KASEL_K3_R,
    GX_TEV_KASEL_K0_G,
    GX_TEV_KASEL_K1_G,
    GX_TEV_KASEL_K2_G,
    GX_TEV_KASEL_K3_G,
    GX_TEV_KASEL_K0_B,
    GX_TEV_KASEL_K1_B,
    GX_TEV_KASEL_K2_B,
    GX_TEV_KASEL_K3_B,
    GX_TEV_KASEL_K0_A,
    GX_TEV_KASEL_K1_A,
    GX_TEV_KASEL_K2_A,
    GX_TEV_KASEL_K3_A
} GXTevKAlphaSel;

typedef enum _GXTevKColorID {
    GX_KCOLOR0,
    GX_KCOLOR1,
    GX_KCOLOR2,
    GX_KCOLOR3,

    GX_MAX_KCOLOR
} GXTevKColorID;

typedef enum _GXTevKColorSel {
    GX_TEV_KCSEL_8_8,
    GX_TEV_KCSEL_7_8,
    GX_TEV_KCSEL_6_8,
    GX_TEV_KCSEL_5_8,
    GX_TEV_KCSEL_4_8,
    GX_TEV_KCSEL_3_8,
    GX_TEV_KCSEL_2_8,
    GX_TEV_KCSEL_1_8,

    GX_TEV_KCSEL_1 = 0,
    GX_TEV_KCSEL_3_4 = 2,
    GX_TEV_KCSEL_1_2 = 4,
    GX_TEV_KCSEL_1_4 = 6,

    GX_TEV_KCSEL_K0 = 12,
    GX_TEV_KCSEL_K1,
    GX_TEV_KCSEL_K2,
    GX_TEV_KCSEL_K3,
    GX_TEV_KCSEL_K0_R,
    GX_TEV_KCSEL_K1_R,
    GX_TEV_KCSEL_K2_R,
    GX_TEV_KCSEL_K3_R,
    GX_TEV_KCSEL_K0_G,
    GX_TEV_KCSEL_K1_G,
    GX_TEV_KCSEL_K2_G,
    GX_TEV_KCSEL_K3_G,
    GX_TEV_KCSEL_K0_B,
    GX_TEV_KCSEL_K1_B,
    GX_TEV_KCSEL_K2_B,
    GX_TEV_KCSEL_K3_B,
    GX_TEV_KCSEL_K0_A,
    GX_TEV_KCSEL_K1_A,
    GX_TEV_KCSEL_K2_A,
    GX_TEV_KCSEL_K3_A
} GXTevKColorSel;

typedef enum _GXTevMode {
    GX_MODULATE,
    GX_DECAL,
    GX_BLEND,
    GX_REPLACE,
    GX_PASSCLR
} GXTevMode;

typedef enum _GXTexCoordID {
    GX_TEXCOORD0,
    GX_TEXCOORD1,
    GX_TEXCOORD2,
    GX_TEXCOORD3,
    GX_TEXCOORD4,
    GX_TEXCOORD5,
    GX_TEXCOORD6,
    GX_TEXCOORD7,

    GX_MAX_TEXCOORD,
    GX_TEXCOORD_NULL = 255
} GXTexCoordID;

typedef enum _GXTexFilter {
    GX_NEAR,
    GX_LINEAR,
    GX_NEAR_MIP_NEAR,
    GX_LIN_MIP_NEAR,
    GX_NEAR_MIP_LIN,
    GX_LIN_MIP_LIN,
} GXTexFilter;

typedef enum _GXTexFmt {
    GX_TF_I4,
    GX_TF_I8,
    GX_TF_IA4,
    GX_TF_IA8,
    GX_TF_RGB565,
    GX_TF_RGB5A3,
    GX_TF_RGBA8,
    GX_TF_CMPR = 14,

    GX_CTF_R4 = 32,
    GX_CTF_RA4 = 34,
    GX_CTF_RA8 = 35,
    GX_CTF_YUVA8 = 38,
    GX_CTF_A8 = 39,
    GX_CTF_R8 = 40,
    GX_CTF_G8 = 41,
    GX_CTF_B8 = 42,
    GX_CTF_RG8 = 43,
    GX_CTF_GB8 = 44,

    GX_TF_Z8 = 17,
    GX_TF_Z16 = 19,
    GX_TF_Z24X8 = 22,

    GX_CTF_Z4 = 48,
    GX_CTF_Z8M = 57,
    GX_CTF_Z8L = 58,
    GX_CTF_Z16L = 60,

    GX_TF_A8 = GX_CTF_YUVA8
} GXTexFmt;

typedef enum _GXTexGenSrc {
    GX_TG_POS,
    GX_TG_NRM,
    GX_TG_BINRM,
    GX_TG_TANGENT,
    GX_TG_TEX0,
    GX_TG_TEX1,
    GX_TG_TEX2,
    GX_TG_TEX3,
    GX_TG_TEX4,
    GX_TG_TEX5,
    GX_TG_TEX6,
    GX_TG_TEX7,
    GX_TG_TEXCOORD0,
    GX_TG_TEXCOORD1,
    GX_TG_TEXCOORD2,
    GX_TG_TEXCOORD3,
    GX_TG_TEXCOORD4,
    GX_TG_TEXCOORD5,
    GX_TG_TEXCOORD6,
    GX_TG_COLOR0,
    GX_TG_COLOR1,
} GXTexGenSrc;

typedef enum _GXTexGenType {
    GX_TG_MTX3x4,
    GX_TG_MTX2x4,
    GX_TG_BUMP0,
    GX_TG_BUMP1,
    GX_TG_BUMP2,
    GX_TG_BUMP3,
    GX_TG_BUMP4,
    GX_TG_BUMP5,
    GX_TG_BUMP6,
    GX_TG_BUMP7,
    GX_TG_SRTG
} GXTexGenType;

typedef enum _GXTexMapID {
    GX_TEXMAP0,
    GX_TEXMAP1,
    GX_TEXMAP2,
    GX_TEXMAP3,
    GX_TEXMAP4,
    GX_TEXMAP5,
    GX_TEXMAP6,
    GX_TEXMAP7,
    GX_MAX_TEXMAP,

    GX_TEXMAP_NULL = 255,
    GX_TEX_DISABLE
} GXTexMapID;

typedef enum _GXTexMtx {
    // Any dimension (in standard XF matrix memory)
    // Enum represents base row of matrix
    GX_TEXMTX0 = 30,
    GX_TEXMTX1 = 33,
    GX_TEXMTX2 = 36,
    GX_TEXMTX3 = 39,
    GX_TEXMTX4 = 42,
    GX_TEXMTX5 = 45,
    GX_TEXMTX6 = 48,
    GX_TEXMTX7 = 51,
    GX_TEXMTX8 = 54,
    GX_TEXMTX9 = 57,
    GX_IDENTITY = 60,
} GXTexMtx;

typedef enum _GXPTTexMtx {
    // 3x4 matrices (in dual-tex / "post-matrix" XF matrix memory)
    // Enum represents base row of matrix
    GX_PTTEXMTX0 = 64,
    GX_PTTEXMTX1 = 67,
    GX_PTTEXMTX2 = 70,
    GX_PTTEXMTX3 = 73,
    GX_PTTEXMTX4 = 76,
    GX_PTTEXMTX5 = 79,
    GX_PTTEXMTX6 = 82,
    GX_PTTEXMTX7 = 85,
    GX_PTTEXMTX8 = 88,
    GX_PTTEXMTX9 = 91,
    GX_PTTEXMTX10 = 94,
    GX_PTTEXMTX11 = 97,
    GX_PTTEXMTX12 = 100,
    GX_PTTEXMTX13 = 103,
    GX_PTTEXMTX14 = 106,
    GX_PTTEXMTX15 = 109,
    GX_PTTEXMTX16 = 112,
    GX_PTTEXMTX17 = 115,
    GX_PTTEXMTX18 = 118,
    GX_PTTEXMTX19 = 121,
    GX_PTIDENTITY = 125
} GXPTTexMtx;

typedef enum _GXTexWrapMode {
    GX_CLAMP,
    GX_REPEAT,
    GX_MIRROR,

    GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

typedef enum _GXTlut {
    GX_TLUT0,
    GX_TLUT1,
    GX_TLUT2,
    GX_TLUT3,
    GX_TLUT4,
    GX_TLUT5,
    GX_TLUT6,
    GX_TLUT7,
    GX_TLUT8,
    GX_TLUT9,
    GX_TLUT10,
    GX_TLUT11,
    GX_TLUT12,
    GX_TLUT13,
    GX_TLUT14,
    GX_TLUT15,

    GX_BIGTLUT0,
    GX_BIGTLUT1,
    GX_BIGTLUT2,
    GX_BIGTLUT3,
} GXTlut;

typedef enum _GXTlutFmt {
    GX_TL_IA8,
    GX_TL_RGB565,
    GX_TL_RGB5A3,

    GX_MAX_TLUTFMT
} GXTlutFmt;

typedef enum _GXVtxFmt {
    GX_VTXFMT0,
    GX_VTXFMT1,
    GX_VTXFMT2,
    GX_VTXFMT3,
    GX_VTXFMT4,
    GX_VTXFMT5,
    GX_VTXFMT6,
    GX_VTXFMT7,

    GX_MAX_VTXFMT
} GXVtxFmt;

typedef enum _GXZFmt16 {
    GX_ZC_LINEAR,
    GX_ZC_NEAR,
    GX_ZC_MID,
    GX_ZC_FAR,
} GXZFmt16;

// From patent
typedef enum _GXZTexOp {
    GX_ZT_DISABLE,
    GX_ZT_ADD,
    GX_ZT_REPLACE,

    GX_MAX_ZTEXOP
} GXZTexOp;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void GXBegin(GXPrimitive prim, GXVtxFmt fmt, u16 verts);
static void GXEnd(void) {}

void GXSetLineWidth(u8 width, u32 offset);
void GXSetPointSize(u8 size, u32 offset);
void GXEnableTexOffsets(GXTexCoordID coordId, GXBool lineOfs, GXBool pointOfs);
void GXSetCullMode(GXCullMode mode);
void GXGetCullMode(GXCullMode* out);
void GXSetCoPlanar(GXBool coplanar);

void __GXSetDirtyState(void);
void __GXSendFlushPrim(void);
void __GXSetGenMode(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/gx/GXGeometry.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 7 "revolution/gx/GXTransform.h" */
#ifndef RVL_SDK_GX_TRANSFORM_H
#define RVL_SDK_GX_TRANSFORM_H
/* "libs/RVL_SDK/include/revolution/gx/GXTransform.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXTransform.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXTransform.h" line 6 "revolution/MTX.h" */
#ifndef RVL_SDK_PUBLIC_MTX_H
#define RVL_SDK_PUBLIC_MTX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/MTX.h" line 6 "revolution/MTX/mtx.h" */
#ifndef RVL_SDK_MTX_MTX_H
#define RVL_SDK_MTX_MTX_H
/* "libs/RVL_SDK/include/revolution/MTX/mtx.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtx.h" line 4 "revolution/MTX/mtxtypes.h" */
#ifndef RVL_SDK_MTX_TYPES_H
#define RVL_SDK_MTX_TYPES_H
/* "libs/RVL_SDK/include/revolution/MTX/mtxtypes.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef struct Vec {
    f32 x, y, z;
} Vec;

typedef struct Vec2 {
    f32 x, y;
} Vec2;

typedef struct Quaternion {
    f32 x, y, z, w;
} Quaternion;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

// TODO(kiwi) Create macros for PS/C_ functions
void PSMTXIdentity(Mtx);
void PSMTXCopy(const Mtx, Mtx);
void PSMTXConcat(const Mtx, const Mtx, Mtx);
void PSMTXConcatArray(const Mtx, const Mtx, Mtx, u32);
void PSMTXTranspose(const Mtx, Mtx);
u32 PSMTXInverse(const Mtx, Mtx);
u32 PSMTXInvXpose(const Mtx, Mtx);
void PSMTXRotRad(Mtx, char, f32);
void PSMTXRotTrig(Mtx, f32, f32, char);
void PSMTXRotAxisRad(Mtx, const Vec*, f32);
void PSMTXTrans(Mtx, f32, f32, f32);
void PSMTXTransApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXScale(Mtx, f32, f32, f32);
void PSMTXScaleApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXQuat(Mtx, const Quaternion*);

void C_MTXLookAt(Mtx, const Vec*, const Vec*, const Vec*);
void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);
void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtx.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 7 "revolution/MTX/mtx44.h" */
#ifndef RVL_SDK_MTX_MTX44_H
#define RVL_SDK_MTX_MTX44_H
/* "libs/RVL_SDK/include/revolution/MTX/mtx44.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtx44.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSMTX44Copy(Mtx44, Mtx44);
void C_MTXFrustum(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx44 mtx, f32 fovy, f32 aspect, f32 n, f32 f);
void C_MTXOrtho(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtx44.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 8 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 9 "revolution/MTX/mtxvec.h" */
#ifndef RVL_SDK_MTX_MTXVEC_H
#define RVL_SDK_MTX_MTXVEC_H
/* "libs/RVL_SDK/include/revolution/MTX/mtxvec.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtxvec.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSMTXMultVec(const Mtx mtx, const Vec* vec, Vec* out);
void PSMTXMultVecSR(const Mtx, const Vec*, Vec*);
void PSMTXMultVecArraySR();

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtxvec.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 10 "revolution/MTX/quat.h" */
#ifndef RVL_SDK_MTX_QUAT_H
#define RVL_SDK_MTX_QUAT_H
/* "libs/RVL_SDK/include/revolution/MTX/quat.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/quat.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif


void PSQUATAdd(const Quaternion* a, const Quaternion* b, Quaternion* sum);
void PSQUATMultiply(const Quaternion* a, const Quaternion* b, Quaternion* prod);
void PSQUATScale(const Quaternion* in, Quaternion* out, f32 scale);
f32 PSQUATDotProduct(const Quaternion* a, const Quaternion* b);
void PSQUATNormalize(const Quaternion* in, Quaternion* out);
void PSQUATInverse(const Quaternion* in, Quaternion* out);
void C_QUATMtx(Quaternion* quat, const Mtx mtx);
void C_QUATLerp(const Quaternion* a, const Quaternion* b, Quaternion* out,
                f32 t);
void C_QUATSlerp(const Quaternion* a, const Quaternion* b, Quaternion* out,
                 f32 t);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/quat.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 11 "revolution/MTX/vec.h" */
#ifndef RVL_SDK_MTX_VEC_H
#define RVL_SDK_MTX_VEC_H
/* "libs/RVL_SDK/include/revolution/MTX/vec.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/vec.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSVECAdd(const Vec* a, const Vec* b, Vec* sum);
void PSVECScale(const Vec* in, Vec* out, f32 scale);
void PSVECNormalize(const Vec* in, Vec* out);
f32 PSVECMag(const Vec* v);
f32 PSVECDotProduct(const Vec* a, const Vec* b);
void PSVECCrossProduct(const Vec* a, const Vec* b, Vec* prod);
f32 PSVECSquareDistance(const Vec* a, const Vec* b);
void C_VECHalfAngle(const Vec* a, const Vec* b, Vec* half);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/vec.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define GX_PROJECTION_SZ 7
#define GX_VIEWPORT_SZ 6

typedef enum {
    GX_FIELD_EVEN,
    GX_FIELD_ODD,
} GXInterlaceField;

void GXSetProjection(const Mtx44 proj, GXProjectionType type);
void GXSetProjectionv(const f32 proj[GX_PROJECTION_SZ]);
void GXGetProjectionv(f32 proj[GX_PROJECTION_SZ]);
void GXLoadPosMtxImm(const Mtx mtx, u32 id);
void GXLoadPosMtxIndx(u16 index, u32 id);
void GXLoadNrmMtxImm(const Mtx mtx, u32 id);
void GXLoadNrmMtxIndx3x3(u16 index, u32 id);
void GXSetCurrentMtx(u32 id);
void GXLoadTexMtxImm(const Mtx mtx, u32 id, GXMtxType type);
void GXSetViewportJitter(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far,
                         u32 nextField);
void GXSetViewport(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far);
void GXGetViewportv(f32 view[GX_VIEWPORT_SZ]);
void GXSetZScaleOffset(f32 scale, f32 offset);
void GXSetScissor(u32 x, u32 y, u32 w, u32 h);
void GXGetScissor(u32* x, u32* y, u32* w, u32* h);
void GXSetScissorBoxOffset(u32 ox, u32 oy);
void GXSetClipMode(GXClipMode mode);

void __GXSetProjection(void);
void __GXSetViewport(void);
void __GXSetMatrixIndex(GXAttr index);

static void GXSetViewportv(const f32 view[GX_VIEWPORT_SZ]) {
    GXSetViewport(view[0], view[1], view[2], view[3], view[4], view[5]);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/gx/GXTransform.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 8 "revolution/gx/GXTev.h" */
#ifndef RVL_SDK_GX_TEV_H
#define RVL_SDK_GX_TEV_H
/* "libs/RVL_SDK/include/revolution/gx/GXTev.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXTev.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void GXSetTevOp(GXTevStageID, GXTevMode);
void GXSetTevColorIn(GXTevStageID, GXTevColorArg, GXTevColorArg, GXTevColorArg,
                     GXTevColorArg);
void GXSetTevAlphaIn(GXTevStageID, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg,
                     GXTevAlphaArg);
void GXSetTevColorOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, GXBool,
                     GXTevRegID);
void GXSetTevAlphaOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, GXBool,
                     GXTevRegID);

void GXSetTevColor(GXTevRegID, GXColor);
void GXSetTevColorS10(GXTevRegID, GXColorS10);

void GXSetTevKColor(GXTevKColorID, GXColor);
void GXSetTevKColorSel(GXTevStageID, GXTevKColorSel);
void GXSetTevKAlphaSel(GXTevStageID, GXTevKAlphaSel);
void GXSetTevSwapMode(GXTevStageID, GXTevSwapSel, GXTevSwapSel);
void GXSetTevSwapModeTable(GXTevSwapSel, GXTevColorChan, GXTevColorChan,
                           GXTevColorChan, GXTevColorChan);

void GXSetAlphaCompare(GXCompare, u8, GXAlphaOp, GXCompare, u8);
void GXSetZTexture(GXZTexOp, GXTexFmt, u32);
void GXSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID);
void GXSetNumTevStages(u8);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/gx/GXTev.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 9 "revolution/gx/GXPixel.h" */
#ifndef RVL_SDK_GX_PIXEL_H
#define RVL_SDK_GX_PIXEL_H
/* "libs/RVL_SDK/include/revolution/gx/GXPixel.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXPixel.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXPixel.h" line 6 "revolution/MTX.h" */
/* end "revolution/MTX.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXFogAdjTable {
    u16 r[10]; // at 0x0
} GXFogAdjTable;

#define GX_FOG_ADJ_TABLE_SZ 10

void GXSetFog(GXFogType type, GXColor color, f32 start, f32 end, f32 near,
              f32 far);
void GXInitFogAdjTable(GXFogAdjTable* table, u16 width, const Mtx44 proj);
void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable* table);
void GXSetBlendMode(GXBlendMode mode, GXBlendFactor src, GXBlendFactor dst,
                    GXLogicOp op);
void GXSetColorUpdate(GXBool enable);
void GXSetAlphaUpdate(GXBool enable);
void GXSetZMode(GXBool enableTest, GXCompare func, GXBool enableUpdate);
void GXSetZCompLoc(GXBool beforeTex);
void GXSetPixelFmt(GXPixelFmt pixelFmt, GXZFmt16 zFmt);
void GXSetDither(GXBool enable);
void GXSetDstAlpha(GXBool enable, u8 alpha);
void GXSetFieldMask(GXBool enableEven, GXBool enableOdd);
void GXSetFieldMode(GXBool texLOD, GXBool adjustAR);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/gx/GXPixel.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 10 "revolution/gx/GXAttr.h" */
#ifndef RVL_SDK_GX_ATTR_H
#define RVL_SDK_GX_ATTR_H
/* "libs/RVL_SDK/include/revolution/gx/GXAttr.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXAttr.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXVtxDescList {
    GXAttr attr;     // at 0x0
    GXAttrType type; // at  0x4
} GXVtxDescList;

typedef struct _GXVtxAttrFmtList {
    GXAttr attr;         // at 0x0
    GXCompCnt compCnt;   // at 0x4
    GXCompType compType; // at 0x8
    u8 shift;            // at 0xC
} GXVtxAttrFmtList;

void GXSetVtxDesc(GXAttr name, GXAttrType type);
void GXSetVtxDescv(const GXVtxDescList* list);
void GXGetVtxDesc(GXAttr name, GXAttrType* type);
void GXGetVtxDescv(GXVtxDescList* list);
void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt compCnt,
                     GXCompType compType, u8 shift);

// TODO(kiwi) Please find a way to get rid of this
#if defined(GXATTR_MATCH_HACK)
void GXSetVtxAttrFmtv(s16 fmt, const GXVtxAttrFmtList* list);
#else
void GXSetVtxAttrFmtv(GXVtxFmt fmt, const GXVtxAttrFmtList* list);
#endif

void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt* compCnt,
                     GXCompType* compType, u8* shift);
void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList* list);
void GXSetArray(GXAttr attr, const void* base, u8 stride);
void GXInvalidateVtxCache(void);
void GXSetTexCoordGen2(GXTexCoordID id, GXTexGenType type, GXTexGenSrc src,
                       u32 texMtxIdx, GXBool normalize, u32 dualTexMtxIdx);
void GXSetNumTexGens(u8 num);

void __GXSetVCD(void);
void __GXCalculateVLim(void);
void __GXSetVAT(void);

static void GXSetTexCoordGen(GXTexCoordID id, GXTexGenType type,
                             GXTexGenSrc src, u32 texMtxIdx) {
    GXSetTexCoordGen2(id, type, src, texMtxIdx, FALSE, GX_PTIDENTITY);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/gx/GXAttr.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 11 "revolution/gx/GXLight.h" */
#ifndef RVL_SDK_GX_LIGHT_H
#define RVL_SDK_GX_LIGHT_H
/* "libs/RVL_SDK/include/revolution/gx/GXLight.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXLight.h" line 4 "revolution/GX/GXInternal.h" */
#ifndef RVL_SDK_GX_INTERNAL_H
#define RVL_SDK_GX_INTERNAL_H
/* "libs/RVL_SDK/include/revolution/GX/GXInternal.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXInternal.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * GX internal structures.
 *
 * Wouldn't be necessary if the public ones didn't include padding; but they do,
 * so there has to be different structure definitions.
 *
 * These internal structures are implemented like the RFL ones since we don't
 * have DWARF info for most GX structures.
 */

/**
 * Declare a public structure from the corresponding internal structure.
 * (Implementation size is included to require that such a structure already
 * exists.)
 */
#define GX_PUBLIC_STRUCT_DECL(name, size)                                      \
    typedef struct _##name {                                                   \
        u32 dummy[((size) - sizeof(name##Impl) + sizeof(name##Impl)) /         \
                  sizeof(u32)];                                                \
    } name;

typedef struct _GXFifoObjImpl {
    void* base;        // at 0x0
    void* end;         // at 0x4
    u32 size;          // at 0x8
    void* hiWatermark; // at 0xC
    void* loWatermark; // at 0x10
    void* readPtr;     // at 0x14
    void* writePtr;    // at 0x18
    u32 count;         // at 0x1C
    u8 wrap;           // at 0x20
    u8 bind_cpu;       // at 0x21
    u8 bind_gp;        // at 0x22
    u8 pad;            // at 0x23
} GXFifoObjImpl;

typedef struct _GXLightObjImpl {
    char UNK_0x0[0xC];
    GXColor color; // at 0xC
    f32 aa;        // at 0x10
    f32 ab;        // at 0x14
    f32 ac;        // at 0x18
    f32 ka;        // at 0x1C
    f32 kb;        // at 0x20
    f32 kc;        // at 0x24
    f32 posX;      // at 0x28
    f32 posY;      // at 0x2C
    f32 posZ;      // at 0x30
    f32 dirX;      // at 0x34
    f32 dirY;      // at 0x38
    f32 dirZ;      // at 0x3C
} GXLightObjImpl;

typedef struct _GXTexObjImpl {
    u32 mode0;
    u32 mode1;
    u32 image0;
    u32 image3;
    void* userData;
    GXTexFmt fmt;
    u32 tlutName;
    u16 loadCnt;
    u8 loadFmt;
    u8 flags;
} GXTexObjImpl;

typedef struct _GXTlutObjImpl {
    u32 tlut;
    u32 loadTlut0;
    u16 numEntries;
} GXTlutObjImpl;

typedef struct _GXTexRegionImpl {
    u32 image1;
    u32 image2;
    u16 sizeEven;
    u16 sizeOdd;
    u8 is32bMipmap;
    u8 isCached;
} GXTexRegionImpl;

typedef struct _GXTlutRegionImpl {
    u32 loadTlut1;
    GXTlutObjImpl tlutObj;
} GXTlutRegionImpl;

#define GX_SETUP_TEXOBJ(l, p) GXTexObjImpl* l = (GXTexObjImpl*)(p);

#define GX_SETUP_ALL_TEXOBJS(l, p, m, q) \
    GXTexObjImpl* l = (GXTexObjImpl*)(p); \
    GXTexRegionImpl* m = (GXTexRegionImpl*)(q);

#define GX_SETUP_TLUTOBJ(l, p) GXTlutObjImpl* l = (GXTlutObjImpl*)(p);

#define GX_SETUP_TREGOBJ(l, p) GXTexRegionImpl* l = (GXTexRegionImpl*)(p);

#define GX_SETUP_TLUTREGOBJ(l, p) GXTlutRegionImpl* l = (GXTlutRegionImpl*)(p);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/gx/GXLight.h" line 5 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

GX_PUBLIC_STRUCT_DECL(GXLightObj, 64);

void GXInitLightAttn(GXLightObj* light, f32 aa, f32 ab, f32 ac, f32 ka, f32 kb,
                     f32 kc);
void GXInitLightAttnA(GXLightObj* light, f32 a, f32 b, f32 c);
void GXInitLightAttnK(GXLightObj* light, f32 a, f32 b, f32 c);
void GXInitLightSpot(GXLightObj* light, f32 angle, GXSpotFn fn);
void GXInitLightDistAttn(GXLightObj* light, f32 distance, f32 brightness,
                         GXDistAttnFn fn);
void GXInitLightPos(GXLightObj* light, f32 x, f32 y, f32 z);
void GXGetLightPos(const GXLightObj* light, f32* x, f32* y, f32* z);
void GXInitLightDir(GXLightObj* light, f32 x, f32 y, f32 z);
void GXGetLightDir(const GXLightObj* light, f32* x, f32* y, f32* z);
void GXInitSpecularDir(GXLightObj* light, f32 x, f32 y, f32 z);
void GXInitLightColor(GXLightObj* light, GXColor color);
void GXLoadLightObjImm(const GXLightObj* light, GXLightID id);
void GXLoadLightObjIndx(u16 index, GXLightID id);
void GXSetChanAmbColor(GXChannelID chan, GXColor color);
void GXSetChanMatColor(GXChannelID chan, GXColor color);
void GXSetNumChans(u8 num);
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc ambSrc,
                   GXColorSrc matSrc, GXLightID lightMask, GXDiffuseFn diffFn,
                   GXAttnFn attnFn);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/gx/GXLight.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 12 "revolution/gx/GXVert.h" */
#ifndef RVL_SDK_GX_VERT_H
#define RVL_SDK_GX_VERT_H
/* "libs/RVL_SDK/include/revolution/gx/GXVert.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/gx/GXVert.h" line 4 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
#ifdef __cplusplus
extern "C" {
#endif

static inline void GXCmd1u8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXCmd1u16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXCmd1u32(u32 ul) {
    WGPIPE.ui = ul;
}

static inline void GXPosition3f32(f32 x, f32 y, f32 z) {
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
}

static inline void GXPosition3u8(u8 x, u8 y, u8 z) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
    WGPIPE.uc = z;
}

static inline void GXPosition3s8(s8 x, s8 y, s8 z) {
    WGPIPE.c = x;
    WGPIPE.c = y;
    WGPIPE.c = z;
}

static inline void GXPosition3u16(u16 x, u16 y, u16 z) {
    WGPIPE.us = x;
    WGPIPE.us = y;
    WGPIPE.us = z;
}

static inline void GXPosition3s16(s16 x, s16 y, s16 z) {
    WGPIPE.s = x;
    WGPIPE.s = y;
    WGPIPE.s = z;
}

static inline void GXPosition2f32(f32 x, f32 y) {
    WGPIPE.f = x;
    WGPIPE.f = y;
}

static inline void GXPosition2u8(u8 x, u8 y) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
}

static inline void GXPosition2s8(s8 x, s8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXPosition2u16(u16 x, u16 y) {
    WGPIPE.us = x;
    WGPIPE.us = y;
}

static inline void GXPosition2s16(s16 x, s16 y) {
    WGPIPE.s = x;
    WGPIPE.s = y;
}

static inline void GXPosition1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXPosition1x8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXNormal3f32(f32 x, f32 y, f32 z) {
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
}

static inline void GXNormal3u16(s16 x, s16 y, s16 z) {
    WGPIPE.us = x;
    WGPIPE.us = y;
    WGPIPE.us = z;
}

static inline void GXNormal3s16(s16 x, s16 y, s16 z) {
    WGPIPE.s = x;
    WGPIPE.s = y;
    WGPIPE.s = z;
}

static inline void GXNormal3u8(u8 x, u8 y, u8 z) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
    WGPIPE.uc = z;
}

static inline void GXNormal3s8(s8 x, s8 y, s8 z) {
    WGPIPE.c = x;
    WGPIPE.c = y;
    WGPIPE.c = z;
}

static inline void GXNormal1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXNormal1x8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXColor4u8(u8 r, u8 g, u8 b, u8 a) {
    WGPIPE.uc = r;
    WGPIPE.uc = g;
    WGPIPE.uc = b;
    WGPIPE.uc = a;
}

static inline void GXColor1u32(u32 color) {
    WGPIPE.ui = color;
}

static inline void GXColor3u8(u8 r, u8 g, u8 b) {
    WGPIPE.uc = r;
    WGPIPE.uc = g;
    WGPIPE.uc = b;
}

static inline void GXColor1u16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXColor1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXColor1x8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXTexCoord2f32(f32 x, f32 y) {
    WGPIPE.f = x;
    WGPIPE.f = y;
}

static inline void GXTexCoord2s16(s16 x, s16 y) {
    WGPIPE.s = x;
    WGPIPE.s = y;
}

static inline void GXTexCoord2u16(u16 x, u16 y) {
    WGPIPE.us = x;
    WGPIPE.us = y;
}

static inline void GXTexCoord2s8(s8 x, s8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXTexCoord2u8(u8 x, u8 y) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
}

static inline void GXTexCoord1f32(f32 f) {
    WGPIPE.f = f;
}

static inline void GXTexCoord1s16(s16 s) {
    WGPIPE.s = s;
}

static inline void GXTexCoord1u16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXTexCoord1s8(s8 c) {
    WGPIPE.c = c;
}

static inline void GXTexCoord1u8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXTexCoord1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXTexCoord1x8(u8 uc) {
    WGPIPE.uc = uc;
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/gx/GXVert.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 13 "revolution/mtx/mtx.h" */
/* end "revolution/mtx/mtx.h" */
/* "libs/monolib/src/scn/code_804BC9EC.cpp" line 14 "revolution/mtx/vec.h" */
/* end "revolution/mtx/vec.h" */

// Entry in the top-level chunk list (8 bytes each)
struct ScnResEntry {
    u16 type;
    u16 field_0x2;
    u32 offset;  // relative to data base
};

// Extended ScnResData with named fields for offsets 0x00-0x60.
// Layout-compatible with ScnResData from the header.
struct ScnResDataEx {
    void* field_0x00;  // set by func_804BC9B4 (base + src[1])
    void* field_0x04;  // set by func_804BC9B4 (base + src[2])
    void* field_0x08;  // set by func_804BC9B4 (base + src[3])
    u8 field_0x0C[0x14 - 0x0C];
    void* field_0x14;  // base pointer to parsed data
    void* field_0x18;  // entry type 1
    void* field_0x1C;  // entry type 3
    void* field_0x20;  // entry type 2
    void* field_0x24;  // entry type 5
    void* field_0x28;  // entry type 4
    void* field_0x2C;  // entry type 0: data + 0x20
    void* field_0x30;  // entry type 0: data + 0x20 (same as 0x2C)
    void* field_0x34;  // entry type 0: data pointer
    void* field_0x38;  // entry type 6
    void* field_0x3C;  // entry type 7
    void* field_0x40;  // entry type 8
    u8 field_0x44[0x48 - 0x44];
    void* field_0x48;  // entry type 9
    void* field_0x4C;  // entry type 12
    u8 field_0x50[0x60 - 0x50];
    void* field_0x60;  // array of 0xE0-byte elements
    u8 field_0x64[0x6C - 0x64];
    u32 field_0x6C;    // entry type 0: u16 count
    u32 field_0x70;    // entry type 7: u16 value
    u8 field_0x74[0x78 - 0x74];
    int count;         // 0x78
};

// Additional extern function declarations
void func_804BC9B4(int* dest, int baseOffset, int* src);
void func_804B74F0(ScnResData* res, void* data);
void func_804B7540(ScnResData* res, void* data, u16 param);
void* func_804B5A70(void);

// Vertex: 12 bytes (3 floats: x, y, z)
struct SceneVertex {
    f32 x, y, z;
};

// Triangle element: 20 bytes (0x14)
struct SceneTriElem {
    u16 field_0x00;
    u16 field_0x02;
    u16 indexA;   // 0x04
    u16 indexB;   // 0x06
    u16 indexC;   // 0x08
    u8 colorIdx;  // 0x0C
    u8 field_0x0D;
    u8 field_0x0E;
    u8 field_0x0F;
    u16 field_0x10;
    u16 flags;    // 0x12: bit 17 = red?, bit 22 = green?, bit 18 = blue?
};

// Type 0x0B sub-element: 4 bytes
struct SceneVisElem {
    u16 field_0x00;
    u16 field_0x02;  // visibility bitfield index
};

// Type 0x13 element: 0x3C bytes (bounding sphere + transform)
struct SceneCullElem {
    u16 index;       // 0x00: index into geometry data
    u16 visIdx;      // 0x02: visibility bitfield index
    u16 field_0x04;
    u16 field_0x06;
    u16 field_0x08;
    u16 field_0x0A;
    f32 sphereX;     // 0x0C
    f32 sphereY;     // 0x10
    f32 sphereZ;     // 0x14
    f32 sphereR;     // 0x18
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
};

// Float constants from SDA2
// extern const f32 lbl_eu_8066AF10; // 1/3
// extern const f32 lbl_eu_8066AF14; // 0.0f
// extern const f32 lbl_eu_8066AF18; // 255.0f

// nw4r math functions
extern "C" {
float FrSqrt__Q24nw4r4mathFf(float);
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
}

// String constants for warning
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

extern u32 lbl_eu_80665968;
extern "C" void __dt__804BD8E8();
extern u8 lbl_eu_8065F418[0x10];
int func_804BC9EC__Fv(void* self) { return (int)(intptr_t)&lbl_eu_80665968; }

int func_804BC9F4(void* outStruct, u32 data) {
    *(u32*)outStruct = 0;
    func_804B7804(&lbl_eu_8065F32C);

    if (data == 0) {
        return 0;
    }

    u8* base = (u8*)data;
    u32 magic1 = *(u32*)base;

    // Check for "WP" container chunk: upper 16 bits = 0x5750, lower 16 bits <= 0x4F49
    if (magic1 - 0x57500000 <= 0x4F49) {
        base += *(u32*)(base + 8);
    }

    magic1 = *(u32*)base;
    // Check for "ID" data chunk: upper 16 bits = 0x4944, lower 16 bits <= 0x4445
    if (magic1 - 0x49440000 > 0x4445) {
        return 0;
    }

    if (*(u16*)(base + 4) != 0x3EA) {
        return 0;
    }

    ScnResDataEx* res = (ScnResDataEx*)&lbl_eu_8065F32C;
    res->field_0x14 = base;

    u16 entryCount = *(u16*)(base + 6);
    ScnResEntry* entries = (ScnResEntry*)(base + 8);

    int i = 0;
    goto check;

loop:
    switch (entries[i].type) {
    case 0: {
        u8* dataPtr = base + entries[i].offset;
        res->field_0x2C = dataPtr + 0x20;
        res->field_0x30 = dataPtr + 0x20;
        res->field_0x6C = entries[i].field_0x2;
        res->field_0x34 = dataPtr;
        break;
    }
    case 1:
        res->field_0x18 = base + entries[i].offset;
        break;
    case 2:
        res->field_0x20 = base + entries[i].offset;
        break;
    case 3:
        res->field_0x1C = base + entries[i].offset;
        break;
    case 4:
        res->field_0x28 = base + entries[i].offset;
        break;
    case 5:
        res->field_0x24 = base + entries[i].offset;
        break;
    case 6:
        res->field_0x38 = base + entries[i].offset;
        break;
    case 7:
        res->field_0x3C = base + entries[i].offset;
        res->field_0x70 = entries[i].field_0x2;
        break;
    case 8:
        res->field_0x40 = base + entries[i].offset;
        break;
    case 9:
        res->field_0x48 = base + entries[i].offset;
        break;
    case 10:
        func_804BC9B4((int*)res, (int)base, (int*)(base + entries[i].offset));
        break;
    case 11:
        func_804B74F0((ScnResData*)res, base + entries[i].offset);
        break;
    case 12:
        res->field_0x4C = base + entries[i].offset;
        break;
    case 13:
        func_804B7540((ScnResData*)res, base + entries[i].offset, entries[i].field_0x2);
        break;
    }
    i++;
check:
    if (i < entryCount) {
        goto loop;
    }

    return 1;
}

void func_804BCC10(){}

void func_804BCC1C(void* p) {
    *(u32*)p = 0;
    func_804B7804(&lbl_eu_8065F32C);
}

void func_804BCC30(){}

void func_804BCC3C(){}

void func_804BCC48(){}

void func_804BCC54(){}

void func_804BCC60(){}

void func_804BCC6C() {
    func_804B80CC(&lbl_eu_8065F32C);
}

void func_804BCC78(void* modelMtx, void* viewData, void* renderParams) {
    ScnResDataEx* res = (ScnResDataEx*)&lbl_eu_8065F32C;
    u8* base = (u8*)res->field_0x14;
    if (base == NULL) {
        return;
    }

    f32* rp = (f32*)renderParams;
    f32* vf = (f32*)viewData;

    u16 entryCount = *(u16*)(base + 6);
    u8* entries = base + 8;

    Mtx mv;
    mv[0][0] = vf[0];  mv[0][1] = vf[1];  mv[0][2] = vf[2];  mv[0][3] = vf[3];
    mv[1][0] = vf[4];  mv[1][1] = vf[5];  mv[1][2] = vf[6];  mv[1][3] = vf[7];
    mv[2][0] = vf[8];  mv[2][1] = vf[9];  mv[2][2] = vf[10]; mv[2][3] = vf[11];

    Mtx44 projMtx;
    projMtx[0][0] = rp[0];  projMtx[0][1] = rp[1];  projMtx[0][2] = rp[2];  projMtx[0][3] = rp[3];
    projMtx[1][0] = rp[4];  projMtx[1][1] = rp[5];  projMtx[1][2] = rp[6];  projMtx[1][3] = rp[7];
    projMtx[2][0] = rp[8];  projMtx[2][1] = rp[9];  projMtx[2][2] = rp[10]; projMtx[2][3] = rp[11];
    projMtx[3][0] = rp[12]; projMtx[3][1] = rp[13]; projMtx[3][2] = rp[14]; projMtx[3][3] = rp[15];

    PSMTXInverse(mv, mv);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(mv, GX_PNMTX0);

    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_AND, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    f32 camX = vf[3];
    f32 camY = vf[7];
    f32 camZ = vf[11];

    SceneVertex* vertBase = (SceneVertex*)res->field_0x18;

    // First pass: find type 0x10 and 0x08 entry data
    u8* colorData = NULL;
    u32* flagData = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type == 0x10) {
            colorData = base + e->offset;
        } else if (e->type == 0x08) {
            flagData = (u32*)(base + e->offset);
        }
    }

    f32 oneThird = 1.0f / 3.0f;
    f32 zero = 0.0f;

    // Second pass: render type 1 entries  
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type != 1) continue;

        u8* data = base + e->offset;
        u16 triCount = *(u16*)(data + 2);
        SceneTriElem* tris = (SceneTriElem*)(data + 0x20);

        for (int j = 0; j < triCount; j++) {
            SceneTriElem* tri = &tris[j];
            SceneVertex* va = &vertBase[tri->indexA];
            SceneVertex* vb = &vertBase[tri->indexB];
            SceneVertex* vc = &vertBase[tri->indexC];

            f32 cx = (va->x + vb->x + vc->x) * oneThird;
            f32 cy = (va->y + vb->y + vc->y) * oneThird;
            f32 cz = (va->z + vb->z + vc->z) * oneThird;

            f32 dx0 = cx - va->x, dy0 = cy - va->y, dz0 = cz - va->z;
            f32 r2 = dx0 * dx0 + dy0 * dy0 + dz0 * dz0;

            f32 dx1 = cx - vb->x, dy1 = cy - vb->y, dz1 = cz - vb->z;
            f32 t2 = dx1 * dx1 + dy1 * dy1 + dz1 * dz1;
            if (t2 > r2) r2 = t2;

            f32 dx2 = cx - vc->x, dy2 = cy - vc->y, dz2 = cz - vc->z;
            t2 = dx2 * dx2 + dy2 * dy2 + dz2 * dz2;
            if (t2 > r2) r2 = t2;

            if (r2 != zero) {
                if (r2 < zero) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273, lbl_eu_80526300);
                }
                f32 r = (r2 < zero) ? zero : FrSqrt__Q24nw4r4mathFf(r2) * r2;

                f32 dcamX = camX - cx, dcamY = camY - cy, dcamZ = camZ - cz;
                f32 dist2 = dcamX * dcamX + dcamY * dcamY + dcamZ * dcamZ;

                u32 flg = tri->flags;
                if (dist2 > r * r && !(flg & 0x20000)) {
                    continue;
                }
            }

            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
            for (int k = 0; k < 3; k++) {
                const SceneVertex* v = (k == 0) ? va : ((k == 1) ? vb : vc);
                GXPosition3f32(v->x, v->y, v->z);

                u32 flg = tri->flags;
                if (flg & 0x20000) {
                    GXColor3u8(0xFF, 0, 0);
                } else if (flg & 0x400000) {
                    GXColor3u8(0, 0xFF, 0);
                } else if (flg & 0x40000) {
                    GXColor3u8(0, 0, 0xFF);
                } else if (colorData != NULL) {
                    f32* colors = (f32*)(colorData + tri->colorIdx * 0xC);
                    GXColor3u8((u8)(colors[0] * 255.0f), (u8)(colors[1] * 255.0f), (u8)(colors[2] * 255.0f));
                }
            }
        }
    }

    // Third pass: render type 0x0B entries
    u8* visData = NULL;
    u16 visCount = 0;
    u8* visGeomBase = NULL;
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type == 2) {
            visGeomBase = base + e->offset;
        } else if (e->type == 0x0B) {
            visData = base + e->offset;
            visCount = e->field_0x2;
        }
    }

    // Fourth pass: render type 0x13 entries
    for (int i = 0; i < entryCount; i++) {
        ScnResEntry* e = &((ScnResEntry*)entries)[i];
        if (e->type != 0x13) continue;

        u16 cullCount = e->field_0x2;
        u8* cullBase = base + e->offset;
        u8* cullRef = (u8*)res->field_0x60;

        for (int j = 0; j < cullCount; j++) {
            SceneCullElem* ce = (SceneCullElem*)(cullBase + j * 0x3C);

            u32 visIdx = ce->visIdx;
            u32* bits = (u32*)func_804B5A70();
            if (bits[visIdx >> 5] & (1 << (visIdx & 0x1F))) continue;

            f32* cf = (f32*)(cullRef + j * 0xE0);
            f32 sx = cf[5];  /* 0x14/4 */
            f32 sy = cf[9];  /* 0x24/4 */
            f32 sz = cf[13]; /* 0x34/4 */
            f32 dx = camX - sx, dy = camY - sy, dz = camZ - sz;
            f32 dist2 = dx * dx + dy * dy + dz * dz;

            Mtx localMtx;
            localMtx[0][0] = cf[2];  localMtx[0][1] = cf[3];  localMtx[0][2] = cf[4];  localMtx[0][3] = cf[5];
            localMtx[1][0] = cf[6];  localMtx[1][1] = cf[7];  localMtx[1][2] = cf[8];  localMtx[1][3] = cf[9];
            localMtx[2][0] = cf[10]; localMtx[2][1] = cf[11]; localMtx[2][2] = cf[12]; localMtx[2][3] = cf[13];

            Mtx concatMtx;
            PSMTXConcat(mv, localMtx, concatMtx);
            GXLoadPosMtxImm(concatMtx, GX_PNMTX0);

            u16 geomIdx = ce->index;
            u8* geomEntry = visGeomBase + geomIdx * 0xC;
            u16 triCount = *(u16*)(geomEntry + 2);
            u8* geomTris = base + *(u32*)(geomEntry + 8);

            GXBegin(GX_TRIANGLES, GX_VTXFMT0, triCount * 3);
            for (int t = 0; t < triCount; t++) {
                SceneTriElem* tri = (SceneTriElem*)(geomTris + t * 0x14);
                for (int k = 0; k < 3; k++) {
                    SceneVertex* v = &vertBase[(k == 0) ? tri->indexA : ((k == 1) ? tri->indexB : tri->indexC)];
                    GXPosition3f32(v->x, v->y, v->z);
                    if (colorData != NULL) {
                        f32* colors = (f32*)(colorData + tri->colorIdx * 0xC);
                        GXColor3u8((u8)(colors[0] * 255.0f), (u8)(colors[1] * 255.0f), (u8)(colors[2] * 255.0f));
                    }
                }
            }
        }
    }
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BD8A0() {
    lbl_eu_80665968 = 0;
    func_804B7804(&lbl_eu_8065F32C);
    void* const obj = &lbl_eu_80665968;
    void* const dtor = (void*)&__dt__804BD8E8;
    void* const reg = &lbl_eu_8065F418;
    __register_global_object(obj, dtor, reg);
}
