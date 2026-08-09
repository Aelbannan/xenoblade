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

// ---------------------------------------------------------------------------
// Target 3: func_80294638 - input handling for the save dialog's open state
// (called from Move while mFlagDC == 2). Reads the current cf pad data; the
// two confirm/cancel triggers come from different button bits depending on
// co-op vs single-player; an up/down D-pad input wraps the save-slot flag.
// ---------------------------------------------------------------------------
void func_80294638(CSysWinSave* self);

void func_80294638(CSysWinSave* self) {
    CSysPadData* pad = (CSysPadData*)getCfPadData__Q22cf13CfGameManagerFv();
    int in1, in2;
    int sels, confirm;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        in1     = (pad->field_04 >> 10) & 1;
        in2     = (pad->field_04 >> 9) & 1;
        confirm = ((pad->field_104 >> 16) & 1) != 0 || ((pad->field_104 >> 3) & 1) != 0;
        sels    = (pad->field_104 & 0x8004) != 0;
    } else {
        in1     = (pad->field_04 >> 27) & 1;
        in2     = (pad->field_04 >> 26) & 1;
        confirm = ((pad->field_104 >> 16) & 1) != 0 || ((pad->field_104 >> 3) & 1) != 0;
        sels    = (pad->field_104 & 0x8004) != 0;
    }

    if (in1) {
        // Close the dialog without advancing the save slot.
        self->mFlagDC = 3;
        func_8022B8E4(&self->mSysWin[0]);
        func_801D216C(&self->mCur18[0], 0);
        func_80138078__FUl(3);
    } else if (in2) {
        // Close the dialog, marking the save slot as re-selected.
        self->mFlagDC = 3;
        func_8022B8E4(&self->mSysWin[0]);
        func_801D216C(&self->mCur18[0], 0);
        self->mFlagDD = 1;
        func_80138078__FUl(6);
    } else if (sels) {
        // Cursor up: decrement the slot index, wrapping to 1 when below 0.
        self->mFlagDD = self->mFlagDD - 1;
        if (self->mFlagDD < 0) self->mFlagDD = 1;
        u8 tmp[0x18];
        func_8022C1B4(tmp, &self->mSysWin[0], self->mFlagDD);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        func_80138078__FUl(1);
    } else if (confirm) {
        // Cursor down: increment the slot index, wrapping over 1 to 0.
        self->mFlagDD = self->mFlagDD + 1;
        if (self->mFlagDD > 1) self->mFlagDD = 0;
        u8 tmp[0x18];
        func_8022C1B4(tmp, &self->mSysWin[0], self->mFlagDD);
        reinterpret_cast<CCur18View*>(&self->mCur18[0])->vf04(tmp);
        func_80138078__FUl(1);
    }
}

// ---------------------------------------------------------------------------
// Target 1: CSysWinSave::Init - register the render callback, create the
// scratch region (guarded by a Class_8045F858 RAII temp), then rebuild the
// embedded CSysWin and CCur18 bodies by constructing stack temps and copying
// their non-vtable fields into the embedded storage.
// ---------------------------------------------------------------------------
void CSysWinSave::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(true);

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mScnRender);
    mScene->addRenderCB(render, 0xd, 0);

    mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x4000,
                            lbl_eu_8050FD6C, 0);

    // RAII scratch-region guard, released at the end of this scope.
    Class_8045F858 guard(&mMemRegion);

    // Rebuild the embedded CSysWin: construct a temp (+2), copy its body
    // (non-vtable fields, matching retail's memberwise lwz/stw+lbz/stb), then
    // destroy the temp and dispatch the layout-build virtual at vtable+0x88.
    u8 tempW[0x3C];
    __ct__CSysWin(tempW, 2);
    CSysWinData* sw = reinterpret_cast<CSysWinData*>(&mSysWin[0]);
    CSysWinData* tw = reinterpret_cast<CSysWinData*>(tempW);
    sw->f_04 = tw->f_04;
    sw->f_08 = tw->f_08;
    sw->f_0c = tw->f_0c;
    sw->f_10 = tw->f_10;
    sw->f_14 = tw->f_14;
    sw->f_18 = tw->f_18;
    sw->f_1c = tw->f_1c;
    sw->f_20 = tw->f_20;
    sw->f_24 = tw->f_24;
    sw->f_28 = tw->f_28;
    sw->f_2c = tw->f_2c;
    sw->f_30 = tw->f_30;
    sw->f_34 = tw->f_34;
    sw->f_35 = tw->f_35;
    sw->f_36 = tw->f_36;
    sw->f_37 = tw->f_37;
    sw->f_38 = tw->f_38;
    sw->f_39 = tw->f_39;
    __dt__7CSysWinFv(tempW, -1);
    reinterpret_cast<CSysWinView*>(&mSysWin[0])->v20();

    // Rebuild the embedded CCur18 cursor the same way (shared arc accessor).
    u8 tempC[0x18];
    __ct__CCur18(tempC, (void*)func_801355F4());
    CCur18Data* mc = reinterpret_cast<CCur18Data*>(&mCur18[0]);
    CCur18Data* tc = reinterpret_cast<CCur18Data*>(tempC);
    mc->f_04 = tc->f_04;
    mc->f_08 = tc->f_08;
    mc->f_0c = tc->f_0c;
    mc->f_10 = tc->f_10;
    mc->f_14 = tc->f_14;
    mc->f_15 = tc->f_15;
    __dt__6CCur18Fv(tempC, -1);
    reinterpret_cast<CCur18View*>(&mCur18[0])->vf02();

    mMemRegion.func_8045F810();
}

// ---------------------------------------------------------------------------
// Target 2: CSysWinSave::Move - gate prefix (task busy / global render flag /
// scene active), then a 4-state machine controlling the save dialog:
//   0 load strings and ready the window; 1 wait active then update cursor;
//   2 dispatch input to func_80294638; 3 closing transition.
// ---------------------------------------------------------------------------
void CSysWinSave::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;

    switch (mFlagDC) {
    case 0: {
        // Window just became ready: load the three save-panel messages and
        // start the open animation.
        if (CSysWin_isReady(&mSysWin[0])) {
            mFlagDC = 1;
            const char* base = lbl_eu_8050FD6C;
            const char* s1 = func_80136190(base + 0xc, base + 0x17, 0x4e);
            const char* s2 = func_80136190(base + 0xc, base + 0x17, 0x4f);
            const char* s3 = func_80136190(base + 0xc, base + 0x17, 0x50);
            func_8022B9B4(&mSysWin[0], (void*)s1, 0);
            func_8022BF6C(&mSysWin[0], (void*)s2, (void*)s3);
            func_8022BFC8(&mSysWin[0], 0);
            func_8022B8B8(&mSysWin[0]);
        }
        break;
    }
    case 1:
        // Window active: update the cursor from the stored slot flag.
        if (CSysWin_isActive(&mSysWin[0])) {
            mFlagDC = 2;
            func_801D216C(&mCur18[0], 1);
            u8 tmp[0x18];
            func_8022C1B4(tmp, &mSysWin[0], mFlagDD);
            reinterpret_cast<CCur18View*>(&mCur18[0])->vf04(tmp);
        }
        break;
    case 2:
        // Input handling for the open dialog.
        func_80294638(this);
        break;
    case 3:
        // Closing transition finished - hand control back to the UI.
        if (CSysWin_isActive(&mSysWin[0])) {
            if (mFlagDD == 0)
                func_80134B50(1, 1);
            field_67 = 0;
            field_64 = 1;
        }
        break;
    }

    func_8022B748(&mSysWin[0]);
    func_801D202C(&mCur18[0]);
}

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