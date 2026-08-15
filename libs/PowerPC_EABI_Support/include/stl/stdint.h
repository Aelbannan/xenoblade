#ifndef MSL_STDINT_H
#define MSL_STDINT_H

#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif //ifdef __cplusplus

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#if defined(__LP64__)
// LP64 hosts (clang/PC port): `long` is 8 bytes; MSL's `unsigned long`
// spelling would make uint32_t 64-bit and break every struct/array layout
// written against the Wii's 4-byte long. uint32_t stays exact-width here;
// MWCC (Wii, no __LP64__) keeps the `unsigned long` spelling so its C++
// name mangling stays byte-identical.
typedef unsigned int uint32_t;
#else
typedef unsigned long uint32_t;
#endif

typedef signed char int8_t;
typedef short int16_t;
#if defined(__LP64__)
typedef int int32_t;
#else
typedef long int32_t;
#endif

typedef unsigned long long uint64_t;
typedef long long int64_t;

#if defined(__LP64__)
// LP64 hosts: uintptr_t must be pointer-sized (8 bytes), not uint32_t.
typedef unsigned long uintptr_t;
typedef long intptr_t;
#else
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;
#endif

#ifdef __cplusplus
};
#endif //ifdef __cplusplus

#endif
