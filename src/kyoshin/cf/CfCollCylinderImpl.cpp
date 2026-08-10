#include "types.h"
#include "kyoshin/cf/CfCollCylinderImpl.hpp"
#include "monolib/math/CCol4.hpp"

// Debug-render a collision cylinder from its impl, colouring it by collision kind.
// Colour chosen per collision kind (see retail .sdata2 constants).
void func_800AB010(void* self, cf::CfCollCylinderImpl* impl) {
    ml::CCol4 col;

    // Case 1 is a standalone `if` (not part of the else-chain below): retail
    // falls through past it, re-loading mKind for the 6/5/0/2 chain (so a
    // kind==1 hull ends up coloured by the default else). The else-chain then
    // binds to case 6, giving each matched body its own branch-to-tail.
    if (impl->mKind == 1) {
        col = ml::CCol4(lbl_eu_80666910, lbl_eu_80666914, lbl_eu_80666914, lbl_eu_8066692C);
    }
    if (impl->mKind == 6) {
        col = ml::CCol4(lbl_eu_80666910, lbl_eu_80666938, lbl_eu_80666938, lbl_eu_8066692C);
    } else if (impl->mKind == 5) {
        if ((impl->mSubKind & 0xFFFF) == 1) {
            col = ml::CCol4(lbl_eu_80666910, lbl_eu_80666914, lbl_eu_80666914, lbl_eu_8066693C);
        } else {
            col = ml::CCol4(lbl_eu_80666930, lbl_eu_80666910, lbl_eu_80666914, lbl_eu_8066692C);
        }
    } else if (impl->mKind == 0) {
        col = ml::CCol4(lbl_eu_80666918, lbl_eu_80666914, lbl_eu_80666930, lbl_eu_8066692C);
    } else if (impl->mKind == 2) {
        col = ml::CCol4(lbl_eu_8066693C, lbl_eu_80666918, lbl_eu_80666914, lbl_eu_80666934);
    } else {
        col = ml::CCol4(lbl_eu_80666910, lbl_eu_80666930, lbl_eu_80666914, lbl_eu_80666934);
    }

    renderCylinder__Q22cf18CfDebugDrawManagerFv(&impl->mStart, &impl->mEnd, &col, impl->mRadius);
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