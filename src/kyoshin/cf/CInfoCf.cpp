// Auto-scaffolded catalog TU for kyoshin/cf/CInfoCf
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CInfoCf.hpp"

#include "kyoshin/menu/CMenuGCItem.hpp"
#include "monolib/core/CPadManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <revolution/gx/GXPixel.h>
#include "kyoshin/CTitleAHelpImports.hpp"
void playUISound(u32);


// The retail ctor symbol __ct__cf_CInfoCf is a C-linkage name (no C++ mangling
// markers), so it is emitted as a global function taking the instance (same
// convention as __ct__cf_CfTFile / __ct__Q22cf8CREvtMemFv). It installs the
// externally-defined vtable, zeroes the flag word and the cached setting, and
// records the instance in the singleton global for the rest of the TU.
cf::CInfoCf* __ct__cf_CInfoCf(cf::CInfoCf* self) {
    self->vtable = lbl_eu_805309B0;
    self->mFlags = 0;
    self->mField08 = 0;
    lbl_eu_80664250 = self;
    return self;
}

// Destructor: clear the singleton pointer. The null check and delete-flag
// handling (conditional __dl__FPv) are emitted by MWCC for the member dtor.
cf::CInfoCf::~CInfoCf() {
    lbl_eu_80664250 = 0;
}

extern "C" void* InfoCfGetUnk0C() { return (void*)lbl_eu_80664250->mField0C; }

extern "C" void* InfoCfGetUnk10() { return (void*)lbl_eu_80664250->mField10; }

// FULL_MATCH - no-op virtual function (vtable slot 2).  Immediately returns.
void InfoCfNoopVirt() {
}

// Toggle bits of the UI-state singleton's flag word: set bit 0x2, then
// immediately clear bit 0x1. The singleton pointer is re-read from the global
// between the two (retail reloads it), so both statements dereference the
// global directly rather than sharing a local.
void InfoCfSetBit2ClearBit1() {
    lbl_eu_80664250->mFlags |= 0x2;
    lbl_eu_80664250->mFlags &= ~0x1;
}

void InfoCfRefreshSettings(cf::CInfoCf* self) {
    // Refresh the cached setting state: re-read the UI flag word, the option
    // and timer values, then rewrite the flag word (clear bit 0x1; set it back
    // when the cached setting matches the option-flag/pause combo).
    cf::CfGameManager::getInstance();
    self->mField08 = InfoCfGetUiFlags();
    CfRes_getInstPtrBC();
    self->mField0C = KyoshinHeap_GetActive54();
    u32 v = KyoshinHeap_GetSize500000();
    u32 flags = self->mFlags & ~1;
    u32 f08 = self->mField08;
    self->mField10 = v;
    self->mFlags = flags;
    if ((f08 & 0x30800) == 0x30800 && (f08 & 0x65000004) == 0) {
        self->mFlags = flags | 1;
    }
}

// FULL_MATCH - Returns the global UI state flag value (lbl_eu_80663E24).
// noinline: retail calls this out-of-line from InfoCfRefreshSettings.
__declspec(noinline) u32 InfoCfGetUiFlags() {
    return lbl_eu_80663E24;
}

// Destructor (retail D1 form; symbol keeps the CW member mangling in US).
// Written as an extern "C" free function (same scheme as ~CMenuGCItem) so the
// member subobjects (CItemBoxGrid @0xB8, CTitleAHelp @0x80, CBgTex @0x60) are
// destroyed explicitly in retail order with a non-deleting -1 flag, and the
// base-class destruction targets the library D1 symbol __dt__8CProcessFv
// (0x804474D0). The nested double null-check before the base call is an MWCC
// D2-inlined-into-D1 artifact (same shape as ~CSystemWindow / ~CMenuGCItem);
// the delete follows when the incoming deleting flag is > 0.
