#pragma once

#include <monolib/work/CProcess.hpp>
#include <monolib/scn/IScnRender.hpp>

/*
 * Shop sell menu screen process.
 * IScnRender subobject is at offset 0x58.
 */
class CMenuShopSell;

extern "C" void __ct__CMenuShopSell();
extern "C" void __dt__13CMenuShopSellFv(void* self);
extern "C" void Init__13CMenuShopSellFv();
extern "C" void Term__13CMenuShopSellFv();
extern "C" void Move__13CMenuShopSellFv();
extern "C" void cbRenderBefore__13CMenuShopSellFv(void* self);
extern "C" void func_8018B0FC();
extern "C" void func_8018B130();
extern "C" void func_8018B324();
extern "C" void func_8018B398();
extern "C" void func_8018B3A8();
extern "C" void func_8018B420();
extern "C" void func_8018B470();
extern "C" void func_8018B658();

class CMenuShopSell : public CProcess, public IScnRender {
public:
    CMenuShopSell();
    virtual ~CMenuShopSell();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    void cbRenderBefore();

    // IScnRender vtable this-adjusting thunks
    void func_8018B6A8();
    void func_8018B6B0();

    // TODO: add fields
};

