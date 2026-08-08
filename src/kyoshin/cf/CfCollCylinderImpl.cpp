#include "types.h"
#include "kyoshin/cf/CfCollCylinderImpl.hpp"
#include "monolib/math/CCol4.hpp"

void renderCylinder__Q22cf18CfDebugDrawManagerFv(void* self, void* end, ml::CCol4 color, float radius);

// Debug-render a collision cylinder from its impl, colouring it by collision kind.
void func_800AB010(void* self, cf::CfCollCylinderImpl* impl) {
    ml::CCol4 col;

    if (impl->mKind == 1) {
        col.set(0.0f, 1.0f, 1.0f, 0.1f);
    } else if (impl->mKind == 6) {
        col.set(0.0f, 0.7f, 0.7f, 0.1f);
    } else if (impl->mKind == 5) {
        if ((impl->mSubKind & 0xFFFF) == 1) {
            col.set(1.0f, 0.0f, 0.0f, 0.2f);
        } else {
            col.set(0.5f, 0.0f, 1.0f, 0.1f);
        }
    } else if (impl->mKind == 0) {
        col.set(0.3f, 1.0f, 0.5f, 0.1f);
    } else if (impl->mKind == 2) {
        col.set(0.2f, 0.3f, 1.0f, 0.15f);
    } else {
        col.set(0.0f, 0.5f, 1.0f, 0.15f);
    }

    renderCylinder__Q22cf18CfDebugDrawManagerFv(&impl->mStart, &impl->mEnd, col, impl->mRadius);
}

// Push a cylinder segment (starting at mStart, offset by f1 along its axis) into
// the debug draw stack. When f1 is near zero the whole cylinder is submitted;
// otherwise a truncated/shortened segment is drawn.
void func_800AB248(void* self, cf::CfCollCylinderImpl* impl, void* a, void* b, float f1) {
    float heightDiff = impl->mEnd.y - impl->mStart.y;

    if (f1 < lbl_eu_80666940) {
        func_800A5B18(a, &impl->mStart, b, impl->mRadius + f1, heightDiff);
    } else {
        float pos[3];
        pos[0] = impl->mStart.x;
        float adj = lbl_eu_80666944 * f1;
        pos[1] = impl->mStart.y;
        pos[2] = impl->mStart.z;
        pos[1] -= adj;
        func_800A5B18(a, pos, b, impl->mRadius + f1, heightDiff + (f1 - adj));
    }
}