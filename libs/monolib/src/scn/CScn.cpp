// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual unsigned long v9() = 0; // mtl::ALLOC_HANDLE (scene heap handle)
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual ~VTarget() {}
};

// Inline-empty ~IWorkEvent for this TU so MWCC elides the empty base-dtor
// call in CScn::~CScn (retail shape; see IWorkEvent.hpp note).
#define IWORK_EVENT_INLINE_DTOR
#include "monolib/work.hpp"
#include "monolib/scn/CScnItemPool.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "libs/monolib/src/scn/CScn.hpp"
#include "libs/monolib/src/scn/CScnCameraMan.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"
#include "monolib/scn/CScnItemCamera.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/scn/CScnNw4r.hpp"
#include <new>

// --- CTTask<CScn> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move__13CTTask<4CScn>Fv /
// Draw__13CTTask<4CScn>Fv / __dt__13CTTask<4CScn>Fv symbols.
template<>
void CTTask<CScn>::Move() {
    if (mMoveFunc) {
        (static_cast<CScn*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CScn>::Draw() {
    if (mDrawFunc) {
        (static_cast<CScn*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CScn>::~CTTask() {}

// CScn::~CScn() (retail __dt__4CScnFv). MWCC repoints both vtable pointers
// (+0x10 primary, +0x54 IWorkEvent) at __vt__4CScn, destroys the +0x88
// subobject, then runs the ~CProcess base dtor directly (the empty ~CTTask
// specialization above is elided). The deleting-flag tail (operator delete)
// is emitted automatically because the destructor is virtual.
CScn::~CScn() {
    if (unk88 != NULL) {
        if (unk88 != NULL) {  // twin guard matches retail's paired beq
            ((ScnSubDtor*)unk88)->dtorSlot(1);
        }
        unk88 = NULL;
    }
}

// CScn constructor (retail __ct__4CScnFv). The inlined CTTask<CScn> ctor runs
// the CProcess base ctor, installs __vt__13CTTask<4CScn> and nulls the two
// member-function pointers; MWCC then installs the final __vt__4CScn pair
// (+0x10 / +0x54) before the body zeroes the subsystem slots and seeds the
// timing / render-callback fields.
CScn::CScn() {
    unk58 = 0;
    unk5C = NULL;
    mItemPool = NULL;
    unk64 = NULL;
    mCamWork = NULL;
    unk6C = NULL;
    mFilterMan = NULL;
    unk74 = NULL;
    unk78 = NULL;
    mLgtCtrl = NULL;
    mFadeMan = NULL;
    mFrame84 = NULL;
    unk88 = NULL;
    mUnk8C = NULL;
    unk90 = 0;
    unk98 = 10.0f;
    unk9C = 0x100;
    unkB2 = 0x40;
    for (int i = 0; i < 10; i++) {
        unk9E[i] = 0x50;
    }
    unkB4 = NULL;
    unkB8 = 0;
    mRenderCBCount = 0;      //0x3BC
    unk3C0[0] = 0;
    unk3E0 = 0;
    unk_3E4 = 0;
    unk_3E5 = 1;
    unk_3E6 = 1;
    unk_3E7 = 0;
    unk_3E8 = 1;
    unk_3E9 = 0;
}

extern "C" {
CScn* lbl_eu_80665908;
}

void CScn::addRenderCB(IScnRender* cb, u32 prio, u32 flag) {
    mRenderCBs[mRenderCBCount].cb = cb;
    mRenderCBs[mRenderCBCount].prio = prio;
    mRenderCBs[mRenderCBCount].flag = flag;

    u32 count = mRenderCBCount + 1;
    mRenderCBCount = count;

    s32 i = count - 2;
    CScn* p = (CScn*)((u8*)this + i * sizeof(ScnRenderCB));
    for (; i >= 0; i--) {
        ScnRenderCB& a = p->mRenderCBs[0];
        ScnRenderCB& b = ((CScn*)((u8*)p + sizeof(ScnRenderCB)))->mRenderCBs[0];
        if ((s32)a.prio > (s32)b.prio) {
            IScnRender* tmpCb = a.cb;
            u32 tmpPrio = a.prio;
            u8 tmpFlag = a.flag;

            a.cb = b.cb;
            a.prio = b.prio;
            a.flag = b.flag;

            b.cb = tmpCb;
            b.prio = tmpPrio;
            b.flag = tmpFlag;
        }
        p = (CScn*)((u8*)p - sizeof(ScnRenderCB));
    }
}

void CScn::removeRenderCB(IScnRender* cb) {
    s32 count = mRenderCBCount;
    u32 i = 0;

    for (; count > 0; count--) {
        if (mRenderCBs[i].cb == cb) {
            for (; (s32)i < (s32)(mRenderCBCount - 1);) {
                IScnRender* nextCb = mRenderCBs[i + 1].cb;
                i++;
                mRenderCBs[i - 1].cb = nextCb;
                mRenderCBs[i - 1].prio = mRenderCBs[i].prio;
                mRenderCBs[i - 1].flag = mRenderCBs[i].flag;
            }
            mRenderCBCount = mRenderCBCount - 1;
            return;
        }
        i++;
    }
}

// CScn::Init() = empty (retail blr)
void CScn::Init() {}

void CScn::Draw() {
    if (((CScnCameraMan*)mCamWork)->syncCameraToView() != 0) {
        CView* view = (CView*)((CScnCameraMan*)mCamWork)->getViewThread(
            (s16)((CScnCameraMan*)mCamWork)->mCamId);
        if (view == CView::getCurrentView()) {
            if (unk_3E4 == 0) {
                lbl_eu_80665908 = this;

                if (unk_3E5 != 0) {
                    if (unk_3E6 != 0) {
                        mUnk8C->vf18();
                    }
                }

                s32 i = 0;
                for (; i < (s32)mRenderCBCount;) {
                    if (unk_3E5 != 0 || mRenderCBs[i].flag != 0) {
                        mRenderCBs[i].cb->cbRenderBefore(this);
                    }
                    i += 1;
                }

                lbl_eu_80665908 = 0;
                unk_3E8 = 1;
                mUnk8C->vf1C();
            }
        }
    }
    unk_3E8 = 1;
}


extern "C" bool Scn_IsAnimActiveOrNull(CScnItemModel* self) {
    extern bool CScnItemPool_unlinkAndReleaseItem(void*, void*);
    if (self == 0) {
        return 1;
    }
    return CScnItemPool_unlinkAndReleaseItem(self->field_04->pool, self);
}
extern "C" bool Scn_HasWorkItem(CScnItemModelOwner* self) {
    return ((bool (*)(void*))CScnItemPool_releaseItemByKey)(self->pool); // DEBT(callcast)
}
extern "C" void* Scn_SetupAnim(void* a, void* b, void* c, void* d) {
    return simFwdAnimSetup102(a, b, c, d, 0);
}
extern "C" CScnItemModel* Scn_SetupAnimDefault(u32 param_1, void* param_2) {
    return (CScnItemModel*)simFwdAnimSetup102(
        (void*)param_1, (void*)param_2, (void*)-1, (void*)0, 0);
}
extern "C" int Scn_ResetAnim(int a, int b) {
    return simFwdAnimReset(a, b, 8);
}
extern "C" void Scn_InitGlobalA() {
    __ct__8049E710();
}
extern "C" void Scn_InitGlobalB() {
    __ct__804820F8();
}
extern "C" void Scn_CallUnk8C_V6(u8* self) {
    VTarget* obj = (VTarget*)((CScn*)self)->mUnk8C;
    obj->v6();
}
extern "C" void Scn_CallUnk8C_V7(u8* self) {
    VTarget* obj = (VTarget*)((CScn*)self)->mUnk8C;
    obj->v7();
}
u32 CScn::getAllocHandle() {
    VTarget* obj = (VTarget*)mUnk8C;
    return obj->v9();
}
extern "C" void Scn_CallUnk8C_V10(u8* self) {
    VTarget* obj = (VTarget*)((CScn*)self)->mUnk8C;
    obj->v10();
}
extern "C" void Scn_CallUnk8C_V8(u8* self) {
    VTarget* obj = (VTarget*)((CScn*)self)->mUnk8C;
    obj->v8();
}
extern "C" void Scn_ReleaseUnk80(CScn* self) {
    startFade(self->mFadeMan);
}
extern "C" void* Scn_GetUnk80Handle(CScn* self) {
    return isFadeIdle(self->mFadeMan);
}
extern "C" u32 Scn_QueryUnk80State(CScn* self) {
    return (u32)getFadeColorPtr(self->mFadeMan);
}
extern "C" bool Scn_IsDefaultScale(CScn* self) {
    bool result = false;
    if (isFadeIdle(self->mFadeMan)) {
        if (((ScnFloats*)getFadeColorPtr(self->mFadeMan))->unk0C == 0.0f)
            result = true;
    }
    return result;
}
// Same shape as Scn_IsDefaultScale but an ordered >= compare against 1.0f.
extern "C" bool Scn_IsScaleAtLeastOne(CScn* self) {
    bool result = false;
    if (isFadeIdle(self->mFadeMan)) {
        if (((ScnFloats*)getFadeColorPtr(self->mFadeMan))->unk0C >= 1.0f)
            result = true;
    }
    return result;
}
extern "C" void* Scn_GetUnk80Resource(CScn* self) {
    return (void*)isFadeSettled((CScnFadeMan*)self->mFadeMan);
}
extern "C" void Scn_GetCamWorkInt(CScn* self, CView* view, u32 idx) {
    ((CScnCameraMan*)self->mCamWork)->swapViewParam(view, idx);
}
// Fetches the camera item for `id` and copies its +0x194 projection matrix
// (MTX44, 16 words) into dest. Per-slot coloring recipe (MWCC_CASES.md
// LODMemMan rule): the hi-word temp is DECLARED first (colors r0 like
// retail) while reads/stores run lo-word first (pins emission order); each
// slot uses a fresh declaration pair so slots 1..7 keep the same colors.
extern "C" void Scn_CopyCamProjMatrix(CScn* self, ScnCamParams* dest, s32 id) {
    // Camera-item lookup by camera id (CScnCameraMan::findItem).
    CScnItemCamera* item =
        (CScnItemCamera*)((CScnCameraMan*)self->mCamWork)->findItem(id);
    u32* proj = (u32*)item->mProjMtx2;
    // Decl order colors the temps (hi -> r0, lo -> r4 like retail); the
    // read/store order below pins the emission order (lo word first).
    u32 w1;
    u32 w0 = proj[0];
    w1 = proj[1];
    dest->w[1] = w1;
    dest->w[0] = w0;
    {
        u32 h1;
        u32 l1 = proj[2];
        h1 = proj[3];
        dest->w[3] = h1;
        dest->w[2] = l1;
    }
    {
        u32 h2;
        u32 l2 = proj[4];
        h2 = proj[5];
        dest->w[5] = h2;
        dest->w[4] = l2;
    }
    {
        u32 h3;
        u32 l3 = proj[6];
        h3 = proj[7];
        dest->w[7] = h3;
        dest->w[6] = l3;
    }
    {
        u32 h4;
        u32 l4 = proj[8];
        h4 = proj[9];
        dest->w[9] = h4;
        dest->w[8] = l4;
    }
    {
        u32 h5;
        u32 l5 = proj[10];
        h5 = proj[11];
        dest->w[11] = h5;
        dest->w[10] = l5;
    }
    {
        u32 h6;
        u32 l6 = proj[12];
        h6 = proj[13];
        dest->w[13] = h6;
        dest->w[12] = l6;
    }
    {
        u32 h7;
        u32 l7 = proj[14];
        h7 = proj[15];
        dest->w[15] = h7;
        dest->w[14] = l7;
    }
}
// Get the camera item handle, lazily creating id -1 when missing; returns
// the item's local matrix (+0x9C, sibling Scn_GetCamItemCC returns the
// inverse at +0xCC).
extern "C" void* Scn_GetCamItem9C(CScn* self, s32 id) {
    CScnItemCamera* r = (CScnItemCamera*)((CScnCameraMan*)self->mCamWork)->getItem(id);
    if (!r)
        r = (CScnItemCamera*)((CScnCameraMan*)self->mCamWork)->getItem(-1);
    return (void*)&r->mTransform.mLocalMat;
}

// Sibling returning the inverse local matrix (+0xCC).
extern "C" void* Scn_GetCamItemCC(CScn* self, s32 id) {
    CScnItemCamera* r = (CScnItemCamera*)((CScnCameraMan*)self->mCamWork)->getItem(id);
    if (!r)
        r = (CScnItemCamera*)((CScnCameraMan*)self->mCamWork)->getItem(-1);
    return (void*)&r->mTransform.mLocalMatInv;
}
extern "C" void* Scn_FindCamItem(CScn* self, s32 id) {
    return ((CScnCameraMan*)self->mCamWork)->findItem(id);
}
extern "C" void* Scn_HasCamItem(CScn* self, CScnCameraParam* view) {
    return ((CScnCameraMan*)self->mCamWork)->findViewItem(view);
}
extern "C" void* Scn_GetCamList(CScn* self, CScnCameraParam* view) {
    return ((CScnCameraMan*)self->mCamWork)->getViewItem(view);
}
extern "C" void Scn_SetCamIndex(CScn* self, int arg) { ((CScnCameraMan*)self->mCamWork)->getViewThread((short)arg); }
// Scene time scale: PAL consoles without the speed fix run at 1.2x.
float Scn_GetFrameDelta(void* self_) {
    CScn* self = (CScn*)self_;
    int pal = 0;
    if (CDeviceVI::isTvFormatPal()) {
        if (self->unk_3E9 == 0)
            pal = 1;
    }
    float mul;
    if (pal != 0)
        mul = 1.2f;
    else
        mul = 1.0f;
    float scale = self->mFrame84->mElapsed;
    return scale * mul;
}
// retail: lwz r3,0x84(r3); stfs f1,0xC(r3); blr
void Scn_SetTimeScale(CScn* self, float v) {
    self->mFrame84->mStep = v;
}
extern "C" void Scn_SetPauseFlag(void* p, unsigned char v) {
    ((CScn*)p)->unk_3E5 = v;
}
// retail: stb r4,0x3e6(r3); blr
extern "C" void Scn_SetStopFlag(u8* self, u8 v) { ((CScn*)self)->unk_3E6 = v; }
// IWorkEvent overrides: forward the event to the camera manager / item pool
// (both IWorkEvent-derived). Overriding a secondary-base virtual makes MWCC
// append these real bodies to the end of __vt__4CScn and route the +0x54
// slots through weak @84@ thunks (see MWCC_CASES "MI: overriding a
// secondary-base virtual").
bool CScn::WorkEvent3(UNKTYPE* r4) {
    return ((CScnCameraMan*)mCamWork)->WorkEvent3(r4);
}
bool CScn::WorkEvent4() {
    return mItemPool->WorkEvent4();
}
extern "C" void Scn_SetCurrentScene(u32 value) {
    lbl_eu_80665908 = (CScn *)value;
}
extern "C" int Scn_GetCurrentScene() {
    return (int)lbl_eu_80665908;
}
extern "C" void CamMan_EvalScript0_B3FC();
extern "C" void CamMan_EvalScript_B408();
void execScriptCode__Fv(void) { CamMan_EvalScript0_B3FC(); }
void resetScriptCode__Fv(void) { CamMan_EvalScript_B408(); }
// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).


extern "C" void Scn_SetPalFixFlag(u8* self, unsigned char byte) {
    ((CScn*)self)->unk_3E9 = byte;
}

extern "C" void Scn_CallUnk8C_V11(u8* self) {
    VTarget* obj = (VTarget*)((CScn*)self)->mUnk8C;
    obj->v11();
}


// CScn::Term() (retail Term__4CScnFv). Releases every subsystem handle in
// retail order: each non-null field gets its first virtual slot called with
// flag=1 behind a twin guard (paired beq), then the field is nulled. The
// +0x7C light control is destroyed via a direct destructor call instead.
void CScn::Term() {
    if (mFadeMan != NULL) {
        if (mFadeMan != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)mFadeMan)->release(1);
        }
        mFadeMan = NULL;
    }
    if (mFilterMan != NULL) {
        if (mFilterMan != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)mFilterMan)->release(1);
        }
        mFilterMan = NULL;
    }
    if (unk6C != NULL) {
        if (unk6C != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)unk6C)->release(1);
        }
        unk6C = NULL;
    }
    if (unk74 != NULL) {
        if (unk74 != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)unk74)->release(1);
        }
        unk74 = NULL;
    }
    if (unk64 != NULL) {
        if (unk64 != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)unk64)->release(1);
        }
        unk64 = NULL;
    }
    if (unk78 != NULL) {
        if (unk78 != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)unk78)->release(1);
        }
        unk78 = NULL;
    }
    // Direct destructor call, not a virtual dispatch.
    if (mLgtCtrl != NULL) {
        __dt__804C0E48(mLgtCtrl, 1);
        mLgtCtrl = NULL;
    }
    if (mCamWork != NULL) {
        if (mCamWork != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)mCamWork)->release(1);
        }
        mCamWork = NULL;
    }
    if (unk5C != NULL) {
        if (unk5C != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)unk5C)->release(1);
        }
        unk5C = NULL;
    }
    if (mItemPool != NULL) {
        if (mItemPool != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)mItemPool)->release(1);
        }
        mItemPool = NULL;
    }
    if (mFrame84 != NULL) {
        if (mFrame84 != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)mFrame84)->release(1);
        }
        mFrame84 = NULL;
    }
    if (mUnk8C != NULL) {
        if (mUnk8C != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)mUnk8C)->release(1);
        }
        mUnk8C = NULL;
    }
}
// CScn::Move() (retail Move__4CScnFv). Runs every scene subsystem's per-frame
// update, gated by the pause/stop flags.
void CScn::Move() {
    if (unk_3E4 == 0 && unk_3E7 == 0) {
        mFrame84->update();
        scnLgtStepLightItems(mLgtCtrl);
        mItemPool->update();
        mFilterMan->update();
        mFadeMan->update();
        mUnk8C->vf14();
        mItemPool->updateItemD0();
    }
    unk_3E8 = 0;
}
// Scene factory. Retail exports it under the flat, unmangled symbol
// create__8CScnNw4rFv (the public header declares it parameterless even
// though every caller passes five arguments), so the definition keeps the
// flat name via C linkage.

// Flat declarations so the emitted relocations carry the retail names.
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                              bool top);
extern "C" void initSubsystems__4CScnFUlUlPvUl(CScn* self, u32 a, u32 b,
                                              void* str, u32 d);

extern "C" CScnNw4r* create__8CScnNw4rFv(CProcess* parent, char* name,
                                         u32 arg, u32 handle, void* param) {
    // Inline clear(): mString[0] = 0, mLength = 0 (retail stores, no
    // out-of-line FixStr ctor call).
    ml::FixStr<32> str(true);
    str.format("%s(%s)", "CScn", name);

    // Default handle sentinel: fall back to the MEM1 region handle.
    if ((u32)(handle + 0x10000) == 0xffff) {
        handle = mtl::MemManager::getHandleMEM1();
    }

    CScnNw4r* scn = new (mtl::MemManager::allocate(
        sizeof(CScnNw4r), CWorkThreadSystem::getWorkMem())) CScnNw4r();

    Regist__8CProcessFP8CProcessb((CProcess*)scn, parent, false);
    initSubsystems__4CScnFUlUlPvUl((CScn*)scn, handle, arg, &str, (u32)param);
    return scn;
}


