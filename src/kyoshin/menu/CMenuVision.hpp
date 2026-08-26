#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"

class CProcess;

// Null pointer-to-member-function word triple (retail __ptmf_null layout).
struct PtmfWords {
    void* fn;       // +0x00
    u32 adj1;       // +0x04
    u32 adj2;       // +0x08
};

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

// Mirror of the font object returned by CDeviceFont::func_80452C10.
// vtable slot 9 (offset 0x24 with -RTTI) yields the u32 font handle bound
// into the layout's text panes. Never instantiated, so no vtable is emitted;
// a genuine virtual call makes MWCC emit the retail r12 dispatch sequence.
struct CMenuVisionFontView {
    virtual void vf2() = 0;
    virtual void vf3() = 0;
    virtual void vf4() = 0;
    virtual void vf5() = 0;
    virtual void vf6() = 0;
    virtual void vf7() = 0;
    virtual void vf8() = 0;
    virtual u32 getFontHandle() = 0; // vtable offset 0x24
};

class CMenuVision {
public:
    virtual ~CMenuVision();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x00: implicit vptr followed by CProcess base-class storage through 0x5F.
    u8 unk04[0x0C];        // 0x04-0x0F (CProcess/CDoubleListNode region)
    void* field_0x10;      // 0x10 - vtable slot (retail writes it twice: base
                           //         vtable, then final composite vtable group)
    u8 unk14[0x28];        // 0x14-0x3B
    PtmfWords cbSlot0;     // 0x3C-0x47 - null-ptmf callback slot
    PtmfWords cbSlot1;     // 0x48-0x53 - null-ptmf callback slot
    u8 field_0x54;         // 0x54 - screen-active flag byte (set by func_801AC1F8)
    u8 field_0x55;         // 0x55
    u8 _pad56[2];          // 0x56-0x57
    void* field_0x58;      // 0x58 - secondary vtable slot (+0x24)
    void* field_0x5C;      // 0x5C - IScnRender subobject vtable slot (+0xAC)
    CScn* mScn;            // 0x60 - owning scene; addRenderCB target
    UnkClass_8045F564 mLayoutMem;               // 0x64 - layout memory region (MEM2 alloc)
    CMenuVisionEntry mEntries[6];               // 0x74 - 6 vision slots (stride 0x30)
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);
extern "C" void* __dt__11CMenuVisionFv(void*, int);
extern "C" void cbRenderBefore__11CMenuVisionFv(void*);
extern "C" void __ct__8CProcessFv(CProcess* self);                     // CProcess base ctor (C-ABI import)
extern "C" void __dt__8CProcessFv(CProcess* self, int flags);          // CProcess base dtor (C-ABI import)
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop); // CProcess::Regist (C-ABI import)
extern "C" CMenuVision* __ct__CMenuVision(CMenuVision* self, CProcess* parent); // retail-unmangled ctor
// CDeviceFont font-handle lookup; retail Init calls it with a single argument
// (the second Layout* parameter is left unset at this call site).
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg);

// DrawInfo raw-storage ct/dt helpers: pre-mangled retail identifiers, so they
// must keep C linkage (a C++ declaration double-mangles the name).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(nw4r::lyt::DrawInfo* self, int flags);

// cf::CfGameManager helper (retail mangled member symbol, one u32 arg).
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 index);

// Retail data symbols referenced by the constructor.
extern char lbl_eu_8052C1C0[];   // base-class vtable group (pre-overwrite)
extern char lbl_eu_80533538[];   // final composite vtable group
extern u32 __ptmf_null[3];       // null pointer-to-member-function constant