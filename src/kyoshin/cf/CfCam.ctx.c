/* "src/kyoshin/cf/CfCam.cpp" line 0 "kyoshin/cf/CfCam.hpp" */
#pragma once

/* "src/kyoshin/cf/CfCam.hpp" line 2 "types.h" */
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

namespace cf {

// Minimal layout for cf::CfCamFollow ctor (__ct__cf_CfCamFollow).
// Base cf::CfCam occupies +0x00..+0x10; follow extends through ~0x25A.
struct CfCamFollow {
    void* vtable; // 0x00
    u32 unk04;    // 0x04
    u32 unk08;    // 0x08
    void* unk0C;  // 0x0C base ctor arg
    u8 unk10[0x1C - 0x10];
    u8 unk1C[0x70 - 0x1C];
    u8 unk70[0xC0]; // 0x70..0x130
    u8 unk130[0x160 - 0x130];
    void* unk160; // 0x160 ctor arg2
    void* unk164; // 0x164 ctor arg1
    u8 unk168[0x1D4 - 0x168];
    u32 unk1D4; // 0x1D4
    u16 unk1D8; // 0x1D8
    u8 unk1DA[0x1DC - 0x1DA];
    f32 unk1DC;
    f32 unk1E0;
    f32 unk1E4;
    f32 unk1E8;
    f32 unk1EC;
    f32 unk1F0;
    f32 unk1F4;
    f32 unk1F8;
    f32 unk1FC;
    f32 unk200;
    f32 unk204;
    u8 unk208[0x21C - 0x208];
    f32 unk21C;
    f32 unk220;
    f32 unk224;
    f32 unk228;
    f32 unk22C;
    f32 unk230;
    f32 unk234;
    f32 unk238;
    u8 unk23C[0x24C - 0x23C];
    u32 unk24C;
    u16 unk250;
    u16 unk252;
    u16 unk254;
    u16 unk256;
    u16 unk258;
};

} // namespace cf

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1, void* arg2);
/* end "kyoshin/cf/CfCam.hpp" */

extern "C" {
void* __ct__cf_CfCam(void* self, void* arg);
void func_8004B0B0(void* self);
void func_8004B60C(void* self, f32 x, f32 y, f32 z);
void func_8006BEF8(void* self);
void func_8006BEC0(void* self);
void func_8006BEC4(void* self);
void func_8006BEE4(void* self);
void func_8006BEFC(void* self);
void func_8006BF08(void* self);
void* func_8006BF14(void* self, int index);
void* memset(void* dest, int val, u32 count);

char lbl_eu_80527260[];
cf::CfCamFollow* lbl_eu_80663DEC;

extern const f32 lbl_eu_806662DC; // 0.0f
extern const f32 lbl_eu_806662B8; // 8.0f
extern f32 lbl_eu_80661B50;       // 40.0f (.sdata)
extern const f32 lbl_eu_806662A0; // 0.1f
extern const f32 lbl_eu_806662F0; // 0.4f
extern const f32 lbl_eu_806662D0; // 1.0f
}

typedef void (*CfCamVFn_ui)(cf::CfCamFollow* self, u32 arg);
typedef void (*CfCamVFn)(cf::CfCamFollow* self);

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1,
                                                 void* arg2) {
    u8* base = reinterpret_cast<u8*>(self);

    __ct__cf_CfCam(self, arg2);
    self->vtable = lbl_eu_80527260;

    func_8004B0B0(base + 0x1C);
    func_8004B60C(base + 0x28, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(base + 0x34, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(base + 0x40);
    func_8004B0B0(base + 0x4C);
    func_8004B0B0(base + 0x58);
    func_8004B60C(base + 0x64, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8006BEF8(base + 0x70);
    func_8006BEC0(base + 0x130);

    self->unk164 = arg1;

    func_8004B0B0(base + 0x168);
    func_8004B0B0(base + 0x174);
    func_8004B60C(base + 0x180, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(base + 0x18C, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(base + 0x198);
    func_8004B0B0(base + 0x1A4);
    func_8004B0B0(base + 0x1B0);
    func_8004B0B0(base + 0x1BC);
    func_8004B60C(base + 0x1C8, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8006BEFC(base + 0x1D4);
    func_8006BF08(base + 0x1D8);

    self->unk1DC = lbl_eu_806662DC;
    self->unk1E0 = lbl_eu_80661B50;
    self->unk1E4 = lbl_eu_806662DC;
    self->unk1E8 = lbl_eu_806662DC;
    self->unk1EC = lbl_eu_806662DC;
    self->unk1F0 = lbl_eu_806662DC;
    self->unk1F4 = lbl_eu_806662DC;
    self->unk1F8 = lbl_eu_806662DC;
    self->unk1FC = lbl_eu_806662B8;
    self->unk200 = lbl_eu_806662DC;
    self->unk204 = lbl_eu_806662A0;
    self->unk21C = lbl_eu_806662DC;
    self->unk220 = lbl_eu_806662DC;
    self->unk224 = lbl_eu_806662DC;
    self->unk228 = lbl_eu_806662F0;
    self->unk234 = lbl_eu_806662DC;
    self->unk238 = lbl_eu_806662DC;
    self->unk24C = 0;
    self->unk252 = 0;
    self->unk254 = 0;
    self->unk256 = 0;
    self->unk258 = 0;

    lbl_eu_80663DEC = self;
    self->unk160 = arg2;

    func_8006BEC4(base + 0x10);
    func_8006BEC4(base + 0x1A4);
    func_8006BEC4(base + 0x1B0);
    func_8006BEC4(base + 0x1BC);
    func_8006BEE4(self);
    memset(func_8006BF14(base + 0x70, 0), 0, 0xC0);

    self->unk250 = 0;
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;

    reinterpret_cast<CfCamVFn_ui>((*reinterpret_cast<void***>(self))[0x40 / 4])(self, 1);
    reinterpret_cast<CfCamVFn>((*reinterpret_cast<void***>(self))[0x28 / 4])(self);

    return self;
}

extern "C" void* func_8006B6A0(void* self) { return (char*)self + 0x10; }
extern "C" u32 func_8006B6A8(const void* self) { return *(const u32*)((const char*)self + 0x164); }
extern "C" void* func_8006B6B0(void* ptr) { return static_cast<char*>(ptr) + 0x40; }
extern "C" void* func_8006B6B8(void* self) { return (char*)self + 0x1c; }
extern "C" bool func_8006B6C0() { return true; }
extern "C" void func_8006BA80() {}
extern "C" float func_8006BAF0(void* self) {
    return __fabs(*(float*)self);
}
extern "C" float PSVECMag(const float* v);
extern "C" float func_8006BB00(const float* v) { return PSVECMag(v); }
extern "C" void func_8006BB04() {}
extern "C" void func_8006BBF4(void* r3, int r4, int r5) {
    if (r5) {
        *(int*)((char*)r3 + 4) |= r4;
    } else {
        *(int*)((char*)r3 + 4) &= ~r4;
    }
}
extern "C" void func_8006BC1C(unsigned int* p, unsigned int mask) {
    p[1] &= ~mask;
}
extern "C" void __ct__Q22cf5CfCamFv() {}
extern "C" void func_8006BEC0(void* self) {}
extern "C" void func_8006BEC4(void* self) {}
extern "C" void func_8006BEE4(void* p) {
    ((u32*)p)[1] = 0;
}
extern "C" void func_8006BEF0(void* self, int val) { *(int*)((char*)self + 0x8) = val; }
extern "C" void func_8006BEF8(void* self) {}
void func_8006BEFC(void* param_1) { *(u32*)param_1 = 0; }
extern "C" void func_8006BF08(void* ptr) {
    *(short*)ptr = 0;
}
void* func_8006BF14(void* r3, int r4) { return (char*)r3 + r4 * 12; }
extern "C" u32 func_8006BFBC(void *self) { return *(u32 *)((char *)self + 8); }
extern "C" bool func_8006BFC4(const void* self, unsigned int mask) { return ((*reinterpret_cast<const unsigned int*>(static_cast<const unsigned char*>(self) + 4)) & mask) != 0; }
extern "C" int func_8006C1B0(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
extern "C" void func_8006C1BC(void* ptr, float f) {
    lbl_eu_80661B50 = f;
    *(float*)((u8*)ptr + 0x1e0) = f;
}
extern "C" void func_8006C640(void* obj, unsigned int bits, int set_flag) {
    unsigned int* ptr = (unsigned int*)((char*)obj + 0x1d4);
    if (set_flag != 0)
        *ptr |= bits;
    else
        *ptr &= ~bits;
}
extern "C" int func_8006C670(void* self) { return *(int*)((char*)self + 0xc4); }
extern "C" int func_8006C678(void* p) {
    return *(unsigned short*)((unsigned char*)p + 0x530) & 1;
}
extern "C" bool func_8006C684(const unsigned char* this_) { return (*(const unsigned short*)(this_ + 0x530) & 0x000C) != 0; }
extern "C" bool func_8006C69C(const void* self) { return (*(const unsigned short*)((const unsigned char*)self + 0x530) & 0x30) != 0; }
extern "C" bool func_8006C6B4(void* self, unsigned int mask) { return ((*(unsigned int*)((char*)self + 0x1d4) & mask) != 0); }
extern "C" void func_8006C6CC() {}
extern "C" void func_8006C6E8() {}
extern "C" void func_8006C730(void* _this, unsigned int mask) {
    *(unsigned int*)((char*)_this + 0x1d4) &= ~mask;
}
extern "C" int func_8006CA20(void* _this) {
    return (*(int*)((char*)_this + 0x4EC) >> 1) & 1;
}
extern "C" void func_8006CBD8() {}
extern "C" void func_8006CBEC() {}
extern "C" void func_8006CC4C() {}
extern "C" void func_8006CE18(void* arg0) {
    *(u32*)((char*)arg0 + 0x1d4) = 0;
}
extern "C" u32 func_8006D374(void* p) {
    return *(u32*)((u8*)p + 4) >> 31;
}
extern "C" u32 func_8006D400(void *self) { return *(u32*)((char*)self + 4); }
extern "C" float func_8006D408(void* self) { return *(float*)((char*)self + 0x1e0); }
extern "C" float func_8006D410(float f) {
    extern float lbl_eu_8066A20C;
    return f * lbl_eu_8066A20C;
}
extern "C" void func_8006D440(unsigned* p) {
    p[1] |= 0x200;
}
extern "C" void* func_8006D6F8(void* ptr) { return (char*)ptr + 0x9c; }
extern "C" bool func_8006D72C(void* param_1) {
    return ((bool (*)(void*, int))func_8006BFC4)(param_1, 256);
}
extern "C" unsigned int func_8006DC34(unsigned char* self) {
    return (*(unsigned int*)(self + 0x4ec) >> 26) & 1;
}
extern "C" int func_8006DC40(void* p) {
    return (*(unsigned int*)((char*)p + 0x4EC) >> 24) & 1;
}
extern "C" int func_8006DC4C(void* self, unsigned int mask) { return ((*((unsigned int*)((char*)self + 0x4ec)) & mask) != 0); }
extern "C" uint32_t func_8006DC64(void* self) {
    return (*(uint32_t*)((char*)self + 0x4ec) >> 25) & 1;
}
extern "C" int func_8006DC70(void* self) {
    return (*(const unsigned int*)((const char*)self + 0x4ec) >> 23) & 1;
}
struct CfCam;
extern "C" float func_8006DC7C(CfCam* self) {
    return *(float *)((char *)self + 0x4FC);
}
extern "C" float func_8006DC84(void* self) { return *(float*)((char*)self + 0x4f8); }
extern "C" float func_8006DC8C(const void* self) {
    return *(const float*)((const char*)self + 0x508);
}
extern "C" int func_8006DC94(void* self) { return (*(uint32_t*)((char*)self + 0x4ec) >> 13) & 1; }
extern "C" unsigned int func_8006DF9C(void* self) {
    unsigned int val = *(unsigned int*)((unsigned char*)self + 0x64);
    return (val >> 2) & 1U;
}
extern "C" int func_8006DFA8(void *self) { return *(int *)((char *)self + 0x90e4); }
extern "C" unsigned int func_8006DFBC(void *self) {
    unsigned int word = *(unsigned int *)((char *)self + 0x64);
    return (word >> 27) & 1U;
}
extern "C" void func_8006DFC8() {}
extern int lbl_eu_80663DF0;

extern "C" int func_8006E59C()
{
    return lbl_eu_80663DF0;
}
extern "C" bool func_800755B0(void*, int);
extern "C" bool func_8006E5A4(void* self) { return func_800755B0(self, 0); }
extern "C" void func_8006E5AC(int* dst, int* src) {
    *dst = *src;
}
extern "C" int func_8006E5B8() { return 0x10; }
extern "C" void func_8006E5C0(unsigned* dst, const unsigned* src) {
    *dst = *src;
}
extern "C" void func_8006E5CC(u16* dest, const u16* src) {
    *dest = *src;
}
extern unsigned int lbl_eu_80663E28;

extern "C" int func_8006EEE4() {
    return (lbl_eu_80663E28 >> 24) & 1;
}
extern "C" void* func_8006EEF0(void* self) { return static_cast<char*>(self) + 0x3d8; }
extern "C" int func_8006EEF8(unsigned int* arg0) {
    return (arg0[1] >> 9) & 1;
}
extern "C" void func_8006EF04__Fi() {}
extern "C" void func_8006EF1C() {}
extern "C" void func_8006F9D4() {}
extern "C" void func_80071694() {}
extern "C" void func_80071730(void *r3, const void *r4) {
    const unsigned int *src = (const unsigned int *)r4;
    unsigned int *dst = (unsigned int *)r3;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}
extern "C" int func_80071A90(void* ptr) {
    return (*(unsigned int*)((char*)ptr + 4) >> 12) & 1;
}
extern "C" void func_80071A9C(void* self) { *(reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 4)) &= 0xFFFFEDFFu; }
extern "C" bool func_8049EB60(void* obj);

extern "C" bool func_80071B6C(void* obj, float val) {
    *(float*)((char*)obj + 0x1e0) = val;
    return func_8049EB60(obj);
}
extern "C" void func_80071B74() {}
extern "C" void func_80071B78() {}
extern "C" int func_80071CDC() {
    extern int lbl_eu_80665958;
    return lbl_eu_80665958;
}
extern "C" void* func_80071CE4(void* self) { return static_cast<char*>(self) + 0x60; }
extern "C" void* func_80071CEC(void* ptr) {
    return (char*)ptr + 0x6c;
}
extern "C" void func_80071CF4() {}
extern "C" int func_80073390(void* _this) {
    return ((*(unsigned*)((char*)_this + 0xc)) >> 19) & 1;
}
extern "C" unsigned int lbl_eu_80663E24;

extern "C" int func_8007339C() {
    return (lbl_eu_80663E24 >> 6) & 1;
}
extern "C" float func_800733B0(const void* arg) { return *(const float*)((const char*)arg + 0x14); }
extern "C" void func_8004B3F0(void*);

extern "C" void func_80073C74(void* p) {
    func_8004B3F0((char*)p + 0x10);
}
extern "C" void func_8007420C() {}
extern "C" void func_800743A4(float* dst, const float* src) {
    dst[3] = src[0];
    dst[7] = src[1];
    dst[11] = src[2];
}
extern "C" void func_800743C0() {}
extern "C" bool func_80074CD4(const unsigned* value, unsigned mask) { return (*value & mask) != 0; }
extern "C" int func_80074D44(void* self) { return 0x0; }
extern "C" void* func_80074D4C(void* p) {
    return (void*)((char*)*(void**)((char*)p + 0xC) + 0x118);
}
extern "C" float func_80074D58(void* self) { return *(float*)((char*)self + 0x1e0); }
extern "C" void func_80074D60(u32* self, const u32* src)
{
    self[10] = src[0];
    self[11] = src[1];
    self[12] = src[2];
}
extern "C" void sinit_80074D7C() {}

extern "C" void func_8006B720() {}
extern "C" void func_8006B8E4() {}
extern "C" float lbl_eu_80527230[];
extern "C" float lbl_eu_80661BA8;

struct Class_80296898 {
    static void* getInstance();
};

extern "C" void func_8006B948() {
    unsigned char idx = reinterpret_cast<unsigned char*>(Class_80296898::getInstance())[4];
    lbl_eu_80661BA8 = lbl_eu_80527230[idx];
}
extern "C" void func_8006B980() {}
extern "C" void func_8006BB20() {}
extern "C" void func_8006BFDC() {}
extern "C" void func_8006C16C() {}
extern "C" void func_8006C1C8() {}
extern "C" void func_8006C740() {}
extern "C" void func_8006CA2C() {}
extern "C" void func_8006CB0C() {}
extern "C" void func_8006CC0C() {}
extern "C" void func_8006CC68() {}
extern "C" void func_8006CE24() {}
extern "C" void func_8006D380() {}
extern "C" void func_8006D3D0() {}
extern "C" void func_8006D41C() {}
extern "C" void func_8006D450() {}
extern "C" void func_8006D6A8() {}
extern "C" void func_8006D700() {}
extern "C" void func_8006D734() {}
extern "C" void func_8006D7A8() {}
extern "C" void func_8006D804() {}
extern "C" void func_8006D8D0() {}
extern "C" void func_8006DBD4() {}
extern "C" void func_8006DCA0() {}
extern "C" void func_8006DD58() {}
extern "C" void func_8006DFE0() {}
extern "C" void func_8006E0C8() {}
extern "C" void func_8006E18C() {}
extern "C" void func_8006E2FC() {}
extern "C" void func_8006E5D8() {}
extern "C" void func_8006E884() {}
extern "C" void func_8006EFA0() {}
extern "C" void func_8006F5C8() {}
extern "C" void func_8006F9EC() {}
extern "C" void func_8006FC44() {}
extern "C" void func_8006FD3C() {}
extern "C" void func_8006FFA8() {}
extern "C" void func_80070088() {}
extern "C" void func_80070518() {}
extern "C" void func_800705D8() {}
extern "C" void func_80070674() {}
extern "C" void func_804BE4B4(void*, int);
extern "C" void* func_804BE4E0(void*, int);

extern "C" void* func_80070784(void* a, void* b) {
    func_804BE4B4(a, 0);
    return func_804BE4E0(b, 0);
}
extern "C" void func_800707C0() {}
extern "C" void func_80070EBC() {}
extern "C" void func_80070FB8() {}
extern "C" void func_80071364() {}
extern "C" void func_80071398() {}
extern "C" void func_80071754() {}
extern "C" void func_80071AB0() {}
extern "C" void func_80071B7C() {}
extern "C" void func_80071D50() {}
extern "C" void func_80071ED0() {}
extern "C" void func_80071F14() {}
extern "C" void func_80071F74() {}
extern "C" void func_80072194() {}
extern "C" void func_800733B8() {}
extern "C" void func_8007351C() {}
extern "C" void func_80073640() {}
extern "C" void func_80073C7C() {}
extern "C" void func_80073D8C() {}
extern "C" void func_80073DDC() {}
extern "C" void func_80073E74() {}
extern "C" void func_80073F88() {}
extern "C" void func_80074010() {}
extern "C" void func_80074090() {}
extern "C" void func_80074230() {}
extern "C" void func_800742FC() {}
extern "C" void func_80074488() {}
extern "C" void func_80074534() {}
extern "C" void func_80074900() {}
extern "C" void func_800749AC() {}
extern "C" void func_80074A3C() {}
extern "C" void func_80074A74() {}
extern "C" void func_80074AA4() {}
extern "C" void func_80074CEC() {}
extern "C" int CfObject_UnkVirtualFunc48__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv() { return 0; }
