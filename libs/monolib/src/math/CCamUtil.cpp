#include "monolib/math.hpp"

namespace ml {
    //Not in XC3D
    void CCamUtil::getXYZ2ZXY(CVec3& outVec, const CVec3& vec){
        CMat33 mat;
        float sX = math::sin(vec.x);
        float cX = math::cos(vec.x);
        float sY = math::sin(vec.y);
        float cY = math::cos(vec.y);
        float sZ = math::sin(vec.z);
        float cZ = math::cos(vec.z);

        mat.m[0][1] = sX*sY*cZ - cX*sZ;
        mat.m[0][2] = cZ*cX*sY + sX*sZ;
        mat.m[0][0] = cY*cZ;
        mat.m[1][2] = cX*sY*sZ - sX*cZ;
        mat.m[1][0] = cY*sZ;
        mat.m[1][1] = sX*sY*sZ + cX*cZ;
        mat.m[2][0] = -sY;
        mat.m[2][1] = sX*cY;
        mat.m[2][2] = cX*cY;

        mat.getRotZXY(outVec);
    }
} //namespace ml
