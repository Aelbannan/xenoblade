/* "src/kyoshin/CUICfManager.cpp" line 0 "kyoshin/CUICfManager.hpp" */
#pragma once

/* "src/kyoshin/CUICfManager.hpp" line 2 "kyoshin/cf/IFlagEvent.hpp" */
#pragma once

namespace cf{

    class IFlagEvent{
    public:
        virtual ~IFlagEvent();

        virtual void FlagEvent1(int arg1, int arg2, int arg3);
        virtual void OnFileEvent(void* arg1);
        virtual void FlagEvent3(int arg1, int arg2, int arg3);
    };

} //namespace cf
/* end "kyoshin/cf/IFlagEvent.hpp" */
/* "src/kyoshin/CUICfManager.hpp" line 3 "monolib/device/CFileHandle.hpp" */
#pragma once

/* "libs/monolib/include/monolib/device/CFileHandle.hpp" line 2 "types.h" */
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
/* "libs/monolib/include/monolib/device/CFileHandle.hpp" line 3 "monolib/util.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util.hpp" line 2 "monolib/util/BoolUtils.hpp" */
#pragma once

namespace ml {
    /* Optimized way of calculating if only one of two bools are true. This may
    or may not have had a dedicated inline function, but it'll get one anyway */
    inline bool boolXor(bool a, bool b){
        return a ^ b;
    }
}
/* end "monolib/util/BoolUtils.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 3 "monolib/util/CChildListNode.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/CChildListNode.hpp" line 2 "monolib/util/CDoubleListNode.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/CDoubleListNode.hpp" line 2 "types.h" */
/* end "types.h" */

/*
Doubly-linked list node
*/
class CDoubleListNode {
public:
    CDoubleListNode* mPrev; //0x0
    CDoubleListNode* mNext; //0x4
    u8 unk8;
    int unkC;

public:
    CDoubleListNode();
    virtual ~CDoubleListNode() {}
    virtual void Reset();

    //Node links
    CDoubleListNode* GetPrev() const { return mPrev; }
    CDoubleListNode* GetNext() const { return mNext; }
    void SetPrev(CDoubleListNode* prev) { mPrev = prev; }
    void SetNext(CDoubleListNode* next) { mNext = next; }
}; // size = 0x10

/*
Doubly-linked, circular list
*/
class CDoubleListHeader {
private:
    /* Circular list which begins *AFTER* mEnd:
    mEnd ("End") -> X ("Top") -> Y -> Z -> mEnd */
    CDoubleListNode* mEnd; //0x0

public:
    CDoubleListHeader() { Reset(); }
    
    void Reset() { mEnd = nullptr; }

    //List iterators
    CDoubleListNode* Begin() const {
        return mEnd != nullptr ? mEnd->GetNext() : nullptr;
    }
    CDoubleListNode* End() const { return mEnd; }

    //Add/remove nodes
    CDoubleListNode* InsertTop(CDoubleListNode* node);
    CDoubleListNode* InsertEnd(CDoubleListNode* node);
    CDoubleListNode* Remove(CDoubleListNode* node);
};
/* end "monolib/util/CDoubleListNode.hpp" */

//Forward declarations
class CChildListNode;

/*
List containing nodes designated as children
*/
template <typename T>
class TChildListHeader : private CDoubleListHeader {
private:
    //Many methods delegate to the implementation
    typedef CDoubleListHeader Base;

private:
    //0x0-4: CDoubleListHeader
    char unk4[0x10 - 0x4];

public:
    TChildListHeader() { Reset(); }
    virtual ~TChildListHeader() {}
    
    void Reset() { Base::Reset(); }

    //List iterators/elements
    T* Begin() const {
        return static_cast<T*>(Base::Begin());
    }
    T* End() const {
        return static_cast<T*>(Base::End());
    }

    //Add/remove nodes
    CChildListNode* InsertTop(CChildListNode* node) {
        return static_cast<CChildListNode*>(Base::InsertTop(node));
    }
    CChildListNode* InsertEnd(CChildListNode* node) {
        return static_cast<CChildListNode*>(Base::InsertEnd(node));
    }
    CChildListNode* Remove(CChildListNode* node) {
        return static_cast<CChildListNode*>(Base::Remove(node));
    }

    //Advance iterators
    T* IterNext(const T* iter) const {
        //Need specific inline depth
        (void)0;
        
        if (iter != nullptr) {
            return iter != static_cast<T*>(End())
                ? static_cast<T*>(iter->GetNext()) : nullptr;
        }

        return nullptr;
    }
    T* IterPrev(const T* iter) const {
        //Need specific inline depth
        (void)0;
        
        if (iter != nullptr) {
            return iter != static_cast<T*>(Begin())
                ? static_cast<T*>(iter->GetPrev()) : nullptr;
        }

        return nullptr;
    }
};

/*
Children-list node
*/
class CChildListNode : public CDoubleListNode {
private:
    typedef TChildListHeader<CChildListNode> ChildrenType;

public:
    CChildListNode();
    virtual ~CChildListNode() {}
    virtual void Reset();

    //Parent node
    CChildListNode* GetParent() const { return mParent; }
    void SetParent(CChildListNode* parent) { mParent = parent; }

    //Children of this child
    ChildrenType& GetChildren() { return mChildren; }
    const ChildrenType& GetChildren() const { return mChildren; }

protected:
    //0x0-10: CDoubleListNode
    //0x10: vtable
    CChildListNode* mParent; //0x14
    char unk18[0x24 - 0x18];
    ChildrenType mChildren; //0x24
};
/* end "monolib/util/CChildListNode.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 4 "monolib/util/CDoubleListNode.hpp" */
/* end "monolib/util/CDoubleListNode.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 5 "monolib/util/CErrorWii.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/CErrorWii.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/util/CErrorWii.hpp" line 3 "monolib/util/fixed_vector.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/fixed_vector.hpp" line 2 "types.h" */
/* end "types.h" */

namespace mtl {
    template <typename T, size_t N>
    struct fixed_vector {

        fixed_vector(){
            mCount = 0;
        }

        void erase(T& value){
            for(int i = 0; i < size(); i++){
                if(mArray[i] == value){
                    while(i < mCount - 1){
                        mArray[i] = mArray[i + 1];
                        i++;
                    }
                
                    mCount--;
                    return;
                }
            }
        }

        //T* begin()
        //T* end()
        //T& at(u32 index)
        //T* find(const T& value)
        //T* erase(T* value)

        int size() const {
            return mCount;
        }

        bool empty() const {
            return mCount == 0;
        }

        T& operator[](u32 index){
            return mArray[index];
        }

        void push_back(const T& value){
            mArray[mCount++] = value;
        }

        T mArray[N];
        int mCount;
    };
} //namespace mtl
/* end "monolib/util/fixed_vector.hpp" */
/* "libs/monolib/include/monolib/util/CErrorWii.hpp" line 4 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
#ifndef RVL_SDK_OS_H
#define RVL_SDK_OS_H
/* "libs/RVL_SDK/include/revolution/OS/OS.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OS.h" line 4 "revolution/OS/OSContext.h" */
#ifndef RVL_SDK_OS_CONTEXT_H
#define RVL_SDK_OS_CONTEXT_H
/* "libs/RVL_SDK/include/revolution/OS/OSContext.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OS_CONTEXT_STATE_FP_SAVED = (1 << 0),
} OSContextState;

typedef struct OSContext {
    u32 gprs[32];  // at 0x0
    u32 cr;        // at 0x80
    u32 lr;        // at 0x84
    u32 ctr;       // at 0x88
    u32 xer;       // at 0x8C
    f64 fprs[32];  // at 0x90
    u32 fpscr_pad; // at 0x190
    u32 fpscr;     // at 0x194
    u32 srr0;      // at 0x198
    u32 srr1;      // at 0x19C
    u16 mode;      // at 0x1A0
    u16 state;     // at 0x1A2
    u32 gqrs[8];   // at 0x1A4
    u32 psf_pad;   // at 0x1C4
    f64 psfs[32];  // at 0x1C8
} OSContext;

void OSSaveFPUContext(OSContext* ctx);
void OSSetCurrentContext(OSContext* ctx);
OSContext* OSGetCurrentContext(void);
BOOL OSSaveContext(OSContext* ctx);
void OSLoadContext(OSContext* ctx);
void* OSGetStackPointer(void);
void OSSwitchFiber(void* func, void* stack);
void OSSwitchFiberEx(u32 r3, u32 r4, u32 r5, u32 r6, void* func, void* stack);
void OSClearContext(OSContext* ctx);
void OSInitContext(OSContext* ctx, void* _srr0, void* stack);
void OSDumpContext(const OSContext* ctx);
void __OSContextInit(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS/OS.h" line 5 "revolution/OS/OSExec.h" */
#ifndef RVL_SDK_OS_EXEC_H
#define RVL_SDK_OS_EXEC_H
/* "libs/RVL_SDK/include/revolution/OS/OSExec.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSExecParams {
    UNKWORD WORD_0x0;
    UNKWORD WORD_0x4;
    char UNK_0x8[0x4];
    void* regionStart; // at 0xC
    void* regionEnd;   // at 0x10
    char UNK_0x14[0x1C - 0x14];
} OSExecParams;

extern BOOL __OSInReboot;
extern void* __OSNextPartitionType;

void __OSGetExecParams(OSExecParams* out);
void __OSLaunchMenu(void);
void __OSBootDol(u32, u32, u32*);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSExec.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OS_CONSOLE_MASK = 0xF0000000,
    OS_CONSOLE_MASK_RVL = 0x00000000,
    OS_CONSOLE_MASK_EMU = 0x10000000,
    OS_CONSOLE_MASK_TDEV = 0x20000000,

    OS_CONSOLE_RVL_PP_1 = 0x00000011,
    OS_CONSOLE_RVL_PP_2_1 = 0x00000012,
    OS_CONSOLE_RVL_PP_2_2 = 0x00000020,
    OS_CONSOLE_RVA_1 = 0x00000100,

    OS_CONSOLE_RVL_EMU = 0x10000008,
    OS_CONSOLE_NDEV_1_0 = 0x10000010,
    OS_CONSOLE_NDEV_1_1 = 0x10000011,
    OS_CONSOLE_NDEV_1_2 = 0x10000012,
    OS_CONSOLE_NDEV_2_0 = 0x10000020,
    OS_CONSOLE_NDEV_2_1 = 0x10000021,
} OSConsoleType;

typedef enum {
    OS_APP_TYPE_IPL = 0x40,
    OS_APP_TYPE_DVD = 0x80,
    OS_APP_TYPE_CHANNEL = 0x81,
} OSAppType;

typedef enum {
    OS_EXC_SYSTEM_RESET,
    OS_EXC_MACHINE_CHECK,
    OS_EXC_DSI,
    OS_EXC_ISI,
    OS_EXC_EXT_INTERRUPT,
    OS_EXC_ALIGNMENT,
    OS_EXC_PROGRAM,
    OS_EXC_FP_UNAVAIL,
    OS_EXC_DECREMENTER,
    OS_EXC_SYSTEM_CALL,
    OS_EXC_TRACE,
    OS_EXC_PERF_MONITOR,
    OS_EXC_IABR,
    OS_EXC_SMI,
    OS_EXC_THERMAL_INT,

    OS_EXC_MAX
} OSExceptionType;

typedef struct OSIOSRev {
    u8 idHi;       // at 0x0
    u8 idLo;       // at 0x1
    u8 verMajor;   // at 0x2
    u8 verMinor;   // at 0x3
    u8 buildMon;   // at 0x4
    u8 buildDay;   // at 0x5
    u16 buildYear; // at 0x6
} OSIOSRev;

typedef void (*OSExceptionHandler)(u8 type, OSContext* ctx);

extern BOOL __OSInIPL;
extern BOOL __OSInNandBoot;
extern BOOL __OSIsGcam;
extern s64 __OSStartTime;
extern OSExecParams __OSRebootParams;

void __OSFPRInit(void);
u32 __OSGetHollywoodRev(void);
void __OSGetIOSRev(OSIOSRev* rev);
u32 OSGetConsoleType(void);
void OSInit(void);
OSExceptionHandler __OSSetExceptionHandler(u8 type, OSExceptionHandler handler);
OSExceptionHandler __OSGetExceptionHandler(u8 type);
void OSDefaultExceptionHandler(u8 type, OSContext* ctx);
void __OSPSInit(void);
u32 __OSGetDIConfig(void);
void OSRegisterVersion(const char* ver);
const char* OSGetAppGamename(void);
u8 OSGetAppType(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
#ifndef RVL_SDK_OS_ADDRESS_H
#define RVL_SDK_OS_ADDRESS_H
/* "libs/RVL_SDK/include/revolution/OS/OSAddress.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

static inline void* OSPhysicalToCached(u32 ofs) {
    return (void*)(ofs + 0x80000000);
}

static inline void* OSPhysicalToUncached(u32 ofs) {
    return (void*)(ofs + 0xC0000000);
}

static inline void* OSCachedToPhysical(const void* ofs) {
    return (u8*)ofs - 0x80000000;
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
#ifndef RVL_SDK_OS_ALARM_H
#define RVL_SDK_OS_ALARM_H
/* "libs/RVL_SDK/include/revolution/OS/OSAlarm.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSAlarm;
typedef struct OSContext;
typedef struct OSThread;

typedef void (*OSAlarmHandler)(struct OSAlarm* alarm, struct OSContext* ctx);

typedef struct OSAlarm {
    OSAlarmHandler handler; // at 0x0
    u32 tag;                // at 0x4
    s64 end;                // at 0x8
    struct OSAlarm* prev;   // at 0x10
    struct OSAlarm* next;   // at 0x14
    s64 period;             // at 0x18
    s64 start;              // at 0x20
    void* userData;         // at 0x28
} OSAlarm;

typedef struct OSAlarmQueue {
    OSAlarm* head; // at 0x0
    OSAlarm* tail; // at 0x4
} OSAlarmQueue;

void __OSInitAlarm(void);
void OSCreateAlarm(OSAlarm* alarm);
void OSSetAlarm(OSAlarm* alarm, s64 tick, OSAlarmHandler handler);
void OSSetPeriodicAlarm(OSAlarm* alarm, s64 tick, s64 period,
                        OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm* alarm);
void OSSetAlarmTag(OSAlarm* alarm, u32 tag);
void OSSetAlarmUserData(OSAlarm* alarm, void* userData);
void* OSGetAlarmUserData(const OSAlarm* alarm);
void __OSCancelInternalAlarms(struct OSThread* thread);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
#ifndef RVL_SDK_OS_ALLOC_H
#define RVL_SDK_OS_ALLOC_H
/* "libs/RVL_SDK/include/revolution/OS/OSAlloc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

extern volatile s32 __OSCurrHeap;

void* OSAllocFromHeap(s32 handle, s32 size);
void OSFreeToHeap(s32 handle, void* p);
s32 OSSetCurrentHeap(s32 handle);
void* OSInitAlloc(void* start, void* end, s32 numHeaps);
s32 OSCreateHeap(void* start, void* end);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
#ifndef RVL_SDK_OS_ARENA_H
#define RVL_SDK_OS_ARENA_H
/* "libs/RVL_SDK/include/revolution/OS/OSArena.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void* OSGetMEM1ArenaHi(void);
void* OSGetMEM2ArenaHi(void);
void* OSGetArenaHi(void);

void* OSGetMEM1ArenaLo(void);
void* OSGetMEM2ArenaLo(void);
void* OSGetArenaLo(void);

void OSSetMEM1ArenaHi(void* hi);
void OSSetMEM2ArenaHi(void* hi);
void OSSetArenaHi(void* hi);

void OSSetMEM1ArenaLo(void* lo);
void OSSetMEM2ArenaLo(void* lo);
void OSSetArenaLo(void* lo);

void* OSAllocFromMEM1ArenaLo(size_t size, u32 align);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
#ifndef RVL_SDK_OS_AUDIOSYSTEM_H
#define RVL_SDK_OS_AUDIOSYSTEM_H
/* "libs/RVL_SDK/include/revolution/OS/OSAudioSystem.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitAudioSystem(void);
void __OSStopAudioSystem(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
#ifndef RVL_SDK_OS_CACHE_H
#define RVL_SDK_OS_CACHE_H
/* "libs/RVL_SDK/include/revolution/OS/OSCache.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_CACHE_BASE 0xE0000000

// Forward declarations
typedef struct OSContext;

void DCEnable(void);
void DCInvalidateRange(const void* buf, u32 len);
void DCFlushRange(const void* buf, u32 len);
void DCStoreRange(const void* buf, u32 len);
void DCFlushRangeNoSync(const void* buf, u32 len);
void DCStoreRangeNoSync(const void* buf, u32 len);
void DCZeroRange(const void* buf, u32 len);

void ICInvalidateRange(const void* buf, u32 len);
void ICFlashInvalidate(void);
void ICEnable(void);

void LCEnable(void);
void LCDisable(void);
void LCLoadBlocks(void* dst, const void* src, u32 blocks);
void LCStoreBlocks(void* dst, const void* src, u32 blocks);
u32 LCStoreData(void* dst, const void* src, u32 len);
u32 LCQueueLength(void);
void LCQueueWait(u32 n);

void L2Enable(void);
void L2Disable(void);
void L2GlobalInvalidate(void);

void DMAErrorHandler(u8 error, OSContext* ctx, u32 dsisr, u32 dar, ...);

void __OSCacheInit(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
#ifndef RVL_SDK_OS_CRC_H
#define RVL_SDK_OS_CRC_H
/* "libs/RVL_SDK/include/revolution/OS/OSCrc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

u16 OSCalcCRC16(const u8* array, s32 length);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
#ifndef RVL_SDK_OS_ERROR_H
#define RVL_SDK_OS_ERROR_H
/* "libs/RVL_SDK/include/revolution/OS/OSError.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

#define OS_ERROR(...) OSPanic(__FILE__, __LINE__, __VA_ARGS__)

#define OS_ASSERT(exp, ...)                                                    \
    if (!(exp))                                                                \
    OSPanic(__FILE__, __LINE__, __VA_ARGS__)

typedef enum {
    OS_ERR_SYSTEM_RESET,
    OS_ERR_MACHINE_CHECK,
    OS_ERR_DSI,
    OS_ERR_ISI,
    OS_ERR_EXT_INTERRUPT,
    OS_ERR_ALIGNMENT,
    OS_ERR_PROGRAM,
    OS_ERR_FP_UNAVAIL,
    OS_ERR_DECREMENTER,
    OS_ERR_SYSTEM_CALL,
    OS_ERR_TRACE,
    OS_ERR_PERF_MONITOR,
    OS_ERR_IABR,
    OS_ERR_SMI,
    OS_ERR_THERMAL_INT,
    OS_ERR_PROTECTION,
    OS_ERR_FP_EXCEPTION,

    OS_ERR_MAX
} OSErrorType;

typedef void (*OSErrorHandler)(u8 error, struct OSContext* ctx, u32 dsisr, u32 dar,
                               ...);

extern OSErrorHandler __OSErrorTable[OS_ERR_MAX];
extern u32 __OSFpscrEnableBits;

/* DECL_WEAK */ void OSReport(const char* msg, ...);
/* DECL_WEAK */ void OSPanic(const char* file, int line, const char* msg, ...);
/* DECL_WEAK */ void OSVReport(const char* msg, va_list arg);

OSErrorHandler OSSetErrorHandler(u16 error, OSErrorHandler handler);
void __OSUnhandledException(u8 error, struct OSContext* ctx, u32 dsisr, u32 dar);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
#ifndef RVL_SDK_OS_FAST_CAST_H
#define RVL_SDK_OS_FAST_CAST_H
/* "libs/RVL_SDK/include/revolution/OS/OSFastCast.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_GQR_TYPE_U8 4
#define OS_GQR_TYPE_U16 5
#define OS_GQR_TYPE_S8 6
#define OS_GQR_TYPE_S16 7

static void OSInitFastCast(void) {
    ASM (
        li r3, 4
        oris r3, r3, 4
        mtgqr2 r3

        li r3, 5
        oris r3, r3, 5
        mtgqr3 r3

        li r3, 6
        oris r3, r3, 6
        mtgqr4 r3
        
        li r3, 7
        oris r3, r3, 7
        mtgqr5 r3
    )
}

static void OSSetGQR6(register u32 type, register u32 scale) {
    register u32 val = ((scale << 8 | type) << 16) | ((scale << 8) | type);

    ASM (
        mtspr 0x396, val
    )
}

static void OSSetGQR7(register u32 type, register u32 scale) {
    register u32 val = ((scale << 8 | type) << 16) | ((scale << 8) | type);

    ASM (
        mtspr 0x397, val
    )
}

/******************************************************************************
 *
 * Convert from U8
 *
 ******************************************************************************/
static f32 __OSu8tof32(register u8* in) {
    register f32 ret;

    ASM (
        psq_l ret, 0(in), 1, 2
    )

    return ret;
}

static void OSu8tof32(u8* in, volatile f32* out) {
    *out = __OSu8tof32(in);
}

/******************************************************************************
 *
 * Convert from U16
 *
 ******************************************************************************/
static f32 __OSu16tof32(register u16* arg) {
    register f32 ret;

    ASM (
        psq_l ret, 0(arg), 1, 3
    )

    return ret;
}

static void OSu16tof32(u16* in, volatile f32* out) {
    *out = __OSu16tof32(in);
}

/******************************************************************************
 *
 * Convert from S16
 *
 ******************************************************************************/
static f32 __OSs16tof32(register s16* arg) {
    register f32 ret;

    ASM (
        psq_l ret, 0(arg), 1, 5
    )

    return ret;
}

static void OSs16tof32(s16* in, volatile f32* out) {
    *out = __OSs16tof32(in);
}

/******************************************************************************
 *
 * Convert from F32
 *
 ******************************************************************************/
static u8 __OSf32tou8(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    u8 r;

    ASM (
        psq_st arg, 0(ptr), 1, 2
    )

    r = *(u8*)ptr;
    return r;
}

static void OSf32tou8(f32* in, volatile u8* out) {
    *out = __OSf32tou8(*in);
}

static u16 __OSf32tou16(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    u16 r;

    ASM (
        psq_st arg, 0(ptr), 1, 3
    )

    r = *(u16*)ptr;
    return r;
}

static void OSf32tou16(f32* in, volatile u16* out) {
    *out = __OSf32tou16(*in);
}

static s16 __OSf32tos16(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    s16 r;

    ASM (
        psq_st arg, 0(ptr), 1, 5
    )

    r = *(s16*)ptr;
    return r;
}

static void OSf32tos16(f32* in, volatile s16* out) {
    *out = __OSf32tos16(*in);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
#ifndef RVL_SDK_OS_FATAL_H
#define RVL_SDK_OS_FATAL_H
/* "libs/RVL_SDK/include/revolution/OS/OSFatal.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSFatal.h" line 4 "revolution/GX/GXTypes.h" */
#ifndef RVL_SDK_GX_TYPES_H
#define RVL_SDK_GX_TYPES_H
/* "libs/RVL_SDK/include/revolution/GX/GXTypes.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Pack value into bitfield.
 * Value is shifted to the specified bit position.
 * (Bit indices are LSB)
 */
#define GX_BITSET(field, pos, size, value)                                     \
    __rlwimi((field), (value), 31 - (pos) - (size) + 1, (pos),                 \
             (pos) + (size) - 1)

/**
 * Compose value from bitfield.
 * Value is shifted after masking.
 * (Bit indices are LSB)
 */
#define GX_BITGET(field, pos, size)                                            \
    ((field) >> (31 - (pos) - (size) + 1) & ((1 << (size)) - 1))

/**
 * Pack value into bitfield.
 * Value is not shifted, only masked.
 * (Bit indices are LSB)
 */
#define GX_BITSET_TRUNC(field, pos, size, value)                               \
    __rlwimi((field), (value), 0, (pos), (pos) + (size) - 1)

/**
 * Compose value from bitfield.
 * Value is not shifted, only masked.
 * (Bit indices are LSB)
 */
#define GX_BITGET_TRUNC(field, pos, size)                                      \
    ((field) & (((1 << (size)) - 1) << (32 - (pos) - (size))))

/**
 * Common types used throughout many GX files.
 * To avoid circular depencies, common structures and all enums have been moved
 * here. Everything is organized in alphabetical order.
 */

// Differentiate between 8-byte and boolean values
// Real name! (from patent)
typedef unsigned char GXBool;

#define GX_TRUE ((GXBool)TRUE)
#define GX_FALSE ((GXBool)FALSE)

#define GX_ENABLE GX_TRUE
#define GX_DISABLE GX_FALSE

/**
 * Common structs
 */

typedef struct _GXColor {
    u8 r, g, b, a;
} GXColor;

typedef struct _GXColorS10 {
    s16 r, g, b, a;
} GXColorS10;

/**
 * Common enums
 */

typedef enum _GXAlphaOp {
    GX_AOP_AND,
    GX_AOP_OR,
    GX_AOP_XOR,
    GX_AOP_XNOR,

    GX_MAX_ALPHAOP
} GXAlphaOp;

typedef enum _GXAnisotropy {
    GX_ANISO_1,
    GX_ANISO_2,
    GX_ANISO_4,

    GX_MAX_ANISOTROPY
} GXAnisotropy;

typedef enum _GXAttnFn {
    GX_AF_SPEC,
    GX_AF_SPOT,
    GX_AF_NONE,
} GXAttnFn;

typedef enum _GXAttr {
    GX_VA_PNMTXIDX,   // Position/Normal matrix index
    GX_VA_TEX0MTXIDX, // GX_TEXCOORD0 matrix index
    GX_VA_TEX1MTXIDX, // GX_TEXCOORD1 matrix index
    GX_VA_TEX2MTXIDX, // GX_TEXCOORD2 matrix index
    GX_VA_TEX3MTXIDX, // GX_TEXCOORD3 matrix index
    GX_VA_TEX4MTXIDX, // GX_TEXCOORD4 matrix index
    GX_VA_TEX5MTXIDX, // GX_TEXCOORD5 matrix index
    GX_VA_TEX6MTXIDX, // GX_TEXCOORD6 matrix index
    GX_VA_TEX7MTXIDX, // GX_TEXCOORD7 matrix index

    GX_VA_POS, // Position
    GX_VA_NRM, // Normal

    GX_VA_CLR0, // Color 0
    GX_VA_CLR1, // Color 1

    GX_VA_TEX0, // Texture Coordinate 0
    GX_VA_TEX1, // Texture Coordinate 1
    GX_VA_TEX2, // Texture Coordinate 2
    GX_VA_TEX3, // Texture Coordinate 3
    GX_VA_TEX4, // Texture Coordinate 4
    GX_VA_TEX5, // Texture Coordinate 5
    GX_VA_TEX6, // Texture Coordinate 6
    GX_VA_TEX7, // Texture Coordinate 7

    GX_POS_MTX_ARRAY,
    GX_NRM_MTX_ARRAY,
    GX_TEX_MTX_ARRAY,
    GX_LIGHT_ARRAY,

    GX_VA_NBT, // All three normal elements (normal/binormal/tangent)

    GX_VA_MAX_ATTR,
    GX_VA_NULL = 255
} GXAttr;

typedef enum _GXAttrType {
    GX_NONE,   // No data sent
    GX_DIRECT, // Data sent directly to FIFO
    GX_INDEX8, // 8-bit index sent to FIFO
    GX_INDEX16 // 16-bit index sent to FIFO
} GXAttrType;

typedef enum _GXBlendFactor {
    GX_BL_ZERO,
    GX_BL_ONE,
    GX_BL_SRCCLR,
    GX_BL_INVSRCCLR,
    GX_BL_SRCALPHA,
    GX_BL_INVSRCALPHA,
    GX_BL_DSTALPHA,
    GX_BL_INVDSTALPHA,

    GX_BL_DSTCLR = GX_BL_SRCCLR,
    GX_BL_INVDSTCLR = GX_BL_INVSRCCLR
} GXBlendFactor;

typedef enum _GXBlendMode {
    GX_BM_NONE,
    GX_BM_BLEND,
    GX_BM_LOGIC,
    GX_BM_SUBTRACT,

    GX_MAX_BLENDMODE
} GXBlendMode;

typedef enum _GXChannelID {
    GX_COLOR0,
    GX_COLOR1,
    GX_ALPHA0,
    GX_ALPHA1,
    GX_COLOR0A0,
    GX_COLOR1A1,
    GX_COLOR_ZERO,
    GX_ALPHA_BUMP,
    GX_ALPHA_BUMPN,

    GX_COLOR_NULL = 255
} GXChannelID;

typedef enum _GXCITexFmt {
    GX_TF_C4 = 8,
    GX_TF_C8,
    GX_TF_C14X2,
} GXCITexFmt;

typedef enum _GXClearZ {
    GX_CLEAR_Z_MIN = 0,
    GX_CLEAR_Z_MAX = (1 << 24) - 1,
} GXClearZ;

typedef enum _GXClipMode {
    // "ClipDisable" in XF mem, so 0 = enable
    GX_CLIP_ENABLE,
    GX_CLIP_DISABLE,
} GXClipMode;

typedef enum _GXColorSrc { GX_SRC_REG, GX_SRC_VTX } GXColorSrc;

typedef enum _GXCompare {
    GX_NEVER,
    GX_LESS,
    GX_EQUAL,
    GX_LEQUAL,
    GX_GREATER,
    GX_NEQUAL,
    GX_GEQUAL,
    GX_ALWAYS
} GXCompare;

typedef enum _GXCompCnt {
    GX_POS_XY = 0,
    GX_POS_XYZ,

    GX_NRM_XYZ = 0,
    GX_NRM_NBT,
    GX_NRM_NBT3,

    GX_CLR_RGB = 0,
    GX_CLR_RGBA,

    GX_TEX_S = 0,
    GX_TEX_ST
} GXCompCnt;

typedef enum _GXCompType {
    GX_U8,
    GX_S8,
    GX_U16,
    GX_S16,
    GX_F32,

    GX_RGB565 = 0,
    GX_RGB8,
    GX_RGBX8,
    GX_RGBA4,
    GX_RGBA6,
    GX_RGBA8
} GXCompType;

typedef enum _GXCopyClamp {
    GX_CLAMP_NONE,
    GX_CLAMP_TOP,
    GX_CLAMP_BOTTOM,
    GX_CLAMP_ALL,
} GXCopyClamp;

typedef enum _GXCullMode {
    GX_CULL_NONE,
    GX_CULL_FRONT,
    GX_CULL_BACK,
    GX_CULL_ALL
} GXCullMode;

typedef enum _GXDiffuseFn { GX_DF_NONE, GX_DF_SIGN, GX_DF_CLAMP } GXDiffuseFn;

typedef enum _GXDirtyFlag {
    GX_DIRTY_SU_TEX = (1 << 0),
    GX_DIRTY_BP_MASK = (1 << 1),
    GX_DIRTY_GEN_MODE = (1 << 2),
    GX_DIRTY_VCD = (1 << 3),
    GX_DIRTY_VAT = (1 << 4),
    // . . .
    GX_DIRTY_AMB_COLOR0 = (1 << 8),
    GX_DIRTY_AMB_COLOR1 = (1 << 9),
    GX_DIRTY_MAT_COLOR0 = (1 << 10),
    GX_DIRTY_MAT_COLOR1 = (1 << 11),
    GX_DIRTY_CHAN_COLOR0 = (1 << 12),
    GX_DIRTY_CHAN_COLOR1 = (1 << 13),
    GX_DIRTY_CHAN_ALPHA0 = (1 << 14),
    GX_DIRTY_CHAN_ALPHA1 = (1 << 15),
    GX_DIRTY_TEX0 = (1 << 16),
    GX_DIRTY_TEX1 = (1 << 17),
    GX_DIRTY_TEX2 = (1 << 18),
    GX_DIRTY_TEX3 = (1 << 19),
    GX_DIRTY_TEX4 = (1 << 20),
    GX_DIRTY_TEX5 = (1 << 21),
    GX_DIRTY_TEX6 = (1 << 22),
    GX_DIRTY_TEX7 = (1 << 23),
    GX_DIRTY_NUM_COLORS = (1 << 24),
    GX_DIRTY_NUM_TEX = (1 << 25),
    GX_DIRTY_MTX_IDX = (1 << 26),
    GX_DIRTY_PROJECTION = (1 << 27),
    GX_DIRTY_VIEWPORT = (1 << 28),

    GX_AMB_MAT_MASK = GX_DIRTY_AMB_COLOR0 | GX_DIRTY_AMB_COLOR1 |
                      GX_DIRTY_MAT_COLOR0 | GX_DIRTY_MAT_COLOR1,

    GX_LIGHT_CHAN_MASK = GX_DIRTY_CHAN_COLOR0 | GX_DIRTY_CHAN_COLOR1 |
                         GX_DIRTY_CHAN_ALPHA0 | GX_DIRTY_CHAN_ALPHA1 |
                         GX_DIRTY_NUM_COLORS,

    GX_TEX_GEN_MASK = 0x2FF0000,
} GXDirtyFlag;

typedef enum _GXDistAttnFn {
    GX_DA_OFF,
    GX_DA_GENTLE,
    GX_DA_MEDIUM,
    GX_DA_STEEP
} GXDistAttnFn;

typedef enum _GXFogType {
    GX_FOG_NONE,

    GX_FOG_PERSP_LIN = 2,
    GX_FOG_PERSP_EXP = 4,
    GX_FOG_PERSP_EXP2 = 5,
    GX_FOG_PERSP_REVEXP = 6,
    GX_FOG_PERSP_REVEXP2 = 7,

    // Fourth bit is set to mark orthographic
    GX_FOG_ORTHO_LIN = 1 << 3 | GX_FOG_PERSP_LIN,
    GX_FOG_ORTHO_EXP = 1 << 3 | GX_FOG_PERSP_EXP,
    GX_FOG_ORTHO_EXP2 = 1 << 3 | GX_FOG_PERSP_EXP2,
    GX_FOG_ORTHO_REVEXP = 1 << 3 | GX_FOG_PERSP_REVEXP,
    GX_FOG_ORTHO_REVEXP2 = 1 << 3 | GX_FOG_PERSP_REVEXP2
} GXFogType;

// Access components of the fog type
#define GX_FOG_GET_PROJ(x) ((x) >> 3 & 1)
#define GX_FOG_GET_FSEL(x) ((x) & 7)

typedef enum _GXIndTexAlphaSel {
    GX_ITBA_OFF,
    GX_ITBA_S,
    GX_ITBA_T,
    GX_ITBA_U,

    GX_MAX_ITBALPHA
} GXIndTexAlphaSel;

typedef enum _GXIndTexBiasSel {
    GX_ITB_NONE,
    GX_ITB_S,
    GX_ITB_T,
    GX_ITB_ST,
    GX_ITB_U,
    GX_ITB_SU,
    GX_ITB_TU,
    GX_ITB_STU,

    GX_MAX_ITBIAS
} GXIndTexBiasSel;

typedef enum _GXIndTexFormat {
    GX_ITF_8,
    GX_ITF_5,
    GX_ITF_4,
    GX_ITF_3,

    GX_MAX_ITFORMAT
} GXIndTexFormat;

typedef enum _GXIndTexMtxID {
    GX_ITM_OFF,
    GX_ITM_0,
    GX_ITM_1,
    GX_ITM_2,

    GX_ITM_S0 = 5,
    GX_ITM_S1,
    GX_ITM_S2,

    GX_ITM_T0 = 9,
    GX_ITM_T1,
    GX_ITM_T2,
} GXIndTexMtxID;

typedef enum _GXIndTexScale {
    GX_ITS_1,
    GX_ITS_2,
    GX_ITS_4,
    GX_ITS_8,
    GX_ITS_16,
    GX_ITS_32,
    GX_ITS_64,
    GX_ITS_128,
    GX_ITS_256,

    GX_MAX_ITSCALE
} GXIndTexScale;

typedef enum _GXIndTexStageID {
    GX_INDTEXSTAGE0,
    GX_INDTEXSTAGE1,
    GX_INDTEXSTAGE2,
    GX_INDTEXSTAGE3,

    GX_MAX_INDTEXSTAGE
} GXIndTexStageID;

typedef enum _GXIndTexWrap {
    GX_ITW_OFF,
    GX_ITW_256,
    GX_ITW_128,
    GX_ITW_64,
    GX_ITW_32,
    GX_ITW_16,
    GX_ITW_0,

    GX_MAX_ITWRAP,
} GXIndTexWrap;

typedef enum _GXLightID {
    GX_LIGHT0 = (1 << 0),
    GX_LIGHT1 = (1 << 1),
    GX_LIGHT2 = (1 << 2),
    GX_LIGHT3 = (1 << 3),
    GX_LIGHT4 = (1 << 4),
    GX_LIGHT5 = (1 << 5),
    GX_LIGHT6 = (1 << 6),
    GX_LIGHT7 = (1 << 7),

    GX_MAX_LIGHT = (1 << 8),
    GX_LIGHT_NULL = 0
} GXLightID;

typedef enum _GXLogicOp {
    GX_LO_CLEAR,
    GX_LO_AND,
    GX_LO_REVAND,
    GX_LO_COPY,
    GX_LO_INVAND,
    GX_LO_NOOP,
    GX_LO_XOR,
    GX_LO_OR,
    GX_LO_NOR,
    GX_LO_EQUIV,
    GX_LO_INV,
    GX_LO_REVOR,
    GX_LO_INVCOPY,
    GX_LO_INVOR,
    GX_LO_NAND,
    GX_LO_SET
} GXLogicOp;

typedef enum _GXMtxType {
    GX_MTX_3x4,
    GX_MTX_2x4,
} GXMtxType;

typedef enum _GXPixelFmt {
    GX_PF_RGB8_Z24,
    GX_PF_RGBA6_Z24,
    GX_PF_RGBA565_Z16,
    GX_PF_Z24,
    GX_PF_Y8,
    GX_PF_U8,
    GX_PF_V8,
    GX_PF_YUV420,

    GX_MAX_PIXELFMT
} GXPixelFmt;

/**
 * Matrix column index into XF memory.
 * (Multiply by row dimension to get XF mem offset)
 */
typedef enum _GXPosNrmMtx {
    GX_PNMTX0 = 0,
    GX_PNMTX1 = 3,
    GX_PNMTX2 = 6,
    GX_PNMTX3 = 9,
    GX_PNMTX4 = 12,
    GX_PNMTX5 = 15,
    GX_PNMTX6 = 18,
    GX_PNMTX7 = 21,
    GX_PNMTX8 = 24,
    GX_PNMTX9 = 27
} GXPosNrmMtx;

typedef enum _GXPrimitive {
    GX_POINTS = 0xB8,
    GX_LINES = 0xA8,
    GX_LINESTRIP = 0xB0,
    GX_TRIANGLES = 0x90,
    GX_TRIANGLESTRIP = 0x98,
    GX_TRIANGLEFAN = 0xA0,
    GX_QUADS = 0x80,
} GXPrimitive;

typedef enum _GXProjectionType {
    GX_PERSPECTIVE,
    GX_ORTHOGRAPHIC
} GXProjectionType;

typedef enum _GXSpotFn {
    GX_SP_OFF,
    GX_SP_FLAT,
    GX_SP_COS,
    GX_SP_COS2,
    GX_SP_SHARP,
    GX_SP_RING1,
    GX_SP_RING2
} GXSpotFn;

typedef enum _GXTevAlphaArg {
    GX_CA_APREV,
    GX_CA_A0,
    GX_CA_A1,
    GX_CA_A2,
    GX_CA_TEXA,
    GX_CA_RASA,
    GX_CA_KONST,
    GX_CA_ZERO,
    GX_CA_ONE
} GXTevAlphaArg;

typedef enum _GXTevBias {
    GX_TB_ZERO,
    GX_TB_ADDHALF,
    GX_TB_SUBHALF,

    GX_MAX_TEVBIAS
} GXTevBias;

typedef enum _GXTevColorArg {
    GX_CC_CPREV,
    GX_CC_APREV,
    GX_CC_C0,
    GX_CC_A0,
    GX_CC_C1,
    GX_CC_A1,
    GX_CC_C2,
    GX_CC_A2,
    GX_CC_TEXC,
    GX_CC_TEXA,
    GX_CC_RASC,
    GX_CC_RASA,
    GX_CC_ONE,
    GX_CC_HALF,
    GX_CC_KONST,
    GX_CC_ZERO,
    GX_CC_TEXRRR,
    GX_CC_TEXGGG,
    GX_CC_TEXBBB,

    GX_CC_QUARTER = GX_CC_KONST
} GXTevColorArg;

typedef enum _GXTevColorChan {
    GX_CH_RED,
    GX_CH_GREEN,
    GX_CH_BLUE,
    GX_CH_ALPHA
} GXTevColorChan;

typedef enum _GXTevOp {
    GX_TEV_ADD,
    GX_TEV_SUB,

    GX_TEV_COMP_R8_GT = 8,
    GX_TEV_COMP_R8_EQ,
    GX_TEV_COMP_GR16_GT,
    GX_TEV_COMP_GR16_EQ,
    GX_TEV_COMP_BGR24_GT,
    GX_TEV_COMP_BGR24_EQ,
    GX_TEV_COMP_RGB8_GT,
    GX_TEV_COMP_RGB8_EQ,

    GX_TEV_COMP_A8_GT = GX_TEV_COMP_RGB8_GT,
    GX_TEV_COMP_A8_EQ = GX_TEV_COMP_RGB8_EQ
} GXTevOp;

typedef enum _GXTevRegID {
    GX_TEVPREV,
    GX_TEVREG0,
    GX_TEVREG1,
    GX_TEVREG2,

    GX_MAX_TEVREG
} GXTevRegID;

typedef enum _GXTevScale {
    GX_CS_SCALE_1,
    GX_CS_SCALE_2,
    GX_CS_SCALE_4,
    GX_CS_DIVIDE_2,

    GX_MAX_TEVSCALE
} GXTevScale;

typedef enum _GXTevStageID {
    GX_TEVSTAGE0,
    GX_TEVSTAGE1,
    GX_TEVSTAGE2,
    GX_TEVSTAGE3,
    GX_TEVSTAGE4,
    GX_TEVSTAGE5,
    GX_TEVSTAGE6,
    GX_TEVSTAGE7,
    GX_TEVSTAGE8,
    GX_TEVSTAGE9,
    GX_TEVSTAGE10,
    GX_TEVSTAGE11,
    GX_TEVSTAGE12,
    GX_TEVSTAGE13,
    GX_TEVSTAGE14,
    GX_TEVSTAGE15,

    GX_MAX_TEVSTAGE
} GXTevStageID;

typedef enum _GXTevSwapSel {
    GX_TEV_SWAP0,
    GX_TEV_SWAP1,
    GX_TEV_SWAP2,
    GX_TEV_SWAP3,

    GX_MAX_TEVSWAP
} GXTevSwapSel;

typedef enum _GXTevKAlphaSel {
    GX_TEV_KASEL_8_8,
    GX_TEV_KASEL_7_8,
    GX_TEV_KASEL_6_8,
    GX_TEV_KASEL_5_8,
    GX_TEV_KASEL_4_8,
    GX_TEV_KASEL_3_8,
    GX_TEV_KASEL_2_8,
    GX_TEV_KASEL_1_8,

    GX_TEV_KASEL_1 = 0,
    GX_TEV_KASEL_3_4 = 2,
    GX_TEV_KASEL_1_2 = 4,
    GX_TEV_KASEL_1_4 = 6,

    GX_TEV_KASEL_K0_R = 16,
    GX_TEV_KASEL_K1_R,
    GX_TEV_KASEL_K2_R,
    GX_TEV_KASEL_K3_R,
    GX_TEV_KASEL_K0_G,
    GX_TEV_KASEL_K1_G,
    GX_TEV_KASEL_K2_G,
    GX_TEV_KASEL_K3_G,
    GX_TEV_KASEL_K0_B,
    GX_TEV_KASEL_K1_B,
    GX_TEV_KASEL_K2_B,
    GX_TEV_KASEL_K3_B,
    GX_TEV_KASEL_K0_A,
    GX_TEV_KASEL_K1_A,
    GX_TEV_KASEL_K2_A,
    GX_TEV_KASEL_K3_A
} GXTevKAlphaSel;

typedef enum _GXTevKColorID {
    GX_KCOLOR0,
    GX_KCOLOR1,
    GX_KCOLOR2,
    GX_KCOLOR3,

    GX_MAX_KCOLOR
} GXTevKColorID;

typedef enum _GXTevKColorSel {
    GX_TEV_KCSEL_8_8,
    GX_TEV_KCSEL_7_8,
    GX_TEV_KCSEL_6_8,
    GX_TEV_KCSEL_5_8,
    GX_TEV_KCSEL_4_8,
    GX_TEV_KCSEL_3_8,
    GX_TEV_KCSEL_2_8,
    GX_TEV_KCSEL_1_8,

    GX_TEV_KCSEL_1 = 0,
    GX_TEV_KCSEL_3_4 = 2,
    GX_TEV_KCSEL_1_2 = 4,
    GX_TEV_KCSEL_1_4 = 6,

    GX_TEV_KCSEL_K0 = 12,
    GX_TEV_KCSEL_K1,
    GX_TEV_KCSEL_K2,
    GX_TEV_KCSEL_K3,
    GX_TEV_KCSEL_K0_R,
    GX_TEV_KCSEL_K1_R,
    GX_TEV_KCSEL_K2_R,
    GX_TEV_KCSEL_K3_R,
    GX_TEV_KCSEL_K0_G,
    GX_TEV_KCSEL_K1_G,
    GX_TEV_KCSEL_K2_G,
    GX_TEV_KCSEL_K3_G,
    GX_TEV_KCSEL_K0_B,
    GX_TEV_KCSEL_K1_B,
    GX_TEV_KCSEL_K2_B,
    GX_TEV_KCSEL_K3_B,
    GX_TEV_KCSEL_K0_A,
    GX_TEV_KCSEL_K1_A,
    GX_TEV_KCSEL_K2_A,
    GX_TEV_KCSEL_K3_A
} GXTevKColorSel;

typedef enum _GXTevMode {
    GX_MODULATE,
    GX_DECAL,
    GX_BLEND,
    GX_REPLACE,
    GX_PASSCLR
} GXTevMode;

typedef enum _GXTexCoordID {
    GX_TEXCOORD0,
    GX_TEXCOORD1,
    GX_TEXCOORD2,
    GX_TEXCOORD3,
    GX_TEXCOORD4,
    GX_TEXCOORD5,
    GX_TEXCOORD6,
    GX_TEXCOORD7,

    GX_MAX_TEXCOORD,
    GX_TEXCOORD_NULL = 255
} GXTexCoordID;

typedef enum _GXTexFilter {
    GX_NEAR,
    GX_LINEAR,
    GX_NEAR_MIP_NEAR,
    GX_LIN_MIP_NEAR,
    GX_NEAR_MIP_LIN,
    GX_LIN_MIP_LIN,
} GXTexFilter;

typedef enum _GXTexFmt {
    GX_TF_I4,
    GX_TF_I8,
    GX_TF_IA4,
    GX_TF_IA8,
    GX_TF_RGB565,
    GX_TF_RGB5A3,
    GX_TF_RGBA8,
    GX_TF_CMPR = 14,

    GX_CTF_R4 = 32,
    GX_CTF_RA4 = 34,
    GX_CTF_RA8 = 35,
    GX_CTF_YUVA8 = 38,
    GX_CTF_A8 = 39,
    GX_CTF_R8 = 40,
    GX_CTF_G8 = 41,
    GX_CTF_B8 = 42,
    GX_CTF_RG8 = 43,
    GX_CTF_GB8 = 44,

    GX_TF_Z8 = 17,
    GX_TF_Z16 = 19,
    GX_TF_Z24X8 = 22,

    GX_CTF_Z4 = 48,
    GX_CTF_Z8M = 57,
    GX_CTF_Z8L = 58,
    GX_CTF_Z16L = 60,

    GX_TF_A8 = GX_CTF_YUVA8
} GXTexFmt;

typedef enum _GXTexGenSrc {
    GX_TG_POS,
    GX_TG_NRM,
    GX_TG_BINRM,
    GX_TG_TANGENT,
    GX_TG_TEX0,
    GX_TG_TEX1,
    GX_TG_TEX2,
    GX_TG_TEX3,
    GX_TG_TEX4,
    GX_TG_TEX5,
    GX_TG_TEX6,
    GX_TG_TEX7,
    GX_TG_TEXCOORD0,
    GX_TG_TEXCOORD1,
    GX_TG_TEXCOORD2,
    GX_TG_TEXCOORD3,
    GX_TG_TEXCOORD4,
    GX_TG_TEXCOORD5,
    GX_TG_TEXCOORD6,
    GX_TG_COLOR0,
    GX_TG_COLOR1,
} GXTexGenSrc;

typedef enum _GXTexGenType {
    GX_TG_MTX3x4,
    GX_TG_MTX2x4,
    GX_TG_BUMP0,
    GX_TG_BUMP1,
    GX_TG_BUMP2,
    GX_TG_BUMP3,
    GX_TG_BUMP4,
    GX_TG_BUMP5,
    GX_TG_BUMP6,
    GX_TG_BUMP7,
    GX_TG_SRTG
} GXTexGenType;

typedef enum _GXTexMapID {
    GX_TEXMAP0,
    GX_TEXMAP1,
    GX_TEXMAP2,
    GX_TEXMAP3,
    GX_TEXMAP4,
    GX_TEXMAP5,
    GX_TEXMAP6,
    GX_TEXMAP7,
    GX_MAX_TEXMAP,

    GX_TEXMAP_NULL = 255,
    GX_TEX_DISABLE
} GXTexMapID;

typedef enum _GXTexMtx {
    // Any dimension (in standard XF matrix memory)
    // Enum represents base row of matrix
    GX_TEXMTX0 = 30,
    GX_TEXMTX1 = 33,
    GX_TEXMTX2 = 36,
    GX_TEXMTX3 = 39,
    GX_TEXMTX4 = 42,
    GX_TEXMTX5 = 45,
    GX_TEXMTX6 = 48,
    GX_TEXMTX7 = 51,
    GX_TEXMTX8 = 54,
    GX_TEXMTX9 = 57,
    GX_IDENTITY = 60,
} GXTexMtx;

typedef enum _GXPTTexMtx {
    // 3x4 matrices (in dual-tex / "post-matrix" XF matrix memory)
    // Enum represents base row of matrix
    GX_PTTEXMTX0 = 64,
    GX_PTTEXMTX1 = 67,
    GX_PTTEXMTX2 = 70,
    GX_PTTEXMTX3 = 73,
    GX_PTTEXMTX4 = 76,
    GX_PTTEXMTX5 = 79,
    GX_PTTEXMTX6 = 82,
    GX_PTTEXMTX7 = 85,
    GX_PTTEXMTX8 = 88,
    GX_PTTEXMTX9 = 91,
    GX_PTTEXMTX10 = 94,
    GX_PTTEXMTX11 = 97,
    GX_PTTEXMTX12 = 100,
    GX_PTTEXMTX13 = 103,
    GX_PTTEXMTX14 = 106,
    GX_PTTEXMTX15 = 109,
    GX_PTTEXMTX16 = 112,
    GX_PTTEXMTX17 = 115,
    GX_PTTEXMTX18 = 118,
    GX_PTTEXMTX19 = 121,
    GX_PTIDENTITY = 125
} GXPTTexMtx;

typedef enum _GXTexWrapMode {
    GX_CLAMP,
    GX_REPEAT,
    GX_MIRROR,

    GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

typedef enum _GXTlut {
    GX_TLUT0,
    GX_TLUT1,
    GX_TLUT2,
    GX_TLUT3,
    GX_TLUT4,
    GX_TLUT5,
    GX_TLUT6,
    GX_TLUT7,
    GX_TLUT8,
    GX_TLUT9,
    GX_TLUT10,
    GX_TLUT11,
    GX_TLUT12,
    GX_TLUT13,
    GX_TLUT14,
    GX_TLUT15,

    GX_BIGTLUT0,
    GX_BIGTLUT1,
    GX_BIGTLUT2,
    GX_BIGTLUT3,
} GXTlut;

typedef enum _GXTlutFmt {
    GX_TL_IA8,
    GX_TL_RGB565,
    GX_TL_RGB5A3,

    GX_MAX_TLUTFMT
} GXTlutFmt;

typedef enum _GXVtxFmt {
    GX_VTXFMT0,
    GX_VTXFMT1,
    GX_VTXFMT2,
    GX_VTXFMT3,
    GX_VTXFMT4,
    GX_VTXFMT5,
    GX_VTXFMT6,
    GX_VTXFMT7,

    GX_MAX_VTXFMT
} GXVtxFmt;

typedef enum _GXZFmt16 {
    GX_ZC_LINEAR,
    GX_ZC_NEAR,
    GX_ZC_MID,
    GX_ZC_FAR,
} GXZFmt16;

// From patent
typedef enum _GXZTexOp {
    GX_ZT_DISABLE,
    GX_ZT_ADD,
    GX_ZT_REPLACE,

    GX_MAX_ZTEXOP
} GXZTexOp;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void OSFatal(GXColor textColor, GXColor bgColor, const char* msg);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
#ifndef RVL_SDK_OS_FONT_H
#define RVL_SDK_OS_FONT_H
/* "libs/RVL_SDK/include/revolution/OS/OSFont.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OS_FONT_ENCODE_ANSI,
    OS_FONT_ENCODE_SJIS,
    OS_FONT_ENCODE_2,
    OS_FONT_ENCODE_UTF8,
    OS_FONT_ENCODE_UTF16,
    OS_FONT_ENCODE_UTF32,
    OS_FONT_ENCODE_MAX
} OSFontEncode;

typedef struct OSFontHeader {
    u16 type;          // at 0x0
    u16 firstChar;     // at 0x2
    u16 lastChar;      // at 0x4
    u16 invalidChar;   // at 0x6
    u16 ascent;        // at 0x8
    u16 descent;       // at 0xA
    u16 width;         // at 0xC
    u16 leading;       // at 0xE
    u16 cellWidth;     // at 0x10
    u16 cellHeight;    // at 0x12
    u32 sheetSize;     // at 0x14
    u16 sheetFormat;   // at 0x18
    u16 sheetNumCol;   // at 0x1A
    u16 sheetNumRow;   // at 0x1C
    u16 sheetWidth;    // at 0x1E
    u16 sheetHeight;   // at 0x20
    u16 widthTableOfs; // at 0x22
    u32 sheetImageOfs; // at 0x24
    u32 sheetFullSize; // at 0x28
    u8 c0;             // at 0x2C
    u8 c1;             // at 0x2D
    u8 c2;             // at 0x2E
    u8 c3;             // at 0x2F
} OSFontHeader;

u16 OSGetFontEncode(void);
u16 OSSetFontEncode(u16 encode);
u32 OSLoadFont(OSFontHeader* font, void* dst);
const char* OSGetFontTexel(const char* str, void* dst, s32 xOfs, s32 arg3,
                           u32* widthOut);
BOOL OSInitFont(OSFontHeader* font);
const char* OSGetFontTexture(const char* str, void** texOut, u32* xOut,
                             u32* yOut, u32* widthOut);
const char* OSGetFontWidth(const char* str, u32* widthOut);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
#ifndef RVL_SDK_DVD_H
#define RVL_SDK_DVD_H
/* "libs/RVL_SDK/include/revolution/DVD/dvd.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// OS sets MSB to signal that the device code was successfully read
#define DVD_DEVICE_CODE_READ (1 << 15)
#define MAKE_DVD_DEVICE_CODE(x) (DVD_DEVICE_CODE_READ | (x))

#define H3_HASH_TABLE_SIZE 0x18000

// Forward declarations
typedef struct DVDCommandBlock;
typedef struct DVDFileInfo;
typedef struct OSAlarm;

typedef enum {
    DVD_RESULT_COVER_CLOSED = -4,
    DVD_RESULT_CANCELED,
    DVD_RESULT_IGNORED,
    DVD_RESULT_FATAL_ERROR,
    DVD_RESULT_GOOD,
} DVDResult;

typedef enum {
    DVD_STATE_FATAL = -1,
    DVD_STATE_IDLE,
    DVD_STATE_BUSY,
    DVD_STATE_WAITING,
    DVD_STATE_COVER_CLOSED,
    DVD_STATE_NO_DISK,
    DVD_STATE_COVER_OPENED,
    DVD_STATE_WRONG_DISK_ID,
    DVD_STATE_7,
    DVD_STATE_PAUSED,
    DVD_STATE_9,
    DVD_STATE_CANCELED,
    DVD_STATE_DISK_ERROR,
    DVD_STATE_MOTOR_STOPPED,
} DVDAsyncState;

typedef enum {
    DVD_COVER_BUSY,
    DVD_COVER_OPENED,
    DVD_COVER_CLOSED,
} DVDCoverState;

typedef void (*DVDAsyncCallback)(s32 result, struct DVDFileInfo* info);
typedef void (*DVDCommandCallback)(s32 result, struct DVDCommandBlock* block);

typedef struct DVDDiskID {
    char game[4];    // at 0x0
    char company[2]; // at 0x4
    u8 disk;         // at 0x6
    u8 version;      // at 0x7
    u8 strmEnable;   // at 0x8
    u8 strmBufSize;  // at 0x9
    u8 padding[14];  // at 0xA
    u32 rvlMagic;    // at 0x18
    u32 gcMagic;     // at 0x1C
} DVDDiskID;

typedef struct DVDCommandBlock {
    struct DVDCommandBlock* next;       // at 0x0
    struct DVDCommandBlock* prev;       // at 0x4
    u32 command;                 // at 0x8
    volatile s32 state;          // at 0xC
    u32 offset;                  // at 0x10
    u32 length;                  // at 0x14
    void* addr;                  // at 0x18
    u32 transferSize;            // at 0x1C
    u32 transferTotal;           // at 0x20
    DVDDiskID* id;               // at 0x24
    DVDCommandCallback callback; // at 0x28
    void* userData;              // at 0x2C
} DVDCommandBlock;

typedef struct DVDDriveInfo {
    u16 revision;    // at 0x0
    u16 deviceCode;  // at 0x2
    u32 releaseDate; // at 0x4
    char padding[32 - 0x8];
} DVDDriveInfo;

typedef struct DVDFileInfo {
    DVDCommandBlock block;     // at 0x0
    u32 offset;                // at 0x30
    u32 size;                  // at 0x34
    DVDAsyncCallback callback; // at 0x38
} DVDFileInfo;

typedef struct DVDDir{
    u32 entryNum; // at 0x0
    u32 location; // at 0x4
    u32 next;     // at 0x8
} DVDDir;

typedef struct DVDDirEntry{
    u32 entryNum; // at 0x0
    BOOL isDir;   // at 0x4
    char* name;   // at 0x8
} DVDDirEntry;

extern volatile u32 __DVDLayoutFormat;

void DVDInit(void);
BOOL DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* dst, u32 size,
                         u32 offset, DVDCommandCallback callback, s32 prio);
BOOL DVDInquiryAsync(DVDCommandBlock* block, DVDDriveInfo* info,
                     DVDCommandCallback callback);
s32 DVDGetCommandBlockStatus(const DVDCommandBlock* block);
s32 DVDGetDriveStatus(void);
void DVDPause(void);
void DVDResume(void);
BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCommandCallback callback);
s32 DVDCancel(DVDCommandBlock* block);
BOOL DVDCancelAllAsync(DVDCommandCallback callback);
const DVDDiskID* DVDGetCurrentDiskID(void);
u32 __DVDGetCoverStatus(void);
void __DVDPrepareResetAsync(DVDCommandCallback callback);
void __DVDPrepareReset(void);
BOOL __DVDTestAlarm(const struct OSAlarm* alarm);
BOOL __DVDLowBreak(void);
BOOL __DVDStopMotorAsync(DVDCommandBlock* block, DVDCommandCallback callback);
void __DVDRestartMotor(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
#ifndef RVL_SDK_OS_THREAD_H
#define RVL_SDK_OS_THREAD_H
/* "libs/RVL_SDK/include/revolution/OS/OSThread.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSThread.h" line 4 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_PRIORITY_MIN 0
#define OS_PRIORITY_MAX 31

#define OS_THREAD_STACK_MAGIC 0xDEADBABE

typedef enum {
    OS_THREAD_STATE_EXITED = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_RUNNING = 2,
    OS_THREAD_STATE_SLEEPING = 4,
    OS_THREAD_STATE_MORIBUND = 8
} OSThreadState;

typedef enum { OS_THREAD_DETACHED = (1 << 0) } OSThreadFlag;

typedef struct OSThreadQueue {
    struct OSThread* head; // at 0x0
    struct OSThread* tail; // at 0x4
} OSThreadQueue;

typedef struct OSMutexQueue {
    struct OSMutex* head; // at 0x0
    struct OSMutex* tail; // at 0x4
} OSMutexQueue;

typedef struct OSThread {
    OSContext context;
    u16 state;                   // at 0x2C8
    u16 flags;                   // at 0x2CA
    s32 suspend;                 // at 0x2CC
    s32 priority;                // at 0x2D0
    s32 base;                    // at 0x2D4
    u32 val;                     // at 0x2D8
    OSThreadQueue* queue;        // at 0x2DC
    struct OSThread* next;       // at 0x2E0
    struct OSThread* prev;       // at 0x2E4
    OSThreadQueue joinQueue;     // at 0x2E8
    struct OSMutex* mutex;       // at 0x2F0
    OSMutexQueue mutexQueue;     // at 0x2F4
    struct OSThread* nextActive; // at 0x2FC
    struct OSThread* prevActive; // at 0x300
    u32* stackBegin;             // at 0x304
    u32* stackEnd;               // at 0x308
    s32 error;                   // at 0x30C
    void* specific[2];           // at 0x310
} OSThread;

typedef void (*OSSwitchThreadCallback)(OSThread* currThread,
                                       OSThread* newThread);
typedef void* (*OSThreadFunc)(void* arg);

OSSwitchThreadCallback
OSSetSwitchThreadCallback(OSSwitchThreadCallback callback);
void __OSThreadInit(void);
void OSSetCurrentThread(OSThread* thread);
void OSInitMutexQueue(OSMutexQueue* queue);
void OSInitThreadQueue(OSThreadQueue* queue);
OSThread* OSGetCurrentThread(void);
BOOL OSIsThreadTerminated(OSThread* thread);
s32 OSDisableScheduler(void);
s32 OSEnableScheduler(void);
s32 __OSGetEffectivePriority(OSThread* thread);
void __OSPromoteThread(OSThread* thread, s32 prio);
void __OSReschedule(void);
void OSYieldThread(void);
BOOL OSCreateThread(OSThread* thread, OSThreadFunc func, void* funcArg,
                    void* stackBegin, u32 stackSize, s32 prio, u16 flags);
void OSExitThread(OSThread* thread);
void OSCancelThread(OSThread* thread);
BOOL OSJoinThread(OSThread* thread, void* val);
void OSDetachThread(OSThread* thread);
s32 OSResumeThread(OSThread* thread);
s32 OSSuspendThread(OSThread* thread);
void OSSleepThread(OSThreadQueue* queue);
void OSWakeupThread(OSThreadQueue* queue);
BOOL OSSetThreadPriority(OSThread* thread, s32 prio);
s32 OSGetThreadPriority(OSThread* thread);
void OSClearStack(u8 val);
void OSSleepTicks(s64 ticks);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
#ifndef RVL_SDK_OS_INTERRUPT_H
#define RVL_SDK_OS_INTERRUPT_H
/* "libs/RVL_SDK/include/revolution/OS/OSInterrupt.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

// Create mask from interrupt ID
#define OS_INTR_MASK(intr) (1 << (31 - intr))

typedef enum {
    OS_INTR_MEM_0,
    OS_INTR_MEM_1,
    OS_INTR_MEM_2,
    OS_INTR_MEM_3,
    OS_INTR_MEM_ADDRESS,
    OS_INTR_DSP_AI,
    OS_INTR_DSP_ARAM,
    OS_INTR_DSP_DSP,
    OS_INTR_AI_AI,
    OS_INTR_EXI_0_EXI,
    OS_INTR_EXI_0_TC,
    OS_INTR_EXI_0_EXT,
    OS_INTR_EXI_1_EXI,
    OS_INTR_EXI_1_TC,
    OS_INTR_EXI_1_EXT,
    OS_INTR_EXI_2_EXI,
    OS_INTR_EXI_2_TC,
    OS_INTR_PI_CP,
    OS_INTR_PI_PE_TOKEN,
    OS_INTR_PI_PE_FINISH,
    OS_INTR_PI_SI,
    OS_INTR_PI_DI,
    OS_INTR_PI_RSW,
    OS_INTR_PI_ERROR,
    OS_INTR_PI_VI,
    OS_INTR_PI_DEBUG,
    OS_INTR_PI_HSP,
    OS_INTR_PI_ACR,
    OS_INTR_28,
    OS_INTR_29,
    OS_INTR_30,
    OS_INTR_31,

    OS_INTR_MAX
} OSInterruptType;

//TODO: is this s16 or s32???
typedef void (*OSInterruptHandler)(s32 intr, struct OSContext* ctx);

extern u32 __OSLastInterruptSrr0;
extern s16 __OSLastInterrupt;
extern s64 __OSLastInterruptTime;

BOOL OSDisableInterrupts(void);
BOOL OSEnableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL status);

OSInterruptHandler __OSSetInterruptHandler(OSInterruptType type,
                                           OSInterruptHandler handler);
OSInterruptHandler __OSGetInterruptHandler(OSInterruptType type);

void __OSInterruptInit(void);

u32 __OSMaskInterrupts(u32 userMask);
u32 __OSUnmaskInterrupts(u32 userMask);
void __OSDispatchInterrupt(u8 intr, struct OSContext* ctx);

void __RAS_OSDisableInterrupts_begin(void);
void __RAS_OSDisableInterrupts_end(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
#ifndef RVL_SDK_OS_IPC_H
#define RVL_SDK_OS_IPC_H
/* "libs/RVL_SDK/include/revolution/OS/OSIpc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void* __OSGetIPCBufferHi(void);
void* __OSGetIPCBufferLo(void);
void __OSInitIPCBuffer(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
#ifndef RVL_SDK_OS_LINK_H
#define RVL_SDK_OS_LINK_H
/* "libs/RVL_SDK/include/revolution/OS/OSLink.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSModuleInit(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
#ifndef RVL_SDK_OS_MEMORY_H
#define RVL_SDK_OS_MEMORY_H
/* "libs/RVL_SDK/include/revolution/OS/OSMemory.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define OS_MEM_KB_TO_B(mb) ((mb) * 1024)
#define OS_MEM_B_TO_KB(mb) ((mb) / 1024)

#define OS_MEM_MB_TO_B(mb) ((mb) * 1024 * 1024)
#define OS_MEM_B_TO_MB(mb) ((mb) / 1024 / 1024)

#define OSIsMEM1Region(addr) (((u32)(addr) & 0x30000000) == 0)
#define OSIsMEM2Region(addr) (((u32)(addr) & 0x30000000) == 0x10000000)

#define OS_IS_MEM1_REGION(addr) OSIsMEM1Region(addr)
#define OS_IS_MEM2_REGION(addr) OSIsMEM2Region(addr)

u32 OSGetPhysicalMem1Size(void);
u32 OSGetPhysicalMem2Size(void);
u32 OSGetConsoleSimulatedMem1Size(void);
u32 OSGetConsoleSimulatedMem2Size(void);
void __OSRestoreCodeExecOnMEM1(u32 size);
void __OSInitMemoryProtection(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
#ifndef RVL_SDK_OS_MESSAGE_H
#define RVL_SDK_OS_MESSAGE_H
/* "libs/RVL_SDK/include/revolution/OS/OSMessage.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSMessage.h" line 4 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// General-purpose typedef
typedef void* OSMessage;

typedef enum { OS_MSG_BLOCKING = (1 << 0) } OSMessageFlags;

typedef struct OSMessageQueue {
    OSThreadQueue sendQueue; // at 0x0
    OSThreadQueue recvQueue; // at 0x8
    OSMessage* buffer;       // at 0x10
    s32 capacity;            // at 0x14
    s32 front;               // at 0x18
    s32 size;                // at 0x1C
} OSMessageQueue;

void OSInitMessageQueue(OSMessageQueue* queue, OSMessage* buffer, s32 capacity);
BOOL OSSendMessage(OSMessageQueue* queue, OSMessage mesg, u32 flags);
BOOL OSReceiveMessage(OSMessageQueue* queue, OSMessage* mesg, u32 flags);
BOOL OSJamMessage(OSMessageQueue* queue, OSMessage mesg, u32 flags);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
#ifndef RVL_SDK_OS_MUTEX_H
#define RVL_SDK_OS_MUTEX_H
/* "libs/RVL_SDK/include/revolution/OS/OSMutex.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSMutex.h" line 4 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSMutex {
    OSThreadQueue queue;  // at 0x0
    OSThread* thread;     // at 0x8
    s32 lock;             // at 0xC
    struct OSMutex* next; // at 0x10
    struct OSMutex* prev; // at 0x14
} OSMutex;

void OSInitMutex(OSMutex* mutex);
void OSLockMutex(OSMutex* mutex);
void OSUnlockMutex(OSMutex* mutex);
void __OSUnlockAllMutex(OSThread* thread);
BOOL OSTryLockMutex(OSMutex* mutex);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
#ifndef RVL_SDK_OS_NET_H
#define RVL_SDK_OS_NET_H
/* "libs/RVL_SDK/include/revolution/OS/OSNet.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitNet(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
#ifndef RVL_SDK_OS_PLAY_RECORD_H
#define RVL_SDK_OS_PLAY_RECORD_H
/* "libs/RVL_SDK/include/revolution/OS/OSPlayRecord.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSStartPlayRecord(void);
void __OSStopPlayRecord(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
#ifndef RVL_SDK_OS_PLAY_TIME_H
#define RVL_SDK_OS_PLAY_TIME_H
/* "libs/RVL_SDK/include/revolution/OS/OSPlayTime.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitPlayTime(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
#ifndef RVL_SDK_OS_RESET_H
#define RVL_SDK_OS_RESET_H
/* "libs/RVL_SDK/include/revolution/OS/OSReset.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (*OSShutdownFunction)(BOOL final, u32 event);

typedef enum {
    OS_SD_EVENT_FATAL,
    OS_SD_EVENT_1,
    OS_SD_EVENT_SHUTDOWN,
    OS_SD_EVENT_3,
    OS_SD_EVENT_RESTART,
    OS_SD_EVENT_RETURN_TO_MENU,
    OS_SD_EVENT_LAUNCH_APP,
} OSShutdownEvent;

typedef struct OSShutdownFunctionInfo {
    OSShutdownFunction func;             // at 0x0
    u32 prio;                            // at 0x4
    struct OSShutdownFunctionInfo* next; // at 0x8
    struct OSShutdownFunctionInfo* prev; // at 0xC
} OSShutdownFunctionInfo;

typedef struct OSShutdownFunctionQueue {
    OSShutdownFunctionInfo* head; // at 0x0
    OSShutdownFunctionInfo* tail; // at 0x4
} OSShutdownFunctionQueue;

void OSRegisterShutdownFunction(OSShutdownFunctionInfo* info);
BOOL __OSCallShutdownFunctions(u32 pass, u32 event);
void __OSShutdownDevices(u32 event);
void __OSGetDiscState(u8* out);
void OSShutdownSystem(void);
void OSRestart(u32 resetCode);
void OSReturnToMenu(void);
u32 OSGetResetCode(void);
void OSResetSystem(BOOL reset, u32 resetCode, BOOL forceMenu);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
#ifndef RVL_SDK_OS_RTC_H
#define RVL_SDK_OS_RTC_H
/* "libs/RVL_SDK/include/revolution/OS/OSRtc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSSram {
    u16 checksum;    // at 0x0
    u16 invchecksum; // at 0x2
    u32 ead0;        // at 0x4
    u32 ead1;        // at 0x8
    u32 counterBias; // at 0xC
    u8 dispOfsH;     // at 0x10
    u8 ntd;          // at 0x11
    u8 lang;         // at 0x12
    u8 flags;        // at 0x13
} OSSram;

typedef struct OSSramEx {
    char UNK_0x0[0x1C];
    u16 wirelessPadId[4]; // at 0x1C
    char UNK_0x38[0x3C - 0x38];
    u16 gbs; // at 0x3C
    char UNK_0x3E[0x40 - 0x3E];
} OSSramEx;

void __OSInitSram(void);
OSSramEx* __OSLockSramEx(void);
BOOL __OSUnlockSramEx(BOOL save);
BOOL __OSSyncSram(void);
BOOL __OSReadROM(void* dst, s32 size, const void* src);
u16 OSGetWirelessID(s32 pad);
void OSSetWirelessID(s32 pad, u16 id);
u16 OSGetGbsMode(void);
void OSSetGbsMode(u16 gbs);
BOOL __OSGetRTCFlags(u32* out);
BOOL __OSClearRTCFlags(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
#ifndef RVL_SDK_OS_SERIAL
#define RVK_SDK_OS_SERIAL

/* "libs/RVL_SDK/include/revolution/OS/OSSerial.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif

#define SI_MAX_COMCSR_INLNGTH 128
#define SI_MAX_COMCSR_OUTLNGTH 128
#define SI_ERROR_UNDER_RUN 0x0001
#define SI_ERROR_OVER_RUN 0x0002
#define SI_ERROR_COLLISION 0x0004
#define SI_ERROR_NO_RESPONSE 0x0008
#define SI_ERROR_WRST 0x0010
#define SI_ERROR_RDST 0x0020
#define SI_ERROR_UNKNOWN 0x0040
#define SI_CHAN0 0
#define SI_CHAN1 1
#define SI_CHAN2 2
#define SI_CHAN3 3
#define SI_CHAN0_BIT 0x80000000
#define SI_CHAN1_BIT 0x40000000
#define SI_CHAN2_BIT 0x20000000
#define SI_CHAN3_BIT 0x10000000
#define SI_CHAN_BIT(chan) (SI_CHAN0_BIT >> (chan))
#define SI_TYPE_MASK 0x18000000u
#define SI_TYPE_N64 0x00000000u
#define SI_TYPE_DOLPHIN 0x08000000u
#define SI_TYPE_GC SI_TYPE_DOLPHIN
#define SI_GC_WIRELESS 0x80000000
#define SI_GC_NOMOTOR 0x20000000
#define SI_GC_STANDARD 0x01000000
#define SI_WIRELESS_RECEIVED 0x40000000
#define SI_WIRELESS_IR 0x04000000
#define SI_WIRELESS_STATE 0x02000000
#define SI_WIRELESS_ORIGIN 0x00200000
#define SI_WIRELESS_FIX_ID 0x00100000
#define SI_WIRELESS_TYPE 0x000f0000
#define SI_WIRELESS_LITE_MASK 0x000c0000
#define SI_WIRELESS_CONT_MASK 0x00080000
#define SI_WIRELESS_ID 0x00c0ff00
#define SI_WIRELESS_TYPE_ID (SI_WIRELESS_TYPE | SI_WIRELESS_ID)
#define SI_N64_CONTROLLER (SI_TYPE_N64 | 0x05000000)
#define SI_N64_MIC (SI_TYPE_N64 | 0x00010000)
#define SI_N64_KEYBOARD (SI_TYPE_N64 | 0x00020000)
#define SI_N64_MOUSE (SI_TYPE_N64 | 0x02000000)
#define SI_GBA (SI_TYPE_N64 | 0x00040000)
#define SI_GC_CONTROLLER (SI_TYPE_GC | SI_GC_STANDARD)
#define SI_GC_RECEIVER (SI_TYPE_GC | SI_GC_WIRELESS)
#define SI_GC_WAVEBIRD                                                                             \
  (SI_TYPE_GC | SI_GC_WIRELESS | SI_GC_STANDARD | SI_WIRELESS_STATE | SI_WIRELESS_FIX_ID)
#define SI_GC_KEYBOARD (SI_TYPE_GC | 0x00200000)
#define SI_GC_STEERING (SI_TYPE_GC | 0x00000000)

u32 SIProbe(s32 chan);
char* SIGetTypeString(u32 type);
void SIRefreshSamplingRate(void);
void SISetSamplingRate(u32 msec);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
#ifndef RVL_SDK_OS_STATE_FLAGS_H
#define RVL_SDK_OS_STATE_FLAGS_H
/* "libs/RVL_SDK/include/revolution/OS/OSStateFlags.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSStateFlags {
    u32 checksum; // at 0x0
    u8 BYTE_0x4;
    u8 BYTE_0x5;  // at 0x5
    u8 discState; // at 0x6
    u8 BYTE_0x7;
    u32 WORD_0x8;
    u32 WORD_0xC;
    u32 WORD_0x10;
    u32 WORD_0x14;
    u32 WORD_0x18;
    u32 WORD_0x1C;
} OSStateFlags;

BOOL __OSWriteStateFlags(const OSStateFlags* state);
BOOL __OSReadStateFlags(OSStateFlags* state);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
#ifndef RVL_SDK_OS_STATETM_H
#define RVL_SDK_OS_STATETM_H
/* "libs/RVL_SDK/include/revolution/OS/OSStateTM.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*OSStateCallback)(void);

OSStateCallback OSSetResetCallback(OSStateCallback callback);
OSStateCallback OSSetPowerCallback(OSStateCallback callback);
BOOL __OSInitSTM(void);
void __OSShutdownToSBY(void);
void __OSHotReset(void);
BOOL __OSGetResetButtonStateRaw(void);
s32 __OSSetVIForceDimming(u32 arg0, u32 arg1, u32 arg2);
s32 __OSSetIdleLEDMode(u32 mode);
s32 __OSUnRegisterStateEvent(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
#ifndef RVL_SDK_OS_SYNC_H
#define RVL_SDK_OS_SYNC_H
/* "libs/RVL_SDK/include/revolution/OS/OSSync.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __OSInitSystemCall(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
#ifndef RVL_SDK_OS_TIME_H
#define RVL_SDK_OS_TIME_H
/* "libs/RVL_SDK/include/revolution/OS/OSTime.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSTime.h" line 4 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Time base frequency = 1/4 bus clock
#define OS_TIME_SPEED (OS_BUS_CLOCK_SPEED / 4)

// OS time -> Real time
#define OS_TICKS_TO_SEC(x) ((x) / (OS_TIME_SPEED))
#define OS_TICKS_TO_MSEC(x) ((x) / (OS_TIME_SPEED / 1000))
#define OS_TICKS_TO_USEC(x) (((x) * 8) / (OS_TIME_SPEED / 125000))
#define OS_TICKS_TO_NSEC(x) (((x) * 8000) / (OS_TIME_SPEED / 125000))

// Real time -> OS time
#define OS_SEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED))
#define OS_MSEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 1000))
#define OS_USEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 125000) / 8)
#define OS_NSEC_TO_TICKS(x) ((x) * (OS_TIME_SPEED / 125000) / 8000)

// Interpret as signed to find tick delta
#define OS_TICKS_DELTA(x, y) ((s32)x - (s32)y)

typedef struct OSCalendarTime {
    s32 sec;   // at 0x0
    s32 min;   // at 0x4
    s32 hour;  // at 0x8
    s32 mday;  // at 0xC
    s32 month; // at 0x10
    s32 year;  // at 0x14
    s32 wday;  // at 0x18
    s32 yday;  // at 0x1C
    s32 msec;  // at 0x20
    s32 usec;  // at 0x24
} OSCalendarTime;

s64 OSGetTime(void);
u32 OSGetTick(void);

s64 __OSGetSystemTime(void);
s64 __OSTimeToSystemTime(s64 time);

void OSTicksToCalendarTime(s64 time, OSCalendarTime* cal);
s64 OSCalendarTimeToTicks(const OSCalendarTime* cal);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
#ifndef RVL_SDK_OS_UTF_H
#define RVL_SDK_OS_UTF_H
/* "libs/RVL_SDK/include/revolution/OS/OSUtf.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

const u8* OSUTF8to32(const u8* utf8, u32* utf32);
const wchar_t* OSUTF16to32(const wchar_t* utf16, u32* utf32);
u8 OSUTF32toANSI(u32 utf32);
wchar_t OSUTF32toSJIS(u32 utf32);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
#ifndef RVL_SDK_OS_PPC_EABI_INIT_H
#define RVL_SDK_OS_PPC_EABI_INIT_H
/* "libs/RVL_SDK/include/revolution/OS/__ppc_eabi_init.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#pragma section ".init"
DECL_SECTION(".init") void __init_hardware(void);
DECL_SECTION(".init") void __flush_cache(void*, size_t);

void __init_user(void);
void __init_cpp(void);
void __fini_cpp(void);
DECL_WEAK void exit(void);
void _ExitProcess(void);

/**
 * Linker Generated Symbols
 */

// Declare linker symbols for a section in the ROM
#define DECL_ROM_SECTION(x)                                                    \
    extern u8 _f##x[];                                                         \
    extern u8 _f##x##_rom[];                                                   \
    extern u8 _e##x[];

// Declare linker symbols for a BSS section
#define DECL_BSS_SECTION(x)                                                    \
    extern u8 _f##x[];                                                         \
    extern u8 _e##x[];

// Debugger stack
extern u8 _db_stack_addr[];
extern u8 _db_stack_end[];

// Program arena
extern u8 __ArenaLo[];
extern u8 __ArenaHi[];

// Program stack
extern u8 _stack_addr[];
extern u8 _stack_end[];

// Small data bases
extern u8 _SDA_BASE_[];
extern u8 _SDA2_BASE_[];

// ROM sections
DECL_ROM_SECTION(_init);
DECL_ROM_SECTION(extab);
DECL_ROM_SECTION(extabindex);
DECL_ROM_SECTION(_text);
DECL_ROM_SECTION(_ctors);
DECL_ROM_SECTION(_dtors);
DECL_ROM_SECTION(_rodata);
DECL_ROM_SECTION(_data);
DECL_ROM_SECTION(_sdata);
DECL_ROM_SECTION(_sdata2);
DECL_ROM_SECTION(_stack);

// BSS sections
DECL_BSS_SECTION(_bss);
DECL_BSS_SECTION(_sbss);
DECL_BSS_SECTION(_sbss2);

typedef struct RomSection {
    void* phys;  // at 0x4
    void* virt;  // at 0x0
    size_t size; // at 0x8
} RomSection;

typedef struct BssSection {
    void* virt;  // at 0x0
    size_t size; // at 0x8
} BssSection;

typedef struct ExtabIndexInfo {
    void* section;                // at 0x0
    struct ExtabIndexInfo* extab; // at 0x4
    void* codeStart;              // at 0x8
    u32 codeSize;                 // at 0xC
} ExtabIndexInfo;

DECL_SECTION(".init") extern const RomSection _rom_copy_info[];
DECL_SECTION(".init") extern const BssSection _bss_init_info[];
DECL_SECTION(".init") extern const ExtabIndexInfo _eti_init_info[];

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */
/* "libs/monolib/include/monolib/util/CErrorWii.hpp" line 5 "revolution/VI.h" */
#ifndef RVL_SDK_PUBLIC_VI_H
#define RVL_SDK_PUBLIC_VI_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/VI.h" line 6 "revolution/VI/vi.h" */
#ifndef RVL_SDK_VI_H
#define RVL_SDK_VI_H
/* "libs/RVL_SDK/include/revolution/VI/vi.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/VI/vi.h" line 4 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
#ifndef RVL_SDK_GX_ATTR_H
#define RVL_SDK_GX_ATTR_H
/* "libs/RVL_SDK/include/revolution/GX/GXAttr.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXAttr.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXVtxDescList {
    GXAttr attr;     // at 0x0
    GXAttrType type; // at  0x4
} GXVtxDescList;

typedef struct _GXVtxAttrFmtList {
    GXAttr attr;         // at 0x0
    GXCompCnt compCnt;   // at 0x4
    GXCompType compType; // at 0x8
    u8 shift;            // at 0xC
} GXVtxAttrFmtList;

void GXSetVtxDesc(GXAttr name, GXAttrType type);
void GXSetVtxDescv(const GXVtxDescList* list);
void GXGetVtxDesc(GXAttr name, GXAttrType* type);
void GXGetVtxDescv(GXVtxDescList* list);
void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt compCnt,
                     GXCompType compType, u8 shift);

// TODO(kiwi) Please find a way to get rid of this
#if defined(GXATTR_MATCH_HACK)
void GXSetVtxAttrFmtv(s16 fmt, const GXVtxAttrFmtList* list);
#else
void GXSetVtxAttrFmtv(GXVtxFmt fmt, const GXVtxAttrFmtList* list);
#endif

void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt* compCnt,
                     GXCompType* compType, u8* shift);
void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList* list);
void GXSetArray(GXAttr attr, const void* base, u8 stride);
void GXInvalidateVtxCache(void);
void GXSetTexCoordGen2(GXTexCoordID id, GXTexGenType type, GXTexGenSrc src,
                       u32 texMtxIdx, GXBool normalize, u32 dualTexMtxIdx);
void GXSetNumTexGens(u8 num);

void __GXSetVCD(void);
void __GXCalculateVLim(void);
void __GXSetVAT(void);

static void GXSetTexCoordGen(GXTexCoordID id, GXTexGenType type,
                             GXTexGenSrc src, u32 texMtxIdx) {
    GXSetTexCoordGen2(id, type, src, texMtxIdx, FALSE, GX_PTIDENTITY);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
#ifndef RVL_SDK_GX_BUMP_H
#define RVL_SDK_GX_BUMP_H
/* "libs/RVL_SDK/include/revolution/GX/GXBump.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXBump.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void GXSetTevIndirect(GXTevStageID tevStage, GXIndTexStageID texStage,
                      GXIndTexFormat texFmt, GXIndTexBiasSel biasSel,
                      GXIndTexMtxID mtxId, GXIndTexWrap wrapS,
                      GXIndTexWrap wrapT, GXBool addPrev, GXBool utcLod,
                      GXIndTexAlphaSel alphaSel);
void GXSetIndTexMtx(GXIndTexMtxID id, const f32 offset[2][3], s8 scaleExp);
void GXSetIndTexCoordScale(GXIndTexStageID stage, GXIndTexScale scaleS,
                           GXIndTexScale scaleT);
void GXSetIndTexOrder(GXIndTexStageID stage, GXTexCoordID coord,
                      GXTexMapID map);
void GXSetNumIndStages(u8 num);
void GXSetTevDirect(GXTevStageID stage);

void GXSetTevIndWarp(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
GXBool signed_offsets, GXBool replace_mode, GXIndTexMtxID matrix_sel);

void __GXUpdateBPMask(void);
void __GXSetIndirectMask(u32 mask);
void __GXFlushTextureState(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
#ifndef RVL_SDK_GX_DISPLAY_LIST_H
#define RVL_SDK_GX_DISPLAY_LIST_H
/* "libs/RVL_SDK/include/revolution/GX/GXDisplayList.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXDisplayList.h" line 4 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
#ifdef __cplusplus
extern "C" {
#endif

void GXBeginDisplayList(void* list, u32 size);
u32 GXEndDisplayList(void);
void GXCallDisplayList(void* list, u32 size);

static void GXFastCallDisplayList(void* list, u32 size) {
    WGPIPE.c = GX_FIFO_CMD_CALL_DL;
    WGPIPE.p = list;
    WGPIPE.ui = size;
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
#ifndef RVL_SDK_GX_DRAW_H
#define RVL_SDK_GX_DRAW_H
/* "libs/RVL_SDK/include/revolution/GX/GXDraw.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void GXDrawCylinder(u8 sides);
void GXDrawSphere(u32 stacks, u32 sectors);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
#ifndef RVL_SDK_GX_FIFO_H
#define RVL_SDK_GX_FIFO_H
/* "libs/RVL_SDK/include/revolution/GX/GXFifo.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXFifo.h" line 4 "revolution/GX/GXInternal.h" */
#ifndef RVL_SDK_GX_INTERNAL_H
#define RVL_SDK_GX_INTERNAL_H
/* "libs/RVL_SDK/include/revolution/GX/GXInternal.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXInternal.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * GX internal structures.
 *
 * Wouldn't be necessary if the public ones didn't include padding; but they do,
 * so there has to be different structure definitions.
 *
 * These internal structures are implemented like the RFL ones since we don't
 * have DWARF info for most GX structures.
 */

/**
 * Declare a public structure from the corresponding internal structure.
 * (Implementation size is included to require that such a structure already
 * exists.)
 */
#define GX_PUBLIC_STRUCT_DECL(name, size)                                      \
    typedef struct _##name {                                                   \
        u32 dummy[((size) - sizeof(name##Impl) + sizeof(name##Impl)) /         \
                  sizeof(u32)];                                                \
    } name;

typedef struct _GXFifoObjImpl {
    void* base;        // at 0x0
    void* end;         // at 0x4
    u32 size;          // at 0x8
    void* hiWatermark; // at 0xC
    void* loWatermark; // at 0x10
    void* readPtr;     // at 0x14
    void* writePtr;    // at 0x18
    u32 count;         // at 0x1C
    u8 wrap;           // at 0x20
} GXFifoObjImpl;

typedef struct _GXLightObjImpl {
    char UNK_0x0[0xC];
    GXColor color; // at 0xC
    f32 aa;        // at 0x10
    f32 ab;        // at 0x14
    f32 ac;        // at 0x18
    f32 ka;        // at 0x1C
    f32 kb;        // at 0x20
    f32 kc;        // at 0x24
    f32 posX;      // at 0x28
    f32 posY;      // at 0x2C
    f32 posZ;      // at 0x30
    f32 dirX;      // at 0x34
    f32 dirY;      // at 0x38
    f32 dirZ;      // at 0x3C
} GXLightObjImpl;

typedef struct _GXTexObjImpl {
    u8 todo;
} GXTexObjImpl;

typedef struct _GXTlutObjImpl {
    u8 todo;
} GXTlutObjImpl;

typedef struct _GXTexRegionImpl {
    u8 todo;
} GXTexRegionImpl;

typedef struct _GXTlutRegionImpl {
    u8 todo;
} GXTlutRegionImpl;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXInternal.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXFifo.h" line 6 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */
#ifdef __cplusplus
extern "C" {
#endif

GX_PUBLIC_STRUCT_DECL(GXFifoObj, 128);

typedef void (*GXBreakPtCallback)(void);

void GXGetGPStatus(GXBool* overhi, GXBool* underlow, GXBool* readIdle,
                   GXBool* cmdIdle, GXBool* brkpt);
void GXGetFifoStatus(GXFifoObj* fifo, GXBool* overhi, GXBool* underlow, u32* fifoCount,
                     GXBool* cpu_write, GXBool* gp_read, GXBool* fifowrap);

void GXSetCPUFifo(GXFifoObj*);
void GXSetGPFifo(GXFifoObj*);
GXBool GXGetCPUFifo(GXFifoObj*);
GXBool GXGetGPFifo(GXFifoObj*);
void GXSaveCPUFifo(GXFifoObj*);

void GXGetFifoPtrs(const GXFifoObj*, void** readPtr, void** writePtr);
void GXInitFifoBase(GXFifoObj* fifo, void* base, u32 size);
void GXInitFifoLimits(GXFifoObj* fifo, u32 hiWaterMark, u32 loWaterMark);
GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb);
void GXEnableBreakPt(void* breakPt);
void GXDisableBreakPt(void);

u32 GXGetFifoCount(GXFifoObj*);
u8 GXGetFifoWrap(GXFifoObj*);

void GXEnableBreakPt(void* writePtr);
void GXDisableBreakPt(void);

OSThread* GXGetCurrentGXThread(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
#ifndef RVL_SDK_GX_FRAMEBUF_H
#define RVL_SDK_GX_FRAMEBUF_H
/* "libs/RVL_SDK/include/revolution/GX/GXFrameBuf.h" line 2 "types.h" */
/* end "types.h" */
/* "libs/RVL_SDK/include/revolution/GX/GXFrameBuf.h" line 3 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX/GXFrameBuf.h" line 4 "revolution/VI/vitypes.h" */
#ifndef RVL_SDK_VI_TYPES
#define RVL_SDK_VI_TYPES

/* "libs/RVL_SDK/include/revolution/VI/vitypes.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif


typedef u8 VIBool;
#define VI_FALSE ((VIBool)0)
#define VI_TRUE ((VIBool)1)
#define VI_DISABLE ((VIBool)0)
#define VI_ENABLE ((VIBool)1)

// Merge format/scanmode to one value for TV mode (see GXRenderModeObj)
#define VI_TVMODE(format, scanMode) (((format) << 2) + (scanMode))

// Get TV format from TV mode
#define VI_TVMODE_FORMAT(mode) ((mode) >> 2)
// Get TV scan mode from TV mode
#define VI_TVMODE_SCANMODE(mode) ((mode) & 0b00000011)

typedef enum {
    VI_TVFORMAT_NTSC,
    VI_TVFORMAT_PAL,
    VI_TVFORMAT_MPAL,
    VI_TVFORMAT_DEBUG,
    VI_TVFORMAT_DEBUG_PAL,
    VI_TVFORMAT_EURGB60
} VITVFormat;

typedef enum VIScanMode {
    VI_SCANMODE_INT,
    VI_SCANMODE_DS,
    VI_SCANMODE_PROG
} VIScanMode;

typedef enum VIXFBMode {
    VI_XFBMODE_SF,
    VI_XFBMODE_DF,
} VIXFBMode;


typedef enum VITVMode {
    VI_TVMODE_NTSC_INT = VI_TVMODE(VI_TVFORMAT_NTSC, VI_SCANMODE_INT),
    VI_TVMODE_NTSC_DS = VI_TVMODE(VI_TVFORMAT_NTSC, VI_SCANMODE_DS),
    VI_TVMODE_NTSC_PROG = VI_TVMODE(VI_TVFORMAT_NTSC, VI_SCANMODE_PROG),

    VI_TVMODE_PAL_INT = VI_TVMODE(VI_TVFORMAT_PAL, VI_SCANMODE_INT),
    VI_TVMODE_PAL_DS = VI_TVMODE(VI_TVFORMAT_PAL, VI_SCANMODE_DS),

    VI_TVMODE_EURGB60_INT = VI_TVMODE(VI_TVFORMAT_EURGB60, VI_SCANMODE_INT),
    VI_TVMODE_EURGB60_DS = VI_TVMODE(VI_TVFORMAT_EURGB60, VI_SCANMODE_DS),
    VI_TVMODE_EURGB60_PROG = VI_TVMODE(VI_TVFORMAT_EURGB60, VI_SCANMODE_PROG),

    VI_TVMODE_MPAL_INT = VI_TVMODE(VI_TVFORMAT_MPAL, VI_SCANMODE_INT),
    VI_TVMODE_MPAL_DS = VI_TVMODE(VI_TVFORMAT_MPAL, VI_SCANMODE_DS),
    VI_TVMODE_MPAL_PROG = VI_TVMODE(VI_TVFORMAT_MPAL, VI_SCANMODE_PROG),

    VI_TVMODE_DEBUG_INT = VI_TVMODE(VI_TVFORMAT_DEBUG, VI_SCANMODE_INT),

    VI_TVMODE_DEBUG_PAL_INT = VI_TVMODE(VI_TVFORMAT_DEBUG_PAL, VI_SCANMODE_INT),
    VI_TVMODE_DEBUG_PAL_DS = VI_TVMODE(VI_TVFORMAT_DEBUG_PAL, VI_SCANMODE_DS),
} VITVMode;

typedef enum _VITiming {
    VI_TMG_GAME = 0,
    VI_TMG_DVD  = 1
} VITiming;

typedef enum _VIVideo {
    VI_VMODE_NTSC = 0,
    VI_VMODE_MPAL = 1,
    VI_VMODE_PAL = 2,
    VI_VMODE_RGB = 3
} VIVideo;


typedef struct VIGammaObj {
    u16 a[6];
    u8 yin[7];
    u16 yout[7];
} VIGammaObj;

typedef enum _VITimeToDIM {
    VI_DM_DEFAULT = 0,
    VI_DM_10M,
    VI_DM_15M
} VITimeToDIM;

typedef enum _VIACPType {
    VI_ACP_OFF = 1,
    VI_ACP_TYPE1 = 2,
    VI_ACP_TYPE2 = 3,
    VI_ACP_TYPE3 = 4
} VIACPType;

typedef enum _VIGamma {
    VI_GM_0_1 = 1,
    VI_GM_0_2,
    VI_GM_0_3,
    VI_GM_0_4,
    VI_GM_0_5,
    VI_GM_0_6,
    VI_GM_0_7,
    VI_GM_0_8,
    VI_GM_0_9,
    VI_GM_1_0,
    VI_GM_1_1,
    VI_GM_1_2,
    VI_GM_1_3,
    VI_GM_1_4,
    VI_GM_1_5,
    VI_GM_1_6,
    VI_GM_1_7,
    VI_GM_1_8,
    VI_GM_1_9,
    VI_GM_2_0,
    VI_GM_2_1,
    VI_GM_2_2,
    VI_GM_2_3,
    VI_GM_2_4,
    VI_GM_2_5,
    VI_GM_2_6,
    VI_GM_2_7,
    VI_GM_2_8,
    VI_GM_2_9,
    VI_GM_3_0
} VIGamma;

typedef enum _VIOverDrive {
    VI_ODV_L1 = 0,
    VI_ODV_L2 = 1,
    VI_ODV_L3 = 2,
    VI_ODV_L4 = 3,
    VI_ODV_L5 = 4,
    VI_ODV_L6 = 5
} VIOverDrive;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/VI/vitypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define GX_VFILTER_SZ 7

typedef struct _GXRenderModeObj {
    VITVMode viTVmode;         // at 0x0
    u16 fbWidth;               // at 0x4
    u16 efbHeight;             // at 0x6
    u16 xfbHeight;             // at 0x8
    u16 viXOrigin;             // at 0xA
    u16 viYOrigin;             // at 0xC
    u16 viWidth;               // at 0xE
    u16 viHeight;              // at 0x10
    VIXFBMode xFBmode;         // at 0x14
    GXBool field_rendering;    // at 0x18
    GXBool aa;                 // at 0x19
    u8 sample_pattern[12][2];  // at 0x1A
    u8 vfilter[GX_VFILTER_SZ]; // at 0x32
} GXRenderModeObj;

extern GXRenderModeObj GXNtsc240Ds;
extern GXRenderModeObj GXNtsc480IntDf;
extern GXRenderModeObj GXNtsc480Int;
extern GXRenderModeObj GXNtsc480Prog;
extern GXRenderModeObj GXNtsc480ProgSoft;
extern GXRenderModeObj GXMpal240Ds;
extern GXRenderModeObj GXMpal480IntDf;
extern GXRenderModeObj GXMpal480Int;
extern GXRenderModeObj GXMpal480Prog;
extern GXRenderModeObj GXMpal480ProgSoft;
extern GXRenderModeObj GXPal264Ds;
extern GXRenderModeObj GXPal528IntDf;
extern GXRenderModeObj GXPal528Int;
extern GXRenderModeObj GXEurgb60Hz240Ds;
extern GXRenderModeObj GXEurgb60Hz480IntDf;
extern GXRenderModeObj GXEurgb60Hz480Int;
extern GXRenderModeObj GXEurgb60Hz480Prog;
extern GXRenderModeObj GXEurgb60Hz480ProgSoft;

void GXAdjustForOverscan(GXRenderModeObj* rmin, GXRenderModeObj* rmout,
                         u16 hor, u16 ver);

void GXSetDispCopySrc(u16 x, u16 y, u16 w, u16 h);
void GXSetTexCopySrc(u16 x, u16 y, u16 w, u16 h);

void GXSetDispCopyDst(u16 w, u16 numXfbLines);
void GXSetTexCopyDst(u16 w, u16 h, GXTexFmt fmt, GXBool mipmap);

void GXSetCopyClamp(GXCopyClamp clamp);

void GXSetDispCopyGamma(u32);

u32 GXGetNumXfbLines(u16 efbHeight, f32 scaleY);
f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight);
void GXSetDispCopyYScale(f32 scaleY);

void GXSetCopyClear(GXColor color, u32 z);
void GXSetCopyFilter(GXBool, const u8 sample_pattern[12][2], GXBool,
                     const u8 vfilter[GX_VFILTER_SZ]);

void GXCopyDisp(void*, GXBool);
void GXCopyTex(void*, GXBool);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
#ifndef RVL_SDK_GX_GEOMETRY_H
#define RVL_SDK_GX_GEOMETRY_H
/* "libs/RVL_SDK/include/revolution/GX/GXGeometry.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXGeometry.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void GXBegin(GXPrimitive prim, GXVtxFmt fmt, u16 verts);
static void GXEnd(void) {}

void GXSetLineWidth(u8 width, u32 offset);
void GXSetPointSize(u8 size, u32 offset);
void GXEnableTexOffsets(GXTexCoordID coordId, GXBool lineOfs, GXBool pointOfs);
void GXSetCullMode(GXCullMode mode);
void GXGetCullMode(GXCullMode* out);
void GXSetCoPlanar(GXBool coplanar);

void __GXSetDirtyState(void);
void __GXSendFlushPrim(void);
void __GXSetGenMode(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
#ifndef RVL_SDK_GX_HARDWARE_BP_H
#define RVL_SDK_GX_HARDWARE_BP_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardwareBP.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardwareBP.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

/******************************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 *****************************************************************************/

/**
 * BP registers
 */
typedef enum {
    GX_BP_REG_GENMODE = 0x0,
    GX_BP_REG_DISPCOPYFILTER0 = 0x1,
    GX_BP_REG_DISPCOPYFILTER1 = 0x2,
    GX_BP_REG_DISPCOPYFILTER2 = 0x3,
    GX_BP_REG_DISPCOPYFILTER3 = 0x4,
    GX_BP_REG_INDMTX0A = 0x6,
    GX_BP_REG_INDMTX0B = 0x7,
    GX_BP_REG_INDMTX0C = 0x8,
    GX_BP_REG_INDMTX1A = 0x9,
    GX_BP_REG_INDMTX1B = 0xA,
    GX_BP_REG_INDMTX1C = 0xB,
    GX_BP_REG_INDMTX2A = 0xC,
    GX_BP_REG_INDMTX2B = 0xD,
    GX_BP_REG_INDMTX2C = 0xE,
    GX_BP_REG_INDIMASK = 0xF,
    GX_BP_REG_INDTEVSTAGE0 = 0x10,
    GX_BP_REG_INDTEVSTAGE1 = 0x11,
    GX_BP_REG_INDTEVSTAGE2 = 0x12,
    GX_BP_REG_INDTEVSTAGE3 = 0x13,
    GX_BP_REG_INDTEVSTAGE4 = 0x14,
    GX_BP_REG_INDTEVSTAGE5 = 0x15,
    GX_BP_REG_INDTEVSTAGE6 = 0x16,
    GX_BP_REG_INDTEVSTAGE7 = 0x17,
    GX_BP_REG_INDTEVSTAGE8 = 0x18,
    GX_BP_REG_INDTEVSTAGE9 = 0x19,
    GX_BP_REG_INDTEVSTAGE10 = 0x1A,
    GX_BP_REG_INDTEVSTAGE11 = 0x1B,
    GX_BP_REG_INDTEVSTAGE12 = 0x1C,
    GX_BP_REG_INDTEVSTAGE13 = 0x1D,
    GX_BP_REG_INDTEVSTAGE14 = 0x1E,
    GX_BP_REG_INDTEVSTAGE15 = 0x1F,
    GX_BP_REG_SCISSORTL = 0x20,
    GX_BP_REG_SCISSORBR = 0x21,
    GX_BP_REG_LINEPTWIDTH = 0x22,
    GX_BP_REG_PERF0TRI = 0x23,
    GX_BP_REG_PERF0QUAD = 0x24,
    GX_BP_REG_RAS1_SS0 = 0x25,
    GX_BP_REG_RAS1_SS1 = 0x26,
    GX_BP_REG_RAS1_IREF = 0x27,
    GX_BP_REG_RAS1_TREF0 = 0x28,
    GX_BP_REG_RAS1_TREF1 = 0x29,
    GX_BP_REG_RAS1_TREF2 = 0x2A,
    GX_BP_REG_RAS1_TREF3 = 0x2B,
    GX_BP_REG_RAS1_TREF4 = 0x2C,
    GX_BP_REG_RAS1_TREF5 = 0x2D,
    GX_BP_REG_RAS1_TREF6 = 0x2E,
    GX_BP_REG_RAS1_TREF7 = 0x2F,
    GX_BP_REG_SU_SSIZE0 = 0x30,
    GX_BP_REG_SU_TSIZE0 = 0x31,
    GX_BP_REG_SU_SSIZE1 = 0x32,
    GX_BP_REG_SU_TSIZE1 = 0x33,
    GX_BP_REG_SU_SSIZE2 = 0x34,
    GX_BP_REG_SU_TSIZE2 = 0x35,
    GX_BP_REG_SU_SSIZE3 = 0x36,
    GX_BP_REG_SU_TSIZE3 = 0x37,
    GX_BP_REG_SU_SSIZE4 = 0x38,
    GX_BP_REG_SU_TSIZE4 = 0x39,
    GX_BP_REG_SU_SSIZE5 = 0x3A,
    GX_BP_REG_SU_TSIZE5 = 0x3B,
    GX_BP_REG_SU_SSIZE6 = 0x3C,
    GX_BP_REG_SU_TSIZE6 = 0x3D,
    GX_BP_REG_SU_SSIZE7 = 0x3E,
    GX_BP_REG_SU_TSIZE7 = 0x3F,
    GX_BP_REG_ZMODE = 0x40,
    GX_BP_REG_BLENDMODE = 0x41,
    GX_BP_REG_DSTALPHA = 0x42,
    GX_BP_REG_ZCONTROL = 0x43,
    GX_BP_REG_FIELDMASK = 0x44,
    GX_BP_REG_DRAWDONE = 0x45,
    GX_BP_REG_PETOKEN = 0x47,
    GX_BP_REG_PETOKENINT = 0x48,
    GX_BP_REG_TEXCOPYSRCXY = 0x49,
    GX_BP_REG_TEXCOPYSRCWH = 0x4A,
    GX_BP_REG_TEXCOPYDST = 0x4B,
    GX_BP_REG_DISPCOPYSTRIDE = 0x4D,
    GX_BP_REG_DISPCOPYSCALEY = 0x4E,
    GX_BP_REG_COPYCLEARAR = 0x4F,
    GX_BP_REG_COPYCLEARGB = 0x50,
    GX_BP_REG_COPYCLEARZ = 0x51,
    GX_BP_REG_COPYFILTER0 = 0x53,
    GX_BP_REG_COPYFILTER1 = 0x54,
    GX_BP_REG_BOUNDINGBOX0 = 0x55,
    GX_BP_REG_BOUNDINGBOX1 = 0x56,
    GX_BP_REG_SCISSOROFFSET = 0x59,
    GX_BP_REG_TMEMPRELOADADDR = 0x60,
    GX_BP_REG_TMEMPRELOADEVEN = 0x61,
    GX_BP_REG_TMEMPRELOADODD = 0x62,
    GX_BP_REG_TMEMPRELOADMODE = 0x63,
    GX_BP_REG_TMEMTLUTSRC = 0x64,
    GX_BP_REG_TMEMTLUTDST = 0x65,
    GX_BP_REG_TMEMTEXINVALIDATE = 0x66,
    GX_BP_REG_PERF1 = 0x67,
    GX_BP_REG_FIELDMODE = 0x68,
    GX_BP_REG_SETMODE0_TEX0 = 0x80,
    GX_BP_REG_SETMODE0_TEX1 = 0x81,
    GX_BP_REG_SETMODE0_TEX2 = 0x82,
    GX_BP_REG_SETMODE0_TEX3 = 0x83,
    GX_BP_REG_SETMODE1_TEX0 = 0x84,
    GX_BP_REG_SETMODE1_TEX1 = 0x85,
    GX_BP_REG_SETMODE1_TEX2 = 0x86,
    GX_BP_REG_SETMODE1_TEX3 = 0x87,
    GX_BP_REG_SETIMAGE0_TEX0 = 0x88,
    GX_BP_REG_SETIMAGE0_TEX1 = 0x89,
    GX_BP_REG_SETIMAGE0_TEX2 = 0x8A,
    GX_BP_REG_SETIMAGE0_TEX3 = 0x8B,
    GX_BP_REG_SETIMAGE1_TEX0 = 0x8C,
    GX_BP_REG_SETIMAGE1_TEX1 = 0x8D,
    GX_BP_REG_SETIMAGE1_TEX2 = 0x8E,
    GX_BP_REG_SETIMAGE1_TEX3 = 0x8F,
    GX_BP_REG_SETIMAGE2_TEX0 = 0x90,
    GX_BP_REG_SETIMAGE2_TEX1 = 0x91,
    GX_BP_REG_SETIMAGE2_TEX2 = 0x92,
    GX_BP_REG_SETIMAGE2_TEX3 = 0x93,
    GX_BP_REG_SETIMAGE3_TEX0 = 0x94,
    GX_BP_REG_SETIMAGE3_TEX1 = 0x95,
    GX_BP_REG_SETIMAGE3_TEX2 = 0x96,
    GX_BP_REG_SETIMAGE3_TEX3 = 0x97,
    GX_BP_REG_SETTLUT_TEX0 = 0x98,
    GX_BP_REG_SETTLUT_TEX1 = 0x99,
    GX_BP_REG_SETTLUT_TEX2 = 0x9A,
    GX_BP_REG_SETTLUT_TEX3 = 0x9B,
    GX_BP_REG_SETMODE0_TEX4 = 0xA0,
    GX_BP_REG_SETMODE0_TEX5 = 0xA1,
    GX_BP_REG_SETMODE0_TEX6 = 0xA2,
    GX_BP_REG_SETMODE0_TEX7 = 0xA3,
    GX_BP_REG_SETMODE1_TEX4 = 0xA4,
    GX_BP_REG_SETMODE1_TEX5 = 0xA5,
    GX_BP_REG_SETMODE1_TEX6 = 0xA6,
    GX_BP_REG_SETMODE1_TEX7 = 0xA7,
    GX_BP_REG_SETIMAGE0_TEX4 = 0xA8,
    GX_BP_REG_SETIMAGE0_TEX5 = 0xA9,
    GX_BP_REG_SETIMAGE0_TEX6 = 0xAA,
    GX_BP_REG_SETIMAGE0_TEX7 = 0xAB,
    GX_BP_REG_SETIMAGE1_TEX4 = 0xAC,
    GX_BP_REG_SETIMAGE1_TEX5 = 0xAD,
    GX_BP_REG_SETIMAGE1_TEX6 = 0xAE,
    GX_BP_REG_SETIMAGE1_TEX7 = 0xAF,
    GX_BP_REG_SETIMAGE2_TEX4 = 0xB0,
    GX_BP_REG_SETIMAGE2_TEX5 = 0xB1,
    GX_BP_REG_SETIMAGE2_TEX6 = 0xB2,
    GX_BP_REG_SETIMAGE2_TEX7 = 0xB3,
    GX_BP_REG_SETIMAGE3_TEX4 = 0xB4,
    GX_BP_REG_SETIMAGE3_TEX5 = 0xB5,
    GX_BP_REG_SETIMAGE3_TEX6 = 0xB6,
    GX_BP_REG_SETIMAGE3_TEX7 = 0xB7,
    GX_BP_REG_SETTLUT_TEX4 = 0xB8,
    GX_BP_REG_SETTLUT_TEX5 = 0xB9,
    GX_BP_REG_SETTLUT_TEX6 = 0xBA,
    GX_BP_REG_SETTLUT_TEX7 = 0xBB,
    GX_BP_REG_TEVCOLORCOMBINER0 = 0xC0,
    GX_BP_REG_TEVALPHACOMBINER0 = 0xC1,
    GX_BP_REG_TEVCOLORCOMBINER1 = 0xC2,
    GX_BP_REG_TEVALPHACOMBINER1 = 0xC3,
    GX_BP_REG_TEVCOLORCOMBINER2 = 0xC4,
    GX_BP_REG_TEVALPHACOMBINER2 = 0xC5,
    GX_BP_REG_TEVCOLORCOMBINER3 = 0xC6,
    GX_BP_REG_TEVALPHACOMBINER3 = 0xC7,
    GX_BP_REG_TEVCOLORCOMBINER4 = 0xC8,
    GX_BP_REG_TEVALPHACOMBINER4 = 0xC9,
    GX_BP_REG_TEVCOLORCOMBINER5 = 0xCA,
    GX_BP_REG_TEVALPHACOMBINER5 = 0xCB,
    GX_BP_REG_TEVCOLORCOMBINER6 = 0xCC,
    GX_BP_REG_TEVALPHACOMBINER6 = 0xCD,
    GX_BP_REG_TEVCOLORCOMBINER7 = 0xCE,
    GX_BP_REG_TEVALPHACOMBINER7 = 0xCF,
    GX_BP_REG_TEVCOLORCOMBINER8 = 0xD0,
    GX_BP_REG_TEVALPHACOMBINER8 = 0xD1,
    GX_BP_REG_TEVCOLORCOMBINER9 = 0xD2,
    GX_BP_REG_TEVALPHACOMBINER9 = 0xD3,
    GX_BP_REG_TEVCOLORCOMBINER10 = 0xD4,
    GX_BP_REG_TEVALPHACOMBINER10 = 0xD5,
    GX_BP_REG_TEVCOLORCOMBINER11 = 0xD6,
    GX_BP_REG_TEVALPHACOMBINER11 = 0xD7,
    GX_BP_REG_TEVCOLORCOMBINER12 = 0xD8,
    GX_BP_REG_TEVALPHACOMBINER12 = 0xD9,
    GX_BP_REG_TEVCOLORCOMBINER13 = 0xDA,
    GX_BP_REG_TEVALPHACOMBINER13 = 0xDB,
    GX_BP_REG_TEVCOLORCOMBINER14 = 0xDC,
    GX_BP_REG_TEVALPHACOMBINER14 = 0xDD,
    GX_BP_REG_TEVCOLORCOMBINER15 = 0xDE,
    GX_BP_REG_TEVALPHACOMBINER15 = 0xDF,
    GX_BP_REG_TEVREG0LO = 0xE0,
    GX_BP_REG_TEVREG0HI = 0xE1,
    GX_BP_REG_TEVREG1LO = 0xE2,
    GX_BP_REG_TEVREG1HI = 0xE3,
    GX_BP_REG_TEVREG2LO = 0xE4,
    GX_BP_REG_TEVREG2HI = 0xE5,
    GX_BP_REG_TEVREG3LO = 0xE6,
    GX_BP_REG_TEVREG3HI = 0xE7,
    GX_BP_REG_FOGRANGE = 0xE8,
    GX_BP_REG_FOGRANGEK0 = 0xE9,
    GX_BP_REG_FOGRANGEK1 = 0xEA,
    GX_BP_REG_FOGRANGEK2 = 0xEB,
    GX_BP_REG_FOGRANGEK3 = 0xEC,
    GX_BP_REG_FOGRANGEK4 = 0xED,
    GX_BP_REG_FOGPARAM0 = 0xEE,
    GX_BP_REG_FOGPARAM1 = 0xEF,
    GX_BP_REG_FOGPARAM2 = 0xF0,
    GX_BP_REG_FOGPARAM3 = 0xF1,
    GX_BP_REG_FOGCOLOR = 0xF2,
    GX_BP_REG_ALPHACOMPARE = 0xF3,
    GX_BP_REG_ZTEXTURE0 = 0xF4,
    GX_BP_REG_ZTEXTURE1 = 0xF5,
    GX_BP_REG_TEVKSEL0 = 0xF6,
    GX_BP_REG_TEVKSEL1 = 0xF7,
    GX_BP_REG_TEVKSEL2 = 0xF8,
    GX_BP_REG_TEVKSEL3 = 0xF9,
    GX_BP_REG_TEVKSEL4 = 0xFA,
    GX_BP_REG_TEVKSEL5 = 0xFB,
    GX_BP_REG_TEVKSEL6 = 0xFC,
    GX_BP_REG_TEVKSEL7 = 0xFD,
    GX_BP_REG_SSMASK = 0xFE,
} GX_BP_REG;

/******************************************************************************
 * BP register 0x0 - GenMode
 *****************************************************************************/
// NUMTEX [28:31] (4) - Active texture count
        /* start bit  */ #define GX_BP_GENMODE_NUMTEX_B 28
        /* end bit    */ #define GX_BP_GENMODE_NUMTEX_E 31
        /* bit size   */ #define GX_BP_GENMODE_NUMTEX_SZ 4

        /* raw mask   */ #define GX_BP_GENMODE_NUMTEX_MASK (((1 << 4) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_GENMODE_NUMTEX_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_GENMODE_NUMTEX_SHIFT 0

        /* get value  */ #define GX_BP_GET_GENMODE_NUMTEX(reg) GX_BITGET((reg), 28, 4)
        /* set value  */ #define GX_BP_SET_GENMODE_NUMTEX(reg, x) ((reg) = GX_BITSET((reg), 28, 4, x))

// NUMCOLORS [25:27] (3) - Color/channel count
        /* start bit  */ #define GX_BP_GENMODE_NUMCOLORS_B 25
        /* end bit    */ #define GX_BP_GENMODE_NUMCOLORS_E 27
        /* bit size   */ #define GX_BP_GENMODE_NUMCOLORS_SZ 3

        /* raw mask   */ #define GX_BP_GENMODE_NUMCOLORS_MASK (((1 << 3) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_GENMODE_NUMCOLORS_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_GENMODE_NUMCOLORS_SHIFT 4

        /* get value  */ #define GX_BP_GET_GENMODE_NUMCOLORS(reg) GX_BITGET((reg), 25, 3)
        /* set value  */ #define GX_BP_SET_GENMODE_NUMCOLORS(reg, x) ((reg) = GX_BITSET((reg), 25, 3, x))

// MULTISAMPLE [22:22] (1)
        /* start bit  */ #define GX_BP_GENMODE_MULTISAMPLE_B 22
        /* end bit    */ #define GX_BP_GENMODE_MULTISAMPLE_E 22
        /* bit size   */ #define GX_BP_GENMODE_MULTISAMPLE_SZ 1

        /* raw mask   */ #define GX_BP_GENMODE_MULTISAMPLE_MASK (((1 << 1) - 1) << (31 - 22))
        /* local mask */ #define GX_BP_GENMODE_MULTISAMPLE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_GENMODE_MULTISAMPLE_SHIFT 9

        /* get value  */ #define GX_BP_GET_GENMODE_MULTISAMPLE(reg) GX_BITGET((reg), 22, 1)
        /* set value  */ #define GX_BP_SET_GENMODE_MULTISAMPLE(reg, x) ((reg) = GX_BITSET((reg), 22, 1, x))

// NUMTEVSTAGES [18:21] (4)
        /* start bit  */ #define GX_BP_GENMODE_NUMTEVSTAGES_B 18
        /* end bit    */ #define GX_BP_GENMODE_NUMTEVSTAGES_E 21
        /* bit size   */ #define GX_BP_GENMODE_NUMTEVSTAGES_SZ 4

        /* raw mask   */ #define GX_BP_GENMODE_NUMTEVSTAGES_MASK (((1 << 4) - 1) << (31 - 21))
        /* local mask */ #define GX_BP_GENMODE_NUMTEVSTAGES_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_GENMODE_NUMTEVSTAGES_SHIFT 10

        /* get value  */ #define GX_BP_GET_GENMODE_NUMTEVSTAGES(reg) GX_BITGET((reg), 18, 4)
        /* set value  */ #define GX_BP_SET_GENMODE_NUMTEVSTAGES(reg, x) ((reg) = GX_BITSET((reg), 18, 4, x))

// CULLMODE [16:17] (2)
        /* start bit  */ #define GX_BP_GENMODE_CULLMODE_B 16
        /* end bit    */ #define GX_BP_GENMODE_CULLMODE_E 17
        /* bit size   */ #define GX_BP_GENMODE_CULLMODE_SZ 2

        /* raw mask   */ #define GX_BP_GENMODE_CULLMODE_MASK (((1 << 2) - 1) << (31 - 17))
        /* local mask */ #define GX_BP_GENMODE_CULLMODE_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_GENMODE_CULLMODE_SHIFT 14

        /* get value  */ #define GX_BP_GET_GENMODE_CULLMODE(reg) GX_BITGET((reg), 16, 2)
        /* set value  */ #define GX_BP_SET_GENMODE_CULLMODE(reg, x) ((reg) = GX_BITSET((reg), 16, 2, x))

// NUMINDSTAGES [13:15] (3)
        /* start bit  */ #define GX_BP_GENMODE_NUMINDSTAGES_B 13
        /* end bit    */ #define GX_BP_GENMODE_NUMINDSTAGES_E 15
        /* bit size   */ #define GX_BP_GENMODE_NUMINDSTAGES_SZ 3

        /* raw mask   */ #define GX_BP_GENMODE_NUMINDSTAGES_MASK (((1 << 3) - 1) << (31 - 15))
        /* local mask */ #define GX_BP_GENMODE_NUMINDSTAGES_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_GENMODE_NUMINDSTAGES_SHIFT 16

        /* get value  */ #define GX_BP_GET_GENMODE_NUMINDSTAGES(reg) GX_BITGET((reg), 13, 3)
        /* set value  */ #define GX_BP_SET_GENMODE_NUMINDSTAGES(reg, x) ((reg) = GX_BITSET((reg), 13, 3, x))

// COPLANAR [12:12] (1) - Toggle co-planar ("Z freeze" according to Dolphin)
        /* start bit  */ #define GX_BP_GENMODE_COPLANAR_B 12
        /* end bit    */ #define GX_BP_GENMODE_COPLANAR_E 12
        /* bit size   */ #define GX_BP_GENMODE_COPLANAR_SZ 1

        /* raw mask   */ #define GX_BP_GENMODE_COPLANAR_MASK (((1 << 1) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_GENMODE_COPLANAR_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_GENMODE_COPLANAR_SHIFT 19

        /* get value  */ #define GX_BP_GET_GENMODE_COPLANAR(reg) GX_BITGET((reg), 12, 1)
        /* set value  */ #define GX_BP_SET_GENMODE_COPLANAR(reg, x) ((reg) = GX_BITSET((reg), 12, 1, x))


/******************************************************************************
 * BP structure - IndMtxA
 *****************************************************************************/
// M00 [21:31] (11) - Texture offset matrix #0 [0][0]
        /* start bit  */ #define GX_BP_INDMTXA_M00_B 21
        /* end bit    */ #define GX_BP_INDMTXA_M00_E 31
        /* bit size   */ #define GX_BP_INDMTXA_M00_SZ 11

        /* raw mask   */ #define GX_BP_INDMTXA_M00_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_INDMTXA_M00_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_INDMTXA_M00_SHIFT 0

        /* get value  */ #define GX_BP_GET_INDMTXA_M00(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_INDMTXA_M00(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// M10 [10:20] (11) - Texture offset matrix #0 [1][0]
        /* start bit  */ #define GX_BP_INDMTXA_M10_B 10
        /* end bit    */ #define GX_BP_INDMTXA_M10_E 20
        /* bit size   */ #define GX_BP_INDMTXA_M10_SZ 11

        /* raw mask   */ #define GX_BP_INDMTXA_M10_MASK (((1 << 11) - 1) << (31 - 20))
        /* local mask */ #define GX_BP_INDMTXA_M10_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_INDMTXA_M10_SHIFT 11

        /* get value  */ #define GX_BP_GET_INDMTXA_M10(reg) GX_BITGET((reg), 10, 11)
        /* set value  */ #define GX_BP_SET_INDMTXA_M10(reg, x) ((reg) = GX_BITSET((reg), 10, 11, x))

// EXP [8:9] (2) - Bits 0-1 of scaling exponent #0 (2^x)
        /* start bit  */ #define GX_BP_INDMTXA_EXP_B 8
        /* end bit    */ #define GX_BP_INDMTXA_EXP_E 9
        /* bit size   */ #define GX_BP_INDMTXA_EXP_SZ 2

        /* raw mask   */ #define GX_BP_INDMTXA_EXP_MASK (((1 << 2) - 1) << (31 - 9))
        /* local mask */ #define GX_BP_INDMTXA_EXP_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_INDMTXA_EXP_SHIFT 22

        /* get value  */ #define GX_BP_GET_INDMTXA_EXP(reg) GX_BITGET((reg), 8, 2)
        /* set value  */ #define GX_BP_SET_INDMTXA_EXP(reg, x) ((reg) = GX_BITSET((reg), 8, 2, x))


/******************************************************************************
 * BP structure - IndMtxB
 *****************************************************************************/
// M01 [21:31] (11) - Texture offset matrix #0 [0][1]
        /* start bit  */ #define GX_BP_INDMTXB_M01_B 21
        /* end bit    */ #define GX_BP_INDMTXB_M01_E 31
        /* bit size   */ #define GX_BP_INDMTXB_M01_SZ 11

        /* raw mask   */ #define GX_BP_INDMTXB_M01_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_INDMTXB_M01_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_INDMTXB_M01_SHIFT 0

        /* get value  */ #define GX_BP_GET_INDMTXB_M01(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_INDMTXB_M01(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// M11 [10:20] (11) - Texture offset matrix #0 [1][1]
        /* start bit  */ #define GX_BP_INDMTXB_M11_B 10
        /* end bit    */ #define GX_BP_INDMTXB_M11_E 20
        /* bit size   */ #define GX_BP_INDMTXB_M11_SZ 11

        /* raw mask   */ #define GX_BP_INDMTXB_M11_MASK (((1 << 11) - 1) << (31 - 20))
        /* local mask */ #define GX_BP_INDMTXB_M11_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_INDMTXB_M11_SHIFT 11

        /* get value  */ #define GX_BP_GET_INDMTXB_M11(reg) GX_BITGET((reg), 10, 11)
        /* set value  */ #define GX_BP_SET_INDMTXB_M11(reg, x) ((reg) = GX_BITSET((reg), 10, 11, x))

// EXP [8:9] (2) - Bits 2-3 of scaling exponent #0 (2^x)
        /* start bit  */ #define GX_BP_INDMTXB_EXP_B 8
        /* end bit    */ #define GX_BP_INDMTXB_EXP_E 9
        /* bit size   */ #define GX_BP_INDMTXB_EXP_SZ 2

        /* raw mask   */ #define GX_BP_INDMTXB_EXP_MASK (((1 << 2) - 1) << (31 - 9))
        /* local mask */ #define GX_BP_INDMTXB_EXP_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_INDMTXB_EXP_SHIFT 22

        /* get value  */ #define GX_BP_GET_INDMTXB_EXP(reg) GX_BITGET((reg), 8, 2)
        /* set value  */ #define GX_BP_SET_INDMTXB_EXP(reg, x) ((reg) = GX_BITSET((reg), 8, 2, x))


/******************************************************************************
 * BP structure - IndMtxC
 *****************************************************************************/
// M02 [21:31] (11) - Texture offset matrix #0 [0][2]
        /* start bit  */ #define GX_BP_INDMTXC_M02_B 21
        /* end bit    */ #define GX_BP_INDMTXC_M02_E 31
        /* bit size   */ #define GX_BP_INDMTXC_M02_SZ 11

        /* raw mask   */ #define GX_BP_INDMTXC_M02_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_INDMTXC_M02_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_INDMTXC_M02_SHIFT 0

        /* get value  */ #define GX_BP_GET_INDMTXC_M02(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_INDMTXC_M02(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// M12 [10:20] (11) - Texture offset matrix #0 [1][2]
        /* start bit  */ #define GX_BP_INDMTXC_M12_B 10
        /* end bit    */ #define GX_BP_INDMTXC_M12_E 20
        /* bit size   */ #define GX_BP_INDMTXC_M12_SZ 11

        /* raw mask   */ #define GX_BP_INDMTXC_M12_MASK (((1 << 11) - 1) << (31 - 20))
        /* local mask */ #define GX_BP_INDMTXC_M12_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_INDMTXC_M12_SHIFT 11

        /* get value  */ #define GX_BP_GET_INDMTXC_M12(reg) GX_BITGET((reg), 10, 11)
        /* set value  */ #define GX_BP_SET_INDMTXC_M12(reg, x) ((reg) = GX_BITSET((reg), 10, 11, x))

// EXP [8:9] (2) - Bit 4 of scaling exponent #0 (2^x)
        /* start bit  */ #define GX_BP_INDMTXC_EXP_B 8
        /* end bit    */ #define GX_BP_INDMTXC_EXP_E 9
        /* bit size   */ #define GX_BP_INDMTXC_EXP_SZ 2

        /* raw mask   */ #define GX_BP_INDMTXC_EXP_MASK (((1 << 2) - 1) << (31 - 9))
        /* local mask */ #define GX_BP_INDMTXC_EXP_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_INDMTXC_EXP_SHIFT 22

        /* get value  */ #define GX_BP_GET_INDMTXC_EXP(reg) GX_BITGET((reg), 8, 2)
        /* set value  */ #define GX_BP_SET_INDMTXC_EXP(reg, x) ((reg) = GX_BITSET((reg), 8, 2, x))


/******************************************************************************
 * BP register 0xF - IndIMask
 *****************************************************************************/
// IMASK [24:31] (8) - Indirect mask for textures
        /* start bit  */ #define GX_BP_INDIMASK_IMASK_B 24
        /* end bit    */ #define GX_BP_INDIMASK_IMASK_E 31
        /* bit size   */ #define GX_BP_INDIMASK_IMASK_SZ 8

        /* raw mask   */ #define GX_BP_INDIMASK_IMASK_MASK (((1 << 8) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_INDIMASK_IMASK_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_INDIMASK_IMASK_SHIFT 0

        /* get value  */ #define GX_BP_GET_INDIMASK_IMASK(reg) GX_BITGET((reg), 24, 8)
        /* set value  */ #define GX_BP_SET_INDIMASK_IMASK(reg, x) ((reg) = GX_BITSET((reg), 24, 8, x))


/******************************************************************************
 * BP structure - IndTevStage
 *****************************************************************************/
// STAGE [30:31] (2) - Indirect texture stage ID
        /* start bit  */ #define GX_BP_INDTEVSTAGE_STAGE_B 30
        /* end bit    */ #define GX_BP_INDTEVSTAGE_STAGE_E 31
        /* bit size   */ #define GX_BP_INDTEVSTAGE_STAGE_SZ 2

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_STAGE_MASK (((1 << 2) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_INDTEVSTAGE_STAGE_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_STAGE_SHIFT 0

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_STAGE(reg) GX_BITGET((reg), 30, 2)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_STAGE(reg, x) ((reg) = GX_BITSET((reg), 30, 2, x))

// FORMAT [28:29] (2) - Indirect texture format
        /* start bit  */ #define GX_BP_INDTEVSTAGE_FORMAT_B 28
        /* end bit    */ #define GX_BP_INDTEVSTAGE_FORMAT_E 29
        /* bit size   */ #define GX_BP_INDTEVSTAGE_FORMAT_SZ 2

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_FORMAT_MASK (((1 << 2) - 1) << (31 - 29))
        /* local mask */ #define GX_BP_INDTEVSTAGE_FORMAT_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_FORMAT_SHIFT 2

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_FORMAT(reg) GX_BITGET((reg), 28, 2)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_FORMAT(reg, x) ((reg) = GX_BITSET((reg), 28, 2, x))

// BIAS [25:27] (3) - Indirect texture bias
        /* start bit  */ #define GX_BP_INDTEVSTAGE_BIAS_B 25
        /* end bit    */ #define GX_BP_INDTEVSTAGE_BIAS_E 27
        /* bit size   */ #define GX_BP_INDTEVSTAGE_BIAS_SZ 3

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_BIAS_MASK (((1 << 3) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_INDTEVSTAGE_BIAS_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_BIAS_SHIFT 4

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_BIAS(reg) GX_BITGET((reg), 25, 3)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_BIAS(reg, x) ((reg) = GX_BITSET((reg), 25, 3, x))

// ALPHA [23:24] (2) - Indirect texture alpha
        /* start bit  */ #define GX_BP_INDTEVSTAGE_ALPHA_B 23
        /* end bit    */ #define GX_BP_INDTEVSTAGE_ALPHA_E 24
        /* bit size   */ #define GX_BP_INDTEVSTAGE_ALPHA_SZ 2

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_ALPHA_MASK (((1 << 2) - 1) << (31 - 24))
        /* local mask */ #define GX_BP_INDTEVSTAGE_ALPHA_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_ALPHA_SHIFT 7

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_ALPHA(reg) GX_BITGET((reg), 23, 2)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_ALPHA(reg, x) ((reg) = GX_BITSET((reg), 23, 2, x))

// MTX [19:22] (4) - Indirect texture matrix
        /* start bit  */ #define GX_BP_INDTEVSTAGE_MTX_B 19
        /* end bit    */ #define GX_BP_INDTEVSTAGE_MTX_E 22
        /* bit size   */ #define GX_BP_INDTEVSTAGE_MTX_SZ 4

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_MTX_MASK (((1 << 4) - 1) << (31 - 22))
        /* local mask */ #define GX_BP_INDTEVSTAGE_MTX_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_MTX_SHIFT 9

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_MTX(reg) GX_BITGET((reg), 19, 4)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_MTX(reg, x) ((reg) = GX_BITSET((reg), 19, 4, x))

// WRAPS [16:18] (3) - S component wrap factor
        /* start bit  */ #define GX_BP_INDTEVSTAGE_WRAPS_B 16
        /* end bit    */ #define GX_BP_INDTEVSTAGE_WRAPS_E 18
        /* bit size   */ #define GX_BP_INDTEVSTAGE_WRAPS_SZ 3

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_WRAPS_MASK (((1 << 3) - 1) << (31 - 18))
        /* local mask */ #define GX_BP_INDTEVSTAGE_WRAPS_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_WRAPS_SHIFT 13

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_WRAPS(reg) GX_BITGET((reg), 16, 3)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_WRAPS(reg, x) ((reg) = GX_BITSET((reg), 16, 3, x))

// WRAPT [13:15] (3) - T component wrap factor
        /* start bit  */ #define GX_BP_INDTEVSTAGE_WRAPT_B 13
        /* end bit    */ #define GX_BP_INDTEVSTAGE_WRAPT_E 15
        /* bit size   */ #define GX_BP_INDTEVSTAGE_WRAPT_SZ 3

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_WRAPT_MASK (((1 << 3) - 1) << (31 - 15))
        /* local mask */ #define GX_BP_INDTEVSTAGE_WRAPT_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_WRAPT_SHIFT 16

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_WRAPT(reg) GX_BITGET((reg), 13, 3)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_WRAPT(reg, x) ((reg) = GX_BITSET((reg), 13, 3, x))

// UTCLOD [12:12] (1) - Whether to use unmodified texcoords for mipmaps
        /* start bit  */ #define GX_BP_INDTEVSTAGE_UTCLOD_B 12
        /* end bit    */ #define GX_BP_INDTEVSTAGE_UTCLOD_E 12
        /* bit size   */ #define GX_BP_INDTEVSTAGE_UTCLOD_SZ 1

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_UTCLOD_MASK (((1 << 1) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_INDTEVSTAGE_UTCLOD_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_UTCLOD_SHIFT 19

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_UTCLOD(reg) GX_BITGET((reg), 12, 1)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_UTCLOD(reg, x) ((reg) = GX_BITSET((reg), 12, 1, x))

// ADDPREV [11:11] (1) - Whether to add in results from previous tev stage
        /* start bit  */ #define GX_BP_INDTEVSTAGE_ADDPREV_B 11
        /* end bit    */ #define GX_BP_INDTEVSTAGE_ADDPREV_E 11
        /* bit size   */ #define GX_BP_INDTEVSTAGE_ADDPREV_SZ 1

        /* raw mask   */ #define GX_BP_INDTEVSTAGE_ADDPREV_MASK (((1 << 1) - 1) << (31 - 11))
        /* local mask */ #define GX_BP_INDTEVSTAGE_ADDPREV_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_INDTEVSTAGE_ADDPREV_SHIFT 20

        /* get value  */ #define GX_BP_GET_INDTEVSTAGE_ADDPREV(reg) GX_BITGET((reg), 11, 1)
        /* set value  */ #define GX_BP_SET_INDTEVSTAGE_ADDPREV(reg, x) ((reg) = GX_BITSET((reg), 11, 1, x))


/******************************************************************************
 * BP register 0x20 - scissorTL
 *****************************************************************************/
// TOP [21:31] (11) - Top component
        /* start bit  */ #define GX_BP_SCISSORTL_TOP_B 21
        /* end bit    */ #define GX_BP_SCISSORTL_TOP_E 31
        /* bit size   */ #define GX_BP_SCISSORTL_TOP_SZ 11

        /* raw mask   */ #define GX_BP_SCISSORTL_TOP_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_SCISSORTL_TOP_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_SCISSORTL_TOP_SHIFT 0

        /* get value  */ #define GX_BP_GET_SCISSORTL_TOP(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_SCISSORTL_TOP(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// LEFT [9:19] (11) - Left component
        /* start bit  */ #define GX_BP_SCISSORTL_LEFT_B 9
        /* end bit    */ #define GX_BP_SCISSORTL_LEFT_E 19
        /* bit size   */ #define GX_BP_SCISSORTL_LEFT_SZ 11

        /* raw mask   */ #define GX_BP_SCISSORTL_LEFT_MASK (((1 << 11) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_SCISSORTL_LEFT_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_SCISSORTL_LEFT_SHIFT 12

        /* get value  */ #define GX_BP_GET_SCISSORTL_LEFT(reg) GX_BITGET((reg), 9, 11)
        /* set value  */ #define GX_BP_SET_SCISSORTL_LEFT(reg, x) ((reg) = GX_BITSET((reg), 9, 11, x))


/******************************************************************************
 * BP register 0x21 - scissorBR
 *****************************************************************************/
// BOT [21:31] (11) - Bottom component
        /* start bit  */ #define GX_BP_SCISSORBR_BOT_B 21
        /* end bit    */ #define GX_BP_SCISSORBR_BOT_E 31
        /* bit size   */ #define GX_BP_SCISSORBR_BOT_SZ 11

        /* raw mask   */ #define GX_BP_SCISSORBR_BOT_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_SCISSORBR_BOT_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_SCISSORBR_BOT_SHIFT 0

        /* get value  */ #define GX_BP_GET_SCISSORBR_BOT(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_SCISSORBR_BOT(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// RIGHT [9:19] (11) - Right component
        /* start bit  */ #define GX_BP_SCISSORBR_RIGHT_B 9
        /* end bit    */ #define GX_BP_SCISSORBR_RIGHT_E 19
        /* bit size   */ #define GX_BP_SCISSORBR_RIGHT_SZ 11

        /* raw mask   */ #define GX_BP_SCISSORBR_RIGHT_MASK (((1 << 11) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_SCISSORBR_RIGHT_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_SCISSORBR_RIGHT_SHIFT 12

        /* get value  */ #define GX_BP_GET_SCISSORBR_RIGHT(reg) GX_BITGET((reg), 9, 11)
        /* set value  */ #define GX_BP_SET_SCISSORBR_RIGHT(reg, x) ((reg) = GX_BITSET((reg), 9, 11, x))


/******************************************************************************
 * BP register 0x22 - linePtWidth
 *****************************************************************************/
// LINESZ [24:31] (8) - Line size/width
        /* start bit  */ #define GX_BP_LINEPTWIDTH_LINESZ_B 24
        /* end bit    */ #define GX_BP_LINEPTWIDTH_LINESZ_E 31
        /* bit size   */ #define GX_BP_LINEPTWIDTH_LINESZ_SZ 8

        /* raw mask   */ #define GX_BP_LINEPTWIDTH_LINESZ_MASK (((1 << 8) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_LINEPTWIDTH_LINESZ_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_LINEPTWIDTH_LINESZ_SHIFT 0

        /* get value  */ #define GX_BP_GET_LINEPTWIDTH_LINESZ(reg) GX_BITGET((reg), 24, 8)
        /* set value  */ #define GX_BP_SET_LINEPTWIDTH_LINESZ(reg, x) ((reg) = GX_BITSET((reg), 24, 8, x))

// POINTSZ [16:23] (8) - Point size
        /* start bit  */ #define GX_BP_LINEPTWIDTH_POINTSZ_B 16
        /* end bit    */ #define GX_BP_LINEPTWIDTH_POINTSZ_E 23
        /* bit size   */ #define GX_BP_LINEPTWIDTH_POINTSZ_SZ 8

        /* raw mask   */ #define GX_BP_LINEPTWIDTH_POINTSZ_MASK (((1 << 8) - 1) << (31 - 23))
        /* local mask */ #define GX_BP_LINEPTWIDTH_POINTSZ_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_LINEPTWIDTH_POINTSZ_SHIFT 8

        /* get value  */ #define GX_BP_GET_LINEPTWIDTH_POINTSZ(reg) GX_BITGET((reg), 16, 8)
        /* set value  */ #define GX_BP_SET_LINEPTWIDTH_POINTSZ(reg, x) ((reg) = GX_BITSET((reg), 16, 8, x))

// LINEOFS [13:15] (3) - Line offset
        /* start bit  */ #define GX_BP_LINEPTWIDTH_LINEOFS_B 13
        /* end bit    */ #define GX_BP_LINEPTWIDTH_LINEOFS_E 15
        /* bit size   */ #define GX_BP_LINEPTWIDTH_LINEOFS_SZ 3

        /* raw mask   */ #define GX_BP_LINEPTWIDTH_LINEOFS_MASK (((1 << 3) - 1) << (31 - 15))
        /* local mask */ #define GX_BP_LINEPTWIDTH_LINEOFS_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_LINEPTWIDTH_LINEOFS_SHIFT 16

        /* get value  */ #define GX_BP_GET_LINEPTWIDTH_LINEOFS(reg) GX_BITGET((reg), 13, 3)
        /* set value  */ #define GX_BP_SET_LINEPTWIDTH_LINEOFS(reg, x) ((reg) = GX_BITSET((reg), 13, 3, x))

// POINTOFS [10:12] (3) - Point offset
        /* start bit  */ #define GX_BP_LINEPTWIDTH_POINTOFS_B 10
        /* end bit    */ #define GX_BP_LINEPTWIDTH_POINTOFS_E 12
        /* bit size   */ #define GX_BP_LINEPTWIDTH_POINTOFS_SZ 3

        /* raw mask   */ #define GX_BP_LINEPTWIDTH_POINTOFS_MASK (((1 << 3) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_LINEPTWIDTH_POINTOFS_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_LINEPTWIDTH_POINTOFS_SHIFT 19

        /* get value  */ #define GX_BP_GET_LINEPTWIDTH_POINTOFS(reg) GX_BITGET((reg), 10, 3)
        /* set value  */ #define GX_BP_SET_LINEPTWIDTH_POINTOFS(reg, x) ((reg) = GX_BITSET((reg), 10, 3, x))

// ADJUST_AR [9:9] (1) - Interlacing: adjust for pixels having aspect ratio of 1/2
        /* start bit  */ #define GX_BP_LINEPTWIDTH_ADJUST_AR_B 9
        /* end bit    */ #define GX_BP_LINEPTWIDTH_ADJUST_AR_E 9
        /* bit size   */ #define GX_BP_LINEPTWIDTH_ADJUST_AR_SZ 1

        /* raw mask   */ #define GX_BP_LINEPTWIDTH_ADJUST_AR_MASK (((1 << 1) - 1) << (31 - 9))
        /* local mask */ #define GX_BP_LINEPTWIDTH_ADJUST_AR_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_LINEPTWIDTH_ADJUST_AR_SHIFT 22

        /* get value  */ #define GX_BP_GET_LINEPTWIDTH_ADJUST_AR(reg) GX_BITGET((reg), 9, 1)
        /* set value  */ #define GX_BP_SET_LINEPTWIDTH_ADJUST_AR(reg, x) ((reg) = GX_BITSET((reg), 9, 1, x))


/******************************************************************************
 * BP register 0x25 - ras1_ss0
 *****************************************************************************/
// S0 [28:31] (4) - S-component scale (stage 0)
        /* start bit  */ #define GX_BP_RAS1_SS0_S0_B 28
        /* end bit    */ #define GX_BP_RAS1_SS0_S0_E 31
        /* bit size   */ #define GX_BP_RAS1_SS0_S0_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS0_S0_MASK (((1 << 4) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_RAS1_SS0_S0_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS0_S0_SHIFT 0

        /* get value  */ #define GX_BP_GET_RAS1_SS0_S0(reg) GX_BITGET((reg), 28, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS0_S0(reg, x) ((reg) = GX_BITSET((reg), 28, 4, x))

// T0 [24:27] (4) - T-component scale (stage 0)
        /* start bit  */ #define GX_BP_RAS1_SS0_T0_B 24
        /* end bit    */ #define GX_BP_RAS1_SS0_T0_E 27
        /* bit size   */ #define GX_BP_RAS1_SS0_T0_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS0_T0_MASK (((1 << 4) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_RAS1_SS0_T0_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS0_T0_SHIFT 4

        /* get value  */ #define GX_BP_GET_RAS1_SS0_T0(reg) GX_BITGET((reg), 24, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS0_T0(reg, x) ((reg) = GX_BITSET((reg), 24, 4, x))

// S1 [20:23] (4) - S-component scale (stage 1)
        /* start bit  */ #define GX_BP_RAS1_SS0_S1_B 20
        /* end bit    */ #define GX_BP_RAS1_SS0_S1_E 23
        /* bit size   */ #define GX_BP_RAS1_SS0_S1_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS0_S1_MASK (((1 << 4) - 1) << (31 - 23))
        /* local mask */ #define GX_BP_RAS1_SS0_S1_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS0_S1_SHIFT 8

        /* get value  */ #define GX_BP_GET_RAS1_SS0_S1(reg) GX_BITGET((reg), 20, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS0_S1(reg, x) ((reg) = GX_BITSET((reg), 20, 4, x))

// T1 [16:19] (4) - T-component scale (stage 1)
        /* start bit  */ #define GX_BP_RAS1_SS0_T1_B 16
        /* end bit    */ #define GX_BP_RAS1_SS0_T1_E 19
        /* bit size   */ #define GX_BP_RAS1_SS0_T1_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS0_T1_MASK (((1 << 4) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_RAS1_SS0_T1_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS0_T1_SHIFT 12

        /* get value  */ #define GX_BP_GET_RAS1_SS0_T1(reg) GX_BITGET((reg), 16, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS0_T1(reg, x) ((reg) = GX_BITSET((reg), 16, 4, x))


/******************************************************************************
 * BP register 0x26 - ras1_ss1
 *****************************************************************************/
// S2 [28:31] (4) - S-component scale (stage 2)
        /* start bit  */ #define GX_BP_RAS1_SS1_S2_B 28
        /* end bit    */ #define GX_BP_RAS1_SS1_S2_E 31
        /* bit size   */ #define GX_BP_RAS1_SS1_S2_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS1_S2_MASK (((1 << 4) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_RAS1_SS1_S2_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS1_S2_SHIFT 0

        /* get value  */ #define GX_BP_GET_RAS1_SS1_S2(reg) GX_BITGET((reg), 28, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS1_S2(reg, x) ((reg) = GX_BITSET((reg), 28, 4, x))

// T2 [24:27] (4) - T-component scale (stage 2)
        /* start bit  */ #define GX_BP_RAS1_SS1_T2_B 24
        /* end bit    */ #define GX_BP_RAS1_SS1_T2_E 27
        /* bit size   */ #define GX_BP_RAS1_SS1_T2_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS1_T2_MASK (((1 << 4) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_RAS1_SS1_T2_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS1_T2_SHIFT 4

        /* get value  */ #define GX_BP_GET_RAS1_SS1_T2(reg) GX_BITGET((reg), 24, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS1_T2(reg, x) ((reg) = GX_BITSET((reg), 24, 4, x))

// S3 [20:23] (4) - S-component scale (stage 3)
        /* start bit  */ #define GX_BP_RAS1_SS1_S3_B 20
        /* end bit    */ #define GX_BP_RAS1_SS1_S3_E 23
        /* bit size   */ #define GX_BP_RAS1_SS1_S3_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS1_S3_MASK (((1 << 4) - 1) << (31 - 23))
        /* local mask */ #define GX_BP_RAS1_SS1_S3_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS1_S3_SHIFT 8

        /* get value  */ #define GX_BP_GET_RAS1_SS1_S3(reg) GX_BITGET((reg), 20, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS1_S3(reg, x) ((reg) = GX_BITSET((reg), 20, 4, x))

// T3 [16:19] (4) - T-component scale (stage 3)
        /* start bit  */ #define GX_BP_RAS1_SS1_T3_B 16
        /* end bit    */ #define GX_BP_RAS1_SS1_T3_E 19
        /* bit size   */ #define GX_BP_RAS1_SS1_T3_SZ 4

        /* raw mask   */ #define GX_BP_RAS1_SS1_T3_MASK (((1 << 4) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_RAS1_SS1_T3_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_RAS1_SS1_T3_SHIFT 12

        /* get value  */ #define GX_BP_GET_RAS1_SS1_T3(reg) GX_BITGET((reg), 16, 4)
        /* set value  */ #define GX_BP_SET_RAS1_SS1_T3(reg, x) ((reg) = GX_BITSET((reg), 16, 4, x))


/******************************************************************************
 * BP register 0x27 - ras1_iref
 *****************************************************************************/
// MAP0 [29:31] (3) - Texmap id (stage 0)
        /* start bit  */ #define GX_BP_RAS1_IREF_MAP0_B 29
        /* end bit    */ #define GX_BP_RAS1_IREF_MAP0_E 31
        /* bit size   */ #define GX_BP_RAS1_IREF_MAP0_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_MAP0_MASK (((1 << 3) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_RAS1_IREF_MAP0_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_MAP0_SHIFT 0

        /* get value  */ #define GX_BP_GET_RAS1_IREF_MAP0(reg) GX_BITGET((reg), 29, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_MAP0(reg, x) ((reg) = GX_BITSET((reg), 29, 3, x))

// TXC0 [26:28] (3) - Texcoord ID (stage 0)
        /* start bit  */ #define GX_BP_RAS1_IREF_TXC0_B 26
        /* end bit    */ #define GX_BP_RAS1_IREF_TXC0_E 28
        /* bit size   */ #define GX_BP_RAS1_IREF_TXC0_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_TXC0_MASK (((1 << 3) - 1) << (31 - 28))
        /* local mask */ #define GX_BP_RAS1_IREF_TXC0_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_TXC0_SHIFT 3

        /* get value  */ #define GX_BP_GET_RAS1_IREF_TXC0(reg) GX_BITGET((reg), 26, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_TXC0(reg, x) ((reg) = GX_BITSET((reg), 26, 3, x))

// MAP1 [23:25] (3) - Texmap id (stage 1)
        /* start bit  */ #define GX_BP_RAS1_IREF_MAP1_B 23
        /* end bit    */ #define GX_BP_RAS1_IREF_MAP1_E 25
        /* bit size   */ #define GX_BP_RAS1_IREF_MAP1_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_MAP1_MASK (((1 << 3) - 1) << (31 - 25))
        /* local mask */ #define GX_BP_RAS1_IREF_MAP1_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_MAP1_SHIFT 6

        /* get value  */ #define GX_BP_GET_RAS1_IREF_MAP1(reg) GX_BITGET((reg), 23, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_MAP1(reg, x) ((reg) = GX_BITSET((reg), 23, 3, x))

// TXC1 [20:22] (3) - Texcoord ID (stage 1)
        /* start bit  */ #define GX_BP_RAS1_IREF_TXC1_B 20
        /* end bit    */ #define GX_BP_RAS1_IREF_TXC1_E 22
        /* bit size   */ #define GX_BP_RAS1_IREF_TXC1_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_TXC1_MASK (((1 << 3) - 1) << (31 - 22))
        /* local mask */ #define GX_BP_RAS1_IREF_TXC1_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_TXC1_SHIFT 9

        /* get value  */ #define GX_BP_GET_RAS1_IREF_TXC1(reg) GX_BITGET((reg), 20, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_TXC1(reg, x) ((reg) = GX_BITSET((reg), 20, 3, x))

// MAP2 [17:19] (3) - Texmap id (stage 2)
        /* start bit  */ #define GX_BP_RAS1_IREF_MAP2_B 17
        /* end bit    */ #define GX_BP_RAS1_IREF_MAP2_E 19
        /* bit size   */ #define GX_BP_RAS1_IREF_MAP2_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_MAP2_MASK (((1 << 3) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_RAS1_IREF_MAP2_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_MAP2_SHIFT 12

        /* get value  */ #define GX_BP_GET_RAS1_IREF_MAP2(reg) GX_BITGET((reg), 17, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_MAP2(reg, x) ((reg) = GX_BITSET((reg), 17, 3, x))

// TXC2 [14:16] (3) - Texcoord ID (stage 2)
        /* start bit  */ #define GX_BP_RAS1_IREF_TXC2_B 14
        /* end bit    */ #define GX_BP_RAS1_IREF_TXC2_E 16
        /* bit size   */ #define GX_BP_RAS1_IREF_TXC2_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_TXC2_MASK (((1 << 3) - 1) << (31 - 16))
        /* local mask */ #define GX_BP_RAS1_IREF_TXC2_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_TXC2_SHIFT 15

        /* get value  */ #define GX_BP_GET_RAS1_IREF_TXC2(reg) GX_BITGET((reg), 14, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_TXC2(reg, x) ((reg) = GX_BITSET((reg), 14, 3, x))

// MAP3 [11:13] (3) - Texmap id (stage 3)
        /* start bit  */ #define GX_BP_RAS1_IREF_MAP3_B 11
        /* end bit    */ #define GX_BP_RAS1_IREF_MAP3_E 13
        /* bit size   */ #define GX_BP_RAS1_IREF_MAP3_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_MAP3_MASK (((1 << 3) - 1) << (31 - 13))
        /* local mask */ #define GX_BP_RAS1_IREF_MAP3_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_MAP3_SHIFT 18

        /* get value  */ #define GX_BP_GET_RAS1_IREF_MAP3(reg) GX_BITGET((reg), 11, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_MAP3(reg, x) ((reg) = GX_BITSET((reg), 11, 3, x))

// TXC3 [8:10] (3) - Texcoord ID (stage 3)
        /* start bit  */ #define GX_BP_RAS1_IREF_TXC3_B 8
        /* end bit    */ #define GX_BP_RAS1_IREF_TXC3_E 10
        /* bit size   */ #define GX_BP_RAS1_IREF_TXC3_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_IREF_TXC3_MASK (((1 << 3) - 1) << (31 - 10))
        /* local mask */ #define GX_BP_RAS1_IREF_TXC3_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_IREF_TXC3_SHIFT 21

        /* get value  */ #define GX_BP_GET_RAS1_IREF_TXC3(reg) GX_BITGET((reg), 8, 3)
        /* set value  */ #define GX_BP_SET_RAS1_IREF_TXC3(reg, x) ((reg) = GX_BITSET((reg), 8, 3, x))


/******************************************************************************
 * BP structure - ras1_tref
 *****************************************************************************/
// TEXMAP_EVEN [29:31] (3)
        /* start bit  */ #define GX_BP_RAS1_TREF_TEXMAP_EVEN_B 29
        /* end bit    */ #define GX_BP_RAS1_TREF_TEXMAP_EVEN_E 31
        /* bit size   */ #define GX_BP_RAS1_TREF_TEXMAP_EVEN_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_TREF_TEXMAP_EVEN_MASK (((1 << 3) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_RAS1_TREF_TEXMAP_EVEN_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_TEXMAP_EVEN_SHIFT 0

        /* get value  */ #define GX_BP_GET_RAS1_TREF_TEXMAP_EVEN(reg) GX_BITGET((reg), 29, 3)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_TEXMAP_EVEN(reg, x) ((reg) = GX_BITSET((reg), 29, 3, x))

// TEXCOORD_EVEN [26:28] (3)
        /* start bit  */ #define GX_BP_RAS1_TREF_TEXCOORD_EVEN_B 26
        /* end bit    */ #define GX_BP_RAS1_TREF_TEXCOORD_EVEN_E 28
        /* bit size   */ #define GX_BP_RAS1_TREF_TEXCOORD_EVEN_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_TREF_TEXCOORD_EVEN_MASK (((1 << 3) - 1) << (31 - 28))
        /* local mask */ #define GX_BP_RAS1_TREF_TEXCOORD_EVEN_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_TEXCOORD_EVEN_SHIFT 3

        /* get value  */ #define GX_BP_GET_RAS1_TREF_TEXCOORD_EVEN(reg) GX_BITGET((reg), 26, 3)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_TEXCOORD_EVEN(reg, x) ((reg) = GX_BITSET((reg), 26, 3, x))

// ENABLE_TEX_EVEN [25:25] (1)
        /* start bit  */ #define GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_B 25
        /* end bit    */ #define GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_E 25
        /* bit size   */ #define GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_SZ 1

        /* raw mask   */ #define GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_MASK (((1 << 1) - 1) << (31 - 25))
        /* local mask */ #define GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_ENABLE_TEX_EVEN_SHIFT 6

        /* get value  */ #define GX_BP_GET_RAS1_TREF_ENABLE_TEX_EVEN(reg) GX_BITGET((reg), 25, 1)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_ENABLE_TEX_EVEN(reg, x) ((reg) = GX_BITSET((reg), 25, 1, x))

// COLORCHAN_EVEN [22:24] (3)
        /* start bit  */ #define GX_BP_RAS1_TREF_COLORCHAN_EVEN_B 22
        /* end bit    */ #define GX_BP_RAS1_TREF_COLORCHAN_EVEN_E 24
        /* bit size   */ #define GX_BP_RAS1_TREF_COLORCHAN_EVEN_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_TREF_COLORCHAN_EVEN_MASK (((1 << 3) - 1) << (31 - 24))
        /* local mask */ #define GX_BP_RAS1_TREF_COLORCHAN_EVEN_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_COLORCHAN_EVEN_SHIFT 7

        /* get value  */ #define GX_BP_GET_RAS1_TREF_COLORCHAN_EVEN(reg) GX_BITGET((reg), 22, 3)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_COLORCHAN_EVEN(reg, x) ((reg) = GX_BITSET((reg), 22, 3, x))

// TEXMAP_ODD [17:19] (3)
        /* start bit  */ #define GX_BP_RAS1_TREF_TEXMAP_ODD_B 17
        /* end bit    */ #define GX_BP_RAS1_TREF_TEXMAP_ODD_E 19
        /* bit size   */ #define GX_BP_RAS1_TREF_TEXMAP_ODD_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_TREF_TEXMAP_ODD_MASK (((1 << 3) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_RAS1_TREF_TEXMAP_ODD_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_TEXMAP_ODD_SHIFT 12

        /* get value  */ #define GX_BP_GET_RAS1_TREF_TEXMAP_ODD(reg) GX_BITGET((reg), 17, 3)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_TEXMAP_ODD(reg, x) ((reg) = GX_BITSET((reg), 17, 3, x))

// TEXCOORD_ODD [14:16] (3)
        /* start bit  */ #define GX_BP_RAS1_TREF_TEXCOORD_ODD_B 14
        /* end bit    */ #define GX_BP_RAS1_TREF_TEXCOORD_ODD_E 16
        /* bit size   */ #define GX_BP_RAS1_TREF_TEXCOORD_ODD_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_TREF_TEXCOORD_ODD_MASK (((1 << 3) - 1) << (31 - 16))
        /* local mask */ #define GX_BP_RAS1_TREF_TEXCOORD_ODD_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_TEXCOORD_ODD_SHIFT 15

        /* get value  */ #define GX_BP_GET_RAS1_TREF_TEXCOORD_ODD(reg) GX_BITGET((reg), 14, 3)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_TEXCOORD_ODD(reg, x) ((reg) = GX_BITSET((reg), 14, 3, x))

// ENABLE_TEX_ODD [13:13] (1)
        /* start bit  */ #define GX_BP_RAS1_TREF_ENABLE_TEX_ODD_B 13
        /* end bit    */ #define GX_BP_RAS1_TREF_ENABLE_TEX_ODD_E 13
        /* bit size   */ #define GX_BP_RAS1_TREF_ENABLE_TEX_ODD_SZ 1

        /* raw mask   */ #define GX_BP_RAS1_TREF_ENABLE_TEX_ODD_MASK (((1 << 1) - 1) << (31 - 13))
        /* local mask */ #define GX_BP_RAS1_TREF_ENABLE_TEX_ODD_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_ENABLE_TEX_ODD_SHIFT 18

        /* get value  */ #define GX_BP_GET_RAS1_TREF_ENABLE_TEX_ODD(reg) GX_BITGET((reg), 13, 1)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_ENABLE_TEX_ODD(reg, x) ((reg) = GX_BITSET((reg), 13, 1, x))

// COLORCHAN_ODD [10:12] (3)
        /* start bit  */ #define GX_BP_RAS1_TREF_COLORCHAN_ODD_B 10
        /* end bit    */ #define GX_BP_RAS1_TREF_COLORCHAN_ODD_E 12
        /* bit size   */ #define GX_BP_RAS1_TREF_COLORCHAN_ODD_SZ 3

        /* raw mask   */ #define GX_BP_RAS1_TREF_COLORCHAN_ODD_MASK (((1 << 3) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_RAS1_TREF_COLORCHAN_ODD_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_RAS1_TREF_COLORCHAN_ODD_SHIFT 19

        /* get value  */ #define GX_BP_GET_RAS1_TREF_COLORCHAN_ODD(reg) GX_BITGET((reg), 10, 3)
        /* set value  */ #define GX_BP_SET_RAS1_TREF_COLORCHAN_ODD(reg, x) ((reg) = GX_BITSET((reg), 10, 3, x))


/******************************************************************************
 * BP structure - su_size
 *****************************************************************************/
// SCALE [16:31] (16)
        /* start bit  */ #define GX_BP_SU_SIZE_SCALE_B 16
        /* end bit    */ #define GX_BP_SU_SIZE_SCALE_E 31
        /* bit size   */ #define GX_BP_SU_SIZE_SCALE_SZ 16

        /* raw mask   */ #define GX_BP_SU_SIZE_SCALE_MASK (((1 << 16) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_SU_SIZE_SCALE_LMASK ((1 << 16) - 1)
        /* bit shift  */ #define GX_BP_SU_SIZE_SCALE_SHIFT 0

        /* get value  */ #define GX_BP_GET_SU_SIZE_SCALE(reg) GX_BITGET((reg), 16, 16)
        /* set value  */ #define GX_BP_SET_SU_SIZE_SCALE(reg, x) ((reg) = GX_BITSET((reg), 16, 16, x))

// RANGEBIAS [15:15] (1)
        /* start bit  */ #define GX_BP_SU_SIZE_RANGEBIAS_B 15
        /* end bit    */ #define GX_BP_SU_SIZE_RANGEBIAS_E 15
        /* bit size   */ #define GX_BP_SU_SIZE_RANGEBIAS_SZ 1

        /* raw mask   */ #define GX_BP_SU_SIZE_RANGEBIAS_MASK (((1 << 1) - 1) << (31 - 15))
        /* local mask */ #define GX_BP_SU_SIZE_RANGEBIAS_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_SU_SIZE_RANGEBIAS_SHIFT 16

        /* get value  */ #define GX_BP_GET_SU_SIZE_RANGEBIAS(reg) GX_BITGET((reg), 15, 1)
        /* set value  */ #define GX_BP_SET_SU_SIZE_RANGEBIAS(reg, x) ((reg) = GX_BITSET((reg), 15, 1, x))

// CYLINDRICWRAP [14:14] (1)
        /* start bit  */ #define GX_BP_SU_SIZE_CYLINDRICWRAP_B 14
        /* end bit    */ #define GX_BP_SU_SIZE_CYLINDRICWRAP_E 14
        /* bit size   */ #define GX_BP_SU_SIZE_CYLINDRICWRAP_SZ 1

        /* raw mask   */ #define GX_BP_SU_SIZE_CYLINDRICWRAP_MASK (((1 << 1) - 1) << (31 - 14))
        /* local mask */ #define GX_BP_SU_SIZE_CYLINDRICWRAP_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_SU_SIZE_CYLINDRICWRAP_SHIFT 17

        /* get value  */ #define GX_BP_GET_SU_SIZE_CYLINDRICWRAP(reg) GX_BITGET((reg), 14, 1)
        /* set value  */ #define GX_BP_SET_SU_SIZE_CYLINDRICWRAP(reg, x) ((reg) = GX_BITSET((reg), 14, 1, x))

// USELINEOFS [13:13] (1)
        /* start bit  */ #define GX_BP_SU_SIZE_USELINEOFS_B 13
        /* end bit    */ #define GX_BP_SU_SIZE_USELINEOFS_E 13
        /* bit size   */ #define GX_BP_SU_SIZE_USELINEOFS_SZ 1

        /* raw mask   */ #define GX_BP_SU_SIZE_USELINEOFS_MASK (((1 << 1) - 1) << (31 - 13))
        /* local mask */ #define GX_BP_SU_SIZE_USELINEOFS_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_SU_SIZE_USELINEOFS_SHIFT 18

        /* get value  */ #define GX_BP_GET_SU_SIZE_USELINEOFS(reg) GX_BITGET((reg), 13, 1)
        /* set value  */ #define GX_BP_SET_SU_SIZE_USELINEOFS(reg, x) ((reg) = GX_BITSET((reg), 13, 1, x))

// USEPOINTOFS [12:12] (1)
        /* start bit  */ #define GX_BP_SU_SIZE_USEPOINTOFS_B 12
        /* end bit    */ #define GX_BP_SU_SIZE_USEPOINTOFS_E 12
        /* bit size   */ #define GX_BP_SU_SIZE_USEPOINTOFS_SZ 1

        /* raw mask   */ #define GX_BP_SU_SIZE_USEPOINTOFS_MASK (((1 << 1) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_SU_SIZE_USEPOINTOFS_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_SU_SIZE_USEPOINTOFS_SHIFT 19

        /* get value  */ #define GX_BP_GET_SU_SIZE_USEPOINTOFS(reg) GX_BITGET((reg), 12, 1)
        /* set value  */ #define GX_BP_SET_SU_SIZE_USEPOINTOFS(reg, x) ((reg) = GX_BITSET((reg), 12, 1, x))


/******************************************************************************
 * BP register 0x40 - ZMode
 *****************************************************************************/
// TEST_ENABLE [31:31] (1)
        /* start bit  */ #define GX_BP_ZMODE_TEST_ENABLE_B 31
        /* end bit    */ #define GX_BP_ZMODE_TEST_ENABLE_E 31
        /* bit size   */ #define GX_BP_ZMODE_TEST_ENABLE_SZ 1

        /* raw mask   */ #define GX_BP_ZMODE_TEST_ENABLE_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_ZMODE_TEST_ENABLE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_ZMODE_TEST_ENABLE_SHIFT 0

        /* get value  */ #define GX_BP_GET_ZMODE_TEST_ENABLE(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_BP_SET_ZMODE_TEST_ENABLE(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// COMPARE [28:30] (3)
        /* start bit  */ #define GX_BP_ZMODE_COMPARE_B 28
        /* end bit    */ #define GX_BP_ZMODE_COMPARE_E 30
        /* bit size   */ #define GX_BP_ZMODE_COMPARE_SZ 3

        /* raw mask   */ #define GX_BP_ZMODE_COMPARE_MASK (((1 << 3) - 1) << (31 - 30))
        /* local mask */ #define GX_BP_ZMODE_COMPARE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_ZMODE_COMPARE_SHIFT 1

        /* get value  */ #define GX_BP_GET_ZMODE_COMPARE(reg) GX_BITGET((reg), 28, 3)
        /* set value  */ #define GX_BP_SET_ZMODE_COMPARE(reg, x) ((reg) = GX_BITSET((reg), 28, 3, x))

// UPDATE_ENABLE [27:27] (1)
        /* start bit  */ #define GX_BP_ZMODE_UPDATE_ENABLE_B 27
        /* end bit    */ #define GX_BP_ZMODE_UPDATE_ENABLE_E 27
        /* bit size   */ #define GX_BP_ZMODE_UPDATE_ENABLE_SZ 1

        /* raw mask   */ #define GX_BP_ZMODE_UPDATE_ENABLE_MASK (((1 << 1) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_ZMODE_UPDATE_ENABLE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_ZMODE_UPDATE_ENABLE_SHIFT 4

        /* get value  */ #define GX_BP_GET_ZMODE_UPDATE_ENABLE(reg) GX_BITGET((reg), 27, 1)
        /* set value  */ #define GX_BP_SET_ZMODE_UPDATE_ENABLE(reg, x) ((reg) = GX_BITSET((reg), 27, 1, x))


/******************************************************************************
 * BP register 0x41 - BlendMode
 *****************************************************************************/
// BLEND_ENABLE [31:31] (1)
        /* start bit  */ #define GX_BP_BLENDMODE_BLEND_ENABLE_B 31
        /* end bit    */ #define GX_BP_BLENDMODE_BLEND_ENABLE_E 31
        /* bit size   */ #define GX_BP_BLENDMODE_BLEND_ENABLE_SZ 1

        /* raw mask   */ #define GX_BP_BLENDMODE_BLEND_ENABLE_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_BLENDMODE_BLEND_ENABLE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_BLEND_ENABLE_SHIFT 0

        /* get value  */ #define GX_BP_GET_BLENDMODE_BLEND_ENABLE(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_BP_SET_BLENDMODE_BLEND_ENABLE(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// LOGIC_OP_ENABLE [30:30] (1)
        /* start bit  */ #define GX_BP_BLENDMODE_LOGIC_OP_ENABLE_B 30
        /* end bit    */ #define GX_BP_BLENDMODE_LOGIC_OP_ENABLE_E 30
        /* bit size   */ #define GX_BP_BLENDMODE_LOGIC_OP_ENABLE_SZ 1

        /* raw mask   */ #define GX_BP_BLENDMODE_LOGIC_OP_ENABLE_MASK (((1 << 1) - 1) << (31 - 30))
        /* local mask */ #define GX_BP_BLENDMODE_LOGIC_OP_ENABLE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_LOGIC_OP_ENABLE_SHIFT 1

        /* get value  */ #define GX_BP_GET_BLENDMODE_LOGIC_OP_ENABLE(reg) GX_BITGET((reg), 30, 1)
        /* set value  */ #define GX_BP_SET_BLENDMODE_LOGIC_OP_ENABLE(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// DITHER [29:29] (1)
        /* start bit  */ #define GX_BP_BLENDMODE_DITHER_B 29
        /* end bit    */ #define GX_BP_BLENDMODE_DITHER_E 29
        /* bit size   */ #define GX_BP_BLENDMODE_DITHER_SZ 1

        /* raw mask   */ #define GX_BP_BLENDMODE_DITHER_MASK (((1 << 1) - 1) << (31 - 29))
        /* local mask */ #define GX_BP_BLENDMODE_DITHER_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_DITHER_SHIFT 2

        /* get value  */ #define GX_BP_GET_BLENDMODE_DITHER(reg) GX_BITGET((reg), 29, 1)
        /* set value  */ #define GX_BP_SET_BLENDMODE_DITHER(reg, x) ((reg) = GX_BITSET((reg), 29, 1, x))

// COLOR_UPDATE [28:28] (1)
        /* start bit  */ #define GX_BP_BLENDMODE_COLOR_UPDATE_B 28
        /* end bit    */ #define GX_BP_BLENDMODE_COLOR_UPDATE_E 28
        /* bit size   */ #define GX_BP_BLENDMODE_COLOR_UPDATE_SZ 1

        /* raw mask   */ #define GX_BP_BLENDMODE_COLOR_UPDATE_MASK (((1 << 1) - 1) << (31 - 28))
        /* local mask */ #define GX_BP_BLENDMODE_COLOR_UPDATE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_COLOR_UPDATE_SHIFT 3

        /* get value  */ #define GX_BP_GET_BLENDMODE_COLOR_UPDATE(reg) GX_BITGET((reg), 28, 1)
        /* set value  */ #define GX_BP_SET_BLENDMODE_COLOR_UPDATE(reg, x) ((reg) = GX_BITSET((reg), 28, 1, x))

// ALPHA_UPDATE [27:27] (1)
        /* start bit  */ #define GX_BP_BLENDMODE_ALPHA_UPDATE_B 27
        /* end bit    */ #define GX_BP_BLENDMODE_ALPHA_UPDATE_E 27
        /* bit size   */ #define GX_BP_BLENDMODE_ALPHA_UPDATE_SZ 1

        /* raw mask   */ #define GX_BP_BLENDMODE_ALPHA_UPDATE_MASK (((1 << 1) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_BLENDMODE_ALPHA_UPDATE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_ALPHA_UPDATE_SHIFT 4

        /* get value  */ #define GX_BP_GET_BLENDMODE_ALPHA_UPDATE(reg) GX_BITGET((reg), 27, 1)
        /* set value  */ #define GX_BP_SET_BLENDMODE_ALPHA_UPDATE(reg, x) ((reg) = GX_BITSET((reg), 27, 1, x))

// DST_FACTOR [24:26] (3)
        /* start bit  */ #define GX_BP_BLENDMODE_DST_FACTOR_B 24
        /* end bit    */ #define GX_BP_BLENDMODE_DST_FACTOR_E 26
        /* bit size   */ #define GX_BP_BLENDMODE_DST_FACTOR_SZ 3

        /* raw mask   */ #define GX_BP_BLENDMODE_DST_FACTOR_MASK (((1 << 3) - 1) << (31 - 26))
        /* local mask */ #define GX_BP_BLENDMODE_DST_FACTOR_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_DST_FACTOR_SHIFT 5

        /* get value  */ #define GX_BP_GET_BLENDMODE_DST_FACTOR(reg) GX_BITGET((reg), 24, 3)
        /* set value  */ #define GX_BP_SET_BLENDMODE_DST_FACTOR(reg, x) ((reg) = GX_BITSET((reg), 24, 3, x))

// SRC_FACTOR [21:23] (3)
        /* start bit  */ #define GX_BP_BLENDMODE_SRC_FACTOR_B 21
        /* end bit    */ #define GX_BP_BLENDMODE_SRC_FACTOR_E 23
        /* bit size   */ #define GX_BP_BLENDMODE_SRC_FACTOR_SZ 3

        /* raw mask   */ #define GX_BP_BLENDMODE_SRC_FACTOR_MASK (((1 << 3) - 1) << (31 - 23))
        /* local mask */ #define GX_BP_BLENDMODE_SRC_FACTOR_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_SRC_FACTOR_SHIFT 8

        /* get value  */ #define GX_BP_GET_BLENDMODE_SRC_FACTOR(reg) GX_BITGET((reg), 21, 3)
        /* set value  */ #define GX_BP_SET_BLENDMODE_SRC_FACTOR(reg, x) ((reg) = GX_BITSET((reg), 21, 3, x))

// SUBTRACT [20:20] (1)
        /* start bit  */ #define GX_BP_BLENDMODE_SUBTRACT_B 20
        /* end bit    */ #define GX_BP_BLENDMODE_SUBTRACT_E 20
        /* bit size   */ #define GX_BP_BLENDMODE_SUBTRACT_SZ 1

        /* raw mask   */ #define GX_BP_BLENDMODE_SUBTRACT_MASK (((1 << 1) - 1) << (31 - 20))
        /* local mask */ #define GX_BP_BLENDMODE_SUBTRACT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_SUBTRACT_SHIFT 11

        /* get value  */ #define GX_BP_GET_BLENDMODE_SUBTRACT(reg) GX_BITGET((reg), 20, 1)
        /* set value  */ #define GX_BP_SET_BLENDMODE_SUBTRACT(reg, x) ((reg) = GX_BITSET((reg), 20, 1, x))

// LOGIC_MODE [16:19] (4)
        /* start bit  */ #define GX_BP_BLENDMODE_LOGIC_MODE_B 16
        /* end bit    */ #define GX_BP_BLENDMODE_LOGIC_MODE_E 19
        /* bit size   */ #define GX_BP_BLENDMODE_LOGIC_MODE_SZ 4

        /* raw mask   */ #define GX_BP_BLENDMODE_LOGIC_MODE_MASK (((1 << 4) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_BLENDMODE_LOGIC_MODE_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_BLENDMODE_LOGIC_MODE_SHIFT 12

        /* get value  */ #define GX_BP_GET_BLENDMODE_LOGIC_MODE(reg) GX_BITGET((reg), 16, 4)
        /* set value  */ #define GX_BP_SET_BLENDMODE_LOGIC_MODE(reg, x) ((reg) = GX_BITSET((reg), 16, 4, x))


/******************************************************************************
 * BP register 0x42 - DstAlpha
 *****************************************************************************/
// ALPHA [24:31] (8)
        /* start bit  */ #define GX_BP_DSTALPHA_ALPHA_B 24
        /* end bit    */ #define GX_BP_DSTALPHA_ALPHA_E 31
        /* bit size   */ #define GX_BP_DSTALPHA_ALPHA_SZ 8

        /* raw mask   */ #define GX_BP_DSTALPHA_ALPHA_MASK (((1 << 8) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_DSTALPHA_ALPHA_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_DSTALPHA_ALPHA_SHIFT 0

        /* get value  */ #define GX_BP_GET_DSTALPHA_ALPHA(reg) GX_BITGET((reg), 24, 8)
        /* set value  */ #define GX_BP_SET_DSTALPHA_ALPHA(reg, x) ((reg) = GX_BITSET((reg), 24, 8, x))

// ENABLE [23:23] (1)
        /* start bit  */ #define GX_BP_DSTALPHA_ENABLE_B 23
        /* end bit    */ #define GX_BP_DSTALPHA_ENABLE_E 23
        /* bit size   */ #define GX_BP_DSTALPHA_ENABLE_SZ 1

        /* raw mask   */ #define GX_BP_DSTALPHA_ENABLE_MASK (((1 << 1) - 1) << (31 - 23))
        /* local mask */ #define GX_BP_DSTALPHA_ENABLE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_DSTALPHA_ENABLE_SHIFT 8

        /* get value  */ #define GX_BP_GET_DSTALPHA_ENABLE(reg) GX_BITGET((reg), 23, 1)
        /* set value  */ #define GX_BP_SET_DSTALPHA_ENABLE(reg, x) ((reg) = GX_BITSET((reg), 23, 1, x))

// YUV_FMT [21:22] (2)
        /* start bit  */ #define GX_BP_DSTALPHA_YUV_FMT_B 21
        /* end bit    */ #define GX_BP_DSTALPHA_YUV_FMT_E 22
        /* bit size   */ #define GX_BP_DSTALPHA_YUV_FMT_SZ 2

        /* raw mask   */ #define GX_BP_DSTALPHA_YUV_FMT_MASK (((1 << 2) - 1) << (31 - 22))
        /* local mask */ #define GX_BP_DSTALPHA_YUV_FMT_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_DSTALPHA_YUV_FMT_SHIFT 9

        /* get value  */ #define GX_BP_GET_DSTALPHA_YUV_FMT(reg) GX_BITGET((reg), 21, 2)
        /* set value  */ #define GX_BP_SET_DSTALPHA_YUV_FMT(reg, x) ((reg) = GX_BITSET((reg), 21, 2, x))


/******************************************************************************
 * BP register 0x43 - ZControl
 *****************************************************************************/
// PIXEL_FMT [29:31] (3)
        /* start bit  */ #define GX_BP_ZCONTROL_PIXEL_FMT_B 29
        /* end bit    */ #define GX_BP_ZCONTROL_PIXEL_FMT_E 31
        /* bit size   */ #define GX_BP_ZCONTROL_PIXEL_FMT_SZ 3

        /* raw mask   */ #define GX_BP_ZCONTROL_PIXEL_FMT_MASK (((1 << 3) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_ZCONTROL_PIXEL_FMT_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_ZCONTROL_PIXEL_FMT_SHIFT 0

        /* get value  */ #define GX_BP_GET_ZCONTROL_PIXEL_FMT(reg) GX_BITGET((reg), 29, 3)
        /* set value  */ #define GX_BP_SET_ZCONTROL_PIXEL_FMT(reg, x) ((reg) = GX_BITSET((reg), 29, 3, x))

// Z_FMT [26:28] (3)
        /* start bit  */ #define GX_BP_ZCONTROL_Z_FMT_B 26
        /* end bit    */ #define GX_BP_ZCONTROL_Z_FMT_E 28
        /* bit size   */ #define GX_BP_ZCONTROL_Z_FMT_SZ 3

        /* raw mask   */ #define GX_BP_ZCONTROL_Z_FMT_MASK (((1 << 3) - 1) << (31 - 28))
        /* local mask */ #define GX_BP_ZCONTROL_Z_FMT_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_ZCONTROL_Z_FMT_SHIFT 3

        /* get value  */ #define GX_BP_GET_ZCONTROL_Z_FMT(reg) GX_BITGET((reg), 26, 3)
        /* set value  */ #define GX_BP_SET_ZCONTROL_Z_FMT(reg, x) ((reg) = GX_BITSET((reg), 26, 3, x))

// BEFORE_TEX [25:25] (1) - Determines whether Z-buffering occurs before or after texturing
        /* start bit  */ #define GX_BP_ZCONTROL_BEFORE_TEX_B 25
        /* end bit    */ #define GX_BP_ZCONTROL_BEFORE_TEX_E 25
        /* bit size   */ #define GX_BP_ZCONTROL_BEFORE_TEX_SZ 1

        /* raw mask   */ #define GX_BP_ZCONTROL_BEFORE_TEX_MASK (((1 << 1) - 1) << (31 - 25))
        /* local mask */ #define GX_BP_ZCONTROL_BEFORE_TEX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_ZCONTROL_BEFORE_TEX_SHIFT 6

        /* get value  */ #define GX_BP_GET_ZCONTROL_BEFORE_TEX(reg) GX_BITGET((reg), 25, 1)
        /* set value  */ #define GX_BP_SET_ZCONTROL_BEFORE_TEX(reg, x) ((reg) = GX_BITSET((reg), 25, 1, x))


/******************************************************************************
 * BP register 0x44 - FieldMask
 *****************************************************************************/
// ODD [31:31] (1) - Whether to write odd fields to the EFB
        /* start bit  */ #define GX_BP_FIELDMASK_ODD_B 31
        /* end bit    */ #define GX_BP_FIELDMASK_ODD_E 31
        /* bit size   */ #define GX_BP_FIELDMASK_ODD_SZ 1

        /* raw mask   */ #define GX_BP_FIELDMASK_ODD_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FIELDMASK_ODD_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_FIELDMASK_ODD_SHIFT 0

        /* get value  */ #define GX_BP_GET_FIELDMASK_ODD(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_BP_SET_FIELDMASK_ODD(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// EVEN [30:30] (1) - Whether to write even fields to the EFB
        /* start bit  */ #define GX_BP_FIELDMASK_EVEN_B 30
        /* end bit    */ #define GX_BP_FIELDMASK_EVEN_E 30
        /* bit size   */ #define GX_BP_FIELDMASK_EVEN_SZ 1

        /* raw mask   */ #define GX_BP_FIELDMASK_EVEN_MASK (((1 << 1) - 1) << (31 - 30))
        /* local mask */ #define GX_BP_FIELDMASK_EVEN_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_FIELDMASK_EVEN_SHIFT 1

        /* get value  */ #define GX_BP_GET_FIELDMASK_EVEN(reg) GX_BITGET((reg), 30, 1)
        /* set value  */ #define GX_BP_SET_FIELDMASK_EVEN(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))


/******************************************************************************
 * BP register 0x59 - ScissorOffset
 *****************************************************************************/
// OX [22:31] (10)
        /* start bit  */ #define GX_BP_SCISSOROFFSET_OX_B 22
        /* end bit    */ #define GX_BP_SCISSOROFFSET_OX_E 31
        /* bit size   */ #define GX_BP_SCISSOROFFSET_OX_SZ 10

        /* raw mask   */ #define GX_BP_SCISSOROFFSET_OX_MASK (((1 << 10) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_SCISSOROFFSET_OX_LMASK ((1 << 10) - 1)
        /* bit shift  */ #define GX_BP_SCISSOROFFSET_OX_SHIFT 0

        /* get value  */ #define GX_BP_GET_SCISSOROFFSET_OX(reg) GX_BITGET((reg), 22, 10)
        /* set value  */ #define GX_BP_SET_SCISSOROFFSET_OX(reg, x) ((reg) = GX_BITSET((reg), 22, 10, x))

// OY [12:21] (10)
        /* start bit  */ #define GX_BP_SCISSOROFFSET_OY_B 12
        /* end bit    */ #define GX_BP_SCISSOROFFSET_OY_E 21
        /* bit size   */ #define GX_BP_SCISSOROFFSET_OY_SZ 10

        /* raw mask   */ #define GX_BP_SCISSOROFFSET_OY_MASK (((1 << 10) - 1) << (31 - 21))
        /* local mask */ #define GX_BP_SCISSOROFFSET_OY_LMASK ((1 << 10) - 1)
        /* bit shift  */ #define GX_BP_SCISSOROFFSET_OY_SHIFT 10

        /* get value  */ #define GX_BP_GET_SCISSOROFFSET_OY(reg) GX_BITGET((reg), 12, 10)
        /* set value  */ #define GX_BP_SET_SCISSOROFFSET_OY(reg, x) ((reg) = GX_BITSET((reg), 12, 10, x))


/******************************************************************************
 * BP register 0x68 - FieldMode
 *****************************************************************************/
// TEX_LOD [31:31] (1) - Adjust vertex tex LOD computation to account for interlacing
        /* start bit  */ #define GX_BP_FIELDMODE_TEX_LOD_B 31
        /* end bit    */ #define GX_BP_FIELDMODE_TEX_LOD_E 31
        /* bit size   */ #define GX_BP_FIELDMODE_TEX_LOD_SZ 1

        /* raw mask   */ #define GX_BP_FIELDMODE_TEX_LOD_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FIELDMODE_TEX_LOD_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_FIELDMODE_TEX_LOD_SHIFT 0

        /* get value  */ #define GX_BP_GET_FIELDMODE_TEX_LOD(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_BP_SET_FIELDMODE_TEX_LOD(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))


/******************************************************************************
 * BP structure - TevColorCombiner
 *****************************************************************************/
// D [28:31] (4)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_D_B 28
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_D_E 31
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_D_SZ 4

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_D_MASK (((1 << 4) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_D_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_D_SHIFT 0

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_D(reg) GX_BITGET((reg), 28, 4)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_D(reg, x) ((reg) = GX_BITSET((reg), 28, 4, x))

// C [24:27] (4)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_C_B 24
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_C_E 27
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_C_SZ 4

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_C_MASK (((1 << 4) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_C_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_C_SHIFT 4

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_C(reg) GX_BITGET((reg), 24, 4)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_C(reg, x) ((reg) = GX_BITSET((reg), 24, 4, x))

// B [20:23] (4)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_B_B 20
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_B_E 23
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_B_SZ 4

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_B_MASK (((1 << 4) - 1) << (31 - 23))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_B_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_B_SHIFT 8

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_B(reg) GX_BITGET((reg), 20, 4)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_B(reg, x) ((reg) = GX_BITSET((reg), 20, 4, x))

// A [16:19] (4)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_A_B 16
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_A_E 19
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_A_SZ 4

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_A_MASK (((1 << 4) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_A_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_A_SHIFT 12

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_A(reg) GX_BITGET((reg), 16, 4)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_A(reg, x) ((reg) = GX_BITSET((reg), 16, 4, x))

// BIAS [14:15] (2)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_BIAS_B 14
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_BIAS_E 15
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_BIAS_SZ 2

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_BIAS_MASK (((1 << 2) - 1) << (31 - 15))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_BIAS_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_BIAS_SHIFT 16

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_BIAS(reg) GX_BITGET((reg), 14, 2)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_BIAS(reg, x) ((reg) = GX_BITSET((reg), 14, 2, x))

// OP_OR_COMPARISON [13:13] (1)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_B 13
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_E 13
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_SZ 1

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_MASK (((1 << 1) - 1) << (31 - 13))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_OP_OR_COMPARISON_SHIFT 18

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_OP_OR_COMPARISON(reg) GX_BITGET((reg), 13, 1)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_OP_OR_COMPARISON(reg, x) ((reg) = GX_BITSET((reg), 13, 1, x))

// CLAMP [12:12] (1)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_CLAMP_B 12
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_CLAMP_E 12
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_CLAMP_SZ 1

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_CLAMP_MASK (((1 << 1) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_CLAMP_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_CLAMP_SHIFT 19

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_CLAMP(reg) GX_BITGET((reg), 12, 1)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_CLAMP(reg, x) ((reg) = GX_BITSET((reg), 12, 1, x))

// SCALE_OR_COMPARE_MODE [10:11] (2)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_B 10
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_E 11
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_SZ 2

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_MASK (((1 << 2) - 1) << (31 - 11))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE_SHIFT 20

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE(reg) GX_BITGET((reg), 10, 2)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_SCALE_OR_COMPARE_MODE(reg, x) ((reg) = GX_BITSET((reg), 10, 2, x))

// DEST [8:9] (2)
        /* start bit  */ #define GX_BP_TEVCOLORCOMBINER_DEST_B 8
        /* end bit    */ #define GX_BP_TEVCOLORCOMBINER_DEST_E 9
        /* bit size   */ #define GX_BP_TEVCOLORCOMBINER_DEST_SZ 2

        /* raw mask   */ #define GX_BP_TEVCOLORCOMBINER_DEST_MASK (((1 << 2) - 1) << (31 - 9))
        /* local mask */ #define GX_BP_TEVCOLORCOMBINER_DEST_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_TEVCOLORCOMBINER_DEST_SHIFT 22

        /* get value  */ #define GX_BP_GET_TEVCOLORCOMBINER_DEST(reg) GX_BITGET((reg), 8, 2)
        /* set value  */ #define GX_BP_SET_TEVCOLORCOMBINER_DEST(reg, x) ((reg) = GX_BITSET((reg), 8, 2, x))


/******************************************************************************
 * BP structure - TevRegLo
 *****************************************************************************/
// RED [21:31] (11)
        /* start bit  */ #define GX_BP_TEVREGLO_RED_B 21
        /* end bit    */ #define GX_BP_TEVREGLO_RED_E 31
        /* bit size   */ #define GX_BP_TEVREGLO_RED_SZ 11

        /* raw mask   */ #define GX_BP_TEVREGLO_RED_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_TEVREGLO_RED_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_TEVREGLO_RED_SHIFT 0

        /* get value  */ #define GX_BP_GET_TEVREGLO_RED(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_TEVREGLO_RED(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// ALPHA [9:19] (11)
        /* start bit  */ #define GX_BP_TEVREGLO_ALPHA_B 9
        /* end bit    */ #define GX_BP_TEVREGLO_ALPHA_E 19
        /* bit size   */ #define GX_BP_TEVREGLO_ALPHA_SZ 11

        /* raw mask   */ #define GX_BP_TEVREGLO_ALPHA_MASK (((1 << 11) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_TEVREGLO_ALPHA_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_TEVREGLO_ALPHA_SHIFT 12

        /* get value  */ #define GX_BP_GET_TEVREGLO_ALPHA(reg) GX_BITGET((reg), 9, 11)
        /* set value  */ #define GX_BP_SET_TEVREGLO_ALPHA(reg, x) ((reg) = GX_BITSET((reg), 9, 11, x))

// TYPE [8:8] (1)
        /* start bit  */ #define GX_BP_TEVREGLO_TYPE_B 8
        /* end bit    */ #define GX_BP_TEVREGLO_TYPE_E 8
        /* bit size   */ #define GX_BP_TEVREGLO_TYPE_SZ 1

        /* raw mask   */ #define GX_BP_TEVREGLO_TYPE_MASK (((1 << 1) - 1) << (31 - 8))
        /* local mask */ #define GX_BP_TEVREGLO_TYPE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_TEVREGLO_TYPE_SHIFT 23

        /* get value  */ #define GX_BP_GET_TEVREGLO_TYPE(reg) GX_BITGET((reg), 8, 1)
        /* set value  */ #define GX_BP_SET_TEVREGLO_TYPE(reg, x) ((reg) = GX_BITSET((reg), 8, 1, x))


/******************************************************************************
 * BP structure - TevRegHi
 *****************************************************************************/
// BLUE [21:31] (11)
        /* start bit  */ #define GX_BP_TEVREGHI_BLUE_B 21
        /* end bit    */ #define GX_BP_TEVREGHI_BLUE_E 31
        /* bit size   */ #define GX_BP_TEVREGHI_BLUE_SZ 11

        /* raw mask   */ #define GX_BP_TEVREGHI_BLUE_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_TEVREGHI_BLUE_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_TEVREGHI_BLUE_SHIFT 0

        /* get value  */ #define GX_BP_GET_TEVREGHI_BLUE(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_TEVREGHI_BLUE(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// GREEN [9:19] (11)
        /* start bit  */ #define GX_BP_TEVREGHI_GREEN_B 9
        /* end bit    */ #define GX_BP_TEVREGHI_GREEN_E 19
        /* bit size   */ #define GX_BP_TEVREGHI_GREEN_SZ 11

        /* raw mask   */ #define GX_BP_TEVREGHI_GREEN_MASK (((1 << 11) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_TEVREGHI_GREEN_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_TEVREGHI_GREEN_SHIFT 12

        /* get value  */ #define GX_BP_GET_TEVREGHI_GREEN(reg) GX_BITGET((reg), 9, 11)
        /* set value  */ #define GX_BP_SET_TEVREGHI_GREEN(reg, x) ((reg) = GX_BITSET((reg), 9, 11, x))

// TYPE [8:8] (1)
        /* start bit  */ #define GX_BP_TEVREGHI_TYPE_B 8
        /* end bit    */ #define GX_BP_TEVREGHI_TYPE_E 8
        /* bit size   */ #define GX_BP_TEVREGHI_TYPE_SZ 1

        /* raw mask   */ #define GX_BP_TEVREGHI_TYPE_MASK (((1 << 1) - 1) << (31 - 8))
        /* local mask */ #define GX_BP_TEVREGHI_TYPE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_TEVREGHI_TYPE_SHIFT 23

        /* get value  */ #define GX_BP_GET_TEVREGHI_TYPE(reg) GX_BITGET((reg), 8, 1)
        /* set value  */ #define GX_BP_SET_TEVREGHI_TYPE(reg, x) ((reg) = GX_BITSET((reg), 8, 1, x))


/******************************************************************************
 * BP register 0xE8 - FogRange
 *****************************************************************************/
// CENTER [22:31] (10)
        /* start bit  */ #define GX_BP_FOGRANGE_CENTER_B 22
        /* end bit    */ #define GX_BP_FOGRANGE_CENTER_E 31
        /* bit size   */ #define GX_BP_FOGRANGE_CENTER_SZ 10

        /* raw mask   */ #define GX_BP_FOGRANGE_CENTER_MASK (((1 << 10) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FOGRANGE_CENTER_LMASK ((1 << 10) - 1)
        /* bit shift  */ #define GX_BP_FOGRANGE_CENTER_SHIFT 0

        /* get value  */ #define GX_BP_GET_FOGRANGE_CENTER(reg) GX_BITGET((reg), 22, 10)
        /* set value  */ #define GX_BP_SET_FOGRANGE_CENTER(reg, x) ((reg) = GX_BITSET((reg), 22, 10, x))

// ENABLED [21:21] (1)
        /* start bit  */ #define GX_BP_FOGRANGE_ENABLED_B 21
        /* end bit    */ #define GX_BP_FOGRANGE_ENABLED_E 21
        /* bit size   */ #define GX_BP_FOGRANGE_ENABLED_SZ 1

        /* raw mask   */ #define GX_BP_FOGRANGE_ENABLED_MASK (((1 << 1) - 1) << (31 - 21))
        /* local mask */ #define GX_BP_FOGRANGE_ENABLED_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_FOGRANGE_ENABLED_SHIFT 10

        /* get value  */ #define GX_BP_GET_FOGRANGE_ENABLED(reg) GX_BITGET((reg), 21, 1)
        /* set value  */ #define GX_BP_SET_FOGRANGE_ENABLED(reg, x) ((reg) = GX_BITSET((reg), 21, 1, x))


/******************************************************************************
 * BP structure - FogRangeK
 *****************************************************************************/
// HI [20:31] (12)
        /* start bit  */ #define GX_BP_FOGRANGEK_HI_B 20
        /* end bit    */ #define GX_BP_FOGRANGEK_HI_E 31
        /* bit size   */ #define GX_BP_FOGRANGEK_HI_SZ 12

        /* raw mask   */ #define GX_BP_FOGRANGEK_HI_MASK (((1 << 12) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FOGRANGEK_HI_LMASK ((1 << 12) - 1)
        /* bit shift  */ #define GX_BP_FOGRANGEK_HI_SHIFT 0

        /* get value  */ #define GX_BP_GET_FOGRANGEK_HI(reg) GX_BITGET((reg), 20, 12)
        /* set value  */ #define GX_BP_SET_FOGRANGEK_HI(reg, x) ((reg) = GX_BITSET((reg), 20, 12, x))

// LO [8:19] (12)
        /* start bit  */ #define GX_BP_FOGRANGEK_LO_B 8
        /* end bit    */ #define GX_BP_FOGRANGEK_LO_E 19
        /* bit size   */ #define GX_BP_FOGRANGEK_LO_SZ 12

        /* raw mask   */ #define GX_BP_FOGRANGEK_LO_MASK (((1 << 12) - 1) << (31 - 19))
        /* local mask */ #define GX_BP_FOGRANGEK_LO_LMASK ((1 << 12) - 1)
        /* bit shift  */ #define GX_BP_FOGRANGEK_LO_SHIFT 12

        /* get value  */ #define GX_BP_GET_FOGRANGEK_LO(reg) GX_BITGET((reg), 8, 12)
        /* set value  */ #define GX_BP_SET_FOGRANGEK_LO(reg, x) ((reg) = GX_BITSET((reg), 8, 12, x))


/******************************************************************************
 * BP register 0xEE - FogParam0
 *****************************************************************************/
// A_MANT [21:31] (11)
        /* start bit  */ #define GX_BP_FOGPARAM0_A_MANT_B 21
        /* end bit    */ #define GX_BP_FOGPARAM0_A_MANT_E 31
        /* bit size   */ #define GX_BP_FOGPARAM0_A_MANT_SZ 11

        /* raw mask   */ #define GX_BP_FOGPARAM0_A_MANT_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FOGPARAM0_A_MANT_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM0_A_MANT_SHIFT 0

        /* get value  */ #define GX_BP_GET_FOGPARAM0_A_MANT(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_FOGPARAM0_A_MANT(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// A_EXP [13:20] (8)
        /* start bit  */ #define GX_BP_FOGPARAM0_A_EXP_B 13
        /* end bit    */ #define GX_BP_FOGPARAM0_A_EXP_E 20
        /* bit size   */ #define GX_BP_FOGPARAM0_A_EXP_SZ 8

        /* raw mask   */ #define GX_BP_FOGPARAM0_A_EXP_MASK (((1 << 8) - 1) << (31 - 20))
        /* local mask */ #define GX_BP_FOGPARAM0_A_EXP_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM0_A_EXP_SHIFT 11

        /* get value  */ #define GX_BP_GET_FOGPARAM0_A_EXP(reg) GX_BITGET((reg), 13, 8)
        /* set value  */ #define GX_BP_SET_FOGPARAM0_A_EXP(reg, x) ((reg) = GX_BITSET((reg), 13, 8, x))

// A_SIGN [12:12] (1)
        /* start bit  */ #define GX_BP_FOGPARAM0_A_SIGN_B 12
        /* end bit    */ #define GX_BP_FOGPARAM0_A_SIGN_E 12
        /* bit size   */ #define GX_BP_FOGPARAM0_A_SIGN_SZ 1

        /* raw mask   */ #define GX_BP_FOGPARAM0_A_SIGN_MASK (((1 << 1) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_FOGPARAM0_A_SIGN_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM0_A_SIGN_SHIFT 19

        /* get value  */ #define GX_BP_GET_FOGPARAM0_A_SIGN(reg) GX_BITGET((reg), 12, 1)
        /* set value  */ #define GX_BP_SET_FOGPARAM0_A_SIGN(reg, x) ((reg) = GX_BITSET((reg), 12, 1, x))


/******************************************************************************
 * BP register 0xEF - FogParam1
 *****************************************************************************/
// B_MAG [8:31] (24)
        /* start bit  */ #define GX_BP_FOGPARAM1_B_MAG_B 8
        /* end bit    */ #define GX_BP_FOGPARAM1_B_MAG_E 31
        /* bit size   */ #define GX_BP_FOGPARAM1_B_MAG_SZ 24

        /* raw mask   */ #define GX_BP_FOGPARAM1_B_MAG_MASK (((1 << 24) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FOGPARAM1_B_MAG_LMASK ((1 << 24) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM1_B_MAG_SHIFT 0

        /* get value  */ #define GX_BP_GET_FOGPARAM1_B_MAG(reg) GX_BITGET((reg), 8, 24)
        /* set value  */ #define GX_BP_SET_FOGPARAM1_B_MAG(reg, x) ((reg) = GX_BITSET((reg), 8, 24, x))


/******************************************************************************
 * BP register 0xF0 - FogParam2
 *****************************************************************************/
// B_SHIFT [27:31] (5)
        /* start bit  */ #define GX_BP_FOGPARAM2_B_SHIFT_B 27
        /* end bit    */ #define GX_BP_FOGPARAM2_B_SHIFT_E 31
        /* bit size   */ #define GX_BP_FOGPARAM2_B_SHIFT_SZ 5

        /* raw mask   */ #define GX_BP_FOGPARAM2_B_SHIFT_MASK (((1 << 5) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FOGPARAM2_B_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM2_B_SHIFT_SHIFT 0

        /* get value  */ #define GX_BP_GET_FOGPARAM2_B_SHIFT(reg) GX_BITGET((reg), 27, 5)
        /* set value  */ #define GX_BP_SET_FOGPARAM2_B_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 27, 5, x))


/******************************************************************************
 * BP register 0xF1 - FogParam3
 *****************************************************************************/
// C_MANT [21:31] (11)
        /* start bit  */ #define GX_BP_FOGPARAM3_C_MANT_B 21
        /* end bit    */ #define GX_BP_FOGPARAM3_C_MANT_E 31
        /* bit size   */ #define GX_BP_FOGPARAM3_C_MANT_SZ 11

        /* raw mask   */ #define GX_BP_FOGPARAM3_C_MANT_MASK (((1 << 11) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FOGPARAM3_C_MANT_LMASK ((1 << 11) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM3_C_MANT_SHIFT 0

        /* get value  */ #define GX_BP_GET_FOGPARAM3_C_MANT(reg) GX_BITGET((reg), 21, 11)
        /* set value  */ #define GX_BP_SET_FOGPARAM3_C_MANT(reg, x) ((reg) = GX_BITSET((reg), 21, 11, x))

// C_EXP [13:20] (8)
        /* start bit  */ #define GX_BP_FOGPARAM3_C_EXP_B 13
        /* end bit    */ #define GX_BP_FOGPARAM3_C_EXP_E 20
        /* bit size   */ #define GX_BP_FOGPARAM3_C_EXP_SZ 8

        /* raw mask   */ #define GX_BP_FOGPARAM3_C_EXP_MASK (((1 << 8) - 1) << (31 - 20))
        /* local mask */ #define GX_BP_FOGPARAM3_C_EXP_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM3_C_EXP_SHIFT 11

        /* get value  */ #define GX_BP_GET_FOGPARAM3_C_EXP(reg) GX_BITGET((reg), 13, 8)
        /* set value  */ #define GX_BP_SET_FOGPARAM3_C_EXP(reg, x) ((reg) = GX_BITSET((reg), 13, 8, x))

// C_SIGN [12:12] (1)
        /* start bit  */ #define GX_BP_FOGPARAM3_C_SIGN_B 12
        /* end bit    */ #define GX_BP_FOGPARAM3_C_SIGN_E 12
        /* bit size   */ #define GX_BP_FOGPARAM3_C_SIGN_SZ 1

        /* raw mask   */ #define GX_BP_FOGPARAM3_C_SIGN_MASK (((1 << 1) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_FOGPARAM3_C_SIGN_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM3_C_SIGN_SHIFT 19

        /* get value  */ #define GX_BP_GET_FOGPARAM3_C_SIGN(reg) GX_BITGET((reg), 12, 1)
        /* set value  */ #define GX_BP_SET_FOGPARAM3_C_SIGN(reg, x) ((reg) = GX_BITSET((reg), 12, 1, x))

// PROJ [11:11] (1)
        /* start bit  */ #define GX_BP_FOGPARAM3_PROJ_B 11
        /* end bit    */ #define GX_BP_FOGPARAM3_PROJ_E 11
        /* bit size   */ #define GX_BP_FOGPARAM3_PROJ_SZ 1

        /* raw mask   */ #define GX_BP_FOGPARAM3_PROJ_MASK (((1 << 1) - 1) << (31 - 11))
        /* local mask */ #define GX_BP_FOGPARAM3_PROJ_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM3_PROJ_SHIFT 20

        /* get value  */ #define GX_BP_GET_FOGPARAM3_PROJ(reg) GX_BITGET((reg), 11, 1)
        /* set value  */ #define GX_BP_SET_FOGPARAM3_PROJ(reg, x) ((reg) = GX_BITSET((reg), 11, 1, x))

// FSEL [8:10] (3)
        /* start bit  */ #define GX_BP_FOGPARAM3_FSEL_B 8
        /* end bit    */ #define GX_BP_FOGPARAM3_FSEL_E 10
        /* bit size   */ #define GX_BP_FOGPARAM3_FSEL_SZ 3

        /* raw mask   */ #define GX_BP_FOGPARAM3_FSEL_MASK (((1 << 3) - 1) << (31 - 10))
        /* local mask */ #define GX_BP_FOGPARAM3_FSEL_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_FOGPARAM3_FSEL_SHIFT 21

        /* get value  */ #define GX_BP_GET_FOGPARAM3_FSEL(reg) GX_BITGET((reg), 8, 3)
        /* set value  */ #define GX_BP_SET_FOGPARAM3_FSEL(reg, x) ((reg) = GX_BITSET((reg), 8, 3, x))


/******************************************************************************
 * BP register 0xF2 - FogColor
 *****************************************************************************/
// RGB [8:31] (24)
        /* start bit  */ #define GX_BP_FOGCOLOR_RGB_B 8
        /* end bit    */ #define GX_BP_FOGCOLOR_RGB_E 31
        /* bit size   */ #define GX_BP_FOGCOLOR_RGB_SZ 24

        /* raw mask   */ #define GX_BP_FOGCOLOR_RGB_MASK (((1 << 24) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_FOGCOLOR_RGB_LMASK ((1 << 24) - 1)
        /* bit shift  */ #define GX_BP_FOGCOLOR_RGB_SHIFT 0

        /* get value  */ #define GX_BP_GET_FOGCOLOR_RGB(reg) GX_BITGET((reg), 8, 24)
        /* set value  */ #define GX_BP_SET_FOGCOLOR_RGB(reg, x) ((reg) = GX_BITSET((reg), 8, 24, x))


/******************************************************************************
 * BP register 0xF3 - AlphaCompare
 *****************************************************************************/
// REF0 [24:31] (8)
        /* start bit  */ #define GX_BP_ALPHACOMPARE_REF0_B 24
        /* end bit    */ #define GX_BP_ALPHACOMPARE_REF0_E 31
        /* bit size   */ #define GX_BP_ALPHACOMPARE_REF0_SZ 8

        /* raw mask   */ #define GX_BP_ALPHACOMPARE_REF0_MASK (((1 << 8) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_ALPHACOMPARE_REF0_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_ALPHACOMPARE_REF0_SHIFT 0

        /* get value  */ #define GX_BP_GET_ALPHACOMPARE_REF0(reg) GX_BITGET((reg), 24, 8)
        /* set value  */ #define GX_BP_SET_ALPHACOMPARE_REF0(reg, x) ((reg) = GX_BITSET((reg), 24, 8, x))

// REF1 [16:23] (8)
        /* start bit  */ #define GX_BP_ALPHACOMPARE_REF1_B 16
        /* end bit    */ #define GX_BP_ALPHACOMPARE_REF1_E 23
        /* bit size   */ #define GX_BP_ALPHACOMPARE_REF1_SZ 8

        /* raw mask   */ #define GX_BP_ALPHACOMPARE_REF1_MASK (((1 << 8) - 1) << (31 - 23))
        /* local mask */ #define GX_BP_ALPHACOMPARE_REF1_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_BP_ALPHACOMPARE_REF1_SHIFT 8

        /* get value  */ #define GX_BP_GET_ALPHACOMPARE_REF1(reg) GX_BITGET((reg), 16, 8)
        /* set value  */ #define GX_BP_SET_ALPHACOMPARE_REF1(reg, x) ((reg) = GX_BITSET((reg), 16, 8, x))

// COMP0 [13:15] (3)
        /* start bit  */ #define GX_BP_ALPHACOMPARE_COMP0_B 13
        /* end bit    */ #define GX_BP_ALPHACOMPARE_COMP0_E 15
        /* bit size   */ #define GX_BP_ALPHACOMPARE_COMP0_SZ 3

        /* raw mask   */ #define GX_BP_ALPHACOMPARE_COMP0_MASK (((1 << 3) - 1) << (31 - 15))
        /* local mask */ #define GX_BP_ALPHACOMPARE_COMP0_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_ALPHACOMPARE_COMP0_SHIFT 16

        /* get value  */ #define GX_BP_GET_ALPHACOMPARE_COMP0(reg) GX_BITGET((reg), 13, 3)
        /* set value  */ #define GX_BP_SET_ALPHACOMPARE_COMP0(reg, x) ((reg) = GX_BITSET((reg), 13, 3, x))

// COMP1 [10:12] (3)
        /* start bit  */ #define GX_BP_ALPHACOMPARE_COMP1_B 10
        /* end bit    */ #define GX_BP_ALPHACOMPARE_COMP1_E 12
        /* bit size   */ #define GX_BP_ALPHACOMPARE_COMP1_SZ 3

        /* raw mask   */ #define GX_BP_ALPHACOMPARE_COMP1_MASK (((1 << 3) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_ALPHACOMPARE_COMP1_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_BP_ALPHACOMPARE_COMP1_SHIFT 19

        /* get value  */ #define GX_BP_GET_ALPHACOMPARE_COMP1(reg) GX_BITGET((reg), 10, 3)
        /* set value  */ #define GX_BP_SET_ALPHACOMPARE_COMP1(reg, x) ((reg) = GX_BITSET((reg), 10, 3, x))

// LOGIC [8:9] (2)
        /* start bit  */ #define GX_BP_ALPHACOMPARE_LOGIC_B 8
        /* end bit    */ #define GX_BP_ALPHACOMPARE_LOGIC_E 9
        /* bit size   */ #define GX_BP_ALPHACOMPARE_LOGIC_SZ 2

        /* raw mask   */ #define GX_BP_ALPHACOMPARE_LOGIC_MASK (((1 << 2) - 1) << (31 - 9))
        /* local mask */ #define GX_BP_ALPHACOMPARE_LOGIC_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_ALPHACOMPARE_LOGIC_SHIFT 22

        /* get value  */ #define GX_BP_GET_ALPHACOMPARE_LOGIC(reg) GX_BITGET((reg), 8, 2)
        /* set value  */ #define GX_BP_SET_ALPHACOMPARE_LOGIC(reg, x) ((reg) = GX_BITSET((reg), 8, 2, x))


/******************************************************************************
 * BP structure - TevKSel
 *****************************************************************************/
// SWAP_RB [30:31] (2) - Odd ksel number: red; even: blue
        /* start bit  */ #define GX_BP_TEVKSEL_SWAP_RB_B 30
        /* end bit    */ #define GX_BP_TEVKSEL_SWAP_RB_E 31
        /* bit size   */ #define GX_BP_TEVKSEL_SWAP_RB_SZ 2

        /* raw mask   */ #define GX_BP_TEVKSEL_SWAP_RB_MASK (((1 << 2) - 1) << (31 - 31))
        /* local mask */ #define GX_BP_TEVKSEL_SWAP_RB_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_TEVKSEL_SWAP_RB_SHIFT 0

        /* get value  */ #define GX_BP_GET_TEVKSEL_SWAP_RB(reg) GX_BITGET((reg), 30, 2)
        /* set value  */ #define GX_BP_SET_TEVKSEL_SWAP_RB(reg, x) ((reg) = GX_BITSET((reg), 30, 2, x))

// SWAP_GA [28:29] (2) - Odd ksel number: green; even: alpha
        /* start bit  */ #define GX_BP_TEVKSEL_SWAP_GA_B 28
        /* end bit    */ #define GX_BP_TEVKSEL_SWAP_GA_E 29
        /* bit size   */ #define GX_BP_TEVKSEL_SWAP_GA_SZ 2

        /* raw mask   */ #define GX_BP_TEVKSEL_SWAP_GA_MASK (((1 << 2) - 1) << (31 - 29))
        /* local mask */ #define GX_BP_TEVKSEL_SWAP_GA_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_BP_TEVKSEL_SWAP_GA_SHIFT 2

        /* get value  */ #define GX_BP_GET_TEVKSEL_SWAP_GA(reg) GX_BITGET((reg), 28, 2)
        /* set value  */ #define GX_BP_SET_TEVKSEL_SWAP_GA(reg, x) ((reg) = GX_BITSET((reg), 28, 2, x))

// KCSEL_EVEN [23:27] (5)
        /* start bit  */ #define GX_BP_TEVKSEL_KCSEL_EVEN_B 23
        /* end bit    */ #define GX_BP_TEVKSEL_KCSEL_EVEN_E 27
        /* bit size   */ #define GX_BP_TEVKSEL_KCSEL_EVEN_SZ 5

        /* raw mask   */ #define GX_BP_TEVKSEL_KCSEL_EVEN_MASK (((1 << 5) - 1) << (31 - 27))
        /* local mask */ #define GX_BP_TEVKSEL_KCSEL_EVEN_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_BP_TEVKSEL_KCSEL_EVEN_SHIFT 4

        /* get value  */ #define GX_BP_GET_TEVKSEL_KCSEL_EVEN(reg) GX_BITGET((reg), 23, 5)
        /* set value  */ #define GX_BP_SET_TEVKSEL_KCSEL_EVEN(reg, x) ((reg) = GX_BITSET((reg), 23, 5, x))

// KASEL_EVEN [18:22] (5)
        /* start bit  */ #define GX_BP_TEVKSEL_KASEL_EVEN_B 18
        /* end bit    */ #define GX_BP_TEVKSEL_KASEL_EVEN_E 22
        /* bit size   */ #define GX_BP_TEVKSEL_KASEL_EVEN_SZ 5

        /* raw mask   */ #define GX_BP_TEVKSEL_KASEL_EVEN_MASK (((1 << 5) - 1) << (31 - 22))
        /* local mask */ #define GX_BP_TEVKSEL_KASEL_EVEN_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_BP_TEVKSEL_KASEL_EVEN_SHIFT 9

        /* get value  */ #define GX_BP_GET_TEVKSEL_KASEL_EVEN(reg) GX_BITGET((reg), 18, 5)
        /* set value  */ #define GX_BP_SET_TEVKSEL_KASEL_EVEN(reg, x) ((reg) = GX_BITSET((reg), 18, 5, x))

// KCSEL_ODD [13:17] (5)
        /* start bit  */ #define GX_BP_TEVKSEL_KCSEL_ODD_B 13
        /* end bit    */ #define GX_BP_TEVKSEL_KCSEL_ODD_E 17
        /* bit size   */ #define GX_BP_TEVKSEL_KCSEL_ODD_SZ 5

        /* raw mask   */ #define GX_BP_TEVKSEL_KCSEL_ODD_MASK (((1 << 5) - 1) << (31 - 17))
        /* local mask */ #define GX_BP_TEVKSEL_KCSEL_ODD_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_BP_TEVKSEL_KCSEL_ODD_SHIFT 14

        /* get value  */ #define GX_BP_GET_TEVKSEL_KCSEL_ODD(reg) GX_BITGET((reg), 13, 5)
        /* set value  */ #define GX_BP_SET_TEVKSEL_KCSEL_ODD(reg, x) ((reg) = GX_BITSET((reg), 13, 5, x))

// KASEL_ODD [8:12] (5)
        /* start bit  */ #define GX_BP_TEVKSEL_KASEL_ODD_B 8
        /* end bit    */ #define GX_BP_TEVKSEL_KASEL_ODD_E 12
        /* bit size   */ #define GX_BP_TEVKSEL_KASEL_ODD_SZ 5

        /* raw mask   */ #define GX_BP_TEVKSEL_KASEL_ODD_MASK (((1 << 5) - 1) << (31 - 12))
        /* local mask */ #define GX_BP_TEVKSEL_KASEL_ODD_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_BP_TEVKSEL_KASEL_ODD_SHIFT 19

        /* get value  */ #define GX_BP_GET_TEVKSEL_KASEL_ODD(reg) GX_BITGET((reg), 8, 5)
        /* set value  */ #define GX_BP_SET_TEVKSEL_KASEL_ODD(reg, x) ((reg) = GX_BITSET((reg), 8, 5, x))

// clang-format on

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
#ifndef RVL_SDK_GX_HARDWARE_CP_H
#define RVL_SDK_GX_HARDWARE_CP_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardwareCP.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardwareCP.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

/******************************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 *****************************************************************************/

/**
 * CP registers
 */
typedef enum {
    GX_CP_REG_MATRIXINDEXA = 0x30,
    GX_CP_REG_MATRIXINDEXB = 0x40,
    GX_CP_REG_VCD_LO = 0x50,
    GX_CP_REG_VCD_HI = 0x60,
    GX_CP_REG_VAT_GROUP0 = 0x70,
    GX_CP_REG_VAT_GROUP1 = 0x80,
    GX_CP_REG_VAT_GROUP2 = 0x90,
    GX_CP_REG_ARRAYBASE = 0xA0,
    GX_CP_REG_ARRAYSTRIDE = 0xB0,
} GX_CP_REG;

/******************************************************************************
 * CP register 0x30 - MatrixIndexA
 *****************************************************************************/
// TEX3 [2:7] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXA_TEX3_B 2
        /* end bit    */ #define GX_CP_MATRIXINDEXA_TEX3_E 7
        /* bit size   */ #define GX_CP_MATRIXINDEXA_TEX3_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX3_MASK (((1 << 6) - 1) << (31 - 7))
        /* local mask */ #define GX_CP_MATRIXINDEXA_TEX3_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX3_SHIFT 24

        /* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX3(reg) GX_BITGET((reg), 2, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX3(reg, x) ((reg) = GX_BITSET((reg), 2, 6, x))

// TEX2 [8:13] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXA_TEX2_B 8
        /* end bit    */ #define GX_CP_MATRIXINDEXA_TEX2_E 13
        /* bit size   */ #define GX_CP_MATRIXINDEXA_TEX2_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX2_MASK (((1 << 6) - 1) << (31 - 13))
        /* local mask */ #define GX_CP_MATRIXINDEXA_TEX2_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX2_SHIFT 18

        /* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX2(reg) GX_BITGET((reg), 8, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX2(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))

// TEX1 [14:19] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXA_TEX1_B 14
        /* end bit    */ #define GX_CP_MATRIXINDEXA_TEX1_E 19
        /* bit size   */ #define GX_CP_MATRIXINDEXA_TEX1_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX1_MASK (((1 << 6) - 1) << (31 - 19))
        /* local mask */ #define GX_CP_MATRIXINDEXA_TEX1_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX1_SHIFT 12

        /* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX1(reg) GX_BITGET((reg), 14, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX1(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX0 [20:25] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXA_TEX0_B 20
        /* end bit    */ #define GX_CP_MATRIXINDEXA_TEX0_E 25
        /* bit size   */ #define GX_CP_MATRIXINDEXA_TEX0_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX0_MASK (((1 << 6) - 1) << (31 - 25))
        /* local mask */ #define GX_CP_MATRIXINDEXA_TEX0_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX0_SHIFT 6

        /* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX0(reg) GX_BITGET((reg), 20, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX0(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// POSNRM [26:31] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXA_POSNRM_B 26
        /* end bit    */ #define GX_CP_MATRIXINDEXA_POSNRM_E 31
        /* bit size   */ #define GX_CP_MATRIXINDEXA_POSNRM_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXA_POSNRM_MASK (((1 << 6) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_MATRIXINDEXA_POSNRM_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXA_POSNRM_SHIFT 0

        /* get value  */ #define GX_CP_GET_MATRIXINDEXA_POSNRM(reg) GX_BITGET((reg), 26, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXA_POSNRM(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))


/******************************************************************************
 * CP register 0x40 - MatrixIndexB
 *****************************************************************************/
// TEX7 [8:13] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXB_TEX7_B 8
        /* end bit    */ #define GX_CP_MATRIXINDEXB_TEX7_E 13
        /* bit size   */ #define GX_CP_MATRIXINDEXB_TEX7_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX7_MASK (((1 << 6) - 1) << (31 - 13))
        /* local mask */ #define GX_CP_MATRIXINDEXB_TEX7_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX7_SHIFT 18

        /* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX7(reg) GX_BITGET((reg), 8, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX7(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))

// TEX6 [14:19] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXB_TEX6_B 14
        /* end bit    */ #define GX_CP_MATRIXINDEXB_TEX6_E 19
        /* bit size   */ #define GX_CP_MATRIXINDEXB_TEX6_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX6_MASK (((1 << 6) - 1) << (31 - 19))
        /* local mask */ #define GX_CP_MATRIXINDEXB_TEX6_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX6_SHIFT 12

        /* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX6(reg) GX_BITGET((reg), 14, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX6(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX5 [20:25] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXB_TEX5_B 20
        /* end bit    */ #define GX_CP_MATRIXINDEXB_TEX5_E 25
        /* bit size   */ #define GX_CP_MATRIXINDEXB_TEX5_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX5_MASK (((1 << 6) - 1) << (31 - 25))
        /* local mask */ #define GX_CP_MATRIXINDEXB_TEX5_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX5_SHIFT 6

        /* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX5(reg) GX_BITGET((reg), 20, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX5(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// TEX4 [26:31] (6)
        /* start bit  */ #define GX_CP_MATRIXINDEXB_TEX4_B 26
        /* end bit    */ #define GX_CP_MATRIXINDEXB_TEX4_E 31
        /* bit size   */ #define GX_CP_MATRIXINDEXB_TEX4_SZ 6

        /* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX4_MASK (((1 << 6) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_MATRIXINDEXB_TEX4_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX4_SHIFT 0

        /* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX4(reg) GX_BITGET((reg), 26, 6)
        /* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX4(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))


/******************************************************************************
 * CP register 0x50 - VCD_Lo
 *****************************************************************************/
// POSMATIDX [31:31] (1)
        /* start bit  */ #define GX_CP_VCD_LO_POSMATIDX_B 31
        /* end bit    */ #define GX_CP_VCD_LO_POSMATIDX_E 31
        /* bit size   */ #define GX_CP_VCD_LO_POSMATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_POSMATIDX_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_VCD_LO_POSMATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_POSMATIDX_SHIFT 0

        /* get value  */ #define GX_CP_GET_VCD_LO_POSMATIDX(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_POSMATIDX(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// TEX0MATIDX [30:30] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX0MATIDX_B 30
        /* end bit    */ #define GX_CP_VCD_LO_TEX0MATIDX_E 30
        /* bit size   */ #define GX_CP_VCD_LO_TEX0MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX0MATIDX_MASK (((1 << 1) - 1) << (31 - 30))
        /* local mask */ #define GX_CP_VCD_LO_TEX0MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX0MATIDX_SHIFT 1

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX0MATIDX(reg) GX_BITGET((reg), 30, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX0MATIDX(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// TEX1MATIDX [29:29] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX1MATIDX_B 29
        /* end bit    */ #define GX_CP_VCD_LO_TEX1MATIDX_E 29
        /* bit size   */ #define GX_CP_VCD_LO_TEX1MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX1MATIDX_MASK (((1 << 1) - 1) << (31 - 29))
        /* local mask */ #define GX_CP_VCD_LO_TEX1MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX1MATIDX_SHIFT 2

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX1MATIDX(reg) GX_BITGET((reg), 29, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX1MATIDX(reg, x) ((reg) = GX_BITSET((reg), 29, 1, x))

// TEX2MATIDX [28:28] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX2MATIDX_B 28
        /* end bit    */ #define GX_CP_VCD_LO_TEX2MATIDX_E 28
        /* bit size   */ #define GX_CP_VCD_LO_TEX2MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX2MATIDX_MASK (((1 << 1) - 1) << (31 - 28))
        /* local mask */ #define GX_CP_VCD_LO_TEX2MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX2MATIDX_SHIFT 3

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX2MATIDX(reg) GX_BITGET((reg), 28, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX2MATIDX(reg, x) ((reg) = GX_BITSET((reg), 28, 1, x))

// TEX3MATIDX [27:27] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX3MATIDX_B 27
        /* end bit    */ #define GX_CP_VCD_LO_TEX3MATIDX_E 27
        /* bit size   */ #define GX_CP_VCD_LO_TEX3MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX3MATIDX_MASK (((1 << 1) - 1) << (31 - 27))
        /* local mask */ #define GX_CP_VCD_LO_TEX3MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX3MATIDX_SHIFT 4

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX3MATIDX(reg) GX_BITGET((reg), 27, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX3MATIDX(reg, x) ((reg) = GX_BITSET((reg), 27, 1, x))

// TEX4MATIDX [26:26] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX4MATIDX_B 26
        /* end bit    */ #define GX_CP_VCD_LO_TEX4MATIDX_E 26
        /* bit size   */ #define GX_CP_VCD_LO_TEX4MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX4MATIDX_MASK (((1 << 1) - 1) << (31 - 26))
        /* local mask */ #define GX_CP_VCD_LO_TEX4MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX4MATIDX_SHIFT 5

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX4MATIDX(reg) GX_BITGET((reg), 26, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX4MATIDX(reg, x) ((reg) = GX_BITSET((reg), 26, 1, x))

// TEX5MATIDX [25:25] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX5MATIDX_B 25
        /* end bit    */ #define GX_CP_VCD_LO_TEX5MATIDX_E 25
        /* bit size   */ #define GX_CP_VCD_LO_TEX5MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX5MATIDX_MASK (((1 << 1) - 1) << (31 - 25))
        /* local mask */ #define GX_CP_VCD_LO_TEX5MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX5MATIDX_SHIFT 6

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX5MATIDX(reg) GX_BITGET((reg), 25, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX5MATIDX(reg, x) ((reg) = GX_BITSET((reg), 25, 1, x))

// TEX6MATIDX [24:24] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX6MATIDX_B 24
        /* end bit    */ #define GX_CP_VCD_LO_TEX6MATIDX_E 24
        /* bit size   */ #define GX_CP_VCD_LO_TEX6MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX6MATIDX_MASK (((1 << 1) - 1) << (31 - 24))
        /* local mask */ #define GX_CP_VCD_LO_TEX6MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX6MATIDX_SHIFT 7

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX6MATIDX(reg) GX_BITGET((reg), 24, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX6MATIDX(reg, x) ((reg) = GX_BITSET((reg), 24, 1, x))

// TEX7MATIDX [23:23] (1)
        /* start bit  */ #define GX_CP_VCD_LO_TEX7MATIDX_B 23
        /* end bit    */ #define GX_CP_VCD_LO_TEX7MATIDX_E 23
        /* bit size   */ #define GX_CP_VCD_LO_TEX7MATIDX_SZ 1

        /* raw mask   */ #define GX_CP_VCD_LO_TEX7MATIDX_MASK (((1 << 1) - 1) << (31 - 23))
        /* local mask */ #define GX_CP_VCD_LO_TEX7MATIDX_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_TEX7MATIDX_SHIFT 8

        /* get value  */ #define GX_CP_GET_VCD_LO_TEX7MATIDX(reg) GX_BITGET((reg), 23, 1)
        /* set value  */ #define GX_CP_SET_VCD_LO_TEX7MATIDX(reg, x) ((reg) = GX_BITSET((reg), 23, 1, x))

// POSITION [21:22] (2)
        /* start bit  */ #define GX_CP_VCD_LO_POSITION_B 21
        /* end bit    */ #define GX_CP_VCD_LO_POSITION_E 22
        /* bit size   */ #define GX_CP_VCD_LO_POSITION_SZ 2

        /* raw mask   */ #define GX_CP_VCD_LO_POSITION_MASK (((1 << 2) - 1) << (31 - 22))
        /* local mask */ #define GX_CP_VCD_LO_POSITION_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_POSITION_SHIFT 9

        /* get value  */ #define GX_CP_GET_VCD_LO_POSITION(reg) GX_BITGET((reg), 21, 2)
        /* set value  */ #define GX_CP_SET_VCD_LO_POSITION(reg, x) ((reg) = GX_BITSET((reg), 21, 2, x))

// NORMAL [19:20] (2)
        /* start bit  */ #define GX_CP_VCD_LO_NORMAL_B 19
        /* end bit    */ #define GX_CP_VCD_LO_NORMAL_E 20
        /* bit size   */ #define GX_CP_VCD_LO_NORMAL_SZ 2

        /* raw mask   */ #define GX_CP_VCD_LO_NORMAL_MASK (((1 << 2) - 1) << (31 - 20))
        /* local mask */ #define GX_CP_VCD_LO_NORMAL_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_NORMAL_SHIFT 11

        /* get value  */ #define GX_CP_GET_VCD_LO_NORMAL(reg) GX_BITGET((reg), 19, 2)
        /* set value  */ #define GX_CP_SET_VCD_LO_NORMAL(reg, x) ((reg) = GX_BITSET((reg), 19, 2, x))

// COLORDIFFUSED [17:18] (2)
        /* start bit  */ #define GX_CP_VCD_LO_COLORDIFFUSED_B 17
        /* end bit    */ #define GX_CP_VCD_LO_COLORDIFFUSED_E 18
        /* bit size   */ #define GX_CP_VCD_LO_COLORDIFFUSED_SZ 2

        /* raw mask   */ #define GX_CP_VCD_LO_COLORDIFFUSED_MASK (((1 << 2) - 1) << (31 - 18))
        /* local mask */ #define GX_CP_VCD_LO_COLORDIFFUSED_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_COLORDIFFUSED_SHIFT 13

        /* get value  */ #define GX_CP_GET_VCD_LO_COLORDIFFUSED(reg) GX_BITGET((reg), 17, 2)
        /* set value  */ #define GX_CP_SET_VCD_LO_COLORDIFFUSED(reg, x) ((reg) = GX_BITSET((reg), 17, 2, x))

// COLORSPECULAR [15:16] (2)
        /* start bit  */ #define GX_CP_VCD_LO_COLORSPECULAR_B 15
        /* end bit    */ #define GX_CP_VCD_LO_COLORSPECULAR_E 16
        /* bit size   */ #define GX_CP_VCD_LO_COLORSPECULAR_SZ 2

        /* raw mask   */ #define GX_CP_VCD_LO_COLORSPECULAR_MASK (((1 << 2) - 1) << (31 - 16))
        /* local mask */ #define GX_CP_VCD_LO_COLORSPECULAR_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_LO_COLORSPECULAR_SHIFT 15

        /* get value  */ #define GX_CP_GET_VCD_LO_COLORSPECULAR(reg) GX_BITGET((reg), 15, 2)
        /* set value  */ #define GX_CP_SET_VCD_LO_COLORSPECULAR(reg, x) ((reg) = GX_BITSET((reg), 15, 2, x))


/******************************************************************************
 * CP register 0x60 - VCD_Hi
 *****************************************************************************/
// TEX0COORD [30:31] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX0COORD_B 30
        /* end bit    */ #define GX_CP_VCD_HI_TEX0COORD_E 31
        /* bit size   */ #define GX_CP_VCD_HI_TEX0COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX0COORD_MASK (((1 << 2) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_VCD_HI_TEX0COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX0COORD_SHIFT 0

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX0COORD(reg) GX_BITGET((reg), 30, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX0COORD(reg, x) ((reg) = GX_BITSET((reg), 30, 2, x))

// TEX1COORD [28:29] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX1COORD_B 28
        /* end bit    */ #define GX_CP_VCD_HI_TEX1COORD_E 29
        /* bit size   */ #define GX_CP_VCD_HI_TEX1COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX1COORD_MASK (((1 << 2) - 1) << (31 - 29))
        /* local mask */ #define GX_CP_VCD_HI_TEX1COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX1COORD_SHIFT 2

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX1COORD(reg) GX_BITGET((reg), 28, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX1COORD(reg, x) ((reg) = GX_BITSET((reg), 28, 2, x))

// TEX2COORD [26:27] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX2COORD_B 26
        /* end bit    */ #define GX_CP_VCD_HI_TEX2COORD_E 27
        /* bit size   */ #define GX_CP_VCD_HI_TEX2COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX2COORD_MASK (((1 << 2) - 1) << (31 - 27))
        /* local mask */ #define GX_CP_VCD_HI_TEX2COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX2COORD_SHIFT 4

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX2COORD(reg) GX_BITGET((reg), 26, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX2COORD(reg, x) ((reg) = GX_BITSET((reg), 26, 2, x))

// TEX3COORD [24:25] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX3COORD_B 24
        /* end bit    */ #define GX_CP_VCD_HI_TEX3COORD_E 25
        /* bit size   */ #define GX_CP_VCD_HI_TEX3COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX3COORD_MASK (((1 << 2) - 1) << (31 - 25))
        /* local mask */ #define GX_CP_VCD_HI_TEX3COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX3COORD_SHIFT 6

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX3COORD(reg) GX_BITGET((reg), 24, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX3COORD(reg, x) ((reg) = GX_BITSET((reg), 24, 2, x))

// TEX4COORD [22:23] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX4COORD_B 22
        /* end bit    */ #define GX_CP_VCD_HI_TEX4COORD_E 23
        /* bit size   */ #define GX_CP_VCD_HI_TEX4COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX4COORD_MASK (((1 << 2) - 1) << (31 - 23))
        /* local mask */ #define GX_CP_VCD_HI_TEX4COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX4COORD_SHIFT 8

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX4COORD(reg) GX_BITGET((reg), 22, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX4COORD(reg, x) ((reg) = GX_BITSET((reg), 22, 2, x))

// TEX5COORD [20:21] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX5COORD_B 20
        /* end bit    */ #define GX_CP_VCD_HI_TEX5COORD_E 21
        /* bit size   */ #define GX_CP_VCD_HI_TEX5COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX5COORD_MASK (((1 << 2) - 1) << (31 - 21))
        /* local mask */ #define GX_CP_VCD_HI_TEX5COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX5COORD_SHIFT 10

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX5COORD(reg) GX_BITGET((reg), 20, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX5COORD(reg, x) ((reg) = GX_BITSET((reg), 20, 2, x))

// TEX6COORD [18:19] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX6COORD_B 18
        /* end bit    */ #define GX_CP_VCD_HI_TEX6COORD_E 19
        /* bit size   */ #define GX_CP_VCD_HI_TEX6COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX6COORD_MASK (((1 << 2) - 1) << (31 - 19))
        /* local mask */ #define GX_CP_VCD_HI_TEX6COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX6COORD_SHIFT 12

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX6COORD(reg) GX_BITGET((reg), 18, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX6COORD(reg, x) ((reg) = GX_BITSET((reg), 18, 2, x))

// TEX7COORD [16:17] (2)
        /* start bit  */ #define GX_CP_VCD_HI_TEX7COORD_B 16
        /* end bit    */ #define GX_CP_VCD_HI_TEX7COORD_E 17
        /* bit size   */ #define GX_CP_VCD_HI_TEX7COORD_SZ 2

        /* raw mask   */ #define GX_CP_VCD_HI_TEX7COORD_MASK (((1 << 2) - 1) << (31 - 17))
        /* local mask */ #define GX_CP_VCD_HI_TEX7COORD_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_CP_VCD_HI_TEX7COORD_SHIFT 14

        /* get value  */ #define GX_CP_GET_VCD_HI_TEX7COORD(reg) GX_BITGET((reg), 16, 2)
        /* set value  */ #define GX_CP_SET_VCD_HI_TEX7COORD(reg, x) ((reg) = GX_BITSET((reg), 16, 2, x))


/******************************************************************************
 * CP register 0x70 - VAT_group0
 *****************************************************************************/
// POS_CNT [31:31] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP0_POS_CNT_B 31
        /* end bit    */ #define GX_CP_VAT_GROUP0_POS_CNT_E 31
        /* bit size   */ #define GX_CP_VAT_GROUP0_POS_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP0_POS_CNT_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_VAT_GROUP0_POS_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_POS_CNT_SHIFT 0

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_POS_CNT(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_POS_CNT(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// POS_TYPE [28:30] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP0_POS_TYPE_B 28
        /* end bit    */ #define GX_CP_VAT_GROUP0_POS_TYPE_E 30
        /* bit size   */ #define GX_CP_VAT_GROUP0_POS_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP0_POS_TYPE_MASK (((1 << 3) - 1) << (31 - 30))
        /* local mask */ #define GX_CP_VAT_GROUP0_POS_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_POS_TYPE_SHIFT 1

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_POS_TYPE(reg) GX_BITGET((reg), 28, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_POS_TYPE(reg, x) ((reg) = GX_BITSET((reg), 28, 3, x))

// POS_SHIFT [23:27] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP0_POS_SHIFT_B 23
        /* end bit    */ #define GX_CP_VAT_GROUP0_POS_SHIFT_E 27
        /* bit size   */ #define GX_CP_VAT_GROUP0_POS_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP0_POS_SHIFT_MASK (((1 << 5) - 1) << (31 - 27))
        /* local mask */ #define GX_CP_VAT_GROUP0_POS_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_POS_SHIFT_SHIFT 4

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_POS_SHIFT(reg) GX_BITGET((reg), 23, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_POS_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 23, 5, x))

// NRM_CNT [22:22] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP0_NRM_CNT_B 22
        /* end bit    */ #define GX_CP_VAT_GROUP0_NRM_CNT_E 22
        /* bit size   */ #define GX_CP_VAT_GROUP0_NRM_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP0_NRM_CNT_MASK (((1 << 1) - 1) << (31 - 22))
        /* local mask */ #define GX_CP_VAT_GROUP0_NRM_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_NRM_CNT_SHIFT 9

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_NRM_CNT(reg) GX_BITGET((reg), 22, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_NRM_CNT(reg, x) ((reg) = GX_BITSET((reg), 22, 1, x))

// NRM_TYPE [19:21] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP0_NRM_TYPE_B 19
        /* end bit    */ #define GX_CP_VAT_GROUP0_NRM_TYPE_E 21
        /* bit size   */ #define GX_CP_VAT_GROUP0_NRM_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP0_NRM_TYPE_MASK (((1 << 3) - 1) << (31 - 21))
        /* local mask */ #define GX_CP_VAT_GROUP0_NRM_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_NRM_TYPE_SHIFT 10

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_NRM_TYPE(reg) GX_BITGET((reg), 19, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_NRM_TYPE(reg, x) ((reg) = GX_BITSET((reg), 19, 3, x))

// COLORDIFF_CNT [18:18] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_B 18
        /* end bit    */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_E 18
        /* bit size   */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_MASK (((1 << 1) - 1) << (31 - 18))
        /* local mask */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_SHIFT 13

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORDIFF_CNT(reg) GX_BITGET((reg), 18, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORDIFF_CNT(reg, x) ((reg) = GX_BITSET((reg), 18, 1, x))

// COLORDIFF_TYPE [15:17] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_B 15
        /* end bit    */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_E 17
        /* bit size   */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_MASK (((1 << 3) - 1) << (31 - 17))
        /* local mask */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_SHIFT 14

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORDIFF_TYPE(reg) GX_BITGET((reg), 15, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORDIFF_TYPE(reg, x) ((reg) = GX_BITSET((reg), 15, 3, x))

// COLORSPEC_CNT [14:14] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_B 14
        /* end bit    */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_E 14
        /* bit size   */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_MASK (((1 << 1) - 1) << (31 - 14))
        /* local mask */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_SHIFT 17

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORSPEC_CNT(reg) GX_BITGET((reg), 14, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORSPEC_CNT(reg, x) ((reg) = GX_BITSET((reg), 14, 1, x))

// COLORSPEC_TYPE [11:13] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_B 11
        /* end bit    */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_E 13
        /* bit size   */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_MASK (((1 << 3) - 1) << (31 - 13))
        /* local mask */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_SHIFT 18

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORSPEC_TYPE(reg) GX_BITGET((reg), 11, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORSPEC_TYPE(reg, x) ((reg) = GX_BITSET((reg), 11, 3, x))

// TXC0_CNT [10:10] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP0_TXC0_CNT_B 10
        /* end bit    */ #define GX_CP_VAT_GROUP0_TXC0_CNT_E 10
        /* bit size   */ #define GX_CP_VAT_GROUP0_TXC0_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP0_TXC0_CNT_MASK (((1 << 1) - 1) << (31 - 10))
        /* local mask */ #define GX_CP_VAT_GROUP0_TXC0_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_TXC0_CNT_SHIFT 21

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_TXC0_CNT(reg) GX_BITGET((reg), 10, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_TXC0_CNT(reg, x) ((reg) = GX_BITSET((reg), 10, 1, x))

// TXC0_TYPE [7:9] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_B 7
        /* end bit    */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_E 9
        /* bit size   */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_MASK (((1 << 3) - 1) << (31 - 9))
        /* local mask */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_SHIFT 22

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_TXC0_TYPE(reg) GX_BITGET((reg), 7, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_TXC0_TYPE(reg, x) ((reg) = GX_BITSET((reg), 7, 3, x))

// TXC0_SHIFT [2:6] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_B 2
        /* end bit    */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_E 6
        /* bit size   */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_MASK (((1 << 5) - 1) << (31 - 6))
        /* local mask */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_SHIFT 25

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_TXC0_SHIFT(reg) GX_BITGET((reg), 2, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_TXC0_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 2, 5, x))

// BYTEDEQUANT [1:1] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_B 1
        /* end bit    */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_E 1
        /* bit size   */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_MASK (((1 << 1) - 1) << (31 - 1))
        /* local mask */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_SHIFT 30

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_BYTEDEQUANT(reg) GX_BITGET((reg), 1, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_BYTEDEQUANT(reg, x) ((reg) = GX_BITSET((reg), 1, 1, x))

// NORMALINDEX3 [0:0] (1) - Input will be treated as three staggered indices (one per triple biased by component size) into normal table
        /* start bit  */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_B 0
        /* end bit    */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_E 0
        /* bit size   */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_MASK (((1 << 1) - 1) << (31 - 0))
        /* local mask */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_SHIFT 31

        /* get value  */ #define GX_CP_GET_VAT_GROUP0_NORMALINDEX3(reg) GX_BITGET((reg), 0, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP0_NORMALINDEX3(reg, x) ((reg) = GX_BITSET((reg), 0, 1, x))


/******************************************************************************
 * CP register 0x80 - VAT_group1
 *****************************************************************************/
// TXC1_CNT [31:31] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC1_CNT_B 31
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC1_CNT_E 31
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC1_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC1_CNT_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC1_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC1_CNT_SHIFT 0

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC1_CNT(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC1_CNT(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// TXC1_TYPE [28:30] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_B 28
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_E 30
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_MASK (((1 << 3) - 1) << (31 - 30))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_SHIFT 1

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC1_TYPE(reg) GX_BITGET((reg), 28, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC1_TYPE(reg, x) ((reg) = GX_BITSET((reg), 28, 3, x))

// TXC1_SHIFT [23:27] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_B 23
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_E 27
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_MASK (((1 << 5) - 1) << (31 - 27))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_SHIFT 4

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC1_SHIFT(reg) GX_BITGET((reg), 23, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC1_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 23, 5, x))

// TXC2_CNT [22:22] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC2_CNT_B 22
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC2_CNT_E 22
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC2_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC2_CNT_MASK (((1 << 1) - 1) << (31 - 22))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC2_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC2_CNT_SHIFT 9

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC2_CNT(reg) GX_BITGET((reg), 22, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC2_CNT(reg, x) ((reg) = GX_BITSET((reg), 22, 1, x))

// TXC2_TYPE [19:21] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_B 19
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_E 21
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_MASK (((1 << 3) - 1) << (31 - 21))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_SHIFT 10

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC2_TYPE(reg) GX_BITGET((reg), 19, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC2_TYPE(reg, x) ((reg) = GX_BITSET((reg), 19, 3, x))

// TXC2_SHIFT [14:18] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_B 14
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_E 18
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_MASK (((1 << 5) - 1) << (31 - 18))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_SHIFT 13

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC2_SHIFT(reg) GX_BITGET((reg), 14, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC2_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 14, 5, x))

// TXC3_CNT [13:13] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC3_CNT_B 13
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC3_CNT_E 13
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC3_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC3_CNT_MASK (((1 << 1) - 1) << (31 - 13))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC3_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC3_CNT_SHIFT 18

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC3_CNT(reg) GX_BITGET((reg), 13, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC3_CNT(reg, x) ((reg) = GX_BITSET((reg), 13, 1, x))

// TXC3_TYPE [10:12] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_B 10
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_E 12
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_MASK (((1 << 3) - 1) << (31 - 12))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_SHIFT 19

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC3_TYPE(reg) GX_BITGET((reg), 10, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC3_TYPE(reg, x) ((reg) = GX_BITSET((reg), 10, 3, x))

// TXC3_SHIFT [5:9] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_B 5
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_E 9
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_MASK (((1 << 5) - 1) << (31 - 9))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_SHIFT 22

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC3_SHIFT(reg) GX_BITGET((reg), 5, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC3_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 5, 5, x))

// TXC4_CNT [4:4] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC4_CNT_B 4
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC4_CNT_E 4
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC4_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC4_CNT_MASK (((1 << 1) - 1) << (31 - 4))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC4_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC4_CNT_SHIFT 27

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC4_CNT(reg) GX_BITGET((reg), 4, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC4_CNT(reg, x) ((reg) = GX_BITSET((reg), 4, 1, x))

// TXC4_TYPE [1:3] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_B 1
        /* end bit    */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_E 3
        /* bit size   */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_MASK (((1 << 3) - 1) << (31 - 3))
        /* local mask */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_SHIFT 28

        /* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC4_TYPE(reg) GX_BITGET((reg), 1, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC4_TYPE(reg, x) ((reg) = GX_BITSET((reg), 1, 3, x))


/******************************************************************************
 * CP register 0x90 - VAT_group2
 *****************************************************************************/
// TXC4_SHIFT [27:31] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_B 27
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_E 31
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_MASK (((1 << 5) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_SHIFT 0

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC4_SHIFT(reg) GX_BITGET((reg), 27, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC4_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 27, 5, x))

// TXC5_CNT [26:26] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC5_CNT_B 26
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC5_CNT_E 26
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC5_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC5_CNT_MASK (((1 << 1) - 1) << (31 - 26))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC5_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC5_CNT_SHIFT 5

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC5_CNT(reg) GX_BITGET((reg), 26, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC5_CNT(reg, x) ((reg) = GX_BITSET((reg), 26, 1, x))

// TXC5_TYPE [23:25] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_B 23
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_E 25
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_MASK (((1 << 3) - 1) << (31 - 25))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_SHIFT 6

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC5_TYPE(reg) GX_BITGET((reg), 23, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC5_TYPE(reg, x) ((reg) = GX_BITSET((reg), 23, 3, x))

// TXC5_SHIFT [18:22] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_B 18
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_E 22
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_MASK (((1 << 5) - 1) << (31 - 22))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_SHIFT 9

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC5_SHIFT(reg) GX_BITGET((reg), 18, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC5_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 18, 5, x))

// TXC6_CNT [17:17] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC6_CNT_B 17
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC6_CNT_E 17
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC6_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC6_CNT_MASK (((1 << 1) - 1) << (31 - 17))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC6_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC6_CNT_SHIFT 14

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC6_CNT(reg) GX_BITGET((reg), 17, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC6_CNT(reg, x) ((reg) = GX_BITSET((reg), 17, 1, x))

// TXC6_TYPE [14:16] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_B 14
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_E 16
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_MASK (((1 << 3) - 1) << (31 - 16))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_SHIFT 15

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC6_TYPE(reg) GX_BITGET((reg), 14, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC6_TYPE(reg, x) ((reg) = GX_BITSET((reg), 14, 3, x))

// TXC6_SHIFT [9:13] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_B 9
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_E 13
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_MASK (((1 << 5) - 1) << (31 - 13))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_SHIFT 18

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC6_SHIFT(reg) GX_BITGET((reg), 9, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC6_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 9, 5, x))

// TXC7_CNT [8:8] (1)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC7_CNT_B 8
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC7_CNT_E 8
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC7_CNT_SZ 1

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC7_CNT_MASK (((1 << 1) - 1) << (31 - 8))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC7_CNT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC7_CNT_SHIFT 23

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC7_CNT(reg) GX_BITGET((reg), 8, 1)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC7_CNT(reg, x) ((reg) = GX_BITSET((reg), 8, 1, x))

// TXC7_TYPE [5:7] (3)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_B 5
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_E 7
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_SZ 3

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_MASK (((1 << 3) - 1) << (31 - 7))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_SHIFT 24

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC7_TYPE(reg) GX_BITGET((reg), 5, 3)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC7_TYPE(reg, x) ((reg) = GX_BITSET((reg), 5, 3, x))

// TXC7_SHIFT [0:4] (5)
        /* start bit  */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_B 0
        /* end bit    */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_E 4
        /* bit size   */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_SZ 5

        /* raw mask   */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_MASK (((1 << 5) - 1) << (31 - 4))
        /* local mask */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_SHIFT 27

        /* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC7_SHIFT(reg) GX_BITGET((reg), 0, 5)
        /* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC7_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 0, 5, x))


/******************************************************************************
 * CP register 0xA0 - ArrayBase
 *****************************************************************************/
// BASE [6:31] (26)
        /* start bit  */ #define GX_CP_ARRAYBASE_BASE_B 6
        /* end bit    */ #define GX_CP_ARRAYBASE_BASE_E 31
        /* bit size   */ #define GX_CP_ARRAYBASE_BASE_SZ 26

        /* raw mask   */ #define GX_CP_ARRAYBASE_BASE_MASK (((1 << 26) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_ARRAYBASE_BASE_LMASK ((1 << 26) - 1)
        /* bit shift  */ #define GX_CP_ARRAYBASE_BASE_SHIFT 0

        /* get value  */ #define GX_CP_GET_ARRAYBASE_BASE(reg) GX_BITGET((reg), 6, 26)
        /* set value  */ #define GX_CP_SET_ARRAYBASE_BASE(reg, x) ((reg) = GX_BITSET((reg), 6, 26, x))


/******************************************************************************
 * CP register 0xB0 - ArrayStride
 *****************************************************************************/
// STRIDE [24:31] (8)
        /* start bit  */ #define GX_CP_ARRAYSTRIDE_STRIDE_B 24
        /* end bit    */ #define GX_CP_ARRAYSTRIDE_STRIDE_E 31
        /* bit size   */ #define GX_CP_ARRAYSTRIDE_STRIDE_SZ 8

        /* raw mask   */ #define GX_CP_ARRAYSTRIDE_STRIDE_MASK (((1 << 8) - 1) << (31 - 31))
        /* local mask */ #define GX_CP_ARRAYSTRIDE_STRIDE_LMASK ((1 << 8) - 1)
        /* bit shift  */ #define GX_CP_ARRAYSTRIDE_STRIDE_SHIFT 0

        /* get value  */ #define GX_CP_GET_ARRAYSTRIDE_STRIDE(reg) GX_BITGET((reg), 24, 8)
        /* set value  */ #define GX_CP_SET_ARRAYSTRIDE_STRIDE(reg, x) ((reg) = GX_BITSET((reg), 24, 8, x))

// clang-format on

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
#ifndef RVL_SDK_GX_HARDWARE_XF_H
#define RVL_SDK_GX_HARDWARE_XF_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardwareXF.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardwareXF.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

/******************************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 *****************************************************************************/

/**
 * XF registers
 */
typedef enum {
    GX_XF_REG_ERROR = 0x1000,
    GX_XF_REG_DIAGNOSTICS = 0x1001,
    GX_XF_REG_STATE0 = 0x1002,
    GX_XF_REG_STATE1 = 0x1003,
    GX_XF_REG_CLOCK = 0x1004,
    GX_XF_REG_CLIPDISABLE = 0x1005,
    GX_XF_REG_PERF0 = 0x1006,
    GX_XF_REG_PERF1 = 0x1007,
    GX_XF_REG_INVERTEXSPEC = 0x1008,
    GX_XF_REG_NUMCOLORS = 0x1009,
    GX_XF_REG_AMBIENT0 = 0x100A,
    GX_XF_REG_AMBIENT1 = 0x100B,
    GX_XF_REG_MATERIAL0 = 0x100C,
    GX_XF_REG_MATERIAL1 = 0x100D,
    GX_XF_REG_COLOR0CNTRL = 0x100E,
    GX_XF_REG_COLOR1CNTRL = 0x100F,
    GX_XF_REG_ALPHA0CNTRL = 0x1010,
    GX_XF_REG_ALPHA1CNTRL = 0x1011,
    GX_XF_REG_DUALTEXTRAN = 0x1012,
    GX_XF_REG_MATRIXINDEX0 = 0x1018,
    GX_XF_REG_MATRIXINDEX1 = 0x1019,
    GX_XF_REG_SCALEX = 0x101A,
    GX_XF_REG_SCALEY = 0x101B,
    GX_XF_REG_SCALEZ = 0x101C,
    GX_XF_REG_OFFSETX = 0x101D,
    GX_XF_REG_OFFSETY = 0x101E,
    GX_XF_REG_OFFSETZ = 0x101F,
    GX_XF_REG_PROJECTIONA = 0x1020,
    GX_XF_REG_PROJECTIONB = 0x1021,
    GX_XF_REG_PROJECTIONC = 0x1022,
    GX_XF_REG_PROJECTIOND = 0x1023,
    GX_XF_REG_PROJECTIONE = 0x1024,
    GX_XF_REG_PROJECTIONF = 0x1025,
    GX_XF_REG_PROJECTORTHO = 0x1026,
    GX_XF_REG_NUMTEX = 0x103F,
    GX_XF_REG_TEX0 = 0x1040,
    GX_XF_REG_TEX1 = 0x1041,
    GX_XF_REG_TEX2 = 0x1042,
    GX_XF_REG_TEX3 = 0x1043,
    GX_XF_REG_TEX4 = 0x1044,
    GX_XF_REG_TEX5 = 0x1045,
    GX_XF_REG_TEX6 = 0x1046,
    GX_XF_REG_TEX7 = 0x1047,
    GX_XF_REG_DUALTEX0 = 0x1050,
    GX_XF_REG_DUALTEX1 = 0x1051,
    GX_XF_REG_DUALTEX2 = 0x1052,
    GX_XF_REG_DUALTEX3 = 0x1053,
    GX_XF_REG_DUALTEX4 = 0x1054,
    GX_XF_REG_DUALTEX5 = 0x1055,
    GX_XF_REG_DUALTEX6 = 0x1056,
    GX_XF_REG_DUALTEX7 = 0x1057,
} GX_XF_REG;

/******************************************************************************
 * XF register 0x1005 - ClipDisable
 *****************************************************************************/
// DETECT [31:31] (1) - Disable clipping detection
        /* start bit  */ #define GX_XF_CLIPDISABLE_DETECT_B 31
        /* end bit    */ #define GX_XF_CLIPDISABLE_DETECT_E 31
        /* bit size   */ #define GX_XF_CLIPDISABLE_DETECT_SZ 1

        /* raw mask   */ #define GX_XF_CLIPDISABLE_DETECT_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_XF_CLIPDISABLE_DETECT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_CLIPDISABLE_DETECT_SHIFT 0

        /* get value  */ #define GX_XF_GET_CLIPDISABLE_DETECT(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_XF_SET_CLIPDISABLE_DETECT(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// REJECT [30:30] (1) - Disable trivial rejection
        /* start bit  */ #define GX_XF_CLIPDISABLE_REJECT_B 30
        /* end bit    */ #define GX_XF_CLIPDISABLE_REJECT_E 30
        /* bit size   */ #define GX_XF_CLIPDISABLE_REJECT_SZ 1

        /* raw mask   */ #define GX_XF_CLIPDISABLE_REJECT_MASK (((1 << 1) - 1) << (31 - 30))
        /* local mask */ #define GX_XF_CLIPDISABLE_REJECT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_CLIPDISABLE_REJECT_SHIFT 1

        /* get value  */ #define GX_XF_GET_CLIPDISABLE_REJECT(reg) GX_BITGET((reg), 30, 1)
        /* set value  */ #define GX_XF_SET_CLIPDISABLE_REJECT(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// ACCEL [29:29] (1) - Disable cpoly clipping acceleration
        /* start bit  */ #define GX_XF_CLIPDISABLE_ACCEL_B 29
        /* end bit    */ #define GX_XF_CLIPDISABLE_ACCEL_E 29
        /* bit size   */ #define GX_XF_CLIPDISABLE_ACCEL_SZ 1

        /* raw mask   */ #define GX_XF_CLIPDISABLE_ACCEL_MASK (((1 << 1) - 1) << (31 - 29))
        /* local mask */ #define GX_XF_CLIPDISABLE_ACCEL_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_CLIPDISABLE_ACCEL_SHIFT 2

        /* get value  */ #define GX_XF_GET_CLIPDISABLE_ACCEL(reg) GX_BITGET((reg), 29, 1)
        /* set value  */ #define GX_XF_SET_CLIPDISABLE_ACCEL(reg, x) ((reg) = GX_BITSET((reg), 29, 1, x))


/******************************************************************************
 * XF register 0x1007 - Perf1
 *****************************************************************************/
// TARGET [25:31] (7) - Target performance (Cycles/vertex)
        /* start bit  */ #define GX_XF_PERF1_TARGET_B 25
        /* end bit    */ #define GX_XF_PERF1_TARGET_E 31
        /* bit size   */ #define GX_XF_PERF1_TARGET_SZ 7

        /* raw mask   */ #define GX_XF_PERF1_TARGET_MASK (((1 << 7) - 1) << (31 - 31))
        /* local mask */ #define GX_XF_PERF1_TARGET_LMASK ((1 << 7) - 1)
        /* bit shift  */ #define GX_XF_PERF1_TARGET_SHIFT 0

        /* get value  */ #define GX_XF_GET_PERF1_TARGET(reg) GX_BITGET((reg), 25, 7)
        /* set value  */ #define GX_XF_SET_PERF1_TARGET(reg, x) ((reg) = GX_BITSET((reg), 25, 7, x))


/******************************************************************************
 * XF register 0x1008 - InVertexSpec
 *****************************************************************************/
// CLR [30:31] (2)
        /* start bit  */ #define GX_XF_INVERTEXSPEC_CLR_B 30
        /* end bit    */ #define GX_XF_INVERTEXSPEC_CLR_E 31
        /* bit size   */ #define GX_XF_INVERTEXSPEC_CLR_SZ 2

        /* raw mask   */ #define GX_XF_INVERTEXSPEC_CLR_MASK (((1 << 2) - 1) << (31 - 31))
        /* local mask */ #define GX_XF_INVERTEXSPEC_CLR_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_XF_INVERTEXSPEC_CLR_SHIFT 0

        /* get value  */ #define GX_XF_GET_INVERTEXSPEC_CLR(reg) GX_BITGET((reg), 30, 2)
        /* set value  */ #define GX_XF_SET_INVERTEXSPEC_CLR(reg, x) ((reg) = GX_BITSET((reg), 30, 2, x))

// NRM [28:29] (2)
        /* start bit  */ #define GX_XF_INVERTEXSPEC_NRM_B 28
        /* end bit    */ #define GX_XF_INVERTEXSPEC_NRM_E 29
        /* bit size   */ #define GX_XF_INVERTEXSPEC_NRM_SZ 2

        /* raw mask   */ #define GX_XF_INVERTEXSPEC_NRM_MASK (((1 << 2) - 1) << (31 - 29))
        /* local mask */ #define GX_XF_INVERTEXSPEC_NRM_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_XF_INVERTEXSPEC_NRM_SHIFT 2

        /* get value  */ #define GX_XF_GET_INVERTEXSPEC_NRM(reg) GX_BITGET((reg), 28, 2)
        /* set value  */ #define GX_XF_SET_INVERTEXSPEC_NRM(reg, x) ((reg) = GX_BITSET((reg), 28, 2, x))

// TXC [24:27] (4)
        /* start bit  */ #define GX_XF_INVERTEXSPEC_TXC_B 24
        /* end bit    */ #define GX_XF_INVERTEXSPEC_TXC_E 27
        /* bit size   */ #define GX_XF_INVERTEXSPEC_TXC_SZ 4

        /* raw mask   */ #define GX_XF_INVERTEXSPEC_TXC_MASK (((1 << 4) - 1) << (31 - 27))
        /* local mask */ #define GX_XF_INVERTEXSPEC_TXC_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_XF_INVERTEXSPEC_TXC_SHIFT 4

        /* get value  */ #define GX_XF_GET_INVERTEXSPEC_TXC(reg) GX_BITGET((reg), 24, 4)
        /* set value  */ #define GX_XF_SET_INVERTEXSPEC_TXC(reg, x) ((reg) = GX_BITSET((reg), 24, 4, x))


/******************************************************************************
 * XF register 0x100E - Color0Cntrl
 *****************************************************************************/
// MATSRC [31:31] (1)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_MATSRC_B 31
        /* end bit    */ #define GX_XF_COLOR0CNTRL_MATSRC_E 31
        /* bit size   */ #define GX_XF_COLOR0CNTRL_MATSRC_SZ 1

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_MATSRC_MASK (((1 << 1) - 1) << (31 - 31))
        /* local mask */ #define GX_XF_COLOR0CNTRL_MATSRC_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_MATSRC_SHIFT 0

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_MATSRC(reg) GX_BITGET((reg), 31, 1)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_MATSRC(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// LIGHT [30:30] (1)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_LIGHT_B 30
        /* end bit    */ #define GX_XF_COLOR0CNTRL_LIGHT_E 30
        /* bit size   */ #define GX_XF_COLOR0CNTRL_LIGHT_SZ 1

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_LIGHT_MASK (((1 << 1) - 1) << (31 - 30))
        /* local mask */ #define GX_XF_COLOR0CNTRL_LIGHT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_LIGHT_SHIFT 1

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_LIGHT(reg) GX_BITGET((reg), 30, 1)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_LIGHT(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// LMASKLO [26:29] (4)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_LMASKLO_B 26
        /* end bit    */ #define GX_XF_COLOR0CNTRL_LMASKLO_E 29
        /* bit size   */ #define GX_XF_COLOR0CNTRL_LMASKLO_SZ 4

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_LMASKLO_MASK (((1 << 4) - 1) << (31 - 29))
        /* local mask */ #define GX_XF_COLOR0CNTRL_LMASKLO_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_LMASKLO_SHIFT 2

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_LMASKLO(reg) GX_BITGET((reg), 26, 4)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_LMASKLO(reg, x) ((reg) = GX_BITSET((reg), 26, 4, x))

// AMBSRC [25:25] (1)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_AMBSRC_B 25
        /* end bit    */ #define GX_XF_COLOR0CNTRL_AMBSRC_E 25
        /* bit size   */ #define GX_XF_COLOR0CNTRL_AMBSRC_SZ 1

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_AMBSRC_MASK (((1 << 1) - 1) << (31 - 25))
        /* local mask */ #define GX_XF_COLOR0CNTRL_AMBSRC_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_AMBSRC_SHIFT 6

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_AMBSRC(reg) GX_BITGET((reg), 25, 1)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_AMBSRC(reg, x) ((reg) = GX_BITSET((reg), 25, 1, x))

// DIFFUSEATTN [23:24] (2)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_B 23
        /* end bit    */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_E 24
        /* bit size   */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_SZ 2

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_MASK (((1 << 2) - 1) << (31 - 24))
        /* local mask */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_LMASK ((1 << 2) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_SHIFT 7

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_DIFFUSEATTN(reg) GX_BITGET((reg), 23, 2)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_DIFFUSEATTN(reg, x) ((reg) = GX_BITSET((reg), 23, 2, x))

// ATTNENABLE [22:22] (1)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_B 22
        /* end bit    */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_E 22
        /* bit size   */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_SZ 1

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_MASK (((1 << 1) - 1) << (31 - 22))
        /* local mask */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_SHIFT 9

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_ATTNENABLE(reg) GX_BITGET((reg), 22, 1)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_ATTNENABLE(reg, x) ((reg) = GX_BITSET((reg), 22, 1, x))

// ATTNSELECT [21:21] (1)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_B 21
        /* end bit    */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_E 21
        /* bit size   */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_SZ 1

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_MASK (((1 << 1) - 1) << (31 - 21))
        /* local mask */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_SHIFT 10

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_ATTNSELECT(reg) GX_BITGET((reg), 21, 1)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_ATTNSELECT(reg, x) ((reg) = GX_BITSET((reg), 21, 1, x))

// LMASKHI [17:20] (4)
        /* start bit  */ #define GX_XF_COLOR0CNTRL_LMASKHI_B 17
        /* end bit    */ #define GX_XF_COLOR0CNTRL_LMASKHI_E 20
        /* bit size   */ #define GX_XF_COLOR0CNTRL_LMASKHI_SZ 4

        /* raw mask   */ #define GX_XF_COLOR0CNTRL_LMASKHI_MASK (((1 << 4) - 1) << (31 - 20))
        /* local mask */ #define GX_XF_COLOR0CNTRL_LMASKHI_LMASK ((1 << 4) - 1)
        /* bit shift  */ #define GX_XF_COLOR0CNTRL_LMASKHI_SHIFT 11

        /* get value  */ #define GX_XF_GET_COLOR0CNTRL_LMASKHI(reg) GX_BITGET((reg), 17, 4)
        /* set value  */ #define GX_XF_SET_COLOR0CNTRL_LMASKHI(reg, x) ((reg) = GX_BITSET((reg), 17, 4, x))


/******************************************************************************
 * XF register 0x1018 - MatrixIndex0
 *****************************************************************************/
// GEOM [26:31] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX0_GEOM_B 26
        /* end bit    */ #define GX_XF_MATRIXINDEX0_GEOM_E 31
        /* bit size   */ #define GX_XF_MATRIXINDEX0_GEOM_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX0_GEOM_MASK (((1 << 6) - 1) << (31 - 31))
        /* local mask */ #define GX_XF_MATRIXINDEX0_GEOM_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX0_GEOM_SHIFT 0

        /* get value  */ #define GX_XF_GET_MATRIXINDEX0_GEOM(reg) GX_BITGET((reg), 26, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX0_GEOM(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

// TEX0 [20:25] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX0_TEX0_B 20
        /* end bit    */ #define GX_XF_MATRIXINDEX0_TEX0_E 25
        /* bit size   */ #define GX_XF_MATRIXINDEX0_TEX0_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX0_MASK (((1 << 6) - 1) << (31 - 25))
        /* local mask */ #define GX_XF_MATRIXINDEX0_TEX0_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX0_SHIFT 6

        /* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX0(reg) GX_BITGET((reg), 20, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX0(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// TEX1 [14:19] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX0_TEX1_B 14
        /* end bit    */ #define GX_XF_MATRIXINDEX0_TEX1_E 19
        /* bit size   */ #define GX_XF_MATRIXINDEX0_TEX1_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX1_MASK (((1 << 6) - 1) << (31 - 19))
        /* local mask */ #define GX_XF_MATRIXINDEX0_TEX1_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX1_SHIFT 12

        /* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX1(reg) GX_BITGET((reg), 14, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX1(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX2 [8:13] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX0_TEX2_B 8
        /* end bit    */ #define GX_XF_MATRIXINDEX0_TEX2_E 13
        /* bit size   */ #define GX_XF_MATRIXINDEX0_TEX2_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX2_MASK (((1 << 6) - 1) << (31 - 13))
        /* local mask */ #define GX_XF_MATRIXINDEX0_TEX2_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX2_SHIFT 18

        /* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX2(reg) GX_BITGET((reg), 8, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX2(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))

// TEX3 [2:7] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX0_TEX3_B 2
        /* end bit    */ #define GX_XF_MATRIXINDEX0_TEX3_E 7
        /* bit size   */ #define GX_XF_MATRIXINDEX0_TEX3_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX3_MASK (((1 << 6) - 1) << (31 - 7))
        /* local mask */ #define GX_XF_MATRIXINDEX0_TEX3_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX3_SHIFT 24

        /* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX3(reg) GX_BITGET((reg), 2, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX3(reg, x) ((reg) = GX_BITSET((reg), 2, 6, x))


/******************************************************************************
 * XF register 0x1019 - MatrixIndex1
 *****************************************************************************/
// TEX4 [26:31] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX1_TEX4_B 26
        /* end bit    */ #define GX_XF_MATRIXINDEX1_TEX4_E 31
        /* bit size   */ #define GX_XF_MATRIXINDEX1_TEX4_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX4_MASK (((1 << 6) - 1) << (31 - 31))
        /* local mask */ #define GX_XF_MATRIXINDEX1_TEX4_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX4_SHIFT 0

        /* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX4(reg) GX_BITGET((reg), 26, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX4(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

// TEX5 [20:25] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX1_TEX5_B 20
        /* end bit    */ #define GX_XF_MATRIXINDEX1_TEX5_E 25
        /* bit size   */ #define GX_XF_MATRIXINDEX1_TEX5_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX5_MASK (((1 << 6) - 1) << (31 - 25))
        /* local mask */ #define GX_XF_MATRIXINDEX1_TEX5_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX5_SHIFT 6

        /* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX5(reg) GX_BITGET((reg), 20, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX5(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// TEX6 [14:19] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX1_TEX6_B 14
        /* end bit    */ #define GX_XF_MATRIXINDEX1_TEX6_E 19
        /* bit size   */ #define GX_XF_MATRIXINDEX1_TEX6_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX6_MASK (((1 << 6) - 1) << (31 - 19))
        /* local mask */ #define GX_XF_MATRIXINDEX1_TEX6_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX6_SHIFT 12

        /* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX6(reg) GX_BITGET((reg), 14, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX6(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX7 [8:13] (6)
        /* start bit  */ #define GX_XF_MATRIXINDEX1_TEX7_B 8
        /* end bit    */ #define GX_XF_MATRIXINDEX1_TEX7_E 13
        /* bit size   */ #define GX_XF_MATRIXINDEX1_TEX7_SZ 6

        /* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX7_MASK (((1 << 6) - 1) << (31 - 13))
        /* local mask */ #define GX_XF_MATRIXINDEX1_TEX7_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX7_SHIFT 18

        /* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX7(reg) GX_BITGET((reg), 8, 6)
        /* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX7(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))


/******************************************************************************
 * XF structure - Tex
 *****************************************************************************/
// PROJTYPE [30:30] (1)
        /* start bit  */ #define GX_XF_TEX_PROJTYPE_B 30
        /* end bit    */ #define GX_XF_TEX_PROJTYPE_E 30
        /* bit size   */ #define GX_XF_TEX_PROJTYPE_SZ 1

        /* raw mask   */ #define GX_XF_TEX_PROJTYPE_MASK (((1 << 1) - 1) << (31 - 30))
        /* local mask */ #define GX_XF_TEX_PROJTYPE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_TEX_PROJTYPE_SHIFT 1

        /* get value  */ #define GX_XF_GET_TEX_PROJTYPE(reg) GX_BITGET((reg), 30, 1)
        /* set value  */ #define GX_XF_SET_TEX_PROJTYPE(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// INPUTFORM [29:29] (1)
        /* start bit  */ #define GX_XF_TEX_INPUTFORM_B 29
        /* end bit    */ #define GX_XF_TEX_INPUTFORM_E 29
        /* bit size   */ #define GX_XF_TEX_INPUTFORM_SZ 1

        /* raw mask   */ #define GX_XF_TEX_INPUTFORM_MASK (((1 << 1) - 1) << (31 - 29))
        /* local mask */ #define GX_XF_TEX_INPUTFORM_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_TEX_INPUTFORM_SHIFT 2

        /* get value  */ #define GX_XF_GET_TEX_INPUTFORM(reg) GX_BITGET((reg), 29, 1)
        /* set value  */ #define GX_XF_SET_TEX_INPUTFORM(reg, x) ((reg) = GX_BITSET((reg), 29, 1, x))

// TEXGENTYPE [25:27] (3)
        /* start bit  */ #define GX_XF_TEX_TEXGENTYPE_B 25
        /* end bit    */ #define GX_XF_TEX_TEXGENTYPE_E 27
        /* bit size   */ #define GX_XF_TEX_TEXGENTYPE_SZ 3

        /* raw mask   */ #define GX_XF_TEX_TEXGENTYPE_MASK (((1 << 3) - 1) << (31 - 27))
        /* local mask */ #define GX_XF_TEX_TEXGENTYPE_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_XF_TEX_TEXGENTYPE_SHIFT 4

        /* get value  */ #define GX_XF_GET_TEX_TEXGENTYPE(reg) GX_BITGET((reg), 25, 3)
        /* set value  */ #define GX_XF_SET_TEX_TEXGENTYPE(reg, x) ((reg) = GX_BITSET((reg), 25, 3, x))

// SRCROW [20:24] (5)
        /* start bit  */ #define GX_XF_TEX_SRCROW_B 20
        /* end bit    */ #define GX_XF_TEX_SRCROW_E 24
        /* bit size   */ #define GX_XF_TEX_SRCROW_SZ 5

        /* raw mask   */ #define GX_XF_TEX_SRCROW_MASK (((1 << 5) - 1) << (31 - 24))
        /* local mask */ #define GX_XF_TEX_SRCROW_LMASK ((1 << 5) - 1)
        /* bit shift  */ #define GX_XF_TEX_SRCROW_SHIFT 7

        /* get value  */ #define GX_XF_GET_TEX_SRCROW(reg) GX_BITGET((reg), 20, 5)
        /* set value  */ #define GX_XF_SET_TEX_SRCROW(reg, x) ((reg) = GX_BITSET((reg), 20, 5, x))

// BUMPSRCTEX [17:19] (3)
        /* start bit  */ #define GX_XF_TEX_BUMPSRCTEX_B 17
        /* end bit    */ #define GX_XF_TEX_BUMPSRCTEX_E 19
        /* bit size   */ #define GX_XF_TEX_BUMPSRCTEX_SZ 3

        /* raw mask   */ #define GX_XF_TEX_BUMPSRCTEX_MASK (((1 << 3) - 1) << (31 - 19))
        /* local mask */ #define GX_XF_TEX_BUMPSRCTEX_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_XF_TEX_BUMPSRCTEX_SHIFT 12

        /* get value  */ #define GX_XF_GET_TEX_BUMPSRCTEX(reg) GX_BITGET((reg), 17, 3)
        /* set value  */ #define GX_XF_SET_TEX_BUMPSRCTEX(reg, x) ((reg) = GX_BITSET((reg), 17, 3, x))

// BUMPSRCLIGHT [14:16] (3)
        /* start bit  */ #define GX_XF_TEX_BUMPSRCLIGHT_B 14
        /* end bit    */ #define GX_XF_TEX_BUMPSRCLIGHT_E 16
        /* bit size   */ #define GX_XF_TEX_BUMPSRCLIGHT_SZ 3

        /* raw mask   */ #define GX_XF_TEX_BUMPSRCLIGHT_MASK (((1 << 3) - 1) << (31 - 16))
        /* local mask */ #define GX_XF_TEX_BUMPSRCLIGHT_LMASK ((1 << 3) - 1)
        /* bit shift  */ #define GX_XF_TEX_BUMPSRCLIGHT_SHIFT 15

        /* get value  */ #define GX_XF_GET_TEX_BUMPSRCLIGHT(reg) GX_BITGET((reg), 14, 3)
        /* set value  */ #define GX_XF_SET_TEX_BUMPSRCLIGHT(reg, x) ((reg) = GX_BITSET((reg), 14, 3, x))


/******************************************************************************
 * XF structure - DualTex
 *****************************************************************************/
// BASEROW [26:31] (6) - Indicates which is the base row of the transform matrix
        /* start bit  */ #define GX_XF_DUALTEX_BASEROW_B 26
        /* end bit    */ #define GX_XF_DUALTEX_BASEROW_E 31
        /* bit size   */ #define GX_XF_DUALTEX_BASEROW_SZ 6

        /* raw mask   */ #define GX_XF_DUALTEX_BASEROW_MASK (((1 << 6) - 1) << (31 - 31))
        /* local mask */ #define GX_XF_DUALTEX_BASEROW_LMASK ((1 << 6) - 1)
        /* bit shift  */ #define GX_XF_DUALTEX_BASEROW_SHIFT 0

        /* get value  */ #define GX_XF_GET_DUALTEX_BASEROW(reg) GX_BITGET((reg), 26, 6)
        /* set value  */ #define GX_XF_SET_DUALTEX_BASEROW(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

// NORMALIZE [23:23] (1) - Normalize texcoord before sending transform
        /* start bit  */ #define GX_XF_DUALTEX_NORMALIZE_B 23
        /* end bit    */ #define GX_XF_DUALTEX_NORMALIZE_E 23
        /* bit size   */ #define GX_XF_DUALTEX_NORMALIZE_SZ 1

        /* raw mask   */ #define GX_XF_DUALTEX_NORMALIZE_MASK (((1 << 1) - 1) << (31 - 23))
        /* local mask */ #define GX_XF_DUALTEX_NORMALIZE_LMASK ((1 << 1) - 1)
        /* bit shift  */ #define GX_XF_DUALTEX_NORMALIZE_SHIFT 8

        /* get value  */ #define GX_XF_GET_DUALTEX_NORMALIZE(reg) GX_BITGET((reg), 23, 1)
        /* set value  */ #define GX_XF_SET_DUALTEX_NORMALIZE(reg, x) ((reg) = GX_BITSET((reg), 23, 1, x))

// clang-format on

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
#ifndef RVL_SDK_GX_INIT_H
#define RVL_SDK_GX_INIT_H
/* "libs/RVL_SDK/include/revolution/GX/GXInit.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXInit.h" line 4 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX/GXInit.h" line 5 "revolution/GX/GXTransform.h" */
#ifndef RVL_SDK_GX_TRANSFORM_H
#define RVL_SDK_GX_TRANSFORM_H
/* "libs/RVL_SDK/include/revolution/GX/GXTransform.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXTransform.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXTransform.h" line 6 "revolution/MTX.h" */
#ifndef RVL_SDK_PUBLIC_MTX_H
#define RVL_SDK_PUBLIC_MTX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/MTX.h" line 6 "revolution/MTX/mtx.h" */
#ifndef RVL_SDK_MTX_MTX_H
#define RVL_SDK_MTX_MTX_H
/* "libs/RVL_SDK/include/revolution/MTX/mtx.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtx.h" line 4 "revolution/MTX/mtxtypes.h" */
#ifndef RVL_SDK_MTX_TYPES_H
#define RVL_SDK_MTX_TYPES_H
/* "libs/RVL_SDK/include/revolution/MTX/mtxtypes.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef f32 Mtx[3][4];
typedef f32 Mtx44[4][4];

typedef struct Vec {
    f32 x, y, z;
} Vec;

typedef struct Vec2 {
    f32 x, y;
} Vec2;

typedef struct Quaternion {
    f32 x, y, z, w;
} Quaternion;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

// TODO(kiwi) Create macros for PS/C_ functions
void PSMTXIdentity(Mtx);
void PSMTXCopy(const Mtx, Mtx);
void PSMTXConcat(const Mtx, const Mtx, Mtx);
void PSMTXConcatArray(const Mtx, const Mtx, Mtx, u32);
void PSMTXTranspose(const Mtx, Mtx);
u32 PSMTXInverse(const Mtx, Mtx);
u32 PSMTXInvXpose(const Mtx, Mtx);
void PSMTXRotRad(Mtx, char, f32);
void PSMTXRotTrig(Mtx, f32, f32, char);
void PSMTXRotAxisRad(Mtx, const Vec*, f32);
void PSMTXTrans(Mtx, f32, f32, f32);
void PSMTXTransApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXScale(Mtx, f32, f32, f32);
void PSMTXScaleApply(const Mtx, Mtx, f32, f32, f32);
void PSMTXQuat(Mtx, const Quaternion*);

void C_MTXLookAt(Mtx, const Vec*, const Vec*, const Vec*);
void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);
void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtx.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 7 "revolution/MTX/mtx44.h" */
#ifndef RVL_SDK_MTX_MTX44_H
#define RVL_SDK_MTX_MTX44_H
/* "libs/RVL_SDK/include/revolution/MTX/mtx44.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtx44.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSMTX44Copy(Mtx44, Mtx44);
void C_MTXFrustum(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);
void C_MTXPerspective(Mtx44 mtx, f32 fovy, f32 aspect, f32 n, f32 f);
void C_MTXOrtho(Mtx44 mtx, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtx44.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 8 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 9 "revolution/MTX/mtxvec.h" */
#ifndef RVL_SDK_MTX_MTXVEC_H
#define RVL_SDK_MTX_MTXVEC_H
/* "libs/RVL_SDK/include/revolution/MTX/mtxvec.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/mtxvec.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSMTXMultVec(const Mtx mtx, const Vec* vec, Vec* out);
void PSMTXMultVecSR(const Mtx, const Vec*, Vec*);
void PSMTXMultVecArraySR();

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/mtxvec.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 10 "revolution/MTX/quat.h" */
#ifndef RVL_SDK_MTX_QUAT_H
#define RVL_SDK_MTX_QUAT_H
/* "libs/RVL_SDK/include/revolution/MTX/quat.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/quat.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif


void PSQUATAdd(const Quaternion* a, const Quaternion* b, Quaternion* sum);
void PSQUATMultiply(const Quaternion* a, const Quaternion* b, Quaternion* prod);
void PSQUATScale(const Quaternion* in, Quaternion* out, f32 scale);
f32 PSQUATDotProduct(const Quaternion* a, const Quaternion* b);
void PSQUATNormalize(const Quaternion* in, Quaternion* out);
void PSQUATInverse(const Quaternion* in, Quaternion* out);
void C_QUATMtx(Quaternion* quat, const Mtx mtx);
void C_QUATLerp(const Quaternion* a, const Quaternion* b, Quaternion* out,
                f32 t);
void C_QUATSlerp(const Quaternion* a, const Quaternion* b, Quaternion* out,
                 f32 t);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/quat.h" */
/* "libs/RVL_SDK/include/revolution/MTX.h" line 11 "revolution/MTX/vec.h" */
#ifndef RVL_SDK_MTX_VEC_H
#define RVL_SDK_MTX_VEC_H
/* "libs/RVL_SDK/include/revolution/MTX/vec.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MTX/vec.h" line 4 "revolution/MTX/mtxtypes.h" */
/* end "revolution/MTX/mtxtypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void PSVECAdd(const Vec* a, const Vec* b, Vec* sum);
void PSVECScale(const Vec* in, Vec* out, f32 scale);
void PSVECNormalize(const Vec* in, Vec* out);
f32 PSVECMag(const Vec* v);
f32 PSVECDotProduct(const Vec* a, const Vec* b);
void PSVECCrossProduct(const Vec* a, const Vec* b, Vec* prod);
f32 PSVECSquareDistance(const Vec* a, const Vec* b);
void C_VECHalfAngle(const Vec* a, const Vec* b, Vec* half);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX/vec.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MTX.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define GX_PROJECTION_SZ 7
#define GX_VIEWPORT_SZ 6

typedef enum {
    GX_FIELD_EVEN,
    GX_FIELD_ODD,
} GXInterlaceField;

void GXSetProjection(const Mtx44 proj, GXProjectionType type);
void GXSetProjectionv(const f32 proj[GX_PROJECTION_SZ]);
void GXGetProjectionv(f32 proj[GX_PROJECTION_SZ]);
void GXLoadPosMtxImm(const Mtx mtx, u32 id);
void GXLoadPosMtxIndx(u16 index, u32 id);
void GXLoadNrmMtxImm(const Mtx mtx, u32 id);
void GXLoadNrmMtxIndx3x3(u16 index, u32 id);
void GXSetCurrentMtx(u32 id);
void GXLoadTexMtxImm(const Mtx mtx, u32 id, GXMtxType type);
void GXSetViewportJitter(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far,
                         u32 nextField);
void GXSetViewport(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far);
void GXGetViewportv(f32 view[GX_VIEWPORT_SZ]);
void GXSetZScaleOffset(f32 scale, f32 offset);
void GXSetScissor(u32 x, u32 y, u32 w, u32 h);
void GXGetScissor(u32* x, u32* y, u32* w, u32* h);
void GXSetScissorBoxOffset(u32 ox, u32 oy);
void GXSetClipMode(GXClipMode mode);

void __GXSetProjection(void);
void __GXSetViewport(void);
void __GXSetMatrixIndex(GXAttr index);

static void GXSetViewportv(const f32 view[GX_VIEWPORT_SZ]) {
    GXSetViewport(view[0], view[1], view[2], view[3], view[4], view[5]);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXTransform.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXData {
    union {
        UNKWORD WORD_0x0;
        struct {
            u16 SHORT_0x0;      // at 0x0
            u16 lastWriteWasXF; // at 0x2
        };
    }; // at 0x0
    u16 SHORT_0x4;
    u16 vlim;      // at 0x6
    u32 cpCtrlReg; // at 0x8
    u32 cpStatReg; // at 0xC
    char UNK_0x10[0x4];
    u32 vcdLoReg;            // at 0x14
    u32 vcdHiReg;            // at 0x18
    u32 vatA[GX_MAX_VTXFMT]; // at 0x1C
    u32 vatB[GX_MAX_VTXFMT]; // at 0x3C
    u32 vatC[GX_MAX_VTXFMT]; // at 0x5C
    u32 linePtWidth;         // at 0x7C
    u32 matrixIndex0;        // at 0x80
    u32 matrixIndex1;        // at 0x84
    char UNK_0x88[0xA8 - 0x88];
    GXColor ambColors[2];             // at 0xA8
    GXColor matColors[2];             // at 0xB0
    u32 colorControl[4];              // at 0xB8
    u32 texRegs[GX_MAX_TEXCOORD];     // at 0xC8
    u32 dualTexRegs[GX_MAX_TEXCOORD]; // at 0xE8
    u32 txcRegs[GX_MAX_TEXCOORD];     // at 0x108
    char UNK_0x128[0x148 - 0x128];
    u32 scissorTL; // at 0x148
    u32 scissorBR; // at 0x14C
    char UNK_0x150[0x170 - 0x150];
    u32 ras1_iref; // at 0x170
    u32 ind_imask; // at 0x174
    u32 ras1_ss0;  // at 0x178
    u32 ras1_ss1;  // at 0x17C
    char UNK_0x180[0x220 - 0x180];
    u32 blendMode; // at 0x220
    u32 dstAlpha;  // at 0x224
    u32 zMode;     // at 0x228
    u32 zControl;  // at 0x22C
    char UNK_0x230[0x254 - 0x230];
    u32 genMode; // at 0x254
    char UNK_0x258[0x520 - 0x258];
    GXAttrType normalType;          // at 0x520
    GXBool normal;                  // at 0x524
    GXBool binormal;                // at 0x525
    GXProjectionType projType;      // at 0x528
    f32 proj[GX_PROJECTION_SZ - 1]; // at 0x52C
    union {
        struct {
            f32 vpOx;   // at 0x544
            f32 vpOy;   // at 0x548
            f32 vpSx;   // at 0x54C
            f32 vpSy;   // at 0x550
            f32 vpNear; // at 0x554
            f32 vpFar;  // at 0x558
        };
        f32 view[GX_VIEWPORT_SZ];
    }; // at 0x544
    f32 offsetZ; // at 0x55C
    f32 scaleZ;  // at 0x560
    char UNK_0x564[0x5F8 - 0x564];
    GXBool dlistActive; // at 0x5F8
    GXBool dlistSave;   // at 0x5F9
    u8 BYTE_0x5FA;
    u8 vatDirtyFlags; // at 0x5FB
    u32 gxDirtyFlags; // at 0x5FC
} GXData;

extern GXData* const __GXData;

// I hate typing this name out
#define gxdt __GXData

GXFifoObj* GXInit(void*, u32);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
#ifndef RVL_SDK_GX_LIGHT_H
#define RVL_SDK_GX_LIGHT_H
/* "libs/RVL_SDK/include/revolution/GX/GXLight.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXLight.h" line 4 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX/GXLight.h" line 5 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

GX_PUBLIC_STRUCT_DECL(GXLightObj, 64);

void GXInitLightAttn(GXLightObj* light, f32 aa, f32 ab, f32 ac, f32 ka, f32 kb,
                     f32 kc);
void GXInitLightAttnA(GXLightObj* light, f32 a, f32 b, f32 c);
void GXInitLightAttnK(GXLightObj* light, f32 a, f32 b, f32 c);
void GXInitLightSpot(GXLightObj* light, f32 angle, GXSpotFn fn);
void GXInitLightDistAttn(GXLightObj* light, f32 distance, f32 brightness,
                         GXDistAttnFn fn);
void GXInitLightPos(GXLightObj* light, f32 x, f32 y, f32 z);
void GXGetLightPos(const GXLightObj* light, f32* x, f32* y, f32* z);
void GXInitLightDir(GXLightObj* light, f32 x, f32 y, f32 z);
void GXGetLightDir(const GXLightObj* light, f32* x, f32* y, f32* z);
void GXInitSpecularDir(GXLightObj* light, f32 x, f32 y, f32 z);
void GXInitLightColor(GXLightObj* light, GXColor color);
void GXLoadLightObjImm(const GXLightObj* light, GXLightID id);
void GXLoadLightObjIndx(u16 index, GXLightID id);
void GXSetChanAmbColor(GXChannelID chan, GXColor color);
void GXSetChanMatColor(GXChannelID chan, GXColor color);
void GXSetNumChans(u8 num);
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc ambSrc,
                   GXColorSrc matSrc, GXLightID lightMask, GXDiffuseFn diffFn,
                   GXAttnFn attnFn);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
#ifndef RVL_SDK_GX_MISC_H
#define RVL_SDK_GX_MISC_H
/* "libs/RVL_SDK/include/revolution/GX/GXMisc.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GXDrawDoneCallback)(void);
typedef void (*GXDrawSyncCallback)(u16 token);

void GXSetMisc(UNKWORD token, UNKWORD val);
void GXFlush(void);
void GXResetWriteGatherPipe(void);

void GXAbortFrame(void);

void GXSetDrawSync(u16 token);
u16 GXReadDrawSync(void);

void GXDrawDone(void);
void GXSetDrawDone(void);
void GXPixModeSync(void);

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback);
GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
#ifndef RVL_SDK_GX_PIXEL_H
#define RVL_SDK_GX_PIXEL_H
/* "libs/RVL_SDK/include/revolution/GX/GXPixel.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXPixel.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXPixel.h" line 6 "revolution/MTX.h" */
/* end "revolution/MTX.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXFogAdjTable {
    u16 r[10]; // at 0x0
} GXFogAdjTable;

#define GX_FOG_ADJ_TABLE_SZ 10

void GXSetFog(GXFogType type, GXColor color, f32 start, f32 end, f32 near,
              f32 far);
void GXInitFogAdjTable(GXFogAdjTable* table, u16 width, const Mtx44 proj);
void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable* table);
void GXSetBlendMode(GXBlendMode mode, GXBlendFactor src, GXBlendFactor dst,
                    GXLogicOp op);
void GXSetColorUpdate(GXBool enable);
void GXSetAlphaUpdate(GXBool enable);
void GXSetZMode(GXBool enableTest, GXCompare func, GXBool enableUpdate);
void GXSetZCompLoc(GXBool beforeTex);
void GXSetPixelFmt(GXPixelFmt pixelFmt, GXZFmt16 zFmt);
void GXSetDither(GXBool enable);
void GXSetDstAlpha(GXBool enable, u8 alpha);
void GXSetFieldMask(GXBool enableEven, GXBool enableOdd);
void GXSetFieldMode(GXBool texLOD, GXBool adjustAR);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
#ifndef RVL_SDK_GX_TEV_H
#define RVL_SDK_GX_TEV_H
/* "libs/RVL_SDK/include/revolution/GX/GXTev.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXTev.h" line 4 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void GXSetTevOp(GXTevStageID, GXTevMode);
void GXSetTevColorIn(GXTevStageID, GXTevColorArg, GXTevColorArg, GXTevColorArg,
                     GXTevColorArg);
void GXSetTevAlphaIn(GXTevStageID, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg,
                     GXTevAlphaArg);
void GXSetTevColorOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, GXBool,
                     GXTevRegID);
void GXSetTevAlphaOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, GXBool,
                     GXTevRegID);

void GXSetTevColor(GXTevRegID, GXColor);
void GXSetTevColorS10(GXTevRegID, GXColorS10);

void GXSetTevKColor(GXTevKColorID, GXColor);
void GXSetTevKColorSel(GXTevStageID, GXTevKColorSel);
void GXSetTevKAlphaSel(GXTevStageID, GXTevKAlphaSel);
void GXSetTevSwapMode(GXTevStageID, GXTevSwapSel, GXTevSwapSel);
void GXSetTevSwapModeTable(GXTevSwapSel, GXTevColorChan, GXTevColorChan,
                           GXTevColorChan, GXTevColorChan);

void GXSetAlphaCompare(GXCompare, u8, GXAlphaOp, GXCompare, u8);
void GXSetZTexture(GXZTexOp, GXTexFmt, u32);
void GXSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID);
void GXSetNumTevStages(u8);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
#ifndef RVL_SDK_GX_TEXTURE_H
#define RVL_SDK_GX_TEXTURE_H
/* "libs/RVL_SDK/include/revolution/GX/GXTexture.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXTexture.h" line 4 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX/GXTexture.h" line 5 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

GX_PUBLIC_STRUCT_DECL(GXTexObj, 32);
GX_PUBLIC_STRUCT_DECL(GXTlutObj, 0x0C);

GX_PUBLIC_STRUCT_DECL(GXTexRegion, 16);
GX_PUBLIC_STRUCT_DECL(GXTlutRegion, 16);

typedef GXTexRegion* (*GXTexRegionCallback)(const GXTexObj* pObj,
                                            GXTexMapID map);

typedef GXTlutRegion* (*GXTlutRegionCallback)(u32 id);

void __GXSetSUTexRegs(void);

void GXInitTexObj(GXTexObj* obj, void* image, u16 w, u16 h, GXTexFmt fmt,
                  GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXBool mipmap);
void GXInitTexObjCI(GXTexObj*, void*, u16, u16, GXTexFmt, GXTexWrapMode,
                    GXTexWrapMode, GXBool, u32);
void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt,
                     f32 min_lod, f32 max_lod, f32 lod_bias, GXBool bias_clamp,
                     GXBool do_edge_lod, GXAnisotropy max_aniso);

void GXGetTexObjLODAll(GXTexObj* obj, GXTexFilter* min_filt,
                       GXTexFilter* mag_filt, f32* minLod, f32* maxLod,
                       f32* lodBias, GXBool* biasClampEnable,
                       GXBool* edgeLodEnable, GXAnisotropy* anisotropy);

GXTexWrapMode GXGetTexObjWrapS(GXTexObj* obj);
GXTexWrapMode GXGetTexObjWrapT(GXTexObj* obj);

u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
GXTexFmt GXGetTexObjFmt(const GXTexObj* obj);
GXBool GXGetTexObjMipMap(const GXTexObj* obj);

void GXLoadTexObj(const GXTexObj*, GXTexMapID);

void GXInitTexObjTlut(GXTexObj*, u32);
u32 GXGetTexObjTlut(GXTexObj*);

void GXInitTlutObj(GXTlutObj*, void*, GXTlutFmt, u16);

void GXLoadTlut(GXTlutObj*, u32);

void GXInvalidateTexAll(void);

void GXInitTexCacheRegion(GXTexRegion* pRegion, GXBool r4, u32 addrTMemEven,
                          u32 sizeTMemEven, u32 addrTMemOdd, u32 sizeTMemOdd);

void GXInitTlutRegion(GXTlutRegion* pRegion, u32 addrTMem, u32 sizeTMem);

GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback callback);
GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback callback);

u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap,
                       u8 max_lod);

// TODO
UNKTYPE GXSetTexCoordScaleManually(UNKWORD, UNKWORD, UNKWORD, UNKWORD);
UNKTYPE GXSetTexCoordCylWrap(UNKWORD, UNKWORD, UNKWORD);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
#ifndef RVL_SDK_GX_VERT_H
#define RVL_SDK_GX_VERT_H
/* "libs/RVL_SDK/include/revolution/GX/GXVert.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXVert.h" line 4 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
#ifdef __cplusplus
extern "C" {
#endif

static inline void GXCmd1u8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXCmd1u16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXCmd1u32(u32 ul) {
    WGPIPE.ui = ul;
}

static inline void GXPosition3f32(f32 x, f32 y, f32 z) {
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
}

static inline void GXPosition3u8(u8 x, u8 y, u8 z) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
    WGPIPE.uc = z;
}

static inline void GXPosition3s8(s8 x, s8 y, s8 z) {
    WGPIPE.c = x;
    WGPIPE.c = y;
    WGPIPE.c = z;
}

static inline void GXPosition3u16(u16 x, u16 y, u16 z) {
    WGPIPE.us = x;
    WGPIPE.us = y;
    WGPIPE.us = z;
}

static inline void GXPosition3s16(s16 x, s16 y, s16 z) {
    WGPIPE.s = x;
    WGPIPE.s = y;
    WGPIPE.s = z;
}

static inline void GXPosition2f32(f32 x, f32 y) {
    WGPIPE.f = x;
    WGPIPE.f = y;
}

static inline void GXPosition2u8(u8 x, u8 y) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
}

static inline void GXPosition2s8(s8 x, s8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXPosition2u16(u16 x, u16 y) {
    WGPIPE.us = x;
    WGPIPE.us = y;
}

static inline void GXPosition2s16(s16 x, s16 y) {
    WGPIPE.s = x;
    WGPIPE.s = y;
}

static inline void GXPosition1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXPosition1x8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXNormal3f32(f32 x, f32 y, f32 z) {
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
}

static inline void GXNormal3u16(s16 x, s16 y, s16 z) {
    WGPIPE.us = x;
    WGPIPE.us = y;
    WGPIPE.us = z;
}

static inline void GXNormal3s16(s16 x, s16 y, s16 z) {
    WGPIPE.s = x;
    WGPIPE.s = y;
    WGPIPE.s = z;
}

static inline void GXNormal3u8(u8 x, u8 y, u8 z) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
    WGPIPE.uc = z;
}

static inline void GXNormal3s8(s8 x, s8 y, s8 z) {
    WGPIPE.c = x;
    WGPIPE.c = y;
    WGPIPE.c = z;
}

static inline void GXNormal1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXNormal1x8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXColor4u8(u8 r, u8 g, u8 b, u8 a) {
    WGPIPE.uc = r;
    WGPIPE.uc = g;
    WGPIPE.uc = b;
    WGPIPE.uc = a;
}

static inline void GXColor1u32(u32 color) {
    WGPIPE.ui = color;
}

static inline void GXColor3u8(u8 r, u8 g, u8 b) {
    WGPIPE.uc = r;
    WGPIPE.uc = g;
    WGPIPE.uc = b;
}

static inline void GXColor1u16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXColor1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXColor1x8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXTexCoord2f32(f32 x, f32 y) {
    WGPIPE.f = x;
    WGPIPE.f = y;
}

static inline void GXTexCoord2s16(s16 x, s16 y) {
    WGPIPE.s = x;
    WGPIPE.s = y;
}

static inline void GXTexCoord2u16(u16 x, u16 y) {
    WGPIPE.us = x;
    WGPIPE.us = y;
}

static inline void GXTexCoord2s8(s8 x, s8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXTexCoord2u8(u8 x, u8 y) {
    WGPIPE.uc = x;
    WGPIPE.uc = y;
}

static inline void GXTexCoord1f32(f32 f) {
    WGPIPE.f = f;
}

static inline void GXTexCoord1s16(s16 s) {
    WGPIPE.s = s;
}

static inline void GXTexCoord1u16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXTexCoord1s8(s8 c) {
    WGPIPE.c = c;
}

static inline void GXTexCoord1u8(u8 uc) {
    WGPIPE.uc = uc;
}

static inline void GXTexCoord1x16(u16 us) {
    WGPIPE.us = us;
}

static inline void GXTexCoord1x8(u8 uc) {
    WGPIPE.uc = uc;
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */
/* "libs/RVL_SDK/include/revolution/VI/vi.h" line 5 "revolution/VI/vitypes.h" */
/* end "revolution/VI/vitypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*VIRetraceCallback)(u32 retraceCount);
typedef void (*VIPositionCallback)(s16 displayX, s16 displayY);

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback);

void VIInit(void);
void VIWaitForRetrace(void);

void VIConfigure(const GXRenderModeObj* rmo);
void VIConfigurePan(u16 x, u16 y, u16 w, u16 h);
void VIFlush(void);

void VISetNextFrameBuffer(void* fb);
void* VIGetCurrentFrameBuffer(void);

void VISetBlack(BOOL black);
s32 VIGetRetraceCount(void);

VITVFormat VIGetTvFormat(void);
VIScanMode VIGetScanMode(void);
u32 VIGetDTVStatus(void);
u32 VIGetDimmingCount(void);
BOOL VIEnableDimming(BOOL enable);
s32 VISetTimeToDimming(s32 time);
BOOL VIResetDimmingCount(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/VI/vi.h" */
/* "libs/RVL_SDK/include/revolution/VI.h" line 7 "revolution/VI/vi3in1.h" */
#ifndef RVL_SDK_VI_VI3IN1_H
#define RVL_SDK_VI_VI3IN1_H
/* "libs/RVL_SDK/include/revolution/VI/vi3in1.h" line 2 "types.h" */
/* end "types.h" */
/* "libs/RVL_SDK/include/revolution/VI/vi3in1.h" line 3 "revolution/vi/vitypes.h" */
/* end "revolution/vi/vitypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __VISetRGBModeImm(void);

void __VISetRevolutionModeSimple(void);

void VISetGamma(VIGamma gamma);
void VISetTrapFilter(VIBool filter);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/VI/vi3in1.h" */
/* "libs/RVL_SDK/include/revolution/VI.h" line 8 "revolution/VI/vihardware.h" */
#ifndef RVL_SDK_VI_HARDWARE_H
#define RVL_SDK_VI_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/VI/vihardware.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * VI hardware registers
 */
volatile u16 DECL_HW_REGS(VI) DECL_ADDRESS(0xCC002000);

/**
 * Hardware register indexes
 */
typedef enum {
    VI_VTR,    //!< 0xCC002000
    VI_DCR,    //!< 0xCC002002
    VI_HTR0_H, //!< 0xCC002004
    VI_HTR0_L, //!< 0xCC002006
    VI_HTR1_H, //!< 0xCC002008
    VI_HTR1_L, //!< 0xCC00200A
    VI_VTO_H,  //!< 0xCC00200C
    VI_VTO_L,  //!< 0xCC00200E
    VI_VTE_H,  //!< 0xCC002010
    VI_VTE_L,  //!< 0xCC002012
    VI_BBEI_H, //!< 0xCC002014
    VI_BBEI_L, //!< 0xCC002016
    VI_BBOI_H, //!< 0xCC002018
    VI_BBOI_L, //!< 0xCC00201A
    VI_TFBL_H, //!< 0xCC00201C
    VI_TFBL_L, //!< 0xCC00201E
    VI_TFBR_H, //!< 0xCC002020
    VI_TFBR_L, //!< 0xCC002022
    VI_BFBL_H, //!< 0xCC002024
    VI_BFBL_L, //!< 0xCC002026
    VI_BFBR_H, //!< 0xCC002028
    VI_BFBR_L, //!< 0xCC00202A
    VI_DPV,    //!< 0xCC00202C
    VI_DPH,    //!< 0xCC00202E
    VI_DI0_H,  //!< 0xCC002030
    VI_DI0_L,  //!< 0xCC002032
    VI_DI1_H,  //!< 0xCC002034
    VI_DI1_L,  //!< 0xCC002036
    VI_DI2_H,  //!< 0xCC002038
    VI_DI2_L,  //!< 0xCC00203A
    VI_DI3_H,  //!< 0xCC00203C
    VI_DI3_L,  //!< 0xCC00203E
    VI_DL0_H,  //!< 0xCC002040
    VI_DL0_L,  //!< 0xCC002042
    VI_DL1_H,  //!< 0xCC002044
    VI_DL1_L,  //!< 0xCC002046
    VI_HSW,    //!< 0xCC002048
    VI_HSR,    //!< 0xCC00204A
    VI_FCT0_H, //!< 0xCC00204C
    VI_FCT0_L, //!< 0xCC00204E
    VI_FCT1_H, //!< 0xCC002050
    VI_FCT1_L, //!< 0xCC002052
    VI_FCT2_H, //!< 0xCC002054
    VI_FCT2_L, //!< 0xCC002056
    VI_FCT3_H, //!< 0xCC002058
    VI_FCT3_L, //!< 0xCC00205A
    VI_FCT4_H, //!< 0xCC00205C
    VI_FCT4_L, //!< 0xCC00205E
    VI_FCT5_H, //!< 0xCC002060
    VI_FCT5_L, //!< 0xCC002062
    VI_FCT6_H, //!< 0xCC002064
    VI_FCT6_L, //!< 0xCC002066
    VI_0x68,   //!< 0xCC002068
    VI_0x6A,   //!< 0xCC00206A
    VI_VICLK,  //!< 0xCC00206C
    VI_VISEL,  //!< 0xCC00206E
    VI_0x70,   //!< 0xCC002070
    VI_HBE,    //!< 0xCC002072
    VI_HBS,    //!< 0xCC002074
    VI_0x76,   //!< 0xCC002076
    VI_0x78,   //!< 0xCC002078
    VI_0x7A,   //!< 0xCC00207A
    VI_0x7C,   //!< 0xCC00207C
} VIHwReg;

// DCR - Display Configuration Register
#define VI_DCR_ENB (1 << 0)
#define VI_DCR_RST (1 << 1)
#define VI_DCR_NIN (1 << 2)
#define VI_DCR_FMT (0b11 << 8)

// DI{n} - Display Interrupt Register N
#define VI_DI_INT (1 << 15)
#define VI_DI_ENB (1 << 12)

// VICLK - VI Clock Select Register
#define VI_VICLK_SPEED (1 << 0)
typedef enum { VI_VICLK_27MHZ, VI_VICLK_54MHZ } VIClkSpeed;

// VISEL - VI DTV Status Register
#define VI_VISEL_COMPONENT (1 << 0)

// HBE - Border HBE
#define VI_HBE_BRDR_EN (1 << 15)

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/VI/vihardware.h" */
/* "libs/RVL_SDK/include/revolution/VI.h" line 9 "revolution/VI/vitypes.h" */
/* end "revolution/VI/vitypes.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/VI.h" */

class IErrorWii {
public:
    IErrorWii(){}
    virtual ~IErrorWii(){}
    virtual void errorWiiCB() = 0;
};

class CErrorWii {
public:
    CErrorWii();
    ~CErrorWii();

    static void addCallback(IErrorWii* pError);
    static void removeCallback(IErrorWii* pError);

    static void initialize();
    static void destroy();

    static void errorHandler(u8 error, OSContext* ctx, u32 dsisr, u32 dar);
    static void postExceptionCallback();

    static void powerCallback();
    static bool isPowerCallbackCalled();
    static void resetCallback();
    static bool isResetCallbackCalled();

private:
    static void setErrorHandler(u16 error);
    static void resetErrorHandler(u16 error);

    //Unused in release
    mtl::fixed_vector<IErrorWii*, 8> mCallbackList; //0x0
    u8 unk24[4]; //part of previous class?
    OSContext mContext; //0x28
    u16 unk2F0;
    u32 unk2F4;
    u32 unk2F8;

    static CErrorWii* spInstance;
    static bool sPowerCallbackCalled;
    static bool sResetCallbackCalled;
};
/* end "monolib/util/CErrorWii.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 6 "monolib/util/CPathUtil.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/CPathUtil.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/util/CPathUtil.hpp" line 3 "monolib/util/FixStr.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/FixStr.hpp" line 2 "cstring" */
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
/* "libs/monolib/include/monolib/util/FixStr.hpp" line 3 "cstdio" */
#ifndef MSL_CPP_CSTDIO_H
#define MSL_CPP_CSTDIO_H
/* "libs/PowerPC_EABI_Support/include/stl/cstdio" line 2 "stdio.h" */
#ifndef MSL_STDIO_H
#define MSL_STDIO_H

/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 3 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 8 "PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" */
#ifndef STDIO_API_H
#define STDIO_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 4 "PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" */
#ifndef _MSL_COMMON_FILE_STRUC_H
#define _MSL_COMMON_FILE_STRUC_H
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" line 2 "types.h" */
/* end "types.h" */

typedef unsigned long __file_handle;
typedef unsigned long fpos_t;
typedef struct _FILE _FILE, *P_FILE;

#define __ungetc_buffer_size 2

enum __file_kinds {
    __closed_file,
    __disk_file,
    __console_file,
    __unavailable_file
};

enum __open_modes {
    __must_exist,
    __create_if_necessary,
    __create_or_truncate
};

enum __file_orientation {
    __unoriented,
    __char_oriented,
    __wide_oriented
};

enum __io_modes {
    __read = 1,
    __write = 2,
    __read_write = 3,
    __append = 4
};

typedef struct __file_modes {
    u32 open_mode : 2;
    u32 io_mode : 3;
    u32 buffer_mode : 2;
    u32 file_kind : 3;

#ifdef _MSL_WIDE_CHAR
    u32 file_orientation : 2;
#endif /* _MSL_WIDE_CHAR */

    u32 binary_io : 1;
} __file_modes;

enum __io_states {
    __neutral,
    __writing,
    __reading,
    __rereading
};

typedef struct __file_state {
    u32 io_state : 3;
    u32 free_buffer : 1;
    u8 eof;
    u8 error;
} __file_state;

typedef void* __ref_con;
typedef void (*__idle_proc)(void);
typedef int (*__pos_proc)(__file_handle file, fpos_t* position, int mode, __ref_con ref_con);
typedef int (*__io_proc)(__file_handle file, u8* buff, size_t* count, __ref_con ref_con);
typedef int (*__close_proc)(__file_handle file);

struct _FILE {
    __file_handle handle;                           // _00
    __file_modes mode;                              // _04
    __file_state state;                              // _08
    u8 is_dynamically_allowed;                      // _0C
    u8 char_buffer;                                 // _0D
    u8 char_buffer_overflow;                        // _0E
    u8 ungetc_buffer[__ungetc_buffer_size];         // _0F
    wchar_t ungetwc_buffer[__ungetc_buffer_size];   // _12
    u32 position;                                   // _18
    u8* buffer;                                   // _1C
    u32 buffer_size;                                // _20
    u8* buffer_ptr;                               // _24
    u32 buffer_len;                                 // _28
    u32 buffer_alignment;                           // _2C
    u32 saved_buffer_len;                           // _30
    u32 buffer_pos;                                 // _34
    __pos_proc position_proc;                       // _38
    __io_proc read_proc;                            // _3C
    __io_proc write_proc;                           // _40
    __close_proc close_proc;                        // _44
    __ref_con ref_con;                              // _48
    _FILE* next_file_struct;                        // _4C
};

typedef struct _FILE FILE;


#define _IONBF 0
#define _IOLBF 1
#define _IOFBF 2

// define standard C file pointer location names
#define SEEK_SET (0)
#define SEEK_CUR (1)
#define SEEK_END (2)

#define stdin &(__files[0])
#define stdout &(__files[1])
#define stderr &(__files[2])

#define _STATIC_FILES 4

extern FILE __files[];

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/file_struc.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" line 5 "wchar.h" */
#ifndef MSL_WCHAR_H
#define MSL_WCHAR_H

/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif

/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" */
#ifndef _WCHAR_IO_H
#define _WCHAR_IO_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" line 4 "stdio.h" */
/* end "stdio.h" */

int fwide(FILE* stream, int mode);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wchar_io.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" */
#ifndef MSL_WCSTOUL_H
#define MSL_WCSTOUL_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" line 3 "types.h" */
/* end "types.h" */


unsigned long __wcstoul(int, int, wint_t (*wReadProc)(void*, wint_t, int), void*, int*, int*, int*);
//__wcstoull
//wcstoul
//wcstoull
long wcstol(const wchar_t*, wchar_t**, int);
//wcstoll
//watoi
//watol

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wcstoul.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 11 "PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" */
#ifndef MSL_WMEM_H
#define MSL_WMEM_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" line 3 "types.h" */
/* end "types.h" */

wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n);
wchar_t* wmemchr(wchar_t* s, wchar_t c, int n);
void* memmove(void*, const void*, size_t);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wmem.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 12 "PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" */
#ifndef MSL_WPRINTF_H
#define MSL_WPRINTF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 4 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" line 5 "stdio.h" */
/* end "stdio.h" */

//wprintf
//wprintf_s
//fwprintf
//fwprintf_s
//vwprintf
//vwprintf_s
//vfwprintf
//vfwprintf_s
int swprintf(wchar_t*, size_t, const wchar_t*, ...);
//swprintf_s
//snwprintf_s
int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);
//vswprintf_s
//vsnwprintf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wprintf.h" */
/* "libs/PowerPC_EABI_Support/include/stl/wchar.h" line 13 "PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" */
#ifndef MSL_WSTRING_H
#define MSL_WSTRING_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" line 3 "types.h" */
/* end "types.h" */

size_t wcslen(const wchar_t*);
wchar_t* wcscpy(wchar_t*, const wchar_t*);
wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t);
wchar_t* wcscat(wchar_t*, const wchar_t*);
int wcscmp(const wchar_t*, const wchar_t*);
wchar_t* wcschr(const wchar_t*, wchar_t);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h" */

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "wchar.h" */

enum __ReadProcActions {
    __GetAChar,
    __UngetAChar,
    __TestForError
};

enum __WReadProcActions
{
    __GetAwChar,
    __UngetAwChar,
    __TestForwcsError
};

typedef struct {
    char* CharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __OutStrCtrl;

typedef struct{
    char* NextChar;
    int NullCharDetected;
} __InStrCtrl;

typedef struct {
    wchar_t * wCharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __wOutStrCtrl;

typedef struct {
    wchar_t * wNextChar;
    int    wNullCharDetected;
} __wInStrCtrl;

//__fread
size_t __fwrite(const void *pPtr, size_t memb_size, size_t num_memb, FILE *file);
int __StringRead(void *, int, int);
wint_t __wStringRead(void*, wint_t, int);

#endif // STDIO_API_H
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/stdio_api.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" */
#ifndef MSL_FILE_POS_H
#define MSL_FILE_POS_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" line 4 "stdio.h" */
/* end "stdio.h" */

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

int fseek(FILE* stream, u32 offset, int whence);
int _fseek(FILE* stream, u32 offset, int whence);
int ftell(FILE* stream);
int _ftell(FILE* stream);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/FILE_POS.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" */
#ifndef MSL_FILE_IO_H
#define MSL_FILE_IO_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" line 3 "types.h" */
/* end "types.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" line 4 "stdio.h" */
/* end "stdio.h" */

int fclose(FILE* file);
int fflush(FILE* file);

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/file_io.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 11 "PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" */
#ifndef MSL_PRINTF_H
#define MSL_PRINTF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" line 3 "stdarg.h" */
/* end "stdarg.h" */
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" line 4 "stdio.h" */
/* end "stdio.h" */


//printf
//printf_s
int fprintf(FILE*, const char* format, ...);
//fprintf_s
int vprintf(const char*, va_list);
//vprintf_s
//vfprintf
//vfprintf_s
int vsnprintf(char*, size_t, const char*, va_list);
//vsnprintf_s
int vsprintf(char*, const char*, va_list);
//vsprintf_s
int snprintf(char*, size_t, const char*, ...);
//snprintf_s
int sprintf(char*, const char*, ...);
//sprintf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/printf.h" */
/* "libs/PowerPC_EABI_Support/include/stl/stdio.h" line 12 "PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" */
#ifndef MSL_SCANF_H
#define MSL_SCANF_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" line 3 "stdarg.h" */
/* end "stdarg.h" */

//fscanf
//fscanf_s
//vscanf
//scanf
//scanf_s
//vfscanf
//vfscanf_s
int vsscanf(const char*, const char*, va_list);
//vsscanf_s
int sscanf(const char*, const char*, ...);
//sscanf_s

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/scanf.h" */

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "stdio.h" */
#ifdef __cplusplus

namespace std {
using ::fclose;
using ::fflush;
using ::FILE;
using ::ftell;
using ::fwide;
using ::snprintf;
using ::sprintf;
using ::sscanf;
using ::vprintf;
using ::vsnprintf;
using ::vsprintf;
} // namespace std

#endif
#endif
/* end "cstdio" */

namespace ml{

    template <size_t N>
    struct FixStr{
        FixStr(){
            clear();
        }

        //probably fake
        FixStr(bool initialize){
            if(initialize){
                clear();
            }
        }

        FixStr(const FixStr<N>& str){
            *this = str;
        }

        FixStr(const char* str){
            *this = str;
        }

        FixStr(const FixStr<N>& str, int pos, int length){
            copy(str, pos, length);
        }

        void copy(const FixStr<N>& str, int pos, int length){
            clear();
            if (str.empty()) return;

            //Copy entire string if length is -1
            if (length == npos) length = str.size();
            std::strncpy(mString, str.mString + pos, length);
            //Stop the string after the copied characters, and recalculate the length
            mString[length] = 0;
            mLength = std::strlen(mString);
        }

        void clear(){
            mString[0] = 0;
            mLength = 0;
        }

        void operator=(const FixStr<N>& str){
            mLength = std::strlen(str.mString);
            std::strcpy(mString, str.mString);
        }

        void operator=(const char* str){
            mLength = std::strlen(str);
            std::strcpy(mString, str);
        }

        void operator+=(const FixStr<N>& str){
            int strLength = std::strlen(str.mString);
            std::strcat(mString, str.mString);
            mLength += strLength;
        }

        void operator+=(const char* str){
            int strLength = std::strlen(str);
            std::strcat(mString, str);
            mLength += strLength;
        }

        bool operator==(const char* str) const {
            return std::strcmp(c_str(), str) == 0;
        }

        bool operator!=(const char* str) const {
            return std::strcmp(c_str(), str) != 0;
        }

        FixStr<N> operator+(const FixStr<N>& str) const {
            FixStr<N> result = *this;
            result += str;
            return result;
        }

        char operator[](int index) {
            return mString[index];
        }

        const char* c_str() const {
            return mString;
        }

        int size() const {
            return mLength;
        }

        bool empty() const {
            return size() == 0;
        }
        
        void format(const char* format, ...){
            //Why hardcode the buffer size to 256??
            char buffer[256];
            va_list args;
            va_start(args, format);
            std::vsnprintf(buffer, sizeof(buffer), format, args);
            *this = buffer;
        }

        //Sets the given string to the first characters of this string, up to the specified length.
        //TODO: This might just be substr, but when the start index is 0?
        const char* substr(int pos = 0, int length = npos) const {
            FixStr<N> str = FixStr(*this, pos, length);
            return str.c_str();
        }

        //void erase(int, int){}
        //void erase(const char*){}
        //append_int(const int&){}
        //void insert(int r4, char const* str, int r6){}
        //slice(int){}

        //also has version with const char& and const FixStr<64>&
        int append_back(const char* str){

        }

        int append_front(const char* str){

        }

        int find(const char* str, int pos) const {

        }

        int rfind(const char* str, int pos = npos) const {
            int length = mLength;
            
            if (length == 0) {
                //Return -1 if the string is empty
                return npos;
            }
            
            int strLength = std::strlen(str);

            char* string = (char*)mString + pos;

            for (char* p = string + length; p != string; p--) {
                if (!std::strncmp(p, str, strLength)) {
                    return (int)(p - mString);
                }
            }

            //Reached start of string without finding the string, return -1
            return npos;
        }

        int find_last_of(char c, int pos) const {

        }

        //TODO: this might be a CPathUtil inline?
        void unkInline1(const char* str){
            int index = rfind(str, -1);

            if(index != -1 && index + 1 < mLength){
                mString[index + 1] = 0;
                mLength = index;
            }
        }

    private:
        char mString[N];
        int mLength;

    public:
        static const int npos = -1;
    };

}
/* end "monolib/util/FixStr.hpp" */

namespace ml{

    class CPathUtil {
    public:
        static const char* getFilePtrFromPath(const char* pPath);
        static const char* getFileExtPtr(const char* pFilename);
        static void getNoPathExtName(FixStr<64>& param_1, const char* param_2);
        static void itoa(FixStr<16>& param_1, int param_2, int param_3);

        static inline void removeExt(FixStr<32>& str){
            int length = str.rfind(".", -1);

            if((u32)length + 1 > 1){
                str = str.substr(0, length);
            }
        }
    };

}
/* end "monolib/util/CPathUtil.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 7 "monolib/util/CStopwatchUtil.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/CStopwatchUtil.hpp" line 2 "types.h" */
/* end "types.h" */

//Unofficial name
class CStopwatchUtil {
    struct StopwatchEntry{
        enum Flags{
            FLAGS_OCCUPIED = (1 << 0)
        };

        u8 mFlags; //0x0
        char mString[23]; //0x1
        s64 mTime; //0x18
        float mCostTime; //0x20
        u8 unk24[0x28 - 0x24];
    };

public:
    static void initialize();
    static void entry(const char* pStr);
    static void updateCostTime(const char* pStr);
    static float getCostTime(const char* pStr);

    static inline StopwatchEntry* findEntry(const char* pStr);
    static inline StopwatchEntry* findEmptyEntry();

private:
    static const int MAX_ENTRIES = 16;

    static StopwatchEntry sEntries[MAX_ENTRIES];
};
/* end "monolib/util/CStopwatchUtil.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 8 "monolib/util/fixed_vector.hpp" */
/* end "monolib/util/fixed_vector.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 9 "monolib/util/FixStr.hpp" */
/* end "monolib/util/FixStr.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 10 "monolib/util/MemManager.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/MemManager.hpp" line 2 "decomp.h" */
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
/* "libs/monolib/include/monolib/util/MemManager.hpp" line 3 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/util/MemManager.hpp" line 4 "monolib/util/FixStr.hpp" */
/* end "monolib/util/FixStr.hpp" */
/* "libs/monolib/include/monolib/util/MemManager.hpp" line 5 "monolib/util/RawArray.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/RawArray.hpp" line 2 "types.h" */
/* end "types.h" */

namespace mtl{

template <typename T>
struct RawArrayEntry {
    u8 data[sizeof(T)];

    void initialize(){
        T* element = getValue();
        new (element) T();
    }

    T* getValue(){
        return reinterpret_cast<T*>(data);
    }
};

template <typename T, size_t N>
class RawArray {
public:
    static const int MAX_ELEMENTS = N;

    void initialize(){
        for(int i = 0; i < MAX_ELEMENTS; i++){
            values[i].initialize();
        }
    }

    T* operator[](u32 index){
        return values[index].getValue();
    }

private:
    RawArrayEntry<T> values[N];
};

} //namespace mtl
/* end "monolib/util/RawArray.hpp" */
/* "libs/monolib/include/monolib/util/MemManager.hpp" line 6 "cstring" */
/* end "cstring" */
/* "libs/monolib/include/monolib/util/MemManager.hpp" line 7 "cstddef" */
/* end "cstddef" */

namespace mtl {
    
    /*
    Handle to an allocation region (MemRegion).
    Bits 16-23: Region UID
    Bits 24-31: Region index
    */
    typedef u32 ALLOC_HANDLE;
    static const ALLOC_HANDLE INVALID_HANDLE = 0xFFFFFFFF;

    #define ALLOC_HANDLE_UID(handle) ((handle) >> 8 & 0xFF)
    #define ALLOC_HANDLE_REGION(handle) ((handle) & 0xFF)

    static const int MAX_ALLOC_REGION = 80;

    /*
    Allocatable block inside of a memory region.
    */
    struct MemBlock {
        //Minimum size of allocated data
        static const int MIN_SIZE = 64;
        //Maximum size of allocated data
        static const int MAX_SIZE = 0x7FFFFFF;

        MemBlock* prev; //0x0
        MemBlock* next; //0x4
        MemBlock* aligned; //0x8
        u32 size; //0xC
        u16 region; //0x10
        u8 padding[32 - 0x12]; //0x12

        u8* getStartAddr() {
            return reinterpret_cast<u8*>(this) + sizeof(MemBlock);
        }
        u8* getEndAddr() {
            return reinterpret_cast<u8*>(this) + size;
        }

        u32 getDataSize() const {
            return size - sizeof(MemBlock);            
        }

        static MemBlock* getBlockAddr(void* p) {
            return reinterpret_cast<MemBlock*>(
                static_cast<u8*>(p) - sizeof(MemBlock));
        }
    };

    class MemManager {
    public:
        /*
        Region or section of the memory heap.
        */
        class MemRegion {
            friend class MemManager;

        public:
            MemRegion();
            ~MemRegion();

            static void setRegionMaxSize(u32 maxMEM1, u32 maxMEM2);
            static u32 getMEM1MaxSize();
            static u32 getMEM2MaxSize();

            void* allocate(void* buffer, u32 size, int align);

            MemBlock* reallocate(MemBlock* block);
            MemBlock* coalesceRecursive(MemBlock* block);

            MemBlock* getTailBuffer(u32 size, int align, void** buffer);

            static inline void initialize(){
                setRegionMaxSize(MEM1_MAX_SIZE, MEM2_MAX_SIZE);
            }

        private:
            void* allocateImpl(MemBlock* block, void* buffer, u32 size, int align);

            MemBlock* mHead; //0x0
            MemBlock* mTail; //0x4
            MemBlock* mOldest; //0x8
            MemBlock* mYoungest; //0xC
            void* mStartAddress; //0x10
            void* mEndAddress; //0x14
            u32 mNumAlloc; //0x18
            u32 mSize; //0x1C
            u32 mFreeBytes; //0x20
            ml::FixStr<64> mName; //0x24
            ALLOC_HANDLE mHandle; //0x68
            u8 unk6C;

            static u32 sMaxSizeMEM1;
            static u32 sMaxSizeMEM2;

            static const int MEM1_MAX_SIZE = 0x680000;
            static const int MEM2_MAX_SIZE = 0;
        };

        static void initialize();
        static void finalize();

        static DECOMP_INLINE ALLOC_HANDLE create(void* head, u32 size, const char* name);
        static ALLOC_HANDLE create(ALLOC_HANDLE handle, u32 size, const char* name);
        static ALLOC_HANDLE create_tail(ALLOC_HANDLE handle, u32 size, const char* name);

        static ALLOC_HANDLE getHandleMEM1();
        static void setHandleMEM1(ALLOC_HANDLE handle);

        static ALLOC_HANDLE getHandleMEM2();
        static void setHandleMEM2(ALLOC_HANDLE handle);
        
        static ALLOC_HANDLE getHandleStatic();
        static void setHandleStatic(ALLOC_HANDLE handle);

        static bool erase(ALLOC_HANDLE handle);
        static bool empty(ALLOC_HANDLE handle);

        static void* allocate_head(ALLOC_HANDLE handle, u32 size, int align);
        static void* allocate_tail(ALLOC_HANDLE handle, u32 size, int align);
        static bool deallocate(void* p);
        static bool deallocateImpl(void* p);

        static MemRegion* getRegion(ALLOC_HANDLE handle);
        static u32 getRegionSize(ALLOC_HANDLE handle);
        static u32 getBlockSize(ALLOC_HANDLE handle);
        static MemBlock* getTailBuffer(MemRegion* region, u32 size, int align, void** buffer);
        static MemBlock* getMaxBlock(ALLOC_HANDLE handle);
        static u32 getMaxAllocSize(ALLOC_HANDLE handle);
        static void* getMaxAllocData(ALLOC_HANDLE handle);
        static f32 getPercentAlloc(ALLOC_HANDLE handle);
        static void func_804348A4(ALLOC_HANDLE handle, u8 val);

        static u16 calculateCrc(const void* data, u32 len);
        static void func_80434A4C(bool value);

        static bool isOptimalAlloc();
        static void setOptimalAlloc(bool enable);

        static void* allocate(u32 size, ALLOC_HANDLE handle);
        static void* allocate_array(u32 size, ALLOC_HANDLE handle);

        static void* allocate_ex(u32 size, ALLOC_HANDLE handle, int align);
        static void* allocate_array_ex(u32 size, ALLOC_HANDLE handle, int align);

        /* Commented out log function, which would have printed debug messages. It seems like
        monolithsoft had log functions for many classes in addition to this one. */
        //static void log(int something);

    private:
        static const char* scRegionNameMEM1;
        static const char* scRegionNameMEM2;

        static ALLOC_HANDLE sHandleMEM1;
        static ALLOC_HANDLE sHandleMEM2;

        static RawArray<MemRegion, MAX_ALLOC_REGION> sRegionArray;
        static u32 sRegionUniqueId;

        static bool lbl_80667E54;
        static bool lbl_80665E38;
        static bool lbl_80665E39;        
        static bool sIsOptimalAlloc;
    };

}

/*
Allocates object memory from the specified region.
*/
inline void* operator new(size_t size, mtl::ALLOC_HANDLE handle) {
    return mtl::MemManager::allocate(size, handle);
}

/*
Allocates aligned object memory from the specified region.
Specify negative alignment to perform a tail allocation.
*/
inline void* operator new(size_t size, int align, mtl::ALLOC_HANDLE handle) {
    return mtl::MemManager::allocate_ex(size, handle, align);
}

/*
Allocates array memory from the specified region.
*/
inline void* operator new[](size_t size, mtl::ALLOC_HANDLE handle) {
    return mtl::MemManager::allocate_array(size, handle);
}

/*
Allocates aligned array memory from the specified region.
Specify negative alignment to perform a tail allocation.
*/
inline void* operator new[](size_t size, mtl::ALLOC_HANDLE handle, int align) {
    return mtl::MemManager::allocate_array_ex(size, handle, align);
}

//Utility macros

#define DELETE_OBJ(p)                   \
    {                                   \
    if (p != nullptr){                  \
        mtl::MemManager::deallocate(p); \
        p = nullptr;                    \
    }                                   \
    }                                   \


#define DELETE_ARRAY(p)                   \
    {                                     \
    if (p != nullptr){                    \
        delete[] p;                       \
        p = nullptr;                      \
    }                                     \
    }                                     \
/* end "monolib/util/MemManager.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 11 "monolib/util/PtrSingleton.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/PtrSingleton.hpp" line 2 "types.h" */
/* end "types.h" */

namespace mtl{

template <typename T>
class PtrSingleton{
public:
    PtrSingleton(){
        spInstance = static_cast<T*>(this);
    }

    ~PtrSingleton(){
        spInstance = nullptr;
    }

    static T* getInstance(){
        return spInstance;
    }

protected:
    static T* spInstance;
};

template <typename T> T* PtrSingleton<T>::spInstance;

}
/* end "monolib/util/PtrSingleton.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 12 "monolib/util/RawArray.hpp" */
/* end "monolib/util/RawArray.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 13 "monolib/util/reslist.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/reslist.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/util/reslist.hpp" line 3 "monolib/util/MemManager.hpp" */
/* end "monolib/util/MemManager.hpp" */
/* "libs/monolib/include/monolib/util/reslist.hpp" line 4 "monolib/work/CWorkThreadSystem.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkThreadSystem.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/CWorkThreadSystem.hpp" line 3 "monolib/monolib_types.hpp" */
#pragma once

//List of forward declarations for commonly used classes.

//Core
class CView;
class CException;

//Device
class CFileHandle;
class CDeviceFileJob;

//Math
namespace ml {
    struct CPnt16;
    struct CRect16;
    struct CVec3;
    struct CVec4;
    struct CCol3;
    struct CCol4;
    struct CMat34;
    struct CFrustum;
} //namespace ml

//Scene
class CScn;
class CScnNw4r;
class IScnRender;
class ICulling;

//Util
class CChildListNode;

//Work
class CEventFile;
class CProcess;
class CProc;
class CWorkThread;
/* end "monolib/monolib_types.hpp" */
/* "libs/monolib/include/monolib/work/CWorkThreadSystem.hpp" line 4 "monolib/util.hpp" */
/* end "monolib/util.hpp" */

/*
Handle to a work memory region
*/
typedef u32 WORK_ID;
static const WORK_ID INVALID_WORK_ID = 0xFFFFFFFF;

class CWorkThreadSystem{
    friend class CWorkThread;

public:
    static void initialize();
    static void destroy();

    static WORK_ID allocWID(CWorkThread* thread);
    static void freeWID(WORK_ID wid){
        sAllocFlags[wid / 32] &= ~(1 << wid % 32);
        sWorkThreads[wid] = nullptr;
    }

    static mtl::ALLOC_HANDLE getWorkMem();

private:
    static const u32 REGION_SIZE = 0x70000 - sizeof(mtl::MemBlock);

    //Highest allowed work ID
    static const WORK_ID MAX_WORK_ID = 2048;
    //One registration bit flag per work ID
    static const u32 ALLOC_FLAGS_COUNT = MAX_WORK_ID / (sizeof(u32) * 8);

    static const char* scRegionName;
    static BOOL sMemAvailable;

    //Handle for all work memory allocations
    static mtl::ALLOC_HANDLE sAllocHandle;

    //Work thread registration flags, by ID
    static u32* sAllocFlags;

    //Registered work threads, by ID
    static CWorkThread** sWorkThreads;
};
/* end "monolib/work/CWorkThreadSystem.hpp" */
/* "libs/monolib/include/monolib/util/reslist.hpp" line 5 "algorithm" */
#ifndef MSL_CPP_ALGORITHM_H
#define MSL_CPP_ALGORITHM_H
/* "libs/PowerPC_EABI_Support/include/stl/algorithm" line 2 "iterator" */
#ifndef MSL_CPP_ITERATOR_H
#define MSL_CPP_ITERATOR_H

namespace std {

    struct random_access_iterator_tag {};

} // namespace std

#endif
/* end "iterator" */

namespace std {

template <typename T> inline const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <typename T> inline const T& min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

template <typename TPtr, typename T>
inline TPtr find(TPtr first, TPtr last, const T& value) {
    while (first != last && *first != value) {
        ++first;
    }

    return first;
}

template <typename TPtr> inline long distance(TPtr first, TPtr last) {
    random_access_iterator_tag tag;
    return __distance(first, last, tag);
}

template <typename TPtr>
inline long __distance(TPtr first, TPtr last, random_access_iterator_tag tag) {
    long dist = reinterpret_cast<long>(last) - reinterpret_cast<long>(first);
    return dist / static_cast<long>(sizeof(TPtr));
}

template <typename T> inline T& move(T& x) { return x; }

template <typename T> inline void swap(T& a, T& b) {
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
}

} // namespace std

#endif
/* end "algorithm" */

/* TODO: This breaks ctors/dtors, but it looks alot nicer than the method below. Maybe there's a way to get
this to work */
template <typename T>
struct _reslist_node_item {
    _reslist_node_item(){}
    _reslist_node_item(const T& value) try :
    item(value) {} catch(...){}
    T item;
};

template <typename T>
struct _reslist_node{
    void setItem(const T& value){
        //This doesn't work
        //new (&mItem) _reslist_node_item<T>(value);
        //Possibly fake?
        T* ptr = &mItem;
        if(ptr != nullptr) {
            try{
                *ptr = value;
            }catch(...){
                throw;
            }
        }
    }

    _reslist_node<T>* mNext; //0x0
    _reslist_node<T>* mPrev; //0x4
    T mItem; //0x8

};

template <typename T>
class _reslist_base{
public:
    _reslist_base(){
        mList = nullptr;
        mCapacity = 0;
        unk1C = false;
        mStartNodePtr = &mStartNode;
        mStartNodePtr->mNext = &mStartNode;
        mStartNodePtr->mPrev = mStartNode.mNext;
    }

    virtual ~_reslist_base(){
        clearList();

        if (unk1C == false) DELETE_ARRAY(mList);
    }

    void func_8049CB6C(T* item){

    }

    void func_8049CB70(_reslist_node<T>* r4){
        r4->mNext = nullptr;
    }

    int findFirstEmptySlotIndex(){
        int i = 0;
        
        //Go through the list until we find an empty slot
        while(i < mCapacity){
            if(mList[i].mNext == nullptr) break;

            //BUG: There's no handling for if no empty slot is found. As is,
            //the last element index will get returned if that happens.
            //if(i == mCapacity - 1) return -1;

            i++;
        }

        return i;
    }

    //func_8049CAF4
    void clearList(){
        _reslist_node<T>* r5 = mStartNodePtr->mNext;
        
        while (r5 != mStartNodePtr) {
            _reslist_node<T>* r4 = r5;
            r5 = r5->mNext;
            func_8049CB6C(&r4->mItem);
            func_8049CB70(r4);
        }
    
        mStartNodePtr->mNext = mStartNodePtr;
        mStartNodePtr->mPrev = mStartNodePtr;
    }

    //0x0: vtable
    _reslist_node<T>* mStartNodePtr; //0x4
    _reslist_node<T> mStartNode; //0x8
    _reslist_node<T>* mList; //0x14
    int mCapacity; //0x18
    bool unk1C; //0x1C
};

template <typename T>
class reslist;

//Unofficial name
template <typename T, typename Ref, typename Ptr>
class _reslist_iterator {
    friend class reslist<T>;
public:
    _reslist_iterator() : mNode(nullptr){}
    explicit _reslist_iterator(_reslist_node<T>* node) : mNode(node){}

    _reslist_iterator& operator++(){
        mNode = mNode->mNext;
        return *this;
    }

    _reslist_iterator operator++(int){

        _reslist_iterator temp = *this;
        mNode = mNode->mNext;
        return temp;
    }

    _reslist_iterator& operator--(){
        mNode = mNode->mPrev;
        return *this;
    }

    _reslist_iterator operator--(int){
        _reslist_iterator temp = *this;
        mNode = mNode->mPrev;
        return temp;
    }

    Ref operator*() const {
        return mNode->mItem;
    }

    Ptr operator->() const {
        return &operator*();
    }

    bool operator==(_reslist_iterator const& rhs) const {
        return mNode == rhs.mNode;
    }

    bool operator!=(_reslist_iterator const& rhs) const {
        return mNode != rhs.mNode;
    }

private:
    _reslist_node<T>* mNode; //0x0
};

//size: 0x20
template <typename T>
class reslist : public _reslist_base<T> {
public:
    typedef _reslist_iterator<T, T&, T*> iterator;

    reslist() : _reslist_base<T>() {
    }
    virtual ~reslist(){
    }

    iterator begin() const {
        return iterator(mStartNodePtr->mNext);
    }
    iterator end() const {
        return iterator(mStartNodePtr);
    }

    T& front() {
        return *begin();
    }
    T& back() {
        return *begin();
    }

    const T& front() const {
        return *begin();
    }
    const T& back() const {
        return *begin();
    }

    u32 size() const {
        _reslist_node<T>* curNode;
        _reslist_node<T>* endNode;
        
        u32 length = 0;
        endNode = mStartNodePtr;
        curNode = mStartNodePtr->mNext;

        while(curNode != endNode){
            length++;
            curNode = curNode->mNext;
        }
        
        return length;
    }

    bool empty() const {
        return mStartNodePtr->mNext == mStartNodePtr;
    }

    inline void reserve(mtl::ALLOC_HANDLE handle, int capacity) {
        mList = new (handle) _reslist_node<T>[capacity];

        for(int i = 0; i < capacity; i++){
            mList[i].mNext = nullptr;
        }

        mCapacity = capacity;
    }

    iterator find(const T& item){
        return std::find(begin(), end(), item);
    }

    void push_front(const T& item){
        _reslist_node<T>* startNode = mStartNodePtr->mNext;
        int i = findFirstEmptySlotIndex();

        _reslist_node<T>* temp = &mList[i];
        temp->setItem(item);
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }

    void push_back(const T& item){
        _reslist_node<T>* startNode = mStartNodePtr;
        int i = findFirstEmptySlotIndex();

        _reslist_node<T>* temp = &mList[i];

        temp->setItem(item);
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }

    void remove(const T& item){
        _reslist_node<T>* curr;
        _reslist_node<T>* next;
        _reslist_node<T>* head;

        head = mStartNodePtr;
        curr = head->mNext;
        
        //Walk through the list
        while(curr != head){
            //Save next node in case we invalidate the curr iterator
            next = curr->mNext;

            //If we find an entry containing the item, remove the entry
            if(curr->mItem == item){
                _reslist_node<T>* prev = curr->mPrev;
                prev->mNext = next;
                next->mPrev = prev;
                curr->mNext = nullptr;
            }

            curr = next;
        }
    }

    void pop_front(){
        _reslist_node<T>* prevNode;
        _reslist_node<T>* nextNode;
        _reslist_node<T>* frontNode;

        frontNode = mStartNodePtr->mNext;
        nextNode = frontNode->mNext;
        prevNode = frontNode->mPrev;
        prevNode->mNext = nextNode;
        nextNode->mPrev = prevNode;
        frontNode->mNext = nullptr;
    }

    void erase(iterator& it){
        _reslist_node<T>* r5;
        _reslist_node<T>* r4;
        _reslist_node<T>* node = it.mNode;
        
        r4 = node->mPrev;
        r5 = node->mNext;
        r4->mNext = r5;
        r5->mPrev = r4;
        node->mNext = nullptr;
    }

    void clear(){
        clearList();
    }

    void destroyList(){
        clearList();
        if (unk1C == false && mList != nullptr) {
            delete[](this->mList);
            mList = nullptr;
        }
        mCapacity = 0;
    }
};
/* end "monolib/util/reslist.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 14 "monolib/util/resvector.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/resvector.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/util/resvector.hpp" line 3 "monolib/util/MemManager.hpp" */
/* end "monolib/util/MemManager.hpp" */

template <typename T>
class resvector {
public:
    resvector() : mArray(nullptr), mCount(0) {
    }
    resvector(mtl::ALLOC_HANDLE handle) : mArray(nullptr), mCount(0), mHandle(handle) {
    }
    ~resvector(){   
        destroy();
    }
    
    T* begin(){
        return &mArray[0];
    }

    T* end(){
        return &mArray[mCount];
    }

    const T* begin() const{
        return begin();
    }

    const T* end() const{
        return end();
    }

    u32 size() const {
        return mCount;
    }

    T& operator[](int index){
        return mArray[index];
    }

    const T& operator[](int index) const {
        return mArray[index];
    }

    void reserve(mtl::ALLOC_HANDLE handle, u32 size){
        mHandle = handle;
        mArray = static_cast<T*>(mtl::MemManager::allocate_head(handle, size * sizeof(T), 4));
        mCount = 0;
        mSize = size;
    }

    void push_back(T item){
        //NOTE: no bounds check
        mArray[mCount++] = item;
    }

    void destroy(){
        mCount = 0;
        if(mHandle != mtl::INVALID_HANDLE){
            DELETE_OBJ(mArray);
        }

        mArray = nullptr;
        mSize = 0;
        mHandle = mtl::INVALID_HANDLE;
    }

    void clearList(){
        T* ptr = mArray;
        while(ptr != &mArray[mCount]){
            DELETE_OBJ(*ptr);
            ptr++;
        }
    }

    void resetCount(){
        mCount = 0;
    }

private:
    T* mArray; //0x0
    u32 mCount; //0x4
    u32 mSize; //0x8
    u32 mHandle; //0xC
};
/* end "monolib/util/resvector.hpp" */
/* "libs/monolib/include/monolib/util.hpp" line 15 "monolib/util/TPLUtils.hpp" */
#pragma once

/* "libs/monolib/include/monolib/util/TPLUtils.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/util/TPLUtils.hpp" line 3 "revolution/TPL.h" */
/**
 * References: YAGCD, BrawlBox
 */

#ifndef RVL_SDK_PUBLIC_TPL_H
#define RVL_SDK_PUBLIC_TPL_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/TPL.h" line 10 "revolution/TPL/TPL.h" */
#ifndef RVL_SDK_TPL_H
#define RVL_SDK_TPL_H
/* "libs/RVL_SDK/include/revolution/TPL/TPL.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/TPL/TPL.h" line 4 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TPLHeader {
    u16 height;            // at 0x0
    u16 width;             // at 0x2
    u32 format;            // at 0x4
    char* data;            // at 0x8
    GXTexWrapMode wrapS;   // at 0xC
    GXTexWrapMode wrapT;   // at 0x10
    GXTexFilter minFilter; // at 0x14
    GXTexFilter magFilter; // at 0x18
    f32 LODBias;           // at 0x1C
    u8 edgeLODEnable;      // at 0x20
    u8 minLOD;             // at 0x21
    u8 maxLOD;             // at 0x22
    u8 unpacked;           // at 0x23
} TPLHeader;

typedef struct TPLClutHeader {
    u16 numEntries;   // at 0x0
    u8 unpacked;      // at 0x1
    u8 pad8;          // at 0x2
    GXTlutFmt format; // at 0x4
    char* data;       // at 0x8
} TPLClutHeader;

typedef struct TPLDescriptor {
    TPLHeader* textureHeader;  // at 0x0
    TPLClutHeader* CLUTHeader; // at 0x4
} TPLDescriptor;

typedef struct TPLPalette {
    u32 versionNumber;              // at 0x0
    u32 numDescriptors;             // at 0x4
    TPLDescriptor* descriptorArray; // at 0x8
} TPLPalette;

void TPLBind(TPLPalette* pal);
TPLDescriptor* TPLGet(TPLPalette* pal, u32 id);
void TPLGetGXTexObjFromPalette(TPLPalette* pal, GXTexObj* to, u32 id);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/TPL/TPL.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/TPL.h" */

namespace ml{

    /* Wrapper TPL file struct to allow for easy access to the image header data in simple single image
    TPL files without needing to resort to raw pointer/macro abuse. Note that this struct only is valid for
    TPL files that have a single image in them and don't use a palette based format. */
    struct CTPLData{
        TPLPalette fileHeader; //0x0
        TPLDescriptor descriptor; //0xC
        TPLHeader imageHeader; //0x14
    };

}
/* end "monolib/util/TPLUtils.hpp" */
/* end "monolib/util.hpp" */

enum CBM {
    CBM_0,
    CBM_1,
    CBM_2,
    CBM_3,
    CBM_4,
    CBM_5
};

struct CFileHandle {
    int unk0;
    void* mData; //0x4
    u8 unk8[0x10 - 0x8];
    int unk10;
    u32 unk14;
    u8 unk18[0x3C - 0x18];
    u32 mLength; //0x3C
    u8 unk40[0x5C - 0x40];
    ml::FixStr<32> mName; //0x5C
    u8 unk80[0x160 - 0x80];
    u32 unk160;

    void call(CBM cbm);
    bool checkExistRsrc(CBM cbm);
    UNKTYPE* getRsrc();

    inline void* getData(){
        void* r31 = mData;
        mData = nullptr;
        return r31;
    }

    inline bool unkInline2() const {
        return unk10 != 0 && unk10 == mLength;
    }

    inline u32 getLength() const {
        return mLength;
    }

};
/* end "monolib/device/CFileHandle.hpp" */
/* "src/kyoshin/CUICfManager.hpp" line 4 "monolib/work.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work.hpp" line 2 "monolib/work/CEventFile.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CEventFile.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/CEventFile.hpp" line 3 "monolib/monolib_types.hpp" */
/* end "monolib/monolib_types.hpp" */

class CEventFile {
public:
    BOOL unk0;
    CFileHandle* mFileHandle; //0x4

    void* getFileDataPtr();
};
/* end "monolib/work/CEventFile.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 3 "monolib/work/CMsgParam.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CMsgParam.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CMsgParam.hpp" line 4 "monolib/work/CWorkThreadSystem.hpp" */
/* end "monolib/work/CWorkThreadSystem.hpp" */

//Message param entry format:
//0x0: message id
//0x4: work ID
//0x8: address
//0xC: code address
//0x10: value
//0x14: address
//0x18: address
//0x1C: address
//0x20: address (pointer to string?)
struct CMsgParamEntry{
    u32 command; //0x0
    WORK_ID wid; //0x4
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u16 unk20;
    u8 unk22;
    u8 unk23;
};

template <int N>
class CMsgParam{
public:
    CMsgParam(u32 r4){
        mCapacity = N;
        mArrayPtr = mEntries;
        mSize = 0;
        mFront = 0;
        field6 = 0;
        field7 = r4;
    }

    virtual ~CMsgParam(){
        clear();
    }

    void clear(){
        mSize = 0;
        mFront = 0;
    }

    bool empty() const{
        return mSize == 0;
    }

    u32 size() const{
        return mSize;
    }

    const CMsgParamEntry& front() const{
        return mArrayPtr[mFront % mCapacity];
    }

    void enqueue(u32 msg){
        volatile CMsgParamEntry entry;
        u32 wid = entry.wid;
        u32 value8 = entry.unk8;
        u32 valueC = entry.unkC;
        u32 value10 = entry.unk10;
        u32 value14 = entry.unk14;
        u32 value18 = entry.unk18;
        u32 value1C = entry.unk1C;
        u16 value20 = entry.unk20;
        u8 value22 = entry.unk22;
        int index = (int)(mFront + mSize) % (int)mCapacity;
        u8* dst = reinterpret_cast<u8*>(mArrayPtr);

        *reinterpret_cast<u32*>(dst += index * sizeof(CMsgParamEntry)) = msg;
        *reinterpret_cast<u32*>(dst + 0x4) = wid;
        *reinterpret_cast<u32*>(dst + 0x8) = value8;
        *reinterpret_cast<u32*>(dst + 0xC) = valueC;
        *reinterpret_cast<u32*>(dst + 0x10) = value10;
        *reinterpret_cast<u32*>(dst + 0x14) = value14;
        *reinterpret_cast<u32*>(dst + 0x18) = value18;
        *reinterpret_cast<u32*>(dst + 0x1C) = value1C;
        *reinterpret_cast<u16*>(dst + 0x20) = value20;
        *(dst + 0x22) = value22;
        *(dst + 0x23) = 0;

        mSize++;
        field6 = mSize - 1;
    }

    CMsgParamEntry& last(){
        return mArrayPtr[(mFront + field6) % mCapacity];
    }

    void pop(){
        mSize--;
        mFront = (mFront + 1) % mCapacity;
    }

    int find(u32 msg) const{
        for(int i = 0; i < mSize; i++){
            if(mArrayPtr[(mFront + i) % mCapacity].command == msg){
                return i;
            }
        }

        return -1;
    }

private:
    //0x0: vtable
    CMsgParamEntry mEntries[N]; //0x4
    CMsgParamEntry* mArrayPtr; //N*0x24 + 0x4
    u32 mFront; //N*0x24 + 0x8
    u32 mSize; //N*0x24 + 0xC
    u32 mCapacity; //N*0x24 + 0x10
    u32 field6; //N*0x24 + 0x14
    u32 field7; //N*0x24 + 0x18
};
/* end "monolib/work/CMsgParam.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 4 "monolib/work/CTTask.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CTTask.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/CTTask.hpp" line 3 "monolib/work/CProcess.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CProcess.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/CProcess.hpp" line 3 "monolib/util.hpp" */
/* end "monolib/util.hpp" */

/*
Instance of a game task.

Processes can contain child sub-processes,
and somewhat maintain a priority system.

Each process can implement specific behavior
for its initialization/termination,
and for the update ("Move") and render ("Draw") game events.
*/
class CProcess : public CChildListNode{
    friend class CProcessMan;

public:
    CProcess();
    virtual ~CProcess();

    virtual void Init() = 0;
    virtual void Term() = 0;

    virtual void Move() = 0;
    virtual void Draw() = 0;
    // Out-of-line: inline `{}` emits Tail__8CProcessFv into every derived TU (CfPadTask +.text).
    virtual void Tail();

    void Regist(CProcess* parent, bool insertTop);
    void Remove();

    void SetRemove(){
        mIsRemove = true;    
    }

private:
    //0x0-10: CDoubleListNode
    //0x10: vtable
    //0x14-38: CChildListNode
    bool mIsRegist; //0x38
    bool mIsRemove; //0x39
    bool mIsDisableMove; //0x3A
    bool mIsDisableDraw; //0x3B
};

/*
Process manager.

Responsible for dispatching all process events every tick,
and for maintaining the lists of both root-level processes and released processes.
*/
class CProcessMan {
public:
    static void Reset();
    static void Delete();

    static void Init();
    static void Term();
    
    static void Move();
    static void Draw();
    static void Tail();

    static TChildListHeader<CProcess>& GetFreeProcessList() {
        return sFreeProcessList;
    }
    static TChildListHeader<CProcess>& GetRootProcessList() {
        return sRootProcessList;
    }

private:
    static void MoveImpl(CProcess* proc);
    static void DrawImpl(CProcess* proc);
    static void TailImpl(CProcess* proc);

    static bool Remove(CProcess* proc);

    static void DeleteList(TChildListHeader<CProcess>& list);
    static void DeleteImpl(CProcess* proc);

    static bool sIsInitialized;
    static TChildListHeader<CProcess> sFreeProcessList;
    static TChildListHeader<CProcess> sRootProcessList;
};
/* end "monolib/work/CProcess.hpp" */

/*
Generic task object.

Provides a way to implement Move/Draw behavior without needing to work with the
CProcess api.

Derived classes must inherit using CRTP to allow binding the move/draw functions.
*/
template <typename TDerived>
class CTTask : public CProcess {
public:
    typedef void (TDerived::*MoveFunc)();
    typedef void (TDerived::*DrawFunc)();

public:
    CTTask() : mMoveFunc(nullptr), mDrawFunc(nullptr) {}

    virtual void Move() {
        if (mMoveFunc) {
            (static_cast<TDerived*>(this)->*mMoveFunc)();
        }
    }
    virtual void Draw() {
        if (mDrawFunc) {
            (static_cast<TDerived*>(this)->*mDrawFunc)();
        }
    }

protected:
    //0x0-10: CDoubleListNode
    //0x10: vtable
    //0x14-3C: CProcess
    MoveFunc mMoveFunc; //0x3C
    DrawFunc mDrawFunc; //0x48
}; // size: 0x54
/* end "monolib/work/CTTask.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 5 "monolib/work/CWorkControl.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkControl.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CWorkControl.hpp" line 4 "monolib/work/CWorkThread.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkThread.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CWorkThread.hpp" line 4 "monolib/work/CMsgParam.hpp" */
/* end "monolib/work/CMsgParam.hpp" */
/* "libs/monolib/include/monolib/work/CWorkThread.hpp" line 5 "monolib/work/IWorkEvent.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/IWorkEvent.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/IWorkEvent.hpp" line 3 "monolib/monolib_types.hpp" */
/* end "monolib/monolib_types.hpp" */

/* Interface for work events, which provides a set of 32 event handler functions that
get triggered when a certain event happens (such as when loading a file for OnFileEvent).
Deriving classes can override any of these functions to run their own code when the
corresponding event happens.

Of the 32 available event slots, however, only events 1-5 are ever overriden, with the rest
being empty slots that were left in for some dumb reason (tysm monolithsoft <3). Additionally,
out of the 5 overriden events, only OnFileEvent and OnPauseTrigger seem to be used,
with no apparent calls to the other 3 (possibly debug only).

In XC3D, all instances of the unused event functions (including events 1, 3, and 4) are absent,
with the entries for each instead just being 0 in the vtable. This points to the extra 3 overridden
events being unused as well. */
class IWorkEvent {
public:
    virtual ~IWorkEvent(){}
    virtual bool WorkEvent1(UNKTYPE* r4, const char* r5){ return false; }
    virtual bool OnFileEvent(CEventFile* pEventFile){ return false; }
    virtual bool WorkEvent3(UNKTYPE* r4){ return false; }
    virtual bool WorkEvent4(){ return false; }
    virtual void OnPauseTrigger(bool paused){}
    //Completely unused, but still left in...
    virtual bool WorkEvent6(){ return false; }
    virtual bool WorkEvent7(){ return false; }
    virtual bool WorkEvent8(){ return false; }
    virtual bool WorkEvent9(){ return false; }
    virtual bool WorkEvent10(){ return false; }
    virtual bool WorkEvent11(){ return false; }
    virtual bool WorkEvent12(){ return false; }
    virtual bool WorkEvent13(){ return false; }
    virtual bool WorkEvent14(){ return false; }
    virtual bool WorkEvent15(){ return false; }
    virtual bool WorkEvent16(){ return false; }
    virtual bool WorkEvent17(){ return false; }
    virtual bool WorkEvent18(){ return false; }
    virtual bool WorkEvent19(){ return false; }
    virtual bool WorkEvent20(){ return false; }
    virtual bool WorkEvent21(){ return false; }
    virtual bool WorkEvent22(){ return false; }
    virtual bool WorkEvent23(){ return false; }
    virtual bool WorkEvent24(){ return false; }
    virtual bool WorkEvent25(){ return false; }
    virtual bool WorkEvent26(){ return false; }
    virtual bool WorkEvent27(){ return false; }
    virtual bool WorkEvent28(){ return false; }
    virtual bool WorkEvent29(){ return false; }
    virtual bool WorkEvent30(){ return false; }
    virtual void WorkEvent31(){}
};
/* end "monolib/work/IWorkEvent.hpp" */
/* "libs/monolib/include/monolib/work/CWorkThread.hpp" line 6 "monolib/work/CWorkThreadSystem.hpp" */
/* end "monolib/work/CWorkThreadSystem.hpp" */
/* "libs/monolib/include/monolib/work/CWorkThread.hpp" line 7 "monolib/work/CWorkUtil.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkUtil.hpp" line 2 "monolib/monolib_types.hpp" */
/* end "monolib/monolib_types.hpp" */
/* "libs/monolib/include/monolib/work/CWorkUtil.hpp" line 3 "monolib/work/UnkStruct_80438AF0.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/UnkStruct_80438AF0.hpp" line 2 "types.h" */
/* end "types.h" */

struct UnkStruct_80438AF0_3 {};

struct UnkStruct_80438AF0_2 {
    UnkStruct_80438AF0_3* unk0;
};

struct UnkStruct_80438AF0 {
    char unk0[0x4];
    UnkStruct_80438AF0_2* unk4;
};
/* end "monolib/work/UnkStruct_80438AF0.hpp" */
/* "libs/monolib/include/monolib/work/CWorkUtil.hpp" line 4 "monolib/work/CWorkThreadSystem.hpp" */
/* end "monolib/work/CWorkThreadSystem.hpp" */

class CWorkUtil{
public:
    static UnkStruct_80438AF0_3* func_80438AF0(UnkStruct_80438AF0* arg0);
    static CWorkThread* getWorkThread(WORK_ID wid);
    static void dispTree(const CWorkThread* pThread, int indent = 0);
    static void entryWork(CWorkThread* pChild, CWorkThread* pParent, bool prepend);
};

//Possibly member of CWorkThread?
extern bool hasChild(CWorkThread* pThread);
/* end "monolib/work/CWorkUtil.hpp" */
/* "libs/monolib/include/monolib/work/CWorkThread.hpp" line 8 "monolib/util.hpp" */
/* end "monolib/util.hpp" */

//size: 0x1C4
class CWorkThread : public IWorkEvent{
public:
    /* Enum used to keep track of the type of this work thread, which is useful when
     casting from a generic instance of CWorkThread. Most classes inheriting from
     CWorkThread have a unique value reserved, but not all do. If a custom value
     isn't set, derived classes will default to the value for CWorkThread.
     
     Special type ranges:
     23-47: CProc inheriting classes
     48-52: CView inheriting classes
    */
    enum ThreadType{
        //Work classes
        THREAD_CWORKTHREAD = 0,
        THREAD_CWORKCONTROL = 1,
        THREAD_CWORKSYSTEM = 2,
        THREAD_03 = 3,
        THREAD_04 = 4,
        THREAD_05 = 5,
        THREAD_CWORKSYSTEMCACHE = 6,
        THREAD_CWORKSYSTEMPACK = 7,
        THREAD_08 = 8,
        THREAD_09 = 9,
        THREAD_CDEVICE = 10,
        //CLib classes
        THREAD_CLIB = 11,
        THREAD_CLIBG3D = 12,
        THREAD_CLIBHBM = 13,
        THREAD_CLIBLAYOUT = 14,
        THREAD_CLIBCRI = 15,
        THREAD_CLIBCRIMOVIEPLAY = 16,
        THREAD_CLIBCRISTREAMINGPLAY = 17,
        THREAD_CLIBVM = 18,
        THREAD_CLIBSTATICDATA = 19,
        THREAD_20 = 20,
        //Root classes
        THREAD_CPROCROOT = 21,
        THREAD_CVIEWROOT = 22,
        //CProc inheriting classes
        THREAD_CPROC = 23,
        THREAD_CDESKTOP = 24,
        THREAD_CEXCEPTION = 25,
        THREAD_CLIBHBMCONTROL = 45,
        THREAD_CPROC_MAX = 47,
        //CView inheriting classes
        THREAD_CVIEW = 48,
        THREAD_CVIEW_MAX = 53,
        THREAD_CRSRCDATA = 56,
        THREAD_CSCRIPTCODE = 59,
        //Misc device classes
        THREAD_CDEVICEFONTLAYER = 63,
        THREAD_CDEVICEFONTLOADER = 64,
        THREAD_CDEVICEFILEJOB = 65,
        THREAD_CDEVICEFILEJOBREADDVD = 68
    };

    enum EVT{
        EVT_NONE,
        EVT_1,
        EVT_EXCEPTION,
        EVT_3,
        EVT_4,
        EVT_PAUSE,
        EVT_UNPAUSE,
        EVT_7,
        EVT_8,
        EVT_9,
        EVT_APPEXCEPTION_ON,
        EVT_APPEXCEPTION_OFF,
    };

    enum ThreadFlags{
        THREAD_FLAG_NO_EVENT = 1 << 0,
        THREAD_FLAG_EVT1 = 1 << 1,
        THREAD_FLAG_2 = 1 << 2,
        THREAD_FLAG_3 = 1 << 3,
        THREAD_FLAG_EXCEPTION = 1 << 4,
        THREAD_FLAG_EVT3 = 1 << 5,
        THREAD_FLAG_EVT4 = 1 << 6,
        THREAD_FLAG_PAUSE = 1 << 7,
        THREAD_FLAG_EVT9 = 1 << 8,
        THREAD_FLAG_EVT7 = 1 << 9,
        THREAD_FLAG_APPEXCEPTION = 1 << 10,
    };

    enum ThreadState{
        THREAD_STATE_NONE,
        THREAD_STATE_INIT,
        THREAD_STATE_LOGIN,
        THREAD_STATE_RUN,
        THREAD_STATE_LOGOUT,
        THREAD_STATE_SHUTDOWN,
    };

public:
    CWorkThread(const char* pName, CWorkThread* pParent, int capacity);
    virtual ~CWorkThread();
    virtual void wkUpdate();                           //0x88
    virtual void wkRender(){}                          //0x8C
    virtual void wkRenderAfter(){}                     //0x90
    virtual bool wkStandbyLogin();                     //0x94
    virtual bool wkStandbyLogout();                    //0x98
    virtual bool wkStandbyExceptionRetry(WORK_ID wid){ //0x9C
        return true;
    }

    void wkReplaceHasChild(int capacity);
    void wkEntryChild(CWorkThread* pChild, bool prepend);
    void wkRemoveChild(CWorkThread* pChild);

    void wkSetEvent(EVT evt);
    void wkSetEventChild(EVT evt);

    bool wkCheckTimeout(u32 arg0, bool arg1, const char* pMessage);
    bool wkIsCurrent() const;
    void func_804385CC(u32);

    void wkTimeoutInit();
    void wkStandby();

    static CWorkThread* getWorkThread(WORK_ID wid);
    CWorkThread* getWorkThread(const char* name);

    CWorkThread* wkGetChild(){
        return mChildren.front();
    }
    CWorkThread* wkGetChild() const {
        return mChildren.front();
    }

    bool isRunning() const {
        return !isException() && (mState == THREAD_STATE_LOGIN || mState == THREAD_STATE_RUN);
    }

    bool isException() const {
        return checkFlag(THREAD_FLAG_EXCEPTION) ? true : mMsgQueue.find(EVT_EXCEPTION) >= 0;
    }

    bool isEvent3() const {
        return checkFlag(THREAD_FLAG_EVT3) ? true : mMsgQueue.find(EVT_3) >= 0;
    }
    
    bool isNoEvent() const {
        return checkFlag(THREAD_FLAG_NO_EVENT);
    }

    bool isPaused() const {
        return checkFlag(THREAD_FLAG_PAUSE) && checkFlag(THREAD_FLAG_EVT4);
    }

    bool isEvent7() const {
        return checkFlag(THREAD_FLAG_EVT7) && !checkFlag(THREAD_FLAG_EVT9);
    }

    bool isAppException() const {
        return checkFlag(THREAD_FLAG_APPEXCEPTION);
    }

    bool checkFlag(ThreadFlags flag) const {
        return mFlags & flag;
    }

    bool hasSingleChild() const {
        return mChildren.size() == 1;
    }

    //0x0: vtable
    ml::FixStr<64> mName;            //0x4
    ThreadState mState;              //0x48
    WORK_ID mWorkID;                 //0x4C
    ThreadType mType;                //0x50
    mtl::ALLOC_HANDLE mAllocHandle;  //0x54
    CWorkThread* mParent;            //0x58
    reslist<CWorkThread*> mChildren; //0x5C
    u32 mFlags;                      //0x7C
    CMsgParam<8> mMsgQueue;          //0x80
    u32 unk1BC;
    WORK_ID mExceptionWorkID; //0x1C0

private:
    bool wkStandbyInit();
    bool wkStandbyRun();
    bool wkStandbyShutdown();
};

//Utility macros b/c i don't wanna type this out every time
//TODO: improve these macros

#define DECL_WORKTHREAD_CREATE(class)                                            \
    static class* create(const char* pName, CWorkThread* pParent){               \
        WORK_ID id = CWorkThreadSystem::getWorkMem();                            \
        class* threadClass = new (id) class(pName, pParent);                     \
        CWorkUtil::entryWork(threadClass, pParent, false);                       \
        return threadClass;                                                      \
    }                                                                            \

#define CREATE_WORKTHREAD(class, parent) class::create(#class, parent);
/* end "monolib/work/CWorkThread.hpp" */
/* "libs/monolib/include/monolib/work/CWorkControl.hpp" line 5 "monolib/work/CWorkUtil.hpp" */
/* end "monolib/work/CWorkUtil.hpp" */

class CWorkControl : public CWorkThread{
public:
    static CWorkControl* getInstance();
    static CWorkControl* create(CWorkThread* pParent);
    static CWorkControl* create(const char* pName, CWorkThread* pParent);

    static bool setFlowSetup();
    static void pause(bool paused);
    static bool hasFlow();

    virtual bool wkStandbyLogin();  //0x94
    virtual bool wkStandbyLogout(); //0x98

private:
    static const int MAX_CHILD = 32;

private:
    CWorkControl(const char* pName, CWorkThread* pParent);
    virtual ~CWorkControl();

private:
    char unk1C8[4];

private:
    static CWorkControl* spInstance;
};
/* end "monolib/work/CWorkControl.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 6 "monolib/work/CWorkFlowSetup.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkFlowSetup.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CWorkFlowSetup.hpp" line 4 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */
/* "libs/monolib/include/monolib/work/CWorkFlowSetup.hpp" line 5 "monolib/work/CWorkThreadSystem.hpp" */
/* end "monolib/work/CWorkThreadSystem.hpp" */
/* "libs/monolib/include/monolib/work/CWorkFlowSetup.hpp" line 6 "monolib/work/CWorkUtil.hpp" */
/* end "monolib/work/CWorkUtil.hpp" */

class CWorkFlowSetup : public CWorkThread{
public:
    static CWorkFlowSetup* getInstance();

    DECL_WORKTHREAD_CREATE(CWorkFlowSetup);

    virtual bool wkStandbyLogin();  //0x94
    virtual bool wkStandbyLogout(); //0x98

private:
    CWorkFlowSetup(const char* pName, CWorkThread* pParent);
    virtual ~CWorkFlowSetup();

private:
    u32 unk1C4;

    static CWorkFlowSetup* spInstance;
};
/* end "monolib/work/CWorkFlowSetup.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 7 "monolib/work/CWorkFlowShutdownAll.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkFlowShutdownAll.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CWorkFlowShutdownAll.hpp" line 4 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */

class CWorkFlowShutdownAll : public CWorkThread{
public:
    static CWorkFlowShutdownAll* getInstance();

private:
    static CWorkFlowShutdownAll* spInstance;
};
/* end "monolib/work/CWorkFlowShutdownAll.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 8 "monolib/work/CWorkFlowWiiMenu.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkFlowWiiMenu.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CWorkFlowWiiMenu.hpp" line 4 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */

class CWorkFlowWiiMenu : public CWorkThread{
public:
    static CWorkFlowWiiMenu* getInstance();

private:
    static CWorkFlowWiiMenu* spInstance;
};
/* end "monolib/work/CWorkFlowWiiMenu.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 9 "monolib/work/CWorkFlowWiiPowerOff.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkFlowWiiPowerOff.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CWorkFlowWiiPowerOff.hpp" line 4 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */

class CWorkFlowWiiPowerOff : public CWorkThread{
public:
    static CWorkFlowWiiPowerOff* getInstance();

private:
    static CWorkFlowWiiPowerOff* spInstance;
};
/* end "monolib/work/CWorkFlowWiiPowerOff.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 10 "monolib/work/CWorkFlowWiiReset.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkFlowWiiReset.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/work/CWorkFlowWiiReset.hpp" line 4 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */

class CWorkFlowWiiReset : public CWorkThread{
public:
    static CWorkFlowWiiReset* getInstance();

private:
    static CWorkFlowWiiReset* spInstance;
};
/* end "monolib/work/CWorkFlowWiiReset.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 11 "monolib/work/CWorkRoot.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkRoot.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/CWorkRoot.hpp" line 3 "monolib/monolib_types.hpp" */
/* end "monolib/monolib_types.hpp" */
/* "libs/monolib/include/monolib/work/CWorkRoot.hpp" line 4 "monolib/util.hpp" */
/* end "monolib/util.hpp" */

/*
Main game framework class. It handles setting up/cleaning up most of the framework
components, and contains the core program loop, which is responsible for managing/
updating all of the threads/processes the game uses as well as other things. This
includes the main game process, which gets ran by CDesktop, which itself is ran
by this class.
*/
class CWorkRoot{
public:

    static void initialize();
    static void destroy();

    static void entryWork(CWorkThread* pChild, CWorkThread* pParent, bool prepend);
    static void standbyWork(CWorkThread* pThread, bool arg1);
    static void updateWork(CWorkThread* pThread, bool arg1);
    static void standbyWork();
    static void renderWork();

    static bool runSingle();
    static void exit();
    static void run();
    static void preRetraceCallback(u32 retraceCount);

    static void setException(CException* pException);
    static CException* getException();

private:
    enum ExitMode {
        EXIT_PROG_END,
        EXIT_WII_MENU,
        EXIT_RESTART,
        EXIT_SHUTDOWN
    };

    static bool dummy1(CWorkThread* pThread);
    static inline bool isShutdownAll();
    static inline void initializeComponents();
    static inline void destroyComponents();

    static ExitMode sExitMode;
    static CException* sException;
    static CErrorWii sErrorWii;
};
/* end "monolib/work/CWorkRoot.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 12 "monolib/work/CWorkSystem.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkSystem.hpp" line 2 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */
/* "libs/monolib/include/monolib/work/CWorkSystem.hpp" line 3 "monolib/util.hpp" */
/* end "monolib/util.hpp" */

class CWorkSystem : public CWorkThread {
public:
    typedef void (*ExitFunc)();

public:
    CWorkSystem(const char *pName, CWorkThread *pParent);
    virtual ~CWorkSystem();

    static CWorkSystem* getInstance();
    static bool isOff();
    static mtl::ALLOC_HANDLE getMem();
    static bool isPowerOff();
    static bool isReset();
    static void setSaveLoadInvalidReset(bool state);

    virtual void wkUpdate();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    static CWorkSystem* create();
    DECL_WORKTHREAD_CREATE(CWorkSystem);

    static void setExitFunc(ExitFunc func);
    static void callExitFunc();

private:
    //0x0: vtable
    //0x0-1c4: CWorkThread
    mtl::ALLOC_HANDLE mMemHandle; //0x1C4
    bool mPowerOff; //0x1C8
    bool mReset; //0x1C9
    bool mSaveLoadInvalidReset; //0x1CA
    u8 unk1CB[0x1D0 - 0x1CB];

    static CWorkSystem* spInstance;
    static ExitFunc sExitFunc;
};

//Reset handling functions. Due to string pooling, these had to have been defined outside of a class as static functions.

/* TODO: Ideally this wouldn't need to be a macro, but for files using O4,s (CWorkSystem.cpp), if a function
ends up calling the same function twice, which happens in CWorkSystem::wkUpdate, it refuses to inline it. */
#define prepareReset(){          \
    CWorkSystem::callExitFunc(); \
                                 \
    VISetBlack(VI_TRUE);         \
    VIFlush();                   \
                                 \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
    VIWaitForRetrace();          \
}                           

static inline void resetGame(bool direct){
    if(!direct){
        prepareReset();
    }

    //Restart
    OSReport("exit wii reset\n");
    OSRestart(0);
}

static inline void shutdownGame(bool direct){
    if(!direct){
        prepareReset();
    }

    //Restart
    OSReport("exit wii power off\n");
    OSShutdownSystem();
}


static inline void returnToWiiMenu(bool direct){
    if(!direct){
        prepareReset();
    }

    //Restart
    OSReport("exit wii menu\n");
    OSShutdownSystem();
}
/* end "monolib/work/CWorkSystem.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 13 "monolib/work/CWorkSystemMem.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkSystemMem.hpp" line 2 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */
/* "libs/monolib/include/monolib/work/CWorkSystemMem.hpp" line 3 "monolib/util.hpp" */
/* end "monolib/util.hpp" */

class CWorkSystemMem : public CWorkThread {
public:
    CWorkSystemMem(const char* pName, CWorkThread* pParent);
    virtual ~CWorkSystemMem();

    DECL_WORKTHREAD_CREATE(CWorkSystemMem);

    static mtl::ALLOC_HANDLE getHandle();

    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

private:
    //0x0: vtable
    //0x0-1c4: CWorkThread
    mtl::ALLOC_HANDLE mHandle; //0x1C4

    static const u32 REGION_SIZE = 0x1000 - sizeof(mtl::MemBlock);

    static CWorkSystemMem* spInstance;
};
/* end "monolib/work/CWorkSystemMem.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 14 "monolib/work/CWorkSystemCache.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkSystemCache.hpp" line 2 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */
/* "libs/monolib/include/monolib/work/CWorkSystemCache.hpp" line 3 "monolib/work/CWorkThreadSystem.hpp" */
/* end "monolib/work/CWorkThreadSystem.hpp" */

class CWorkSystemCache : public CWorkThread {
public:
    CWorkSystemCache(const char* pName, CWorkThread* pParent);

    DECL_WORKTHREAD_CREATE(CWorkSystemCache);

    //inline CWorkSystemCache* create(){
    //    return new (CWorkThreadSystem::getWorkMem()) CWorkSystemCache()
    //}

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u8 unk1E8[0x1E8 - 0x1C4];
};
/* end "monolib/work/CWorkSystemCache.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 15 "monolib/work/CWorkSystemPack.hpp" */
#pragma once

/* "libs/monolib/include/monolib/work/CWorkSystemPack.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/work/CWorkSystemPack.hpp" line 3 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */

class CWorkSystemPack : public CWorkThread {
public:
    CWorkSystemPack(const char* pName, CWorkThread* pParent);

    DECL_WORKTHREAD_CREATE(CWorkSystemPack);

    static bool func_804DDFBC(UNKWORD r3);
    static bool func_804DDDF4(const char* r3, void* r4, u32* r5);
    static bool func_804DE08C();
    static bool func_804DE100();
    static void SavePkhFilenamesArrayPtr(const char* const[]);
    static void SaveStaticArcFilenameStringPtr(const char* const*);

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u8 unk1C4[0x208 - 0x1C4];
};
/* end "monolib/work/CWorkSystemPack.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 16 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 17 "monolib/work/CWorkThreadSystem.hpp" */
/* end "monolib/work/CWorkThreadSystem.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 18 "monolib/work/CWorkUtil.hpp" */
/* end "monolib/work/CWorkUtil.hpp" */
/* "libs/monolib/include/monolib/work.hpp" line 19 "monolib/work/IWorkEvent.hpp" */
/* end "monolib/work/IWorkEvent.hpp" */
/* end "monolib/work.hpp" */

/* "src/kyoshin/CUICfManager.hpp" line 6 "nw4r/lyt.h" */
#ifndef NW4R_PUBLIC_LYT_H
#define NW4R_PUBLIC_LYT_H

/* "libs/nw4r/include/nw4r/lyt.h" line 3 "nw4r/lyt/lyt_animation.h" */
#ifndef NW4R_LYT_ANIMATION_H
#define NW4R_LYT_ANIMATION_H
/* "libs/nw4r/include/nw4r/lyt/lyt_animation.h" line 2 "nw4r/types_nw4r.h" */
#ifndef NW4R_TYPES_H
#define NW4R_TYPES_H
/* "libs/nw4r/include/nw4r/types_nw4r.h" line 2 "types.h" */
/* end "types.h" */

#define NW4R_BYTEORDER_BIG 0xFEFF
#define NW4R_BYTEORDER_LITTLE 0xFFFE

#if defined(NW4R_LITTLE_ENDIAN)
#define NW4R_BYTEORDER_NATIVE NW4R_BYTEORDER_LITTLE
#else
#define NW4R_BYTEORDER_NATIVE NW4R_BYTEORDER_BIG
#endif

#define NW4R_VERSION(major, minor) ((major & 0xFF) << 8 | minor & 0xFF)

#define NW4R_LIB_VERSION(NAME, ORIGINAL_DATE, ORIGINAL_TIME, ORIGINAL_CWCC)    \
    const char* NW4R_##NAME##_Version_ =                                       \
        "<< NW4R    - " #NAME " \tfinal   build: " ORIGINAL_DATE               \
        " " ORIGINAL_TIME " (" ORIGINAL_CWCC ") >>"

#endif
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_animation.h" line 4 "nw4r/lyt/lyt_common.h" */
#ifndef NW4R_LYT_COMMON_H
#define NW4R_LYT_COMMON_H
/* "libs/nw4r/include/nw4r/lyt/lyt_common.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_common.h" line 4 "nw4r/math.h" */
#ifndef NW4R_PUBLIC_MATH_H
#define NW4R_PUBLIC_MATH_H

/* "libs/nw4r/include/nw4r/math.h" line 3 "nw4r/math/math_arithmetic.h" */
#ifndef NW4R_MATH_ARITHMETIC_H
#define NW4R_MATH_ARITHMETIC_H
/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 4 "nw4r/db/db_assert.h" */
#ifndef NW4R_DB_ASSERT_H
#define NW4R_DB_ASSERT_H

/* "libs/nw4r/include/nw4r/db/db_assert.h" line 3 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r
{
    namespace db
    {
        // Forward declarations
        namespace detail {
            class ConsoleHead;
        }

        #define NW4R_WARNING(...) nw4r::db::Warning(__FILE__, __LINE__, __VA_ARGS__)
        #define NW4R_PANIC(...) nw4r::db::Panic(__FILE__, __LINE__, __VA_ARGS__)
        #define NW4R_DB_ASSERT(exp, ...) \
            if(!(exp))                \
            nw4r::db::Panic(__FILE__, __LINE__, __VA_ARGS__)

        #define NW4R_DB_ASSERT_WARN(exp, ...) \
            if(!(exp))                \
            nw4r::db::Warning(__FILE__, __LINE__, __VA_ARGS__)

        /* DECL_WEAK */ void VPanic(const char* file, int line, const char* fmt, std::va_list vlist, bool halt);
        /* DECL_WEAK */ void Panic(const char* file, int line, const char* fmt, ...);
        /* DECL_WEAK */ void VWarning(const char* file, int line, const char* fmt, std::va_list vlist);
        /* DECL_WEAK */ void Warning(const char* file, int line, const char* msg, ...);
        
        namespace detail
        {
            void Log(const char* fmt, ...);
        }
        
        detail::ConsoleHead* Assertion_SetConsole(detail::ConsoleHead* console);
        detail::ConsoleHead* Assertion_GetConsole();
        void Assertion_ShowConsole(u32 time);
        void Assertion_HideConsole();
        void Assertion_SetWarningTime(u32 time);
        bool Assertion_SetAutoWarning(bool enable);
    }
}

#endif
/* end "nw4r/db/db_assert.h" */
/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 5 "nw4r/math/math_constant.h" */
#ifndef NW4R_MATH_CONSTANT_H
#define NW4R_MATH_CONSTANT_H
/* "libs/nw4r/include/nw4r/math/math_constant.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_constant.h" line 4 "cmath" */
#ifndef MSL_CPP_CMATH_H
#define MSL_CPP_CMATH_H
/* "libs/PowerPC_EABI_Support/include/stl/cmath" line 2 "math.h" */
#ifndef MSL_MATH_H
#define MSL_MATH_H

/* "libs/PowerPC_EABI_Support/include/stl/math.h" line 3 "types.h" */
/* end "types.h" */

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

/* "libs/PowerPC_EABI_Support/include/stl/math.h" line 9 "PowerPC_EABI_Support/MSL_C/MSL_Common/math_double.h" */
#ifndef MSL_MATH_DOUBLE_H
#define MSL_MATH_DOUBLE_H
/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/math_double.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif


double atan(double x);
double ceil(double x);
double copysign(double x, double y);
double cos(double x);
double floor(double x);
double frexp(double x, int* eptr);
double ldexp(double value, int exp);
double modf(double x, double* iptr);
double sin(double x);
double tan(double x);
double acos(double x);
double asin(double x);
double atan2(double y, double x);
double fmod(double x, double y);
double pow(double x, double y);
double sqrt(double x);
double log(double x);

inline float acosf(float x) {
    return acos(x);
}
inline float ceilf(float x) {
    return ceil(x);
}
inline float cosf(float x) {
    return cos(x);
}
inline float sinf(float x) {
    return sin(x);
}

float sqrtf(float x);

inline float tanf(float x) {
    return tan(x);
}
inline float floorf(float x) {
    return floor(x);
}
inline float fmodf(float x, float y) {
    return fmod(x, y);
}

inline float modff(float x, float* iptr) {
    float frac;
    double intg;

    x = (double)x;
    frac = modf(x, &intg);
    *iptr = intg;

    return frac;
}

float fabsf(float);

#ifdef __cplusplus
}
#endif
#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/math_double.h" */
/* "libs/PowerPC_EABI_Support/include/stl/math.h" line 10 "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h" */
#ifndef _MSL_MATH_API_H
#define _MSL_MATH_API_H

/* "libs/PowerPC_EABI_Support/include/PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h" line 3 "types.h" */
/* end "types.h" */


#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus


#if defined(i386) || defined(i486) || defined(intel) || defined(x86) || defined(i86pc) || defined(__alpha) || defined(__osf__)
#define __LITTLE_ENDIAN
#endif

#ifdef __LITTLE_ENDIAN
#define __HI(x)  *(1 + (int*)&x)
#define __LO(x)  *(int*)&x
#define __HIp(x) *(1 + (int*)x)
#define __LOp(x) *(int*)x
#else
#define __HI(x)  *(int*)&x
#define __LO(x)  *(1 + (int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1 + (int*)x)
#endif


#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "PowerPC_EABI_Support/MSL_C/MSL_Common/math_api.h" */

extern int __float_nan[];
extern int __float_huge[];
extern int __double_huge[];

#define INFINITY (*(float*) __float_huge)
#define NAN (*(float*) __float_nan)
#define HUGE_VAL (*(double*) __double_huge)

#define FP_NAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL

#define M_PI    3.1415927f
#define M_SQRT3 1.73205f

int __signbitf(float);
int __fpclassifyf(float);
int __signbitd(double);
int __fpclassifyd(double);

#define fpclassify(x) ((sizeof(x) == sizeof(float)) ? __fpclassifyf((float)(x)) : __fpclassifyd((double)(x)))
#define signbit(x) ((sizeof(x) == sizeof(float)) ? __signbitf((float)(x)) : __signbitd((double)(x)))

#define isfinite(x) ((fpclassify(x) > FP_INFINITE))
#define isnan(x) (fpclassify(x) == FP_NAN)
#define isnormal(x) (fpclassify(x) == FP_NORMAL)

inline long double fabsl(long double x) {
    return __fabs((double)x);
}

double __ieee754_acos(double);
double __ieee754_fmod(double, double);
double __ieee754_log(double);
double __ieee754_log10(double);
double __ieee754_pow(double, double);
double __ieee754_sqrt(double);
double __ieee754_atan2(double, double);
double __ieee754_asin(double);

double copysign(double, double);
double scalbn(double, int);

double __kernel_sin(double, double, int);
double __kernel_cos(double, double);
double __kernel_tan(double, double, int);

int __ieee754_rem_pio2(double, double *);

double nan(const char* arg);

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
/* end "math.h" */
#ifdef __cplusplus

namespace std {
using ::acos;
using ::acosf;
using ::asin;
using ::atan;
using ::atan2;
using ::ceil;
using ::ceilf;
using ::copysign;
using ::cos;
using ::cosf;
using ::fabsf;
using ::floor;
using ::floorf;
using ::fmod;
using ::fmodf;
using ::frexp;
using ::ldexp;
using ::modf;
using ::modff;
using ::nan;
using ::pow;
using ::scalbn;
using ::sin;
using ::sinf;
using ::sqrt;
using ::sqrtf;
using ::tan;
using ::tanf;
} // namespace std

#endif
#endif
/* end "cmath" */

/******************************************************************************
 *
 * Mathematical constants
 *
 ******************************************************************************/
// Pi mathematical constant
#define NW4R_MATH_PI M_PI

// ln(2)
#define NW4R_MATH_LN_2 0.69314718056f

// 1 / sqrt(3)
#define NW4R_MATH_INVSQRT3 0.577350258f

/******************************************************************************
 *
 * Not-a-number (NaN) types
 *
 ******************************************************************************/
// Quiet NaN (0x7FC00000)
#define NW4R_MATH_QNAN (-(0.0f / 0.0f))

/******************************************************************************
 *
 * Float limits
 *
 ******************************************************************************/
// Smallest finite floating-point value
#define NW4R_MATH_FLT_MIN 1.175494350e-38f

// Largest finite floating-point value
#define NW4R_MATH_FLT_MAX 3.402823466e+38f

// Difference between 1.0 and the next representable value
#define NW4R_MATH_FLT_EPSILON 1.192092895e-7f

#endif
/* end "nw4r/math/math_constant.h" */

/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 7 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */

/* "libs/nw4r/include/nw4r/math/math_arithmetic.h" line 9 "cmath" */
/* end "cmath" */

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * Implementation details
 *
 ******************************************************************************/
namespace detail {

f32 FExp(f32 x);
f32 FLog(f32 x);

} // namespace detail

/******************************************************************************
 *
 * Arithmetic functions
 *
 ******************************************************************************/
f32 FrSqrt(f32 x);

inline f32 FAbs(register f32 x) {
    register f32 ax;

    ASM (
        fabs ax, x
    )

    return ax;
}

inline f32 FCeil(f32 x) {
    return std::ceilf(x);
}

inline f32 FExp(f32 x) {
    return detail::FExp(x);
}

inline f32 FFloor(f32 x) {
    return std::floorf(x);
}

inline f32 FInv(register f32 x) {
    register f32 work0, work1, work2, work3;

    ASM (
        fmr  work1, x     // x
        fres work0, work1 // 1/x

        // Refine estimate
        ps_add   work2, work0, work0        // 2/x
        ps_mul   work3, work0, work0        // 1/x^2
        ps_nmsub work0, work1, work3, work2 // -(x * 1/x^2 - 2/x)
    )

    return work0;
}

inline f32 FMod(f32 x, f32 y) {
    return std::fmodf(x, y);
}

inline f32 FModf(f32 x, f32* pY) {
    return std::modff(x, pY);
}

inline f32 FSqrt(f32 x) {
#line 627
    NW4R_DB_ASSERT_WARN(x >= 0.0f, "FSqrt: Input is out of the domain.");

    return x <= 0.0f ? 0.0f : x * FrSqrt(x);
}

inline f32 FLog(f32 x) {
    if (x > 0.0f) {
        return detail::FLog(x);
    }

    return NW4R_MATH_QNAN;
}

inline f32 FSelect(register f32 value, register f32 ge_zero,
                   register f32 lt_zero) {
    register f32 ret;

    ASM (
        fsel ret, value, ge_zero, lt_zero
    )

    return ret;
}

/******************************************************************************
 *
 * Fastcast functions
 *
 ******************************************************************************/
inline f32 U16ToF32(u16 arg) {
    f32 ret;
    OSu16tof32(&arg, &ret);
    return ret;
}
inline u16 F32ToU16(f32 arg) {
    u16 ret;
    OSf32tou16(&arg, &ret);
    return ret;
}

inline f32 S16ToF32(s16 arg) {
    f32 ret;
    OSs16tof32(&arg, &ret);
    return ret;
}
inline s16 F32ToS16(f32 arg) {
    s16 ret;
    OSf32tos16(&arg, &ret);
    return ret;
}

inline u32 F32AsU32(f32 arg) {
    return *reinterpret_cast<u32*>(&arg);
}
inline f32 U32AsF32(u32 arg) {
    return *reinterpret_cast<f32*>(&arg);
}

inline s32 FGetExpPart(f32 x) {
    s32 s = F32AsU32(x);
    return ((s >> 23) & 0xFF) - 127;
}
inline f32 FGetMantPart(f32 x) {
    u32 u = F32AsU32(x);
    return U32AsF32((u & 0x807FFFFF) | 0x3F800000);
}

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_arithmetic.h" */
/* "libs/nw4r/include/nw4r/math.h" line 4 "nw4r/math/math_geometry.h" */
#ifndef NW4R_MATH_GEOMETRY_H
#define NW4R_MATH_GEOMETRY_H
/* "libs/nw4r/include/nw4r/math/math_geometry.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_geometry.h" line 4 "nw4r/math/math_types.h" */
#ifndef NW4R_MATH_TYPES_H
#define NW4R_MATH_TYPES_H
/* "libs/nw4r/include/nw4r/math/math_types.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_types.h" line 4 "nw4r/math/math_arithmetic.h" */
/* end "nw4r/math/math_arithmetic.h" */
/* "libs/nw4r/include/nw4r/math/math_types.h" line 5 "nw4r/math/math_triangular.h" */
#ifndef NW4R_MATH_TRIANGULAR_H
#define NW4R_MATH_TRIANGULAR_H
/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 4 "nw4r/db/db_assert.h" */
/* end "nw4r/db/db_assert.h" */
/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 5 "nw4r/math/math_arithmetic.h" */
/* end "nw4r/math/math_arithmetic.h" */
/* "libs/nw4r/include/nw4r/math/math_triangular.h" line 6 "nw4r/math/math_constant.h" */
/* end "nw4r/math/math_constant.h" */

/******************************************************************************
 *
 * Macros to convert between units
 *
 ******************************************************************************/
// [0, 65535] -> [0.0, 256.0f]
#define NW4R_MATH_IDX_TO_FIDX(x) ((x) * (1.0f / 256.0f))

// Convert degrees <-> lookup-table index
#define NW4R_MATH_DEG_TO_FIDX(x) ((x) * (256.0f / 360.0f))
#define NW4R_MATH_FIDX_TO_DEG(x) ((x) * (360.0f / 256.0f))

// Convert radians <-> lookup-table index
#define NW4R_MATH_RAD_TO_FIDX(x) ((x) * (128.0f / NW4R_MATH_PI))
#define NW4R_MATH_FIDX_TO_RAD(x) ((x) * (NW4R_MATH_PI / 128.0f))

// Convert degrees <-> radians
#define NW4R_MATH_DEG_TO_RAD(x) ((x) * (NW4R_MATH_PI / 180.0f))
#define NW4R_MATH_RAD_TO_DEG(x) ((x) * (180.0f / NW4R_MATH_PI))

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * Sin functions
 *
 ******************************************************************************/
f32 SinFIdx(f32 fidx);

inline f32 SinIdx(u16 idx) {
    return SinFIdx(NW4R_MATH_IDX_TO_FIDX(U16ToF32(idx)));
}
inline f32 SinDeg(f32 deg) {
    return SinFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
}
inline f32 SinRad(f32 rad) {
    return SinFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Cosine functions
 *
 ******************************************************************************/
f32 CosFIdx(f32 fidx);

inline f32 CosDeg(f32 deg) {
    return CosFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
}
inline f32 CosRad(f32 rad) {
    return CosFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Tangent functions
 *
 ******************************************************************************/
inline f32 TanFIdx(f32 fidx) {
    return std::tanf(NW4R_MATH_FIDX_TO_RAD(fidx));
}
inline f32 TanDeg(f32 deg) {
    return TanFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
}
inline f32 TanRad(f32 rad) {
    return TanFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Sin & cosine functions
 *
 ******************************************************************************/
void SinCosFIdx(f32* pSin, f32* pCos, f32 fidx);

inline void SinCosDeg(f32* pSin, f32* pCos, f32 deg) {
    return SinCosFIdx(pSin, pCos, NW4R_MATH_DEG_TO_FIDX(deg));
}
inline void SinCosRad(f32* pSin, f32* pCos, f32 rad) {
    return SinCosFIdx(pSin, pCos, NW4R_MATH_RAD_TO_FIDX(rad));
}

/******************************************************************************
 *
 * Arc-sine functions
 *
 ******************************************************************************/

 inline f32 AsinRad(f32 x){
    bool inRange = x <= 1.0f && x >= -1.0f;

#line 228
    NW4R_DB_ASSERT_WARN(inRange, "AsinRad: Input is out of the domain.");

    return std::asin(x);
}

/******************************************************************************
 *
 * Arc-cosine functions
 *
 ******************************************************************************/

 inline f32 AcosRad(f32 x){
    bool inRange = x <= 1.0f && x >= -1.0f;

#line 239
    NW4R_DB_ASSERT_WARN(inRange, "AcosRad: Input is out of the domain.");

    return std::acos(x);
}

/******************************************************************************
 *
 * Arc-tangent functions
 *
 ******************************************************************************/
f32 AtanFIdx(f32 x);

inline f32 AtanDeg(f32 x) {
    return NW4R_MATH_FIDX_TO_DEG(AtanFIdx(x));
}
inline f32 AtanRad(f32 x) {
    return NW4R_MATH_FIDX_TO_RAD(AtanFIdx(x));
}

/******************************************************************************
 *
 * Arc-tangent (2-argument) functions
 *
 ******************************************************************************/
f32 Atan2FIdx(f32 y, f32 x);

inline f32 Atan2Deg(f32 y, f32 x) {
    return NW4R_MATH_FIDX_TO_DEG(Atan2FIdx(y, x));
}
inline f32 Atan2Rad(f32 y, f32 x) {
    return NW4R_MATH_FIDX_TO_RAD(Atan2FIdx(y, x));
}

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_triangular.h" */

/* "libs/nw4r/include/nw4r/math/math_types.h" line 7 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace nw4r {
namespace math {

/******************************************************************************
 *
 * VEC2 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _VEC2 {
    f32 x, y;
};

struct VEC2 : _VEC2 {
    VEC2() {}
    VEC2(f32 fx, f32 fy) {
        x = fx;
        y = fy;
    }

    operator f32*() {
        return reinterpret_cast<f32*>(this);
    }
    operator const f32*() const {
        return reinterpret_cast<const f32*>(this);
    }

    VEC2 operator+(const VEC2& rRhs) const {
        return VEC2(x + rRhs.x, y + rRhs.y);
    }
    VEC2 operator-(const VEC2& rRhs) const {
        return VEC2(x - rRhs.x, y - rRhs.y);
    }

    VEC2& operator+=(const VEC2& rRhs) {
        x += rRhs.x;
        y += rRhs.y;
        return *this;
    }
    VEC2& operator-=(const VEC2& rRhs) {
        x -= rRhs.x;
        y -= rRhs.y;
        return *this;
    }

    bool operator==(const VEC2& rRhs) const {
        return x == rRhs.x && y == rRhs.y;
    }
    bool operator!=(const VEC2& rRhs) const {
        return x != rRhs.x || y != rRhs.y;
    }
};

/******************************************************************************
 *
 * VEC3 structure
 *
 ******************************************************************************/
// Forward declarations
struct VEC3;
VEC3* VEC3Add(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3Sub(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3Scale(VEC3* pOut, const VEC3* pIn, f32 scale);

// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _VEC3 {
    f32 x, y, z;
};

struct VEC3 : _VEC3 {
    VEC3() {}
    VEC3(f32 fx, f32 fy, f32 fz) {
        x = fx;
        y = fy;
        z = fz;
    }
    VEC3(const _VEC3& rVec) {
        x = rVec.x;
        y = rVec.y;
        z = rVec.z;
    }
    VEC3(const Vec& rVec) {
        x = rVec.x;
        y = rVec.y;
        z = rVec.z;
    }
    VEC3(const f32* pData) {
        x = pData[0];
        y = pData[1];
        z = pData[2];
    }

    operator Vec*() {
        return reinterpret_cast<Vec*>(this);
    }
    operator const Vec*() const {
        return reinterpret_cast<const Vec*>(this);
    }

    f32 LenSq() const {
        return x * x + y * y + z * z;
    }

    VEC3 operator-() const {
        return VEC3(-x, -y, -z);
    }

    VEC3 operator+(const VEC3& rRhs) const {
        VEC3 out;
        VEC3Add(&out, this, &rRhs);
        return out;
    }
    VEC3 operator-(const VEC3& rRhs) const {
        VEC3 out;
        VEC3Sub(&out, this, &rRhs);
        return out;
    }
    VEC3 operator*(f32 x) const {
        VEC3 out;
        VEC3Scale(&out, this, x);
        return out;
    }
    VEC3 operator/(f32 x) const {
        f32 r = 1 / x;
        return *this * r;
    }

    VEC3& operator+=(const VEC3& rRhs) {
        VEC3Add(this, this, &rRhs);
        return *this;
    }
    VEC3& operator-=(const VEC3& rRhs) {
        VEC3Sub(this, this, &rRhs);
        return *this;
    }
    VEC3& operator*=(f32 x) {
        VEC3Scale(this, this, x);
        return *this;
    }
    VEC3& operator/=(f32 x) {
        return *this *= (1 / x);
    }

    bool operator==(const VEC3& rRhs) const {
        return x == rRhs.x && y == rRhs.y && z == rRhs.z;
    }
    bool operator!=(const VEC3& rRhs) const {
        return x != rRhs.x || y != rRhs.y || z != rRhs.z;
    }
};

/******************************************************************************
 *
 * MTX33 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _MTX33 {
    union {
        struct {
            f32 _00, _01, _02;
            f32 _10, _11, _12;
            f32 _20, _21, _22;
        };

        f32 m[3][3];
        f32 a[3 * 3];
    };
};

struct MTX33 : _MTX33 {
    MTX33() {}
};

/******************************************************************************
 *
 * MTX34 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _MTX34 {
    union {
        struct {
            f32 _00, _01, _02, _03;
            f32 _10, _11, _12, _13;
            f32 _20, _21, _22, _23;
        };

        f32 m[3][4];
        f32 a[3 * 4];
        Mtx mtx;
    };
};

struct MTX34 : _MTX34 {
    typedef f32 (*MtxRef)[4];
    typedef const f32 (*MtxRefConst)[4];

    MTX34() {}

    // clang-format off
    MTX34(f32 f00, f32 f01, f32 f02, f32 f03,
          f32 f10, f32 f11, f32 f12, f32 f13,
          f32 f20, f32 f21, f32 f22, f32 f23) {
        _00 = f00; _01 = f01; _02 = f02; _03 = f03;
        _10 = f10; _11 = f11; _12 = f12; _13 = f13;
        _20 = f20; _21 = f21; _22 = f22; _23 = f23;
    }
    // clang-format on

    operator MtxRef() {
        return mtx;
    }
    operator MtxRefConst() const {
        return mtx;
    }
};

/******************************************************************************
 *
 * MTX44 structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _MTX44 {
    union {
        struct {
            f32 _00, _01, _02, _03;
            f32 _10, _11, _12, _13;
            f32 _20, _21, _22, _23;
            f32 _30, _31, _32, _33;
        };

        f32 m[4][4];
        f32 a[4 * 4];
        Mtx44 mtx;
    };
};

struct MTX44 : _MTX44 {
    typedef f32 (*Mtx44Ref)[4];
    typedef const f32 (*Mtx44RefConst)[4];

    MTX44() {}

    operator Mtx44Ref() {
        return mtx;
    }
    operator Mtx44RefConst() const {
        return mtx;
    }
};

/******************************************************************************
 *
 * QUAT structure
 *
 ******************************************************************************/
// Provides a POD type that can be upcasted to the real structure.
// (Has the added benefit of forcing struct copies to use lwz/stw.)
struct _QUAT {
    f32 x, y, z, w;
};

struct QUAT : _QUAT {
    QUAT() {}
    QUAT(f32 fx, f32 fy, f32 fz, f32 fw) {
        x = fx;
        y = fy;
        z = fz;
        w = fw;
    }

    // TODO(kiwi) These are not real AFAIK. Do they really manually cast the
    // QUAT?
    operator Quaternion*() {
        return reinterpret_cast<Quaternion*>(this);
    }
    operator const Quaternion*() const {
        return reinterpret_cast<const Quaternion*>(this);
    }
};

/******************************************************************************
 *
 * VEC2 functions
 *
 ******************************************************************************/
inline f32 VEC2Len(const VEC2* pVec) {
    return FSqrt(pVec->x * pVec->x + pVec->y * pVec->y);
}

/******************************************************************************
 *
 * VEC3 functions
 *
 ******************************************************************************/
VEC3* VEC3Maximize(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3Minimize(VEC3* pOut, const VEC3* pA, const VEC3* pB);
VEC3* VEC3TransformNormal(VEC3* pOut, const MTX34* pMtx, const VEC3* pVec);

inline VEC3* VEC3Add(register VEC3* pOut, register const VEC3* pA,
                     register const VEC3* pB) {
    register f32 work0, work1, work2;

    ASM (
        // Add XY
        psq_l  work0, VEC3.x(pA),   0, 0
        psq_l  work1, VEC3.x(pB),   0, 0
        ps_add work2, work0, work1
        psq_st work2, VEC3.x(pOut), 0, 0
        
        // Add Z
        psq_l  work0, VEC3.z(pA),   1, 0
        psq_l  work1, VEC3.z(pB),   1, 0
        ps_add work2, work0, work1
        psq_st work2, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline f32 VEC3Dot(register const VEC3* pA, register const VEC3* pB) {
    register f32 dot;
    register f32 work0, work1, work2, work3;

    ASM (
        // YZ product
        psq_l  work0, VEC3.y(pA), 0, 0
        psq_l  work1, VEC3.y(pB), 0, 0
        ps_mul work0, work0, work1
        
        // X product + YZ product
        psq_l   work3, VEC3.x(pA), 1, 0
        psq_l   work2, VEC3.x(pB), 1, 0
        ps_madd work1, work3, work2, work0
        
        // Dot product
        ps_sum0 dot, work1, work0, work0
    )

    return dot;
}

inline f32 VEC3LenSq(register const VEC3* pVec) {
    register f32 work0, work1, work2;

    ASM (
        // Square XY
        psq_l  work0, VEC3.x(pVec), 0, 0
        ps_mul work0, work0, work0

        // Square Z
        lfs     work1, VEC3.z(pVec)
        ps_madd work2, work1, work1, work0

        // Sum products
        ps_sum0 work2, work2, work0, work0
    )

    return work2;
}

inline VEC3* VEC3Lerp(register VEC3* pOut, register const VEC3* pVec1,
                      register const VEC3* pVec2, register f32 t) {
    register f32 work0, work1, work2;

    ASM (
        // X/Y delta
        psq_l  work0, VEC3.x(pVec1), 0, 0
        psq_l  work1, VEC3.x(pVec2), 0, 0
        ps_sub work2, work1, work0
        // Scale with time and add to v0
        ps_madds0 work2, work2, t, work0
        psq_st    work2, VEC3.x(pOut), 0, 0
        
        // Z delta
        psq_l  work0, VEC3.z(pVec1), 1, 0
        psq_l  work1, VEC3.z(pVec2), 1, 0
        ps_sub work2, work1, work0
        // Scale with time and add to v0
        ps_madds0 work2, work2, t, work0
        psq_st    work2, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline VEC3* VEC3Scale(register VEC3* pOut, register const VEC3* pIn,
                       register f32 scale) {
    register f32 work0, work1;

    ASM (
        // Scale XY
        psq_l    work0, VEC3.x(pIn),  0, 0
        ps_muls0 work1, work0, scale
        psq_st   work1, VEC3.x(pOut), 0, 0

        // Scale Z
        psq_l    work0, VEC3.z(pIn),  1, 0
        ps_muls0 work1, work0, scale
        psq_st   work1, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline VEC3* VEC3Sub(register VEC3* pOut, register const VEC3* pA,
                     register const VEC3* pB) {
    register f32 work0, work1, work2;

    ASM (
        // Sub XY
        psq_l  work0, VEC3.x(pA),   0, 0
        psq_l  work1, VEC3.x(pB),   0, 0
        ps_sub work2, work0, work1
        psq_st work2, VEC3.x(pOut), 0, 0
        
        // Sub Z
        psq_l  work0, VEC3.z(pA),   1, 0
        psq_l  work1, VEC3.z(pB),   1, 0
        ps_sub work2, work0, work1
        psq_st work2, VEC3.z(pOut), 1, 0
    )

    return pOut;
}

inline VEC3* VEC3Cross(VEC3* pOut, const VEC3* pA, const VEC3* pB) {
    PSVECCrossProduct(*pA, *pB, *pOut);
    return pOut;
}

inline f32 VEC3DistSq(const VEC3* pA, const VEC3* pB) {
    return PSVECSquareDistance(*pA, *pB);
}

inline f32 VEC3Len(const VEC3* pVec) {
    return PSVECMag(*pVec);
}

inline VEC3* VEC3Normalize(VEC3* pOut, const VEC3* pIn) {
    PSVECNormalize(*pIn, *pOut);
    return pOut;
}

inline VEC3* VEC3Transform(VEC3* pOut, const MTX34* pMtx, const VEC3* pVec) {
    PSMTXMultVec(*pMtx, *pVec, *pOut);
    return pOut;
}

inline VEC3* VEC3TransformCoord(VEC3* pOut, const MTX34* pMtx,
                                const VEC3* pVec) {
    PSMTXMultVec(*pMtx, *pVec, *pOut);
    return pOut;
}

/******************************************************************************
 *
 * MTX33 functions
 *
 ******************************************************************************/
MTX33* MTX33Identity(MTX33* pMtx);

/******************************************************************************
 *
 * MTX34 functions
 *
 ******************************************************************************/
MTX33* MTX34ToMTX33(MTX33* pOut, const MTX34* pIn);
u32 MTX34InvTranspose(MTX33* pOut, const MTX34* pIn);
MTX34* MTX34Zero(MTX34* pMtx);
MTX34* MTX34Scale(MTX34* pOut, const MTX34* pIn, const VEC3* pScale);
MTX34* MTX34Trans(MTX34* pOut, const MTX34* pIn, const VEC3* pTrans);
MTX34* MTX34RotAxisFIdx(MTX34* pMtx, const VEC3* pAxis, f32 fidx);
MTX34* MTX34RotXYZFIdx(MTX34* pMtx, f32 fx, f32 fy, f32 fz);

inline MTX34* MTX34Copy(MTX34* pOut, const MTX34* pIn) {
    PSMTXCopy(*pIn, *pOut);
    return pOut;
}

inline MTX34* MTX34Identity(MTX34* pMtx) {
    PSMTXIdentity(*pMtx);
    return pMtx;
}

inline u32 MTX34Inv(MTX34* pOut, const MTX34* pIn) {
    return PSMTXInverse(*pIn, *pOut);
}

inline u32 MTX34InvTranspose(MTX34* pOut, const MTX34* pIn) {
    return PSMTXInvXpose(*pIn, *pOut);
}

inline MTX34* MTX34LookAt(MTX34* pMtx, const VEC3* pPos, const VEC3* pUp,
                          const VEC3* pTarget) {
    C_MTXLookAt(*pMtx, *pPos, *pUp, *pTarget);
    return pMtx;
}

inline MTX34* MTX34Mult(MTX34* pOut, const MTX34* pA, const MTX34* pB) {
    PSMTXConcat(*pA, *pB, *pOut);
    return pOut;
}

inline MTX34* MTX34MultArray(MTX34* pOut, const MTX34* p1, const MTX34* pSrc,
                             u32 len) {
    PSMTXConcatArray(*p1, *pSrc, *pOut, len);
    return pOut;
}

inline MTX34* MTX34RotAxisRad(MTX34* pOut, const VEC3* pAxis, f32 frad) {
    return MTX34RotAxisFIdx(pOut, pAxis, NW4R_MATH_RAD_TO_FIDX(frad));
}

inline MTX34* MTX34RotXYZDeg(MTX34* pMtx, f32 dx, f32 dy, f32 dz) {
    return MTX34RotXYZFIdx(pMtx, NW4R_MATH_DEG_TO_FIDX(dx),
                           NW4R_MATH_DEG_TO_FIDX(dy),
                           NW4R_MATH_DEG_TO_FIDX(dz));
}

inline MTX34* MTX34RotXYZRad(MTX34* pMtx, f32 rx, f32 ry, f32 rz) {
    return MTX34RotXYZFIdx(pMtx, NW4R_MATH_RAD_TO_FIDX(rx),
                           NW4R_MATH_RAD_TO_FIDX(ry),
                           NW4R_MATH_RAD_TO_FIDX(rz));
}

inline MTX34* MTX34Scale(MTX34* pOut, const VEC3* pScale, const MTX34* pIn) {
    PSMTXScaleApply(*pIn, *pOut, pScale->x, pScale->y, pScale->z);
    return pOut;
}

inline QUAT* MTX34ToQUAT(QUAT* pQuat, const MTX34* pMtx) {
    C_QUATMtx(*pQuat, *pMtx);
    return pQuat;
}

inline MTX34* MTX34Trans(MTX34* pOut, const VEC3* pTrans, const MTX34* pIn) {
    PSMTXTransApply(*pIn, *pOut, pTrans->x, pTrans->y, pTrans->z);
    return pOut;
}

/******************************************************************************
 *
 * MTX44 functions
 *
 ******************************************************************************/
MTX44* MTX44Identity(MTX44* pMtx);
MTX44* MTX44Copy(MTX44* pDst, const MTX44* pSrc);

/******************************************************************************
 *
 * QUAT functions
 *
 ******************************************************************************/
inline MTX34* QUATToMTX34(MTX34* pMtx, const QUAT* pQuat) {
    PSMTXQuat(*pMtx, *pQuat);
    return pMtx;
}

// @bug QUATSlerp macro changes this function name!
inline QUAT* C_QUATSlerp(QUAT* pOut, const QUAT* p1, const QUAT* p2, f32 t) {
    ::C_QUATSlerp(*p1, *p2, *pOut, t);
    return pOut;
}

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_types.h" */

namespace nw4r {
namespace math {

// Forward declarations
struct AABB;

enum IntersectionResult {
    INTERSECTION_NONE,
    INTERSECTION_1,
    INTERSECTION_2,

    INTERSECTION_LINE3_ON_PLANE = INTERSECTION_2,
    INTERSECTION_RAY3_ON_PLANE = INTERSECTION_2,
    INTERSECTION_SEGMENT3_ON_PLANE = INTERSECTION_2,

    INTERSECTION_OUTSIDE = 0,
    INTERSECTION_INSIDE,
    INTERSECTION_INTERSECT
};

bool IntersectionAABB(const AABB* pA, const AABB* pB);

/******************************************************************************
 *
 * Plane
 *
 ******************************************************************************/
struct PLANE {
    PLANE() {}

    f32 Test(const VEC3& rPoint) const {
        return d + VEC3Dot(&n, &rPoint);
    }

    void Set(const VEC3* p0, const VEC3* p1, const VEC3* p2);

    VEC3 n; // at 0x0
    f32 d;  // at 0xC
};

/******************************************************************************
 *
 * Axis-aligned bounding box
 *
 ******************************************************************************/
struct AABB {
    AABB() {}

    void Set(const VEC3* pPoints, unsigned int num);
    void Set(const AABB* pBox, const MTX34* pMtx);

    VEC3 min; // at 0x0
    VEC3 max; // at 0xC
};

/******************************************************************************
 *
 * Frustum
 *
 ******************************************************************************/
class FRUSTUM {
private:
    enum Point {
        POINT_NEAR_TL,
        POINT_NEAR_TR,
        POINT_NEAR_BR,
        POINT_NEAR_BL,

        POINT_FAR_TL,
        POINT_FAR_TR,
        POINT_FAR_BR,
        POINT_FAR_BL,

        POINT_MAX
    };

    enum Plane {
        PLANE_L,
        PLANE_R,
        PLANE_N,
        PLANE_F,
        PLANE_T,
        PLANE_B,

        PLANE_MAX
    };

public:
    void Set(f32 fovy, f32 aspect, f32 n, f32 f, const MTX34& rCamMtx);
    void Set(f32 t, f32 b, f32 l, f32 r, f32 n, f32 f, const MTX34& rCamMtx);

    IntersectionResult IntersectAABB_Ex(const AABB* pBox) const;

private:
    MTX34 mCamMtx;            // at 0x0
    PLANE mPlaneL;            // at 0x30
    PLANE mPlaneR;            // at 0x40
    PLANE mPlaneT;            // at 0x50
    PLANE mPlaneB;            // at 0x60
    f32 mNearZ;               // at 0x70
    f32 mFarZ;                // at 0x74
    AABB mBox;                // at 0x78
    PLANE mPlanes[PLANE_MAX]; // at 0x90
};

} // namespace math
} // namespace nw4r

#endif
/* end "nw4r/math/math_geometry.h" */
/* "libs/nw4r/include/nw4r/math.h" line 5 "nw4r/math/math_triangular.h" */
/* end "nw4r/math/math_triangular.h" */
/* "libs/nw4r/include/nw4r/math.h" line 6 "nw4r/math/math_types.h" */
/* end "nw4r/math/math_types.h" */

#endif
/* end "nw4r/math.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_common.h" line 5 "nw4r/ut.h" */
#ifndef NW4R_PUBLIC_UT_H
#define NW4R_PUBLIC_UT_H

/* "libs/nw4r/include/nw4r/ut.h" line 3 "nw4r/ut/ut_CharStrmReader.h" */
#ifndef NW4R_UT_CHAR_STRM_READER_H
#define NW4R_UT_CHAR_STRM_READER_H
/* "libs/nw4r/include/nw4r/ut/ut_CharStrmReader.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace ut {

class CharStrmReader {
public:
    typedef u16 (CharStrmReader::*ReadFunc)();

public:
    explicit CharStrmReader(ReadFunc pFunc)
        : mCharStrm(NULL), mReadFunc(pFunc) {}

    ~CharStrmReader() {}

    u16 ReadNextCharUTF8();
    u16 ReadNextCharUTF16();
    u16 ReadNextCharCP1252();
    u16 ReadNextCharSJIS();

    u16 Next() {
        return (this->*mReadFunc)();
    }

    const void* GetCurrentPos() const {
        return mCharStrm;
    }

    void Set(const char* pStrm) {
        mCharStrm = pStrm;
    }
    void Set(const wchar_t* pStrm) {
        mCharStrm = pStrm;
    }

private:
    template <typename T> T GetChar(int offset) const {
        return static_cast<const T*>(mCharStrm)[offset];
    }

    template <typename T> void StepStrm(int offset) {
        static_cast<const T*>(mCharStrm) += offset;
    }

private:
    const void* mCharStrm; // at 0x0
    ReadFunc mReadFunc;    // at 0x4
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_CharStrmReader.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 4 "nw4r/ut/ut_CharWriter.h" */
#ifndef NW4R_UT_CHAR_WRITER_H
#define NW4R_UT_CHAR_WRITER_H
/* "libs/nw4r/include/nw4r/ut/ut_CharWriter.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_CharWriter.h" line 4 "nw4r/ut/ut_Color.h" */
#ifndef NW4R_UT_COLOR_H
#define NW4R_UT_COLOR_H
/* "libs/nw4r/include/nw4r/ut/ut_Color.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_Color.h" line 4 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */

namespace nw4r {
namespace ut {

struct Color : public GXColor {
public:
    Color() {
        *this = WHITE;
    }
    Color(u32 color) {
        *this = color;
    }
    Color(int red, int green, int blue, int alpha) {
        Set(red, green, blue, alpha);
    }
    Color(const GXColor& rColor) {
        *this = rColor;
    }

    ~Color() {}

    void Set(int red, int green, int blue, int alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    Color& operator=(u32 color) {
        ToU32ref() = color;
        return *this;
    }
    Color& operator=(const GXColor& rColor) {
        *this = *reinterpret_cast<const u32*>(&rColor);
        return *this;
    }

    Color operator|(u32 color) const {
        return Color(ToU32() | color);
    }
    Color operator&(u32 color) const {
        return Color(ToU32() & color);
    }

    u32& ToU32ref() {
        return *reinterpret_cast<u32*>(this);
    }
    const u32& ToU32ref() const {
        return *reinterpret_cast<const u32*>(this);
    }

    u32 ToU32() const {
        return ToU32ref();
    }

    operator u32() const {
        return ToU32ref();
    }

    // clang-format off
    static const u32 RED   = 0xFF0000FF;
    static const u32 GREEN = 0x00FF00FF;
    static const u32 BLUE  = 0x0000FFFF;

    static const u32 CYAN    = 0x00FFFFFF;
    static const u32 MAGENTA = 0xFF00FFFF;
    static const u32 YELLOW  = 0xFFFF00FF;

    static const u32 BLACK = 0x000000FF;
    static const u32 GRAY  = 0x808080FF;
    static const u32 WHITE = 0xFFFFFFFF;

    static const u32 ELEMENT_MIN = 0x00000000;
    static const u32 ELEMENT_MAX = 0xFFFFFFFF;
    // clang-format on
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_Color.h" */

/* "libs/nw4r/include/nw4r/ut/ut_CharWriter.h" line 6 "nw4r/math.h" */
/* end "nw4r/math.h" */

/* "libs/nw4r/include/nw4r/ut/ut_CharWriter.h" line 8 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */

namespace nw4r {
namespace ut {

// Forward declarations
class Font;
struct Glyph;

class CharWriter {
public:
    enum GradationMode {
        GRADMODE_NONE,
        GRADMODE_H,
        GRADMODE_V,

        GRADMODE_MAX
    };

public:
    CharWriter();
    ~CharWriter();

    void SetupGX();
    void EnableLinearFilter(bool atSmall, bool atLarge);
    f32 Print(u16 ch);

    void SetColorMapping(Color min, Color max) {
        mColorMapping.min = min;
        mColorMapping.max = max;
    }

    void ResetColorMapping() {
        SetColorMapping(DEFAULT_COLOR_MAPPING_MIN, DEFAULT_COLOR_MAPPING_MAX);
    }

    void SetTextColor(Color start) {
        mTextColor.start = start;
        UpdateVertexColor();
    }

    void SetTextColor(Color start, Color end) {
        mTextColor.start = start;
        mTextColor.end = end;
        UpdateVertexColor();
    }

    void SetGradationMode(GradationMode mode) {
        mTextColor.gradationMode = mode;
        UpdateVertexColor();
    }

    f32 GetScaleH() const {
        return mScale.x;
    }
    f32 GetScaleV() const {
        return mScale.y;
    }

    void SetScale(f32 x, f32 y) {
        mScale.x = x;
        mScale.y = y;
    }

    f32 GetCursorX() const {
        return mCursorPos.x;
    }
    void SetCursorX(f32 x) {
        mCursorPos.x = x;
    }

    f32 GetCursorY() const {
        return mCursorPos.y;
    }
    void SetCursorY(f32 y) {
        mCursorPos.y = y;
    }

    void SetCursor(f32 x, f32 y) {
        mCursorPos.x = x;
        mCursorPos.y = y;
    }
    void SetCursor(f32 x, f32 y, f32 z) {
        mCursorPos.x = x;
        mCursorPos.y = y;
        mCursorPos.z = z;
    }

    void MoveCursorX(f32 dx) {
        mCursorPos.x += dx;
    }
    void MoveCursorY(f32 dy) {
        mCursorPos.y += dy;
    }

    void SetAlpha(u8 alpha) {
        mAlpha = alpha;
        UpdateVertexColor();
    }
    u8 GetAlpha() const {
        return mAlpha;
    }

    void EnableFixedWidth(bool enable) {
        mIsWidthFixed = enable;
    }
    bool IsWidthFixed() const {
        return mIsWidthFixed;
    }

    void SetFixedWidth(f32 width) {
        mFixedWidth = width;
    }
    f32 GetFixedWidth() const {
        return mFixedWidth;
    }

    void SetFont(const Font& rFont) {
        mFont = &rFont;
    }
    const Font* GetFont() const {
        return mFont;
    }

    void SetFontSize(f32 width, f32 height);
    void SetFontSize(f32 height);

    f32 GetFontWidth() const;
    f32 GetFontHeight() const;
    f32 GetFontAscent() const;
    f32 GetFontDescent() const;

private:
    struct ColorMapping {
        Color min; // at 0x0
        Color max; // at 0x4
    };

    struct VertexColor {
        Color lu; // at 0x0
        Color ru; // at 0x4
        Color ld; // at 0x8
        Color rd; // at 0xC
    };

    struct TextColor {
        Color start;                 // at 0x0
        Color end;                   // at 0x4
        GradationMode gradationMode; // at 0x8
    };

    struct TextureFilter {
        GXTexFilter atSmall; // at 0x0
        GXTexFilter atLarge; // at 0x4

        bool operator!=(const TextureFilter& rOther) const {
            return atSmall != rOther.atSmall || atLarge != rOther.atLarge;
        }
    };

    struct LoadingTexture {
        GXTexMapID slot;      // at 0x0
        void* texture;        // at 0x4
        TextureFilter filter; // at 0x8

        bool operator!=(const LoadingTexture& rOther) const {
            return slot != rOther.slot || texture != rOther.texture ||
                   filter != rOther.filter;
        }

        void Reset() {
            slot = GX_TEXMAP_NULL;
            texture = NULL;
        }
    };

    static const u32 DEFAULT_COLOR_MAPPING_MIN = 0x00000000;
    static const u32 DEFAULT_COLOR_MAPPING_MAX = 0xFFFFFFFF;

private:
    static void SetupVertexFormat();
    static void SetupGXDefault();
    static void SetupGXWithColorMapping(Color min, Color max);
    static void SetupGXForI();
    static void SetupGXForRGBA();

    void UpdateVertexColor();

    void DrawGlyph(const Glyph& glyph);
    void PrintGlyph(f32 x, f32 y, f32 z, const Glyph& rGlyph);

    void LoadTexture(const Glyph& rGlyph, GXTexMapID slot);
    void ResetTextureCache() {
        mLoadingTexture.Reset();
    }

private:
    ColorMapping mColorMapping;   // at 0x0
    VertexColor mVertexColor;     // at 0x8
    TextColor mTextColor;         // at 0x18
    math::VEC2 mScale;            // at 0x24
    math::VEC3 mCursorPos;        // at 0x2C
    TextureFilter mFilter;        // at 0x38
    u8 PADDING_0x40[0x42 - 0x40]; // at 0x40
    u8 mAlpha;                    // at 0x42
    bool mIsWidthFixed;           // at 0x43
    f32 mFixedWidth;              // at 0x44
    const Font* mFont;            // at 0x48

    static LoadingTexture mLoadingTexture;
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_CharWriter.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 5 "nw4r/ut/ut_Color.h" */
/* end "nw4r/ut/ut_Color.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 6 "nw4r/ut/ut_DvdFileStream.h" */
#ifndef NW4R_UT_DVD_FILE_STREAM_H
#define NW4R_UT_DVD_FILE_STREAM_H
/* "libs/nw4r/include/nw4r/ut/ut_DvdFileStream.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_DvdFileStream.h" line 4 "nw4r/ut/ut_FileStream.h" */
#ifndef NW4R_UT_FILE_STREAM_H
#define NW4R_UT_FILE_STREAM_H
/* "libs/nw4r/include/nw4r/ut/ut_FileStream.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_FileStream.h" line 4 "nw4r/ut/ut_IOStream.h" */
#ifndef NW4R_UT_IO_STREAM_H
#define NW4R_UT_IO_STREAM_H
/* "libs/nw4r/include/nw4r/ut/ut_IOStream.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_IOStream.h" line 4 "nw4r/ut/ut_RuntimeTypeInfo.h" */
#ifndef NW4R_UT_RUNTIME_TYPE_INFO_H
#define NW4R_UT_RUNTIME_TYPE_INFO_H
/* "libs/nw4r/include/nw4r/ut/ut_RuntimeTypeInfo.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace ut {
namespace detail {

/******************************************************************************
 *
 * RuntimeTypeInfo
 *
 ******************************************************************************/
struct RuntimeTypeInfo {
    explicit RuntimeTypeInfo(const RuntimeTypeInfo* pBase)
        : mParentTypeInfo(pBase) {}

    bool IsDerivedFrom(const RuntimeTypeInfo* pInfo) const {
        for (const RuntimeTypeInfo* pIt = this; pIt != NULL;
             pIt = pIt->mParentTypeInfo) {

            if (pIt == pInfo) {
                return true;
            }
        }

        return false;
    }

    const RuntimeTypeInfo* mParentTypeInfo; // at 0x0
};

template <typename T>
inline const RuntimeTypeInfo* GetTypeInfoFromPtr_(T* /* pPtr */) {
    return &T::typeInfo;
}

} // namespace detail

/******************************************************************************
 *
 * DynamicCast
 *
 ******************************************************************************/
template <typename TDerived, typename TBase>
inline TDerived DynamicCast(TBase* pPtr) {
    const detail::RuntimeTypeInfo* pDerivedTypeInfo =
        detail::GetTypeInfoFromPtr_(static_cast<TDerived>(NULL));

    // Downcast only if possible
    if (pPtr->GetRuntimeTypeInfo()->IsDerivedFrom(pDerivedTypeInfo)) {
        return static_cast<TDerived>(pPtr);
    }

    return NULL;
}

} // namespace ut
} // namespace nw4r

/******************************************************************************
 *
 * Macros
 *
 ******************************************************************************/
/**
 * Declare type RTTI and accessor function.
 */
#define NW4R_UT_RTTI_DECL(T)                                                   \
    virtual const nw4r::ut::detail::RuntimeTypeInfo* GetRuntimeTypeInfo()      \
        const {                                                                \
        return &typeInfo;                                                      \
    }                                                                          \
                                                                               \
    static nw4r::ut::detail::RuntimeTypeInfo typeInfo;

/**
 * Define type RTTI (base type).
 */
#define NW4R_UT_RTTI_DEF_BASE(T)                                               \
    nw4r::ut::detail::RuntimeTypeInfo T::typeInfo(NULL)

/**
 * Define type RTTI (derived type).
 */
#define NW4R_UT_RTTI_DEF_DERIVED(T, BASE)                                      \
    nw4r::ut::detail::RuntimeTypeInfo T::typeInfo(&BASE::typeInfo)

#endif
/* end "nw4r/ut/ut_RuntimeTypeInfo.h" */

namespace nw4r {
namespace ut {

extern "C" detail::RuntimeTypeInfo lbl_eu_80665540;

class IOStream {
public:
    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {
        return &lbl_eu_80665540;
    }

    typedef void (*StreamCallback)(s32 result, IOStream* pStream,
                                   void* pCallbackArg);

public:
    IOStream() : mAvailable(false), mCallback(NULL), mArg(NULL) {}
    virtual ~IOStream() {} // at 0xC

    virtual void Close() = 0; // at 0x10

    virtual s32 Read(void* pDst, u32 size) = 0; // at 0x14
    virtual bool ReadAsync(void* pDst, u32 size, StreamCallback pCallback,
                           void* pCallbackArg); // at 0x18

    virtual bool Write(const void* pSrc, u32 size); // at 0x1C
    virtual bool WriteAsync(const void* pSrc, u32 size,
                            StreamCallback pCallback,
                            void* pCallbackArg); // at 0x20

    virtual bool IsBusy() const; // at 0x24

    virtual bool CanAsync() const = 0; // at 0x28
    virtual bool CanRead() const = 0;  // at 0x2C
    virtual bool CanWrite() const = 0; // at 0x30

    virtual u32 GetOffsetAlign() const; // at 0x34
    virtual u32 GetSizeAlign() const; // at 0x38
    virtual u32 GetBufferAlign() const; // at 0x3C

    bool IsAvailable() const {
        return mAvailable;
    }

protected:
    bool mAvailable;          // at 0x4
    s32 mAsyncResult;         // at 0x8
    StreamCallback mCallback; // at 0xC
    void* mArg;               // at 0x10
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_IOStream.h" */

namespace nw4r {
namespace ut {

extern "C" detail::RuntimeTypeInfo lbl_eu_80665548;

class FileStream : public IOStream {
public:
    virtual const detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {
        return &lbl_eu_80665548;
    }

    enum SeekOrigin { SEEK_ORIGIN_BEG, SEEK_ORIGIN_CUR, SEEK_ORIGIN_END };

public:
    FileStream() {}
    virtual ~FileStream() {} // at 0xC

    virtual u32 GetSize() const = 0; // at 0x40

    virtual void Seek(s32 offset, u32 origin); // at 0x44

    virtual void Cancel(); // at 0x48
    virtual bool CancelAsync(StreamCallback pCallback,
                             void* pCallbackArg); // at 0x4C

    virtual bool CanSeek() const = 0;   // at 0x50
    virtual bool CanCancel() const = 0; // at 0x54

    virtual u32 Tell() const = 0; // at 0x58

protected:
    class FilePosition {
    public:
        FilePosition() : mFileSize(0), mPosition(0) {}

        u32 GetFileSize() const {
            return mFileSize;
        }
        void SetFileSize(u32 size) {
            mFileSize = size;
        }

        u32 Tell() const {
            return mPosition;
        }

        u32 Skip(s32 offset);
        u32 Append(s32 offset);
        void Seek(s32 offset, u32 origin);

    private:
        u32 mFileSize; // at 0x0
        u32 mPosition; // at 0x4
    };
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_FileStream.h" */

/* "libs/nw4r/include/nw4r/ut/ut_DvdFileStream.h" line 6 "revolution/DVD.h" */
/**
 * References: WiiBrew, YAGCD
 */

#ifndef RVL_SDK_PUBLIC_DVD_H
#define RVL_SDK_PUBLIC_DVD_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/DVD.h" line 10 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/DVD.h" line 11 "revolution/DVD/dvd_broadway.h" */
#ifndef RVL_SDK_DVD_BROADWAY_H
#define RVL_SDK_DVD_BROADWAY_H
/* "libs/RVL_SDK/include/revolution/DVD/dvd_broadway.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define DVD_LOW_OFFSET(x) ((x) >> 2)
#define DVD_LOW_SPEED(x) (((x) & 3) << 16)

// Forward declarations
typedef struct DVDDiskID;
typedef struct DVDDriveInfo;
typedef struct ESTicket;
typedef struct ESTicketView;
typedef struct ESTitleMeta;
typedef struct OSAlarm;
typedef struct DVDVideoReportKey;

typedef enum {
    DVD_INTTYPE_TC = (1 << 0),   // Transaction callback?
    DVD_INTTYPE_DE = (1 << 1),   // Drive error
    DVD_INTTYPE_CVR = (1 << 2),  // Something with DVD cover
    DVD_INTTYPE_BR = (1 << 3),   // Break requested
    DVD_INTTYPE_TIME = (1 << 4), // Time out
    DVD_INTTYPE_SERR = (1 << 5), // Security error
    DVD_INTTYPE_VERR = (1 << 6), // Verify error
    DVD_INTTYPE_ARGS = (1 << 7), // Bad arguments
} DVDLowIntType;

// DICVR - DI Cover Register (via DVDLowGetCoverRegister)
#define DVD_DICVR_CVR (1 << 0)
#define DVD_DICVR_CVRINTMASK (1 << 1)
#define DVD_DICVR_CVRINT (1 << 2)

typedef void (*DVDLowCallback)(u32 intType);

BOOL DVDLowInit(void);
BOOL DVDLowReadDiskID(struct DVDDiskID* out, DVDLowCallback callback);
//BOOL DVDLowOpenPartition(u32 offset, const ESTicket* ticket, u32 certsSize,
//                         const void* certs, ESTitleMeta* tmd,
//                         DVDLowCallback callback);
BOOL DVDLowOpenPartitionWithTmdAndTicketView(const u32, const struct ESTicketView* const, const u32,
                                             const struct ESTitleMeta* const, const u32, const u8* const,
                                             DVDLowCallback);
BOOL DVDLowGetNoDiscBufferSizes(const u32 offset, u32* numTmdBytes, u32* numCertBytes, DVDLowCallback callback);
BOOL DVDLowGetNoDiscOpenPartitionParams(const u32 offset, struct ESTicket* eTicket, u32* numTmdBytes,
                                        struct ESTitleMeta* tmd, u32* numCertBytes, u8* certificates, u32* dataWordOffset,
                                        u8* h3HashPtr, DVDLowCallback callback);
BOOL DVDLowOpenPartition(const u32, const struct ESTicket* const, const u32,
                         const u8* const, struct ESTitleMeta *, DVDLowCallback);
BOOL DVDLowClosePartition(DVDLowCallback callback);
BOOL DVDLowUnencryptedRead(void* dst, u32 size, u32 offset,
                           DVDLowCallback callback);
BOOL DVDLowStopMotor(BOOL eject, BOOL kill, DVDLowCallback callback);
BOOL DVDLowInquiry(struct DVDDriveInfo* out, DVDLowCallback callback);
BOOL DVDLowRequestError(DVDLowCallback callback);
BOOL DVDLowSetSpinupFlag(BOOL enable);
BOOL DVDLowReset(DVDLowCallback callback);
BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback);
BOOL DVDLowSetMaximumRotation(u32 speed, DVDLowCallback callback);
BOOL DVDLowRead(void* dst, u32 size, u32 offset, DVDLowCallback callback);
BOOL DVDLowSeek(u32 offset, DVDLowCallback callback);
u32 DVDLowGetCoverRegister(void);
BOOL DVDLowPrepareCoverRegister(DVDLowCallback callback);
u32 DVDLowGetImmBufferReg(void);
BOOL DVDLowUnmaskStatusInterrupts(void);
BOOL DVDLowMaskCoverInterrupt(void);
BOOL DVDLowClearCoverInterrupt(DVDLowCallback callback);
BOOL __DVDLowTestAlarm(const struct OSAlarm* alarm);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvd_broadway.h" */
/* "libs/RVL_SDK/include/revolution/DVD.h" line 12 "revolution/DVD/dvdDeviceError.h" */
#ifndef RVL_SDK_DVD_DEVICE_ERROR_H
#define RVL_SDK_DVD_DEVICE_ERROR_H
/* "libs/RVL_SDK/include/revolution/DVD/dvdDeviceError.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

BOOL __DVDCheckDevice(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvdDeviceError.h" */
/* "libs/RVL_SDK/include/revolution/DVD.h" line 13 "revolution/DVD/dvderror.h" */
#ifndef RVL_SDK_DVD_ERROR_H
#define RVL_SDK_DVD_ERROR_H
/* "libs/RVL_SDK/include/revolution/DVD/dvderror.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef void (*DVDErrorCallback)(s32 result, s32 arg1);

#define DVD_ERROR_CMD_MAX 5

typedef struct DVDErrorInfo {
    char game[4]; // at 0x0
    u8 disk;      // at 0x4
    u8 version;   // at 0x5
    u32 error;    // at 0x8
    s32 sec;      // at 0xC
    u32 disr;     // at 0x10
    u32 dicr;     // at 0x14
    u32 next;     // at 0x18

    struct {
        u32 command; // at 0x1C
        u32 param1;  // at 0x20
        u32 param2;  // at 0x24
        u32 intType; // at 0x28
        u32 tick;    // at 0x2C
    } info[DVD_ERROR_CMD_MAX];
} DVDErrorInfo;

extern DVDErrorInfo __ErrorInfo;

void __DVDStoreErrorCode(u32 error, DVDErrorCallback callback);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvderror.h" */
/* "libs/RVL_SDK/include/revolution/DVD.h" line 14 "revolution/DVD/dvdfatal.h" */
#ifndef RVL_SDK_DVD_FATAL_H
#define RVL_SDK_DVD_FATAL_H
/* "libs/RVL_SDK/include/revolution/DVD/dvdfatal.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

void __DVDShowFatalMessage(void);
BOOL DVDSetAutoFatalMessaging(BOOL enable);
BOOL __DVDGetAutoFatalMessaging(void);
void __DVDPrintFatalMessage(void);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvdfatal.h" */
/* "libs/RVL_SDK/include/revolution/DVD.h" line 15 "revolution/DVD/dvdfs.h" */
#ifndef RVL_SDK_DVD_FS_H
#define RVL_SDK_DVD_FS_H
/* "libs/RVL_SDK/include/revolution/DVD/dvdfs.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/DVD/dvdfs.h" line 4 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */

/* "libs/RVL_SDK/include/revolution/DVD/dvdfs.h" line 6 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */
#ifdef __cplusplus
extern "C" {
#endif

extern OSThreadQueue __DVDThreadQueue;
extern BOOL __DVDLongFileNameFlag;

void __DVDFSInit(void);
s32 DVDConvertPathToEntrynum(const char* path);
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* info);
BOOL DVDOpen(const char* path, DVDFileInfo* info);
BOOL DVDClose(DVDFileInfo* info);
BOOL DVDGetCurrentDir(char* buffer, u32 maxlen);
BOOL DVDReadAsyncPrio(DVDFileInfo* info, void* dst, s32 size, s32 offset,
                      DVDAsyncCallback callback, s32 prio);
s32 DVDReadPrio(DVDFileInfo* info, void* dst, s32 size, s32 offset, s32 prio);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvdfs.h" */
/* "libs/RVL_SDK/include/revolution/DVD.h" line 16 "revolution/DVD/dvdidutils.h" */
#ifndef RVL_SDK_DVD_ID_UTILS_H
#define RVL_SDK_DVD_ID_UTILS_H
/* "libs/RVL_SDK/include/revolution/DVD/dvdidutils.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/DVD/dvdidutils.h" line 4 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
#ifdef __cplusplus
extern "C" {
#endif

BOOL DVDCompareDiskID(const DVDDiskID* id1, const DVDDiskID* id2);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvdidutils.h" */
/* "libs/RVL_SDK/include/revolution/DVD.h" line 17 "revolution/DVD/dvdqueue.h" */
#ifndef RVL_SDK_DVD_QUEUE_H
#define RVL_SDK_DVD_QUEUE_H
/* "libs/RVL_SDK/include/revolution/DVD/dvdqueue.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/DVD/dvdqueue.h" line 4 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DVD_PRIO_HIGHEST,
    DVD_PRIO_HIGH,
    DVD_PRIO_MEDIUM,
    DVD_PRIO_LOW,

    DVD_PRIO_MAX,
} DVDQueuePriority;

void __DVDClearWaitingQueue(void);
BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block);
DVDCommandBlock* __DVDPopWaitingQueue(void);
BOOL __DVDCheckWaitingQueue(void);
DVDCommandBlock* __DVDGetNextWaitingQueue(void);
BOOL __DVDDequeueWaitingQueue(const DVDCommandBlock* block);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD/dvdqueue.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/DVD.h" */

namespace nw4r {
namespace ut {

class DvdFileStream : public FileStream {
public:
    NW4R_UT_RTTI_DECL(DvdFileStream);

public:
    explicit DvdFileStream(s32 entrynum);
    DvdFileStream(const DVDFileInfo* pInfo, bool close);
    virtual ~DvdFileStream(); // at 0xC

    bool Open(s32 entrynum);
    bool Open(const DVDFileInfo* pInfo, bool close);

    void SetPriority(s32 priority) {
        mPriority = priority;
    }

    virtual void Close(); // at 0x10

    virtual s32 Read(void* pDst, u32 size); // at 0x14
    virtual bool ReadAsync(void* pDst, u32 size, StreamCallback pCallback,
                           void* pCallbackArg); // at 0x18

    virtual s32 Peek(void* pDst, u32 size); // at 0x5C
    virtual bool PeekAsync(void* pDst, u32 size, StreamCallback pCallback,
                           void* pCallbackArg); // at 0x60

    virtual void Seek(s32 offset, u32 origin); // at 0x44

    virtual void Cancel(); // at 0x48
    virtual bool CancelAsync(StreamCallback pCallback,
                             void* pCallbackArg); // at 0x4C

    virtual bool IsBusy() const {
        return mIsBusy;
    } // at 0x24

    virtual u32 Tell() const {
        return mFilePosition.Tell();
    } // at 0x58
    virtual u32 GetSize() const {
        return mFilePosition.GetFileSize();
    } // at 0x40

    virtual bool CanAsync() const {
        return true;
    } // at 0x28
    virtual bool CanSeek() const {
        return true;
    } // at 0x50
    virtual bool CanRead() const {
        return true;
    } // at 0x2C
    virtual bool CanWrite() const {
        return false;
    } // at 0x30
    virtual bool CanCancel() const {
        return true;
    } // at 0x54

    virtual u32 GetOffsetAlign() const {
        return 4;
    } // at 0x34
    virtual u32 GetSizeAlign() const {
        return 32;
    } // at 0x38
    virtual u32 GetBufferAlign() const {
        return 32;
    } // at 0x3C

private:
    struct DvdFileStreamInfo {
        DVDFileInfo dvdInfo;   // at 0x0
        DvdFileStream* stream; // at 0x3C
    };

private:
    static void DvdAsyncCallback_(s32 result, DVDFileInfo* pInfo);
    static void DvdCBAsyncCallback_(s32 result, DVDCommandBlock* pBlock);

    void Initialize_();
    u32 AdjustReadLength_(u32 len);

private:
    FilePosition mFilePosition;     // at 0x14
    StreamCallback mCancelCallback; // at 0x1C
    void* mCancelArg;               // at 0x20
    volatile bool mIsCanceling;     // at 0x24
    DvdFileStreamInfo mFileInfo;    // at 0x28
    s32 mPriority;                  // at 0x68
    volatile bool mIsBusy;          // at 0x6C
    bool mCloseOnDestroyFlg;        // at 0x6D
    bool mCloseEnableFlg;           // at 0x6E
};

} // namespace ut
} // namespace nw4r
#endif
/* end "nw4r/ut/ut_DvdFileStream.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 7 "nw4r/ut/ut_DvdLockedFileStream.h" */
#ifndef NW4R_UT_DVD_LOCKED_FILE_STREAM_H
#define NW4R_UT_DVD_LOCKED_FILE_STREAM_H
/* "libs/nw4r/include/nw4r/ut/ut_DvdLockedFileStream.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_DvdLockedFileStream.h" line 4 "nw4r/ut/ut_DvdFileStream.h" */
/* end "nw4r/ut/ut_DvdFileStream.h" */

/* "libs/nw4r/include/nw4r/ut/ut_DvdLockedFileStream.h" line 6 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */

namespace nw4r {
namespace ut {

class DvdLockedFileStream : public DvdFileStream {
public:
    NW4R_UT_RTTI_DECL(DvdLockedFileStream);

public:
    explicit DvdLockedFileStream(s32 entrynum);
    DvdLockedFileStream(const DVDFileInfo* pInfo, bool close);
    virtual ~DvdLockedFileStream(); // at 0xC

    virtual s32 Read(void* pDst, u32 size); // at 0x14

    virtual bool ReadAsync(void* /* pDst */, u32 /* size */,
                           StreamCallback /* pCallback */,
                           void* /* pCallbackArg */) {

        return false;
    } // at 0x18

    virtual s32 Peek(void* pDst, u32 size); // at 0x5C

    virtual bool PeekAsync(void* /* pDst */, u32 /* size */,
                           StreamCallback /* pCallback */,
                           void* /* pCallbackArg */) {

        return false;
    } // at 0x60

    virtual bool CanAsync() const {
        return false;
    } // at 0x28

private:
    static void InitMutex_();

private:
    bool mCancelFlag; // at 0x6F

    static bool sInitialized;
    static OSMutex sMutex;
};

} // namespace ut
} // namespace nw4r
#endif
/* end "nw4r/ut/ut_DvdLockedFileStream.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 8 "nw4r/ut/ut_FileStream.h" */
/* end "nw4r/ut/ut_FileStream.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 9 "nw4r/ut/ut_Font.h" */
#ifndef NW4R_UT_FONT_H
#define NW4R_UT_FONT_H
/* "libs/nw4r/include/nw4r/ut/ut_Font.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_Font.h" line 4 "nw4r/ut/ut_CharStrmReader.h" */
/* end "nw4r/ut/ut_CharStrmReader.h" */

/* "libs/nw4r/include/nw4r/ut/ut_Font.h" line 6 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */

namespace nw4r {
namespace ut {

enum FontEncoding {
    FONT_ENCODING_UTF8,
    FONT_ENCODING_UTF16,
    FONT_ENCODING_SJIS,
    FONT_ENCODING_CP1252,

    FONT_ENCODING_MAX
};

struct CharWidths {
    s8 left;       // at 0x0
    u8 glyphWidth; // at 0x1
    s8 charWidth;  // at 0x2
};

struct Glyph {
    void* pTexture;     // at 0x0
    CharWidths widths;  // at 0x4
    u8 height;          // at 0x7
    GXTexFmt texFormat; // at 0x8
    u16 texWidth;       // at 0xC
    u16 texHeight;      // at 0xE
    u16 cellX;          // at 0x10
    u16 cellY;          // at 0x12
};

/******************************************************************************
 *
 * Font
 *
 ******************************************************************************/
class Font {
public:
    enum Type { TYPE_NULL, TYPE_ROM, TYPE_RESOURCE, TYPE_PAIR };

public:
    Font() : mReadFunc(&CharStrmReader::ReadNextCharCP1252) {}
    virtual ~Font() {} // at 0x8

    virtual int GetWidth() const = 0;  // at 0xC
    virtual int GetHeight() const = 0; // at 0x10

    virtual int GetAscent() const = 0;      // at 0x14
    virtual int GetDescent() const = 0;     // at 0x18
    virtual int GetBaselinePos() const = 0; // at 0x1C

    virtual int GetCellHeight() const = 0;   // at 0x20
    virtual int GetCellWidth() const = 0;    // at 0x24
    virtual int GetMaxCharWidth() const = 0; // at 0x28

    virtual Type GetType() const = 0;              // at 0x2C
    virtual GXTexFmt GetTextureFormat() const = 0; // at 0x30
    virtual int GetLineFeed() const = 0;           // at 0x34

    virtual CharWidths GetDefaultCharWidths() const = 0;              // at 0x38
    virtual void SetDefaultCharWidths(const CharWidths& rWidths) = 0; // at 0x3C

    virtual bool SetAlternateChar(u16 ch) = 0; // at 0x40
    virtual void SetLineFeed(int lf) = 0;      // at 0x44

    virtual int GetCharWidth(u16 ch) const = 0;             // at 0x48
    virtual CharWidths GetCharWidths(u16 ch) const = 0;     // at 0x4C
    virtual void GetGlyph(Glyph* pGlyph, u16 ch) const = 0; // at 0x50
    virtual bool HasGlyph(u16 ch) const = 0;                // at 0x54
    virtual FontEncoding GetEncoding() const = 0;           // at 0x58

    void InitReaderFunc(FontEncoding encode);

    CharStrmReader GetCharStrmReader() const {
        return CharStrmReader(mReadFunc);
    }

private:
    CharStrmReader::ReadFunc mReadFunc; // at 0x4
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_Font.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 10 "nw4r/ut/ut_IOStream.h" */
/* end "nw4r/ut/ut_IOStream.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 11 "nw4r/ut/ut_LinkList.h" */
#ifndef NW4R_UT_LINK_LIST_H
#define NW4R_UT_LINK_LIST_H
/* "libs/nw4r/include/nw4r/ut/ut_LinkList.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_LinkList.h" line 4 "nw4r/ut/ut_NonCopyable.h" */
#ifndef NW4R_UT_NON_COPYABLE_H
#define NW4R_UT_NON_COPYABLE_H
/* "libs/nw4r/include/nw4r/ut/ut_NonCopyable.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace ut {
namespace {

class NonCopyable {
protected:
    NonCopyable() {}
    NonCopyable(const NonCopyable& /* rOther */) {}
    ~NonCopyable() {}
};

} // namespace
} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_NonCopyable.h" */

namespace nw4r {
namespace ut {

// Forward declarations
namespace detail {
class LinkListImpl;
} // namespace detail

/******************************************************************************
 *
 * Linked list node
 *
 ******************************************************************************/
class LinkListNode : private NonCopyable {
    friend class detail::LinkListImpl;

public:
    LinkListNode() : mNext(NULL), mPrev(NULL) {}

    LinkListNode* GetNext() const {
        return mNext;
    }
    LinkListNode* GetPrev() const {
        return mPrev;
    }

private:
    LinkListNode* mNext; // at 0x0
    LinkListNode* mPrev; // at 0x4
};

namespace detail {

/******************************************************************************
 *
 * Linked list implementation
 *
 ******************************************************************************/
class LinkListImpl : private NonCopyable {
public:
    // Forward declarations
    class ConstIterator;

    /******************************************************************************
     * Iterator implementation
     ******************************************************************************/
    class Iterator {
        friend class LinkListImpl;
        friend class ConstIterator;

    public:
        Iterator() : mNode(NULL) {}
        explicit Iterator(LinkListNode* pNode) : mNode(pNode) {}

        Iterator& operator++() {
            mNode = mNode->GetNext();
            return *this;
        }

        Iterator& operator--() {
            mNode = mNode->GetPrev();
            return *this;
        }

        LinkListNode* operator->() const {
            return mNode;
        }

        friend bool operator==(LinkListImpl::Iterator lhs,
                               LinkListImpl::Iterator rhs) {
            return lhs.mNode == rhs.mNode;
        }

    private:
        LinkListNode* mNode; // at 0x0
    };

    /******************************************************************************
     * Iterator implementation (const-view)
     ******************************************************************************/
    class ConstIterator {
        friend class LinkListImpl;

    public:
        explicit ConstIterator(Iterator it) : mNode(it.mNode) {}

        ConstIterator& operator++() {
            mNode = mNode->GetNext();
            return *this;
        }

        ConstIterator& operator--() {
            mNode = mNode->GetPrev();
            return *this;
        }

        const LinkListNode* operator->() const {
            return mNode;
        }

        friend bool operator==(LinkListImpl::ConstIterator lhs,
                               LinkListImpl::ConstIterator rhs) {
            return lhs.mNode == rhs.mNode;
        }

    private:
        LinkListNode* mNode; // at 0x0
    };

protected:
    static Iterator GetIteratorFromPointer(LinkListNode* pNode) {
        return Iterator(pNode);
    }

    LinkListImpl() {
        Initialize_();
    }
    ~LinkListImpl();

    Iterator GetBeginIter() {
        return Iterator(mNode.GetNext());
    }
    Iterator GetEndIter() {
        return Iterator(&mNode);
    }

    Iterator Insert(Iterator it, LinkListNode* pNode);

    Iterator Erase(Iterator it);
    Iterator Erase(LinkListNode* pNode);
    Iterator Erase(Iterator begin, Iterator end);

public:
    u32 GetSize() const {
        return mSize;
    }
    bool IsEmpty() const {
        return mSize == 0;
    }

    void PopFront() {
        Erase(GetBeginIter());
    }
    void PopBack() {
        Erase(--GetEndIter());
    }

    void Clear();

private:
    void Initialize_() {
        mSize = 0;
        mNode.mNext = &mNode;
        mNode.mPrev = &mNode;
    }

private:
    u32 mSize;          // at 0x0
    LinkListNode mNode; // at 0x4
};

/******************************************************************************
 *
 * Reverse iterator
 *
 ******************************************************************************/
template <typename TIter> class ReverseIterator {
public:
    explicit ReverseIterator(TIter it) : mCurrent(it) {}

    TIter GetBase() const {
        return mCurrent;
    }

    ReverseIterator& operator++() {
        --mCurrent;
        return *this;
    }

    const typename TIter::TElem* operator->() const {
        return &this->operator*();
    }

    typename TIter::TElem& operator*() const {
        TIter it = mCurrent;
        return *--it;
    }

    friend bool operator==(const ReverseIterator& rLhs,
                           const ReverseIterator& rRhs) {
        return rLhs.mCurrent == rRhs.mCurrent;
    }

    friend bool operator!=(const ReverseIterator& rLhs,
                           const ReverseIterator& rRhs) {
        return !(rLhs.mCurrent == rRhs.mCurrent);
    }

private:
    TIter mCurrent; // at 0x0
};

} // namespace detail

/******************************************************************************
 *
 * Templated linked list
 *
 ******************************************************************************/
template <typename T, int Ofs> class LinkList : public detail::LinkListImpl {
public:
    // Forward declarations
    class ConstIterator;

    /******************************************************************************
     * Templated iterator
     ******************************************************************************/
    class Iterator {
        friend class LinkList;
        friend class ConstIterator;

    public:
        // Element type must be visible to ReverseIterator
        typedef T TElem;

    public:
        Iterator() : mIterator(NULL) {}
        explicit Iterator(LinkListImpl::Iterator it) : mIterator(it) {}

        Iterator& operator++() {
            ++mIterator;
            return *this;
        }

        Iterator& operator--() {
            --mIterator;
            return *this;
        }

        Iterator operator++(int) {
            Iterator ret = *this;
            ++*this;
            return ret;
        }

        T* operator->() const {
            return GetPointerFromNode(mIterator.operator->());
        }

        T& operator*() const {
            return *this->operator->();
        }

        friend bool operator==(Iterator lhs, Iterator rhs) {
            return lhs.mIterator == rhs.mIterator;
        }

        friend bool operator!=(Iterator lhs, Iterator rhs) {
            return !(lhs == rhs);
        }

    private:
        LinkListImpl::Iterator mIterator; // at 0x0
    };

    /******************************************************************************
     * Templated iterator (const-view)
     ******************************************************************************/
    class ConstIterator {
        friend class LinkList;

    public:
        // Element type must be visible to ReverseIterator
        typedef T TElem;

    public:
        explicit ConstIterator(LinkListImpl::Iterator it) : mIterator(it) {}
        explicit ConstIterator(Iterator it) : mIterator(it.mIterator) {}

        ConstIterator& operator++() {
            ++mIterator;
            return *this;
        }

        ConstIterator& operator--() {
            --mIterator;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator ret = *this;
            ++*this;
            return ret;
        }

        const T* operator->() const {
            return GetPointerFromNode(mIterator.operator->());
        }

        const T& operator*() const {
            return *this->operator->();
        }

        friend bool operator==(ConstIterator lhs, ConstIterator rhs) {
            return lhs.mIterator == rhs.mIterator;
        }

        friend bool operator!=(ConstIterator lhs, ConstIterator rhs) {
            return !(lhs == rhs);
        }

    private:
        LinkListImpl::ConstIterator mIterator; // at 0x0
    };

public:
    // Shorthand names for reverse iterator types
    typedef detail::ReverseIterator<Iterator> RevIterator;
    typedef detail::ReverseIterator<ConstIterator> ConstRevIterator;

public:
    LinkList() {}

    Iterator GetBeginIter() {
        return Iterator(LinkListImpl::GetBeginIter());
    }
    ConstIterator GetBeginIter() const {
        return ConstIterator(const_cast<LinkList*>(this)->GetBeginIter());
    }
    RevIterator GetBeginReverseIter() {
        return RevIterator(GetBeginIter());
    }
    ConstRevIterator GetBeginReverseIter() const {
        return ConstRevIterator(GetBeginIter());
    }

    Iterator GetEndIter() {
        return Iterator(LinkListImpl::GetEndIter());
    }
    ConstIterator GetEndIter() const {
        return ConstIterator(const_cast<LinkList*>(this)->GetEndIter());
    }
    RevIterator GetEndReverseIter() {
        return RevIterator(GetEndIter());
    }
    ConstRevIterator GetEndReverseIter() const {
        return ConstRevIterator(GetEndIter());
    }

    Iterator Insert(Iterator it, T* pElem) {
        return Iterator(
            LinkListImpl::Insert(it.mIterator, GetNodeFromPointer(pElem)));
    }

    Iterator Erase(T* pElem) {
        return Iterator(LinkListImpl::Erase(GetNodeFromPointer(pElem)));
    }
    Iterator Erase(Iterator it) {
        return Iterator(LinkListImpl::Erase(it.mIterator));
    }

    void PushBack(T* pElem) {
        Insert(GetEndIter(), pElem);
    }

    T& GetFront() {
        return *GetBeginIter();
    }
    const T& GetFront() const {
        return *GetBeginIter();
    }

    T& GetBack() {
        return *--GetEndIter();
    }
    const T& GetBack() const {
        return *--GetEndIter();
    }

    static Iterator GetIteratorFromPointer(T* pElem) {
        return GetIteratorFromPointer(GetNodeFromPointer(pElem));
    }

    static Iterator GetIteratorFromPointer(LinkListNode* pNode) {
        return Iterator(LinkListImpl::GetIteratorFromPointer(pNode));
    }

    static LinkListNode* GetNodeFromPointer(T* pElem) {
        return reinterpret_cast<LinkListNode*>(reinterpret_cast<char*>(pElem) +
                                               Ofs);
    }

    static T* GetPointerFromNode(LinkListNode* pNode) {
        return reinterpret_cast<T*>(reinterpret_cast<char*>(pNode) - Ofs);
    }

    static const T* GetPointerFromNode(const LinkListNode* pNode) {
        return reinterpret_cast<const T*>(reinterpret_cast<const char*>(pNode) -
                                          Ofs);
    }
};

} // namespace ut
} // namespace nw4r

/******************************************************************************
 *
 * Macros
 *
 ******************************************************************************/
/**
 * Declare typedef for linked-list specialization.
 */
#define NW4R_UT_LINKLIST_TYPEDEF_DECL(T)                                       \
    typedef nw4r::ut::LinkList<T, offsetof(T, node)> T##List;

/**
 * Declare typedef for linked-list specialization.
 *
 * Use the specified link node (name suffix) for classes with multiple nodes.
 */
#define NW4R_UT_LINKLIST_TYPEDEF_DECL_EX(T, SUFFIX)                            \
    typedef nw4r::ut::LinkList<T, offsetof(T, node##SUFFIX)> T##SUFFIX##List;

/**
 * Declare a member LinkListNode for use with the typedef.
 */
#define NW4R_UT_LINKLIST_NODE_DECL() nw4r::ut::LinkListNode node

/**
 * Declare a member LinkListNode for use with the typedef.
 *
 * Use the specified link node (name suffix) for classes with multiple nodes.
 */
#define NW4R_UT_LINKLIST_NODE_DECL_EX(SUFFIX)                                  \
    nw4r::ut::LinkListNode node##SUFFIX

/**
 * Explicitly instantiate a linked list specialization.
 * (RESERVED FOR MATCHING DECOMP HACKS)
 */
#if !defined(NONMATCHING)
#define NW4R_UT_LINKLIST_TYPEDEF_FORCE(T)                                      \
    template struct nw4r::ut::LinkList<T, offsetof(T, node)>
#else
#define NW4R_UT_LINKLIST_TYPEDEF_FORCE(T)
#endif

/**
 * Linked-list for-each macro.
 *
 * @param NAME Element name
 * @param LIST Reference to list
 * @param ... Statement(s) to execute
 */
#define NW4R_UT_LINKLIST_FOREACH(NAME, LIST, ...)                              \
    {                                                                          \
        typedef DECLTYPE((LIST).GetBeginIter()) IterType;                      \
                                                                               \
        for (IterType NAME = (LIST).GetBeginIter();                            \
             NAME != (LIST).GetEndIter(); ++NAME) {                            \
                                                                               \
            __VA_ARGS__;                                                       \
        }                                                                      \
    }

/**
 * List for-each macro, with robust iteration.
 *
 * @param NAME Element name
 * @param LIST Reference to list
 * @param ... Statement(s) to execute
 */
#define NW4R_UT_LINKLIST_FOREACH_SAFE(NAME, LIST, ...)                         \
    {                                                                          \
        typedef DECLTYPE((LIST).GetBeginIter()) IterType;                      \
                                                                               \
        for (IterType __impl__ = (LIST).GetBeginIter();                        \
             __impl__ != (LIST).GetEndIter();) {                               \
                                                                               \
            IterType NAME = __impl__++;                                        \
            __VA_ARGS__;                                                       \
        }                                                                      \
    }

#endif
/* end "nw4r/ut/ut_LinkList.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 12 "nw4r/ut/ut_LockedCache.h" */
#ifndef NW4R_UT_LOCKED_CACHE_H
#define NW4R_UT_LOCKED_CACHE_H
/* "libs/nw4r/include/nw4r/ut/ut_LockedCache.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_LockedCache.h" line 4 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */

namespace nw4r {
namespace ut {
namespace LC {

void Enable();
void Disable();

bool Lock();
void Unlock();

void LoadBlocks(void* pDst, void* pSrc, u32 blocks);
void StoreBlocks(void* pDst, void* pSrc, u32 blocks);
void StoreData(void* pDst, void* pSrc, u32 size);

inline void* GetBase() {
    return reinterpret_cast<void*>(OS_CACHE_BASE);
}

inline void QueueWait(u32 len) {
    LCQueueWait(len);
}

inline void QueueWaitEx(u32 len) {
    while (LCQueueLength() != len) {
        OSYieldThread();
    }
}

} // namespace LC
} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_LockedCache.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 13 "nw4r/ut/ut_NandFileStream.h" */
#ifndef NW4R_UT_NAND_FILE_STREAM_H
#define NW4R_UT_NAND_FILE_STREAM_H
/* "libs/nw4r/include/nw4r/ut/ut_NandFileStream.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_NandFileStream.h" line 4 "nw4r/ut/ut_FileStream.h" */
/* end "nw4r/ut/ut_FileStream.h" */

/* "libs/nw4r/include/nw4r/ut/ut_NandFileStream.h" line 6 "revolution/NAND.h" */
/**
 * References: WiiBrew
 */

#ifndef RVL_SDK_PUBLIC_NAND_H
#define RVL_SDK_PUBLIC_NAND_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/NAND.h" line 10 "revolution/NAND/NANDCheck.h" */
#ifndef RVL_SDK_NAND_CHECK_H
#define RVL_SDK_NAND_CHECK_H
/* "libs/RVL_SDK/include/revolution/NAND/NANDCheck.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/NAND/NANDCheck.h" line 4 "revolution/NAND/nand.h" */
#ifndef RVL_SDK_NAND_H
#define RVL_SDK_NAND_H
/* "libs/RVL_SDK/include/revolution/NAND/nand.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/NAND/nand.h" line 4 "revolution/FS.h" */
/**
 * References: WiiBrew
 */

#ifndef RVL_SDK_PUBLIC_FS_H
#define RVL_SDK_PUBLIC_FS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/FS.h" line 10 "revolution/FS/fs.h" */
#ifndef RVL_SDK_FS_H
#define RVL_SDK_FS_H
/* "libs/RVL_SDK/include/revolution/FS/fs.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/FS/fs.h" line 4 "revolution/IPC.h" */
/**
 * References: WiiBrew, Dolphin Emulator, fail0verflow
 */

#ifndef RVL_SDK_PUBLIC_IPC_H
#define RVL_SDK_PUBLIC_IPC_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/IPC.h" line 10 "revolution/IPC/ipcMain.h" */
#ifndef RVL_SDK_IPC_MAIN_H
#define RVL_SDK_IPC_MAIN_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcMain.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/IPC/ipcMain.h" line 4 "revolution/IPC/ipcHardware.h" */
#ifndef RVL_SDK_IPC_HARDWARE_H
#define RVL_SDK_IPC_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcHardware.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * IPC hardware registers
 */
volatile u32 DECL_HW_REGS(IPC_PPC) DECL_ADDRESS(0xCD000000);
volatile u32 DECL_HW_REGS(IPC) DECL_ADDRESS(0xCD800000);

/**
 * Hardware register indexes
 */
// clang-format off
#define LIST_OF_REGS                                                           \
    X(IPC_PPCMSG,      0xCD000000)                                             \
    X(IPC_PPCCTRL,     0xCD000004)                                             \
    X(IPC_ARMMSG,      0xCD000008)                                             \
    X(IPC_ARMCTRL,     0xCD00000C)                                             \
    X(TIMER,           0xCD000010)                                             \
    X(ALARM,           0xCD000014)                                             \
    X(PPCIRQFLAG,      0xCD000030)                                             \
    X(PPCIRQMASK,      0xCD000034)                                             \
    X(ARMIRQFLAG,      0xCD000038)                                             \
    X(ARMIRQMASK,      0xCD00003C)                                             \
    X(MEMMIRR,         0xCD000060)                                             \
    X(AHBPROT,         0xCD000064)                                             \
    X(EXICTRL,         0xCD000070)                                             \
    X(GPIO1BOUT,       0xCD0000C0)                                             \
    X(GPIO1BDIR,       0xCD0000C4)                                             \
    X(GPIO1BIN,        0xCD0000C8)                                             \
    X(GPIO1BINTLVL,    0xCD0000CC)                                             \
    X(GPIO1BINTFLAG,   0xCD0000D0)                                             \
    X(GPIO1BINTENABLE, 0xCD0000D4)                                             \
    X(GPIO1BINMIR,     0xCD0000D8)                                             \
    X(GPIO1ENABLE,     0xCD0000DC)                                             \
    X(GPIO1OUT,        0xCD0000E0)                                             \
    X(GPIO1DIR,        0xCD0000E4)                                             \
    X(GPIO1IN,         0xCD0000E8)                                             \
    X(GPIO1INTLVL,     0xCD0000EC)                                             \
    X(GPIO1INTFLAG,    0xCD0000F0)                                             \
    X(GPIO1INTENABLE,  0xCD0000F4)                                             \
    X(GPIO1INMIR,      0xCD0000F8)                                             \
    X(GPIO1OWNER,      0xCD0000FC)                                             \
    X(DIFLAGS,         0xCD000180)                                             \
    X(RESETS,          0xCD000194)                                             \
    X(CLOCKS,          0xCD0001B4)                                             \
    X(GPIO2OUT,        0xCD0001C8)                                             \
    X(GPIO2DIR,        0xCD0001CC)                                             \
    X(GPIO2IN,         0xCD0001D0)                                             \
    X(OTPCMD,          0xCD0001EC)                                             \
    X(OTPDATA,         0xCD0001F0)                                             \
    X(VERSION,         0xCD000214)
// clang-format on

/**
 * Hardware register indexes (IPC)
 */
#define X(NAME, ADDR) IPC_##NAME = (ADDR - 0xCD000000) / 4,
typedef enum { LIST_OF_REGS } IPCHwReg;
#undef X

/**
 * Hardware register indexes (ACR)
 */
#define X(NAME, ADDR) ACR_##NAME = (ADDR - 0xCD000000),
typedef enum { LIST_OF_REGS } ACRHwReg;
#undef X

/**
 * GPIO register flags
 */
typedef enum {
    GPIO_POWER = (1 << 0),
    GPIO_SHUTDOWN = (1 << 1),
    GPIO_FAN = (1 << 2),
    GPIO_DCDC = (1 << 3),
    GPIO_DISPIN = (1 << 4),
    GPIO_SLOTLED = (1 << 5),
    GPIO_EJECTBTN = (1 << 6),
    GPIO_SLOTIN = (1 << 7),
    GPIO_SENSORBAR = (1 << 8),
    GPIO_DOEJECT = (1 << 9),
    GPIO_EEP_CS = (1 << 10),
    GPIO_EEP_CLK = (1 << 11),
    GPIO_EEP_MOSI = (1 << 12),
    GPIO_EEP_MISO = (1 << 13),
    GPIO_AVE_SCL = (1 << 14),
    GPIO_AVE_SDA = (1 << 15),
    GPIO_DEBUG0 = (1 << 16),
    GPIO_DEBUG1 = (1 << 17),
    GPIO_DEBUG2 = (1 << 18),
    GPIO_DEBUG3 = (1 << 19),
    GPIO_DEBUG4 = (1 << 20),
    GPIO_DEBUG5 = (1 << 21),
    GPIO_DEBUG6 = (1 << 22),
    GPIO_DEBUG7 = (1 << 23),
} GPIOFlag;

#undef LIST_OF_REGS

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcHardware.h" */
#ifdef __cplusplus
extern "C" {
#endif

inline u32 ACRReadReg(u32 reg) {
    return IPC_PPC_HW_REGS[reg / 4];
}

inline void ACRWriteReg(u32 reg, u32 val) {
    IPC_PPC_HW_REGS[reg / 4] = val;
}

void IPCInit(void);
void IPCReInit(void);
u32 IPCReadReg(s32 index);
void IPCWriteReg(s32 index, u32 value);
void* IPCGetBufferHi(void);
void* IPCGetBufferLo(void);
void IPCSetBufferLo(void* lo);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcMain.h" */
/* "libs/RVL_SDK/include/revolution/IPC.h" line 11 "revolution/IPC/ipcProfile.h" */
#ifndef RVL_SDK_IPC_PROFILE_H
#define RVL_SDK_IPC_PROFILE_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcProfile.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/IPC/ipcProfile.h" line 4 "revolution/IPC/ipcclt.h" */
#ifndef RVL_SDK_IPC_CLT_H
#define RVL_SDK_IPC_CLT_H
/* "libs/RVL_SDK/include/revolution/IPC/ipcclt.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/IPC/ipcclt.h" line 4 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IPC_RESULT_FATAL_ERROR = -119,
    IPC_RESULT_BUSY,
    IPC_RESULT_NOTEMPTY = -115,
    IPC_RESULT_ECC_CRIT,
    IPC_RESULT_OPENFD = -111,
    IPC_RESULT_MAXFD = -109,
    IPC_RESULT_MAXBLOCKS,
    IPC_RESULT_MAXFILES,
    IPC_RESULT_NOEXISTS,
    IPC_RESULT_EXISTS,
    IPC_RESULT_CORRUPT = -103,
    IPC_RESULT_ACCESS,
    IPC_RESULT_INVALID,

    IPC_RESULT_ALLOC_FAILED = -22,
    IPC_RESULT_ECC_CRIT_INTERNAL = -12,
    IPC_RESULT_BUSY_INTERNAL = -8,
    IPC_RESULT_NOEXISTS_INTERNAL = -6,
    IPC_RESULT_CONN_MAX_INTERNAL = -5,
    IPC_RESULT_INVALID_INTERNAL = -4,
    IPC_RESULT_EXISTS_INTERNAL = -2,
    IPC_RESULT_ACCESS_INTERNAL = -1,

    IPC_RESULT_OK = 0
} IPCResult;

typedef enum {
    IPC_REQ_NONE,
    IPC_REQ_OPEN,
    IPC_REQ_CLOSE,
    IPC_REQ_READ,
    IPC_REQ_WRITE,
    IPC_REQ_SEEK,
    IPC_REQ_IOCTL,
    IPC_REQ_IOCTLV
} IPCRequestType;

typedef enum {
    // Ioctl
    IPC_IOCTL_GET_NAND_STATS = 2,
    IPC_IOCTL_CREATE_DIR = 3,
    IPC_IOCTL_READ_DIR = 4,
    IPC_IOCTL_GET_ATTR = 6,
    IPC_IOCTL_DELETE_PATH = 7,
    IPC_IOCTL_RENAME_PATH = 8,
    IPC_IOCTL_CREATE_FILE = 9,
    IPC_IOCTL_GET_FILE_STATS = 11,
    IPC_IOCTL_SHUTDOWN_FS = 13,
    IPC_IOCTL_REG_STM_EVENT = 0x1000,
    IPC_IOCTL_HOT_RESET = 0x2001,
    IPC_IOCTL_SHUTDOWN_TO_SBY = 0x2003,
    IPC_IOCTL_UNREG_STM_EVENT = 0x3002,
    IPC_IOCTL_SET_VI_DIM = 0x5001,
    IPC_IOCTL_SET_IDLE_LED_MODE = 0x6002,

    // Ioctlv
    IPC_IOCTLV_LAUNCH_TITLE = 8,
    IPC_IOCTLV_GET_USAGE = 12,
    IPC_IOCTLV_GET_NUM_TICKET_VIEWS = 18,
    IPC_IOCTLV_GET_TICKET_VIEWS = 19,
    IPC_IOCTLV_GET_DATA_DIR = 29,
    IPC_IOCTLV_GET_TITLE_ID = 32,
} IPCIoctlType;

typedef enum {
    IPC_OPEN_NONE = 0,
    IPC_OPEN_READ = (1 << 0),
    IPC_OPEN_WRITE = (1 << 1),
    IPC_OPEN_RW = IPC_OPEN_READ | IPC_OPEN_WRITE
} IPCOpenMode;

typedef enum {
    IPC_SEEK_BEG,
    IPC_SEEK_CUR,
    IPC_SEEK_END,
} IPCSeekMode;

typedef s32 (*IPCAsyncCallback)(s32 result, void* arg);

typedef struct IPCIOVector {
    void* base; // at 0x0
    u32 length; // at 0x4
} IPCIOVector;

typedef struct IPCOpenArgs {
    const char* path; // at 0x0
    IPCOpenMode mode; // at 0x4
} IPCOpenArgs;

typedef struct IPCReadWriteArgs {
    void* data; // at 0x0
    u32 length; // at 0x4
} IPCReadWriteArgs;

typedef struct IPCSeekArgs {
    s32 offset;       // at 0x0
    IPCSeekMode mode; // at 0x4
} IPCSeekArgs;

typedef struct IPCIoctlArgs {
    s32 type;    // at 0x0
    void* in;    // at 0x4
    s32 inSize;  // at 0x8
    void* out;   // at 0xC
    s32 outSize; // at 0x10
} IPCIoctlArgs;

typedef struct IPCIoctlvArgs {
    s32 type;             // at 0x0
    u32 inCount;          // at 0x4
    u32 outCount;         // at 0x8
    IPCIOVector* vectors; // at 0xC
} IPCIoctlvArgs;

typedef struct IPCRequest {
    IPCRequestType type; // at 0x0
    s32 ret;             // at 0x4
    s32 fd;              // at 0x8
    union {
        IPCOpenArgs open;
        IPCReadWriteArgs rw;
        IPCSeekArgs seek;
        IPCIoctlArgs ioctl;
        IPCIoctlvArgs ioctlv;
    }; // at 0xC
} IPCRequest;

typedef struct IPCRequestEx {
    IPCRequest base;           // at 0x0
    IPCAsyncCallback callback; // at 0x20
    void* callbackArg;         // at 0x24
    BOOL reboot;               // at 0x28
    OSThreadQueue queue;       // at 0x2C
    char padding[64 - 0x34];
} IPCRequestEx;

s32 IPCCltInit(void);
s32 IPCCltReInit(void);
s32 IOS_OpenAsync(const char* path, IPCOpenMode mode, IPCAsyncCallback callback,
                  void* callbackArg);
s32 IOS_Open(const char* path, IPCOpenMode mode);
s32 IOS_CloseAsync(s32 fd, IPCAsyncCallback callback, void* callbackArg);
s32 IOS_Close(s32 fd);
s32 IOS_ReadAsync(s32 fd, void* buf, s32 len, IPCAsyncCallback callback,
                  void* callbackArg);
s32 IOS_Read(s32 fd, void* buf, s32 len);
s32 IOS_WriteAsync(s32 fd, const void* buf, s32 len, IPCAsyncCallback callback,
                   void* callbackArg);
s32 IOS_Write(s32 fd, const void* buf, s32 len);
s32 IOS_SeekAsync(s32 fd, s32 offset, IPCSeekMode mode,
                  IPCAsyncCallback callback, void* callbackArg);
s32 IOS_Seek(s32 fd, s32 offset, IPCSeekMode mode);
s32 IOS_IoctlAsync(s32 fd, s32 type, void* in, s32 inSize, void* out,
                   s32 outSize, IPCAsyncCallback callback, void* callbackArg);
s32 IOS_Ioctl(s32 fd, s32 type, void* in, s32 inSize, void* out, s32 outSize);
s32 IOS_IoctlvAsync(s32 fd, s32 type, s32 inCount, s32 outCount,
                    IPCIOVector* vectors, IPCAsyncCallback callback,
                    void* callbackArg);
s32 IOS_Ioctlv(s32 fd, s32 type, s32 inCount, s32 outCount,
               IPCIOVector* vectors);
s32 IOS_IoctlvReboot(s32 fd, s32 type, s32 inCount, s32 outCount,
                     IPCIOVector* vectors);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcclt.h" */
#ifdef __cplusplus
extern "C" {
#endif

void IPCiProfInit(void);
void IPCiProfQueueReq(IPCRequestEx* req, s32 fd);
void IPCiProfAck(void);
void IPCiProfReply(IPCRequestEx* req, s32 fd);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/ipcProfile.h" */
/* "libs/RVL_SDK/include/revolution/IPC.h" line 12 "revolution/IPC/ipcclt.h" */
/* end "revolution/IPC/ipcclt.h" */
/* "libs/RVL_SDK/include/revolution/IPC.h" line 13 "revolution/IPC/memory.h" */
#ifndef RVL_SDK_IPC_MEMORY_H
#define RVL_SDK_IPC_MEMORY_H
/* "libs/RVL_SDK/include/revolution/IPC/memory.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

s32 iosCreateHeap(void* base, u32 size);
void* iosAllocAligned(s32 handle, u32 size, u32 align);
s32 iosFree(s32 handle, void* block);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC/memory.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/IPC.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define FS_MAX_PATH 64

typedef void (*FSAsyncCallback)(s32 result, void* arg);

typedef struct FSStats {
    char UNK_0x0[0x1C];
} FSStats;

// Could be more fields, but not larger than 32B
typedef struct FSFileStats {
    u32 length;   // at 0x0
    u32 position; // at 0x4
} FSFileStats ALIGN(32);

s32 ISFS_OpenLib(void);
s32 ISFS_CreateDir(const char* path, u32 attr, u32 ownerPerm, u32 groupPerm,
                   u32 otherPerm);
s32 ISFS_CreateDirAsync(const char* path, u32 attr, u32 ownerPerm,
                        u32 groupPerm, u32 otherPerm, FSAsyncCallback callback,
                        void* callbackArg);
s32 ISFS_ReadDir(const char* path, char* filesOut, u32* fileCountOut);
s32 ISFS_ReadDirAsync(const char* path, char* filesOut, u32* fileCountOut,
                      FSAsyncCallback callback, void* callbackArg);
s32 ISFS_GetAttr(const char* path, u32* ownerIdOut, u16* groupIdOut,
                 u32* attrOut, u32* ownerPermOut, u32* groupPermOut,
                 u32* otherPermOut);
s32 ISFS_GetAttrAsync(const char* path, u32* ownerIdOut, u16* groupIdOut,
                      u32* attrOut, u32* ownerPermOut, u32* groupPermOut,
                      u32* otherPermOut, FSAsyncCallback callback,
                      void* callbackArg);
s32 ISFS_Delete(const char* path);
s32 ISFS_DeleteAsync(const char* path, FSAsyncCallback callback,
                     void* callbackArg);
s32 ISFS_Rename(const char* from, const char* to);
s32 ISFS_RenameAsync(const char* from, const char* to, FSAsyncCallback callback,
                     void* callbackArg);
s32 ISFS_GetUsage(const char* path, s32* blockCountOut, s32* fileCountOut);
s32 ISFS_CreateFile(const char* path, u32 attr, u32 ownerPerm, u32 groupPerm,
                    u32 otherPerm);
s32 ISFS_CreateFileAsync(const char* path, u32 attr, u32 ownerPerm,
                         u32 groupPerm, u32 otherPerm, FSAsyncCallback callback,
                         void* callbackArg);
s32 ISFS_Open(const char* path, IPCOpenMode mode);
s32 ISFS_OpenAsync(const char* path, IPCOpenMode mode, FSAsyncCallback callback,
                   void* callbackArg);
s32 ISFS_GetFileStats(s32 fd, FSFileStats* statsOut);
s32 ISFS_GetFileStatsAsync(s32 fd, FSFileStats* statsOut,
                           FSAsyncCallback callback, void* callbackArg);
s32 ISFS_Seek(s32 fd, s32 offset, IPCSeekMode mode);
s32 ISFS_SeekAsync(s32 fd, s32 offset, IPCSeekMode mode,
                   FSAsyncCallback callback, void* callbackArg);
s32 ISFS_Read(s32 fd, void* dst, s32 len);
s32 ISFS_ReadAsync(s32 fd, void* dst, s32 len, FSAsyncCallback callback,
                   void* callbackArg);
s32 ISFS_Write(s32 fd, const void* src, s32 len);
s32 ISFS_WriteAsync(s32 fd, const void* src, s32 len, FSAsyncCallback callback,
                    void* callbackArg);
s32 ISFS_Close(s32 fd);
s32 ISFS_CloseAsync(s32 fd, FSAsyncCallback callback, void* callbackArg);
s32 ISFS_ShutdownAsync(FSAsyncCallback callback, void* callbackArg);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/FS/fs.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/FS.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define NAND_BANNER_TITLE_MAX 32
#define NAND_BANNER_ICON_MAX_FRAME 8

// Forward declarations
struct NANDCommandBlock;

typedef enum {
    NAND_RESULT_FATAL_ERROR = -128,
    NAND_RESULT_UNKNOWN = -64,

    NAND_RESULT_MAXDEPTH = -16,
    NAND_RESULT_AUTHENTICATION,
    NAND_RESULT_OPENFD,
    NAND_RESULT_NOTEMPTY,
    NAND_RESULT_NOEXISTS,
    NAND_RESULT_MAXFILES,
    NAND_RESULT_MAXFD,
    NAND_RESULT_MAXBLOCKS,
    NAND_RESULT_INVALID,

    NAND_RESULT_EXISTS = -6,
    NAND_RESULT_ECC_CRIT,
    NAND_RESULT_CORRUPT,
    NAND_RESULT_BUSY,
    NAND_RESULT_ALLOC_FAILED,
    NAND_RESULT_ACCESS,

    NAND_RESULT_OK,
} NANDResult;

typedef enum {
    NAND_SEEK_BEG,
    NAND_SEEK_CUR,
    NAND_SEEK_END,
} NANDSeekMode;

typedef enum {
    NAND_ACCESS_NONE,
    NAND_ACCESS_READ,
    NAND_ACCESS_WRITE,
    NAND_ACCESS_RW
} NANDAccessType;

typedef enum {
    NAND_FILE_TYPE_NONE,
    NAND_FILE_TYPE_FILE,
    NAND_FILE_TYPE_DIR,
} NANDFileType;

typedef enum {
    // Read/write by owner
    NAND_PERM_RUSR = (NAND_ACCESS_READ << 4),
    NAND_PERM_WUSR = (NAND_ACCESS_WRITE << 4),
    // Read/write by group
    NAND_PERM_RGRP = (NAND_ACCESS_READ << 2),
    NAND_PERM_WGRP = (NAND_ACCESS_WRITE << 2),
    // Read/write by other
    NAND_PERM_ROTH = (NAND_ACCESS_READ << 0),
    NAND_PERM_WOTH = (NAND_ACCESS_WRITE << 0),
    // Read/write by all
    NAND_PERM_RALL = NAND_PERM_RUSR | NAND_PERM_RGRP | NAND_PERM_ROTH,
    NAND_PERM_WALL = NAND_PERM_WUSR | NAND_PERM_WGRP | NAND_PERM_WOTH,
    NAND_PERM_RWALL = NAND_PERM_RALL | NAND_PERM_WALL
} NANDPermission;

typedef void (*NANDAsyncCallback)(s32 result, struct NANDCommandBlock* block);

typedef struct NANDStatus {
    u32 ownerId; // at 0x0
    u16 groupId; // at 0x4
    u8 attr;     // at 0x6
    u8 perm;     // at 0x7
} NANDStatus;

typedef struct NANDFileInfo {
    s32 fd;                     // at 0x0
    s32 tempFd;                 // at 0x4
    char openPath[FS_MAX_PATH]; // at 0x8
    char tempPath[FS_MAX_PATH]; // at 0x48
    u8 access;                  // at 0x88
    u8 stage;                   // at 0x89
    u8 mark;                    // at 0x8A
} NANDFileInfo;

typedef struct NANDCommandBlock {
    void* userData;             // at 0x0
    NANDAsyncCallback callback; // at 0x4
    NANDFileInfo* info;         // at 0x8
    void* bytes;                // at 0xC
    void* inodes;               // at 0x10
    NANDStatus* status;         // at 0x14
    u32 ownerId;                // at 0x18
    u16 groupId;                // at 0x1C
    u8 nextStage;               // at 0x1E
    u32 attr;                   // at 0x20
    u32 ownerPerm;              // at 0x24
    u32 groupPerm;              // at 0x28
    u32 otherPerm;              // at 0x2C
    u32 dirFileCount;           // at 0x30
    char path[FS_MAX_PATH];     // at 0x34
    u32* length;                // at 0x74
    u32* position;              // at 0x78
    s32 state;                  // at 0x7C
    void* buffer;               // at 0x80
    u32 bufferSize;             // at 0x84
    u8* type;                   // at 0x88
    u32 uniqueNo;               // at 0x8C
    u32 reqBlocks;              // at 0x90
    u32 reqInodes;              // at 0x94
    u32* answer;                // at 0x98
    u32 homeBlocks;             // at 0x9C
    u32 homeInodes;             // at 0xA0
    u32 userBlocks;             // at 0xA4
    u32 userInodes;             // at 0xA8
    u32 workBlocks;             // at 0xAC
    u32 workInodes;             // at 0xB0
    const char** dir;           // at 0xB4
    u32 unkB8; //TODO: necessary so that OSPlayRecord's bss is correct. might be fake?
} NANDCommandBlock;

typedef struct NANDBanner {
    u32 magic;                                          // at 0x0
    u32 flags;                                          // at 0x4
    u16 iconSpeed;                                      // at 0x8
    u8 reserved[0x20 - 0xA];                            // at 0xA
    wchar_t title[NAND_BANNER_TITLE_MAX];               // at 0x20
    wchar_t subtitle[NAND_BANNER_TITLE_MAX];            // at 0x60
    u8 bannerTexture[0x6000];                           // at 0xA0
    u8 iconTexture[0x1200][NAND_BANNER_ICON_MAX_FRAME]; // at 0x60A0
} NANDBanner;

s32 NANDCreate(const char* path, u8 perm, u8 attr);
s32 NANDPrivateCreate(const char* path, u8 perm, u8 attr);
s32 NANDPrivateCreateAsync(const char* path, u8 perm, u8 attr,
                           NANDAsyncCallback callback, NANDCommandBlock* block);

s32 NANDDelete(const char* path);
s32 NANDPrivateDelete(const char* path);
s32 NANDPrivateDeleteAsync(const char* path, NANDAsyncCallback callback,
                           NANDCommandBlock* block);

s32 NANDRead(NANDFileInfo* info, void* buf, u32 len);
s32 NANDReadAsync(NANDFileInfo* info, void* buf, u32 len,
                  NANDAsyncCallback callback, NANDCommandBlock* block);

s32 NANDWrite(NANDFileInfo* info, const void* buf, u32 len);
s32 NANDWriteAsync(NANDFileInfo* info, const void* buf, u32 len,
                   NANDAsyncCallback callback, NANDCommandBlock* block);

s32 NANDSeek(NANDFileInfo* info, s32 offset, NANDSeekMode whence);
s32 NANDSeekAsync(NANDFileInfo* info, s32 offset, NANDSeekMode whence,
                  NANDAsyncCallback callback, NANDCommandBlock* block);

s32 NANDPrivateCreateDir(const char* path, u8 perm, u8 attr);
s32 NANDPrivateCreateDirAsync(const char* path, u8 perm, u8 attr,
                              NANDAsyncCallback callback,
                              NANDCommandBlock* block);

s32 NANDMove(const char* from, const char* to);

s32 NANDGetLength(NANDFileInfo* info, u32* length);
s32 NANDGetLengthAsync(NANDFileInfo* info, u32* lengthOut,
                       NANDAsyncCallback callback, NANDCommandBlock* block);

s32 NANDGetStatus(const char* path, NANDStatus* status);
s32 NANDPrivateGetStatusAsync(const char* path, NANDStatus* status,
                              NANDAsyncCallback callback,
                              NANDCommandBlock* block);

void NANDSetUserData(NANDCommandBlock* block, void* data);
void* NANDGetUserData(NANDCommandBlock* block);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/NAND/nand.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NAND_CHECK_TOO_MANY_APP_BLOCKS = (1 << 0),
    NAND_CHECK_TOO_MANY_APP_FILES = (1 << 1),
    NAND_CHECK_TOO_MANY_USER_BLOCKS = (1 << 2),
    NAND_CHECK_TOO_MANY_USER_FILES = (1 << 3),
} NANDCheckFlags;

s32 NANDCheck(u32 neededBlocks, u32 neededFiles, u32* answer);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/NAND/NANDCheck.h" */
/* "libs/RVL_SDK/include/revolution/NAND.h" line 11 "revolution/NAND/NANDCore.h" */
#ifndef RVL_SDK_NAND_CORE_H
#define RVL_SDK_NAND_CORE_H
/* "libs/RVL_SDK/include/revolution/NAND/NANDCore.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/NAND/NANDCore.h" line 4 "revolution/NAND/nand.h" */
/* end "revolution/NAND/nand.h" */
#ifdef __cplusplus
extern "C" {
#endif

void nandRemoveTailToken(char* newp, const char* oldp);
void nandGetHeadToken(char* head, char* rest, const char* path);
void nandGetRelativeName(char* name, const char* path);
void nandConvertPath(char* abs, const char* dir, const char* rel);
BOOL nandIsRelativePath(const char* path);
BOOL nandIsPrivatePath(const char* path);
BOOL nandIsUnderPrivatePath(const char* path);
BOOL nandIsInitialized(void);
void nandReportErrorCode(s32 result) DECOMP_DONT_INLINE;
s32 nandConvertErrorCode(s32 result);
void nandGenerateAbsPath(char* abs, const char* rel);
void nandGetParentDirectory(char* dir, const char* path);
s32 NANDInit(void);
s32 NANDGetCurrentDir(char* out);
s32 NANDGetHomeDir(char* out);
void nandCallback(s32 result, void* arg);
s32 NANDGetType(const char* path, u8* type);
s32 NANDPrivateGetTypeAsync(const char* path, u8* type,
                            NANDAsyncCallback callback,
                            NANDCommandBlock* block);
const char* nandGetHomeDir(void);
void NANDInitBanner(NANDBanner* banner, u32 flags, const wchar_t* title,
                    const wchar_t* subtitle);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/NAND/NANDCore.h" */
/* "libs/RVL_SDK/include/revolution/NAND.h" line 12 "revolution/NAND/NANDOpenClose.h" */
#ifndef RVL_SDK_NAND_OPEN_CLOSE_H
#define RVL_SDK_NAND_OPEN_CLOSE_H
/* "libs/RVL_SDK/include/revolution/NAND/NANDOpenClose.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/NAND/NANDOpenClose.h" line 4 "revolution/NAND/nand.h" */
/* end "revolution/NAND/nand.h" */
#ifdef __cplusplus
extern "C" {
#endif

s32 NANDOpen(const char* path, NANDFileInfo* info, u8 mode);
s32 NANDPrivateOpen(const char* path, NANDFileInfo* info, u8 mode);
s32 NANDOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                  NANDAsyncCallback callback, NANDCommandBlock* block);
s32 NANDPrivateOpenAsync(const char* path, NANDFileInfo* info, u8 mode,
                         NANDAsyncCallback callback, NANDCommandBlock* block);

s32 NANDClose(NANDFileInfo* info);
s32 NANDCloseAsync(NANDFileInfo* info, NANDAsyncCallback callback,
                   NANDCommandBlock* block);

s32 NANDPrivateSafeOpenAsync(const char* path, NANDFileInfo* info, u8 access,
                             void* buffer, u32 bufferSize,
                             NANDAsyncCallback callback,
                             NANDCommandBlock* block);
s32 NANDSafeCloseAsync(NANDFileInfo* info, NANDAsyncCallback callback,
                       NANDCommandBlock* block);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/NAND/NANDOpenClose.h" */
/* "libs/RVL_SDK/include/revolution/NAND.h" line 13 "revolution/NAND/nand.h" */
/* end "revolution/NAND/nand.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/NAND.h" */

namespace nw4r {
namespace ut {

class NandFileStream : public FileStream {
public:
    NW4R_UT_RTTI_DECL(NandFileStream);

public:
    NandFileStream(const char* pPath, u32 mode);
    NandFileStream(const NANDFileInfo* pInfo, u32 mode, bool enableClose);
    virtual ~NandFileStream(); // at 0xC

    bool Open(const char* pPath, u32 mode);
    bool Open(const NANDFileInfo* pInfo, u32 mode, bool enableClose)
        DECOMP_DONT_INLINE;

    virtual void Close(); // at 0x10

    virtual s32 Read(void* pDst, u32 size); // at 0x14
    virtual bool ReadAsync(void* pDst, u32 size, StreamCallback pCallback,
                           void* pCallbackArg); // at 0x18

    virtual bool Write(const void* pSrc, u32 size); // at 0x1C
    virtual bool WriteAsync(const void* pSrc, u32 size,
                            StreamCallback pCallback,
                            void* pCallbackArg); // at 0x20

    virtual void Seek(s32 offset, u32 origin); // at 0x44

    virtual bool IsBusy() const {
        return mIsBusy;
    } // at 0x24

    virtual u32 Tell() const {
        return mFilePosition.Tell();
    } // at 0x58
    virtual u32 GetSize() const {
        return mFilePosition.GetFileSize();
    } // at 0x40

    virtual bool CanAsync() const {
        return true;
    } // at 0x28
    virtual bool CanSeek() const {
        return true;
    } // at 0x50
    virtual bool CanRead() const {
        return mCanRead;
    } // at 0x2C
    virtual bool CanWrite() const {
        return mCanWrite;
    } // at 0x30
    virtual bool CanCancel() const {
        return false;
    } // at 0x54

    virtual u32 GetOffsetAlign() const {
        return 1;
    } // at 0x34
    virtual u32 GetSizeAlign() const {
        return 32;
    } // at 0x38
    virtual u32 GetBufferAlign() const {
        return 32;
    } // at 0x3C

private:
    struct NandFileStreamInfo {
        NANDCommandBlock nandBlock; // at 0x0
        NANDFileInfo nandInfo;      // at 0xB8
        NandFileStream* stream;     // at 0x144
    };

private:
    static void NandAsyncCallback_(s32 result, NANDCommandBlock* pBlock);

    void Initialize_();

private:
    FilePosition mFilePosition;   // at 0x14
    NandFileStreamInfo mFileInfo; // at 0x1C
    bool mCanRead;                // at 0x164
    bool mCanWrite;               // at 0x165
    volatile bool mIsBusy;        // at 0x166
    bool mCloseOnDestroyFlg;      // at 0x167
    bool mCloseEnableFlg;         // at 0x168
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_NandFileStream.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 14 "nw4r/ut/ut_NonCopyable.h" */
/* end "nw4r/ut/ut_NonCopyable.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 15 "nw4r/ut/ut_Rect.h" */
#ifndef NW4R_UT_RECT_H
#define NW4R_UT_RECT_H
/* "libs/nw4r/include/nw4r/ut/ut_Rect.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_Rect.h" line 4 "nw4r/math.h" */
/* end "nw4r/math.h" */

namespace nw4r {
namespace ut {

struct Rect {
    f32 left;   // at 0x0
    f32 top;    // at 0x4
    f32 right;  // at 0x8
    f32 bottom; // at 0xC

    Rect() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
    Rect(f32 l, f32 t, f32 r, f32 b) : left(l), top(t), right(r), bottom(b) {}
    ~Rect() {}

    void SetWidth(f32 width) {
        right = left + width;
    }
    f32 GetWidth() const {
        return right - left;
    }

    void SetHeight(f32 height) {
        bottom = top + height;
    }
    f32 GetHeight() const {
        return bottom - top;
    }

    void Normalize() {
        f32 l = left;
        f32 t = top;
        f32 r = right;
        f32 b = bottom;

        left = math::FSelect(r - l, l, r);   // min(r, l)
        right = math::FSelect(r - l, r, l);  // max(r, l)
        top = math::FSelect(b - t, t, b);    // min(b, t)
        bottom = math::FSelect(b - t, b, t); // max(b, t)
    }

    void MoveTo(f32 x, f32 y) {
        right = x + GetWidth();
        left = x;
        bottom = y + GetHeight();
        top = y;
    }
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_Rect.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 16 "nw4r/ut/ut_ResFont.h" */
#ifndef NW4R_UT_RES_FONT_H
#define NW4R_UT_RES_FONT_H
/* "libs/nw4r/include/nw4r/ut/ut_ResFont.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_ResFont.h" line 4 "nw4r/ut/ut_ResFontBase.h" */
#ifndef NW4R_UT_RES_FONT_BASE_H
#define NW4R_UT_RES_FONT_BASE_H
/* "libs/nw4r/include/nw4r/ut/ut_ResFontBase.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_ResFontBase.h" line 4 "nw4r/ut/ut_Font.h" */
/* end "nw4r/ut/ut_Font.h" */

namespace nw4r {
namespace ut {

enum FontMapMethod {
    FONT_MAPMETHOD_DIRECT,
    FONT_MAPMETHOD_TABLE,
    FONT_MAPMETHOD_SCAN
};

struct FontTextureGlyph {
    u8 cellWidth;    // at 0x0
    u8 cellHeight;   // at 0x1
    s8 baselinePos;  // at 0x2
    u8 maxCharWidth; // at 0x3
    u32 sheetSize;   // at 0x4
    u16 sheetNum;    // at 0x8
    u16 sheetFormat; // at 0xA
    u16 sheetRow;    // at 0xC
    u16 sheetLine;   // at 0xE
    u16 sheetWidth;  // at 0x10
    u16 sheetHeight; // at 0x12
    u8* sheetImage;  // at 0x14
};

struct FontWidth {
    u16 indexBegin;          // at 0x0
    u16 indexEnd;            // at 0x2
    FontWidth* pNext;        // at 0x4
    CharWidths widthTable[]; // at 0x8
};

struct FontCodeMap {
    u16 ccodeBegin;     // at 0x0
    u16 ccodeEnd;       // at 0x2
    u16 mappingMethod;  // at 0x4
    u16 reserved;       // at 0x6
    FontCodeMap* pNext; // at 0x8
    u16 mapInfo[];      // at 0xC
};

struct FontInformation {
    u8 fontType;              // at 0x0
    s8 linefeed;              // at 0x1
    u16 alterCharIndex;       // at 0x2
    CharWidths defaultWidth;  // at 0x4
    u8 encoding;              // at 0x7
    FontTextureGlyph* pGlyph; // at 0x8
    FontWidth* pWidth;        // at 0xC
    FontCodeMap* pMap;        // at 0x10
    u8 height;                // at 0x14
    u8 width;                 // at 0x15
    u8 ascent;                // at 0x16
};

namespace detail {

/******************************************************************************
 *
 * ResFontBase
 *
 ******************************************************************************/
class ResFontBase : public Font {
public:
    static const u16 GLYPH_INDEX_NOT_FOUND = 0xFFFF;

public:
    ResFontBase();
    virtual ~ResFontBase(); // at 0x8

    virtual int GetWidth() const;  // at 0xC
    virtual int GetHeight() const; // at 0x10

    virtual int GetAscent() const;      // at 0x14
    virtual int GetDescent() const;     // at 0x18
    virtual int GetBaselinePos() const; // at 0x1C

    virtual int GetCellHeight() const;   // at 0x20
    virtual int GetCellWidth() const;    // at 0x24
    virtual int GetMaxCharWidth() const; // at 0x28

    virtual Type GetType() const;              // at 0x2C
    virtual GXTexFmt GetTextureFormat() const; // at 0x30
    virtual int GetLineFeed() const;           // at 0x34

    virtual CharWidths GetDefaultCharWidths() const;              // at 0x38
    virtual void SetDefaultCharWidths(const CharWidths& rWidths); // at 0x3C

    virtual bool SetAlternateChar(u16 ch); // at 0x40
    virtual void SetLineFeed(int lf);      // at 0x44

    virtual int GetCharWidth(u16 ch) const;             // at 0x48
    virtual CharWidths GetCharWidths(u16 ch) const;     // at 0x4C
    virtual void GetGlyph(Glyph* pGlyph, u16 ch) const; // at 0x50
    virtual bool HasGlyph(u16 ch) const;                // at 0x54
    virtual FontEncoding GetEncoding() const;           // at 0x58

protected:
    bool IsManaging(const void* pBuffer) const {
        return mResource == pBuffer;
    }

    void SetResourceBuffer(void* pBuffer, FontInformation* pInfo);
    void* RemoveResourceBuffer();

private:
    u16 GetGlyphIndex(u16 ch) const;

    u16 FindGlyphIndex(u16 ch) const;
    u16 FindGlyphIndex(const FontCodeMap* pMap, u16 ch) const;

    const CharWidths& GetCharWidthsFromIndex(u16 index) const;
    const CharWidths& GetCharWidthsFromIndex(const FontWidth* pWidth,
                                             u16 index) const;

    void GetGlyphFromIndex(Glyph* pGlyph, u16 index) const;

private:
    void* mResource;            // at 0x10
    FontInformation* mFontInfo; // at 0x14
    mutable u16 mLastCharCode;  // at 0x18
    mutable u16 mLastGlyphIndex;// at 0x1A
};

} // namespace detail
} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_ResFontBase.h" */

namespace nw4r {
namespace ut {

// Forward declarations
struct BinaryFileHeader;

class ResFont : public detail::ResFontBase {
public:
    ResFont();
    virtual ~ResFont(); // at 0x8

    bool SetResource(void* pBuffer);

private:
    static FontInformation* Rebuild(BinaryFileHeader* pHeader);

private:
    static const u32 SIGNATURE = FOURCC('R', 'F', 'N', 'T');
    static const u32 SIGNATURE_UNPACKED = FOURCC('R', 'F', 'N', 'U');

    static const u32 SIGNATURE_FONTINFO = FOURCC('F', 'I', 'N', 'F');
    static const u32 SIGNATURE_TEXGLYPH = FOURCC('T', 'G', 'L', 'P');
    static const u32 SIGNATURE_CHARWIDTH = FOURCC('C', 'W', 'D', 'H');
    static const u32 SIGNATURE_CHARMAP = FOURCC('C', 'M', 'A', 'P');

    // TODO(kiwi) Which block is this?
    static const u32 SIGNATURE_GLGR = FOURCC('G', 'L', 'G', 'R');
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_ResFont.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 17 "nw4r/ut/ut_ResFontBase.h" */
/* end "nw4r/ut/ut_ResFontBase.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 18 "nw4r/ut/ut_RomFont.h" */
#ifndef NW4R_UT_ROM_FONT_H
#define NW4R_UT_ROM_FONT_H
/* "libs/nw4r/include/nw4r/ut/ut_RomFont.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_RomFont.h" line 4 "nw4r/ut/ut_Font.h" */
/* end "nw4r/ut/ut_Font.h" */

/* "libs/nw4r/include/nw4r/ut/ut_RomFont.h" line 6 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */

namespace nw4r {
namespace ut {

class RomFont : public Font {
public:
    RomFont();
    virtual ~RomFont(); // at 0x8

    virtual int GetWidth() const;  // at 0xC
    virtual int GetHeight() const; // at 0x10

    virtual int GetAscent() const;      // at 0x14
    virtual int GetDescent() const;     // at 0x18
    virtual int GetBaselinePos() const; // at 0x1C

    virtual int GetCellHeight() const;   // at 0x20
    virtual int GetCellWidth() const;    // at 0x24
    virtual int GetMaxCharWidth() const; // at 0x28

    virtual Type GetType() const;              // at 0x2C
    virtual GXTexFmt GetTextureFormat() const; // at 0x30
    virtual int GetLineFeed() const;           // at 0x34

    virtual CharWidths GetDefaultCharWidths() const;              // at 0x38
    virtual void SetDefaultCharWidths(const CharWidths& rWidths); // at 0x3C

    virtual bool SetAlternateChar(u16 ch); // at 0x40
    virtual void SetLineFeed(int lf);      // at 0x44

    virtual int GetCharWidth(u16 ch) const;             // at 0x48
    virtual CharWidths GetCharWidths(u16 ch) const;     // at 0x4C
    virtual void GetGlyph(Glyph* pGlyph, u16 ch) const; // at 0x50
    virtual FontEncoding GetEncoding() const;           // at 0x54

    u32 GetRequireBufferSize();
    bool Load(void* pBuffer);

private:
    static const int CHAR_PTR_BUFFER_SIZE = 4;

private:
    void MakeCharPtr(char* pBuffer, u16 ch) const;
    u16 HandleUndefinedChar(u16 ch) const;

private:
    OSFontHeader* mFontHeader; // at 0x10
    CharWidths mDefaultWidths; // at 0x14
    u16 mAlternateChar;        // at 0x18

    static u16 mFontEncode;
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_RomFont.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 19 "nw4r/ut/ut_RuntimeTypeInfo.h" */
/* end "nw4r/ut/ut_RuntimeTypeInfo.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 20 "nw4r/ut/ut_TagProcessor.h" */
#ifndef NW4R_UT_TAG_PROCESSOR_H
#define NW4R_UT_TAG_PROCESSOR_H
/* "libs/nw4r/include/nw4r/ut/ut_TagProcessor.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_TagProcessor.h" line 4 "nw4r/ut/ut_TagProcessorBase.h" */
#ifndef NW4R_UT_TAG_PROCESSOR_BASE_H
#define NW4R_UT_TAG_PROCESSOR_BASE_H
/* "libs/nw4r/include/nw4r/ut/ut_TagProcessorBase.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace ut {

// Forward declarations
struct Rect;
template <typename T> class TextWriterBase;

/******************************************************************************
 *
 * PrintContext
 *
 ******************************************************************************/
template <typename T> struct PrintContext {
    enum Flags {
        FLAGS_CHARSPACE = (1 << 0),
    };

    TextWriterBase<T>* writer; // at 0x0
    const T* str;              // at 0x4
    f32 x;                     // at 0x8
    f32 y;                     // at 0xC
    u32 flags;                 // at 0x10
};

/******************************************************************************
 *
 * TagProcessorBase
 *
 ******************************************************************************/
template <typename T> class TagProcessorBase {
public:
    typedef PrintContext<T> ContextType;

    enum Operation {
        OPERATION_DEFAULT,
        OPERATION_NO_CHAR_SPACE,
        OPERATION_CHAR_SPACE,
        OPERATION_NEXT_LINE,
        OPERATION_END_DRAW
    };

public:
    TagProcessorBase();
    virtual ~TagProcessorBase(); // at 0x8

    virtual Operation Process(u16 ch, ContextType* pCtx); // at 0xC
    virtual Operation CalcRect(Rect* pRect, u16 ch,
                               ContextType* pCtx); // at 0x10

private:
    void ProcessTab(ContextType* pCtx);
    void ProcessLinefeed(ContextType* pCtx);
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_TagProcessorBase.h" */

namespace nw4r {
namespace ut {

typedef TagProcessorBase<char> TagProcessor;

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_TagProcessor.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 21 "nw4r/ut/ut_TagProcessorBase.h" */
/* end "nw4r/ut/ut_TagProcessorBase.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 22 "nw4r/ut/ut_TextWriter.h" */
#ifndef NW4R_UT_TEXT_WRITER_H
#define NW4R_UT_TEXT_WRITER_H
/* "libs/nw4r/include/nw4r/ut/ut_TextWriter.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_TextWriter.h" line 4 "nw4r/ut/ut_TextWriterBase.h" */
#ifndef NW4R_UT_TEXT_WRITER_BASE_H
#define NW4R_UT_TEXT_WRITER_BASE_H
/* "libs/nw4r/include/nw4r/ut/ut_TextWriterBase.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_TextWriterBase.h" line 4 "nw4r/ut/ut_CharWriter.h" */
/* end "nw4r/ut/ut_CharWriter.h" */
/* "libs/nw4r/include/nw4r/ut/ut_TextWriterBase.h" line 5 "nw4r/ut/ut_TagProcessorBase.h" */
/* end "nw4r/ut/ut_TagProcessorBase.h" */

/* "libs/nw4r/include/nw4r/ut/ut_TextWriterBase.h" line 7 "nw4r/math.h" */
/* end "nw4r/math.h" */

/* "libs/nw4r/include/nw4r/ut/ut_TextWriterBase.h" line 9 "cstdio" */
/* end "cstdio" */
/* "libs/nw4r/include/nw4r/ut/ut_TextWriterBase.h" line 10 "cwchar" */
#ifndef MSL_CPP_CWCHAR_H
#define MSL_CPP_CWCHAR_H
/* "libs/PowerPC_EABI_Support/include/stl/cwchar" line 2 "wchar.h" */
/* end "wchar.h" */
#ifdef __cplusplus

namespace std {
using ::swprintf;
using ::vswprintf;
using ::wcscat;
using ::wcschr;
using ::wcscmp;
using ::wcscpy;
using ::wcslen;
using ::wcsncpy;
} // namespace std

#endif
#endif
/* end "cwchar" */

namespace nw4r {
namespace ut {

template <typename T> class TextWriterBase : public CharWriter {
public:
    typedef TagProcessorBase<T> TagProcessorType;

public:
    enum DrawFlag {
        // Align text lines
        DRAWFLAG_ALIGN_TEXT_BASELINE = 0,
        DRAWFLAG_ALIGN_TEXT_CENTER = (1 << 0),
        DRAWFLAG_ALIGN_TEXT_RIGHT = (1 << 1),

        // Align text block (horizontal)
        DRAWFLAG_ALIGN_H_BASELINE = 0,
        DRAWFLAG_ALIGN_H_CENTER = (1 << 4),
        DRAWFLAG_ALIGN_H_RIGHT = (1 << 5),

        // Align text block (vertical)
        DRAWFLAG_ALIGN_V_BASELINE = 0,
        DRAWFLAG_ALIGN_V_CENTER = (1 << 8),
        DRAWFLAG_ALIGN_V_TOP = (1 << 9),

        // Mask constants
        DRAWFLAG_MASK_ALIGN_TEXT = DRAWFLAG_ALIGN_TEXT_BASELINE |
                                   DRAWFLAG_ALIGN_TEXT_CENTER |
                                   DRAWFLAG_ALIGN_TEXT_RIGHT,

        DRAWFLAG_MASK_ALIGN_H = DRAWFLAG_ALIGN_H_BASELINE |
                                DRAWFLAG_ALIGN_H_CENTER |
                                DRAWFLAG_ALIGN_H_RIGHT,

        DRAWFLAG_MASK_ALIGN_V = DRAWFLAG_ALIGN_V_BASELINE |
                                DRAWFLAG_ALIGN_V_CENTER | DRAWFLAG_ALIGN_V_TOP,
    };

public:
    TextWriterBase();
    ~TextWriterBase();

    f32 GetWidthLimit() const {
        return mWidthLimit;
    }
    void SetWidthLimit(f32 limit) {
        mWidthLimit = limit;
    }
    void ResetWidthLimit() {
        mWidthLimit = NW4R_MATH_FLT_MAX;
    }

    f32 GetCharSpace() const {
        return mCharSpace;
    }
    void SetCharSpace(f32 space) {
        mCharSpace = space;
    }

    f32 GetLineSpace() const {
        return mLineSpace;
    }
    void SetLineSpace(f32 space) {
        mLineSpace = space;
    }

    int GetTabWidth() const {
        return mTabWidth;
    }
    void SetTabWidth(int width) {
        mTabWidth = width;
    }

    u32 GetDrawFlag() const {
        return mDrawFlag;
    }
    void SetDrawFlag(u32 flag) {
        mDrawFlag = flag;
    }

    TagProcessorBase<T>* GetTagProcessor() const {
        return mTagProcessor;
    }
    void SetTagProcessor(TagProcessorBase<T>* pProcessor) {
        mTagProcessor = pProcessor;
    }
    void ResetTagProcessor() {
        mTagProcessor = &mDefaultTagProcessor;
    }

    f32 GetLineHeight() const;

    f32 CalcLineWidth(const T* pStr, int len);
    f32 CalcStringWidth(const T* pStr, int len) const;
    void CalcStringRect(Rect* pRect, const T* pStr, int len) const;

    int VSNPrintf(T* buffer, u32 count, const T* pStr, std::va_list args);
    f32 VPrintf(const T* pStr, std::va_list args);
    f32 Print(const T* pStr, int len);
    f32 PrintMutable(const T* pStr, int len);

    static T* GetBuffer() {
        return mFormatBuffer;
    }
    static T* SetBuffer(T* pBuffer, u32 size) {
        T* pOldBuffer = mFormatBuffer;
        mFormatBuffer = pBuffer;
        mFormatBufferSize = size;
        return pOldBuffer;
    }

    static u32 GetBufferSize() {
        return mFormatBufferSize;
    }

private:
    static const int DEFAULT_FORMAT_BUFFER_SIZE = 256;

    static const u32 DRAWFLAG_MASK_ALL = DRAWFLAG_MASK_ALIGN_TEXT |
                                         DRAWFLAG_MASK_ALIGN_H |
                                         DRAWFLAG_MASK_ALIGN_V;

private:
    bool IsDrawFlagSet(u32 mask, u32 flag) const {
        return (mDrawFlag & mask) == flag;
    }

    bool CalcLineRectImpl(Rect* pRect, const T** ppStr, int len);
    void CalcStringRectImpl(Rect* pRect, const T* pStr, int len);

    f32 PrintImpl(const T* pStr, int len, bool bMutable);
    f32 AdjustCursor(f32* pX, f32* pY, const T* pStr, int len);

private:
    f32 mWidthLimit;                    // at 0x4C
    f32 mCharSpace;                     // at 0x50
    f32 mLineSpace;                     // at 0x54
    int mTabWidth;                      // at 0x58
    u32 mDrawFlag;                      // at 0x5C
    TagProcessorBase<T>* mTagProcessor; // at 0x60

    static T* mFormatBuffer;
    static u32 mFormatBufferSize;
    static TagProcessorBase<T> mDefaultTagProcessor;
};

template <>
inline int TextWriterBase<char>::VSNPrintf(char* pBuffer, u32 count,
                                           const char* pStr,
                                           std::va_list args) {

    return std::vsnprintf(pBuffer, count, pStr, args);
}

template <>
inline int TextWriterBase<wchar_t>::VSNPrintf(wchar_t* pBuffer, u32 count,
                                              const wchar_t* pStr,
                                              std::va_list args) {

    return std::vswprintf(pBuffer, count, pStr, args);
}

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_TextWriterBase.h" */

namespace nw4r {
namespace ut {

typedef TextWriterBase<char> TextWriter;

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_TextWriter.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 23 "nw4r/ut/ut_TextWriterBase.h" */
/* end "nw4r/ut/ut_TextWriterBase.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 24 "nw4r/ut/ut_WideTagProcessor.h" */
#ifndef NW4R_UT_WIDE_TAG_PROCESSOR_H
#define NW4R_UT_WIDE_TAG_PROCESSOR_H
/* "libs/nw4r/include/nw4r/ut/ut_WideTagProcessor.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_WideTagProcessor.h" line 4 "nw4r/ut/ut_TagProcessorBase.h" */
/* end "nw4r/ut/ut_TagProcessorBase.h" */

namespace nw4r {
namespace ut {

typedef TagProcessorBase<wchar_t> WideTagProcessor;

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_WideTagProcessor.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 25 "nw4r/ut/ut_WideTextWriter.h" */
#ifndef NW4R_UT_WIDE_TEXT_WRITER_H
#define NW4R_UT_WIDE_TEXT_WRITER_H
/* "libs/nw4r/include/nw4r/ut/ut_WideTextWriter.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_WideTextWriter.h" line 4 "nw4r/ut/ut_TextWriterBase.h" */
/* end "nw4r/ut/ut_TextWriterBase.h" */

namespace nw4r {
namespace ut {

typedef TextWriterBase<wchar_t> WideTextWriter;

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_WideTextWriter.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 26 "nw4r/ut/ut_algorithm.h" */
#ifndef NW4R_UT_ALGORITHM_H
#define NW4R_UT_ALGORITHM_H
/* "libs/nw4r/include/nw4r/ut/ut_algorithm.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace ut {
namespace {

/******************************************************************************
 *
 * Value operations
 *
 ******************************************************************************/
template <typename T> inline T Max(T t1, T t2) {
    return (t1 < t2) ? t2 : t1;
}

template <typename T> inline T Min(T t1, T t2) {
    return (t1 > t2) ? t2 : t1;
}

template <typename T> inline T Clamp(T value, T min, T max) {
    return value > max ? max : (value < min ? min : value);
}

template <typename T> inline T Abs(T x) {
    // Static cast needed to break abs optimization
    return x < 0 ? static_cast<T>(-x) : static_cast<T>(x);
}

template <> f32 inline Abs(register f32 x) {
    register f32 ax;

    ASM (
        fabs ax, x
    )

    return ax;
}

/******************************************************************************
 *
 * Bit operations
 *
 ******************************************************************************/
template <typename T> inline T BitExtract(T bits, int pos, int len) {
    T mask = (1 << len) - 1;
    return (bits >> pos) & mask;
}

template <typename T> inline bool TestBit(T t, int pos) {
    return BitExtract<T>(t, sizeof(T), pos);
}

/******************************************************************************
 *
 * Pointer arithmetic
 *
 ******************************************************************************/
inline u32 GetIntPtr(const void* pPtr) {
    return reinterpret_cast<u32>(pPtr);
}

template <typename T>
inline const void* AddOffsetToPtr(const void* pBase, T offset) {
    return reinterpret_cast<const void*>(GetIntPtr(pBase) + offset);
}
template <typename T> inline void* AddOffsetToPtr(void* pBase, T offset) {
    return reinterpret_cast<void*>(GetIntPtr(pBase) + offset);
}

inline s32 GetOffsetFromPtr(const void* pStart, const void* pEnd) {
    return static_cast<s32>(GetIntPtr(pEnd) - GetIntPtr(pStart));
}

inline int ComparePtr(const void* pPtr1, const void* pPtr2) {
    return static_cast<int>(GetIntPtr(pPtr1) - GetIntPtr(pPtr2));
}

/******************************************************************************
 *
 * Rounding
 *
 ******************************************************************************/
template <typename T> inline T RoundUp(T t, unsigned int alignment) {
    return (alignment + t - 1) & ~(alignment - 1);
}

template <typename T> inline void* RoundUp(T* pPtr, unsigned int alignment) {
    u32 value = reinterpret_cast<u32>(pPtr);
    u32 rounded = (alignment + value - 1) & ~(alignment - 1);
    return reinterpret_cast<void*>(rounded);
}

template <typename T> inline T RoundDown(T t, unsigned int alignment) {
    return t & ~(alignment - 1);
}

template <typename T> inline void* RoundDown(T* pPtr, unsigned int alignment) {
    u32 value = reinterpret_cast<u32>(pPtr);
    u32 rounded = value & ~(alignment - 1);
    return reinterpret_cast<void*>(rounded);
}

} // namespace
} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_algorithm.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 27 "nw4r/ut/ut_binaryFileFormat.h" */
#ifndef NW4R_UT_BINARY_FILE_FORMAT_H
#define NW4R_UT_BINARY_FILE_FORMAT_H
/* "libs/nw4r/include/nw4r/ut/ut_binaryFileFormat.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace ut {

struct BinaryBlockHeader {
    u32 kind; // at 0x0
    u32 size; // at 0x4
};

struct BinaryFileHeader {
    u32 signature;  // at 0x0
    u16 byteOrder;  // at 0x4
    u16 version;    // at 0x6
    u32 fileSize;   // at 0x8
    u16 headerSize; // at 0xC
    u16 dataBlocks; // at 0xE
};

bool IsValidBinaryFile(const BinaryFileHeader* pHeader, u32 signature,
                       u16 version, u16 minBlocks);
bool IsReverseEndianBinaryFile(const BinaryFileHeader* pHeader);
BinaryBlockHeader* GetNextBinaryBlockHeader(BinaryFileHeader* pHeader,
                                            BinaryBlockHeader* pBlockHeader);

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_binaryFileFormat.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 28 "nw4r/ut/ut_list.h" */
#ifndef NW4R_UT_LIST_H
#define NW4R_UT_LIST_H
/* "libs/nw4r/include/nw4r/ut/ut_list.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace ut {

struct List {
    void* headObject; // at 0x0
    void* tailObject; // at 0x4
    u16 numObjects;   // at 0x8
    u16 offset;       // at 0xA
};

struct Link {
    void* prevObject; // at 0x0
    void* nextObject; // at 0x4
};

void List_Init(List* pList, u16 offset);

void List_Append(List* pList, void* pObject);
void List_Prepend(List* pList, void* pObject);

void List_Insert(List* pList, void* pTarget, void* pObject);
void List_Remove(List* pList, void* pObject);

void* List_GetNext(const List* pList, const void* pObject);
inline const void* List_GetNextConst(const List* pList, const void* pObject) {
    return List_GetNext(pList, pObject);
}

void* List_GetPrev(const List* pList, const void* pObject);
inline const void* List_GetPrevConst(const List* pList, const void* pObject) {
    return List_GetPrev(pList, pObject);
}

void* List_GetNth(const List* pList, u16 n);
inline const void* List_GetNthConst(const List* pList, u16 n) {
    return List_GetNth(pList, n);
}

inline void* List_GetFirst(const List* pList) {
    return List_GetNext(pList, NULL);
}
inline const void* List_GetFirstConst(const List* pList) {
    return List_GetFirst(pList);
}

inline void* List_GetLast(const List* pList) {
    return List_GetPrev(pList, NULL);
}
inline const void* List_GetLastConst(const List* pList) {
    return List_GetLast(pList);
}

inline u16 List_GetSize(const List* pList) {
    return pList->numObjects;
}

/******************************************************************************
 *
 * Macros
 *
 ******************************************************************************/
/**
 * Declares a member Link.
 */
#define NW4R_UT_LIST_LINK_DECL() nw4r::ut::Link link

/**
 * Initializes a List object for use with the specified type.
 *
 * @param LIST Reference to list
 * @param T List element type
 */
#define NW4R_UT_LIST_INIT(LIST, T)                                             \
    nw4r::ut::List_Init(&(LIST), offsetof(T, link))

/**
 * Gets the underlying Link within the specified object.
 *
 * @param LIST Reference to list
 * @param OBJ Pointer to list object
 */
#define NW4R_UT_LIST_GET_LINK(LIST, OBJ)                                       \
    reinterpret_cast<nw4r::ut::Link*>((u8*)(OBJ) + (LIST).offset)

/**
 * List for-each macro.
 *
 * @param TYPE Element type
 * @param NAME Element name
 * @param LIST Reference to list
 * @param ... Statement(s) to execute
 */
#define NW4R_UT_LIST_FOREACH(TYPE, NAME, LIST, ...)                            \
    {                                                                          \
        TYPE* NAME = NULL;                                                     \
                                                                               \
        while ((NAME = static_cast<TYPE*>(                                     \
                    nw4r::ut::List_GetNext(&(LIST), NAME))) != NULL) {         \
                                                                               \
            __VA_ARGS__;                                                       \
        }                                                                      \
    }
/**
 * List for-each macro (reverse order).
 *
 * @param TYPE Element type
 * @param NAME Element name
 * @param LIST Reference to list
 * @param ... Statement(s) to execute
 */
#define NW4R_UT_LIST_FOREACH_REV(TYPE, NAME, LIST, ...)                        \
    {                                                                          \
        TYPE* NAME = NULL;                                                     \
                                                                               \
        while ((NAME = static_cast<TYPE*>(                                     \
                    nw4r::ut::List_GetPrev(&(LIST), NAME))) != NULL) {         \
                                                                               \
            __VA_ARGS__;                                                       \
        }                                                                      \
    }

/**
 * List for-each macro, with robust iteration.
 *
 * @param TYPE Element type
 * @param NAME Element name
 * @param LIST Reference to list
 * @param ... Statement(s) to execute
 */
#define NW4R_UT_LIST_FOREACH_SAFE(TYPE, NAME, LIST, ...)                       \
    {                                                                          \
        TYPE* NAME;                                                            \
        TYPE* __next__;                                                        \
                                                                               \
        for (NAME = static_cast<TYPE*>(nw4r::ut::List_GetFirst(&(LIST)));      \
             NAME != NULL; NAME = __next__) {                                  \
                                                                               \
            __next__ =                                                         \
                static_cast<TYPE*>(nw4r::ut::List_GetNext(&(LIST), NAME));     \
                                                                               \
            __VA_ARGS__;                                                       \
        }                                                                      \
    }

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_list.h" */
/* "libs/nw4r/include/nw4r/ut.h" line 29 "nw4r/ut/ut_lock.h" */
#ifndef NW4R_UT_LOCK_H
#define NW4R_UT_LOCK_H
/* "libs/nw4r/include/nw4r/ut/ut_lock.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/ut/ut_lock.h" line 4 "nw4r/ut/ut_NonCopyable.h" */
/* end "nw4r/ut/ut_NonCopyable.h" */

/* "libs/nw4r/include/nw4r/ut/ut_lock.h" line 6 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */

namespace nw4r {
namespace ut {
namespace detail {

/******************************************************************************
 *
 * Mutex lock functions
 *
 ******************************************************************************/
inline void Lock(OSMutex& rMutex) {
    OSLockMutex(&rMutex);
}
inline void Unlock(OSMutex& rMutex) {
    OSUnlockMutex(&rMutex);
}

/******************************************************************************
 *
 * AutoLock
 *
 ******************************************************************************/
template <typename T> class AutoLock : private NonCopyable {
public:
    explicit AutoLock(T& rLockObj) : mLockObj(rLockObj) {
        Lock(mLockObj);
    }
    ~AutoLock() {
        Unlock(mLockObj);
    }

private:
    T& mLockObj; // at 0x0
};

} // namespace detail

/******************************************************************************
 *
 * AutoInterruptLock
 *
 ******************************************************************************/
class AutoInterruptLock : private NonCopyable {
public:
    AutoInterruptLock() : mOldState(OSDisableInterrupts()) {}
    ~AutoInterruptLock() {
        OSRestoreInterrupts(mOldState);
    }

private:
    BOOL mOldState; // at 0x0
};

} // namespace ut
} // namespace nw4r

#endif
/* end "nw4r/ut/ut_lock.h" */

#endif
/* end "nw4r/ut.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_common.h" line 7 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class ResourceAccessor;
struct Size;

namespace res {
struct BinaryFileHeader;
} // namespace res

/******************************************************************************
 *
 * VertexColor
 *
 ******************************************************************************/
enum VertexColor {
    VERTEXCOLOR_LT,
    VERTEXCOLOR_RT,
    VERTEXCOLOR_LB,
    VERTEXCOLOR_RB,

    VERTEXCOLOR_MAX
};

/******************************************************************************
 *
 * TevColor
 *
 ******************************************************************************/
enum TevColor {
    TEVCOLOR_REG0,
    TEVCOLOR_REG1,
    TEVCOLOR_REG2,

    TEVCOLOR_MAX
};

/******************************************************************************
 *
 * Text position
 *
 ******************************************************************************/
enum HorizontalPosition {
    HORIZONTALPOSITION_LEFT,
    HORIZONTALPOSITION_CENTER,
    HORIZONTALPOSITION_RIGHT,
    HORIZONTALPOSITION_MAX
};
enum VerticalPosition {
    VERTICALPOSITION_TOP,
    VERTICALPOSITION_CENTER,
    VERTICALPOSITION_BOTTOM,
    VERTICALPOSITION_MAX
};

namespace detail {

/******************************************************************************
 *
 * Vertex colors
 *
 ******************************************************************************/
inline u8 GetVtxColorElement(const ut::Color* pColors, u32 idx) {
    return reinterpret_cast<const u8*>(&pColors[idx / 4])[idx % 4];
}
inline void SetVtxColorElement(ut::Color* pColors, u32 idx, u8 value) {
    reinterpret_cast<u8*>(&pColors[idx / 4])[idx % 4] = value;
}

/******************************************************************************
 *
 * Positioning
 *
 ******************************************************************************/
inline u8 GetHorizontalPosition(u8 packed) {
    return packed % HORIZONTALPOSITION_MAX;
}
inline u8 GetVerticalPosition(u8 packed) {
    return packed / HORIZONTALPOSITION_MAX;
}

inline void SetHorizontalPosition(u8* pPacked, u8 value) {
    *pPacked = GetVerticalPosition(*pPacked) * HORIZONTALPOSITION_MAX + value;
}
inline void SetVerticalPosition(u8* pPacked, u8 value) {
    *pPacked = value * HORIZONTALPOSITION_MAX + GetHorizontalPosition(*pPacked);
}

/******************************************************************************
 *
 * TexCoordAry
 *
 ******************************************************************************/
typedef math::VEC2 TexCoord[VERTEXCOLOR_MAX];

class TexCoordAry {
public:
    TexCoordAry();

    void Free();
    void Reserve(u8 num);
    void SetCoord(u32 idx, const math::VEC2* coord);
    void SetSize(u8 num);
    void Copy(const void* pSrc, u8 num);
    void DrawLine(const math::VEC2 &pos, const Size &size, ut::Color &color);

    bool IsEmpty() const {
        return mCap == 0;
    }

    u8 GetSize() const {
        return mNum;
    }

    TexCoord* GetArray() const {
        return mpData;
    }

private:
    u8 mCap; // at 0x0
    u8 mNum; // at 0x1
    TexCoord* mpData;
};

/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
inline bool IsCITexelFormat(GXTexFmt format) {
    return format == GX_TF_C4 || format == GX_TF_C8 || format == GX_TF_C14X2;
}

inline s32 GetSignatureInt(const char* pSignature) {
    return *reinterpret_cast<const s32*>(pSignature);
}

inline const char* GetStrTableStr(const void* pTable, int index) {
    const u32* pOffsetTbl = static_cast<const u32*>(pTable);
    const char* pStringPool = static_cast<const char*>(pTable);
    return pStringPool + pOffsetTbl[index];
}

bool TestFileHeader(const res::BinaryFileHeader& rHeader);
bool TestFileHeader(const res::BinaryFileHeader& rHeader, u32 signature);

bool EqualsResName(const char* pLhs, const char* pRhs);
bool EqualsMaterialName(const char* pLhs, const char* pRhs);

bool IsModulateVertexColor(ut::Color* pColors, u8 glbAlpha);

ut::Color MultipleAlpha(ut::Color color, u8 alpha);
void MultipleAlpha(ut::Color* pDst, const ut::Color* pSrc, u8 alpha);

void SetVertexFormat(bool modulate, u8 numCoord);

void DrawQuad(const math::VEC2& rBase, const Size& rSize, u8 num,
              const TexCoord* pCoords, const ut::Color* pColors);
void DrawQuad(const math::VEC2& rBase, const Size& rSize, u8 num,
              const TexCoord* pCoords, const ut::Color* pColors, u8 alpha);

} // namespace detail
} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_common.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_animation.h" line 5 "nw4r/lyt/lyt_resources.h" */
#ifndef NW4R_LYT_RESOURCES_H
#define NW4R_LYT_RESOURCES_H
/* "libs/nw4r/include/nw4r/lyt/lyt_resources.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_resources.h" line 4 "nw4r/lyt/lyt_types.h" */
#ifndef NW4R_LYT_TYPES_H
#define NW4R_LYT_TYPES_H
/* "libs/nw4r/include/nw4r/lyt/lyt_types.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_types.h" line 4 "nw4r/math.h" */
/* end "nw4r/math.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_types.h" line 5 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

#define NW4R_LYT_LIBRARY_VERSION 8

#define NW4R_LYT_RES_NAME_LEN 16
#define NW4R_LYT_PANE_USERDATA_LEN 8
#define NW4R_LYT_MATERIAL_NAME_LEN 20

namespace nw4r {
namespace lyt {

// Forward declarations
class AnimTransform;

namespace detail {

/******************************************************************************
 *
 * Pointer operations
 *
 ******************************************************************************/
template <typename T> T* ConvertOffsToPtr(void* pBase, u32 offset) {
    return reinterpret_cast<T*>(reinterpret_cast<u8*>(pBase) + offset);
}
template <typename T> const T* ConvertOffsToPtr(const void* pBase, u32 offset) {
    return reinterpret_cast<const T*>(reinterpret_cast<const u8*>(pBase) +
                                      offset);
}

/******************************************************************************
 *
 * Bit operations
 *
 ******************************************************************************/
template <typename T> inline void SetBit(T* pBits, int pos, bool value) {
    T mask = ~(1 << pos);
    *pBits &= mask;
    *pBits |= (value ? 1 : 0) << pos;
}
template <typename T> inline bool TestBit(T bits, int pos) {
    T mask = 1 << pos;
    return (bits & mask) != 0;
}
template <typename T> inline T GetBits(T bits, int pos, int len) {
    T mask = ~(static_cast<T>(-1) << len);
    return bits >> pos & mask;
}

} // namespace detail

/******************************************************************************
 *
 * Size
 *
 ******************************************************************************/
struct Size {
    f32 width;  // at 0x0
    f32 height; // at 0x4

    Size() : width(0.0f), height(0.0f) {}
    Size(const Size& rOther) : width(rOther.width), height(rOther.height) {}
    Size(f32 w, f32 h) : width(w), height(h) {}

    friend bool operator==(const Size& rLhs, const Size& rRhs) {
        return rLhs.width == rRhs.width && rLhs.height == rRhs.height;
    }
};

/******************************************************************************
 *
 * AnimationLink
 *
 ******************************************************************************/
class AnimationLink {
public:
    AnimationLink() : mbDisable(false) {
        Reset();
    }

    void Reset() {
        Set(NULL, 0, false);
    }

    AnimTransform* GetAnimTransform() const {
        return mAnimTrans;
    }
    void Set(AnimTransform* pAnimTrans, u16 idx, bool bDisable) {
        mAnimTrans = pAnimTrans;
        mIdx = idx;
        mbDisable = bDisable;
    }

    u16 GetIndex() const {
        return mIdx;
    }

    bool IsEnable() const {
        return !mbDisable;
    }
    void SetEnable(bool enable) {
        mbDisable = !enable;
    }

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x0

private:
    AnimTransform* mAnimTrans; // at 0x8
    u16 mIdx;                  // at 0xC
    bool mbDisable;            // at 0xE
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(AnimationLink);

/******************************************************************************
 *
 * AlphaCompare
 *
 ******************************************************************************/
struct AlphaCompare {
    u8 comp; // at 0x0
    u8 op;   // at 0x1
    u8 ref0; // at 0x2
    u8 ref1; // at 0x3

    AlphaCompare() {
        Set(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    }

    void Set(GXCompare comp0, u8 _ref0, GXAlphaOp _op, GXCompare comp1,
             u8 _ref1) {

        comp = (comp0 & 0b111) | ((comp1 & 0b111) << 4);
        op = _op;
        ref0 = _ref0;
        ref1 = _ref1;
    }

    GXCompare GetComp0() const {
        return static_cast<GXCompare>(comp & 0b1111);
    }
    GXCompare GetComp1() const {
        return static_cast<GXCompare>(comp >> 4 & 0b1111);
    }

    GXAlphaOp GetOp() const {
        return static_cast<GXAlphaOp>(op);
    }

    u8 GetRef0() const {
        return ref0;
    }
    u8 GetRef1() const {
        return ref1;
    }
};

/******************************************************************************
 *
 * BlendMode
 *
 ******************************************************************************/
struct BlendMode {
    u8 type;      // at 0x0
    u8 srcFactor; // at 0x1
    u8 dstFactor; // at 0x2
    u8 op;        // at 0x3

    BlendMode() {
        Set(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    }

    void Set(GXBlendMode _type, GXBlendFactor src, GXBlendFactor dst,
             GXLogicOp _op) {

        type = _type;
        srcFactor = src;
        dstFactor = dst;
        op = _op;
    }

    GXBlendMode GetType() const {
        return static_cast<GXBlendMode>(type);
    }

    GXBlendFactor GetSrcFactor() const {
        return static_cast<GXBlendFactor>(srcFactor);
    }
    GXBlendFactor GetDstFactor() const {
        return static_cast<GXBlendFactor>(dstFactor);
    }

    GXLogicOp GetOp() const {
        return static_cast<GXLogicOp>(op);
    }
};

/******************************************************************************
 *
 * ChanCtrl
 *
 ******************************************************************************/
struct ChanCtrl {
    u8 matSrcCol; // at 0x0
    u8 matSrcAlp; // at 0x1
    u8 reserve1;  // at 0x2
    u8 reserve2;  // at 0x3

    ChanCtrl() : reserve1(0), reserve2(0) {
        Set(GX_SRC_VTX, GX_SRC_VTX);
    }

    void Set(GXColorSrc color, GXColorSrc alpha) {
        matSrcCol = color;
        matSrcAlp = alpha;
    }

    GXColorSrc GetColorSrc() const {
        return static_cast<GXColorSrc>(matSrcCol);
    }
    GXColorSrc GetAlphaSrc() const {
        return static_cast<GXColorSrc>(matSrcAlp);
    }
};

/******************************************************************************
 *
 * IndirectStage
 *
 ******************************************************************************/
struct IndirectStage {
    u8 texCoordGen; // at 0x0
    u8 texMap;      // at 0x1
    u8 scaleS;      // at 0x2
    u8 scaleT;      // at 0x3

    IndirectStage() {
        Set(GX_TEXCOORD0, GX_TEXMAP0, GX_ITS_1, GX_ITS_1);
    }

    void Set(GXTexCoordID gen, GXTexMapID map, GXIndTexScale _scaleS,
             GXIndTexScale _scaleT) {

        texCoordGen = gen;
        texMap = map;
        scaleS = _scaleS;
        scaleT = _scaleT;
    }

    GXTexCoordID GetTexCoordGen() const {
        return static_cast<GXTexCoordID>(texCoordGen);
    }

    GXTexMapID GetTexMap() const {
        return static_cast<GXTexMapID>(texMap);
    }

    GXIndTexScale GetScaleS() const {
        return static_cast<GXIndTexScale>(scaleS);
    }
    GXIndTexScale GetScaleT() const {
        return static_cast<GXIndTexScale>(scaleT);
    }
};

/******************************************************************************
 *
 * TevStageInOp
 *
 ******************************************************************************/
struct TevStageInOp {
    u8 ab; // at 0x0
    u8 cd; // at 0x1
    u8 op; // at 0x2
    u8 cl; // at 0x3

    u8 GetA() const {
        return ab & 0b1111;
    }
    u8 GetB() const {
        return ab >> 4 & 0b1111;
    }

    u8 GetC() const {
        return cd & 0b1111;
    }
    u8 GetD() const {
        return cd >> 4 & 0b1111;
    }

    void SetIn(u8 a, u8 b, u8 c, u8 d) {
        ab = a & 0b1111 | (b & 0b1111) << 4;
        cd = c & 0b1111 | (d & 0b1111) << 4;
    }

    u8 GetOp() const {
        return op & 0b1111;
    }
    u8 GetBias() const {
        return op >> 4 & 0b11;
    }
    u8 GetScale() const {
        return op >> 6 & 0b11;
    }

    bool IsClamp() const {
        return cl & 0b1;
    }
    u8 GetOutReg() const {
        return cl >> 1 & 0b11;
    }
    u8 GetKSel() const {
        return cl >> 3 & 0b11111;
    }

    void SetOp(u8 _op, u8 bias, u8 scale, bool clamp, u8 outReg, u8 ksel) {
        op = _op & 0b1111 | (bias & 0b11) << 4 | (scale & 0b11) << 6;
        cl = clamp & 0b1 | (outReg & 0b11) << 1 | (ksel & 0b11111) << 3;
    }
};

/******************************************************************************
 *
 * TevStage
 *
 ******************************************************************************/
struct TevStage {
    u8 texCoordGen;     // at 0x0
    u8 colChan;         // at 0x1
    u8 texMap;          // at 0x2
    u8 swapSel;         // at 0x3
    TevStageInOp colIn; // at 0x4
    TevStageInOp alpIn; // at 0x8
    u8 indStage;        // at 0xC
    u8 indBiMt;         // at 0xD
    u8 indWrap;         // at 0xE
    u8 indFoAdUtAl;     // at 0xF

    TevStage() {
        SetOrder(GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0, GX_TEV_SWAP0,
                 GX_TEV_SWAP0);

        SetColorIn(GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        SetAlphaIn(GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);

        SetColorOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV,
                   GX_TEV_KCSEL_K0);
        SetAlphaOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV,
                   GX_TEV_KASEL_K0_R);

        SetIndirect(GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF,
                    GX_ITW_OFF, GX_ITW_OFF, false, false, GX_ITBA_OFF);
    }

    void SetOrder(GXTexCoordID gen, GXTexMapID map, GXChannelID chan,
                  GXTevSwapSel ras, GXTevSwapSel tex) {

        texCoordGen = gen;
        colChan = chan;
        texMap = map;
        swapSel = (ras & 0b111) << 1 | (tex & 0b111) << 3 | (map >> 8);
    }

    void SetColorIn(GXTevColorArg a, GXTevColorArg b, GXTevColorArg c,
                    GXTevColorArg d) {

        colIn.SetIn(a, b, c, d);
    }
    void SetColorOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp,
                    GXTevRegID outReg, GXTevKColorSel ksel) {

        colIn.SetOp(op, bias, scale, clamp, outReg, ksel);
    }

    void SetAlphaIn(GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c,
                    GXTevAlphaArg d) {

        alpIn.SetIn(a, b, c, d);
    }
    void SetAlphaOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp,
                    GXTevRegID outReg, GXTevKAlphaSel ksel) {

        alpIn.SetOp(op, bias, scale, clamp, outReg, ksel);
    }

    void SetIndirect(GXIndTexStageID stage, GXIndTexFormat format,
                     GXIndTexBiasSel bias, GXIndTexMtxID mtx,
                     GXIndTexWrap wrapS, GXIndTexWrap wrapT, bool addPrev,
                     bool utcLod, GXIndTexAlphaSel alphaSel) {

        indStage = stage;
        indBiMt = bias & 0b111 | (mtx & 0b1111) << 4;
        indWrap = wrapS & 0b111 | (wrapT & 0b111) << 3;

        indFoAdUtAl = format & 0b111 | (addPrev ? 1 : 0) << 2 | utcLod << 3 |
                      (alphaSel & 0b11) << 4;
    }

    GXTexCoordID GetTexCoordGen() const {
        return static_cast<GXTexCoordID>(texCoordGen);
    }

    GXChannelID GetColorChan() const {
        return static_cast<GXChannelID>(colChan);
    }

    GXTexMapID GetTexMap() const {
        return static_cast<GXTexMapID>(texMap | (swapSel & 0b1) << 8);
    }

    GXTevSwapSel GetRasSwapSel() const {
        return static_cast<GXTevSwapSel>(swapSel >> 1 & 0b11);
    }
    GXTevSwapSel GetTexSwapSel() const {
        return static_cast<GXTevSwapSel>(swapSel >> 3 & 0b11);
    }

    GXTevColorArg GetColorInA() const {
        return static_cast<GXTevColorArg>(colIn.GetA());
    }
    GXTevColorArg GetColorInB() const {
        return static_cast<GXTevColorArg>(colIn.GetB());
    }
    GXTevColorArg GetColorInC() const {
        return static_cast<GXTevColorArg>(colIn.GetC());
    }
    GXTevColorArg GetColorInD() const {
        return static_cast<GXTevColorArg>(colIn.GetD());
    }

    GXTevOp GetColorOp() const {
        return static_cast<GXTevOp>(colIn.GetOp());
    }
    GXTevBias GetColorBias() const {
        return static_cast<GXTevBias>(colIn.GetBias());
    }
    GXTevScale GetColorScale() const {
        return static_cast<GXTevScale>(colIn.GetScale());
    }

    bool IsColorClamp() const {
        return colIn.IsClamp();
    }
    GXTevRegID GetColorOutReg() const {
        return static_cast<GXTevRegID>(colIn.GetOutReg());
    }
    GXTevKColorSel GetKColorSel() const {
        return static_cast<GXTevKColorSel>(colIn.GetKSel());
    }

    GXTevAlphaArg GetAlphaInA() const {
        return static_cast<GXTevAlphaArg>(alpIn.GetA());
    }
    GXTevAlphaArg GetAlphaInB() const {
        return static_cast<GXTevAlphaArg>(alpIn.GetB());
    }
    GXTevAlphaArg GetAlphaInC() const {
        return static_cast<GXTevAlphaArg>(alpIn.GetC());
    }
    GXTevAlphaArg GetAlphaInD() const {
        return static_cast<GXTevAlphaArg>(alpIn.GetD());
    }

    GXTevOp GetAlphaOp() const {
        return static_cast<GXTevOp>(alpIn.GetOp());
    }
    GXTevBias GetAlphaBias() const {
        return static_cast<GXTevBias>(alpIn.GetBias());
    }
    GXTevScale GetAlphaScale() const {
        return static_cast<GXTevScale>(alpIn.GetScale());
    }

    bool IsAlphaClamp() const {
        return alpIn.IsClamp();
    }
    GXTevRegID GetAlphaOutReg() const {
        return static_cast<GXTevRegID>(alpIn.GetOutReg());
    }
    GXTevKAlphaSel GetKAlphaSel() const {
        return static_cast<GXTevKAlphaSel>(alpIn.GetKSel());
    }

    GXIndTexStageID GetIndStage() const {
        return static_cast<GXIndTexStageID>(indStage);
    }

    GXIndTexBiasSel GetIndBiasSel() const {
        return static_cast<GXIndTexBiasSel>(indBiMt & 0b111);
    }
    GXIndTexMtxID GetIndMtxSel() const {
        return static_cast<GXIndTexMtxID>(indBiMt >> 3 & 0b1111);
    }

    GXIndTexWrap GetIndWrapS() const {
        return static_cast<GXIndTexWrap>(indWrap & 0b111);
    }
    GXIndTexWrap GetIndWrapT() const {
        return static_cast<GXIndTexWrap>(indWrap >> 3 & 0b111);
    }

    GXIndTexFormat GetIndFormat() const {
        return static_cast<GXIndTexFormat>(indFoAdUtAl & 0b11);
    }
    bool IsIndAddPrev() const {
        return indFoAdUtAl >> 2 & 0b1;
    }
    bool IsIndUtcLod() const {
        return indFoAdUtAl >> 3 & 0b1;
    }
    GXIndTexAlphaSel GetIndAlphaSel() const {
        return static_cast<GXIndTexAlphaSel>(indFoAdUtAl >> 4 & 0b11);
    }
};

/******************************************************************************
 *
 * TevSwapMode
 *
 ******************************************************************************/
struct TevSwapMode {
    u8 swap; // at 0x0

    void Set(GXTevColorChan r, GXTevColorChan g, GXTevColorChan b,
             GXTevColorChan a) {

        swap = r | g << 2 | b << 4 | a << 6;
    }

    GXTevColorChan GetR() const {
        return static_cast<GXTevColorChan>(swap & 0b11);
    }
    GXTevColorChan GetG() const {
        return static_cast<GXTevColorChan>(swap >> 2 & 0b11);
    }
    GXTevColorChan GetB() const {
        return static_cast<GXTevColorChan>(swap >> 4 & 0b11);
    }
    GXTevColorChan GetA() const {
        return static_cast<GXTevColorChan>(swap >> 6 & 0b11);
    }
};

/******************************************************************************
 *
 * TexCoordGen
 *
 ******************************************************************************/
struct TexCoordGen {
    TexCoordGen() : reserve(0) {
        Set(GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    }

    void Set(GXTexGenType type, GXTexGenSrc src, u32 mtxID) {
        texGenType = type;
        texGenSrc = src;
        texMtx = mtxID;
    }

    GXTexGenType GetTexGenType() const {
        return static_cast<GXTexGenType>(texGenType);
    }

    GXTexGenSrc GetTexGenSrc() const {
        return static_cast<GXTexGenSrc>(texGenSrc);
    }

    u32 GetTexMtx() const {
        return texMtx;
    }

    u8 texGenType; // at 0x0
    u8 texGenSrc;  // at 0x1
    u8 texMtx;     // at 0x2
    u8 reserve;    // at 0x3
};

/******************************************************************************
 *
 * TexSRT
 *
 ******************************************************************************/
struct TexSRT {
    math::VEC2 translate; // at 0x0
    f32 rotate;           // at 0x8
    math::VEC2 scale;     // at 0xC
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_types.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class ResourceAccessor;

namespace res {

/******************************************************************************
 *
 * BinaryFileHeader
 *
 ******************************************************************************/
struct BinaryFileHeader {
    char signature[4]; // at 0x0
    u16 byteOrder;     // at 0x4
    u16 version;       // at 0x6
    u32 fileSize;      // at 0x8
    u16 headerSize;    // at 0xC
    u16 dataBlocks;    // at 0xE
};

/******************************************************************************
 *
 * DataBlockHeader
 *
 ******************************************************************************/
struct DataBlockHeader {
    char kind[4]; // at 0x0
    u32 size;     // at 0x4
};

/******************************************************************************
 *
 * Texture
 *
 ******************************************************************************/
struct Texture {
    u32 nameStrOffset;         // at 0x0
    u8 type;                   // at 0x4
    u8 PADDING_0x5[0x8 - 0x5]; // at 0x5
};
/******************************************************************************
 *
 * TextureList
 *
 ******************************************************************************/
struct TextureList {
    res::DataBlockHeader blockHeader; // at 0x0
    u16 texNum;                       // at 0x8
    u8 PADDING_0xA[0xC - 0xA];        // at 0xA
};

/******************************************************************************
 *
 * Font
 *
 ******************************************************************************/
struct Font {
    u32 nameStrOffset;         // at 0x0
    u8 type;                   // at 0x4
    u8 PADDING_0x5[0x8 - 0x5]; // at 0x5
};
/******************************************************************************
 *
 * FontList
 *
 ******************************************************************************/
struct FontList {
    res::DataBlockHeader blockHeader; // at 0x0
    u16 fontNum;                      // at 0x8
    u8 PADDING_0xA[0xC - 0xA];        // at 0xA
};

/******************************************************************************
 *
 * MaterialList
 *
 ******************************************************************************/
struct MaterialList {
    res::DataBlockHeader blockHeader; // at 0x0
    u16 materialNum;                  // at 0x8
    u8 PADDING_0xA[0xC - 0xA];        // at 0xA
};

/******************************************************************************
 *
 * TexMap
 *
 ******************************************************************************/
struct TexMap {
    u16 texIdx; // at 0x0
    u8 wrapS;   // at 0x2
    u8 wrapT;   // at 0x3
};

} // namespace res

/******************************************************************************
 *
 * ResBlockSet
 *
 ******************************************************************************/
struct ResBlockSet {
    const res::TextureList* pTextureList;   // at 0x0
    const res::FontList* pFontList;         // at 0x4
    const res::MaterialList* pMaterialList; // at 0x8
    ResourceAccessor* pResAccessor;         // at 0xC
};

/******************************************************************************
 *
 * MaterialResourceNum
 *
 ******************************************************************************/
struct MaterialResourceNum {
    u32 bits;

    u8 GetTexMapNum() const {
        return detail::GetBits(bits, 0, 4);
    }
    u8 GetTexSRTNum() const {
        return detail::GetBits(bits, 4, 4);
    }
    u8 GetTexCoordGenNum() const {
        return detail::GetBits(bits, 8, 4);
    }
    bool HasTevSwapTable() const {
        return detail::TestBit(bits, 12);
    }
    u8 GetIndTexSRTNum() const {
        return detail::GetBits(bits, 13, 2);
    }
    u8 GetIndTexStageNum() const {
        return detail::GetBits(bits, 15, 3);
    }
    u8 GetTevStageNum() const {
        return detail::GetBits(bits, 18, 5);
    }
    bool HasAlphaCompare() const {
        return detail::TestBit(bits, 23);
    }
    bool HasBlendMode() const {
        return detail::TestBit(bits, 24);
    }
    u8 GetChanCtrlNum() const {
        return detail::GetBits(bits, 25, 1);
    }
    u8 GetMatColNum() const {
        return detail::GetBits(bits, 27, 1);
    }
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_resources.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_animation.h" line 6 "nw4r/lyt/lyt_types.h" */
/* end "nw4r/lyt/lyt_types.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_animation.h" line 8 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class Pane;
class Material;
class ResourceAccessor;

namespace res {

/******************************************************************************
 *
 * AnimationBlock
 *
 ******************************************************************************/
struct AnimationBlock {
    DataBlockHeader blockHeader; // at 0x0
    u16 frameSize;               // at 0x8
    u8 loop;                     // at 0xA
    u8 PADDING_0xB;              // at 0xB
    u16 fileNum;                 // at 0xC
    u16 animContNum;             // at 0xE
    u32 animContOffsetsOffset;   // at 0x10
};

/******************************************************************************
 *
 * AnimationTagBlock
 *
 ******************************************************************************/
struct AnimationTagBlock {
    DataBlockHeader blockHeader; // at 0x0
    u16 tagOrder;                // at 0x8
    u16 groupNum;                // at 0xA
    u32 nameOffset;              // at 0xC
    u32 groupsOffset;            // at 0x10
    s16 startFrame;              // at 0x14
    s16 endFrame;                // at 0x16
    u8 flag;                     // at 0x18
    u8 padding[3];               // at 0x19
};

/******************************************************************************
 *
 * AnimationShareBlock
 *
 ******************************************************************************/
struct AnimationShareBlock {
    DataBlockHeader blockHeader; // at 0x0
    u32 animShareInfoOffset;     // at 0x8
    u16 shareNum;                // at 0xC
    u8 padding[2];               // at 0xE
};


/******************************************************************************
 *
 * AnimationInfo
 *
 ******************************************************************************/
struct AnimationInfo {
    u32 kind;                  // at 0x0
    u8 num;                    // at 0x4
    u8 PADDING_0x5[0x8 - 0x5]; // at 0x5

    static const u32 SIGNATURE_ANMPANESRT = FOURCC('R', 'L', 'P', 'A');
    static const u32 SIGNATURE_ANMPANEVIS = FOURCC('R', 'L', 'V', 'I');
    static const u32 SIGNATURE_ANMVTXCLR = FOURCC('R', 'L', 'V', 'C');

    static const u32 SIGNATURE_ANMMATCLR = FOURCC('R', 'L', 'M', 'C');
    static const u32 SIGNATURE_ANMTEXSRT = FOURCC('R', 'L', 'T', 'S');
    static const u32 SIGNATURE_ANMTEXPAT = FOURCC('R', 'L', 'T', 'P');
    static const u32 SIGNATURE_ANMINDTEXSRT = FOURCC('R', 'L', 'I', 'M');
};

/******************************************************************************
 *
 * AnimationContent
 *
 ******************************************************************************/
struct AnimationContent {
    static const int NAME_LEN =
        MAX(NW4R_LYT_RES_NAME_LEN, NW4R_LYT_MATERIAL_NAME_LEN);

    enum AnimType { ANIMTYPE_PANE, ANIMTYPE_MATERIAL, ANIMTYPE_MAX };

    char name[NAME_LEN];          // at 0x0
    u8 num;                       // at 0x14
    u8 type;                      // at 0x15
    u8 PADDING_0x16[0x18 - 0x16]; // at 0x16
};

} // namespace res

/******************************************************************************
 *
 * AnimResource
 *
 ******************************************************************************/
class AnimResource {
protected:
    const res::BinaryFileHeader* mpFileHeader;
    const res::AnimationBlock* mpResBlock;
    const res::AnimationTagBlock* mpTagBlock;
    const res::AnimationShareBlock* mpShareBlock;
};

/******************************************************************************
 *
 * AnimTransform
 *
 ******************************************************************************/
class AnimTransform {
public:
    AnimTransform();
    virtual ~AnimTransform(); // at 0x8

    virtual void SetResource(const res::AnimationBlock* pBlock,
                             ResourceAccessor* pAccessor) = 0; // at 0xC
    virtual void SetResource(const res::AnimationBlock* pBlock,
                             ResourceAccessor* pAccessor,
                             u16 animNum) = 0; // at 0x10

    virtual void Bind(Pane* pPane, bool recursive) = 0; // at 0x14
    virtual void Bind(Material* pMaterial) = 0;         // at 0x18

    virtual void Animate(u32 idx, Pane* pPane) = 0;         // at 0x1C
    virtual void Animate(u32 idx, Material* pMaterial) = 0; // at 0x20

    u16 GetFrameSize() const;

    bool IsLoopData() const;

    f32 GetFrame() const {
        return mFrame;
    }
    void SetFrame(f32 frame) {
        mFrame = frame;
    }

    const res::AnimationBlock* GetAnimResource() const {
        return mpRes;
    }

    f32 GetFrameMax() const {
        return static_cast<f32>(GetFrameSize());
    }

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x4

protected:
    const res::AnimationBlock* mpRes; // at 0xC
    f32 mFrame;                       // at 0x10
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(AnimTransform);

/******************************************************************************
 *
 * AnimTransformBasic
 *
 ******************************************************************************/
class AnimTransformBasic : public AnimTransform {
public:
    AnimTransformBasic();
    virtual ~AnimTransformBasic(); // at 0x8

    virtual void SetResource(const res::AnimationBlock* pBlock,
                             ResourceAccessor* pAccessor); // at 0xC
    virtual void SetResource(const res::AnimationBlock* pBlock,
                             ResourceAccessor* pAccessor,
                             u16 animNum); // at 0x10

    virtual void Bind(Pane* pPane, bool recursive); // at 0x14
    virtual void Bind(Material* pMaterial);         // at 0x18

    virtual void Animate(u32 idx, Pane* pPane);         // at 0x1C
    virtual void Animate(u32 idx, Material* pMaterial); // at 0x20

protected:
    void** mpFileResAry;         // at 0x14
    AnimationLink* mAnimLinkAry; // at 0x18
    u16 mAnimLinkNum;            // at 0x1C
};

/******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/
namespace detail {

AnimationLink* FindAnimationLink(AnimationLinkList* pAnimList,
                                 AnimTransform* pAnimTrans);
AnimationLink* FindAnimationLink(AnimationLinkList* pAnimList,
                                 const AnimResource& rResource);
void UnbindAnimationLink(AnimationLinkList* pAnimList,
                                 AnimTransform* pAnimTrans);

} // namespace detail

/******************************************************************************
 *
 * AnimTargetPane
 *
 ******************************************************************************/
enum AnimTargetPane {
    ANIMTARGET_PANE_TRANSX,
    ANIMTARGET_PANE_TRANSY,
    ANIMTARGET_PANE_TRANSZ,

    ANIMTARGET_PANE_ROTX,
    ANIMTARGET_PANE_ROTY,
    ANIMTARGET_PANE_ROTZ,

    ANIMTARGET_PANE_SCALEX,
    ANIMTARGET_PANE_SCALEY,

    ANIMTARGET_PANE_SIZEX,
    ANIMTARGET_PANE_SIZEY,

    ANIMTARGET_PANE_MAX,

    ANIMTARGET_PANE_COLOR_ALPHA = 16,
    ANIMTARGET_PANE_COLOR_MAX,
};

/******************************************************************************
 *
 * AnimTargetVtxColor
 *
 ******************************************************************************/
enum AnimTargetVtxColor {
    ANIMTARGET_VERTEXCOLOR_LT_RED,
    ANIMTARGET_VERTEXCOLOR_LT_GREEN,
    ANIMTARGET_VERTEXCOLOR_LT_BLUE,
    ANIMTARGET_VERTEXCOLOR_LT_ALPHA,

    ANIMTARGET_VERTEXCOLOR_RT_RED,
    ANIMTARGET_VERTEXCOLOR_RT_GREEN,
    ANIMTARGET_VERTEXCOLOR_RT_BLUE,
    ANIMTARGET_VERTEXCOLOR_RT_ALPHA,

    ANIMTARGET_VERTEXCOLOR_LB_RED,
    ANIMTARGET_VERTEXCOLOR_LB_GREEN,
    ANIMTARGET_VERTEXCOLOR_LB_BLUE,
    ANIMTARGET_VERTEXCOLOR_LB_ALPHA,

    ANIMTARGET_VERTEXCOLOR_RB_RED,
    ANIMTARGET_VERTEXCOLOR_RB_GREEN,
    ANIMTARGET_VERTEXCOLOR_RB_BLUE,
    ANIMTARGET_VERTEXCOLOR_RB_ALPHA,

    ANIMTARGET_VERTEXCOLOR_MAX
};

/******************************************************************************
 *
 * AnimTargetMatColor
 *
 ******************************************************************************/
enum AnimTargetMatColor {
    ANIMTARGET_MATCOLOR_MATR,
    ANIMTARGET_MATCOLOR_MATG,
    ANIMTARGET_MATCOLOR_MATB,
    ANIMTARGET_MATCOLOR_MATA,

    ANIMTARGET_MATCOLOR_TEV0R,
    ANIMTARGET_MATCOLOR_TEV0G,
    ANIMTARGET_MATCOLOR_TEV0B,
    ANIMTARGET_MATCOLOR_TEV0A,

    ANIMTARGET_MATCOLOR_TEV1R,
    ANIMTARGET_MATCOLOR_TEV1G,
    ANIMTARGET_MATCOLOR_TEV1B,
    ANIMTARGET_MATCOLOR_TEV1A,

    ANIMTARGET_MATCOLOR_TEV2R,
    ANIMTARGET_MATCOLOR_TEV2G,
    ANIMTARGET_MATCOLOR_TEV2B,
    ANIMTARGET_MATCOLOR_TEV2A,

    ANIMTARGET_MATCOLOR_TEVK0R,
    ANIMTARGET_MATCOLOR_TEVK0G,
    ANIMTARGET_MATCOLOR_TEVK0B,
    ANIMTARGET_MATCOLOR_TEVK0A,

    ANIMTARGET_MATCOLOR_TEVK1R,
    ANIMTARGET_MATCOLOR_TEVK1G,
    ANIMTARGET_MATCOLOR_TEVK1B,
    ANIMTARGET_MATCOLOR_TEVK1A,

    ANIMTARGET_MATCOLOR_TEVK2R,
    ANIMTARGET_MATCOLOR_TEVK2G,
    ANIMTARGET_MATCOLOR_TEVK2B,
    ANIMTARGET_MATCOLOR_TEVK2A,

    ANIMTARGET_MATCOLOR_TEVK3R,
    ANIMTARGET_MATCOLOR_TEVK3G,
    ANIMTARGET_MATCOLOR_TEVK3B,
    ANIMTARGET_MATCOLOR_TEVK3A,

    ANIMTARGET_MATCOLOR_MAX
};

/******************************************************************************
 *
 * AnimTargetTexSRT
 *
 ******************************************************************************/
enum AnimTargetTexSRT {
    ANIMTARGET_TEXSRT_TRANSX,
    ANIMTARGET_TEXSRT_TRANSY,

    ANIMTARGET_TEXSRT_ROT,

    ANIMTARGET_TEXSRT_SCALEX,
    ANIMTARGET_TEXSRT_SCALEY,

    ANIMTARGET_TEXSRT_MAX
};

/******************************************************************************
 *
 * AnimTargetTexPat
 *
 ******************************************************************************/
enum AnimTargetTexPat {
    ANIMTARGET_TEXPATTURN_IMAGE,

    ANIMTARGET_TEXPATTURN_MAX
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_animation.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 4 "nw4r/lyt/lyt_arcResourceAccessor.h" */
#ifndef NW4R_LYT_ARC_RESOURCE_ACCESSOR_H
#define NW4R_LYT_ARC_RESOURCE_ACCESSOR_H
/* "libs/nw4r/include/nw4r/lyt/lyt_arcResourceAccessor.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_arcResourceAccessor.h" line 4 "nw4r/lyt/lyt_resourceAccessor.h" */
#ifndef NW4R_LYT_RESOURCE_ACCESSOR_H
#define NW4R_LYT_RESOURCE_ACCESSOR_H
/* "libs/nw4r/include/nw4r/lyt/lyt_resourceAccessor.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_resourceAccessor.h" line 4 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

class ResourceAccessor {
public:
    ResourceAccessor();
    virtual ~ResourceAccessor(); // at 0x8

    virtual void* GetResource(u32 type, const char* pName,
                              u32* pSize) = 0; // at 0xC

    virtual ut::Font* GetFont(const char* pName); // at 0x10
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_resourceAccessor.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_arcResourceAccessor.h" line 6 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_arcResourceAccessor.h" line 8 "revolution/ARC.h" */
#ifndef RVL_SDK_PUBLIC_ARC_H
#define RVL_SDK_PUBLIC_ARC_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/ARC.h" line 6 "revolution/ARC/arc.h" */
/**
 * Modified from decompilation by riidefi in WiiCore
 */

#ifndef RVL_SDK_ARC_H
#define RVL_SDK_ARC_H
/* "libs/RVL_SDK/include/revolution/ARC/arc.h" line 6 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ARCHandle {
    void* archiveStartAddr; // at 0x0
    void* FSTStart;         // at 0x4
    void* fileStart;        // at 0x8
    u32 entryNum;           // at 0xC
    char* FSTStringStart;   // at 0x10
    u32 FSTLength;          // at 0x14
    s32 currDir;            // at 0x18
} ARCHandle;

typedef struct ARCFileInfo {
    ARCHandle* handle; // at 0x0
    u32 startOffset;   // at 0x4
    u32 length;        // at 0x8
} ARCFileInfo;

typedef struct ARCDir {
    ARCHandle* handle; // at 0x0
    u32 entryNum;      // at 0x4
    u32 location;      // at 0x8
    u32 next;          // at 0xC
} ARCDir;

typedef struct ARCDirEntry {
    ARCHandle* handle; // at 0x0
    u32 entryNum;      // at 0x4
    BOOL isDir;        // at 0x8
    char* name;        // at 0xC
} ARCDirEntry;

BOOL ARCGetCurrentDir(ARCHandle* handle, char* string, u32 maxlen);
BOOL ARCInitHandle(void* bin, ARCHandle* handle);
BOOL ARCOpen(ARCHandle* handle, const char* path, ARCFileInfo* info);
BOOL ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* info);
s32 ARCConvertPathToEntrynum(ARCHandle* handle, const char* path);
void* ARCGetStartAddrInMem(ARCFileInfo* info);
u32 ARCGetStartOffset(ARCFileInfo* info);
u32 ARCGetLength(ARCFileInfo* info);
BOOL ARCClose(ARCFileInfo* info);
BOOL ARCChangeDir(ARCHandle* handle, const char* path);
BOOL ARCOpenDir(ARCHandle* handle, const char* path, ARCDir* dir);
BOOL ARCReadDir(ARCDir* dir, ARCDirEntry* entry);
BOOL ARCCloseDir(ARCDir* dir);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/ARC/arc.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/ARC.h" */

namespace nw4r {
namespace lyt {

/******************************************************************************
 *
 * FontRefLink
 *
 ******************************************************************************/
class FontRefLink {
public:
    const char* GetFontName() const {
        return mFontName;
    }

    ut::Font* GetFont() const {
        return mpFont;
    }

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x0

protected:
    static const int FONTNAMEBUF_MAX = 128;

protected:
    char mFontName[FONTNAMEBUF_MAX]; // at 0x8
    ut::Font* mpFont;                // at 0x88
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(FontRefLink);

/******************************************************************************
 *
 * ArcResourceAccessor
 *
 ******************************************************************************/
class ArcResourceAccessor : public ResourceAccessor {
public:
    static const u32 RES_TYPE_NAME = 0;
    static const u32 RES_TYPE_ANIMATION = FOURCC('a', 'n', 'i', 'm');
    static const u32 RES_TYPE_LAYOUT = FOURCC('b', 'l', 'y', 't');
    static const u32 RES_TYPE_FONT = FOURCC('f', 'o', 'n', 't');
    static const u32 RES_TYPE_TEXTURE = FOURCC('t', 'i', 'm', 'g');

public:
    ArcResourceAccessor();

    virtual void* GetResource(u32 type, const char* pName,
                              u32* pSize); // at 0xC

    virtual ut::Font* GetFont(const char* pName); // at 0x10

    bool Attach(void* pArchive, const char* pRootDir);
    void* Detach();

    bool IsAttached() const {
        return mArcBuf != NULL;
    }

protected:
    static const int ROOTPATH_MAX = 128;

protected:
    ARCHandle mArcHandle;           // at 0x4
    void* mArcBuf;                  // at 0x20
    FontRefLinkList mFontList;      // at 0x24
    char mResRootDir[ROOTPATH_MAX]; // at 0x30
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_arcResourceAccessor.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 5 "nw4r/lyt/lyt_bounding.h" */
#ifndef NW4R_LYT_BOUNDING_H
#define NW4R_LYT_BOUNDING_H
/* "libs/nw4r/include/nw4r/lyt/lyt_bounding.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_bounding.h" line 4 "nw4r/lyt/lyt_pane.h" */
#ifndef NW4R_LYT_PANE_H
#define NW4R_LYT_PANE_H
/* "libs/nw4r/include/nw4r/lyt/lyt_pane.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_pane.h" line 4 "nw4r/lyt/lyt_animation.h" */
/* end "nw4r/lyt/lyt_animation.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_pane.h" line 5 "nw4r/lyt/lyt_common.h" */
/* end "nw4r/lyt/lyt_common.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_pane.h" line 6 "nw4r/lyt/lyt_resources.h" */
/* end "nw4r/lyt/lyt_resources.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_pane.h" line 7 "nw4r/lyt/lyt_types.h" */
/* end "nw4r/lyt/lyt_types.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_pane.h" line 8 "nw4r/math.h" */
/* end "nw4r/math.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_pane.h" line 9 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class DrawInfo;
class Material;
class AnimResource;
class ExtUserDataList;

/******************************************************************************
 *
 * AnimOption
 *
 ******************************************************************************/
enum AnimOption {
    ANIMOPTION_SKIP_INVISIBLE = (1 << 0),
};

namespace detail {

/******************************************************************************
 *
 * PaneBase
 *
 ******************************************************************************/
class PaneBase {
public:
    PaneBase();
    virtual ~PaneBase(); // at 0x8

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x4
};

} // namespace detail

namespace res {

/******************************************************************************
 *
 * PAN1 binary layout
 *
 ******************************************************************************/
struct Pane {
    static const u32 SIGNATURE = FOURCC('p', 'a', 'n', '1');

    DataBlockHeader blockHeader;               // at 0x0
    u8 flag;                                   // at 0x8
    u8 basePosition;                           // at 0x9
    u8 alpha;                                  // at 0xA
    u8 padding;                                // at 0xB
    char name[NW4R_LYT_RES_NAME_LEN];          // at 0xC
    char userData[NW4R_LYT_PANE_USERDATA_LEN]; // at 0x1C
    math::VEC3 translate;                      // at 0x24
    math::VEC3 rotate;                         // at 0x30
    math::VEC2 scale;                          // at 0x3C
    Size size;                                 // at 0x44
};

} // namespace res

/******************************************************************************
 *
 * Pane
 *
 ******************************************************************************/
class Pane : public detail::PaneBase {
public:
    NW4R_UT_RTTI_DECL(Pane);

private:
    enum {
        BIT_VISIBLE,
        BIT_INFLUENCED_ALPHA,
        BIT_LOCATION_ADJUST,
    };

    // Need the typedef before the class definition is complete
    typedef ut::LinkList<Pane, offsetof(PaneBase, node)> PaneList;

public:
    Pane();
    explicit Pane(const res::Pane* pRes);
    virtual ~Pane(); // at 0x8

    virtual void CalculateMtx(const DrawInfo& rInfo); // at 0x10
    void CalculateMtxChild(const DrawInfo& rInfo);

    virtual void Draw(const DrawInfo& rInfo);     // at 0x14
    virtual void DrawSelf(const DrawInfo& rInfo); // at 0x18

    virtual void Animate(u32 option = 0);     // at 0x1C
    virtual void AnimateSelf(u32 option = 0); // at 0x20

    virtual ut::Color GetVtxColor(u32 idx) const;       // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color color); // at 0x28
    virtual u8 GetColorElement(u32 idx) const;          // at 0x2C
    virtual void SetColorElement(u32 idx, u8 value);    // at 0x30
    virtual u8 GetVtxColorElement(u32 idx) const;       // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value); // at 0x38

    virtual Pane* FindPaneByName(const char* pName, bool recursive); // at 0x3C
    virtual Material* FindMaterialByName(const char* pName,
                                         bool recursive); // at 0x40

    virtual void BindAnimation(AnimTransform* pAnimTrans,
                               bool recursive, bool disable); // at 0x44
    virtual void UnbindAnimation(AnimTransform* pAnimTrans,
                                 bool recursive); // at 0x48

    virtual void UnbindAllAnimation(bool recursive);             // at 0x4C
    virtual void UnbindAnimationSelf(AnimTransform* pAnimTrans); // at 0x50

    virtual AnimationLink*
    FindAnimationLinkSelf(AnimTransform* pAnimTrans); // at 0x54
    virtual AnimationLink*
    FindAnimationLinkSelf(const AnimResource& rResource); // at 0x58
    
    virtual void
    SetAnimationEnable(AnimTransform* pAnimTrans, bool enable, bool recursive); // at 0x5C
    virtual void
    SetAnimationEnable(const AnimResource& rResource, bool enable, bool recursive); // at 0x60
    
    virtual u8 GetMaterialNum() const; // at 0x64
    virtual Material* GetMaterial() const;       // at 0x68
    virtual Material* GetMaterial(u32 index) const;  // at 0x6C
    virtual void LoadMtx(const DrawInfo& rInfo); // at 0x70

    void AppendChild(Pane* pChild);
    void PrependChild(Pane* pChild);
    void RemoveChild(Pane* pChild);

    ut::Rect GetPaneRect(const DrawInfo& rInfo) const;

    void AddAnimationLink(AnimationLink* pAnimLink);

    math::VEC2 GetVtxPos() const;

    Pane* GetParent() const {
        return mpParent;
    }

    PaneList& GetChildList() {
        return mChildList;
    }

    void SetSRTElement(u32 idx, f32 value) {
        reinterpret_cast<f32*>(&mTranslate)[idx] = value;
    }

    const math::VEC3& GetTranslate() const {
        return mTranslate;
    }
    void SetTranslate(const math::VEC2& rTransXY) {
        SetTranslate(math::VEC3(rTransXY.x, rTransXY.y, 0.0f));
    }
    void SetTranslate(const math::VEC3& rTrans) {
        mTranslate = rTrans;
    }

    const math::VEC3& GetRotate() const {
        return mRotate;
    }
    void SetRotate(const math::VEC3& rRotate) {
        mRotate = rRotate;
    }

    const math::VEC2& GetScale() const {
        return mScale;
    }
    void SetScale(const math::VEC2& rScale) {
        mScale = rScale;
    }

    const Size& GetSize() const {
        return mSize;
    }
    void SetSize(const Size& rSize) {
        mSize = rSize;
    }

    const math::MTX34& GetMtx() const {
        return mMtx;
    }
    void SetMtx(const math::MTX34& rMtx) {
        mMtx = rMtx;
    }

    const math::MTX34& GetGlobalMtx() const {
        return mGlbMtx;
    }
    void SetGlobalMtx(const math::MTX34& rGlbMtx) {
        mGlbMtx = rGlbMtx;
    }

    u8 GetAlpha() const {
        return mAlpha;
    }
    void SetAlpha(u8 alpha) {
        mAlpha = alpha;
    }

    u8 GetGlbAlpha() const {
        return mGlbAlpha;
    }
    void SetGlbAlpha(u8 alpha) {
        mGlbAlpha = alpha;
    }

    u8 GetBasePositionH() const {
        return detail::GetHorizontalPosition(mBasePosition);
    }
    void SetBasePositionH(u8 position) {
        detail::SetHorizontalPosition(&mBasePosition, position);
    }

    u8 GetBasePositionV() const {
        return detail::GetVerticalPosition(mBasePosition);
    }
    void SetBasePositionV(u8 position) {
        detail::SetVerticalPosition(&mBasePosition, position);
    }

    bool IsVisible() const {
        return detail::TestBit(mFlag, BIT_VISIBLE);
    }
    void SetVisible(bool visible) {
        detail::SetBit(&mFlag, BIT_VISIBLE, visible);
    }

    bool IsInfluencedAlpha() const {
        return detail::TestBit(mFlag, BIT_INFLUENCED_ALPHA);
    }
    void SetInfluencedAlpha(bool influenced) {
        detail::SetBit(&mFlag, BIT_INFLUENCED_ALPHA, influenced);
    }

    bool IsLocationAdjust() const {
        return detail::TestBit(mFlag, BIT_LOCATION_ADJUST);
    }
    void SetLocationAdjust(bool adjust) {
        detail::SetBit(&mFlag, BIT_LOCATION_ADJUST, adjust);
    }

    const char* GetName() const {
        return mName;
    }
    void SetName(const char* pName);

    const char* GetUserData() const {
        return mUserData;
    }
    void SetUserData(const char* pUserData);

    bool IsUserAllocated() const {
        return mbUserAllocated;
    }

protected:
    Pane* mpParent;              // at 0xC
    PaneList mChildList;         // at 0x10
    AnimationLinkList mAnimList; // at 0x1C
    Material* mpMaterial;        // at 0x28

    math::VEC3 mTranslate; // at 0x2C
    math::VEC3 mRotate;    // at 0x38
    math::VEC2 mScale;     // at 0x44
    Size mSize;            // at 0x4C

    math::MTX34 mMtx;    // at 0x54
    math::MTX34 mGlbMtx; // at 0x84
    
    const ExtUserDataList* mpExtUserDataList; //at 0xB4

    u8 mAlpha;        // at 0xB8
    u8 mGlbAlpha;     // at 0xB9
    u8 mBasePosition; // at 0xBA
    u8 mFlag;         // at 0xBB

    char mName[NW4R_LYT_RES_NAME_LEN + 1];          // at 0xBC
    char mUserData[NW4R_LYT_PANE_USERDATA_LEN + 1]; // at 0xCD

    bool mbUserAllocated; // at 0xD6
    u8 mPadding;          // at 0xD7

protected:
    void InsertChild(PaneList::Iterator next, Pane* pChild);

private:
    void Init();
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(Pane);

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_pane.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_bounding.h" line 6 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
struct ResBlockSet;

namespace res {

/******************************************************************************
 *
 * BND1 binary layout
 *
 ******************************************************************************/
struct Bounding : Pane {
    static const u32 SIGNATURE = FOURCC('b', 'n', 'd', '1');
};

} // namespace res

/******************************************************************************
 *
 * Bounding
 *
 ******************************************************************************/
class Bounding : public Pane {
public:
    NW4R_UT_RTTI_DECL(Bounding);

public:
    Bounding(const res::Bounding* pRes, const ResBlockSet& rBlockSet);
    virtual ~Bounding(); // at 0x8

    virtual void DrawSelf(const DrawInfo& rInfo); // at 0x18
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_bounding.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 6 "nw4r/lyt/lyt_common.h" */
/* end "nw4r/lyt/lyt_common.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 7 "nw4r/lyt/lyt_drawInfo.h" */
#ifndef NW4R_LYT_DRAW_INFO_H
#define NW4R_LYT_DRAW_INFO_H
/* "libs/nw4r/include/nw4r/lyt/lyt_drawInfo.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_drawInfo.h" line 4 "nw4r/math.h" */
/* end "nw4r/math.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_drawInfo.h" line 5 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

class DrawInfo {
public:
    DrawInfo();
    virtual ~DrawInfo(); // at 0x8

    bool IsYAxisUp() const {
        return mViewRect.bottom - mViewRect.top < 0.0f;
    }

    const math::MTX34& GetViewMtx() const {
        return mViewMtx;
    }
    void SetViewMtx(const math::MTX34& rViewMtx) {
        mViewMtx = rViewMtx;
    }

    const ut::Rect& GetViewRect() const {
        return mViewRect;
    }
    void SetViewRect(const ut::Rect& rViewRect) {
        mViewRect = rViewRect;
    }

    const math::VEC2& GetLocationAdjustScale() const {
        return mLocationAdjustScale;
    }
    void SetLocationAdjustScale(const math::VEC2& rScale) {
        mLocationAdjustScale = rScale;
    }

    f32 GetGlobalAlpha() const {
        return mGlobalAlpha;
    }
    void SetGlobalAlpha(f32 alpha) {
        mGlobalAlpha = alpha;
    }

    bool IsMultipleViewMtxOnDraw() const {
        return mFlag.mulViewDraw;
    }

    bool IsInfluencedAlpha() const {
        return mFlag.influencedAlpha;
    }
    void SetInfluencedAlpha(bool influenced) {
        mFlag.influencedAlpha = influenced;
    }

    bool IsLocationAdjust() const {
        return mFlag.locationAdjust;
    }
    void SetLocationAdjust(bool adjust) {
        mFlag.locationAdjust = adjust;
    }

    bool IsInvisiblePaneCalculateMtx() const {
        return mFlag.invisiblePaneCalculateMtx;
    }
    void SetInvisiblePaneCalculateMtx(bool calc) {
        mFlag.invisiblePaneCalculateMtx = calc;
    }

    bool IsDebugDrawMode() const {
        return mFlag.debugDrawMode;
    }
    void SetDebugDrawMode(bool debug) {
        mFlag.debugDrawMode = debug;
    }

protected:
    math::MTX34 mViewMtx;            // at 0x4
    ut::Rect mViewRect;              // at 0x34
    math::VEC2 mLocationAdjustScale; // at 0x44
    f32 mGlobalAlpha;                // at 0x4C

    struct {
        u8 mulViewDraw : 1;
        u8 influencedAlpha : 1;
        u8 locationAdjust : 1;
        u8 invisiblePaneCalculateMtx : 1;
        u8 debugDrawMode : 1;
    } mFlag; // at 0x50
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_drawInfo.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 8 "nw4r/lyt/lyt_group.h" */
#ifndef NW4R_LYT_GROUP_H
#define NW4R_LYT_GROUP_H
/* "libs/nw4r/include/nw4r/lyt/lyt_group.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_group.h" line 4 "nw4r/lyt/lyt_resources.h" */
/* end "nw4r/lyt/lyt_resources.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_group.h" line 6 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class Pane;

namespace detail {

/******************************************************************************
 *
 * PaneLink
 *
 ******************************************************************************/
struct PaneLink {
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x4
    Pane* mTarget;                // at 0x8
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(PaneLink);

} // namespace detail

namespace res {

/******************************************************************************
 *
 * GRP1 binary layout
 *
 ******************************************************************************/
struct Group {
    static const u32 SIGNATURE = FOURCC('g', 'r', 'p', '1');

    DataBlockHeader blockHeader;      // at 0x0
    char name[NW4R_LYT_RES_NAME_LEN]; // at 0x8
    u16 paneNum;                      // at 0x18
    u8 PADDING_0x1A[0x1C - 0x1A];     // at 0x1A
};

} // namespace res

/******************************************************************************
 *
 * Group
 *
 ******************************************************************************/
class Group {
public:
    Group(const res::Group* pRes, Pane* pRootPane);
    virtual ~Group(); // at 0x8

    void AppendPane(Pane* pPane);

    detail::PaneLinkList& GetPaneList() {
        return mPaneLinkList;
    }

    const char* GetName() const {
        return mName;
    }

    bool IsUserAllocated() const {
        return mbUserAllocated;
    }

public:
    NW4R_UT_LINKLIST_NODE_DECL(); // at 0x4

protected:
    detail::PaneLinkList mPaneLinkList;    // at 0xC
    char mName[NW4R_LYT_RES_NAME_LEN + 1]; // at 0x18
    bool mbUserAllocated;                  // at 0x29
    u8 PADDING_0x2A[0x2C - 0x2A];          // at 0x2A

private:
    void Init();
};

NW4R_UT_LINKLIST_TYPEDEF_DECL(Group);

/******************************************************************************
 *
 * GroupContainer
 *
 ******************************************************************************/
class GroupContainer {
public:
    GroupContainer() {}
    ~GroupContainer();

    void AppendGroup(Group* pGroup);
    Group* FindGroupByName(const char* pName);

    GroupList& GetGroupList() {
        return mGroupList;
    }

protected:
    GroupList mGroupList; // at 0x0
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_group.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 9 "nw4r/lyt/lyt_init.h" */
#ifndef NW4R_LYT_INIT_H
#define NW4R_LYT_INIT_H
/* "libs/nw4r/include/nw4r/lyt/lyt_init.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

namespace nw4r {
namespace lyt {

void LytInit();

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_init.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 10 "nw4r/lyt/lyt_layout.h" */
#ifndef NW4R_LYT_LAYOUT_H
#define NW4R_LYT_LAYOUT_H
/* "libs/nw4r/include/nw4r/lyt/lyt_layout.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_layout.h" line 4 "nw4r/lyt/lyt_animation.h" */
/* end "nw4r/lyt/lyt_animation.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_layout.h" line 5 "nw4r/lyt/lyt_types.h" */
/* end "nw4r/lyt/lyt_types.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_layout.h" line 7 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_layout.h" line 9 "revolution/MEM.h" */
#ifndef RVL_SDK_PUBLIC_MEM_H
#define RVL_SDK_PUBLIC_MEM_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/MEM.h" line 6 "revolution/MEM/mem_allocator.h" */
#ifndef RVL_SDK_MEM_ALLOCATOR_H
#define RVL_SDK_MEM_ALLOCATOR_H
/* "libs/RVL_SDK/include/revolution/MEM/mem_allocator.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct MEMAllocator;
typedef struct MEMiHeapHead;

typedef void* (*MEMAllocatorAllocFunc)(struct MEMAllocator* allocator, u32 size);
typedef void (*MEMAllocatorFreeFunc)(struct MEMAllocator* allocator, void* block);

typedef struct MEMAllocatorFuncs {
    MEMAllocatorAllocFunc allocFunc; // at 0x0
    MEMAllocatorFreeFunc freeFunc;   // at 0x4
} MEMAllocatorFuncs;

typedef struct MEMAllocator {
    const MEMAllocatorFuncs* funcs; // at 0x0
    struct MEMiHeapHead* heap;             // at 0x4
    u32 heapParam1;                 // at 0x8
    u32 heapParam2;                 // at 0xC
} MEMAllocator;

void* MEMAllocFromAllocator(MEMAllocator* allocator, u32 size);
void MEMFreeToAllocator(MEMAllocator* allocator, void* block);

void MEMInitAllocatorForExpHeap(MEMAllocator* allocator, struct MEMiHeapHead* heap,
                                s32 align);
void MEMInitAllocatorForFrmHeap(MEMAllocator* allocator, struct MEMiHeapHead* heap,
                                s32 align);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MEM/mem_allocator.h" */
/* "libs/RVL_SDK/include/revolution/MEM.h" line 7 "revolution/MEM/mem_expHeap.h" */
#ifndef RVL_SDK_MEM_EXP_HEAP_H
#define RVL_SDK_MEM_EXP_HEAP_H
/* "libs/RVL_SDK/include/revolution/MEM/mem_expHeap.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define MEM_EXP_HEAP_MIN_SIZE                                                  \
    (sizeof(MEMiHeapHead) + sizeof(MEMiExpHeapHead) +                          \
     sizeof(MEMiExpHeapMBlock) + 4)

// Forward declarations
typedef struct MEMiHeapHead;

typedef enum {
    MEM_EXP_HEAP_ALLOC_FAST, //!< When allocating memory blocks, take the first
                             //!< usable found block rather than trying to
                             //!< find a more optimal block
} MEMiExpHeapAllocMode;

typedef struct MEMiExpHeapMBlock {
    u16 state; // at 0x0
    union {
        u16 settings;
        struct {
            u16 allocDir : 1;
            u16 align : 7;
            u16 group : 8;
        };
    }; // at 0x2
    u32 size;                       // at 0x4
    struct MEMiExpHeapMBlock* prev; // at 0x8
    struct MEMiExpHeapMBlock* next; // at 0xC
} MEMiExpHeapMBlock;

typedef struct MEMiExpHeapMBlockList {
    MEMiExpHeapMBlock* head; // at 0x0
    MEMiExpHeapMBlock* tail; // at 0x4
} MEMiExpHeapMBlockList;

// Placed in heap after base heap head
typedef struct MEMiExpHeapHead {
    MEMiExpHeapMBlockList freeMBlocks; // at 0x0
    MEMiExpHeapMBlockList usedMBlocks; // at 0x8
    u16 group;                         // at 0x10
    union {
        u16 SHORT_0x12;
        struct {
            u16 SHORT_0x12_0_15 : 15;
            u16 allocMode : 1;
        };
    }; // at 0x12
} MEMiExpHeapHead;

struct MEMiHeapHead* MEMCreateExpHeapEx(void* start, u32 size, u16 opt);
struct MEMiHeapHead* MEMDestroyExpHeap(struct MEMiHeapHead* heap);
void* MEMAllocFromExpHeapEx(struct MEMiHeapHead* heap, u32 size, s32 align);
u32 MEMResizeForMBlockExpHeap(struct MEMiHeapHead* heap, void* memBlock, u32 size);
void MEMFreeToExpHeap(struct MEMiHeapHead* heap, void* memBlock);
u32 MEMGetAllocatableSizeForExpHeapEx(struct MEMiHeapHead* heap, s32 align);
u32 MEMAdjustExpHeap(struct MEMiHeapHead* heap);

static struct MEMiHeapHead* MEMCreateExpHeap(void* start, u32 size) {
    return MEMCreateExpHeapEx(start, size, 0);
}

static void* MEMAllocFromExpHeap(struct MEMiHeapHead* heap, u32 size) {
    return MEMAllocFromExpHeapEx(heap, size, 4);
}

static u32 MEMGetAllocatableSizeForExpHeap(struct MEMiHeapHead* heap) {
    return MEMGetAllocatableSizeForExpHeapEx(heap, 4);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MEM/mem_expHeap.h" */
/* "libs/RVL_SDK/include/revolution/MEM.h" line 8 "revolution/MEM/mem_frameHeap.h" */
#ifndef RVL_SDK_MEM_FRAME_HEAP_H
#define RVL_SDK_MEM_FRAME_HEAP_H
/* "libs/RVL_SDK/include/revolution/MEM/mem_frameHeap.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

#define MEM_FRM_HEAP_MIN_SIZE (sizeof(MEMiHeapHead) + sizeof(MEMiFrmHeapHead))

// Forward declarations
typedef struct MEMiHeapHead;

// Specify how to free memory
typedef enum {
    MEM_FRM_HEAP_FREE_TO_HEAD = (1 << 0),
    MEM_FRM_HEAP_FREE_TO_TAIL = (1 << 1),
    MEM_FRM_HEAP_FREE_ALL =
        MEM_FRM_HEAP_FREE_TO_HEAD | MEM_FRM_HEAP_FREE_TO_TAIL
} MEMiFrmFreeFlag;

typedef struct MEMiFrmHeapState {
    u32 id;                        // at 0x0
    u8* head;                      // at 0x4
    u8* tail;                      // at 0x8
    struct MEMiFrmHeapState* next; // at 0xC
} MEMiFrmHeapState;

// Placed in heap after base heap head
typedef struct MEMiFrmHeapHead {
    u8* head;                 // at 0x0
    u8* tail;                 // at 0x4
    MEMiFrmHeapState* states; // at 0x8
} MEMiFrmHeapHead;

struct MEMiHeapHead* MEMCreateFrmHeapEx(void* start, u32 size, u16 opt);
struct MEMiHeapHead* MEMDestroyFrmHeap(struct MEMiHeapHead* heap);
void* MEMAllocFromFrmHeapEx(struct MEMiHeapHead* heap, u32 size, s32 align);
void MEMFreeToFrmHeap(struct MEMiHeapHead* heap, u32 flags);
u32 MEMGetAllocatableSizeForFrmHeapEx(struct MEMiHeapHead* heap, s32 align);
BOOL MEMRecordStateForFrmHeap(struct MEMiHeapHead* heap, u32 id);
BOOL MEMFreeByStateToFrmHeap(struct MEMiHeapHead* heap, u32 id);
u32 MEMAdjustFrmHeap(struct MEMiHeapHead* heap);
u32 MEMResizeForMBlockFrmHeap(struct MEMiHeapHead* heap, void* memBlock, u32 size);

static struct MEMiHeapHead* MEMCreateFrmHeap(void* start, u32 size) {
    return MEMCreateFrmHeapEx(start, size, 0);
}

static void* MEMAllocFromFrmHeap(struct MEMiHeapHead* heap, u32 size) {
    return MEMAllocFromFrmHeapEx(heap, size, 4);
}

static u32 MEMGetAllocatableSizeForFrmHeap(struct MEMiHeapHead* heap) {
    return MEMGetAllocatableSizeForFrmHeapEx(heap, 4);
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MEM/mem_frameHeap.h" */
/* "libs/RVL_SDK/include/revolution/MEM.h" line 9 "revolution/MEM/mem_heapCommon.h" */
#ifndef RVL_SDK_MEM_HEAP_COMMON_H
#define RVL_SDK_MEM_HEAP_COMMON_H
/* "libs/RVL_SDK/include/revolution/MEM/mem_heapCommon.h" line 2 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/MEM/mem_heapCommon.h" line 4 "revolution/MEM/mem_list.h" */
#ifndef RVL_SDK_MEM_LIST_H
#define RVL_SDK_MEM_LIST_H
/* "libs/RVL_SDK/include/revolution/MEM/mem_list.h" line 2 "types.h" */
/* end "types.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MEMList {
    void* head; // at 0x0
    void* tail; // at 0x4
    u16 length; // at 0x8
    u16 offset; // at 0xA
} MEMList;

typedef struct MEMLink {
    void* prev; // at 0x0
    void* next; // at 0x4
} MEMLink;

void MEMInitList(MEMList* list, u16 offset);
void MEMAppendListObject(MEMList* list, void* object);
void MEMRemoveListObject(MEMList* list, void* object);
void* MEMGetNextListObject(MEMList* list, void* object);

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MEM/mem_list.h" */

/* "libs/RVL_SDK/include/revolution/MEM/mem_heapCommon.h" line 6 "revolution/OS.h" */
/**
 * References: YAGCD, WiiBrew, Dolphin Emulator
 */

#ifndef RVL_SDK_PUBLIC_OS_H
#define RVL_SDK_PUBLIC_OS_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/OS.h" line 10 "revolution/OS/OS.h" */
/* end "revolution/OS/OS.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 11 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 12 "revolution/OS/OSAlarm.h" */
/* end "revolution/OS/OSAlarm.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 13 "revolution/OS/OSAlloc.h" */
/* end "revolution/OS/OSAlloc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 14 "revolution/OS/OSArena.h" */
/* end "revolution/OS/OSArena.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 15 "revolution/OS/OSAudioSystem.h" */
/* end "revolution/OS/OSAudioSystem.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 16 "revolution/OS/OSCache.h" */
/* end "revolution/OS/OSCache.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 17 "revolution/OS/OSContext.h" */
/* end "revolution/OS/OSContext.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 18 "revolution/OS/OSCrc.h" */
/* end "revolution/OS/OSCrc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 19 "revolution/OS/OSError.h" */
/* end "revolution/OS/OSError.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 20 "revolution/OS/OSExec.h" */
/* end "revolution/OS/OSExec.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 21 "revolution/OS/OSFastCast.h" */
/* end "revolution/OS/OSFastCast.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 22 "revolution/OS/OSFatal.h" */
/* end "revolution/OS/OSFatal.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 23 "revolution/OS/OSFont.h" */
/* end "revolution/OS/OSFont.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 24 "revolution/OS/OSHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap4.html#sec4
 * https://www.gc-forever.com/yagcd/chap13.html#sec13
 * https://wiibrew.org/wiki/Memory_map
 */

#ifndef RVL_SDK_OS_HARDWARE_H
#define RVL_SDK_OS_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 9 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 11 "revolution/DVD/dvd.h" */
/* end "revolution/DVD/dvd.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 12 "revolution/OS/OSAddress.h" */
/* end "revolution/OS/OSAddress.h" */
/* "libs/RVL_SDK/include/revolution/OS/OSHardware.h" line 13 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;
typedef struct OSExecParams;

// Derive offsets for use with OSAddress functions
#define __DEF_ADDR_OFFSETS(name, addr)                                         \
    static const u32 OS_PHYS_##name = (addr) - 0x80000000;                     \
    static const u32 OS_CACHED_##name = (addr);                                \
    static const u32 OS_UNCACHED_##name = (addr) + (0xC0000000 - 0x80000000);

// Define a global variable in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_VAR(type, name, addr)                                    \
    /* Memory-mapped value for direct access */                                \
    type OS_##name DECL_ADDRESS(addr);                                         \
    __DEF_ADDR_OFFSETS(name, addr)

// Define a global array in *CACHED* MEM1.
// Can be accessed directly or with OSAddress functions.
#define OS_DEF_GLOBAL_ARR(type, name, arr, addr)                               \
    /* Memory-mapped value for direct access */                                \
    type OS_##name arr DECL_ADDRESS(addr);                                     \
    __DEF_ADDR_OFFSETS(name, addr)

// Define an global variable in the hardware-register range.
#define OS_DEF_HW_REG(type, name, addr)                                        \
    /* Memory-mapped value for direct access */                                \
    type OS_##name : (addr);

typedef enum {
    OS_BOOT_MAGIC_BOOTROM = 0xD15EA5E,
    OS_BOOT_MAGIC_JTAG = 0xE5207C22,
} OSBootMagic;

typedef struct OSBootInfo {
    DVDDiskID diskID; // at 0x0
    u32 bootMagic;    // at 0x20
    u32 aplVersion;   // at 0x24
    u32 physMemSize;  // at 0x28
    u32 consoleType;  // at 0x2C
    void* arenaLo;    // at 0x30
    void* arenaHi;    // at 0x34
    void* fstStart;   // at 0x38
    u32 fstSize;      // at 0x3C
} OSBootInfo;

typedef struct OSDebugInterface {
    BOOL usingDebugger;    // at 0x0
    u32 exceptionMask;     // at 0x4
    void* exceptionHook;   // at 0x8
    void* exceptionHookLR; // at 0xC
} OSDebugInterface;

typedef struct OSBI2 {
    u32 dbgMonitorSize;   // at 0x0
    u32 simulatedMemSize; // at 0x4
    u32 argumentOfs;      // at 0x8
    u32 debugFlag;        // at 0xC
    u32 trackLocation;    // at 0x10
    u32 trackSize;        // at 0x14
    u32 countryCode;      // at 0x18
    u32 WORD_0x1C;
    u32 lastInsert;
    u32 padSpec;            // at 0x24
    u32 totalTextDataLimit; // at 0x28
    u32 simulatedMem2Size;  // at 0x2C
} OSBI2;

/**
 * 0x80000000 - 0x80000100
 */
// clang-format off
OS_DEF_GLOBAL_VAR(OSBootInfo, BOOT_INFO,                   0x80000000);
OS_DEF_GLOBAL_VAR(OSDebugInterface, DEBUG_INTERFACE,       0x80000040);
OS_DEF_GLOBAL_ARR(u8, DB_INTEGRATOR_HOOK, [0x24],          0x80000060);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT_PHYS,        0x800000C0);
OS_DEF_GLOBAL_VAR(u32, PREV_INTR_MASK,                     0x800000C4);
OS_DEF_GLOBAL_VAR(u32, CURRENT_INTR_MASK,                  0x800000C8);
OS_DEF_GLOBAL_VAR(u32, TV_FORMAT,                          0x800000CC);
OS_DEF_GLOBAL_VAR(u32, ARAM_SIZE,                          0x800000D0);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_CONTEXT,             0x800000D4);
OS_DEF_GLOBAL_VAR(OSContext*, CURRENT_FPU_CONTEXT,         0x800000D8);
OS_DEF_GLOBAL_VAR(OSThreadQueue, THREAD_QUEUE,             0x800000DC);
OS_DEF_GLOBAL_VAR(OSThread*, CURRENT_THREAD,               0x800000E4);
OS_DEF_GLOBAL_VAR(u32, DEBUG_MONITOR_SIZE,                 0x800000E8);
OS_DEF_GLOBAL_VAR(void*, DEBUG_MONITOR,                    0x800000EC);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM_SIZE,                 0x800000F0);
OS_DEF_GLOBAL_VAR(OSBI2*, DVD_BI2,                         0x800000F4);
OS_DEF_GLOBAL_VAR(u32, BUS_CLOCK_SPEED,                    0x800000F8);
OS_DEF_GLOBAL_VAR(u32, CPU_CLOCK_SPEED,                    0x800000FC);
// clang-format on

/**
 * 0x80003000 - 0x80003F00
 */
// clang-format off
OS_DEF_GLOBAL_ARR(void*, EXCEPTION_TABLE, [15],          0x80003000);
OS_DEF_GLOBAL_VAR(void*, INTR_HANDLER_TABLE,             0x80003040);
OS_DEF_GLOBAL_ARR(volatile s32, EXI_LAST_INSERT, [2],    0x800030C0);
OS_DEF_GLOBAL_VAR(void*, FIRST_REL,                      0x800030C8);
OS_DEF_GLOBAL_VAR(void*, LAST_REL,                       0x800030CC);
OS_DEF_GLOBAL_VAR(void*, REL_NAME_TABLE,                 0x800030D0);
OS_DEF_GLOBAL_VAR(u32, DOL_TOTAL_TEXT_DATA,              0x800030D4);
OS_DEF_GLOBAL_VAR(s64, SYSTEM_TIME,                      0x800030D8);
OS_DEF_GLOBAL_VAR(s8, PAD_FLAGS,                         0x800030E3);
OS_DEF_GLOBAL_VAR(u16, GC_PAD_3_BTN,                     0x800030E4);
OS_DEF_GLOBAL_VAR(volatile u16, DVD_DEVICE_CODE,         0x800030E6);
OS_DEF_GLOBAL_VAR(u8, BI2_DEBUG_FLAG,                    0x800030E8);
OS_DEF_GLOBAL_VAR(u8, PAD_SPEC,                          0x800030E9);
OS_DEF_GLOBAL_VAR(struct OSExecParams*, DOL_EXEC_PARAMS, 0x800030F0);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM1_SIZE,               0x80003100);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM1_SIZE,              0x80003104);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_START,              0x8000310C);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM1_END,                0x80003110);
OS_DEF_GLOBAL_VAR(u32, PHYSICAL_MEM2_SIZE,               0x80003118);
OS_DEF_GLOBAL_VAR(u32, SIMULATED_MEM2_SIZE,              0x8000311C);
OS_DEF_GLOBAL_VAR(void*, ACCESSIBLE_MEM2_END,            0x80003120);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_START,              0x80003124);
OS_DEF_GLOBAL_VAR(void*, USABLE_MEM2_END,                0x80003128);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_START,               0x80003130);
OS_DEF_GLOBAL_VAR(void*, IPC_BUFFER_END,                 0x80003134);
OS_DEF_GLOBAL_VAR(u32, HOLLYWOOD_REV,                    0x80003138);
OS_DEF_GLOBAL_VAR(u32, IOS_VERSION,                      0x80003140);
OS_DEF_GLOBAL_VAR(u32, IOS_BUILD_DATE,                   0x80003144);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_START,                 0x80003148);
OS_DEF_GLOBAL_VAR(void*, IOS_HEAP_END,                   0x8000314C);
OS_DEF_GLOBAL_VAR(u32, GDDR_VENDOR_CODE,                 0x80003158);
OS_DEF_GLOBAL_VAR(u8, BOOT_PROGRAM_TARGET,               0x8000315C);
OS_DEF_GLOBAL_VAR(u8, APPLOADER_TARGET,                  0x8000315D);
OS_DEF_GLOBAL_VAR(BOOL, MIOS_SHUTDOWN_FLAG,              0x80003164);
OS_DEF_GLOBAL_VAR(u32, CURRENT_APP_NAME,                 0x80003180);
OS_DEF_GLOBAL_VAR(u8, CURRENT_APP_TYPE,                  0x80003184);
OS_DEF_GLOBAL_VAR(u8, LOCKED_FLAG,                       0x80003187);
OS_DEF_GLOBAL_VAR(u32, MINIMUM_IOS_VERSION,              0x80003188);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_LAUNCH_CODE,           0x8000318C);
OS_DEF_GLOBAL_VAR(u32, NAND_TITLE_RETURN_CODE,           0x80003190);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_TYPE,              0x80003194);
OS_DEF_GLOBAL_VAR(u32, BOOT_PARTITION_OFFSET,            0x80003198);
OS_DEF_GLOBAL_VAR(u8, BOOT_PARTITION_319C,               0x8000319C);
OS_DEF_GLOBAL_VAR(s8, WIFI_AFH_CHANNEL,                  0x800031A2);
OS_DEF_GLOBAL_ARR(u8, NWC24_USER_ID_BUFFER, [32],        0x800031C0);
OS_DEF_GLOBAL_VAR(u64, NWC24_USER_ID,                    0x800031C0);
OS_DEF_GLOBAL_ARR(u8, SC_PRDINFO, [0x100],               0x80003800);
// clang-format on

/**
 * PI hardware globals
 */
volatile u32 DECL_HW_REGS(PI) DECL_ADDRESS(0xCC003000);
typedef enum {
    PI_INTSR,    //!< 0xCC003000
    PI_INTMR,    //!< 0xCC003004
    PI_REG_0x8,  //!< 0xCC003008
    PI_REG_0xC,  //!< 0xCC00300C
    PI_REG_0x10, //!< 0xCC003010
    PI_REG_0x14, //!< 0xCC003014
    PI_REG_0x18, //!< 0xCC003018
    PI_REG_0x1C, //!< 0xCC00301C
    PI_REG_0x20, //!< 0xCC003020
    PI_RESET,    //!< 0xCC003024
    // . . .
} PIHwReg;

// INTSR - Interrupt Cause Register
#define PI_INTSR_ERROR (1 << 0)
#define PI_INTSR_RSW (1 << 1)
#define PI_INTSR_DI (1 << 2)
#define PI_INTSR_SI (1 << 3)
#define PI_INTSR_EXI (1 << 4)
#define PI_INTSR_AI (1 << 5)
#define PI_INTSR_DSP (1 << 6)
#define PI_INTSR_MEM (1 << 7)
#define PI_INTSR_VI (1 << 8)
#define PI_INTSR_PE_TOKEN (1 << 9)
#define PI_INTSR_PE_FINISH (1 << 10)
#define PI_INTSR_CP (1 << 11)
#define PI_INTSR_DEBUG (1 << 12)
#define PI_INTSR_HSP (1 << 13)
#define PI_INTSR_ACR (1 << 14)
#define PI_INTSR_RSWST (1 << 16)

// INTMR - Interrupt Mask Register
#define PI_INTMR_ERROR (1 << 0)
#define PI_INTMR_RSW (1 << 1)
#define PI_INTMR_DI (1 << 2)
#define PI_INTMR_SI (1 << 3)
#define PI_INTMR_EXI (1 << 4)
#define PI_INTMR_AI (1 << 5)
#define PI_INTMR_DSP (1 << 6)
#define PI_INTMR_MEM (1 << 7)
#define PI_INTMR_VI (1 << 8)
#define PI_INTMR_PE_TOKEN (1 << 9)
#define PI_INTMR_PE_FINISH (1 << 10)
#define PI_INTMR_CP (1 << 11)
#define PI_INTMR_DEBUG (1 << 12)
#define PI_INTMR_HSP (1 << 13)
#define PI_INTMR_ACR (1 << 14)

/**
 * MI hardware registers
 */
volatile u16 DECL_HW_REGS(MI) DECL_ADDRESS(0xCC004000);
typedef enum {
    MI_PAGE_MEM0_H, //!< 0xCC004000
    MI_PAGE_MEM0_L, //!< 0xCC004002
    MI_PAGE_MEM1_H, //!< 0xCC004004
    MI_PAGE_MEM1_L, //!< 0xCC004006
    MI_PAGE_MEM2_H, //!< 0xCC004008
    MI_PAGE_MEM2_L, //!< 0xCC00400A
    MI_PAGE_MEM3_H, //!< 0xCC00400C
    MI_PAGE_MEM3_L, //!< 0xCC00400E
    MI_PROT_MEM0,   //!< 0xCC004010
    MI_PROT_MEM1,   //!< 0xCC004012
    MI_PROT_MEM2,   //!< 0xCC004014
    MI_PROT_MEM3,   //!< 0xCC004016
    MI_REG_0x18,    //!< 0xCC004018
    MI_REG_0x1A,    //!< 0xCC00401A
    MI_INTMR,       //!< 0xCC00401C
    MI_INTSR,       //!< 0xCC00401E
    MI_REG_0x20,    //!< 0xCC004020
    MI_ADDRLO,      //!< 0xCC004022
    MI_ADDRHI,      //!< 0xCC004024
    MI_REG_0x26,    //!< 0xCC004026
    MI_REG_0x28,    //!< 0xCC004028
    // . . .
} MIHwReg;

// INTMR - Interrupt Mask Register
#define MI_INTMR_MEM0 (1 << 0)
#define MI_INTMR_MEM1 (1 << 1)
#define MI_INTMR_MEM2 (1 << 2)
#define MI_INTMR_MEM3 (1 << 3)
#define MI_INTMR_ADDR (1 << 4)

// INTSR - Interrupt Cause Register
#define MI_INTSR_MEM0 (1 << 0)
#define MI_INTSR_MEM1 (1 << 1)
#define MI_INTSR_MEM2 (1 << 2)
#define MI_INTSR_MEM3 (1 << 3)
#define MI_INTSR_ADDR (1 << 4)

/**
 * DI hardware registers
 */
volatile u32 DECL_HW_REGS(DI) DECL_ADDRESS(0xCD006000);
typedef enum {
    DI_DMA_ADDR = 5, // !< 0xCD006014
    DI_CONFIG = 9,   // !< 0xCD006024
} DIHwReg;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS/OSHardware.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 25 "revolution/OS/OSInterrupt.h" */
/* end "revolution/OS/OSInterrupt.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 26 "revolution/OS/OSIpc.h" */
/* end "revolution/OS/OSIpc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 27 "revolution/OS/OSLink.h" */
/* end "revolution/OS/OSLink.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 28 "revolution/OS/OSMemory.h" */
/* end "revolution/OS/OSMemory.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 29 "revolution/OS/OSMessage.h" */
/* end "revolution/OS/OSMessage.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 30 "revolution/OS/OSMutex.h" */
/* end "revolution/OS/OSMutex.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 31 "revolution/OS/OSNet.h" */
/* end "revolution/OS/OSNet.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 32 "revolution/OS/OSPlayRecord.h" */
/* end "revolution/OS/OSPlayRecord.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 33 "revolution/OS/OSPlayTime.h" */
/* end "revolution/OS/OSPlayTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 34 "revolution/OS/OSReset.h" */
/* end "revolution/OS/OSReset.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 35 "revolution/OS/OSRtc.h" */
/* end "revolution/OS/OSRtc.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 36 "revolution/OS/OSSerial.h" */
/* end "revolution/OS/OSSerial.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 37 "revolution/OS/OSStateFlags.h" */
/* end "revolution/OS/OSStateFlags.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 38 "revolution/OS/OSStateTM.h" */
/* end "revolution/OS/OSStateTM.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 39 "revolution/OS/OSSync.h" */
/* end "revolution/OS/OSSync.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 40 "revolution/OS/OSThread.h" */
/* end "revolution/OS/OSThread.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 41 "revolution/OS/OSTime.h" */
/* end "revolution/OS/OSTime.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 42 "revolution/OS/OSUtf.h" */
/* end "revolution/OS/OSUtf.h" */
/* "libs/RVL_SDK/include/revolution/OS.h" line 43 "revolution/OS/__ppc_eabi_init.h" */
/* end "revolution/OS/__ppc_eabi_init.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/OS.h" */

/* "libs/RVL_SDK/include/revolution/MEM/mem_heapCommon.h" line 8 "string.h" */
/* end "string.h" */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MEM_HEAP_OPT_CLEAR_ALLOC = (1 << 0),
    MEM_HEAP_OPT_DEBUG_FILL = (1 << 1),
    MEM_HEAP_OPT_CAN_LOCK = (1 << 2)
} MEMHeapOpt;

typedef struct MEMiHeapHead {
    u32 magic;     // at 0x0
    MEMLink link;  // at 0x4
    MEMList list;  // at 0xC
    u8* start;     // at 0x18
    u8* end;       // at 0x1C
    OSMutex mutex; // at 0x20

    union {
        u32 attribute;
        struct {
            u32 attribute_0_24 : 24;
            u32 opt : 8;
        };
    }; // at 0x38
} MEMiHeapHead;

void MEMiInitHeapHead(MEMiHeapHead* heap, u32 magic, void* start, void* end,
                      u16 opt);
void MEMiFinalizeHeap(MEMiHeapHead* heap);
MEMiHeapHead* MEMFindContainHeap(const void* memBlock);

static uintptr_t GetUIntPtr(const void* p) {
    return (uintptr_t)p;
}

static int ComparePtr(const void* p0, const void* p1) {
    return (const u8*)p0 - (const u8*)p1;
}

static void* AddU32ToPtr(const void* p, u32 ofs) {
    return (void*)(GetUIntPtr(p) + ofs);
}

static void* SubU32ToPtr(const void* p, u32 ofs) {
    return (void*)(GetUIntPtr(p) - ofs);
}

static const void* AddU32ToCPtr(const void* p, u32 ofs) {
    return (const void*)(GetUIntPtr(p) + ofs);
}

static const void* SubU32ToCPtr(const void* p, u32 ofs) {
    return (const void*)(GetUIntPtr(p) - ofs);
}

static s32 GetOffsetFromPtr(const void* start, const void* end) {
    return GetUIntPtr(end) - GetUIntPtr(start);
}

static u16 GetOptForHeap(const MEMiHeapHead* heap) {
    return heap->opt;
}

static void SetOptForHeap(MEMiHeapHead* heap, u16 opt) {
    heap->opt = (u8)opt;
}

static void LockHeap(MEMiHeapHead* heap) {
    if (GetOptForHeap(heap) & MEM_HEAP_OPT_CAN_LOCK) {
        OSLockMutex(&heap->mutex);
    }
}

static void UnlockHeap(MEMiHeapHead* heap) {
    if (GetOptForHeap(heap) & MEM_HEAP_OPT_CAN_LOCK) {
        OSUnlockMutex(&heap->mutex);
    }
}

static void FillAllocMemory(MEMiHeapHead* heap, void* memBlock, u32 size) {
    if (GetOptForHeap(heap) & MEM_HEAP_OPT_CLEAR_ALLOC) {
        memset(memBlock, 0, size);
    }
}

static s32 MEMGetHeapTotalSize(const MEMiHeapHead* heap) {
    return GetOffsetFromPtr(heap, heap->end);
}

static void* MEMGetHeapEndAddress(const MEMiHeapHead* heap) {
    return heap->end;
}

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MEM/mem_heapCommon.h" */
/* "libs/RVL_SDK/include/revolution/MEM.h" line 10 "revolution/MEM/mem_list.h" */
/* end "revolution/MEM/mem_list.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/MEM.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class ArcResourceAccessor;
class DrawInfo;
class GroupContainer;
class Pane;

/******************************************************************************
 *
 * OriginType
 *
 ******************************************************************************/
enum OriginType {
    ORIGINTYPE_TOPLEFT,
    ORIGINTYPE_CENTER,

    ORIGINTYPE_MAX
};

namespace res {

/******************************************************************************
 *
 * LYT1 binary layout
 *
 ******************************************************************************/
struct Layout {
    static const u32 SIGNATURE = FOURCC('l', 'y', 't', '1');

    DataBlockHeader blockHeader; // at 0x0
    u8 originType;               // at 0x8
    u8 PADDING_0x9[0xC - 0x9];   // at 0x9
    Size layoutSize;             // at 0xC
};

} // namespace res

/******************************************************************************
 *
 * Layout
 *
 ******************************************************************************/
class Layout {
public:
    static const u32 SIGNATURE = FOURCC('R', 'L', 'Y', 'T');
    static const u32 SIGNATURE_ANIMATION = FOURCC('R', 'L', 'A', 'N');

public:
    Layout();
    virtual ~Layout(); // at 0x8

    virtual bool Build(const void* pLytBinary,
                       ResourceAccessor* pAccessor); // at 0xC
    
    virtual AnimTransform* CreateAnimTransform(); // at 0x10
    virtual AnimTransform*
    CreateAnimTransform(const void* pAnmBinary,
                        ResourceAccessor* pAccessor); // at 0x14
    virtual void
    CreateAnimTransform(const AnimResource& rResource,
                        ResourceAccessor* pAccessor); //at 0x18

    virtual void BindAnimation(AnimTransform* pAnimTrans);   // at 0x1C
    virtual void UnbindAnimation(AnimTransform* pAnimTrans); // at 0x20
    virtual void UnbindAllAnimation();                       // at 0x24
    virtual void
    BindAnimationAuto(const AnimResource& rResource,
                     ResourceAccessor* pAccessor); //at 0x28
    virtual void SetAnimationEnable(AnimTransform* pAnimTrans,
                                    bool enable); // at 0x2C

    virtual void CalculateMtx(const DrawInfo& rInfo); // at 0x30
    virtual void Draw(const DrawInfo& rInfo);         // at 0x34
    virtual void Animate(u32 option = 0);             // at 0x38

    virtual void SetTagProcessor(ut::WideTagProcessor* pProcessor); // at 0x3C

    ut::Rect GetLayoutRect() const;

    Pane* GetRootPane() const {
        return mpRootPane;
    }

    GroupContainer* GetGroupContainer() const {
        return mpGroupContainer;
    }

    static MEMAllocator* GetAllocator() {
        return mspAllocator;
    }
    static void SetAllocator(MEMAllocator* pAllocator) {
        mspAllocator = pAllocator;
    }

    static void* AllocMemory(u32 size) {
        return MEMAllocFromAllocator(mspAllocator, size);
    }
    static void FreeMemory(void* pBlock) {
        MEMFreeToAllocator(mspAllocator, pBlock);
    }

    template <typename TObj> static TObj* NewObj() {
        void* pMem = AllocMemory(sizeof(TObj));

        if (pMem != NULL) {
            return new (pMem) TObj();
        }

        return NULL;
    }

    template <typename TObj, typename TParam> static TObj* NewObj(TParam param1) {
        void* pMem = AllocMemory(sizeof(TObj));

        if (pMem != NULL) {
            return new (pMem) TObj(param1);
        }

        return NULL;
    }

    template <typename TObj, typename TParam1, typename TParam2>
    static TObj* NewObj(TParam1 param1, TParam2 param2) {
        void* pMem = AllocMemory(sizeof(TObj));
        
        if (pMem != NULL) {
            return new (pMem) TObj(param1, param2);
        }

        return NULL;
    }
    
    template <typename TObj> static TObj* NewArray(size_t num) {
        void* pMem = AllocMemory(num * sizeof(TObj));

        if(pMem == NULL){
            return NULL;
        }else{
            TObj* const objAry = static_cast<TObj*>(pMem);

            for (size_t i = 0; i < num; i++) {
               new (&objAry[i]) TObj();
            }

            return objAry;
        }
    }

    template <typename TObj> static void DeleteObj(TObj* pObj) {
        if(pObj != NULL){
            pObj->~TObj();
            FreeMemory(pObj);
        }
    }

    template <typename TObj> static void DeleteArray(TObj* objAry, size_t num) {
        if(objAry != NULL){
            for (size_t i = 0; i < num; i++) {
                objAry[i].~TObj();
            }

            FreeMemory(objAry);
            objAry = NULL;
        }
    }

protected:
    static const u32 SIGNATURE_TEXTURELIST = FOURCC('t', 'x', 'l', '1');
    static const u32 SIGNATURE_FONTLIST = FOURCC('f', 'n', 'l', '1');
    static const u32 SIGNATURE_MATERIALLIST = FOURCC('m', 'a', 't', '1');

    static const u32 SIGNATURE_ANIMATIONINFO = FOURCC('p', 'a', 'i', '1');

    static const u32 SIGNATURE_PANESTART = FOURCC('p', 'a', 's', '1');
    static const u32 SIGNATURE_PANEEND = FOURCC('p', 'a', 'e', '1');

    static const u32 SIGNATURE_GROUPSTART = FOURCC('g', 'r', 's', '1');
    static const u32 SIGNATURE_GROUPEND = FOURCC('g', 'r', 'e', '1');

protected:
    static Pane* BuildPaneObj(s32 kind, const void* pBinary,
                              const ResBlockSet& rBlockSet) DECOMP_DONT_INLINE;

protected:
    AnimTransformList mAnimTransList; // at 0x4
    Pane* mpRootPane;                 // at 0x10
    GroupContainer* mpGroupContainer; // at 0x14
    Size mLayoutSize;                 // at 0x18
    u8 mOriginType;                   // at 0x20

    static MEMAllocator* mspAllocator;
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_layout.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 11 "nw4r/lyt/lyt_material.h" */
#ifndef NW4R_LYT_MATERIAL_H
#define NW4R_LYT_MATERIAL_H
/* "libs/nw4r/include/nw4r/lyt/lyt_material.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_material.h" line 4 "nw4r/lyt/lyt_common.h" */
/* end "nw4r/lyt/lyt_common.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_material.h" line 5 "nw4r/lyt/lyt_texMap.h" */
#ifndef NW4R_LYT_TEX_MAP_H
#define NW4R_LYT_TEX_MAP_H
/* "libs/nw4r/include/nw4r/lyt/lyt_texMap.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_texMap.h" line 4 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_texMap.h" line 5 "revolution/TPL.h" */
/**
 * References: YAGCD, BrawlBox
 */

#ifndef RVL_SDK_PUBLIC_TPL_H
#define RVL_SDK_PUBLIC_TPL_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/TPL.h" line 10 "revolution/TPL/TPL.h" */
/* end "revolution/TPL/TPL.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/TPL.h" */

namespace nw4r {
namespace lyt {

class TexMap {
public:
    TexMap() {
        SetImage(NULL);
        SetSize(0, 0);
        SetTexelFormat(GX_TF_I4);
        SetWrapMode(GX_CLAMP, GX_CLAMP);
        SetMipMap(false);
        SetFilter(GX_LINEAR, GX_LINEAR);
        SetLOD(0.0f, 0.0f);
        SetLODBias(0.0f);
        SetBiasClampEnable(false);
        SetEdgeLODEnable(false);
        SetAnisotropy(GX_ANISO_1);
        SetPalette(NULL);
        SetPaletteFormat(GX_TL_IA8);
        SetPaletteEntryNum(0);
    }

    TexMap(TPLPalette* pPalette, u32 id) {
        Set(pPalette, id);
        SetBiasClampEnable(false);
        SetAnisotropy(GX_ANISO_1);
    }

    void Get(GXTexObj* pTexObj) const;
    void Get(GXTlutObj* pTlutObj) const;

    void Set(const TexMap& rOther) {
        *this = rOther;
    }
    void Set(TPLPalette* pPalette, u32 id);
    void Set(const TPLDescriptor* pDesc);

    void SetNoWrap(const TexMap& rOther);
    void SetNoWrap(const TPLDescriptor* pDesc);

    void* GetImage() const {
        return mpImage;
    }
    void SetImage(void* pImage) {
        mpImage = pImage;
    }

    void* GetPalette() const {
        return mpPalette;
    }
    void SetPalette(void* pPalette) {
        mpPalette = pPalette;
    }

    u16 GetWidth() const {
        return mWidth;
    }
    u16 GetHeight() const {
        return mHeight;
    }
    Size GetSize() const {
        return Size(static_cast<f32>(mWidth), static_cast<f32>(mHeight));
    }
    void SetSize(u16 width, u16 height) {
        mWidth = width;
        mHeight = height;
    }

    f32 GetMinLOD() const {
        return mMinLOD;
    }
    f32 GetMaxLOD() const {
        return mMaxLOD;
    }
    void SetLOD(f32 minLOD, f32 maxLOD) {
        mMinLOD = minLOD;
        mMaxLOD = maxLOD;
    }

    f32 GetLODBias() const {
        return mLODBias / 256.0f;
    }
    void SetLODBias(f32 bias) {
        mLODBias = static_cast<u16>(bias * 256.0f);
    }

    u16 GetPaletteEntryNum() const {
        return mPaletteEntryNum;
    }
    void SetPaletteEntryNum(u16 entrynum) {
        mPaletteEntryNum = entrynum;
    }

    GXTexFmt GetTexelFormat() const {
        return static_cast<GXTexFmt>(mBits.textureFormat);
    }
    void SetTexelFormat(GXTexFmt format) {
        mBits.textureFormat = format;
    }

    bool IsMipMap() const {
        return mBits.mipmap;
    }
    void SetMipMap(bool mipmap) {
        mBits.mipmap = mipmap;
    }

    GXTexWrapMode GetWrapModeS() const {
        return static_cast<GXTexWrapMode>(mBits.wrapS);
    }
    GXTexWrapMode GetWrapModeT() const {
        return static_cast<GXTexWrapMode>(mBits.wrapT);
    }
    void SetWrapMode(GXTexWrapMode wrapS, GXTexWrapMode wrapT) {
        mBits.wrapS = wrapS;
        mBits.wrapT = wrapT;
    }

    GXTexFilter GetMinFilter() const {
        return static_cast<GXTexFilter>(mBits.minFilter);
    }
    GXTexFilter GetMagFilter() const {
        return static_cast<GXTexFilter>(mBits.magFilter);
    }
    void SetFilter(GXTexFilter minFilt, GXTexFilter magFilt) {
        mBits.minFilter = minFilt;
        mBits.magFilter = magFilt;
    }

    bool IsBiasClampEnable() const {
        return mBits.biasClampEnable;
    }
    void SetBiasClampEnable(bool enable) {
        mBits.biasClampEnable = enable;
    }

    bool IsEdgeLODEnable() const {
        return mBits.edgeLODEnable;
    }
    void SetEdgeLODEnable(bool enable) {
        mBits.edgeLODEnable = enable;
    }

    GXAnisotropy GetAnisotropy() const {
        return static_cast<GXAnisotropy>(mBits.anisotropy);
    }
    void SetAnisotropy(GXAnisotropy aniso) {
        mBits.anisotropy = aniso;
    }

    GXTlutFmt GetPaletteFormat() const {
        return static_cast<GXTlutFmt>(mBits.paletteFormat);
    }
    void SetPaletteFormat(GXTlutFmt format) {
        mBits.paletteFormat = format;
    }

private:
    void* mpImage;        // at 0x0
    void* mpPalette;      // at 0x4
    u16 mWidth;           // at 0x8
    u16 mHeight;          // at 0xA
    f32 mMinLOD;          // at 0xC
    f32 mMaxLOD;          // at 0x10
    u16 mLODBias;         // at 0x14
    u16 mPaletteEntryNum; // at 0x16

    struct {
        u32 textureFormat : 4;
        u32 mipmap : 1;
        u32 wrapS : 2;
        u32 wrapT : 2;
        u32 minFilter : 3;
        u32 magFilter : 3;
        u32 biasClampEnable : 1;
        u32 edgeLODEnable : 1;
        u32 anisotropy : 2;
        u32 paletteFormat : 2;
    } mBits; // at 0x18
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_texMap.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_material.h" line 6 "nw4r/lyt/lyt_resources.h" */
/* end "nw4r/lyt/lyt_resources.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_material.h" line 7 "nw4r/lyt/lyt_types.h" */
/* end "nw4r/lyt/lyt_types.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_material.h" line 9 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_material.h" line 11 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class AnimTransform;
class AnimResource;
struct ResBlockSet;

namespace detail {

/******************************************************************************
 *
 * BitGXNums
 *
 ******************************************************************************/
struct BitGXNums {
    u32 texMap : 4;
    u32 texSRT : 4;
    u32 texCoordGen : 4;
    u32 indSRT : 2;
    u32 indStage : 3;
    u32 tevSwap : 1;
    u32 tevStage : 5;
    u32 chanCtrl : 1;
    u32 matCol : 1;
    u32 alpComp : 1;
    u32 blendMode : 1;
};

} // namespace detail

namespace res {

/******************************************************************************
 *
 * MAT1 binary layout
 *
 ******************************************************************************/
struct Material {
    char name[NW4R_LYT_MATERIAL_NAME_LEN]; // at 0x0
    GXColorS10 tevCols[TEVCOLOR_MAX];      // at 0x14
    GXColor tevKCols[GX_MAX_KCOLOR];       // at 0x2C
    MaterialResourceNum resNum;            // at 0x3C
};

} // namespace res

/******************************************************************************
 *
 * Material
 *
 ******************************************************************************/
class Material {
public:
    Material();
    Material(const res::Material* pRes, const ResBlockSet& rBlockSet);
    virtual ~Material(); // at 0x8

    virtual bool SetupGX(bool modulate, u8 alpha); // at 0xC

    virtual void BindAnimation(AnimTransform* pAnimTrans);   // at 0x10
    virtual void UnbindAnimation(AnimTransform* pAnimTrans); // at 0x14
    virtual void UnbindAllAnimation();                       // at 0x18

    virtual void Animate(); // at 0x1C

    virtual AnimationLink*
    FindAnimationLink(AnimTransform* pAnimTrans); // at 0x20
    virtual AnimationLink*
    FindAnimationLink(const AnimResource& rResource); // at 0x24

    virtual void SetAnimationEnable(AnimTransform* pAnimTrans,
                                    bool enable); // at 0x28
    virtual void SetAnimationEnable(const AnimResource& rResource,
                                    bool enable); // at 0x2C

    void AddAnimationLink(AnimationLink* pAnimLink);

    GXColorS10 GetTevColor(u32 idx) const {
        return mTevCols[idx];
    }
    void SetTevColor(u32 idx, const GXColorS10& rColor) {
        mTevCols[idx] = rColor;
    }

    ut::Color GetTevKColor(u32 idx) const {
        return mTevKCols[idx];
    }
    void SetTevKColor(u32 idx, ut::Color color) {
        mTevKCols[idx] = color;
    }

    u8 GetTextureNum() const {
        return mGXMemNum.texMap;
    }
    u8 GetTextureCap() const {
        return mGXMemCap.texMap;
    }
    void SetTextureNum(u8 num);

    u8 GetTexSRTCap() const {
        return mGXMemCap.texSRT;
    }

    u8 GetTexCoordGenCap() const {
        return mGXMemCap.texCoordGen;
    }
    void SetTexCoordGenNum(u8 num);

    u8 GetIndTexSRTCap() const {
        return mGXMemCap.indSRT;
    }

    u8 GetTevStageNum() const {
        return mGXMemNum.tevStage;
    }
    void SetTevStageNum(u8 num);

    void SetIndStageNum(u8 num);

    void SetColorElement(u32 idx, s16 value);

    bool IsTevSwapCap() const {
        return mGXMemCap.tevSwap;
    }
    bool IsChanCtrlCap() const {
        return mGXMemCap.chanCtrl;
    }
    bool IsMatColorCap() const {
        return mGXMemCap.matCol;
    }
    bool IsAlphaCompareCap() const {
        return mGXMemCap.alpComp;
    }
    bool IsBlendModeCap() const {
        return mGXMemCap.blendMode;
    }

    const TexMap* GetTexMapAry() const;
    TexMap* GetTexMapAry();

    const TexSRT* GetTexSRTAry() const;
    TexSRT* GetTexSRTAry();

    const TexCoordGen* GetTexCoordGenAry() const;
    TexCoordGen* GetTexCoordGenAry();

    const ChanCtrl* GetChanCtrlAry() const;
    ChanCtrl* GetChanCtrlAry();

    const ut::Color* GetMatColAry() const;
    ut::Color* GetMatColAry();

    const TevSwapMode* GetTevSwapAry() const;
    TevSwapMode* GetTevSwapAry();

    const AlphaCompare* GetAlphaComparePtr() const;
    AlphaCompare* GetAlphaComparePtr();

    const BlendMode* GetBlendModePtr() const;
    BlendMode* GetBlendModePtr();

    const IndirectStage* GetIndirectStageAry() const;
    IndirectStage* GetIndirectStageAry();

    const TexSRT* GetIndTexSRTAry() const;
    TexSRT* GetIndTexSRTAry();

    const TevStage* GetTevStageAry() const;
    TevStage* GetTevStageAry();

    const TexMap& GetTexture(u8 idx) const {
        return GetTexMapAry()[idx];
    }
    void GetTexture(GXTexObj* pTexObj, u8 idx) const {
        GetTexMapAry()[idx].Get(pTexObj);
    }
    void SetTexture(u8 idx, const TexMap& rTexMap) {
        GetTexMapAry()[idx].Set(rTexMap);
    }
    void SetTextureNoWrap(u8 idx, const TexMap& rTexMap) {
        GetTexMapAry()[idx].SetNoWrap(rTexMap);
    }

    const TexSRT& GetTexSRT(u32 idx) const {
        return GetTexSRTAry()[idx];
    }
    void SetTexSRT(const TexSRT& rTexSRT, u32 idx) {
        GetTexSRTAry()[idx] = rTexSRT;
    }

    void SetTexSRTElement(u32 srt, u32 idx, f32 value) {
        f32* const pArray = reinterpret_cast<f32*>(&GetTexSRTAry()[srt]);
        pArray[idx] = value;
    }

    void SetTexCoordGen(u32 idx, TexCoordGen gen) {
        GetTexCoordGenAry()[idx] = gen;
    }

    ut::Color GetMatColor() const {
        return GetMatColAry()[0];
    }

    void SetIndTexSRTElement(u32 srt, u32 idx, f32 value) {
        f32* const pArray = reinterpret_cast<f32*>(&GetIndTexSRTAry()[srt]);
        pArray[idx] = value;
    }

    void SetName(const char* pName);
    const char* GetName() const {
        return mName;
    }

    bool IsUserAllocated() const {
        return mbUserAllocated;
    }

    void ReserveGXMem(u8 texMapNum, u8 texSrtNum, u8 texCoordGenNum,
                  u8 tevStageNum, bool allocTevSwap, u8 indStageNum,
                  u8 indSrtNum, bool allocChanCtrl, bool allocMatCol,
                  bool allocAlpComp, bool allocBlendMode);

protected:
    static const int MAX_TEX_SRT = (GX_TEXMTX9 - GX_TEXMTX0) / 3 + 1;
    static const int MAX_IND_SRT = (GX_ITM_2 - GX_ITM_0) + 1;

protected:
    AnimationLinkList mAnimList; // at 0x4

    GXColorS10 mTevCols[TEVCOLOR_MAX];  // at 0x10
    ut::Color mTevKCols[GX_MAX_KCOLOR]; // at 0x28

    detail::BitGXNums mGXMemCap; // at 0x38
    detail::BitGXNums mGXMemNum; // at 0x3C
    void* mpGXMem;               // at 0x40

    char mName[NW4R_LYT_MATERIAL_NAME_LEN + 1]; // at 0x44
    bool mbUserAllocated;                       // at 0x59

    u8 PADDING_0x5A[0x5C - 0x5A]; // at 0x5A

private:
    void Init();
    void InitBitGXNums(detail::BitGXNums* pNums);
};

/******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/
namespace detail {

Size GetTextureSize(Material* pMaterial, u8 idx);

} // namespace detail
} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_material.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 12 "nw4r/lyt/lyt_pane.h" */
/* end "nw4r/lyt/lyt_pane.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 13 "nw4r/lyt/lyt_picture.h" */
#ifndef NW4R_LYT_PICTURE_H
#define NW4R_LYT_PICTURE_H
/* "libs/nw4r/include/nw4r/lyt/lyt_picture.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_picture.h" line 4 "nw4r/lyt/lyt_common.h" */
/* end "nw4r/lyt/lyt_common.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_picture.h" line 5 "nw4r/lyt/lyt_pane.h" */
/* end "nw4r/lyt/lyt_pane.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
struct ResBlockSet;
class TexMap;

namespace res {

/******************************************************************************
 *
 * PIC1 binary layout
 *
 ******************************************************************************/
struct Picture : Pane {
    static const u32 SIGNATURE = FOURCC('p', 'i', 'c', '1');

    u32 vtxCols[VERTEXCOLOR_MAX]; // at 0x4C
    u16 materialIdx;              // at 0x5C
    u8 texCoordNum;               // at 0x5E
    u8 PADDING_0x5F;              // at 0x5F
};

} // namespace res

/******************************************************************************
 *
 * Picture
 *
 ******************************************************************************/
class Picture : public Pane {
public:
    NW4R_UT_RTTI_DECL(Picture);

public:
    Picture(u8 num);
    Picture(const TexMap& rTexMap);
    Picture(const res::Picture* pRes, const ResBlockSet& rBlockSet);
    virtual ~Picture(); // at 0x8

    virtual void DrawSelf(const DrawInfo& rInfo); // at 0x18

    void SetTexCoord(u32 idx, const detail::TexCoord coord);

    virtual ut::Color GetVtxColor(u32 idx) const;       // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color color); // at 0x28

    virtual u8 GetVtxColorElement(u32 idx) const;       // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value); // at 0x38

    virtual void Append(const TexMap& rTexMap); // at 0x74

protected:
    ut::Color mVtxColors[VERTEXCOLOR_MAX]; // at 0xD4
    detail::TexCoordAry mTexCoordAry;      // at 0xE4

protected:
    void ReserveTexCoord(u8 num);
    void SetTexCoordNum(u8 num);

private:
    void Init(u8 num);
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_picture.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 14 "nw4r/lyt/lyt_resourceAccessor.h" */
/* end "nw4r/lyt/lyt_resourceAccessor.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 15 "nw4r/lyt/lyt_resources.h" */
/* end "nw4r/lyt/lyt_resources.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 16 "nw4r/lyt/lyt_texMap.h" */
/* end "nw4r/lyt/lyt_texMap.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 17 "nw4r/lyt/lyt_textBox.h" */
#ifndef NW4R_LYT_TEXTBOX_H
#define NW4R_LYT_TEXTBOX_H
/* "libs/nw4r/include/nw4r/lyt/lyt_textBox.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_textBox.h" line 4 "nw4r/lyt/lyt_common.h" */
/* end "nw4r/lyt/lyt_common.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_textBox.h" line 5 "nw4r/lyt/lyt_pane.h" */
/* end "nw4r/lyt/lyt_pane.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_textBox.h" line 6 "nw4r/lyt/lyt_types.h" */
/* end "nw4r/lyt/lyt_types.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_textBox.h" line 8 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
struct ResBlockSet;

/******************************************************************************
 *
 * TextColor
 *
 ******************************************************************************/
enum TextColor {
    TEXTCOLOR_TOP,
    TEXTCOLOR_BOTTOM,

    TEXTCOLOR_MAX
};

namespace res {

/******************************************************************************
 *
 * TXT1 binary layout
 *
 ******************************************************************************/
struct TextBox : Pane {
    static const u32 SIGNATURE = FOURCC('t', 'x', 't', '1');

    u16 textBufBytes;             // at 0x4C
    u16 textStrBytes;             // at 0x4E
    u16 materialIdx;              // at 0x50
    u16 fontIdx;                  // at 0x52
    u8 textPosition;              // at 0x54
    u8 textAlignment;             // at 0x55
    u8 PADDING_0x56[0x58 - 0x56]; // at 0x56
    u32 textStrOffset;            // at 0x58
    u32 textCols[TEXTCOLOR_MAX];  // at 0x5C
    Size fontSize;                // at 0x64
    f32 charSpace;                // at 0x6C
    f32 lineSpace;                // at 0x70
};

} // namespace res

/******************************************************************************
 *
 * TextBox
 *
 ******************************************************************************/
class TextBox : public Pane {
public:
    NW4R_UT_RTTI_DECL(TextBox);

public:
    TextBox(const res::TextBox* pRes, const ResBlockSet& rBlockSet);
    virtual ~TextBox(); // at 0x8

    virtual void DrawSelf(const DrawInfo& rInfo); // at 0x18

    virtual ut::Color GetVtxColor(u32 idx) const;       // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color color); // at 0x28

    virtual u8 GetVtxColorElement(u32 idx) const;       // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value); // at 0x38

    virtual void AllocStringBuffer(u16 len); // at 0x64
    virtual void FreeStringBuffer();         // at 0x68
    u16 GetStringBufferLength() const;

    virtual u16 SetString(const wchar_t* pStr, u16 pos = 0);      // at 0x6C
    virtual u16 SetString(const wchar_t* pStr, u16 pos, u16 len); // at 0x70

    ut::Rect GetTextDrawRect(const DrawInfo& rInfo) const;
    ut::Rect GetTextDrawRect(ut::WideTextWriter* pWriter) const;

    const wchar_t* GetString() const {
        return mTextBuf;
    }
    const wchar_t* GetStringBuffer() const {
        return mTextBuf;
    }

    const ut::Font* GetFont() const;
    void SetFont(const ut::Font* pFont);

    ut::Color GetTextColor(u32 idx) const {
        return mTextColors[idx];
    }
    void SetTextColor(u32 idx, ut::Color color) {
        mTextColors[idx] = color;
    }

    const Size& GetFontSize() const {
        return mFontSize;
    }
    void SetFontSize(const Size& rFontSize) {
        mFontSize = rFontSize;
    }

    f32 GetLineSpace() const {
        return mLineSpace;
    }
    void SetLineSpace(f32 space) {
        mLineSpace = space;
    }

    f32 GetCharSpace() const {
        return mCharSpace;
    }
    void SetCharSpace(f32 space) {
        mCharSpace = space;
    }

    ut::WideTagProcessor* GetTagProcessor() const {
        return mpTagProcessor;
    }
    void SetTagProcessor(ut::WideTagProcessor* pProcessor) {
        mpTagProcessor = pProcessor;
    }

    u8 GetTextPositionH() const {
        return detail::GetHorizontalPosition(mTextPosition);
    }
    void SetTextPositionH(u8 value) {
        detail::SetHorizontalPosition(&mTextPosition, value);
    }

    u8 GetTextPositionV() const {
        return detail::GetVerticalPosition(mTextPosition);
    }
    void SetTextPositionV(u8 value) {
        detail::SetVerticalPosition(&mTextPosition, value);
    }

    f32 GetTextMagH() const;
    f32 GetTextMagV() const;

    u32 MakeDrawFlag() const;

protected:
    wchar_t* mTextBuf;                    // at 0xD8
    ut::Color mTextColors[TEXTCOLOR_MAX]; // at 0xDC

    const ut::Font* mpFont; // at 0xE4
    Size mFontSize;         // at 0xE8
    f32 mLineSpace;         // at 0xF0
    f32 mCharSpace;         // at 0xF4

    ut::WideTagProcessor* mpTagProcessor; // at 0xF8
    u16 mTextBufBytes;                    // at 0xFC
    u16 mTextLen;                         // at 0xFE
    u8 mTextPosition;                     // at 0xFF

    struct {
        u8 bAllocFont : 1;
    } mBits; // at 0x100

private:
    void Init(u16 len);
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_textBox.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 18 "nw4r/lyt/lyt_types.h" */
/* end "nw4r/lyt/lyt_types.h" */
/* "libs/nw4r/include/nw4r/lyt.h" line 19 "nw4r/lyt/lyt_window.h" */
#ifndef NW4R_LYT_WINDOW_H
#define NW4R_LYT_WINDOW_H
/* "libs/nw4r/include/nw4r/lyt/lyt_window.h" line 2 "nw4r/types_nw4r.h" */
/* end "nw4r/types_nw4r.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_window.h" line 4 "nw4r/lyt/lyt_common.h" */
/* end "nw4r/lyt/lyt_common.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_window.h" line 5 "nw4r/lyt/lyt_layout.h" */
/* end "nw4r/lyt/lyt_layout.h" */
/* "libs/nw4r/include/nw4r/lyt/lyt_window.h" line 6 "nw4r/lyt/lyt_pane.h" */
/* end "nw4r/lyt/lyt_pane.h" */

/* "libs/nw4r/include/nw4r/lyt/lyt_window.h" line 8 "nw4r/ut.h" */
/* end "nw4r/ut.h" */

namespace nw4r {
namespace lyt {

// Forward declarations
class Material;
struct ResBlockSet;

/******************************************************************************
 *
 * InflationLRTB
 *
 ******************************************************************************/
struct InflationLRTB {
    f32 l; // at 0x0
    f32 r; // at 0x4
    f32 t; // at 0x8
    f32 b; // at 0xC
};

namespace res {

/******************************************************************************
 *
 * WindowContent
 *
 ******************************************************************************/
struct WindowContent {
    u32 vtxCols[VERTEXCOLOR_MAX]; // at 0x0
    u16 materialIdx;              // at 0x10
    u8 texCoordNum;               // at 0x12
    u8 PADDING_0x13[0x14 - 0x13]; // at 0x13
};

/******************************************************************************
 *
 * WindowFrame
 *
 ******************************************************************************/
struct WindowFrame {
    u16 materialIdx; // at 0x0
    u8 textureFlip;  // at 0x2
    u8 PADDING_0x3;  // at 0x3
};

/******************************************************************************
 *
 * WND1 binary layout
 *
 ******************************************************************************/
struct Window : public Pane {
    static const u32 SIGNATURE = FOURCC('w', 'n', 'd', '1');

    InflationLRTB inflation;    // at 0x4C
    u8 frameNum;                // at 0x5C
    u8 PADDING_0x5D;            // at 0x5D
    u8 PADDING_0x5E;            // at 0x5E
    u8 PADDING_0x5F;            // at 0x5F
    u32 contentOffset;          // at 0x60
    u32 frameOffsetTableOffset; // at 0x64
};

} // namespace res

/******************************************************************************
 *
 * WindowFrameEnum
 *
 ******************************************************************************/
enum WindowFrameEnum {
    WINDOWFRAME_LT,
    WINDOWFRAME_RT,
    WINDOWFRAME_LB,
    WINDOWFRAME_RB,

    WINDOWFRAME_L,
    WINDOWFRAME_R,
    WINDOWFRAME_T,
    WINDOWFRAME_B,

    WINDOWFRAME_MAX
};

/******************************************************************************
 *
 * WindowFrameSize
 *
 ******************************************************************************/
struct WindowFrameSize {
    f32 l; // at 0x0
    f32 r; // at 0x4
    f32 t; // at 0x8
    f32 b; // at 0xC
};

/******************************************************************************
 *
 * Window
 *
 ******************************************************************************/
class Window : public Pane {
public:
    NW4R_UT_RTTI_DECL(Window);

public:
    Window(const res::Window* pRes, const ResBlockSet& rBlockSet);
    virtual ~Window(); // at 0x8

    virtual void DrawSelf(const DrawInfo& rInfo); // at 0x18
    virtual void AnimateSelf(u32 option = 0);     // at 0x20

    virtual ut::Color GetVtxColor(u32 idx) const;       // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color color); // at 0x28
    virtual u8 GetVtxColorElement(u32 idx) const;       // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value); // at 0x38

    virtual Material* FindMaterialByName(const char* pName,
                                         bool recursive); // at 0x40

    virtual void UnbindAnimationSelf(AnimTransform* pAnimTrans); // at 0x50

    virtual AnimationLink*
    FindAnimationLinkSelf(AnimTransform* pAnimTrans); // at 0x54

    virtual void SetAnimationEnable(AnimTransform* pAnimTrans, bool enable,
                                    bool recursive); // at 0x5C
    
    virtual u8 GetMaterialNum() const; // at 0x64
    virtual Material* GetMaterial() const; // at 0x68
	virtual Material* GetMaterial(u32 index) const; // at 0x6C

    virtual Material* GetContentMaterial() const;      // at 0x74
    virtual Material* GetFrameMaterial(u32 idx) const; // at 0x78

protected:
    struct Frame {
        u8 textureFlip;      // at 0x0
        Material* pMaterial; // at 0x4

        Frame() : textureFlip(0), pMaterial(NULL) {}
        ~Frame() {
            Layout::DeleteObj(pMaterial);
        }
    };

    struct Content {
        ut::Color vtxColors[VERTEXCOLOR_MAX]; // at 0x0
        detail::TexCoordAry texCoordAry;      // at 0x10
    };

protected:
    virtual void DrawContent(const math::VEC2& rBase,
                             const WindowFrameSize& rFrameSize,
                             u8 alpha); // at 0x7C

    virtual void DrawFrame(const math::VEC2& rBase, const Frame& rFrame,
                           const WindowFrameSize& rFrameSize,
                           u8 alpha); // at 0x80

    virtual void DrawFrame4(const math::VEC2& rBase, const Frame* pFrames,
                            const WindowFrameSize& rFrameSize,
                            u8 alpha); // at 0x84

    virtual void DrawFrame8(const math::VEC2& rBase, const Frame* pFrames,
                            const WindowFrameSize& rFrameSize,
                            u8 alpha); // at 0x88

    void InitContent(u8 texNum);
    void InitFrame(u8 frameNum);
    
    void ReserveTexCoord(u8 num);

    WindowFrameSize GetFrameSize(u8 frameNum, const Frame* pFrames);

protected:
    InflationLRTB mContentInflation; // at 0xD8
    Content mContent;                // at 0xE8
    Frame* mFrames;                  // at 0x100
    u8 mFrameNum;                    // at 0x104
};

} // namespace lyt
} // namespace nw4r

#endif
/* end "nw4r/lyt/lyt_window.h" */

#endif
/* end "nw4r/lyt.h" */
/* "src/kyoshin/CUICfManager.hpp" line 7 "types.h" */
/* end "types.h" */

namespace nw4r {
namespace ut {

// Stub: retail size 0x3C; Destroy returns buffer for MemManager::deallocate.
class PackedFont {
public:
    void* Destroy();

private:
    u8 unk[0x3C];
};

} // namespace ut
} // namespace nw4r

struct CUICfUnk144 {
    u8 unk00[0x39];
    u8 unk39; // 0x39
};

// Queue item type for CUICfManager::Move list walks (CProcess-sized prefix).
struct CUICfMenuItem {
    u8 unk00[0x39];
    u8 unk39; // 0x39 - remove / SetRemove
    u8 unk3A[0x54 - 0x3A];
    u8 unk54; // 0x54
    u8 unk55; // 0x55
};

struct CUICfInitBlock {
    u32 unk00;
    u16 unk04;
    u8 unk06[0x34 - 6];
};

struct CUICfInitTail {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u32 unk34;
    u32 unk38;
    u32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    u32 unk58;
    u32 unk5C;
    u32 unk60;
    u32 unk64;
    u32 unk68;
    u32 unk6C;
    u32 unk70;
    u32 unk74;
    u32 unk78;
    u32 unk7C;
    u32 unk80;
    u32 unk84;
    u32 unk88;
    u32 unk8C;
};

struct CUICfInitState {
    u8 mode;
    u8 state;
    u8 unk02[2];
};

// 0xC-byte pool node for func_80133324's event queue - same layout as
// `_reslist_node<u32>` (mNext@0, mPrev@4, mItem@8). Empty slots have mNext==0.
struct CUICfListNode {
    CUICfListNode* next; // 0x0
    CUICfListNode* prev; // 0x4
    u32 item;            // 0x8
};

// 27-entry, 0-terminated id table copied onto the stack by func_80133324
// (retail: sp+0x28..0x5D, matches lbl_eu_804FFFDC minus its trailing entry).
struct CUICfIdTable {
    u16 ids[27];
};

class CUICfManager;

// Retail mangles this as a no-arg CUICfManager member (`Fv`) but the body
// reads r4/r5/r6 as real event-dispatch arguments; declared extern "C" here
// (before the class, so the in-class friend declaration below binds to this
// same linkage) so its ABI is r3=<unused self>, r4=id, r5=a1, r6=a2.
extern "C" void func_80133324__12CUICfManagerFv(CUICfManager* self, int id, int a1, int a2);

struct CUICfInitSlot {
    u8 unk00[4];
    u8 unk04;
    u8 unk05;
    u8 unk06[2];
    CUICfInitBlock unk08;
    CUICfInitBlock unk3C;
    CUICfInitBlock unk70;
    CUICfInitBlock unkA4;
    CUICfInitTail unkD8;
}; // size = 0x168

class CUICfManager : public CTTask<CUICfManager>, public IWorkEvent, public cf::IFlagEvent {
public:
    static CUICfManager* getInstance() {
        return spInstance;
    }
    static CUICfManager* create(CProcess* pParent, CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle);
    static nw4r::lyt::ArcResourceAccessor* func_801355F4();
    static int func_80135FDC();

    void Init();
    void Term();
    void Move();
    // func_80133324__12CUICfManagerFv is a free function below (retail Fv-mangled
    // member that actually reads r4/r5/r6 event args; see CUICfManager.cpp).
    friend void func_80133324__12CUICfManagerFv(CUICfManager* self, int id, int a1, int a2);

    // Fork helper for presentation gating (coop::ShouldRenderSplitScreen).
    u16 getFlags() const {
        return mFlags;
    }

private:
    // 0x000-0x054 CTTask
    // 0x054-0x058 IWorkEvent
    // 0x058-0x05C cf::IFlagEvent
    nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor; // 0x05C
    nw4r::ut::PackedFont mPackedFont60;                  // 0x060
    nw4r::ut::PackedFont mPackedFont9C;                  // 0x09C
    nw4r::ut::PackedFont mPackedFontD8;                  // 0x0D8
    CFileHandle* mFileHandle;                            // 0x114
    int unk118;                                          // 0x118
    u32 unk11C;                                          // 0x11C
    u32 unk120;                                          // 0x120 - Move countdown
    u8 unk124[0x128 - 0x124];                            // 0x124
    CUICfListNode* unk128;                               // 0x128 (event queue head)
    u8 unk12C[0x138 - 0x12C];                            // 0x12C
    CUICfListNode* unk138;                               // 0x138 (event node array)
    int unk13C;                                          // 0x13C (event node array count)
    u8 unk140[0x144 - 0x140];                            // 0x140
    CUICfUnk144* unk144;                                 // 0x144
    CUICfInitSlot mInitSlots[8];                         // 0x148
    // Slot0.unk00[0]/[1] are manager-wide clear/mark flags (0x148/0x149).
    u8 unkC88[8];                                        // 0xC88
    u16 mFlags;                                          // 0xC90 - Move bitflags
    u8 unkC92[2];                                        // 0xC92

    static CUICfManager* spInstance;
}; // size = 0xC94
/* end "kyoshin/CUICfManager.hpp" */

/* "src/kyoshin/CUICfManager.cpp" line 2 "monolib/device/CDeviceFile.hpp" */
#pragma once

/* "libs/monolib/include/monolib/device/CDeviceFile.hpp" line 2 "types.h" */
/* end "types.h" */

/* "libs/monolib/include/monolib/device/CDeviceFile.hpp" line 4 "monolib/device/CDeviceBase.hpp" */
#pragma once

/* "libs/monolib/include/monolib/device/CDeviceBase.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/device/CDeviceBase.hpp" line 3 "monolib/work/CWorkThread.hpp" */
/* end "monolib/work/CWorkThread.hpp" */
/* "libs/monolib/include/monolib/device/CDeviceBase.hpp" line 4 "monolib/work/CWorkUtil.hpp" */
/* end "monolib/work/CWorkUtil.hpp" */

//size: 0x1C8
class CDeviceBase : public CWorkThread {
public:
    enum DEVICE_BASE_FLAGS{
        FLAG_CREATED = (1 << 0)
    };

    CDeviceBase(const char* pName, CWorkThread* pParent, int capacity) : CWorkThread(pName, pParent, capacity) {
        mFlags = 0;
    }
    virtual ~CDeviceBase();

    inline void CDeviceBase_inline1(CWorkThread* pWorkThread){
        CWorkUtil::entryWork(this, pWorkThread, false);
        mFlags |= FLAG_CREATED;
    }

    inline bool CDeviceBase_inline2() const {
        return mFlags & FLAG_CREATED;
    }

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u32 mFlags; //0x1C4
};
/* end "monolib/device/CDeviceBase.hpp" */
/* "libs/monolib/include/monolib/device/CDeviceFile.hpp" line 5 "monolib/monolib_types.hpp" */
/* end "monolib/monolib_types.hpp" */
/* "libs/monolib/include/monolib/device/CDeviceFile.hpp" line 6 "monolib/util.hpp" */
/* end "monolib/util.hpp" */

//size: 0x1f0
class CDeviceFile : public CDeviceBase {
public:
    CDeviceFile(const char* pName, CWorkThread* pParent);
    static CDeviceFile* getInstance();

    DECL_WORKTHREAD_CREATE(CDeviceFile);

    static bool isInitialized();
    static bool func_8044E768();

    static CFileHandle* readFile(mtl::ALLOC_HANDLE allocHandle, const char* pPath, IWorkEvent* pWorkEvent, int r6, int r7);
    static CFileHandle* readCommonArchiveFile(mtl::ALLOC_HANDLE allocHandle, const char* pPath, IWorkEvent* pWorkEvent, int r6, int r7);
    static int getFileSize(const char* pPath);

    static void removeFileJob(CDeviceFileJob* pJob);
    static void cancel(CFileHandle* pFileHandle);
    static void func_8044F154(CFileHandle* pFileHandle, int);
    static void setHandleFlag1(CFileHandle* pFileHandle);
    static void setHandleFlag2(CFileHandle* pFileHandle);

    //0x0: vtable
    //0x0-1c8: CDeviceBase
    u8 unk1C8[0x1F0 - 0x1C8];
};
/* end "monolib/device/CDeviceFile.hpp" */
/* "src/kyoshin/CUICfManager.cpp" line 3 "monolib/util/MemManager.hpp" */
/* end "monolib/util/MemManager.hpp" */

/* "src/kyoshin/CUICfManager.cpp" line 5 "decomp.h" */
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

// §17.6 whole-function asm: packed tail copy + -0x1A0/stmw frame not recoverable
// in high-level C++ (60% STRUCTURAL). User-approved "Fix it".

extern "C" {
CProcess* lbl_eu_80664054;
const char* lbl_eu_806621A8;
char lbl_eu_8052E404[];
char lbl_eu_8052E3BC[];
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void* getWorkMem__17CWorkThreadSystemFv(void);
void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
CFileHandle* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, IWorkEvent*, int, int);
void func_8044F154__11CDeviceFileFP11CFileHandlei(CFileHandle*, int);
void Regist__8CProcessFP8CProcessb(CProcess*, CProcess*, bool);
void func_8015704C(void*, const void*);
void func_8009D0B4();
void func_8009D514(cf::IFlagEvent*);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);
}

asm void CUICfManager::Init() {
    nofralloc
    stwu r1, -0x1a0(r1)
    mflr r0
    cmpwi r3, 0x0
    stw r0, 0x1a4(r1)
    stmw r22, 0x178(r1)
    mr r23, r3
    mr r5, r23
    beq lbl_80132EEC
    addi r5, r3, 0x54
lbl_80132EEC:
    lwz r3, 0x118(r3)
    li r6, 0x0
    lwz r4, lbl_eu_806621A8
    li r7, 0x0
    bl readFile__11CDeviceFileFUlPCcP10IWorkEventii
    stw r3, 0x114(r23)
    li r4, 0x3
    bl func_8044F154__11CDeviceFileFP11CFileHandlei
    bl getWorkMem__17CWorkThreadSystemFv
    mr r4, r3
    li r3, 0x54
    bl allocate__Q23mtl10MemManagerFUlUl
    cmpwi r3, 0x0
    mr r22, r3
    beq lbl_80132F7C
    bl __ct__8CProcessFv
    lis r3, lbl_eu_8052E404@ha
    lis r5, __ptmf_null@ha
    addi r3, r3, lbl_eu_8052E404@l
    stw r3, 0x10(r22)
    addi r5, r5, __ptmf_null@l
    lwz r0, 0x4(r5)
    lis r3, lbl_eu_8052E3BC@ha
    lwz r4, 0x0(r5)
    addi r3, r3, lbl_eu_8052E3BC@l
    stw r4, 0x3c(r22)
    stw r0, 0x40(r22)
    lwz r0, 0x8(r5)
    stw r0, 0x44(r22)
    lwz r0, 0x4(r5)
    lwz r4, 0x0(r5)
    stw r4, 0x48(r22)
    stw r0, 0x4c(r22)
    lwz r0, 0x8(r5)
    stw r0, 0x50(r22)
    stw r3, 0x10(r22)
lbl_80132F7C:
    stw r22, 0x144(r23)
    mr r3, r22
    li r5, 0x0
    lwz r4, lbl_eu_80664054
    bl Regist__8CProcessFP8CProcessb
    addi r27, r1, 0xdc
    addi r26, r1, 0x40
    addi r25, r1, 0x74
    addi r24, r1, 0xa8
    li r28, 0x0
    li r30, 0x0
    li r31, 0x7
    li r22, 0x34
lbl_80132FB0:
    clrlwi r0, r28, 24
    addi r3, r1, 0x40
    mulli r0, r0, 0x168
    sth r30, 0x10(r1)
    cmplw r3, r27
    stw r30, 0xc(r1)
    add r29, r23, r0
    bge lbl_80132FF4
    addi r0, r27, 0x33
    subf r0, r3, r0
    divwu r0, r0, r22
    mtctr r0
    bge lbl_80132FF4
lbl_80132FE4:
    sth r30, 0x4(r3)
    stw r30, 0x0(r3)
    addi r3, r3, 0x34
    bdnz lbl_80132FE4
lbl_80132FF4:
    stb r30, 0x9(r1)
    addi r3, r29, 0x150
    addi r4, r1, 0xc
    stw r30, 0xdc(r1)
    sth r30, 0xe0(r1)
    sth r30, 0xe2(r1)
    sth r30, 0xe4(r1)
    sth r30, 0xe6(r1)
    sth r30, 0xe8(r1)
    sth r30, 0xea(r1)
    stb r30, 0xec(r1)
    stb r30, 0xee(r1)
    stb r30, 0x12e(r1)
    stb r31, 0x8(r1)
    stb r31, 0x14c(r29)
    lbz r0, 0x9(r1)
    stb r0, 0x14d(r29)
    bl func_8015704C
    mr r4, r26
    addi r3, r29, 0x184
    bl func_8015704C
    mr r4, r25
    addi r3, r29, 0x1b8
    bl func_8015704C
    mr r4, r24
    addi r3, r29, 0x1ec
    bl func_8015704C
    lwz r0, 0xdc(r1)
    addi r28, r28, 0x1
    stw r0, 0x220(r29)
    cmplwi r28, 0x8
    lwz r0, 0xe4(r1)
    lwz r3, 0xe0(r1)
    stw r3, 0x224(r29)
    stw r0, 0x228(r29)
    lhz r0, 0xe8(r1)
    sth r0, 0x22c(r29)
    lwz r0, 0xea(r1)
    stw r0, 0x22e(r29)
    lwz r0, 0xf2(r1)
    lwz r3, 0xee(r1)
    stw r3, 0x232(r29)
    stw r0, 0x236(r29)
    lwz r0, 0xfa(r1)
    lwz r3, 0xf6(r1)
    stw r3, 0x23a(r29)
    stw r0, 0x23e(r29)
    lwz r0, 0x102(r1)
    lwz r3, 0xfe(r1)
    stw r3, 0x242(r29)
    stw r0, 0x246(r29)
    lwz r0, 0x10a(r1)
    lwz r3, 0x106(r1)
    stw r3, 0x24a(r29)
    stw r0, 0x24e(r29)
    lwz r0, 0x112(r1)
    lwz r3, 0x10e(r1)
    stw r3, 0x252(r29)
    stw r0, 0x256(r29)
    lwz r0, 0x11a(r1)
    lwz r3, 0x116(r1)
    stw r3, 0x25a(r29)
    stw r0, 0x25e(r29)
    lwz r0, 0x122(r1)
    lwz r3, 0x11e(r1)
    stw r3, 0x262(r29)
    stw r0, 0x266(r29)
    lwz r0, 0x12a(r1)
    lwz r3, 0x126(r1)
    stw r3, 0x26a(r29)
    stw r0, 0x26e(r29)
    lwz r0, 0x132(r1)
    lwz r3, 0x12e(r1)
    stw r3, 0x272(r29)
    stw r0, 0x276(r29)
    lwz r0, 0x13a(r1)
    lwz r3, 0x136(r1)
    stw r3, 0x27a(r29)
    stw r0, 0x27e(r29)
    lwz r0, 0x142(r1)
    lwz r3, 0x13e(r1)
    stw r3, 0x282(r29)
    stw r0, 0x286(r29)
    lwz r0, 0x14a(r1)
    lwz r3, 0x146(r1)
    stw r3, 0x28a(r29)
    stw r0, 0x28e(r29)
    lwz r0, 0x152(r1)
    lwz r3, 0x14e(r1)
    stw r3, 0x292(r29)
    stw r0, 0x296(r29)
    lwz r0, 0x15a(r1)
    lwz r3, 0x156(r1)
    stw r3, 0x29a(r29)
    stw r0, 0x29e(r29)
    lwz r0, 0x162(r1)
    lwz r3, 0x15e(r1)
    stw r3, 0x2a2(r29)
    stw r0, 0x2a6(r29)
    lwz r0, 0x16a(r1)
    lwz r3, 0x166(r1)
    stw r3, 0x2aa(r29)
    stw r0, 0x2ae(r29)
    blt lbl_80132FB0
    lmw r22, 0x178(r1)
    lwz r0, 0x1a4(r1)
    mtlr r0
    addi r1, r1, 0x1a0
    blr
}

void CUICfManager::Term() {
    func_801390E0(&mFileHandle);

    mtl::MemManager::deallocate(mPackedFont60.Destroy());
    mtl::MemManager::deallocate(mPackedFont9C.Destroy());
    mtl::MemManager::deallocate(mPackedFontD8.Destroy());

    func_80139124(mArcResourceAccessor);
    mArcResourceAccessor = NULL;
    unk118 = -1;

    cf::IFlagEvent* flagEvent = this;
    func_8009D0B4();
    func_8009D514(flagEvent);

    unk144->unk39 = 1;
    lbl_eu_80664054 = NULL;
}

// ---------------------------------------------------------------------------
// func_80133324__12CUICfManagerFv
//
// Retail mangles this as a no-arg CUICfManager member (`Fv`) but the body
// reads r4/r5/r6 as real event-dispatch arguments and never touches `this`
// (all state comes from the `lbl_eu_80664054` singleton). Declared as a free
// `extern "C"` function so the ABI is r3=<unused self>, r4=id, r5=a1, r6=a2.
//
// Event-queue insert is the same shape as reslist::push_back -> setItem
// (try/catch forces -0x80 / mr r31,r1 / stw r1 epilogue). Leaf body order
// matches retail text: 221 -> 312c -> 7fc -> 22 -> 609.
/* "src/kyoshin/CUICfManager.cpp" line 252 "monolib/util/reslist.hpp" */
/* end "monolib/util/reslist.hpp" */

extern "C" {
int lbl_eu_80664050;
int func_80138138(int);
u32 func_8013B87C(u8);
int func_8014A1D4(void*, u32, u8, int);
void func_8013DA60(int, int, int);
void* func_8009EC9C(u16);
void func_800A21F8(void*, u16, int, int);
u32 func_801361E8(void*, char*, u32);
u16 func_8013606C(char*, char*, u16);
void* lbl_eu_80573D18[];
u8 lbl_eu_805000A8[];
u16 lbl_eu_804FFFDC[];
}

// C++ mangling -> retail `func_8013B428__FUl`.
void func_8013B428(u32);

void func_80133324__12CUICfManagerFv(CUICfManager* self, int id, int a1, int a2) {
    // Decl order: savedRet@0x8, gap, setItem stw-r1 home, idTable@0x28 (retail frame).
    int savedRet;
    int pad0C;
    int pad10;
    int pad14;
    int pad18;
    int pad1C;
    int pad20;
    CUICfIdTable idTable;
    u8 codePersist;

    if (a1 == a2) {
        goto end;
    }

    if (id >= 0x798) {
        goto ge798;
    }

    if (id >= 0x221) {
        goto ge221;
    }

    if (id >= 0x28) {
        goto end;
    }
    if (id >= 0x22) {
        goto range_22_27;
    }
    goto end;

ge221:
    if (id == 0x608) {
        goto end;
    }
    if (id >= 0x608) {
        goto range_609_797;
    }
    goto range_221_607;

ge798:
    if (id >= 0x312c) {
        goto ge312c;
    }
    if (id >= 0x805) {
        goto end;
    }
    if (id >= 0x7fc) {
        goto range_7fc_804;
    }
    goto end;

ge312c:
    if (id >= 0x31f4) {
        goto end;
    }
    goto range_312c_31f3;

range_221_607: {
    u32 check = (u32)(a1 - 0xfe);
    int off = id - 0x220;
    lbl_eu_80664050 = off;
    if (check > 1) {
        goto end;
    }

    int idx = func_80138138(off);
    void* tableVal = lbl_eu_80573D18[idx];
    u8 res = (u8)func_801361E8(tableVal, (char*)lbl_eu_805000A8 + 0x43, lbl_eu_80664050);
    if (res == 2) {
        goto end;
    }

    func_8013DA60(lbl_eu_80664050, 1, 0);

    idTable = *(const CUICfIdTable*)lbl_eu_804FFFDC;
    {
        int target = lbl_eu_80664050;
        u8 i = 0;
        goto id_check;
    id_body:
        if ((u16)idTable.ids[i] == target) {
            func_8013B428(0xb9);
            goto end;
        }
        i++;
    id_check:
        if (idTable.ids[i] != 0) {
            goto id_body;
        }
    }
    goto end;
}

range_312c_31f3: {
    if (a1 == 0) {
        goto end;
    }
    if (a2 != 0) {
        goto end;
    }

    {
        u32 diff = id - 0x312c;
        u32 code = 0xc8;
        if ((u8)diff != 0) {
            code = (u8)diff;
        }
        codePersist = (u8)code;
    }

    func_8013B87C(codePersist);

    CUICfManager* inst = (CUICfManager*)lbl_eu_80664054;
    if (inst != NULL) {
        savedRet = func_8014A1D4(inst->unk144, inst->unk11C, codePersist, 1);
        if (savedRet != 0) {
            inst = (CUICfManager*)lbl_eu_80664054;

            _reslist_node<u32>* startNode = (_reslist_node<u32>*)inst->unk128;
            int capacity = inst->unk13C;
            int i = 0;
            int byteOff = 0;
            // Keep pads "live" enough that MWCC does not DCE them entirely.
            pad0C = capacity;
            pad10 = byteOff;
            pad14 = i;
            pad18 = pad0C;
            pad1C = pad10;
            pad20 = pad14;
            goto slot_check;
        slot_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto slot_found;
            }
            byteOff += 0xc;
            i++;
        slot_check:
            if (i < capacity) {
                goto slot_body;
            }
        slot_found:
            {
                _reslist_node<u32>* temp = (_reslist_node<u32>*)((u8*)inst->unk138 + i * 0xc);
                temp->setItem((u32)savedRet);
                temp->mNext = startNode;
                temp->mPrev = startNode->mPrev;
                startNode->mPrev->mNext = temp;
                startNode->mPrev = temp;
            }
        }
    }

    {
        u16 ret2 = func_8013606C((char*)lbl_eu_805000A8 + 0x4d, (char*)lbl_eu_805000A8 + 0x5b,
                                   codePersist);
        if (ret2 != 0) {
            int i = 1;
            goto party_check;
        party_body:
            {
                void* r = func_8009EC9C((u16)i);
                func_800A21F8(r, ret2, 0, 0);
                i++;
            }
        party_check:
            if (i <= 8) {
                goto party_body;
            }
        }
    }
    goto end;
}

range_7fc_804: {
    int rel = id - 0x7fc;
    if (rel == 0) {
        goto case_7fc;
    }
    if (rel == 1) {
        goto case_7fd;
    }
    goto end;
}

case_7fc:
    if (a1 == 0x64) {
        func_8013B428(0xc8);
        goto end;
    }
    if (a1 >= 0x32) {
        func_8013B428(0xc7);
        goto end;
    }
    if (a1 >= 1) {
        func_8013B428(0xc6);
    }
    goto end;

case_7fd:
    if (a1 == 0x96) {
        func_8013B428(0xc5);
        goto end;
    }
    if (a1 >= 0x64) {
        func_8013B428(0xc4);
        goto end;
    }
    if (a1 >= 0x32) {
        func_8013B428(0xc3);
    }
    goto end;

range_22_27:
    if (a1 < 0xfa0) {
        goto end;
    }
    func_8013B428((u8)(id + 0x81));
    if (a1 < 0x1f40) {
        goto end;
    }
    func_8013B428(0xa8);
    func_8013B428(0xa9);
    goto end;

range_609_797:
    if (a2 == 0) {
        if (a1 > 0) {
            func_8013B428(0x9f);
            func_8013B428(0xa0);
            func_8013B428(0xa1);
            func_8013B428(0xa2);
        }
    }
    if (a1 < 5) {
        goto end;
    }
    func_8013B428(0x9e);
    goto end;

end:
    return;
}

// ---------------------------------------------------------------------------
// CUICfManager::Move
//
// Early r4 is NOT a fake-Fv arg — retail does `lhz r4, 0xc90(r3)` (mFlags).
// Bitflag-driven create/teardown against lbl_eu_80664054, then optional
// enum-list proximity spawn, then mark/clear walks of the menu queue.
// ---------------------------------------------------------------------------

/* "src/kyoshin/CUICfManager.cpp" line 525 "monolib/device/CDeviceVI.hpp" */
#pragma once

/* "libs/monolib/include/monolib/device/CDeviceVI.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/device/CDeviceVI.hpp" line 3 "monolib/device/CDeviceBase.hpp" */
/* end "monolib/device/CDeviceBase.hpp" */
/* "libs/monolib/include/monolib/device/CDeviceVI.hpp" line 4 "monolib/device/CDeviceVICb.hpp" */
#pragma once

/* "libs/monolib/include/monolib/device/CDeviceVICb.hpp" line 2 "types.h" */
/* end "types.h" */

//size: 0x4
class CDeviceVICb {
public:
    enum VICallback {
        VI_CALLBACK_BEFORE_DRAW_DONE,
        VI_CALLBACK_AFTER_DRAW_DONE,
        VI_CALLBACK_BEGIN_FRAME
    };

    CDeviceVICb();
    virtual ~CDeviceVICb();
    virtual void viBeforeDrawDone(){}
    virtual void viAfterDrawDone(){}
    virtual void viBeginFrame(){}
};
/* end "monolib/device/CDeviceVICb.hpp" */
/* "libs/monolib/include/monolib/device/CDeviceVI.hpp" line 5 "monolib/util.hpp" */
/* end "monolib/util.hpp" */
/* "libs/monolib/include/monolib/device/CDeviceVI.hpp" line 6 "monolib/math.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math.hpp" line 2 "monolib/math/CAttrTransform.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 3 "monolib/math/CVec3.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CVec3.hpp" line 2 "monolib/math/Utility.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/Utility.hpp" line 2 "nw4r/math.h" */
/* end "nw4r/math.h" */

#define PI 3.14159265f
#define DEG2RAD(x) ((x)*(PI/180.0f))

//Utility math functions
namespace ml{
    namespace math{

    inline float abs(float x){
        return (double)nw4r::math::FAbs(x);
    }

    inline float sqrt(float x){
        return nw4r::math::FSqrt(x);
    }

    inline float clamp(float x, float min, float max) {
        if (x < min) x = min;
        else if(x > max) x = max;
        return x;
    }

    inline float sin(float x){
        return nw4r::math::SinRad(x);
    }

    inline float cos(float x){
        return nw4r::math::CosRad(x);
    }

    inline void sincos(float x, float& sinX, float& cosX){
        sinX = sin(x);
        cosX = cos(x);
    }

    inline float asin(float x){
        x = clamp(x, -1.0f, 1.0f);
        return nw4r::math::AsinRad(x);
    }

    inline float acos(float x){
        x = clamp(x, -1.0f, 1.0f);
        return nw4r::math::AcosRad(x);
    }

    inline float atan2(float y, float x){
        return nw4r::math::Atan2FIdx(y, x);
    }

    inline float atan2Deg(float y, float x){
        return nw4r::math::Atan2Deg(y, x);
    }

    inline float atan2Rad(float y, float x){
        return nw4r::math::Atan2Rad(y, x);
    }

    inline float dot(const float* vec){
        return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    }

    } //namespace math
} //namespace ml
/* end "monolib/math/Utility.hpp" */
/* "libs/monolib/include/monolib/math/CVec3.hpp" line 3 "monolib/math/MathConstants.hpp" */
#pragma once

namespace ml{
    //General math constants
    extern const float pi;
    extern const float tau;
    extern const float halfpi;
    extern const float quarterpi;
    extern const float epsilon;
    extern const float rad2deg;
    extern const float deg2rad;
    extern const float huge;
    extern const float hugeminus;
} //namespace ml
/* end "monolib/math/MathConstants.hpp" */
/* "libs/monolib/include/monolib/math/CVec3.hpp" line 4 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    //Possibly inherits from nw4r VEC3?
    struct CVec3 {
        static CVec3 zero;
        static CVec3 unitX;
        static CVec3 unitY;
        static CVec3 unitZ;
        static CVec3 unit;

        CVec3(){}

        CVec3(float x, float y, float z){
            set(x, y, z);
        }

        CVec3(const CVec3& vec){
            set(vec.x, vec.y, vec.z);
        }

        //Conversion functions for converting to the SDK/NW4R vector types.
        operator Vec*(){
            return reinterpret_cast<Vec*>(this);
        }

        operator const Vec*() const {
            return reinterpret_cast<const Vec*>(this);
        }

        operator nw4r::math::VEC3*(){
            return reinterpret_cast<nw4r::math::VEC3*>(this);
        }

        operator const nw4r::math::VEC3*() const {
            return reinterpret_cast<const nw4r::math::VEC3*>(this);
        }

        void set(float x, float y, float z){
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void set(const CVec3& vec){
            x = vec.x;
            y = vec.y;
            z = vec.z;
        }

        void setZero(){
            *this = zero;
        }
        
        CVec3 operator-() const {
            return CVec3(-x, -y, -z);
        }

        CVec3 operator+(const CVec3& rhs) const {
            CVec3 out;
            //add(out, *this, rhs);
            nw4r::math::VEC3Add(out, *this, rhs);
            return out;
        }
        CVec3 operator-(const CVec3& rhs) const {
            CVec3 out;
            sub(out, *this, rhs);
            //nw4r::math::VEC3Sub(out, *this, rhs);
            return out;
        }
        CVec3 operator*(float x) const {
            CVec3 out;
            scale(out, *this, x);
            return out;
        }
        CVec3 operator/(float x) const {
            float r = 1/x;
            return *this * r;
        }

        CVec3& operator+=(const CVec3& rhs) {
            add(*this, *this, rhs);
            return *this;
        }
        CVec3& operator-=(const CVec3& rhs) {
            sub(*this, *this, rhs);
            return *this;
        }
        CVec3& operator*=(float x) {
            scale(*this, *this, x);
            return *this;
        }
        CVec3& operator/=(float x) {
            return *this *= (1/x);
        }

        bool operator==(const CVec3& vec) const {
            return x == vec.x && y == vec.y && z == vec.z;
        }

        bool operator!=(const CVec3& vec) const {
            return x != vec.x || y != vec.y || z != vec.z;
        }

        void normalize(){
            if(!isZero()) {
                normalizeSub();
            }else{
                set(0,0,1);
            }
        }

        //Unofficial
        void normalizeSub(){
            if(x*x + y*y + z*z == 0.0f){
                setZero();
            }else {
                PSVECNormalize(*this,*this);
            }
        }
        
        //Unofficial
        float magnitude() const {
            return x*x + y*y + z*z;
        }

        float getLength() const {
            return math::sqrt(magnitude());
        }

        bool isZero() const {
            //TODO: this can't be it, right???
            bool result = false;
            bool temp = false;
            if(math::abs(x) <= epsilon && math::abs(y) <= epsilon){
                temp = true;
            }
            if(temp && math::abs(z) <= epsilon) result = true;
            return result;
        }

        bool isErr() const;

        //TODO: properly figure out these asm inlines

        static float dot(const CVec3& lhs, const CVec3& rhs) {
            return nw4r::math::VEC3Dot(lhs, rhs);
        }
        
        static void add(CVec3& outVec, const CVec3& lhs, const CVec3& rhs){
            CVec3 temp;
            nw4r::math::VEC3Add(temp, lhs, rhs);
            outVec.set(temp);
        }

        static void sub(CVec3& outVec, const CVec3& lhs, const CVec3& rhs){
            CVec3 temp;
            nw4r::math::VEC3Sub(temp, lhs, rhs);
            outVec.set(temp);
        }

        static void scale(CVec3& outVec, const CVec3& vec, float scale){
            CVec3 temp;
            nw4r::math::VEC3Scale(temp, vec, scale);
            outVec.set(temp);
        }

        static void cross(CVec3& outVec, const CVec3& lhs, const CVec3& rhs){
            CVec3 temp;
            nw4r::math::VEC3Cross(temp, lhs, rhs);
            outVec.set(temp);
        }
        
        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float x;
            float y;
            float z;
        };

    };

} //namespace ml
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 4 "monolib/math/CMat34.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CMat34.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CMat34.hpp" line 3 "monolib/math/CQuat.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CQuat.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CQuat.hpp" line 3 "monolib/math/MathConstants.hpp" */
/* end "monolib/math/MathConstants.hpp" */
/* "libs/monolib/include/monolib/math/CQuat.hpp" line 4 "monolib/math/Utility.hpp" */
/* end "monolib/math/Utility.hpp" */
/* "libs/monolib/include/monolib/math/CQuat.hpp" line 5 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CQuat{
        CQuat(){}

        CQuat(float x, float y, float z, float w){
            set(x,y,z,w);
        }

        operator Quaternion*(){
            return reinterpret_cast<Quaternion*>(this);
        }

        operator const Quaternion*() const{
            return reinterpret_cast<const Quaternion*>(this);
        }

        CQuat& operator*=(const CQuat& other){
            PSQUATMultiply(*this, other, *this);
            return *this;
        }

        inline void set(float x, float y, float z, float w){
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        inline void setIdentity(){
            set(0,0,0,1);
        }
  
        //Sets the quaternion from the given euler angle, following the 3-2-1 conversion.
        inline void setRotXYZ(const CVec3* angle){
            float x = angle->x * 0.5f;
            float y = angle->y * 0.5f;
            float z = angle->z * 0.5f;
            float sinX = math::sin(x);
            float cosX = math::cos(x);
            float sinY = math::sin(y);
            float cosY = math::cos(y);
            float sinZ = math::sin(z);
            float cosZ = math::cos(z);

            this->x = cosZ*(sinX*cosY) - sinZ*(cosX*sinY);
            this->y = cosZ*(cosX*sinY) + sinZ*(sinX*cosY);
            this->z = sinZ*(cosX*cosY) - cosZ*(sinX*sinY);
            this->w = cosZ*(cosX*cosY) + sinZ*(sinX*sinY);
        }

        //Converts this quaternion to euler angles, storing the result in the given vector.
        inline void getRotXYZ(CVec3* result) const {
            //So many variables :p
            float twoX = x + x;
            float twoY = y + y;
            float twoZ = z + z;

            float twoXZ = x * twoZ;
            float twoYW = w * twoY;

            //NOTE: Unnecessary clamp
            float input = -(twoXZ - twoYW);
            if(input >= 1) input = 1;
            else if(input <= -1) input = -1;
            //y = asin(-(2xz - 2yw))
            float angle = math::asin(input);
            result->y = angle;

            float twoX2 = x * twoX;
            float twoXY = x * twoY;
            float twoZ2 = z * twoZ;
            float twoZW = w * twoZ;
                    
            if (angle < halfpi) {
                if (angle > -halfpi) {
                    //-pi/2 < angle < pi/2
                    float twoY2 = y * twoY;
                    float twoYZ = y * twoZ;
                    float twoXW = w * twoX;
                    //x = atan2(2yz + 2xw, 1 - (2x^2 + 2y^2))
                    result->x = math::atan2Rad(twoYZ + twoXW, 1.0f - (twoX2 + twoY2));
                    //z = atan2(2xy + 2zw, 1 - (2y^2 + 2z^2))
                    result->z = math::atan2Rad(twoXY + twoZW, 1.0f - (twoY2 + twoZ2));
                }else{
                    //angle > pi/2
                    //x = -atan2(2xy - 2zw, 1 - (2x^2 + 2z^2))
                    result->x = -math::atan2Rad(twoXY - twoZW, 1.0f - (twoX2 + twoZ2));
                    result->z = 0;
                }
            }else{
                //angle < -pi/2
                //x = atan2(2xy - 2zw, 1 - (2x^2 + 2z^2))
                result->x = math::atan2Rad(twoXY - twoZW, 1.0f - (twoX2 + twoZ2));
                result->z = 0;
            }
        }

        static void slerp(CQuat& outQuat, const CQuat& a, const CQuat& b, float t);
        void setRotZXY(const CVec3* angle);
        void getRotZXY(CVec3* result) const;


        struct{
        float x;
        float y;
        float z;
        float w;
        };

        static CQuat zero;
        static CQuat identity;

    };

} //namespace ml
/* end "monolib/math/CQuat.hpp" */
/* "libs/monolib/include/monolib/math/CMat34.hpp" line 4 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CMat34{
        CMat34(){}

        CMat34(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23){
            set(
                m00,m01,m02,m03,
                m10,m11,m12,m13,
                m20,m21,m22,m23
            );
        }

        void set(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23){
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
        }

        operator nw4r::math::MTX34*(){
            return reinterpret_cast<nw4r::math::MTX34*>(this);
        }

        operator const nw4r::math::MTX34*() const {
            return reinterpret_cast<const nw4r::math::MTX34*>(this);
        }

        //TODO: can this be included without interfering with the above operators?
        /*
        operator Mtx*(){
            return &m;
        }

        operator const Mtx*() const {
            return &m;
        }
        */

        CMat34 operator*(CMat34& rhs) const {
            CMat34 mat;
            mul(mat, *this, rhs);
            return mat;
        }

        void mul(CVec3& outVec, const CVec3& vec) const {
            PSMTXMultVec(mtx, vec, outVec);
        }


        static void mul(CMat34& outMat, const CMat34& mat1, const CMat34& mat2){
            PSMTXConcat(mat2.mtx, mat1.mtx, outMat.mtx);
        }

        void setUnit(){
            PSMTXIdentity(mtx);
        }

        void setScale(float x, float y, float z){
            set(
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0
            );
        }

        void setScale(const CVec3& scale){
            setScale(scale.x, scale.y, scale.z);
        }

        void setRotX(float x){
            float sinX, cosX;
            math::sincos(x, sinX, cosX);

            set(
            1, 0,    0,     0,
            0, cosX, -sinX, 0,
            0, sinX, cosX,  0
            );
        }

        void setRotY(float y){
            float sinY, cosY;
            math::sincos(y, sinY, cosY);

            set(
            cosY,  0, sinY, 0,
            0,     1, 0,    0,
            -sinY, 0, cosY, 0
            );
        }

        void setRotZ(float z){
            float sinZ, cosZ;
            math::sincos(z, sinZ, cosZ);

            set(
            cosZ, -sinZ, 0, 0,
            sinZ, cosZ,  0, 0,
            0,    0,     1, 0
            );
        }

        void addRotX(float x){
            CMat34 mat;
            mat.setRotX(x);
            mul(*this, *this, mat);
        }

        void addRotY(float y){
            CMat34 mat;
            mat.setRotY(y);
            mul(*this, *this, mat);
        }

        void addRotZ(float z){
            CMat34 mat;
            mat.setRotZ(z);
            mul(*this, *this, mat);
        }

        void getRotQuat(CQuat& quat) const{
            CQuat temp;
            C_QUATMtx(temp, mtx);
            quat = temp;
        }

        void setRotQuat(const CQuat& quat){
            PSMTXQuat(mtx, quat);
        }

        void setRotXYZ(const CVec3& angle){
            nw4r::math::MTX34RotXYZRad(*this, angle.x, angle.y, angle.z);
        }

        void setRotZXY(const CVec3& angle){
            setRotZ(angle.z);
            addRotX(angle.x);
            addRotY(angle.y);
        }

        void invert(CMat34* outMat){
            PSMTXInverse(mtx, outMat->mtx);
        }

        CVec3 getTranslation(){
            return CVec3(m[0][3], m[1][3], m[2][3]);
        }

        void addTranslation(const CVec3& vec){
            m[0][3] += vec.x;
            m[1][3] += vec.y;
            m[2][3] += vec.z;
        }


        void replaceTranslation(const CVec3& vec){
            m[0][3] = vec.x;
            m[1][3] = vec.y;
            m[2][3] = vec.z;
        }

        void getRotAxis(CVec3& vec, float* outAngle) const;
        bool getRotXYZ(CVec3& vec) const;
        bool getRotZXY(CVec3& vec) const;
        bool getRotZYX(CVec3& vec) const;

        union {
            float m[3][4];
            Mtx mtx;
        };

        static CMat34 zero;
        static CMat34 identity;
    };
} //namespace ml
/* end "monolib/math/CMat34.hpp" */
/* "libs/monolib/include/monolib/math/CAttrTransform.hpp" line 5 "monolib/math/CQuat.hpp" */
/* end "monolib/math/CQuat.hpp" */

namespace ml{
    struct CAttrTransform{
        enum Flags{
            FLAG_0 = 1 << 0,
            FLAG_1 = 1 << 1,
            FLAG_2 = 1 << 2,
            FLAG_USE_ZXY = 1 << 3
        };

        CAttrTransform(){
            clear();
        }

        void clear();
        void update();

        CVec3 mPos; //0x0
        CVec3 mRot; //0xC
        CMat34 mMtx1; //0x18
        CMat34 mLocalMat; //0x48
        CMat34 mLocalMatInv; //0x78
        CQuat unkA8;
        CVec3 mPrevPos; //0xB8
        CVec3 mPrevRot; //0xC4
        CQuat mLocalQuat; //0xD0
        u32 mFlags; //0xE0
    };
} //namespace ml
/* end "monolib/math/CAttrTransform.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 3 "monolib/math/CCamUtil.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CCamUtil.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */

namespace ml {
    struct CCamUtil {
        static void getXYZ2ZXY(CVec3& outVec, const CVec3& vec);
    };
} //namespace ml
/* end "monolib/math/CCamUtil.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 4 "monolib/math/CCol3.hpp" */
#pragma once

namespace ml {
    struct CCol3{
        CCol3(){}
        CCol3(float r, float g, float b){
            set(r, g, b);
        }

        void set(float r, float g, float b){
            this->r = r;
            this->g = g;
            this->b = b;
        }

        void clamp(float min, float max){
            if(r > max) r = max;
            else if(r < min) r = min;
            if(g > max) g = max;
            else if(g < min) g = min;
            if(b > max) b = max;
            else if(b < min) b = min;
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float r;
            float g;
            float b;
        };

        static CCol3 white;
        static CCol3 gray;
        static CCol3 black;
        static CCol3 red;
        static CCol3 green;
        static CCol3 blue;
        static CCol3 yellow;
        static CCol3 cyan;
        static CCol3 magenta;
        static CCol3 salmon;
        static CCol3 orange;
    };
} //namespace ml
/* end "monolib/math/CCol3.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 5 "monolib/math/CCol4.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CCol4.hpp" line 2 "types.h" */
/* end "types.h" */


#define COLOR_TO_U32(r, g, b, a) (((u8)(255 * r) << 24) | ((u8)(255 * g) << 16) | ((u8)(255 * b) << 8) | (u8)(255 * a))

namespace ml {
    
    struct CCol4{
        CCol4(){}
        CCol4(float r, float g, float b, float a){
            set(r,g,b,a);
        }

        void set(float r, float g, float b, float a){
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        void clamp(float min, float max){
            if(r > max) r = max;
            else if(r < min) r = min;
            if(g > max) g = max;
            else if(g < min) g = min;
            if(b > max) b = max;
            else if(b < min) b = min;
            if(a > max) a = max;
            else if(a < min) a = min;
        }

        u32 toU32() const {
            return COLOR_TO_U32(r, g, b, a);
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float r;
            float g;
            float b;
            float a;
        };

        static CCol4 white;
        static CCol4 gray;
        static CCol4 black;
        static CCol4 red;
        static CCol4 green;
        static CCol4 blue;
        static CCol4 yellow;
        static CCol4 cyan;
        static CCol4 magenta;
        static CCol4 salmon;
        static CCol4 orange;
        static CCol4 zero;

    };

} //namespace ml
/* end "monolib/math/CCol4.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 6 "monolib/math/CFrustum.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 3 "monolib/math/CMat34.hpp" */
/* end "monolib/math/CMat34.hpp" */
/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 4 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CFrustum.hpp" line 5 "monolib/math/CPlane.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CPlane.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */

namespace ml {
    struct CPlane {
        CPlane(){
        }
        CPlane(const CVec3& pos, const CVec3& p1, const CVec3& p2){
            set(pos, p1, p2);
        }

        CPlane* set(const CVec3& pos, const CVec3& p1, const CVec3& p2);
        static void getCross(CVec3& outVec, const CPlane& plane, const CVec3& rayOrigin, const CVec3& rayDir);

        void normalize(){
            mNormal.normalizeSub();
        }

        CVec3 convertRayToNormal(const CVec3& rayStartPos, const CVec3& rayEndPos) const {
            CVec3 normal = rayEndPos - rayStartPos;
            normal.normalizeSub();
            return normal;
        }

        //Sets this plane from the given ray start/end position.
        void set(const CVec3& rayStartPos, const CVec3& rayEndPos){
            mNormal = convertRayToNormal(rayStartPos, rayEndPos);
            mDist = -CVec3::dot(rayStartPos, mNormal);
        }

        float getPointDistance(const CVec3& pos) const {
            return CVec3::dot(pos, mNormal) + mDist;
        }
        
        bool isWithinDistance(const CVec3& vec, float distance) const {
            if(getPointDistance(vec) < distance) return true;
            else return false;
        }

        bool isOnNegativeSide(const CVec3& vec) const {
            if(getPointDistance(vec) < 0) return true;
            else return false;
        }

        bool isOnPositiveSide(const CVec3& vec) const {
            if(getPointDistance(vec) >= 0) return true;
            else return false;
        }

        //Determines if the points are on different sides of the plane.
        bool isDifferentSide(const CVec3& vec1, const CVec3& vec2) const {
            float f0 = getPointDistance(vec2);
            float f1 = getPointDistance(vec1);
            
            bool r3 = f1 >= 0;
            bool r0 = f0 >= 0;
            //TODO: is this some weird inline for calculating if only one of two conditions are true?
            return r3 ^ r0;
        }

        CVec3 mNormal; //0x0
        float mDist; //0x4
    };

} //namespace ml
/* end "monolib/math/CPlane.hpp" */

//TODO: idk if this belongs here in monolib or in the scene code. There's no evidence of it in XCX
namespace ml{

    struct CFrustum{
        u8 unk0[0xCC];
        ml::CMat34 unkCC;
        u8 unkFC[0x10];
        ml::CVec3 unk10C;
        u8 unk118[0x258 - 0x118];
        ml::CPlane unk248[6];
    };

} //namespace ml
/* end "monolib/math/CFrustum.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 7 "monolib/math/CMat33.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CMat33.hpp" line 2 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math/CMat33.hpp" line 3 "monolib/math/Utility.hpp" */
/* end "monolib/math/Utility.hpp" */
/* "libs/monolib/include/monolib/math/CMat33.hpp" line 4 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CMat33{
        CMat33(){}

        CMat33(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22){
            set(m00, m01, m02, m10, m11, m12, m20, m21, m22);
        }

        void set(float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22){
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
        }

        void setRotXYZ(const CVec3& angle){
            float sinX = math::sin(angle.x);
            float cosX = math::cos(angle.x);
            float sinY = math::sin(angle.y);
            float cosY = math::cos(angle.y);
            float sinZ = math::sin(angle.z);
            float cosZ = math::cos(angle.z);

            set(
            cosY*cosZ, sinX*sinY*cosZ - cosX*sinZ, cosX*sinY*cosZ + sinX*sinZ,
            cosY*sinZ, sinX*sinY*sinZ + cosX*cosZ, cosX*sinY*sinZ - sinX*cosZ,
            -sinY,     sinX*cosY,                  cosX*cosY
            );
        }

        void getRotZXY(CVec3& outVec){
            float angle = math::asin(-m[1][2]);
            outVec.x = angle;

            if(angle < halfpi){
                if(angle > -halfpi){
                    //-pi/2 < angle < pi/2
                    outVec.z = math::atan2Rad(m[1][0], m[1][1]);
                    outVec.y = math::atan2Rad(m[0][2], m[2][2]);
                }else{
                    //angle < -pi/2
                    outVec.z = -math::atan2Rad(m[2][0], m[0][0]);
                    outVec.y = 0;
                }
            }else{
                //angle > pi/2
                outVec.z = math::atan2Rad(m[2][0], m[0][0]);
                outVec.y = 0;
            }
        }

        float m[3][3];
    };
} //namespace ml
/* end "monolib/math/CMat33.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 8 "monolib/math/CMat34.hpp" */
/* end "monolib/math/CMat34.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 9 "monolib/math/CMat44.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CMat44.hpp" line 2 "revolution/MTX.h" */
/* end "revolution/MTX.h" */

namespace ml {
    struct CMat44{
        CMat44(){}

        CMat44(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33){
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }

        union {
            float m[4][4];
            Mtx44 mtx;
        };
    
        static CMat44 zero;
        static CMat44 identity;
    };
} //namespace ml
/* end "monolib/math/CMat44.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 10 "monolib/math/CPlane.hpp" */
/* end "monolib/math/CPlane.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 11 "monolib/math/CPnt16.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CPnt16.hpp" line 2 "types.h" */
/* end "types.h" */

namespace ml {
    struct CPnt16{
        CPnt16(){}

        CPnt16(s16 x, s16 y){
            set(x,y);
        }

        void set(s16 x, s16 y){
            this->x = x;
            this->y = y;
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lhz/sth. */
        struct{
            s16 x;
            s16 y;
        };
    };
} //namespace ml
/* end "monolib/math/CPnt16.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 12 "monolib/math/CQuat.hpp" */
/* end "monolib/math/CQuat.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 13 "monolib/math/CRect16.hpp" */
#pragma once

/* "libs/monolib/include/monolib/math/CRect16.hpp" line 2 "types.h" */
/* end "types.h" */
/* "libs/monolib/include/monolib/math/CRect16.hpp" line 3 "monolib/math/CPnt16.hpp" */
/* end "monolib/math/CPnt16.hpp" */

namespace ml {
    struct CRect16 {
        CRect16(){
        }
    
        CRect16(s16 x, s16 y, s16 width, s16 height){
            set(x, y, width, height);
        }

        CRect16(const CPnt16& pos, const CPnt16& size){
            set(pos, size);
        }

        void set(s16 x, s16 y, s16 width, s16 height){
            mPos.x = x;
            mPos.y = y;
            mSize.x = width;
            mSize.y = height;
        }

        void set(const CPnt16& pos, const CPnt16& size){
            mPos.x = pos.x;
            mPos.y = pos.y;
            mSize.x = size.x;
            mSize.y = size.y;
        }
    
        bool isInside(const CPnt16& point) const;

        CPnt16 mPos; //0x0
        CPnt16 mSize; //0x4
    };
} //namespace ml
/* end "monolib/math/CRect16.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 14 "monolib/math/CVec2.hpp" */
#pragma once

/* Not known to be used yet, but it's here anyways just in case they did end up using it (it exists
in XCX, so it probably at least existed). It also doesn't hurt to have anyway for modding and stuff. */
namespace ml {
    struct CVec2 {

        CVec2(){}

        CVec2(float x, float y){
            set(x, y);
        }

        CVec2(const CVec2& vec){
            set(vec.x, vec.y);
        }

        void set(float x, float y){
            this->x = x;
            this->y = y;
        }

        void set(const CVec2& vec){
            x = vec.x;
            y = vec.y;
        }

        bool operator==(const CVec2& vec) const {
            return x == vec.x && y == vec.y;
        }

        bool operator!=(const CVec2& vec) const {
            return x != vec.x || y != vec.y;
        }
        
        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float x;
            float y;
        };

    };

} //namespace ml
/* end "monolib/math/CVec2.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 15 "monolib/math/CVec3.hpp" */
/* end "monolib/math/CVec3.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 16 "monolib/math/CVec4.hpp" */
#pragma once

namespace ml {
    struct CVec4{
        static CVec4 zero;
        static CVec4 unitX;
        static CVec4 unitY;
        static CVec4 unitZ;
        static CVec4 unit;

        CVec4() {}

        CVec4(float x, float y, float z, float w){
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        /* Nesting the variables in a nameless makes mwcc use lwz/stw for struct copies,
        which is more efficient than lfs/stfd. */
        struct{
            float x;
            float y;
            float z;
            float w;
        };

    };
} //namespace ml
/* end "monolib/math/CVec4.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 17 "monolib/math/Main.hpp" */
#pragma once

namespace ml{
    namespace math{
        void initialize();
    } //namespace math
} //namespace ml
/* end "monolib/math/Main.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 18 "monolib/math/FloatUtils.hpp" */
#pragma once

#define FLOAT_LARGE 1e14f
#define FLOAT_SMALL -1e14f
#define FLOAT_MAX_HEX 0xD3D3D3D3
#define FLOAT_MAX_HEX_NEG 0xF3F3F3F3
#define FLOAT_NAN 0xFFFFFFFF
#define FLOAT_TO_HEX(f) (*(u32*)&f)

namespace ml{
    namespace math{
        bool isErrFloat(float f);
    } //namespace math
} //namespace ml
/* end "monolib/math/FloatUtils.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 19 "monolib/math/MathConstants.hpp" */
/* end "monolib/math/MathConstants.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 20 "monolib/math/MTRand.hpp" */
//Original copyright comments:

// Mersenne Twister random number generator -- a C++ class MTRand
// Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus
// Richard J. Wagner  v1.1  28 September 2009  wagnerr@umich.edu

// The Mersenne Twister is an algorithm for generating random numbers.  It
// was designed with consideration of the flaws in various other generators.
// The period, 2^19937-1, and the order of equidistribution, 623 dimensions,
// are far greater.  The generator is also fast; it avoids multiplication and
// division, and it benefits from caches and pipelines.  For more information
// see the inventors' web page at
// http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html

// Reference
// M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
// Equidistributed Uniform Pseudo-Random Number Generator", ACM Transactions on
// Modeling and Computer Simulation, Vol. 8, No. 1, January 1998, pp 3-30.

// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
// Copyright (C) 2000 - 2009, Richard J. Wagner
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//   3. The names of its contributors may not be used to endorse or promote 
//      products derived from this software without specific prior written 
//      permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// The original code included the following notice:
// 
//     When you use this, send an email to: m-mat@math.sci.hiroshima-u.ac.jp
//     with an appropriate reference to your work.
// 
// It would be nice to CC: wagnerr@umich.edu and Cokus@math.washington.edu
// when you write.


/* Monolithsoft made slight modifications from the most commonly found version,
most notably changing function names.
This might be the version this is based on: https://gist.github.com/yuikns/10017640 */

#pragma once

/* "libs/monolib/include/monolib/math/MTRand.hpp" line 65 "types.h" */
/* end "types.h" */

namespace ml{

//Probably uses a singleton template

class MTRand{
protected:
    static const int N = 624; //length
    static const int M = 397; //period
    u32 state[N]; //twister
    int left; //0x9C0
    BOOL initialized; //0x9C4
    u32* pNext; //0x9C8
    u32 unk9D0;

public:
    static MTRand* getInstance();
    void srand(u32);
    void nextMt();
    u32 rand32();
    u32 rand31();
    float randFloat();
    float randFloat1();

    inline u32 rand(){
        return rand31();
    }

    inline u32 rand(int max){
        if(max != 0){
            int randVal = rand();
            return randVal % max;
        }else{
            return 0;
        }
    }

    inline u32 rand(int min, int max){
        int range = max - min;
        int result = 0;

        if(range != 0){
            int randVal = rand();
            result = (randVal % range);
        }else{
            result = 0;
        }

        return min + result; //wtf? why not just add it before?
    }

protected:
    inline u32 hiBit(u32 u) { return u & 0x80000000; }
    inline u32 loBit(u32 u) { return u & 0x00000001; }
    inline u32 loBits(u32 u) { return u & 0x7fffffff; }
    inline u32 mixBits(u32 u, u32 v) { return hiBit(u) | loBits(v); }
    inline u32 magic(u32 u) { return loBit(u) ? 0x9908b0dfUL : 0x0UL; }
    inline u32 twist(u32 m, u32 s0, u32 s1) {
        const u32 y = mixBits(s0, s1);
        const u32 mask = magic(s1);
        return m ^ (mask ^ (y >> 1));
    }
};

} //namespace ml
/* end "monolib/math/MTRand.hpp" */
/* "libs/monolib/include/monolib/math.hpp" line 21 "monolib/math/Random.hpp" */
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
/* end "monolib/math.hpp" */
/* "libs/monolib/include/monolib/device/CDeviceVI.hpp" line 7 "revolution/GX.h" */
/**
 * References: YAGCD, Dolphin Emulator, publicly available patents
 */

#ifndef RVL_SDK_PUBLIC_GX_H
#define RVL_SDK_PUBLIC_GX_H
#ifdef __cplusplus
extern "C" {
#endif

/* "libs/RVL_SDK/include/revolution/GX.h" line 10 "revolution/GX/GXAttr.h" */
/* end "revolution/GX/GXAttr.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 11 "revolution/GX/GXBump.h" */
/* end "revolution/GX/GXBump.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 12 "revolution/GX/GXDisplayList.h" */
/* end "revolution/GX/GXDisplayList.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 13 "revolution/GX/GXDraw.h" */
/* end "revolution/GX/GXDraw.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 14 "revolution/GX/GXFifo.h" */
/* end "revolution/GX/GXFifo.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 15 "revolution/GX/GXFrameBuf.h" */
/* end "revolution/GX/GXFrameBuf.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 16 "revolution/GX/GXGeometry.h" */
/* end "revolution/GX/GXGeometry.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 17 "revolution/GX/GXHardware.h" */
/**
 * For more details, see:
 * https://www.gc-forever.com/yagcd/chap8.html#sec8
 * https://www.gc-forever.com/yagcd/chap5.html#sec5
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/BPMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/XFMemory.h
 * https://github.com/dolphin-emu/dolphin/blob/master/Source/Core/VideoCommon/OpcodeDecoding.h
 * https://patents.google.com/patent/US6700586B1/en
 * https://patents.google.com/patent/US6639595B1/en
 * https://patents.google.com/patent/US7002591
 * https://patents.google.com/patent/US6697074
 */

#ifndef RVL_SDK_GX_HARDWARE_H
#define RVL_SDK_GX_HARDWARE_H
/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 15 "types.h" */
/* end "types.h" */

/* "libs/RVL_SDK/include/revolution/GX/GXHardware.h" line 17 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
 *
 *
 * GX FIFO
 *
 *
 ***********************************************************/

/**
 * FIFO write/gather pipe
 */
extern volatile union {
    // 1-byte
    char c;
    unsigned char uc;
    // 2-byte
    short s;
    unsigned short us;
    // 4-byte
    int i;
    unsigned int ui;
    void* p;
    float f;
} WGPIPE DECL_ADDRESS(0xCC008000);

/**
 * FIFO commands
 */
typedef enum {
    GX_FIFO_CMD_NOOP = 0x00,

    GX_FIFO_CMD_LOAD_BP_REG = 0x61,
    GX_FIFO_CMD_LOAD_CP_REG = 0x08,
    GX_FIFO_CMD_LOAD_XF_REG = 0x10,

    GX_FIFO_CMD_LOAD_INDX_A = 0x20,
    GX_FIFO_CMD_LOAD_INDX_B = 0x28,
    GX_FIFO_CMD_LOAD_INDX_C = 0x30,
    GX_FIFO_CMD_LOAD_INDX_D = 0x38,

    GX_FIFO_CMD_CALL_DL = 0x40,
    GX_FIFO_CMD_INVAL_VTX = 0x48,

    GX_FIFO_CMD_DRAW_POINTS = GX_POINTS,
    GX_FIFO_CMD_DRAW_LINES = GX_LINES,
    GX_FIFO_CMD_DRAW_LINESTRIP = GX_LINESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLES = GX_TRIANGLES,
    GX_FIFO_CMD_DRAW_TRIANGLESTRIP = GX_TRIANGLESTRIP,
    GX_FIFO_CMD_DRAW_TRIANGLEFAN = GX_TRIANGLEFAN,
    GX_FIFO_CMD_DRAW_QUADS = GX_QUADS,
} GXFifoCmd;

/**
 * FIFO command sizes
 */
#define GX_FIFO_CMD_LOAD_INDX_SIZE 5
#define GX_FIFO_CMD_DRAW_SIZE 3

#define __GX_FIFO_SET_LOAD_INDX_DST(reg, x) ((reg) = GX_BITSET(reg, 20, 12, x))
#define __GX_FIFO_SET_LOAD_INDX_NELEM(reg, x) ((reg) = GX_BITSET(reg, 16, 4, x))
#define __GX_FIFO_SET_LOAD_INDX_INDEX(reg, x) ((reg) = GX_BITSET(reg, 0, 16, x))

#define __GX_FIFO_LOAD_INDX(reg, dst, nelem, index)                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        __GX_FIFO_SET_LOAD_INDX_DST(cmd, dst);                                 \
        __GX_FIFO_SET_LOAD_INDX_NELEM(cmd, nelem);                             \
        __GX_FIFO_SET_LOAD_INDX_INDEX(cmd, index);                             \
        WGPIPE.c = reg;                                                        \
        WGPIPE.i = cmd;                                                        \
    }

#define GX_FIFO_LOAD_INDX_A(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_A, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_B(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_B, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_C(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_C, dst, nelem, index)

#define GX_FIFO_LOAD_INDX_D(dst, nelem, index)                                 \
    __GX_FIFO_LOAD_INDX(GX_FIFO_CMD_LOAD_INDX_D, dst, nelem, index)

/************************************************************
 *
 *
 * GX Blitting Processor (BP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into BP register
 */
#define GX_BP_LOAD_REG(data)                                                   \
    WGPIPE.c = GX_FIFO_CMD_LOAD_BP_REG;                                        \
    WGPIPE.i = (data);

/**
 * Set BP command opcode (first 8 bits)
 */
#define GX_BP_SET_OPCODE(cmd, opcode) (cmd) = GX_BITSET(cmd, 0, 8, (opcode))

#define GX_BP_OPCODE_SHIFT 24
#define GX_BP_CMD_SZ (sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 ***********************************************************/

/**
 * Load immediate value into CP register
 */
#define GX_CP_LOAD_REG(addr, data)                                             \
    WGPIPE.c = GX_FIFO_CMD_LOAD_CP_REG;                                        \
    WGPIPE.c = (addr);                                                         \
    WGPIPE.i = (data);

#define GX_CP_CMD_SZ (sizeof(u8) + sizeof(u8) + sizeof(u32))

/************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 ***********************************************************/

/**
 * XF memory
 */
typedef enum {
    GX_XF_MEM_POSMTX = 0x0000,
    GX_XF_MEM_NRMMTX = 0x0400,
    GX_XF_MEM_DUALTEXMTX = 0x0500,
    GX_XF_MEM_LIGHTOBJ = 0x0600
} GXXfMem;

/**
 * Header for an XF register load
 */
#define GX_XF_LOAD_REG_HDR(addr)                                               \
    WGPIPE.c = GX_FIFO_CMD_LOAD_XF_REG;                                        \
    WGPIPE.i = (addr);

/**
 * Load immediate value into XF register
 */
#define GX_XF_LOAD_REG(addr, data)                                             \
    GX_XF_LOAD_REG_HDR(addr);                                                  \
    WGPIPE.i = (data);

#define GX_XF_CMD_SZ (sizeof(u8) + sizeof(u32) + sizeof(u32))

/**
 * Load immediate values into multiple XF registers
 */
#define GX_XF_LOAD_REGS(size, addr)                                            \
    {                                                                          \
        u32 cmd = 0;                                                           \
        cmd |= (addr);                                                         \
        cmd |= (size) << 16;                                                   \
        GX_XF_LOAD_REG_HDR(cmd);                                               \
    }

/**
 * Enums for Tex0-Tex7 register fields
 */
typedef enum {
    GX_XF_TEX_PROJ_ST, // (s,t): texmul is 2x4
    GX_XF_TEX_PROJ_STQ // (s,t,q): texmul is 3x4
} GXXfTexProj;

typedef enum {
    GX_XF_TEX_FORM_AB11, // (A, B, 1.0, 1.0) (used for regular texture source)
    GX_XF_TEX_FORM_ABC1  // (A, B, C, 1.0) (used for geometry or normal source)
} GXXfTexForm;

typedef enum {
    GX_XF_TG_REGULAR, // Regular transformation (transform incoming data)
    GX_XF_TG_BUMP,    // Texgen bump mapping

    GX_XF_TG_CLR0, // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                   // color0

    GX_XF_TG_CLR1 // Color texgen: (s,t)=(r,g:b) (g and b are concatenated),
                  // color1
} GXXfTexGen;

/**
 * Misc. hardware enums
 */
typedef enum {
    GX_RAS_COLOR0A0,
    GX_RAS_COLOR1A1,
    GX_RAS_ALPHA_BUMP = 5,
    GX_RAS_ALPHA_BUMPN,
    GX_RAS_COLOR_ZERO,

    GX_RAS_MAX_CHANNEL
} GXRasChannelID;

typedef enum {
    GX_TEVREG_COLOR,
    GX_TEVREG_KONST,
} GXTevRegType;

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX/GXHardware.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 18 "revolution/GX/GXHardwareBP.h" */
/* end "revolution/GX/GXHardwareBP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 19 "revolution/GX/GXHardwareCP.h" */
/* end "revolution/GX/GXHardwareCP.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 20 "revolution/GX/GXHardwareXF.h" */
/* end "revolution/GX/GXHardwareXF.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 21 "revolution/GX/GXInit.h" */
/* end "revolution/GX/GXInit.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 22 "revolution/GX/GXInternal.h" */
/* end "revolution/GX/GXInternal.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 23 "revolution/GX/GXLight.h" */
/* end "revolution/GX/GXLight.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 24 "revolution/GX/GXMisc.h" */
/* end "revolution/GX/GXMisc.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 25 "revolution/GX/GXPixel.h" */
/* end "revolution/GX/GXPixel.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 26 "revolution/GX/GXTev.h" */
/* end "revolution/GX/GXTev.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 27 "revolution/GX/GXTexture.h" */
/* end "revolution/GX/GXTexture.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 28 "revolution/GX/GXTransform.h" */
/* end "revolution/GX/GXTransform.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 29 "revolution/GX/GXTypes.h" */
/* end "revolution/GX/GXTypes.h" */
/* "libs/RVL_SDK/include/revolution/GX.h" line 30 "revolution/GX/GXVert.h" */
/* end "revolution/GX/GXVert.h" */

#ifdef __cplusplus
}
#endif
#endif
/* end "revolution/GX.h" */
/* "libs/monolib/include/monolib/device/CDeviceVI.hpp" line 8 "revolution/VI.h" */
/* end "revolution/VI.h" */

//size: 0x2c0
class CDeviceVI : public CDeviceBase, public IErrorWii {
public:
    CDeviceVI(const char* pName, CWorkThread* pParent);
    virtual ~CDeviceVI();
    static CDeviceVI* getInstance();

    static void setFlag4(bool state);
    static bool checkFlag4();
    static bool checkFlag2();
    static bool func_804482DC();
    static void setFlag0(bool state);
    static bool checkFlag0();

    static void func_804483DC(u32 gamma);
    static GXRenderModeObj* getRenderModeObj();
    static u32 getTargetFramerate();
    static float getSecPerFrame();
    static u32 func_80448420();
    static u32 getVisPerFrame();
    static bool entryCb(CDeviceVICb* entry);
    static bool removeCb(CDeviceVICb* entry);
    static bool isWideAspectRatio();
    static bool isTvFormatPal();
    static u32 getXfbBuffersSize();
    static float getWidthScale();
    static bool initBaseRenderModeStruct(u32 r3, u32 r4);
    bool updateMainRenderModeStruct();
    virtual void wkUpdate();
    static void beginFrame();
    static void waitForDrawDone();
    static void endFrame();
    static bool onPreRetrace();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    static void setUseStaticHandle(bool state);
    static bool usingStaticHandle();
    void func_80448E88();
    virtual void errorWiiCB();

    static bool unkInline1();
    static void copyEfb(u32 index);
    void setNextFrameBuffer();
    static void cb(CDeviceVICb::VICallback callback);
    static void setupRenderMode2(GXRenderModeObj* pRenderMode, u32 viWidth);
    void unkInline3(u32 index, u32 val);

    static inline CDeviceVI* create(const char* pName, CWorkThread* pParent){
        CDeviceVI* device = new (CWorkThreadSystem::getWorkMem()) CDeviceVI(pName, pParent);
        CWorkUtil::entryWork(device, pParent, false);
        device->mFlags |= FLAG_CREATED;
        return device;
    }

    static inline u16 getEfbHeight(){
        return getRenderModeObj()->efbHeight;
    }

    static inline u16 getFbWidth(){
        return getRenderModeObj()->fbWidth;
    }

    //0x0: vtable
    //0x0-1C8: CDeviceBase
    //0x1C8-1CC: UnkClass_80447FDC
    u32 mViFlags; //0x1CC
    reslist<CDeviceVICb*> mCallbackList; //0x1D0
    u32 mTvFormat; //0x1F0
    u32 mGammaLevel; //0x1F4
    u32 mScanMode; //0x1F8
    u32 mDimmingCount; //0x1FC
    GXRenderModeObj mMainRenderMode; //0x200
    GXRenderModeObj mBaseRenderMode; //0x23C
    //These four are unused?
    u16 mViXOrigin; //0x278
    u16 mViYOrigin; //0x27A
    u16 mViWidth; //0x27C
    u16 mViHeight; //0x27E
    u8* mXfbBuffersPtr; //0x280
    u32 unk284;
    void* mFrameBufferPtrArray[3]; //0x288
    u32 unk294;
    u32 unk298;
    u32 unk29C;
    ml::CPnt16 unk2A0;
    u32 unk2A4;
    u32 mVisPerFrame; //0x2A8
    u32 unk2AC;
    u32 mNewVisPerFrame;
    u8 unk2B4; //padding?
    u8 unk2B5;
    u8 unk2B6[2]; //padding?
    u32 mTargetFramerate; //0x2B8
    float mSecPerFrame; //0x2BC

    //General screen dimensions
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 456;

    static const int NTSC_VPS = 60;
    static const int PAL_VPS = 50;
    //2 VIs (vertical interrupts) per frame -> 30fps
    static const int VI_PER_FRAME = 2;

    //VI max output dimensions
    static const int VI_MAX_WIDTH = 720;
    static const int VI_MAX_HEIGHT = 480;
    //VI width
    static const int VI_WIDTH_4_3 = 670;
    static const int VI_WIDTH_16_9 = 686;

    static const int TARGET_FRAMERATE = NTSC_VPS/VI_PER_FRAME;

private:
    enum Flags{
        VI_FLAG_0 = 0,
        VI_FLAG_1 = 1,
        VI_FLAG_2 = 2,
        VI_FLAG_3 = 3,
        VI_FLAG_4 = 4,
        VI_FLAG_31 = 31
    };

    //Custom tv format/scan mode enums for the render mode table
    enum TVFormat {
        TV_FORMAT_NTSC,
        TV_FORMAT_PAL,
        TV_FORMAT_EURGB60,
        TV_FORMAT_MPAL,
        MAX_TV_FORMAT = 4
    };

    enum ScanMode {
        SCAN_MODE_DS,
        SCAN_MODE_INT,
        SCAN_MODE_PROG,
        SCAN_MODE_PROGSOFT,
        MAX_SCAN_MODE = 4
    };

    u32 getTvFormatIndex() const;
    u32 getScanModeIndex() const;
    u32 calculateRenderModeIndex() const;
    
    void setFlag(u32 flag, bool state){
        if(state != false) mViFlags |= (1 << flag);
        else mViFlags &= ~(1 << flag);
    }

    bool checkFlag(u32 flag) const {
        return mViFlags & (1 << flag);
    }

    //XFB dimensions
    //JP: 640x480, US: 640x456, PAL: 640x542
    static const int XFB_WIDTH = 640;
    #if defined(VERSION_JP)
    static const int XFB_HEIGHT = 480;
    #elif defined(VERSION_EU)
    static const int XFB_HEIGHT = 542;
    #elif defined(VERSION_US)
    static const int XFB_HEIGHT = 456;
    #endif

    static const int NUM_XFB_BUFFERS = 2; //double buffered

    static const int MAX_CHILD = 8;

    static CDeviceVI* spInstance;
    static const VIGamma gammaLevels[];
    static GXRenderModeObj* renderModes[];
    static ml::CPnt16 lbl_8065A6B8[];
    static bool sUseStaticHandle;
};

/* Utility defines since some code just directly uses hardcoded constants for
framerate stuff */
//TODO: idk where else to put these but ideally smth better than this maybe
static const double MS_PER_FRAME = 1.0/CDeviceVI::TARGET_FRAMERATE;

#define SECONDS_TO_FRAMES(n) (CDeviceVI::TARGET_FRAMERATE * n)
/* end "monolib/device/CDeviceVI.hpp" */
/* "src/kyoshin/CUICfManager.cpp" line 526 "kyoshin/cf/CfGameManager.hpp" */
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

extern "C" {
u32 lbl_eu_80663E24;
u32 lbl_eu_80663E28;
f32 lbl_eu_806672CC;

void func_801338C8(CUICfManager*);
void func_80133770();
void* __ct__CMenuKeyAssign(void*, u32);
void* func_801109D8(void*, u32, void*); // create menu; r5=0 or enum object*
void* func_8011E4C4(void*, u32);
void* __ct__CMenuBattleMode(void*, u32);
void* __ct__CMenuLvUp(void*, u32);

// Opaque 8-byte holder around a CfObjEnumList* (func_80043D90 / __dt__80043E88).
struct CUICfEnumListHolder {
    void* list; // 0x0
    u32 handle; // 0x4
};

void func_80043D90(CUICfEnumListHolder*);
void* func_80043F18(CUICfEnumListHolder*); // returns holder->list
void __dt__80043E88(CUICfEnumListHolder*, s16);
void func_800F4A98(void* list, int type, int);
void* __ct__800FB044(void* list, f32, void* obj, int);
void* func_80496264(void* obj, int index);
void* func_800F6EC0(void* list, int index); // &slot → has +0x4 object ptr
void* func_800F6E98(void* list, int index); // *slot → object*
int func_800B8920(void*);
int func_8013A4B4(void* a, void* b, void* c);
void func_8012FFB4(void*); // &mInitSlots[0].unk04
}

typedef void* (*CUICfVPtrFn)(void*);

void CUICfManager::Move() {
    // Retail: -0x120 / manual stw r31..r28 / mr r31,r1. Stack home 0x18 for
    // created; setItem try/catch forces the EH frame. Spill created so the
    // find loop can reuse r3 for unk138 (retail), instead of keeping created
    // live and deepening saves to _savegpr_27.
    // Retail: -0x120 / manual stw r31..r28 / mr r31,r1. Spill created through
    // *createdHome so the find loop can reuse r3 for unk138 (retail), instead
    // of keeping created live. Soft-cap: collect still strength-reduces
    // pending[] into a walk cursor → node in r27 → _savegpr_27 (retail uses
    // indexed stwx with node in r28); decomp size 0x960 vs retail 0x97C.
    void* savedRet18;
    volatile void** createdHome;
    int framePad[4];
    CUICfEnumListHolder holder;
    f32 posC[3];
    f32 posB[3];
    f32 posA[3];
    _reslist_node<CUICfMenuItem*>* pending[18];
    u16 flags;
    CUICfManager* inst;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<void*>* startNode;
    _reslist_node<void*>* temp;
    void* party;
    void* list;
    void* handle;
    void* qpos;
    void* pose;
    void* slot;
    void* createdArg;
    _reslist_node<CUICfMenuItem*>* head;
    _reslist_node<CUICfMenuItem*>* it;
    _reslist_node<CUICfMenuItem*>* walk;
    _reslist_node<CUICfMenuItem*>* node;
    _reslist_node<CUICfMenuItem*>* prev;
    _reslist_node<CUICfMenuItem*>* next;
    CUICfMenuItem* item;
    int pendingCount;
    int needWait;
    int limit;
    int canUnroll;
    int nextCount;

    framePad[0] = 0;
    createdHome = (volatile void**)&savedRet18;
    flags = mFlags;

    if ((flags & 0x2) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xfffd);
        }
        func_801338C8(this);
        goto after_flags;
    }
    if ((flags & 0x1) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & ~0x1); // rlwinm …,16,30
        }
        func_80133770();
        goto after_flags;
    }
    if ((flags & 0x4) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xfffb);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x4);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xfffb);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x8);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x10);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x20);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x40);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x80);
        goto after_flags;
    }
    if ((flags & 0x8) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xfff7);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x8);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xfff7);
        inst = (CUICfManager*)lbl_eu_80664054;
        *createdHome = __ct__CMenuKeyAssign(inst->unk144, inst->unk11C);
        if (*createdHome != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push8_check;
        push8_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push8_found;
            }
            byteOff += 0xc;
            i++;
        push8_check:
            if (i < capacity) {
                goto push8_body;
            }
        push8_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*createdHome);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x10) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xffef);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x10);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xffef);
        inst = (CUICfManager*)lbl_eu_80664054;
        *createdHome = func_801109D8(inst->unk144, inst->unk11C, NULL);
        if (*createdHome != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push10_check;
        push10_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push10_found;
            }
            byteOff += 0xc;
            i++;
        push10_check:
            if (i < capacity) {
                goto push10_body;
            }
        push10_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*createdHome);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x20) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xffdf);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x20);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xffdf);
        inst = (CUICfManager*)lbl_eu_80664054;
        *createdHome = func_8011E4C4(inst->unk144, inst->unk11C);
        if (*createdHome != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push20_check;
        push20_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push20_found;
            }
            byteOff += 0xc;
            i++;
        push20_check:
            if (i < capacity) {
                goto push20_body;
            }
        push20_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*createdHome);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x40) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xffbf);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x40);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xffbf);
        inst = (CUICfManager*)lbl_eu_80664054;
        *createdHome = __ct__CMenuBattleMode(inst->unk144, inst->unk11C);
        if (*createdHome != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push40_check;
        push40_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push40_found;
            }
            byteOff += 0xc;
            i++;
        push40_check:
            if (i < capacity) {
                goto push40_body;
            }
        push40_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*createdHome);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x80) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xff7f);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x80);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xff7f);
        inst = (CUICfManager*)lbl_eu_80664054;
        *createdHome = __ct__CMenuLvUp(inst->unk144, inst->unk11C);
        if (*createdHome != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push80_check;
        push80_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push80_found;
            }
            byteOff += 0xc;
            i++;
        push80_check:
            if (i < capacity) {
                goto push80_body;
            }
        push80_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*createdHome);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
    }

after_flags:
    framePad[1] = framePad[0];
    // Gate: bits 6|21 then bit 13 of lbl_eu_80663E24 (0x02040400)
    if ((lbl_eu_80663E24 & 0x02040400u) != 0) {
        goto after_enum;
    }
    party = cf::CfGameManager::func_80082D54(0);
    if (party == NULL) {
        goto after_enum;
    }

    func_80043D90(&holder);
    list = func_80043F18(&holder);
    func_800F4A98(list, 0x130, 0);
    party = cf::CfGameManager::func_80082D54(0);
    {
        void** vt = *reinterpret_cast<void***>(party);
        handle = reinterpret_cast<CUICfVPtrFn>(vt[0xAC / 4])(party);
    }
    list = func_80043F18(&holder);
    __ct__800FB044(list, lbl_eu_806672CC, handle, 0);

    pose = func_80496264((void*)unk11C, -1);
    posA[0] = *reinterpret_cast<f32*>((u8*)pose + 0x10c);
    posA[1] = *reinterpret_cast<f32*>((u8*)pose + 0x110);
    posA[2] = *reinterpret_cast<f32*>((u8*)pose + 0x114);
    posB[0] = *reinterpret_cast<f32*>((u8*)pose + 0x138);
    posB[1] = *reinterpret_cast<f32*>((u8*)pose + 0x13c);
    posB[2] = *reinterpret_cast<f32*>((u8*)pose + 0x140);

    i = 0;
    goto enum_check;
enum_body:
    list = func_80043F18(&holder);
    slot = func_800F6EC0(list, i);
    handle = *(void**)((u8*)slot + 4);
    if (handle == NULL) {
        goto enum_next;
    }
    {
        void** vt = *reinterpret_cast<void***>(handle);
        if (reinterpret_cast<int (*)(void*)>(vt[0x160 / 4])(handle) == 0) {
            goto enum_next;
        }
    }
    if (func_800B8920(handle) == 0) {
        goto enum_next;
    }
    list = func_80043F18(&holder);
    slot = func_800F6EC0(list, i);
    handle = *(void**)((u8*)slot + 4);
    {
        void** vt = *reinterpret_cast<void***>(handle);
        qpos = reinterpret_cast<CUICfVPtrFn>(vt[0xAC / 4])(handle);
    }
    posC[0] = *reinterpret_cast<f32*>((u8*)qpos + 0);
    posC[1] = *reinterpret_cast<f32*>((u8*)qpos + 4);
    posC[2] = *reinterpret_cast<f32*>((u8*)qpos + 8);
    if (func_8013A4B4(posA, posB, posC) == 0) {
        goto enum_next;
    }
    list = func_80043F18(&holder);
    createdArg = func_800F6E98(list, i);
    func_801109D8(unk144, unk11C, createdArg);
enum_next:
    i++;
enum_check:
    list = func_80043F18(&holder);
    if ((u32)i < *(u32*)((u8*)list + 0x620)) {
        goto enum_body;
    }
    __dt__80043E88(&holder, -1);

after_enum:
    // Scan with walk cursor stuck at head->next; on hit, mark from start to end.
    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    walk = head->mNext;
    it = walk;
    goto mark_check;
mark_set_body:
    walk->mItem->unk55 = 1;
    walk = walk->mNext;
mark_set_check:
    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    if (walk != head) {
        goto mark_set_body;
    }
    goto mark_done;
mark_body:
    if (it->mItem->unk55 != 0) {
        goto mark_set_check;
    }
    if (mInitSlots[0].unk00[1] != 0) {
        goto mark_set_check;
    }
    it = it->mNext;
mark_check:
    if (it != head) {
        goto mark_body;
    }
mark_done:

    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    node = head->mNext;
    pendingCount = 0;
    needWait = 1;
    goto collect_check;
collect_body:
    item = node->mItem;
    if (item->unk54 != 0) {
        goto collect_take;
    }
    if (mInitSlots[0].unk00[0] == 0) {
        goto collect_next;
    }
collect_take:
    if (needWait != 0) {
        CDeviceVI::waitForDrawDone();
        needWait = 0;
    }
    item = node->mItem;
    item->unk39 = 1;
    pending[pendingCount] = node;
    pendingCount++;
collect_next:
    node = node->mNext;
collect_check:
    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    if (node != head) {
        goto collect_body;
    }

    i = 0;
    if (pendingCount <= 0) {
        goto unlink_done;
    }
    if (pendingCount <= 8) {
        goto unlink_tail;
    }
    limit = pendingCount - 8;
    canUnroll = 0;
    if (pendingCount < 0) {
        goto unlink_unroll_test;
    }
    if (pendingCount > (int)0x7FFFFFFE) {
        goto unlink_unroll_test;
    }
    canUnroll = 1;
unlink_unroll_test:
    if (canUnroll == 0) {
        goto unlink_tail;
    }
    goto unlink_unroll_check;
unlink_unroll_body:
    for (byteOff = 0; byteOff < 8; byteOff++) {
        temp = (_reslist_node<void*>*)pending[i + byteOff];
        prev = (_reslist_node<CUICfMenuItem*>*)((_reslist_node<CUICfMenuItem*>*)temp)->mPrev;
        next = (_reslist_node<CUICfMenuItem*>*)((_reslist_node<CUICfMenuItem*>*)temp)->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        ((_reslist_node<CUICfMenuItem*>*)temp)->mNext = NULL;
    }
    i += 8;
unlink_unroll_check:
    if (i < limit) {
        goto unlink_unroll_body;
    }
unlink_tail:
    goto unlink_tail_check;
unlink_tail_body:
    node = pending[i];
    prev = node->mPrev;
    next = node->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    node->mNext = NULL;
    i++;
unlink_tail_check:
    if (i < pendingCount) {
        goto unlink_tail_body;
    }
unlink_done:

    mInitSlots[0].unk00[0] = 0;
    mInitSlots[0].unk00[1] = 0;
    func_8012FFB4(&mInitSlots[0].unk04);

    nextCount = (int)unk120 - 1;
    unk120 = (u32)nextCount;
    if (nextCount < 0) {
        unk120 = 0;
    }
    framePad[2] = framePad[1];
}
