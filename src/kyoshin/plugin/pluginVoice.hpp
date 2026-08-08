#pragma once

#include <types.h>
#include "monolib/vm/yvm2.h"

// Accessor into a CfObjectActor: the voice/character-group id lives at +0x3F28.
// Used by voice_play to match a requested voice id while iterating the global
// object list (whose items point at the embedded CfObjectMove at base+0x3E9C).
struct VoiceActorVoiceId {
    u8 _pad00[0x3F28];
    u16 field_3F28; // +0x3F28: character/voice-group id (u16)
};

int voice_play(_sVMThread* pThread);
void pluginVoiceRegist();