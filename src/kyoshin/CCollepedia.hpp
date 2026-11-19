#pragma once

#include <types.h>
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

namespace nw4r {
namespace lyt {
    class Layout;
    class Pane;
    class AnimTransform;
    class ArcResourceAccessor;
    class DrawInfo;
}
}

// One cell of a page row (stride 0xA). The item id and kind are the
// BDAT keys; the last byte is the unlock state (0 empty, 1 locked,
// 2 claimable, 3 collected).
struct CLPCell {
    /* 0x00 */ u16 mItemId;
    /* 0x02 */ u16 mKind;
    /* 0x04 */ u16 mSortKey;
    /* 0x06 */ u16 mNameId;
    /* 0x08 */ u8 mState;
    /* 0x09 */ u8 _09;
}; // sizeof == 0xA

// A row holds five cells; the first two bytes select the row's sub-category
// and record whether all of its cells have been collected.
struct CLPRow {
    /* 0x00 */ u8 mSubCategory;
    /* 0x01 */ u8 mDone;
    /* 0x02 */ CLPCell mCells[5];
}; // sizeof == 0x34

// One category page. Retail strides these at 0x140. The trailing bytes are
// padding; the sixth-row bounds check in the retail helpers addresses the
// start of this padding rather than a separately allocated row.
struct CLPPage {
    /* 0x00 */ u8 mCategory;
    /* 0x01 */ u8 mRowCount;
    /* 0x02 */ u8 mDone;
    /* 0x03 */ u8 _03;
    /* 0x04 */ f32 mProgress;
    /* 0x08 */ CLPRow mRows[5];
    /* 0x10C */ u8 _10C[0x34];
}; // sizeof == 0x140

// The 0x2800-byte page table, copied member-wise in clpStartLoads.
struct CLPPageTable {
    CLPPage mPages[32];
}; // sizeof == 0x2800

// Catalog at CCollepedia+0xE8: count, selected page, then the page table.
struct CLPCatalog {
    /* 0x00 */ u8 mCount;
    /* 0x01 */ s8 mCurrentPage;
    /* 0x02 */ u8 _02[2];
    /* 0x04 */ CLPPageTable mPageTable;
}; // sizeof == 0x2804

// Stack temporary used to initialize the entry storage (matches retail frame layout)
struct CLPInitTemp {
    u8 _00;
    s8 _01;
    u8 pad[2];
    CLPPageTable data;
};

// Container with a layout pointer at +0x04 (used by clpCalcCurPos)
struct LayoutContainer {
    u8 _00[0x04];
    nw4r::lyt::Layout* mpLayout; // at +0x04
};

// Cursor class derived from CBaseCur, used by CCollepedia for cursor display.
// Vtable at lbl_eu_80537474 (3 entries, size 0x18 with padding).
class CCLPCur : public CBaseCur {
public:
    CCLPCur(nw4r::lyt::ArcResourceAccessor* pAccessor);
};

// Cursor body shared by CCur07/CCur18/CCLPCur (everything after the vtable
// pointer); OnFileEvent builds cursors on the stack and copies this region
// into the embedded members.
struct CLPCurBody {
    void* field_04;
    void* field_08;
    void* field_0C;
    void* field_10;
    u8 mActive;   // +0x14
    u8 mVisible;  // +0x15
};

// Second-page setup record (built by clpInitPgInfo, finished by
// clpSetupDetPg): accessor, loaded layout, bound animation transform.
struct CLPPageSetup {
    nw4r::lyt::ArcResourceAccessor* mpAccessor;   // +0x00
    nw4r::lyt::Layout* mpLayout;                  // +0x04
    nw4r::lyt::AnimTransform* mpAnimTrans;        // +0x08
};

// Texture size header reachable from a lyt texture resource's +0x08 pointer.
struct CLPTexSize {
    u16 field_00;
    u16 field_02;
};

// Shared .sdata2 u32->f64 conversion magic (2^52); retail .sdata2 label (owning TU or extern).
extern double lbl_eu_80668808;

// POD mirror of nw4r::lyt::Size (two f32): using the real type would emit
// the default-constructor zeroing, which retail does not have.
struct CLPSize {
    f32 width;
    f32 height;
};


// Page-info record produced by clpInitPgInfo and stored at +0x28EC.
struct CLPPageInfo {
    u32 field_00;                // +0x28EC
    nw4r::lyt::Layout* mpLayout; // +0x28F0
    u32 field_08;                // +0x28F4
    u8 field_0C;                 // +0x28F8
    u8 field_0D;                 // +0x28F9
    u8 field_0E;                 // +0x28FA
};



// nw4r offset views (forbidden headers: views live here, not TU-local, for -ipa line stability).
struct CLPLytRootView { // layout root-pane view (clpCalcCurPos)
    u8 pad_00[0x10];
    nw4r::lyt::Pane* rootPane; // +0x10
};
struct CLPPanePosView { // pane position view (clpInitLayout/clpSetPageVis)
    u8 pad_00[0x2C];
    f32 pos[3]; // +0x2C position triple
};
// NOTE: +0x00 vtable-ptr field, dtors via __dt__ thunks; +0x0C doubles as state bytes/AnimTransform*.
// (flat non-virtual layout; see field_0/field_c_ptr.)
struct CCollepedia {
    /* 0x00 */ int field_0; // vtable ptr (set by ctor)
    /* 0x04 */ nw4r::lyt::Layout* field_4;
    /* 0x08 */ nw4r::lyt::Layout* field_8; // used as Layout* in rendering, AnimTransform* in state checks
    /* 0x0C */ nw4r::lyt::AnimTransform* field_c_ptr; // state machine (low bytes) or AnimTransform*
    /* 0x10 */ nw4r::lyt::AnimTransform* field_10_ptr;
    /* 0x14 */ u8 _14[0x24 - 0x14];
    /* 0x24 */ void* field_24; // CFileHandle* - file handle 1
    /* 0x28 */ void* field_28; // CFileHandle* - file handle 2
    /* 0x2C */ void* field_2C; // CFileHandle* - archive file handle
    /* 0x30 */ nw4r::lyt::ArcResourceAccessor* field_30;
    /* 0x34 */ nw4r::lyt::ArcResourceAccessor* field_34;
    /* 0x38 */ nw4r::lyt::Layout* field_38;
    /* 0x3C */ nw4r::lyt::AnimTransform* field_3C;
    /* 0x40 */ nw4r::lyt::AnimTransform* field_40; // AnimTransform* at 0x40
    /* 0x44 */ nw4r::lyt::AnimTransform* field_44; // AnimTransform* at 0x44
    /* 0x48 */ u8 field_48;
    /* 0x49 */ u8 field_49;
    /* 0x4A */ u8 _4A[0x4C - 0x4A];
    /* 0x4C */ u32 field_4C;
    /* 0x50 */ u8 field_50;
    /* 0x51 */ u8 field_51;
    /* 0x52 */ u8 _52[0x54 - 0x52];
    /* 0x54 */ u8 field_54[0x9C - 0x54]; // sub-object at 0x54 (two embedded cursors at +0x54/+0x84)
    /* 0x9C */ u8 field_9C; // CSysWin sub-object starts here
    /* 0x9D */ u8 _9D[0xD8 - 0x9D];
    /* 0xD8 */ u8 field_D8;
    /* 0xD9 */ u8 field_D9;
    /* 0xDA */ u8 field_DA;
    /* 0xDB */ u8 _DB; // padding
    /* 0xDC */ float field_DC[3]; // VEC3 copied from pane position (0xDC-0xE7)
    /* 0xE8 */ CLPCatalog mCatalog; // page catalog (ends at 0x28EC)
    /* 0x28EC */ u8 field_28EC[0x28F0 - 0x28EC];
    /* 0x28F0 */ nw4r::lyt::Layout* field_28F0; // Layout* at 0x28F0 (used by clpDrawWindow)
    /* 0x28F4 */ u8 _28F4[0x28F9 - 0x28F4];
    /* 0x28F9 */ u8 field_28F9;
    /* 0x28FA */ u8 field_28FA;

    bool OnFileEvent(CEventFile* pEventFile);

    ~CCollepedia();
    CCollepedia();
};




// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" u32 GetItemMulti_IsActiveFlag();
extern "C" u32 CItem_sumFamilyByte6(u16);
extern "C" char lbl_eu_8050C6E8[];
extern "C" void* lbl_eu_806640A8;
extern "C" void* lbl_eu_80664104;
extern "C" const float lbl_eu_80668800;
extern "C" const float lbl_eu_806687F0;
extern "C" const float lbl_eu_806687F4;
extern "C" const float lbl_eu_806687F8;
extern "C" const float lbl_eu_806687FC;
extern "C" const float lbl_eu_80668810;
extern "C" const float lbl_eu_80668814;
extern "C" const double lbl_eu_80668818;
extern "C" void* lbl_eu_80537474[];
extern "C" void UIWin_CreateItemMulti(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern "C" char* lbl_eu_806647DC;
extern "C" void* lbl_eu_806647D8;
extern "C" u32 CtrlObjectParam_GetStateBitFlag(u16);
extern "C" u16 lbl_eu_8050C6A0[];
// CCur per-frame update
extern "C" u32 lbl_eu_806640EC;
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__6CCur07Fv(void*, int);
extern "C" void deleteRegion__17UnkClass_8045F564Fv(void*);
extern "C" void CtrlObjectParam_SetStateBitFlag(u16, u32);
extern "C" u32 CItem_findRecByFamily(u16);
extern "C" void CItemData_initFromFamily(void*, u16, u32);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 KyoshinHeap_GetField44();
extern "C" float MenuSnd_GetMasterVol_95EC();
extern "C" void CTaskGame_openStream(u32, const char*, void*, u32, u32, u32);
extern "C" void copyVEC3(void*, const void*);

// Additional imports for the CCollepedia constructor
extern void* lbl_eu_805373E0[]; // CCollepedia vtable (retail .data)
extern "C" void __ct__CCur07(void* self, void* param);   // CCur07 ctor (+0x54)
extern "C" void __ct__7CSysWinFUc(CSysWin* self, int arg);   // CSysWin ctor

// Imports used by OnFileEvent / clpBuildPages
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);                     // bdat row count
extern "C" u32 CtrlRemote_TouchBitByArg(u32);                       // unlock-flag lookup
extern "C" void* lbl_eu_806640A0;                        // bdat table A
extern "C" u32 lbl_eu_80664184;                          // default category id
extern "C" void* Bdat_GetTable_AA34();                        // BDAT table unload
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32,
    nw4r::lyt::Layout*);                                 // CDeviceFont helper
// Shared tag string; retail symbol is the MANGLED getPackedFont__Fv, so this
// import must keep C++ linkage (MWCC appends __Fv).
// Verbatim-mangled import: the real retail CfGameManager controller-type check

// Mirror of CSysWin +0x04..end (everything after the vtable pointer), used to
// member-wise copy a stack-constructed CSysWin into the embedded window at
// +0x9C the way retail does in the CCollepedia constructor.
struct CLPSysWinBody {
    UnkClass_8045F564 mMemRegion;                  // +0x04
    void* mFileHandle;                             // +0x14
    void* mTagProcessor;                           // +0x18
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // +0x1C
    nw4r::lyt::Layout* mLayout;                    // +0x20
    nw4r::lyt::AnimTransform* mAnimTrans;          // +0x24
    u8 field_28;                                   // +0x28
    u32 field_2C;                                  // +0x2C
    u32 field_30;                                  // +0x30
    u8 field_34;                                   // +0x34
    u8 field_35;
    u8 field_36;
    u8 field_37;
    u8 field_38;
    u8 field_39;
};