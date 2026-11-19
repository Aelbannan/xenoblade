#pragma once

#include <types.h>
// Member types of CModelDispMakeCrystal (the class owns them by value so that
// their ctors/dtors run from the compiler-generated ctor/dtor).
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/makecrystal/CMCEffStart.hpp"
#include "kyoshin/makecrystal/CMCCylinderGauge.hpp"
#include "kyoshin/makecrystal/CMCCrystalList.hpp"
#include "kyoshin/makecrystal/CMCCrystalSupport.hpp"
#include "kyoshin/makecrystal/CMCGetItemBox.hpp"

class CEventFile;
class CScn;

namespace cf { class CfObjEnumList; }

namespace nw4r { namespace lyt { class DrawInfo; } }
class CMCEffStart;
// Retail free draw helper for CMCEffStart effects (defined in CMCEffStart.cpp;
// global C++ linkage mangles to draw__FP11CMCEffStartPQ34nw4r3lyt8DrawInfo).
void draw(CMCEffStart*, nw4r::lyt::DrawInfo*);

// Full object layout for CModelDispMakeCrystal (used by C-linkage accessors)
struct CModelDispMakeCrystalFull {
    u8 _00[0x5F0];
    u8 field_5F0;                 // +0x5F0 char state
    u8 _5F1[0xBBC - 0x5F1];
    u8 field_BBC;                 // +0xBBC
    u8 _BBD[0xBDC - 0xBBD];
    u8 field_BDC;                 // +0xBDC
    u8 field_BDD;                 // +0xBDD
    u8 _BDE[0xBE8 - 0xBDE];
    u8 field_BE8;                 // +0xBE8
    u8 _BE9[0x2DC3 - 0xBE9];
    u8 field_2DC3;                // +0x2DC3
    u8 _2DC4[0x2DCC - 0x2DC4];
    f32 field_2DCC;               // +0x2DCC
    u8 _2DD0[0x2DD1 - 0x2DD0];
    s8 field_2DD1;                // +0x2DD1
    u8 field_2DD2;                // +0x2DD2
};

// CMCryAnim folded into cf::CActParamAnimGame (vtable +0xE0 is resetAnimGame) - use void* vptr to avoid header conflict

// (was CMCryMove: virtual dispatch on the crystal-charged actor's embedded
// CfObjectMove at +0x3e9c, at +0x74 (no args) and +0x148 (index arg). These
// are cf::CfObject::CfObject_isMoveActiveNow (bool, +0x74) and
// cf::CfObject::CfObject_getSlotBits (u32, +0x148, retail index arity)
// (cf. CModelDispEquip.hpp); call sites now use the owning class directly.)

// (was CMCModel: virtual dispatch on the built model object (sub->field_00)
// at +0x48 (float) and +0x9C (2 ints). Retail table lbl_eu_8056DD70 words 18
// and 39 are CScnItemModel::vfunc48/vfunc9C (cf. CModelDispEquip.hpp); call
// sites now use the owning class directly.)

// (was CMCCryParamObj: virtual dispatch at +0x8 on the per-crystal param
// object. The +0x2C word is a ::CResLookup (cf. CfResPcTableEntry in
// CfResPcImpl.hpp and CModelDispEquip.hpp); its first virtual is
// getResourceBase(void*, int) at +0x8. Call sites now use the owning class
// directly.)
class CResLookup;

// Crystal-charged actor container: CfObjectMove embedded at +0x3e9c.
struct CMCCryChgActor {
    u8 _00[0x3e9c];
    u8 move[0x3f28 - 0x3e9c]; // embedded CfObjectMove
    u16 field_3f28;            // +0x3f28
    u32 field_3f2c;            // +0x3f2c
};

// 12-byte crystal sub-buffer (stride 0xc); 6 per display slot at +0x564.
// initCrystalSubStruct zeroes field_00/field_04/field_08.
struct CMCrySubBuf {
    u32 field_00;   // +0x00
    u32 field_04;   // +0x04
    u8  field_08;   // +0x08
    u8  _09[3];     // +0x09
};

// 12-byte vector holding the ctor's crystal-position groups (copied into the
// slot fields at +0x5b0/+0x5bc as raw words).
struct CMCryVec3 {
    f32 x, y, z;
};


// Per-crystal param slot (stride 0x3c); +0x2c holds a CResLookup.
struct CMCCryParamSlot {
    u8 _00[0x2c];
    CResLookup* field_2c;            // +0x2c
    u8 _30[0x3c - 0x30];
};

// Crystal display sub-object (per 0x5cc slot, embedded at +0x44).
// Real class: retail constructs the two slots with __construct_array
// (element ctor MakeCrystal_InitSlotArray @0x8021E348, element dtor
// __dt__8021C540 @0x8021E398) and destroys them with __destroy_arr, so the
// owner declares `CMCrystalDispSub mSlots[2];` and the compiler emits both.
struct CMCrystalDispSub {
    CMCrystalDispSub();
    ~CMCrystalDispSub();

    void* field_00;            // +0x00 built model pointer (assigned from void* func -> kept void*)
    void* field_04;            // +0x04 (assigned from void* func -> kept void*)
    void* mAnimVptr;           // +0x08 inline CActParamAnimGame vptr
    u8 _0c[0x14 - 0x0c];       // +0x0c anim-object payload
    u32 field_14;              // +0x14 anim flags (CActParamAnimGame::field_0C)
    u8 _18[0x544 - 0x18];      // +0x18 rest of the anim object
    u32 mCrystalVals[6];       // +0x544
    u8 _55c[0x5a0 - 0x55c];
    u32 field_5a0;             // +0x5a0 file handle
    u32 field_5a4;             // +0x5a4
    u8 _5a8[0x5ac - 0x5a8];
    s8 field_5ac;              // +0x5ac state
    u8 _5ad[0x5b0 - 0x5ad];
    u32 field_5b0;             // +0x5b0 pos group 1
    u32 field_5b4;             // +0x5b4
    u32 field_5b8;             // +0x5b8
    u32 field_5bc;             // +0x5bc pos group 2
    u32 field_5c0;             // +0x5c0
    u32 field_5c4;             // +0x5c4
    u8 field_5c8;              // +0x5c8 built flag
};

// CModelDispMakeCrystal fields used by MakeCrystal_BuildCrystalList.
struct CMCCrySelfFields {
    u8 _00[0xc];
    u8* field_0c;            // +0x0c allocator/parent
    u8 _10[0xbe0 - 0x10];
    f32 field_be0;             // +0xbe0
};

class CModelDispMakeCrystal {
public:
    CModelDispMakeCrystal(CScn* scene);
    ~CModelDispMakeCrystal();
    int OnFileEvent(CEventFile* ev);

    void initCrystalSubStruct();
    u8 getCrystalStateA();
    u8 getCrystalStateB();
    u8 getCrystalStateC();
    void setCrystalCursor(u32 val);
    void destroyCrystalDispThunk4();
    void releaseCrystalDispThunk();
    void destroyCrystalDispThunk8();

    // Retail layout. The class is not polymorphic in retail (the dtor resets no
    // vptr, cf. CModelDisp::mVtbl): the three vtable pointers are plain data
    // members written by the ctor. Everything up to +0x13BF is a real typed
    // member so that its ctor runs from the ctor init list in declaration
    // (= retail store) order; the tail is still raw.
    u32* mVtbl;                   // 0x000 (lbl_eu_80535E70)
    u32* mVtblRender;             // 0x004 (lbl_eu_80535E70 + 0x88)
    u32* mVtblAnim;               // 0x008 (lbl_eu_80535E70 + 0xb4)
    CScn* mScene;                 // 0x00C owning scene
    void* mField10;               // 0x010
    void* mField14;               // 0x014
    void* mFileHandle18;          // 0x018 (MakeCrystal_InitFiles readFile slot)
    void* mField1C;               // 0x01C
    void* mField20;               // 0x020
    void* mFileHandle24;          // 0x024 (MakeCrystal_InitFiles readFile slot)
    void* mField28;               // 0x028
    UnkClass_8045F564 mRegion;    // 0x02C
    void* mFileHandle3C;          // 0x03C (MakeCrystal_InitFiles readFile slot)
    void* mField40;               // 0x040
    CMCrystalDispSub mSlots[2];   // 0x044 (2 x 0x5CC crystal display slots)
    u8 field_BDC;                 // 0xBDC
    u8 field_BDD;                 // 0xBDD
    u8 _BDE[0xBE0 - 0xBDE];
    f32 field_BE0;                // 0xBE0
    f32 field_BE4;                // 0xBE4
    u8 field_BE8;                 // 0xBE8 (getCrystalStateA)
    u8 field_BE9;                 // 0xBE9
    u8 _BEA[0xBEC - 0xBEA];
    CMCCylinderGauge mGauge;      // 0x0BEC
    CMCCrystalList mCrystalList;  // 0xC18
    CMCCrystalSupport mSupport;   // 0xC70
    CMCEffStart mEffStart;        // 0xC8C
    CMCEffUpRed mEffUpRed;        // 0xCA4
    CMCEffUpBlue mEffUpBlue;      // 0xCBC
    CMCEffUpGreen mEffUpGreen;    // 0xCD4
    CMCEffUpPrm mEffUpPrm;        // 0xCEC
    CMCEffSuccess mEffSuccess;    // 0xD5C
    CMCEffFailure mEffFailure;    // 0xD78
    CMCEffCrystal mEffCrystal;    // 0xD90
    CMCEffUpRank mEffUpRank;      // 0xDCC
    CMCEffDivide mEffDivide;      // 0xDE4
    CMCEffCylinder mEffCylinder;  // 0xDFC
    u32 field_E1C;                // 0xE1C
    CCur18 mCurE20;               // 0xE20
    CScrollBar mScrollBar;        // 0xE38
    CSysWin mSysWin;              // 0xE78
    CCur18 mCurEB4;               // 0xEB4
    CMCGetItemBox mItemBox;       // 0xECC (0x4EC bytes)
    u8 mCrystalIds[7];            // 0x13B8
    u8 mCrystalNum;               // 0x13BF
    u8 mStepTable[0x80 * 0x34];   // 0x13C0 (128 step entries, stride 0x34)
    u8 mCursorState[0x0C];        // 0x2DC0
    f32 field_2DCC;               // 0x2DCC
    u8 mCursorState2[0x06];       // 0x2DD0
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 lbl_eu_805090D8[]; // 3 state filter values for startEnumObjects
extern "C" u8 lbl_eu_80664718[0x8]; // makecrystal crystal-probability state (sbss, 8 bytes)
// Color table: rows of 3 bytes, indexed by character state - 1.
struct CMCryColorRow { u8 c[3]; };
extern "C" const CMCryColorRow lbl_eu_80535D90[];
// Retail hard symbols with C linkage (defined in this TU's retail .o).
extern "C" void initCrystalSubStruct(void* self);
extern "C" void __ct__Q22cf17CActParamAnimGameFv(void* p);
extern "C" void __dt__Q22cf17CActParamAnimGameFv(void* obj, int flag);

// Plain-C-linkage functions defined in this TU (retail symbol = plain name).
extern "C" void MakeCrystal_ResetSlotList(CModelDispMakeCrystal*, u8);
extern "C" void MakeCrystal_RefreshSlotEntries(CModelDispMakeCrystal*);
extern "C" void MakeCrystal_AdvanceSlotEntries(void*);
extern "C" void MakeCrystal_SpawnCrystalItem(void*, int, u8);
// CTaskGame_openVision is owned by kyoshin/CTaskGame.hpp (single unified decl).

// C-linkage helpers for the cylinder-count finalize (MakeCrystal_FinalizeCylCount):
// item-step-list setup and scrollbar refresh.
extern "C" void CItem_initKind9Rec(void* item, int mode);
extern "C" void CItemData_callVf0C(void* item, u8 flag);
extern "C" void CItem_copyRecMasked(void* dst, const void* src);
// CItem_initItemImplInstances: canonical CItemImpl*(CItemData*) declaration
// comes from kyoshin/cf/CfGameManager.hpp.

// C-linkage imports for the constructor (retail bare-name symbols - the
// member ctors in the sub-object headers would mangle differently).
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
extern "C" void __construct_array(void*, void*, void*, u32, u32);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);
extern "C" size_t strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int ItemMenu_IsPresent();   // item-menu active gate (CMainMenu.cpp)

// Constructor data imports.
extern "C" const void* lbl_eu_80535E70[];   // CModelDispMakeCrystal vtable (+0x88/+0xb4 sub-vtables)
// Non-const singletons would sink MWCC's SDA loads below aliasing stores
// (see CModelDispMakeCrystal.cpp .sdata2 notes); each is a const [1] whose
// [0] reads hoist like retail. Defined by the TU's .cpp pool block.
extern const f32 lbl_eu_806684A8[1];
extern const f32 lbl_eu_806684B0[1];
extern const f32 lbl_eu_806684B4[1];
extern const f32 lbl_eu_806684B8[1];
extern const f32 lbl_eu_806684BC[1];
extern const f32 lbl_eu_806684C0[1];

