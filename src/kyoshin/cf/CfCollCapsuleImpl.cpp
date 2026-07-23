#include "types.h"

extern "C" void renderCapsule__Q22cf18CfDebugDrawManagerFv(void* this_ptr, void* r4, void* r5, float f1);
extern "C" void func_800A4C48(void* a, void* b, void* c, float f1);

extern "C" void func_800AB2E4(void* r3, void* r4) {
    float col[4] = {0.0f, 0.5f, 1.0f, 0.15f};
    int field94 = *(int*)((char*)r4 + 0x94);

    if (field94 == 1) {
        col[0] = 0.0f;
        col[1] = 1.0f;
        col[2] = 1.0f;
        col[3] = 0.1f;
    } else if (field94 == 6) {
        col[0] = 0.0f;
        col[1] = 0.75f;
        col[2] = 0.75f;
        col[3] = 0.1f;
    }

    float f1 = *(float*)((char*)r4 + 0xB8);
    renderCapsule__Q22cf18CfDebugDrawManagerFv(
        (char*)r4 + 0xA0,
        (char*)r4 + 0xAC,
        col,
        f1
    );
}

extern "C" void func_800AB3B8(void* r3, void* r4, void* r5, float f1) {
    func_800A4C48(r5, (char*)r4 + 0xA0, (char*)r4 + 0xAC, *(float*)((char*)r4 + 0xB8) + f1);
}
