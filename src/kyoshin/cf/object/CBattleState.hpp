#pragma once

#include <types.h>

namespace cf {
    // 0x34-byte slot layout used by CBattleState_UnkVirtualFunc6's incoming
    // arg (r4) and by the 8-entry array at CBattleState+0x1388. Same struct
    // shape reused for both (see MWCC_REFERENCE §CBattleState_UnkVirtualFunc6).
    struct CBattleStateEntry {
        u32 unk00; // 0x00
        u32 unk04; // 0x04
        u32 unk08; // 0x08
        u16 unk0C; // 0x0C - id; also bit index into CBattleState::unk15AC
        s32 unk10; // 0x10 - clamped value
        s16 unk14; // 0x14
        s16 unk16; // 0x16
        s16 unk18; // 0x18 - lower clamp bound (0 == no lower clamp)
        s16 unk1A; // 0x1A
        f32 unk1C; // 0x1C
        f32 unk20; // 0x20
        f32 unk24; // 0x24
        f32 unk28; // 0x28
        u16 unk2C; // 0x2C
        u16 unk2E; // 0x2E
        u32 unk30; // 0x30
    };

    // size: 0x15DC
    class CBattleState {
    public:
        virtual void CBattleState_UnkVirtualFunc1();  //0x8
        virtual void CBattleState_UnkVirtualFunc2();  //0xC
        virtual void CBattleState_UnkVirtualFunc3();  //0x10
        virtual void CBattleState_UnkVirtualFunc4();  //0x14
        virtual void CBattleState_UnkVirtualFunc5();  //0x18
        virtual void CBattleState_UnkVirtualFunc6();  //0x1C
        virtual void CBattleState_UnkVirtualFunc7();  //0x20
        virtual void CBattleState_UnkVirtualFunc8();  //0x24
        virtual void CBattleState_UnkVirtualFunc9();  //0x28
        virtual void CBattleState_UnkVirtualFunc10(); //0x2C
        virtual void CBattleState_UnkVirtualFunc11(); //0x30
        virtual void CBattleState_UnkVirtualFunc12(); //0x34
        virtual void CBattleState_UnkVirtualFunc13(); //0x38
        virtual void CBattleState_UnkVirtualFunc14(); //0x3C
        virtual void CBattleState_UnkVirtualFunc15(); //0x40
        virtual void CBattleState_UnkVirtualFunc16(); //0x44
        virtual void CBattleState_UnkVirtualFunc17(); //0x48
        virtual void CBattleState_UnkVirtualFunc18(); //0x4C
        virtual void CBattleState_UnkVirtualFunc19(); //0x50
        virtual void CBattleState_UnkVirtualFunc20(); //0x54
        virtual void CBattleState_UnkVirtualFunc21(); //0x58
        virtual void CBattleState_UnkVirtualFunc22(); //0x5C
        virtual void CBattleState_UnkVirtualFunc23(); //0x60
        virtual void CBattleState_UnkVirtualFunc24(); //0x64
        virtual void CBattleState_UnkVirtualFunc25(); //0x68
        virtual void CBattleState_UnkVirtualFunc26(); //0x6C
        virtual void CBattleState_UnkVirtualFunc27(); //0x70
        virtual void CBattleState_UnkVirtualFunc28(); //0x74
        virtual void CBattleState_UnkVirtualFunc29(); //0x78
        virtual void CBattleState_UnkVirtualFunc30(); //0x7C
        virtual void CBattleState_UnkVirtualFunc31(); //0x80
        virtual void CBattleState_UnkVirtualFunc32(); //0x84
        virtual void CBattleState_UnkVirtualFunc33(); //0x88

        CBattleState();

        u16 unk4;
        u16 unk6;
        u8 unk8[0x1520];
        u8 unk1528[4];
        u8 unk152C[0x80];
        u8 unk15AC[0x15DC - 0x15AC];
    };
}

// symbols.txt mangles Fv; retail leaves the arg entry in r4 (same pattern
// as cf::CAIAction's UnkVirtualFunc1/2).
extern "C" void CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg);

// symbols.txt mangles Fv; retail leaves the caller's mask in r4 (same ABI
// pattern as CBattleState_UnkVirtualFunc6).
extern "C" void CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 mask);

// symbols.txt mangles Fv; retail leaves the id in r4 (same fake-Fv ABI as
// UnkVirtualFunc6 above).
extern "C" int CBattleState_UnkVirtualFunc31__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id);

namespace cf {
    struct CBattleStateSrcEntry;
}

// symbols.txt mangles Fv; retail leaves the source table pointer in r4
// (same ABI pattern as UnkVirtualFunc6 above).
extern "C" void CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv(
    cf::CBattleState* self, const cf::CBattleStateSrcEntry* src);

// symbols.txt mangles Fv; retail leaves the entry arg in r4 (same fake-Fv
// ABI as UnkVirtualFunc6 above).
extern "C" void CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* entry);

// symbols.txt mangles Fv; retail leaves the entry arg in r4 (same fake-Fv
// ABI as UnkVirtualFunc6/8). Matches on unk2E, then clears matching slots.
extern "C" void CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg);
