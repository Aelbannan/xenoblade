#include "kyoshin/cf/object/CAIAction.hpp"

#include <cstring>

// Batch 2026-07-14e: aiaction-ctor owns CAIAction::CAIAction()
// Batch 2026-07-14f: aiaction-vfunc1 owns CAIAction_UnkVirtualFunc1 (extern "C" Fv)

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

// symbols.txt: Fv; retail uses r3=this, r4=outA, r5=outB.
// Soft-cap ~93.2%: frameless leaf; MWCC CSE of early zeros and stwux vs retail
// stwx+add keep the last ~7% under high-level-only (see MWCC_REFERENCE).
extern "C" void CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv(cf::CAIAction* self,
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

        u8* dstBytes = (u8*)outB->buffer + ((u32)outIdx << 5);
        *(u32*)dstBytes = src->unk00;
        cf::CAIActionSlot* dst = (cf::CAIActionSlot*)dstBytes;

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

        outB->unk208 = outB->unk208 + 1;
    }
}

// Batch 2026-07-14g: aiaction-vfunc2 owns CAIAction_UnkVirtualFunc2 (extern "C" Fv)
// Inverse of UnkVirtualFunc1: imports trailer from inA, then drains ring
// entries from inB into this->unk20C.
extern "C" void CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv(cf::CAIAction* self,
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

// LLM-HARNESS-BEGIN: us-8014b2fc
void func_8014A8F8() {}
// LLM-HARNESS-END: us-8014b2fc
// LLM-HARNESS-BEGIN: us-8014b414
extern "C" bool func_8014AA10() { return false; }
// LLM-HARNESS-END: us-8014b414
// LLM-HARNESS-BEGIN: us-8014b63c
extern "C" void func_8014AC38() {}
// LLM-HARNESS-END: us-8014b63c
// LLM-HARNESS-BEGIN: us-8014b804
extern "C" void func_8014AE00() {}
// LLM-HARNESS-END: us-8014b804
// LLM-HARNESS-BEGIN: us-8014bb24
extern "C" void func_8014B120() {}
// LLM-HARNESS-END: us-8014bb24
// LLM-HARNESS-BEGIN: us-8014bce0
extern "C" void func_8014B2DC() {}
// LLM-HARNESS-END: us-8014bce0
// LLM-HARNESS-BEGIN: us-8014bcf0
extern "C" void func_8014B2EC() {}
// LLM-HARNESS-END: us-8014bcf0
// LLM-HARNESS-BEGIN: us-8014bd48
extern "C" void func_8014B344() {}
// LLM-HARNESS-END: us-8014bd48
// LLM-HARNESS-BEGIN: us-8014c208
extern "C" void func_8014B804() {}
// LLM-HARNESS-END: us-8014c208
// LLM-HARNESS-BEGIN: us-80154224
extern "C" void func_801537E0() {}
// LLM-HARNESS-END: us-80154224

// LLM-HARNESS-BEGIN: us-8014b270
void func_8014A86C(void*) {}
// LLM-HARNESS-END: us-8014b270
// LLM-HARNESS-BEGIN: us-8014c1b4
extern "C" void func_8014B7B0() {}
// LLM-HARNESS-END: us-8014c1b4
// LLM-HARNESS-BEGIN: us-8014c2c0
extern "C" void func_8014B8BC() {}
// LLM-HARNESS-END: us-8014c2c0
// LLM-HARNESS-BEGIN: us-8014d87c
extern "C" void func_8014CE78() {}
// LLM-HARNESS-END: us-8014d87c
// LLM-HARNESS-BEGIN: us-8014eb68
extern "C" void func_8014E164() {}
// LLM-HARNESS-END: us-8014eb68
// LLM-HARNESS-BEGIN: us-8015105c
extern "C" void func_80150618() {}
// LLM-HARNESS-END: us-8015105c
// LLM-HARNESS-BEGIN: us-8015126c
extern "C" void func_80150828() {}
// LLM-HARNESS-END: us-8015126c
// LLM-HARNESS-BEGIN: us-80152d08
extern "C" void func_801522C4() {}
// LLM-HARNESS-END: us-80152d08
// LLM-HARNESS-BEGIN: us-80154234
extern "C" void func_801537F0() {}
// LLM-HARNESS-END: us-80154234
// LLM-HARNESS-BEGIN: us-801543b0
extern "C" void func_8015396C() {}
// LLM-HARNESS-END: us-801543b0
