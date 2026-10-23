#pragma once

#include <harness_catalog.h>

// Scene-handle accessor (getScnHandle__Fv), defined in code_804BC9EC.cpp.
void* getScnHandle();

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// Forward-declared instead of including NMWException.h (which conflicts with
// __ppc_eabi_init.h's _stack_addr declaration).
extern "C" void* __register_global_object(void* object, void* destructor, void* registration);
extern "C" void __dt__804BD8E8();

// Minimal layout for the global data at lbl_eu_8065F32C.
// Field offsets 0x60 and 0x78 are used by Coli_GetOutMatrix_80A4 / Coli_ClearOutFlag_8078.
struct ScnResData {
    u8 field_0x00[0x60];
    u8* field_0x60; // array of 0xE0-byte elements
    u8 field_0x64[0x78 - 0x64];
    int count; // 0x78
    // Retail .bss block lbl_eu_8065F32C spans 0xC4 bytes; only the fields
    // above (0x7C) are modeled. Fold the anonymous block tail in here so no
    // separate file-scope static pads (which -ipa file strips) are needed.
    u8 blockTail[0xC4 - 0x7C];
};

extern ScnResData lbl_eu_8065F32C;

// The real class of lbl_eu_8065F32C is CColiMoverState (full 0xC4 layout in
// coli/code_804B59C8.cpp). This scn-side view declares only the member the
// scn TUs call; ScnResData above stays the field view used here.
struct CColiMoverState {
    void initColiMover();
};

extern "C" {
ScnResData* Coli_GetOutMatrix_80A4(ScnResData* self, int index);
void Coli_ClearOutFlag_8078(ScnResData* self, int index);
void getMoverEntry(ScnResData* self);
// (memberized: initColiMover -> CColiMoverState::initColiMover; declaration moved into class CColiMoverState)
void Coli_CheckFlag(ScnResData*);
void clrSeenBitTbl(int, int);
void setSeenBitTbl(ScnResData*);
}
