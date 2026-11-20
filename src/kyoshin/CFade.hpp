#pragma once

#include <types.h>

#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// Shared string pool (split1 .rodata): fade layout arc filename.
extern char lbl_eu_8050B5A0[];
// SDA float - fade animation frame target (defined in CFade.cpp as
// const f32 1.0f; the .rodata arc-name string is defined there too as
// lbl_eu_8050B5A0; OnFileEvent fetches the handle first to keep order).
extern const f32 lbl_eu_80668750;

/* Fade overlay layout widget. Loads and animates a nw4r layout for fade-in/out
effects; receives its archive through IWorkEvent::OnFileEvent. */
class CFade : public IWorkEvent {
public:
    CFade();
    virtual ~CFade();
    virtual bool OnFileEvent(CEventFile* pEventFile);
    void Unload();
    void In();

    UnkClass_8045F564 mMemRegion;            // 0x04
    CFileHandle* mFileHandle;                // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // 0x18
    nw4r::lyt::Layout* mLayout;              // 0x1C
    nw4r::lyt::AnimTransform* mAnimTrans;    // 0x20

    u8 mIsLoaded;   // 0x24 - true after OnFileEvent completes loading
    u8 mFadeState;  // 0x25 - 0:idle, 1:fading in, 2:faded in, 3:fading out
    u8 mReady;      // 0x26 - true after layout is attached and visible
    u8 mVisible;    // 0x27 - visibility flag (toggled during fade animation)
};

// Retail-unmangled fade helpers
extern "C" int CFade_IsReady(CFade* self);
// C-linkage symbols (verbatim CFade_StepFadeIn/CFade_StepFadeOut/CFade_MarkLoaded).
// Mark the overlay loaded/ready once the layout is attached.
extern "C" void CFade_MarkLoaded(CFade* self);
// Animation-end handlers: fade-in reached target -> faded-in;
// fade-out rewound -> idle.
extern "C" void CFade_StepFadeIn(CFade* self);
extern "C" void CFade_StepFadeOut(CFade* self);