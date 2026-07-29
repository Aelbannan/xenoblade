#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObject.hpp"

namespace cf {
    //min size: 0xbe
    class CfObjectModel : public CfObject {
    public:
        //vtable 1 (CfObject)
        virtual ~CfObjectModel();
        //vtable 1 (CfObjectModel)
        virtual void CfObjectModel_UnkVirtualFunc1();  //0x178
        virtual void CfObjectModel_UnkVirtualFunc2();  //0x17C
        virtual void CfObjectModel_UnkVirtualFunc3();  //0x180
        virtual void CfObjectModel_UnkVirtualFunc4();  //0x184
        virtual void CfObjectModel_UnkVirtualFunc5();  //0x188
        virtual void CfObjectModel_UnkVirtualFunc6();  //0x18C
        virtual void CfObjectModel_UnkVirtualFunc7();  //0x190
        virtual void CfObjectModel_UnkVirtualFunc8();  //0x194
        virtual void CfObjectModel_UnkVirtualFunc9();  //0x198
        virtual void CfObjectModel_UnkVirtualFunc10(); //0x19C
        virtual void CfObjectModel_UnkVirtualFunc11(); //0x1A0
        virtual void CfObjectModel_UnkVirtualFunc12(); //0x1A4
        virtual void CfObjectModel_UnkVirtualFunc13(); //0x1A8
        virtual void CfObjectModel_UnkVirtualFunc14(); //0x1AC
        virtual void CfObjectModel_UnkVirtualFunc15(); //0x1B0
        virtual void CfObjectModel_UnkVirtualFunc16(); //0x1B4
        virtual void CfObjectModel_UnkVirtualFunc17(); //0x1B8
        virtual void CfObjectModel_UnkVirtualFunc18(); //0x1BC
        virtual void CfObjectModel_UnkVirtualFunc19(); //0x1C0
        virtual void CfObjectModel_UnkVirtualFunc20(); //0x1C4

        //0x0: vtable
        // CfObject ends at 0x70.
        u8 field_0x70[0x1C];
        u16 unk8C_3;
        u16 field_0x8E;
        u8 field_0x90[0x20]; // 0x90-0xAF
        void* mSubObjB0;      // 0xB0-0xB3
        u8 unkB4[0xBC - 0xB4]; // 0xB4-0xBB
        u8 field_BC;          // 0xBC
        u8 field_BD;          // 0xBD
    CfObjectModel();
    void CfObject_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc8();
    void CfObject_UnkVirtualFunc63();
    void CfObject_UnkVirtualFunc19();
    void CfObject_UnkVirtualFunc22();
    void CfObject_UnkVirtualFunc20();
    u32 CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27();
    void CfObject_UnkVirtualFunc29();
    void CfObject_UnkVirtualFunc32();
    void CfObject_UnkVirtualFunc34();
    void CfObject_UnkVirtualFunc33(float amount);
    void CfObject_UnkVirtualFunc30();
    float CfObject_UnkVirtualFunc56();
    void CfObject_UnkVirtualFunc52();
    CfObject* CfObject_UnkVirtualFunc53();
    void CfObject_UnkVirtualFunc54();
    void CfObject_UnkVirtualFunc55();
    void CObjectParam_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc66();
    void CfObject_UnkVirtualFunc67();
    void CfObject_UnkVirtualFunc70(float value);
    void CfObject_UnkVirtualFunc69();
    void CfObject_UnkVirtualFunc68();
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc28();
    void CfObject_UnkVirtualFunc31();
    void CfObject_UnkVirtualFunc35();
    void CfObject_UnkVirtualFunc36();
    void CfObject_UnkVirtualFunc72();
    };
}

