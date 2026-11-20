// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_calcworld
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d/g3d_calcworld.h>

namespace nw4r {
namespace g3d {

void CalcWorld(math::MTX34* pModelMtxArray, u32* pModelMtxAttribArray,
               const u8* pByteCode, const math::MTX34* pBaseMtx, ResMdl mdl,
               AnmObjChr* pAnmChr, FuncObjCalcWorld* pFuncObj, u32 rootAttrib) {
}

void ICalcWorldCallback::ExecCallbackA(ChrAnmResult* pResult, ResMdl mdl,
                                       FuncObjCalcWorld* pFuncObj) {}

void ICalcWorldCallback::ExecCallbackB(WorldMtxManip* pManip, ResMdl mdl,
                                       FuncObjCalcWorld* pFuncObj) {}

void ICalcWorldCallback::ExecCallbackC(math::MTX34* pMtxArray, ResMdl mdl,
                                       FuncObjCalcWorld* pFuncObj) {}

void CalcSkinning(math::MTX34* pModelMtxArray, u32* pModelMtxAttribArray,
                  const ResMdl mdl, const u8* pByteCode) {
}

} // namespace g3d
} // namespace nw4r
