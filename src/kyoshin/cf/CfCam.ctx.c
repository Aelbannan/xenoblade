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

    void clearUnk04Bits(unsigned int mask);
    unsigned int getBit26_0x4EC();
};

} // namespace cf

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1, void* arg2);
namespace cf {

class CfCam {
public:
    virtual ~CfCam();
    float getUnk4FC();

    // TODO: add fields
};
} // namespace cf
namespace cf {

class CfObject {
public:
    void CfObject_UnkVirtualFunc54();
    void CfObject_UnkVirtualFunc9();

    // TODO: add fields
    void CfObject_UnkVirtualFunc48();
    void CfObject_UnkVirtualFunc55();
    void CfObject_UnkVirtualFunc52();
};
} // namespace cf
/* end "kyoshin/cf/CfCam.hpp" */

void* __ct__cf_CfCam(void* self, void* arg);
void func_8006BEC4(void* self);
extern "C" {
void func_8004B0B0(void* self);
void func_8004B60C(void* self, f32 x, f32 y, f32 z);
void cfCam_stub_8006BEF8(void* self);
void cfCam_stub_8006BEC0(void* self);

void cfCam_clearUnk04(void* self);
void cfCam_clearFirstWord(void* self);
void cfCam_clearWord16(void* self);
void* cfCam_getElemStride12(void* self, int index);
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
    __ct__cf_CfCam(self, arg2);
    self->vtable = lbl_eu_80527260;

    func_8004B0B0(self->unk1C);
    func_8004B60C(self->unk1C + 12, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(self->unk1C + 24, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(self->unk1C + 36);
    func_8004B0B0(self->unk1C + 48);
    func_8004B0B0(self->unk1C + 60);
    func_8004B60C(self->unk1C + 72, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    cfCam_stub_8006BEF8(self->unk70);
    cfCam_stub_8006BEC0(self->unk130);

    self->unk164 = arg1;

    func_8004B0B0(self->unk168);
    func_8004B0B0(self->unk168 + 12);
    func_8004B60C(self->unk168 + 24, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B60C(self->unk168 + 36, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    func_8004B0B0(self->unk168 + 48);
    func_8004B0B0(self->unk168 + 60);
    func_8004B0B0(self->unk168 + 72);
    func_8004B0B0(self->unk168 + 84);
    func_8004B60C(self->unk168 + 96, lbl_eu_806662DC, lbl_eu_806662DC, lbl_eu_806662DC);
    cfCam_clearFirstWord(&self->unk1D4);
    cfCam_clearWord16(&self->unk1D8);

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

    func_8006BEC4(self->unk10);
    func_8006BEC4(self->unk168 + 60);
    func_8006BEC4(self->unk168 + 72);
    func_8006BEC4(self->unk168 + 84);
    cfCam_clearUnk04(self);
    memset(cfCam_getElemStride12(self->unk70, 0), 0, 0xC0);

    self->unk250 = 0;
    self->unk22C = lbl_eu_806662D0;
    self->unk230 = lbl_eu_806662D0;

    {
        void** vtbl = reinterpret_cast<void**>(self->vtable);
        reinterpret_cast<CfCamVFn_ui>(vtbl[16])(self, 1);
        reinterpret_cast<CfCamVFn>(vtbl[10])(self);
    }

    return self;
}

extern "C" void* cfCam_getUnk10Ptr(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk10; }
extern "C" u32 cfCam_getUnk164(const void* self) { return reinterpret_cast<u32>(static_cast<const cf::CfCamFollow*>(self)->unk164); }
extern "C" void* cfCam_getUnk40Ptr(void* ptr) { return static_cast<cf::CfCamFollow*>(ptr)->unk1C + 0x24; }
extern "C" void* cfCam_getUnk1CPtr(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk1C; }
extern "C" bool cfCam_getTrue() { return true; }
void func_8006BA80(){}
float func_8006BAF0(void* self){
    return __fabs(*(float*)self);
}
extern "C" float PSVECMag(const float* v);
extern "C" float cfCam_vecMag(const float* v) { return PSVECMag(v); }
void func_8006BB04(){}
extern "C" void cfCam_setOrClearUnk04Bits(void* r3, int r4, int r5) {
    cf::CfCamFollow* self = static_cast<cf::CfCamFollow*>(r3);
    if (r5) {
        self->unk04 |= r4;
    } else {
        self->unk04 &= ~r4;
    }
}
void cf::CfCamFollow::clearUnk04Bits(unsigned int mask) {
    unk04 &= ~mask;
}
extern "C" void __ct__Q22cf5CfCamFv() {}
extern "C" void cfCam_stub_8006BEC0(void* self) {}
void func_8006BEC4(void* self){}
extern "C" void cfCam_clearUnk04(void* p) {
    static_cast<cf::CfCamFollow*>(p)->unk04 = 0;
}
extern "C" void cfCam_setUnk08(void* self, int val) { static_cast<cf::CfCamFollow*>(self)->unk08 = val; }
extern "C" void cfCam_stub_8006BEF8(void* self) {}
void cfCam_clearFirstWord(void* param_1) { *(u32*)param_1 = 0; }
extern "C" void cfCam_clearWord16(void* ptr) {
    *(short*)ptr = 0;
}
void* cfCam_getElemStride12(void* r3, int r4) { return (char*)r3 + r4 * 12; }
extern "C" u32 cfCam_getUnk08(void *self) { return static_cast<cf::CfCamFollow*>(self)->unk08; }
extern "C" bool cfCam_testUnk04Bits(const void* self, unsigned int mask) { return (static_cast<const cf::CfCamFollow*>(self)->unk04 & mask) != 0; }
extern "C" int cfCam_getBit1_0x64(void* p) {
    return (*(int*)((char*)p + 0x64) >> 1) & 1;
}
extern "C" void cfCam_setField1E0AndGlobal(void* ptr, float f) {
    lbl_eu_80661B50 = f;
    static_cast<cf::CfCamFollow*>(ptr)->unk1E0 = f;
}
extern "C" void cfCam_setOrClearUnk1D4Bits(void* obj, unsigned int bits, int set_flag) {
    cf::CfCamFollow* self = static_cast<cf::CfCamFollow*>(obj);
    if (set_flag != 0)
        self->unk1D4 |= bits;
    else
        self->unk1D4 &= ~bits;
}
extern "C" int cfCam_getUnkC4(void* self) { return *(int*)((char*)self + 0xc4); }
extern "C" int cfCam_getBit0_0x530(void* p) {
    return *(unsigned short*)((unsigned char*)p + 0x530) & 1;
}
extern "C" bool cfCam_testBits2_3_0x530(const unsigned char* this_) { return (*(const unsigned short*)(this_ + 0x530) & 0x000C) != 0; }
extern "C" bool cfCam_testBits4_5_0x530(const void* self) { return (*(const unsigned short*)((const unsigned char*)self + 0x530) & 0x30) != 0; }
extern "C" bool cfCam_testUnk1D4Bits(void* self, unsigned int mask) { return (static_cast<cf::CfCamFollow*>(self)->unk1D4 & mask) != 0; }
void func_8006C6CC(){}
void func_8006C6E8(){}
extern "C" void cfCam_clearUnk1D4Bits(void* _this, unsigned int mask) {
    static_cast<cf::CfCamFollow*>(_this)->unk1D4 &= ~mask;
}
extern "C" int cfCam_getBit1_0x4EC(void* _this) {
    return (*(int*)((char*)_this + 0x4EC) >> 1) & 1;
}
void func_8006CBD8(){}
void func_8006CBEC(){}
void func_8006CC4C(){}
extern "C" void cfCam_zeroUnk1D4(void* arg0) {
    static_cast<cf::CfCamFollow*>(arg0)->unk1D4 = 0;
}
extern "C" u32 cfCam_getSignBitUnk04(void* p) {
    return static_cast<cf::CfCamFollow*>(p)->unk04 >> 31;
}
extern "C" u32 cfCam_getUnk04(void *self) { return static_cast<cf::CfCamFollow*>(self)->unk04; }
extern "C" float cfCam_getField1E0(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk1E0; }
extern "C" float cfCam_scaleByConst(float f) {
    extern float lbl_eu_8066A20C;
    return f * lbl_eu_8066A20C;
}
extern "C" void cfCam_setBit9_Unk04(unsigned* p) {
    p[1] |= 0x200;
}
extern "C" void* cfCam_getUnk9CPtr(void* ptr) { return (char*)ptr + 0x9c; }
extern "C" bool cfCam_testUnk04Bit8(void* param_1) {
    return ((bool (*)(void*, int))cfCam_testUnk04Bits)(param_1, 256);
}
unsigned int cf::CfCamFollow::getBit26_0x4EC() {
    return (*(unsigned int*)((u8*)this + 0x4ec) >> 26) & 1;
}
extern "C" int cfCam_getBit24_0x4EC(void* p) {
    return (*(unsigned int*)((char*)p + 0x4EC) >> 24) & 1;
}
extern "C" int cfCam_testUnk4ECBits(void* self, unsigned int mask) { return ((*((unsigned int*)((char*)self + 0x4ec)) & mask) != 0); }
extern "C" uint32_t cfCam_getBit25_0x4EC(void* self) {
    return (*(uint32_t*)((char*)self + 0x4ec) >> 25) & 1;
}
extern "C" int cfCam_getBit23_0x4EC(void* self) {
    return (*(const unsigned int*)((const char*)self + 0x4ec) >> 23) & 1;
}
float cf::CfCam::getUnk4FC() {
    return *(float *)((char *)this + 0x4FC);
}
extern "C" float cfCam_getUnk4F8(void* self) { return *(float*)((char*)self + 0x4f8); }
extern "C" float cfCam_getUnk508(const void* self) {
    return *(const float*)((const char*)self + 0x508);
}
extern "C" int cfCam_getBit13_0x4EC(void* self) { return (*(uint32_t*)((char*)self + 0x4ec) >> 13) & 1; }
extern "C" unsigned int cfCam_getBit2_0x64(void* self) {
    unsigned int val = *(unsigned int*)((unsigned char*)self + 0x64);
    return (val >> 2) & 1U;
}
extern "C" int cfCam_getUnk90E4(void *self) { return *(int *)((char *)self + 0x90e4); }
extern "C" unsigned int cfCam_getBit27_0x64(void *self) {
    unsigned int word = *(unsigned int *)((char *)self + 0x64);
    return (word >> 27) & 1U;
}
void func_8006DFC8(){}
extern int lbl_eu_80663DF0;

extern "C" int cfCam_getGlobal_80663DF0()
{
    return lbl_eu_80663DF0;
}
extern "C" bool func_800755B0(void*, int);
extern "C" bool cfCam_callCheckFlag(void* self) { return func_800755B0(self, 0); }
extern "C" void cfCam_copyInt(int* dst, int* src) {
    *dst = *src;
}
extern "C" int cfCam_getConst16() { return 0x10; }
extern "C" void cfCam_copyU32(unsigned* dst, const unsigned* src) {
    *dst = *src;
}
extern "C" void cfCam_copyU16(u16* dest, const u16* src) {
    *dest = *src;
}
extern unsigned int lbl_eu_80663E28;

extern "C" int cfCam_getBit24_GlobalE28() {
    return (lbl_eu_80663E28 >> 24) & 1;
}
extern "C" void* cfCam_getUnk3D8Ptr(void* self) { return static_cast<char*>(self) + 0x3d8; }
extern "C" int cfCam_getBit9_Arg1(unsigned int* arg0) {
    return (arg0[1] >> 9) & 1;
}
void func_8006EF04__Fi(){}
void func_8006EF1C(){}
void func_8006F9D4(){}
void func_80071694(){}
extern "C" void cfCam_copy4Words(void *r3, const void *r4) {
    const unsigned int *src = (const unsigned int *)r4;
    unsigned int *dst = (unsigned int *)r3;
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
}
extern "C" int cfCam_getBit12_Unk04(void* ptr) {
    return (*(unsigned int*)((char*)ptr + 4) >> 12) & 1;
}
extern "C" void cfCam_clearUnk04Bits_EDFF(void* self) { static_cast<cf::CfCamFollow*>(self)->unk04 &= 0xFFFFEDFFu; }
extern "C" bool func_8049EB60(void* obj);

extern "C" bool cfCam_setUnk1E0AndValidate(void* obj, float val) {
    *(float*)((char*)obj + 0x1e0) = val;
    return func_8049EB60(obj);
}
extern "C" void cfCam_stub_80071B74() {}
extern "C" void cfCam_stub_80071B78() {}
extern "C" int cfCam_getGlobal_80665958() {
    extern int lbl_eu_80665958;
    return lbl_eu_80665958;
}
extern "C" void* cfCam_getUnk60Ptr(void* self) { return static_cast<char*>(self) + 0x60; }
extern "C" void* cfCam_getUnk6CPtr(void* ptr) {
    return (char*)ptr + 0x6c;
}
void func_80071CF4(){}
extern "C" int cfCam_getBit19_0x0C(void* _this) {
    return ((*(unsigned*)((char*)_this + 0xc)) >> 19) & 1;
}
extern "C" unsigned int lbl_eu_80663E24;

extern "C" int cfCam_getBit6_GlobalE24() {
    return (lbl_eu_80663E24 >> 6) & 1;
}
extern "C" float cfCam_getUnk14Float(const void* arg) { return *(const float*)((const char*)arg + 0x14); }
extern "C" void func_8004B3F0(void*);

extern "C" void cfCam_resetUnk10(void* p) {
    func_8004B3F0((char*)p + 0x10);
}
void func_8007420C(){}
extern "C" void cfCam_setMtxTranslation(float* dst, const float* src) {
    dst[3] = src[0];
    dst[7] = src[1];
    dst[11] = src[2];
}
void func_800743C0(){}
extern "C" bool cfCam_testBits(const unsigned* value, unsigned mask) { return (*value & mask) != 0; }
extern "C" int cfCam_getZero(void* self) { return 0x0; }
extern "C" void* cfCam_getNestedPtr118(void* p) {
    return static_cast<char*>(static_cast<cf::CfCamFollow*>(p)->unk0C) + 0x118;
}
extern "C" float cfCam_getFloat1E0(void* self) { return static_cast<cf::CfCamFollow*>(self)->unk1E0; }
extern "C" void cfCam_copyVec3U32(u32* self, const u32* src)
{
    self[10] = src[0];
    self[11] = src[1];
    self[12] = src[2];
}
void sinit_80074D7C(){}

void func_8006B720(){}
void func_8006B8E4(){}
extern "C" float lbl_eu_80527230[];
extern "C" float lbl_eu_80661BA8;

struct Class_80296898 {
    static void* getInstance();
};

extern "C" void cfCam_updateLbl61BA8() {
    unsigned char idx = reinterpret_cast<unsigned char*>(Class_80296898::getInstance())[4];
    lbl_eu_80661BA8 = lbl_eu_80527230[idx];
}
void func_8006B980(){}
void func_8006BB20(){}
void func_8006BFDC(){}
void func_8006C16C(){}
void func_8006C1C8(){}
void func_8006C740(){}
void func_8006CA2C(){}
void func_8006CB0C(){}
void func_8006CC0C(){}
void func_8006CC68(){}
void func_8006CE24(){}
void func_8006D380(){}
void func_8006D3D0(){}
void func_8006D41C(){}
void func_8006D450(){}
void func_8006D6A8(){}
void func_8006D700(){}
void func_8006D734(){}
void func_8006D7A8(){}
void func_8006D804(){}
void func_8006D8D0(){}
void func_8006DBD4(){}
void func_8006DCA0(){}
void func_8006DD58(){}
void func_8006DFE0(){}
void func_8006E0C8(){}
void func_8006E18C(){}
void func_8006E2FC(){}
void func_8006E5D8(){}
void func_8006E884(){}
void func_8006EFA0(){}
void func_8006F5C8(){}
void func_8006F9EC(){}
void func_8006FC44(){}
void func_8006FD3C(){}
void func_8006FFA8(){}
void func_80070088(){}
void func_80070518(){}
void func_800705D8(){}
void func_80070674(){}
extern "C" void func_804BE4B4(void*, int);
extern "C" void* func_804BE4E0(void*, int);

extern "C" void* cfCam_initAndGetSomething(void* a, void* b) {
    func_804BE4B4(a, 0);
    return func_804BE4E0(b, 0);
}
void func_800707C0(){}
void func_80070EBC(){}
void func_80070FB8(){}
void func_80071364(){}
void func_80071398(){}
void func_80071754(){}
void func_80071AB0(){}
void func_80071B7C(){}
void func_80071D50(){}
void func_80071ED0(){}
void func_80071F14(){}
void func_80071F74(){}
void func_80072194(){}
void func_800733B8(){}
void func_8007351C(){}
void func_80073640(){}
void func_80073C7C(){}
void func_80073D8C(){}
void func_80073DDC(){}
void func_80073E74(){}
void func_80073F88(){}
void func_80074010(){}
void func_80074090(){}
void func_80074230(){}
void func_800742FC(){}
void func_80074488(){}
void func_80074534(){}
void func_80074900(){}
void func_800749AC(){}
void func_80074A3C(){}
void func_80074A74(){}
void func_80074AA4(){}
void func_80074CEC(){}
extern "C" int CfObject_UnkVirtualFunc48__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv() { return 0; }
