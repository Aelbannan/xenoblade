// Auto-scaffolded catalog TU for kyoshin/cf/code_8018F8D8
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/code_8018F8D8.hpp"

// C-linkage (unmangled) helpers referenced by the catalog functions below.
// These retail symbols are defined in other TUs / the linked library.
extern "C" {
    extern u32 lbl_eu_80663E24;
    extern u32 lbl_eu_80663E28;
    extern u32 lbl_eu_80664300;
    extern float lbl_eu_80667A70;
    int func_80061A80(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    void func_800BE12C(void* obj, int a, int b, int c, int d);
    void func_8012F860();
    void func_801338C8();
    void func_80133AE8();
    bool func_8011C2E8();
    void func_80080F40__Q22cf13CfGameManagerFv(void* this_, u32 second, u32 third);

    void func_801AAC78(int arg);
    int  func_800B8FC4();
    int  func_8023C1C0();
    int  func_8012FA5C();
    void func_8012FAA8();
    int  func_80062A84(s32 arg);
    int  func_802A3748(s32 value);
    void func_80135550();
    int  func_804962A0(CScn* scn, int flag);
    UnkR31_8019E88* func_8009D5FC();
    void func_8009EB2C(int a, int b, u8* c);
    int  func_eu_80062E58(u16 a, u16 b, int c);
    int  func_80061D2C(const void* obj, int idx);
    void func_801C3D9C(u8* obj);
    void func_801FA254(u8* obj);
    void func_8008566C__Q22cf13CfGameManagerFv(u32 mode, const UnkFloat4* value, u32 third);
    void func_80085878__Q22cf13CfGameManagerFv();
    int  func_80085838__Q22cf13CfGameManagerFv();
    void func_8007FECC__Q22cf13CfGameManagerFv();
    void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
    void func_8007E0D0__Q22cf13CfGameManagerFv(bool alternate);
}

namespace cf {}
using namespace cf;

int func_8018F8D8(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (lbl_eu_80663E24 & 0x8000) {
        func_80061A80(p0, 0x23, p1, p2, p3, p4);
    }
    return 1;
}

int func_8018F924(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (func_800B8FC4() != 0) {
        lbl_eu_80663E28 &= ~0x20;
        func_8007E0D0__Q22cf13CfGameManagerFv(true);
    } else {
        func_80061A80(p0, 0x24, p1, p2, p3, p4);
    }
    return 1;
}

void func_8018F9A0(){}

void func_8018FA2C(){}

int func_8018FC78(void* p0, void* p1, cf::CfGameManager* gm, u32* p3) {
    func_80080F40__Q22cf13CfGameManagerFv(gm, *p3, 0);
    return 0;
}

void func_8018FCA8(){}

int func_80190034(CFuncHost* self) {
    cf::CfGameManager* gm =
        reinterpret_cast<cf::CfGameManager*>(self->manager->unk94[0]);
    gm->func_80082C48();
    return 0;
}

int func_80190060(void* self) { return 0; }

int func_80190068(void* self) { return 0; }

int func_80190070(u32 p0, int p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p1 != 0) {
        func_80061A80(p0, 6, (u16)(p1 - 1), p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_801900C0(void* p0, int a, int b) {
    if (a != 0) {
        func_8012F860();
    }
    if (b != 0) {
        func_801338C8();
        func_80133AE8();
    }
    return 0;
}

int func_80190108(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (func_8012FA5C() != 0) {
        func_8012FAA8();
    } else {
        func_80061A80(p0, 0x1f, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_8019017C(CFuncHost* self) {
    self->manager->func_80086778();
    return 0;
}

void func_801901A4(){}

int func_80190254(int a, int b) { extern int func_801C0094(int); func_801C0094(b); return 0; }

int func_8019027C(void* self) { return 0; }

unsigned long func_80190284() {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 |= 8;
    return 0;
}

void func_80190298(){}

int func_80190334(u32 p0, u16 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p1 != 0) {
        func_80061A80(p0, 0x13, (u16)(p1 - 1), p2, p3, p4);
        r = 1;
    } else {
        lbl_eu_80663E28 &= ~1;
    }
    return r;
}

int func_80190394(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if ((u32)(u16)p2 + 0x10000 != 0xffff && func_802A3748((u16)p2)) {
        func_80061A80(p0, 0x16, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_80190414(void* p0, void* p1) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        func_800BE12C(player, (int)p1, 0, 0, 1);
    }
    return 0;
}

int func_80190464(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (func_80085838__Q22cf13CfGameManagerFv() == 0) {
        func_80061A80(p0, 0x18, p1, p2, p3, p4);
        return 1;
    }
    return 0;
}

void func_801904D0(){}

void func_80190568(){}

int func_80190690(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (func_8023C1C0() != 0) {
        func_80061A80(p0, 0x1e, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

extern "C" unsigned long func_801906FC() {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 &= ~0x80000;
    return 0;
}

int func_80190710() {
    func_8012F860();
    func_801338C8();
    func_80133AE8();
    return 0;
}

int func_8019073C() {
    if (func_8011C2E8() == 0) {
        func_801338C8();
    }
    return 0;
}

int func_8019076C(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p2 != 0 && func_80062A84(p2) == 0) {
        func_80061A80(p0, 0x1b, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_801907E4() {
    if (!(lbl_eu_80663E24 & 0x400000)) {
        float f = lbl_eu_80667A70;
        UnkFloat4 v;
        v.field_0x0 = f;
        v.field_0x4 = f;
        v.field_0x8 = f;
        v.field_0xC = f;
        func_8008566C__Q22cf13CfGameManagerFv(0x1e, &v, 0);
        func_80085878__Q22cf13CfGameManagerFv();
    }
    func_801AAC78(0);
    return 0;
}

int func_eu_80191E88(CFuncHost408* self, u32 arg1, u32 arg2) {
    UnkR31_8019E88* p = func_8009D5FC();
    func_8009EB2C((arg2 >> 20) & 0x7f, (arg2 >> 10) & 0x3ff, self->field_0x408 + 0x28);
    if (func_eu_80062E58(p->field_0x2, p->field_0x0, 3) == 0) {
        func_80061D2C(self, 0x28);
    }
    return 0;
}

int func_eu_80191F08(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (!(lbl_eu_80663E28 & 0x2000)) {
        func_80061A80(p0, 0x28, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

void func_80190840(){}

extern float lbl_eu_80667A90;
float func_80190938() { return lbl_eu_80667A90; }

void func_80190940(){}

CMenuPTState::~CMenuPTState() {
    // Scalar-deleting destructor: retail guards the whole destruction
    // (member dtors + CProcess base) with `if (this)` and emits a separate
    // `__dl` (operator delete) under the deleteFlag. Members are destroyed in
    // reverse declaration order (field_0x80, then field_0x60), matching retail.
    if (this) {
        field_0x80.~CPartyStateWin();
        field_0x60.~CBgTex();
    }
}

void CMenuPTState::Init() {}

void func_80191C88(){}

void func_80192268(){}

void CMenuPTState::Term() {
    CDeviceVI::waitForDrawDone();
    func_804962A0(field_0x5C, 1);
    field_0x5C->removeRenderCB(this);
    func_801C3D9C((u8*)&field_0x60);
    func_801FA254((u8*)&field_0x80);
    lbl_eu_80664300 = 0;
    func_8007FECC__Q22cf13CfGameManagerFv();
    func_8008294C__Q22cf13CfGameManagerFv(false);
    func_80135550();
}

void CMenuPTState::Move() {}

void CMenuPTState::cbRenderBefore() {}