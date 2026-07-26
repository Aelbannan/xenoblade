// Auto-scaffolded catalog TU for nw4r/src/g3d/g3d_state
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern "C" void EnvironmentMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc() {}

extern "C" void ProjectionMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc() {}

extern "C" void EnvironmentSpecularMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc() {}

extern "C" void DefaultMapping__Q44nw4r3g3d6detail19ScnDependentMtxFuncFPQ34nw4r4math5MTX34ScSc() {}

extern "C" void FifoSend__Q44nw4r3g3d8G3DState13IndTexMtxInfoCFv() {}

extern "C" void __dt__Q34nw4r3g3d12LightSettingFv() {}

extern "C" void __ct__Q34nw4r3g3d8LightObjFv(void) {}

extern "C" void __dt__Q34nw4r3g3d8LightObjFv() {}

extern "C" void LoadResMatMisc__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResMatMisc() {}

extern "C" void LoadResTexObj__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResTexObj() {}

extern "C" void LoadResTlutObj__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResTlutObj() {}

extern "C" void LoadResGenMode__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResGenMode() {}

extern "C" void LoadResTev__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResTev() {}

extern "C" void LoadResMatPix__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResMatPix() {}

extern "C" void LoadResMatTevColor__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d14ResMatTevColor() {}

extern "C" void LoadResMatIndMtxAndScale__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d20ResMatIndMtxAndScale() {}

extern "C" void LoadResMatIndMtxAndScale__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d20ResMatIndMtxAndScaleRQ44nw4r3g3d8G3DState8IndMtxOp() {}

extern "C" void LoadResMatChan__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d10ResMatChanUlUlUlUl8_GXColorb() {}

extern "C" void LoadResMatTexCoordGen__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d17ResMatTexCoordGen() {}

extern "C" void LoadResTexSrt__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d9ResTexSrt() {}

extern "C" void LoadResShpPrePrimitive__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResShp() {}

extern "C" void LoadResShpPrimitive__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d6ResShpPCQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34() {}

extern "C" void SetViewPosNrmMtxArray__Q34nw4r3g3d8G3DStateFPCQ34nw4r4math5MTX34PCQ34nw4r4math5MTX33PCQ34nw4r4math5MTX34() {}

extern "C" void GetViewNrmMtxPtr__Q34nw4r3g3d8G3DStateFUl() {}

extern "C" u32 GetIndMtxOp__Q34nw4r3g3d8G3DStateFv(void) {
    extern u32 lbl_eu_8066346C;
    return lbl_eu_8066346C;
}

extern "C" void SetFog__Q34nw4r3g3d8G3DStateFQ34nw4r3g3d3Fogi() {}

extern "C" void LoadFog__Q34nw4r3g3d8G3DStateFi() {}

extern "C" void SetLightSetting__Q34nw4r3g3d8G3DStateFRCQ34nw4r3g3d12LightSetting() {}

extern "C" void SetLightObj__Q34nw4r3g3d8G3DStateFRCQ34nw4r3g3d8LightObji() {}

extern "C" void* GetLightObj__Q34nw4r3g3d8G3DStateFi(int index) {
    if (index < 0 || index >= 128) return nullptr;
    extern char lbl_eu_8061B574[];
    return &lbl_eu_8061B574[index * 0x44 + 0x2c];
}

extern "C" void SetAmbLightObj__Q34nw4r3g3d8G3DStateFRCQ34nw4r3g3d11AmbLightObji() {}

extern "C" void LoadLightSet__Q34nw4r3g3d8G3DStateFiPUlPUlPUlPUlPQ34nw4r3g3d11AmbLightObj() {}

extern "C" void SetCameraProjMtx__Q34nw4r3g3d8G3DStateFRCQ34nw4r3g3d6Cameraib() {}

extern "C" const unsigned char lbl_eu_8061DFA0[];

extern "C" const void* GetCameraMtxPtr__Q34nw4r3g3d8G3DStateFv() {
    const unsigned char* base = lbl_eu_8061DFA0;
    unsigned short index = *(const unsigned short*)(base + 2);
    return base + index * 0x30 + 0x34;
}

extern "C" void SetRenderModeObj__Q34nw4r3g3d8G3DStateFRC16_GXRenderModeObj() {}

extern "C" void* GetRenderModeObj__Q34nw4r3g3d8G3DStateFv(void) { return 0; }

extern "C" void Invalidate__Q34nw4r3g3d8G3DStateFUl() {}

extern "C" void SetNrmMapMtx__Q44nw4r3g3d8G3DState11IndMtxOpStdF14_GXIndTexMtxIDPCQ34nw4r4math4VEC3PCQ34nw4r4math5MTX34Q44nw4r3g3d14ResMatMiscData14IndirectMethod() {}

extern "C" void Reset__Q44nw4r3g3d8G3DState11IndMtxOpStdFv() {}

extern "C" void __cl__Q44nw4r3g3d8G3DState11IndMtxOpStdFPQ44nw4r3g3d8G3DState13IndTexMtxInfo() {}

extern "C" void __dt__Q44nw4r3g3d8G3DState11IndMtxOpStdFv() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
namespace nw4r { namespace g3d { class LightSetting; }}
namespace nw4r { namespace g3d { class LightObj; }}
namespace nw4r { namespace g3d { class AmbLightObj; }}
namespace nw4r {
    namespace g3d {
        namespace G3DState {
            namespace {
                class CameraMtxState {
                    public:
                    CameraMtxState();
                };
                CameraMtxState::CameraMtxState() {}
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
