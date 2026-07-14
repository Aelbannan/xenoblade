/* "src/kyoshin/cf/object/CBattleState.cpp" line 0 "kyoshin/cf/object/CBattleState.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CBattleState.hpp" line 2 "types.h" */
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
        virtual void CBattleState_UnkVirtualFunc3();  //0x10
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
        u8 unk15AC[0x15DC - 0x15AC];
    };
}

// symbols.txt mangles Fv; retail leaves the arg entry in r4 (same pattern
// as cf::CAIAction's UnkVirtualFunc1/2).
extern "C" void CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg);

// symbols.txt mangles Fv; retail leaves the caller's mask in r4 (same ABI
// pattern as CBattleState_UnkVirtualFunc6).
extern "C" void CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 mask);

// symbols.txt mangles Fv; retail leaves the id in r4 (same fake-Fv ABI as
// UnkVirtualFunc6 above).
extern "C" int CBattleState_UnkVirtualFunc31__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id);

namespace cf {
    struct CBattleStateSrcEntry;
}

// symbols.txt mangles Fv; retail leaves the source table pointer in r4
// (same ABI pattern as UnkVirtualFunc6 above).
extern "C" void CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv(
    cf::CBattleState* self, const cf::CBattleStateSrcEntry* src);

// symbols.txt mangles Fv; retail leaves the entry arg in r4 (same fake-Fv
// ABI as UnkVirtualFunc6 above).
extern "C" void CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* entry);

// symbols.txt mangles Fv; retail leaves the entry arg in r4 (same fake-Fv
// ABI as UnkVirtualFunc6/8). Matches on unk2E, then clears matching slots.
extern "C" void CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg);
/* end "kyoshin/cf/object/CBattleState.hpp" */

extern "C" void* memset(void* dest, int val, size_t count);
extern "C" void* memcpy(void* dest, const void* src, size_t count);

namespace cf {

CBattleState::CBattleState() {
    u8* p;
    u8* end;

    unk4 = 0;
    unk6 = 0;
    memset(unk8, 0, 0x1520);

    p = unk152C;
    end = (u8*)this + 0x15AC;
    do {
        memset(p, 0, 0x10);
        p += 0x10;
    } while (p < end);

    memset(unk152C, 0, 0x80);
    memset(this, 0, 0x15D4);
}

// Batch 2026-07-14j: battlestate-vfunc29 owns CBattleState_UnkVirtualFunc29
// exclusively. Do not touch the ctor / other vfuncs.
//
// True Fv (r3=this only). Clears the 8 CBattleStateEntry slots at +0x1388;
// for each prior id, if it is still present among the 0x68 entries at +0x8
// (13×8 halfword scan) the this+0x15AC status bit stays; otherwise cleared.
// Ids >= 0x12f always clear (skip the scan). Ends with memset(+0x152C,0,0x80).
void CBattleState::CBattleState_UnkVirtualFunc29() {
    CBattleStateEntry* entry;
    int i;

    entry = (CBattleStateEntry*)((u8*)this + 0x1388);
    for (i = 0; i < 8; i++, entry++) {
        u32 id;
        int found;

        id = entry->unk0C;
        memset(entry, 0, 0x34);

        if (id >= 0x12f) {
            found = 0;
        } else {
            // Soft-cap (CODE_MATCH ~95.5%): retail keeps a dead trip counter in
            // r3 (li 0 / addi +7 / unused after bdnz) alongside found in r0 and
            // scan base in r4. Goto found-paths put found in r0 but walks via
            // r3 (loses r4 scan); break form keeps r4 scan but coalesces found
            // into r3 and DSE's the trip. Same Chaitin class as UnkVirtualFunc11.
            u8* p = (u8*)this;
            int g;

            found = 0;
            for (g = 13; g != 0; g--) {
                if (id == *(u16*)(p + 0x14)) { found = 1; goto scan_done; }
                if (id == *(u16*)(p + 0x48)) { found = 1; goto scan_done; }
                if (id == *(u16*)(p + 0x7c)) { found = 1; goto scan_done; }
                if (id == *(u16*)(p + 0xb0)) { found = 1; goto scan_done; }
                if (id == *(u16*)(p + 0xe4)) { found = 1; goto scan_done; }
                if (id == *(u16*)(p + 0x118)) { found = 1; goto scan_done; }
                if (id == *(u16*)(p + 0x14c)) { found = 1; goto scan_done; }
                if (id == *(u16*)(p + 0x180)) { found = 1; goto scan_done; }
                p += 0x1a0;
            }
            found = 0;
        scan_done:
            ;
        }

        if (!found) {
            u8* wordPtr = unk15AC + ((id >> 3) & ~3u);
            *(u32*)wordPtr &= ~(1u << (id & 0x1F));
        }
    }

    memset(unk152C, 0, 0x80);
}

} // namespace cf

// Batch 2026-07-14g: battlestate-vfunc6 owns CBattleState_UnkVirtualFunc6
// exclusively. Do not touch CBattleState::CBattleState() above.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// pattern as cf::CAIAction's UnkVirtualFunc1/2 — see
// docs/MWCC_REFERENCE.md).
//
// sdata2 float pool constant read via lbl_eu_80667414@sda21 (0.9f).
extern "C" const float lbl_eu_80667414;

extern "C" void CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc17Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    cf::CBattleStateEntry* entries;
    cf::CBattleStateEntry* p;
    int n;

    // Bit `arg->unk0C` set into the this+0x15AC bitfield (word-aligned byte
    // offset = (id >> 3) & ~3, bit position = id & 0x1F).
    *(u32*)(self->unk15AC + (((u32)arg->unk0C >> 3) & ~3u)) |=
        1u << (arg->unk0C & 0x1F);

    entries = (cf::CBattleStateEntry*)((u8*)self + 0x1388);
    p = entries;
    for (n = 8; n != 0; n--, p++) {
        if (p->unk0C == arg->unk0C) {
            if (p->unk10 < arg->unk10) {
                p->unk10 = arg->unk10;
            }
            if (arg->unk18 != 0) {
                if (p->unk10 > arg->unk18) {
                    p->unk10 = arg->unk18;
                }
            }
            return ((Vfunc17Fn*)*(void**)self)[18](self, p);
        }
    }

    for (n = 8; n != 0; n--, entries++) {
        if (entries->unk0C == 0) {
            entries->unk00 = arg->unk00;
            entries->unk04 = arg->unk04;
            entries->unk08 = arg->unk08;
            entries->unk0C = arg->unk0C;
            entries->unk10 = arg->unk10;
            entries->unk14 = arg->unk14;
            entries->unk16 = arg->unk16;
            entries->unk18 = arg->unk18;
            entries->unk1A = arg->unk1A;
            entries->unk1C = arg->unk1C;
            entries->unk20 = arg->unk20;
            entries->unk24 = arg->unk24;
            {
                f32 scaled = lbl_eu_80667414 * entries->unk24;
                entries->unk28 = arg->unk28;
                entries->unk2C = arg->unk2C;
                entries->unk2E = arg->unk2E;
                entries->unk30 = arg->unk30;
                entries->unk1C = entries->unk20;
                entries->unk28 = scaled;
            }
            ((Vfunc17Fn)(*(void***)self)[18])(self, entries);
            return;
        }
    }
}

// symbols.txt mangles Fv, but retail leaves the id in r4. This lookup reads
// the independent halfword state at +0x6.
extern "C" int CBattleState_UnkVirtualFunc33__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id) {
    u16 mask;

    if (id >= 0x12f) {
        return 0;
    }

    mask = 0;

    switch (id) {
    case 0x4:
        mask = 0x1;
        break;
    case 0x7:
        mask = 0x2;
        break;
    case 0x6:
        mask = 0x4;
        break;
    case 0x11:
        mask = 0x8;
        break;
    case 0x9:
        mask = 0x10;
        break;
    case 0x13:
        mask = 0x20;
        break;
    case 0xb:
        mask = 0x40;
        break;
    case 0xc:
        mask = 0x80;
        break;
    case 0xdc:
        mask = 0x100;
        break;
    case 0x12:
        mask = 0x200;
        break;
    case 0x3c:
        mask = 0x400;
        break;
    case 0x3d:
        mask = 0x800;
        break;
    case 0x52:
        mask = 0x1000;
        break;
    case 0x53:
        mask = 0x2000;
        break;
    case 0x54:
        mask = 0x4000;
        break;
    case 0x117:
        mask = 0x8000;
        break;
    }

    return (self->unk6 & mask) != 0;
}

// Batch 2026-07-14h: battlestate-vfunc11 owns CBattleState_UnkVirtualFunc11
// exclusively. Do not touch the ctor / vfunc6 / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the caller's mask in r4 (same
// ABI pattern as CBattleState_UnkVirtualFunc6). Walks the this+0x8 entry
// array (stride 0x34, count 0x68 == sizeof(unk8)/0x34) and, for any slot
// whose unk30 flags intersect the mask: fires the vt+0x4C callback
// (UnkVirtualFunc18) with that slot, remembers its id, then clears the
// slot. If another live entry still shares that id (scan of the same
// array, 13 groups of 8, matching MWCC's fixed-trip-count unroll), the
// this+0x15AC status bit for that id is left alone; otherwise it's
// cleared (ids >= 0x12f always clear, skipping the scan).
extern "C" void CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 mask) {
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    int i;
    cf::CBattleStateEntry* entry;

    entry = (cf::CBattleStateEntry*)((u8*)self + 0x8);
    for (i = 0; i < 0x68; i++, entry++) {
        u32 id;
        int stillActive;

        if ((entry->unk30 & mask) == 0) {
            continue;
        }

        ((Vfunc18Fn)(*(void***)self)[19])(self, entry);
        id = entry->unk0C;
        memset(entry, 0, 0x34);

        if (id >= 0x12f) {
            stillActive = 0;
        } else {
            // Soft-cap (CODE_MATCH ~96.2%): retail keeps a dead trip counter in
            // r3 (li 0 / addi +7 / unused after bdnz) alongside found in r0 and
            // scan base in r4. MWCC coalesces found into r3 and DSE's any dead
            // trip (`trip&0`, `trip^trip`, comma) or blows the shape with
            // volatile / `trip>1000` boolify. Instruction shapes, layout, and
            // the rlwinm bit clear otherwise match — remaining gap is Chaitin
            // register coloring only (see MWCC_REFERENCE).
            u8* p = (u8*)self;
            int g;

            stillActive = 0;
            for (g = 13; g != 0; g--) {
                if (id == *(u16*)(p + 0x14)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x48)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x7c)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0xb0)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0xe4)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x118)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x14c)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x180)) { stillActive = 1; break; }
                p += 0x1a0;
            }
        }

        if (!stillActive) {
            u8* wordPtr = self->unk15AC + ((id >> 3) & ~3u);
            *(u32*)wordPtr &= ~(1u << (id & 0x1F));
        }
    }
}

// Batch 2026-07-14h: battlestate-vfunc31 owns CBattleState_UnkVirtualFunc31
// exclusively. Do not touch ctor / UnkVirtualFunc6 / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the id in r4 (same fake-Fv ABI
// as UnkVirtualFunc6 — see docs/MWCC_REFERENCE.md).
//
// Leaf / no stack frame: maps specific ids to single-bit masks (or 0 for
// unmapped ids), then returns (self->unk4 & mask) != 0 via the standard
// MWCC branchless neg/or/srwi boolify idiom (see MWCC_REFERENCE §8c9).
extern "C" int CBattleState_UnkVirtualFunc31__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id) {
    u16 mask;

    if (id >= 0x12f) {
        return 0;
    }

    mask = 0;

    switch (id) {
    case 0x4:
        mask = 0x1;
        break;
    case 0x7:
        mask = 0x2;
        break;
    case 0x6:
        mask = 0x4;
        break;
    case 0x11:
        mask = 0x8;
        break;
    case 0x9:
        mask = 0x10;
        break;
    case 0x13:
        mask = 0x20;
        break;
    case 0xb:
        mask = 0x40;
        break;
    case 0xc:
        mask = 0x80;
        break;
    case 0xdc:
        mask = 0x100;
        break;
    case 0x12:
        mask = 0x200;
        break;
    case 0x3c:
        mask = 0x400;
        break;
    case 0x3d:
        mask = 0x800;
        break;
    case 0x52:
        mask = 0x1000;
        break;
    case 0x53:
        mask = 0x2000;
        break;
    case 0x54:
        mask = 0x4000;
        break;
    case 0x117:
        mask = 0x8000;
        break;
    }

    return (self->unk4 & mask) != 0;
}

// Batch 2026-07-14h: battlestate-vfunc26 owns CBattleState_UnkVirtualFunc26
// exclusively. Do not touch ctor / UnkVirtualFunc6 / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the source table pointer in r4
// (same ABI pattern as UnkVirtualFunc6). 0x10-byte record read from the
// incoming table; 8 consecutive records precede the raw copy landed at
// CBattleState::unk152C.
namespace cf {
struct CBattleStateSrcEntry {
    u16 unk00; // 0x00
    u16 unk02; // 0x02
    u16 unk04; // 0x04 - id, copied into CBattleStateEntry::unk0C
    u16 unk06; // 0x06 - copied into CBattleStateEntry::unk1A
    s16 unk08; // 0x08 - sign-extended into CBattleStateEntry::unk10
    s16 unk0A; // 0x0A - copied into CBattleStateEntry::unk14
    s16 unk0C; // 0x0C - copied into CBattleStateEntry::unk18
    u16 unk0E; // 0x0E - bit 0x8000 selects CBattleStateEntry::unk08
};
} // namespace cf

extern "C" void CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv(
    cf::CBattleState* self, const cf::CBattleStateSrcEntry* src) {
    typedef void (*Vfunc6Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    const cf::CBattleStateSrcEntry* rec;
    const cf::CBattleStateSrcEntry* recFlags;
    int n;
    int allZero;
    u32 flag2000;
    u32 flag4000;

    self->CBattleState_UnkVirtualFunc29();

    flag2000 = 0x2000;
    flag4000 = 0x4000;

    for (n = 0, rec = src, recFlags = src; n < 8; n++, rec++, recFlags++) {
        cf::CBattleStateEntry entry;
        u32 flags;
        u16 id;

        allZero = 0;
        if (rec->unk00 == 0) {
            if (rec->unk02 == 0) {
                if (rec->unk04 == 0) {
                    allZero = 1;
                }
            }
        }
        if (allZero) {
            continue;
        }

        memset(&entry, 0, sizeof(entry));
        id = rec->unk04;
        flags = entry.unk30;
        entry.unk0C = id;
        flags |= 1;
        entry.unk08 = flag2000;
        entry.unk10 = rec->unk08;
        entry.unk14 = rec->unk0A;
        entry.unk18 = rec->unk0C;
        entry.unk1A = (s16)rec->unk06;
        entry.unk30 = flags;
        // u32 >> 15 & 1 → extrwi (rlwinm. …,17,31,31); u16 >> yields srawi
        if ((((u32)recFlags->unk0E >> 15) & 1) != 0) {
            entry.unk08 = flag4000;
        }

        ((Vfunc6Fn*)*(void**)self)[7](self, &entry);
    }

    // Retail: mtctr/bdnz + lwzu/stwu from this+0x1528 / src-4. do-while(--i)
    // stays compact (for!=0 fully unrolls to 0x208); addic./bne soft-cap.
    {
        u32* dst = (u32*)((u8*)self + 0x1528);
        u32* s = (u32*)src - 1;
        u32 i = 0x10;

        do {
            u32 a = *(s + 1);
            u32 b = *(s += 2);
            *(dst + 1) = a;
            *(dst += 2) = b;
        } while (--i);
    }
}

// Batch 2026-07-14j: battlestate-vfunc8 owns CBattleState_UnkVirtualFunc8
// exclusively. Do not touch ctor / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// fake-Fv ABI as UnkVirtualFunc6). Nested cmpwi on entry->unk0C picks a
// kind; kind==3 clears this+0x1528. Calls vt+0x2C (UnkVirtualFunc10) with
// the arg, then walks this+0x8 entries (stride 0x34, count 0x68): matching
// id (+ optional field eq unless unk30 bit 0x200) → stack-copy/clear →
// vt+0x2C → id-dup scan / clear unk15AC bit → vt+0x4C; stop early if
// arg->unk0C == 0.
extern "C" void CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* entry) {
    typedef void (*Vfunc10Fn)(cf::CBattleState*, cf::CBattleStateEntry*);
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    // Hoist one/thirteen so Chaitin parks them in r26/r27 and leaves
    // this/entry in r28/r29 matching retail's early mr pair.
    int one;
    int thirteen;
    int id;
    int kind;

    one = 1;
    thirteen = 0xd;
    id = entry->unk0C;

    // Flat if+goto mirrors retail's cmpwi/beq/bge chain 1:1. Nested if/else
    // preserves pivots but duplicates kind leaves (oversized); nested if+goto
    // to shared leaves lets MWCC rebuild a different decision tree.
    if (id >= 0xd4)
        goto L_801486EC;
    if (id >= 0x3e)
        goto L_80148684;
    if (id >= 0x2c)
        goto L_80148658;
    if (id == 0x27)
        goto kind0;
    if (id >= 0x27)
        goto L_8014864C;
    if (id >= 4)
        goto L_80148640;
    if (id >= 2)
        goto kind0;
    goto kind2;

L_80148640:
    if (id >= 0x14)
        goto kind2;
    goto kind1;

L_8014864C:
    if (id >= 0x2a)
        goto kind1;
    goto kind2;

L_80148658:
    if (id == 0x36)
        goto kind0;
    if (id >= 0x36)
        goto L_80148678;
    if (id >= 0x35)
        goto kind2;
    if (id >= 0x33)
        goto kind0;
    goto kind2;

L_80148678:
    if (id >= 0x3c)
        goto kind1;
    goto kind2;

L_80148684:
    if (id == 0x5f)
        goto kind0;
    if (id >= 0x5f)
        goto L_801486C0;
    if (id >= 0x52)
        goto L_801486AC;
    if (id >= 0x46)
        goto kind2;
    if (id >= 0x44)
        goto kind0;
    goto kind2;

L_801486AC:
    if (id >= 0x5d)
        goto kind2;
    if (id >= 0x58)
        goto kind0;
    goto kind1;

L_801486C0:
    if (id == 0x93)
        goto kind0;
    if (id >= 0x93)
        goto L_801486E0;
    if (id >= 0x6a)
        goto kind2;
    if (id >= 0x65)
        goto kind1;
    goto kind2;

L_801486E0:
    if (id >= 0xce)
        goto kind0;
    goto kind2;

L_801486EC:
    if (id >= 0x103)
        goto L_8014874C;
    if (id == 0xeb)
        goto kind2;
    if (id >= 0xeb)
        goto L_80148728;
    if (id >= 0xdf)
        goto L_80148714;
    if (id == 0xdc)
        goto kind1;
    goto kind2;

L_80148714:
    if (id >= 0xea)
        goto kind3;
    if (id >= 0xe3)
        goto kind2;
    goto kind1;

L_80148728:
    if (id == 0xf7)
        goto kind0;
    if (id >= 0xf7)
        goto L_80148740;
    if (id >= 0xed)
        goto kind3;
    goto kind0;

L_80148740:
    if (id == 0xff)
        goto kind2;
    goto kind3;

L_8014874C:
    if (id == 0x117)
        goto kind1;
    if (id >= 0x117)
        goto L_80148780;
    if (id >= 0x109)
        goto L_8014876C;
    if (id == 0x106)
        goto kind3;
    goto kind2;

L_8014876C:
    if (id >= 0x111)
        goto kind3;
    if (id >= 0x10d)
        goto kind2;
    goto kind3;

L_80148780:
    if (id == 0x12d)
        goto kind0;
    if (id >= 0x12d)
        goto kind2;
    if (id == 0x11e)
        goto kind0;
    goto kind2;

kind0:
    kind = 0;
    goto kind_done;
kind1:
    kind = 1;
    goto kind_done;
kind3:
    kind = 3;
    goto kind_done;
kind2:
    kind = 2;
kind_done:

    if (kind == 3) {
        *(u32*)self->unk1528 = 0;
    }

    ((Vfunc10Fn)(*(void***)self)[11])(self, entry);

    {
        cf::CBattleStateEntry* slot;
        int i;

        // Retail init order after the first vt+0x2C call:
        //   slot=this+8, i=0  (one/thirteen already live in r26/r27)
        slot = (cf::CBattleStateEntry*)((u8*)self + 0x8);
        i = 0;

        for (; i < 0x68; i++, slot++) {
            u32 savedId;
            int stillActive;
            u32 savedWords[0x34 / 4];
            u32* s;
            u32 a;
            u32 b;
            void* clearPtr;
            int clearVal;
            int clearLen;
            int trip;
            int g;
            u8* p;

            if (slot->unk0C != entry->unk0C) {
                continue;
            }

            if ((entry->unk30 & 0x200) == 0) {
                if (slot->unk00 != entry->unk00) {
                    continue;
                }
                if (slot->unk04 != entry->unk04) {
                    continue;
                }
                if (slot->unk08 != entry->unk08) {
                    continue;
                }
                if (slot->unk2E != entry->unk2E) {
                    continue;
                }
            }

            // Retail schedules memset args into the first pair of the
            // word-copy (lwz r6/r0, mr dest, li val/len, stw pair hi/lo).
            s = (u32*)slot;
            a = s[0];
            clearPtr = slot;
            b = s[1];
            clearVal = 0;
            savedWords[1] = b;
            clearLen = 0x34;
            savedWords[0] = a;
            a = s[2];
            b = s[3];
            savedWords[3] = b;
            savedWords[2] = a;
            a = s[4];
            b = s[5];
            savedWords[5] = b;
            savedWords[4] = a;
            a = s[6];
            b = s[7];
            savedWords[7] = b;
            savedWords[6] = a;
            a = s[8];
            b = s[9];
            savedWords[9] = b;
            savedWords[8] = a;
            a = s[10];
            b = s[11];
            savedWords[11] = b;
            savedWords[10] = a;
            savedWords[12] = s[12];
            memset(clearPtr, clearVal, clearLen);

            ((Vfunc10Fn)(*(void***)self)[11])(
                self, (cf::CBattleStateEntry*)savedWords);

            // Load halfword id into a wide local first (retail lhz → r5).
            savedId = *(u16*)((u8*)savedWords + 0xc);
            if (savedId >= 0x12f) {
                stillActive = 0;
            } else {
                // Retail: found in r0, dead trip in r3 (li 0 / addi +7 /
                // unused after bdnz), scan base in r4. Keep trip live via
                // a zeroing OR so MWCC cannot DSE the addi+7.
                p = (u8*)self;
                trip = 0;
                stillActive = 0;
                for (g = thirteen; g != 0; g--) {
                    if (savedId == *(u16*)(p + 0x14)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x48)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x7c)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0xb0)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0xe4)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x118)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x14c)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x180)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    p += 0x1a0;
                    trip += 7;
                }
                stillActive = 0;
            scan_done:
                stillActive |= trip & 0;
            }

            if (stillActive == 0) {
                u8* wordPtr = self->unk15AC + ((savedId >> 3) & ~3u);
                *(u32*)wordPtr &= ~(one << (savedId & 0x1F));
            }

            ((Vfunc18Fn)(*(void***)self)[19])(
                self, (cf::CBattleStateEntry*)savedWords);

            if (entry->unk0C == 0) {
                break;
            }
        }
    }
}

// Batch 2026-07-14k: battlestate-vfunc10 owns CBattleState_UnkVirtualFunc10
// exclusively. Do not touch ctor / other vfuncs.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// fake-Fv ABI as UnkVirtualFunc8). Early-out when arg->unk2E == 0. Walks
// this+0x8 (0x68 × stride 0x34): match on slot->unk2E (== arg->unk2E);
// when arg->unk30 bit 0x200 is clear also require unk00/04/08 eq. Same
// flat if+goto kind tree on the *slot* id as UnkVirtualFunc8; kind==3
// clears this+0x1528. Stack-copy + memset slot, id-dup scan / clear
// unk15AC bit, then vt+0x4C (UnkVirtualFunc18). No recursive vt+0x2C;
// walks all slots (no early break).
extern "C" void CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    int one;
    int thirteen;
    cf::CBattleStateEntry* slot;
    int i;
    // Full arg spill (retail sp+0x3c..0x6c). Do not keep stackedWords[0] in a
    // separate local — retail reloads it from the frame on the optional-eq
    // path; holding it live costs an extra CSR (stmw r21 vs retail stmw r22).
    u32 stackedWords[0x34 / 4];
    u32* aw;
    u16 key;
    u32 flagBit;
    u32 a04;
    u32 a08;

    if (arg->unk2E == 0) {
        return;
    }

    aw = (u32*)arg;
    stackedWords[0] = aw[0];
    stackedWords[1] = aw[1];
    stackedWords[2] = aw[2];
    stackedWords[3] = aw[3];
    stackedWords[4] = aw[4];
    stackedWords[5] = aw[5];
    stackedWords[6] = aw[6];
    stackedWords[7] = aw[7];
    stackedWords[8] = aw[8];
    stackedWords[9] = aw[9];
    stackedWords[10] = aw[10];
    stackedWords[11] = aw[11];
    stackedWords[12] = aw[12];

    // Match key = lhz of low half of word at arg+0x2C (unk2E). a04/a08 stay
    // live in GPRs (retail r29/r30); a00 is only read back from the spill.
    key = *(u16*)((u8*)stackedWords + 0x2e);
    flagBit = stackedWords[12] & 0x200;
    a04 = stackedWords[1];
    a08 = stackedWords[2];

    one = 1;
    thirteen = 0xd;
    slot = (cf::CBattleStateEntry*)((u8*)self + 0x8);
    i = 0;

    for (; i < 0x68; i++, slot++) {
        int id;
        int kind;
        u32 savedId;
        int stillActive;
        u32 savedWords[0x34 / 4];
        u32* s;
        u32 a;
        u32 b;
        void* clearPtr;
        int clearVal;
        int clearLen;
        int trip;
        int g;
        u8* p;

        if (slot->unk2E != key) {
            continue;
        }

        if (flagBit == 0) {
            if (slot->unk00 != stackedWords[0]) {
                continue;
            }
            if (slot->unk04 != a04) {
                continue;
            }
            if (slot->unk08 != a08) {
                continue;
            }
        }

        // Flat if+goto mirrors retail's cmpwi/beq/bge chain on slot->unk0C
        // (same leaf set as UnkVirtualFunc8).
        id = slot->unk0C;

        if (id >= 0xd4)
            goto L10_80148BE0;
        if (id >= 0x3e)
            goto L10_80148B78;
        if (id >= 0x2c)
            goto L10_80148B4C;
        if (id == 0x27)
            goto kind0;
        if (id >= 0x27)
            goto L10_80148B40;
        if (id >= 4)
            goto L10_80148B34;
        if (id >= 2)
            goto kind0;
        goto kind2;

    L10_80148B34:
        if (id >= 0x14)
            goto kind2;
        goto kind1;

    L10_80148B40:
        if (id >= 0x2a)
            goto kind1;
        goto kind2;

    L10_80148B4C:
        if (id == 0x36)
            goto kind0;
        if (id >= 0x36)
            goto L10_80148B6C;
        if (id >= 0x35)
            goto kind2;
        if (id >= 0x33)
            goto kind0;
        goto kind2;

    L10_80148B6C:
        if (id >= 0x3c)
            goto kind1;
        goto kind2;

    L10_80148B78:
        if (id == 0x5f)
            goto kind0;
        if (id >= 0x5f)
            goto L10_80148BB4;
        if (id >= 0x52)
            goto L10_80148BA0;
        if (id >= 0x46)
            goto kind2;
        if (id >= 0x44)
            goto kind0;
        goto kind2;

    L10_80148BA0:
        if (id >= 0x5d)
            goto kind2;
        if (id >= 0x58)
            goto kind0;
        goto kind1;

    L10_80148BB4:
        if (id == 0x93)
            goto kind0;
        if (id >= 0x93)
            goto L10_80148BD4;
        if (id >= 0x6a)
            goto kind2;
        if (id >= 0x65)
            goto kind1;
        goto kind2;

    L10_80148BD4:
        if (id >= 0xce)
            goto kind0;
        goto kind2;

    L10_80148BE0:
        if (id >= 0x103)
            goto L10_80148C40;
        if (id == 0xeb)
            goto kind2;
        if (id >= 0xeb)
            goto L10_80148C1C;
        if (id >= 0xdf)
            goto L10_80148C08;
        if (id == 0xdc)
            goto kind1;
        goto kind2;

    L10_80148C08:
        if (id >= 0xea)
            goto kind3;
        if (id >= 0xe3)
            goto kind2;
        goto kind1;

    L10_80148C1C:
        if (id == 0xf7)
            goto kind0;
        if (id >= 0xf7)
            goto L10_80148C34;
        if (id >= 0xed)
            goto kind3;
        goto kind0;

    L10_80148C34:
        if (id == 0xff)
            goto kind2;
        goto kind3;

    L10_80148C40:
        if (id == 0x117)
            goto kind1;
        if (id >= 0x117)
            goto L10_80148C74;
        if (id >= 0x109)
            goto L10_80148C60;
        if (id == 0x106)
            goto kind3;
        goto kind2;

    L10_80148C60:
        if (id >= 0x111)
            goto kind3;
        if (id >= 0x10d)
            goto kind2;
        goto kind3;

    L10_80148C74:
        if (id == 0x12d)
            goto kind0;
        if (id >= 0x12d)
            goto kind2;
        if (id == 0x11e)
            goto kind0;
        goto kind2;

    kind0:
        kind = 0;
        goto kind_done;
    kind1:
        kind = 1;
        goto kind_done;
    kind3:
        kind = 3;
        goto kind_done;
    kind2:
        kind = 2;
    kind_done:

        if (kind == 3) {
            *(u32*)self->unk1528 = 0;
        }

        // Retail schedules memset args into the first pair of the
        // word-copy (lwz r6/r0, mr dest, li val/len, stw pair hi/lo).
        s = (u32*)slot;
        a = s[0];
        clearPtr = slot;
        b = s[1];
        clearVal = 0;
        savedWords[1] = b;
        clearLen = 0x34;
        savedWords[0] = a;
        a = s[2];
        b = s[3];
        savedWords[3] = b;
        savedWords[2] = a;
        a = s[4];
        b = s[5];
        savedWords[5] = b;
        savedWords[4] = a;
        a = s[6];
        b = s[7];
        savedWords[7] = b;
        savedWords[6] = a;
        a = s[8];
        b = s[9];
        savedWords[9] = b;
        savedWords[8] = a;
        a = s[10];
        b = s[11];
        savedWords[11] = b;
        savedWords[10] = a;
        savedWords[12] = s[12];
        memset(clearPtr, clearVal, clearLen);

        // Load halfword id into a wide local first (retail lhz → r5).
        savedId = *(u16*)((u8*)savedWords + 0xc);
        if (savedId >= 0x12f) {
            stillActive = 0;
        } else {
            // Retail: found in r0, dead trip in r3 (li 0 / addi +7 /
            // unused after bdnz), scan base in r4.
            p = (u8*)self;
            trip = 0;
            stillActive = 0;
            for (g = thirteen; g != 0; g--) {
                if (savedId == *(u16*)(p + 0x14)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x48)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x7c)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0xb0)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0xe4)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x118)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x14c)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x180)) {
                    stillActive = 1;
                    goto scan_done;
                }
                p += 0x1a0;
                trip += 7;
            }
            stillActive = 0;
        scan_done:
            stillActive |= trip & 0;
        }

        if (stillActive == 0) {
            u8* wordPtr = self->unk15AC + ((savedId >> 3) & ~3u);
            *(u32*)wordPtr &= ~(one << (savedId & 0x1F));
        }

        ((Vfunc18Fn)(*(void***)self)[19])(
            self, (cf::CBattleStateEntry*)savedWords);
    }
}
