#pragma once

#include <types.h>

// Layer-view descriptor consumed by func_8047C040: one 8-byte record per
// outer-loop iteration (retail advances the pointer by 8 each iteration).
struct UnkLayerView {
    u16 field_0x0;   // +0x00 layer entry index into obj->field_0x10
    u16 field_0x2;   // +0x02 element count (inner loop bound)
    u32 field_0x4;   // +0x04 element base index (x 0xC into obj->field_0xC)
};

// Tail of the embedded UnkClass_80471EC8 subobject (this+0x24) holding the
// two per-frame culling constants read by func_8047C040.
struct EmbeddedCullParams {
    u8 pad_0x0[0x2DFC];
    f32 field_0x2DFC;   // +0x2DFC
    f32 field_0x2E00;   // +0x2E00
};

// MPF draw-object vtable mirrors for the two-argument mark-flush virtuals:
// path C flushes via vtable slot 0 (+0x8), paths A/B via slot 1 (+0xC).
struct MPFDrawVt8_2 {
    virtual void vf0(void* arg, u32* slot) = 0;
};
struct MPFDrawVtC_2 {
    virtual void vf0(void* arg, u32* slot) = 0;
    virtual void vf1(void* arg, u32* slot) = 0;
};

// .sdata2 constants / bitmask-table pointer / warning strings referenced by
// func_8047C040 (retail names kept so the relocs match).
extern const f32 lbl_eu_8066A860;
extern const f32 lbl_eu_8066A864;
extern const f32 lbl_eu_8066A868;
extern const f32 lbl_eu_80665880;
extern const f32 lbl_eu_80665884;
extern const f32 lbl_eu_806638A0;
extern u32* lbl_eu_80665864;
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

class UnkClass_8047BB54 {
public:
    void initMpfSystem();
    void checkMpfFlags();
    void resetMpfInstance();
    void initMpfDrawBuffer();
    void getMpfStatus();
    void func_8047C040();
    void func_8047C588();
    void func_8047C904();

    // TODO: add fields
    void setMpfFloatParam();
    void forwardMpfCallA();
    void forwardMpfCallB();
    void forwardMpfCallC();
    void flushMpfBuffer();
};
