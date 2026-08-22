#pragma once

#include <types.h>

#include "kyoshin/cf/CfGimmick.hpp"

class UnkClass_8003AA34;
namespace cf { class CfGameManager; }

// ---------------------------------------------------------------------------
// C-linkage imports (unmangled retail symbols defined in other TUs).
// ---------------------------------------------------------------------------

// CfGimmick base-class constructor/destructor (retail emits them with
// unmangled C-linkage names; see __ct__cf_CfGimmick in CfGimmick.cpp).
extern "C" void __ct__cf_CfGimmick(void* self);
extern "C" void __dt__Q22cf9CfGimmickFv(void* self, int flag);

// Gimmick work-area registration helpers (register three column blocks with
// the bdat manager returned by func_8003AA34).
extern "C" void func_80208F34(void* self, void* block, void* mgr, void* holder);
extern "C" void func_80209020(void* self, void* block, void* mgr, void* holder);
extern "C" void func_80209288(void* self, void* block, void* mgr, void* holder);

// State-machine side effects shared with CfGimmickObject.
extern "C" void func_8020A03C();
extern "C" void func_8020A0CC();

// Poke the object registered at result+0x84 (save-point activation).
extern "C" void func_8008B95C(void* target);

// Scenario/sequence counter from the game manager.
extern "C" u32 func_800822F4__Q22cf13CfGameManagerFv();

// ---------------------------------------------------------------------------
// External data (retail linker symbols; global-scope variables keep their
// plain names without `extern "C"`).
// ---------------------------------------------------------------------------

// Bdat table descriptor whose +0x34 / +0x38 words are min/max column names.
extern u8 lbl_eu_805357E8[];
// Bdat table descriptor whose base is the type column name.
extern u8 lbl_eu_80510B28[];
// CfGimmickSaveOff vtable (stored at +0x00 by the constructor).
extern u8 lbl_eu_8053A1E0[];
// Bdat file holder seed handed to the registration helpers (.sdata pointer).
extern void* lbl_eu_80664140;

namespace cf {

// Save-point gimmick.  Flattened layout (CfGimmick base fields inlined) with
// a manual vptr at 0x00; the derived-vtable init hook sits at dispatch
// offset 0x20 (declared slot 6, shifted +2 for the RTTI/dtor headers).
class __declspec(novtable) CfGimmickSaveOff {
public:
    virtual ~CfGimmickSaveOff();   // vtable slot 0
    virtual void virt01();         // slot 1
    virtual void virt02();         // slot 2
    virtual void virt03();         // slot 3
    virtual void virt04();         // slot 4
    virtual void virt05();         // slot 5
    virtual void vinit();          // declared slot 6, dispatched at offset 0x20

    // vptr at 0x00 (implicit; written directly by the ctor/dtor).
    /* 0x04 */ u8 gap04[0x30 - 0x04];
    /* 0x30 */ f32 field_30;   // CfGimmick base extent fields
    /* 0x34 */ f32 field_34;
    /* 0x38 */ f32 field_38;
    /* 0x3C */ f32 field_3C;
    /* 0x40 */ f32 field_40;
    /* 0x44 */ s32 field_44;   // placement kind (jumptable index in the base)
    /* 0x48 */ u8 gap48[0x60 - 0x48];
    /* 0x60 */ u32 mState;      // jump table index
    /* 0x64 */ u16 mParam;      // saved constructor param
    /* 0x66 */ u8 _66[6];
    /* 0x6C */ u16 mMinVal;     // lower bound of the player-count gate
    /* 0x6E */ u16 mMaxVal;     // upper bound (0 = gate disabled)
    /* 0x70 */ u8 _70[0x12];
    /* 0x82 */ u16 mFlag;       // set to 8 by the constructor
    /* 0x84 */ u32 mType;       // completion behaviour selector (byte-extended)
};

// State-machine tick: optional range gate on the game-manager counter, then
// dispatch through the state jump table, then act on mType.
void func_802ABCB4(CfGimmickSaveOff* self);

} // namespace cf
