#pragma once

#include <types.h>

#include "monolib/util/FixStr.hpp"
#include "monolib/work.hpp"

class CTaskGame;

namespace cf{
    class CTaskGameCf : public CTTask<CTaskGameCf>{
        static CTaskGameCf* spInstance;

    public:
        CTaskGameCf(CProcess* taskGame, BOOL arg2);
        virtual ~CTaskGameCf();

        static CTaskGameCf* getInstance();
        static CTaskGameCf* create(CProcess* pParent, int arg2);

        virtual void Init() override;
        virtual void Term() override;
        virtual void Draw() override;

        bool chkUnk54(u32 bit){
            return unk_54 & (1 << bit);
        }

        void setUnk54(u32 bit, bool state){
            if(state){
                unk_54 |= (1 << bit);
            } else {
                unk_54 &= ~(1 << bit);
            }
        }

        void reqExit();
        void startMode1New();
        void startMode2Con();
        void startMission(s16 arg1, s16 arg2, ml::FixStr<32>& arg3, s16 arg4);

        void startNewGame();
        void initNewGame();
        void startContinue();
        void initContinue();
        void chainNextMove();
        void setPooledMove();
        void bootGameWorld();
        void tickExitCheck();
        void beginExit();
        void waitExit();
        void finishExit();

        //0x0-0x54: CTTask
        u32 unk_54; // 0x54
        CTaskGame* pTaskGame; // 0x58
        u16 unk_5C; // 0x5C
        u16 unk_5E; // 0x5E
        u16 unk_60; // 0x60
        u16 unk_62; // 0x62
        ml::FixStr<32> unk_64; // 0x64
        u16 unk_88; // 0x88
        s32 unk_8C; // 0x8C
    }; //size = 0x90

} //namespace cf

// Cross-unit imports used by tickExitCheck (retail unmangled symbols).
extern "C" u8* getGlobalSda();
extern "C" u32 UIWin_GetInstance();
extern "C" u32 evtGetManagerAddr();
extern u32 gmPauseFlags;   // .sbss mode bitfield (bit 0x01000000)
extern const f32 lbl_eu_80665D88; // .sdata2 float
extern const f32 lbl_eu_80665D8C; // .sdata2 float

class CScnNw4r;
class CView;

// Retail-unmangled / Fv-suffixed imports used by bootGameWorld (defined in
// other TUs - keep linkage/signatures verbatim so the relocs bind by name).
// init's third parameter is int-sized in retail (no u16 zero-extension at the
// call site), so it is redeclared there under its raw retail symbol.
extern "C" void init__17UnkClass_8007DAE0FP8CScnNw4rP5CViewUs(CScnNw4r* scene, CView* view, int arg3);
// Alias so the call site reads cleanly next to the other manager setup calls.
#define UnkClass_8007DAE0_init init__17UnkClass_8007DAE0FP8CScnNw4rP5CViewUs
extern "C" void* __ct__800FDB4C();
extern "C" void setPauseMode__Q22cf13CfGameManagerFv(bool arg);
extern "C" void UIWin_CreateManager(void* self, CScnNw4r* pScene, u32 memHandle);
class CUIBattleManager;
CUIBattleManager* UIBattleGetOrCreate(CProcess* self, CScnNw4r* pScene, u32 memHandle);
extern "C" void CUICfManager_createInstance(CProcess* pParent, CScnNw4r* pScene, s32 memHandle);
extern "C" void evtCreateTaskRegister(CProcess* pParent, CScnNw4r* pScene, CView* pView);
namespace cf { struct CtrlObjectParamSlots; }
extern "C" int CtrlObjectParam_InsertSlotEntry(cf::CtrlObjectParamSlots* self, int value, int type, int index);
extern "C" void func_8007E514__Q22cf13CfGameManagerFv(int arg1, int arg2, const char* str, int arg4, u32 arg5);
extern "C" u32 CtrlRemote_TouchBitByArg(u32 id);
extern "C" void CTaskGame_setLoadScreen(int a, int b);
