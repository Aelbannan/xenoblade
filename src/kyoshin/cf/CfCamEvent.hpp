#pragma once

#include <types.h>
#include "monolib/math.hpp"
#include "kyoshin/cf/CfCam.hpp"

namespace cf {

class CfCamEvent;

// Camera interface objects (CfCamDirectionIntf / CfCamLookatIntf /
// CfCamTargetIntf). Each is a stateless "vtable-only" interface: the instance
// is a pointer-sized slot whose first word is the interface vtable. With RTTI
// on, MWCC prefixes each vtable with offset-to-top + RTTI (slots 0-1), so the
// virtual at index N sits at vtable slot N+2. Slot 3 dispatches setPos,
// slot 4 setDir, slot 5 setLookat (see the intf helpers).
class CfCamIntf {
public:
    virtual void v_00();
    virtual void fn_0x0C(CfCamEvent* ev, void* arg);  // idx 1 -> 0x0C setPos
    virtual void fn_0x10(CfCamEvent* ev, void* arg);  // idx 2 -> 0x10 setDir
    virtual void fn_0x14(CfCamEvent* ev, void* arg);  // idx 3 -> 0x14 setLookat
};

// 48-byte camera vector block shared between the source object at unk160
// (offset 0x9C) and the event state (offset 0x130). The direction vector
// used by the angle check sits at +0x20.
struct CfCamBlock48 {
    u8 unk00[0x20];       // 0x00
    ml::CVec3 dir;        // 0x20
    f32 unk2C;            // 0x2C
};

// 0x25C camera body block - shared view used by func_8006A82C to refresh
// the direction/lookat/pos vectors in one pass.
struct CfCamBody25C {
    ml::CVec3 dir;      // +0x00 (unk25C)
    ml::CVec3 lookat;   // +0x0C (mLookat)
    ml::CVec3 pos;      // +0x18 (unk274)
    f32 unk24;          // +0x24 (unk280)
    f32 unk28;          // +0x28 (unk284)
    f32 unk2C;          // +0x2C (unk288)
};

// Camera source object referenced by CfCamEvent::unk160 (ctor arg2).
struct CfCamSrc160 {
    u8 unk00[0x9C];       // 0x00
    CfCamBlock48 block;   // 0x9C
};

// Linked-list node iterated by the cam-event flush loop in func_8006ACC0.
struct CfCamNode {
    CfCamNode* next;      // 0x00
    u8 unk04[4];          // 0x04
    void* field_8;        // 0x08 - payload object
};

// Payload object referenced by CfCamNode::field_8.
struct CfCamNodePayload {
    u8 unk00[0x68];        // 0x00
    u32 field_68;          // 0x68 - flags (bit 0x8000 skips the reset call)
    u8 unk6C[0x98 - 0x6C]; // 0x6C
    void* field_98;        // 0x98 - target object
};

// List head returned by CfGameManager::func_80086B0C. field_4 is a sentinel
// node: iteration starts at sentinel->next and stops back at the sentinel.
struct CfCamNodeList {
    CfCamNode* field_0;    // 0x00
    CfCamNode* sentinel;   // 0x04
};

// Camera event state block (CfCamFollow derives from CfCamEvent). The
// vtable is the full camera vtable; only the slots this TU dispatches
// through are named. Body layout (0x25C+) is shared with the intf helpers
// declared in CfCamDirectionIntf.hpp.
class CfCamEvent {
public:
    virtual void v_00();                          // idx 0  -> 0x08
    virtual void v_01();                          // idx 1  -> 0x0C
    virtual void v_02();                          // idx 2  -> 0x10
    virtual void fn_0x14(void* param);            // idx 3  -> 0x14 follow dispatch
    virtual void v_04();                          // idx 4  -> 0x18
    virtual ml::CVec3* v_05();                    // idx 5  -> 0x1C returns direction
    virtual void v_06();                          // idx 6  -> 0x20
    virtual void v_07();                          // idx 7  -> 0x24
    virtual void fn_0x28();                       // idx 8  -> 0x28 reset hook
    virtual void v_09();                          // idx 9  -> 0x2C
    virtual void v_10();                          // idx 10 -> 0x30
    virtual void v_11();                          // idx 11 -> 0x34
    virtual void v_12();                          // idx 12 -> 0x38
    virtual void v_13();                          // idx 13 -> 0x3C
    virtual void v_14();                          // idx 14 -> 0x40
    virtual void v_15();                          // idx 15 -> 0x44
    virtual void v_16(void* param);               // idx 16 -> 0x48
    virtual void v_17();                          // idx 17 -> 0x4C
    virtual void v_18();                          // idx 18 -> 0x50
    virtual void v_19();                          // idx 19 -> 0x54
    virtual void v_20();                          // idx 20 -> 0x58
    virtual void v_21();                          // idx 21 -> 0x5C
    virtual void v_22();                          // idx 22 -> 0x60
    virtual void fn_0x64(void* param);            // idx 23 -> 0x64 follow dispatch

    u32 unk4;                 // 0x04 - flag word
    s32 unk8;                 // 0x08 - cam type id (8/9/10 selects intf)
    u8 unk0C[0x10 - 0x0C];    // 0x0C
    ml::CVec3 unk10;          // 0x10 - source lookat
    u8 unk1C[0x28 - 0x1C];    // 0x1C
    ml::CVec3 unk28;          // 0x28 - position copy
    ml::CVec3 unk34;          // 0x34 - source pos
    ml::CVec3 unk40;          // 0x40
    u8 unk4C[0x58 - 0x4C];    // 0x4C
    ml::CVec3 unk58;          // 0x58
    ml::CVec3 unk64;          // 0x64
    u8 unk70[0x130 - 0x70];   // 0x70
    CfCamBlock48 unk130;      // 0x130 (0x30 bytes -> 0x160)
    CfCamSrc160* unk160;      // 0x160 - camera source object (ctor arg2)
    void* unk164;             // 0x164 - parent/handle (ctor arg1)
    u8 unk168[0x18C - 0x168]; // 0x168
    ml::CVec3 unk18C;         // 0x18C
    u8 unk198[0x1D4 - 0x198]; // 0x198
    u32 field_0x1D4;          // 0x1D4 - flag word
    u8 unk1D8[0x1DC - 0x1D8]; // 0x1D8
    f32 field_0x1DC;          // 0x1DC
    f32 field_0x1E0;          // 0x1E0
    u8 unk1E4[0x238 - 0x1E4]; // 0x1E4
    f32 field_0x238;          // 0x238
    u8 unk23C[0x25C - 0x23C]; // 0x23C
    ml::CVec3 unk25C;         // 0x25C - direction vector
    ml::CVec3 mLookat;        // 0x268 - current lookat target
    ml::CVec3 unk274;         // 0x274 - base position / orbit pivot
    f32 unk280;               // 0x280
    f32 unk284;               // 0x284 - camera speed factor
    f32 unk288;               // 0x288
    u8 unk28C[0x290 - 0x28C]; // 0x28C
    s32 field_0x290;          // 0x290 - angle trigger countdown
    u8 field_0x294;           // 0x294
    u8 unk295[0x298 - 0x295]; // 0x295

    char** initCamIntfInstances();
};

} // namespace cf

// C-linkage imports (retail symbol names - keep verbatim).
extern "C" float func_800749AC(ml::CVec3* pos274, ml::CVec3* lookat, ml::CVec3* out);
extern "C" void func_80074090(void* out);
extern "C" cf::CfCamIntf* CfCamEvent_initCamIntfInstances(cf::CfCamEvent* self);
extern "C" const f32 lbl_eu_80666268;
void func_8006BC1C(void* self, int mask);  // clears bits in the 0x04 flag word

// C-linkage imports for the camera-event factory (retail symbol names verbatim).
extern "C" u32 func_80061FE8();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
extern "C" cf::CfObject* func_8008221C__Q22cf13CfGameManagerFv();
extern "C" void func_80085878__Q22cf13CfGameManagerFv();
extern "C" u8 lbl_eu_80527048[];

// C-linkage imports for func_8006ACC0 (retail symbol names verbatim).
extern "C" void func_8007FE20__Q22cf13CfGameManagerFv(u32);
int func_8007FE24__Q22cf13CfGameManagerFv(u32);
extern "C" int func_8007F91C__Q22cf13CfGameManagerFv();
extern "C" cf::CfCamNodeList* func_80086B0C__Q22cf13CfGameManagerFv();
extern "C" void func_80071B78(void*);
extern "C" void func_8006D8D0(void*, void*);
extern "C" void* func_800BBC0C(void*);
extern "C" int func_800B8920(void*);
extern "C" void func_800707C0(void*, void*, void*);
extern "C" int func_80073640(void*, ml::CVec3*, ml::CVec3*, ml::CVec3*, int);
extern "C" void func_80071AB0(void*, void*, void*, int, float, float);
extern "C" int CfRes_getD80Flag();
extern "C" int func_804BE398(void*, u32, u32, u32, f32, f32);
extern "C" void func_804876DC();
extern "C" void func_80484E04(void*, int);
extern "C" void func_804876C0(void*);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" const f32 lbl_eu_8066626C;
extern "C" const f32 lbl_eu_80666270;
extern "C" const f32 lbl_eu_80666274;
extern "C" const f32 lbl_eu_80666278;
extern "C" const f32 lbl_eu_8066627C;
extern "C" const f32 lbl_eu_80666280;
extern "C" const f32 lbl_eu_80666284;
extern "C" const f32 lbl_eu_8066A20C;
extern const f32 lbl_eu_80661B58;
extern char lbl_eu_805262F0[];
extern char lbl_eu_805262C8[];
