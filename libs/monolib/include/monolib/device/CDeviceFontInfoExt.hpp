#pragma once

#include <types.h>

class CDeviceFontInfoExt {
public:
    CDeviceFontInfoExt();
    virtual ~CDeviceFontInfoExt();
    void func_80453468();
    void create();
    void func_804535C0();
    void func_804535DC();
    void func_804535F4();
    void func_80453608();

    u32 field_04;
    void* mFontData;  //written from allocate_tail void*
    void* mFontInfo;
    u16 mState;
    u16 mMode;
    u8 _14[0x44];
    u32 field_58;
    u16 field_5C;
    u16 _5E;
    u32 field_60;

    void func_80453624();
    void func_8045362C();
    void func_80453634();
    void func_8045363C();
    void func_80453644();
    void func_8045364C();
    void func_80453654();
};
