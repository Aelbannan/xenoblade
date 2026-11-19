#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/cf/CfTaskMain.hpp"

#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

// CTTask<cf::CfTaskMain> out-of-line specializations (see CTTask.hpp).
template<>
CTTask<cf::CfTaskMain>::~CTTask() {}

extern u32 gmPauseFlags;

namespace cf {

CfTaskMain::~CfTaskMain() {}

void CfTaskMain::Init() {}
void CfTaskMain::Term() {}

void CfTaskMain::Move() {
    if ((gmPauseFlags & 0x200000) != 0) return;
    CfGameManager::func_8007F9C4();
}

void CfTaskMain::Draw() {
    CfGameManager::refreshCameraIfCurrentView();
}

void CfTaskMain::Tail() {
    CfGameManager::resetCameraIfCurrentView();
}

CfTaskMain* CfTaskMain::create(CProcess* parent, CScn* scene) {
    CfTaskMain* task = new (CWorkThreadSystem::getWorkMem()) CfTaskMain(scene);
    task->Regist(parent, false);
    return task;
}

} // namespace cf

template<>
void CTTask<cf::CfTaskMain>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CfTaskMain*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CfTaskMain>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CfTaskMain*>(this)->*mDrawFunc)();
    }
}
