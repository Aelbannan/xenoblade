#include "monolib/core/CException.hpp"

// Split listed in configure.py; retail .text work lands here later.
// IGameException dtor stays inline in the header so CfPadTask::~CfPadTask
// stays 0xA0 (out-of-line empty base adds a bl / +12).

extern "C" {
    // 0x806656C0: global pointer; func_8045925C stores `this` at offset 0x1F0
    void* lbl_eu_806656C0;
    // 0x806656C4: global counter used by func_804591BC
    u32 lbl_eu_806656C4;
    // 0x80657B50: global array of 16 pointers used by func_804591BC
    void* lbl_eu_80657B50[16];
}

void __ct__CException() {}

void func_80458B64(u8* buffer, u8 r4, u8 r5, u8 r6, u8 r7) {
    // Writes 4 bytes (RGBA color) into the given buffer
    buffer[0] = r4;
    buffer[1] = r5;
    buffer[2] = r6;
    buffer[3] = r7;
}

void CException::func_8045925C() {
    // Store `this` at offset 0x1F0 of the global struct (if non-null)
    if (lbl_eu_806656C0 != nullptr) {
        *(CException**)((u8*)lbl_eu_806656C0 + 0x1F0) = this;
    }
}

void CException::func_804591BC(IException* pException) {
    // Register this CException into the global exception array
    u32 idx = lbl_eu_806656C4;
    *(CException**)((u8*)lbl_eu_80657B50 + idx * 4) = this;
    lbl_eu_806656C4 = idx + 1;
}

bool CException::func_80457C8C() {
    // Compare field_0x1F8 against 0.8f; return EQ bit of CR0
    return field_0x1F8 == 0.8f;
}

CException* CException::func_80457CA4(CWorkThread* r3, const wchar_t* message, u32 r5) { return nullptr; }
