// Auto-scaffolded catalog TU for kyoshin/CMapSel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMapSel.hpp"
#include "kyoshin/CScrollBar.hpp"
// CMapSel.hpp declares the C-ABI layout helpers (func_80137E7C 2-arg etc.);
// skip code_80135FDC.hpp's caller-tuned exports so the differing arity decls
// don't clash (same mechanism code_80135FDC.cpp itself uses).
#define CODE_80135FDC_CPP
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include <nw4r/lyt.h>

// Intra-TU state-machine / selection helpers (defined below in address order).
// extern "C" keeps the call relocs unmangled (retail symbols are unmangled
// func_* names); noinline: retail calls each via bl, -ipa file would inline.
extern "C" void __declspec(noinline) func_80243ABC(CMapSel*);
extern "C" void __declspec(noinline) func_80243B88(CMapSel*);
extern "C" void __declspec(noinline) func_80243BE8(CMapSel*);
extern "C" void __declspec(noinline) func_80243C6C(CMapSel*);
extern "C" void __declspec(noinline) func_80243CB8(CMapSel*);
extern "C" void __declspec(noinline) func_80243CFC(CMapSel*);
extern "C" void __declspec(noinline) func_80243E08(CMapSel*);
extern "C" void __declspec(noinline) func_80243ED8(CMapSel*);
extern "C" void __declspec(noinline) func_80243FC4(CMapSel*);
extern "C" void __declspec(noinline) func_80243838(CMapSel*);
extern "C" void __declspec(noinline) func_80244020(CMapSel*);

/* __ct__CMapSel - Construct the map-selection widget: set the IWorkEvent
   vtable, construct the sub-objects (UnkClass_8045F564 mem region, scrollbar,
   cursor), then copy a flag-1 temporary CScrollBar's body over the embedded
   scrollbar (retail skips the +0x00 vtable). Afterwards build the map grid:
   collect the valid maps (ids 2..28, types 10/20/22/26/28 excluded), bubble-
   sort the grid by BDAT sort key, and place the selection on the current map. */
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
#pragma push
#pragma optimize_for_size on
extern "C" CMapSel* __ct__CMapSel(CMapSel* self) {
    // IWorkEvent vtable + UnkClass_8045F564 mem region.
    *(void**)self = lbl_eu_80536E10;
    __ct__17UnkClass_8045F564Fv(
        reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion[0]));

    self->mFileHandle = 0;
    self->mFileHandle2 = 0;
    self->mArcAccessor = 0;
    self->mAllocatedMem = 0;
    self->mLayout = 0;
    self->mAnimTransform1 = 0;
    self->mAnimTransform2 = 0;
    self->field_0x30 = 0;
    self->mState = 0;
    self->field_0x32 = 0;
    self->mFlag33 = 1;

    // Embedded sub-objects.
    __ct__CScrollBar(&self->mScrollBar[0], 0);
    __ct__CCur18(&self->mCursor[0], 0);

    float timer0 = lbl_eu_80668738;   // load early, store after the cell fields
    self->mSelY = 0;
    self->mSelX = 0;
    self->mGridData[0x20] = 0;
    self->mGridData[0x21] = 0;
    self->mTimer = timer0;

    // Copy a flag-1 temporary scrollbar's body into the embedded scrollbar
    // (the +0x00 vtable was already set by the first ctor above).
    u8 tempBuf[0x40];
    __ct__CScrollBar(tempBuf, 1);
    CScrollBar* dst = reinterpret_cast<CScrollBar*>(&self->mScrollBar[0]);
    CScrollBar* temp = reinterpret_cast<CScrollBar*>(tempBuf);
    dst->mMemRegion = temp->mMemRegion;
    dst->mFileHandle = temp->mFileHandle;
    dst->mAccessor = temp->mAccessor;
    dst->mLayout = temp->mLayout;
    dst->mAnimTransform = temp->mAnimTransform;
    dst->mReady = temp->mReady;
    dst->mVisible = temp->mVisible;
    dst->mState = temp->mState;
    dst->mActive = temp->mActive;
    dst->mAnimOffset = temp->mAnimOffset;
    dst->mScrollPosY = temp->mScrollPosY;
    dst->mScrollRatio = temp->mScrollRatio;
    dst->mThumbHeight = temp->mThumbHeight;
    dst->mContentHeight = temp->mContentHeight;
    dst->mDirection = temp->mDirection;
    __dt__10CScrollBarFv(tempBuf, -1);

    // --- Build the map grid ---------------------------------------------
    void* mapTable = lbl_eu_806640A0;    // BDAT table for map ids
    void* orderTable = lbl_eu_806640A8;  // BDAT table for sort keys
    s32 mapCount = (s32)func_8003B1EC(mapTable);
    u8 curMap = (u8)lbl_eu_80664184;     // current map id -> initial selection

    // For each map id 2..28 (types 10/20/22/26/28 excluded), append the map
    // to the grid when it is a valid map of that type.
    for (s32 mapId = 2; mapId <= 0x1c; mapId++) {
        switch (mapId) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        case 8: case 9: case 11: case 12: case 13: case 14: case 15: case 16:
        case 17: case 18: case 19: case 21: case 23: case 24: case 25: case 27:
            for (s32 j = 1; j <= mapCount; j++) {
                if (func_801361E8((u32)mapTable, lbl_eu_8050B4A8, (u32)j) == mapId) {
                    if (func_8009CF8C((u32)(j + 0x20c8)) != 0) {
                        u8 idx = self->mGridData[0x20];
                        self->mGridData[idx] = mapId;
                        self->mGridData[0x20] = idx + 1;
                        break;
                    }
                }
            }
            break;
        case 10: case 20: case 22: case 26: case 28:
            ;  // empty: falls to the loop increment
        }
    }

    // Bubble-sort the grid by BDAT sort key, descending (early exit when a
    // pass makes no swap).
    for (u8 pass = 0; pass < self->mGridData[0x20]; pass++) {
        int swapped = 0;
        for (u8 k = 0; k < self->mGridData[0x20] - 1 - pass; k++) {
            u8 b = self->mGridData[k + 1];
            u8 a = self->mGridData[k];
            u8 keyA = func_801361E8((u32)orderTable, lbl_eu_8050B4A8 + 6, (u32)a);
            if (keyA > func_801361E8((u32)orderTable, lbl_eu_8050B4A8 + 6, (u32)b)) {
                self->mGridData[k] ^= self->mGridData[k + 1];
                self->mGridData[k + 1] ^= self->mGridData[k];
                self->mGridData[k] ^= self->mGridData[k + 1];
                swapped = 1;
            }
        }
        if (!swapped) break;
    }

    // Place the selection on the current map: grid index -> (y, x) cell.
    u8 count = self->mGridData[0x20];
    for (u8 i = 0; i < count; i++) {
        if (self->mGridData[i] == curMap) {
            s8 v = (s8)(i + 1);
            self->mSelY = v;
            if (v >= 10) {
                self->mSelX = (s8)(v - 10);
                self->mSelY = 9;
            } else {
                self->mSelY = (s8)(v - 1);
            }
            break;
        }
    }
    return self;
}
#pragma pop

/* Complete-object destructor.  Sub-objects are opaque byte arrays, so their
   retail destructors are invoked explicitly (reverse of construction order);
   the flags-conditional delete matches the MWCC deleting-destructor shape. */
// extern "C" free-function form (CCol6CheckBat precedent): retail calls the
// sub-object dtors in +0x74, +0x34, +0x4 order with flags -1, then the
// flags-based delete; uses stmw/lmw (optimize_for_size) for the frame.
#pragma push
#pragma optimize_for_size on
extern "C" void* __dt__7CMapSelFv(CMapSel* self, int flags) {
    if (self != 0) {
        __dt__6CCur18Fv(&self->mCursor, -1);
        __dt__10CScrollBarFv(&self->mScrollBar, -1);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion, -1);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}
#pragma pop

void func_8024343C(){}

/* func_802434A0 - Per-frame update: drive the state machine, then animate the
   layout, scrollbar, and cursor. Guards on field_0x30 (widget active) and
   mState (not uninitialized); the state dispatch mirrors retail (1: loading,
   2: visible, 3: transitioning out, 4: closing, 5: post-close cleanup). */
extern "C" void __declspec(noinline) func_802434A0(CMapSel* self) {
    if (self->field_0x30 != 0 && self->mState != 0) {
        switch (self->mState) {
        case 1:
            func_80243ABC(self);
            break;
        case 2:
            func_80243B88(self);
            break;
        case 3:
            func_80243ED8(self);
            func_80243E08(self);
            break;
        case 4:
            func_80243BE8(self);
            break;
        case 5:
            func_80243C6C(self);
            break;
        }
        self->mLayout->Animate(0);
        func_801F3540(self->mScrollBar);
        func_801D202C(self->mCursor);
    }
}

/* func_80243560 - Draw the map-select UI: layout, scrollbar, and cursor.
   Guards on field_0x30 (widget active) and mState (not uninitialized). */
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

void func_80243560(CMapSel* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x30 != 0 && self->mState != 0) {
        func_80137038(self->mLayout, drawInfo, 0, 1);
        func_801F35B0(self->mScrollBar, drawInfo);
        func_801D20B0(self->mCursor, drawInfo);
    }
}

/* func_802435CC - Tear down the map-select widget: release both file
   handles, deactivate, then (after vblank sync) delete the layout, free the
   map-data buffer, release the arc accessor / mem region / scrollbar, and
   run the cursor's release virtual. The doubled null-check before the
   layout virtual mirrors retail codegen. */
extern "C" void __declspec(noinline) func_802435CC(CMapSel* self) {
    func_801390E0(&self->mFileHandle);
    func_801390E0(&self->mFileHandle2);
    self->field_0x30 = 0;
    CDeviceVI::waitForDrawDone();
    if (self->mLayout != nullptr) {
        if (self->mLayout != nullptr) {
            reinterpret_cast<CMapSelLayoutObj*>(self->mLayout)->vfRelease(1);
        }
        self->mLayout = nullptr;
    }
    if (self->mAllocatedMem != nullptr) {
        mtl::MemManager::deallocate(self->mAllocatedMem);
        self->mAllocatedMem = nullptr;
    }
    func_80139124(self->mArcAccessor);
    reinterpret_cast<UnkClass_8045F564*>(&self->mMemRegion[0])->func_8045F778();
    func_801F35DC(self->mScrollBar);
    reinterpret_cast<CMapSelCurObj*>(&self->mCursor[0])->vfUpdate();
}

// Scrollbar-visible check: when the scrollbar at +0x34 is visible, return
// the +0x32 flag (lbz), else 0.
extern "C" int CScrollBar_isVisible(void* scrollbar);
extern "C" u8 func_80243680(void* self) {
    if (CScrollBar_isVisible((u8*)self + 0x34) != 0) {
        return *(u8*)((u8*)self + 0x32);
    }
    return 0;
}

// FULL_MATCH: reads the initial-setup flag at +0x33 (set to 1 in ctor, cleared on play/close)

// One-shot init: when the +0x31 byte is clear, set it and +0x33, then run
// the two init hooks.
extern "C" void func_802436CC(CMapSel* self) {
    if (*(u8*)((u8*)self + 0x31) == 0) {
        *((u8*)self + 0x31) = 1;
        *((u8*)self + 0x33) = 0;
        func_80243CFC(self);
        func_80244020(self);
    }
}

// When the +0x24 layout is present, run the grid rebuild (func_80243CFC) and
// raise the +0x32/+0x30 flags.
extern "C" void func_80243CB8(CMapSel* self) {
    if (*(u32*)((u8*)self + 0x24) != 0) {
        func_80243CFC(self);
        *(u8*)((u8*)self + 0x32) = 1;
        *(u8*)((u8*)self + 0x30) = 1;
    }
}

// When the +0x31 state is 3, advance to 4, clear +0x33, run the scrollbar
// cleanup at +0x74 and play sound 6.
extern "C" void func_801D216C(void*, int);
extern "C" void func_8024371C(CMapSel* self) {
    if (*(u8*)((u8*)self + 0x31) == 3) {
        *(u8*)((u8*)self + 0x31) = 4;
        *(u8*)((u8*)self + 0x33) = 0;
        func_801D216C((u8*)self + 0x74, 0);
        func_80138078__FUl(6);
    }
}

/* func_80243768 - Move the selection up one step. When the top of a column
   is passed, step to the previous column; past the first column, wrap to the
   last map entry (grid has 10 rows; mGridData[0x20] is the map count). */
extern "C" void __declspec(noinline) func_80243768(CMapSel* self) {
    // u8 locals + (s8) sign tests reproduce the retail clrlwi/extsb. shape.
    u8 y = self->mSelY - 1;
    self->mSelY = y;
    if ((s8)y < 0) {
        self->mSelY = 0;
        u8 x = self->mSelX - 1;
        self->mSelX = x;
        if ((s8)x < 0) {
            u8 count = self->mGridData[0x20];
            if (count >= 10) {
                self->mSelY = 9;
                self->mSelX = count - 10;
            } else {
                u8 cy = count - 1;
                self->mSelY = cy;
                self->mSelX = 0;
                if ((s8)cy < 0) self->mSelY = 0;
            }
        }
    }
    func_80243CFC(self);
    func_80244020(self);
    func_801F3850(self->mScrollBar, (u16)(s8)self->mSelX);
    func_80243FC4(self);
    func_80138078__FUl(1);
}

/* func_80243838 - Move the selection right one column. Wraps from the bottom
   row of a column to the top of the next column; past the last column, wraps
   to the first entry. mGridData[0x20] is the map count (grid has 10 rows). */
extern "C" void __declspec(noinline) func_80243838(CMapSel* self) {
    u8 count = self->mGridData[0x20];
    if (count >= 10) {
        s8 y = (s8)((u8)(self->mSelY + 1));
        self->mSelY = y;
        if (y >= 10) {
            self->mSelY = 9;
            s8 x = (s8)((u8)(self->mSelX + 1));
            self->mSelX = x;
            if (x > (count - 10)) {
                self->mSelY = 0;
                self->mSelX = 0;
            }
        }
    } else {
        s8 y = (s8)((u8)(self->mSelY + 1));
        self->mSelY = y;
        if (y >= count) {
            self->mSelY = 0;
            self->mSelX = 0;
        }
    }
    func_80243CFC(self);
    func_80244020(self);
    func_801F3850(self->mScrollBar, self->mSelX);
    func_80243FC4(self);
    func_80138078__FUl(1);
}

void func_8024391C(){}

/* func_802439CC - Move the selection down one row (grid has 10 columns;
   mGridData[0x20] is the map count). Wraps past the last row to the next
   column and clamps the row index to 9. */
extern "C" void __declspec(noinline) func_802439CC(CMapSel* self) {
    // Grid has 10 rows; mGridData[0x20] is the map count. Signed (s8)
    // comparisons against the count mirror the retail cmpw/cmpwi forms.
    u8 count = self->mGridData[0x20];
    if (count >= 10) {
        u8 nx = self->mSelX + 10;
        self->mSelX = nx;
        if ((s8)nx > count - 10) {
            u8 ny = nx - (count - 10);
            self->mSelY = ny;
            self->mSelX = count - 10;
            if ((s8)ny >= 10) self->mSelY = 9;
        }
    } else {
        u8 ny = count - 1;
        self->mSelY = ny;
        self->mSelX = 0;
        if ((s8)ny < 0) self->mSelY = 0;
    }
    func_80243CFC(self);
    func_80244020(self);
    func_801F3850(self->mScrollBar, (u16)(s8)self->mSelX);
    func_80243FC4(self);
    func_80138078__FUl(1);
}

u8 func_80243A9C(CMapSelFull* self){
    s8 x = self->field_8D;
    s8 y = self->field_8C;
    return self->mGridData[x + y];
}

/* func_80243ABC - Finish the map-data load: when animation 1 completes,
   enable both animations, mark the widget visible (state 2), and configure
   the scrollbar for the current selection. mGridData[0x20] is the map count. */
extern "C" void __declspec(noinline) func_80243ABC(CMapSel* self) {
    if (func_80137444(self->mAnimTransform1, lbl_eu_8066873C) != 0) {
        self->mLayout->SetAnimationEnable(self->mAnimTransform1, false);
        self->mLayout->SetAnimationEnable(self->mAnimTransform2, true);
        self->mState = 2;
        float vec[3];
        vec[0] = lbl_eu_80668740;
        u8* sb = &self->mScrollBar[0];
        vec[1] = lbl_eu_80668744;
        float* vecPtr = &vec[0];
        vec[2] = lbl_eu_80668738;
        func_801F3670(sb, vecPtr);
        func_801F36BC(self->mScrollBar, 10, self->mGridData[0x20]);
        func_801F3850(self->mScrollBar, (u16)(s8)self->mSelX);
        func_801F367C(self->mScrollBar);
    }
}

extern "C" void __declspec(noinline) func_80243B88(CMapSel* self){}

/* func_80243BE8 - Poll animation completion on mAnimTransform2, then enable both
   animations, set state to 5 (post-close cleanup), and notify the scrollbar. */
extern "C" void __declspec(noinline) func_80243BE8(CMapSel* self) {
    if (func_80137510(self->mAnimTransform2, 1.0f) != 0) {
        self->mLayout->SetAnimationEnable(self->mAnimTransform2, false);
        self->mLayout->SetAnimationEnable(self->mAnimTransform1, true);
        self->mState = 5;
        func_801F369C(self->mScrollBar);
    }
}

extern "C" void __declspec(noinline) func_80243C6C(CMapSel* self) {
    if (func_80137510(self->mAnimTransform1, lbl_eu_8066873C) != 0) {
        *(u8*)((u8*)self + 0x33) = 1;
        self->mState = 0;
    }
}

/* func_80243CFC - Refresh the 10 grid cells' text. For each column index i
   (0..9), sprintf the pane name, then set the cell text from the BDAT map
   table; when the column matches the current selection X, also set the
   selected-row labels. Out-of-range columns are blanked with the placeholder
   string at +0x52. */
extern "C" void __declspec(noinline) func_80243CFC(CMapSel* self) {
    char buf[32];
    char* base = lbl_eu_8050B4A8;
    for (u32 i = 0; i < 10; i++) {
        sprintf(buf, base + 0x1f, (u8)i + 1);
        if ((s8)self->mSelX + (u8)i < self->mGridData[0x20]) {
            u8 g = self->mGridData[(s8)self->mSelX + (u8)i];
            char* res = func_8013639C(lbl_eu_806640A8, base + 0x2b, g);
            func_80136B4C(self->mLayout, buf, res, 0);
            if ((s8)self->mSelY == (u8)i) {
                func_80136B4C(self->mLayout, base + 0x30, res, 0);
                char* res2 = func_8013639C(lbl_eu_806640A8, base + 0x3a, g);
                func_80136B4C(self->mLayout, base + 0x46, res2, 0);
            }
        } else {
            func_80136B4C(self->mLayout, buf, base + 0x52, 0);
        }
    }
}

/* func_80243E08 - Show/hide the map-selection images. When no file is in
   flight and a map-data buffer is present and the load flag is clear, display
   the layout text pane for the current map; otherwise re-request the map
   texture from the arc and display its text pane. */
#pragma push
#pragma optimize_for_size on
extern "C" void __declspec(noinline) func_80243E08(CMapSel* self) {
    char* base;
    if (self->mFileHandle2 != 0 || self->mAllocatedMem == 0 ||
        self->mGridData[0x21] != 0) {
        base = lbl_eu_8050B4A8;
        const void* res = self->mArcAccessor->GetResource(
            nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE, base + 0x53, 0);
        if (res != 0) {
            func_80137E7C(self->mLayout, base + 0x66);
        }
        return;
    }
    base = lbl_eu_8050B4A8;
    func_80137E7C(self->mLayout, base + 0x66);
    nw4r::lyt::Pane* pane =
        self->mLayout->GetRootPane()->FindPaneByName(base + 0x66, true);
    func_80124270(pane, 1);
}
#pragma pop

/* func_80243ED8 - Count down the map-select transition timer; when it
   expires, tear down the previous map-data load (release the handle and the
   allocated buffer) and start loading the newly selected map's file. */
extern "C" void __declspec(noinline) func_80243ED8(CMapSel* self) {
    CDeviceVI::waitForDrawDone();
    if (self->mTimer > lbl_eu_80668738) {
        self->mTimer -= lbl_eu_8066873C;
        return;
    }
    if (self->mGridData[0x21] == 0) return;
    self->mGridData[0x21] = 0;
    func_801390E0(&self->mFileHandle2);
    if (self->mAllocatedMem != 0) {
        mtl::MemManager::deallocate(self->mAllocatedMem);
        self->mAllocatedMem = 0;
    }
    char buf[64];
    char* base = lbl_eu_8050B4A8;
    u8 idx = self->mGridData[(s8)self->mSelX + (s8)self->mSelY];
    u16 res = func_80136254(lbl_eu_806640A8, base + 0x6e, idx);
    char* name = func_80138F78(res);
    sprintf(buf, base + 0x79, name);
    self->mFileHandle2 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), buf,
        reinterpret_cast<IWorkEvent*>(self), 0, 0);
}

extern "C" void __declspec(noinline) func_80243FC4(CMapSel* self){}

/* func_80244020 - Refresh the cursor position on the map grid: format the
   current row's pane name from the selection Y, look up the two panes that
   bracket the selection, and move the cursor to the computed point. */
extern "C" void __declspec(noinline) func_80244020(CMapSel* self) {
    char buf[0x2C];
    nw4r::math::VEC3 out;
    char* base = lbl_eu_8050B4A8;
    sprintf(buf, base + 0x1f, self->mSelY + 1);
    nw4r::lyt::Pane* pane1 =
        self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(
        base + 0x88, true);
    func_80137924(&out, pane1, pane2, self->mLayout->GetRootPane());
    reinterpret_cast<CCur18View*>(&self->mCursor[0])->vf04(&out);
}

/* CMapSel::OnFileEvent - Build the map-select layout when the main file
   finishes loading: create the scratch mem region + host, detach the file
   buffer into the arc accessor, build the layout and both animations, bind
   the font, and copy a freshly-constructed CCur18 cursor into +0x74. If the
   event instead matches the second handle, adopt its buffer as mAllocatedMem. */
#pragma push
#pragma optimize_for_size on
bool CMapSel::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
        char* base = lbl_eu_8050B4A8;
        UnkClass_8045F564* region =
            reinterpret_cast<UnkClass_8045F564*>(&mMemRegion[0]);
        region->createRegion(mem2, 0x14000, base + 0x97, 0);
        Class_8045F858 memHost((UnkClass_8045F564*)(void*)&mMemRegion[0]);
        void* fileData = mFileHandle->getData();
        mtl::MemManager::func_80434A4C(false);
        mArcAccessor = CLibLayout::createArcResourceAccessor();
        mArcAccessor->Attach(fileData, base + 0x9f);
        func_80136E84(&mLayout, mArcAccessor, base + 0xa3);
        func_80136F08(mLayout, &mAnimTransform1, mArcAccessor, base + 0xbb);
        func_80136F08(mLayout, &mAnimTransform2, mArcAccessor, base + 0xd6);

        // Bind the font: take the layout root pane, ask the font object for
        // its pane, and push it back onto the root.
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        void* fontObj = CDeviceFont::func_80452C10(1, mLayout);
        u32 fontResult = static_cast<CDeviceFontView*>(fontObj)->vf7();
        func_8013676C(rootPane, fontResult);

        mLayout->SetAnimationEnable(mAnimTransform2, false);
        mLayout->SetAnimationEnable(mAnimTransform1, true);
        mLayout->Animate(0);

        // Build a CCur18 on the stack and copy it into +0x74, skipping the
        // vtable pointer (the embedded cursor keeps its own), then poke its
        // first virtual and run the post-build hook.
        CCur18Data temp;
        __ct__CCur18(&temp, func_801355F4());
        CCur18Data* dst = reinterpret_cast<CCur18Data*>(&mCursor[0]);
        dst->field_4 = temp.field_4;
        dst->field_8 = temp.field_8;
        dst->field_C = temp.field_C;
        dst->field_10 = temp.field_10;
        dst->field_14 = temp.field_14;
        dst->field_15 = temp.field_15;
        __dt__6CCur18Fv(&temp, -1);
        reinterpret_cast<CCur18View*>(&mCursor[0])->vf02();
        func_80243CB8(this);
        mFileHandle = 0;
        reinterpret_cast<UnkClass_8045F564*>(&mMemRegion[0])->func_8045F810();
        return true;
    }

    if (mFileHandle2 == event->mFileHandle) {
        u8* data = mFileHandle2->mData;
        mFileHandle2->mData = 0;
        mAllocatedMem = data;
        mFileHandle2 = 0;
        return true;
    }
    return false;
}
#pragma pop
