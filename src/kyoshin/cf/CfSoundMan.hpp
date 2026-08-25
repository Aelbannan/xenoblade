#pragma once

#include <types.h>
#include <nw4r/snd.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

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
        static u32 func_801BFC38(u32 idx, u32 a, u32 b, u32 c, float volume);
    };

    // Sound-slot record: 0x268-byte entries in the manager's record array
    // (CfSoundManGlobal::mRecords at +0x9FC). Bit 0 of mFlag marks the slot
    // active; each slot embeds an nw4r SoundArchivePlayer at +0x17C.
    struct CfSoundRecord {
        /* 0x00 */ u16 mFlag;
        u8 field_0x02[0x02];
        /* 0x04 */ u32 field_0x04; // sound-data buffer base (passed to mArchive.Setup)
        /* 0x08 */ u32 field_0x08; // aligned data size
        /* 0x0C */ u32 field_0x0C; // work-buffer size
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
        /* 0x20 */ u32 field_0x20;
        /* 0x24 */ u32 field_0x24;
        /* 0x28 */ u16 field_0x28;
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

    // 12-byte position block returned by CfSoundActorSrc::getPosition
    // (vtable +0xAC). Copied word-wise into CfSoundSlot::field_0x08..0x10 by
    // the sound-slot starters (func_801C10C0 / func_801C03C8).
    struct CfSoundActorPos {
        /* 0x00 */ u32 field_0x00;
        /* 0x04 */ u32 field_0x04;
        /* 0x08 */ u32 field_0x08;
    };

    // 3-float staging block used by func_801C03C8: the struct member layout
    // pins the frame slots (x lowest) while the assignment statement order
    // controls the retail lfs load sequence.
    struct CfSoundPos3f {
        f32 x, y, z;
    };

    // func_800B708C(id) result view ("voice source"): the sound-slot starters
    // call the vtable+0xAC slot (getPosition) and the vtable+0x12C slot
    // (vf73, position query) to fetch the actor's position blocks. 74
    // virtuals so getPosition lands at 0x8 + 41*4 = 0xAC and vf73 at
    // 0x8 + 73*4 = 0x12C (MWCC reserves vtable[0]=offset-to-top,
    // vtable[1]=typeinfo).
    class CfSoundActorSrc {
    public:
        virtual void vf00();  virtual void vf01();  virtual void vf02();
        virtual void vf03();  virtual void vf04();  virtual void vf05();
        virtual void vf06();  virtual void vf07();  virtual void vf08();
        virtual void vf09();  virtual void vf10();  virtual void vf11();
        virtual void vf12();  virtual void vf13();  virtual void vf14();
        virtual void vf15();  virtual void vf16();  virtual void vf17();
        virtual void vf18();  virtual void vf19();  virtual void vf20();
        virtual void vf21();  virtual void vf22();  virtual void vf23();
        virtual void vf24();  virtual void vf25();  virtual void vf26();
        virtual void vf27();  virtual void vf28();  virtual void vf29();
        virtual void vf30();  virtual void vf31();  virtual void vf32();
        virtual void vf33();  virtual void vf34();  virtual void vf35();
        virtual void vf36();  virtual void vf37();  virtual void vf38();
        virtual void vf39();  virtual void vf40();
        virtual CfSoundActorPos* getPosition();   // vtable +0xAC
        virtual void vf42();  virtual void vf43();  virtual void vf44();
        virtual void vf45();  virtual void vf46();  virtual void vf47();
        virtual void vf48();  virtual void vf49();  virtual void vf50();
        virtual void vf51();  virtual void vf52();  virtual void vf53();
        virtual void vf54();  virtual void vf55();  virtual void vf56();
        virtual void vf57();  virtual void vf58();  virtual void vf59();
        virtual void vf60();  virtual void vf61();  virtual void vf62();
        virtual void vf63();  virtual void vf64();  virtual void vf65();
        virtual void vf66();  virtual void vf67();  virtual void vf68();
        virtual void vf69();  virtual void vf70();  virtual void vf71();
        virtual void vf72();
        virtual void* vf73(u16 id);                 // vtable +0x12C
        u8 field_0x04[0x98 - 0x04];
        /* 0x98 */ void* field_0x98;              // sub-position object
    };

    // vf73 (vtable +0x12C) result view: floats at +0x0C/+0x1C/+0x2C read by
    // func_801C03C8 (position block).
    struct CfSoundActorPos3 {
        u8 field_0x00[0x0C];
        /* 0x0C */ f32 field_0x0C;
        u8 field_0x10[0x1C - 0x10];
        /* 0x1C */ f32 field_0x1C;
        u8 field_0x20[0x2C - 0x20];
        /* 0x2C */ f32 field_0x2C;
    };

    // obj->field_0x98 sub-object: floats at +0x2DC/+0x2E0/+0x2E4 read by
    // func_801C03C8 (position block).
    struct CfSoundActorSub98 {
        u8 field_0x00[0x2DC];
        /* 0x2DC */ f32 field_0x2DC;
        /* 0x2E0 */ f32 field_0x2E0;
        /* 0x2E4 */ f32 field_0x2E4;
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

// Minimal linked-list node view matching nw4r::ut::LinkListNode's layout
// (its real members are private), used for manual BasicSound list walks.
struct CfSoundListNode {
    CfSoundListNode* mNext; // at 0x0
    CfSoundListNode* mPrev; // at 0x4
};

// Prefix view of nw4r::snd::SoundPlayer: one leading word, then the
// BasicSound play-list head node at +0x04 (begin = mList.mNext,
// end = &mList).
struct CfSoundPlayerView {
    u8 field_0x00[0x04];
    /* 0x04 */ CfSoundListNode mList;
};

// nw4r assertion strings used by func_801C1618's checked list deref.
extern const char lbl_eu_80533C54[];
extern const char lbl_eu_80533C30[];
extern const char lbl_eu_80533C84[];
extern const char lbl_eu_80533C60[];

// C++-mangled retail helper func_800B708C__Fi (actor id -> voice source).
void* func_800B708C(int id);

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

// Global event/presentation flag words (.sbss). lbl_eu_80663E24 bit 9 / bit 11
// and lbl_eu_80663E28 bit 7 gate the func_801BFB34 sound-start dispatch.
extern u32 lbl_eu_80663E28;

// Sound-start gate helpers (defined in other TUs; C ABI so the call relocs
// bind to the retail-unmangled names).
extern "C" bool func_8008585C__Q22cf13CfGameManagerFv();
extern "C" u32 func_80294624();
extern "C" u32 func_8028E440();
extern "C" u32 func_802B22E0();
extern "C" bool isInitialized__10CMenuPauseFv();

// Sound-id counter for freshly allocated slots (retail .sdata u16; read and
// written by func_801C0DC4 when a new slot id is assigned).
extern u16 lbl_eu_80664438;

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

// Position triple view over CfSoundSlot::field_0x08 (three f32 words copied
// word-wise by the sound-slot starters and read as floats by the per-slot
// update).
struct CfSoundPos3 {
    f32 x;
    f32 y;
    f32 z;
};

// Camera-position object behind func_800821F8()->field_0xC (only the three
// position floats the per-slot update reads are declared).
struct CfSndCamObj {
    u8 field_0x00[0x10C];
    nw4r::math::VEC3 mPos; // +0x10C camera position
};

// Scene camera-view word returned by func_8049603C (float at +0xC is the
// remaining display-time fraction used for volume scaling).
struct CfSndCamView {
    u8 field_0x00[0xC];
    f32 field_0x0C;
};

// Opaque pose block returned by func_80496264(scene, -1); passed straight
// through to func_8049B834.
struct CfSndPoseBlock {
    u8 data[0x20];
};

// Minimal data view of the func_800821F8 result (only field_0xC is read);
// call sites cast from the canonical UnkClass_800821F8* returned by the
// owner decl on CfGameManagerApi.hpp.
struct UnkClass_800821F8Snd {
    u8 field_0x00[0xC];
    CfSndCamObj* field_0xC;
};

// func_800821F8__Q22cf13CfGameManagerFv: single winning decl lives on
// CfGameManagerApi.hpp (canonical UnkClass_800821F8* view); this TU's call
// sites cast to the minimal UnkClass_800821F8Snd data view above.
#include "kyoshin/cf/CfGameManagerApi.hpp"

// Sound-start gate helpers used by cf::CfSoundMan::func_801BFC38 (defined in
// other TUs).
extern "C" u32 func_80252538();
extern "C" int CfRes_getD80Flag();

// Computes a distance-based pan/volume pair from the actor position against
// the pose block; the two trailing floats are the near/far distance pair
// (near constant and near + slot range).
extern "C" void func_8049B834(f32* outPan, f32* outVol, CfSndPoseBlock* pose,
                              CfSoundPos3* pos, f32 nearDist, f32 farDist);

// nw4r math/db helpers and assert strings (retail symbols).
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// Volume-scaling constants (retail .sdata2).
extern const f32 lbl_eu_80667E88;
extern const f32 lbl_eu_80667EA4;
