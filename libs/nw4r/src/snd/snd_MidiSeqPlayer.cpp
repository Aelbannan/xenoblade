#include <nw4r/snd.h>
#include <nw4r/ut.h>

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
