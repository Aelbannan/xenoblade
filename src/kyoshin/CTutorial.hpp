#pragma once

#include <types.h>

// Full object layout for CTutorial (used by C-linkage accessors)
struct CTutorialFull {
    u8 _00[0x46];
    u8 field_46;
    u8 field_47;
    u8 _48[0x52 - 0x48];
    u8 field_52;
};

class CTutorial {
public:
    CTutorial();
    virtual ~CTutorial();
    void OnFileEvent();

    u8 func_8029ACAC();
    u8 func_8029ACB4();
    u8 func_8029ACBC();

    // layout after vtable ptr (4 bytes):
    u8 _pad04[0x46 - 4];     // 0x04-0x45
    u8 field_46;              // 0x46
    u8 field_47;              // 0x47
    u8 _pad48[0x52 - 0x48];   // 0x48-0x51
    u8 field_52;              // 0x52
};

