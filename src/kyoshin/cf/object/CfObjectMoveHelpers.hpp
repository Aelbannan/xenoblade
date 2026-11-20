#pragma once

#include <types.h>

namespace cf {
class CfObjectMove;
void CfResObj_noop2C();
}

// Real C++ linkage helpers defined in CfObjectMove.cpp (retail symbols carry
// these manglings). Callers include this owning header instead of keeping
// per-TU extern "C" copies with invented call shapes.
void CfObjectMove_detachMoveRegion(cf::CfObjectMove* self);
void CfObjectMove_forwardNpcSignal(cf::CfObjectMove* self);
void CfObjectMove_setModelDisplayFlag(cf::CfObjectMove* self, int flag);
void CfObjectMove_setModelListLock(cf::CfObjectMove* self, int flag);
const char* CfObjectMove_getBdatNameCol7(cf::CfObjectMove* self, u32 index);
const char* CfObjectMove_getBdatNameCol11(cf::CfObjectMove* self, u32 index);
