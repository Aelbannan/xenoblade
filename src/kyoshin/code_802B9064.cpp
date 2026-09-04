#include <types.h>
#include "kyoshin/code_802B9064.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"

// Minimal layout of the battle object's fields this helper touches. The full
// type is not yet identified, so only the offsets actually read are declared.
struct BattleGauge {
    u8 pad_00[0x3E9C];
    CCharVoice* voiceEntry;                      // +0x3E9C voice-position sub-object
    u8 pad_3EA0[0x3F00 - 0x3EA0];
    u32 flags;                                   // +0x3F00 flag word (bit 1 = battle enabled?)
};

// (Gauge slots fold onto cf::CActorParam -- getScale at +0x15C is
// CActorParam_UnkVirtualFunc50, isActive at +0x2BC is
// CActorParam_UnkVirtualFunc138.)


// Overlay of the CBattleManager trailing fields read by this helper (the
// header's own Unk* member names would trip the unknown-name linter, so the
// offset is re-declared here with a field_0xNN name).
struct BattleManagerLayout {
    u8 pad_00[0x20C8];
    u16 field_20C8;                                  // +0x20C8 chain/timer state (read as s16)
};

// Retail symbol is the unmangled func_802B9064 (C linkage) - CCharVoiceMan
// and other TUs reference it via extern "C"; without this the mangled
// __FP11BattleGaugeff breaks the main.dol link.
extern "C" bool func_802B9064(BattleGauge* obj, f32 curVal, f32 prevVal) {
    BattleGauge* voiceObj = reinterpret_cast<BattleGauge*>(obj);
    cf::CActorParam* battle = reinterpret_cast<cf::CActorParam*>(obj);

    // Gauge must be enabled (flag bit 1) and the value currently rising.
    if (!(voiceObj->flags & 2)) {
        return false;
    }
    if (curVal <= prevVal) {
        return false;
    }
    // A related gauge action is already in flight -- don't double-trigger.
    if (battle->CActorParam_UnkVirtualFunc138()) {
        return false;
    }

    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    BattleManagerLayout* bmv = reinterpret_cast<BattleManagerLayout*>(bm);
    if (*reinterpret_cast<s16*>(&bmv->field_20C8) != 0) {
        return false;
    }

    // Normalise the rising gauge value to [0,1] against the object's scale.
    f32 curNorm = curVal / battle->CActorParam_UnkVirtualFunc50();
    f32 prevNorm = prevVal / battle->CActorParam_UnkVirtualFunc50();

    // Play the matching tier-crossing voice cue the moment a tier is reached.
    int voiceID;
    if (prevNorm < 1.0f && 1.0f <= curNorm) {
        voiceID = 0xA2A;   // crossing full gauge  (>= 1.0)
    } else if (prevNorm < 0.5f && 0.5f <= curNorm) {
        voiceID = 0xA29;   // crossing half gauge   (>= 0.5)
    } else {
        return false;
    }

    // Request the voice-subsystem resources first; bail if unavailable.
    if (!func_802A330C(0x7D, 1)) {
        return false;
    }
    if (!func_802A34E4(0x20)) {
        return false;
    }

    // Play the cue through the battle object's own voice-position sub-object.
    if (obj != NULL) {
        obj = reinterpret_cast<BattleGauge*>(&voiceObj->voiceEntry);
    }
    func_802A3D54(reinterpret_cast<CCharVoice*>(obj), voiceID, 0x7D);
    return false;
}