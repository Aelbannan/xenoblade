// Auto-scaffolded catalog TU for kyoshin/CPassiveSkill
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPassiveSkill.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/code_80135FDC.hpp"

// Skill-info sub-struct initializer (defined later in this TU; noinline keeps
// the __ct__UI_CPassiveSkillInfo `bl` a real call).
void func_802641D0(UI_PassiveSkillInit* self, u32 arg);

// Skill-info update (defined later in this TU; C linkage so callers emit the
// plain retail reloc, noinline keeps the retail `bl`).
extern "C" void func_802660EC(UI::CPassiveSkillInfo* self);

// Line update tail (defined later in this TU; noinline keeps the retail `bl`).
void func_8026AAF4(UI::CPassiveSkillLine* self);

// Cursor step helper (defined later in this TU; noinline keeps the retail `bl`).
void func_80264140();

void func_80267BA0(void* self);
extern "C" __declspec(noinline) void func_80267C44(u8* self);
void func_80267CE0(void* self);
extern "C" __declspec(noinline) void func_80267E2C(u8* self);
void func_80267F88(void* self);
extern "C" __declspec(noinline) void func_80268250(u8* self);
void func_80268518(UI::CPassiveSkillLine* self);
extern "C" __declspec(noinline) void func_80268594(u8* self);
void func_80268C38(void* self);

// State-step helpers defined later in this TU. C linkage so the call relocs
// are the plain retail names (func_80269808 / func_80269924 / func_80269D20 /
// func_802640B8).
extern "C" void func_80269808(UI::CPassiveSkillLine* self);
extern "C" void func_80269924(UI::CPassiveSkillLine* self);
extern "C" void func_80269D20(UI::CPassiveSkillLine* self);
extern "C" void func_802640B8(u8* self, u8 arg);

// +0x28 sub-object updates (defined later in this TU; noinline keeps the
// `bl` from func_8026DAD0/func_8026D894).
__declspec(noinline) void func_80268BA8(u8* self);
__declspec(noinline) void func_80268F7C(UI::CPassiveSkillLine* self);
// +0x28 sub-object update tail (retail func_8026BB60, 0x944 bytes; stub kept
// noinline so func_802699A4 keeps the retail `bl`). C linkage so the call
// reloc is the plain retail name.
extern "C" __declspec(noinline) void func_8026BB60(UI::CPassiveSkillLine* self);
void func_8026DCF4(UI_CPassiveSkill* self);
void func_8026DD3C(UI_CPassiveSkill* self);

// this-adjusting thunk: forward to the sub-object at +0x28 (retail `UI_CPassiveSkill_thunk28_68594`).
void UI_CPassiveSkill_thunk28_68594(u8* self) { func_80268594(self + 0x28); }


// Cursor constructor: installs the vtable and inits the CBaseCur-compatible
// layout (visible=1, everything else 0). Written as a global function so the
// emitted symbol is exactly the retail `__ct__UI_CPassiveSkillCur` (MWCC does
// not mangle global-scope names).
void* __ct__UI_CPassiveSkillCur(UI::CPassiveSkillCur* _this, nw4r::lyt::ArcResourceAccessor* accessor) {
    _this->mVtbl = (void*)lbl_eu_80537A14;
    _this->mArcResAcc = accessor;
    _this->mpLayout = 0;
    _this->mpAnimTrans0 = 0;
    _this->mpAnimTrans1 = 0;
    _this->mActive = 0;
    _this->mVisible = 1;
    _this->field_16 = 0;
    return _this;
}

UI::CPassiveSkillCur::~CPassiveSkillCur() {}

// Skill-info animation step: while the +0x18 active flag is set, dispatch on
// the +0x1B mode byte (0 = func_802644D4 path, 1 = func_80264588 path) and
// then advance the +0x4 layout animation (vtbl 0x38, Animate(0)).
void func_8026439C(UI_PassiveSkillInit* self) {
    if (self->field_18 == 0) {
        return;
    }
    switch (self->field_1B) {
    case 0:
        func_802644D4(self);
        break;
    case 1:
        func_80264588(self);
        break;
    }
    self->field_4->Animate(0);
}

void func_8026440C(UI_PassiveSkillInit* self) {
    // Animation trigger: when the init flag is 1, rewind the anim transform,
    // mark the sub-state active and show the skill pane.
    if (self->field_1B == 1) {
        self->field_C->SetFrame(lbl_eu_80668904);
        self->field_1A = 1;
        func_80124270(self->field_14, 1);
    } else {
        self->field_19 = 0;
        self->field_1A = 1;
        func_80124270(self->field_14, 1);
    }
}

void func_80264470(UI_PassiveSkillInit* self) {
    // Same trigger as func_8026440C but drives the other pane (+0x10).
    if (self->field_1B == 1) {
        self->field_C->SetFrame(lbl_eu_80668904);
        self->field_1A = 0;
        func_80124270(self->field_10, 1);
    } else {
        self->field_19 = 0;
        self->field_1A = 0;
        func_80124270(self->field_10, 1);
    }
}

__declspec(noinline) void func_802644D4(UI_PassiveSkillInit* self) {}

__declspec(noinline) void func_80264588(UI_PassiveSkillInit* self) {}

// Skill-info constructor: installs the vtable, stores the caller arg, zeroes
// the word fields, sets the byte flags (+0x19..0x1B = 1) and frame float, then
// inits the +0x20 sub-struct (with a null caller arg).
void* __ct__UI_CPassiveSkillInfo(UI_CPassiveSkillInfo* self, u32 arg) {
    self->arg = arg;
    self->vptr = (void*)lbl_eu_80537A08;
    self->field_8 = 0;
    self->field_C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_19 = 1;
    self->field_1A = 1;
    self->field_1B = 1;
    self->field_1C = lbl_eu_80668904;
    func_802641D0(&self->sub, 0);
    return self;
}

// noinline: the CPassiveSkillLine dtor's member destruction must emit the
// retail `bl __dt__Q22UI17CPassiveSkillInfoFv` call, not inline this body.
__declspec(noinline) UI::CPassiveSkillInfo::~CPassiveSkillInfo() {}

void func_802646E8(){}

// Copy the 0x1C-byte init struct (6 words + 4 byte flags).
void func_80264AC8(UI_PassiveSkillInit* dst, const UI_PassiveSkillInit* src) {
    *dst = *src;
}

void func_80264B2C(){}

// Draw the skill-info cursor: always draw the main layout at +0x8; when both
// the +0x1B and +0x38 gate flags are set, also draw the +0x24 second layout.
void func_80264BE4(UI::CPassiveSkillInfo* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mpLayout != 0) {
        func_80137038(self->mpLayout, drawInfo, 0, 1);
    }
    if (self->field_1B != 0 && self->field_38 != 0) {
        func_80137038(self->field_24, drawInfo, 0, 1);
    }
}

// Release both cursor layouts: drop the primary layout at +0x8 through its
// vtable delete slot, then release the second-layout holder at +0x20 via the
// +0x4 release helper (func_80264344).
extern "C" void func_80264C58(UI::CPassiveSkillCur* self) {
    if (self->mpLayout != 0) {
        delete self->mpLayout;
        self->mpLayout = 0;
    }
    func_80264344(reinterpret_cast<UI_PassiveSkillLayoutRef*>(&self->_pad20));
}

// Cursor step: set the state byte to 1, clear the +0x19/+0x1A flags, then
// bind and enable the +0xC animation transform on the layout at +0x8.
void func_80264CB8(UI::CPassiveSkillCur* self) {
    self->field_18 = 1;
    self->field_19 = 0;
    self->field_1A = 0;
    self->mpLayout->BindAnimation(self->mpAnimTrans0);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, 1);
}

// Cursor step: same as func_80264CB8 but with state 4 and the +0x10
// animation transform.
void func_80264D28(UI::CPassiveSkillCur* self) {
    self->field_18 = 4;
    self->field_19 = 0;
    self->field_1A = 0;
    self->mpLayout->BindAnimation(self->mpAnimTrans1);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, 1);
}

// Cursor step: set the state byte to 6, clear the +0x19 flag, then bind and
// enable the +0x14 animation transform on the layout at +0x8.
void func_80264D98(UI_PassiveSkillStep* self) {
    self->field_18 = 6;
    self->field_19 = 0;
    self->mpLayout->BindAnimation(self->field_14);
    self->mpLayout->SetAnimationEnable(self->field_14, 1);
}

// Cursor step: same as func_80264D98 but with state 7. C linkage so callers
// (func_80269924 / func_80269A18) emit the plain retail reloc `func_80264E04`;
// noinline keeps the retail `bl` at those call sites.
extern "C" __declspec(noinline) void func_80264E04(UI_PassiveSkillStep* self) {
    self->field_18 = 7;
    self->field_19 = 0;
    self->mpLayout->BindAnimation(self->field_14);
    self->mpLayout->SetAnimationEnable(self->field_14, 1);
}

void func_80264E70(){}

void func_80264F7C(){}

// Skill-info update for the embedded CPassiveSkillInfo (retail func_802660EC).
// C linkage so callers (func_80269A98 / func_80269B94) emit the plain retail
// reloc; noinline keeps the retail `bl`.
extern "C" __declspec(noinline) void func_802660EC(UI::CPassiveSkillInfo* self) {}

// Set the cursor's skill-name pane text: look up message 135 from the shared
// string table, then push it into the layout pane named at pool+0x1a9.
// noinline: func_802698B8 keeps the retail `bl` to this 0x54-byte body.
__declspec(noinline) void func_802661A8(UI::CPassiveSkillCur* self) {
    char* text = func_80136190(&lbl_eu_8050DC20[0x196], &lbl_eu_8050DC20[0x1a4], 135);
    func_80136B4C(self->mpLayout, &lbl_eu_8050DC20[0x1a9], text, 0);
}

// Set the cursor's skill-name pane text using message 136 from the shared
// string table (variant of func_802661A8's 135 lookup). The two callee-saved
// registers (r30/r31) need the stmw/lmw frame retail uses, which -O4,p only
// emits under optimize_for_size.
#pragma optimize_for_size on
// extern "C": the retail symbol is the plain `func_802661FC` (the call reloc
// from func_802699A4 must match); noinline: func_802699A4 keeps the `bl`.
extern "C" __declspec(noinline) void func_802661FC(UI::CPassiveSkillCur* self) {
    char* text = func_80136190(&lbl_eu_8050DC20[0x196], &lbl_eu_8050DC20[0x1a4], 0x88);
    func_80136B4C(self->mpLayout, &lbl_eu_8050DC20[0x1a9], text, 0);
}
#pragma optimize_for_size off

// Drive the cursor's second layout root pane with a translation. Retail takes
// the translation by value and re-materializes it into a frame local before
// handing the address to the layout translate helper.
void func_80266250(UI::CPassiveSkillCur* self, nw4r::math::VEC3 pos) {
    nw4r::math::VEC3 local = pos;
    func_801D2150(self->field_24->GetRootPane(), &local);
}

void func_80266294(){}

// Cursor step: when the second anim transform's frame check succeeds, set the
// step state to 3, advance the layout animation, and unbind the transform.
void func_80266344(UI::CPassiveSkillCur* self) {
    if (func_80137444(self->mpAnimTrans1, lbl_eu_80668900) != 0) {
        self->field_18 = 3;
        self->field_19 = 1;
        self->mpLayout->Animate(0);
        self->mpLayout->UnbindAnimation(self->mpAnimTrans1);
    }
}

void func_802663C0(){}

// Cursor step: when the first anim transform's frame check succeeds, set the
// step state to 0, advance the layout animation, and unbind the transform.
void func_80266470(UI::CPassiveSkillCur* self) {
    if (func_80137510(self->mpAnimTrans0, lbl_eu_80668900) != 0) {
        self->field_18 = 0;
        self->field_19 = 1;
        self->mpLayout->Animate(0);
        self->mpLayout->UnbindAnimation(self->mpAnimTrans0);
    }
}

void func_802664EC(){}

void func_80266574(){}

void func_802665FC(){}

void func_80266724(){}

void func_80266950(UI_CPassiveSkill* self, u8 index);

void func_80266930(UI_CPassiveSkill* self, u8 row, u8 col, u8 slot) {
    // Linearize a 5x5 skill grid into a slot index: (row-1)*25 + col*5 + slot + 1.
    return func_80266950(self, (u8)(col * 5 + slot + (row - 1) * 25 + 1));
}

void func_80266950(UI_CPassiveSkill* self, u8 index) {}

// Line sub-object constructor (retail symbol __ct__UI_CPassiveSkillLine).
// C linkage so __ct__UI_CPassiveSkill's `bl` binds to the plain name; noinline
// keeps the retail `bl` (the real body is not yet matched).
extern "C" __declspec(noinline) void __ct__UI_CPassiveSkillLine(UI::CPassiveSkillLine* self, nw4r::lyt::ArcResourceAccessor* accessor) {}

// CPassiveSkillLine destructor (retail __dt__Q22UI17CPassiveSkillLineFv): the
// body is empty, so MWCC auto-generates the member destruction (CCur18 at
// +0x178 first, then the CPassiveSkillInfo at +0x120, both with the -1
// embedded-member flag), the this-null guard and the flags>0 operator-delete
// tail. The r30/r31 stmw/lmw frame needs optimize_for_size.
#pragma optimize_for_size on
// noinline: the CPassiveSkill dtor's member destruction must emit the retail
// `bl __dt__Q22UI17CPassiveSkillLineFv` call, not inline this body.
__declspec(noinline) UI::CPassiveSkillLine::~CPassiveSkillLine() {}
#pragma optimize_for_size off

void func_80266B78(){}

void func_80267268(){}

// Copy a cursor object's state (fields +0x4..+0x16, skipping the vtable at +0x0).
void func_802672F0(UI::CPassiveSkillCur* dst, const UI::CPassiveSkillCur* src) {
    dst->mArcResAcc = src->mArcResAcc;
    dst->mpLayout = src->mpLayout;
    dst->mpAnimTrans0 = src->mpAnimTrans0;
    dst->mpAnimTrans1 = src->mpAnimTrans1;
    dst->mActive = src->mActive;
    dst->mVisible = src->mVisible;
    dst->field_16 = src->field_16;
}

// Copy the CBaseCur-sized portion of a cursor (fields +0x4..+0x15).
void func_8026732C(UI::CPassiveSkillCur* dst, const UI::CPassiveSkillCur* src) {
    dst->mArcResAcc = src->mArcResAcc;
    dst->mpLayout = src->mpLayout;
    dst->mpAnimTrans0 = src->mpAnimTrans0;
    dst->mpAnimTrans1 = src->mpAnimTrans1;
    dst->mActive = src->mActive;
    dst->mVisible = src->mVisible;
}

void func_80267360(u8* self){}

void func_80267484(u8* self, u8* arg2) {}

void func_802675D8(){}

// Retail symbol is the unmangled `func_802676DC`; C linkage so call sites
// (func_8026D9F0) emit the plain reloc name instead of a C++ mangling, and
// noinline so the 0x1C-byte body is not inlined into them (retail keeps the
// `bl` — the callee lives in a separate retail TU).
extern "C" __declspec(noinline) u32 func_802676DC(UI_CPassiveSkill* self) {
    if (self->field_139 != 0) {
        return self->field_E9;
    }
    return 0;
}

// +0x28 sub-object update (retail func_80269B68, 0x410 bytes). Stub kept
// noinline so func_8026DA4C's tail call stays a `b` instead of being inlined.
// C linkage so both the definition symbol and call relocs are the plain
// retail name `func_802676F8`.
extern "C" __declspec(noinline) void func_802676F8(u8* self) {}

// +0x28 sub-object update used by the menu-close path (retail func_80267B08).
// C linkage so the call reloc from func_8026DA88 is the plain retail name.
extern "C" __declspec(noinline) void func_80267B08(u8* self) {}

void func_80267BA0(void* self){}

extern "C" __declspec(noinline) void func_80267C44(u8* self){}

void func_80267CE0(void* self){}

extern "C" __declspec(noinline) void func_80267E2C(u8* self){}

void func_80267F88(void* self){}

extern "C" __declspec(noinline) void func_80268250(u8* self){}

// Menu-close dispatch for the +0x28 line sub-object: when the embedded syswin
// is armed (non-null and active) the caller already handled the close, so
// return early; otherwise pick the state-5 (func_80269808) or state-0xC
// (func_80269924) close path and play the shared UI SFX 109 on either.
void func_80268518(UI::CPassiveSkillLine* self) {
    CSysWin* syswin = self->mInfo.field_54;
    if (syswin != 0 && CSysWin_getUnk34(syswin) != 0) {
        return;
    }
    if (self->field_E8 == 5) {
        func_80269808(self);
        func_80138078(0x6d);
    } else if (self->field_E8 == 0xC) {
        func_80269924(self);
        func_80138078(0x6d);
    }
}

extern "C" __declspec(noinline) void func_80268594(u8* self) {}

// +0x28 sub-object update used by func_8026DAD0's learned-skill path. noinline:
// retail keeps the `bl` (the body lives at 0x80268BA8).
__declspec(noinline) void func_80268BA8(u8* self){}

void func_80268C38(void* self){}

// +0x28 sub-object update used by func_8026DAD0's fallback path: when the
// lazily-attached CSysWin (mInfo.field_54) is armed and active, play the open
// SFX, quiet the embedded cursor, and advance the system window. noinline:
// retail keeps the `bl` (the body lives at 0x80268F7C).
__declspec(noinline) void func_80268F7C(UI::CPassiveSkillLine* self) {
    CSysWin* syswin = self->mInfo.field_54;
    if (syswin == 0) {
        return;
    }
    if (CSysWin_getUnk34(syswin) == 0) {
        return;
    }
    if (CSysWin_isActive(syswin) == 0) {
        return;
    }
    func_80138078(6);
    func_801D216C(&self->mCur, 0);
    func_8022B8E4(syswin);
}

void func_80268FEC(void* self){
    if (*(u8*)((u8*)self + 0x13A) != 0) {
        *(u8*)((u8*)self + 0xE8) = 2;
    }
}

void func_80269004(){}

void func_8026916C(){}

void func_80269200(UI_CPassiveSkill* self) {
    if (self->field_139 == 0) {
        return;
    }
    self->field_E8 = 0;
    self->field_E9 = 1;
}

void func_80269220(){}

void func_802692E0(){}

void func_80269370(){}

void func_80269410(){}

void func_802694F4(){}

void func_802695A8(){}

void func_80269638(){}

void func_802696D8(){}

void func_80269768(){}

// Menu-close state-5 helper (called by func_80268518). C linkage so the call
// reloc is the plain retail `func_80269808`; noinline keeps the retail `bl`
// (the real body lives at 0x80269808, not yet matched).
extern "C" __declspec(noinline) void func_80269808(UI::CPassiveSkillLine* self) {}

// Skill-menu state step: set the +0xE8 menu-state byte to 4, bind and enable
// the +0x10 animation transform on the +0x8 layout, then refresh the skill
// name text on the embedded cursor at +0x120 (layout-compatible with
// CPassiveSkillCur: Layout* at +0x8).
void func_802698B8(UI::CPassiveSkillLine* self) {
    self->field_E8 = 4;
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_802661A8(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
}

// Skill-menu state step: set the +0xE8 menu-state byte to 3, clear +0xE9,
// bind/enable the +0x10 animation transform on the +0x8 layout, then step the
// embedded skill-info cursor at +0x120 (UI_PassiveSkillStep view) and push
// 0xff into the +0x3C info region via func_802640B8. C linkage so
// func_80268518's `bl func_80269924` binds to the plain retail name; noinline
// keeps that `bl` (MWCC would otherwise inline this body into func_80268518).
extern "C" __declspec(noinline) void func_80269924(UI::CPassiveSkillLine* self) {
    self->field_E8 = 3;
    self->field_E9 = 0;
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_80264E04(reinterpret_cast<UI_PassiveSkillStep*>(&self->mInfo));
    func_802640B8(&self->mInfo.field_3C[0], 0xff);
}

// Line state step: set the +0xE8 menu-state byte to 9, bind and enable the
// +0x1C animation transform on the +0x18 layout, refresh the skill-name pane
// on the embedded cursor at +0x120, then run the line update tail.
void func_802699A4(UI::CPassiveSkillLine* self) {
    self->field_E8 = 9;
    self->field_18->BindAnimation(self->field_1C);
    self->field_18->SetAnimationEnable(self->field_1C, 1);
    func_802661FC(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
    func_8026BB60(self);
}

// Skill-menu state step: same shape as func_80269924 but with menu state 0xD.
void func_80269A18(UI::CPassiveSkillLine* self) {
    self->field_E8 = 0xD;
    self->field_E9 = 0;
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_80264E04(reinterpret_cast<UI_PassiveSkillStep*>(&self->mInfo));
    func_802640B8(&self->mInfo.field_3C[0], 0xff);
}

// Skill-menu state step: set the +0xE8 menu-state byte to 0xB, bind/enable
// the +0x1C animation transform on the +0x18 layout, then run the skill-info
// update, the state-0xB helper and the line update tail.
void func_80269A98(UI::CPassiveSkillLine* self) {
    self->field_E8 = 0xB;
    self->field_18->BindAnimation(self->field_1C);
    self->field_18->SetAnimationEnable(self->field_1C, 1);
    func_802660EC(&self->mInfo);
    func_80269D20(self);
    func_8026BB60(self);
}

void func_80269B14(){}

// Skill-menu state step: set the +0xE8 menu-state byte to 14, bind and enable
// the +0x10 animation transform on the +0x8 layout, then run the skill-info
// update and the line update tail.
void func_80269B94(UI::CPassiveSkillLine* self) {
    self->field_E8 = 14;
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_802660EC(&self->mInfo);
    func_8026AAF4(self);
}

void func_80269C08(){}

// State-0xB line helper (called by func_80269A98). C linkage so the call reloc
// is the plain retail `func_80269D20`; noinline keeps the retail `bl` (the
// real body lives at 0x80269D20, not yet matched).
extern "C" __declspec(noinline) void func_80269D20(UI::CPassiveSkillLine* self) {}

// Line update tail (retail func_8026AAF4). noinline: func_80269B94 keeps the
// retail `bl` (the body lives at 0x8026AAF4).
__declspec(noinline) void func_8026AAF4(UI::CPassiveSkillLine* self) {}

extern "C" __declspec(noinline) void func_8026BB60(UI::CPassiveSkillLine* self){}

void func_8026C4A4(){}

u8 func_8026CC34(UI_CPassiveSkill* self) {
    s8 row = self->field_F7;
    s8 col = self->field_F8;
    return self->field_104[row * 5 + col];
}

void func_8026CC58(){}

void func_8026CD44(){}

void func_8026CE30(){}

void func_8026D080(){}

void func_8026D210(){}

void func_8026D3CC(){}

// UI::CPassiveSkill constructor (retail symbol __ct__UI_CPassiveSkill, plain
// name; MWCC passes __-prefixed global names through unmangled). Stores the
// vtable, constructs the embedded mem-region class, zero/one-inits the state
// fields, then constructs the +0x28 line and +0x1B8 system window sub-objects.
void* __ct__UI_CPassiveSkill(UI::CPassiveSkill* self) {
    self->mVtbl = (void*)lbl_eu_80537968;
    __ct__17UnkClass_8045F564Fv(&self->mUnk8);
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_20 = 0;
    self->field_24 = 0;
    self->field_25 = 0;
    self->field_26 = 0;
    self->field_27 = 1;
    __ct__UI_CPassiveSkillLine(&self->mLine, 0);
    __ct__CSysWin(&self->mSysWin, 0);
    return self;
}

// UI::CPassiveSkill destructor: empty body; MWCC auto-generates the member
// destruction (CSysWin at +0x1B8, CPassiveSkillLine at +0x28, UnkClass at
// +0x8, in reverse declaration order) plus the this-null guard and the
// flags>0 operator-delete tail. The r30/r31 stmw/lmw frame needs
// optimize_for_size (same as the CPassiveSkillLine dtor).
#pragma optimize_for_size on
UI::CPassiveSkill::~CPassiveSkill() {}
#pragma optimize_for_size off

void func_8026D5A8(){}

void func_8026D894(UI_CPassiveSkill* self) {
    // Visibility-gated update: while the menu is open (both gates set), drive
    // the +0x28 sub-object update and, depending on the window state byte
    // (1 = normal, 3 = closing), run the matching +0x28 state transition first.
    if (self->field_24 == 0 || self->field_25 == 0) {
        return;
    }
    if (self->field_25 == 1) {
        func_8026DCF4(self);
    } else if (self->field_25 == 3) {
        func_8026DD3C(self);
    }
    func_80267360(self->_pad28);
}

void func_8026D8FC(UI_CPassiveSkill* self, u8* arg2) {
    if (self->field_24 == 0) {
        return;
    }
    if (self->field_25 == 0) {
        return;
    }
    // Tail-call the +0x28 sub-object's update, forwarding arg2 (DrawInfo-like).
    return func_80267484(self->_pad28, arg2);
}

void func_8026D920(){}

// Return the +0x26 visibility byte once the +0x1B8 system window is ready.
u8 func_8026D9AC(UI_CPassiveSkill* self) {
    if (CSysWin_isReady(self->field_1B8) != 0) {
        return self->field_26;
    }
    return 0;
}

// Return the +0x27 byte once the +0x28 sub-object reports active.
u8 func_8026D9F0(UI_CPassiveSkill* self) {
    if (func_802676DC(reinterpret_cast<UI_CPassiveSkill*>(self->_pad28)) != 0) {
        return self->field_27;
    }
    return 0;
}

u32 func_8026DA34(u8* self) {
    // returns 1 when the byte at +0x110 is >= 8 (retail subfc/subfze carry idiom)
    return (u32)self[0x110] >= 8;
}

// Open the skill menu: lazily attach the +0x1B8 CSysWin sub-object pointer at
// +0x19C, mark visible, then tail-call the +0x28 sub-object's update.
void func_8026DA4C(UI_CPassiveSkill* self) {
    if (self->field_25 != 0) {
        return;
    }
    if (self->field_19C == 0) {
        self->field_19C = self->field_1B8;
    }
    self->field_25 = 1;
    self->field_27 = 0;
    func_802676F8(self->_pad28);
}

// Close the skill menu: when the visibility gate is in state 2, bump it to 3,
// clear the +0x27 flag, run the +0x28 sub-object update, and play UI SFX 6.
void func_8026DA88(UI_CPassiveSkill* self) {
    if (self->field_25 == 2) {
        self->field_25 = 3;
        self->field_27 = 0;
        func_80267B08(self->_pad28);
        func_80138078(6);
    }
}

void func_8026DAD0(UI_CPassiveSkill* self) {
    // Menu-open helper: a learned skill cell (marker 15) updates the +0x28
    // sub-object directly; otherwise the CSysWin window state picks between
    // the two update paths.
    if (self->field_104[12] == 15) {
        func_80268BA8(self->_pad28);
    } else if (CSysWin_getUnk34(self->field_1B8) != 0) {
        func_80268F7C(reinterpret_cast<UI::CPassiveSkillLine*>(self->_pad28));
    } else {
        func_80268518(reinterpret_cast<UI::CPassiveSkillLine*>(self->_pad28));
    }
}


void UI_CPassiveSkill::thunk28_67CE0() { ((void(*)(void*))func_80267CE0)((char*)this + 0x28); }

void UI_CPassiveSkill_thunk28_67E2C(u8* self) { func_80267E2C(self + 0x28); }

void UI_CPassiveSkill::thunk28_67F88() { ((void(*)(void*))func_80267F88)((char*)this + 0x28); }

void UI_CPassiveSkill_thunk28_68250(u8* self) { func_80268250(self + 0x28); }

void UI_CPassiveSkill::thunk28_67BA0() { ((void(*)(void*))func_80267BA0)((char*)this + 0x28); }

void UI_CPassiveSkill_thunk28_67C44(u8* self) { func_80267C44(self + 0x28); }

void UI_CPassiveSkill::thunk28_68518() { ((void(*)(void*))func_80268518)((char*)this + 0x28); }

void func_8026DB74(){}

extern "C" void UI_CPassiveSkill_setByte180(u8* self, u8 val) { *(u8*)(self + 0x180) = val; }

void UI_CPassiveSkill::thunk28_68C38() { ((void(*)(void*))func_80268C38)((char*)this + 0x28); }

s32 func_8026DCA0(UI_CPassiveSkill* self) {
    // Once the +0x1B8 CSysWin reports ready the menu is considered open.
    if (UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34(self) != 0) {
        return 1;
    }
    // Otherwise report whether the skill cell at +0x110 holds the "learned" marker (15).
    return (self->field_104[12] == 15) ? 1 : 0;
}

extern "C" __declspec(noinline) int UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34(UI_CPassiveSkill* self) { return CSysWin_getUnk34(self->field_1B8); }

void func_8026DCF4(UI_CPassiveSkill* self) {
    if ((s32)func_802676DC(reinterpret_cast<UI_CPassiveSkill*>(self->_pad28)) != 0) {
        self->field_25 = 2;
        self->field_27 = 1;
    }
}

void func_8026DD3C(UI_CPassiveSkill* self) {
    if ((s32)func_802676DC(reinterpret_cast<UI_CPassiveSkill*>(self->_pad28)) != 0) {
        self->field_25 = 0;
        self->field_27 = 1;
    }
}

void func_8026DD84(){}

void CPassiveSkill::OnFileEvent() {}

extern "C" void func_80263F30() {}
// Cursor animation step: skip when no layout; for mActive 0 rewind the first
// anim transform, for mActive 1 run the func_80264140 step, then advance the
// layout animation.
void func_80263FE8(UI::CPassiveSkillCur* self) {
    if (self->mpLayout == 0) {
        return;
    }
    if (self->mActive == 0) {
        func_80137444(self->mpAnimTrans0, lbl_eu_80668900);
    } else if (self->mActive == 1) {
        func_80264140();
    }
    self->mpLayout->Animate(0);
}
// Release the cursor's primary layout: drop it through the layout vtable's
// delete slot (index 0 at vtable+0x8 with flag 1) and null the pointer.
extern "C" void func_80264060(UI::CPassiveSkillCur* self) {
    if (self->mpLayout != 0) {
        delete self->mpLayout;
        self->mpLayout = 0;
    }
}

// Pane/region helper (retail func_802640B8, not yet matched). noinline keeps
// the retail `bl` from func_80269924 / func_80269A18.
extern "C" __declspec(noinline) void func_802640B8(u8* self, u8 arg) {}
// Cursor step helper (retail func_80264140). noinline: func_80263FE8 keeps the
// retail `bl` (the body lives at 0x80264140).
__declspec(noinline) void func_80264140() {}
// Init a 0x1C-byte structure: caller pointer at +0x0, zeroed words, byte flags
// with the active flag (+0x19) starting at 1. noinline: the skill-info ctor
// keeps the retail `bl` (MWCC would otherwise inline this body).
__declspec(noinline) void func_802641D0(UI_PassiveSkillInit* self, u32 arg) {
    self->field_0 = arg;
    self->field_4 = 0;
    self->field_8 = 0;
    self->field_C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_19 = 1;
    self->field_1A = 0;
    self->field_1B = 0;
}
extern "C" void func_80264204() {}
// Release the layout pointer at +0x4 of the second-layout holder (aliases
// CPassiveSkillCur.field_24 when called on the +0x20 region). noinline: retail
// keeps the `bl` from func_80264C58.
extern "C" __declspec(noinline) void func_80264344(UI_PassiveSkillLayoutRef* self) {
    if (self->field_4 != 0) {
        delete self->field_4;
        self->field_4 = 0;
    }
}

extern "C" void func_8026DB34() {}
extern "C" void func_8026DB44() {}
extern "C" void func_8026DB54() {}
extern "C" void func_8026DB64() {}
