#include "types.h"
#include "kyoshin/cf/CfCollAABBImpl.hpp"

extern "C" void __ct__7CDrawGXFv(void* self);
extern "C" void __dt__7CDrawGXFv(void* self, int param);
extern "C" void setCol__7CDrawGXFRCQ22ml5CCol4(void* self, const void* col);
extern "C" void setMatrix__7CDrawGXFRCQ22ml6CMat34(void* self, const void* mat);
extern "C" void renderCube__7CDrawGXFRCQ22ml5CVec3RCQ22ml5CVec3(void* self, const void* min, const void* max);
extern "C" void func_8049034C(void* a, void* b, int c);
extern "C" float SinFIdx__Q24nw4r4mathFf(float x);
extern "C" float CosFIdx__Q24nw4r4mathFf(float x);
extern "C" void func_800A5FE8(void* a, void* b, void* c, void* d, void* e);

extern "C" void* lbl_eu_80663E14;

extern "C" void func_800AAE24(void* r3, void* r4) {
    float angle = *(float*)((char*)r4 + 0x14C);
    float sinVal;
    float cosVal;

    sinVal = SinFIdx__Q24nw4r4mathFf(40.743664f * angle);
    cosVal = CosFIdx__Q24nw4r4mathFf(40.743664f * angle);

    float negSin = -sinVal;

    float mat[12];
    mat[0] = cosVal;
    mat[1] = 0.0f;
    mat[2] = sinVal;
    mat[3] = 0.0f;
    mat[4] = 0.0f;
    mat[5] = 1.0f;
    mat[6] = 0.0f;
    mat[7] = 0.0f;
    mat[8] = negSin;
    mat[9] = 0.0f;
    mat[10] = cosVal;
    mat[11] = 0.0f;

    void** vtbl = *(void***)r4;
    const void* pos = ((void*(*)(void*))vtbl[0xAC / 4])(r4);
    mat[4] = *(float*)((char*)pos + 0x0);
    mat[7] = *(float*)((char*)pos + 0x4);
    mat[10] = *(float*)((char*)pos + 0x8);

    int field94 = *(int*)((char*)r4 + 0x94);
    float lineCol[4];
    if (field94 == 1) {
        lineCol[0] = 0.0f;
        lineCol[1] = 1.0f;
        lineCol[2] = 1.0f;
        lineCol[3] = 0.1f;
    } else {
        lineCol[0] = 0.0f;
        lineCol[1] = 0.5f;
        lineCol[2] = 1.0f;
        lineCol[3] = 0.15f;
    }

    char gx[0xF0];
    __ct__7CDrawGXFv(gx);
    func_8049034C(*(void**)&lbl_eu_80663E14, gx, 0);

    float col[4] = {0.0f, 1.0f, 1.0f, 0.3f};
    setCol__7CDrawGXFRCQ22ml5CCol4(gx, col);
    setMatrix__7CDrawGXFRCQ22ml6CMat34(gx, mat);
    setCol__7CDrawGXFRCQ22ml5CCol4(gx, lineCol);

    renderCube__7CDrawGXFRCQ22ml5CVec3RCQ22ml5CVec3(gx, (char*)r4 + 0xD8, (char*)r4 + 0xE4);
    __dt__7CDrawGXFv(gx, -1);
}

// Render AABB collision shape into a collision query context.
// r4 is the AABB collision shape; r3 (unused) and r5/r6 pass through to func_800A5FE8.
extern "C" void func_800AAFF4(void* r3, cf::CfCollAABBImpl* aabb, void* query, void* result) {
    func_800A5FE8(query, &aabb->mMin, &aabb->mMax, &aabb->mCenter, result);
}
