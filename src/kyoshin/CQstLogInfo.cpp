// Auto-scaffolded catalog TU for kyoshin/CQstLogInfo
// Replace stubs with high-level C/C++ during decomp.

// Inline-empty ~IWorkEvent: the ctor's cleanup action references the (link-deduped)
// weak copy, and ~CQstLogInfo elides the base call (retail shape).
#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CQstLogInfo.hpp"
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/CTagProcessor.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "nw4r/lyt/lyt_textBox.h"

#include <stdio.h>

// Compiler-owned quest-log data tables.  The pointer values and their
// pointee strings are exactly the retail .data/.rodata objects emitted by
// this TU; writing them as ordinary initializers lets MWCC build the pool.
char* sQstLogTplNames[] = {
    "/common/jp/bdat_qs0000.bin",
    "/common/jp/bdat_qs0101.bin",
    "/common/jp/bdat_qs0201.bin",
    "/common/jp/bdat_qs0301.bin",
    "/common/jp/bdat_qs0401.bin",
    "/common/jp/bdat_qs0402.bin",
    "/common/jp/bdat_qs0501.bin",
    "/common/jp/bdat_qs0601.bin",
    "/common/jp/bdat_qs0701.bin",
    "/common/jp/bdat_qs0801.bin",
    "/common/jp/bdat_qs0901.bin",
    "/common/jp/bdat_qs1001.bin",
    "/common/jp/bdat_qs1101.bin",
    "/common/jp/bdat_qs1201.bin",
    "/common/jp/bdat_qs1202.bin",
    "/common/jp/bdat_qs1301.bin",
    "/common/jp/bdat_qs1401.bin",
    "/common/jp/bdat_qs1501.bin",
    "/common/jp/bdat_qs1601.bin",
    "/common/jp/bdat_qs1601.bin",
    "/common/jp/bdat_qs1701.bin",
    "/common/jp/bdat_qs1801.bin",
    "/common/jp/bdat_qs1901.bin",
    "/common/jp/bdat_qs2001.bin",
    "/common/jp/bdat_qs2101.bin",
    "/common/jp/bdat_qs2201.bin",
    "/common/jp/bdat_qs2301.bin",
    "/common/jp/bdat_qs2401.bin",
    0,
    0,
};

char* sQstLogBdatColumnNames[] = {
    "MNU_qt0000",
    "MNU_qt0101",
    "MNU_qt0201",
    "MNU_qt0301",
    "MNU_qt0401",
    "MNU_qt0402",
    "MNU_qt0501",
    "MNU_qt0601",
    "MNU_qt0701",
    "MNU_qt0801",
    "MNU_qt0901",
    "MNU_qt1001",
    "MNU_qt1101",
    "MNU_qt1201",
    "MNU_qt1202",
    "MNU_qt1301",
    "MNU_qt1401",
    "MNU_qt1501",
    "MNU_qt1601",
    "MNU_qt1601",
    "MNU_qt1701",
    "MNU_qt1801",
    "MNU_qt1901",
    "MNU_qt2001",
    "MNU_qt2101",
    "MNU_qt2201",
    "MNU_qt2301",
    "MNU_qt2401",
};

#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"


int CQstLogInfo::QstLogInfo_GetLoadedMark() { return mField38; }

int CQstLogInfo::QstLogInfo_GetReadyMark() { return mField39; }

// Start the quest-log state machine: mark busy (+0x34=1), clear the 0x39
// flag, run the state step and play sound 0x6d. Guarded by the state.
extern "C" void QstLogInfo_Start(CQstLogInfo* self) {
    if (self->field_0x34 == 0) {
        self->field_0x34 = 1;
        self->mField39 = 0;
        QstLogInfo_SwapAnimEnablePair(self);
        playUISound(0x6d);
    }
}

// Continue the quest-log machine when idle-at-step-3: mark busy (+0x34=4),
// clear the 0x39 flag, step and play sound 6.
extern "C" void QstLogInfo_Resume(CQstLogInfo* self) {
    if (self->field_0x34 == 3) {
        self->field_0x34 = 4;
        self->mField39 = 0;
        QstLogInfo_SwapAnimEnableDual(self);
        playUISound(6);
    }
}

void CQstLogInfo::QstLogInfo_SetQuestIdSlot(u16 val) { mField3A = val; }

// Destructor: destroys the embedded scratch region, then frees the object
// when the deleting-dtor flag is set (standard MWCC pattern).
CQstLogInfo::~CQstLogInfo() {}

// Constructor: IWorkEvent base, the mMemRegion member ctor, then the
// zero/default field init.
CQstLogInfo::CQstLogInfo() : mMemRegion() {
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

// us-8022b5a8 - QstLogInfo_AdvanceState1To2
// If the animation at 0x24 finishes, transition to state 2.
extern "C" void QstLogInfo_AdvanceState1To2(CQstLogInfo* self) {
    f32 frame = 1.0f;
    nw4r::lyt::AnimTransform* anim = self->field_0x24;
    if (advanceAnimTransform(anim, frame) != 0) {
        self->field_0x34 = 2;
        QstLogInfo_SwapAnimEnableDual(self);
    }
}

// us-8022b5f4 - QstLogInfo_AdvanceState2To3
// If the animation at 0x28 finishes, transition to state 3 and set field 0x39.
extern "C" void QstLogInfo_AdvanceState2To3(CQstLogInfo* self) {
    f32 frame = 1.0f;
    nw4r::lyt::AnimTransform* anim = self->field_0x28;
    if (advanceAnimTransform(anim, frame) != 0) {
        self->field_0x34 = 3;
        self->mField39 = 1;
    }
}

// us-8022b640 - QstLogInfo_RewindState4To5
// If the animation at 0x28 is done (via AnimRewindFrame), transition to state 5.
extern "C" void QstLogInfo_RewindState4To5(CQstLogInfo* self) {
    if (AnimRewindFrame(self->field_0x28, 1.0f) != 0) {
        self->field_0x34 = 5;
        QstLogInfo_SwapAnimEnablePair(self);
    }
}

// us-8022b68c - QstLogInfo_RewindState5To0
// If the animation at 0x24 is done (via AnimRewindFrame), reset to state 0.
extern "C" void QstLogInfo_RewindState5To0(CQstLogInfo* self) {
    if (AnimRewindFrame(self->field_0x24, 1.0f) != 0) {
        self->field_0x34 = 0;
        self->mField39 = 1;
    }
}

// us-8022b2f8 - QstLogInfo_LoadArcHandle
// Load the quest-log arc into a mem2 file handle, then clear the loaded flag.
void QstLogInfo_LoadArcHandle(CQstLogInfo* self) {
    self->mFileHandle = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(),
                                              "menu/jp/QstLogInfo.arc", self, 0, 0);
    self->mField38 = 0;
}

// Same SetAnimationEnable shape as CQstLogList::QstLogList_ShowOpenAnim (FULL_MATCH).
extern "C" void QstLogInfo_SwapAnimEnablePair(CQstLogInfo* self) {
    self->mUnk20->SetAnimationEnable(self->field_0x28, false);
    self->mUnk20->SetAnimationEnable(self->field_0x24, true);
}

extern "C" void QstLogInfo_SwapAnimEnableDual(CQstLogInfo* self) {
    self->mUnk20->SetAnimationEnable(self->field_0x24, false);
    self->mUnk20->SetAnimationEnable(self->field_0x28, true);
}

// State machine driver: while the quest-log info is active, dispatch on the
// current state (1/2/4/5) to the per-state animation handlers, then advance
// the layout animation by one frame.
extern "C" void QstLogInfo_StepStateMachine(CQstLogInfo* self) {
    if (self->mUnk30 == 0) return;
    switch (self->field_0x34) {
    case 1: QstLogInfo_AdvanceState1To2(self); break;
    case 2: QstLogInfo_AdvanceState2To3(self); break;
    case 4: QstLogInfo_RewindState4To5(self); break;
    case 5: QstLogInfo_RewindState5To0(self); break;
    }
    self->mUnk20->Animate(0);
}

// Quest-log teardown: release the bdat handle, close the two arc handles,
// delete the layout + tag processor, release the resource accessor and tear
// down the scratch region.
extern "C" void QstLogInfo_TeardownLayout(CQstLogInfo* self) {
    CBdat::getEntry(2);
    closeFileHandle(&self->mFileHandle);
    closeFileHandle(&self->field_0x18);
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
    self->mMemRegion.deleteRegion();
}

// Loads the quest-category arc: maps mField3A through the quest-name table
// (sQstLogTplNames) and reads the common archive file into field_0x18,
// clearing the loaded flag mField38.
void QstLogInfo_LoadCommonArchive(CQstLogInfo* self) {
    self->field_0x18 = CDeviceFile::readCommonArchiveFile(
        KyoshinHeap_GetField44(),
        sQstLogTplNames[SearchThresholdIdx(self->mField3A)], // name-guess: .data[0x78] of 30 quest TPL names ("t05.tpl", "mf00_reg20_cat06.tpl", …)
        self, 0, 0);
    self->mField38 = 0;
}

// Full quest-log refresh: reset the log to the current quest id, then run the
// name / description / pane-color / icon chain of sub-updates.
extern "C" void QstLogInfo_RefreshQuestDisplay(CQstLogInfo* self) {
    QstLogInfo_ClearAllLogPanes(self);
    u8 v = (u8)CtrlRemote_TouchBitByArg(self->mField3A + 0x220);
    u32 idx = SearchThresholdIdx(self->mField3A);
    QstLogInfo_UpdateNameIconPane(self, (const char*)lbl_eu_80573D18[idx], self->mField3A, v);
    QstLogInfo_UpdateClearReasonPane(self, (const char*)lbl_eu_80573D18[idx], self->mField3A);
    QstLogInfo_UpdateExpRewardPane(self, (const char*)self->field_0x3C, self->mField3A);
    QstLogInfo_UpdateBandObjectives(self, (const char*)lbl_eu_80573D18[idx], self->mField3A, v);
    QstLogInfo_UpdateTypeObjectives(self, (const char*)lbl_eu_80573D18[idx], self->mField3A, v);
}

// Name-pane + icon update for one quest entry: BDAT text lookup into the
// layout, root-pane icon lookup, then a 'timg' texture resource select by the
// quest-category column, applied to the icon pane.
extern "C" void QstLogInfo_UpdateNameIconPane(CQstLogInfo* self,
    const char* table, u32 key, u8 v) {
    char* str = BdatGetPtrDirect(table, "title", key);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_tit", str, 0);
    nw4r::lyt::Pane* pane = self->mUnk20->GetRootPane()->FindPaneByName(
        "pic_icon_tit", true);
    u8 kind = BdatGetU8Direct((const void*)table, "qst_genre", key);
    void* res = 0;
    switch (kind) {
    case 0:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C)
                  ->GetResource(0x74696D67, "mf00_reg00_qt01.tpl", 0);
        break;
    case 1:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C)
                  ->GetResource(0x74696D67, "mf00_reg00_qt02.tpl", 0);
        break;
    case 3:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->field_0x1C)
                  ->GetResource(0x74696D67, "mf00_reg00_qt03.tpl", 0);
        break;
    }
    if (res != 0) {
        PaneSetTexPalette(pane, (TPLPalette*)((const char*)res));
        setPaneVisible(pane, 1);
    }
}

// Sets the quest-log "txt_clear" pane from the BDAT table `table`: looks up
// the "purpose" column of row `key` (0x78), pushes it through the UTF-8
// conversion (GetBdatNameByNumId) into the pane, then picks the follow-up text
// ("cleared"/"uncleared"/level-up variants) by quest state and category and
// updates the "txt_clear_reason" pane at 0xab.
extern "C" void QstLogInfo_UpdateClearReasonPane(CQstLogInfo* self,
    const char* table, u32 key) {
    u16 v = BdatGetU16Direct(table, "npc_id", key);
    const char* numName = GetBdatNameByNumId(BdatGetPtrDirect(
        (const char*)::cf::CfBdat::spFldNpcListFileData, "name", v));
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_man", numName, 0);
    int flag = 0;
    if (v >= 0x36C && v <= 0x36E || v == 0x38C || v == 0x391 || v == 0x393) {
        flag = 1;
    }
    char* str;
    if (flag != 0) {
        str = BdatTouchStringCell("MNU_quest", "name", 0x35);
    } else {
        if (BdatGetU8Direct((const void*)table, "qst_genre", key) != 1) {
            u16 v2 = BdatGetU16Direct((const char*)::cf::CfBdat::spFldNpcListFileData,
                                   "rlt_lnd", v);
            str = BdatTouchStringCell("landmarklist", "name", v2);
        } else {
            str = BdatTouchStringCell("MNU_quest", "name", 0x35);
        }
    }
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_map", str, 0);
}

// Sets the quest-log "txt_exp" pane from the BDAT table `table`: looks up
// the "purpose" column of row `key` and writes the result into the layout
// (field_0x2C selects the pane color/state).
extern "C" void QstLogInfo_UpdateExpRewardPane(CQstLogInfo* self, const char* table, u32 key) {
    char* str = BdatGetPtrDirect(table, "purpose", key);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_exp", str, self->field_0x2C);
}

// Quest-log objective/sub-objective update. For each of the four level bands
// (0x6f / 0x79 / 0x83 / 0xfc) it formats a pane name from the band-specific
// format, looks the row up in the BDAT quest table, and binds the found text
// into the layout; the odd/even variant of each format picks the column. The
// kind != 1 path repeats the fixed rows (loop over i) and binds them by name.
extern "C" void QstLogInfo_UpdateBandObjectives(CQstLogInfo* self,
    const char* table, u32 key, u8 v) {
    char bufE8[0x20];
    char bufC8[0x20];
    char bufA8[0x20];
    char buf88[0x20];
    char buf68[0x20];
    char buf48[0x20];
    char buf28[0x20];
    char buf8[0x20];
    if (BdatGetU8Direct((const void*)table, "order_succ_A", key) == 1) {
        char* s = BdatGetPtrDirect((const void*)self->field_0x3C,
                                "info_prog", key);
        LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit01", s,
                      self->field_0x2C);
        setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                          "txt_subtit01", true), 1);
        setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                          "pic_chbx01", true), 1);
        if (v > 1) {
            setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                              "pic_ch01", true), 1);
        }
        u32 idx = 1;
        if (v >= 0x6F) {
            u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
            if (odd) {
                sprintf(bufE8, "type_succ_A2");
            } else {
                sprintf(bufE8, "type_succ_B2");
            }
            u32 val = BdatGetU16Direct(table, bufE8, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(bufE8, "info_succ_A1");
                } else {
                    sprintf(bufE8, "info_succ_B1");
                }
                char* s2 = BdatGetPtrDirect((const void*)self->field_0x3C,
                                         bufE8, key);
                sprintf(bufE8, "txt_subtit%02d", 2);
                LayoutSetTextBoxFmtValue(self->mUnk20, bufE8, s2, self->field_0x2C);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufE8, true), 1);
                sprintf(bufE8, "pic_chbx%02d", 2);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufE8, true), 1);
                if (v >= 0x79) {
                    sprintf(bufE8, "pic_ch%02d", 2);
                    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                      bufE8, true), 1);
                }
                idx = 2;
            }
        }
        if (v >= 0x79) {
            u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
            if (odd) {
                sprintf(bufC8, "type_succ_A3");
            } else {
                sprintf(bufC8, "type_succ_B3");
            }
            u32 val = BdatGetU16Direct(table, bufC8, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(bufC8, "info_succ_A2");
                } else {
                    sprintf(bufC8, "info_succ_B2");
                }
                char* s2 = BdatGetPtrDirect((const void*)self->field_0x3C,
                                         bufC8, key);
                sprintf(bufC8, "txt_subtit%02d", idx + 1);
                LayoutSetTextBoxFmtValue(self->mUnk20, bufC8, s2, self->field_0x2C);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufC8, true), 1);
                sprintf(bufC8, "pic_chbx%02d", idx + 1);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufC8, true), 1);
                if (v >= 0x83) {
                    sprintf(bufC8, "pic_ch%02d", idx + 1);
                    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                      bufC8, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0x83) {
            u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
            if (odd) {
                sprintf(bufA8, "type_succ_A4");
            } else {
                sprintf(bufA8, "type_succ_B4");
            }
            u32 val = BdatGetU16Direct(table, bufA8, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(bufA8, "info_succ_A3");
                } else {
                    sprintf(bufA8, "info_succ_B3");
                }
                char* s2 = BdatGetPtrDirect((const void*)self->field_0x3C,
                                         bufA8, key);
                sprintf(bufA8, "txt_subtit%02d", idx + 1);
                LayoutSetTextBoxFmtValue(self->mUnk20, bufA8, s2, self->field_0x2C);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufA8, true), 1);
                sprintf(bufA8, "pic_chbx%02d", idx + 1);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  bufA8, true), 1);
                if (v >= 0xFC) {
                    sprintf(bufA8, "pic_ch%02d", idx + 1);
                    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                      bufA8, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0xFC) {
            if (BdatGetU8Direct((const void*)table, "no_report", key) == 0) {
                char* s2 = BdatGetPtrDirect((const void*)self->field_0x3C,
                                         "info_report", key);
                sprintf(buf88, "txt_subtit%02d", idx + 1);
                LayoutSetTextBoxFmtValue(self->mUnk20, buf88, s2, self->field_0x2C);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf88, true), 1);
                sprintf(buf88, "pic_chbx%02d", idx + 1);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf88, true), 1);
                if (v >= 0xFE) {
                    sprintf(buf88, "pic_ch%02d", idx + 1);
                    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                      buf88, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0xFE) {
            char* s2 = (v & 1) != 0
                ? BdatGetPtrDirect((const void*)self->field_0x3C,
                                "info_end_B", key)
                : BdatGetPtrDirect((const void*)self->field_0x3C,
                                "info_end_A", key);
            sprintf(buf68, "txt_subtit%02d", idx + 1);
            LayoutSetTextBoxFmtValue(self->mUnk20, buf68, s2, self->field_0x2C);
            setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                              buf68, true), 1);
        }
    } else {
        char* s = BdatGetPtrDirect((const void*)self->field_0x3C,
                                "info_prog", key);
        LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit01", s,
                      self->field_0x2C);
        setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                          "txt_subtit01", true), 1);
        setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                          "pic_chbx01", true), 1);
        u32 odd = ((s32)v >= 0xFC ? v - 1 : v) & 1;
        void* queryBuf = UIWin_BuildFlagBuf(key);
        u32 notOdd = !odd;
        if (UIWin_QueryItemAvail((CItemQuery*)queryBuf, notOdd, 0) != 0) {
            setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                              "pic_ch01", true), 1);
        }
        u32 idx = 1;
        for (u8 i = 0; i < 3; i++) {
            if (odd) {
                sprintf(buf48, "type_succ_A%d", i + 2);
            } else {
                sprintf(buf48, "type_succ_B%d", i + 2);
            }
            u32 val = BdatGetU16Direct(table, buf48, key);
            if ((u16)val != 0) {
                if (odd) {
                    sprintf(buf48, "info_succ_A%d", i + 1);
                } else {
                    sprintf(buf48, "info_succ_B%d", i + 1);
                }
                char* s2 = BdatGetPtrDirect((const void*)self->field_0x3C,
                                         buf48, key);
                sprintf(buf48, "txt_subtit%02d", idx + 1);
                LayoutSetTextBoxFmtValue(self->mUnk20, buf48, s2, self->field_0x2C);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf48, true), 1);
                sprintf(buf48, "pic_chbx%02d", idx + 1);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf48, true), 1);
                if (i == 3) {
                    if (v >= 0xFC) {
                        sprintf(buf48, "pic_ch%02d", idx + 1);
                        setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                          buf48, true), 1);
                        idx++;
                    }
                } else {
                    if (UIWin_QueryItemAvail((CItemQuery*)queryBuf, notOdd, i + 1) != 0) {
                        sprintf(buf48, "pic_ch%02d", idx + 1);
                        setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                          buf48, true), 1);
                    }
                    idx++;
                }
            }
        }
        if (v >= 0xFC) {
            if (BdatGetU8Direct((const void*)table, "no_report", key) == 0) {
                char* s2 = BdatGetPtrDirect((const void*)self->field_0x3C,
                                         "info_report", key);
                sprintf(buf28, "txt_subtit%02d", idx + 1);
                LayoutSetTextBoxFmtValue(self->mUnk20, buf28, s2, self->field_0x2C);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf28, true), 1);
                sprintf(buf28, "pic_chbx%02d", idx + 1);
                setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                  buf28, true), 1);
                if (v >= 0xFE) {
                    sprintf(buf28, "pic_ch%02d", idx + 1);
                    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                                      buf28, true), 1);
                }
                idx++;
            }
        }
        if (v >= 0xFE) {
            char* s2 = (v & 1) != 0
                ? BdatGetPtrDirect((const void*)self->field_0x3C,
                                "info_end_B", key)
                : BdatGetPtrDirect((const void*)self->field_0x3C,
                                "info_end_A", key);
            sprintf(buf8, "txt_subtit%02d", idx + 1);
            LayoutSetTextBoxFmtValue(self->mUnk20, buf8, s2, self->field_0x2C);
            setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                              buf8, true), 1);
        }
    }
}

// Quest-log objective/sub-objective update with per-band pane binding (see
// QstLogInfo_UpdateBandObjectives for the v >= 0xFE band). The two loops are identical except
// for their pane-name formats and buffers; the switch maps the BDAT quest
// type (v1) to the table that supplies the objective text.
extern "C" void QstLogInfo_UpdateTypeObjectives(CQstLogInfo* self,
    const char* table, u32 key, u8 v) {
    char buf8[0x20];
    char buf28[0x20];
    char buf48[0x20];
    char buf68[0x20];
    char* str = 0;
    if (v >= 0xFE) {
        u8 kind = BdatGetU8Direct((const void*)table, "reward_mes", key);
        if (kind == 1) {
            LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item01",
                          BdatTouchStringCell("MNU_quest",
                                        "name", 0x33), 0);
            setLayoutTextBoxNumber(self->mUnk20, "txt_gold00", 0);
            setLayoutTextBoxNumber(self->mUnk20, "txt_exp01", 0);
            return;
        }
        u16 a = BdatGetU16Direct(table, "reward_exp", key);
        u16 b = BdatGetU16Direct(table, "reward_money", key);
        setLayoutTextBoxNumber(self->mUnk20, "txt_gold00", b * 10);
        setLayoutTextBoxNumber(self->mUnk20, "txt_exp01", a * 10);
        u32 odd = v & 1;
        for (u8 i = 0; i < 3; i++) {
            if (odd) {
                sprintf(buf48, "reward_B%d", i + 1);
            } else {
                sprintf(buf48, "reward_A%d", i + 1);
            }
            u32 val = BdatGetU16Direct(table, buf48, key);
            if ((u16)val != 0) {
                u16 v1 = BdatGetItemType((u16)val);
                u16 v2 = BdatGetItemId((u16)val);
                switch ((u16)v1) {
                case 0:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmWpnListFileData,
                                        "name", (u16)v2);
                    break;
                case 1:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spBtlSkillListFileData,
                                        "name", (u16)v2);
                    break;
                case 2:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmEquipListFileData,
                                        "name", (u16)v2);
                    break;
                case 3:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmCrystalListFileData,
                                        "name", (u16)v2);
                    break;
                case 4:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmCollectListFileData,
                                        "name", (u16)v2);
                    break;
                case 5:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmMaterialListFileData,
                                        "name", (u16)v2);
                    break;
                case 6:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmValuableListFileData,
                                        "name", (u16)v2);
                    break;
                case 7:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmArtsListFileData,
                                        "name", (u16)v2);
                    break;
                default:
                    break;
                }
                sprintf(buf48, "txt_item%02d", i + 1);
                if ((u16)v1 == 3) {
                    u8 x = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData,
                                         "rankType", (u16)val);
                    const char* itemTable = "MNU_item";
                    sprintf(buf68, "%s%s", str,
                            BdatTouchStringCell(itemTable,
                                          "name",
                                          0x1e - (x - 1)));
                } else {
                    sprintf(buf68, "%s", str);
                }
                LayoutSetTextBoxText(self->mUnk20, buf48, buf68, 0);
            } else if (i == 0) {
                str = BdatTouchStringCell("MNU_quest",
                                    "name", 0x33);
                LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item01", str, 0);
            }
        }
    } else {
        u8 kind = BdatGetU8Direct((const void*)table, "reward_mes", key);
        if (kind == 3) {
            LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item01",
                          BdatTouchStringCell("MNU_quest",
                                        "name", 0x32), 0);
            u16 a = BdatGetU16Direct(table, "reward_exp", key);
            u16 b = BdatGetU16Direct(table, "reward_money", key);
            setLayoutTextBoxNumber(self->mUnk20, "txt_gold00", b * 10);
            setLayoutTextBoxNumber(self->mUnk20, "txt_exp01", a * 10);
            return;
        }
        if (kind == 1) {
            LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item01",
                          BdatTouchStringCell("MNU_quest",
                                        "name", 0x33), 0);
            setLayoutTextBoxNumber(self->mUnk20, "txt_gold00", 0);
            setLayoutTextBoxNumber(self->mUnk20, "txt_exp01", 0);
            return;
        }
        u16 a = BdatGetU16Direct(table, "reward_exp", key);
        u16 b = BdatGetU16Direct(table, "reward_money", key);
        setLayoutTextBoxNumber(self->mUnk20, "txt_gold00", b * 10);
        setLayoutTextBoxNumber(self->mUnk20, "txt_exp01", a * 10);
        u32 odd = v & 1;
        for (u8 i = 0; i < 3; i++) {
            if (v == 1) {
                sprintf(buf8, "reward_A%d", i + 1);
            } else if (v >= 0xFC) {
                if (odd) {
                    sprintf(buf8, "reward_B%d", i + 1);
                } else {
                    sprintf(buf8, "reward_A%d", i + 1);
                }
            } else {
                if (odd) {
                    sprintf(buf8, "reward_A%d", i + 1);
                } else {
                    sprintf(buf8, "reward_B%d", i + 1);
                }
            }
            u32 val = BdatGetU16Direct(table, buf8, key);
            if ((u16)val != 0) {
                u16 v1 = BdatGetItemType((u16)val);
                u16 v2 = BdatGetItemId((u16)val);
                switch ((u16)v1) {
                case 0:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmWpnListFileData,
                                        "name", (u16)v2);
                    break;
                case 1:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spBtlSkillListFileData,
                                        "name", (u16)v2);
                    break;
                case 2:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmEquipListFileData,
                                        "name", (u16)v2);
                    break;
                case 3:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmCrystalListFileData,
                                        "name", (u16)v2);
                    break;
                case 4:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmCollectListFileData,
                                        "name", (u16)v2);
                    break;
                case 5:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmMaterialListFileData,
                                        "name", (u16)v2);
                    break;
                case 6:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmValuableListFileData,
                                        "name", (u16)v2);
                    break;
                case 7:
                    str = BdatGetPtrDirect((const void*)::cf::CfBdat::spItmArtsListFileData,
                                        "name", (u16)v2);
                    break;
                default:
                    break;
                }
                sprintf(buf8, "txt_item%02d", i + 1);
                if ((u16)v1 == 3) {
                    u8 x = BdatGetU8Direct((const void*)::cf::CfBdat::spItmItemListFileData,
                                         "rankType", (u16)val);
                    const char* itemTable = "MNU_item";
                    sprintf(buf28, "%s%s", str,
                            BdatTouchStringCell(itemTable,
                                          "name",
                                          0x1e - (x - 1)));
                } else {
                    sprintf(buf28, "%s", str);
                }
                LayoutSetTextBoxText(self->mUnk20, buf8, buf28, 0);
            } else if (i == 0) {
                str = BdatTouchStringCell("MNU_quest",
                                    "name", 0x33);
                LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item01", str, 0);
            }
        }
    }
}

// Quest-log reset: clears every quest-name / objective / status pane back to
// the empty string (pool+0x2a2) and resets the state panes' visibility.
extern "C" void QstLogInfo_ClearAllLogPanes(CQstLogInfo* self) {
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_tit", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_man", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_map", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_exp", "", 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_icon_tit", true), 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit01", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit02", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit03", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit04", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit05", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_subtit06", "", 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_chbx01", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_chbx02", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_chbx03", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_chbx04", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_chbx05", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_chbx06", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_ch01", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_ch02", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_ch03", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_ch04", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_ch05", true), 0);
    setPaneVisible(self->mUnk20->GetRootPane()->FindPaneByName(
                      "pic_ch06", true), 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item01", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item02", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_item03", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_gold00", "", 0);
    LayoutSetTextBoxFmtValue(self->mUnk20, "txt_exp01", "", 0);
}

// File-load completion callback for the two quest-log arc handles. The
// quest-log arc (mFileHandle) builds the layout; the common archive
// (field_0x18) primes the quest-category BDAT table (field_0x3C) and refreshes
// the whole log display.
bool CQstLogInfo::OnFileEvent(CEventFile* event) {
    if (mFileHandle == event->mFileHandle) {
        mMemRegion.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x10000,
                                "CQstLogInfo", 0);
        Class_8045F858 guard(&mMemRegion);
        void* fileData = mFileHandle->getData();
        mtl::MemManager::setMemInitFlag(false);
        void* tagMem = mtl::MemManager::allocate(
            0x858, CLibLayout::getAllocHandle());
        if (tagMem != 0) {
            tagMem = __ct__CTagProcessor(tagMem);
        }
        field_0x2C = (u32)tagMem;
        field_0x1C = (u32)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)field_0x1C)->Attach(
            fileData, "arc");
        buildLayout(&mUnk20, (nw4r::lyt::ArcResourceAccessor*)field_0x1C,
                      "mf30_qst_dtl.brlyt");
        bindLayoutAnimTransform(mUnk20, &field_0x24,
                      (nw4r::lyt::ArcResourceAccessor*)field_0x1C,
                      "mf30_qst_dtl_in.brlan");
        bindLayoutAnimTransform(mUnk20, &field_0x28,
                      (nw4r::lyt::ArcResourceAccessor*)field_0x1C,
                      "mf30_qst_dtl_info_in.brlan");

        nw4r::lyt::Pane* rootPane = mUnk20->GetRootPane();
        void* font = CDeviceFont::getFontInfo(1, mUnk20);
        ApplyFontToTxtPanes(rootPane, ((IDeviceFontInfo*)font)->getFont());

        char* questName = (char*)CUICfManager_getPackedFont9C();
        if (questName != 0) {
            setLayoutTextBoxFont(mUnk20, "txt_gold00", (u32)questName);
            setLayoutTextBoxFont(mUnk20, "txt_gold01", (u32)questName);
            setLayoutTextBoxFont(mUnk20, "txt_exp01", (u32)questName);
        }
        LayoutSetTextBoxFmtValue(mUnk20, "txt_gold01",
                      BdatTouchStringCell("MNU_item",
                                    "name", 3), 0);
        QstLogInfo_SwapAnimEnablePair(this);
        mUnk20->Animate(0);
        char buf[0x18];
        for (u8 i = 1; i <= 6; i++) {
            sprintf(buf, "txt_subtit%02d", i);
            nw4r::lyt::Pane* pane = mUnk20->GetRootPane()->FindPaneByName(
                buf, true);
            static_cast<nw4r::lyt::TextBox*>(pane)->SetTagProcessor(
                (nw4r::ut::WideTagProcessor*)field_0x2C);
        }
        nw4r::lyt::Pane* pane = mUnk20->GetRootPane()->FindPaneByName(
            "txt_exp", true);
        static_cast<nw4r::lyt::TextBox*>(pane)->SetTagProcessor(
            (nw4r::ut::WideTagProcessor*)field_0x2C);
        mField38 = 1;
        mUnk30 = 1;
        mFileHandle = 0;
        mMemRegion.validateHeap();
        return 1;
    } else if (field_0x18 == event->mFileHandle) {
        void* fileData = field_0x18->getData();
        CBdat::setBdatEntry(2, fileData);
        Bdat_GetTable_AA34();
        field_0x3C = (u32)getFP(sQstLogBdatColumnNames[SearchThresholdIdx(mField3A)]); // name-guess: .data[0x70] of BDAT column strings ("d%s", "eva_rate", "pc_arts", …)
        QstLogInfo_RefreshQuestDisplay(this);
        mField38 = 1;
        field_0x18 = 0;
        return 1;
    }
    return 0;
}

// retail: if (mUnk30) tail drawLayout(mUnk20, drawInfo passthrough, 0, 1)
void QstLogInfo_DrawLayoutTail(CQstLogInfo* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mUnk30 == 0) return;
    drawLayout(self->mUnk20, drawInfo, 0, 1);
}
