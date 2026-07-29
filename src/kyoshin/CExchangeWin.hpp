#pragma once

#include <types.h>
#include <nw4r/lyt.h>

// Full object layout for CExchangeWin (used by C-linkage accessors)
struct CExchangeWinFull {
    u8 _00[0x1C];
    nw4r::lyt::Layout* mLayout;  // 0x1C
    u8 _20[0x24 - 0x20];
    u8 field_24;
    u8 field_25;
    u8 _26;
    u8 field_27;
};

class CExchangeWin {
public:
    CExchangeWin();
    virtual ~CExchangeWin();
    void OnFileEvent();

    // TODO: add fields
};

