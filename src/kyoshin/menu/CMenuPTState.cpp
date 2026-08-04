// Cleaned TU for kyoshin/menu/CMenuPTState.
// FULL_MATCH: func_80192BE4, func_80192BEC (vtable adjustor thunks).

#include "kyoshin/menu/CMenuPTState.hpp"
extern u32 lbl_eu_80664300;

// Real member functions defined in code_8018F8D8.cpp -- accessed via their
// mangled linker names (suppresses further name decoration).
void cbRenderBefore__12CMenuPTStateFv(CMenuPTState* self);
void __dt__12CMenuPTStateFv(CMenuPTState* self);

void __ct__CMenuPTState(){}

extern "C" unsigned long func_80192BD0() { return lbl_eu_80664300 != 0; }

// FULL_MATCH: vtable adjustor thunks (IScnRender secondary base at offset +0x58).
void func_80192BE4(void* self) {
    cbRenderBefore__12CMenuPTStateFv(
        static_cast<CMenuPTState*>(reinterpret_cast<IScnRender*>(self)));
}

void func_80192BEC(void* self) {
    __dt__12CMenuPTStateFv(
        static_cast<CMenuPTState*>(reinterpret_cast<IScnRender*>(self)));
}

// ---------------------------------------------------------------------------
// Stubs (not yet FULL_MATCH for all)
// ---------------------------------------------------------------------------

// Target 1: us-80194310 - FULL_MATCH (0 structural mismatch, reloc drift only)
cf::UnkClass_80192BF4::UnkClass_80192BF4() {
    field_0x00 = 0;
    field_0x04 = 0.0f;
    field_0x08 = -1.0f;
}

// Target 2: us-8019432c - same bytecode as target 1
// Initializes a UnkClass_80192BF4-like struct at offsets 0x00/0x04/0x08
void __ct__80192C10(cf::UnkClass_80192BF4* self){
    self->field_0x00 = 0;
    self->field_0x04 = 0.0f;
    self->field_0x08 = -1.0f;
}

// Target 3: us-80194348 - func_80192C2C
extern "C" void func_80192C2C(cf::UnkClass_80192BF4* self, void* obj){
    if (self->field_0x04 > 0.0f) {
        self->field_0x00++;
    }
    
    // Call virtual function at vtable slot 0x61 (offset 0x308)
    void** vtable = *(void***)obj;
    int count = ((int (*)(void*))vtable[0x308 / 4])(obj);
    
    float val = (float)(count * 2 + 6);
    self->field_0x08 = val;
    self->field_0x04 = val;
}

// Target 5: us-801943cc - func_80192CB0
// Updates timer on the UnkClass_80192BF4 struct.
// If field_0x04 > 0, scans a list of battle objects looking for actors
// with specific status flags. If no matching actor is found, decrements
// field_0x04 by deltaTime * multiplier. When field_0x04 <= 0, resets all fields.
void func_80192CB0(){
    // TODO: implement
    // Uses CUICfEnumListHolder for list enumeration
    // Scans for objects with status 0x10 or 0xf at offset +8
    // If found, returns early (doesn't decrement timer)
    // Otherwise decrements and may reset
}