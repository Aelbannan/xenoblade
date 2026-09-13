#pragma once

#include <types.h>
#include "kyoshin/cf/object/CObjectState.hpp"

namespace cf {
    //min size: 0x38
    class CObjectParam : public CObjectState {
    public:
        virtual void CObjectParam_UnkVirtualFunc1(const char* name); //0x3C
        virtual void* CObjectParam_UnkVirtualFunc2(); //0x40
        virtual int CObjectParam_hasObjectName(); //0x44 (was UnkVirtualFunc3)
        virtual void CObjectParam_UnkVirtualFunc4(); //0x48 (base clears +0x34; Point overrides as loadPointData)
        virtual BOOL CObjectParam_getSelfObjectId(); //0x4C
        virtual void CObjectParam_signalActionEnd(int flag); //0x50

        // Wave-36 caller alias (non-virtual, inline): sets the object name
        // (slot 0x3C). Forwards to the same virtual slot; see CActorParam.hpp.
        void CObjectParam_setObjectName(const char* name) { CObjectParam_UnkVirtualFunc1(name); } //0x3C

        // Wave-63 mop-up alias (non-virtual, inline): legacy UnkVirtualFunc3
        // spelling for slot 0x44 (name-length word at +0x30). Same arity;
        // MWCC inlines it into the identical virtual dispatch.
        int CObjectParam_UnkVirtualFunc3() { return CObjectParam_hasObjectName(); } //0x44

        // Wave-37 named forwarder (non-virtual, inline): real API spelling
        // for slot 0x40 (active-param pointer). Same arity; MWCC inlines it
        // into the identical virtual dispatch. The virtual keeps its name
        // because C++ definitions live in ocUnit/CfObjectMove.cpp.
        void* CObjectParam_getParamPtr() { return CObjectParam_UnkVirtualFunc2(); }

        // Wave-63 caller alias (non-virtual, inline): slot 0x48 base clears the
        // +0x34 trailing word (CfGameManager Fv thunk). Virtual keeps its Unk
        // linker spelling for hand-built vtables; Point overrides this slot as
        // loadPointData.
        void CObjectParam_clearExtraWord() { CObjectParam_UnkVirtualFunc4(); } //0x48

        //0x0: vtable
        //0x0-10: CObjectState
        void* mPtr10;          // 0x10-0x13 (pointer stored at offset 0x10)
        u8 unk14[0x20 - 0x14]; // 0x14-0x2F
        u32 field_30;          // 0x30  - checked for non-zero by hasObjectName
        u8  unk34[4];          // 0x34..0x37  (remainder of old unk10_3[0x28])
    };
}
