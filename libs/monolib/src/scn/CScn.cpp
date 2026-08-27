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
    virtual void v9() = 0;
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

    u32* move = (u32*)&v->mMoveFunc;
    u32* draw = (u32*)&v->mDrawFunc;
    const u32* n = __ptmf_null;
    u32 m0 = *n++;
    u32 m1 = *n++;
    f32 f98 = lbl_eu_8066AAB0;
    move[1] = m1;
    move[0] = m0;
    move[2] = *n++;
    // Second copy re-reads from the null-ptmf base.
    n = __ptmf_null;
    u32 d0 = *n++;
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
    ((u8*)self)[0x3E9] = 0;
    return self;
}

// Retail thunk: the IWorkEvent subobject at +0x54 delegates back to the
// complete-object destructor with this-0x54.
extern "C" void* __dt__4CScnFv(CScn* self, int flags);
extern "C" void func_80496B04(void* pThis, int r4) {
    __dt__4CScnFv((CScn*)((char*)pThis - 0x54), r4);
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
    CScn* p = this;
    u32 i = 0;

    for (; count > 0; count--) {
        if (p->mRenderCBs[0].cb == cb) {
            CScn* slot = (CScn*)((u8*)this + i * sizeof(ScnRenderCB));
            for (; (s32)i < (s32)(mRenderCBCount - 1);) {
                IScnRender* nextCb = slot->mRenderCBs[1].cb;
                i++;
                slot->mRenderCBs[0].cb = nextCb;
                slot->mRenderCBs[0].prio = slot->mRenderCBs[1].prio;
                slot->mRenderCBs[0].flag = slot->mRenderCBs[1].flag;
                slot = (CScn*)((u8*)slot + sizeof(ScnRenderCB));
            }
            mRenderCBCount = mRenderCBCount - 1;
            return;
        }
        p = (CScn*)((u8*)p + sizeof(ScnRenderCB));
        i++;
    }
}

// CScn::Init() = empty (retail blr)
void CScn::Init() {}

void CScn::Draw() {
    if (func_8049AF80(mCamWork) != 0) {
        UnkScn68* cam = mCamWork;
        CView* view = func_8049B0A0(cam, (s16)cam->unk34);
        if (view == CView::getCurrentView()) {
            if (unk_3E4 == 0) {
                lbl_eu_80665908 = this;

                if (unk_3E5 != 0) {
                    if (unk_3E6 != 0) {
                        mUnk8C->vf18();
                    }
                }

                CScn* p = this;
                s32 i = 0;
                for (; i < (s32)mRenderCBCount;) {
                    if (unk_3E5 != 0 || p->mRenderCBs[0].flag != 0) {
                        reinterpret_cast<IScnRenderDraw*>(p->mRenderCBs[0].cb)->onRender(this);
                    }
                    p = (CScn*)((u8*)p + sizeof(ScnRenderCB));
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


extern "C" bool func_80495E60(u8* self) {
    extern bool func_8048C8C4(void*, void*);
    if (self == 0) {
        return 1;
    }
    return func_8048C8C4(*(void**)((char*)*(void**)(self + 4) + 0x60), self);
}
extern "C" bool func_80495E84(u8* self) {
    extern bool func_8048CB14(void*);
    return func_8048CB14(*(void**)((char*)self + 0x60));
}
extern "C" void* func_80495E8C(void* a, void* b, void* c, void* d) {
    return func_80486068(a, b, c, d, 0);
}
extern "C" void func_80495E94(s32 param_1, s32 param_2) {
    func_80486068((void*)param_1, (void*)param_2, (void*)-1, (void*)0, 0);
}
extern "C" int func_80495EA4(int a, int b) {
    return func_80486070(a, b, 8);
}
extern "C" void func_80495EAC() {
    __ct__8049E710();
}
extern "C" void func_80495EB0() {
    __ct__804820F8();
}
extern "C" void func_80495FC8(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v6();
}
extern "C" void func_80495FDC(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v7();
}
extern "C" void func_80495FF0(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v9();
}
extern "C" void func_80496004(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v10();
}
extern "C" void func_80496018(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 140);
    obj->v8();
}
extern "C" void func_8049602C(void* p) {
    func_8049C72C(*(int*)((u8*)p + 0x80));
}
extern "C" void* func_80496034(void* _this) {
    return func_8049C794(*(void**)((char*)_this + 0x80));
}
extern "C" u32 func_8049603C(u8* self) {
    extern u32 func_8049C7A8(u32);
    return func_8049C7A8(*(u32*)((char*)self + 0x80));
}
extern "C" bool func_80496044(u8* self) {
    extern void* func_8049C7A8(void*);
    bool result = false;
    if (func_8049C794(*(void**)((u8*)self + 0x80))) {
        if (((ScnFloats*)func_8049C7A8(*(void**)((u8*)self + 0x80)))->unk0C == lbl_eu_8066AAB4)
            result = true;
    }
    return result;
}
// Same shape as func_80496044 but an ordered >= compare against 1.0f.
extern "C" bool func_804960A8(u8* self) {
    extern void* func_8049C7A8(void*);
    bool result = false;
    if (func_8049C794(*(void**)((u8*)self + 0x80))) {
        if (((ScnFloats*)func_8049C7A8(*(void**)((u8*)self + 0x80)))->unk0C >= lbl_eu_8066AAB8)
            result = true;
    }
    return result;
}
extern "C" void* func_80496110(u8* self) {
    return func_8049C7B0(*(void**)((char*)self + 0x80));
}
extern "C" int func_80496118(void* _this) {
    return func_8049AED4(*(int*)((char*)_this + 0x68));
}
// Fetches the camera item for `id` and copies its +0x194 projection matrix
// (MTX44, 16 words) into dest. Per-slot coloring recipe (MWCC_CASES.md
// LODMemMan rule): the hi-word temp is DECLARED first (colors r0 like
// retail) while reads/stores run lo-word first (pins emission order); each
// slot uses a fresh declaration pair so slots 1..7 keep the same colors.
extern "C" void func_80496120(CScn* self, ScnCamParams* dest, s32 id) {
    // Two-arg camera lookup (the TU-wide decl above is the 1-arg form kept
    // for the already-matched single-arg callers).
    extern ScnCamItemView* func_8049B158(void* camWork, s32 id);
    ScnCamItemView* item = func_8049B158(self->mCamWork, id);
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
// Get the camera item handle, lazily creating id -1 when missing; returns
// the item payload +0x9C (the sibling +0xCC variant mirrors it).
extern "C" void* func_8049B1CC(void*);
extern "C" void* func_804961D4(void* self) {
    void* r = func_8049B1CC(*(void**)((u8*)self + 0x68));
    if (!r)
        r = ((void* (*)(void*, int))func_8049B1CC)(*(void**)((u8*)self + 0x68), -1);
    return (u8*)r + 0x9C;
}

// Sibling with the +0xCC payload offset.
extern "C" void* func_8049621C(void* self) {
    void* r = func_8049B1CC(*(void**)((u8*)self + 0x68));
    if (!r)
        r = ((void* (*)(void*, int))func_8049B1CC)(*(void**)((u8*)self + 0x68), -1);
    return (u8*)r + 0xCC;
}
extern "C" int func_80496264(void* _this) {
    return func_8049B158(*(int*)((char*)_this + 0x68));
}
extern "C" bool func_8049626C(void* _this) {
    return func_8049B240(*(int*)((char*)_this + 0x68));
}
extern "C" void* func_80496274(void* _this) {
    return func_8049B2C4(*(void**)((char*)_this + 0x68));
}
extern "C" void func_8049627C(u8* self, int arg) { func_8049B0A0(*(UnkScn68**)((char*)self + 0x68), (short)arg); }
// Scene time scale: PAL consoles without the speed fix run at 1.2x.
float func_80496288(void* self_) {
    u8* self = (u8*)self_;
    int pal = 0;
    if (CDeviceVI::isTvFormatPal()) {
        if (self[0x3E9] == 0)
            pal = 1;
    }
    float mul;
    if (pal != 0)
        mul = lbl_eu_8066AABC;
    else
        mul = lbl_eu_8066AAB8;
    ScnFloats* p = (ScnFloats*)*(void**)((u8*)self + 0x84);
    float scale = p->unk08;
    return scale * mul;
}
// retail: lwz r3,0x84(r3); stfs f1,0xC(r3); blr
void func_80496294(CScn* self, float v) {
    ((ScnFloats*)*(void**)((u8*)self + 0x84))->unk0C = v;
}
extern "C" void func_804962A0(void* p, unsigned char v) {
    ((unsigned char*)p)[0x3e5] = v;
}
// retail: stb r4,0x3e6(r3); blr
extern "C" void func_804962A8(u8* self, u8 v) { self[0x3E6] = v; }
extern "C" void func_8049695C(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 104);
    obj->v3();
}
extern "C" void func_80496970(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 96);
    obj->v4();
}
extern "C" void func_80496984(u32 value) {
    lbl_eu_80665908 = (CScn *)value;
}
extern "C" int func_8049698C() {
    return (int)lbl_eu_80665908;
}
extern "C" void func_8049B3FC();
extern "C" void func_8049B408();
void func_80496994__Fv(void) { func_8049B3FC(); }
void func_80496998__Fv(void) { func_8049B408(); }
// Virtual dispatch target: v_i at vtable offset 8+4*i (MWCC RTTI header).


extern "C" void func_eu_8049AB50(u8* self, unsigned char byte) {
    *(unsigned char*)((uintptr_t)self + 0x3e9) = byte;
}

extern "C" void func_8049699C(u8* self) {
    VTarget* obj = (VTarget*)*(void**)((u8*)self + 0x8C);
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
        func_804C12A4(self->lgtCtrl);
        self->itemPool->update();
        self->filterMan->update();
        self->fadeMan->update();
        self->unk8C->vf14();
        self->itemPool->func_8048CEDC();
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
extern "C" void func_804962B0__4CScnFUlUlPvUl(CScn* self, u32 a, u32 b,
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
    func_804962B0__4CScnFUlUlPvUl((CScn*)scn, handle, arg, &str, (u32)param);
    return scn;
}


