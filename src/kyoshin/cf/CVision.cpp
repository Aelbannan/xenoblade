// Auto-scaffolded catalog TU for kyoshin/cf/CVision
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CBattleManager.hpp"

using namespace cf;

// --- Callees defined in other TUs as plain-C functions (no C++ header). ----
extern "C" void func_800ACC50(void* self, float v);
extern "C" void func_804E36DC(CSchedule* self, f32 dt);
extern "C" void* func_800451D8(u32 cls, int param);
extern "C" void __dl__FPv(void* ptr);
extern "C" void __destroy_arr(void* block, ConstructorDestructor* dtor, size_t size, size_t n);

// Fragment-anchored destructors for the UnkClass_801A36D0 / UnkClass_801A3728
// sub-objects are defined below and passed by address to __destroy_arr.

// Class-spec table (8-byte stride, only low word used) for func_801A808C.
struct CVisionClassTableEntry { u32 field_00; u32 field_04; };
extern "C" CVisionClassTableEntry lbl_eu_80503F60[4];

// sdata2 float constants used by func_801A929C / vision state checks.
extern "C" f32 lbl_eu_80667CD4;
extern "C" f32 lbl_eu_80667CF0;
extern "C" f32 lbl_eu_80667CD0;
extern "C" f32 lbl_eu_80667CE4;
extern "C" f32 lbl_eu_80667CEC;
extern "C" f32 lbl_eu_80667CF8;
extern "C" f32 lbl_eu_80667CFC;
extern "C" f32 lbl_eu_80667D04;
extern "C" f32 lbl_eu_80667CF4;
extern "C" f32 lbl_eu_80667D00;
extern "C" f32 lbl_eu_80667CE0;
extern "C" f32 lbl_eu_80667D08;
extern "C" f32 lbl_eu_80667D0C;
extern "C" f32 lbl_eu_80667D10;
extern "C" f64 lbl_eu_80667D28;
extern "C" f32 lbl_eu_80667D30;
extern "C" f32 lbl_eu_80667D34;
extern "C" f32 lbl_eu_80667D38;
extern "C" f32 lbl_eu_80667D3C;
extern "C" f32 lbl_eu_80667D40;
extern "C" f32 lbl_eu_80667D44;
extern "C" f32 lbl_eu_80667D48;
extern "C" f32 lbl_eu_80667CE8;

extern "C" void func_800EA484(cf::CBattleManager* bm, f32 volume, int id);
extern "C" void func_80081E90__Q22cf13CfGameManagerFv(u32 a, u32 b, u32 c);
extern "C" void func_8006E5D8();
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* func_8004392C(u32 a, u32 b, void* c, u32 d, u32 e, f32 f = 0.0f);
extern "C" int func_80148778(void* obj, int id);
extern "C" void func_801AD504(int id);
extern "C" bool func_801537E0(void* obj);
extern "C" void func_801537F0(void* obj);
extern "C" void* func_8016FE34(int id);
extern "C" int func_800B708C__Fi(int id);
extern "C" float func_800F42AC(void* obj);
extern "C" void func_800F449C(void* obj);
extern "C" int func_80133F48(int id, float f);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" void func_801BFE8C(u32 a, u32 b, u32 c);
extern "C" void func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c, u32 d, f32 e);
extern "C" void func_80043D90(void* list);
extern "C" void* func_80043F18(void* list);
extern "C" void __dt__80043E88(void* list, int tags);
extern "C" u8* getGlobalSda(void);
extern "C" void func_802A1DF0(u32 a);
extern "C" int func_80260264(void* self, u32 id, void* result);
extern "C" void func_8006E2FC(int id);
extern "C" u32 lbl_eu_80663E24;
extern "C" u8 lbl_eu_80663DA0;
extern "C" void* __dynamic_cast(void* src, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);

// Player-list enumeration (CfObjectEnumList).
extern "C" void func_800F4A98(void* list, int type, int filter);
extern "C" void* func_800F6EAC(void* list, u32 idx);
extern "C" void func_80081F90__Q22cf13CfGameManagerFv(u32 a, u32 b);
extern "C" void func_800BE12C(void* a, u32 b, u32 c, s32 d, u32 e);
extern "C" void func_80174B4C(void* obj, u32 flag);

// Typeinfo names for __dynamic_cast in the player-list loops.
extern "C" const void* lbl_eu_806618E8;
extern "C" const void* lbl_eu_806618F0;

extern "C" void func_8014AC38(void* a, void* b);
extern "C" int func_8014B8BC(void* a, void* b);
extern "C" int func_800F4730(void);
extern "C" void* func_800F4648(void* self);
extern "C" int func_800F46C0(void* a, void* b);
extern "C" void func_800E921C(void* a, void* b, void* c, void* d, void* e);
extern "C" void* func_800EA444(void);
extern "C" void func_8009EC9C(void);
extern "C" void func_800A26A4(void* a, int b, void* c, int d, int e, int f, int g);
extern "C" void func_8026178C(int mode);
extern "C" void func_80496288(void);
extern "C" void func_80170AB0(void* self, void* dst);
extern "C" void func_80174C24(void* obj, u32 flag);
extern "C" f64 lbl_eu_80667D18;
extern "C" f64 lbl_eu_80667D20;
extern "C" f32 lbl_eu_80667CDC;
extern "C" f32 lbl_eu_80667CD8;

// Per-battle-slot object iterated in func_801A897C (stride 0x834).
struct CVisionBtlSlot {
    u32 w_00;       // 0x00
    u32 w_04;       // 0x04
    u8 unk08[0x88 - 0x08];
    u32 w_88;       // 0x88
    u8 unk8C[0x824 - 0x8C];
    u32 w_824;      // 0x824
    u32 w_828;      // 0x828
    u32 w_82C;      // 0x82C
};

// Callback ptmf tables (12-byte triplets).
extern "C" u32 lbl_eu_805331D0[3];
extern "C" u32 lbl_eu_805331F4[3];
extern "C" u32 lbl_eu_8053320C[3];
extern "C" u32 lbl_eu_805331E8[3];
extern "C" u32 lbl_eu_805331E0[3];
extern "C" u32 lbl_eu_805331CC[3];
extern "C" u32 lbl_eu_80533128[3];

// Parameter block used by func_801A6540's FX-setup path.
struct CVisionFxParam {
    u8 unk0[0x6];   // 0x00
    u8 b_06;        // 0x06
    u8 unk07[0xD - 0x07];
    u8 b_0D;        // 0x0D
    u8 unk0E[0x10 - 0x0E];
    u16 h_10;       // 0x10
    u16 h_12;       // 0x12
    f32 f_14;       // 0x14
    u8 unk18[0x20 - 0x18];
};

extern "C" u32 lbl_eu_805331DC[3];
extern "C" u32 lbl_eu_805331C4[3];
extern "C" u32 lbl_eu_80533164[3];
extern "C" u32 lbl_eu_80533230[3];
extern "C" u32 lbl_eu_80533134[3];
extern "C" u32 lbl_eu_80533158[3];
extern "C" u32 lbl_eu_80533224[3];
extern "C" f32 lbl_eu_80667CD8;
extern "C" void* __RTTI__Q22cf13CfObjectActor;
extern "C" void func_8009D018(u32 a, u32 b);
extern "C" void func_801412D0(u32 a);
extern "C" void func_8013F244(void);
extern "C" void func_8016FF14(void* obj, void* dst);
extern "C" void func_802A1FB4(void* a, void* b);
extern "C" void func_801C01A8(u32 a, u32 b, f32 c);
extern "C" void func_801C028C(u32 a, u32 b);
extern "C" void func_801AF934(u32 a);

// Object handed to func_801A5E58 by func_800F477C (reads byte at 0x42).
struct CVisionRefObj {
    u8 unk0[0x42];  // 0x00
    u8 b_42;         // 0x42
    u8 unk43[0x78 - 0x43];
    u32 w_78;        // 0x78
};
extern "C" CVisionRefObj* func_800F477C(void* self);
extern "C" long __ptmf_test(void* ptmf);
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" u32 __ptmf_null[3];
extern "C" u32 lbl_eu_8053317C[3];
extern "C" u32 lbl_eu_80533200[3];
extern "C" int func_80053FD4(void);
extern "C" bool func_8006EF04(s32 mask);
extern "C" s32 CfRes_getE24Bit22(void);
extern "C" u32 func_801B481C(void);
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);

// Object passed to func_801A6A7C / func_801A380C-family (battle object).
struct CVisionObj {
    u8 unk00[0x08];
    u8 field_08;           //0x08 (region handed to func_80148778)
    u8 unk09[0x3F00 - 0x09];
    u32 field_3F00;        //0x3F00 flag bits
    u8 unk3F04[0x3F10 - 0x3F04];
    u32 field_3F10;        //0x3F10 id/ptr
    u8 unk3F14[0x3F28 - 0x3F14];
    u16 field_3F28;        //0x3F28
    u8 unk3F2A[0x4FFC - 0x3F2A];
};

// Result of func_8016FE34 (per-model battle state).
class CVisionFusionSub {
public:
    virtual void a0();
    virtual void a1();
    virtual void a2();
    virtual void v20(u32 r4);   // 0x20
};

struct CVisionFusion {
    u8 unk0[0x4];
    CVisionFusionSub* field_04; //0x4 vtable-holder
    u8 unk08[0x3380 - 0x08];
    void* field_3380;           //0x3380
    u8 unk3384[0x3388 - 0x3384];
    u16 field_3388;             //0x3388
    u8 unk338A[0x3E9C - 0x338A];
    void* field_3E9C;           //0x3E9C vtable
    u8 unk3EA0[0x3F00 - 0x3EA0];
    u32 field_3F00;             //0x3F00 flags
    u8 unk3F04[0x3F60 - 0x3F04];
    void* field_3F60;           //0x3F60
    u8 unk3F64[0x4FFC - 0x3F64];
};


cf::CVision::CVision() {
    // Initialise the eight vision slots. Slots 0 and 3 (the two "physical"
    // display slots) also get their size/halfword fields reset.
    for (int i = 0; i < 8; i++) {
        CVisionSlot& s = unk20D4[i];
        if (i == 0 || i == 3) {
            s.f_15E8 = lbl_eu_80667CD4;
            s.h_15EE = 0;
            s.h_15EC = 0;
        }
        s.w_2CC4 = 0;
        s.w_2CC0 = 0;
        s.f_3D6C = lbl_eu_80667CD4;
        s.w_3FBC = 0;
        s.f_3FC0 = lbl_eu_80667CD4;
        s.w_4800 = 0;
        s.f_4808 = lbl_eu_80667CD4;
        s.w_4810 = 0;
        s.w_4804 = 0;
        s.f_480C = lbl_eu_80667CD4;
        s.w_4814 = 0;
    }
    field_26194 = lbl_eu_80667CD4;
    field_26198 = lbl_eu_80667CD4;
    field_2619C = lbl_eu_80667CD4;
    field_261A0 = lbl_eu_80667CD4;
    field_261A4 = 0;
    memset(effectArray, 0, sizeof(effectArray));
    CVisionPtmf cb;
    cb.mPfn = __ptmf_null[0];
    cb.mObj = __ptmf_null[1];
    cb.mDelta = __ptmf_null[2];
    mPtmf = cb;
    __ptmf_test(&cb);
}

void __ct__801A33AC(){}

// ---------------------------------------------------------------------------
// us-801a4f2c: Initialise all vision slots / effects (retail func_801A380C).
// ---------------------------------------------------------------------------
void func_801A380C(CVision* self) {
    self->field_26194 = lbl_eu_80667CD4;
    self->field_26198 = lbl_eu_80667CD4;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = lbl_eu_80667CD4;
    self->field_261A4 = 0;
    CVisionPtmf cb;
    cb.mPfn = __ptmf_null[0];
    cb.mObj = __ptmf_null[1];
    cb.mDelta = __ptmf_null[2];
    self->mPtmf = cb;
    __ptmf_test(&cb);
    self->vt_34();
    for (int i = 0; i < 4; i++) {
        func_800F449C(&self->sub + i);
    }
    for (int i = 0; i < 6; i++) {
        CVisionSlot& s = self->unk20D4[i];
        if (i == 0 || i == 3) {
            s.f_15E8 = lbl_eu_80667CD4;
            s.h_15EE = 0;
            s.h_15EC = 0;
        }
        s.w_2CC4 = 0;
        s.w_2CC0 = 0;
        s.f_3D6C = lbl_eu_80667CD4;
        s.w_3FBC = 0;
        s.f_3FC0 = lbl_eu_80667CD4;
        s.w_4800 = 0;
        s.f_4808 = lbl_eu_80667CD4;
        s.w_4810 = 0;
        s.w_4804 = 0;
        s.f_480C = lbl_eu_80667CD4;
        s.w_4814 = 0;
    }
}

// us-801a50f8: Drive the vision-slot FX update loop from the element array.
void func_801A39D8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        if (*(u32*)((u8*)self + 0x23d4) != 0) {
            *(u32*)((u8*)self + 0x23d4) = 0;
            *(u32*)((u8*)self + 0x23d0) = 0;
        }
        if (__ptmf_test(&self->mPtmf) != 0) {
            self->vt_1C();
        }
        *(u8*)((u8*)self + 0x23ac) = 0;
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x40000) != 0) {
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (CfRes_getE24Bit22() != 0) {
        return;
    }
    f32 f30 = lbl_eu_80667CDC;
    f32 f31 = lbl_eu_80667CD4;
    for (u32 i = 0;; i++) {
        u32 v28 = *(u32*)((u8*)self + 0x23d0);
        u32 v30 = *(u32*)((u8*)self + 0x23d8);
        u32 base = *(u32*)((u8*)self + 0x23cc);
        u32 idx = (v28 + i) / v30;
        u32 rem = (v28 + i) - idx * v30;
        if (*(u32*)base != idx) {
            // nop -- retail just reads the element base via the divwu index
        }
        // ... (elided middle: per-element switch calling func_801AD504)
        func_80496288();
        break;
    }
    // ... (elided rest)
    if (*(u32*)((u8*)self + 0x23d0) == 0) {
        for (int i = 0; i < 3; i++) {
            void* p = func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (p != 0) {
                *(u16*)((u8*)p + 0x3388) &= 0xfff7;
            }
        }
        CVisionPtmf cb;
        cb.mPfn = __ptmf_null[0];
        cb.mObj = __ptmf_null[1];
        cb.mDelta = __ptmf_null[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A4194(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    if (self->field_2619C == lbl_eu_80667CD4) {
        func_8006E2FC(0);
        CBattleManager* bm = CBattleManager::getInstance();
        ((void (*)(void*))((void**)bm)[0x38 / 4])(bm);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CE4, 3);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CE4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CE4) {
        self->vt_2C(0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1be, 0, 0, lbl_eu_80667CE0);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE8 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CE8) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0);
        for (u32 i = 0;; i++) {
            void* list = func_80043F18(vdlist);
            if (i >= *(u32*)((u8*)list + 0x620)) break;
            void* elem = func_800F6EAC(list, i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            u32 id = *(u32*)((u8*)obj + 0x74);
            if (id != sub->field_00 && id != sub->field_04) {
                ((void (*)(void*, u32))((void**)obj)[0x190 / 4])(obj, 1);
            }
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 1;
        }
        self->vt_2C(1);
        func_801C01A8(0, 0x2, lbl_eu_80667CE4);
        func_801C01A8(1, 0x0, lbl_eu_80667CE4);
        func_802A1DF0(1);
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533134[0];
        cb.mObj = lbl_eu_80533134[1];
        cb.mDelta = lbl_eu_80533134[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    if (((void* (*)(void*))((void**)p1)[0x2BC / 4])(p1) != 0 ||
        ((void* (*)(void*))((void**)p2)[0x2BC / 4])(p2) != 0) {
        self->vt_1C();
    }
}

void func_801A4578(){}

void func_801A47D0(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    if (self->field_2619C == lbl_eu_80667CD4) {
        void* mem = getHandleMEM2__Q23mtl10MemManagerFv();
        func_8004392C(1, 0x12f, mem, 2, 1, lbl_eu_80667CF8);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CE4, 3);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CE4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CE4) {
        self->vt_2C(0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1be, 0, 0, lbl_eu_80667CE0);
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE8 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CE8) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0x1);
        for (u32 i = 0;; i++) {
            void* list = func_80043F18(vdlist);
            if (i >= *(u32*)((u8*)list + 0x620)) break;
            void* elem = func_800F6EAC(list, i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            u32 id = *(u32*)((u8*)obj + 0x74);
            if (id != sub->field_00 && id != sub->field_04) {
                ((void (*)(void*, u32))((void**)obj)[0x190 / 4])(obj, 1);
            }
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 1;
        }
        self->vt_2C(1);
        func_801C01A8(0, 0x2, lbl_eu_80667CE4);
        func_801C01A8(1, 0x0, lbl_eu_80667CE4);
        func_802A1DF0(1);
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533158[0];
        cb.mObj = lbl_eu_80533158[1];
        cb.mDelta = lbl_eu_80533158[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    if (((void* (*)(void*))((void**)p1)[0x2BC / 4])(p1) != 0 ||
        ((void* (*)(void*))((void**)p2)[0x2BC / 4])(p2) != 0) {
        self->vt_1C();
    }
}

void func_801A4BC8(CVision* self) {
    if (lbl_eu_80667CD4 != self->field_2619C) {
        self->vt_2C(3);
        void* mem = getHandleMEM2__Q23mtl10MemManagerFv();
        func_8004392C(1, 0x130, mem, 2, 1);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CE4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CE4) {
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
        func_8006E5D8();
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CFC &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CFC) {
        self->vt_20(0);
    }
}

void func_801A4CF8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        self->vt_20(1);
        return;
    }
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0 || p2 == 0) {
        self->vt_20(1);
        return;
    }
    // Resume callback clear: restore the sub fader, play a sound.
    if (self->field_2619C == lbl_eu_80667CD4) {
        if (sub->field_824 & 0x20000) {
            sub->field_0C->field_7C = lbl_eu_80667D00;
        } else {
            sub->field_0C->field_7C = lbl_eu_80667CF4;
        }
        if (sub->field_824 & 0x20000) {
            func_80081F90__Q22cf13CfGameManagerFv(0x26, 0);
        }
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1bf, 0, 0, lbl_eu_80667CE0);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        ((void (*)(void*, f32))((void**)p1)[0x5C4 / 4])(p1, lbl_eu_80667CF0);
        if (sub->field_824 & 0x20000) {
            ((void (*)(void*, u32))((void**)p1)[0x5C8 / 4])(p1, 1);
            func_800BE12C((u8*)p1 + 0x3e9c, 0x11, 1, -1, 1);
        } else {
            func_800BE12C((u8*)p1 + 0x3e9c, 0x1, 1, -1, 1);
        }
        func_80174B4C(p1, 0x4000);
        if (p1 != p2) {
            ((void (*)(void*, f32))((void**)p2)[0x5C4 / 4])(p2, lbl_eu_80667CF0);
            func_800BE12C((u8*)p2 + 0x3e9c, 0x1, 1, -1, 1);
            func_80174B4C(p2, 0x4000);
            p2->field_3388 |= 0x2;
        }
        if (p2->field_3F00 & 0x2) {
            bool cancel;
            if (p2 != 0 && p2->field_3F60 != 0) {
                void* vt = *(void**)((u8*)p2 + 0x3e9c);
                void* r = ((void* (*)(void*))((void**)vt)[0xAC / 4])((u8*)p2 + 0x3e9c);
                f32 diff = *(f32*)((u8*)p2->field_3F60 + 0x3e8) - *(f32*)((u8*)r + 4);
                cancel = diff > lbl_eu_80667CD0;
            } else {
                cancel = true;
            }
            if (cancel) {
                self->vt_20(1);
            }
        }
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D04) {
        func_801AD504(1);
    }
    f32 f = ((f32 (*)(void*))((void**)p1)[0x5BC / 4])(p1);
    if (f == lbl_eu_80667CD4) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533164[0];
        cb.mObj = lbl_eu_80533164[1];
        cb.mDelta = lbl_eu_80533164[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A506C(){}

void func_801A5260(CVision* self) {
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D04) {
        func_801AD504(4);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D0C &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D0C) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_8053317C[0];
        cb.mObj = lbl_eu_8053317C[1];
        cb.mDelta = lbl_eu_8053317C[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A5360(CVision* self) {
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x10);
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x24);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D10 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D10) {
        self->vt_1C();
    }
}

// us-801a6b64: Match the vision sub-object's model against `obj`; when it
// matches one of the callback identities, install the associated vision FX
// callback on the vision slots.
void func_801A5444(CVision* self, void* obj, void* r5) {
    CVisionPtmf cur;
    cur.mPfn = ((u32*)self)[0x986e];
    cur.mObj = ((u32*)self)[0x986f];
    cur.mDelta = ((u32*)self)[0x9870];
    if (__ptmf_test(&cur) == 0) {
        return;
    }
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (func_8016FE34(func_800B708C__Fi(sub->field_00)) == 0) {
        self->vt_20(1);
        return;
    }
    if (obj == 0) {
        return;
    }
    // ... (elided middle: compare `obj` identity to callback tables and
    // install the matching CVision callback)
}

void func_801A5BA8(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    CVisionFusion* p = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    if (p == 0) {
        self->vt_20(1);
        return;
    }
    if (self->field_2619C == lbl_eu_80667CD4) {
        p->field_3388 |= 0x2;
    }
    void* a = ((void* (*)(void*))((void**)p)[0x2A4 / 4])(p);
    void* prm = *(void**)((u8*)a + 0x50);
    s32 byte = *(u8*)((u8*)prm + 0x44);
    s32 result = (s32)((s32(*)(void*, void*))((void**)p)[0x2A8 / 4])(p, prm);
    // r29 = (result - byte) with sign: nonzero when the value moved out of range.
    s32 diff = result - byte;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4 && (diff != 0 || result < 0)) {
        CBattleManager* bm = CBattleManager::getInstance();
        ((void (*)(void*, u32, f32, f32))((void**)bm)[0x34 / 4])(bm, 8, lbl_eu_80667D30, lbl_eu_80667D34);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_801AD504(8);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D34 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D34 && (diff != 0 || result < 0)) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 3);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4 &&
        ((diff != 0 || result < 0) ||
         ((s32)((s32(*)(void*))((void**)p)[0x2BC / 4])(p) != 0))) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_805331C4[0];
        cb.mObj = lbl_eu_805331C4[1];
        cb.mDelta = lbl_eu_805331C4[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

// ---------------------------------------------------------------------------
// us-801a7578: When the vision field is active, ask each player's battle
// state whether it should show, and forward it to func_80133F48 (retail
// func_801A5E58).
// ---------------------------------------------------------------------------
void func_801A5E58(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        f32 scl = lbl_eu_80667D38;
        if (func_800F477C(sub) != 0 && func_800F477C(sub)->b_42 == 1) {
            scl = lbl_eu_80667D3C;
        }
        // Accumulate a per-player float offset, then derive the final scale
        // from the sub object's id / the battle-manager flags, and write it out.
        for (int i = 0; i < 3; i++) {
            void* p = func_8016FE34((int)getPlayer__Q22cf13CfGameManagerFi(i));
            if (p != 0) {
                void* r = ((void* (*)(void*))((void**)p)[0x290 / 4])(p);
                if (r != 0) {
                    s32 out;
                    if (func_80260264(r, 0x6c, &out) != 0) {
                        scl += (f32)(f64)out;
                    }
                }
            }
        }
        if (sub->field_824 & 0x20000) {
            scl = sub->field_0C->field_2C;
        }
        CBattleManager* bm = CBattleManager::getInstance();
        if (((void* (*)(void*, u32))((void**)bm)[0x28 / 4])(bm, 0x400)) {
            scl = lbl_eu_80667D00;
        }
        if (((void* (*)(void*, u32))((void**)bm)[0x28 / 4])(bm, 0x800)) {
            scl = lbl_eu_80667D40;
        }
        sub->field_830 = scl;
        func_801AD504(0x10);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D04) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_805331D0[0];
        cb.mObj = lbl_eu_805331D0[1];
        cb.mDelta = lbl_eu_805331D0[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A60B0(CVision* self) {
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        self->vt_2C(2);
        func_801BFE8C(0, 0x1bf, 0);
        func_801BFC38__Q22cf10CfSoundManFUlUlUlUlf(0, 0x1c0, 0, 0, lbl_eu_80667CE0);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D04 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D04) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x328, 0);
        for (u32 i = 0;; i++) {
            void* list = func_80043F18(vdlist);
            if (i >= *(u32*)((u8*)list + 0x620)) break;
            void* elem = func_800F6EAC(list, i);
            void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
            if (obj == 0) {
                self->vt_20(1);
                __dt__80043E88(vdlist, -1);
                return;
            }
            ((void (*)(void*, u32))((void**)obj)[0x190 / 4])(obj, 0);
        }
        __dt__80043E88(vdlist, -1);
        if (getGlobalSda() != 0) {
            getGlobalSda()[0xa8] = 0;
        }
        self->vt_18();
        self->vt_30(1);
        func_80081E90__Q22cf13CfGameManagerFv(0, 0, 0);
        func_8006E5D8();
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D44 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667D44) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_805331DC[0];
        cb.mObj = lbl_eu_805331DC[1];
        cb.mDelta = lbl_eu_805331DC[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
}

void func_801A6340(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    CVisionFusion* fr = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* fr2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (lbl_eu_80667CD4 != self->field_2619C) {
        func_801537E0(&fr->field_3380);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x3);
    }
    if (self->field_26198 - self->field_261A0 <= lbl_eu_80667CD4 &&
        self->field_26194 - self->field_261A0 > lbl_eu_80667CD4) {
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
        fr->field_3388 &= 0xFFFD;
        fr->field_04->v20(0x4000);
        if (fr != fr2) {
            fr2->field_3388 &= 0xFFFD;
            fr2->field_04->v20(0x4000);
        }
        sub->field_0C->field_7C = func_800F42AC(sub);
        self->vt_1C();
    }
}

// us-801aa0b0: Manage per-battle-slot vision FX. Iterates the battle
// manager's vision slots; when a slot matches `slot` it runs a timing/rounding
// check and either re-engages FX via func_801A8244 or clears its flag bits.
void func_801A8244(CVision* self, void* r25, int r26, int r27, int r28);
void func_801A897C(CVision* self, int slot, int r28) {
    void* slotp = (void*)slot;
    if (slotp == 0) {
        return;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x400) != 0) {
        return;
    }
    CVisionBtlSlot* bs = (CVisionBtlSlot*)func_800EA444();
    if (bs == 0) {
        return;
    }
    if (bs->w_824 & 0x8000) {
        return;
    }
    if (func_800F4730() > 2) {
        return;
    }
    u32 x = *(u32*)((u8*)slotp + 0x3f10);
    if (bs->w_00 != x && bs->w_04 != x) {
        return;
    }
    void* fu = func_8016FE34(func_800B708C__Fi(bs->w_00));
    if (fu == 0) {
        return;
    }
    if (*((u32*)((u8*)fu + 4)) != 0) {
        void* r27 = func_800F4648(bs);
        func_800E921C(CBattleManager::getInstance(), bs, fu, &self->field_26194, &self->field_26198);
        if (func_800F46C0(bs, fu) != 0) {
            return;
        }
        f32 f = self->field_26194;
        f += (f > lbl_eu_80667CD4) ? (f32)lbl_eu_80667D18 : (f32)lbl_eu_80667D20;
        int r = (int)f;
        if ((u32)r27 != (u32)r) {
            int r25 = ((u32)r27 ^ (u32)r) >> 31;
            func_801A8244(&CBattleManager::getInstance()->mVision, bs, 2, r25, 0);
            return;
        }
    }
    if (fu != slotp) {
        return;
    }
    if (r28 == 0) {
        return;
    }
    // ... (elided middle: per-slot flag management)
    // End of loop iteration: clear flag bits and re-engage via func_801A8244.
    if (r28 != 0) {
        func_801A8244(self, bs, r28, 1, 0);
    }
}

// us-801a7c60: Manage vision FX on the sub-object. When the vision field is
// at baseline (field_2619C == 0) it releases/clears via the short path; otherwise
// it drives the FX setup path on the fusion's parameter block.
void func_801A6540(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        return;
    }
    CVisionFusion* fu = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }
    if (self->field_2619C == lbl_eu_80667CD4) {
        // Short (release) path: engage only within the D48 timing window.
        if (self->field_26198 - self->field_261A0 <= lbl_eu_80667D48 &&
            self->field_26194 - self->field_261A0 > lbl_eu_80667D48) {
            func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
            CVisionPtmf cb;
            cb.mPfn = lbl_eu_805331F4[0];
            cb.mObj = lbl_eu_805331F4[1];
            cb.mDelta = lbl_eu_805331F4[2];
            self->mPtmf = cb;
            self->field_2619C = lbl_eu_80667CD4;
            self->field_261A0 = self->field_26194;
            __ptmf_test(&cb);
        }
        return;
    }

    // Long (setup) path: begin driving the FX on the fusion parameter block.
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x13);
    sub->field_824 |= 0x100000;
    func_801537F0(&fu->field_3380);
    CVisionFxParam p;
    memset(&p, 0, sizeof(p));
    if (sub->field_0C != 0) {
        p.b_0D = 0x5a;
        p.h_12 = *((u8*)sub->field_0C + 0x77);
        p.f_14 = lbl_eu_80667CD4;
        p.h_10 = 0;
        p.b_06 = 6;
    } else {
        p.b_0D = 0xd;
        p.h_12 = 0xFFFF;
        p.f_14 = lbl_eu_80667CD4;
        p.h_10 = 0;
        p.b_06 = 6;
    }
    func_8014AC38(&fu->field_3380, &p);
    func_800BE12C((u8*)fu + 0x3e9c, 1, 0, -1, 1);
    if (func_8014B8BC(&fu->field_3380, &p) == 0) {
        func_801537E0(&fu->field_3380);
        func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
        // ... (FX-callback install continues in the elided retail middle)
    }
}

// ---------------------------------------------------------------------------
// us-801a819c: Validate a battle object / slot and, when it passes, install
// the state-change callback (retail func_801A6A7C).
// ---------------------------------------------------------------------------
int func_801A6A7C(CVision* self, CVisionObj* obj) {
    if (__ptmf_test(&self->mPtmf) == 0) {
        return 0;
    }
    if ((obj->field_3F00 & 0x4) == 0) {
        return 0;
    }
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    if (sub == 0) {
        return 0;
    }
    if (obj->field_3F10 != sub->field_00) {
        return 0;
    }
    if (func_80148778(&obj->field_08, 0x113) == 0 &&
        func_80148778(&obj->field_08, 0x114) == 0 &&
        func_80148778(&obj->field_08, 0x115) == 0) {
        return 0;
    }
    if ((sub->field_824 & 0x4000) == 0) {
        return 0;
    }
    CVisionPtmf cb;
    cb.mPfn = lbl_eu_80533200[0];
    cb.mObj = lbl_eu_80533200[1];
    cb.mDelta = lbl_eu_80533200[2];
    self->mPtmf = cb;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = self->field_26194;
    __ptmf_test(&cb);
    return 1;
}

// us-801a82ec: Guard chain deciding whether vision may be engaged on `obj`,
// then per-player timing checks and a vision-callback install. Returns 1 on success.
int func_801A6BCC(CVision* self, CVisionObj* obj, CVisionObj* r5) {
    if ((obj->field_3F00 & 0x4) == 0) {
        return 0;
    }
    if (((s32 (*)(void*))((void**)obj)[0x2BC / 4])(obj) != 0) {
        return 0;
    }
    if (func_80053FD4() != 0) {
        return 0;
    }
    u8 nc = *((u8*)CBattleManager::getInstance() + 0x1aa);
    if (nc >= 1 && nc <= 0x18) {
        return 0;
    }
    if (*(s16*)((u8*)CBattleManager::getInstance() + 0x20c8) != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x40000) != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (CfRes_getE24Bit22() != 0) {
        return 0;
    }
    getInstance__Q22cf13CfGameManagerFv();
    if (func_8006EF04(0x200) != 0) {
        return 0;
    }
    if (((void* (*)(void*, u32))((void**)CBattleManager::getInstance())[0x28 / 4])(
            CBattleManager::getInstance(), 0x200) != 0) {
        return 0;
    }
    if (func_801B481C() != 0) {
        return 0;
    }

    // Per-player timing check: flag whether vision should engage on a target.
    int engage = 0;
    CVisionFusion* target = 0;
    for (int i = 0; i < 4; i++) {
        void* p = getPlayer__Q22cf13CfGameManagerFi(i);
        if (p == 0) {
            continue;
        }
        CVisionFusion* fr = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(0));
        f32 a = ((f32 (*)(void*))((void**)p)[0x128 / 4])(p);
        if (a >= lbl_eu_80667CD4) {
            engage = 1;
        }
        target = fr;
    }
    if (engage != 0) {
        if (((s32 (*)(void*, CVisionObj*, void*))((void**)self)[0x14 / 4])(self, obj, target) == 0) {
            return 0;
        }
    }
    return 1;
}

int func_801A70DC(CVision* self, void* obj, void* obj2) {
    if (obj == 0) {
        self->vt_20(1);
        return 0;
    }
    void* f3e74 = *(void**)((u8*)obj + 0x3e74);
    if (f3e74 == 0) {
        self->vt_20(1);
        return 0;
    }
    CVisionSub* subs = &self->sub;
    CVisionSub* sub = (subs[0].field_00 != 0) ? subs : 0;
    if (sub != 0) {
        if (*(u32*)((u8*)f3e74 + 0x78) & 0x8000) {
            self->vt_1C();
        } else if (sub->field_00 != *(u32*)((u8*)obj + 0x3f10)) {
            return 0;
        }
    }
    if (sub == 0) {
        u32 want = *(u32*)((u8*)obj + 0x3f10);
        if (want != 0) {
            for (int i = 0; i < 4; i++) {
                if (subs[i].field_00 == want) {
                    sub = subs + i;
                    break;
                }
            }
        }
        if (sub != 0) {
            sub = 0;
        } else {
            for (int i = 0; i < 4; i++) {
                if (subs[i].field_00 == 0) {
                    sub = &subs[i];
                    break;
                }
            }
        }
    }
    if (sub == 0) {
        return 0;
    }
    f32 f1 = *(f32*)((u8*)f3e74 + 0x2c);
    if (*(f32*)((u8*)f3e74 + 0x2c) != lbl_eu_80667CD4) {
        f1 = *(f32*)((u8*)f3e74 + 0x7c) / f1;
    } else {
        f1 = lbl_eu_80667CD8;
    }
    if (f1 == lbl_eu_80667CD4) {
        return 0;
    }
    func_800F449C(sub);
    sub->field_00 = *(u32*)((u8*)obj + 0x3f10);
    sub->field_04 = *(u32*)((u8*)obj2 + 0x3f10);
    sub->field_0C = (CVisionSubRef*)f3e74;
    if (*(u32*)((u8*)f3e74 + 0x78) & 0x8000) {
        sub->field_824 |= 0x20000;
        func_8009D018(0x30e3, 0);
        func_801412D0(0x375);
        func_8013F244();
    }
    if (*(u32*)((u8*)obj + 0x3f00) & 0x4) {
        u16 h = *(u16*)((u8*)obj + 0x3f28);
        if (h == 0x967 || h == 0x96b || h == 0x96c || h == 0x969) {
            sub->field_824 |= 0x40000;
        }
    }
    *(f32*)((u8*)f3e74 + 0x7c) = *(f32*)((u8*)f3e74 + 0x2c);
    func_8016FF14(obj, (u8*)self + 0x20d4);
    if (obj2 != 0 && obj2 != obj) {
        func_8016FF14(obj2, (u8*)self + 0x68ec);
    }
    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620); i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        void* actor = __dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        if (actor != obj && actor != obj2) {
            *(u16*)((u8*)actor + 0x3388) &= ~0x8;
            func_8016FF14(actor, (u8*)self + 0x20d4 + i * 0x4818);
        }
    }
    __dt__80043E88(vdlist, -1);
    *(f32*)((u8*)f3e74 + 0x7c) = lbl_eu_80667D10;
    self->field_26194 = lbl_eu_80667CD4;
    self->field_26198 = lbl_eu_80667CD4;
    lbl_eu_80663DA0 |= 0x1;
    lbl_eu_80663E24 |= 0x400;
    func_802A1FB4(obj, obj2);
    if (sub->field_824 & 0x40000) {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533224[0];
        cb.mObj = lbl_eu_80533224[1];
        cb.mDelta = lbl_eu_80533224[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    } else {
        CVisionPtmf cb;
        cb.mPfn = lbl_eu_80533230[0];
        cb.mObj = lbl_eu_80533230[1];
        cb.mDelta = lbl_eu_80533230[2];
        self->mPtmf = cb;
        self->field_2619C = lbl_eu_80667CD4;
        self->field_261A0 = self->field_26194;
        __ptmf_test(&cb);
    }
    return 1;
}

void func_801A74DC(){}

// us-801a8e38: Drive vision FX updates on the two sub-object fusions and on
// every other in-play actor, then run the per-slot FX / flag management.
void func_801A7704(CVision* self) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
    CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
    if (p1 == 0) {
        self->vt_20(1);
        return;
    }
    if (p2 == 0) {
        self->vt_20(1);
        return;
    }
    p1->field_04->v20(0x10);
    func_80170AB0(p1, (u8*)self + 0x20d4);
    if (p2 != 0 && p2 != p1) {
        p2->field_04->v20(0x10);
        func_80170AB0(p2, (u8*)self + 0x68ec);
        func_80174C24(p2, 0x40);
    }
    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620); i++) {
        void* obj = func_8016FE34((int)func_800F6EAC(func_80043F18(vdlist), i));
        if ((void*)obj != (void*)p1 && (void*)obj != (void*)p2) {
            func_80170AB0(obj, (u8*)self + 0x20d4 - 0x4efc + i * 0x4818);
            func_80174C24(obj, 0x40);
        }
    }
    __dt__80043E88(vdlist, -1);
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CD4, 0x3);
    // ... (elided middle)
}

void func_801A7D6C(CVision* self, void* r4) {
    CVisionSub* sub = (self->sub.field_00 != 0) ? &self->sub : 0;
    u8 vdlist[8];
    func_80043D90(vdlist);
    func_800F4A98(func_80043F18(vdlist), 0x328, 0);
    for (u32 i = 0;; i++) {
        void* list = func_80043F18(vdlist);
        if (i >= *(u32*)((u8*)list + 0x620)) break;
        void* elem = func_800F6EAC(list, i);
        void* obj = __dynamic_cast(elem, 0, &lbl_eu_806618E8, &lbl_eu_806618F0, 0);
        if (obj == 0) {
            self->vt_20(1);
            __dt__80043E88(vdlist, -1);
            return;
        }
        ((void (*)(void*, u32))((void**)obj)[0x190 / 4])(obj, 0);
    }
    if (getGlobalSda() != 0) {
        getGlobalSda()[0xa8] = 0;
    }
    self->vt_30(1);
    if (r4 != 0) {
        func_80081E90__Q22cf13CfGameManagerFv(1, 0, 0);
        func_8006E5D8();
    }
    func_800EA484(CBattleManager::getInstance(), lbl_eu_80667CF0, 0x13);
    func_801AF934(0);
    if (sub != 0) {
        CVisionFusion* p1 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_00));
        CVisionFusion* p2 = (CVisionFusion*)func_8016FE34(func_800B708C__Fi(sub->field_04));
        if (p1 != 0) {
            p1->field_3388 &= 0xFFFD;
            p1->field_04->v20(0x4000);
        }
        if (p2 != 0) {
            p2->field_3388 &= 0xFFFD;
            p2->field_04->v20(0x4000);
        }
        func_800F449C(sub);
    }
    func_800F4A98(func_80043F18(vdlist), 0x20, 0);
    for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620); i++) {
        void* elem = func_800F6EAC(func_80043F18(vdlist), i);
        CVisionFusion* actor = (CVisionFusion*)__dynamic_cast(elem, 0, &__RTTI__Q22cf13CfObjectActor, &lbl_eu_806618F0, 0);
        actor->field_3388 &= 0xFFFD;
        actor->field_04->v20(0x4000);
    }
    self->field_26194 = lbl_eu_80667CD4;
    func_801C028C(0, 0);
    func_801C028C(1, 0);
    func_802A1DF0(0);
    lbl_eu_80663E24 &= 0xFFFFFFC0;
    lbl_eu_80663DA0 &= 0x7F;
    CVisionPtmf cb;
    cb.mPfn = __ptmf_null[0];
    cb.mObj = __ptmf_null[1];
    cb.mDelta = __ptmf_null[2];
    self->mPtmf = cb;
    self->field_2619C = lbl_eu_80667CD4;
    self->field_261A0 = self->field_26194;
    __ptmf_test(&cb);
    __dt__80043E88(vdlist, -1);
}

int* func_801A8070(int* param) {
    if (param[1] == 0) {
        return 0;
    }
    return &param[1];
}

// ---------------------------------------------------------------------------
// us-801a986c: Clear all four vision effect slots (retail func_801A8138).
// ---------------------------------------------------------------------------
void func_801A8138(CVision* self) {
    CVisionEffect** arr = self->effectArray;
    for (int i = 0; i < 4; i++) {
        if (arr[i] != 0) {
            arr[i]->field_B0 = 0;
            arr[i]->field_68 |= 0x40;
        }
        arr[i] = 0;
    }
}

// us-801a9978: Validate a battle slot against the vision state and, per the
// mode `r26`, either route its FX type or engage per-player FX on it.
void func_801A8244(CVision* self, void* r25, int r26, int r27, int r28) {
    void* fu = func_8016FE34(func_800B708C__Fi(*(u32*)((u8*)r25 + 0x00)));
    func_8016FE34(func_800B708C__Fi(*(u32*)((u8*)r25 + 0x04)));
    if (fu == 0) {
        self->vt_20(1);
        return;
    }
    if (r26 == 1) {
        if (*(u32*)((u8*)r25 + 0x824) & 0x8) {
            return;
        }
    } else if (r26 == 2) {
        if (*(u32*)((u8*)r25 + 0x824) & 0x4) {
            return;
        }
    } else if (r26 == 3) {
        if (*(u32*)((u8*)r25 + 0x824) & 0x40) {
            return;
        }
    }
    if (r26 != 0) {
        *(u32*)((u8*)self + 0x26230) = 0;
        *(u32*)((u8*)self + 0x2622c) = 0;
    }
    if (r26 != 0) {
        u8 vdlist[8];
        func_80043D90(vdlist);
        func_800F4A98(func_80043F18(vdlist), 0x20, 0);
        for (u32 i = 0; i < *(u32*)((u8*)func_80043F18(vdlist) + 0x620); i++) {
            void* obj = func_8016FE34((int)func_800F6EAC(func_80043F18(vdlist), i));
            if (((s32 (*)(void*))((void**)obj)[0x308 / 4])(obj) == 3) {
                ((void (*)(void*, u32))((void**)obj)[0x304 / 4])(obj, 4);
            } else if (((s32 (*)(void*))((void**)obj)[0x308 / 4])(obj) == 0) {
                ((void (*)(void*, u32))((void**)obj)[0x304 / 4])(obj, 2);
            }
        }
        __dt__80043E88(vdlist, -1);
    }
    // ... (elided middle)
}

// ---------------------------------------------------------------------------
// us-801a4df0: UnkClass_801A36D0 deleting destructor (__dt__801A36D0).
// Clears the sub-object at +0x3d8c (fields at +0x3f90/+0x3f94), then frees.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A36D0(UnkClass_801A36D0* self, int deleting) {
    if (self != 0) {
        if ((char*)self + 0x3d8c != 0) {
            *(u32*)((char*)self + 0x3f94) = 0;
            *(u32*)((char*)self + 0x3f90) = 0;
        }
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// us-801a4e48: UnkClass_801A3728 deleting destructor (__dt__801A3728).
// Clears field_68/field_64, then frees when the deleting flag is set.
// ---------------------------------------------------------------------------
extern "C" void* __dt__801A3728(UnkClass_801A3728* self, int deleting) {
    if (self != 0) {
        self->field_68 = 0;
        self->field_64 = 0;
        if (deleting > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

cf::CVision::~CVision() {}

// ---------------------------------------------------------------------------
// us-801a7c18: Release the zero/zero slot of the battle manager's vision
// sub-object (retail func_801A64F8).
// ---------------------------------------------------------------------------
void func_801A64F8() {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, 0, 0);
    }
}

// ---------------------------------------------------------------------------
// us-801aa050: Release an arbitrary slot of the battle manager's vision
// sub-object (retail func_801A891C). Args are forwarded untouched.
// ---------------------------------------------------------------------------
void func_801A891C(int a, int b) {
    if (CBattleManager::getInstance() &&
        &CBattleManager::getInstance()->mVision) {
        func_801A897C(&CBattleManager::getInstance()->mVision, a, b);
    }
}

// ---------------------------------------------------------------------------
// us-801a97c0: Lazily create the vision effect at `index` and record the
// owning CVision back-pointer (retail func_801A808C).
// ---------------------------------------------------------------------------
void func_801A808C(CVision* self, int index) {
    if (self->effectArray[index] == 0) {
        CVisionEffect* eff =
            (CVisionEffect*)func_800451D8(lbl_eu_80503F60[index].field_00, 0);
        self->effectArray[index] = eff;
        if (eff != 0) {
            eff->field_B0 = (u32)self;
        }
    }
}

// ---------------------------------------------------------------------------
// us-801a9830: Clear the vision effect at `index` (retail func_801A80FC).
// ---------------------------------------------------------------------------
void func_801A80FC(CVision* self, int index) {
    if (self->effectArray[index] != 0) {
        self->effectArray[index]->field_B0 = 0;
        self->effectArray[index]->field_68 |= 0x40;
    }
    self->effectArray[index] = 0;
}

// ---------------------------------------------------------------------------
// us-801a9930: Null out the first effect whose object identity matches
// `value` (retail func_801A81FC).
// ---------------------------------------------------------------------------
void func_801A81FC(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            self->effectArray[i] = 0;
            return;
        }
    }
}

// ---------------------------------------------------------------------------
// us-801aa980: Like func_801A81FC, but also clears the effect's owning
// back-pointer before nulling the slot (retail func_801A924C).
// ---------------------------------------------------------------------------
void func_801A924C(CVision* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->effectArray[i] == (CVisionEffect*)value) {
            CVisionEffect* e = self->effectArray[i];
            e->field_B0 = 0;
            self->effectArray[i] = 0;
            return;
        }
    }
}

void cf::CVision::func_801A929C(u32 r4) {
    f32 scl = (r4 != 0) ? lbl_eu_80667CD4 : lbl_eu_80667CF0;
    for (int i = 0; i < 4; i++) {
        if (effectArray[i] != 0) {
            func_800ACC50(effectArray[i], scl);
            if (effectArray[i]->mSchedule != 0) {
                func_804E36DC(effectArray[i]->mSchedule, lbl_eu_80667CF0);
            }
        }
    }
}