#include "types.h"
#include "kyoshin/cf/CfCollCylinderImpl.hpp"

void renderCylinder__Q22cf18CfDebugDrawManagerFv(void* this_ptr, void* r4, void* r5, float f1);
void func_800A5B18(void* a, void* b, void* c, float f1, float f2);

void func_800AB010(void* r3, void* r4){
    int field94 = *(int*)((char*)r4 + 0x94);
    float col[4];

    if (field94 == 1) {
        col[0] = 0.0f;
        col[1] = 1.0f;
        col[2] = 1.0f;
        col[3] = 0.1f;
    } else if (field94 == 6) {
        col[0] = 0.0f;
        col[1] = 0.7f;
        col[2] = 0.7f;
        col[3] = 0.1f;
    } else if (field94 == 5) {
        unsigned short field9c = (unsigned short)*(int*)((char*)r4 + 0x9C);
        if (field9c == 1) {
            col[0] = 1.0f;
            col[1] = 0.0f;
            col[2] = 0.0f;
            col[3] = 0.2f;
        } else {
            col[0] = 0.0f;
            col[1] = 0.5f;
            col[2] = 1.0f;
            col[3] = 0.1f;
        }
    } else if (field94 == 0) {
        col[0] = 0.3f;
        col[1] = 1.0f;
        col[2] = 0.5f;
        col[3] = 0.1f;
    } else if (field94 == 2) {
        col[0] = 0.2f;
        col[1] = 0.3f;
        col[2] = 1.0f;
        col[3] = 0.15f;
    } else {
        col[0] = 0.0f;
        col[1] = 0.5f;
        col[2] = 1.0f;
        col[3] = 0.15f;
    }

    float f1 = *(float*)((char*)r4 + 0xB8);
    renderCylinder__Q22cf18CfDebugDrawManagerFv(
        (char*)r4 + 0xA0,
        (char*)r4 + 0xAC,
        col,
        f1
    );
}

void func_800AB248(void* r3, void* r4, void* r5, void* r6, float f1){
    float threshold = 0.01f;
    cf::CfCollCylinderImpl* impl = static_cast<cf::CfCollCylinderImpl*>(r4);
    float heightDiff = impl->mEnd.y - impl->mStart.y;

    if (f1 < threshold) {
        func_800A5B18(r5, &impl->mStart, r6, impl->mRadius + f1, heightDiff);
    } else {
        float pos[3];
        pos[0] = impl->mStart.x;
        float adj = 0.4f * f1;
        pos[1] = impl->mStart.y - adj;
        pos[2] = impl->mStart.z;
        float extra = f1 - adj;
        func_800A5B18(r5, pos, r6, impl->mRadius + f1, heightDiff + extra);
    }
}
