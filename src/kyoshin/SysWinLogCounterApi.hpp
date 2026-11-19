#pragma once

#include <types.h>

// Retail-unmangled scenario-counter entry points defined in code_8028130C.
// Callers include this header instead of repeating a TU-local declaration.
extern "C" void SysWinLog_CheckOpenGate();
extern "C" void SysWinLog_TickCounter0B(void* self);
extern "C" void SysWinLog_TickCounter2C(s32 self);
extern "C" void SysWinLog_TickCounter23();
extern "C" void SysWinLog_TickCounter29();
extern "C" void SysWinLog_TickCounter5A();
