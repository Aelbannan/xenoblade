#pragma once

// Forward declaration for adjusting thunks.
// Full class definition TBD as more functions are decompiled.
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

// Adjusting thunks (virtual dispatch from secondary base at offset 0x58)
extern "C" void func_8018B6A8(CMenuShopSell* self);
extern "C" void func_8018B6B0(CMenuShopSell* self);
