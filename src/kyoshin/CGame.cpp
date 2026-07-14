#include "kyoshin/CGame.hpp"
#include "kyoshin/cf/CTaskREvent.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/lib.hpp"
#include "monolib/core.hpp"
#include "monolib/device.hpp"
#include "monolib/work.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/lib/CLibStaticData.hpp"
#include "decomp.h"
#include <cstring>
#include <revolution/GX.h>
#include <revolution/VI.h>
#include <revolution/WPAD.h>

using namespace ml;

extern void func_801BF93C();
extern float func_801C0014();
extern void func_801BFFAC(float f1, float f2);
extern void func_801644BC(u32 value);
extern void func_80044FBC(u32 value);

// Non-vararg sink avoids crclr (varargs float ABI) so five pool strings fit in 0x1C.
void force_cgame_strings(const char*, const char*, const char*, const char*, const char*);
void FORCEACTIVECGame_cpp_wkStandbyLogin(void);
void FORCEACTIVECGame_cpp_wkStandbyLogin(void) {
    force_cgame_strings("CGameRestart", "", "43", "arc", "4_3mode.brlyt");
}

CGame* CGame::spInstance;
static FixStr<64> lbl_80573C80;
nw4r::lyt::Layout* CGame::lbl_80666604;
nw4r::lyt::ArcResourceAccessor* CGame::sArcResourceAccessor;
const char* CGame::scViewName = "巨神"; //"Bionis"
CGameRestart* CGameRestart::spInstance;

CGame::CGame(const char* pName, CWorkThread* pParent) :
    CProc(pName, pParent, MAX_CHILD),
    mView(nullptr),
    mShutdownState(SHUTDOWN_STATE_0),
    unk1F4(-1),
    unk1F6(-1),
    unk1F8(0),
    unk1FC(),
    mTaskManUpdateCount(1),
    unk224(1.0f),
    unk228(0) {
    spInstance = this;
    CLibHbm::func_8045D5C8(1);
    CWorkSystem::setExitFunc(&onExit);
    wkSetEvent(EVT_4);
}

CGame::~CGame() {
    CWorkSystem::setExitFunc(nullptr);
    CLibHbm::func_8045D5C8(0);
    spInstance = nullptr;
}

CGame* CGame::getInstance() {
    return spInstance;
}

bool CGame::func_8003933C() {
    return func_80164910() == 0;
}

void CGame::func_80039364() {
    if (spInstance == nullptr) {
        GameMain();
    } else if (CGameRestart::spInstance == nullptr) {
        CGameRestart* gameRestart = CGameRestart::create("CGameRestart", CDesktop::getInstance());

        if (gameRestart != nullptr) {
            gameRestart->mHandle = spInstance->mWorkID;
            spInstance->wkSetEvent(EVT_NONE);
        }
    }
}

void CGame::setTaskManagerUpdateCount(u32 count) {
    if (spInstance != nullptr) {
        spInstance->mTaskManUpdateCount = count;
    }
}

void CGame::wkUpdate() {
    if ((s16)unk1F4 >= 0 && CTaskGame::getInstance() != nullptr) {
        if (unk1FC.size() == 0) {
            CTaskGame::getInstance()->func_80040A3C(unk1F4, unk1F6, nullptr, unk1F8);
        } else {
            CTaskGame::getInstance()->func_80040A3C(unk1F4, unk1F6, unk1FC.c_str(), unk1F8);
        }

        unk1F4 = -1;
        unk1F6 = -1;
        // Preserve the retail string-pool reference to the terminator after
        // "CGameRestart"; a plain "" resolves to a different pool entry.
        unk1FC = "CGameRestart" + 13;
        unk1F8 = 0;
    }

    if (isNoEvent() && CTaskGame::getInstance() != nullptr) {
        // The repeated singleton lookup is part of the retail call schedule.
        (void)CTaskGame::getInstance();
        if (CTaskGame::func_800426F0() == false) {
            CTaskGame::getInstance()->func_80042720();
        }
    }

    for (u32 i = 0; i < mTaskManUpdateCount; i++) {
        CTaskManager::Move();
    }
}

void CGame::wkRender() {
    if (lbl_80666604 != nullptr) {
        lbl_80666604->Animate(0);
    }

    // Draw the 4:3 presentation overlay before dispatching task rendering.
    if (lbl_80666604 != nullptr) {
        if (!CDeviceSC::isWideAspectRatio()) {
            CDeviceGX::getCacheInstance()->func_8044BE38();
            GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
            nw4r::lyt::DrawInfo drawInfo;
            func_80137250(&drawInfo);
            func_80137038(lbl_80666604, &drawInfo, 0, 1);
            CViewRoot::func_80442DA8();
        }
    }

    CTaskManager::Draw();
}

void CGame::func_800395F4(bool wide) {
    if (spInstance != nullptr && spInstance->mView != nullptr) {
        if (!wide) {
            setViewRect(spInstance->mView, 0, 56,
                CDeviceVI::getRenderModeObj()->fbWidth,
                CDeviceVI::getRenderModeObj()->efbHeight - 114);
        } else {
            setViewRect(spInstance->mView, 0, 0,
                CDeviceVI::getRenderModeObj()->fbWidth,
                CDeviceVI::getRenderModeObj()->efbHeight);
        }
    }
}

void CGame::setViewRect(CView* view, s16 x, s16 y, s16 width, s16 height) {
    view->setRect(ml::CRect16(x, y, width, height));
}

bool CGame::wkStandbyLogin() {
    StaticDataHandle handle;

    if (!CLibStaticData::isInitialized()) {
        return false;
    }

    {
        CView* view = pssCreateView(scViewName, CDesktop::getView(), 0);
        mView = view;
        const char* viewName = scViewName;
        view->mName = viewName;
        if (static_cast<CWorkThread*>(view)->mName.size() == 0) {
            static_cast<CWorkThread*>(view)->mName = viewName;
        }
    }

    if (CDeviceSC::isWideAspectRatio()) {
        setViewRect(mView, 0, 0,
            CDeviceVI::getRenderModeObj()->fbWidth,
            CDeviceVI::getRenderModeObj()->efbHeight);
    } else {
        setViewRect(mView, 0,
            (s16)((u16)unk230 - 1),
            CDeviceVI::getRenderModeObj()->fbWidth,
            (s16)((u16)CDeviceVI::getRenderModeObj()->efbHeight
                - ((u32)(u16)unk230 << 1)));
    }

    mView->unk444 = CVec4(0.0f, 0.0f, 0.0f, 1.0f);

    func_800395F4(CDeviceSC::isWideAspectRatio());
    CDeviceGX::updateVerticalFilter(VFILTER_NONE);
    CTaskManager::Reset();
    CTaskGame::create(mView, this, 1);
    WPADSetAutoSleepTime(5);
    VIEnableDimming(1);
    VISetTimeToDimming(1);

    if (CLibStaticData::getStaticFileData("CGameRestart" + 14, &handle, nullptr)) {
        sArcResourceAccessor = CLibLayout::createArcResourceAccessor();
        sArcResourceAccessor->Attach(handle.data, "CGameRestart" + 17);
        func_80136E84(&lbl_80666604, sArcResourceAccessor, "CGameRestart" + 21);
    }

    return CProc::wkStandbyLogin();
}

bool CGame::wkStandbyLogout() {
    if (mShutdownState == SHUTDOWN_STATE_0) {
        CTaskGame::getInstance()->func_80042710();
        mShutdownState = SHUTDOWN_STATE_1;
    }

    if (mShutdownState == SHUTDOWN_STATE_1) {
        if (!(CTaskGame::getInstance()->unk68 & 0x10)) {
            return false;
        }
        mShutdownState = SHUTDOWN_STATE_2;
    }

    // Teardown can begin only after all child threads have stopped.
    if (mChildren.empty()) {
        CTaskManager::Reset();

        if (sArcResourceAccessor != nullptr) {
            delete sArcResourceAccessor;
            sArcResourceAccessor = nullptr;
        }

        if (lbl_80666604 != nullptr) {
            delete lbl_80666604;
            lbl_80666604 = nullptr;
        }

        return CProc::wkStandbyLogout();
    }

    return false;
}

// Keep the ArcResourceAccessor destructor instantiated in this translation unit.
static void dummy() {
    nw4r::lyt::ArcResourceAccessor* accessor = new nw4r::lyt::ArcResourceAccessor();
    accessor->Attach(0, 0);
    delete accessor;
}

void CGame::GameMain() {
    if (spInstance != nullptr) {
        spInstance->pssSetFocus();
    } else {
        create("CGame", CDesktop::getInstance(), CDesktop::getView()->mWorkID);
    }
}

// Register an exception entry for a controller-related error, such as a disconnect.
void CGame::registerControllerErrorEntry(const wchar_t* message, IGameException* handler, u32 param) {
    if (spInstance != nullptr && CTaskGame::func_800426F0() == nullptr && !spInstance->isNoEvent()) {
        CException* exception = CException::func_80457CA4(spInstance, message, 5);
        if (exception != nullptr) {
            exception->mException = handler;
            exception->unk204 = param;
        }
    }
}

// Retry a controller exception raised by CfPadTask.
bool CGame::wkStandbyExceptionRetry(u32 wid) {
    if (isNoEvent()) {
        return true;
    }
    if (CLibHbm::func_8045DE00()) {
        return false;
    }

    CWorkThread* workThread = CWorkThread::getWorkThread(wid);
    CException* exception = CException::convertToException(workThread);

    if (exception == nullptr) {
        return true;
    }
    if (exception->func_80457C8C() == false) {
        return false;
    }
    if (exception->mException == nullptr) {
        return true;
    }

    IGameException* handler = exception->mException;
    return handler->gameExceptionCB(exception->unk204);
}

void CGame::OnPauseTrigger(bool paused) {
    if (cf::CfGameManager::func_8007E1B4()) {
        if (paused) {
            if (unk228 == 0) {
                unk224 = func_801C0014();
                func_801BFFAC(0, 0);
                func_801644BC(1);

                if (cf::CBattleManager::getInstance() != nullptr) {
                    cf::CBattleManager* battleManager = cf::CBattleManager::getInstance();
                    battleManager->mVision.func_801A929C(1);
                }

                func_80044FBC(1);
            }

            unk228++;
        } else {
            if (unk228 <= 1) {
                func_801BFFAC(unk224, 0);
                func_801644BC(0);

                if (cf::CBattleManager::getInstance() != nullptr) {
                    cf::CBattleManager* battleManager = cf::CBattleManager::getInstance();
                    battleManager->mVision.func_801A929C(0);
                }

                func_80044FBC(0);
            }

            unk228--;
            if (unk228 < 0) {
                unk228 = 0;
            }
        }
    }
}

void CGame::onExit() {
    if (spInstance != nullptr) {
        if (cf::CfGameManager::func_8007E1B4()) {
            func_801BF93C();
        }
    }
}
