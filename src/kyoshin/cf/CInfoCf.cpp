// Auto-scaffolded catalog TU for kyoshin/cf/CInfoCf
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CInfoCf.hpp"

// Global: UI state flag read by func_80166830.
extern u32 lbl_eu_80663E24;

void __ct__cf_CInfoCf(){}

cf::CInfoCf::~CInfoCf() {}

void func_8016676C(){}

void func_80166778(){}

// FULL_MATCH — no-op virtual function (vtable slot 2).  Immediately returns.
void func_80166784() {
}

void func_80166788(){}

void func_801667AC(){}

// FULL_MATCH — Returns the global UI state flag value (lbl_eu_80663E24).
u32 func_80166830() {
    return lbl_eu_80663E24;
}

CMenuItem::~CMenuItem() {}

void CMenuItem::Init() {}

extern "C" void func_80166E48(void* dst, const void* src) {
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;
    *(u32*)(d + 4) = *(u32*)(s + 4);
    *(u32*)(d + 8) = *(u32*)(s + 8);
    *(u32*)(d + 0xc) = *(u32*)(s + 0xc);
    *(u32*)(d + 0x10) = *(u32*)(s + 0x10);
    *(u32*)(d + 0x14) = *(u32*)(s + 0x14);
    *(u32*)(d + 0x18) = *(u32*)(s + 0x18);
    *(u32*)(d + 0x1c) = *(u32*)(s + 0x1c);
    *(u32*)(d + 0x20) = *(u32*)(s + 0x20);
    *(u32*)(d + 0x24) = *(u32*)(s + 0x24);
    d[0x28] = s[0x28]; d[0x29] = s[0x29]; d[0x2a] = s[0x2a]; d[0x2b] = s[0x2b];
    *(u32*)(d + 0x30) = *(u32*)(s + 0x30);
    *(u32*)(d + 0x34) = *(u32*)(s + 0x34);
    *(u32*)(d + 0x38) = *(u32*)(s + 0x38);
    *(u32*)(d + 0x3c) = *(u32*)(s + 0x3c);
    *(u32*)(d + 0x40) = *(u32*)(s + 0x40);
    *(u32*)(d + 0x44) = *(u32*)(s + 0x44);
    *(u32*)(d + 0x48) = *(u32*)(s + 0x48);
    *(u32*)(d + 0x4c) = *(u32*)(s + 0x4c);
    d[0x50] = s[0x50]; d[0x51] = s[0x51]; d[0x52] = s[0x52]; d[0x53] = s[0x53];
    *(f32*)(d + 0x54) = *(f32*)(s + 0x54);
    *(f32*)(d + 0x58) = *(f32*)(s + 0x58);
    *(f32*)(d + 0x5c) = *(f32*)(s + 0x5c);
    *(f32*)(d + 0x60) = *(f32*)(s + 0x60);
    *(f32*)(d + 0x64) = *(f32*)(s + 0x64);
    d[0x68] = s[0x68];
    for (int i = 0; i < 0x10; i++) {
        *(u64*)(d + 0x6c + i * 8) = *(u64*)(s + 0x6c + i * 8);
    }
    d[0xec] = s[0xec]; d[0xed] = s[0xed]; d[0xee] = s[0xee];
}

void func_80166F80(){}

void func_801671D4(){}

void func_80167260(){}

void func_801672E4(){}

void func_80167368(){}

void func_8016742C(){}

void func_801674D0(){}

void CMenuItem::Term() {}

void CMenuItem::Move() {}

void CMenuItem::cbRenderBefore() {}
