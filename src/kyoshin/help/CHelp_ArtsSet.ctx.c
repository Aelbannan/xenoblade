/* "src/kyoshin/help/CHelp_ArtsSet.cpp" line 0 "kyoshin/help/CHelp_ArtsSet.hpp" */
#pragma once

/* "src/kyoshin/help/CHelp_ArtsSet.hpp" line 2 "types.h" */
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
/* "src/kyoshin/help/CHelp_ArtsSet.hpp" line 3 "kyoshin/help/CHelp.hpp" */
#pragma once

/* "src/kyoshin/help/CHelp.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {

// Manual interface table at CHelp+0x8 (not a C++ vptr at +0).
struct CHelpVtbl {
    void* mSlots[8]; // +0x00..+0x1C
};

// Retail ctor writes: owner@0, param@4, vtbl@8. Base size is 0xC.
// Construction uses retail symbol __ct__Q22cf5CHelpFv(self, owner, param).
class CHelp {
public:
    void CHelp_UnkVirtualFunc2();
    void func_802B7C68();

    void* mOwner; // 0x0
    u32 mParam; // 0x4 (low byte used by UnkVirtualFunc2)
    CHelpVtbl* mVtbl; // 0x8
    CHelp(void* owner, u32 param);
};

// Flag helper sharing the CHelp prefix; flag byte at +0xC.
// CBC/CE4 keep Fv symbols via extern "C" in CHelp.cpp (arg in r4).
class CHelpSwitch : public CHelp {
public:
    void func_802B7CB0();

    u8 mFlag; // 0xC
    u32 func_802B7CBC(u32 flag);
    u32 func_802B7CE4(u8 flag);
};

} // namespace cf
/* end "kyoshin/help/CHelp.hpp" */

namespace cf {
// size: 0x10
class CHelp_ArtsSet : public CHelp {
public:
    bool func_802B7D9C();

    s32 unkC; // 0xC
};
} // namespace cf
/* end "kyoshin/help/CHelp_ArtsSet.hpp" */
/* "src/kyoshin/help/CHelp_ArtsSet.cpp" line 1 "kyoshin/cf/CfGameManager.hpp" */
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
    void func_80083100();
    void func_8008310C();
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

extern "C" bool func_8022F530();

namespace cf {
bool CHelp_ArtsSet::func_802B7D9C() {
    if (unkC != 0 && CfGameManager::func_800822F4() < unkC) {
        return false;
    }
    return func_8022F530();
}
} // namespace cf
