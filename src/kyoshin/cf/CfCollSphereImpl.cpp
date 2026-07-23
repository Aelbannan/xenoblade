#include "types.h"

// renderSphere: member of cf::CfDebugDrawManager.
// Stub declares Fv but retail passes a float in f1.
extern "C" void renderSphere__Q22cf18CfDebugDrawManagerFv(void* self, float val);

// func_800A5738: defined in kyoshin/code_800A3B24.
extern "C" void func_800A5738(void* a, void* b, float val, void* c);

// func_800AAD28: debug draw for sphere collision shape.
// Reads field at shape+0xB8, converts to unsigned, calls shape's vfunc at
// vtable offset 0xAC to get the draw manager, then calls renderSphere.
extern "C" void func_800AAD28(void* r3, void* r4) {
    float fval = *(float*)((char*)r4 + 0xB8);
    unsigned int uval = (unsigned int)fval;
    void** vtbl = *(void***)r4;
    void* mgr = ((void*(*)(void*))vtbl[0xAC / 4])(r4);
    float result = (float)uval;
    renderSphere__Q22cf18CfDebugDrawManagerFv(mgr, result);
}

// func_800AAD94: same sphere debug draw but passes extra parameters through
// to func_800A5738.
extern "C" void func_800AAD94(void* r3, void* r4, void* r5, void* r6) {
    float fval = *(float*)((char*)r4 + 0xB8);
    unsigned int uval = (unsigned int)fval;
    void** vtbl = *(void***)r4;
    void* mgr = ((void*(*)(void*))vtbl[0xAC / 4])(r4);
    float result = (float)uval;
    func_800A5738(r5, mgr, result, r6);
}
