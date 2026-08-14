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

// Dual-mode data-label entry used by the include/lbls_<area>.hpp headers:
// the MWCC matching build declares the label (`extern`; the retail image
// supplies the data), non-MWCC builds define it. The generated data TU
// (port/data_defs.cpp) defines LBLS_DEFINE_DATA BEFORE including types.h so
// the macro is baked in define mode; every other TU sees extern. Note the
// mode is chosen at macro-DEFINITION time, hence the ordering requirement.
#if defined(__MWERKS__) && !defined(NONMATCHING)
#  define LBLS_ENTRY(ext, dfn, init) extern ext;
#elif defined(LBLS_DEFINE_DATA)
#  define LBLS_ENTRY(ext, dfn, init) dfn = LBLS_UNWRAP init;
#else
#  define LBLS_ENTRY(ext, dfn, init) extern ext;
#endif
#ifndef LBLS_UNWRAP
#  define LBLS_UNWRAP(...) __VA_ARGS__
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
