#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class Pane;
class AnimTransform;
class ArcResourceAccessor;
}
} // namespace nw4r

class CScn;

// One get-item entry (stride 0x42 bytes). The item type byte gates whether
// the entry is shown, the name is a sprintf'd string, and the tail byte is
// copied verbatim when entries are restaged.
struct GetItemEntry {
    u8 type;         // 0x00 (1 = ? / 2 = ? / 3 = ?, or 0 = empty)
    char name[0x40]; // 0x01 sprintf target
    u8 field_0x41;   // 0x41
}; // 0x42 bytes

// View of the nw4r Pane bytes this TU touches (retail nw4r keeps the visible
// flag in the low bit of the byte at +0xBB; the repo's nw4r headers don't
// model that field).
struct PaneFlagView {
    u8 _00[0xBB];
    u8 flags; // 0xBB - bit 0 = visible
};

// View of the AnimTransform float this TU touches (current-frame counter at
// +0x10; repo nw4r AnimTransform is a skeleton without members).
struct AnimFrameView {
    u8 _00[0x10];
    f32 frame; // 0x10
};

/*
 * Get-item screen controller.
 *
 * Deliberately NON-polymorphic (the CSystemWindow idiom): the CProcess base
 * region is embedded at +0x00 and the UnkClass_8045F564 scratch region at
 * +0x64 is raw storage. The dtor drives both destructions by hand (via their
 * retail dtor symbols); a C++ CProcess base or a real UnkClass_8045F564
 * member would make MWCC emit an extra vptr store / duplicate subobject
 * destruction that the retail dtor does not have.
 *
 * Layout is 0x2B4 bytes (retail allocation size; ctor writes through 0x2B0).
 */
class CMenuGetItem {
public:
    CMenuGetItem();
    ~CMenuGetItem();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    u8 mProcess[0x10];         // 0x00 CProcess storage head (CDoubleListNode)
    u32 mProcessVt;            // 0x10 CProcess vtable slot (temp, then composite)
    u8 mProcRest[0x39 - 0x14]; // 0x14..0x38 CProcess tail (CChildListNode)
    u8 mIsRemove;              // 0x39 CProcess::mIsRemove
    u8 mIsDisableMove;         // 0x3A
    u8 mIsDisableDraw;         // 0x3B
    u32 ptmfMove[3];           // 0x3C
    u32 ptmfDraw[3];           // 0x48
    u8 mField_54;              // 0x54 - set to 1 by func_8014A2D0
    u8 mField_55;              // 0x55
    u8 _pad56[2];              // 0x56
    u32 mIWorkEventVt;         // 0x58 - IWorkEvent subobject vtable slot
    u32 mIScnRenderVt;         // 0x5C - IScnRender subobject vtable slot
    CScn* mScn;                // 0x60 - owning scene (removeRenderCB target)
    u8 mUnkClass[0x10];        // 0x64 - scratch region storage (manual dtor)
    nw4r::lyt::Layout* mLayout;  // 0x74 - layout drawn in cbRenderBefore
    nw4r::lyt::AnimTransform* mField_78; // 0x78 - first anim transform
    nw4r::lyt::Pane* mField_7C;          // 0x7C - FindPaneByName result (variant 1)
    nw4r::lyt::Pane* mField_80;          // 0x80 - FindPaneByName result (variant 2)
    nw4r::lyt::Pane* mField_84;          // 0x84 - FindPaneByName result (variant 3)
    u8 mField_88;              // 0x88
    u8 _pad89[0x8C - 0x89];    // 0x89..0x8B
    f32 mField_8C;             // 0x8C - initialised to lbl_eu_80667418
    u8 mField_90;              // 0x90 - read by func_8014A2B4
    u8 _pad91[0x94 - 0x91];    // 0x91..0x93
    u32 mField_94;             // 0x94 - single variant stores the ctor arg
    u32 mField_98;             // 0x98
    u8 mField_9C;              // 0x9C - multi variant stores ctor arg0
    u8 mField_9D;              // 0x9D - multi variant stores ctor arg1
    GetItemEntry mItems[8];    // 0x9E - get-item entries (8 * 0x42 bytes)
    u8 mField_2AE;             // 0x2AE - entry count
    u8 mField_2AF;             // 0x2AF - single variant: 1
    u16 mField_2B0;            // 0x2B0
};

// GetItem singleton instance (retail SDA symbol).
extern CMenuGetItem* lbl_eu_806641A8;

// cbRenderBefore gate flag (bit 21 skips the draw).
extern u32 lbl_eu_80663E28;

// Temporary (CProcess) vtable, pre-PMF copy; composite vtable afterwards.
extern char lbl_eu_8052BF70[];
// Composite vtable: IWorkEvent sub-vtable at +0x24, IScnRender sub-vtable at +0xac.
extern char lbl_eu_8052EA40[];
// Null pointer-to-member-function constant (3 words, copied into +0x3C..0x53).
extern u32 __ptmf_null[3];
// mField_8C initial value (sdata2 float pool).
extern const f32 lbl_eu_80667418;

// Get-item string table (layout name, anim name, pane names, sprintf formats).
extern char lbl_eu_805018D0[];

// C-linkage imports (retail names are unmangled free functions in this TU;
// MWCC mangles plain global-scope free functions with a __F suffix, so the
// declarations must sit under extern "C" to emit the retail reloc names).
extern "C" u8 func_80144FC8();
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void* func_801394D4(u32);
// Pre-mangled static member name (CDeviceFont::func_80452C10(u32, Layout*));
// extern "C" keeps the literal identifier (no double mangling).
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);

// C-ABI imports (retail pre-mangled symbols keep C linkage).
extern "C" void __dt__8CProcessFv(CProcess* self, int flags);
// Raw CProcess ctor: CProcess is abstract (pure virtuals) so it cannot be
// placement-new'd from the factory/ctor; retail emits its mangled C-ABI name.
extern "C" void __ct__8CProcessFv(CProcess* self);
// Scratch-region ctor (retail emits the direct C-ABI call with a plain addi;
// a C++ placement new would add a null-check branch the retail does not have).
extern "C" void __ct__17UnkClass_8045F564Fv(u8* self);
// nw4r DrawInfo C-ABI ct/dt (retail emits the direct calls; a C++ local would
// virtual-dispatch its scope-exit destructor - CMenuGCItem idiom).
extern "C" void __ct__Q34nw4r3lyt8DrawInfoFv(u8* self);
extern "C" void __dt__Q34nw4r3lyt8DrawInfoFv(u8* self, int flags);
