// Auto-scaffolded catalog TU for monolib/src/scn/CMdlDynamics
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

#include <nw4r/math/math_types.h>
#include <nw4r/db/db_assert.h>

// --- Cross-TU retail data imports (global scope: plain extern, no extern "C") ---
extern const char lbl_eu_80530D54[]; // panic file name
// --- panic message ---
extern const char lbl_eu_80530D2C[];

// CMdlDynamics retail vtable (3 entries, 0xC bytes) lives in retail .data.
struct CMdlDynamicsVtbl {
    u32 entry0;
    u32 entry1;
    u32 entry2;
};
extern CMdlDynamicsVtbl lbl_eu_805701FC;

class CMdlDynamics {
public:
    CMdlDynamics();

    CMdlDynamicsVtbl* vtbl; // 0x0
    u32 field_0x4;          // 0x4
    u32 field_0x8;          // 0x8
    u32 field_0xC;          // 0xC
    u32 field_0x10;         // 0x10 (not set by ctor)
    u32 field_0x14;         // 0x14
    u32 field_0x18;         // 0x18
    u32 field_0x1C;         // 0x1C
    u32 field_0x20;         // 0x20 (not set by ctor)
    u32 field_0x24;         // 0x24
};

CMdlDynamics::CMdlDynamics() {
    vtbl = &lbl_eu_805701FC;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xC = 0;
    field_0x14 = -1;
    field_0x18 = 0;
    field_0x1C = 0;
    field_0x24 = -1;
}

// func_804E9FC8 (inline getter at 0x804E9FC8) defined once - all harness stubs
// mapping to the same symbol reference it rather than redefining.
u32 func_804E9FC8(u8* self) { return *(u32*)((u8*)self + 0x146c); }

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

// (mapped to func_804E9FC8 above)

extern "C" void func_804EA0D4(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

u32 func_804EA0E0(u8* self) { return *(u32*)((u8*)self + 0x4); }

// (mapped to func_804E9FC8 above)

void __dt__804EA1D0(){}

u32 func_804EA268(u8* self) { return *(u32*)((u8*)self + 0x0); }

extern "C" void* func_804EA270(u8* self) { return (char*)*(void**)self + (*(u32*)((u8*)self + 4) << 2); }

void func_804EA284(){}

// Thin wrappers over the SDK matrix functions (header-declared via <revolution/MTX.h>).
extern "C" void func_804EAA10(const Mtx a, Mtx b) {
    PSMTXInverse(a, b);
}

void func_804EAA14(void) {}

void func_804EAA18(){}

extern "C" u32 func_804EB1C4(u8* self) { return *(u32*)self != 0; }

// Stores a value at self+0 and asserts it is 4-byte aligned (retail panics
// with the same file/line/message as the nw4r ResDic alignment checks).
u8* func_804EB1D8(u8* self, u32 arg) {
    *(u32*)self = arg;
    if ((arg & 3) != 0) {
        nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
    }
    return self;
}

void func_804EB22C(){}

extern "C" u32 func_804EB2FC(u8* self) { return *(u32*)self != 0; }

void func_804EB310(){}

void func_804EB3E8(){}

void func_804EB4C0(){}

extern "C" void func_804EB524(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

extern "C" void func_804EB530(u8* self) {
    u8* s = (u8*)self;
    *(u32*)(s + 0x8C) = 0;
    *(u32*)(s + 0x90) = 0;
    *(u32*)(s + 0x94) = 0;
    *(u32*)(s + 0x9C) = -1;
    *(u32*)(s + 0xA0) = 0;
    *(u32*)(s + 0xA4) = 0;
    *(u32*)(s + 0xAC) = -1;
}

extern "C" void func_804EB558(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

void func_804EB564(){}

// Returns a pointer to element idx of a 0x98-byte-stride array stored at self+0.
u8* func_804EB6E0(u8* self, u32 idx) {
    return *(u8**)self + idx * 0x98;
}

u32 func_804EB6F0(u8* self) { return *(u32*)((u8*)self + 0x4); }

// Returns a pointer to element idx of a 0xB0-byte-stride array stored at self+0.
u8* func_804EB6F8(u8* self, u32 idx) {
    return *(u8**)self + idx * 0xB0;
}

extern "C" u32 func_804EB708(u8* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0) = a;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = b;
    return b << 2;
}

extern "C" void func_804EB720(u8* self, const void* src) {
    u32 idx = *(u32*)((u8*)self + 4);
    *(u32*)(*(u8**)self + idx * 4) = *(u32*)src;
    *(u32*)((u8*)self + 4) = idx + 1;
}

extern "C" u32 func_804EB740(u8* self, u32 a, u32 b) {
    *(u32*)((u8*)self + 0) = a;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = b;
    return b * 12;
}

extern "C" void func_804EB758(u8* self, const void* src) { *(u32*)self = *(const u32*)src; }

void func_804EB764(void *r3, void *r4) {
    int *p = (int *)r3;
    int *src = (int *)r4;
    int idx = p[1];
    int base = p[0];
    int off = idx * 12;
    p[1] = idx + 1;
    int *dst = (int *)(base + off);
    dst[0] = src[0];
    float *fdst = (float *)dst;
    float *fsrc = (float *)src;
    fdst[1] = fsrc[1];
    dst[2] = src[2];
}

void func_804EB798(){}

void func_804EB7F8(){}

void func_804EB8A0(){}

void func_804EBAE8(){}

u32 func_804EBBC4(u8* self) { return *(u32*)((u8*)self + 0x7e8); }

void func_804EBBCC(){}

extern "C" u32 func_804EC32C(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 3) & 1; }

extern "C" u32 func_804EC338(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 19) & 1; }

void func_804EC344(){}

extern "C" u32 func_804EC3AC(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 20) & 1; }

extern "C" u32 func_804EC3B8(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 7) & 1; }

extern "C" u32 func_804EC3C4(u8* self) { return (*(u32*)((u8*)self + 0x7A4) >> 17) & 1; }

extern "C" void func_804EC3D0(u8* self, void* out) {
    const float c = *(float*)((u8*)self + 0x2C);
    const float b = *(float*)((u8*)self + 0x1C);
    const float a = *(float*)((u8*)self + 0x0C);
    *(float*)((u8*)out + 0) = a;
    *(float*)((u8*)out + 4) = b;
    *(float*)((u8*)out + 8) = c;
}

extern "C" void func_804EC3EC(u8* self, float a, float b, float c) {
    *(float*)((u8*)self + 0) = a;
    *(float*)((u8*)self + 4) = b;
    *(float*)((u8*)self + 8) = c;
}

extern "C" void func_804EC3FC(u8* self, const void* src) {
    *(float*)((u8*)self + 0) = *(float*)((u8*)src + 0);
    *(float*)((u8*)self + 4) = *(float*)((u8*)src + 4);
    *(float*)((u8*)self + 8) = *(float*)((u8*)src + 8);
}

void func_804EC418(){}

extern "C" void func_804EC478(Mtx a, const Quaternion* b) {
    PSMTXQuat(a, b);
}

void func_804EC47C(){}

void func_804EC514(){}

// Converts a radian rotation vector to rotation indices and builds the XYZ
// rotation matrix (retail tail-call; 128/pi scale pooled at lbl_eu_8066B3F0).
nw4r::math::MTX34* func_804EC76C(nw4r::math::MTX34* mtx, const f32* v) {
    return nw4r::math::MTX34RotXYZRad(mtx, v[0], v[1], v[2]);
}

void func_804EC78C(){}

void func_804EC81C(){}

void func_804EC8AC(){}

u32 func_804EC9E4(u8* self) { return *(u32*)((u8*)self + 0x0); }

extern "C" void* func_804EC9EC(u8* self) { return (char*)*(void**)self + *(u32*)((u8*)self + 4) * 0x98; }

void func_804ECA00(){}

void func_804ECAC4(){}

void func_804ECEB4(){}

void func_804ED18C(){}

void func_804ED67C(){}

extern "C" void func_804E95E0() {}
extern "C" void func_804E9FD0() {}
extern "C" void func_804EA038() {}
