#pragma once

#include <types.h>

namespace cf {

// Minimal head layout for initCamIntfInstances (field at +0x8).
// Full camera event body is declared in CfCamDirectionIntf.hpp for intf helpers.
class CfCamEvent {
public:
    u8 unk0[8];
    s32 unk8;

    char** initCamIntfInstances();
};

} // namespace cf
