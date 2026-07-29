#pragma once

#include <types.h>
namespace cf {

class CfObjectPoint {
public:
    virtual ~CfObjectPoint();
    void func_800C1638();
    int func_800AC604();

    u8 pad04[0x68 - 0x4];
    u32 field_0x68;
};

class CfObjectColl : public CfObjectPoint {
public:
    virtual ~CfObjectColl();
    void func_800AB3EC();
    void func_800AB498();
    void func_800AB57C();
    void func_800AB798();
    void func_800AB7A8();
};

class CfObject {
public:
    void CfObject_UnkVirtualFunc67();
    void CfObject_UnkVirtualFunc65();
    int CfObject_UnkVirtualFunc11();
    void CfObject_UnkVirtualFunc10();

    u8 pad04[0x6C - 0x4];
    u32 field_0x6C;
    void CfObject_UnkVirtualFunc73();
    void CfObject_UnkVirtualFunc72();
    void CfObject_UnkVirtualFunc71();
    void CfObject_UnkVirtualFunc70();
    void CfObject_UnkVirtualFunc69();
    void CfObject_UnkVirtualFunc62();
    void CfObject_UnkVirtualFunc61();
    void CfObject_UnkVirtualFunc60();
    void CfObject_UnkVirtualFunc59();
    void CfObject_UnkVirtualFunc57();
    void CfObject_UnkVirtualFunc51();
    void CfObject_UnkVirtualFunc45();
    void CfObject_UnkVirtualFunc42();
    void CfObject_UnkVirtualFunc43();
    void CfObject_UnkVirtualFunc44();
    void CfObject_UnkVirtualFunc39();
    void CfObject_UnkVirtualFunc40();
    void CfObject_UnkVirtualFunc41();
    void CfObject_UnkVirtualFunc38();
    void CfObject_UnkVirtualFunc37();
    void CfObject_UnkVirtualFunc36();
    void CfObject_UnkVirtualFunc35();
    void CfObject_UnkVirtualFunc28();
    void CfObject_UnkVirtualFunc24();
    void CfObject_UnkVirtualFunc21();
    void CfObject_UnkVirtualFunc18();
    void CfObject_UnkVirtualFunc17();
    void CfObject_UnkVirtualFunc16();
    void CfObject_UnkVirtualFunc15();
    void CfObject_UnkVirtualFunc14();
    void CfObject_UnkVirtualFunc13();
    void CfObject_UnkVirtualFunc12();
    void CfObject_UnkVirtualFunc5();
};
} // namespace cf
