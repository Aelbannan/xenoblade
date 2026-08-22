#pragma once

#include <types.h>
#include <revolution/GX.h>
#include "monolib/math/CMat34.hpp"
#include "monolib/math/CCol4.hpp"

// novtable: retail vtables stored manually (stored by ctor, not compiler)
class __declspec(novtable) CGXCache {
public:
    CGXCache();
    virtual ~CGXCache();
    void func_80449D68();
    void func_8044A578();
    void func_8044A6C8(int param1, int param2);
    void func_8044A7F8();
    void func_8044A94C(int param1, int param2);
    void func_8044AA7C(int param1, int param2);
    void func_8044ABAC();
    void func_8044ACDC(const ml::CCol4& color, int flag);
    void func_8044AE8C(const ml::CCol4& color, int flag);
    void func_8044B03C(int param);
    void func_8044B168(int param);
    void func_8044B298();
    bool func_8044B4B8(GXTexObj* texObj, u16 w, u16 h);
    void func_8044B5B4();
    void func_8044B5C0();
    void func_8044B660();
    void func_8044B8CC(f32 fovy, f32 znear, f32 zfar);
    void func_8044BB20();
    s32 func_8044BD74(s32 param);
    void func_8044BE10();
    void func_8044BE2C();
    void func_8044BE3C();
    void func_8044BFC0();
    void func_8044C034();
    void func_8044C1FC();
    void func_8044CE68();
    void func_8044CEF8();
    void func_8044CF74(u32 id);

    // TODO: add fields
    void func_8044B294();
    void func_8044BE1C();
    void func_8044BE24();
    void func_8044BE38();
};

class __declspec(novtable) IStateCache {
public:
    virtual ~IStateCache();
    IStateCache();

    // TODO: add fields
};

class __declspec(novtable) CMsgParam_32 {
public:
    CMsgParam_32();

    // TODO: add fields
    virtual ~CMsgParam_32();
};

