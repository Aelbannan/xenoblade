// kyoshin/CTaskGamePic.cpp
// CTaskGamePic task wrapper: CProcess-derived task that hosts a scene render
// callback (IScnRender subobject) which registers itself with the owning CScn.

#include "kyoshin/CTaskGamePic.hpp"

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
// Target member: Init - registers the render callback subobject with the scene,
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
// Non-target glue stubs (preserved from scaffold; not part of the match set).
// ---------------------------------------------------------------------------

// Forward references used by scaffold thunk wrappers.
void func_8029539C(void* self);

void CTaskGamePic::Draw() {}

void CTaskGamePic::create() {}

void OnFileEvent__12CTaskGamePicFP10CEventFile(void* self) {
    ((void (*)(void*))func_8029539C)((char*)self - 0x54);
}

void func_8029554C(void* self) { ((void (*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x54); }

void func_80295554(void* self) { ((void (*)(void*))cbRenderBefore__12CTaskGamePicFv)((char*)self - 0x58); }

void func_8029555C(void* self) { ((void (*)(void*))__dt__12CTaskGamePicFv)((char*)self - 0x58); }

extern "C" s16 func_80295388(u8* self) {
    return (s16)(*(s16*)(self + 2) + *(s16*)(self + 6));
}

extern "C" void func_80294E58() {}
extern "C" void func_80294EC0() {}