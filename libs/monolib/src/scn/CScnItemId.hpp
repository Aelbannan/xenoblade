#pragma once

#include <types.h>

// Retail vtable for CScnItemId (unmapped .data label). Declared as an array so
// MWCC emits the full lis/addi pair (no sda21 short-form) with the exact
// retail reloc name; the object vptr is stored manually (see CScnItemId).
extern "C" u32 lbl_eu_8056DCD8[];

// Scene-resource current-data getter (scn/code_804BC9EC.cpp). The retail name
// already ends in `__Fv` as a literal identifier, so C linkage is required to
// avoid double-mangling. Takes no arguments, returns the active scene-resource
// container pointer.
extern "C" void* func_804BC9EC__Fv(void);

// Scene-resource helpers (scn/code_804BC9EC.cpp). Declared as C++ global
// functions so MWCC mangles them to the retail forms func_804BCC1C__FPv and
// func_804BC9F4__FPvUl.
void func_804BCC1C(void* p);
int func_804BC9F4(void* outStruct, u32 data);

// Host scene-object accessors (unmangled C-linkage retail names).
extern "C" u32 func_80496018(void* self);            // scene allocation handle
extern "C" u32 func_8048C5B8(void* pool, s32 kind);  // sub-pool capacity check
extern "C" void* func_8048C698(void* pool, int kind);  // sub-pool slot accessor
extern "C" void func_8048C630(void* pool, void* item, u32 value);  // register item

// Node in the scene-id circular list (single link at +0x00).
struct CScnItemIdNode {
    CScnItemIdNode* next;   // +0x00
};

// Sub-pool slot returned by func_8048C698(kind 5): circular-list sentinel at
// +0x04.
struct CScnItemIdListSlot {
    u32 field_0x0;              // +0x00
    CScnItemIdNode* mAnchor;    // +0x04 list sentinel
};

// Host scene object for the id-item factory ctor (retail placeholder symbol
// __ct__804820F8): scene-item pool at +0x60.
struct CScnItemIdHost {
    u8 _00[0x60];           // +0x00
    u8* mPool;              // +0x60
};

// Scene id item (0x54 bytes). Polymorphic in retail (vtable lbl_eu_8056DCD8);
// novtable so the vptr is stored manually with the retail reloc name.
class __declspec(novtable) CScnItemId {
public:
    virtual ~CScnItemId();

    // +0x00: vtable pointer (lbl_eu_8056DCD8, stored manually)
    CScnItemIdHost* mParent;  // +0x04 owning scene host
    u16 mType;                // +0x08 item kind (5 = id)
    char mName[0x40];         // +0x0C name buffer
    u32 mNameLen;             // +0x4C name length
    u32 mValue;               // +0x50
};
