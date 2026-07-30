// Auto-scaffolded catalog TU for monolib/src/effect/code_804DB938
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/MTX.h>

void func_804DB980(){}

void func_804DCA88(){}

void func_804DCD94(){}

void func_804DD0A0(){}

// Sin/cos lookup tables (360 entries, each 8 bytes: {value, delta})
extern f32 lbl_eu_80660038[]; // sin table
extern f32 lbl_eu_80660B78[]; // cos table
extern f32 lbl_eu_8066B260;   // 57.29578 (180/pi)

// Helper: compute sin/cos from angle using lookup tables
static void getSinCos(f32 angle, f32* sin_out, f32* cos_out) {
    f32 deg = angle * lbl_eu_8066B260;
    int idx = ((int)deg) % 360;
    if (idx < 0) idx += 360;
    f32 frac = deg - (f32)(int)deg;
    *sin_out = lbl_eu_80660038[idx * 2] + frac * lbl_eu_80660038[idx * 2 + 1];
    *cos_out = lbl_eu_80660B78[idx * 2] + frac * lbl_eu_80660B78[idx * 2 + 1];
}

void func_804DD388(Mtx mtx, f32 angle) {
    f32 sin_val, cos_val;
    getSinCos(angle, &sin_val, &cos_val);
    PSMTXRotTrig(mtx, sin_val, cos_val, 'x');
}

void func_804DD440(Mtx mtx, f32 angle) {
    f32 sin_val, cos_val;
    getSinCos(angle, &sin_val, &cos_val);
    PSMTXRotTrig(mtx, sin_val, cos_val, 'y');
}

void func_804DD4F8(Mtx mtx, f32 angle) {
    f32 sin_val, cos_val;
    getSinCos(angle, &sin_val, &cos_val);
    PSMTXRotTrig(mtx, sin_val, cos_val, 'z');
}

void func_804DD5B0(){}

// func_804DD6E8: simple switch/lookup function
// Maps input 0-5 to output: 5, 3, 4, 1, 2, 0
int func_804DD6E8(int val) {
    switch (val) {
        case 0: return 5;
        case 1: return 3;
        case 2: return 4;
        case 3: return 1;
        case 4: return 2;
        case 5: return 0;
        default: return 0;
    }
}

void func_804DD754(){}

// func_804DD89C: vector lerp using paired-single operations
// out = a + (b - a) * t  (only x and z components)
void func_804DD89C(Vec* out, Vec* a, Vec* b, f32 t) {
    out->x = a->x + (b->x - a->x) * t;
    out->z = a->z + (b->z - a->z) * t;
}

// func_804DD8C8: vector lerp using paired-single operations
// out = a + (b - a) * t  (only x and z components)
void func_804DD8C8(Vec* out, Vec* a, Vec* b, f32 t) {
    out->x = a->x + (b->x - a->x) * t;
    out->z = a->z + (b->z - a->z) * t;
}
