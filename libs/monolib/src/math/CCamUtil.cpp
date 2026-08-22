#include "monolib/math.hpp"

namespace ml {
    //Not in XC3D
    // Builds a ZXY rotation matrix from an Euler-angle vector and extracts the
    // equivalent XYZ2ZXY-ordered rotation back into outVec.
    void CCamUtil::getXYZ2ZXY(CVec3& outVec, const CVec3& vec){
        CMat33 mat;
        float sX = math::sin(vec.x);
        float cX = math::cos(vec.x);
        float sY = math::sin(vec.y);
        float cY = math::cos(vec.y);
        float sZ = math::sin(vec.z);
        float cZ = math::cos(vec.z);

        // Named temps pin MWCC's FPR allocation/CSE to retail. Computation
        // order follows retail's scheduler; declaration order (by increasing
        // lifetime) drives the register claim order.
        float sXcZ, cYcZ, sXsZ, cXsY;
        cXsY = cX*sY;
        sXcZ = sX*cZ;
        cYcZ = cY*cZ;
        sXsZ = sX*sZ;
        mat.m[1][2] = sZ*cXsY - sXcZ;
        mat.m[0][0] = cYcZ;
        float cXcZ, cXsZ, sXsY;
        sXsY = sX*sY;
        cXsZ = cX*sZ;
        cXcZ = cX*cZ;
        mat.m[0][1] = cZ*sXsY - cXsZ;
        mat.m[0][2] = cZ*cXsY + sXsZ;
        float m11, m10;
        m11 = sZ*sXsY + cXcZ;
        m10 = cY*sZ;
        mat.m[1][0] = m10;
        mat.m[1][1] = m11;
        mat.m[2][0] = -sY;
        mat.m[2][1] = sX*cY;
        mat.m[2][2] = cX*cY;

        mat.getRotZXY(outVec);
    }
} //namespace ml
