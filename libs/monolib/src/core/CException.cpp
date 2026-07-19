#include "monolib/core/CException.hpp"

// Split listed in configure.py; retail .text work lands here later.
// IGameException dtor stays inline in the header so CfPadTask::~CfPadTask
// stays 0xA0 (out-of-line empty base adds a bl / +12).

// LLM-HARNESS-BEGIN: us-8045bc8c
extern "C" void ct_CException() {}
// LLM-HARNESS-END: us-8045bc8c
// LLM-HARNESS-BEGIN: us-8045bd7c
extern "C" bool func_80457C8C() { return false; }
// LLM-HARNESS-END: us-8045bd7c
// LLM-HARNESS-BEGIN: us-8045bd94
extern "C" void* func_80457CA4() { return 0; }
// LLM-HARNESS-END: us-8045bd94
