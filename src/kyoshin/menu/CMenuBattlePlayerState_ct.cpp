// menu-bps-ctor: __ct__CMenuBattlePlayerState only.
// Own TU with -O4,s so the inlined 0x60/0x0C pad fill keeps li/divwu/mtctr
// without breaking Term/cbRenderBefore on the main -O4,p object.
//
// Retail shape (sibling CMenuBattleCommu factory idiom): unmangled extern "C"
// free function __ct__CMenuBattlePlayerState(void*, CScn*), NOT a C++ ctor.
// A C++ ctor here mangles to __ct__22CMenuBattlePlayerStateFP4CScn AND makes
// MWCC emit implicit base/member init calls (extra CProcess + 2x UnkClass
// bls). Manual CProcess-style stores with raw label vtables.
#include "kyoshin/menu/CMenuBattlePlayerState.hpp"

#include "monolib/work/CProcess.hpp"

#include "decomp.h"

struct CMenuBpsProcessShim {
    u8 unk00[0x10];
    void* vtable;
    u8 unk14[0x28];
    u32 callbacks[6]; // +0x3c
};

extern "C" {
char lbl_eu_8052C1C0[];
char lbl_eu_8052C330[];
char lbl_eu_8052C42C[];
// Retail storage is the PowerPC_EABI_Support runtime's ptmf.o global;
// this TU must not emit its own .bss copy.
extern u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
void BpsStateClearSlot(CMenuBattlePlayerStateSlot*);
void __dt__8010B444(CMenuBattlePlayerStateSlot*, s16);
void __construct_array(void* ptr, void* ctor, void* dtor, u32 size, u32 n);
extern const f32 lbl_eu_80666F94; // 0.0f
extern const f32 lbl_eu_80666FB0; // -1.0f
}

// Unmangled retail symbol: __ct__CMenuBattlePlayerState (not a C++ ctor).
// Prelude mirrors the FULL_MATCH __ct__CMenuBattleCommu factory: temp vtable,
// post-increment ptmf triple loads, interleaved dispatch slots, UnkClass ctor.
extern "C" void* __ct__CMenuBattlePlayerState(CMenuBattlePlayerState* self, CScn* scn) {
    // MWCC colors NV regs in declaration order (first -> highest): z takes
    // r31 (whole-function zero), then loop-carried values, leaving `self`
    // at the retail r27 home.
    u32 z;
    u32 qpSign;
    u32 nCopy;
    u8 i;
    CMenuBattlePlayerState* me;
    u8* padStart;
    CMenuBpsProcessShim* process;
    char* vtWork;
    char* vtRender;
    u32 p0, p1, p2;
    u32 q0, q1, q2;
    u32 v4;
    u32 v6;
    u32 vB;
    u8* padEnd;
    u8* padLim;
    s32 padSize;
    s32 sizePlus;
    s32 q12;
    s32 q12p1;

    me = reinterpret_cast<CMenuBattlePlayerState*>(self);
    process = reinterpret_cast<CMenuBpsProcessShim*>(me);
    __ct__8CProcessFv(reinterpret_cast<CProcess*>(me));
    // Single ptmf cursor (commu idiom): one base register + lwzu head.
    const u32* ptmf = __ptmf_null;

    // Interim CProcess vtable, then final MI vtable + interface pieces.
    // Retail order: lwzu p0, vtWork add, z li, lwz p1, cb1 store, vtRender
    // add, cb0 store, lwz p2, cb2 store, reload q0/q1, cb4/cb3 stores, q2,
    // cb5 store, state bytes, then final vtable + dispatch slots + scene.
    process->vtable = lbl_eu_8052C1C0;
    vtWork = lbl_eu_8052C330 + 0x24;
    z = 0;
    p0 = *ptmf++;
    p1 = *ptmf++;
    process->callbacks[1] = p1;
    vtRender = lbl_eu_8052C330 + 0xac;
    process->callbacks[0] = p0;
    p2 = *ptmf++;
    process->callbacks[2] = p2;
    q0 = ptmf[-3];
    q1 = ptmf[-2];
    process->callbacks[4] = q1;
    process->callbacks[3] = q0;
    q2 = ptmf[-1];
    process->callbacks[5] = q2;
    me->mField54 = (u8)z;
    me->mField55 = (u8)z;
    process->vtable = lbl_eu_8052C330;
    *reinterpret_cast<char**>(reinterpret_cast<u8*>(self) + 0x58) = vtWork;
    *reinterpret_cast<char**>(reinterpret_cast<u8*>(self) + 0x5c) = vtRender;
    me->mScn = scn;

    __ct__17UnkClass_8045F564Fv(&me->unk64);
    __construct_array(me->mSlots, reinterpret_cast<void*>(BpsStateClearSlot),
                      reinterpret_cast<void*>(__dt__8010B444), 0x270, 3);

    // Retail stores 0.0f then constructs unk7D0 before loading loop floats.
    me->unk7C4 = lbl_eu_80666F94;
    me->unk7C8 = (u8)z;
    me->unk7C9 = (u8)z;
    me->unk7CC = lbl_eu_8052C42C;
    __ct__17UnkClass_8045F564Fv(&me->unk7D0);

    {
        f32 zeroF;
        f32 neg1F;
        CMenuBattlePlayerStateSlot slot;
        u32 pSign;
        u32 spSign;
        u32 qSign;
        u32 ok;
        u32 ok2;
        u32 ok3;

        // Retail guards use sign-bit masks (rlwinm ..,0,0,0 keeps MSB).
        // Freeze statement order: the split div statements are dataflow-
        // independent, and even -O4,s hoists/reorders them (padLim/counts
        // sink or hoist). Scheduling-off keeps retail's interleaved order.
        padStart = slot.pad90;
        padEnd = reinterpret_cast<u8*>(&slot.unk204);
        // Plain /12 (divw under -O4,s). The retail mulhw/bit30-fixup delta
        // is the known residual (see stall note); hand-splitting it invites
        // scheduler hoists that cost more than the div itself.
        padSize = static_cast<s32>(padEnd - padStart);
        sizePlus = padSize + 0xb;
        me->unk7F4 = 1;
        q12 = sizePlus / 12;
        pSign = (u32)padSize & 0x80000000u;
        me->unk7E0 = (void*)z;
        padLim = padEnd - 0x60;
        zeroF = lbl_eu_80666F94;
        spSign = (u32)sizePlus & 0x80000000u;
        me->unk7E4 = (nw4r::lyt::Layout*)z;
        neg1F = lbl_eu_80666FB0;
        me->unk7E8 = (nw4r::lyt::AnimTransform*)z;
        q12p1 = q12 + 1;
        i = 0;
        qSign = (u32)q12 & 0x80000000u;
        me->unk7EC = (nw4r::lyt::AnimTransform*)z;
        qpSign = (u32)q12p1 & 0x80000000u;
        v4 = 4;
        me->unk7F0 = (nw4r::lyt::AnimTransform*)z;
        v6 = 6;
        vB = 0xb;
        nCopy = 0x30;
        me->unk7F8 = z;
        // Literal strides: under -O4,s MWCC keeps divwu/mtctr (only -O4,p
        // would strength-reduce to mulhwu).
        do {
            // Inlined BpsStateClearSlot: clear +0x74..+0x8c then pad90[0x174].
            slot.unk74 = (void*)z;
            slot.unk78 = (nw4r::lyt::Layout*)z;
            slot.unk7C = (nw4r::lyt::AnimTransform*)z;
            slot.unk80 = (u8)z;
            slot.unk84 = (void*)z;
            slot.unk88 = (void*)z;
            slot.unk8C = (void*)z;
            {
                u8* bp = padStart;
                u8* be = padEnd;
                u8* lim = padLim;
                u32 c;

                if (bp < be) {
                    if (q12p1 > 8) {
                        // Retail dual bool gate (padSize/sizePlus signs, then
                        // q12/q12p1), with sign-bit-masked checks.
                        ok = 0;
                        ok2 = 0;
                        if (!(padStart > be)) {
                            ok3 = 1;
                            if (pSign == 0 && spSign != 0) {
                                ok3 = 0;
                            }
                            if (ok3 != 0) {
                                ok2 = 1;
                            }
                        }
                        if (ok2 != 0) {
                            ok3 = 1;
                            if (qSign == 0 && qSign != qpSign) {
                                ok3 = 0;
                            }
                            if (ok3 != 0) {
                                ok = 1;
                            }
                        }
                        if (ok != 0) {
                            if (bp < lim) {
                                // Count from padStart (not bp) like leaf/retail.
                                // Computed inside the taken branch so the
                                // long-latency divwu is not speculated above
                                // the gate.
                                c = (u32)(lim + 0x5f - padStart) / 0x60;
                                do {
                                    u32* w = reinterpret_cast<u32*>(bp);
                                    w[0] = z;
                                    w[1] = z;
                                    w[2] = z;
                                    w[3] = z;
                                    w[4] = z;
                                    w[5] = z;
                                    w[6] = z;
                                    w[7] = z;
                                    w[8] = z;
                                    w[9] = z;
                                    w[10] = z;
                                    w[11] = z;
                                    w[12] = z;
                                    w[13] = z;
                                    w[14] = z;
                                    w[15] = z;
                                    w[16] = z;
                                    w[17] = z;
                                    w[18] = z;
                                    w[19] = z;
                                    w[20] = z;
                                    w[21] = z;
                                    w[22] = z;
                                    w[23] = z;
                                    bp += 0x60;
                                } while (--c);
                            }
                        }
                    }
                    if (bp < be) {
                        c = (u32)(be + 0xb - bp) / 0x0C;
                        do {
                            u32* w = reinterpret_cast<u32*>(bp);
                            w[0] = z;
                            w[1] = z;
                            w[2] = z;
                            bp += 0xc;
                        } while (--c);
                    }
                }
            }

            // Retail: clrlwi i, then stfs/const stores, then unrolled header zeros.
            slot.unk220 = zeroF;
            slot.unk224 = zeroF;
            slot.unk228 = zeroF;
            slot.unk22C = neg1F;
            slot.unk248 = v4;
            slot.unk250 = v6;
            slot.unk254 = vB;
            slot.unk258 = i;
            slot.unk264 = zeroF;

            {
                u32* hp = reinterpret_cast<u32*>(&slot);
                hp[0] = z;
                hp[1] = z;
                hp[2] = z;
                hp[3] = z;
                hp[4] = z;
                hp[5] = z;
                hp[6] = z;
                hp[7] = z;
                hp[8] = z;
                hp[9] = z;
                hp[10] = z;
                hp[11] = z;
                hp[12] = z;
                hp[13] = z;
                hp[14] = z;
                hp[15] = z;
                hp[16] = z;
            }
            slot.unk204 = (u8)z;
            slot.unk208 = z;
            slot.unk20C = z;
            slot.unk210 = z;
            slot.unk214 = z;
            slot.unk218 = z;
            slot.unk21C = z;
            slot.unk230 = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x234) = z;
            slot.unk238 = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x23c) = z;
            slot.unk25C = z;
            slot.unk240 = (u8)z;
            slot.unk244 = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x24c) = z;
            *reinterpret_cast<u8*>(reinterpret_cast<u8*>(&slot) + 0x260) = (u8)z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x268) = z;
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x26c) = z;

            // Retail copy: dest first word = 0, head words from slot+4,
            // lbz unk80, mtctr/lwzu/stwu x0x30 from +0x84, then tail.
            {
                CMenuBattlePlayerStateSlot* dst = &me->mSlots[i];
                u32* s;
                u32* d;
                u32* sp;
                u32* dp;

                sp = reinterpret_cast<u32*>(&slot);
                dp = reinterpret_cast<u32*>(dst);
                dp[0] = z;
                dp[1] = sp[1];
                dp[2] = sp[2];
                dp[3] = sp[3];
                dp[4] = sp[4];
                dp[5] = sp[5];
                dp[6] = sp[6];
                dp[7] = sp[7];
                dp[8] = sp[8];
                dp[9] = sp[9];
                dp[10] = sp[10];
                dp[11] = sp[11];
                dp[12] = sp[12];
                dp[13] = sp[13];
                dp[14] = sp[14];
                dp[15] = sp[15];
                dp[16] = sp[16];
                dp[17] = sp[17];
                dp[18] = sp[18];
                dp[19] = sp[19];
                dp[20] = sp[20];
                dp[21] = sp[21];
                dp[22] = sp[22];
                dp[23] = sp[23];
                dp[24] = sp[24];
                dp[25] = sp[25];
                dp[26] = sp[26];
                dp[27] = sp[27];
                dp[28] = sp[28];
                dp[29] = sp[29];
                dp[30] = sp[30];
                dp[31] = sp[31]; // through +0x7c
                dst->unk80 = slot.unk80;

                // First pair is +0x84/+0x88; pointers biased like CBattleState.
                u32* slotWords = reinterpret_cast<u32*>(&slot.unk84);
                u32* dstWords = reinterpret_cast<u32*>(&dst->unk84);
                s = slotWords - 1;
                d = dstWords - 1;
                do {
                    u32 a = *(s + 1);
                    u32 b = *(s += 2);
                    *(d + 1) = a;
                    *(d += 2) = b;
                } while (--nCopy);

                dst->unk204 = slot.unk204;
                dst->unk208 = slot.unk208;
                dst->unk20C = slot.unk20C;
                dst->unk210 = slot.unk210;
                dst->unk214 = slot.unk214;
                dst->unk218 = slot.unk218;
                dst->unk21C = slot.unk21C;
                dst->unk220 = slot.unk220;
                dst->unk224 = slot.unk224;
                dst->unk228 = slot.unk228;
                dst->unk22C = slot.unk22C;
                dst->unk230 = slot.unk230;
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(dst) + 0x234) =
                    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x234);
                dst->unk238 = slot.unk238;
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(dst) + 0x23c) =
                    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x23c);
                dst->unk240 = slot.unk240;
                dst->unk244 = slot.unk244;
                dst->unk248 = slot.unk248;
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(dst) + 0x24c) =
                    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x24c);
                dst->unk250 = slot.unk250;
                dst->unk254 = slot.unk254;
                dst->unk258 = slot.unk258;
                dst->unk25C = slot.unk25C;
                *reinterpret_cast<u8*>(reinterpret_cast<u8*>(dst) + 0x260) =
                    *reinterpret_cast<u8*>(reinterpret_cast<u8*>(&slot) + 0x260);
                dst->unk264 = slot.unk264;
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(dst) + 0x268) =
                    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x268);
                *reinterpret_cast<u32*>(reinterpret_cast<u8*>(dst) + 0x26c) =
                    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&slot) + 0x26c);
            }
            i = (u8)(i + 1);
        } while (i < 3);
    }

    return me;
}
