#pragma once

#include <types.h>

#include "monolib/work/CProcess.hpp"
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CQstLogList.hpp"
#include "kyoshin/CQstLogInfo.hpp"

class CMenuQuestLog {
public:
    CMenuQuestLog();
    virtual ~CMenuQuestLog();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x00: vtable + unmodelled region up to the first sub-object.
    u8 _00[0x7C];                    // 0x04 (after vptr) .. 0x80
    CTitleAHelp mTitleAHelp;         // 0x80 (sizeof 0x38)
    CQstLogList mQstLogList;         // 0xB8 (sizeof 0x2188) .. 0x2240
    CQstLogInfo mQstLogInfo;         // 0x2240 (sizeof 0x40) .. 0x2280
    u8 _2280[4];                     // 0x2280 .. 0x2284
    u8 mState;                       // 0x2284
};

class UnkClass_8011C974 {
public:
    UnkClass_8011C974();

    // TODO: add fields
};

// Quest-log sub-object helpers. Retail uses the unmangled func_ names, so
// declare them with C linkage (a C++ member call would re-mangle the reloc).
extern "C" bool func_802296D0(CQstLogInfo* self);
extern "C" bool func_802296D8(CQstLogInfo* self);
extern "C" void func_802296E0(CQstLogInfo* self);
extern "C" void func_80227CDC(CQstLogList* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 mode);
extern "C" void func_801C416C(CTitleAHelp* self);

// Retail constructor symbol (unmangled global, 2 args after `this`). The
// factory (func_8011CCE0) calls it out-of-line; the stub definition in the
// .cpp inherits C linkage from this declaration.
extern "C" CMenuQuestLog* __ct__CMenuQuestLog(CMenuQuestLog* _this, CProcess* parent, u32 arg2);

// Singleton pointer for the quest-log menu (.sbss).
extern CMenuQuestLog* lbl_eu_80663FC0;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" bool isIdle__11CTitleAHelpFv(void*);
extern "C" bool func_80227CCC(void*);
extern "C" unsigned int func_80228394(void*);
extern "C" void func_80229768(void*, unsigned short);
extern "C" void func_80229510(void*);
