// Auto-scaffolded catalog TU for kyoshin/CPartyStateWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include <revolution/GX.h>
#include "monolib/util/MemManager.hpp"
#include "monolib/core/CTaskManager.hpp"
#include "functions.hpp"
#include "kyoshin/CPartyStateWin.hpp"
#include "monolib/core/CViewFrame.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CPadManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/core/CProc.hpp"
#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>

extern "C" u32 func_801F9694(void) {
    return lbl_eu_80663E10;
}


extern "C" __declspec(noinline) u32 func_801F9684(u8* self) { return *(u32*)(self + 0x6C); }

extern "C" __declspec(noinline) u32 func_801F968C(u8* self) { return *(u32*)(self + 0x70); }


// Memory-decommit callback: returns the alloc handle of the region the size
// belongs to (the last getHandle* result; the caller passes it to
// create__8CScnNw4rFv), and shrinks the corresponding free counter by the
// released size plus the 0x80 block header. The 0x10000 / 0x80000
// thresholds pick which region the size belongs to.
// optimize_for_size merges the r30/r31 saves into stmw (retail shape;
// plain -O4,p emits two stw's + reversed copy order - MWCC_CASES).
#pragma optimize_for_size on
__declspec(noinline) mtl::ALLOC_HANDLE func_801F9894(CPartyStateWinMem* self, s32 size) {
    mtl::ALLOC_HANDLE handle = mtl::MemManager::getHandleMEM2();
    if (size < self->field_0x0 - 0x10000) {
        handle = mtl::MemManager::getHandleMEM1();
        self->field_0x0 -= size + 0x80;
    } else if (size < self->field_0x4 - 0x80000) {
        handle = func_80495FF0(lbl_eu_80663E14);
        self->field_0x4 -= size + 0x80;
    }
    return handle;
}
#pragma optimize_for_size off

// Copies the three 0xFF0-byte blocks and trailing scalars of a
// CPartyStateWinCopy blob; returns dst (retail keeps `this` in r3). src is
// intentionally non-const: MWCC would otherwise hoist the trailing loads
// above the stores (const implies non-aliasing) and batch them, while the
// retail interleaves each load with its store.
__declspec(noinline) CPartyStateWinCopy* func_801F9914(CPartyStateWinCopy* dst,
                                   CPartyStateWinCopy* src) {
    dst->field_0x4 = src->field_0x4;
    func_801F9998(dst->block0, src->block0);
    func_801F9998(dst->block1, src->block1);
    func_801F9998(dst->block2, src->block2);
    dst->field_0x2FD8 = src->field_0x2FD8;
    dst->field_0x2FDC = src->field_0x2FDC;
    dst->field_0x2FE0 = src->field_0x2FE0;
    dst->field_0x2FE4 = src->field_0x2FE4;
    return dst;
}

// Copies a 0xFF0-byte block from src to dst (retail shape: three u32 + one
// SCopy_80191C88 blob, two u32, two more blobs, then a 4x2-word loop) and
// returns dst (retail ends with mr r3,r30). src is intentionally non-const:
// MWCC would otherwise hoist the trailing loads above the stores and batch
// them, while the retail interleaves each load with its store.
// extern "C" keeps the retail unmangled symbol; noinline keeps the call
// sites in func_801F9914 real branches.
#pragma optimize_for_size on
extern "C" __declspec(noinline) u8* func_801F9998(u8* dst, u8* src) {
    CPartyStateWinBlock* d = reinterpret_cast<CPartyStateWinBlock*>(dst);
    CPartyStateWinBlock* s = reinterpret_cast<CPartyStateWinBlock*>(src);
    d->field_0x0 = s->field_0x0;
    d->field_0x4 = s->field_0x4;
    d->field_0x8 = s->field_0x8;
    func_80191C88(d->copy0, s->copy0);
    d->field_0x548 = s->field_0x548;
    d->field_0x54c = s->field_0x54c;
    func_80191C88(d->copy1, s->copy1);
    func_80191C88(d->copy2, s->copy2);
    // Tail: word0, then the four word-pairs, walked by explicit pointers at
    // word0 with +4/+8 offsets. Best -O4,p shape: retail lwz/lwzu + stw/stwu
    // 8-byte counted loop, but MWCC interleaves load/store per word here
    // (retail batches both loads; that shape needs -O4,s - unit flag, see
    // MWCC_CASES wall #6). The field_0xfc8 load is hoisted into a temp
    // so its store lands after the word0 copy (retail order).
    // Retail schedule: hoist field_0xfc8 into a temp before the word0 copy,
    // store word0, then field_0xfc8.
    u32 fc8 = s->field_0xfc8;
    u32 w0 = s->word0;
    u32* wd = &d->word0;
    u32* ws = &s->word0;
    d->word0 = w0;
    d->field_0xfc8 = fc8;
    // Pair loop mirrors the retail load/store schedule exactly: both source
    // words load first (second via a pre-increment walk, giving lwzu), then
    // both stores land (second via pre-increment, giving stwu).
    for (int i = 0; i < 4; i++) {
        u32 a = ws[1];
        ws += 2;
        u32 b = ws[0];
        wd[1] = a;
        wd += 2;
        wd[0] = b;
    }
    return dst;
}
#pragma optimize_for_size off

// Copy helper (retail C-linkage): copy-constructs a 0x58-byte menu-state blob
// from src into dst. The 4-word mem region at +0x04 goes through the
// __ct__UnkClass_8011C974 helper, scalar fields +0x14..+0x31 are copied
// field-by-field, the cursor sub-object at +0x34 through func_8018B0FC, then
// the packed tail +0x4C..+0x57. Returns dst (retail keeps `this` in r3). src
// is intentionally non-const: MWCC would otherwise hoist the field loads above
// the stores and batch them, while the retail interleaves each load with its
// store.
// optimize_for_size merges the r30/r31 saves into stmw (retail shape;
// plain -O4,p emits two stw's - same fix as func_801F9894).
#pragma optimize_for_size on
__declspec(noinline) CPartyStateWinBlob58* func_801F9A48(CPartyStateWinBlob58* self,
                                     CPartyStateWinBlob58* src) {
    __ct__UnkClass_8011C974(&self->field_0x4[0], &src->field_0x4[0]);
    self->field_0x14 = src->field_0x14;
    self->field_0x18 = src->field_0x18;
    self->field_0x1C = src->field_0x1C;
    self->field_0x20 = src->field_0x20;
    self->field_0x24 = src->field_0x24;
    self->field_0x28 = src->field_0x28;
    self->field_0x2C = src->field_0x2C;
    self->field_0x30 = src->field_0x30;
    self->field_0x31 = src->field_0x31;
    func_8018B0FC(&self->field_0x34, &src->field_0x34);
    self->field_0x4C = src->field_0x4C;
    self->field_0x4D = src->field_0x4D;
    self->field_0x4E = src->field_0x4E;
    self->field_0x4F = src->field_0x4F;
    self->field_0x53 = src->field_0x53;
    self->field_0x55 = src->field_0x55;
    self->field_0x56 = src->field_0x56;
    self->field_0x57 = src->field_0x57;
    return self;
}
#pragma optimize_for_size off

// Copy helper (retail C-linkage): copies a 0x10C0-byte state blob from src
// into dst: the scalar head, three 0x53C SCopy_80191C88 regions, the fused
// word pairs, two counted 8-byte pair loops and the trailing scalars.
// Returns dst (retail keeps `this` in r3). src is intentionally non-const
// (the scalar fields must copy load-store interleaved, not batched - same
// scheme as func_801F9A48). The pair runs use the u32 pointer-walk form so
// MWCC keeps them as counted loops at -O4,p (the indexed .lo/.hi form
// unrolls - MWCC_CASES wall #9); the fused word pairs are 8-byte struct
// assignments for the 2-load/2-reverse-store shape (CInfoCfPair pattern).
// optimize_for_size: retail keeps the two pair loops rolled (mtctr/bdnz)
// and merges the saves into stmw - plain -O4,p unrolls (MWCC_CASES wall #6).
#pragma optimize_for_size on
__declspec(noinline) CPartyStateWinBlob10C0* func_801F9B18(CPartyStateWinBlob10C0* self,
                                       CPartyStateWinBlob10C0* src) {
    self->field_0xC = src->field_0xC;
    self->field_0x10 = src->field_0x10;
    self->field_0x14 = src->field_0x14;
    self->field_0x18 = src->field_0x18;
    func_80191C88(self->copy0, src->copy0);
    self->field_0x558 = src->field_0x558;
    self->field_0x55c = src->field_0x55c;
    func_80191C88(self->copy1, src->copy1);
    func_80191C88(self->copy2, src->copy2);
    // Retail shape: value temps (lo first, held longer), dst/src pointer
    // setup at +hi base, stores hi-then-lo.
    u32 vLo = src->pairFd8.lo;
    u32 vHi = src->pairFd8.hi;
    u32* wd = &self->pairFd8.hi;
    u32* ws = &src->pairFd8.hi;
    self->pairFd8.hi = vHi;
    self->pairFd8.lo = vLo;
    for (int i = 0; i < 4; i++) {
        wd[1] = ws[1];
        wd[2] = ws[2];
        wd += 2;
        ws += 2;
    }
    self->field_0x1000 = src->field_0x1000;
    u32* wd2 = reinterpret_cast<u32*>(&self->field_0x1020);
    u32* ws2 = reinterpret_cast<u32*>(&src->field_0x1020);
    u32 pLo = src->pair1004.lo;
    u32 pHi = src->pair1004.hi;
    self->pair1004.hi = pHi;
    self->pair1004.lo = pLo;
    self->field_0x100c = src->field_0x100c;
    self->field_0x1010 = src->field_0x1010;
    self->field_0x1014 = src->field_0x1014;
    self->field_0x1018 = src->field_0x1018;
    self->field_0x101c = src->field_0x101c;
    self->field_0x1020 = src->field_0x1020;
    self->field_0x1021 = src->field_0x1021;
    for (int i = 0; i < 13; i++) {
        wd2[1] = ws2[1];
        wd2[2] = ws2[2];
        wd2 += 2;
        ws2 += 2;
    }
    self->field_0x108c = src->field_0x108c;
    self->field_0x1090 = src->field_0x1090;
    self->field_0x1094 = src->field_0x1094;
    self->pair1098 = src->pair1098;
    self->field_0x10a0 = src->field_0x10a0;
    self->field_0x10a4 = src->field_0x10a4;
    self->pair10A8 = src->pair10A8;
    self->field_0x10b0 = src->field_0x10b0;
    self->pair10B4 = src->pair10B4;
    self->field_0x10bc = src->field_0x10bc;
    return self;
}
#pragma optimize_for_size off

// optimize_for_size keeps the three 8-byte record loops rolled (retail
// mtctr/bdnz shape); the retail _savegpr_29 prologue remains a known
// residual under this flag (MWCC_CASES wall #6/#13).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801F9CB4(CEquipChange* dstP,
                                                    CEquipChange* srcP) {
    // Full copy of the embedded CEquipChange state (retail size 0x2A58):
    // sub-blobs go through the shared copy helpers, scalars are copied
    // load-store interleaved (src is non-const on purpose - same scheme as
    // func_801F9914), and the three 8-byte record runs use the rolled
    // pointer-walk loops (optimize_for_size keeps them mtctr/bdnz).
    CEquipChangeCopyView* self = reinterpret_cast<CEquipChangeCopyView*>(dstP);
    CEquipChangeCopyView* src = reinterpret_cast<CEquipChangeCopyView*>(srcP);
    __ct__UnkClass_8011C974(&self->f04[0], &src->f04[0]);
    __ct__UnkClass_8011C974(&self->f14[0], &src->f14[0]);
    for (int i = 0; i < 8; i++) {
        self->f24[i] = src->f24[i];
    }
    self->f44 = src->f44;
    self->f48 = src->f48;
    self->f4c = src->f4c;
    self->f4d = src->f4d;
    func_8018B0FC(&self->f50[0], &src->f50[0]);
    func_8018B0FC(&self->f68[0], &src->f68[0]);
    func_8018B0FC(&self->f80[0], &src->f80[0]);
    self->f98 = src->f98;
    self->f99 = src->f99;
    // Adjacent word pairs copy as one 2-load/2-reverse-store unit.
    u32 vLo = src->f9a;
    u32 vHi = src->f9e;
    self->f9e = vHi;
    self->f9a = vLo;
    __ct__UnkClass_8011C974(&self->fa8[0], &src->fa8[0]);
    __ct__UnkClass_8011C974(&self->fb8[0], &src->fb8[0]);
    for (int i = 0; i < 27; i++) {
        self->fc8[i] = src->fc8[i];
    }
    self->f134 = src->f134;
    self->f138 = src->f138;
    self->f13c = src->f13c;
    self->f13d = src->f13d;
    self->f13e = src->f13e;
    vLo = src->f140;
    vHi = src->f144;
    self->f144 = vHi;
    self->f140 = vLo;
    vLo = src->f148;
    vHi = src->f14c;
    self->f14c = vHi;
    self->f148 = vLo;
    self->f150 = src->f150;
    self->f152 = src->f152;
    func_8018BE74(&self->f154[0], &src->f154[0]);
    __ct__UnkClass_8011C974(&self->f2b4[0], &src->f2b4[0]);
    __ct__UnkClass_8011C974(&self->f2c4[0], &src->f2c4[0]);
    for (int i = 0; i < 7; i++) {
        self->f2d4[i] = src->f2d4[i];
    }
    for (int i = 0; i < 4; i++) {
        self->f2f0[i] = src->f2f0[i];
    }
    func_801FA220(&self->f2f4[0], &src->f2f4[0]);
    func_801FA220(&self->f30c[0], &src->f30c[0]);
    func_8018B0FC(&self->f324[0], &src->f324[0]);
    __ct__UnkClass_8011C974(&self->f340[0], &src->f340[0]);
    for (int i = 0; i < 5; i++) {
        self->f350[i] = src->f350[i];
    }
    for (int i = 0; i < 4; i++) {
        self->f364[i] = src->f364[i];
    }
    __ct__UnkClass_8011C974(&self->f36c[0], &src->f36c[0]);
    // Tail of the +0x368 sub-record: the source base sits at +0x368 and the
    // copied fields start at +0x14 (dst +0x37c).
    self->f37c[0] = src->f37c[0];
    self->f37c[1] = src->f37c[1];
    self->f37c[2] = src->f37c[2];
    self->f37c[3] = src->f37c[3];
    for (int i = 0; i < 4; i++) {
        self->f38c[i] = src->f38c[i];
    }
    for (int i = 0; i < 5; i++) {
        self->f390[i] = src->f390[i];
    }
    self->f3a4 = src->f3a4;
    u32* wd = reinterpret_cast<u32*>(&self->f3a4);
    u32* ws = reinterpret_cast<u32*>(&src->f3a4);
    for (int i = 0; i < 16; i++) {
        wd[1] = ws[1];
        wd[2] = ws[2];
        wd += 2;
        ws += 2;
    }
    for (int i = 0; i < 3; i++) {
        self->f428[i] = src->f428[i];
    }
    func_8016742C(&self->f42c[0], &src->f42c[0]);
    func_8016742C(&self->f468[0], &src->f468[0]);
    for (int i = 0; i < 3; i++) {
        self->f4a4[i] = src->f4a4[i];
    }
    self->f4a8 = src->f4a8;
    self->f4aa = src->f4aa;
    self->f4ac = src->f4ac;
    self->f4ae = src->f4ae;
    vLo = src->f4b0;
    vHi = src->f4b4;
    self->f4b4 = vHi;
    self->f4b0 = vLo;
    self->f4b8 = src->f4b8;
    self->f4bc = src->f4bc;
    func_8018BE74(&self->f4c0[0], &src->f4c0[0]);
    self->f61c = src->f61c;
    self->f620 = src->f620;
    for (int i = 0; i < 11; i++) {
        self->f622[i] = src->f622[i];
    }
    wd = reinterpret_cast<u32*>(&self->f622[8]);   // base 0x62a
    ws = reinterpret_cast<u32*>(&src->f622[8]);
    for (int i = 0; i < 0x400; i++) {
        wd[1] = ws[1];
        wd[2] = ws[2];
        wd += 2;
        ws += 2;
    }
    self->f262e = src->f262e;
    for (int i = 0; i < 4; i++) {
        self->f2630[i] = src->f2630[i];
    }
    wd = reinterpret_cast<u32*>(&self->f2630[0]);  // base 0x2630
    ws = reinterpret_cast<u32*>(&src->f2630[0]);
    for (int i = 0; i < 4; i++) {
        wd[1] = ws[1];
        wd[2] = ws[2];
        wd += 2;
        ws += 2;
    }
    wd = reinterpret_cast<u32*>(&self->f2650[0]);  // base 0x2650
    ws = reinterpret_cast<u32*>(&src->f2650[0]);
    for (int i = 0; i < 0x80; i++) {
        wd[1] = ws[1];
        wd[2] = ws[2];
        wd += 2;
        ws += 2;
    }
    self->f2a54 = src->f2a54;
}
#pragma optimize_for_size off

extern "C" void func_801FA220(u8* r3, const u8* r4) {
    unsigned int* destWords = (unsigned int*)(r3 + 4);
    const unsigned int* srcWords = (const unsigned int*)(r4 + 4);
    destWords[0] = srcWords[0];
    destWords[1] = srcWords[1];
    destWords[2] = srcWords[2];
    destWords[3] = srcWords[3];
    char* destBytes = (char*)r3;
    const char* srcBytes = (const char*)r4;
    destBytes[0x14] = srcBytes[0x14];
    destBytes[0x15] = srcBytes[0x15];
}

// us-801fbf10 | func_801FA254
// Window teardown: wait for the draw callback, switch the mem allocator off
// the optimal path, unregister the owning scene's render callback, destroy
// the six embedded sub-objects, flag the scene bytes and detach the child
// work thread's view, then clear both owning pointers.
// optimize_for_size merges the r30/r31 saves into stmw (retail shape;
// plain -O4,p emits two stw's - same fix as func_801F9894).
#pragma optimize_for_size on
void func_801FA254(CPartyStateWin* self) {
    CDeviceVI::waitForDrawDone();
    mtl::MemManager::setOptimalAlloc(false);
    func_800453EC(self->mScene);
    CScn* scene = self->mScene;
    if (scene != 0) {
        // The `if (self)` splits mr r4,r30 / beq / addi r4,+0x4 (MWCC idiom
        // for the IScnRender subobject at +0x04, same as CMakeCrystalWin).
        IScnRender* render = reinterpret_cast<IScnRender*>(self);
        if (self != 0) {
            render = reinterpret_cast<IScnRender*>(&self->mVtbl4);
        }
        scene->removeRenderCB(render);
    }
    func_801C40A0(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
    func_801FC0C4(reinterpret_cast<CModelDisp*>(&self->_pad50));
    func_801FD0F4(reinterpret_cast<CPartyState*>(&self->_pad3038));
    func_801FF874(&self->mModelDispEquip);
    func_8020228C(reinterpret_cast<CEquipChange*>(&self->_pad4150));
    func_8022B7F4(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
    CScn* scene2 = self->mScene;
    if (scene2 != 0) {
        // Retail re-reads mScene for the second store (no CSE across the
        // pointer writes - same scheme as CMakeCrystalWin).
        reinterpret_cast<CPartyStateWinScnFlags*>(self->mScene)->field_3E4 = 1;
        reinterpret_cast<CPartyStateWinScnFlags*>(self->mScene)->field_39 = 1;
        self->mScene = 0;
    }
    CWorkThread* thread = self->mWork14;
    if (thread != 0) {
        // Retail loads thread->mWorkID into a saved register BEFORE the
        // getInstance call (the value must survive it), then passes it as the
        // pssDetachView argument. Both the value and the receiver are
        // materialized as locals so MWCC cannot sink the load into the call
        // argument slot.
        WORK_ID wid = thread->mWorkID;
        CTaskGameProcView* tv =
            reinterpret_cast<CTaskGameProcView*>(getInstance__9CTaskGameFv());
        tv->unk6C->pssDetachView(wid);
        self->mWork14->wkSetEvent(CWorkThread::EVT_NONE);
        self->mWork14 = 0;
    }
}
#pragma optimize_for_size off

// us-801fbff4 | func_801FA338
// Party-window frame step: dispatch on the window state byte (0..0x14) and
// run the per-state handler, then update every embedded sub-object (the
// equip-change update is skipped while the equip display is active, state
// 0x12) and refresh the child view's client rect from the shared window
// object's stored s16 pair.
// extern "C" keeps the retail unmangled symbol; the per-state handlers are
// declared above.
// Forward decls for the per-state handlers (defined later in this TU). The
// retail symbols are unmangled (global C-linkage), so the handlers are
// extern "C" (same scheme as func_801FBC7C); noinline keeps the retail
// `bl func_801FAxxx` calls in the dispatch below real branches.
extern "C" __declspec(noinline) void func_801FA59C(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FA614(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FA674(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FA8AC(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FA92C(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FAA10(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FAA60(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB560(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB60C(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB66C(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB6CC(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB72C(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB834(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB8B0(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FB900(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FBBE0(CPartyStateWin* self);
extern "C" __declspec(noinline) void func_801FBC30(CPartyStateWin* self);
extern "C" void func_801C473C(void* self, u32 a);

// When the equip model display (+0x3090) is active, mark the +0x6BE4 state
// byte 7 and run the +0x18 sub-object init.
extern "C" __declspec(noinline) void func_801FBC30(CPartyStateWin* self) {
    if (func_801FF95C(&self->mModelDispEquip) != 0) {
        *(u8*)((u8*)self + 0x6BE4) = 7;
        func_801C473C((u8*)self + 0x18, 1);
    }
}

extern "C" void func_801FA338(CPartyStateWin* self) {
    switch (self->field_6BE4) {
    case 0x0: func_801FA59C(self); break;
    case 0x1: func_801FA614(self); break;
    case 0x2: func_801FA674(self); break;
    case 0x3: func_801FA8AC(self); break;
    case 0x4: func_801FA92C(self); break;
    case 0x5:
        func_801C416C(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
        func_802024CC(reinterpret_cast<CEquipChange*>(&self->_pad4150));
        self->field_6BE4 = 0x6;
        break;
    case 0x6: func_801FAA10(self); break;
    case 0x7: func_801FAA60(self); break;
    case 0x8: func_801FB560(self); break;
    case 0x9: func_801FB60C(self); break;
    case 0xA: func_801FB66C(self); break;
    case 0xB: func_801FB6CC(self); break;
    case 0xC: func_801FB72C(self); break;
    case 0xD:
        if (CSysWin_isActive(reinterpret_cast<CSysWin*>(&self->_pad6BA8)) != 0) {
            self->field_6BE4 = 0xE;
        }
        break;
    case 0xE: func_801FB834(self); break;
    case 0xF:
        if (CSysWin_isActive(reinterpret_cast<CSysWin*>(&self->_pad6BA8)) != 0) {
            self->field_6BE4 = 0x7;
        }
        break;
    case 0x10: func_801FB8B0(self); break;
    case 0x11:
        if (func_801FF95C(&self->mModelDispEquip) != 0) {
            self->field_6BE4 = 0x12;
        }
        break;
    case 0x12: func_801FB900(self); break;
    case 0x13: func_801FBBE0(self); break;
    case 0x14: func_801FBC30(self); break;
    }
    func_801C3FF0(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
    func_801FC060(reinterpret_cast<CModelDisp*>(&self->_pad50));
    func_801FCFF4(reinterpret_cast<CPartyState*>(&self->_pad3038));
    func_801FF82C(&self->mModelDispEquip);
    if (self->field_6BE4 != 0x12) {
        func_80202110(reinterpret_cast<CEquipChange*>(&self->_pad4150));
    }
    func_8022B748(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
    if (self->mWork14 != 0) {
        // The shared window object's stored pair is the rect size; the origin
        // is {0,0}. The quad is fed into setRect through func_801F970C's call
        // result (retail: mr r4, r3) rather than a fresh address computation.
        CPartyStateWinRectSrc* obj =
            reinterpret_cast<CPartyStateWinRectSrc*>(lbl_eu_80663E10);
        CPartyStateS16Pair zero;
        zero.x = 0;
        zero.y = 0;
        CPartyStateS16Quad quad;
        reinterpret_cast<CView*>(self->mWork14)->setRect(
            *reinterpret_cast<ml::CRect16*>(
                func_801F970C(&quad, &zero, &obj->mPair1C8)));
    }
}

u8 func_801FA4EC(CPartyStateWin* self) { return self->field_6BE5; }

// Party-state sub-step gate: once the embedded party state at +0x3038 is
// ready (func_801FD18C), advance its internal step (func_8012FAA8).
extern "C" void func_801FA4F4(CPartyStateWin* self) {
    if (func_801FD18C(reinterpret_cast<CPartyState*>(&self->_pad3038))) {
        func_8012FAA8();
    }
}

// us-801fc1e0 | func_801FA524
// Menu-close gate: returns 1 while the window is armed (state 0xC), the game
// manager is not ready, or the system window is active; otherwise advances the
// equip-change state and returns its result.
int func_801FA524(CPartyStateWin* self) {
    if (self->field_6BE4 == 0xC) {
        return 1;
    }
    if (isPlayerReadyForEvent__Q22cf13CfGameManagerFv(0, 1) == 0) {
        return 1;
    }
    if (CSysWin_getUnk34(reinterpret_cast<CSysWin*>(&self->_pad6BA8)) != 0) {
        return 1;
    }
    return func_80202484(reinterpret_cast<CEquipChange*>(&self->_pad4150));
}

// us-801fc258 | func_801FA59C
// Party-menu open: once the title help and party state settle, update all
// sub-objects, arm the window (state 0x1) and play the open sound.
extern "C" __declspec(noinline) void func_801FA59C(CPartyStateWin* self) {
    if (func_801C4114(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) &&
        func_801FD17C(reinterpret_cast<CPartyState*>(&self->_pad3038))) {
        func_801C412C(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
        func_801FD194(reinterpret_cast<CPartyState*>(&self->_pad3038));
        func_801FC11C(reinterpret_cast<CModelDisp*>(&self->_pad50));
        self->field_6BE4 = 0x1;
        func_801FBC7C(self);
        func_80138078(0x6D);
    }
}

// us-801fc2d0 | func_801FA614
// Menu-open gate: when the title help is idle, the party state is settled and
// the model display is ready, arm the window (state 0x2).
extern "C" __declspec(noinline) void func_801FA614(CPartyStateWin* self) {
    if (isIdle__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) &&
        func_801FD184(reinterpret_cast<CPartyState*>(&self->_pad3038)) &&
        func_801FC114(reinterpret_cast<CModelDisp*>(&self->_pad50))) {
        self->field_6BE4 = 0x2;
    }
}

// us-801fc330 | func_801FA674
// Party-menu pad input step: map the turbo/pressed flags to the A/B/Y
// buttons (classic vs Wii layout) and the four directions, then run the
// party-state sub-step for whichever input is active. A/B act on the
// party-state selection; the directions move the selection cursor; Y opens
// the sort dialog when the party is in the multi-member config. The flag
// decoding is duplicated per controller layout (retail shape: each branch
// reloads turbo/pressed and re-decodes).
// optimize_for_size merges the r30/r31 saves into stmw (retail shape;
// plain -O4,p emits two stw's - same fix as func_801F9894).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801FA674(CPartyStateWin* self) {
    cf::CfPadData* cfPad = cf::CfGameManager::getCfPadData();
    bool up, down, left, right;
    u32 a, b, y;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        up = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_UP | PAD_INPUT_FLAG_UP)) != 0;
        down = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_DOWN | PAD_INPUT_FLAG_DOWN)) != 0;
        left = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_LEFT | PAD_INPUT_FLAG_LEFT)) != 0;
        right = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_RIGHT | PAD_INPUT_FLAG_RIGHT)) != 0;
        a = (cfPad->mPad.mPressedButtonFlags >> PAD_INPUT_CLASSIC_A) & 1;
        b = (cfPad->mPad.mPressedButtonFlags >> PAD_INPUT_CLASSIC_B) & 1;
        y = (cfPad->mPad.mPressedButtonFlags >> PAD_INPUT_CLASSIC_Y) & 1;
    } else {
        up = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_UP | PAD_INPUT_FLAG_UP)) != 0;
        down = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_DOWN | PAD_INPUT_FLAG_DOWN)) != 0;
        left = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_LEFT | PAD_INPUT_FLAG_LEFT)) != 0;
        right = (cfPad->mTurboPressButtonFlags & (PAD_INPUT_FLAG_LSTICK_RIGHT | PAD_INPUT_FLAG_RIGHT)) != 0;
        a = (cfPad->mPad.mPressedButtonFlags >> PAD_INPUT_CORE_A) & 1;
        b = (cfPad->mPad.mPressedButtonFlags >> PAD_INPUT_CORE_B) & 1;
        y = (cfPad->mPad.mPressedButtonFlags >> PAD_INPUT_FS_C) & 1;
    }
    if (a) {
        func_801FD48C(reinterpret_cast<CPartyState*>(&self->_pad3038));
        func_801FBC7C(self);
        if (func_801FD5F4(reinterpret_cast<CPartyState*>(&self->_pad3038)) != 0) {
            func_80139198(0);
            func_801C4198(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
            func_801FD1BC(reinterpret_cast<CPartyState*>(&self->_pad3038));
            func_801FC13C(reinterpret_cast<CModelDisp*>(&self->_pad50));
            self->field_6BE4 = 4;
        }
    } else if (b) {
        if (func_801FD580(reinterpret_cast<CPartyState*>(&self->_pad3038)) != 0) {
            func_801FD594(reinterpret_cast<CPartyState*>(&self->_pad3038));
            func_801FBC7C(self);
        } else if (isPlayerReadyForEvent__Q22cf13CfGameManagerFv(0, 1)) {
            func_801C414C(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
            func_801FD1BC(reinterpret_cast<CPartyState*>(&self->_pad3038));
            func_801FC13C(reinterpret_cast<CModelDisp*>(&self->_pad50));
            self->field_6BE4 = 3;
        }
    } else if (up) {
        func_801FD220(reinterpret_cast<CPartyState*>(&self->_pad3038));
        func_801FBC7C(self);
    } else if (down) {
        func_801FD290(reinterpret_cast<CPartyState*>(&self->_pad3038));
        func_801FBC7C(self);
    } else if (left) {
        func_801FD304(reinterpret_cast<CPartyState*>(&self->_pad3038));
        func_801FBC7C(self);
    } else if (right) {
        func_801FD3D4(reinterpret_cast<CPartyState*>(&self->_pad3038));
        func_801FBC7C(self);
    } else if (y) {
        // Sort dialog opens only in the non-default party config with more
        // than one member. Same cfgIsZero idiom as func_801FBC7C: the
        // == 0 normalization's srwi. flag feeds the branch directly.
        u32 cfgIsZero = (func_8009CF8C(0x3358) == 0);
        if (cfgIsZero || code80135FDC_getByte_64077() <= 1) {
        } else {
            func_801FD604(reinterpret_cast<CPartyState*>(&self->_pad3038));
            func_801FBC7C(self);
        }
    }
}
#pragma optimize_for_size off

// Party menu per-frame settle: once the title help, party state and model
// display are all idle, run the party-state sub-step (with the announce
// sound when it advances) and latch the 0x6BE5 flag.
extern "C" __declspec(noinline) void func_801FA8AC(CPartyStateWin* self) {
    if (isIdle__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) &&
        func_801FD184(reinterpret_cast<CPartyState*>(&self->_pad3038)) &&
        func_801FC114(reinterpret_cast<CModelDisp*>(&self->_pad50))) {
        if (self->field_6BE5 == 0) {
            if (func_801FD18C(reinterpret_cast<CPartyState*>(&self->_pad3038))) {
                func_8012FAA8();
            }
            self->field_6BE5 = 1;
        }
    }
}

// us-801fc5e8 | func_801FA92C
// Window refresh gate: once the title help, party state, model display,
// equip change and system window all settle, rebuild the window (reload the
// string-pool name, stamp it into the title help, set the title-help byte,
// refresh the equip display slot and the equip-change category) and arm
// state 0x5.
extern "C" __declspec(noinline) void func_801FA92C(CPartyStateWin* self) {
    if (isIdle__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) &&
        func_801FD184(reinterpret_cast<CPartyState*>(&self->_pad3038)) &&
        func_801FC114(reinterpret_cast<CModelDisp*>(&self->_pad50)) &&
        func_80202364(reinterpret_cast<CEquipChange*>(&self->_pad4150)) &&
        CSysWin_isReady(reinterpret_cast<CSysWin*>(&self->_pad6BA8))) {
        func_801FC0C4(reinterpret_cast<CModelDisp*>(&self->_pad50));
        char* name =
            func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x6b);
        func_801C41C0(reinterpret_cast<CTitleAHelp*>(&self->_pad18), name);
        func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->_pad18), 0x15);
        func_8020147C(&self->mModelDispEquip,
                      (u8)func_801FD5FC(reinterpret_cast<CPartyState*>(&self->_pad3038)));
        // The slot value goes through a local so the mask lands after the
        // receiver setup (retail: mr r0,r3 / addi r3 / clrlwi r4,r0,24 -
        // same shape as func_801FB66C).
        u32 equipSlot = func_801FF9A0(&self->mModelDispEquip);
        func_80202EB4(reinterpret_cast<CEquipChange*>(&self->_pad4150),
                      (u8)equipSlot);
        self->field_6BE4 = 0x5;
    }
}

// Gate: when the help sub-object is idle and the +0x4150 window accepts
// input, raise the +0x6BE4 state byte to 7.
extern "C" __declspec(noinline) void func_801FAA10(CPartyStateWin* self) {
    if (isIdle__11CTitleAHelpFv((CTitleAHelp*)((u8*)self + 0x18)) && func_802023C0((CEquipChange*)((u8*)self + 0x4150)))
        *((u8*)self + 0x6BE4) = 7;
}

// us-801fc71c | func_801FAA60
// Equip-change input step: increments the window timer field_6BE8 (clamped at
// lbl_eu_806681E0), then while the equip-change sub-object is active,
// dispatches on the held/pressed/turbo pad flags. Two mirror chains (classic
// vs Wii controller layout, selected by isClassicController): each chain tests a
// list of flag combinations and either opens a system window (string ids
// 0x84/0x85/0x8A via func_8022B9B4/8022BFC8/8022B8B8 + state 0xD), arms a
// display state (0x8/0xA/0xB/0xC/0x10), or plays a sound (func_80138078).
// The shared tail refreshes the title help from the sys-win state or the
// equip-change result.
extern "C" __declspec(noinline) void func_801FAA60(CPartyStateWin* self) {
    self->field_6BE8 += lbl_eu_806681DC;
    if (self->field_6BE8 > lbl_eu_806681E0) {
        self->field_6BE8 = lbl_eu_806681E0;
    }
    if (func_802023C0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
        return;
    }
    cf::CfPadData* cfPad = cf::CfGameManager::getCfPadData();
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // -- classic-controller chain --
        func_8020397C(reinterpret_cast<CEquipChange*>(&self->_pad4150), 0);
        if ((cfPad->mTurboPressButtonFlags & 0x02000000) != 0 &&
            code80135FDC_getByte_64077() > 1 &&
            func_802023D0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0 &&
            func_80202424(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0 &&
                (u8)func_80203138(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 3) {
                func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                              func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x85),
                              0);
                func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                self->field_6BE4 = 0xD;
                goto tail;
            }
            func_801FF98C(&self->mModelDispEquip);
            self->field_6BE4 = 0xB;
            func_80138078(0xA);
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x04000000) != 0 &&
            code80135FDC_getByte_64077() > 1 &&
            func_802023D0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0 &&
            func_80202424(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0 &&
                (u8)func_80203138(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 3) {
                func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                              func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x85),
                              0);
                func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                self->field_6BE4 = 0xD;
                goto tail;
            }
            func_801FF98C(&self->mModelDispEquip);
            self->field_6BE4 = 0xA;
            func_80138078(0xA);
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x00200000) != 0) {
            func_80202CCC(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            if (func_8020392C(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_801FF98C(&self->mModelDispEquip);
                self->field_6BE4 = 0xC;
                goto tail;
            }
            if ((u8)func_80203994(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 2) {
                func_80138078(0x5);
                func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                              func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x84),
                              0);
                func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                self->field_6BE4 = 0xD;
                goto tail;
            }
            if (func_80203C9C(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                u16 v = func_80203A98(reinterpret_cast<CEquipChange*>(&self->_pad4150), 1);
                if (v == 0x296 || v == 0x2A0) {
                    func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                  func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x8A),
                                  0);
                    func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                    func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                    self->field_6BE4 = 0xD;
                    goto tail;
                }
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x00400000) != 0) {
            if (func_802023C0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
                return;
            }
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_80202C4C(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                goto tail;
            }
            func_801C4198(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
            func_80202578(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            func_801FF98C(&self->mModelDispEquip);
            self->field_6BE4 = 0x8;
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x8004) != 0) {
            func_80202644(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x00010008) != 0) {
            func_80202790(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x2001) != 0) {
            func_802028E4(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x4002) != 0) {
            func_80202A70(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x01000000) != 0) {
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_80202BFC(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                goto tail;
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x10000000) != 0) {
            if (func_802023D0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
                u8 v = (u8)func_80203210(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                if (v == 1) {
                    func_801FF98C(&self->mModelDispEquip);
                    self->field_6BE4 = 0xC;
                    goto tail;
                }
                if (v == 2) {
                    func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                  func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x84),
                                  0);
                    func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                    func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                    self->field_6BE4 = 0xD;
                    goto tail;
                }
                if (v != 3) {
                    func_80138078(0x5);
                    u16 w = func_80203A98(reinterpret_cast<CEquipChange*>(&self->_pad4150), 0);
                    if (w == 0x296 || w == 0x2A0) {
                        func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                      func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x8A),
                                      0);
                        func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                        func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                        self->field_6BE4 = 0xD;
                        goto tail;
                    }
                }
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x00000200) != 0) {
            if (func_8020247C(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0 &&
                func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_80203984(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                goto tail;
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x00000400) != 0) {
            if (func_802023D0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0 &&
                func_801FF964(&self->mModelDispEquip) != 0 &&
                func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
                self->field_6BE4 = 0x10;
                func_801FF98C(&self->mModelDispEquip);
                func_80138078(0x2);
                goto tail;
            }
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_8020398C(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                goto tail;
            }
        }
        goto tail;
    } else {
        // -- Wii-layout chain --
        func_8020397C(reinterpret_cast<CEquipChange*>(&self->_pad4150), 0);
        if ((cfPad->mPad.mHeldButtonFlags & 0x00001000) != 0 &&
            code80135FDC_getByte_64077() > 1 &&
            func_802023D0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0 &&
            func_80202424(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
            if (self->field_6BE8 > lbl_eu_806681DC) {
                func_80138078(0x2);
            }
            self->field_6BE8 = lbl_eu_806681D8;
            func_8020397C(reinterpret_cast<CEquipChange*>(&self->_pad4150), 1);
            if ((cfPad->mTurboPressButtonFlags & 0x2001) != 0) {
                if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0 &&
                    (u8)func_80203138(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 3) {
                    func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                  func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x85),
                                  0);
                    func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                    func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                    self->field_6BE4 = 0xD;
                    goto tail;
                }
                func_801FF98C(&self->mModelDispEquip);
                self->field_6BE4 = 0xB;
                func_80138078(0xA);
                goto tail;
            }
            if ((cfPad->mTurboPressButtonFlags & 0x4002) != 0) {
                if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0 &&
                    (u8)func_80203138(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 3) {
                    func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                  func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x85),
                                  0);
                    func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                    func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                    self->field_6BE4 = 0xD;
                    goto tail;
                }
                func_801FF98C(&self->mModelDispEquip);
                self->field_6BE4 = 0xA;
                func_80138078(0xA);
                goto tail;
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x10) != 0) {
            func_80202CCC(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            if (func_8020392C(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_801FF98C(&self->mModelDispEquip);
                self->field_6BE4 = 0xC;
                goto tail;
            }
            if ((u8)func_80203994(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 2) {
                func_80138078(0x5);
                func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                              func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x84),
                              0);
                func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                self->field_6BE4 = 0xD;
                goto tail;
            }
            if (func_80203C9C(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                u16 v = func_80203A98(reinterpret_cast<CEquipChange*>(&self->_pad4150), 1);
                if (v == 0x296 || v == 0x2A0) {
                    func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                  func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x8A),
                                  0);
                    func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                    func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                    self->field_6BE4 = 0xD;
                    goto tail;
                }
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x20) != 0) {
            if (func_802023C0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
                return;
            }
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_80202C4C(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                goto tail;
            }
            func_801C4198(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
            func_80202578(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            func_801FF98C(&self->mModelDispEquip);
            self->field_6BE4 = 0x8;
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x8004) != 0) {
            func_80202644(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x00010008) != 0) {
            func_80202790(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x2001) != 0) {
            func_802028E4(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mTurboPressButtonFlags & 0x4002) != 0) {
            func_80202A70(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x800) != 0) {
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_80202BFC(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                goto tail;
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x80) != 0) {
            if (func_802023D0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
                u8 v = (u8)func_80203210(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                if (v == 1) {
                    func_801FF98C(&self->mModelDispEquip);
                    self->field_6BE4 = 0xC;
                    goto tail;
                }
                if (v == 2) {
                    func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                  func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x84),
                                  0);
                    func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                    func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                    self->field_6BE4 = 0xD;
                    goto tail;
                }
                if (v != 3) {
                    func_80138078(0x5);
                    u16 w = func_80203A98(reinterpret_cast<CEquipChange*>(&self->_pad4150), 0);
                    if (w == 0x296 || w == 0x2A0) {
                        func_8022B9B4(reinterpret_cast<CSysWin*>(&self->_pad6BA8),
                                      func_80136190(&lbl_eu_80507C94[0x2d], &lbl_eu_80507C94[0x28], 0x8A),
                                      0);
                        func_8022BFC8(reinterpret_cast<CSysWin*>(&self->_pad6BA8), 1);
                        func_8022B8B8(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
                        self->field_6BE4 = 0xD;
                        goto tail;
                    }
                }
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x00000200) != 0) {
            if (func_8020247C(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0 &&
                func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_80203984(reinterpret_cast<CEquipChange*>(&self->_pad4150));
                goto tail;
            }
            goto tail;
        }
        if ((cfPad->mPad.mPressedButtonFlags & 0x40) != 0) {
            if (func_802023D0(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0 &&
                func_801FF964(&self->mModelDispEquip) != 0 &&
                func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) == 0) {
                self->field_6BE4 = 0x10;
                func_801FF98C(&self->mModelDispEquip);
                func_80138078(0x2);
                goto tail;
            }
            if (func_802023C8(reinterpret_cast<CEquipChange*>(&self->_pad4150)) != 0) {
                func_8020398C(reinterpret_cast<CEquipChange*>(&self->_pad4150));
            }
        }
    }
tail:
    if (CSysWin_getUnk34(reinterpret_cast<CSysWin*>(&self->_pad6BA8)) != 0) {
        func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->_pad18), 0);
    } else {
        // The equip-change result goes through a u32 local so the mask lands after
        // the receiver setup (retail: mr r0,r3 / addi r3 / clrlwi r4,r0,24 -
        // same shape as func_801FA92C / func_801FB66C).
        u32 v = func_802039F4(reinterpret_cast<CEquipChange*>(&self->_pad4150));
        func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->_pad18), (u8)v);
    }
}

// us-801fd21c | func_801FB560
// Party-window refresh: once the title help is idle and the equip display /
// equip-change states are settled, rebuild the window (reload the string
// pool names, refresh all sub-objects) and arm state 0x9.
extern "C" __declspec(noinline) void func_801FB560(CPartyStateWin* self) {
    if (isIdle__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) &&
        func_801FF95C(&self->mModelDispEquip) &&
        func_802023C0(reinterpret_cast<CEquipChange*>(&self->_pad4150))) {
        func_801FF874(&self->mModelDispEquip);
        char* name = func_80136190(&lbl_eu_80507C94[0x1e], &lbl_eu_80507C94[0x28], 1);
        func_801C41C0(reinterpret_cast<CTitleAHelp*>(&self->_pad18), name);
        func_801FBC7C(self);
        func_801C416C(reinterpret_cast<CTitleAHelp*>(&self->_pad18));
        func_801FD194(reinterpret_cast<CPartyState*>(&self->_pad3038));
        func_801FC11C(reinterpret_cast<CModelDisp*>(&self->_pad50));
        self->field_6BE4 = 0x9;
    }
}

// us-801fd2c8 | func_801FB60C (same body as func_801FA614)
extern "C" __declspec(noinline) void func_801FB60C(CPartyStateWin* self) {
    if (isIdle__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) &&
        func_801FD184(reinterpret_cast<CPartyState*>(&self->_pad3038)) &&
        func_801FC114(reinterpret_cast<CModelDisp*>(&self->_pad50))) {
        self->field_6BE4 = 0x2;
    }
}

// us-801fd328 | func_801FB66C
// Equip display enter: once state20 clears, restart the display, hand the
// current equip-slot category to CEquipChange and arm the window (state 0x7).
extern "C" __declspec(noinline) void func_801FB66C(CPartyStateWin* self) {
    if (func_801FF95C(&self->mModelDispEquip)) {
        func_802014C0(&self->mModelDispEquip);
        u32 equipSlot = func_801FF9A0(&self->mModelDispEquip);
        func_80202EB4(reinterpret_cast<CEquipChange*>(&self->_pad4150),
                      (u8)equipSlot);
        self->field_6BE4 = 0x7;
    }
}

// us-801fd388 | func_801FB6CC
// Equip display enter (restart variant): once state20 clears, restart the
// display, hand the current equip-slot category to CEquipChange and arm the
// window (state 0x7).
extern "C" __declspec(noinline) void func_801FB6CC(CPartyStateWin* self) {
    if (func_801FF95C(&self->mModelDispEquip)) {
        func_8020151C(&self->mModelDispEquip);
        u32 equipSlot = func_801FF9A0(&self->mModelDispEquip);
        func_80202EB4(reinterpret_cast<CEquipChange*>(&self->_pad4150),
                      (u8)equipSlot);
        self->field_6BE4 = 0x7;
    }
}

// us-801fd3e8 | func_801FB72C
// Equip display refresh: once the display settles, reset its slot, hand the
// current slot to CEquipChange, and when the party has fewer than 3 members
// filter the party list by the slot's name-table entry and detach the
// matching member's object. Arms state 0x7.
// optimize_for_size merges the r29-r31 saves into stmw (retail shape;
// plain -O4,p emits three stw's - same fix as func_801F9894).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801FB72C(CPartyStateWin* self) {
    if (func_801FF95C(&self->mModelDispEquip)) {
        func_8020147C(&self->mModelDispEquip, -1);
        // The slot value goes through a local so the mask lands after the
        // receiver setup (retail: mr r0,r3 / addi r3 / clrlwi r4,r0,24).
        u32 v = func_801FF9A0(&self->mModelDispEquip);
        func_80202EB4(reinterpret_cast<CEquipChange*>(&self->_pad4150), (u8)v);
        if ((u8)func_801FF9A0(&self->mModelDispEquip) < 3) {
            CPartyStateWinHolder holder;
            u32 names[3];
            // Post-increment loads fold the base materialization into the
            // first access (retail: lis + lwzu + +4/+8 displacements) - same
            // trick as btm_sco_init in MWCC_CASES.
            const u32* src = lbl_eu_80507C78;
            names[0] = *src++;
            names[1] = *src++;
            names[2] = *src++;
            func_80043D90(&holder);
            func_800F4A98(func_80043F18(&holder),
                          names[(u8)func_801FF9A0(&self->mModelDispEquip)], 0);
            if (reinterpret_cast<CPartyStateWinList*>(func_80043F18(&holder))
                    ->field_0x620 >= 1) {
                CPartyStateWinListSlot* slot = reinterpret_cast<CPartyStateWinListSlot*>(
                    func_800F6EC0(func_80043F18(&holder), 0));
                if (slot->field_0x4 != 0) {
                    func_800BFDE0(
                        func_800BFC68__FPQ22cf12CfObjectMove(slot->field_0x4),
                        0);
                }
            }
            __dt__80043E88(&holder, -1);
        }
        self->field_6BE4 = 0x7;
    }
}
#pragma optimize_for_size off

// us-801fd4f0 | func_801FB834
// Shortcut-open: when the mode-appropriate button combo is pressed (classic
// mode vs not), arm the window (state 0xF) and open the system window.
extern "C" __declspec(noinline) void func_801FB834(CPartyStateWin* self);

// (subic/subfe setnz + stmw are optimize_for_size shapes - MWCC_CASES)
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801FB834(CPartyStateWin* self) {
    const CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 cond;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        cond = (pad->mPressedButtonFlags & 0x00600000) != 0;
    } else {
        cond = (pad->mPressedButtonFlags & 0x00000030) != 0;
    }
    if (cond != 0) {
        self->field_6BE4 = 0xF;
        func_8022B8E4(reinterpret_cast<CSysWin*>(&self->_pad6BA8));
    }
}
#pragma optimize_for_size off

// us-801fd56c | func_801FB8B0
// Equip display exit: once state20 clears, arm the window (state 0x11),
// restart the display and repaint it.
extern "C" __declspec(noinline) void func_801FB8B0(CPartyStateWin* self) {
    if (func_801FF95C(&self->mModelDispEquip)) {
        self->field_6BE4 = 0x11;
        func_801FF96C(&self->mModelDispEquip);
        func_80201900(&self->mModelDispEquip);
    }
}

extern "C" __declspec(noinline) void func_801FB900(CPartyStateWin* self) {
    // Equip-display input step (state 0x12): decode the mode-appropriate
    // held/pressed flag set, then walk a fixed dispatch chain over the equip
    // display. The flag decoding is duplicated per controller layout (retail
    // shape: each branch reloads both flag words and re-decodes).
    const CPad* pad = cf::CfGameManager::getCurrentPad();
    int up, down, left, right, y, aCond;
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        u32 pressed = pad->mPressedButtonFlags;
        u32 held = pad->mHeldButtonFlags;
        aCond = (pressed & 0x00400400) != 0;
        y = (pressed >> 9) & 1;
        up = (held >> 25) & 1;
        down = (held >> 28) & 1;
        left = (held >> 1) & 1;
        right = held & 1;
        int c4 = (held >> 17) & 1;
        int c5 = (held >> 18) & 1;
        int c6 = (held >> 2) & 1;
        int c7 = (held >> 3) & 1;
        if (down) {
            func_80201900(&self->mModelDispEquip);
        } else if (left) {
            func_80201740(&self->mModelDispEquip);
        } else if (right) {
            func_802017A4(&self->mModelDispEquip);
        } else if (c4) {
            func_802015D4(&self->mModelDispEquip);
        } else if (c5) {
            func_80201570(&self->mModelDispEquip);
        } else if (c6) {
            func_80201638(&self->mModelDispEquip);
        } else if (c7) {
            func_802016BC(&self->mModelDispEquip);
        } else if (y) {
            goto helpBlock;
        } else if (aCond) {
            goto armBlock;
        }
        func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->_pad18), 0x2f);
        return;
    }
    {
        u32 pressed = pad->mPressedButtonFlags;
        u32 held = pad->mHeldButtonFlags;
        aCond = (pressed & 0x00600000) != 0;
        y = (pressed >> 9) & 1;
        int wUp = (held >> 11) & 1;
        int wDown = (held >> 12) & 1;
        int c3 = (held >> 4) & 1;
        int c2 = (held >> 5) & 1;
        int c4b = held & 1;
        int c5b = (held >> 1) & 1;
        int c6b = (held >> 2) & 1;
        int c7b = (held >> 3) & 1;
        up = wUp;
        down = wDown;
        left = c2;
        right = c3;
        if (wUp) {
            func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->_pad18), 0x2d);
            if (down) {
                func_80201900(&self->mModelDispEquip);
            } else if (left) {
                func_80201740(&self->mModelDispEquip);
            } else if (right) {
                func_802017A4(&self->mModelDispEquip);
            } else if (c4b) {
                func_802015D4(&self->mModelDispEquip);
            } else if (c5b) {
                func_80201570(&self->mModelDispEquip);
            }
            return;
        }
        if (c6b) {
            func_802016BC(&self->mModelDispEquip);
        } else if (c7b) {
            func_80201638(&self->mModelDispEquip);
        } else if (y) {
            goto helpBlock;
        } else if (aCond) {
            goto armBlock;
        }
        func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->_pad18), 0x2c);
        return;
    }
helpBlock:
    // Refresh the title help from the idle state and play the matching cue.
    func_801C473C(reinterpret_cast<CTitleAHelp*>(&self->_pad18),
                  (u32)(func_801C411C(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) == 0));
    if (func_801C411C(reinterpret_cast<CTitleAHelp*>(&self->_pad18)) != 0) {
        func_80138078(0xd);
    } else {
        func_80138078(0xe);
    }
    return;
armBlock:
    self->field_6BE4 = 0x13;
    func_801FF98C(&self->mModelDispEquip);
    func_80138078(0x2);
}

// us-801fd89c | func_801FBBE0
// Menu-open for the equip window: once state20 clears, arm the window
// (state 0x14), restart the display and repaint it.
extern "C" __declspec(noinline) void func_801FBBE0(CPartyStateWin* self) {
    if (func_801FF95C(&self->mModelDispEquip)) {
        self->field_6BE4 = 0x14;
        func_801FF96C(&self->mModelDispEquip);
        func_80201808(&self->mModelDispEquip);
    }
}

// Window-row styling for the party state: pick one of the four bytes of the
// lbl_eu_806681E4 window-state word by the party-state selection value, and
// apply it to the title help. In the default (3+ member / normal config)
// cases the selection is forced to 3.
// optimize_for_size merges the r30/r31 saves into stmw (retail shape).
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801FBC7C(CPartyStateWin* self) {
    CPartyStateWinWord local;
    local.word = lbl_eu_806681E4;
    u8 val = func_801FD5C4(reinterpret_cast<CPartyState*>(&self->_pad3038));
    u32 cfgIsZero = (func_8009CF8C(0x3358) == 0);
    if (cfgIsZero || code80135FDC_getByte_64077() <= 1) {
        if (val == 0) {
            val = 3;
        }
    }
    func_801C41E8(reinterpret_cast<CTitleAHelp*>(&self->_pad18),
                  local.bytes[val]);
}
#pragma optimize_for_size off

// us-801faf94 | ctor (retail symbol __ct__14CPartyStateWinFUlUl)
// Constructs the composite vtables, stores the two ctor args, then builds
// the six embedded sub-objects in ascending address order and finishes with
// the byte flags and the field_6BE8 float.
CPartyStateWin::CPartyStateWin(u32 arg1, u32 arg2) {
    mArg1 = arg1;
    mArg2 = arg2;
    u8* vtbl = lbl_eu_805352DC;
    mVtbl0 = (u32)vtbl;
    mVtbl4 = (u32)(vtbl + 0x88);
    mScene = 0;
    mWork14 = 0;
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(&_pad18), 0, 0);
    __ct__CModelDisp(reinterpret_cast<CModelDisp*>(&_pad50), 0);
    __ct__CPartyState(reinterpret_cast<CPartyState*>(&_pad3038));
    __ct__CModelDispEquip(&mModelDispEquip, 0, 0);
    __ct__CEquipChange(reinterpret_cast<CEquipChange*>(&_pad4150));
    __ct__CSysWin(reinterpret_cast<CSysWin*>(&_pad6BA8), 0);
    field_6BE4 = 0;
    field_6BE5 = 0;
    field_6BE8 = lbl_eu_806681D8;
}

// noinline keeps func_801FBDB8's forwarder a real tail branch (the retail
// cbRenderBefore is a large out-of-line function, 0xBC bytes).
__declspec(noinline) void CPartyStateWin::cbRenderBefore() {
    getInstance__9CTaskGameFv();
    // Gate: when the task game is not ready or the global mode bit (0x200000)
    // is set, skip the render. The if-&&-goto body / goto end / end: return
    // chain (exit label before the body label) keeps the body off the
    // fallthrough so MWCC emits retail's branch-over-branch: `bne end` for
    // the first disjunct, `beq body; b end` for the second (same scheme as
    // CMenuKizunagram::Move).
    if (isFlag01Set__9CTaskGameFv() == 0 &&
        (lbl_eu_80663E28 & 0x200000) == 0) {
        goto body;
    }
    goto end;
end:
    return;
body:
    if (func_8013BE50() == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI pre-mangled ct/dt calls
    // to match the retail direct calls (a C++ local would virtual-dispatch
    // its scope-exit destructor and bloat the body).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801FD0A0(reinterpret_cast<CPartyState*>(&_pad3038),
                  (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    if (field_6BE4 < 0x10) {
        func_802021E4(reinterpret_cast<CEquipChange*>(&_pad4150),
                      (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    }
    func_8022B7C8(reinterpret_cast<CSysWin*>(&_pad6BA8),
                  (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(reinterpret_cast<CTitleAHelp*>(&_pad18),
                  (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv((nw4r::lyt::DrawInfo*)&drawInfo[0], -1);
}

// Destroys the embedded sub-objects in reverse construction order (CSysWin at
// the highest offset down to CTitleAHelp). MWCC supplies the null check and
// the delete-this guard (r4 flag) from the dtor shape itself; each embedded
// dtor is invoked with the -1 "delete" flag like the retail calls.
// optimize_for_size merges the r30/r31 saves into stmw (retail shape);
// dont_inline keeps func_801FBDC0's forwarder a real tail branch (the retail
// dtor is out-of-line).
#pragma push
#pragma optimize_for_size on
#pragma dont_inline on
CPartyStateWin::~CPartyStateWin() {
    __dt__7CSysWinFv(reinterpret_cast<CSysWin*>(&_pad6BA8), -1);
    __dt__12CEquipChangeFv(reinterpret_cast<CEquipChange*>(&_pad4150), -1);
    __dt__15CModelDispEquipFv(&mModelDispEquip, -1);
    __dt__11CPartyStateFv(reinterpret_cast<CPartyState*>(&_pad3038), -1);
    __dt__10CModelDispFv(reinterpret_cast<CModelDisp*>(&_pad50), -1);
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(&_pad18), -1);
}
#pragma pop

void func_801FBDB8(void* self) { reinterpret_cast<CPartyStateWin*>((char*)self - 0x4)->cbRenderBefore(); }

// r4-passthrough thunk for the +4 interface: call the dtor through a 1-arg
// function-pointer cast so the incoming delete flag survives untouched
// (retail: addi r3,r3,-4; b __dt__14CPartyStateWinFv).
void func_801FBDC0(void* self) {
    ((CPartyStateWin* (*)(CPartyStateWin*))__dt__14CPartyStateWinFv)(
        reinterpret_cast<CPartyStateWin*>((char*)self - 0x4));
}

// Factory for the party-state window. Builds the child work-thread view,
// sizes the client rect from the shared window object, allocates the scene,
// then constructs each embedded sub-object through a stack temp + copy
// helper + temp-dtor sequence.
extern "C" void func_801F941C(CPartyStateWin* self, u32 arg1, u32 arg2) {
    // Locals declared in retail stack-frame order (displacements are encoded
    // in the instructions): mem@0x08, quad@0x10, vec4 tmp@0x18, CSysWin
    // temp@0x28, CTitleAHelp temp@0x68, gauge record@0xa0, CPartyState
    // temp@0xc8, CEquipChange temp@0x120, CModelDispEquip temp@0x2b78,
    // CModelDisp temp@0x3c38.
    CPartyStateWinMem mem;
    CPartyStateS16Quad quad;
    func_800407C8_tmp quadTmp;
    u32 sysWinTmp[0x10];
    u32 titleTmp[0x18];
    PartyGaugeRecord gauge;
    u32 partyStateTmp[0x16];
    u32 equipChangeTmp[0xA96];
    u32 equipViewTmp[0x430];
    u32 modelDispTmp[0xBFE];

    CTaskGame* game = getInstance__9CTaskGameFv();
    CWorkThread* work =
        reinterpret_cast<CWorkThread*>(func_801F968C(reinterpret_cast<u8*>(game)));
    CProc* proc = reinterpret_cast<CProc*>(func_801F9684(reinterpret_cast<u8*>(game)));
    self->mWork14 =
        reinterpret_cast<CWorkThread*>(proc->pssCreateView(lbl_eu_80507C94, work, 0));

    func_801F969C(&quad, reinterpret_cast<CPartyStateWinRectSrc*>(func_801F9694()));
    reinterpret_cast<CView*>(self->mWork14)->setRect(
        *reinterpret_cast<ml::CRect16*>(&quad));

    f32 c = lbl_eu_806681D8;
    func_801F9730(self, reinterpret_cast<const u32*>(
                            func_800407C8(&quadTmp, c, c, c, c)));
    func_801F9754(reinterpret_cast<CPartyStateWinRing*>(self->mWork14), 0);

    func_801F981C(&mem);
    func_801F9864(&gauge, arg2, lbl_eu_806681D8, 0x20, 0x20, 0x10);
    mtl::ALLOC_HANDLE handle = func_801F9894(&mem, 0x09000000);

    CScn* scn = create__8CScnNw4rFv(CTaskManager::GetRootProcScn(),
                                    &lbl_eu_80507C94[0xf], 0x09000000,
                                    handle, &gauge);
    self->mScene = scn;
    func_80496118(scn, self->mWork14, 0);
    func_800452EC(scn);
    func_80492E08(getField5C(scn));
    mtl::MemManager::setOptimalAlloc(true);

    char* titleName =
        func_80136190(&lbl_eu_80507C94[0x1e], &lbl_eu_80507C94[0x28], 1);
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(titleTmp), titleName, 0x10);
    func_801BE16C(reinterpret_cast<CTitleAHelp*>(&self->_pad18),
                  reinterpret_cast<CTitleAHelp*>(titleTmp));
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(titleTmp), -1);
    CTitleAHelp_load(reinterpret_cast<CTitleAHelp*>(&self->_pad18));

    __ct__CModelDisp(reinterpret_cast<CModelDisp*>(modelDispTmp), (u32)scn);
    func_801F9914(reinterpret_cast<CPartyStateWinCopy*>(&self->_pad50),
                  reinterpret_cast<CPartyStateWinCopy*>(modelDispTmp));
    __dt__10CModelDispFv(reinterpret_cast<CModelDisp*>(modelDispTmp), -1);
    func_801FBFD8(reinterpret_cast<CModelDisp*>(&self->_pad50));

    __ct__CPartyState(reinterpret_cast<CPartyState*>(partyStateTmp));
    func_801F9A48(reinterpret_cast<CPartyStateWinBlob58*>(&self->_pad3038),
                  reinterpret_cast<CPartyStateWinBlob58*>(partyStateTmp));
    __dt__11CPartyStateFv(reinterpret_cast<CPartyState*>(partyStateTmp), -1);
    func_801FCF5C(reinterpret_cast<CPartyState*>(&self->_pad3038));

    __ct__CModelDispEquip(reinterpret_cast<CModelDispEquipView*>(equipViewTmp),
                          (u32)scn, 0);
    func_801F9B18(reinterpret_cast<CPartyStateWinBlob10C0*>(&self->mModelDispEquip),
                  reinterpret_cast<CPartyStateWinBlob10C0*>(equipViewTmp));
    __dt__15CModelDispEquipFv(
        reinterpret_cast<CModelDispEquipView*>(equipViewTmp), -1);
    func_801FF7B0(&self->mModelDispEquip);

    __ct__CEquipChange(reinterpret_cast<CEquipChange*>(equipChangeTmp));
    func_801F9CB4(reinterpret_cast<CEquipChange*>(&self->_pad4150),
                  reinterpret_cast<CEquipChange*>(equipChangeTmp));
    __dt__12CEquipChangeFv(reinterpret_cast<CEquipChange*>(equipChangeTmp), -1);
    func_80202090(reinterpret_cast<CEquipChange*>(&self->_pad4150));

    __ct__CSysWin(reinterpret_cast<CSysWin*>(sysWinTmp), 0);
    func_8016742C(&self->_pad6BA8[0], reinterpret_cast<const u8*>(sysWinTmp));
    __dt__7CSysWinFv(reinterpret_cast<CSysWin*>(sysWinTmp), -1);

    // Retail dispatches vtable slot +0x88 on the embedded CSysWin.
    CSysWinSlot88* vt88 =
        *reinterpret_cast<CSysWinSlot88**>(&self->_pad6BA8[0]);
    vt88->slot88(reinterpret_cast<CSysWin*>(&self->_pad6BA8[0]));

    // The IScnRender sub-object sits at +0x04; MWCC's null-guarded add-of-4
    // idiom (same as func_801FA254).
    IScnRender* render = reinterpret_cast<IScnRender*>(self);
    if (self != 0) {
        render = reinterpret_cast<IScnRender*>(&self->mVtbl4);
    }
    scn->addRenderCB(render, 0xd, 0);
}
// us-801fb358 | func_801F969C
// Content rect for a window row: offset the embedded frame rect by the frame's
// content origin, then copy the offset pair + the stored pair out as a 4x s16
// quad.
// Cached frame reference lets MWCC hoist the second content load above the
// first store (retail interleave).
// noinline keeps the factory call site in func_801F941C a real branch.
extern "C" __declspec(noinline) void func_801F969C(CPartyStateS16Quad* dst, CPartyStateWinRectSrc* obj) {
    ml::CRect16 local;
    getFrame2ViewOffset(local, &obj->mFrame1DC);
    // Cached frame reference: lets MWCC hoist the second content load above
    // the first store (retail interleave).
    const CViewFrame& frame = obj->mFrame1DC;
    // Declared in reverse pair order so MWCC emits the y-chain first with
    // the retail load schedule (cy, py, cx, px).
    s16 cx = frame.mContentX;
    s16 px = local.mPos.x;
    s16 cy = frame.mContentY;
    s16 py = local.mPos.y;
    local.mSize.x = cx + px;
    local.mSize.y = cy + py;
    func_801F970C(dst, reinterpret_cast<CPartyStateS16Pair*>(&local.mSize),
                  &obj->mPair1C8);
}

// Copies two s16 pairs (e.g. a rect) into a 4x s16 quad. Retail: 4x lha + 4x sth.
// extern "C" keeps the retail unmangled symbol name for the call site in
// func_801F969C; noinline keeps that call site a real branch. Returns dst
// (which stays in r3 throughout the body, so the return adds no instructions -
// byte-identical to the matched void form) so func_801FA338 can feed the quad
// straight into setRect via the call result (retail: mr r4, r3).
extern "C" __declspec(noinline) CPartyStateS16Quad* func_801F970C(CPartyStateS16Quad* dst, const CPartyStateS16Pair* srcA,
                                                   const CPartyStateS16Pair* srcB) {
    dst->x = srcA->x;
    dst->y = srcA->y;
    dst->z = srcB->x;
    dst->w = srcB->y;
    return dst;
}

// Copies 4 u32 from src into the object's 0x444 region. Retail: 4x lwz + 4x stw.
extern "C" __declspec(noinline) void func_801F9730(CPartyStateWin* self, const u32* src) {
    self->mQuad444[0] = src[0];
    self->mQuad444[1] = src[1];
    self->mQuad444[2] = src[2];
    self->mQuad444[3] = src[3];
}

// us-801fb410 | func_801F9754
// Pushes a 0x24-byte record into the ring buffer of the view object at
// CPartyStateWin +0x14. The record payload is read from an uninitialized
// stack local (retail reads its own frame at sp+0xC..0x2A; the only retail
// caller passes just the object and the 4/5 type flag) - reproduced verbatim
// so the stack reads land at the retail offsets.
// Escape hatch so MWCC cannot delete the (retail-garbage) record reads:
// reading through a volatile pointer keeps every load real and pinned in
// ascending order before the address math (retail batch-load shape).
extern "C" void func_801F9754(CPartyStateWinRing* self, u32 flag) {
    // Uninitialized stack record: retail reads its own frame at sp+0xC..0x2A
    // (the only retail caller passes just the object and the type flag, so
    // these are stale stack bytes).
    CPartyStateWinRec rec;
    volatile CPartyStateWinRec* rp = &rec;
    s32 total = self->field_0x3f0 + self->field_0x3f4;
    CPartyStateWinRec* dst =
        (CPartyStateWinRec*)(self->field_0x3ec +
                             (total % self->field_0x3f8) * 0x24);
    u32 type = 4;
    if (flag != 0) {
        type = 5;
    }
    u32 v4 = rp->field_0x4;
    u32 v8 = rp->field_0x8;
    u32 vc = rp->field_0xc;
    u32 v10 = rp->field_0x10;
    u32 v14 = rp->field_0x14;
    u32 v18 = rp->field_0x18;
    u32 v1c = rp->field_0x1c;
    u16 v20 = rp->field_0x20;
    u8 v22 = rp->field_0x22;
    dst->field_0x0 = type;
    dst->field_0x4 = rp->field_0x4;
    dst->field_0x8 = rp->field_0x8;
    dst->field_0xc = rp->field_0xc;
    dst->field_0x10 = rp->field_0x10;
    dst->field_0x14 = rp->field_0x14;
    dst->field_0x18 = rp->field_0x18;
    dst->field_0x1c = v1c;
    dst->field_0x20 = v20;
    dst->field_0x22 = v22;
    dst->field_0x23 = 0;
    self->field_0x3f4 += 1;
    self->field_0x3fc = self->field_0x3f4 - 1;
}
// us-801fb4d8 | func_801F981C (0x48)
// Pre-fills the two memory-accounting counters of a CPartyStateWinMem with
// the largest allocatable size of the MEM1 handle and of the scene-alloc
// handle; returns self (retail keeps `this` in r31 and returns it).
__declspec(noinline) CPartyStateWinMem* func_801F981C(CPartyStateWinMem* self) {
    self->field_0x0 = mtl::MemManager::getMaxAllocSize(mtl::MemManager::getHandleMEM1());
    self->field_0x4 = mtl::MemManager::getMaxAllocSize(func_80495FF0(lbl_eu_80663E14));
    return self;
}

// us-801fb520 | func_801F9864 (0x30)
// Records a 6-argument party-window payload into a PartyGaugeRecord (layout
// in CPartyStateWin.hpp). The array fill is a constant-trip countdown loop;
// optimize_for_size keeps it ROLLED (retail 0x30 body; plain -O4,p fully
// unrolls the 10 stores).
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_801F9864(PartyGaugeRecord* rec, u32 a, f32 f, u16 b, u16 c,
                              u16 d) {
    rec->field_0 = a;
    rec->field_8 = f;
    rec->field_C = b;
    rec->field_22 = d;
    for (int i = 0; i < 10; i++) {
        rec->gauge[i] = c;
    }
}
#pragma pop
