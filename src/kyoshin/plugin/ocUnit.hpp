#pragma once

/**
 * Object-container / unit plugin + CfObject virtuals that live in this TU.
 * Class layouts and virtuals come from the recovered cf object headers.
 */

#include <cstring>

#include "monolib/vm/yvm2.h"
#include "kyoshin/code_801862C0.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void ocUnitRegist();
void func_8013D07C(void* obj, const char* str, int mode);
void func_8013D448(void* obj, const char* str);
unsigned int func_8013EC58();
int func_80174C98(void* player, u32* val, int mode);
u8 code80135FDC_getByte_64058();
unsigned int func_8013EC58();

#ifdef __cplusplus
}
#endif



