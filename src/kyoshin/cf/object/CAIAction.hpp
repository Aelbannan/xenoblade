#pragma once

#include <types.h>

namespace cf {

// 0x20-byte AI action slot (slots[] / trailer / export buffer)
struct CAIActionSlot {
    u32 unk00; // 0x00
    u32 unk04; // 0x04
    u32 unk08; // 0x08
    u32 unk0C; // 0x0C
    u16 unk10; // 0x10
    s16 unk12; // 0x12
    f32 unk14; // 0x14
    u32 unk18; // 0x18
    u32 unk1C; // 0x1C
};

// symbols.txt mangles Fv; retail passes out buffers in r4/r5
struct CAIActionExport {
    u8 pad[0x200];
    CAIActionSlot* buffer; // 0x200
    u32 unk204;            // 0x204
    u32 unk208;            // 0x208
    u32 unk20C;            // 0x20C - column stride
};

// size: 0xB1C
class CAIAction {
public:
    CAIAction();

    // Declared Fv for vtable; body is extern "C" with outA/outB args
    virtual void CAIAction_UnkVirtualFunc1(); // 0x8
    virtual void CAIAction_UnkVirtualFunc2(); // 0xC

    // 0x0: vtable
    u32 unk4;          // 0x4
    u16 unk8;          // 0x8
    u8 padA[2];        // 0xA
    u8 slots[0x200];   // 0xC .. 0x20C (16 * 0x20)
    void* unk20C;      // 0x20C - pointer to slots
    u32 unk210;        // 0x210
    u32 unk214;        // 0x214
    u32 unk218;        // 0x218 - capacity (0x10)
    u8 entries[0x8C0]; // 0x21C .. 0xADC (160 * 0xE)
    u8 trailer[0x20];  // 0xADC .. 0xAFC (CAIActionSlot)
    u32 unkAFC;        // 0xAFC
    u8 unkB00;         // 0xB00
    u8 padB01[0xF];    // 0xB01
    u32 unkB10;        // 0xB10
    u32 unkB14;        // 0xB14
    u32 unkB18;        // 0xB18
};

} // namespace cf

// Fv mangling, but callers leave outA in r4 and outB in r5
extern "C" void CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* outA,
                                                              cf::CAIActionExport* outB);

// Inverse of UnkVirtualFunc1: imports trailer from inA, then imports ring
// entries from inB into this->unk20C. Same Fv mangling / r4-r5 ABI.
extern "C" void CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* inA,
                                                              cf::CAIActionExport* inB);

extern void func_8014A86C(void*);
extern void func_8014A8F8();
