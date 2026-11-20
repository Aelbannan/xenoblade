#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/makecrystal/CMCCrystalInfo.hpp"
#include "kyoshin/makecrystal/CMCCrystalData.hpp"
#include "kyoshin/makecrystal/CMCCrystalParamTable.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
class CBaseCur;
class CEventFile;

namespace nw4r { namespace lyt { class AnimTransform; class ArcResourceAccessor; class Layout; class Pane; } }

// Pane +0x2C is the protected mTranslate VEC3 (lyt_pane.h); mirror it so the
// crystal-box refresh (updateCrystalBoxRows) can hand the field to copyVEC3.
struct PaneTranslateMirror {
    f32 mX;   // +0x2C
    f32 mY;   // +0x30
    f32 mZ;   // +0x34
};

// SDA21 float constants referenced by the anim-completion callbacks (retail
// symbol names; the sentinel is the completion-check value passed to
// advanceAnimTransform / AnimRewindFrame).
extern const float lbl_eu_80668470;  // anim sentinel constant used by completion checks
extern float lbl_eu_8066845C;
extern float lbl_eu_80668474;
extern float lbl_eu_80668478;

// BDAT table pointer (sda21) and sdata2 constants used by the crystal-box
// refresh callbacks (setCrystalSlotTexture / setCrystalSlotCapacity / updateCrystalBoxRows).
extern char* lbl_eu_806640EC;     // BDAT table pointer (sda21)
extern double lbl_eu_80668460;    // 2^52 double conversion constant (unsigned path)
extern double lbl_eu_80668468;    // 2^52 + 2^31 double conversion constant (signed path)
extern float lbl_eu_8066847C;



// Transient (crystal id, quantity) pair of the aggregation loops
// (CMCCrystalSelTable::buildIdList, getCrystalSelTopCount). set is emitted
// out of line after its first user (retail 0x8021BB68).
class CMCCrystalAggPair {
public:
    CMCCrystalAggPair(u16 id = 0, u16 count = 0) : id(id), count(count) {}
    void set(const CMCCrystalAggPair& other) {
        id = other.id;
        count = other.count;
    }

    u16 id;     // +0x00
    u16 count;  // +0x02
};

// {id, quantity, flag} record of the crystal-grid aggregation
// (rebuildCrystalGridTable). Copy ctor and set are emitted out of line after
// that function (retail 0x8021AE2C / 0x8021AE48).
class CMCCrystalGridRec {
public:
    CMCCrystalGridRec(u16 id = 0, u16 count = 0, u8 flag = 0) : m0(id), m2(count), m4(flag) {}
    CMCCrystalGridRec(const CMCCrystalGridRec& other) : m0(other.m0), m2(other.m2), m4(other.m4) {}
    void set(const CMCCrystalGridRec& other) { *this = other; }

    u16 m0;  // +0x00 crystal id
    u16 m2;  // +0x02 quantity
    u8  m4;  // +0x04 selected-crystal flag
};




// View into the object returned by CDeviceFont::getFontInfo: vtable+0x24
// (index 7, no args) yields the u32 bound into the layout's font pane
// (handed to ApplyFontToTxtPanes). All-pure so no vtable is emitted.
class CDeviceFontVtblView {
public:
    virtual void vf0() = 0; // index 0 -> +0x08
    virtual void vf1() = 0; // index 1 -> +0x0C
    virtual void vf2() = 0; // index 2 -> +0x10
    virtual void vf3() = 0; // index 3 -> +0x14
    virtual void vf4() = 0; // index 4 -> +0x18
    virtual void vf5() = 0; // index 5 -> +0x1C
    virtual void vf6() = 0; // index 6 -> +0x20
    virtual u32 vf7() = 0;  // index 7 -> +0x24
};

// Pane +0x4C/+0x50 float pair written by OnFileEvent from the timg-resource
// u16 position header (same +0x4C offset TagCopyVec2f reads as a drag pair).
struct PanePosMirror {
    u8 _pad[0x4C];   // +0x00..+0x4B
    f32 m4C;         // +0x4C
    f32 m50;         // +0x50
};



// Placed-item selection table at CMCCrystalBox+0x1480 (retail ctor
// 0x80215CE4, dtor 0x80215D24): 8 item pointers, the placement order, and the
// aggregated crystal-id list built from them.
class CMCCrystalSelTable {
public:
    CMCCrystalSelTable() : mCount(0), mTop(0), mIdCount(0) {
        for (u8 i = 0; i < 8; i++) {
            mItems[i] = 0;
            mOrder[i] = -1;
        }
    }
    ~CMCCrystalSelTable() {}

    unsigned long getItem(unsigned int idx);
    int getSelectEntry();
    void release();
    void buildIdList();

    unsigned long mItems[8];   // +0x00 placed item records
    u8  mCount;                // +0x20
    s8  mOrder[8];             // +0x21 placement order (item slot per step)
    u8  mTop;                  // +0x29 placement steps
    u16 mIds[0x20];            // +0x2A aggregated crystal ids
    u8  mIdCount;              // +0x6A
};

// Crystal-box list widget of the make-crystal window (retail vtable
// lbl_eu_80535C60 -> __vt__13CMCCrystalBox: IWorkEvent with ~CMCCrystalBox and
// OnFileEvent overridden; RTTI base list names only IWorkEvent).
class CMCCrystalBox : public IWorkEvent {
public:
    CMCCrystalBox(u8 parentType);
    virtual ~CMCCrystalBox();
    virtual bool OnFileEvent(CEventFile* event);

    void refreshCrystalBoxName();
    void setCrystalNameLabel();
    void initCrystalBoxResources();
    void updateCrystalBoxState();
    void drawCrystalBox(nw4r::lyt::DrawInfo* di);
    void teardownCrystalBox();
    int isCrystalBoxReady();
    int getCrystalBoxActive();
    u8 isCrystalBoxBusy();
    int getCrystalBoxSysWin();
    void openCrystalBox();
    void enterCrystalBoxState4();
    void enterCrystalBoxState6();
    void enterCrystalBoxState8();
    void enterCrystalBoxState13();
    void stepCrystalBoxCursorPrev();
    void stepCrystalBoxCursorNext();
    void stepCrystalBoxPageBack();
    void pageCrystalBoxCursorFwd();
    void confirmCrystalSelect();
    void confirmCrystalUnselect();
    void toggleCrystalSortMenu();
    void advanceCrystalBoxSortMenu(int a);
    CMCCrystalParamTable* rebuildCrystalBoxSelTable();
    u8 getCrystalBoxKindA();
    u8 getCrystalBoxKindB();
    void tickCrystalBoxPageFwd();
    void tickCrystalBoxPageBack();
    void refreshCrystalPagePair(int dir);
    void initCrystalPageLabels();
    void updateCrystalBoxSysWin();
    int getCrystalBoxAction();
    void deselectCrystalBoxItem();
    void onCrystalAnimToState2();
    void onCrystalAnimToState3();
    void onCrystalAnimToState5();
    void onCrystalAnimToIdle();
    void onCrystalAnim14Idle();
    void onCrystalAnimToState9();
    void onCrystalAnim9ToState3();
    void onCrystalAnim11State3();
    void onCrystalAnim12State3();
    void onCrystalAnim13State3();
    void onCrystalAnimToState14();
    void onCrystalAnimResetState3();
    void loadCrystalBoxArchive();
    void setCrystalSlotTexture(u16 id, unsigned int* item, u8 i);
    void setCrystalSlotTexAlt(u16 id, unsigned int* item, u8 i);
    void setCrystalSlotCapacity(u16 id, unsigned int* item, u8 i);
    void setCrystalSlotCapAlt(u16 id, unsigned int* item, u8 i);
    void updateCrystalBoxRows();
    void rebuildCrystalSortMenu();
    void placeCrystalBoxSlots();
    void refreshCrystalBoxCursors();
    void rebuildCrystalGridTable();
    void playCrystalAnim6();
    void playCrystalAnim7();
    void playCrystalAnim8();
    void playCrystalAnim9();
    void playCrystalAnim10();
    void playCrystalAnim11();
    void playCrystalAnim12();
    void playCrystalAnim13();
    void playCrystalAnim14();
    void showCrystalBoxStatus();
    s32 getCrystalSelTopCount();

    UnkClass_8045F564 mMemRegion1;      // +0x04
    UnkClass_8045F564 mMemRegion2;      // +0x14
    CFileHandle* mFileHandle0;          // +0x24 sort-menu archive
    CFileHandle* mFileHandle1;          // +0x28 crystal-info archive
    CFileHandle* mFileHandle2;          // +0x2C common archive
    nw4r::lyt::ArcResourceAccessor* mArcAcc0; // +0x30
    nw4r::lyt::ArcResourceAccessor* mArcAcc1; // +0x34
    nw4r::lyt::Layout* mLayout;         // +0x38
    nw4r::lyt::AnimTransform* mAnim0;   // +0x3C
    nw4r::lyt::AnimTransform* mAnim1;   // +0x40
    nw4r::lyt::AnimTransform* mAnim2;   // +0x44
    nw4r::lyt::AnimTransform* mAnim3;   // +0x48
    nw4r::lyt::AnimTransform* mAnim4;   // +0x4C
    nw4r::lyt::AnimTransform* mAnim5;   // +0x50
    nw4r::lyt::AnimTransform* mAnim6;   // +0x54
    nw4r::lyt::AnimTransform* mAnim7;   // +0x58
    nw4r::lyt::AnimTransform* mAnim8;   // +0x5C
    u8 unk60;                           // +0x60
    s32 unk64;                          // +0x64: state/status
    u8 unk68;                           // +0x68
    u8 unk69;                           // +0x69: active flag
    CCur07 mCur07;                      // +0x6C
    CCur09 mCur09;                      // +0x84
    CCur18 mCur18A;                     // +0x9C
    CCur11 mCur11;                      // +0xB4
    CCur16 mCur16;                      // +0xCC
    CCur18 mCur18B;                     // +0xE4
    CSortMenu sortMenu;                 // +0xFC
    CMCCrystalInfo crystalInfo;         // +0x1EC
    CScrollBar mScrollBar;              // +0x250
    CSysWin sysWin;                     // +0x290
    u8 unk2CC;                          // +0x2CC
    s8 unk2CD;                          // +0x2CD
    u8 unk2CE;                          // +0x2CE
    u8 unk2CF;                          // +0x2CF
    u8 unk2D0;                          // +0x2D0: parent type
    u8 unk2D1;                          // +0x2D1
    u8 unk2D2;                          // +0x2D2
    u8 unk2D3;                          // +0x2D3
    u8 unk2D4;                          // +0x2D4: read by getCrystalBoxKindA
    u8 unk2D5;                          // +0x2D5: read by getCrystalBoxKindB
    CMCCrystalParamTable mParamTable;   // +0x2D6 selected-crystal table
    CMCCrystalData data;                // +0x3D8 crystal-state table
    CMCCrystalSelTable mSelTable;       // +0x1480 placed-item selection table
    u8 field_14EC[5];                   // +0x14EC..0x14F0 (indexed by field_14F2)
    u8 field_14F1;                      // +0x14F1
    u8 field_14F2;                      // +0x14F2: countdown ticker / index
    f32 field_14F4;                     // +0x14F4
    f32 field_14F8;                     // +0x14F8
    f32 field_14FC;                     // +0x14FC
    u8 unk1500;                         // +0x1500
    u8 unk1501;                         // +0x1501
    union {                             // +0x1502 (signed alias view)
        u8 unk1502;                     // +0x1502
        s8 unk1502s;                    // +0x1502 (signed)
    };
    u8 unk1503;                         // +0x1503
    u16 unk1504;                        // +0x1504
    u8 field_1506;                      // +0x1506
    u8 field_1507;                      // +0x1507
};

// Dispatch-only view of the item-implementation vtable (name getter at slot
// 0x20). Never instantiated; pure virtuals keep the vtable unemitted. Lets
// getCrystalItemName dispatch through r12 like retail instead of a cast call.
class CItemImplDispatch {
public:
    // MWCC puts two RTTI slots at vtable 0x00/0x04, so the first user
    // virtual is at 0x08. Six fillers + getName = slot 0x20 (retail name
    // getter).
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual char* getName(void* slot) = 0;   // vtable slot 0x20
};


// Item-implementation vtable view up to slot 0x64 (GetName/GetFlag) for the
// selection loop in rebuildCrystalBoxSelTable. Never instantiated; pure virtuals keep the
// vtable unemitted. Slot 0x4C = GetName (u16), slot 0x64 = GetFlag (u8).
class CItemImplFacade2 {
public:
    virtual u8 GetCount(void* item) = 0;      // slot 0x08
    virtual void vf_04() = 0;                 // 0x0C
    virtual void vf_08(void* item) = 0;       // 0x10
    virtual void vf_0C() = 0; virtual void vf_10() = 0;
    virtual void vf_14() = 0; virtual void vf_18() = 0;
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual void vf_24() = 0; virtual void vf_28() = 0;
    virtual void vf_2C() = 0; virtual void vf_30() = 0;
    virtual void vf_34() = 0; virtual void vf_38() = 0;
    virtual void vf_3C() = 0; virtual void vf_40() = 0;
    virtual void vf_44() = 0;                 // 0x48
    virtual u16 GetName(void* item, u8 idx) = 0;  // slot 0x4C
    virtual void vf_50() = 0; virtual void vf_54() = 0;
    virtual void vf_58() = 0; virtual void vf_5C() = 0;
    virtual void vf_60() = 0;                 // 0x60
    virtual u8 GetFlag(void* item, u8 idx) = 0;   // slot 0x64
};

// Aggregation-loop facade (buildCrystalBoxIdList): retail adds the slot-0x64 getter's
// result to the running count unmasked (raw 32-bit), while the slot-0x4C
// result is masked/tested at each use (u16). Never instantiated; pure
// virtuals keep the vtable unemitted.
class CItemImplFacadeAgg {
public:
    virtual u8 GetCount(void* item) = 0;      // slot 0x08
    virtual void vf_04() = 0;                 // 0x0C
    virtual void vf_08(void* item) = 0;       // 0x10
    virtual void vf_0C() = 0; virtual void vf_10() = 0;
    virtual void vf_14() = 0; virtual void vf_18() = 0;
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual void vf_24() = 0; virtual void vf_28() = 0;
    virtual void vf_2C() = 0; virtual void vf_30() = 0;
    virtual void vf_34() = 0; virtual void vf_38() = 0;
    virtual void vf_3C() = 0; virtual void vf_40() = 0;
    virtual u16 GetName(void* item, u8 idx) = 0;  // slot 0x4C
    virtual void vf_50() = 0; virtual void vf_54() = 0;
    virtual void vf_58() = 0; virtual void vf_5C() = 0;
    virtual void vf_60() = 0;                 // 0x60
    virtual u32 GetFlag(void* item, u8 idx) = 0;  // slot 0x64
};

// Item-implementation vtable view for the crystal-slot refresh callbacks
// (setCrystalSlotTexture / setCrystalSlotCapacity): slot 0x08 is the count getter (u32 raw;
// callers mask to u16/u8), slot 0x80 the boxed/u16 state getter. Never
// instantiated; pure virtuals keep the vtable unemitted.
class CItemImplFacade3 {
public:
    virtual u32 GetCount(void* item) = 0;     // slot 0x08
    virtual void vf_04() = 0; virtual void vf_08() = 0;
    virtual void vf_0C() = 0; virtual void vf_10() = 0;
    virtual void vf_14() = 0; virtual void vf_18() = 0;
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual void vf_24() = 0; virtual void vf_28() = 0;
    virtual void vf_2C() = 0; virtual void vf_30() = 0;
    virtual void vf_34() = 0; virtual void vf_38() = 0;
    virtual void vf_3C() = 0; virtual void vf_40() = 0;
    virtual void vf_44() = 0; virtual void vf_48() = 0;
    virtual void vf_4C() = 0; virtual void vf_50() = 0;
    virtual void vf_54() = 0; virtual void vf_58() = 0;
    virtual void vf_5C() = 0; virtual void vf_60() = 0;
    virtual void vf_64() = 0; virtual void vf_68() = 0;
    virtual void vf_6C() = 0; virtual void vf_70() = 0;
    virtual void vf_74() = 0;                 // 0x7C
    virtual u16 GetBoxed(void* item) = 0;     // slot 0x80
};

// Item-implementation vtable view for the 8x8 grid aggregation in
// rebuildCrystalGridTable: slot 0x4C returns the item name id (u16), slot 0x64 the
// quantity (returned unmasked as int). Never instantiated.
class CItemImplFacadeGrid {
public:
    virtual void vf_00() = 0;                 // slot 0x08
    virtual void vf_04() = 0; virtual void vf_08() = 0;
    virtual void vf_0C() = 0; virtual void vf_10() = 0;
    virtual void vf_14() = 0; virtual void vf_18() = 0;
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual u16 GetName(void* item, u8 idx) = 0;   // slot 0x4C
    virtual void vf_50() = 0; virtual void vf_54() = 0;
    virtual void vf_58() = 0; virtual void vf_5C() = 0;
    virtual void vf_60() = 0;                 // 0x60
    virtual int GetQuantity(void* item, u8 idx) = 0;   // slot 0x64
};

class CLytVf38 {   // virtual at vtable slot 0x38 (user slot 12)
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual void vf_18() = 0; virtual void vf_1C() = 0;
    virtual void vf_20() = 0; virtual void vf_24() = 0;
    virtual void vf_28() = 0; virtual void vf_2C() = 0;
    virtual void vf_30(int) = 0;   // slot 0x38
};
struct CrystalBoxSubObj5;
class CLytVf3C;
// Sub-object reached through CMCCrystalBox::subObjPtrs[5] (+0x38): its +0x10
// field is the layout whose pane-lookup virtual (slot 0x3C) the crystal-box
// refresh (updateCrystalBoxRows) dispatches through. Real member call keeps MWCC's
// lwz/lwz/lwz/mtctr/bctrl sequence (cast-calls add a scratch load).
struct CrystalBoxSubObj5 {
    u8 pad_00[0x10];    // +0x00
    CLytVf3C* layout;   // +0x10
};
class CLytVf3C {   // virtual at vtable slot 0x3C (user slot 13)
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual void vf_18() = 0; virtual void vf_1C() = 0;
    virtual void vf_20() = 0; virtual void vf_24() = 0;
    virtual void vf_28() = 0; virtual void vf_2C() = 0;
    virtual void vf_30() = 0;                    // user slot 12 = 0x38
    virtual nw4r::lyt::Pane* vf_3C(char* s, int flag) = 0;  // slot 0x3C
};
// 3-arg variant used by OnFileEvent: retail passes the double-deref of the
// timg-resource +8 field in r6 (extra arg), which also feeds the u16 reads.
class CLytVf3C3 {
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual void vf_18() = 0; virtual void vf_1C() = 0;
    virtual void vf_20() = 0; virtual void vf_24() = 0;
    virtual void vf_28() = 0; virtual void vf_2C() = 0;
    virtual void vf_30() = 0;                    // user slot 12 = 0x38
    virtual nw4r::lyt::Pane* vf_3C(char* s, int flag, u32* extra) = 0;  // slot 0x3C
};

// C-linkage imports
// -----------------
// Retail symbols with C linkage (unmangled names, per retail relocs).
// Previously declared inline in each TU as pseudo-imports; kept here as real
// imports. The names ARE the retail symbol names - keep verbatim.
// Returns the destination pointer (retail reuses it as a call argument).
extern "C" int func_8021A8EC(void*);
extern "C" char sMakeCrystalStrings[];
extern "C" unsigned long lbl_eu_80508870[];

extern unsigned long lbl_eu_806646D0;
// Selection-table helpers on the +0x1480 box-head scratch (not CMCCrystalBox).
// Crystal-slot refresh callbacks (defined in this TU).
// Item-kind check (defined in this TU); noinline keeps the bl in the slot
// refresh callbacks instead of inlining the two-word test.
extern "C" __declspec(noinline) int isCrystalKindGem(void*);
// Message-resource / text helpers shared with the other kyoshin menu TUs.
extern "C" void copyVEC3(void*, const void*);

extern "C" void MCCrystal_ShowItem(void*, unsigned short, void*);
extern "C" void MCCrystal_ClearSlots(void*);   // crystal-info grid refresh (CMCCrystalInfo.cpp)
extern "C" void MCCrystal_FillSlot(void*, unsigned char, unsigned short, unsigned short, unsigned char); // set crystal slot
extern "C" int IsSkillItem(void*);

// comes from kyoshin/cf/CfGameManager.hpp; call sites cast the item pointer.
extern "C" u16 CItemBlock_getFlag120EC(int);
// OnFileEvent (code_80135FDC-unit) imports: unmangled retail symbols keep C
// linkage; getPackedFont uses the plain C++ form that re-derives the retail
// mangled name (cf::CfGameManager::isClassicController comes from its header).
// C++ linkage so MWCC mangles to the retail getPackedFont__Fv.
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
extern "C" void* Bdat_GetTable_AA34();   // matches code_801862C0.hpp's declaration
extern "C" float lbl_eu_80668488;
extern "C" float lbl_eu_8066848C;
extern "C" float lbl_eu_80668490;
extern "C" void* CItem_kindPtr(u32, s16);
extern "C" u32 CItemBlock_countKindSlots(u8);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);

// State-machine anim-completion callbacks (defined in this TU; retail symbols
// are unmangled). extern "C" first declaration keeps the switch call relocs
// unmangled like retail; noinline keeps the bodies out-of-line.

// retail masks the u16 crystal id into the argument register (clrlwi 16), so
// this TU saw a u16 parameter. Literal mangled identifier links to the same
// symbol as setLayoutTextBoxNumber.
extern void* lbl_eu_80664090;   // BDAT table pointer (sda21; CfGameManager.hpp type)
extern float lbl_eu_80668480;
extern float lbl_eu_80668484;

// Selection/refresh helpers for the update callbacks and the ctor copy-init.
// Retail symbols are unmangled; declared here so the call relocs bind to the
// retail names. copyCrystalParamRet returns its first argument (callers chain it as
// the copy source for copyCrystalBoxParam); its definition lives in this TU under
// the name copyCrystalParamRet (linked by address via symbols.txt).
// getCrystalSelTopCount's s32 prototype is load-bearing: an s64 return made
// callers compare the high result word (r3/r4 swap) and lost a full match.
// No casts are needed at the use sites.
// noinline: these update-callback helpers must stay out-of-line (retail
// emits bl to them); without it MWCC inlines their bodies into the callers.

// initCrystalBoxResources (ctor-style copy-init) imports.
extern "C" void* getHandleMEM1__Q23mtl10MemManagerFv();
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
extern "C" u32 KyoshinHeap_GetField44();
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void*, void*);   // 4-word mem-region copy
extern "C" void CItemBlock_setFlag120EC(u8, u32);
