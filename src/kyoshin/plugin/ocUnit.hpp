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

#ifdef __cplusplus
extern "C" {
#endif

void ocUnitRegist();

#ifdef __cplusplus
}
#endif

namespace cf {

class CfObjectMove {
public:
    void CfObject_UnkVirtualFunc50();
    void CfObject_UnkVirtualFunc51();

    // TODO: add fields
};
} // namespace cf

namespace cf {

class CfObject {
public:
    void CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc34();
    void CfObject_UnkVirtualFunc30();
    void CfObject_UnkVirtualFunc29();
    void CfObject_UnkVirtualFunc26();
    void CfObject_UnkVirtualFunc50();

    // TODO: add fields
    void CfObject_UnkVirtualFunc22();
    void CfObject_UnkVirtualFunc19();
    void CfObject_UnkVirtualFunc49();
    void CfObject_UnkVirtualFunc27();
    void CfObject_UnkVirtualFunc64();
};
} // namespace cf

namespace cf {

class CObjectParam {
public:
    void CObjectParam_UnkVirtualFunc2();
    void CObjectParam_UnkVirtualFunc1();

    // TODO: add fields
    void CObjectParam_UnkVirtualFunc6();
    void CObjectParam_UnkVirtualFunc5();
};
} // namespace cf

namespace cf {

class CActorParam {
public:
    void CActorParam_UnkVirtualFunc37();

    // TODO: add fields
};
} // namespace cf

namespace cf {

class CObjectState {
public:
    void CObjectState_UnkVirtualFunc8();
    void CObjectState_UnkVirtualFunc11();
    void CObjectState_UnkVirtualFunc10();
    void CObjectState_UnkVirtualFunc9();

    // TODO: add fields
    void CObjectState_UnkVirtualFunc2();
    void CObjectState_UnkVirtualFunc3();
};
} // namespace cf

