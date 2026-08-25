#pragma once

#include <types.h>
#include <nw4r/lyt.h>
#include "monolib/lib/UnkClass_8045F564.hpp"

class CFileHandle;
class CEventFile;

// Accessor view of the trailing CLoad flag bytes used by func_802AE8E0.
struct CLoadFull {
    u8 _00[0x1C];
    u32 field_1C; // CLoad::mLayout
    u8 _20[0x28 - 0x20];
    u8 field_28; // CLoad::mIsLoaded
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

// Loading-screen layout controller: owns the lyt layout + animations used
// while a file request completes, driven by CEventFile callbacks.
class CLoad : public CLoadVtblBase {
public:
    CLoad(u8 arg);
    ~CLoad();
    bool OnFileEvent(CEventFile* pEventFile);

    /* 0x04 */ UnkClass_8045F564 mMemRegion;
    /* 0x14 */ CFileHandle* mFileHandle;
    /* 0x18 */ nw4r::lyt::ArcResourceAccessor* mAccessor;
    /* 0x1C */ nw4r::lyt::Layout* mLayout;
    /* 0x20 */ nw4r::lyt::AnimTransform* mAnimTrans0;
    /* 0x24 */ nw4r::lyt::AnimTransform* mAnimTrans1;
    /* 0x28 */ u8 mIsLoaded;   // set once the layout was built from the archive
    /* 0x29 */ u8 field_29;    // draw gate (nonzero = draw allowed)
    /* 0x2A */ u8 mAnimStep;   // animation state machine step (0 idle, 1..3)
    /* 0x2B */ u8 field_2B;
    /* 0x2C */ u8 field_2C;    // clear while an animation runs, set when done
    /* 0x2D */ u8 field_2D;    // selects the language-specific resource names
};

void func_802AE508(CLoad* self);
void func_802AE560(CLoad* self);
void func_802AE5F0(CLoad* self, nw4r::lyt::DrawInfo* drawInfo);
void func_802AE62C(CLoad* self);
u8 func_802AE6AC(CLoad* self);
u8 func_802AE6B4(CLoad* self);
u8 func_802AE6BC(CLoad* self);
void func_802AE6C4(CLoad* self);
void func_802AE758(CLoad* self);
void func_802AE7EC(CLoad* self);
void func_802AE894(CLoad* self);
void func_802AE8E0(CLoadFull* self);
