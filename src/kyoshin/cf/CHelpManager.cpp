// High-level C++ reconstruction for kyoshin/cf/CHelpManager

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CHelpManager.hpp"
// NOTE: do NOT include cf/CBattleManager.hpp here - its CfObjectActor ->
// CAIAction chain declares getInstance__Q22cf14CBattleManagerFv as void*,
// conflicting with the CBattleManagerView* declaration this TU already gets
// via kyoshin/harness_catalog.hpp (cf/CfGameManager.hpp).
#include "monolib/util/MemManager.hpp"

// Free-function form (not the member ctor): the retail ignores the incoming
// `this` and carves the real instance from MEM2 - the member form makes MWCC
// save the dead `this` in an extra saved register (r28).
extern "C" void* __ct__Q22cf12CHelpManagerFv(void* ignored) {
    cf::CHelpListHolder* end;
    cf::CHelpManagerVtblSlot* self = (cf::CHelpManagerVtblSlot*)mtl::MemManager::allocate(
        0x20, mtl::MemManager::getHandleMEM2());
    cf::CHelpListHolder* p;
    if (self != NULL) {
        // Install the retail interface table by hand.
        self->mVptr = (u8*)lbl_eu_8053911C;
        p = &self->mListA;
        end = &self->mListB + 1;
        do {
            func_802968A8(p);
            p++;
        } while (p < end);
        self->mField10 = -1;
        self->mField14 = 0;
        self->mField15 = 0;
        self->mLearnArtsFlag = 0;
        self->mField17 = 0;
        self->mField18 = 0;
    }
    lbl_eu_80664A10 = (cf::CHelpManager*)self;
}

void func_80295924() {
    // Reset the help-manager singleton: destroy it and clear the global.
    if (lbl_eu_80664A10 != NULL) {
        delete lbl_eu_80664A10;
        lbl_eu_80664A10 = NULL;
    }
}

cf::CHelpManager::~CHelpManager() {}

void func_802959AC(cf::CHelpManager* self) {
    self->mField10 = -1;
    self->mLearnArtsFlag = 0;
    self->mField17 = 0;
    self->mField18 = 0;
    func_802968B8(&self->mListA, NULL, 0);
    func_802968B8(&self->mListB, NULL, 0);
    self->mField10 = func_800822F4__Q22cf13CfGameManagerFv();
    func_802968B8(&self->mListB, lbl_eu_80538E30, 1);
    // Pick the item array whose key matches the current scenario sequence.
    for (int i = 0; lbl_eu_80538E90[i].mKey != 0; i++) {
        if (self->mField10 == lbl_eu_80538E90[i].mKey) {
            func_802968B8(&self->mListA, lbl_eu_80538E90[i].mItems, 0);
            return;
        }
    }
    func_802968B8(&self->mListA, NULL, 0);
}

void func_80295A88(cf::CHelpManager* self) {
    if (lbl_eu_80663E28 & 0x1000000) return;
    // Re-sync the list when the scenario sequence moved on.
    if (self->mField10 != func_800822F4__Q22cf13CfGameManagerFv()) {
        self->mField10 = func_800822F4__Q22cf13CfGameManagerFv();
        func_802968B8(&self->mListB, lbl_eu_80538E30, 1);
        for (int i = 0; lbl_eu_80538E90[i].mKey != 0; i++) {
            if (self->mField10 == lbl_eu_80538E90[i].mKey) {
                func_802968B8(&self->mListA, lbl_eu_80538E90[i].mItems, 0);
                goto done;
            }
        }
        func_802968B8(&self->mListA, NULL, 0);
    }
done:
    func_80296924(&self->mListA);
    func_80296924(&self->mListB);
    // Refresh the "learned arts" popup state (mirrors func_80295D30).
    self->mField14 = 0;
    self->mField15 = 0;
    int cond;
    if (self->mField17 == 0) {
        cond = 0;
    } else if (func_80085840__Q22cf13CfGameManagerFv() == 0) {
        cond = 0;
    } else {
        cond = (int)(((*(volatile u32*)&lbl_eu_80663E24 >> 22) & 1) ^ 1);
    }
    if (cond != 0) {
        self->mField17 = 0;
    }
}

void func_80295BAC() {
    // One-time "arts learned" hint: fire the popup and mark it seen.
    if (func_8009CF8C((u32)0x3395) == 0) {
        func_80134D18(0x56, 0, 0);
        func_8009D018(0x3395, 1);
    }
}

void func_80295BF4(cf::CHelpManager* self) {
    // Base hoisted into a callee-saved register (retail holds it in r31).
    cf::CHelpManagerTbl* tbl = &lbl_eu_80576D08;
    // Manual interface-table dispatch: slot 3 of the table stored at +0x8,
    // emitted as the CHelpVtblView f0C virtual call.
    if (self->mField10 == 3) {
        // bool local: MWCC's -O4,p idiom for the != 0 check (neg/or/srwi).
        bool isActive = func_8009CF8C((u32)tbl->mHelp1.mOwner) != 0;
        if (!isActive) {
            tbl->mHelp1.f0C();
        }
    }
    if (self->mField10 == 5) {
        bool isActive = func_8009CF8C((u32)tbl->mHelp2.mOwner) != 0;
        if (!isActive) {
            tbl->mHelp2.f0C();
        }
        bool isActive2 = func_8009CF8C((u32)tbl->mHelp3.mOwner) != 0;
        if (!isActive2) {
            tbl->mHelp3.f0C();
        }
    }
}

void func_80295CC8(cf::CHelpManager* self, cf::CHelpBattleObjArg* obj) {
    // During battle: record whether the actor list is non-empty.
    if (obj->mField3F08 & 0x10000) {
        // During battle: record whether the actor list (mActorList3) is
        // non-empty via the inlined reslist size walk.
        self->mField18 = (u8)(cf::CBattleManager::getInstance()->mActorList3.size() != 0);
    }
}

int func_80295D30(cf::CHelpManager* self) {
    // Two separate early-return guards, matching retail's branch-per-guard
    // shape (volatile read keeps MWCC from if-converting the guards).
    if (self->mField17 == 0)
        return 0;
    if (func_80085840__Q22cf13CfGameManagerFv() == 0)
        return 0;
    return (int)((((*(volatile u32*)&lbl_eu_80663E24) >> 22) & 1) ^ 1);
}

int CHelp_UnkVirtualFunc5__Q22cf5CHelpFv(void* self) { return 0; }

void CHelp_UnkVirtualFunc1__Q22cf5CHelpFv() {}

extern "C" void func_802B7C68(cf::CHelp* self);
void cf::CHelp_EtherMakeTIPS::func_80295D8C(void) { ::func_802B7C68(this); }

int CHelp_UnkVirtualFunc4__Q22cf5CHelpFv(void* self) { return 0; }

void cf::CHelp_TalkTIPS::func_80295D98(void) { ::func_802B7C68(this); }

void cf::CHelp_KizunaTIPS::func_80295D9C(void) { ::func_802B7C68(this); }

void cf::CHelp_EndEventTIPS::func_80295DA0(void) { ::func_802B7C68(this); }

int func_80295DA4__Q22cf12CHelp_AlwaysFv(void* self) { return 1; }

void cf::CHelp_AlwaysTIPS::func_80295DAC(void) { ::func_802B7C68(this); }

void func_80296898__Q22cf14CHelp_ToAttackFv(void* self) {
    ((unsigned char*)self)[0xc] = 0;
    ((unsigned char*)self)[0xd] = 0;
}

extern "C" DECOMP_DONT_INLINE void func_802968A8(cf::CHelpListHolder* self) {
    *(unsigned long*)self = 0;
    ((unsigned char*)self)[4] = 0;
}

extern "C" DECOMP_DONT_INLINE void func_802968B8(cf::CHelpListHolder* self, void** items, u8 flag) {
    self->mItems = items;
    self->mFlag = flag;
    if (items != NULL) {
        // Run each registered object's interface slot 2 (CHelp::UnkVirtualFunc1).
        for (int i = 0; self->mItems[i] != NULL; i++) {
            cf::CHelpTIPS* item = (cf::CHelpTIPS*)self->mItems[i];
            item->UnkVirtual1();
        }
    }
}

void cf::CHelpTIPS::UnkVirtual1() {}

// DECOMP_DONT_INLINE: retail calls this through real `bl`s from
// DECOMP_DONT_INLINE: retail calls this through real `bl`s from
// func_80295A88 / the sinit; MWCC would otherwise inline the body.
extern "C" DECOMP_DONT_INLINE void func_80296924(cf::CHelpListHolder* self) {
    if (self->mItems == NULL) return;
    if (func_802B7C64() != 0) return;
    int keep = 1;
    for (int i = 0; self->mItems[i] != NULL; i++) {
        // bool local: MWCC's -O4,p idiom for the != 0 check (neg/or/srwi).
        bool isActive = func_8009CF8C(
            (u32)reinterpret_cast<cf::CHelpDispatchIface*>(self->mItems[i])->mOwner) != 0;
        if (!isActive) {
            if (reinterpret_cast<cf::CHelpDispatchIface*>(self->mItems[i])->Slot4() != 0) {
                reinterpret_cast<cf::CHelpDispatchIface*>(self->mItems[i])->Slot3();
            }
            if (self->mFlag == 0) {
                return;
            }
            keep = 0;
        }
    }
    if (keep != 0) {
        self->mItems = NULL;
    }
}

// --- Static help-table construction (retail sinit_80295DB0) ---
// Builds every CHelp-derived entry of the static table lbl_eu_80576D08
// (.bss): base CHelp ctor call, then interface-table override at +0x8 and
// the extra payload writes. MWCC keeps the table base and the reused vtable
// addresses / small constants in callee-saved registers across the ctor
// calls (retail: base in r22, constants in r23-r31).
void sinit_80295DB0() {
    cf::CHelpTableFull* t = (cf::CHelpTableFull*)&lbl_eu_80576D08;

    // obj0 @ 0x00: base ctor, vtable override, extra word at +0xC.
    __ct__Q22cf5CHelpFv(&t->mObj00, (void*)0x3341, 0x2);
    t->mObj00.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B7C0;
    t->mObj00.mFieldC = 1;

    // obj1 @ 0x10: ctor then dispatch through vtable slot 2.
    __ct__Q22cf5CHelpFv(&t->mObj10, (void*)0x3342, 0x3);
    t->mObj10.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B378;
    ((cf::CHelpDispatchIface*)&t->mObj10)->Slot2();

    // obj2 @ 0x20: extra float at +0xC.
    __ct__Q22cf5CHelpFv(&t->mObj20, (void*)0x3343, 0x4);
    t->mObj20.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B770;
    t->mObj20.mFieldC = lbl_eu_80668BC8;

    // obj3 @ 0x30: dispatch type (same vtable as obj1).
    __ct__Q22cf5CHelpFv(&t->mObj30, (void*)0x3344, 0x5);
    t->mObj30.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B378;
    ((cf::CHelpDispatchIface*)&t->mObj30)->Slot2();

    __ct__Q22cf5CHelpFv(&t->mObj40, (void*)0x3345, 0x6);
    t->mObj40.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObj50, (void*)0x3346, 0x7);
    t->mObj50.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B3C0;

    __ct__Q22cf5CHelpFv(&t->mObj60, (void*)0x3347, 0x8);
    t->mObj60.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B770;
    t->mObj60.mFieldC = lbl_eu_80668BC8;

    __ct__Q22cf5CHelpFv(&t->mObj70, (void*)0x3348, 0x9);
    t->mObj70.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4F0;
    t->mObj70.mFieldC = 0;

    __ct__Q22cf5CHelpFv(&t->mObj84, (void*)0x3349, 0xA);
    t->mObj84.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B518;

    __ct__Q22cf5CHelpFv(&t->mObj90, (void*)0x334A, 0xB);
    t->mObj90.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B378;
    ((cf::CHelpDispatchIface*)&t->mObj90)->Slot2();

    __ct__Q22cf5CHelpFv(&t->mObjA0, (void*)0x334B, 0xC);
    t->mObjA0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObjA0.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObjB4, (void*)0x3351, 0x12);
    t->mObjB4.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObjC0, (void*)0x334C, 0xD);
    t->mObjC0.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObjCC, (void*)0x334D, 0xE);
    t->mObjCC.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObjD8, (void*)0x3365, 0x26);
    t->mObjD8.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObjE4, (void*)0x3396, 0x57);
    t->mObjE4.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObjF0, (void*)0x3361, 0x22);
    t->mObjF0.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObjFC, (void*)0x3362, 0x23);
    t->mObjFC.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj108, (void*)0x334E, 0xF);
    t->mObj108.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj118, (void*)0x334F, 0x10);
    t->mObj118.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObj124, (void*)0x3363, 0x24);
    t->mObj124.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj130, (void*)0x3352, 0x13);
    t->mObj130.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B6E8;

    __ct__Q22cf5CHelpFv(&t->mObj140, (void*)0x3353, 0x14);
    t->mObj140.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj140.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj154, (void*)0x3366, 0x27);
    t->mObj154.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj160, (void*)0x335F, 0x20);
    t->mObj160.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B6E8;

    __ct__Q22cf5CHelpFv(&t->mObj170, (void*)0x3354, 0x15);
    t->mObj170.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObj17C, (void*)0x3358, 0x19);
    t->mObj17C.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObj188, (void*)0x336D, 0x2E);
    t->mObj188.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj194, (void*)0x3368, 0x29);
    t->mObj194.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj1A0, (void*)0x3355, 0x16);
    t->mObj1A0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj1B0, (void*)0x3356, 0x17);
    t->mObj1B0.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObj1BC, (void*)0x3369, 0x2A);
    t->mObj1BC.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj1C8, (void*)0x336A, 0x2B);
    t->mObj1C8.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj1D8, (void*)0x3357, 0x18);
    t->mObj1D8.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj1E8, (void*)0x3383, 0x44);
    t->mObj1E8.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObj1F4, (void*)0x3359, 0x1A);
    t->mObj1F4.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj1F4.mField10 = (u32)lbl_eu_8050FF08;

    __ct__Q22cf5CHelpFv(&t->mObj208, (void*)0x335A, 0x1B);
    t->mObj208.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj214, (void*)0x3399, 0x5A);
    t->mObj214.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj220, (void*)0x3367, 0x28);
    t->mObj220.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj220.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj234, (void*)0x3350, 0x11);
    t->mObj234.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj234.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj248, (void*)0x335C, 0x1D);
    t->mObj248.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj258, (void*)0x335D, 0x1E);
    t->mObj258.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj268, (void*)0x336C, 0x2D);
    t->mObj268.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj268.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj27C, (void*)0x3398, 0x59);
    t->mObj27C.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj288, (void*)0x3393, 0x54);
    t->mObj288.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj288.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj2A0, (void*)0x3360, 0x21);
    t->mObj2A0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj2B0, (void*)0x3370, 0x31);
    t->mObj2B0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj2C0, (void*)0x3371, 0x32);
    t->mObj2C0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj2C0.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj2D8, (void*)0x335E, 0x1F);
    t->mObj2D8.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B490;

    __ct__Q22cf5CHelpFv(&t->mObj2E8, (void*)0x336F, 0x30);
    t->mObj2E8.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj2F8, (void*)0x339C, 0x5D);
    t->mObj2F8.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B458;

    __ct__Q22cf5CHelpFv(&t->mObj308, (void*)0x3372, 0x33);
    t->mObj308.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390E8;

    __ct__Q22cf5CHelpFv(&t->mObj314, (void*)0x3387, 0x48);
    t->mObj314.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj320, (void*)0x3373, 0x34);
    t->mObj320.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj320.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj334, (void*)0x335B, 0x1C);
    t->mObj334.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj334.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj348, (void*)0x3374, 0x35);
    t->mObj348.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj354, (void*)0x339A, 0x5B);
    t->mObj354.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj360, (void*)0x3375, 0x36);
    t->mObj360.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj360.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj374, (void*)0x3364, 0x25);
    t->mObj374.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B698;

    __ct__Q22cf5CHelpFv(&t->mObj380, (void*)0x3376, 0x37);
    t->mObj380.mVtbl = (cf::CHelpVtbl*)lbl_eu_80539064;
    t->mObj380.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj398, (void*)0x3388, 0x49);
    t->mObj398.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj3A8, (void*)0x339D, 0x5E);
    t->mObj3A8.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B490;

    __ct__Q22cf5CHelpFv(&t->mObj3B8, (void*)0x339B, 0x5C);
    t->mObj3B8.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj3B8.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj3CC, (void*)0x3377, 0x38);
    t->mObj3CC.mVtbl = (cf::CHelpVtbl*)lbl_eu_805390B8;

    __ct__Q22cf5CHelpFv(&t->mObj3D8, (void*)0x3378, 0x39);
    t->mObj3D8.mVtbl = (cf::CHelpVtbl*)lbl_eu_80539064;
    t->mObj3D8.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj3EC, (void*)0x3389, 0x4A);
    t->mObj3EC.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj3EC.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj400, (void*)0x338C, 0x4D);
    t->mObj400.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B660;

    __ct__Q22cf5CHelpFv(&t->mObj410, (void*)0x3379, 0x3A);
    t->mObj410.mVtbl = (cf::CHelpVtbl*)lbl_eu_80539064;
    t->mObj410.mField10 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj424, (void*)0x337A, 0x3B);
    t->mObj424.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B5D8;

    __ct__Q22cf5CHelpFv(&t->mObj430, (void*)0x337B, 0x3C);
    t->mObj430.mVtbl = (cf::CHelpVtbl*)lbl_eu_80539028;

    __ct__Q22cf5CHelpFv(&t->mObj43C, (void*)0x337C, 0x3D);
    t->mObj43C.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B5B0;

    __ct__Q22cf5CHelpFv(&t->mObj448, (void*)0x337E, 0x3F);
    t->mObj448.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B720;

    __ct__Q22cf5CHelpFv(&t->mObj454, (void*)0x3394, 0x55);
    t->mObj454.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B748;

    __ct__Q22cf5CHelpFv(&t->mObj460, (void*)0x336E, 0x2F);
    t->mObj460.mVtbl = (cf::CHelpVtbl*)lbl_eu_80538FD4;
    t->mObj460.mField10 = 0;
    t->mObj460.mField14 = 0;
    t->mObj460.mField15 = 1;
    t->mObj460.mField16 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj478, (void*)0x337F, 0x40);
    t->mObj478.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B6E8;

    __ct__Q22cf5CHelpFv(&t->mObj488, (void*)0x3380, 0x41);
    t->mObj488.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B420;

    __ct__Q22cf5CHelpFv(&t->mObj498, (void*)0x3381, 0x42);
    t->mObj498.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B7C0;
    t->mObj498.mFieldC = 2;

    // CHelp_LandMark entry: C-linkage ctor, then atexit destructor
    // registration with the cookie slot at tbl+0x4A8 (inside mObj498).
    __ct__cf_CHelp_LandMark(&t->mObj4B4, (void*)0x3382, 0x43, 0x6);
    __register_global_object(&t->mObj4B4,
                            (void*)__dt__Q22cf14CHelp_LandMarkFv,
                            &t->mObj498.mField10);

    __ct__Q22cf5CHelpFv(&t->mObj4D0, (void*)0x3384, 0x45);
    t->mObj4D0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B560;

    __ct__Q22cf5CHelpFv(&t->mObj4E0, (void*)0x3385, 0x46);
    t->mObj4E0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B638;
    t->mObj4E0.mFieldC = 9;

    __ct__Q22cf5CHelpFv(&t->mObj4F0, (void*)0x338A, 0x4B);
    t->mObj4F0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B798;
    t->mObj4F0.mField10 = 0x9C;
    t->mObj4F0.mField14 = 0;
    t->mObj4F0.mField15 = 0;
    t->mObj4F0.mField16 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj508, (void*)0x3397, 0x58);
    t->mObj508.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B588;
    t->mObj508.mFieldC = 0;

    __ct__Q22cf5CHelpFv(&t->mObj51C, (void*)0x338B, 0x4C);
    t->mObj51C.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B588;
    t->mObj51C.mFieldC = 0x11;
    t->mObj51C.mField10 = 1;

    __ct__Q22cf5CHelpFv(&t->mObj530, (void*)0x338D, 0x4E);
    t->mObj530.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B7C0;
    t->mObj530.mFieldC = 3;

    __ct__Q22cf5CHelpFv(&t->mObj540, (void*)0x338E, 0x4F);
    t->mObj540.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B3F8;
    t->mObj540.mFieldC = 0x2C;

    __ct__Q22cf5CHelpFv(&t->mObj550, (void*)0x338F, 0x50);
    t->mObj550.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B798;
    t->mObj550.mField10 = 0x19;
    t->mObj550.mField14 = 1;
    t->mObj550.mField15 = 0;
    t->mObj550.mField16 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj568, (void*)0x3390, 0x51);
    t->mObj568.mVtbl = (cf::CHelpVtbl*)lbl_eu_80538FD4;
    t->mObj568.mField10 = 0x19;
    t->mObj568.mField14 = 1;
    t->mObj568.mField15 = 0;
    t->mObj568.mField16 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj580, (void*)0x3391, 0x52);
    t->mObj580.mVtbl = (cf::CHelpVtbl*)lbl_eu_80538FD4;
    t->mObj580.mField10 = 0x19;
    t->mObj580.mField14 = 1;
    t->mObj580.mField15 = 0;
    t->mObj580.mField16 = 0;

    __ct__Q22cf5CHelpFv(&t->mObj598, (void*)0x3386, 0x47);
    t->mObj598.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B540;

    __ct__Q22cf5CHelpFv(&t->mObj5A4, (void*)0x3392, 0x53);
    t->mObj5A4.mVtbl = (cf::CHelpVtbl*)lbl_eu_80538F98;

    __ct__Q22cf5CHelpFv(&t->mObj5B0, (void*)0x336B, 0x2C);
    t->mObj5B0.mVtbl = (cf::CHelpVtbl*)lbl_eu_8053B6C0;
    t->mObj5B0.mFieldC = lbl_eu_80668BCC;
}
