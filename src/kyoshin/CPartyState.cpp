// Auto-scaffolded catalog TU for kyoshin/CPartyState
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPartyState.hpp"
#include "kyoshin/CEquipItemBox.hpp"

#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/work/CEventFile.hpp"
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
    func_801FD8F8((CPartyState*)self);
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
    func_801FD8F8((CPartyState*)self);
    func_80138078(1);
}

// Party-select cancel (page-back): reset the highlight slot, refresh, and cue
// the cancel sound; busy-slot checks match func_801FD604.
extern "C" void func_801FD48C(CPartyState* self) {
    u8 highlight = self->field_0x4D;
    if ((s8)highlight < 0) {
        self->field_0x4E = 1;
        self->field_0x4D = -1;
        func_801FD8F8(self);
        func_80138078(3);
        return;
    }
    u8 cur = self->field_0x4C;
    if ((s8)highlight == (s8)cur) {
        goto same;
    }
    if ((s8)cur == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4(highlight));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    } else if ((s8)highlight == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4(cur));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    }
    func_801FE0C8(self);
    func_801FD8F8(self);
    func_80138078(0x11);
    return;
same:
    self->field_0x4D = -1;
    func_801FD8F8(self);
    func_80138078(6);
}

u32 CPartyState::func_801FD580() {
    s8 val = field_0x4D;
    // Match rlwinm extraction of sign bit followed by xori
    return ((u32)(val >> 31) & 1) ^ 1;
}

extern "C" void func_801FD594(void* self) {
    *(s8*)((u8*)self + 0x4D) = -1;
    func_801FD8F8((CPartyState*)self);
    func_80138078(6);
}

extern "C" int func_801FD5C4(void* self) {
    s8 a = (s8)((u8*)self)[0x4D];
    if (a < 0) return 0;
    s8 b = (s8)((u8*)self)[0x4C];
    return (a - b) != 0 ? 1 : 2;
}

u8 CPartyState::func_801FD5F4() { return field_0x4E; }

u8 CPartyState::func_801FD5FC() { return field_0x4C; }

// Party-select confirm: swap the highlighted member (0x4D) into the current
// slot (0x4C). Refuses when the target slot's character is busy (+0x176C==1);
// a stale highlight (< 0) is re-seeded from the current slot instead.
extern "C" void func_801FD604(CPartyState* self) {
    u8 highlight = self->field_0x4D;
    if ((s8)highlight < 0) {
        self->field_0x4D = self->field_0x4C;
        func_801FD8F8(self);
        func_80138078(2);
        return;
    }
    u8 cur = self->field_0x4C;
    if ((s8)highlight == (s8)cur) {
        goto same;
    }
    if ((s8)cur == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4(highlight));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    } else if ((s8)highlight == 0) {
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(func_801392B4(cur));
        if (data->field_0x176C == 1) {
            func_80138078(5);
            return;
        }
    }
    func_801FE0C8(self);
    func_801FD8F8(self);
    func_80138078(0x11);
    return;
same:
    self->field_0x4D = -1;
    func_801FD8F8(self);
    func_80138078(6);
}

// Refresh the party-select cursor: format each slot's pane name, look up the
// pane, accumulate its translate, and move the cursor sub-panes onto it.
extern "C" __declspec(noinline) void func_801FD8F8(CPartyState* self) {
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
    if ((s8)self->field_0x4D >= 0) {
        sprintf(buf, lbl_eu_80507D40, self->field_0x4F[(s8)self->field_0x4D]);
        nw4r::lyt::Pane* pane2 = self->mLayout->GetRootPane()->FindPaneByName(buf, true);
        nw4r::math::VEC3 tmp2;
        func_801375A0(&tmp2, pane2);
        tmp = tmp2;
        if ((s8)self->field_0x4D < 3) {
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
extern "C" __declspec(noinline) void func_801FD6F0(CPartyState* self) {
    float target = lbl_eu_80668218;
    if (func_80137444(self->mAnimTrans1, target) != 0) {
        self->mLayout->SetAnimationEnable(self->mAnimTrans1, false);
        self->mLayout->SetAnimationEnable(self->mAnimTrans0, true);
        self->field_0x2C = 2;
    }
}

// Party-state advance: when the +0x1C gate is set, refresh the shared cursor
// sound state, rebuild the state panel, and reset the visibility flags.
// DECOMP_DONT_INLINE: retail calls these helpers through real `bl`s; -inline
// auto would otherwise inline them into OnFileEvent and blow the split.
extern "C" __declspec(noinline) void func_801FD8A0(CPartyState* self) {
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
#pragma push
#pragma optimize_for_size on
CPartyState::~CPartyState() {
    __dt__6CCur22Fv(&mCur22, -1);
}
#pragma pop

// Intro-animation finish handler: when the +0x24 anim transform reaches the
// target frame, move to state 3, show the cursor, and run the shared tail.
extern "C" __declspec(noinline) void func_801FD76C(CPartyState* self) {
    if (func_80137444(self->mAnimTrans0, lbl_eu_80668218) != 0) {
        self->field_0x2C = 3;
        self->field_0x31 = 1;
        func_801D216C(&self->mCur22, 1);
        func_801FD8F8((CPartyState*)self);
    }
}

// Party-state panel advance (closing): when the +0x24 anim transform reaches
// the target frame, bind the panel anims and move to state 5.
extern "C" __declspec(noinline) void func_801FD7CC(CPartyState* self) {
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
    if ((u32)(s - 4) > 2u) {
        if (s == 3) {
            self->field_0x4C = next;
        }
    } else {
        self->field_0x4C = (u8)(cur - 1);
    }
    func_801FD8F8((CPartyState*)self);
    func_80138078(1);
}

// Selection confirm: swap the party member at the highlight slot (0x4D) with
// the selected member (0x4C), reset the highlight, refresh, and mark ready.
extern "C" __declspec(noinline) void func_801FE0C8(CPartyState* self) {
    int* party = func_8009ECB0();
    u8 slotA = func_801392B4((u8)self->field_0x4D);
    func_8009E168(party, slotA, func_801392B4(self->field_0x4C));
    func_80139198(0);
    func_80080888__Q22cf13CfGameManagerFv(func_801392B4(0), 0);
    func_801FDA7C(self);
    self->field_0x4D = -1;
    self->field_0x57 = 1;
}

// Scale a party-slot pane's position by the ratio of the slot's play-time
// value to the current party member's progress. The ratio is clamped to
// [0, 1] and applied to the pane X translate.
extern "C" __declspec(noinline) void func_801FE20C(CPartyState* self, u32 memberIdx, const char* paneName) {
    CPartyCharData* data = (CPartyCharData*)func_8009EC9C(memberIdx);
    u8 slot = (u8)data->slotArea.field_0x3DD0;
    s32 cur = ((u32*)data->slotArea.records)[0x222 + slot];
    u32 found = 0;
    for (u8 i = 1; i <= 5; i++) {
        if (data->slotArea.records[slot].entries[i].word == 0) {
            found = func_8013600C(&lbl_eu_80507D40[0x11a], &lbl_eu_80507D40[0x127],
                                  (u8)(i + slot * 5 + (memberIdx - 1) * 0x19)) * 100;
            break;
        }
    }
    float fCur = (float)cur;
    float fFound = (float)found;
    float ratio;
    if (fCur != 0.0f && fFound != 0.0f) {
        ratio = fCur / fFound;
    } else {
        ratio = 0.0f;
    }
    if (ratio > lbl_eu_80668218) {
        ratio = lbl_eu_80668218;
    }
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(paneName, true);
    if (pane != NULL) {
        float tmp[2];
        func_80127BC4(tmp, const_cast<float*>(&pane->GetSize().width));
        tmp[0] = lbl_eu_80668234 * ratio;
        func_80124288(pane, tmp);
    }
}

// Rebuild the party-select screen: mix the .sdata2 seed bytes into the slot
// table (0x4F/0x56), then format each member slot's panes (name, level, HP,
// face texture) and the party-list tail slots.
extern "C" __declspec(noinline) void func_801FDA7C(CPartyState* self) {
    PartyStateSeed seed;
    seed.f.h = lbl_eu_8066821C;
    seed.f.b = lbl_eu_8066821E;
    seed.f.w = lbl_eu_80668220;
    memset(self->field_0x4F, 0, 7);
    self->field_0x56 = 0;
    u8 j = 0;
    u8 dst[7] = {0};

    u8 count = code80135FDC_getByte_64077();
    for (u8 i = 0; i < 3; i++) {
        if (i < count) {
            self->field_0x4F[self->field_0x56] = seed.bytes[4 + i];
            self->field_0x56++;
        } else {
            dst[j++] = seed.bytes[4 + i];
        }
    }
    s8 lim = (s8)(4 - (count - 3));
    if (lim < 0) lim = 0;
    for (u8 i = 0; i < 4; i++) {
        if (i < lim) {
            dst[j++] = seed.bytes[i];
        } else {
            self->field_0x4F[self->field_0x56] = seed.bytes[i];
            self->field_0x56++;
        }
    }
    for (u8 k = 0; k < j; k++) {
        self->field_0x4F[self->field_0x56] = dst[k];
        self->field_0x56++;
    }

    char buf18[0x20];
    char buf38[0x20];
    char buf58[0x20];
    char buf78[0x20];

    for (u8 i = 0; i < 7; i++) {
        sprintf(buf78, lbl_eu_80507D40, self->field_0x4F[i]);
        if (i >= count) {
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf78, true);
            func_80124270(pane, 0);
            continue;
        }
        u8 member = func_801392B4(i);
        CPartyCharData* data = (CPartyCharData*)func_8009EC9C(member);
        CPartySlotStats* s = &data->stats;
        s32 v0 = s->mVtbl->fn[0x42](s);
        s32 v1 = (int)((CPartySlotFnF)s->mVtbl->fn[0x4A])(s);
        s32 v2 = (int)((CPartySlotFnF)s->mVtbl->fn[0x4B])(s);
        s32 v3 = s->mVtbl->fn[0x7A](s);
        s32 v4 = s->mVtbl->fn[0x7C](s);
        char* v5 = (char*)s->mVtbl->fn[0x26](s);

        sprintf(buf58, &lbl_eu_80507D40[0xb], v0);
        if (v0 >= 0x63) {
            v3 = 0;
            v4 = 0;
        }
        if (v1 > 0x270f) v1 = 0x270f;
        if (v2 > 0x270f) v2 = 0x270f;
        sprintf(buf78, &lbl_eu_80507D40[0x10], self->field_0x4F[i]);
        func_80136A1C(self->mLayout, buf78, buf58, 0);
        sprintf(buf78, &lbl_eu_80507D40[0x23], self->field_0x4F[i]);
        func_80136910(self->mLayout, buf78, v1);
        sprintf(buf78, &lbl_eu_80507D40[0x36], self->field_0x4F[i]);
        func_80136910(self->mLayout, buf78, v2);
        sprintf(buf78, &lbl_eu_80507D40[0x49], self->field_0x4F[i]);
        func_80136B4C(self->mLayout, buf78, v5, 0);
        sprintf(buf78, &lbl_eu_80507D40[0x5b], self->field_0x4F[i]);
        func_801FE20C(self, member, buf78);
        sprintf(buf78, &lbl_eu_80507D40[0x6c], self->field_0x4F[i]);
        func_801FE39C(self, (float)v3, (float)v4, member, (u32)buf78);

        if (i >= 3) {
            u16 id = func_80136254(lbl_eu_80664090, &lbl_eu_80507D40[0x7d], member);
            char* name = func_80138F78(id);
            u32 tex = (u32)self->mArcResAcc->GetResource(0x74696D67, name, 0);
            if (tex != 0) {
                sprintf(buf78, &lbl_eu_80507D40[0x88], self->field_0x4F[i]);
                func_80137E7C(self->mLayout, buf78, tex);
            }
        } else {
            CPartyCharData* data2 = (CPartyCharData*)func_8009EC9C(member);
            u32 next = i + 1;
            sprintf(buf78, &lbl_eu_80507D40[0x95], next);
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf78, true);
            if (data2->field_0x176C == 1 || func_8009CF8C(0x3356) == 0) {
                func_80124270(pane, 0);
            } else {
                func_80124270(pane, 1);
                sprintf(buf78, &lbl_eu_80507D40[0xa4], member, data2->slotArea.field_0x3DD0 + 1);
                u32 tex = (u32)self->mArcResAcc->GetResource(0x74696D67, buf78, 0);
                if (tex != 0) {
                    sprintf(buf78, &lbl_eu_80507D40[0xc1], next);
                    func_80137E7C(self->mLayout, buf78, tex);
                }
            }
        }
    }

    u8 count2 = code80135FDC_getByte_6407E();
    for (u8 k = 0; k < 3; k++) {
        if (k >= count2) {
            sprintf(buf38, &lbl_eu_80507D40[0xd0], k + 1);
            nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName(buf38, true);
            func_80124270(pane, 0);
            continue;
        }
        u8 m2 = func_801392C8(k);
        if (m2 == 0) continue;
        u16 id = func_80136254(lbl_eu_80664098, &lbl_eu_80507D40[0xe1], m2);
        char* name = func_80138F78(id);
        u32 tex = (u32)func_801355F4()->GetResource(0x74696D67, name, 0);
        if (tex != 0) {
            sprintf(buf18, &lbl_eu_80507D40[0xed], k + 1);
            func_80137E7C(self->mLayout, buf18, tex);
        }
    }
}

// Refresh the party-state header text: format the frame-timer counters and
// bind them to the panel's text panes. The timer >> 12 is the minute counter
// (clamped to 99/999); the low field (raw >> 6 & 0x3f, max 59) is the seconds.
extern "C" __declspec(noinline) void func_801FE154(CPartyState* self) {
    u32 raw = func_8006A80C();
    u32 t = raw >> 12;
    char buf[8];
    if (t > 0x63) {
        sprintf(buf, lbl_eu_80507D40 + 0xb, (t > 0x3e7) ? 0x3e7 : t);
    } else {
        sprintf(buf, lbl_eu_80507D40 + 0xb, (t > 0x63) ? 0x63 : t);
    }
    func_80136A1C(self->mLayout, lbl_eu_80507D40 + 0x100, buf, 0);
    u8 secs;
    if (t > 0x3e7) {
        secs = 0x3b;
    } else {
        secs = (raw >> 6) & 0x3f;
    }
    sprintf(buf, lbl_eu_80507D40 + 0xb, secs);
    func_80136A1C(self->mLayout, lbl_eu_80507D40 + 0x10d, buf, 0);
}

// Position a party panel pane by name: compute a ratio (0 when either input
// is zero), index a 5-float position table by it, and write the pane's X
// translate (Y/Z kept from the pane's size field). arg4 is unused.
extern "C" __declspec(noinline) void func_801FE39C(CPartyState* self, float f1, float f2, u32 arg4, u32 name) {
    u32 flag = 0;
    if (f1 != lbl_eu_80668230 && f2 != lbl_eu_80668230) {
        flag = 1;
    }
    float ratio;
    if (flag != 0) {
        ratio = f1 / f2;
    } else {
        ratio = lbl_eu_80668230;
    }
    nw4r::lyt::Pane* pane = self->mLayout->GetRootPane()->FindPaneByName((const char*)name, true);
    if (pane != NULL) {
        int idx = (int)((lbl_eu_80668244 * ratio) * lbl_eu_80668240);
        CPartyStateFiveFloats table;
        table = *(const CPartyStateFiveFloats*)lbl_eu_80507D20;
        float value = lbl_eu_80668248 * ratio + table.f[(u8)idx];
        float tmp[2];
        func_80127BC4(tmp, reinterpret_cast<float*>(reinterpret_cast<u8*>(pane) + 0x4c));
        tmp[0] = value;
        func_80124288(pane, tmp);
    }
}

// Load the party-state layout archive and build the UI: scratch region,
// layout + anim transforms, font binding, member text panes, cursor.
bool CPartyState::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle != pEventFile->mFileHandle) return false;

    // Set up the scratch heap region and attach the exchanged file data as
    // the nw4r layout archive, then build the layout + two animations.
    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    mMemRegion.createRegion(mem2, 0x18000, &lbl_eu_80507D40[0x130], 1);
    Class_8045F858 memHost(&mMemRegion);
    CBaseCur tmp;
    char buf28[0x20];
    char buf48[0x20];
    char buf68[0x20];

    u8* fileData = (u8*)mFileHandle->getData();
    mtl::MemManager::func_80434A4C(false);
    mArcResAcc = CLibLayout::createArcResourceAccessor();
    mArcResAcc->Attach(fileData, &lbl_eu_80507D40[0x13c]);
    func_80136E84(&mLayout, mArcResAcc, &lbl_eu_80507D40[0x140]);
    func_80136F08(mLayout, &mAnimTrans1, mArcResAcc, &lbl_eu_80507D40[0x150]);
    func_80136F08(mLayout, &mAnimTrans0, mArcResAcc, &lbl_eu_80507D40[0x163]);

    // Bind the font: root pane + font object slot 7, push back onto root.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    FontHelper* font = reinterpret_cast<FontHelper*>(CDeviceFont::func_80452C10(1, mLayout));
    func_8013676C(rootPane, (void*)font->v7());

    u32 textVal = (u32)func_801355BC();
    if (textVal != 0) {
        func_801368C0(mLayout, &lbl_eu_80507D40[0x100], textVal);
        func_801368C0(mLayout, &lbl_eu_80507D40[0x10d], textVal);
        func_801368C0(mLayout, &lbl_eu_80507D40[0x17b], textVal);
        func_801368C0(mLayout, &lbl_eu_80507D40[0x189], textVal);
        for (u8 i = 1; i <= 7; i++) {
            sprintf(buf68, &lbl_eu_80507D40[0x196], i);
            func_801368C0(mLayout, buf68, textVal);
            sprintf(buf68, &lbl_eu_80507D40[0x10], i);
            func_801368C0(mLayout, buf68, func_801355D8());
            sprintf(buf68, &lbl_eu_80507D40[0x1a8], i);
            func_801368C0(mLayout, buf68, func_801355D8());
            sprintf(buf68, &lbl_eu_80507D40[0x23], i);
            func_801368C0(mLayout, buf68, textVal);
            sprintf(buf68, &lbl_eu_80507D40[0x36], i);
            func_801368C0(mLayout, buf68, textVal);
        }
    }
    mLayout->SetAnimationEnable(mAnimTrans0, false);
    mLayout->SetAnimationEnable(mAnimTrans1, true);
    mLayout->Animate(0);

    char* s2 = func_80136190(&lbl_eu_80507D40[0x1ba], &lbl_eu_80507D40[0x1c4], 2);
    char* s3 = func_80136190(&lbl_eu_80507D40[0x1ba], &lbl_eu_80507D40[0x1c4], 3);
    char* s4 = func_80136190(&lbl_eu_80507D40[0x1ba], &lbl_eu_80507D40[0x1c4], 4);
    func_80136190(&lbl_eu_80507D40[0x1ba], &lbl_eu_80507D40[0x1c4], 5);
    for (u8 i = 1; i <= 7; i++) {
        sprintf(buf48, &lbl_eu_80507D40[0x1a8], i);
        func_80136B4C(mLayout, buf48, s2, 0);
        sprintf(buf48, &lbl_eu_80507D40[0x1c9], i);
        func_80136B4C(mLayout, buf48, s3, 0);
        sprintf(buf48, &lbl_eu_80507D40[0x196], i);
        func_80136B4C(mLayout, buf48, s4, 0);
    }
    func_801FE154(this);
    char* s8 = func_80136190(&lbl_eu_80507D40[0x1ba], &lbl_eu_80507D40[0x1c4], 8);
    sprintf(buf28, &lbl_eu_80507D40[0x1db], func_801571FC(), s8);
    func_80136A1C(mLayout, &lbl_eu_80507D40[0x189], buf28, 0);
    char* s9 = func_80136190(&lbl_eu_80507D40[0x1ba], &lbl_eu_80507D40[0x1c4], 9);
    func_80136B4C(mLayout, &lbl_eu_80507D40[0x17b], s9, 0);

    // Build the embedded cursor on the stack and copy its state into place.
    __ct__CCur22(&tmp, mArcResAcc);
    mCur22.mArcResAcc = tmp.mArcResAcc;
    mCur22.mpLayout = tmp.mpLayout;
    mCur22.mpAnimTrans0 = tmp.mpAnimTrans0;
    mCur22.mpAnimTrans1 = tmp.mpAnimTrans1;
    mCur22.mActive = tmp.mActive;
    mCur22.mVisible = tmp.mVisible;
    __dt__6CCur22Fv(&tmp, -1);
    reinterpret_cast<CPartyStateCur*>(&mCur22)->vfn0();

    func_801FD8A0(this);
    mFileHandle = 0;
    mMemRegion.func_8045F810();
    return true;
}

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

// Layout render (retail mangled name; declared per code_80135FDC.hpp). The
// cursor draw helper is declared in CEquipItemBox.hpp.
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);

// Render the party list when the visible flag is set: draw the bound layout
// with the given draw info (projection off, calc mtx on), then draw the
// embedded cursor.
#pragma optimize_for_size on
extern "C" void func_801FD0A0(CPartyState* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->field_0x28 != 0) {
        func_80137038(self->mLayout, drawInfo, 0, 1);
        func_801D20B0(&self->mCur22, drawInfo);
    }
}
#pragma optimize_for_size off

// One-time member-list setup: skip if the advance/state word is already
// active; otherwise mark it active, clear the panel flags, and run the shared
// slot-table refresh.
extern "C" void func_801FD194(CPartyState* self) {
    if (self->field_0x2C != 0)
        return;
    self->field_0x2C = 1;
    self->field_0x31 = 0;
    self->field_0x4E = 0;
    func_801FDA7C(self);
}

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
    func_801FD8F8((CPartyState*)self);
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
