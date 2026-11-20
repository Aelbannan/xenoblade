#pragma once

#include <types.h>

// Scenario-log helpers defined in kyoshin/code_8028130C.cpp (C++ linkage;
// retail emits their mangled names). The argument record types are declared
// with their owning header, CSysWinScenarioLog.hpp; only pointers cross this
// boundary here.
struct CScenarioLogOwner;
struct CScenarioLogArg2;
struct CSysWinDevice;

void SysWinLog_PollBattleEnd();
void SysWinLog_ResetState();
void SysWinLog_PollBattleStart();
void SysWinLog_PollSequenceProgress(CScenarioLogOwner* owner, s32 value,
                                    CScenarioLogArg2* arg);
void SysWinLog_DrivePair(CScenarioLogOwner* first, CScenarioLogOwner* second);
void SysWinLog_LatchOneShot(CSysWinDevice* device);
void SysWinLog_GateDevice(CSysWinDevice* device);
void SysWinLog_GateOwner(CScenarioLogOwner* owner);
void SysWinLog_AdvanceTiers(s32 value);
void SysWinLog_ScanGrid(u8* self);

// Retail-unmangled scenario-counter entry points defined in code_8028130C.
// Callers include this header instead of repeating a TU-local declaration.
// The mangled scenario helpers (PollBattleStart etc.) are declared with their
// real C++ linkage in code_8028130C.hpp.
extern "C" void SysWinLog_CheckOpenGate();
extern "C" void SysWinLog_TickCounter0B(void* self);
extern "C" void SysWinLog_TickCounter2C(s32 self);
extern "C" void SysWinLog_TickCounter23();
extern "C" void SysWinLog_TickCounter29();
extern "C" void SysWinLog_TickCounter5A();
