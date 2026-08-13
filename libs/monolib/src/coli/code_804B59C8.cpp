// Auto-scaffolded catalog TU for monolib/src/coli/code_804B59C8
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/os/OSFastCast.h>
#include <nw4r/math/math_types.h>

// ---------------------------------------------------------------------------
// Data globals
// ---------------------------------------------------------------------------

// Scene-resource entry count (sbss; shared with monolib/scn/code_804BD8E8).
extern s32 lbl_eu_80665988;

// Collision state manager (lbl_eu_8065F1C8): reference vectors at 0x00/0x0C,
// sampled half-float triple at 0xC8, result at 0xD4, mover hook at 0xE0,
// acceptance thresholds at 0x120/0x130.
struct CColiMgr {
    nw4r::math::_VEC3 field_0x00;   // 0x00
    nw4r::math::_VEC3 field_0x0C;   // 0x0C
    u8 field_0x18[0x9C - 0x18];     // 0x18
    void* field_0x9C;               // 0x9C
    void* field_0xA0;               // 0xA0
    u8 field_0xA4[0xC8 - 0xA4];     // 0xA4
    nw4r::math::_VEC3 field_0xC8;   // 0xC8
    f32 field_0xD4;                 // 0xD4
    void* field_0xE0;               // 0xE0
    u32 field_0xEC;                 // 0xEC
    u8 field_0xF0[0x120 - 0xF0];    // 0xF0
    f32 field_0x120;                // 0x120
    u32 field_0x130;                // 0x130
};
extern CColiMgr lbl_eu_8065F1C8;

// Acceptance-threshold constant (sdata2).
extern const f32 lbl_eu_8066AED0;

// 0x24-byte scene-resource entry (lbl_eu_8065F428 holds 32 of these).
struct ScnResourceEntry {
    u32 field_0x00;   // 0x00
    u32 field_0x04;   // 0x04
    u32 field_0x08;   // 0x08
    u32 field_0x0C;   // 0x0C
    u32 field_0x10;   // 0x10
    u32 field_0x14;   // 0x14
    void* field_0x18; // 0x18
    u32 field_0x1C;   // 0x1C
    int field_0x20;   // 0x20
};
extern ScnResourceEntry lbl_eu_8065F428[32];

// Object owning a 6-byte half-float triple table at +0x1C.
struct CColiSrc {
    u8 field_0x00[0x1C];  // 0x00
    void* field_0x1C;     // 0x1C
};

// Query/mover object: position at +0x00, half-float table index at +0x10.
struct CColiMover {
    f32 field_0x00;             // 0x00
    u8 field_0x04[0x10 - 0x04]; // 0x04
    u16 field_0x10;             // 0x10
};

// Movement-tracked mover: previous position +0x0C, saved copy +0x38,
// current position +0x50, distance-squared threshold +0x5C, flag +0x60.
struct CTrackedMover {
    u8 field_0x00[0x0C];         // 0x00
    f32 field_0x0C;              // 0x0C
    f32 field_0x10;              // 0x10
    f32 field_0x14;              // 0x14
    f32 field_0x18;              // 0x18
    f32 field_0x1C;              // 0x1C
    f32 field_0x20;              // 0x20
    u8 field_0x24[0x38 - 0x24];  // 0x24
    f32 field_0x38;              // 0x38
    f32 field_0x3C;              // 0x3C
    f32 field_0x40;              // 0x40
    f32 field_0x44;              // 0x44
    f32 field_0x48;              // 0x48
    f32 field_0x4C;              // 0x4C
    f32 field_0x50;              // 0x50
    f32 field_0x54;              // 0x54
    f32 field_0x58;              // 0x58
    f32 field_0x5C;              // 0x5C
    u8 field_0x60;               // 0x60
};

// Collision-register callbacks (retail symbols in this TU).
void func_804B708C(CColiSrc* a, CColiMover* b);
void func_804B71CC(CColiSrc* a, CColiMover* b);

// func_804B59C8 - track mover movement. When the squared distance between
// the current position (0x50) and the previous position (0x0C) is at least
// the stored threshold (0x5C), refresh the threshold and save the previous
// position block (0x0C..0x20) to 0x38..0x4C. First move always registers.
void func_804B59C8(CTrackedMover* self) {
    nw4r::math::VEC3 delta;
    nw4r::math::VEC3Sub(&delta, (const nw4r::math::VEC3*)&self->field_0x50,
                        (const nw4r::math::VEC3*)&self->field_0x0C);
    f32 dist2 = nw4r::math::VEC3LenSq(&delta);
    if (self->field_0x60 == 0) {
        self->field_0x60 = 1;
    } else if (dist2 >= self->field_0x5C) {
        return;
    }
    self->field_0x5C = dist2;
    self->field_0x38 = self->field_0x0C;
    self->field_0x3C = self->field_0x10;
    self->field_0x40 = self->field_0x14;
    self->field_0x44 = self->field_0x18;
    self->field_0x48 = self->field_0x1C;
    self->field_0x4C = self->field_0x20;
}

u32 func_804B5A68(void) {
    extern u32 lbl_eu_80663AC8;
    return lbl_eu_80663AC8;
}

extern char lbl_eu_8065F1A0[];
extern "C" void* func_804B5A70(void){ return (void*)&lbl_eu_8065F1A0; }

void __dt__804B5A7C(){}

void __dt__804B5AD0(){}

void func_804B5B34(void) {}

void func_804B5B38(){}

void func_804B5BB0(){}

// func_804B5C58 - sample the half-float triple for the mover's table index,
// then ray-cast against the manager's reference vectors: t is the distance
// along the direction vector (0x0C) from the offset plane (0x00) to the
// mover. Register the mover when the hit is in front.
void func_804B5C58(CColiSrc* a, CColiMover* b) {
    u16 idx = b->field_0x10;
    s16* tri = (s16*)((u8*)a->field_0x1C + idx * 6);
    lbl_eu_8065F1C8.field_0xC8.x = __OSs16tof32(&tri[0]);
    lbl_eu_8065F1C8.field_0xC8.y = __OSs16tof32(&tri[1]);
    lbl_eu_8065F1C8.field_0xC8.z = __OSs16tof32(&tri[2]);
    f32 dot2 = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                   (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x0C);
    if (dot2 == lbl_eu_8066AED0) return;
    f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                  (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x00);
    f32 t = (b->field_0x00 - dot) / dot2;
    lbl_eu_8065F1C8.field_0xD4 = t;
    if (t <= lbl_eu_8066AED0) return;
    lbl_eu_8065F1C8.field_0xE0 = b;
    func_804B708C(a, b);
}

void func_804B5CEC(){}

// func_804B5DE8 - sample the half-float triple for the mover's table index,
// then check the offset of the mover from the plane defined by the sampled
// normal (0xC8) and the offset vector (0x00). Register the mover when the
// offset lands in the acceptance window [const, 0x120).
void func_804B5DE8(CColiSrc* a, CColiMover* b) {
    u16 idx = b->field_0x10;
    s16* tri = (s16*)((u8*)a->field_0x1C + idx * 6);
    lbl_eu_8065F1C8.field_0xC8.x = __OSs16tof32(&tri[0]);
    lbl_eu_8065F1C8.field_0xC8.y = __OSs16tof32(&tri[1]);
    lbl_eu_8065F1C8.field_0xC8.z = __OSs16tof32(&tri[2]);
    f32 dot = nw4r::math::VEC3Dot((const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0xC8,
                                  (const nw4r::math::VEC3*)&lbl_eu_8065F1C8.field_0x00);
    f32 diff = dot - b->field_0x00;
    lbl_eu_8065F1C8.field_0xD4 = diff;
    if (lbl_eu_8066AED0 > diff) return;
    if (diff >= lbl_eu_8065F1C8.field_0x120) return;
    lbl_eu_8065F1C8.field_0xE0 = b;
    func_804B71CC(a, b);
}

void func_804B5E68(){}

void func_804B61F8(void) {}

void func_804B61FC(){}

void func_804B62B0(){}

void func_804B6364(){}

void func_804B6418(){}

void func_804B64CC(){}

void func_804B6580(){}

void func_804B6634(void) {}

// func_804B6638 / func_804B66B0 - append a new scene-resource entry to the
// 32-slot table (lbl_eu_8065F428): copy the 12-byte header and info block
// from the arguments and fill the manager-owned fields from lbl_eu_8065F1C8.
void func_804B6638(const u32* head, const u32* info) {
    s32 count = lbl_eu_80665988;
    if (count >= 0x20) return;
    lbl_eu_8065F428[count].field_0x00 = head[0];
    lbl_eu_8065F428[count].field_0x04 = head[1];
    lbl_eu_8065F428[count].field_0x08 = head[2];
    lbl_eu_8065F428[count].field_0x0C = info[0];
    lbl_eu_8065F428[count].field_0x10 = info[1];
    lbl_eu_8065F428[count].field_0x14 = info[2];
    lbl_eu_8065F428[count].field_0x18 = lbl_eu_8065F1C8.field_0xE0;
    lbl_eu_8065F428[count].field_0x1C = lbl_eu_8065F1C8.field_0xEC;
    lbl_eu_8065F428[count].field_0x20 = lbl_eu_8065F1C8.field_0x130;
    lbl_eu_80665988 = count + 1;
}

void func_804B66B0(const u32* head, const u32* info) {
    s32 count = lbl_eu_80665988;
    if (count >= 0x20) return;
    lbl_eu_8065F428[count].field_0x00 = head[0];
    lbl_eu_8065F428[count].field_0x04 = head[1];
    lbl_eu_8065F428[count].field_0x08 = head[2];
    lbl_eu_8065F428[count].field_0x0C = info[0];
    lbl_eu_8065F428[count].field_0x10 = info[1];
    lbl_eu_8065F428[count].field_0x14 = info[2];
    lbl_eu_8065F428[count].field_0x18 = lbl_eu_8065F1C8.field_0xE0;
    lbl_eu_8065F428[count].field_0x1C = lbl_eu_8065F1C8.field_0xEC;
    lbl_eu_8065F428[count].field_0x20 = lbl_eu_8065F1C8.field_0x130;
    lbl_eu_80665988 = count + 1;
}

void func_804B6728(){}

void func_804B684C(){}

// Invoke the manager's callback slot with the stored context pointer.
void func_804B6954(void* a, void* b, void* c) {
    void (*fn)(void*, void*, void*) = (void (*)(void*, void*, void*))lbl_eu_8065F1C8.field_0xA0;
    void* p = lbl_eu_8065F1C8.field_0x9C;
    fn(b, c, p);
}

void func_804B6974(){}

void func_804B6A3C(){}

void func_804B6C48(){}

void func_804B6D30(){}

void func_804B6F58(){}

extern "C" u32 func_804B7044(u8* self, float f) {
    u32 r = 0;
    if (f <= *(float*)((u8*)self + 0x64) && f >= *(float*)((u8*)self + 0x68)) r = 1;
    return r;
}

extern "C" u32 func_804B7074(u8* self, float f) { return f <= *(float*)((u8*)self + 0x64); }

void func_804B708C(CColiSrc* a, CColiMover* b){}

void func_804B71CC(CColiSrc* a, CColiMover* b){}

void func_804B74F0(){}

void func_804B7540(){}

void func_804B7804(){}

void func_804B791C(){}

void func_804B7944(){}

void func_804B7A00(){}

void func_804B7AC8(void) {}

void func_804B7ACC(){}

void func_804B7B6C(){}

extern char lbl_eu_8065F1A0[];
void func_804B7D9C(int, int bit_index) {
    if (bit_index > 0x140) bit_index = 0x140;
    int word_idx = bit_index >> 5;
    int bit_off = bit_index & 0x1F;
    int mask = 1 << bit_off;
    int* bits = (int*)&lbl_eu_8065F1A0;
    bits[word_idx] &= ~mask;
}

void func_804B7DD4(){}

void func_804B7E0C(){}

void func_804B8078(){}

void func_804B80A4(){}

void func_804B80CC(){}

void func_804B8108(){}

void func_804B85C8(){}

void func_804B877C(){}

void func_804B8C2C(){}

void func_804B91E0(){}

void func_804B9818(){}

void func_804B9E14(){}

void func_804BA26C(){}

void func_804BA7BC(){}

void func_804BA8B4(){}

extern "C" u32 func_804BADA0(u8* self) { return *(u32*)((u8*)self + 0x5C) != 0; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_804BADB4(){}
