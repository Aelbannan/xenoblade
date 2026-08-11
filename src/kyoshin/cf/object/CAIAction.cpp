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
extern "C" void* func_80150618(cf::CAIAction* self, CAIActionQuery* in) {
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
                void* obj = func_8016FE34(func_800B708C((int)(uintptr_t)result));
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
                void* obj = func_8016FE34(func_800B708C((int)(uintptr_t)result));
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

// ---------------------------------------------------------------------------
// func_801522C4 — AI-action dispatcher (retail 0x80152D08, 0x151C bytes).
// Given a 0x20-byte action-query struct, builds a CfObjEnumList of event
// entries (switch on byte 8 = sel), filters it (switch on byte 7 = op),
// and returns the selected entry payload (func_800F6E08) or 0.
// ---------------------------------------------------------------------------

// Object view for indirect vtable calls (slots 0x00..0x328).
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
    virtual void* v036();
    virtual void* v037();
    virtual void* v038();
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
    virtual void* v0C0();
    virtual void* v0C1();
    virtual void* v0C2();
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

// CfObjEnumList fields accessed by this function.
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
// moveBase = unkB14 + 0x3E9C; retail pattern is lwzu on the base+0x3E9C,
// so the vtable call always targets (base + 0x3E9C) even when base is 0.
static inline void* aiMoveBaseVt4C(void* partyBase) {
    CAIVtObj* mb = (CAIVtObj*)((u8*)partyBase + 0x3E9C);
    return mb->v04C();
}

extern "C" void* func_801522C4(cf::CAIAction* self, const void* cmd) {
    CAIActionEnumHolder holder;
    const void* c = cmd;

    s32 op = (s32)((const u8*)c)[7];  // dispatch key A — the "command"
    s32 sel = (s32)((const u8*)c)[8]; // dispatch key B — the "selector"
    void* result = 0;
    u32 countBefore = 0;

    func_80043D90(&holder);

    switch (sel) {
    case 6: {
        if (func_80148778((u8*)self->unkB14 + 8, 0x11)) {
            void* o = func_80149154((u8*)self->unkB14 + 8, 0x11);
            func_800F6D50(func_80043F18(&holder), *(u32*)((u8*)o + 0x10));
        } else if (self->unkB18) {
            void* v = func_8016FE34(func_800B708C((int)self->unkB18));
            if (v && ((CAIVtObj*)v)->v2BC() == 0)
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
                        func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)((CAIVtObj*)moveBase)->v4C());
                } else if (!(*(u32*)((u8*)v + 0x3F00) & 0x4)) {
                    func_800F6D50(func_80043F18(&holder), (u32)(uintptr_t)((CAIVtObj*)moveBase)->v4C());
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
            if (v && ((CAIVtObj*)v)->v2BC() == 0)
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
        __ct__800FC19C(func_80043F18(&holder), 0x1000, 0);
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
                  ((CAIVtObj*)obj)->vE0() == (s32)(op - 151)))
                aiListClear(func_80043F18(&holder));
        }
        break;
    }

    case 1: {
        void* obj;
        if (!func_80148778((u8*)self->unkB14 + 8, 0x11)) {
            aiListClear(func_80043F18(&holder));
            obj = ((CAIVtObj*)(u8*)self->unkB14)->v2E8();
            if (obj == 0) {
                obj = aiMoveBaseVt4C((u8*)self->unkB14);
                void* v = func_8016FE34(func_800B708C((int)(uintptr_t)obj));
                if (v == 0)
                    goto fallback1;
                if (((CAIVtObj*)v)->v2BC() != 0)
                    goto fallback1;
                if (v != 0)
                    goto done1;
            fallback1:
                if (*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x2)
                    obj = ((CAIVtObj*)getPlayer__Q22cf13CfGameManagerFi(0))->v4C();
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
            obj = ((CAIVtObj*)(u8*)self->unkB14)->v2EC();
            if (obj == 0) {
                obj = aiMoveBaseVt4C((u8*)self->unkB14);
                void* v = func_8016FE34(func_800B708C((int)(uintptr_t)obj));
                if (v == 0)
                    goto fallback2;
                if (((CAIVtObj*)v)->v2BC() != 0)
                    goto fallback2;
                if (v != 0)
                    goto done2;
            fallback2:
                if (*(u32*)((u8*)self->unkB14 + 0x3F00) & 0x2)
                    obj = ((CAIVtObj*)getPlayer__Q22cf13CfGameManagerFi(0))->v4C();
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
        func_800F6ED0(func_80043F18(&holder), ((CAIVtObj*)moveBase)->vAC());
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
            if (!obj || ((CAIVtObj*)obj)->v2BC() != 0) {
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
            if (((CAIVtObj*)obj)->v308() <= 3)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L137:
            if (((CAIVtObj*)obj)->v308() <= 2)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L173:
            if (((CAIVtObj*)obj)->v308() != 2)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L138:
            if (((CAIVtObj*)obj)->v308() >= 2)
                slot->unk18 = slot->unk18 | 0x70;
            goto next137;
        L175:
            if (((CAIVtObj*)obj)->v308() >= 1)
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
