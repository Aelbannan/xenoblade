#pragma once

#include <types.h>
#include <nw4r/snd.h>

// FX-slot record (0x4FC bytes): two of these sit at CfSoundManGlobal +0x04
// (mFxSlots). The full field layout lives in code_801C2C14.cpp; this TU only
// forwards the array base to func_801C34B0, so opaque storage is enough here.
struct FxSoundSlot {
    u8 mData[0x4FC];
};

// FX-slot effect setter (retail func_801C34B0, defined in code_801C2C14.cpp).
// extern "C" keeps the call reloc bound to the retail-unmangled name (a plain
// C++ declaration would have MWCC append the parameter mangling).
extern "C" int func_801C34B0(FxSoundSlot* slots, int index, int frame);

// FX-slot pair constructor (retail func_801C32E8, defined in
// code_801C2C14.cpp): constructs the two 0x4FC-byte FX slots. extern "C" so
// the call reloc binds to the retail-unmangled name.
extern "C" FxSoundSlot* func_801C32E8(FxSoundSlot* self);

// FX-slot pair destructor (retail __dt__801C3340, defined in
// code_801C2C14.cpp): clears both slots' effects and destroys the member
// array. extern "C" keeps the call reloc bound to the retail-unmangled name
// (a plain C++ declaration would have MWCC append the parameter mangling).
extern "C" void* __dt__801C3340(FxSoundSlot* self, int flag);

// FX effect applicator (retail func_801C358C, defined in code_801C2C14.cpp):
// tail-calls func_801C30BC and passes its bool result through in r3 (the
// fork's definition returns void; the bool declaration lets callers test it).
// extern "C" keeps the call reloc bound to the retail-unmangled name.
extern "C" bool func_801C358C(FxSoundSlot* slots, int idx, int fxType, u32 memSize, u32 size);

// FX work-buffer size query (defined in code_800A75FC.cpp): returns the
// per-slot allocation for FX slot index 0 or 1 (or 0). extern "C" keeps the
// call reloc bound to the retail-unmangled name.
extern "C" int func_800A9E50(int index);

// Retail nw4r::snd::SoundArchive::GetSoundCount() - the fork's
// snd_SoundArchive.h does not declare it (only the detail file-reader
// variant); declared here with extern "C" so the call reloc binds to the
// retail mangled-name symbol.
extern "C" int GetSoundCount__Q34nw4r3snd12SoundArchiveCFv(const nw4r::snd::SoundArchive* self);

// Retail nw4r::snd::SoundSystem::IsInitializedSoundSystem() - the fork's
// snd_SoundSystem.h does not declare it (it is defined in the nw4r lib as a
// mangled-name free function); declared here with extern "C" so the call
// reloc binds to the retail-unmangled name.
extern "C" bool IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv();

namespace cf {
    class CfSoundMan {
    public:
        static void func_801BFC38(u32 r3, u32 r4, u32 r5, u32 r6, float f1);
    };

    // Sound-slot record: 0x268-byte entries in the manager's record array
    // (CfSoundManGlobal::mRecords at +0x9FC). Bit 0 of mFlag marks the slot
    // active; each slot embeds an nw4r SoundArchivePlayer at +0x17C.
    struct CfSoundRecord {
        /* 0x00 */ u16 mFlag;
        u8 field_0x02[0x0E];
        /* 0x10 */ u32 field_0x10;
        /* 0x14 */ void* field_0x14; // sound-data buffer; freed by func_801C0A14
        /* 0x18 */ u32 field_0x18;
        /* 0x1C */ u32 field_0x1C;
        /* 0x20 */ u32 field_0x20;
        /* 0x24 */ u32 field_0x24;
        /* 0x28 */ s32 field_0x28; // record id, -1 when unused
        /* 0x2C */ nw4r::snd::MemorySoundArchive mArchive;
        /* 0x17C */ nw4r::snd::SoundArchivePlayer mArchivePlayer;
        u8 field_0x25C[0xC]; // tail pad: sizeof(SoundArchivePlayer)=0xE0 -> 0x268
    };

    // Sound manager singleton (retail lbl_eu_80664430, .sbss). Layout:
    // 0x00 header word, 0x04 the two FX slots (2 x 0x4FC = 0x9F8), 0x9FC the
    // record array (func_801C028C hands the FX-slot array to func_801C34B0).
    struct CfSoundManGlobal {
        /* 0x00 */ u16 field_0x00;
        u8 field_0x02[0x02];
        /* 0x04 */ FxSoundSlot mFxSlots[2];
        /* 0x9FC */ CfSoundRecord mRecords[1];
    };

    // 0x30-byte sound-slot record: 64 entries in the global table
    // (lbl_eu_80575928). Initialized by func_801C02F8, looked up by id
    // (func_801C087C), torn down by func_eu_801C22F0.
    struct CfSoundSlot {
        /* 0x00 */ nw4r::snd::detail::BasicSound* mSound;
        /* 0x04 */ u16 mId;
        u8 field_0x06[0x02];
        /* 0x08 */ u32 field_0x08;
        /* 0x0C */ u32 field_0x0C;
        /* 0x10 */ u32 field_0x10;
        /* 0x14 */ u32 mSoundId;
        /* 0x18 */ f32 field_0x18;
        /* 0x1C */ f32 field_0x1C;
        u8 field_0x20[0x0A];
        /* 0x2A */ u16 field_0x2A;
        /* 0x2C */ u16 field_0x2C;
        /* 0x2E */ s16 field_0x2E;
    };

    // 12-byte parameter block copied into a sound-slot record at +0x08
    // (see func_801BFAE8).
    struct CfSoundSlotParam {
        /* 0x00 */ u32 field_0x00;
        /* 0x04 */ u32 field_0x04;
        /* 0x08 */ u32 field_0x08;
    };

    // 4-byte wrapper owning an nw4r SoundHandle; deleting dtor
    // (__dt__801C0374) detaches the handle before freeing.
    struct CfSoundHandle {
        /* 0x00 */ nw4r::snd::SoundHandle mHandle;
    };

    // Sound-pause request parameters read by func_801C15C0 (retail .bss,
    // 0x10 bytes). field_0x00 is the target sound id (-1 = all sounds),
    // field_0x04 the fade frames, field_0x08 the pause flag.
    struct CfSoundPauseParam {
        /* 0x00 */ s32 field_0x00;
        /* 0x04 */ u16 field_0x04;
        u8 field_0x06[0x02];
        /* 0x08 */ u32 field_0x08;
        /* 0x0C */ u32 field_0x0C;
    };
}

// 64-entry sound-slot table (retail .bss, 0xC00 bytes).
extern cf::CfSoundSlot lbl_eu_80575928[64];

// Global-object registration link cell for the sound-slot table (retail .bss,
// sits 0x10 bytes before the table; passed by address to __register_global_object).
extern u8 lbl_eu_80575918[0x10];

// MWCC runtime helpers (C linkage so the call relocs bind to the
// retail-unmangled names; same pattern as CMenuQstCnt.hpp).
extern "C" void __construct_array(void*, void* ctor, void* dtor, int size, int count);
extern "C" void* __register_global_object(void* object, void* destructor, void* registration);

// Array-destruction helper (MWCC runtime; extern "C" keeps the call reloc
// bound to the retail-unmangled name, same as __construct_array above).
extern "C" void __destroy_arr(void*, void* dtor, int size, int count);

// Sound-pause request parameters (retail .bss, 0x10 bytes).
extern cf::CfSoundPauseParam lbl_eu_80576528;

// Default sound-slot constants (retail .sdata2 floats; const routes them into
// the readonly pool so MWCC can hoist the lfs loads early).
extern const float lbl_eu_80667E98;
extern const float lbl_eu_80667E9C;

// Default FX-send volume (retail .sdata2; passed to func_801C0DC4 by
// func_801C0FCC when starting a sound slot at the default volume).
extern const float lbl_eu_80667EA0;

// Master-volume backing store (retail .sdata, 8 bytes; written by
// func_801BFFAC).
extern float lbl_eu_80662628;

// Volume ramp multiplier (retail .sdata2 const; used by func_801BFFAC).
extern const float lbl_eu_80667E8C;

// Retail sound-manager pointer (unmangled at global scope).
extern cf::CfSoundManGlobal* lbl_eu_80664430;

// Sound-system-init flag byte (retail .sbss, one byte; cleared by the
// manager ctor before the first InitSoundSystem call).
extern u8 lbl_eu_8066443A;

// No-op notification handler. Called from func_801C0DC4 when
// nw4r::snd::detail::SoundStartable::detail_StartSound succeeds.
// Left intentionally empty in the retail binary (single blr).
extern "C" void func_801C0DC0();

// nw4r member dtors called directly by __dt__801BF80C (retail symbols).
// extern "C" keeps the call relocs bound to the retail-unmangled names (a
// plain global-scope C++ declaration would have MWCC append the parameter
// mangling). Explicit member-dtor syntax would make MWCC virtual-dispatch
// through the vtable, but the retail binary calls these directly.
extern "C" void __dt__Q34nw4r3snd18SoundArchivePlayerFv(nw4r::snd::SoundArchivePlayer* self, int flag);
extern "C" void __dt__Q34nw4r3snd18MemorySoundArchiveFv(nw4r::snd::MemorySoundArchive* self, int flag);

// nw4r archive ctors called directly by func_801C09BC (retail symbols):
// placement new would add MWCC's exceptions-on NULL check around the call,
// which the retail binary does not have (whole TU compiled without it).
extern "C" void __ct__Q34nw4r3snd18MemorySoundArchiveFv(nw4r::snd::MemorySoundArchive* self);
extern "C" void __ct__Q34nw4r3snd18SoundArchivePlayerFv(nw4r::snd::SoundArchivePlayer* self);
