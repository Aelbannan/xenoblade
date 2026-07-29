#pragma once

#include "monolib/math/CMat34.hpp"
#include <types.h>

// Forward declarations for functions in other TUs
extern "C" s16 func_804DFA84(void* param0, void* param1, void* param2);
extern "C" void func_804DFB88(s16 handle);
extern "C" void* lbl_eu_80570068;
extern "C" float lbl_eu_8066B2E0;
extern "C" float lbl_eu_8066B2E4;

void __ct__CSchedule(void* thisPtr);
void __dt__9CScheduleFv(void* thisPtr, int del);
int func_804E3434(void* thisPtr, void* p4, void* p5, void* p6, u32 r7, u32 r8, void* p9);
int func_804E3614(void* thisPtr);
