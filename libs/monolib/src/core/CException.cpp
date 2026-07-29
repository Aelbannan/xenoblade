#include "monolib/core/CException.hpp"

// Split listed in configure.py; retail .text work lands here later.
// IGameException dtor stays inline in the header so CfPadTask::~CfPadTask
// stays 0xA0 (out-of-line empty base adds a bl / +12).

void __ct__CException(){}
bool CException::func_80457C8C() {
    // Compare field_0x1F8 against 0.8f; return EQ bit of CR0
    return field_0x1F8 == 0.8f;
}
CException* CException::func_80457CA4(CWorkThread* r3, const wchar_t* message, u32 r5) { return nullptr; }
