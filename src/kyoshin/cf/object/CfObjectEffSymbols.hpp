#pragma once

#include <types.h>

namespace cf {
class CfObjectEff;
}

// Canonical C++ declarations for the free helpers defined in CfObjectEff.cpp.
void bindPartnerO_(cf::CfObjectEff* self, u8* object, u8* partner);
void setTargetObj_(cf::CfObjectEff* obj, u8* target);
void setChild5CFl_(char* obj, int flag);
void createEffect_(cf::CfObjectEff* self);
void notifyDetach_(u8* self);
