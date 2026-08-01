/* "libs/RVL_SDK/src/revolution/bte/stack/rfcomm/rfc_l2cap_if.c" line 0 "harness_catalog.h" */
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
    };

    class bad_exception : public exception {
    public:
        bad_exception(){}
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

/* The RFCOMM callback block contains the multiplexer table followed by ports. */
typedef struct BT_HDR BT_HDR;
typedef struct RfcMuxChannel RfcMuxChannel;
typedef struct RfcPort RfcPort;
typedef struct RfcControlBlock RfcControlBlock;

struct BT_HDR {
    u16 event;
    u16 len;
    u16 offset;
    u16 layer_specific;
};

struct RfcMuxChannel {
    u8 pad_0x00[0x62];
    u8 bd_addr[6];
    u16 lcid;
    u16 field_0x6a;
    u8 state;
    u8 initiator;
    u8 field_0x6e;
    u8 field_0x6f;
    u8 field_0x70;
    u8 field_0x71;
};

struct RfcPort {
    u8 dlci;
    u8 pad_0x01[0x23];
    u8 field_0x24;
    u8 pad_0x25[0x47];
    RfcMuxChannel* mcb;
};

/* L2CAP application registration block embedded in rfc_cb at 0x14.
 * Retail layout: 10 callback pointers, 0x14..0x3B (ConnectInd, ConnectCfm,
 * NULL, ConfigInd, ConfigCfm, DisconnectInd, NULL, QoSViolationInd,
 * BufDataInd, CongestionStatusInd). */
struct RfcL2capApplInfo {
    void (*pL2CA_ConnectInd_Cb)(u8* bd_addr, u16 lcid, u16 psm, u8 id);
    void (*pL2CA_ConnectCfm_Cb)(void);
    void (*pL2CA_ConnectPnd_Cb)(void);
    void (*pL2CA_ConfigInd_Cb)(void);
    void (*pL2CA_ConfigCfm_Cb)(u16 lcid, u8* config);
    void (*pL2CA_DisconnectInd_Cb)(u16 lcid, u8 response);
    void (*pL2CA_DisconnectCfm_Cb)(void);
    void (*pL2CA_QoSViolationInd_Cb)(void);
    void (*pL2CA_DataInd_Cb)(u16 lcid, BT_HDR* buffer);
    void (*pL2CA_CongestionStatus_Cb)(u16 lcid, u8 congested);
};

struct RfcControlBlock {
    u8 dlci;
    u8 pad_0x01[1];
    u8 field_0x02;
    u8 pad_0x03[1];
    u8 field_0x04;
    u8 credit_based;
    u8 pad_0x06[0x0E];
    struct RfcL2capApplInfo l2cap_Appl_Info;
    RfcMuxChannel* mcb[10];
    u8 pad_0x64[0x3b0];
    u8 trace_level;
};

extern RfcControlBlock rfc_cb;

#define RFCOMM_PSM 3
#define L2CAP_CONN_NO_RESOURCES 4
#define RFC_MX_EVENT_CONN_IND 0x0a

extern void LogMsg_1(u32 level, const char* message, u32 value);
extern void LogMsg_2(u32 level, const char* message, u32 value1, u32 value2);
extern void GKI_freebuf(BT_HDR* buffer);
extern void L2CA_DisconnectRsp(u16 lcid);
extern u16 L2CA_Register(u16 psm, struct RfcL2capApplInfo* p_cb_info);
extern void L2CA_ConnectRsp(u8* bd_addr, u8 id, u16 lcid, u16 result, u16 status);
extern RfcMuxChannel* rfc_alloc_multiplexer_channel(u8* bd_addr, u8 initiator);
extern void rfc_mx_sm_execute(RfcMuxChannel* channel, u16 event, u8* data);
extern void rfc_process_mx_message(RfcMuxChannel* channel, BT_HDR* buffer);
extern u8 rfc_parse_data(RfcMuxChannel* channel, RfcControlBlock* control, BT_HDR* buffer);
extern RfcPort* port_find_mcb_dlci_port(RfcMuxChannel* channel, u8 dlci);
extern RfcPort* port_find_dlci_port(u8 dlci);
extern void rfc_send_dm(RfcMuxChannel* channel, int poll, int command);
extern void rfc_port_sm_execute(RfcPort* port, u8 event, BT_HDR* buffer);
extern void rfc_process_l2cap_congestion(RfcMuxChannel* channel, u8 congested);
extern void rfc_inc_credit(RfcPort* port);

void RFCOMM_ConnectInd(u8* bd_addr, u16 lcid, u16 psm, u8 id);
void RFCOMM_ConnectCnf();
void RFCOMM_ConfigInd();
void RFCOMM_ConfigCnf(u16 lcid, u8* config);
void RFCOMM_DisconnectInd(u16 lcid, u8 response);
void RFCOMM_QoSViolationInd(void);
void RFCOMM_BufDataInd(u16 lcid, BT_HDR* buffer);
void RFCOMM_CongestionStatusInd(u16 lcid, u8 congested);

void rfcomm_l2cap_if_init() {
    struct RfcL2capApplInfo* p_info = &rfc_cb.l2cap_Appl_Info;

    p_info->pL2CA_ConnectInd_Cb = RFCOMM_ConnectInd;
    p_info->pL2CA_ConnectCfm_Cb = RFCOMM_ConnectCnf;
    p_info->pL2CA_ConnectPnd_Cb = 0;
    p_info->pL2CA_ConfigInd_Cb = RFCOMM_ConfigInd;
    p_info->pL2CA_ConfigCfm_Cb = RFCOMM_ConfigCnf;
    p_info->pL2CA_DisconnectInd_Cb = RFCOMM_DisconnectInd;
    p_info->pL2CA_DisconnectCfm_Cb = 0;
    p_info->pL2CA_QoSViolationInd_Cb = RFCOMM_QoSViolationInd;
    p_info->pL2CA_DataInd_Cb = RFCOMM_BufDataInd;
    p_info->pL2CA_CongestionStatus_Cb = RFCOMM_CongestionStatusInd;

    L2CA_Register(RFCOMM_PSM, p_info);
}

void RFCOMM_ConnectInd(u8* bd_addr, u16 lcid, u16 psm, u8 id) {
    RfcMuxChannel* p_mcb = rfc_alloc_multiplexer_channel(bd_addr, 0);

    rfc_cb.mcb[lcid - 0x40] = p_mcb;

    if (p_mcb == 0) {
        L2CA_ConnectRsp(bd_addr, id, lcid, L2CAP_CONN_NO_RESOURCES, 0);
        return;
    }

    p_mcb->lcid = lcid;

    rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_IND, (u8*)&id);
}

void RFCOMM_ConnectCnf() {}
void RFCOMM_ConfigInd() {}
void RFCOMM_QoSViolationInd(void) {}

void RFCOMM_ConfigCnf(u16 lcid, u8* config) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
        channel = 0;
    } else {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "RFCOMM_ConfigCnf LCID:0x%x", lcid);
        }
    } else {
        rfc_mx_sm_execute(channel, 0xb, config);
    }
}

void RFCOMM_DisconnectInd(u16 lcid, u8 response) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
        channel = 0;
    } else {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (response != 0) {
        L2CA_DisconnectRsp(lcid);
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_1(0x90001, "RFCOMM_DisconnectInd LCID:0x%x", lcid);
        }
    } else {
        rfc_mx_sm_execute(channel, 0xe, 0);
    }
}

void RFCOMM_CongestionStatusInd(u16 lcid, u8 congested) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
        channel = 0;
    } else {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "RFCOMM_CongestionStatusInd dropped LCID:0x%x", lcid);
        }
    } else {
        if (rfc_cb.trace_level >= 4) {
            LogMsg_1(0x90003, "RFCOMM_CongestionStatusInd LCID:0x%x", lcid);
        }
        rfc_process_l2cap_congestion(channel, congested);
    }
}

void RFCOMM_BufDataInd(u16 lcid, BT_HDR* buffer) {
    int index = (int)lcid - 0x40;
    RfcMuxChannel* channel;
    RfcPort* port;
    u8 frame_type;
    u8 dlci;

    if (index >= 10) {
        if (rfc_cb.trace_level >= 1) {
            LogMsg_1(0x90000, "rfc_find_lcid_mcb LCID:0x%x", lcid);
        }
    } else if (index >= 0) {
        channel = rfc_cb.mcb[index];
        if (channel != 0 && channel->lcid != lcid) {
            if (rfc_cb.trace_level >= 2) {
                LogMsg_2(0x90001, "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x", lcid,
                         channel->lcid);
            }
            channel = 0;
        }
    }

    if (channel == 0) {
        if (rfc_cb.trace_level >= 2) {
            LogMsg_1(0x90001, "RFCOMM_BufDataInd LCID:0x%x", lcid);
        }
        GKI_freebuf(buffer);
        return;
    }

    frame_type = rfc_parse_data(channel, &rfc_cb, buffer);
    if (frame_type == 0x32) {
        GKI_freebuf(buffer);
        return;
    }

    if (rfc_cb.dlci == 0) {
        if (frame_type == 4) {
            rfc_process_mx_message(channel, buffer);
        } else {
            rfc_mx_sm_execute(channel, frame_type, 0);
            GKI_freebuf(buffer);
        }
        return;
    }

    dlci = rfc_cb.dlci;
    port = port_find_mcb_dlci_port(channel, dlci);
    if (port == 0 || port->mcb == 0) {
        if (frame_type != 0) {
            if (channel->initiator != 0 || rfc_cb.field_0x02 == 0) {
                if (channel->initiator == 0 && rfc_cb.field_0x02 != 0) {
                    rfc_send_dm(channel, rfc_cb.dlci, rfc_cb.credit_based);
                }
            } else {
                rfc_send_dm(channel, rfc_cb.dlci, rfc_cb.credit_based);
            }
        }
        GKI_freebuf(buffer);
        return;
    }

    if (frame_type == 0) {
        port = port_find_dlci_port(rfc_cb.dlci);
        if (port == 0) {
            rfc_send_dm(channel, rfc_cb.dlci, 1);
            GKI_freebuf(buffer);
            return;
        }
        port->field_0x24 = port->dlci;
        port->mcb = channel;
    }

    if (frame_type == 4) {
        if (buffer->len != 0) {
            rfc_port_sm_execute(port, frame_type, buffer);
        } else {
            GKI_freebuf(buffer);
        }
        if (rfc_cb.credit_based != 0) {
            rfc_inc_credit(port);
        }
    } else {
        rfc_port_sm_execute(port, 0, 0);
        GKI_freebuf(buffer);
    }
}

void rfc_save_lcid_mcb(RfcMuxChannel* p_mcb, u16 lcid) {
    rfc_cb.mcb[lcid - 0x40] = p_mcb;
}
