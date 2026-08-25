#include <nw4r/g3d.h>
#include <nw4r/math.h>

// Retail float-pool literals referenced below (1.0f / deg-to-fidx / 0.0f)
extern const float lbl_eu_80669AA0;
extern const float lbl_eu_80669AA4;
extern const float lbl_eu_80669AA8;

namespace nw4r {
namespace g3d {

void ResNode::PatchChrAnmResult(ChrAnmResult* pResult) const {
    if (!IsValid()) {
        return;
    }

    const ResNodeData& r = ref();
    u32 flags = pResult->flags;

    if (flags & ChrAnmResult::FLAG_PATCH_SCALE) {
        if (r.flags & ResNodeData::FLAG_SCALE_ONE) {
            flags |=
                ChrAnmResult::FLAG_SCALE_ONE | ChrAnmResult::FLAG_SCALE_UNIFORM;

            pResult->s.x = pResult->s.y = pResult->s.z = lbl_eu_80669AA0;
        } else {
            // Propagate/clear the uniform fast-path flag...
            u32 tmp;
            if (r.flags & ResNodeData::FLAG_SCALE_UNIFORM) {
                tmp = flags | ChrAnmResult::FLAG_SCALE_UNIFORM;
            } else {
                tmp = flags & ~ChrAnmResult::FLAG_SCALE_UNIFORM;
            }

            // ...and invalidate any cached identity/uniform result
            flags = tmp & ~(ChrAnmResult::FLAG_MTX_IDENT |
                            ChrAnmResult::FLAG_SCALE_ONE);

            pResult->s = static_cast<const math::VEC3&>(r.scale);
        }
    }

    if (flags & ChrAnmResult::FLAG_PATCH_ROT) {
        // Rotation overwrites the whole matrix, so preserve the existing
        // translation column across the rotation write. The VEC3 ctor args
        // evaluate right-to-left (loads land z, y, x).
        math::VEC3 t(pResult->rt._03, pResult->rt._13, pResult->rt._23);

        if (r.flags & ResNodeData::FLAG_ROT_ZERO) {
            PSMTXIdentity(pResult->rt);
            flags |= ChrAnmResult::FLAG_ROT_ZERO;
        } else {
            // Declared before assignment so their registers are born f0..f3,
            // but assigned in z, const, y, x order so the loads land in
            // retail order.
            f32 deg;
            f32 fx;
            f32 fy;
            f32 fz;
            fz = r.rot.z;
            deg = lbl_eu_80669AA4;
            fy = r.rot.y;
            fx = r.rot.x;

            math::MTX34RotXYZFIdx(&pResult->rt, deg * fx, deg * fy, deg * fz);
            flags &= ~(ChrAnmResult::FLAG_MTX_IDENT |
                       ChrAnmResult::FLAG_ROT_TRANS_ZERO |
                       ChrAnmResult::FLAG_ROT_ZERO);
        }

        pResult->rt._03 = t.x;
        pResult->rt._13 = t.y;
        pResult->rt._23 = t.z;

        flags |= ChrAnmResult::FLAG_ROT_RAW_FMT;
    }

    if (flags & ChrAnmResult::FLAG_PATCH_TRANS) {
        if (r.flags & ResNodeData::FLAG_TRANS_ZERO) {
            flags |= ChrAnmResult::FLAG_TRANS_ZERO;

            pResult->rt._03 = pResult->rt._13 = pResult->rt._23 =
                lbl_eu_80669AA8;
        } else {
            flags &= ~(ChrAnmResult::FLAG_MTX_IDENT |
                       ChrAnmResult::FLAG_ROT_TRANS_ZERO |
                       ChrAnmResult::FLAG_TRANS_ZERO);

            pResult->rt._03 = r.translate.x;
            pResult->rt._13 = r.translate.y;
            pResult->rt._23 = r.translate.z;
        }
    }

    if ((flags & ChrAnmResult::FLAG_ROT_ZERO) &&
        (flags & ChrAnmResult::FLAG_TRANS_ZERO)) {
        flags |= ChrAnmResult::FLAG_ROT_TRANS_ZERO;

        if (flags & ChrAnmResult::FLAG_SCALE_ONE) {
            flags |= ChrAnmResult::FLAG_MTX_IDENT;
        }
    }

    pResult->flags = flags & ~(ChrAnmResult::FLAG_PATCH_SCALE |
                               ChrAnmResult::FLAG_PATCH_ROT |
                               ChrAnmResult::FLAG_PATCH_TRANS);
}

void ResNode::CalcChrAnmResult(ChrAnmResult* pResult) const {
    if (!IsValid()) {
        return;
    }

    const ResNodeData& r = ref();
    u32 flags = 0;

    if (r.flags & ResNodeData::FLAG_SCALE_ONE) {
        flags |=
            ChrAnmResult::FLAG_SCALE_ONE | ChrAnmResult::FLAG_SCALE_UNIFORM;

        pResult->s.z = 1.0f;
        pResult->s.y = 1.0f;
        pResult->s.x = 1.0f;
    } else {
        if (r.flags & ResNodeData::FLAG_SCALE_UNIFORM) {
            flags |= ChrAnmResult::FLAG_SCALE_UNIFORM;
        }

        pResult->s = static_cast<const math::VEC3&>(r.scale);
    }

    if (r.flags & ResNodeData::FLAG_ROT_ZERO) {
        PSMTXIdentity(pResult->rt);
        flags |= ChrAnmResult::FLAG_ROT_ZERO;
    } else {
        pResult->rawR = math::VEC3(r.rot);
        math::MTX34RotXYZDeg(&pResult->rt, r.rot.x, r.rot.y, r.rot.z);
    }

    if (r.flags & ResNodeData::FLAG_TRANS_ZERO) {
        flags |= ChrAnmResult::FLAG_TRANS_ZERO;
    } else {
        pResult->rt._03 = r.translate.x;
        pResult->rt._13 = r.translate.y;
        pResult->rt._23 = r.translate.z;
    }

    if (flags & ChrAnmResult::FLAG_ROT_ZERO) {
        if (flags & ChrAnmResult::FLAG_TRANS_ZERO) {
            flags |= ChrAnmResult::FLAG_ROT_TRANS_ZERO;

            if (flags & ChrAnmResult::FLAG_SCALE_ONE) {
                flags |= ChrAnmResult::FLAG_MTX_IDENT;
            }
        }
    }

    flags |= ChrAnmResult::FLAG_ROT_RAW_FMT;
    flags |= ChrAnmResult::FLAG_ANM_EXISTS;

    if (r.flags & ResNodeData::FLAG_SSC_APPLY) {
        flags |= ChrAnmResult::FLAG_SSC_APPLY;
    }

    if (r.flags & ResNodeData::FLAG_SSC_PARENT) {
        flags |= ChrAnmResult::FLAG_SSC_PARENT;
    }

    pResult->flags = flags;
}

} // namespace g3d
} // namespace nw4r

namespace nw4r {
namespace g3d {

void* ResNode::GetResUserData() {
    return ofs_to_ptr<void>(ref().toResUserData);
}

} // namespace g3d
} // namespace nw4r

