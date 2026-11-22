#pragma once

#include "kyoshin/cf/CfBdat.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <types.h>
#include "kyoshin/menu/MenuSndApi.hpp"

#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
#include "kyoshin/CSysWin.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

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
// Retail emits CCollepedia's assignment member-wise through this struct (count,
// page index, then the page-table block), so it is not a trivial aggregate.
struct CLPCatalog {
    ~CLPCatalog();
    /* 0x00 */ u8 mCount;
    /* 0x01 */ s8 mCurrentPage;
    /* 0x04 */ CLPPageTable mPageTable;
}; // sizeof == 0x2804

// Stack temporary used to initialize the entry storage (matches retail frame layout)
struct CLPInitTemp {
    u8 _00;
    s8 _01;
    u8 pad[2];
    CLPPageTable data;
};

// Stand-alone cursor embedded at CCollepedia+0x6C. Not a CBaseCur: its
// vtable (one slot, initLayout) and RTTI ("CCLPCur", no bases) are emitted by
// the compiler, and retail copies its members inline in CCollepedia's
// implicit assignment.
class CCLPCur {
public:
    CCLPCur(nw4r::lyt::ArcResourceAccessor* pAccessor);
    ~CCLPCur();
    virtual void initLayout();
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // +0x04
    nw4r::lyt::Layout* mpLayout;                // +0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;     // +0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;     // +0x10
    u8 mActive;                                 // +0x14
    u8 mVisible;                                // +0x15
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


// Detail-page record at CCollepedia+0x28EC (built by clpInitPgInfo, finished
// by clpSetupDetPg): accessor, detail layout, its open/close animation, then
// the open/close state machine bytes.
struct CLPPageInfo {
    ~CLPPageInfo();
    nw4r::lyt::ArcResourceAccessor* mpAccessor; // +0x00
    nw4r::lyt::Layout* mpLayout;                // +0x04
    nw4r::lyt::AnimTransform* mpAnimTrans;      // +0x08
    u8 mState;                                  // +0x0C 0 closed, 1 opening, 2 open, 3 closing
    u8 field_0D;                                // +0x0D
    u8 field_0E;                                // +0x0E detail view shown
};

// CCollepedia is a real IWorkEvent: MWCC emits its vptr and event virtuals.
struct CCollepedia : public IWorkEvent {
    /* 0x04 */ UnkClass_8045F564 mRegion0;
    /* 0x14 */ UnkClass_8045F564 mRegion1;
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
    /* 0x4C */ u32 field_4C;
    /* 0x50 */ u8 field_50;
    /* 0x51 */ u8 field_51;
    /* 0x54 */ CCur07 mCur07;
    /* 0x6C */ CCLPCur mClpCur;
    /* 0x84 */ CCur18 mCur18;
    /* 0x9C */ CSysWin mSysWin;
    /* 0xD8 */ u8 field_D8;
    /* 0xD9 */ u8 field_D9;
    /* 0xDA */ u8 field_DA;
    /* 0xDC */ float field_DC[3]; // VEC3 copied from pane position (0xDC-0xE7)
    /* 0xE8 */ CLPCatalog mCatalog; // page catalog (ends at 0x28EC)
    /* 0x28EC */ CLPPageInfo mPageInfo;

    bool OnFileEvent(CEventFile* pEventFile);

    ~CCollepedia();
    CCollepedia();
};




// CCollepedia logic functions (C++ linkage, mangled retail symbols).
void clpStartLoads(CCollepedia* self);
void clpFreeAllRes(CCollepedia* self);
void clpReqCloseVw(CCollepedia* self);
void clpCursorLeft(CCollepedia* self);
void clpCursorRght(CCollepedia* self);
void clpCursorUp(CCollepedia* self);
void clpCursorDown(CCollepedia* self);
void clpOnCancel(CCollepedia* self);
int clpCanClaim(CCollepedia* self);
void clpSetPageVis(CCollepedia* self);

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" u32 GetItemMulti_IsActiveFlag();
extern "C" u32 CItem_sumFamilyByte6(u16);
extern "C" const float lbl_eu_80668800;
extern "C" const float lbl_eu_806687F0;
extern "C" const float lbl_eu_806687F4;
extern "C" const float lbl_eu_806687F8;
extern "C" const float lbl_eu_806687FC;
extern "C" const float lbl_eu_80668810;
extern "C" const float lbl_eu_80668814;
extern "C" const double lbl_eu_80668818;
extern "C" void UIWin_CreateItemMulti(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern "C" char* gpClpCollectMsgTable;
extern "C" void* gpClpItemMsgTable;
extern "C" u32 CtrlObjectParam_GetStateBitFlag(u16);
// CCur per-frame update

extern "C" u32 CItem_findRecByFamily(u16);
extern "C" void CItemData_initFromFamily(void*, u16, u32);
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 KyoshinHeap_GetField44();

extern "C" void copyVEC3(void*, const void*);


// Imports used by OnFileEvent / clpBuildPages
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);                     // bdat row count
#include "kyoshin/cf/CtrlRemoteApi.hpp"                       // unlock-flag lookup
extern "C" u32 lbl_eu_80664184;                          // default category id
extern "C" void* Bdat_GetTable_AA34();                        // BDAT table unload
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32,
    nw4r::lyt::Layout*);                                 // CDeviceFont helper
// Shared tag string; retail symbol is the MANGLED getPackedFont__Fv, so this
// import must keep C++ linkage (MWCC appends __Fv).
// Verbatim-mangled import: the real retail CfGameManager controller-type check

