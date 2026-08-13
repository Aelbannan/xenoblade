#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 func_8009D1F8(u32* buffer, s32 index);
extern "C" u32 func_80061FE8();

// Player-object state/event probe (retail unmangled C-ABI name; arg1 is the
// actor object + 0x8 state region, arg2 a probe id).
extern "C" int func_80148778(u8* obj, int id);

// Global flag-bit probe on the game-manager state word. Retail symbol is the
// C++-mangled func_8006EF04__Fi, so this is a plain global C++ declaration
// (CfCam.cpp defines it under C linkage with the same linker name).
bool func_8006EF04(int mask);

namespace cf {
class CtrlRemote;
class CtrlPc;
}

// Full retail layout view of cf::CtrlRemote (the CtrlPc.hpp declaration only
// exposes the implicit vptr; __ct__CtrlRemote initializes the fields below).
// The ctor stores the retail vtable manually, so this view carries the
// vptr slot for the manual store but relies on the caller's novtable class.
struct CtrlRemoteLayout {
    void* vtbl;                  // 0x00
    u8 _pad_04[0x1DC - 0x04];    // 0x04-0x1DB (CtrlPc-compatible base region)
    f32 mFloats1DC[32];          // 0x1DC-0x25B
    u32 mField25C;               // 0x25C
    u32 mField260;               // 0x260
};

// Retail CtrlPc sub-ctor called by __ct__CtrlRemote (defined in CtrlPc.cpp).
extern "C" void __ct__cf_CtrlPc(cf::CtrlPc* obj, void* posObj, void* arg5);

// Retail CtrlRemote ctor (0x800995BC). Returns `this` in r3; the CtrlPc.hpp
// scaffold declares it `void` (its caller discards the result), so this TU
// declares the real signature here and hides the scaffold decl via #define.
cf::CtrlRemote* __ct__CtrlRemote(cf::CtrlRemote* obj, void* posObj, int arg5);

// Serialized control-data buffer parsed by func_8009CE14:
// [0x00 u32 kind][0x04 u32 payload size][0x08 payload bytes].
struct CtrlRemoteBuf {
    u32 field_00;   // 0x00
    u32 mSize;      // 0x04
    u8 mData[4];    // 0x08 (payload follows the 8-byte header)
};

// Typed-parse helpers dispatched by func_8009CE14. Retail bodies live in the
// retail binary (not decompiled in this TU) - declare only so the call sites
// emit real `bl` relocs instead of inlining. C linkage keeps the retail
// unmangled reloc names.
extern "C" u32 func_8009C9B8(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg);
extern "C" u32 func_8009CAAC(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg);
extern "C" u32 func_8009CB80(cf::CtrlRemote* self, CtrlRemoteBuf* buf, u32 arg);

// Heap alloc/free callbacks installed into the UnkClass_80460C34Ctx below;
// defined in this TU (C linkage so the address-of relocs match retail).
extern "C" void* func_8009C8F4(cf::CtrlRemote* self, u32 a, u32 b);
extern "C" void func_8009C980(cf::CtrlRemote* self, u8* ptr);

// Retail bit-setter over the control-data buffer; stub body lives in this TU.
extern "C" void func_8009D2C8(u32* buffer, u32 index, u32 value);

// Control-index -> (word pointer, type, sub-index) mapper; defined in this TU.
extern "C" u32* func_8009D12C(u32* buffer, s32 index, s32* typeOut, s32* idxOut);

// External data (retail linker symbols)
extern "C" u32 lbl_eu_80571848[];
extern "C" u8 lbl_eu_80663E80;
extern "C" u32 lbl_eu_80661C70[2];
extern "C" u32 lbl_eu_80528048[4];
extern "C" u32 lbl_eu_80528058[32];

// Global flag word read by func_8009D5FC (retail: lwz r3, lbl_eu_80663E88@sda21).
extern u32 lbl_eu_80663E88;

// 0x38-byte stack context handed to the retail UnkClass_80460C34 methods by
// func_8009CAAC (fields at the offsets the retail prologue stores).
struct UnkClass_80460C34Ctx {
    u8* mPayload;    // 0x00 (buf + 8)
    u32 mArg;        // 0x04
    u8 _08[0x0C - 0x08];
    void* mSelf;     // 0x0C
    u32 mSize;       // 0x10
    u8 _14[0x20 - 0x14];
    void* mAlloc;    // 0x20 (func_8009C8F4)
    void* mFree;     // 0x24 (func_8009C980)
    u32 mZero;       // 0x28
    u8 _2C[0x38 - 0x2C];
};

// Retail UnkClass_80460C34 stream methods (retail binary only - declared so
// the call sites emit real `bl` relocs; C linkage keeps the retail names).
extern "C" int func_80460DCC__17UnkClass_80460C34Fv(void* self, const void* a, int b);
extern "C" int func_80460F58__17UnkClass_80460C34Fv(void* self, int a);
extern "C" int func_80461FE0__17UnkClass_80460C34Fv(void* self);

// Data pointer passed as the second arg of func_80460DCC (retail .data).
extern u8 lbl_eu_804FBC20[];

// Retail CtrlRemote vtable data (.data at 0x80527F88). Stored manually by
// __ct__CtrlRemote because the class is __declspec(novtable).
extern u8 lbl_eu_80527F88[];

// sdata2 float constant stored to the 32 floats at +0x1DC by __ct__CtrlRemote
// (const declaration so MWCC references the retail pool slot directly).
extern const f32 lbl_eu_80666730;
