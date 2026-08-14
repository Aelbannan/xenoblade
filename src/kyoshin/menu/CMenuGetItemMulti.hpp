#pragma once

#include <types.h>

#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"

namespace nw4r {
namespace lyt {
class AnimTransform;
class Layout;
class DrawInfo;
}
}

class CFileHandle;
class CScn;
class IUIWindow;

struct CMenuGetItemMultiEntry {
    u32 packed;
    u16 value;
    u8 rest[0x2e];
};

struct CMenuGetItemPaneView {
    u8 _00[0x4c];
    f32 width;
    f32 height;
    u8 _54[0xbb - 0x54];
    u8 flags;
};

// 4 x s16 pane colour/position entry (RGBA-style; alpha (.d) stays 0).
struct CMenuGetItemFourShorts {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
};

class CMenuGetItemMulti {
public:
    CMenuGetItemMulti();
    ~CMenuGetItemMulti();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent();

    // Deliberately NON-polymorphic (CMenuGetItem idiom): the CProcess base
    // region (incl. composite vtable at +0x10) is opaque storage; the
    // IWorkEvent/IScnRender subobject vtable slots are explicit fields below.
    u8 _00[0x54];
    nw4r::lyt::Layout* mLayout;              // +0x54
    u32 field_58;
    u32 field_5C;
    s32 field_60;
    u8 field_64;
    u8 field_65;
    u8 field_66;
    u8 field_67;
    u32 field_68;
    u32 mIWorkEventVtable;                   // +0x6C
    u32 mIScnRenderVtable;                   // +0x70
    CScn* mScn;                              // +0x74
    u8 mRegion[0x10];                        // +0x78
    nw4r::lyt::AnimTransform* mAnim;         // +0x88
    u32 mInitialItems[4];                    // +0x8C
    u32 mVisibleItemCount;                   // +0x9C
    CMenuGetItemMultiEntry mEntries[4];      // +0xA0
    u32 mEntryCount;                         // +0x170
    CBaseCur mCursor;                        // +0x174
    u8 mSystemWindow[0x3C];                  // +0x18C CSysWin storage (raw;
                                             // ctor/dtor driven via C-ABI
                                             // __ct__CSysWin/__dt__7CSysWinFv)
    u8 mMaxVisibleItems;                     // +0x1C8
    u8 mHasSpecialItem;                      // +0x1C9
    u8 mPaneVisible[6];                      // +0x1CA
    CMenuGetItemMultiEntry* mVisibleEntries[6]; // +0x1D0
    u16 mVisibleItemIds[6];                  // +0x1E8
    u8 field_1F4;
    u8 field_1F5;
    u8 field_1F6;
    u8 field_1F7;
    u32 field_1F8;
    u32 field_1FC;
    u8 field_200;
    u8 field_201;
    u8 field_202[2];
    CFileHandle* mFileHandle;
    u32 field_208;
    u8 field_20C;
    u8 field_20D;
    u8 field_20E;
    u8 field_20F;
};

// --- unit imports ---

// Get-item-multi singleton instance pointer (retail SDA symbol).
extern u32 lbl_eu_80664414;

// Currently-loaded item file buffer pointer (released by Term/func_801B45A0).
extern u32 lbl_eu_80664418;

// Pane colour/position defaults (.sbss, written by sinit_801B9FC8).
extern CMenuGetItemFourShorts lbl_eu_806643A0;
extern CMenuGetItemFourShorts lbl_eu_806643A8;
extern CMenuGetItemFourShorts lbl_eu_806643B0;
extern CMenuGetItemFourShorts lbl_eu_806643B8;
extern CMenuGetItemFourShorts lbl_eu_806643C0;
extern CMenuGetItemFourShorts lbl_eu_806643C8;
extern CMenuGetItemFourShorts lbl_eu_806643D0;
extern CMenuGetItemFourShorts lbl_eu_806643D8;
extern CMenuGetItemFourShorts lbl_eu_806643E0;
extern CMenuGetItemFourShorts lbl_eu_806643E8;
extern CMenuGetItemFourShorts lbl_eu_806643F0;
extern CMenuGetItemFourShorts lbl_eu_806643F8;
extern CMenuGetItemFourShorts lbl_eu_80664400;
extern CMenuGetItemFourShorts lbl_eu_80664408;

// Global flag word: bit 21 (0x200000) gates the render path (cbRenderBefore)
// and is cleared by Term.
extern u32 lbl_eu_80663E28;

// Pane size/position step factor used by func_801B5630's pane adjustments
// (.sdata2). Sibling lbl_eu_80667E18 is the int->double conversion magic
// (0x4330000080000000) that MWCC emits for the (f32)(s32) casts there.
extern f32 lbl_eu_80667E14;
extern const f64 lbl_eu_80667E18;

// Minimal CTaskGame decl (same shape as CSystemWindow.hpp) - only the
// statics cbRenderBefore consumes.
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// Vtable-dispatch view of the embedded cursor (same scheme as CPartyStateCur):
// MWCC puts two implicit dtor slots at the start of every polymorphic vtable,
// so func_801D2180 (retail cursor slot 3) is declared as the SECOND virtual
// to land at vtable offset 0xC. All-pure, never constructed directly.
class CMenuGetItemMultiCur {
public:
    virtual void vfn0() = 0;          // MWCC vtable slot 2
    virtual void func_801D2180() = 0; // MWCC vtable slot 3 (retail slot 3)
};

// Minimal view of the CfGameManager fields Term's action-source teardown
// touches (the full class exposes these offsets as byte arrays).
struct CfGameManagerTermFields {
    u8 _00[0x64];
    u32 field_64;              // +0x64 action-source flags (bit 8 = active)
    u32 field_68;              // +0x68 flags (|= 0x60 on teardown)
    u8 _6C[0x80 - 0x6C];
    u32 field_80;              // +0x80 action-source id slot
};

// C-ABI imports (retail emits these unmangled). The .sbss/.rodata labels each
// interacts with are noted on the decl.
extern "C" void func_8003AA34();                    // bdat refresh (paired with the lbl_eu_80504A3C fp lookup)
extern "C" void func_8003AA78__5CBdatFUlPv(u32 value, u8* data);
extern "C" void func_8003AA8C__5CBdatFUl(u32 value);
extern "C" u8 func_8013B980();                      // flag reset alongside code80135FDC_getByte_64080
extern "C" u8 code80135FDC_getByte_64080();
extern "C" void func_8022B7F4(u8* syswin);          // CSysWin teardown (Term, after the lbl_eu_80664414/18 clears)
extern "C" void func_8022B7C8(u8* syswin, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801D20B0(CBaseCur* cur, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_800B7320(u32 obj);             // action-source teardown (Term, after the lbl_eu_80663E24 clear)
// Retail ctor symbol is the unmangled `__ct__CMenuGetItemMulti` (constructs
// the singleton stored in lbl_eu_80664414); 8 reg args + a byte on the stack.
extern "C" u8* __ct__CMenuGetItemMulti(u8* obj, CScn* pScene, u32 a, u32 b, u32 c,
                                       u32 d, u32 e, u32 f, u8 g);

// C++-linkage import: retail symbol is the mangled func_800B708C__Fi
// (actor id -> action source).
int func_800B708C(int id);
