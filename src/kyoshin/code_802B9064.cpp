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

// (Gauge slots fold onto cf::CActorParam -- getArtsScale at +0x15C is
// CActorParam_getArtsScale (slot 0x15C), isActive at +0x2BC is
// CActorParam_isBattleLocked.)

// C-ABI import from the CVS voice-thread family (retail .text:0x802A6488,
// unmangled name; the CVS_THREAD_* classes declare the same parameter list).
extern "C" int RequestVoicePlayback(CCharVoice* voicePtr, int voiceId, int arg);
// C-ABI voice-event dispatch defined in CCharVoiceMan.cpp.
extern "C" int CCharVoiceMan_DispatchById(int a, int b);

// Retail symbol is the unmangled BattleGaugeTierVoiceGate (C linkage) - CCharVoiceMan
// and other TUs reference it via extern "C"; without this the mangled
// __FP11BattleGaugeff breaks the main.dol link.
extern "C" bool BattleGaugeTierVoiceGate(BattleGauge* obj, f32 curVal, f32 prevVal) {
    cf::CActorParam* battle = reinterpret_cast<cf::CActorParam*>(obj);

    // Gauge must be enabled (flag bit 1) and the value currently rising.
    if (!(obj->flags & 2)) {
        return false;
    }
    if (curVal <= prevVal) {
        return false;
    }
    // A related gauge action is already in flight -- don't double-trigger.
    if (battle->CActorParam_isBattleLocked()) {
        return false;
    }

    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    if (static_cast<s16>(bm->unk20C8.unk0) != 0) {
        return false;
    }

    // Normalise the rising gauge value to [0,1] against the object's scale.
    f32 curNorm = curVal / battle->CActorParam_getArtsScale();
    f32 prevNorm = prevVal / battle->CActorParam_getArtsScale();

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
    if (!CCharVoiceMan_DispatchById(0x7D, 1)) {
        return false;
    }
    if (!CCharVoiceMan_AllocVoiceArena(0x20)) {
        return false;
    }

// Play the cue through the battle object's own voice-position sub-object.
    if (obj != NULL) {
        obj = reinterpret_cast<BattleGauge*>(&obj->voiceEntry);
    }
    RequestVoicePlayback(reinterpret_cast<CCharVoice*>(obj), voiceID, 0x7D);
    return false;
}
