// Decompiled CriWare sfd_mps TU — high-level C reconstruction
// Matches retail assembly at 0x803C3798..0x803C570C

/* "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_mps.c" line 3 "harness_catalog.h" */
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

void SFD_SetElementOutSj(void* self, s32 idx, s32 data, s32 arg3, s32 arg4);
void SFMPS_Init(void);
int SFMPS_Finish(void);
void SFMPS_ExecServer(void* self);
int sfmps_DecodeSomeUnit(void* self);
int criware_803C1490(void* self, s32* out_a, s32* out_b, s32* out_c);
int sfmps_DecodeOneUnit(void* self, s32 buf, s32 size, s32* out_size, s32 a5, s32* out_flag);
void sfmps_pesfn(void* self, u8 stream_kind, s32 arg3, s32 arg4);
void sfmps_SkipNext(void* self, s32 buf, s32 size, s32* out_size);
int sfmps_CopyPketData(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag);
int sfmps_CopyAudio(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi);
int sfmps_CopyVideo(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi);
int sfmps_CopyPrvate(void* self, s32 buf, s32 size, s32 a4);
int sfmps_CopyUsrSj(void* self, s32 buf, s32 size, s32 out_kind);
int sfmps_CopyPadding(void);
int sfmps_CopyDstBuft(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi);
int sfmps_ChkSupply(void* self, s32 buf, s32 size, s32 a5);
void sfmps_GetStmNum(void* self, s32* out_a, s32* out_b);
void sfmps_SetMvInf(void* self);
void sfmps_SetMpsHd(void* self);
void sfmps_SetMpsRaw(void* self, void* mps_work, s32 buf, s32 size);
void* SFMPS_Create(void* self);
void sfmps_InitInf(void* inf);
s32 sfmps_ErrFn(void* h, u32 err_code);
int SFMPS_Destroy(void* self);
int SFMPS_Seek(void* self);

// External symbols
extern void* lbl_eu_80606E34;
extern u32 lbl_eu_805687F4[];
extern u32 lbl_eu_80607AF0;
extern u8 lbl_eu_80607160[];
extern u32 lbl_eu_8051C930[];
extern s32 (*lbl_eu_80619BAC)(void*, void*);

// External function declarations
s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(s32 val, u32 err_code);
void SFSET_SetCond(void* self, u32 idx, u32 val);
u32 SFSET_GetCond(void* self, u32 idx);
void SFBUF_SetPrepFlg(void* self, u32 idx, u32 val);
int SFBUF_GetPrepFlg(void* self, int idx);
void SFBUF_SetTermFlg(void* buf, s32 idx, u32 flg);
int SFBUF_GetTermFlg(void* self, int idx);
u32 SFBUF_GetRTot(void* self, u32 idx);
s32 SFBUF_GetWTot(void* self, u32 idx);
s32 SFBUF_RingGetRead(void* self, u32 idx, void* out);
s32 SFBUF_RingGetWrite(void* self, u32 idx, void* out);
s32 SFBUF_RingAddRead(void* self, u32 idx, u32 size);
s32 SFBUF_RingAddWrite(void* self, u32 idx, u32 size, void* extra);
void SFBUF_GetFlowCnt(void* self, int* a, int* b);
int SFBUF_UpdateFlowCnt(int count, int new_val, int old_val);
void SFBUF_GetUoch(void* self, int idx, int sub_idx, u32* dst);
u32 SFTRN_GetPrepFlg(void* self, u32 idx);
void SFTRN_SetTermFlg(void* self, u32 idx, u32 val);
s32 SFCON_IsEndcodeSkip(void* h);
s32 SFCON_IsSystemEndcodeSkip(void* h);
s32 SFHDS_SetHdr(void* self, void* buf, void* size, void* out);
void SFHDS_ReprocessHdr(void* self);
s32 SFPTS_IsPtsQueFull(void* self, int idx);
s32 SFPTS_WritePtsQue(void* self, int idx, void* data, void* out);
void MEM_Copy(void* dst, const void* src, u32 size);

void* MPS_Create(void);
int MPS_Destroy(void* handle);
int MPS_Init(int max_streams, void* work_buf);
void MPS_Finish(void);
void MPS_SetSystemFn(void* mps, void* fn, void* arg);
void MPS_SetPsMapFn(void* mps, void* fn, void* arg);
void MPS_SetPesFn(void* mps, void* pes_fn, void* pes_obj);
int MPS_DecHd(void* mps, void* buf, int size, int* out_size, int* out_flag);
int MPS_CheckDelim(const u8* buf);
int MPS_GetPketHd(void* mps, void* out);
int MPS_GetSysHd(void* mps, void* out, int idx);
int MPS_GetPackHd(void* mps, void* out);
int MPS_GetLastSysHd(void* mps, void* out);
int MPS_GetErrInf(void* handle, void* cb, void* arg);

void SFD_SetElementOutSj(void* self, s32 idx, s32 data, s32 arg3, s32 arg4) {
    void* mps;
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000171);
        return;
    }
    if ((u32)(idx - 0xbc) > 0x43) {
        return;
    }
    mps = *(void**)((u8*)self + 0x2024);
    *(s32*)((u8*)mps + 0x150) = arg3;
    *(s32*)((u8*)mps + 0x154) = arg4;
    *(s32*)((u8*)mps + idx * 4 - 0x2b0) = data;
}

void SFMPS_Init(void) {
    if (MPS_Init(8, lbl_eu_80607160)) {
        SFLIB_SetErr(0, 0xff000d01);
    } else {
        lbl_eu_80607AF0 = 0;
    }
}

int SFMPS_Finish(void) {
    MPS_Finish();
    return 0;
}

void SFMPS_ExecServer(void* self) {
    void (*fn)(void*, void*);
    int endflg;
    int ret;
    s32* mps_sub;
    s32 prepflg;
    s32 cond_val;

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805687F4[3] = (u32)self;
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_805687F4[1]);
    }

    if (*(s32*)((u8*)self + 0x2034) != 8) {
        endflg = SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2034)) & 1;
    } else {
        endflg = 1;
    }
    if (*(s32*)((u8*)self + 0x2030) != 8) {
        endflg = endflg & SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2030));
    }
    if (*(s32*)((u8*)self + 0x2038) != 8) {
        endflg = endflg & SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x2038));
    }

    if (endflg == 1) {
        ret = 0;
    } else {
        void* mps_work;
        mps_sub = *(s32**)((u8*)self + 0x2024);
        mps_work = (void*)mps_sub[0];
        MPS_SetSystemFn(mps_work,
                        *(void**)((u8*)self + 0xd44),
                        *(void**)((u8*)self + 0xd48));
        ret = sfmps_DecodeSomeUnit(self);

        if (*(s32*)((u8*)self + 0x54) == 2) {
            s32 num_a, num_b;
            sfmps_GetStmNum(self, &num_a, &num_b);

            if (*(s32*)((u8*)self + 0x2034) != 8) {
                prepflg = SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2034));
            } else {
                prepflg = 0;
            }
            if (*(s32*)((u8*)self + 0x2030) != 8) {
                prepflg |= SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2030));
            }
            if (*(s32*)((u8*)self + 0x2038) != 8) {
                prepflg |= SFBUF_GetPrepFlg(self, *(s32*)((u8*)self + 0x2038));
            }

            if (prepflg != 1) {
                s32 idx = *(s32*)((u8*)self + 0x202c);
                if (SFBUF_GetPrepFlg(self, idx) == 1) {
                    s32* sub = (s32*)((u8*)self + idx * 0x74);
                    s32 f8 = *(s32*)((u8*)self + 0x8);
                    s32 max_buf = *(s32*)((u8*)self + 0xa74);
                    s32 f13d4 = sub[0x13d4 / 4];
                    s32 limit;
                    s32 wt;

                    if (f8 <= 0) f8 = f13d4;
                    if (f8 <= 0) f8 = max_buf;
                    if (f8 < max_buf) max_buf = f8;

                    wt = SFBUF_GetWTot(self, 0);
                    if (wt >= max_buf) {
                        if (*(s32*)((u8*)self + 0x2034) != 8)
                            SFBUF_SetPrepFlg(self, *(s32*)((u8*)self + 0x2034), 1);
                        if (*(s32*)((u8*)self + 0x2030) != 8)
                            SFBUF_SetPrepFlg(self, *(s32*)((u8*)self + 0x2030), 1);
                        if (*(s32*)((u8*)self + 0x2038) != 8)
                            SFBUF_SetPrepFlg(self, *(s32*)((u8*)self + 0x2038), 1);
                    }
                }
            }

            sfmps_SetMvInf(self);

            mps_sub = *(s32**)((u8*)self + 0x2024);
            cond_val = SFSET_GetCond(self, 6);
            if (cond_val != 0) {
                if (SFSET_GetCond(self, 0x50) != 0) {
                    if (SFBUF_GetWTot(self, 2) == 0) {
                        if (SFTRN_GetPrepFlg(self, 6) != 0) {
                            SFSET_SetCond(self, 6, 0);
                        }
                    }
                }
            }

            if (SFSET_GetCond(self, 5) != 0) {
                if (SFSET_GetCond(self, 0x4f) != 0) {
                    if (SFBUF_GetWTot(self, 1) == 0) {
                        if (mps_sub[1] == 0) {
                            if (SFTRN_GetPrepFlg(self, 7) != 0) {
                                SFSET_SetCond(self, 5, 0);
                            }
                        }
                    }
                }
            }

            sfmps_SetMpsHd(self);
        }
    }

    if (lbl_eu_80606E34 != NULL) {
        lbl_eu_805687F4[0x74 / 4] = (u32)((u8*)self + 0x998);
        lbl_eu_805687F4[0x80 / 4] = (u32)((u8*)self + 0x9a0);
        lbl_eu_805687F4[0x8c / 4] = (u32)((u8*)self + 0x9a8);
        fn = *(void (**)(void*, void*))((u8*)*(u32*)lbl_eu_80606E34 + 0x24);
        fn(lbl_eu_80606E34, &lbl_eu_805687F4[0x6c / 4]);
    }
}

int sfmps_DecodeSomeUnit(void* self) {
    int ret = 0;
    s32 limit;
    s32 total = 0;
    s32 cond_val;
    s32 flow_a, flow_b;

    limit = *(s32*)((u8*)self + 0x2c);
    cond_val = SFSET_GetCond(self, 0x4b);

    for (;;) {
        s32 read_size = 0;
        s32 data_size = 0;
        s32 dummy = 0;
        s32 err;
        s32 hi, lo, carry;

        if (*(s32*)((u8*)self + 0x70) != 0)
            break;

        err = criware_803C1490(self, &read_size, &data_size, &dummy);
        if (err != 0) { ret = err; break; }

        err = sfmps_DecodeOneUnit(self, read_size, data_size, &data_size, dummy, &dummy);
        if (err != 0) { ret = err; break; }

        hi = *(s32*)((u8*)self + 0x9a4);
        lo = *(s32*)((u8*)self + 0x9a0);
        *(s32*)((u8*)self + 0x9a4) = hi + data_size;
        carry = ((u32)(hi + data_size) < (u32)hi) ? 1 : 0;
        *(s32*)((u8*)self + 0x9a0) = lo + carry;

        hi = *(s32*)((u8*)self + 0x9ac);
        lo = *(s32*)((u8*)self + 0x9a8);
        *(s32*)((u8*)self + 0x9ac) = hi + dummy;
        carry = ((u32)(hi + dummy) < (u32)hi) ? 1 : 0;
        *(s32*)((u8*)self + 0x9a8) = lo + carry;

        if (data_size == 0)
            break;

        err = SFBUF_RingAddRead(self, *(s32*)((u8*)self + 0x202c), data_size);
        if (err == 0) {
            *(s32*)((u8*)self + 0x39a8) += data_size;
        } else {
            ret = err;
            break;
        }

        total += data_size + dummy;
        if (cond_val != -1 && total >= cond_val)
            break;
    }

    SFBUF_GetFlowCnt(*(void**)((u8*)self + 0x13cc), (int*)&flow_a, (int*)&flow_b);
    {
        s32 lo = *(s32*)((u8*)self + 0x998);
        s32 hi = *(s32*)((u8*)self + 0x99c);
        *(s32*)((u8*)self + 0x998) = SFBUF_UpdateFlowCnt(lo, hi, flow_a);
        *(s32*)((u8*)self + 0x99c) = SFBUF_UpdateFlowCnt(lo, hi, flow_a);
    }

    return ret;
}

int criware_803C1490(void* self, s32* out_a, s32* out_b, s32* out_c) {
    s32 tmp[5];
    void (*cb)(s32, s32);

    *out_a = 0;
    *out_b = 0;
    *out_c = 0;

    if (SFBUF_RingGetRead(self, *(s32*)((u8*)self + 0x202c), tmp))
        return 0;

    *out_a = tmp[0];
    *out_b = tmp[1];
    *out_c = tmp[1] + tmp[3];

    if (tmp[1] >= 0x800) {
        u32 addr = *out_a;
        if (addr != *(u32*)((u8*)self + 0x39ac)) {
            u32 accum = *(u32*)((u8*)self + 0x39a8);
            if ((accum & 0x7ff) == 0) {
                cb = (void (*)(s32, s32))(*(u32*)((u8*)self + 0x39a0));
                s32 arg = *(s32*)((u8*)self + 0x39a4);
                if (cb != NULL)
                    cb(arg, addr);
                *(u32*)((u8*)self + 0x39ac) = *out_a;
            }
        }
    }

    return 0;
}

void criware_803C1570(void* self, u32 a, u32 b) {
    if (self == NULL) return;
    *(u32*)((u8*)self + 0x39a0) = a;
    *(u32*)((u8*)self + 0x39a4) = b;
}

int sfmps_DecodeOneUnit(void* self, s32 buf, s32 size, s32* out_size, s32 a5, s32* out_flag) {
    int ret = 0;
    int has_delim;
    int cond_val;
    void* mps_sub;
    void* mps_work;
    int flags = 0;
    int flags2 = 0;

    *out_size = 0;
    *out_flag = 0;

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    if (!sfmps_ChkSupply(self, buf, size, a5))
        return 0;

    has_delim = (size >= 4) ? MPS_CheckDelim((const u8*)buf) : 0;

    MPS_SetPsMapFn(mps_work, *(void**)((u8*)self + 0xd4c), *(void**)((u8*)self + 0xd50));

    if (*(s32*)((u8*)self + 0xd5c) != 0) {
        MPS_SetPesFn(mps_work, sfmps_pesfn, self);
    } else {
        MPS_SetPesFn(mps_work, NULL, NULL);
    }

    if (MPS_DecHd(mps_work, (void*)buf, size, &flags, &flags2)) {
        ret = SFLIB_SetErr((s32)self, 0xff000d03);
    }

    if (flags2 & 0x2000) {
        sfmps_SetMpsRaw(self, mps_work, buf, size);
    }

    cond_val = SFSET_GetCond(self, 0x56);
    if (*(s32*)((u8*)self + 0x39a0) != 0)
        cond_val = 0x800;

    if ((flags2 & 0x08000000) && SFCON_IsEndcodeSkip(self)) {
        void* sub = *(void**)((u8*)self + 0x2024);
        *(s32*)((u8*)sub + 0x20) += 1;
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
        return ret;
    }

    if ((flags2 & 0x08000000) && SFCON_IsSystemEndcodeSkip(self)) {
        *out_size = cond_val;
        *(s32*)((u8*)mps_sub + 0x158) = cond_val;
        return ret;
    }

    if (!has_delim) {
        s32 skip_cnt;
        sfmps_SkipNext(self, buf, size, out_flag);
        skip_cnt = *out_flag;
        *out_size = skip_cnt;
        if (skip_cnt > 0) {
            s32 cur = *(s32*)((u8*)mps_sub + 0x158);
            s32 hdr_size = *(s32*)((u8*)self + 0x2c);
            if (cur >= 0) {
                if (cur >= hdr_size) {
                    *(s32*)((u8*)mps_sub + 0x158) = cur + skip_cnt;
                } else if (cur + skip_cnt > hdr_size) {
                    s32 overlap = hdr_size - cur;
                    *out_flag = skip_cnt - overlap;
                    *(s32*)((u8*)mps_sub + 0x158) = hdr_size + (skip_cnt - overlap);
                } else {
                    *(s32*)((u8*)mps_sub + 0x158) = cur + skip_cnt;
                    *out_flag = 0;
                }
            }
        }
        return ret;
    }

    if (flags2 & 0x1000) {
        s32 pket_out;
        sfmps_CopyPketData(self, buf + flags, size - flags, out_size, &pket_out);
        ret = pket_out;
        if (pket_out == 1) {
            *out_size = flags + *out_size;
        }
        *(s32*)((u8*)mps_sub + 0x158) = -1;
        return ret;
    }

    if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x202c)) == 1) {
        SFTRN_SetTermFlg(self, 1, 1);
        if (*(s32*)((u8*)self + 0x2034) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
        if (*(s32*)((u8*)self + 0x2030) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
        if (*(s32*)((u8*)self + 0x2038) != 8)
            SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
        return ret;
    }

    if (*(s32*)((u8*)self + 0x2c) < size) {
        s32 f10 = flags;
        if (f10 > 0) {
            *out_size = f10;
            *out_flag = f10;
        } else {
            *out_size = 1;
            *out_flag = 1;
        }
    }

    return ret;
}

void sfmps_pesfn(void* self, u8 stream_kind, s32 arg3, s32 arg4) {
    void (*cb)(s32, u8*, s32*, s32*, u32, s32*);
    u8 kind_copy;
    s32 args[2];
    s32 stm_info[2];

    cb = (void (*)(s32, u8*, s32*, s32*, u32, s32*))(*(u32*)((u8*)self + 0xd5c));
    if (cb == NULL) return;

    kind_copy = stream_kind;
    args[0] = arg3;
    args[1] = arg4;
    stm_info[0] = *(s32*)((u8*)self + 0x9a0);
    stm_info[1] = *(s32*)((u8*)self + 0x9a4);
    cb(*(s32*)((u8*)self + 0xd60), &kind_copy, stm_info, args, 0, NULL);
}

void sfmps_SkipNext(void* self, s32 buf, s32 size, s32* out_size) {
    s32 skip_cnt = 0;
    s32 hdr_size = *(s32*)((u8*)self + 0x2c);

    *out_size = 0;

    if (size >= hdr_size + 3) {
        int all_zero = 1;
        s32 i;
        for (i = 0; i < hdr_size; i++) {
            if (*(s8*)(buf + i) != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) {
            *out_size = hdr_size;
            return;
        }
    }

    while (size >= 4) {
        int delim = MPS_CheckDelim((const u8*)buf);
        if (delim & 0x000d0000) {
            *out_size = skip_cnt;
            return;
        }
        skip_cnt++;
        buf++;
        size--;
    }

    if (size > 0 && size < 4) {
        s32 idx = *(s32*)((u8*)self + 0x202c);
        s32* sub = (s32*)((u8*)self + idx * 0x74);
        int is_wrap = 0;

        if (sub[0x13c8 / 4] == 0) {
            if (sub[0x13d8 / 4] == 0 && sub[0x13dc / 4] == 0) {
                is_wrap = 1;
            } else {
                is_wrap = 0;
            }
        } else {
            s32 start = sub[0x13d0 / 4];
            s32 end = sub[0x13d4 / 4];
            is_wrap = ((start + end) == (buf + size)) ? 1 : 0;
        }

        if (is_wrap) {
            skip_cnt += size;
            size = 0;
        }
    }

    if (size > 0 && size < 4) {
        if (SFSET_GetCond(self, 0x55) != 0) {
            skip_cnt += size;
        }
    }

    *out_size = skip_cnt;
}

int sfmps_CopyPketData(void* self, s32 buf, s32 size, s32* out_size, s32* out_flag) {
    int ret = 0;
    void* mps_sub;
    s32 pket_buf[10];
    s32 pket_type;
    s32 pket_size;
    void* sj;
    s32 tmp_size;

    *out_size = 0;
    *out_flag = 0;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (MPS_GetPketHd(*(void**)mps_sub, pket_buf)) {
        ret = SFLIB_SetErr((s32)self, 0xff000d06);
    }

    pket_type = pket_buf[2];
    pket_size = pket_buf[3];

    if ((u32)(pket_type - 0xbc) > 0x43) {
        *out_flag = 1;
        SFLIB_SetErr((s32)self, 0xff000d0f);
        return ret;
    }
    if (pket_size > 3) {
        *out_flag = 1;
        return 0;
    }
    if (pket_size < 0) {
        SFLIB_SetErr((s32)self, 0xff000d0e);
        return ret;
    }
    if (pket_size == 0) {
        *out_size = 0;
        *out_flag = 1;
        return 0;
    }

    if (size < pket_size) {
        if (SFBUF_GetTermFlg(self, *(s32*)((u8*)self + 0x202c)) == 1) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
        }
        return 0;
    }

    sj = *(void**)((u8*)mps_sub + pket_type * 4 + 0x40);
    if (sj != NULL) {
        void** vtbl = *(void***)sj;
        s32 avail;
        void (*notify_fn)(void*, s32) = *(void(**)(void*, s32))((u8*)mps_sub + 0x150);
        void* notify_arg = *(void**)((u8*)mps_sub + 0x154);
        s32 total_copied = 0;

        avail = ((s32(*)(void*, s32))vtbl[9])(sj, 0);
        if (avail < pket_size) {
            total_copied = 0;
        } else {
            void* tmp_ptr;
            s32 tmp_sz;
            ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, pket_size, &tmp_ptr);
            MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
            ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);

            if (tmp_sz == 0) {
                total_copied = 0;
            } else {
                s32 remaining = pket_size - tmp_sz;
                buf += tmp_sz;

                if (remaining > 0) {
                    ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, remaining, &tmp_ptr);
                    MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
                    ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);
                    if (tmp_sz != remaining) {
                        lbl_eu_80607AF0++;
                    }
                }

                if (notify_fn != NULL) {
                    notify_fn(notify_arg, pket_type);
                }
                total_copied = 1;
            }
        }
        *out_flag = total_copied;
    } else {
        typedef int (*copy_fn_t)(void*, s32, s32, s32*);
        copy_fn_t fn = ((copy_fn_t*)lbl_eu_8051C930)[pket_size];
        *out_flag = fn(self, buf, pket_size, &tmp_size);
    }

    if (*out_flag == 1) {
        *out_size = pket_size;
    } else if (*out_flag != 0) {
        ret = *out_flag;
    }

    return ret;
}

int sfmps_CopyAudio(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi) {
    void* mps_sub;
    s32 split_val;
    int skip;

    if (SFSET_GetCond(self, 6) == 0)
        return 1;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (*(s32*)((u8*)mps_sub + 0x38) == -1)
        *(s32*)((u8*)mps_sub + 0x38) = stream_kind;
    if (*(s32*)((u8*)mps_sub + 0x30) == -1)
        *(s32*)((u8*)mps_sub + 0x30) = stream_kind;

    split_val = SFSET_GetCond(self, 0x1e);
    if (split_val != -1) {
        skip = 0;
        if (SFSET_GetCond(self, 0x37) != 0) {
            s32 prev = *(s32*)((u8*)mps_sub + 0x28);
            s32 xored = stream_kind ^ prev;
            skip = (((xored >> 1) & ~(xored & prev)) >> 31) & 1;
        } else {
            skip = (*(s32*)((u8*)mps_sub + 0x30) == stream_kind) ? 1 : 0;
        }
        if (skip)
            *(s32*)((u8*)mps_sub + 0x38) = split_val;
    }

    *(s32*)((u8*)mps_sub + 0x28) = stream_kind;

    if (*(s32*)((u8*)mps_sub + 0x38) != stream_kind)
        return 1;

    {
        s32 hi, lo;
        s32 min_lo, min_hi, max_lo, max_hi;
        s32 tmp_lo, tmp_hi;

        if (pts_hi == 0 && pts_lo == 0) {
            /* skip PTS update */
        } else {
            min_lo = *(s32*)((u8*)mps_sub + 0x10);
            min_hi = *(s32*)((u8*)mps_sub + 0x14);
            if (pts_hi < min_hi || (pts_hi == min_hi && (u32)pts_lo < (u32)min_lo)) {
                tmp_hi = pts_hi;
                tmp_lo = pts_lo;
            } else {
                tmp_hi = min_hi;
                tmp_lo = min_lo;
            }
            *(s32*)((u8*)mps_sub + 0x14) = tmp_hi;
            *(s32*)((u8*)mps_sub + 0x10) = tmp_lo;

            max_lo = *(s32*)((u8*)mps_sub + 0x18);
            max_hi = *(s32*)((u8*)mps_sub + 0x1c);
            if (pts_hi > max_hi || (pts_hi == max_hi && (u32)pts_lo > (u32)max_lo)) {
                tmp_hi = pts_hi;
                tmp_lo = pts_lo;
            } else {
                tmp_hi = max_hi;
                tmp_lo = max_lo;
            }
            *(s32*)((u8*)mps_sub + 0x1c) = tmp_hi;
            *(s32*)((u8*)mps_sub + 0x18) = tmp_lo;
        }
    }

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2034), buf, size, pts_lo, pts_hi);
}

int sfmps_CopyVideo(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi) {
    void* mps_sub;
    s32 split_val;

    if (SFSET_GetCond(self, 5) == 0)
        return 1;

    mps_sub = *(void**)((u8*)self + 0x2024);

    if (*(s32*)((u8*)mps_sub + 0x34) == -1) {
        s32 mode = SFSET_GetCond(self, 0x3b);
        s32 new_val;

        if (mode == 1) {
            new_val = stream_kind;
        } else if (mode == 2) {
            s32 na, nb;
            sfmps_GetStmNum(self, &na, &nb);
            new_val = (nb >= 2) ? 2 : stream_kind;
        } else {
            new_val = stream_kind;
        }
        *(s32*)((u8*)mps_sub + 0x34) = new_val;

        {
            s32 sys_buf[7];
            MPS_GetSysHd(*(void**)mps_sub, sys_buf, 1);
            if (sys_buf[3] == -1) {
                SFSET_SetCond(self, 0x49, 1);
            }
        }
    }

    if (*(s32*)((u8*)mps_sub + 0x2c) == -1)
        *(s32*)((u8*)mps_sub + 0x2c) = stream_kind;

    split_val = SFSET_GetCond(self, 0x1d);
    if (split_val != -1) {
        int skip = 0;
        if (SFSET_GetCond(self, 0x37) != 0) {
            s32 prev = *(s32*)((u8*)mps_sub + 0x24);
            s32 xored = stream_kind ^ prev;
            skip = (((xored >> 1) & ~(xored & prev)) >> 31) & 1;
        } else {
            skip = (*(s32*)((u8*)mps_sub + 0x2c) == stream_kind) ? 1 : 0;
        }
        if (skip && *(s32*)((u8*)mps_sub + 0x34) != split_val) {
            int is_video = 0;
            if (size >= 4) {
                u8* p = (u8*)buf;
                if (p[0] == 0 && p[1] == 0 && p[2] == 1) {
                    is_video = (p[3] == 0xb3 || p[3] == 0xb8) ? 1 : 0;
                }
            }
            if (is_video)
                *(s32*)((u8*)mps_sub + 0x34) = split_val;
        }
    }

    *(s32*)((u8*)mps_sub + 0x24) = stream_kind;

    if (*(s32*)((u8*)mps_sub + 0x34) != stream_kind)
        return 1;

    return sfmps_CopyDstBuft(self, *(s32*)((u8*)self + 0x2030), buf, size, pts_lo, pts_hi);
}

int sfmps_CopyPrvate(void* self, s32 buf, s32 size, s32 a4) {
    s32 hdr_out;

    if (SFHDS_SetHdr(self, (void*)buf, &size, &hdr_out)) {
        if (hdr_out != 0) {
            sfmps_CopyUsrSj(self, 0, size - 0x12, a4 + 0x12);
        }
        return 1;
    }

    return sfmps_CopyUsrSj(self, buf, size, a4);
}

int sfmps_CopyUsrSj(void* self, s32 buf, s32 size, s32 out_kind) {
    void* sj;
    void** vtbl;
    s32 avail;
    u32 uoch[4];
    void (*fn1)(void*, s32);
    void (*fn2)(void*, s32);
    void* arg1;
    void* arg2;

    if (*(s32*)((u8*)self + 0x2038) == 8)
        return 1;

    SFBUF_GetUoch(self, *(s32*)((u8*)self + 0x2038), buf, uoch);

    sj = (void*)uoch[0];
    fn1 = (void(*)(void*, s32))uoch[1];
    arg1 = (void*)uoch[2];
    fn2 = (void(*)(void*, s32))uoch[3];
    arg2 = (void*)uoch[2];

    if (sj == NULL)
        return 1;

    vtbl = *(void***)sj;
    avail = ((s32(*)(void*, s32))vtbl[9])(sj, 0);

    if (avail < size) {
        out_kind = 0;
    } else {
        void* tmp_ptr;
        s32 tmp_sz;
        s32 total = 1;

        ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, size, &tmp_ptr);
        MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
        ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);

        if (tmp_sz == 0) {
            out_kind = 0;
        } else {
            s32 remaining = size - tmp_sz;
            buf += tmp_sz;

            if (remaining > 0) {
                ((void(*)(void*, s32, s32, void*))vtbl[6])(sj, 0, remaining, &tmp_ptr);
                MEM_Copy((void*)buf, tmp_ptr, tmp_sz);
                ((void(*)(void*, s32, void*))vtbl[8])(sj, 1, &tmp_ptr);
                if (tmp_sz != remaining) {
                    lbl_eu_80607AF0++;
                }
            }

            if (total == 1) {
                if (fn1 != NULL)
                    fn1(arg1, buf);
                if (fn2 != NULL)
                    fn2(arg2, buf);
            }
            return total;
        }
    }

    if (out_kind == 1) {
        if (fn1 != NULL)
            fn1(arg1, buf);
        if (fn2 != NULL)
            fn2(arg2, buf);
    }

    return out_kind;
}

int sfmps_CopyPadding(void) {
    return 1;
}

int sfmps_CopyDstBuft(void* self, s32 stream_kind, s32 buf, s32 size, s32 pts_lo, s32 pts_hi) {
    s32 ring_buf[6];
    s32 write_pos;
    s32 total_size;
    s32 first_size;
    void* first_ptr;
    void* second_ptr;

    if (SFBUF_RingGetWrite(self, stream_kind, ring_buf))
        return 0;

    first_size = ring_buf[1];
    total_size = ring_buf[3];
    write_pos = ring_buf[2];
    first_ptr = (void*)ring_buf[0];
    second_ptr = (void*)ring_buf[4];

    if (size > first_size + total_size)
        return 0;

    if (stream_kind == 1) {
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3];
            data[0] = pts_lo;
            data[1] = pts_hi;
            data[2] = size;
            if (lbl_eu_80619BAC((u8*)self + 0x1374, data) == -1)
                return 0;
        }

        if (!(pts_hi == 0 && pts_lo == 0)) {
            if (SFPTS_IsPtsQueFull(self, stream_kind)) {
                return 0;
            } else {
                s32 pts_data[4];
                s32 out;
                pts_data[0] = pts_lo;
                pts_data[1] = pts_hi;
                pts_data[2] = write_pos;
                pts_data[3] = size;
                if (SFPTS_WritePtsQue(self, stream_kind, pts_data, &out))
                    return 0;
            }
        }
    } else if (stream_kind == 2) {
        if (lbl_eu_80619BAC != NULL) {
            s32 data[3];
            data[0] = pts_lo;
            data[1] = pts_hi;
            data[2] = size;
            if (lbl_eu_80619BAC((u8*)self + 0x1368, data) == -1)
                return 0;
        }
    }

    if (size <= write_pos) {
        MEM_Copy(first_ptr, (void*)buf, size);
    } else {
        MEM_Copy(first_ptr, (void*)buf, write_pos);
        MEM_Copy(second_ptr, (void*)(buf + write_pos), size - write_pos);
    }

    if (SFBUF_RingAddWrite(self, stream_kind, size, second_ptr))
        return 0;

    return 1;
}

int sfmps_ChkSupply(void* self, s32 buf, s32 size, s32 a5) {
    void* mps_sub;
    s32 stream_idx;
    int delim = 0;
    int endflg;

    mps_sub = *(void**)((u8*)self + 0x2024);
    stream_idx = *(s32*)((u8*)self + 0x202c);

    if (size >= 4) {
        delim = MPS_CheckDelim((const u8*)buf);
        if ((delim & 0x08000000)) {
            if (*(s32*)((u8*)self + 0x203c) < 0) {
                *(s32*)((u8*)self + 0x203c) = SFBUF_GetRTot(self, stream_idx) + 4;
            }
            *(s32*)((u8*)mps_sub + 0x3c) = 1;
        } else if (delim != 0) {
            *(s32*)((u8*)mps_sub + 0x3c) = 0;
        }
    }

    {
        int check_end = 0;
        if ((delim & 0x08000000) != 0) {
            if (SFCON_IsEndcodeSkip(self) || SFCON_IsSystemEndcodeSkip(self)) {
                check_end = 0;
            } else {
                check_end = 1;
            }
        }

        if (check_end) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
            return 0;
        }
    }

    if (a5 < 4) {
        if (SFBUF_GetTermFlg(self, stream_idx) == 1) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
            return 0;
        }
    }

    if (size < 0x40) {
        if ((delim & 0x00010000) == 0 && (delim & 0x00040000) == 0) {
            return 1;
        }
        if (SFBUF_GetTermFlg(self, stream_idx) == 1) {
            SFTRN_SetTermFlg(self, 1, 1);
            if (*(s32*)((u8*)self + 0x2034) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2034), 1);
            if (*(s32*)((u8*)self + 0x2030) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2030), 1);
            if (*(s32*)((u8*)self + 0x2038) != 8)
                SFBUF_SetTermFlg(self, *(s32*)((u8*)self + 0x2038), 1);
            return 0;
        }
        return 1;
    }

    return 1;
}

void sfmps_GetStmNum(void* self, s32* out_a, s32* out_b) {
    void* mps_sub;
    void* mps_work;
    s32 max_a = 0, max_b = 0;
    int i;
    s32 sys_buf[6];

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    for (i = 0; i < 3; i++) {
        MPS_GetSysHd(mps_work, sys_buf, i);
        if (max_a < sys_buf[2])
            max_a = sys_buf[2];
        if (max_b < sys_buf[3])
            max_b = sys_buf[3];
    }

    *(s32*)((u8*)mps_sub + 0x8) = max_a;
    *(s32*)((u8*)mps_sub + 0x4) = max_b;
    *out_a = max_a;
    *out_b = *(s32*)((u8*)mps_sub + 0x4);
}

void sfmps_SetMvInf(void* self) {
    void* mps_sub;
    void* mps_work;
    s32 sys_buf[6];
    s32 pack_buf[6];

    mps_sub = *(void**)((u8*)self + 0x2024);
    mps_work = *(void**)mps_sub;

    MPS_GetPackHd(mps_work, pack_buf);
    if (pack_buf[3] >= 1)
        *(s32*)((u8*)self + 0x934) = pack_buf[3];

    MPS_GetSysHd(mps_work, sys_buf, 1);
    if (sys_buf[4] != -1)
        *(s32*)((u8*)self + 0x938) = sys_buf[4];

    if (*(s32*)((u8*)self + 0x940) == -1)
        *(s32*)((u8*)self + 0x940) = *(s32*)((u8*)mps_sub + 0x8);
    if (*(s32*)((u8*)self + 0x944) == -1)
        *(s32*)((u8*)self + 0x944) = *(s32*)((u8*)mps_sub + 0x4);
}

void sfmps_SetMpsHd(void* self) {
    s32* hdr;
    void* mps_sub;
    s32* sub;

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL) return;

    mps_sub = *(void**)((u8*)self + 0x2024);
    if (*(s32*)((u8*)mps_sub + 0x20) > 0) return;

    hdr = (s32*)((u8*)hdr + 0x8a0);
    if (hdr == NULL) return;

    sub = *(s32**)((u8*)self + 0x2024);
    {
        s32 hi = sub[4];
        s32 lo = sub[5];
        if (hi == 0x7fffffff && lo == -1)
            return;

        {
            s32 diff_lo = lo - hdr[9];
            s32 diff_hi = hi - hdr[8];
            *(s32*)((u8*)self + 0xef4) = diff_lo;
            *(s32*)((u8*)self + 0xef0) = diff_hi;
        }

        if (hdr[0] != 0) return;

        hdr[1] = *(s32*)((u8*)self + 0x934) * 50;
        hdr[2] = *(s32*)((u8*)self + 0x938);
        hdr[3] = sub[1];
        hdr[4] = sub[2];
        hdr[6] = *(s32*)((u8*)self + 0xeec);
        hdr[7] = *(s32*)((u8*)self + 0xee8);
        hdr[8] = sub[4];
        hdr[9] = sub[5];
        hdr[10] = sub[11];
        hdr[11] = sub[12];
    }
}

void sfmps_SetMpsRaw(void* self, void* mps_work, s32 buf, s32 size) {
    s32* hdr;
    s32* sub;
    s32* raw_hdr;
    s32 sys_buf[6];
    s32 copy_size;

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL) return;

    sub = *(s32**)((u8*)self + 0x2024);
    if (*(s32*)((u8*)sub + 0x20) > 0) return;

    raw_hdr = (s32*)((u8*)hdr + 0x8a0);
    if (raw_hdr == NULL || raw_hdr[0] != 0) return;

    raw_hdr = (s32*)((u8*)raw_hdr + 0x30);
    MPS_GetLastSysHd(mps_work, sys_buf);

    copy_size = (size >= 0xb0) ? 0xb0 : size;

    if (sys_buf[3] > 0) {
        *(s32*)((u8*)raw_hdr + 0x160) = copy_size;
    } else if (sys_buf[2] > 0) {
        *(s32*)((u8*)raw_hdr + 0x164) = copy_size;
        raw_hdr = (s32*)((u8*)raw_hdr + 0xb0);
    } else {
        return;
    }

    MEM_Copy(raw_hdr, (void*)buf, copy_size);
}

void* SFMPS_Create(void* self) {
    void* mps_sub;
    void* mps;

    mps_sub = (u8*)self + 0x2240;
    *(void**)((u8*)self + 0x2024) = mps_sub;

    sfmps_InitInf(mps_sub);

    mps = MPS_Create();
    if (mps == NULL) {
        SFLIB_SetErr(0, 0xff000d08);
        return NULL;
    }

    if (MPS_GetErrInf(mps, sfmps_ErrFn, self)) {
        MPS_Destroy(mps);
        SFLIB_SetErr(0, 0xff000d09);
        return NULL;
    }

    *(void**)mps_sub = mps;
    *(u32*)((u8*)self + 0x39a0) = 0;
    *(u32*)((u8*)self + 0x39a4) = 0;
    *(u32*)((u8*)self + 0x39a8) = 0;
    *(u32*)((u8*)self + 0x39ac) = 0;

    return NULL;
}

void sfmps_InitInf(void* inf) {
    s32* p = (s32*)inf;
    int i, j;

    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[4] = 0x7fffffff;
    p[5] = -1;
    p[6] = 0x7fffffff;
    p[7] = -1;
    p[8] = 0;
    p[9] = 0x7fffffff;
    p[10] = 0x7fffffff;
    p[11] = -1;
    p[12] = -1;
    p[13] = -1;
    p[14] = -1;
    p[15] = 0;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 32; j++) {
            p[16 + i * 32 + j] = 0;
        }
    }

    p[16 + 64] = 0;
    p[16 + 65] = 0;
    p[16 + 66] = 0;
    p[16 + 67] = 0;
    *(s32*)((u8*)p + 0x150) = 0;
    *(s32*)((u8*)p + 0x154) = 0;
    *(s32*)((u8*)p + 0x158) = -1;
}

s32 sfmps_ErrFn(void* h, u32 err_code) {
    return SFLIB_SetErr((s32)h, err_code);
}

int SFMPS_Destroy(void* self) {
    void* mps_sub = *(void**)((u8*)self + 0x2024);
    void* mps = *(void**)mps_sub;

    if (MPS_Destroy(mps)) {
        return SFLIB_SetErr((s32)self, 0xff000d0a);
    }
    return 0;
}

int SFMPS_RequestStop(void) { return 0; }
int SFMPS_Start(void) { return 0; }
int SFMPS_Stop(void) { return 0; }
int SFMPS_Pause(void) { return 0; }

s32 SFMPS_GetWrite(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

s32 SFMPS_AddWrite(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

s32 SFMPS_GetRead(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

s32 SFMPS_AddRead(void* h) {
    return SFLIB_SetErr((s32)h, 0xff000d0b);
}

int SFMPS_Seek(void* self) {
    s32* hdr;
    void* mps_sub;
    void* mps_work;
    s32* raw_hdr;
    int ret1, ret2;
    s32 sys_buf[3];

    hdr = *(s32**)((u8*)self + 0x2670);
    if (hdr == NULL) return 0;

    mps_sub = *(void**)((u8*)self + 0x2024);
    if (*(s32*)((u8*)mps_sub + 0x20) > 0) return 0;

    raw_hdr = (s32*)((u8*)hdr + 0x8a0);
    if (raw_hdr == NULL || raw_hdr[0] != 0) return 0;

    SFHDS_ReprocessHdr(self);
    mps_work = *(void**)mps_sub;

    ret1 = MPS_DecHd(mps_work, (u8*)raw_hdr + 0x30, *(s32*)((u8*)raw_hdr + 0x190), &ret1, &ret1);
    ret2 = MPS_DecHd(mps_work, (u8*)raw_hdr + 0x30 + 0xb0, *(s32*)((u8*)raw_hdr + 0x164), &ret2, &ret2);

    if (ret1 != 0 || ret2 != 0) {
        return SFLIB_SetErr((s32)self, 0xff000d0d);
    }

    *(s32*)((u8*)mps_sub + 0x2c) = *(s32*)((u8*)raw_hdr + 0x28);
    *(s32*)((u8*)mps_sub + 0x30) = *(s32*)((u8*)raw_hdr + 0x2c);
    *(s32*)((u8*)self + 0xeec) = *(s32*)((u8*)raw_hdr + 0x1c);
    *(s32*)((u8*)self + 0xee8) = *(s32*)((u8*)raw_hdr + 0x18);
    *(s32*)((u8*)mps_sub + 0x14) = *(s32*)((u8*)raw_hdr + 0x24);
    *(s32*)((u8*)mps_sub + 0x10) = *(s32*)((u8*)raw_hdr + 0x20);

    return 0;
}

u32 SFMPS_GetConcatCnt(void* self) {
    void* ptr = *(void**)((u8*)self + 0x2024);
    return *(u32*)((u8*)ptr + 0x20);
}
