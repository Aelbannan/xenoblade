/* "src/kyoshin/help/CHelp_ArtsAttack.cpp" line 0 "kyoshin/help/CHelp_ArtsAttack.hpp" */
#pragma once

/* "src/kyoshin/help/CHelp_ArtsAttack.hpp" line 2 "types.h" */
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
/* "src/kyoshin/help/CHelp_ArtsAttack.hpp" line 3 "kyoshin/help/CHelp.hpp" */
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

    void* mOwner; // 0x0
    u32 mParam; // 0x4 (low byte used by UnkVirtualFunc2)
    CHelpVtbl* mVtbl; // 0x8
};

// Flag helper sharing the CHelp prefix; flag byte at +0xC.
// CBC/CE4 keep Fv symbols via extern "C" in CHelp.cpp (arg in r4).
class CHelpSwitch : public CHelp {
public:
    void func_802B7CB0();

    u8 mFlag; // 0xC
};

} // namespace cf
/* end "kyoshin/help/CHelp.hpp" */

namespace cf {

// size: 0xC (no extra members beyond CHelp).
class CHelp_ArtsAttack : public CHelp {
public:
    bool func_802B7D00();
};

} // namespace cf/* end "kyoshin/help/CHelp_ArtsAttack.hpp" */
/* "src/kyoshin/help/CHelp_ArtsAttack.cpp" line 1 "kyoshin/cf/CfGameManager.hpp" */
#pragma once

/* "src/kyoshin/cf/CfGameManager.hpp" line 2 "types.h" */
/* end "types.h" */

class CPad;
class CScnNw4r;
class CView;

/* TODO: it's possible this file contains multiple separate classes, either just all being put in here,
or due to being in separate files, but compiled together in one file (unity compilation). For now,
to make things simpler, everything exists in a single class. */
namespace cf{
    class CfPadData;
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
        static UNKWORD func_800829B8();
        static u32 getCurrentPadChannel();
        static UNKTYPE* func_80083298();
        static CfObjectMove* func_80082D54(int playerIndex);
        static u32 getEnabledInputFlags();
        static bool func_80086F9C(s16);
        static void setCurrentPadPtr(const CPad* pPad, u32 r4);
        static CPad* getPad(int r3);
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
        u32 unk4;
        u32 unk8;
        u8 unkC[0x28 - 0xC];
        u8 unk28;
        u8 unk29[0x68 - 0x29];
        u32 unk68;
        u8 unk6C;
        u8 unk6D[0x7C - 0x6D];
        u32 unk7C;
        u8 unk80[0x8C - 0x80];
        u32 unk8C;
        u32 unk90;
        //between CObjectParam - CfObjectMove
        //likely player character object array, seems to always store pointers
        //to CfObjectPc objects except pointing at the 4th vtable
        CfObjectMove* unk94[3];
        u32 unkA0;
        u32 unkA4;
        u32 unkA8;
        u32 unkAC;
        u32 unkB0;
        u32 unkB4;

        static u32 sUnkFlags;
        static CScnNw4r* spScene;
    }; //size = 0xB8

} //namespace cf
/* end "kyoshin/cf/CfGameManager.hpp" */
/* "src/kyoshin/help/CHelp_ArtsAttack.cpp" line 2 "kyoshin/cf/object/CfObjectPc.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObjectPc.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObjectPc.hpp" line 3 "kyoshin/cf/object/CfObjectActor.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CfObjectActor.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/object/CfObjectActor.hpp" line 3 "kyoshin/cf/object/CfObjectMove.hpp" */
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
        virtual void CObjectState_UnkVirtualFunc12(); //0x34
        virtual void CObjectState_UnkVirtualFunc13(); //0x38

        //0x0: vtable
        u8 unk4_3[0xC];
    };
}
/* end "kyoshin/cf/object/CObjectState.hpp" */

namespace cf {
    //min size: 0x38
    class CObjectParam : public CObjectState {
    public:
        virtual void CObjectParam_UnkVirtualFunc1(); //0x3C
        virtual void CObjectParam_UnkVirtualFunc2(); //0x40
        virtual void CObjectParam_UnkVirtualFunc3(); //0x44
        virtual void CObjectParam_UnkVirtualFunc4(); //0x48
        virtual BOOL CObjectParam_UnkVirtualFunc5(); //0x4C
        virtual void CObjectParam_UnkVirtualFunc6(); //0x50

        //0x0: vtable
        //0x0-10: CObjectState
        u8 unk10_3[0x28];
    };
}
/* end "kyoshin/cf/object/CObjectParam.hpp" */

namespace cf {
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
        virtual void CfObject_UnkVirtualFunc20();     //0xA0
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
        virtual void CfObject_UnkVirtualFunc53();     //0x124
        virtual void CfObject_UnkVirtualFunc54();     //0x128
        virtual void CfObject_UnkVirtualFunc55();     //0x12C
        virtual void CfObject_UnkVirtualFunc56();     //0x130
        virtual void CfObject_UnkVirtualFunc57();     //0x134
        virtual void CfObject_UnkVirtualFunc58();     //0x138
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
        //0x0-38: CObjectParam
        u8 unk38_3[0x64 - 0x38];
        u32 unk64;
        u8 unk68[0x70 - 0x68];
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
        //0x0-70: CfObject
        u8 unk70_3[0x1C];
        u16 unk8C_3;
        u8 unk8E_3[0x30];
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
        u8 unkBE_3[0x657];
        u8 unk715[3]; //might not belong here
    };
}
/* end "kyoshin/cf/object/CfObjectMove.hpp" */
/* "src/kyoshin/cf/object/CfObjectActor.hpp" line 4 "kyoshin/cf/object/CAIAction.hpp" */
#pragma once

/* "src/kyoshin/cf/object/CAIAction.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {

// 0x20-byte AI action slot (slots[] / trailer / export buffer)
struct CAIActionSlot {
    u32 unk00; // 0x00
    u32 unk04; // 0x04
    u32 unk08; // 0x08
    u32 unk0C; // 0x0C
    u16 unk10; // 0x10
    s16 unk12; // 0x12
    f32 unk14; // 0x14
    u32 unk18; // 0x18
    u32 unk1C; // 0x1C
};

// symbols.txt mangles Fv; retail passes out buffers in r4/r5
struct CAIActionExport {
    u8 pad[0x200];
    CAIActionSlot* buffer; // 0x200
    u32 unk204;            // 0x204
    u32 unk208;            // 0x208
    u32 unk20C;            // 0x20C - column stride
};

// size: 0xB1C
class CAIAction {
public:
    CAIAction();

    // Declared Fv for vtable; body is extern "C" with outA/outB args
    virtual void CAIAction_UnkVirtualFunc1(); // 0x8
    virtual void CAIAction_UnkVirtualFunc2(); // 0xC

    // 0x0: vtable
    u32 unk4;          // 0x4
    u16 unk8;          // 0x8
    u8 padA[2];        // 0xA
    u8 slots[0x200];   // 0xC .. 0x20C (16 * 0x20)
    void* unk20C;      // 0x20C - pointer to slots
    u32 unk210;        // 0x210
    u32 unk214;        // 0x214
    u32 unk218;        // 0x218 - capacity (0x10)
    u8 entries[0x8C0]; // 0x21C .. 0xADC (160 * 0xE)
    u8 trailer[0x20];  // 0xADC .. 0xAFC (CAIActionSlot)
    u32 unkAFC;        // 0xAFC
    u8 unkB00;         // 0xB00
    u8 padB01[0xF];    // 0xB01
    u32 unkB10;        // 0xB10
    u32 unkB14;        // 0xB14
    u32 unkB18;        // 0xB18
};

} // namespace cf

// Fv mangling, but callers leave outA in r4 and outB in r5
extern "C" void CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* outA,
                                                              cf::CAIActionExport* outB);

// Inverse of UnkVirtualFunc1: imports trailer from inA, then imports ring
// entries from inB into this->unk20C. Same Fv mangling / r4-r5 ABI.
extern "C" void CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* inA,
                                                              cf::CAIActionExport* inB);

extern void func_8014A86C(void*);
extern void func_8014A8F8();
/* end "kyoshin/cf/object/CAIAction.hpp" */
/* "src/kyoshin/cf/object/CfObjectActor.hpp" line 5 "kyoshin/cf/object/CActorParam.hpp" */
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
        u8 unk3F; //padding?
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
        u8 unk71; //filler?
        u16 unk72;
        u16 unk74;
        u8 unk76; //filler?
        u8 unk77;
        u32 unk78;
        float unk7C;
        float unk80;
        //0x84: vtable

        CAttackParam();

        virtual void CAttackParam_UnkVirtualFunc1(){ //0x8
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
        virtual u8 CAttackParam_UnkVirtualFunc2(); //0xC
        virtual void CAttackParam_UnkVirtualFunc3(u8 r4); //0x10
        virtual void CAttackParam_UnkVirtualFunc4(); //0x14

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
        //0x0: vtable
        //0x0-0x88: CAttackParam
        UNKTYPE* unk88;

        CArtsParam();
        virtual void CArtsParam_UnkVirtualFunc1();
        virtual u8 CArtsParam_UnkVirtualFunc2();
        virtual void CArtsParam_UnkVirtualFunc3(u8 r4);
    };

    //size: 0x38
    struct _sArtsSet {
        u16 unk0;
        u8 unk2[2];
        u8 unk4[0x30];
        //0x34: vtable

        _sArtsSet();
        virtual void _sArtsSet_UnkVirtualFunc1(){ //0x8
            unk0 = 0;
            std::memset(unk4, 0, sizeof(unk4));
        }
    };

    //size: 0xD58
    class CArtsSet : _sArtsSet {
    public:
        CArtsSet(){}
        virtual void CArtsSet_UnkVirtualFunc1(); //0x8

        //0x0: vtable
        //0x0-38: _sArtsSet
        CArtsParam mArtsParams[24]; //0x38
    };
};
/* end "kyoshin/cf/CArtsSet.hpp" */
/* "src/kyoshin/cf/object/CActorParam.hpp" line 7 "cstring" */
/* end "cstring" */

namespace cf {

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
        virtual void CActorParam_UnkVirtualFunc14();  //0xCC
        virtual void CActorParam_UnkVirtualFunc15();  //0xD0
        virtual void CActorParam_UnkVirtualFunc16();  //0xD4
        virtual void CActorParam_UnkVirtualFunc17();  //0xD8
        virtual void CActorParam_UnkVirtualFunc18();  //0xDC
        virtual int CActorParam_UnkVirtualFunc19();  //0xE0
        virtual void CActorParam_UnkVirtualFunc20();  //0xE4
        virtual void CActorParam_UnkVirtualFunc21();  //0xE8
        virtual void CActorParam_UnkVirtualFunc22();  //0xEC
        virtual void CActorParam_UnkVirtualFunc23();  //0xF0
        virtual void CActorParam_UnkVirtualFunc24();  //0xF4
        virtual void CActorParam_UnkVirtualFunc25();  //0xF8
        virtual void CActorParam_UnkVirtualFunc26();  //0xFC
        virtual void CActorParam_UnkVirtualFunc27();  //0x100
        virtual void CActorParam_UnkVirtualFunc28();  //0x104
        virtual void CActorParam_UnkVirtualFunc29();  //0x108
        virtual void CActorParam_UnkVirtualFunc30();  //0x10C
        virtual void CActorParam_UnkVirtualFunc31();  //0x110
        virtual void CActorParam_UnkVirtualFunc32();  //0x114
        virtual void CActorParam_UnkVirtualFunc33();  //0x118
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
        virtual void CActorParam_UnkVirtualFunc55();  //0x170
        virtual void CActorParam_UnkVirtualFunc56();  //0x174
        virtual void CActorParam_UnkVirtualFunc57();  //0x178
        virtual void CActorParam_UnkVirtualFunc58();  //0x17C
        virtual void CActorParam_UnkVirtualFunc59();  //0x180
        virtual void CActorParam_UnkVirtualFunc60();  //0x184
        virtual void CActorParam_UnkVirtualFunc61();  //0x188
        virtual void CActorParam_UnkVirtualFunc62();  //0x18C
        virtual void CActorParam_UnkVirtualFunc63();  //0x190
        virtual void CActorParam_UnkVirtualFunc64();  //0x194
        virtual void CActorParam_UnkVirtualFunc65();  //0x198
        virtual void CActorParam_UnkVirtualFunc66();  //0x19C
        virtual void CActorParam_UnkVirtualFunc67();  //0x1A0
        virtual void CActorParam_UnkVirtualFunc68();  //0x1A4
        virtual void CActorParam_UnkVirtualFunc69();  //0x1A8
        virtual void CActorParam_UnkVirtualFunc70();  //0x1AC
        virtual void CActorParam_UnkVirtualFunc71();  //0x1B0
        virtual void CActorParam_UnkVirtualFunc72();  //0x1B4
        virtual void CActorParam_UnkVirtualFunc73();  //0x1B8
        virtual void CActorParam_UnkVirtualFunc74();  //0x1BC
        virtual void CActorParam_UnkVirtualFunc75();  //0x1C0
        virtual void CActorParam_UnkVirtualFunc76();  //0x1C4
        virtual void CActorParam_UnkVirtualFunc77();  //0x1C8
        virtual void CActorParam_UnkVirtualFunc78();  //0x1CC
        virtual void CActorParam_UnkVirtualFunc79();  //0x1D0
        virtual void CActorParam_UnkVirtualFunc80();  //0x1D4
        virtual void CActorParam_UnkVirtualFunc81();  //0x1D8
        virtual void CActorParam_UnkVirtualFunc82();  //0x1DC
        virtual void CActorParam_UnkVirtualFunc83();  //0x1E0
        virtual void CActorParam_UnkVirtualFunc84();  //0x1E4
        virtual void CActorParam_UnkVirtualFunc85();  //0x1E8
        virtual void CActorParam_UnkVirtualFunc86();  //0x1EC
        virtual void CActorParam_UnkVirtualFunc87();  //0x1F0
        virtual void CActorParam_UnkVirtualFunc88();  //0x1F4
        virtual void CActorParam_UnkVirtualFunc89();  //0x1F8
        virtual void CActorParam_UnkVirtualFunc90();  //0x1FC
        virtual void CActorParam_UnkVirtualFunc91();  //0x200
        virtual void CActorParam_UnkVirtualFunc92();  //0x204
        virtual void CActorParam_UnkVirtualFunc93();  //0x208
        virtual void CActorParam_UnkVirtualFunc94();  //0x20C
        virtual void CActorParam_UnkVirtualFunc95();  //0x210
        virtual void CActorParam_UnkVirtualFunc96();  //0x214
        virtual void CActorParam_UnkVirtualFunc97();  //0x218
        virtual void CActorParam_UnkVirtualFunc98();  //0x21C
        virtual void CActorParam_UnkVirtualFunc99();  //0x220
        virtual void CActorParam_UnkVirtualFunc100(); //0x224
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
        virtual void CActorParam_UnkVirtualFunc113(); //0x258
        virtual void CActorParam_UnkVirtualFunc114(); //0x25C
        virtual void CActorParam_UnkVirtualFunc115(); //0x260
        virtual void CActorParam_UnkVirtualFunc116(); //0x264
        virtual void CActorParam_UnkVirtualFunc117(); //0x268
        virtual void CActorParam_UnkVirtualFunc118(); //0x26C
        virtual void CActorParam_UnkVirtualFunc119(); //0x270
        virtual void CActorParam_UnkVirtualFunc120(); //0x274
        virtual void CActorParam_UnkVirtualFunc121(); //0x278
        virtual void CActorParam_UnkVirtualFunc122(); //0x27C
        virtual void CActorParam_UnkVirtualFunc123(); //0x280
        virtual void CActorParam_UnkVirtualFunc124(); //0x284
        virtual void CActorParam_UnkVirtualFunc125(); //0x288
        virtual void CActorParam_UnkVirtualFunc126(); //0x28C
        virtual void CActorParam_UnkVirtualFunc127(); //0x290
        virtual void CActorParam_UnkVirtualFunc128(); //0x294
        virtual CActorParam_UnkStruct1* CActorParam_UnkVirtualFunc129(); //0x298
        virtual void CActorParam_UnkVirtualFunc130(); //0x29C
        virtual void CActorParam_UnkVirtualFunc131(); //0x2A0
        virtual void CActorParam_UnkVirtualFunc132(); //0x2A4
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
        virtual void CActorParam_UnkVirtualFunc152(); //0x2F4
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
        virtual void CActorParam_UnkVirtualFunc165(); //0x328
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
        UNKTYPE* unk15E0;
        u32 unk15E4;
        float unk15E8;
        u32 unk15EC;
        u32 unk15F0;
        u8 unk15F4[8];
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
    };
}
/* end "kyoshin/cf/object/CActorParam.hpp" */

namespace cf {
    //min size: 0x45BC
    class CfObjectActor : public CActorParam, public CAIAction, public CfObjectMove {
    public:
        CfObjectActor();
        //vtable 4 (CfObjectActor)
        virtual ~CfObjectActor();                  //0x5A0
        virtual void CfObjectActor_UnkVirtualFunc2();  //0x5A4
        virtual void CfObjectActor_UnkVirtualFunc3();  //0x5A8
        virtual void CfObjectActor_UnkVirtualFunc4();  //0x5AC
        virtual void CfObjectActor_UnkVirtualFunc5();  //0x5B0
        virtual void CfObjectActor_UnkVirtualFunc6();  //0x5B4
        virtual void CfObjectActor_UnkVirtualFunc7();  //0x5B8
        virtual void CfObjectActor_UnkVirtualFunc8();  //0x5BC
        virtual void CfObjectActor_UnkVirtualFunc9();  //0x5C0
        virtual void CfObjectActor_UnkVirtualFunc10(); //0x5C4
        virtual void CfObjectActor_UnkVirtualFunc11(); //0x5C8
        virtual void CfObjectActor_UnkVirtualFunc12(); //0x5CC
        virtual void CfObjectActor_UnkVirtualFunc13(); //0x5D0

        //0x0: vtable 1
        //0x0-3380: CActorParam
        //0x3380: vtable 2
        //0x3380-3e9c: CAIAction
        //0x3e9c: vtable 3
        //0x3e9c-45b4: CfObjectMove
        u8 unk45B4[0x8];
    };
}
/* end "kyoshin/cf/object/CfObjectActor.hpp" */

extern UNKTYPE* func_800B708C(BOOL r3);
extern UNKTYPE* func_800AD860(UNKTYPE* r3);
extern UNKTYPE* func_800C1228(UNKTYPE* r3);

namespace cf {
    //size: 0x45C0
    class CfObjectPc : public CfObjectActor {
    public:
        virtual ~CfObjectPc();
        virtual void func_800BFF20(); //0x5D4
        virtual void func_800C0080(); //0x5D8
        virtual void func_800C0524(); //0x5DC
        virtual void func_800BFFEC(); //0x5E0
        virtual void func_800C00C0(); //0x5E4
        virtual void func_800C0174(); //0x5E8
        virtual void func_800C02C4(); //0x5EC
        virtual void func_800C02EC(); //0x5F0
        virtual void func_800C032C(); //0x5F4
        virtual void func_800C03A8(); //0x5F8
        virtual void func_800C0474(); //0x5FC
        virtual void func_800C0504(); //0x600
        virtual void func_800C0514(); //0x604
        virtual void func_800C11CC(); //0x608

        inline UNKTYPE* unkInline1(){
            BOOL thing = CObjectParam_UnkVirtualFunc5();
            UNKTYPE* idk = func_800B708C(thing);
            return func_800AD860(idk);
        }

        inline UNKTYPE* unkInline2(){
            BOOL thing2 = CObjectParam_UnkVirtualFunc5();
            UNKTYPE* idk2 = func_800B708C(thing2);
            return func_800C1228(idk2);
        }

        //0x0: vtable
        //0x0-45BC: CfObjectActor
        u8 unk45BC[0x4];
    };
}
/* end "kyoshin/cf/object/CfObjectPc.hpp" */

extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);

extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);

namespace cf {

typedef void* (*GetPtrFn)(void*);

bool CHelp_ArtsAttack::func_802B7D00() {
    CfObjectPc* objPc = func_800BFC68(CfGameManager::func_80082D54(0));

    if (objPc != nullptr) {
        void* sub = *reinterpret_cast<void**>(reinterpret_cast<u8*>(objPc) + 4);
        u32 localVal = *reinterpret_cast<u32*>(
            reinterpret_cast<GetPtrFn>((*reinterpret_cast<void***>(sub))[0x30 / 4])(sub));

        u32 funcResult = func_80174C98(objPc, &localVal, 0xa);

        CHelpVtbl* vt = mVtbl;
        return reinterpret_cast<bool (*)(CHelp*, u32)>(vt->mSlots[7])(
            this, static_cast<u32>(funcResult == 0));
    }

    return false;
}

} // namespace cf