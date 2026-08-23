// Auto-scaffolded catalog TU for kyoshin/cf/code_8018F8D8
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>

#include "kyoshin/cf/code_8018F8D8.hpp"

// CTaskGame.hpp declares func_8004392C with a u32 third arg and func_8049603C
// with a CScn* arg, conflicting with the CVision.hpp / CSuddenCommu.hpp copies
// reached via CBattleManager.hpp below; this TU uses neither copy.
#define func_8004392C f8d8CtaskGame4392CUnused
#define func_8049603C f8d8CtaskGame9603CUnused
#include "kyoshin/CTaskGame.hpp"
#undef func_8049603C
#undef func_8004392C
// Several headers redeclare battle-manager imports with incompatible
// signatures; this TU calls none of them. Pre-include CfObjectActor.hpp so
// its 5-arg func_80174B4C declaration keeps the real name, then rename the
// CChainTimer / CVision copies and every free-function singleton getter out
// of the way (same idiom as CMenuBattlePlayerState.cpp).
#define getInstance__Q22cf14CBattleManagerFv f8d8BmGetInstanceActor
#include "kyoshin/cf/object/CfObjectActor.hpp"
#undef getInstance__Q22cf14CBattleManagerFv
#define func_80174B4C f8d8ChainTimer74B4CUnused
#define getInstance__Q22cf14CBattleManagerFv f8d8BmGetInstanceChain
#include "kyoshin/cf/CBattleManager.hpp"
#undef getInstance__Q22cf14CBattleManagerFv
#undef func_80174B4C

// C-linkage (unmangled) helpers referenced by the catalog functions below.
// These retail symbols are defined in other TUs / the linked library.
extern "C" {
    extern u32 lbl_eu_80663E24;
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
    int  func_80174C98(void* actor, int* outVal, int flags);
    bool func_8006EF04__Fi(int mask);
    u32  func_8009CF8C(u32 resource);
    int  func_80148778(void* obj, int id);
    int  func_8027E018(void* obj, void* arg);
    void __dt__80043E88(void* holder, int flag);
    int func_80061A80(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    int func_80061870(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
    void func_800BE12C(u8* obj, int a, int b, int c, int d);
    void func_8012F860();
    void func_801338C8();
    void func_80133AE8();
    bool func_8011C2E8();
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
    int  func_80061D2C(const void* obj, int idx);
    void func_801C3D9C(u8* obj);
    void func_801FA254(u8* obj);
    void func_8008566C__Q22cf13CfGameManagerFv(u32 mode, const UnkFloat4* value, u32 third);
    void func_80085878__Q22cf13CfGameManagerFv();
    int  func_80085840__Q22cf13CfGameManagerFv();
    int  func_80085838__Q22cf13CfGameManagerFv();
    void func_8007FECC__Q22cf13CfGameManagerFv();
    void func_8008294C__Q22cf13CfGameManagerFv(bool enable);
    void func_8007E0D0__Q22cf13CfGameManagerFv(bool alternate);
}

namespace cf {}
using namespace cf;

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
    FuncHostMgrView* mgr = reinterpret_cast<FuncHostMgrView*>(self->manager);
    cf::CfObjectMove* found = 0;
    u16 selectId = 0;
    if (lbl_eu_80663E24 & 0x100000) {
        selectId = lbl_eu_8066476D;
    }
    found = func_8007FF6C__Q22cf13CfGameManagerFv(lbl_eu_80663E40, mgr->field_0xC,
                                                  mgr->field_0x24);
    if (lbl_eu_80663E24 & 0x100000) {
        // Fixed setup: force players into/out of "command" mode (bit 4 at
        // +0x68), then move each by an offset vector before the notify calls.
        u8 tag = lbl_eu_8066476D;
        float* base = reinterpret_cast<float*>(func_8023C1B4());
        float ox = lbl_eu_80667A70;
        float oy = lbl_eu_80667A74;
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player == 0) continue;
            CPlayerFlags* pf = reinterpret_cast<CPlayerFlags*>(player);
            if (tag != 0) pf->flags |= 0x10;
            else pf->flags &= ~0x10;
            float v[3] = { ox + base[i * 4 + 0], oy + base[i * 4 + 1],
                           ox + base[i * 4 + 2] };
            (*reinterpret_cast<CPlayerVtbl**>(player))->fw_a8(player, v);
            (*reinterpret_cast<CPlayerVtbl**>(player))
                ->fw_d4(player, &base[i * 4], base[i * 4 + 3]);
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
            // per-player notify calls.
            u8 tag = lbl_eu_8066476D;
            float* base = reinterpret_cast<float*>(func_8023C1B4());
            float ox = lbl_eu_80667A70;
            float oy = lbl_eu_80667A74;
            for (int i = 0; i < 3; i++) {
                cf::CfObjectMove* p = cf::CfGameManager::getPlayer(i);
                if (p == 0) continue;
                CPlayerFlags* pf = reinterpret_cast<CPlayerFlags*>(p);
                if (tag != 0) pf->flags |= 0x10;
                else pf->flags &= ~0x10;
                float v[3] = { ox + base[i*4 + 0], oy + base[i*4 + 1], ox + base[i*4 + 2] };
                (*reinterpret_cast<CPlayerVtbl**>(p))->fw_a8(p, v);
                (*reinterpret_cast<CPlayerVtbl**>(p))->fw_d4(p, &base[i*4], base[i*4 + 3]);
            }
        } else {
            // Alternate path: only players 1..2, gated on the busy flag.
            // Scratch matches retail's sp+0x2c..0x38 window (vec3 + pad).
            float stk[4];
            for (int i = 1; i < 3; i++) {
                cf::CfObjectMove* p = self->manager->unk94[i];
                if (p == 0) continue;
                func_8008064C__Q22cf13CfGameManagerFv(self->manager->unk94[0], i, stk);
                if (lbl_eu_80663E28 & 0x100) continue;
                (*reinterpret_cast<CPlayerVtbl**>(p))->fw_a8(p, stk);
                (*reinterpret_cast<CPlayerVtbl**>(self->manager->unk94[0]))->fw_cc(self->manager->unk94[0]);
                (*reinterpret_cast<CPlayerVtbl**>(p))->fw_c8(p);
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
        lbl_eu_80663E24 &= ~0x2000;
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
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 |= 8;
    return 0;
}

void func_80190298(){}

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
    if (!(lbl_eu_80663E28 & 0x2)) {
        float f = lbl_eu_80667A70;
        UnkFloat4 v;
        v.field_0x0 = v.field_0x4 = v.field_0x8 = v.field_0xC = f;
        func_8008566C__Q22cf13CfGameManagerFv(0x1e, &v, 1);
        if (lbl_eu_80663E24 & 0x80000) {
            func_80061870((u32)self, 0x1c, 0x28, 0, 0, 0);
        }
    } else {
        lbl_eu_80663E28 &= ~0x2;
    }
    func_80085878__Q22cf13CfGameManagerFv();
    return 0;
}

// Party-menu event dispatcher: cmd 0 resets every player's tint flag and
// closes the party gauges; cmd 0x19 re-triggers the first-player colour flash
// via func_8008402C; any other cmd forwards (cmd-1) to the task queue.
int func_80190568(u32 self, u32 cmd, u32 a2, u32 a3, u32 a4) {
    int result = 0;
    if (cmd == 0) {
        for (int i = 0; i < 3; i++) {
            cf::CfObjectMove* player = cf::CfGameManager::getPlayer(i);
            if (player == 0) continue;
            // Clear the tint-request bit (bit 3 from MSB) then run the
            // +0x168 virtual tint setter and the gauge reset helper.
            CPlayerFlags* pf = reinterpret_cast<CPlayerFlags*>(player);
            pf->flags &= ~0x10000000;
            (*reinterpret_cast<CPlayerVtbl**>(player))->fw_168(player,
                                                               lbl_eu_80667A8C);
            func_800BC3B0(player, lbl_eu_80667A88);
        }
        func_80061870(self, 6, 0x1e, 0, 0, 0);
        func_80061870(self, 0x1d, 0, 0, 0, 0);
    } else {
        if (cmd == 0x19) {
            func_8008402C__Q22cf13CfGameManagerFv(lbl_eu_80663E48[1],
                                                  lbl_eu_80663E4C[1],
                                                  lbl_eu_80667A88);
        }
        func_80061A80(self, 0x1c, (u16)(cmd - 1), a2, a3, a4);
        result = 1;
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
    extern unsigned long lbl_eu_80663E24;
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
        func_80061D2C(self, 0x28);
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

// Menu-command pump: pop header/data records off the CfResBuffer ring and
// dispatch each through the member-function-pointer handler table indexed by
// the header's top byte. A record with bit 12 (0x80000) set stops the pump;
// a handler returning nonzero publishes the command byte and ends it too.
int func_80190840(MenuCmdRingView* buf, u32* outFlag) {
    *outFlag = 0;
    if (!(lbl_eu_80663E28 & 0x10000) && cf::CfGameManager::func_800829B8()) {
        int result = 1;
        while (buf->field_404 != 0) {
            u32 hdr;
            u32 data[8];
            func_80061C5C(buf, &hdr, data);
            if (hdr & 0x80000) break;
            result = 1;
            u32 idx = hdr >> 24;
            if (idx < 0x29) {
                result = (reinterpret_cast<MenuCmdHost*>(buf)
                              ->*lbl_eu_80532838[idx])((u16)hdr, data);
            }
            if (result != 0) {
                *outFlag = hdr >> 24;
                break;
            }
        }
    }
    return buf->field_404 == 0;
}

extern float lbl_eu_80667A90;
float func_80190938() { return lbl_eu_80667A90; }

void func_80190940(){}

// Member dtors (~CPartyStateWin, ~CBgTex) and the CProcess base dtor are
// compiler-generated here, matching retail's guard structure.
CMenuPTState::~CMenuPTState() {}

void CMenuPTState::Init() {}

// Memberwise copy of the SCopy_80191C88 state blob from src to dst, in the
// exact order retail reads them (regions 0x00/0x10/0x39/0x499/0x4db are not
// copied). The three 8-byte runs compile to mtctr/bdnz lwzu+stwu copy loops
// (loop counts: 52, 16, 16). Note: the retail bloat-free form (f0 reuse, no
// FPR-save prologue) requires -O4,s; under unit -O4,p MWCC over-schedules the
// float copies into f14-f31.
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

    for (int i = 0; i < 52; i++) {
        dst->arrA[i].lo = src->arrA[i].lo;
        dst->arrA[i].hi = src->arrA[i].hi;
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
        dst->arr3[i].lo = src->arr3[i].lo;
        dst->arr3[i].hi = src->arr3[i].hi;
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
        dst->arr4[i].lo = src->arr4[i].lo;
        dst->arr4[i].hi = src->arr4[i].hi;
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
    if (CTaskGame::func_800426F0()) {
        return;
    }
    if (lbl_eu_80663E28 & 0x200000) {
        return;
    }
    if (lbl_eu_80663E28 & 0x200000) {
    } else {
        if ((u8)(field_0x6C6C - 1) <= 1) {
            if (func_801FA524(&field_0x80) == 0) {
                MenuPTPadView* pad = reinterpret_cast<MenuPTPadView*>(
                    cf::CfGameManager::getCurrentPad());
                int accept;
                if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
                    accept = pad->buttons & 0x400000;
                } else {
                    accept = pad->buttons & 0x400;
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
                int uninit1;
                int uninit2;
                func_801F941C(&field_0x80, uninit1, uninit2);
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
    // Gate: skip while a task event is running or a realtime event is busy,
    // then disable Z testing and draw the background texture via a temp
    // DrawInfo (construct -> setup -> CBgTex draw -> destroy).
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0()) {
        return;
    }
    if (lbl_eu_80663E28 & (1u << 21)) {
        return;
    } else {
        if (func_8013BE50() == 0) {
            return;
        }
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
        field_0x60.func_801C3D7C(&drawInfo);
    }
}