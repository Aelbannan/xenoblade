// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_state
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <nw4r/g3d.h>
#include <nw4r/math.h>

namespace nw4r {
namespace g3d {
namespace detail {

#include <nw4r/g3d/detail/g3d_transform_ps.inl>

} // namespace detail
} // namespace g3d
} // namespace nw4r

extern "C" GXRenderModeObj lbl_eu_8061F9D4;
extern "C" char lbl_eu_8061A750[];
extern "C" char lbl_eu_8061FAB8[];
extern "C" char lbl_eu_80665458[1];

struct FogState {
    u32 flags;                  // at 0x0
    s32 curFogID;               // at 0x4
    nw4r::g3d::FogData fogArray[0x20]; // at 0x8
};

extern "C" FogState lbl_eu_8061AF60;

namespace nw4r {
namespace g3d {

LightObj::LightObj() : mFlag(0) {}

const GXRenderModeObj* G3DState::GetRenderModeObj() {
    return &lbl_eu_8061F9D4;
}

const math::MTX33* G3DState::GetViewNrmMtxPtr(u32 idx) {
    math::MTX33* pNrmMtx =
        *reinterpret_cast<math::MTX33**>(lbl_eu_8061A750 + 4);

    if (pNrmMtx != NULL) {
        return &pNrmMtx[idx];
    }

    if (!lbl_eu_80665458[0]) {
        lbl_eu_80665458[0] = 1;
    }

    math::MTX34* pViewMtx =
        *reinterpret_cast<math::MTX34**>(lbl_eu_8061A750);

    detail::CalcViewNrmMtx(reinterpret_cast<math::MTX33*>(lbl_eu_8061FAB8),
                           pViewMtx != NULL ? &pViewMtx[idx] : NULL);

    return reinterpret_cast<const math::MTX33*>(lbl_eu_8061FAB8);
}

void G3DState::IndMtxOpStd::Reset() {
    PSMTXIdentity(mIndMtx[0]);
    PSMTXIdentity(mIndMtx[1]);
    PSMTXIdentity(mIndMtx[2]);
    mIsValidMtx[2] = false;
    mIsValidMtx[1] = false;
    mIsValidMtx[0] = false;
}

void G3DState::SetRenderModeObj(const GXRenderModeObj& rObj) {
    lbl_eu_8061F9D4 = rObj;
}

void G3DState::SetViewPosNrmMtxArray(const math::MTX34* pViewPosMtxArray,
                                     const math::MTX33* pViewNrmMtxArray,
                                     const math::MTX34* pViewEnvTexMtxArray) {
    *reinterpret_cast<const math::MTX34**>(lbl_eu_8061A750) = pViewPosMtxArray;
    *reinterpret_cast<const math::MTX33**>(lbl_eu_8061A750 + 4) = pViewNrmMtxArray;
    *reinterpret_cast<const math::MTX34**>(lbl_eu_8061A750 + 8) = pViewEnvTexMtxArray;

    if (pViewPosMtxArray != NULL) {
        GXSetArray(GX_POS_MTX_ARRAY, pViewPosMtxArray, 0x30);
    }

    math::MTX33* pNrm =
        *reinterpret_cast<math::MTX33**>(lbl_eu_8061A750 + 4);

    if (pNrm != NULL) {
        GXSetArray(GX_NRM_MTX_ARRAY, pNrm, 0x24);
    }

    math::MTX34* pEnv =
        *reinterpret_cast<math::MTX34**>(lbl_eu_8061A750 + 8);

    if (pEnv != NULL) {
        GXSetArray(GX_TEX_MTX_ARRAY, pEnv, 0x30);
    }
}

void G3DState::SetFog(Fog fog, int id) {
    FogData* pData = fog.ptr();

    if (id < 0 || id >= 0x20 || pData == NULL) {
        return;
    }

    if (lbl_eu_8061AF60.curFogID != id ||
        memcmp(pData, &lbl_eu_8061AF60.fogArray[id], 0x30) != 0) {
        lbl_eu_8061AF60.flags = 0;
        fog.CopyTo(&lbl_eu_8061AF60.fogArray[id]);
    }
}

void G3DState::LoadFog(int id) {
    if ((lbl_eu_8061AF60.flags & 1) && (lbl_eu_8061AF60.flags & 2) &&
        id == lbl_eu_8061AF60.curFogID) {
        return;
    }

    if (id < 0 || id >= 0x20) {
        GXColor color = {0, 0, 0, 0};
        GXSetFog(GX_FOG_NONE, color, 0.0f, 0.0f, 0.0f, 0.0f);
    } else {
        Fog fog(&lbl_eu_8061AF60.fogArray[id]);
        fog.SetGP();
    }

    lbl_eu_8061AF60.curFogID = id;
    lbl_eu_8061AF60.flags |= 3;
}

} // namespace g3d
} // namespace nw4r

void EnvironmentMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc(){}

void ProjectionMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc(){}

void EnvironmentSpecularMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc(){}

void DefaultMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc(){}

void FifoSend__Q44nw4r3g3d8G3DState13IndTexMtxInfoCFv(){}

void __dt__Q34nw4r3g3d12LightSettingFv(){}

void __dt__Q34nw4r3g3d8LightObjFv(){}

void LoadResMatMisc__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResMatMisc(){}

void LoadResTexObj__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResTexObj(){}

void LoadResTlutObj__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResTlutObj(){}

void LoadResGenMode__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResGenMode(){}

void LoadResTev__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResTev(){}

void LoadResMatPix__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResMatPix(){}

void LoadResMatTevColor__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d14ResMatTevColor(){}

void LoadResMatIndMtxAndScale__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d20ResMatIndMtxAndScale(){}

void LoadResMatIndMtxAndScale__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d20ResMatIndMtxAndScaleRQ44nw4r3g3d8G3DState8IndMtxOp(){}

void LoadResMatChan__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResMatChanUlUlUlUl8_GXColorb(){}

void LoadResMatTexCoordGen__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d17ResMatTexCoordGen(){}

void LoadResTexSrt__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResTexSrt(){}

void LoadResShpPrePrimitive__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResShp(){}

void LoadResShpPrimitive__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResShpPCQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34(){}

void SetViewPosNrmMtxArray__Q34nw4r3g3d8G3DStateFPCQ34nw4r4math5MTX34PCQ34nw4r4math5MTX33PCQ34nw4r4math5MTX34(){}

void GetViewNrmMtxPtr__Q34nw4r3g3d8G3DStateFUl(){}

u32 GetIndMtxOp__Q34nw4r3g3d8G3DStateFv(void) {
    extern u32 lbl_eu_8066346C;
    return lbl_eu_8066346C;
}

void SetFog__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d3Fogi(){}

void LoadFog__Q34nw4r3g3d8G3DStateFi(){}

void SetLightSetting__Q34nw4r3g3d8G3DStateFRCQ34nw4r3g3d12LightSetting(){}



// Light object array (retail global at 8061B574).
extern char lbl_eu_8061B574[];

void* GetLightObj__Q34nw4r3g3d8G3DStateFi(int index) {
    if (index < 0 || index >= 128) return nullptr;
    return &lbl_eu_8061B574[index * 0x44 + 0x2c];
}
void SetLightObj__Q34nw4r3g3d8G3DStateFRCQ34nw4r3g3d8LightObji(){}

namespace nw4r {
namespace g3d {

void G3DState::SetAmbLightObj(const AmbLightObj& rObj, int idx) {
    if (idx < 0) {
        return;
    }
    if (idx >= 0x80) {
        return;
    }
    AmbLightObj* dst = reinterpret_cast<AmbLightObj*>(lbl_eu_8061B574 + idx * 4 + 0x222c);
    *dst = rObj;
}

} // namespace g3d
} // namespace nw4r

void LoadLightSet__Q34nw4r3g3d8G3DStateFiPUlPUlPUlPUlPQ34nw4r3g3d11AmbLightObj(){}

void SetCameraProjMtx__Q34nw4r3g3d8G3DStateFRCQ34nw4r3g3d6Cameraib(){}

extern const unsigned char lbl_eu_8061DFA0[];

const void* GetCameraMtxPtr__Q34nw4r3g3d8G3DStateFv() {
    const unsigned char* base = lbl_eu_8061DFA0;
    unsigned short index = *(const unsigned short*)(base + 2);
    return base + index * 0x30 + 0x34;
}

void SetRenderModeObj__Q34nw4r3g3d8G3DStateFRC16_GXRenderModeObj(){}

void Invalidate__Q34nw4r3g3d8G3DStateFUl(){}

void SetNrmMapMtx__Q44nw4r3g3d8G3DState11IndMtxOpStdF14_GXIndTexMtxIDPCQ34nw4r4math4VEC3PCQ34nw4r4math5MTX34Q44nw4r3g3d14ResMatMiscData14IndirectMethod(){}

void Reset__Q44nw4r3g3d8G3DState11IndMtxOpStdFv(){}

void __cl__Q44nw4r3g3d8G3DState11IndMtxOpStdFPQ44nw4r3g3d8G3DState13IndTexMtxInfo(){}

void __dt__Q44nw4r3g3d8G3DState11IndMtxOpStdFv(){}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace nw4r { namespace g3d { class LightSetting; }}
namespace nw4r { namespace g3d { class LightObj; }}
namespace nw4r { namespace g3d { class AmbLightObj; }}
namespace nw4r {
    namespace g3d {
        namespace G3DState {
            namespace {
                struct CameraMtxState {
                    u16 mFlag;                 // at 0x0
                    u16 mCurCameraID;          // at 0x2
                    math::MTX34 mViewMtx;      // at 0x4
                    math::MTX34 mViewMtxArray[0x20];  // at 0x34
                    math::MTX44 mProjMtxArray[0x20];  // at 0x634
                    math::MTX34 mNrmMtxArray[0x20];   // at 0xE34
                    math::MTX34 mEnvMtxArray[0x20];   // at 0x1434

                    CameraMtxState();
                };

                CameraMtxState::CameraMtxState() {
                    mFlag = 0;
                    mCurCameraID = 0;
                    PSMTXIdentity(mViewMtx);

                    for (u32 i = 0; i < 0x20; i++) {
                        PSMTXIdentity(mViewMtxArray[i]);
                        MTX44Identity(&mProjMtxArray[i]);
                        PSMTXIdentity(mNrmMtxArray[i]);
                        PSMTXIdentity(mEnvMtxArray[i]);
                    }
                }
            }
        }
    }
}
namespace nw4r {
    namespace g3d {
        namespace G3DState {
            namespace {
                class LightState {
                    public:
                    void SetLightSetting(const nw4r::g3d::LightSetting&);
                    void SetLightObj(const nw4r::g3d::LightObj&, int);
                    void LoadLightSet(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*, nw4r::g3d::AmbLightObj*);
                    ~LightState();
                };
                void LightState::SetLightSetting(const nw4r::g3d::LightSetting&) {}
                void LightState::SetLightObj(const nw4r::g3d::LightObj&, int) {}
                void LightState::LoadLightSet(int, unsigned long*, unsigned long*, unsigned long*, unsigned long*, nw4r::g3d::AmbLightObj*) {}
                LightState::~LightState() {}
            }

        }
    }
}
