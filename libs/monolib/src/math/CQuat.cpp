#include "monolib/math.hpp"
#include <revolution/MTX.h>

// Float-pool constants from FloatUtils.cpp (radians -> LUT fidx factor, 0.0f, 1.0f)
extern const float lbl_eu_8066A268;
extern const float lbl_eu_8066A26C;
extern const float lbl_eu_8066A270[2];

namespace ml{
    CQuat CQuat::zero = CQuat(0,0,0,0);
    CQuat CQuat::identity = CQuat(0,0,0,1);

    void CQuat::slerp(CQuat& outQuat, const CQuat& a, const CQuat& b, float t){
        C_QUATSlerp(a, b, outQuat, t);
    }

    // Builds R = Ry * Rx * Rz from the euler angles (radians) and converts to
    // a quaternion. Each axis rotation is built with the nw4r sin/cos LUT and
    // concatenated in-place.
    void CQuat::setRotZXY(const CVec3* vec) {
        Mtx rotZ, rotY, rotX;
        CQuat quat;

        // Keep each raw angle in a callee-saved FPR; the rad->fidx scale
        // multiply is emitted separately for the sin and cos lookups.
        f32 zAng = vec->z;
        f32 sinZ = nw4r::math::SinFIdx(lbl_eu_8066A268 * zAng);
        f32 cosZ = nw4r::math::CosFIdx(lbl_eu_8066A268 * zAng);

        rotZ[0][0] = cosZ;  rotZ[0][1] = -sinZ; rotZ[0][2] = lbl_eu_8066A26C;   rotZ[0][3] = lbl_eu_8066A26C;
        rotZ[1][0] = sinZ;  rotZ[1][1] = cosZ;  rotZ[1][2] = lbl_eu_8066A26C;   rotZ[1][3] = lbl_eu_8066A26C;
        rotZ[2][0] = lbl_eu_8066A26C;   rotZ[2][1] = lbl_eu_8066A26C;   rotZ[2][2] = lbl_eu_8066A270[0]; rotZ[2][3] = lbl_eu_8066A26C;

        f32 xAng = vec->x;
        f32 sinX = nw4r::math::SinFIdx(lbl_eu_8066A268 * xAng);
        f32 cosX = nw4r::math::CosFIdx(lbl_eu_8066A268 * xAng);

        rotX[0][0] = lbl_eu_8066A270[0];    rotX[0][1] = lbl_eu_8066A26C;   rotX[0][2] = lbl_eu_8066A26C;   rotX[0][3] = lbl_eu_8066A26C;
        rotX[1][0] = lbl_eu_8066A26C;   rotX[1][1] = cosX;  rotX[1][2] = -sinX; rotX[1][3] = lbl_eu_8066A26C;
        rotX[2][0] = lbl_eu_8066A26C;   rotX[2][1] = sinX;  rotX[2][2] = cosX;  rotX[2][3] = lbl_eu_8066A26C;

        PSMTXConcat(rotX, rotZ, rotZ);

        f32 yAng = vec->y;
        f32 sinY = nw4r::math::SinFIdx(lbl_eu_8066A268 * yAng);
        f32 cosY = nw4r::math::CosFIdx(lbl_eu_8066A268 * yAng);

        rotY[0][0] = cosY;  rotY[0][1] = lbl_eu_8066A26C;   rotY[0][2] = sinY;  rotY[0][3] = lbl_eu_8066A26C;
        rotY[1][0] = lbl_eu_8066A26C;   rotY[1][1] = lbl_eu_8066A270[0];    rotY[1][2] = lbl_eu_8066A26C;   rotY[1][3] = lbl_eu_8066A26C;
        rotY[2][0] = -sinY; rotY[2][1] = lbl_eu_8066A26C;   rotY[2][2] = cosY;  rotY[2][3] = lbl_eu_8066A26C;

        PSMTXConcat(rotY, rotZ, rotZ);

        C_QUATMtx(quat, rotZ);
        *this = quat;
    }
    
    void CQuat::getRotZXY(CVec3* outVec) const {
        CMat34 mat;
        mat.setRotQuat(*this);
        mat.getRotZXY(*outVec);
    }
} //namespace ml
