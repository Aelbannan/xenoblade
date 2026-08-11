#pragma once

#include <types.h>

#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CSysWin.hpp"

namespace nw4r {
namespace lyt {
class AnimTransform;
class Layout;
}
}

class CFileHandle;
class CScn;

struct CMenuGetItemMultiEntry {
    u32 packed;
    u16 value;
    u8 rest[0x2e];
};

struct CMenuGetItemPaneView {
    u8 _00[0x4c];
    f32 width;
    f32 height;
    u8 _54[0xbb - 0x54];
    u8 flags;
};

class CMenuGetItemMulti {
public:
    CMenuGetItemMulti();
    virtual ~CMenuGetItemMulti();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();
    void OnFileEvent();

    u8 _04[0x50];
    nw4r::lyt::Layout* mLayout;              // +0x54
    u32 field_58;
    u32 field_5C;
    s32 field_60;
    u8 field_64;
    u8 field_65;
    u8 field_66;
    u8 field_67;
    u32 field_68;
    u32 mIWorkEventVtable;                   // +0x6C
    u32 mIScnRenderVtable;                   // +0x70
    CScn* mScn;                              // +0x74
    u8 mRegion[0x10];                        // +0x78
    nw4r::lyt::AnimTransform* mAnim;         // +0x88
    u32 mInitialItems[4];                    // +0x8C
    u32 mVisibleItemCount;                   // +0x9C
    CMenuGetItemMultiEntry mEntries[4];      // +0xA0
    u32 mEntryCount;                         // +0x170
    CBaseCur mCursor;                        // +0x174
    CSysWin mSystemWindow;                   // +0x18C
    u8 mMaxVisibleItems;                     // +0x1C8
    u8 mHasSpecialItem;                      // +0x1C9
    u8 mPaneVisible[6];                      // +0x1CA
    CMenuGetItemMultiEntry* mVisibleEntries[6]; // +0x1D0
    u16 mVisibleItemIds[6];                  // +0x1E8
    u8 field_1F4;
    u8 field_1F5;
    u8 field_1F6;
    u8 field_1F7;
    u32 field_1F8;
    u32 field_1FC;
    u8 field_200;
    u8 field_201;
    u8 field_202[2];
    CFileHandle* mFileHandle;
    u32 field_208;
    u8 field_20C;
    u8 field_20D;
    u8 field_20E;
    u8 field_20F;
};
