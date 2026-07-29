#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CFileHandle;

// Full object layout for CExchangeWin (used by C-linkage accessors)
struct CExchangeWinFull {
    u8 _00[0x14];
    CFileHandle* mFileHandle;                // 0x14
    u8 _18[0x1C - 0x18];
    nw4r::lyt::Layout* mLayout;              // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransform; // 0x20
    u8 field_24;                              // 0x24
    u8 field_25;                              // 0x25
    u8 _26;                                   // 0x26
    u8 field_27;                              // 0x27
};

class CExchangeWin {
public:
    CExchangeWin();
    virtual ~CExchangeWin();
    void OnFileEvent();

    // TODO: add fields
};

