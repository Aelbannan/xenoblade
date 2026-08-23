#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"
#include "kyoshin/CBgTex.hpp"
#include "revolution/gx/GXPixel.h"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/core/CPadManager.hpp"
#include <nw4r/lyt/lyt_drawInfo.h>

// ---------------------------------------------------------------------------
// Minimal CTaskGame view: getInstance is static, but func_800426F0 is a MEMBER
// call in retail (the instance returned by getInstance flows straight into the
// second call without a register reload), so it must be declared as a member.
// ---------------------------------------------------------------------------
class CTaskGame {
public:
    static CTaskGame* getInstance();
    bool func_800426F0();
};

// Render-gate mode bitfield (.sbss); bit 21 set means rendering is blocked.
extern u32 lbl_eu_80663E28;

// Singleton instance pointer for the crystal-crafting menu process.
extern void* lbl_eu_806646C8;

// Retail-unmangled data symbols (global-scope var names are not mangled).
extern char lbl_eu_8052BF70[];   // interim CProcess composite vtable
extern char lbl_eu_80535AD0[];   // composite vtable (IScnRender sub-vtable at +0x24)
extern u32 __ptmf_null[3];       // null pointer-to-member-function constant

// CProcess base ctor (retail mangled symbol; C linkage binds it literally).
extern "C" void __ct__8CProcessFv(CProcess* self);

// Constructor (defined below; retail symbol is the unmangled global).
extern "C" CMenuMakeCrystal* __ct__CMenuMakeCrystal(CMenuMakeCrystal* pThis, void* param);

// ---------------------------------------------------------------------------
// Retail-unmangled callees bound literally (US relocs carry stripped names;
// see CTaskGameEvt.cpp / CSysWinScenarioLog.cpp precedent).
// ---------------------------------------------------------------------------
extern "C" {
void waitForDrawDone__9CDeviceVIFv();
void func_801338C8();
void func_80137250__FPQ34nw4r3lyt8DrawInfo(nw4r::lyt::DrawInfo* di);
int  func_8013BE50();
void func_801C3D7C(void* self, nw4r::lyt::DrawInfo* di);   // CBgTex layout draw
void func_801C3D54(void* self);                            // CBgTex animate
int  func_80212B68(void* self);                            // CMakeCrystalWin query
int  func_80212B70(void* self);                            // CMakeCrystalWin query
void func_80212A68(void* self);                            // CMakeCrystalWin tick
int  func_800FEDF8();
void func_800FF914();
void __ct__CBgTex(void* self, int arg);
void __ct__UnkClass_8011C974(void* self, const void* src);
void func_804962A0(void*, int);
void func_801C3D9C(void*);
void func_8021299C(void*);
void func_8008294C__Q22cf13CfGameManagerFv(int);
// Retail symbol keeps its Fv mangling even though it takes the pad index.
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
// Unmangled local helper (bound literally).
extern "C" void func_802124AC(void* self);
void func_801C3C14(void* self);                            // CBgTex init
void func_80210E9C(void* dst, const void* src);            // big state copy
void func_80211CEC(void* dst, const void* src);            // big state copy
void func_801BE16C(void* dst, void* src);                  // win data post-ctor
void func_80212158(void* dst, const void* src);            // state record copy
void addRenderCB__4CScnFP10IScnRenderUlUl(void* scn, void* render, u32 a, u32 b);
int  func_8009CF8C(int id);
void func_8011C400();
void func_802A1500();
void func_80189C88();
void func_80043C88();
void cbRenderBefore__16CMenuMakeCrystalFv(void*);
void* __dt__15CMakeCrystalWinFv(void*, int);
void __dt__6CBgTexFv(void*, int);
void __dt__800FED0C(void*, int);
void __ct__CMakeCrystalWin(void* self, u32 arg4, u32 arg5);
}

namespace cf {
class CfGameManager {
public:
    static CPad* getCurrentPad();
};
} // namespace cf

// UI sound effect (func_80138078__FUl).
void func_80138078(u32 op);
// func_802124AC helper: init the CBgTex sub-object and arm the state byte.
void func_802124AC(CMenuMakeCrystal* self);

// ---------------------------------------------------------------------------
// Factory: create the singleton CMenuMakeCrystal under `parent`.
// Returns NULL when the singleton already exists; otherwise allocates the
// 0x43E8-byte object from work memory, constructs it, registers it and
// returns the stored singleton (reloaded from the global after Regist).
// ---------------------------------------------------------------------------
CMenuMakeCrystal* func_8021240C(CProcess* parent, void* arg) {
    if (lbl_eu_806646C8 != 0) {
        return 0;
    }
    CMenuMakeCrystal* obj = reinterpret_cast<CMenuMakeCrystal*>(
        mtl::MemManager::allocate(0x43e8, CWorkThreadSystem::getWorkMem()));
    if (obj != 0) {
        // The ctor returns its argument, so the checked pointer flows straight
        // into the singleton store (retail keeps it live in r3).
        obj = __ct__CMenuMakeCrystal(obj, arg);
    }
    lbl_eu_806646C8 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return static_cast<CMenuMakeCrystal*>(lbl_eu_806646C8);
}

// extern "C" free-function form (CCol6CheckBat precedent): retail calls the
// sub-object dtors at +0x80 (CMakeCrystalWin), +0x60 (CBgTex), then the
// +0x00 embedded dtor (flags 0), then the flags-based delete; stmw/lmw frame.
#pragma push
#pragma optimize_for_size on
extern "C" void* __dt__16CMenuMakeCrystalFv(void* self, int flags) {
    if (self != 0) {
        __dt__15CMakeCrystalWinFv((u8*)self + 0x80, -1);
        __dt__6CBgTexFv((u8*)self + 0x60, -1);
        __dt__800FED0C(self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}
#pragma pop

// Stack temporaries used by Init(). The retail frame is 0x43A0 bytes; these
// mirror the local objects at their frame offsets so the callee calls see the
// same sub-object layout.
// CBgTex-shaped temp (frame +0x08): UnkClass_8011C974 sub-object at +0x04,
// then three words and three bytes copied out to this+0x74..+0x7E.
struct MCBgTemp {
    u8 _00[0x04];       // +0x00
    u8 unk[0x10];       // +0x04: UnkClass_8011C974 sub-object
    u32 field_0x14;
    u32 field_0x18;
    u8 field_0x1C;
    u8 field_0x1D;
    u8 field_0x1E;
};
// CMakeCrystalWin-shaped temp (frame +0x28): four words at +0x08 copied to
// this+0x88..+0x94, remaining bytes consumed by func_801BE16C.
struct MCWinTemp {
    u8 _00[0x08];
    u32 field_0x08;     // frame +0x30
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u8 tail[0x38];      // frame +0x40..+0x77
};
// Trailing state bytes of CMenuMakeCrystal written by Init.
struct MCTailState {
    u8 _00[0x74];
    u32 field_0x74;
    u32 field_0x78;
    u8 field_0x7C;
    u8 field_0x7D;
    u8 field_0x7E;
    u8 _7F[0x43E0 - 0x7F];
    u8 ready0;          // 0x43E0
    u8 ready1;          // 0x43E1
    u8 _e2[3];
    u8 gate;            // 0x43E5
};
// Bytes at frame +0x4388/+0x4389 inside the record temp.
struct MCRecTail {
    u8 _00[0x30];
    u8 b30;
    u8 b31;
};

void CMenuMakeCrystal::Init() {
    func_802A1500();
    func_80189C88();
    func_80043C88();
    func_8008294C__Q22cf13CfGameManagerFv(1);

    // Build a temporary CBgTex, clone its UnkClass sub-object into this+0x64
    // plus its scalar fields into this+0x74..+0x7E, then destroy the temp.
    MCBgTemp bg;
    __ct__CBgTex(&bg, 0);
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(this) + 0x64, bg.unk);
    reinterpret_cast<MCTailState*>(this)->field_0x74 = bg.field_0x14;
    reinterpret_cast<MCTailState*>(this)->field_0x78 = bg.field_0x18;
    reinterpret_cast<MCTailState*>(this)->field_0x7C = bg.field_0x1C;
    reinterpret_cast<MCTailState*>(this)->field_0x7D = bg.field_0x1D;
    reinterpret_cast<MCTailState*>(this)->field_0x7E = bg.field_0x1E;
    __dt__6CBgTexFv(&bg, -1);

    func_801C3C14(reinterpret_cast<u8*>(this) + 0x60);

    // Build a temporary CMakeCrystalWin seeded from this object, harvest its
    // four words into this+0x88..+0x94, hand the rest to func_801BE16C, then
    // destroy the temp.
    MCWinTemp win;
    __ct__CMakeCrystalWin(
        &win, reinterpret_cast<u32>(this),
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x5C));
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x88) = win.field_0x08;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x8C) = win.field_0x0C;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x90) = win.field_0x10;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x94) = win.field_0x14;

    // Large uninitialized stack buffers copied wholesale into the window state
    // (retail copies them before any initialization). Kept as one aggregate so
    // the stack slots stay sequential after the win temp.
    struct {
        u8 mid[0x2DD8];
        u8 upper[0x1508];
        u8 rec[0x32];
    } bufs;

    func_801BE16C(reinterpret_cast<u8*>(this) + 0x98, win.tail);
    func_80210E9C(reinterpret_cast<u8*>(this) + 0xD0, bufs.mid);
    func_80211CEC(reinterpret_cast<u8*>(this) + 0x2EA8, bufs.upper);
    func_80212158(reinterpret_cast<u8*>(this) + 0x43B0, bufs.rec);
    reinterpret_cast<MCTailState*>(this)->ready0 = bufs.rec[0x30];
    reinterpret_cast<MCTailState*>(this)->ready1 = bufs.rec[0x31];
    __dt__15CMakeCrystalWinFv(&win, -1);

    // Register the render callback (the IScnRender sub-object at +0x58) with
    // the owning scene.
    void* render = this;
    if (this != NULL) {
        render = reinterpret_cast<u8*>(this) + 0x58;
    }
    addRenderCB__4CScnFP10IScnRenderUlUl(
        *reinterpret_cast<void**>(reinterpret_cast<u8*>(this) + 0x5C), render,
        0xD, 1);
    func_804962A0(*reinterpret_cast<void**>(reinterpret_cast<u8*>(this) + 0x5C),
                  0);

    reinterpret_cast<MCTailState*>(this)->gate = (func_8009CF8C(0x3386) != 0);
    func_8011C400();
}

void func_80210E9C(){}

void func_802116D4(){}

// Copy fields +0x4..+0x14 from src to dst (byte/word/byte copy).
void func_80211CB8(CMakeCrystalCopyBlock* dst, const CMakeCrystalCopyBlock* src) {
    dst->field_0x4 = src->field_0x4;
    dst->field_0x5 = src->field_0x5;
    dst->field_0x8 = src->field_0x8;
    dst->field_0xC = src->field_0xC;
    dst->field_0x10 = src->field_0x10;
    dst->field_0x14 = src->field_0x14;
}

void func_80211CEC(){}

// Copy-construct the window-state record: run the UnkClass_8011C974 base
// copy-ctor at +0x04, then clone the five words (+0x14..+0x24) and six
// bytes (+0x28..+0x2D) verbatim.
struct MCStateWords {
    u32 w0;
    u32 w1;
    u32 w2;
    u32 w3;
    u32 w4;
};
struct MCStateBytes6 {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
};
struct MCStateRecord {
    u8 _00[0x04];
    u8 base[0x10];   // 0x04-0x13: UnkClass_8011C974 sub-object
    MCStateWords words;    // 0x14-0x27
    MCStateBytes6 bytes;   // 0x28-0x2D
};

MCStateRecord* func_80212158(MCStateRecord* dst, volatile MCStateRecord* src) {
    __ct__UnkClass_8011C974(dst->base, (const void*)src->base);
    dst->words.w0 = src->words.w0;
    dst->words.w1 = src->words.w1;
    dst->words.w2 = src->words.w2;
    dst->words.w3 = src->words.w3;
    dst->words.w4 = src->words.w4;
    dst->bytes.b0 = src->bytes.b0;
    dst->bytes.b1 = src->bytes.b1;
    dst->bytes.b2 = src->bytes.b2;
    dst->bytes.b3 = src->bytes.b3;
    dst->bytes.b4 = src->bytes.b4;
    dst->bytes.b5 = src->bytes.b5;
    return dst;
}

// CScn view exposing the render-callback removal (retail mangled name).
class CScn {
public:
    void removeRenderCB(void* cb);
};

void CMenuMakeCrystal::Term() {
    waitForDrawDone__9CDeviceVIFv();
    func_801338C8();
    func_804962A0(*(void**)((u8*)this + 0x5C), 1);
    void* render = this;
    if (this) render = (u8*)this + 0x58;
    ((CScn*)*(void**)((u8*)this + 0x5C))->removeRenderCB(render);
    func_801C3D9C((u8*)this + 0x60);
    func_8021299C((u8*)this + 0x80);
    lbl_eu_806646C8 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

// State byte at +0x43E4 plus its "ready" flag at +0x43E5.
struct MCStateBytes {
    u8 _00[0x43E4];
    u8 state;     // 0x43E4
    u8 ready;     // 0x43E5
};

// Callback-era flag byte at +0x54 (set when the menu finishes).
struct MCDoneFlag {
    u8 _00[0x54];
    u8 done;
};

void CMenuMakeCrystal::Move() {
    if (CTaskGame::getInstance()->func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    u8* self = reinterpret_cast<u8*>(this);
    MCStateBytes* st = reinterpret_cast<MCStateBytes*>(this);
    MCDoneFlag* done = reinterpret_cast<MCDoneFlag*>(this);

    if (func_80212B70(self + 0x80) == 0) {
        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 btn;
        // Button bit depends on the controller type (classic vs Wii).
        if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
            btn = (pad->mPressedButtonFlags >> 23) & 1;
        } else {
            btn = (pad->mPressedButtonFlags >> 10) & 1;
        }
        if (btn != 0) {
            if (func_800FEDF8() != 0) {
                func_800FF914();
                func_80138078(6);
            }
            st->state = 3;
        }
    }

    switch (st->state) {
    case 0:
        func_802124AC(this);
        break;
    case 1:
        st->state = 2;
        break;
    case 2:
        if (func_80212B68(self + 0x80) != 0) {
            st->state = 3;
        }
        break;
    case 3:
        done->done = 1;
        break;
    }

    func_801C3D54(self + 0x60);
    func_80212A68(self + 0x80);
}

void CMenuMakeCrystal::cbRenderBefore() {
    if (CTaskGame::getInstance()->func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0)
        return;
    GXSetZMode(GX_DISABLE, GX_NEVER, GX_DISABLE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250__FPQ34nw4r3lyt8DrawInfo(&drawInfo);
    func_801C3D7C(reinterpret_cast<u8*>(this) + 0x60, &drawInfo);
}

// Constructor (retail unmangled `__ct__CMenuMakeCrystal`; written as a C-ABI
// global like CTaskGameEvt because the retail symbol carries no class-length
// mangling). Builds the CProcess base, sets the interim composite vtable,
// clears both null-PTMF callback slots (+0x3C/+0x48) and the trailing bytes,
// installs the IScnRender sub-vtable (+0x58 = lbl_eu_80535AD0 + 0x24), stores
// the scene param (+0x5C), then constructs CBgTex (+0x60) and
// CMakeCrystalWin (+0x80) and arms the state bytes.
extern "C" CMenuMakeCrystal* __ct__CMenuMakeCrystal(CMenuMakeCrystal* pThis, void* param) {
    __ct__8CProcessFv(pThis);
    // IScnRender sub-vtable: base label + 0x24 (kept live to the 0x58 store).
    char* vtBase = lbl_eu_80535AD0;
    u32 scnVt = reinterpret_cast<u32>(vtBase) + 0x24;
    u32* p = reinterpret_cast<u32*>(pThis);
    u8* self = reinterpret_cast<u8*>(pThis);

    // Interim composite vtable (pre-PMF copy).
    p[4] = reinterpret_cast<u32>(lbl_eu_8052BF70);

    // NULL PTMF -> callback slots at 0x3C / 0x48 (retail store order
    // 0x40,0x3C,0x44 then 0x4C,0x48,0x50; post-increment derefs fold the
    // first load into `lwzu`, cf. CTaskGameEvt).
    u32 w1 = __ptmf_null[1];
    u32 w0 = __ptmf_null[0];
    u32 w2 = __ptmf_null[2];
    p[0x10] = w1; // 0x40
    p[0xF] = w0;  // 0x3C
    p[0x11] = w2; // 0x44
    w1 = __ptmf_null[1];
    w0 = __ptmf_null[0];
    w2 = __ptmf_null[2];
    p[0x13] = w0; // 0x48
    p[0x12] = w1; // 0x4C
    p[0x14] = w2; // 0x50

    self[0x54] = 0;
    self[0x55] = 0;
    p[0x16] = scnVt;                        // 0x58: IScnRender sub-vtable
    p[0x17] = reinterpret_cast<u32>(param); // 0x5C: owning scene

    __ct__CBgTex(self + 0x60, 0);
    __ct__CMakeCrystalWin(self + 0x80, 0, 0);
    self[0x43E4] = 0;
    self[0x43E5] = 1;
    return pThis;
}

// (lbl_eu_806646C8 != 0)
bool func_80212480() { return lbl_eu_806646C8 != 0; }

void* lbl_eu_806646C8;
void* func_80212BE0(void*);

void* func_80212490() {
    unsigned char* p = static_cast<unsigned char*>(lbl_eu_806646C8);
    if (p != 0) {
        return func_80212BE0(p + 0x80);
    }
    return 0;
}

extern "C" int func_801C3E34(void*);
extern "C" void func_8021260C(void*);

// Init the +0x60 sub-object when it reports active, then set the +0x43E4 flag.
extern "C" void func_802124AC(void* self) {
    if (func_801C3E34((u8*)self + 0x60) != 0) {
        func_8021260C((u8*)self + 0x80);
        *(u8*)((u8*)self + 0x43E4) = 1;
    }
}

// IScnRender vtable this-adjusting thunk for cbRenderBefore.
// IScnRender is a non-primary base at offset 0x58 within CMenuMakeCrystal.
extern "C" void func_802124F4(void* self) {
    ((void(*)(void*))cbRenderBefore__16CMenuMakeCrystalFv)((char*)self - 0x58);
}

// IScnRender vtable this-adjusting thunk for destructor.
extern "C" void func_802124FC(void* self) {
    ((void(*)(void*))__dt__16CMenuMakeCrystalFv)((char*)self - 0x58);
}
