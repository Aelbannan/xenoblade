#pragma once

// The C++ declaration of Counter_TestBit for migrated callers.  CfGameManager.cpp
// keeps its one-argument C copy: the retail wrapper tail-calls with only r3 set,
// so supplying the mangled signature's second argument would add an instruction.
#include <types.h>

struct CUnk8025FB10;

int Counter_TestBit(CUnk8025FB10* obj, u32 index);
