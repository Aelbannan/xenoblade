#pragma once

#include "kyoshin/cf/CTaskGameCf.hpp"
#include "kyoshin/CTaskEnvironment.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/CUIBattleManager.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/CUIWindowManager.hpp"
#include "kyoshin/cf/CTaskCulling.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfNandManager.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "kyoshin/code_80296898.hpp"
#include "monolib/device/CDeviceVI.hpp"

// Minimal local declaration of cf::CTaskREvent: CTaskREvent.hpp currently
// conflicts with CfGameManager.hpp (both declare a global func_8009D5FC with
// different return types), so this TU re-declares only the members it uses.
// Signatures match the real header so the mangled symbols link to retail.
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

extern const u32 lbl_eu_80525B9C[];
extern const u32 lbl_eu_80525B54[];
// __ptmf_null: declared (non-const u32[3]) by CfNandManager.hpp etc -
// local const form conflicts (10563)
extern u32 __ptmf_null[3];

// optimize_for_size: retail prologue/epilogue use stmw/lmw r29.
#pragma optimize_for_size on
extern "C" cf::CTaskGameCf* __ct__cf_CTaskGameCf(cf::CTaskGameCf* pThis, CProcess* pParent, int arg2) {
    __ct__8CProcessFv(pThis);

    u32* p = reinterpret_cast<u32*>(pThis);

    // Interim CTTask<CTaskGameCf> vtable, written before the callback slots
    p[4] = reinterpret_cast<u32>(&lbl_eu_80525B9C[0]);

    // Null PTMF copied into the CTTask move/draw callback slots
    typedef void (cf::CTaskGameCf::*Ptmf)();
    *(Ptmf*)((char*)pThis + 0x3C) = *(const Ptmf*)&__ptmf_null[0];
    *(Ptmf*)((char*)pThis + 0x48) = *(const Ptmf*)&__ptmf_null[0];

    // Final CTaskGameCf vtable (overwrites CTTask vtable)
    p[4] = reinterpret_cast<u32>(&lbl_eu_80525B54[0]);

    pThis->unk_54 = 0;
    pThis->pTaskGame = reinterpret_cast<CTaskGame*>(pParent);
    pThis->unk_5C = 1;
    pThis->unk_5E = 1;
    pThis->unk_60 = 16;
    pThis->unk_62 = 0;
    pThis->unk_64.mString[0] = 0;
    pThis->unk_64.mLength = 0;

    if (arg2 != 0) {
        pThis->unk_54 |= 8;
    } else {
        pThis->unk_54 = 0;
    }
    return pThis;
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

    // Retail has no mangled ctor: the class ctor is the extern "C" __ct__cf_CTaskGameCf
    // wrapper above (0xc8). The C++ ctor definition was removed to eliminate the extra
    // 0xd8 function that inflated the unit past its split budget.
    // optimize_for_size emits the retail stmw/lmw r30 frame (0x54, not 0x5c).
    #pragma optimize_for_size on
    CTaskGameCf::~CTaskGameCf(){}
    #pragma optimize_for_size off

cf::CTaskGameCf* lbl_eu_80663D38;

CTaskGameCf* CTaskGameCf::getInstance() {
    return lbl_eu_80663D38;
}

    void CTaskGameCf::reqExit(){
        unk_54 |= 1;
        if(cf::CfGameManager::isManagerInitialized()){
            cf::CfGameManager::sUnkFlags |= 0x200000;
        }
    }

extern "C" const CTaskGameCf::MoveFunc lbl_eu_80525AB8;
extern "C" const CTaskGameCf::MoveFunc lbl_eu_80525B30;
extern "C" const u32 lbl_eu_80525B0C[3];
// Pooled constant PTMF for startMission's mMoveFunc store (retail .data label);
// referencing it by name keeps the pool reloc named instead of a private @label.
extern "C" const CTaskGameCf::MoveFunc lbl_eu_80525AD0;

void CTaskGameCf::func_8004431C() {
    mMoveFunc = lbl_eu_80525AB8;
}

void CTaskGameCf::func_8004433C() {
    typedef void (cf::CTaskGameCf::*Ptmf)();
    *(Ptmf*)((char*)this + 0x3C) = &cf::CTaskGameCf::startContinue;
}

    #pragma optimize_for_size on
    void CTaskGameCf::startMission(s16 arg1, s16 arg2, ml::FixStr<32>& arg3, s16 arg4){
        unk_5C = arg1;
        unk_5E = arg2;
        unk_60 = 12;
        unk_62 = 0;
        unk_64 = arg3;
        unk_88 = arg4;
        mMoveFunc = lbl_eu_80525AD0;
    }
    #pragma optimize_for_size off

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

extern "C" CTaskGameCf::MoveFunc lbl_eu_80525B18;
extern "C" const CTaskGameCf::MoveFunc lbl_eu_80525B24;

void CTaskGameCf::func_800444FC(){
    mMoveFunc = lbl_eu_80525B18;
}

    #pragma optimize_for_size on
    void CTaskGameCf::func_8004451C(){
        // New-game/continue boot path: build the battle scene, managers and UI
        // tasks. Bit 3 of unk_54 marks a restart (skip the scene-color reset).
        if(!(unk_54 & 8)){
            func_800407C8_tmp tmp;
            func_8049602C(pTaskGame->getScene(), 0,
                          reinterpret_cast<u32*>(func_800407C8(&tmp, lbl_eu_80665D88, lbl_eu_80665D88, lbl_eu_80665D88, lbl_eu_80665D8C)));
        }

        func_80135FDC();
        __ct__800FDB4C();

        // No mission ids queued: fresh campaign (extra controller setup path).
        bool newCampaign = unk_5C == 0 && unk_5E == 0;
        bool padInit = (unk_54 & 8) == 0;
        if(newCampaign){
            padInit = false;
        }

        UnkClass_8007DAE0_init(pTaskGame->getScene(), pTaskGame->unk70, padInit);
        setPauseMode__Q22cf13CfGameManagerFv((unk_54 >> 3) & 1);

        if((lbl_eu_80663E28 & 0x01000000) == 0){
            // Scene argument is evaluated before the MEM2 handle query.
            CScnNw4r* wmScene = pTaskGame->getScene();
            u32 memHandle = mtl::MemManager::getHandleMEM2();
            func_8013CFDC(this, wmScene, memHandle);
            func_8012F558(this, pTaskGame->getScene(), -1);
            lbl_eu_80663E28 |= 0x10000000;
        }

        if(CTaskGame::isFlag2000Set()){
            lbl_eu_80663E28 |= 0x40000000;
        } else {
            lbl_eu_80663E28 &= ~0x40000000u;
        }

        func_801336E4(CTaskManager::GetRootProcGame(), pTaskGame->getScene(), -1);
        func_801665A4(CTaskManager::GetRootProcRealTime(), pTaskGame->getScene(), pTaskGame->unk70);

        if(newCampaign){
            func_8009ECB0();
            func_8009ECB0();
            func_eu_8006B238();
        } else {
            cf::CtrlObjectParamSlots* party = reinterpret_cast<cf::CtrlObjectParamSlots*>(func_8009ECB0());
            func_8009ECB0();

            if((lbl_eu_80663E28 & 0x01000000) == 0){
                func_8009E574(party, 2, 1, 1);
                func_8009E574(party, 4, 1, 2);
                func_8009E574(party, 3, 2, 0);
                func_8009E574(party, 5, 2, 1);
                func_8009E574(party, 6, 2, 2);
                func_8009E574(party, 7, 2, 3);
            }
        }

        func_80086B5C__Q22cf13CfGameManagerFv(unk_60, unk_62, 0);
        func_8007E514__Q22cf13CfGameManagerFv(unk_5C, unk_5E,
            unk_64.mString[0] ? unk_64.c_str() : nullptr, unk_88, func_8024005C());
        func_80059C58(pTaskGame, pTaskGame->getScene());
        CTaskCulling::create(pTaskGame, pTaskGame->getScene());
        if(func_8009CF8C((u32)0x20) == 0){
            func_8009D018(0x20, 1);
        }
        func_8004302C(1, 0);
        // Switch to the running-state move handler (pooled PTMF copy from
        // the retail .data constant at lbl_eu_80525B24).
        MoveFunc nextMove = *(const MoveFunc*)&lbl_eu_80525B24;
        mMoveFunc = nextMove;
    }
    #pragma optimize_for_size off

    void CTaskGameCf::func_800447B4(){
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

            // Reset scene color while the message system is unloaded (bit 24 of lbl_eu_80663E28).
            if(!(lbl_eu_80663E28 & 0x01000000)){
                func_800407C8_tmp tmp;
                func_8049602C(pTaskGame->getScene(), 0, func_800407C8(&tmp, lbl_eu_80665D88, lbl_eu_80665D88, lbl_eu_80665D88, lbl_eu_80665D8C));
            }

            if(func_8013C54C()){
                ((CProcess*)func_8013C54C())->SetRemove();
            }

            ::func_8012F87C(0);

            ((CProcess*)func_801644B4())->SetRemove();

            mMoveFunc = lbl_eu_80525B30;
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

    void CTaskGameCf::waitExit(){
        unk_8C--;
        if(unk_8C <= 0){
            if(!chkUnk54(2)){
                CfGameManager::teardownGameManager();
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

#pragma optimize_for_size on
extern "C" cf::CTaskGameCf* create__Q22cf11CTaskGameCfFv(CProcess* pParent, int arg2) {
    u32 handle = getWorkMem__17CWorkThreadSystemFv();
    cf::CTaskGameCf* task = (cf::CTaskGameCf*)allocate__Q23mtl10MemManagerFUlUl(0x90, handle);

    if (task != nullptr) {
        task = __ct__cf_CTaskGameCf(task, pParent, arg2);
    }

    Regist__8CProcessFP8CProcessb(task, pParent, false);
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

// absorb: split1 retail data sections
// generated from retail build/us/asm via split1_spec.txt
__declspec(section ".data") __attribute__((aligned(8))) __attribute__((used)) unsigned char __absorb_kyoshin_cf_CTaskGameCf_data[0x1E0] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
__declspec(section ".rodata") __attribute__((aligned(8))) __attribute__((used)) const unsigned char __absorb_kyoshin_cf_CTaskGameCf_rodata[0x28] = {
    0x63, 0x66, 0x3A, 0x3A, 0x43, 0x54, 0x61, 0x73, 0x6B, 0x47, 0x61, 0x6D,
    0x65, 0x43, 0x66, 0x00, 0x43, 0x54, 0x54, 0x61, 0x73, 0x6B, 0x3C, 0x63,
    0x66, 0x3A, 0x3A, 0x43, 0x54, 0x61, 0x73, 0x6B, 0x47, 0x61, 0x6D, 0x65,
    0x43, 0x66, 0x3E, 0x00
};
__declspec(section ".sdata") __attribute__((aligned(8))) __attribute__((used)) unsigned char __absorb_kyoshin_cf_CTaskGameCf_sdata[0x30] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
