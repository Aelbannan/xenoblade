#include "kyoshin/cf/object/CAIAction.hpp"

#include <cstring>

// Batch 2026-07-14e: aiaction-ctor owns CAIAction::CAIAction()
// Batch 2026-07-14f: aiaction-vfunc1 owns CAIAction_UnkVirtualFunc1 (Fv)

namespace cf {

CAIAction::CAIAction() {
    // Retail homes: r31=p (then end2), r30=q, r29=end, r28=this.
    // Reuse p as the second-loop limit so it coalesces with end2, not q.
    u8* p;
    u8* q;
    u8* end;
    void* slotBase;

    p = slots;
    end = (u8*)this + 0x20C;

    unk8 = 0;

    do {
        std::memset(p + 4, 0, 0xE);
        std::memset(p, 0, 0x20);
        p += 0x20;
    } while (p < end);

    slotBase = slots;
    unk218 = 0x10;
    q = (u8*)this + 0x21C;
    p = (u8*)this + 0xADC;
    *(void**)end = slotBase;
    unk214 = 0;
    unk210 = 0;

    do {
        std::memset(q, 0, 0xE);
        q += 0xE;
    } while (q < p);

    std::memset((u8*)this + 0xAE0, 0, 0xE);
    std::memset(trailer, 0, 0x20);

    unkAFC = 1;
    unk4 = 1;
    unkB00 = 0;
    unkB10 = 0;
    unkB14 = 0;
    unkB18 = 0;
}

} // namespace cf

// Soft-cap ~96.02941%: MWCC fuses first dest store to stwux; retail is stwx+add
// (src in r9, 8-then-4). slots[]/dstBytes identical; dual buffer aliases regress.
// EQUIVALENT blocked by ring-loop bounds / Z3 timeout. No insn_patches/asm void.
void CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* outA,
                                                              cf::CAIActionExport* outB) {
    cf::CAIActionSlot* trailer = (cf::CAIActionSlot*)self->trailer;

    outA->unk00 = trailer->unk00;
    {
        u32 a = trailer->unk04;
        u32 b = trailer->unk08;
        outA->unk08 = b;
        outA->unk04 = a;
    }
    outA->unk0C = trailer->unk0C;
    outA->unk10 = trailer->unk10;
    outA->unk12 = trailer->unk12;
    outA->unk14 = trailer->unk14;
    outA->unk18 = trailer->unk18;
    outA->unk1C = trailer->unk1C;

    outB->unk208 = 0;
    outB->unk204 = 0;

    for (u32 i = 0; i < self->unk214; i++) {
        u32 ringIdx = (self->unk210 + i) % self->unk218;
        int outIdx = (int)(outB->unk204 + outB->unk208) % (int)outB->unk20C;
        cf::CAIActionSlot* src =
            (cf::CAIActionSlot*)((u8*)self->unk20C + (ringIdx << 5));

        cf::CAIActionSlot* dst =
            (cf::CAIActionSlot*)((u8*)outB->buffer + ((u32)outIdx << 5));

        dst->unk00 = src->unk00;
        dst->unk08 = src->unk08;
        dst->unk04 = src->unk04;
        dst->unk0C = src->unk0C;
        dst->unk10 = src->unk10;
        dst->unk12 = src->unk12;
        dst->unk14 = src->unk14;
        dst->unk18 = src->unk18;
        dst->unk1C = src->unk1C;

        outB->unk208 = outB->unk208 + 1;
    }
}

// Batch 2026-07-14g: aiaction-vfunc2 owns CAIAction_UnkVirtualFunc2 (Fv)
// Inverse of UnkVirtualFunc1: imports trailer from inA, then drains ring
// entries from inB into this->unk20C.
void CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* inA,
                                                              cf::CAIActionExport* inB) {
    cf::CAIActionSlot* trailer = (cf::CAIActionSlot*)self->trailer;
    u32 i;

    self->unk214 = 0;
    i = 0;
    self->unk210 = 0;

    trailer->unk00 = inA->unk00;
    {
        u32 a = inA->unk04;
        u32 b = inA->unk08;
        trailer->unk08 = b;
        trailer->unk04 = a;
    }
    trailer->unk0C = inA->unk0C;
    trailer->unk10 = inA->unk10;
    trailer->unk12 = inA->unk12;
    trailer->unk14 = inA->unk14;
    trailer->unk18 = inA->unk18;
    trailer->unk1C = inA->unk1C;

    while (i < inB->unk208) {
        u32 srcIdx = (inB->unk204 + i) % inB->unk20C;
        int dstIdx = (int)(self->unk210 + self->unk214) % (int)self->unk218;
        cf::CAIActionSlot* src =
            (cf::CAIActionSlot*)((u8*)inB->buffer + (srcIdx << 5));
        cf::CAIActionSlot* dst =
            (cf::CAIActionSlot*)((u8*)self->unk20C + (dstIdx << 5));

        dst->unk00 = src->unk00;
        {
            u32 t8 = src->unk08;
            u32 t4 = src->unk04;
            dst->unk04 = t4;
            dst->unk08 = t8;
        }
        dst->unk0C = src->unk0C;
        dst->unk10 = src->unk10;
        dst->unk12 = src->unk12;
        dst->unk14 = src->unk14;
        dst->unk18 = src->unk18;
        dst->unk1C = src->unk1C;

        self->unk214 = self->unk214 + 1;
        i = i + 1;
    }
}

void func_8014A8F8__Fv() {}
void func_8014AA10(void* obj, unsigned int value) {
    *(unsigned int*)((unsigned char*)obj + 0xB14) = value;
}

// Appends a slot to the ring buffer, inserting before the current head
// (decrementing unk210 with modulo wraparound). If an existing ring entry
// already matches bytes [0x5..0xD] of `in`, returns 0 (duplicate); otherwise
// stores the slot, increments unk214, sets bit 0x8 in the new slot's flags,
// and returns 1.
u32 func_8014AC38(cf::CAIAction* self, const cf::CAIActionSlot* in) {
    u32 count = self->unk214;
    for (u32 i = 0; i < count; i++) {
        u32 ringIdx = (self->unk210 + i) % self->unk218;
        const u8* sb = (const u8*)self->unk20C + (ringIdx << 5);
        const u8* ib = (const u8*)in;
        if (sb[0x5] != ib[0x5] || sb[0x6] != ib[0x6] || sb[0x7] != ib[0x7] ||
            sb[0x8] != ib[0x8] || sb[0x9] != ib[0x9] || sb[0xA] != ib[0xA] ||
            sb[0xB] != ib[0xB] || sb[0xC] != ib[0xC] || sb[0xD] != ib[0xD])
            return 0;
    }
    // Decrement head with wraparound.
    s32 newHead = (s32)self->unk210 - 1;
    if (newHead < 0)
        newHead += (s32)self->unk218;
    self->unk210 = (u32)newHead;
    cf::CAIActionSlot* dst =
        (cf::CAIActionSlot*)((u8*)self->unk20C + (self->unk210 << 5));
    dst->unk00 = in->unk00;
    dst->unk04 = in->unk04;
    dst->unk08 = in->unk08;
    dst->unk0C = in->unk0C;
    dst->unk10 = in->unk10;
    dst->unk12 = in->unk12;
    dst->unk14 = in->unk14;
    dst->unk18 = in->unk18;
    dst->unk1C = in->unk1C;
    self->unk214 = self->unk214 + 1;
    // Re-resolve the new slot pointer and OR bit 0x8 into its flags.
    {
        cf::CAIActionSlot* slot =
            (cf::CAIActionSlot*)((u8*)self->unk20C + (self->unk210 << 5));
        slot->unk10 = slot->unk10 | 0x8;
    }
    return 1;
}
void func_8014AE00(){}
void func_8014B120(){}
void* func_8014B2DC(void* p) {
    return memset((char*)p + 0xADC, 0, 0x20);
}
void func_8014B2EC(void* self, float delta) {
    struct Entry {
        unsigned char pad0[0x14];
        float value;
        unsigned char pad1[0x08];
    };
    struct Object {
        unsigned char pad0[8];
        unsigned short flags;
        unsigned char pad1[0x202];
        Entry* entries;
        unsigned int start;
        unsigned int count;
        unsigned int period;
    };
    Object* object = static_cast<Object*>(self);
    if ((object->flags & 2u) != 0)
        return;
    unsigned int i = 0;
    while (i < object->count) {
        unsigned int index = (object->start + i) % object->period;
        object->entries[index].value -= delta;
        ++i;
    }
}
void func_8014B344(){}
void func_8014B804(unsigned char* self, int index, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13) { unsigned char* base = self + index * 14; base[0x21c] = a2; base[0x21d] = a3; base[0x21e] = a4; base[0x21f] = a5; base[0x220] = a6; base[0x221] = a7; base[0x222] = a8; base[0x223] = a9; base[0x224] = a10; base[0x225] = a11; base[0x226] = a12; *(unsigned short*)(base + 0x228) = a13; if (a7 == 11 || a9 == 11) *(unsigned short*)(base + 0x228) |= 1; if (a7 == 10 || a9 == 10) *(unsigned short*)(base + 0x228) |= 1; if (a7 == 7 || a9 == 7) *(unsigned short*)(base + 0x228) |= 2; }
void func_801537E0(void* self) {
    *(u16*)((u8*)self + 8) &= ~0x0006;
}

void func_8014A86C__FPv(){}
void func_8014B7B0(){}
void func_8014B8BC(){}
void func_8014CE78(){}
void func_8014E164(){}
// 0x20-byte query struct passed as the second arg to func_80150618.
// Layout mirrors CAIActionSlot: byte at 0x6 (== 0x25 short-circuit), byte at
// 0xD (mapped to a flag via (b+0xCB)&0xFF and ==0x3B), u16 flags at 0x10
// (bit 0x400 set in two paths), pointer at 0x18 (dispatch), and u32 at 0x1C
// (set to 6 when result is null).
struct CAIActionQuery {
    u32 unk00; // 0x00
    u32 unk04; // 0x04
    u32 unk08; // 0x08
    u32 unk0C; // 0x0C
    u16 unk10; // 0x10
    s16 unk12; // 0x12
    f32 unk14; // 0x14
    u32 unk18; // 0x18
    u32 unk1C; // 0x1C
};

// Stack-allocated 8-byte holder (list + handle) used by func_80043D90.
struct CAIActionEnumHolder {
    void* list;  // 0x0
    u32 handle;  // 0x4
};

// Returns the result of dispatching the query through self's CfObjectMove
// vtable (or the cached handle at 0x3f10), with bit 0x400 in the query's
// flags set in two pre-dispatch paths.
void* func_80150618(cf::CAIAction* self, CAIActionQuery* in) {
    CAIActionEnumHolder holder;
    void* result = 0;
    u8* selfB14 = (u8*)self->unkB14;

    func_80043D90(&holder);

    u32 subObj = in->unk18;
    if (subObj == 0 || *(u16*)((u8*)subObj + 0x3C) == 3) {
        u8 byteD = (u8)(in->unk0C >> 24);
        u8 sum = (u8)(byteD + 0xCB);
        if (sum <= 2 || byteD == 0x3B)
            in->unk10 = in->unk10 | 0x400;
    } else if (*(u16*)((u8*)subObj + 0x3C) == 4) {
        in->unk10 = in->unk10 | 0x400;
    } else {
        in->unk10 = in->unk10 | 0x400;
    }

    if ((u8)(in->unk04 >> 16) == 0x25) {
        result = (void*)in->unk00;
        __dt__80043E88(&holder, -1);
        return result;
    }

    // func_80150828 returns the primary handle; on success one of the
    // three sub-cases (0x1/0x2/0x3 by in->unk18->unk5C) refines it.
    result = func_80150828(self, in);

    u32 ref = in->unk18;
    if (ref != 0) {
        u16 which = *(u16*)((u8*)ref + 0x5C);
        if (which == 1) {
            if (result != 0) {
                func_800B708C((BOOL)(uintptr_t)result);
                void* obj = func_8016FE34();
                if (obj == 0) {
                    __dt__80043E88(&holder, -1);
                    return 0;
                }
                u8* moveBase = selfB14 + 0x3e9c;
                u32 flags64 = *(u32*)(moveBase + 0x64);
                if ((flags64 & 0x20000000) != 0) {
                    if ((*(u32*)((u8*)obj + 0x3f00) & 0x40000000) != 0)
                        goto dispatched;
                }
                if ((flags64 & 0x40000000) != 0) {
                    if ((*(u32*)((u8*)obj + 0x3f00) & 0x20000000) != 0)
                        goto dispatched;
                }
                // vtable[0x4C] on the move base.
                void* (*vt)(void*) = (void* (*)(void*))(*(u32*)((u8*)moveBase + 0x4C) ? 0 : 0);
                (void)vt;
                result = ((void* (*)(void*))(*(u32*)(*(u32*)moveBase) + 0x4C))(moveBase);
            }
        } else if (which == 2) {
            void* v = *(void**)(selfB14 + 0x3f10);
            if (result != v)
                result = v;
        } else if (which == 3) {
            if (result != 0) {
                func_800B708C((BOOL)(uintptr_t)result);
                void* obj = func_8016FE34();
                if (obj == 0) {
                    __dt__80043E88(&holder, -1);
                    return 0;
                }
                u32 flags3f00 = *(u32*)(selfB14 + 0x3f00);
                if ((flags3f00 & 0x20000000) != 0) {
                    if ((*(u32*)((u8*)obj + 0x3f00) & 0x20000000) != 0)
                        goto dispatched;
                }
                if ((flags3f00 & 0x40000000) != 0) {
                    if ((*(u32*)((u8*)obj + 0x3f00) & 0x40000000) != 0)
                        goto dispatched;
                }
                result = *(void**)(selfB14 + 0x3f10);
            }
        }
    }

dispatched:
    if (result == 0)
        in->unk1C = 6;
    __dt__80043E88(&holder, -1);
    return result;
}
void func_80150828(){}
void func_801522C4(){}
void func_801537F0(){}
void func_8015396C(){}
