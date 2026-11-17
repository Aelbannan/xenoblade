#ifndef MSL_STRING_H
#define MSL_STRING_H

#include <types.h>

// MSL declares every C library function as unable to throw, so a call to one
// opens no exception range in a C++ caller.
#ifndef _MSL_CANT_THROW
#ifdef __cplusplus
#define _MSL_CANT_THROW __attribute__((nothrow))
#else
#define _MSL_CANT_THROW
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/string_api.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/extras.h"

char* strcpy(char*, const char*) _MSL_CANT_THROW;
char* strncpy(char*, const char*, size_t) _MSL_CANT_THROW;

char* strcat(char*, const char*) _MSL_CANT_THROW;
char* strncat(char*, const char*, size_t) _MSL_CANT_THROW;

int strcmp(const char*, const char*) _MSL_CANT_THROW;
int strncmp(const char*, const char*, size_t) _MSL_CANT_THROW;

char* strchr(const char*, int) _MSL_CANT_THROW;
char* strstr(const char*, const char*) _MSL_CANT_THROW;

size_t strlen(const char*) _MSL_CANT_THROW;

void* memmove(void*, const void*, size_t) _MSL_CANT_THROW;
int memcmp(const void*, const void*, size_t) _MSL_CANT_THROW;
void* memchr(const void*, int, size_t) _MSL_CANT_THROW;

void* memcpy(void* dest, const void* src, size_t n) _MSL_CANT_THROW;
void* memset(void* dest, int val, size_t count) _MSL_CANT_THROW;

#ifdef __cplusplus
}
#endif
#endif
