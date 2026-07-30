#pragma once 

#include <types.h>
#include "monolib/work.hpp"

class IException;

class IGameException {
public:
    virtual ~IGameException() {}
    virtual bool gameExceptionCB(u32 r4) = 0;
};

class CException : public CWorkThread {
public:
    bool func_80457C8C();
    void func_80458B64(u8* buffer, u8 r4, u8 r5, u8 r6, u8 r7);
    void func_8045925C();
    void func_804591BC(IException* pException);
    static CException* func_80457CA4(CWorkThread* r3, const wchar_t* message, u32 r5);

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

    u8 unk1C4[0x1F8 - 0x1C4];
    float field_0x1F8;           // 0x1F8 - compared against 0.8f in func_80457C8C
    u8 unk1FC[0x200 - 0x1FC];
    IGameException* mException; //0x200
    u32 unk204;
};
