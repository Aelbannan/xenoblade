// Auto-scaffolded catalog TU for kyoshin/CCollepedia
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CCollepedia.hpp"
#include <revolution/tpl/TPL.h>
#include "nw4r/lyt.h"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"

// C-linkage imports (functions + data from other TUs) live in CCollepedia.hpp
// (see the "C-linkage imports" section at the end of that header).

// These functions are declared in code_80135FDC.hpp

int sprintf(char*, const char*, ...);

// Forward declarations for stubs defined at end of file
extern "C" void func_8025406C(u8*);
extern "C" u8 func_802540DC(u8*);
extern "C" u32 func_80254144(u8*);
extern "C" __declspec(noinline) void func_80255F98(CCollepedia*);
extern "C" __declspec(noinline) void func_8025641C();
extern "C" void func_8025629C(CCollepedia*);
extern "C" void func_80256314(CCollepedia*);
extern "C" void func_8025516C(CCollepedia*);
extern "C" void func_80255210(CCollepedia*);
// Forward declarations for functions defined before target callers
extern "C" __declspec(noinline) void func_80253A14(CCollepedia*);
extern "C" __declspec(noinline) void func_80253A60(CCollepedia*);
extern "C" void func_802533F4(CCollepedia*);
extern "C" u16 func_80254204(u8*, u32, u32);
extern "C" u8 func_8025440C(u8*, u32, u32);
extern "C" void func_802545C0(u8*);
extern "C" void func_802557E0(CCollepedia*);
extern "C" void func_80255894(CCollepedia*);
extern "C" void func_8025592C(CCollepedia*);
extern "C" void func_80255984(CCollepedia*);
extern "C" void func_802559DC(CCollepedia*);
extern "C" void func_80255AB4(CCollepedia*);
extern "C" void func_80255B60(CCollepedia*);
extern "C" void func_80255C28(CCollepedia*);


// C-linkage declarations for functions from code_80135FDC.cpp to avoid reloc name drift
// (provided by code_80135FDC.hpp in the current tree)

// CCLPCur constructor - sets vtable, stores accessor, zeros everything
// noinline: retail callers invoke this out-of-line.
extern "C" __declspec(noinline) void __ct__CCLPCur(CCLPCur* self, nw4r::lyt::ArcResourceAccessor* pAccessor) {
    self->mVtable = lbl_eu_80537474;
    self->mArcResAcc = pAccessor;
    self->mpLayout = nullptr;
    self->mpAnimTrans0 = nullptr;
    self->mpAnimTrans1 = nullptr;
    self->mActive = 0;
    self->mVisible = 0;
}

extern "C" void* __dt__802531C4(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

// us-80255440
// Initialize CCollepedia layout: load layout, bind animations, position panes
void func_80253204(CCollepedia* this_) {
    buildLayout(&this_->field_8,
        (nw4r::lyt::ArcResourceAccessor*)this_->field_4,
        &lbl_eu_8050C6E8[0x00]);
    bindLayoutAnimTransform(this_->field_8, &this_->field_c_ptr,
        (nw4r::lyt::ArcResourceAccessor*)this_->field_4,
        &lbl_eu_8050C6E8[0x18]);

    // Set up position for first pane
    float vec1[3];
    code80135FDC_setVec3(vec1, lbl_eu_806687F0, lbl_eu_806687F4, lbl_eu_806687F8);

    // Find first pane and copy position
    nw4r::lyt::Pane* pane1 = this_->field_8->GetRootPane()->FindPaneByName(
        &lbl_eu_8050C6E8[0x35], true);
    copyVEC3((float*)((u8*)pane1 + 0x2C), vec1);

    // Set up position for second pane
    float vec2[3];
    code80135FDC_setVec3(vec2, lbl_eu_806687FC, lbl_eu_806687F4, lbl_eu_806687F8);

    // Find second pane and copy position
    nw4r::lyt::Pane* pane2 = this_->field_8->GetRootPane()->FindPaneByName(
        &lbl_eu_8050C6E8[0x41], true);
    copyVEC3((float*)((u8*)pane2 + 0x2C), vec2);

    // Call UnbindAllAnimation, then bind specific ones
    this_->field_8->UnbindAllAnimation();

    func_802533F4(this_);
}

extern "C" __declspec(noinline) void func_802532FC(CBaseCur* this_) {
    if (this_->mpLayout == nullptr) return;
    if (this_->mVisible == 0) {
        advanceAnimTransform(this_->mpAnimTrans0, lbl_eu_80668800);
    }
    this_->mpLayout->Animate(0);
}

// us-80257984
// Check if animation at field_3C is done, then disable/enable animations and set state
extern "C" __declspec(noinline) void func_80255748(CCollepedia* this_) {
    if (advanceAnimTransform(this_->field_3C, lbl_eu_80668800) != 0) {
        this_->field_38->SetAnimationEnable(this_->field_44, false);
        this_->field_38->SetAnimationEnable(this_->field_3C, false);
        this_->field_38->SetAnimationEnable(this_->field_40, true);
        this_->field_49 = 2;
    }
}

// Render layout if active and non-null
extern "C" __declspec(noinline) void func_80253360(CBaseCur* this_, nw4r::lyt::DrawInfo* drawInfo) {
    if (this_->mpLayout == nullptr) return;
    if (this_->mActive == 0) return;
    drawLayout(this_->mpLayout, drawInfo, 0, 1);
}

// noinline: retail callers (func_80254C04) invoke this out-of-line
extern "C" __declspec(noinline) void func_8025338C(CBaseCur* this_) {
    this_->mActive = 0;
    this_->mpAnimTrans0 = nullptr;
    this_->mpAnimTrans1 = nullptr;
    if (this_->mpLayout != nullptr) {
        delete this_->mpLayout;
        this_->mpLayout = nullptr;
    }
}

// us-80255630
// Unbind animation, bind new animation, enable animation, set frame to 0, animate
extern "C" __declspec(noinline) void func_802533F4(CCollepedia* this_) {
    this_->field_8->UnbindAnimation(this_->field_10_ptr);
    this_->field_8->BindAnimation(this_->field_c_ptr);
    this_->field_8->SetAnimationEnable(this_->field_c_ptr, true);
    this_->field_c_ptr->SetFrame(lbl_eu_806687F8);
    this_->field_8->Animate(0);
}

void CCollepedia_reset(CCollepedia* obj, int val) {
    obj->field_0 = val;
    obj->field_4 = 0;
    obj->field_8 = 0;
    obj->field_c_ptr = 0;
    reinterpret_cast<u8*>(&obj->field_c_ptr)[1] = 1;
}

extern "C" void* __dt__802534B0(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

// Target us-8025572c
// Finish second-page (detail view) setup after func_8025348C: load the detail
// layout, bind its animation, install the device font, fill the fixed text
// panes, then resolve the sample texture and size its pane.
// Shape mirrors the FULL_MATCHed CExchangeWin layout-init function.
// optimize_for_size: retail uses the _savegpr_29/_restgpr_29 frame.
//
// OPEN ITEM (single reloc-name drift): the u16->f32 pane-size casts pool the
// 2^52 conversion magic as TU-local @N in .sdata2; retail references the
// named lbl_eu_80668808. Source cannot name it (MWCC will not pool plain
// casts onto a declared symbol - see the CArtsInfo ConvS32ToF64 note), so
// this needs the standard pool_patterns rule in tools/postprocess_reloc_names.py:
//   "CCollepedia.o": UnitRules(
//       pool_patterns=((struct.pack(">II", MAGIC_HI, 0), "lbl_eu_80668808"),),
//       trim_sdata2_size=0,
//   ),
// With that rule the function is byte-identical (mismatch: 0).
#pragma push
#pragma optimize_for_size on
void func_802534F0(CLPPageSetup* pg) {
    buildLayout(&pg->mpLayout, pg->mpAccessor, &lbl_eu_8050C6E8[0x4d]);
    bindLayoutAnimTransform(pg->mpLayout, &pg->mpAnimTrans, pg->mpAccessor, &lbl_eu_8050C6E8[0x66]);

    // Bind the device font: root pane first (retail loads it before the
    // CDeviceFont call), then fetch the font handle from vtable slot 9.
    nw4r::lyt::Pane* rootPane = pg->mpLayout->GetRootPane();
    void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, pg->mpLayout);
    u32 fontHandle = static_cast<CLPFontView*>(fontObj)->sf9();
    func_8013676C(rootPane, fontHandle);

    char* tagStr = (char*)func_801355BC();
    setLayoutTextBoxFont(pg->mpLayout, &lbl_eu_8050C6E8[0x82], (u32)tagStr);
    setLayoutTextBoxFont(pg->mpLayout, &lbl_eu_8050C6E8[0x92], (u32)tagStr);

    pg->mpLayout->SetAnimationEnable(pg->mpAnimTrans, true);
    pg->mpLayout->Animate(0);

    // Fixed description/label panes: five category labels plus the header.
    func_80136B4C(pg->mpLayout, &lbl_eu_8050C6E8[0xb3], func_80136190(&lbl_eu_8050C6E8[0xa2], &lbl_eu_8050C6E8[0xae], 12), 0);
    func_80136B4C(pg->mpLayout, &lbl_eu_8050C6E8[0xc0], func_80136190(&lbl_eu_8050C6E8[0xa2], &lbl_eu_8050C6E8[0xae], 15), 0);
    func_80136B4C(pg->mpLayout, &lbl_eu_8050C6E8[0xcd], func_80136190(&lbl_eu_8050C6E8[0xa2], &lbl_eu_8050C6E8[0xae], 16), 0);
    func_80136B4C(pg->mpLayout, &lbl_eu_8050C6E8[0xda], func_80136190(&lbl_eu_8050C6E8[0xa2], &lbl_eu_8050C6E8[0xae], 13), 0);
    func_80136B4C(pg->mpLayout, &lbl_eu_8050C6E8[0xe5], func_80136190(&lbl_eu_8050C6E8[0xa2], &lbl_eu_8050C6E8[0xae], 14), 0);
    func_80136B4C(pg->mpLayout, &lbl_eu_8050C6E8[0x103], func_80136190(&lbl_eu_8050C6E8[0xf0], &lbl_eu_8050C6E8[0xfe], 0x2b), 0);

    // Sample-texture msg id: lookup table picked by controller type; result is
    // arg2 of the id lookup (retail keeps it in r4 across the branch merge).
    const char* texTable = isClassicController__Q22cf13CfGameManagerFv(-1) != 0
        ? &lbl_eu_8050C6E8[0x10f] : &lbl_eu_8050C6E8[0x118];

    u16 id = func_8013606C(&lbl_eu_8050C6E8[0xf0], texTable, 0x2b);
    char* texName = func_80138F78(id);

    nw4r::lyt::ArcResourceAccessor* resAcc = func_801355F4();
    TPLPalette* resource = (TPLPalette*)resAcc->GetResource(0x74696D67, texName, NULL);

    if (resource != NULL) {
        func_80137E7C(pg->mpLayout, &lbl_eu_8050C6E8[0x121], resource);

        // TPL dims captured before the pane lookup (retail keeps them in
        // callee-saved regs across FindPaneByName).
        TPLHeader* header = resource->descriptorArray->textureHeader;
        u16 w = header->width;
        u16 h = header->height;

        nw4r::lyt::Pane* pane = pg->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050C6E8[0x121], true);
        if (pane != NULL) {
            CLPSize size;
            size.width = (f32)(u32)w;
            size.height = (f32)(u32)h;
            pane->SetSize(*reinterpret_cast<nw4r::lyt::Size*>(&size));
        }
    }
}
#pragma pop

// Delete the layout at field_4 if present, then clear the pointer
extern "C" __declspec(noinline) void func_80253794(CCollepedia* this_) {
    if (this_->field_4 != nullptr) {
        delete this_->field_4;
        this_->field_4 = nullptr;
    }
}

// If layout is loaded, handle state machine and animate
extern "C" __declspec(noinline) void func_802537EC(CCollepedia* this_) {
    if (this_->field_4 == nullptr) {
        return;
    }
    u8 state = reinterpret_cast<u8*>(&this_->field_c_ptr)[0];
    switch ((int)state) {
    case 1:
        func_80253A14(this_);
        break;
    case 3:
        func_80253A60(this_);
        break;
    }
    this_->field_4->Animate(0);
}

// noinline: retail callers call this out-of-line.
extern "C" __declspec(noinline) void func_8025385C(CCollepedia* this_) {
    u8* stateBytes = reinterpret_cast<u8*>(&this_->field_c_ptr);
    if (stateBytes[0] != 0) return;
    stateBytes[0] = 1;
    stateBytes[1] = 0;
    stateBytes[2] = 1;
    playUISound__FUl(0xD);
}

// Return byte at +0xD from a sub-array entry (indexed by arg)
// noinline: retail callers invoke this out-of-line.
extern "C" __declspec(noinline) u32 func_8025415C(u8* obj, u32 index) {
    if (index >= 6) return 0;
    s8 idx = (s8)obj[1];
    return obj[idx * 0x140 + index * 0x34 + 0xD];
}

// Return byte at +0xC from a sub-array entry (indexed by arg)
extern "C" __declspec(noinline) u8 func_8025418C(u8* obj, u32 index) {
    if (index >= 6) return 0;
    s8 idx = (s8)obj[1];
    return obj[idx * 0x140 + index * 0x34 + 0xC];
}

// If field_c is 2, set it to 3, clear field_d, and schedule event 0xE
// noinline: retail callers call this out-of-line.
extern "C" __declspec(noinline) void func_80253888(CCollepedia* this_) {
    u8* stateBytes = reinterpret_cast<u8*>(&this_->field_c_ptr);
    if (stateBytes[0] != 2) return;
    stateBytes[0] = 3;
    stateBytes[1] = 0;
    playUISound__FUl(0xE);
}

// Set two named panes (0x82, 0x92) on the layout at field_4
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" void func_802538B0(CCollepedia* this_, unsigned char arg) {
    setLayoutTextBoxNumber(this_->field_4, &lbl_eu_8050C6E8[0x82], (unsigned char)1);
    setLayoutTextBoxNumber(this_->field_4, &lbl_eu_8050C6E8[0x92], arg);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" void func_80253904(CCollepedia* this_, char* arg2, char* arg3) {
    if (arg2 != NULL) {
        func_80137E7C(this_->field_4, &lbl_eu_8050C6E8[0x138], arg2);
    }
    if (arg3 != NULL) {
        func_80137E7C(this_->field_4, &lbl_eu_8050C6E8[0x144], arg3);
    }
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// us-80255bac
// Format a pane name, find two panes by name, and call func_80137924 to copy text
// Retail saves r28-r31 with stmw and uses a 0x40-byte stack frame
// Retail reloads rootPane from *(layout+0x10) each time (no caching)
extern "C" void func_80253970(CCollepedia* this_, LayoutContainer* container, int arg3) {
    char buf[0x28];
    sprintf(buf, &lbl_eu_8050C6E8[0x14f], arg3 + 1);
    nw4r::lyt::Pane* pane1 = (*(nw4r::lyt::Pane**)((u8*)container->mpLayout + 0x10))->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 = (*(nw4r::lyt::Pane**)((u8*)container->mpLayout + 0x10))->FindPaneByName(&lbl_eu_8050C6E8[0x15c], true);
    func_80137924((nw4r::math::VEC3*)this_, pane1, pane2, *(nw4r::lyt::Pane**)((u8*)container->mpLayout + 0x10));
}
#pragma pop

// Check if anim transform is finished; if so, set state to 2
// field_8 is a Layout* but used as AnimTransform* in this context
extern "C" __declspec(noinline) void func_80253A14(CCollepedia* self) {
    if (advanceAnimTransform(reinterpret_cast<nw4r::lyt::AnimTransform*>(self->field_8), lbl_eu_80668800) != 0) {
        u8* stateBytes = reinterpret_cast<u8*>(&self->field_c_ptr);
        stateBytes[0] = 2;
        stateBytes[1] = 1;
    }
}


// If the anim transform at field_8 is not finished, reset the state flags
// field_8 is a Layout* but used as AnimTransform* in this context
extern "C" __declspec(noinline) void func_80253A60(CCollepedia* this_) {
    if (func_80137510(reinterpret_cast<nw4r::lyt::AnimTransform*>(this_->field_8), lbl_eu_80668800) != 0) {
        u8* stateBytes = reinterpret_cast<u8*>(&this_->field_c_ptr);
        stateBytes[0] = 0;
        stateBytes[1] = 1;
        stateBytes[2] = 0;
    }
}

// Init the sub-array header (count=0, index=-1) and zero the entry storage
// noinline: retail callers invoke this out-of-line.
extern "C" __declspec(noinline) s8* func_80253AB0(s8* this_) {
    this_[0] = 0;
    this_[1] = -1;
    memset(this_ + 4, 0, 0x2800);
    return this_;
}

extern "C" void* __dt__80253AFC(void* self, int dealloc_flag) {
    void* result = self;
    if (result != nullptr && dealloc_flag > 0) {
        __dl__FPv(result);
    }
    return result;
}

// us-80255d78
// Build the collepedia page list:
//  1. collect the ids of every "collectible" item (kind 0xa), capped at 0x12c,
//  2. append one page (stride 0x140) per BDAT category (2..0x19) that has at
//     least one unlocked, uncompleted entry,
//  3. bubble-sort pages by their category sort key,
//  4. select the default category page and append a sentinel page (0x1a),
//  5. distribute every collected item into the pages' 6x5 grids, starting a
//     new row whenever the item sub-category changes.
extern "C" __declspec(noinline) void func_80253B3C(u8* self_) {
    extern void func_8025449C(u8* self, int a, int b, int c, int itemId);

    u16 n = 0;
    u16 numIds = (u16)func_8003B1EC((void*)lbl_eu_806640EC);
    u16 ids[0x12c];
    for (u16 id = 1; id <= numIds; id++) {
        if ((u8)func_801392E4(id) == 0xa) {
            ids[n] = id;
            n++;
            if (n >= 0x12c) break;
        }
    }

    // One page per category with at least one active, uncompleted entry.
    void* tblA = lbl_eu_806640A0;
    void* tblB = lbl_eu_806640A8;
    u32 numA = func_8003B1EC(tblA);
    char* const s = lbl_eu_8050C6E8;
    for (u8 cat = 2; cat <= 0x19; cat++) {
        switch (cat) {
        case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 0xa: case 0xb: case 0xc: case 0xd: case 0xe: case 0xf: case 0x10:
        case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16:
        case 0x17: case 0x18: case 0x19:
            break;
        }
        for (u16 j = 1; j <= (u16)numA; j++) {
            if ((u8)func_801361E8((u32)tblA, &lbl_eu_8050C6E8[0x16b], j) == cat &&
                func_801361E8((u32)tblA, &lbl_eu_8050C6E8[0x171], j) == 0 &&
                func_8009CF8C(j + 0x20c8) != 0) {
                u8 cnt = self_[0];
                self_[cnt * 0x140 + 4] = cat;
                self_[0] = cnt + 1;
                break;
            }
        }
    }

    // Bubble-sort pages by their BDAT sort key (adjacent-page swap).
    u8 pageCount = self_[0];
    for (u8 i = 0; i < pageCount; i++) {
        u8 swapped = 0;
        for (u8 j = 0; j < pageCount - 1 - i; j++) {
            u8* pg = self_ + j * 0x140;
            u8 keyCur = pg[4];
            u8 keyNext = pg[0x144];
            if ((u8)func_801361E8((u32)tblB, &s[0x17a], keyCur) >
                (u8)func_801361E8((u32)tblB, &s[0x17a], keyNext)) {
                pg[4] = keyNext;
                pg[0x144] = keyCur;
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }

    // Select the page matching the default category.
    u32 defCat = lbl_eu_80664184;
    for (u8 k = 0; k < self_[0]; k++) {
        if ((u32)self_[k * 0x140 + 4] == defCat) {
            self_[1] = k;
            break;
        }
    }

    // Append the sentinel page (category 0x1a); if no default page was found,
    // select the last real page instead.
    u8 cnt = self_[0];
    u8 newCnt = cnt + 1;
    self_[cnt * 0x140 + 4] = 0x1a;
    u8 sel = self_[1];
    self_[0] = newCnt;
    if ((s8)sel == -1) {
        self_[1] = newCnt - 1;
    }

    // Distribute every collected item across the pages' grids. A row break is
    // inserted when the item's sub-category (+0x180 column) differs from the
    // previous row's; same-sub-category items stack in the current row.
    for (u8 g = 0; g < self_[0]; g++) {
        u8* grp = self_ + g * 0x140;
        u8 cat = grp[4];
        u8 row = 0;
        u8 col = 0;
        for (u16 c = 0; c < n; c++) {
            u16 itemId = ids[c];
            u16 kind = func_80139358(itemId);
            if ((u8)func_801361E8((u32)lbl_eu_80664104, &lbl_eu_8050C6E8[0x16b], kind) != cat) {
                continue;
            }
            u8 sub = (u8)func_801361E8((u32)lbl_eu_80664104, &lbl_eu_8050C6E8[0x180], kind);
            if (row == 0) {
                grp[0 * 0x34 + 0xC] = sub;
                func_8025449C(self_, g, row, 0, itemId);
                row = 1;
                col = 1;
            } else if (sub != grp[row * 0x34 - 0x28]) {
                grp[row * 0x34 + 0xC] = sub;
                func_8025449C(self_, g, row, 0, itemId);
                row++;
                col = 1;
            } else {
                func_8025449C(self_, g, row - 1, col++, itemId);
            }
        }
        grp[5] = row;
    }

    func_802545C0(self_);
}

// noinline: retail callers tail-branch to this symbol.
// noinline: retail callers invoke this out-of-line.
extern "C" __declspec(noinline) bool func_80253EE8(u8* this_, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return false;
    if (arg2 >= 5) return false;
    s8 idx = (s8)this_[1];
    u8 val = *(this_ + idx * 0x140 + arg1 * 0x34 + arg2 * 0xA + 0x16);
    return val == 2;
}

// us-80256178
// Handle item selection in the collepedia grid.
// Entry address folds as ((self + (s8)self[1]*0x140) + row*0x34)[col*0xA + off];
// retail rematerializes self[1] per access, so the load stays inline.
// Grant path falls through first (if remaining<=0), consume call branched to.
// Residual (16 mm / 3 structural): MWCC associates the pointer sum as
// (self+row)+idx while retail folds self+idx first; scheduler-invariant
// across every source shape tried (flat/subscript/cast/chained-&/locals).
extern "C" __declspec(noinline) void func_80253F3C(u8* self, u32 row, u32 col) {
    if (row >= 6) return;
    if (col >= 5) return;

    func_8009EC18(*(u16*)&(self + (s8)self[1] * 0x140 + row * 0x34)[0x10 + col * 0xA], 1);

    (self + (s8)self[1] * 0x140 + row * 0x34)[0x16 - 4 + col * 0xA] = 3;

    u8 count = func_8025440C(self, row, col);
    s32 remaining = count - 1;

    u16 itemId = *(u16*)&(self + (s8)self[1] * 0x140 + row * 0x34)[col * 0xA + 0xE];
    u32 res = func_801587E8(itemId);

    // Grant path falls through first in retail; consume call is branched to
    if (remaining <= 0) {
        // Grant the item via its impl instance (virtual method at slot 0x10)
        void* itemInst = CItem_initItemImplInstances((void*)res);
        ((void (*)(void*, u32))(*(void***)itemInst)[4])(itemInst, res);
    } else {
        func_80158118((void*)res, itemId, remaining);
    }

    func_802545C0(self);
}

// Increment sub-array counter, wrap to 0 at max
extern "C" __declspec(noinline) void func_80254040(u8* obj) {
    obj[1]++;
    if ((s8)obj[1] >= (s32)obj[0]) {
        obj[1] = 0;
    }
}

extern "C" void func_8025406C(unsigned char* obj) {
    obj[1] = obj[1] - 1;
    if ((signed char)obj[1] < 0) {
        obj[1] = obj[0] - 1;
    }
}

void CCollepedia_decrementWrap(unsigned char* obj) {
    func_8025406C(obj);
}

extern "C" __declspec(noinline) char* func_80254094(u8* self) {
    s8 idx = (s8)self[1];
    u8* base = self + idx * 0x140;
    u8 val = base[4];
    if (val == 0x1a) {
        return (char*)func_80136190((const void*)&lbl_eu_8050C6E8[0xA2], (const void*)&lbl_eu_8050C6E8[0xAE], 0x14);
    } else {
        return (char*)func_8013639C((const void*)lbl_eu_806640A8, (const void*)&lbl_eu_8050C6E8[0xAE], val);
    }
}

unsigned char CCollepedia_getFieldAtIdx(u8* thisPtr) {
    s8 idx = (s8)thisPtr[1];
    u8* base = thisPtr + idx * 320;
    return base[4];
}

// Return a formatted string for the entry at `index` if its byte at +0xC is set
extern "C" __declspec(noinline) char* func_802540F4(u8* self, u32 index) {
    if (index >= 6) return 0;
    s8 idx = (s8)self[1];
    u8 val = self[idx * 0x140 + index * 0x34 + 0xC];
    if (val != 0) {
        return (char*)func_80136190((const void*)&lbl_eu_8050C6E8[0xA2], (const void*)&lbl_eu_8050C6E8[0xAE], val);
    }
    return 0;
}

unsigned char CCollepedia_getFieldOffset(const unsigned char* ptr) {
    int idx = (signed char)ptr[1];
    return ptr[5 + idx * 0x140];
}

// Return byte at +0x16 from a sub-sub-array entry (indexed by arg2, arg3)
// Computes arg2*0x34 first to match retail register order
extern "C" __declspec(noinline) u8 func_802541BC(u8* self, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)self[1];
    u32 rowOff = arg2 * 0x34;
    u8* ptr = self + idx * 0x140;
    ptr = ptr + rowOff;
    return ptr[arg3 * 0xA + 0x16];
}

u16 CCollepedia_getField12(u8* p1, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)p1[1];
    u8* ptr = p1 + idx * 0x140 + arg2 * 0x34 + arg3 * 0xa;
    return reinterpret_cast<CCollepediaEntry*>(ptr)[1].field_08;
}

extern "C" __declspec(noinline) u32 func_8025424C(u8* this_, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)this_[1];
    u8* ptr = this_ + idx * 0x140;
    ptr += arg2 * 0x34;
    ptr += arg3 * 0x0A;
    u16 val = *(u16*)(ptr + 0x0E);
    if (val == 0) return 0;
    return func_80136254((char*)lbl_eu_806640EC, &lbl_eu_8050C6E8[0x185], val);
}

// Load a float from offset 8 of the current sub-array entry
extern "C" __declspec(noinline) float func_802542B8(u8* this_) {
    s8 idx = (s8)this_[1];
    return *(float*)(this_ + idx * 0x140 + 8);
}

// us-8025650c
// Look up an entry in the sub-array, check +0x10 u16 and +0x16 byte.
// If byte != 1, resolve string via func_8013639C.
extern "C" __declspec(noinline) char* func_802542D0(u8* self, u32 arg1, u32 arg2) {
    if (arg1 >= 6) return nullptr;
    if (arg2 >= 5) return nullptr;
    // Fold the whole entry address into one expression to match retail dataflow
    // (base+idx*0x140 must accumulate in r0; statement/index forms regress).
    u8* entry = self + (s8)self[1] * 0x140 + arg1 * 0x34 + arg2 * 0xA;
    u16 val = *(u16*)(entry + 0x10);
    if (val == 0) return nullptr;
    u8 b = entry[0x16];
    if (b == 1) return nullptr;
    return (char*)func_8013639C(lbl_eu_80664104, &lbl_eu_8050C6E8[0xAE], val);
}

// us-8025658c
// Look up an entry in the sub-array, check if item is unlocked (via func_8009EC6C), format its name
// Returns nullptr if the entry is invalid or locked
extern "C" char* func_80254350(u8* self, u32 arg2, u32 arg3) {
    if (lbl_eu_806647D8 == NULL) return NULL;
    if (arg2 >= 6) return NULL;
    if (arg3 >= 5) return NULL;

    s8 idx = (s8)self[1];
    u8* entry = self + idx * 0x140 + arg2 * 0x34 + arg3 * 0xA;

    u16 val = *(u16*)(entry + 0x14);
    if (val == 0) return NULL;

    u16 check = *(u16*)(entry + 0x10);
    if (func_8009EC6C(check) != 0) {
        return (char*)func_8013639C(lbl_eu_806647D8, &lbl_eu_8050C6E8[0x18f], val);
    }
    return NULL;
}

// Read u16 at offset 0xE within the indexed sub-array entry
// If non-zero, resolve via func_80158068 and return as u8
// The sub-array is indexed by: (s8)self[1] * 0x140 + arg2 * 0x34 + arg3 * 0xA + 0xE
extern "C" __declspec(noinline) u8 func_8025440C(u8* self, u32 arg2, u32 arg3) {
    if (arg2 >= 6) return 0;
    if (arg3 >= 5) return 0;
    s8 idx = (s8)self[1];
    self += idx * 0x140;
    self += arg2 * 0x34;
    u16 val = *(u16*)(self + arg3 * 0xA + 0xE);
    if (val != 0) {
        return (u8)func_80158068(val);
    }
    return 0;
}

// Load a byte from offset 6 of the current sub-array entry
extern "C" __declspec(noinline) u32 func_80254484(u8* this_) {
    s8 idx = (s8)this_[1];
    u8* base = this_ + idx * 0x140;
    return base[6];
}

// us-802566d8
// Fill one grid entry (stride 0xA at self + a*0x140 + b*0x34 + c*0xA):
// item id, category lookups, and an unlock-state byte.
extern "C" void func_8025449C(u8* self, int a, int b, int c, u16 itemId) {
    // Retail rebuilds the entry address from three live stride offsets before
    // every store (see open-item note below).
    *(u16*)(self + a * 0x140 + b * 0x34 + c * 0xA + 0xE) = itemId;

    u16 kind = func_80139358(itemId);

    *(u16*)(self + a * 0x140 + b * 0x34 + c * 0xA + 0x10) = kind;

    *(u16*)(self + a * 0x140 + b * 0x34 + c * 0xA + 0x12) =
        func_80136254((const void*)lbl_eu_806640EC, &lbl_eu_8050C6E8[0x197], (int)itemId);
    *(u16*)(self + a * 0x140 + b * 0x34 + c * 0xA + 0x14) =
        func_80136254((const void*)lbl_eu_806640EC, &lbl_eu_8050C6E8[0x18f], (int)itemId);

    // Unlock state: 0=kindless, 3=owned, 2=new/claimable, 1=locked
    // OPEN ITEM (us-802566d8): retail rematerializes the entry base address
    // into volatile regs before every store (7 callee-saved, stmw r25);
    // this build GVN-caches the shared pointer sum in an extra callee-saved
    // reg (stmw r24). Not source-controllable: 9 expression shapes tried
    // (reassociation, strength variants, pressure changes, def-point moves,
    // param retypes) all yield identical codegen. See MWCC_PATTERNS.md:1329
    // and wall class 11 (uncontrollable CSE).
    if (kind == 0) {
        *(self + a * 0x140 + b * 0x34 + c * 0xA + 0x16) = 0;
    } else if (func_8009EC6C(kind) != 0) {
        *(self + a * 0x140 + b * 0x34 + c * 0xA + 0x16) = 3;
    } else if (func_801587E8(itemId) != 0) {
        *(self + a * 0x140 + b * 0x34 + c * 0xA + 0x16) = 2;
    } else {
        *(self + a * 0x140 + b * 0x34 + c * 0xA + 0x16) = 1;
    }
}

// us-80256ae4
// Destructor for CCollepedia.
// Calls sub-object destructors and frees memory if dealloc_flag > 0.
#pragma optimize_for_size on

// us-802567fc
// Recompute collection progress for every category page.
// A row counts as "complete" when the leading non-empty slots of its 5
// columns all have unlock-state 3 (collected); a page is complete when all
// its rows are complete. The per-page progress float at +8 is either 1.0
// (complete page) or collected-count / seen-count.
extern "C" __declspec(noinline) void func_802545C0(u8* self) {
    u8* group;
    u8* row;
    u8 i;
    u8 groupDone;
    u8 rowCount;
    u8 r;
    u8 rowDone;
    u8 c;

    for (i = 0; i < self[0]; i++) {
        group = self + i * 0x140;
        rowCount = group[5];
        groupDone = 1;
        for (r = 0; r < rowCount; r++) {
            row = group + r * 0x34;
            rowDone = 1;
            for (c = 0; c < 5; c++) {
                u8 state = row[c * 0xA + 0x16];
                if (state == 0) break;
                if (state == 3) continue;
                rowDone = 0;
                break;
            }
            row[0xD] = rowDone;
            if (rowDone == 0) groupDone = 0;
        }
        group[6] = groupDone;
    }

    // Ratio of collected (state 3) columns over columns reached before hitting
    // an empty slot, seeded with the constant below.
    float one = lbl_eu_80668800;
    {
        u8* row2;
        u8* grp;
        u8 ii;
        for (ii = 0; ii < self[0]; ii++) {
            grp = self + ii * 0x140;
            if (grp[6] != 0) {
                *(float*)(grp + 8) = one;
            } else {
            float denom = lbl_eu_806687F8;
            float num = denom;
            u8 cnt = grp[5];
            u8 rr;
            u8 cc;
            for (rr = 0; rr < cnt; rr++) {
                row2 = grp + rr * 0x34;
                for (cc = 0; cc < 5; cc++) {
                    u8 state = row2[cc * 0xA + 0x16];
                    if (state == 0) break;
                    if (state == 3) num += one;
                    denom += one;
                }
            }
            *(float*)(grp + 8) = num / denom;
            }
        }
    }
}

extern "C" void* __dt__11CCollepediaFv(CCollepedia* self, int flag) {
    if (self) {
        __dt__7CSysWinFv((u8*)self + 0x9C, -1);
        __dt__6CCur18Fv((u8*)self + 0x84, -1);
        __dt__6CCur07Fv((u8*)self + 0x54, -1);
        __dt__17UnkClass_8045F564Fv((UnkClass_8045F564*)((u8*)self + 0x14), -1);
        __dt__17UnkClass_8045F564Fv((UnkClass_8045F564*)((u8*)self + 0x04), -1);
        if (flag > 0) {
            __dl__FPv((u8*)self);
        }
    }
    return self;
}
#pragma optimize_for_size off

// us-8025695c - CCollepedia constructor.
// Stores vtable, constructs both memory regions, cursors and the embedded
// CSysWin at +0x9C, initializes the entry storage, then re-initializes the
// window from a stack temporary (copying everything after the vtable ptr).
#pragma push
#pragma optimize_for_size on
CCollepedia::CCollepedia() {
    field_0 = (int)lbl_eu_805373E0;
    __ct__17UnkClass_8045F564Fv((UnkClass_8045F564*)((u8*)this + 0x04));
    __ct__17UnkClass_8045F564Fv((UnkClass_8045F564*)((u8*)this + 0x14));

    field_24 = nullptr;
    field_28 = nullptr;
    field_2C = nullptr;
    field_30 = nullptr;
    field_34 = nullptr;
    field_38 = nullptr;
    field_3C = nullptr;
    field_40 = nullptr;
    field_44 = nullptr;
    field_48 = 0;
    field_49 = 0;
    field_4C = 0;
    field_50 = 0;
    field_51 = 1;

    __ct__CCur07((u8*)this + 0x54, nullptr);
    __ct__CCLPCur((CCLPCur*)((u8*)this + 0x6C), nullptr);
    __ct__CCur18((u8*)this + 0x84, nullptr);
    __ct__CSysWin((CSysWin*)((u8*)this + 0x9C), 0);

    field_D8 = 0;
    field_D9 = 0;
    field_DA = 0;
    func_80253AB0((s8*)&field_E8);
    func_8025348C(&field_28EC, 0);

    // Stack temporary window, re-initialized then copied over the embedded
    // window body (everything after the vtable pointer).
    u8 wt[0x3C];
    __ct__CSysWin((CSysWin*)wt, 0);
    *(CLPSysWinBody*)((u8*)this + 0xA0) = *(CLPSysWinBody*)&wt[4];
    __dt__7CSysWinFv(wt, -1);

    lbl_eu_806647D8 = nullptr;
    lbl_eu_806647DC = nullptr;
}
#pragma pop

// us-80256b68
#pragma push
#pragma optimize_for_size on
// Initialize CCollepedia: load 3 files, init CSysWin, init sub-array storage.
// The entry storage is built in a stack temp and copied member-wise to match
// retail codegen (byte stores for the header, bulk struct copy for the data).
void func_8025492C(CCollepedia* this_) {
    this_->field_24 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_8050C6E8[0x19c], this_, 0, 0);

    this_->field_28 = readFile__11CDeviceFileFUlPCcP10IWorkEventii(
        (u32)getHandleMEM2__Q23mtl10MemManagerFv(), &lbl_eu_8050C6E8[0x1b3], this_, 0, 0);

    this_->field_2C = readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        func_800A9D90(), &lbl_eu_8050C6E8[0x1cc], this_, 0, 0);

    // Virtual call on CSysWin at this+0x9C, vtable slot 34 (offset 0x88)
    ((CSysWinProxy*)(void*)&this_->field_9C)->v32();

    CLPInitTemp tmp;
    tmp._00 = 0;
    tmp._01 = -1;
    memset(tmp.data.v, 0, 0x2800);
    this_->field_E8 = tmp._00;
    this_->_E9[0] = tmp._01;
    this_->field_EC = tmp.data;

    func_80253B3C(&this_->field_E8);
}
#pragma pop

// us-80256c5c
// Per-frame state machine dispatch on field_49, then update all sub-objects.
// Routing mirrors retail jumptable_eu_80537378 (17 entries): slots 0/7/10/14
// are empty and route to the shared tail.
extern "C" void func_80254A20(CCollepedia* this_) {
    if (this_->field_48 == 0) return;
    if (this_->field_49 == 0) return;

    switch (this_->field_49) {
    case 1:
        func_80255748(this_);
        break;
    case 2:
        func_802557E0(this_);
        break;
    case 3:
        // Keep polling the intro anim transform until it finishes
        advanceAnimTransform(this_->field_44, lbl_eu_80668800);
        break;
    case 4:
        func_80255894(this_);
        break;
    case 5:
        func_8025592C(this_);
        break;
    case 6:
        func_80255984(this_);
        break;
    case 7:
        break;
    case 8:
        func_802559DC(this_);
        break;
    case 9:
        if (CSysWin_isActive(&this_->field_9C)) {
            this_->field_49 = 0xA;
        }
        break;
    case 10:
        break;
    case 11:
        func_80255AB4(this_);
        break;
    case 12:
        func_80255B60(this_);
        break;
    case 13:
        if (CSysWin_isActive(&this_->field_9C)) {
            this_->field_49 = 0xE;
        }
        break;
    case 14:
        break;
    case 15:
        func_80255C28(this_);
        break;
    case 16:
        if (!func_801B481C()) {
            this_->field_49 = 3;
        }
        break;
    }

    this_->field_38->Animate(0);
    func_801D202C(&this_->field_54);
    func_802532FC(reinterpret_cast<CBaseCur*>(&this_->field_54[0x18])); // +0x6c
    func_8022B748(&this_->field_9C);
    // Retail calls the second page's state machine with the +0x28EC sub-object
    func_802537EC(reinterpret_cast<CCollepedia*>(&this_->field_28EC[0]));
    func_801D202C(&this_->field_54[0x30]); // +0x84
}

#pragma push
#pragma optimize_for_size on
// us-80256da0
// Render the CCollepedia display: draw layout, sub-cursors, and syswin if active
// Retail reads field_48/field_49 and field_38 from r3 (original this) before any reg copy
extern "C" void func_80254B64(CCollepedia* this_, nw4r::lyt::DrawInfo* drawInfo) {
    if (this_->field_48 == 0) return;
    if (this_->field_49 == 0) return;
    
    drawLayout(this_->field_38, drawInfo, 0, 1);
    func_80253360((CBaseCur*)((u8*)this_ + 0x6c), drawInfo);
    
    if (this_->field_28F0 != NULL) {
        drawLayout(this_->field_28F0, drawInfo, 0, 1);
    }
    
    func_801D20B0((CBaseCur*)((u8*)this_ + 0x54), drawInfo);
    func_8022B7C8(&this_->field_9C, drawInfo);
    func_801D20B0((CBaseCur*)((u8*)this_ + 0x84), drawInfo);
}
#pragma pop

// us-80256e40
// Cleanup CCollepedia: free files, layouts, sub-objects
void func_80254C04(CCollepedia* this_) {
    getEntry__5CBdatFUl(2);
    closeFileHandle__FPP11CFileHandle(&this_->field_24);
    closeFileHandle__FPP11CFileHandle(&this_->field_28);
    closeFileHandle__FPP11CFileHandle(&this_->field_2C);

    this_->field_48 = 0;
    func_80253794(reinterpret_cast<CCollepedia*>(&this_->field_28EC[0]));

    // Free layout at +0x38 via its virtual deleting dtor
    // (single if: `delete` itself emits the second null-check seen in retail)
    if (this_->field_38 != nullptr) {
        delete reinterpret_cast<CLPDelProxy*>(this_->field_38);
        this_->field_38 = nullptr;
    }

    // Release arc resource accessors
    releaseArcResourceAccessor(this_->field_30);
    releaseArcResourceAccessor(this_->field_34);

    // Free heap object at +0x4C via its virtual deleting dtor
    if (this_->field_4C != 0) {
        delete reinterpret_cast<CLPDelProxy*>(this_->field_4C);
        this_->field_4C = 0;
    }

    // Tear down memory regions backing the layouts
    deleteRegion__17UnkClass_8045F564Fv(&this_->field_4);
    deleteRegion__17UnkClass_8045F564Fv(&this_->_14[0]);

    // Sub-objects at +0x54 and +0x84: parameterless finalizer at vtable +0xC
    reinterpret_cast<CLPSubProxy*>(&this_->field_54[0])->finalize();

    func_8025338C(reinterpret_cast<CBaseCur*>(&this_->field_54[0x18]));
    func_8022B7F4(&this_->field_9C);

    reinterpret_cast<CLPSubProxy*>(&this_->field_54[0x30])->finalize();
}

extern "C" u8 func_80254D0C(CCollepedia* self) {
    if (CSysWin_isReady(&self->field_9C)) {
        return self->field_50;
    }
    return 0;
}

struct CCollepediaState {
    u8 _00[0x49];
    u8 initialized;
    u8 _4A[7];
    u8 field51;
    u8 _52[0x28F9 - 0x52];
    u8 condition;
};

unsigned char CCollepedia_condGetField51(u8* self) {
    CCollepediaState* state = (CCollepediaState*)self;
    return state->condition ? state->field51 : 0;
}

void CCollepedia_initFields(u8* p) {
    CCollepediaState* state = (CCollepediaState*)p;
    if (state->initialized != 0) return;
    state->initialized = 1;
    state->field51 = 0;
}

// us-80256fc8
// Close the collepedia detail view: set state, reset sub-object, disable animations, enable outro, play sound
void func_80254D8C(CCollepedia* this_) {
    if (this_->field_28F9 == 0) return;
    if (!CSysWin_isActive(&this_->field_9C)) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;
    if (this_->field_49 != 3) return;

    this_->field_49 = 4;
    this_->field_51 = 0;
    func_801D216C(&this_->field_54, 0);

    this_->field_38->SetAnimationEnable(this_->field_44, false);
    this_->field_38->SetAnimationEnable(this_->field_3C, false);
    this_->field_38->SetAnimationEnable(this_->field_40, true);

    playUISound__FUl(6);
}

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// us-802570a0
// Decrement timer counters (field_DA or field_D9) with wrapping, update display, schedule event
void func_80254E64(CCollepedia* this_) {
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    if (this_->field_28FA != 0) {
        if (this_->field_28F9 == 0) return;

        u8 val = this_->field_DA - 1;
        this_->field_DA = val;
        if ((s8)val < 0) {
            this_->field_DA = 1;
        }
        func_80256314(this_);
    } else {
        if (this_->field_49 != 3) return;

        u8 val = this_->field_D9 - 1;
        this_->field_D9 = val;
        if ((s8)val < 0) {
            this_->field_D9 = (u8)(func_80254144(&this_->field_E8) - 1);
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    playUISound__FUl(1);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// us-80257168
// Increment timer-based navigation: increment field_DA with wrap, or advance field_D9 and update display
void func_80254F2C(CCollepedia* this_) {
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    if (this_->field_28FA != 0) {
        if (this_->field_28F9 == 0) return;

        this_->field_DA = this_->field_DA + 1;
        if ((s8)this_->field_DA >= 2) {
            this_->field_DA = 0;
        }
        func_80256314(this_);
    } else {
        if (this_->field_49 != 3) return;

        this_->field_D9 = this_->field_D9 + 1;
        if ((s8)this_->field_D9 >= (s32)(func_80254144(&this_->field_E8) & 0xFF)) {
            this_->field_D9 = 0;
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    playUISound__FUl(1);
}
#pragma pop

// Timer-based countdown that calls func_80255210 when field_D8 reaches 0
void func_80255000(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    u8 d8 = this_->field_D8;
    if ((s8)d8 == 0) {
        func_80255210(this_);
        this_->field_D8 = 4;
        func_80256314(this_);
        func_8025629C(this_);
    } else {
        u8 val = d8 - 1;
        this_->field_D8 = val;
        if ((s8)val < 0) {
            this_->field_D8 = 4;
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    playUISound__FUl(1);
}

// us-802572f0
// Increment field_D8 with wrap at 5; when field_D8 == 4 call func_8025516C, then update display
void func_802550B4(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    if ((s8)this_->field_D8 == 4) {
        func_8025516C(this_);
        this_->field_D8 = 0;
        func_80256314(this_);
        func_8025629C(this_);
    } else {
        u8 val = this_->field_D8 + 1;
        this_->field_D8 = val;
        if ((s8)val >= 5) {
            this_->field_D8 = 0;
        }
        func_80256314(this_);
        func_8025629C(this_);
    }

    playUISound__FUl(1);
}

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// us-802573a8
// Decrement field_D9 with wrapping, update display, and schedule next event
extern "C" void func_8025516C(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    func_80254040(&this_->field_E8);
    u32 count = func_80254144(&this_->field_E8) & 0xFF;

    // Force field_D9 sign-extend before comparing with masked count
    if ((s8)this_->field_D9 >= (s32)count) {
        this_->field_D9 = (u8)(func_80254144(&this_->field_E8) - 1);
        func_80256314(this_);
    }

    func_80255F98(this_);
    func_8025629C(this_);
    playUISound__FUl(10);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// us-8025744c
// Decrement field_D9 with wrapping (calls func_8025406C instead of func_80254040)
extern "C" void func_80255210(CCollepedia* this_) {
    if (this_->field_49 != 3) return;
    if (this_->field_28FA != 0) return;
    if (CSysWin_getUnk34(&this_->field_9C)) return;

    func_8025406C(&this_->field_E8);
    int count = (int)func_80254144(&this_->field_E8) & 0xFF;
    if ((s8)this_->field_D9 >= count) {
        this_->field_D9 = (u8)(func_80254144(&this_->field_E8) - 1);
        func_80256314(this_);
    }

    func_80255F98(this_);
    func_8025629C(this_);
    playUISound__FUl(10);
}
#pragma pop

// us-802574f0
// Handle CCollepedia navigation: check state and dispatch actions.
// Early-return states are tested as an unsigned compare chain (cmplwi).
void func_802552B4(CCollepedia* this_) {
    u32 state = this_->field_49;
    if (state == 9) return;
    if (state == 0xB) return;
    if (state == 0xC) return;
    if (state == 0xD) return;
    if (state == 0xF) return;
    if (state == 0x10) return;

    if (CSysWin_getUnk34(&this_->field_9C) != 0) {
        if (CSysWin_isActive(&this_->field_9C) == 0) return;
        s32 cur = this_->field_49;
        switch (cur) {
        case 0xA:
            this_->field_49 = 0xB;
            break;
        case 0xE:
            this_->field_49 = 0xF;
            break;
        }
        func_8022B8E4(&this_->field_9C);
        return;
    }

    // CSysWin is not active, check sub-array conditions
    if (this_->field_28FA == 0) return;
    if (this_->field_28F9 == 0) return;

    func_80253888(reinterpret_cast<CCollepedia*>(&this_->field_28EC[0]));
    func_801D216C(&this_->field_54[0x30], 0);
    this_->field_49 = 8;
    this_->field_51 = 0;
    playUISound__FUl(6);
}

// us-802575e8
// Per-frame advance for the collepedia selection flow.
// Early-return states are tested as a signed compare chain (cmpwi/beq).
#pragma push
#pragma optimize_for_size on
void func_802553AC(CCollepedia* this_) {
    u32 state = this_->field_49;
    if (state == 9) return;
    if (state == 0xB) return;
    if (state == 0xC) return;
    if (state == 0xD) return;
    if (state == 0xF) return;
    if (state == 0x10) return;

    if (CSysWin_getUnk34(&this_->field_9C)) {
        if (CSysWin_isActive(&this_->field_9C) == 0) return;
        // Two-case switch: MWCC branches forward to the grouped case bodies
        switch (this_->field_49) {
        case 0xA:
            this_->field_49 = 0xB;
            break;
        case 0xE:
            this_->field_49 = 0xF;
            break;
        }
        func_8022B8E4(&this_->field_9C);
        return;
    }

    if (this_->field_28FA != 0) {
        if (this_->field_28F9 == 0) return;

        if ((s8)this_->field_DA == 0) {
            // Grid page: grant the selected item, refresh both display pages,
            // then play a category-keyed fanfare.
            func_80253F3C(&this_->field_E8, this_->field_D9, this_->field_D8);
            func_80255F98(this_);
            func_8025629C(this_);

            u32 cat = func_8025418C(&this_->field_E8, this_->field_D9);
            // Switch range-check covers 0..8; case 8 is empty (fanfare only)
            switch (cat) {
            case 0: playUISound__FUl(0x4B); break;
            case 1: playUISound__FUl(0x4C); break;
            case 2: playUISound__FUl(0x4D); break;
            case 3: playUISound__FUl(0x4E); break;
            case 4: playUISound__FUl(0x4F); break;
            case 5: playUISound__FUl(0x50); break;
            case 6: playUISound__FUl(0x51); break;
            case 7:
            case 8: playUISound__FUl(0x52); break;
            }
            incrementEventCounter__FUl(0x84);
        } else {
            // Detail page confirm: schedule sound 6 instead.
            playUISound__FUl(6);
        }

        func_80253888(reinterpret_cast<CCollepedia*>(&this_->field_28EC[0]));
        func_801D216C(&this_->field_54[0x30], 0);
        this_->field_49 = 8;
        this_->field_51 = 0;
        return;
    }

    // List page: open the detail view for the selected entry.
    if (func_80253EE8(&this_->field_E8, this_->field_D9, this_->field_D8) == 0) return;

    char* desc = func_802542D0(&this_->field_E8, this_->field_D9, this_->field_D8);
    func_80136B4C(this_->field_28F0, &lbl_eu_8050C6E8[0x12b], desc, 0);

    func_802538B0(reinterpret_cast<CCollepedia*>(&this_->field_28EC[0]),
        func_8025440C(&this_->field_E8, this_->field_D9, this_->field_D8));

    char* name1 = func_80138F78((u16)func_8025424C(&this_->field_E8, this_->field_D9, this_->field_D8));
    void* tex1 = this_->field_30->GetResource(0x74696D67, name1, NULL);

    char* name2 = func_80138F78((u16)func_80254204(&this_->field_E8, this_->field_D9, this_->field_D8));
    void* tex2 = this_->field_34->GetResource(0x74696D67, name2, NULL);

    func_80253904(reinterpret_cast<CCollepedia*>(&this_->field_28EC[0]), (char*)tex1, (char*)tex2);
    func_8025385C(reinterpret_cast<CCollepedia*>(&this_->field_28EC[0]));

    this_->field_DA = 0;
    func_801D216C(&this_->field_54, 0);
    this_->field_49 = 6;
    this_->field_51 = 0;
    playUISound__FUl(3);
}
#pragma pop

void func_80255688(CCollepediaFull* self) {
    func_80253EE8((u8*)self + 0xE8, self->field_D9, self->field_D8);
}

extern "C" u8 func_80255698(CCollepedia* self) {
    if (CSysWin_getUnk34(&self->field_9C)) {
        return 1;
    }
    return self->field_28FA;
}

// Returns 1 if any block condition is active, otherwise calls func_801B481C
extern "C" u32 func_802556DC(CCollepedia* this_) {
    if (this_->field_49 != 3) {
        return 1;
    }
    if (CSysWin_getUnk34(&this_->field_9C)) {
        return 1;
    }
    if (this_->field_28FA != 0) {
        return 1;
    }
    return func_801B481C();
}

// us-80257a1c
// Check if animation at field_40 is done; if so, set state, init sub-object, switch animations
__declspec(noinline)
void func_802557E0(CCollepedia* this_) {
    if (advanceAnimTransform(this_->field_40, lbl_eu_80668800) != 0) {
        this_->field_49 = 3;
        this_->field_51 = 1;
        func_801D216C(&this_->field_54, 1);

        this_->field_38->SetAnimationEnable(this_->field_3C, false);
        this_->field_38->SetAnimationEnable(this_->field_40, false);
        this_->field_38->SetAnimationEnable(this_->field_44, true);

        func_80256314(this_);
    }
}

// us-80257ad0
// Check if animation at field_40 is still playing; if so, disable field_40 and field_44, enable field_3C, set state to 5
__declspec(noinline) void func_80255894(CCollepedia* this_) {
    if (func_80137510(this_->field_40, lbl_eu_80668800) != 0) {
        this_->field_38->SetAnimationEnable(this_->field_40, false);
        this_->field_38->SetAnimationEnable(this_->field_44, false);
        this_->field_38->SetAnimationEnable(this_->field_3C, true);
        this_->field_49 = 5;
    }
}

// Check if animation at field_3C is done, then set state and init sub-object
extern "C" __declspec(noinline) void func_8025592C(CCollepedia* this_) {
    if (func_80137510(this_->field_3C, lbl_eu_80668800) != 0) {
        this_->field_51 = 1;
        this_->field_49 = 0;
        func_801D216C(this_->field_54, 0);
    }
}

extern "C" __declspec(noinline) void func_80255984(CCollepedia* this_) {
    if (this_->field_28F9 == 0) return;
    this_->field_49 = 7;
    this_->field_51 = 1;
    func_801D216C(&this_->field_54[0x30], 1);
    func_80256314(this_);
}

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// us-80257c18
// Open the collepedia detail view: set state, enable sub-object, update display, load file
__declspec(noinline) void func_802559DC(CCollepedia* this_) {
    if (this_->field_28F9 == 0) return;

    this_->field_49 = 3;
    this_->field_51 = 1;
    func_801D216C(&this_->field_54, 1);

    func_80256314(this_);

    if (func_8025415C(&this_->field_E8, this_->field_D9) == 0) return;

    this_->field_49 = 9;

    char* str = (char*)func_80136190(&lbl_eu_8050C6E8[0xA2], &lbl_eu_8050C6E8[0xAE], 0x12);
    func_8022B9B4(&this_->field_9C, str, 0);
    func_8022BFC8((CSysWin*)&this_->field_9C, 1);
    func_8022B8B8(&this_->field_9C);

    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    func_801895EC();
    func_80043738(0, &lbl_eu_8050C6E8[0x1E9], handle, 2, 1, 0);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// us-80257cf0
// If CSysWin is active, set state, look up table entry, format string, call func_8013E2E0
extern "C" __declspec(noinline) void func_80255AB4(CCollepedia* this_) {
    if (!CSysWin_isActive(&this_->field_9C)) return;

    this_->field_49 = 12;

    // NOTE (residual): retail interleaves the two @ha/@l address pairs around
    // the table lookup and loads the fmt pointer after entry+1; our build
    // schedules the sdata load early regardless of source shape (10+ variants
    // tried). Residual: lhzx dest reg + swapped lwz/addi pair.
    u32 idx = func_802540DC(&this_->field_E8);
    s32 d9 = (s8)this_->field_D9;
    u32 count = lbl_eu_8050C6A0[idx & 0xFF] + 1;
    u32 result = func_80136254(lbl_eu_806647DC, &lbl_eu_8050C6E8[0x1f9],
        (u32)(u16)count + d9);

    u32 zero = 0;
    func_8013E2E0((u16)result, 0, 0, 0, 0, 0, 0, 0, zero);
}
#pragma pop

// us-80257d9c
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
// Initialize collepedia display: check condition, set up CSysWin with text, play sounds, load file
__declspec(noinline) void func_80255B60(CCollepedia* this_) {
    if (func_801B481C()) return;

    this_->field_49 = 3;

    if (func_80254484(&this_->field_E8) == 0) return;

    this_->field_49 = 0xD;

    char* str = (char*)func_80136190(&lbl_eu_8050C6E8[0xA2], &lbl_eu_8050C6E8[0xAE], 0x13);
    func_8022B9B4(&this_->field_9C, str, 0);
    func_8022BFC8((CSysWin*)&this_->field_9C, 1);
    func_8022B8B8(&this_->field_9C);

    incrementEventCounter__FUl(0x85);
    incrementEventCounter__FUl(0x86);

    void* handle = getHandleMEM2__Q23mtl10MemManagerFv();
    func_801895EC();
    func_80043738(0, &lbl_eu_8050C6E8[0x200], handle, 2, 1, 0);
}
#pragma pop

// us-80257e64
// If CSysWin is active, do a table lookup and format a sound/effect call
__declspec(noinline) void func_80255C28(CCollepedia* this_) {
    if (!CSysWin_isActive(&this_->field_9C)) return;

    this_->field_49 = 0x10;

    // Table lookup folded into the call so the index comes straight from func_802540DC
    u32 result = func_80136254(lbl_eu_806647DC, &lbl_eu_8050C6E8[0x1f9],
        lbl_eu_8050C6A0[func_802540DC(&this_->field_E8) & 0xFF]);
    func_8013E2E0((u16)result, 0, 0, 0, 0, 0, 0, 0, 0);
}

// us-80257efc
// Check layout/resource pointers and the two SDA string-pool pointers, then update display
void func_80255CC0(CCollepedia* this_) {
    if (this_->field_38 == nullptr || this_->field_34 == nullptr ||
        lbl_eu_806647D8 == nullptr || lbl_eu_806647DC == nullptr) {
        return;
    }
    func_8025641C();
    func_80255F98(this_);
    func_8025629C(this_);
    this_->field_50 = 1;
    this_->field_48 = 1;
}

// us-80257f78
// Load the texture resource for the detail pane selected by `mode` and show it.
// optimize_for_size gives the retail stmw/lmw block-save frame.
#pragma push
#pragma optimize_for_size on
// noinline: retail callers (func_80255F98) invoke this out-of-line.
__declspec(noinline) void func_80255D3C(CCollepedia* this_, const char* name, int mode) {
    void* res = NULL;
    // Mode 3 intentionally shares mode 2's resource and falls through without break.
    switch (mode) {
    case 0:
        res = this_->field_30->GetResource(0x74696D67, &lbl_eu_8050C6E8[0x210], NULL);
        break;
    case 1:
        res = this_->field_30->GetResource(0x74696D67, &lbl_eu_8050C6E8[0x228], NULL);
        break;
    case 2:
        res = this_->field_30->GetResource(0x74696D67, &lbl_eu_8050C6E8[0x240], NULL);
        break;
    case 3:
        res = this_->field_30->GetResource(0x74696D67, &lbl_eu_8050C6E8[0x240], NULL);
    }

    if (res != NULL) {
        char buf[0x28];
        sprintf(buf, &lbl_eu_8050C6E8[0x256], name + 1);
        func_80137E7C(this_->field_38, buf, res);
    }
}
#pragma pop

// us-802580cc
// Like func_80255D3C but mode 3 resolves the resource name from an item id.
// optimize_for_size gives the retail stmw/lmw block-save frame (three call sites total).
#pragma push
#pragma optimize_for_size on
// noinline: retail caller func_80255F98 invokes this out-of-line.
__declspec(noinline) void func_80255E90(CCollepedia* this_, const char* name, u32 mode, int id) {
    void* tex;
    if (mode == 3) {
        if (id != 0)
            // Detail view: resolve the texture name from the item id via the second accessor.
            tex = this_->field_34->GetResource(0x74696D67, func_80138F78(id), NULL);
        else
            tex = this_->field_30->GetResource(0x74696D67, &lbl_eu_8050C6E8[0x264], NULL);
    } else {
        tex = this_->field_30->GetResource(0x74696D67, &lbl_eu_8050C6E8[0x264], NULL);
    }

    if (tex != NULL) {
        char buf[0x28];
        sprintf(buf, &lbl_eu_8050C6E8[0x277], name + 1);
        func_80137E7C(this_->field_38, buf, tex);
    }
}
#pragma pop

#pragma push
#pragma optimize_for_size on
// us-80258658
// Iterate sub-array entries, set pane visibility for each, copy position for current entry, update display
void func_8025641C(CCollepedia* this_) {
    char buf[0x20];
    register u8 count = this_->field_E8;
    for (u8 i = 1; i <= 0x16; i++) {
        sprintf(buf, &lbl_eu_8050C6E8[0x2F7], i);

        func_80124270(this_->field_38->GetRootPane()->FindPaneByName(buf, true), i <= count);

        if (i == count) {
            copyVEC3(this_->field_DC,
                (float*)((u8*)this_->field_38->GetRootPane()->FindPaneByName(buf, true) + 0x2C));
        }
    }

    setLayoutTextBoxNumber(this_->field_38, &lbl_eu_8050C6E8[0x306], count);
    setLayoutTextBoxNumber(this_->field_38, &lbl_eu_8050C6E8[0x284], 1);
}
#pragma pop

// Stubs for functions called by targets
extern "C" __declspec(noinline) u8 func_802540DC(u8* self) {
    s8 idx = (s8)self[1];
    return *(u8*)(self + idx * 0x140 + 4);
}
extern "C" __declspec(noinline) u32 func_80254144(u8*){ return 0; }
// Manual signed-int -> double conversion (docs/MWCC_PATTERNS.md 7i idiom,
// see CArtsInfo ConvS32ToF64): build the 0x4330000080000000 bit pattern and
// subtract the shared sdata2 magic so the lfd references lbl_eu_80668818
// instead of a TU-local pool label.
static double clpConvS32ToF64(s32 x) {
    union {
        double d;
        u32 w[2];
    } u;
    // xoris word first, then 0x43300000, or MWCC hoists the lis out of order.
    u.w[1] = (u32)x ^ 0x80000000;
    u.w[0] = 0x43300000;
    return u.d - lbl_eu_80668818;
}

// us-802581d4
// Refresh the detail page: show/hide panes, move the cursor, description
// text, collected-count label, per-entry rows and texture slots.
// Retail frame is 0x70 with _savegpr_25: only r25-r31 stay live across calls.
extern "C" __declspec(noinline) void func_80255F98(CCollepedia* this_) {
    char buf[0x18];
    u8 idx = (u8)(this_->_E9[0] + 1); // retail reads the index byte unsigned
    setLayoutTextBoxNumber(this_->field_38, &lbl_eu_8050C6E8[0x284], idx);

    // Move the cursor pane right by one slot width from the saved position.
    // The int->double step references the shared 2^52 pool entry explicitly.
    nw4r::lyt::Pane* cursorPane =
        this_->field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050C6E8[0x28d], true);
    nw4r::math::VEC3 pos = *(nw4r::math::VEC3*)&this_->field_DC[0];
    pos.x = lbl_eu_80668810 * clpConvS32ToF64((s32)idx - 1) + pos.x;
    copyVEC3((float*)((u8*)cursorPane + 0x2C), &pos);

    func_80136B4C(this_->field_38, &lbl_eu_8050C6E8[0x29b],
        func_80254094(&this_->field_E8), 0);

    // "collected / total" label: scale the completion ratio to slot count.
    // Call first, constant load second - matches retail schedule.
    float ratio = func_802542B8(&this_->field_E8);
    u8 slot = (u8)(lbl_eu_80668814 * ratio); // fctiwz truncation
    char* label = func_80136190(&lbl_eu_8050C6E8[0xA2], &lbl_eu_8050C6E8[0xAE], 0xB);
    sprintf(buf, &lbl_eu_8050C6E8[0x2a3], slot, label);
    func_80136A1C(this_->field_38, &lbl_eu_8050C6E8[0x2a8], buf, 0);

    u32 count = func_80254144(&this_->field_E8);
    // Exactly six rows exist per page; visibility still follows the count.
    for (u8 i = 0; i < 6; i++) {
        // Retail computes i+1 once per iteration into a callee-saved reg.
        u8 num = (u8)(i + 1);
        sprintf(buf, &lbl_eu_8050C6E8[0x2b0], num);
        func_80136B4C(this_->field_38, buf, func_802540F4(&this_->field_E8, i), 0);

        sprintf(buf, &lbl_eu_8050C6E8[0x2bb], num);
        nw4r::lyt::Pane* namePane =
            this_->field_38->GetRootPane()->FindPaneByName(buf, true);
        func_80124270(namePane, (int)i < (int)(u8)count);

        sprintf(buf, &lbl_eu_8050C6E8[0x2c8], num);
        nw4r::lyt::Pane* numPane =
            this_->field_38->GetRootPane()->FindPaneByName(buf, true);
        func_80124270(numPane, func_8025415C(&this_->field_E8, i));
    }

    // Fill the 6x5 grid: texture slots and item icons per visible entry.
    for (u8 row = 0; row < (u32)(u8)count; row++) {
        u32 base = row * 5;
        for (u8 col = 0; col < 5; col++) {
            u32 mode = col + base;
            func_80255D3C(this_, (const char*)func_802541BC(&this_->field_E8, row, col),
                (u8)mode);
            u16 id = func_80254204(&this_->field_E8, row, col);
            func_80255E90(this_, (const char*)func_802541BC(&this_->field_E8, row, col),
                (u8)mode, id);
        }
    }

    // Final pane visibility follows the page's completion flag.
    u32 done = func_80254484(&this_->field_E8);
    nw4r::lyt::Pane* markPane = this_->field_38->GetRootPane()->FindPaneByName(
        &lbl_eu_8050C6E8[0x2d6], true);
    func_80124270(markPane, done);
}
// Set two pane names on the layout using the current sub-array entry
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
extern "C" __declspec(noinline) void func_8025629C(CCollepedia* this_) {
    char* str1 = func_802542D0(&this_->field_E8, this_->field_D9, this_->field_D8);
    func_80136B4C(this_->field_38, &lbl_eu_8050C6E8[0x2e2], str1, 0);
    char* str2 = func_80254350(&this_->field_E8, this_->field_D9, this_->field_D8);
    func_80136B4C(this_->field_38, &lbl_eu_8050C6E8[0x2eb], str2, this_->field_4C);
}
#pragma pop
// us-80258550
// Refresh the collepedia display: detail view routes through the shared
// formatter, list view formats the current entry name and moves the cursor.
#pragma push
#pragma optimize_for_size on
// noinline: retail keeps this out-of-line; without it MWCC inlines the whole
// body into its earlier callers (late-definition inlining defeats auto_inline off).
extern "C" __declspec(noinline) void func_80256314(CCollepedia* this_) {
    // Scratch block: detail-path pane-name buffer at +0x00, cursor
    // position VEC3 at +0x0C, list-path page-name buffer at +0x18.
    u8 work[0x38];
    if (this_->field_28FA != 0) {
        func_80253970(reinterpret_cast<CCollepedia*>(work),
            reinterpret_cast<LayoutContainer*>(&this_->field_28EC), this_->field_DA);

        reinterpret_cast<CLPCurRefreshProxy*>(&this_->field_54[0x30])->refresh(work);
    } else {
        char* name = reinterpret_cast<char*>(work + 0x18);
        sprintf(reinterpret_cast<char*>(work + 0x18), &lbl_eu_8050C6E8[0x256],
            (s8)this_->field_D9 * 5 + (s8)this_->field_D8 + 1);

        nw4r::lyt::Pane* pane1 = this_->field_38->GetRootPane()->FindPaneByName(name, true);
        nw4r::lyt::Pane* pane2 = this_->field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050C6E8[0x15c], true);

        func_80137924(reinterpret_cast<nw4r::math::VEC3*>(work + 0x0C), pane1, pane2,
            this_->field_38->GetRootPane());

        reinterpret_cast<CLPCurSetPosProxy*>(&this_->field_54)->setPos(
            reinterpret_cast<nw4r::math::VEC3*>(work + 0x0C));
    }
}
#pragma pop

// us-80258748
// File-load completion callback for the three collepedia archives:
//  - field_24: main menu arc -> builds the full layout, fonts, animations,
//    cursors (+0x54/+0x6C/+0x84) and the second-page info record (+0x28EC).
//  - field_28: detail-page arc -> resource accessor only.
//  - field_2C: common BDAT archive -> releases the tables into the two
//    global message-file pointers.
bool CCollepedia::OnFileEvent(CEventFile* pEventFile) {
    if (field_24 == pEventFile->mFileHandle) {
        UnkClass_8045F564* memRegion =
            reinterpret_cast<UnkClass_8045F564*>(reinterpret_cast<u8*>(this) + 0x04);
        int mem2 = (int)getHandleMEM2__Q23mtl10MemManagerFv();
        memRegion->createRegion(mem2, 0x8000,
                                &lbl_eu_8050C6E8[0x30f], 0);

        Class_8045F858 regionGuard(memRegion);

        // Detach the arc payload from the handle and attach it to the layout
        // resource accessor.
        void* fileData = reinterpret_cast<CFileHandle*>(field_24)->getData();
        mtl::MemManager::setMemInitFlag(false);

        void* tagMem = allocate__Q23mtl10MemManagerFUlUl(
            getAllocHandle__10CLibLayoutFv(), 0x858);
        if (tagMem != NULL) {
            tagMem = __ct__CTagProcessor(tagMem);
        }
        field_4C = (u32)tagMem;

        field_30 = createArcResourceAccessor__10CLibLayoutFv();
        field_30->Attach(fileData, &lbl_eu_8050C6E8[0x31b]);

        buildLayout(&field_38, field_30, &lbl_eu_8050C6E8[0x31f]);
        bindLayoutAnimTransform(field_38, &field_3C, field_30, &lbl_eu_8050C6E8[0x335]);
        bindLayoutAnimTransform(field_38, &field_40, field_30, &lbl_eu_8050C6E8[0x34e]);
        bindLayoutAnimTransform(field_38, &field_44, field_30, &lbl_eu_8050C6E8[0x36c]);

        // Bind the device font into every text pane via the root pane.
        void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, field_38);
        u32 fontHandle = ((u32 (*)(void*))(((void**)fontObj)[0x24 / 4]))(fontObj);
        func_8013676C(field_38->GetRootPane(), fontHandle);

        char* tagStr = (char*)getPackedFont();
        setLayoutTextBoxFont(field_38, &lbl_eu_8050C6E8[0x284], (u32)tagStr);
        setLayoutTextBoxFont(field_38, &lbl_eu_8050C6E8[0x306], (u32)tagStr);
        setLayoutTextBoxFont(field_38, &lbl_eu_8050C6E8[0x2a8], (u32)tagStr);

        field_38->SetAnimationEnable(field_40, false);
        field_38->SetAnimationEnable(field_44, false);
        field_38->SetAnimationEnable(field_3C, true);
        field_38->Animate(0);

        char* titleText =
            func_80136190(&lbl_eu_8050C6E8[0xa2], &lbl_eu_8050C6E8[0xae], 0xa);
        func_80136B4C(field_38, &lbl_eu_8050C6E8[0x38a], titleText, 0);

        // Install our tag processor on the detail text-box pane.
        nw4r::lyt::Pane* textBox =
            field_38->GetRootPane()->FindPaneByName(&lbl_eu_8050C6E8[0x2eb], true);
        if (textBox != NULL) {
            static_cast<nw4r::lyt::TextBox*>(textBox)->SetTagProcessor(
                reinterpret_cast<nw4r::ut::WideTagProcessor*>(tagMem));
        }

        // Stack-build the +0x6C cursor, copy its body (everything after the
        // vtable pointer), refresh its position, then mark it active.
        u8 tmpCur[0x18];
        __ct__CCLPCur(reinterpret_cast<CCLPCur*>(tmpCur), field_30);
        CLPCurBody* curDst = reinterpret_cast<CLPCurBody*>(&field_54[0x18]);
        CLPCurBody* curSrc = reinterpret_cast<CLPCurBody*>(&tmpCur[4]);
        curDst->field_04 = curSrc->field_04;
        curDst->field_08 = curSrc->field_08;
        curDst->field_0C = curSrc->field_0C;
        curDst->field_10 = curSrc->field_10;
        curDst->mActive = curSrc->mActive;
        curDst->mVisible = curSrc->mVisible;
        reinterpret_cast<CLPCurVt*>(curDst)->cv2();
        curDst->mActive = 1;

        // Second-page info record built from a stack temporary.
        CLPPageInfo pageInfo;
        func_8025348C(&pageInfo, (int)field_30);
        CLPPageInfo* pageDst = reinterpret_cast<CLPPageInfo*>(&field_28EC[0]);
        pageDst->field_00 = pageInfo.field_00;
        pageDst->mpLayout = pageInfo.mpLayout;
        pageDst->field_08 = pageInfo.field_08;
        pageDst->field_0C = pageInfo.field_0C;
        pageDst->field_0D = pageInfo.field_0D;
        pageDst->field_0E = pageInfo.field_0E;

        func_802534F0(reinterpret_cast<CLPPageSetup*>(&field_28EC[0]));

        // +0x84 cursor (CCur18): stack-build, copy, refresh.
        u8 tmpCur18[0x18];
        __ct__CCur18(tmpCur18, func_801355F4());
        func_8018B0FC(reinterpret_cast<u8*>(this) + 0x84, tmpCur18);
        __dt__6CCur18Fv(tmpCur18, -1);
        reinterpret_cast<CLPCurVt*>(reinterpret_cast<u8*>(this) + 0x84)->cv2();

        // +0x54 cursor (CCur07): same pattern.
        u8 tmpCur07[0x18];
        __ct__CCur07(tmpCur07, field_30);
        func_8018B0FC(reinterpret_cast<u8*>(this) + 0x54, tmpCur07);
        __dt__6CCur07Fv(tmpCur07, -1);
        reinterpret_cast<CLPCurVt*>(reinterpret_cast<u8*>(this) + 0x54)->cv2();

        func_80255CC0(this);
        field_24 = nullptr;
        memRegion->func_8045F810();
        return true;
    }

    if (field_28 == pEventFile->mFileHandle) {
        UnkClass_8045F564* memRegion =
            reinterpret_cast<UnkClass_8045F564*>(reinterpret_cast<u8*>(this) + 0x14);
        int mem2 = (int)getHandleMEM2__Q23mtl10MemManagerFv();
        memRegion->createRegion(mem2, 0x100,
                                &lbl_eu_8050C6E8[0x395], 0);

        Class_8045F858 regionGuard(memRegion);

        void* fileData = reinterpret_cast<CFileHandle*>(field_28)->getData();
        mtl::MemManager::setMemInitFlag(false);

        field_34 = createArcResourceAccessor__10CLibLayoutFv();
        field_34->Attach(fileData, &lbl_eu_8050C6E8[0x31b]);

        func_80255CC0(this);
        field_28 = nullptr;
        memRegion->func_8045F810();
        return true;
    }

    if (field_2C == pEventFile->mFileHandle) {
        void* data = reinterpret_cast<CFileHandle*>(field_2C)->getData();
        setBdatEntry__5CBdatFUlPv(2, data);

        // Resolve the two message files used by the item/quest name lookups.
        func_8003AA34();
        lbl_eu_806647D8 = getFP__FPCc(&lbl_eu_8050C6E8[0x3a4]);
        func_8003AA34();
        lbl_eu_806647DC = (char*)getFP__FPCc(&lbl_eu_8050C6E8[0x3b3]);

        func_80255CC0(this);
        field_2C = nullptr;
        return true;
    }

    return false;
}
