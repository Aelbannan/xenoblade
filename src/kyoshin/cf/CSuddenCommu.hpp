#pragma once

#include <types.h>
#include "kyoshin/cf/object/IObjectInfo.hpp"

namespace cf{
    //size: 0x30?
    class CSuddenCommu : public IObjectInfo {
    public:
        CSuddenCommu();
        virtual ~CSuddenCommu(){}
        virtual void IObjectInfo_UnkVirtualFunc1();
        void func_801BA1DC();

        //0x0: vtable
        //0x0-4: IObjectInfo?
        /* 0x04 */ s16 field_4;
        /* 0x06 */ s16 field_6;
        /* 0x08 */ s16 field_8;
        /* 0x0A */ s16 field_A;
        /* 0x0C */ u8 pad_C[0x10 - 0xC];
        /* 0x10 */ u32 field_10; // voice id probed via func_802A3748
        /* 0x14 */ u32 field_14;
        /* 0x18 */ u8 unk18[0x24 - 0x18];
        /* 0x24 */ volatile u32 field_24;
        /* 0x28 */ u32 unk28;
        /* 0x2C */ float unk2C;
    };
}

// C-linkage callees defined in CSuddenCommu.cpp (retail symbols are unmangled
// global func_* names - the declarations below give the definitions C linkage
// so the object emits the exact retail symbol names).
extern "C" {
    void func_801BA490(cf::CSuddenCommu* self);
    void func_801BA978(cf::CSuddenCommu* self);
    void func_801BC6A4(cf::CSuddenCommu* self, int val, int num);
    // Imported voice-node lookup (defined in voice/CCharVoiceMan.cpp): returns
    // 0 when no voice node is registered for the given voice id.
    int func_802A3748(u32 arg);
}
