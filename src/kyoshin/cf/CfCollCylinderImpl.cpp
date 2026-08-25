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
    //
    // Colour components go through named float locals: MWCC colours the FPRs
    // by local declaration order (low -> high) while emitting the sdata2
    // loads in assignment order, which is what fixes the retail register
    // assignment (components allocated high-to-low).
    if (impl->mKind == 1) {
        float ca, cg, cr;
        cg = lbl_eu_80666914;
        cr = lbl_eu_80666910;
        ca = lbl_eu_8066692C;
        col = ml::CCol4(cr, cg, cg, ca);
    }
    if (impl->mKind == 6) {
        float ca, cg, cr;
        cg = lbl_eu_80666938;
        cr = lbl_eu_80666910;
        ca = lbl_eu_8066692C;
        col = ml::CCol4(cr, cg, cg, ca);
    } else if (impl->mKind == 5) {
        if ((impl->mSubKind & 0xFFFF) == 1) {
            float ca, cg, cr;
            cg = lbl_eu_80666910;
            cr = lbl_eu_80666914;
            ca = lbl_eu_8066693C;
            col = ml::CCol4(cr, cg, cg, ca);
        } else {
            float ca, cb, cg, cr;
            cr = lbl_eu_80666930;
            cg = lbl_eu_80666910;
            cb = lbl_eu_80666914;
            ca = lbl_eu_8066692C;
            col = ml::CCol4(cr, cg, cb, ca);
        }
    } else if (impl->mKind == 0) {
        float ca, cb, cg, cr;
        cr = lbl_eu_80666918;
        cg = lbl_eu_80666914;
        cb = lbl_eu_80666930;
        ca = lbl_eu_8066692C;
        col = ml::CCol4(cr, cg, cb, ca);
    } else if (impl->mKind == 2) {
        float ca, cb, cg, cr;
        cr = lbl_eu_8066693C;
        cg = lbl_eu_80666918;
        cb = lbl_eu_80666914;
        ca = lbl_eu_80666934;
        col = ml::CCol4(cr, cg, cb, ca);
    } else {
        float ca, cb, cg, cr;
        cr = lbl_eu_80666910;
        cg = lbl_eu_80666930;
        cb = lbl_eu_80666914;
        ca = lbl_eu_80666934;
        col = ml::CCol4(cr, cg, cb, ca);
    }

    renderCylinder__Q22cf18CfDebugDrawManagerFv(&impl->mStart, &impl->mEnd, &col, impl->mRadius);
}

// Push a cylinder segment (starting at mStart, offset by f1 along its axis) into
// Push a cylinder segment (starting at mStart, offset by f1 along its axis)
// against the collision object and write the pushed-out point to out. When f1
// is near zero the whole cylinder is submitted; otherwise a truncated segment
// starting below mStart is drawn.
void func_800AB248(void* self, cf::CfCollCylinderImpl* impl, void* a, void* b, float f1) {
    // Axis height span, used to clamp the probe point along Y.
    float heightDiff = impl->mEnd.y - impl->mStart.y;

    if (f1 < lbl_eu_80666940) {
        // Whole cylinder: probe straight from mStart with radius grown by f1.
        func_800A5B18(a, &impl->mStart, b, impl->mRadius + f1, heightDiff);
    } else {
        // Truncated segment: drop the probe origin by 0.4*f1 and extend the
        // height span by what remains (f1 - 0.4*f1).
        float pos[3];
        pos[0] = impl->mStart.x;
        pos[1] = impl->mStart.y;
        float adj = lbl_eu_80666944 * f1;
        pos[2] = impl->mStart.z;
        pos[1] -= adj;
        func_800A5B18(a, pos, b, impl->mRadius + f1, heightDiff + (f1 - adj));
    }
}