#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include <nw4r/lyt.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Shared data pools (retail names; global scope - no C++ mangling).
// const float: MWCC hoists the SDA21 load into the prologue (pos 2) like
// retail; a plain extern float schedules the load late (MWCC_CASES:8787).
extern const float lbl_eu_80668590;   // animation-completion sentinel (.sdata2)
extern char lbl_eu_8050A0B4[];  // quest-log string pool (split1 .rodata)
extern char* lbl_eu_80536320[];  // quest-name table (30 entries, split1 .data)

struct CFileHandle;
struct CEventFile;

// C-linkage imports used by QstLogInfo_LoadCommonArchive (retail names are unmangled).
extern "C" u32 KyoshinHeap_GetField44();
// Random / name-table helpers (retail unmangled).
extern "C" u32 CtrlRemote_TouchBitByArg(u32);
extern u32 lbl_eu_80573D18[];  // quest-name table (split1 .rodata)
// Pane helpers (retail unmangled).

// BDAT text / name helpers (retail unmangled).
// UIWin_BuildFlagBuf is owned by kyoshin/CUIWindowManager.hpp.

// Font-info lookup (retail pre-mangled name).
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
// Layout-build helpers (retail unmangled).
extern "C" void* __ct__CTagProcessor(void*);
// BDAT manager (C-linkage free function; the CBdat:: form would mangle).
extern "C" void* Bdat_GetTable_AA34();

// BDAT table handles used by the quest-log sub-updates (.sbss).
extern u32 lbl_eu_806640D8;
extern u32 lbl_eu_806640EC;
extern u32 lbl_eu_80664104;
// Quest-category name table indexed by SearchThresholdIdx (split1 .data).
extern char* lbl_eu_80536398[];

// View into a nw4r::lyt::Pane's trailing user-data region: OnFileEvent stores
// the tag-processor pointer (field_0x2C) into each bound pane at +0xF8.
struct CQstLogInfoPaneUser {
    u8 pad[0xF8];
    u32 field_F8;
};

// Mirror of the font object returned by CDeviceFont::getFontInfo: vtable
// slot 9 (+0x24) yields the u32 bound into the layout's font pane. Never
// instantiated (all pure virtual) so no vtable is emitted.
struct CQstLogInfoFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};

class CQstLogInfo;

// Same-TU helpers called by QstLogInfo_StepStateMachine etc. (retail names unmangled).
// Retail callers emit real `bl` branches. extern "C" binds the call relocs to
// the unmangled retail symbol names.
extern "C" void QstLogInfo_AdvanceState1To2(CQstLogInfo* self);
extern "C" void QstLogInfo_AdvanceState2To3(CQstLogInfo* self);
extern "C" void QstLogInfo_RewindState4To5(CQstLogInfo* self);
extern "C" void QstLogInfo_RewindState5To0(CQstLogInfo* self);
extern "C" void QstLogInfo_SwapAnimEnablePair(CQstLogInfo* self);
extern "C" void QstLogInfo_SwapAnimEnableDual(CQstLogInfo* self);
// Quest-log sub-widget updates (called from QstLogInfo_RefreshQuestDisplay).
extern "C" void QstLogInfo_UpdateNameIconPane(CQstLogInfo* self,
    const char* table, u32 key, u8 v);
extern "C" void QstLogInfo_UpdateClearReasonPane(CQstLogInfo* self,
    const char* table, u32 key);
extern "C" void QstLogInfo_UpdateExpRewardPane(CQstLogInfo* self,
    const char* table, u32 key);
extern "C" void QstLogInfo_UpdateBandObjectives(CQstLogInfo* self,
    const char* table, u32 key, u8 v);
extern "C" void QstLogInfo_UpdateTypeObjectives(CQstLogInfo* self,
    const char* table, u32 key, u8 v);
extern "C" void QstLogInfo_ClearAllLogPanes(CQstLogInfo* self);

/* Quest-log info panel; receives its arcs through IWorkEvent::OnFileEvent. */
class CQstLogInfo : public IWorkEvent {
public:
    CQstLogInfo();
    virtual ~CQstLogInfo();
    virtual bool OnFileEvent(CEventFile* event);

    u8 QstLogInfo_GetLoadedMark();
    u8 QstLogInfo_GetReadyMark();
    void QstLogInfo_SetQuestIdSlot(u16 val);

    UnkClass_8045F564 mMemRegion;             // 0x04 - scratch region (0x10 bytes)
    CFileHandle* mFileHandle;                 // 0x14 - loaded quest-log arc handle
    CFileHandle* field_0x18;                  // 0x18 - common-archive handle
    u32 field_0x1C;                           // 0x1C - ArcResourceAccessor* (raw)
    nw4r::lyt::Layout* mUnk20;                // 0x20
    nw4r::lyt::AnimTransform* field_0x24;     // 0x24
    nw4r::lyt::AnimTransform* field_0x28;     // 0x28
    u32 field_0x2C;                           // 0x2C - CTagProcessor* (raw)
    u8 mUnk30;                                // 0x30
    s32 field_0x34;                           // 0x34 - state (signed compares)
    u8 mField38;                              // 0x38
    u8 mField39;                              // 0x39
    u16 mField3A;                             // 0x3A
    u32 field_0x3C;                           // 0x3C
};
