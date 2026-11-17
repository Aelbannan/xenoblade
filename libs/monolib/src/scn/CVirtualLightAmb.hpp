#pragma once

#include "monolib/scn/CVirtualLightObj.hpp"

// Ambient virtual light (size 0x34): base light with kind +0x2C = 1.
class CVirtualLightAmb : public CVirtualLightObj {
public:
    CVirtualLightAmb();
    ~CVirtualLightAmb();
};
