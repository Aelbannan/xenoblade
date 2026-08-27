// Auto-scaffolded catalog TU for kyoshin/CQstLogInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CQstLogInfo.hpp"
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/CTagProcessor.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"

#include <stdio.h>

u8 CQstLogInfo::func_802296D0() { return mField38; }

u8 CQstLogInfo::func_802296D8() { return mField39; }

// Start the quest-log state machine: mark busy (+0x34=1), clear the 0x39
// flag, run the state step and play sound 0x6d. Guarded by the state.
extern "C" void func_802296E0(CQstLogInfo* self) {
    if (self->field_0x34 == 0) {
        self->field_0x34 = 1;
        self->mField39 = 0;
        func_802298A0(self);
        playUISound(0x6d);
    }
}

// Continue the quest-log machine when idle-at-step-3: mark busy (+0x34=4),
// clear the 0x39 flag, step and play sound 6.
extern "C" void func_80229724(CQstLogInfo* self) {
    if (self->field_0x34 == 3) {
        self->field_0x34 = 4;
        self->mField39 = 0;
        func_80229900(self);
        playUISound(6);
    }
}

void CQstLogInfo::func_80229768(u16 val) { mField3A = val; }

// Destructor: destroys the embedded scratch region, then frees the object
// when the deleting-dtor flag is set (standard MWCC pattern).
// optimize_for_size: retail uses the stmw r30/lmw r30 frame; -O4,p splits
// the saves into separate stw's (MWCC_CASES kyoshin-dtor pattern).
#pragma optimize_for_size on
CQstLogInfo::~CQstLogInfo() {}
#pragma optimize_for_size off

// Constructor: base ctor stores the vtable first, then the mMemRegion member
// ctor runs (retail member-init order; CBatteryVtblBase pattern), then the
// zero/default field init.
CQstLogInfo::CQstLogInfo() {
    mFileHandle = nullptr;
    field_0x18 = nullptr;
    field_0x1C = 0;
    mUnk20 = nullptr;
    field_0x24 = nullptr;
    field_0x28 = nullptr;
    field_0x2C = 0;
    mUnk30 = 0;
    field_0x34 = 0;
    mField38 = 0;
    mField39 = 1;
    mField3A = 0;
    field_0x3C = 0;
}

// us-8022b5a8 - func_80229770
// If the animation at 0x24 finishes, transition to state 2.
extern "C" __declspec(noinline) void func_80229770(CQstLogInfo* self) {
    f32 frame = lbl_eu_80668590;
    nw4r::lyt::AnimTransform* anim = self->field_0x24;
    if (advanceAnimTransform(anim, frame) != 0) {
        self->field_0x34 = 2;
        func_80229900(self);
    }
}

// us-8022b5f4 - func_802297BC
// If the animation at 0x28 finishes, transition to state 3 and set field 0x39.
extern "C" __declspec(noinline) void func_802297BC(CQstLogInfo* self) {
    f32 frame = lbl_eu_80668590;
    nw4r::lyt::AnimTransform* anim = self->field_0x28;
    if (advanceAnimTransform(anim, frame) != 0) {
        self->field_0x34 = 3;
        self->mField39 = 1;
    }
}

// us-8022b640 - func_80229808
// If the animation at 0x28 is done (via func_80137510), transition to state 5.
extern "C" __declspec(noinline) void func_80229808(CQstLogInfo* self) {
    if (func_80137510(self->field_0x28, lbl_eu_80668590) != 0) {
        self->field_0x34 = 5;
        func_802298A0(self);
    }
}

// us-8022b68c - func_80229854
// If the animation at 0x24 is done (via func_80137510), reset to state 0.
extern "C" __declspec(noinline) void func_80229854(CQstLogInfo* self) {
    if (func_80137510(self->field_0x24, lbl_eu_80668590) != 0) {
        self->field_0x34 = 0;
        self->mField39 = 1;
    }
}

// us-8022b2f8 - func_802294C0
// Load the quest-log arc into a mem2 file handle, then clear the loaded flag.
void func_802294C0(CQstLogInfo* self) {
    self->mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              lbl_eu_8050A0B4, reinterpret_cast<IWorkEvent*>(self), 0, 0);
    self->mField38 = 0;
}

// Same SetAnimationEnable shape as CQstLogList::func_802284E4 (FULL_MATCH).
extern "C" __declspec(noinline) void func_802298A0(CQstLogInfo* self) {
    self->mUnk20->SetAnimationEnable(self->field_0x28, false);
    self->mUnk20->SetAnimationEnable(self->field_0x24, true);
}

extern "C" __declspec(noinline) void func_80229900(CQstLogInfo* self) {
    self->mUnk20->SetAnimationEnable(self->field_0x24, false);
    self->mUnk20->SetAnimationEnable(self->field_0x28, true);
}

// State machine driver: while the quest-log info is active, dispatch on the
// current state (1/2/4/5) to the per-state animation handlers, then advance
// the layout animation by one frame.
extern "C" __declspec(noinline) void func_80229570(CQstLogInfo* self) {
    if (self->mUnk30 == 0) return;
    switch (self->field_0x34) {
    case 1: func_80229770(self); break;
    case 2: func_802297BC(self); break;
    case 4: func_80229808(self); break;
    case 5: func_80229854(self); break;
    }
    self->mUnk20->Animate(0);
}

// Quest-log teardown: release the bdat handle, close the two arc handles,
// delete the layout + tag processor, release the resource accessor and tear
// down the scratch region.
extern "C" __declspec(noinline) void func_80229620(CQstLogInfo* self) {
    CBdat::getEntry(2);
    func_801390E0(&self->mFileHandle);
    func_801390E0(&self->field_0x18);
    self->mUnk30 = 0;
    if (self->mUnk20 != nullptr) {
        delete self->mUnk20;
        self->mUnk20 = nullptr;
    }
    if (self->field_0x2C != 0) {
        delete (CTagProcessor*)self->field_0x2C;
        self->field_0x2C = 0;
    }
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C);
    self->mMemRegion.func_8045F778();
}

// Loads the quest-category arc: maps mField3A through the quest-name table
// (lbl_eu_80536320) and reads the common archive file into field_0x18,
// clearing the loaded flag mField38.
#pragma optimize_for_size on
void func_80229510(CQstLogInfo* self) {
    self->field_0x18 = CDeviceFile::readCommonArchiveFile(
        func_800A9D90(),
        lbl_eu_80536320[func_80138138(self->mField3A)],
        reinterpret_cast<IWorkEvent*>(self), 0, 0);
    self->mField38 = 0;
}
#pragma optimize_for_size off

// Full quest-log refresh: reset the log to the current quest id, then run the
// name / description / pane-color / icon chain of sub-updates.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80229960(CQstLogInfo* self) {
    func_8022AFF8(self);
    u8 v = (u8)func_8009CF8C(self->mField3A + 0x220);
    u32 idx = func_80138138(self->mField3A);
    func_80229A0C(self, (const char*)lbl_eu_80573D18[idx], self->mField3A, v);
    func_80229B54(self, (const char*)lbl_eu_80573D18[idx], self->mField3A);
    func_80229CA0(self, (const char*)self->field_0x3C, self->mField3A);
    func_80229CF0(self, (const char*)lbl_eu_80573D18[idx], self->mField3A, v);
    func_8022A904(self, (const char*)lbl_eu_80573D18[idx], self->mField3A, v);
}
#pragma optimize_for_size off

// Name-pane + icon update for one quest entry: BDAT text lookup into the
// layout, root-pane icon lookup, then a 'timg' texture resource select by the
// quest-category column, applied to the icon pane.
extern "C" __declspec(noinline) void func_80229A0C(CQstLogInfo* self,
    const char* table, u32 key, u8 v) {
    char* str = func_8013639C(table, &lbl_eu_8050A0B4[0x17], key);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x1d], str, 0);
    nw4r::lyt::Pane* pane = self->mUnk20->GetRootPane()->FindPaneByName(
        &lbl_eu_8050A0B4[0x25], true);
    u8 kind = func_801361E8((u32)table, &lbl_eu_8050A0B4[0x32], key);
    void* res = 0;
    switch (kind) {
    case 0:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C)
                  ->GetResource(0x74696D67, &lbl_eu_8050A0B4[0x3c], 0);
        break;
    case 1:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C)
                  ->GetResource(0x74696D67, &lbl_eu_8050A0B4[0x50], 0);
        break;
    case 3:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C)
                  ->GetResource(0x74696D67, &lbl_eu_8050A0B4[0x64], 0);
        break;
    }
    if (res != 0) {
        func_80137F88(pane, (const char*)res);
        func_80124270(pane, 1);
    }
}

// Sets the quest-log "txt_clear" pane from the BDAT table `table`: looks up
// the "purpose" column of row `key` (0x78), pushes it through the UTF-8
// conversion (func_80138DA4) into the pane, then picks the follow-up text
// ("cleared"/"uncleared"/level-up variants) by quest state and category and
// updates the "txt_clear_reason" pane at 0xab.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80229B54(CQstLogInfo* self,
    const char* table, u32 key) {
    u16 v = func_80136254(table, &lbl_eu_8050A0B4[0x78], key);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x84],
                  (char*)func_80138DA4(func_8013639C(
                      (const char*)lbl_eu_80664098, &lbl_eu_8050A0B4[0x7f], v)),
                  0);
    int flag = 0;
    if (v >= 0x36C && v <= 0x36E || v == 0x38C || v == 0x391 || v == 0x393) {
        flag = 1;
    }
    char* str;
    if (flag != 0) {
        str = func_80136190(&lbl_eu_8050A0B4[0x8c], &lbl_eu_8050A0B4[0x7f], 0x35);
    } else {
        if (func_801361E8((u32)table, &lbl_eu_8050A0B4[0x32], key) != 1) {
            u16 v2 = func_80136254((const char*)lbl_eu_80664098,
                                   &lbl_eu_8050A0B4[0x96], v);
            str = func_80136190(&lbl_eu_8050A0B4[0x9e], &lbl_eu_8050A0B4[0x7f], v2);
        } else {
            str = func_80136190(&lbl_eu_8050A0B4[0x8c], &lbl_eu_8050A0B4[0x7f], 0x35);
        }
    }
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0xab], str, 0);
}
#pragma optimize_for_size off

// Sets the quest-log "txt_exp" pane from the BDAT table `table`: looks up
// the "purpose" column of row `key` and writes the result into the layout
// (field_0x2C selects the pane color/state).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80229CA0(CQstLogInfo* self, const char* table, u32 key) {
    char* str = func_8013639C(table, &lbl_eu_8050A0B4[0xb3], key);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0xbb], str, self->field_0x2C);
}
#pragma optimize_for_size off

// Quest-log objective/sub-objective update. For each of the four level bands
// (0x6f / 0x79 / 0x83 / 0xfc) it formats a pane name from the band-specific
// format, looks the row up in the BDAT quest table, and binds the found text
// into the layout; the odd/even variant of each format picks the column. The
// kind != 1 path repeats the fixed rows (loop over i) and binds them by name.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80229CF0(CQstLogInfo* self,
    const char* table, u32 key, u8 v) {
    char bufE8[0x20];
    char bufC8[0x20];
    char bufA8[0x20];
    char buf88[0x20];
    char buf68[0x20];
    char buf48[0x20];
    char buf28[0x20];
    char buf8[0x20];
    if (func_801361E8((u32)table, &lbl_eu_8050A0B4[0xc3], key) == 1) {
        char* s = func_8013639C((const void*)self->field_0x3C,
                                &lbl_eu_8050A0B4[0xd0], key);
        func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0xda], s,
                      self->field_0x2C);
        func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                          &lbl_eu_8050A0B4[0xda], true), 1);
        func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                          &lbl_eu_8050A0B4[0xe7], true), 1);
        if (v > 1) {
            func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                              &lbl_eu_8050A0B4[0xf2], true), 1);
        }
        u32 idx = 1;
        if (v >= 0x6F) {
            u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
            if (odd) {
                sprintf(bufE8, &lbl_eu_8050A0B4[0xfb]);
            } else {
                sprintf(bufE8, &lbl_eu_8050A0B4[0x108]);
            }
            u32 val = func_80136254(table, bufE8, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(bufE8, &lbl_eu_8050A0B4[0x115]);
                } else {
                    sprintf(bufE8, &lbl_eu_8050A0B4[0x122]);
                }
                char* s2 = func_8013639C((const void*)self->field_0x3C,
                                         bufE8, key);
                sprintf(bufE8, &lbl_eu_8050A0B4[0x12f], 2);
                func_80136B4C(self->mUnk20, bufE8, s2, self->field_0x2C);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufE8, true), 1);
                sprintf(bufE8, &lbl_eu_8050A0B4[0x13e], 2);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufE8, true), 1);
                if (v >= 0x79) {
                    sprintf(bufE8, &lbl_eu_8050A0B4[0x14b], 2);
                    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                      bufE8, true), 1);
                }
                idx = 2;
            }
        }
        if (v >= 0x79) {
            u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
            if (odd) {
                sprintf(bufC8, &lbl_eu_8050A0B4[0x156]);
            } else {
                sprintf(bufC8, &lbl_eu_8050A0B4[0x163]);
            }
            u32 val = func_80136254(table, bufC8, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(bufC8, &lbl_eu_8050A0B4[0x170]);
                } else {
                    sprintf(bufC8, &lbl_eu_8050A0B4[0x17d]);
                }
                char* s2 = func_8013639C((const void*)self->field_0x3C,
                                         bufC8, key);
                sprintf(bufC8, &lbl_eu_8050A0B4[0x12f], idx + 1);
                func_80136B4C(self->mUnk20, bufC8, s2, self->field_0x2C);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufC8, true), 1);
                sprintf(bufC8, &lbl_eu_8050A0B4[0x13e], idx + 1);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufC8, true), 1);
                if (v >= 0x83) {
                    sprintf(bufC8, &lbl_eu_8050A0B4[0x14b], idx + 1);
                    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                      bufC8, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0x83) {
            u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
            if (odd) {
                sprintf(bufA8, &lbl_eu_8050A0B4[0x18a]);
            } else {
                sprintf(bufA8, &lbl_eu_8050A0B4[0x197]);
            }
            u32 val = func_80136254(table, bufA8, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(bufA8, &lbl_eu_8050A0B4[0x1a4]);
                } else {
                    sprintf(bufA8, &lbl_eu_8050A0B4[0x1b1]);
                }
                char* s2 = func_8013639C((const void*)self->field_0x3C,
                                         bufA8, key);
                sprintf(bufA8, &lbl_eu_8050A0B4[0x12f], idx + 1);
                func_80136B4C(self->mUnk20, bufA8, s2, self->field_0x2C);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufA8, true), 1);
                sprintf(bufA8, &lbl_eu_8050A0B4[0x13e], idx + 1);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufA8, true), 1);
                if (v >= 0xFC) {
                    sprintf(bufA8, &lbl_eu_8050A0B4[0x14b], idx + 1);
                    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                      bufA8, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0xFC) {
            if (func_801361E8((u32)table, &lbl_eu_8050A0B4[0x1be], key) == 0) {
                char* s2 = func_8013639C((const void*)self->field_0x3C,
                                         &lbl_eu_8050A0B4[0x1c8], key);
                sprintf(buf88, &lbl_eu_8050A0B4[0x12f], idx + 1);
                func_80136B4C(self->mUnk20, buf88, s2, self->field_0x2C);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf88, true), 1);
                sprintf(buf88, &lbl_eu_8050A0B4[0x13e], idx + 1);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf88, true), 1);
                if (v >= 0xFE) {
                    sprintf(buf88, &lbl_eu_8050A0B4[0x14b], idx + 1);
                    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                      buf88, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0xFE) {
            char* s2 = (v & 1) != 0
                ? func_8013639C((const void*)self->field_0x3C,
                                &lbl_eu_8050A0B4[0x1d4], key)
                : func_8013639C((const void*)self->field_0x3C,
                                &lbl_eu_8050A0B4[0x1df], key);
            sprintf(buf68, &lbl_eu_8050A0B4[0x12f], idx + 1);
            func_80136B4C(self->mUnk20, buf68, s2, self->field_0x2C);
            func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                              buf68, true), 1);
        }
    } else {
        char* s = func_8013639C((const void*)self->field_0x3C,
                                &lbl_eu_8050A0B4[0xd0], key);
        func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0xda], s,
                      self->field_0x2C);
        func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                          &lbl_eu_8050A0B4[0xda], true), 1);
        func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                          &lbl_eu_8050A0B4[0xe7], true), 1);
        u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
        void* r28 = func_801412D0(key);
        u32 notOdd = !odd;
        if (func_80140854((CItemQuery*)r28, notOdd, 0) != 0) {
            func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                              &lbl_eu_8050A0B4[0xf2], true), 1);
        }
        u32 idx = 1;
        for (u8 i = 0; i < 3; i++) {
            if (odd) {
                sprintf(buf48, &lbl_eu_8050A0B4[0x1ea], i + 2);
            } else {
                sprintf(buf48, &lbl_eu_8050A0B4[0x1f8], i + 2);
            }
            u32 val = func_80136254(table, buf48, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(buf48, &lbl_eu_8050A0B4[0x206], i + 1);
                } else {
                    sprintf(buf48, &lbl_eu_8050A0B4[0x214], i + 1);
                }
                char* s2 = func_8013639C((const void*)self->field_0x3C,
                                         buf48, key);
                sprintf(buf48, &lbl_eu_8050A0B4[0x12f], idx + 1);
                func_80136B4C(self->mUnk20, buf48, s2, self->field_0x2C);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf48, true), 1);
                sprintf(buf48, &lbl_eu_8050A0B4[0x13e], idx + 1);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf48, true), 1);
                if (i == 3) {
                    if (v >= 0xFC) {
                        sprintf(buf48, &lbl_eu_8050A0B4[0x14b], idx + 1);
                        func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                          buf48, true), 1);
                        idx++;
                    }
                } else {
                    if (func_80140854((CItemQuery*)r28, notOdd, i + 1) != 0) {
                        sprintf(buf48, &lbl_eu_8050A0B4[0x14b], idx + 1);
                        func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                          buf48, true), 1);
                    }
                    idx++;
                }
            }
        }
        if (v >= 0xFC) {
            if (func_801361E8((u32)table, &lbl_eu_8050A0B4[0x1be], key) == 0) {
                char* s2 = func_8013639C((const void*)self->field_0x3C,
                                         &lbl_eu_8050A0B4[0x1c8], key);
                sprintf(buf28, &lbl_eu_8050A0B4[0x12f], idx + 1);
                func_80136B4C(self->mUnk20, buf28, s2, self->field_0x2C);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf28, true), 1);
                sprintf(buf28, &lbl_eu_8050A0B4[0x13e], idx + 1);
                func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf28, true), 1);
                if (v >= 0xFE) {
                    sprintf(buf28, &lbl_eu_8050A0B4[0x14b], idx + 1);
                    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                                      buf28, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0xFE) {
            char* s2 = (v & 1) != 0
                ? func_8013639C((const void*)self->field_0x3C,
                                &lbl_eu_8050A0B4[0x1d4], key)
                : func_8013639C((const void*)self->field_0x3C,
                                &lbl_eu_8050A0B4[0x1df], key);
            sprintf(buf8, &lbl_eu_8050A0B4[0x12f], idx + 1);
            func_80136B4C(self->mUnk20, buf8, s2, self->field_0x2C);
            func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                              buf8, true), 1);
        }
    }
}
#pragma optimize_for_size off

// Quest-log objective/sub-objective update with per-band pane binding (see
// func_80229CF0 for the v >= 0xFE band). The two loops are identical except
// for their pane-name formats and buffers; the switch maps the BDAT quest
// type (v1) to the table that supplies the objective text.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8022A904(CQstLogInfo* self,
    const char* table, u32 key, u8 v) {
    char buf8[0x20];
    char buf28[0x20];
    char buf48[0x20];
    char buf68[0x20];
    char* str = 0;
    if (v >= 0xFE) {
        u8 kind = func_801361E8((u32)table, &lbl_eu_8050A0B4[0x222], key);
        if (kind == 1) {
            func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x22d],
                          func_80136190(&lbl_eu_8050A0B4[0x8c],
                                        &lbl_eu_8050A0B4[0x7f], 0x33), 0);
            setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x238], 0);
            setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x243], 0);
            return;
        }
        u16 a = func_80136254(table, &lbl_eu_8050A0B4[0x24d], key);
        u16 b = func_80136254(table, &lbl_eu_8050A0B4[0x258], key);
        setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x238], b * 10);
        setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x243], a * 10);
        u32 odd = v & 1;
        for (u8 i = 0; i < 3; i++) {
            if (odd) {
                sprintf(buf48, &lbl_eu_8050A0B4[0x265], i + 1);
            } else {
                sprintf(buf48, &lbl_eu_8050A0B4[0x270], i + 1);
            }
            u32 val = func_80136254(table, buf48, key);
            if ((u16)val != 0) {
                u32 v1 = func_801392E4((u16)val);
                u32 v2 = func_80139358((u16)val);
                switch ((u16)v1) {
                case 0:
                    str = func_8013639C((const void*)lbl_eu_806640F4,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 1:
                    str = func_8013639C((const void*)lbl_eu_806640D8,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 2:
                    str = func_8013639C((const void*)lbl_eu_806640F8,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 3:
                    str = func_8013639C((const void*)lbl_eu_806640FC,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 4:
                    str = func_8013639C((const void*)lbl_eu_80664104,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 5:
                    str = func_8013639C((const void*)lbl_eu_80664108,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 6:
                    str = func_8013639C((const void*)lbl_eu_8066410C,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 7:
                    str = func_8013639C((const void*)lbl_eu_80664110,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                default:
                    break;
                }
                sprintf(buf48, &lbl_eu_8050A0B4[0x27b], i + 1);
                if ((u16)v1 == 3) {
                    u8 x = func_801361E8((u32)lbl_eu_806640EC,
                                         &lbl_eu_8050A0B4[0x288], (u16)val);
                    sprintf(buf68, &lbl_eu_8050A0B4[0x29a], str,
                            func_80136190(&lbl_eu_8050A0B4[0x291],
                                          &lbl_eu_8050A0B4[0x7f],
                                          0x1e - (x - 1)));
                } else {
                    sprintf(buf68, &lbl_eu_8050A0B4[0x29f], str);
                }
                func_80136A1C(self->mUnk20, buf48, buf68, 0);
            } else if (i == 0) {
                str = func_80136190(&lbl_eu_8050A0B4[0x8c],
                                    &lbl_eu_8050A0B4[0x7f], 0x33);
                func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x22d], str, 0);
            }
        }
    } else {
        u8 kind = func_801361E8((u32)table, &lbl_eu_8050A0B4[0x222], key);
        if (kind == 3) {
            func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x22d],
                          func_80136190(&lbl_eu_8050A0B4[0x8c],
                                        &lbl_eu_8050A0B4[0x7f], 0x32), 0);
            u16 a = func_80136254(table, &lbl_eu_8050A0B4[0x24d], key);
            u16 b = func_80136254(table, &lbl_eu_8050A0B4[0x258], key);
            setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x238], b * 10);
            setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x243], a * 10);
            return;
        }
        if (kind == 1) {
            func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x22d],
                          func_80136190(&lbl_eu_8050A0B4[0x8c],
                                        &lbl_eu_8050A0B4[0x7f], 0x33), 0);
            setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x238], 0);
            setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x243], 0);
            return;
        }
        u16 a = func_80136254(table, &lbl_eu_8050A0B4[0x24d], key);
        u16 b = func_80136254(table, &lbl_eu_8050A0B4[0x258], key);
        setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x238], b * 10);
        setLayoutTextBoxNumber(self->mUnk20, &lbl_eu_8050A0B4[0x243], a * 10);
        u32 odd = v & 1;
        for (u8 i = 0; i < 3; i++) {
            if (v == 1) {
                sprintf(buf8, &lbl_eu_8050A0B4[0x270], i + 1);
            } else if (v >= 0xFC) {
                if (odd) {
                    sprintf(buf8, &lbl_eu_8050A0B4[0x265], i + 1);
                } else {
                    sprintf(buf8, &lbl_eu_8050A0B4[0x270], i + 1);
                }
            } else {
                if (odd) {
                    sprintf(buf8, &lbl_eu_8050A0B4[0x270], i + 1);
                } else {
                    sprintf(buf8, &lbl_eu_8050A0B4[0x265], i + 1);
                }
            }
            u32 val = func_80136254(table, buf8, key);
            if ((u16)val != 0) {
                u32 v1 = func_801392E4((u16)val);
                u32 v2 = func_80139358((u16)val);
                switch ((u16)v1) {
                case 0:
                    str = func_8013639C((const void*)lbl_eu_806640F4,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 1:
                    str = func_8013639C((const void*)lbl_eu_806640D8,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 2:
                    str = func_8013639C((const void*)lbl_eu_806640F8,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 3:
                    str = func_8013639C((const void*)lbl_eu_806640FC,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 4:
                    str = func_8013639C((const void*)lbl_eu_80664104,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 5:
                    str = func_8013639C((const void*)lbl_eu_80664108,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 6:
                    str = func_8013639C((const void*)lbl_eu_8066410C,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                case 7:
                    str = func_8013639C((const void*)lbl_eu_80664110,
                                        &lbl_eu_8050A0B4[0x7f], (u16)v2);
                    break;
                default:
                    break;
                }
                sprintf(buf8, &lbl_eu_8050A0B4[0x27b], i + 1);
                if ((u16)v1 == 3) {
                    u8 x = func_801361E8((u32)lbl_eu_806640EC,
                                         &lbl_eu_8050A0B4[0x288], (u16)val);
                    sprintf(buf28, &lbl_eu_8050A0B4[0x29a], str,
                            func_80136190(&lbl_eu_8050A0B4[0x291],
                                          &lbl_eu_8050A0B4[0x7f],
                                          0x1e - (x - 1)));
                } else {
                    sprintf(buf28, &lbl_eu_8050A0B4[0x29f], str);
                }
                func_80136A1C(self->mUnk20, buf8, buf28, 0);
            } else if (i == 0) {
                str = func_80136190(&lbl_eu_8050A0B4[0x8c],
                                    &lbl_eu_8050A0B4[0x7f], 0x33);
                func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x22d], str, 0);
            }
        }
    }
}
#pragma optimize_for_size off

// Quest-log reset: clears every quest-name / objective / status pane back to
// the empty string (pool+0x2a2) and resets the state panes' visibility.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_8022AFF8(CQstLogInfo* self) {
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x1d], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x84], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0xab], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0xbb], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x25], true), 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0xda], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x2a3], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x2b0], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x2bd], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x2ca], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x2d7], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0xe7], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x2e4], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x2ef], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x2fa], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x305], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x310], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0xf2], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x31b], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x324], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x32d], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x336], true), 0);
    func_80124270(self->mUnk20->GetRootPane()->FindPaneByName(
                      &lbl_eu_8050A0B4[0x33f], true), 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x22d], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x348], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x353], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x238], &lbl_eu_8050A0B4[0x2a2], 0);
    func_80136B4C(self->mUnk20, &lbl_eu_8050A0B4[0x243], &lbl_eu_8050A0B4[0x2a2], 0);
}
#pragma optimize_for_size off

// File-load completion callback for the two quest-log arc handles. The
// quest-log arc (mFileHandle) builds the layout; the common archive
// (field_0x18) primes the quest-category BDAT table (field_0x3C) and refreshes
// the whole log display.
#pragma optimize_for_size on
int CQstLogInfo::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        mMemRegion.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x10000,
                                &lbl_eu_8050A0B4[0x35e], 0);
        Class_8045F858 guard(&mMemRegion);
        void* fileData = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);
        void* tagMem = mtl::MemManager::allocate(
            0x858, getAllocHandle__10CLibLayoutFv());
        if (tagMem != 0) {
            tagMem = __ct__CTagProcessor(tagMem);
        }
        field_0x2C = (u32)tagMem;
        field_0x1C = (u32)createArcResourceAccessor__10CLibLayoutFv();
        ((nw4r::lyt::ArcResourceAccessor*)field_0x1C)->Attach(
            fileData, &lbl_eu_8050A0B4[0x36a]);
        buildLayout(&mUnk20, (nw4r::lyt::ArcResourceAccessor*)field_0x1C,
                      &lbl_eu_8050A0B4[0x36e]);
        bindLayoutAnimTransform(mUnk20, &field_0x24,
                      (nw4r::lyt::ArcResourceAccessor*)field_0x1C,
                      &lbl_eu_8050A0B4[0x381]);
        bindLayoutAnimTransform(mUnk20, &field_0x28,
                      (nw4r::lyt::ArcResourceAccessor*)field_0x1C,
                      &lbl_eu_8050A0B4[0x397]);

        nw4r::lyt::Pane* rootPane = mUnk20->GetRootPane();
        void* font = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, mUnk20);
        func_8013676C(rootPane, ((CQstLogInfoFontView*)font)->sf9());

        char* questName = func_801355BC();
        if (questName != 0) {
            setLayoutTextBoxFont(mUnk20, &lbl_eu_8050A0B4[0x238], (u32)questName);
            setLayoutTextBoxFont(mUnk20, &lbl_eu_8050A0B4[0x3b2], (u32)questName);
            setLayoutTextBoxFont(mUnk20, &lbl_eu_8050A0B4[0x243], (u32)questName);
        }
        func_80136B4C(mUnk20, &lbl_eu_8050A0B4[0x3b2],
                      func_80136190(&lbl_eu_8050A0B4[0x291],
                                    &lbl_eu_8050A0B4[0x7f], 3), 0);
        func_802298A0(this);
        mUnk20->Animate(0);
        char buf[0x18];
        for (u8 i = 1; i <= 6; i++) {
            sprintf(buf, &lbl_eu_8050A0B4[0x12f], i);
            nw4r::lyt::Pane* pane = mUnk20->GetRootPane()->FindPaneByName(
                buf, true);
            ((CQstLogInfoPaneUser*)pane)->field_F8 = field_0x2C;
        }
        nw4r::lyt::Pane* pane = mUnk20->GetRootPane()->FindPaneByName(
            &lbl_eu_8050A0B4[0xbb], true);
        ((CQstLogInfoPaneUser*)pane)->field_F8 = field_0x2C;
        mField38 = 1;
        mUnk30 = 1;
        mFileHandle = 0;
        mMemRegion.func_8045F810();
        return 1;
    } else if (field_0x18 == event->mFileHandle) {
        void* fileData = field_0x18->getData();
        CBdat::func_8003AA78(2, fileData);
        func_8003AA34();
        field_0x3C = (u32)getFP__FPCc(lbl_eu_80536398[func_80138138(mField3A)]);
        func_80229960(this);
        mField38 = 1;
        field_0x18 = 0;
        return 1;
    }
    return 0;
}
#pragma optimize_for_size off

// retail: if (mUnk30) tail drawLayout(mUnk20, drawInfo passthrough, 0, 1)
void func_80229600(CQstLogInfo* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mUnk30 == 0) return;
    drawLayout(self->mUnk20, drawInfo, 0, 1);
}
