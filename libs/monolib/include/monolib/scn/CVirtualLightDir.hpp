#pragma once

#include "monolib/scn/CVirtualLightObj.hpp"

// Directional virtual light (size 0x3C): base light + two f32 fields.
class CVirtualLightDir : public CVirtualLightObj {
public:
    CVirtualLightDir();
    ~CVirtualLightDir();

    f32 mField34;   // +0x34
    f32 mField38;   // +0x38
};
