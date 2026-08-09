// Decompiled TU for kyoshin/cf/object/CfObjectNpc
// NPC object implementation.
//
// CfObjectNpc represents an NPC in the game world. It extends CfObjectMove
// and adds NPC-specific fields and behavior: icon type (from BDAT icon_type),
// relationship meet value (rlt_meet), and a timer for dialogue triggers.

#include "kyoshin/cf/object/CfObjectNpc.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"   // CfGameManager::getInstance, func_8006EF04

namespace cf {

// Unknown model-system object stored at offset 0x98 of the NPC. It carries a
// flags word at 0x7A4 that is OR-ed with a high bit to mark the model.
struct CfObjectNpc_model98 {
    u8 pad_0x00[0x7A4];   // 0x000 - 0x7A3
    u32 field_0x7A4;      // 0x7A4 - flag bits
};

// vtable slot 0x8C returns a f32 in retail even though the base header declares
// it void; call it through a float-typed member-function pointer to read f1.
float CfObjectNpc::readSlot8c() {
    typedef float (cf::CfObject::*CfVf15)();
    return (this->*reinterpret_cast<CfVf15>(
        &cf::CfObject::CfObject_UnkVirtualFunc15))();
}

// 0x800BFD48
CfObjectNpc::CfObjectNpc() {}

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
    bool resetTimer = true;

    this->CObjectState_UnkVirtualFunc13();

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
                this->CObjectParam_UnkVirtualFunc6();
                this->CfObjectModel_UnkVirtualFunc14();
                this->mTimer = lbl_eu_80666AE0;
            } else {
                this->mTimer += func_80496288(lbl_eu_80663E14) *
                                this->readSlot8c() / lbl_eu_80666AF0;
            }
            resetTimer = false;
        }
    }

    if (resetTimer) {
        this->mTimer = lbl_eu_80666AE0;
    }
    this->CfObject_UnkVirtualFunc4();
}

// 0x800C016C
void CfObjectNpc::func_800BF764() {}

// 0x800C0314
void CfObjectNpc::func_800BF8CC() {
    func_8003AA34();
    void* fp = getFP__FPCc((const char*)this + 0x78);
    u16 row = unk8C_3;
    // Read the icon_type column value (return value's low byte holds it).
    const char* v = getBdatStringColumnValue(fp, (const char*)&lbl_eu_804FC580[0x22], row);
    mIconType = (u8)(uintptr_t)v;
}

// 0x800C0368
s16 CfObjectNpc::func_800BF920() {
    if (mRltMeet == -1) {
        func_8003AA34();
        void* fp = getFP__FPCc((const char*)this + 0x78);
        u16 row = unk8C_3;
        // Lazily load the rlt_meet value from the BDAT column on first access.
        const char* v = getBdatStringColumnValue(fp, (const char*)&lbl_eu_804FC580[0x2C], row);
        mRltMeet = (s16)(uintptr_t)v;
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
