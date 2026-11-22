// High-level C++ reconstruction for kyoshin/cf/CHelpManager

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/cf/CHelpManager.hpp"
#include <new>
#include "kyoshin/CTitle.hpp"
// cf::CBattleManager comes from CHelpManager.hpp's owning-header include.
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/cf/voice/CCharVoiceMan.hpp"

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
            clearHelpListHolder(p);
            p++;
        } while (p < end);
        self->mField10 = -1;
        self->mField14 = 0;
        self->mField15 = 0;
        self->mLearnArtsFlag = 0;
        self->mField17 = 0;
        self->mField18 = 0;
    }
    cf::CHelpManager::sInstance = (cf::CHelpManager*)self;
}

cf::CHelpManager* cf::CHelpManager::sInstance;

void destroyHelpManager() {
    // Reset the help-manager singleton: destroy it and clear the global.
    if (cf::CHelpManager::sInstance != NULL) {
        delete cf::CHelpManager::sInstance;
        cf::CHelpManager::sInstance = NULL;
    }
}

cf::CHelpManager::~CHelpManager() {}

void resetHelpManager(cf::CHelpManager* self) {
    self->mField10 = -1;
    self->mLearnArtsFlag = 0;
    self->mField17 = 0;
    self->mField18 = 0;
    bindHelpListItems(&self->mListA, NULL, 0);
    bindHelpListItems(&self->mListB, NULL, 0);
    self->mField10 = cf::CfGameManager::getQueuedFileEventCount();
    bindHelpListItems(&self->mListB, sDefaultHelpItemList, 1);
    // Pick the item array whose key matches the current scenario sequence.
    for (int i = 0; sScenarioHelpTable[i].mKey != 0; i++) {
        if (self->mField10 == sScenarioHelpTable[i].mKey) {
            bindHelpListItems(&self->mListA, sScenarioHelpTable[i].mItems, 0);
            return;
        }
    }
    bindHelpListItems(&self->mListA, NULL, 0);
}

void updateHelpManager(cf::CHelpManager* self) {
    if (gmPauseFlags & 0x1000000) return;
    // Re-sync the list when the scenario sequence moved on.
    if (self->mField10 != (s32)cf::CfGameManager::getQueuedFileEventCount()) {
        self->mField10 = (s32)cf::CfGameManager::getQueuedFileEventCount();
        bindHelpListItems(&self->mListB, sDefaultHelpItemList, 1);
        for (int i = 0; sScenarioHelpTable[i].mKey != 0; i++) {
            if (self->mField10 == sScenarioHelpTable[i].mKey) {
                bindHelpListItems(&self->mListA, sScenarioHelpTable[i].mItems, 0);
                goto done;
            }
        }
        bindHelpListItems(&self->mListA, NULL, 0);
    }
done:
    clearHelpList(&self->mListA);
    clearHelpList(&self->mListB);
    // Refresh the "learned arts" popup state (mirrors isHelpPopupReady).
    self->mField14 = 0;
    self->mField15 = 0;
    int cond;
    if (self->mField17 == 0) {
        cond = 0;
    } else if (isSceneReadyForInput__Q22cf13CfGameManagerFv() == 0) {
        cond = 0;
    } else {
        cond = (int)(((*(u32*)&gmGameFlags >> 22) & 1) ^ 1);
    }
    if (cond != 0) {
        self->mField17 = 0;
    }
}

void fireArtsLearnedHint() {
    // One-time "arts learned" hint: fire the popup and mark it seen.
    if (CtrlRemote_TouchBitByArg((u32)0x3395) == 0) {
        CUICfManager_queueTutorialMenu(0x56, 0, 0);
        CtrlRemote_SetSharedBit(0x3395, 1);
    }
}

void dispatchScenarioHelp(cf::CHelpManager* self) {
    // Base hoisted into a callee-saved register (retail holds it in r31).
    cf::CHelpTableFull* tbl = &sHelpDispatchObjects;
    if (self->mField10 == 3) {
        // bool local: MWCC's -O4,p idiom for the != 0 check (neg/or/srwi).
        bool isActive = CtrlRemote_TouchBitByArg((u32)tbl->mObj20.mOwner) != 0;
        if (!isActive) {
            tbl->mObj20.dispatchHelp();
        }
    }
    if (self->mField10 == 5) {
        bool isActive = CtrlRemote_TouchBitByArg((u32)tbl->mObj50.mOwner) != 0;
        if (!isActive) {
            tbl->mObj50.dispatchHelp();
        }
        bool isActive2 = CtrlRemote_TouchBitByArg((u32)tbl->mObj60.mOwner) != 0;
        if (!isActive2) {
            tbl->mObj60.dispatchHelp();
        }
    }
}

void updateBattleHelpState(cf::CHelpManager* self, cf::CVoiceActorState* obj) {
    // During battle: record whether the actor list is non-empty.
    if (obj->field_3F08 & 0x10000) {
        // During battle: record whether the actor list (mActorList3) is
        // non-empty via the inlined reslist size walk.
        self->mField18 = (u8)(cf::CBattleManager::getInstance()->mActorList3.size() != 0);
    }
}

int isHelpPopupReady(cf::CHelpManager* self) {
    // Two separate early-return guards, matching retail's branch-per-guard
    // shape (the pointer read keeps MWCC from if-converting the guards).
    if (self->mField17 == 0)
        return 0;
    if (isSceneReadyForInput__Q22cf13CfGameManagerFv() == 0)
        return 0;
    return (int)(((*(u32*)&gmGameFlags >> 22) & 1) ^ 1);
}

UNKWORD cf::CHelp::CHelp_UnkVirtualFunc5() { return 0; }

// Base slot-0x08 impl: no-op, the vtable target for direct CHelp leaves.
void cf::CHelp::CHelp_UnkVirtualFunc1() {}

void cf::CHelp_EtherMakeTIPS::dispatchEtherTips(void) { refreshPartyMenu(); }

UNKWORD cf::CHelp::CHelp_UnkVirtualFunc4() { return 0; }

void cf::CHelp_TalkTIPS::dispatchTalkTips(void) { refreshPartyMenu(); }

void cf::CHelp_KizunaTIPS::dispatchKizunaTips(void) { refreshPartyMenu(); }

void cf::CHelp_EndEventTIPS::dispatchEndEventTips(void) { refreshPartyMenu(); }

int cf::CHelp_Always::isHelpAlwaysEnabled() { return 1; }

void cf::CHelp_AlwaysTIPS::dispatchAlwaysTips(void) { refreshPartyMenu(); }

void cf::CHelp_ToAttack::clearHelpToAttackFlags() {
    field_0xC = 0;
    field_0xD = 0;
}

extern "C" void clearHelpListHolder(cf::CHelpListHolder* self) {
    *(unsigned long*)self = 0;
    ((unsigned char*)self)[4] = 0;
}

extern "C" void bindHelpListItems(cf::CHelpListHolder* self, void** items, u8 flag) {
    self->mItems = items;
    self->mFlag = flag;
    if (items != NULL) {
        // Run each registered object's vtable slot 0x08 (CHelp_UnkVirtualFunc1).
        for (int i = 0; self->mItems[i] != NULL; i++) {
            cf::CHelp* item = (cf::CHelp*)self->mItems[i];
            item->CHelp_UnkVirtualFunc1();
        }
    }
}

// Retail calls this through real `bl`s from updateHelpManager / the sinit.
extern "C" void clearHelpList(cf::CHelpListHolder* self) {
    if (self->mItems == NULL) return;
    if (CHelp_ForwardPartyNotice() != 0) return;
    int keep = 1;
    for (int i = 0; self->mItems[i] != NULL; i++) {
        // No item local: retail reloads self->mItems[i] at every use (the
        // virtual calls block CSE), so each access re-indexes (r29/r30/r31
        // coloring with no extra saved reg).
        // bool local: MWCC's -O4,p idiom for the != 0 check (neg/or/srwi).
        bool isActive = CtrlRemote_TouchBitByArg((u32)((cf::CHelp*)self->mItems[i])->mOwner) != 0;
        if (!isActive) {
            if (((cf::CHelp*)self->mItems[i])->f10() != 0) {
                ((cf::CHelp*)self->mItems[i])->dispatchHelp();
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
// Builds every CHelp-derived entry of the static table sHelpDispatchObjects
// (.bss): base CHelp ctor call, then interface-table override at +0x8 and
// the extra payload writes. MWCC keeps the table base and the reused vtable
// addresses / small constants in callee-saved registers across the ctor
// calls (retail: base in r22, constants in r23-r31).
void sinit_80295DB0() {
    cf::CHelpTableFull* t = &sHelpDispatchObjects;

    // obj0 @ 0x00: base ctor, vtable override, extra word at +0xC.
    new (&t->mObj00) cf::CHelp(0x3341, 0x2);
    t->mObj00.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B7C0;
    t->mObj00.mFieldC = 1;

    // obj1 @ 0x10: ctor then dispatch through vtable slot 2.
    new (&t->mObj10) cf::CHelp(0x3342, 0x3);
    t->mObj10.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B378;
    t->mObj10.CHelp_UnkVirtualFunc1();

    // obj2 @ 0x20: extra float at +0xC.
    new (&t->mObj20) cf::CHelp(0x3343, 0x4);
    t->mObj20.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B770;
t->mObj20.mFieldC = 1.0f;

    // obj3 @ 0x30: dispatch type (same vtable as obj1).
    new (&t->mObj30) cf::CHelp(0x3344, 0x5);
    t->mObj30.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B378;
    t->mObj30.CHelp_UnkVirtualFunc1();

    new (&t->mObj40) cf::CHelp(0x3345, 0x6);
    t->mObj40.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObj50) cf::CHelp(0x3346, 0x7);
    t->mObj50.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B3C0;

    new (&t->mObj60) cf::CHelp(0x3347, 0x8);
    t->mObj60.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B770;
t->mObj60.mFieldC = 1.0f;

    new (&t->mObj70) cf::CHelp(0x3348, 0x9);
    t->mObj70.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4F0;
    t->mObj70.mFieldC = 0;

    new (&t->mObj84) cf::CHelp(0x3349, 0xA);
    t->mObj84.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B518;

    new (&t->mObj90) cf::CHelp(0x334A, 0xB);
    t->mObj90.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B378;
    t->mObj90.CHelp_UnkVirtualFunc1();

    new (&t->mObjA0) cf::CHelp(0x334B, 0xC);
    t->mObjA0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObjA0.mField10 = 0;

    new (&t->mObjB4) cf::CHelp(0x3351, 0x12);
    t->mObjB4.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObjC0) cf::CHelp(0x334C, 0xD);
    t->mObjC0.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObjCC) cf::CHelp(0x334D, 0xE);
    t->mObjCC.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObjD8) cf::CHelp(0x3365, 0x26);
    t->mObjD8.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObjE4) cf::CHelp(0x3396, 0x57);
    t->mObjE4.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObjF0) cf::CHelp(0x3361, 0x22);
    t->mObjF0.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObjFC) cf::CHelp(0x3362, 0x23);
    t->mObjFC.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj108) cf::CHelp(0x334E, 0xF);
    t->mObj108.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj118) cf::CHelp(0x334F, 0x10);
    t->mObj118.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObj124) cf::CHelp(0x3363, 0x24);
    t->mObj124.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj130) cf::CHelp(0x3352, 0x13);
    t->mObj130.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B6E8;

    new (&t->mObj140) cf::CHelp(0x3353, 0x14);
    t->mObj140.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj140.mField10 = 0;

    new (&t->mObj154) cf::CHelp(0x3366, 0x27);
    t->mObj154.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj160) cf::CHelp(0x335F, 0x20);
    t->mObj160.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B6E8;

    new (&t->mObj170) cf::CHelp(0x3354, 0x15);
    t->mObj170.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObj17C) cf::CHelp(0x3358, 0x19);
    t->mObj17C.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObj188) cf::CHelp(0x336D, 0x2E);
    t->mObj188.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj194) cf::CHelp(0x3368, 0x29);
    t->mObj194.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj1A0) cf::CHelp(0x3355, 0x16);
    t->mObj1A0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj1B0) cf::CHelp(0x3356, 0x17);
    t->mObj1B0.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObj1BC) cf::CHelp(0x3369, 0x2A);
    t->mObj1BC.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj1C8) cf::CHelp(0x336A, 0x2B);
    t->mObj1C8.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj1D8) cf::CHelp(0x3357, 0x18);
    t->mObj1D8.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj1E8) cf::CHelp(0x3383, 0x44);
    t->mObj1E8.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObj1F4) cf::CHelp(0x3359, 0x1A);
    t->mObj1F4.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj1F4.mField10 = (u32)"vs01240050";

    new (&t->mObj208) cf::CHelp(0x335A, 0x1B);
    t->mObj208.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj214) cf::CHelp(0x3399, 0x5A);
    t->mObj214.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj220) cf::CHelp(0x3367, 0x28);
    t->mObj220.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj220.mField10 = 0;

    new (&t->mObj234) cf::CHelp(0x3350, 0x11);
    t->mObj234.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj234.mField10 = 0;

    new (&t->mObj248) cf::CHelp(0x335C, 0x1D);
    t->mObj248.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj258) cf::CHelp(0x335D, 0x1E);
    t->mObj258.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj268) cf::CHelp(0x336C, 0x2D);
    t->mObj268.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj268.mField10 = 0;

    new (&t->mObj27C) cf::CHelp(0x3398, 0x59);
    t->mObj27C.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj288) cf::CHelp(0x3393, 0x54);
    t->mObj288.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj288.mField10 = 0;

    new (&t->mObj2A0) cf::CHelp(0x3360, 0x21);
    t->mObj2A0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj2B0) cf::CHelp(0x3370, 0x31);
    t->mObj2B0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj2C0) cf::CHelp(0x3371, 0x32);
    t->mObj2C0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj2C0.mField10 = 0;

    new (&t->mObj2D8) cf::CHelp(0x335E, 0x1F);
    t->mObj2D8.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B490;

    new (&t->mObj2E8) cf::CHelp(0x336F, 0x30);
    t->mObj2E8.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj2F8) cf::CHelp(0x339C, 0x5D);
    t->mObj2F8.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B458;

    new (&t->mObj308) cf::CHelp(0x3372, 0x33);
    t->mObj308.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390E8;

    new (&t->mObj314) cf::CHelp(0x3387, 0x48);
    t->mObj314.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj320) cf::CHelp(0x3373, 0x34);
    t->mObj320.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj320.mField10 = 0;

    new (&t->mObj334) cf::CHelp(0x335B, 0x1C);
    t->mObj334.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj334.mField10 = 0;

    new (&t->mObj348) cf::CHelp(0x3374, 0x35);
    t->mObj348.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj354) cf::CHelp(0x339A, 0x5B);
    t->mObj354.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj360) cf::CHelp(0x3375, 0x36);
    t->mObj360.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj360.mField10 = 0;

    new (&t->mObj374) cf::CHelp(0x3364, 0x25);
    t->mObj374.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B698;

    new (&t->mObj380) cf::CHelp(0x3376, 0x37);
    t->mObj380.vtbl() = (cf::CHelpVtbl*)lbl_eu_80539064;
    t->mObj380.mField10 = 0;

    new (&t->mObj398) cf::CHelp(0x3388, 0x49);
    t->mObj398.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj3A8) cf::CHelp(0x339D, 0x5E);
    t->mObj3A8.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B490;

    new (&t->mObj3B8) cf::CHelp(0x339B, 0x5C);
    t->mObj3B8.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj3B8.mField10 = 0;

    new (&t->mObj3CC) cf::CHelp(0x3377, 0x38);
    t->mObj3CC.vtbl() = (cf::CHelpVtbl*)lbl_eu_805390B8;

    new (&t->mObj3D8) cf::CHelp(0x3378, 0x39);
    t->mObj3D8.vtbl() = (cf::CHelpVtbl*)lbl_eu_80539064;
    t->mObj3D8.mField10 = 0;

    new (&t->mObj3EC) cf::CHelp(0x3389, 0x4A);
    t->mObj3EC.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B4C8;
    t->mObj3EC.mField10 = 0;

    new (&t->mObj400) cf::CHelp(0x338C, 0x4D);
    t->mObj400.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B660;

    new (&t->mObj410) cf::CHelp(0x3379, 0x3A);
    t->mObj410.vtbl() = (cf::CHelpVtbl*)lbl_eu_80539064;
    t->mObj410.mField10 = 0;

    new (&t->mObj424) cf::CHelp(0x337A, 0x3B);
    t->mObj424.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B5D8;

    new (&t->mObj430) cf::CHelp(0x337B, 0x3C);
    t->mObj430.vtbl() = (cf::CHelpVtbl*)lbl_eu_80539028;

    new (&t->mObj43C) cf::CHelp(0x337C, 0x3D);
    t->mObj43C.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B5B0;

    new (&t->mObj448) cf::CHelp(0x337E, 0x3F);
    t->mObj448.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B720;

    new (&t->mObj454) cf::CHelp(0x3394, 0x55);
    t->mObj454.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B748;

    new (&t->mObj460) cf::CHelp(0x336E, 0x2F);
    t->mObj460.vtbl() = (cf::CHelpVtbl*)lbl_eu_80538FD4;
    t->mObj460.mField10 = 0;
    t->mObj460.mField14 = 0;
    t->mObj460.mField15 = 1;
    t->mObj460.mField16 = 0;

    new (&t->mObj478) cf::CHelp(0x337F, 0x40);
    t->mObj478.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B6E8;

    new (&t->mObj488) cf::CHelp(0x3380, 0x41);
    t->mObj488.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B420;

    new (&t->mObj498) cf::CHelp(0x3381, 0x42);
    t->mObj498.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B7C0;
    t->mObj498.mFieldC = 2;

    // CHelp_LandMark entry: C-linkage ctor, then atexit destructor
    // registration with the cookie slot at tbl+0x4A8 (inside mObj498).
    __ct__cf_CHelp_LandMark(&t->mObj4B4, (void*)0x3382, 0x43, 0x6);
    __register_global_object(&t->mObj4B4,
                            (void*)__dt__Q22cf14CHelp_LandMarkFv,
                            &t->mObj498.mField10);

    new (&t->mObj4D0) cf::CHelp(0x3384, 0x45);
    t->mObj4D0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B560;

    new (&t->mObj4E0) cf::CHelp(0x3385, 0x46);
    t->mObj4E0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B638;
    t->mObj4E0.mFieldC = 9;

    new (&t->mObj4F0) cf::CHelp(0x338A, 0x4B);
    t->mObj4F0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B798;
    t->mObj4F0.mField10 = 0x9C;
    t->mObj4F0.mField14 = 0;
    t->mObj4F0.mField15 = 0;
    t->mObj4F0.mField16 = 0;

    new (&t->mObj508) cf::CHelp(0x3397, 0x58);
    t->mObj508.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B588;
    t->mObj508.mFieldC = 0;

    new (&t->mObj51C) cf::CHelp(0x338B, 0x4C);
    t->mObj51C.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B588;
    t->mObj51C.mFieldC = 0x11;
    t->mObj51C.mField10 = 1;

    new (&t->mObj530) cf::CHelp(0x338D, 0x4E);
    t->mObj530.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B7C0;
    t->mObj530.mFieldC = 3;

    new (&t->mObj540) cf::CHelp(0x338E, 0x4F);
    t->mObj540.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B3F8;
    t->mObj540.mFieldC = 0x2C;

    new (&t->mObj550) cf::CHelp(0x338F, 0x50);
    t->mObj550.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B798;
    t->mObj550.mField10 = 0x19;
    t->mObj550.mField14 = 1;
    t->mObj550.mField15 = 0;
    t->mObj550.mField16 = 0;

    new (&t->mObj568) cf::CHelp(0x3390, 0x51);
    t->mObj568.vtbl() = (cf::CHelpVtbl*)lbl_eu_80538FD4;
    t->mObj568.mField10 = 0x19;
    t->mObj568.mField14 = 1;
    t->mObj568.mField15 = 0;
    t->mObj568.mField16 = 0;

    new (&t->mObj580) cf::CHelp(0x3391, 0x52);
    t->mObj580.vtbl() = (cf::CHelpVtbl*)lbl_eu_80538FD4;
    t->mObj580.mField10 = 0x19;
    t->mObj580.mField14 = 1;
    t->mObj580.mField15 = 0;
    t->mObj580.mField16 = 0;

    new (&t->mObj598) cf::CHelp(0x3386, 0x47);
    t->mObj598.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B540;

    new (&t->mObj5A4) cf::CHelp(0x3392, 0x53);
    t->mObj5A4.vtbl() = (cf::CHelpVtbl*)lbl_eu_80538F98;

    new (&t->mObj5B0) cf::CHelp(0x336B, 0x2C);
    t->mObj5B0.vtbl() = (cf::CHelpVtbl*)lbl_eu_8053B6C0;
t->mObj5B0.mFieldC = 300.0f;
}
