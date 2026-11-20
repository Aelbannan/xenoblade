#define IWORK_EVENT_INLINE_DTOR // ~CArtsInfo elides the empty base dtor (retail)
// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

// CItem.hpp declares the typed CItem_initItemImplInstances while
// CfGameManager.hpp's import block repeats it with a void* shape; MWCC forbids
// overloading an extern "C" function (10197), so hide the import-block copy.
#define CItem_initItemImplInstances cfImport_CItem_initItemImplInstances
#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#undef CItem_initItemImplInstances
#include "kyoshin/CArtsInfo.hpp"
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/CTitle.hpp"  // setGXColorS10 (canonical C++ declaration)
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/cf/CItem.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/device/CDeviceSC.hpp"

#include <stdlib.h>
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Extern declarations for referenced symbols
// Counter_TestBit is declared once in the owning header code_8025FB10.hpp.


extern void* jumptable_eu_805369A0[];

// Float constants in small data area
extern const float lbl_eu_80668684; // 1.0f (const: retail hoists the pool load)

extern float lbl_eu_80668680; // 0.0f (HEAD-style retail extern)

// Small data objects (.sdata, addressed via @sda21): declared as complete
// 4-byte objects so MWCC emits EMB_SDA21 relocs like retail (an incomplete
// char[] extern would force lis/addi absolute addressing).
extern GXColorS10 sArtsPaneTevColorA;
extern GXColorS10 sArtsPaneTevColorB;
extern u32 sArtsChangeColorA;
extern u32 sArtsChangeColorB;

// Small data object (.sbss, addressed via @sda21): arts bdat file pointer.
// void* (not u32): shared with kyoshin/cf/CItem.hpp (MWCC 10197).

// Shared sdata2 int->float correction constants (2^52-family magics) are
// declared in CArtsInfo.hpp; referenced by name so the conversion lfd's emit
// retail SDA21 relocs instead of TU-local pool labels.


// Forward declarations for state machine functions (defined later in this TU).
// CArtsInfo_RefreshLayout/CArtsInfo_UpdateCursor are extern "C": retail calls the C names (not
// __FP9CArtsInfo).
extern "C" void CArtsInfo_BindAllPanes(CArtsInfo*);
extern "C" void CArtsInfo_RefreshLayout(CArtsInfo*);
extern "C" void CArtsInfo_UpdateCursor(CArtsInfo*);
extern "C" void CArtsInfo_UpdateArtsRow(CArtsInfo*, u8, u8);
// Damage/level helpers: extern "C" so the sprintf-family call sites
// (CArtsInfo_FmtRow05 / CArtsInfo_FmtRow04) emit bl to the unmangled retail names.
extern "C" int CArtsInfo_CalcDmg52(CArtsInfo*);
extern "C" int CArtsInfo_CalcDmg53(CArtsInfo*);
// The definition (later in this TU) inherits the linkage; its body is unchanged.
extern "C" int CArtsInfo_GetStatId1C(CArtsInfo*);

// Forward declarations for animation state handlers. extern "C": retail reloc names
// are the unmangled func_* symbols, not func_*__FP9CArtsInfo.
extern "C" void CArtsInfo_AnimState0(CArtsInfo*);
extern "C" void CArtsInfo_AnimState1(CArtsInfo*);
extern "C" void CArtsInfo_AnimState2(CArtsInfo*);
extern "C" void CArtsInfo_AnimState3(CArtsInfo*);
extern "C" void CArtsInfo_AnimState4(CArtsInfo*);
extern "C" void CArtsInfo_AnimState5(CArtsInfo*);
extern "C" void CArtsInfo_AnimState6(CArtsInfo*);
extern "C" void CArtsInfo_AnimState7(CArtsInfo*);
extern "C" void CArtsInfo_AnimState8(CArtsInfo*);
extern "C" void CArtsInfo_AnimState9(CArtsInfo*);

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
// CArtsInfo.o pool_patterns postprocess rule (see CArtsInfo_FmtRow12 note).
const double lbl_eu_80668698 = 4503601774854144.0;


// (Layout/Pane virtual dispatches go through the real SDK classes:
// `delete` for the +0x08 deleting-dtor, GetResource for +0x0C.)

u8 CArtsInfo::getField48() { return field_0x48; }

u8 CArtsInfo::getField49() { return field_0x49; }

// __ct__CArtsInfo - constructor .text:0x0, size 0xAC
CArtsInfo::CArtsInfo() : mCursor(nullptr) {
    // Initialize all fields to zero/default
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x1C = 0;
    mpLayout1 = nullptr;
    mpAnimTrans1 = nullptr;
    mpAnimTrans2 = nullptr;
    mpAnimTrans3 = nullptr;
    mpAnimTrans4 = nullptr;
    mpLayout2 = nullptr;
    mpAnimTrans5 = nullptr;
    mpAnimTrans6 = nullptr;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x48 = 0;
    field_0x49 = 1;
    field_0x4C = 0;
    field_0x50 = 0;
    field_0x54 = 0;
    field_0x55 = 0;
    field_0x56 = 0;
    field_0x58 = 0;
    field_0x5A = 0;
}

// __dt__9CArtsInfoFv - destructor .text:0xAC, size 0x60. The member dtor
// destroys the +0x5C CCur18 cursor and the +0x04 UnkClass_8045F564 member;
// MWCC adds the deleting-destructor flag branch around operator delete.
CArtsInfo::~CArtsInfo() {}

// CArtsInfo_LoadFiles - file loading
// .text:0x10C, size 0x68
void CArtsInfo_LoadFiles(CArtsInfo* self) {
    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    self->field_0x14 = (int)readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)handle, (const char*)"menu/jp/ArtsInfo.arc", (void*)self, 0, 0);

    self->field_0x18 =
        (int)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
            (u32)KyoshinHeap_GetField44(), (const char*)"common/jp/bdat_menu_arts.bin",
            (void*)self, 0, 0);
}

// CArtsInfo_UpdateAnims - state machine dispatch
// .text:0x174, size 0xDC
void CArtsInfo_UpdateAnims(CArtsInfo* self) {
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
    // Retail's 13-slot jump table maps states 0xA-0xC onto the state-9 body
    // (the table entries all point at its `bl CArtsInfo_AnimState9`), so the
    // switch bound stays 12 and the post-dispatch tail is shared.
    case 9:
    case 0xA:
    case 0xB:
    case 0xC:
        CArtsInfo_AnimState9(self);
        break;
    default:
        break;
    }

    // Post-dispatch: animate both layouts and update cursor
    self->mpLayout1->Animate(0);
    self->mpLayout2->Animate(0);
    self->mCursor.Update();
}

void CArtsInfo_Draw(CArtsInfo* self, void* drawInfo) {
    if (self->field_0x40 == 0) return;

    drawLayout(self->mpLayout1, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);
    drawLayout(self->mpLayout2, reinterpret_cast<nw4r::lyt::DrawInfo*>(drawInfo), 0, 1);

    s8 cursorActive = self->field_0x5A;
    if (cursorActive >= 0) {
        self->mCursor.DrawLayout((nw4r::lyt::DrawInfo*)drawInfo);
    }
}

// CArtsInfo_Cleanup - cleanup
// .text:0x2C4, size 0xC4
void CArtsInfo_Cleanup(CArtsInfo* self) {
    CBdat::getEntry(2);

    closeFileHandle((CFileHandle**)&self->field_0x14);
    closeFileHandle((CFileHandle**)&self->field_0x18);

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
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C);

    // Call virtual cleanup on cursor (+0x0C)
    self->mCursor.cleanup();

    // Cleanup memory region
    deleteRegion__17UnkClass_8045F564Fv(&self->mMemRegion);
}

// CArtsInfo_GetFlag48 - get field_0x48
// .text:0x388, size 0x8
u32 CArtsInfo_GetFlag48(CArtsInfo* self) { return self->field_0x48; }

// CArtsInfo_GetFlag49 - get field_0x49
// .text:0x390, size 0x8
u32 CArtsInfo_GetFlag49(CArtsInfo* self) { return self->field_0x49; }

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
    self->mpAnimTrans4->SetFrame(lbl_eu_80668680);

    self->mpLayout1->Animate(0);

    // Bind animations to layout 1 again
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);

    // Reset anim transform frame
    self->mpAnimTrans3->SetFrame(lbl_eu_80668680);

    self->mpLayout1->Animate(0);

    // Bind animations to layout 2
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, true);

    // Reset anim transform frame
    self->mpAnimTrans5->SetFrame(lbl_eu_80668680);

    self->mpLayout2->Animate(0);

    // Reset cursor state
    self->field_0x5A = 0;

    // Update layout elements with arts data: fetch the character data, then
    // read the display value through the stats sub-object's vtable+0x200
    // virtual (user virtual #126).
    CArtsCharData* obj = (CArtsCharData*)CtrlObjectParam_GetCharEntry(self->field_0x54);
    char* str1 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x18);
    int dispVal = (int)((cf::CActorParam*)&obj->stats)->CActorParam_getSecondCurrency();

    char buf[32];
    sprintf(buf, (char*)"%d%s", dispVal, str1);

    LayoutSetTextBoxText(self->mpLayout1, (char*)"txt_ap00", buf, 0);

    playUISound(0x6D);
}

// CArtsInfo_ToStateA - state machine (state==9 -> 0xA)
// .text:0x61C, size 0xB4
void CArtsInfo_ToStateA(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xA;
    self->field_0x49 = 0;

    // Set cursor visibility
    self->mCursor.SetVisible(0);

    // Real member calls so MWCC emits the retail r12 vtable dispatch
    // (vtable[0x2C] = SetAnimationEnable, vtable[0x38] = Animate).
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans6, false);
    self->mpLayout2->SetAnimationEnable(self->mpAnimTrans5, true);

    // Set anim transform frame
    self->mpAnimTrans5->SetFrame(lbl_eu_80668680);

    self->mpLayout2->Animate(0);

    playUISound(6);
}

// CArtsInfo_ToStateC - state machine (state==9 -> 0xC)
// .text:0x6D0, size 0xAC
void CArtsInfo_ToStateC(CArtsInfo* self) {
    if (self->field_0x44 != 9) return;

    self->field_0x44 = 0xC;
    self->field_0x49 = 0;

    // Set cursor visibility
    self->mCursor.SetVisible(0);

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
    if (advanceAnimTransform(self->mpAnimTrans1, f) == 0) return;

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
    if (advanceAnimTransform(self->mpAnimTrans2, f)) {
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
    if (advanceAnimTransform(self->mpAnimTrans3, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 7;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, true);
}

// CArtsInfo_AnimState5 - animation state 5
// .text:0xB18, size 0xB4
void CArtsInfo_AnimState5(CArtsInfo* self) {
    if (advanceAnimTransform(self->mpAnimTrans4, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 8;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, true);
}

// CArtsInfo_AnimState6 - animation state 6
// .text:0xBCC, size 0x60
void CArtsInfo_AnimState6(CArtsInfo* self) {
    if (advanceAnimTransform(self->mpAnimTrans5, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 9;
    self->field_0x49 = 1;
    self->mCursor.SetVisible(1);
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
    if (advanceAnimTransform(self->mpAnimTrans6, lbl_eu_80668684) == 0) return;

    self->field_0x44 = 0xB;

    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans2, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans4, false);
    self->mpLayout1->SetAnimationEnable(self->mpAnimTrans3, true);
}

// CArtsInfo_BindAllPanes - large layout setup (bind all pane animations) retail
// symbol; inlining would balloon their bodies.
extern "C" void CArtsInfo_BindAllPanes(CArtsInfo* self) {
    // Bind animations for all named panes on layout 1
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_tit", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para00", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para01_00", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para01_01", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para03", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para04", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para05", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para06", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para07", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para08", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para09", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para10", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para11", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value02_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value03_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value04_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value05_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value06_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value07_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value08_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value09_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value10_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_value11_02", (char*)"", 0);

    // Bind animations for all named panes on layout 2
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_tit", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para00", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para01_00", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para01_01", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para03", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para04", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para05", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para06", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para07", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para08", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para09", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para10", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para11", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value02_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value03_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value04_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value05_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value06_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value07_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value08_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value09_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value10_02", (char*)"", 0);
    LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_value11_02", (char*)"", 0);

    // Set text rendering parameters for specific panes on layout 2
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value02_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value03_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value04_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value05_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value06_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value07_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value08_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value09_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value10_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
    PaneMatSetTevColorsByName(self->mpLayout2, (char*)"txt_value11_02", &sArtsPaneTevColorA, &sArtsPaneTevColorB);
}

// CArtsInfo_RefreshLayout - arts info full layout refresh. Formats the arts name/lv
// strings and the colour/flag rows, then binds the row strings to the named panes on
// both layouts. The trailing loop walks arts rows 1..10 (slot 0x1b2 names) and grows
// the arts list (CArtsInfo_UpdateArtsRow) for each row that has a non-empty entry.
// would balloon its body.
extern "C" void CArtsInfo_RefreshLayout(CArtsInfo* self) {
    char buf[32]; // sprintf at +0x8
    char* str1 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x18);
    sprintf(buf, (char*)"%d%s", self->field_0x58, str1);
    LayoutSetTextBoxText(self->mpLayout1, (char*)"txt_ap", buf, 0);

    char* str2 = BdatTouchStringCell((char*)"pc_arts", (char*)"name", self->field_0x55);
    char* str3 = 0;
    if (self->field_0x56 != 0) {
        str3 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", self->field_0x56 + 0x1e);
    }
    sprintf(buf, (char*)"%s%s", str2, str3);
    LayoutSetTextBoxText(self->mpLayout1, (char*)"txt_tit", buf, 0);

    char* str4 = 0;
    if (self->field_0x56 < 0xA) {
        if (self->field_0x56 != 0) {
            str4 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", self->field_0x56 + 0x1f);
        }
        sprintf(buf, (char*)"%s%s", str2, str4);
        LayoutSetTextBoxText(self->mpLayout2, (char*)"txt_tit", buf, 0);
    }

    char* s = BdatGetPtrDirect((const void*)self->field_0x4C, (char*)"msg", self->field_0x55);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para00", s, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para00", s, 0);
    }

    u8 f1 = BdatGetU8Direct((const void*)self->field_0x4C, (char*)"trgA", self->field_0x55);
    u8 f2 = BdatGetU8Direct((const void*)self->field_0x4C, (char*)"trgB", self->field_0x55);
    u8 f3 = BdatGetU8Direct((const void*)self->field_0x4C, (char*)"trgC", self->field_0x55);
    char* p1 = 0;
    char* p2 = 0;
    char* p3 = 0;
    if (f1 != 0) p1 = BdatGetPtrDirect((const void*)self->field_0x50, (char*)"tagStr", f1);
    if (f2 != 0) p2 = BdatGetPtrDirect((const void*)self->field_0x50, (char*)"tagStr", f2);
    if (f3 != 0) p3 = BdatGetPtrDirect((const void*)self->field_0x50, (char*)"tagStr", f3);
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para01_00", p1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para01_00", p1, 0);
    }
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para02", p2, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para02", p2, 0);
    }
    LayoutSetTextBoxFmtValue(self->mpLayout1, (char*)"txt_para03", p3, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, (char*)"txt_para03", p3, 0);
    }

    // Loop counters: full-width ints narrowed to u8 at the call sites
    // (retail clrlwi's the sprintf vararg and the CArtsInfo_UpdateArtsRow arg, but
    // tests the loop bound with a bare cmplwi - a u8 counter would force an
    // extra rlwinm mask before the bound test). n declared first: saved-reg
    // allocation gives it the lower register (retail n->r25, i->r26).
    unsigned int n = 2;
    unsigned int i = 1;
    do {
        sprintf(buf, (char*)"tag%d", (u8)i);
        u8 f = BdatGetU8Direct((const void*)self->field_0x4C, (const char*)buf, self->field_0x55);
        if (f != 0) {
            CArtsInfo_UpdateArtsRow(self, f, (u8)n++);
        }
        i++;
    } while (i <= 0xA);
}

// CArtsInfo_UpdateCursor - cursor name display. Formats the current cursor slot name
// (field_0x5A + 1) and binds it to the pane found by name on layout 1, then sends the
// string to the cursor's message virtual. Early-exits when the cursor slot is negative
// (cursor hidden). retail symbol; inlining would balloon their bodies.
extern "C" void CArtsInfo_UpdateCursor(CArtsInfo* self) {
    char buf[0x20]; // sprintf at +0x18
    nw4r::math::VEC3 pos; // ScalePaneWorldPos output at +0x8
    s8 idx = self->field_0x5A;
    if (idx < 0) return;

    sprintf(buf, (char*)"txt_slct%02d", idx + 1);
    nw4r::lyt::Pane* pane1 = self->mpLayout1->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = self->mpLayout1->GetRootPane()->FindPaneByName((char*)"nul_proportion", true);
    ScalePaneWorldPos(&pos, pane1, pane2, self->mpLayout1->GetRootPane());
    self->mCursor.setRootPaneTranslate(&pos);
}

// CArtsInfo_GetStatId1C - fetch stat block via vtable[0x224] dispatch, return s16 id.
// Declared int (not s16): the callee already sign-extends the s16 slot value
// in r3, so callers (CArtsInfo_FmtRow04/802384F4) add it directly without an
// explicit extsh (retail codegen).
int CArtsInfo_GetStatId1C(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)CtrlObjectParam_GetCharEntry(self->field_0x54);
    CArtsStatBlock* st = (CArtsStatBlock*)((cf::CActorParam*)&obj->stats)->CActorParam_getBattleParams();
    return st->field_0x1C;
}

// CArtsInfo_GetStatId20 - same dispatch, return the id at +0x20. Retail treats the
// result as a full-width int (callers use it without re-sign-extending:
// CArtsInfo_FmtRow15 shifts it directly), so the return type is int.
// .text:0x16E8, size 0x38
// a bl to the retail (unmangled) symbol, not an inlined vtable dispatch.
extern "C" int CArtsInfo_GetStatId20(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)CtrlObjectParam_GetCharEntry(self->field_0x54);
    CArtsStatBlock* st = (CArtsStatBlock*)((cf::CActorParam*)&obj->stats)->CActorParam_getBattleParams();
    return st->field_0x20;
}

// CArtsInfo_GetStatF10 - same dispatch, return (int)float at +0x10
// .text:0x1720, size 0x44
// bl to the retail (unmangled) symbol, not an inlined vtable dispatch.
extern "C" int CArtsInfo_GetStatF10(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)CtrlObjectParam_GetCharEntry(self->field_0x54);
    CArtsStatBlock* st = (CArtsStatBlock*)((cf::CActorParam*)&obj->stats)->CActorParam_getBattleParams();
    return (int)st->field_0x10;
}

// CArtsInfo_GetEquipValue - arts equipment/level lookup. Reads the character's six s16
// slots (weapon + five stat ids at +0x26/+0x1C..+0x24), and for each slot type (a byte
// of the 0x80668688/0x8066868C flags) fetches the item list entry; if the entry's
// slots contain the requested item id, the matching slot's value accumulates into the
// result. Finally the result is capped at the row value from the 0x1d4 table (signed
// compare).
extern "C" int CArtsInfo_GetEquipValue(CArtsInfo* self, int arg2) {
    CArtsCharData* obj = (CArtsCharData*)CtrlObjectParam_GetCharEntry(self->field_0x54);
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
        CItemExt* e = CItem_kindPtr(flags.b[i], ids[i]);
        if (e == 0) continue;
        if (e->field_00 == 0) continue;
        // Retail re-fetches the impl object at every dispatch (5 bl's); a
        // hoisted local would drop two of them.
        u8 count = CItem_initItemImplInstances((CItemData*)e)->vf30((CItemData*)e);
        for (u8 j = 0; j < count; j++) {
            s16 id = CItem_initItemImplInstances((CItemData*)e)->vf40((CItemData*)e, j);
            if (id != -1) {
                CItemExt* e3 = CItem_kindPtr(3, id);
                if (e3 != 0 && e3->field_00 != 0) {
                    if ((u32)arg2 == (u16)CItem_initItemImplInstances((CItemData*)e3)->vf54((CItemData*)e3)) {
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
    u16 t = (u16)BdatGetU16Direct((const void*)::cf::CfBdat::spBtlSkillListFileData, (char*)"max", arg2);
    if (t < result) result = t;
    return result;
}

// CArtsInfo_GetSkill5C - arts skill lookup (flag 0x5C) .text:0x18E0, size 0x58 Walks
// the character-data +0x3534 object: Counter_TestBit tests whether the skill is
// reachable, IdTable_SumValues returns its summed value (see code_8025FB10.cpp). int
// return: the caller's (float) conversion uses the signed xoris magic (retail
// codegen).
extern "C" int CArtsInfo_GetSkill5C(CArtsInfo* self) {
    u8* obj = (u8*)CtrlObjectParam_GetCharEntry(self->field_0x54) + 0x3534;
    if (Counter_TestBit(obj, 0x5C) != 0) {
        return IdTable_SumValues((CUnkObj8025FB10*)obj, 0x5C);
    }
    return 0;
}

// Skill id (0x2D) read from the character record.
extern "C" u32 CArtsInfo_GetSkill2D(CArtsInfo* self) {
    u8* obj = (u8*)CtrlObjectParam_GetCharEntry(self->field_0x54) + 0x3534;
    if (Counter_TestBit(obj, 0x2D) != 0) {
        return IdTable_SumValues((CUnkObj8025FB10*)obj, 0x2D);
    }
    return 0;
}

// CArtsInfo_GetGridA - arts list lookup: fetch two table entries keyed by arg3 (or
// field_0x55 when arg3 is 0) and combine them into a grid offset: u16 first-entry + u8
// second-entry * (arg2 - 1). The ternaries stay int so the u8 mask is emitted at the
// call sites (retail: mr r0,r30 / clrlwi). u8 params: retail call sites mask the
// level+1 argument (clrlwi 24) before the call.
extern "C" int CArtsInfo_GetGridA(CArtsInfo* self, u8 arg2, u8 arg3) {
    int v1 = (arg3 != 0) ? arg3 : self->field_0x55;
    u16 a = BdatGetU16ByTableKey((char*)"pc_arts", (char*)"rate1", v1);
    int v2 = (arg3 != 0) ? arg3 : self->field_0x55;
    u8 b = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_powl", v2);
    return a + b * (arg2 - 1);
}

// CArtsInfo_GetGridB - arts list lookup: same shape as CArtsInfo_GetGridA but with
// different string-table offsets (0x1e8/0x1ee).
extern "C" int CArtsInfo_GetGridB(CArtsInfo* self, u8 arg2, u8 arg3) {
    int v1 = (arg3 != 0) ? arg3 : self->field_0x55;
    u16 a = BdatGetU16ByTableKey((char*)"pc_arts", (char*)"rate2", v1);
    int v2 = (arg3 != 0) ? arg3 : self->field_0x55;
    u8 b = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_powh", v2);
    return a + b * (arg2 - 1);
}

// CArtsInfo_CalcDmg52 - arts info damage/level helper. Reads the character's weapon
// id (+0x26 of the CtrlObjectParam_GetCharEntry data), finds the entry whose first word's
// top 12 bits match it (CItem_kindPtr/BdatGetItemId), then looks up the
// weapon's name row (BdatGetU16Direct) and a flag byte (BdatGetU8Direct). When
// flag bit 4 is set, computes a capped HP value = scale1 * (nameRow * base),
// clamped to 999. The final result is
// scale2 * ((hp & 0xffff) * (skill + artLevel + 100)) masked to 16 bits.
// Both int->float conversions use MWCC's s32 magic (xoris + lbl_eu_80668698).
// Single shared fail label (return 0) at the bottom, matching retail's block
// layout (all guards jump forward to it).
int CArtsInfo_CalcDmg52(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)CtrlObjectParam_GetCharEntry(self->field_0x54);
    s16 weapon = obj->field_0x26;
    if (weapon == -1) goto fail;
    CItemExt* e = CItem_kindPtr(2, weapon);
    if (e == 0) goto fail;
    u32 v0 = e->field_00;
    if (v0 == 0) goto fail;
    u16 id = BdatGetItemId(v0 >> 20);
    u16 hp = BdatGetU16Direct((const void*)::cf::CfBdat::spItmWpnListFileData, (char*)"dmg_low", id);
    u8 b = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmWpnListFileData, (char*)"flag", id);
    if ((b & 4) != 0) {
        // u16 local: retail masks the scale at definition (clrlwi in r3).
        u16 base = (u16)CtrlObjectParam_GetArtsDataWord(obj);
        hp = 1.4f * (float)(hp * base);
        if (hp >= 0x3e7) hp = 0x3e7;  // unsigned cmpli, clamp 999
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so CArtsInfo_GetSkill2D
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(0.01f * (float)(hp * ((int)CArtsInfo_GetEquipValue(self, 0x52) + (int)CArtsInfo_GetSkill2D(self) + 100))) & 0xffff;
fail:
    return 0;
}

// CArtsInfo_CalcDmg53
// CArtsInfo_CalcDmg52 but the name-row string offset is 0x205 and the HP
// scale factor is lbl_eu_806686A0.
int CArtsInfo_CalcDmg53(CArtsInfo* self) {
    CArtsCharData* obj = (CArtsCharData*)CtrlObjectParam_GetCharEntry(self->field_0x54);
    s16 weapon = obj->field_0x26;
    if (weapon == -1) goto fail;
    CItemExt* e = CItem_kindPtr(2, weapon);
    if (e == 0) goto fail;
    u32 v0 = e->field_00;
    if (v0 == 0) goto fail;
    u16 id = BdatGetItemId(v0 >> 20);
    u16 hp = BdatGetU16Direct((const void*)::cf::CfBdat::spItmWpnListFileData, (char*)"dmg_hi", id);
    u8 b = (u8)BdatGetU8Direct((const void*)::cf::CfBdat::spItmWpnListFileData, (char*)"flag", id);
    if ((b & 4) != 0) {
        // u16 local: retail masks the scale at definition (clrlwi in r3).
        u16 base = (u16)CtrlObjectParam_GetArtsDataWord(obj);
        hp = 1.5f * (float)(hp * base);
        if (hp >= 0x3e7) hp = 0x3e7;
    }
    // Both lookups inline: MWCC evaluates + right-to-left, so CArtsInfo_GetSkill2D
    // (rightmost) runs first and its result survives in r31 like retail.
    return (int)(0.01f * (float)(hp * ((int)CArtsInfo_GetEquipValue(self, 0x53) + (int)CArtsInfo_GetSkill2D(self) + 100))) & 0xffff;
fail:
    return 0;
}

// CArtsInfo_GetSkill20C - arts skill string lookup
// .text:0x1E00, size 0x5C
// BdatGetU8Direct finds the row for field_0x50 in the (char*)"tagFS"
// list; if present, BdatTouchStringCell formats the string (3rd arg = the row's
// byte index, which retail keeps in r5 from the clrlwi mask). Result lives
// in a local so MWCC hoists `li r30, 0` before the call and returns it via
// r30 (same shape as CArtsInfo_GetSkillStr).
extern "C" char* CArtsInfo_GetSkill20C(CArtsInfo* self, u32 arg2) {
    char* result = 0;
    u8 r = BdatGetU8Direct((const void*)self->field_0x50, (char*)"tagFS", arg2);
    if (r != 0) {
        result = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", r);
    }
    return result;
}

// CArtsInfo_GetSkillStr - arts skill string lookup (same shape as CArtsInfo_GetSkill20C,
// different string-table offset 0x212). result lives in a local so MWCC
// hoists `li r30, 0` before the call and returns it via r30.
extern "C" char* CArtsInfo_GetSkillStr(CArtsInfo* self, u32 arg2) {
    char* result = 0;
    u8 r = BdatGetU8Direct((const void*)self->field_0x50, (char*)"tagBS", arg2);
    if (r != 0) {
        result = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", r);
    }
    return result;
}

// CArtsInfo_UpdateArtsRow - arts list entry update. Formats the current arts-row name
// (BdatGetPtrDirect row lookup for field_0x50 at table 0x1ab) and the level string
// (arg3 + 2) into a buffer bound to the row pane on both layouts (layout 2 only when
// field_0x56 < 0xA), then dispatches on the row id (BdatGetU8Direct lookup at 0x225,
// masked to u8) through a 44-case jump table to the per-arts text-update handlers.
extern "C" void CArtsInfo_FmtRow00(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow01(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow02(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow03(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow04(CArtsInfo*, u8, int, u8);
extern "C" void CArtsInfo_FmtRow05(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow06(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow07(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow08(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow09(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow0A(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow0B(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow0C(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow0D(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow0E(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow0F(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow10(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow11(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow12(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow13(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow14(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow15(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow16(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow17(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow18(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow19(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow1A(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow1B(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow1C(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow1D(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow1E(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow1F(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow20(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow21(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow22(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow23(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow24(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow25(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow26(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow27(CArtsInfo*, u8, int);
extern "C" void CArtsInfo_FmtRow2C(CArtsInfo*, u8, int);

extern "C" void CArtsInfo_UpdateArtsRow(CArtsInfo* self, u8 arg2, u8 arg3) {
    char buf[32]; // sprintf at +0x8
    char* s = BdatGetPtrDirect((const void*)self->field_0x50, (char*)"tagStr", arg2);
    sprintf(buf, (char*)"txt_para%02d", arg3 + 2);
    LayoutSetTextBoxFmtValue(self->mpLayout1, buf, s, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxFmtValue(self->mpLayout2, buf, s, 0);
    }
    u8 k = (u8)BdatGetU8Direct((const void*)self->field_0x50, (char*)"tagCALC", arg2);
    switch (k) {
    case 0x00: CArtsInfo_FmtRow00(self, arg2, arg3); break;
    case 0x01: CArtsInfo_FmtRow01(self, arg2, arg3); break;
    case 0x02: CArtsInfo_FmtRow02(self, arg2, arg3); break;
    case 0x03: CArtsInfo_FmtRow03(self, arg2, arg3); break;
    case 0x04: CArtsInfo_FmtRow04(self, arg2, arg3, 0); break;
    case 0x05: CArtsInfo_FmtRow05(self, arg2, arg3); break;
    case 0x06: CArtsInfo_FmtRow06(self, arg2, arg3); break;
    case 0x07: CArtsInfo_FmtRow07(self, arg2, arg3); break;
    case 0x08: CArtsInfo_FmtRow08(self, arg2, arg3); break;
    case 0x09: CArtsInfo_FmtRow09(self, arg2, arg3); break;
    case 0x0A: CArtsInfo_FmtRow0A(self, arg2, arg3); break;
    case 0x0B: CArtsInfo_FmtRow0B(self, arg2, arg3); break;
    case 0x0C: CArtsInfo_FmtRow0C(self, arg2, arg3); break;
    case 0x0D: CArtsInfo_FmtRow0D(self, arg2, arg3); break;
    case 0x0E: CArtsInfo_FmtRow0E(self, arg2, arg3); break;
    case 0x0F: CArtsInfo_FmtRow0F(self, arg2, arg3); break;
    case 0x10: CArtsInfo_FmtRow10(self, arg2, arg3); break;
    case 0x11: CArtsInfo_FmtRow11(self, arg2, arg3); break;
    case 0x12: CArtsInfo_FmtRow12(self, arg2, arg3); break;
    case 0x13: CArtsInfo_FmtRow13(self, arg2, arg3); break;
    case 0x14: CArtsInfo_FmtRow14(self, arg2, arg3); break;
    case 0x15: CArtsInfo_FmtRow15(self, arg2, arg3); break;
    case 0x16: CArtsInfo_FmtRow16(self, arg2, arg3); break;
    case 0x17: CArtsInfo_FmtRow17(self, arg2, arg3); break;
    case 0x18: CArtsInfo_FmtRow18(self, arg2, arg3); break;
    case 0x19: CArtsInfo_FmtRow19(self, arg2, arg3); break;
    case 0x1A: CArtsInfo_FmtRow1A(self, arg2, arg3); break;
    case 0x1B: CArtsInfo_FmtRow1B(self, arg2, arg3); break;
    case 0x1C: CArtsInfo_FmtRow1C(self, arg2, arg3); break;
    case 0x1D: CArtsInfo_FmtRow1D(self, arg2, arg3); break;
    case 0x1E: CArtsInfo_FmtRow1E(self, arg2, arg3); break;
    case 0x1F: CArtsInfo_FmtRow1F(self, arg2, arg3); break;
    case 0x20: CArtsInfo_FmtRow20(self, arg2, arg3); break;
    case 0x21: CArtsInfo_FmtRow21(self, arg2, arg3); break;
    case 0x22: CArtsInfo_FmtRow22(self, arg2, arg3); break;
    case 0x23: CArtsInfo_FmtRow23(self, arg2, arg3); break;
    case 0x24: CArtsInfo_FmtRow24(self, arg2, arg3); break;
    case 0x25: CArtsInfo_FmtRow25(self, arg2, arg3); break;
    case 0x26: CArtsInfo_FmtRow26(self, arg2, arg3); break;
    case 0x27: CArtsInfo_FmtRow27(self, arg2, arg3); break;
    case 0x28: CArtsInfo_FmtRow04(self, arg2, arg3, (u8)(self->field_0x55 + 1)); break;
    case 0x29: CArtsInfo_FmtRow04(self, arg2, arg3, (u8)(self->field_0x55 + 2)); break;
    case 0x2A: CArtsInfo_FmtRow04(self, arg2, arg3, (u8)(self->field_0x55 + 3)); break;
    case 0x2B: break;
    case 0x2C: CArtsInfo_FmtRow2C(self, arg2, arg3); break;
    default: break;
    }
}

// CArtsInfo_FmtRow00 - arts info text update. Formats a computed percentage
// (base value scaled by the arts-skill lookups CArtsInfo_GetEquipValue(0x2d) and
// CArtsInfo_GetSkill5C) into buf1, post-processes it (SjisDotToCommaEU +
// CArtsInfo_DotToCommaDE), then pushes it onto both layouts (layout 2 only when
// field_0x56 < 0xA). The base is a u8-keyed lookup scaled by
// lbl_eu_806686A4 when non-zero; the skill values are s32->float converted
// with MWCC's xoris magic.
void CArtsInfo_FmtRow00(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"cast", self->field_0x55);
    float d = (float)r;
    if (r != 0) {
        d = d * 0.1f;
    }
    // f = d - scale2 * (d * (float)x) - scale2 * (d * (float)y), each step
    // folded through fnmsubs (retail: fmuls d*(float)x; fnmsubs f30, scale2,
    // f1, f31).
    float f = d - 0.01f * (d * (float)CArtsInfo_GetEquipValue(self, 0x2d));
    f -= 0.01f * (d * (float)CArtsInfo_GetSkill5C(self));
    sprintf(buf1, (char*)"%s%.1f%s", s1, f, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow01 - arts info text update. Reads the character-data category
// (CtrlObjectParam_GetCurrentRowKey), discards a 0x24c-keyed lookup, then formats
// v - 0.5 * (level-1) * v * w (float arithmetic via MWCC's u32/s32
// conversion magics: clrlwi + 2^52 for the u8 lookups, xoris + 2^52+2^31
// for the signed (level-1)) into buf1, post-processes it, pushes it onto
// layout 1, then on level-up (< 0xA) formats the next-level value
// (level instead of level-1) into buf3 for layout 2 and runs the
// colour-pair helper when the raw lookup v differs from the next value.
void CArtsInfo_FmtRow01(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x48
    char buf2[32]; // sprintf at +0x28
    char buf3[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    CtrlObjectParam_GetCurrentRowKey(CtrlObjectParam_GetCharEntry(self->field_0x54));
    BdatGetU8ByTableKey((char*)"pc_arts", (char*)"idx", self->field_0x55);
    u8 v = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"recast", self->field_0x55);
    u8 w = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"glow_recast", self->field_0x55);
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
    float cur = fv - 0.01f * ((float)(self->field_0x56 - 1) * (fv * fw));
    sprintf(buf1, (char*)"%s%.1f%s", s1, cur, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        // (int) cast: retail converts level with the s32 magic (xoris);
        // plain (float)u8 would use the shorter u32 magic.
        float nxt = fv - 0.01f * ((float)(int)self->field_0x56 * (fv * fw));
        sprintf(buf3, (char*)"%s%.1f%s", s1, nxt, s2);
        SjisDotToCommaEU(buf3);
        CArtsInfo_DotToCommaDE(self, arg2, buf3);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf3, 0);
        if (fv != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow02 - arts info text update. Same shape as CArtsInfo_FmtRow13 but the
// lookup is BdatGetS8ByTableKey (s8 result): the value is abs()'d and narrowed back
// to s8 (retail: extsb after the abs call) for the sprintf vararg.
// Entry param shadows: declaring arg2's shadow first asks MWCC to copy r4
// before r3 in the entry block (retail order).
// u32 const on arg2: prologue-scheduling lever (KB: const params altered
// entry-store order in matched siblings); retail copies r4 before r3.
// Residual 2-instr reg swap: entry param-copy emission order (retail births
// r4->r29 before r3->r27). Documented MWCC claim-order invariant - not
// source-controllable (see CArtsInfo_FmtRow09 / CArtsInfo_FmtRow20 notes).
void CArtsInfo_FmtRow02(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s8 r = BdatGetS8ByTableKey((char*)"pc_arts", (char*)"tp", self->field_0x55);
    sprintf(buf1, (char*)"%s%d%s", s1, (s8)abs(r), s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow03 - arts info text update. The first grid value (u16-keyed
// lookup BdatGetU16ByTableKey at 0x26d) is converted to float and scaled by
// lbl_eu_806686A4 when non-zero, then overridden by the field_0x55
// arts-type switch (0x68/0x69/0x70 -> fixed constants); the second grid
// value (u8-keyed lookup BdatGetU8ByTableKey at 0x275) is scaled the same way.
// The displayed value is base + second*(level-1), post-processed and pushed
// onto both layouts; on level-up the level grid is recomputed for layout 2
// and the colour-pair helper runs when the two values differ.
// Entry param-save-order wall (see CArtsInfo_FmtRow09).
void CArtsInfo_FmtRow03(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u16 v1 = BdatGetU16ByTableKey((char*)"pc_arts", (char*)"st_time", self->field_0x55);
    float f1 = (float)v1;
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_time", self->field_0x55);
    float f2 = (float)v2;
    if (f1 != lbl_eu_80668680) {
        float scaled = f1 * lbl_eu_806686A4;
        f1 = scaled;
    }
    switch (self->field_0x55) {
    case 0x68:
        f1 = 20.0f;
        break;
    case 0x69:
        f1 = 10.0f;
        break;
    case 0x70:
        f1 = 30.0f;
        break;
    default:
        break;
    }
    if (f2 != lbl_eu_80668680) {
        float scaled2 = f2 * lbl_eu_806686A4;
        f2 = scaled2;
    }
    float cur = f1 + f2 * (float)(self->field_0x56 - 1);
    sprintf(buf1, (char*)"%s%.1f%s", s1, cur, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = f1 + f2 * (float)(int)self->field_0x56;
        sprintf(buf1, (char*)"%s%.1f%s", s1, nxt, s2);
        SjisDotToCommaEU(buf1);
        CArtsInfo_DotToCommaDE(self, arg2, buf1);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow04 - arts info text update. Same shape as CArtsInfo_FmtRow05
// (stat + min(damage1, damage2) grids scaled by 0.5) but the grid lookups
// CArtsInfo_GetGridA/8023719C are keyed on the 4th parameter instead of 0 and
// the stat comes from CArtsInfo_GetStatId1C.
void CArtsInfo_FmtRow04(CArtsInfo* self, u8 arg2, int arg3, u8 arg4) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    CArtsInfo_GetSkill20C(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId1C(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, arg4);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, arg4);
    int m1 = CArtsInfo_CalcDmg52(self);
    int m2 = CArtsInfo_CalcDmg53(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = RoundHalfAway0((float)(0.01f * (float)(g1 * s)));
    int t = m2 + stat;
    int v2 = RoundHalfAway0((float)(0.01f * (float)(g2 * t)));
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, (char*)"txt_value%02d_02", level);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, arg4);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, arg4);
        int v1n = RoundHalfAway0((float)(0.01f * (float)(g1n * s)));
        int v2n = RoundHalfAway0((float)(0.01f * (float)(g2n * t)));
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        sprintf(buf1, (char*)"%d%s%d", v1n, str2, v2n);
        sprintf(buf2, (char*)"txt_value%02d_02", level);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
    }
}

// CArtsInfo_FmtRow05 - arts info text update. Combines the stat id
// (CArtsInfo_GetStatId20) with the min of the two damage/level helpers
// (CArtsInfo_CalcDmg52/80237394): the two grids are 0.5*stat*(m+stat) products,
// formatted into buf1 (sprintf 0x282). On level-up the level+1 grids are
// recomputed for layout 2 and the colour-pair helper always runs.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (retail-identical schedule: lis 0x4330 hoisted into the prologue, one
// stack-slot pair per conversion site).
void CArtsInfo_FmtRow05(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    int level;
    CArtsInfo_GetSkill20C(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId20(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    int m1 = CArtsInfo_CalcDmg52(self);
    int m2 = CArtsInfo_CalcDmg53(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = RoundHalfAway0(0.01f * (float)(g1 * s));
    int t = m2 + stat;
    int v2 = RoundHalfAway0(0.01f * (float)(g2 * t));
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, (char*)"txt_value%02d_02", level);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(0.01f * (float)(g1n * s));
        int v2n = RoundHalfAway0(0.01f * (float)(g2n * t));
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        sprintf(buf1, (char*)"%d%s%d", v1n, str2, v2n);
        sprintf(buf2, (char*)"txt_value%02d_02", level);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
    }
}

// CArtsInfo_FmtRow06 - arts info text update. Combines the stat id
// (CArtsInfo_GetStatId1C) with the min of the two damage/level helpers
// (CArtsInfo_CalcDmg52/80237394): the two grids are 0.5*stat*(m+stat) products
// (RoundHalfAway0), then both grids are scaled by a byte-keyed factor
// (BdatGetU8ByTableKey at 0x289) and get the 0x49-keyed arts lookup
// (CArtsInfo_GetEquipValue) contribution added: value = v + 0.5*v*CArtsInfo_GetEquipValue(0x49).
// Two BdatGetU16ByTableKey lookups (0x26d/0x290) are called with results discarded.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (same retail schedule as CArtsInfo_FmtRow05: lis 0x4330 hoisted into the
// prologue, one stack-slot pair per conversion site).
void CArtsInfo_FmtRow06(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    CArtsInfo_GetSkill20C(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId1C(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    int m1 = CArtsInfo_CalcDmg52(self);
    int m2 = CArtsInfo_CalcDmg53(self);
    if (m1 > m2) m1 = m2;
    int s = m1 + stat;
    int v1 = RoundHalfAway0(0.01f * (float)(g1 * s));
    int t = m2 + stat;
    int v2 = RoundHalfAway0(0.01f * (float)(g2 * t));
    u8 v3 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    BdatGetU16ByTableKey((char*)"pc_arts", (char*)"st_time", self->field_0x55);
    BdatGetU16ByTableKey((char*)"pc_arts", (char*)"st_itv", self->field_0x55);
    v1 = RoundHalfAway0(0.01f * (float)(v1 * v3));
    int a = CArtsInfo_GetEquipValue(self, 0x49);
    int value1 = v1 + RoundHalfAway0(0.01f * (float)(v1 * a));
    v2 = RoundHalfAway0(0.01f * (float)(v2 * v3));
    a = CArtsInfo_GetEquipValue(self, 0x49);
    int value2 = v2 + RoundHalfAway0(0.01f * (float)(v2 * a));
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", value1, str, value2);
    int level = arg3 + 2;
    sprintf(buf2, (char*)"txt_value%02d_02", level);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(0.01f * (float)(g1n * s));
        int v2n = RoundHalfAway0(0.01f * (float)(g2n * t));
        v1n = RoundHalfAway0(0.01f * (float)(v1n * v3));
        a = CArtsInfo_GetEquipValue(self, 0x49);
        int value1n = v1n + RoundHalfAway0(0.01f * (float)(v1n * a));
        v2n = RoundHalfAway0(0.01f * (float)(v2n * v3));
        a = CArtsInfo_GetEquipValue(self, 0x49);
        int value2n = v2n + RoundHalfAway0(0.01f * (float)(v2n * a));
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        sprintf(buf1, (char*)"%d%s%d", value1n, str2, value2n);
        sprintf(buf2, (char*)"txt_value%02d_02", level);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
    }
}

// CArtsInfo_FmtRow07 - arts info text update. Same stat+min(damage1,damage2)
// grid shape as CArtsInfo_FmtRow05 (CArtsInfo_GetStatId20 stat, CArtsInfo_GetGridA/8023719C
// offsets), then the grids are scaled by a field_0x55 arts-type base
// (BdatGetU8ByTableKey at 0x289, overridden by the 0x68/0x69/0x70 switch to fixed
// 0x28/0x3c/0x64). A BdatGetU16ByTableKey lookup at 0x297 picks the arts lookup
// key (CArtsInfo_GetEquipValue) used to add 0.5*v*lookup to each grid: 0x66 -> 0x49,
// 0x67 -> 0x9, 0x68 -> 0x3a, 0x69 -> 0x5c. On level-up the level+1 grids
// are recomputed for layout 2 (re-dispatching the 0x297 key switch; the
// 0x289 base is kept in a register across the branch) and the colour-pair
// helper always runs.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (same retail schedule as CArtsInfo_FmtRow06: lis 0x4330 hoisted into the
// prologue, one stack-slot pair per conversion site).
// u32 const on arg2: entry param-copy lever (retail copies r4 before r3).
void CArtsInfo_FmtRow07(CArtsInfo* self, u8 const arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    CArtsInfo_GetSkill20C(self, arg2);
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
    int v1 = RoundHalfAway0(0.01f * (float)(g1 * s));
    int t = m2 + stat;
    int v2 = RoundHalfAway0(0.01f * (float)(g2 * t));
    int base = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
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
    v1 = RoundHalfAway0(0.01f * (float)(v1 * base));
    v2 = RoundHalfAway0(0.01f * (float)(v2 * base));
    u32 k = BdatGetU16ByTableKey((char*)"pc_arts", (char*)"st_type", self->field_0x55);
    if ((u16)k == 0x66) {
        int a = CArtsInfo_GetEquipValue(self, 0x49);
        v1 += RoundHalfAway0(0.01f * (float)(v1 * a));
        a = CArtsInfo_GetEquipValue(self, 0x49);
        v2 += RoundHalfAway0(0.01f * (float)(v2 * a));
    } else if ((u16)k == 0x67) {
        int a = CArtsInfo_GetEquipValue(self, 0x9);
        v1 += RoundHalfAway0(0.01f * (float)(v1 * a));
        a = CArtsInfo_GetEquipValue(self, 0x9);
        v2 += RoundHalfAway0(0.01f * (float)(v2 * a));
    } else if ((u16)k == 0x68) {
        int a = CArtsInfo_GetEquipValue(self, 0x3a);
        v1 += RoundHalfAway0(0.01f * (float)(v1 * a));
        a = CArtsInfo_GetEquipValue(self, 0x3a);
        v2 += RoundHalfAway0(0.01f * (float)(v2 * a));
    } else if ((u16)k == 0x69) {
        int a = CArtsInfo_GetEquipValue(self, 0x5c);
        v1 += RoundHalfAway0(0.01f * (float)(v1 * a));
        a = CArtsInfo_GetEquipValue(self, 0x5c);
        v2 += RoundHalfAway0(0.01f * (float)(v2 * a));
    }
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", v1, str, v2);
    int level = arg3 + 2;
    sprintf(buf2, (char*)"txt_value%02d_02", level);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(0.01f * (float)(g1n * s));
        int v2n = RoundHalfAway0(0.01f * (float)(g2n * t));
        v1n = RoundHalfAway0(0.01f * (float)(v1n * base));
        v2n = RoundHalfAway0(0.01f * (float)(v2n * base));
        u32 k2 = BdatGetU16ByTableKey((char*)"pc_arts", (char*)"st_type", self->field_0x55);
        if ((u16)k2 == 0x66) {
            int a = CArtsInfo_GetEquipValue(self, 0x49);
            v1n += RoundHalfAway0(0.01f * (float)(v1n * a));
            a = CArtsInfo_GetEquipValue(self, 0x49);
            v2n += RoundHalfAway0(0.01f * (float)(v2n * a));
        } else if ((u16)k2 == 0x67) {
            int a = CArtsInfo_GetEquipValue(self, 0x9);
            v1n += RoundHalfAway0(0.01f * (float)(v1n * a));
            a = CArtsInfo_GetEquipValue(self, 0x9);
            v2n += RoundHalfAway0(0.01f * (float)(v2n * a));
        } else if ((u16)k2 == 0x68) {
            int a = CArtsInfo_GetEquipValue(self, 0x3a);
            v1n += RoundHalfAway0(0.01f * (float)(v1n * a));
            a = CArtsInfo_GetEquipValue(self, 0x3a);
            v2n += RoundHalfAway0(0.01f * (float)(v2n * a));
        } else if ((u16)k2 == 0x69) {
            int a = CArtsInfo_GetEquipValue(self, 0x5c);
            v1n += RoundHalfAway0(0.01f * (float)(v1n * a));
            a = CArtsInfo_GetEquipValue(self, 0x5c);
            v2n += RoundHalfAway0(0.01f * (float)(v2n * a));
        }
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        sprintf(buf1, (char*)"%d%s%d", v1n, str2, v2n);
        sprintf(buf2, (char*)"txt_value%02d_02", level);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
    }
}

// CArtsInfo_FmtRow08 - arts info text update. Same shape as CArtsInfo_FmtRow24 but the
// two lookups' results are combined into a grid offset (CArtsInfo_GetGridA
// pattern: first-entry + second-entry * (level - 1)), formatted, then on
// level-up the offset for the next level is formatted on layout 2 and the
// colour-pair helper runs when the two offsets differ.
void CArtsInfo_FmtRow08(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, (char*)"%s%d%s", s1, cur, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, (char*)"%s%d%s", s1, nxt, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow09 - arts grid percentage. Both string lookups are called but
// their results discarded; the stat id (CArtsInfo_GetStatId20) is multiplied by two
// grid offsets (CArtsInfo_GetGridA/8023719C keyed on the arts level), each scaled
// by 0.5 and converted via RoundHalfAway0, then formatted with the
// BdatTouchStringCell(0x32, 0x3d, 0x52) string into buf1. On level-up (< 0xA) the
// level+1 offsets are recomputed and formatted on layout 2 and the
// colour-pair helper always runs.
// Entry param-save-order wall: retail copies arg2 (r4) before self (r3);
// MWCC invariantly emits the param-1 (r3) copy first (same ABI-boundary
// artifact as CMenuShopSell MenuShopSellCopyData). Shadows kept for documentation.
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
void CArtsInfo_FmtRow09(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // level assigned late (after sprintf1) so the addi lands after the first
    // sprintf like retail's in-place addi r27, r26, 2.
    int level;
    CArtsInfo_GetSkill20C(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatId20(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    int v1 = RoundHalfAway0(0.01f * (float)(g1 * stat));
    int v2 = RoundHalfAway0(0.01f * (float)(g2 * stat));
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", v1, str, v2);
    level = arg3 + 2;
    sprintf(buf2, (char*)"txt_value%02d_02", level);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        int v1n = RoundHalfAway0(0.01f * (float)(g1n * stat));
        int v2n = RoundHalfAway0(0.01f * (float)(g2n * stat));
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        sprintf(buf1, (char*)"%d%s%d", v1n, str2, v2n);
        sprintf(buf2, (char*)"txt_value%02d_02", level);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
    }
}

// CArtsInfo_FmtRow0A - arts info text update. The stat value (CArtsInfo_GetStatF10) is
// scaled by 100 into cur, combined with a grid offset (v1*(level-1)+v2) and
// scaled again, then formatted into buf1. On level-up (< 0xA) the lookups are
// re-fetched and the level grid recomputed for layout 2; the colour-pair
// helper runs when the two formatted values differ.
// u32 const on arg2: entry param-copy lever - retail copies r4 (arg2)
// before r3 (self).
void CArtsInfo_FmtRow0A(CArtsInfo* self, u8 const arg2, int arg3) {
    // Declaration order = reverse frame-slot order (MWCC allocates
    // last-declared lowest): cvB@+0x58, cvA@+0x48, buf1@+0x28, buf2@+0x08.
    char buf2[32]; // sprintf at +0x8
    char buf1[32]; // sprintf at +0x28
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    int base = CArtsInfo_GetStatF10(self);
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    int cur = (int)(0.01f * ConvS32ToF64(base * 100));
    // Grid offset folded into the conversion expression: retail interleaves
    // the (level-1) multiply with the conversion store sequence. Note the
    // operand order: the SECOND lookup (v2) is multiplied by (level-1), the
    // FIRST (v1) is added.
    int val1 =
        (int)(0.01f * ConvS32ToF64((int)(cur * (v2 * (u32)(self->field_0x56 - 1) + v1))));
    sprintf(buf1, (char*)"%s%d%s", s1, val1, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u8 n1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
        u8 n2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
        // Next-level grid folded into the conversion like retail (second
        // lookup multiplied by level, first added).
        int val2 =
            (int)(0.01f * ConvS32ToF64((int)(cur * (n2 * (u32)self->field_0x56 + n1))));
        sprintf(buf1, (char*)"%s%d%s", s1, val2, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (val1 != val2) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow0B - arts info text update. The stat value (CArtsInfo_GetStatF10,
// (int)float at +0x10) is scaled by 100 then halved into x1; the two grids
// are x1*g1 and x1*g2, each halved, formatted into buf1 (sprintf 0x282). On
// level-up the level+1 grids are recomputed for layout 2 and the colour-pair
// helper always runs.
// NOTE: the (float)(int) casts intentionally use MWCC's plain-cast form
// (retail-identical instruction schedule and compiler-temp slot placement);
// the cast magic pools as TU-local @8117 instead of lbl_eu_80668698 - see
// the pool_patterns note at the lbl_eu_80668698 definition.
// Entry param-save-order wall (same as CArtsInfo_FmtRow09): retail copies arg2
// (r4) before self (r3); MWCC invariantly emits the param-1 (r3) copy first.
void CArtsInfo_FmtRow0B(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // level assigned late (after sprintf1) so the addi lands after the first
    // sprintf like retail's in-place addi r27, r26, 2.
    int level;

    // Results discarded: retail keeps both calls but never uses the strings.
    CArtsInfo_GetSkill20C(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    int stat = CArtsInfo_GetStatF10(self);
    int g1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    int g2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    // (float)(int) casts emit MWCC's 0x4330-magic stack-slot conversion
    // (lis 0x4330 hoisted into the prologue, xoris'd low word per site).
    int x1 = RoundHalfAway0(0.01f * (float)(stat * 100));
    int x2 = RoundHalfAway0(0.01f * (float)(x1 * g1));
    int x3 = RoundHalfAway0(0.01f * (float)(x1 * g2));
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", x2, str, x3);
    level = arg3 + 2;
    sprintf(buf2, (char*)"txt_value%02d_02", level);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        int g1n = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        int g2n = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        // Dead call kept: retail emits the conversion+call and drops the result.
        RoundHalfAway0(0.01f * (float)(x1 * g1n));
        int x3n = RoundHalfAway0(0.01f * (float)(x1 * g2n));
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        // Retail formats x1 (not the dropped first conversion) as the middle value.
        sprintf(buf1, (char*)"%d%s%d", x1, str2, x3n);
        sprintf(buf2, (char*)"txt_value%02d_02", level);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
    }
}

// CArtsInfo_FmtRow0C - arts info text update. Same shape as CArtsInfo_FmtRow26: the
// two string lookups are discarded and the formatted string comes from
// BdatTouchStringCell(0x32, 0x3d, 0x52), but the two grid offsets come from the
// CArtsInfo_GetGridA/8023719C lookups keyed on the current arts level. On level-up
// (< 0xA) the offsets for level+1 are formatted on layout 2 and the
// colour-pair helper always runs.
void CArtsInfo_FmtRow0C(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    // Declaration order drives saved-reg allocation (first -> highest):
    // retail keeps level in r30, grid2 in r29, grid1 in r28 (sharing arg2).
    // level is assigned late (after sprintf1) so the addi lands after the
    // first sprintf like retail's in-place addi r30, r30, 2.
    int level;
    u32 grid1;
    u32 grid2;
    CArtsInfo_GetSkill20C(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    grid1 = CArtsInfo_GetGridA(self, self->field_0x56, 0);
    grid2 = CArtsInfo_GetGridB(self, self->field_0x56, 0);
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", grid1, str, grid2);
    // Eager intermediate forces the addi next to sprintf1 like retail
    // (addi r30, r30, 2).
    level = arg3 + 2;
    sprintf(buf2, (char*)"txt_value%02d_02", level);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        grid2 = CArtsInfo_GetGridA(self, self->field_0x56 + 1, 0);
        grid1 = CArtsInfo_GetGridB(self, self->field_0x56 + 1, 0);
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        sprintf(buf1, (char*)"%d%s%d", grid2, str2, grid1);
        sprintf(buf2, (char*)"txt_value%02d_02", level);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
    }
}

// CArtsInfo_FmtRow0D - arts info text update. Byte-identical body to
// CArtsInfo_FmtRow08 (same lookups, same grid-offset formatting, same level-up
// branch), kept as a separate retail symbol.
void CArtsInfo_FmtRow0D(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, (char*)"%s%d%s", s1, cur, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, (char*)"%s%d%s", s1, nxt, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow0E - arts info text update. Byte-identical body to
// CArtsInfo_FmtRow08 (same lookups, same grid-offset formatting, same level-up
// branch), kept as a separate retail symbol.
void CArtsInfo_FmtRow0E(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, (char*)"%s%d%s", s1, cur, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, (char*)"%s%d%s", s1, nxt, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow0F - arts info text update. Same shape as CArtsInfo_FmtRow08 but the
// grid offset is scaled by 10 (mulli 0xa) before formatting: the value is the
// arts AP cost rather than a raw grid index.
void CArtsInfo_FmtRow0F(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    u32 cur = (v1 + v2 * (self->field_0x56 - 1)) * 10;
    sprintf(buf1, (char*)"%s%d%s", s1, cur, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = (v1 + v2 * self->field_0x56) * 10;
        sprintf(buf1, (char*)"%s%d%s", s1, nxt, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow10 - arts info text update. The two byte lookups are converted
// to float; when the first is non-zero it is scaled by lbl_eu_806686A4, then
// the value scale*v2*(level-1) is added and formatted into buf1. On level-up
// (< 0xA) the level grid is recomputed for layout 2; the colour-pair helper
// runs when the two values differ.
// Entry param-save-order wall (see CArtsInfo_FmtRow09).
// fb declared (uninitialized) before fa: MWCC colours equal-lifetime FPRs
// in reverse declaration order, and retail keeps fb in f31 / fa in f30.
void CArtsInfo_FmtRow10(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
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
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    float fa = (float)v1;
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    fb = (float)v2;
    if (fa != lbl_eu_80668680) {
        float scaled = fa * lbl_eu_806686A4;
        fa = scaled;
    }
    float cur = fa + lbl_eu_806686A4 * (fb * (float)(self->field_0x56 - 1));
    sprintf(buf1, (char*)"%s%.1f%s", s1, cur, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = fa + lbl_eu_806686A4 * (fb * (float)(int)self->field_0x56);
        sprintf(buf1, (char*)"%s%.1f%s", s1, nxt, s2);
        SjisDotToCommaEU(buf1);
        CArtsInfo_DotToCommaDE(self, arg2, buf1);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow21 - arts info text update. Formats
// 1.0 + scale*(float)v1 + (float)v2*(level-1) into buf1, post-processes it
// and pushes it onto layout 1. On level-up (< 0xA) the level grid is
// recomputed (level instead of level-1, s32 magic) for layout 2; the
// colour-pair helper runs when the two values differ.
// Entry param-save-order wall (see CArtsInfo_FmtRow09).
// Int->float conversions: retail lfds the shared sdata2 correction constants
// (lbl_eu_806686A8 unsigned / lbl_eu_80668698 signed); the plain-cast forms
// reproduce retail's exact schedule but MWCC pools the magics as TU-local
// labels - needs a CArtsInfo.o pool_patterns postprocess rule.
void CArtsInfo_FmtRow21(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    float fa = (float)v1;
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    float fb = (float)v2;
    float cur = lbl_eu_80668684 + (lbl_eu_806686A4 * fa + fb * (float)(self->field_0x56 - 1));
    sprintf(buf1, (char*)"%s%.1f%s", s1, cur, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        float nxt = lbl_eu_80668684 + (lbl_eu_806686A4 * fa + fb * (float)(int)self->field_0x56);
        sprintf(buf1, (char*)"%s%.1f%s", s1, nxt, s2);
        SjisDotToCommaEU(buf1);
        CArtsInfo_DotToCommaDE(self, arg2, buf1);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}
// CArtsInfo_FmtRow11 - arts info text update. Formats the arts skill name and a
// byte-keyed table value (BdatGetU8ByTableKey) into buf1, the level string into
// buf2, then pushes both onto the layouts (layout 2 only when field_0x56
// < 0xA). Same shape as CArtsInfo_FmtRow18 but the 3rd sprintf arg is a lookup.
void CArtsInfo_FmtRow11(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    sprintf(buf1, (char*)"%s%d%s", s1, r, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow12 - arts info text update. Same shape as CArtsInfo_FmtRow11 (u8
// lookup at 0x2ab) but the displayed value is a percentage:
// scale * level + base (MWCC u32->float conversions via the 2^52 magic),
// rounded to unsigned. On level-up the level+1 percentage is formatted on
// layout 2 and the colour-pair helper runs when the two values differ.
// NOTE: retail lfds the 2^52 correction constants from the shared sdata2
// labels (lbl_eu_806686A8 unsigned / lbl_eu_80668698 signed); MWCC pools the
// plain-cast equivalents as TU-local @N labels - needs a CArtsInfo.o
// pool_patterns postprocess rule (values 43300000_00000000 / 43300000_80000000).
void CArtsInfo_FmtRow12(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val2", self->field_0x55);
    u32 cur = (u32)(lbl_eu_806686BC * (float)self->field_0x56 + (float)r);
    sprintf(buf1, (char*)"%s%d%s", s1, cur, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = (u32)(lbl_eu_806686BC * (float)(self->field_0x56 + 1) + (float)r);
        sprintf(buf1, (char*)"%s%d%s", s1, nxt, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow13 - arts info text update. Same shape as CArtsInfo_FmtRow11 but
// uses the s16-keyed lookup BdatGetS16ByTableKey (result sign-extended at the
// sprintf call site).
void CArtsInfo_FmtRow13(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey((char*)"pc_arts", (char*)"sp_val1", self->field_0x55);
    sprintf(buf1, (char*)"%s%d%s", s1, r, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow14 - arts info text update. Same shape as CArtsInfo_FmtRow13
// (s16-keyed lookup BdatGetS16ByTableKey, result sign-extended at the sprintf call
// site via the s16 vararg promotion).
void CArtsInfo_FmtRow14(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey((char*)"pc_arts", (char*)"sp_val1", self->field_0x55);
    sprintf(buf1, (char*)"%s%d%s", s1, r, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow15 - arts info text update. Formats the arts skill name and the
// doubled stat id (CArtsInfo_GetStatId20) into buf1, the level string into buf2,
// then pushes both onto the layouts (layout 2 only when field_0x56 < 0xA).
void CArtsInfo_FmtRow15(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    int id = CArtsInfo_GetStatId20(self);
    sprintf(buf1, (char*)"%s%d%s", s1, id << 1, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow16 - arts info text update. Same shape as CArtsInfo_FmtRow11
// (u8-keyed lookup BdatGetU8ByTableKey at 0x289, zero-extended at the sprintf
// call site via the u8 vararg promotion).
void CArtsInfo_FmtRow16(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    sprintf(buf1, (char*)"%s%d%s", s1, r, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow17 - arts info text update. Same shape as CArtsInfo_FmtRow27 but the
// converted double is scaled by lbl_eu_806686A4 when the s16 lookup is
// non-zero (retail: extsh. branch + lfs/fmuls).
void CArtsInfo_FmtRow17(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey((char*)"pc_arts", (char*)"sp_val1", self->field_0x55);
    // MWCC s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs, no frsp).
    float d = (float)r;
    if (r != 0) {
        float scaled_d = d * lbl_eu_806686A4;
        d = scaled_d;
    }
    sprintf(buf1, (char*)"%s%.1f%s", s1, d, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow18 - arts info text update. Formats two strings (arts skill
// names + level) and pushes them onto both layouts (layout 2 only when
// field_0x56 < 0xA).
// Shadowed locals keep the retail register mapping (arg2->r31, self->r28);
// the residual 2-instruction reg swap is the entry param-save order
// (retail copies r4->r31 before r3->r28; see CArtsInfo_FmtRow09). Witness-blocked
// (abi-boundary); plain -O4,p without the pragma breaks the stmw merge.
void CArtsInfo_FmtRow18(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    sprintf(buf1, (char*)"%s%d%s", s1, 5, CArtsInfo_GetSkillStr(self, arg2));
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow19 - arts info text update. Formats the skill name, a double
// constant and the second skill string into buf1, post-processes it
// (SjisDotToCommaEU) and hands it to CArtsInfo_DotToCommaDE before the usual
// level-string + dual-layout push (layout 2 only when field_0x56 < 0xA).
// CArtsInfo_FmtRow19 - arts info text update. Formats the skill name, a double
// constant and the second skill string into buf1, post-processes it
// (SjisDotToCommaEU) and hands it to CArtsInfo_DotToCommaDE before the usual
// level-string + dual-layout push (layout 2 only when field_0x56 < 0xA).
// Residual 2-instruction reg swap is the retail entry param-save order:
// retail copies r4 before r3; MWCC invariantly emits the param-1 (r3) copy
// first (same ABI-boundary artifact as CMenuShopSell MenuShopSellCopyData).
void CArtsInfo_FmtRow19(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%.1f%s", s1, lbl_eu_806686C0, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow1A - same shape as CArtsInfo_FmtRow18 with level constant 0x19.
void CArtsInfo_FmtRow1A(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%d%s", s1, 25, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow1B - arts info text update with fixed level 25 (same shape as
// CArtsInfo_FmtRow1A). Formats the arts skill name/level strings and pushes them
// onto both layouts (layout 2 only when field_0x56 < 0xA).
void CArtsInfo_FmtRow1B(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%d%s", s1, 25, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow1C - same shape as CArtsInfo_FmtRow1B with level constant 50.
void CArtsInfo_FmtRow1C(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%d%s", s1, 50, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow1D - same shape as CArtsInfo_FmtRow1B with level constant 100.
void CArtsInfo_FmtRow1D(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%d%s", s1, 100, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow1E - arts info text update. Same shape as CArtsInfo_FmtRow19 with
// a different format offset (0x2bb) and double constant (lbl_eu_806686C8).
void CArtsInfo_FmtRow1E(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%.2f%s", s1, lbl_eu_806686C8, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow1F - same shape as CArtsInfo_FmtRow1B with level constant 50.
// Entry param-save-order wall (see CArtsInfo_FmtRow09): retail saves arg2 (r31)
// before self (r28); MWCC invariantly emits the param-1 copy first.
void CArtsInfo_FmtRow1F(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%d%s", s1, 50, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow20 - same shape as CArtsInfo_FmtRow1B with level constant 2.
// Residual: retail spills arg2->r31 BEFORE self->r28; MWCC (size-mode, which
// is required for the retail stmw r28 frame) always copies r3 first. This is
// the documented param-save claim-order invariant (MWCC_CASES "VM-op/
// saved-param claim order", OPEN) - not source-controllable.
void CArtsInfo_FmtRow20(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%d%s", s1, 2, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow22 - arts info text update. Same shape as CArtsInfo_FmtRow11 with
// the u8-keyed lookup BdatGetU8ByTableKey at 0x2ab.
void CArtsInfo_FmtRow22(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 r = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val2", self->field_0x55);
    sprintf(buf1, (char*)"%s%d%s", s1, r, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow23 - arts info text update. Byte-identical body to
// CArtsInfo_FmtRow08 / CArtsInfo_FmtRow0E (same lookups, same grid-offset formatting,
// same level-up branch), kept as a separate retail symbol. Entry param-save-order
// artifact (unit-wide, ~40 dispatch handlers): retail copies arg2 (r4->r25) BEFORE
// self (r3->r31) at entry; our build invariantly emits the param-1 copy first. Same
// 2-op reg_swap present in CArtsInfo_FmtRow08/39AA0/916C/9EFC/A55C.
void CArtsInfo_FmtRow23(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    sprintf(buf1, (char*)"%s%d%s", s1, cur, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        sprintf(buf1, (char*)"%s%d%s", s1, nxt, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow24 - arts info text update. Two byte-keyed lookups
// (BdatGetU8ByTableKey at 0x289/0x29f) are called and their results discarded
// (retail keeps the bl's but never materializes the return values), then the
// arts level (field_0x56) is formatted. On level-up (< 0xA) the level+1
// string replaces buf1 on layout 2 and the colour-pair helper runs when the
// two level values differ.
void CArtsInfo_FmtRow24(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    u32 level = self->field_0x56;
    sprintf(buf1, (char*)"%s%d%s", s1, level, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = self->field_0x56 + 1;
        sprintf(buf1, (char*)"%s%d%s", s1, nxt, s2);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (level != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow25 - arts info text update. Same shape as CArtsInfo_FmtRow19 but the
// vararg double constant is loaded from a float (lfs) pool slot
// (lbl_eu_806686A0).
void CArtsInfo_FmtRow25(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    sprintf(buf1, (char*)"%s%.1f%s", s1, lbl_eu_806686A0, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow26 - arts info text update. The two string lookups
// (CArtsInfo_GetSkill20C/8023754C) are called but their results discarded; the
// formatted string comes from BdatTouchStringCell(0x32, 0x3d, 0x52) instead. Same
// grid-offset (v1 + v2*(level-1)) and level-up branch as CArtsInfo_FmtRow08, with
// the format at 0x282 and the level constant 5.
void CArtsInfo_FmtRow26(CArtsInfo* self, u8 arg2, int arg3) {
    CArtsInfo_GetSkill20C(self, arg2);
    CArtsInfo_GetSkillStr(self, arg2);
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    u8 v1 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"st_val", self->field_0x55);
    u8 v2 = BdatGetU8ByTableKey((char*)"pc_arts", (char*)"grow_st_val", self->field_0x55);
    u32 cur = v1 + v2 * (self->field_0x56 - 1);
    char* str = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
    sprintf(buf1, (char*)"%d%s%d", 5, str, cur);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        u32 nxt = v1 + v2 * self->field_0x56;
        char* str2 = BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 0x52);
        sprintf(buf1, (char*)"%d%s%d", 5, str2, nxt);
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
        if (cur != nxt) {
            PaneMatSetTevColorsByName(self->mpLayout2, buf2, (const GXColorS10*)&sArtsChangeColorA, (const GXColorS10*)&sArtsChangeColorB);
        }
    }
}

// CArtsInfo_FmtRow27 - arts info text update. Same shape as CArtsInfo_FmtRow19 but the
// vararg double is the s16-keyed lookup result (BdatGetS16ByTableKey at 0x2b3)
// converted via MWCC's s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs
// trick, no frsp).
void CArtsInfo_FmtRow27(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey((char*)"pc_arts", (char*)"sp_val1", self->field_0x55);
    // MWCC s32->f32 magic (retail: extsh + 0x4330/xoris/lfd/fsubs, no frsp).
    float d = (float)r;
    sprintf(buf1, (char*)"%s%.1f%s", s1, d, s2);
    SjisDotToCommaEU(buf1);
    CArtsInfo_DotToCommaDE(self, arg2, buf1);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_FmtRow2C - arts info text update. Same shape as CArtsInfo_FmtRow13 but the
// s16-keyed lookup BdatGetS16ByTableKey uses table offset 0x2c4.
void CArtsInfo_FmtRow2C(CArtsInfo* self, u8 arg2, int arg3) {
    char buf1[32]; // sprintf at +0x28
    char buf2[32]; // sprintf at +0x8
    char* s1 = CArtsInfo_GetSkill20C(self, arg2);
    char* s2 = CArtsInfo_GetSkillStr(self, arg2);
    s16 r = BdatGetS16ByTableKey((char*)"pc_arts", (char*)"sp_val2", self->field_0x55);
    sprintf(buf1, (char*)"%s%d%s", s1, r, s2);
    sprintf(buf2, (char*)"txt_value%02d_02", arg3 + 2);
    LayoutSetTextBoxText(self->mpLayout1, buf2, buf1, 0);
    if (self->field_0x56 < 0xA) {
        LayoutSetTextBoxText(self->mpLayout2, buf2, buf1, 0);
    }
}

// CArtsInfo_DotToCommaDE - German-language arts text post-processing. Only runs when
// the system language is 5 (German) and the row id for field_0x50 in the 0x20c table
// is 0x4f. Replaces '.' with ',' and skips the second byte of double-byte (Shift-JIS)
// characters so byte counts stay aligned. C linkage (retail symbol is unmangled; the
// extern "C" declaration is in the header).
void CArtsInfo_DotToCommaDE(CArtsInfo* self, u32 arg2, char* str) {
    u8 lang = CDeviceSC::getLanguage();
    if (lang != 5) return;

    u8 r = BdatGetU8Direct((const void*)self->field_0x50, (char*)"tagFS", arg2);
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

// CArtsInfo_Refresh - request a refresh once both BDAT tables and the primary
// layout have been loaded. The two request flags are the low bytes at +0x40
// and +0x48; retail stores the same literal 1 to both.
extern "C" void CArtsInfo_Refresh(CArtsInfo* self) {
    if (self->mpLayout1 == nullptr) {
        return;
    }
    if (self->field_0x4C == 0) {
        return;
    }
    if (self->field_0x50 == 0) {
        return;
    }
    self->field_0x48 = 1;
    self->field_0x40 = 1;
}

// Retail OnFileEvent: layout-archive loader + state-2 message handler.
// Branch 1 (event handle == field_0x14, i.e. the arts arc finished loading):
// creates the MEM2 region, attaches the arc to a fresh resource accessor,
// builds layout1 (+4 anims) and layout2 (+2 anims), binds the device font
// into both root panes, seeds panes from the language string, sets message
// texts, positions the message pane from the 'timg' resource coords,
// snapshots the two colour pairs into the sbss globals, builds the embedded
// cursor from a temp CCur18, runs the layout setup pass, then frees the
// region and returns 1.
// Branch 2 (event handle == field_0x18, the message bdat file): routes the
// loaded data into CBdat, resolves the two FP table entries named by the
// string table, calls the split1 reset stub, clears field_0x18, returns 1.
bool CArtsInfo::OnFileEvent(CEventFile* event) {
    if ((u32)field_0x14 == (u32)event->mFileHandle) {
        mMemRegion.createRegion((int)getHandleMEM2__Q23mtl10MemManagerFv(),
                                0x18000, (char*)"CArtsInfo", 0);
        Class_8045F858 host(&mMemRegion);
        // NOTE: accessor is re-read from field_0x1C at every use (retail
        // never caches it in a saved register).
        CFileHandle* fh = (CFileHandle*)field_0x14;
        void* arcData = fh->getData();
        setMemInitFlag__Q23mtl10MemManagerFb(false);
        field_0x1C = (int)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)field_0x1C)->Attach(arcData, (char*)"arc");

        buildLayout(&mpLayout1, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf.brlyt");
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans1, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf_in.brlan");
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans2, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf_info_in.brlan");
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans3, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf_lvup_out.brlan");
        bindLayoutAnimTransform(mpLayout1, &mpAnimTrans4, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf_lvup_in.brlan");

        nw4r::lyt::Pane* root1 = mpLayout1->GetRootPane();
        void* fontObj1 = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout1);
        ApplyFontToTxtPanes(root1, (const nw4r::ut::Font*)((u32)((IDeviceFontInfo*)fontObj1)->getFont()));

        char* lang = (char*)CUICfManager_getPackedFont9C();
        if (lang != 0) {
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_ap00", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_ap", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value02_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value03_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value04_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value05_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value06_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value07_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value08_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value09_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout1, (char*)"txt_value10_02", (u32)lang);
        }
        mpLayout1->SetAnimationEnable(mpAnimTrans2, false);
        mpLayout1->SetAnimationEnable(mpAnimTrans3, false);
        mpLayout1->SetAnimationEnable(mpAnimTrans4, false);
        mpLayout1->SetAnimationEnable(mpAnimTrans1, true);
        mpLayout1->Animate(0);

        // Fixed caption textboxes (message panes keyed by arts type).
        LayoutSetTextBoxFmtValue(mpLayout1, (char*)"txt_aptit",
                      BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 47), 0);
        LayoutSetTextBoxFmtValue(mpLayout1, (char*)"txt_rd_ap",
                      BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 23), 0);
        LayoutSetTextBoxFmtValue(mpLayout1, (char*)"txt_slct01",
                      BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 49), 0);
        LayoutSetTextBoxFmtValue(mpLayout1, (char*)"txt_slct02",
                      BdatTouchStringCell((char*)"MNU_battle", (char*)"name", 50), 0);
        char* closeText = BdatTouchStringCell((char*)"MNU_kyeassign", (char*)"help", 43);
        LayoutSetTextBoxFmtValue(mpLayout1, (char*)"txt_close00", closeText, 0);

        // Message-detail textbox: pane name depends on the CFGameManager
        // query result.
        const char* detailPane = (cf::CfGameManager::isClassicController(-1) != 0)
                                     ? (char*)"fileID_2"
                                     : (char*)"fileID_1";
        // Retail masks the lookup result to its low half and feeds it through
        // MakeTplNameSysFile to get the message resource name.
        char* resName = MakeTplNameSysFile(
            BdatGetU16ByTableKey((char*)"MNU_kyeassign", detailPane, 43));
        void* timg = ((nw4r::lyt::ArcResourceAccessor*)field_0x1C)->GetResource(0x74696d67, resName, 0);
        if (timg != 0) {
            CArtsMsgObj* msg = (CArtsMsgObj*)timg;
            LayoutSetTextBoxFmtValue(mpLayout1, (char*)"pic_btn00", (char*)timg, 0);
            PaneSetTexPaletteByName(mpLayout1, (char*)"pic_btn00", (TPLPalette*)((u32)msg->chain));
            nw4r::lyt::Pane* msgPane1 =
                root1->FindPaneByName((char*)"pic_btn00", true);
            if (msgPane1 != 0) {
                u16 row = msg->chain->pCoords->row;
                u16 col = msg->chain->pCoords->col;
                msgPane1->SetSize(nw4r::lyt::Size((float)(int)row, (float)(int)col));
            }
        }

        buildLayout(&mpLayout2, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf.brlyt");
        bindLayoutAnimTransform(mpLayout2, &mpAnimTrans5, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf_lvup_in.brlan");
        bindLayoutAnimTransform(mpLayout2, &mpAnimTrans6, (nw4r::lyt::ArcResourceAccessor*)field_0x1C, (char*)"mf15_arts30_set_inf_lvup.brlan");

        void* fontObj2 = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mpLayout2);
        ApplyFontToTxtPanes(mpLayout2->GetRootPane(),
                      (const nw4r::ut::Font*)((u32)((IDeviceFontInfo*)fontObj2)->getFont()));

        if (lang != 0) {
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value02_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value03_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value04_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value05_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value06_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value07_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value08_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value09_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value10_02", (u32)lang);
            setLayoutTextBoxFont(mpLayout2, (char*)"txt_value11_02", (u32)lang);
        }
        mpLayout2->SetAnimationEnable(mpAnimTrans6, false);
        mpLayout2->SetAnimationEnable(mpAnimTrans5, true);
        mpLayout2->Animate(0);

        // Detail panes on layout2: hide each extra pane; the message pane's
        // anchor triple is rotated before it is hidden.
        setPaneVisible((mpLayout2->GetRootPane())->FindPaneByName((char*)"nul_inf", true), 0);
        nw4r::lyt::Pane* pn1028 = (mpLayout2->GetRootPane())->FindPaneByName((char*)"pic_bl", true);
        if (pn1028 != 0) {
            const nw4r::math::VEC3& pos = pn1028->GetTranslate();
            float x = pos.x;
            float y = pos.y;
            float z = pos.z;
            pn1028->SetSRTElement(0, z);
            pn1028->SetSRTElement(1, x);
            pn1028->SetSRTElement(2, y);
        }
        setPaneVisible((mpLayout2->GetRootPane())->FindPaneByName((char*)"nul_arw", true), 0);
        setPaneVisible((mpLayout2->GetRootPane())->FindPaneByName((char*)"txt_aptit", true), 0);
        setPaneVisible((mpLayout2->GetRootPane())->FindPaneByName((char*)"txt_ap", true), 0);
        setPaneVisible((mpLayout2->GetRootPane())->FindPaneByName((char*)"nul_meswin", true), 0);
        nw4r::lyt::Pane* pn238 = (mpLayout2->GetRootPane())->FindPaneByName((char*)"txt_value02_02", true);
        setPaneVisible(pn238, 0);

        // Snapshot the message pane's two vertex-colour pairs into the sbss
        // colour globals.
        GXColorS10 col1 = PaneMatGetS16Quad(pn238, 0);
        CopyVec4s(&sArtsPaneTevColorA, &col1);
        GXColorS10 col2 = PaneMatGetS16Quad(pn238, 1);
        CopyVec4s(&sArtsPaneTevColorB, &col2);

        // Swap the last halfword of the two cached colour records (retail
        // exchanges element [3] between sArtsPaneTevColorA/64750 via sda21).
        s16 sw = *(s16*)((u8*)&sArtsPaneTevColorA + 6);
        *(s16*)((u8*)&sArtsPaneTevColorA + 6) = *(s16*)((u8*)&sArtsPaneTevColorB + 6);
        *(s16*)((u8*)&sArtsPaneTevColorB + 6) = sw;

        // Build a temporary cursor against the shared accessor and copy its
        // body over the embedded one (vptr at +0 preserved; word copies).
        CCur18 tmpCursor;
        __ct__6CCur18FPQ34nw4r3lyt19ArcResourceAccessor(&tmpCursor, CUICfManager_getArcResourceAccessor());
        mCursor = tmpCursor;
        __dt__6CCur18Fv(&tmpCursor, -1);
        mCursor.initLayout();
        CArtsInfo_BindAllPanes(this);
        CArtsInfo_Refresh(this);
        field_0x14 = 0;
        mMemRegion.validateHeap();
        return 1;
    }

    if ((u32)field_0x18 == (u32)event->mFileHandle) {
        CFileHandle* fh2 = (CFileHandle*)field_0x18;
        void* data = fh2->getData();
        CBdat::setBdatEntry(2, data);
        Bdat_GetTable_AA34();
        field_0x4C = (int)getFP((char*)"MNU_arts");
        Bdat_GetTable_AA34();
        field_0x50 = (int)getFP((char*)"MNU_arts_tag");
        // split1 refresh notification (retail bl at +0x678c)
        CArtsInfo_Refresh(this);
        field_0x18 = 0;
        return 1;
    }
    return 0;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static constructor: initialise the arts font/colour small-data entries
// (same shape as CItemBoxInfo's sinit_801EABC4).
void sinit_8023BC8C() {
    SplitU32ToS16s(&sArtsPaneTevColorA, 0);
    SplitU32ToS16s(&sArtsPaneTevColorB, 0);
    setGXColorS10((GXColorS10*)&sArtsChangeColorA, 0xff, 0xff, 0xfa, 0);
    setGXColorS10((GXColorS10*)&sArtsChangeColorB, 0x25, 0x8a, 0xce, 0);
}
