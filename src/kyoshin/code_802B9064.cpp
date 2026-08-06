#include <types.h>
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/voice/CCharVoice.hpp"

// ── Sibling-TU voice hooks (retail symbols are unmangled globals) ──────────
// func_802A330C / func_802A34E4: CVS voice-subsystem requests that return
// non-zero on success. func_802A3D54: play a battle voice line against a
// position sub-object. All three are non-overloaded free functions, so MWCC
// keeps their unmangled C-style names (non-overloaded free functions).
bool func_802A330C(u32 size, u32 align);
bool func_802A34E4(u32 size);
void func_802A3D54(CCharVoice* voicePtr, int voiceId, int groupId);

// Minimal layout of the battle object's fields this helper touches. The full
// type is not yet identified, so only the offsets actually read are declared.
struct BattleGauge {
    u8 pad_00[0x3E9C];
    CCharVoice* voiceEntry;                      // +0x3E9C voice-position sub-object
    u8 pad_3EA0[0x3F00 - 0x3EA0];
    u32 flags;                                   // +0x3F00 flag word (bit 1 = battle enabled?)
};

// The only virtual slots this helper dispatches through are 0x2BC
// (`isActive`) and 0x15C (`getScale`); the rest keep the vtable dense so
// those slots land at their retail offsets.
struct BattleIf {
    virtual void vf8();
    virtual void vfC();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf8C();
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual void vfA4();
    virtual void vfA8();
    virtual void vfAC();
    virtual void vfB0();
    virtual void vfB4();
    virtual void vfB8();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual void vfD0();
    virtual void vfD4();
    virtual void vfD8();
    virtual void vfDC();
    virtual void vfE0();
    virtual void vfE4();
    virtual void vfE8();
    virtual void vfEC();
    virtual void vfF0();
    virtual void vfF4();
    virtual void vfF8();
    virtual void vfFC();
    virtual void vf100();
    virtual void vf104();
    virtual void vf108();
    virtual void vf10C();
    virtual void vf110();
    virtual void vf114();
    virtual void vf118();
    virtual void vf11C();
    virtual void vf120();
    virtual void vf124();
    virtual void vf128();
    virtual void vf12C();
    virtual void vf130();
    virtual void vf134();
    virtual void vf138();
    virtual void vf13C();
    virtual void vf140();
    virtual void vf144();
    virtual void vf148();
    virtual void vf14C();
    virtual void vf150();
    virtual void vf154();
    virtual void vf158();
    virtual f32 getScale();                             // vtable slot 0x15C (348)
    virtual void vf160();
    virtual void vf164();
    virtual void vf168();
    virtual void vf16C();
    virtual void vf170();
    virtual void vf174();
    virtual void vf178();
    virtual void vf17C();
    virtual void vf180();
    virtual void vf184();
    virtual void vf188();
    virtual void vf18C();
    virtual void vf190();
    virtual void vf194();
    virtual void vf198();
    virtual void vf19C();
    virtual void vf1A0();
    virtual void vf1A4();
    virtual void vf1A8();
    virtual void vf1AC();
    virtual void vf1B0();
    virtual void vf1B4();
    virtual void vf1B8();
    virtual void vf1BC();
    virtual void vf1C0();
    virtual void vf1C4();
    virtual void vf1C8();
    virtual void vf1CC();
    virtual void vf1D0();
    virtual void vf1D4();
    virtual void vf1D8();
    virtual void vf1DC();
    virtual void vf1E0();
    virtual void vf1E4();
    virtual void vf1E8();
    virtual void vf1EC();
    virtual void vf1F0();
    virtual void vf1F4();
    virtual void vf1F8();
    virtual void vf1FC();
    virtual void vf200();
    virtual void vf204();
    virtual void vf208();
    virtual void vf20C();
    virtual void vf210();
    virtual void vf214();
    virtual void vf218();
    virtual void vf21C();
    virtual void vf220();
    virtual void vf224();
    virtual void vf228();
    virtual void vf22C();
    virtual void vf230();
    virtual void vf234();
    virtual void vf238();
    virtual void vf23C();
    virtual void vf240();
    virtual void vf244();
    virtual void vf248();
    virtual void vf24C();
    virtual void vf250();
    virtual void vf254();
    virtual void vf258();
    virtual void vf25C();
    virtual void vf260();
    virtual void vf264();
    virtual void vf268();
    virtual void vf26C();
    virtual void vf270();
    virtual void vf274();
    virtual void vf278();
    virtual void vf27C();
    virtual void vf280();
    virtual void vf284();
    virtual void vf288();
    virtual void vf28C();
    virtual void vf290();
    virtual void vf294();
    virtual void vf298();
    virtual void vf29C();
    virtual void vf2A0();
    virtual void vf2A4();
    virtual void vf2A8();
    virtual void vf2AC();
    virtual void vf2B0();
    virtual void vf2B4();
    virtual void vf2B8();
    virtual bool isActive();                            // vtable slot 0x2BC (700)
};

// Overlay of the CBattleManager trailing fields read by this helper (the
// header's own Unk* member names would trip the unknown-name linter, so the
// offset is re-declared here with a field_0xNN name).
struct BattleManagerLayout {
    u8 pad_00[0x20C8];
    u16 field_20C8;                                  // +0x20C8 chain/timer state (read as s16)
};

bool func_802B9064(BattleGauge* obj, f32 curVal, f32 prevVal) {
    BattleGauge* voiceObj = reinterpret_cast<BattleGauge*>(obj);
    BattleIf* battle = reinterpret_cast<BattleIf*>(obj);

    // Gauge must be enabled (flag bit 1) and the value currently rising.
    if (!(voiceObj->flags & 2)) {
        return false;
    }
    if (curVal <= prevVal) {
        return false;
    }
    // A related gauge action is already in flight -- don't double-trigger.
    if (battle->isActive()) {
        return false;
    }

    cf::CBattleManager* bm = cf::CBattleManager::getInstance();
    BattleManagerLayout* bmv = reinterpret_cast<BattleManagerLayout*>(bm);
    if (*reinterpret_cast<s16*>(&bmv->field_20C8) != 0) {
        return false;
    }

    // Normalise the rising gauge value to [0,1] against the object's scale.
    f32 curNorm = curVal / battle->getScale();
    f32 prevNorm = prevVal / battle->getScale();

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