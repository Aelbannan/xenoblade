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
#include "libs/monolib/src/scn/CScn.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
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

// CScn::~CScn() (retail __dt__4CScnFv). Repoints the accessor table (+0x10)
// and secondary IWorkEvent vtable slot (+0x54), destroys the +0x88
// subobject, then runs the ~CProcess base dtor directly (the empty ~CTTask
// specialization above is elided). The deleting-flag tail (operator delete)
// is emitted automatically because the destructor is virtual.
CScn::~CScn() {
    CScnDtorView* v = (CScnDtorView*)this;
    v->unk10 = lbl_eu_8056E8D0;
    v->unk54 = lbl_eu_8056E8D0 + 0x24;
    if (v->unk88 != NULL) {
        if (v->unk88 != NULL) {  // twin guard matches retail's paired beq
            ((ScnSubDtor*)v->unk88)->dtorSlot(1);
        }
        v->unk88 = NULL;
    }
}

// CScn constructor (retail flat symbol __ct__CScn). Runs the CProcess base
// ctor, installs the intermediate then final vtable pointers, nulls the two
// CTTask member-function pointers (MWCC materializes __ptmf_null for a null
// ptmf store; the post-increment walk folds the first read into lwzu @l),
// zeroes the 0x58..0x90 block and initializes the timing / render-callback
// fields. Returns self (retail keeps an or r3,r31,r31 copy live to blr).
extern "C" void __ct__8CProcessFv(CProcess* self);
extern const u32 __ptmf_null[3];
extern "C" CScn* __ct__CScn(CScn* self) {
    CScnCtorView* v = (CScnCtorView*)self;
    __ct__8CProcessFv((CProcess*)self);

    // Intermediate accessor-table pointer, repointed below.
    v->unk10 = lbl_eu_8056E984;

    u32 d0, m0;
    u32* move = (u32*)&v->mMoveFunc;
    const u32* n;
    f32 f98;
    n = __ptmf_null;
    m0 = *n++;
    f98 = lbl_eu_8066AAB0;
    move[1] = *n++;
    move[0] = m0;
    move[2] = *n++;
    // Second copy re-reads from the null-ptmf base.
    u32* draw = (u32*)&v->mDrawFunc;
    n = __ptmf_null;
    d0 = *n++;
    draw[1] = *n++;
    draw[0] = d0;
    draw[2] = *n++;

    // Final vtable pair (CTTask + IWorkEvent subobjects).
    v->unk10 = lbl_eu_8056E8D0;
    v->unk54 = lbl_eu_8056E8D0 + 0x24;

    v->unk58[0] = 0;
    v->unk58[1] = 0;
    v->unk58[2] = 0;
    v->unk58[3] = 0;
    v->unk58[4] = 0;
    v->unk58[5] = 0;
    v->unk58[6] = 0;
    v->unk58[7] = 0;
    v->unk58[8] = 0;
    v->unk58[9] = 0;
    v->unk58[10] = 0;
    v->unk58[11] = 0;
    v->unk58[12] = 0;
    v->unk58[13] = 0;
    v->unk58[14] = 0;

    v->unk98 = f98;
    v->unk9C = 0x100;
    v->unkB2 = 0x40;
    v->unk9E[0] = 0x50;
    v->unk9E[1] = 0x50;
    v->unk9E[2] = 0x50;
    v->unk9E[3] = 0x50;
    v->unk9E[4] = 0x50;
    v->unk9E[5] = 0x50;
    v->unk9E[6] = 0x50;
    v->unk9E[7] = 0x50;
    v->unk9E[8] = 0x50;
    v->unk9E[9] = 0x50;
    v->unkB4 = 0;
    v->unkB8 = 0;

    self->mRenderCBCount = 0;      //0x3BC
    ((u8*)self)[0x3C0] = 0;
    *(u32*)((char*)self + 0x3E0) = 0;
    self->unk_3E4 = 0;
    self->unk_3E5 = 1;
    self->unk_3E6 = 1;
    self->unk_3E7 = 0;
    self->unk_3E8 = 1;
    self->unk_3E9 = 0;
    return self;
}

// Retail thunk: the IWorkEvent subobject at +0x54 delegates back to the
// complete-object destructor with this-0x54.
extern "C" void* __dt__4CScnFv(CScn* self, int flags);
extern "C" void Scn_DtorThunk54(void* pThis, int flags) {
    __dt__4CScnFv((CScn*)((char*)pThis - 0x54), flags);
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
    if (CamMan_SyncCameraToView(mCamWork) != 0) {
        UnkScn68* cam = mCamWork;
        CView* view = CamMan_GetViewThread(cam, (s16)cam->unk34);
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
                        reinterpret_cast<IScnRenderDraw*>(mRenderCBs[i].cb)->onRender(this);
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


// Owner/pool lens for Scn_IsAnimActiveOrNull / Scn_HasWorkItem: the item
// carries its owner at +0x04 (CScnItemModel::field_04) and the owner carries
// the pool at +0x60 (CScnItemModelOwner::pool). TU-local so this TU stays
// free of the CScnItemModel header (whose Scn_SetupAnimDefault decl clashes
// with this TU's definition under -W error).
struct ScnOwnerPoolView {
    u8 _00[0x60]; // 0x00..0x60
    void* pool;   // 0x60
};
struct ScnItemOwnerView {
    u8 _00[4];                 // 0x00..0x04
    ScnOwnerPoolView* owner04; // 0x04
};
extern "C" bool Scn_IsAnimActiveOrNull(u8* self) {
    extern bool CScnItemPool_unlinkAndReleaseItem(void*, void*);
    if (self == 0) {
        return 1;
    }
    return CScnItemPool_unlinkAndReleaseItem(((ScnItemOwnerView*)self)->owner04->pool, self);
}
extern "C" bool Scn_HasWorkItem(u8* self) {
    extern bool CScnItemPool_releaseItemByKey(void*);
    return CScnItemPool_releaseItemByKey(((ScnOwnerPoolView*)self)->pool);
}
extern "C" void* Scn_SetupAnim(void* a, void* b, void* c, void* d) {
    return simFwdAnimSetup102(a, b, c, d, 0);
}
extern "C" void Scn_SetupAnimDefault(s32 param_1, s32 param_2) {
    simFwdAnimSetup102((void*)param_1, (void*)param_2, (void*)-1, (void*)0, 0);
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
extern "C" void Scn_ReleaseUnk80(void* p) {
    startFade((int)((CScnMoveView*)p)->fadeMan);
}
extern "C" void* Scn_GetUnk80Handle(void* _this) {
    return isFadeIdle(((CScnMoveView*)_this)->fadeMan);
}
extern "C" u32 Scn_QueryUnk80State(u8* self) {
    extern u32 getFadeColorPtr(u32);
    return getFadeColorPtr((u32)((CScnMoveView*)self)->fadeMan);
}
extern "C" bool Scn_IsDefaultScale(u8* self) {
    extern void* getFadeColorPtr(void*);
    bool result = false;
    if (isFadeIdle(((CScnMoveView*)self)->fadeMan)) {
        if (((ScnFloats*)getFadeColorPtr(((CScnMoveView*)self)->fadeMan))->unk0C == lbl_eu_8066AAB4)
            result = true;
    }
    return result;
}
// Same shape as Scn_IsDefaultScale but an ordered >= compare against 1.0f.
extern "C" bool Scn_IsScaleAtLeastOne(u8* self) {
    extern void* getFadeColorPtr(void*);
    bool result = false;
    if (isFadeIdle(((CScnMoveView*)self)->fadeMan)) {
        if (((ScnFloats*)getFadeColorPtr(((CScnMoveView*)self)->fadeMan))->unk0C >= lbl_eu_8066AAB8)
            result = true;
    }
    return result;
}
extern "C" void* Scn_GetUnk80Resource(u8* self) {
    return isFadeSettled(((CScnMoveView*)self)->fadeMan);
}
extern "C" int Scn_GetCamWorkInt(void* _this) {
    return CamMan_SwapViewParam_AED4((int)((CScn*)_this)->mCamWork);
}
// Fetches the camera item for `id` and copies its +0x194 projection matrix
// (MTX44, 16 words) into dest. Per-slot coloring recipe (MWCC_CASES.md
// LODMemMan rule): the hi-word temp is DECLARED first (colors r0 like
// retail) while reads/stores run lo-word first (pins emission order); each
// slot uses a fresh declaration pair so slots 1..7 keep the same colors.
extern "C" void Scn_CopyCamProjMatrix(CScn* self, ScnCamParams* dest, s32 id) {
    // Two-arg camera lookup (the TU-wide decl above is the 1-arg form kept
    // for the already-matched single-arg callers).
    extern ScnCamItemView* CamMan_FindItemA_B158(void* camWork, s32 id);
    ScnCamItemView* item = CamMan_FindItemA_B158(self->mCamWork, id);
    // Decl order colors the temps (hi -> r0, lo -> r4 like retail); the
    // read/store order below pins the emission order (lo word first).
    u32 w1;
    u32 w0 = item->proj[0];
    w1 = item->proj[1];
    dest->w[1] = w1;
    dest->w[0] = w0;
    {
        u32 h1;
        u32 l1 = item->proj[2];
        h1 = item->proj[3];
        dest->w[3] = h1;
        dest->w[2] = l1;
    }
    {
        u32 h2;
        u32 l2 = item->proj[4];
        h2 = item->proj[5];
        dest->w[5] = h2;
        dest->w[4] = l2;
    }
    {
        u32 h3;
        u32 l3 = item->proj[6];
        h3 = item->proj[7];
        dest->w[7] = h3;
        dest->w[6] = l3;
    }
    {
        u32 h4;
        u32 l4 = item->proj[8];
        h4 = item->proj[9];
        dest->w[9] = h4;
        dest->w[8] = l4;
    }
    {
        u32 h5;
        u32 l5 = item->proj[10];
        h5 = item->proj[11];
        dest->w[11] = h5;
        dest->w[10] = l5;
    }
    {
        u32 h6;
        u32 l6 = item->proj[12];
        h6 = item->proj[13];
        dest->w[13] = h6;
        dest->w[12] = l6;
    }
    {
        u32 h7;
        u32 l7 = item->proj[14];
        h7 = item->proj[15];
        dest->w[15] = h7;
        dest->w[14] = l7;
    }
}
// Payload view of the camera item handed out by CamMan_FindItemB_B1CC:
// Scn_GetCamItem9C/CC return the +0x9C/+0xCC words as untyped payloads.
struct ScnCamItemPayloadView {
    u8 _00[0x9C];         // 0x00..0x9C
    void* item9C;         // 0x9C Scn_GetCamItem9C result
    u8 _A0[0xCC - 0xA0];  // 0xA0..0xCC
    void* itemCC;         // 0xCC Scn_GetCamItemCC result
};
// Get the camera item handle, lazily creating id -1 when missing; returns
// the item payload +0x9C (the sibling +0xCC variant mirrors it).
extern "C" void* CamMan_FindItemB_B1CC(void*);
extern "C" void* Scn_GetCamItem9C(void* self) {
    void* r = CamMan_FindItemB_B1CC(((CScn*)self)->mCamWork);
    if (!r)
        r = ((void* (*)(void*, int))CamMan_FindItemB_B1CC)(((CScn*)self)->mCamWork, -1);
    return (void*)&((ScnCamItemPayloadView*)r)->item9C;
}

// Sibling with the +0xCC payload offset.
extern "C" void* Scn_GetCamItemCC(void* self) {
    void* r = CamMan_FindItemB_B1CC(((CScn*)self)->mCamWork);
    if (!r)
        r = ((void* (*)(void*, int))CamMan_FindItemB_B1CC)(((CScn*)self)->mCamWork, -1);
    return (void*)&((ScnCamItemPayloadView*)r)->itemCC;
}
extern "C" int Scn_FindCamItem(void* _this) {
    return CamMan_FindItemA_B158((int)((CScn*)_this)->mCamWork);
}
extern "C" bool Scn_HasCamItem(void* _this) {
    return CamMan_FindViewItemA((int)((CScn*)_this)->mCamWork);
}
extern "C" void* Scn_GetCamList(void* _this) {
    return CamMan_FindViewItemB(((CScn*)_this)->mCamWork);
}
extern "C" void Scn_SetCamIndex(u8* self, int arg) { CamMan_GetViewThread(((CScn*)self)->mCamWork, (short)arg); }
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
        mul = lbl_eu_8066AABC;
    else
        mul = lbl_eu_8066AAB8;
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
extern "C" void Scn_CallUnk68_V3(u8* self) {
    VTarget* obj = (VTarget*)((CScn*)self)->mCamWork;
    obj->v3();
}
extern "C" void Scn_CallUnk60_V4(u8* self) {
    VTarget* obj = (VTarget*)((CScnMoveView*)self)->itemPool;
    obj->v4();
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
// +0x7C subobject is destroyed via a direct destructor call instead.
void CScn::Term() {
    CScnTermView* v = (CScnTermView*)this;
    if (v->unk80 != NULL) {
        if (v->unk80 != NULL) {  // twin guard matches retail's paired beq
            ((ScnTermRelease*)v->unk80)->release(1);
        }
        v->unk80 = NULL;
    }
    if (v->unk70 != NULL) {
        if (v->unk70 != NULL) {
            ((ScnTermRelease*)v->unk70)->release(1);
        }
        v->unk70 = NULL;
    }
    if (v->unk6C != NULL) {
        if (v->unk6C != NULL) {
            ((ScnTermRelease*)v->unk6C)->release(1);
        }
        v->unk6C = NULL;
    }
    if (v->unk74 != NULL) {
        if (v->unk74 != NULL) {
            ((ScnTermRelease*)v->unk74)->release(1);
        }
        v->unk74 = NULL;
    }
    if (v->unk64 != NULL) {
        if (v->unk64 != NULL) {
            ((ScnTermRelease*)v->unk64)->release(1);
        }
        v->unk64 = NULL;
    }
    if (v->unk78 != NULL) {
        if (v->unk78 != NULL) {
            ((ScnTermRelease*)v->unk78)->release(1);
        }
        v->unk78 = NULL;
    }
    // Direct destructor call, not a virtual dispatch.
    if (v->unk7C != NULL) {
        __dt__804C0E48(v->unk7C, 1);
        v->unk7C = NULL;
    }
    if (v->unk68 != NULL) {
        if (v->unk68 != NULL) {
            ((ScnTermRelease*)v->unk68)->release(1);
        }
        v->unk68 = NULL;
    }
    if (v->unk5C != NULL) {
        if (v->unk5C != NULL) {
            ((ScnTermRelease*)v->unk5C)->release(1);
        }
        v->unk5C = NULL;
    }
    if (v->unk60 != NULL) {
        if (v->unk60 != NULL) {
            ((ScnTermRelease*)v->unk60)->release(1);
        }
        v->unk60 = NULL;
    }
    if (v->unk84 != NULL) {
        if (v->unk84 != NULL) {
            ((ScnTermRelease*)v->unk84)->release(1);
        }
        v->unk84 = NULL;
    }
    if (v->unk8C != NULL) {
        if (v->unk8C != NULL) {
            ((ScnTermRelease*)v->unk8C)->release(1);
        }
        v->unk8C = NULL;
    }
}
// CScn::Move(). The literal retail symbol name is preserved via C linkage
// (the public CScn class header is outside this session's writable scope).
// Runs every scene subsystem's per-frame update, gated by the pause/stop flags.
extern "C" void Move__4CScnFv(CScnMoveView* self) {
    if (self->unk3E4 == 0 && self->unk3E7 == 0) {
        self->frame->update();
        scnLgtStepLightItems(self->lgtCtrl);
        self->itemPool->update();
        self->filterMan->update();
        self->fadeMan->update();
        self->unk8C->vf14();
        self->itemPool->updateItemD0();
    }
    self->unk3E8 = 0;
}
// Scene factory. Retail exports it under the flat, unmangled symbol
// create__8CScnNw4rFv (the public header declares it parameterless even
// though every caller passes five arguments), so the definition keeps the
// flat name via C linkage.
class CScnNw4r {
public:
    // Flat-name ctor emitted by the CScnMem TU (retail calls __ct__CScnNw4r).
};

// Flat declarations so the emitted relocations carry the retail names.
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent,
                                              bool top);
extern "C" void initSubsystems__4CScnFUlUlPvUl(CScn* self, u32 a, u32 b,
                                              void* str, u32 d);
extern "C" void* __ct__CScnNw4r(void* self);

extern "C" CScnNw4r* create__8CScnNw4rFv(CProcess* parent, char* name,
                                         u32 arg, u32 handle, void* param) {
    // Inline clear(): mString[0] = 0, mLength = 0 (retail stores, no
    // out-of-line FixStr ctor call).
    ml::FixStr<32> str(true);
    str.format(lbl_eu_80524090, lbl_eu_80524090 + 7, name);

    // Default handle sentinel: fall back to the MEM1 region handle.
    if ((u32)(handle + 0x10000) == 0xffff) {
        handle = mtl::MemManager::getHandleMEM1();
    }

    CScnNw4r* scn = static_cast<CScnNw4r*>(mtl::MemManager::allocate(
        sizeof(CScn), CWorkThreadSystem::getWorkMem()));
    if (scn != NULL) {
        scn = (CScnNw4r*)__ct__CScnNw4r(scn);
    }

    Regist__8CProcessFP8CProcessb((CProcess*)scn, parent, false);
    initSubsystems__4CScnFUlUlPvUl((CScn*)scn, handle, arg, &str, (u32)param);
    return scn;
}


