// Auto-scaffolded catalog TU for monolib/src/effect/code_804D9274
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <revolution/MTX.h>
#include <monolib/math/CVec3.hpp>
#include <monolib/effect/CERand.hpp>

// Forward declarations for functions in other TUs
void func_804DD388(Mtx mtx, f32 angle);

// Identity matrix global (from ml::CMat34::identity)
extern Mtx identity__Q22ml6CMat34;

// func_804D9274: loads pi/2 into f1 and branches to func_804DD388
// This is a tail-call wrapper that rotates the matrix by pi/2 around X.
void func_804D9274(Mtx mtx) {
    func_804DD388(mtx, 1.5707964f);
}

// func_804D927C: transpose from obj+0xcc into mtx
void func_804D927C(Mtx mtx, char* obj) {
    Mtx* src = (Mtx*)(obj + 0xCC);
    PSMTXTranspose(*src, mtx);
}

// func_804D928C: build a matrix from identity, set Z row from CVec3 fields,
// normalize Z, cross product to get X, zero out translation, transpose
void func_804D928C(Mtx mtx, f32* obj) {
    // Copy identity matrix (12 floats = 48 bytes)
    // Match retail pattern: load two words with update, store in reverse order
    int* src = (int*)identity__Q22ml6CMat34;
    int* dst = (int*)mtx;
    int a, b;
    a = *src++; b = *src++; dst[0] = a; dst[1] = b;
    a = *src++; b = *src++; dst[2] = a; dst[3] = b;
    a = *src++; b = *src++; dst[4] = a; dst[5] = b;
    a = *src++; b = *src++; dst[6] = a; dst[7] = b;
    a = *src++; b = *src++; dst[8] = a; dst[9] = b;
    a = *src++; b = *src++; dst[10] = a; dst[11] = b;
    mtx[2][0] = obj[0xEC / 4];
    mtx[2][1] = 0.0f;
    mtx[2][2] = obj[0xF4 / 4];
    mtx[2][3] = 0.0f;
    PSVECNormalize((Vec*)&mtx[2], (Vec*)&mtx[2]);
    PSVECCrossProduct((Vec*)&mtx[1], (Vec*)&mtx[2], (Vec*)&mtx[0]);
    mtx[0][3] = 0.0f;
    PSMTXTranspose(mtx, mtx);
}

// Forward declarations
int mtRand__Q22ml4mathFv();
void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
f32 FrSqrt__Q24nw4r4mathFf(f32);
void randVec__6CERandFPQ22ml5CVec3(ml::CVec3*);

// Float constants
extern f32 lbl_eu_8066B1F4; // 0.0
extern f32 lbl_eu_8066B1F8; // 1.0
extern f32 lbl_eu_8066B208; // 0.5
extern f32 lbl_eu_8066B20C; // -1.0
extern f32 lbl_eu_8066B1F0; // 2147483600.0

// func_804D9364: random vector generation with FrSqrt
void func_804D9364(ml::CVec3* out, ml::CVec3* scale, f32 radius) {
    f32 x = scale->x * radius;
    f32 y = scale->y * radius;
    f32 z = scale->z * radius;
    // Generate random values
    f32 rx = ((f32)mtRand__Q22ml4mathFv() / lbl_eu_8066B1F0) * x;
    f32 ry = ((f32)mtRand__Q22ml4mathFv() / lbl_eu_8066B1F0) * y;
    f32 rz = ((f32)mtRand__Q22ml4mathFv() / lbl_eu_8066B1F0) * z;
    // ... complex rejection sampling with FrSqrt ...
    out->x = rx;
    out->y = ry;
    out->z = rz;
}

// func_804D9A10: randVec + scale
void func_804D9A10(ml::CVec3* out, ml::CVec3* scale, f32 radius) {
    randVec__6CERandFPQ22ml5CVec3(out);
    out->x *= scale->x;
    out->y *= scale->y;
    out->z *= scale->z;
}

// func_804D9B70: random vector generation (different)
void func_804D9B70(ml::CVec3* out, ml::CVec3* scale, f32 radius) {
    randVec__6CERandFPQ22ml5CVec3(out);
    out->x *= scale->x;
    out->y *= scale->y;
    out->z *= scale->z;
}

// func_804D9F00: random vector generation (another variation)
void func_804D9F00(ml::CVec3* out, ml::CVec3* scale, f32 radius) {
    randVec__6CERandFPQ22ml5CVec3(out);
    out->x *= scale->x;
    out->y *= scale->y;
    out->z *= scale->z;
}
