#pragma once

#include "kyoshin/cf/CTaskGameCf.hpp"
#include "kyoshin/CTaskEnvironment.hpp"
#include "kyoshin/CTaskGameImports.hpp"
#include "kyoshin/CUIBattleManager.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/CUIWindowManager.hpp"
#include "kyoshin/cf/CTaskCulling.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfTFile.hpp"
#include "kyoshin/cf/CfNandManager.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "kyoshin/code_80296898.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"  // canonical mtl::MemManager::allocate

// Minimal local declaration of cf::CTaskREvent: CTaskREvent.hpp currently
// conflicts with CfGameManager.hpp (both declare a global CtrlRemote_GetFileEventIds with
// different return types), so this TU re-declares only the members it uses.
// Signatures match the real header so the mangled symbols link to retail.
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(int, int, int);
extern "C" void Env_CreateTask__FP8CProcessP8CProcess(CProcess*, CProcess*);

namespace cf {
    class CTaskREvent : public CProcess {
    public:
        static CTaskREvent* getInstance();
        static void create(CProcess* pParent, CScnNw4r* pScene, CView* pView);
    };
}

// NOTE: function definitions in this TU are ordered to mirror the retail
// .text layout (ctor first, CTTask<...> Move/Draw template specializations
// last). MWCC emits functions in definition order, so keeping this order
// aligns decomp .text offsets with the retail split object.


// local const form conflicts (10563)
extern u32 __ptmf_null[3];

// optimize_for_size: retail prologue/epilogue use stmw/lmw r29.
// Retail's ctor is a real cf::CTaskGameCf constructor: MWCC emits the base
// CTTask ctor (vtable + null PTMFs) and the derived vtable/member stores, then
// create() calls it directly (no placement-new null guard).
// optimize_for_size emits the retail stmw/lmw r29 frame (0xc8).
#pragma optimize_for_size on
cf::CTaskGameCf::CTaskGameCf(CProcess* taskGame, BOOL arg2)
    : unk_54(0),
      pTaskGame(reinterpret_cast<CTaskGame*>(taskGame)),
      unk_5C(1),
      unk_5E(1),
      unk_60(16),
      unk_62(0) {
    unk_64.mString[0] = 0;
    unk_64.mLength = 0;
    if (arg2 != 0) {
        unk_54 |= 8;
    } else {
        unk_54 = 0;
    }
}
#pragma optimize_for_size off

// Retail __dt__26CTTask<Q22cf11CTaskGameCf>Fv is 0x50 (stmw r30 frame); keep
// optimize_for_size on like CTaskGameEff's dtor.
// Placed immediately after the ctor to mirror the retail .text layout.
#pragma optimize_for_size on
template<>
CTTask<cf::CTaskGameCf>::~CTTask() {}
#pragma optimize_for_size off

namespace cf{
    CTaskGameCf* CTaskGameCf::spInstance;


    #pragma optimize_for_size on
    CTaskGameCf::~CTaskGameCf(){}
    #pragma optimize_for_size off

CTaskGameCf* CTaskGameCf::getInstance() {
    return spInstance;
}

    void CTaskGameCf::reqExit(){
        unk_54 |= 1;
        if(cf::CfGameManager::isManagerInitialized()){
            gmGameFlags |= 0x200000;
        }
    }

void CTaskGameCf::startMode1New() {
    mMoveFunc = &CTaskGameCf::startNewGame;
}

void CTaskGameCf::startMode2Con() {
    mMoveFunc = &CTaskGameCf::startContinue;
}

    #pragma optimize_for_size on
    void CTaskGameCf::startMission(s16 arg1, s16 arg2, ml::FixStr<32>& arg3, s16 arg4){
        unk_5C = arg1;
        unk_5E = arg2;
        unk_60 = 12;
        unk_62 = 0;
        unk_64 = arg3;
        unk_88 = arg4;
        mMoveFunc = &CTaskGameCf::chainNextMove;
    }
    #pragma optimize_for_size off

void CTaskGameCf::Init() {
    spInstance = this;
}

    void CTaskGameCf::Term(){
        if(cf::CTaskCulling::getInstance()){
            cf::CTaskCulling::getInstance()->SetRemove();
        }
        spInstance = nullptr;
    }

void CTaskGameCf::Draw() {}

    void CTaskGameCf::startNewGame(){
        mMoveFunc = &CTaskGameCf::initNewGame;
    }

    void CTaskGameCf::initNewGame(){
        unk_5C = 1;
        unk_5E = 1;
        unk_60 = 12;
        unk_62 = 0;
        mMoveFunc = &CTaskGameCf::bootGameWorld;
    }

    void CTaskGameCf::startContinue(){
        mMoveFunc = &CTaskGameCf::initContinue;
    }

    void CTaskGameCf::initContinue(){
        unk_5C = 1;
        unk_5E = 1;
        unk_60 = 12;
        unk_62 = 0;
        mMoveFunc = &CTaskGameCf::bootGameWorld;
    }

void cf::CTaskGameCf::chainNextMove(){
    mMoveFunc = &CTaskGameCf::setPooledMove;
}

void CTaskGameCf::setPooledMove(){
    mMoveFunc = &CTaskGameCf::bootGameWorld;
}

    #pragma optimize_for_size on
    void CTaskGameCf::bootGameWorld(){
        // New-game/continue boot path: build the battle scene, managers and UI
        // tasks. Bit 3 of unk_54 marks a restart (skip the scene-color reset).
        if(!(unk_54 & 8)){
            CTaskGame_setVec4_tmp tmp;
            Scn_ReleaseUnk80(pTaskGame->getScene(), 0,
                          reinterpret_cast<u32*>(CTaskGame_setVec4(&tmp, 0.0f, 0.0f, 0.0f, 1.0f)));
        }

        MenuStateInitFlags();
        __ct__800FDB4C();

        // No mission ids queued: fresh campaign (extra controller setup path).
        bool newCampaign = unk_5C == 0 && unk_5E == 0;
        bool padInit = (unk_54 & 8) == 0;
        if(newCampaign){
            padInit = false;
        }

        UnkClass_8007DAE0_init(pTaskGame->getScene(), pTaskGame->unk70, padInit);
        setPauseMode__Q22cf13CfGameManagerFv((unk_54 >> 3) & 1);

        if((gmPauseFlags & 0x01000000) == 0){
            // Scene argument is evaluated before the MEM2 handle query.
            CScnNw4r* wmScene = pTaskGame->getScene();
            u32 memHandle = mtl::MemManager::getHandleMEM2();
            UIWin_CreateManager(this, wmScene, memHandle);
            UIBattleGetOrCreate(this, pTaskGame->getScene(), -1);
            gmPauseFlags |= 0x10000000;
        }

        if(CTaskGame::isFlag2000Set()){
            gmPauseFlags |= 0x40000000;
        } else {
            gmPauseFlags &= ~0x40000000u;
        }

        CUICfManager_createInstance(CTaskManager::GetRootProcGame(), pTaskGame->getScene(), -1);
        evtCreateTaskRegister(CTaskManager::GetRootProcRealTime(), pTaskGame->getScene(), pTaskGame->unk70);

        if(newCampaign){
            CtrlObjectParam_GetSlotTableBase();
            CtrlObjectParam_GetSlotTableBase();
            CfT_ResetEventState();
        } else {
            cf::CtrlObjectParamSlots* party = reinterpret_cast<cf::CtrlObjectParamSlots*>(CtrlObjectParam_GetSlotTableBase());
            CtrlObjectParam_GetSlotTableBase();

            if((gmPauseFlags & 0x01000000) == 0){
                CtrlObjectParam_InsertSlotEntry(party, 2, 1, 1);
                CtrlObjectParam_InsertSlotEntry(party, 4, 1, 2);
                CtrlObjectParam_InsertSlotEntry(party, 3, 2, 0);
                CtrlObjectParam_InsertSlotEntry(party, 5, 2, 1);
                CtrlObjectParam_InsertSlotEntry(party, 6, 2, 2);
                CtrlObjectParam_InsertSlotEntry(party, 7, 2, 3);
            }
        }

        func_80086B5C__Q22cf13CfGameManagerFv(unk_60, unk_62, 0);
        func_8007E514__Q22cf13CfGameManagerFv(unk_5C, unk_5E,
            unk_64.mString[0] ? unk_64.c_str() : nullptr, unk_88, NandImageLive());
        Env_CreateTask__FP8CProcessP8CProcess(pTaskGame, pTaskGame->getScene());
        CTaskCulling::create(pTaskGame, pTaskGame->getScene());
        if(CtrlRemote_TouchBitByArg((u32)0x20) == 0){
            CtrlRemote_SetSharedBit(0x20, 1);
        }
        CTaskGame_setLoadScreen(1, 0);
        mMoveFunc = &CTaskGameCf::tickExitCheck;
    }
    #pragma optimize_for_size off

    void CTaskGameCf::tickExitCheck(){
        if(Class_80296898::getInstance()->mFrameCount == 0){
            u8 frame = 10;
            Class_80296898::getInstance()->mFrameCount = frame;
        }

        CDeviceVI::setGammaValue(Class_80296898::getInstance()->mFrameCount - 1);

        // volatile forces MWCC to emit the retail load/test + reload/clear shape
        volatile u32& flags = unk_54;
        if(flags & 1){
            flags &= ~1u;

            // Tear down the environment/culling/UI tasks (SetRemove = flag byte 0x39).
            if(getGlobalSda()){
                ((CProcess*)getGlobalSda())->SetRemove();
            }

            if(CTaskCulling::getInstance()){
                CTaskCulling::getInstance()->SetRemove();
            }

            // Reset scene color while the message system is unloaded (bit 24 of gmPauseFlags).
            if(!(gmPauseFlags & 0x01000000)){
                CTaskGame_setVec4_tmp tmp;
                Scn_ReleaseUnk80(pTaskGame->getScene(), 0, CTaskGame_setVec4(&tmp, 0.0f, 0.0f, 0.0f, 1.0f));
            }

            if(UIWin_GetInstance()){
                ((CProcess*)UIWin_GetInstance())->SetRemove();
            }

            ::UIBattleRemoveAll(0);

            ((CProcess*)evtGetManagerAddr())->SetRemove();

            mMoveFunc = &CTaskGameCf::beginExit;
        }
    }

void CTaskGameCf::beginExit() {
    if (CUICfManager::getInstance()) {
        CUICfManager::getInstance()->SetRemove();
    }

    setUnk54(2, false);
    pTaskGame->getScene()->unk_3E4 = 1;
    unk_8C = 2;
    mMoveFunc = &CTaskGameCf::waitExit;
}

    // Retail singleton teardown is the flat symbol __dt__800FDC1C (not the
    // recovered CfObjectSelectorObj::destroy mangling).
    extern "C" void __dt__800FDC1C();

    void CTaskGameCf::waitExit(){
        unk_8C--;
        if(unk_8C <= 0){
            if(!chkUnk54(2)){
                CfGameManager::teardownGameManager();
            }
            __dt__800FDC1C();
            mMoveFunc = &CTaskGameCf::finishExit;
        }
    }

void CTaskGameCf::finishExit() {
    CScnNw4r* sceneData = pTaskGame->getScene();
    sceneData->unk_3E4 = 0;
    unk_54 |= 2;
}

} //namespace cf

// Direct call to the real constructor: placement new would inline the ctor
// under this TU's -inline auto, expanding create() from 0x6c to 0xf8.
extern "C" cf::CTaskGameCf* __ct__Q22cf11CTaskGameCfFP8CProcessi(cf::CTaskGameCf* self, CProcess* taskGame, BOOL arg2);

#pragma optimize_for_size on
extern "C" cf::CTaskGameCf* create__Q22cf11CTaskGameCfFv(CProcess* pParent, int arg2) {
    u32 handle = getWorkMem__17CWorkThreadSystemFv();
    cf::CTaskGameCf* task = (cf::CTaskGameCf*)mtl::MemManager::allocate(0x90, handle);

    if (task != nullptr) {
        task = __ct__Q22cf11CTaskGameCfFP8CProcessi(task, pParent, arg2);
    }

    task->Regist(pParent, false);
    return task;
}
#pragma optimize_for_size off

// --- CTTask<cf::CTaskGameCf> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
// Kept LAST to match the retail .text layout (they sit after create()).
template<>
void CTTask<cf::CTaskGameCf>::Move() {
    if (mMoveFunc) {
        (static_cast<cf::CTaskGameCf*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<cf::CTaskGameCf>::Draw() {
    if (mDrawFunc) {
        (static_cast<cf::CTaskGameCf*>(this)->*mDrawFunc)();
    }
}
