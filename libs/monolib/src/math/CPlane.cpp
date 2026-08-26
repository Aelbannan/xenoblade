#include "monolib/math.hpp"

namespace ml {

    //Sets this plane from the given position, and two given points on the plane.
    CPlane* CPlane::set(const CVec3& pos, const CVec3& p1, const CVec3& p2){
        CVec3 temp1, temp2, cross;
        CVec3::sub(temp1, p1, pos);
        CVec3::sub(temp2, p2, pos);
        CVec3::cross(cross, temp1, temp2);

        mNormal = cross;
        mNormal.normalizeSub();
        mDist = -CVec3::dot(pos, mNormal);

        return this;
    }

    //Calculates the intersection point of the given ray with this plane.
    /* BUG: No check is done to see if the first dot product is near or less than 0 (the ray won't intersect).
    In that specific case, the function will return a nonsense value. */
    void CPlane::getCross(CVec3& outVec, const CPlane& plane, const CVec3& rayOrigin, const CVec3& rayDir){
        CVec3 scaled;
        //Calculate the time t at which the ray will intersect the plane
        //Distance is flipped, so + distance is used
        float t = -(plane.mDist + CVec3::dot(plane.mNormal, rayOrigin)) / CVec3::dot(plane.mNormal, rayDir);
        
        //Calculate the intersection point
        CVec3::scale(scaled, rayDir, t);
        CVec3::add(outVec, rayOrigin, scaled);
    }

}
