// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Five matched functions: completion callback, voice removal, slot-rotation
// advance, slot-2 play, and the standalone EHP voice selector.

/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.cpp" line 4 "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" */
#pragma once

/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" line 2 "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp" */
#pragma once

/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD.hpp" line 2 "types.h" */
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

class CVS_THREAD{
public:
    u32* unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;

    CVS_THREAD();

    //Virtual table (0x1c)
    virtual void func_802A3B50();
    virtual void func_802A3BEC();
    virtual int blank1();
    virtual void func_802A1EA0();
    virtual void func_802A3740();
    virtual int blank2();
    int func_802A5ECC() { return 240; }
};

extern void func_802A35A0(u32* destPtr);
/* end "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp" */
/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" line 3 "kyoshin/cf/voice/CCharVoice.hpp" */
#pragma once

/* "src/kyoshin/cf/voice/CCharVoice.hpp" line 2 "types.h" */
/* end "types.h" */

/**
 * CCharVoice -- single character voice playback instance.
 *
 * Each instance manages a voice file path and interacts with the sound
 * system to play/stop/update character voices.  The owner object decides
 * which voice profile (normal vs battle) is used.
 *
 * N.B.  The class is NOT declared with virtual functions even though it
 * has a vtable pointer at offset 0x3C.  The vtable (lbl_eu_805398B0) is
 * set up as assembly data and assigned manually in the constructor so
 * that the C-linkage symbol name __ct__CCharVoice is used (no C++
 * namespace mangling).
 *
 * Field layout (total size 0x40 = 64 bytes):
 *   0x00  mOwner            parent/owner object
 *   0x04  mVoiceId          current voice ID
 *   0x08  mPriorityCheck    priority value for play-through gate
 *   0x0C  mSoundHandle      handle from archive-voice sound system
 *   0x10  mFileName[0x20]   voice file path buffer (32 bytes)
 *   0x30  mFileNameLen      strlen of mFileName
 *   0x34  mField34          offset into mFileName for digit formatting
 *   0x38  mBattleSndHandle  sound handle for battle-voice path
 *   0x3C  mVtable           pointer to lbl_eu_805398B0 (vtable)
 */
struct CCharVoice {
    void* mOwner;            // 0x00
    s32   mVoiceId;          // 0x04
    s32   mPriorityCheck;    // 0x08
    s32   mSoundHandle;      // 0x0C
    char  mFileName[0x20];  // 0x10
    u32   mFileNameLen;      // 0x30
    s32   mField34;          // 0x34
    u16   mBattleSndHandle;  // 0x38
    // 2 bytes padding to 0x3C
    void* mVtable;           // 0x3C -- vtable pointer

    void func_802A0B8C(void* owner);
    void func_802A0E08();
    void func_802A0FE8();
    bool func_802A109C(float volume, int priority, int voiceId);
    void func_802A1304();
};
/* end "kyoshin/cf/voice/CCharVoice.hpp" */

struct CVoiceHandle;

// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Object size 0x48 (72 bytes). The buffer-size virtual (func_802A6818, the
// CVS_THREAD::blank1 slot) returns 0xB4 (180). The EHP vtable
// (lbl_eu_80539B2C) is assigned manually by the factory __ct__802A5ED4.
//
// Field layout (base CVS_THREAD occupies 0x00-0x1F, vtable at 0x1C):
//   0x20  field_0x20   voice handle slot 1 (owner1)
//   0x24  field_0x24   voice handle slot 2 (owner2)
//   0x28  field_0x28   third constructor parameter
//   0x2C  field_0x2c   voice handle array (3 slots)
//   0x38  field_0x38   current rotating index
//   0x3C  field_0x3c   slot count / wrap bound (index range 0..field_0x3c)
//   0x40  field_0x40   stop/target index (triggers playback virtual)
//   0x44  field_0x44   direction flag (0 = forward, nonzero = backward)
class CVS_THREAD_EHP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0xB4;

    CVoiceHandle* field_0x20;    // 0x20: voice handle slot 1
    CVoiceHandle* field_0x24;    // 0x24: voice handle slot 2
    s32 field_0x28;              // 0x28: third constructor parameter
    CVoiceHandle* field_0x2c[3]; // 0x2C: voice handle slots
    s32 field_0x38;              // 0x38: current rotating index
    s32 field_0x3c;              // 0x3C: slot count / wrap bound
    s32 field_0x40;              // 0x40: stop/target index
    u8 field_0x44;               // 0x44: direction flag
};

// Forward declaration of a polymorphic sub-object reached through a voice
// handle (CVoiceHandle+0x04). Only the vtable pointer at offset 0 is used.
struct CVSubObj {
    void** vtable;               // 0x00: vtable pointer
};

// Voice-handle type. The actual CCharVoice is embedded at offset 0x3E9C
// within the handle allocation (0x3E9C bytes of handle data + CCharVoice).
// Code biases a handle pointer by 0x3E9C to reach the embedded CCharVoice.
struct CVoiceHandle {
    void** vtable;               // 0x00: vtable pointer
    CVSubObj* field_0x04;        // 0x04: sub-object pointer (used by func_802A6820)
    u8 _pad[0x3E9C - 0x08];      // 0x08-0x3E9B: handle data
    CCharVoice voice;            // 0x3E9C: the actual voice object
};

// The voice manager owns a circular list of handles.  The list stores the
// embedded voice address, which the retail code converts back to its handle.
struct CVoiceListNode {
    CVoiceListNode* field_0x00;  // next node
    u32 field_0x04;
    CCharVoice* field_0x08;      // embedded CCharVoice address
};

struct CVoiceList {
    u32 field_0x00;
    CVoiceListNode* field_0x04;  // end sentinel
};

struct CVoiceManager {
    void** vtable;
    CVSubObj* field_0x04;
    u8 _pad[0x3E9C - 0x08];
    CCharVoice voice;
    u8 _tail[0x3F08 - 0x3EDC];
    u32 field_0x3F08;
};

// Sibling TU functions (unmangled global symbols).
int func_802A3E88(CVS_THREAD* self);
void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
CVoiceHandle* func_802A7998(CVoiceHandle* exclude);
CVoiceHandle* func_802A330C(int size, int align);
int func_80174C98(CVoiceHandle* handle, u32* value, int arg);
CVoiceList* func_800B6BC8();
int func_802A7FE4(CVoiceManager* self);
/* end "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" */
/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.cpp" line 5 "monolib/math/Random.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/Random.hpp" line 2 "types.h" */
/* end "types.h" */

namespace ml{
    namespace math{
        void mtInit(u32 seed);
        int mtRand();
        int mtRand(int max);
        int mtRand(int min,int max);
    } //namespace math
} //namespace ml
/* end "monolib/math/Random.hpp" */

// Init-data tables (3 u32s each: {0, -1, callback}). Each slot state carries
// a {field_0, field_4, callback} triple that the rotation function copies into
// unk0/unk4/unk8 when the rotating index is not at the stop index.
extern "C" u32 lbl_eu_80539B14[3]; // {0, -1, func_802A6408}
extern "C" u32 lbl_eu_80539B20[3]; // {0, -1, func_802A6718}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; EHP subclass returns 0xB4 (180).
int func_802A6818() {
    return 0xB4;
}

// ── Target 1: us-802a8e4c (func_802A6718) ──────────────────────────────────
// Completion callback: if no active voice is playing, invoke the
// playback-start virtual (CVS_THREAD::func_802A3B50, vtable slot 0).
void func_802A6718(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802a8e94 (func_802A6760) ──────────────────────────────────
// Remove a voice from all slots by matching its embedded CCharVoice pointer.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void func_802A6760(CVS_THREAD_EHP* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    // Slot 0x20
    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }

    // Slot 0x24
    handle = self->field_0x24;
    biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x24 = NULL;
    }

    // Rotating slots 0x2C..0x34 (count in field_0x3c)
    for (int i = 0; i < self->field_0x3c; i++) {
        handle = self->field_0x2c[i];
        biased = (CCharVoice*)handle;
        if (handle != NULL) {
            biased = &handle->voice;
        }
        if (biased == voicePtr) {
            self->field_0x2c[i] = NULL;
        }
    }
}

// ── Target 3: us-802a8cc0 (func_802A658C) ──────────────────────────────────
// Advance the rotating index (field_0x38) forward or backward depending on the
// direction flag (field_0x44), wrapping at the bounds (0..field_0x3c). When the
// index lands on the stop index (field_0x40), the playback-start virtual is
// invoked; otherwise the slot-state triple is reloaded from lbl_eu_80539B14.
void func_802A658C(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    if (self->field_0x44 == 0) {
        // Forward: increment, wrap to 0 once past the bound.
        int newIdx = self->field_0x38 + 1;
        self->field_0x38 = newIdx;
        if (self->field_0x3c < newIdx) {
            self->field_0x38 = 0;
        }
    } else {
        // Backward: decrement, wrap to bound-1 on underflow.
        int newIdx = self->field_0x38 - 1;
        self->field_0x38 = newIdx;
        if (newIdx < 0) {
            self->field_0x38 = self->field_0x3c - 1;
        }
    }

    if (self->field_0x38 == self->field_0x40) {
        self->func_802A3B50();
    } else {
        // Reload slot-state triple {field_0, field_4, callback}.
        self->unk4 = lbl_eu_80539B14[1];
        self->unk0 = (u32*)lbl_eu_80539B14[0];
        self->unk8 = lbl_eu_80539B14[2];
    }
}

// ── Target 4: us-802a8d84 (func_802A6650) ──────────────────────────────────
// Play function for slot 2. Reloads the slot-state triple from lbl_eu_80539B20,
// finds a free voice handle (excluding slot 2's own handle), and if that handle
// is inactive plays a random voice ID (mtRand(2) + 0x51D). On any failure the
// playback-start virtual is invoked as a fallback.
void func_802A6650(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    // Reload slot-state triple {field_0, field_4, callback}.
    self->unk4 = lbl_eu_80539B20[1];
    self->unk0 = (u32*)lbl_eu_80539B20[0];
    self->unk8 = lbl_eu_80539B20[2];

    CVoiceHandle* handle = func_802A7998(self->field_0x24);
    if (handle != NULL) {
        // is-active check via the handle's vtable (offset 0x2BC).
        typedef int (*IsActiveFunc)(CVoiceHandle*);
        IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
        if (isActive(handle) == 0) {
            // Voice is not active -- play a random voice ID.
            CCharVoice* voicePtr = (CCharVoice*)handle;
            if (handle != NULL) {
                voicePtr = &handle->voice;
            }
            int voiceId = ml::math::mtRand(2) + 0x51D;
            if (func_802A3C44(self, voicePtr, voiceId) != 0) {
                return;
            }
        }
    }

    // Fallback: invoke the playback-start virtual.
    self->func_802A3B50();
}

// ── Target 5: us-802a8f54 (func_802A6820) ──────────────────────────────────
// Standalone EHP voice selector. Given two party-slot indices, it selects and
// starts the corresponding recovery voice on an available voice handle.
int func_802A6820(int first, int second) {
    if (first == second) {
        return 0;
    }

    CVoiceHandle* handle = func_802A7998(NULL);
    if (handle == NULL) {
        return 0;
    }

    typedef int (*IsActiveFunc)(CVoiceHandle*);
    IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
    if (isActive(handle) != 0) {
        return 0;
    }

    CVSubObj* subobj = handle->field_0x04;
    typedef u32* (*GetValueFunc)(CVSubObj*);
    GetValueFunc getValue = (GetValueFunc)subobj->vtable[0x30 / 4];
    u32 value = *getValue(subobj);
    if (func_80174C98(handle, &value, 0x803) == 0) {
        return 0;
    }

    if (second < first && first >= 2) {
        second = ml::math::mtRand(2) + 0x6A5;
    } else if (first < second && first == 1) {
        second = 0x6A7;
    } else {
        return 0;
    }

    if (func_802A330C(0xAA, 1) == NULL) {
        return 0;
    }

    CCharVoice* voice = (CCharVoice*)handle;
    if (handle != NULL) {
        voice = &handle->voice;
    }
    func_802A3D54(voice, second, 0xAA);
    return 0;
}

// ── Target 6: us-802a908c (func_802A6958) ──────────────────────────────────
// If the manager is enabled, find an eligible inactive voice in its list,
// reserve the short playback buffer, then prepare and play the EHP line.
int func_802A6958(CVoiceManager* manager) {
    int found = 0;
    if ((manager->field_0x3F08 & 0x8000) == 0) {
        return 0;
    }

    typedef int (*IsActiveFunc)(CVoiceHandle*);
    CVoiceHandle* managerHandle = (CVoiceHandle*)manager;
    IsActiveFunc isActive = (IsActiveFunc)managerHandle->vtable[0x2BC / 4];
    if (isActive(managerHandle) != 0) {
        return 0;
    }

    CVoiceList* list = func_800B6BC8();
    CVoiceListNode* node = list->field_0x04->field_0x00;
    while (node != list->field_0x04) {
        CVoiceHandle* handle = (CVoiceHandle*)node->field_0x08;
        if (handle != NULL) {
            handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
        }

        isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
        if (isActive(handle) == 0) {
            CVSubObj* subobj = handle->field_0x04;
            typedef u32* (*GetValueFunc)(CVSubObj*);
            GetValueFunc getValue = (GetValueFunc)subobj->vtable[0x30 / 4];
            u32 value = *getValue(subobj);
            if (func_80174C98(handle, &value, 0x803) != 0) {
                found = 1;
                break;
            }
        }
        node = node->field_0x00;
    }

    if (found == 0) {
        return 0;
    }
    if (func_802A330C(0x28, 1) == NULL) {
        return 0;
    }
    if (func_802A7FE4(manager) != 0) {
        return 0;
    }

    CCharVoice* voice = &manager->voice;
    func_802A3D54(voice, 0xA8D, 0x28);
    return 0;
}

// Stubs for functions not yet decompiled in this TU.
void __ct__802A5ED4() {}
void func_802A617C() {}
void func_802A6408() {}
