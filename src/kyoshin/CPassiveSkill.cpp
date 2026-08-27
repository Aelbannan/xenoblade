// Auto-scaffolded catalog TU for kyoshin/CPassiveSkill
// Replace stubs with high-level C/C++ during decomp.

// func_80137924: CSysWin.hpp declares the sret spelling (void(VEC3*,...));
// this TU needs the value-returning spelling (VEC3(Pane*,Pane*,Pane*)) so
// the nested-call form allocates the sret buffer and the by-value argument
// copy in retail's stack-slot order. Same PPC ABI (hidden pointer in r3).
// The value view lives in its own namespace (extern "C" keeps the retail
// symbol) so it cannot collide with CSysWin.hpp's pointer-form decl - same
// pattern as CMenuArtsSet.cpp's artsFAD0View.
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPassiveSkill.hpp"
#include "kyoshin/CSysWin.hpp"
namespace passiveSkillVecView {
extern "C" nw4r::math::VEC3
    func_80137924(nw4r::lyt::Pane* paneA, nw4r::lyt::Pane* paneB,
                  nw4r::lyt::Pane* root);
} // namespace passiveSkillVecView
#include "kyoshin/code_80135FDC.hpp"

// Value-returning spelling of func_80137924 (passiveSkillVecView above);
// ABI-identical to the sret form.

// Named .sdata2 conversion magic: defining it lets MWCC's constant pool reuse
// the retail symbol for the (f32)u16 casts in func_802646E8 instead of
// emitting a TU-local @N label (CfResReloadImpl / CSuddenCommu idiom, same as
// CMiniMap.cpp's 806670A8). Value is 2^52 = 0x4330000000000000.
extern const double lbl_eu_80668910 = 4503599627370496.0; // exact bits 0x4330000000000000 (2^52); an integer literal like 0x4330000000000000ll gets VALUE-converted to double and lands as 0x43d0cc0000000000, which defeats pool merging

// Per-character slot-spacing table read by func_8026BB60 (retail .sdata2,
// read at (id-1)*2 with an 8-byte per-character stride; 2 elements keep it
// in .sdata2 so the retail sda21 reloc is emitted). Values unresolved
// (placeholder).
extern const f32 lbl_eu_80668930[] = { 0.0f, 0.0f };

// Skill-info sub-struct initializer (defined later in this TU; noinline keeps
// the __ct__UI_CPassiveSkillInfo `bl` a real call). C linkage so the call
// reloc is the plain retail name.
extern "C" void func_802641D0(UI_PassiveSkillInit* self, u32 arg);
// Skill-info sub-struct copier / second-layout init (defined later in this
// TU). C linkage so the call relocs from func_802646E8 are the plain retail
// names.
extern "C" void func_80264AC8(UI_PassiveSkillInit* dst, const UI_PassiveSkillInit* src);
extern "C" void func_80264204(UI_PassiveSkillLayoutInit* self);

// Line sub-object constructors (defined later in this TU). C linkage so the
// call relocs from __ct__UI_CPassiveSkillLine are the plain retail names
// (MWCC would otherwise mangle the C++ references to them).
extern "C" void* __ct__UI_CPassiveSkillInfo(UI_CPassiveSkillInfo* self, u32 arg);
extern "C" void* __ct__UI_CPassiveSkillCur(UI::CPassiveSkillCur* _this, nw4r::lyt::ArcResourceAccessor* accessor);

// Skill-info update (defined later in this TU; C linkage so callers emit the
// plain retail reloc, noinline keeps the retail `bl`).
extern "C" void func_802660EC(UI::CPassiveSkillInfo* self);

// Line update tail (defined later in this TU; noinline keeps the retail `bl`).
// C linkage so the call relocs from func_80269B94 / func_802694F4 are the
// plain retail name.
extern "C" void func_8026AAF4(UI::CPassiveSkillLine* self);

// State-0xB line step (defined later in this TU). C linkage so the call reloc
// from func_80269638 is the plain retail name.
extern "C" void func_80269A98(UI::CPassiveSkillLine* self);

// Cursor step helper (defined later in this TU; noinline keeps the retail `bl`).
extern "C" void func_80264140(UI::CPassiveSkillCur* self);

void func_80267BA0(UI::CPassiveSkillLine* self);
extern "C" __declspec(noinline) void func_80267C44(UI::CPassiveSkillLine* self);
void func_80267CE0(UI::CPassiveSkillLine* self);
extern "C" __declspec(noinline) void func_80267E2C(u8* self);
void func_80267F88(UI::CPassiveSkillLine* self);
__declspec(noinline) void func_80268250(UI::CPassiveSkillLine* self);
__declspec(noinline) void func_80268518(UI::CPassiveSkillLine* self);
extern "C" __declspec(noinline) void func_80268594(u8* self);
extern "C" __declspec(noinline) void func_80268C38(void* self);

// State-step helpers defined later in this TU. C linkage so the call relocs
// are the plain retail names (func_80269808 / func_80269924 / func_80269D20).
// func_802640B8 is a plain global (MWCC does not mangle global-scope names).
extern "C" void func_80269808(UI::CPassiveSkillLine* self);
extern "C" void func_80269924(UI::CPassiveSkillLine* self);
extern "C" void func_80269D20(UI::CPassiveSkillLine* self);
extern "C" void func_802640B8(UI::UI_PassiveSkillRegion3C* self, u8 arg);

// Cursor/skill-info step forward declarations (defined later in this TU). C
// linkage so the call relocs from func_80264B2C are the plain retail names
// (func_80266294 etc.), noinline at the definitions keeps the retail `bl`s.
extern "C" void func_80266294(UI::CPassiveSkillCur* self);
extern "C" void func_80266344(UI::CPassiveSkillCur* self);
extern "C" void func_802663C0(UI::CPassiveSkillCur* self);
extern "C" void func_80266470(UI::CPassiveSkillCur* self);
extern "C" void func_802664EC(UI_CPassiveSkillInfo* self);
extern "C" void func_80266574(UI_CPassiveSkillInfo* self);
extern "C" void func_8026439C(UI_PassiveSkillInit* self);
// Cursor/skill-menu step helpers called by the menu-close targets below. C
// linkage so the call relocs are the plain retail names; the plain-C++
// definitions later in this TU inherit the linkage. noinline is applied at
// the definitions so retail keeps the `bl` at these call sites.
extern "C" void func_80264D28(UI::CPassiveSkillCur* self);
extern "C" void func_802698B8(UI::CPassiveSkillLine* self);
extern "C" void func_802699A4(UI::CPassiveSkillLine* self);
extern "C" void func_80269B94(UI::CPassiveSkillLine* self);
extern "C" void func_80269C08(UI::CPassiveSkillLine* self);
extern "C" void func_8026C4A4(UI::CPassiveSkillLine* self);
extern "C" void func_8026D080(UI::CPassiveSkillLine* self);
extern "C" void func_8026D210(UI::CPassiveSkillLine* self);

// +0x28 sub-object updates (defined later in this TU; noinline keeps the
extern "C" __declspec(noinline) void func_80268BA8(u8* self);
extern "C" __declspec(noinline) void func_80269B14(UI::CPassiveSkillLine* self);
extern "C" __declspec(noinline) void func_80268F7C(UI::CPassiveSkillLine* self);
// +0x28 sub-object update tail (retail func_8026BB60, 0x944 bytes; stub kept
// noinline so func_802699A4 keeps the retail `bl`). C linkage so the call
// reloc is the plain retail name.
extern "C" __declspec(noinline) void func_8026BB60(UI::CPassiveSkillLine* self);
extern "C" __declspec(noinline) void func_8026DCF4(UI_CPassiveSkill* self);
extern "C" __declspec(noinline) void func_8026DD3C(UI_CPassiveSkill* self);

// this-adjusting thunk: forward to the sub-object at +0x28 (retail `UI_CPassiveSkill_thunk28_68594`).
void UI_CPassiveSkill_thunk28_68594(u8* self) { func_80268594(self + 0x28); }


// Cursor constructor: installs the vtable and inits the CBaseCur-compatible
// layout (visible=1, everything else 0). Written as a global function so the
// emitted symbol is exactly the retail `__ct__UI_CPassiveSkillCur` (MWCC does
// not mangle global-scope names). noinline: the line ctor's member
// construction must emit the retail `bl __ct__UI_CPassiveSkillCur`.
__declspec(noinline) void* __ct__UI_CPassiveSkillCur(UI::CPassiveSkillCur* _this, nw4r::lyt::ArcResourceAccessor* accessor) {
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
// noinline: func_80264B2C keeps the retail `bl` to this 0x70-byte body.
__declspec(noinline) void func_8026439C(UI_PassiveSkillInit* self) {
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

// Animation trigger: when the init flag is 1, rewind the anim transform,
// mark the sub-state active and show the skill pane. C linkage so the call
// reloc from func_80267BA0 is the plain retail name; noinline keeps the
// retail `bl`.
extern "C" __declspec(noinline) void func_8026440C(UI_PassiveSkillInit* self) {
    // Animation trigger: when the init flag is 1, rewind the anim transform,
    // mark the sub-state active and show the skill pane.
    if (self->field_1B == 1) {
        self->field_C->SetFrame(lbl_eu_80668904);
        self->field_1A = 1;
        func_80124270(self->field_14, 1);
    } else {
        self->field_19 = 0;
        self->field_1A = 1;
        func_80124270(self->field_14, 0);
    }
}

// Same trigger as func_8026440C but drives the other pane (+0x10).
// noinline: func_80267C44 keeps the retail `bl`.
__declspec(noinline) void func_80264470(UI_PassiveSkillInit* self) {
    // Same trigger as func_8026440C but drives the other pane (+0x10).
    if (self->field_1B == 1) {
        self->field_C->SetFrame(lbl_eu_80668904);
        self->field_1A = 0;
        func_80124270(self->field_10, 1);
    } else {
        self->field_19 = 0;
        self->field_1A = 0;
        func_80124270(self->field_10, 0);
    }
}

// Skill-info pane step (mode-0 path of func_8026439C): while the +0x19 active
// flag is clear, run the frame-check on the +0x8 anim transform (result
// discarded), then disable the +0x8 transform, enable and rewind the +0xC one,
// set the mode byte, and show the +0x14/+0x10 pane depending on the +0x1A flag.
__declspec(noinline) void func_802644D4(UI_PassiveSkillInit* self) {
    advanceAnimTransform(self->field_8, lbl_eu_80668900);
    if (self->field_19 != 0) {
        return;
    }
    self->field_4->SetAnimationEnable(self->field_8, 0);
    self->field_4->SetAnimationEnable(self->field_C, 1);
    self->field_C->SetFrame(lbl_eu_80668904);
    self->field_1B = 1;
    if (self->field_1A != 0) {
        func_80124270(self->field_14, 1);
    } else {
        func_80124270(self->field_10, 1);
    }
}

// Skill-info pane step (mode-1 path of func_8026439C): when the +0xC anim
// transform's frame check succeeds, disable it, enable and rewind the +0x8
// transform, clear the mode byte, set the active flag, then hide the
// +0x10/+0x14 panes.
__declspec(noinline) void func_80264588(UI_PassiveSkillInit* self) {
    if (advanceAnimTransform(self->field_C, lbl_eu_80668900) != 0) {
        self->field_4->SetAnimationEnable(self->field_C, 0);
        self->field_4->SetAnimationEnable(self->field_8, 1);
        self->field_8->SetFrame(lbl_eu_80668904);
        self->field_1B = 0;
        self->field_19 = 1;
        func_80124270(self->field_10, 0);
        func_80124270(self->field_14, 0);
    }
}

// Skill-info constructor: installs the vtable, stores the caller arg, zeroes
// the word fields, sets the byte flags (+0x19..0x1B = 1) and frame float, then
// inits the +0x20 sub-struct (with a null caller arg). noinline: the line
// ctor's member construction must emit the retail `bl __ct__UI_CPassiveSkillInfo`.
__declspec(noinline) void* __ct__UI_CPassiveSkillInfo(UI_CPassiveSkillInfo* self, u32 arg) {
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

// u16 -> f32 via the 2^52 magic double: the union stores the value + 0x4330
// prefix and subtracts the named .sdata2 magic so the pool reloc stays
// lbl_eu_80668910 (a plain `(f32)u16` cast would synthesize an @N entry).
// Inlined into func_802646E8 so the conversion doubles land in the temp
// area (sp+0x30/0x38), matching the retail frame.

// Skill-info init (retail func_802646E8): create the main layout, bind the
// three animation transforms, attach the font, push the name text into the
// six skill panes, configure the animation states, build the +0x20 second
// layout sub-struct, then set the skill-name pane flag and text, attach the
// character name texture and size its pane from the texture dimensions, and
// hide the party-count panes when the party has at most one member. The
// r29-r31 _savegpr_29 frame retail uses needs optimize_for_size.
#pragma optimize_for_size on
void func_802646E8(UI_CPassiveSkillInfo* self) {
    buildLayout(&self->field_8,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->arg),
                  &lbl_eu_8050DC20[0xc7]);
    bindLayoutAnimTransform(self->field_8,
                  reinterpret_cast<nw4r::lyt::AnimTransform**>(&self->field_C),
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->arg),
                  &lbl_eu_8050DC20[0xdc]);
    bindLayoutAnimTransform(self->field_8,
                  reinterpret_cast<nw4r::lyt::AnimTransform**>(&self->field_10),
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->arg),
                  &lbl_eu_8050DC20[0xf4]);
    bindLayoutAnimTransform(self->field_8, &self->field_14,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->arg),
                  &lbl_eu_8050DC20[0x111]);
    // Bind the font handle into the layout's root pane.
    nw4r::lyt::Pane* rootPane = self->field_8->GetRootPane();
    CDeviceFontView* font = reinterpret_cast<CDeviceFontView*>(getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->field_8));
    func_8013676C(rootPane, font->vf7());
    u32 textVal = func_801355BC();
    if (textVal != 0) {
        setLayoutTextBoxFont(self->field_8, &lbl_eu_8050DC20[0x12d], textVal);
        setLayoutTextBoxFont(self->field_8, &lbl_eu_8050DC20[0x139], textVal);
        setLayoutTextBoxFont(self->field_8, &lbl_eu_8050DC20[0x145], textVal);
        setLayoutTextBoxFont(self->field_8, &lbl_eu_8050DC20[0x154], textVal);
        setLayoutTextBoxFont(self->field_8, &lbl_eu_8050DC20[0x160], textVal);
        setLayoutTextBoxFont(self->field_8, &lbl_eu_8050DC20[0x16c], textVal);
    }
    self->field_8->SetAnimationEnable(
        reinterpret_cast<nw4r::lyt::AnimTransform*>(self->field_10), 0);
    self->field_8->SetAnimationEnable(self->field_14, 0);
    self->field_8->SetAnimationEnable(
        reinterpret_cast<nw4r::lyt::AnimTransform*>(self->field_C), 1);
    self->field_8->Animate(0);
    self->field_8->UnbindAllAnimation();
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x178],
                  &lbl_eu_8050DC20[0x182], 0);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x183],
                  &lbl_eu_8050DC20[0x182], 0);
    float out[2];
    UI_PassiveSkillInit init;
    func_802641D0(&init, self->arg);
    func_80264AC8(&self->sub, &init);
    func_80264204(reinterpret_cast<UI_PassiveSkillLayoutInit*>(&self->sub));
    u8 flag = 0;
    u32 isZero = (func_8009CF8C(0x3372) == 0);
    if (isZero) {
        // flag stays 0
    } else {
        u8 tmp = 0;
        if (func_8009CF8C(0x3508) != 0 && func_8009CF8C(0x20) < 0x38) {
            tmp = 1;
        }
        if (tmp == 0) {
            flag = 1;
        }
    }
    nw4r::lyt::Pane* pane = self->field_8->GetRootPane()->FindPaneByName(
        &lbl_eu_8050DC20[0x18d], true);
    func_80124270(pane, flag);
    char* text = func_80136190(&lbl_eu_8050DC20[0x196], &lbl_eu_8050DC20[0x1a4], 0x87);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x1a9], text, 0);
    const char* sel = isClassicController__Q22cf13CfGameManagerFv(-1) != 0
                          ? &lbl_eu_8050DC20[0x1b4]
                          : &lbl_eu_8050DC20[0x1bd];
    u16 msgId = func_8013606C(&lbl_eu_8050DC20[0x196], sel, 0x87);
    char* texName = func_80138F78(msgId);
    void* res = func_801355F4()->GetResource(0x74696d67, texName, 0);
    if (res != 0) {
        func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x1c6], res);
        // Hoist the texture dimension reads ahead of the pane lookup so they
        // stay in the saved registers across the virtual call (retail keeps
        // c2 in r29 / c0 in r30 during the FindPaneByName dispatch).
        CPSkillTexCoords* coords =
            reinterpret_cast<CPSkillTexRes*>(res)->chain->pCoords;
        u16 c2 = coords->c2;
        u16 c0 = coords->c0;
        nw4r::lyt::Pane* pane2 = self->field_8->GetRootPane()->FindPaneByName(
            &lbl_eu_8050DC20[0x1c6], true);
        if (pane2 != 0) {
            out[0] = (f32)c2;
            out[1] = (f32)c0;
            func_80124288(pane2, out);
        }
    }
    if ((u8)code80135FDC_getByte_64077() <= 1) {
        nw4r::lyt::Pane* paneA = self->field_8->GetRootPane()->FindPaneByName(
            &lbl_eu_8050DC20[0x1d2], true);
        func_80124270(paneA, 0);
        nw4r::lyt::Pane* paneB = self->field_8->GetRootPane()->FindPaneByName(
            &lbl_eu_8050DC20[0x1dd], true);
        func_80124270(paneB, 0);
    }
}
#pragma optimize_for_size off

// Copy the 0x1C-byte init struct (6 words + 4 byte flags). noinline: retail
// keeps the `bl func_80264AC8` call from func_802646E8 / func_80267268.
__declspec(noinline) void func_80264AC8(UI_PassiveSkillInit* dst, const UI_PassiveSkillInit* src) {
    dst->field_0 = src->field_0;
    dst->field_4 = src->field_4;
    dst->field_8 = src->field_8;
    dst->field_C = src->field_C;
    dst->field_10 = src->field_10;
    dst->field_14 = src->field_14;
    dst->field_18 = src->field_18;
    dst->field_19 = src->field_19;
    dst->field_1A = src->field_1A;
    dst->field_1B = src->field_1B;
}

// Cursor animation dispatch: while a layout is attached, step the cursor
// state machine (states 1/2 -> cursor steps, 4/5 -> cursor steps, 6/7 ->
// skill-info steps), then always advance the layout animation and run the
// +0x20 skill-info animation step.
extern "C" void func_80264B2C(UI::CPassiveSkillCur* self) {
    if (self->mpLayout == 0) {
        return;
    }
    switch (self->field_18) {
    case 1:
        func_80266294(self);
        break;
    case 2:
        func_80266344(self);
        break;
    case 4:
        func_802663C0(self);
        break;
    case 5:
        func_80266470(self);
        break;
    case 6:
        func_802664EC(reinterpret_cast<UI_CPassiveSkillInfo*>(self));
        break;
    case 7:
        func_80266574(reinterpret_cast<UI_CPassiveSkillInfo*>(self));
        break;
    }
    self->mpLayout->Animate(0);
    func_8026439C(reinterpret_cast<UI_PassiveSkillInit*>(&self->_pad20));
}

// Draw the skill-info cursor: early-return shape per retail -- bail before
// drawing anything when the main layout at +0x8 is null; otherwise draw it,
// then require both gate flags (+0x1B and +0x38) via separate early returns
// before drawing the +0x24 second layout.
// extern "C": func_80267484's call reloc must be the plain retail name;
// noinline keeps the retail `bl func_80264BE4`.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80264BE4(UI::CPassiveSkillInfo* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mpLayout == 0) {
        return;
    }
    drawLayout(self->mpLayout, drawInfo, 0, 1);
    if (self->field_1B == 0) {
        return;
    }
    if (self->field_38 == 0) {
        return;
    }
    drawLayout(self->field_24, drawInfo, 0, 1);
}
#pragma optimize_for_size off

// Release both cursor layouts: drop the primary layout at +0x8 through its
// vtable delete slot, then release the second-layout holder at +0x20 via the
// +0x4 release helper (func_80264344). noinline: func_802675D8 keeps the
// retail `bl`.
extern "C" __declspec(noinline) void func_80264C58(UI::CPassiveSkillCur* self) {
    if (self->mpLayout != 0) {
        delete self->mpLayout;
        self->mpLayout = 0;
    }
    func_80264344(reinterpret_cast<UI_PassiveSkillLayoutRef*>(&self->_pad20));
}

// Cursor step: set the state byte to 1, clear the +0x19/+0x1A flags, then
// bind and enable the +0xC animation transform on the layout at +0x8.
extern "C" __declspec(noinline) void func_80264CB8(UI::CPassiveSkillCur* self) {
    self->field_18 = 1;
    self->field_19 = 0;
    self->field_1A = 0;
    self->mpLayout->BindAnimation(self->mpAnimTrans0);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, 1);
}

// Cursor step: same as func_80264CB8 but with state 4 and the +0x10
// animation transform. noinline: func_80267B08 keeps the retail `bl`.
__declspec(noinline) void func_80264D28(UI::CPassiveSkillCur* self) {
    self->field_18 = 4;
    self->field_19 = 0;
    self->field_1A = 0;
    self->mpLayout->BindAnimation(self->mpAnimTrans1);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, 1);
}

// Cursor step: set the state byte to 6, clear the +0x19 flag, then bind and
// enable the +0x14 animation transform on the layout at +0x8. C linkage so
// func_80269B14's call reloc is the plain retail `func_80264D98`; noinline
// keeps the retail `bl`.
extern "C" __declspec(noinline) void func_80264D98(UI_PassiveSkillStep* self) {
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

// Set the skill-name textures for the embedded info object's two slots: look
// up the message ids for the two character ids in the shared BDAT table,
// resolve each to its 'timg' texture resource and attach it to the info
// layout's two named panes (pool+0x1f1 / pool+0x1fc) when found. The five
// callee-saved registers (r27-r31) need the stmw/lmw frame retail uses, which
// -O4,p only emits under optimize_for_size.
#pragma optimize_for_size on
// C linkage so func_80269C08's call reloc is the plain retail
// `func_80264E70` (retail has no C++ mangled callee here).
extern "C" __declspec(noinline) void func_80264E70(UI::CPassiveSkillInfo* self, u8 id1, u8 id2) {
    if (id1 == 0 || id2 == 0) {
        return;
    }
    void* table = lbl_eu_80664090;
    u16 v1 = func_80136254(table, &lbl_eu_8050DC20[0x1e8], id1);
    u16 v2 = func_80136254(table, &lbl_eu_8050DC20[0x1e8], id2);
    char* text1 = func_80138F78(v1);
    void* res1 = func_801355F4()->GetResource(0x74696d67, text1, 0);
    if (res1 != 0) {
        func_80137E7C(self->mpLayout, &lbl_eu_8050DC20[0x1f1], res1);
    }
    char* text2 = func_80138F78(v2);
    void* res2 = func_801355F4()->GetResource(0x74696d67, text2, 0);
    if (res2 != 0) {
        func_80137E7C(self->mpLayout, &lbl_eu_8050DC20[0x1fc], res2);
    }
}
#pragma optimize_for_size off

// Forward declarations for the skill-pane setters called by func_80264F7C
// (defined later in this TU).
extern "C" void func_802665FC(UI::CPassiveSkillLine* self, int id);
extern "C" void func_80266724(UI::CPassiveSkillLine* self, int charId, int row, int slot);
void func_80266930(UI_CPassiveSkill* self, u8 row, u8 col, u8 slot);
extern "C" void func_80266950(UI::CPassiveSkillLine* self, u8 index);

// Skill-menu selection state handler (retail func_80264F7C): dispatch on the
// menu state byte to refresh the name/desc/cost/icon panes for a selected
// skill-grid cell. Each state uses a different BDAT table set and ends with
// the shared summary refresh (func_802665FC + a grid-slot setter).
extern "C" void func_80264F7C(UI::CPassiveSkillLine* self, int state, int id,
                              int row, int col, int r8) {
    char buf3[0x20];  // slot-name buffer (sp+0x68)
    char buf2[0x20];  // slot-name buffer (sp+0x48)
    char buf1[0x20];  // slot-name buffer (sp+0x28)
    char buf0[0x20];  // slot-name buffer (sp+0x08)
    switch (state) {
    case 0: {
        u8 idx = (u8)(row + (id - 1) * 5 + 1);
        char* text = func_8013639C(lbl_eu_80664884, &lbl_eu_8050DC20[0x207], idx);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x20c], text, 0);
        text = func_8013639C(lbl_eu_80664884, &lbl_eu_8050DC20[0x1a4], idx);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x215], text, 0);
        text = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x15);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x22b], text, 0);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x154], &lbl_eu_8050DC20[0x182], 0);
        u8 v1 = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x235], idx);
        text = func_8013639C(lbl_eu_80664894, &lbl_eu_8050DC20[0x207], (u8)v1);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x23b], text, 0);
        u8 v2 = func_801361E8((u32)lbl_eu_80664894, &lbl_eu_8050DC20[0x245], (u8)v1);
        UI::CPSkillRecord* rec = reinterpret_cast<UI::CPSkillRecord*>(
            (u8*)func_8009EC9C(id) + row * 0xc4 + 0x3534);
        u8 found = 1;
        for (u8 i = 1; i <= 5; i++) {
            if (rec->slots[i].word == 0) {
                sprintf(buf3, &lbl_eu_8050DC20[0x24e], i);
                u8 v3 = func_801361E8((u32)lbl_eu_80664890, buf3, idx);
                if (v3 != 0 && rec->slots[i].word != 0xe && rec->slots[i].word != 0x4b &&
                    rec->slots[i].word != 0xa5 && rec->slots[i].word != 0xc8) {
                    char* t12 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x12);
                    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], t12, 0);
                    char* t6 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x6);
                    if (v2 == 1) {
                        sprintf(buf3, &lbl_eu_8050DC20[0x25e], (u8)v3);
                    } else {
                        sprintf(buf3, &lbl_eu_8050DC20[0x261], (u8)v3, t6);
                    }
                    func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x160], buf3, 0);
                } else {
                    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], &lbl_eu_8050DC20[0x182], 0);
                    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x160], &lbl_eu_8050DC20[0x182], 0);
                }
                found = 0;
                break;
            }
        }
        if (found != 0) {
            u8 v3 = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x266], idx);
            if (v3 != 0) {
                char* t12 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x12);
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], t12, 0);
                char* t6 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x6);
                if (v2 == 1) {
                    sprintf(buf3, &lbl_eu_8050DC20[0x25e], (u8)v3);
                } else {
                    sprintf(buf3, &lbl_eu_8050DC20[0x261], (u8)v3, t6);
                }
                func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x160], buf3, 0);
            } else {
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], &lbl_eu_8050DC20[0x182], 0);
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x160], &lbl_eu_8050DC20[0x182], 0);
            }
        }
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x26b], &lbl_eu_8050DC20[0x182], 0);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x16c], &lbl_eu_8050DC20[0x182], 0);
        func_802665FC(self, id);
        func_80266724(self, 0, row, col);
        break;
    }
    case 1:
    case 2: {
        u8 idx = (u8)(col + row * 5 + (id - 1) * 0x19 + 1);
        char* text = func_8013639C(lbl_eu_8066488C, &lbl_eu_8050DC20[0x207], idx);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x20c], text, 0);
        text = func_8013639C(lbl_eu_80664880, &lbl_eu_8050DC20[0x1a4], idx);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x215], text, 0);
        u8 v0 = func_801361E8((u32)lbl_eu_80664880, &lbl_eu_8050DC20[0x275], idx);
        char* tSel = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], v0 == 1 ? 0x15 : 0x14);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x22b], tSel, 0);
        u8 v1 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x27a], idx);
        char* t6 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x6);
        if (v1 == 0) {
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x154], &lbl_eu_8050DC20[0x182], 0);
        } else {
            sprintf(buf2, &lbl_eu_8050DC20[0x261], (u8)v1, t6);
            func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x154], buf2, 0);
        }
        u8 v2 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x235], idx);
        text = func_8013639C(lbl_eu_80664894, &lbl_eu_8050DC20[0x207], (u8)v2);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x23b], text, 0);
        u8 v3 = func_801361E8((u32)lbl_eu_80664894, &lbl_eu_8050DC20[0x245], (u8)v2);
        u8 v4 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x27f], idx);
        if (v4 != 0 && idx != 0xe && idx != 0x4b && idx != 0xa5 && idx != 0xc8) {
            char* t12 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x12);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], t12, 0);
            if (v3 == 1) {
                sprintf(buf2, &lbl_eu_8050DC20[0x25e], (u8)v4);
            } else {
                sprintf(buf2, &lbl_eu_8050DC20[0x261], (u8)v4, t6);
            }
            func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x160], buf2, 0);
        } else {
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x160], &lbl_eu_8050DC20[0x182], 0);
        }
        u8 v5 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x284], idx);
        if (v5 != 0) {
            char* t13 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x13);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x26b], t13, 0);
            char* t5 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x5);
            sprintf(buf2, &lbl_eu_8050DC20[0x261], (u8)v5, t5);
            func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x16c], buf2, 0);
        } else {
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x26b], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x16c], &lbl_eu_8050DC20[0x182], 0);
        }
        if (state == 1) {
            func_802665FC(self, id);
            func_80266724(self, id, row, col);
        } else {
            func_802665FC(self, r8);
            func_80266930(reinterpret_cast<UI_CPassiveSkill*>(self), (u8)id, (u8)row, (u8)col);
        }
        break;
    }
    default: {
        if (col != 0) {
            char* t19 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x19);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x20c], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x215], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x22b], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x154], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x23b], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x160], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x26b], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x16c], &lbl_eu_8050DC20[0x182], 0);
            char* t18 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x18);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x183], t18, 0);
            void* res = 0;
            switch (r8) {
            case 0: res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x289], 0); break;
            case 1: res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x2a1], 0); break;
            case 2: res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x2b9], 0); break;
            case 3: res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x2d1], 0); break;
            case 4: res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x2e9], 0); break;
            }
            if (res != 0) {
                func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x301], res);
                func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x301], true), 1);
            }
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x178], &lbl_eu_8050DC20[0x182], 0);
        } else if (row != 0) {
            char* text = func_8013639C(lbl_eu_8066488C, &lbl_eu_8050DC20[0x207], row);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x20c], text, 0);
            text = func_8013639C(lbl_eu_80664880, &lbl_eu_8050DC20[0x1a4], row);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x215], text, 0);
            u8 v0 = func_801361E8((u32)lbl_eu_80664880, &lbl_eu_8050DC20[0x275], row);
            char* tSel = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], v0 == 1 ? 0x15 : 0x14);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x22b], tSel, 0);
            u8 v1 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x27a], row);
            char* t6 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x6);
            if (v1 == 0) {
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x154], &lbl_eu_8050DC20[0x182], 0);
            } else {
                sprintf(buf0, &lbl_eu_8050DC20[0x261], (u8)v1, t6);
                func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x154], buf0, 0);
            }
            u8 v2 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x235], row);
            text = func_8013639C(lbl_eu_80664894, &lbl_eu_8050DC20[0x207], (u8)v2);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x23b], text, 0);
            u8 v3 = func_801361E8((u32)lbl_eu_80664894, &lbl_eu_8050DC20[0x245], (u8)v2);
            u8 v4 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x27f], row);
            if (v4 != 0 && row != 0xe && row != 0x4b && row != 0xa5 && row != 0xc8) {
                char* t12 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x12);
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], t12, 0);
                if (v3 == 1) {
                    sprintf(buf0, &lbl_eu_8050DC20[0x25e], (u8)v4);
                } else {
                    sprintf(buf0, &lbl_eu_8050DC20[0x261], (u8)v4, t6);
                }
                func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x160], buf0, 0);
            } else {
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], &lbl_eu_8050DC20[0x182], 0);
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x160], &lbl_eu_8050DC20[0x182], 0);
            }
            u8 v5 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x284], row);
            if (v5 != 0) {
                char* t13 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x13);
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x26b], t13, 0);
                char* t5 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x5);
                sprintf(buf0, &lbl_eu_8050DC20[0x261], (u8)v5, t5);
                func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x16c], buf0, 0);
            } else {
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x26b], &lbl_eu_8050DC20[0x182], 0);
                func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x16c], &lbl_eu_8050DC20[0x182], 0);
            }
            char* t16 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x16);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x178], t16, 0);
        } else {
            char* t19 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x19);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x20c], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x215], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x22b], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x154], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x23b], t19, 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x254], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x160], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x26b], &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x16c], &lbl_eu_8050DC20[0x182], 0);
            char* t17 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x17);
            func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x178], t17, 0);
        }
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x183], &lbl_eu_8050DC20[0x182], 0);
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x301], true), 0);
        func_802665FC(self, id);
        func_80266950(self, row);
        break;
    }
    }
}

// Skill-info update for the embedded CPassiveSkillInfo (retail func_802660EC).
// C linkage so callers (func_80269A98 / func_80269B94) emit the plain retail
// reloc; noinline keeps the retail `bl`. The three callee-saved registers
// (r29-r31) need the stmw/lmw frame retail uses, which -O4,p only emits under
// optimize_for_size.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802660EC(UI::CPassiveSkillInfo* self) {
    // Toggle both skill-name panes: find them by name in the main layout's
    // root pane, then flip each pane's visibility to its current inverse,
    // and finally toggle the +0x1B mode byte.
    nw4r::lyt::Pane* pane0 =
        self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x18d], true);
    nw4r::lyt::Pane* pane1 =
        self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x30d], true);
    func_80124270(pane0, !func_801C4648(pane0));
    func_80124270(pane1, !func_801C4648(pane1));
    // Retail flips the byte with the `!= 1` idiom (xori/addic/subfe).
    self->field_1B = (u8)((self->field_1B ^ 1) != 0);
}
#pragma optimize_for_size off

// Set the cursor's skill-name pane text: look up message 135 from the shared
// string table, then push it into the layout pane named at pool+0x1a9.
// noinline: func_802698B8 keeps the retail `bl` to this 0x54-byte body.
#pragma optimize_for_size on  // -O4,s keeps the retail stmw r30 frame
__declspec(noinline) void func_802661A8(UI::CPassiveSkillCur* self) {
    char* text = func_80136190(&lbl_eu_8050DC20[0x196], &lbl_eu_8050DC20[0x1a4], 135);
    func_80136B4C(self->mpLayout, &lbl_eu_8050DC20[0x1a9], text, 0);
}
#pragma optimize_for_size off

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
// handing the address to the layout translate helper. extern "C": the retail
// symbol is the plain `func_80266250` (call reloc from func_80269004 must be
// the unmangled name); noinline keeps func_80269004's retail `bl` (MWCC would
// otherwise inline this body and grow the caller's frame).
extern "C" __declspec(noinline) void func_80266250(UI::CPassiveSkillCur* self, nw4r::math::VEC3 pos) {
    nw4r::math::VEC3 local = pos;
    func_801D2150(self->field_24->GetRootPane(), &local);
}

// Cursor step (state 2): when the first anim transform's frame check
// succeeds, set the step state to 2, clear the +0x1A flag, advance the layout
// animation, bind the first anim transform, then unbind and re-enable the
// second one.
__declspec(noinline) void func_80266294(UI::CPassiveSkillCur* self) {
    if (advanceAnimTransform(self->mpAnimTrans0, lbl_eu_80668900) != 0) {
        self->field_18 = 2;
        self->field_1A = 1;
        self->mpLayout->Animate(0);
        self->mpLayout->UnbindAnimation(self->mpAnimTrans0);
        self->mpLayout->BindAnimation(self->mpAnimTrans1);
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, 1);
    }
}

// Cursor step: when the second anim transform's frame check succeeds, set the
// step state to 3, advance the layout animation, and unbind the transform.
__declspec(noinline) void func_80266344(UI::CPassiveSkillCur* self) {
    if (advanceAnimTransform(self->mpAnimTrans1, lbl_eu_80668900) != 0) {
        self->field_18 = 3;
        self->field_19 = 1;
        self->mpLayout->Animate(0);
        self->mpLayout->UnbindAnimation(self->mpAnimTrans1);
    }
}

// Cursor step (state 5): when the second anim transform's frame check
// succeeds, set the step state to 5, clear the +0x1A flag, advance the layout
// animation, then unbind the second transform and bind/re-enable the first
// one (mirror of func_80266294).
__declspec(noinline) void func_802663C0(UI::CPassiveSkillCur* self) {
    if (func_80137510(self->mpAnimTrans1, lbl_eu_80668900) != 0) {
        self->field_18 = 5;
        self->field_1A = 1;
        self->mpLayout->Animate(0);
        self->mpLayout->UnbindAnimation(self->mpAnimTrans1);
        self->mpLayout->BindAnimation(self->mpAnimTrans0);
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, 1);
    }
}

// Cursor step: when the first anim transform's frame check succeeds, set the
// step state to 0, advance the layout animation, and unbind the transform.
__declspec(noinline) void func_80266470(UI::CPassiveSkillCur* self) {
    if (func_80137510(self->mpAnimTrans0, lbl_eu_80668900) != 0) {
        self->field_18 = 0;
        self->field_19 = 1;
        self->mpLayout->Animate(0);
        self->mpLayout->UnbindAnimation(self->mpAnimTrans0);
    }
}

// Skill-info step: when the +0x14 anim transform's frame check succeeds, set
// the step state to 3, advance the layout animation and unbind the transform;
// then copy the anim transform's current frame into the +0x1C float.
__declspec(noinline) void func_802664EC(UI_CPassiveSkillInfo* self) {
    if (advanceAnimTransform(self->field_14, lbl_eu_80668900) != 0) {
        self->field_18 = 3;
        self->field_19 = 1;
        self->field_8->Animate(0);
        self->field_8->UnbindAnimation(self->field_14);
    }
    self->field_1C = self->field_14->GetFrame();
}

// Skill-info step: same as func_802664EC but using the other frame-check
// helper (func_80137510).
__declspec(noinline) void func_80266574(UI_CPassiveSkillInfo* self) {
    if (func_80137510(self->field_14, lbl_eu_80668900) != 0) {
        self->field_18 = 3;
        self->field_19 = 1;
        self->field_8->Animate(0);
        self->field_8->UnbindAnimation(self->field_14);
    }
    self->field_1C = self->field_14->GetFrame();
}

// Skill-points summary (retail func_802665FC): sum the message ids of the
// character's learned secondary-record slots, format the remaining/total SP
// into the +0x12d pane via sprintf, then set the +0x328/+0x332 panes to the
// 0x10/0xF messages. The seven callee-saved registers (r25-r31) need the
// stmw/lmw frame retail uses, which -O4,p only emits under optimize_for_size.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802665FC(UI::CPassiveSkillLine* self, int id) {
    // Declared at the top so the retail allocator keeps its web across the
    // loop (spill-list coloring) and reuses the dead blob register (r30) for
    // the post-loop total load; declaring it after the loop colors it last
    // into a lower register (r26).
    u32 total;
    UI::CPSkillBlobTotal* blob = reinterpret_cast<UI::CPSkillBlobTotal*>(
        (u8*)func_8009EC9C((u8)id) + 0x3534);
    UI::CPSkillRecord* rec;
    u32 sum = 0;
    for (u8 row = 0; row < 6; row++) {
        rec = &blob->records2[row];
        for (u8 slot = 1; slot <= 5; slot++) {
            if (rec->slots[slot].word != 0) {
                sum += func_80136254(lbl_eu_8066488C, &lbl_eu_8050DC20[0x318], rec->slots[slot].word);
            }
        }
    }
    total = blob->totalSP;
    char* text = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 4);
    char buf[0x1C];
    sprintf(buf, &lbl_eu_8050DC20[0x321], total - sum, text, total);
    func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x12d], buf, 0);
    char* text2 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x10);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x328], text2, 0);
    char* text3 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0xf);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x332], text3, 0);
}
#pragma optimize_for_size off

// Skill-learn slot display (retail func_80266724): when the character's
// (row, slot) record cell is occupied or the character id is null, show the
// generic error message; otherwise compute the grid index for the slot,
// look up its message and SP cost, and push the formatted text into the
// +0x139 pane. Always refresh the three summary panes. The stmw r25 frame
// retail uses needs optimize_for_size (seven callee-saved registers).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80266724(UI::CPassiveSkillLine* self, int charId, int row, int slot) {
    UI::CPSkillBlobCost* blob = reinterpret_cast<UI::CPSkillBlobCost*>(
        (u8*)func_8009EC9C(charId) + 0x3534);
    UI::CPSkillRecord* rec = &blob->records[row];
    if (rec->slots[slot + 1].word != 0 || charId == 0) {
        char* msg4 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 4);
        char* msgE = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0xe);
        char* msg19 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x19);
        char errBuf[0x28];
        sprintf(errBuf, &lbl_eu_8050DC20[0x33b], msg19, msg4, msg19, msgE);
        func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x139], errBuf, 0);
    } else {
        u32 table = (u32)lbl_eu_8066488C;
        u8 v = (u8)(row * 5 + slot + (charId - 1) * 0x19 + 1);
        u8 result = func_801361E8(table, &lbl_eu_8050DC20[0x344], v);
        u16 v100 = (u16)(result * 100);
        u16 cost = 0;
        if (slot == 0) {
            cost = (u16)blob->costs[(u8)row];
        } else if (rec->slots[(u8)slot].word != 0) {
            cost = (u16)blob->costs[(u8)row];
        }
        char* msg4 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 4);
        char* msgE = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0xe);
        char buf[0x20];
        sprintf(buf, &lbl_eu_8050DC20[0x34d], cost, msg4, v100, msgE);
        func_80136A1C(self->field_8, &lbl_eu_8050DC20[0x139], buf, 0);
    }
    char* msgD = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0xd);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x356], msgD, 0);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x145], &lbl_eu_8050DC20[0x182], 0);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x35f], &lbl_eu_8050DC20[0x182], 0);
}
#pragma optimize_for_size off

// Skill-name pane setter for the +0x28 line (retail func_80266950): the
// layout it drives is the line's +0x8 field. C linkage so the definition
// symbol and func_80266930's call reloc are the plain retail name.
// noinline: -ipa would otherwise fold the body into func_80266930's call
// site (retail keeps the pure 0x20 tail call).
extern "C" __declspec(noinline) void func_80266950(UI::CPassiveSkillLine* self,
                                                   u8 index);

// Linearize a 5x5 skill grid into a slot index: (row-1)*25 + col*5 + slot + 1.
// optimize_for_size on: retail keeps mulli for col*5 (the -O4,s lowering;
// -O4,p strength-reduces to rlwinm+add).
#pragma optimize_for_size on
void func_80266930(UI_CPassiveSkill* self, u8 row, u8 col, u8 slot) {
    return func_80266950(reinterpret_cast<UI::CPassiveSkillLine*>(self),
                         (u8)(1 + slot + col * 5 + (row - 1) * 25));
}
#pragma optimize_for_size off

#pragma optimize_for_size on
// Skill-name pane setter (retail func_80266950): for the given grid slot,
// set the +0x356 pane to message 0x11's text; look up the slot's table id and
// either push the id text into the +0x145 pane or set the pane's text from
// message 0x19; then set the +0x35f pane to message 0x10's text and the
// +0x139 pane to the fixed +0x182 string. The stmw r29 frame retail uses
// needs optimize_for_size (three callee-saved registers).
void func_80266950(UI::CPassiveSkillLine* self, u8 index) {
    char* text = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x11);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x356], text, 0);
    u16 id = func_80136254(lbl_eu_8066488C, &lbl_eu_8050DC20[0x318], index);
    if ((id & 0xFF) == 0) {
        char* text2 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x19);
        func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x145], text2, 0);
    } else {
        setLayoutTextBoxNumber(self->field_8, &lbl_eu_8050DC20[0x145], id);
    }
    char* text3 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x10);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x35f], text3, 0);
    func_80136B4C(self->field_8, &lbl_eu_8050DC20[0x139], &lbl_eu_8050DC20[0x182], 0);
}
#pragma optimize_for_size off

// Line sub-object constructor (retail symbol __ct__UI_CPassiveSkillLine).
// C linkage so __ct__UI_CPassiveSkill's `bl` binds to the plain name; noinline
// keeps the retail `bl`. Installs the vtable, stores the caller arg, zero/one-
// inits the state fields (+0xE9 starts at 1), seeds the +0x100 frame float,
// then constructs the embedded info (+0x120), its +0x3C cursor, the +0x178
// CCur18, and finally memsets the +0x20..0xE7 region.
extern "C" __declspec(noinline) void* __ct__UI_CPassiveSkillLine(UI::CPassiveSkillLine* self, nw4r::lyt::ArcResourceAccessor* accessor) {
    self->mArg = accessor;
    self->mVtbl = (void*)lbl_eu_805379FC;
    self->field_8 = 0;
    self->field_C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C = 0;
    self->field_E8 = 0;
    self->field_E9 = 1;
    self->field_F2 = 0;
    self->field_F3 = 0;
    self->field_F4 = 0;
    self->field_F5 = 0;
    self->field_F6 = 0;
    self->field_F7 = 0;
    self->field_F8 = 0;
    self->field_F9 = 0;
    self->field_FA = 0;
    self->field_FD = 0;
    self->field_FE = 0;
    self->field_100 = lbl_eu_80668904;
    self->field_11D = 0;
    self->field_11E = 0;
    self->field_11F = 0;
    __ct__UI_CPassiveSkillInfo(reinterpret_cast<UI_CPassiveSkillInfo*>(&self->mInfo), 0);
    __ct__UI_CPassiveSkillCur(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo.field_3C), 0);
    self->mInfo.field_54 = 0;
    __ct__CCur18(&self->mCur, 0);
    memset(&self->cells, 0, 0xC8);
    return self;
}

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

extern "C" void func_80139198(u32 arg);
extern "C" void __dt__Q22UI17CPassiveSkillInfoFv(UI_CPassiveSkillInfo* self, int flag);
extern "C" void __dt__6CCur18Fv(CCur18* self, int flag);

// Line-subobject rebuild (retail func_80266B78): called from OnFileEvent after
// the skill-data read. Creates the main layout (+0x8) and three anim
// transforms, binds the device font onto the root pane, creates the secondary
// layout (+0x18), builds the 5x5 cell layouts with per-pane translate setup,
// then reconstructs the embedded mInfo/mCur/CCur18 sub-objects from the loaded
// resources and seeds the category table by character id.
void func_80266B78(UI::CPassiveSkillLine* self){}

// Copy a UI_CPassiveSkillInfo's non-vtable state (words +0x4..+0x14, bytes
// +0x18..+0x1B, float +0x1C) and the +0x20 sub-struct via func_80264AC8,
// returning the destination.
// noinline: retail keeps the `bl` from OnFileEvent's load path.
__declspec(noinline) UI_CPassiveSkillInfo* func_80267268(UI_CPassiveSkillInfo* dst,
                                    const UI_CPassiveSkillInfo* src) {
    // Volatile-sourced accesses: retail emits strict load/store interleaving
    // through one scratch register per member (conservative aliasing); with
    // plain member copies -ipa proves the call-site operands disjoint and
    // batches all loads into separate registers first.
    #define CPS_COPY(member) \
        ((volatile UI_CPassiveSkillInfo*)dst)->member = \
            ((volatile const UI_CPassiveSkillInfo*)src)->member
    CPS_COPY(arg);
    CPS_COPY(field_8);
    CPS_COPY(field_C);
    CPS_COPY(field_10);
    CPS_COPY(field_14);
    CPS_COPY(field_18);
    CPS_COPY(field_19);
    CPS_COPY(field_1A);
    CPS_COPY(field_1B);
    CPS_COPY(field_1C);
    #undef CPS_COPY
    func_80264AC8(&dst->sub, &src->sub);
    return dst;
}
// noinline: retail keeps the `bl` from OnFileEvent's load path.
__declspec(noinline) void func_802672F0(UI::CPassiveSkillCur* dst, const UI::CPassiveSkillCur* src) {
    dst->mArcResAcc = src->mArcResAcc;
    dst->mpLayout = src->mpLayout;
    dst->mpAnimTrans0 = src->mpAnimTrans0;
    dst->mpAnimTrans1 = src->mpAnimTrans1;
    dst->mActive = src->mActive;
    dst->mVisible = src->mVisible;
    dst->field_16 = src->field_16;
}

// noinline: retail keeps the `bl` from OnFileEvent's load path.
__declspec(noinline) void func_8026732C(UI::CPassiveSkillCur* dst, const UI::CPassiveSkillCur* src) {
    dst->mArcResAcc = src->mArcResAcc;
    dst->mpLayout = src->mpLayout;
    dst->mpAnimTrans0 = src->mpAnimTrans0;
    dst->mpAnimTrans1 = src->mpAnimTrans1;
    dst->mActive = src->mActive;
    dst->mVisible = src->mVisible;
}

// Forward declarations for the func_80267360 state-machine dispatch (defined
// later in this TU).
// extern "C" on all dispatch targets: the retail call relocs from
// func_80267360 use the plain (unmangled) names.
extern "C" void func_80268FEC(void* self);
extern "C" void func_80269004(UI::CPassiveSkillLine* self);
extern "C" void func_8026916C(UI::CPassiveSkillLine* self);
extern "C" void func_80269200(UI_CPassiveSkill* self);
extern "C" void func_80269220(UI::CPassiveSkillLine* self);
extern "C" void func_802692E0(UI::CPassiveSkillLine* self);
extern "C" void func_80269370(UI::CPassiveSkillLine* self);
extern "C" void func_80269410(UI::CPassiveSkillLine* self);
extern "C" void func_802694F4(UI::CPassiveSkillLine* self);
extern "C" void func_802695A8(UI::CPassiveSkillLine* self);
extern "C" void func_80269638(UI::CPassiveSkillLine* self);
extern "C" void func_802696D8(UI::CPassiveSkillLine* self);
extern "C" void func_80269768(UI::CPassiveSkillLine* self);
extern "C" __declspec(noinline) void func_80263FE8(UI::CPassiveSkillCur* self);

// +0x28 sub-object update (retail func_80267360): dispatch on the +0xE8 menu
// state byte (jump table over 0..0x10; state 0xD falls through), then always
// advance the primary/secondary layouts, the embedded info cursor at +0x120
// and the +0x3C region cursor, and finally the +0x178 CCur18.
extern "C" __declspec(noinline) void func_80267360(u8* selfRaw) {
    UI::CPassiveSkillLine* self = reinterpret_cast<UI::CPassiveSkillLine*>(selfRaw);
    if (self->field_8 == 0) {
        return;
    }
    // NOTE: the idle states 0xE..0x10 skip the common update pass (their
    // jump-table rows route straight to the epilogue); the explicit goto
    // keeps those rows distinct from the default/exit target so MWCC does
    // not truncate the table to the last real handler.
    switch (self->field_E8) {
    case 0:
        func_80268FEC(self);
        break;
    case 1:
        func_80269004(self);
        break;
    case 2:
        advanceAnimTransform(self->field_14, lbl_eu_80668900);
        break;
    case 3:
        func_8026916C(self);
        break;
    case 4:
        func_80269200(reinterpret_cast<UI_CPassiveSkill*>(self));
        break;
    case 5:
        func_80269220(self);
        break;
    case 6:
        func_802692E0(self);
        break;
    case 7:
        func_80269370(self);
        break;
    case 8:
        func_80269410(self);
        break;
    case 9:
        func_802694F4(self);
        break;
    case 0xA:
        func_802695A8(self);
        break;
    case 0xB:
        func_80269638(self);
        break;
    case 0xC:
        func_802696D8(self);
        break;
    case 0xD:
        func_80269768(self);
        break;
    case 0xE:
    case 0xF:
    case 0x10:
        goto done;
    default:
        break;
    }
    self->field_8->Animate(0);
    self->field_18->Animate(0);
    func_80264B2C(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
    func_80263FE8(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo.field_3C));
    if (self->mInfo.field_54 != 0) {
        func_8022B748(self->mInfo.field_54);
    }
    func_801D202C(&self->mCur);
done:;
}

// Skill-menu draw (retail func_80267484): draw the primary/secondary
// layouts, then the 5x5 skill grid when the menu state is in {1,2,4..8},
// then the embedded info sub-object, an extra region layout when the window
// is closed, and finally the syswin + cursor. The five callee-saved
// registers (r27-r31) need the stmw/lmw frame retail uses, which -O4,p only
// emits under optimize_for_size. extern "C" keeps the plain retail symbol;
// noinline stops -ipa folding the body into func_8026D8FC's call site.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80267484(UI::CPassiveSkillLine* self,
                                                   nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_8 == 0) {
        return;
    }
    drawLayout(self->field_8, drawInfo, 0, 1);
    drawLayout(self->field_18, drawInfo, 0, 1);
    u8 state = self->field_E8;
    if ((u8)(state + 0xFC) <= 4 || (u8)(state + 0xFF) <= 1) {
        for (u8 row = 0; row < 5; row++) {
            for (u8 col = 0; col < 5; col++) {
                drawLayout(self->cells[row][col].mpLayout, drawInfo, 0, 1);
            }
        }
    }
    func_80264BE4(&self->mInfo, drawInfo);
    if (self->mInfo.field_38 == 0 && self->mInfo.field_54 != 0 &&
        CSysWin_getUnk34(self->mInfo.field_54) == 0 &&
        self->mInfo.field_3C.field_8 != 0) {
        drawLayout(self->mInfo.field_3C.field_8, drawInfo, 0, 1);
    }
    if (self->mInfo.field_54 != 0) {
        func_8022B7C8(self->mInfo.field_54, drawInfo);
        if (CSysWin_getUnk34(self->mInfo.field_54) != 0 &&
            CSysWin_isActive(self->mInfo.field_54) != 0) {
            func_801D20B0(&self->mCur, drawInfo);
        }
    }
}
#pragma optimize_for_size off

// Line teardown (retail func_802675D8): delete every layout in the 5x5
// skill grid (through its virtual dtor), then the +0x18 secondary and +0x8
// primary layouts, release the embedded info layouts (+0x120 cursor pair /
// +0x15C region) and run the embedded CCur18 cursor's +0xC teardown
// virtual. The 6 callee-saved registers (r26-r31) need the stmw/lmw frame
// retail uses, which -O4,p only emits under optimize_for_size.
#pragma optimize_for_size on
// noinline: retail keeps the `bl` from func_8026D920's teardown.
__declspec(noinline) void func_802675D8(UI::CPassiveSkillLine* self) {
    for (u8 row = 0; row < 5; row++) {
        for (u8 col = 0; col < 5; col++) {
            if (self->cells[row][col].mpLayout != 0) {
                delete self->cells[row][col].mpLayout;
                self->cells[row][col].mpLayout = 0;
            }
        }
    }
    if (self->field_18 != 0) {
        delete self->field_18;
        self->field_18 = 0;
    }
    if (self->field_8 != 0) {
        delete self->field_8;
        self->field_8 = 0;
    }
    func_80264C58(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
    func_80264060(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo.field_3C));
    reinterpret_cast<CCur18TearView*>(&self->mCur)->vf03();
}
#pragma optimize_for_size off

// Retail symbol is the unmangled `func_802676DC`; C linkage so call sites
// (func_8026D9F0) emit the plain reloc name instead of a C++ mangling, and
// noinline so the 0x1C-byte body is not inlined into them (retail keeps the
// `bl` -- the callee lives in a separate retail TU).
extern "C" __declspec(noinline) u32 func_802676DC(UI_CPassiveSkill* self) {
    if (self->field_139 != 0) {
        return self->field_E9;
    }
    return 0;
}

// +0x28 sub-object update (retail func_80269B68, 0x410 bytes): skill-menu open
// refresh. Sets the menu state to 1, binds the line layout's +0xC animation,
// steps the embedded info cursor (+0x120) and the line state machines, then
// redraws every description pane for the currently selected grid cell
// (mirrors func_80264F7C's state-0 arm) and refreshes the SP summary.
// noinline so func_8026DA4C's tail call stays a `b` instead of being inlined;
// C linkage so the definition symbol and call relocs stay the plain retail
// name `func_802676F8`.
#pragma optimize_for_size on  // retail stmw r23 frame needs -O4,s
extern "C" __declspec(noinline) void func_802676F8(UI::CPassiveSkillLine* self) {
    self->field_E8 = 1;
    self->field_E9 = 0;
    self->field_8->BindAnimation(self->field_C);
    self->field_8->SetAnimationEnable(self->field_C, 1);
    func_80264CB8(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
    func_80269C08(self);
    func_80269D20(self);

    u8 row = self->field_F5;
    u8 chId = func_801392B4(self->field_F3);
    int t = (chId - 1) * 5;
    u8 idx = (u8)(row + t + 1);
    char buf[0x20];

    char* text = func_8013639C(lbl_eu_80664884, &lbl_eu_8050DC20[0x207], idx);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x20c], text, 0);
    text = func_8013639C(lbl_eu_80664884, &lbl_eu_8050DC20[0x1a4], idx);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x215], text, 0);
    text = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x15);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x22b], text, 0);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x154],
                  &lbl_eu_8050DC20[0x182], 0);
    u8 v1 = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x235], idx);
    text = func_8013639C(lbl_eu_80664894, &lbl_eu_8050DC20[0x207], (u8)v1);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x23b], text, 0);
    u8 v2 = func_801361E8((u32)lbl_eu_80664894, &lbl_eu_8050DC20[0x245], (u8)v1);

    // Scan the character's five learn slots for the first empty cell.
    u8* base = (u8*)func_8009EC9C((u8)chId) + row * 0xc4;
    UI::CPSkillRecord* rec = reinterpret_cast<UI::CPSkillRecord*>(base + 0x3534);
    u8 i, found;
    for (found = 1, i = 1; i <= 5; i++) {
        if (rec->slots[i].word == 0) {
            sprintf(buf, &lbl_eu_8050DC20[0x24e], (u8)i);
            u8 v3 = func_801361E8((u32)lbl_eu_80664890, buf, idx);
            if (v3 != 0 && rec->slots[i].word != 0xe && rec->slots[i].word != 0x4b &&
                rec->slots[i].word != 0xa5 && rec->slots[i].word != 0xc8) {
                char* t12 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x12);
                func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x254], t12, 0);
                char* t6 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x6);
                if (v2 == 1) {
                    sprintf(buf, &lbl_eu_8050DC20[0x25e], (u8)v3);
                } else {
                    sprintf(buf, &lbl_eu_8050DC20[0x261], (u8)v3, t6);
                }
                func_80136A1C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x160], buf, 0);
            } else {
                func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x254],
                              &lbl_eu_8050DC20[0x182], 0);
                func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x160],
                              &lbl_eu_8050DC20[0x182], 0);
            }
            found = 0;
            break;
        }
    }
    if (found != 0) {
        // All slots occupied: show the "no free slots" message instead.
        u8 v3 = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x266], idx);
        if (v3 != 0) {
            char* t12 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x12);
            func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x254], t12, 0);
            char* t6 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x6);
            if (v2 == 1) {
                sprintf(buf, &lbl_eu_8050DC20[0x25e], (u8)v3);
            } else {
                sprintf(buf, &lbl_eu_8050DC20[0x261], (u8)v3, t6);
            }
            func_80136A1C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x160], buf, 0);
        } else {
            func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x254],
                          &lbl_eu_8050DC20[0x182], 0);
            func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x160],
                          &lbl_eu_8050DC20[0x182], 0);
        }
    }
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x26b],
                  &lbl_eu_8050DC20[0x182], 0);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x16c],
                  &lbl_eu_8050DC20[0x182], 0);
    func_802665FC(reinterpret_cast<UI::CPassiveSkillLine*>(&self->mInfo), (u8)chId);
    func_80266724(reinterpret_cast<UI::CPassiveSkillLine*>(&self->mInfo), 0, row, 0);
}
#pragma optimize_for_size off

// +0x28 sub-object update used by the menu-close path (retail func_80267B08):
// set the menu state to 6, rebind the +0x8 layout's animation from the +0x14
// transform to the +0xC one, then step the embedded info cursor at +0x120 and
// push 0xff into the +0x3C info region. C linkage so the call reloc from
// func_8026DA88 is the plain retail name.
extern "C" __declspec(noinline) void func_80267B08(UI::CPassiveSkillLine* self) {
    self->field_E8 = 6;
    self->field_E9 = 0;
    self->field_8->UnbindAnimation(self->field_14);
    self->field_8->BindAnimation(self->field_C);
    self->field_8->SetAnimationEnable(self->field_C, 1);
    func_80264D28(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
    func_802640B8(&self->mInfo.field_3C, 0xff);
}

#pragma optimize_for_size on
// Menu-close line update (retail func_80267BA0): when the menu state is not
// the closed marker (15), refresh the +0xF3 countdown byte from the shared
// byte (increment, wrapping to 0 past the fresh value), trigger the info pane
// animation, clear the +0xF5/+0xF6 and +0xFD/+0xFE flags and run the line
// update steps. The stmw r30 frame retail uses needs optimize_for_size (same
// as the other stmw-frame helpers in this TU). noinline keeps the thunk's
// retail `b func_80267BA0` tail call.
__declspec(noinline) void func_80267BA0(UI::CPassiveSkillLine* self) {
    if (self->field_E8 == 0xF) {
        return;
    }
    u8 fresh = code80135FDC_getByte_64077();
    u8 v = (u8)(self->field_F3 + 1);
    self->field_F3 = v;
    if ((s8)v >= (u8)fresh) {
        self->field_F3 = 0;
    }
    func_8026440C(reinterpret_cast<UI_PassiveSkillInit*>(&self->mInfo._pad20));
    self->field_F5 = 0;
    self->field_F6 = 0;
    func_80269C08(self);
    func_80269D20(self);
    self->field_FD = 0;
    self->field_FE = 0;
    func_8026BB60(self);
    func_8026C4A4(self);
    playUISound(0xA);
}
#pragma optimize_for_size off

#pragma optimize_for_size on
// +0x28 sub-object update (retail func_80267C44): when the menu state is not
// the closed marker (15), count down the +0xF3 timer byte (refreshing it from
// the shared byte when it wraps below zero), trigger the info pane animation,
// clear the +0xF5/+0xF6 and +0xFD/+0xFE flags and run the line update steps.
// The stmw r30 frame retail uses needs optimize_for_size (same as the other
// stmw-frame helpers in this TU). C linkage so the call reloc from the +0x28
// thunk is plain.
extern "C" __declspec(noinline) void func_80267C44(UI::CPassiveSkillLine* self) {
    if (self->field_E8 == 0xF) {
        return;
    }
    u8 fresh = code80135FDC_getByte_64077();
    u8 remaining = (u8)(self->field_F3 - 1);
    self->field_F3 = remaining;
    if ((s8)remaining < 0) {
        self->field_F3 = (u8)(fresh - 1);
    }
    func_80264470(reinterpret_cast<UI_PassiveSkillInit*>(&self->mInfo._pad20));
    self->field_F5 = 0;
    self->field_F6 = 0;
    func_80269C08(self);
    func_80269D20(self);
    self->field_FD = 0;
    self->field_FE = 0;
    func_8026BB60(self);
    func_8026C4A4(self);
    playUISound(0xA);
}
#pragma optimize_for_size off

// Menu-cursor down (retail func_80267CE0): when the embedded syswin is armed
// and active, move the cursor to the previous slot (wrapping to 1); otherwise
// decrement the state machine's grid/row counters for states 5/0xC/0xF and
// play the UI SFX 1.
void func_80267CE0(UI::CPassiveSkillLine* self) {
    if (self->mInfo.field_54 != 0 && CSysWin_getUnk34(self->mInfo.field_54) != 0) {
        if (CSysWin_isActive(self->mInfo.field_54) != 0) {
            u8 v = (u8)(self->field_11D - 1);
            self->field_11D = v;
            if ((s8)v < 0) {
                self->field_11D = 1;
            }
            nw4r::math::VEC3 pos;
            func_8022C1B4(&pos, self->mInfo.field_54, self->field_11D);
            reinterpret_cast<CCur18View*>(&self->mCur)->vf04(&pos);
            playUISound(1);
        }
        return;
    }
    if (self->field_E8 == 5) {
        u8 v = (u8)(self->field_F6 - 1);
        self->field_F6 = v;
        if ((s8)v < 0) {
            self->field_F6 = 5;
        }
        func_8026C4A4(self);
    } else if (self->field_E8 == 0xC) {
        u8 v = (u8)(self->field_FE - 1);
        self->field_FE = v;
        if ((s8)v < 0) {
            self->field_FE = 4;
        }
        func_8026C4A4(self);
    } else if (self->field_E8 == 0xF) {
        u8 v = (u8)(self->field_F8 - 1);
        self->field_F8 = v;
        if ((s8)v < 0) {
            self->field_F8 = 4;
        }
        func_8026C4A4(self);
    }
    playUISound(1);
}

// Menu-cursor up (retail func_80267E2C): mirror of func_80267CE0 with
// incrementing counters; the +0x11D cursor selector wraps to 0 past 1, the
// state-5 counter past 5, and the 0xC/0xF counters past 4.
extern "C" __declspec(noinline) void func_80267E2C(u8* self) {
    UI::CPassiveSkillLine* line = reinterpret_cast<UI::CPassiveSkillLine*>(self);
    if (line->mInfo.field_54 != 0 && CSysWin_getUnk34(line->mInfo.field_54) != 0) {
        if (CSysWin_isActive(line->mInfo.field_54) != 0) {
            u8 v = (u8)(line->field_11D + 1);
            line->field_11D = v;
            if ((s8)v >= 2) {
                line->field_11D = 0;
            }
            nw4r::math::VEC3 pos;
            func_8022C1B4(&pos, line->mInfo.field_54, line->field_11D);
            reinterpret_cast<CCur18View*>(&line->mCur)->vf04(&pos);
            playUISound(1);
        }
        return;
    }
    if (line->field_E8 == 5) {
        u8 v = (u8)(line->field_F6 + 1);
        line->field_F6 = v;
        if ((s8)v >= 6) {
            line->field_F6 = 0;
        }
        func_8026C4A4(line);
    } else if (line->field_E8 == 0xC) {
        u8 v = (u8)(line->field_FE + 1);
        line->field_FE = v;
        if ((s8)v >= 5) {
            line->field_FE = 0;
        }
        func_8026C4A4(line);
    } else if (line->field_E8 == 0xF) {
        u8 v = (u8)(line->field_F8 + 1);
        line->field_F8 = v;
        if ((s8)v >= 5) {
            line->field_F8 = 0;
        }
        func_8026C4A4(line);
    }
    playUISound(1);
}

// Menu-cursor right / tab-next (retail func_80267F88): when the embedded
// syswin is armed (non-null and active) the caller already handled the nav,
// so return early; otherwise advance the state machine's tab/row selectors
// for states 5/0xC/0xF and play the UI SFX 1. The state selectors wrap via
// availability flags (+0xF9/+0xFA for state 5, +0xFB/+0xFC for state 0xF,
// and the +0xFD ladder against the +0xF2 count), mirroring the left/right
// menu-tab movement of func_80268250.
void func_80267F88(UI::CPassiveSkillLine* self) {
    CSysWin* syswin = self->mInfo.field_54;
    if (syswin != 0 && CSysWin_getUnk34(syswin) != 0) {
        return;
    }
    if (self->field_E8 == 5) {
        switch ((s8)self->field_F5) {
        case 0:
            self->field_F5 = 1;
            break;
        case 1:
            if (self->field_F9 != 0) {
                self->field_F5 = 3;
            } else if (self->field_FA != 0) {
                self->field_F5 = 4;
            } else {
                self->field_F5 = 2;
            }
            break;
        case 2:
            self->field_F5 = 0;
            break;
        case 3:
            if (self->field_FA != 0) {
                self->field_F5 = 4;
            } else {
                self->field_F5 = 2;
            }
            break;
        case 4:
            self->field_F5 = 2;
            break;
        }
        func_8026C4A4(self);
    } else if (self->field_E8 == 0xC) {
        u8 v = (u8)(self->field_F2 - 1);
        switch ((s8)self->field_FD) {
        case 0:
            if (v > 2) {
                self->field_FD = 2;
            } else if (v > 1) {
                self->field_FD = 1;
            }
            break;
        case 1:
            self->field_FD = 0;
            break;
        case 2:
            if (v > 4) {
                self->field_FD = 4;
            } else if (v > 3) {
                self->field_FD = 3;
            } else {
                self->field_FD = 1;
            }
            break;
        case 3:
            self->field_FD = 1;
            break;
        case 4:
            if (v > 5) {
                self->field_FD = 5;
            } else {
                self->field_FD = 3;
            }
            break;
        case 5:
            self->field_FD = 3;
            break;
        }
        func_8026C4A4(self);
    } else if (self->field_E8 == 0xF) {
        switch ((s8)self->field_F7) {
        case 0:
            self->field_F7 = 1;
            break;
        case 1:
            if (self->field_FB != 0) {
                self->field_F7 = 3;
            } else if (self->field_FC != 0) {
                self->field_F7 = 4;
            } else {
                self->field_F7 = 2;
            }
            break;
        case 2:
            self->field_F7 = 0;
            break;
        case 3:
            if (self->field_FC != 0) {
                self->field_F7 = 4;
            } else {
                self->field_F7 = 2;
            }
            break;
        case 4:
            self->field_F7 = 2;
            break;
        }
        func_8026C4A4(self);
    }
    playUISound(1);
}

// Menu-cursor left / tab-prev (retail func_80268250): mirror of
// func_80267F88 with decrementing tab selectors - state 5 moves through the
// +0xF5 selector via the +0xF9/+0xFA flags, state 0xC walks the +0xFD ladder
// down against the +0xF2 count, state 0xF moves the +0xF7 selector via the
// +0xFB/+0xFC flags; the line update tail runs per state and the UI SFX 1
// plays on every path except the syswin-armed early return.
__declspec(noinline) void func_80268250(UI::CPassiveSkillLine* self) {
    CSysWin* syswin = self->mInfo.field_54;
    if (syswin != 0 && CSysWin_getUnk34(syswin) != 0) {
        return;
    }
    if (self->field_E8 == 5) {
        switch ((s8)self->field_F5) {
        case 0:
            self->field_F5 = 2;
            break;
        case 1:
            self->field_F5 = 0;
            break;
        case 2:
            if (self->field_FA != 0) {
                self->field_F5 = 4;
            } else if (self->field_F9 != 0) {
                self->field_F5 = 3;
            } else {
                self->field_F5 = 1;
            }
            break;
        case 3:
            self->field_F5 = 1;
            break;
        case 4:
            if (self->field_F9 != 0) {
                self->field_F5 = 3;
            } else {
                self->field_F5 = 1;
            }
            break;
        }
        func_8026C4A4(self);
    } else if (self->field_E8 == 0xC) {
        u8 v = (u8)(self->field_F2 - 1);
        switch ((s8)self->field_FD) {
        case 0:
            if (v > 1) {
                self->field_FD = 1;
            }
            break;
        case 1:
            if (v > 3) {
                self->field_FD = 3;
            } else if (v > 2) {
                self->field_FD = 2;
            } else {
                self->field_FD = 0;
            }
            break;
        case 2:
            self->field_FD = 0;
            break;
        case 3:
            if (v > 5) {
                self->field_FD = 5;
            } else if (v > 4) {
                self->field_FD = 4;
            } else {
                self->field_FD = 2;
            }
            break;
        case 4:
            self->field_FD = 2;
            break;
        case 5:
            self->field_FD = 4;
            break;
        }
        func_8026C4A4(self);
    } else if (self->field_E8 == 0xF) {
        switch ((s8)self->field_F7) {
        case 0:
            self->field_F7 = 2;
            break;
        case 1:
            self->field_F7 = 0;
            break;
        case 2:
            if (self->field_FC != 0) {
                self->field_F7 = 4;
            } else if (self->field_FB != 0) {
                self->field_F7 = 3;
            } else {
                self->field_F7 = 1;
            }
            break;
        case 3:
            self->field_F7 = 1;
            break;
        case 4:
            if (self->field_FB != 0) {
                self->field_F7 = 3;
            } else {
                self->field_F7 = 1;
            }
            break;
        }
        func_8026C4A4(self);
    }
    playUISound(1);
}

// Menu-close dispatch for the +0x28 line sub-object: when the embedded syswin
// is armed (non-null and active) the caller already handled the close, so
// return early; otherwise pick the state-5 (func_80269808) or state-0xC
// (func_80269924) close path and play the shared UI SFX 109 on either.
__declspec(noinline) void func_80268518(UI::CPassiveSkillLine* self) {
    CSysWin* syswin = self->mInfo.field_54;
    if (syswin != 0 && CSysWin_getUnk34(syswin) != 0) {
        return;
    }
    if (self->field_E8 == 5) {
        func_80269808(self);
        playUISound(0x6d);
    } else if (self->field_E8 == 0xC) {
        func_80269924(self);
        playUISound(0x6d);
    }
}

// Callees for func_80268594 (definitions later in this TU unless noted).
// C linkage so the call relocs are the plain retail names.
using UI::CPSkillBlobCost;
using UI::CPSkillSlot;
extern "C" void func_8026CE30(UI::CPassiveSkillLine* self);
extern "C" u8 func_8026CC34(UI_CPassiveSkill* self);
extern "C" u8 func_8026CC58(UI::CPassiveSkillLine* self);
extern "C" u8 func_8026CD44(UI::CPassiveSkillLine* self);
extern "C" void func_80269A18(UI::CPassiveSkillLine* self);
extern "C" void func_80269B14(UI::CPassiveSkillLine* self);
extern "C" void func_80264F7C(UI::CPassiveSkillLine* self, int state, int id,
                            int row, int col, int r8);
// Kizuna-chart helpers (defined in CPcKizunagram.cpp) called with the
// character skill blob. func_8025F9AC's trailing argument registers are left
// unset by retail, so only the blob pointer is passed.
extern "C" void func_8025F9AC(void* self);
extern "C" void func_8025F768(void* self, int a, int b, int value);
extern "C" void func_80280E9C(u8* self);
extern "C" void func_8022B9B4(void*, const char*, int);
extern "C" void func_8022BF6C(void*, void*, void*);
extern "C" void func_8022B8B8(void*);

// Main input/update handler for the +0x28 skill-line sub-object (retail
// func_80268594). When the lazily-attached syswin is armed and active it
// consumes the pending purchase/learn confirmation (field_11D/11E select the
// buy vs learn-all flow), quiets the cursor and advances the window.
// Otherwise it dispatches on the menu state byte (field_E8):
//   5  - SP-total refresh gate,
//   12 - learn-check dispatch (func_8026CC58/CD44 -> confirm handler),
//   15 - confirm on the selected grid cell: push the acquisition messages
//        (item-category 0x85/0x84 variants) or mark the skill learned.
extern "C" __declspec(noinline) void func_80268594(u8* selfRaw) {
    UI::CPassiveSkillLine* self = reinterpret_cast<UI::CPassiveSkillLine*>(selfRaw);
    if (self->mInfo.field_54 != 0 && CSysWin_getUnk34(self->mInfo.field_54) != 0) {
        if (CSysWin_isActive(self->mInfo.field_54) != 0) {
            if ((s8)self->field_11D == 0) {
                func_8026CE30(self);
                if (self->field_11E == 0) {
                    // Buy flow: reload the selected cell and set the learned
                    // marker on the character's secondary record slot.
                    u8 ch = func_801392B4(self->field_F3);
                    u8 f4 = self->field_F4;
                    CPSkillBlobCost* blob = reinterpret_cast<CPSkillBlobCost*>(
                        (u8*)func_8009EC9C(ch) + 0x3534);
                    s8 fe = (s8)self->field_FE;
                    u8 row = lbl_eu_8050DB60[(ch - 1) * 8 + f4 - 1];
                    func_8025F768(blob, row, fe + 1,
                                  self->field_F8 + self->field_F7 * 5 +
                                      (f4 - 1) * 0x19 + 1);
                    CPSkillSlot* slot = &blob->records2[row].slots[fe + 1];
                    slot->byte14 |= 1;
                    func_80280E9C((u8*)blob);
                    func_80269B14(self);
                    playUISound(0x93);
                } else if (self->field_11E == 1) {
                    // Learn-all flow: walk the grid column entries, find the
                    // field_FD-th non-matching entry, and pay its SP cost.
                    u8 ch = func_801392B4(self->field_F3);
                    CPSkillBlobCost* blob = reinterpret_cast<CPSkillBlobCost*>(
                        (u8*)func_8009EC9C(ch) + 0x3534);
                    s8 fe = (s8)self->field_FE;
                    u8 f2 = self->field_F2;
                    u32 costLo = 0;
                    u32 freeFlag = 0;
                    u8 j = 0;
                    for (u8 i = 0; i < f2;) {
                        if (self->field_EA[i] != ch) {
                            if (j == self->field_FD) {
                                u8 row = lbl_eu_8050DB60[(ch - 1) * 8 + i];
                                CPSkillSlot* slot =
                                    &blob->records2[row].slots[fe + 1];
                                costLo = slot->word & 0xFF;
                                freeFlag = (slot->byte14 & 1) == 0;
                                if (freeFlag != 0 && costLo != 0) {
                                    self->field_11F = 0;
                                    func_8025F9AC(blob);
                                    slot->byte14 |= 1;
                                    func_80269D20(self);
                                    func_8026BB60(self);
                                    costLo = 0;
                                    freeFlag = (slot->byte14 & 1) == 0;
                                    playUISound(0x77);
                                }
                                break;
                            }
                            j++;
                            if (j >= 6) {
                                break;
                            }
                        }
                        i++;
                    }
                    func_80264F7C(
                        reinterpret_cast<UI::CPassiveSkillLine*>(&self->mInfo),
                        3, ch, costLo, freeFlag, self->field_FE);
                }
            } else {
                playUISound(6);
            }
            func_801D216C(&self->mCur, 0);
            func_8022B8E4(self->mInfo.field_54);
        }
        return;
    }
    if (self->field_E8 == 5) {
        // SP-summary refresh: if the stored total still matches the current
        // one there is nothing to update.
        u8 ch = func_801392B4(self->field_F3);
        CPSkillBlobCost* blob = reinterpret_cast<CPSkillBlobCost*>(
            (u8*)func_8009EC9C(ch) + 0x3534);
        if ((s8)self->field_F5 == (s32)blob->costs[5]) {
            playUISound(5);
            return;
        }
        func_8025EE7C(blob, (s8)self->field_F5);
        func_80269D20(self);
        playUISound(0x12);
        return;
    }
    if (self->field_E8 == 12) {
        if (func_8026CC58(self) != 0) {
            if (func_8026CD44(self) != 0) {
                func_80268C38(self);
                return;
            }
            func_80269A18(self);
            playUISound(3);
            return;
        }
        playUISound(5);
        return;
    }
    if (self->field_E8 != 15) {
        return;
    }
    if (func_8026CC34(reinterpret_cast<UI_CPassiveSkill*>(self)) == 0) {
        playUISound(5);
        return;
    }
    {
        u8 ch = func_801392B4(self->field_F3);
        u8 f4 = self->field_F4;
        CPSkillBlobCost* blob = reinterpret_cast<CPSkillBlobCost*>(
            (u8*)func_8009EC9C(ch) + 0x3534);
        s8 fe = (s8)self->field_FE;
        u8 skillId = lbl_eu_8050DB60[(ch - 1) * 8 + f4 - 1];
        CPSkillSlot* slot = &blob->records2[skillId].slots[fe + 1];
        if (slot->word != 0) {
            // Occupied slot: look up the acquisition message category and
            // either push the confirm dialogs or apply a free/unlearned
            // acquisition directly.
            u8 itemCat = func_801361E8((u32)lbl_eu_8066488C,
                                       &lbl_eu_8050DC20[0x235], slot->word);
            self->field_11F = itemCat;
            if (itemCat == 0x85 || itemCat == 0x84) {
                self->field_E8 = 0x10;
                self->field_11E = 0;
                char* msgA = func_80136190(&lbl_eu_8050DC20[0x21f],
                                           &lbl_eu_8050DC20[0x207],
                                           itemCat == 0x85 ? 0x1a : 0x1b);
                char* msgB = func_80136190(&lbl_eu_8050DC20[0x21f],
                                           &lbl_eu_8050DC20[0x207], 0x1c);
                char* msgC = func_80136190(&lbl_eu_8050DC20[0x21f],
                                           &lbl_eu_8050DC20[0x207], 0x1d);
                func_8022B9B4(self->mInfo.field_54, msgA, 0);
                func_8022BF6C(self->mInfo.field_54, msgB, msgC);
                func_8022BFC8(static_cast<CSysWin*>(self->mInfo.field_54), 0);
                func_8022B8B8(self->mInfo.field_54);
                func_801D216C(&self->mCur, 0);
                playUISound(3);
                return;
            }
            self->field_11F = 0;
            func_8025F768(blob, skillId, fe + 1,
                          self->field_F8 + self->field_F7 * 5 +
                              (f4 - 1) * 0x19 + 1);
            CPSkillSlot* slot2 = &blob->records2[skillId].slots[fe + 1];
            slot2->byte14 |= 1;
            func_80280E9C((u8*)blob);
            func_80269B14(self);
            playUISound(0x93);
            return;
        }
        // Empty slot: acquire without the message flow.
        self->field_11F = 0;
        func_8025F768(blob, skillId, fe + 1,
                      self->field_F8 + self->field_F7 * 5 +
                          (f4 - 1) * 0x19 + 1);
        slot->byte14 |= 1;
        func_80280E9C((u8*)blob);
        func_80269B14(self);
        playUISound(0x93);
    }
    return;
}

// +0x28 sub-object update used by func_8026DAD0's learned-skill path.
// Two entry conditions: with a lazily-attached syswin present and its +0x34
// query nonzero, an active window plays the close sound, quiets the embedded
// cursor and tears the syswin down; otherwise a menu in state 15 runs the
// state-15 helper and plays the same sound. extern C keeps the plain retail
// symbol; noinline keeps the retail `bl` from func_8026DAD0.
extern "C" __declspec(noinline) void func_80268BA8(u8* selfRaw) {
    UI::CPassiveSkillLine* self = reinterpret_cast<UI::CPassiveSkillLine*>(selfRaw);
    if (self->mInfo.field_54 != 0 && CSysWin_getUnk34(self->mInfo.field_54) != 0) {
        if (CSysWin_isActive(self->mInfo.field_54) != 0) {
            playUISound(6);
            func_801D216C(&self->mCur, 0);
            func_8022B8E4(self->mInfo.field_54);
        }
    } else {
        if (self->field_E8 == 15) {
            func_80269B14(self);
            playUISound(6);
        }
    }
}

// Callee declarations (retail names; also declared in CCol6System.hpp /
// CCollepedia.hpp / CPcKizunagram.hpp for their TUs).
extern "C" void func_8022B9B4(void*, const char*, int);
extern "C" void func_8022BF6C(void*, void*, void*);
extern "C" void func_8022B8B8(void*);
u8 func_8026D3CC(UI::CPassiveSkillLine* self, int id);

// Skill-acquisition confirm handler (retail func_80268C38): with the syswin
// present and its +0x34 query clear, requires menu state 12. Scans the grid's
// field_EA category entries for the selected character (field_F3 via
// func_801392B4), matches the +0xFD slot against the +0xFE category, and reads
// the learned bit / cost word from the character's learn record (row from
// lbl_eu_8066488C's table). On a learned entry: set state 16, push three
// messages, open the syswin close sequence and quiet the cursor. Category 133
// vs 132 variants select different message sets. Otherwise: clear the +0x287
// flag, run the state-15 helper, play the sounds and run the update tail.
// Callee declarations (retail names; also declared in CCol6System.hpp /
// CCollepedia.hpp / CPcKizunagram.hpp for their TUs).
extern "C" void func_8022B9B4(void*, const char*, int);
extern "C" void func_8022BF6C(void*, void*, void*);
extern "C" void func_8022B8B8(void*);
u8 func_8026D3CC(UI::CPassiveSkillLine* self, int id);

// Skill-acquisition confirm handler (retail func_80268C38): with the syswin
// present and its +0x34 query clear, requires menu state 12. Scans the grid's
// field_EA category entries for the selected character (field_F3 via
// func_801392B4), matches the +0xFD category slot against the +0xFE entry,
// and reads the learned bit / cost from the character's learn record row.
// Learned entries with SP cost: set state 16, push three messages, open the
// syswin close sequence and quiet the cursor (two message-set variants by
// item category 0x85/0x84). Unlearned or free: play a sound, run the state-15
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80268C38(void* selfRaw) {
    UI::CPassiveSkillLine* self = static_cast<UI::CPassiveSkillLine*>(selfRaw);
    if (self->mInfo.field_54 != 0) {
        if (CSysWin_getUnk34(self->mInfo.field_54) != 0) {
            return;
        }
    }
    if (self->field_E8 != 12) {
        return;
    }
    u8 ch = func_801392B4(self->field_F3);
    u8* learnBase = (u8*)func_8009EC9C(ch) + 0x3534;
    u8 catIdx = 0;
    for (u8 i7 = 0; i7 < self->field_F2;) {
        if (self->field_EA[i7] == ch) {
            i7++;
            continue;
        }
        if ((u8)catIdx != (u8)(s8)self->field_FD) {
            catIdx++;
            if (catIdx >= 6) {
                break;
            }
            continue;
        }
        u8 row = reinterpret_cast<const u8*>(lbl_eu_8066488C)[
            (u8)(ch - 1) * 8 + self->field_EA[i7] - 1];
        u8* entry = learnBase + row * 0xC4 + (((s8)self->field_FE + 1) << 5);
        u32 cost = *reinterpret_cast<const u32*>(entry + 0x3D4);
        u8 learnedBit = *(entry + 0x3E8) & 1;
        if (learnedBit == 0) {
            playUISound(5);
            return;
        }
        if (cost == 0) {
            playUISound(5);
            return;
        }
        u16 msgId =
            func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x235], cost);
        u8 itemCat = (u8)msgId;
        u8 count = func_8026D3CC(self, 0);
        char* msgA;
        char* msgB;
        char* msgC;
        if (itemCat == 0x85 && count <= 1) {
            self->field_E8 = 0x10;
            self->field_11E = 1;
            msgA = func_80136190(&lbl_eu_8050DC20[0x21f],
                                 &lbl_eu_8050DC20[0x207], 0x1a);
            msgB = func_80136190(&lbl_eu_8050DC20[0x21f],
                                 &lbl_eu_8050DC20[0x207], 0x1c);
            msgC = func_80136190(&lbl_eu_8050DC20[0x21f],
                                 &lbl_eu_8050DC20[0x207], 0x1d);
        } else if (itemCat == 0x84 && count <= 1) {
            self->field_E8 = 0x10;
            msgA = func_80136190(&lbl_eu_8050DC20[0x21f],
                                 &lbl_eu_8050DC20[0x207], 0x1b);
            msgB = func_80136190(&lbl_eu_8050DC20[0x21f],
                                 &lbl_eu_8050DC20[0x207], 0x1c);
            msgC = func_80136190(&lbl_eu_8050DC20[0x21f],
                                 &lbl_eu_8050DC20[0x207], 0x1d);
        } else {
            self->field_11F = 0;
            break;
        }
        func_8022B9B4(self->mInfo.field_54, msgA, 0);
        func_8022BF6C(self->mInfo.field_54, msgB, msgC);
        func_8022BFC8(static_cast<CSysWin*>(self->mInfo.field_54), 0);
        func_8022B8B8(self->mInfo.field_54);
        func_801D216C(&self->mCur, 0);
        break;
    }
}
#pragma optimize_for_size off
// +0x28 sub-object update used by func_8026DAD0's fallback path: when the
// lazily-attached CSysWin (mInfo.field_54) is armed and active, play the open
// SFX, quiet the embedded cursor, and advance the system window. noinline:
// retail keeps the `bl` (the body lives at 0x80268F7C).
extern "C" __declspec(noinline) void func_80268F7C(UI::CPassiveSkillLine* self) {
    if (self->mInfo.field_54 == 0) {
        return;
    }
    if (CSysWin_getUnk34(self->mInfo.field_54) == 0) {
        return;
    }
    if (CSysWin_isActive(self->mInfo.field_54) == 0) {
        return;
    }
    playUISound(6);
    func_801D216C(&self->mCur, 0);
    func_8022B8E4(self->mInfo.field_54);
}

extern "C" __declspec(noinline) void func_80268FEC(void* self){
    if (*(u8*)((u8*)self + 0x13A) != 0) {
        *(u8*)((u8*)self + 0xE8) = 2;
    }
}

// Menu-open line step (retail func_80269004): when the +0xC anim transform's
// frame check succeeds, move the menu to state 5, animate the +0x8 layout and
// swap its bound transform from +0xC to +0x14, run the line-close tail, then
// find two panes by name in the primary layout root, compute the position
// between them via func_80137924 and drive the embedded cursor's second
// layout root pane with it; the layout animation always advances and the
// +0x28 update tail runs. The _savegpr_29 helper frame retail uses needs
// optimize_for_size (3 callee-saved registers; -O4,p would emit individual
// stw pairs).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80269004(UI::CPassiveSkillLine* self) {
    if (advanceAnimTransform(self->field_C, lbl_eu_80668900) != 0) {
        self->field_E8 = 5;
        self->field_E9 = 1;
        self->field_8->Animate(0);
        self->field_8->UnbindAnimation(self->field_C);
        self->field_8->BindAnimation(self->field_14);
        self->field_8->SetAnimationEnable(self->field_14, 1);
        func_8026C4A4(self);
        nw4r::lyt::Pane* paneA =
            self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x497], true);
        nw4r::lyt::Pane* paneB =
            self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x4a5], true);
        func_80266250(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo),
                      passiveSkillVecView::func_80137924(paneA, paneB, self->field_8->GetRootPane()));
    }
    self->field_8->Animate(0);
    func_8026D080(self);
}
#pragma optimize_for_size off

// +0x28 sub-object update (retail func_8026916C): when the +0xC anim
// transform's frame check succeeds, set the menu state to 7, animate and
// unbind it; then always animate the +0x8 layout and run the line tail.
// C linkage so the emitted symbol is the plain retail name.
extern "C" __declspec(noinline) void func_8026916C(UI::CPassiveSkillLine* self) {
    if (func_80137510(self->field_C, lbl_eu_80668900) != 0) {
        self->field_E8 = 7;
        self->field_8->Animate(0);
        self->field_8->UnbindAnimation(self->field_C);
    }
    self->field_8->Animate(0);
    func_8026D080(self);
}

extern "C" __declspec(noinline) void func_80269200(UI_CPassiveSkill* self) {
    if (self->field_139 == 0) {
        return;
    }
    self->field_E8 = 0;
    self->field_E9 = 1;
}

// Skill-menu update (retail func_80269220): while the embedded info's frame
// float still matches the closing marker, run the line-close tail and push
// 0xff into the +0x3C info region; when the +0x10 anim transform's frame
// check succeeds and the embedded info's active flag is set, animate and
// unbind the +0x8 layout's transform, then run the state-9 line step; the
// layout animation always advances and the update tail runs.
extern "C" __declspec(noinline) void func_80269220(UI::CPassiveSkillLine* self) {
    f32 frame = self->mInfo.field_1C;
    if (lbl_eu_80668918 == frame) {
        func_8026C4A4(self);
        func_802640B8(&self->mInfo.field_3C, 0xff);
    }
    if (advanceAnimTransform(self->field_10, lbl_eu_80668900) != 0) {
        if (self->mInfo.field_19 != 0) {
            self->field_8->Animate(0);
            self->field_8->UnbindAnimation(self->field_10);
            func_802699A4(self);
        }
    }
    self->field_8->Animate(0);
    func_8026D210(self);
}

// +0x28 sub-object update (retail func_802692E0): state-machine case 6.
// Retail keeps a second copy of the func_802696D8 body: when the +0x1C anim
// transform's frame check succeeds and the embedded info's active flag is
// set, move the menu to state 0xC, animate and unbind the +0x18 layout's
// transform, then run the line update tail. C linkage so the emitted symbol
// is the plain retail name.
extern "C" __declspec(noinline) void func_802692E0(UI::CPassiveSkillLine* self) {
    if (advanceAnimTransform(self->field_1C, lbl_eu_80668900) != 0) {
        if (self->mInfo.field_19 != 0) {
            self->field_E8 = 0xC;
            self->field_E9 = 1;
            self->field_18->Animate(0);
            self->field_18->UnbindAnimation(self->field_1C);
            func_8026C4A4(self);
        }
    }
}

// +0x28 sub-object update (retail func_80269370): while the embedded info's
// frame float still matches the closing marker, quiet the line and push 0xff
// into the +0x3C info region; when the +0x1C anim transform's frame check
// succeeds and the info's active flag is set, animate/unbind and run the
// skill-menu state step. C linkage so the emitted symbol is the plain retail
// name.
extern "C" __declspec(noinline) void func_80269370(UI::CPassiveSkillLine* self) {
    f32 frame = self->mInfo.field_1C;
    if (lbl_eu_80668918 == frame) {
        func_8026C4A4(self);
        func_802640B8(&self->mInfo.field_3C, 0xff);
    }
    if (func_80137510(self->field_1C, lbl_eu_80668900) != 0) {
        if (self->mInfo.field_19 != 0) {
            self->field_18->Animate(0);
            self->field_18->UnbindAnimation(self->field_1C);
            func_802698B8(self);
        }
    }
}

// +0x28 sub-object update (retail func_80269410): when the +0x10 anim
// transform's frame check succeeds and the embedded info's active flag is
// set, move the menu to state 5, animate the +0x8 layout, unbind the +0x10
// transform, bind/enable the +0x14 one, then run the line-close tail; the
// layout animation always advances and the update tail runs.
extern "C" __declspec(noinline) void func_80269410(UI::CPassiveSkillLine* self) {
    if (func_80137510(self->field_10, lbl_eu_80668900) != 0 && self->mInfo.field_19 != 0) {
        self->field_E8 = 5;
        self->field_E9 = 1;
        self->field_8->Animate(0);
        self->field_8->UnbindAnimation(self->field_10);
        self->field_8->BindAnimation(self->field_14);
        self->field_8->SetAnimationEnable(self->field_14, 1);
        func_8026C4A4(self);
    }
    self->field_8->Animate(0);
    func_8026D210(self);
}

// Skill-menu update (retail func_802694F4): while the embedded info's frame
// float sits in the open-window band (>= +0x1C marker and <= +0x20 marker),
// reset the skill grid row/col bytes and run the line close steps; when the
// +0x1C anim transform's frame check succeeds and the embedded info's active
// flag is set, animate and unbind the +0x18 layout's transform, then run the
// state-14 line step.
extern "C" __declspec(noinline) void func_802694F4(UI::CPassiveSkillLine* self) {
    f32 frame = self->mInfo.field_1C;
    if (frame >= lbl_eu_8066891C && frame <= lbl_eu_80668920) {
        self->field_F7 = 0;
        self->field_F8 = 0;
        func_8026AAF4(self);
        func_8026C4A4(self);
        func_802640B8(&self->mInfo.field_3C, 0xff);
    }
    if (func_80137510(self->field_1C, lbl_eu_80668900) != 0) {
        if (self->mInfo.field_19 != 0) {
            self->field_18->Animate(0);
            self->field_18->UnbindAnimation(self->field_1C);
            func_80269B94(self);
        }
    }
}

// +0x28 sub-object update (retail func_802695A8): state-machine case for the
// +0x10 secondary anim transform: when its frame check succeeds and the
// embedded info's active flag is set, move the menu to state 0xF, animate and
// unbind the +0x8 primary layout's transform, then run the line update tail.
// C linkage so the emitted symbol is the plain retail name.
extern "C" __declspec(noinline) void func_802695A8(UI::CPassiveSkillLine* self) {
    if (func_80137510(self->field_10, lbl_eu_80668900) != 0) {
        if (self->mInfo.field_19 != 0) {
            self->field_E8 = 0xF;
            self->field_E9 = 1;
            self->field_8->Animate(0);
            self->field_8->UnbindAnimation(self->field_10);
            func_8026C4A4(self);
        }
    }
}

// +0x28 sub-object update: while the embedded info's frame float matches the
// closing marker, run the line-close tail and push 0xff into the +0x3C info
// region; when the +0x10 anim transform's frame check succeeds and the
// embedded info's active flag is set, animate the +0x8 layout, bind the
// +0x10 transform, and run the state-0xB line step.
extern "C" __declspec(noinline) void func_80269638(UI::CPassiveSkillLine* self) {
    f32 frame = self->mInfo.field_1C;
    if (lbl_eu_80668918 == frame) {
        func_8026C4A4(self);
        func_802640B8(&self->mInfo.field_3C, 0xff);
    }
    if (advanceAnimTransform(self->field_10, lbl_eu_80668900) != 0) {
        if (self->mInfo.field_19 != 0) {
            self->field_8->Animate(0);
            self->field_8->UnbindAnimation(self->field_10);
            func_80269A98(self);
        }
    }
}

// +0x28 sub-object update (retail func_802696D8): when the +0x1C anim
// transform's frame check succeeds and the embedded info's active flag is
// set, move the menu to state 0xC, animate and unbind the +0x18 layout's
// transform, then run the line update tail. C linkage so the emitted symbol
// is the plain retail name.
extern "C" __declspec(noinline) void func_802696D8(UI::CPassiveSkillLine* self) {
    if (advanceAnimTransform(self->field_1C, lbl_eu_80668900) != 0) {
        if (self->mInfo.field_19 != 0) {
            self->field_E8 = 0xC;
            self->field_E9 = 1;
            self->field_18->Animate(0);
            self->field_18->UnbindAnimation(self->field_1C);
            func_8026C4A4(self);
        }
    }
}

// Menu-close path for the +0x28 line: when the lazily-attached syswin
// (mInfo.field_54) is active, pick the close state from the +0x11E selector
// byte, mark +0x11D, build the cursor target position from the syswin and
// move the embedded CCur18 cursor there, then quiet the cursor.
extern "C" __declspec(noinline) void func_80269768(UI::CPassiveSkillLine* self) {
    if (self->mInfo.field_54 != 0 && CSysWin_isActive(self->mInfo.field_54) != 0) {
        if (self->field_11E == 1) {
            self->field_E8 = 0xC;
        } else {
            self->field_E8 = 0xF;
        }
        self->field_11D = 1;
        nw4r::math::VEC3 pos;
        func_8022C1B4(&pos, self->mInfo.field_54, 1);
        reinterpret_cast<CCur18View*>(&self->mCur)->vf04(&pos);
        func_801D216C(&self->mCur, 1);
    }
}

// Menu-close state-5 helper (called by func_80268518). C linkage so the call
// reloc is the plain retail `func_80269808`; noinline keeps the retail `bl`
// (the real body lives at 0x80269808, not yet matched).
// Menu-close state-8 helper (called by func_80268518's state-5 dispatch and
// the +0x28 state machine): set the menu state to 8, clear +0xE9, animate the
// +0x8 layout, then unbind the +0x14 transform and bind/enable the +0x10 one;
// step the embedded skill-info cursor at +0x120 and push 0xff into the +0x3C
// info region. C linkage so the call relocs from func_80268518 are the plain
// retail `func_80269808`; noinline keeps the retail `bl`.
extern "C" __declspec(noinline) void func_80269808(UI::CPassiveSkillLine* self) {
    self->field_E8 = 8;
    self->field_E9 = 0;
    self->field_8->Animate(0);
    self->field_8->UnbindAnimation(self->field_14);
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_80264D98(reinterpret_cast<UI_PassiveSkillStep*>(&self->mInfo));
    func_802640B8(&self->mInfo.field_3C, 0xff);
}

// Skill-menu state step: set the +0xE8 menu-state byte to 4, bind and enable
// the +0x10 animation transform on the +0x8 layout, then refresh the skill
// name text on the embedded cursor at +0x120 (layout-compatible with
// CPassiveSkillCur: Layout* at +0x8). noinline: func_80269370 keeps the
// retail `bl`.
__declspec(noinline) void func_802698B8(UI::CPassiveSkillLine* self) {
    self->field_E8 = 4;
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_802661A8(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
}

// Skill-menu state step: set the +0xE8 menu-state byte to 3, clear +0xE9,
// bind/enable the +0x1C animation transform on the +0x18 layout, then step
// the embedded skill-info cursor at +0x120 (UI_PassiveSkillStep view) and push
// 0xff into the +0x3C info region via func_802640B8. C linkage so
// func_80268518's `bl func_80269924` binds to the plain retail name; noinline
// keeps that `bl` (MWCC would otherwise inline this body into func_80268518).
extern "C" __declspec(noinline) void func_80269924(UI::CPassiveSkillLine* self) {
    self->field_E8 = 3;
    self->field_E9 = 0;
    self->field_18->BindAnimation(self->field_1C);
    self->field_18->SetAnimationEnable(self->field_1C, 1);
    func_80264E04(reinterpret_cast<UI_PassiveSkillStep*>(&self->mInfo));
    func_802640B8(&self->mInfo.field_3C, 0xff);
}

// Line state step: set the +0xE8 menu-state byte to 9, bind and enable the
// +0x1C animation transform on the +0x18 layout, refresh the skill-name pane
// on the embedded cursor at +0x120, then run the line update tail. noinline:
// func_80269220 keeps the retail `bl`.
__declspec(noinline) void func_802699A4(UI::CPassiveSkillLine* self) {
    self->field_E8 = 9;
    self->field_18->BindAnimation(self->field_1C);
    self->field_18->SetAnimationEnable(self->field_1C, 1);
    func_802661FC(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mInfo));
    func_8026BB60(self);
}

// Skill-menu state step: same shape as func_80269924 but with menu state 0xD.
__declspec(noinline) void func_80269A18(UI::CPassiveSkillLine* self) {
    self->field_E8 = 0xD;
    self->field_E9 = 0;
    self->field_18->BindAnimation(self->field_1C);
    self->field_18->SetAnimationEnable(self->field_1C, 1);
    func_80264E04(reinterpret_cast<UI_PassiveSkillStep*>(&self->mInfo));
    func_802640B8(&self->mInfo.field_3C, 0xff);
}

// Skill-menu state step: set the +0xE8 menu-state byte to 0xB, bind/enable
// the +0x1C animation transform on the +0x18 layout, then run the skill-info
// update, the state-0xB helper and the line update tail. C linkage so the
// call reloc from func_80269638 is the plain retail name; noinline keeps the
// retail `bl`.
extern "C" __declspec(noinline) void func_80269A98(UI::CPassiveSkillLine* self) {
    self->field_E8 = 0xB;
    self->field_18->BindAnimation(self->field_1C);
    self->field_18->SetAnimationEnable(self->field_1C, 1);
    func_802660EC(&self->mInfo);
    func_80269D20(self);
    func_8026BB60(self);
}

// Skill-menu state step: set the +0xE8 menu-state byte to 0xA, clear +0xE9,
// bind/enable the +0x10 animation transform on the +0x8 layout, then step the
// embedded skill-info cursor at +0x120 (state-6 step) and push 0xff into the
// +0x3C info region via func_802640B8.
// noinline: retail keeps the `bl` from func_80268BA8 (state-15 path).
__declspec(noinline) void func_80269B14(UI::CPassiveSkillLine* self) {
    self->field_E8 = 0xA;
    self->field_E9 = 0;
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_80264D98(reinterpret_cast<UI_PassiveSkillStep*>(&self->mInfo));
    func_802640B8(&self->mInfo.field_3C, 0xff);
}

// Skill-menu state step: set the +0xE8 menu-state byte to 14, bind and enable
// the +0x10 animation transform on the +0x8 layout, then run the skill-info
// update and the line update tail. noinline: func_802694F4 keeps the retail
// `bl` (the body is byte-matched but must stay a separate function).
__declspec(noinline) void func_80269B94(UI::CPassiveSkillLine* self) {
    self->field_E8 = 14;
    self->field_8->BindAnimation(self->field_10);
    self->field_8->SetAnimationEnable(self->field_10, 1);
    func_802660EC(&self->mInfo);
    func_8026AAF4(self);
}

// Line update step (retail func_80269C08): refresh the skill-name textures.
// Walk the +0xF3 counter to its two neighbouring grid slots (clamping at the
// shared byte's bounds), resolve the current character's name texture from the
// BDAT table and attach it to the +0x4bf/+0x4cb primary and +0x4d7 secondary
// layout panes, then update the embedded info's two skill-name panes. The five
// callee-saved registers (r27-r31) need the stmw/lmw frame retail uses, which
// -O4,p only emits under optimize_for_size.
#pragma optimize_for_size on
__declspec(noinline) void func_80269C08(UI::CPassiveSkillLine* self) {
    u8 v = self->field_F3;
    s8 up = (s8)(v + 1);
    s8 down = (s8)(v - 1);
    if (up >= code80135FDC_getByte_64077()) {
        up = 0;
    }
    if (down < 0) {
        down = (s8)(code80135FDC_getByte_64077() - 1);
    }
    u8 id = func_801392B4(v);
    u8 idUp = func_801392B4((u8)up);
    u8 idDown = func_801392B4((u8)down);
    u16 msgId = func_80136254(lbl_eu_80664090, &lbl_eu_8050DC20[0x4b4], id);
    char* text = func_80138F78(msgId);
    u32 resU = (u32)(reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)
                         ->GetResource(0x74696d67, text, 0));
    if (resU != 0) {
        void* res = (void*)resU;
        func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x4bf], res);
        func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x4cb], res);
        func_80137E7C(self->field_18, &lbl_eu_8050DC20[0x4d7], res);
    }
    func_80264E70(&self->mInfo, idUp, idDown);
}
#pragma optimize_for_size off

// State-0xB line helper (called by func_80269A98). C linkage so the call reloc
// is the plain retail `func_80269D20`; noinline keeps the retail `bl` (the
// real body lives at 0x80269D20, not yet matched).
// Skill-grid tab update (retail func_80269D20): show/hide the two skill-row
// tabs and their pane slots, set the row-availability flags from the two
// BDAT lookups, then for each grid cell pick the icon/name textures by the
// slot category ladder, drive the cell layout animation frame from the SP
// cost ratio, and show the pane matching the category.
extern "C" __declspec(noinline) void func_80269D20(UI::CPassiveSkillLine* self) {
    // Shared 0x4330-magic conversion slots: declared first so MWCC hoists
    // the 0x43300000 high-word stores into the prologue (retail emits the
    // lis between the first self->field_8 load and the self spill).
    CPSkillF64Conv convA;
    CPSkillF64Conv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;
    char bufTab[0x20];
    char buf[0x20];
    func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x48b], true), 0);
    func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x497], true), 1);
    u8 id = func_801392B4(self->field_F3);
    for (u8 i = 1; i <= 5; i++) {
        sprintf(bufTab, &lbl_eu_8050DC20[0x4e3], id, i);
        void* res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, bufTab, 0);
        if (res != 0) {
            sprintf(bufTab, &lbl_eu_8050DC20[0x500], i);
            func_80137E7C(self->field_8, bufTab, res);
        }
    }
    self->field_F9 = 0;
    self->field_FA = 0;
    u32 t16 = (u32)((u8)id - 1);
    // Retail indexes the BDAT table words by the entry number (byte-scaled
    // indexed lwz off the sda21 base) instead of passing the table base.
    int idxA = (int)(t16 * 5 + 4);
    int idxB = (int)(id * 5);
    u8 v = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x50c], (u8)idxA);
    if (func_8009CF8C((u8)v + 0x3509) != 0) {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x511], true), 1);
        void* res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x51b], 0);
        if (res != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x531], res);
        }
        for (u8 i = 0; i < 5; i++) {
            func_80124270(self->cells[3][i].mpLayout->GetRootPane(), 1);
        }
        self->field_F9 = 1;
    } else {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x511], true), 0);
        void* res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x53b], 0);
        if (res != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x531], res);
        }
        for (u8 i = 0; i < 5; i++) {
            func_80124270(self->cells[3][i].mpLayout->GetRootPane(), 0);
        }
    }
    u8 v2 = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x50c], (u8)idxB);
    if (func_8009CF8C((u8)v2 + 0x3509) != 0) {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x551], true), 1);
        void* res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x55b], 0);
        if (res != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x571], res);
        }
        for (u8 i = 0; i < 5; i++) {
            func_80124270(self->cells[4][i].mpLayout->GetRootPane(), 1);
        }
        self->field_FA = 1;
    } else {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x551], true), 0);
        void* res = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x57b], 0);
        if (res != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x571], res);
        }
        for (u8 i = 0; i < 5; i++) {
            func_80124270(self->cells[4][i].mpLayout->GetRootPane(), 0);
        }
    }
    CPSkillCharData3DD0* data = reinterpret_cast<CPSkillCharData3DD0*>(func_8009EC9C(id));
    u32 cur = data->field_3DD0;
    UI::CPSkillBlob* blob = reinterpret_cast<UI::CPSkillBlob*>((u8*)data + 0x3534);
    for (u8 row = 0; row < 5; row++) {
        sprintf(buf, &lbl_eu_8050DC20[0x591], row + 1);
        nw4r::lyt::Pane* paneA = self->field_8->GetRootPane()->FindPaneByName(buf, true);
        sprintf(buf, &lbl_eu_8050DC20[0x59f], row + 1);
        nw4r::lyt::Pane* paneB = self->field_8->GetRootPane()->FindPaneByName(buf, true);
        u32 isCur = (u32)(cur - (u32)row) == 0;
        func_80124270(paneA, isCur);
        func_80124270(paneB, isCur);
    }
    u32 gridBase = t16 * 25;
    for (u8 row = 0; row < 5; row++) {
        u8 rowP1 = row + 1;
        u32 rowIdxBase = row * 5 + gridBase;
        UI::CPSkillRecord* rec = &blob->records[row];
        UI::CPSkillBlobCost* costBlob = reinterpret_cast<UI::CPSkillBlobCost*>(blob);
        for (u8 i = 1; i <= 5; i++) {
            int gridIdx = i + rowIdxBase;
            u8 v1 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x5ad], (u8)gridIdx);
            u8 v2b = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x5b3], (u8)gridIdx);
            u8 v3 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x344], (u8)gridIdx);
            u32 word = rec->slots[i].word;
            u16 value = (u16)(v3 * 0x64);
            void* res2 = 0;
            if (word == 0) {
                switch (v1) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x5ba], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x5d3], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x5ec], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x605], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x61e], 0); break;
                }
            } else if (v2b == 1) {
                switch (v1) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x637], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x650], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x669], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x682], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x69b], 0); break;
                }
            } else {
                switch (v1) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6b4], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6cd], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6e6], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6ff], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x718], 0); break;
                }
            }
            if (res2 != 0) {
                sprintf(buf, &lbl_eu_8050DC20[0x431], rowP1, i);
                func_80137E7C(self->field_8, buf, res2);
            }
            u16 msgId = func_80136254(lbl_eu_80664880, &lbl_eu_8050DC20[0x731], (u8)gridIdx);
            char* text = func_80138F78(msgId);
            void* res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, text, 0);
            if (res3 != 0) {
                sprintf(buf, &lbl_eu_8050DC20[0x736], rowP1, i);
                func_80137E7C(self->field_8, buf, res3);
                CPSkillFourShorts fs0 = func_80139658(self->field_8, buf, 0);
                CPSkillFourShorts fs1 = fs0;
                CPSkillFourShorts fs2 = func_80139658(self->field_8, buf, 1);
                CPSkillFourShorts fs3 = fs2;
                // Each branch declares its own colour pair so MWCC gives
                // them distinct stack slots (retail r1+0x20/0x18 vs
                // r1+0x10/0x8).
                if (word == 0) {
                    CPSkillColorS10 c1, c2;
                    CPSkillColorS10* col1 = func_801C4B60(&c1, 0xf0, 0xf0, 0xeb, fs3.d);
                    CPSkillColorS10* col2 = func_801C4B60(&c2, 0xf0, 0xf0, 0xeb, fs1.d);
                    func_80139A18(self->field_8, buf, col2, col1);
                } else {
                    CPSkillColorS10 c1, c2;
                    CPSkillColorS10* col1 = func_801C4B60(&c1, 0x2a, 0x22, 0x18, fs3.d);
                    CPSkillColorS10* col2 = func_801C4B60(&c2, 0x2a, 0x22, 0x18, fs1.d);
                    func_80139A18(self->field_8, buf, col2, col1);
                }
            }
            sprintf(buf, &lbl_eu_8050DC20[0x749], rowP1, i);
            func_80124270(self->field_8->GetRootPane()->FindPaneByName(buf, true), 0);
            nw4r::lyt::Layout* cellLayout = self->cells[row][i - 1].mpLayout;
            nw4r::lyt::AnimTransform* anim = self->cells[row][i - 1].mpAnimTrans;
            anim->SetFrame(lbl_eu_80668904);
            cellLayout->Animate(0);
            if (word == 0) {
                if (i == 1) {
                    // Retail converts cost/value through the 0x4330-magic
                    // doubles (signed xoris trick for cost, plain u16 for
                    // value) instead of direct int->float casts.
                    s32 cost = (s32)costBlob->costs[(u8)row];
                    convB.w[1] = value;
                    convA.w[1] = (u32)cost ^ 0x80000000u;
                    f64 ratio;
                    if (cost == 0) {
                        ratio = lbl_eu_80668904;
                    } else {
                        ratio = (convA.d - lbl_eu_80668928) /
                                (convB.d - lbl_eu_80668910);
                    }
                    convA.w[1] = (u32)(u16)anim->GetFrameSize();
                    f32 f = ((convA.d - lbl_eu_80668910) - lbl_eu_80668900) * ratio;
                    anim->SetFrame(f);
                    cellLayout->Animate(0);
                } else if (rec->slots[i - 1].word != 0) {
                    s32 cost = (s32)costBlob->costs[(u8)row];
                    convB.w[1] = value;
                    convA.w[1] = (u32)cost ^ 0x80000000u;
                    f64 ratio;
                    if (cost == 0) {
                        ratio = lbl_eu_80668904;
                    } else {
                        ratio = (convA.d - lbl_eu_80668928) /
                                (convB.d - lbl_eu_80668910);
                    }
                    convA.w[1] = (u32)(u16)anim->GetFrameSize();
                    f32 f = ((convA.d - lbl_eu_80668910) - lbl_eu_80668900) * ratio;
                    anim->SetFrame(f);
                    cellLayout->Animate(0);
                }
            } else {
                convA.w[1] = (u32)(u16)anim->GetFrameSize();
                f32 f = (convA.d - lbl_eu_80668910) - lbl_eu_80668900;
                anim->SetFrame(f);
                cellLayout->Animate(0);
            }
            func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x446], true), 0);
            func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x451], true), 0);
            func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x45c], true), 0);
            func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x467], true), 0);
            func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x472], true), 0);
            switch (v1) {
            case 1: func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x446], true), 1); break;
            case 2: func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x451], true), 1); break;
            case 3: func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x45c], true), 1); break;
            case 4: func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x467], true), 1); break;
            case 5: func_80124270(cellLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x472], true), 1); break;
            }
        }
    }
}

// Skill-learn grid update (retail func_8026AAF4): scan the column entries
// for the row marker to select the active skill id, refresh the name texture
// and the five tab slots, set the row-availability flags, then for each grid
// cell attach the category icon/name textures and drive the pane visibility
// from the learned state and the remaining-SP affordability check.
__declspec(noinline) void func_8026AAF4(UI::CPassiveSkillLine* self) {
    func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x48b], true), 1);
    func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x497], true), 0);
    // Buffer size/order and statement order matter: MWCC lays the two 0x20
    // scratch buffers at sp+0xa8/sp+0xc8 inside the 0x140 frame, and the
    // entry2 -> catBase -> v -> cnt sequence fixes the r16/r14/r17/r6 alloc.
    char bufB[0x20];
    char bufA[0x20];
    u8 id = func_801392B4(self->field_F3);
    u8 entry2 = id;
    u32 catBase = ((u32)((u8)id - 1) & 0x1F) * 8;
    u8 v = 0;
    u8 cnt = 0;
    for (u8 idx = 0; idx < self->field_F2; idx++) {
        u8 entry = self->field_EA[idx];
        if (entry == id) {
            continue;
        }
        if ((u8)cnt == (s8)self->field_FD) {
            entry2 = entry;
            for (u8 i = 1; i <= 5; i++) {
                if ((u8)i == (s8)self->field_FE + 1) {
                    sprintf(bufB, &lbl_eu_8050DC20[0x772], i);
                    v = func_801361E8((u32)lbl_eu_80664888, bufB, catBase + entry);
                    break;
                }
            }
            break;
        }
        cnt++;
        if (cnt >= 6) {
            break;
        }
    }
    self->field_F4 = entry2;
    u16 msgId = func_80136254(lbl_eu_80664090, &lbl_eu_8050DC20[0x1e8], entry2);
    char* text = func_80138F78(msgId);
    void* res = func_801355F4()->GetResource(0x74696d67, text, 0);
    if (res != 0) {
        func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x781], res);
    }
    for (u8 i = 1; i <= 5; i++) {
        sprintf(bufA, &lbl_eu_8050DC20[0x4e3], entry2, i);
        void* res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, bufA, 0);
        if (res2 != 0) {
            sprintf(bufA, &lbl_eu_8050DC20[0x500], i);
            func_80137E7C(self->field_8, bufA, res2);
        }
    }
    self->field_FB = 0;
    self->field_FC = 0;
    u32 t16 = (u32)((u8)entry2 - 1);
    u8 r0 = (u8)(t16 * 5 + 4);
    u8 r19 = (u8)(entry2 * 5);
    u8 vv = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x50c], r0);
    if (func_8009CF8C((u8)vv + 0x3509) != 0) {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x511], true), 1);
        void* res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x51b], 0);
        if (res3 != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x531], res3);
        }
        self->field_FB = 1;
    } else {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x511], true), 0);
        void* res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x53b], 0);
        if (res3 != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x531], res3);
        }
    }
    u8 vv2 = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x50c], r19);
    if (func_8009CF8C((u8)vv2 + 0x3509) != 0) {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x551], true), 1);
        void* res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x55b], 0);
        if (res3 != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x571], res3);
        }
        self->field_FC = 1;
    } else {
        func_80124270(self->field_8->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x551], true), 0);
        void* res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x57b], 0);
        if (res3 != 0) {
            func_80137E7C(self->field_8, &lbl_eu_8050DC20[0x571], res3);
        }
    }
    UI::CPSkillBlob* blob = reinterpret_cast<UI::CPSkillBlob*>(
        (u8*)func_8009EC9C((u8)entry2) + 0x3534);
    for (u8 row = 0; row < 5; row++) {
        sprintf(bufB, &lbl_eu_8050DC20[0x591], row + 1);
        nw4r::lyt::Pane* paneA = self->field_8->GetRootPane()->FindPaneByName(bufB, true);
        sprintf(bufB, &lbl_eu_8050DC20[0x59f], row + 1);
        nw4r::lyt::Pane* paneB = self->field_8->GetRootPane()->FindPaneByName(bufB, true);
        func_80124270(paneA, 0);
        func_80124270(paneB, 0);
    }
    u32 gridBase = (u32)((u8)entry2 - 1) * 0x19;
    u8 v0 = (u8)v;
    for (u8 row = 0; row < 5; row++) {
        u8 rowP1 = row + 1;
        u32 rowIdxBase = row * 5;
        UI::CPSkillRecord* rec = &blob->records[row];
        for (u8 i = 1; i <= 5; i++) {
            u32 gridIdx = (u8)(i + rowIdxBase + gridBase);
            u8 v1 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x5ad], gridIdx);
            u8 v2 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x5b3], gridIdx);
            func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x344], gridIdx);
            u32 word = rec->slots[i].word;
            void* res2 = 0;
            if (word == 0) {
                switch (v1) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x5ba], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x5d3], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x5ec], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x605], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x61e], 0); break;
                }
            } else if (v2 == 1) {
                switch (v1) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x637], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x650], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x669], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x682], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x69b], 0); break;
                }
            } else {
                switch (v1) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6b4], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6cd], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6e6], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6ff], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x718], 0); break;
                }
            }
            if (res2 != 0) {
                sprintf(bufB, &lbl_eu_8050DC20[0x431], rowP1, i);
                func_80137E7C(self->field_8, bufB, res2);
            }
            u16 msgId2 = func_80136254(lbl_eu_80664880, &lbl_eu_8050DC20[0x731], gridIdx);
            char* text2 = func_80138F78(msgId2);
            void* res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, text2, 0);
            if (res3 != 0) {
                sprintf(bufB, &lbl_eu_8050DC20[0x736], rowP1, i);
                func_80137E7C(self->field_8, bufB, res3);
                CPSkillFourShorts fs0 = func_80139658(self->field_8, bufB, 0);
                CPSkillFourShorts fs1 = fs0;
                CPSkillFourShorts fs2 = func_80139658(self->field_8, bufB, 1);
                CPSkillFourShorts fs3 = fs2;
                CPSkillColorS10 c1, c2;
                if (word == 0) {
                    CPSkillColorS10* col1 = func_801C4B60(&c1, 0xf0, 0xf0, 0xeb, fs3.d);
                    CPSkillColorS10* col2 = func_801C4B60(&c2, 0xf0, 0xf0, 0xeb, fs1.d);
                    func_80139A18(self->field_8, bufB, col2, col1);
                } else {
                    CPSkillColorS10* col1 = func_801C4B60(&c1, 0x2a, 0x22, 0x18, fs3.d);
                    CPSkillColorS10* col2 = func_801C4B60(&c2, 0x2a, 0x22, 0x18, fs1.d);
                    func_80139A18(self->field_8, bufB, col2, col1);
                }
            }
            sprintf(bufB, &lbl_eu_8050DC20[0x749], rowP1, i);
            nw4r::lyt::Pane* pane6 = self->field_8->GetRootPane()->FindPaneByName(bufB, true);
            func_80124270(pane6, (u32)(word == 0));
            void* res4 = 0;
            switch (v1) {
            case 1: res4 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x78d], 0); break;
            case 2: res4 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x7a2], 0); break;
            case 3: res4 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x7b7], 0); break;
            case 4: res4 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x7cc], 0); break;
            case 5: res4 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x7e1], 0); break;
            }
            if (res4 != 0) {
                nw4r::lyt::Pane* pane2 = self->field_8->GetRootPane()->FindPaneByName(bufB, true);
                if (pane2 != 0) {
                    func_80137E7C(self->field_8, bufB, res4);
                    CPSkillFourShorts fs0 = func_80139658(self->field_8, bufB, 0);
                    CPSkillFourShorts fs1 = fs0;
                    CPSkillFourShorts fs2 = func_80139658(self->field_8, bufB, 1);
                    CPSkillFourShorts fs3 = fs2;
                    CPSkillColorS10 c1, c2;
                    CPSkillColorS10* col1 = func_801C4B60(&c1, 0, 0, 0, fs3.d);
                    CPSkillColorS10* col2 = func_801C4B60(&c2, 0, 0, 0, fs1.d);
                    func_80139AC8(pane2, col2, col1);
                    reinterpret_cast<CPSkillPaneB8*>(pane2)->field_B8 = 0xa0;
                }
            }
            sprintf(bufB, &lbl_eu_8050DC20[0x75d], rowP1, i);
            nw4r::lyt::Pane* pane3 = self->field_8->GetRootPane()->FindPaneByName(bufB, true);
            func_80124270(pane3, (u32)(v0 == v1));
            u8 id3 = func_801392B4(self->field_F3);
            u8 id2b = self->field_F4;
            u8* data3 = (u8*)func_8009EC9C(id3);
            u8 rowR = lbl_eu_8050DB60[(u8)id3 * 8 + (u8)id2b - 9];
            u32* cellWords = reinterpret_cast<u32*>(data3 + rowR * 0xc4 + 0x3908);
            u8 found = 0;
            for (u8 i2 = 1; i2 <= 5; i2++) {
                if (cellWords[i2 * 8] == gridIdx) {
                    func_80124270(self->field_8->GetRootPane()->FindPaneByName(bufB, true), 1);
                    found = 1;
                    break;
                }
            }
            self->field_104[row * 5 + i - 1] = 0;
            if (func_801C4648(pane3) != 0) {
                sprintf(bufB, &lbl_eu_8050DC20[0x75d], rowP1, i);
                nw4r::lyt::Pane* pane5 = self->field_8->GetRootPane()->FindPaneByName(bufB, true);
                if (found != 0) {
                    void* res5 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x7f6], 0);
                    if (res5 != 0) {
                        func_80137F88(pane5, reinterpret_cast<u32>(res5));
                        CPSkillColorS10 c1, c2;
                        CPSkillColorS10* col1 = func_801C4B60(&c1, 0xff, 0xff, 0xfa, 0xff);
                        CPSkillColorS10* col2 = func_801C4B60(&c2, 0x48, 0x3a, 0x21, 0);
                        func_80139AC8(pane5, col2, col1);
                    }
                } else {
                    u32 sum = 0;
                    for (u8 row2 = 0; row2 < 6; row2++) {
                        UI::CPSkillRecord* rec2 = &blob->records2[row2];
                        for (u8 i2 = 1; i2 <= 5; i2++) {
                            if (rec2->slots[i2].word != 0) {
                                sum += func_80136254(lbl_eu_8066488C, &lbl_eu_8050DC20[0x318], rec2->slots[i2].word);
                            }
                        }
                    }
                    u8 id3b = func_801392B4(self->field_F3);
                    u8 rowR2 = lbl_eu_8050DB60[(u8)id3b * 8 + (u8)self->field_F4 - 9];
                    UI::CPSkillRecord* rec3 = &blob->records2[rowR2];
                    if (rec3->slots[(s8)self->field_FE + 1].word != 0) {
                        sum -= func_80136254(lbl_eu_8066488C, &lbl_eu_8050DC20[0x318], rec3->slots[(s8)self->field_FE + 1].word);
                    }
                    s32 remaining = (s32)((UI::CPSkillBlobTotal*)blob)->totalSP - (s32)sum;
                    u8 costIdx = (u8)(((u32)(u8)self->field_F4 - 1) * 0x19 + rowIdxBase + i);
                    u16 costId = func_80136254(lbl_eu_8066488C, &lbl_eu_8050DC20[0x318], costIdx);
                    sprintf(bufB, &lbl_eu_8050DC20[0x749], rowP1, i);
                    if (remaining >= (s32)(u16)costId) {
                        nw4r::lyt::Pane* pane6b = self->field_8->GetRootPane()->FindPaneByName(bufB, true);
                        if (func_801C4648(pane6b) == 0) {
                            void* res6 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x80c], 0);
                            if (res6 != 0) {
                                func_80137F88(pane6b, reinterpret_cast<u32>(res6));
                                CPSkillColorS10 c1, c2;
                                CPSkillColorS10* col1 = func_801C4B60(&c1, 0xff, 0xff, 0xfa, 0xff);
                                CPSkillColorS10* col2 = func_801C4B60(&c2, 0xff, 0xff, 0xfa, 0);
                                func_80139AC8(pane6b, col2, col1);
                            }
                            self->field_104[row * 5 + i - 1] = 1;
                        }
                    } else {
                        void* res7 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x822], 0);
                        if (res7 != 0) {
                            func_80137F88(pane5, reinterpret_cast<u32>(res7));
                            CPSkillColorS10 c1, c2;
                            CPSkillColorS10* col1 = func_801C4B60(&c1, 0xff, 0xff, 0xfa, 0xff);
                            CPSkillColorS10* col2 = func_801C4B60(&c2, 0xaa, 0x19, 0x19, 0);
                            func_80139AC8(pane5, col2, col1);
                        }
                    }
                }
            }
        }
    }
}

// Skill-learn grid refresh (retail func_8026BB60): hide the six slot panes
// of the secondary layout, then for each character-column entry that is not
// the current character, show the slot pane and attach the character
// texture; for each of the 5 skill slots pick the icon/name textures by the
// slot category ladder (unlearned / learned-empty / learned-type1 /
// learned-type2), set the slot colours from the pane animation vectors, and
// finally position the slot pane by the learned-slot count.
extern "C" __declspec(noinline) void func_8026BB60(UI::CPassiveSkillLine* self) {
    u8 id = func_801392B4(self->field_F3);
    UI::CPSkillBlob* blob = reinterpret_cast<UI::CPSkillBlob*>(
        (u8*)func_8009EC9C((u8)id) + 0x3534);
    func_80124270(self->field_18->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x838], true), 0);
    func_80124270(self->field_18->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x841], true), 0);
    func_80124270(self->field_18->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x84a], true), 0);
    func_80124270(self->field_18->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x853], true), 0);
    func_80124270(self->field_18->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x85c], true), 0);
    func_80124270(self->field_18->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0x865], true), 0);
    u32 t = id - 1;
    u32 catBase = (t & 0x1F) * 8;
    f32 scale = lbl_eu_80668930[t * 2];
    u8* table = &lbl_eu_8050DB60[t * 8];
    char buf[0x28];
    u8 slotCount = 0;
    for (u8 idx = 0; idx < self->field_F2; idx++) {
        u8 entry = self->field_EA[idx];
        if (entry == id) {
            continue;
        }
        u8 slot = slotCount + 1;
        sprintf(buf, &lbl_eu_8050DC20[0x86e], slot);
        func_80124270(self->field_18->GetRootPane()->FindPaneByName(buf, true), 1);
        u16 msgId = func_80136254(lbl_eu_80664090, &lbl_eu_8050DC20[0x1e8], entry);
        char* text = func_80138F78(msgId);
        void* res = func_801355F4()->GetResource(0x74696d67, text, 0);
        if (res != 0) {
            sprintf(buf, &lbl_eu_8050DC20[0x879], slot);
            func_80137E7C(self->field_18, buf, res);
        }
        u8 row = table[entry - 1];
        UI::CPSkillRecord* rec = &blob->records2[row];
        u32 cat = catBase + entry;
        u8 learnedCount = 0;
        for (u8 s = 1; s <= 5; s++) {
            sprintf(buf, &lbl_eu_8050DC20[0x772], s);
            u8 v = func_801361E8((u32)lbl_eu_80664888, buf, cat);
            UI::CPSkillSlot* slotPtr = &rec->slots[s];
            void* res2 = 0;
            void* res3;
            if ((slotPtr->byte14 & 1) == 0) {
                switch (v) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x887], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x8a0], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x8b9], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x8d2], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x8eb], 0); break;
                }
                res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x904], 0);
            } else if (slotPtr->word != 0) {
                u8 v2 = func_801361E8((u32)lbl_eu_8066488C, &lbl_eu_8050DC20[0x5b3], slotPtr->word);
                if (v2 == 1) {
                    switch (v) {
                    case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x637], 0); break;
                    case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x650], 0); break;
                    case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x669], 0); break;
                    case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x682], 0); break;
                    case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x69b], 0); break;
                    }
                } else if (v2 == 2) {
                    switch (v) {
                    case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6b4], 0); break;
                    case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6cd], 0); break;
                    case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6e6], 0); break;
                    case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x6ff], 0); break;
                    case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x718], 0); break;
                    }
                }
                msgId = func_80136254(lbl_eu_80664880, &lbl_eu_8050DC20[0x731], slotPtr->word);
                text = func_80138F78(msgId);
                res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, text, 0);
            } else {
                switch (v) {
                case 1: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x917], 0); break;
                case 2: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x930], 0); break;
                case 3: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x949], 0); break;
                case 4: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x962], 0); break;
                case 5: res2 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x97b], 0); break;
                }
                res3 = reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->mArg)->GetResource(0x74696d67, &lbl_eu_8050DC20[0x904], 0);
            }
            if (res2 != 0) {
                sprintf(buf, &lbl_eu_8050DC20[0x994], slot, s);
                func_80137E7C(self->field_18, buf, res2);
            }
            if (res3 != 0) {
                sprintf(buf, &lbl_eu_8050DC20[0x9a9], slot, s);
                func_80137E7C(self->field_18, buf, res3);
                CPSkillFourShorts fs0 = func_80139658(self->field_18, buf, 0);
                CPSkillFourShorts fs1 = fs0;
                CPSkillFourShorts fs2 = func_80139658(self->field_18, buf, 1);
                CPSkillFourShorts fs3 = fs2;
                CPSkillColorS10 c1, c2;
                if ((slotPtr->byte14 & 1) == 0) {
                    CPSkillColorS10* col1 = func_801C4B60(&c1, 0xf0, 0xf0, 0xeb, fs3.d);
                    CPSkillColorS10* col2 = func_801C4B60(&c2, 0xf0, 0xf0, 0xeb, fs1.d);
                    func_80139A18(self->field_18, buf, col2, col1);
                } else {
                    CPSkillColorS10* col1 = func_801C4B60(&c1, 0x2a, 0x22, 0x18, fs3.d);
                    CPSkillColorS10* col2 = func_801C4B60(&c2, 0x2a, 0x22, 0x18, fs1.d);
                    func_80139A18(self->field_18, buf, col2, col1);
                }
            }
            if (s > 1 && (slotPtr->byte14 & 1) != 0) {
                learnedCount++;
            }
        }
        sprintf(buf, &lbl_eu_8050DC20[0x9bc], slot);
        nw4r::lyt::Pane* pane = self->field_18->GetRootPane()->FindPaneByName(buf, true);
        // Retail converts the learned count through the 0x4330-magic double
        // (store count into a u32 pair viewed as f64, subtract 2^52) rather
        // than a direct int->float conversion; lbl_eu_80668910 is the .sdata2
        // 2^52 constant the subtraction consumes.
        CPSkillF64Conv conv;
        f32 pos[2];
        func_80127BC4(pos, reinterpret_cast<f32*>(reinterpret_cast<u8*>(pane) + 0x4C));
        conv.w[0] = 0x43300000;
        conv.w[1] = learnedCount;
        pos[1] = self->field_100 + scale * (f32)(conv.d - lbl_eu_80668910);
        func_80124288(pane, pos);
        slotCount++;
        if (slotCount >= 6) {
            break;
        }
    }
}

// Line update tail (retail func_8026C4A4); body lives at 0x8026C4A4, not yet
// matched. C linkage inherited from the file-top declaration keeps the call
// relocs from func_802696D8/func_80267C44/func_80269370 plain; noinline keeps
// the retail `bl`.
__declspec(noinline) void func_8026C4A4(UI::CPassiveSkillLine* self) {
    char buf[0x50];

    if (self->field_E8 <= 5) {
    // States 1..5.
    if ((s8)self->field_F6 == 0) {
        sprintf(buf, &lbl_eu_8050DC20[0x500], (s8)self->field_F5 + 1);
        func_802640B8(&self->mInfo.field_3C, 0);
    } else {
        sprintf(buf, &lbl_eu_8050DC20[0x431], (s8)self->field_F5 + 1);
        func_802640B8(&self->mInfo.field_3C, 1);
    }
    nw4r::lyt::Pane* paneE =
        self->field_8->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* paneF = self->field_8->GetRootPane()->FindPaneByName(
        &lbl_eu_8050DC20[0x4a5], true);
    nw4r::math::VEC3 pos =
        passiveSkillVecView::func_80137924(paneE, paneF, self->field_8->GetRootPane());
    func_801D2150(self->mInfo.field_24->GetRootPane(), &pos);
    if ((s8)self->field_F6 != 0) {
        u8 chId = func_801392B4((s8)self->field_F3);
        func_80264F7C(self, 1, chId, (u8)(chId - 1), (s8)self->field_F5,
                      0);
        return;
    }
    u8 slotId = func_801392B4((s8)self->field_F3);
    int row5v = (s8)self->field_F5 - 1;
        u8 v = (u8)(slotId + row5v * 5 + 1);
    char* msg = func_8013639C(lbl_eu_80664884, &lbl_eu_8050DC20[0x207], v);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x20c], msg, 0);
    char* msg2 = func_8013639C(lbl_eu_80664884, &lbl_eu_8050DC20[0x207], v);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x215], msg2, 0);
    char* msg3 = func_80136190(&lbl_eu_8050DC20[0x21f], &lbl_eu_8050DC20[0x207], 0x15);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x22b], msg3, 0);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x154],
                  &lbl_eu_8050DC20[0x182], 0);
    u16 cost = func_801361E8((u32)lbl_eu_80664890, &lbl_eu_8050DC20[0x235], v);
    char* msg4 = func_8013639C(lbl_eu_80664894, &lbl_eu_8050DC20[0x207], cost);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x23b], msg4, 0);
    u16 v100 = func_801361E8((u32)lbl_eu_80664894, &lbl_eu_8050DC20[0x245], cost);
    func_8009EC9C((u8)slotId);
    UI::CPSkillLearnRecord* rec = reinterpret_cast<UI::CPSkillLearnRecord*>(
        (u8*)func_8009EC9C((u8)slotId) + 0x3534 + (s8)self->field_F5 * 0xC4);
    for (u8 si = 1; si <= 5; si++) {
        u32 word = *(u32*)&rec[si];
        if (word != 0) {
            char* msgS = func_8013639C(lbl_eu_80664890, &lbl_eu_8050DC20[0x207], v);
            sprintf(buf, &lbl_eu_8050DC20[0x24e], (u8)si);
            u16 sp = func_801361E8((u32)lbl_eu_80664890, buf, v);
            if (sp != 0 && sp != 0xe && sp != 0x4b && sp != 0xa5 && sp != 0xc8) {
                char* msgA = func_80136190(&lbl_eu_8050DC20[0x21f],
                                           &lbl_eu_8050DC20[0x207], 0x12);
                func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x254],
                              msgA, 0);
                char* msgB = func_80136190(&lbl_eu_8050DC20[0x21f],
                                           &lbl_eu_8050DC20[0x207], 6);
                if (cost == 1) {
                    sprintf(buf, &lbl_eu_8050DC20[0x25e], v100);
                } else {
                    sprintf(buf, &lbl_eu_8050DC20[0x261], v100);
                }
                func_80136A1C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x160],
                              buf, 0);
            } else {
                func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x254],
                              &lbl_eu_8050DC20[0x182], 0);
                func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x160],
                              &lbl_eu_8050DC20[0x182], 0);
            }
            break;
        }
    }
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x26b],
                  &lbl_eu_8050DC20[0x182], 0);
    func_80136B4C(self->mInfo.mpLayout, &lbl_eu_8050DC20[0x16c],
                  &lbl_eu_8050DC20[0x182], 0);
    func_802665FC(self, (u8)slotId);
    func_80266724(self, 0, (s8)self->field_F5, 0);
        return;
    }
    if (self->field_E8 <= 0xC) {
// States 6..0xC: rebuild the +0xFD/+0xFE pane pair and sweep the
// character's field_EA category table.
sprintf(buf, &lbl_eu_8050DC20[0x994], (s8)self->field_FD + 1,
    (s8)self->field_FE + 1);
func_802640B8(&self->mInfo.field_3C, 1);
nw4r::lyt::Pane* paneC =
self->field_8->GetRootPane()->FindPaneByName(buf, true);
nw4r::lyt::Pane* paneD = self->field_8->GetRootPane()->FindPaneByName(
&lbl_eu_8050DC20[0x4a5], true);
nw4r::math::VEC3 pos =
passiveSkillVecView::func_80137924(paneC, paneD, self->field_8->GetRootPane());
func_801D2150(self->mInfo.field_24->GetRootPane(), &pos);
u8 ch = func_801392B4((s8)self->field_F3);
func_8009EC9C(ch);
const u8* rowTab = &lbl_eu_8050DB60[(u8)(ch - 1) * 8];
u8 found = 0;
u8 cat = 0;
u8 cost = 0;
for (u8 i2 = 0; i2 < (s8)self->field_F3; i2++) {
if (self->field_EA[i2] == ch) {
    continue;
}
if ((s8)self->field_FD != i2) {
    continue;
}
u8 row = rowTab[self->field_EA[i2]];
cost = row;
cat = (u8)(self->field_FE + 1);
UI::CPSkillLearnRecord* rec = reinterpret_cast<UI::CPSkillLearnRecord*>(
    (u8*)func_8009EC9C(ch) + 0x3534 + row * 0xC4);
u8 flag = rec->byte393C;
found = (u8)(((flag & 1) == 0) ? 0 : (u32)((~(u32)(flag & 1) >> 24) & 0xFF));
found = (u8)(rec->byte393C & 1) == 0 ? 0 : found;
break;
}
func_80264F7C(self, 3, found, ch, cat, cost);
return;
        return;
    }
// States 0xD..0xF: refresh the +0xF7/+0xF8 cell panes.
sprintf(buf, &lbl_eu_8050DC20[0x431], (s8)self->field_F7 + 1,
        (s8)self->field_F8 + 1);
func_802640B8(&self->mInfo.field_3C, 1);
nw4r::lyt::Pane* paneA =
    self->field_8->GetRootPane()->FindPaneByName(buf, true);
nw4r::lyt::Pane* paneB = self->field_8->GetRootPane()->FindPaneByName(
    &lbl_eu_8050DC20[0x4a5], true);
nw4r::math::VEC3 pos =
    passiveSkillVecView::func_80137924(paneA, paneB, self->field_8->GetRootPane());
    func_801D2150(self->mInfo.field_24->GetRootPane(), &pos);
func_80264F7C(self, 2, 0, func_801392B4((s8)self->field_F3),
              (s8)self->field_F7, (s8)self->field_F8);
        return;
}
#pragma optimize_for_size off


// Skill-grid cell flag lookup: the cell byte at +0x104 for the signed
// row/col cursor (row*5 + col). Pointer-arithmetic form keeps MWCC's
// accumulation on self (retail add r3,r3,r4/add r3,r3,r0), and
// optimize_for_size on fixes the -O4,p extsb scheduling bug (extsb would
// read the uninitialised r4) and keeps mulli for the *5.
#pragma optimize_for_size on
__declspec(noinline) u8 func_8026CC34(UI_CPassiveSkill* self) {
    return *(u8*)((u8*)self + 0x104 + self->field_F7 * 5 + self->field_F8);
}
#pragma optimize_for_size off

// Skill-learn check (retail func_8026CC58): scan the line's grid column
// entries; for each entry that is not the current skill id, when the slot
// counter reaches the +0xFD row marker, look up the record row in the
// character skill table and test the learned-flag bit at the row's slot
// (bit 0 of the byte at +0x14 of the (fe+1)th 0x20-byte slot of record
// row+5). Returns 1 on the first learned row, 0 when the slot counter
// exhausts 6 non-matching entries or the grid ends. The stmw r30 frame
// retail uses needs optimize_for_size.
#pragma optimize_for_size on
__declspec(noinline) u8 func_8026CC58(UI::CPassiveSkillLine* self) {
    u8 id = func_801392B4(self->field_F3);
    UI::CPSkillBlob* blob = reinterpret_cast<UI::CPSkillBlob*>(
        (u8*)func_8009EC9C(id) + 0x3534);
    u8* table = &lbl_eu_8050DB60[(id - 1) * 8];
    u8 count = self->field_F2;
    u8 i = 0;
    for (u8 j = 0; j < count; j++) {
        u8 entry = self->field_EA[j];
        if (entry == id) {
            continue;
        }
        if (i == (s8)self->field_FD) {
            u8 row = table[entry - 1];
            UI::CPSkillSlot* slot = reinterpret_cast<UI::CPSkillSlot*>(
                reinterpret_cast<u8*>(&blob->records2[row]) +
                (u32)((s8)self->field_FE + 1) * 0x20);
            if ((slot->byte14 & 1) != 0) {
                return 1;
            }
        }
        i++;
        if (i >= 6) {
            break;
        }
    }
    return 0;
}
#pragma optimize_for_size off

// Skill-learn check (retail func_8026CD44): same shape as func_8026CC58
// but tests the slot word at +0x0 of the row's slot instead of the flag
// byte's bit 0. The stmw r30 frame retail uses needs optimize_for_size.
#pragma optimize_for_size on
__declspec(noinline) u8 func_8026CD44(UI::CPassiveSkillLine* self) {
    u8 id = func_801392B4(self->field_F3);
    UI::CPSkillBlob* blob = reinterpret_cast<UI::CPSkillBlob*>(
        (u8*)func_8009EC9C(id) + 0x3534);
    u8* table = &lbl_eu_8050DB60[(id - 1) * 8];
    u8 count = self->field_F2;
    u8 i = 0;
    for (u8 j = 0; j < count; j++) {
        u8 entry = self->field_EA[j];
        if (entry == id) {
            continue;
        }
        if (i == (s8)self->field_FD) {
            u8 row = table[entry - 1];
            UI::CPSkillSlot* slot = reinterpret_cast<UI::CPSkillSlot*>(
                reinterpret_cast<u8*>(&blob->records2[row]) +
                (u32)((s8)self->field_FE + 1) * 0x20);
            if (slot->word != 0) {
                return 1;
            }
        }
        i++;
        if (i >= 6) {
            break;
        }
    }
    return 0;
}
#pragma optimize_for_size off

// Equipment-state sync (retail func_8026CE30): for each of the five
// category/equipment pairs read from the character data, when the equipped
// item exists and its item category matches the character's +0x11F state
// marker, clear the equipped slot through the per-category setter; then wake
// the character bdat row, and for the first three grid rows rebuild the
// scene's enemy list from the shared scene-name table and clear the first
// matching CfObjectMove. The stmw r24 frame retail uses needs
// optimize_for_size (eight callee-saved registers).
#pragma optimize_for_size on
__declspec(noinline) void func_8026CE30(UI::CPassiveSkillLine* self) {
    u8 charId = func_801392B4(self->field_F3);
    CPSkillCharData* data =
        reinterpret_cast<CPSkillCharData*>(func_8009EC9C(charId));
    s16 ids[5];
    ids[0] = data->equip4;
    ids[1] = data->equip5;
    ids[2] = data->equip6;
    ids[3] = data->equip7;
    ids[4] = data->equip8;
    CPSkillCatFilter cats;
    cats.w.word = lbl_eu_80668934;
    cats.w.byte4 = lbl_eu_80668938;
    for (u8 i = 0; i < 5; i++) {
        s16 v = ids[i];
        if (v == -1) {
            continue;
        }
        int b = cats.bytes[i];
        CPSkillItem* item = (CPSkillItem*)func_80157C4C(b, v);
        if (item == 0 || item->word == 0) {
            continue;
        }
        u16 cat = func_80139358(item->word >> 20);
        u8 flag = 0;
        u8 result = func_801361E8(lbl_eu_806640F8, &lbl_eu_8050DC20[0x9cc], cat);
        if (self->field_11F == 0x85 && result == 3) {
            flag = 1;
        } else if (self->field_11F == 0x84 && result == 2) {
            flag = 1;
        }
        if (flag != 0) {
            switch (b) {
            case 4:
                func_8009E024(data, -1);
                break;
            case 5:
                func_8009E030(data, -1);
                break;
            case 6:
                func_8009E03C(data, -1);
                break;
            case 7:
                func_8009E048(data, -1);
                break;
            case 8:
                func_8009E054(data, -1);
                break;
            }
        }
    }
    func_800A1370(data);
    if ((s8)self->field_F3 < 3) {
        CPSkillEnumListHolder holder;
        u32 names[3];
        const u32* src = lbl_eu_8050DBB8;
        names[0] = *src++;
        names[1] = *src++;
        names[2] = *src++;
        func_80043D90(&holder);
        func_800F4A98(func_80043F18(&holder), names[(s8)self->field_F3], 0);
        if (((CPSkillEnumListCount*)func_80043F18(&holder))->count >= 1) {
            CPSkillEnumListSlot* slot = func_800F6EC0(func_80043F18(&holder), 0);
            cf::CfObjectMove* move = slot->move;
            if (move != 0) {
                cf::CfObjectMove* m = getCfObjectPc__FPQ22cf12CfObjectMove(move);
                if (m != 0) {
                    func_800BFDE0(m, 0);
                }
            }
        }
        __dt__80043E88(&holder, -1);
    }
}
#pragma optimize_for_size off

// Skill-grid position refresh (retail func_8026D080): for each of the 25
// skill-grid cells, format the cell pane name into a local buffer, find that
// pane in the primary layout root, compute its translated position via
// func_8013775C, then translate the five name panes of the cell layout to
// that position and advance the cell layout animation.
__declspec(noinline) void func_8026D080(UI::CPassiveSkillLine* self) {
    char buf[0x28];
    UI::CPSkillGridCell* cellRow;
    u8 row;
    u8 col;
    for (row = 0; row < 5; row++) {
        cellRow = &self->cells[row][0];
        for (col = 0; col < 5; col++) {
            UI::CPSkillGridCell* cell = cellRow + col;
            sprintf(buf, &lbl_eu_8050DC20[0x431], row + 1, col + 1);
            nw4r::lyt::Pane* pane =
                self->field_8->GetRootPane()->FindPaneByName(buf, true);
            nw4r::math::VEC3 pos;
            func_8013775C(&pos, pane);
            func_801D2150(
                cell->mpLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_8050DC20[0x446], true),
                &pos);
            func_801D2150(
                cell->mpLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_8050DC20[0x451], true),
                &pos);
            func_801D2150(
                cell->mpLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_8050DC20[0x45c], true),
                &pos);
            func_801D2150(
                cell->mpLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_8050DC20[0x467], true),
                &pos);
            func_801D2150(
                cell->mpLayout->GetRootPane()->FindPaneByName(
                    &lbl_eu_8050DC20[0x472], true),
                &pos);
            cell->mpLayout->Animate(0);
        }
    }
}

// Skill-grid alpha refresh (retail func_8026D210): read the alpha byte of a
// named pane in the primary layout root, convert it to a frame fraction
// (u8 -> float via the 2^52 magic double, scaled by the divisor), collapse
// it to a step function at 0, scale and truncate to a byte, then write that
// byte into the five name panes of every skill-grid cell layout and advance
// each cell animation.
__declspec(noinline) void func_8026D210(UI::CPassiveSkillLine* self) {
    u8 v = self->field_8->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050DC20[0x9d5], true)
                ->GetAlpha();
    // u8 -> f32 via the 2^52 magic: build 0x43300000_<v> on the stack and
    // subtract the NAMED .sdata2 magic so the pool reloc is lbl_eu_80668910
    // (a plain (f32) cast synthesizes an unmerged TU-local @N entry).
    union {
        struct { u32 hi; u32 lo; } w;
        double d;
    } conv;
    conv.w.lo = v;
    conv.w.hi = 0x43300000u;
    f32 f = (f32)(conv.d - lbl_eu_80668910) / lbl_eu_8066893C;
    if (f > lbl_eu_80668900) {
        f = lbl_eu_80668900;
    } else if (f < lbl_eu_80668904) {
        f = lbl_eu_80668904;
    }
    UI::CPSkillGridCell* cellRow;
    s32 scaled = (s32)(lbl_eu_80668940 * f);
    for (u8 row = 0; row < 5; row++) {
        cellRow = &self->cells[row][0];
        for (u8 col = 0; col < 5; col++) {
            cellRow[col].mpLayout->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050DC20[0x446], true)
                ->SetAlpha((u8)scaled);
            cellRow[col].mpLayout->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050DC20[0x451], true)
                ->SetAlpha((u8)scaled);
            cellRow[col].mpLayout->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050DC20[0x45c], true)
                ->SetAlpha((u8)scaled);
            cellRow[col].mpLayout->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050DC20[0x467], true)
                ->SetAlpha((u8)scaled);
            cellRow[col].mpLayout->GetRootPane()
                ->FindPaneByName(&lbl_eu_8050DC20[0x472], true)
                ->SetAlpha((u8)scaled);
            cellRow[col].mpLayout->Animate(0);
        }
    }
}

// Skill-count helper (retail func_8026D3CC): count how many slot cells in
// the character skill blob's primary (rows 0-4) and secondary (rows 5-10)
// record areas hold the given skill id (a non-zero word with a matching
// halfword id at +4). Retail keeps the inner loops as counted mtctr/bdnz
// loops, which -O4,p only emits under optimize_for_size.
#pragma optimize_for_size on
u8 func_8026D3CC(UI::CPassiveSkillLine* self, int id) {
    UI::CPSkillBlob* blob = reinterpret_cast<UI::CPSkillBlob*>(
        (u8*)func_8009EC9C(func_801392B4(self->field_F3)) + 0x3534);
    u8 count = 0;
    UI::CPSkillRecord* rec;
    for (u8 row = 0; row < 5; row++) {
        rec = &blob->records[row];
        for (u8 slot = 1; slot <= 5; slot++) {
            if (rec->slots[slot].word != 0) {
                u16 cell = rec->slots[slot].id;
                if (cell == id) {
                    count++;
                }
            }
        }
    }
    for (u8 row = 0; row < 6; row++) {
        rec = &blob->records2[row];
        for (u8 slot = 1; slot <= 5; slot++) {
            if (rec->slots[slot].word != 0) {
                u16 cell = rec->slots[slot].id;
                if (cell == id) {
                    count++;
                }
            }
        }
    }
    return count;
}
#pragma optimize_for_size off

// UI::CPassiveSkill constructor (retail symbol __ct__UI_CPassiveSkill, plain
// name). extern "C" so the emitted symbol is exactly `__ct__UI_CPassiveSkill`
// (objdiff would otherwise attribute the name to the longer sibling ctors).
// Stores the vtable, constructs the embedded mem-region class, zero/one-inits
// the state fields, then constructs the +0x28 line and +0x1B8 system window
// sub-objects.
extern "C" void* __ct__UI_CPassiveSkill(UI::CPassiveSkill* self) {
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

// Callee declarations (retail plain/mangled names; also declared in
// CArtsInfo.hpp / CCollepedia.hpp for their TUs).
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void __dt__7CSysWinFv(void*, int);

// Skill-data load + learned-flag sweep (retail func_8026D5A8): load the two
// skill data files into the +0x18/+0x1C handles, reset the embedded syswin
// through a temp-construct/copy/destroy cycle, then for characters 0x29..0x3D
// split each grid entry via func_8013AB0C and set the learned bit of the
// entry's 32 per-slot records in the char-data learn table at +0x3534.
#pragma optimize_for_size on
extern "C" void func_8026D5A8(UI::CPassiveSkill* self) {
    self->field_18 = (u32)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_8050DC20[2525],
        (void*)self, 0, 0);
    self->field_1C = (u32)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        func_800A9D90(), &lbl_eu_8050DC20[2550], (void*)self, 0, 0);

    u8 tempW[0x3C];
    __ct__CSysWin(reinterpret_cast<CSysWin*>(tempW), 2);
    u32* dstW = reinterpret_cast<u32*>(reinterpret_cast<u8*>(&self->mSysWin) + 4);
    u32* srcW = reinterpret_cast<u32*>(tempW + 4);
    dstW[0] = srcW[0];
    dstW[1] = srcW[1];
    dstW[2] = srcW[2];
    dstW[3] = srcW[3];
    self->mSysWin.mFileHandle = reinterpret_cast<CSysWin*>(tempW)->mFileHandle;
    self->mSysWin.mTagProcessor = reinterpret_cast<CSysWin*>(tempW)->mTagProcessor;
    self->mSysWin.mArcAccessor = reinterpret_cast<CSysWin*>(tempW)->mArcAccessor;
    self->mSysWin.mLayout = reinterpret_cast<CSysWin*>(tempW)->mLayout;
    self->mSysWin.mAnimTrans = reinterpret_cast<CSysWin*>(tempW)->mAnimTrans;
    self->mSysWin.field_28 = reinterpret_cast<CSysWin*>(tempW)->field_28;
    self->mSysWin.field_2C = reinterpret_cast<CSysWin*>(tempW)->field_2C;
    self->mSysWin.field_30 = reinterpret_cast<CSysWin*>(tempW)->field_30;
    self->mSysWin.field_34 = reinterpret_cast<CSysWin*>(tempW)->field_34;
    self->mSysWin.field_35 = reinterpret_cast<CSysWin*>(tempW)->field_35;
    self->mSysWin.field_36 = reinterpret_cast<CSysWin*>(tempW)->field_36;
    self->mSysWin.field_37 = reinterpret_cast<CSysWin*>(tempW)->field_37;
    self->mSysWin.field_38 = reinterpret_cast<CSysWin*>(tempW)->field_38;
    self->mSysWin.field_39 = reinterpret_cast<CSysWin*>(tempW)->field_39;
    __dt__7CSysWinFv(reinterpret_cast<CSysWin*>(tempW), -1);
    reinterpret_cast<UI_CPassiveSkillSysWinView*>(&self->mSysWin)->v34();
    for (u8 i = 1; i <= 0x15; i++) {
        u8 ch = (u8)(i + 0x28);
        u16 v = func_8009CF8C(ch);
        u8 b1 = 0;
        u8 b0 = 0;
        func_8013AB0C(&b1, &b0, ch);

        u8* recBase1 = (u8*)func_8009EC9C(b1) + 0x3534;
        u8 cat1 = 0;
        u32 idx1 = (b1 - 1) * 8;
        u8 row1 = lbl_eu_8050DB60[idx1 + b0 - 1];
        if (v < 3000) {
            if (v < 2000) {
                if (v < 1000) {
                    cat1 = 1;
                } else {
                    cat1 = 2;
                }
            } else {
                cat1 = 3;
            }
        } else if (v < 10001) {
            if (v < 5000) {
                cat1 = 4;
            } else {
                cat1 = 5;
            }
        }
        {
            u8* rec1 = recBase1 + row1 * 0xC4 + 980;
            for (u32 slot = 1; slot <= cat1; slot++) {
                u8* entry = rec1 + ((slot << 5) & 0x1FE0);
                if ((entry[20] & 1) == 0) {
                    entry[20] |= 1;
                }
            }
        }

        u8* recBase2 = (u8*)func_8009EC9C(b0) + 0x3534;
        u8 cat2 = 0;
        u32 idx2 = (b0 - 1) * 8;
        u8 row2 = lbl_eu_8050DB60[idx2 + b1 - 1];
        if (v < 3000) {
            if (v < 2000) {
                if (v < 1000) {
                    cat2 = 1;
                } else {
                    cat2 = 2;
                }
            } else {
                cat2 = 3;
            }
        } else if (v < 10001) {
            if (v < 5000) {
                cat2 = 4;
            } else {
                cat2 = 5;
            }
        }
        {
            u8* rec2 = recBase2 + row2 * 0xC4 + 980;
            for (u32 slot = 1; slot <= cat2; slot++) {
                u8* entry = rec2 + ((slot << 5) & 0x1FE0);
                if ((entry[20] & 1) == 0) {
                    entry[20] |= 1;
                }
            }
        }
    }
}
#pragma optimize_for_size off

void func_8026D894(UI_CPassiveSkill* self) {
    // Visibility-gated update: while the menu is open (both gates set), drive
    // the +0x28 sub-object update and, depending on the window state byte
    // (1 = normal, 3 = closing), run the matching +0x28 state transition first.
    // NB: written as a switch - retail emits the small-switch compare chain
    // (beq into case bodies laid out after dispatch, break-jumps to tail);
    // an if/else-if chain produces inverted bne-over branches instead.
    // NB: two separate early-returns (compound || gives the second test
    // inverted bne-over polarity) and a real switch for the state dispatch -
    // retail emits beq-into-case-body chains with break-jumps.
    if (self->field_24 == 0) {
        return;
    }
    if (self->field_25 == 0) {
        return;
    }
    switch (self->field_25) {
    case 1:
        func_8026DCF4(self);
        break;
    case 3:
        func_8026DD3C(self);
        break;
    }
    func_80267360(self->_pad28);
}

extern "C" void func_8026D8FC(UI_CPassiveSkill* self, u8* arg2) {
    if (self->field_24 == 0) {
        return;
    }
    if (self->field_25 == 0) {
        return;
    }
    // Tail-call the +0x28 sub-object's update, forwarding arg2 (DrawInfo-like).
    return func_80267484(reinterpret_cast<UI::CPassiveSkillLine*>(self->_pad28),
                         reinterpret_cast<nw4r::lyt::DrawInfo*>(arg2));
}

// Callee declarations (retail plain/mangled names; also declared in
// CArtsInfo.hpp / CCollepedia.hpp / CEquipItemBox.hpp for their TUs).
extern "C" void getEntry__5CBdatFUl(u32);
extern "C" void closeFileHandle__FPP11CFileHandle(void*);
extern "C" void releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor(void*);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);
extern "C" void func_800A13C4(void*, u32);
extern "C" void func_8022B7F4(void*);
#pragma optimize_for_size on
// UI::CPassiveSkill teardown (retail func_8026D920): close the shared bdat
// reader, release the two file handles and the arc resource accessor, tear
// down the embedded line sub-object and syswin, release the mem-region
// object, then clear each character's skill-blob pointer (rows 1..8).
extern "C" void func_8026D920(UI::CPassiveSkill* self) {
    getEntry__5CBdatFUl(2);
    closeFileHandle__FPP11CFileHandle(&self->field_18);
    closeFileHandle__FPP11CFileHandle(&self->field_1C);
    self->field_24 = 0;
    func_802675D8(&self->mLine);
    func_8022B7F4(&self->mSysWin);
    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor((void*)self->field_20);
    self->field_20 = 0;
    deleteRegion__17UnkClass_8045F564Fv(&self->mUnk8);
    for (u8 i = 1; i <= 8; i++) {
        func_800A13C4(func_8009EC9C(i), 1);
    }
}
#pragma optimize_for_size off
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

// returns 1 when the byte at +0x110 is >= 8 (retail subfc/subfze carry
// idiom -- optimize_for_size on forces the -O4,s lowering in this -O4,p unit).
#pragma optimize_for_size on
u32 func_8026DA34(u8* self) {
    return (u32)self[0x110] >= 8;
}
#pragma optimize_for_size off

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
    func_802676F8(reinterpret_cast<UI::CPassiveSkillLine*>(self->_pad28));
}

// Close the skill menu: when the visibility gate is in state 2, bump it to 3,
// clear the +0x27 flag, run the +0x28 sub-object update, and play UI SFX 6.
void func_8026DA88(UI_CPassiveSkill* self) {
    if (self->field_25 == 2) {
        self->field_25 = 3;
        self->field_27 = 0;
        func_80267B08(reinterpret_cast<UI::CPassiveSkillLine*>(self->_pad28));
        playUISound(6);
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


extern "C" void UI_CPassiveSkill_thunk28_67CE0(u8* self) { func_80267CE0(reinterpret_cast<UI::CPassiveSkillLine*>(self + 0x28)); }

void UI_CPassiveSkill_thunk28_67E2C(u8* self) { func_80267E2C(self + 0x28); }

extern "C" void UI_CPassiveSkill_thunk28_67F88(u8* self) { func_80267F88(reinterpret_cast<UI::CPassiveSkillLine*>(self + 0x28)); }

void UI_CPassiveSkill_thunk28_68250(u8* self) { func_80268250(reinterpret_cast<UI::CPassiveSkillLine*>(self + 0x28)); }

extern "C" void UI_CPassiveSkill_thunk28_67BA0(u8* self) { func_80267BA0(reinterpret_cast<UI::CPassiveSkillLine*>(self + 0x28)); }

void UI_CPassiveSkill_thunk28_67C44(u8* self) { func_80267C44(reinterpret_cast<UI::CPassiveSkillLine*>(self + 0x28)); }

void UI_CPassiveSkill_thunk28_68518(u8* self) { func_80268518(reinterpret_cast<UI::CPassiveSkillLine*>(self + 0x28)); }

// Pick the skill-menu open state (retail func_8026DB74): when the +0x1B8
// system window is armed or the +0x180 byte is set, return the closed marker
// 0/0x5f; otherwise key the +0x110 skill cell against the learned marker and
// the party-size flag ((shared byte > 1) via the subfic/srwi carry idiom) to
// pick one of the eight open-state ids, split by whether the menu-open
// configuration (0x3372/0x3508/0x20 BDAT values) is satisfied. The two
// callee-saved registers (r30/r31) need the stmw/lmw frame retail uses, which
// -O4,p only emits under optimize_for_size.
#pragma optimize_for_size on
s32 func_8026DB74(UI_CPassiveSkill* self) {
    if (CSysWin_getUnk34(&self->field_1B8) != 0) {
        return 0;
    }
    if (self->field_180 != 0) {
        return 0x5f;
    }
    u8 b = code80135FDC_getByte_64077();
    int flag = (int)((u32)(1 - b) >> 31);
    // Retail zero-tests func_8009CF8C(0x3372) via the cntlzw/srwi idiom.
#undef __cntlzw
    {
        u32 cf8cVal = (u32)func_8009CF8C(0x3372);
        u32 cntlz = (u32)__cntlzw(cf8cVal);
        if ((cntlz >> 5) != 0 ||
            (func_8009CF8C(0x3508) != 0 && func_8009CF8C(0x20) < 0x38)) {
            u8 cell = self->field_104[12];
            if (cell == 0xf) {
                return 0x62;
            }
            if (cell >= 8) {
                return flag != 0 ? 0x61 : 0x64;
            }
            return flag != 0 ? 0x60 : 0x63;
        }
        u8 cell = self->field_104[12];
        if (cell == 0xf) {
            return 0x5c;
        }
        if (cell >= 8) {
            return flag != 0 ? 0x5b : 0x5e;
        }
        return flag != 0 ? 0x5a : 0x5d;
    }
}
#pragma optimize_for_size off

extern "C" void UI_CPassiveSkill_setByte180(u8* self, u8 val) { *(u8*)(self + 0x180) = val; }

extern "C" void UI_CPassiveSkill_thunk28_68C38(u8* self) { func_80268C38(reinterpret_cast<UI::CPassiveSkillLine*>(self + 0x28)); }

s32 func_8026DCA0(UI_CPassiveSkill* self) {
    // Once the +0x1B8 CSysWin reports ready the menu is considered open.
    if (UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34(self) != 0) {
        return 1;
    }
    // Otherwise report whether the skill cell at +0x110 holds the "learned" marker (15).
    return (self->field_104[12] == 15) ? 1 : 0;
}

extern "C" __declspec(noinline) int UI_CPassiveSkill_thunk1b8_CSysWin_getUnk34(UI_CPassiveSkill* self) { return CSysWin_getUnk34(self->field_1B8); }

// noinline: retail keeps the `bl` from func_8026D894.
extern "C" __declspec(noinline) void func_8026DCF4(UI_CPassiveSkill* self) {
    if ((s32)func_802676DC(reinterpret_cast<UI_CPassiveSkill*>(self->_pad28)) != 0) {
        self->field_25 = 2;
        self->field_27 = 1;
    }
}

// noinline: retail keeps the `bl` from func_8026D894.
extern "C" __declspec(noinline) void func_8026DD3C(UI_CPassiveSkill* self) {
    if ((s32)func_802676DC(reinterpret_cast<UI_CPassiveSkill*>(self->_pad28)) != 0) {
        self->field_25 = 0;
        self->field_27 = 1;
    }
}

// Learn-all-skills init (retail func_8026DD84): when the menu is not yet
// initialised (field_20 set, field_1C clear), mark the open gates and walk
// grid slots 0x29..0x3D, splitting each slot into (character, entry) via
// func_8013AB0C and setting the learned bit of both the character's entry and
// the entry's character in the char-data learn table at +0x393C. The four
// callee-saved registers (r28-r31) need the stmw/lmw frame retail uses, which
// -O4,p only emits under optimize_for_size.
#pragma optimize_for_size on
// noinline: retail keeps the `bl` from OnFileEvent's two call sites.
__declspec(noinline) void func_8026DD84(UI::CPassiveSkill* self) {
    if (self->field_20 == 0 || self->field_1C != 0) {
        return;
    }
    self->field_26 = 1;
    self->field_24 = 1;
    for (u8 i = 1; i <= 0x15; i++) {
            u8 ch = (u8)(i + 0x28);
            func_8009CF8C(ch);
            u8 b1 = 0;
            u8 b0 = 0;
            func_8013AB0C(&b1, &b0, ch);
            void* data1 = func_8009EC9C(b1);
            u32 idx1 = b1 * 8;
            u8 row1 = lbl_eu_8050DB60[idx1 + b0 - 9];
            UI::CPSkillLearnRecord* rec1 = reinterpret_cast<UI::CPSkillLearnRecord*>(
                (u8*)data1 + row1 * 0xC4);
            if ((rec1->byte393C & 1) == 0) {
                rec1->byte393C |= 1;
            }
            void* data2 = func_8009EC9C(b0);
            u32 idx2 = b0 * 8;
            u8 row2 = lbl_eu_8050DB60[idx2 + b1 - 9];
            UI::CPSkillLearnRecord* rec2 = reinterpret_cast<UI::CPSkillLearnRecord*>(
                (u8*)data2 + row2 * 0xC4);
            if ((rec2->byte393C & 1) == 0) {
                rec2->byte393C |= 1;
            }
    }
}
#pragma optimize_for_size off

void CPassiveSkill::OnFileEvent() {}

// Cursor layout init: create the layout from the shared arc resource accessor,
// bind the two animation transforms, then start with the second transform
// disabled and the first enabled before animating once. The r30/r31 stmw/lmw
// frame retail uses needs optimize_for_size.
#pragma optimize_for_size on
void func_80263F30(UI::CPassiveSkillCur* self) {
    buildLayout(&self->mpLayout, self->mArcResAcc, lbl_eu_8050DC20);
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans0, self->mArcResAcc, &lbl_eu_8050DC20[0x18]);
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans1, self->mArcResAcc, &lbl_eu_8050DC20[0x35]);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, 0);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, 1);
    self->mpLayout->Animate(0);
}
#pragma optimize_for_size off
// Cursor animation step: skip when no layout; dispatch on mActive as a real
// switch (retail lowers the two-case byte test to a cmpi/beq compare chain;
// sequential ifs produce inverted bne-over bodies).
// noinline: retail keeps the `bl` from func_80267360's common tail.
void func_80263FE8(UI::CPassiveSkillCur* self) {
    if (self->mpLayout == 0) {
        return;
    }
    switch (self->mActive) {
    case 0:
        advanceAnimTransform(self->mpAnimTrans0, lbl_eu_80668900);
        break;
    case 1:
        func_80264140(self);
        break;
    }
    self->mpLayout->Animate(0);
}
// Release the cursor's primary layout: drop it through the layout vtable's
// delete slot (index 0 at vtable+0x8 with flag 1) and null the pointer.
// noinline: func_802675D8 keeps the retail `bl`.
extern "C" __declspec(noinline) void func_80264060(UI::CPassiveSkillCur* self) {
    if (self->mpLayout != 0) {
        delete self->mpLayout;
        self->mpLayout = 0;
    }
}

// Pane/region helper (retail func_802640B8): for each of the two pane-name
// pointers (lbl_eu_80668908 / lbl_eu_8066890C), look the pane up in the region
// layout's root pane and show it only when arg matches the slot index. C
// linkage so call relocs are the plain retail name; noinline keeps the retail
// `bl` from func_80269924 / func_80269A18 / func_80269B14.
// NB: no optimize_for_size here - retail's stmw r28 frame comes from
// -use_lmw_stmw on plain -O4,p, whose scheduler keeps the two SDA pointer
// loads after the register setup (the size pragma hoists them above it).
// NB: optimize_for_size required for the stmw/lmw r28-r31 frame.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_802640B8(UI::UI_PassiveSkillRegion3C* self, u8 arg) {
    const char* names[2];
    names[0] = lbl_eu_80668908;
    names[1] = lbl_eu_8066890C;
    for (u8 i = 0; i < 2; i++) {
        nw4r::lyt::Pane* pane =
            self->field_8->GetRootPane()->FindPaneByName(names[i], true);
        func_80124270(pane, i == arg);
    }
}
#pragma optimize_for_size off
// Cursor step helper (retail func_80264140):
// frame check succeeds, deactivate/hide the cursor, swap the enabled transform
// back to mpAnimTrans0 and rewind its frame. extern C so func_80263FE8's call
// reloc is the plain retail name; noinline keeps the retail `bl`.
extern "C" __declspec(noinline) void func_80264140(UI::CPassiveSkillCur* self) {
    if (advanceAnimTransform(self->mpAnimTrans1, lbl_eu_80668900) != 0) {
        self->mActive = 0;
        self->mVisible = 1;
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, 0);
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, 1);
        self->mpAnimTrans0->SetFrame(lbl_eu_80668904);
    }
}
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
// Cursor layout init (retail func_80264204): create the main layout from the
// accessor, bind the two animation transforms (second disabled, first
// enabled), animate once, then find the three panes (hiding the first) and
// store the other two finders at +0x10/+0x14. The r30/r31 stmw/lmw frame
// retail uses needs optimize_for_size. noinline: retail keeps the `bl
// func_80264204` call from func_802646E8.
#pragma optimize_for_size on
__declspec(noinline) void func_80264204(UI_PassiveSkillLayoutInit* self) {
    buildLayout(&self->mpLayout, self->mArcResAcc, &lbl_eu_8050DC20[0x50]);
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans0, self->mArcResAcc, &lbl_eu_8050DC20[0x68]);
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans1, self->mArcResAcc, &lbl_eu_8050DC20[0x85]);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, 0);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, 1);
    self->mpLayout->Animate(0);
    nw4r::lyt::Pane* pane0 =
        self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0xa0], true);
    func_80124270(pane0, 0);
    self->field_10 =
        self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0xa7], true);
    self->field_14 =
        self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050DC20[0xb7], true);
    func_80124270(self->field_10, 0);
    func_80124270(self->field_14, 0);
}
#pragma optimize_for_size off
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

// Callee declarations (retail plain/mangled names; also declared in
// CEquipItemBox.hpp / CCollepedia.hpp for their TUs).
extern "C" void* createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
extern "C" void __dt__14Class_8045F858Fv(void*, int);
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor*, void*, const char*);
extern "C" void validateHeap__17UnkClass_8045F564Fv(void*);
extern "C" void setBdatEntry__5CBdatFUlPv(u32, void*);
class CEventFile;

// UI::CPassiveSkill file-event handler (retail
// OnFileEvent__13CPassiveSkillFP10CEventFile). Two accepted handles:
// - field_18 (skill-data read): rebuild the region, wrap the loaded buffer in
//   an arc accessor, construct a temp line from it, member-copy it into the
//   +0x28 sub-object, run its three fixups, then learn-all init.
// - field_1C (archive read): close the bdat reader and refresh the six
//   per-category BDAT table pointers, then learn-all init.
#pragma optimize_for_size on
extern "C" int OnFileEvent__13CPassiveSkillFP10CEventFile(UI::CPassiveSkill* self,
                                                          CEventFile* ev) {
    u32 handle = (u32)*(void**)((u8*)ev + 4);
    if (self->field_18 == handle) {
        createRegion__17UnkClass_8045F564FiiPCci(
            &self->mUnk8, (int)getHandleMEM2__Q23mtl10MemManagerFv(), 0x40000,
            &lbl_eu_8050DC20[0xa12], 0);
        u8 guard[8];
        u8 lineRaw[0x190];
        __ct__14Class_8045F858FP17UnkClass_8045F564(guard, &self->mUnk8);
        void* fh = (void*)self->field_18;
        void* buf = *(void**)((u8*)fh + 4);
        *(void**)((u8*)fh + 4) = 0;
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        self->field_20 = (u32)createArcResourceAccessor__10CLibLayoutFv();
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(
            (nw4r::lyt::ArcResourceAccessor*)self->field_20, buf,
            &lbl_eu_8050DC20[0xa20]);

        // Direct-init the temp via the retail accessor ctor (a default-constructed
        // temp would emit an extra CPassiveSkillInfo ctor call).
        __ct__UI_CPassiveSkillLine(reinterpret_cast<UI::CPassiveSkillLine*>(lineRaw),
                                   (nw4r::lyt::ArcResourceAccessor*)self->field_20);
        UI::CPassiveSkillLine& lineObj = *reinterpret_cast<UI::CPassiveSkillLine*>(lineRaw);
        // Explicit member-wise copy: vptr excluded; mInfo and mCur are
        // re-copied by the helper calls below. An implicit whole-struct assign
        // here makes MWCC emit extra copy-helper calls (declared dtors on
        // CPassiveSkillInfo) and bloats the body to 0x3e0.
        self->mLine.mArg = lineObj.mArg;
        self->mLine.field_8 = lineObj.field_8;
        self->mLine.field_C = lineObj.field_C;
        self->mLine.field_10 = lineObj.field_10;
        self->mLine.field_14 = lineObj.field_14;
        self->mLine.field_18 = lineObj.field_18;
        self->mLine.field_1C = lineObj.field_1C;
        UI::CPSkillGridCell* dstCell = &self->mLine.cells[0][0];
        const UI::CPSkillGridCell* srcCell = &lineObj.cells[0][0];
        for (u8 ci = 0; ci < 25; ci++) {
            *dstCell++ = *srcCell++;
        }
        self->mLine.field_E8 = lineObj.field_E8;
        self->mLine.field_E9 = lineObj.field_E9;
        *reinterpret_cast<u32*>(&self->mLine.field_EA[0]) =
            *reinterpret_cast<const u32*>(&lineObj.field_EA[0]);
        *reinterpret_cast<u32*>(&self->mLine.field_EA[4]) =
            *reinterpret_cast<const u32*>(&lineObj.field_EA[4]);
        self->mLine.field_F2 = lineObj.field_F2;
        self->mLine.field_F3 = lineObj.field_F3;
        self->mLine.field_F4 = lineObj.field_F4;
        self->mLine.field_F5 = lineObj.field_F5;
        self->mLine.field_F6 = lineObj.field_F6;
        self->mLine.field_F7 = lineObj.field_F7;
        self->mLine.field_F8 = lineObj.field_F8;
        self->mLine.field_F9 = lineObj.field_F9;
        self->mLine.field_FA = lineObj.field_FA;
        self->mLine.field_FB = lineObj.field_FB;
        self->mLine.field_FC = lineObj.field_FC;
        self->mLine.field_FD = lineObj.field_FD;
        self->mLine.field_FE = lineObj.field_FE;
        self->mLine.field_100 = lineObj.field_100;
        *reinterpret_cast<u32*>(&self->mLine.field_104[0]) =
            *reinterpret_cast<const u32*>(&lineObj.field_104[0]);
        *reinterpret_cast<u32*>(&self->mLine.field_104[4]) =
            *reinterpret_cast<const u32*>(&lineObj.field_104[4]);
        *reinterpret_cast<u32*>(&self->mLine.field_104[8]) =
            *reinterpret_cast<const u32*>(&lineObj.field_104[8]);
        *reinterpret_cast<u32*>(&self->mLine.field_104[12]) =
            *reinterpret_cast<const u32*>(&lineObj.field_104[12]);
        *reinterpret_cast<u32*>(&self->mLine.field_104[16]) =
            *reinterpret_cast<const u32*>(&lineObj.field_104[16]);
        *reinterpret_cast<u32*>(&self->mLine.field_104[20]) =
            *reinterpret_cast<const u32*>(&lineObj.field_104[20]);
        self->mLine.field_104[24] = lineObj.field_104[24];
        func_80267268(reinterpret_cast<UI_CPassiveSkillInfo*>(&self->mLine.mInfo),
                      reinterpret_cast<const UI_CPassiveSkillInfo*>(&lineObj.mInfo));
        func_802672F0(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mLine.mInfo.field_3C),
                      reinterpret_cast<const UI::CPassiveSkillCur*>(&lineObj.mInfo.field_3C));
        self->mLine.mInfo.field_54 = lineObj.mInfo.field_54;
        func_8026732C(reinterpret_cast<UI::CPassiveSkillCur*>(&self->mLine.mCur),
                      reinterpret_cast<const UI::CPassiveSkillCur*>(&lineObj.mCur));
        lineObj.~CPassiveSkillLine();
        func_80266B78(reinterpret_cast<UI::CPassiveSkillLine*>(&self->mLine));
        func_8026DD84(self);
        self->field_18 = 0;
        validateHeap__17UnkClass_8045F564Fv(&self->mUnk8);
        __dt__14Class_8045F858Fv(guard, -1);
        return 1;
    }

    if (self->field_1C != handle) {
        return 0;
    }
    {
        void* fh2 = (void*)self->field_1C;
        *(void**)((u8*)fh2 + 4) = 0;
        setBdatEntry__5CBdatFUlPv(2, 0);
        func_8003AA34();
        lbl_eu_80664880 = getFP__FPCc(&lbl_eu_8050DC20[0xa24]);
        func_8003AA34();
        lbl_eu_80664884 = getFP__FPCc(&lbl_eu_8050DC20[0xa2f]);
        func_8003AA34();
        lbl_eu_80664888 = getFP__FPCc(&lbl_eu_8050DC20[0xa3a]);
        func_8003AA34();
        lbl_eu_8066488C = getFP__FPCc(&lbl_eu_8050DC20[0xa44]);
        func_8003AA34();
        lbl_eu_80664890 = getFP__FPCc(&lbl_eu_8050DC20[0xa51]);
        func_8003AA34();
        lbl_eu_80664894 = getFP__FPCc(&lbl_eu_8050DC20[0xa5d]);
        self->field_1C = 0;
        func_8026DD84(self);
        return 1;
    }
}
#pragma optimize_for_size off
