#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r {
namespace lyt {
class Layout;
class Pane;
class AnimTransform;
} // namespace lyt
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

// Selects the variant constructor (it takes the same arguments as the single one).

/*
 * Get-item screen controller (IUICf task with IWorkEvent and IScnRender bases).
 *
 * Constructed through the three factories CMenuGetItem_CreateSingle/Variant/Multi.
 * Layout is 0x2B4 bytes (retail allocation size).
 */
class CMenuGetItem : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuGetItem(CScn* parent, u32 arg);                    // single: arg -> mField_94, mField_2AF = 1
    CMenuGetItem(CScn* parent, s32 arg);                    // variant: arg -> mField_98, mField_2AF = 1
    CMenuGetItem(CScn* parent, u32 arg0, u32 arg1);         // multi: args -> mField_9C/9D, mField_2AF = 0
    virtual ~CMenuGetItem();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore();

    /* 0x60 */ CScn* mScn;                 // owning scene (render-callback target)
    /* 0x64 */ UnkClass_8045F564 mMemRegion; // scratch region for the layout file data
    /* 0x74 */ nw4r::lyt::Layout* mLayout; // layout drawn in cbRenderBefore
    /* 0x78 */ nw4r::lyt::AnimTransform* mField_78; // first anim transform
    /* 0x7C */ nw4r::lyt::Pane* mField_7C; // FindPaneByName result (variant 1)
    /* 0x80 */ nw4r::lyt::Pane* mField_80; // FindPaneByName result (variant 2)
    /* 0x84 */ nw4r::lyt::Pane* mField_84; // FindPaneByName result (variant 3)
    /* 0x88 */ u8 mField_88;
    /* 0x8C */ f32 mField_8C;
    /* 0x90 */ u8 mField_90;               // read by CMenuGetItem_GetField90
    /* 0x94 */ u32 mField_94;              // single variant stores the ctor arg
    /* 0x98 */ s32 mField_98;              // variant stores the ctor arg
    /* 0x9C */ u8 mField_9C;               // multi variant stores ctor arg0
    /* 0x9D */ u8 mField_9D;               // multi variant stores ctor arg1
    /* 0x9E */ GetItemEntry mItems[8];     // get-item entries (8 * 0x42 bytes)
    /* 0x2AE */ u8 mField_2AE;             // entry count
    /* 0x2AF */ u8 mField_2AF;             // single/variant: 1
    /* 0x2B0 */ u16 mField_2B0;
}; // size: 0x2B4
