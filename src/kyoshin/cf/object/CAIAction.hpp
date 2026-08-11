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
    ~CAIAction();

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
extern void* func_800B708C(int);        // C++ linkage -> func_800B708C__Fi

struct CAIActionQuery;
struct CAIActionEnumHolder;

extern "C" void* func_8016FE34(void*);          // -> func_8016FE34
// NOTE (agent pi-019fef06, 2026-08-11): the two declarations below were
// changed from CAIActionEnumHolder* to void* to match the identical extern
// "C" declarations already in CVision.hpp (included alongside this header
// by CBattleManager.hpp). With both typed pointers visible in one TU, MWCC
// reports (10197) illegal function overloading, breaking every TU that
// includes both headers. ABI is unchanged (opaque pointer).
extern "C" void func_80043D90(void*);
extern "C" void __dt__80043E88(void*, int);
extern "C" void* func_80150828(cf::CAIAction*, CAIActionQuery*);
extern "C" void* func_80150618(cf::CAIAction*, CAIActionQuery*);
extern "C" int func_8014B8BC(void* a, void* b);
extern "C" void* func_801522C4(cf::CAIAction*, const void*);
extern "C" int func_8014CE78(cf::CAIAction*, const u8*, cf::CAIActionSlot*);
extern "C" int func_8014B344(cf::CAIAction*, u32);

// CfObjEnumList helper family (retail names unmangled -> extern "C").
extern "C" void* func_80043F18(void*);
extern "C" void func_800F4A98(void*, u32, u32);
extern "C" void func_800F6D50(void*, u32);
extern "C" void* func_800F6E08(void*);
extern "C" void* func_800F6EAC(void*, u32);
extern "C" void* func_800F6EC0(void*, u32);
extern "C" void func_800F6ED0(void*, void*);
extern "C" void func_800F9AEC(void*);
extern "C" void __ct__800FBF08(void*, u32, u32);
extern "C" void __ct__800FC040(void*, u32, u32, u32);
extern "C" void __ct__800FC19C(void*, u32, u32);
extern "C" void __ct__800FC4FC(void*, u32, f32);
extern "C" void __ct__800FC648(void*, u32, f32);
extern "C" void __ct__800FC790(void*, u32, f32);
extern "C" void __ct__800FC8DC(void*, u32, f32);
extern "C" void __ct__800FCA24(void*, void*, u32);
extern "C" void __ct__800FCB80(void*, void*, u32);
extern "C" void __ct__800FCD10(void*, void*, u32);
extern "C" void __ct__800FCF1C(void*, u32, u32);
extern "C" void __ct__800FD250(void*);
extern "C" void* func_800B76A4(void*);
extern "C" void* func_800B77E4(void*, void*);
extern "C" void* func_800B8B94(s32);
extern void* func_800B07E8();   // C++ linkage -> func_800B07E8__Fv
extern "C" int func_80148778(void*, int);
extern "C" void* func_80149154(void*, u32);
extern "C" void* func_800EA444(void*);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int);
extern "C" void* getInstance__Q22cf14CBattleManagerFv(void);
extern "C" int rand(void);
