#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

} // namespace detail
} // namespace snd
} // namespace nw4r

extern "C" void Update__Q44nw4r3snd6detail9SeqPlayerFv(void);
extern "C" void OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv(void) {
    Update__Q44nw4r3snd6detail9SeqPlayerFv();
}
extern "C" void OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv(void* self) {
    // vtable dispatch (empty virtual -> base class tail call)
    typedef void (*VFunc)(void*);
    VFunc* vtable = *(VFunc**)self;
    vtable[4](self);
}
extern "C" void InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv(void) {
}
extern "C" void OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv(void) {
}
extern "C" u8 IsPause__Q44nw4r3snd6detail9SeqPlayerCFv(void* self) { return ((u8*)self)[0xee]; }
extern "C" u8 IsStarted__Q44nw4r3snd6detail9SeqPlayerCFv(void* self) { return ((u8*)self)[0xed]; }
extern "C" u8 IsActive__Q44nw4r3snd6detail9SeqPlayerCFv(void* self) { return ((u8*)self)[0xec]; }
