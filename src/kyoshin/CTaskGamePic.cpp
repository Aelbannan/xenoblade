// kyoshin/CTaskGamePic.cpp
// CTaskGamePic task wrapper: CProcess-derived task that hosts a scene render
// callback (IScnRender subobject), an async file load, and a GX texture bound
// from the loaded palette. Size 0xC8.

#include "kyoshin/CTaskGamePic.hpp"

#include "monolib/core/CDrawGX.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/tpl/TPL.h>

// Helper import from another TU (retail C-linkage symbol, no mangling).
extern "C" void* func_80495FF0(const void* scene);

// forward declarations for scaffold thunk references (non-target glue)
void __dt__12CTaskGamePicFv(void*);
void cbRenderBefore__12CTaskGamePicFv(void*);

// CTTask<CTaskGamePic> out-of-line Move/Draw/dtors (retail emits these as
// standalone functions; the inline CTTask header copy would mark them inline).
#pragma optimize_for_size on
template <>
CTTask<CTaskGamePic>::~CTTask() {}
#pragma optimize_for_size off

template <>
void CTTask<CTaskGamePic>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskGamePic*>(this)->*mMoveFunc)();
    }
}

template <>
void CTTask<CTaskGamePic>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskGamePic*>(this)->*mDrawFunc)();
    }
}

// ---------------------------------------------------------------------------
// ctor - Retail `__ct__CTaskGamePic` is a *stripped* symbol (no length
// mangling), reconstructed as a global free function (cf. CTaskGameEvt). Calls
// the CProcess base ctor, sets the interim CTTask vtable, NULLs the Move/Draw
// PTMFs, installs the final CTaskGamePic vtable and member fields, zeroes the
// trailing texture byte, and initialises the three colour vectors at
// 0x90/0xA0/0xB0 to the shared sdata2 LOD-bias float.
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
// C-linkage so the emitted symbol is exactly the retail `__ct__CTaskGamePic`
// (stripped symbol; a C++ declaration would mangle to __ct__CTaskGamePicFi),
// and so create()'s call reloc references the retail name (cf. CTaskGameEvt).
extern "C" __declspec(noinline) CTaskGamePic* __ct__CTaskGamePic(CTaskGamePic* pThis, int arg) {
    __ct__8CProcessFv(pThis);

    char* vtbl = lbl_eu_80538AD8;      // final CTaskGamePic vtable region
    u32 v54 = (u32)(vtbl + 0x24);      // field_54 sub-vtable
    u32 v58 = (u32)(vtbl + 0xac);      // IScnRender member vtable

    u32* p = reinterpret_cast<u32*>(pThis);

    // Interim CTTask<CTaskGamePic> vtable (overwritten by the final vptr).
    p[4] = reinterpret_cast<u32>(lbl_eu_80538BC0);

    // NULL PTMF -> mMoveFunc (0x3C) / mDrawFunc (0x48): retail assigns the
    // whole 3-word pointer-to-member, so mirror that with typed struct copies
    // and let MWCC pick the load/store schedule.
    PTMF3* pMf = reinterpret_cast<PTMF3*>(reinterpret_cast<char*>(pThis) + 0x3c);
    const PTMF3* pNull = reinterpret_cast<const PTMF3*>(__ptmf_null);
    *pMf = *pNull;
    PTMF3* pDf = reinterpret_cast<PTMF3*>(reinterpret_cast<char*>(pThis) + 0x48);
    *pDf = *pNull;

    // Final vtable + member fields.
    p[4] = (u32)vtbl;
    p[0x15] = v54;                 // 0x54 field_54
    p[0x16] = v58;                 // 0x58 IScnRender member
    p[0x17] = (u32)arg;            // 0x5C mScene
    p[0x18] = 0;                   // 0x60 mFileHandle
    p[0x19] = 0;                   // 0x64
    p[0x1A] = 0;                   // 0x68

    reinterpret_cast<u8*>(pThis)[0x8c] = 0;  // trailing texture byte

    // Three colour (RGBA) vectors - single shared float, four components.
    func_800407C8(reinterpret_cast<func_800407C8_tmp*>(p + 0x24),
                  lbl_eu_80668BB0, lbl_eu_80668BB0, lbl_eu_80668BB0, lbl_eu_80668BB0);
    func_800407C8(reinterpret_cast<func_800407C8_tmp*>(p + 0x28),
                  lbl_eu_80668BB0, lbl_eu_80668BB0, lbl_eu_80668BB0, lbl_eu_80668BB0);
    func_800407C8(reinterpret_cast<func_800407C8_tmp*>(p + 0x2c),
                  lbl_eu_80668BB0, lbl_eu_80668BB0, lbl_eu_80668BB0, lbl_eu_80668BB0);
    p[0x30] = 0;                   // param_C0
    p[0x31] = 0;                   // param_C4

    return pThis;
}
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// Move - animates the colour fade between the "from" (0xA0) and "to"
// (0xB0) RGBA vectors into the current colour (0x90). param_C0 is an 8.8
// fixed-point countdown, param_C4 the fixed total. Each call steps 0x100 and
// lerps current by t = param_C0/param_C4; when the countdown ends the target
// colour is snapped in and the counter cleared. The counters are split into
// high/low bytes (/256 and %256) before the int->f32 casts so the ratio is
// computed from exact byte values.
// ---------------------------------------------------------------------------
// Size-opt: retail keeps both /256 computations per counter (no CSE) and
// interleaves the sdata2 float loads, matching the -Os scheduler.
#pragma optimize_for_size on
void CTaskGamePic::Move() {
    if (param_C0 == 0) return;
    s32 c1 = (s32)param_C0 - 0x100;
    param_C0 = (u32)c1;
    if (c1 <= 0) {
        // Countdown finished: snap the target colour in and clear the counter.
        param_90 = param_B0;
        param_94 = param_B4;
        param_98 = param_B8;
        param_9C = param_BC;
        param_C0 = 0;
        return;
    }
    s32 c4 = (s32)param_C4;
    // Size-opt (-O4,s) lowers signed % to the div-based srawi/addze/slwi/subf
    // form seen in retail (the -O4,p default gives an rlwinm pair instead).
    // Spelled as one expression so MWCC interleaves the numerator/denominator
    // slot stores like retail. NOTE: the int->f32 casts emit MWCC's internal
    // magic-double pool item where retail references the shared .sdata2
    // symbol lbl_eu_80668BC0; hand-spelling the slots to name that symbol
    // perturbs the schedule (+16B), so the cast shape is kept.
    f32 t = ((f32)(c1 % 256) * lbl_eu_80668BB4 + (f32)(c1 / 256)) /
            ((f32)(c4 % 256) * lbl_eu_80668BB4 + (f32)(c4 / 256));
    f32 inv = lbl_eu_80668BB8 - t;
    f32* cur = reinterpret_cast<f32*>(&param_90);
    const f32* from = reinterpret_cast<const f32*>(&param_A0);
    const f32* to = reinterpret_cast<const f32*>(&param_B0);
    cur[0] = from[0] * t + to[0] * inv;
    cur[1] = from[1] * t + to[1] * inv;
    cur[2] = from[2] * t + to[2] * inv;
    cur[3] = from[3] * t + to[3] * inv;
}
#pragma optimize_for_size off
// ---------------------------------------------------------------------------
// Init - registers the render callback subobject with the scene,
// using priority 0xb (HUD layer).
// ---------------------------------------------------------------------------
void CTaskGamePic::Init() {
    IScnRender* rp = reinterpret_cast<IScnRender*>(this); // null-this -> this(0)
    if (this) rp = reinterpret_cast<IScnRender*>(mRenderCB); // live: this + 0x58
    mScene->addRenderCB(rp, 0xb, 0);
}

#pragma optimize_for_size on
CTaskGamePic::~CTaskGamePic() {}
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// func_80294E58 - shifts the 8-word texture/palette parameter block
// (0x90..0xC4). If the 3rd source word is non-zero the source block also
// overwrites the "current" block at 0x90.
// ---------------------------------------------------------------------------
extern "C" void func_80294E58(CTaskGamePic* self, u32 index, const u32* src) {
    u32 sh = index << 8;
    self->param_A0 = self->param_90;
    self->param_A4 = self->param_94;
    self->param_A8 = self->param_98;
    self->param_AC = self->param_9C;
    self->param_B0 = src[0];
    self->param_B4 = src[1];
    self->param_B8 = src[2];
    self->param_BC = src[3];
    self->param_C0 = sh;
    self->param_C4 = sh;
    if (index == 0) {
        self->param_90 = src[0];
        self->param_94 = src[1];
        self->param_98 = src[2];
        self->param_9C = src[3];
    }
}

// ---------------------------------------------------------------------------
// func_80294EC0 - kicks off an async file load for the scene's
// resource, using the embedded file-event object at +0x54 as the callback.
// Retail saves r29-r31 with stmw/lmw (size-opt frame shape).
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
extern "C" void func_80294EC0(CTaskGamePic* self, const char* path) {
    IWorkEvent* ev = reinterpret_cast<IWorkEvent*>(self); // null-this -> null
    if (self) ev = reinterpret_cast<IWorkEvent*>(&self->field_54);
    u32 handle = (u32)func_80495FF0(self->mScene);
    CFileHandle* fh = CDeviceFile::readFile(handle, path, ev, 0, 0);
    self->mFileHandle = fh;
    CDeviceFile::func_8044F154(fh, 0);
}
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// Term - unregisters the render callback, cancels the async load,
// waits for drawing to drain, and frees palette/texture memory.
// ---------------------------------------------------------------------------
void CTaskGamePic::Term() {
    IScnRender* rp = reinterpret_cast<IScnRender*>(this); // null-this -> this(0)
    if (this) rp = reinterpret_cast<IScnRender*>(mRenderCB); // live: this + 0x58
    mScene->removeRenderCB(rp);
    if (mFileHandle) {
        CDeviceFile::cancel(mFileHandle);
        mFileHandle = nullptr;
    }
    CDeviceVI::waitForDrawDone();
    if (field_64) {
        mtl::MemManager::deallocate(field_64);
        field_64 = nullptr;
    }
}

// ---------------------------------------------------------------------------
// func_8029539C - the file-event handler for the loaded texture.
// Binds the palette, builds a GX texture object from it, then clears the load.
// ---------------------------------------------------------------------------
extern "C" bool func_8029539C(CTaskGamePic* self, CEventFile* pEvent) {
    // Retail loads pEvent->mFileHandle first and compares it against
    // self->mFileHandle (cmplw r0, r5), so the event side is the left operand.
    if (pEvent->mFileHandle == self->mFileHandle) {
        if (pEvent->unk0 == 1) {
            u8* data = static_cast<u8*>(self->mFileHandle->getData());
            self->field_64 = data;
            self->field_68 = data;
            TPLBind((TPLPalette*)data);
            TPLGetGXTexObjFromPalette((TPLPalette*)self->field_68, &self->mTexObj, 0);
            f32 bias = lbl_eu_80668BB0;
            GXInitTexObjLOD(&self->mTexObj, GX_LINEAR, GX_LINEAR,
                            bias, bias, bias, GX_FALSE, GX_FALSE, GX_ANISO_1);
        }
        self->mFileHandle = nullptr;
        return true;
    }
    return false;
}

// Non-target glue thunks (preserved from scaffold; not part of the match set).
void OnFileEvent__12CTaskGamePicFP10CEventFile(void* self) {
    ((void (*)(void*))func_8029539C)((char*)self - 0x54);
}

void func_8029554C(void* self) { ((void (*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x54); }

void func_80295554(void* self) { ((void (*)(void*))cbRenderBefore__12CTaskGamePicFv)((char*)self - 0x58); }

void func_8029555C(void* self) { ((void (*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x58); }

// Returns int (not s16) so callers re-sign-extend the result like retail.
extern "C" s16 func_80295388(u8* self) {
    return (s16)(*(s16*)(self + 2) + *(s16*)(self + 6));
}
// ---------------------------------------------------------------------------
// create - factory. Retail symbol keeps the C-linkage Fv name although
// the source takes a parent and a scene arg (cf. CTaskGameCf). Size-opt frame:
// stmw r29/lmw r29, ctor NOT inlined (noinline on __ct__CTaskGamePic).
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
extern "C" CTaskGamePic* create__12CTaskGamePicFv(CProcess* pParent, int arg) {
    u32 handle = CWorkThreadSystem::getWorkMem();
    CTaskGamePic* self = (CTaskGamePic*)mtl::MemManager::allocate(0xc8, handle);
    if (self) {
        self = __ct__CTaskGamePic(self, arg);
    }
    self->Regist(pParent, false);
    return self;
}
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// cbRenderBefore - pre-render hook: draws the loaded picture texture
// (mTexObj) as a 4-vertex quad centered on the current view, then a full-
// screen colour wash from the current 0x90 RGBA block. Both passes go through
// stack CDrawGX helpers; the whole thing is gated on the 0x8C ready flag and
// the palette data pointer.
//
// Retail prologue is the size-opt _savegpr_28 form (addi r11 + bl _savegpr),
// so the function sits under the same optimize_for_size wrap as the ctor/
// create (r28-r31 live across the many CDrawGX calls).
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
void CTaskGamePic::cbRenderBefore() {
    const CTaskGamePicTexData* tex;
    if (field_8C == 0) return;
    CView* view = CView::getCurrentView();
    if (field_68 == 0) return;
    CDeviceGX::getCacheInstance()->func_8044BE38();
    tex = static_cast<const CTaskGamePicTexData*>(field_68);

    // View-sized rect: narrow it to 3/4 width (centred) on 16:9.
    // (s16) casts on the u16 render-mode fields fold the sign extension
    // into lha loads, matching retail.
    ml::CRect rectA;
    func_8043EA88__5CViewFRQ22ml5CRectP5CView(rectA, view);
    if (CDeviceVI::isWideAspectRatio()) {
        // Signed /2 (round toward zero): MWCC lowers to srawi+addze.
        rectA.mPos.x = (rectA.mSize.x - (s32)(tex->mWidth * 75 / 100)) / 2;
        rectA.mSize.x = tex->mWidth * 75 / 100;
    } else {
        s16 h = (s16)CDeviceVI::getRenderModeObj()->efbHeight;
        s16 w = (s16)CDeviceVI::getRenderModeObj()->fbWidth;
        rectA.mSize.x = w;
        rectA.mPos.x = 0;
        rectA.mPos.y = 0;
        rectA.mSize.y = h;
    }

    // Texture quad: white, texture-cache flag cleared, view-rect based.
    // Inner scope so the automatic destructor fires where retail's does.
    {
        CDrawGX dgx0;
        dgx0.func_80456570(0);
        dgx0.func_8045657C(0);
        ml::CCol3 col;
        col.r = lbl_eu_80668BB8;
        col.g = lbl_eu_80668BB8;
        col.b = lbl_eu_80668BB8;
        dgx0.setCol(col);
        reinterpret_cast<CDrawGXFlagWord*>(&dgx0)->mFlags &= ~0x10u;
        ml::CRect16 rectB;
        func_8043EA88__5CViewFRQ22ml5CRectP5CView(*(ml::CRect*)&rectB, view);
        dgx0.renderRect(rectB);
        dgx0.setTex(&mTexObj, tex->mWidth, tex->mHeight);
        dgx0.begin(6, 4);
        dgx0.add(rectA.mPos.x, rectA.mPos.y, 0, 0);
        dgx0.add((s16)(rectA.mPos.x + rectA.mSize.x), rectA.mPos.y,
                 tex->mWidth, 0);
        // x computed into a local first so MWCC stages it in a callee-saved
        // register across the nested helper call (retail shape).
        s16 vx3 = rectA.mPos.x;
        dgx0.add(vx3, func_80295388(reinterpret_cast<u8*>(&rectA)), 0,
                 tex->mHeight);
        s16 vx4 = rectA.mPos.x + rectA.mSize.x;
        dgx0.add(vx4, func_80295388(reinterpret_cast<u8*>(&rectA)),
                 tex->mWidth, tex->mHeight);
        dgx0.end();
    }

    // Full-screen colour wash from the animated 0x90 RGBA block.
    CDeviceGX::getCacheInstance()->func_8044BE38();
    {
        CDrawGX dgx1;
        dgx1.func_80456570(0);
        dgx1.func_8045657C(0);
        dgx1.setCol(*(ml::CCol4*)&param_90);
        dgx1.begin(9, 1);
        ml::CRect rectC;
        s16 h = (s16)CDeviceVI::getRenderModeObj()->efbHeight;
        s16 w = (s16)CDeviceVI::getRenderModeObj()->fbWidth;
        rectC.mSize.x = w;
        rectC.mPos.x = 0;
        rectC.mPos.y = 0;
        rectC.mSize.y = h;
        dgx1.add(*(ml::CRect16*)&rectC);
        dgx1.end();
    }
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::func_80442DA8();
}

// Preserve empty Draw member (retail 4-byte body).
void CTaskGamePic::Draw() {}