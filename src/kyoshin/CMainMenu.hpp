#pragma once

#include <types.h>

class CFileHandle;
class CProcess;

namespace nw4r { namespace lyt {
class Layout;
class Pane;
class ArcResourceAccessor;
class AnimTransform;
class DrawInfo;
} }
namespace nw4r { namespace math { struct VEC3; } }

class CBaseCur;
class CSubCur;
class CEventFile;
class CFileHandle;

// Minimal layout-identical view of CArtsInfo (only field_0x54 is used here).
// Full definition in kyoshin/CArtsInfo.hpp, which cannot be included from
// this TU (extern "C" func_8013606C signature clash with code_80135FDC.hpp).
class CArtsInfo {
public:
    u8 _00[0x54];
    u8 field_0x54;
};

// CSubCur view at CMainMenu+0xA8 (CBaseCur layout, 0x16 bytes). Byte-array
// storage keeps sizeof == 0x16 with no tail padding (member views are taken
// by pointer cast). Cast-only iface for the vtable dispatches.
struct CMainMenuSubCur {
    u8 _data[0x16];
};
// Member view of the same 0x16-byte cursor body (used for stack-temp copies).
struct CMainMenuSubCurView {
    void* mVtable;
    nw4r::lyt::ArcResourceAccessor* mArcResAcc;
    nw4r::lyt::Layout* mpLayout;
    nw4r::lyt::AnimTransform* mpAnimTrans0;
    nw4r::lyt::AnimTransform* mpAnimTrans1;
    u8 mActive;
    u8 mVisible;
};
// Mirror of the font object returned by CDeviceFont::getFontInfo; vtable
// slot 9 (offset 0x24) yields the pane bound by func_8013676C.
struct CMainMenuFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};
// Minimal CScn view (retail addRenderCB__4CScnFP10IScnRenderUlUl member),
// guarded so the kyoshin headers can be included together.
class IScnRender;
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif
struct CMainMenuCurVt {
    virtual void _v008();       // VUpdate
    virtual void _v00C();       // cleanup
    virtual void vfn_0x10(const nw4r::math::VEC3* trans);  // setRootPaneTranslate
};

class CMainMenu {
public:
    CMainMenu();
    CMainMenu(void* param);
    virtual ~CMainMenu();
    void Init();
    void Term();
    void cbRenderBefore();
    int isAnyMenuOpen();
    inline int isMenuOpen() { return isAnyMenuOpen(); }
    void func_800FEB14(float* pos);

    // CProcess layout (inherited, 0x00-0x3C):
    //   +0x00: vtable (CProcess)
    //   +0x04: mParent, mChild, mNext, mPrev, etc.
    //   +0x10: CProcess vtable set in constructor
    //   +0x3C-0x44: PTMF (IWorkEvent vtable entry)
    //   +0x48-0x50: PTMF (IWorkEvent vtable entry)
    //   +0x54: byte (field_0x54)
    //   +0x55: byte (field_0x55)
    //   +0x58-0x5C: IWorkEvent vtable slot (2 words)
    //   +0x60: UnkClass_8045F564 (embedded object)
    //   +0x70: void* param (from constructor)
    //   +0x74: CFileHandle* (file handle)
    //   +0x78: ArcResourceAccessor*
    //   +0x7C: Layout* (main layout)
    //   +0x80: AnimTransform* (anim0)
    //   +0x84: AnimTransform* (anim1)
    //   +0x88: AnimTransform* (anim2)
    //   +0x8C: AnimTransform* (anim3)
    //   +0x90: CBaseCur (embedded)
    //   +0xA8: CSubCur (embedded)
    //   +0xC0: s32 cursorIndex
    //   +0xC4: s32 subIndex
    //   +0xC8: u8 cursorAvail[8] (cursor availability flags)
    //   +0xD0: u8 flags[13] (various flags)
    //   +0xE0: s32 state

    u8 _04[0x08 - 0x04];                      // 0x04
    nw4r::lyt::Layout* field_0x08;             // 0x08 - layout holder read by func_800FEB14
    u8 _0C[0x10 - 0x0C];                      // 0x0C
    // +0x10: CProcess vtable slot set by the constructor (temp, then composite)
    void* mVtable;                             // 0x10
    u8 _14[0x3C - 0x14];                       // 0x14-0x3B
    // PTMF at 0x3C (IWorkEvent vtable, 3 words)
    u32 field_0x3C;                            // 0x3C
    u32 field_0x40;                            // 0x40
    u32 field_0x44;                            // 0x44
    u32 field_0x48;                            // 0x48
    u32 field_0x4C;                            // 0x4C
    u32 field_0x50;                            // 0x50
    u8 field_0x54;                             // 0x54
    u8 field_0x55;                             // 0x55
    u8 _56[0x58 - 0x56];                      // 0x56-0x57
    u32 mIWorkEventVtbl;                       // 0x58 - IWorkEvent vtable (raw)
    u32 field_0x5C;                            // 0x5C
    u8 _60[0x70 - 0x60];                       // 0x60-0x6F - UnkClass_8045F564 embedded
    CScn* field_0x70;                          // 0x70 - owning scene (from ctor param)
    CFileHandle* field_0x74;                   // 0x74 - file handle
    nw4r::lyt::ArcResourceAccessor* field_0x78; // 0x78 - arc resource accessor
    nw4r::lyt::Layout* field_0x7C;             // 0x7C - main layout
    nw4r::lyt::AnimTransform* field_0x80;      // 0x80 - anim0
    nw4r::lyt::AnimTransform* field_0x84;      // 0x84 - anim1
    nw4r::lyt::AnimTransform* field_0x88;      // 0x88 - anim2
    nw4r::lyt::AnimTransform* field_0x8C;      // 0x8C - anim3
    // CBaseCur at +0x90 (16 bytes), CSubCur at +0xA8
    u8 _90[0xA8 - 0x90];                       // 0x90-0xA7 - CBaseCur
    CMainMenuSubCur subCur;                    // 0xA8-0xBD - CSubCur
    u8 _BE[0xC0 - 0xBE];
    s32 field_0xC0;                            // 0xC0 - cursor index
    s32 field_0xC4;                            // 0xC4 - sub-index
    u8 field_0xC8[8];                          // 0xC8-0xCF - cursor availability flags
    u8 field_0xD0;                             // 0xD0 - flag: hasSave
    u8 field_0xD1;                             // 0xD1 - flag: something1
    u8 field_0xD2;                             // 0xD2 - flag: something2
    u8 field_0xD3;                             // 0xD3 - flag: something3
    u8 field_0xD4;                             // 0xD4 - flag: something4
    u8 field_0xD5;                             // 0xD5 - flag: something5
    u8 field_0xD6;                             // 0xD6 - flag: something6
    u8 field_0xD7;                             // 0xD7
    u8 field_0xD8;                             // 0xD8
    u8 field_0xD9;                             // 0xD9
    u8 field_0xDA;                             // 0xDA
    u8 field_0xDB;                             // 0xDB
    u8 field_0xDC;                             // 0xDC
    u8 _DD[0xE0 - 0xDD];                       // 0xDD-0xDF padding
    s32 field_0xE0;                            // 0xE0 - state

    void OnFileEvent();
};

// Gameplay-input gate for the menu-frame dispatch (defined in this TU).
// Takes the owner like retail (the caller emits mr r3,r31 before the call).
extern "C" int func_80101A88(CMainMenu* self);

// Per-frame menu handlers (defined in this TU). extern "C" so every
// referencing reloc carries the exact retail (unmangled) symbol.
extern "C" void func_80101BF8(CMainMenu* self);
extern "C" void func_800FF920(CMainMenu* self);
extern "C" void func_801010B8(CMainMenu* self);
extern "C" void func_80100E14(CMainMenu* self);
extern "C" void func_801018F4(CMainMenu* self);
extern "C" void func_800FEF4C(CMainMenu* self);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void cbRenderBefore__9CMainMenuFv();
extern "C" bool __ct__800FF300(CMainMenu* self, CEventFile* pEventFile);
extern "C" void __dt__8CBaseCurFv(void*, int);   // defined in kyoshin/CCur.cpp
extern "C" void __dt__8CProcessFv(void*, int);   // CProcess base destructor
extern "C" void* __dt__7CSubCurFv(CBaseCur*, int); // defined in kyoshin/CCur.cpp
extern "C" CBaseCur* __ct__CSubCur(CBaseCur*, nw4r::lyt::ArcResourceAccessor*);
extern "C" char lbl_eu_804FCEBC[];              // rodata: menu resource names
extern "C" void Regist__8CProcessFP8CProcessb(void* _this, void* parent, bool insertTop);
extern "C" bool isMenuOpen__9CMainMenuFv();
extern "C" bool isAnyMenuOpen__9CMainMenuFv();

// Menu singleton/state guards (defined in their owning menu TUs)
extern "C" u32 func_80167A18();   // item menu active (CMenuItem.cpp)
extern "C" u32 func_80242354();   // map-select menu active (CMenuMapSelect.cpp)
extern "C" u32 func_80252CD4();   // collepedia active (CMenuCollepedia.cpp)
extern "C" u32 func_80257308();   // kizunagram active (CMenuKizunagram.cpp)
extern "C" u32 func_8027037C();   // play-award active (CMenuPlayAward.cpp)
extern "C" u32 func_80272488();   // kizuna-talk-list active (CMenuKizunaTalkList.cpp)
extern "C" u32 func_8029BBA0();   // option menu active (CMenuOption.cpp)
extern "C" u32 func_802AC510();   // tutorial-list active (CMenuTutorialList.cpp)
extern "C" u32 func_8011CD5C();   // quest-log active (CMenuQuestLog.cpp)
extern "C" u32 func_80124B78();   // close-system-menu gate (CHelp_CloseSysMenu.cpp)
extern "C" u32 func_8028E440();   // save-menu active (CMenuSave.cpp)
extern "C" u32 func_8029EE58();   // update menu active (CMenuUpdate.cpp)
extern "C" u32 func_80122450();   // close-quest-menu gate (CHelp_CloseQuestMenu.cpp)
extern "C" int func_80135898();   // menu-system close (CUICfManager.cpp)
extern "C" u32 func_80192BD0();   // party-state screen active (CMenuPTState.cpp)
extern "C" u32 func_80212480();   // make-crystal menu active (CMenuMakeCrystal.cpp)
extern "C" u32 func_8022F530();   // arts-set menu active (CMenuArtsSet.cpp)

// Gameplay-input gate helpers (cf::CfGameManager / cf::CBattleManager)
extern "C" int isSceneLoading__Q22cf13CfGameManagerFv();
extern "C" int isAnyFieldFlagSet__Q22cf13CfGameManagerFv();

// split1 .sdata2 float constants (int->float magic f64 at 0x80666F10,
// then two 0.0f)
extern f64 lbl_eu_80666F10;   // 0x80666F10 - s32->f32 conversion constant (0x4330000080000000)
extern f32 lbl_eu_80666F18;   // 0x80666F18 - anim frame time (0.0f)
extern f32 lbl_eu_80666F1C;   // 0x80666F1C - HP death threshold (0.0f)

// Presentation/mode bitfield (sibling of lbl_eu_80663E24; split1 .sbss).
extern u32 lbl_eu_80663E28;

// Resource/flag getter (resource id -> value; defined in CMiniMap.cpp).
extern "C" u32 func_8009CF8C(u32 resourceId);

// UnkClass_8045F564 embedded-object lifecycle (defined in split1; normally
// declared via kyoshin/CArtsInfo.hpp, which this TU cannot include).
class UnkClass_8045F564;
extern "C" void __ct__17UnkClass_8045F564Fv(void* _this);
extern "C" void __dt__17UnkClass_8045F564Fv(void* _this, int flags);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void* _this);

// Mangled-identifier call forms previously supplied by CArtsInfo.hpp.
extern "C" int advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*, float);
extern "C" void playUISound__FUl(u32);
extern "C" void func_801D216C(void* cur, u8 flag);
extern "C" void func_80137924(nw4r::math::VEC3* out, nw4r::lyt::Pane*, nw4r::lyt::Pane*, nw4r::lyt::Pane*);

// Cursor constructors (defined in kyoshin/CCur.cpp)
extern "C" void __ct__8CBaseCurFv(CBaseCur*, nw4r::lyt::ArcResourceAccessor*);
// Font binding imports (CDeviceFont / CLibLayout)
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();

// Menu pane-position/color helper (defined in code_80135FDC.cpp).
extern "C" void func_801398A4(nw4r::lyt::Layout* layout, const char* paneName,
                              const void* src, u32 idx);
// Menu open/close gate (defined in code_80135FDC.cpp).
extern "C" void func_80139198(u32 arg);
// System-window busy gate (defined in code_80135FDC.cpp).
extern "C" int func_8013BE50();
// Another menu-open gate (kizuna-talk-list / message-log family).
extern "C" u32 func_80263944();
// Message-log busy gate (CSysWinMsgLog.cpp).
extern "C" u32 func_8027EA64();
// System window buffer singleton gate (CSysWinBuff.cpp).
class CSysWinBuff;
extern "C" CSysWinBuff* getInstance__11CSysWinBuffFv();
// Window focus gates (CUICfManager.cpp).
extern "C" void func_80134460();
extern "C" void func_801341D8();
// Pad-enable/disable and mode gates (cf::CfGameManager).
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable); // bool in CMenuPassiveSkill.hpp
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

// cf::CPad view: only the three flag words this unit reads.
struct CMainMenuPad {
    u32 mHeldButtonFlags;       // 0x00
    u32 mPressedButtonFlags;    // 0x04
    u32 mTurboPressButtonFlags; // 0x08
};
extern "C" CMainMenuPad* getCurrentPad__Q22cf13CfGameManagerFv();

// Cursor activate helper (defined in CCur.cpp).
extern "C" void func_801D2174(CBaseCur* cur);
// Cursor draw helper (defined in CCur.cpp; retail symbol is unmangled C linkage).
extern "C" void func_801D20B0(void* cur, nw4r::lyt::DrawInfo* drawInfo);

// Minimal CTaskGame decl (retail symbols getInstance__9CTaskGameFv /
// isFlag01Set__9CTaskGameFv). The full CTaskGame.hpp pulls monolib headers
// whose declarations clash with this TU's include set.
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool isFlag01Set();
};
// Cursor per-frame update helper (defined in CCur.cpp).
extern "C" void func_801D202C(void* cur);
// Pane-config finish gate (defined in CUICfManager.cpp).
extern "C" void func_8013D8A0();

// Sub-menu pane-name table (14 words) and cursor->angle s16 table.
struct CMainMenuNameTable {
    const char* name[14];
};
extern u32 lbl_eu_804FCE50[];
extern s16 lbl_eu_804FCD60[];
// CBaseCur runtime vtable (stored over the stack temp after construction).
extern void* lbl_eu_8052BF28[];

// Constructor data symbols (see __ct__CMainMenu in CMainMenu.cpp).
extern char lbl_eu_8052BF70[];   // interim CProcess composite vtable
extern char lbl_eu_8052BE24[];  // CMainMenu composite vtable (+0x24/+0xAC slots)
extern u32 lbl_eu_8052BDE8[3];   // move-callback PTMF hook
extern u32 __ptmf_null[3];      // null pointer-to-member-function constant
// CProcess base constructor (abstract class, out-of-line in retail).
extern "C" void __ct__8CProcessFv(CProcess* self);

// Player view structs for func_80101A88 (mirror CfGimmick.hpp layouts).
// getPlayer returns the +0x3E9C embedded spot object; the player base is the
// de-biased pointer (base = spot - 0x3E9C).
struct CMainMenuPlayerSub {
    u8 _00[0x3B4];
    u8 field_0x3B4[0x4EC - 0x3B4];  // 0x3B4 - sub-object passed to the spot vfn
    u32 field_4EC;              // 0x4EC - state flags
    u8 _4F0[0x530 - 0x4F0];
    u16 field_530;              // 0x530 - action flags
};

// Cast-only iface for the player-spot vtable slot 0x9C (party/status open).
// RTTI on -> two hidden slots before the first declared virtual.
struct CMainMenuSpotVt {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void vfn_0x9C(u8* arg);  // slot 0x9C
};
struct CMainMenuPlayerSpot {
    void** vtable;              // 0x00 - embedded +0x3E9C sub-object
    u8 _04[0xC4 - 0x04];
    CMainMenuPlayerSub* field_0xC4;   // 0xC4 - battle-state sub-object
};
struct CMainMenuPlayer {
    void** vtable;              // 0x00 - slot 0x128 yields HP (float)
    u8 _04[0x3E9C - 0x04];
    CMainMenuPlayerSpot spot;   // 0x3E9C (0xC8 bytes; field_0xC4 == player+0x3F60)
};

// Cast-only iface for the player vtable: MWCC virtual dispatch emits the
// retail lwz r12 / lwz r12,off(r12) pattern (fp-style vtable loads color the
// vptr temp r4). RTTI on -> two hidden slots before the first declared
// virtual, so placeholders fill 0x08..0x124 and vf128 lands at retail 0x128.
// Never constructed.
struct CMainMenuPlayerVt {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void _v04C();
    virtual void _v050();
    virtual void _v054();
    virtual void _v058();
    virtual void _v05C();
    virtual void _v060();
    virtual void _v064();
    virtual void _v068();
    virtual void _v06C();
    virtual void _v070();
    virtual void _v074();
    virtual void _v078();
    virtual void _v07C();
    virtual void _v080();
    virtual void _v084();
    virtual void _v088();
    virtual void _v08C();
    virtual void _v090();
    virtual void _v094();
    virtual void _v098();
    virtual void _v09C();
    virtual void _v0A0();
    virtual void _v0A4();
    virtual void _v0A8();
    virtual void _v0AC();
    virtual void _v0B0();
    virtual void _v0B4();
    virtual void _v0B8();
    virtual void _v0BC();
    virtual void _v0C0();
    virtual void _v0C4();
    virtual void _v0C8();
    virtual void _v0CC();
    virtual void _v0D0();
    virtual void _v0D4();
    virtual void _v0D8();
    virtual void _v0DC();
    virtual void _v0E0();
    virtual void _v0E4();
    virtual void _v0E8();
    virtual void _v0EC();
    virtual void _v0F0();
    virtual void _v0F4();
    virtual void _v0F8();
    virtual void _v0FC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual void _v11C();
    virtual void _v120();
    virtual void _v124();
    virtual float vf128();     // slot 0x128 - HP
};
extern "C" CMainMenuPlayerSpot* getPlayer__Q22cf13CfGameManagerFi(int index);

// cf::CBattleManager battle-list view (sentinel node at +0x8, nodes at +0x0).
struct CMainMenuBattleNode {
    CMainMenuBattleNode* next;  // 0x00
};
struct CMainMenuBattleMgr {
    u8 _0[0x8];
    CMainMenuBattleNode* list;  // 0x8 - sentinel node
};
#include "kyoshin/cf/CBattleManagerApi.hpp"

// Global settings object returned by getUnk80664658 (flag word at +0x214).
struct CMainMenuGimmickGlobal {
    u8 _00[0x214];
    u32 field_214;              // 0x214 - flag bits
};
extern "C" CMainMenuGimmickGlobal* getUnk80664658();

// CSubCur view at +0xA8 (CBaseCur layout, 0x16 bytes). Cast-only iface for
// the vtable+0x10 dispatch (setRootPaneTranslate, takes a translate VEC3).

// Menu dispatch handlers (defined in CUICfManager.cpp / menu TUs)
extern "C" int func_8029A658();            // party-change notice gate
extern "C" void func_80133D78();
extern "C" void func_801342B0();
extern "C" void func_80134714();
extern "C" void func_80134388();
extern "C" void func_801348C8();
extern "C" void func_80133A08(u32 value);
extern "C" void func_801347EC(u32 value);
extern "C" void func_80134A78();
extern "C" void func_801349A0();
extern "C" void func_80133CA0();
extern "C" void func_80134F2C(u32 value);
extern "C" void func_80134B50(int a, int b);
extern "C" void func_80134C34();
extern "C" void func_80134E50(u32 value);