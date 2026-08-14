#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <new>
#else
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#endif

#include <compat.h>
#include <decomp.h>
#include <lang.h>
#include <macros.h>
#include <build_settings.h>

typedef unsigned long long u64;
typedef signed long long s64;

#if defined(__MWERKS__) && !defined(NONMATCHING)
// MWCC matching build: keep the `long` spellings so C++ name mangling stays
// byte-identical (`unsigned long` mangles as `Ul`; `unsigned int` as `Ui`).
typedef unsigned long u32;
typedef signed long s32;
#else
// PC port / NONMATCHING builds: exact-width. `long` is 64-bit on LP64
// (macOS/Linux x86-64, aarch64), which would make every u32/s32 data label
// and every data_defs.cpp definition 64-bit -- diverging from retail layout
// (array strides, struct offsets, element counts). uint32_t/int32_t are
// 32-bit on every supported target (types.h already includes <stdint.h>).
typedef uint32_t u32;
typedef int32_t s32;
#endif

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
