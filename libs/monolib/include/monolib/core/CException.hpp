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
    
    bool func_80457C8C();
    static CException* func_80457CA4(CWorkThread* pThread, const wchar_t* message, u32 r5);
    CException* func_80457EB0();
    void* func_80457ED4(u32 r4);
    virtual void wkRender();
    void func_80458084(const wchar_t* message);
    void func_eu_8045C964(void* pLayout);
    static void func_80458B64(u8* buffer, u8 r, u8 g, u8 b, u8 a);
    void func_80458B78(float x, float y, float z);
    void func_80458CBC(const wchar_t* text);
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();
    void func_80459118(const char* message);
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
    u32 mAnimState;              //0x1FC
    IGameException* mGameException; //0x200
    u32 unk204;                  //0x204
    u32 unk208;                  //0x208
    u32 mFrameCounter;           //0x20C
    u8 mFlag210;                 //0x210
    u8 pad211[3];                //0x211
    u32 mMaxExceptions;          //0x214
};
