#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// SDA globals (retail names; defined in common/sbss splits).
extern u32 lbl_eu_80664184;
extern s32 lbl_eu_80664460;
extern nw4r::lyt::ArcResourceAccessor* lbl_eu_80664464;

// Background layout texture widget; receives archive completion through IWorkEvent.
struct CBgTex : public IWorkEvent {

    UnkClass_8045F564 mMemRegion; // 0x4
    CFileHandle* mFileHandle; // 0x14
    nw4r::lyt::Layout* mLayout; // 0x18
    bool mLayoutReady; // 0x1C
    u8 mLoaded; // 0x1D
    u8 mPtmMode; // 0x1E

    CBgTex(u8 arg);
    void BgTex_SetupRegion_3A24();
    bool BgTex_Acquire_3C14();
    void BgTex_Tick_3D54();
    void BgTex_Draw_3D7C(nw4r::lyt::DrawInfo* drawInfo);
    void BgTex_Release_3D9C();
    u8 BgTex_IsLoaded_3E34();
    void BgTex_MarkLoaded_3E3C();
    virtual ~CBgTex();
    virtual bool OnFileEvent(CEventFile* pEventFile);
};
