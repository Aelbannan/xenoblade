
/* "src/kyoshin/cf/chain/CChainActorPc.cpp" line 1 "kyoshin/cf/chain/CChainActorPc.hpp" */
#pragma once

/* "src/kyoshin/cf/chain/CChainActorPc.hpp" line 2 "types.h" */
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
/* "src/kyoshin/cf/chain/CChainActorPc.hpp" line 3 "kyoshin/cf/chain/CChainActor.hpp" */
#pragma once

/* "src/kyoshin/cf/chain/CChainActor.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/chain/CChainActor.hpp" line 3 "kyoshin/cf/chain/CChainTemp.hpp" */
#pragma once

/* "src/kyoshin/cf/chain/CChainTemp.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/chain/CChainTemp.hpp" line 3 "cstring" */
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
    //size: 0x68
    class CChainTemp {
    public:
        u8 unk0[0x60];
        bool unk60; //0x60
        u8 _pad61[3];
        u32 mVTable; //0x64: vtable pointer (manually managed, non-standard ABI)

        CChainTemp(){
            std::memset(unk0, 0, sizeof(unk0));
            unk60 = false;
        }
    };
}
/* end "kyoshin/cf/chain/CChainTemp.hpp" */
/* "src/kyoshin/cf/chain/CChainActor.hpp" line 4 "kyoshin/cf/chain/CChainEffect.hpp" */
#pragma once

/* "src/kyoshin/cf/chain/CChainEffect.hpp" line 2 "types.h" */
/* end "types.h" */
/* "src/kyoshin/cf/chain/CChainEffect.hpp" line 3 "kyoshin/cf/object/IObjectInfo.hpp" */
#pragma once

/* "src/kyoshin/cf/object/IObjectInfo.hpp" line 2 "types.h" */
/* end "types.h" */

namespace cf {
    class IObjectInfo {
    public:
        virtual ~IObjectInfo(){}
        virtual void IObjectInfo_UnkVirtualFunc1() = 0; //not sure if this exists
    };
}
/* end "kyoshin/cf/object/IObjectInfo.hpp" */

namespace cf {
    //size: 0xC
    class CChainEffect : public IObjectInfo {
    public:
        CChainEffect();
        virtual ~CChainEffect(){}
        virtual void IObjectInfo_UnkVirtualFunc1();

        //0x0: vtable
        //0x0-4: IObjectInfo
        u32 unk4;
        u32 unk8;
    };
}

extern "C" void func_802A0950(cf::CChainEffect*, int, int, int, int, int);
/* end "kyoshin/cf/chain/CChainEffect.hpp" */
/* "src/kyoshin/cf/chain/CChainActor.hpp" line 5 "cstring" */
/* end "cstring" */

namespace cf {
    //size: 0x80
    // Vtable at 0x70 is manually managed; no implicit C++ vtable at 0x00.
    class CChainActor {
    public:
        u32 unk0;
        CChainTemp mChainTemp; //0x4
        u16 unk6C;              //0x6C
        u8 _pad6E[2];           //0x6E
        u32 mVTable;            //0x70: vtable pointer (manually managed, non-standard ABI)

        CChainActor() : unk6C(0) {
            unk0 = 0;
        }
        ~CChainActor();

        CChainEffect mChainEffect; //0x74
    };
}
/* end "kyoshin/cf/chain/CChainActor.hpp" */

namespace cf {
    class CChainActorPc : public CChainActor {

    };
}
/* end "kyoshin/cf/chain/CChainActorPc.hpp" */

namespace cf { class CBattleManager; class CfObjectMove; class CfGameManager; class CChain; class CfObjectSelectorObj; }

// Retail symbol: cf::CBattleManager::getInstance()
cf::CBattleManager* getInstance__Q22cf14CBattleManagerFv();

// Forward: free function operating on the CChain/CChainActor memory layout
extern "C" void CChain_setFieldAndClear(void*, int);

// Forward decl: cf::CfGameManager::getPlayer(int)
namespace cf { class CfGameManager { public: static cf::CfObjectMove* getPlayer(int); }; }

void func_80282020(){}
int func_802A0804(int, int);
extern "C" void func_802A08F4(void*);
extern "C" void func_802A0904(void*);
extern "C" void func_80279DC0(void*);
extern "C" int func_80148778(void*, int);
extern "C" int func_8027A024(void*, void*);
extern "C" int* func_8009ECB0();

// Forward declarations for Target 1 (func_802822F8)
cf::CfObjectSelectorObj* func_800FE68C();
void func_800FE6A4(cf::CfObjectSelectorObj*, int, int, int);

// Holder struct for func_80043D90 / func_80043F18 / __dt__80043E88
struct ChainEnumHolder {
    UNKTYPE* list;  // 0x0
    u32 handle;     // 0x4
};
// Tail of the enumeration list struct, exposing the flag at offset 0x620
struct EnumListTail {
    u8 _pad[0x620];
    int field_0x620;
};

// Forward declarations for Target 2 (func_80282380)
void func_80043D90(ChainEnumHolder*);
UNKTYPE* func_80043F18(ChainEnumHolder*);
void __dt__80043E88(ChainEnumHolder*, int);
void func_800F4A98(UNKTYPE*, int, int);
void func_800F6ED0(UNKTYPE*, UNKTYPE*);
UNKTYPE* func_800F6E08(UNKTYPE*);
UNKTYPE* func_800B708C(BOOL);
UNKTYPE* func_800AD860(UNKTYPE*);

// Forward declaration for Target 3 (func_802821E0)
int func_80174C98(UNKTYPE*, UNKTYPE*, int);

int func_80282048(int arg) {
    return func_802A0804(0xb3, arg);
}
struct ChainActorInnerData {
    u8 _pad00[0x3f28];
    unsigned short field_3f28;
};

int func_80282174(void* self) {
    cf::CChainActor* actor = static_cast<cf::CChainActor*>(self);
    ChainActorInnerData* inner = reinterpret_cast<ChainActorInnerData*>(static_cast<uintptr_t>(actor->unk0));
    int value = inner->field_3f28;
    if (value == 8) return 3;
    if (value == 9) return 8;
    if (value == 10) return 9;
    if (value == 11) return 10;
    if (value == 12) return 11;
    if (value == 13) return 12;
    return value;
}
bool func_802A0AA0(void*);
bool func_8028245C(void* self) { return func_802A0AA0((void*)((char*)self + 0x74)); }
bool func_80282464() { return true; }
void func_8028246C(){}
int func_80282480(void*, void* p) {
    return ((*(int*)((char*)p + 0x3f00) >> 1) & 1);
}
void func_80277154();
void func_8028248C() {
    func_80277154();
}
void func_80282490(){}

// Resets chain state (setFieldAndClear) and clears the chain effect.
extern "C" void func_80281924(cf::CChainActorPc* self, int val) {
    CChain_setFieldAndClear(self, val);
    func_802A08F4(&self->mChainEffect);
}
void func_80281958(){}
// Resets the chain effect and calls func_80279DC0 on this actor.
extern "C" void func_80281CB8(cf::CChainActorPc* self) {
    func_802A0904(&self->mChainEffect);
    func_80279DC0(self);
}
void func_80281CF0(){}
// Retail symbol: func_804B1DC0
extern "C" void func_804B1DC0(void*, int);
// Retail symbol: func_80279F6C
extern "C" void func_80279F6C(void*, int);

// Sets a chain actor flag and optionally resets chain field via func_804B1DC0
// when arg is 0 and the current bit state differs.
extern "C" void func_80281F38(cf::CChainActorPc* self, int arg) {
    // Use u32 compare to force cmpl (unsigned) to match retail
    u32 bit = (self->unk6C >> 1) & 1;
    if ((u32)arg != bit && arg == 0) {
        func_804B1DC0((void*)(self->unk0 + 0x44a8), 1);
    }
    func_80279F6C(self, arg);
}
// Checks preconditions before calling func_8027A024 to execute chain logic.
// Returns 0 if the actor is inactive (vtable check) or if a battle-manager
// flag 0xf8 is set on the unk0 object; otherwise delegates to func_8027A024.
extern "C" int func_80281FA0(cf::CChainActorPc* self, void* arg) {
    // Call vtable entry 29: check some active/in-battle condition
    int (*vfunc)(void*) = ((int(**)(void*))self->mVTable)[29];
    if (vfunc(self) != 0) return 0;
    // Check battle-manager flag 0xf8 on this->unk0 + 8
    if (func_80148778((void*)(self->unk0 + 8), 0xf8) != 0) return 0;
    return func_8027A024(self, arg);
}
// Checks if this actor is the player's current actor.
// Compares getPlayer(0) against unk0 + 0x3e9c (or 0 if unk0 is null).
extern "C" int func_80282054(cf::CChainActor* self) {
    u32 addr = self->unk0;
    if (addr != 0) {
        addr += 0x3e9c;
    }
    return (void*)addr == cf::CfGameManager::getPlayer(0);
}
// Returns true if CBattleManager+0x194 >= 300.
extern "C" int func_8028209C() {
    return *(int*)((u8*)getInstance__Q22cf14CBattleManagerFv() + 0x194) >= 0x12c;
}
void func_802820D4(){}
// Compares the halfword at this->unk0 + 0x3f28 against the result of
// func_8009ECB0() (a global config struct). Returns 0, 1, or 2 if the
// value matches fields at offsets 4, 8, or 12 respectively; returns 3
// otherwise.
extern "C" int func_80282100(cf::CChainActorPc* self) {
    int* config = func_8009ECB0();
    u16 value = *(u16*)(self->unk0 + 0x3f28);
    if (value == config[1]) return 0;
    if (value == config[2]) return 1;
    if (value == config[3]) return 2;
    return 3;
}
// Checks multiple conditions to determine if the player can initiate a
// chain attack from this actor. Returns 1 if chain-ready, 0 otherwise.
int func_802821E0(cf::CChainActorPc* self) {
    // vfunc22 must return exactly 5
    if (((int(*)(cf::CChainActorPc*))((void**)self->mVTable)[22])(self) == 5) {
        // Check battle-manager flags 0xf0 and 0xf1
        if (func_80148778((UNKTYPE*)(self->unk0 + 8), 0xf0) != 0) return 1;
        if (func_80148778((UNKTYPE*)(self->unk0 + 8), 0xf1) != 0) return 1;

        // Call vfunc12 on sub-object at unk0+4, pass its field_0x0 to func_80174C98
        u32 obj = self->unk0;
        u32 subObj = *(u32*)(obj + 4);
        u32 val = *(u32*)((UNKTYPE*(*)(u32))(*(void***)subObj)[12])(subObj);
        if (func_80174C98((UNKTYPE*)obj, &val, 0xb) != 0) return 1;

        // Float comparison: vfunc87 <= vfunc86 on the object at this->unk0
        obj = self->unk0;
        float f86 = ((float(*)(u32))(*(void***)obj)[86])(obj);
        float f87 = ((float(*)(u32))(*(void***)obj)[87])(obj);
        if (f87 <= f86) return 1;
    }
    return 0;
}

// Calls a vfunc on the CfObjectMove subobject (this->unk0 + 0x3e9c), checks
// whether the actor is active via CChainActor vtable, then if both conditions
// pass, dispatches to the object selector with type 0x80000003.
void func_802822F8(cf::CChainActorPc* self, int arg) {
    u32 moveBase = self->unk0 + 0x3e9c;
    ((void(*)(u32))(*(void***)moveBase)[20])(moveBase);

    // Check whether the actor is active (vfunc16 returns non-zero)
    if (((int(*)(cf::CChainActorPc*))((void**)self->mVTable)[16])(self) != 0 && arg != 0) {
        cf::CfObjectSelectorObj* sel = func_800FE68C();
        func_800FE6A4(sel, 0x80000003, 0, arg);
    }
}

// Enumerates objects of type 0x80000002 via a stack-allocated holder, calls
// a vfunc at offset 0xac on the CfObjectMove subobject, then validates the
// result through func_800F6ED0/func_800F6E08 and a null check on the returned
// object. Returns the valid object or 0.
UNKTYPE* func_80282380(cf::CChainActorPc* self) {
    ChainEnumHolder holder;

    func_80043D90(&holder);
    UNKTYPE* list = func_80043F18(&holder);
    func_800F4A98(list, 0x80000002, 0);

    // Call vfunc at vtable offset 0xac on this->unk0 + 0x3e9c
    u32 moveBase = self->unk0 + 0x3e9c;
    void** moveVtbl = *(void***)moveBase;
    UNKTYPE* (*vfunc)(u32) = (UNKTYPE* (*)(u32))moveVtbl[0xac / 4];
    UNKTYPE* result = vfunc(moveBase);

    list = func_80043F18(&holder);
    func_800F6ED0(list, result);

    list = func_80043F18(&holder);
    if (((EnumListTail*)list)->field_0x620 == 0) {
        __dt__80043E88(&holder, -1);
        return 0;
    }

    list = func_80043F18(&holder);
    result = func_800F6E08(list);
    UNKTYPE* checked = func_800B708C((BOOL)(uintptr_t)result);
    if (func_800AD860(checked) == 0) {
        __dt__80043E88(&holder, -1);
        return 0;
    }

    __dt__80043E88(&holder, -1);
    return result;
}
