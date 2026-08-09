#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CFileHandle;

// Full object layout for CLoad (used by C-linkage accessors)
struct CLoadFull {
    u8 _00[0x1C];
    u32 field_1C;
    u8 _20[0x28 - 0x20];
    u8 field_28;
    u8 _29[0x2B - 0x29];
    u8 field_2B;
    u8 field_2C;
};

// CLoad's vtable data symbol (retail .data). The constructor stores V->mvVtbl
// to this label manually (retail sets vptr to lbl_eu_8053A340, not the
// compiler-generated __vt__CLoad), mirroring the CBattery/CBgTex pattern.
extern void* lbl_eu_8053A340[];

// Stores vptr at +0x00 before the mMemRegion member constructs, matching
// retail ctor order (vptr set, then member ctor, then body).
struct CLoadVtblBase {
    void* mVtbl; // +0x00
    CLoadVtblBase() { mVtbl = lbl_eu_8053A340; }
};

class CLoad : public CLoadVtblBase {
public:
    CLoad(u8 arg);
    ~CLoad();
    void OnFileEvent();

    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ CFileHandle* mFileHandle;
    /* 0x18 */ nw4r::lyt::ArcResourceAccessor* mAccessor;
    /* 0x1C */ nw4r::lyt::Layout* mLayout;
    /* 0x20 */ nw4r::lyt::AnimTransform* mAnimTrans0;
    /* 0x24 */ nw4r::lyt::AnimTransform* mAnimTrans1;
    /* 0x28 */ u8 field_28;
    /* 0x29 */ u8 field_29;
    /* 0x2A */ u8 field_2A;
    /* 0x2B */ u8 field_2B;
    /* 0x2C */ u8 field_2C;
    /* 0x2D */ u8 field_2D;
};