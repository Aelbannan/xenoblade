// Auto-scaffolded catalog TU for kyoshin/CTutorialList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTutorialList.hpp"
#include "kyoshin/CSortMenu.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/work/CEventFile.hpp"

#include <stdio.h>

u8 CTutorialList::func_802AD300() { return ((u8*)this)[0x177]; }


void __ct__CTutorialList(){}

// Deleting destructor (retail __dt__802ACBF0): free self when mode > 0,
// return self.
extern "C" void* __dt__802ACBF0(void* self, int mode) {
    if (self != 0 && mode > 0) {
        __dl__FPv(self);
    }
    return self;
}


// func_802AD308 - initialise the list widget: latch the state byte, clear the
// initialised flag, seed the scrollbar fade colours and sizes, then run the
// two per-frame helpers.
extern "C" void func_802AD308(CTutorialList* self) {
    if (self->mState175 != 0) return;
    self->mState175 = 1;
    self->mInitialized = 0;
    func_802ADC28(self);

    f32 v[3];
    v[0] = lbl_eu_80668DD8;
    v[1] = lbl_eu_80668DDC;
    v[2] = lbl_eu_80668DE0;
    func_801F3670(&self->mScrollBar, v);
    func_801F36BC(&self->mScrollBar, 0xa, self->mField280);
    func_801F3850(&self->mScrollBar, self->mField17A);
    func_802ADCE8(self);
    func_802ADE18(self);
}

// func_802AD3A0 - start closing the list once anim state 3 is reached:
// latch state 4, clear the initialised flag, swap the anim transforms,
// park the cursor, request the scrollbar scroll-out and play sound 6.
extern "C" void func_802AD3A0(CTutorialList* self) {
    if (self->mState175 == 3) {
        self->mState175 = 4;
        self->mInitialized = 0;
        func_802ADC88(self);
        func_801D216C(&self->mGap2C[0], 0);
        func_801F369C(&self->mScrollBar);
        func_80138078(6);
    }
}

// func_802AD404 - page up: wheel the sort menu back one page when idle; when
// the menu is still animating, decrement the page byte and wrap to the
// previous 10-row block of the content.
extern "C" void func_802AD404(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) != 0) {
        if (func_801D3328(&self->mSortMenu84[0]) == 0)
            return;
        nw4r::math::VEC3 pos;
        func_801D3620(&self->mSortMenu84[0]);
        func_801D3454(&pos, &self->mSortMenu84[0]);
        ((CTutorialCurView*)self->mGap2C)->vf2(&pos);
    } else {
        // idle page-up: wrap to the previous 10-row block of the content
        u8 page = self->mField178;
    u16 content = self->mField280;
        u8 cnt8 = (u8)content;
        self->mField178 = page - 1;
        if ((s8)(u8)(page - 1) < 0) {
            s16 sel = self->mField17A;
            self->mField178 = 0;
            self->mField17A = sel - 1;
            if (sel - 1 < 0) {
                if (cnt8 >= 10) {
                    self->mField178 = 9;
                    self->mField17A = cnt8 - 10;
                } else {
                    self->mField178 = cnt8 - 1;
                    self->mField17A = content;
                    if ((s8)(u8)(cnt8 - 1) < 0)
                        self->mField178 = 0;
                }
            }
        }
        func_802ADCE8(self);
        func_802ADE18(self);
        func_801F3850(&self->mScrollBar, self->mField17A);
    }
    func_80138078(1);
}

// func_802AD514 - page up (alt): same gate shape as func_802AD404 but with
// different wrap arithmetic on the page/selection bytes.
extern "C" void func_802AD514(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) != 0) {
        if (func_801D3328(&self->mSortMenu84[0]) == 0)
            return;
        nw4r::math::VEC3 pos;
        func_801D3698(&self->mSortMenu84[0]);
        func_801D3454(&pos, &self->mSortMenu84[0]);
        ((CTutorialCurView*)self->mGap2C)->vf2(&pos);
    } else {
        // idle page-up: step the page byte and wrap to the previous block
        u8 count = (u8)self->mField280;
        if (count >= 10) {
            self->mField178 = self->mField178 + 1;
            if ((s8)self->mField178 >= 10) {
                int limit = count - 10;
                self->mField178 = 9;
                self->mField17A = (s16)self->mField17A + 1;
                if ((int)(s16)self->mField17A > limit) {
                    self->mField178 = 0;
                    self->mField17A = 0;
                }
            }
        } else {
            self->mField178 = self->mField178 + 1;
            if ((s8)(u8)self->mField178 >= count) {
                self->mField178 = 0;
                self->mField17A = 0;
            }
        }
        func_802ADCE8(self);
        func_802ADE18(self);
        func_801F3850(&self->mScrollBar, self->mField17A);
    }
    func_80138078(1);
}

// func_802AD638 - page down: advance one page while the sort menu is busy;
// otherwise wheel it forward and move the cursor onto the new entry.
extern "C" void func_802AD638(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) != 0) {
        if (func_801D3328(&self->mSortMenu84[0]) == 0)
            return;
        nw4r::math::VEC3 pos;
        func_801D3724(&self->mSortMenu84[0]);
        func_801D3454(&pos, &self->mSortMenu84[0]);
        ((CTutorialCurView*)self->mGap2C)->vf2(&pos);
    } else {
        u8 count = (u8)self->mField280;
    if (count >= 10) {
        s16 sel = self->mField17A;
        s16 v = sel - 10;
        self->mField17A = v;
        if (v < 0) {
            u8 b = v + 9;
            self->mField178 = b;
            self->mField17A = 0;
            if ((s8)b < 0)
                self->mField178 = 0;
        }
    } else {
        self->mField178 = 0;
        self->mField17A = 0;
    }
    func_802ADCE8(self);
    func_802ADE18(self);
    func_801F3850(&self->mScrollBar, self->mField17A);
    }
    func_80138078(1);
}

// func_802AD728 - page down (alt): busy path wheels the menu forward; idle
// path advances the selection by 10 and re-derives the page byte.
extern "C" void func_802AD728(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) != 0) {
        if (func_801D3328(&self->mSortMenu84[0]) == 0)
            return;
        nw4r::math::VEC3 pos;
        func_801D377C(&self->mSortMenu84[0]);
        func_801D3454(&pos, &self->mSortMenu84[0]);
        ((CTutorialCurView*)self->mGap2C)->vf2(&pos);
    } else {
        // idle page-down: advance one full 10-row block
        u8 count = (u8)self->mField280;
        if (count >= 10) {
            int limit = count - 10;
            s16 v = (s16)self->mField17A + 10;
            self->mField17A = v;
            if ((int)v > limit) {
                u8 d = (u8)((int)v - limit);
                self->mField178 = d;
                self->mField17A = limit;
                if ((s8)d >= 10)
                    self->mField178 = 9;
            }
        } else {
            count = count - 1;
            self->mField178 = count;
            self->mField17A = 0;
            if ((s8)(u8)count < 0)
                self->mField178 = 0;
        }
        func_802ADCE8(self);
        func_802ADE18(self);
        func_801F3850(&self->mScrollBar, self->mField17A);
    }
    func_80138078(1);
}

struct CTutorialWindowIds;
extern "C" u16 func_802ACE04(CTutorialWindowIds* self, u16 index);

extern "C" u16 func_802AD838(CTutorialList* self) {
    return func_802ACE04((CTutorialWindowIds*)self->mSubObj180,
                         (u16)((s8)self->mField178 + (s16)self->mField17A));
}

// func_802ADCE8 (retail 0x130 bytes, recovered): per-frame tutorial list
// upkeep.  For each of the 10 rows: format the icon pane name ("pic_icon%02d")
// and the title pane name ("txt_tit%02d"), resolve the icon pane through the
// layout root-pane vtable call, and either push the title text directly
// (empty string) or walk the text-lookup path keyed on the window-id table.
extern "C" void func_80124270(void*, u32);
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" __declspec(noinline) void func_802ADCE8(CTutorialList* self) {
    for (u32 i = 0; i < 10; i++) {
        char buf[0x20];
        // Retail computes the window-id index early (live across the layout
        // calls, kept in callee-saved r28); the mField17A read sign-extends
        // (lha).
        u8 idx = (u8)(i + (s16)self->mField17A);
        sprintf(buf, &lbl_eu_80510B78[0x5d], (u8)i + 1);
        nw4r::lyt::Pane* pane = self->mLayout20->GetRootPane();
        nw4r::lyt::Pane* text = pane->FindPaneByName(buf, true);
        func_80124270(text, 0);
        sprintf(buf, &lbl_eu_80510B78[0x6a], (u8)i + 1);
        u16 sel = func_802ACE04((CTutorialWindowIds*)self->mSubObj180, idx);
        if (sel == 0) {
            func_80136B4C(self->mLayout20, buf, &lbl_eu_80510B78[0x76], 0);
        } else {
            // Retail passes only two args here (the third header param is
            // unused at this site); cast to the 2-arg form so no li r5 is
            // emitted.
            char* txt = ((char* (*)(const void*, const void*))func_8013639C)(
                (const void*)lbl_eu_80664BF0, &lbl_eu_80510B78[0x77]);
            func_80136B4C(self->mLayout20, buf, txt, 0);
            if (func_801361E8(lbl_eu_80664BF0, &lbl_eu_80510B78[0x7d], sel) == 0) {
                if (func_8009CF8C((u16)sel + 0x33bf) == 0) {
                    func_80124270(text, 1);
                }
            }
        }
    }
}
extern "C" void func_802AD854(void* self) { func_802ADCE8((CTutorialList*)self); }

// func_802AD858 - confirm/open: while the sort menu is animating just refresh
// cursor/page text; on first open resolve the two anchor panes around the
// scrollbar, place the sort menu there, rebuild its entries and move the
// cursor onto the selected row.
extern "C" void func_802AD858(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) != 0) {
        if (func_801D3328(&self->mSortMenu84[0]) == 0)
            return;
        func_801D216C(&self->mGap2C[0], 1);
        func_801D3408(&self->mSortMenu84[0]);
        func_802ADFA8(self);
        func_802ADE18(self);
        func_80138078(6);
        return;
    }
    nw4r::math::VEC3 pos;
    nw4r::lyt::Pane* root = self->mLayout20->GetRootPane();
    nw4r::lyt::Pane* barPane = root->FindPaneByName(&lbl_eu_80510B78[0x4e], true);
    func_80137924(&pos,
                  root->FindPaneByName(&lbl_eu_80510B78[0x46], true),
                  barPane,
                  root);
    func_801D3430(&self->mSortMenu84[0], &pos);
    func_802ADEE4(self);
    func_801D216C(&self->mGap2C[0], 1);
    func_801D3330(&self->mSortMenu84[0]);
    nw4r::math::VEC3 curPos;
    func_801D3454(&curPos, &self->mSortMenu84[0]);
    ((CTutorialCurView*)self->mGap2C)->vf2(&curPos);
    func_80138078(2);
}

u8 CTutorialList::func_802AD984() { return (u8)func_801D3320(&mSortMenu84[0]); }

// func_802AD98C - advance the list: gate on the sort-menu active/button flags,
// move the cursor, refresh the sort menu, run the per-frame helpers and (when
// requested) play the confirm sound.
extern "C" __declspec(noinline) void func_802AD98C(CTutorialList* self, int arg) {
    if (func_801D3320(&self->mSortMenu84[0]) == 0) return;
    if (func_801D3328(&self->mSortMenu84[0]) == 0) return;
    func_801D216C(&self->mGap2C[0], 1);
    func_801D3408(&self->mSortMenu84[0]);
    func_802ADFA8(self);
    func_802ADE18(self);
    if (arg == 0) {
        func_80138078(6);
    }
}

// func_802ADA0C - rebuild the list content once the sort menu finished
// loading: gate on the sort-menu active/button flags, latch the page byte,
// seed the sub-object at +0x180, reset the page/selection ids, size the
// scrollbar to the content and play the confirm sound.
extern "C" void func_802ADA0C(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) == 0) return;
    if (func_801D3328(&self->mSortMenu84[0]) == 0) return;
    s32 page = func_801D3808(&self->mSortMenu84[0]);
    self->mField17E = (s8)page;
    func_802ACC30(self->mSubObj180, 0, (u8)page);
    self->mField178 = 0;
    self->mField17A = 0;
    func_801F36BC(&self->mScrollBar, 0xa, self->mField280);
    func_801F3850(&self->mScrollBar, self->mField17A);
    func_802AD98C(self, 1);
    func_802ADCE8(self);
    func_80138078(3);
}

extern "C" int func_802ADAB8(void* self) {
    // existing decl: extern "C" unsigned char func_801D3320(void*)
    return (func_801D3320((u8*)self + 0x84) != 0) + 0x73;
}

// Animation-finish handlers: the +0x24/+0x28 anim transform reached its end
// frame (bound in .sdata2) -> latch the state byte and run the follow-up.
extern "C" void func_802ADAE8(CTutorialList* self) {
    if (func_80137444(self->mAnim24, lbl_eu_80668DE4) != 0) {
        self->mState175 = 2;
        func_802ADC88(self);
        func_801F367C(&self->mScrollBar);
    }
}

extern "C" void func_802ADB3C(CTutorialList* self) {
    if (func_80137444(self->mAnim28, lbl_eu_80668DE4) != 0) {
        self->mState175 = 3;
        func_802ADE18(self);
        self->mInitialized = 1;
    }
}

extern "C" __declspec(noinline) void func_802ADB90(CTutorialList* self) {
    if (func_80137510(self->mAnim28, lbl_eu_80668DE4) != 0) {
        self->mState175 = 5;
        func_802ADC28(self);
    }
}

extern "C" void func_802ADBDC(CTutorialList* self) {
    if (func_80137510(self->mAnim28, lbl_eu_80668DE4) != 0) {
        self->mState175 = 0;
        self->mInitialized = 1;
    }
}

// Disable the +0x28 anim transform and enable the +0x24 one
// (SetAnimationEnable, vtable slot at +0x2C).
extern "C" __declspec(noinline) void func_802ADC28(CTutorialList* self) {
    self->mLayout20->SetAnimationEnable(self->mAnim28, false);
    self->mLayout20->SetAnimationEnable(self->mAnim24, true);
}

// Re-enable the two anim transforms on the layout (SetAnimationEnable,
// vtable slot 11 = 0x2C): mAnim24 disabled, mAnim28 enabled.
extern "C" __declspec(noinline) void func_802ADC88(CTutorialList* self) {
    self->mLayout20->SetAnimationEnable(self->mAnim24, false);
    self->mLayout20->SetAnimationEnable(self->mAnim28, true);
}

// func_802ADE18 - move the cursor onto the current tutorial row: format the
// page number, resolve the root pane, find the page-name pane and the
// scrollbar thumb pane, ask func_80137924 for the position between them and
// feed it to the cursor's Move virtual (vtable +0x10).
extern "C" __declspec(noinline) void func_802ADE18(CTutorialList* self) {
    char name[0x2C];
    nw4r::math::VEC3 pos;
    sprintf(name, &lbl_eu_80510B78[0x82], (int)self->mField178 + 1);
    func_801D216C(&self->mGap2C[0], 1);
    nw4r::lyt::Pane* pagePane =
        self->mLayout20->GetRootPane()->FindPaneByName(name, true);
    nw4r::lyt::Pane* barPane =
        self->mLayout20->GetRootPane()->FindPaneByName(&lbl_eu_80510B78[0x4e], true);
    func_80137924(&pos, pagePane, barPane, self->mLayout20->GetRootPane());
    ((CTutorialCurView*)self->mGap2C)->vf2(&pos);
}

// func_802ADEE4 - rebuild the sort menu entries: reset the menu, push the
// four tutorial-row labels (text ids 0x25-0x28) and select the current page.
// noinline: retail calls this from func_802AD858 rather than inlining.
extern "C" __declspec(noinline) void func_802ADEE4(CTutorialList* self) {
    if (func_801D3320(&self->mSortMenu84[0]) != 0) return;
    func_801D350C(&self->mSortMenu84[0]);
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x25));
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x26));
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x27));
    func_801D3518(&self->mSortMenu84[0],
                  func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], 0x28));
    func_801D353C(&self->mSortMenu84[0], self->mField17E);
}

extern "C" __declspec(noinline) void func_802ADFA8(CTutorialList* self) {
    char* text = func_80136190(&lbl_eu_80510B78[0x8e], &lbl_eu_80510B78[0x97], self->mField17E + 0x25);
    func_80136B4C(self->mLayout20, &lbl_eu_80510B78[0x9c], text, 0);
}

// func_802AE004 - open the tutorial list once the layout resource finished
// loading: flag the visible selection, seed the sub-object at +0x180, copy the
// row/selection ids and run the per-frame helper.
extern "C" void func_802AE004(CTutorialList* self) {
    if (self->mField1C == 0 || self->mField18 != 0) return;
    self->mField176 = 1;
    self->mField174 = 1;
    func_802ACC30(self->mSubObj180, self->mField17C, 0);
    self->mField178 = (s8)self->mField282;
    self->mField17A = (u16)(s32)self->mField284;
    func_802ADCE8(self);
}

// Destructor (retail __dt__13CTutorialListFv): tear down the embedded
// sub-objects in reverse construction order, free the storage when
// flags > 0, and return self.  Written under C linkage so MWCC does not
// emit its own vtable-store/deleting wrapper.
extern "C" CTutorialList* __dt__13CTutorialListFv(CTutorialList* _this,
                                                  int flags) {
    if (_this != 0) {
        __dt__9CSortMenuFv(&_this->mSortMenu84[0], -1);
        __dt__10CScrollBarFv(&_this->mScrollBar, -1);
        __dt__6CCur18Fv(&_this->mGap2C[0], -1);
        __dt__17UnkClass_8045F564Fv(&_this->mGap04[0], -1);
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}

// File-load completion callback: on the layout-archive handle, rebuild the
// whole scene (region guard, arc accessor, layout + two anims, font binding,
// message pane sizing, embedded cursor); on the shared-archive handle, hand
// its data to the BDAT layer and latch the shared table pointer.
int CTutorialList::OnFileEvent(CEventFile* event) {
    if (mField14 == event->mFileHandle) {
        // Layout archive finished loading: rebuild the whole scene.
        u8 regionBuf[8];
        void* mem2 = getHandleMEM2__Q23mtl10MemManagerFv();
        char* const s = lbl_eu_80510B78;
        createRegion__17UnkClass_8045F564FiiPCci(&mGap04[0], (int)mem2, 0x10000,
                                                 &s[0xa4], 0);
        __ct__14Class_8045F858FP17UnkClass_8045F564(regionBuf, &mGap04[0]);

        // getData() reads and clears the handle's buffer pointer.
        void* fileData = ((CFileHandle*)mField14)->getData();
        func_80434A4C__Q23mtl10MemManagerFb(false);

        mField1C = createArcResourceAccessor__10CLibLayoutFv();
        Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(
            (nw4r::lyt::ArcResourceAccessor*)mField1C, fileData, &s[0xb2]);

        func_80136E84(&mLayout20, (nw4r::lyt::ArcResourceAccessor*)mField1C,
                      &s[0xb6]);
        func_80136F08(mLayout20, &mAnim24,
                      (nw4r::lyt::ArcResourceAccessor*)mField1C, &s[0xcf]);
        func_80136F08(mLayout20, &mAnim28,
                      (nw4r::lyt::ArcResourceAccessor*)mField1C, &s[0xeb]);

        // Bind the device font onto the layout's root pane. Retail hoists
        // the root-pane fetch (+0x10 raw field) above the font lookup.
        void* rootPane = *(void**)((char*)mLayout20 + 0x10);
        void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mLayout20);
        void** fontVtbl = *(void***)fontObj;
        u32 fontResult = ((u32 (*)(void*))fontVtbl[0x24 / 4])(fontObj);
        func_8013676C(rootPane, fontResult);

        func_802ADC28(this);
        mLayout20->Animate(0);

        char* title = func_80136190(&s[0x8e], &s[0x97], 0x24);
        func_80136B4C(mLayout20, &s[0x10c], title, 0);

        // Pick the message archive variant from the game-manager mode.
        const char* sel = func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0
                              ? &s[0x117]
                              : &s[0x120];
        u16 msgId = func_8013606C(&s[0x129], sel, 0x61);
        char* texName = func_80138F78((u32)msgId);
        CTutorialMsgObj* obj =
            (CTutorialMsgObj*)func_801355F4()->GetResource(0x74696d67, texName, 0);
        if (obj != 0) {
            func_80137E7C(mLayout20, &s[0x137], obj);
            CTutorialCoords* coords = obj->chain->pCoords;
            u16 row = coords->c2;
            u16 col = coords->c0;
            nw4r::lyt::Pane* pane = *(nw4r::lyt::Pane**)((char*)mLayout20 + 0x10);
            pane = pane->FindPaneByName(&s[0x137], true);
            if (pane != 0) {
                // u16->f32 via the 2^52 magic: build 0x43300000_<v> on the
                // stack and subtract the named .sdata2 double so the pool
                // reloc stays on lbl_eu_80668DE8 (a plain (f32) cast
                // synthesizes a TU-local @N entry).
                union {
                    struct { u32 hi; u32 lo; } w;
                    double d;
                } conv[2];
                const double magic = lbl_eu_80668DE8;
                conv[0].w.hi = 0x43300000u;
                conv[0].w.lo = row;
                conv[1].w.hi = 0x43300000u;
                conv[1].w.lo = col;
                float src[2];
                src[0] = (f32)(conv[0].d - magic);
                src[1] = (f32)(conv[1].d - magic);
                reinterpret_cast<PaneSizeRegion*>(pane)->width = src[0];
                reinterpret_cast<PaneSizeRegion*>(pane)->height = src[1];
            }
        }

        func_802ADFA8(this);

        // Build the cursor on the stack, copy its body into +0x2C (skipping
        // the vtable), destroy the temp and run the cursor init virtual.
        u8 tmpCur[0x18];
        __ct__CCur18(tmpCur, func_801355F4());
        CTutorialCur18Data* curDst =
            reinterpret_cast<CTutorialCur18Data*>(&mGap2C[0]);
        CTutorialCur18Data* curSrc =
            reinterpret_cast<CTutorialCur18Data*>(tmpCur);
        curDst->field_4 = curSrc->field_4;
        curDst->field_8 = curSrc->field_8;
        curDst->field_C = curSrc->field_C;
        curDst->field_10 = curSrc->field_10;
        curDst->field_14 = curSrc->field_14;
        curDst->field_15 = curSrc->field_15;
        __dt__6CCur18Fv(tmpCur, -1);
        ((CTutorialCurView*)mGap2C)->vf1();

        func_802AE004(this);
        mField14 = 0;
        func_8045F810__17UnkClass_8045F564Fv(&mGap04[0]);
        __dt__14Class_8045F858Fv(regionBuf, -1);
        return 1;
    }
    if (mField18 == event->mFileHandle) {
        // Shared archive hit: release its buffered data into the BDAT layer.
        void* data2 = ((CFileHandle*)mField18)->getData();
        func_8003AA78__5CBdatFUlPv(4, data2);
        func_8003AA34();
        lbl_eu_80664BF0 = (u32)getFP__FPCc(&lbl_eu_80510B78[0x143]);
        mField18 = 0;
        func_802AE004(this);
        return 1;
    }
    return 0;
}

// func_802AE38C - play the tutorial voice: gate on the handle's +0x3F00
// bit-1 flag, the voice-idle virtual (vtable slot 0x2BC), the battle-state
// mapping and a 0xA0 handle-buffer allocation, then play voice 0xDD2 through
// the embedded +0x3E9C sub-object. Always returns 0.
extern "C" int func_802AE38C(CVoiceHandle* self) {
    if ((self->field_0x3F00 & 0x2) == 0) return 0;
    if (((CVoiceHandleVTV*)self)->isActive() != 0) return 0;
    if (func_802A77E8(self) != 5) return 0;
    if (func_802A330C(0xa0, 1) == 0) return 0;
    CCharVoice* voice = (CCharVoice*)self;
    if (self != 0) voice = &self->voice;
    func_802A3D54(voice, 0xdd2, 0xa0);
    return 0;
}

extern "C" void func_802ACBDC(u8* self) {
    *(unsigned short*)(self + 0x100) = 0;
    *(unsigned short*)(self + 0x102) = 0;
    *(unsigned short*)(self + 0x104) = 0;
}

// func_802AD060 - file loading: request the list file and the common archive
// file, then initialise the scrollbar and the sort-menu sub-object.
extern "C" void func_802AD060(CTutorialList* self) {
    self->mField14 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_80510B78[0xf], self, 0, 0);
    self->mField18 = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (unsigned long)func_800A9D90(), &lbl_eu_80510B78[0x28], self, 0, 0);
    func_801F34F4(&self->mScrollBar);
    func_801D3064(&self->mSortMenu84[0]);
    self->mField176 = 0;
}

// func_802AD0E0 - per-frame update: dispatch on the anim state byte, animate
// the layout, then move the cursor, scrollbar and sort-menu sub-object.
extern "C" void func_802AD0E0(CTutorialList* self) {
    if (self->mField174 == 0) return;
    switch (self->mState175) {
    case 1:
        func_802ADAE8(self);
        break;
    case 2:
        func_802ADB3C(self);
        break;
    case 4:
        func_802ADB90(self);
        break;
    case 5:
        func_802ADBDC(self);
        break;
    }
    self->mLayout20->Animate(0);
    func_801D202C(&self->mGap2C[0]);
    func_801F3540(&self->mScrollBar);
    func_801D3160(&self->mSortMenu84[0]);
}

extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801D31F8(void*, void*);
extern "C" void func_801D20B0(void*, nw4r::lyt::DrawInfo*);

// Render the tutorial list when the visible gate byte is set: draw the
// layout, scrollbar, sort menu and cursor with the given draw info.
#pragma optimize_for_size on
extern "C" void func_802AD188(CTutorialList* self,
                              nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField174 != 0) {
        func_80137038(self->mLayout20, drawInfo, 0, 1);
        func_801F35B0(&self->mScrollBar, drawInfo);
        func_801D31F8(&self->mSortMenu84[0], drawInfo);
        func_801D20B0(&self->mGap2C[0], drawInfo);
    }
}
#pragma optimize_for_size off
// func_802AD1F4 - release the list widget: free the CBdat index, close the
// two file handles, release the bound layout and resource accessor, then run
// the cursor/scrollbar/sort-menu/region teardown helpers.
extern "C" void func_802AD1F4(CTutorialList* self) {
    func_8003AA8C__5CBdatFUl(4);
    func_801390E0((CFileHandle**)&self->mField14);
    func_801390E0((CFileHandle**)&self->mField18);
    self->mField174 = 0;
    if (self->mLayout20 != 0) {
        delete self->mLayout20;
        self->mLayout20 = 0;
    }
    func_80139124((nw4r::lyt::ArcResourceAccessor*)self->mField1C);
    lbl_eu_80664BF0 = 0;
    ((CTutorialCurView*)self->mGap2C)->vf1();
    func_801F35DC(&self->mScrollBar);
    func_801D3258(&self->mSortMenu84[0]);
    func_8045F778__17UnkClass_8045F564Fv(&self->mGap04[0]);
}

struct CTutorialWindowIds {
    u16 mIds[0x80];   // 0x00: collected tutorial ids
    u16 mCount;       // 0x100: number of entries
    u16 mSelRow;      // 0x102: row of the selected entry within its page
    s16 mScrollBase;  // 0x104: scroll offset of the selected entry's page
};

// func_802ACC30 - rebuild the tutorial id table in the +0x180 sub-object:
// collect every active tutorial whose BDAT category matches the filter
// (all of them when filter==0), sort ascending by name key, then locate the
// target id to derive the row/scroll split stored after the table.
extern "C" __declspec(noinline) void func_802ACC30(u8* self, u16 target, int filter) {
    if (lbl_eu_80664BF0 == 0) return;
    CTutorialWindowIds* list = (CTutorialWindowIds*)self;
    list->mCount = 0;
    void* table = (void*)lbl_eu_80664BF0;
    u32 total = func_8003B1EC(table);
    for (int id = 1; (u32)id <= total; id++) {
        if (func_8009CF8C((u32)id + 0x333f) == 0) continue;
        if (filter != 0 &&
            (int)func_801361E8((u32)table, lbl_eu_80510B78, (u32)id) != filter - 1)
            continue;
        list->mIds[list->mCount++] = (u16)id;
    }
    // Bubble-sort ascending by each entry's name lookup key.
    for (int i = 0; i < list->mCount;) {
        int swapped = 0;
        for (int j = 0; j < (int)(list->mCount - 1 - i); j++) {
            u16 a = list->mIds[j];
            u16 b = list->mIds[j + 1];
            if ((u32)func_801361E8((u32)table, &lbl_eu_80510B78[9], a) >
                (u32)func_801361E8((u32)table, &lbl_eu_80510B78[9], b)) {
                a ^= b;
                b ^= a;
                a ^= b;
                list->mIds[j] = a;
                list->mIds[j + 1] = b;
                swapped = 1;
            }
        }
        if (swapped == 0) break;
        i++;
    }
    if (target != 0) {
        // Locate the target id and derive its page row / scroll base.
        s16 pos = 0;
        for (u16 j = 0; j < list->mCount; j++) {
            if (list->mIds[j] == target) {
                pos = j;
                break;
            }
        }
        list->mSelRow = pos >= 10 ? 9 : pos;
        s16 base = pos - 9;
        list->mScrollBase = base >= 0 ? base : 0;
    }
}

extern "C" __declspec(noinline) u16 func_802ACE04(CTutorialWindowIds* self, u16 index) {
    if (index >= self->mCount)
        return 0;
    return self->mIds[index];
}

u8 CTutorialList::func_802AD2A4() {
    // Inner test written as != so MWCC emits beq-forward + li-tail like retail.
    if (CScrollBar_isVisible(&mScrollBar) == 0)
        return 0;
    if (func_801D32DC(mSortMenu84) != 0)
        return mField176;
    return 0;
}
