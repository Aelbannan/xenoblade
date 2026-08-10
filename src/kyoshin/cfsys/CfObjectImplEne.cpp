// Auto-scaffolded catalog TU for kyoshin/cfsys/CfObjectImplEne
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cfsys/CfObjectImplEne.hpp"

void __dt__Q22cf15CfObjectImplEneFv();

void func_800D0C2C();

// Releases the registered effect token at +0x36C (if any) and forwards the
// self pointer to func_800CAA44.
void func_800CFFCC(cf::CfObjectImplEneObj* self) {
    func_800CA948(self);
    self->vfE4();
    func_8015BB3C(self->field_14, self->field_18);
    func_802A0B8C(self->field_28, self->field_14);

    cf::CfObjectImplEneToken* p = (cf::CfObjectImplEneToken*)self->field_14;
    if (p != 0) {
        ml::FixStr<64> buf(true);
        if (func_800AA33C(buf, p->field_70, 0, 0) != 0) {
            if (strstr(buf.mString, lbl_eu_80661D48) != 0) {
                self->vf30(0x400, 1);
            }
        }
    }
}

void* func_800D0088(void* self) { return (void*)((u8*)self + 0x6c); }

// Releases the effect token at +0x36C (if set) and clears it, then forwards
// to func_800CAA44.
void func_800D0090(cf::CfObjectImplEneObj* self) {
    u32 v = self->field_36C;
    if (v != 0) {
        func_804E3CCC((void*)v);
        self->field_36C = 0;
    }
    func_800CAA44(self);
}

void func_800D00DC(){}

void func_800D02D4(){}

void func_800D0A58(void* self){ func_800D0C2C(); }

// Plays a sound based on `kind` when the battle object's +0x3F60 id matches.
// (extra params p4/p5/p6 are live-in from the virtual call site, unused here)
// Plays a sound based on `kind` when the battle object's +0x3F60 id matches.
// Extra params p4/p5/p6 are live-in from the virtual call site (unused here).
void func_800D0A60(cf::CfObjectImplEneObj* self, void* id, u32 kind, u32 p4, u32 p5, u32 p6) {
    if (id != self->field_18->field_3F60) return;
    func_800CD5DC(self);
    switch (kind) {
    case 7:
    case 0xb:
    case 5:
    case 0x31:
    case 2:
        func_801BFE8C(0, 0x1bb, 0);
        break;
    case 0x11:
        cf::CfSoundMan::func_801BFC38(0, 0x1bb, 0, 0, lbl_eu_80666CEC);
        break;
    }
}

void func_800D0AFC(void){}

void func_800D0B00(void){}

// Scans the battle actor's up-to-16 timeline entries; for each entry not
// already tracked by the battle manager, applies the shared action from
// vtable[0x4C] of the own sub-object and counts it.
int func_800D0B04(cf::CfObjectImplEneObj* self) {
    void* base = self->field_18;
    if (base != 0) base = (u8*)base + 0x3e9c;
    void* obj = func_800AD860(base);

    cf::CfObjectImplEneActor* actor = (cf::CfObjectImplEneActor*)func_800B8A64(obj);
    int count = 0;
    if (actor != 0 && (actor->field_A0 & 1) != 0) {
        void* v = ((cf::CfImplEneBattleObj*)self->field_18)->mSub.sf4C();
        void* src = func_8016FE34((int)func_800B708C((int)v));
        for (int i = 0; i < 0x10; i++) {
            cf::CfImplEneBattleObj* p =
                (cf::CfImplEneBattleObj*)func_800AD860((void*)func_800B708C((int)func_801984E4(actor, i)));
            if (p == 0) continue;
            if (p == (cf::CfImplEneBattleObj*)self->field_18) continue;
            if (func_800DA06C(cf::CBattleManager::getInstance(), (unsigned int)p) != 0) continue;
            if (p->field_3F34 == 0) continue;
            if (p->field_3F60 == 0) continue;
            func_800D9978(cf::CBattleManager::getInstance(), p);
            p->bhC4((int)src, lbl_eu_80666CEC, lbl_eu_80666CE4, lbl_eu_80666CE4);
            count++;
        }
    }
    return count;
}

void func_800D0C2C(){}

void func_800D1020(cf::CfObjectImplEneObj* self, u32 arg) {
    u32 v = self->field_36C;
    if (v == arg) {
        void* p = (u8*)self;
        if (self != 0) p = (u8*)self + 0x68;
        func_804E3D48((void*)v, p);
        self->field_36C = 0;
    }
}

cf::CfObjectImplEne::~CfObjectImplEne() {}

void func_800D10B4(void* self) { ((void(*)(void*))func_800D0A58)((char*)self - 0xc); }

void func_800D10BC(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplEneFv)((char*)self - 0xc); }

void func_800D10C4(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplEneFv)((char*)self - 0x10); }

void func_800D10CC(void* self) { ((void(*)(void*))func_800D1020)((char*)self - 0x68); }

void func_800D10D4(void* self) { ((void(*)(void*))__dt__Q22cf15CfObjectImplEneFv)((char*)self - 0x68); }
