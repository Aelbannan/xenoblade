// CVS_THREAD_TENSION_UP: Voice thread for tension-up audio events.
// FULL_MATCH: func_802A92D0 -- buffer-size getter (virtual method override).
// 5 matched functions: completion callback, voice removal, constructor,
// update function, and voice-select + play function.

/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.cpp" line 5 "kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.hpp" */
#pragma once

/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.hpp" line 2 "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp" */
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
/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.hpp" line 3 "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" */
#pragma once

/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" line 2 "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp" */
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

// Sibling TU functions (unmangled global symbols).
int func_802A3E88(CVS_THREAD* self);
void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg);
CVoiceHandle* func_802A7998(CVoiceHandle* exclude);
CVoiceHandle* func_802A330C(int size, int align);
int func_80174C98(CVoiceHandle* handle, u32* value, int arg);
/* end "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" */

/**
 * CVS_THREAD_TENSION_UP - voice thread for tension-up audio events.
 *
 * Inherits from CVS_THREAD and returns a smaller thread-local buffer
 * (130 bytes) compared to other variants like BUF (260) or FAINT (240).
 */
class CVS_THREAD_TENSION_UP : public CVS_THREAD {
public:
    static const int BUFFER_SIZE = 0x82;

    CVoiceHandle* field_0x20;  // 0x20: back-pointer to owning manager
    s32 field_0x24;            // 0x24: thread index
    u8 field_0x28;             // 0x28: flag (0 = normal, 1 = reversed)

    int blank1() override;
};
/* end "kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.hpp" */
/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.cpp" line 6 "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" */
/* end "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp" */
/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.cpp" line 7 "kyoshin/cf/CfGameManager.hpp" */
#pragma once

/* "include/kyoshin/cf/CfGameManager.hpp" line 2 "types.h" */
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
        static CfObjectMove* getPlayer(int playerIndex);
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
        u8 unk6D[3];          // 0x6D-0x6F
        u32 unk70;             // 0x70-0x73
        u8 unk74[8];           // 0x74-0x7B
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
    virtual ~CfGameManager() {}
    void func_8007C5B8();
    cf::CfObjectMove** func_8007C6B4(cf::CfObjectMove** slots, int index);
    void func_8007C6C0();
    void func_8007C8C8();
    void func_8007CBC8();
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
    void func_8007DA00();
    void func_8007DA0C();
    void func_8007DCA8();
    void func_8007DCB8();
    void func_8007DE94();
    void func_8007DECC();
    void func_8007E030();
    void func_8007E038();
    void func_8007E0C8();
    void func_8007E0D0();
    void func_8007E4CC();
    void func_8007E4DC();
    void func_8007E864();
    void func_8007E908();
    void func_8007E960();
    void func_8007E9CC();
    void func_8007EEE0();
    void func_8007EEF0();
    void func_8007EEF8();
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
    void func_8007F8B8();
    void func_8007F8C0();
    void func_8007F8D0();
    void func_8007F8DC();
    void func_8007F8F4();
    void func_8007F900();
    void func_8007F91C();
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
    void func_80080E20();
    void func_80080E28();
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
    void func_80082104();
    void func_8008212C();
    void func_800821F8();
    void func_8008221C();
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
    void func_80082680();
    void func_80082694();
    void func_8008269C();
    void func_800826F0();
    void func_80082768();
    void func_80082770();
    void func_800827A8();
    void func_800827E4();
    void func_80082834();
    void func_800828DC();
    void func_80082900();
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
    void func_80083458();
    void func_80083460();
    void func_80083468();
    void func_80083470();
    void func_80083538();
    void func_80083544();
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
    void func_80084B68();
    void func_80084BAC();
    void func_80084BF4();
    void func_80084C10();
    void func_80084CA4();
    void func_80084F50();
    void func_80085220();
    void func_80085248();
    void func_80085334();
    void func_800853C8();
    void func_8008566C();
    void func_80085838();
    void func_80085840();
    void func_8008585C();
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
    void func_80086DA0();
    void func_80086DA4();
    void func_80086DA8();
    void func_80086DAC();
    void func_80086DB0();
    void func_80086DB4();
    void func_80086DBC();
    void func_80086E6C();
    void func_80087244();
    void func_80087250();
    void func_80087280();
    void func_80087330();
    void func_80087334();
    void func_80087348();
    void func_80087364();
    void func_80087378();
    void func_80087390();
    void func_800873AC();
    void func_800873C8();
    void func_800873D4();
    void func_800873E8();
    void func_800873FC();
    void func_80087410();
    void func_80087424();
    void func_8008742C();
    void func_8008743C();
    void func_80087448();
    }; //size = 0xB8
} // namespace cf
/* end "kyoshin/cf/CfGameManager.hpp" */
/* "src/kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP.cpp" line 8 "kyoshin/harness_catalog.hpp" */
#pragma once

/**
 * Umbrella for auto-scaffolded kyoshin catalog TUs that lack a unit header.
 *
 * Pulls recovered VM / script-helper headers only. Plugin units with their own
 * header (ocUnit.hpp, ocBuiltin.hpp, …) should include that instead.
 */

/* "src/kyoshin/harness_catalog.hpp" line 9 "types.h" */
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

// Forward declarations for external functions not yet in headers.
extern char* func_802A34E4(int size);
extern void __ct__cf_CVS_THREAD();
extern int func_802A77E8(CVoiceHandle* handle);
extern int func_802A7B90(CVoiceHandle* handle, CVoiceHandle* owner);

// The voice manager/factory object. The flags field at offset 0x3F00
// controls thread construction (bit 1 = TENSION_UP factory active).
struct CVoiceFactory {
    char _pad[0x3F00];
    u32 flags;
};

// Init-data tables (3 u32s each: {field_0, field_4, callback}).
extern "C" u32 lbl_eu_80539D20[3];
extern "C" u32 lbl_eu_80539D2C[3];
extern "C" u32 lbl_eu_80539D38[3];
extern "C" u32 lbl_eu_80539D44;  // vtable for CVS_THREAD_TENSION_UP

// ── Target 1: us-802ab968 (func_802A9230) ──────────────────────────────────
// Completion callback: if no active voice is playing, invoke the
// playback-start virtual (CVS_THREAD::func_802A3B50, vtable slot 2).
void func_802A9230(CVS_THREAD_TENSION_UP* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802ab9b0 (func_802A9278) ──────────────────────────────────
// Remove a voice by matching its embedded CCharVoice pointer against the
// single slot (field_0x20). A CVoiceHandle stores the CCharVoice at offset
// 0x3E9C, so a non-null handle is biased by 0x3E9C before comparing.
void func_802A9278(CVS_THREAD_TENSION_UP* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }
}

// ── Target 3: us-802ab51c (__ct__802A8DE8) ─────────────────────────────────
// Factory constructor for CVS_THREAD_TENSION_UP.
// Takes a factory/manager pointer and a thread index (must be >= 3).
// Allocates a buffer and the thread object, constructs the base,
// sets vtable/fields, copies init data, returns the object (or NULL).
CVS_THREAD_TENSION_UP* __ct__802A8DE8(CVoiceFactory* factory, int index) {
    if ((factory->flags & 0x2) == 0) {
        return NULL;
    }
    if (index < 3) {
        return NULL;
    }
    if (func_802A330C(0x82, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_TENSION_UP* obj = (CVS_THREAD_TENSION_UP*)func_802A34E4(0x2c);
    if (obj == NULL) {
        return NULL;
    }
    __ct__cf_CVS_THREAD();

    // Set the vtable for this subclass.
    obj->unk0 = (u32*)&lbl_eu_80539D44;

    // Set fields.
    obj->field_0x20 = (CVoiceHandle*)factory;
    obj->field_0x24 = index;
    obj->field_0x28 = 0;

    // Copy the slot-state init data triple into the object's first 3 u32s.
    obj->unk4 = lbl_eu_80539D20[1];
    obj->unk0 = (u32*)lbl_eu_80539D20[0];
    obj->unk8 = lbl_eu_80539D20[2];

    return obj;
}

// ── Target 4: us-802ab628 (func_802A8EEC) ──────────────────────────────────
// Update function: reloads the slot-state triple, checks voice state,
// plays appropriate voice ID (0x5DE standard or 0x5DD reversed).
void func_802A8EEC(CVS_THREAD_TENSION_UP* self) {
    self->unk4 = lbl_eu_80539D2C[1];
    self->unk0 = (u32*)lbl_eu_80539D2C[0];
    self->unk8 = lbl_eu_80539D2C[2];

    CVoiceHandle* handle = self->field_0x20;
    if (handle != NULL) {
        typedef int (*IsActiveFunc)(CVoiceHandle*);
        IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
        if (isActive(handle) != 0) {
            goto fallback;
        }
    }

    if (self->field_0x24 == 4) {
        self->field_0x28 = 0;
        CVoiceHandle* h = self->field_0x20;
        CCharVoice* voicePtr = (CCharVoice*)h;
        if (h != NULL) {
            voicePtr = &h->voice;
        }
        if (func_802A3C44(self, voicePtr, 0x5DE) != 0) {
            return;
        }
    }

    if (self->field_0x24 == 3) {
        int ownerState = func_802A77E8(self->field_0x20);
        int isThird;
        if (ownerState == 4) {
            isThird = (cf::CfGameManager::func_800822F4() < 4) ? 1 : 0;
        } else {
            isThird = 0;
        }

        if (isThird != 0) {
            self->field_0x28 = 0;
            CVoiceHandle* h = self->field_0x20;
            CCharVoice* voicePtr = (CCharVoice*)h;
            if (h != NULL) {
                voicePtr = &h->voice;
            }
            if (func_802A3C44(self, voicePtr, 0x5DE) != 0) {
                return;
            }
        } else {
            self->field_0x28 = 1;
            CVoiceHandle* h = self->field_0x20;
            CCharVoice* voicePtr = (CCharVoice*)h;
            if (h != NULL) {
                voicePtr = &h->voice;
            }
            if (func_802A3C44(self, voicePtr, 0x5DD) != 0) {
                return;
            }
        }
    }

fallback:
    self->func_802A3B50();
}

// ── Target 5: us-802ab784 (func_802A9030) ──────────────────────────────────
// Voice-select and play function. Reloads slot-state, finds a free handle,
// selects voice ID based on owner state and flag.
void func_802A9030(CVS_THREAD_TENSION_UP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    self->unk4 = lbl_eu_80539D38[1];
    self->unk0 = (u32*)lbl_eu_80539D38[0];
    self->unk8 = lbl_eu_80539D38[2];

    CVoiceHandle* handle = func_802A7998(self->field_0x20);
    if (handle == NULL) {
        goto fallback;
    }

    int voiceId;
    int ownerState = func_802A77E8(self->field_0x20);
    switch (ownerState) {
    case 1:
        voiceId = (self->field_0x28 != 0) ? 0x5DF : 0x5E6;
        break;
    case 2:
        voiceId = (self->field_0x28 != 0) ? 0x5E0 : 0x5E7;
        break;
    case 3:
        voiceId = (self->field_0x28 != 0) ? 0x5E1 : 0x5E8;
        break;
    case 4:
        voiceId = (self->field_0x28 != 0) ? 0x5E2 : 0x5E9;
        break;
    case 5:
        voiceId = (self->field_0x28 != 0) ? 0x5E3 : 0x5EA;
        break;
    case 6:
        voiceId = (self->field_0x28 != 0) ? 0x5E4 : 0x5EB;
        break;
    case 7:
        voiceId = (self->field_0x28 != 0) ? 0x5E5 : 0x5EC;
        break;
    default:
        voiceId = -1;
        break;
    }

    if (voiceId == 0x5E4 && func_802A7B90(handle, self->field_0x20) != 0) {
        voiceId = 0x89F;
    }
    if (voiceId == 0x5EB && func_802A7B90(handle, self->field_0x20) != 0) {
        voiceId = 0x451;
    }

    if (voiceId > 0) {
        CVoiceHandle* h = handle;
        CCharVoice* voicePtr = (CCharVoice*)h;
        if (h != NULL) {
            voicePtr = &h->voice;
        }
        if (func_802A3C44(self, voicePtr, voiceId) != 0) {
            return;
        }
    }

fallback:
    self->func_802A3B50();
}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; TENSION_UP subclass returns 0x82 (130).
int CVS_THREAD_TENSION_UP::blank1() {
    return BUFFER_SIZE;
}