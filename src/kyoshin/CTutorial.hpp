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

    // TODO: add fields
};

