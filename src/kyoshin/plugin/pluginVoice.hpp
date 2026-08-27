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

// Raw-node view of the voice/actor-manager list returned by getListB28():
// same CErrMesList/reslist shape (sentinel pointer at +0x04, node next at
// +0x00, item at +0x08). Retail walks these nodes directly instead of using
// the reslist<cf::CfObject*>::iterator API (see CSysWinScenarioLog.hpp).
struct VoiceActorListNode {
    VoiceActorListNode* next; // 0x00
    u8 _04[4];                // 0x04
    u8* object;               // 0x08 - item (embedded CfObjectMove at +0x3E9C)
};
struct VoiceActorList {
    u8* field_0x0;                 // 0x00
    VoiceActorListNode* sentinel;  // 0x04
};

// Voice/actor-manager list accessor (retail mangled name getListB28__Fv -
// plain C++ linkage reproduces the Fv suffix).
VoiceActorList* getListB28();

int voice_play(_sVMThread* pThread);
void pluginVoiceRegist();