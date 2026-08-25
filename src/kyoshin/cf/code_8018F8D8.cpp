// Auto-scaffolded catalog TU for kyoshin/cf/code_8018F8D8
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

// Concurrent-edit shield: CfCam.hpp now carries tentative definitions of
// lbl_eu_8066A1FC while several headers declare it extern "C"; rename the
// identifier out of the way per include group (this TU never uses it).
#define lbl_eu_8066A1FC cf_ptstate_2pi_a

// func_801F941C's shared header declares it with (self, u32, u32), but the
// CMenuPTState::Move call site leaves r4/r5 unset in retail; shadow the
// 3-arg declaration during include and re-declare the 1-arg form below.
#define func_801F941C func_801F941C_3arg_unused
#include "kyoshin/cf/code_8018F8D8.hpp"
#undef func_801F941C

#include "kyoshin/CTaskGame.hpp"
// NOTE: CfObjectActor.hpp is deliberately NOT included here:
// CAIAction.hpp (its line-5 include) still carries a stale 2-arg declaration
// of getBdatStringColumnValue that collides with the canonical 3-arg decl in
// plugin/ocBdat.hpp (MWCC 10197 "illegal function overloading"). Instead this
// TU includes CfObjectMove.hpp directly for the func_800BE12C owner decl and
// the other CfObject* declarations CfObjectActor would have provided.
#include "kyoshin/cf/object/CfObjectMove.hpp"  // func_800BE12C (owner decl)
// CfGameManager.hpp (via code_8018F8D8.hpp) internally mixes a void* and a
// CBattleManagerView* declaration of this getter; this TU calls none of them,
// so rename it out of the way for every include below.
// (func_80174B4C / func_80174C98 now have single unified decls on the
// CfMapItemManager owner header; no pre-include renames needed for them.)
// NOTE: CBattleManager.hpp temporarily dropped - its include subtree currently
// mixes a CfCam.hpp tentative definition of lbl_eu_8066A1FC with extern "C"
// declarations of the same symbol (concurrent-edit conflict).
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "monolib/math/CVec3.hpp"
#undef lbl_eu_8066A1FC

// Imports used only by func_80190940.
extern float lbl_eu_80667A94;
extern u32 lbl_eu_80532A28[3];   // 12-byte member-function-pointer pool entry


// C-linkage (unmangled) helpers referenced by the catalog functions below.
// These retail symbols are defined in other TUs / the linked library.
extern "C" {
    // Defined later in this TU (state-blob copy used by Init).
    void func_80192268(SCopy_80192268* dst, const SCopy_80192268* src);
    extern u32 lbl_eu_80663E28;
    extern u32 lbl_eu_80664300;
    extern float lbl_eu_80667A70;
    extern float lbl_eu_80667A74;
    extern u16 lbl_eu_80663E3C;
    extern u8  lbl_eu_8066476D;
    int*  func_8009ECB0();
    void* func_8023C1B4();
    void func_800628C4(u32 a, u32 b);
    void func_8008064C__Q22cf13CfGameManagerFv(void* element0, int idx, float* stk);
    void func_8007FE18__Q22cf13CfGameManagerFv(int flag);
    bool func_8006EF04__Fi(int mask);
    u32  func_8009CF8C(u32 resource);
    int  func_80148778(void* obj, int id);
    int  func_8027E018(volatile s16* obj, FuncActorRef* arg);
    void __dt__80043E88(void* holder, int flag);
    int func_80061A80(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    int func_80061870(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    void func_80068DAC();
    void func_800B98C8(int);
    void func_8012F860();
    void func_801338C8();
    void func_80133AE8();
    s32 func_8011C2E8();
    void func_80080F40__Q22cf13CfGameManagerFv(void* this_, u32 second, u32 third);

    void func_801AAC78(int arg);
    int  func_800B8FC4();
    int  func_8023C1C0();
    int  func_8012FA5C();
    void func_8012FAA8();
    int  func_80062A84(s32 arg);
    int  func_802A3748(u32 value);
    void func_80135550();
    int  func_804962A0(CScn* scn, int flag);
    void func_8009EB2C(int a, int b, u8* c);
    int  func_eu_80062E58(u16 a, u16 b, int c);
    bool func_80061D2C(UnkClass_80085334* obj, u32 mode);
    void func_801C3D9C(u8* obj);
    void func_801FA254(u8* obj);
    void func_8008566C__Q22cf13CfGameManagerFv(u32 mode, const UnkFloat4* value, u32 third);
    void func_80085878__Q22cf13CfGameManagerFv();
    int  func_80085840__Q22cf13CfGameManagerFv();
    int  func_80085838__Q22cf13CfGameManagerFv();
    void func_8007FECC__Q22cf13CfGameManagerFv();
    void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
    void func_8007FE2C__Q22cf13CfGameManagerFv();
    void func_8007E0D0__Q22cf13CfGameManagerFv(bool alternate);

    long func_8017FD44();
    // func_8004C5EC: canonical u32(void*) decl comes from CfObjectMove.hpp.
    int func_800EA444();
    long __ptmf_cmpr(u32* slot, u32* pmf1, u32* pmf2);
    void func_80043E08(CfEnumListHolder90940* holder, int cap, int mode);
    // func_80043F18: canonical void*(void*) decl comes from CPartyStateWin.hpp
    // (included via code_8018F8D8.hpp); call sites cast to CfEnumList90940*.
    u8* func_800F6EAC(CfEnumList90940* list, int idx);
}

namespace cf {}
using namespace cf;

// 1-arg view of func_801F941C for Move's call site (r4/r5 unset in retail).
extern "C" void func_801F941C(CPartyStateWin* self);

int func_8018F8D8(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (lbl_eu_80663E24 & 0x8000) {
        func_80061A80(p0, 0x23, p1, p2, p3, p4);
    }
    return 1;
}

int func_8018F924(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (func_800B8FC4() != 0) {
        lbl_eu_80663E28 &= ~0x20;
        func_8007E0D0__Q22cf13CfGameManagerFv(true);
    } else {
        func_80061A80(p0, 0x24, p1, p2, p3, p4);
    }
    return 1;
}

int func_8018F9A0(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    // Clear the event-pending flag when an event is already active/disabled
    // (busy manager, bit9 latching, or bit7 set); otherwise forward the request.
    if (func_80085840__Q22cf13CfGameManagerFv() != 0 ||
        (lbl_eu_80663E24 & 0x400000) ||
        (lbl_eu_80663E28 & 0x1000000)) {
        lbl_eu_80663E24 &= ~0x2000;
    } else {
        func_80061A80(p0, 0x25, p1, p2, p3, p4);
    }
    return 1;
}

// View of the manager object behind CFuncHost::manager for func_8018FA2C:
// only the +0xC data region and the +0x24 float are read.
struct FuncHostMgrView {
    u8 _0[0xc];
    u8 field_0xC[0x18];
    float field_0x24;
};

// Party-rebuild event: reposition/notify all players from the shared offset
// table, refresh the nine party slots, then raise the rebuild task events.
int func_8018FA2C(CFuncHost* self, u32 p1, u32 p2, u32 p3, u32 p4) {
    // Selector byte defaults to 0 and is only loaded when bit20 is set.
    u32 selectId = 0;
    if (lbl_eu_80663E24 & 0x100000) {
        selectId = lbl_eu_8066476D;
    }
    FuncHostMgrView* mgr = reinterpret_cast<FuncHostMgrView*>(self->manager);
    cf::CfObjectMove* found = func_8007FF6C__Q22cf13CfGameManagerFv(
        lbl_eu_80663E40, mgr->field_0xC, selectId, mgr->field_0x24);
    if (lbl_eu_80663E24 & 0x100000) {
        // Fixed setup: force players into/out of "command" mode (bit 4 at
        // +0x68), then move each by an offset vector before the notify calls.
        float* base = reinterpret_cast<float*>(func_8023C1B4());
        u8 tag = lbl_eu_8066476D;
        float ox = lbl_eu_80667A70;
        float oy = lbl_eu_80667A74;
        // Scratch buffers: component-wise sum of the fixed offset vector
        // {ox, oy, ox} and the per-player base entry (MWCC vectorizes the
        // summation with paired singles). Declared pos-first to match
        // retail's stack slot assignment (pos@8, offset@0x14, sum@0x20).
        float pos[3];
        float offset[3];
        float sum[3];
        const float* pOff = offset;
        float* pSum = sum;
        float* pPos = pos;
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player != 0) {
                CPlayerFlags* pf = reinterpret_cast<CPlayerFlags*>(player);
                if (tag != 0) pf->flags |= 0x10;
                else pf->flags &= ~0x10;
                float* src = &base[i * 4];
                offset[0] = ox;
                offset[1] = oy;
                offset[2] = ox;
                for (int j = 0; j < 3; j++) {
                    pSum[j] = pOff[j] + src[j];
                }
                pPos[0] = pSum[0];
                pPos[1] = pSum[1];
                pPos[2] = pSum[2];
                (*reinterpret_cast<CPlayerVtbl**>(player))->fw_a8(player, pPos);
                (*reinterpret_cast<CPlayerVtbl**>(player))
                    ->fw_d4(player, src, src[3]);
            }
        }
    }

    // Refresh every occupied party slot: resolve its character data, reset
    // the two display sub-object modes (+0x17C slots 0xA4/0xA8) and sync.
    PartySlotList* slotList = reinterpret_cast<PartySlotList*>(func_8009ECB0());
    for (int i = 0; i < 9; i++) {
        u32 entry = slotList->slots[i];
        if (entry == 0) continue;
        void* data = func_8009EC9C((u16)entry);
        func_800A30E4(data);
        CCharDataView* cd = reinterpret_cast<CCharDataView*>(data);
        cd->vtbl17c->fw_a4(cd->vtbl17c, 0);
        cd->vtbl17c->fw_a8(cd->vtbl17c, 1);
        func_800A1370(cd);
    }

    if (found != 0) {
        func_80061A80((u32)self, 0xe, 1, 1, 0, 0);
        func_80081D8C__Q22cf13CfGameManagerFv(reinterpret_cast<u32>(found));
    }
    if (lbl_eu_80663E24 & 0x80000) {
        // Mark all players as needing a full status refresh (bit 28 at +0x68).
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player == 0) continue;
            CPlayerFlags* pf = reinterpret_cast<CPlayerFlags*>(player);
            pf->flags |= 0x10000000;
        }
    }
    return 0;
}

int func_8018FC78(void* p0, void* p1, cf::CfGameManager* gm, u32* p3) {
    func_80080F40__Q22cf13CfGameManagerFv(gm, *p3, 0);
    return 0;
}

int func_8018FCA8(CFuncHost* self, u32 a, u32 b, u32 c, u32 d) {
    // The manager pointer is intentionally re-read from self (+0x408) at every
    // use site; retail reloads it each time rather than caching it in one
    // register, which is what fixes the callee-saved register allocation.
    if (self->manager->unk94[0] != 0) {
        func_8009ECB0();
        if (lbl_eu_80663E24 & 0x100000) {
            // Fixed setup: force players 0..2 into-or-out-of "command" mode
            // (bit 4 at +0x68) and move them by an offset vector before the
            // per-player notify calls. Declaration order mirrors retail's
            // stack window: pos@0x8, offset@0x14, sum@0x20.
            float* base = reinterpret_cast<float*>(func_8023C1B4());
            u8 tag = lbl_eu_8066476D;
            float ox = lbl_eu_80667A70;
            float oy = lbl_eu_80667A74;
            float pos[3];
            float offset[3];
            float sum[3];
            // Loop-invariant scratch pointers: these compile to retail's
            // pre-loop addi rD,sp,imm materialization (pos@0x8, offset@0x14,
            // sum@0x20).
            float* pPos = pos;
            float* pOff = offset;
            float* pSum = sum;
            int byteOfs = 0;
            for (int i = 0; i < 3; i++) {
                cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
                if (p != 0) {
                    CPlayerFlags* pf = reinterpret_cast<CPlayerFlags*>(p);
                    if (tag != 0) pf->flags |= 0x10;
                    else pf->flags &= ~0x10;
                    const float* src = &base[i * 4];
                    offset[0] = ox;
                    offset[1] = oy;
                    offset[2] = ox;
                    // Component-wise sum of the fixed offset vector and the
                    // per-player base row (retail vectorizes with paired singles).
                    pSum[0] = pOff[0] + src[0];
                    pSum[1] = pOff[1] + src[1];
                    pSum[2] = pOff[2] + src[2];
                    pPos[0] = pSum[0];
                    pPos[1] = pSum[1];
                    pPos[2] = pSum[2];
                    (*reinterpret_cast<CPlayerVtbl**>(p))->fw_a8(p, pPos);
                    (*reinterpret_cast<CPlayerVtbl**>(p))->fw_d4(
                        p, const_cast<float*>(src), src[3]);
                }
                byteOfs += 0x10;
            }
        } else {
            // Alternate path: only players 1..2, gated on the busy flag.
            // Scratch matches retail's sp+0x2c..0x38 window (vec3 + pad).
            float stk[4];
            for (int i = 1; i < 3; i++) {
                cf::CfObjectMove* p = self->manager->unk94[i];
                if (p != 0) {
                    func_8008064C__Q22cf13CfGameManagerFv(self->manager->unk94[0], i, stk);
                    if (!(lbl_eu_80663E28 & 0x100)) {
                        (*reinterpret_cast<CPlayerVtbl**>(p))->fw_a8(p, stk);
                        (*reinterpret_cast<CPlayerVtbl**>(self->manager->unk94[0]))->fw_cc(self->manager->unk94[0]);
                        (*reinterpret_cast<CPlayerVtbl**>(p))->fw_c8(p);
                    }
                }
            }
        }
    }

    cf::CfObjectMove* e0 = self->manager->unk94[0];
    if (e0 != 0 && (*reinterpret_cast<CPlayerVtbl**>(e0))->fw_74(e0) == 0) {
        func_80061A80((u32)self, 4, a, b, c, d);
        return 1;
    }

    if (lbl_eu_80663E24 & 0x200) {
        lbl_eu_80663E3C = 0;
        lbl_eu_80663E24 = (lbl_eu_80663E24 & ~0x200) | 0x8;
    }
    if (lbl_eu_80663E24 & 0x400000) {
        // rlwinm 19,17 clears BOTH bits 0x2000 and 0x1000.
        lbl_eu_80663E24 &= ~0x3000;
    } else {
        if (b == 0) func_80061870((u32)self, 0x25, 0, 0, 0, 0);
    }
    if (lbl_eu_80663E24 & 0x100000) {
        func_80061A80((u32)self, 0x24, 0, 0, 0, 0);
        func_80061A80((u32)self, 6, 2, 0, 0, 0);
        func_80061870((u32)self, 0x1f, 0, 0, 0, 0);
    }
    lbl_eu_80663E24 = (lbl_eu_80663E24 & 0xffefbfff) | 0x80;
    lbl_eu_80663E28 &= ~0x200;
    func_800628C4(0x35300001, 5);
    func_80061870((u32)self, 6, 0xf, 0, 0, 0);
    func_80061A80((u32)self, 0xf, 0, 0, 0, 0);
    func_8007FE18__Q22cf13CfGameManagerFv(1);
    return 0;
}

int func_80190034(CFuncHost* self) {
    cf::CfGameManager* gm =
        reinterpret_cast<cf::CfGameManager*>(self->manager->unk94[0]);
    gm->func_80082C48();
    return 0;
}

int func_80190060(void* self) { return 0; }

int func_80190068(void* self) { return 0; }

int func_80190070(u32 p0, int p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p1 != 0) {
        func_80061A80(p0, 6, (u16)(p1 - 1), p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_801900C0(void* p0, int a, int b) {
    if (a != 0) {
        func_8012F860();
    }
    if (b != 0) {
        func_801338C8();
        func_80133AE8();
    }
    return 0;
}

int func_80190108(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (func_8012FA5C() != 0) {
        func_8012FAA8();
    } else {
        func_80061A80(p0, 0x1f, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_8019017C(CFuncHost* self) {
    self->manager->func_80086778();
    return 0;
}

// Colour-unpack helper: convert the four bytes of `color` to normalized
// floats (byte * scale) and hand them to the scene vec4 setter when bit 7 of
// the mode word is clear. Retail leaves arg2 (r4) unset at the call site, so
// the local is intentionally uninitialized.
int func_801901A4(u32 p0, u32 p1, u32 color) {
    const float scale = lbl_eu_80667A78;
    u8* c = reinterpret_cast<u8*>(&color);
    float cr = c[0] * scale;
    float cg = c[1] * scale;
    float cb = c[2] * scale;
    float ca = c[3] * scale;
    if (!(lbl_eu_80663E28 & 0x1000000)) {
        // Retail leaves arg2 (r4) unconsumed here; forwarding the untouched
        // p1 parameter keeps r4 untouched too.
        func_800407C8_tmp v = { cr, cg, cb, ca };
        func_8049602C(reinterpret_cast<CScnNw4r*>(lbl_eu_80663E14), (int)p1, &v);
    }
    return 0;
}

int func_80190254(int a, int b) { extern int func_801C0094(int); func_801C0094(b); return 0; }

int func_8019027C(void* self) { return 0; }

unsigned long func_80190284() {
    lbl_eu_80663E24 |= 8;
    return 0;
}

// Menu-open request dispatcher: forward cmd 0x12 to the task queue unless the
// manager is locked (sentinel id -1 at +0x88, or busy per func_80085838), in
// which case abort: clear the pending-event bit, reset the scene state and
// disable the menu input path.
int func_80190298(CFuncHost* self, u32 p1, u32 p2, u32 p3, u32 p4) {
    cf::CfGameManager* mgr = self->manager;
    if (mgr->field_0x88 != -1 && func_80085838__Q22cf13CfGameManagerFv() == 0) {
        func_80061A80((u32)self, 0x12, p1, p2, p3, p4);
        return 1;
    }
    lbl_eu_80663E24 &= ~0x100;
    // Retail invokes the reset hook with a null this pointer.
    ((cf::CfGameManager*)NULL)->func_80085FB8();
    func_80068DAC();
    func_800B98C8(0);
    return 0;
}

int func_80190334(u32 p0, u16 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p1 != 0) {
        func_80061A80(p0, 0x13, (u16)(p1 - 1), p2, p3, p4);
        r = 1;
    } else {
        lbl_eu_80663E28 &= ~1;
    }
    return r;
}

int func_80190394(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if ((u32)(u16)p2 + 0x10000 != 0xffff && func_802A3748((u16)p2)) {
        func_80061A80(p0, 0x16, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_80190414(void* p0, void* p1) {
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player != 0) {
        func_800BE12C((u8*)player, (int)p1, 0, 0, 1);
    }
    return 0;
}

int func_80190464(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    if (func_80085838__Q22cf13CfGameManagerFv() == 0) {
        func_80061A80(p0, 0x18, p1, p2, p3, p4);
        return 1;
    }
    return 0;
}

int func_801904D0(void* self) {
    // If the colour-fade is already active (bit30 of lbl28), just clear it.
    // Otherwise set up an all-zero colour mix and dispatch a full-colour
    // request event when the colour-change latch (bit12) is set.
    // Bit 1 (0x2): colour-fade-active latch.
    if (!(lbl_eu_80663E28 & 0x2)) {
        float f = lbl_eu_80667A70;
        UnkFloat4 v;
        v.field_0x0 = f;
        v.field_0x4 = f;
        v.field_0x8 = f;
        v.field_0xC = f;
        func_8008566C__Q22cf13CfGameManagerFv(0x1e, &v, 1);
        if (lbl_eu_80663E24 & 0x80000) {
            func_80061870((u32)self, 0x1c, 0x28, 0, 0, 0);
        }
    } else {
        // Volatile view forces retail's fresh reload of the flag word here
        // (MWCC would otherwise CSE it with the test above).
        lbl_eu_80663E28 = *(volatile u32*)&lbl_eu_80663E28 & ~0x2;
    }
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

// Party-menu event dispatcher: cmd 0 resets every player's tint flag and
// closes the party gauges; cmd 0x19 re-triggers the first-player colour flash
// via func_8008402C; any other cmd forwards (cmd-1) to the task queue.
// Retail keeps the (redundant-looking) per-player cmd==0 re-test inside the
// loop, so it is written out explicitly here.
int func_80190568(u32 self, u32 cmd, u32 a2, u32 a3, u32 a4) {
    int result = 0;
    if (cmd != 0) {
        if (cmd == 0x19) {
            func_8008402C__Q22cf13CfGameManagerFv(lbl_eu_80663E48[1],
                                                  lbl_eu_80663E4C[1],
                                                  lbl_eu_80667A88);
        }
        func_80061A80(self, 0x1c, (u16)(cmd - 1), a2, a3, a4);
        result = 1;
    } else {
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player != 0 && cmd == 0) {
                // Clear the tint-request bit (bit 3 from MSB) then run the
                // +0x168 virtual tint setter and the gauge reset helper.
                CPlayerFlags* pf = reinterpret_cast<CPlayerFlags*>(player);
                pf->flags &= ~0x10000000;
                player->CfObject_UnkVirtualFunc70(lbl_eu_80667A8C);
                func_800BC3B0(player, lbl_eu_80667A88);
            }
        }
        func_80061870(self, 6, 0x1e, 0, 0, 0);
        func_80061870(self, 0x1d, 0, 0, 0, 0);
    }
    return result;
}

int func_80190690(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (func_8023C1C0() != 0) {
        func_80061A80(p0, 0x1e, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

extern "C" unsigned long func_801906FC() {
    lbl_eu_80663E24 &= ~0x80000;
    return 0;
}

int func_80190710() {
    func_8012F860();
    func_801338C8();
    func_80133AE8();
    return 0;
}

int func_8019073C() {
    if (func_8011C2E8() == 0) {
        func_801338C8();
    }
    return 0;
}

int func_8019076C(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (p2 != 0 && func_80062A84(p2) == 0) {
        func_80061A80(p0, 0x1b, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

int func_801907E4() {
    if (!(lbl_eu_80663E24 & 0x400000)) {
        float f = lbl_eu_80667A70;
        UnkFloat4 v;
        v.field_0x0 = f;
        v.field_0x4 = f;
        v.field_0x8 = f;
        v.field_0xC = f;
        func_8008566C__Q22cf13CfGameManagerFv(0x1e, &v, 0);
        func_80085878__Q22cf13CfGameManagerFv();
    }
    func_801AAC78(0);
    return 0;
}

int func_eu_80191E88(CFuncHost408* self, u32 arg1, u32 arg2) {
    // func_8009D5FC is declared in CfGameManager.hpp (CfFileEventIdsView*);
    // retail treats the result as this 4-byte pair.
    UnkR31_8019E88* p = reinterpret_cast<UnkR31_8019E88*>(func_8009D5FC());
    func_8009EB2C((arg2 >> 20) & 0x7f, (arg2 >> 10) & 0x3ff, self->field_0x408 + 0x28);
    if (func_eu_80062E58(p->field_0x2, p->field_0x0, 3) == 0) {
        func_80061D2C(reinterpret_cast<UnkClass_80085334*>(self), 0x28);
    }
    return 0;
}

int func_eu_80191F08(u32 p0, u32 p1, u32 p2, u32 p3, u32 p4) {
    int r = 0;
    if (!(lbl_eu_80663E28 & 0x2000)) {
        func_80061A80(p0, 0x28, p1, p2, p3, p4);
        r = 1;
    }
    return r;
}

// Menu-command pump: pop an 8-byte record off the CfResBuffer ring and
// dispatch it through the member-function-pointer handler table indexed by
// the cmd word's top byte; bits 20-23 hold the popped data-word count and
// bit 19 suppresses dispatch for that record. A handler returning nonzero
// publishes the command byte and ends the pump.
int func_80190840(MenuCmdRingView* buf, u32* outFlag) {
    *outFlag = 0;
    if (!(lbl_eu_80663E28 & 0x10000) && cf::CfGameManager::func_800829B8()) {
        return buf->field_404 == 0;
    }
    MenuCmdHost* host = reinterpret_cast<MenuCmdHost*>(buf);
    u32 rec[2];
    u32 data[0x20];
    while (buf->field_404 != 0) {
        int result;
        func_80061C5C(buf, &rec[0], data);
        u32 cmd = rec[0];
        if (!((cmd >> 19) & 1)) {
            result = 1;
            u32 idx = cmd >> 24;
            if (idx < 0x29) {
                result = (host->*lbl_eu_80532838[idx])((u16)cmd, rec[1], data,
                                                       (cmd >> 20) & 0xF);
            }
            if (result != 0) {
                *outFlag = rec[0] >> 24;
                break;
            }
        }
    }
    return buf->field_404 == 0;
}

extern float lbl_eu_80667A90;
float func_80190938() { return lbl_eu_80667A90; }

// State-gate probe: refresh the actor's current control word through the
// +0x4 sub-object's slot-0x30 getter, then test it against `gate` via
// func_80174C98. Inlined by MWCC at every retail call site.
static int probeGate90940(FuncActorRef* actor, u32 gate) {
    u32 val = *(u32*)(*reinterpret_cast<CActorSubVtbl**>(actor->field_0004))
                  ->fw_30(actor->field_0004);
    return func_80174C98(actor, &val, gate);
}

// Battle-target search used by the party menu: validates the queried actor
// against a long chain of state gates, then sweeps the enum-list of nearby
// actors (tracking distance to each candidate), and finally runs a
// mode-specific selection pass over the closest candidates.
int func_80190940(FuncResultRef* self, FuncActorRef* actor, int mode,
                  FuncActorRef* filter) {
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = NULL;

    if (probeGate90940(actor, 0x802) == 0) return 0;
    if (probeGate90940(actor, 1) == 0) return 0;
    cf::CfGameManager::getInstance();
    if (!func_8006EF04__Fi(0x100)) return 0;
    if (func_8009CF8C(0x335f) != 0) return 0;
    if (func_8017FD44() != 0) return 0;
    if (!func_8006EF04__Fi(0x400)) return 0;
    if (!func_8006EF04__Fi(0x1000)) return 0;
    if (probeGate90940(actor, 0x1f) == 0) return 0;
    if (probeGate90940(actor, 0x1000) == 0) return 0;
    if (probeGate90940(actor, 0x806) == 0) return 0;

    BmView90940* bm = reinterpret_cast<BmView90940*>(getInstance__Q22cf14CBattleManagerFv());
    if (bm->field_0x1aa == 0) return 0;
    if (bm->field_0x20c8 == 0) return 0;

    // If a talk object is attached its +0x374 word must be set.
    if (actor->field_3f60 != NULL &&
        reinterpret_cast<Sub3F60View*>(actor->field_3f60)->field_0x374 == 0) {
        return 0;
    }
    if ((*reinterpret_cast<CActorVtblExt**>(actor))->fw_2bc(actor) != 0) {
        return 0;
    }

    u8* tags = actor->field_0008;
    // None of the busy/exclusion tags may be present; tag 0xB must be.
    if (func_80148778(tags, 0x9) != 0) return 0;
    if (func_80148778(tags, 0xf) != 0) return 0;
    if (func_80148778(tags, 0x10) != 0) return 0;
    if (func_80148778(tags, 0xa) != 0) return 0;
    if (func_80148778(tags, 0xb) == 0) return 0;
    // None of the event-lockout tags may be present either.
    if (func_80148778(tags, 0xeb) != 0) return 0;
    if (func_80148778(tags, 0xf0) != 0) return 0;
    if (func_80148778(tags, 0xf1) != 0) return 0;
    if (func_80148778(tags, 0xf8) != 0) return 0;
    if (probeGate90940(actor, 0xa) == 0) return 0;
    if (probeGate90940(actor, 0xb) == 0) return 0;
    if (probeGate90940(actor, 0x1a) == 0) return 0;
    if (func_8004C5EC(actor->field_3f60) == 0x31) return 0;

    long r190 = (*reinterpret_cast<CActorCmpVtbl**>(actor))->fw_190(actor);
    long r18c = (*reinterpret_cast<CActorCmpVtbl**>(actor))->fw_18c(actor);
    if (r190 == r18c) return 0;

    // While in state 5 the actor's voice-height must stay below the cap.
    if (actor->field_3f28 == 5 &&
        (*reinterpret_cast<CActorVtblExt**>(actor))->fw_158(actor) >=
            lbl_eu_80667A94) {
        return 0;
    }

    self->field_0008 = 0;
    CfEnumListHolder90940 holder;
    func_80043E08(&holder, 0x20, 0);

    float dists[2];       // sp+0x38
    FuncActorRef* cands[2]; // sp+0x40

    // Candidate sweep: collect actors whose position sub-object differs from
    // the queried one, recording their distance (mag of position delta minus
    // each candidate's bias float). The two-slot arrays intentionally overlap
    // later stack slots like retail when more than two entries appear.
    int count = 0;
    for (int i = 0; i < (int)((CfEnumList90940*)func_80043F18(&holder))->count; i++) {
        u8* basePos = actor ? reinterpret_cast<u8*>(&actor->field_3e9c)
                            : reinterpret_cast<u8*>(NULL);
        u8* data = func_800F6EAC((CfEnumList90940*)func_80043F18(&holder), i);
        if (data == basePos) continue;
        data = func_800F6EAC((CfEnumList90940*)func_80043F18(&holder), i);
        FuncActorRef* cand = (data != NULL)
            ? reinterpret_cast<FuncActorRef*>(data - 0x3e9c)
            : static_cast<FuncActorRef*>(NULL);
        cands[count] = cand;

        // Component-wise distance between the actor's and the candidate's
        // +0x3E9C positions (slot-0xAC Vec3 getters); MWCC pairs the XY/ZW
        // subtracts into ps_sub.
        u8* posSubA = reinterpret_cast<u8*>(&actor->field_3e9c);
        u8* posSubB = reinterpret_cast<u8*>(&cand->field_3e9c);
        const float* posA =
            (*reinterpret_cast<CSubPosVtblAC**>(posSubA))->fw_ac(posSubA);
        const float* posB =
            (*reinterpret_cast<CSubPosVtblAC**>(posSubB))->fw_ac(posSubB);
        Vec diff;
        diff.x = posB[0] - posA[0];
        diff.y = posB[1] - posA[1];
        diff.z = posB[2] - posA[2];
        float mag = PSVECMag(&diff);
        dists[count] = mag - cand->field_44d8;
        count++;
    }
    // Move the nearer of the first two candidates into slot 0.
    if (cands[1] != NULL && dists[0] > dists[1]) {
        float td = dists[0];
        FuncActorRef* tp = cands[0];
        dists[0] = dists[1];
        cands[0] = cands[1];
        dists[1] = td;
        cands[1] = tp;
    }

#define ACCEPT(v, subFlagSet)                                                  \
    do {                                                                       \
        self->field_000c = cand;                                               \
        if (subFlagSet) self->field_0008 = 1;                                  \
        self->field_0004 = (v);                                                \
        __dt__80043E88(&holder, -1);                                           \
        return (v);                                                            \
    } while (0)

    if (mode <= 1) {
        // Mode 0/1: nearest candidate engaged in battle with a free slot.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if ((*reinterpret_cast<CActorVtblExt**>(cand))->fw_2bc(cand) == 0)
                continue;
            u8* candPos = reinterpret_cast<u8*>(&cand->field_3e9c);
            if ((*reinterpret_cast<CSubPosVtbl84**>(candPos))->fw_84(candPos) == 0)
                continue;
            if (func_8004C5EC(cand->field_3f60) != 5) continue;
            if (probeGate90940(cand, 0x1d) != 0) continue;
            if (bm->field_0x194 < 100) continue;
            if (dists[k] > thr) continue;
            if (filter != NULL && filter != cand) continue;
            ACCEPT(1, false);
        }
    }
    if (mode == 0 || mode == 2) {
        // Mode 0/2: nearest candidate carrying one of the support tags.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if ((*reinterpret_cast<CActorVtblExt**>(cand))->fw_2bc(cand) != 0)
                continue;
            u8* tags2 = cand->field_0008;
            if (!(func_80148778(tags2, 0xf) != 0 ||
                  func_80148778(tags2, 0x9) != 0 ||
                  func_80148778(tags2, 0xb) != 0))
                continue;
            if (dists[k] > thr) continue;
            if (filter != NULL && filter != cand) continue;
            ACCEPT(2, false);
        }
    }
    if (mode == 0 || mode == 3) {
        // Mode 0/3: nearest candidate with the chain-attack tag ready.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if ((*reinterpret_cast<CActorVtblExt**>(cand))->fw_2bc(cand) != 0)
                continue;
            if (func_80148778(cand->field_0008, 0x10) == 0) continue;
            if (probeGate90940(cand, 0x16) != 0) continue;
            if (dists[k] > thr) continue;
            if (filter != NULL && filter != cand) continue;
            ACCEPT(3, false);
        }
    }
    if (mode == 0 || mode == 4) {
        // Mode 0/4: revive-target scan gated on the battle manager's
        // revive-handler registration (member-function-pointer compare).
        int gate = 0;
        if (func_800EA444() != 0 &&
            reinterpret_cast<BmByte261A4*>(bm)->field_0x261a4 == 0) {
            gate = 1;
        }
        int armed = 0;
        if (gate && bm->field_0x194 >= 100) {
            armed = 1;
        }
        if (armed) {
            u32 pmfLocal[3];
            pmfLocal[0] = lbl_eu_80532A28[0];
            pmfLocal[1] = lbl_eu_80532A28[1];
            pmfLocal[2] = lbl_eu_80532A28[2];
            if (__ptmf_cmpr(reinterpret_cast<BmPtmf28354*>(bm)->field_0x28354,
                            pmfLocal, lbl_eu_80532A28) != 0) {
                gate = 2;
            }
        }
        if (gate == 2) {
            if (count == 0) {
                self->field_0004 = 0;
                __dt__80043E88(&holder, -1);
                return 0;
            }
            const float thr = lbl_eu_80667A90;
            for (int k = 0; k < count; k++) {
                FuncActorRef* cand = cands[k];
                if ((*reinterpret_cast<CActorVtblExt**>(cand))->fw_2bc(cand) !=
                    0)
                    continue;
                if (dists[k] > thr) continue;
                if (cand->field_3388 & 0x10) continue;
                if (func_8027E018(&bm->field_0x20c8, cand) == 0) continue;
                ACCEPT(4, true);
            }
        }
    }
    if (mode == 0 || mode == 7) {
        // Mode 0/7: nearest candidate available for a top-up action.
        const float thr = lbl_eu_80667A90;
        for (int k = 0; k < count; k++) {
            FuncActorRef* cand = cands[k];
            if ((*reinterpret_cast<CActorVtblExt**>(cand))->fw_2bc(cand) != 0)
                continue;
            if (probeGate90940(cand, 0x1d) != 0) continue;
            if ((*reinterpret_cast<CActorVtblExt**>(cand))->fw_308(cand) >=
                2)
                continue;
            if (dists[k] > thr) continue;
            ACCEPT(7, false);
        }
    }
#undef ACCEPT

    {
        int res = self->field_0004;
        __dt__80043E88(&holder, -1);
        return res;
    }
}

// Member dtors (~CPartyStateWin, ~CBgTex) and the CProcess base dtor are
// compiler-generated here, matching retail's guard structure.
CMenuPTState::~CMenuPTState() {}

// Party-menu state initialization: rebuild the background texture and the
// party-state window on the stack, copy them member-by-member into the
// persistent fields (region layout per CMenuPTWinCopyView), then register
// the IScnRender sub-object as a render callback at priority 0xd.
void CMenuPTState::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(true);
    func_8007FE2C__Q22cf13CfGameManagerFv();
    {
        // Retail builds both temporaries via the unmangled slice ctor symbols,
        // so they are constructed into raw storage under C linkage. The view
        // macros re-derive the addresses at every statement: retail keeps no
        // cached pointers in callee-saved registers across the copy.
        u32 bgStore[sizeof(CBgTex) / 4];
#define bgTemp reinterpret_cast<CBgTex*>(bgStore)
        __ct__CBgTex(bgTemp, 0);
        field_0x60.mMemRegion = bgTemp->mMemRegion;
        field_0x60.mFileHandle = bgTemp->mFileHandle;
        field_0x60.mLayout = bgTemp->mLayout;
        field_0x60.mLayoutReady = bgTemp->mLayoutReady;
        field_0x60.mLoaded = bgTemp->mLoaded;
        field_0x60.mPtmMode = bgTemp->mPtmMode;
        __dt__6CBgTexFv(bgTemp, -1);
#undef bgTemp
    }
    func_801C3C14(&field_0x60);

    u32 winStore[sizeof(CPartyStateWin) / 4];
#define winTmp reinterpret_cast<CPartyStateWin*>(winStore)
#define d reinterpret_cast<CMenuPTWinCopyView*>(&field_0x80)
#define s reinterpret_cast<CMenuPTWinCopyView*>(winStore)
    __ct__CPartyStateWin(winTmp, (u32)this, (u32)field_0x5C);
    // Memberwise copy of the freshly built window into the persistent member.

    d->v0008 = s->v0008;
    d->v000c = s->v000c;
    d->v0010 = s->v0010;
    d->v0014 = s->v0014;
    d->v001c = s->v001c;
    d->v0020 = s->v0020;
    d->v0024 = s->v0024;
    d->v0028 = s->v0028;
    d->v002c = s->v002c;
    d->v0030 = s->v0030;
    d->v0034 = s->v0034;
    d->v0038 = s->v0038;
    d->v003c = s->v003c;
    d->v0040 = s->v0040;
    d->v0044 = s->v0044;
    d->v0048 = s->v0048;
    d->v004c = s->v004c;
    d->v004d = s->v004d;
    d->v004e = s->v004e;
    d->v004f = s->v004f;
    d->v0054 = s->v0054;
    d->v0058 = s->v0058;
    d->v005c = s->v005c;
    d->v0060 = s->v0060;
    // func_80191C88 is imported under C linkage with opaque u8* parameters
    // (see CPartyStateWin.hpp); call sites pass the blob sub-objects.
    func_80191C88((u8*)&d->blob0064, (const u8*)&s->blob0064);
    func_80191C88((u8*)&d->blob05a8, (const u8*)&s->blob05a8);
    func_80191C88((u8*)&d->blob0ae4, (const u8*)&s->blob0ae4);
    d->v1020 = s->v1020;
    d->v1024 = s->v1024;
    d->v1028 = s->v1028;
    d->v102c = s->v102c;
    d->v1030 = s->v1030;
    d->v1034 = s->v1034;
    d->v1038 = s->v1038;
    d->v103c = s->v103c;
    d->v1040 = s->v1040;
    d->v1044 = s->v1044;
    d->v1048 = s->v1048;
    d->v104c = s->v104c;
    d->v1050 = s->v1050;
    func_80191C88((u8*)&d->blob1054, (const u8*)&s->blob1054);
    d->v1590 = s->v1590;
    d->v1594 = s->v1594;
    func_80191C88((u8*)&d->blob1598, (const u8*)&s->blob1598);
    func_80191C88((u8*)&d->blob1ad4, (const u8*)&s->blob1ad4);
    d->v2010 = s->v2010;
    d->v2014 = s->v2014;
    d->v2018 = s->v2018;
    d->v201c = s->v201c;
    d->v2020 = s->v2020;
    d->v2024 = s->v2024;
    d->v2028 = s->v2028;
    d->v202c = s->v202c;
    d->v2030 = s->v2030;
    d->v2034 = s->v2034;
    d->v2038 = s->v2038;
    d->v203c = s->v203c;
    d->v2040 = s->v2040;
    func_80191C88((u8*)&d->blob2044, (const u8*)&s->blob2044);
    d->v2580 = s->v2580;
    d->v2584 = s->v2584;
    func_80191C88((u8*)&d->blob2588, (const u8*)&s->blob2588);
    func_80191C88((u8*)&d->blob2ac4, (const u8*)&s->blob2ac4);
    d->v3000 = s->v3000;
    d->v3004 = s->v3004;
    d->v3008 = s->v3008;
    d->v300c = s->v300c;
    d->v3010 = s->v3010;
    d->v3014 = s->v3014;
    d->v3018 = s->v3018;
    d->v301c = s->v301c;
    d->v3020 = s->v3020;
    d->v3024 = s->v3024;
    d->v3028 = s->v3028;
    d->v302c = s->v302c;
    d->v3030 = s->v3030;
    d->v3034 = s->v3034;
    d->v303c = s->v303c;
    d->v3040 = s->v3040;
    d->v3044 = s->v3044;
    d->v3048 = s->v3048;
    d->v304c = s->v304c;
    d->v3050 = s->v3050;
    d->v3054 = s->v3054;
    d->v3058 = s->v3058;
    d->v305c = s->v305c;
    d->v3060 = s->v3060;
    d->v3064 = s->v3064;
    d->v3068 = s->v3068;
    d->v3069 = s->v3069;
    d->kpack3084.v3087 = s->kpack3084.v3087;
    d->v3070 = s->v3070;
    d->v3074 = s->v3074;
    d->v3078 = s->v3078;
    d->v307c = s->v307c;
    d->v3080 = s->v3080;
    d->v3081 = s->v3081;
    d->kpack3084.b3084 = s->kpack3084.b3084;
    d->kpack3084.b3085 = s->kpack3084.b3085;
    d->kpack3084.b3086 = s->kpack3084.b3086;
    d->kpack3084.h308b = s->kpack3084.h308b;
    d->kpack3084.b308d = s->kpack3084.b308d;
    d->kpack3084.b308e = s->kpack3084.b308e;
    d->kpack3084.b308f = s->kpack3084.b308f;
    d->v309c = s->v309c;
    d->v30a0 = s->v30a0;
    d->v30a4 = s->v30a4;
    d->v30a8 = s->v30a8;
    func_80191C88((u8*)&d->blob30ac, (const u8*)&s->blob30ac);
    d->v35e8 = s->v35e8;
    d->v35ec = s->v35ec;
    func_80191C88((u8*)&d->blob35f0, (const u8*)&s->blob35f0);
    func_80191C88((u8*)&d->blob3b2c, (const u8*)&s->blob3b2c);
    {
        // 15-word run, fully unrolled (retail emits lwz/stw per element).
        d->v4068[0] = s->v4068[0];
        d->v4068[1] = s->v4068[1];
        d->v4068[2] = s->v4068[2];
        d->v4068[3] = s->v4068[3];
        d->v4068[4] = s->v4068[4];
        d->v4068[5] = s->v4068[5];
        d->v4068[6] = s->v4068[6];
        d->v4068[7] = s->v4068[7];
        d->v4068[8] = s->v4068[8];
        d->v4068[9] = s->v4068[9];
        d->v4068[10] = s->v4068[10];
        d->v4068[11] = s->v4068[11];
        d->v4068[12] = s->v4068[12];
        d->v4068[13] = s->v4068[13];
        d->v4068[14] = s->v4068[14];
    }
    d->v40a4 = s->v40a4;
    d->v40a8 = s->v40a8;
    d->v40ac = s->v40ac;
    d->v40b0 = s->v40b0;
    d->v40b1 = s->v40b1;
    // 13-record 8-byte pair walk (retail lwzu/stwu mtctr/bdnz loop) plus the
    // trailing word copied after the loop.
    u32* sp13 = (u32*)&s->pairs13[0].lo - 1;
    u32* dp13 = (u32*)&d->pairs13[0].lo - 1;
    for (int n = 13; n != 0; n--) {
        u32 lo = *(sp13 + 1);
        u32 hi = *(sp13 + 2);
        *(dp13 + 1) = lo;
        *(dp13 + 2) = hi;
        sp13 += 2;
        dp13 += 2;
    }
    *(dp13 + 1) = *(sp13 + 1);
    d->v4120 = s->v4120;
    d->v4124 = s->v4124;
    d->v4128 = s->v4128;
    d->v412c = s->v412c;
    d->v4130 = s->v4130;
    d->v4134 = s->v4134;
    d->v4138 = s->v4138;
    d->v413c = s->v413c;
    d->v4140 = s->v4140;
    d->v4144 = s->v4144;
    d->v4148 = s->v4148;
    d->v414c = s->v414c;
    d->v4154 = s->v4154;
    d->v4158 = s->v4158;
    d->v415c = s->v415c;
    d->v4160 = s->v4160;
    d->v4164 = s->v4164;
    d->v4168 = s->v4168;
    d->v416c = s->v416c;
    d->v4170 = s->v4170;
    d->v4174 = s->v4174;
    d->v4178 = s->v4178;
    d->v417c = s->v417c;
    d->v4180 = s->v4180;
    d->v4184 = s->v4184;
    d->v4188 = s->v4188;
    d->v418c = s->v418c;
    d->v4198 = s->v4198;
    d->v419c = s->v419c;
    d->v419d = s->v419d;
    d->v41a4 = s->v41a4;
    d->v41a8 = s->v41a8;
    d->v41ac = s->v41ac;
    d->v41b0 = s->v41b0;
    d->v41b4 = s->v41b4;
    d->v41b5 = s->v41b5;
    d->v41bc = s->v41bc;
    d->v41c0 = s->v41c0;
    d->v41c4 = s->v41c4;
    d->v41c8 = s->v41c8;
    d->v41cc = s->v41cc;
    d->v41cd = s->v41cd;
    d->v41d4 = s->v41d4;
    d->v41d8 = s->v41d8;
    d->v41dc = s->v41dc;
    d->v41e0 = s->v41e0;
    d->opack41e8.v41e8 = s->opack41e8.v41e8;
    d->opack41e8.v41e9 = s->opack41e8.v41e9;
    d->opack41e8.v41ea = s->opack41e8.v41ea;
    d->opack41e8.v41ee = s->opack41e8.v41ee;
    func_80166F80(&d->f41f4, &s->f41f4);
    func_80192268(&d->blob4400, &s->blob4400);
    // Tail run (CSysWin flags + window timer float).
    d->v6bac = s->v6bac;
    d->v6bb0 = s->v6bb0;
    d->v6bb4 = s->v6bb4;
    d->v6bb8 = s->v6bb8;
    d->v6bbc = s->v6bbc;
    d->v6bc0 = s->v6bc0;
    d->v6bc4 = s->v6bc4;
    d->v6bc8 = s->v6bc8;
    d->v6bcc = s->v6bcc;
    d->v6bd0 = s->v6bd0;
    d->v6bd4 = s->v6bd4;
    d->v6bd8 = s->v6bd8;
    d->v6bdc = s->v6bdc;
    d->v6bdd = s->v6bdd;
    d->v6bde = s->v6bde;
    d->v6bdf = s->v6bdf;
    d->v6be0 = s->v6be0;
    d->v6be1 = s->v6be1;
    d->v6be4 = s->v6be4;
    d->v6be5 = s->v6be5;
    d->v6be8 = s->v6be8;

    __dt__14CPartyStateWinFv(winTmp, -1);
#undef winTmp
#undef d
#undef s

    field_0x5C->addRenderCB(this, 0xd, 1);
    func_804962A0(field_0x5C, 0);
}

// Memberwise copy of the SCopy_80191C88 state blob from src to dst, in the
// exact order retail reads them (regions 0x00/0x10/0x39/0x499/0x4db are not
// copied). The three 8-byte runs compile to mtctr/bdnz lwzu+stwu copy loops
// (loop counts: 52, 16, 16). Retail is size-optimized here: under unit -O4,p
// the long runs unroll and the float copies get batched into f14-f31 (which
// drags in _savegpr/_restgpr prologue code retail doesn't have), so this one
// function compiles with optimize_for_size like other rolled-copy sites.
// NOTE: no codegen-steering pragmas here: the vu*/vf* volatile field types
// already force the retail load/store program order and keep the pair runs
// rolled (see SCopy_80191C88 above).
void func_80191C88(SCopy_80191C88* dst, const SCopy_80191C88* src) {
    dst->f_04 = src->f_04;
    dst->f_08 = src->f_08;
    dst->f_0c = src->f_0c;
    dst->f_14 = src->f_14;
    dst->f_18 = src->f_18;
    dst->f_1c = src->f_1c;
    dst->f_20 = src->f_20;
    dst->f_24 = src->f_24;
    dst->f_28 = src->f_28;
    dst->f_2c = src->f_2c;
    dst->f_30 = src->f_30;
    dst->f_34 = src->f_34;
    dst->b_38 = src->b_38;
    dst->f_3c = src->f_3c;

    // Biased one element back so the accesses fuse into lwzu/stwu forms
    // (retail mtctr/bdnz walk shape).
    u32* sA = (u32*)&src->arrA[0].lo - 1;
    u32* dA = (u32*)&dst->arrA[0].lo - 1;
    for (int n = 52; n != 0; n--) {
        u32 lo = *(sA + 1);
        u32 hi = *(sA + 2);
        *(dA + 1) = lo;
        *(dA + 2) = hi;
        sA += 2;
        dA += 2;
    }

    dst->f_1e0 = src->f_1e0; dst->f_1e4 = src->f_1e4;
    dst->f_1e8 = src->f_1e8; dst->f_1ec = src->f_1ec;
    dst->f_1f0 = src->f_1f0; dst->f_1f4 = src->f_1f4;
    dst->f_1f8 = src->f_1f8; dst->f_1fc = src->f_1fc;
    dst->f_200 = src->f_200; dst->f_204 = src->f_204;
    dst->f_208 = src->f_208; dst->f_20c = src->f_20c;
    dst->f_210 = src->f_210; dst->f_214 = src->f_214;
    dst->f_218 = src->f_218; dst->f_21c = src->f_21c;
    dst->f_220 = src->f_220;
    dst->b_224 = src->b_224; dst->b_225 = src->b_225;
    dst->b_226 = src->b_226;

    dst->f_228 = src->f_228; dst->f_22c = src->f_22c;
    dst->f_230 = src->f_230; dst->f_234 = src->f_234;
    dst->f_238 = src->f_238; dst->f_23c = src->f_23c;
    dst->f_240 = src->f_240; dst->f_244 = src->f_244;
    dst->f_248 = src->f_248; dst->f_24c = src->f_24c;
    dst->f_250 = src->f_250; dst->f_254 = src->f_254;
    dst->f_258 = src->f_258; dst->f_25c = src->f_25c;
    dst->f_260 = src->f_260; dst->f_264 = src->f_264;
    dst->f_268 = src->f_268;
    dst->f_26c = src->f_26c;

    for (int i = 0; i < 16; i++) {
        dst->arrB[i].lo = src->arrB[i].lo;
        dst->arrB[i].hi = src->arrB[i].hi;
    }
    for (int i = 0; i < 16; i++) {
        dst->arrC[i].lo = src->arrC[i].lo;
        dst->arrC[i].hi = src->arrC[i].hi;
    }

    dst->f_370 = src->f_370; dst->f_374 = src->f_374;
    dst->f_378 = src->f_378; dst->f_37c = src->f_37c;
    dst->f_380 = src->f_380; dst->f_384 = src->f_384;
    dst->f_388 = src->f_388; dst->f_38c = src->f_38c;
    dst->f_390 = src->f_390; dst->f_394 = src->f_394;
    dst->f_398 = src->f_398; dst->f_39c = src->f_39c;

    dst->f_3a0 = src->f_3a0; dst->f_3a4 = src->f_3a4;
    dst->f_3a8 = src->f_3a8; dst->f_3ac = src->f_3ac;
    dst->f_3b0 = src->f_3b0; dst->f_3b4 = src->f_3b4;
    dst->f_3b8 = src->f_3b8; dst->f_3bc = src->f_3bc;
    dst->f_3c0 = src->f_3c0; dst->f_3c4 = src->f_3c4;
    dst->f_3c8 = src->f_3c8; dst->f_3cc = src->f_3cc;
    dst->f_3d0 = src->f_3d0; dst->f_3d4 = src->f_3d4;
    dst->f_3d8 = src->f_3d8; dst->f_3dc = src->f_3dc;
    dst->f_3e0 = src->f_3e0; dst->f_3e4 = src->f_3e4;
    dst->f_3e8 = src->f_3e8; dst->f_3ec = src->f_3ec;
    dst->f_3f0 = src->f_3f0; dst->f_3f4 = src->f_3f4;
    dst->f_3f8 = src->f_3f8; dst->f_3fc = src->f_3fc;
    dst->f_400 = src->f_400; dst->f_404 = src->f_404;
    dst->f_408 = src->f_408; dst->f_40c = src->f_40c;
    dst->f_410 = src->f_410; dst->f_414 = src->f_414;
    dst->f_418 = src->f_418; dst->f_41c = src->f_41c;
    dst->f_420 = src->f_420; dst->f_424 = src->f_424;
    dst->f_428 = src->f_428; dst->f_42c = src->f_42c;

    dst->f_430 = src->f_430; dst->f_434 = src->f_434;
    dst->f_438 = src->f_438; dst->f_43c = src->f_43c;
    dst->f_440 = src->f_440; dst->f_444 = src->f_444;
    dst->f_448 = src->f_448; dst->f_44c = src->f_44c;
    dst->f_450 = src->f_450; dst->f_454 = src->f_454;
    dst->f_458 = src->f_458; dst->f_45c = src->f_45c;
    dst->f_460 = src->f_460; dst->f_464 = src->f_464;
    dst->f_468 = src->f_468; dst->f_46c = src->f_46c;
    dst->f_470 = src->f_470; dst->f_474 = src->f_474;
    dst->f_478 = src->f_478; dst->f_47c = src->f_47c;
    dst->f_480 = src->f_480; dst->f_484 = src->f_484;
    dst->f_488 = src->f_488; dst->f_48c = src->f_48c;
    dst->f_490 = src->f_490; dst->f_494 = src->f_494;
    dst->b_498 = src->b_498;
    dst->f_49c = src->f_49c; dst->f_4a0 = src->f_4a0;
    dst->f_4a4 = src->f_4a4;

    dst->f_4a8 = src->f_4a8; dst->f_4ac = src->f_4ac;
    dst->f_4b0 = src->f_4b0; dst->f_4b4 = src->f_4b4;
    dst->f_4b8 = src->f_4b8;
    dst->b_4bc = src->b_4bc; dst->b_4bd = src->b_4bd;
    dst->b_4be = src->b_4be; dst->b_4bf = src->b_4bf;
    dst->f_4c0 = src->f_4c0; dst->f_4c4 = src->f_4c4;
    dst->f_4c8 = src->f_4c8;
    dst->f_4cc = src->f_4cc; dst->f_4d0 = src->f_4d0;
    dst->h_4d4 = src->h_4d4; dst->h_4d6 = src->h_4d6;
    dst->h_4d8 = src->h_4d8;
    dst->b_4da = src->b_4da;
    dst->h_4dc = src->h_4dc;
    dst->f_4e0 = src->f_4e0; dst->f_4e4 = src->f_4e4;
    dst->f_4e8 = src->f_4e8; dst->f_4ec = src->f_4ec;
    dst->f_4f0 = src->f_4f0; dst->f_4f4 = src->f_4f4;
    dst->f_4f8 = src->f_4f8; dst->f_4fc = src->f_4fc;
    dst->f_500 = src->f_500; dst->f_504 = src->f_504;
    dst->f_508 = src->f_508; dst->f_50c = src->f_50c;
    dst->f_510 = src->f_510;
    dst->f_514 = src->f_514; dst->f_518 = src->f_518;
    dst->f_51c = src->f_51c; dst->f_520 = src->f_520;
    dst->f_524 = src->f_524; dst->f_528 = src->f_528;
    dst->f_52c = src->f_52c;
    dst->h_530 = src->h_530;
    dst->h_532 = src->h_532; dst->h_534 = src->h_534;
    dst->h_536 = src->h_536; dst->h_538 = src->h_538;
}

// Per-field copy of a large (~0x27A8-byte) state blob from src to dst, in the
// exact order retail reads them. The five runs of 8-byte elements compile to
// mtctr/bdnz lwzu+stwu copy loops (loop counts: 16, 18, 16, 0x400, 0x80).
void func_80192268(SCopy_80192268* dst, const SCopy_80192268* src) {
    dst->f_04 = src->f_04; dst->f_08 = src->f_08;
    dst->f_0c = src->f_0c; dst->f_10 = src->f_10;
    dst->f_14 = src->f_14; dst->f_18 = src->f_18;
    dst->f_1c = src->f_1c; dst->f_20 = src->f_20;
    dst->f_24 = src->f_24; dst->f_28 = src->f_28;
    dst->f_2c = src->f_2c; dst->f_30 = src->f_30;
    dst->f_34 = src->f_34; dst->f_38 = src->f_38;
    dst->f_3c = src->f_3c;

    dst->b_40 = src->b_40; dst->b_41 = src->b_41;
    dst->b_42 = src->b_42; dst->b_43 = src->b_43;

    dst->f_48 = src->f_48; dst->f_4c = src->f_4c;
    dst->f_50 = src->f_50; dst->f_54 = src->f_54;
    dst->b_58 = src->b_58; dst->b_59 = src->b_59;

    dst->f_60 = src->f_60; dst->f_64 = src->f_64;
    dst->f_68 = src->f_68; dst->f_6c = src->f_6c;
    dst->b_70 = src->b_70; dst->b_71 = src->b_71;

    dst->f_78 = src->f_78; dst->f_7c = src->f_7c;
    dst->f_80 = src->f_80; dst->f_84 = src->f_84;
    dst->b_88 = src->b_88; dst->b_89 = src->b_89;

    dst->f_90 = src->f_90; dst->f_94 = src->f_94;
    dst->f_98 = src->f_98; dst->f_9c = src->f_9c;
    dst->f_a0 = src->f_a0; dst->f_a4 = src->f_a4;
    dst->f_a8 = src->f_a8; dst->f_ac = src->f_ac;
    dst->f_b0 = src->f_b0;

    dst->b_b4 = src->b_b4; dst->b_b5 = src->b_b5;
    dst->b_b6 = src->b_b6; dst->b_b7 = src->b_b7;

    dst->f_bc = src->f_bc; dst->f_c0 = src->f_c0;
    dst->f_c4 = src->f_c4; dst->f_c8 = src->f_c8;
    dst->f_cc = src->f_cc; dst->f_d0 = src->f_d0;
    dst->f_d4 = src->f_d4; dst->f_d8 = src->f_d8;
    dst->b_dc = src->b_dc; dst->b_dd = src->b_dd;
    dst->b_de = src->b_de; dst->b_df = src->b_df;

    dst->f_e0 = src->f_e0; dst->f_e4 = src->f_e4;
    dst->f_e8 = src->f_e8; dst->f_ec = src->f_ec;
    dst->f_f0 = src->f_f0;
    dst->b_f4 = src->b_f4;

    for (int i = 0; i < 16; i++) {
        dst->arr1[i].lo = src->arr1[i].lo;
        dst->arr1[i].hi = src->arr1[i].hi;
    }

    dst->b_178 = src->b_178; dst->b_179 = src->b_179;
    dst->b_17a = src->b_17a;

    dst->f_180 = src->f_180; dst->f_184 = src->f_184;
    dst->f_188 = src->f_188; dst->f_18c = src->f_18c;
    dst->f_190 = src->f_190; dst->f_194 = src->f_194;
    dst->f_198 = src->f_198; dst->f_19c = src->f_19c;
    dst->f_1a0 = src->f_1a0;
    dst->b_1a4 = src->b_1a4;
    dst->f_1a8 = src->f_1a8; dst->f_1ac = src->f_1ac;

    dst->b_1b0 = src->b_1b0; dst->b_1b1 = src->b_1b1;
    dst->b_1b2 = src->b_1b2; dst->b_1b3 = src->b_1b3;
    dst->b_1b4 = src->b_1b4; dst->b_1b5 = src->b_1b5;

    dst->f_1bc = src->f_1bc; dst->f_1c0 = src->f_1c0;
    dst->f_1c4 = src->f_1c4; dst->f_1c8 = src->f_1c8;
    dst->f_1cc = src->f_1cc; dst->f_1d0 = src->f_1d0;
    dst->f_1d4 = src->f_1d4; dst->f_1d8 = src->f_1d8;
    dst->f_1dc = src->f_1dc;
    dst->b_1e0 = src->b_1e0;
    dst->f_1e4 = src->f_1e4; dst->f_1e8 = src->f_1e8;

    dst->b_1ec = src->b_1ec; dst->b_1ed = src->b_1ed;
    dst->b_1ee = src->b_1ee; dst->b_1ef = src->b_1ef;
    dst->b_1f0 = src->b_1f0; dst->b_1f1 = src->b_1f1;

    dst->b_1f4 = src->b_1f4; dst->b_1f5 = src->b_1f5;
    dst->b_1f6 = src->b_1f6;
    dst->h_1f8 = src->h_1f8; dst->h_1fa = src->h_1fa;
    dst->h_1fc = src->h_1fc;
    dst->b_1fe = src->b_1fe;

    dst->f_200 = src->f_200; dst->f_204 = src->f_204;
    dst->f_208 = src->f_208; dst->f_20c = src->f_20c;
    dst->f_210 = src->f_210; dst->f_214 = src->f_214;
    dst->f_218 = src->f_218; dst->f_21c = src->f_21c;
    dst->f_220 = src->f_220;
    dst->f_224 = src->f_224;

    for (int i = 0; i < 18; i++) {
        dst->arr2[i].lo = src->arr2[i].lo;
        dst->arr2[i].hi = src->arr2[i].hi;
    }

    dst->f_2b8 = src->f_2b8; dst->f_2bc = src->f_2bc;
    dst->f_2c0 = src->f_2c0; dst->f_2c4 = src->f_2c4;
    dst->f_2c8 = src->f_2c8; dst->f_2cc = src->f_2cc;
    dst->f_2d0 = src->f_2d0; dst->f_2d4 = src->f_2d4;
    dst->f_2d8 = src->f_2d8; dst->f_2dc = src->f_2dc;
    dst->f_2e0 = src->f_2e0; dst->f_2e4 = src->f_2e4;
    dst->b_2e8 = src->b_2e8;

    for (int i = 0; i < 16; i++) {
        dst->arr3.items[i].lo = src->arr3.items[i].lo;
        dst->arr3.items[i].hi = src->arr3.items[i].hi;
    }

    dst->f_36c = src->f_36c;
    dst->h_370 = src->h_370;
    dst->b_372 = src->b_372; dst->b_373 = src->b_373;
    dst->b_374 = src->b_374; dst->b_375 = src->b_375;
    dst->b_376 = src->b_376; dst->b_377 = src->b_377;
    dst->b_378 = src->b_378; dst->b_379 = src->b_379;
    dst->b_37a = src->b_37a; dst->b_37b = src->b_37b;
    dst->b_37c = src->b_37c;

    for (int i = 0; i < 1024; i++) {
        dst->arr4.items[i].lo = src->arr4.items[i].lo;
        dst->arr4.items[i].hi = src->arr4.items[i].hi;
    }

    dst->h_237e = src->h_237e;
    dst->b_2380 = src->b_2380; dst->b_2381 = src->b_2381;
    dst->b_2382 = src->b_2382; dst->b_2383 = src->b_2383;
    dst->f_2384 = src->f_2384; dst->f_2388 = src->f_2388;
    dst->f_238c = src->f_238c; dst->f_2390 = src->f_2390;
    dst->f_2394 = src->f_2394; dst->f_2398 = src->f_2398;
    dst->f_239c = src->f_239c; dst->f_23a0 = src->f_23a0;

    for (int i = 0; i < 128; i++) {
        dst->arr5[i].lo = src->arr5[i].lo;
        dst->arr5[i].hi = src->arr5[i].hi;
    }

    dst->f_27a4 = src->f_27a4;
}

void CMenuPTState::Term() {
    CDeviceVI::waitForDrawDone();
    func_804962A0(field_0x5C, 1);
    field_0x5C->removeRenderCB(this);
    func_801C3D9C((u8*)&field_0x60);
    func_801FA254((u8*)&field_0x80);
    lbl_eu_80664300 = 0;
    func_8007FECC__Q22cf13CfGameManagerFv();
    func_8008294C__Q22cf13CfGameManagerFv(false);
    func_80135550();
}

// Minimal view of the pad word block returned by getCurrentPad() (only the
// +4 button word is read by CMenuPTState::Move).
struct MenuPTPadView {
    u8 _0[4];
    u32 buttons; // 0x4
};

// Per-frame party-menu update: gate on task state, run the open/close input
// edge while the window is settling, then advance the 4-state phase byte.
void CMenuPTState::Move() {
    CTaskGame::getInstance();
    // OR-combined guard: retail emits bne-exit for the first disjunct and a
    // beq-body / b-exit pair for the second (same shape as cbRenderBefore).
    if (CTaskGame::func_800426F0() != 0 || (lbl_eu_80663E28 & 0x200000) != 0) {
        return;
    }
    {
        if ((u8)(field_0x6C6C + 0xff) <= 1) {
            if (func_801FA524(&field_0x80) == 0) {
                MenuPTPadView* pad = reinterpret_cast<MenuPTPadView*>(
                    cf::CfGameManager::getCurrentPad());
                int accept;
                // Retail extracts single bits via rotate-left forms.
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    accept = (pad->buttons >> 23) & 1;
                } else {
                    accept = (pad->buttons >> 10) & 1;
                }
                if (accept != 0) {
                    func_801FA4F4(&field_0x80);
                    if (func_800FEDF8() != 0) {
                        func_800FF914();
                        func_80138078(6);
                    }
                    field_0x6C6C = 3;
                }
            }
        }
        switch (field_0x6C6C) {
        case 0:
            // Background layout finished loading -> start the window open-in.
            if (func_801C3E34(&field_0x60) != 0) {
                // Retail leaves arg2/arg3 (r4/r5) unset at this call site.
                func_801F941C(&field_0x80);
                field_0x6C6C = 1;
            }
            break;
        case 1:
            field_0x6C6C = 2;
            break;
        case 2:
            if (func_801FA4EC(&field_0x80) != 0) {
                field_0x6C6C = 3;
            }
            break;
        case 3:
            // System window opened over us: flag completion on the owner.
            if (func_8012FA5C() != 0) {
                field_0x54 = 1;
            }
            break;
        }
        func_801C3D54(&field_0x60);
        if (field_0x6C6C != 3) {
            func_801FA338(&field_0x80);
        }
    }
}

void CMenuPTState::cbRenderBefore() {
    // OR-combined guard: MWCC emits the retail beq-body/b-exit pair for the
    // second disjunct (MWCC_CASES "OR-combined guard" pattern).
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() != 0 || (lbl_eu_80663E28 & (1u << 21)) != 0) {
        return;
    }
    if (func_8013BE50() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
    func_801C3D7C(&field_0x60, &drawInfo);
}