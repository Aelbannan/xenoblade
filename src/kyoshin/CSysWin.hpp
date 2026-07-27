#pragma once

#include <types.h>

// Full object layout for CSysWin (used by C-linkage accessors)
struct CSysWinFull {
    u8 _00[0x28];
    u8 field_28;
    u8 _29[0x34 - 0x29];
    u8 field_34;
    u8 _35;
    u8 field_36;
};

class CSysWin {
public:
    CSysWin();
    virtual ~CSysWin();
    void OnFileEvent();

    // TODO: add fields
};

