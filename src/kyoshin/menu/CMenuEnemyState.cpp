#include "kyoshin/menu/CMenuEnemyState.hpp"

#include "monolib/scn.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/util/reslist.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include "decomp.h"
#include <nw4r/math.h>
#include <revolution/GX.h>

// ---------------------------------------------------------------------------
// Local overlay structs for external objects accessed via pointer arithmetic.
// Each matches the retail layout of the accessed fields.
// ---------------------------------------------------------------------------

// Pane-size view for the enemy-indicator panes found by func_80111B08.
// Mirrors nw4r::lyt::Pane's mSize region (+0x4C), which is protected.
struct PaneSizeView {
    u8 gap00[0x4C];
    f32 width;    // +0x4C
    f32 height;   // +0x50
};

// Scale helper used by func_80111B08: keep the pane's current height, set the
// width. Written as a helper so the inlined Size temp shares one home pair
// across the three call sites (retail frame holds a single 0x8/0xc pair).
static void MenuEnemySetPaneWidth(nw4r::lyt::Pane* pane, f32 width) {
    pane->SetSize(nw4r::lyt::Size(width, pane->GetSize().height));
}

// Many rendering / actor objects have a flag byte at +0xBB.
struct ObjBBFlag {
    u8 gap00[0xBB];
    u8 flagBB;           // +0xBB, bit 0 = visible / highlight
};

// Object with flag word at +0x64 and anim-state byte at +0x91.
// (CfObjectMove / CfObjectPc base region.)
struct Obj64_91 {
    u8 gap00[0x64];
    u32 word64;           // +0x64  bitfield flags
    u8 gap68[0x74 - 0x68];
    u32 word74;           // +0x74  sub-object / target id
    u8 gap78[0x8C - 0x78];
    u16 id8C;             // +0x8C  actor id (checked against 0x967/0x96b/0x9c9)
    u8 gap8E[0x91 - 0x8E];
    u8 byte91;            // +0x91  animation state id
};

// func_800FE68C() result: last-selected actor id at +0x90E4.
struct Fe68CView {
    u8 gap00[0x90E4];
    u32 lastId90E4;       // +0x90E4
};

// func_800EA444() result: current battle-target id at +0x04.
struct BattleTargetView {
    u8 gap00[4];
    void* field04;        // +0x04
};

// func_800AD860(obj) result: u16 id at +0x3F28 (func_801361E8 key).
struct AD860Result {
    u8 gap00[0x3F28];
    u16 id3F28;           // +0x3F28
};

// func_801984F0 list entry: actor id at +0x3F10.
struct EnemySlotListEntry {
    u8 gap00[0x3F10];
    u32 id3F10;           // +0x3F10
};

// actor2 vt[0x298]() result: sub-object pointer at +0x50.
struct Obj298View {
    u8 gap00[0x50];
    void* field50;        // +0x50
};

// Object at Obj298View::field50: flag word at +0x78 (bit 30) + float at +0x7C.
struct Obj50View {
    u8 gap00[0x78];
    u32 word78;           // +0x78
    f32 f7C;              // +0x7C
};

// The object that actor2->sub3F34 points to.
struct Sub3F34Layout {
    u8 gap00[0x7A4];
    u32 bits7A4;          // +0x7A4  bitfield (bit 26 checked)
};

// Actor2 from func_8016FE34 - has a sub-object pointer at +0x04 and
// a larger sub-object pointer at +0x3F34. +0x08 holds a sub-object with
// its own vtable (panel-highlight slot dispatch in func_801132A8).
struct Actor2Sub8 {
    void* vtable;   // +0x00 (at actor2+0x8)
};

struct Actor2Layout {
    u8 gap00[0x04];
    void* subObj4;        // +0x04
    Actor2Sub8 sub8;      // +0x08
    u8 gap0C[0x1530 - 0x0C];
    u32 field1530;        // +0x1530
    u8 gap1534[0x3F34 - 0x1534];
    Sub3F34Layout* sub3F34;  // +0x3F34
};

// CfObjectPc has an embedded sub-object at +0x3E9C with its own vtable.
struct PcEmbedLayout {
    void* vtable;         // +0x00 (at pc+0x3E9C)
    u8 gap04[0x74 - 0x04];
    u32 field74;          // +0x74  current battle-target id
};

// Pose data returned by func_80496264(unk60, -1).
// Matrix at +0x9C (VEC3TransformNormal), two Vec3f clusters at +0x10C and
// +0x138, and a scale vec at +0x118.
struct PoseLayout {
    u8 gap00[0x9C];
    nw4r::math::MTX34 mtx9C;   // +0x9C (3x4 matrix)
    u8 gapCC[0x10C - 0xCC];
    f32 vec10c_x;              // +0x10C
    f32 vec10c_y;
    f32 vec10c_z;
    f32 vec118_x;              // +0x118 (marker scale vec)
    f32 vec118_y;
    f32 vec118_z;
    u8 gap124[0x138 - 0x124];
    f32 vec138_x;              // +0x138
    f32 vec138_y;
    f32 vec138_z;
};

// Object returned by vslot GetVecFn(handle, 0x12C).
// Three f32 components at 16-byte stride (0x0C / 0x1C / 0x2C).
struct RLayout {
    u8 gap00[0x0C];
    f32 val0C;
    u8 gap10[0x1C - 0x10];
    f32 val1C;
    u8 gap20[0x2C - 0x20];
    f32 val2C;
};

// Minimal overlay for nw4r::lyt::AnimTransform field at +0x10.
struct AnimTransformOverlay {
    u8 gap00[0x10];
    f32 field10;
};



// ---------------------------------------------------------------------------
// End of overlay structs
// ---------------------------------------------------------------------------

// Batch 2026-07-14f: menu-enemy-cbrender owns cbRenderBefore exclusively.
// Batch 2026-07-14h: menu-enemy-move owns Move exclusively; do not touch
// cbRenderBefore above.
// Batch 2026-07-14k: menu-enemy-ctor owns __ct__CMenuEnemyState exclusively.

struct CMenuEnemyCtorProcess {
    u8 unk00[0x10];
    void* vtable; // 0x10
    u8 unk14[0x28];
    u32 callbacks[6]; // 0x3C / 0x48 PTMF blocks
};

extern "C" {
char lbl_eu_8052BF70[];
char lbl_eu_8052C438[];
extern u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void __ct__17UnkClass_8045F564Fv(void*);
void __dt__17UnkClass_8045F564Fv(void*, s16);

extern const f32 lbl_eu_80666FEC;
extern const f32 lbl_eu_80667004;
}

CMenuEnemyState::CMenuEnemyState(void* scn) {
    // NV decl order targets retail homes: r31=this, r30=zero, r29=scn.
    u32 zero;
    CMenuEnemyCtorProcess* process;
    u8* ptmfBase;
    char* vtFinal;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;
    MenuEnemyPanel* panel;
    f32 panelMarker;
    u8 one;
    u8 tmp[0x48];
    u32 copy;

    process = reinterpret_cast<CMenuEnemyCtorProcess*>(this);
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
    ptmfBase = reinterpret_cast<u8*>(__ptmf_null);
    process->vtable = lbl_eu_8052BF70;
    ptmfWord1 = *reinterpret_cast<u32*>(ptmfBase + 4);
    vtFinal = lbl_eu_8052C438;
    ptmfWord0 = *reinterpret_cast<u32*>(ptmfBase + 0);
    process->callbacks[1] = ptmfWord1;
    process->callbacks[0] = ptmfWord0;
    ptmfWord2 = *reinterpret_cast<u32*>(ptmfBase + 8);
    process->callbacks[2] = ptmfWord2;
    ptmfWord0 = *reinterpret_cast<u32*>(ptmfBase + 0);
    ptmfWord1 = *reinterpret_cast<u32*>(ptmfBase + 4);
    process->callbacks[4] = ptmfWord1;
    process->callbacks[3] = ptmfWord0;
    ptmfWord2 = *reinterpret_cast<u32*>(ptmfBase + 8);
    process->callbacks[5] = ptmfWord2;
    zero = 0;
    unk54 = 0;
    unk55 = 0;
    process->vtable = vtFinal;
    vtPtr1 = vtFinal + 0x24;
    vtPtr2 = vtFinal + 0xac;
    unk60 = scn;

    __ct__17UnkClass_8045F564Fv(unk64);

    unk74 = NULL;
    // Retail hoists the loop bound (&panels[24]) into a temp and keeps the
    // unk29 constant in a register for the whole loop (r3); defining `one`
    // before the panel pointer pins that color order.
    one = 1;
    panel = panels;
    panelMarker = lbl_eu_80666FEC;
    unk78 = NULL;
    field7C = zero;
    field80 = zero;
    field84 = zero;
    field88 = zero;
    field8C = zero;
    field90 = zero;
    field94 = zero;
    field98 = zero;
    field9C = zero;
    fieldA0 = zero;

    // do-while + live panelEnd matches retail fall-into-body cmplw/blt shape.
    // panelEnd/one r0/r3 Chaitin soft-cap - keep iterating in high-level C.
    do {
        panel->actorId = zero;
        panel->layout1 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panel->unk08 = zero;
        panel->layout2 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panel->unk10 = zero;
        panel->drawLayout0Flag = 0;
        panel->visible = 0;
        panel->animMarker = panelMarker;
        panel->unk1C = 0;
        panel->unk1D = 0;
        panel->unk1E = 0;
        panel->unk1F = 0;
        panel->unk20 = 0;
        panel->unk21 = 0;
        panel->unk22 = 0;
        panel->unk24 = zero;
        panel->panelType = 0;
        panel->unk29 = one;
        panel->obj1 = reinterpret_cast<void*>(zero);
        panel->obj2 = reinterpret_cast<void*>(zero);
        panel->obj3 = reinterpret_cast<void*>(zero);
        panel->unk38 = zero;
        panel->unk3C = zero;
        panel->unk40 = zero;
        panel->unk44 = zero;
        panel->unk48 = zero;
        panel++;
    } while (panel < &panels[24]);

    zero = 0;
    field7C4 = 0;
    field7C8 = panelMarker;
    field7CC = panelMarker;
    field7D0 = panelMarker;
    field7D4 = zero;
    field7D8 = panelMarker;
    field7DC = zero;
    field7E0 = zero;

    __ct__CPcSelectCursor(&selectCursor);

    field82C = zero;
    unk830 = zero;
    unk834 = 0;
    unk838 = lbl_eu_80667004;

    __ct__CPcSelectCursor(tmp);
    {
        CPcSelectCursorLayout* tmpSel = reinterpret_cast<CPcSelectCursorLayout*>(tmp);
        copy = tmpSel->field04;
        selectCursor.field04 = copy;
        copy = tmpSel->field08;
        selectCursor.field08 = copy;
        copy = tmpSel->field0C;
        selectCursor.field0C = copy;
        copy = tmpSel->field10;
        selectCursor.field10 = copy;
        copy = tmpSel->field14;
        selectCursor.field14 = copy;
        copy = tmpSel->field18;
        selectCursor.field18 = copy;
        copy = reinterpret_cast<u32>(tmpSel->layout1C);
        selectCursor.layout1C = reinterpret_cast<nw4r::lyt::Layout*>(copy);
        copy = reinterpret_cast<u32>(tmpSel->anim20);
        selectCursor.anim20 = reinterpret_cast<nw4r::lyt::AnimTransform*>(copy);
        copy = reinterpret_cast<u32>(tmpSel->anim24);
        selectCursor.anim24 = reinterpret_cast<nw4r::lyt::AnimTransform*>(copy);
        copy = tmpSel->field28;
        selectCursor.field28 = copy;
        copy = tmpSel->field2C;
        selectCursor.field2C = copy;
        copy = tmpSel->field30;
        selectCursor.field30 = copy;
        copy = tmpSel->field34;
        selectCursor.field34 = copy;
        copy = tmpSel->field38;
        selectCursor.field38 = copy;
        copy = tmpSel->field3C;
        selectCursor.field3C = copy;
        selectCursor.byte40 = tmpSel->byte40;
        selectCursor.byte41 = tmpSel->byte41;
        copy = tmpSel->field44;
        selectCursor.field44 = copy;
    }
    __dt__17UnkClass_8045F564Fv(tmp + 0x08, -1);

    return;
}

// ---------------------------------------------------------------------------
// CMenuEnemyState dtor (us-8010fe9c): retail D0/D1/D2-merged dtor. The
// embedded UnkClass_8045F564 regions (+0x7EC = selectCursor.mem08, +0x64) and
// the CProcess base are destroyed explicitly because this TU models them as a
// flat overlay; MWCC inlines the tiny ~CPcSelectCursor (its own null-guard
// becomes the addic./beq on &selectCursor) and emits the delete-on-flag for
// the virtual dtor automatically.
// ---------------------------------------------------------------------------
CMenuEnemyState::~CMenuEnemyState() {
    reinterpret_cast<CPcSelectCursor*>(&selectCursor)->~CPcSelectCursor();
    reinterpret_cast<UnkClass_8045F564*>(unk64)->~UnkClass_8045F564();
    // Double null-guard reproduces the retail `cmpwi r30,0; beq; beq` before
    // the base-dtor call (MWCC emits both guards against the one test); the
    // explicit call passes flags 0 (base subobjects are never deleted).
    if (this != 0) {
        if (this != 0) {
            __dt__8CProcessFv(this, 0);
        }
    }
}

// ---------------------------------------------------------------------------
// CPcSelectCursor ctor (us-8010f540): retail stores the vtable address at
// +0x00 (a plain u32 field in this deliberately non-polymorphic model),
// zeroes field04, constructs the embedded UnkClass_8045F564 region, zeroes
// the rest, sets byte40 and returns this. The vtable value is a plain linker
// constant (lbl_eu_8052C534), so no __vt__ symbol / automatic vptr store is
// emitted anywhere.
// ---------------------------------------------------------------------------
CPcSelectCursor::CPcSelectCursor()
    : field00((u32)lbl_eu_8052C534), field04(0), mem08() {
    field18 = 0;
    layout1C = NULL;
    anim20 = NULL;
    anim24 = NULL;
    field28 = 0;
    field2C = 0;
    field30 = 0;
    field34 = 0;
    field38 = 0;
    field3C = 0;
    byte40 = 1;
    field44 = 0;
}

// ---------------------------------------------------------------------------
// CPcSelectCursor dtor (us-8010f5c4): the embedded UnkClass_8045F564 member
// at +0x8 is destroyed (MWCC passes -1), then the object is deleted when the
// incoming delete flag is > 0. Non-polymorphic: no vtable store in retail.
// ---------------------------------------------------------------------------
CPcSelectCursor::~CPcSelectCursor() {
    // member mem08 destruction + delete-on-flag are generated by MWCC
}

extern "C" {
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
}

// ---------------------------------------------------------------------------
// Minimal local views replacing kyoshin/cf/CBattleManager.hpp + CfObjectPc.hpp.
// Those headers include CfObjectActor.hpp, which currently cannot be parsed:
// its extern "C" func_800BE12C 5-arg form (line 39) conflicts with the 4-arg
// form in CfObjectMove.hpp (line 192) - same C symbol, MWCC 10197. Only the
// members/symbols used below are declared; layouts match the retail offsets.
// ---------------------------------------------------------------------------
namespace cf {
class CfObjectActor;   // forward (reslist element type only)
class CfObjectPc;      // forward (func_800BFC68 return type, used via cast)
class CfObjectMove;    // forward (func_800BFC68 param type, getPlayer return)

// Partial view of cf::CBattleManager (full header kyoshin/cf/CBattleManager.hpp).
class CBattleManager {
public:
    static CBattleManager* getInstance();
    u8 pad00[4];                         // vtable / IFactoryEvent subobject
    reslist<CfObjectActor*> mActorList1;  // +0x04 (retail offset)
};

// Minimal cf::CfGameManager (full header kyoshin/cf/CfGameManager.hpp, which
// is not includable here: its extern "C" bool func_804960A8 clashes with the
// int form in CTaskGame.hpp under MWCC 10505). Only the statics this TU uses
// are declared.
class CfGameManager {
public:
    static CfGameManager* getInstance();
    static CfObjectMove* getPlayer(int playerIndex);
};
}

// Minimal CTaskGame (full header kyoshin/CTaskGame.hpp is not includable here
// while its extern "C" func_8049603C/func_804960A8 caller-shape imports clash
// with code_80135FDC.hpp / CfGameManager.hpp under MWCC 10197/10505). Only
// the statics this TU uses are declared.
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// func_800B708C(BOOL)/func_800BFC68(CfObjectMove*) declared (not extern "C")
// - MWCC's own C++ mangling matches the retail linker names
// (func_800B708C__Fi / func_800BFC68__FPQ22cf12CfObjectMove).
extern void* func_800B708C(int);
extern cf::CfObjectPc* func_800BFC68(cf::CfObjectMove* objMove);
// Last-selected actor id source (was declared in kyoshin/CTaskGame.hpp, which
// is not includable here due to the concurrent func_8049603C/A8 conflict).
extern "C" void* func_800FE68C();

// Unit functions whose retail linker symbols are UNMANGLED: declare them
// extern "C" so call sites emit the unmangled reloc names (retail uses C
// linkage for these menu helpers).
extern "C" void func_8010EE40(CPcSelectCursorLayout* self);
extern "C" void func_8010EB44(CPcSelectCursorLayout* self);
extern "C" void func_80111080(CMenuEnemyState* self, u8* panelData, void* posA, void* posB);
extern "C" void func_801115E8(CMenuEnemyState* self, u8* panelData);
void func_80111B08(CMenuEnemyState* self, u8* panelData, f32 v128, f32 v12c);
extern "C" void func_80111C50(CMenuEnemyState* self, u8* panelData, int which);
void func_80111E70(CMenuEnemyState* self, u8* panelData, f32 v128, f32 v12c);
extern "C" void func_80112170(CMenuEnemyState* self, u8* panelData);
extern "C" void func_801127B0(CMenuEnemyState* self);
// Menu sound effect for the enemy-menu (plain C++ so MWCC mangles the retail
// name func_80138078__FUl).
extern void func_80138078(u32);
void func_801132A8(CMenuEnemyState* self, u8* panelData, void* actor);
extern "C" void func_801124C8(CMenuEnemyState* self, Actor2Layout* actor2);
extern "C" {
// CfGameManager::func_8008585C is not declared in CfGameManager.hpp (owned
// by other batch targets); call the mangled linker name directly instead of
// touching that shared header.
int func_8008585C__Q22cf13CfGameManagerFv();

extern u8 lbl_eu_80663F98;        // once-guard byte
extern f32 lbl_eu_80573A60[3];    // mutable pulse vector (shake/scale-like)
extern const f32 lbl_eu_80666FE8; // 1.0f
extern const f32 lbl_eu_80667004; // 0.0f
extern const f32 lbl_eu_8066700C; // pulse step
extern const f32 lbl_eu_80667010; // pulse frequency
extern const f32 lbl_eu_80667014; // distance^2 threshold
extern const f32 lbl_eu_80666FEC; // anim-state marker value
extern const f32 lbl_eu_8066A1F8; // pulse amplitude

extern "C" void* func_8016FE34(void* r3);
int func_8013BF48();
void func_800BBA08(void* r3);
void func_800BBA7C(void* r3);
int func_8013A4B4(void* a, void* b, void* c);


// Retail leaves f1=vt+0x128 and f2=vt+0x12C live into these calls.


int func_80174C98(void* actor, int* outVal, int flags);
void* func_80496264(void* obj, int index);

// Retail leaves func_80137510 unmangled (declared extern "C" in
// code_80135FDC.hpp); calls pass the AnimTransform* directly.
}

struct Vec3f {
    f32 x, y, z;
};

// Pane-like object pointed to by MenuEnemyPanel::unk44: flag byte at +0xBB
// and a scale vec3 at +0x2C.
struct PaneScaleVec3 {
    u8 gap00[0x2C];
    Vec3f scale;   // +0x2C
};

// Pane-size view of a nw4r::lyt::Pane used by func_80111080: position vec at
// +0x2C, marker-scale vec at +0x38, two marker factors at +0x44/+0x48, and
// the visible-flag byte at +0xBB.
struct PanePosLayout {
    u8 gap00[0x2C];
    f32 v2C;                       // +0x2C
    f32 v30;                       // +0x30
    f32 v34;                       // +0x34
    f32 v38;                       // +0x38
    f32 v3C;                       // +0x3C
    f32 v40;                       // +0x40
    f32 v44;                       // +0x44
    f32 v48;                       // +0x48
    u8 gap4C[0xBB - 0x4C];
    u8 flagBB;                     // +0xBB, bit 0 = visible / highlight
};

struct Vec2f {
    f32 x, y;
};

struct U32x4 {
    u32 a, b, c, d;
};

// Object pointed to by MenuEnemyPanel::field48 (panel-highlight target):
// a 5-float scale/colour vector at +0x2C/+0x30/+0x34 and +0x44/+0x48, and
// the visible flag byte at +0xBB.
struct PanelObj48 {
    u8 gap00[0x2C];
    f32 v2C;                       // +0x2C
    f32 v30;                       // +0x30
    f32 v34;                       // +0x34
    u8 gap38[0x44 - 0x38];
    f32 v44;                       // +0x44
    f32 v48;                       // +0x48
    u8 gap4C[0xBB - 0x4C];
    u8 flagBB;                     // +0xBB, bit 0 = visible / highlight
};

// Result of the actor2->sub8 vtable slot calls (0x58/0x5C): an object with a
// u16 resource id at +0x0C and a type word at +0x30.
struct SubSlot5CResult {
    u8 gap00[0x0C];
    u16 id0C;    // +0x0C
    u8 gap0E[0x30 - 0x0E];
    u32 word30;  // +0x30
};

// Polymorphic view of the actor2->sub8 sub-object used for the slot-0x5C
// probe. The dtor + dummy virtuals occupy vtable 0x08..0x54 so that the
// probes land at vtable +0x58 / +0x5C and MWCC emits the retail r12 dispatch
// chain for genuine virtual calls. Never constructed - no vtable is emitted.
class Sub8Probe {
public:
    virtual ~Sub8Probe();              // 0x08
    virtual void m0C(); virtual void m10(); virtual void m14(); virtual void m18();
    virtual void m1C(); virtual void m20(); virtual void m24(); virtual void m28();
    virtual void m2C(); virtual void m30(); virtual void m34(); virtual void m38();
    virtual void m3C(); virtual void m40(); virtual void m44(); virtual void m48();
    virtual void m4C(); virtual void m50(); virtual void m54();
    virtual SubSlot5CResult* probe2(u8 idx);  // 0x58
    virtual SubSlot5CResult* probe(u8 idx);   // 0x5C
};

// Vtable slot dispatch helper (same style as cf::CBattleState_UnkVirtualFuncN
// ? call through the vtable without declaring the callee's own methods).
template <typename Fn>
static inline Fn vslot(void* obj, u32 offset) {
    return reinterpret_cast<Fn>((*reinterpret_cast<void***>(obj))[offset / 4]);
}

void CMenuEnemyState::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_CASES 8c9.
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);

        u32 indices[0x18];
        u8 fill;
        for (fill = 0; fill < 0x18; fill++) {
            indices[fill] = fill;
        }

        // Outer for(left!=0) -> mtctr/bdnz. Inner bottom-tested goto keeps CTR outer.
        // Rematerialize &indices[j] each step (retail addi r1,0x8). Load depthB before
        // depthA so fcmpo uses f1/f0. XOR via store-reload of order[j]/pair[1].
        {
            // pass-before-order + block-scoped limit → order=r9, j=r12, swapped=r11
            // (retail). Peak 99.172%: pass/limit/pair still color r8/r10/r30 vs
            // retail r10/r5/r8; XOR operand/dest Chaitin follows from that.
            // Decl order pins colors: order=r9, pass=r10, limit=r5, left=CTR;
            // pair rematerializes into the freed r8 each step.
            u32* order = indices;
            u8 pass = 0;
            u32 left;
            for (left = 0x17; left != 0; left--) {
                u8 swapped = 0;
                u8 j = 0;
                goto sort_test;
            sort_body: {
                    u32 idxA = order[j];
                    u32* pair = &indices[j];
                    u32 idxB = pair[1];
                    f32 depthB = panels[idxB].animMarker;
                    f32 depthA = panels[idxA].animMarker;
                    if (depthA > depthB) {
                        u32 tmp = order[j] ^ pair[1];
                        order[j] = tmp;
                        swapped = 1; // retail sets this before finishing the XOR swap
                        tmp = pair[1] ^ tmp;
                        pair[1] = tmp;
                        tmp = order[j] ^ tmp;
                        order[j] = tmp;
                    }
                    j++;
                }
            sort_test:
                if ((s32)j < 0x17 - (s32)pass) {
                    goto sort_body;
                }
                if (swapped == 0) {
                    break;
                }
                pass++;
            }
        }

        // NV decl order entry, order, i -> r30/r29/r28 (MWCC_CASES 8c6).
        // u32 counter; cast to u8 only when indexing. Compare stays cmpli (no
        // terminal clrlwi) so .text is retail 0x274.
        {
            u32 i;
            u32* order;
            order = indices;
            i = 0;
            do {
                MenuEnemyPanel& p = panels[order[static_cast<u8>(i)]];
                if (p.visible == 0) {
                    goto draw_next;
                }
                if (p.unk29 == 0) {
                    goto draw_next;
                }
                if (p.panelType != 0) {
                    func_80137038(unk74, &drawInfo, 0, 1);
                }
                {
                    int drawFlag = (p.panelType == 0) ? 0 : 1;
                    drawFlag = (drawFlag == 0) ? 1 : 0;
                    func_80137038(p.layout2, &drawInfo, drawFlag, 1);
                }
                if (p.drawLayout0Flag == 0) {
                    func_80137038(p.layout1, &drawInfo, 0, 1);
                }
            draw_next:
                i++;
            } while (i < 0x18);
        }

        if (selectCursor.field44 != 0) {
            func_80137038(selectCursor.layout1C, &drawInfo, 0, 1);
        }
    }
done:
    ;
}

void CMenuEnemyState::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        goto done;
    }
    // Retail: rlwinm.; beq +8; b done. MWCC collapses if->goto to bne; keep beq
    // via fallthrough asm b (PLAN.md section 17.6). See MWCC_CASES 8c9.
    if ((lbl_eu_80663E28 & (1u << 21)) == 0) {
        goto after_bit21;
    }
    DECOMP_ASM_INSN_BEGIN
    asm {
        b done
    }
    DECOMP_ASM_INSN_END
after_bit21:
    if (!func_8013BE50()) {
        goto done;
    }
    if (lbl_eu_80663E24 & 0xAFA40000u) {
        goto done;
    }
    if (func_8008585C__Q22cf13CfGameManagerFv()) {
        goto done;
    }

    {
    // Pulse / target-highlight setup. Scope ends before the panel loop so
    // noTarget does not inflate callee-saved pressure past retail _savegpr_22.
    {
        int noTarget = 1;

        if (cf::CfGameManager::getPlayer(0) != NULL) {
            // (Not in the 47-reinterpret_arith findings - reading offset
            //  0x90E4 from func_800FE68C().  Overlay struct with a u32 at
            //  that offset would be unwieldy; keep the original expression.)
            u32 lastId = *reinterpret_cast<u32*>(
                reinterpret_cast<u8*>(func_800FE68C()) + 0x90E4);

            if (lastId != 0) {
                Obj64_91* obj = reinterpret_cast<Obj64_91*>(func_800B708C(static_cast<int>(lastId)));

                if (obj != NULL) {
                    bool active = (obj->word64 & 4) != 0;

                    if (active) {
                        if (unk834 != 0) {
                            f32 v = unk838 - lbl_eu_8066700C;
                            unk838 = v;
                            if (v <= lbl_eu_80667004) {
                                unk838 = lbl_eu_80667004;
                            }
                        } else {
                            f32 v = unk838 + lbl_eu_8066700C;
                            unk838 = v;
                            if (v >= lbl_eu_80666FE8) {
                                unk838 = lbl_eu_80666FE8;
                                unk834 = 1;
                            }
                        }

                        if (lastId != unk830) {
                            unk834 = 0;
                            unk838 = lbl_eu_80667004;

                            void* oldObj = func_800B708C(static_cast<int>(unk830));
                            if (oldObj != NULL) {
                                func_800BBA08(oldObj);
                            }
                            unk830 = lastId;
                        }

                        if (!lbl_eu_80663F98) {
                            lbl_eu_80663F98 = 1;
                        }

                        {
                            f32 t = lbl_eu_8066A1F8 * unk838;
                            t = lbl_eu_80667010 * t;
                            f32 s = nw4r::math::SinFIdx(t);
                            lbl_eu_80573A60[0] = s;
                            lbl_eu_80573A60[1] = s;
                            lbl_eu_80573A60[2] = lbl_eu_80666FE8;
                            func_800BBA7C(obj);
                        }

                        noTarget = 0;
                    }
                }
            }
        }

        if (noTarget) {
            if (unk830 != 0) {
                void* h = func_800B708C(static_cast<int>(unk830));
                if (h != NULL) {
                    func_800BBA08(h);
                }
                unk830 = 0;
            }
        }
    }

    func_801127B0(this);

    // Retail pre-loop materialization (80110A88..AA4): pc call, then
    // f30/r30/f31/r29/f28/r28/r27/r31 - declare in that dependence order.
    cf::CfObjectPc* pc =
        func_800BFC68(cf::CfGameManager::getPlayer(0));
    f32 animMarker = lbl_eu_80666FEC;
    f32 distThresh = lbl_eu_80667014;
    nw4r::math::VEC3 scratch;
    nw4r::math::VEC3* pScratch = &scratch;
    f32 one = lbl_eu_80666FE8;
    nw4r::math::VEC3 delta;
    nw4r::math::VEC3* pDelta = &delta;
    u8 i = 0;
    u8 z = 0;

    for (; i < 0x18; i++) {
        MenuEnemyPanel& panel = panels[i];

        if (panel.visible == 0) {
            continue;
        }

        if (panel.panelType == 0) {
            if (func_8013BF48()) {
                u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
                u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
                u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
                b1 = (b1 & 0xFE) | 1;
                b2 = (b2 & 0xFE) | 1;
                b3 = (b3 & 0xFE) | 1;
            }
        }

        u32 actorId = panel.actorId;
        Obj64_91* handle = reinterpret_cast<Obj64_91*>(func_800B708C(static_cast<int>(actorId)));
        if (handle == NULL) {
            panel.visible = z;
            continue;
        }

        // r24 in retail's loop = result of func_8016FE34 (not the early target).
        Actor2Layout* actor2 = reinterpret_cast<Actor2Layout*>(func_8016FE34(NULL));
        int skipDist = 0;
        int hasSub = 0;
        if (actor2 != NULL) {
            Sub3F34Layout* sub3f34 = actor2->sub3F34;
            if (sub3f34 != NULL) {
                hasSub = 1;
            }
        }
        if (hasSub) {
            Sub3F34Layout* sub3f34 = actor2->sub3F34;
            u32 bits = sub3f34->bits7A4;
            if ((bits >> 26) & 1) {
                skipDist = 1;
            }
        }

        if (actor2 != NULL) {
            typedef f32 (*GetFloatFn)(void*);
            f32 stateVal = vslot<GetFloatFn>(actor2, 0x128)(actor2);
            // Retail: state==FEC && panelData[0x1c]==0 → always cull.
            if (animMarker == stateVal) {
                if (panel.unk1C == 0) {
                    panel.visible = z;
                    continue;
                }
            }
        }
        if (skipDist) {
            panel.visible = z;
            continue;
        }

        // Distance cull + frustum test only when panelType==0 and pc!=NULL.
        // Retail skips both when either gate fails (falls through to flag work).
        if (panel.panelType == 0 && pc != NULL) {
            typedef void* (*GetPosFn)(void*);
            void* handlePos = vslot<GetPosFn>(handle, 0xAC)(handle);
            PcEmbedLayout* pcEmbed = reinterpret_cast<PcEmbedLayout*>(pc);
            void* pcPos = vslot<GetPosFn>(&pcEmbed->vtable, 0xAC)(&pcEmbed->vtable);

            nw4r::math::VEC3Sub(
                pDelta,
                reinterpret_cast<const nw4r::math::VEC3*>(pcPos),
                reinterpret_cast<const nw4r::math::VEC3*>(handlePos));
            scratch = delta;
            // Retail: interleaved assign + VEC3LenSq (ps_mul/ps_madd/ps_sum0).
            f32 distSq = nw4r::math::VEC3LenSq(pScratch);
            if (distSq > distThresh) {
                panel.visible = z;
                continue;
            }

            {
                PoseLayout* pose = static_cast<PoseLayout*>(func_80496264(unk60, -1));
                Vec3f a;
                a.x = pose->vec10c_x;
                a.y = pose->vec10c_y;
                a.z = pose->vec10c_z;
                Vec3f b;
                b.x = pose->vec138_x;
                b.y = pose->vec138_y;
                b.z = pose->vec138_z;

                void* qpos = vslot<GetPosFn>(handle, 0xAC)(handle);
                Vec3f c = *static_cast<const Vec3f*>(qpos);

                if (!func_8013A4B4(&a, &b, &c)) {
                    panel.visible = z;
                    continue;
                }
            }
        }

        if (panel.panelType == 0) {
            if (panel.unk1D != 0) {
                if (actor2 != NULL) {
                    u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
                    u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
                    u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;

                    b1 = (b1 & 0xFE) | 1;
                    b2 = b2 & 0xFE;

                    typedef u32* (*GetPtrFn)(void*);
                    u32* r = vslot<GetPtrFn>(actor2, 0x258)(actor2);
                    u32 v = *r;
                    if (v - 1 <= 3) {
                        b2 = (b2 & 0xFE) | 1;
                    }

                    typedef u32 (*GetU8Fn)(void*);
                    u32 byteVal = vslot<GetU8Fn>(actor2, 0x260)(actor2) & 0xFF;
                    b3 = (b3 & 0xFE) | static_cast<u8>(byteVal);
                }
            } else {
                u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
                u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
                u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
                b1 = b1 & 0xFE;
                b2 = b2 & 0xFE;
                b3 = b3 & 0xFE;
            }
        }

        func_801115E8(this, reinterpret_cast<u8*>(&panel));

        {
            Vec3f posTmp;
            Vec3f posA;
            Vec3f posB;
            Vec3f* posTmpPtr = &posTmp;

            typedef RLayout* (*GetVecFn)(void*, int);
            RLayout* r = vslot<GetVecFn>(handle, 0x12C)(handle, 0x64);
            if (r != NULL) {
                posTmpPtr->x = r->val0C;
                posTmpPtr->y = r->val1C;
                posTmpPtr->z = r->val2C;
                posA = *posTmpPtr;
            } else {
                typedef void* (*GetPosFn)(void*);
                void* p = vslot<GetPosFn>(handle, 0xAC)(handle);
                posA = *static_cast<const Vec3f*>(p);
            }

            typedef void* (*GetPosFn)(void*);
            void* p2 = vslot<GetPosFn>(handle, 0xAC)(handle);
            posB = *static_cast<const Vec3f*>(p2);

            Obj64_91* hf = handle;
            u32 flagWord = hf->word64;
            if ((flagWord & 0x4000) != 0 || (flagWord & 0x8000) != 0) {
                if (handle != NULL) {
                    u8 animState = hf->byte91;
                    if (animState == 6) {
                        posA.y += one;
                    }
                }
            }

            func_80111080(this, reinterpret_cast<u8*>(&panel), &posA, &posB);
        }

        if (panel.unk1F == 0 && actor2 != NULL) {
            func_80112170(this, reinterpret_cast<u8*>(&panel));

            if (panel.unk1C != 0) {
                u32 count = cf::CBattleManager::getInstance()->mActorList1.size();
                panel.drawLayout0Flag = (count == 0) ? 1 : 0;
            } else {
                void* subObj = actor2->subObj4;
                typedef int* (*SubGetFn)(void*);
                int* result = vslot<SubGetFn>(subObj, 0x30)(subObj);
                int localVal = *result;
                u32 ret = func_80174C98(actor2, &localVal, 0x803);
                panel.drawLayout0Flag = (ret == 0) ? 1 : 0;
            }

            if (panel.drawLayout0Flag == 0) {
                typedef f32 (*GetFloatFn)(void*);
                f32 v12c = vslot<GetFloatFn>(actor2, 0x12C)(actor2);
                f32 v128 = vslot<GetFloatFn>(actor2, 0x128)(actor2);
                func_80111B08(this, reinterpret_cast<u8*>(&panel), v128, v12c);

                v12c = vslot<GetFloatFn>(actor2, 0x12C)(actor2);
                v128 = vslot<GetFloatFn>(actor2, 0x128)(actor2);
                func_80111E70(this, reinterpret_cast<u8*>(&panel), v128, v12c);

                func_801132A8(this, reinterpret_cast<u8*>(&panel), actor2);
            }
        }

        if (panel.panelType != 0) {
            u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
            u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
            u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
            b1 = b1 & 0xFE;
            b2 = b2 & 0xFE;
            b3 = b3 & 0xFE;
        } else if (!func_8013BF48()) {
            u8& b1 = static_cast<ObjBBFlag*>(panel.obj1)->flagBB;
            u8& b2 = static_cast<ObjBBFlag*>(panel.obj2)->flagBB;
            u8& b3 = static_cast<ObjBBFlag*>(panel.obj3)->flagBB;
            b1 = b1 & 0xFE;
            b2 = b2 & 0xFE;
            b3 = b3 & 0xFE;
        }
    }

    if (panels[23].visible != 0) {
        u32 left = 0x17;
        u32 j = 0;
        do {
            MenuEnemyPanel& panel2 = panels[j];
            if (panel2.visible != 0) {
                if (panel2.actorId == panels[23].actorId) {
                    panel2.visible = z;
                    break;
                }
            }
            j++;
        } while (--left != 0);
    }

    func_80137444(unk78, lbl_eu_80666FE8);
    unk74->Animate(0);

    switch (selectCursor.field44) {
    case 1:
        if (func_80137444(selectCursor.anim20, lbl_eu_80666FE8) != 0) {
            selectCursor.byte40 = 1;
            selectCursor.field44 = 2;
        }
        break;
    case 3:
        if (func_80137510(selectCursor.anim20, lbl_eu_80666FE8) != 0) {
            selectCursor.byte40 = 1;
            selectCursor.field44 = 0;
        }
        break;
    case 4:
        if (func_80137444(selectCursor.anim24, lbl_eu_80666FE8) != 0) {
            reinterpret_cast<AnimTransformOverlay*>(selectCursor.anim24)->field10 = lbl_eu_80666FEC;
            selectCursor.layout1C->Animate(0);
            selectCursor.layout1C->SetAnimationEnable(selectCursor.anim24, false);
            selectCursor.layout1C->SetAnimationEnable(selectCursor.anim20, true);
            selectCursor.field44 = 3;
        }
        break;
    default:
        break;
    }

    if (selectCursor.field44 != 0) {
        func_8010EE40(&selectCursor);
    }

    selectCursor.layout1C->Animate(0);
    }
done:
    ;
}

// func_8010EB44 (us-8010f620): CPcSelectCursor layout/anim init. Creates a
// 0x2000 scratch region, builds the layout + two anims from the shared arc
// accessor (field18), grabs six named panes off the root pane, then enables
// anim20. Called after the ctor from the owning menu's file-event path.
void func_8010EB44(CPcSelectCursor* self) {
    self->mem08.createRegion(mtl::MemManager::getHandleMEM2(), 0x2000,
                             &lbl_eu_804FDBF8[0x00], 0);
    Class_8045F858 regionGuard(&self->mem08);
    mtl::MemManager::func_80434A4C(false);

    // Retail reloads field18 (the arc accessor) and layout1C per call instead
    // of caching them in locals, so each is re-loaded from the object.
    func_80136E84(&self->layout1C,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->field18),
                  &lbl_eu_804FDBF8[0x10]);
    func_80136F08(self->layout1C, &self->anim20,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->field18),
                  &lbl_eu_804FDBF8[0x2c]);
    func_80136F08(self->layout1C, &self->anim24,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(self->field18),
                  &lbl_eu_804FDBF8[0x4b]);

    self->field28 = (u32)self->layout1C->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0x6a], true);
    self->field34 = (u32)self->layout1C->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0x72], true);
    self->field2C = (u32)self->layout1C->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0x7c], true);
    self->field30 = (u32)self->layout1C->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0x86], true);
    self->field38 = (u32)self->layout1C->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0x90], true);
    self->field3C = (u32)self->layout1C->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0x9c], true);

    self->layout1C->SetAnimationEnable(self->anim24, false);
    self->layout1C->SetAnimationEnable(self->anim20, true);
    self->layout1C->Animate(0);
}

// func_8010ED58 (us-8010f834): cursor state 2 -> 4; swaps the active anim.
void func_8010ED58(CPcSelectCursor* self) {
    if ((s32)self->field44 == 2) {
        self->field44 = 4;
        self->byte40 = 0;
        self->layout1C->SetAnimationEnable(self->anim20, false);
        self->layout1C->SetAnimationEnable(self->anim24, true);
    }
}

// func_8010EDE4 (us-8010f8c0): reset the cursor (state 0, marker frame,
// anim advance) and mark it ready.
void func_8010EDE4(CPcSelectCursor* self) {
    self->field44 = 0;
    reinterpret_cast<AnimTransformOverlay*>(self->anim20)->field10 = lbl_eu_80666FEC;
    self->layout1C->Animate(0);
    self->byte40 = 1;
}

// func_80111B08 (us-801125e4): scale the widths of the three enemy-indicator
// panes by a clamped v128/v12c ratio, then animate the panel layout. t starts
// at the anim marker and is only replaced when both input floats exceed it.
void func_80111B08(CMenuEnemyState* self, u8* panelData, f32 v128, f32 v12c) {
    f32 t = lbl_eu_80666FEC;
    if (v12c > lbl_eu_80666FEC && v128 > lbl_eu_80666FEC) {
        t = v128 / v12c;
        if (t > lbl_eu_80666FE8) {
            t = lbl_eu_80666FE8;
        }
    }

    MenuEnemyPanel* panel = reinterpret_cast<MenuEnemyPanel*>(panelData);

    // One named Size temp (copy-inited from the first pane's size - no
    // zero-init stores) shared by all three sites; retail keeps a single
    // 0x8/0xc spill pair.
    nw4r::lyt::Pane* p;
    p = panel->layout1->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0xd3], true);
    nw4r::lyt::Size sz = p->GetSize();
    sz.width = self->field7C8 * t;
    p->SetSize(sz);

    p = panel->layout1->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0xdd], true);
    sz.height = p->GetSize().height;
    sz.width = self->field7CC * t;
    p->SetSize(sz);

    p = panel->layout1->GetRootPane()->FindPaneByName(
        &lbl_eu_804FDBF8[0x11d], true);
    sz.height = p->GetSize().height;
    sz.width = self->field7D0 * t;
    p->SetSize(sz);

    panel->layout1->Animate(0);
}

// Vtable adjustor thunks: retail "this" lands at the +0x5C subobject; back it
// off to the CMenuEnemyState base and forward (subi r3,r3,0x5c; b <fn>).
void func_801135D0(void* self) {
    reinterpret_cast<CMenuEnemyState*>(reinterpret_cast<char*>(self) - 0x5c)->cbRenderBefore();
}

void func_801135D8(void* self) {
    // r4 (delete flag) passes through untouched, so call through a 1-arg
    // pointer view.
    typedef void* (*DtorFn)(CMenuEnemyState*);
    ((DtorFn)__dt__15CMenuEnemyStateFv)(reinterpret_cast<CMenuEnemyState*>(reinterpret_cast<char*>(self) - 0x5c));
}

// retail: lwz r3, lbl_eu_80663F50; blr
extern "C" u32 func_80110A70() { return (u32)lbl_eu_80663F50; }

// func_801109D8 (us-801114b4): create-or-reuse the CMenuEnemyState singleton.
// When it does not exist yet and the shared arc accessor is available,
// allocate the object from the work heap, construct it with the scene,
// register it under the parent process and stash it in lbl_eu_80663F50. If it
// already exists, forward (existing, arg3) to func_80110A78. Returns the
// singleton (or 0 when the accessor was missing).
void* func_801109D8(void* parent, u32 scn, void* arg3) {
    if (lbl_eu_80663F50 == 0) {
        if (func_801355F4() == 0) {
            return 0;
        }
        CMenuEnemyState* p = (CMenuEnemyState*)mtl::MemManager::allocate(
            0x83c, CWorkThreadSystem::getWorkMem());
        if (p != 0) {
            // The ctor returns `this`, so reassigning keeps p in r3 across the
            // call (retail saves no extra register for it).
            p = (CMenuEnemyState*)__ct__CMenuEnemyState(p, (void*)scn);
        }
        lbl_eu_80663F50 = (u32)p;
        reinterpret_cast<CProcess*>(p)->Regist(
            reinterpret_cast<CProcess*>(parent), false);
    } else {
        func_80110A78(reinterpret_cast<CMenuEnemyState*>(lbl_eu_80663F50),
                      reinterpret_cast<u32>(arg3));
    }
    return (void*)lbl_eu_80663F50;
}

// func_80110A78 (us-80111554): register a new enemy indicator panel. Scans
// the first 16 panels for an existing entry with the same actor id (returns
// if found), finds the first invisible panel, then fills it from the actor
// object's flag bits, positions it via func_80111080, picks the colour/scale
// quad from the sdata globals (or the unk1D/unk1E/unk1F-selected s16 pairs)
// and applies the actor2 slot/type textures.
extern "C" void func_80110A78(CMenuEnemyState* self, u32 actorId) {
    if (self->field7C4 == 0) return;

    // Already registered -> nothing to do (entry-pointer walk: entry = this +
    // idx*0x4C, visible at +0xB9, actor id at +0xA4).
    for (u8 i = 0; i < 0x10; i++) {
        MenuEnemyPanel* entry = &self->panels[i];
        if (entry->visible != 0 && entry->actorId == actorId) return;
    }

    // First free slot (only 23 are scanned in retail).
    s32 freeIdx = -1;
    for (u8 i = 0; i < 0x17; i++) {
        if (self->panels[i].visible == 0) {
            freeIdx = i;
            break;
        }
    }
    if (freeIdx == -1) return;

    Obj64_91* obj = reinterpret_cast<Obj64_91*>(func_800B708C(static_cast<int>(actorId)));
    if (obj == NULL) return;

    // entry/panelData are the same address; retail keeps one base (r30).
    MenuEnemyPanel* panel = &self->panels[freeIdx];
    u8* panelData = reinterpret_cast<u8*>(panel);
    // Mirror the actor's flag word bits onto the panel state bytes.
    panelData[0x1C] = static_cast<u8>((obj->word64 >> 1) & 1);
    panelData[0x1D] = static_cast<u8>((obj->word64 >> 2) & 1);
    panelData[0x1E] = static_cast<u8>((obj->word64 >> 3) & 1);
    panelData[0x1F] = static_cast<u8>((obj->word64 >> 14) & 1);
    panelData[0x20] = 0;
    if (((obj->word64 >> 14) & 1) == 0) {
        panelData[0x1F] = static_cast<u8>((obj->word64 >> 15) & 1);
    }

    panel->actorId = actorId;
    panel->visible = 1;
    panel->animMarker = lbl_eu_80666FEC;
    panel->drawLayout0Flag = 1;
    panel->unk24 = 0;
    panel->panelType = 0;

    // Position fetch (0x12C object vec or the 0xAC position).
    typedef RLayout* (*GetVecFn)(void*, int);
    typedef void* (*GetPosFn)(void*);
    Vec3f posA;
    const Vec3f* posPtr;
    RLayout* r = vslot<GetVecFn>(obj, 0x12C)(obj, 0x64);
    if (r != NULL) {
        Vec3f tmp;
        tmp.x = r->val0C;
        tmp.y = r->val1C;
        tmp.z = r->val2C;
        posPtr = &tmp;
    } else {
        posPtr = static_cast<const Vec3f*>(vslot<GetPosFn>(obj, 0xAC)(obj));
    }
    posA = *posPtr;
    Vec3f posB = *static_cast<const Vec3f*>(vslot<GetPosFn>(obj, 0xAC)(obj));

    func_80111080(self, reinterpret_cast<u8*>(panel), &posA, &posB);

    // Colour/scale quads from sdata; the four words are individual retail
    // symbols (58 / 5C / 60 / 64). unk1D/unk1E/unk1F pick s16 pairs.
    u32 a[2];
    u32 b[2];
    a[0] = *reinterpret_cast<u32*>(&lbl_eu_80663F58[0]);
    a[1] = lbl_eu_80663F5C;
    b[0] = *reinterpret_cast<u32*>(&lbl_eu_80663F60[0]);
    b[1] = lbl_eu_80663F64;
    if (panel->unk1D != 0) {
        u16* au = reinterpret_cast<u16*>(a);
        u16* bu = reinterpret_cast<u16*>(b);
        au[0] = static_cast<u16>(lbl_eu_80663F68[0]);
        au[1] = static_cast<u16>(lbl_eu_80663F68[1]);
        au[2] = static_cast<u16>(lbl_eu_80663F68[2]);
        au[3] = static_cast<u16>(lbl_eu_80663F68[3]);
        bu[0] = static_cast<u16>(lbl_eu_80663F70[0]);
        bu[1] = static_cast<u16>(lbl_eu_80663F70[1]);
        bu[2] = static_cast<u16>(lbl_eu_80663F70[2]);
        bu[3] = static_cast<u16>(lbl_eu_80663F70[3]);
    } else if (panel->unk1E != 0) {
        u16* au = reinterpret_cast<u16*>(a);
        u16* bu = reinterpret_cast<u16*>(b);
        au[0] = static_cast<u16>(lbl_eu_80663F78[0]);
        au[1] = static_cast<u16>(lbl_eu_80663F78[1]);
        au[2] = static_cast<u16>(lbl_eu_80663F78[2]);
        au[3] = static_cast<u16>(lbl_eu_80663F78[3]);
        bu[0] = static_cast<u16>(lbl_eu_80663F80[0]);
        bu[1] = static_cast<u16>(lbl_eu_80663F80[1]);
        bu[2] = static_cast<u16>(lbl_eu_80663F80[2]);
        bu[3] = static_cast<u16>(lbl_eu_80663F80[3]);
    } else if (panel->unk1F != 0) {
        u16* au = reinterpret_cast<u16*>(a);
        u16* bu = reinterpret_cast<u16*>(b);
        au[0] = static_cast<u16>(lbl_eu_80663F88[0]);
        au[1] = static_cast<u16>(lbl_eu_80663F88[1]);
        au[2] = static_cast<u16>(lbl_eu_80663F88[2]);
        au[3] = static_cast<u16>(lbl_eu_80663F88[3]);
        bu[0] = static_cast<u16>(lbl_eu_80663F90[0]);
        bu[1] = static_cast<u16>(lbl_eu_80663F90[1]);
        bu[2] = static_cast<u16>(lbl_eu_80663F90[2]);
        bu[3] = static_cast<u16>(lbl_eu_80663F90[3]);
    }
    func_80139AC8(reinterpret_cast<void*>(panel->unk38), reinterpret_cast<void*>(a),
                  reinterpret_cast<void*>(b));

    reinterpret_cast<ObjBBFlag*>(panel->unk38)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(panel->obj2)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(panel->obj3)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(panel->obj1)->flagBB &= 0xFE;

    if (panel->unk1F != 0) return;

    Actor2Layout* actor2 = reinterpret_cast<Actor2Layout*>(func_8016FE34(obj));
    if (actor2 == NULL) return;

    typedef int* (*SubGetFn)(void*);
    int v = *vslot<SubGetFn>(actor2->subObj4, 0x30)(actor2->subObj4);
    panel->drawLayout0Flag = static_cast<u8>(func_80174C98(actor2, &v, 0x802));

    typedef void* (*GetObjFn)(void*);
    panel->unk24 = reinterpret_cast<u32>(vslot<GetObjFn>(actor2, 0x108)(actor2));

    func_80111C50(self, reinterpret_cast<u8*>(panel), (panel->unk1C != 0) ? 1 : 2);

    // Slot-type texture by the vt[0x258] value (1/2/3 select a name offset).
    typedef u32* (*GetPtrFn)(void*);
    u32* p258 = vslot<GetPtrFn>(actor2, 0x258)(actor2);
    void* tex = NULL;
    switch (*p258) {
    case 1:
        tex = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                  ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x20e], 0);
        break;
    case 2:
        tex = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                  ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x226], 0);
        break;
    case 3:
        tex = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                  ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x23e], 0);
        break;
    }
    if (tex != NULL) {
        func_80137F88(panel->obj2, tex);
        reinterpret_cast<ObjBBFlag*>(panel->obj2)->flagBB |= 1;
    }

    typedef u32 (*GetU8Fn)(void*);
    u8 byteVal = static_cast<u8>(vslot<GetU8Fn>(actor2, 0x260)(actor2));
    ObjBBFlag* f34 = reinterpret_cast<ObjBBFlag*>(panel->obj3);
    f34->flagBB = (f34->flagBB & 0xFE) | byteVal;
}

// func_801127B0 (us-8011328c): per-frame enemy-menu refresh driven by the
// last-selected actor id (func_800FE68C). The 24th panel (panels[23], aka
// panelData at this+0x778) mirrors the current selection; when it is already
// visible and the selection changed, or when it is hidden, the previous
// matching panel entry is cleared and the panel is re-registered from the
// actor's flag bits, position, colour quads and actor2 slot/type textures
// (same core as func_80110A78, then the highlight-scan panes and the scan
// timer are reset).
extern "C" void func_801127B0(CMenuEnemyState* self) {
    MenuEnemyPanel* panel = &self->panels[23];
    u8* panelData = reinterpret_cast<u8*>(panel);
    u32 lastId = static_cast<Fe68CView*>(func_800FE68C())->lastId90E4;

    if (panel->visible != 0) {
        // Already showing: clear and re-register when the selection changed.
        if (lastId == 0) {
            panel->visible = 0;
            self->field82C = 0;
            func_80138078(0x58);
            return;
        }
        if (self->field82C == lastId) return;

        panel->visible = 0;
        self->field82C = 0;
        Obj64_91* obj = reinterpret_cast<Obj64_91*>(
            func_800B708C(static_cast<int>(lastId)));
        if (obj == NULL) return;
        self->field82C = lastId;

        // Mirror the actor's flag word bits onto the panel state bytes.
        panelData[0x1C] = static_cast<u8>((obj->word64 >> 30) & 1);
        panelData[0x1D] = static_cast<u8>((obj->word64 >> 29) & 1);
        panelData[0x1E] = static_cast<u8>((obj->word64 >> 28) & 1);
        panelData[0x20] = 0;
        panelData[0x1F] = static_cast<u8>((obj->word64 >> 17) & 1);
        panelData[0x21] = 0;
        panelData[0x22] = 0;
        if (((obj->word64 >> 17) & 1) == 0) {
            panelData[0x1F] = static_cast<u8>((obj->word64 >> 16) & 1);
        }
        if (panelData[0x1F] == 0) {
            panelData[0x1F] = static_cast<u8>((obj->word64 >> 23) & 1);
            panelData[0x20] = panelData[0x1F];
        }
        if (panelData[0x1F] != 0 && panelData[0x20] == 0 && obj != NULL) {
            if (obj->byte91 == 0xC) {
                panelData[0x21] = 1;
            } else if (obj->byte91 == 0xE) {
                panelData[0x22] = 1;
            }
        }

        panel->actorId = lastId;
        panel->visible = 1;
        panel->animMarker = lbl_eu_80666FEC;
        panel->drawLayout0Flag = 1;
        panel->unk24 = 0;
        panel->panelType = 1;

        // Position fetch (0x12C object vec or the 0xAC position).
        typedef RLayout* (*GetVecFn)(void*, int);
        typedef void* (*GetPosFn)(void*);
        Vec3f posA;
        const Vec3f* posPtr;
        RLayout* r = vslot<GetVecFn>(obj, 0x12C)(obj, 0x64);
        if (r != NULL) {
            Vec3f tmp;
            tmp.x = r->val0C;
            tmp.y = r->val1C;
            tmp.z = r->val2C;
            posPtr = &tmp;
        } else {
            posPtr = static_cast<const Vec3f*>(vslot<GetPosFn>(obj, 0xAC)(obj));
        }
        posA = *posPtr;
        Vec3f posB = *static_cast<const Vec3f*>(vslot<GetPosFn>(obj, 0xAC)(obj));

        func_80111080(self, panelData, &posA, &posB);

        // Colour/scale quads from sdata; the four words are individual retail
        // symbols (58 / 5C / 60 / 64). unk1D/unk1E/unk1F pick s16 pairs.
        u32 a[2];
        u32 b[2];
        a[0] = *reinterpret_cast<u32*>(&lbl_eu_80663F58[0]);
        a[1] = lbl_eu_80663F5C;
        b[0] = *reinterpret_cast<u32*>(&lbl_eu_80663F60[0]);
        b[1] = lbl_eu_80663F64;
        if (panel->unk1D != 0) {
            u16* au = reinterpret_cast<u16*>(a);
            u16* bu = reinterpret_cast<u16*>(b);
            au[0] = static_cast<u16>(lbl_eu_80663F68[0]);
            au[1] = static_cast<u16>(lbl_eu_80663F68[1]);
            au[2] = static_cast<u16>(lbl_eu_80663F68[2]);
            au[3] = static_cast<u16>(lbl_eu_80663F68[3]);
            bu[0] = static_cast<u16>(lbl_eu_80663F70[0]);
            bu[1] = static_cast<u16>(lbl_eu_80663F70[1]);
            bu[2] = static_cast<u16>(lbl_eu_80663F70[2]);
            bu[3] = static_cast<u16>(lbl_eu_80663F70[3]);
        } else if (panel->unk1E != 0) {
            u16* au = reinterpret_cast<u16*>(a);
            u16* bu = reinterpret_cast<u16*>(b);
            au[0] = static_cast<u16>(lbl_eu_80663F78[0]);
            au[1] = static_cast<u16>(lbl_eu_80663F78[1]);
            au[2] = static_cast<u16>(lbl_eu_80663F78[2]);
            au[3] = static_cast<u16>(lbl_eu_80663F78[3]);
            bu[0] = static_cast<u16>(lbl_eu_80663F80[0]);
            bu[1] = static_cast<u16>(lbl_eu_80663F80[1]);
            bu[2] = static_cast<u16>(lbl_eu_80663F80[2]);
            bu[3] = static_cast<u16>(lbl_eu_80663F80[3]);
        } else if (panel->unk1F != 0) {
            u16* au = reinterpret_cast<u16*>(a);
            u16* bu = reinterpret_cast<u16*>(b);
            au[0] = static_cast<u16>(lbl_eu_80663F88[0]);
            au[1] = static_cast<u16>(lbl_eu_80663F88[1]);
            au[2] = static_cast<u16>(lbl_eu_80663F88[2]);
            au[3] = static_cast<u16>(lbl_eu_80663F88[3]);
            bu[0] = static_cast<u16>(lbl_eu_80663F90[0]);
            bu[1] = static_cast<u16>(lbl_eu_80663F90[1]);
            bu[2] = static_cast<u16>(lbl_eu_80663F90[2]);
            bu[3] = static_cast<u16>(lbl_eu_80663F90[3]);
        }
        func_80139AC8(reinterpret_cast<void*>(panel->unk38), reinterpret_cast<void*>(a),
                      reinterpret_cast<void*>(b));

        // Clear the highlight bits on the panel + shared panes.
        reinterpret_cast<ObjBBFlag*>(panel->obj2)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->obj3)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->unk40)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->obj1)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(self->field9C)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(self->fieldA0)->flagBB &= 0xFE;

        if (panel->unk1F == 0) {
            Actor2Layout* actor2 = reinterpret_cast<Actor2Layout*>(func_8016FE34(obj));
            if (actor2 != NULL) {
                typedef int* (*SubGetFn)(void*);
                int v = *vslot<SubGetFn>(actor2->subObj4, 0x30)(actor2->subObj4);
                panel->drawLayout0Flag = static_cast<u8>(func_80174C98(actor2, &v, 0x802));

                typedef void* (*GetObjFn)(void*);
                panel->unk24 = reinterpret_cast<u32>(vslot<GetObjFn>(actor2, 0x108)(actor2));

                func_80111C50(self, panelData, (panel->unk1C != 0) ? 1 : 2);

                // Slot-type texture by the vt[0x258] value (1/2/3 -> name).
                typedef u32* (*GetPtrFn)(void*);
                void* tex = NULL;
                switch (*vslot<GetPtrFn>(actor2, 0x258)(actor2)) {
                case 1:
                    tex = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                              ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x20e], 0);
                    break;
                case 2:
                    tex = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                              ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x226], 0);
                    break;
                case 3:
                    tex = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                              ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x23e], 0);
                    break;
                }
                if (tex != NULL) {
                    func_80137F88(panel->obj2, tex);
                    reinterpret_cast<ObjBBFlag*>(panel->obj2)->flagBB |= 1;
                }

                typedef u32 (*GetU8Fn)(void*);
                u8 byteVal = static_cast<u8>(vslot<GetU8Fn>(actor2, 0x260)(actor2));
                ObjBBFlag* f34 = reinterpret_cast<ObjBBFlag*>(panel->obj3);
                f34->flagBB = (f34->flagBB & 0xFE) | byteVal;
            }
        }

        // Reset the highlight-scan panes and the scan timer.
        reinterpret_cast<ObjBBFlag*>(self->field94)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(self->field98)->flagBB &= 0xFE;
        self->field7D8 = lbl_eu_80667038;
        self->field7DC = 0;
        self->field7E0 = 0;
        func_80138078(0x59);
    } else {
        // Hidden: clear the old matching panel entry, then re-register.
        if (lastId == 0) {
            panel->visible = 0;
            self->field82C = 0;
            return;
        }
        if (self->field82C == lastId) return;

        for (u8 i = 0; i < 24; i++) {
            if (self->panels[i].visible != 0 && self->panels[i].actorId == lastId) {
                self->panels[i].visible = 0;
                break;
            }
        }

        panel->visible = 0;
        self->field82C = 0;
        Obj64_91* obj = reinterpret_cast<Obj64_91*>(
            func_800B708C(static_cast<int>(lastId)));
        if (obj == NULL) return;
        self->field82C = lastId;

        // Mirror the actor's flag word bits onto the panel state bytes.
        panelData[0x1C] = static_cast<u8>((obj->word64 >> 30) & 1);
        panelData[0x1D] = static_cast<u8>((obj->word64 >> 29) & 1);
        panelData[0x1E] = static_cast<u8>((obj->word64 >> 28) & 1);
        panelData[0x20] = 0;
        panelData[0x1F] = static_cast<u8>((obj->word64 >> 17) & 1);
        if (((obj->word64 >> 17) & 1) == 0) {
            panelData[0x1F] = static_cast<u8>((obj->word64 >> 16) & 1);
        }
        if (panelData[0x1F] == 0) {
            panelData[0x1F] = static_cast<u8>((obj->word64 >> 23) & 1);
            panelData[0x20] = panelData[0x1F];
        }

        panel->actorId = lastId;
        panel->visible = 1;
        panel->animMarker = lbl_eu_80666FEC;
        panel->drawLayout0Flag = 1;
        panel->unk24 = 0;
        panel->panelType = 1;

        // Position fetch (0x12C object vec or the 0xAC position).
        typedef RLayout* (*GetVecFn2)(void*, int);
        typedef void* (*GetPosFn2)(void*);
        Vec3f posA2;
        const Vec3f* posPtr2;
        RLayout* r2 = vslot<GetVecFn2>(obj, 0x12C)(obj, 0x64);
        if (r2 != NULL) {
            Vec3f tmp2;
            tmp2.x = r2->val0C;
            tmp2.y = r2->val1C;
            tmp2.z = r2->val2C;
            posPtr2 = &tmp2;
        } else {
            posPtr2 = static_cast<const Vec3f*>(vslot<GetPosFn2>(obj, 0xAC)(obj));
        }
        posA2 = *posPtr2;
        Vec3f posB2 = *static_cast<const Vec3f*>(vslot<GetPosFn2>(obj, 0xAC)(obj));

        func_80111080(self, panelData, &posA2, &posB2);

        // Colour/scale quads from sdata; the four words are individual retail
        // symbols (58 / 5C / 60 / 64). unk1D/unk1E/unk1F pick s16 pairs.
        u32 a2[2];
        u32 b2[2];
        a2[0] = *reinterpret_cast<u32*>(&lbl_eu_80663F58[0]);
        a2[1] = lbl_eu_80663F5C;
        b2[0] = *reinterpret_cast<u32*>(&lbl_eu_80663F60[0]);
        b2[1] = lbl_eu_80663F64;
        if (panel->unk1D != 0) {
            u16* au2 = reinterpret_cast<u16*>(a2);
            u16* bu2 = reinterpret_cast<u16*>(b2);
            au2[0] = static_cast<u16>(lbl_eu_80663F68[0]);
            au2[1] = static_cast<u16>(lbl_eu_80663F68[1]);
            au2[2] = static_cast<u16>(lbl_eu_80663F68[2]);
            au2[3] = static_cast<u16>(lbl_eu_80663F68[3]);
            bu2[0] = static_cast<u16>(lbl_eu_80663F70[0]);
            bu2[1] = static_cast<u16>(lbl_eu_80663F70[1]);
            bu2[2] = static_cast<u16>(lbl_eu_80663F70[2]);
            bu2[3] = static_cast<u16>(lbl_eu_80663F70[3]);
        } else if (panel->unk1E != 0) {
            u16* au2 = reinterpret_cast<u16*>(a2);
            u16* bu2 = reinterpret_cast<u16*>(b2);
            au2[0] = static_cast<u16>(lbl_eu_80663F78[0]);
            au2[1] = static_cast<u16>(lbl_eu_80663F78[1]);
            au2[2] = static_cast<u16>(lbl_eu_80663F78[2]);
            au2[3] = static_cast<u16>(lbl_eu_80663F78[3]);
            bu2[0] = static_cast<u16>(lbl_eu_80663F80[0]);
            bu2[1] = static_cast<u16>(lbl_eu_80663F80[1]);
            bu2[2] = static_cast<u16>(lbl_eu_80663F80[2]);
            bu2[3] = static_cast<u16>(lbl_eu_80663F80[3]);
        } else if (panel->unk1F != 0) {
            u16* au2 = reinterpret_cast<u16*>(a2);
            u16* bu2 = reinterpret_cast<u16*>(b2);
            au2[0] = static_cast<u16>(lbl_eu_80663F88[0]);
            au2[1] = static_cast<u16>(lbl_eu_80663F88[1]);
            au2[2] = static_cast<u16>(lbl_eu_80663F88[2]);
            au2[3] = static_cast<u16>(lbl_eu_80663F88[3]);
            bu2[0] = static_cast<u16>(lbl_eu_80663F90[0]);
            bu2[1] = static_cast<u16>(lbl_eu_80663F90[1]);
            bu2[2] = static_cast<u16>(lbl_eu_80663F90[2]);
            bu2[3] = static_cast<u16>(lbl_eu_80663F90[3]);
        }
        func_80139AC8(reinterpret_cast<void*>(panel->unk38), reinterpret_cast<void*>(a2),
                      reinterpret_cast<void*>(b2));

        // Clear the highlight bits on the panel + shared panes.
        reinterpret_cast<ObjBBFlag*>(panel->obj2)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->obj3)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->unk40)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(panel->obj1)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(self->field9C)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(self->fieldA0)->flagBB &= 0xFE;

        if (panel->unk1F == 0) {
            Actor2Layout* actor2 = reinterpret_cast<Actor2Layout*>(func_8016FE34(obj));
            if (actor2 != NULL) {
                typedef int* (*SubGetFn2)(void*);
                int v2 = *vslot<SubGetFn2>(actor2->subObj4, 0x30)(actor2->subObj4);
                panel->drawLayout0Flag = static_cast<u8>(func_80174C98(actor2, &v2, 0x802));

                typedef void* (*GetObjFn2)(void*);
                panel->unk24 = reinterpret_cast<u32>(vslot<GetObjFn2>(actor2, 0x108)(actor2));

                func_80111C50(self, panelData, (panel->unk1C != 0) ? 1 : 2);

                // Slot-type texture by the vt[0x258] value (1/2/3 -> name).
                typedef u32* (*GetPtrFn2)(void*);
                void* tex2 = NULL;
                switch (*vslot<GetPtrFn2>(actor2, 0x258)(actor2)) {
                case 1:
                    tex2 = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                               ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x20e], 0);
                    break;
                case 2:
                    tex2 = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                               ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x226], 0);
                    break;
                case 3:
                    tex2 = static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4())
                               ->GetResource(0x74696D67, &lbl_eu_804FDBF8[0x23e], 0);
                    break;
                }
                if (tex2 != NULL) {
                    func_80137F88(panel->obj2, tex2);
                    reinterpret_cast<ObjBBFlag*>(panel->obj2)->flagBB |= 1;
                }

                typedef u32 (*GetU8Fn2)(void*);
                u8 byteVal2 = static_cast<u8>(vslot<GetU8Fn2>(actor2, 0x260)(actor2));
                ObjBBFlag* f34b = reinterpret_cast<ObjBBFlag*>(panel->obj3);
                f34b->flagBB = (f34b->flagBB & 0xFE) | byteVal2;
            }
        }

        // Reset the highlight-scan panes and the scan timer.
        reinterpret_cast<ObjBBFlag*>(self->field94)->flagBB &= 0xFE;
        reinterpret_cast<ObjBBFlag*>(self->field98)->flagBB &= 0xFE;
        self->field7D8 = lbl_eu_80667038;
        self->field7DC = 0;
        self->field7E0 = 0;
        func_80138078(0x57);
    }
}

// func_8010EE40 (us-8010f91c): CPcSelectCursor per-frame update. Clears the
// highlight bit on all six indicator panes, re-lights the four "target" panes
// when the battle actor list is non-empty, then positions the arrow pane
// (field28, or field34 when the battle target matches) over the current enemy
// (func_800B8B94 table entry by byte41, subobject at +0x3E9C): the cursor is
// projected with the scene pose and its Y only moves downward toward the
// enemy's live position.
extern "C" void func_8010EE40(CPcSelectCursorLayout* self) {
    // Retail reloads every pane field from the object (no pane locals).
    reinterpret_cast<ObjBBFlag*>(self->field28)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field34)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field2C)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field30)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field38)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field3C)->flagBB &= 0xFE;

    // Battle actor list non-empty -> light the four "target" panes.
    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    _reslist_node<cf::CfObjectActor*>* end = bm->mActorList1.mStartNodePtr;
    _reslist_node<cf::CfObjectActor*>* node = end->mNext;
    u32 count = 0;
    while (node != end) {
        node = node->mNext;
        count++;
    }
    if (count != 0) {
        reinterpret_cast<ObjBBFlag*>(self->field2C)->flagBB |= 1;
        reinterpret_cast<ObjBBFlag*>(self->field30)->flagBB |= 1;
        reinterpret_cast<ObjBBFlag*>(self->field38)->flagBB |= 1;
        reinterpret_cast<ObjBBFlag*>(self->field3C)->flagBB |= 1;
    }

    // Arrow pane; enemy subobject via the byte41 table entry (+4 words).
    ObjBBFlag* arrow = reinterpret_cast<ObjBBFlag*>(self->field28);
    u8* tbl = reinterpret_cast<u8*>(func_8009ECB0());
    void* entry = *reinterpret_cast<void**>(tbl + self->byte41 * 4 + 4);
    void* sub = func_800B8B94(static_cast<s32>(reinterpret_cast<u32>(entry)));
    PcEmbedLayout* target = NULL;
    if (sub != NULL) {
        target = reinterpret_cast<PcEmbedLayout*>(reinterpret_cast<u8*>(sub) + 0x3E9C);
    }
    if (target != NULL) {
        BattleTargetView* bt = static_cast<BattleTargetView*>(func_800EA444());
        if (bt != NULL && bt->field04 != NULL &&
            target->field74 == reinterpret_cast<u32>(bt->field04)) {
            arrow = reinterpret_cast<ObjBBFlag*>(self->field34);
        }
        arrow->flagBB |= 1;

        // Current enemy position (0x12C object vec or the 0xAC position).
        typedef RLayout* (*GetVecFn)(void*, int);
        typedef void* (*GetPosFn)(void*);
        Vec3f pos;
        RLayout* r = vslot<GetVecFn>(target, 0x12C)(target, 0x64);
        if (r != NULL) {
            pos.x = r->val0C;
            pos.y = r->val1C;
            pos.z = r->val2C;
        } else {
            pos = *static_cast<const Vec3f*>(vslot<GetPosFn>(target, 0xAC)(target));
        }

        PoseLayout* pose = static_cast<PoseLayout*>(
            func_80496264(reinterpret_cast<void*>(self->field04), -1));
        Vec3f v;
        func_8049B59C(&v, pose, &pos);
        pos = v;   // dead copy kept by retail (lwz/stw block)

        arrow->flagBB |= 1;
        if (v.x <= lbl_eu_80666FEC) {
            arrow->flagBB &= 0xFE;
        }

        // Marker Y moves only downward: scale, clamp, then snap to the live
        // position when it is below the clamped value.
        v.y = (v.y - lbl_eu_80666FF4) * lbl_eu_80666FF8;
        v.x = v.x - lbl_eu_80666FF0;
        v.z = lbl_eu_80666FEC;
        if (v.y > lbl_eu_80666FFC) {
            v.y = lbl_eu_80666FFC;
            Vec3f tmp;
            func_8049B59C(&tmp, pose, vslot<GetPosFn>(target, 0xAC)(target));
            tmp.y = (tmp.y - lbl_eu_80666FF4) * lbl_eu_80666FF8 - lbl_eu_80667000;
            if (v.y < (tmp.y >= v.y ? tmp.y : v.y)) {
                v.y = (tmp.y >= v.y ? tmp.y : v.y);
            }
        }
        PaneScaleVec3* pane = reinterpret_cast<PaneScaleVec3*>(arrow);
        pane->scale = v;
    }
}

// func_80112170 (us-80112c4c): refresh the enemy-name colour pane. When the
// panel's actor is a "special" enemy (word64 bit 2 + actor id), force the
// highlight colour; otherwise pick the colour from the diff between the
// panel's unk24 and the player-target id. Then scan the enemy slot list for a
// matching id and light the unk40 pane.
extern "C" void func_80112170(CMenuEnemyState* self, u8* panelData) {
    MenuEnemyPanel* panel = reinterpret_cast<MenuEnemyPanel*>(panelData);
    if (panel->unk1D == 0) return;

    cf::CfObjectPc* pc = func_800BFC68(cf::CfGameManager::getPlayer(0));
    if (pc == NULL) return;
    typedef void* (*GetObjFn)(void*);
    void* tgt = vslot<GetObjFn>(pc, 0x108)(pc);

    s32 special = 0;
    Obj64_91* obj = reinterpret_cast<Obj64_91*>(
        func_800B708C(static_cast<int>(panel->actorId)));
    if (obj != NULL && (obj->word64 & (1 << 2)) != 0) {
        u16 id = obj->id8C;
        if ((u32)(id - 0x967) <= 1 || id == 0x96b || id == 0x9c9) {
            special = 1;
        }
    }

    if (special != 0) {
        nw4r::lyt::Pane* pane = panel->layout2->GetRootPane()->FindPaneByName(
            &lbl_eu_804FDBF8[0x28e], true);
        func_80137DB8(pane, 0xFF5A3CFF, 0xFF5A3CFF);
        if (panel->panelType != 0) {
            func_80137DB8(reinterpret_cast<void*>(self->field8C), 0xFF5A3CFF, 0xFF5A3C00);
            func_80137DB8(reinterpret_cast<void*>(self->field90), 0xFF5A3CFF, 0xFF5A3C00);
        }
    } else {
        nw4r::lyt::Pane* pane = panel->layout2->GetRootPane()->FindPaneByName(
            &lbl_eu_804FDBF8[0x28e], true);
        s32 diff = static_cast<s32>(panel->unk24 - reinterpret_cast<u32>(tgt));
        if (diff >= 6) {
            func_80137DB8(pane, 0xFF5A3CFF, 0xFF5A3CFF);
        } else if (diff >= 3) {
            func_80137DB8(pane, 0xD2D228FF, 0xD2D228FF);
        } else if (diff <= -6) {
            func_80137DB8(pane, 0, 0);
        } else if (diff <= -3) {
            func_80137DB8(pane, 0x288CFFFF, 0x288CFFFF);
        } else {
            func_80137DB8(pane, 0xD2D3D2FF, 0xD2D3D2FF);
        }
        if (panel->panelType != 0) {
            if (diff >= 6) {
                func_80137DB8(reinterpret_cast<void*>(self->field8C), 0xFF5A3CFF, 0xFF5A3C00);
                func_80137DB8(reinterpret_cast<void*>(self->field90), 0xFF5A3CFF, 0xFF5A3C00);
            } else if (diff >= 3) {
                func_80137DB8(reinterpret_cast<void*>(self->field8C), 0xD2D228FF, 0xD2D22800);
                func_80137DB8(reinterpret_cast<void*>(self->field90), 0xD2D228FF, 0xD2D22800);
            } else if (diff <= -6) {
                func_80137DB8(reinterpret_cast<void*>(self->field8C), 0, 0);
                func_80137DB8(reinterpret_cast<void*>(self->field90), 0, 0);
            } else if (diff <= -3) {
                func_80137DB8(reinterpret_cast<void*>(self->field8C), 0x288CFFFF, 0x288CFF00);
                func_80137DB8(reinterpret_cast<void*>(self->field90), 0x288CFFFF, 0x288CFF00);
            } else {
                func_80137DB8(reinterpret_cast<void*>(self->field8C), 0xD2D3D2FF, 0xD2D3D200);
                func_80137DB8(reinterpret_cast<void*>(self->field90), 0xD2D3D2FF, 0xD2D3D200);
            }
        }
    }

    // Enemy slot list scan: light unk40 when a slot holds the same actor id.
    // (Retail reloads panel->unk40 for the clear and the set.)
    reinterpret_cast<ObjBBFlag*>(panel->unk40)->flagBB &= 0xFE;
    u32 lastId = static_cast<Fe68CView*>(func_800FE68C())->lastId90E4;
    Obj64_91* lastObj = reinterpret_cast<Obj64_91*>(
        func_800B708C(static_cast<int>(lastId)));
    if (lastObj != NULL) {
        void* sub = func_800AD860(lastObj);
        if (sub != NULL) {
            void* list = func_800B8A64();
            if (list != NULL) {
                for (u8 i = 0; i < 0x10; i++) {
                    EnemySlotListEntry* e = static_cast<EnemySlotListEntry*>(
                        func_801984F0(list, i));
                    if (e != NULL && e != sub && e->id3F10 == panel->actorId) {
                        reinterpret_cast<ObjBBFlag*>(panel->unk40)->flagBB |= 1;
                        break;
                    }
                }
            }
        }
    }
}

// func_801115E8 (us-801120c4): per-panel highlight update. Selects the
// highlight style by unk1E/unk1F/unk20/unk1D, loads the matching name text
// (or formats the actor2 sub-id with sprintf), refreshes the highlight-slot
// textures (func_801124C8) and closes with the unk44 scale reset.
extern "C" void func_801115E8(CMenuEnemyState* self, u8* panelData) {
    MenuEnemyPanel* panel = reinterpret_cast<MenuEnemyPanel*>(panelData);
    panel->panelType = 0;
    Obj64_91* obj = reinterpret_cast<Obj64_91*>(
        func_800B708C(static_cast<int>(panel->actorId)));
    if (obj == NULL) return;

    // Only the last-selected enemy gets the full highlight path. (Retail
    // reloads the panel/self pane fields at every site - no cached locals.)
    reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB &= 0xFE;
    if (panel->unk1D != 0) {
        reinterpret_cast<ObjBBFlag*>(panel->obj1)->flagBB |= 1;
    }
    // Retail: li r5,2 default, overwritten with 1 - must stay a branchy
    // if/else (the ?: form compiles branchless via neg/srawi).
    int which = 2;
    if (panel->unk1C != 0) {
        which = 1;
    }
    func_80111C50(self, panelData, which);

    u32 lastId = static_cast<Fe68CView*>(func_800FE68C())->lastId90E4;
    if (lastId != panel->actorId) return;
    Obj64_91* lastObj = reinterpret_cast<Obj64_91*>(
        func_800B708C(static_cast<int>(lastId)));
    if (lastObj == NULL) return;

    u32 sub74 = lastObj->word74;
    reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB |= 1;
    reinterpret_cast<ObjBBFlag*>(panel->obj1)->flagBB &= 0xFE;
    func_80111C50(self, panelData, 3);
    panel->panelType = 1;
    if (self->field7D4 != sub74) {
        self->field7D4 = sub74;
    }

    reinterpret_cast<ObjBBFlag*>(self->field7C)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field80)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field84)->flagBB &= 0xFE;
    reinterpret_cast<ObjBBFlag*>(self->field88)->flagBB &= 0xFE;

    typedef char* (*GetStrFn)(void*);
    if (panel->unk1E != 0 || panel->unk1F != 0) {
        reinterpret_cast<ObjBBFlag*>(self->field7C)->flagBB |= 1;
        if (panel->unk20 != 0) {
            char* name = func_80136190(&lbl_eu_804FDBF8[0x256],
                                       &lbl_eu_804FDBF8[0x261], 8);
            func_80136B4C(self->unk74, &lbl_eu_804FDBF8[0x266], name, 0);
        } else if (panel->unk1F != 0) {
            func_80136B4C(self->unk74, &lbl_eu_804FDBF8[0x266],
                          vslot<GetStrFn>(obj, 0x40)(obj), 0);
        } else {
            func_80136B4C(self->unk74, &lbl_eu_804FDBF8[0x266],
                          func_80138DA4(vslot<GetStrFn>(obj, 0x40)(obj)), 0);
        }
    } else if (panel->unk1D != 0) {
        reinterpret_cast<ObjBBFlag*>(self->field80)->flagBB |= 1;
        Actor2Layout* actor2 = reinterpret_cast<Actor2Layout*>(func_8016FE34(obj));
        u32 special = 0;
        if ((obj->word64 & (1 << 2)) != 0) {
            u16 id = obj->id8C;
            if ((u32)(id - 0x967) <= 1 || id == 0x96b || id == 0x9c9) {
                special = 1;
            }
        }
        typedef void* (*GetObjFn)(void*);
        void* a2id = vslot<GetObjFn>(actor2, 0x108)(actor2);
        if (special != 0) {
            char* name = func_80136190(&lbl_eu_804FDBF8[0x256],
                                       &lbl_eu_804FDBF8[0x261], 7);
            func_80136B4C(self->unk74, &lbl_eu_804FDBF8[0x1b2], name, 0);
        } else {
            char buf[0x20];
            sprintf(buf, &lbl_eu_804FDBF8[0x273], a2id);
            func_80136A1C(self->unk74, &lbl_eu_804FDBF8[0x1b2], buf, 0);
        }
        a2id = vslot<GetObjFn>(actor2, 0x108)(actor2);
        if (panel->unk24 != reinterpret_cast<u32>(a2id)) {
            panel->unk24 = reinterpret_cast<u32>(
                vslot<GetObjFn>(actor2, 0x108)(actor2));
            func_80112170(self, panelData);
        }
        func_80136B4C(self->unk74, &lbl_eu_804FDBF8[0x278],
                      vslot<GetStrFn>(actor2, 0x98)(actor2), 0);
        func_80137B44(self->unk74, &lbl_eu_804FDBF8[0x278], 0xEDE8DAFF);
        func_801124C8(self, actor2);

        void* sub = func_800AD860(obj);
        if (sub != NULL) {
            u8 v = func_801361E8(lbl_eu_806640CC, &lbl_eu_804FDBF8[0x288],
                                 static_cast<AD860Result*>(sub)->id3F28);
            if (v == 1) {
                reinterpret_cast<ObjBBFlag*>(self->field84)->flagBB |= 1;
            } else if (v == 2) {
                reinterpret_cast<ObjBBFlag*>(self->field88)->flagBB |= 1;
            }
        }

        typedef int* (*SubGetFn)(void*);
        int v1 = *vslot<SubGetFn>(actor2->subObj4, 0x30)(actor2->subObj4);
        if (func_80174C98(actor2, &v1, 0xa) == 0) {
            int v2 = *vslot<SubGetFn>(actor2->subObj4, 0x30)(actor2->subObj4);
            if (func_80174C98(actor2, &v2, 9) == 0) {
                goto tail;
            }
        }
        Obj298View* v298 = static_cast<Obj298View*>(
            vslot<GetObjFn>(actor2, 0x298)(actor2));
        if (v298->field50 != NULL) {
            Obj50View* o50 = static_cast<Obj50View*>(v298->field50);
            if ((o50->word78 & (1 << 30)) != 0 && o50->f7C == lbl_eu_80666FEC) {
                func_80136B4C(self->unk74, &lbl_eu_804FDBF8[0x278],
                              reinterpret_cast<char*>(o50), 0);
                func_80137B44(self->unk74, &lbl_eu_804FDBF8[0x278], 0xF52819FF);
            }
        }
    }
tail:
    PaneScaleVec3* v = reinterpret_cast<PaneScaleVec3*>(panel->unk44);
    Vec3f tmp = v->scale;
    tmp.y = lbl_eu_80667028;
    v->scale = tmp;
}

// func_80111080 (us-80111b5c): position one enemy indicator panel. Projects
// posA with the scene pose into v, lights the two indicator panes, then for
// panelType==0 builds the marker scale from the (1,1,0) normal transform and
// the posA/t offsets and writes posA + the scaled pose vector into the
// layout2 root pane; otherwise zeroes the vector fields. Then the position
// marker v is clamped (y only moves downward) and written into the layout1
// root pane (and layout2/unk74 panes when panelType != 0).
extern "C" void func_80111080(CMenuEnemyState* self, u8* panelData, void* posA, void* posB) {
    MenuEnemyPanel* panel = reinterpret_cast<MenuEnemyPanel*>(panelData);
    PoseLayout* pose = static_cast<PoseLayout*>(func_80496264(self->unk60, -1));
    const nw4r::math::VEC3* pa = static_cast<const nw4r::math::VEC3*>(posA);
    const nw4r::math::VEC3* pb = static_cast<const nw4r::math::VEC3*>(posB);
    nw4r::math::VEC3 v;
    func_8049B59C(&v, pose, pa);
    panel->animMarker = v.z;

    ObjBBFlag* f4 = reinterpret_cast<ObjBBFlag*>(panel->layout1->GetRootPane());
    ObjBBFlag* fC = reinterpret_cast<ObjBBFlag*>(panel->layout2->GetRootPane());
    f4->flagBB |= 1;
    fC->flagBB |= 1;
    if (v.x <= lbl_eu_80666FEC) {
        f4->flagBB &= 0xFE;
        fC->flagBB &= 0xFE;
    }

    PanePosLayout* pane = reinterpret_cast<PanePosLayout*>(panel->layout2->GetRootPane());
    if (panel->panelType == 0) {
        nw4r::math::VEC3 t;
        t.x = lbl_eu_80666FE8;
        t.y = lbl_eu_80666FE8;
        t.z = lbl_eu_80666FEC;
        VEC3TransformNormal(&t, &pose->mtx9C, &t);
        nw4r::math::VEC3 sum = *pa + t;
        func_8049B59C(&sum, pose, &sum);
        f32 scaleX = lbl_eu_80666FE8 / fabsf(v.x - t.x);
        f32 scaleY = lbl_eu_80666FE8 / fabsf(v.y - t.y);
        if (!CDeviceVI::isWideAspectRatio()) {
            scaleX *= lbl_eu_80667018;
        }
        f32 s = lbl_eu_8066701C / lbl_eu_8066A1F8;
        pane->v2C = pa->x;
        pane->v30 = pa->y;
        pane->v34 = pa->z;
        nw4r::math::VEC3 sc =
            *reinterpret_cast<const nw4r::math::VEC3*>(&pose->vec118_x) * s;
        pane->v38 = sc.x;
        pane->v3C = sc.y;
        pane->v40 = sc.z;
        pane->v44 = scaleX;
        pane->v48 = scaleY;
    } else {
        pane->v38 = zero__Q22ml5CVec3.x;
        pane->v3C = zero__Q22ml5CVec3.y;
        pane->v40 = zero__Q22ml5CVec3.z;
        pane->v44 = lbl_eu_80666FE8;
        pane->v48 = lbl_eu_80666FE8;
    }

    panel->unk29 = 1;
    nw4r::math::VEC3 a;
    a.x = pose->vec10c_x;
    a.y = pose->vec10c_y;
    a.z = pose->vec10c_z;
    nw4r::math::VEC3 b;
    b.x = pose->vec138_x;
    b.y = pose->vec138_y;
    b.z = pose->vec138_z;
    if (!func_8013A4B4(&a, &b, const_cast<nw4r::math::VEC3*>(pa))) {
        panel->unk29 = 0;
    }

    // Marker Y moves only downward: scale, clamp per panelType, snap.
    v.y = (v.y - lbl_eu_80666FF4) * lbl_eu_80666FF8;
    v.x = v.x - lbl_eu_80666FF0;
    v.z = lbl_eu_80666FEC;
    if (panel->panelType != 0) {
        if (v.y > lbl_eu_80667020) {
            v.y = lbl_eu_80667020;
            nw4r::math::VEC3 tmp;
            func_8049B59C(&tmp, pose, pb);
            tmp.y = (tmp.y - lbl_eu_80666FF4) * lbl_eu_80666FF8 - lbl_eu_80667024;
            if (v.y < (tmp.y >= v.y ? tmp.y : v.y)) {
                v.y = (tmp.y >= v.y ? tmp.y : v.y);
            }
            reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB &= 0xFE;
        }
    } else if (panel->unk1C != 0) {
        if (v.y > lbl_eu_80666FFC) {
            v.y = lbl_eu_80666FFC;
            nw4r::math::VEC3 tmp;
            func_8049B59C(&tmp, pose, pb);
            tmp.y = (tmp.y - lbl_eu_80666FF4) * lbl_eu_80666FF8 - lbl_eu_80667000;
            if (v.y < (tmp.y >= v.y ? tmp.y : v.y)) {
                v.y = (tmp.y >= v.y ? tmp.y : v.y);
            }
            reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB &= 0xFE;
        }
    } else {
        if (v.y > lbl_eu_80667020) {
            v.y = lbl_eu_80667020;
            nw4r::math::VEC3 tmp;
            func_8049B59C(&tmp, pose, pb);
            tmp.y = (tmp.y - lbl_eu_80666FF4) * lbl_eu_80666FF8 - lbl_eu_80667024;
            if (v.y < (tmp.y >= v.y ? tmp.y : v.y)) {
                v.y = (tmp.y >= v.y ? tmp.y : v.y);
            }
            reinterpret_cast<ObjBBFlag*>(panel->unk3C)->flagBB &= 0xFE;
        }
    }

    PanePosLayout* p1 = reinterpret_cast<PanePosLayout*>(panel->layout1->GetRootPane());
    p1->v2C = v.x;
    p1->v30 = v.y;
    p1->v34 = v.z;
    if (panel->panelType != 0) {
        pane->v2C = v.x;
        pane->v30 = v.y;
        pane->v34 = v.z;
        PanePosLayout* p74 =
            reinterpret_cast<PanePosLayout*>(self->unk74->GetRootPane());
        p74->v2C = v.x;
        p74->v30 = v.y;
        p74->v34 = v.z;
    }
}

// func_801124C8 (us-80112fa4): enemy-slot highlight scan. When the highlight
// timer (field7D8) passes its threshold, reset it and scan the actor's slots
// (from field7DC+1, wrapping at 0x20) for a valid id, loading the matching
// texture and flagging field94. Then rebuild the 32-entry id table plus the
// 0x20 sentinel from actor2->field1530 and scan field98 from field7E0+1
// (wrapping at 0x21) the same way.
extern "C" void func_801124C8(CMenuEnemyState* self, Actor2Layout* actor2) {
    self->field7D8 += lbl_eu_80666FE8;
    if (self->field7D8 > lbl_eu_80667038) {
        self->field7D8 = lbl_eu_80666FEC;
        u8 idx = (u8)(self->field7DC + 1);
        for (;;) {
            if (idx >= 0x20) {
                idx = 0;
            }
            ObjBBFlag* f94 = reinterpret_cast<ObjBBFlag*>(self->field94);
            f94->flagBB &= 0xFE;
            SubSlot5CResult* res =
                reinterpret_cast<Sub8Probe*>(&actor2->sub8)->probe(idx);
            if (res->word30 != 0x800 && res->id0C != 0 && res->id0C != 0xF &&
                res->id0C != 0x10 && res->id0C != 0x12) {
                u16 id = func_80136254(lbl_eu_806640E0,
                                       &lbl_eu_804FDBF8[0x295], res->id0C);
                if (id != 0) {
                    char* name = func_80138F78(id);
                    nw4r::lyt::ArcResourceAccessor* accessor =
                        static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4());
                    void* tex = accessor->GetResource(0x74696D67, name, 0);
                    if (tex != 0) {
                        f94->flagBB = (f94->flagBB & 0xFE) | 1;
                        func_80137F88(f94, tex);
                        self->field7DC = idx;
                        break;
                    }
                }
            }
            if ((s32)(u8)idx == (s32)self->field7DC) {
                break;
            }
            idx++;
        }

        {
            u16 arr16[0x21];
            for (u8 i = 0; i < 0x20; i++) {
                SubSlot5CResult* res =
                    reinterpret_cast<Sub8Probe*>(&actor2->sub8)->probe2(i);
                arr16[i] = (res->word30 == 0x800) ? 0 : res->id0C;
            }
            arr16[0x20] = (u16)actor2->field1530;

            u8 idx2 = (u8)(self->field7E0 + 1);
            for (;;) {
                if (idx2 >= 0x21) {
                    idx2 = 0;
                }
                ObjBBFlag* f98 = reinterpret_cast<ObjBBFlag*>(self->field98);
                f98->flagBB &= 0xFE;
                u16 v = arr16[idx2];
                if (v != 0) {
                    if (idx2 == 0x20) {
                        char* name = func_80138F78(0x13d);
                        nw4r::lyt::ArcResourceAccessor* accessor =
                            static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4());
                        void* tex = accessor->GetResource(0x74696D67, name, 0);
                        if (tex != 0) {
                            f98->flagBB = (f98->flagBB & 0xFE) | 1;
                            func_80137F88(f98, tex);
                            self->field7E0 = idx2;
                            break;
                        }
                    } else {
                        u16 id = func_80136254(lbl_eu_806640E0,
                                               &lbl_eu_804FDBF8[0x295], v);
                        if (id != 0) {
                            char* name = func_80138F78(id);
                            nw4r::lyt::ArcResourceAccessor* accessor =
                                static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4());
                            void* tex = accessor->GetResource(0x74696D67, name, 0);
                            if (tex != 0) {
                                f98->flagBB = (f98->flagBB & 0xFE) | 1;
                                func_80137F88(f98, tex);
                                self->field7E0 = idx2;
                                break;
                            }
                        }
                    }
                }
                if ((s32)(u8)idx2 == (s32)self->field7E0) {
                    break;
                }
                idx2++;
            }
        }
    }
}



// func_80111C50 (us-8011272c): highlight one of the three enemy-indicator
// panes - two found by name off panelData->layout1's root pane, the third is
// panelData->unk44. `which` 0-3 selects which pane gets its +0xBB bit-0 set
// (the others are cleared); case 3 additionally plays a layout sound and
// nudges a scale vec3 on unk44. Called from func_80111E70 with which=3.
// (arg1 `self` is not referenced by retail.)
extern "C" void func_80111C50(CMenuEnemyState* self, u8* panelData, int which) {
    MenuEnemyPanel* panel = reinterpret_cast<MenuEnemyPanel*>(panelData);

    ObjBBFlag* f1 = reinterpret_cast<ObjBBFlag*>(
        panel->layout1->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x10b], true));
    ObjBBFlag* f2 = reinterpret_cast<ObjBBFlag*>(
        panel->layout1->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x112], true));
    ObjBBFlag* f3 = reinterpret_cast<ObjBBFlag*>(panel->unk44);

    switch (which) {
    case 0:
        f1->flagBB &= 0xFE;
        f2->flagBB &= 0xFE;
        f3->flagBB &= 0xFE;
        break;
    case 1:
        f1->flagBB = (f1->flagBB & 0xFE) | 1;
        f2->flagBB &= 0xFE;
        f3->flagBB &= 0xFE;
        break;
    case 2:
        f1->flagBB &= 0xFE;
        f2->flagBB = (f2->flagBB & 0xFE) | 1;
        f3->flagBB &= 0xFE;
        break;
    case 3:
        f1->flagBB &= 0xFE;
        f2->flagBB &= 0xFE;
        f3->flagBB = (f3->flagBB & 0xFE) | 1;
        func_80137B44(panel->layout1, &lbl_eu_804FDBF8[0x11d],
                      (panel->unk1C != 0) ? 0x0CD8FDFF : 0xFAF01EFF);
        if (panel->unk1C != 0) {
            PaneScaleVec3* v = reinterpret_cast<PaneScaleVec3*>(panel->unk44);
            Vec3f tmp = v->scale;
            tmp.y = lbl_eu_80666FEC;
            v->scale = tmp;
        } else if (panel->unk1D != 0 && panel->panelType == 0) {
            PaneScaleVec3* v = reinterpret_cast<PaneScaleVec3*>(panel->unk44);
            Vec3f tmp = v->scale;
            tmp.y = lbl_eu_80667008;
            v->scale = tmp;
        }
        break;
    }
}

// func_801132A8 (us-80113d84): panel-highlight texture search. Scans the
// actor's 32 sub8 slots for a u16 id matching one of the four byte patterns
// taken from lbl_eu_8066703C; on a match, loads the corresponding texture
// (tbl32 index) through the shared arc accessor and raises field48's bit-0.
// When the flag is set, the 5-float vector on field48 (+0x2C..+0x48) is
// re-derived from the panel's highlight state. (arg1 `self` is unused.)
void func_801132A8(CMenuEnemyState* self, u8* panelData, void* actor) {
    MenuEnemyPanel* panel = reinterpret_cast<MenuEnemyPanel*>(panelData);
    PanelObj48* obj48 = reinterpret_cast<PanelObj48*>(panel->unk48);
    Actor2Layout* actor2 = reinterpret_cast<Actor2Layout*>(actor);
    obj48->flagBB &= 0xFE;

    u32 bits = *(const u32*)&lbl_eu_8066703C;
    U32x4 tbl32 = *reinterpret_cast<const U32x4*>(lbl_eu_804FDBC8);

    for (u8 oi = 0; oi < 4; oi++) {
        for (u8 ii = 0; ii < 0x20; ii++) {
            SubSlot5CResult* res =
                reinterpret_cast<Sub8Probe*>(&actor2->sub8)->probe(ii);
            // Retail reads the pattern byte after the id check (short-circuit)
            // and the texture index inside the load call.
            if (res->id0C != 0 && res->id0C == *((u8*)&bits + oi)) {
                nw4r::lyt::ArcResourceAccessor* accessor =
                    static_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4());
                void* tex = accessor->GetResource(
                    0x74696D67, reinterpret_cast<const char*>((&tbl32.a)[oi]), 0);
                if (tex != 0) {
                    obj48->flagBB = (obj48->flagBB & 0xFE) | 1;
                    func_80137F88(obj48, tex);
                    break;
                }
            }
        }
        if (obj48->flagBB & 1) {
            break;
        }
    }

    if (obj48->flagBB & 1) {
        // Re-derive the 5-float scale/colour vector from panel state.
        Vec3f v3 = *reinterpret_cast<Vec3f*>(&obj48->v2C);
        Vec2f v2 = *reinterpret_cast<Vec2f*>(&obj48->v44);
        v3.z = lbl_eu_80666FEC;
        if (panel->panelType != 0) {
            v3.x = lbl_eu_80667040;
            v3.y = lbl_eu_80667044;
            v2.x = lbl_eu_80666FE8;
            v2.y = lbl_eu_80666FE8;
        } else {
            v2.x = lbl_eu_80667048;
            v2.y = lbl_eu_80667048;
            if (panel->unk1C != 0) {
                ObjBBFlag* p44 = reinterpret_cast<ObjBBFlag*>(panel->unk44);
                if (p44->flagBB & 1) {
                    v3.x = lbl_eu_8066704C;
                    v3.y = lbl_eu_80666FEC;
                } else {
                    v3.x = lbl_eu_80667050;
                    v3.y = lbl_eu_80666FEC;
                }
            } else if (panel->unk1D != 0) {
                ObjBBFlag* p44 = reinterpret_cast<ObjBBFlag*>(panel->unk44);
                if (p44->flagBB & 1) {
                    v3.x = lbl_eu_8066704C;
                    v3.y = lbl_eu_80667008;
                } else {
                    v3.x = lbl_eu_80667054;
                    v3.y = lbl_eu_80667008;
                }
            }
        }
        obj48->v2C = v3.x;
        obj48->v30 = v3.y;
        obj48->v34 = v3.z;
        obj48->v44 = v2.x;
        obj48->v48 = v2.y;
    }
}

// ---------------------------------------------------------------------------
// CMenuEnemyState::Init (us-8010ff24): create the 24-panel enemy-indicator
// array (zeroed template copied into each entry), build the per-panel
// layout1/layout2 arcs + panes, snapshot the shared colour quads from pane
// 0x147 on the first pass, then set up the main menu layout, the font, the
// ten pane handles and the select-cursor subobject, register the render CB
// and release the scratch region.
// ---------------------------------------------------------------------------
void CMenuEnemyState::Init() {
    // Scratch region for all menu layouts (freed by the RAII guard). Retail
    // re-derives this+0x64 at every use - no cached region pointer.
    reinterpret_cast<UnkClass_8045F564*>(unk64)->createRegion(
        mtl::MemManager::getHandleMEM2(), 0x3EC00, &lbl_eu_804FDBF8[0xa8], 0);
    Class_8045F858 regionGuard(reinterpret_cast<UnkClass_8045F564*>(unk64));

    // Loop-invariant values hoisted by retail before the loop (f30/f31 +
    // r21-r24): the anim marker, the marker y value, zero, one and the two
    // pane sound/colour constants.
    f32 animMarker = lbl_eu_80666FEC;
    f32 markerY = lbl_eu_80667008;
    u32 zero = 0;
    u8 one = 1;
    u32 colD3 = 0xCD8FDFF;
    u32 colDD = 0xFAF01EFF;

    for (u8 i = 0; i < 24; i++) {
        MenuEnemyPanel* panel = &panels[i];
        nw4r::lyt::Layout** pLayout1 = &panel->layout1;
        // Volatile template copy: retail writes the same constant values to a
        // dead stack panelData after the entry stores (3 hoisted into the
        // index-computation latency, the rest after). volatile stops MWCC
        // dead-store-eliminating the stack copy.
        volatile MenuEnemyPanel panelData;

        panel->actorId = zero;
        panel->layout1 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panel->unk08 = zero;
        panel->layout2 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panel->unk10 = zero;
        panel->drawLayout0Flag = zero;
        panel->visible = zero;
        panel->animMarker = animMarker;
        panel->unk1C = zero;
        panel->unk1D = zero;
        panel->unk1E = zero;
        panel->unk1F = zero;
        panel->unk20 = zero;
        panel->unk21 = zero;
        panel->unk22 = zero;
        panel->unk24 = zero;
        panel->panelType = zero;
        panel->unk29 = one;
        panel->obj1 = reinterpret_cast<void*>(zero);
        panel->obj2 = reinterpret_cast<void*>(zero);
        panel->obj3 = reinterpret_cast<void*>(zero);
        panel->unk38 = zero;
        panel->unk3C = zero;
        panel->unk40 = zero;
        panel->unk44 = zero;
        panel->unk48 = zero;

        panelData.actorId = zero;
        panelData.layout1 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panelData.unk08 = zero;
        panelData.layout2 = reinterpret_cast<nw4r::lyt::Layout*>(zero);
        panelData.unk10 = zero;
        panelData.drawLayout0Flag = zero;
        panelData.visible = zero;
        panelData.animMarker = animMarker;
        panelData.unk1C = zero;
        panelData.unk1D = zero;
        panelData.unk1E = zero;
        panelData.unk1F = zero;
        panelData.unk20 = zero;
        panelData.unk21 = zero;
        panelData.unk22 = zero;
        panelData.unk24 = zero;
        panelData.panelType = zero;
        panelData.unk29 = one;
        panelData.obj1 = reinterpret_cast<void*>(zero);
        panelData.obj2 = reinterpret_cast<void*>(zero);
        panelData.obj3 = reinterpret_cast<void*>(zero);
        panelData.unk38 = zero;
        panelData.unk3C = zero;
        panelData.unk40 = zero;
        panelData.unk44 = zero;
        panelData.unk48 = zero;

        // ---- layout1 arc + panes ----
        func_80136E84(pLayout1,
                      reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4()),
                      &lbl_eu_804FDBF8[0xb8]);
        func_80137B44(*pLayout1, &lbl_eu_804FDBF8[0xd3], colD3);
        func_80137B44(*pLayout1, &lbl_eu_804FDBF8[0xdd], colDD);

        {
            nw4r::lyt::Pane* p = (*pLayout1)->GetRootPane()
                ->FindPaneByName(&lbl_eu_804FDBF8[0xe7], true);
            reinterpret_cast<ObjBBFlag*>(p)->flagBB &= 0xFE;
        }
        {
            nw4r::lyt::Pane* p = (*pLayout1)->GetRootPane()
                ->FindPaneByName(&lbl_eu_804FDBF8[0xf3], true);
            reinterpret_cast<ObjBBFlag*>(p)->flagBB &= 0xFE;
        }
        {
            nw4r::lyt::Pane* p = (*pLayout1)->GetRootPane()
                ->FindPaneByName(&lbl_eu_804FDBF8[0x101], true);
            reinterpret_cast<ObjBBFlag*>(p)->flagBB &= 0xFE;
        }

        // Copy pane 0x10b onto itself preserving x/z, resetting y. One temp
        // Vec3f is shared by both copy blocks (retail keeps a single slot).
        {
            Vec3f tmp;
            {
                PanePosLayout* s = reinterpret_cast<PanePosLayout*>((*pLayout1)
                    ->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x10b], true));
                tmp = *reinterpret_cast<Vec3f*>(&s->v2C);
                PanePosLayout* d = reinterpret_cast<PanePosLayout*>((*pLayout1)
                    ->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x10b], true));
                d->v2C = tmp.x;
                d->v30 = lbl_eu_80666FEC;
                d->v34 = tmp.z;
            }
            // Copy pane 0x112 the same way, y = markerY.
            {
                PanePosLayout* s = reinterpret_cast<PanePosLayout*>((*pLayout1)
                    ->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x112], true));
                tmp.x = s->v2C;
                tmp.y = markerY;
                tmp.z = s->v34;
                PanePosLayout* d = reinterpret_cast<PanePosLayout*>((*pLayout1)
                    ->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x112], true));
                d->v2C = tmp.x;
                d->v30 = lbl_eu_80667008;
                d->v34 = tmp.z;
            }
        }

        // First pass: snapshot the indicator-pane widths for the scale math.
        if (i == 0) {
            field7C8 = reinterpret_cast<PaneSizeView*>((*pLayout1)->GetRootPane()
                ->FindPaneByName(&lbl_eu_804FDBF8[0xd3], true))->width;
            field7CC = reinterpret_cast<PaneSizeView*>((*pLayout1)->GetRootPane()
                ->FindPaneByName(&lbl_eu_804FDBF8[0xdd], true))->width;
            field7D0 = reinterpret_cast<PaneSizeView*>((*pLayout1)->GetRootPane()
                ->FindPaneByName(&lbl_eu_804FDBF8[0x11d], true))->width;
        }

        panel->unk44 = (u32)(*pLayout1)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x127], true);
        panel->unk48 = (u32)(*pLayout1)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x101], true);

        // ---- layout2 arc + panes ----
        nw4r::lyt::Layout** pLayout2 = &panel->layout2;
        func_80136E84(pLayout2,
                      reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4()),
                      &lbl_eu_804FDBF8[0x12e]);

        // First pass: pull the four shared colour quads off pane 0x147 and
        // propagate the last channel into the three alternate quads.
        if (i == 0) {
            nw4r::lyt::Pane* pane = (*pLayout2)->GetRootPane()
                ->FindPaneByName(&lbl_eu_804FDBF8[0x147], true);
            FourShorts fs0 = func_801397AC(pane, 0);
            lbl_eu_80663F58[0] = fs0.a;
            lbl_eu_80663F58[1] = fs0.b;
            lbl_eu_80663F58[2] = fs0.c;
            lbl_eu_80663F58[3] = fs0.d;
            FourShorts fs1 = func_801397AC(pane, 1);
            lbl_eu_80663F60[0] = fs1.a;
            lbl_eu_80663F60[1] = fs1.b;
            lbl_eu_80663F60[2] = fs1.c;
            lbl_eu_80663F60[3] = fs1.d;
            lbl_eu_80663F68[3] = lbl_eu_80663F58[3];
            lbl_eu_80663F70[3] = lbl_eu_80663F60[3];
            lbl_eu_80663F78[3] = lbl_eu_80663F58[3];
            lbl_eu_80663F80[3] = lbl_eu_80663F60[3];
            lbl_eu_80663F88[3] = lbl_eu_80663F58[3];
            lbl_eu_80663F90[3] = lbl_eu_80663F60[3];
        }

        panel->obj1 = (*pLayout2)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x151], true);
        panel->obj2 = (*pLayout2)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x158], true);
        panel->obj3 = (*pLayout2)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x160], true);
        panel->unk38 = (u32)(*pLayout2)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x147], true);
        panel->unk3C = (u32)(*pLayout2)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x6a], true);
        panel->unk40 = (u32)(*pLayout2)->GetRootPane()
            ->FindPaneByName(&lbl_eu_804FDBF8[0x169], true);
    }

    // ---- main menu layout ----
    func_80136E84(&unk74,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4()),
                  &lbl_eu_804FDBF8[0x173]);
    func_80136F08(unk74, &unk78,
                  reinterpret_cast<nw4r::lyt::ArcResourceAccessor*>(func_801355F4()),
                  &lbl_eu_804FDBF8[0x190]);

    // Bind the font: root pane + font object slot 0x24, push back onto root.
    nw4r::lyt::Pane* rootPane = unk74->GetRootPane();
    void* fontObj = CDeviceFont::func_80452C10(1, unk74);
    func_8013676C(rootPane, reinterpret_cast<MenuFontView*>(fontObj)->m24());

    func_801368C0(unk74, &lbl_eu_804FDBF8[0x1b2], func_801355D8());
    func_801368C0(unk74, &lbl_eu_804FDBF8[0x1bb], func_801355D8());

    unk74->SetAnimationEnable(unk78, true);
    reinterpret_cast<AnimTransformOverlay*>(unk78)->field10 = lbl_eu_80666FEC;
    unk74->Animate(0);

    field7C = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x1c4], true);
    field80 = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x112], true);
    field84 = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x1cc], true);
    field88 = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x1d5], true);
    field8C = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x1de], true);
    field90 = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x1e9], true);
    field94 = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x1f4], true);
    field98 = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x201], true);
    field9C = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x158], true);
    fieldA0 = (u32)unk74->GetRootPane()->FindPaneByName(&lbl_eu_804FDBF8[0x160], true);

    // Cursor: stash the shared arc accessor + scene, then build the layouts.
    u32 scn = (u32)unk60;
    selectCursor.field18 = (u32)func_801355F4();
    selectCursor.field04 = scn;
    func_8010EB44(&selectCursor);

    field7C4 = 1;

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x5c
    // for the IScnRender subobject passed to addRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&vtPtr2);
    }
    reinterpret_cast<CScn*>(unk60)->addRenderCB(render, 8, 0);

    reinterpret_cast<UnkClass_8045F564*>(unk64)->func_8045F810();
}

// ---------------------------------------------------------------------------
// CMenuEnemyState::Term (us-8011074c): wait for VI draw completion, detach the
// IScnRender callback at +0x5C, delete all panel/layout objects (double
// null-guard is the retail MWCC idiom for delete-then-null), release the two
// scratch regions and clear the singleton.
// ---------------------------------------------------------------------------
void CMenuEnemyState::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4 / beq / addi +0x5c
    // for the IScnRender subobject passed to removeRenderCB.
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        render = reinterpret_cast<IScnRender*>(&vtPtr2);
    }
    reinterpret_cast<CScn*>(unk60)->removeRenderCB(render);

    for (u8 i = 0; i < 24; i++) {
        MenuEnemyPanel& p = panels[i];
        // `delete` emits its own null-check, so the single outer if gives the
        // retail two `beq`s (skip-all + skip-delete) around the vtable dtor.
        if (p.layout1 != 0) {
            delete p.layout1;
            p.layout1 = 0;
        }
        if (p.layout2 != 0) {
            delete p.layout2;
            p.layout2 = 0;
        }
    }

    if (unk74 != 0) {
        delete unk74;
        unk74 = 0;
    }
    if (selectCursor.layout1C != 0) {
        delete selectCursor.layout1C;
        selectCursor.layout1C = 0;
    }

    reinterpret_cast<UnkClass_8045F564*>(&selectCursor.field08)->func_8045F778();
    reinterpret_cast<UnkClass_8045F564*>(unk64)->func_8045F778();
    lbl_eu_80663F50 = 0;
}
