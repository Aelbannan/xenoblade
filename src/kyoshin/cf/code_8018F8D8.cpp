// Auto-scaffolded catalog TU for kyoshin/cf/code_8018F8D8
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/code_8018F8D8.hpp"

// C-linkage (unmangled) helpers referenced by the catalog functions below.
// These retail symbols are defined in other TUs / the linked library.
extern "C" {
    extern u32 lbl_eu_80663E24;
    extern u32 lbl_eu_80663E28;
    int func_80061A80(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    void func_800BE12C(void* obj, int a, int b, int c, int d);
    void func_8012F860();
    void func_801338C8();
    void func_80133AE8();
    bool func_8011C2E8();
    void func_80080F40__Q22cf13CfGameManagerFv(void* this_, u32 second, u32 third);
}

namespace cf {}
using namespace cf;

int func_8018F8D8(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (lbl_eu_80663E24 & 0x8000) {
        func_80061A80(p0, 0x23, p1, p2, p3, p4);
    }
    return 1;
}

void func_8018F924(){}

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

void func_80190108(){}

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

void func_80190334(){}

void func_80190394(){}

int func_80190414(void* p0, void* p1) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        func_800BE12C(player, (int)p1, 0, 0, 1);
    }
    return 0;
}

void func_80190464(){}

void func_801904D0(){}

void func_80190568(){}

void func_80190690(){}

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

void func_8019076C(){}

void func_801907E4(){}

void func_eu_80191E88(){}

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

CMenuPTState::~CMenuPTState() {}

void CMenuPTState::Init() {}

void func_80191C88(){}

void func_80192268(){}

void CMenuPTState::Term() {}

void CMenuPTState::Move() {}

void CMenuPTState::cbRenderBefore() {}
