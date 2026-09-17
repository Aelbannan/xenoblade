// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CArtsInfo.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/cf/CItem.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include <nw4r/lyt.h>
#include <stdlib.h>

// Extern declarations for referenced symbols

// String table base (rodata, accessed via sda21 relocation)
extern char lbl_eu_8050B00C[];

// Vtable symbol for CArtsInfo (array form: name decays to its address so the
// ctor stores it directly like retail)
extern void* lbl_eu_80536A88[];

// Jump table for state machine dispatch (func_8023587C)
extern void* jumptable_eu_805369A0[];

// Float constants in small data area
extern float lbl_eu_80668680; // 0.0f
extern const float lbl_eu_80668684; // 1.0f (const: retail hoists the pool load)

// Small data objects (.sdata, addressed via @sda21): declared as complete
// 4-byte objects so MWCC emits EMB_SDA21 relocs like retail (an incomplete
// char[] extern would force lis/addi absolute addressing).
extern u32 lbl_eu_80664748;
extern u32 lbl_eu_80664750;
extern u32 lbl_eu_80664758;
extern u32 lbl_eu_80664760;

// Small data object (.sbss, addressed via @sda21): arts bdat file pointer.
// void* (not u32): shared with kyoshin/cf/CItem.hpp (MWCC 10197).
extern void* lbl_eu_806640F4;

// Shared sdata2 int->float correction constants (2^52-family magics) are
// declared in CArtsInfo.hpp; referenced by name so the conversion lfd's emit
// retail SDA21 relocs instead of TU-local pool labels.

void drawLayout(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Forward declarations for state machine functions (defined later in this TU).
// CArtsInfo_RefreshLayout/CArtsInfo_UpdateCursor are extern "C": retail calls the C names (not
// __FP9CArtsInfo).
extern "C" __declspec(noinline) void CArtsInfo_BindAllPanes(CArtsInfo*);
extern "C" void CArtsInfo_RefreshLayout(CArtsInfo*);
extern "C" void CArtsInfo_UpdateCursor(CArtsInfo*);
extern "C" void CArtsInfo_UpdateArtsRow(CArtsInfo*, u8, u8);
// Damage/level helpers: extern "C" so the sprintf-family call sites
// (func_80238298 / func_80238038) emit bl to the unmangled retail names.
extern "C" int CArtsInfo_CalcDmg52(CArtsInfo*);
extern "C" int CArtsInfo_CalcDmg53(CArtsInfo*);
// Stat-id helper: noinline + C linkage so func_80238038 emits a bl to the
// retail (unmangled) symbol instead of an inlined vtable dispatch. The
// definition (later in this TU) inherits the linkage; its body is unchanged.
extern "C" __declspec(noinline) int CArtsInfo_GetStatId1C(CArtsInfo*);

// Forward declarations for animation state handlers. noinline: the dispatcher
// func_8023587C must emit bl's to them through the retail jump table;
// inlining would erase the dispatch entirely. extern "C": retail reloc names
// are the unmangled func_* symbols, not func_*__FP9CArtsInfo.
extern "C" __declspec(noinline) void CArtsInfo_AnimState0(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState1(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState2(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState3(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState4(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState5(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState6(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState7(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState8(CArtsInfo*);
extern "C" __declspec(noinline) void CArtsInfo_AnimState9(CArtsInfo*);

// Manual signed-int -> double conversion (docs/MWCC_PATTERNS.md 7i): build
// the 0x4330000080000000 bit pattern and subtract the shared sdata2 magic so
// the lfd references lbl_eu_80668698 instead of a TU-local pool label.
static double ConvS32ToF64(s32 x) {
    union {
        double d;
        u32 w[2];
    } u;
    // xoris word first, then 0x43300000, or MWCC hoists the lis out of order.
    u.w[1] = (u32)x ^ 0x80000000;
    u.w[0] = 0x43300000;
    return u.d - lbl_eu_80668698;
}

// Definition (value 0x4330000080000000 = 2^52 + 2^31). NOTE: MWCC does NOT
// pool plain-cast s32->float literals onto this named symbol (still emits
// TU-private @N labels) - matching retail's sda21 refs here needs the
// CArtsInfo.o pool_patterns postprocess rule (see func_80239FC4 note).
const double lbl_eu_80668698 = 4503601774854144.0;


// (Layout/Pane virtual dispatches go through the real SDK classes:
// `delete` for the +0x08 deleting-dtor, GetResource for +0x0C.)

u8 CArtsInfo::getField48() { return field_0x48; }

u8 CArtsInfo::getField49() { return field_0x49; }

// __ct__CArtsInfo - constructor
// .text:0x0, size 0xAC
// __ct__CArtsInfo - constructor. Retail is a C-linkage global taking `this`
// (MWCC leaves __-prefixed globals unmangled); written free-function so the
// UnkClass_8045F564 member is not auto-constructed twice.
CArtsInfo* __ct__CArtsInfo(CArtsInfo* self) {
    // Set vtable pointer
    *(void**)self = lbl_eu_80536A88;

    // Construct embedded UnkClass_8045F564 member
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion);

    // Initialize all fields to zero/default
    self->field_0x14 = 0;
    self->field_0x18 = 0;
    self->field_0x1C = 0;
    self->mpLayout1 = nullptr;
    self->mpAnimTrans1 = nullptr;
    self->mpAnimTrans2 = nullptr;
    self->mpAnimTrans3 = nullptr;
    self->mpAnimTrans4 = nullptr;
    self->mpLayout2 = nullptr;
    self->mpAnimTrans5 = nullptr;
    self->mpAnimTrans6 = nullptr;
    self->field_0x40 = 0;
    self->field_0x44 = 0;
    self->field_0x48 = 0;
    self->field_0x49 = 1;
    self->field_0x4C = 0;
    self->field_0x50 = 0;
    self->field_0x54 = 0;
    self->field_0x55 = 0;
    self->field_0x56 = 0;
    self->field_0x58 = 0;
    self->field_0x5A = 0;

    // Construct embedded CCur18 cursor
    __ct__CCur18(self->mCursor, 0);
    return self;
}

// __dt__9CArtsInfoFv - destructor
// .text:0xAC, size 0x60
// Retail dtor is a plain function: destroys the +0x5C CCur18 cursor and the
// +0x04 UnkClass_8045F564 member, then frees self when the delete flag is
// positive. Written in the free-function form (CTitle pattern) because the
// member-dtor shape would emit a vtable store for the virtual dtor that
// retail does not have. optimize_for_size merges the r30/r31 saves into the
// retail stmw r30 prologue.
#pragma optimize_for_size on
extern "C" void* __dt__9CArtsInfoFv(void* self, int flags) {
    if (self != 0) {
        __dt__6CCur18Fv((u8*)self + 0x5C, -1);
        __dt__17UnkClass_8045F564Fv((u8*)self + 0x04, -1);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
#pragma optimize_for_size off

// CArtsInfo_LoadFiles - file loading
// .text:0x10C, size 0x68
#pragma optimize_for_size on
void CArtsInfo_LoadFiles(CArtsInfo* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (int)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, (const char*)lbl_eu_8050B00C, (void*)self, 0, 0);

    self->field_0x18 =
        (int)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
            (u32)KyoshinHeap_GetField44(), (const char*)(lbl_eu_8050B00C + 0x15),
            (void*)self, 0, 0);
}
#pragma optimize_for_size off

// func_8023587C - state machine dispatch
// .text:0x174, size 0xDC
void func_8023587C(CArtsInfo* self) {
    if (self->field_0x40 == 0) return;

    // Retail guards the dispatch with a single unsigned cmplwi against 0xC
    // (emitted by the switch itself; no outer if).
    u32 state = self->field_0x44;
    switch (state) {
    case 0: CArtsInfo_AnimState0(self); break;
    case 1: CArtsInfo_AnimState1(self); break;
    case 2: CArtsInfo_AnimState2(self); break;
    case 3: CArtsInfo_AnimState3(self); break;
    case 4: CArtsInfo_AnimState4(self); break;
    case 5: CArtsInfo_AnimState5(self); break;
    case 6: CArtsInfo_AnimState6(self); break;
    case 7: CArtsInfo_AnimState7(self); break;
    case 8: CArtsInfo_AnimState8(self); break;
    case 9: CArtsInfo_AnimState9(self); break;
    // Cases 0xA-0xC share the post-dispatch with default and with the
    // cmpli-bgt tail, so MWCC folds the table bound to 9. `return` would
    // keep bound 12 but skip Animate (wrong table targets). Left as
    // explicit empty cases for the 13-slot range; bound residual stands.
    case 0xA:
    case 0xB:
    case 0xC:
        break;
    default:
        break;
    }

    // Post-dispatch: animate both layouts and update cursor
    self->mpLayout1->Animate(0);
    self->mpLayout2->Animate(0);
    func_801D202C(self->mCursor);
}

// CArtsInfo_Draw - draw
// .text:0x250, size 0x74
// optimize_for_size gives the retail stmw r30 prologue for the 2 saved regs.
#pragma optimize_for_size on
void CArtsInfo_Draw(CArtsInfo* self, void* drawInfo) {
    if (self->field_0x40 == 0) return;

    drawLayout(self->mpLayout1, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    drawLayout(self->mpLayout2, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);

    s8 cursorActive = self->field_0x5A;
    if (cursorActive >= 0) {
        func_801D20B0(self->mCursor, drawInfo);
    }
}
#pragma optimize_for_size off

// CArtsInfo_Cleanup - cleanup
// .text:0x2C4, size 0xC4
__declspec(noinline) void CArtsInfo_Cleanup(CArtsInfo* self) {
    getEntry__5CBdatFUl(2);

    closeFileHandle__FPP11CFileHandle(&self->field_0x14);
    closeFileHandle__FPP11CFileHandle(&self->field_0x18);

    self->field_0x40 = 0;

    // Doubled-beq shape: outer guard, inner guard around the deleting-dtor
    // virtual (+0x08, flag=1), unconditional-clear inside the outer guard.
    // Single guard: `delete` emits its own null check, so one source-level
    // if plus the delete check reproduces retail's doubled beq.
    if (self->mpLayout1 != nullptr) {
        delete self->mpLayout1;
        self->mpLayout1 = nullptr;
    }

    if (self->mpLayout2 != nullptr) {
        delete self->mpLayout2;
        self->mpLayout2 = nullptr;
    }

    // Cleanup arc resource accessor (field holds the pointer by value)
    releaseArcResourceAccessor__FPQ34nw4r3lyt19ArcResourceAccessor((void*)self->field_0x1C);

    // Call virtual cleanup on cursor (+0x0C)
    ((CBaseCur*)self->mCursor)->cleanup();

    // Cleanup memory region
    deleteRegion__17UnkClass_8045F564Fv(&self->mMemRegion);
}

// CArtsInfo_GetFlag48 - get field_0x48
// .text:0x388, size 0x8
u8 CArtsInfo_GetFlag48(CArtsInfo* self) { return self->field_0x48; }

// CArtsInfo_GetFlag49 - get field_0x49
// .text:0x390, size 0x8
u8 CArtsInfo_GetFlag49(CArtsInfo* self) { return self->field_0x49; }

// CArtsInfo_ReqState1 - state check (state==0 -> 1)
// .text:0x398, size 0x20
void CArtsInfo_ReqState1(CArtsInfo* self) {
    if (self->field_0x44 != 0) return;
    self->field_0x44 = 1;
    self->field_0x49 = 0;
}

// CArtsInfo_ReqState4 - state check (state==3 -> 4)
// .text:0x3B8, size 0x20
void CArtsInfo_ReqState4(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;
    self->field_0x44 = 4;
    self->field_0x49 = 0;
}

// CArtsInfo_OpenDetail6 - large state machine (state==3 -> 6, layout animation setup)
// .text:0x3D8, size 0x244
// optimize_for_size gives the retail _savegpr_28/_restgpr_28 prologue; real
// member calls give the retail r12 vtable dispatches (vtable+0x2C =
// SetAnimationEnable, vtable+0x38 = Animate).
#pragma optimize_for_size on
void CArtsInfo_OpenDetail6(CArtsInfo* self) {
    if (self->field_0x44 != 3) return;

    self->field_0x44 = 6;
    self->field_0x49 = 0;

    // Bind animations to layout 1
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, true);

    // Reset anim transform frame (retail stores 0.0f straight to +0x10)
    *(float*)((u8*)self->mpAnimTrans4 + 0x10) = lbl_eu_80668680;

    self->mpLayout1->Animate(0);

    // Bind animations to layout 1 again
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);

    // Reset anim transform frame
    *(float*)((u8*)self->mpAnimTrans3 + 0x10) = lbl_eu_80668680;

    self->mpLayout1->Animate(0);

    // Bind animations to layout 2
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, true);

    // Reset anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    self->mpLayout2->Animate(0);

    // Reset cursor state
    self->field_0x5A = 0;

    // Update layout elements with arts data: fetch the character data, then
    // read the display value through the stats sub-object's vtable+0x200
    // virtual (user virtual #126).
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    char* base = lbl_eu_8050B00C;
    char* str1 = BdatTouchStringCell(base + 0x32, base + 0x3D, 0x18);
    int dispVal = (int)((cf::CActorParam*)&obj->stats)->CActorParam_getSecondCurrency();

    char buf[32];
    sprintf(buf, base + 0x42, dispVal, str1);

    func_80136A1C(self->mpLayout1, base + 0x47, buf, 0);

    playUISound__FUl(0x6D);
}
#pragma optimize_for_size off

// CArtsInfo_ToStateA - state machine (state==9 -> 0xA)
// .text:0x61C, size 0xB4
void CArtsInfo_ToStateA(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xA;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Real member calls so MWCC emits the retail r12 vtable dispatch
    // (vtable[0x2C] = SetAnimationEnable, vtable[0x38] = Animate).
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, true);

    // Set anim transform frame
    *(float*)((u8*)self->mpAnimTrans5 + 0x10) = lbl_eu_80668680;

    self->mpLayout2->Animate(0);

    playUISound__FUl(6);
}

// CArtsInfo_ToStateC - state machine (state==9 -> 0xC)
// .text:0x6D0, size 0xAC
void CArtsInfo_ToStateC(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xC;
    self->field_0x49 = 0;

    // Set cursor visibility
    func_801D216C(self->mCursor, 0);

    // Bind animations to layout 2
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, true);

    // Set anim transform frame
    self->mpAnimTrans6->SetFrame(lbl_eu_80668680);

    self->mpLayout2->Animate(0);
}

// CArtsInfo_SetCharId - set field_0x54
// .text:0x77C, size 0x8
void CArtsInfo_SetCharId(CArtsInfo* self, u8 val) { self->field_0x54 = val; }

// CArtsInfo_SetField55 - set field_0x55
// .text:0x784, size 0x8
void CArtsInfo_SetField55(CArtsInfo* self, u8 val) { self->field_0x55 = val; }

// CArtsInfo_SetField56 - set field_0x56
// .text:0x78C, size 0x8
void CArtsInfo_SetField56(CArtsInfo* self, u8 val) { self->field_0x56 = val; }

// CArtsInfo_SetLevel58 - set field_0x58
// .text:0x794, size 0x8
void CArtsInfo_SetLevel58(CArtsInfo* self, u16 val) { self->field_0x58 = val; }

// CArtsInfo_RefreshIfReady - call CArtsInfo_BindAllPanes, check fields, call CArtsInfo_RefreshLayout
// .text:0x79C, size 0x4C
void CArtsInfo_RefreshIfReady(CArtsInfo* self) {
    CArtsInfo_BindAllPanes(self);

    if (self->field_0x54 != 0 && self->field_0x55 != 0) {
        CArtsInfo_RefreshLayout(self);
    }
}

// CArtsInfo_CursorPrev - decrement field_0x5A, tail-call CArtsInfo_UpdateCursor
// .text:0x7E8, size 0x24
void CArtsInfo_CursorPrev(CArtsInfo* self) {
    // Retail keeps the byte arithmetic unsigned: lbz (no extsb), subi, stb,
    // then masks/sign-extends only for the clamp test.
    u8 val = (u8)self->field_0x5A - 1;
    self->field_0x5A = val;
    if ((s8)val < 0) self->field_0x5A = 1;
    CArtsInfo_UpdateCursor(self);
}

// CArtsInfo_CursorNext - increment field_0x5A, tail-call CArtsInfo_UpdateCursor
// .text:0x80C, size 0x28
void CArtsInfo_CursorNext(CArtsInfo* self) {
    u8 val = (u8)self->field_0x5A + 1;
    self->field_0x5A = val;
    if ((s8)val > 1) self->field_0x5A = 0;
    CArtsInfo_UpdateCursor(self);
}

// CArtsInfo_IsCursorZero - check if field_0x5A is zero
// .text:0x834, size 0x14
u32 CArtsInfo_IsCursorZero(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    return (val == 0) ? 1 : 0;
}

// CArtsInfo_IsStateGE6 - signed (field_0x44 >= 6) via subfc/adde idiom
// .text:0x848, size 0x1C
u32 CArtsInfo_IsStateGE6(CArtsInfo* self) {
    int state = self->field_0x44;
    return (state >= 6) ? 1 : 0;
}

// CArtsInfo_AnimState0 - animation state 0
// .text:0x864, size 0xB4
void CArtsInfo_AnimState0(CArtsInfo* self) {
    float f = lbl_eu_80668684;
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans1, f) == 0) return;

    // Real member calls so MWCC emits the retail r12 vtable dispatch.
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, true);

    self->field_0x44 = 2;
}

// CArtsInfo_AnimState1 - animation state 1
// .text:0x918, size 0x4C
void CArtsInfo_AnimState1(CArtsInfo* self) {
    float f = lbl_eu_80668684;
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans2, f)) {
        self->field_0x44 = 3;
        self->field_0x49 = 1;
    }
}

// CArtsInfo_AnimState2 - animation state 2
// .text:0x964, size 0xB4
void CArtsInfo_AnimState2(CArtsInfo* self) {
    if (AnimRewindFrame(self->mpAnimTrans2, lbl_eu_80668684) == 0) return;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, true);

    self->field_0x44 = 5;
}

// CArtsInfo_AnimState3 - animation state 3
// .text:0xA18, size 0x4C
void CArtsInfo_AnimState3(CArtsInfo* self) {
    if (AnimRewindFrame(self->mpAnimTrans1, lbl_eu_80668684) == 0) return;

    self->field_0x49 = 1;
    self->field_0x44 = 0;
}

// CArtsInfo_AnimState4 - animation state 4
// .text:0xA64, size 0xB4
void CArtsInfo_AnimState4(CArtsInfo* self) {
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 7;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, true);
}

// CArtsInfo_AnimState5 - animation state 5
// .text:0xB18, size 0xB4
void CArtsInfo_AnimState5(CArtsInfo* self) {
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans4, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 8;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, true);
}

// CArtsInfo_AnimState6 - animation state 6
// .text:0xBCC, size 0x60
void CArtsInfo_AnimState6(CArtsInfo* self) {
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans5, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 9;
    self->field_0x49 = 1;
    func_801D216C(self->mCursor, 1);
    CArtsInfo_UpdateCursor(self);
}

// CArtsInfo_AnimState7 - animation state 7
// .text:0xC2C, size 0xD4
void CArtsInfo_AnimState7(CArtsInfo* self) {
    u32 done1 = AnimRewindFrame(self->mpAnimTrans4, lbl_eu_80668684);
    u32 done2 = AnimRewindFrame(self->mpAnimTrans5, lbl_eu_80668684);

    if (done1 == 0) return;
    if (done2 == 0) return;

    self->field_0x44 = 0xB;

    // Real member calls so MWCC emits the retail r12 vtable dispatch
    // (vtable[0x2C] = Layout::SetAnimationEnable).
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);
}

// CArtsInfo_AnimState8 - animation state 8
// .text:0xD00, size 0x4C
void CArtsInfo_AnimState8(CArtsInfo* self) {
    if (AnimRewindFrame(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 3;
    self->field_0x49 = 1;
}

// CArtsInfo_AnimState9 - animation state 9
// .text:0xD4C, size 0xB4
void CArtsInfo_AnimState9(CArtsInfo* self) {
    if (advanceAnimTransform__FPQ34nw4r3lyt13AnimTransformf(self->mpAnimTrans6, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 0xB;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);
}

// CArtsInfo_BindAllPanes - large layout setup (bind all pane animations)
// noinline + extern "C": CArtsInfo_RefreshIfReady (and OnFileEvent) tail-call the bare
// retail symbol; inlining would balloon their bodies. optimize_for_size
// merges the r30/r31 saves into the retail stmw r30 prologue (plain -O4,p
// emits two separate stw's, +8 bytes).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void CArtsInfo_BindAllPanes(CArtsInfo* self) {
    // Bind animations for all named panes on layout 1
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x50, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x59, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x64, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x72, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x80, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x8B, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x96, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xA1, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xAC, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xB7, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xC2, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xCD, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xD8, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xE3, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xEE, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0xFD, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x10C, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x11B, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x12A, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x139, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x148, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x157, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x166, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x175, lbl_eu_8050B00C + 0x58, 0);

    // Bind animations for all named panes on layout 2
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x50, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x59, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x64, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x72, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x80, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x8B, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x96, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xA1, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xAC, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xB7, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xC2, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xCD, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xD8, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xE3, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xEE, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0xFD, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x10C, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x11B, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x12A, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x139, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x148, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x157, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x166, lbl_eu_8050B00C + 0x58, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x175, lbl_eu_8050B00C + 0x58, 0);

    // Set text rendering parameters for specific panes on layout 2
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0xEE, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0xFD, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x10C, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x11B, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x12A, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x139, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x148, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x157, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x166, &lbl_eu_80664748, &lbl_eu_80664750);
    PaneMatSetTevColorsByName(self->mpLayout2, lbl_eu_8050B00C + 0x175, &lbl_eu_80664748, &lbl_eu_80664750);
}
#pragma optimize_for_size off

// CArtsInfo_RefreshLayout - arts info full layout refresh. Formats the arts name/lv
// strings and the colour/flag rows, then binds the row strings to the named
// panes on both layouts. The trailing loop walks arts rows 1..10 (slot 0x1b2
// names) and grows the arts list (CArtsInfo_UpdateArtsRow) for each row that has a
// non-empty entry.
// noinline: CArtsInfo_RefreshIfReady (FULL_MATCH) tail-calls the retail symbol; inlining
// would balloon its body. optimize_for_size gives the retail stmw r25
// prologue for the 7 saved GPRs.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void CArtsInfo_RefreshLayout(CArtsInfo* self) {
    char buf[32]; // sprintf at +0x8
    char* str1 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x18);
    sprintf(buf, lbl_eu_8050B00C + 0x42, self->field_0x58, str1);
    func_80136A1C(self->mpLayout1, lbl_eu_8050B00C + 0x184, buf, 0);

    char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x3d, self->field_0x55);
    char* str3 = 0;
    if (self->field_0x56 != 0) {
        str3 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, self->field_0x56 + 0x1e);
    }
    sprintf(buf, lbl_eu_8050B00C + 0x193, str2, str3);
    func_80136A1C(self->mpLayout1, lbl_eu_8050B00C + 0x50, buf, 0);

    char* str4 = 0;
    if (self->field_0x56 < 0xA) {
        if (self->field_0x56 != 0) {
            str4 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, self->field_0x56 + 0x1f);
        }
        sprintf(buf, lbl_eu_8050B00C + 0x193, str2, str4);
        func_80136A1C(self->mpLayout2, lbl_eu_8050B00C + 0x50, buf, 0);
    }

    char* s = BdatGetPtrDirect((const void*)self->field_0x4C, lbl_eu_8050B00C + 0x198, self->field_0x55);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x59, s, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x59, s, 0);
    }

    u8 f1 = BdatGetU8Direct(self->field_0x4C, lbl_eu_8050B00C + 0x19c, self->field_0x55);
    u8 f2 = BdatGetU8Direct(self->field_0x4C, lbl_eu_8050B00C + 0x1a1, self->field_0x55);
    u8 f3 = BdatGetU8Direct(self->field_0x4C, lbl_eu_8050B00C + 0x1a6, self->field_0x55);
    char* p1 = 0;
    char* p2 = 0;
    char* p3 = 0;
    if (f1 != 0) p1 = BdatGetPtrDirect((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, f1);
    if (f2 != 0) p2 = BdatGetPtrDirect((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, f2);
    if (f3 != 0) p3 = BdatGetPtrDirect((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, f3);
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x64, p1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x64, p1, 0);
    }
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x80, p2, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x80, p2, 0);
    }
    LayoutSetTextBoxFmtValue(self->mpLayout1, lbl_eu_8050B00C + 0x8b, p3, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, lbl_eu_8050B00C + 0x8b, p3, 0);
    }

    // Loop counters: full-width ints narrowed to u8 at the call sites
    // (retail clrlwi's the sprintf vararg and the CArtsInfo_UpdateArtsRow arg, but
    // tests the loop bound with a bare cmplwi - a u8 counter would force an
    // extra rlwinm mask before the bound test). n declared first: saved-reg
    // allocation gives it the lower register (retail n->r25, i->r26).
    unsigned int n = 2;
    unsigned int i = 1;
    do {
        sprintf(buf, lbl_eu_8050B00C + 0x1b2, (u8)i);
        u8 f = BdatGetU8Direct(self->field_0x4C, buf, self->field_0x55);
        if (f != 0) {
            CArtsInfo_UpdateArtsRow(self, f, (u8)n++);
        }
        i++;
    } while (i <= 0xA);
}
#pragma optimize_for_size off

// CArtsInfo_UpdateCursor - cursor name display. Formats the current cursor slot name
// (field_0x5A + 1) and binds it to the pane found by name on layout 1, then
// sends the string to the cursor's message virtual. Early-exits when the
// cursor slot is negative (cursor hidden).
// noinline: the state handlers (CArtsInfo_CursorPrev/35F14/362D4) tail-call the
// retail symbol; inlining would balloon their bodies.
// optimize_for_size merges the r29/r30/r31 saves into the retail stmw r29
// prologue.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void CArtsInfo_UpdateCursor(CArtsInfo* self) {
    char buf[0x20]; // sprintf at +0x18
    nw4r::math::VEC3 pos; // func_80137924 output at +0x8
    s8 idx = self->field_0x5A;
    if (idx < 0) return;

    sprintf(buf, lbl_eu_8050B00C + 0x1b8, idx + 1);
    nw4r::lyt::Pane* pane1 = self->mpLayout1->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = self->mpLayout1->GetRootPane()->FindPaneByName(lbl_eu_8050B00C + 0x1c5, true);
    func_80137924(&pos, pane1, pane2, self->mpLayout1->GetRootPane());
    reinterpret_cast<CBaseCur*>(&self->mCursor[0])->setRootPaneTranslate(&pos);
}
#pragma optimize_for_size off

// CArtsInfo_GetStatId1C - fetch stat block via vtable[0x224] dispatch, return s16 id.
// Declared int (not s16): the callee already sign-extends the s16 slot value
// in r3, so callers (func_80238038/802384F4) add it directly without an
// explicit extsh (retail codegen).
int CArtsInfo_GetStatId1C(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    CArtsStatBlock* st = (CArtsStatBlock*)((cf::CActorParam*)&obj->stats)->CActorParam_getBattleParams();
    return st->field_0x1C;
}

// CArtsInfo_GetStatId20 - same dispatch, return the id at +0x20. Retail treats the
// result as a full-width int (callers use it without re-sign-extending:
// func_8023A2D8 shifts it directly), so the return type is int.
// .text:0x16E8, size 0x38
// noinline + extern "C": the sprintf-family callers (func_8023A2D8) must emit
// a bl to the retail (unmangled) symbol, not an inlined vtable dispatch.
extern "C" __declspec(noinline) int CArtsInfo_GetStatId20(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    CArtsStatBlock* st = (CArtsStatBlock*)((cf::CActorParam*)&obj->stats)->CActorParam_getBattleParams();
    return st->field_0x20;
}

// CArtsInfo_GetStatF10 - same dispatch, return (int)float at +0x10
// .text:0x1720, size 0x44
// noinline + extern "C": the arts-level callers (func_8023939C) must emit a
// bl to the retail (unmangled) symbol, not an inlined vtable dispatch.
extern "C" __declspec(noinline) int CArtsInfo_GetStatF10(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    CArtsStatBlock* st = (CArtsStatBlock*)((cf::CActorParam*)&obj->stats)->CActorParam_getBattleParams();
    return (int)st->field_0x10;
}

// func_80236E6C - arts equipment/level lookup. Reads the character's six
// s16 slots (weapon + five stat ids at +0x26/+0x1C..+0x24), and for each
// slot type (a byte of the 0x80668688/0x8066868C flags) fetches the item
// list entry; if the entry's slots contain the requested item id, the
// matching slot's value accumulates into the result. Finally the result is
// capped at the row value from the 0x1d4 table (signed compare).
// extern "C" + noinline: retail callers emit bl to the unmangled symbol.
extern "C" __declspec(noinline) int func_80236E6C(CArtsInfo* self, int arg2) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    s16 ids[6];
    CArtsSlotFlags flags;
    ids[0] = obj->field_0x26;
    ids[1] = obj->field_0x1C;
    ids[2] = obj->field_0x1E;
    ids[3] = obj->field_0x20;
    ids[4] = obj->field_0x22;
    ids[5] = obj->field_0x24;
    flags.w.field_0x0 = lbl_eu_80668688;
    flags.w.field_0x4 = lbl_eu_8066868C;
    int result = 0;

    for (u8 i = 0; i < 6; i++) {
        if (ids[i] == -1) continue;
        CItemExt* e = func_80157C4C(flags.b[i], ids[i]);
        if (e == 0) continue;
        if (e->field_00 == 0) continue;
        // Retail re-fetches the impl object at every dispatch (5 bl's); a
        // hoisted local would drop two of them.
        u8 count = CItem_initItemImplInstances((CItemData*)e)->vf30((CItemData*)e);
        for (u8 j = 0; j < count; j++) {
            s16 id = CItem_initItemImplInstances((CItemData*)e)->vf40((CItemData*)e, j);
            if (id != -1) {
                CItemExt* e3 = func_80157C4C(3, id);
                if (e3 != 0 && e3->field_00 != 0) {
                    if ((u32)arg2 == CItem_initItemImplInstances((CItemData*)e3)->vf54((CItemData*)e3)) {
                        result += CItem_initItemImplInstances((CItemData*)e3)->vf90((CItemData*)e3);
                    }
                }
            } else {
                CArtsItemInfo* p = (CArtsItemInfo*)CItem_initItemImplInstances((CItemData*)e)->vf2C((CItemData*)e, j);
                if (p != 0) {
                    if ((u32)arg2 == ((p->field_0x4 >> 4) & 0xfff)) {
                        result += (s16)((p->field_0x0 >> 10) & 0x7ff);
                    }
                }
            }
        }
    }
    u16 t = (u16)BdatGetU16Direct((const void*)lbl_eu_806640D8, lbl_eu_8050B00C + 0x1d4, arg2);
    if (t < result) result = t;
    return result;
}

// CArtsInfo_GetSkill5C - arts skill lookup (flag 0x5C)
// .text:0x18E0, size 0x58
// Walks the character-data +0x3534 object: func_8026178C tests whether the
// skill is reachable, func_8025FB10 returns its summed value (see
// code_8025FB10.cpp).
// extern "C" + noinline: retail callers (func_80237A0C) emit a bl to the
// unmangled out-of-line symbol; without both, MWCC mangles the call-site
// reloc and -inline auto expands the body inline. int return: the caller's
// (float) conversion uses the signed xoris magic (retail codegen).
extern "C" __declspec(noinline) int CArtsInfo_GetSkill5C(CArtsInfo* self) {
    u8* obj = (u8*)func_8009EC9C(self->field_0x54) + 0x3534;
    if (func_8026178C(obj, 0x5C) != 0) {
        return func_8025FB10(obj, 0x5C);
    }
    return 0;
}

// CArtsInfo_GetSkill2D - arts skill lookup (flag 0x2D)
// .text:0x1938, size 0x58
// extern "C": the retail symbol is unmangled, and CArtsInfo_CalcDmg52 must emit a
// bl to it (noinline + C linkage keeps the call-site reloc name retail-identical).
extern "C" __declspec(noinline) u32 CArtsInfo_GetSkill2D(CArtsInfo* self) {
    u8* obj = (u8*)func_8009EC9C(self->field_0x54) + 0x3534;
    if (func_8026178C(obj, 0x2D) != 0) {
        return func_8025FB10(obj, 0x2D);
    }
    return 0;
}

// CArtsInfo_GetGridA - arts list lookup: fetch two table entries keyed by arg3
// (or field_0x55 when arg3 is 0) and combine them into a grid offset:
// u16 first-entry + u8 second-entry * (arg2 - 1). The ternaries stay int so
// the u8 mask is emitted at the call sites (retail: mr r0,r30 / clrlwi).
#pragma optimize_for_size on
// extern "C" + noinline: retail callers (func_802397F4) emit a bl to the
// unmangled out-of-line symbol; without both, MWCC mangles the call-site
// reloc and -inline auto expands the body inline. u8 params: retail call
// sites mask the level+1 argument (clrlwi 24) before the call.
extern "C" __declspec(noinline) int CArtsInfo_GetGridA(CArtsInfo* self, u8 arg2, u8 arg3) {
    int v1 = (arg3 != 0) ? arg3 : self->field_0x55;
    u16 a = BdatGetU16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1d8, v1);
    int v2 = (arg3 != 0) ? arg3 : self->field_0x55;
    u8 b = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1de, v2);
    return a + b * (arg2 - 1);
}
#pragma optimize_for_size off

// CArtsInfo_GetGridB - arts list lookup: same shape as CArtsInfo_GetGridA but with
// different string-table offsets (0x1e8/0x1ee).
#pragma optimize_for_size on
// extern "C" + noinline: see CArtsInfo_GetGridA (u8 params).
extern "C" __declspec(noinline) int CArtsInfo_GetGridB(CArtsInfo* self, u8 arg2, u8 arg3) {
    int v1 = (arg3 != 0) ? arg3 : self->field_0x55;
    u16 a = BdatGetU16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1e8, v1);
    int v2 = (arg3 != 0) ? arg3 : self->field_0x55;
    u8 b = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x1ee, v2);
    return a + b * (arg2 - 1);
}
#pragma optimize_for_size off

// CArtsInfo_CalcDmg52 - arts info damage/level helper. Reads the character's weapon
// id (+0x26 of the func_8009EC9C data), finds the entry whose first word's
// top 12 bits match it (func_80157C4C/BdatGetItemId), then looks up the
// weapon's name row (BdatGetU16Direct) and a flag byte (BdatGetU8Direct). When
// flag bit 4 is set, computes a capped HP value = scale1 * (nameRow * base),
// clamped to 999. The final result is
// scale2 * ((hp & 0xffff) * (skill + artLevel + 100)) masked to 16 bits.
// Both int->float conversions use MWCC's s32 magic (xoris + lbl_eu_80668698).
// Single shared fail label (return 0) at the bottom, matching retail's block
// layout (all guards jump forward to it).
int CArtsInfo_CalcDmg52(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    s16 weapon = obj->field_0x26;
    if (weapon == -1) goto fail;
    CItemExt* e = func_80157C4C(2, weapon);
    if (e == 0) goto fail;
    u32 v0 = e->field_00;
    if (v0 == 0) goto fail;
    u16 id = BdatGetItemId(v0 >> 20);
    int hp = (int)BdatGetU16Direct((const void*)lbl_eu_806640F4, lbl_eu_8050B00C + 0x1f8, id);
    u8 b = (u8)BdatGetU8Direct((u32)lbl_eu_806640F4, lbl_eu_8050B00C + 0x200, id);
    if ((b & 4) != 0) {
        // u16 local: retail masks the scale at definition (clrlwi in r3).
        u16 base = (u16)func_800A082C(obj);
        hp = (int)(lbl_eu_80668690 * (float)((hp & 0xffff) * base));
        if ((hp & 0xffffu) >= 0x3e7u) hp = 0x3e7;  // unsigned cmpli, clamp 999
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so CArtsInfo_GetSkill2D
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(lbl_eu_80668694 * (float)((hp & 0xffff) * ((int)func_80236E6C(self, 0x52) + (int)CArtsInfo_GetSkill2D(self) + 100))) & 0xffff;
fail:
    return 0;
}

// CArtsInfo_CalcDmg53
// CArtsInfo_CalcDmg52 but the name-row string offset is 0x205 and the HP
// scale factor is lbl_eu_806686A0.
int CArtsInfo_CalcDmg53(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)func_8009EC9C(self->field_0x54);
    s16 weapon = obj->field_0x26;
    if (weapon == -1) goto fail;
    CItemExt* e = func_80157C4C(2, weapon);
    if (e == 0) goto fail;
    u32 v0 = e->field_00;
    if (v0 == 0) goto fail;
    u16 id = BdatGetItemId(v0 >> 20);
    int hp = (int)BdatGetU16Direct((const void*)lbl_eu_806640F4, lbl_eu_8050B00C + 0x205, id);
    u8 b = (u8)BdatGetU8Direct((u32)lbl_eu_806640F4, lbl_eu_8050B00C + 0x200, id);
    if ((b & 4) != 0) {
        // u16 local: retail masks the scale at definition (clrlwi in r3).
        u16 base = (u16)func_800A082C(obj);
        hp = (int)(lbl_eu_806686A0 * (float)((hp & 0xffff) * base));
        if ((hp & 0xffffu) >= 0x3e7u) hp = 0x3e7;  // unsigned cmpli, clamp 999
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so CArtsInfo_GetSkill2D
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(lbl_eu_80668694 * (float)((hp & 0xffff) * ((int)func_80236E6C(self, 0x53) + (int)CArtsInfo_GetSkill2D(self) + 100))) & 0xffff;
fail:
    return 0;
}

// func_802374F0 - arts skill string lookup
// .text:0x1E00, size 0x5C
// BdatGetU8Direct finds the row for field_0x50 in the lbl_eu_8050B00C+0x20C
// list; if present, BdatTouchStringCell formats the string (3rd arg = the row's
// byte index, which retail keeps in r5 from the clrlwi mask). Result lives
// in a local so MWCC hoists `li r30, 0` before the call and returns it via
// r30 (same shape as CArtsInfo_GetSkillStr).
#pragma optimize_for_size on
extern "C" __declspec(noinline) char* func_802374F0(CArtsInfo* self, u32 arg2) {
    char* result = 0;
    u8 r = BdatGetU8Direct((u32)self->field_0x50, lbl_eu_8050B00C + 0x20C, arg2);
    if (r != 0) {
        result = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3D, r);
    }
    return result;
}
#pragma optimize_for_size off

// CArtsInfo_GetSkillStr - arts skill string lookup (same shape as func_802374F0,
// different string-table offset 0x212). result lives in a local so MWCC
// hoists `li r30, 0` before the call and returns it via r30.
#pragma optimize_for_size on
extern "C" __declspec(noinline) char* CArtsInfo_GetSkillStr(CArtsInfo* self, u32 arg2) {
    char* result = 0;
    u8 r = BdatGetU8Direct((u32)self->field_0x50, lbl_eu_8050B00C + 0x212, arg2);
    if (r != 0) {
        result = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3D, r);
    }
    return result;
}
#pragma optimize_for_size off

// CArtsInfo_UpdateArtsRow - arts list entry update. Formats the current arts-row name
// (BdatGetPtrDirect row lookup for field_0x50 at table 0x1ab) and the level
// string (arg3 + 2) into a buffer bound to the row pane on both layouts
// (layout 2 only when field_0x56 < 0xA), then dispatches on the row id
// (BdatGetU8Direct lookup at 0x225, masked to u8) through a 44-case jump table
// to the per-arts text-update handlers.
//
// Dispatch-target forward declarations: C linkage so the jump-table calls
// emit the bare retail reloc names, and noinline so -inline auto keeps the
// bl's (the definitions below inherit the extern "C" linkage).
extern "C" __declspec(noinline) void func_80237A0C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80237B88(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80237D58(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80237E24(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80238038(CArtsInfo*, u32, int, u8);
extern "C" __declspec(noinline) void func_80238298(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_802384F4(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80238904(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239030(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023916C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023939C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023959C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_802397F4(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239964(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239AA0(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239BDC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239D20(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239EFC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_80239FC4(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A148(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A210(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A2D8(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A398(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A460(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A55C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A60C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A6BC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A76C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A81C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A8CC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023A97C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AA2C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AADC(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AB8C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AD5C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AE24(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023AF60(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B074(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B12C(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B280(CArtsInfo*, u32, int);
extern "C" __declspec(noinline) void func_8023B368(CArtsInfo*, u32, int);

extern "C" __declspec(noinline) void CArtsInfo_UpdateArtsRow(CArtsInfo* self, u8 arg2, u8 arg3) {
    char buf[32]; // sprintf at +0x8
    char* s = BdatGetPtrDirect((const void*)self->field_0x50, lbl_eu_8050B00C + 0x1ab, arg2);
    sprintf(buf, lbl_eu_8050B00C + 0x218, arg3 + 2);
    LayoutSetTextBoxFmtValue(self->mpLayout1, buf, s, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, buf, s, 0);
    }
    u8 k = (u8)BdatGetU8Direct(self->field_0x50, lbl_eu_8050B00C + 0x225, arg2);
    switch (k) {
    case 0x00: func_80237A0C(self, arg2, arg3); break;
    case 0x01: func_80237B88(self, arg2, arg3); break;
    case 0x02: func_80237D58(self, arg2, arg3); break;
    case 0x03: func_80237E24(self, arg2, arg3); break;
    case 0x04: func_80238038(self, arg2, arg3, 0); break;
    case 0x05: func_80238298(self, arg2, arg3); break;
    case 0x06: func_802384F4(self, arg2, arg3); break;
    case 0x07: func_80238904(self, arg2, arg3); break;
    case 0x08: func_80239030(self, arg2, arg3); break;
    case 0x09: func_8023916C(self, arg2, arg3); break;
    case 0x0A: func_8023939C(self, arg2, arg3); break;
    case 0x0B: func_8023959C(self, arg2, arg3); break;
    case 0x0C: func_802397F4(self, arg2, arg3); break;
    case 0x0D: func_80239964(self, arg2, arg3); break;
    case 0x0E: func_80239AA0(self, arg2, arg3); break;
    case 0x0F: func_80239BDC(self, arg2, arg3); break;
    case 0x10: func_80239D20(self, arg2, arg3); break;
    case 0x11: func_80239EFC(self, arg2, arg3); break;
    case 0x12: func_80239FC4(self, arg2, arg3); break;
    case 0x13: func_8023A148(self, arg2, arg3); break;
    case 0x14: func_8023A210(self, arg2, arg3); break;
    case 0x15: func_8023A2D8(self, arg2, arg3); break;
    case 0x16: func_8023A398(self, arg2, arg3); break;
    case 0x17: func_8023A460(self, arg2, arg3); break;
    case 0x18: func_8023A55C(self, arg2, arg3); break;
    case 0x19: func_8023A60C(self, arg2, arg3); break;
    case 0x1A: func_8023A6BC(self, arg2, arg3); break;
    case 0x1B: func_8023A76C(self, arg2, arg3); break;
    case 0x1C: func_8023A81C(self, arg2, arg3); break;
    case 0x1D: func_8023A8CC(self, arg2, arg3); break;
    case 0x1E: func_8023A97C(self, arg2, arg3); break;
    case 0x1F: func_8023AA2C(self, arg2, arg3); break;
    case 0x20: func_8023AADC(self, arg2, arg3); break;
    case 0x21: func_8023AB8C(self, arg2, arg3); break;
    case 0x22: func_8023AD5C(self, arg2, arg3); break;
    case 0x23: func_8023AE24(self, arg2, arg3); break;
    case 0x24: func_8023AF60(self, arg2, arg3); break;
    case 0x25: func_8023B074(self, arg2, arg3); break;
    case 0x26: func_8023B12C(self, arg2, arg3); break;
    case 0x27: func_8023B280(self, arg2, arg3); break;
    case 0x28: func_80238038(self, arg2, arg3, (u8)(self->field_0x55 + 1)); break;
    case 0x29: func_80238038(self, arg2, arg3, (u8)(self->field_0x55 + 2)); break;
    case 0x2A: func_80238038(self, arg2, arg3, (u8)(self->field_0x55 + 3)); break;
    case 0x2B: break;
    case 0x2C: func_8023B368(self, arg2, arg3); break;
    default: break;
    }
}

// func_80237A0C - arts info text update. Formats a computed percentage
// (base value scaled by the arts-skill lookups func_80236E6C(0x2d) and
// CArtsInfo_GetSkill5C) into buf1, post-processes it (func_eu_80136F90 +
// func_eu_8023D490), then pushes it onto both layouts (layout 2 only when
// field_0x56 < 0xA). The base is a u8-keyed lookup scaled by
// lbl_eu_806686A4 when non-zero; the skill values are s32->float converted
// with MWCC's xoris magic.
void func_80237A0C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x22d, self->field_0x55);
    float d = (float)r;
    if (r != 0) {
        d = d * lbl_eu_806686A4;
    }
    // f = d - scale2 * (d * (float)x) - scale2 * (d * (float)y), each step
    // folded through fnmsubs (retail: fmuls d*(float)x; fnmsubs f30, scale2,
    // f1, f31).
    float f = d - lbl_eu_80668694 * (d * (float)func_80236E6C(self, 0x2d));
    f -= lbl_eu_80668694 * (d * (float)CArtsInfo_GetSkill5C(self));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, f, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_80237B88 - arts info text update. Reads the character-data category
// (func_800A32BC), discards a 0x24c-keyed lookup, then formats
// v - 0.5 * (level-1) * v * w (float arithmetic via MWCC's u32/s32
// conversion magics: clrlwi + 2^52 for the u8 lookups, xoris + 2^52+2^31
// for the signed (level-1)) into buf1, post-processes it, pushes it onto
// layout 1, then on level-up (< 0xA) formats the next-level value
// (level instead of level-1) into buf3 for layout 2 and runs the
// colour-pair helper when the raw lookup v differs from the next value.
void func_80237B88(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x48
    char buf2[32]; // sprintf at +0x28
    char buf3[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    func_800A32BC(func_8009EC9C(self->field_0x54));
    BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x24c, self->field_0x55);
    u8 v = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x250, self->field_0x55);
    u8 w = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x257, self->field_0x55);
    // NOTE: retail lfds the 2^52 correction constants from the shared sdata2
    // labels (lbl_eu_806686A8 unsigned / lbl_eu_80668698 signed); every source
    // form that references the named doubles rewrites the conversion schedule
    // (frame/slot allocation diverges), so the plain casts are kept and MWCC
    // pools value-identical TU-local constants - needs a CArtsInfo.o
    // pool_patterns postprocess rule (43300000_00000000 / 43300000_80000000).
    // fv/fw stay in saved FPRs across the calls (reused in the level-up
    // branch below); retail keeps them in f31/f30.
    float fv = (float)v;
    float fw = (float)w;
    float cur = fv - lbl_eu_80668694 * ((float)(self->field_0x56 - 1) * (fv * fw));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        // (int) cast: retail converts level with the s32 magic (xoris);
        // plain (float)u8 would use the shorter u32 magic.
        float nxt = fv - lbl_eu_80668694 * ((float)(int)self->field_0x56 * (fv * fw));
        sprintf(buf3, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf3);
        func_eu_8023D490(self, arg2, buf3);
        func_80136A1C(self->mpLayout2, buf2, buf3, 0);
        if (fv != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80237D58 - arts info text update. Same shape as func_8023A148 but the
// lookup is BdatGetS8ByTableKey (s8 result): the value is abs()'d and narrowed back
// to s8 (retail: extsb after the abs call) for the sprintf vararg.
// Entry param shadows: declaring arg2's shadow first asks MWCC to copy r4
// before r3 in the entry block (retail order).
// u32 const on arg2: prologue-scheduling lever (KB: const params altered
// entry-store order in matched siblings); retail copies r4 before r3.
// Residual 2-instr reg swap: entry param-copy emission order (retail births
// r4->r29 before r3->r27). Documented MWCC claim-order invariant - not
// source-controllable (see func_8023916C / func_8023AADC notes).
void func_80237D58(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s8 r = BdatGetS8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x263, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, (s8)abs(r), s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_80237E24 - arts info text update. The first grid value (u16-keyed
// lookup BdatGetU16ByTableKey at 0x26d) is converted to float and scaled by
// lbl_eu_806686A4 when non-zero, then overridden by the field_0x55
// arts-type switch (0x68/0x69/0x70 -> fixed constants); the second grid
// value (u8-keyed lookup BdatGetU8ByTableKey at 0x275) is scaled the same way.
// The displayed value is base + second*(level-1), post-processed and pushed
// onto both layouts; on level-up the level grid is recomputed for layout 2
// and the colour-pair helper runs when the two values differ.
// Entry param-save-order wall (see func_8023916C).
void func_80237E24(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u16 v1 = BdatGetU16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x26d, self->field_0x55);
    float f1 = (float)v1;
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x275, self->field_0x55);
    float f2 = (float)v2;
    if (f1 != lbl_eu_80668680) {
        f1 = f1 * lbl_eu_806686A4;
    }
    switch (self->field_0x55) {
    case 0x68:
        f1 = lbl_eu_806686B0;
        break;
    case 0x69:
        f1 = lbl_eu_806686B4;
        break;
    case 0x70:
        f1 = lbl_eu_806686B8;
        break;
    default:
        break;
    }
    if (f2 != lbl_eu_80668680) {
        f2 = f2 * lbl_eu_806686A4;
    }
    float cur = f1 + f2 * (float)(self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = f1 + f2 * (float)(int)self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf1);
        func_eu_8023D490(self, arg2, buf1);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80238038 - arts info text update. Same shape as func_80238298
// (stat + min(damage1, damage2) grids scaled by 0.5) but the grid lookups
// CArtsInfo_GetGridA/8023719C are keyed on the 4th parameter instead of 0 and
// the stat comes from CArtsInfo_GetStatId1C.
void func_80238038(CArtsInfo* self, u32 arg2, int arg3, u8 arg4) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    func_802374F0(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId1C(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, arg4);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, arg4);
    int m1 = CArtsInfo_CalcDmg52(self);
    int m2 = CArtsInfo_CalcDmg53(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = RoundHalfAway0((float)(lbl_eu_80668694 * (float)(g1 * s)));
    int t = m2 + stat;
    int v2 = RoundHalfAway0((float)(lbl_eu_80668694 * (float)(g2 * t)));
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, arg4);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, arg4);
        int v1n = RoundHalfAway0((float)(lbl_eu_80668694 * (float)(g1n * s)));
        int v2n = RoundHalfAway0((float)(lbl_eu_80668694 * (float)(g2n * t)));
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_80238298 - arts info text update. Combines the stat id
// (CArtsInfo_GetStatId20) with the min of the two damage/level helpers
// (CArtsInfo_CalcDmg52/80237394): the two grids are 0.5*stat*(m+stat) products,
// formatted into buf1 (sprintf 0x282). On level-up the level+1 grids are
// recomputed for layout 2 and the colour-pair helper always runs.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (retail-identical schedule: lis 0x4330 hoisted into the prologue, one
// stack-slot pair per conversion site).
void func_80238298(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    func_802374F0(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId20(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    int m1 = CArtsInfo_CalcDmg52(self);
    int m2 = CArtsInfo_CalcDmg53(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = RoundHalfAway0(lbl_eu_80668694 * (float)(g1 * s));
    int t = m2 + stat;
    int v2 = RoundHalfAway0(lbl_eu_80668694 * (float)(g2 * t));
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(lbl_eu_80668694 * (float)(g1n * s));
        int v2n = RoundHalfAway0(lbl_eu_80668694 * (float)(g2n * t));
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_802384F4 - arts info text update. Combines the stat id
// (CArtsInfo_GetStatId1C) with the min of the two damage/level helpers
// (CArtsInfo_CalcDmg52/80237394): the two grids are 0.5*stat*(m+stat) products
// (RoundHalfAway0), then both grids are scaled by a byte-keyed factor
// (BdatGetU8ByTableKey at 0x289) and get the 0x49-keyed arts lookup
// (func_80236E6C) contribution added: value = v + 0.5*v*func_80236E6C(0x49).
// Two BdatGetU16ByTableKey lookups (0x26d/0x290) are called with results discarded.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (same retail schedule as func_80238298: lis 0x4330 hoisted into the
// prologue, one stack-slot pair per conversion site).
void func_802384F4(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    func_802374F0(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId1C(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    int m1 = CArtsInfo_CalcDmg52(self);
    int m2 = CArtsInfo_CalcDmg53(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = RoundHalfAway0(lbl_eu_80668694 * (float)(g1 * s));
    int t = m2 + stat;
    int v2 = RoundHalfAway0(lbl_eu_80668694 * (float)(g2 * t));
    u8 v3 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    BdatGetU16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x26d, self->field_0x55);
    BdatGetU16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x290, self->field_0x55);
    v1 = RoundHalfAway0(lbl_eu_80668694 * (float)(v1 * v3));
    int a = func_80236E6C(self, 0x49);
    int value1 = v1 + RoundHalfAway0(lbl_eu_80668694 * (float)(v1 * a));
    v2 = RoundHalfAway0(lbl_eu_80668694 * (float)(v2 * v3));
    a = func_80236E6C(self, 0x49);
    int value2 = v2 + RoundHalfAway0(lbl_eu_80668694 * (float)(v2 * a));
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, value1, str, value2);
    int level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(lbl_eu_80668694 * (float)(g1n * s));
        int v2n = RoundHalfAway0(lbl_eu_80668694 * (float)(g2n * t));
        v1n = RoundHalfAway0(lbl_eu_80668694 * (float)(v1n * v3));
        a = func_80236E6C(self, 0x49);
        int value1n = v1n + RoundHalfAway0(lbl_eu_80668694 * (float)(v1n * a));
        v2n = RoundHalfAway0(lbl_eu_80668694 * (float)(v2n * v3));
        a = func_80236E6C(self, 0x49);
        int value2n = v2n + RoundHalfAway0(lbl_eu_80668694 * (float)(v2n * a));
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, value1n, str2, value2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_80238904 - arts info text update. Same stat+min(damage1,damage2)
// grid shape as func_80238298 (CArtsInfo_GetStatId20 stat, CArtsInfo_GetGridA/8023719C
// offsets), then the grids are scaled by a field_0x55 arts-type base
// (BdatGetU8ByTableKey at 0x289, overridden by the 0x68/0x69/0x70 switch to fixed
// 0x28/0x3c/0x64). A BdatGetU16ByTableKey lookup at 0x297 picks the arts lookup
// key (func_80236E6C) used to add 0.5*v*lookup to each grid: 0x66 -> 0x49,
// 0x67 -> 0x9, 0x68 -> 0x3a, 0x69 -> 0x5c. On level-up the level+1 grids
// are recomputed for layout 2 (re-dispatching the 0x297 key switch; the
// 0x289 base is kept in a register across the branch) and the colour-pair
// helper always runs.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (same retail schedule as func_802384F4: lis 0x4330 hoisted into the
// prologue, one stack-slot pair per conversion site).
// u32 const on arg2: entry param-copy lever (retail copies r4 before r3).
void func_80238904(CArtsInfo* self, u32 const arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    func_802374F0(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId20(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    int m1 = CArtsInfo_CalcDmg52(self);
    int m2 = CArtsInfo_CalcDmg53(self);
    if (m1 > m2) m1 = m2;
    // Distinct locals so MWCC keeps the clamped m1 live in its own saved reg
    // and computes s into a fresh one (retail: min@r29, s@r30).
    int s = m1 + stat;
    // t computed between the two conversions like retail (the (m2+stat) addi
    // lands between the first and second RoundHalfAway0 calls).
    int v1 = RoundHalfAway0(lbl_eu_80668694 * (float)(g1 * s));
    int t = m2 + stat;
    int v2 = RoundHalfAway0(lbl_eu_80668694 * (float)(g2 * t));
    int base = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    switch (self->field_0x55) {
    case 0x68:
        base = 0x28;
        break;
    case 0x69:
        base = 0x3c;
        break;
    case 0x70:
        base = 0x64;
        break;
    default:
        break;
    }
    v1 = RoundHalfAway0(lbl_eu_80668694 * (float)(v1 * base));
    v2 = RoundHalfAway0(lbl_eu_80668694 * (float)(v2 * base));
    u16 k = BdatGetU16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x297, self->field_0x55);
    if (k == 0x66) {
        int a = func_80236E6C(self, 0x49);
        v1 += RoundHalfAway0(lbl_eu_80668694 * (float)(v1 * a));
        a = func_80236E6C(self, 0x49);
        v2 += RoundHalfAway0(lbl_eu_80668694 * (float)(v2 * a));
    } else if (k == 0x67) {
        int a = func_80236E6C(self, 0x9);
        v1 += RoundHalfAway0(lbl_eu_80668694 * (float)(v1 * a));
        a = func_80236E6C(self, 0x9);
        v2 += RoundHalfAway0(lbl_eu_80668694 * (float)(v2 * a));
    } else if (k == 0x68) {
        int a = func_80236E6C(self, 0x3a);
        v1 += RoundHalfAway0(lbl_eu_80668694 * (float)(v1 * a));
        a = func_80236E6C(self, 0x3a);
        v2 += RoundHalfAway0(lbl_eu_80668694 * (float)(v2 * a));
    } else if (k == 0x69) {
        int a = func_80236E6C(self, 0x5c);
        v1 += RoundHalfAway0(lbl_eu_80668694 * (float)(v1 * a));
        a = func_80236E6C(self, 0x5c);
        v2 += RoundHalfAway0(lbl_eu_80668694 * (float)(v2 * a));
    }
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    int level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(lbl_eu_80668694 * (float)(g1n * s));
        int v2n = RoundHalfAway0(lbl_eu_80668694 * (float)(g2n * t));
        v1n = RoundHalfAway0(lbl_eu_80668694 * (float)(v1n * base));
        v2n = RoundHalfAway0(lbl_eu_80668694 * (float)(v2n * base));
        u16 k2 = BdatGetU16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x297, self->field_0x55);
        if (k2 == 0x66) {
            int a = func_80236E6C(self, 0x49);
            v1n += RoundHalfAway0(lbl_eu_80668694 * (float)(v1n * a));
            a = func_80236E6C(self, 0x49);
            v2n += RoundHalfAway0(lbl_eu_80668694 * (float)(v2n * a));
        } else if (k2 == 0x67) {
            int a = func_80236E6C(self, 0x9);
            v1n += RoundHalfAway0(lbl_eu_80668694 * (float)(v1n * a));
            a = func_80236E6C(self, 0x9);
            v2n += RoundHalfAway0(lbl_eu_80668694 * (float)(v2n * a));
        } else if (k2 == 0x68) {
            int a = func_80236E6C(self, 0x3a);
            v1n += RoundHalfAway0(lbl_eu_80668694 * (float)(v1n * a));
            a = func_80236E6C(self, 0x3a);
            v2n += RoundHalfAway0(lbl_eu_80668694 * (float)(v2n * a));
        } else if (k2 == 0x69) {
            int a = func_80236E6C(self, 0x5c);
            v1n += RoundHalfAway0(lbl_eu_80668694 * (float)(v1n * a));
            a = func_80236E6C(self, 0x5c);
            v2n += RoundHalfAway0(lbl_eu_80668694 * (float)(v2n * a));
        }
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_80239030 - arts info text update. Same shape as func_8023AF60 but the
// two lookups' results are combined into a grid offset (CArtsInfo_GetGridA
// pattern: first-entry + second-entry * (level - 1)), formatted, then on
// level-up the offset for the next level is formatted on layout 2 and the
// colour-pair helper runs when the two offsets differ.
void func_80239030(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023916C - arts grid percentage. Both string lookups are called but
// their results discarded; the stat id (CArtsInfo_GetStatId20) is multiplied by two
// grid offsets (CArtsInfo_GetGridA/8023719C keyed on the arts level), each scaled
// by 0.5 and converted via RoundHalfAway0, then formatted with the
// BdatTouchStringCell(0x32, 0x3d, 0x52) string into buf1. On level-up (< 0xA) the
// level+1 offsets are recomputed and formatted on layout 2 and the
// colour-pair helper always runs.
// Entry param-save-order wall: retail copies arg2 (r4) before self (r3);
// MWCC invariantly emits the param-1 (r3) copy first (same ABI-boundary
// artifact as CMenuShopSell func_8018B130). Shadows kept for documentation.
// Int->float conversions: retail lfds the shared sdata2 correction constant
// lbl_eu_80668698; the plain-cast form reproduces retail's exact schedule
// but MWCC pools the magic as a TU-local @label - needs a CArtsInfo.o
// pool_patterns postprocess rule. Ruled out from source: ConvS32ToF64 helper
// (inlines with per-site temps, +0x20 frame / _savegpr_25 drift), explicit
// shared union temp (aliasing-based hoist of the mullw/xoris stores,
// float-reg rotation), volatile union temps (volatile forces both
// conversions to evaluate eagerly before the calls, +0x10 frame),
// u32-const arg2 lever (full regalloc shift), shadow declaration order
// (no effect on ABI param-copy sequence).
void func_8023916C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Entry param-copy-order wall: retail copies arg2 (r4) before self (r3);
    // shadow declaration order does not change MWCC's ABI param-copy
    // sequence (verified - both orders give r3-first).
    CArtsInfo* self = self_;
    u32 arg2 = arg2_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // level assigned late (after sprintf1) so the addi lands after the first
    // sprintf like retail's in-place addi r27, r26, 2.
    int level;
    func_802374F0(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId20(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    int v1 = RoundHalfAway0(lbl_eu_80668694 * (float)(g1 * stat));
    int v2 = RoundHalfAway0(lbl_eu_80668694 * (float)(g2 * stat));
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(lbl_eu_80668694 * (float)(g1n * stat));
        int v2n = RoundHalfAway0(lbl_eu_80668694 * (float)(g2n * stat));
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, v1n, str2, v2n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_8023939C - arts info text update. The stat value (CArtsInfo_GetStatF10) is
// scaled by 100 into cur, combined with a grid offset (v1*(level-1)+v2) and
// scaled again, then formatted into buf1. On level-up (< 0xA) the lookups are
// re-fetched and the level grid recomputed for layout 2; the colour-pair
// helper runs when the two formatted values differ.
// u32 const on arg2: entry param-copy lever - retail copies r4 (arg2)
// before r3 (self).
void func_8023939C(CArtsInfo* self, u32 const arg2, int arg3) {
    // Declaration order = reverse frame-slot order (MWCC allocates
    // last-declared lowest): cvB@+0x58, cvA@+0x48, buf1@+0x28, buf2@+0x08.
    char buf2[32]; // sprintf at +0x8
    char buf1[32]; // sprintf at +0x28
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    int base = CArtsInfo_GetStatF10(self);
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    int cur = (int)(lbl_eu_80668694 * ConvS32ToF64(base * 100));
    // Grid offset folded into the conversion expression: retail interleaves
    // the (level-1) multiply with the conversion store sequence. Note the
    // operand order: the SECOND lookup (v2) is multiplied by (level-1), the
    // FIRST (v1) is added.
    int val1 =
        (int)(lbl_eu_80668694 * ConvS32ToF64((int)(cur * (v2 * (u32)(self->field_0x56 - 1) + v1))));
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, val1, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u8 n1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
        u8 n2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
        // Next-level grid folded into the conversion like retail (second
        // lookup multiplied by level, first added).
        int val2 =
            (int)(lbl_eu_80668694 * ConvS32ToF64((int)(cur * (n2 * (u32)self->field_0x56 + n1))));
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, val2, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (val1 != val2) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023959C - arts info text update. The stat value (CArtsInfo_GetStatF10,
// (int)float at +0x10) is scaled by 100 then halved into x1; the two grids
// are x1*g1 and x1*g2, each halved, formatted into buf1 (sprintf 0x282). On
// level-up the level+1 grids are recomputed for layout 2 and the colour-pair
// helper always runs.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (retail-identical instruction schedule and compiler-temp slot placement);
// the cast magic pools as TU-local @8117 instead of lbl_eu_80668698 - see
// the pool_patterns note at the lbl_eu_80668698 definition.
// Entry param-save-order wall (same as func_8023916C): retail copies arg2
// (r4) before self (r3); MWCC invariantly emits the param-1 (r3) copy first.
void func_8023959C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    CArtsInfo* self = self_;
    u32 arg2 = arg2_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // level assigned late (after sprintf1) so the addi lands after the first
    // sprintf like retail's in-place addi r27, r26, 2.
    int level;

    // Results discarded: retail keeps both calls but never uses the strings.
    func_802374F0(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatF10(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    // (float)(int) casts emit MWCC's 0x4330-magic stack-slot conversion
    // (lis 0x4330 hoisted into the prologue, xoris'd low word per site).
    int x1 = RoundHalfAway0(lbl_eu_80668694 * (float)(stat * 100));
    int x2 = RoundHalfAway0(lbl_eu_80668694 * (float)(x1 * g1));
    int x3 = RoundHalfAway0(lbl_eu_80668694 * (float)(x1 * g2));
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, x2, str, x3);
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        // Dead call kept: retail emits the conversion+call and drops the result.
        RoundHalfAway0(lbl_eu_80668694 * (float)(x1 * g1n));
        int x3n = RoundHalfAway0(lbl_eu_80668694 * (float)(x1 * g2n));
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        // Retail formats x1 (not the dropped first conversion) as the middle value.
        sprintf(buf1, lbl_eu_8050B00C + 0x282, x1, str2, x3n);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_802397F4 - arts info text update. Same shape as func_8023B12C: the
// two string lookups are discarded and the formatted string comes from
// BdatTouchStringCell(0x32, 0x3d, 0x52), but the two grid offsets come from the
// CArtsInfo_GetGridA/8023719C lookups keyed on the current arts level. On level-up
// (< 0xA) the offsets for level+1 are formatted on layout 2 and the
// colour-pair helper always runs.
void func_802397F4(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // Declaration order drives saved-reg allocation (first -> highest):
    // retail keeps level in r30, grid2 in r29, grid1 in r28 (sharing arg2).
    // level is assigned late (after sprintf1) so the addi lands after the
    // first sprintf like retail's in-place addi r30, r30, 2.
    int level;
    u32 grid1;
    u32 grid2;
    func_802374F0(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    grid1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    grid2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, grid1, str, grid2);
    // Eager intermediate forces the addi next to sprintf1 like retail
    // (addi r30, r30, 2).
    level = arg3 + 2;
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        grid2 = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        grid1 = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, grid2, str2, grid1);
        sprintf(buf2, lbl_eu_8050B00C + 0x23b, level);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
    }
}

// func_80239964 - arts info text update. Byte-identical body to
// func_80239030 (same lookups, same grid-offset formatting, same level-up
// branch), kept as a separate retail symbol.
void func_80239964(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80239AA0 - arts info text update. Byte-identical body to
// func_80239030 (same lookups, same grid-offset formatting, same level-up
// branch), kept as a separate retail symbol.
void func_80239AA0(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80239BDC - arts info text update. Same shape as func_80239030 but the
// grid offset is scaled by 10 (mulli 0xa) before formatting: the value is the
// arts AP cost rather than a raw grid index.
void func_80239BDC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = (v1 + v2 * (self->field_0x56 - 1)) * 10;
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = (v1 + v2 * self->field_0x56) * 10;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_80239D20 - arts info text update. The two byte lookups are converted
// to float; when the first is non-zero it is scaled by lbl_eu_806686A4, then
// the value scale*v2*(level-1) is added and formatted into buf1. On level-up
// (< 0xA) the level grid is recomputed for layout 2; the colour-pair helper
// runs when the two values differ.
// Entry param-save-order wall (see func_8023916C).
// fb declared (uninitialized) before fa: MWCC colours equal-lifetime FPRs
// in reverse declaration order, and retail keeps fb in f31 / fa in f30.
void func_80239D20(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Entry param-save-order wall (see func_8023916C): retail emits the two
    // entry copies in the opposite order. Levers tried: shadow declaration
    // order (both ways), const qualifier, no-shadow direct-arg form - all
    // leave the emission order unchanged (MWCC invariantly copies param 1
    // first).
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    // fb declared (uninitialized) before fa: MWCC colours equal-lifetime FPRs
    // in reverse declaration order, and retail keeps fb in f31 / fa in f30.
    // The conversion statements keep the retail evaluation order.
    // NOTE: the plain casts reproduce retail's exact conversion schedule
    // ([0x43300000, v] lfd/fsubs via the shared sdata2 correction constants),
    // but MWCC pools the magics as TU-local @N labels instead of lfding the
    // named sdata2 symbols (lbl_eu_806686A8 / lbl_eu_80668698) - every source
    // form that references the named doubles rewrites the schedule (+frsp,
    // frame resize).
    float fb;
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    float fa = (float)v1;
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    fb = (float)v2;
    if (fa != lbl_eu_80668680) {
        fa = fa * lbl_eu_806686A4;
    }
    float cur = fa + lbl_eu_806686A4 * (fb * (float)(self->field_0x56 - 1));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = fa + lbl_eu_806686A4 * (fb * (float)(int)self->field_0x56);
        sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf1);
        func_eu_8023D490(self, arg2, buf1);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023AB8C - arts info text update. Formats
// 1.0 + scale*(float)v1 + (float)v2*(level-1) into buf1, post-processes it
// and pushes it onto layout 1. On level-up (< 0xA) the level grid is
// recomputed (level instead of level-1, s32 magic) for layout 2; the
// colour-pair helper runs when the two values differ.
// Entry param-save-order wall (see func_8023916C).
// Int->float conversions: retail lfds the shared sdata2 correction constants
// (lbl_eu_806686A8 unsigned / lbl_eu_80668698 signed); the plain-cast forms
// reproduce retail's exact schedule but MWCC pools the magics as TU-local
// labels - needs a CArtsInfo.o pool_patterns postprocess rule.
void func_8023AB8C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    float fa = (float)v1;
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    float fb = (float)v2;
    float cur = lbl_eu_80668684 + (lbl_eu_806686A4 * fa + fb * (float)(self->field_0x56 - 1));
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, cur, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = lbl_eu_80668684 + (lbl_eu_806686A4 * fa + fb * (float)(int)self->field_0x56);
        sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, nxt, s2);
        func_eu_80136F90(buf1);
        func_eu_8023D490(self, arg2, buf1);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}
// func_80239EFC - arts info text update. Formats the arts skill name and a
// byte-keyed table value (BdatGetU8ByTableKey) into buf1, the level string into
// buf2, then pushes both onto the layouts (layout 2 only when field_0x56
// < 0xA). Same shape as func_8023A55C but the 3rd sprintf arg is a lookup.
void func_80239EFC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_80239FC4 - arts info text update. Same shape as func_80239EFC (u8
// lookup at 0x2ab) but the displayed value is a percentage:
// scale * level + base (MWCC u32->float conversions via the 2^52 magic),
// rounded to unsigned. On level-up the level+1 percentage is formatted on
// layout 2 and the colour-pair helper runs when the two values differ.
// NOTE: retail lfds the 2^52 correction constants from the shared sdata2
// labels (lbl_eu_806686A8 unsigned / lbl_eu_80668698 signed); MWCC pools the
// plain-cast equivalents as TU-local @N labels - needs a CArtsInfo.o
// pool_patterns postprocess rule (values 43300000_00000000 / 43300000_80000000).
void func_80239FC4(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2ab, self->field_0x55);
    u32 cur = (u32)(lbl_eu_806686BC * (float)self->field_0x56 + (float)r);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = (u32)(lbl_eu_806686BC * (float)(self->field_0x56 + 1) + (float)r);
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023A148 - arts info text update. Same shape as func_80239EFC but
// uses the s16-keyed lookup BdatGetS16ByTableKey (result sign-extended at the
// sprintf call site).
void func_8023A148(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A210 - arts info text update. Same shape as func_8023A148
// (s16-keyed lookup BdatGetS16ByTableKey, result sign-extended at the sprintf call
// site via the s16 vararg promotion).
void func_8023A210(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A2D8 - arts info text update. Formats the arts skill name and the
// doubled stat id (CArtsInfo_GetStatId20) into buf1, the level string into buf2,
// then pushes both onto the layouts (layout 2 only when field_0x56 < 0xA).
void func_8023A2D8(CArtsInfo* self, u32 arg2, int arg3) {
    u32 a2 = arg2;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, a2);
    char* s2 = CArtsInfo_GetSkillStr(self, a2);
    int id = CArtsInfo_GetStatId20(self);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, id << 1, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A398 - arts info text update. Same shape as func_80239EFC
// (u8-keyed lookup BdatGetU8ByTableKey at 0x289, zero-extended at the sprintf
// call site via the u8 vararg promotion).
void func_8023A398(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A460 - arts info text update. Same shape as func_8023B280 but the
// converted double is scaled by lbl_eu_806686A4 when the s16 lookup is
// non-zero (retail: extsh. branch + lfs/fmuls).
void func_8023A460(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    // MWCC s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs, no frsp).
    float d = (float)r;
    if (r != 0) {
        d = d * lbl_eu_806686A4;
    }
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, d, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023A55C - arts info text update. Formats two strings (arts skill
// names + level) and pushes them onto both layouts (layout 2 only when
// field_0x56 < 0xA).
#pragma optimize_for_size on
// Shadowed locals keep the retail register mapping (arg2->r31, self->r28);
// the residual 2-instruction reg swap is the entry param-save order
// (retail copies r4->r31 before r3->r28; see func_8023916C). Witness-blocked
// (abi-boundary); plain -O4,p without the pragma breaks the stmw merge.
void func_8023A55C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 5, CArtsInfo_GetSkillStr(self, arg2));
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A60C - arts info text update. Formats the skill name, a double
// constant and the second skill string into buf1, post-processes it
// (func_eu_80136F90) and hands it to func_eu_8023D490 before the usual
// level-string + dual-layout push (layout 2 only when field_0x56 < 0xA).
// func_8023A60C - arts info text update. Formats the skill name, a double
// constant and the second skill string into buf1, post-processes it
// (func_eu_80136F90) and hands it to func_eu_8023D490 before the usual
// level-string + dual-layout push (layout 2 only when field_0x56 < 0xA).
// Residual 2-instruction reg swap is the retail entry param-save order:
// retail copies r4 before r3; MWCC invariantly emits the param-1 (r3) copy
// first (same ABI-boundary artifact as CMenuShopSell func_8018B130).
void func_8023A60C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Shadow params in retail's save order (arg2 copy before self copy).
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, lbl_eu_806686C0, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A6BC - same shape as func_8023A55C with level constant 0x19.
#pragma optimize_for_size on
void func_8023A6BC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 25, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A76C - arts info text update with fixed level 25 (same shape as
// func_8023A6BC). Formats the arts skill name/level strings and pushes them
// onto both layouts (layout 2 only when field_0x56 < 0xA).
#pragma optimize_for_size on
void func_8023A76C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 25, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A81C - same shape as func_8023A76C with level constant 50.
#pragma optimize_for_size on
void func_8023A81C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 50, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A8CC - same shape as func_8023A76C with level constant 100.
#pragma optimize_for_size on
void func_8023A8CC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 100, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023A97C - arts info text update. Same shape as func_8023A60C with
// a different format offset (0x2bb) and double constant (lbl_eu_806686C8).
void func_8023A97C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x2bb, s1, lbl_eu_806686C8, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023AA2C - same shape as func_8023A76C with level constant 50.
// Entry param-save-order wall (see func_8023916C): retail saves arg2 (r31)
// before self (r28); MWCC invariantly emits the param-1 copy first.
#pragma optimize_for_size on
void func_8023AA2C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 50, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}
#pragma optimize_for_size off

// func_8023AADC - same shape as func_8023A76C with level constant 2.
// Residual: retail spills arg2->r31 BEFORE self->r28; MWCC (size-mode, which
// is required for the retail stmw r28 frame) always copies r3 first. This is
// the documented param-save claim-order invariant (MWCC_CASES "VM-op/
// saved-param claim order", OPEN) - not source-controllable.
#pragma optimize_for_size on
void func_8023AADC(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, 2, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023AD5C - arts info text update. Same shape as func_80239EFC with
// the u8-keyed lookup BdatGetU8ByTableKey at 0x2ab.
void func_8023AD5C(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = CArtsInfo_GetSkillStr(self, a2);
    u8 r = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2ab, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023AE24 - arts info text update. Byte-identical body to
// func_80239030 / func_80239AA0 (same lookups, same grid-offset formatting,
// same level-up branch), kept as a separate retail symbol.
// Entry param-save-order artifact (unit-wide, ~40 dispatch handlers): retail
// copies arg2 (r4->r25) BEFORE self (r3->r31) at entry; our build invariantly
// emits the param-1 copy first. Shadow-locals, early local copy of arg2, and
// buf declaration reordering were all tried - none change the copy order
// (copy-locals coalesce; decl reorder swaps frame offsets; optimize_for_size
// is neutral here - unlike CTagProcessor TagWriterCode7). Same 2-op
// reg_swap present in func_80239030/39AA0/916C/9EFC/A55C.
void func_8023AE24(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, cur, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023AF60 - arts info text update. Two byte-keyed lookups
// (BdatGetU8ByTableKey at 0x289/0x29f) are called and their results discarded
// (retail keeps the bl's but never materializes the return values), then the
// arts level (field_0x56) is formatted. On level-up (< 0xA) the level+1
// string replaces buf1 on layout 2 and the colour-pair helper runs when the
// two level values differ.
void func_8023AF60(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 level = self->field_0x56;
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, level, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = self->field_0x56 + 1;
        sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, nxt, s2);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (level != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023B074 - arts info text update. Same shape as func_8023A60C but the
// vararg double constant is loaded from a float (lfs) pool slot
// (lbl_eu_806686A0).
void func_8023B074(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Shadow params in retail's save order (arg2 before self before arg3).
    u32 arg2 = arg2_;
    CArtsInfo* self = self_;
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, lbl_eu_806686A0, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023B12C - arts info text update. The two string lookups
// (func_802374F0/8023754C) are called but their results discarded; the
// formatted string comes from BdatTouchStringCell(0x32, 0x3d, 0x52) instead. Same
// grid-offset (v1 + v2*(level-1)) and level-up branch as func_80239030, with
// the format at 0x282 and the level constant 5.
void func_8023B12C(CArtsInfo* self_, u32 arg2_, int arg3_) {
    // Shadow params in retail's save order (arg2 before self before arg3).
    u32 arg2 = arg2_;
    func_802374F0(self_, arg2);
    CArtsInfo* self = self_;
    CArtsInfo_GetSkillStr(self, arg2);
    int arg3 = arg3_;
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u8 v1 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x289, self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x29f, self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    char* str = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
    sprintf(buf1, lbl_eu_8050B00C + 0x282, 5, str, cur);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        char* str2 = BdatTouchStringCell(lbl_eu_8050B00C + 0x32, lbl_eu_8050B00C + 0x3d, 0x52);
        sprintf(buf1, lbl_eu_8050B00C + 0x282, 5, str2, nxt);
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, &lbl_eu_80664758, &lbl_eu_80664760);
        }
    }
}

// func_8023B280 - arts info text update. Same shape as func_8023A60C but the
// vararg double is the s16-keyed lookup result (BdatGetS16ByTableKey at 0x2b3)
// converted via MWCC's s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs
// trick, no frsp).
void func_8023B280(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = func_802374F0(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2b3, self->field_0x55);
    // MWCC s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs, no frsp).
    float d = (float)r;
    sprintf(buf1, lbl_eu_8050B00C + 0x232, s1, d, s2);
    func_eu_80136F90(buf1);
    func_eu_8023D490(self, arg2, buf1);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_8023B368 - arts info text update. Same shape as func_8023A148 but the
// s16-keyed lookup BdatGetS16ByTableKey uses table offset 0x2c4.
void func_8023B368(CArtsInfo* self, u32 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u32 a2 = arg2;
    char* s1 = func_802374F0(self, a2);
    char* s2 = CArtsInfo_GetSkillStr(self, a2);
    s16 r = BdatGetS16ByTableKey(lbl_eu_8050B00C + 0x18b, lbl_eu_8050B00C + 0x2c4, self->field_0x55);
    sprintf(buf1, lbl_eu_8050B00C + 0x266, s1, r, s2);
    sprintf(buf2, lbl_eu_8050B00C + 0x23b, arg3 + 2);
    func_80136A1C(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        func_80136A1C(self->mpLayout2, buf2, buf1, 0);
    }
}

// func_eu_8023D490 - German-language arts text post-processing. Only runs
// when the system language is 5 (German) and the row id for field_0x50 in the
// 0x20c table is 0x4f. Replaces '.' with ',' and skips the second byte of
// double-byte (Shift-JIS) characters so byte counts stay aligned. C linkage
// (retail symbol is unmangled; the extern "C" declaration is in the header).
// optimize_for_size gives the retail stmw r29 prologue for the 3 saved params.
#pragma optimize_for_size on
__declspec(noinline) void func_eu_8023D490(CArtsInfo* self, u32 arg2, char* str) {
    u8 lang = getLanguage__9CDeviceSCFv();
    if (lang != 5) return;

    u8 r = BdatGetU8Direct((u32)self->field_0x50, lbl_eu_8050B00C + 0x20c, arg2);
    if (r != 0x4f) return;

    int len = strlen(str);
    int c;
    int i = 0;
    while (i < len) {
        c = *str;
        if (c == 0x2e) {
            *str = 0x2c; // '.' -> ','
        } else if (c >= 0x81 && c <= 0x9f) {
            i += 1;
            str += 1;
        } else if (c >= 0xe0 && c <= 0xef) {
            i += 1;
            str += 1;
        }
        i += 1;
        str += 1;
    }
}
#pragma optimize_for_size off

void CArtsInfo::setField54(u8 val) { field_0x54 = val; }

void CArtsInfo::setField55(u8 val) { field_0x55 = val; }

void CArtsInfo::setField56(u8 val) { field_0x56 = val; }

void CArtsInfo::setField58(u16 val) { field_0x58 = val; }

u32 CArtsInfo::getField5A() {
    s8 val = field_0x5A;
    u32 result = __cntlzw(val);
    return result >> 5;
}

int CArtsInfo::isField44GE6() {
    int field = field_0x44;
    return field >= 6 ? 1 : 0;
}

void CArtsInfo::initialize() {
    if (mpLayout1 == nullptr) return;
    if (field_0x4C == 0) return;
    if (field_0x50 != 0) {
        field_0x48 = 1;
        field_0x40 = 1;
    }
}

// Retail OnFileEvent: layout-archive loader + state-2 message handler.
//
// Branch 1 (event handle == field_0x14, i.e. the arts arc finished loading):
// creates the MEM2 region, attaches the arc to a fresh resource accessor,
// builds layout1 (+4 anims) and layout2 (+2 anims), binds the device font
// into both root panes, seeds panes from the language string, sets message
// texts, positions the message pane from the 'timg' resource coords,
// snapshots the two colour pairs into the sbss globals, builds the embedded
// cursor from a temp CCur18, runs the layout setup pass, then frees the
// region and returns 1.
//
// Branch 2 (event handle == field_0x18, the message bdat file): routes the
// loaded data into CBdat, resolves the two FP table entries named by the
// string table, calls the split1 reset stub, clears field_0x18, returns 1.
int CArtsInfo::OnFileEvent(CEventFile* event) {
    if ((u32)field_0x14 == (u32)event->field_04) {
        mMemRegion.createRegion((int)getHandleMEM2__Q23mtl10MemManagerFv(),
                                0x18000, &lbl_eu_8050B00C[0x2CC], 0);
        Class_8045F858 host(&mMemRegion);
        // NOTE: accessor is re-read from field_0x1C at every use (retail
        // never caches it in a saved register).
        CFileHandle* fh = (CFileHandle*)field_0x14;
        void* arcData = fh->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        field_0x1C = (int)createArcResourceAccessor__10CLibLayoutFv();
        ((nw4r::lyt::ArcResourceAccessor*)field_0x1C)->Attach(arcData, &lbl_eu_8050B00C[0x2D6]);

        buildLayout(&mpLayout1, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x2DA]);
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans1, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x2F4]);
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans2, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x311]);
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans3, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x333]);
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans4, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x356]);

        nw4r::lyt::Pane* root1 = *(nw4r::lyt::Pane**)((u8*)mpLayout1 + 0x10);
        void* fontObj1 = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout1);
        func_8013676C(root1, (u32)((IDeviceFontInfo*)fontObj1)->getFont());

        char* lang = CUICfManager_getPackedFont9C();
        if (lang != 0) {
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[71], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[388], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[238], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[253], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[268], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[283], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[298], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[313], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[328], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[343], (u32)lang);
            setLayoutTextBoxFont(mpLayout1, &lbl_eu_8050B00C[358], (u32)lang);
        }
        mpLayout1->SetAnimationEnable(mpAnimTrans2, false);
        mpLayout1->SetAnimationEnable(mpAnimTrans3, false);
        mpLayout1->SetAnimationEnable(mpAnimTrans4, false);
        mpLayout1->SetAnimationEnable(mpAnimTrans1, true);
        mpLayout1->Animate(0);

        // Fixed caption textboxes (message panes keyed by arts type).
        LayoutSetTextBoxFmtValue(mpLayout1, &lbl_eu_8050B00C[0x378],
                      BdatTouchStringCell(&lbl_eu_8050B00C[0x32], &lbl_eu_8050B00C[0x3D], 47), 0);
        LayoutSetTextBoxFmtValue(mpLayout1, &lbl_eu_8050B00C[0x382],
                      BdatTouchStringCell(&lbl_eu_8050B00C[0x32], &lbl_eu_8050B00C[0x3D], 23), 0);
        LayoutSetTextBoxFmtValue(mpLayout1, &lbl_eu_8050B00C[0x38C],
                      BdatTouchStringCell(&lbl_eu_8050B00C[0x32], &lbl_eu_8050B00C[0x3D], 49), 0);
        LayoutSetTextBoxFmtValue(mpLayout1, &lbl_eu_8050B00C[0x397],
                      BdatTouchStringCell(&lbl_eu_8050B00C[0x32], &lbl_eu_8050B00C[0x3D], 50), 0);
        LayoutSetTextBoxFmtValue(mpLayout1, &lbl_eu_8050B00C[0x3B5],
                      BdatTouchStringCell(&lbl_eu_8050B00C[0x3A2], &lbl_eu_8050B00C[0x3B0], 43), 0);

        // Message-detail textbox: pane name depends on the CFGameManager
        // query result.
        const char* detailPane = (isClassicController__Q22cf13CfGameManagerFv(-1) == 0)
                                     ? &lbl_eu_8050B00C[970]
                                     : &lbl_eu_8050B00C[961];
        // Retail masks the lookup result to its low half and feeds it through
        // MakeTplNameSysFile to get the message resource name.
        char* resName = MakeTplNameSysFile(
            BdatGetU16ByTableKey(&lbl_eu_8050B00C[0x3A2], detailPane, 43));
        void* timg = ((nw4r::lyt::ArcResourceAccessor*)field_0x1C)->GetResource(0x74696d67, resName, 0);
        if (timg != 0) {
            CArtsMsgObj* msg = (CArtsMsgObj*)timg;
            LayoutSetTextBoxFmtValue(mpLayout1, &lbl_eu_8050B00C[0x3D3], (char*)timg, 0);
            PaneSetTexPaletteByName(mpLayout1, &lbl_eu_8050B00C[0x3D3], (u32)msg->chain);
            nw4r::lyt::Pane* msgPane1 =
                root1->FindPaneByName(&lbl_eu_8050B00C[0x3D3], true);
            if (msgPane1 != 0) {
                u16 row = msg->chain->pCoords->row;
                u16 col = msg->chain->pCoords->col;
                CArtsPaneSize* ps = (CArtsPaneSize*)msgPane1;
                ps->width.f = (float)(int)row;
                ps->height.f = (float)(int)col;
            }
        }

        buildLayout(&mpLayout2, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x2DA]);
        bindLayoutAnimTransform(mpLayout2, &mpAnimTrans5, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x356]);
        bindLayoutAnimTransform(mpLayout2, &mpAnimTrans6, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, &lbl_eu_8050B00C[0x3DD]);

        void* fontObj2 = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout2);
        func_8013676C(*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10),
                      (u32)((IDeviceFontInfo*)fontObj2)->getFont());

        if (lang != 0) {
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[238], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[253], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[268], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[283], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[298], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[313], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[328], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[343], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[358], (u32)lang);
            setLayoutTextBoxFont(mpLayout2, &lbl_eu_8050B00C[373], (u32)lang);
        }
        mpLayout2->SetAnimationEnable(mpAnimTrans6, false);
        mpLayout2->SetAnimationEnable(mpAnimTrans5, true);
        mpLayout2->Animate(0);

        // Detail panes on layout2: hide each extra pane; the message pane's
        // anchor triple is rotated before it is hidden.
        func_80124270((*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10))->FindPaneByName(&lbl_eu_8050B00C[0x3FC], true), 0);
        nw4r::lyt::Pane* pn1028 = (*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10))->FindPaneByName(&lbl_eu_8050B00C[0x404], true);
        if (pn1028 != 0) {
            CArtsPanePos* pos = (CArtsPanePos*)((char*)pn1028 + 0x2C);
            CArtsFpu x = pos->x, y = pos->y, z = pos->z;
            pos->x.f = z.f;
            pos->y.f = x.f;
            pos->z.f = y.f;
        }
        func_80124270((*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10))->FindPaneByName(&lbl_eu_8050B00C[0x40B], true), 0);
        func_80124270((*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10))->FindPaneByName(&lbl_eu_8050B00C[0x378], true), 0);
        func_80124270((*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10))->FindPaneByName(&lbl_eu_8050B00C[0x184], true), 0);
        func_80124270((*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10))->FindPaneByName(&lbl_eu_8050B00C[0x413], true), 0);
        nw4r::lyt::Pane* pn238 = (*(nw4r::lyt::Pane**)((u8*)mpLayout2 + 0x10))->FindPaneByName(&lbl_eu_8050B00C[0xEE], true);
        func_80124270(pn238, 0);

        // Snapshot the message pane's two vertex-colour pairs into the sbss
        // colour globals.
        CArtsColorPair col1 = func_801397AC(pn238, 0);
        CopyVec4s(reinterpret_cast<CArtsQuadColor*>(&lbl_eu_80664748), &col1);
        CArtsColorPair col2 = func_801397AC(pn238, 1);
        CopyVec4s(reinterpret_cast<CArtsQuadColor*>(&lbl_eu_80664750), &col2);

        // Swap the last halfword of the two cached colour records (retail
        // exchanges element [3] between lbl_eu_80664748/64750 via sda21).
        s16 sw = *(s16*)((u8*)&lbl_eu_80664748 + 6);
        *(s16*)((u8*)&lbl_eu_80664748 + 6) = *(s16*)((u8*)&lbl_eu_80664750 + 6);
        *(s16*)((u8*)&lbl_eu_80664750 + 6) = sw;

        // Build a temporary cursor against the shared accessor and copy its
        // body over the embedded one (vptr at +0 preserved; word copies).
        u8 tmpCursor[0x18];
        __ct__CCur18(tmpCursor, CUICfManager_getArcResourceAccessor());
        *(u32*)&mCursor[4] = *(u32*)&tmpCursor[4];
        *(u32*)&mCursor[8] = *(u32*)&tmpCursor[8];
        *(u32*)&mCursor[12] = *(u32*)&tmpCursor[12];
        *(u32*)&mCursor[16] = *(u32*)&tmpCursor[16];
        mCursor[20] = tmpCursor[20];
        mCursor[21] = tmpCursor[21];
        __dt__6CCur18Fv(tmpCursor, -1);
        ((CBaseCur*)mCursor)->initLayout();
        CArtsInfo_BindAllPanes(this);
        func_8023B430(this);
        field_0x14 = 0;
        mMemRegion.func_8045F810();
        return 1;
    }

    if ((u32)field_0x18 == (u32)event->field_04) {
        CFileHandle* fh2 = (CFileHandle*)field_0x18;
        void* data = fh2->getData();
        setBdatEntry__5CBdatFUlPv(2, data);
        func_8003AA34();
        field_0x4C = (int)getFP__FPCc(&lbl_eu_8050B00C[1054]);
        func_8003AA34();
        field_0x50 = (int)getFP__FPCc(&lbl_eu_8050B00C[1063]);
        // split1 refresh notification (retail bl at +0x678c)
        func_8023B430(this);
        field_0x18 = 0;
        return 1;
    }
    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static constructor: initialise the arts font/colour small-data entries
// (same shape as CItemBoxInfo's sinit_801EABC4).
void sinit_8023BC8C() {
    SplitU32ToS16s(&lbl_eu_80664748, 0);
    SplitU32ToS16s(&lbl_eu_80664750, 0);
    func_801C4B60(&lbl_eu_80664758, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(&lbl_eu_80664760, 0x25, 0x8a, 0xce, 0);
}
