#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class AnimTransform;
class ArcResourceAccessor;
class DrawInfo;
}
namespace math {
struct VEC3;
}
}

class CProcess;
class CScn;
class IScnRender;

// Minimal CTaskGame decl (retail symbols getInstance__9CTaskGameFv /
// func_800426F0__9CTaskGameFv; same scheme as CMenuBattleEnd.hpp).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// Unknown per-entry object, deleted via virtual dispatch (vtable+8, flag 1).
// Only the virtual-dtor shape is required -- the real class lives in the
// retail module that owns it.
class CMenuLvUpEntryObj {
public:
    virtual ~CMenuLvUpEntryObj();
};

// Position components read from the actor->vfn12C(0x64) result (0x10-byte
// stride: +0x0c / +0x1c / +0x2c).
struct MenuLvUpObjPos;

// Opaque view of the func_800B708C(id) actor object (the real class lives in
// the retail module that owns it). The two dispatched virtuals sit at +0xac
// (slot 43) and +0x12c (slot 75): declaring them as real virtuals makes MWCC
// emit the retail dispatch shape (`lwz r12,0(r3); lwz r12,slot(r12)`); a
// cast-and-call helper allocates a temp register instead. Never
// instantiated - only called through. field_0x64 is the actor's bit-1
// "action source" flag read during teardown.
class MenuLvUpActor {
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
    virtual MenuLvUpObjPos* vfn12C(int index) = 0;  // slot 75 (+0x12c)
    u8 _04[0x64 - 0x04];                            // +0x04..0x63
    u32 field_0x64;                                 // +0x64
};

// Opaque view of the nw4r root pane's trailing flag byte at +0xbb (retail
// clears its low bit during per-entry teardown).
struct MenuLvUpPaneFlag {
    u8 _00[0xbb];
    u8 field_0xbb; // +0xbb
};

// Opaque view of the func_80496264(scene, -1) camera/pose block (retail reads
// the two anchor VEC3s at +0x10c and +0x138).
struct MenuLvUpPose {
    u8 _00[0x10c];
    nw4r::math::VEC3 field_0x10c; // +0x10c
    u8 _118[0x138 - 0x118];
    nw4r::math::VEC3 field_0x138; // +0x138
};

// Position components read from the actor->vfn12C(0x64) result (0x10-byte
// stride: +0x0c / +0x1c / +0x2c).
struct MenuLvUpObjPos {
    u8 _00[0x0c];
    f32 field_0x0c; // +0x0c
    u8 _10[0x1c - 0x10];
    f32 field_0x1c; // +0x1c
    u8 _20[0x2c - 0x20];
    f32 field_0x2c; // +0x2c
};

// One slot of the 3-entry array at +0x74 (0x38 stride).
struct CMenuLvUpEntry {
    nw4r::lyt::Layout* field_0x00;        // +0x00 first layout
    nw4r::lyt::AnimTransform* field_0x04; // +0x04 first anim
    nw4r::lyt::Layout* field_0x08;        // +0x08 second layout
    nw4r::lyt::AnimTransform* field_0x0C; // +0x0C second anim
    u32 field_0x10;                       // +0x10 actor id (func_800B708C)
    u8 field_0x14;                        // +0x14 state (0/1/2/3)
    u8 field_0x15;                        // +0x15 last-processed queue state
    u8 field_0x16[0x20];                  // +0x16..0x35 queued-state list (0 = empty)
    u8 field_0x36;                        // +0x36
    u8 field_0x37;                        // +0x37
};

// novtable: the retail dtor does not write the vptr (it is set manually by
// the ctor), so suppress the compiler's implicit vptr store in the dtor.
class __declspec(novtable) CMenuLvUp {
public:
    virtual ~CMenuLvUp();
    CMenuLvUp();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // Compiler vptr at 0x00, then a manual CProcess-style layout (retail
    // stores the CProcess vtable at +0x10; 0x3C-0x53 is the ptmf zone).
    u8 _04[0x10 - 0x04];            // 0x04-0x0F: CDoubleListNode
    u32 mVtable10;                  // 0x10: CProcess vtable (set in ctor)
    u8 _14[0x3C - 0x14];            // 0x14-0x3B: rest of CProcess
    u32 mPtMf3C[3];                 // 0x3C-0x47: run-time ptmf hook (null copy)
    u32 mPtMf48[3];                 // 0x48-0x53: run-time ptmf hook (null copy)
    u8 mField54;                    // 0x54
    u8 mField55;                    // 0x55
    u8 _56[0x58 - 0x56];
    u32 mIWorkEventVtbl;            // 0x58: IWorkEvent vtable slot (manual)
    u32 mIScnRenderVtbl;            // 0x5C: IScnRender vtable slot (manual)
    u8 mMemRegion[0x10];            // 0x60: UnkClass_8045F564 storage
    CScn* mScene;                   // 0x70
    CMenuLvUpEntry mEntries[3];     // 0x74 (0x38 each -> 0x74..0x11B)
    u8 mField11C;                   // 0x11C: menu-active flag
    u8 _11D[0x120 - 0x11D];         // tail (obj size 0x120)
};

// Retail-symbol imports. Data keeps its plain name; functions need C linkage
// so MWCC emits the unmangled retail names (same scheme as CMenuBattleEnd.hpp
// / CMenuCollepedia.hpp). func_80276B14 / func_802768E0 / func_802764A0 are
// the per-entry helpers driven by Move; __ct__CMenuLvUp is the allocating
// factory (retail unmangled, takes the parent process + scene).
extern CMenuLvUp* lbl_eu_80664900;
extern "C" {
void func_80276200(CMenuLvUp* a, CMenuLvUp* b, int c);
void func_80276B14(CMenuLvUp* self, CMenuLvUpEntry* entry);
void func_802768E0(CMenuLvUp* self, CMenuLvUpEntry* entry);
void func_802764A0(CMenuLvUp* self, CMenuLvUpEntry* entry);
CMenuLvUp* __ct__CMenuLvUp(CProcess* parent, CScn* scene);
MenuLvUpPose* func_80496264(CScn* scene, int index);
void func_8049B59C(nw4r::math::VEC3* out, MenuLvUpPose* pose,
                   const nw4r::math::VEC3* in);
int func_8013A4B4(const nw4r::math::VEC3* a, const nw4r::math::VEC3* b,
                  const nw4r::math::VEC3* c);
void func_802A2E68(int id);
void waitForDrawDone__9CDeviceVIFv();
void removeRenderCB__4CScnFP10IScnRender(CScn*, IScnRender*);
void addRenderCB__4CScnFP10IScnRenderUlUl(CScn*, IScnRender*, u32, u32);
void __dt__8CProcessFv(CProcess* obj, int flags);
void __ct__8CProcessFv(CProcess* obj);
void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                   bool insertTop);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* mem);
void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo);
void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* drawInfo, int flags);
bool func_800829B8__Q22cf13CfGameManagerFv();
nw4r::lyt::ArcResourceAccessor* func_801355F4();
}

// C++-linkage imports (retail symbols are the Itanium-mangled forms).
MenuLvUpActor* func_800B708C(int id);
void func_80138078(u32);   // UI sound effect (func_80138078__FUl)

// C-linkage data imports (retail names; global-scope plain externs keep the
// unmangled symbol -- MWCC does not mangle global variables).
extern u32 lbl_eu_8052BF70[];    // CMenuLvUp primary vtable
extern u32 lbl_eu_805380C8[];    // dispatch vtable cluster base
extern u32 __ptmf_null[];        // null member-function-pointer triple
extern char lbl_eu_8050EC70[];   // layout/animation name string table
extern f32 lbl_eu_80668A00;      // anim frame reset value
extern f32 lbl_eu_80668A04;      // anim frame target for func_80137444
extern f32 lbl_eu_80668A08;      // screen-position post-scale offset x
extern f32 lbl_eu_80668A0C;      // screen-position post-scale offset y
extern f32 lbl_eu_80668A10;      // screen-position post-scale factor y
extern u32 lbl_eu_80663E28;      // task-state flag word (bit 21 gates the menu)
