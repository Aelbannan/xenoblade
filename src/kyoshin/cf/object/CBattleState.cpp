#include "kyoshin/cf/object/CBattleState.hpp"

extern "C" void* memset(void* dest, int val, size_t count);

namespace cf {

CBattleState::CBattleState() {
    u8* p;
    u8* end;

    unk4 = 0;
    unk6 = 0;
    memset(unk8, 0, 0x1520);

    p = unk152C;
    end = (u8*)this + 0x15AC;
    do {
        memset(p, 0, 0x10);
        p += 0x10;
    } while (p < end);

    memset(unk152C, 0, 0x80);
    memset(this, 0, 0x15D4);
}

} // namespace cf

// Batch 2026-07-14g: battlestate-vfunc6 owns CBattleState_UnkVirtualFunc6
// exclusively. Do not touch CBattleState::CBattleState() above.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// pattern as cf::CAIAction's UnkVirtualFunc1/2 — see
// docs/MWCC_REFERENCE.md).
//
// sdata2 float pool constant read via lbl_eu_80667414@sda21 (0.9f).
extern "C" const float lbl_eu_80667414;

extern "C" void CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc17Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    u8* wordPtr;
    cf::CBattleStateEntry* entries;
    cf::CBattleStateEntry* p;
    int n;

    // Bit `arg->unk0C` set into the this+0x15AC bitfield (word-aligned byte
    // offset = (id >> 3) & ~3, bit position = id & 0x1F).
    wordPtr = self->unk15AC + (((u32)arg->unk0C >> 3) & ~3u);
    *(u32*)wordPtr |= 1u << (arg->unk0C & 0x1F);

    entries = (cf::CBattleStateEntry*)((u8*)self + 0x1388);

    p = entries;
    for (n = 8; n != 0; n--, p++) {
        if (p->unk0C == arg->unk0C) {
            if (p->unk10 < arg->unk10) {
                p->unk10 = arg->unk10;
            }
            if (arg->unk18 != 0) {
                if (p->unk10 > arg->unk18) {
                    p->unk10 = arg->unk18;
                }
            }
            ((Vfunc17Fn)(*(void***)self)[18])(self, p);
            return;
        }
    }

    for (n = 8; n != 0; n--, entries++) {
        if (entries->unk0C == 0) {
            entries->unk00 = arg->unk00;
            entries->unk04 = arg->unk04;
            entries->unk08 = arg->unk08;
            entries->unk0C = arg->unk0C;
            entries->unk10 = arg->unk10;
            entries->unk14 = arg->unk14;
            entries->unk16 = arg->unk16;
            entries->unk18 = arg->unk18;
            entries->unk1A = arg->unk1A;
            entries->unk1C = arg->unk1C;
            entries->unk20 = arg->unk20;
            entries->unk24 = arg->unk24;
            {
                f32 scaled = lbl_eu_80667414 * arg->unk24;
                entries->unk28 = arg->unk28;
                entries->unk2C = arg->unk2C;
                entries->unk2E = arg->unk2E;
                entries->unk30 = arg->unk30;
                entries->unk1C = arg->unk20;
                entries->unk28 = scaled;
            }
            ((Vfunc17Fn)(*(void***)self)[18])(self, entries);
            return;
        }
    }
}
