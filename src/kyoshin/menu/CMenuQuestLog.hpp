#pragma once

#include <types.h>

// Local IScnRender declaration with NO destructor and no inline bodies (see
// CMenuTutorial.hpp). The shared monolib/scn/IScnRender.hpp defines
// `virtual ~IScnRender(){}` inline; odr-using that inline dtor from this TU
// makes MWCC emit a standalone __dt__10IScnRenderFv strong copy (0x40) here
// and blows the split budget (retail keeps that copy only in CTaskGame.o).
// Omitting the dtor gives IScnRender an implicitly trivial one while keeping
// the vtable layout identical (slot order: dtor slot, func_80043F20).
class IScnRender {
public:
    virtual void func_80043F20();
};

// Minimal CScn view exposing only the render-callback registration used by
// this TU (retail mangled member symbols; defined in the monolib CScn TU).
// A local body-less declaration avoids pulling monolib/scn.hpp (see the
// IScnRender note above).
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

#include "monolib/work/CProcess.hpp"

#include "kyoshin/CBgTex.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CQstLogList.hpp"
#include "kyoshin/CQstLogInfo.hpp"

/* Quest-log menu process.
 *
 * MI layout mirrors CMenuTutorialList: CProcess base at 0x00 (primary vptr
 * lands at +0x10), compiler PMF callback slots at 0x3C/0x48, IScnRender
 * render-callback subobject at +0x58, owning scene pointer at +0x5C, then the
 * embedded widgets CBgTex (0x60) / CTitleAHelp (0x80) / CQstLogList (0xB8) /
 * CQstLogInfo (0x2240) and the state bytes at 0x2280/0x2284. */
class CMenuQuestLog : public CProcess {
public:
    virtual ~CMenuQuestLog();
    virtual void Init();
    virtual void Term();
    void Move();
    void cbRenderBefore();

    u32 ptmf0[3];             // 0x3C-0x47: null PMF callback slot group 1
    u32 ptmf1[3];             // 0x48-0x53: null PMF callback slot group 2
    u8 mField54;              // 0x54: closing flag (set on close request)
    u8 mField55;              // 0x55
    u8 _pad56[2];             // 0x56-0x57
    IScnRender mIScnRender;   // 0x58-0x5B: render-callback subobject (vptr)
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
extern "C" void func_8011CDF4(unsigned char* self);
extern "C" void func_8011CD6C(CMenuQuestLog* self);
extern "C" void func_8011CE44(CMenuQuestLog* self);
extern "C" void func_8011D03C(unsigned char* self);
extern "C" void func_8011D08C(unsigned char* self);
extern "C" void func_8011D0FC(CMenuQuestLog* self);
extern "C" void func_8011D158(CMenuQuestLog* self);
extern "C" void func_8011D1A8(CMenuQuestLog* self);
extern "C" void func_8011D22C(CMenuQuestLog* self);
extern "C" void func_8011D298(CMenuQuestLog* self);

class UnkClass_8011C974 {
public:
    UnkClass_8011C974();

    // TODO: add fields
};

// Quest-log sub-object helpers. Retail uses the unmangled func_ names, so
// declare them with C linkage (a C++ member call would re-mangle the reloc).
extern "C" void func_802282F8(CQstLogList* self);
extern "C" void func_80227D78(CQstLogList* self, int arg);
extern "C" int func_80227CD4(CQstLogList* self);
extern "C" void func_80228280(CQstLogList* self, int arg);
extern "C" void func_80227DE8(CQstLogList* self);
extern "C" void func_80227EC8(CQstLogList* self);
extern "C" void func_80227FC0(CQstLogList* self);
extern "C" void func_8022807C(CQstLogList* self);
extern "C" void func_80228164(CQstLogList* self);
extern "C" void func_801C414C(CTitleAHelp* self);
extern "C" bool func_802296D0(CQstLogInfo* self);
extern "C" bool func_802296D8(CQstLogInfo* self);

// cf::CfPadData view for getCfPadData: pressed-button flags at +0x04 and
// short-press turbo flags at +0x104 (cf::CfPadData itself is incomplete).
struct CQuestLogPadData {
    u8 _00[0x4];
    u32 mPressedButtonFlags;    // +0x04 CPad::mPressedButtonFlags
    u8 _08[0x104 - 0x8];
    u32 mTurboPressButtonFlags; // +0x104
};
extern "C" void func_802296E0(CQstLogInfo* self);
extern "C" void func_80227CDC(CQstLogList* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void func_801C416C(CTitleAHelp* self);
extern "C" void func_801C4198(CTitleAHelp* self);
extern "C" void func_80229724(CQstLogInfo* self);

// Init() imports (retail unmangled names).
extern char lbl_eu_804FE518[];   // menu text string pool (split1 .rodata)
extern "C" bool func_801C3C14(CBgTex* self);
extern "C" void CTitleAHelp_load(CTitleAHelp* self);
extern "C" void func_80227A60(CQstLogList* self);
extern "C" void func_802294C0(CQstLogInfo* self);
extern "C" void __dt__6CBgTexFv(CBgTex* self, int flags);
extern "C" void __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" void __dt__11CQstLogListFv(CQstLogList* self, int flags);
extern "C" void __dt__11CQstLogInfoFv(CQstLogInfo* self, int flags);

// Widget teardown helpers called by Term (retail unmangled names).
extern "C" void func_801C3D9C(CBgTex* self);
extern "C" void func_801C40A0(CTitleAHelp* self);
extern "C" void func_80227BD8(CQstLogList* self);
extern "C" void func_80229620(CQstLogInfo* self);

// Term tail helpers.
extern "C" u8 func_8013B980();
extern "C" void func_80135550();
extern "C" u8 code80135FDC_getByte_64080();

// Retail constructor symbol (unmangled global, 2 args after `this`). The
// factory (func_8011CCE0) calls it out-of-line; the stub definition in the
// .cpp inherits C linkage from this declaration.
extern "C" CMenuQuestLog* __ct__CMenuQuestLog(CMenuQuestLog* _this, CProcess* parent, u32 arg2);

// Singleton pointer for the quest-log menu (.sbss).
extern CMenuQuestLog* lbl_eu_80663FC0;

// Global mode bitfield (bit 0x200000 gates the quest-log Move/render paths);
// declared in kyoshin/CTaskGame.hpp, which this TU avoids including (see the
// IScnRender note at the top).
extern u32 lbl_eu_80663E28;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" bool isIdle__11CTitleAHelpFv(void*);
extern "C" bool func_80227CCC(void*);
extern "C" unsigned int func_80228394(void*);
extern "C" void func_80229768(void*, unsigned short);
extern "C" void func_80229510(void*);

// Widget/base helpers with retail-unmangled names (US retail strips these
// member manglings) - C linkage so calls bind the literal retail symbols.
// Queries return int so callers compare with cmpwi directly.
extern "C" int func_801C3E34(CBgTex* self);
extern "C" int func_801C4114(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" int func_80227C70(CQstLogList* self);
extern "C" void func_801C3D54(CBgTex* self);
extern "C" void func_801C3FF0(CTitleAHelp* self);
extern "C" void func_801C3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_801C4080(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_80227AC4(CQstLogList* self);
extern "C" void func_80227B6C(CQstLogList* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_80229600(CQstLogInfo* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void func_80229570(CQstLogInfo* self);

// Embedded-widget ctors called by the member constructor (US retail strips
// these manglings; C-ABI wrappers for the opaque embedded storages).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern "C" void __ct__CBgTex(CBgTex* self, u8 arg);
extern "C" void __ct__CTitleAHelp(CTitleAHelp* self, char* name, u8 mode);
extern "C" void __ct__CQstLogList(CQstLogList* self, u16 arg);
extern "C" void __ct__CQstLogInfo(CQstLogInfo* self);

// CfGameManager helpers (retail pre-mangled names).
extern "C" void func_8008294C__Q22cf13CfGameManagerFv(u8 enable);
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
// Save/skip system busy queries (retail unmangled).
extern "C" int func_800FEDF8();
extern "C" void func_800FF914();
// Frame counter post-increment helper (code_80135FDC.cpp).
extern "C" void code80135FDC_postIncByte_64080();

// vtable / PMF / shared data (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];
extern char lbl_eu_8052BF70[];    // CProcess primary vtable (pre-PMF copy)
extern char lbl_eu_8052CC38[];    // CMenuQuestLog composite vtable
