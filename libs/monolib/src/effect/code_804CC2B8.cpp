// Auto-scaffolded catalog TU for monolib/src/effect/code_804CC2B8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// Float constant 0.0f, shared across this TU
extern f32 lbl_eu_8066B0DC;

// Effect struct — partial layout; only fields accessed by matching functions
// are declared. The struct is large (~0x338 bytes).
struct EffectStruct {
    s16 field_0x00;
    s16 field_0x02;
    s16 field_0x04;
    u16 field_0x06; // flags/bitfield
    u8 pad_0x08[0x10 - 0x08];
    u32 field_0x10; // also used as float via type punning
    u32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    u8 pad_0x20[0x328 - 0x20];
    u32 field_0x328;
    u32 field_0x32c;
    u32 field_0x330;
    u32 field_0x334;
};

u32 func_804CC800(void* self) { return *(u32*)((u8*)self + 0x0); }

void func_804CCF84(){}

void func_804CD0CC(){}

void func_804CDE50(){}

void func_804CDF20(){}

void func_804CE160(){}

void func_804CE264(){}

u32 func_804CE378(void* self) { return *(u32*)((u8*)self + 0x4); }

u32 func_804CE380(void* self) { return *(u32*)((u8*)self + 0x5c); }

void func_804CE388(){}

void func_804CE418(){}

void func_804CE4C0(){}

void func_804CE79C(){}

void func_804CE9A4(){}

void func_804CF700(){}

void func_804CFBC8(){}

void func_804D0194(){}

void func_804D01E0(){}

void func_804D0AB4(){}

void func_804D189C(){}

void func_804D20EC(){}

void func_804D2690(){}

void func_804D2B60(){}

void func_804D3098(){}

void func_804D361C(){}

void func_804D3B14(){}

void func_804D3DB0(){}

void func_804D3F60(){}

void func_804D3F94(){}

void func_804D401C(){}

void func_804D4144(){}

void func_804D41F8(){}

void func_804D42B8(){}

void func_804D513C(){}

void func_804D5764(){}

void func_804D5D48(){}

void func_804D5DAC(){}

void func_804D5E10(){}

void func_804D5E90(){}

void func_804D5F54(){}

void func_804D6070(void) {}

void func_804D6074(){}

void func_804D6BC0(){}

// Target 1: us-804d8184 — simple setter: stores two params at offsets 0x10/0x14
void func_804D4010(EffectStruct* self, u32 a, u32 b) {
    self->field_0x10 = a;
    self->field_0x14 = b;
}

// Target 2: us-804d22b4 — if field_0x1c <= 0.0f, clear bit 15 (0x8000) of field_0x06
// SOFT-CAP: MWCC optimizes wrapping mask 0xFFFF7FFF (rlwinm 17,15) to
// non-wrapping 0x00007FFF (rlwinm 17,31) because upper bits are zero after lhz.
// Semantically equivalent — same result for all inputs.
void func_804CE140(EffectStruct* self) {
    if (lbl_eu_8066B0DC < self->field_0x1c) return;
    u32 tmp = self->field_0x06;
    tmp &= ~0x8000;
    self->field_0x06 = (u16)tmp;
}

// Target 3: us-804d1218 — if bits 0x0800 and 0x8000 of field_0x06 are set, add field_0x18 to field_0x10
void func_804CD0A4(EffectStruct* self) {
    u32 flags = self->field_0x06;
    if (!((flags >> 11) & 1)) return;
    if (!((flags >> 15) & 1)) return;
    // field_0x10 is stored as u32 but used as float here
    *(f32*)&self->field_0x10 += self->field_0x18;
}

// Target 4: us-804d0414 — init: set first three fields to -1, rest to 0
void func_804CC2B8(EffectStruct* self) {
    self->field_0x00 = -1;
    self->field_0x02 = -1;
    self->field_0x04 = -1;
    self->field_0x06 = 0;
    self->field_0x328 = 0;
    self->field_0x32c = 0;
    self->field_0x330 = 0;
    self->field_0x334 = 0;
}

// Target 5: us-804d255c — if field_0x328 != 0 and field_0x18 > 0.0f, set bit 6 (0x40) of field_0x06
void func_804CE3E8(EffectStruct* self) {
    if (self->field_0x328 == 0) return;
    if (self->field_0x18 <= lbl_eu_8066B0DC) return;
    self->field_0x06 |= 0x40;
}