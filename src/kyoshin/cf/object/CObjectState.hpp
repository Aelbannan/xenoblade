#pragma once

#include <types.h>

namespace cf {
    //min size: 0x10
    class CObjectState {
    public:
        virtual void CObjectState_UnkVirtualFunc1(u32 bits);  //0x8
        virtual int CObjectState_UnkVirtualFunc2(int mask);  //0xC
        virtual void CObjectState_UnkVirtualFunc3(u32 mask);  //0x10
        virtual void CObjectState_UnkVirtualFunc4();  //0x14
        virtual void CObjectState_UnkVirtualFunc5(int arg);  //0x18
        virtual void CObjectState_UnkVirtualFunc6();  //0x1C
        virtual void CObjectState_UnkVirtualFunc7(int arg);  //0x20
        virtual int CObjectState_UnkVirtualFunc8(int arg);  //0x24
        virtual void CObjectState_UnkVirtualFunc9();  //0x28
        virtual int CObjectState_UnkVirtualFunc10(void* arg, int arg2); //0x2C
        virtual void CObjectState_UnkVirtualFunc11(); //0x30
        virtual void* CObjectState_UnkVirtualFunc12(); //0x34
        virtual void CObjectState_UnkVirtualFunc13(); //0x38

        //0x0: vtable
        u32 unk4;          // 0x04
        u32 unk8;          // 0x08
        u32 unkC;          // 0x0C
    };
}
