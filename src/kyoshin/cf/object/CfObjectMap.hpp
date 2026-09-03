#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
class CResLookup;
class CScnEnvLgtCtrlListItem;

class CScn;
class CView;

// ---------------------------------------------------------------------------
// External imports used by CfObjectMap.cpp
//
// Several retail symbols are annotated Fv (or unmangled) although they really
// take register/float arguments (PLAN.md §17.6 relocation-name pattern), so
// the mangled names are spelled out verbatim with extern "C" linkage.
// ---------------------------------------------------------------------------

// Map resource managers returned by func_80062F18/60/A8/FF0.
struct UnkRes866A0 {
    u32 field_0x0;         // 0x00 flags
    u32 field_0x4;         // 0x04
    u32 field_0x8;         // 0x08
    u8 field_0xC[0x14];    // 0x0C-0x1F
    u32 field_0x20;        // 0x20
    u32 field_0x24;        // 0x24
    u32 field_0x28;        // 0x28
    CResLookup* field_0x2C; // 0x2C
};

// Big embedded sub-object at +0xF0 (UnkClass_8047BB54 in retail).  Only the
// flag word at 0x10 is touched directly; the rest pads up to +0x2F2C.
struct UnkMapSubF0 {
    u8 field_0x0[0x10];
    u32 flags_0x10;                  // 0x100 (bit 0x8 toggled by func_800BA66C)
    u8 field_0x14[0x2E3C - 0x14];    // 0x104-0x2F2B
};

// Node-pool manager embedded at +0x2F2C (UnkClass_8047CD0C in retail).
struct UnkMapPool2F2C {
    u8 field_0x0[0xC];
};

// View of the global scene object (lbl_eu_80663E14) used to reach its
// resource manager at +0x7C.
struct UnkSceneView {
    u8 field_0x0[0x7C];
    void* field_0x7C;   // 0x7C
};

// Global settings object returned by getUnk80664658.
struct UnkGimmickGlobalView {
    u8 field_0x0[0x210];
    u32 field_210;      // 0x210
    u32 field_214;      // 0x214
};

namespace cf {
    class CfObjectMap;
}

extern "C" {
    // ---- CTaskLOD task helpers ----
    void acquireLODResource__8CTaskLODFv(void* task, u32 value);
    void resetActiveLOD__8CTaskLODFv(void* task);
    void bindTaskToLOD__8CTaskLODFv(void* task);
    void updateLODFrame__8CTaskLODFv(float value);
    void notifyLODTick__8CTaskLODFv(float value);
    void setLODFilterFlag__8CTaskLODFv(void* task);
    void updateLODRange__8CTaskLODFv(u16 value, float amount);
    void syncLODTask__8CTaskLODFv(u32 value);

    // ---- embedded UnkClass_8047BB54 subobject (at +0xF0) ----
    void* initMpfSystem__17UnkClass_8047BB54Fv(void* self, void* r4, void* r5, void* r6);
    void setMpfFloatParam__17UnkClass_8047BB54Fv(void* self, float value);
    void forwardMpfCallA__17UnkClass_8047BB54Fv(void* self, u32 value);
    void resetMpfInstance__17UnkClass_8047BB54Fv(void* self);

    // ---- embedded UnkClass_8047CD0C node-pool manager (at +0x2F2C) ----
    void func_8047CD0C__17UnkClass_8047CD0CFv(void* self, void* r4);
    void initNodePool__17UnkClass_8047CD0CFv(void* self, void* r4, u32 r5, u32 r6);
    void clearPoolData__17UnkClass_8047CD0CFv(void* self);
    void resetPoolState__17UnkClass_8047CD0CFv(void* self);

    // ---- CfGameManager / map-effect helpers ----
    u32 getMapEffectManager__Q22cf13CfGameManagerFv();
    void func_8016FBA8(u32 manager, void* task);
    u32 func_8016E08C();
    u32 func_8016E094();

    // ---- CTaskCulling statics ----
    void* func_801A2C04__Q22cf12CTaskCullingFv();
    void func_801A2C94__Q22cf12CTaskCullingFv();
    void func_801A2CAC__Q22cf12CTaskCullingFv();

    // ---- scene / resource helpers ----
    void func_80495E60(void* ptr);
    int getFileSize__11CDeviceFileFPCc(const char* path, int flags);
    void* func_804C1BA0(void* mgr, const void* name, int flag);
    void func_804C1D7C(void* mgr, void* resource);
    void func_804C1F10(void* mgr, u16 value, float amount);
    void func_80496294(CScn* scene, float value);
    void* func_80495EB0(void* r3, void* r4, void* r5);
    void* func_80489A60(void* r3, void* r4, u32 r5, u32 r6, u32 r7, u32 r8);
    void func_800BBADC(void* r3, void* r4);
    void* getHandleMEM1__Q23mtl10MemManagerFv();

    // ---- CfRes helpers ----
    u32 func_800621A0();
    void* func_800621F4(void* r3, s16 r4, void* r5, void* r6, void* r7);
    void* func_800624A8(u32 a, u32 b, u32 c);
    void func_80062680(u32 a, u32 b, u32 c);
    u32 func_800626F4(u32 a, u32 b, u32 c, u32 d);
    UnkRes866A0* func_80062F18();
    UnkRes866A0* func_80062F60();
    UnkRes866A0* func_80062FA8();
    UnkRes866A0* func_80062FF0();
    void func_80065CA4(void* child, void* parent);
    void* func_80065D00(void* r3, void* r4);
    void* func_80065D04(void* r3, void* r4);
    void* func_800A7FBC();
    void* func_800A81FC();
    void* func_800A9534(void* r3);
    void* func_800A965C(void* r3);
    void* func_800A9784(u32 size);
    void* func_800A98A8(void* r3);
    void* func_800A9CD0();
    void func_800627BC(void* r3);
    u32 func_800AA2BC(u32 a, u32 b);
    int func_800AA33C(char* buf, u32 packed, int prefixFlag, int suffixFlag);
    void CfRes_delegateOp2(void* r3, void* r4, void* r5);
    void func_801C028C(u32 r3, u32 r4);
    // getBdatStringColumnValue: canonical decl in plugin/ocBdat.hpp.
    UnkGimmickGlobalView* getUnk80664658();

    // ---- this TU's own retail symbols (Fv-annotated / unmangled) ----
    void setMapEffectFlag__Q22cf11CfObjectMapFv(cf::CfObjectMap* self, void* task);
    int func_800B9C74(cf::CfObjectMap* self, u32 a, u32 b);
    void func_800BA440(cf::CfObjectMap* self);
    void func_800B9E4C(cf::CfObjectMap* self);
}

// ---- data symbols (global scope: no mangling, plain extern is enough) ----
extern CView* lbl_eu_80663E10;     // global scene object (secondary)
extern u32 lbl_eu_80663E28;        // global flag word
extern void* lbl_eu_806640A8;      // bdat table pointer
extern u32 lbl_eu_80664184;        // bdat row index
extern char lbl_eu_804FC538[];     // map file-name suffix
extern const float lbl_eu_80666A38;
extern const float lbl_eu_80666A3C;
extern const float lbl_eu_80666A40;
extern const float lbl_eu_80666A44;
extern const float lbl_eu_80666A48;
extern const float lbl_eu_80666A4C;
extern const float lbl_eu_80666A50;
extern const float lbl_eu_80666A54;
extern const float lbl_eu_80666A58;
extern const float lbl_eu_80666A5C;
extern const float lbl_eu_80666A60;
extern const float lbl_eu_80666A64;

namespace cf {
    //size: 0x2F50
    class CfObjectMap : public CfObjectModel {
    public:
        //0x0-BE: CfObjectModel
        u8 field_0xBE[0xDC - 0xBE];        // 0xBE-0xDB
        void* field_0xDC;                  // 0xDC
        void* field_0xE0;                  // 0xE0
        void* field_0xE4;                  // 0xE4
        void* field_0xE8;                  // 0xE8
        void* field_0xEC;                  // 0xEC
        UnkMapSubF0 field_0xF0;            // 0xF0-0x2F2B (UnkClass_8047BB54)
        UnkMapPool2F2C field_0x2F2C;       // 0x2F2C-0x2F37 (UnkClass_8047CD0C)
        u32 field_0x2F38;                  // 0x2F38 packed map id
        CScnEnvLgtCtrlListItem* field_0x2F3C; // 0x2F3C map effect object
        u8 field_0x2F40;                   // 0x2F40
        u8 field_0x2F41;                   // 0x2F41
        u8 field_0x2F42[2];                // 0x2F42-0x2F43
        u32 field_0x2F44;                  // 0x2F44
        float field_0x2F48;                // 0x2F48
        u8 field_0x2F4C[0x2F50 - 0x2F4C];  // 0x2F4C-0x2F4F

        void* checkTarget();
        void cleanupMap();
        void setMapVisibility(u32 arg);
        void setMapId(unsigned long v);
        void dispatchMapState();
        virtual ~CfObjectMap();
        bool isObjectMapReady();
        void clearStatus();
        void setMapScale();
        void nopMap();
    };
} // namespace cf
