// kyoshin/CSysWinSave - system window "save" process (singleton).

#include "kyoshin/CSysWinSave.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <new>
#include <revolution/gx/GXPixel.h>

// Global-view declaration so the catalog thunks can address the member render
// callback by its retail symbol name (the definition below emits that symbol).
void cbRenderBefore__11CSysWinSaveFv(void* self);

// ---------------------------------------------------------------------------
// VTable-compatible thunks adjacent to the CSysWinSave subobjects (retail
// emits these as offset-adjusted dispatch stubs: only r3 is re-based, then a
// tail jump into the right subobject trampoline). Preserved from the scaffold.
// ---------------------------------------------------------------------------
void func_8029480C(void* self) { ((void(*)(void*))__dt__11CSysWinSaveFv)((char*)self - 0x6c); }

void func_80294814(void* self) { ((void(*)(void*))cbRenderBefore__11CSysWinSaveFv)((char*)self - 0x70); }

void func_8029481C(void* self) { ((void(*)(void*))__dt__11CSysWinSaveFv)((char*)self - 0x70); }

extern "C" void func_80294824__FPv(float* self) {
    float v = lbl_eu_80668BA0;
    self[0] = v;
    self[1] = v;
}

extern "C" void func_80294834__FPv(float* self) {
    float v = lbl_eu_80668BA0;
    self[0] = v;
    self[1] = v;
}

// ---------------------------------------------------------------------------
// Target 1: func_80294844 (us-80296e40)
// Write a base value to [out+0] and [out+4]. If the incoming parameter is
// below the threshold A, additionally store a corrected value:
//   corrected = C * (param - A) - A
// to both slots (the whole point: clamp/recentre toward a target).
// ---------------------------------------------------------------------------
extern "C" void func_80294844(float* self, float param) {
    float b = lbl_eu_80668BA0;
    float a = lbl_eu_80668BA4;
    self[0] = b;
    self[1] = b;
    if (a < param) {
        param -= a;
        float c = lbl_eu_80668BA8;
        float r = (c * param) - a;
        self[1] = r;
        self[0] = r;
    }
}

// ---------------------------------------------------------------------------
// Producers of the save-window singleton. Kept as scaffold stubs (not matching
// targets in this batch); must still emit their retail symbols.
// ---------------------------------------------------------------------------
extern "C" unsigned long func_80294624() {
    return lbl_eu_80664A08 != 0;
}

extern "C" void func_80294638() {}

extern "C" void Init__11CSysWinSaveFv() {}
extern "C" void Move__11CSysWinSaveFv() {}

// ---------------------------------------------------------------------------
// Target 2: CSysWinSave::Term (us-802967ec)
// Wait for VI draw completion, release the CSysWin and its scratch region, run
// the CCur18 cursor-update virtual, clear the singleton, detach the render
// callback, and re-enable the cf load if the signed flag was set.
// ---------------------------------------------------------------------------
void CSysWinSave::Term() {
    CDeviceVI::waitForDrawDone();
    func_8022B7F4(&mSysWin[0]);
    reinterpret_cast<CCursor18*>(&mCur18[0])->vf3();
    mMemRegion.func_8045F778();
    lbl_eu_80664A08 = 0;

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->removeRenderCB(render);

    if (mFlagDD != 0)
        func_8008294C__Q22cf13CfGameManagerFv(false);
}

// ---------------------------------------------------------------------------
// Target 3: CSysWinSave::~CSysWinSave (us-802965b4)
// Complete-object destructor. Subobjects destroyed in reverse construction
// order: CCur18@0xC4, CSysWin@0x88, UnkClass_8045F564@0x78, then the CProcess
// base (guarded by the nested double null-check, an MWCC D2-inlined-into-D1
// artifact), then conditional operator delete when flags > 0.
// ---------------------------------------------------------------------------
extern "C" void* __dt__11CSysWinSaveFv(CSysWinSave* _this, int flags) {
    if (_this != 0) {
        __dt__6CCur18Fv(&_this->mCur18[0], -1);
        __dt__7CSysWinFv(&_this->mSysWin[0], -1);
        __dt__17UnkClass_8045F564Fv(&_this->mMemRegion, -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv(_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// ---------------------------------------------------------------------------
// Target 4: CSysWinSave::cbRenderBefore (us-80296a3c)
// Gate prefix (task busy / global bit / scene-active) then draw the embedded
// CSysWin and CCur18 cursor through a stack DrawInfo.
// ---------------------------------------------------------------------------
void CSysWinSave::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (!func_8013BE50()) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    func_80137250(&drawInfo);
    func_8022B7C8(&mSysWin[0], &drawInfo);
    func_801D20B0(&mCur18[0], &drawInfo);
    drawInfo.~DrawInfo();
}

// ---------------------------------------------------------------------------
// Target 5: __ct__802944D8 (us-80296ad4)
// Allocate the CSysWinSave singleton (0xE0 bytes) from work memory, run the
// CProcess ctor, lay out the IUIWindow + Move region by hand, then construct
// the embedded UnkClass_8045F564 / CSysWin / CCur18 and register as a CProcess
// child of `registerParent`. Returns 0 when the singleton already exists.
// Regist is called even when the allocation/ctor failed, matching retail.
// ---------------------------------------------------------------------------
extern "C" CSysWinSave* __ct__802944D8(CProcess* registerParent, CScn* scene) {
    if (lbl_eu_80664A08 != 0)
        return 0;

    mtl::ALLOC_HANDLE workMem = CWorkThreadSystem::getWorkMem();
    CSysWinSave* obj = (CSysWinSave*)mtl::MemManager::allocate(0xe0, workMem);
    if (obj != 0) {
        __ct__8CProcessFv((CProcess*)obj);

        // IUIWindow vtable at +0x10 is written twice (temp vtable, then the
        // CSysWinSave composite vtable); the two null ptmf callback slots are
        // copied, then the scalar fields are initialised.
        *(u32*)((u8*)obj + 0x10) = (u32)lbl_eu_8052D238;
        obj->ptmf0[0] = __ptmf_null[0];
        obj->ptmf0[1] = __ptmf_null[1];
        obj->ptmf0[2] = __ptmf_null[2];
        obj->ptmf1[0] = __ptmf_null[0];
        obj->ptmf1[1] = __ptmf_null[1];
        obj->ptmf1[2] = __ptmf_null[2];

        obj->field_54 = 0;
        obj->field_58 = 0;
        obj->field_5C = 0;
        obj->field_60 = -1;
        obj->field_64 = 0;
        obj->field_65 = 0;
        obj->field_66 = 0;
        obj->field_67 = 1;
        obj->field_68 = 0;

        *(u32*)((u8*)obj + 0x10) = (u32)lbl_eu_805389A0;
        obj->mWorkEvent = (u32)lbl_eu_805389A0 + 0x24;
        obj->mScnRender = (u32)lbl_eu_805389A0 + 0xac;
        obj->mScene = scene;

        __ct__17UnkClass_8045F564Fv(&obj->mMemRegion);
        __ct__CSysWin(&obj->mSysWin[0], 0);
        __ct__CCur18(&obj->mCur18[0], 0);

        obj->mFlagDC = 0;
        obj->mFlagDD = 0;
    }
    lbl_eu_80664A08 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(registerParent, 0);
    return lbl_eu_80664A08;
}