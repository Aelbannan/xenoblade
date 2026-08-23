#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "kyoshin/CSystemWindow.hpp"
#include "kyoshin/cf/CfGimmick.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>

/*
 * Key-assign (button mapping) menu controller (singleton, factory
 * __ct__CMenuKeyAssign).
 *
 * Deliberately NON-polymorphic (the CSystemWindow idiom): the CProcess base
 * region is embedded at +0x00 and the UnkClass_8045F564 scratch region at
 * +0x64 is raw storage. The dtor drives both destructions by hand (via their
 * retail dtor symbols); a C++ CProcess base or a real UnkClass_8045F564
 * member would make MWCC emit an extra vptr store / duplicate subobject
 * destruction that the retail dtor does not have.
 *
 * Layout (mirrors CMenuGetItem):
 *   0x00  CProcess storage head (CDoubleListNode)
 *   0x10  CProcess vtable slot (temp, then composite)
 *   0x3C  ptmf Move / ptmf Draw callback slots
 *   0x58  IWorkEvent vtable slot
 *   0x5C  IScnRender vtable slot (render-callback subobject, passed to
 *         CScn::removeRenderCB in Term)
 *   0x60  CScn* owning scene
 *   0x64  UnkClass_8045F564 scratch region storage (manual dtor)
 *   0x74  nw4r::lyt::Layout* drawn in cbRenderBefore
 */
class CMenuKeyAssign {
public:
    CMenuKeyAssign();
    ~CMenuKeyAssign();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void Draw();

    u8 mProcess[0x10];         // 0x00 CProcess storage head (CDoubleListNode)
    u32 mProcessVt;            // 0x10 CProcess vtable slot (temp, then composite)
    u8 mProcRest[0x39 - 0x14]; // 0x14..0x38 CProcess tail (CChildListNode)
    u8 mIsRemove;              // 0x39
    u8 mIsDisableMove;         // 0x3A
    u8 mIsDisableDraw;         // 0x3B
    u32 ptmfMove[3];           // 0x3C
    u32 ptmfDraw[3];           // 0x48
    u8 mField_54;              // 0x54
    u8 mField_55;              // 0x55
    u8 _pad56[2];              // 0x56
    u32 mIWorkEventVt;         // 0x58 - IWorkEvent subobject vtable slot
    u32 mIScnRenderVt;         // 0x5C - IScnRender subobject vtable slot
    CScn* mScn;                // 0x60 - owning scene (removeRenderCB target)
    u8 mUnkClass[0x10];        // 0x64 - scratch region storage (manual dtor)
    nw4r::lyt::Layout* mLayout; // 0x74 - layout drawn in cbRenderBefore
    u32 mField_78;             // 0x78 - open state (func_801159DC: 3 = already open)
};

// View of the nw4r Pane bytes this TU touches (retail nw4r keeps the visible
// flag in the low bit of the byte at +0xBB; the repo's nw4r header exposes it
// as the protected mFlag member, so pane visibility is driven through this
// byte view - CMenuGetItem idiom).
struct PaneFlagView {
    u8 _00[0xBB];
    u8 flags; // 0xBB - bit 0 = visible
};

// Pane view covering the size words (+0x4C/+0x50) and the visible flag
// (+0xBB) - used by func_801159DC to size a picture pane from a texture.
struct PaneSizeFlagView {
    u8 _00[0x4C];
    f32 sizeX; // 0x4C
    f32 sizeY; // 0x50
    u8 _54[0xBB - 0x54];
    u8 flags;  // 0xBB
};

// Texture object returned by the arc accessor's GetResource("timg", ...)
// call in func_801159DC: +0x08 points at the texture header, whose first
// word is a pointer to the u16 width/height pair.
struct TexDimsView {
    u16* mDims; // 0x00
};
struct TexView {
    u8 _00[8];
    TexDimsView* mHeader; // 0x08
};

// u32-word pair / f64 view used for MWCC's 0x43300000 int->float conversion
// (subtracts the retail .sdata2 magic double lbl_eu_80667080).
union F64Conv {
    u32 w[2];
    f64 d;
};

// Mirror of the font object returned by CDeviceFont::func_80452C10: vtable
// slot 0x24 (index 9, no args) yields the u32 bound into the layout's font
// pane via func_8013676C. Never instantiated, so no vtable is emitted; a
// genuine virtual call makes MWCC emit retail's r12 dispatch sequence
// (CMenuQstCntFont / CMenuPTGaugeFont scheme).
class CMenuKeyAssignFont {
public:
    virtual ~CMenuKeyAssignFont();  // 0x00 (3 dtor slots)
    virtual void vf0C();            // 0x0C
    virtual void vf10();            // 0x10
    virtual void vf14();            // 0x14
    virtual void vf18();            // 0x18
    virtual void vf1C();            // 0x1C
    virtual void vf20();            // 0x20
    virtual u32 getFontHandle();    // 0x24
};

// Key-assign singleton instance (retail SDA symbol, .sbss).
extern CMenuKeyAssign* lbl_eu_80663FA8;

// Key-assign string table (region name, layout name, button-pane formats).
extern char lbl_eu_804FDEE8[];
// Temporary (CProcess) vtable, pre-PMF copy; composite vtable afterwards.
extern char lbl_eu_8052BF70[];
// Composite vtable: IWorkEvent sub-vtable at +0x24, IScnRender sub-vtable at +0xac.
extern char lbl_eu_8052C640[];
// sdata2 double 0x4330000000000000 - u32->f32 conversion magic (func_801159DC).
extern f64 lbl_eu_80667080;
// sdata2 f32 compared against the arts-select gauge in Move (arts vf128).
extern f32 lbl_eu_80667078;

// Move's view of the arts-state object (func_8016FE34 result): sub-object at
// +4 whose vtable slot 0x30 returns the actor-id word.
struct CMenuKeyAssignActorSub {
    virtual void vf08();               // +0x08
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual u32* getActorId();         // +0x30
};
struct CMenuKeyAssignActor {
    u8 _00[0x4];
    CMenuKeyAssignActorSub* mSub04;    // +0x04
};

// Move's player view (CfObjectMove): battle sub-object at +0x38 carrying the
// battle-state id at +0x36C and the party count at +0x370.
struct CMenuKeyAssignBattleObj {
    u8 _00[0x36C];
    s32 mState36C;                     // +0x36C
    s32 mCount370;                     // +0x370
};
struct CMenuKeyAssignPlayer {
    u8 _00[0x38];
    CMenuKeyAssignBattleObj* mSub38;   // +0x38
};

// Move's battle-manager view: +0x28/+0x8 list heads, the +0x1A8 sub-object
// (phase byte at +2) and the +0x20C8 s16. Named locally - the shared
// CfGameManager.hpp already declares a CBattleManagerView for another TU.
struct CMenuKeyAssignBattleMgr {
    u8 _00[0x8];
    void* mList08;                     // +0x08
    u8 _0C[0x28 - 0x0C];
    void* mList28;                     // +0x28
    u8 _2C[0x1A8 - 0x2C];
    u8 mSub1A8;                        // +0x1A8 (phase at +2 = 0x1AA)
    u8 _1A9[0x20C8 - 0x1A9];
    s16 mSub20C8;                      // +0x20C8
};

// Circular linked-list node (head at +0x28 / +0x8, next pointer at +0).
struct CListLink {
    CListLink* next;                   // +0x00
};

// func_800B708C result view: +0x64 flags word, +0x91 type byte.
struct CActorTargetView {
    u8 _00[0x64];
    u32 mFlags64;                      // +0x64
    u8 _68[0x91 - 0x68];
    u8 mType91;                        // +0x91
};

// C-ABI import used by cbRenderBefore (retail unmangled name; not declared by
// any shared header yet, so declared here).
extern "C" int func_8013BEE8();

// C-linkage imports (retail names are unmangled free functions; MWCC mangles
// plain global-scope free functions with a __F suffix, so the declarations
// must sit under extern "C" to emit the retail reloc names).
extern "C" {
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void func_80137F88(void* pane, void* tex);
u16 func_8013606C(const void*, const void*, u32);
char* func_80136190(const void*, const void*, int);
void func_80136B4C(nw4r::lyt::Layout*, char*, char*, u32);
void func_8013676C(void*, u32);
// Scratch-region ctor (retail emits the direct C-ABI call with a plain addi;
// a C++ placement new would add a null-check branch the retail does not have).
void __ct__17UnkClass_8045F564Fv(u8* self);
// Move's gate helpers (retail unmangled; declared in their own TUs but not by
// any included header).
int func_801042C8();
int func_801B0F8C();
int func_8017FD44(void*);
int func_800FF738();
// Arts-state gate helpers used by Move (retail unmangled; declared in their
// own TUs but not by any included header).
u32 func_80174C98(void* actor, u32* outVal, u32 flags);
void* func_800BF324(void* objParam);
void* func_8016FE34(void* source);
bool CMenuArtsSelect_isCreated();
}

// C++-mangled import (retail symbol func_800B708C__Fi): actor id -> target.
void* func_800B708C(int id);

// Layout build helper - retail emits the mangled nw4r form
// func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc,
// so declare it as a normal C++ function to let MWCC mangle it (NOT inside
// extern "C").
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);

// Layout draw helper - retail emits the mangled nw4r form
// func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii, so declare it as
// a normal C++ function to let MWCC mangle it (NOT inside extern "C").
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
