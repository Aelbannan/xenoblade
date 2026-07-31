#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

namespace nw4r {
namespace lyt {
    class Layout;
    class AnimTransform;
    class ArcResourceAccessor;
}
}

class CArtsInfo {
public:
    CArtsInfo();
    virtual ~CArtsInfo();
    void OnFileEvent();
    u8 getField48();
    u8 getField49();
    void setField54(u8 val);
    void setField55(u8 val);
    void setField56(u8 val);
    void setField58(u16 val);
    u32 getField5A();
    int isField44GE6();
    void initialize();

    // vtable pointer at 0x00 (implicit)
    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ int field_0x14;  // file handle 1
    /* 0x18 */ int field_0x18;  // file handle 2
    /* 0x1C */ int field_0x1C;  // arc resource accessor
    /* 0x20 */ nw4r::lyt::Layout* mpLayout1;
    /* 0x24 */ nw4r::lyt::AnimTransform* mpAnimTrans1;
    /* 0x28 */ nw4r::lyt::AnimTransform* mpAnimTrans2;
    /* 0x2C */ nw4r::lyt::AnimTransform* mpAnimTrans3;
    /* 0x30 */ nw4r::lyt::AnimTransform* mpAnimTrans4;
    /* 0x34 */ nw4r::lyt::Layout* mpLayout2;
    /* 0x38 */ nw4r::lyt::AnimTransform* mpAnimTrans5;
    /* 0x3C */ nw4r::lyt::AnimTransform* mpAnimTrans6;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 _41[3];      // padding
    /* 0x44 */ int field_0x44;  // state machine
    /* 0x48 */ u8 field_0x48;
    /* 0x49 */ u8 field_0x49;
    /* 0x4A */ u8 _4A[2];      // padding
    /* 0x4C */ int field_0x4C;
    /* 0x50 */ int field_0x50;
    /* 0x54 */ u8 field_0x54;
    /* 0x55 */ u8 field_0x55;
    /* 0x56 */ u8 field_0x56;
    /* 0x57 */ u8 _57;          // padding
    /* 0x58 */ u16 field_0x58;
    /* 0x5A */ s8 field_0x5A;
    /* 0x5B */ u8 _5B;          // padding
    /* 0x5C */ u8 mCursor[0x18]; // CCur18 embedded cursor
};
