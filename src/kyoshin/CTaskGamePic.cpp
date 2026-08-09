// kyoshin/CTaskGamePic.cpp
// CTaskGamePic task wrapper: CProcess-derived task that hosts a scene render
// callback (IScnRender subobject), an async file load, and a GX texture bound
// from the loaded palette. Size 0xC8.

#include "kyoshin/CTaskGamePic.hpp"

#include "monolib/device/CDeviceFile.hpp"
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
        (this->*mMoveFunc)();
    }
}

template <>
void CTTask<CTaskGamePic>::Draw() {
    if (mDrawFunc) {
        (this->*mDrawFunc)();
    }
}

// ---------------------------------------------------------------------------
// Target: ctor - Retail `__ct__CTaskGamePic` is a *stripped* symbol (no length
// mangling), reconstructed as a global free function (cf. CTaskGameEvt). Calls
// the CProcess base ctor, sets the interim CTTask vtable, NULLs the Move/Draw
// PTMFs, installs the final CTaskGamePic vtable and member fields, zeroes the
// trailing texture byte, and initialises the three colour vectors at
// 0x90/0xA0/0xB0 to the shared sdata2 LOD-bias float.
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
CTaskGamePic* __ct__CTaskGamePic(CTaskGamePic* pThis, int arg) {
    __ct__8CProcessFv(pThis);

    char* vtbl = lbl_eu_80538AD8;      // final CTaskGamePic vtable region
    u32 v54 = (u32)(vtbl + 0x24);      // field_54 sub-vtable
    u32 v58 = (u32)(vtbl + 0xac);      // IScnRender member vtable

    u32* p = reinterpret_cast<u32*>(pThis);

    // Interim CTTask<CTaskGamePic> vtable (overwritten by the final vptr).
    p[4] = reinterpret_cast<u32>(lbl_eu_80538BC0);

    // NULL PTMF -> mMoveFunc (0x3C) / mDrawFunc (0x48), retail store order.
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    p[0x10] = w1;
    p[0xF] = w0;
    u32 w2 = *src++;
    p[0x11] = w2;
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    p[0x13] = w0;
    p[0x12] = w1;
    w2 = *src++;
    p[0x14] = w2;

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
// Target: Move - animates the colour fade between the "from" (0xA0) and "to"
// (0xB0) RGBA vectors into the current colour (0x90). param_C0 is a 16-bit
// countdown (index<<8), param_C4 the fixed total. Each call steps 0x100 and
// lerps current by t = param_C0/param_C4; when the countdown ends the target
// colour is snapped in and the counter cleared.
// ---------------------------------------------------------------------------
void CTaskGamePic::Move() {
    s32 c0 = (s32)param_C0;
    if (c0 == 0) return;
    c0 -= 0x100;
    param_C0 = (u32)c0;
    if (c0 > 0) {
        f32* cur = reinterpret_cast<f32*>(&param_90);
        const f32* from = reinterpret_cast<const f32*>(&param_A0);
        const f32* to = reinterpret_cast<const f32*>(&param_B0);
        s32 c4 = (s32)param_C4;
        // Rational blend: decompose each counter into its 8-bit high byte and low
        // byte so MSL emits the srawi+addze / slwi+subf split and routes both
        // through the s32->f32 conversion, folding the low-byte *1/256 scale into
        // a single fmadds via the pooled 0.00390625 constant.
        s32 hi6 = c0 >> 8;
        s32 lo6 = c0 - (hi6 << 8);
        s32 hi7 = c4 >> 8;
        s32 lo7 = c4 - (hi7 << 8);
        f32 t = ((f32)hi6 + (f32)lo6 * 0.00390625f) /
                ((f32)hi7 + (f32)lo7 * 0.00390625f);
        f32 inv = 1.0f - t;
        cur[0] = from[0] * t + to[0] * inv;
        cur[1] = from[1] * t + to[1] * inv;
        cur[2] = from[2] * t + to[2] * inv;
        cur[3] = from[3] * t + to[3] * inv;
    } else {
        param_90 = param_B0;
        param_94 = param_B4;
        param_98 = param_B8;
        param_9C = param_BC;
        param_C0 = 0;
    }
}

// ---------------------------------------------------------------------------
// Target: Init - registers the render callback subobject with the scene,
// using priority 0xb (HUD layer).
// ---------------------------------------------------------------------------
void CTaskGamePic::Init() {
    IScnRender* rp = reinterpret_cast<IScnRender*>(this); // null-this -> this(0)
    if (this) rp = &mRenderCB;                            // live: this + 0x58
    mScene->addRenderCB(rp, 0xb, 0);
}

#pragma optimize_for_size on
CTaskGamePic::~CTaskGamePic() {}
#pragma optimize_for_size off

// ---------------------------------------------------------------------------
// Target: func_80294E58 - shifts the 8-word texture/palette parameter block
// (0x90..0xC4). If the 3rd source word is non-zero the source block also
// overwrites the "current" block at 0x90.
// ---------------------------------------------------------------------------
extern "C" void func_80294E58(CTaskGamePic* self, u32 index, const u32* src) {
    u32 a = self->param_90;
    u32 b = self->param_94;
    u32 c = self->param_98;
    u32 d = self->param_9C;
    u32 e = src[0];
    u32 f = src[1];
    u32 g = src[2];
    u32 h = src[3];
    u32 sh = index << 8;
    self->param_A0 = a;
    self->param_A4 = b;
    self->param_A8 = c;
    self->param_AC = d;
    self->param_B0 = e;
    self->param_B4 = f;
    self->param_B8 = g;
    self->param_BC = h;
    self->param_C0 = sh;
    self->param_C4 = sh;
    if (index == 0) {
        self->param_90 = e;
        self->param_94 = f;
        self->param_98 = g;
        self->param_9C = h;
    }
}

// ---------------------------------------------------------------------------
// Target: func_80294EC0 - kicks off an async file load for the scene's
// resource, using the embedded file-event object at +0x54 as the callback.
// ---------------------------------------------------------------------------
extern "C" void func_80294EC0(CTaskGamePic* self, const char* path) {
    IWorkEvent* ev = reinterpret_cast<IWorkEvent*>(self); // null-this -> null
    if (self) ev = reinterpret_cast<IWorkEvent*>(&self->field_54);
    u32 handle = (u32)func_80495FF0(self->mScene);
    CFileHandle* fh = CDeviceFile::readFile(handle, path, ev, 0, 0);
    self->mFileHandle = fh;
    CDeviceFile::func_8044F154(fh, 0);
}

// ---------------------------------------------------------------------------
// Target: Term - unregisters the render callback, cancels the async load,
// waits for drawing to drain, and frees palette/texture memory.
// ---------------------------------------------------------------------------
void CTaskGamePic::Term() {
    IScnRender* rp = reinterpret_cast<IScnRender*>(this); // null-this -> this(0)
    if (this) rp = &mRenderCB;                            // live: this + 0x58
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
// Target: func_8029539C - the file-event handler for the loaded texture.
// Binds the palette, builds a GX texture object from it, then clears the load.
// ---------------------------------------------------------------------------
extern "C" bool func_8029539C(CTaskGamePic* self, CEventFile* pEvent) {
    if (self->mFileHandle == pEvent->mFileHandle) {
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

extern "C" s16 func_80295388(u8* self) {
    return (s16)(*(s16*)(self + 2) + *(s16*)(self + 6));
}

// ---------------------------------------------------------------------------
// Target: create - factory. Retail symbol keeps the C-linkage Fv name although
// the source takes a parent and a scene arg (cf. CTaskGameCf).
// ---------------------------------------------------------------------------
extern "C" CTaskGamePic* create__12CTaskGamePicFv(CProcess* pParent, int arg) {
    u32 handle = CWorkThreadSystem::getWorkMem();
    CTaskGamePic* self = (CTaskGamePic*)mtl::MemManager::allocate(0xc8, handle);
    if (self) {
        self = __ct__CTaskGamePic(self, arg);
    }
    self->Regist(pParent, false);
    return self;
}

// Preserve empty Draw member (retail 4-byte body).
void CTaskGamePic::Draw() {}