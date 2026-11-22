#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"
#include "kyoshin/cf/IFlagEvent.hpp"
#include "kyoshin/cf/CtrlRemoteApi.hpp" // unified with CChain.hpp/CSuddenCommu.hpp/CPassiveSkill.hpp

namespace cf {

// Retail layout: CHelp at +0x00 (vptr at +0x08), private IFlagEvent base at
// +0x0C (absent from the RTTI base list, so not public), then the payload.
// MWCC emits __vt__Q22cf14CHelp_LandMark (CHelp slots, then the dtor and the
// FlagEvent1 entry) and the two @12@ adjustor thunks of the IFlagEvent table.
class CHelp_LandMark : public CHelp, IFlagEvent {
public:
    CHelp_LandMark(u32 owner, u32 param, s32 landmarkId);
    virtual ~CHelp_LandMark();

    virtual void CHelp_UnkVirtualFunc1();
    virtual UNKWORD f10();
    virtual void FlagEvent1(int eventId, int arg2, int arg3);

    s32 mLandmarkId; // +0x10 name-guess: offset of the landmark flag event id from 0x20C8
    s32 mTimer;      // +0x14
    u32 mSavedFlags; // +0x18
};

} // namespace cf
