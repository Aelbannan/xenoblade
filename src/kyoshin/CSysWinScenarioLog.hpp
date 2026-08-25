#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/IScnRender.hpp"

class CFileHandle;
class CScn;
class CBattleManagerView;

namespace nw4r { namespace lyt {
    class Layout;
    class AnimTransform;
    class ArcResourceAccessor;
    class DrawInfo;
    class Pane;
}}

// Render-callback APIs live on the real monolib CScn (monolib/scn/CScn.hpp,
// pulled in by CTaskGame.hpp at the cpp use site) - only a forward decl is
// needed here for the mScene pointer member.

// Null pointer-to-member-function constant (12 bytes; copied word-by-word into
// the two callback slots).
extern u32 __ptmf_null[3];

class CSysWinScenarioLog : public CProcess {
public:
    CSysWinScenarioLog();
    virtual ~CSysWinScenarioLog();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent();

    /* 0x3c-0x63: region owned by layout / IWorkEvent sub-object (same
       IUIWindow base region as CSystemWindow: two null ptmfs then controls) */
    u32 ptmf0[3];              // 0x3C null pointer-to-member-function
    u32 ptmf1[3];              // 0x48 null pointer-to-member-function
    u32 field_54;              // 0x54
    u32 field_58;              // 0x58
    u32 field_5C;              // 0x5C
    u32 field_60;              // 0x60 (init to -1)
    /* 0x64 */ u8 mFlag64;
    /* 0x65-0x66 */ u8 _65[2];
    /* 0x67 */ u8 mFlag67;
    u32 field_68;              // 0x68
    u32 mWorkEvent;            // 0x6C IWorkEvent vtable slot
    u32 mScnRender;            // 0x70 IScnRender vtable slot
    CFileHandle* mFileHandle74;  // 0x74 (released by func_801390E0 in Term)
    CFileHandle* mFileHandle78;  // 0x78 (released by func_801390E0 in Term)
    /* 0x7c: UnkClass_8045F564 embedded region (dtor calls ~UnkClass_8045F564) */
    UnkClass_8045F564 mMemRegion;
    CScn* mScene;              // 0x8C owning scene (ctor param)
    /* 0x90 */ u8 mState;    // Move() state machine
    u8 _91[3];                 // 0x91-0x93
    nw4r::lyt::Layout* mField94;  // 0x94 (Term calls its vtable slot-2 with 1)
    nw4r::lyt::ArcResourceAccessor* mpAccessor;  // 0x98
    /* 0x9c */ nw4r::lyt::Layout* mpLayout;
    /* 0xa0 */ nw4r::lyt::AnimTransform* mpAnim;
};

// View of the CProcess base vptr slot: the C-linkage ctor writes the IUIWindow
// temp vtable (lbl_eu_8052D238) then the composite vtable (lbl_eu_80538358) at
// +0x10, mirroring the CSystemWindow ctor.
struct CSysWinProcessVtable {
    u8 _0[0x10];
    u32 mpVtable;              // +0x10
};

// Term() helper view: a layout object's vtable-slot-2 method (retail vtable
// offset 0x8, called with argument 1 before dropping the layout reference).
// With -RTTI the prefix shifts index N to offset (N+2)*4, so index 0 lands
// exactly on 0x8. A genuine virtual call makes MWCC emit the retail r12-chain
// (`lwz r12,0(rN); lwz r12,0x8(r12); mtctr; bctrl`).
class CSysWinLayoutSlot2View {
public:
    virtual void mSlot2(u32 arg);   // index 0 -> vtable 0x8
};

// func_80280588 vtable view on the device object: with -RTTI, index N sits at
// vtable offset (N+2)*4, so mAt9C (index 37) lands on 0x9c and mAt2BC (index
// 173) on 0x2bc. The slab virtuals are padding to reach those indices.
struct CScenarioLogOwner;
class CSysWinDevView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual void v0D() = 0;
    virtual void v0E() = 0;
    virtual void v0F() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v1A() = 0;
    virtual void v1B() = 0;
    virtual void v1C() = 0;
    virtual void v1D() = 0;
    virtual void v1E() = 0;
    virtual void v1F() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual CScenarioLogOwner* mAt9C() = 0;  // index 37 -> vtable 0x9c
    virtual void vEC() = 0;
    virtual void vED() = 0;
    virtual void vEE() = 0;
    virtual void vEF() = 0;
    virtual void vF0() = 0;
    virtual void vF1() = 0;
    virtual void vF2() = 0;
    virtual void vF3() = 0;
    virtual void vF4() = 0;
    virtual void vF5() = 0;
    virtual void vF6() = 0;
    virtual void vF7() = 0;
    virtual void vF8() = 0;
    virtual void vF9() = 0;
    virtual void vFA() = 0;
    virtual void vFB() = 0;
    virtual void vFC() = 0;
    virtual void vFD() = 0;
    virtual void vFE() = 0;
    virtual void vFF() = 0;
    virtual void v100() = 0;
    virtual void v101() = 0;
    virtual void v102() = 0;
    virtual void v103() = 0;
    virtual void v104() = 0;
    virtual void v105() = 0;
    virtual void v106() = 0;
    virtual void v107() = 0;
    virtual void v108() = 0;
    virtual void v109() = 0;
    virtual void v10A() = 0;
    virtual void v10B() = 0;
    virtual void v10C() = 0;
    virtual void v10D() = 0;
    virtual void v10E() = 0;
    virtual void v10F() = 0;
    virtual void v110() = 0;
    virtual void v111() = 0;
    virtual void v112() = 0;
    virtual void v113() = 0;
    virtual void v114() = 0;
    virtual void v115() = 0;
    virtual void v116() = 0;
    virtual void v117() = 0;
    virtual void v118() = 0;
    virtual void v119() = 0;
    virtual void v11A() = 0;
    virtual void v11B() = 0;
    virtual void v11C() = 0;
    virtual void v11D() = 0;
    virtual void v11E() = 0;
    virtual void v11F() = 0;
    virtual void v120() = 0;
    virtual void v121() = 0;
    virtual void v122() = 0;
    virtual void v123() = 0;
    virtual void v124() = 0;
    virtual void v125() = 0;
    virtual void v126() = 0;
    virtual void v127() = 0;
    virtual void v128() = 0;
    virtual void v129() = 0;
    virtual void v12A() = 0;
    virtual void v12B() = 0;
    virtual void v12C() = 0;
    virtual void v12D() = 0;
    virtual void v12E() = 0;
    virtual void v12F() = 0;
    virtual void v130() = 0;
    virtual void v131() = 0;
    virtual void v132() = 0;
    virtual void v133() = 0;
    virtual void v134() = 0;
    virtual void v135() = 0;
    virtual void v136() = 0;
    virtual void v137() = 0;
    virtual void v138() = 0;
    virtual void v139() = 0;
    virtual void v13A() = 0;
    virtual void v13B() = 0;
    virtual void v13C() = 0;
    virtual void v13D() = 0;
    virtual void v13E() = 0;
    virtual void v13F() = 0;
    virtual void v140() = 0;
    virtual void v141() = 0;
    virtual void v142() = 0;
    virtual void v143() = 0;
    virtual void v144() = 0;
    virtual void v145() = 0;
    virtual void v146() = 0;
    virtual void v147() = 0;
    virtual void v148() = 0;
    virtual void v149() = 0;
    virtual void v14A() = 0;
    virtual void v14B() = 0;
    virtual void v14C() = 0;
    virtual void v14D() = 0;
    virtual void v14E() = 0;
    virtual void v14F() = 0;
    virtual void v150() = 0;
    virtual void v151() = 0;
    virtual void v152() = 0;
    virtual void v153() = 0;
    virtual void v154() = 0;
    virtual void v155() = 0;
    virtual void v156() = 0;
    virtual void v157() = 0;
    virtual void v158() = 0;
    virtual void v159() = 0;
    virtual void v15A() = 0;
    virtual void v15B() = 0;
    virtual void v15C() = 0;
    virtual void v15D() = 0;
    virtual void v15E() = 0;
    virtual void v15F() = 0;
    virtual void v160() = 0;
    virtual void v161() = 0;
    virtual void v162() = 0;
    virtual void v163() = 0;
    virtual void v164() = 0;
    virtual void v165() = 0;
    virtual void v166() = 0;
    virtual void v167() = 0;
    virtual void v168() = 0;
    virtual void v169() = 0;
    virtual void v16A() = 0;
    virtual void v16B() = 0;
    virtual void v16C() = 0;
    virtual void v16D() = 0;
    virtual void v16E() = 0;
    virtual void v16F() = 0;
    virtual void v170() = 0;
    virtual void v171() = 0;
    virtual void v172() = 0;
    virtual void* mAt2BC() = 0;             // index 173 -> vtable 0x2bc
};

// func_8028120C slot-table view: a 0x80-byte entry carries a used flag at +0x0
// and its vtable pointer at +0x70; table A holds 3 entries at +0x00, table B
// holds 0x38 entries at +0x180, and the two round-robin indexes live at
// +0x1D80 / +0x1D84.
struct CSysWinSlotEntry {
    u32 mUsed;                 // +0x0 (non-zero = slot taken)
    u8 _4[0x70 - 0x4];
    void* mpVtable;            // +0x70
    u8 _74[0x80 - 0x74];
};
struct CSysWinSlotTable {
    CSysWinSlotEntry mEntriesA[3];      // +0x00
    CSysWinSlotEntry mEntriesB[0x38];   // +0x180
    int mIdxA;                          // +0x1D80 (cap 3)
    int mIdxB;                          // +0x1D84 (cap 0x38)
};

// func_8028120C helper: call the found slot entry's virtual at vtable offset
// 0xc (slot 3). Raw dispatch - retail has no null guard at this call site.
// Written as a single expression so MWCC chains both loads through r12 like
// retail (a named vtable local spills it to a second register).
inline void csysWinSlotCall3(CSysWinSlotEntry* entry) {
    typedef void (*Fn)(CSysWinSlotEntry*);
    ((Fn)((u32*)entry->mpVtable)[3])(entry);
}


// Minimal intrusive singly-linked list node used by the battle-list scans
// in this unit (next pointer at +0x00).
struct CSysWinListNode {
    CSysWinListNode* next; // +0x00
};

// Actor/device-list walk used by func_80280640 / func_8027F148: the list
// returned by func_800B6BA4__Fv shares the CErrMesList shape (sentinel at
// +0x04, item pointer at +0x08 pointing at the embedded +0x3E9C sub-object).
struct CSysWinActorListNode {
    CSysWinActorListNode* next;  // 0x00
    u8 _04[4];                   // 0x04
    u8* object;                  // 0x08 - item (points at the +0x3E9C sub-object)
};
struct CSysWinActorList {
    u8* field_0x0;               // 0x00
    CSysWinActorListNode* sentinel;  // 0x04
};

// Voice/actor-manager list accessor (retail mangled name func_800B6BA4__Fv -
// plain C++ linkage reproduces the Fv suffix).
CSysWinActorList* func_800B6BA4();

// func_8027F148 view: the owner's +0x4 sub-object carries a virtual at vtable
// offset 0x30 returning an object whose field_0 feeds the func_80174C98
// arts-state gate. With -RTTI, index N sits at (N+2)*4, so mAt30 (index 10)
// lands on 0x30.
struct CSysWinSubResult {
    u32 field_0;                     // +0x0
};
struct CSysWinOwnerSubView {
    u8 _0[4];
    void* field_0x4;                 // +0x4
};
class CSysWinSubObjView {
public:
    virtual void v00() = 0;  // 0x08
    virtual void v01() = 0;  // 0x0C
    virtual void v02() = 0;  // 0x10
    virtual void v03() = 0;  // 0x14
    virtual void v04() = 0;  // 0x18
    virtual void v05() = 0;  // 0x1C
    virtual void v06() = 0;  // 0x20
    virtual void v07() = 0;  // 0x24
    virtual void v08() = 0;  // 0x28
    virtual void v09() = 0;  // 0x2C
    virtual CSysWinSubResult* mAt30() = 0;  // index 10 -> vtable 0x30
};

// func_8027EC80 font-object view: vtable slot 0x24 returns the font handle
// handed to func_8013676C (same shape as CTalkWindow's CTalkFontObj).
class CSysWinFontObjView {
public:
    virtual void v00() = 0;  // 0x08
    virtual void v01() = 0;  // 0x0C
    virtual void v02() = 0;  // 0x10
    virtual void v03() = 0;  // 0x14
    virtual void v04() = 0;  // 0x18
    virtual void v05() = 0;  // 0x1C
    virtual void v06() = 0;  // 0x20
    virtual u32 getFontHandle() = 0;  // index 7 -> 0x24
};

// func_8027EC80 / func_8027EA6C root-pane view: a custom virtual at vtable
// offset 0x3C (name + flag, returns the pane). The trailing region carries
// the tag-processor pointer (+0xF8) and the texture size floats (+0x4C/+0x50).
class CSysWinPaneView {
public:
    virtual void v00() = 0;  // 0x08
    virtual void v01() = 0;  // 0x0C
    virtual void v02() = 0;  // 0x10
    virtual void v03() = 0;  // 0x14
    virtual void v04() = 0;  // 0x18
    virtual void v05() = 0;  // 0x1C
    virtual void v06() = 0;  // 0x20
    virtual void v07() = 0;  // 0x24
    virtual void v08() = 0;  // 0x28
    virtual void v09() = 0;  // 0x2C
    virtual void v0A() = 0;  // 0x30
    virtual void v0B() = 0;  // 0x34
    virtual void v0C() = 0;  // 0x38
    virtual void* mSetText(const char* text, u32 flag) = 0;  // index 13 -> 0x3C
    u8 _04[0x4C - 0x04];
    f32 field_0x4C;           // +0x4C texture width
    f32 field_0x50;           // +0x50 texture height
    u8 _54[0xF8 - 0x54];
    u32 field_0xF8;           // +0xF8 tag-processor pointer
};

// func_8027EA6C texture-result view: the GetResource result carries a dims
// pointer at +0x8 (u16 width at +2, u16 height at +0).
struct CSysWinTexDims {
    u16 field_0x0;            // +0x0 height
    u16 field_0x2;            // +0x2 width
};
struct CSysWinTexMeta {
    u8 _0[0x8];
    CSysWinTexDims* field_0x8;  // +0x8
};

// u16 -> f32 conversion helper (CfTFile.cpp convention): build the
// 0x43300000-prefixed bit pattern in memory and subtract the named 2^52
// sdata2 magic (lbl_eu_80668AD8) so the pool reloc matches retail instead of
// an MWCC-synthesised @N entry.
union CSysWinF64Conv {
    u32 w[2];
    double d;
};

// func_80280F44 view: character-slot entry at +0x3534 of the func_8009EC9C
// data (0xC4 stride; five sub-slot pointers at +0x20..+0xA0, 0x20 apart).
struct CSysWinCharSlot {
    u8 _0[0x20];
    void* sub0;             // +0x20
    u8 _24[0x40 - 0x24];
    void* sub1;             // +0x40
    u8 _44[0x60 - 0x44];
    void* sub2;             // +0x60
    u8 _64[0x80 - 0x64];
    void* sub3;             // +0x80
    u8 _84[0xA0 - 0x84];
    void* sub4;             // +0xA0
    u8 _A4[0xC4 - 0xA4];
};

// Battle-manager view used by func_8027EF50: the list sentinel node lives at
// +0x48 and the iteration counts nodes until it reaches the sentinel again.
struct CSysWinBattleMgrView {
    u8 _0[0x48];
    CSysWinListNode* mListSentinel; // +0x48
};

// Layout vtable hook: lets Move() call the nw4r Layout virtual at vtable
// offset 0x38 (14 slab padding virtuals then the slot-14 method).
class CSysWinLayoutHook {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void sA();
    virtual void sB();
    virtual void sC();
    virtual void sD();
    virtual void mAt38(u32 arg);   // vtable slot 14 == offset 0x38
};

// Pad-button view used by Move() (button word at +0x4).
struct CSysWinPadView {
    u8 _0[4];
    u32 mButtons; // +0x4
};

// Sub-object structure used by func_8027F2DC: the owner stores a pointer to a
// small sub-state object at +0x3F60 whose +0x8 points at an inner object with
// a u32 sub-state value at +0x18.
struct CScenarioLogSubInner {
    u8 _0[0x18];
    u32 field_0x18;                      // +0x18 sub-state value
};
struct CScenarioLogSub {
    u8 _0[0x8];
    CScenarioLogSubInner* field_0x8;     // +0x8
};

// func_8027F848 / func_8027F2DC / func_8027FC80 argument view: a big object
// with a scenario-flag dword at +0x3F00 (bit 1 = gate), a u16 flag at
// +0x3E6C (bit 12), and an embedded player object whose address at +0x3E9C is
// compared against CfGameManager::getPlayer(0). The sub-state tracking fields
// (+0x3F28 u16 index, +0x3F60 sub-object pointer) are read by func_8027F2DC.
struct CScenarioLogOwner {
    u8 _0[0x4];
    void* field_0x4;                     // +0x4 arts-state source object (func_8027F148)
    u8 _8[0x3374 - 0x8];
    u32 field_0x3374;                    // +0x3374 device flags (func_80280640)
    u8 _3378[0x3E6C - 0x3378];
    u16 field_0x3E6C;                    // +0x3E6C flags (bit 12 = forced player)
    u8 _3E6E[0x3E9C - 0x3E6E];
    u8 field_0x3E9C;                     // +0x3E9C embedded player marker (address compared to getPlayer(0))
    u8 _3E9D[0x3F00 - 0x3E9D];
    u32 field_0x3F00;                    // +0x3F00 scenario flag word (bit 0 = gate)
    u8 _3F04[0x3F28 - 0x3F04];
    u16 field_0x3F28;                    // +0x3F28 sub-state index (0x8 -> base 3)
    u8 _3F2A[0x3F60 - 0x3F2A];
    CScenarioLogSub* field_0x3F60;       // +0x3F60 sub-state object
};

// Type returned by func_8027FC80's per-object virtual calls (vtable offset
// 0x224): the field_0 scalar drives the 0x5d / 0x5e close comparison.
struct CSysWinDevEntry {
    u32 field_0;                          // +0x0
};

// func_8027FC80 arg0 view: the device object. Only the flag word at +0x3374
// is read directly here; the vtable calls are dispatched via the helpers below.
struct CSysWinDevice {
    u8 _0[0x3374];
    u32 field_0x3374;                     // +0x3374 flags
};

// func_8027FC80 helper: invoke a virtual at a runtime vtable offset on a raw
// object pointer. MWCC cannot express these as member calls without emitting a
// whole new vtable, so the call goes through the object's stored vtable.
inline u32 csysWinCallE0(void* self) {
    typedef u32 (*Fn)(void*);
    void** vt = (void**)*(void**)self;
    return ((Fn)vt[0xE0 / 4])(self);
}
inline CScenarioLogOwner* csysWinCall9C(void* self) {
    typedef CScenarioLogOwner* (*Fn)(void*);
    void** vt = (void**)*(void**)self;
    return ((Fn)vt[0x9C / 4])(self);
}
inline CSysWinDevEntry* csysWinCall224(void* self) {
    typedef CSysWinDevEntry* (*Fn)(void*);
    void** vt = (void**)*(void**)self;
    return ((Fn)vt[0x224 / 4])(self);
}

// Vtable view of the scenario-log owner's virtual table used by
// func_802807A0 / func_8027FC04: the dispatch target sits at retail vtable
// offset 0x28c. With -RTTI on, MWCC inserts a 2-entry (offset-to-top +
// typeinfo) vtable prefix, so a virtual declared at index N lands at vtable
// offset (N+2)*4: the 161 slab virtuals below place mAt28C at index 161 ->
// (161+2)*4 = 0x28c. A genuine virtual call makes MWCC emit the retail
// `lwz r12, 0(rN) / lwz r12, 0x28c(r12) / mtctr / bctrl` dispatch sequence.
class CSysWinOwnerView {
public:
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v0A() = 0;
    virtual void v0B() = 0;
    virtual void v0C() = 0;
    virtual void v0D() = 0;
    virtual void v0E() = 0;
    virtual void v0F() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v1A() = 0;
    virtual void v1B() = 0;
    virtual void v1C() = 0;
    virtual void v1D() = 0;
    virtual void v1E() = 0;
    virtual void v1F() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v2A() = 0;
    virtual void v2B() = 0;
    virtual void v2C() = 0;
    virtual void v2D() = 0;
    virtual void v2E() = 0;
    virtual void v2F() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0;
    virtual void v33() = 0;
    virtual void v34() = 0;
    virtual void v35() = 0;
    virtual void v36() = 0;
    virtual void v37() = 0;
    virtual void v38() = 0;
    virtual void v39() = 0;
    virtual void v3A() = 0;
    virtual void v3B() = 0;
    virtual void v3C() = 0;
    virtual void v3D() = 0;
    virtual void v3E() = 0;
    virtual void v3F() = 0;
    virtual void v40() = 0;
    virtual void v41() = 0;
    virtual void v42() = 0;
    virtual void v43() = 0;
    virtual void v44() = 0;
    virtual void v45() = 0;
    virtual void v46() = 0;
    virtual void v47() = 0;
    virtual void v48() = 0;
    virtual void v49() = 0;
    virtual void v4A() = 0;
    virtual void v4B() = 0;
    virtual void v4C() = 0;
    virtual void v4D() = 0;
    virtual void v4E() = 0;
    virtual void v4F() = 0;
    virtual void v50() = 0;
    virtual void v51() = 0;
    virtual void v52() = 0;
    virtual void v53() = 0;
    virtual void v54() = 0;
    virtual void v55() = 0;
    virtual void v56() = 0;
    virtual void v57() = 0;
    virtual void v58() = 0;
    virtual void v59() = 0;
    virtual void v5A() = 0;
    virtual void v5B() = 0;
    virtual void v5C() = 0;
    virtual void v5D() = 0;
    virtual void v5E() = 0;
    virtual void v5F() = 0;
    virtual void v60() = 0;
    virtual void v61() = 0;
    virtual void v62() = 0;
    virtual void v63() = 0;
    virtual void v64() = 0;
    virtual void v65() = 0;
    virtual void v66() = 0;
    virtual void v67() = 0;
    virtual void v68() = 0;
    virtual void v69() = 0;
    virtual void v6A() = 0;
    virtual void v6B() = 0;
    virtual void v6C() = 0;
    virtual void v6D() = 0;
    virtual void v6E() = 0;
    virtual void v6F() = 0;
    virtual void v70() = 0;
    virtual void v71() = 0;
    virtual void v72() = 0;
    virtual void v73() = 0;
    virtual void v74() = 0;
    virtual void v75() = 0;
    virtual void v76() = 0;
    virtual void v77() = 0;
    virtual void v78() = 0;
    virtual void v79() = 0;
    virtual void v7A() = 0;
    virtual void v7B() = 0;
    virtual void v7C() = 0;
    virtual void v7D() = 0;
    virtual void v7E() = 0;
    virtual void v7F() = 0;
    virtual void v80() = 0;
    virtual void v81() = 0;
    virtual void v82() = 0;
    virtual void v83() = 0;
    virtual void v84() = 0;
    virtual void v85() = 0;
    virtual void v86() = 0;
    virtual void v87() = 0;
    virtual void v88() = 0;
    virtual void v89() = 0;
    virtual void v8A() = 0;
    virtual void v8B() = 0;
    virtual void v8C() = 0;
    virtual void v8D() = 0;
    virtual void v8E() = 0;
    virtual void v8F() = 0;
    virtual void v90() = 0;
    virtual void v91() = 0;
    virtual void v92() = 0;
    virtual void v93() = 0;
    virtual void v94() = 0;
    virtual void v95() = 0;
    virtual void v96() = 0;
    virtual void v97() = 0;
    virtual void v98() = 0;
    virtual void v99() = 0;
    virtual void v9A() = 0;
    virtual void v9B() = 0;
    virtual void v9C() = 0;
    virtual void v9D() = 0;
    virtual void v9E() = 0;
    virtual void v9F() = 0;
    virtual void vA0() = 0;
    virtual CSysWinDevice* mAt28C() = 0;   // index 161 -> vtable 0x28c
};

// func_802807A0 / func_8027FC04 helper: invoke the virtual at vtable offset
// 0x28c on a scenario-log owner object. Retail uses the returned device/window
// object as an argument to func_8027FC80.
inline CSysWinDevice* csysWinCall28C(CScenarioLogOwner* self) {
    return ((CSysWinOwnerView*)self)->mAt28C();
}

// arg2 view for func_8027F848: window/state dword at +0x74 (bit 8) and
// +0x78 (bit 1).
struct CScenarioLogArg2 {
    u8 _0[0x74];
    u32 field_0x74;                      // +0x74 (bit 8 = battle window)
    u32 field_0x78;                      // +0x78 (bit 1)
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - do not "fix" the mangled forms).
// Relocated from CSysWinScenarioLog.cpp so every TU that uses these symbols
// gets them from this header instead of inline pseudo-import declarations.
// ---------------------------------------------------------------------------

// Retail callees referenced by the (still-unmatched) CSysWinScenarioLog unit.
// The CfGameManager "sequence/flag" helpers are mangled Fv in retail even
// though they take an argument (see CfMapMineManager / code_80135FDC for the
// same convention), so reference them via their retail mangled names.
extern "C" u32  func_80082694__Q22cf13CfGameManagerFv(u32 id);   // get sequence value
extern "C" void func_8008269C__Q22cf13CfGameManagerFv(u32 id, u32 value); // set sequence value
extern "C" void func_800826F0__Q22cf13CfGameManagerFv(u32 value);
extern "C" u32  func_800822F4__Q22cf13CfGameManagerFv(); // unsigned cf sequence counter
extern "C" CBattleManagerView* getInstance__Q22cf14CBattleManagerFv();
extern "C" int   func_800DA06C(void* bm, void* obj); // battle-list membership check
extern "C" void* func_8016FE34(void* source);
extern "C" u16  lbl_eu_80664772;          // pause / non-enemy-scene flag
extern "C" u16  lbl_eu_80663E42;          // current area id (func_8027FC80 gate)
extern "C" u32  lbl_eu_80664908;          // CSysWinScenarioLog singleton
extern "C" u8   lbl_eu_80664910;
extern "C" u8   lbl_eu_80664911;
extern "C" u8   lbl_eu_80664912;
extern "C" u32  lbl_eu_80664914;
extern "C" u8   lbl_eu_80664918;
extern u32 lbl_eu_8066490C;              // Term clears this (.sbss)

// IUIWindow base vtable + null pmf constant used by the C-linkage ctor (same
// symbols as CSystemWindow's ctor; the composite vtable differs).
extern u32 __ptmf_null[3];
extern char lbl_eu_8052D238[];
extern char lbl_eu_80538358[];

extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
extern "C" void __dt__8CProcessFv(void*, int);
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" CSysWinScenarioLog* __ct__CSysWinScenarioLog(CSysWinScenarioLog* _this, void* param); // returns this

// Term / ctor imports (retail C-ABI names; the CfGameManager helper is Fv-mangled
// but takes a vestigial bool arg - same convention as func_80086F9C above).
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
extern "C" u8   func_8013B980();
extern "C" u8   code80135FDC_getByte_64080();
extern "C" void code80135FDC_postIncByte_64080();
extern "C" void func_8003AA8C__5CBdatFUl(u32);

// C++-linkage helpers (MWCC mangles the plain names to the retail forms
// func_801390E0__FPP11CFileHandle / func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor).
void func_801390E0(CFileHandle** handle);
void func_80139124(nw4r::lyt::ArcResourceAccessor* accessor);

// Float thresholds used by func_8027EF50's player-range check.
extern "C" f32 lbl_eu_80668AE0;
extern "C" f32 lbl_eu_80668AE4;

// Scenario-log unit imports (func_80280640 / func_8027F148 / func_8027EC80 /
// func_8027EA6C / func_80280F44). Retail-unmangled C-ABI symbols.
extern "C" void* func_8009EC9C(u16 index);            // character-data lookup
extern "C" u32 func_800A32BC();                       // character-data category
extern "C" u32 func_8003B1EC(void* fp);               // BDAT row count
extern "C" u32 func_8009CF8C(u32 resourceId);         // message-count lookup
extern "C" u32 func_80174C98(void* actor, u32* outVal, u32 flags);
extern "C" u32 func_8013606C(const void*, const void*, u32);   // msg-id lookup
// func_80136254 / func_8013639C return wider-than-u16 values in retail; the
// call sites truncate with an explicit (u16) cast (see func_8027EA6C).
extern "C" u16 func_80136254(const void*, const void*, int);
// func_8013639C's third arg is the u16 row key; declared int so call sites
// control the clrlwi with an explicit cast.
extern "C" char* func_8013639C(const void*, const void*, int); // row name lookup
extern "C" char* func_80136190(char*, char*, u32);    // format two strings
extern "C" void func_80136B4C(nw4r::lyt::Layout*, const char*, const char*, u32);
extern "C" void func_80137E7C(nw4r::lyt::Layout*, const char*, void* res);
extern "C" char* func_80138F78(u32);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void func_8013676C(nw4r::lyt::Pane* rootPane, u32 fontHandle);
extern "C" void func_8003AA78__5CBdatFUlPv(u32, void*);
extern "C" void* func_8003AA34();
extern "C" void* getFP__FPCc(const char*);
extern "C" void* __ct__CTagProcessor(void* self);
extern "C" u32 getAllocHandle__10CLibLayoutFv();
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(
    u32 arg, nw4r::lyt::Layout* layout);
extern "C" void func_80136E84__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
    nw4r::lyt::Layout** ppLayout, nw4r::lyt::ArcResourceAccessor* accessor,
    const char* name);
extern "C" void func_80136F08__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
    nw4r::lyt::Layout* layout, nw4r::lyt::AnimTransform** ppAnimTrans,
    nw4r::lyt::ArcResourceAccessor* accessor, char* name);

// Scenario-log string pool (split .rodata) and the u16->f32 conversion magic
// double (2^52 = 0x4330000000000000, CfTFile.cpp convention).
extern char lbl_eu_8050EE24[];
extern "C" double lbl_eu_80668AD8;

// CSysWinScenarioLog::Move helpers.
extern "C" int func_8013BE50();
extern "C" s32  func_8029A658();
extern "C" u32  func_80137444__FPQ34nw4r3lyt13AnimTransformf(nw4r::lyt::AnimTransform*, float);
extern "C" u32  func_80137510(nw4r::lyt::AnimTransform*, float);
extern "C" void func_80138078__FUl(u32);
extern "C" int  func_80086F9C__Q22cf13CfGameManagerFv(int arg);
extern "C" u32  lbl_eu_80663E28;
extern "C" f32  lbl_eu_80668AD0;
extern "C" void func_8027EA6C(CSysWinScenarioLog* self);

// Bottom-of-file helper definitions (see CSysWinScenarioLog.cpp): declared
// here so callers below emit a direct `bl` to the retail symbol instead of an
// inline body at each call site (retail calls them out-of-line).
extern "C" u32 __declspec(noinline) func_8027EE88(u32 self, u32 arg);
extern "C" void __declspec(noinline) func_8027EEF4(u32 self);

// cbRenderBefore imports (retail emits direct bl to these symbols).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(u8* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);
void func_80137250(nw4r::lyt::DrawInfo* drawInfo);
void func_80137038(nw4r::lyt::Layout* layout, nw4r::lyt::DrawInfo* drawInfo,
                   int arg2, int arg3);
extern "C" u32 func_800A9D90();
