#pragma once

#include <types.h>
#include "monolib/monolib_types.hpp"

//Unofficial name
class ICulling {
public:
    virtual ~ICulling(){}
    virtual bool update(ml::CFrustum* r4);
    virtual bool isOccluded(const ml::CVec3& r4, float r5);
    virtual bool isRayOccluded(const ml::CVec3& r4, const ml::CVec3& r5, int r6);
};
