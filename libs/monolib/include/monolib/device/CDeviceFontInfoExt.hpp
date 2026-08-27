#pragma once

#include <types.h>

class CDeviceFontInfoExt {
public:
    CDeviceFontInfoExt();
    virtual ~CDeviceFontInfoExt();
    void func_80453468();
    void create();
    void advanceState();
    void initState();
    void isStateNonZero();
    void isStateReady();

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

    void* getFont();
    void* getFontConst();
    u16 getLineHeight();
    u32 getBufferSize();
    u16 getMode();
    u16 getState();
    u32 getFlags();
};
