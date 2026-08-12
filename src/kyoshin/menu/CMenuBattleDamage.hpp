#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class ArcResourceAccessor;
class DrawInfo;
class Pane;
}
namespace math {
struct VEC3;
}
} // namespace nw4r

class CScn;

// 0x38-byte battle-damage entry (stride of the 32-element array at +0x74).
// Each entry owns a big damage layout (+0x00) with three anim transforms and
// a small label layout (+0x14) with one anim transform, plus position/state
// fields used by Move. Layout from the ctor zero-fill and Init.
struct CMenuBattleDamageEntry {
    nw4r::lyt::Layout* mLayout0;      // 0x00 damage layout
    nw4r::lyt::AnimTransform* field_04; // 0x04 selected anim (from mAnim0/1/2)
    nw4r::lyt::AnimTransform* mAnim0; // 0x08
    nw4r::lyt::AnimTransform* mAnim1; // 0x0C
    nw4r::lyt::AnimTransform* mAnim2; // 0x10
    nw4r::lyt::Layout* mLayout1;      // 0x14 label layout
    nw4r::lyt::AnimTransform* mAnim3; // 0x18
    u8 mActive;                       // 0x1C nonzero = slot in use
    u8 _pad1D[3];                     // 0x1D-0x1F
    u32 field_20;                     // 0x20 damage value (func_801098B0 arg)
    u32 field_24;                     // 0x24 actor/target id
    f32 field_28;                     // 0x28 x position
    f32 field_2C;                     // 0x2C y position
    u8 field_30;                      // 0x30 direction
    u8 field_31;                      // 0x31 z-order (cbRenderBefore sort key)
    u8 field_32;                      // 0x32 which layout to draw
    u8 _pad33;                        // 0x33
    f32 field_34;                     // 0x34 anim frame
};

// 0xc-byte queued digit triple (stride of the 0x20-element array at +0x10 of
// CMenuBattleDamageQueue). func_8010A940 appends, func_8010ACC4 draws one.
struct CMenuBattleDamageSlot {
    u32 mVal0; // 0x00
    u32 mVal1; // 0x04
    u32 mVal2; // 0x08
};

// Damage-number queue backing func_8010ACC4: layout + busy flag + digit slots.
struct CMenuBattleDamageQueue {
    u32 field_00;                        // 0x00
    nw4r::lyt::Layout* mLayout;          // 0x04
    u32 field_08;                        // 0x08
    u8 mFlag;                            // 0x0C: 0 = empty, 1 = drawn, 2 = filled
    u8 _pad0D[3];                        // 0x0D-0x0F
    CMenuBattleDamageSlot mSlots[0x20];  // 0x10
};

// Layout for MI subobject offset computation (IWorkEvent at +0x58, IScnRender at +0x5c)
struct CMenuBattleDamageLayout {
    u8 _00[0x58];
    IWorkEvent mIWorkEvent;
    IScnRender mIScnRender;
};

/*
 * Battle damage singleton process.
 *
 * Deliberately NON-polymorphic (the CMenuGetItem / CMenuZeal idiom): the
 * CProcess base region is embedded at +0x00 and the composite vtable lives at
 * +0x10 (written by the factory ctor). The dtor drives the UnkClass_8045F564
 * region at +0x64 and the CProcess base destruction by hand (via their retail
 * dtor symbols); a C++ CProcess base would make MWCC emit an extra vptr store
 * / duplicate subobject destruction that the retail dtor does not have.
 *
 * Layout (from the dtor and the byte setters):
 *   0x00  CProcess region (link nodes, vtable at 0x10, child list, flags)
 *   0x3C  ptmfMove[3] / 0x48 ptmfDraw[3] (__ptmf_null callback slots)
 *   0x54/0x55 state bytes
 *   0x58/0x5C IWorkEvent / IScnRender subobject vtable slots
 *   0x60  CScn* mScn (owning scene)
 *   0x64  UnkClass_8045F564 scratch region (0x10 bytes)
 *   0x774/0x775 byte fields (func_8010975C / func_80109770 setters)
 */
class CMenuBattleDamage {
public:
    CMenuBattleDamage();
    ~CMenuBattleDamage();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    void thunk_IWorkEvent_dtor();
    void thunk_IScnRender_cbRenderBefore();
    void thunk_IScnRender_dtor();

    u8 mProcess[0x10];            // 0x00 CProcess storage head (CDoubleListNode)
    u32 mProcessVt;               // 0x10 vtable slot (composite)
    u8 mProcRest[0x39 - 0x14];    // 0x14..0x38 CProcess tail (CChildListNode)
    u8 mIsRemove;                 // 0x39 CProcess::mIsRemove
    u8 mIsDisableMove;            // 0x3A
    u8 mIsDisableDraw;            // 0x3B
    u32 ptmfMove[3];              // 0x3C null pointer-to-member-function
    u32 ptmfDraw[3];              // 0x48 null pointer-to-member-function
    u8 mField_54;                 // 0x54
    u8 mField_55;                 // 0x55
    u8 _pad56[2];                 // 0x56-0x57
    u32 mIWorkEventVt;            // 0x58 IWorkEvent subobject vtable slot
    u32 mIScnRenderVt;            // 0x5C IScnRender subobject vtable slot
    CScn* mScn;                   // 0x60 owning scene
    UnkClass_8045F564 mMemRegion; // 0x64 UnkClass_8045F564 storage (manual dtor)
    CMenuBattleDamageEntry mEntries[0x20]; // 0x74..0x773 damage entries
    u8 mDamageType;               // 0x774 (func_8010975C setter)
    u8 mDamageDir;                // 0x775 (func_80109770 setter)
    u8 _pad776[0x779 - 0x776];    // 0x776..0x778
    u8 field_0x779;               // 0x779 (func_8010989C setter)
};

/*
 * Battle damage cursor (embedded at CMenuBattlePlayerState+0x7CC).
 *
 * Polymorphic (own vptr at +0x00): the virtual dtor destroys the embedded
 * UnkClass_8045F564 scratch region at +0x04 and frees the object.
 */
class CPcSelectCursor01 {
public:
    virtual ~CPcSelectCursor01();

    void cursorInit();
    u8 cursorGetAnimFrame();

    UnkClass_8045F564 mMemRegion;         // 0x04 scratch region (0x10 bytes)
    void* field_0x14;                     // 0x14
    nw4r::lyt::Layout* mLayout;           // 0x18 layout drawn by cbRenderBefore
    nw4r::lyt::AnimTransform* mAnim1;     // 0x1C first anim transform
    nw4r::lyt::AnimTransform* field_0x20; // 0x20
    nw4r::lyt::AnimTransform* field_0x24; // 0x24
    u8 field_0x28;                        // 0x28
    u8 field_0x29;                        // 0x29
    u8 field_0x2A;                        // 0x2A
    u8 field_0x2B;                        // 0x2B
    u32 field_0x2C;                       // 0x2C state/flag word
    u8 mState;                            // 0x30
    u8 mAnimFrame;                        // 0x31
};

// Opaque view of the func_800B708C(id) actor object (the real class lives in
// the retail module that owns it). The two dispatched virtuals sit at +0xac
// (slot 43) and +0x12c (slot 75): declaring them as real virtuals makes MWCC
// emit the retail dispatch shape (`lwz r12,0(r3); lwz r12,slot(r12)`); a
// cast-and-call helper allocates a temp register instead. field_0x64 bit 1 is
// the actor's "action source" flag read by Move/func_801098B0. Never
// instantiated - only called through.
struct CMenuBattleDamageObjPos;
class CMenuBattleDamageActor {
public:
    virtual void vfn00() = 0;
    virtual void vfn01() = 0;
    virtual void vfn02() = 0;
    virtual void vfn03() = 0;
    virtual void vfn04() = 0;
    virtual void vfn05() = 0;
    virtual void vfn06() = 0;
    virtual void vfn07() = 0;
    virtual void vfn08() = 0;
    virtual void vfn09() = 0;
    virtual void vfn0A() = 0;
    virtual void vfn0B() = 0;
    virtual void vfn0C() = 0;
    virtual void vfn0D() = 0;
    virtual void vfn0E() = 0;
    virtual void vfn0F() = 0;
    virtual void vfn10() = 0;
    virtual void vfn11() = 0;
    virtual void vfn12() = 0;
    virtual void vfn13() = 0;
    virtual void vfn14() = 0;
    virtual void vfn15() = 0;
    virtual void vfn16() = 0;
    virtual void vfn17() = 0;
    virtual void vfn18() = 0;
    virtual void vfn19() = 0;
    virtual void vfn1A() = 0;
    virtual void vfn1B() = 0;
    virtual void vfn1C() = 0;
    virtual void vfn1D() = 0;
    virtual void vfn1E() = 0;
    virtual void vfn1F() = 0;
    virtual void vfn20() = 0;
    virtual void vfn21() = 0;
    virtual void vfn22() = 0;
    virtual void vfn23() = 0;
    virtual void vfn24() = 0;
    virtual void vfn25() = 0;
    virtual void vfn26() = 0;
    virtual void vfn27() = 0;
    virtual void vfn28() = 0;
    virtual nw4r::math::VEC3* vfnAC() = 0;          // slot 43 (+0xac)
    virtual void vfn44() = 0;
    virtual void vfn45() = 0;
    virtual void vfn46() = 0;
    virtual void vfn47() = 0;
    virtual void vfn48() = 0;
    virtual void vfn49() = 0;
    virtual void vfn4A() = 0;
    virtual void vfn4B() = 0;
    virtual void vfn4C() = 0;
    virtual void vfn4D() = 0;
    virtual void vfn4E() = 0;
    virtual void vfn4F() = 0;
    virtual void vfn50() = 0;
    virtual void vfn51() = 0;
    virtual void vfn52() = 0;
    virtual void vfn53() = 0;
    virtual void vfn54() = 0;
    virtual void vfn55() = 0;
    virtual void vfn56() = 0;
    virtual void vfn57() = 0;
    virtual void vfn58() = 0;
    virtual void vfn59() = 0;
    virtual void vfn5A() = 0;
    virtual void vfn5B() = 0;
    virtual void vfn5C() = 0;
    virtual void vfn5D() = 0;
    virtual void vfn5E() = 0;
    virtual void vfn5F() = 0;
    virtual void vfn60() = 0;
    virtual void vfn61() = 0;
    virtual void vfn62() = 0;
    virtual CMenuBattleDamageObjPos* vfn12C(int index) = 0; // slot 75 (+0x12c)
    u8 _04[0x64 - 0x04];                            // +0x04..0x63
    u32 field_0x64;                                 // +0x64
};

// Position components read from the actor->vfn12C(0xfa) result (0x10-byte
// stride: +0x0c / +0x1c / +0x2c).
struct CMenuBattleDamageObjPos {
    u8 _00[0x0c];
    f32 field_0x0c; // +0x0c
    u8 _10[0x1c - 0x10];
    f32 field_0x1c; // +0x1c
    u8 _20[0x2c - 0x20];
    f32 field_0x2c; // +0x2c
};

// Opaque view of the func_80496264(scene, -1) camera/pose block (only passed
// through to func_8049B59C in this unit).
struct CMenuBattleDamagePose;

// Cast-only mirror of the font object returned by
// func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout. With RTTI on, MWCC
// places two hidden slots before the first declared virtual, so the 8th
// declared virtual sits at vtable offset 0x24 and yields the u32 bound into
// the layout's font pane. Never instantiated (no ctor -> no vtable emitted).
struct CMenuBattleDamageFontView {
    virtual void sf0() = 0; // 0x08
    virtual void sf1() = 0; // 0x0C
    virtual void sf2() = 0; // 0x10
    virtual void sf3() = 0; // 0x14
    virtual void sf4() = 0; // 0x18
    virtual void sf5() = 0; // 0x1C
    virtual void sf6() = 0; // 0x20
    virtual u32 sf7() = 0;  // 0x24 font handle getter
};

// Battle-damage singleton instance (retail SDA symbol).
extern CMenuBattleDamage* lbl_eu_80663F28;
// sdata2 float pool (values from CMenuBattlePlayerState sibling).
extern const f32 lbl_eu_80666F94; // 0.0f
extern const f32 lbl_eu_80666F98; // 110.0f
extern const f32 lbl_eu_80666F9C; // 86.0f
extern const f32 lbl_eu_80666FA0; // -178.0f
// Layout pane names / layout file string table (char array).
extern char lbl_eu_804FD720[];

// Unmangled retail ctor symbol (retail `__ct__CMenuBattleDamage`, takes the
// owning scene which is stored at +0x60). Defined as a plain global function
// so MWCC emits the exact linker name; MWCC emits `__`-prefixed global names
// verbatim.
void __ct__CMenuBattleDamage(CMenuBattleDamage* obj, CScn* scene);

// C-ABI imports (retail pre-mangled names; these identifiers must keep C
// linkage so MWCC emits the literal reloc names).
extern "C" {
void __dt__8CProcessFv(CProcess* self, int flags);
void __ct__8CProcessFv(CProcess* self);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564* self, int flags);
void __ct__14Class_8045F858FP17UnkClass_8045F564(Class_8045F858* self,
                                                 UnkClass_8045F564* base);
void __dt__14Class_8045F858Fv(Class_8045F858* self, int dealloc);
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);
nw4r::lyt::ArcResourceAccessor* func_801355F4();
int func_8013BFD8();
int func_8013BE50();
const char* func_80145AA8(int index);
char* func_8013639C(const void*, const void*, int);
void func_8013676C(void*, u32);
// Pre-mangled u8-param name declared with a u32 3rd arg: retail passes the
// full slot word with no rlwinm truncation at the call site (the callee only
// reads the low byte), so the declaration must not force a u32->u8 mask.
void func_80136910__FPQ34nw4r3lyt6LayoutPcUc(nw4r::lyt::Layout*, char*,
                                              u32);
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 arg, nw4r::lyt::Layout* layout);
CMenuBattleDamagePose* func_80496264(CScn* scene, int index);
void func_8049B59C(nw4r::math::VEC3* out, CMenuBattleDamagePose* pose,
                   const nw4r::math::VEC3* in);
char* func_80136190(const void*, const void*, int);
void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
void func_80137E7C(nw4r::lyt::Layout*, const char*);
const char* func_eu_802B142C();
const char* func_eu_802B1444();
const char* func_eu_802B145C();
}

// C++-linkage imports (retail symbols are the Itanium-mangled forms).
void func_80138078(u32 op);  // func_80138078__FUl
u32 func_801355A0();        // func_801355A0__Fv
CMenuBattleDamageActor* func_800B708C(int id);  // func_800B708C__Fi
u32 func_80137444(nw4r::lyt::AnimTransform*, float); // func_80137444__FPQ34nw4r3lyt13AnimTransformf
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*,
                   const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**,
                   nw4r::lyt::ArcResourceAccessor*, char*);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);

// Shared data symbols (global-scope plain externs keep the unmangled names).
extern char lbl_eu_804FD524[];
extern char lbl_eu_8052C1C0[];
extern char lbl_eu_8052C230[];
extern u32 __ptmf_null[3];
extern u32 lbl_eu_80664160;
extern u32 lbl_eu_806640E0;
extern u32 lbl_eu_80663E28;
extern const f32 lbl_eu_80666F68; // 0.0f (zero-fill constant)
extern const f32 lbl_eu_80666F6C; // 1.0f
extern const f32 lbl_eu_80666F70; // screen x offset
extern const f32 lbl_eu_80666F74; // screen y offset
extern const f32 lbl_eu_80666F78; // screen y scale
extern const f32 lbl_eu_80666F7C; // direction offset
extern const f32 lbl_eu_80666F80; // anim frame delay scale
