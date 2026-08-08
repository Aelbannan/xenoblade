// ICamControlRemote - bitfield/flag getters for the remote-controller camera
// control interface.  Each getter calls the virtual input-status getter at
// vtable +0x8 and extracts one or more flag bits from the returned status
// object (or, for the lengthier ones, gates on a controller-usability query).

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// Status/flag object returned by the vtable+0x8 input-status getter.
struct ICamRemoteStatus {
    /* 0x00 */ u32 field_0x0;
    /* 0x04 */ u32 field_0x4;
    /* 0x08 */ u8 pad_0x8[8];
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u32 field_0x14;
};

// Interface exposing the input-status getter at vtable +0x8.
struct ICamControlRemoteIf {
    virtual ICamRemoteStatus* getObject(); // vtable +0x8
};

// Remote pad "connected / usable" query.  C-ABI symbol (defined in CfCam).
extern "C" int func_8006D700(int controllerId);

// Tail-call alias for CfGameManager::getCurrentPad().
CPad* func_80274B28() {
    return cf::CfGameManager::getCurrentPad();
}

u32 func_80274B2C(ICamControlRemoteIf* self, int controllerId) {
    ICamRemoteStatus* st = self->getObject();
    if (func_8006D700(controllerId) == 0) {
        return 0;
    }
    u32 flags = st->field_0x0;
    if ((flags & (1 << 4)) == 0 || (flags & (1 << 5)) != 0) {
        return 0;
    }
    return (flags >> 4) & 1; // bit 27 (MSB)
}

bool func_80274BA4(ICamControlRemoteIf* self, int controllerId) {
    ICamRemoteStatus* st = self->getObject();
    if (func_8006D700(controllerId) != 0) {
        u32 flags = st->field_0x0;
        if ((flags & (1 << 4)) != 0 && (flags & (1 << 5)) != 0) {
            return false;
        }
        return (flags & (1 << 5)) != 0; // bit 26 (MSB)
    }
    return false;
}

bool func_80274C20(ICamControlRemoteIf* self) {
    ICamRemoteStatus* st = self->getObject();
    return ((st->field_0x10 & (1 << 11)) & ~st->field_0x14) != 0;
}

bool func_80274C68(ICamControlRemoteIf* self, int controllerId) {
    ICamRemoteStatus* st = self->getObject();
    if (func_8006D700(controllerId) != 0 && (st->field_0x4 & (1 << 12)) != 0) {
        return true;
    }
    return false;
}

bool func_80274CD4(ICamControlRemoteIf* self) {
    ICamRemoteStatus* st = self->getObject();
    return ((st->field_0x0 >> 2) & 1) != 0;
}

bool func_80274D08(ICamControlRemoteIf* self) {
    ICamRemoteStatus* st = self->getObject();
    return ((st->field_0x0 >> 3) & 1) != 0;
}

bool func_80274D3C(ICamControlRemoteIf* self) {
    ICamRemoteStatus* st = self->getObject();
    return (st->field_0x0 & 1) != 0;
}

bool func_80274D70(ICamControlRemoteIf* self) {
    ICamRemoteStatus* st = self->getObject();
    return ((st->field_0x0 >> 1) & 1) != 0;
}

bool func_80274DA4(ICamControlRemoteIf* self) {
    ICamRemoteStatus* st = self->getObject();
    return ((st->field_0x14 >> 11) & 1) != 0;
}