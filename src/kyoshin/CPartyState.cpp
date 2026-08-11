// Auto-scaffolded catalog TU for kyoshin/CPartyState
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPartyState.hpp"
#include "kyoshin/CEquipItemBox.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/IWorkEvent.hpp"

// Sets the vtable, builds the scratch memory region, zeroes the panel state
// fields, constructs the embedded cursor, and initializes the byte flags.
CPartyState::CPartyState()
    : mVtbl((void*)lbl_eu_805353C8),
      mFileHandle(0), mArcResAcc(0), mLayout(0),
      mAnimTrans1(0), mAnimTrans0(0),
      field_0x28(0), field_0x2C(0), field_0x30(0), field_0x31(1) {
    __ct__CCur22(&mCur22, 0);
    field_0x4C = 0;
    field_0x4D = -1;
    field_0x4E = 0;
    field_0x57 = 0;
}

u8 CPartyState::func_801FD17C() { return field_0x30; }


u8 CPartyState::func_801FD184() { return field_0x31; }

u8 CPartyState::func_801FD18C() { return field_0x57; }


// Selection page-down / left input: clamp the current member index against
// the party size (2/3-per-row page clamp), then refresh and cue the cursor.
extern "C" void func_801FD304(CPartyState* self) {
    u8 memberCount = code80135FDC_getByte_64077();
    u8 cur = self->field_0x4C;
    s8 s = (s8)cur;
    u8 next = (u8)(memberCount - 1);
    if ((u32)(s - 3) > 3u) {
        switch (s) {
        case 0:
            if (s < next) {
                self->field_0x4C = 1;
            }
            break;
        case 1:
            if (next >= 3u) {
                self->field_0x4C = 3;
            } else if (next >= 2u) {
                self->field_0x4C = 2;
            } else {
                self->field_0x4C = 0;
            }
            break;
        case 2:
            self->field_0x4C = 0;
            break;
        }
    } else {
        self->field_0x4C = 2;
    }
    func_801FD8F8(self);
    func_80138078(1);
}

// Selection page-up / right input: clamp the current member index against
// the party size, then refresh and cue the cursor.
extern "C" void func_801FD3D4(CPartyState* self) {
    u8 memberCount = code80135FDC_getByte_64077();
    u8 cur = self->field_0x4C;
    s8 s = (s8)cur;
    u8 next = (u8)(memberCount - 1);
    if ((u32)(s - 3) > 3u) {
        switch (s) {
        case 0:
            self->field_0x4C = (next >= 2u) ? 2 : next;
            break;
        case 1:
            self->field_0x4C = 0;
            break;
        case 2:
            self->field_0x4C = (next >= 3u) ? 3 : 1;
            break;
        }
    } else {
        self->field_0x4C = 1;
    }
    func_801FD8F8(self);
    func_80138078(1);
}

// Party-select cancel (page-back): reset the highlight slot, refresh, and cue
// the cancel sound; busy-slot checks match func_801FD604.
extern "C" void func_801FD48C(CPartyState* self) {
    s8 highlight = self->field_0x4D;
    if (highlight < 0) {
        self->field_0x4E = 1;
        self->field_0x4D = -1;
        func_801FD8F8(self);
        func_80138078(3);
        return;
    }
    s8 cur = (s8)self->field_0x4C;
    if (highlight == cur) {
        self->field_0x4D = -1;
        func_801FD8F8(self);
        func_80138078(6);
        return;
    }
    if (cur == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4((u8)highlight));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    } else if (highlight == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4(self->field_0x4C));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    }
    func_801FE0C8(self);
    func_801FD8F8(self);
    func_80138078(0x11);
}

u32 CPartyState::func_801FD580() {
    s8 val = field_0x4D;
    // Match rlwinm extraction of sign bit followed by xori
    return ((u32)(val >> 31) & 1) ^ 1;
}

void func_801FD594(){}

void func_801FD5C4(){}

u8 CPartyState::func_801FD5F4() { return field_0x4E; }

u8 CPartyState::func_801FD5FC() { return field_0x4C; }

// Party-select confirm: swap the highlighted member (0x4D) into the current
// slot (0x4C). Refuses when the target slot's character is busy (+0x176C==1);
// a stale highlight (< 0) is re-seeded from the current slot instead.
extern "C" void func_801FD604(CPartyState* self) {
    s8 highlight = self->field_0x4D;
    if (highlight < 0) {
        self->field_0x4D = (s8)self->field_0x4C;
        func_801FD8F8(self);
        func_80138078(2);
        return;
    }
    s8 cur = (s8)self->field_0x4C;
    if (highlight == cur) {
        self->field_0x4D = -1;
        func_801FD8F8(self);
        func_80138078(6);
        return;
    }
    if (cur == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4((u8)highlight));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    } else if (highlight == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4(self->field_0x4C));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    }
    func_801FE0C8(self);
    func_801FD8F8(self);
    func_80138078(0x11);
}

// Refresh the party-select cursor: format each slot's pane name, look up the
// pane, accumulate its translate, and move the cursor sub-panes onto it.
extern "C" void func_801FD8F8(CPartyState* self) {
    char buf[0x28];
    sprintf(buf, lbl_eu_80507D40, self->field_0x4F[(s8)self->field_0x4C]);
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
    nw4r::math::VEC3 tmp;
    func_801375A0(&tmp, pane);
    if ((s8)self->field_0x4C < 3) {
        func_801D2C80(&self->mCur22, &tmp, 2);
        func_801D2BFC(&self->mCur22, 2);
    } else {
        func_801D2C80(&self->mCur22, &tmp, 3);
        func_801D2BFC(&self->mCur22, 3);
    }
    s8 highlight = self->field_0x4D;
    if (highlight >= 0) {
        sprintf(buf, lbl_eu_80507D40, self->field_0x4F[highlight]);
        nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::math::VEC3 tmp2;
        func_801375A0(&tmp2, pane2);
        tmp = tmp2;
        if (highlight < 3) {
            func_801D2C80(&self->mCur22, &tmp, 0);
            func_801D2CF4(&self->mCur22, 0, 1);
        } else {
            func_801D2C80(&self->mCur22, &tmp, 1);
            func_801D2CF4(&self->mCur22, 1, 1);
        }
    }
}

// Party-state panel advance (opening): when the +0x20 anim transform reaches
// the target frame, bind the panel anims and move to state 2.
// DECOMP_DONT_INLINE: retail calls these helpers through real `bl`s from
// func_801FCFF4; -inline auto would otherwise inline them into the switch.
extern "C" DECOMP_DONT_INLINE void func_801FD6F0(CPartyState* self) {
    float target = lbl_eu_80668218;
    if (func_80137444(self->mAnimTrans1, target) != 0) {
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
        self->mLayout->SetAnimationEnable(self->mAnimTrans0, true);
        self->field_0x2C = 2;
    }
}

// Party-state advance: when the +0x1C gate is set, refresh the shared cursor
// sound state, rebuild the state panel, and reset the visibility flags.
extern "C" void func_801FD8A0(CPartyState* self) {
    if (self->mLayout != 0) {
        func_80139198(0);
        func_801FDA7C(self);
        self->field_0x30 = 1;
        self->field_0x28 = 1;
        self->field_0x4C = 0;
    }
}

// Destroys the embedded cursor and scratch-memory sub-objects (reverse
// declaration order); MWCC emits the delete guard from the incoming flag.
CPartyState::~CPartyState() {
    __dt__6CCur22Fv(&mCur22, -1);
}

// Intro-animation finish handler: when the +0x24 anim transform reaches the
// target frame, move to state 3, show the cursor, and run the shared tail.
extern "C" DECOMP_DONT_INLINE void func_801FD76C(CPartyState* self) {
    if (func_80137444(self->mAnimTrans0, lbl_eu_80668218) != 0) {
        self->field_0x2C = 3;
        self->field_0x31 = 1;
        func_801D216C(&self->mCur22, 1);
        func_801FD8F8(self);
    }
}

// Party-state panel advance (closing): when the +0x24 anim transform reaches
// the target frame, bind the panel anims and move to state 5.
extern "C" DECOMP_DONT_INLINE void func_801FD7CC(CPartyState* self) {
    if (func_80137510(self->mAnimTrans0, lbl_eu_80668218) != 0) {
        self->mLayout->SetAnimationEnable(self->mAnimTrans0, false);
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, true);
        self->field_0x2C = 5;
    }
}

// Advance the party-state panel out of state 3: set state 4, hide the cursor,
// and play the cancel sound unless the +0x4E flag is set.
extern "C" void func_801FD1BC(CPartyState* self) {
    if (self->field_0x2C == 3) {
        self->field_0x2C = 4;
        self->field_0x31 = 0;
        func_801D2BFC(&self->mCur22, 0xff);
        if (self->field_0x4E == 0) {
            func_80138078(6);
        }
    }
}

// Selection change: wrap the current selection by one step based on the party
// member count, then run the shared refresh and play the cursor sound.
extern "C" void func_801FD220(CPartyState* self) {
    u8 memberCount = code80135FDC_getByte_64077();
    u8 cur = self->field_0x4C;
    s8 s = (s8)cur;
    u8 next = (u8)(memberCount - 1);
    if ((u32)(s - 4) <= 2u) {
        self->field_0x4C = (u8)(cur - 1);
    } else if (s == 3) {
        self->field_0x4C = next;
    }
    func_801FD8F8(self);
    func_80138078(1);
}

// Selection confirm: swap the party member at the highlight slot (0x4D) with
// the selected member (0x4C), reset the highlight, refresh, and mark ready.
extern "C" void func_801FE0C8(CPartyState* self) {
    int* party = func_8009ECB0();
    u8 slotA = func_801392B4((u8)self->field_0x4D);
    func_8009E168(party, slotA, func_801392B4(self->field_0x4C));
    func_80139198(0);
    func_80080888__Q22cf13CfGameManagerFv(func_801392B4(0), 0);
    func_801FDA7C(self);
    self->field_0x4D = -1;
    self->field_0x57 = 1;
}

void func_801FE20C(){}

// Refresh the party-state header text: format the frame-timer counters and
// bind them to the panel's text panes. The timer >> 12 is the minute counter
// (clamped to 99/999); the low field (raw >> 6 & 0x3f, max 59) is the seconds.
extern "C" void func_801FE154(CPartyState* self) {
    u32 raw = func_8006A80C();
    u32 t = raw >> 12;
    char buf[8];
    if (t > 0x63) {
        sprintf(buf, lbl_eu_80507D40 + 0xb, (t > 0x3e7) ? 0x3e7 : t);
    } else {
        sprintf(buf, lbl_eu_80507D40 + 0xb, (t > 0x63) ? 0x63 : t);
    }
    func_80136A1C(self->mLayout, lbl_eu_80507D40 + 0x100, buf, 0);
    sprintf(buf, lbl_eu_80507D40 + 0xb, (t > 0x3e7) ? 0x3b : ((raw >> 6) & 0x3f));
    func_80136A1C(self->mLayout, lbl_eu_80507D40 + 0x10d, buf, 0);
}

// Position a party panel pane by name: compute a ratio (0 when either input
// is zero), index a 5-float position table by it, and write the pane's X
// translate (Y/Z kept from the pane's size field). arg4 is unused.
extern "C" void func_801FE39C(CPartyState* self, float f1, float f2, u32 arg4, u32 name) {
    float ratio = (f1 != 0.0f && f2 != 0.0f) ? (f1 / f2) : 0.0f;
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName((const char*)name, true);
    if (pane != NULL) {
        int idx = (int)(lbl_eu_80668240 * lbl_eu_80668244 * ratio);
        float table[5];
        for (u32 i = 0; i < 5; i++) {
            table[i] = lbl_eu_80507D20[i];
        }
        float value = lbl_eu_80668248 * ratio + table[idx & 0x3f];
        float tmp[2];
        func_80127BC4(tmp, reinterpret_cast<float*>(reinterpret_cast<u8*>(pane) + 0x4c));
        tmp[0] = value;
        func_80124288(pane, tmp);
    }
}

void CPartyState::OnFileEvent() {}

// Start the async read of the party-state layout arc. The alloc region is
// picked by file size: MEM1 when small, the common-archive MEM2 region for
// mid sizes, plain MEM2 otherwise.
extern "C" void func_801FCF5C(CPartyState* self) {
    int fileSize = getFileSize__11CDeviceFileFPCc(lbl_eu_80662728, 1);
    int mem1Max = (int)mtl::MemManager::getMaxAllocSize(mtl::MemManager::getHandleMEM1());
    int mem2Max = (int)mtl::MemManager::getMaxAllocSize(func_80495FF0(lbl_eu_80663E14));
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    if (fileSize < mem1Max - 0x10000) {
        handle = mtl::MemManager::getHandleMEM1();
    } else if (fileSize < mem2Max - 0x80000) {
        handle = func_80495FF0(lbl_eu_80663E14);
    }
    self->mFileHandle = CDeviceFile::readFile(handle, lbl_eu_80662728,
        reinterpret_cast<IWorkEvent*>(self), 0, 0);
    CDeviceFile::setHandleFlag1(self->mFileHandle);
}

// Party-state panel advance: dispatch the per-state animation step, then
// re-animate the layout and update the embedded cursor.
extern "C" void func_801FCFF4(CPartyState* self) {
    if (self->field_0x28 != 0) {
        func_801FE154(self);
        switch (self->field_0x2C) {
        case 1:
            func_801FD6F0(self);
            break;
        case 2:
            func_801FD76C(self);
            break;
        case 4:
            func_801FD7CC(self);
            break;
        case 5:
            func_801FD848(self);
            break;
        }
        self->mLayout->Animate(0);
        func_801D202C(&self->mCur22);
    }
}

extern "C" void func_801FD0A0() {}
extern "C" void func_801FD194() {}

// Selection advance: if the current member index is within the party range,
// step forward by one (clamping to 3 once it passes the last member), then
// run the shared refresh and play the confirm sound.
extern "C" void func_801FD290(CPartyState* self) {
    u8 memberCount = code80135FDC_getByte_64077();
    u8 cur = self->field_0x4C;
    s8 s = (s8)cur;
    u8 next = (u8)(memberCount - 1);
    if ((u32)(s - 3) <= 3u) {
        if (s < next) {
            self->field_0x4C = (u8)(cur + 1);
        } else {
            self->field_0x4C = 3;
        }
    }
    func_801FD8F8(self);
    func_80138078(1);
}

// Teardown: release the file handle, destroy the bound layout, release the
// arc accessor, stop the embedded cursor, and tear down the scratch memory
// region. The double null-check below is retail: `delete` re-checks the
// pointer (see the matched CBgTex::func_801C3D9C for the same shape).
extern "C" void func_801FD0F4(CPartyState* self) {
    func_801390E0(&self->mFileHandle);
    nw4r::lyt::Layout* layout = self->mLayout;
    self->field_0x28 = 0;
    if (layout != 0) {
        delete layout;
        self->mLayout = 0;
    }
    func_80139124(self->mArcResAcc);
    reinterpret_cast<CPartyStateCur*>(&self->mCur22)->func_801D2180();
    self->mMemRegion.func_8045F778();
}
