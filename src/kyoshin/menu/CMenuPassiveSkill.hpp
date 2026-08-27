#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include <nw4r/lyt.h>

class CScn;
class CMenuPassiveSkill;

extern "C" void Init__17CMenuPassiveSkillFv();
extern "C" void Move__17CMenuPassiveSkillFv();
extern "C" __declspec(noinline) CMenuPassiveSkill* __ct__CMenuPassiveSkill(CMenuPassiveSkill* self, u32 arg);
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" CMenuPassiveSkill* __dt__17CMenuPassiveSkillFv(CMenuPassiveSkill* self, int flags);
extern "C" CMenuPassiveSkill* func_802638D0(CProcess* parent, u32 arg);
extern "C" unsigned long func_80263944();
extern "C" void func_80263954(CMenuPassiveSkill* self);
extern "C" void func_802639E4(CMenuPassiveSkill* self);
extern "C" void func_80263A34(CMenuPassiveSkill* self);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_8026D894(u8* self);
extern "C" void func_80263D3C(CMenuPassiveSkill* self);
extern "C" void func_80263D8C(CMenuPassiveSkill* self);
extern "C" void func_80263DE8(CMenuPassiveSkill* self);
extern "C" void func_80263E4C(CMenuPassiveSkill* self);
extern "C" u32 func_800FEDF8();
extern "C" void func_800FF914();

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / isFlag01Set__9CTaskGameFv). Full header
// clashes with the harness_catalog CScn stub in this TU.
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool isFlag01Set();
};

/*
 * Passive skill menu screen process.
 * IScnRender subobject is at offset 0x58.
 *
 * The CProcess head is held as RAW STORAGE, not a base: CProcess has a
 * virtual destructor, so inheriting it makes this class polymorphic and MWCC
 * emits an implicit virtual dtor named __dt__17CMenuPassiveSkillFv alongside
 * the vtable, hijacking the retail D2 free-function symbol. Raw storage keeps
 * the class non-polymorphic (CMenuItem/CMenuGetItem pattern).
 */
class CMenuPassiveSkillBase {
public:
    u8 mProcess[0x10];         // 0x00 CProcess storage head
    u32 mVtable10;             // 0x10 CProcess vtable slot (temp, then composite)
    u8 mProcRest[0x39 - 0x14]; // 0x14..0x38 CProcess tail
    u8 mIsRemove;              // 0x39
    u8 mIsDisableMove;         // 0x3A
    u8 mIsDisableDraw;         // 0x3B
    u32 ptmfCallbacks[6];      // 0x3C - Move (0-2) / Draw (3-5) callback pmfs
    u8 field_54;               // 0x54 - state flag written by func_80263D3C
    u8 mField55;               // 0x55
    u8 _pad56[2];              // 0x56..0x57
};

class CMenuPassiveSkill : public CMenuPassiveSkillBase {
public:
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_80263EAC();
    void func_80263EB4(int flags);

    // --- member fields ---
    u32 mIScnRenderVt;        // 0x58 - IScnRender subobject vtable slot
    CScn* mScnRef;            // 0x5C - owning scene (removeRenderCB target)
    CBgTex mBgTex;            // 0x60
    CTitleAHelp mTitleAHelp;  // 0x80
    /* UI::CPassiveSkill sub-object at 0xB8 (opaque here; its full layout lives
     * in kyoshin/CPassiveSkill.hpp whose tail padding would swallow 0x2AC). */
    u8 mPassiveSkill[0x1F4];  // 0xB8..0x2AB
    u8 field_2AC;             // 0x2AC - widgets-ready flag set by func_80263954
    u8 _pad2AD[3];            // 0x2AD..0x2AF
    f32 field_2B0;            // 0x2B0 - float constant seeded from lbl_eu_806688F0
};

// Temp (CProcess) vtable pre-PMF copy; composite vtable whose IScnRender
// sub-vtable sits at +0x24.
extern char lbl_eu_8052BF70[];
extern char lbl_eu_805378B0[];

// Null pointer-to-member-function constant (3 words).
extern u32 __ptmf_null[3];

// C-ABI widget ctor imports (US strips their member manglings).
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 arg);
extern "C" void* __ct__UI_CPassiveSkill(u8* self);

// Float constant seeded into field_2B0 by the constructor.
extern f32 lbl_eu_806688F0;

// Widget-rebuild imports used by Init (retail-unmangled C symbols).
extern "C" void __ct__UnkClass_8011C974(void* dst, void* src);
extern char lbl_eu_8050DB4C[];
extern "C" char* func_80136190(char* base, char* entry, u32 len);
extern "C" void func_801C3C14(CBgTex* self);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void addRenderCB__4CScnFP10IScnRenderUlUl(CScn* scn, IScnRender* render,
                                                      u32 prio, u32 arg);
extern "C" void func_8026D5A8(u8* self);

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
extern "C" u32 func_8009CF8C(u32 flagId);

// cf::CfPadData view: only the flag words func_80263A34 reads (held buttons
// at +0x00/+0x04, turbo/short-press flags at +0x104).
struct CMenuPassivePadView {
    u32 field_0;                // +0x00
    u32 field_4;                // +0x04 held-button flags
    u8 _pad8[0x104 - 0x8];
    u32 mTurboPressButtonFlags; // +0x104 short-press flags
};

// UI::CPassiveSkill +0x28 sub-object open thunk (retail-unmangled name).
extern "C" void UI_CPassiveSkill_thunk28_68518(u8* self);

// +0x28 sub-object input thunks dispatched by func_80263A34 (retail names
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
extern "C" int func_8026DCA0(u8* self);

// Shared event-file byte + classic-controller query (retail unmangled).
extern "C" u8 code80135FDC_getByte_64077();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" CMenuPassivePadView* getCfPadData__Q22cf13CfGameManagerFv();

// Timer-tick addend / clamp cap seeded around field_2B0.
extern f32 lbl_eu_806688F4;
extern f32 lbl_eu_806688F8;

// ---------------------------------------------------------------------------
// Retail-unmangled imports (US strips these member manglings; C linkage binds
// the verbatim symbol names).
// ---------------------------------------------------------------------------
extern "C" void removeRenderCB__4CScnFP10IScnRender(CScn* scn, IScnRender* render);
extern "C" void func_801C3D9C(CBgTex* self);
// int (not u8): callers compare r3 directly with cmpi, no rlwinm mask.
extern "C" int func_801C3E34(CBgTex* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);

// UI::CPassiveSkill helpers (first arg is the +0xB8 sub-object).
extern "C" void func_8026D920(u8* self);
extern "C" int func_8026D9AC(u8* self);
extern "C" void func_8026DA4C(u8* self);
extern "C" int func_8026D9F0(u8* self);
extern "C" u8 func_8026DB74(u8* self);
extern "C" void func_8026D8FC(u8* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q22UI13CPassiveSkillFv(u8* self, int flags);
extern "C" int func_8026DA34(u8* self);
extern "C" void func_8026DAD0(u8* self);
extern "C" void func_8026DA88(u8* self);

// Destructor retail symbols / game-side CProcess D2 wrapper (CMainMenu.cpp).
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" void __dt__800FED0C(CProcess* self, int flags);

// cf pad enable/disable (retail pre-mangled name), sound-op helper.
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);
extern "C" void playUISound__FUl(u32 op);

// Scene-active gate + TaskGame queries used by cbRenderBefore.
extern "C" int func_8013BE50();
extern "C" void getInstance__9CTaskGameFv();
extern "C" int isFlag01Set__9CTaskGameFv();

// DrawInfo setup helper; the C++ declaration mangles to the retail name
// func_80137250__FPQ34nw4r3lyt8DrawInfo.
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Singleton state: created-screen pointer (cleared by Term) and the shared
// mode bitfield (bit 21 / IBM bit 10 gates cbRenderBefore draws).
extern unsigned long lbl_eu_80664878;
extern u32 lbl_eu_80663E28;
