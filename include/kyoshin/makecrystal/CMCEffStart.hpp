#ifndef CMCEFFSTART_HPP
#define CMCEFFSTART_HPP

#include <types.h>

namespace nw4r {
    namespace lyt {
        class ArcResourceAccessor;
        class Layout;
        class AnimTransform;
    }
}

class CMCEffStart {
public:
    CMCEffStart(nw4r::lyt::ArcResourceAccessor* pArcResourceAccessor);
    virtual ~CMCEffStart();

    void func_80223DB0();
    void func_80223E40();
    void func_80223EBC();
    u8 func_80223F1C();
    void func_80223F24();
    void func_80223F44();

    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x08 */ nw4r::lyt::ArcResourceAccessor* mArcResourceAccessor;
    /* 0x0C */ nw4r::lyt::Layout* mLayout;
    /* 0x10 */ nw4r::lyt::AnimTransform* mAnimTrans;
    /* 0x14 */ u8 unk14;
};

#endif
