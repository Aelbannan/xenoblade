#pragma once

#include <types.h>
#include "kyoshin/code_80135FDC_State.hpp"

// IScnRender without a virtual destructor (CMenuQuestLog.cpp defines
// ISCN_RENDER_NO_DTOR): odr-using the inline `virtual ~IScnRender(){}` from this
// TU makes MWCC emit a standalone __dt__10IScnRenderFv strong copy (0x40) here
// and blows the split budget (retail keeps that copy only in CTaskGame.o).
#include "monolib/scn/IScnRender.hpp"

#include "monolib/scn/CScn.hpp"

#include "monolib/work/CProcess.hpp"
#include "kyoshin/IUICf.hpp"

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CQstLogList.hpp"
#include "kyoshin/CQstLogInfo.hpp"

/* Quest-log menu. IUICf task base (0x00-0x57), the IScnRender render-callback
 * subobject at +0x58, owning scene pointer at +0x5C, then the embedded widgets
 * CBgTex (0x60) / CTitleAHelp (0x80) / CQstLogList (0xB8) / CQstLogInfo
 * (0x2240) and the state byte at 0x2284. */
class CMenuQuestLog : public IUICf, public IScnRender {
public:
    CMenuQuestLog(CProcess* parent, u32 arg2);
    virtual ~CMenuQuestLog();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    CProcess* mScene;         // 0x5C: owning scene/parent process
    CBgTex mBgTex;            // 0x60-0x7F: background layout widget
    CTitleAHelp mTitleAHelp;  // 0x80-0xB7: title/help bar
    CQstLogList mQstLogList;  // 0xB8-0x223F: quest log list widget
    CQstLogInfo mQstLogInfo;  // 0x2240-0x227F: quest log info widget
    u32 field_2280;           // 0x2280 (ctor arg)
    u8 mState;                // 0x2284: state machine byte (Move dispatch)
};

// Move() state-machine handlers (states 0-9). US retail strips the member
// mangling, so declare them with C linkage so calls bind the literal retail
// symbols; definitions live in CMenuQuestLog.cpp.
extern "C" void gateQuestLogList(CMenuQuestLog* self);
extern "C" void openQuestLogMenu(CMenuQuestLog* self);
extern "C" void handleQuestLogInput(CMenuQuestLog* self);
extern "C" void flagQuestLogReady(CMenuQuestLog* self);
extern "C" void selectQuestLogEntry(CMenuQuestLog* self);
extern "C" void startQuestLogInfo(CMenuQuestLog* self);
extern "C" void finishQuestLogInfo(CMenuQuestLog* self);
extern "C" void closeQuestLogMenu(CMenuQuestLog* self);
extern "C" void resortQuestLogList(CMenuQuestLog* self);
extern "C" void reopenQuestLogList(CMenuQuestLog* self);

class UnkClass_8011C974 {
public:
    UnkClass_8011C974();

    // TODO: add fields
};

// Quest-log sub-object helpers. Retail uses the unmangled func_ names, so
// declare them with C linkage (a C++ member call would re-mangle the reloc).

// cf::CfPadData view for getCfPadData: pressed-button flags at +0x04 and
// short-press turbo flags at +0x104 (cf::CfPadData itself is incomplete).
struct CQuestLogPadData {
    u8 _00[0x4];
    u32 mPressedButtonFlags;    // +0x04 CPad::mPressedButtonFlags
    u8 _08[0x104 - 0x8];
    u32 mTurboPressButtonFlags; // +0x104
};
extern "C" void QstLogInfo_Start(CQstLogInfo* self);
extern "C" void QstLogInfo_Resume(CQstLogInfo* self);

// Init() imports (retail unmangled names).
extern "C" const char lbl_eu_804FE518[];   // menu text string pool (split1 .rodata)
extern "C" bool BgTex_Acquire_3C14(CBgTex* self);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
// Embedded widgets are torn down through their owning helpers above.

// Widget teardown helpers called by Term (retail unmangled names).
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" void QstLogInfo_TeardownLayout(CQstLogInfo* self);

// Term tail helpers.
extern "C" void CUICfManager_setTimeout30();

// Singleton pointer for the quest-log menu (.sbss).
extern CMenuQuestLog* spQuestLogMenuInstance[2];

// Global mode bitfield (bit 0x200000 gates the quest-log Move/render paths);
// declared in kyoshin/CTaskGame.hpp, which this TU avoids including (see the
// IScnRender note at the top).
extern u32 gmPauseFlags;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// Widget/base helpers with retail-unmangled names (US retail strips these
// member manglings) - C linkage so calls bind the literal retail symbols.
// Queries return int so callers compare with cmpwi directly.
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void QstLogInfo_StepStateMachine(CQstLogInfo* self);

// CfGameManager helpers (retail pre-mangled names).
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
// Save/skip system busy queries (retail unmangled).
// Declared by kyoshin/CMainMenu.hpp.
extern "C" void ArtsInfo_SetReadyFlag();
// Frame counter post-increment helper (code_80135FDC.cpp).
