#pragma once

#include <types.h>

namespace cf {
    class UnkClass_CActorParam15E0;
    struct CBattleStateSrcEntry;

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
        virtual int CBattleState_UnkVirtualFunc3();  //0x10
        virtual void CBattleState_UnkVirtualFunc4();  //0x14
        virtual void CBattleState_UnkVirtualFunc5(CBattleStateEntry* entry);  //0x18
        virtual void CBattleState_UnkVirtualFunc6(CBattleStateEntry* entry);  //0x1C
        virtual void CBattleState_UnkVirtualFunc7();  //0x20
        virtual void CBattleState_UnkVirtualFunc8(CBattleStateEntry* entry);  //0x24
        virtual void CBattleState_UnkVirtualFunc9();  //0x28
        virtual void CBattleState_UnkVirtualFunc10(CBattleStateEntry* arg); //0x2C
        virtual void CBattleState_UnkVirtualFunc11(u32 mask); //0x30
        virtual void CBattleState_UnkVirtualFunc12(); //0x34
        virtual void* CBattleState_UnkVirtualFunc13(int index); //0x38
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
        virtual void CBattleState_UnkVirtualFunc26(const CBattleStateSrcEntry* src); //0x6C
        virtual void CBattleState_UnkVirtualFunc27(); //0x70
        virtual void CBattleState_UnkVirtualFunc28(); //0x74
        virtual void CBattleState_UnkVirtualFunc29(); //0x78
        virtual void CBattleState_UnkVirtualFunc30(); //0x7C
        virtual int CBattleState_UnkVirtualFunc31(u32 id); //0x80
        virtual void CBattleState_UnkVirtualFunc32(); //0x84
        virtual int CBattleState_UnkVirtualFunc33(u32 id); //0x88

        CBattleState();

        u16 unk4;
        u16 unk6;
        u8 unk8[0x1520];
        u8 unk1528[4];
        u8 unk152C[0x80];
        u8 unk15AC[0x15D8 - 0x15AC];
        UnkClass_CActorParam15E0* field_0x15D8;
    };
}
