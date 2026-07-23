#include "types.h"

extern "C" void __ct__7CDrawGXFv(void* self);
extern "C" void __dt__7CDrawGXFv(void* self, int param);
extern "C" void setCol__7CDrawGXFRCQ22ml5CCol4(void* self, const void* col);
extern "C" void renderCircle__7CDrawGXFRCQ22ml5CVec3if(void* self, const void* pos, int verts, float r);
extern "C" void func_8049034C(void* a, void* b, int c);
extern "C" void func_800A50AC(void* a, void* b, float c, void* d);

extern "C" void* lbl_eu_80663E14;

extern "C" void func_800AABD4(void* r3, void* r4) {
    char gx[0xF0];
    __ct__7CDrawGXFv(gx);
    func_8049034C(lbl_eu_80663E14, gx, 0);
    float col[4];
    col[0] = 0.0f;
    col[1] = 1.0f;
    col[2] = 1.0f;
    col[3] = 0.3f;
    setCol__7CDrawGXFRCQ22ml5CCol4(gx, col);
    float fval = *(float*)((char*)r4 + 0xB8);
    unsigned int uval = (unsigned int)fval;
    void** vtbl = *(void***)r4;
    const void* pos = ((void*(*)(void*))vtbl[0xAC / 4])(r4);
    float result = (float)uval;
    renderCircle__7CDrawGXFRCQ22ml5CVec3if(gx, pos, 0x24, result);
    __dt__7CDrawGXFv(gx, -1);
}

extern "C" void func_800AAC98(void* r3, void* r4, void* r5, void* r6) {
    float fval = *(float*)((char*)r4 + 0xB8);
    unsigned int uval = (unsigned int)fval;
    void** vtbl = *(void***)r4;
    void* mgr = ((void*(*)(void*))vtbl[0xAC / 4])(r4);
    float result = (float)uval;
    func_800A50AC(r5, mgr, result, r6);
}
