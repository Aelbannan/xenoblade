#pragma once

namespace ml {
class CVec3;
}

namespace cf {
class CfObject;
struct CfCamFollow;
}

// Canonical C++ declarations for the free helpers defined in CfCam.cpp.
float cfCam_pitchFromVec(const ml::CVec3* v);
void cfCam_pushStateToActive(cf::CfCamFollow* self);
void cfCam_xformAimPairInv(cf::CfObject* src, ml::CVec3* dst1, ml::CVec3* dst2);
int runCamAction(cf::CfCamFollow* self, int id);
