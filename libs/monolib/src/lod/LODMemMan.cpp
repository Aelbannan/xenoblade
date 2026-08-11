// Auto-scaffolded catalog TU for monolib/src/lod/LODMemMan
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/lod/LODMemMan.hpp"

// --- FULL_MATCH functions ---

// Simple getter: return the float at offset 0x7c.
float LOD::LODMemMan::func_8046F01C() {
    return mFloat_7C;
}

// Tail-forward call to embedded sub-object's func_8046AADC.
void LOD::LODMemMan::func_8046F088() {
    mSubObject.func_8046AADC();
}

// --- Remaining harness stubs (empty bodies) ---

LOD::LODMemMan::LODMemMan() {}

LOD::LODMemMan::~LODMemMan() {}

void __dt__8046D144(){}

void LOD::LODMemMan::func_8046D264() {}

void LOD::LODMemMan::func_8046D898() {}

void LOD::LODMemMan::func_8046DA04() {}

void LOD::LODMemMan::func_8046DA64() {}

void LOD::LODMemMan::func_8046DAC0() {}

void LOD::LODMemMan::func_8046DBC8() {}

void LOD::LODMemMan::func_8046DD9C() {}

void LOD::LODMemMan::func_8046E1DC() {}

void LOD::LODMemMan::func_8046E594() {}

void LOD::LODMemMan::func_8046E5BC() {}

void LOD::LODMemMan::func_8046E6DC() {}

void LOD::LODMemMan::func_8046E770() {}

void LOD::LODMemMan::func_8046E780() {}

void LOD::LODMemMan::func_8046E7D0() {}

void LOD::LODMemMan::func_8046E820() {}

void LOD::LODMemMan::func_8046E8C8() {}

void LOD::LODMemMan::func_8046E920() {}

void LOD::LODMemMan::func_8046E988() {}

void LOD::LODMemMan::func_8046EAE8() {}

void LOD::LODMemMan::func_8046EB50() {}

void LOD::LODMemMan::func_8046EBEC() {}

void LOD::LODMemMan::func_8046EC88() {}

void LOD::LODMemMan::func_8046ECD4() {}

void LOD::LODMemMan::func_8046ED68() {}

void LOD::LODMemMan::func_8046EDD0() {}

void LOD::LODMemMan::func_8046EE9C() {}

void LOD::LODMemMan::func_8046EEE8() {}

void LOD::LODMemMan::func_8046EF30() {}

void LOD::LODMemMan::func_8046EF7C() {}

void LOD::LODMemMan::func_8046F010(float a) { *(float*)((u8*)this + 0x1CC8) = a; *(float*)((u8*)this + 0x7C) = a; }

void LOD::LODMemMan::func_8046F024() {}

void LOD::LODMemMan::func_8046F090() {}

void LOD::LODMemMan::func_8046F164() {}

void LOD::LODMemMan::func_8046F258() {}

void LOD::LODMemMan::func_8046F594() {}

void LOD::LODMemMan::func_8046FC04() {}

void LOD::LODMemMan::func_8046FEB8() {}

void LOD::LODMemMan::func_8046FF84() {}

void LOD::LODMemMan::func_80470184() {}

void LOD::LODMemMan::func_804702F0() {}

void LOD::LODMemMan::func_80470634() {}

void LOD::LODMemMan::func_804708B4() {}

void LOD::LODMemMan::func_804709FC() {}

void LOD::LODMemMan::func_80470A90() {}

void LOD::LODMemMan::func_80470B10() {}

void LOD::LODMemMan::func_80470DB0() {}

void LOD::LODMemMan::func_80470DCC() {}

void LOD::LODMemMan::func_80470EF8() {}

void LOD::LODMemMan::func_8047108C() {}

void LOD::LODMemMan::func_80471184() {}

void LOD::LODMemMan::func_80471224() {}

void LOD::LODMemMan::func_804712E0() {}

void LOD::LODMemMan::func_8047130C() {}

void LOD::LODMemMan::func_8047133C() {}

void LOD::LODMemMan::func_80471450() {}

void LOD::LODMemMan::func_8047146C() {}

void LOD::LODMemMan::func_80471484() {}

void LOD::LODMemMan::func_8047163C() {}

#pragma push
#pragma auto_inline off
void LOD::LODMemMan::func_804716B8() {}
#pragma pop

void LOD::LODMemMan::func_80471718() {}

void LOD::LODMemMan::func_80471748() {}

void LOD::LODMemMan::func_80471780() {}

void LOD::LODMemMan::func_80471794() {}

void LOD::LODMemMan::func_804717FC() {}

void LOD::LODMemMan::func_80471834() {}

void LOD::LODMemMan::func_80471938() {}

void LOD::LODMemMan::func_804719FC() {}

void LOD::LODMemMan::func_80471A70() {}

void LOD::LODMemMan::func_80471ACC() {}

void LOD::LODMemMan::func_80471BC8() {}

void LOD::LODMemMan::func_80471BF4() {}

// retail: lwz r3,0xC(r3); b func_804716B8 -- delegates to the sub-manager stored at +0xC
void LOD::LODMemMan::func_80471CC4() {
    (*(LOD::LODMemMan**)((u8*)this + 0xC))->func_804716B8();
}

void LOD::LODMemMan::func_80471CCC() {}

void LOD::LODMemMan::func_80471CDC() {}

void LOD::LODMemMan::OnFileEvent() {}
