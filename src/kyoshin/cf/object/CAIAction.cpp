#include <types.h>
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/object/CfObjectMoveApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
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

void func_8014A8F8__Fv() {
    lbl_eu_806641B0 = 0;
}
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
        // Byte-wise duplicate test over [0x5..0xD]; retail materializes each
        // comparison into a "duplicate" flag via an if/else-if chain.
        u32 dup;
        if (sb[0x5] != ib[0x5]) dup = 0;
        else if (sb[0x6] != ib[0x6]) dup = 0;
        else if (sb[0x7] != ib[0x7]) dup = 0;
        else if (sb[0x8] != ib[0x8]) dup = 0;
        else if (sb[0x9] != ib[0x9]) dup = 0;
        else if (sb[0xA] != ib[0xA]) dup = 0;
        else if (sb[0xB] != ib[0xB]) dup = 0;
        else if (sb[0xC] != ib[0xC]) dup = 0;
        else dup = (u32)(sb[0xD] == ib[0xD]);
        if (dup)
            return 0;
    }
    // Decrement head with wraparound.
    self->unk210 = self->unk210 - 1;
    if ((s32)self->unk210 < 0)
        self->unk210 = self->unk210 + self->unk218;
    cf::CAIActionSlot* dst =
        (cf::CAIActionSlot*)((u8*)self->unk20C + (self->unk210 << 5));
    *dst = *in;
    self->unk214 = self->unk214 + 1;
    // Re-resolve the new slot pointer and OR bit 0x8 into its flags.
    cf::CAIActionSlot* slot =
        (cf::CAIActionSlot*)((u8*)self->unk20C + (self->unk210 << 5));
    slot->unk10 = slot->unk10 | 0x8;
    return 1;
}
// Removes the action slot found at ring offset i whose flag bit 0x10 (at
// +0x10) is set: clears it out of the ring by compacting from whichever side
// of the ring is closer (shift entries down and bump the head, or shift
// entries up), decrementing unk214. The removed slot's float is read into a
// scratch local first.
void func_8014AE00(cf::CAIAction* self) {
    for (u32 i = 0; i < self->unk214; i++) {
        cf::CAIActionSlot* slot =
            (cf::CAIActionSlot*)((u8*)self->unk20C +
                                 (((self->unk210 + i) % self->unk218) << 5));
        if ((slot->unk10 & 0x8) == 0)
            continue;

        // Scratch copy of the removed entry's float (kept in retail).
        volatile f32 removedValue = slot->unk14;
        (void)removedValue;

        u32 newCount = self->unk214 - 1;
        self->unk214 = newCount;
        s32 mid = (s32)newCount / 2;

        if ((s32)i < mid) {
            // Near the head: move entries [0, i) one slot toward the tail,
            // then advance the ring head past the vacated front slot.
            for (s32 j = (s32)i - 1; j >= 0; j--) {
                s32 cap = (s32)self->unk218;
                s32 pos = (s32)self->unk210 + j;
                s32 q = pos / cap;
                s32 cur = pos - q * cap;
                s32 nxt = cur + 1;
                s32 q2 = nxt / cap;
                cf::CAIActionSlot* src =
                    (cf::CAIActionSlot*)((u8*)self->unk20C + ((u32)cur << 5));
                cf::CAIActionSlot* dst = (cf::CAIActionSlot*)(
                    (u8*)self->unk20C + ((u32)(nxt - q2 * cap) << 5));
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
            }
            s32 head = (s32)self->unk210 + 1;
            self->unk210 = (u32)((head % (s32)self->unk218));
        } else {
            // Near the tail: pull entries [i+1, count) one slot toward the
            // front, leaving the head in place.
            for (u32 j = i; j < self->unk214; j++) {
                s32 cap = (s32)self->unk218;
                s32 pos = (s32)self->unk210 + j;
                s32 q = pos / cap;
                s32 cur = pos - q * cap;
                s32 nxt = cur + 1;
                s32 q2 = nxt / cap;
                cf::CAIActionSlot* src =
                    (cf::CAIActionSlot*)((u8*)self->unk20C + ((u32)cur << 5));
                cf::CAIActionSlot* dst = (cf::CAIActionSlot*)(
                    (u8*)self->unk20C + ((u32)(nxt - q2 * cap) << 5));
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
            }
        }
        // Compensate for the loop increment: the slot at offset i now holds
        // the next entry, so it must be re-examined.
        i--;
    }
}
int func_8014B120(cf::CAIAction* self, const cf::CAIActionSlot* in) {
    u32 i;
    for (i = 0; i < self->unk214; i++) {
        u32 cap = self->unk218;
        u32 pos = self->unk210 + i;
        const u8* sb = (const u8*)self->unk20C + ((pos % cap) << 5);
        // Byte-wise duplicate test over [0x5..0xD].
        u32 dup;
        if (sb[5] != ((const u8*)in)[5]) dup = 0;
        else if (sb[6] != ((const u8*)in)[6]) dup = 0;
        else if (sb[7] != ((const u8*)in)[7]) dup = 0;
        else if (sb[8] != ((const u8*)in)[8]) dup = 0;
        else if (sb[9] != ((const u8*)in)[9]) dup = 0;
        else if (sb[10] != ((const u8*)in)[10]) dup = 0;
        else if (sb[11] != ((const u8*)in)[11]) dup = 0;
        else if (sb[12] != ((const u8*)in)[12]) dup = 0;
        else dup = (u32)(sb[13] == ((const u8*)in)[13]);
        if (dup)
            return 0;
    }
    // Append at the tail of the ring; retail computes the index with signed
    // modulo here (divw) unlike the unsigned loop above.
    s32 abs = (s32)(self->unk210 + self->unk214);
    s32 capS = (s32)self->unk218;
    s32 quot = abs / capS;
    cf::CAIActionSlot* dst =
        (cf::CAIActionSlot*)((u8*)self->unk20C + ((abs - quot * capS) << 5));
    *dst = *in;
    self->unk214 = self->unk214 + 1;
    return 1;
}
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
// extern "C" per the CfObjectPc.hpp declaration (retail symbol is unmangled;
// CfObjectPc.cpp imports it under that exact name).
extern "C" void func_8014B804(unsigned char* self, int index, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13) { unsigned char* base = self + index * 14; base[0x21c] = a2; base[0x21d] = a3; base[0x21e] = a4; base[0x21f] = a5; base[0x220] = a6; base[0x221] = a7; base[0x222] = a8; base[0x223] = a9; base[0x224] = a10; base[0x225] = a11; base[0x226] = a12; *(unsigned short*)(base + 0x228) = a13; if (a7 == 11 || a9 == 11) *(unsigned short*)(base + 0x228) |= 1; if (a7 == 10 || a9 == 10) *(unsigned short*)(base + 0x228) |= 1; if (a7 == 7 || a9 == 7) *(unsigned short*)(base + 0x228) |= 2; }
void func_801537E0(void* self) {
    *(u16*)((u8*)self + 8) &= ~0x0006;
}

// Loads the AI config table pointer into lbl_eu_806641B0 and walks the
// variable-length entries once (copying each art name to a scratch buffer),
// so that later lookups can rely on the cached table pointer.
void func_8014A86C(void* tableArg) {
    cf::CAIActionTable* table = (cf::CAIActionTable*)tableArg;
    lbl_eu_806641B0 = table;
    u32 raw = table->count;
    s32 count = (s32)(u16)__rlwimi((raw << 8) & 0xFF00, raw, 24, 24, 31);
    const u8* ep = (const u8*)table + 2;
    char name[0x11];
    for (s32 i = 0; i < count; i++) {
        std::memcpy(name, ep + 4, 0x10);
        name[0x10] = 0;
        const u8* mid = ep + ep[0x14] * 0xC;
        ep = mid + 0x16;
    }
}
// memset the 160-element x 14-byte action table at +0x21C.
extern "C" void func_8014B7B0(void* self) {
    void* p = (u8*)self + 0x21C;
    for (int i = 0; i < 160; i++) {
        memset(p, 0, 14);
        p = (u8*)p + 14;
    }
}
void func_8014B8BC(){}
void func_8014CE78(){}
void func_8014E164(){}
// Returns the result of dispatching the query through the party's move
// vtable (or the cached battle handle at 0x3F10), with bit 0x400 in the
// query's flags set in two pre-dispatch paths.
typedef void* (*CAIMoveSlot4C)(CAIPartyMoveObj*);

void* func_80150618(cf::CAIAction* self, CAIActionQuery* in) {
    CAIActionEnumHolder holder;
    void* result;

    func_80043D90(&holder);

    CAIQueryTarget* tgt = in->unk18;
    if (tgt != 0 && tgt->unk3C != 3 && tgt->unk3C != 4) {
        in->unk10 = in->unk10 | 0x400;
    } else {
        // Action-class filter: bytes 0xCB..0xCD or the 0x3B sentinel also
        // request the 0x400 flag.
        u8 byteD = in->unk0D;
        u8 sum = byteD + 0xCB;
        if (sum <= 2 || byteD == 0x3B)
            in->unk10 = in->unk10 | 0x400;
    }

    if (in->unk06 == 0x25) {
        result = (void*)in->unk00;
        __dt__80043E88(&holder, -1);
        return result;
    }

    // func_80150828 returns the primary handle; on success one of the three
    // sub-cases (1/2/3 by target unk5C) refines it.
    result = func_80150828(self, in);

    tgt = in->unk18;
    if (tgt != 0) {
        s16 which = tgt->unk5C;
        if (which == 1) {
            if (result != 0) {
                void* obj = func_8016FE34(func_800B708C((int)(uintptr_t)result));
                if (obj == 0) {
                    __dt__80043E88(&holder, -1);
                    return 0;
                }
                // Cross-pair exclusion: party bit 2 (0x2) blocks when the
                // candidate has bit 4 (0x4), and vice versa; otherwise take
                // the move object's own vtable slot 0x4C result.
                CAIPartyObj* party = (CAIPartyObj*)self->unkB14;
                CAIPartyMoveObj* move = &party->move;
                u32 mf = move->moveFlags;
                u32 of = ((CAIPartyObj*)obj)->move.moveFlags;
                if (!((mf & 2) && (of & 4)) && !((mf & 4) && (of & 2))) {
                    result = ((CAIMoveSlot4C*)move->vtable)[0x13](move);
                }
            }
        } else if (which == 2) {
            CAIPartyObj* party = (CAIPartyObj*)self->unkB14;
            void* v = (void*)party->unk3F10;
            if (result != v)
                result = v;
        } else if (which == 3) {
            if (result != 0) {
                void* obj = func_8016FE34(func_800B708C((int)(uintptr_t)result));
                if (obj == 0) {
                    __dt__80043E88(&holder, -1);
                    return 0;
                }
                // Same-pair exclusion: matching bits suppress the fallback.
                CAIPartyObj* party = (CAIPartyObj*)self->unkB14;
                u32 mf = party->move.moveFlags;
                u32 of = ((CAIPartyObj*)obj)->move.moveFlags;
                if (!((mf & 2) && (of & 2)) && !((mf & 4) && (of & 4))) {
                    result = (void*)party->unk3F10;
                }
            }
        }
    }

    if (result == 0)
        in->unk1C = 6;
    __dt__80043E88(&holder, -1);
    return result;
}

// Returns the dispatch result for the query (see func_80150618 above; this
// comment block previously held an untyped draft implementation).
struct CAIVtObj {
    virtual void* v000();
    virtual void* v001();
    virtual void* v002();
    virtual void* v003();
    virtual void* v004();
    virtual void* v005();
    virtual void* v006();
    virtual void* v007();
    virtual void* v008();
    virtual void* v009();
    virtual void* v00A();
    virtual void* v00B();
    virtual void* v00C();
    virtual void* v00D();
    virtual void* v00E();
    virtual void* v00F();
    virtual void* v010();
    virtual void* v011();
    virtual void* v012();
    virtual void* v013();
    virtual void* v014();
    virtual void* v015();
    virtual void* v016();
    virtual void* v017();
    virtual void* v018();
    virtual void* v019();
    virtual void* v01A();
    virtual void* v01B();
    virtual void* v01C();
    virtual void* v01D();
    virtual void* v01E();
    virtual void* v01F();
    virtual void* v020();
    virtual void* v021();
    virtual void* v022();
    virtual void* v023();
    virtual void* v024();
    virtual void* v025();
    virtual void* v026();
    virtual void* v027();
    virtual void* v028();
    virtual void* v029();
    virtual void* v02A();
    virtual void* v02B();
    virtual void* v02C();
    virtual void* v02D();
    virtual void* v02E();
    virtual void* v02F();
    virtual void* v030();
    virtual void* v031();
    virtual void* v032();
    virtual void* v033();
    virtual void* v034();
    virtual void* v035();
    virtual s32 v036();
    virtual void* v037();
    virtual s32 v038();
    virtual void* v039();
    virtual void* v03A();
    virtual void* v03B();
    virtual void* v03C();
    virtual void* v03D();
    virtual void* v03E();
    virtual void* v03F();
    virtual void* v040();
    virtual void* v041();
    virtual void* v042();
    virtual void* v043();
    virtual void* v044();
    virtual void* v045();
    virtual void* v046();
    virtual void* v047();
    virtual void* v048();
    virtual void* v049();
    virtual void* v04A();
    virtual void* v04B();
    virtual void* v04C();
    virtual void* v04D();
    virtual void* v04E();
    virtual void* v04F();
    virtual void* v050();
    virtual void* v051();
    virtual void* v052();
    virtual void* v053();
    virtual void* v054();
    virtual void* v055();
    virtual void* v056();
    virtual void* v057();
    virtual void* v058();
    virtual void* v059();
    virtual void* v05A();
    virtual void* v05B();
    virtual void* v05C();
    virtual void* v05D();
    virtual void* v05E();
    virtual void* v05F();
    virtual void* v060();
    virtual void* v061();
    virtual void* v062();
    virtual void* v063();
    virtual void* v064();
    virtual void* v065();
    virtual void* v066();
    virtual void* v067();
    virtual void* v068();
    virtual void* v069();
    virtual void* v06A();
    virtual void* v06B();
    virtual void* v06C();
    virtual void* v06D();
    virtual void* v06E();
    virtual void* v06F();
    virtual void* v070();
    virtual void* v071();
    virtual void* v072();
    virtual void* v073();
    virtual void* v074();
    virtual void* v075();
    virtual void* v076();
    virtual void* v077();
    virtual void* v078();
    virtual void* v079();
    virtual void* v07A();
    virtual void* v07B();
    virtual void* v07C();
    virtual void* v07D();
    virtual void* v07E();
    virtual void* v07F();
    virtual void* v080();
    virtual void* v081();
    virtual void* v082();
    virtual void* v083();
    virtual void* v084();
    virtual void* v085();
    virtual void* v086();
    virtual void* v087();
    virtual void* v088();
    virtual void* v089();
    virtual void* v08A();
    virtual void* v08B();
    virtual void* v08C();
    virtual void* v08D();
    virtual void* v08E();
    virtual void* v08F();
    virtual void* v090();
    virtual void* v091();
    virtual void* v092();
    virtual void* v093();
    virtual void* v094();
    virtual void* v095();
    virtual void* v096();
    virtual void* v097();
    virtual void* v098();
    virtual void* v099();
    virtual void* v09A();
    virtual void* v09B();
    virtual void* v09C();
    virtual void* v09D();
    virtual void* v09E();
    virtual void* v09F();
    virtual void* v0A0();
    virtual void* v0A1();
    virtual void* v0A2();
    virtual void* v0A3();
    virtual void* v0A4();
    virtual void* v0A5();
    virtual void* v0A6();
    virtual void* v0A7();
    virtual void* v0A8();
    virtual void* v0A9();
    virtual void* v0AA();
    virtual void* v0AB();
    virtual void* v0AC();
    virtual void* v0AD();
    virtual void* v0AE();
    virtual void* v0AF();
    virtual void* v0B0();
    virtual void* v0B1();
    virtual void* v0B2();
    virtual void* v0B3();
    virtual void* v0B4();
    virtual void* v0B5();
    virtual void* v0B6();
    virtual void* v0B7();
    virtual void* v0B8();
    virtual void* v0B9();
    virtual void* v0BA();
    virtual void* v0BB();
    virtual void* v0BC();
    virtual void* v0BD();
    virtual void* v0BE();
    virtual void* v0BF();
    virtual s32 v0C0();
    virtual void* v0C1();
    virtual s32 v0C2();
    virtual void* v0C3();
    virtual void* v0C4();
    virtual void* v0C5();
    virtual void* v0C6();
    virtual void* v0C7();
    virtual void* v0C8();
    virtual void* v0C9();
    virtual void* v0CA();
    virtual void* v0CB();
};


struct CAIEnumList {
    u8 pad0[0x620];
    u32 count;      // 0x620
    u8 pad1[0x3030 - 0x624];
    u32 unk3030;    // 0x3030
};

// One list slot (CfObjEnumList element = 0x20-byte record).
struct CAIEnumSlot {
    u32 unk00;      // 0x00 payload value
    void* unk04;    // 0x04 object (sub-object at base+0x3E9C)
    u8 pad[0x18 - 0x08];
    u8 unk18;       // 0x18 exclusion flag byte (bits 0x38 tested by func_800F6E08)
    u8 pad2[3];
};

static inline u32 aiListCount(void* list) {
    return ((CAIEnumList*)list)->count;
}
static inline void aiListClear(void* list) {
    ((CAIEnumList*)list)->count = 0;
    ((CAIEnumList*)list)->unk3030 = 0;
}
static f32 mlAbs(f32 x) { return x < 0.0f ? -x : x; }

// moveBase = unkB14 + 0x3E9C; retail pattern is lwzu on the base+0x3E9C,
// so the vtable call always targets (base + 0x3E9C) even when base is 0.
static inline void* aiMoveBaseVt4C(void* partyBase) {
    CAIVtObj* mb = (CAIVtObj*)((u8*)partyBase + 0x3E9C);
    return mb->v011();
}

extern "C" void* func_801522C4(cf::CAIAction* self, const void* cmd) {
    CAIActionEnumHolder holder;
    const void* c = cmd;

    s32 op = (s32)((const u8*)c)[7];  // dispatch key A -- the "command"
    s32 sel = (s32)((const u8*)c)[8]; // dispatch key B -- the "selector"
    u32 countBefore;
    void* result = 0;

    func_80043D90(&holder);

    switch (sel) {
    case 6: {
        if (func_80148778((u8*)self->unkB14 + 8, 0x11)) {
            void* o = func_80149154((u8*)self->unkB14 + 8, 0x11);
            func_800F6D50(func_80043F18(&holder), *(u32*)((u8*)o + 0x10));
        } else if (self->unkB18) {
            void* v = func_8016FE34(func_800B708C((int)self->unkB18));
            if (v && ((CAIVtObj*)v)->v0AD() == 0)
                func_800F6D50(func_80043F18(&holder), (u32)self->unkB18);
            else
                self->unkB18 = 0;
        } else {
            void* v = func_8016FE34(func_800B708C((int)(uintptr_t)aiMoveBaseVt4C((u8*)self->unkB14)));
            if (v) {
                u8* moveBase = (u8*)self->unkB14 + 0x3E9C;
                u32 mf = *(u32*)(moveBase + 0x64);
                if (mf & 0x2) {
                    if (!(*(u32*)((u8*)v + 0x3F00) & 0x2))
                        func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)((CAIVtObj*)moveBase)->v011());
                } else if (!(*(u32*)((u8*)v + 0x3F00) & 0x4)) {
                    func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)((CAIVtObj*)moveBase)->v011());
                }
            }
        }
        break;
    }

    case 35:
        func_800F6D50(func_80043F18(&holder), *(u32*)((const u8*)c + 0x00));
        break;

    case 36: {
        void* v = func_8016FE34(func_800B708C(*(u32*)((const u8*)c + 0x00)));
        if (v)
            func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)aiMoveBaseVt4C((u8*)v));
        break;
    }

    case 1:
    case 10: {
        if (func_80148778((u8*)self->unkB14 + 8, 0x11)) {
            void* o = func_80149154((u8*)self->unkB14 + 8, 0x11);
            func_800F6D50(func_80043F18(&holder), *(u32*)((u8*)o + 0x10));
        } else if (self->unkB18) {
            void* v = func_8016FE34(func_800B708C((int)self->unkB18));
            if (v && ((CAIVtObj*)v)->v0AD() == 0)
                func_800F6D50(func_80043F18(&holder), (u32)self->unkB18);
            else
                self->unkB18 = 0;
        } else {
            u32 filter = 0x80000000;
            if (!(*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x4))
                filter = 0x20;
            func_800F4A98(func_80043F18(&holder), filter, 0x800);
            if (aiListCount(func_80043F18(&holder)) == 0)
                func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)aiMoveBaseVt4C((u8*)self->unkB14));
        }
        break;
    }

    case 2:
        func_800F6D50(func_80043F18(&holder), *(u32*)((u8*)self->unkB14 + 0x3F10));
        break;

    case 4:
        func_800F4A98(func_80043F18(&holder), 0x10, 0);
        break;

    case 5: {
        CAIActionEnumHolder h2;
        void* obj;
        func_80043D90(&h2);
        func_800F4A98(func_80043F18(&h2), 0x10, 0);
        if (aiListCount(func_80043F18(&h2)) != 0) {
            obj = func_800F6EAC(func_80043F18(&h2), 0);
            if (obj)
                obj = (u8*)obj - 0x3E9C;
            // unconditional lwzu deref (retail null path quirk).
            func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)aiMoveBaseVt4C(obj));
        }
        __dt__80043E88(&h2, -1);
        break;
    }

    case 7:
    case 9: {
        u32 filter = 0x20;
        if (*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x4)
            filter = 0x80000000;
        func_800F4A98(func_80043F18(&holder), filter, 0);
        break;
    }

    case 8: {
        u32 i;
        u32 filter = 0x20;
        if (*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x4)
            filter = 0x80000000;
        func_800F4A98(func_80043F18(&holder), filter, 0);
        for (i = 0; i < aiListCount(func_80043F18(&holder)); i++) {
            CAIEnumSlot* slot = (CAIEnumSlot*)func_800F6EC0(func_80043F18(&holder), i);
            void* obj = slot ? func_8016FE34(slot->unk04) : 0;
            if (obj && obj == (void*)self->unkB14) {
                slot->unk18 = slot->unk18 | 0x70;
                break;
            }
        }
        __ct__800FD250(func_80043F18(&holder));
        break;
    }

    case 11: {
        CAIActionEnumHolder h2;
        u32 i;
        func_80043D90(&h2);
        {
            u32 filter = 0x20;
            if (*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x4)
                filter = 0x80000000;
            func_800F4A98(func_80043F18(&h2), filter, 0);
        }
        func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)aiMoveBaseVt4C((u8*)self->unkB14));
        for (i = 0; i < aiListCount(func_80043F18(&h2)); i++) {
            void* sub = func_800F6EAC(func_80043F18(&h2), i);
            void* base = sub;
            if (sub)
                base = (u8*)sub - 0x3E9C;
            if (base != (void*)self->unkB14)
                func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)aiMoveBaseVt4C(base));
        }
        __dt__80043E88(&h2, -1);
        break;
    }

    case 12: {
        void* b = func_800EA444(getInstance__Q22cf14CBattleManagerFv());
        if (b != 0) {
            u32 val = *(u32*)((u8*)b + 0x00);
            if (val != 0)
                func_800F6D50(func_80043F18(&holder), val);
        }
        break;
    }

    case 13: {
        void* b = func_800EA444(getInstance__Q22cf14CBattleManagerFv());
        if (b != 0) {
            u32 val = *(u32*)((u8*)b + 0x04);
            if (val != 0)
                func_800F6D50(func_80043F18(&holder), val);
        }
        break;
    }

    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21: {
        void* node = func_800B76A4(func_800B07E8());
        s32 want = (s32)sel - 13;
        while (node) {
            if (want == (s32)(u16)*(u16*)((u8*)node + 0x8C)) {
                func_800F6D50(func_80043F18(&holder), *(u32*)((u8*)node + 0x74));
                break;
            }
            node = func_800B77E4(func_800B07E8(), node);
        }
        break;
    }

    case 34: {
        u32 filter = 0x80000000;
        if (!(*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x4))
            filter = 0x20;
        func_800F4A98(func_80043F18(&holder), filter, 0x800);
        __ct__800FC19C(func_80043F18(&holder), 0x10000000, 0);
        break;
    }

    default:
        break;
    }

    // ---- .L_80153344 : common pre-dispatch check ------------------------
    if (aiListCount(func_80043F18(&holder)) == 0) {
        __dt__80043E88(&holder, -1);
        return 0;
    }
    countBefore = aiListCount(func_80043F18(&holder));

    // ---- second dispatch on op (byte 7), 0..178 -------------------------
    switch (op) {
    case 144:
    case 145:
    case 146:
    case 147:
    case 148:
    case 149:
    case 150:
    case 151: {
        u32 i;
        void* list = func_80043F18(&holder);
        if (aiListCount(list) != 0) {
            for (i = 0; i < aiListCount(func_80043F18(&holder)); i++) {
                CAIEnumSlot* slot = (CAIEnumSlot*)func_800F6EC0(func_80043F18(&holder), i);
                void* obj = slot ? func_8016FE34(slot->unk04) : 0;
                if (!(obj && (*(u32*)((u8*)obj + 0x3F00) & 0x2) &&
                      (op - 143) == (s32)*(u16*)((u8*)obj + 0x3F28)))
                    slot->unk18 = slot->unk18 | 0x70;
            }
            __ct__800FD250(func_80043F18(&holder));
        }
        break;
    }

    case 152:
    case 153:
    case 154:
    case 155:
    case 156:
    case 157:
    case 158:
    case 159:
    case 160:
    case 161:
    case 162:
    case 163: {
        if (aiListCount(func_80043F18(&holder)) != 0) {
            CAIEnumSlot* slot = (CAIEnumSlot*)func_800F6EC0(func_80043F18(&holder), 0);
            void* obj = slot ? func_8016FE34(slot->unk04) : 0;
            if (!(obj && (*(u32*)((u8*)obj + 0x3F00) & 0x4) &&
                  ((CAIVtObj*)obj)->v036() == (s32)(op - 151)))
                aiListClear(func_80043F18(&holder));
        }
        break;
    }

    case 1: {
        void* obj;
        if (!func_80148778((u8*)self->unkB14 + 8, 0x11)) {
            aiListClear(func_80043F18(&holder));
            obj = ((CAIVtObj*)(u8*)self->unkB14)->v0B8();
            if (obj == 0) {
                obj = aiMoveBaseVt4C((u8*)self->unkB14);
                void* v = func_8016FE34(func_800B708C((int)(uintptr_t)obj));
                if (v == 0)
                    goto fallback1;
                if (((CAIVtObj*)v)->v0AD() != 0)
                    goto fallback1;
                if (v != 0)
                    goto done1;
            fallback1:
                if (*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x2)
                    obj = ((CAIVtObj*)getPlayer__Q22cf13CfGameManagerFi(0))->v011();
            }
        done1:
            func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)obj);
        }
        break;
    }

    case 2: {
        void* obj;
        if (!func_80148778((u8*)self->unkB14 + 8, 0x11)) {
            aiListClear(func_80043F18(&holder));
            obj = ((CAIVtObj*)(u8*)self->unkB14)->v0B9();
            if (obj == 0) {
                obj = aiMoveBaseVt4C((u8*)self->unkB14);
                void* v = func_8016FE34(func_800B708C((int)(uintptr_t)obj));
                if (v == 0)
                    goto fallback2;
                if (((CAIVtObj*)v)->v0AD() != 0)
                    goto fallback2;
                if (v != 0)
                    goto done2;
            fallback2:
                if (*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x2)
                    obj = ((CAIVtObj*)getPlayer__Q22cf13CfGameManagerFi(0))->v011();
            }
        done2:
            func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)obj);
        }
        break;
    }

    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        __ct__800FC648(func_80043F18(&holder), 0, (f32)(s32)(op - 18) / 10.0f);
        break;

    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
        __ct__800FC4FC(func_80043F18(&holder), 0, (f32)(s32)(op - 27) / 10.0f);
        __ct__800FC19C(func_80043F18(&holder), 0x1C, 2);
        break;

    case 37:
        __ct__800FC648(func_80043F18(&holder), 0, 1.0f);
        break;

    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
        __ct__800FC8DC(func_80043F18(&holder), 0, (f32)(s32)(op - 37) / 10.0f);
        break;

    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
        __ct__800FC790(func_80043F18(&holder), 0, (f32)(s32)(op - 46) / 10.0f);
        break;

    case 56:
        __ct__800FC8DC(func_80043F18(&holder), 0, 1.0f);
        break;

    case 57: {
        void* moveBase = (u8*)self->unkB14 + 0x3E9C;
        func_800F6ED0(func_80043F18(&holder), ((CAIVtObj*)moveBase)->v029());
        break;
    }

    case 59:
        func_800F9AEC(func_80043F18(&holder));
        __ct__800FC4FC(func_80043F18(&holder), 0, 0.2f);
        __ct__800FC19C(func_80043F18(&holder), 0x1C, 2);
        break;

    case 60:
        func_800F9AEC(func_80043F18(&holder));
        __ct__800FC19C(func_80043F18(&holder), 0x1C, 0);
        break;

    case 61: __ct__800FCF1C(func_80043F18(&holder), 1, 0); break;
    case 62: __ct__800FCF1C(func_80043F18(&holder), 2, 0); break;
    case 63: __ct__800FCF1C(func_80043F18(&holder), 3, 0); break;
    case 64: __ct__800FCF1C(func_80043F18(&holder), 4, 0); break;
    case 65: __ct__800FCF1C(func_80043F18(&holder), 5, 0); break;
    case 66: __ct__800FCF1C(func_80043F18(&holder), 6, 0); break;
    case 67: __ct__800FCF1C(func_80043F18(&holder), 7, 0); break;
    case 68: __ct__800FCF1C(func_80043F18(&holder), 8, 0); break;
    case 69: __ct__800FCF1C(func_80043F18(&holder), 9, 0); break;
    case 70: __ct__800FCF1C(func_80043F18(&holder), 10, 0); break;

    case 71:
        __ct__800FC19C(func_80043F18(&holder), 0x806, 0);
        break;

    case 72:
        __ct__800FC040(func_80043F18(&holder), 0xF, 0x9, 0);
        break;

    case 73:
        __ct__800FBF08(func_80043F18(&holder), 0x10, 0);
        break;

    case 74:
    case 88:
        __ct__800FBF08(func_80043F18(&holder), 0x132, 0);
        break;

    case 81:
        __ct__800FBF08(func_80043F18(&holder), 0x131, 0);
        break;

    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
        __ct__800FBF08(func_80043F18(&holder), op + 0xEE, 0);
        break;

    case 82:
    case 83:
    case 84:
    case 85:
    case 86:
    case 87:
        __ct__800FBF08(func_80043F18(&holder), op + 0xED, 0);
        break;

    case 89:
        __ct__800FBF08(func_80043F18(&holder), 0xCE, 0);
        break;

    case 90:
        __ct__800FC040(func_80043F18(&holder), 0xCF, 0xD0, 0);
        break;

    case 91:
        __ct__800FBF08(func_80043F18(&holder), 0xD1, 0);
        break;

    case 95:
        __ct__800FBF08(func_80043F18(&holder), 0x34, 0);
        break;

    case 96:
        __ct__800FBF08(func_80043F18(&holder), 0x36, 0);
        break;

    case 94:
        __ct__800FBF08(func_80043F18(&holder), 0xF8, 0);
        break;

    case 97:
        __ct__800FBF08(func_80043F18(&holder), 0x33, 0);
        break;

    case 98:
        __ct__800FBF08(func_80043F18(&holder), 0xD7, 0);
        break;

    case 99:
        __ct__800FBF08(func_80043F18(&holder), 0x58, 0);
        break;

    case 100:
        __ct__800FBF08(func_80043F18(&holder), 0x52, 0);
        break;

    case 101:
        __ct__800FBF08(func_80043F18(&holder), 0x59, 0);
        break;

    case 102:
        __ct__800FBF08(func_80043F18(&holder), 0x53, 0);
        break;

    case 103:
        __ct__800FBF08(func_80043F18(&holder), 0x5A, 0);
        break;

    case 104:
        __ct__800FBF08(func_80043F18(&holder), 0x54, 0);
        break;

    case 105:
        __ct__800FBF08(func_80043F18(&holder), 0x57, 0);
        break;

    case 106:
        __ct__800FBF08(func_80043F18(&holder), 0x44, 0);
        break;

    case 107:
        __ct__800FBF08(func_80043F18(&holder), 0x3C, 0);
        break;

    case 108:
        __ct__800FBF08(func_80043F18(&holder), 0x45, 0);
        break;

    case 109:
        __ct__800FBF08(func_80043F18(&holder), 0x3D, 0);
        break;

    case 110:
        __ct__800FBF08(func_80043F18(&holder), 0x9, 0);
        break;

    case 111:
        __ct__800FBF08(func_80043F18(&holder), 0xB, 0);
        break;

    case 112:
        __ct__800FBF08(func_80043F18(&holder), 0xC, 0);
        break;

    case 116:
        __ct__800FBF08(func_80043F18(&holder), 0x4, 0);
        break;

    case 117:
        __ct__800FBF08(func_80043F18(&holder), 0x2, 0);
        break;

    case 118:
        __ct__800FBF08(func_80043F18(&holder), 0x7, 0);
        break;

    case 119:
        __ct__800FBF08(func_80043F18(&holder), 0x5, 0);
        break;

    case 120:
        __ct__800FBF08(func_80043F18(&holder), 0x6, 0);
        break;

    case 121:
        __ct__800FBF08(func_80043F18(&holder), 0x8, 0);
        break;

    case 123:
        __ct__800FBF08(func_80043F18(&holder), 0x2A, 0);
        break;

    case 124:
        __ct__800FBF08(func_80043F18(&holder), 0x2B, 0);
        break;

    case 125:
        __ct__800FBF08(func_80043F18(&holder), 0x2C, 0);
        break;

    case 126:
        __ct__800FBF08(func_80043F18(&holder), 0x2D, 0);
        break;

    case 127:
        __ct__800FBF08(func_80043F18(&holder), 0x11, 0);
        break;

    case 128:
        __ct__800FBF08(func_80043F18(&holder), 0x12, 0);
        break;

    case 129:
        __ct__800FBF08(func_80043F18(&holder), 0x131, 2);
        break;

    case 130:
        __ct__800FBF08(func_80043F18(&holder), 0x132, 2);
        break;

    case 131:
        __ct__800FBF08(func_80043F18(&holder), 0x111, 0);
        break;

    case 132:
        __ct__800FBF08(func_80043F18(&holder), 0x112, 0);
        break;

    case 133:
        __ct__800FBF08(func_80043F18(&holder), 0x111, 2);
        break;

    case 134:
        __ct__800FBF08(func_80043F18(&holder), 0x112, 2);
        break;

    case 176:
        __ct__800FBF08(func_80043F18(&holder), 0x117, 2);
        break;

    case 177:
    case 178: {
        u32 i;
        for (i = 0; i < aiListCount(func_80043F18(&holder)); i++) {
            CAIEnumSlot* slot = (CAIEnumSlot*)func_800F6EC0(func_80043F18(&holder), i);
            void* obj = slot ? func_8016FE34(slot->unk04) : 0;
            if (!obj)
                slot->unk18 = slot->unk18 | 0x70;
            else if (func_80148778((u8*)obj + 8, 0x117))
                slot->unk18 = slot->unk18 | 0x70;
            else if (func_80148778(&((u8*)obj)[8], 0x6E))
                goto final177;
            else if (func_80148778((u8*)((unsigned long)obj + 8), 0x6F))
                goto final177;
            else if (func_80148778((u8*)((unsigned int)obj + 8), 0x70))
                goto final177;
            else if (func_80148778((u8*)obj + 0x8, 0x71))
                goto final177;
            else
                slot->unk18 = slot->unk18 | 0x70;
        final177:
            if (op == 178) {
                if (!(*(u32*)((u8*)obj + 0x3374) & 0x40000))
                    slot->unk18 = slot->unk18 | 0x70;
            } else if (*(u32*)((u8*)obj + 0x3374) & 0x40000) {
                slot->unk18 = slot->unk18 | 0x70;
            }
        }
        __ct__800FD250(func_80043F18(&holder));
        break;
    }

    case 135:
    case 136: {
        u32 i;
        for (i = 0; i < aiListCount(func_80043F18(&holder)); i++) {
            CAIEnumSlot* slot = (CAIEnumSlot*)func_800F6EC0(func_80043F18(&holder), i);
            void* obj = slot ? func_8016FE34(slot->unk04) : 0;
            if (!obj) {
                slot->unk18 = slot->unk18 | 0x70;
            } else if (op == 135) {
                if (*(u32*)((u8*)obj + 0x1530) == 0)
                    slot->unk18 = slot->unk18 | 0x70;
            } else if (op == 136) {
                if (*(u32*)((u8*)obj + 0x1530) != 0)
                    slot->unk18 = slot->unk18 | 0x70;
            }
        }
        __ct__800FD250(func_80043F18(&holder));
        break;
    }

    case 137:
    case 138:
    case 173:
    case 174:
    case 175: {
        u32 i;
        for (i = 0; i < aiListCount(func_80043F18(&holder)); i++) {
            CAIEnumSlot* slot = (CAIEnumSlot*)func_800F6EC0(func_80043F18(&holder), i);
            void* obj = slot ? func_8016FE34(slot->unk04) : 0;
            if (!obj || ((CAIVtObj*)obj)->v0AD() != 0) {
                slot->unk18 = slot->unk18 | 0x70;
                continue;
            }
            if (op == 174)
                goto L174;
            if (op == 137)
                goto L137;
            if (op == 173)
                goto L173;
            if (op == 138)
                goto L138;
            if (op == 175)
                goto L175;
            goto next137;
        L174:
            if (((CAIVtObj*)obj)->v0C0() <= 3)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L137:
            if (((CAIVtObj*)obj)->v0C0() <= 2)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L173:
            if (((CAIVtObj*)obj)->v0C0() != 2)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L138:
            if (((CAIVtObj*)obj)->v0C0() >= 2)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L175:
            if (((CAIVtObj*)obj)->v0C0() >= 1)
                slot->unk18 = slot->unk18 | 0x70;
        next137:
            ;
        }
        __ct__800FD250(func_80043F18(&holder));
        break;
    }

    case 139:
        __ct__800FCD10(func_80043F18(&holder), *(void**)((u8*)self->unkB14 + 0x3F10), 0);
        break;

    case 141:
        __ct__800FCA24(func_80043F18(&holder), *(void**)((u8*)self->unkB14 + 0x3F10), 0);
        break;

    case 142:
        __ct__800FCB80(func_80043F18(&holder), *(void**)((u8*)self->unkB14 + 0x3F10), 0);
        break;

    case 164:
    case 165:
    case 166:
    case 167:
    case 168:
    case 169:
    case 170:
    case 171: {
        if (func_800B8B94((s32)op - 163) != 0) {
            void* node = func_800B8B94((s32)(op - 163));
            __ct__800FCA24(func_80043F18(&holder), *(void**)((u8*)node + 0x3F10), 0);
        } else {
            aiListClear(func_80043F18(&holder));
        }
        break;
    }

    default:
        break;
    }

    // ---- .L_801541A4 : epilogue checks ---------------------------------
    if (sel >= 9 && sel <= 10) {
        if (aiListCount(func_80043F18(&holder)) != countBefore) {
            __dt__80043E88(&holder, -1);
            return 0;
        }
    }
    if (aiListCount(func_80043F18(&holder)) != 0)
        result = func_800F6E08(func_80043F18(&holder));
    __dt__80043E88(&holder, -1);
    return result;
}

// Base cf object as used here (func_8016FE34 returns obj = movePtr - 0x3E9C).
struct CfObjBase {
    void* vtable;          // 0x00
    u8    pad[0x1530 - 4];
    u32   unk1530;         // 0x1530 (battle-related state)
    u8    pad2[0x3374 - 0x1534];
    u32   unk3374;         // 0x3374 (flag 0x1000 = in-battle?)
    u8    pad3[0x3E9C - 0x3378];
    void* moveVtable;      // 0x3E9C embedded move object's vtable
    u8    pad4[0x64];
    u32   moveFlags;       // 0x3E9C+0x64 == 0x3F00 (bits 1,2 = move object)
    u32   unk3F10;         // 0x3F10 battle handle
};

extern "C" void* func_80150828(cf::CAIAction* self, CAIActionQuery* q) {
    CAIActionEnumHolder it;
    const u8* qb = (const u8*)q;
    u8 b5  = qb[5];   // 2nd switch key (0..178)
    u8 b6  = qb[6];   // 1st switch key
    u8 b9  = qb[9];
    u8 b10 = qb[10];
    u8 b11 = qb[11];
    u8 b12 = qb[12];
    u32 result = 0;
    u32 savedCount = 0;

    func_80043D90(&it);

    // ---- first switch: action class ---------------------------------------
    if (b6 == 2) {
        // +0x605C
        func_800F6D50(func_80043F18(&it), ((CfObjBase*)self->unkB14)->unk3F10);
    } else if (b6 == 7 || b6 == 9) {
        // +0x6084
        u32 filter = (((CfObjBase*)self->unkB14)->moveFlags & 0x4) ? 0x80000000 : 32;
        func_800F4A98(func_80043F18(&it), filter, 0);
    } else if (b6 == 8) {
        // +0x60BC : filter, then mark the entry whose object is self, commit.
        u32 filter = (((CfObjBase*)self->unkB14)->moveFlags & 0x4) ? 0x80000000 : 32;
        void* list = func_80043F18(&it);
        func_800F4A98(list, filter, 0);
        for (u32 i = 0; i < aiListCount(list); i++) {
            CAIEnumSlot* e = (CAIEnumSlot*)func_800F6EC0(list, (int)i);
            void* obj = e ? func_8016FE34(e->unk04) : 0;
            if (obj == 0) continue;
            if (obj != ((CfObjBase*)self->unkB14)) continue;
            e->unk18 = e->unk18 | 0x70;
            break;
        }
        __ct__800FD250(func_80043F18(&it));
    } else if (b6 >= 14 && b6 <= 21) {
        // +0x6178 : scan singleton node list (reslist at +0xB28) for tag b6-13
        func_800B07E8();                       // singleton acquire (retail discards result)
        void* node = func_800B76A4(func_800B07E8());          // &singleton->unkB28
        while (node != 0) {
            if (*(u16*)((u8*)node + 0x8C) == (u16)(b6 - 13)) {
                func_800F6D50(func_80043F18(&it), *(u32*)((u8*)node + 0x74));
                break;
            }
            func_800B07E8();
            node = func_800B77E4(func_800B07E8(), node);
        }
    } else if (b6 == 13) {
        // +0x61D0 : battle vision object's field +4
        void* vision = func_800EA444(getInstance__Q22cf14CBattleManagerFv());
        if (vision != 0 && *(u32*)((u8*)vision + 4) != 0) {
            func_800F6D50(func_80043F18(&it), *(u32*)((u8*)vision + 4));
        }
    } else {
        // +0x6200 default
        if (func_80148778((u8*)self->unkB14 + 8, 274)) {
            void* tag = func_80149154((u8*)self->unkB14 + 8, 274);
            func_800F6D50(func_80043F18(&it), *(u32*)((u8*)tag + 0x10));
        } else if (b6 <= 34) {
            // ---- sub-switch (jumptable_eu_8052F1AC, 35 entries 0..34) ----
            switch (b6) {
            case 0: case 2: case 3: case 7: case 8: case 9:
            case 13: case 14: case 15: case 16: case 17: case 18:
            case 19: case 20: case 21: case 22: case 23: case 24:
            case 25: case 26: case 27: case 28: case 29: case 30:
            case 31: case 32: case 33:
                break;  // -> +0x66F0 (no-op case)

            case 1:
            case 10: {
                // +0x6410
                if (func_80148778((u8*)self->unkB14 + 8, 17)) {
                    void* tag = func_80149154((u8*)self->unkB14 + 8, 17);
                    func_800F6D50(func_80043F18(&it), *(u32*)((u8*)tag + 0x10));
                    break;
                }
                if (self->unkB18 != 0) {
                    void* obj = func_8016FE34(func_800B708C((int)self->unkB18));
                    if (obj != 0 && ((CAIVtObj*)obj)->v0AD() == 0) {
                        func_800F6D50(func_80043F18(&it), self->unkB18);
                    } else {
                        self->unkB18 = 0;
                    }
                    break;
                }
                // +0x64A4
                {
                    u32 filter = (((CfObjBase*)self->unkB14)->moveFlags & 0x4) ? 32 : 0x80000000;
                    void* list = func_80043F18(&it);
                    func_800F4A98(list, filter, 0x800);
                    if (aiListCount(list) != 0) {
                        void* id = (void*)(uintptr_t)((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v011();
                        func_800F6D50(func_80043F18(&it), (u32)(uintptr_t)id);
                    }
                }
                break;
            }

            case 4:
                // +0x6510
                func_800F4A98(func_80043F18(&it), 16, 0);
                break;

            case 5: {
                // +0x6528 : secondary holder; filter; add move target of first
                //           entry's second word.
                CAIActionEnumHolder it2;
                func_80043D90(&it2);
                void* l2 = func_80043F18(&it2);
                func_800F4A98(l2, 16, 0);
                if (aiListCount(l2) != 0) {
                    void* base = func_800F6EAC(l2, 0);   // entries[0][1]
                    if (base) base = (u8*)base - 0x3E9C;
                    u32 id = (u32)(uintptr_t)((CAIVtObj*)((u8*)base + 0x3E9C))->v011();
                    func_800F6D50(func_80043F18(&it), id);
                }
                __dt__80043E88(&it2, -1);
                break;
            }

            case 6: {
                // +0x6260
                if (func_80148778((u8*)self->unkB14 + 8, 17)) {
                    void* tag = func_80149154((u8*)self->unkB14 + 8, 17);
                    func_800F6D50(func_80043F18(&it), *(u32*)((u8*)tag + 0x10));
                    break;
                }
                if (self->unkB18 != 0) {
                    void* obj = func_8016FE34(func_800B708C((int)self->unkB18));
                    if (obj != 0 && ((CAIVtObj*)obj)->v0AD() == 0) {
                        func_800F6D50(func_80043F18(&it), self->unkB18);
                    } else {
                        self->unkB18 = 0;
                    }
                    break;
                }
                // +0x62F4 battle-object dance
                {
                    u32 id = (u32)(uintptr_t)((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v011();
                    CfObjBase* obj = (CfObjBase*)func_8016FE34(func_800B708C((int)id));
                    if (obj == 0 || ((CAIVtObj*)obj)->v0AD() != 0) {
                        u32 filter = (((CfObjBase*)self->unkB14)->moveFlags & 0x4) ? 32 : 0x80000000;
                        func_800F4A98(func_80043F18(&it), filter, 0x800);
                        break;
                    }
                    if (((CfObjBase*)self->unkB14)->moveFlags & 0x2) {
                        if (obj->moveFlags & 0x2) break;
                        u32 id2 = (u32)(uintptr_t)((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v011();
                        func_800F6D50(func_80043F18(&it), id2);
                        break;
                    }
                    if (obj->moveFlags & 0x4) break;
                    u32 id3 = (u32)(uintptr_t)((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v011();
                    func_800F6D50(func_80043F18(&it), id3);
                }
                break;
            }

            case 11: {
                // +0x65A8 : filter + add move targets of every list-3 entry
                CAIActionEnumHolder it3;
                func_80043D90(&it3);
                void* l3 = func_80043F18(&it3);
                u32 filter = (((CfObjBase*)self->unkB14)->moveFlags & 0x4) ? 0x80000000 : 32;
                func_800F4A98(l3, filter, 0);
                u32 id0 = (u32)(uintptr_t)((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v011();
                func_800F6D50(func_80043F18(&it), id0);
                for (u32 i = 0; i < aiListCount(l3); i++) {
                    void* base = func_800F6EAC(l3, i);
                    if (base) base = (u8*)base - 0x3E9C;
                    if (base != ((CfObjBase*)self->unkB14)) {
                        u32 id = (u32)(uintptr_t)((CAIVtObj*)((u8*)base + 0x3E9C))->v011();
                        func_800F6D50(func_80043F18(&it), id);
                    }
                }
                __dt__80043E88(&it3, -1);
                break;
            }

            case 12: {
                // +0x6680
                void* vision = func_800EA444(getInstance__Q22cf14CBattleManagerFv());
                if (vision != 0 && *(u32*)((u8*)vision + 0) != 0) {
                    func_800F6D50(func_80043F18(&it), *(u32*)((u8*)vision + 0));
                }
                break;
            }

            case 34:
                // +0x66B0 : filter + queue "0x1000" action
                {
                    u32 filter = (((CfObjBase*)self->unkB14)->moveFlags & 0x4) ? 32 : 0x80000000;
                    void* list = func_80043F18(&it);
                    func_800F4A98(list, filter, 0x800);
                    __ct__800FC19C(func_80043F18(&it), 0x1000, 0);
                }
                break;
            }
        }
    }

    // ---- second switch setup (+0x66F0) ------------------------------------
    {
        void* list = func_80043F18(&it);
        if (aiListCount(list) == 0) {
            __dt__80043E88(&it, -1);
            return 0;
        }
        savedCount = aiListCount(list);   // retail keeps it in r22
    }

    if (b5 <= 178) {
        // ---- second switch (jumptable_eu_8052EEE0, 179 entries 0..178) ----
        switch (b5) {
        default:
            break;  // -> +0x7470 (no-op case; includes 0, 3..18, 58, 92, 93,
                    //    113..115, 122, 140, 143..162)

        case 1: {
            // +0x6744
            if (func_80148778((u8*)self->unkB14 + 8, 17)) break;
            if (self->unkB18 != 0) break;
            void* list = func_80043F18(&it);
            ((CAIEnumList*)list)->count = 0;
            ((CAIEnumList*)list)->unk3030 = 0;
            void* battleObj = ((CAIVtObj*)((CfObjBase*)self->unkB14))->v0B8();
            u32 id = 0;
            if (battleObj != 0) {
                id = (u32)(uintptr_t)((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v011();
                void* obj = func_8016FE34(func_800B708C((int)id));
                if (obj == 0 || ((CAIVtObj*)obj)->v0AD() != 0) {
                    if (((CfObjBase*)self->unkB14)->moveFlags & 0x2) {
                        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
                        id = (u32)(uintptr_t)((CAIVtObj*)player)->v011();
                    }
                }
            }
            func_800F6D50(func_80043F18(&it), id);
            break;
        }

        case 2: {
            // +0x6828 (same as case 1 but vtable slot 0x2EC)
            if (func_80148778((u8*)self->unkB14 + 8, 17)) break;
            if (self->unkB18 != 0) break;
            void* list = func_80043F18(&it);
            ((CAIEnumList*)list)->count = 0;
            ((CAIEnumList*)list)->unk3030 = 0;
            void* battleObj = ((CAIVtObj*)((CfObjBase*)self->unkB14))->v0B9();
            u32 id = 0;
            if (battleObj != 0) {
                id = (u32)(uintptr_t)((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v011();
                void* obj = func_8016FE34(func_800B708C((int)id));
                if (obj == 0 || ((CAIVtObj*)obj)->v0AD() != 0) {
                    if (((CfObjBase*)self->unkB14)->moveFlags & 0x2) {
                        void* player = getPlayer__Q22cf13CfGameManagerFi(0);
                        id = (u32)(uintptr_t)((CAIVtObj*)player)->v011();
                    }
                }
            }
            func_800F6D50(func_80043F18(&it), id);
            break;
        }

        case 19: case 20: case 21: case 22: case 23:
        case 24: case 25: case 26: case 27:
            // +0x690C
            __ct__800FC648(func_80043F18(&it), 0, (f32)(b5 - 18) / 10.0f);
            break;

        case 28: case 29: case 30: case 31: case 32:
        case 33: case 34: case 35: case 36:
            // +0x6940
            __ct__800FC4FC(func_80043F18(&it), 0, (f32)(b5 - 27) / 10.0f);
            __ct__800FC19C(func_80043F18(&it), 28, 2);
            break;

        case 37:
            // +0x6988
            __ct__800FC648(func_80043F18(&it), 0, 1.0f);
            break;

        case 38: case 39: case 40: case 41: case 42: case 43:
        case 44: case 45: case 46:
            // +0x69A0
            __ct__800FC8DC(func_80043F18(&it), 0, (f32)(b5 - 37) / 10.0f);
            break;

        case 47: case 48: case 49: case 50: case 51: case 52:
        case 53: case 54: case 55:
            // +0x69D4
            __ct__800FC790(func_80043F18(&it), 0, (f32)(b5 - 46) / 10.0f);
            break;

        case 56:
            // +0x6A08
            __ct__800FC8DC(func_80043F18(&it), 0, 1.0f);
            break;

        case 57: {
            // +0x6A20 : position-based action
            void* pos = ((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v029();
            func_800F6ED0(func_80043F18(&it), pos);
            break;
        }

        case 59:
            // +0x6A4C
            func_800F9AEC(func_80043F18(&it));
            __ct__800FC4FC(func_80043F18(&it), 0, 0.2f);
            __ct__800FC19C(func_80043F18(&it), 28, 2);
            break;

        case 60:
            // +0x6A84
            func_800F9AEC(func_80043F18(&it));
            __ct__800FC19C(func_80043F18(&it), 28, 0);
            break;

        case 61: __ct__800FCF1C(func_80043F18(&it), 1, 0); break;
        case 62: __ct__800FCF1C(func_80043F18(&it), 2, 0); break;
        case 63: __ct__800FCF1C(func_80043F18(&it), 3, 0); break;
        case 64: __ct__800FCF1C(func_80043F18(&it), 4, 0); break;
        case 65: __ct__800FCF1C(func_80043F18(&it), 5, 0); break;
        case 66: __ct__800FCF1C(func_80043F18(&it), 6, 0); break;
        case 67: __ct__800FCF1C(func_80043F18(&it), 7, 0); break;
        case 68: __ct__800FCF1C(func_80043F18(&it), 8, 0); break;
        case 69: __ct__800FCF1C(func_80043F18(&it), 9, 0); break;
        case 70: __ct__800FCF1C(func_80043F18(&it), 10, 0); break;

        case 71:
            // +0x6B98
            __ct__800FC19C(func_80043F18(&it), 2054, 0);
            break;

        case 72:
            // +0x6BB0
            __ct__800FC040(func_80043F18(&it), 15, 9, 0);
            break;

        case 73:  __ct__800FBF08(func_80043F18(&it), 16, 0);  break;
        case 74:  __ct__800FBF08(func_80043F18(&it), 306, 0); break;
        case 75: case 76: case 77: case 78: case 79: case 80:
            // +0x6C14
            __ct__800FBF08(func_80043F18(&it), b5 + 238, 0);
            break;
        case 81:  __ct__800FBF08(func_80043F18(&it), 305, 0); break;
        case 82: case 83: case 84: case 85: case 86: case 87:
            // +0x6C2C
            __ct__800FBF08(func_80043F18(&it), b5 + 237, 0);
            break;
        case 88:  __ct__800FBF08(func_80043F18(&it), 306, 0); break;
        case 89:  __ct__800FBF08(func_80043F18(&it), 206, 0); break;
        case 90:
            // +0x6C5C
            __ct__800FC040(func_80043F18(&it), 207, 208, 0);
            break;
        case 91:  __ct__800FBF08(func_80043F18(&it), 209, 0); break;
        case 94:  __ct__800FBF08(func_80043F18(&it), 248, 0); break;
        case 95:  __ct__800FBF08(func_80043F18(&it), 52, 0);  break;
        case 96:  __ct__800FBF08(func_80043F18(&it), 54, 0);  break;
        case 97:  __ct__800FBF08(func_80043F18(&it), 51, 0);  break;
        case 98:  __ct__800FBF08(func_80043F18(&it), 215, 0); break;
        case 99:  __ct__800FBF08(func_80043F18(&it), 88, 0);  break;
        case 100: __ct__800FBF08(func_80043F18(&it), 82, 0);  break;
        case 101: __ct__800FBF08(func_80043F18(&it), 89, 0);  break;
        case 102: __ct__800FBF08(func_80043F18(&it), 83, 0);  break;
        case 103: __ct__800FBF08(func_80043F18(&it), 90, 0);  break;
        case 104: __ct__800FBF08(func_80043F18(&it), 84, 0);  break;
        case 105: __ct__800FBF08(func_80043F18(&it), 87, 0);  break;
        case 106: __ct__800FBF08(func_80043F18(&it), 68, 0);  break;
        case 107: __ct__800FBF08(func_80043F18(&it), 60, 0);  break;
        case 108: __ct__800FBF08(func_80043F18(&it), 69, 0);  break;
        case 109: __ct__800FBF08(func_80043F18(&it), 61, 0);  break;
        case 110: __ct__800FBF08(func_80043F18(&it), 9, 0);   break;
        case 111: __ct__800FBF08(func_80043F18(&it), 11, 0);  break;
        case 112: __ct__800FBF08(func_80043F18(&it), 12, 0);  break;
        case 116: __ct__800FBF08(func_80043F18(&it), 4, 0);   break;
        case 117: __ct__800FBF08(func_80043F18(&it), 2, 0);   break;
        case 118: __ct__800FBF08(func_80043F18(&it), 7, 0);   break;
        case 119: __ct__800FBF08(func_80043F18(&it), 5, 0);   break;
        case 120: __ct__800FBF08(func_80043F18(&it), 6, 0);   break;
        case 121: __ct__800FBF08(func_80043F18(&it), 8, 0);   break;
        case 123: __ct__800FBF08(func_80043F18(&it), 42, 0);  break;
        case 124: __ct__800FBF08(func_80043F18(&it), 43, 0);  break;
        case 125: __ct__800FBF08(func_80043F18(&it), 44, 0);  break;
        case 126: __ct__800FBF08(func_80043F18(&it), 45, 0);  break;
        case 127: __ct__800FBF08(func_80043F18(&it), 17, 0);  break;
        case 128: __ct__800FBF08(func_80043F18(&it), 18, 0);  break;
        case 129: __ct__800FBF08(func_80043F18(&it), 305, 2); break;
        case 130: __ct__800FBF08(func_80043F18(&it), 306, 2); break;
        case 131: __ct__800FBF08(func_80043F18(&it), 273, 0); break;
        case 132: __ct__800FBF08(func_80043F18(&it), 274, 0); break;
        case 133: __ct__800FBF08(func_80043F18(&it), 273, 2); break;
        case 134: __ct__800FBF08(func_80043F18(&it), 274, 2); break;
        case 171: __ct__800FBF08(func_80043F18(&it), 235, 0); break;
        case 175: __ct__800FBF08(func_80043F18(&it), 279, 2); break;

        case 135: case 136: {
            // +0x7184 : mark entries by obj->unk1530
            void* list = func_80043F18(&it);
            for (u32 i = 0; i < aiListCount(list); i++) {
                CAIEnumSlot* e = (CAIEnumSlot*)func_800F6EC0(list, (int)i);
                CfObjBase* obj = e ? (CfObjBase*)func_8016FE34(e->unk04) : 0;
                if (obj == 0) {
                    if (e) e->unk18 = e->unk18 | 0x70;
                    continue;
                }
                if (b5 == 135) {
                    if (obj->unk1530 == 0) e->unk18 = e->unk18 | 0x70;
                } else {  // 136
                    if (obj->unk1530 != 0) e->unk18 = e->unk18 | 0x70;
                }
            }
            __ct__800FD250(func_80043F18(&it));
            break;
        }

        case 137: case 138: case 172: case 173: case 174: {
            // +0x723C : mark entries by obj state (vt[0x308]) and liveness
            void* list = func_80043F18(&it);
            for (u32 i = 0; i < aiListCount(list); i++) {
                CAIEnumSlot* e = (CAIEnumSlot*)func_800F6EC0(list, (int)i);
                CfObjBase* obj = e ? (CfObjBase*)func_8016FE34(e->unk04) : 0;
                if (obj == 0 || ((CAIVtObj*)obj)->v0AD() == 0) {
                    if (e) e->unk18 = e->unk18 | 0x70;
                    continue;
                }
                int state = (int)(uintptr_t)((CAIVtObj*)obj)->v0C0();
                if (b5 == 174) { if (state <= 3) e->unk18 = e->unk18 | 0x70; }
                else if (b5 == 137) { if (state <= 2) e->unk18 = e->unk18 | 0x70; }
                else if (b5 == 173) { if (state != 2) e->unk18 = e->unk18 | 0x70; }
                else if (b5 == 138) { if (state >= 2) e->unk18 = e->unk18 | 0x70; }
                else if (b5 == 175) { if (state >= 1) e->unk18 = e->unk18 | 0x70; }
            }
            __ct__800FD250(func_80043F18(&it));
            break;
        }

        case 139:
            // +0x73D4
            __ct__800FCD10(func_80043F18(&it), (void*)(uintptr_t)((CfObjBase*)self->unkB14)->unk3F10, 0);
            break;

        case 141:
            // +0x73F0
            __ct__800FCA24(func_80043F18(&it), (void*)(uintptr_t)((CfObjBase*)self->unkB14)->unk3F10, 0);
            break;

        case 142:
            // +0x740C
            __ct__800FCB80(func_80043F18(&it), (void*)(uintptr_t)((CfObjBase*)self->unkB14)->unk3F10, 0);
            break;

        case 163: case 164: case 165: case 166: case 167:
        case 168: case 169: case 170: {
            // +0x7428
            CfObjBase* obj = (CfObjBase*)func_800B8B94((int)b5 - 163);
            if (obj != 0) {
                __ct__800FCA24(func_80043F18(&it), (void*)(uintptr_t)obj->unk3F10, 0);
            } else {
                void* list = func_80043F18(&it);
                ((CAIEnumList*)list)->count = 0;
                ((CAIEnumList*)list)->unk3030 = 0;
            }
            break;
        }

        case 176: case 177: case 178: {
            // +0x7038 : mark entries by tags 279/110..113 and unk3374
            void* list = func_80043F18(&it);
            for (u32 i = 0; i < aiListCount(list); i++) {
                CAIEnumSlot* e = (CAIEnumSlot*)func_800F6EC0(list, (int)i);
                CfObjBase* obj = e ? (CfObjBase*)func_8016FE34(e->unk04) : 0;
                if (obj == 0) {
                    if (e) e->unk18 = e->unk18 | 0x70;
                    continue;
                }
                if (func_80148778((u8*)obj + 8, 279)) { e->unk18 = e->unk18 | 0x70; continue; }
                if (func_80148778((u8*)obj + 8, 110) &&
                    func_80148778((u8*)obj + 8, 111) &&
                    func_80148778((u8*)obj + 8, 112)) {
                    if (func_80148778((u8*)obj + 8, 113)) { e->unk18 = e->unk18 | 0x70; continue; }
                }
                if (b5 == 178) {
                    if (!(obj->unk3374 & 0x1000)) e->unk18 = e->unk18 | 0x70;
                } else {  // 176, 177
                    if (!(obj->unk3374 & 0x1000)) {
                        e->unk18 = e->unk18 | 0x70;
                    } else if (func_80148778((u8*)obj + 8, 16)) {
                        e->unk18 = e->unk18 | 0x70;
                    }
                }
            }
            __ct__800FD250(func_80043F18(&it));
            break;
        }
        }
    }

    // ---- common tail (+0x7470) --------------------------------------------
    {
        void* vision = func_800EA444(getInstance__Q22cf14CBattleManagerFv());
        if (vision != 0 && *(u32*)((u8*)vision + 0) != ((CfObjBase*)self->unkB14)->unk3F10) {
            // Retail packs the four q bytes into two u32 word-pairs in a
            // stack array {q9,q11} / {q10,q12} and iterates the pair twice:
            //   pair A: word = q[9]  (b9),  other/threshold = q[10] (b10)
            //   pair B: word = q[11] (b11), other/threshold = q[12] (b12)
            void* list = func_80043F18(&it);
            for (u32 pair = 0; pair < 2; pair++) {
                u32 word = (pair == 0) ? b9 : b11;
                u32 other = (pair == 0) ? b10 : b12;
                if (word == 0) continue;
                for (u32 i = 0; i < aiListCount(list); i++) {
                    CAIEnumSlot* e = (CAIEnumSlot*)func_800F6EC0(list, (int)i);
                    CfObjBase* obj = e ? (CfObjBase*)func_8016FE34(e->unk04) : 0;
                    if (obj == 0) continue;
                    if (word - 17 > 60) continue;
                    switch (word) {  // jumptable_eu_8052EDEC, words 17..77
                    case 17: case 18: case 19: {
                        // +0x7528
                        void* target = (u8*)obj + 0x3E9C;
                        f32 dist = func_801C37CC((u8*)((CfObjBase*)self->unkB14) + 0x3E9C, target);
                        if (dist <= (f32)other) {
                            int st = func_801C3850((u8*)((CfObjBase*)self->unkB14) + 0x3E9C, target);
                            if (word == 17) { if ((st & 0xFF) != 1) e->unk18 = e->unk18 | 0x70; }
                            else if (word == 18) { if ((st & 0xFF) != 4) e->unk18 = e->unk18 | 0x70; }
                            else { if ((st & 0xFF) != 2) e->unk18 = e->unk18 | 0x70; }
                        } else {
                            e->unk18 = e->unk18 | 0x70;
                        }
                        break;
                    }
                    case 20: {
                        // +0x7620
                        f32 dist = func_801C37CC((u8*)((CfObjBase*)self->unkB14) + 0x3E9C,
                                                 (u8*)obj + 0x3E9C);
                        if (dist < (f32)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 21: {
                        // +0x75D8
                        f32 dist = func_801C37CC((u8*)((CfObjBase*)self->unkB14) + 0x3E9C,
                                                 (u8*)obj + 0x3E9C);
                        if (dist >= (f32)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 22: {
                        // +0x7740 (args swapped: state of obj vs self)
                        int st = func_801C3850((u8*)obj + 0x3E9C,
                                               (u8*)((CfObjBase*)self->unkB14) + 0x3E9C);
                        if ((st & 0xFF) != 1) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 23: {
                        int st = func_801C3850((u8*)obj + 0x3E9C,
                                               (u8*)((CfObjBase*)self->unkB14) + 0x3E9C);
                        if ((st & 0xFF) != 4) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 24: {
                        int st = func_801C3850((u8*)obj + 0x3E9C,
                                               (u8*)((CfObjBase*)self->unkB14) + 0x3E9C);
                        if ((st & 0xFF) != 2) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 25: {
                        // +0x77DC : vt[0x328] returns struct; u16 flag bits
                        void* st = ((CAIVtObj*)obj)->v0C8();
                        if (*(u16*)st & 1) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 26: {
                        void* st = ((CAIVtObj*)obj)->v0C8();
                        if (*(u16*)st & 4) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 27: {
                        void* st = ((CAIVtObj*)obj)->v0C8();
                        if (*(u16*)st & 2) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 60: {
                        // +0x786C : vt[0x108] value difference
                        int mine = (int)(uintptr_t)((CAIVtObj*)((CfObjBase*)self->unkB14))->v040();
                        int theirs = (int)(uintptr_t)((CAIVtObj*)obj)->v040();
                        if (theirs - mine < (int)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 61: {
                        int mine = (int)(uintptr_t)((CAIVtObj*)((CfObjBase*)self->unkB14))->v040();
                        int theirs = (int)(uintptr_t)((CAIVtObj*)obj)->v040();
                        if (theirs - mine > (int)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 72: {
                        // +0x7904 : reversed difference
                        int mine = (int)(uintptr_t)((CAIVtObj*)((CfObjBase*)self->unkB14))->v040();
                        int theirs = (int)(uintptr_t)((CAIVtObj*)obj)->v040();
                        if (mine - theirs < (int)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 73: {
                        int mine = (int)(uintptr_t)((CAIVtObj*)((CfObjBase*)self->unkB14))->v040();
                        int theirs = (int)(uintptr_t)((CAIVtObj*)obj)->v040();
                        if (mine - theirs > (int)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 74: {
                        // +0x76D4 : |selfVert - objVert| >= threshold
                        // Retail: fabs(f1,f0); frsp f1,f1  -> fabsf(). Use
                        // ml::math::abs (NW4R FAbs) per MWCC_CASES
                        // (fabs+frsp pair), NOT libc fabsf.
                        void* selfPos = ((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v029();
                        void* objPos  = ((CAIVtObj*)(u8*)obj + 0x3E9C)->v029();
                        f32 dz = *(f32*)((u8*)selfPos + 4) - *(f32*)((u8*)objPos + 4);
                        if (mlAbs(dz) >= (f32)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 75: {
                        void* selfPos = ((CAIVtObj*)((u8*)((CfObjBase*)self->unkB14) + 0x3E9C))->v029();
                        void* objPos  = ((CAIVtObj*)(u8*)obj + 0x3E9C)->v029();
                        f32 dz = *(f32*)((u8*)selfPos + 4) - *(f32*)((u8*)objPos + 4);
                        if (mlAbs(dz) < (f32)other) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                    case 76:
                        // +0x799C
                        if (!(obj->unk3374 & 0x1000)) e->unk18 = e->unk18 | 0x70;
                        break;
                    case 77:
                        if (obj->unk3374 & 0x1000) e->unk18 = e->unk18 | 0x70;
                        break;
                    }
                }
            }
            __ct__800FD250(func_80043F18(&it));
        }
    }

    // ---- finalization (+0x7A08) -------------------------------------------
    if (b6 == 9 || b6 == 10) {
        void* list = func_80043F18(&it);
        if (savedCount != aiListCount(list)) {
            __dt__80043E88(&it, -1);
            return 0;
        }
    }
    {
        void* list = func_80043F18(&it);
        if (aiListCount(list) != 0) {
            result = (u32)(uintptr_t)func_800F6E08(func_80043F18(&it));
        }
    }
    __dt__80043E88(&it, -1);
    return (void*)result;
}



// ---------------------------------------------------------------------------
// func_801522C4 -- AI-action dispatcher (retail 0x80152D08, 0x151C bytes).
// Given a 0x20-byte action-query struct, builds a CfObjEnumList of event
// entries (switch on byte 8 = sel), filters it (switch on byte 7 = op),
// and returns the selected entry payload (func_800F6E08) or 0.
// ---------------------------------------------------------------------------

// Object view for indirect vtable calls (slots 0x00..0x328).

// CfObjEnumList fields accessed by this function.


// Move-subobject interface view used by func_801537F0: slot 0x4C returns
// the battle handle, slot 0x50 sets/clears it.
class CAIMoveSubVt {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v0A();
    virtual void v0B();
    virtual void v0C();
    virtual void v0D();
    virtual void v0E();
    virtual void v0F();
    virtual void v10();
    virtual void* v11();      // 0x4C
    virtual void v12(void*); // 0x50
};

// Party-object view extended past CAIVtObj (204 slots) to the 0x5C0 probe.
struct CAIPartyObjVt : CAIVtObj {
    virtual void v0CC(); virtual void v0CD(); virtual void v0CE(); virtual void v0CF();
    virtual void v0D0(); virtual void v0D1(); virtual void v0D2(); virtual void v0D3();
    virtual void v0D4(); virtual void v0D5(); virtual void v0D6(); virtual void v0D7();
    virtual void v0D8(); virtual void v0D9(); virtual void v0DA(); virtual void v0DB();
    virtual void v0DC(); virtual void v0DD(); virtual void v0DE(); virtual void v0DF();
    virtual void v0E0(); virtual void v0E1(); virtual void v0E2(); virtual void v0E3();
    virtual void v0E4(); virtual void v0E5(); virtual void v0E6(); virtual void v0E7();
    virtual void v0E8(); virtual void v0E9(); virtual void v0EA(); virtual void v0EB();
    virtual void v0EC(); virtual void v0ED(); virtual void v0EE(); virtual void v0EF();
    virtual void v0F0(); virtual void v0F1(); virtual void v0F2(); virtual void v0F3();
    virtual void v0F4(); virtual void v0F5(); virtual void v0F6(); virtual void v0F7();
    virtual void v0F8(); virtual void v0F9(); virtual void v0FA(); virtual void v0FB();
    virtual void v0FC(); virtual void v0FD(); virtual void v0FE(); virtual void v0FF();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v10A(); virtual void v10B();
    virtual void v10C(); virtual void v10D(); virtual void v10E(); virtual void v10F();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v11A(); virtual void v11B();
    virtual void v11C(); virtual void v11D(); virtual void v11E(); virtual void v11F();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v12A(); virtual void v12B();
    virtual void v12C(); virtual void v12D(); virtual void v12E(); virtual void v12F();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v13A(); virtual void v13B();
    virtual void v13C(); virtual void v13D(); virtual void v13E(); virtual void v13F();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v14A(); virtual void v14B();
    virtual void v14C(); virtual void v14D(); virtual void v14E(); virtual void v14F();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153();
    virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157();
    virtual void v158(); virtual void v159(); virtual void v15A(); virtual void v15B();
    virtual void v15C(); virtual void v15D(); virtual void v15E(); virtual void v15F();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163();
    virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v16A(); virtual void v16B();
    virtual void v16C(); virtual void v16D();
    virtual s32 vf5C0(void* arg); // #366 -> 0x5C0
};

// Resets the action ring (count/start/trailer), then queries the party's
// current state via three tagged probes (3 / 0x1c / 0x805); if none hits,
// queues action 0x31 and refreshes the battle-object handle.
void func_801537F0(cf::CAIAction* self) {
    // Reset the ring; retail computes unk8|4 between clearing unk214/unk210.
    self->unk8 = self->unk8 | 4;
    self->unk214 = 0;
    self->unk210 = 0;
    std::memset(self->trailer, 0, 0x20);

    // Three tagged probes on the party's state sub-object (vt slot 0x30).
    // Retail re-reads self->unkB14 before every probe and caches the party
    // pointer in a callee-saved reg only within each probe block.
    u32 tagA;
    u32 tagB;
    u32 tagC;
    {
        CAIPartyObj* pr1 = (CAIPartyObj*)self->unkB14;
        tagA = *pr1->unk04->vf30();
        if (func_80174C98(pr1, &tagA, 3) == 0) {
            CAIPartyObj* pr2 = (CAIPartyObj*)self->unkB14;
            tagB = *pr2->unk04->vf30();
            if (func_80174C98(pr2, &tagB, 0x1c) == 0) {
                CAIPartyObj* pr3 = (CAIPartyObj*)self->unkB14;
                tagC = *pr3->unk04->vf30();
                if (func_80174C98(pr3, &tagC, 0x805) == 0) {
                    func_800BE12C(
                        (u8*)(CAIPartyObj*)self->unkB14 + 0x3E9C, 0x31, 0, -1, 1);
                }
            }
        }
    }

    // Resolve the battle object from the move sub-object's vt slot 0x4C;
    // clear its action when it matches this party (or the party reports an
    // active battle via vt slot 0x5C0, probed with the resolved object).
    CAIPartyObj* cur = (CAIPartyObj*)self->unkB14;
    void* src = ((CAIMoveSubVt*)&cur->move)->v11();
    void* obj = func_8016FE34(func_800B708C((int)(intptr_t)src));
    cur = (CAIPartyObj*)self->unkB14;
    if (obj == (void*)cur || ((CAIPartyObjVt*)cur)->vf5C0(obj) != 0) {
        ((CAIMoveSubVt*)&((CAIPartyObj*)self->unkB14)->move)->v12(0);
    }
}
// Walks the AI config table (*lbl_eu_806641B0): finds the entry whose id
// matches (0x70/0x65 derived from the party flags when sel == 0, else sel
// itself) and whose arts id matches (the party's current arts id when
// sel == 0, else 0). The table's u16 fields are stored byte-swapped
// (BDAT/LE export), so they are rotated on load (__rlwimi reproduces the
// retail rlwinm+rlwimi swap pair). On a hit the action ring is cleared,
// the art name is recorded (strlen -> unkB10, strcpy -> unkB00), and the
// entry's 0xC-byte action sub-entries are installed via func_8014B804.
// sel doubles as the lookup id throughout: retail keeps it in one
// callee-saved register, re-derived to 0x70/0x65/0x63 as the walk proceeds.
void func_8015396C(cf::CAIAction* self, u32 sel) {
    cf::CAIActionTable* table = lbl_eu_806641B0;
    u32 want;
    u32 raw = table->count;
    u16 count = (u16)__rlwimi((raw << 8) & 0xFF00, raw, 24, 24, 31);
    const u8* cur = (const u8*)table + 2;

    if (sel == 0) {
        u32 flags = ((cf::CAIPartyInfo*)self->unkB14)->flags;
        if (flags & 0x2)
            sel = 0x70;
        else if (flags & 0x4)
            sel = 0x65;
        if (sel != 0) {
            // Declared index-first: MWCC hands the pointer the lower
            // callee-saved register this way (retail r25=ptr, r26=idx).
            u32 i = 0;
            u8* p = self->entries;
            while (i < 0xA0) {
                std::memset(p, 0, 0xE);
                p += 0xE;
                i++;
            }
            // Countdown form: MWCC converts the unused-counter loop to
            // mtctr/bdnz with an initial zero guard.
            u32 k = 0;
            while (k < count) {
                const cf::CAIActionTableEntry* e = (const cf::CAIActionTableEntry*)cur;
                if (e->id == sel &&
                    (u32)(u16)__rlwimi((e->artsId << 8) & 0xFF00, e->artsId, 24, 24, 31) ==
                        (u32)((cf::CAIPartyInfo*)self->unkB14)->artsId) {
                    char name[0x11];
                    std::memcpy(name, e->name, 0x10);
                    sel = 0;
                    name[0x10] = (u8)sel;
                    self->unkB10 = (u32)std::strlen(name);
                    std::strcpy((char*)&self->unkB00, name);
                    {
                        const cf::CAIActionSubEntry* s = e->actions;
                        for (s32 j = 0; j < e->actionCount; j++) {
                            func_8014B804((unsigned char*)self, j, s->b0, s->b2, s->b3,
                                          s->b5, s->b4, s->b6, s->b7, s->b8, s->b9,
                                          s->b10, s->b11, (int)sel);
                            s++;
                        }
                    }
                    return;
                }
                cur += 0x16 + e->actionCount * 0xC;
                i++;
            }
        }
        sel = 0x63;
        want = 0;
    }

    {
        // Derived via the entries member (same address as table+2, but a
        // distinct expression so MWCC keeps the base pointer live like retail).
        const cf::CAIActionTableEntry* p =
            (const cf::CAIActionTableEntry*)table->entries;
        u32 i = 0;
        while (i < count) {
            if (p->id == sel &&
                (u32)(u16)__rlwimi((p->artsId << 8) & 0xFF00, p->artsId, 24, 24, 31) ==
                    (u32)want) {
                char name[0x11];
                std::memcpy(name, p->name, 0x10);
                sel = 0;
                name[0x10] = (u8)sel;
                self->unkB10 = (u32)std::strlen(name);
                std::strcpy((char*)&self->unkB00, name);
                {
                    const cf::CAIActionSubEntry* s = p->actions;
                    for (s32 j = 0; j < p->actionCount; j++) {
                        func_8014B804((unsigned char*)self, j, s->b0, s->b2, s->b3,
                                      s->b5, s->b4, s->b6, s->b7, s->b8, s->b9,
                                      s->b10, s->b11, (int)sel);
                        s++;
                    }
                }
                return;
            }
            p = (const cf::CAIActionTableEntry*)((const u8*)p + 0x16 + p->actionCount * 0xC);
            i++;
        }
    }
}
