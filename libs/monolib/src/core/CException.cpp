#include "monolib/core/CException.hpp"

// Split listed in configure.py; retail .text work lands here later.
// IGameException dtor stays inline in the header so CfPadTask::~CfPadTask
// stays 0xA0 (out-of-line empty base adds a bl / +12).

// LLM-HARNESS-BEGIN: us-8045bc8c
extern "C" void __ct__CException() {}
// LLM-HARNESS-END: us-8045bc8c
// LLM-HARNESS-BEGIN: us-8045bd7c
extern "C" void func_80457C8C__10CExceptionFv() {}
// LLM-HARNESS-END: us-8045bd7c
// LLM-HARNESS-BEGIN: us-8045bd94
extern "C" void func_80457CA4__10CExceptionFP11CWorkThreadPCwUl() {}
// LLM-HARNESS-END: us-8045bd94
