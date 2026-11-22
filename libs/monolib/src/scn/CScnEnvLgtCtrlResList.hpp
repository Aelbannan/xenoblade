#pragma once

#include <types.h>

// Env-controller list owner (the object at CScn+0x7C). Split out of
// CScnEnvLgtCtrl.hpp so scene-side TUs (CScn, CTaskEnvironment, ...) can use the
// real owner type without pulling in the whole light-controller header.

// Light-item ring entries (IScnEnvCtl view; full definition in CScnEnvLgtCtrl.hpp).
struct CScnEnvLgtCtrlListItem;

// Ring-list node for the light-item ring of the env-controller list owner
// (CScnEnvLgtCtrlResList::base, sentinel pointer at owner+0x0C)
// (mirrors _reslist_node<T>: links at +0/+0x4, item pointer at +0x8).
struct CScnEnvLgtCtrlListNode {
    CScnEnvLgtCtrlListNode* mNext;   // +0x00
    CScnEnvLgtCtrlListNode* mPrev;   // +0x04
    CScnEnvLgtCtrlListItem* mItem;   // +0x08
};

// Flattened reslist<IScnEnvCtl> layout (retail __dt___reslist_base_IScnEnvCtl /
// __dt__reslist_IScnEnvCtl). The template names cannot be reproduced by a
// Wii/1.1 instantiation (symbolic mangling), so the dtors are spelled out
// flat; this mirrors reslist.hpp: vtable@0, mStartNodePtr@4, sentinel@8,
// mList@0x14, mCapacity@0x18, externally-owned flag@0x1C. The nodes are the
// light-item ring nodes (CScnEnvLgtCtrlListNode, _reslist_node<IScnEnvCtl*>).
struct CScnEnvLgtCtrlIScnResBase {
    void* mVtable;                              // +0x00
    CScnEnvLgtCtrlListNode* mStartNodePtr;      // +0x04 ring sentinel pointer
    CScnEnvLgtCtrlListNode mStartNode;          // +0x08 sentinel
    CScnEnvLgtCtrlListNode* mList;              // +0x14 node array
    int mCapacity;                              // +0x18
    bool field_0x1C;                            // +0x1C externally-owned flag
};

struct CScnItemModelNw4rOwner;  // full definition in CScnItemModelNw4r.hpp

// Forward decls for the blend/bloom objects owned by the reslist (full
// definitions in CScnEnvLgtCtrl.hpp; __dt__804C0E48 releases them through these).
struct CScnEnvLgtBlend;
struct CScnEnvLgtBloom;

struct CScnEnvLgtCtrlLgtParamCtl;

// name-guess: CScnEnvLgtCtrlResList (no retail/jp name; the owner is
// non-polymorphic, so it has no RTTI string). The scene's env-light controller
// list owner (CScn::mLgtCtrl, CScn+0x7C) constructed by __ct__reslist_IScnEnvCtl
// (retail flat name; the template cannot be instantiated under Wii/1.1's
// symbolic mangling, cf. the two reslist destructors). The object carries an
// 8-byte prefix (flags word + scene-owner pointer) ahead of the
// _reslist_base<IScnEnvCtl> subobject at +0x08 (its mStartNodePtr at +0x0C is
// the light-item ring sentinel walked by the scnLgt* owner functions), then the
// derived fields at +0x28..+0x40 (the blend/bloom pair released by
// __dt__804C0E48, the light-param control, the slot-blend pair and the flash
// counter).
struct CScnEnvLgtCtrlResList {
    union {
        u32 mCount;                        // +0x00 interface vtable slot / count
        u32 flags;                         // +0x00 flags view (bit 3 = blend/bloom pair, __dt__804C0E48)
    };
    CScnItemModelNw4rOwner* mOwner;    // +0x04 scene owner (Scn_CallUnk8C_V8 arg)
    CScnEnvLgtCtrlIScnResBase base;    // +0x08 _reslist_base<IScnEnvCtl>
    CScnEnvLgtBlend* field_0x28;       // +0x28 blend object (released via vtable slot 2)
    CScnEnvLgtBloom* field_0x2C;       // +0x2C bloom object (main vtable at +0x80, slot 7)
    CScnEnvLgtCtrlLgtParamCtl* field_0x30;  // +0x30 light-param control (Scn_CallUnk8C_V11)
    f32 field_0x34;                    // +0x34 slot-blend weight (scnLgtPushSlotBlend)
    f32 field_0x38;                    // +0x38 slot-blend direction (scnLgtPushSlotBlend)
    void* field_0x3C;                  // +0x3C light-data arg (scnLgtBindLightArg; blend bound word)
    u32 field_0x40;                    // +0x40 flash step counter (scnLgtUpdateSceneLights)
    void CallLightV26(void* a, void* b);
    void CallLightV27(void* a, void* b, float f);
};
