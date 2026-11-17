#pragma once

// CPresentWin class definition only (no TU-local import block), so owners
// that embed a CPresentWin by value (CItemBoxGrid) can include it without
// the C-linkage decls in CPresentWin.hpp.
#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/work/IWorkEvent.hpp"

class CPresentWin : public IWorkEvent {
public:
    CPresentWin();
    virtual ~CPresentWin();

    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x04
    nw4r::lyt::Layout* mpLayout;              // 0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;    // 0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;    // 0x10
    nw4r::lyt::Pane* mPane14;                  // 0x14
    nw4r::lyt::Pane* mPane18;                  // 0x18
    nw4r::lyt::Pane* mPane1C;                  // 0x1C
    nw4r::lyt::Pane* mPane20;                  // 0x20
    nw4r::lyt::Pane* mPane24;                  // 0x24
    nw4r::lyt::Pane* mPane28;                  // 0x28
    nw4r::lyt::Pane* mPane2C;                  // 0x2C
    u8 mField30;                               // 0x30
    u8 mField31;                               // 0x31
    u8 mField32;                               // 0x32
    u8 mField33;                               // 0x33
    u16 mField34;                              // 0x34-0x35 (item id read by showPresentRank)
    u8 mField36;                               // 0x36
    u8 mField37;                               // 0x37
    u8 mField38;                               // 0x38
    u8 mDataArray[8];                          // 0x39-0x40 (indexed by lookupPresentRank)
    u8 mDataCount;                             // 0x41
};
