// Decompiled TU for kyoshin/cf/object/CfObjectNpc
// NPC object implementation.
//
// CfObjectNpc represents an NPC in the game world. It extends CfObjectMove
// and adds NPC-specific fields and behavior: icon type (from BDAT icon_type),
// relationship meet value (rlt_meet), and a timer for dialogue triggers.

#include "kyoshin/cf/object/CfObjectNpc.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"   // CfGameManager::getInstance, isGlobalCamFlagSet
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include <string.h>
#include "kyoshin/plugin/ocBdat.hpp"


// .sdata2 constants (retail lbl_eu_80666AE0..lbl_eu_80666AFC). Declared before
// the code because MWCC lays .sdata2 out in declaration/first-use order: the
// builtin u8/u16->float conversions in loadNpcParams append their 2^52 pool
// double (retail lbl_eu_80666B00) after these eight floats, as in retail.
// Every use goes through *(const float*)&lbl... - a plain reference to a const
// scalar whose initializer is already visible is constant-folded into a second
// anonymous @N pool copy (duplicate .sdata2 objects and an unnamed reloc).
__attribute__((used)) const float lbl_eu_80666AE0 = 0.0f;
__attribute__((used)) const float lbl_eu_80666AE4 = 0.6f;
__attribute__((used)) const float lbl_eu_80666AE8 = 1.8f;
__attribute__((used)) const float lbl_eu_80666AEC = 0.1f;
__attribute__((used)) const float lbl_eu_80666AF0 = 30.0f;
__attribute__((used)) const float lbl_eu_80666AF4 = 25.0f;
__attribute__((used)) const float lbl_eu_80666AF8 = 12.5f;
__attribute__((used)) const float lbl_eu_80666AFC = 100.0f;

namespace cf {

// 0x800BFD48
// heapFlag selects which resource-impl child is allocated into +0xB0 (0x44
// bytes of CfResPcImpl when spawning with full PC resources, 0x20 bytes of
// CfResReloadImpl otherwise).
CfObjectNpc::CfObjectNpc(int heapFlag) {
    if (heapFlag != 0) {
        void* mem = mtl::MemManager::allocate(0x44, CfRes_getAllocHandle());
        if (mem != NULL) {
            mem = __ct__cf_CfResPcImpl((u8*)mem, this);
        }
        mSubObjB0 = mem;
    } else {
        void* mem = mtl::MemManager::allocate(0x20, CfRes_getAllocHandle());
        if (mem != NULL) {
            mem = __ct__cf_CfResReloadImpl(mem, this);
        }
        mSubObjB0 = mem;
    }

    // The 0x34 word is an unnamed CObjectParam gap word (cf. CfObjectModel
    // ctor Gap34); the header's unk34 lands at +0x24, so it cannot be used here.
    struct Gap34 { u8 _p00[0x34]; u32 w34; };   // 0x34 (unnamed gap word)
    f32 timerInit = *(const float*)&lbl_eu_80666AE0;
    int zero = 0;
    s16 invalid = -1;
    reinterpret_cast<Gap34*>(this)->w34 = (u32)zero;
    mIconType = (u8)zero;
    mRltMeet = invalid;
    mTimer = timerInit;
}

// 0x800BFE00
CfObjectNpc::~CfObjectNpc() {
    // Run the model-side cleanup via virtual dispatch (retail vtable slot 0x68).
    this->CfObject_releaseMoveTargets();
}

// 0x800BFE74
int CfObjectNpc::CfObject_UnkVirtualFunc2() {
    this->CfObjectModel::CfObject_UnkVirtualFunc2();
    mFlags68 |= 0x100000;
    CfObjectMove_setModelDisplayFlag(this, 1);
    CfObjectMove_setRegionAttached(this, 1);
    ColiSetMoveVec2(_60C_region, 0, *(const float*)&lbl_eu_80666AE4, *(const float*)&lbl_eu_80666AE8);
    *(u16*)&_60C_region[0xB2] = 0x15E;
    return 1;
}

// 0x800BFEE4
// Per-frame NPC update: decides whether to show the NPC's dialogue bubble
// and advances the trigger timer.
void CfObjectNpc::CfObject_UnkVirtualFunc4() {
    this->setStateBitMask3();

    bool resetTimer = true;

    // Nothing pending if the global mask is clear AND the NPC is not already
    // in its "meet" state: skip straight to the timer reset below.
    if ((lbl_eu_80663E24 & 0x9840000) == 0 &&
        this->checkStateFlags8(1) == 0) {
        // fall through to timer reset (resetTimer stays true)
    } else {
        getInstance__Q22cf13CfGameManagerFv();
        if (isGlobalCamFlagSet__Fi(0x40000) == 0 &&
            this->checkStateFlags8(1) == 0 &&
            this->checkStateFlags(1) == 0 &&
            this->checkStateFlags(0x10) == 0) {
            // Fast path: flag the model and reset the trigger timer.
            CfObjectModelSub98* model = this->mSubObj98;
            if (model != nullptr) {
                model->field_7A4 |= 0x80000000;
            }
            this->CfObject_refreshSubB0();
            this->CfObject_updateMoveRate();
            this->mTimer = *(const float*)&lbl_eu_80666AE0;
            return;   // retail b .L_800C014C (skips the vfunc4 hook)
        }

        // Dialogue-decided path.
        if (Scn_GetFrameDelta(lbl_eu_80663E14) > *(const float*)&lbl_eu_80666AE0 &&
            this->checkStateFlags8(1) != 0 &&
            this->checkStateFlags(1) == 0 &&
            this->checkStateFlags(0x10) == 0 &&
            UIWin_HasPendingUpdate(1) == 0) {
            if (this->mTimer > *(const float*)&lbl_eu_80666AEC) {
                this->applyStateFlags(3);
                static_cast<cf::CObjectParam*>(this)->CObjectParam_signalActionEnd(0);
                static_cast<cf::CfObjectModel*>(this)->CfObjectModel_bindModelTo(nullptr, "JUhead");
                this->mTimer = *(const float*)&lbl_eu_80666AE0;
            } else {
                // Local forces the scene-pointer load before the slot call,
                // matching retail's r31 hoist.
                CScn* scene = lbl_eu_80663E14;
                this->mTimer += Scn_GetFrameDelta(scene) *
                                static_cast<cf::CfObject*>(this)->CfObject_getMoveSpeedRate() / *(const float*)&lbl_eu_80666AF0;            }
            resetTimer = false;
        }
    }

    if (resetTimer) {
        this->mTimer = *(const float*)&lbl_eu_80666AE0;
    }
    // Direct (non-virtual) base call, matching retail's bl.
    this->CfObjectMove::CfObject_UnkVirtualFunc4();
}

// 0x800C016C
// Loads the NPC's display parameters from its BDAT row and pushes them into
// the model through a series of virtual setters. Columns come from the shared
// BDAT column-name table; numeric columns are converted with the classic
// 0x4330 integer-to-double bias constant.
void CfObjectNpc::CObjectParam_UnkVirtualFunc4() {
    Bdat_GetTable_AA34();
    void* fp = getFP((const char*)this + 0x78);

    // Length-probe the name column: if it exceeds 0x1F chars, fall back to
    // the fixed string baked into the column-name table at +0xC.
    if (strlen((const char*)getBdatStringColumnValue(
            fp, "name", unk8C_3)) > 0x1F) {
        static_cast<cf::CObjectParam*>(this)->CObjectParam_setObjectName("OVER");
    } else {
        static_cast<cf::CObjectParam*>(this)->CObjectParam_setObjectName((const char*)getBdatStringColumnValue(fp, "name", unk8C_3));
    }

    static_cast<cf::CfObject*>(this)->CfObject_setMoveValue(*(const float*)&lbl_eu_80666AF4);
    static_cast<cf::CfObjectMove*>(this)->CfObjectMove_recordMoveValue(*(const float*)&lbl_eu_80666AF8);

    // Speed column (+0x11): byte payload scaled by the model's current scale.
    // The (f32)(u32)(u8) casts are MWCC's builtin u8->float conversion: it
    // builds the 0x4330000000000000 stack pair and subtracts the TU's 2^52
    // pool double (retail lbl_eu_80666B00), which is emitted after the named
    // .sdata2 floats below.
    u32 col11 = getBdatStringColumnValue(
        fp, "move_speed", unk8C_3);
    static_cast<cf::CfObject*>(this)->CfObject_setMoveScale(
        (f32)*(u8*)&col11 /
        *(f32*)static_cast<cf::CfObject*>(this)->CfObject_getMoveRateScale());

    // Fade column (+0x1C): halfword payload divided by the fade-scale factor.
    u32 col1c = getBdatStringColumnValue(
        fp, "scale", unk8C_3);
    static_cast<cf::CfObject*>(this)->CfObject_setObjScale(
        (f32)*(u16*)&col1c / *(const float*)&lbl_eu_80666AFC);

    loadIconType();
}

// 0x800C0314
void CfObjectNpc::loadIconType() {
    Bdat_GetTable_AA34();
    void* fp = getFP((const char*)this + 0x78);
    u16 row = unk8C_3;
    // Read the icon_type column value (return value's low byte holds it).
    const char* v = (const char*)getBdatStringColumnValue(fp, "icon_type", row);
    mIconType = *(const u8*)&v;
}

// 0x800C0368
s16 CfObjectNpc::getRltMeet() {
    if (mRltMeet == -1) {
        Bdat_GetTable_AA34();
        void* fp = getFP((const char*)this + 0x78);
        u16 row = unk8C_3;
        // Lazily load the rlt_meet value from the BDAT column on first access.
        const char* v = (const char*)getBdatStringColumnValue(fp, "rlt_meet", row);
        // Pointer's low halfword holds the rlt_meet id (deref form, not a
        // cast: MWCC emits the retail stw+lha memory round-trip; a (s16)
        // cast folds to a direct sth).
        mRltMeet = *(const s16*)&v;
    }
    return mRltMeet;
}

/**
 * Returns the NPC icon type byte.
 * This value comes from the icon_type BDAT column and is stored at offset 0x71C.
 */
u8 CfObjectNpc::getIconType() {
    return mIconType;
}

} // namespace cf
