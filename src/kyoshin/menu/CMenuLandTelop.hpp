#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace nw4r {
namespace lyt {
class Layout;
class DrawInfo;
class ArcResourceAccessor;
class AnimTransform;
class Pane;
}
}
class CScn;

// 4-byte land-telop entry record (array at +0x9A, scanned in func_801453B8).
struct CLandTelopEntry {
    u16 f0;   // 0x00
    u8  f2;   // 0x02
    u8  f3;   // 0x03
};

/*
 * Land-telop menu screen process (CSysWinSave / CMenuQstCnt family).
 *
 * Retail stores the vtable pointer at +0x10 (CProcess-style layout). The base
 * CProcess subobject is constructed OOL via __ct__8CProcessFv and the class's
 * own vtable + the CProcess run-time fields are filled by hand, so the struct
 * is laid out explicitly rather than inheriting (same scheme as CMenuQstCnt /
 * CMenuBattleEnd / CSystemWindow).
 *
 *   0x00 CProcess (0x3C bytes; vtable ptr at +0x10)
 *   0x3C IUIWindow/Move region (48 bytes: ptmf slots + misc u32s + flags)
 *   0x6C IWorkEvent vtable slot
 *   0x70 IScnRender vtable slot (render-callback subobject)
 *   0x74 CScn* owning scene
 *   0x78 UnkClass_8045F564 embedded region (0x10 bytes)
 *   0x88..0xE0 layout-anim / control fields (0x8C u16, 0x8E u8 set from the
 *             ctor args; 0x9A..0xDA the 16-entry scan table)
 *   0xE0 bool flag (restored from func_80226B94 in Init)
 */
struct CMenuLandTelop {
    u8  mPad0[0x10];                     // 0x00 link-list nodes
    u32 mVtab;                           // 0x10 vtable ptr
    u8  mPad14[0x3C - 0x14];             // 0x14 rest of CProcess
    u32 ptmf0[3];                        // 0x3C Move ptmf (__ptmf_null)
    u32 ptmf1[3];                        // 0x48 Draw ptmf (__ptmf_null)
    nw4r::lyt::Layout* field_54;         // 0x54 owning layout (deleted in Term)
    u32 field_58;                        // 0x58
    u32 field_5C;                        // 0x5C
    s32 field_60;                        // 0x60 (-1)
    u8  field_64;                        // 0x64
    u8  field_65;                        // 0x65
    u8  field_66;                        // 0x66
    u8  field_67;                        // 0x67 (default 1, cleared in Init)
    u32 field_68;                        // 0x68
    u32 mWorkEvent;                      // 0x6C IWorkEvent dispatch vtable ptr
    u32 mScnRender;                      // 0x70 IScnRender dispatch vtable ptr
    CScn* mScene;                        // 0x74 owning scene
    UnkClass_8045F564 mMemRegion;        // 0x78 embedded scratch region (0x10)
    nw4r::lyt::AnimTransform* field_88;  // 0x88 layout anim transform
    u16 field_8C;                        // 0x8C
    u8  field_8E;                        // 0x8E
    u8  _8F;                             // 0x8F
    u32 field_90;                        // 0x90
    f32 field_94;                        // 0x94
    u8  field_98;                        // 0x98
    u8  _99;                             // 0x99
    CLandTelopEntry mEntries[16];        // 0x9A (16 * 4 = 0x40 bytes -> 0xDA)
    u8  field_DA;                        // 0xDA
    u8  _DB;                             // 0xDB
    f32 field_DC;                        // 0xDC
    u8  field_E0;                        // 0xE0 bool
    // sizeof = 0xE1

    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
};

// Object behind the global lbl_eu_806641A0 pointer (real type not yet
// recovered). Only the byte offsets touched by this TU are modelled.
struct CLandTelopGlobal {
    u8 _00[0x64];            // 0x00
    u8 field_0x64;           // 0x64
    u8 _65[0x8E - 0x65];     // 0x65
    u8 field_0x8E;           // 0x8E state flag (0/1)
    u8 _8F[0xE0 - 0x8F];     // 0x8F
    u8 field_0xE0;           // 0xE0 bool
};

// Minimal CScn declaration (guarded so the kyoshin menu/task headers can be
// included together; same declaration appears in CTaskGameEff/Evt/Pic.hpp).
// Member calls emit the retail mangled addRenderCB__4CScnFP10IScnRenderUlUl.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// Minimal CTaskGame decl (retail global-namespace class, symbols
// getInstance__9CTaskGameFv / isFlag01Set__9CTaskGameFv). The real
// CTaskGame.hpp pulls broken monolib work.hpp-umbrella headers, so the static
// members are declared here (same scheme as CSystemWindow.hpp).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool isFlag01Set();
};

// cf::CBattleManager battle-list view (only the +0x8 list head is touched by
// func_80144EE4's non-empty check).
struct CBattleListNode {
    CBattleListNode* next;   // 0x0
};
struct CBattleListHead {     // view at cf::CBattleManager + 0
    u8 _0[0x8];
    CBattleListNode* list;   // 0x8
};

namespace cf {
class CBattleManager;  // forward decl only (list head at +0x8 via CBattleListHead)
}

// C-linkage imports/definitions: retail symbols in this TU are unmangled, so
// they are declared `extern "C"` to stop MWCC suffixing the reloc name
// (CSysWinSave.hpp / CMenuQstCnt.hpp idiom). The cpp definitions inherit the
// linkage; __dt__14CMenuLandTelopFv satisfies the member dtor at link time.
extern "C" {
CMenuLandTelop* __ct__CMenuLandTelop(CMenuLandTelop* self, CScn* scene,
                                     u16 opt, u8 param);
CMenuLandTelop* func_80144EE4(CProcess* parent, CScn* scene, u16 opt, u8 param);
int func_801453B8(CMenuLandTelop* self);
void func_8014504C(CLandTelopGlobal* obj, u32 opt, u32 param);
void __ct__17UnkClass_8045F564Fv(void* _this);
void __dt__17UnkClass_8045F564Fv(void* _this, int flags);
void __ct__8CProcessFv(CProcess* _this);
void __dt__8CProcessFv(CProcess* _this, int flags);
CMenuLandTelop* __dt__14CMenuLandTelopFv(CMenuLandTelop* _this, int flags);
void cbRenderBefore__14CMenuLandTelopFv(void* _this);
int func_80226B94();
int func_8013BE50();
// getBdatStringColumnValue / getInstance__Q22cf14CBattleManagerFv come from
// their canonical headers via the include chain (u32 / void* from
// kyoshin/cf/CBattleManagerApi.hpp).
int func_80144FC8();
int func_80144FF0();
void func_80145018();
unsigned char func_80145030(void);
void func_8014548C(CMenuLandTelop* self);
void func_80145A90(void* _this);
void func_80145A98(void* _this);
void func_80145AA0(void* _this);
const char* func_80145AA8(int index);
}

// C++-linkage helpers (retail symbols are their mangled forms).
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
void drawLayout(nw4r::lyt::Layout* layout, nw4r::lyt::DrawInfo* drawInfo,
                   int a, int b);

// nw4r DrawInfo pre-mangled ct/dt names (retail emits the direct calls; a
// C++ local would virtual-dispatch its scope-exit destructor and bloat the
// body - same scheme as CMenuQstCnt / CSystemWindow).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo,
                                              int flags);

// View of the object returned by CDeviceFont::getFontInfo: vtable+0x24
// returns the font handle handed to func_8013676C (root-pane font binding).
class CLandTelopFontObj {
public:
    virtual ~CLandTelopFontObj(); // 0x00 (3 dtor slots)
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10();
    virtual void vfunc_0x14();
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20();
    virtual u32 getFontHandle();  // 0x24
};

// Land-telop string/layout-name table plus the anim timing constants used by
// Move / func_8014548C.
extern char lbl_eu_80501720[];
extern f32 lbl_eu_806673CC;       // banner/frame step delta
extern f32 lbl_eu_806673D0;       // banner timer limit
extern f64 lbl_eu_806673D8;       // progress clamp (high)
extern f64 lbl_eu_806673E0;       // progress clamp (low)
extern f32 lbl_eu_806673E8;       // progress scale
extern f32 lbl_eu_806673EC;       // cue volume scale
extern f32 lbl_eu_806673F0;       // fade timer limit
extern f64 lbl_eu_806673F8;       // u32->double conversion bias

// C-linkage imports whose retail symbols are unmangled or pre-mangled
// identifiers (declaring them C++ would emit a mangled reloc).
extern "C" {
nw4r::lyt::ArcResourceAccessor* func_801355F4();
u8 func_8013600C(const void*, const void*, u32);
void* func_8010CE48();                    // system menu open gate
unsigned char func_8014A2A0();
unsigned char func_8014A2B4();
f32 func_801895EC();                      // scene BGM volume
u32 func_80495FF0(CScn* scene);           // scene audio handle
void func_80043738(u32, const char*, u32, u32, u32, u32, f32);
u32 isResourceFlagSet__Q22cf13CfGameManagerFv(u32);
void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32,
                                                         nw4r::lyt::Layout*);
int func_8026178C(void* data, u32 flag);
u32 func_8025FB10(void* data, u32 flag);
u32 func_80293C10();                      // party-change notice gate
s32 func_8029A658();                      // party-change notice gate
u32 func_801B481C();                      // block-condition gate
u32 func_80122450();                      // quest-menu gate
unsigned int func_80124B78();             // system-menu gate
}

// C++-linkage import (retail symbol is the mangled playUISound__FUl).
void playUISound(u32 cue);

// Global data imports (sbss/rodata; MWCC does not mangle global-scope data).
extern CLandTelopGlobal* lbl_eu_806641A0;
extern u8* lbl_eu_806640E0;
extern char lbl_eu_805018A8[];
extern u32 lbl_eu_80663E28;      // render-gate mode bitfield (.sbss; bit 21 = busy)
extern f32 lbl_eu_806673C8;      // layout reset frame constant
extern u32 __ptmf_null[3];       // null pointer-to-member-function constant
extern u32 lbl_eu_8052D238[];    // primary vtable
extern u32 lbl_eu_8052E8B0[];    // dispatch vtable cluster base
