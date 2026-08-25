// Auto-scaffolded catalog TU for kyoshin/code_8027513C
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/code_8027513C.hpp"
#include "libs/monolib/src/scn/CScn_8049603C.hpp" // func_8049603C (single owner decl)
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CfObjectEff.hpp"
#include <nw4r/math/math_types.h>

class CScn;

// Forward declarations with retail-accurate signatures.
// func_8008187C is declared void() in CfGameManager.hpp but retail
// takes an int type code in r3 and returns CfObjectEff*.
cf::CfObjectEff* func_8008187C(int type);
void func_800ACC14(void* self, s8 val);
void func_800ACC64(cf::CfObjectEff* obj, const float* src);
void func_800ACF78(cf::CfObjectEff* eff, cf::CfObjectMove* obj, int arg);
void func_801BFDE8(int arg0, int arg1, float* pos, float arg3, float arg4);
// func_8049603C: single owner decl in libs/monolib/src/scn/CScn_8049603C.hpp.

// Vtable helper structs for calling methods whose return type
// or signature differs from the header declaration.
struct CfObjectMove_VTable43 {
    u32 _pad[43];
    float* (*fn)(cf::CfObjectMove*);
};

struct CfObjectEff_VTable39 {
    u32 _pad[39];
    void (*fn)(cf::CfObjectEff*, float*);
};

int func_802759A8(void* self) { return 0; }

// ---------------------------------------------------------------------------
// Camera-control registration helpers: fetch the shared camera-control
// instance (initCamControlInstances) and dispatch (self) through its vtable
// slots 4..11. Retail: bl initCamControlInstances; lwz r12,0(r3);
// mr r4,r31; lwz r12,slot(r12); mtctr; bcctrl.
// ---------------------------------------------------------------------------
struct CamCtrlRegVt {
    virtual void m00(); virtual void m01();
    virtual void m04(void* arg); virtual void m05(void* arg); virtual void m06(void* arg);
    virtual void m07(void* arg); virtual void m08(void* arg); virtual void m09(void* arg);
    virtual void m0A(void* arg); virtual void m0B(void* arg);
};
extern "C" void* initCamControlInstances(void);

void func_802751F8(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m04(self); }
void func_80275238(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m05(self); }
void func_80275278(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m06(self); }
void func_802752B8(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m07(self); }
void func_802752F8(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m08(self); }
void func_80275338(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m09(self); }
void func_80275378(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m0A(self); }
void func_802753B8(void* self) { ((CamCtrlRegVt*)initCamControlInstances())->m0B(self); }










// State 0 handler: checks target data flags and either spawns a positional
// effect + sound, or creates a CfObjectEff attached to this controller.
void func_80275454(UnkCode8027513C* self) {
    cf::CfObjectMove* obj;
    UnkTargetData* target;
    UnkTargetData* reloadedTarget;
    float* pos;
    int type;
    int mode;
    cf::CfObjectEff* eff;
    UnkCamObj* camObj;
    float camVal;
    UnkPosContainer* posContainer;
    float posVec[4];
    float result[3];
    float threshold;
    u8* ptr;
    
    obj = self->field_0x14;
    target = (UnkTargetData*)obj->mTargetC4;
    
    if (target == nullptr) {
        return;
    }
    
    // Branch 1: field_0x4EC has bit 1 (0x2) set
    if (target->field_0x4EC & 2) {
        // Load threshold first to match retail scheduling
        threshold = lbl_eu_806689F0;
        self->field_0x6C++;
        
        // Only proceed if field_0x3C4 is below 0.001 threshold
        if (target->field_0x3C4 < threshold) {
            // CfObject_UnkVirtualFunc44 returns float* in retail (header says void)
            pos = (*(CfObjectMove_VTable43**)obj)->fn(obj);
            
            // Reload after vtable call (obj may have been modified)
            obj = self->field_0x14;
            if (obj != nullptr) {
                reloadedTarget = (UnkTargetData*)obj->mTargetC4;
                type = 46;  // 0x2e
                mode = 217; // 0xd9
                
                // Threshold check determines type/mode selection
                if (reloadedTarget->field_0x3C4 > lbl_eu_806689D8 ||
                    reloadedTarget->field_0x4F8 < lbl_eu_806689DC) {
                    type = 45;  // 0x2d
                    mode = 216; // 0xd8
                }
                
                eff = func_8008187C(type);
                if (eff != nullptr) {
                    // Vector addition: pos + (0.0, 0.01, 0.0) using nw4r VEC3Add
                    // which inlines paired-single instructions matching retail.
                    nw4r::math::VEC3 offset;
                    offset.x = lbl_eu_806689E0; // 0.0
                    offset.y = lbl_eu_806689E4; // 0.01
                    offset.z = lbl_eu_806689E0; // 0.0
                    
                    nw4r::math::VEC3 tmp;
                    nw4r::math::VEC3Add(&tmp, (const nw4r::math::VEC3*)pos, &offset);
                    
                    result[0] = tmp.x;
                    result[1] = tmp.y;
                    result[2] = tmp.z;
                    
                    // CfObjectEff vtable[39] takes float* in retail
                    (*(CfObjectEff_VTable39**)eff)->fn(eff, result);
                    
                    // Load position from obj->field_0x90[8] (offset 0x98)
                    posContainer = *(UnkPosContainer**)(&obj->field_0x90[8]);
                    posVec[0] = posContainer->posX;
                    posVec[1] = posContainer->posY;
                    posVec[2] = posContainer->posZ;
                    posVec[3] = lbl_eu_806689E8; // 1.0
                    func_800ACC64(eff, posVec);
                }
                
                // Play sound effect at position
                camObj = (UnkCamObj*)func_8049603C(lbl_eu_80663E14);
                camVal = lbl_eu_806689E8 - camObj->field_0xC;
                func_801BFDE8(0, mode, pos, camVal, lbl_eu_806689EC);
            }
        }
        
        // Set target float fields if field_0x90[0] is non-null
        obj = self->field_0x14;
        if (*(u32*)(&obj->field_0x90[0]) != 0) {
            target->field_0x504 = lbl_eu_806689F4;
            target->field_0x50C = lbl_eu_806689F8;
        }
    } else {
        // Branch 2: field_0x4EC bit 1 not set, check field_0xC bit 1
        if (target->field_0xC & 2) {
            // func_8007F91C ignores its 'this' pointer, so casting self works
            if (!((cf::CfGameManager*)self)->func_8007F91C()) {
                if (self->field_0x74 == nullptr) {
                    eff = func_8008187C(190); // 0xbe
                    if (eff != nullptr) {
                        // Store &self+0x10 into eff at offset 0xB0.
                        // Use ptr = self; if (ptr) ptr += 16 to match retail mr/beq/addi.
                        ptr = (u8*)self;
                        if (ptr != nullptr) {
                            ptr += 0x10;
                        }
                        ((CfObjectEffB0*)eff)->field_0xB0 = ptr;
                        
                        self->field_0x74 = eff;
                        func_800ACF78(eff, self->field_0x14, 0);
                        
                        obj = self->field_0x14;
                        obj->CfObject_UnkVirtualFunc36();
                        eff->CfObject_UnkVirtualFunc35();
                        
                        if (self->field_0x74 != nullptr && self->field_0x78 != 2) {
                            func_800ACC14(self->field_0x74, 2);
                            self->field_0x78 = 2;
                        }
                        
                        // Load position from obj->field_0x90[8] (offset 0x98)
                        posContainer = *(UnkPosContainer**)(&obj->field_0x90[8]);
                        posVec[0] = posContainer->posX;
                        posVec[1] = posContainer->posY;
                        posVec[2] = posContainer->posZ;
                        posVec[3] = lbl_eu_806689E8; // 1.0
                        func_800ACC64(eff, posVec);
                    }
                }
            }
        }
    }
}

// Create and initialize a CfObjectEff attached to this controller.
// Called as state 1 in the dispatch (field_0x6C == 1).
// Guards: requires mTargetC4, game manager check, and no existing effect.
void func_802756F0(UnkCode8027513C* self) {
    cf::CfObjectMove* obj = self->field_0x14;
    if (obj->mTargetC4 == nullptr) {
        return;
    }
    
    // func_8007F91C ignores its 'this' pointer, so casting self works
    if (((cf::CfGameManager*)self)->func_8007F91C()) {
        return;
    }
    
    if (self->field_0x74 != nullptr) {
        return;
    }
    
    cf::CfObjectEff* eff = func_8008187C(190); // 0xbe
    if (eff == nullptr) {
        return;
    }
    
    // Store &self+0x10 into eff at offset 0xB0 (address of CfObjectImplTbox subobject).
    // Use ptr = self; if (ptr) ptr += 16 to match retail mr/beq/addi pattern.
    u8* ptr = (u8*)self;
    if (ptr != nullptr) {
        ptr += 0x10;
    }
    ((CfObjectEffB0*)eff)->field_0xB0 = ptr;
    
    self->field_0x74 = eff;
    func_800ACF78(eff, self->field_0x14, 0);
    
    obj = self->field_0x14;
    obj->CfObject_UnkVirtualFunc36();
    eff->CfObject_UnkVirtualFunc35();
    
    if (self->field_0x74 != nullptr && self->field_0x78 != 2) {
        func_800ACC14(self->field_0x74, 2);
        self->field_0x78 = 2;
    }
    
    // Load position from obj->field_0x90[8] (offset 0x98)
    obj = self->field_0x14;
    UnkPosContainer* posContainer = *(UnkPosContainer**)(&obj->field_0x90[8]);
    float posVec[4];
    posVec[0] = posContainer->posX;
    posVec[1] = posContainer->posY;
    posVec[2] = posContainer->posZ;
    posVec[3] = lbl_eu_806689E8; // 1.0
    func_800ACC64(eff, posVec);
}

void func_80275808(UnkCode8027513C* ptr) {
    ptr->field_0x20 = -1;
    ptr->field_0x1C = 0;
    ptr->field_0x74 = nullptr;
    ptr->field_0x78 = 0;
}

extern "C" void func_80275824(UnkCode8027513C* self) {
    if (self->field_0x74 == 0) return;
    *(u32*)((u8*)self->field_0x74 + 0xB0) = 0;
    *(u32*)((u8*)self->field_0x74 + 0x68) |= 0x40;
    self->field_0x74 = 0;
}

// State machine: dispatch based on field_0x6C, then sync flag bit 11 between
// CfObjectMove->mFlags68 and CfObjectEff->mFlags68,
// and update child effect state via CfGameManager query.
void func_80275850(UnkCode8027513C* self) {
    // Dispatch: state 0->func_80275454, 1->func_802756F0 (originally via PTMF table)
    if (self->field_0x6C == 0) {
        func_80275454(self);
    } else if (self->field_0x6C == 1) {
        func_802756F0(self);
    }
    
    cf::CfObjectMove* obj = self->field_0x14;
    if (obj == nullptr) {
        return;
    }
    
    u32 moveFlags = obj->mFlags68;
    // Bit 25 set means effect already cleaned up - skip
    if (moveFlags & 0x40) {
        return;
    }
    
    cf::CfObjectEff* eff = self->field_0x74;
    if (eff == nullptr) {
        return;
    }
    
    // Synchronize bit 11 from move flags to effect flags
    if (moveFlags & 0x00100000) {
        eff->mFlags68 |= 0x00100000;
    } else {
        eff->mFlags68 &= ~0x00100000;
    }
    
    // If bit 11 was set, query game manager and update child effect state
    if (moveFlags & 0x00100000) {
        // func_8007F91C ignores its 'this' pointer, so casting self works
        if (((cf::CfGameManager*)self)->func_8007F91C()) {
            if (self->field_0x74 != nullptr && self->field_0x78 != 1) {
                func_800ACC14(self->field_0x74, 1);
                self->field_0x78 = 1;
            }
        } else {
            if (self->field_0x74 != nullptr && self->field_0x78 != 2) {
                func_800ACC14(self->field_0x74, 2);
                self->field_0x78 = 2;
            }
        }
    }
}

// retail: if (self->ptr74 == other) { other->b0 = 0; self->ptr74 = 0; } then tail-call func_800CFFA0
extern "C" void func_800CFFA0();
extern "C" void func_8027594C(void* self, void* other) {
    if ((u32)(uintptr_t)other == *(u32*)((char*)self + 0x74)) {
        *(u32*)((char*)other + 0xB0) = 0;
        *(u32*)((char*)self + 0x74) = 0;
    }
    func_800CFFA0();
}

// If field_0x74 is set and field_0x78 differs from arg, notify the child
// effect via func_800ACC14 and cache the new value
void func_802753F8(UnkCode8027513C* self, s16 arg) {
    if (self->field_0x74 != nullptr) {
        if (self->field_0x78 != arg) {
            func_800ACC14(self->field_0x74, (s8)arg);
            self->field_0x78 = arg;
        }
    }
}

cf::CfObjectImplTbox::~CfObjectImplTbox() {}


extern "C" void __dt__Q22cf16CfObjectImplTboxFv(void*, int);

void func_802759B0(void* self) { ((void(*)(void*))__dt__Q22cf16CfObjectImplTboxFv)((u8*)self - 0xc); }

void func_802759B8(void* self) { ((void(*)(void*))func_8027594C)((u8*)self - 0x10); }

void func_802759C0(void* self) { ((void(*)(void*))__dt__Q22cf16CfObjectImplTboxFv)((u8*)self - 0x10); }
