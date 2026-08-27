#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

class CScn;
class CProcess;
class IScnRender;

namespace nw4r {
namespace lyt {
class AnimTransform;
class ArcResourceAccessor;
}
}

// Minimal view of the object stored at +0x74 (a nw4r::lyt::Layout instance).
// Retail dispatches vtable +0x08 with flag 1 (the deleting-dtor slot),
// +0x2C (SetAnimationEnable) and +0x38 (Animate). The filler virtuals pad
// the intervening layout vtable slots so the real slots land at the retail
// offsets. A plain virtual call emits no null check (retail Term has none);
// `delete` would add one.
class CMenuBattleCommuLayout {
public:
    virtual void release(u32 flag);   // +0x08
    virtual void vf0C();              // +0x0C
    virtual void vf10();              // +0x10
    virtual void vf14();              // +0x14
    virtual void vf18();              // +0x18
    virtual void vf1C();              // +0x1C
    virtual void vf20();              // +0x20
    virtual void vf24();              // +0x24
    virtual void vf28();              // +0x28
    virtual void SetAnimationEnable(nw4r::lyt::AnimTransform* anim,
                                    bool enable);   // +0x2C
    virtual void vf30();              // +0x30
    virtual void vf34();              // +0x34
    virtual void Animate(u32 option = 0);           // +0x38
};

// Font-handle object returned by CDeviceFont::getFontInfo: vtable +0x24
// yields the font handle passed to func_801368C0.
class CBattleCommuFontObj {
public:
    virtual void vf08();              // +0x08
    virtual void vf0C();              // +0x0C
    virtual void vf10();              // +0x10
    virtual void vf14();              // +0x14
    virtual void vf18();              // +0x18
    virtual void vf1C();              // +0x1C
    virtual void vf20();              // +0x20
    virtual u32 getFontHandle();      // +0x24
};

// Sub-object at actor2+0x04 (CObjectParam family): vtable +0x30 yields a
// pointer to the actor-id word fed to func_80174C98.
class CBattleCommuActor2Sub {
public:
    virtual void vf08();              // +0x08
    virtual void vf0C();              // +0x0C
    virtual void vf10();              // +0x10
    virtual void vf14();              // +0x14
    virtual void vf18();              // +0x18
    virtual void vf1C();              // +0x1C
    virtual void vf20();              // +0x20
    virtual void vf24();              // +0x24
    virtual void vf28();              // +0x28
    virtual void vf2C();              // +0x2C
    virtual u32* getActorId();        // +0x30
};

// Actor/voice-action handle returned by func_8016FE34.
struct CBattleCommuActor2 {
    u8 _00[0x04];
    CBattleCommuActor2Sub* mSub04;    // +0x04
};

// Battle sub-object of the player actor (CfObjectActor family): battle-state
// machine id/count plus the target/tech sub-object.
struct CBattleCommuBattleObj {
    u8 _00[0x36C];
    s32 mState36C;                    // +0x36C battle state id (1,2,3,4,7,8 special)
    s32 mCount370;                    // +0x370 party count (clamped to 1)
    struct CBattleCommuSub374* mSub374; // +0x374
};

// Target/tech sub-object reached through BattleObj+0x374.
struct CBattleCommuSub374 {
    u8 _00[0x3F28];
    u16 mId3F28;                      // +0x3F28 item/tech id
};

// Minimal view of cf::CfObjectMove for this TU: only the battle sub-object
// pointer at +0x38 is read.
struct CBattleCommuPlayer {
    u8 _00[0x38];
    CBattleCommuBattleObj* mSub38;    // +0x38
};

/*
 * Battle commu menu screen process (singleton, built by the
 * __ct__CMenuBattleCommu factory).
 *
 * Manual CProcess-style layout (NOT a C++ subclass - the vtable is stored at
 * +0x10 by the factory; novtable so MWCC never writes the +0x00 compiler
 * vptr):
 *   0x00  compiler vptr (never written; novtable)
 *   0x04  CDoubleListNode
 *   0x10  CProcess vtable (temp lbl_eu_8052C1C0, then composite
 *         lbl_eu_80533638)
 *   0x3C  null pointer-to-member-function triple (2x)
 *   0x54/0x55  state bytes
 *   0x58  IWorkEvent dispatch vtable slot (lbl_eu_80533638 + 0x24)
 *   0x5C  IScnRender dispatch vtable slot (lbl_eu_80533638 + 0xac)
 *   0x60  owning CScn (stored by factory, used by Term removeRenderCB)
 *   0x64  UnkClass_8045F564 scratch region (0x10)
 *   0x74  layout/task object (created in Init, deleted in Term)
 *   0x78..0x8C  nw4r animation layers (created in Init)
 *   0x90  state word (init 0)
 *   size 0x94
 */
class __declspec(novtable) CMenuBattleCommu {
public:
    CMenuBattleCommu();
    virtual ~CMenuBattleCommu();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x04-0x0F: CDoubleListNode
    u8 _04[0x10 - 0x04];            // 0x04..0x0F
    u32 mVtable10;                  // 0x10: CProcess vtable (set in factory)
    u8 _14[0x3C - 0x14];            // 0x14..0x3B: rest of CProcess
    u32 mPtMf3C[3];                 // 0x3C..0x47: null PMF copy
    u32 mPtMf48[3];                 // 0x48..0x53: null PMF copy
    u8 mField54;                    // 0x54
    u8 mField55;                    // 0x55
    u8 _56[0x58 - 0x56];            // 0x56..0x57
    u32 mWorkEventVt;               // 0x58: IWorkEvent dispatch vtable slot
    u32 mScnRenderVt;               // 0x5C: IScnRender dispatch vtable slot
    CScn* mScene;                   // 0x60: owning scene
    u8 mMemRegion[0x10];            // 0x64: UnkClass_8045F564 storage
    CMenuBattleCommuLayout* mField74; // 0x74: layout object (deleted in Term)
    nw4r::lyt::AnimTransform* mField78;  // 0x78: animation layer
    nw4r::lyt::AnimTransform* mField7C;  // 0x7C: animation layer
    nw4r::lyt::AnimTransform* mField80;  // 0x80: animation layer
    nw4r::lyt::AnimTransform* mField84;  // 0x84: animation layer
    nw4r::lyt::AnimTransform* mField88;  // 0x88: animation layer
    nw4r::lyt::AnimTransform* mField8C;  // 0x8C: animation layer
    s32 mField90;                   // 0x90: state word
};

// C-ABI imports (retail pre-mangled symbols keep C linkage). CProcess is
// abstract (pure virtuals) so its ctor cannot be placement-new'd; the raw
// C-ABI calls reproduce the retail call shapes (CMenuGetItem idiom).
// CScn.hpp's broken work.hpp umbrella is bypassed the same way as
// CMenuBattleEnd.hpp.
extern "C" {
void __ct__8CProcessFv(CProcess* self);
void __dt__8CProcessFv(CProcess* self, int flags);
void __ct__17UnkClass_8045F564Fv(u8* self);
void __dt__17UnkClass_8045F564Fv(u8* self, int flags);
void removeRenderCB__4CScnFP10IScnRender(CScn* self, IScnRender* cb);
void addRenderCB__4CScnFP10IScnRenderUlUl(CScn* self, IScnRender* cb,
                                          u32 prio, u32 flag);
// Battle actor helpers (unmangled retail symbols).
CBattleCommuActor2* func_8016FE34(CBattleCommuPlayer* source);
void* __dynamic_cast(void* src, long offset, const void* src_type,
                     const void* dst_type, void* src2dst);
// Shared ARC resource accessor / message manager.
nw4r::lyt::ArcResourceAccessor* func_801355F4();
// CDeviceFont font handle (retail called with a single argument; the second
// layout parameter is left unset at this call site).
CBattleCommuFontObj* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg);
// Battle/party presence check (retail passes -1).
int isClassicController__Q22cf13CfGameManagerFv(int arg);
}

// Global data imports (MWCC does not mangle global-scope data names).
extern CMenuBattleCommu* lbl_eu_80664390;   // singleton instance pointer (.sbss)
extern u32 __ptmf_null[3];                  // null pointer-to-member-function
extern char lbl_eu_8052C1C0[];              // temp CProcess vtable
extern char lbl_eu_80533638[];              // CMenuBattleCommu composite vtable
extern char lbl_eu_805047FC[];              // battle commu ARC resource name table
extern const f32 lbl_eu_80667DE0;           // 0.0f (animation frame reset)
extern const f32 lbl_eu_80667DE4;           // animation "reached frame" bound
extern void* lbl_eu_80664090;               // localized message table handle
extern u32 lbl_eu_80663E28;                 // global state flags (bit 21 = event busy)
extern const void* lbl_eu_80661C60;         // RTTI typeinfo (actor source type)
extern const void* lbl_eu_80661BE8;         // RTTI typeinfo (actor dest type)

// UI sound effect cue (mangles to func_80138078__FUl).
void func_80138078(u32);
