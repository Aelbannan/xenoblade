#pragma once 

#include <types.h>
#include "monolib/work.hpp"
#include "monolib/core/CProc.hpp"

class IException;
class CWorkThread;

class IGameException {
public:
    virtual ~IGameException() {}
    virtual bool gameExceptionCB(u32 r4) = 0;
};

class CException : public CProc {
public:
    CException(const char* pName, CWorkThread* pParent);
    virtual ~CException();
    
    bool isFadedIn();
    static CException* func_80457CA4(CWorkThread* pThread, const wchar_t* message, u32 r5);
    CException* validateExceptionType();
    void* getMessageQueueSlot(u32 r4);
    virtual void wkRender();
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    void func_804591BC(IException* pException);
    void func_804591DC(IException* pException);
    void func_8045925C();

    static CException* convertToException(CWorkThread* pThread){
         CException* exception;
    
        //Check that the thread is valid, and has the right type id. If not, set the pointer to null.
        if(pThread == nullptr){
            exception = nullptr;
        }else if(pThread->mType != THREAD_CEXCEPTION){
            exception = nullptr;
        }else{
            //The type matches, so casting should be safe
            exception = static_cast<CException*>(pThread);
        }

        return exception;
    }

    //0x0-0x1EC: CProc
    s32 mExceptionCode;          //0x1EC
    const wchar_t* mMessage;     //0x1F0
    float mAlphaStep;            //0x1F4
    float mAlpha;                //0x1F8
    s32 mAnimState;              //0x1FC
    IGameException* mException; //0x200
    u32 unk204;                  //0x204
    u32 unk208;                  //0x208
    u32 mFrameCounter;           //0x20C
    u8 mFlag210;                 //0x210
    u8 pad211[3];                //0x211
    u32 mMaxExceptions;          //0x214
};
