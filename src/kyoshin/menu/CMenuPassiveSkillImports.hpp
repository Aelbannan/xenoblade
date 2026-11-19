#pragma once

// Unit-private import declarations split out of CMenuPassiveSkill.hpp so the class header
// stays includable next to other headers.
#include "kyoshin/menu/CMenuPassiveSkill.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

extern "C" void Init__17CMenuPassiveSkillFv();
extern "C" void Move__17CMenuPassiveSkillFv();
extern "C" unsigned long hasPassiveSkillMenu();
extern "C" void pollPassiveSkillReady(CMenuPassiveSkill* self);
extern "C" void updatePassiveSkillMenu(CMenuPassiveSkill* self);
extern "C" void dispatchPassiveSkillInput(CMenuPassiveSkill* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void CPassiveSkill_update(u8* self);
extern "C" void setPassiveSkillReady(CMenuPassiveSkill* self);
extern "C" void advancePassiveSkillState(CMenuPassiveSkill* self);
extern "C" void tryOpenPassiveSkill(CMenuPassiveSkill* self);
extern "C" void confirmPassiveSkillMenu(CMenuPassiveSkill* self);
extern "C" u32 CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();


// C-ABI widget ctor import (US strips its member manglings).
extern "C" void* __ct__UI_CPassiveSkill(u8* self);

// Float constant seeded into field_2B0 by the constructor.
extern f32 lbl_eu_806688F0;


// Widget-rebuild imports used by Init (retail-unmangled C symbols).
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dst, void* src);
extern "C" void __ct__6CBgTexFUc(CBgTex* self, u8 arg);
extern "C" void __ct__11CTitleAHelpFPcUc(CTitleAHelp* self, char* name, u8 arg);
extern char lbl_eu_8050DB4C[];
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn* scn, IScnRender* render,
                                                      u32 prio, u32 arg);
extern "C" void CPassiveSkill_loadSkillData(u8* self);

#pragma pack(push, 1)

// 8-byte grid cell pair copied by the lwzu/stwu counted loops in Init
// (same shape as CMenuCollepedia's CollepediaSubEntry).
struct CPSkillCellPair {
    u32 field_0; // +0x00
    u32 field_4; // +0x04
};

// Whole-struct-assignment blocks: assigning these by value makes MWCC emit
// the retail shape (first word/float unrolled, remainder via a counted
// 8-byte lwzu/stwu pair loop).
struct CPSkillGridBlock {
    u32 head;                   // +0x00
    CPSkillCellPair cells[25];  // +0x04 (0xC8 bytes)
};
struct CPSkillTailBlock {
    f32 frame;                  // +0x00
    CPSkillCellPair cells[3];   // +0x04 (0x18 bytes)
};

/* Packed 8-byte head block at +0x110: assigning it as a unit reproduces
 * retail's schedule (byte unroll, paired word loads ascending / stores
 * descending). */
struct CPSkillHeadBlock {
    u8 b110;      // +0x00
    u8 b111;      // +0x01
    u32 w112;     // +0x02 (misaligned)
    u32 w116;     // +0x06 (misaligned)
};

/* Packed 8-byte word pair at +0x112: assigning it as a unit reproduces
 * retail's paired load/store schedule (two loads ascending, two stores
 * descending). */
struct CPSkillWordPair {
    u32 w112; // +0x00 (abs +0x112)
    u32 w116; // +0x04 (abs +0x116)
};

// Byte-tail view: base at the last cell pair so +0x04 lands on +0x144.
struct CPSkillByteTail {
    u8 _pad0[4]; // +0x00
    u8 tail;     // +0x04
};
#pragma pack(pop)

/* Field-by-field copy view of the embedded UI::CPassiveSkill used by Init's
 * stack-temp rebuild: a fresh UI::CPassiveSkill is constructed on the stack,
 * its body cloned into the embedded storage (vtable word skipped), then the
 * temp destroyed. Several words sit at odd offsets (+0x112, +0x116), so the
 * struct is 1-packed to reproduce retail's misaligned lwz/stw (same pack
 * pattern as CPartyStateWinBlob58). */
#pragma pack(push, 1)
struct CMenuPassivePSView {
    u8 _pad0[0x4];            // +0x000 vtable word (never copied)
    u32 field_4;              // +0x004
    u8 mUnk8[0x10];           // +0x008 UnkClass_8011C974 mem region
    u32 field_18;             // +0x018
    u32 field_1C;             // +0x01C
    u32 field_20;             // +0x020
    u8 field_24;              // +0x024
    u8 field_25;              // +0x025
    u8 field_26;              // +0x026
    u8 field_27;              // +0x027
    u8 _pad28[0x4];           // +0x028..0x2B (not copied)
    u32 field_2C;             // +0x02C
    u32 field_30;             // +0x030
    u32 field_34;             // +0x034
    u32 field_38;             // +0x038
    u32 field_3C;             // +0x03C
    u32 field_40;             // +0x040
    CPSkillGridBlock block1;      // +0x044..0x10F (assigned as a unit)
    CPSkillHeadBlock headBlock;   // +0x110..0x117 (assigned as a unit)
    u8 field_11A[13];         // +0x11A..0x126
    u8 _pad127[1];            // +0x127
    f32 field_128;            // +0x128
    CPSkillCellPair cells2[3];    // +0x12C..0x143 (counted 8-byte loop)
    u8 field_144;             // +0x144
    u8 field_145;             // +0x145
    u8 field_146;             // +0x146
    u8 field_147;             // +0x147
    u8 _pad148[0x4];          // +0x148..0x14B (not copied)
    u32 field_14C;            // +0x14C
    u32 field_150;            // +0x150
    u32 field_154;            // +0x154
    u32 field_158;            // +0x158
    u32 field_15C;            // +0x15C
    u8 field_160[4];          // +0x160..0x163
    f32 field_164;            // +0x164
    u32 field_168;            // +0x168
    u32 field_16C;            // +0x16C
    u32 field_170;            // +0x170
    u32 field_174;            // +0x174
    u32 field_178;            // +0x178
    u32 field_17C;            // +0x17C
    u8 field_180[4];          // +0x180..0x183
    u8 _pad184[0x4];          // +0x184..0x187 (not copied)
    u32 field_188;            // +0x188
    u32 field_18C;            // +0x18C
    u32 field_190;            // +0x190
    u32 field_194;            // +0x194
    u8 field_198[3];          // +0x198..0x19A
    u8 _pad19B[1];            // +0x19B
    u32 field_19C;            // +0x19C
    u8 _pad1A0[0x4];          // +0x1A0..0x1A3 (not copied)
    u32 field_1A4;            // +0x1A4
    u32 field_1A8;            // +0x1A8
    u32 field_1AC;            // +0x1AC
    u32 field_1B0;            // +0x1B0
    u8 field_1B4;             // +0x1B4
    u8 field_1B5;             // +0x1B5
    u8 _pad1B6[0x6];          // +0x1B6..0x1BB (not copied)
    u8 mUnk1BC[0x10];         // +0x1BC UnkClass_8011C974 mem region (syswin)
    u32 field_1CC;            // +0x1CC
    u32 field_1D0;            // +0x1D0
    u32 field_1D4;            // +0x1D4
    u32 field_1D8;            // +0x1D8
    u32 field_1DC;            // +0x1DC
    u8 field_1E0;             // +0x1E0
    u8 _pad1E1[0x3];          // +0x1E1..0x1E3 (not copied)
    u32 field_1E4;            // +0x1E4
    u32 field_1E8;            // +0x1E8
    u8 field_1EC[6];          // +0x1EC..0x1F1
};
#pragma pack(pop)

// Game-progress flag getter (shared split1 helper, retail-unmangled name).
extern "C" u32 CtrlRemote_TouchBitByArg(u32 flagId);

// cf::CfPadData view: only the flag words dispatchPassiveSkillInput reads (held buttons
// at +0x00/+0x04, turbo/short-press flags at +0x104).
struct CMenuPassivePadView {
    u32 field_0;                // +0x00
    u32 field_4;                // +0x04 held-button flags
    u8 _pad8[0x104 - 0x8];
    u32 mTurboPressButtonFlags; // +0x104 short-press flags
};

// UI::CPassiveSkill +0x28 sub-object open thunk (retail-unmangled name).
extern "C" void UI_CPassiveSkill_thunk28_68518(u8* self);

// +0x28 sub-object input thunks dispatched by dispatchPassiveSkillInput (retail names
// unmangled; each takes the +0xB8 sub-object).
extern "C" void UI_CPassiveSkill_thunk28_67C44(u8* self);
extern "C" void UI_CPassiveSkill_thunk28_67BA0(u8* self);
extern "C" void UI_CPassiveSkill_thunk28_68594(u8* self);
extern "C" void UI_CPassiveSkill_thunk28_67CE0(u8* self);
extern "C" void UI_CPassiveSkill_thunk28_67E2C(u8* self);
extern "C" void UI_CPassiveSkill_thunk28_67F88(u8* self);
extern "C" void UI_CPassiveSkill_thunk28_68250(u8* self);
extern "C" void UI_CPassiveSkill_thunk28_68C38(u8* self);

// +0x180 state-byte setter / pending-transition poller on the +0xB8 object.
extern "C" void UI_CPassiveSkill_setByte180(u8* self, u8 value);
extern "C" int CPassiveSkill_isSyswinOrLearned(u8* self);

// Shared event-file byte + classic-controller query (retail unmangled).
extern "C" CMenuPassivePadView* getCfPadData__Q22cf13CfGameManagerFv();

// Timer-tick addend / clamp cap seeded around field_2B0.
extern f32 lbl_eu_806688F4;
extern f32 lbl_eu_806688F8;

// ---------------------------------------------------------------------------
// Retail-unmangled imports (US strips these member manglings; C linkage binds
// the verbatim symbol names).
// ---------------------------------------------------------------------------
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn* scn, IScnRender* render);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
// int (not u8): callers compare r3 directly with cmpi, no rlwinm mask.
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void teardown(CTitleAHelp* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);

// UI::CPassiveSkill helpers (first arg is the +0xB8 sub-object).
extern "C" void CPassiveSkill_teardown(u8* self);
extern "C" int CPassiveSkill_getVisibleIfReady(u8* self);
extern "C" void CPassiveSkill_open(u8* self);
extern "C" int CPassiveSkill_getActiveIfBusy(u8* self);
extern "C" u8 CPassiveSkill_pickOpenState(u8* self);
extern "C" void CPassiveSkill_draw(u8* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q22UI13CPassiveSkillFv(u8* self, int flags);
extern "C" int CPassiveSkill_cellAtLeast8(u8* self);
extern "C" void CPassiveSkill_confirm(u8* self);
extern "C" void CPassiveSkill_close(u8* self);

// Destructor retail symbols / game-side CProcess D2 wrapper (CMainMenu.cpp).
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);

// cf pad enable/disable (retail pre-mangled name), sound-op helper.
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).

// Scene-active gate + TaskGame queries used by cbRenderBefore.

// DrawInfo setup helper; the C++ declaration mangles to the retail name
// SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo.

// Shared mode bitfield (bit 21 / IBM bit 10 gates cbRenderBefore draws).
extern u32 lbl_eu_80663E28;
