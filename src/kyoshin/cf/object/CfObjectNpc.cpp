// Decompiled TU for kyoshin/cf/object/CfObjectNpc
// NPC object implementation.
//
// CfObjectNpc represents an NPC in the game world. It extends CfObjectMove
// and adds NPC-specific fields and behavior: icon type (from BDAT icon_type),
// relationship meet value (rlt_meet), and a timer for dialogue triggers.

#include "kyoshin/cf/object/CfObjectNpc.hpp"
#include "monolib/scn/CScnTimeApi.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"   // CfGameManager::getInstance, func_8006EF04
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include <string.h>

// 0x800BFD48 - CfObjectNpc constructor (retail symbol __ct__Q22cf11CfObjectNpcFv).
// See the hidden-parameter note on the header declaration: heapFlag selects
// which resource-impl child is allocated into +0xB0 (0x44 bytes of
// CfResPcImpl when spawning with full PC resources, 0x20 bytes of
// CfResReloadImpl otherwise). Defined at global scope (outside namespace cf)
// so MWCC emits the verbatim retail symbol.
struct CfObjectNpcInitView {
    void* vtable;                 // 0x00 - CfObjectNpc vtable
    u8 pad_04[0x34 - 0x04];
    u32 field_34;                 // 0x34 (cleared by the ctor)
    u8 pad_38[0xB0 - 0x38];
    void* subObjB0;               // 0xB0 - CfResPcImpl / CfResReloadImpl child
    u8 pad_B4[0x71C - 0xB4];
    u8 iconType;                  // 0x71C - NPC icon type
    u8 pad_71D;                   // 0x71D - alignment padding
    s16 rltMeet;                  // 0x71E - relationship meet value (-1 = unset)
    f32 timer;                    // 0x720 - dialogue trigger timer
};

cf::CfObjectNpc* __ct__Q22cf11CfObjectNpcFv(cf::CfObjectNpc* self, int heapFlag) {
    __ct__Q22cf12CfObjectMoveFv((cf::CfObjectMove*)self);
    CfObjectNpcInitView* view = (CfObjectNpcInitView*)self;
    view->vtable = lbl_eu_805298B8;

    if (heapFlag != 0) {
        void* mem = mtl::MemManager::allocate(0x44, func_80061FFC());
        if (mem != NULL) {
            mem = __ct__cf_CfResPcImpl((u8*)mem, (cf::CfObjectMove*)self);
        }
        view->subObjB0 = mem;
    } else {
        void* mem = mtl::MemManager::allocate(0x20, func_80061FFC());
        if (mem != NULL) {
            mem = __ct__cf_CfResReloadImpl(mem, self);
        }
        view->subObjB0 = mem;
    }

    // Shared locals (CfResReloadImpl ctor idiom): values are computed before
    // the member stores.
    f32 timerInit = lbl_eu_80666AE0;
    int zero = 0;
    s16 invalid = -1;
    view->field_34 = (u32)zero;
    view->iconType = (u8)zero;
    view->rltMeet = invalid;
    self->mTimer = timerInit;
    return self;
}

namespace cf {

// Unknown model-system object stored at offset 0x98 of the NPC. It carries a
// flags word at 0x7A4 that is OR-ed with a high bit to mark the model.
struct CfObjectNpc_model98 {
    u8 pad_0x00[0x7A4];   // 0x000 - 0x7A3
    u32 field_0x7A4;      // 0x7A4 - flag bits
};

// vtable slot 0x8C returns a f32 in retail even though the base header declares
// it void; the proxy's getScale8C() reads f1.
float CfObjectNpc::readSlot8c() {
    return reinterpret_cast<CfObjectNpcVt*>(this)->getScale8C();
}

// 0x800BFE00
CfObjectNpc::~CfObjectNpc() {
    // Run the model-side cleanup via virtual dispatch (retail vtable slot 0x68).
    this->CfObject_UnkVirtualFunc6();
}

// 0x800BFE74
bool CfObjectNpc::func_800BF46C() {
    this->CfObjectModel::CfObject_UnkVirtualFunc2();
    mFlags68 |= 0x100000;
    func_800BE33C(this, 1);
    func_800BE824(this, 1);
    func_804B0AD4(_60C_region, 0, lbl_eu_80666AE4, lbl_eu_80666AE8);
    *(u16*)&_60C_region[0xB2] = 0x15E;
    return true;
}

// 0x800BFEE4
// Per-frame NPC update: decides whether to show the NPC's dialogue bubble
// and advances the trigger timer.
void CfObjectNpc::func_800BF4DC() {
    this->CObjectState_UnkVirtualFunc13();

    bool resetTimer = true;

    // Nothing pending if the global mask is clear AND the NPC is not already
    // in its "meet" state: skip straight to the timer reset below.
    if ((lbl_eu_80663E24 & 0x9840000) == 0 &&
        this->CObjectState_UnkVirtualFunc8(1) == 0) {
        // fall through to timer reset (resetTimer stays true)
    } else {
        getInstance__Q22cf13CfGameManagerFv();
        if (func_8006EF04__Fi(0x40000) == 0 &&
            this->CObjectState_UnkVirtualFunc8(1) == 0 &&
            this->CObjectState_UnkVirtualFunc2(1) == 0 &&
            this->CObjectState_UnkVirtualFunc2(0x10) == 0) {
            // Fast path: flag the model and reset the trigger timer.
            CfObjectNpc_model98* model =
                *reinterpret_cast<CfObjectNpc_model98**>(
                    &this->field_0x90[0x08]);
            if (model != nullptr) {
                model->field_0x7A4 |= 0x80000000;
            }
            this->CfObject_UnkVirtualFunc12();
            this->CfObject_UnkVirtualFunc5();
            this->mTimer = lbl_eu_80666AE0;
            return;   // retail b .L_800C014C (skips the vfunc4 hook)
        }

        // Dialogue-decided path.
        if (func_80496288(lbl_eu_80663E14) > lbl_eu_80666AE0 &&
            this->CObjectState_UnkVirtualFunc8(1) != 0 &&
            this->CObjectState_UnkVirtualFunc2(1) == 0 &&
            this->CObjectState_UnkVirtualFunc2(0x10) == 0 &&
            func_8013EB90(1) == 0) {
            if (this->mTimer > lbl_eu_80666AEC) {
                this->CObjectState_UnkVirtualFunc5(3);
                // Slots 0x50 and 0x1AC take arguments though the base headers
                // declare them argument-less; call through the vtable proxy.
                reinterpret_cast<CfObjectNpcVt*>(this)->m50(0);
                reinterpret_cast<CfObjectNpcVt*>(this)->m1AC(
                    0, (const char*)lbl_eu_804FC580);
                this->mTimer = lbl_eu_80666AE0;
            } else {
                // Local forces the scene-pointer load before the slot call,
                // matching retail's r31 hoist.
                CScn* scene = lbl_eu_80663E14;
                this->mTimer += func_80496288(scene) *
                                this->readSlot8c() / lbl_eu_80666AF0;            }
            resetTimer = false;
        }
    }

    if (resetTimer) {
        this->mTimer = lbl_eu_80666AE0;
    }
    // Direct (non-virtual) base call, matching retail's bl.
    this->CfObjectMove::CfObject_UnkVirtualFunc4();
}

// 0x800C016C
// Loads the NPC's display parameters from its BDAT row and pushes them into
// the model through a series of virtual setters. Columns come from the shared
// BDAT column-name table; numeric columns are converted with the classic
// 0x4330 integer-to-double bias constant.
void CfObjectNpc::func_800BF764() {
    func_8003AA34();
    void* fp = getFP__FPCc((const char*)this + 0x78);

    // Length-probe the name column: if it exceeds 0x1F chars, fall back to
    // the fixed string baked into the column-name table at +0xC.
    if (strlen((const char*)getBdatStringColumnValue(
            fp, (const char*)&lbl_eu_804FC580[7], unk8C_3)) > 0x1F) {
        reinterpret_cast<CfObjectNpcVt*>(this)->setName(
            (const char*)&lbl_eu_804FC580[0xC]);
    } else {
        reinterpret_cast<CfObjectNpcVt*>(this)->setName(
            (const char*)getBdatStringColumnValue(
                fp, (const char*)&lbl_eu_804FC580[7], unk8C_3));
    }

    reinterpret_cast<CfObjectNpcVt*>(this)->setTime(lbl_eu_80666AF4);
    reinterpret_cast<CfObjectNpcVt*>(this)->moveTime(lbl_eu_80666AF8);

    // Speed column (+0x11): byte payload scaled by the model's current scale.
    // The plain (f32)(u32)(u8) casts emit MWCC's builtin 0x43300000 stack-slot
    // conversion (TU-local @N pool constant - see MWCC_PATTERNS "magic
    // constant" note); retail's schedule keeps the raw result in a register.
    u32 col11 = getBdatStringColumnValue(
        fp, (const char*)&lbl_eu_804FC580[0x11], unk8C_3);
    f32 val1 = (f32)(u32)(u8)col11 /
        *(f32*)reinterpret_cast<CfObjectNpcVt*>(this)->getScalePtr();
    reinterpret_cast<CfObjectNpcVt*>(this)->setSpeed(val1);

    // Fade column (+0x1C): halfword payload divided by the fade-scale factor.
    u32 col1c = getBdatStringColumnValue(
        fp, (const char*)&lbl_eu_804FC580[0x1C], unk8C_3);
    f32 val2 = (f32)(u32)(u16)col1c / lbl_eu_80666AFC;
    reinterpret_cast<CfObjectNpcVt*>(this)->setFade(val2);

    reinterpret_cast<CfObjectNpcVt*>(this)->applyParams();
}

// 0x800C0314
void CfObjectNpc::func_800BF8CC() {
    func_8003AA34();
    void* fp = getFP__FPCc((const char*)this + 0x78);
    u16 row = unk8C_3;
    // Read the icon_type column value (return value's low byte holds it).
    const char* v = (const char*)getBdatStringColumnValue(fp, (const char*)&lbl_eu_804FC580[0x22], row);
    mIconType = *(const u8*)&v;
}

// 0x800C0368
s16 CfObjectNpc::func_800BF920() {
    if (mRltMeet == -1) {
        func_8003AA34();
        void* fp = getFP__FPCc((const char*)this + 0x78);
        u16 row = unk8C_3;
        // Lazily load the rlt_meet value from the BDAT column on first access.
        const char* v = (const char*)getBdatStringColumnValue(fp, (const char*)&lbl_eu_804FC580[0x2C], row);
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
u8 CfObjectNpc::func_800BF984() {
    return mIconType;
}

} // namespace cf
