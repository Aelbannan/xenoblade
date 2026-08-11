// Decomp of kyoshin/menu/CMenuKizunaTalk.
// Kizuna (friendship) talk screen: singleton process with IUIWindow +
// IWorkEvent + IScnRender bases, an embedded CCur18 cursor and CSysWin panel.

#include "kyoshin/menu/CMenuKizunaTalk.hpp"

#include "functions.hpp"

#include "kyoshin/CItemBoxGrid.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

extern "C" void __dt__15CMenuKizunaTalkFv(void*, int);
extern "C" void cbRenderBefore__15CMenuKizunaTalkFv(void*);

// ---------------------------------------------------------------------------
// Constructor (retail symbol is the unmangled __ct__CMenuKizunaTalk, so the
// body is written as a C-linkage free function; the factory func_801BCEBC
// calls it). Runs the CProcess base ctor, stores the composite vtable and the
// IWorkEvent / IScnRender subobject vtables, then constructs the embedded
// memory region and cursor; the remaining scalar state gets retail defaults.
// ---------------------------------------------------------------------------
extern "C" CMenuKizunaTalk* __ct__CMenuKizunaTalk(CMenuKizunaTalk* obj,
                                                  CScn* scene, u32 charId) {
    __ct__8CProcessFv(obj);

    obj->mField10 = (u32)lbl_eu_8052D238;
    obj->mLayout = 0;
    obj->mField58 = 0;
    obj->mField5C = 0;
    obj->mField60 = -1;
    obj->mField64 = 0;
    obj->mField65 = 0;
    obj->mField66 = 0;
    obj->mField67 = 1;
    obj->mField68 = 0;

    obj->mField10 = (u32)lbl_eu_80533A98;
    // Subobject vtable slots (composite vtable + 0x24 / + 0xAC).
    u32 workVt = (u32)lbl_eu_80533A98 + 0x24;
    u32 renderVt = (u32)lbl_eu_80533A98 + 0xAC;
    static_cast<IWorkEvent*>(obj)->mVtable = workVt;
    static_cast<IScnRender*>(obj)->mVtable = renderVt;
    obj->mScene = scene;

    __ct__17UnkClass_8045F564Fv((UnkClass_8045F564*)&obj->mMemRegion[0]);
    obj->mFile1 = 0;
    obj->mFile2 = 0;
    obj->mArcAccessor = 0;
    obj->mAnim = 0;
    obj->mCharId = charId;
    obj->mField9C = 1;
    obj->mFieldA0 = 0;
    obj->mFieldA4 = 0;

    __ct__CCur18(&obj->mCur[0], 0);
    return obj;
}

// ---------------------------------------------------------------------------
// Destructor: the CSysWin / CCur18 / UnkClass_8045F564 regions are raw
// storage whose retail dtors are C-linkage free functions, so they are
// destroyed by hand in reverse construction order; the IUIWindow base dtor
// (__dt__9IUIWindowFv) is emitted automatically after the body.
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
CMenuKizunaTalk::~CMenuKizunaTalk() {
    __dt__7CSysWinFv(&mSysWin[0], -1);
    __dt__6CCur18Fv(&mCur[0], -1);
    __dt__17UnkClass_8045F564Fv(&mMemRegion[0], -1);
}
#pragma optimize_for_size off

// Initialise the talk screen: request the shared archive (the loader gets the
// IWorkEvent subobject at +0x6C via the base conversion), then set the
// character's affinity-talk-seen BDAT flag.
#pragma optimize_for_size on
void CMenuKizunaTalk::Init() {
    mFile2 = (CFileHandle*)readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(
        func_800A9D90(), lbl_eu_80505118, static_cast<IWorkEvent*>(this), 0, 0);
    func_8009D018(mCharId + 0x3440, 1);
}
#pragma optimize_for_size off

// Tear the talk screen down: release the file handles and the layout archive,
// remove the render callback, run the embedded widget teardowns and clear the
// singleton / BDAT globals.
#pragma optimize_for_size on
void CMenuKizunaTalk::Term() {
    waitForDrawDone__9CDeviceVIFv();
    func_8003AA8C__5CBdatFUl(2);
    func_801390E0__FPP11CFileHandle(reinterpret_cast<void**>(&mFile1));
    func_801390E0__FPP11CFileHandle(reinterpret_cast<void**>(&mFile2));

    mScene->removeRenderCB(this);

    if (mLayout != 0) {
        // Layout virtual slot 2 (vtable + 0x8): release with flag 1.
        u32 fn = (*(u32**)mLayout)[0x08 / 4];
        ((void (*)(nw4r::lyt::Layout*, int))fn)(mLayout, 1);
        mLayout = 0;
    }

    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(mArcAccessor);
    mArcAccessor = 0;
    func_8022B7F4(&mSysWin[0]);

    // CCur18 cursor virtual slot 3 (vtable + 0xC): per-frame update.
    u32 curVt = *(u32*)&mCur[0];
    u32 curFn = (*(u32**)curVt)[0x0C / 4];
    ((void (*)(u8*))curFn)(&mCur[0]);

    lbl_eu_80664424 = 0;
    func_8045F778__17UnkClass_8045F564Fv(&mMemRegion[0]);
    lbl_eu_80664420 = 0;
}
#pragma optimize_for_size off

void CMenuKizunaTalk::Move() {}

void CMenuKizunaTalk::cbRenderBefore() {}

// ---------------------------------------------------------------------------
// Factory: lazily allocate + construct the single kizuna talk instance from
// work memory and register it under `parent`. Returns 0 when the singleton
// already exists.
// ---------------------------------------------------------------------------
#pragma optimize_for_size on
CMenuKizunaTalk* func_801BCEBC(CProcess* parent, CScn* scene, u32 charId) {
    if (lbl_eu_80664420 != 0) {
        return 0;
    }
    mtl::ALLOC_HANDLE mem = CWorkThreadSystem::getWorkMem();
    CMenuKizunaTalk* obj =
        (CMenuKizunaTalk*)mtl::MemManager::allocate(0xfc, mem);
    if (obj != 0) {
        obj = __ct__CMenuKizunaTalk(obj, scene, charId);
    }
    lbl_eu_80664420 = obj;
    reinterpret_cast<CProcess*>(obj)->Regist(parent, false);
    return lbl_eu_80664420;
}
#pragma optimize_for_size off

void func_801BCF38(){}

void func_801BCF48(){}

void func_801BD0B4(){}

void func_801BD228(){}

void func_801BD2F8(){}

// Advance the kizuna talk window state machine (retail func_801BD594):
// window states 0 and 2 wait for the CSysWin panel to become active, state 1
// waits for the embedded animation transform to finish; each sets the
// IUIWindow "advance" flag when its wait completes.
#pragma optimize_for_size on
void func_801BD594(CMenuKizunaTalk* self) {
    switch (self->mFieldA4) {
    case 0:
        if (CSysWin_isActive(&self->mSysWin[0]) != 0) {
            self->mField64 = 1;
        }
        break;
    case 1:
        if (func_80137510(self->mAnim, lbl_eu_80667E68) != 0) {
            self->mField64 = 1;
        }
        break;
    case 2:
        if (CSysWin_isActive(&self->mSysWin[0]) != 0) {
            self->mField64 = 1;
        }
        break;
    }
}
#pragma optimize_for_size off

void func_801BD630(){}

void func_801BD7D8(void* self){}

void OnFileEvent__15CMenuKizunaTalkFP10CEventFile(void* self) { ((void(*)(void*))func_801BD7D8)((char*)self - 0x6c); }

void func_801BDD8C(void* self) { ((void(*)(void*))__dt__15CMenuKizunaTalkFv)((char*)self - 0x6c); }

void func_801BDD94(void* self) { ((void(*)(void*))cbRenderBefore__15CMenuKizunaTalkFv)((char*)self - 0x70); }

extern "C" void func_801BDD9C(void* self) { ((void(*)(void*))__dt__15CMenuKizunaTalkFv)((char*)self - 0x70); }
