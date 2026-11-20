#pragma once

// Canonical declarations for the C++ helpers defined in code_8025FB10.cpp.
// The object layouts belong to that TU; callers only need opaque pointers so
// the mangled linker names are emitted from these real C++ signatures.

#include <types.h>

struct CUnk8025FB10;
struct CUnkObj8025FB10;

s32 IdTable_SumValues(CUnkObj8025FB10* obj, int index);
s32 IdTable_MaxValueC(CUnkObj8025FB10* obj, int index);
f32 IdTable_MaxValue10(CUnkObj8025FB10* obj, int index);
void Counter_AddClamped(CUnk8025FB10* obj, int index, int value);
// Retail symbol is the plain C name Counter_TestBit (config/us/symbols.txt).
extern "C" int Counter_TestBit(void* obj, u32 index);
void Counter884_AddClamped(CUnk8025FB10* obj, int delta);
void Counter884_SetClamped(u8* obj, int value);
void TextState_Init(u8* arg);
void TextState_Reset(int arg);
