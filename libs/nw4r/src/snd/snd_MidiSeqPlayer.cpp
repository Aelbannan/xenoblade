#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Out-of-line definition of the SeqPlayer destructor in THIS TU. The retail
// snd_MidiSeqPlayer.o contains the four this-adjusting virtual thunks
// (@208@InvalidateWaveData/@208@InvalidateData/@220@OnShutdownSoundThread/
// @220@OnUpdateFrameSoundThread) alongside the small SeqPlayer overrides. MWCC
// emits those thunks only when the TU emits the class vtable, which is
// triggered by an out-of-line virtual definition here (the dtor is the first
// non-inline virtual in declaration order). The body is intentionally empty:
// the retail dtor lives in snd_SeqPlayer.o and only the vtable/thunk emission
// side effect is needed from this TU. The emitted thunks are byte-identical
// (subi r3,r3,0xd4/0xe0; b <real>); the @NNN@ name is the adjust amount in
// decimal (0xd4=212, 0xe0=224) and resolves against the retail map's stale
// @208@/@220@ annotations via the digit-strip symbol cascade.
SeqPlayer::~SeqPlayer() {}

} // namespace detail
} // namespace snd
} // namespace nw4r

using nw4r::snd::detail::SeqPlayer;

void Update__Q44nw4r3snd6detail9SeqPlayerFv(void);

extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv(void) {
    Update__Q44nw4r3snd6detail9SeqPlayerFv();
}
void OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv(void* self){
    // this->Stop() vtable dispatch (slot 0x10) as a tail call.
    // Typed polymorphic struct form so MWCC keeps the vptr load in r12.
    struct SeqPlayerVt {
        virtual void Slot0();
        virtual void Slot1();
        virtual void Slot2(); // Stop at vtable slot 4 (offset 0x10)
    };
    static_cast<SeqPlayerVt*>(self)->Slot2();
}
extern "C" void InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv(void) {
}
extern "C" void OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv(void) {
}
extern "C" u8 IsPause__Q44nw4r3snd6detail9SeqPlayerCFv(SeqPlayer* self) { return self->IsPause(); }
extern "C" u8 IsStarted__Q44nw4r3snd6detail9SeqPlayerCFv(SeqPlayer* self) { return self->IsStarted(); }
extern "C" u8 IsActive__Q44nw4r3snd6detail9SeqPlayerCFv(SeqPlayer* self) { return self->IsActive(); }
