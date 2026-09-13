#pragma once

#include <types.h>

namespace cf {
    //min size: 0x10
    class CObjectState {
    public:
        virtual void CObjectState_setStateBitMask(u32 bits);  //0x8
        virtual int CObjectState_checkStateFlags(int mask);  //0xC
        virtual void CObjectState_setStateBitFlag(u32 mask);  //0x10
        virtual void CObjectState_clearStateWord4();  //0x14
        virtual void CObjectState_applyStateFlags(int arg);  //0x18
        virtual void CObjectState_clearStateWord8();  //0x1C
        virtual void CObjectState_clearStateFlags8(int arg);  //0x20
        virtual int CObjectState_checkStateFlags8(int arg);  //0x24
        virtual int CObjectState_UnkVirtualFunc9();  //0x28
        virtual int CObjectState_setStateBitMask0(int mask, int flag); //0x2C
        virtual void* CObjectState_getStateData(); //0x30
        virtual void* CObjectState_setStateBitMask2(); //0x34
        virtual void CObjectState_setStateBitMask3(); //0x38

        // Wave-37 caller alias (non-virtual, inline): legacy UnkVirtualFunc8
        // spelling for the 0x24 state-word query. Same arity; forwards to
        // the renamed virtual so other TUs keep compiling unchanged.
        int CObjectState_UnkVirtualFunc8(int arg) { return CObjectState_checkStateFlags8(arg); }
        // Wave-63 mop-up aliases (non-virtual, inline): legacy UnkVirtualFunc4/6
        // spellings for the +0x14/+0x1C clear-word slots.
        void CObjectState_UnkVirtualFunc4() { CObjectState_clearStateWord4(); }
        void CObjectState_UnkVirtualFunc6() { CObjectState_clearStateWord8(); }
        // Wave-40 UnkVirtual mop-up alias (non-virtual, inline): same-slot
        // forwarder for the 0x28 +0xC-word query (ocUnit setStateBitMask0
        // gate); the virtual keeps its Unk name (hand-built vtables spell
        // the mangled name). MWCC inlines the identical virtual dispatch.
        int CObjectState_checkStateFlagsC() { return CObjectState_UnkVirtualFunc9(); }

        //0x0: vtable
        u32 unk4;          // 0x04
        u32 unk8;          // 0x08
        u32 unkC;          // 0x0C
    };
}
