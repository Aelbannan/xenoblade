#pragma once

#include <types.h>
#include "monolib/util.hpp"
#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include <nw4r/lyt.h>

class CGame : public CProc {
public:
    enum ShutdownState {
        SHUTDOWN_STATE_0,
        SHUTDOWN_STATE_1,
        SHUTDOWN_STATE_2
    };

    CGame(const char* pName, CWorkThread* pParent);
    virtual ~CGame();
    static CGame* getInstance();
    static bool func_8003933C();
    static void func_80039364();
    static void setTaskManagerUpdateCount(u32 count);
    virtual void wkUpdate();
    virtual void wkRender();
    static void func_800395F4(bool r3);
    static void setViewRect(CView* view, s16 x, s16 y, s16 width, s16 height);
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    static void GameMain();
    static void registerControllerErrorEntry(const wchar_t* message, IGameException* r4, u32 param);
    virtual bool wkStandbyExceptionRetry(u32 r4);
    virtual void OnPauseTrigger(bool paused);
    static void onExit();

    static inline CGame* create(const char* pName, CWorkThread* pParent, u32 capacity){
        CGame* game = new (CWorkThreadSystem::getWorkMem()) CGame(pName, pParent);
        CWorkUtil::entryWork(game, pParent, false);
        game->unk1E4 = capacity;
        return game;
    }

    //0x0: vtable
    //0x0-1ec: CProc
    CView* mView; //0x1EC
    ShutdownState mShutdownState; //0x1F0
    s16 mSceneReqId;
    s16 mSceneReqId2;
    s16 mSceneReqFlag;
    u8 mPad1FA[2]; //0x1FA - alignment padding between s16 mSceneReqFlag and FixStr<32> unk1FC at 0x1FC
    ml::FixStr<32> unk1FC; //0x1FC - scene transition parameter string (used as pool ref, not content)
    u32 mTaskManUpdateCount; //0x220 - number of CTaskManager::Move() calls per frame (default 1)
    float mPrevBgmSpeed; //0x224 - BGM speed before pause, restored on resume
    int mPauseRefCount; //0x228 - nested pause refcount; 0 = unpaused
    u32 mUnk22C; //0x22C - unknown; written by create() but never read within CGame
    s16 mLetterboxBorder; //0x230 - letterbox half-band in scanlines (initialized to 57 in ctor)
    u16 mPad232; //0x232 - padding to align mAllocSize at 0x234
    u32 mAllocSize; //0x234 - sizeof(CGame)==0x238; used for allocation capacity in GameMain

private:
    static const int MAX_CHILD = 8;

    static CGame* spInstance;
    static nw4r::lyt::Layout* lbl_80666604;
    static nw4r::lyt::ArcResourceAccessor* sArcResourceAccessor;
    static const char* scViewName;
};

namespace {
    class CGameRestart : public CProc {
    public:
        friend class CGame;

        CGameRestart(const char* pName, CWorkThread* pParent, int capacity) :
        CProc(pName, pParent, capacity),
        mHandle(mtl::INVALID_HANDLE) {}
        virtual ~CGameRestart(){}
        virtual void wkUpdate(){
            CWorkThread* r3 = CWorkThread::getWorkThread(mHandle);
            if(r3 == nullptr){
                CGame::GameMain();
                wkSetEvent(EVT_NONE);
                spInstance = nullptr;
            }
        }

        static inline CGameRestart* create(const char* pName, CWorkThread* pParent){
            CGameRestart* gameRestart = new (CWorkThreadSystem::getWorkMem()) CGameRestart(pName, pParent, MAX_CHILD);
            
            CWorkUtil::entryWork(gameRestart, pParent, false);
            gameRestart->unk1E4 = CDesktop::getView()->mWorkID;
            spInstance = gameRestart;
            return gameRestart;
        }

        //0x0: vtable
        //0x0-1ec: CProc
        mtl::ALLOC_HANDLE mHandle; //0x1EC
    
    private:
        static const int MAX_CHILD = 8;

        static CGameRestart* spInstance;
    };
}
