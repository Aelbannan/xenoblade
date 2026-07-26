#include "monolib/core/CException.hpp"

// Split listed in configure.py; retail .text work lands here later.
// IGameException dtor stays inline in the header so CfPadTask::~CfPadTask
// stays 0xA0 (out-of-line empty base adds a bl / +12).

extern "C" void __ct__CException() {}
extern "C" void func_80457C8C__10CExceptionFv() {}
extern "C" void func_80457CA4__10CExceptionFP11CWorkThreadPCwUl() {}
