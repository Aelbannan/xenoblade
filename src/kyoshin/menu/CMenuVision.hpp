#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"

class CProcess;

// Sub-entry for each of the 6 vision slots (array at CMenuVision+0x74, stride 0x30)
struct CMenuVisionEntry {
    nw4r::lyt::Layout* mLayout;                  // 0x00
    nw4r::lyt::AnimTransform* mAnim1;            // 0x04
    nw4r::lyt::AnimTransform* mAnim2;            // 0x08
    nw4r::lyt::AnimTransform* mAnim3;            // 0x0C
    nw4r::lyt::AnimTransform* mAnim4;            // 0x10
    nw4r::lyt::AnimTransform* mAnim5;            // 0x14
    nw4r::lyt::AnimTransform* mAnim6;            // 0x18
    nw4r::lyt::AnimTransform* mAnim7;            // 0x1C
    nw4r::lyt::AnimTransform* mAnim8;            // 0x20
    int mState;                                   // 0x24
    u8 field_0x28;                                // 0x28
    u8 _pad29[3];                                 // 0x29-0x2B
    f32 mTimer;                                   // 0x2C
};

// Pad struct to access pane visibility byte at offset 0xBB
struct PaneVisAccess {
    u8 _pad[0xBB];
    u8 visByte;
};

class CMenuVision {
public:
    virtual ~CMenuVision();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x00: implicit vptr followed by CProcess base-class storage through 0x5F.
    u8 unk00[0x50];   // 0x04-0x53 (CProcess/CDoubleListNode region)
    u8 field_0x54;    // 0x54 - screen-active flag byte (set by func_801AC1F8)
    u8 unk55[0x0B];   // 0x55-0x5F (IScnRender vtable slot at 0x5C)
    CScn* mScn;       // 0x60 - owning scene; addRenderCB target
    UnkClass_8045F564 mLayoutMem;               // 0x64 - layout memory region (MEM2 alloc)
    CMenuVisionEntry mEntries[6];               // 0x74 - 6 vision slots (stride 0x30)
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void* __dt__11CMenuVisionFv(void*, int);
extern "C" void cbRenderBefore__11CMenuVisionFv(void*);
extern "C" void __dt__8CProcessFv(CProcess* self, int flags);          // CProcess base dtor (C-ABI import)
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop); // CProcess::Regist (C-ABI import)
extern "C" CMenuVision* __ct__CMenuVision(CMenuVision* self, CProcess* parent); // retail-unmangled ctor
// CDeviceFont font-handle lookup; retail Init calls it with a single argument
// (the second Layout* parameter is left unset at this call site).
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg);