#pragma once

#include "kyoshin/cf/CTaskGameCf.hpp"
#include "kyoshin/CTaskEnvironment.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUIBattleManager.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/CUIWindowManager.hpp"
#include "kyoshin/cf/CTaskCulling.hpp"
#include "kyoshin/cf/CTaskREvent.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfNandManager.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "kyoshin/code_80296898.hpp"
#include "monolib/device/CDeviceVI.hpp"

extern "C" void func_eu_8006B238();

namespace cf{
    CTaskGameCf* CTaskGameCf::spInstance;

    CTaskGameCf::CTaskGameCf(CProcess* pParent, BOOL arg2)
        : pTaskGame((CTaskGame*)pParent), unk_54(0), unk_5C(1), unk_5E(1), unk_60(16), unk_62(0){
        if(arg2){
            unk_54 |= 8;
        } else {
            unk_54 = 0;
        }
    }

    CTaskGameCf::~CTaskGameCf(){}

cf::CTaskGameCf* lbl_eu_80663D38;

CTaskGameCf* CTaskGameCf::getInstance() {
    return lbl_eu_80663D38;
}

    void CTaskGameCf::reqExit(){
        unk_54 |= 1;
        if(cf::CfGameManager::func_8007E1B4()){
            cf::CfGameManager::sUnkFlags |= 0x200000;
        }
    }

extern const CTaskGameCf::MoveFunc lbl_eu_80525AB8;

void CTaskGameCf::func_8004431C() {
    mMoveFunc = lbl_eu_80525AB8;
}

extern const u32 lbl_eu_80525AC4[3];

void CTaskGameCf::func_8004433C() {
    u32 v1 = lbl_eu_80525AC4[1];
    u32 v0 = lbl_eu_80525AC4[0];
    reinterpret_cast<u32*>(this)[0x40 / 4] = v1;
    reinterpret_cast<u32*>(this)[0x3C / 4] = v0;
    u32 v2 = lbl_eu_80525AC4[2];
    reinterpret_cast<u32*>(this)[0x44 / 4] = v2;
}

    void CTaskGameCf::startMission(s16 arg1, s16 arg2, ml::FixStr<32>& arg3, s16 arg4){
        unk_5C = arg1;
        unk_5E = arg2;
        unk_60 = 12;
        unk_62 = 0;
        unk_64 = arg3;
        unk_88 = arg4;
        mMoveFunc = &CTaskGameCf::func_800444DC;
    }

void CTaskGameCf::Init() {
    lbl_eu_80663D38 = this;
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
        mMoveFunc = &CTaskGameCf::func_8004451C;
    }

    void CTaskGameCf::startContinue(){
        mMoveFunc = &CTaskGameCf::initContinue;
    }

    void CTaskGameCf::initContinue(){
        unk_5C = 1;
        unk_5E = 1;
        unk_60 = 12;
        unk_62 = 0;
        mMoveFunc = &CTaskGameCf::func_8004451C;
    }

void cf::CTaskGameCf::func_800444DC(){
    typedef void (cf::CTaskGameCf::*Ptmf)();
    *(Ptmf*)((char*)this + 0x3C) = &cf::CTaskGameCf::func_800444FC;
}

CTaskGameCf::MoveFunc lbl_eu_80525B18;

void CTaskGameCf::func_800444FC(){
    mMoveFunc = lbl_eu_80525B18;
}

    void CTaskGameCf::func_8004451C(){
        if(!(unk_54 & 8)){
            func_800407C8_tmp tmp;
            pTaskGame->getScene()->func_8049602C(0, func_800407C8(&tmp, 0.0f, 0.0f, 0.0f, 1.0f));
        }

        CUICfManager::func_80135FDC();
        CfObjectSelectorObj::create();

        bool v5 = !unk_5C && !unk_5E;
        bool v6 = (unk_54 & 8) == 0;
        if(v5){
            v6 = false;
        }

        CfGameManager::init(pTaskGame->getScene(), pTaskGame->unk70, v6);
        CfGameManager::func_8007F930((unk_54 >> 3) & 1);

        if(!CfGameManager::checkUnkFlag(24)){
            CUIWindowManager::create(this, pTaskGame->getScene(), mtl::MemManager::getHandleMEM2());
            CUIBattleManager::create(this, pTaskGame->getScene(), mtl::INVALID_HANDLE);
            CfGameManager::setUnkFlag(28, true);
        }

        if(CTaskGame::func_800404F0()){
            CfGameManager::setUnkFlag(30, true);
        } else {
            CfGameManager::setUnkFlag(30, false);
        }

        CUICfManager::create(CTaskManager::GetRootProcGame(), pTaskGame->getScene(), mtl::INVALID_HANDLE);
        CTaskREvent::create(CTaskManager::GetRootProcRealTime(), pTaskGame->getScene(), pTaskGame->unk70);

        if(v5){
            func_8009ECB0();
            func_8009ECB0();
            func_eu_8006B238();
        } else {
            int* v18 = func_8009ECB0();
            func_8009ECB0();

            if(!cf::CfGameManager::checkUnkFlag(24)){
                func_8009E574(v18, 2, 1, 1);
                func_8009E574(v18, 4, 1, 2);
                func_8009E574(v18, 3, 2, 0);
                func_8009E574(v18, 5, 2, 1);
                func_8009E574(v18, 6, 2, 2);
                func_8009E574(v18, 7, 2, 3);
            }
        }

        CfGameManager::func_80086B5C(unk_60, unk_62, 0);
        CfGameManager::func_8007E514(unk_5C, unk_5E, unk_64[0] ? unk_64.c_str() : nullptr, unk_88, CfNandManager::func_8024005C());
        CTaskEnvironment::create(pTaskGame, pTaskGame->getScene());
        CTaskCulling::create(pTaskGame, pTaskGame->getScene());
        if(!func_8009CF8C(32)) func_8009D018(32, 1);
        func_8004302C(1, 0);
        mMoveFunc = &CTaskGameCf::func_800447B4;
    }

    void CTaskGameCf::func_800447B4(){
        if(Class_80296898::getInstance()->mFrameCount == 0){
            Class_80296898::getInstance()->mFrameCount = 10;
        }

        CDeviceVI::func_804483DC(Class_80296898::getInstance()->mFrameCount - 1);

        if(!(unk_54 & 1)){
            return;
        }
        unk_54 &= ~1u;

        if(CTaskEnvironment::getInstance()){
                CTaskEnvironment::getInstance()->SetRemove();
            }

            if(CTaskCulling::getInstance()){
                CTaskCulling::getInstance()->SetRemove();
            }

            if(!CfGameManager::checkUnkFlag(24)){
                func_800407C8_tmp tmp;
                pTaskGame->getScene()->func_8049602C(0, func_800407C8(&tmp, 0.0f, 0.0f, 0.0f, 1.0f));
            }

            if(CUIWindowManager::getInstance()){
                CUIWindowManager::getInstance()->SetRemove();
            }

            CUIBattleManager::func_8012F87C(0);
            CTaskREvent::getInstance()->SetRemove();
            mMoveFunc = &CTaskGameCf::beginExit;
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

    void CTaskGameCf::waitExit(){
        unk_8C--;
        if(unk_8C <= 0){
            if(!chkUnk54(2)){
                CfGameManager::func_8007E218();
            }
            CfObjectSelectorObj::destroy();
            mMoveFunc = &CTaskGameCf::finishExit;
        }
    }

void CTaskGameCf::finishExit() {
    u32* taskGame = *(u32**)((u8*)this + 0x58);
    u32* sceneData = *(u32**)((u8*)taskGame + 0x74);
    *(u8*)((u8*)sceneData + 0x3E4) = 0;
    *(u32*)((u8*)this + 0x54) |= 2;
}

} //namespace cf

// Forward declarations
extern "C" cf::CTaskGameCf* __ct__cf_CTaskGameCf(cf::CTaskGameCf* pThis, CProcess* pParent, int arg2);
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
extern "C" u32 getWorkMem__17CWorkThreadSystemFv();
extern "C" void Regist__8CProcessFP8CProcessb(CProcess* self, CProcess* parent, bool insertTop);

extern const u32 lbl_eu_80525B9C[];
extern const u32 lbl_eu_80525B54[];
extern const u32 __ptmf_null[3];

extern "C" void __ct__8CProcessFv(CProcess*);

extern "C" cf::CTaskGameCf* __ct__cf_CTaskGameCf(cf::CTaskGameCf* pThis, CProcess* pParent, int arg2) {
    __ct__8CProcessFv(pThis);
    
    u32* p = reinterpret_cast<u32*>(pThis);
    const u32* nullPt = &__ptmf_null[0];
    
    // Set CTTask<CTaskGameCf> vtable
    p[4] = reinterpret_cast<u32>(&lbl_eu_80525B9C[0]);
    
    // Copy PTMF null to mMoveFunc (0x3C) in retail store order: 0x40, 0x3C, 0x44
    // Then mDrawFunc (0x48) in same order: 0x4C, 0x48, 0x50
    u32 w0 = nullPt[0];
    u32 w1 = nullPt[1];
    p[0x10] = w1;  // store word 1 to 0x40
    p[0xF] = w0;   // store word 0 to 0x3C
    p[0x11] = nullPt[2]; // store word 2 to 0x44
    // Reload for second PTMF
    w0 = nullPt[0];
    w1 = nullPt[1];
    p[0x13] = w1;  // store word 1 to 0x4C
    p[0x12] = w0;  // store word 0 to 0x48
    p[0x14] = nullPt[2]; // store word 2 to 0x50
    
    // Set CTaskGameCf vtable (overwrites CTTask vtable)
    p[4] = reinterpret_cast<u32>(&lbl_eu_80525B54[0]);
    
    pThis->unk_54 = 0;
    pThis->pTaskGame = reinterpret_cast<CTaskGame*>(pParent);
    pThis->unk_5C = 1;
    pThis->unk_5E = 1;
    pThis->unk_60 = 16;
    pThis->unk_62 = 0;
    pThis->unk_64.mString[0] = 0;
    pThis->unk_64.mLength = 0;
    
    if (arg2 == 0) goto zero_case;
    pThis->unk_54 = 8;
    return pThis;
zero_case:
    pThis->unk_54 = 0;
    return pThis;
}

extern "C" cf::CTaskGameCf* create__Q22cf11CTaskGameCfFv(CProcess* pParent, int arg2) {
    u32 handle = getWorkMem__17CWorkThreadSystemFv();
    cf::CTaskGameCf* task = (cf::CTaskGameCf*)allocate__Q23mtl10MemManagerFUlUl(0x90, handle);

    if (task != nullptr) {
        task = __ct__cf_CTaskGameCf(task, pParent, arg2);
    }

    Regist__8CProcessFP8CProcessb(task, pParent, false);
    return task;
}
