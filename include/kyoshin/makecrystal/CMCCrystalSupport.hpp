#pragma once

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"

namespace nw4r { namespace lyt { class ArcResourceAccessor; class Layout; class AnimTransform; class DrawInfo; } }

// Make-crystal "support" layout widget.
class CMCCrystalSupport : public IWorkEvent {
public:
    CMCCrystalSupport(nw4r::lyt::ArcResourceAccessor* accessor);
    virtual ~CMCCrystalSupport();

    /* 0x04 */ nw4r::lyt::ArcResourceAccessor* mAccessor;
    /* 0x08 */ nw4r::lyt::Layout* mLayout;
    /* 0x0C */ nw4r::lyt::AnimTransform* mAnimTransform;
    /* 0x10 */ u8 mLoaded;
    /* 0x14 */ s32 mState;
    /* 0x18 */ u8 mFinished;
};
